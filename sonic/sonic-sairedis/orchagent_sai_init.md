# orchagent sai init

```cpp
    initSaiApi();
    initSaiRedis(record_location, sairedis_rec_filename);
```

## `initSaiApi`
```cpp
void initSaiApi()
{
    SWSS_LOG_ENTER();

    sai_api_initialize(0, (const sai_service_method_table_t *)&test_services);

    sai_api_query(SAI_API_SWITCH,               (void **)&sai_switch_api);
    sai_api_query(SAI_API_BRIDGE,               (void **)&sai_bridge_api);
    sai_api_query(SAI_API_VIRTUAL_ROUTER,       (void **)&sai_virtual_router_api);
    ...

    sai_log_set(SAI_API_SWITCH,                 SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_BRIDGE,                 SAI_LOG_LEVEL_NOTICE);
    sai_log_set(SAI_API_VIRTUAL_ROUTER,         SAI_LOG_LEVEL_NOTICE);
    ...
}

```


### `sai_api_initialize`
```cpp
std::shared_ptr<SaiInterface> redis_sai = std::make_shared<ClientServerSai>();

sai_status_t sai_api_initialize(
        _In_ uint64_t flags,
        _In_ const sai_service_method_table_t* service_method_table)
{
    return redis_sai->initialize(flags, service_method_table);
}
```
* `ClientServerSai` -> `SaiInterface`: proxy class, it redirect SAI APIs to its member `m_sai`
    * `m_sai` could be either `ClientSai` and `ServerSai`

* `ServerSai` -> `SaiInterface`: proxy class to the `Sai` object, and redirect
* `ClientSai` -> `SaiInterface`

### `sai_api_query`


### `sai_log_set`

