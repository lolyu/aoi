# SAI tunnel
<img width="820" alt="image" src="https://github.com/lolyu/aoi/assets/35479537/f59b577f-1eb1-4493-8605-c19200913bf0">

* for the encap tunnel, the encapsulated packets will go through the fibs to be routed
* for the decap tunnel, the encapsulated packets hit L3 decap table first, the decapsulated packet will go through the fibs to be routed
    * the encapsulated packet matches the most specific L3 decap rule

## references
* https://github.com/opencomputeproject/SAI/blob/master/doc/object-model/pipeline_object_model.pdf
* https://github.com/opencomputeproject/SAI/tree/master/doc/behavioral%20model
* https://github.com/opencomputeproject/SAI/tree/master/doc/tunnel
