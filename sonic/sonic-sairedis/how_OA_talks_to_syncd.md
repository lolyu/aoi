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


## references
