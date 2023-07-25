# FRR architecture

* `struct event_loop`:
```c
/* Master of the theads. */
struct event_loop {
	char *name;

	struct event **read;
	struct event **write;
	struct event_timer_list_head timer;
	struct event_list_head event, ready, unuse;
	struct list *cancel_req;
	bool canceled;
	pthread_cond_t cancel_cond;
	struct hash *cpu_record;
	int io_pipe[2];
	int fd_limit;
	struct fd_handler handler;
	unsigned long alloc;
	long selectpoll_timeout;
	bool spin;
	bool handle_signals;
	pthread_mutex_t mtx;
	pthread_t owner;

	bool ready_run_loop;
	RUSAGE_T last_getrusage;
};
```

* `struct event`:
```c
// lib/frrevent.h
/* Event itself. */
struct event {
	enum event_types type;	   /* event type */
	enum event_types add_type; /* event type */
	struct event_list_item eventitem;
	struct event_timer_list_item timeritem;
	struct event **ref;	      /* external reference (if given) */
	struct event_loop *master;    /* pointer to the struct event_loop */
	void (*func)(struct event *e); /* event function */
	void *arg;		      /* event argument */
	union {
		int val;	      /* second argument of the event. */
		int fd;		      /* file descriptor in case of r/w */
		struct timeval sands; /* rest of time sands value. */
	} u;
	struct timeval real;
	struct cpu_event_history *hist;	    /* cache pointer to cpu_history */
	unsigned long yield;		    /* yield time in microseconds */
	const struct xref_eventsched *xref; /* origin location */
	pthread_mutex_t mtx;		    /* mutex for thread.c functions */
	bool ignore_timer_late;
};
```

## the event loop

![image](https://github.com/lolyu/aoi/assets/35479537/efc1e271-1ada-444a-9f93-1d0af63e5c62)

* terminologies:
    * `task` is `struct event *`
    * `fetch()` is `event_fetch()`
    * `exec()` is `event_call()`
    * `cancel()` is `event_cancel()`
    * `schedule() is any of the various task-specific `event_add_*` functions
* **NOTE**: I/O tasks are keyed off the file descriptor associated with the task, so one file descriptor could have one task, either `read` or `write`


## BFD design

### main entry
```c
int main(int argc, char *argv[])
{
	char ctl_path[512], dplane_addr[512];
	bool ctlsockused = false;
	int opt;

	bglobal.bg_use_dplane = false;

	/* Initialize system sockets. */
	bg_init();

	/* Initialize FRR infrastructure. */
	master = frr_init();

	/* Initialize control socket. */
	control_init(ctl_path);

	/* Initialize BFD data structures. */
	bfd_initialize();

	bfd_vrf_init();

	access_list_init();

	/* Initialize zebra connection. */
	bfdd_zclient_init(&bglobal.bfdd_privs);

	event_add_read(master, control_accept, NULL, bglobal.bg_csock,
		       &bglobal.bg_csockev);

	/* Install commands. */
	bfdd_vty_init();

	/* read configuration file and daemonize  */
	frr_config_fork();

	/* Initialize BFD data plane listening socket. */
	if (bglobal.bg_use_dplane)
		distributed_bfd_init(dplane_addr);

	frr_run(master);
	/* NOTREACHED */

	return 0;
}
```

### BFD session
```c
// bfdd/bfd.h
/*
 * Session state information
 */
struct bfd_session {

	/* protocol state per RFC 5880*/
	uint8_t ses_state;
	struct bfd_discrs discrs;
	uint8_t local_diag;
	uint8_t demand_mode;
	uint8_t detect_mult;
	uint8_t remote_detect_mult;
	uint8_t mh_ttl;
	uint8_t remote_cbit;

	/** BFD profile name. */
	char *profile_name;
	/** BFD pre configured profile. */
	struct bfd_profile *profile;
	/** BFD peer configuration (without profile). */
	struct bfd_profile peer_profile;

	/* Timers */
	struct bfd_config_timers timers;
	struct bfd_timers cur_timers;
	uint64_t detect_TO;
	struct event *echo_recvtimer_ev;
	struct event *recvtimer_ev;
	uint64_t xmt_TO;
	uint64_t echo_xmt_TO;
	struct event *xmttimer_ev;
	struct event *echo_xmttimer_ev;
	uint64_t echo_detect_TO;

	/* software object state */
	uint8_t polling;

	/* This and the localDiscr are the keys to state info */
	struct bfd_key key;
```
* `bfd_session_new`: create a `bfd_session` object
* `bfd_session_apply`: apply configs
* 
