# more about too many open file error
* This note will focus on the too many open file error, why the Linux process hits this error, and how to avoid this?

## file descriptor data structure
![image](https://github.com/user-attachments/assets/c08e1b40-8192-4d96-9231-a68aac2f7d2e)

* key structures:
    * `struct task_struct`: the Linux task, each task has a file descriptor table
    * `struct files_struct`: manages the file descriptors with overall state and metadata
    * `struct fdtable`: low-level structure that manages the mapping of file descriptors to file objects.
	* `struct dentry`: glue together the file and its `inode` together(`inode` is the file meta-data).

```c
struct fdtable {
	unsigned int max_fds;
	struct file __rcu **fd;                /* current fd array */
	unsigned long *close_on_exec;
	unsigned long *open_fds;                /* current fd bit map */
	struct rcu_head rcu;
};
```
 
## socket create process
```c
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)
{
	int retval;
	struct socket *sock;
    ...

	retval = sock_create(family, type, protocol, &sock);
    ...
	retval = sock_map_fd(sock, flags & (O_CLOEXEC | O_NONBLOCK));
    ...
}
```

### `sock_create`
* `sock_create` -> `__sock_create`
* `sock_create` creates the `struct socket_alloc` and returns the `socket` part; also, it creates the `struct sock` based on the protocol as `sock->sk`.
```c
struct socket_alloc {
	struct socket socket;
	struct inode vfs_inode;
};

int __sock_create(struct net *net, int family, int type, int protocol,
			 struct socket **res, int kern)
{
	struct socket *sock;
	const struct net_proto_family *pf;

    ...
	sock = sock_alloc();
	sock->type = type;
    ...
	pf = rcu_dereference(net_families[family]);
	err = pf->create(net, sock, protocol, kern);
    ...
}
```

![image](https://github.com/user-attachments/assets/b4f34316-1446-43bb-8ac7-b677e5c17b39)

### `sock_map_fd`
```c
static int sock_map_fd(struct socket *sock, int flags)
{
	struct file *newfile;
	int fd = get_unused_fd_flags(flags);
	newfile = sock_alloc_file(sock, flags, NULL);
	put_unused_fd(fd);
	return PTR_ERR(newfile);
}
```

#### `get_unused_fd_flags`
* `get_unused_fd_flags` -> `__alloc_fd`:
    * `get_unused_fd_flags` will try to find a not-used file descriptor from the `fdtable`:
        * it uses the RLIMIT NOFILE soft limit as the upper bound to find the not-used file descriptor
            * if the file descriptor is larger than the upper bound, `EMFILE: too many open files` will be raised.
        * during the `fdtable` expansion, if the file descriptor is larger than `fs.nr_open`, `EMFILE: too many open files` will be raised.

```c
int get_unused_fd_flags(unsigned flags)
{
	return __alloc_fd(current->files, 0, rlimit(RLIMIT_NOFILE), flags);        // use the rlimit nofile soft limit as upper bound
}

int __alloc_fd(struct files_struct *files,
	       unsigned start, unsigned end, unsigned flags)
{
	unsigned int fd;
	int error;
	struct fdtable *fdt;

repeat:
	fdt = files_fdtable(files);
	fd = start;
	if (fd < files->next_fd)
		fd = files->next_fd;

	if (fd < fdt->max_fds)
		fd = find_next_zero_bit(fdt->open_fds, fdt->max_fds, fd);            // find next available file descriptor in the bitmap

	error = -EMFILE;
	if (fd >= end)
		goto out;

	error = expand_files(files, fd);
	if (error < 0)
		goto out;

	/*
	 * If we needed to expand the fs array we
	 * might have blocked - try again.
	 */
	if (error)
		goto repeat;

	if (start <= files->next_fd)
		files->next_fd = fd + 1;

	__set_open_fd(fd, fdt);                                                // set this fd as used in the bitmap
    ...
}

static int expand_files(struct files_struct *files, int nr)
{
	struct fdtable *fdt;

	fdt = files_fdtable(files);

	/* Do we need to expand? */
	if (nr < fdt->max_fds)
		return 0;

	/* Can we expand? */
	if (nr >= sysctl_nr_open)
		return -EMFILE;

	/* All good, so we try */
	return expand_fdtable(files, nr);
}
```

```
# ulimit -n
1024
# cat /proc/sys/fs/nr_open
1048576
# ulimit -nS
1024
# ulimit -nH
1048576
```

#### `sock_alloc_file`
* `sock_alloc_file` allocates the `struct file` objects and stores in the `sock->file`.
```c
struct file *sock_alloc_file(struct socket *sock, int flags, const char *dname)
{
	struct qstr name = { .name = "" };
	struct path path;
	struct file *file;

	...
	path.dentry = d_alloc_pseudo(sock_mnt->mnt_sb, &name);					// create the dentry
	...
	d_instantiate(path.dentry, SOCK_INODE(sock));							// bind the dentry with the inode
	SOCK_INODE(sock)->i_fop = &socket_file_ops;

	file = alloc_file(&path, FMODE_READ | FMODE_WRITE,						// create the file object
		  &socket_file_ops);

	sock->file = file;														// bind the file to the socket
	file->f_flags = O_RDWR | (flags & O_NONBLOCK);
	file->private_data = sock;
	return file;
}
```

* `get_empty_filp()` checks if the system-level file count exceeds the limit `fs.file-max`.

```c
struct file *alloc_file(struct path *path, fmode_t mode,
		const struct file_operations *fop)
{
	struct file *file;

	file = get_empty_filp();
	if (IS_ERR(file))
		return file;
	...
	return file;
}

struct file *get_empty_filp(void)
{
	...
	/*
	 * Privileged users can go above max_files
	 */
	if (get_nr_files() >= files_stat.max_files && !capable(CAP_SYS_ADMIN)) {
		/*
		 * percpu_counters are inaccurate.  Do an expensive check before
		 * we go and fail.
		 */
		if (percpu_counter_sum_positive(&nr_files) >= files_stat.max_files)
			goto over;
	}
	...
}
```

## summary
* key factors:
	* `fs.nr_open` limits the max length of the fd table of a single process, this is system level.
	* ulimit `nofile` limits the upper bound of the fd that could be allocated for a single process, this is user level.
	* `fs.file-max` limits the max number of the file objects of the system, this is system level.
		* this limit could be bypassed by `root` user.

```
# ulimit -nH
1048576
# ulimit -nS
1024
# sysctl fs.nr_open
fs.nr_open = 1048576
# sysctl fs.file-max
fs.file-max = 9223372036854775807
```

## references
* https://chenshuo.com/notes/kernel/data-structures/
* https://access.redhat.com/solutions/384633

