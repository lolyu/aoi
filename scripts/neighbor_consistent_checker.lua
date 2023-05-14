-- ARGV[3] - APPL_DB index                          0
-- ARGV[4] - APPL_DB mux state table name           HW_MUX_CABLE_TABLE

-- ARGV[1] - CONFIG_DB index                        4
-- ARGV[2] - CONFIG_DB mux cable config table       MUX_CABLE

-- ARGV[5] - ASIC_DB index                          1
-- ARGV[6] - ASIC_DB neighbor entry prefix          SAI_OBJECT_TYPE_NEIGHBOR_ENTRY
-- ARGV[7] - ASIC_DB route entry prefix             SAI_OBJECT_TYPE_ROUTE_ENTRY
-- ARGV[8] - ASIC_DB fdb entry prefix               SAI_OBJECT_TYPE_FDB_ENTRY

-- local config_db = ARGV[1]
-- local mux_table_name = ARGV[2]
-- local counter_db = ARGV[3]
-- local port_name_map_table_name = ARGV[4]
-- local APPL_DB = ARGV[5]
-- local mux_state_table_name = ARGV[6]
-- local ASIC_DB = ARGV[7]
-- local neighbor_prefix = ARGV[8]
-- local route_prefix = ARGV[9]

local APPL_DB = 0
local mux_state_table_name = 'HW_MUX_CABLE_TABLE'
local counters_db = 2
local port_name_map_table_name = 'COUNTERS_PORT_NAME_MAP'
local CONFIG_DB = 4
local mux_table_name = 'MUX_CABLE'
local STATE_DB = 6
local mux_cable_table_name = 'MUX_CABLE_TABLE'
local ASIC_DB = 1
local bridge_port_prefix = 'ASIC_STATE:SAI_OBJECT_TYPE_BRIDGE_PORT'
local neighbor_prefix = 'ASIC_STATE:SAI_OBJECT_TYPE_NEIGHBOR_ENTRY'
local route_prefix = 'ASIC_STATE:SAI_OBJECT_TYPE_NEIGHBOR_ENTRY'
local fdb_prefix = 'ASIC_STATE:SAI_OBJECT_TYPE_FDB_ENTRY'


local function hgetall(hash_key)
    local flat_map = redis.call('HGETALL', hash_key)
    local result = {}
    for i = 1, #flat_map, 2 do
        result[flat_map[i]] = flat_map[i + 1]
    end
    return result
end

-- neighbor address -> MAC address --> port --> mux state --> neighbor state

-- build mapping from bridge port oid to the port name
local function get_bridge_port_to_name_mapping()
    local bridge_port_to_name = {}
    local port_name_map = {}

    redis.call('SELECT', counters_db)
    local port_id_map = hgetall(port_name_map_table_name)
    for key, value in pairs(port_id_map) do
        port_name_map[value] = key
    end

    redis.call('SELECT', ASIC_DB)
    local bridge_ports = redis.call('KEYS', bridge_port_prefix .. '*')
    for i, bridge_port in ipairs(bridge_ports) do
        local port_id = redis.call('HGET', bridge_port, 'SAI_BRIDGE_PORT_ATTR_PORT_ID')
        local bridge_id = string.sub(bridge_port, string.len(bridge_port_prefix) + 2)
        bridge_port_to_name[bridge_id] = port_name_map[port_id]
    end
    return bridge_port_to_name
end

-- build mapping from mac to port name
local function get_mac_to_port_name_mapping(bridge_port_to_name_map)
    local mac_to_port_name = {}

    redis.call('SELECT', ASIC_DB)
    local fdb_entries = redis.call('KEYS', fdb_prefix .. '*')
    for i, fdb_entry in ipairs(fdb_entries) do
        local bridge_port_id = redis.call('HGET', fdb_entry, 'SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID')
        local port_name = bridge_port_to_name_map[bridge_port_id]
        mac_to_port_name[port_name] = string.sub(fdb_entry, string.len(fdb_prefix) + 2)
        local fdb_details = cjson.decode(string.sub(fdb_entry, string.len(fdb_prefix) + 2))
        if type(fdb_details) == 'table' and fdb_details['mac'] ~= nil then
            mac_to_port_name[fdb_details['mac']] = port_name
        end
    end
    return mac_to_port_name
end

-- get vlan neighbors
local function get_arp_entries()
    local neigh_prefix = 'NEIGH_TABLE:Vlan'
    local arp_entries = {}

    redis.call('SELECT', APPL_DB)
    local arp_keys = redis.call('KEYS', neigh_prefix .. '*')
    for i, arp_key in ipairs(arp_keys) do
        local second_separator_index = string.find(arp_key, ':', string.len(neigh_prefix), true)
        if second_separator_index ~= nil then
            local neighbor_ip = string.sub(arp_key, second_separator_index + 1)
            local mac = redis.call('HGET', arp_key, 'neigh')
            arp_entries[neighbor_ip] = mac
        end
    end
    return arp_entries
end

-- get mux server to port mapping
local function get_mux_server_to_port_mapping()
    local mux_server_to_port = {}
    local mux_table_prefix = mux_table_name .. '|'

    redis.call('SELECT', CONFIG_DB)
    local mux_cables = redis.call('KEYS', mux_table_prefix .. '*')
    for i, mux_cable in ipairs(mux_cables) do
        local port_name = string.sub(mux_cable, string.len(mux_table_prefix) + 1)
        local mux_server_ipv4 = redis.call('HGET', mux_cable, 'server_ipv4')
        local mux_server_ipv6 = redis.call('HGET', mux_cable, 'server_ipv6')
        if mux_server_ipv4 ~= nil then
            local prefix_separator_index = string.find(mux_server_ipv4, '/', 1, true)
            if prefix_separator_index ~= nil then
                mux_server_ipv4 = string.sub(mux_server_ipv4, 0, prefix_separator_index - 1)
            end
            mux_server_to_port[mux_server_ipv4] = port_name
        end
        if mux_server_ipv6 ~= nil then
            local prefix_separator_index = string.find(mux_server_ipv6, '/', 1, true)
            if prefix_separator_index ~= nil then
                mux_server_ipv6 = string.sub(mux_server_ipv6, 0, prefix_separator_index - 1)
            end
            mux_server_to_port[mux_server_ipv6] = port_name
        end
    end
    return mux_server_to_port
end

-- get neighbor to port mapping
local function get_neighbor_to_port_mapping(arp_entries, mac_to_port_name_map, mux_server_to_port_map)
    local neighbor_to_port = {}

    for neighbor_ip, mac in pairs(arp_entries) do
        local port_name = mac_to_port_name_map[mac]
        local port_name_from_mux_config = mux_server_to_port_map[neighbor_ip]
        if port_name_from_mux_config ~= nil then
            port_name = port_name_from_mux_config
        end
        if port_name ~= nil then
            neighbor_to_port[neighbor_ip] = port_name
        end
    end
    return neighbor_to_port
end

-- get mux states
-- FIXME: should we use other tables to get mux state?
local function get_mux_states()
    local mux_states = {}
    local mux_state_table_prefix = mux_cable_table_name .. '|'

    redis.call('SELECT', STATE_DB)
    local mux_cables = redis.call('KEYS', mux_state_table_prefix .. '*')
    for i, mux_cable in ipairs(mux_cables) do
        local port_name = string.sub(mux_cable, string.len(mux_state_table_prefix) + 1)
        local mux_state = redis.call('HGET', mux_cable, 'state')
        if mux_state ~= nil then
            mux_states[port_name] = mux_state
        end
    end
    return mux_states
end

local NEIGHBOR_ACTIVE_INCORRECT_ASIC_ENTRY_COUNT_ERROR      = 1
local NEIGHBOR_ACTIVE_INCORRECT_ASIC_ENTRY_ERROR            = 2
local NEIGHBOR_ACTIVE_NO_NEIGHBOR_IN_AISC_ERROR             = 3
local NEIGHBOR_ACTIVE_TUNNEL_ROUTE_IN_ASIC_ERROR            = 4
local NEIGHBOR_STANDBY_INCORRECT_ASIC_ENTRY_COUNT_ERROR     = 5
local NEIGHBOR_STANDBY_INCORRECT_ASIC_ENTRY_ERROR           = 6
local NEIGHBOR_STANDBY_NO_TUNNEL_ROUTE_IN_ASIC_ERROR        = 7
local NEIGHBOR_STANDBY_NEIGHBOR_IN_ASIC_ERROR               = 8
local NEIGHBOR_FAULTY_MUX                                   = 9
local NEIGHBOR_MUX_STATE_NOT_PRESENT                        = 10


-- check neighbor states based on mux states
local function check_neighbor(neighbor_to_port_map, mux_states)
    local check_results = {}

    redis.call('SELECT', ASIC_DB)

    for neighbor_ip, port_name in pairs(neighbor_to_port_map) do
        local mux_state = mux_states[port_name]
        if mux_state == 'active' then
            local asic_entries = redis.call('KEYS', neighbor_prefix .. '*"' .. neighbor_ip .. '"*')
            local asic_entries_count = table.getn(asic_entries)
            if asic_entries_count == 0 then
                check_results[neighbor_ip] = NEIGHBOR_ACTIVE_NO_NEIGHBOR_IN_AISC_ERROR
            elseif asic_entries_count ~= 1 then
                check_results[neighbor_ip] = NEIGHBOR_ACTIVE_INCORRECT_ASIC_ENTRY_COUNT_ERROR
            end
        elseif mux_state == 'standby' then
            local asic_entries = redis.call('KEYS', route_prefix .. '*"' .. neighbor_ip .. '"*')
            local asic_entries_count = table.getn(asic_entries)
            if asic_entries_count == 0 then
                check_results[neighbor_ip] = NEIGHBOR_STANDBY_NO_TUNNEL_ROUTE_IN_ASIC_ERROR
            elseif asic_entries_count ~= 1 then
                check_results[neighbor_ip] = NEIGHBOR_STANDBY_INCORRECT_ASIC_ENTRY_COUNT_ERROR
            end
        else
            check_results[neighbor_ip] = NEIGHBOR_FAULTY_MUX
        end
    end
    return check_results
end

local bridge_port_to_name_map = get_bridge_port_to_name_mapping()
local mac_to_port_name_map = get_mac_to_port_name_mapping(bridge_port_to_name_map)
local arp_entries = get_arp_entries()
local mux_server_to_port_map = get_mux_server_to_port_mapping()
local neighbor_to_port_map = get_neighbor_to_port_mapping(arp_entries, mac_to_port_name_map, mux_server_to_port_map)
local mux_states = get_mux_states()
local check_results = check_neighbor(neighbor_to_port_map, mux_states)

return redis.status_reply(cjson.encode(check_results))
