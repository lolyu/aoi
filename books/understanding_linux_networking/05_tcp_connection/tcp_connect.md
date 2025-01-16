# tcp connect
```c
int main()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(fd, ...);
    ...
}
```

* `connect()` -> `inet_stream_connect()` -> `__inet_stream_connect()` -> `tcp_v4_connect`

## `tcp_v4_connect`
