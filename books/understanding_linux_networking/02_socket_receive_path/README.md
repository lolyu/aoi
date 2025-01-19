# socket receive path

## questions
### how many buffering in the socket receive path?
* at least two:
    * the device ring buffer
    * the socket receive queue
* for more details of the queue stats and length setting, check https://github.com/lolyu/aoi/blob/master/linux/linux_performance_monitor_and_tuning/networking/receive_path.md
