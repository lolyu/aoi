# introduction to libnl
* `netlink` protocol is a socket based IPC mechanism used for communication between userspace processes and the kernel or between userspace processes themselves.
* the netlink address(port) consists of a 32bit integer.
    * 0 is reserved for the kernel

## scenarios
### user space to kernel
![image](https://user-images.githubusercontent.com/35479537/191257847-772974c5-c2c2-4c41-b870-a9eb4c536f45.png)

### user space to user space
![image](https://user-images.githubusercontent.com/35479537/191257909-02c5f634-65b4-42af-930b-6b39219a9578.png)

### user space listening to kernel notifications
![image](https://user-images.githubusercontent.com/35479537/191258025-b828f73a-610c-4af1-bda1-853a94a4f00e.png)


## message format
### struct nlmsghdr
![image](https://user-images.githubusercontent.com/35479537/191258406-42c044e5-daed-4ef5-bf28-d55f838762be.png)
```
struct nlmsghdr {
	__u32		nlmsg_len;	/* Length of message including header */
	__u16		nlmsg_type;	/* Message content */
	__u16		nlmsg_flags;	/* Additional flags */
	__u32		nlmsg_seq;	/* Sequence number */
	__u32		nlmsg_pid;	/* Sending process port ID */
};
```
* sequence number is not mandatory, its purpose is to assit a sender in relating replies to the corresponding requests.
* netlink messages
    * request
        * `NLM_F_REQUEST` flag set
        * should carry a sequence number incremented for each request sent.
        * are meant to request an action from the receiver.
    * notification
        * informal nature, and sequence number is typically set to 0.
    * reply
        * the receiver may reply to the request with a reply.
        * the sequence number of a reply must match the request it is related.

* message types:
    * `NLMSG_NOOP`: no op
    * `NLMSG_ERROR`: error message or ACK
    * `NLMSG_DONE`
    * `NLMSG_OVERRUN`

### multipart message
* a multipart message has the flag `NLM_F_MULTI` set and the receiver is expected to continue receiving and parsing until the special message type `NLMSG_DONE` is received.

![image](https://user-images.githubusercontent.com/35479537/191264697-4dcc58c4-1ed9-43a1-8b98-8f6aa1c7dfbf.png)

### error message
![image](https://user-images.githubusercontent.com/35479537/191274615-83306caf-e1f6-499b-a6dc-aa2030db9693.png)


### ACK message
* ACK message has `NLM_F_ACK` flag set
![image](https://user-images.githubusercontent.com/35479537/191274693-4bacad97-2b54-4de1-931f-cf8986b04ad1.png)

### standard flags
* flags:
    * `NLM_F_REQUEST`
    * `NLM_F_MULTI`
    * `NLM_F_ACK`
    * `NLM_F_ECHO`
    * `NLM_F_ROOT`
    * `NLM_F_MATCH`
    * `NLM_F_ATOMIC`
    * `NLM_F_DUMP`
    * `NLM_F_REPLACE`
    * `NLM_F_EXCL`
    * `NLM_F_CREATE`
    * `NLM_F_APPEND`

## references
* https://www.infradead.org/~tgr/libnl/doc/core.html#_introduction
