# sk_buff
* `sk_buff` represents packet data and headers

## kernel layout
* kernel keeps all `sk_buff` structures in a double-linked list
* a `sk_buff_header` structure is inserted at the beginning of the list as a dummy header
```c
struct sk_buff_head {
    /* These two members must be first. */
    struct sk_buff    * next;
    struct sk_buff    * prev;

    _ _u32        qlen;
    spinlock_t    lock;
};
```

![image](https://user-images.githubusercontent.com/35479537/178446615-f64f95e3-a50e-42ac-b7e5-a11f0a42ad03.png)


![image](https://user-images.githubusercontent.com/35479537/178447431-4f01601f-3194-4e57-b9e9-9710911796e8.png)

* `sk_buff` fields:
    * `head`
    * `end`
    * `data`
    * `tail`
        * `head` and `end` point to the beginning and end of the space allocated to the buffer
        * `data` and `tail` point to the beginning and end of the actual data
    * `h`
        * pointer to the protocol header for L4 layer
    * `nh`
        * pointer to the protocol header for l3 layer
    * `mac`
        * pointer to the protocol header for l2 layer
    * `dst`
        * the route for this packet
    * `net_device`
        * the device to send/receive the packet

## sk_buff management
* allocate memory: `alloc_skb` and `dev_alloc_skb`
![image](https://user-images.githubusercontent.com/35479537/178456873-35d08b02-ed04-4065-a4de-9a59e91f1d2e.png)
![image](https://user-images.githubusercontent.com/35479537/178457562-d8e3b5b6-180a-4bf7-9a9c-bf155d04a18f.png)


## references
* http://www.embeddedlinux.org.cn/linux_net/0596002556/understandlni-CHP-2-SECT-1.html
