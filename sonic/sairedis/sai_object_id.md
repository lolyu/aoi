# SAI object ID

## virtual object ID
* used by sairedis, to represent an SAI object.
* virtual object ID, total 8 bytes:
```
|switch index(8 bits)|object type(8 bits)|global context(8 bits)|extension flag(1 bit)|object index(39 bits)|
```
* `ASIC_STATE:SAI_OBJECT_TYPE_ROUTER_INTERFACE:oid:0x6000000000698`: oid is `0x6000000000698`
    * `b0110000000000000000000000000000000000000011010011000`
        * switch index is `0x0`
        * object type is `0x6`, which is `sai_object_type_t::SAI_OBJECT_TYPE_ROUTER_INTERFACE`
        * object index is `0x698`
* `VirtualObjectIdManager`: virtual base class to manage the virtual object ID generation

```cpp
sai_object_id_t VirtualObjectIdManager::allocateNewObjectId(
        _In_ sai_object_type_t objectType,
        _In_ sai_object_id_t switchId)
{
    uint32_t switchIndex = (uint32_t)SAI_REDIS_GET_SWITCH_INDEX(switchId);
    uint64_t objectIndex = m_oidIndexGenerator->increment(); // get new object index

    sai_object_id_t objectId = constructObjectId(objectType, switchIndex, objectIndex, m_globalContext);
    return objectId;
}

sai_object_id_t VirtualObjectIdManager::constructObjectId(
        _In_ sai_object_type_t objectType,
        _In_ uint32_t switchIndex,
        _In_ uint64_t objectIndex,
        _In_ uint32_t globalContext)
{
    uint64_t extensionsFlag = (uint64_t)objectType >= SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START;

    objectType = extensionsFlag
        ? (sai_object_type_t)(objectType - SAI_OBJECT_TYPE_EXTENSIONS_RANGE_START)
        : objectType;

    return (sai_object_id_t)(
            ((uint64_t)switchIndex << (SAI_REDIS_OBJECT_TYPE_BITS_SIZE + SAI_REDIS_GLOBAL_CONTEXT_BITS_SIZE + SAI_REDIS_OBJECT_TYPE_EXTENSIONS_FLAG_BITS_SIZE + SAI_REDIS_OBJECT_INDEX_BITS_SIZE)) |
            ((uint64_t)objectType << (SAI_REDIS_GLOBAL_CONTEXT_BITS_SIZE + SAI_REDIS_OBJECT_TYPE_EXTENSIONS_FLAG_BITS_SIZE + SAI_REDIS_OBJECT_INDEX_BITS_SIZE)) |
            ((uint64_t)globalContext << (SAI_REDIS_OBJECT_TYPE_EXTENSIONS_FLAG_BITS_SIZE + SAI_REDIS_OBJECT_INDEX_BITS_SIZE)) |
            ((uint64_t)extensionsFlag << (SAI_REDIS_OBJECT_INDEX_BITS_SIZE)) |
            objectIndex);
}
```

* `OidIndexGenerator`: OID generator class
    * `RedisVidIndexGenerator`: OID generator class, based on the index value stored in the ASIC_DB(`VIDCOUNTER`)
        * `RedisVidIndexGenerator` increments the key `VIDCOUNTER` to generate the OID
```cpp
uint64_t RedisVidIndexGenerator::increment()
{
    return m_dbConnector->incr(m_vidCounterName); // "VIDCOUNTER"
}
```

## real object ID
* used by SAI driver, to represent a SAI object.
* the real object ID is created by the vendor SAI driver, and the mapping between VID and RID is stored in ASIC_DB `VIDTORID` and `RIDTOVID`

```cpp
// syncd/Syncd.cpp
sai_status_t Syncd::processOidCreate(
        _In_ sai_object_type_t objectType,
        _In_ const std::string &strObjectId,
        _In_ uint32_t attr_count,
        _In_ sai_attribute_t *attr_list)
{
    sai_object_id_t objectVid;
    sai_deserialize_object_id(strObjectId, objectVid);

    // extract switch VID from the object VID
    sai_object_id_t switchVid = VidManager::switchIdQuery(objectVid);

    sai_object_id_t switchRid = SAI_NULL_OBJECT_ID;

    if (objectType == SAI_OBJECT_TYPE_SWITCH)
    {
        SWSS_LOG_NOTICE("creating switch number %zu", m_switches.size() + 1);
    }
    else
    {
        // retrieve the switch RID from ASIC_DB VIDTORID
        switchRid = m_translator->translateVidToRid(switchVid);
    }

    sai_object_id_t objectRid;

    sai_status_t status = m_vendorSai->create(objectType, &objectRid, switchRid, attr_count, attr_list);
    return status;
}
```
```cpp
// syncd/VirtualOidTranslator.cpp
void VirtualOidTranslator::insertRidAndVid(
        _In_ sai_object_id_t rid,
        _In_ sai_object_id_t vid)
{
    SWSS_LOG_ENTER();

    std::lock_guard<std::mutex> lock(m_mutex);

    m_rid2vid[rid] = vid;
    m_vid2rid[vid] = rid;

    m_client->insertVidAndRid(vid, rid);
}
```

## references
