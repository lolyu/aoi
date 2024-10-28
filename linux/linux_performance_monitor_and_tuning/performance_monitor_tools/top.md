# top
<img width="565" alt="image" src="https://github.com/user-attachments/assets/62ff321a-c35c-4c8d-8877-3108817756ff">

* first line: uptime and system load
    * uptime, same as command `uptime` output
    * the load average
        * system load: the number of processes which are either currently being executed by the CPU or are waiting for execution
        * for single core processor, the load of 1 means 100% cpu usage
        * for a quad-core processor, the load of 1 means 25% cpu usage
        * `top` display the load average for the last 1 min, 5 min and 15 min.
* second line: tasks
    * `total`
    * `running`
    * `sleeping`
    * `stopped`
    * `zombie`
* third line: cpu usage
    * `us`: percentage of time for user processes
    * `sy`: percentage of time for kernel processes
    * `ni`: percentage of time for processes with manually configured nice values
    * `id`: percentage of idle
    * `wa`: percentage of I/O wait time
    * `hi`: percentage of hardware interrupts
    * `si`: percentage of software interrupts
    * `st`: percentage of virtual CPU time waiting for access to physical CPU
* fourth line: MiB memory
    * `total`: total installed memory
    * `free`: shows available memory
    * `used`: shows consumed memory
    * `buff/cache`: shows the amount of information buffered to be written
* fifth line: MiB swap
    * `total`: total installed swap memory
    * `free`: shows available swap memory
    * `used`: shows consumed swap memory
    * `buff/cache`: shows the amount of information cached for future reads

## cpu usage mode
* two modes:
    * solaris mode: task CPU usage will be divided by the total number of CPUs, the value is within `[0%, 100%]`
    * irix mode: CPU usage based on a summation of all CPU/cores available, the value is within `[0%, 100% * N]` where `N` is the CPU count
* press `shift` + `i` to toggle

## per CPU usage
* press `1` to toggle

## task/cpu-states toggle
* press `t` to toggle

## show full command
* press `c` to toggle

## sort processes
* `M`: Sort by the %MEM column.
* `N`: Sort by PID column.
* `T`: Sort by the TIME+ column.
* `P`: Sort by the %CPU column.

## change display units
* press `E` to change the unit for the system memory.
* press `e` to change the unit for the process memory.

## monitor specified processes
```
# top -p [pid0] -p [pid1]
```

# references
* https://www.redhat.com/en/blog/interpret-top-output
* https://man7.org/linux/man-pages/man1/top.1.html
