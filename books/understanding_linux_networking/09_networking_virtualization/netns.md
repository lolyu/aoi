# network namespace
* `network namespaces` provide isolation of the system resources associated with networking: network devices, IPv4 and IPv6 protocol stacks, IP routing tables, firewall rules, etc.

## netns structures
![netns drawio](https://github.com/user-attachments/assets/d3d35b88-79f8-494e-99ec-7c7154a285c2)

* each network device/socket belongs to one netns.

## the default netns `init_net`
* the linux `init_task` uses the default netns `init_net`, which is inherited by all linux tasks by default.
    * use `CLONE_NEWNET` in `clone` to create new netns.

### `init_net`
```c
static LIST_HEAD(pernet_list);
static struct list_head *first_device = &pernet_list;                                 // the pernet_operations list 
static DEFINE_MUTEX(net_mutex);

LIST_HEAD(net_namespace_list);                                                        // the netns list
EXPORT_SYMBOL_GPL(net_namespace_list);

struct net init_net = {                                                                // the default netns init_net
	.dev_base_head = LIST_HEAD_INIT(init_net.dev_base_head),
};
EXPORT_SYMBOL(init_net);

static struct pernet_operations __net_initdata net_ns_ops = {                          // the netns pernet_operations
	.init = net_ns_net_init,
	.exit = net_ns_net_exit,
};

static int __init net_ns_init(void)                                                    // the netns init function
{
    ...
	mutex_lock(&net_mutex);
	if (setup_net(&init_net, &init_user_ns))
		panic("Could not setup the initial network namespace");

	rtnl_lock();
	list_add_tail_rcu(&init_net.list, &net_namespace_list);
	rtnl_unlock();

	mutex_unlock(&net_mutex);

	register_pernet_subsys(&net_ns_ops);

	return 0;
}
pure_initcall(net_ns_init);                                                            // register the netns init function

static __net_init int setup_net(struct net *net, struct user_namespace *user_ns)        // setup new netns
{
	/* Must be called with net_mutex held */
	const struct pernet_operations *ops, *saved_ops;
    ...

	list_for_each_entry(ops, &pernet_list, list) {                                    // calls each pernet_operation init function
		error = ops_init(ops, net);
        ...
	}
    ...
}
```

## the netns sub system
* the net namespace has multiple sub systems: routing table, `/proc/` file system, iptable rules, etc.
* `struct pernet_operations` represents the netns sub system operations:
```c
struct pernet_operations {
	struct list_head list;
	int (*init)(struct net *net);
	void (*exit)(struct net *net);
	void (*exit_batch)(struct list_head *net_exit_list);
	int *id;
	size_t size;
};

// the sub system
extern int register_pernet_subsys(struct pernet_operations *);
extern void unregister_pernet_subsys(struct pernet_operations *);
extern int register_pernet_device(struct pernet_operations *);
extern void unregister_pernet_device(struct pernet_operations *);
```
* all `pernet_operations` are stored in list `pernet_list`, when a new netns is created, each sub system's init function will be called to initialized the sub system in the new netns.
* let's get the kernel call trace when a netns is created:
```
# perf ftrace -a -G 'setup_net' -g 'smp_*' -g '__smp*' -g 'rcu*' --graph-opts depth=3
```

<img width="626" alt="image" src="https://github.com/user-attachments/assets/397a01e5-f8e3-41c3-b08b-169c3cf85fe6" />

## routing in netns
* when sending packets down a socket, the fib lookup will retrieves the fib tables in the netns that the socket belongs.

```c
static inline int fib_lookup(struct net *net, const struct flowi4 *flp,
			     struct fib_result *res)
{
	struct fib_table *table;

	table = fib_get_table(net, RT_TABLE_LOCAL);
	if (!fib_table_lookup(table, flp, res, FIB_LOOKUP_NOREF))
		return 0;

	table = fib_get_table(net, RT_TABLE_MAIN);
	if (!fib_table_lookup(table, flp, res, FIB_LOOKUP_NOREF))
		return 0;
	return -ENETUNREACH;
}

static inline struct fib_table *fib_get_table(struct net *net, u32 id)
{
	struct hlist_head *ptr;

	ptr = id == RT_TABLE_LOCAL ?
		&net->ipv4.fib_table_hash[TABLE_LOCAL_INDEX] :
		&net->ipv4.fib_table_hash[TABLE_MAIN_INDEX];
	return hlist_entry(ptr->first, struct fib_table, tb_hlist);
}
```
