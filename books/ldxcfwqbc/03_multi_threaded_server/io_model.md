# I/O model

* unix has five I/O models:
    * blocking I/O
    * nonblocking I/O
    * I/O multiplexing(`select` and `poll`)
    * signal driven I/O(`SIGIO`)
    * asynchronous I/O(posix `aio_` functions)

* for an input operation, two phases:
    1. waiting for the data to be ready
    2. copying the data from the kernel to the process

* synchronous I/O vs asynchronous I/O
    * a synchronous I/O causes the requesting process to be blocked utils the I/O completes
    * an asynchronous I/O doesn't cause the requesting process to be blocked

## blocking I/O
* all sockets are blocking

![image](https://github.com/lolyu/aoi/assets/35479537/1fd0c0c3-b6f7-45fc-b411-0538e0879964)

## nonblocking I/O

![image](https://github.com/lolyu/aoi/assets/35479537/c5248f07-7354-4d07-b488-7d8516c0adf5)


## I/O multiplexing

![image](https://github.com/lolyu/aoi/assets/35479537/f6619c10-3e94-4ab8-90ea-18078a1858e2)


## signal-driven I/O
![image](https://github.com/lolyu/aoi/assets/35479537/7e52b2bc-30b8-492d-bd35-a991d9fd7185)
* kernel notifies process with `SIGIO` signal


## asynchronous I/O
![image](https://github.com/lolyu/aoi/assets/35479537/cfc3ddeb-8437-4f10-bcde-dc4f522f6044)


## I/O model comparision
![image](https://github.com/lolyu/aoi/assets/35479537/6dcb02d0-3b34-4923-afbf-87740b26b425)
