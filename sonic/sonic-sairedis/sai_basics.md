# SAI basics

![image](https://github.com/lolyu/aoi/assets/35479537/326e78b9-69d7-4c2a-8df3-5ccdbf6cf6f4)
* key components:
    * `adapter`: implements SAI
    * `control stack`: software components to act as the host and caller of SAI
    * `adapter host`: software component  to load the adapter and exposes its functionality to control stack
    * `forwarding element`: network forwarding plane implementation that a SAI adapter operates on
        * could be a switch chip, NPU or software switch

## references
* https://github.com/opencomputeproject/SAI/blob/master/doc/SAI-Extensions.md
