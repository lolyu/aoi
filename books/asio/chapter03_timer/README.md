#### timers
* timers
    * `deadline_timer`
        * `typedef basic_deadline_timer< boost::posix_time::ptime > deadline_timer;`
    * `steady_timer`
        * `typedef basic_waitable_timer< chrono::steady_clock > steady_timer;`
    * `system_timer`
        * `typedef basic_waitable_timer< chrono::system_clock > system_timer;`

##### differences between those clocks
* `chrono::system_clock` is based on the system clock, it is not monotonic
* `chrono::steady_timer` is based on the machine startup time, it is monotonic
    * best for interval wait
* `boost::posix_time::ptime` is based on the UTC clock, it is not monotonic
