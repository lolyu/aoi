# epoll
* demo code to create a `epoll` fd and add one socket to the `epoll` fd:
```c
    // Create epoll instance
    if ((epoll_fd = epoll_create1(0)) == -1) {
        perror("epoll_create1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Add server socket to epoll
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
```
* epoll related structures will be like:
![image](https://github.com/user-attachments/assets/9d50b990-241f-4ba1-b402-15f80d03b988)

* the difference with `recv_from` on a blocking socket:
    * with `recv_from` on a blocking socket, the current thread will be inserted into the socket waiting queue with wakeup callback as `autoremove_wake_function`.
        * once there is data available, `autoremove_wake_function` will be called: wake up one waiting thread and remove the thread from the waiting queue.
    * with `epoll`, after the socket is added to the `epoll` fd, a mocking wait item (`eppoll_item`) is inserted into the socket waiting queue with wakeup callback as `ep_poll_callback`
        * once there is data available, `ep_poll_callback` will be called, what happened?

## what happens when a thread calls `epoll_wait`?
* `epoll_wait` basically checks if there are ready events in the `eventpoll` ready list:
	* if yes, return
	* if not, put current thread into the `eventpoll` wait list and yield the CPU.
 		* the wakeup callback is `default_wake_function`

```c
SYSCALL_DEFINE4(epoll_wait, int, epfd, struct epoll_event __user *, events,
		int, maxevents, int, timeout)
{
	int error;
	struct fd f;
	struct eventpoll *ep;

	...

	/* Time to fish for events ... */
	error = ep_poll(ep, events, maxevents, timeout);

	...
}

static int ep_poll(struct eventpoll *ep, struct epoll_event __user *events,
		   int maxevents, long timeout)
{
	wait_queue_t wait;

	...

fetch_events:
	spin_lock_irqsave(&ep->lock, flags);

	if (!ep_events_available(ep)) {
		/*
		 * We don't have any available event to return to the caller.
		 * We need to sleep here, and we will be wake up by
		 * ep_poll_callback() when events will become available.
		 */
		init_waitqueue_entry(&wait, current);									// if no ready event, init the wait entry with this thread, and use default_wake_function as wakeup callback
		__add_wait_queue_exclusive(&ep->wq, &wait);

		for (;;) {
			/*
			 * We don't want to sleep if the ep_poll_callback() sends us
			 * a wakeup in between. That's why we set the task state
			 * to TASK_INTERRUPTIBLE before doing the checks.
			 */
			set_current_state(TASK_INTERRUPTIBLE);

			...

			if (!schedule_hrtimeout_range(to, slack, HRTIMER_MODE_ABS))			// yield the CPU
				timed_out = 1;

			...
		}
		__remove_wait_queue(&ep->wq, &wait);									// wakeup again, remove current thread from the eventpoll wait list

		set_current_state(TASK_RUNNING);
	}
check_events:
	/* Is it worth to try to dig for events ? */
	eavail = ep_events_available(ep);

	...

	/*
	 * Try to transfer events to user space. In case we get 0 events and
	 * there's still timeout left over, we go trying again in search of
	 * more luck.
	 */
	if (!res && eavail &&
	    !(res = ep_send_events(ep, events, maxevents)) && !timed_out)			// transfer ready events to userspace
		goto fetch_events;

	return res;
}
```

### how does `epoll_wait` sends the events to user?
```c
static int ep_send_events(struct eventpoll *ep,
			  struct epoll_event __user *events, int maxevents)
{
	struct ep_send_events_data esed;

	esed.maxevents = maxevents;
	esed.events = events;

	return ep_scan_ready_list(ep, ep_send_events_proc, &esed, 0);
}

static int ep_scan_ready_list(struct eventpoll *ep,
			      int (*sproc)(struct eventpoll *,
					   struct list_head *, void *),
			      void *priv,
			      int depth)
{
	int error, pwake = 0;
	...
	LIST_HEAD(txlist);

	/*
	 * Steal the ready list, and re-init the original one to the
	 * empty list. Also, set ep->ovflist to NULL so that events
	 * happening while looping w/out locks, are not lost. We cannot
	 * have the poll callback to queue directly on ep->rdllist,
	 * because we want the "sproc" callback to be able to do it
	 * in a lockless way.
	 */
	list_splice_init(&ep->rdllist, &txlist);

	...
	error = (*sproc)(ep, &txlist, priv);
	...
}

static int ep_send_events_proc(struct eventpoll *ep, struct list_head *head,
			       void *priv)
{
	struct ep_send_events_data *esed = priv;
	int eventcnt;
	unsigned int revents;
	struct epitem *epi;
	struct epoll_event __user *uevent;
	struct wakeup_source *ws;
	poll_table pt;

	init_poll_funcptr(&pt, NULL);

	for (eventcnt = 0, uevent = esed->events;
	     !list_empty(head) && eventcnt < esed->maxevents;) {
		epi = list_first_entry(head, struct epitem, rdllink);

		...

		revents = ep_item_poll(epi, &pt);

		/*
		 * If the event mask intersect the caller-requested one,
		 * deliver the event to userspace. Again, ep_scan_ready_list()
		 * is holding "mtx", so no operations coming from userspace
		 * can change the item.
		 */
		if (revents) {
			if (__put_user(revents, &uevent->events) ||
			    __put_user(epi->event.data, &uevent->data)) {
				list_add(&epi->rdllink, head);
				ep_pm_stay_awake(epi);
				return eventcnt ? eventcnt : -EFAULT;
			}
			eventcnt++;
			uevent++;
			if (epi->event.events & EPOLLONESHOT)
				epi->event.events &= EP_PRIVATE_BITS;
			else if (!(epi->event.events & EPOLLET)) {						// if level triggerd, put the socket back to the ready list
				/*
				 * If this file has been added with Level
				 * Trigger mode, we need to insert back inside
				 * the ready list, so that the next call to
				 * epoll_wait() will check again the events
				 * availability. At this point, no one can insert
				 * into ep->rdllist besides us. The epoll_ctl()
				 * callers are locked out by
				 * ep_scan_ready_list() holding "mtx" and the
				 * poll callback will queue them in ep->ovflist.
				 */
				list_add_tail(&epi->rdllink, &ep->rdllist);
				ep_pm_stay_awake(epi);
			}
		}
	}

	return eventcnt;
}
```

## what happens when the socket has data to read?
* `ep_poll_callback` will be called:
	* put current `epitem` to the `eventpoll` ready list
	* calls one wait thread's wakeup callback, which is `default_wake_function`
```c
static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
	struct epitem *epi = ep_item_from_wait(wait);
	struct eventpoll *ep = epi->ep;

    ...

	if (!ep_is_linked(&epi->rdllink)) {
		list_add_tail(&epi->rdllink, &ep->rdllist);                                // add the epitem to the eventpoll ready list
	}

	/*
	 * Wake up ( if active ) both the eventpoll wait list and the ->poll()
	 * wait list.
	 */
	if (waitqueue_active(&ep->wq))
		wake_up_locked(&ep->wq);

    ...
}
```

## summary

![image](https://github.com/user-attachments/assets/836c0847-544d-4686-b241-0ddfb3b30fb4)

### FAQ

#### why `epoll` is efficient compared to blocking I/O?
* **for blocking I/O, if there is no data, the thread will yield the CPU**.
	* two context switch, one for the thread yield the CPU, one for the I/O ready and the thread is rescheduled.
* `epoll` enables the thread/process to focus on the I/O without unneccessary context switch.
	* as long as there are many sockets to work with, the thread calling `epoll_wait` will unlikely to wait and yield the CPU, it will be busy handling with the ready I/O events.

#### what's the purpose of red-black tree used in `epoll`?
* the red-black tree is used to manage the sockets added to the `eventpoll`.
* the red-black tree can be replaced by other data structures like hash table, AVL tree.
* the red-black tree wins because:
	* costs less space than hash table
 	* costs less with `epoll` as `epoll` operates the tree with more insertion/deletion than lookup.
		* for insert/delete intensive tasks, use red-black trees
  		* for lookup intensive tasks, use OVL trees

## references
* https://stackoverflow.com/questions/13852870/red-black-tree-over-avl-tree
