# boost log basics
## overview
![image](https://user-images.githubusercontent.com/35479537/224226676-e43f0e17-573d-4144-80c7-6ff8eca16a7c.png)

### attributes
* attributes are attached to the a log record
* three kinds of attribute set:
    * global
    * thread-specific
    * source-specific
*  it is possible for a same-named attribute to appear in several attribute sets. Such conflicts are solved on priority basis: global attributes have the least priority, source-specific attributes have the highest; the lower priority attributes are discarded from consideration in case of conflicts.

### logging core and filtering
* logging core is responsible for filtering logs:
    * global filtering
        * within the logging core
    * sink-specific filtering
        * for each sink, allows directing log records to particular sinks

### sinks
* if a log record passes filtering for at least one sink, the record is considered to be consumable
* sinks has two parts:
    * sink frontend
        * filtering, formatting and thread synchronization, provided by the library
    * sink backend
        * log record processing: output to file, statistics, etc.
* boost provides a default sink to console, whenever user provides custom sinks, the default one will be abandoned


## references
* https://theboostcpplibraries.com/boost.log
* https://www.boost.org/doc/libs/1_63_0/libs/log/doc/html/log/extension.html
* https://www.boost.org/doc/libs/1_71_0/libs/log/doc/html/index.html
