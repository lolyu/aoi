# SAI basics

![image](https://github.com/lolyu/aoi/assets/35479537/326e78b9-69d7-4c2a-8df3-5ccdbf6cf6f4)
* key components:
    * `adapter`: implements SAI
    * `control stack`: software components to act as the host and caller of SAI
    * `adapter host`: software component  to load the adapter and exposes its functionality to control stack
    * `forwarding element`: network forwarding plane implementation that a SAI adapter operates on
        * could be a switch chip, NPU or software switch

## adapter startup/shutdown sequence
* after the adapter is loaded, the adapter host acquires pointers to three well-known functions:
    * `sai_api_initialize()`
    * `sai_api_query()`
    * `sai_api_unitialize()`

* `sai_api_initialize`:
    * the adapter is passed a method table of services provided by the adapter host

* `sai_api_query`:
    * used ti retrieve various method tables for SAI functionalities

* `sai_initialize_switch`:
    * used to perform full forwarding element initialization
    * takes the following parameters:
        * handle to switch entity profile
            * the `profile_id` is used by the adapter to retrieve a profile
            * a profile is a list of key-value string pairs that contain vendor specific settings
            * the functions passed by `sai_api_initialize` are used to get the settings
        * hardware id of that switch entity
        * name of corresponding microcode
        * callbacks table

* `sai_connect_switch`
* `sai_disconnect_switch`
* `sai_switch_shutdown`
* `sai_switch_uninitialize`:
    * cleanup anything done by `sai_api_initialize`

## SAI functional APIs

```c
typedef uint64_t sai_object_id_t;
```
* SAI object id `sai_object_id_t` is generated by SAI and then passed to the user during the SAI create function calls.
* SAI object id is used to uniquely identify a SAI object.

* SAI data types: `saitypes.h`
* `SAI` status codes: `saistatus.h`
* functionality query: `sai.h`

## references
* https://github.com/opencomputeproject/SAI/blob/master/doc/SAI-Extensions.md
* https://github.com/lolyu/aoi/blob/master/sonic/sonic-sairedis/sai_basics.md