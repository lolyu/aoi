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
![image](https://github.com/user-attachments/assets/630a9d55-3096-4fdf-a20f-53e2aba686aa)

* the difference with `recv_from` on a blocking socket:
    * with `recv_from` on a blocking socket, the current thread will be inserted into the socket waiting queue with wakeup callback as `autoremove_wake_function`.
        * once there is data available, `autoremove_wake_function` will be called: wake up one waiting thread and remove the thread from the waiting queue.
    * with `epoll`, after the socket is added to the `epoll` fd, a mocking wait item (`eppoll_item`) is inserted into the socket waiting queue with wakeup callback as `ep_poll_callback`
        * once there is data available, `ep_poll_callback` will be called, what happened?

## what happened when the socket has data to read?
* `ep_poll_callback` will be called.
```c
static int ep_poll_callback(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
	struct epitem *epi = ep_item_from_wait(wait);
	struct eventpoll *ep = epi->ep;

    ...

	if (!ep_is_linked(&epi->rdllink)) {
		list_add_tail(&epi->rdllink, &ep->rdllist);                                // add the
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

## references
* 
