# multiprocessing

## process start methods
* start methods:
    * `fork`
        * use `os.fork` to fork the python interpreter
        * copy-on-write of the parent process's entire virtual memory including initialized python interpreter, loaded modules, and constructed objects in memory
        * fork doesn't copy the parent process's threads, thus locks that in the parent process were held by other threads are stuck in the child process without the owning threads to unlock them, this could cause a deadlock
        * child process inherits OS resources like open file descriptors and open network ports
    * `spawn`
        * starts a Python child process from scratch without the parent process's memory, file descriptors, threads, etc.
        * slower than `fork`
    * `forkserver`
        * forks a duplicate of the current Python process that trims down to approximately a fresh Python process. This becomes the "fork server" process. Then each time you start a child process, it asks the fork server to fork a child and run its target callable.

## ipc
### queue
* `multiprocessing.queue` is implemented using a pipe and a few locks/semaphores.
* when a process first puts an item on the queue, a feeder thread is started which transfers objects from a buffer into the pipe
* when a queue object is garbage-collected, `Queue.close()` is called to join the background feeder thread to flush all buffered data to the pipe
```python
from multiprocessing import Process, Queue


q = Queue()


def f():
    q.put("helloworld")


if __name__ == "__main__":
    p = Process(target=f)
    p.start()
    p.join()
    print(q.get())

```
### pipe
```python
from multiprocessing import Process, Pipe


parent_conn, child_conn = Pipe()


def f():
    child_conn.send("helloworld")


if __name__ == "__main__":
    p = Process(target=f)
    p.start()
    p.join()
    print(parent_conn.recv())

```

### sharing state between processes

#### shared memory
* data can be stored in the shared memory map using `Value` or `Array`
```
from multiprocessing import Process, Value, Array

def f(n, a):
    n.value = 3.1415927
    for i in range(len(a)):
        a[i] = -a[i]

if __name__ == '__main__':
    num = Value('d', 0.0)
    arr = Array('i', range(10))

    p = Process(target=f, args=(num, arr))
    p.start()
    p.join()

    print(num.value)
    print(arr[:])
```

#### sync server process
* A `Manager` object controls a server process which holds Python objects and allow other processes to manipulate them using proxies

## references
* https://docs.python.org/3.5/library/multiprocessing.html
