# handler tracking

### handler tracking with packet_sniffer.cpp
```
# g++ packet_sniffer.cpp -lboost_system -lpthread -lboost_thread -D BOOST_ASIO_ENABLE_HANDLER_TRACKING
```
#### handler tracking analysis
```
@asio|1667538635.920471|0*1|descriptor@0x559596720808.async_read_some             # the main process created a new asynchrounous operation(async_read_some) with strand wrapped completion handler handleRecv 1
@asio|1667538635.920585|.1|non_blocking_read,ec=system:11,bytes_transferred=0     # the main process performs system call non_blocking_read in async_read_some
@asio|1667538635.972881|.1|non_blocking_read,ec=system:0,bytes_transferred=30     # the main process performs another non_blocking_read in async_read_some, read success and complete
@asio|1667538635.972921|>1|ec=system:0,bytes_transferred=30                       # enters the strand wrapped handleRecv 
@asio|1667538635.972931|1*2|strand@0x559596720770.dispatch                        # the strand wrapped completion hander calls the handleRecv
@asio|1667538635.972937|>2|                                                       # enters handleRecv 1
@asio|1667538635.973102|2*3|descriptor@0x559596720808.async_read_some             # in handleRecv 1, performs a new async_read_some with strand wrapped handleRecv 3
@asio|1667538635.973124|.3|non_blocking_read,ec=system:0,bytes_transferred=30     # in handleRecv 1, performs a system call non_blocking_read in async_read_some
@asio|1667538635.973130|<2|                                                       # leave handleRecv 1
@asio|1667538635.973136|<1|                                                       # leave the strand wrapper for handleRecv 1
@asio|1667538635.973143|>3|ec=system:0,bytes_transferred=30                       # enters strand wrapped handleRecv 3
@asio|1667538635.973148|3*4|strand@0x559596720770.dispatch                        # the strand wrapped completion hander calls handleRecv
@asio|1667538635.973153|>4|                                                       # enters the handleRecv 4
@asio|1667538635.973209|4*5|descriptor@0x559596720808.async_read_some             # in handleRecv 4, performs a new async_read_some with strand wrapped handleRecv5
@asio|1667538635.973225|.5|non_blocking_read,ec=system:0,bytes_transferred=30     # in handleRecv 4, performs a system call non_blocking_read in async_read_some
@asio|1667538635.973231|<4|                                                       # leave handleRecv 4
@asio|1667538635.973236|<3|                                                       # leave the strand wrapper for handleRecv 4
@asio|1667538635.973242|>5|ec=system:0,bytes_transferred=30                       # enters handleRecv 5
@asio|1667538635.973247|5*6|strand@0x559596720770.dispatch
@asio|1667538635.973252|>6|
@asio|1667538635.973305|6*7|descriptor@0x559596720808.async_read_some
@asio|1667538635.973320|.7|non_blocking_read,ec=system:0,bytes_transferred=30
@asio|1667538635.973325|<6|
@asio|1667538635.973330|<5|
@asio|1667538635.973337|>7|ec=system:0,bytes_transferred=30
@asio|1667538635.973342|7*8|strand@0x559596720770.dispatch
@asio|1667538635.973347|>8|
@asio|1667538635.973398|8*9|descriptor@0x559596720808.async_read_some
@asio|1667538635.973411|.9|non_blocking_read,ec=system:11,bytes_transferred=0
@asio|1667538635.973417|<8|
@asio|1667538635.973421|<7|
@asio|1667538635.973949|.9|non_blocking_read,ec=system:0,bytes_transferred=30
@asio|1667538635.973973|>9|ec=system:0,bytes_transferred=30
@asio|1667538635.973978|9*10|strand@0x559596720770.dispatch
@asio|1667538635.973982|>10|
@asio|1667538635.974033|10*11|descriptor@0x559596720808.async_read_some
@asio|1667538635.974045|.11|non_blocking_read,ec=system:0,bytes_transferred=30
@asio|1667538635.974050|<10|
```

#### visualization
