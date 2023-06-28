# how does OA talk to syncd

## OA SAI initialization
```cpp
    initSaiApi();
    initSaiRedis(record_location, sairedis_rec_filename);
```

### `initSaiApi()`
```cpp
void initSaiApi()
{
    sai_api_initialize(0, (const sai_service_method_table_t *)&test_services);

    sai_api_query(SAI_API_SWITCH,               (void **)&sai_switch_api);
    sai_api_query(SAI_API_BRIDGE,               (void **)&sai_bridge_api);
    ...

    sai_log_set(SAI_API_SWITCH,                 SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_BRIDGE,                 SAI_LOG_LEVEL_NOTICE);
    ...
}
```

#### `sai_api_initialize()`
```cpp
sai_status_t sai_api_initialize(
        _In_ uint64_t flags,
        _In_ const sai_service_method_table_t* service_method_table)
{
    SWSS_LOG_ENTER();

    return redis_sai->initialize(flags, service_method_table);
}
```
* `OA` initializes a `ServerSai` object that is responsible to talk to `SYNCD`


#### `sai_api_query`
* basically, `sai_api_query` binds the `SAI` API pointer to the requested `SAI` API(`sai_<feature>_api_t`) that stores `SAI` Redis APIs.
```cpp
sai_status_t sai_api_query(
        _In_ sai_api_t sai_api_id,
        _Out_ void** api_method_table)
{
    SWSS_LOG_ENTER();

    if (sai_metadata_get_enum_value_name(&sai_metadata_enum_sai_api_t, sai_api_id))
    {
        *api_method_table = ((void**)&redis_apis)[sai_api_id - 1];
        return SAI_STATUS_SUCCESS;
    }

    return SAI_STATUS_INVALID_PARAMETER;
}
```
* key data structures:
    * `sai_api_t`: an enum to list out all supported `SAI` APIs
    * `sai_enum_metadata_t`: a struct to stores the mapping from an enum to its string values
        * `sai_metadata_enum_sai_api_t` stores the mapping from `SAI` API enum to its string values.
    * `sai_apis_t`: a struct acting as a pointer collection of all the `SAI` API structs.
* the macro `API` constructs the `SAI` API pointer to point to `SAI` API structs with `SAI` Redis API functions.
```cpp
#define API(api) .api ## _api = const_cast<sai_ ## api ## _api_t*>(&redis_ ## api ## _api)

static sai_apis_t redis_apis = {
    API(switch),
    API(port),
    API(fdb),
    API(vlan),
    API(virtual_router),
    API(route),
    API(next_hop),
    ...
    API(bfd),
};
```

##### BFD Redis `SAI` API
* https://github.com/sonic-net/sonic-sairedis/blob/master/lib/sai_redis_bfd.cpp
* the `bfd` `SAI` APIs is expanded as the following:
```cpp
#include "sai_redis.h"

static sai_status_t redis_create_bfd_session(sai_object_id_t *object_id, sai_object_id_t switch_id, uint32_t attr_count, const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();
    return redis_sai->create((sai_object_type_t)SAI_OBJECT_TYPE_BFD_SESSION, object_id, switch_id, attr_count, attr_list);
};
static sai_status_t redis_remove_bfd_session(sai_object_id_t object_id)
{
    SWSS_LOG_ENTER();
    return redis_sai->remove((sai_object_type_t)SAI_OBJECT_TYPE_BFD_SESSION, object_id);
};
static sai_status_t redis_set_bfd_session_attribute(sai_object_id_t object_id, const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();
    return redis_sai->set((sai_object_type_t)SAI_OBJECT_TYPE_BFD_SESSION, object_id, attr);
};
static sai_status_t redis_get_bfd_session_attribute(sai_object_id_t object_id, uint32_t attr_count, sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();
    return redis_sai->get((sai_object_type_t)SAI_OBJECT_TYPE_BFD_SESSION, object_id, attr_count, attr_list);
};

const sai_bfd_api_t redis_bfd_api = {
    redis_create_bfd_session,
    redis_remove_bfd_session,
    redis_set_bfd_session_attribute,
    redis_get_bfd_session_attribute,
    redis_get_bfd_session_stats,
    redis_get_bfd_session_stats_ext,
    redis_clear_bfd_session_stats,
};

```

### `initSaiRedis()`
* set the log location and filename.

## CRUD operations
* let's take `BFD` session as example in this section.

### create
```cpp
    sai_attribute_t attr;
    vector<sai_attribute_t> attrs;

    attr.id = SAI_BFD_SESSION_ATTR_TYPE;
    attr.value.s32 = bfd_session_type;
    attrs.emplace_back(attr);

    uint32_t local_discriminator = bfd_gen_id();
    attr.id = SAI_BFD_SESSION_ATTR_LOCAL_DISCRIMINATOR;
    attr.value.u32 = local_discriminator;
    attrs.emplace_back(attr);

    ...

    fvVector.emplace_back("state", session_state_lookup.at(SAI_BFD_SESSION_STATE_DOWN));

    sai_object_id_t bfd_session_id = SAI_NULL_OBJECT_ID;
    sai_status_t status = sai_bfd_api->create_bfd_session(&bfd_session_id, gSwitchId, (uint32_t)attrs.size(), attrs.data());
```
* function chain
    * `sai_bfd_api->create_bfd_session()`
    * `redis_sai->create((sai_object_type_t)SAI_OBJECT_TYPE_BFD_SESSION, object_id, switch_id, attr_count, attr_list)`
    * `context->m_meta->create(objectType, objectId, switchId, attr_count, attr_list)`

* key data structures:
    * `sai_object_type_t`: enum to list out all supported `SAI` object types
    * `sai_attribute_t`
        * `id`: `sai_attr_id_t`
            * points to the `SAI` object attribute enum
        * `value`: `sai_attribute_value_t`
            * attribute value
    * `sai_attribute_value_t`: union to store all possible attribute types



## Notification handle


## references
