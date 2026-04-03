
/* DO NOT MODIFY, FILE AUTO GENERATED */

extern "C" {
#include "sai.h"
#include "saiextensions.h"
}
#include "SaiInterface.h"
#include "Sai.h"
#include "swss/logger.h"
#include <memory>

static std::shared_ptr<sairedis::SaiInterface> stub = std::make_shared<saivpp::Sai>();


/* ==== API STRUCTS === */


/* SAI APIS for SWITCH */

static sai_status_t stub_create_switch(
        _Out_ sai_object_id_t *switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id,SAI_NULL_OBJECT_ID, attr_count, attr_list);
}

static sai_status_t stub_remove_switch(
        _In_ sai_object_id_t switch_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id);
}

static sai_status_t stub_set_switch_attribute(
        _In_ sai_object_id_t switch_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id, attr);
}

static sai_status_t stub_get_switch_attribute(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id, attr_count, attr_list);
}

static sai_status_t stub_get_switch_stats(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_switch_stats_ext(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_switch_stats(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH),switch_id, number_of_counters, counter_ids);
}

static sai_status_t stub_switch_mdio_read(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val)
{
    SWSS_LOG_ENTER();

    return stub->switchMdioRead(switch_id, device_addr, start_reg_addr, number_of_registers, reg_val);
}

static sai_status_t stub_switch_mdio_write(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val)
{
    SWSS_LOG_ENTER();

    return stub->switchMdioWrite(switch_id, device_addr, start_reg_addr, number_of_registers, reg_val);
}

static sai_status_t stub_create_switch_tunnel(
        _Out_ sai_object_id_t *switch_tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH_TUNNEL),switch_tunnel_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_switch_tunnel(
        _In_ sai_object_id_t switch_tunnel_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH_TUNNEL),switch_tunnel_id);
}

static sai_status_t stub_set_switch_tunnel_attribute(
        _In_ sai_object_id_t switch_tunnel_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH_TUNNEL),switch_tunnel_id, attr);
}

static sai_status_t stub_get_switch_tunnel_attribute(
        _In_ sai_object_id_t switch_tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SWITCH_TUNNEL),switch_tunnel_id, attr_count, attr_list);
}

static sai_status_t stub_switch_mdio_cl22_read(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _Out_ uint32_t *reg_val)
{
    SWSS_LOG_ENTER();

    return stub->switchMdioCl22Read(switch_id, device_addr, start_reg_addr, number_of_registers, reg_val);
}

static sai_status_t stub_switch_mdio_cl22_write(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t device_addr,
        _In_ uint32_t start_reg_addr,
        _In_ uint32_t number_of_registers,
        _In_ const uint32_t *reg_val)
{
    SWSS_LOG_ENTER();

    return stub->switchMdioCl22Write(switch_id, device_addr, start_reg_addr, number_of_registers, reg_val);
}

static sai_switch_api_t stub_switch = {
    .create_switch = stub_create_switch,
    .remove_switch = stub_remove_switch,
    .set_switch_attribute = stub_set_switch_attribute,
    .get_switch_attribute = stub_get_switch_attribute,
    .get_switch_stats = stub_get_switch_stats,
    .get_switch_stats_ext = stub_get_switch_stats_ext,
    .clear_switch_stats = stub_clear_switch_stats,
    .switch_mdio_read = stub_switch_mdio_read,
    .switch_mdio_write = stub_switch_mdio_write,
    .create_switch_tunnel = stub_create_switch_tunnel,
    .remove_switch_tunnel = stub_remove_switch_tunnel,
    .set_switch_tunnel_attribute = stub_set_switch_tunnel_attribute,
    .get_switch_tunnel_attribute = stub_get_switch_tunnel_attribute,
    .switch_mdio_cl22_read = stub_switch_mdio_cl22_read,
    .switch_mdio_cl22_write = stub_switch_mdio_cl22_write,
};


/* SAI APIS for PORT */

static sai_status_t stub_create_port(
        _Out_ sai_object_id_t *port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_port(
        _In_ sai_object_id_t port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id);
}

static sai_status_t stub_set_port_attribute(
        _In_ sai_object_id_t port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, attr);
}

static sai_status_t stub_get_port_attribute(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, attr_count, attr_list);
}

static sai_status_t stub_get_port_stats(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_port_stats_ext(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_port_stats(
        _In_ sai_object_id_t port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),port_id, number_of_counters, counter_ids);
}

static sai_status_t stub_clear_port_all_stats(
        _In_ sai_object_id_t port_id)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for clearPortAllStats!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_status_t stub_create_port_pool(
        _Out_ sai_object_id_t *port_pool_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_port_pool(
        _In_ sai_object_id_t port_pool_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id);
}

static sai_status_t stub_set_port_pool_attribute(
        _In_ sai_object_id_t port_pool_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, attr);
}

static sai_status_t stub_get_port_pool_attribute(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, attr_count, attr_list);
}

static sai_status_t stub_get_port_pool_stats(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_port_pool_stats_ext(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_port_pool_stats(
        _In_ sai_object_id_t port_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_POOL),port_pool_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_port_connector(
        _Out_ sai_object_id_t *port_connector_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_CONNECTOR),port_connector_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_port_connector(
        _In_ sai_object_id_t port_connector_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_CONNECTOR),port_connector_id);
}

static sai_status_t stub_set_port_connector_attribute(
        _In_ sai_object_id_t port_connector_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_CONNECTOR),port_connector_id, attr);
}

static sai_status_t stub_get_port_connector_attribute(
        _In_ sai_object_id_t port_connector_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_CONNECTOR),port_connector_id, attr_count, attr_list);
}

static sai_status_t stub_create_port_serdes(
        _Out_ sai_object_id_t *port_serdes_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),port_serdes_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_port_serdes(
        _In_ sai_object_id_t port_serdes_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),port_serdes_id);
}

static sai_status_t stub_set_port_serdes_attribute(
        _In_ sai_object_id_t port_serdes_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),port_serdes_id, attr);
}

static sai_status_t stub_get_port_serdes_attribute(
        _In_ sai_object_id_t port_serdes_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),port_serdes_id, attr_count, attr_list);
}

static sai_status_t stub_create_ports(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_ports(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_ports_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_ports_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_PORT),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_create_port_serdess(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_port_serdess(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_port_serdess_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_port_serdess_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_SERDES),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_create_port_llr_profile(
        _Out_ sai_object_id_t *port_llr_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_LLR_PROFILE),port_llr_profile_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_port_llr_profile(
        _In_ sai_object_id_t port_llr_profile_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_LLR_PROFILE),port_llr_profile_id);
}

static sai_status_t stub_set_port_llr_profile_attribute(
        _In_ sai_object_id_t port_llr_profile_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_LLR_PROFILE),port_llr_profile_id, attr);
}

static sai_status_t stub_get_port_llr_profile_attribute(
        _In_ sai_object_id_t port_llr_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PORT_LLR_PROFILE),port_llr_profile_id, attr_count, attr_list);
}

static sai_port_api_t stub_port = {
    .create_port = stub_create_port,
    .remove_port = stub_remove_port,
    .set_port_attribute = stub_set_port_attribute,
    .get_port_attribute = stub_get_port_attribute,
    .get_port_stats = stub_get_port_stats,
    .get_port_stats_ext = stub_get_port_stats_ext,
    .clear_port_stats = stub_clear_port_stats,
    .clear_port_all_stats = stub_clear_port_all_stats,
    .create_port_pool = stub_create_port_pool,
    .remove_port_pool = stub_remove_port_pool,
    .set_port_pool_attribute = stub_set_port_pool_attribute,
    .get_port_pool_attribute = stub_get_port_pool_attribute,
    .get_port_pool_stats = stub_get_port_pool_stats,
    .get_port_pool_stats_ext = stub_get_port_pool_stats_ext,
    .clear_port_pool_stats = stub_clear_port_pool_stats,
    .create_port_connector = stub_create_port_connector,
    .remove_port_connector = stub_remove_port_connector,
    .set_port_connector_attribute = stub_set_port_connector_attribute,
    .get_port_connector_attribute = stub_get_port_connector_attribute,
    .create_port_serdes = stub_create_port_serdes,
    .remove_port_serdes = stub_remove_port_serdes,
    .set_port_serdes_attribute = stub_set_port_serdes_attribute,
    .get_port_serdes_attribute = stub_get_port_serdes_attribute,
    .create_ports = stub_create_ports,
    .remove_ports = stub_remove_ports,
    .set_ports_attribute = stub_set_ports_attribute,
    .get_ports_attribute = stub_get_ports_attribute,
    .create_port_serdess = stub_create_port_serdess,
    .remove_port_serdess = stub_remove_port_serdess,
    .set_port_serdess_attribute = stub_set_port_serdess_attribute,
    .get_port_serdess_attribute = stub_get_port_serdess_attribute,
    .create_port_llr_profile = stub_create_port_llr_profile,
    .remove_port_llr_profile = stub_remove_port_llr_profile,
    .set_port_llr_profile_attribute = stub_set_port_llr_profile_attribute,
    .get_port_llr_profile_attribute = stub_get_port_llr_profile_attribute,
};


/* SAI APIS for FDB */

static sai_status_t stub_create_fdb_entry(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(fdb_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_fdb_entry(
        _In_ const sai_fdb_entry_t *fdb_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(fdb_entry);
}

static sai_status_t stub_set_fdb_entry_attribute(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(fdb_entry, attr);
}

static sai_status_t stub_get_fdb_entry_attribute(
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(fdb_entry, attr_count, attr_list);
}

static sai_status_t stub_flush_fdb_entries(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->flushFdbEntries(switch_id, attr_count, attr_list);
}

static sai_status_t stub_create_fdb_entries(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, fdb_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_fdb_entries(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, fdb_entry, mode, object_statuses);
}

static sai_status_t stub_set_fdb_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, fdb_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_fdb_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, fdb_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_fdb_api_t stub_fdb = {
    .create_fdb_entry = stub_create_fdb_entry,
    .remove_fdb_entry = stub_remove_fdb_entry,
    .set_fdb_entry_attribute = stub_set_fdb_entry_attribute,
    .get_fdb_entry_attribute = stub_get_fdb_entry_attribute,
    .flush_fdb_entries = stub_flush_fdb_entries,
    .create_fdb_entries = stub_create_fdb_entries,
    .remove_fdb_entries = stub_remove_fdb_entries,
    .set_fdb_entries_attribute = stub_set_fdb_entries_attribute,
    .get_fdb_entries_attribute = stub_get_fdb_entries_attribute,
};


/* SAI APIS for VLAN */

static sai_status_t stub_create_vlan(
        _Out_ sai_object_id_t *vlan_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_vlan(
        _In_ sai_object_id_t vlan_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id);
}

static sai_status_t stub_set_vlan_attribute(
        _In_ sai_object_id_t vlan_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, attr);
}

static sai_status_t stub_get_vlan_attribute(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, attr_count, attr_list);
}

static sai_status_t stub_create_vlan_member(
        _Out_ sai_object_id_t *vlan_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),vlan_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_vlan_member(
        _In_ sai_object_id_t vlan_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),vlan_member_id);
}

static sai_status_t stub_set_vlan_member_attribute(
        _In_ sai_object_id_t vlan_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),vlan_member_id, attr);
}

static sai_status_t stub_get_vlan_member_attribute(
        _In_ sai_object_id_t vlan_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),vlan_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_vlan_members(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_vlan_members(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN_MEMBER),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_get_vlan_stats(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_vlan_stats_ext(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_vlan_stats(
        _In_ sai_object_id_t vlan_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_VLAN),vlan_id, number_of_counters, counter_ids);
}

static sai_vlan_api_t stub_vlan = {
    .create_vlan = stub_create_vlan,
    .remove_vlan = stub_remove_vlan,
    .set_vlan_attribute = stub_set_vlan_attribute,
    .get_vlan_attribute = stub_get_vlan_attribute,
    .create_vlan_member = stub_create_vlan_member,
    .remove_vlan_member = stub_remove_vlan_member,
    .set_vlan_member_attribute = stub_set_vlan_member_attribute,
    .get_vlan_member_attribute = stub_get_vlan_member_attribute,
    .create_vlan_members = stub_create_vlan_members,
    .remove_vlan_members = stub_remove_vlan_members,
    .get_vlan_stats = stub_get_vlan_stats,
    .get_vlan_stats_ext = stub_get_vlan_stats_ext,
    .clear_vlan_stats = stub_clear_vlan_stats,
};


/* SAI APIS for VIRTUAL_ROUTER */

static sai_status_t stub_create_virtual_router(
        _Out_ sai_object_id_t *virtual_router_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_VIRTUAL_ROUTER),virtual_router_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_virtual_router(
        _In_ sai_object_id_t virtual_router_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_VIRTUAL_ROUTER),virtual_router_id);
}

static sai_status_t stub_set_virtual_router_attribute(
        _In_ sai_object_id_t virtual_router_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_VIRTUAL_ROUTER),virtual_router_id, attr);
}

static sai_status_t stub_get_virtual_router_attribute(
        _In_ sai_object_id_t virtual_router_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_VIRTUAL_ROUTER),virtual_router_id, attr_count, attr_list);
}

static sai_virtual_router_api_t stub_virtual_router = {
    .create_virtual_router = stub_create_virtual_router,
    .remove_virtual_router = stub_remove_virtual_router,
    .set_virtual_router_attribute = stub_set_virtual_router_attribute,
    .get_virtual_router_attribute = stub_get_virtual_router_attribute,
};


/* SAI APIS for ROUTE */

static sai_status_t stub_create_route_entry(
        _In_ const sai_route_entry_t *route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(route_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_route_entry(
        _In_ const sai_route_entry_t *route_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(route_entry);
}

static sai_status_t stub_set_route_entry_attribute(
        _In_ const sai_route_entry_t *route_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(route_entry, attr);
}

static sai_status_t stub_get_route_entry_attribute(
        _In_ const sai_route_entry_t *route_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(route_entry, attr_count, attr_list);
}

static sai_status_t stub_create_route_entries(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, route_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_route_entries(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, route_entry, mode, object_statuses);
}

static sai_status_t stub_set_route_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, route_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_route_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_route_entry_t *route_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, route_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_route_api_t stub_route = {
    .create_route_entry = stub_create_route_entry,
    .remove_route_entry = stub_remove_route_entry,
    .set_route_entry_attribute = stub_set_route_entry_attribute,
    .get_route_entry_attribute = stub_get_route_entry_attribute,
    .create_route_entries = stub_create_route_entries,
    .remove_route_entries = stub_remove_route_entries,
    .set_route_entries_attribute = stub_set_route_entries_attribute,
    .get_route_entries_attribute = stub_get_route_entries_attribute,
};


/* SAI APIS for NEXT_HOP */

static sai_status_t stub_create_next_hop(
        _Out_ sai_object_id_t *next_hop_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),next_hop_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_next_hop(
        _In_ sai_object_id_t next_hop_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),next_hop_id);
}

static sai_status_t stub_set_next_hop_attribute(
        _In_ sai_object_id_t next_hop_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),next_hop_id, attr);
}

static sai_status_t stub_get_next_hop_attribute(
        _In_ sai_object_id_t next_hop_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),next_hop_id, attr_count, attr_list);
}

static sai_status_t stub_create_next_hops(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_next_hops(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_next_hops_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_next_hops_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_next_hop_api_t stub_next_hop = {
    .create_next_hop = stub_create_next_hop,
    .remove_next_hop = stub_remove_next_hop,
    .set_next_hop_attribute = stub_set_next_hop_attribute,
    .get_next_hop_attribute = stub_get_next_hop_attribute,
    .create_next_hops = stub_create_next_hops,
    .remove_next_hops = stub_remove_next_hops,
    .set_next_hops_attribute = stub_set_next_hops_attribute,
    .get_next_hops_attribute = stub_get_next_hops_attribute,
};


/* SAI APIS for NEXT_HOP_GROUP */

static sai_status_t stub_create_next_hop_group(
        _Out_ sai_object_id_t *next_hop_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),next_hop_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_next_hop_group(
        _In_ sai_object_id_t next_hop_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),next_hop_group_id);
}

static sai_status_t stub_set_next_hop_group_attribute(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),next_hop_group_id, attr);
}

static sai_status_t stub_get_next_hop_group_attribute(
        _In_ sai_object_id_t next_hop_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),next_hop_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_next_hop_group_member(
        _Out_ sai_object_id_t *next_hop_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),next_hop_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_next_hop_group_member(
        _In_ sai_object_id_t next_hop_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),next_hop_group_member_id);
}

static sai_status_t stub_set_next_hop_group_member_attribute(
        _In_ sai_object_id_t next_hop_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),next_hop_group_member_id, attr);
}

static sai_status_t stub_get_next_hop_group_member_attribute(
        _In_ sai_object_id_t next_hop_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),next_hop_group_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_next_hop_group_members(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_next_hop_group_members(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_next_hop_group_map(
        _Out_ sai_object_id_t *next_hop_group_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP),next_hop_group_map_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_next_hop_group_map(
        _In_ sai_object_id_t next_hop_group_map_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP),next_hop_group_map_id);
}

static sai_status_t stub_set_next_hop_group_map_attribute(
        _In_ sai_object_id_t next_hop_group_map_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP),next_hop_group_map_id, attr);
}

static sai_status_t stub_get_next_hop_group_map_attribute(
        _In_ sai_object_id_t next_hop_group_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MAP),next_hop_group_map_id, attr_count, attr_list);
}

static sai_status_t stub_set_next_hop_group_members_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_next_hop_group_members_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_create_next_hop_groups(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_next_hop_groups(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_next_hop_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_next_hop_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_NEXT_HOP_GROUP),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_next_hop_group_api_t stub_next_hop_group = {
    .create_next_hop_group = stub_create_next_hop_group,
    .remove_next_hop_group = stub_remove_next_hop_group,
    .set_next_hop_group_attribute = stub_set_next_hop_group_attribute,
    .get_next_hop_group_attribute = stub_get_next_hop_group_attribute,
    .create_next_hop_group_member = stub_create_next_hop_group_member,
    .remove_next_hop_group_member = stub_remove_next_hop_group_member,
    .set_next_hop_group_member_attribute = stub_set_next_hop_group_member_attribute,
    .get_next_hop_group_member_attribute = stub_get_next_hop_group_member_attribute,
    .create_next_hop_group_members = stub_create_next_hop_group_members,
    .remove_next_hop_group_members = stub_remove_next_hop_group_members,
    .create_next_hop_group_map = stub_create_next_hop_group_map,
    .remove_next_hop_group_map = stub_remove_next_hop_group_map,
    .set_next_hop_group_map_attribute = stub_set_next_hop_group_map_attribute,
    .get_next_hop_group_map_attribute = stub_get_next_hop_group_map_attribute,
    .set_next_hop_group_members_attribute = stub_set_next_hop_group_members_attribute,
    .get_next_hop_group_members_attribute = stub_get_next_hop_group_members_attribute,
    .create_next_hop_groups = stub_create_next_hop_groups,
    .remove_next_hop_groups = stub_remove_next_hop_groups,
    .set_next_hop_groups_attribute = stub_set_next_hop_groups_attribute,
    .get_next_hop_groups_attribute = stub_get_next_hop_groups_attribute,
};


/* SAI APIS for ROUTER_INTERFACE */

static sai_status_t stub_create_router_interface(
        _Out_ sai_object_id_t *router_interface_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_router_interface(
        _In_ sai_object_id_t router_interface_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id);
}

static sai_status_t stub_set_router_interface_attribute(
        _In_ sai_object_id_t router_interface_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, attr);
}

static sai_status_t stub_get_router_interface_attribute(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, attr_count, attr_list);
}

static sai_status_t stub_get_router_interface_stats(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_router_interface_stats_ext(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_router_interface_stats(
        _In_ sai_object_id_t router_interface_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),router_interface_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_router_interfaces(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_router_interfaces(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_router_interfaces_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_router_interfaces_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_ROUTER_INTERFACE),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_router_interface_api_t stub_router_interface = {
    .create_router_interface = stub_create_router_interface,
    .remove_router_interface = stub_remove_router_interface,
    .set_router_interface_attribute = stub_set_router_interface_attribute,
    .get_router_interface_attribute = stub_get_router_interface_attribute,
    .get_router_interface_stats = stub_get_router_interface_stats,
    .get_router_interface_stats_ext = stub_get_router_interface_stats_ext,
    .clear_router_interface_stats = stub_clear_router_interface_stats,
    .create_router_interfaces = stub_create_router_interfaces,
    .remove_router_interfaces = stub_remove_router_interfaces,
    .set_router_interfaces_attribute = stub_set_router_interfaces_attribute,
    .get_router_interfaces_attribute = stub_get_router_interfaces_attribute,
};


/* SAI APIS for NEIGHBOR */

static sai_status_t stub_create_neighbor_entry(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(neighbor_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_neighbor_entry(
        _In_ const sai_neighbor_entry_t *neighbor_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(neighbor_entry);
}

static sai_status_t stub_set_neighbor_entry_attribute(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(neighbor_entry, attr);
}

static sai_status_t stub_get_neighbor_entry_attribute(
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(neighbor_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_all_neighbor_entries(
        _In_ sai_object_id_t switch_id)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for removeAllNeighborEntries!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_status_t stub_create_neighbor_entries(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, neighbor_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_neighbor_entries(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, neighbor_entry, mode, object_statuses);
}

static sai_status_t stub_set_neighbor_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, neighbor_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_neighbor_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_neighbor_entry_t *neighbor_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, neighbor_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_neighbor_api_t stub_neighbor = {
    .create_neighbor_entry = stub_create_neighbor_entry,
    .remove_neighbor_entry = stub_remove_neighbor_entry,
    .set_neighbor_entry_attribute = stub_set_neighbor_entry_attribute,
    .get_neighbor_entry_attribute = stub_get_neighbor_entry_attribute,
    .remove_all_neighbor_entries = stub_remove_all_neighbor_entries,
    .create_neighbor_entries = stub_create_neighbor_entries,
    .remove_neighbor_entries = stub_remove_neighbor_entries,
    .set_neighbor_entries_attribute = stub_set_neighbor_entries_attribute,
    .get_neighbor_entries_attribute = stub_get_neighbor_entries_attribute,
};


/* SAI APIS for ACL */

static sai_status_t stub_create_acl_table(
        _Out_ sai_object_id_t *acl_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE),acl_table_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_table(
        _In_ sai_object_id_t acl_table_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE),acl_table_id);
}

static sai_status_t stub_set_acl_table_attribute(
        _In_ sai_object_id_t acl_table_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE),acl_table_id, attr);
}

static sai_status_t stub_get_acl_table_attribute(
        _In_ sai_object_id_t acl_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE),acl_table_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_entry(
        _Out_ sai_object_id_t *acl_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_ENTRY),acl_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_entry(
        _In_ sai_object_id_t acl_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_ENTRY),acl_entry_id);
}

static sai_status_t stub_set_acl_entry_attribute(
        _In_ sai_object_id_t acl_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_ENTRY),acl_entry_id, attr);
}

static sai_status_t stub_get_acl_entry_attribute(
        _In_ sai_object_id_t acl_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_ENTRY),acl_entry_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_counter(
        _Out_ sai_object_id_t *acl_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_COUNTER),acl_counter_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_counter(
        _In_ sai_object_id_t acl_counter_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_COUNTER),acl_counter_id);
}

static sai_status_t stub_set_acl_counter_attribute(
        _In_ sai_object_id_t acl_counter_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_COUNTER),acl_counter_id, attr);
}

static sai_status_t stub_get_acl_counter_attribute(
        _In_ sai_object_id_t acl_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_COUNTER),acl_counter_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_range(
        _Out_ sai_object_id_t *acl_range_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_RANGE),acl_range_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_range(
        _In_ sai_object_id_t acl_range_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_RANGE),acl_range_id);
}

static sai_status_t stub_set_acl_range_attribute(
        _In_ sai_object_id_t acl_range_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_RANGE),acl_range_id, attr);
}

static sai_status_t stub_get_acl_range_attribute(
        _In_ sai_object_id_t acl_range_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_RANGE),acl_range_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_table_group(
        _Out_ sai_object_id_t *acl_table_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP),acl_table_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_table_group(
        _In_ sai_object_id_t acl_table_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP),acl_table_group_id);
}

static sai_status_t stub_set_acl_table_group_attribute(
        _In_ sai_object_id_t acl_table_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP),acl_table_group_id, attr);
}

static sai_status_t stub_get_acl_table_group_attribute(
        _In_ sai_object_id_t acl_table_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP),acl_table_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_table_group_member(
        _Out_ sai_object_id_t *acl_table_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER),acl_table_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_table_group_member(
        _In_ sai_object_id_t acl_table_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER),acl_table_group_member_id);
}

static sai_status_t stub_set_acl_table_group_member_attribute(
        _In_ sai_object_id_t acl_table_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER),acl_table_group_member_id, attr);
}

static sai_status_t stub_get_acl_table_group_member_attribute(
        _In_ sai_object_id_t acl_table_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER),acl_table_group_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_acl_table_chain_group(
        _Out_ sai_object_id_t *acl_table_chain_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_CHAIN_GROUP),acl_table_chain_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_acl_table_chain_group(
        _In_ sai_object_id_t acl_table_chain_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_CHAIN_GROUP),acl_table_chain_group_id);
}

static sai_status_t stub_set_acl_table_chain_group_attribute(
        _In_ sai_object_id_t acl_table_chain_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_CHAIN_GROUP),acl_table_chain_group_id, attr);
}

static sai_status_t stub_get_acl_table_chain_group_attribute(
        _In_ sai_object_id_t acl_table_chain_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ACL_TABLE_CHAIN_GROUP),acl_table_chain_group_id, attr_count, attr_list);
}

static sai_acl_api_t stub_acl = {
    .create_acl_table = stub_create_acl_table,
    .remove_acl_table = stub_remove_acl_table,
    .set_acl_table_attribute = stub_set_acl_table_attribute,
    .get_acl_table_attribute = stub_get_acl_table_attribute,
    .create_acl_entry = stub_create_acl_entry,
    .remove_acl_entry = stub_remove_acl_entry,
    .set_acl_entry_attribute = stub_set_acl_entry_attribute,
    .get_acl_entry_attribute = stub_get_acl_entry_attribute,
    .create_acl_counter = stub_create_acl_counter,
    .remove_acl_counter = stub_remove_acl_counter,
    .set_acl_counter_attribute = stub_set_acl_counter_attribute,
    .get_acl_counter_attribute = stub_get_acl_counter_attribute,
    .create_acl_range = stub_create_acl_range,
    .remove_acl_range = stub_remove_acl_range,
    .set_acl_range_attribute = stub_set_acl_range_attribute,
    .get_acl_range_attribute = stub_get_acl_range_attribute,
    .create_acl_table_group = stub_create_acl_table_group,
    .remove_acl_table_group = stub_remove_acl_table_group,
    .set_acl_table_group_attribute = stub_set_acl_table_group_attribute,
    .get_acl_table_group_attribute = stub_get_acl_table_group_attribute,
    .create_acl_table_group_member = stub_create_acl_table_group_member,
    .remove_acl_table_group_member = stub_remove_acl_table_group_member,
    .set_acl_table_group_member_attribute = stub_set_acl_table_group_member_attribute,
    .get_acl_table_group_member_attribute = stub_get_acl_table_group_member_attribute,
    .create_acl_table_chain_group = stub_create_acl_table_chain_group,
    .remove_acl_table_chain_group = stub_remove_acl_table_chain_group,
    .set_acl_table_chain_group_attribute = stub_set_acl_table_chain_group_attribute,
    .get_acl_table_chain_group_attribute = stub_get_acl_table_chain_group_attribute,
};


/* SAI APIS for HOSTIF */

static sai_status_t stub_create_hostif(
        _Out_ sai_object_id_t *hostif_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF),hostif_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hostif(
        _In_ sai_object_id_t hostif_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF),hostif_id);
}

static sai_status_t stub_set_hostif_attribute(
        _In_ sai_object_id_t hostif_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF),hostif_id, attr);
}

static sai_status_t stub_get_hostif_attribute(
        _In_ sai_object_id_t hostif_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF),hostif_id, attr_count, attr_list);
}

static sai_status_t stub_create_hostif_table_entry(
        _Out_ sai_object_id_t *hostif_table_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY),hostif_table_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hostif_table_entry(
        _In_ sai_object_id_t hostif_table_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY),hostif_table_entry_id);
}

static sai_status_t stub_set_hostif_table_entry_attribute(
        _In_ sai_object_id_t hostif_table_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY),hostif_table_entry_id, attr);
}

static sai_status_t stub_get_hostif_table_entry_attribute(
        _In_ sai_object_id_t hostif_table_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY),hostif_table_entry_id, attr_count, attr_list);
}

static sai_status_t stub_create_hostif_trap_group(
        _Out_ sai_object_id_t *hostif_trap_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP),hostif_trap_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hostif_trap_group(
        _In_ sai_object_id_t hostif_trap_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP),hostif_trap_group_id);
}

static sai_status_t stub_set_hostif_trap_group_attribute(
        _In_ sai_object_id_t hostif_trap_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP),hostif_trap_group_id, attr);
}

static sai_status_t stub_get_hostif_trap_group_attribute(
        _In_ sai_object_id_t hostif_trap_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP),hostif_trap_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_hostif_trap(
        _Out_ sai_object_id_t *hostif_trap_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP),hostif_trap_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hostif_trap(
        _In_ sai_object_id_t hostif_trap_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP),hostif_trap_id);
}

static sai_status_t stub_set_hostif_trap_attribute(
        _In_ sai_object_id_t hostif_trap_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP),hostif_trap_id, attr);
}

static sai_status_t stub_get_hostif_trap_attribute(
        _In_ sai_object_id_t hostif_trap_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_TRAP),hostif_trap_id, attr_count, attr_list);
}

static sai_status_t stub_create_hostif_user_defined_trap(
        _Out_ sai_object_id_t *hostif_user_defined_trap_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP),hostif_user_defined_trap_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hostif_user_defined_trap(
        _In_ sai_object_id_t hostif_user_defined_trap_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP),hostif_user_defined_trap_id);
}

static sai_status_t stub_set_hostif_user_defined_trap_attribute(
        _In_ sai_object_id_t hostif_user_defined_trap_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP),hostif_user_defined_trap_id, attr);
}

static sai_status_t stub_get_hostif_user_defined_trap_attribute(
        _In_ sai_object_id_t hostif_user_defined_trap_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP),hostif_user_defined_trap_id, attr_count, attr_list);
}

static sai_status_t stub_recv_hostif_packet(
        _In_ sai_object_id_t hostif_id,
        _Inout_ sai_size_t *buffer_size,
        _Out_ void *buffer,
        _Inout_ uint32_t *attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for recvHostifPacket!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_status_t stub_send_hostif_packet(
        _In_ sai_object_id_t hostif_id,
        _In_ sai_size_t buffer_size,
        _In_ const void *buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for sendHostifPacket!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_status_t stub_allocate_hostif_packet(
        _In_ sai_object_id_t hostif_id,
        _In_ sai_size_t buffer_size,
        _Out_ void **buffer,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for allocateHostifPacket!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_status_t stub_free_hostif_packet(
        _In_ sai_object_id_t hostif_id,
        _Inout_ void *buffer)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for freeHostifPacket!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
static sai_hostif_api_t stub_hostif = {
    .create_hostif = stub_create_hostif,
    .remove_hostif = stub_remove_hostif,
    .set_hostif_attribute = stub_set_hostif_attribute,
    .get_hostif_attribute = stub_get_hostif_attribute,
    .create_hostif_table_entry = stub_create_hostif_table_entry,
    .remove_hostif_table_entry = stub_remove_hostif_table_entry,
    .set_hostif_table_entry_attribute = stub_set_hostif_table_entry_attribute,
    .get_hostif_table_entry_attribute = stub_get_hostif_table_entry_attribute,
    .create_hostif_trap_group = stub_create_hostif_trap_group,
    .remove_hostif_trap_group = stub_remove_hostif_trap_group,
    .set_hostif_trap_group_attribute = stub_set_hostif_trap_group_attribute,
    .get_hostif_trap_group_attribute = stub_get_hostif_trap_group_attribute,
    .create_hostif_trap = stub_create_hostif_trap,
    .remove_hostif_trap = stub_remove_hostif_trap,
    .set_hostif_trap_attribute = stub_set_hostif_trap_attribute,
    .get_hostif_trap_attribute = stub_get_hostif_trap_attribute,
    .create_hostif_user_defined_trap = stub_create_hostif_user_defined_trap,
    .remove_hostif_user_defined_trap = stub_remove_hostif_user_defined_trap,
    .set_hostif_user_defined_trap_attribute = stub_set_hostif_user_defined_trap_attribute,
    .get_hostif_user_defined_trap_attribute = stub_get_hostif_user_defined_trap_attribute,
    .recv_hostif_packet = stub_recv_hostif_packet,
    .send_hostif_packet = stub_send_hostif_packet,
    .allocate_hostif_packet = stub_allocate_hostif_packet,
    .free_hostif_packet = stub_free_hostif_packet,
};


/* SAI APIS for MIRROR */

static sai_status_t stub_create_mirror_session(
        _Out_ sai_object_id_t *mirror_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MIRROR_SESSION),mirror_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_mirror_session(
        _In_ sai_object_id_t mirror_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MIRROR_SESSION),mirror_session_id);
}

static sai_status_t stub_set_mirror_session_attribute(
        _In_ sai_object_id_t mirror_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MIRROR_SESSION),mirror_session_id, attr);
}

static sai_status_t stub_get_mirror_session_attribute(
        _In_ sai_object_id_t mirror_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MIRROR_SESSION),mirror_session_id, attr_count, attr_list);
}

static sai_mirror_api_t stub_mirror = {
    .create_mirror_session = stub_create_mirror_session,
    .remove_mirror_session = stub_remove_mirror_session,
    .set_mirror_session_attribute = stub_set_mirror_session_attribute,
    .get_mirror_session_attribute = stub_get_mirror_session_attribute,
};


/* SAI APIS for SAMPLEPACKET */

static sai_status_t stub_create_samplepacket(
        _Out_ sai_object_id_t *samplepacket_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SAMPLEPACKET),samplepacket_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_samplepacket(
        _In_ sai_object_id_t samplepacket_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SAMPLEPACKET),samplepacket_id);
}

static sai_status_t stub_set_samplepacket_attribute(
        _In_ sai_object_id_t samplepacket_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SAMPLEPACKET),samplepacket_id, attr);
}

static sai_status_t stub_get_samplepacket_attribute(
        _In_ sai_object_id_t samplepacket_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SAMPLEPACKET),samplepacket_id, attr_count, attr_list);
}

static sai_samplepacket_api_t stub_samplepacket = {
    .create_samplepacket = stub_create_samplepacket,
    .remove_samplepacket = stub_remove_samplepacket,
    .set_samplepacket_attribute = stub_set_samplepacket_attribute,
    .get_samplepacket_attribute = stub_get_samplepacket_attribute,
};


/* SAI APIS for STP */

static sai_status_t stub_create_stp(
        _Out_ sai_object_id_t *stp_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_STP),stp_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_stp(
        _In_ sai_object_id_t stp_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_STP),stp_id);
}

static sai_status_t stub_set_stp_attribute(
        _In_ sai_object_id_t stp_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_STP),stp_id, attr);
}

static sai_status_t stub_get_stp_attribute(
        _In_ sai_object_id_t stp_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_STP),stp_id, attr_count, attr_list);
}

static sai_status_t stub_create_stp_port(
        _Out_ sai_object_id_t *stp_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),stp_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_stp_port(
        _In_ sai_object_id_t stp_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),stp_port_id);
}

static sai_status_t stub_set_stp_port_attribute(
        _In_ sai_object_id_t stp_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),stp_port_id, attr);
}

static sai_status_t stub_get_stp_port_attribute(
        _In_ sai_object_id_t stp_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),stp_port_id, attr_count, attr_list);
}

static sai_status_t stub_create_stp_ports(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_stp_ports(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_STP_PORT),object_count, object_id, mode, object_statuses);
}

static sai_stp_api_t stub_stp = {
    .create_stp = stub_create_stp,
    .remove_stp = stub_remove_stp,
    .set_stp_attribute = stub_set_stp_attribute,
    .get_stp_attribute = stub_get_stp_attribute,
    .create_stp_port = stub_create_stp_port,
    .remove_stp_port = stub_remove_stp_port,
    .set_stp_port_attribute = stub_set_stp_port_attribute,
    .get_stp_port_attribute = stub_get_stp_port_attribute,
    .create_stp_ports = stub_create_stp_ports,
    .remove_stp_ports = stub_remove_stp_ports,
};


/* SAI APIS for LAG */

static sai_status_t stub_create_lag(
        _Out_ sai_object_id_t *lag_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_LAG),lag_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_lag(
        _In_ sai_object_id_t lag_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_LAG),lag_id);
}

static sai_status_t stub_set_lag_attribute(
        _In_ sai_object_id_t lag_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_LAG),lag_id, attr);
}

static sai_status_t stub_get_lag_attribute(
        _In_ sai_object_id_t lag_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_LAG),lag_id, attr_count, attr_list);
}

static sai_status_t stub_create_lag_member(
        _Out_ sai_object_id_t *lag_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),lag_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_lag_member(
        _In_ sai_object_id_t lag_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),lag_member_id);
}

static sai_status_t stub_set_lag_member_attribute(
        _In_ sai_object_id_t lag_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),lag_member_id, attr);
}

static sai_status_t stub_get_lag_member_attribute(
        _In_ sai_object_id_t lag_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),lag_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_lag_members(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_lag_members(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_LAG_MEMBER),object_count, object_id, mode, object_statuses);
}

static sai_lag_api_t stub_lag = {
    .create_lag = stub_create_lag,
    .remove_lag = stub_remove_lag,
    .set_lag_attribute = stub_set_lag_attribute,
    .get_lag_attribute = stub_get_lag_attribute,
    .create_lag_member = stub_create_lag_member,
    .remove_lag_member = stub_remove_lag_member,
    .set_lag_member_attribute = stub_set_lag_member_attribute,
    .get_lag_member_attribute = stub_get_lag_member_attribute,
    .create_lag_members = stub_create_lag_members,
    .remove_lag_members = stub_remove_lag_members,
};


/* SAI APIS for POLICER */

static sai_status_t stub_create_policer(
        _Out_ sai_object_id_t *policer_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_policer(
        _In_ sai_object_id_t policer_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id);
}

static sai_status_t stub_set_policer_attribute(
        _In_ sai_object_id_t policer_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, attr);
}

static sai_status_t stub_get_policer_attribute(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, attr_count, attr_list);
}

static sai_status_t stub_get_policer_stats(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_policer_stats_ext(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_policer_stats(
        _In_ sai_object_id_t policer_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_POLICER),policer_id, number_of_counters, counter_ids);
}

static sai_policer_api_t stub_policer = {
    .create_policer = stub_create_policer,
    .remove_policer = stub_remove_policer,
    .set_policer_attribute = stub_set_policer_attribute,
    .get_policer_attribute = stub_get_policer_attribute,
    .get_policer_stats = stub_get_policer_stats,
    .get_policer_stats_ext = stub_get_policer_stats_ext,
    .clear_policer_stats = stub_clear_policer_stats,
};


/* SAI APIS for WRED */

static sai_status_t stub_create_wred(
        _Out_ sai_object_id_t *wred_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_WRED),wred_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_wred(
        _In_ sai_object_id_t wred_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_WRED),wred_id);
}

static sai_status_t stub_set_wred_attribute(
        _In_ sai_object_id_t wred_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_WRED),wred_id, attr);
}

static sai_status_t stub_get_wred_attribute(
        _In_ sai_object_id_t wred_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_WRED),wred_id, attr_count, attr_list);
}

static sai_wred_api_t stub_wred = {
    .create_wred = stub_create_wred,
    .remove_wred = stub_remove_wred,
    .set_wred_attribute = stub_set_wred_attribute,
    .get_wred_attribute = stub_get_wred_attribute,
};


/* SAI APIS for QOS_MAP */

static sai_status_t stub_create_qos_map(
        _Out_ sai_object_id_t *qos_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_QOS_MAP),qos_map_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_qos_map(
        _In_ sai_object_id_t qos_map_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_QOS_MAP),qos_map_id);
}

static sai_status_t stub_set_qos_map_attribute(
        _In_ sai_object_id_t qos_map_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_QOS_MAP),qos_map_id, attr);
}

static sai_status_t stub_get_qos_map_attribute(
        _In_ sai_object_id_t qos_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_QOS_MAP),qos_map_id, attr_count, attr_list);
}

static sai_qos_map_api_t stub_qos_map = {
    .create_qos_map = stub_create_qos_map,
    .remove_qos_map = stub_remove_qos_map,
    .set_qos_map_attribute = stub_set_qos_map_attribute,
    .get_qos_map_attribute = stub_get_qos_map_attribute,
};


/* SAI APIS for QUEUE */

static sai_status_t stub_create_queue(
        _Out_ sai_object_id_t *queue_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_queue(
        _In_ sai_object_id_t queue_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id);
}

static sai_status_t stub_set_queue_attribute(
        _In_ sai_object_id_t queue_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, attr);
}

static sai_status_t stub_get_queue_attribute(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, attr_count, attr_list);
}

static sai_status_t stub_get_queue_stats(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_queue_stats_ext(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_queue_stats(
        _In_ sai_object_id_t queue_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),queue_id, number_of_counters, counter_ids);
}

static sai_status_t stub_set_queues_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_queues_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_QUEUE),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_queue_api_t stub_queue = {
    .create_queue = stub_create_queue,
    .remove_queue = stub_remove_queue,
    .set_queue_attribute = stub_set_queue_attribute,
    .get_queue_attribute = stub_get_queue_attribute,
    .get_queue_stats = stub_get_queue_stats,
    .get_queue_stats_ext = stub_get_queue_stats_ext,
    .clear_queue_stats = stub_clear_queue_stats,
    .set_queues_attribute = stub_set_queues_attribute,
    .get_queues_attribute = stub_get_queues_attribute,
};


/* SAI APIS for SCHEDULER */

static sai_status_t stub_create_scheduler(
        _Out_ sai_object_id_t *scheduler_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER),scheduler_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_scheduler(
        _In_ sai_object_id_t scheduler_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER),scheduler_id);
}

static sai_status_t stub_set_scheduler_attribute(
        _In_ sai_object_id_t scheduler_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER),scheduler_id, attr);
}

static sai_status_t stub_get_scheduler_attribute(
        _In_ sai_object_id_t scheduler_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER),scheduler_id, attr_count, attr_list);
}

static sai_scheduler_api_t stub_scheduler = {
    .create_scheduler = stub_create_scheduler,
    .remove_scheduler = stub_remove_scheduler,
    .set_scheduler_attribute = stub_set_scheduler_attribute,
    .get_scheduler_attribute = stub_get_scheduler_attribute,
};


/* SAI APIS for SCHEDULER_GROUP */

static sai_status_t stub_create_scheduler_group(
        _Out_ sai_object_id_t *scheduler_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER_GROUP),scheduler_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_scheduler_group(
        _In_ sai_object_id_t scheduler_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER_GROUP),scheduler_group_id);
}

static sai_status_t stub_set_scheduler_group_attribute(
        _In_ sai_object_id_t scheduler_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER_GROUP),scheduler_group_id, attr);
}

static sai_status_t stub_get_scheduler_group_attribute(
        _In_ sai_object_id_t scheduler_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SCHEDULER_GROUP),scheduler_group_id, attr_count, attr_list);
}

static sai_scheduler_group_api_t stub_scheduler_group = {
    .create_scheduler_group = stub_create_scheduler_group,
    .remove_scheduler_group = stub_remove_scheduler_group,
    .set_scheduler_group_attribute = stub_set_scheduler_group_attribute,
    .get_scheduler_group_attribute = stub_get_scheduler_group_attribute,
};


/* SAI APIS for BUFFER */

static sai_status_t stub_create_buffer_pool(
        _Out_ sai_object_id_t *buffer_pool_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_buffer_pool(
        _In_ sai_object_id_t buffer_pool_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id);
}

static sai_status_t stub_set_buffer_pool_attribute(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, attr);
}

static sai_status_t stub_get_buffer_pool_attribute(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, attr_count, attr_list);
}

static sai_status_t stub_get_buffer_pool_stats(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_buffer_pool_stats_ext(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_buffer_pool_stats(
        _In_ sai_object_id_t buffer_pool_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_POOL),buffer_pool_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_ingress_priority_group(
        _Out_ sai_object_id_t *ingress_priority_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ingress_priority_group(
        _In_ sai_object_id_t ingress_priority_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id);
}

static sai_status_t stub_set_ingress_priority_group_attribute(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, attr);
}

static sai_status_t stub_get_ingress_priority_group_attribute(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, attr_count, attr_list);
}

static sai_status_t stub_get_ingress_priority_group_stats(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_ingress_priority_group_stats_ext(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_ingress_priority_group_stats(
        _In_ sai_object_id_t ingress_priority_group_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),ingress_priority_group_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_buffer_profile(
        _Out_ sai_object_id_t *buffer_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_PROFILE),buffer_profile_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_buffer_profile(
        _In_ sai_object_id_t buffer_profile_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_PROFILE),buffer_profile_id);
}

static sai_status_t stub_set_buffer_profile_attribute(
        _In_ sai_object_id_t buffer_profile_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_PROFILE),buffer_profile_id, attr);
}

static sai_status_t stub_get_buffer_profile_attribute(
        _In_ sai_object_id_t buffer_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_BUFFER_PROFILE),buffer_profile_id, attr_count, attr_list);
}

static sai_status_t stub_set_ingress_priority_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_ingress_priority_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_buffer_api_t stub_buffer = {
    .create_buffer_pool = stub_create_buffer_pool,
    .remove_buffer_pool = stub_remove_buffer_pool,
    .set_buffer_pool_attribute = stub_set_buffer_pool_attribute,
    .get_buffer_pool_attribute = stub_get_buffer_pool_attribute,
    .get_buffer_pool_stats = stub_get_buffer_pool_stats,
    .get_buffer_pool_stats_ext = stub_get_buffer_pool_stats_ext,
    .clear_buffer_pool_stats = stub_clear_buffer_pool_stats,
    .create_ingress_priority_group = stub_create_ingress_priority_group,
    .remove_ingress_priority_group = stub_remove_ingress_priority_group,
    .set_ingress_priority_group_attribute = stub_set_ingress_priority_group_attribute,
    .get_ingress_priority_group_attribute = stub_get_ingress_priority_group_attribute,
    .get_ingress_priority_group_stats = stub_get_ingress_priority_group_stats,
    .get_ingress_priority_group_stats_ext = stub_get_ingress_priority_group_stats_ext,
    .clear_ingress_priority_group_stats = stub_clear_ingress_priority_group_stats,
    .create_buffer_profile = stub_create_buffer_profile,
    .remove_buffer_profile = stub_remove_buffer_profile,
    .set_buffer_profile_attribute = stub_set_buffer_profile_attribute,
    .get_buffer_profile_attribute = stub_get_buffer_profile_attribute,
    .set_ingress_priority_groups_attribute = stub_set_ingress_priority_groups_attribute,
    .get_ingress_priority_groups_attribute = stub_get_ingress_priority_groups_attribute,
};


/* SAI APIS for HASH */

static sai_status_t stub_create_hash(
        _Out_ sai_object_id_t *hash_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HASH),hash_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_hash(
        _In_ sai_object_id_t hash_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HASH),hash_id);
}

static sai_status_t stub_set_hash_attribute(
        _In_ sai_object_id_t hash_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HASH),hash_id, attr);
}

static sai_status_t stub_get_hash_attribute(
        _In_ sai_object_id_t hash_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HASH),hash_id, attr_count, attr_list);
}

static sai_status_t stub_create_fine_grained_hash_field(
        _Out_ sai_object_id_t *fine_grained_hash_field_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD),fine_grained_hash_field_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_fine_grained_hash_field(
        _In_ sai_object_id_t fine_grained_hash_field_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD),fine_grained_hash_field_id);
}

static sai_status_t stub_set_fine_grained_hash_field_attribute(
        _In_ sai_object_id_t fine_grained_hash_field_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD),fine_grained_hash_field_id, attr);
}

static sai_status_t stub_get_fine_grained_hash_field_attribute(
        _In_ sai_object_id_t fine_grained_hash_field_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_FINE_GRAINED_HASH_FIELD),fine_grained_hash_field_id, attr_count, attr_list);
}

static sai_hash_api_t stub_hash = {
    .create_hash = stub_create_hash,
    .remove_hash = stub_remove_hash,
    .set_hash_attribute = stub_set_hash_attribute,
    .get_hash_attribute = stub_get_hash_attribute,
    .create_fine_grained_hash_field = stub_create_fine_grained_hash_field,
    .remove_fine_grained_hash_field = stub_remove_fine_grained_hash_field,
    .set_fine_grained_hash_field_attribute = stub_set_fine_grained_hash_field_attribute,
    .get_fine_grained_hash_field_attribute = stub_get_fine_grained_hash_field_attribute,
};


/* SAI APIS for UDF */

static sai_status_t stub_create_udf(
        _Out_ sai_object_id_t *udf_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_UDF),udf_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_udf(
        _In_ sai_object_id_t udf_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_UDF),udf_id);
}

static sai_status_t stub_set_udf_attribute(
        _In_ sai_object_id_t udf_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_UDF),udf_id, attr);
}

static sai_status_t stub_get_udf_attribute(
        _In_ sai_object_id_t udf_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_UDF),udf_id, attr_count, attr_list);
}

static sai_status_t stub_create_udf_match(
        _Out_ sai_object_id_t *udf_match_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_MATCH),udf_match_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_udf_match(
        _In_ sai_object_id_t udf_match_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_MATCH),udf_match_id);
}

static sai_status_t stub_set_udf_match_attribute(
        _In_ sai_object_id_t udf_match_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_MATCH),udf_match_id, attr);
}

static sai_status_t stub_get_udf_match_attribute(
        _In_ sai_object_id_t udf_match_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_MATCH),udf_match_id, attr_count, attr_list);
}

static sai_status_t stub_create_udf_group(
        _Out_ sai_object_id_t *udf_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_GROUP),udf_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_udf_group(
        _In_ sai_object_id_t udf_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_GROUP),udf_group_id);
}

static sai_status_t stub_set_udf_group_attribute(
        _In_ sai_object_id_t udf_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_GROUP),udf_group_id, attr);
}

static sai_status_t stub_get_udf_group_attribute(
        _In_ sai_object_id_t udf_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_UDF_GROUP),udf_group_id, attr_count, attr_list);
}

static sai_udf_api_t stub_udf = {
    .create_udf = stub_create_udf,
    .remove_udf = stub_remove_udf,
    .set_udf_attribute = stub_set_udf_attribute,
    .get_udf_attribute = stub_get_udf_attribute,
    .create_udf_match = stub_create_udf_match,
    .remove_udf_match = stub_remove_udf_match,
    .set_udf_match_attribute = stub_set_udf_match_attribute,
    .get_udf_match_attribute = stub_get_udf_match_attribute,
    .create_udf_group = stub_create_udf_group,
    .remove_udf_group = stub_remove_udf_group,
    .set_udf_group_attribute = stub_set_udf_group_attribute,
    .get_udf_group_attribute = stub_get_udf_group_attribute,
};


/* SAI APIS for TUNNEL */

static sai_status_t stub_create_tunnel_map(
        _Out_ sai_object_id_t *tunnel_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP),tunnel_map_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tunnel_map(
        _In_ sai_object_id_t tunnel_map_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP),tunnel_map_id);
}

static sai_status_t stub_set_tunnel_map_attribute(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP),tunnel_map_id, attr);
}

static sai_status_t stub_get_tunnel_map_attribute(
        _In_ sai_object_id_t tunnel_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP),tunnel_map_id, attr_count, attr_list);
}

static sai_status_t stub_create_tunnel(
        _Out_ sai_object_id_t *tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tunnel(
        _In_ sai_object_id_t tunnel_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id);
}

static sai_status_t stub_set_tunnel_attribute(
        _In_ sai_object_id_t tunnel_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, attr);
}

static sai_status_t stub_get_tunnel_attribute(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, attr_count, attr_list);
}

static sai_status_t stub_get_tunnel_stats(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_tunnel_stats_ext(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_tunnel_stats(
        _In_ sai_object_id_t tunnel_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),tunnel_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_tunnel_term_table_entry(
        _Out_ sai_object_id_t *tunnel_term_table_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY),tunnel_term_table_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tunnel_term_table_entry(
        _In_ sai_object_id_t tunnel_term_table_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY),tunnel_term_table_entry_id);
}

static sai_status_t stub_set_tunnel_term_table_entry_attribute(
        _In_ sai_object_id_t tunnel_term_table_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY),tunnel_term_table_entry_id, attr);
}

static sai_status_t stub_get_tunnel_term_table_entry_attribute(
        _In_ sai_object_id_t tunnel_term_table_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY),tunnel_term_table_entry_id, attr_count, attr_list);
}

static sai_status_t stub_create_tunnel_map_entry(
        _Out_ sai_object_id_t *tunnel_map_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY),tunnel_map_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tunnel_map_entry(
        _In_ sai_object_id_t tunnel_map_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY),tunnel_map_entry_id);
}

static sai_status_t stub_set_tunnel_map_entry_attribute(
        _In_ sai_object_id_t tunnel_map_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY),tunnel_map_entry_id, attr);
}

static sai_status_t stub_get_tunnel_map_entry_attribute(
        _In_ sai_object_id_t tunnel_map_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY),tunnel_map_entry_id, attr_count, attr_list);
}

static sai_status_t stub_create_tunnels(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_tunnels(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_tunnels_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_tunnels_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_TUNNEL),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_tunnel_api_t stub_tunnel = {
    .create_tunnel_map = stub_create_tunnel_map,
    .remove_tunnel_map = stub_remove_tunnel_map,
    .set_tunnel_map_attribute = stub_set_tunnel_map_attribute,
    .get_tunnel_map_attribute = stub_get_tunnel_map_attribute,
    .create_tunnel = stub_create_tunnel,
    .remove_tunnel = stub_remove_tunnel,
    .set_tunnel_attribute = stub_set_tunnel_attribute,
    .get_tunnel_attribute = stub_get_tunnel_attribute,
    .get_tunnel_stats = stub_get_tunnel_stats,
    .get_tunnel_stats_ext = stub_get_tunnel_stats_ext,
    .clear_tunnel_stats = stub_clear_tunnel_stats,
    .create_tunnel_term_table_entry = stub_create_tunnel_term_table_entry,
    .remove_tunnel_term_table_entry = stub_remove_tunnel_term_table_entry,
    .set_tunnel_term_table_entry_attribute = stub_set_tunnel_term_table_entry_attribute,
    .get_tunnel_term_table_entry_attribute = stub_get_tunnel_term_table_entry_attribute,
    .create_tunnel_map_entry = stub_create_tunnel_map_entry,
    .remove_tunnel_map_entry = stub_remove_tunnel_map_entry,
    .set_tunnel_map_entry_attribute = stub_set_tunnel_map_entry_attribute,
    .get_tunnel_map_entry_attribute = stub_get_tunnel_map_entry_attribute,
    .create_tunnels = stub_create_tunnels,
    .remove_tunnels = stub_remove_tunnels,
    .set_tunnels_attribute = stub_set_tunnels_attribute,
    .get_tunnels_attribute = stub_get_tunnels_attribute,
};


/* SAI APIS for L2MC */

static sai_status_t stub_create_l2mc_entry(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(l2mc_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_l2mc_entry(
        _In_ const sai_l2mc_entry_t *l2mc_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(l2mc_entry);
}

static sai_status_t stub_set_l2mc_entry_attribute(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(l2mc_entry, attr);
}

static sai_status_t stub_get_l2mc_entry_attribute(
        _In_ const sai_l2mc_entry_t *l2mc_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(l2mc_entry, attr_count, attr_list);
}

static sai_l2mc_api_t stub_l2mc = {
    .create_l2mc_entry = stub_create_l2mc_entry,
    .remove_l2mc_entry = stub_remove_l2mc_entry,
    .set_l2mc_entry_attribute = stub_set_l2mc_entry_attribute,
    .get_l2mc_entry_attribute = stub_get_l2mc_entry_attribute,
};


/* SAI APIS for IPMC */

static sai_status_t stub_create_ipmc_entry(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(ipmc_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_ipmc_entry(
        _In_ const sai_ipmc_entry_t *ipmc_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(ipmc_entry);
}

static sai_status_t stub_set_ipmc_entry_attribute(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(ipmc_entry, attr);
}

static sai_status_t stub_get_ipmc_entry_attribute(
        _In_ const sai_ipmc_entry_t *ipmc_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(ipmc_entry, attr_count, attr_list);
}

static sai_ipmc_api_t stub_ipmc = {
    .create_ipmc_entry = stub_create_ipmc_entry,
    .remove_ipmc_entry = stub_remove_ipmc_entry,
    .set_ipmc_entry_attribute = stub_set_ipmc_entry_attribute,
    .get_ipmc_entry_attribute = stub_get_ipmc_entry_attribute,
};


/* SAI APIS for RPF_GROUP */

static sai_status_t stub_create_rpf_group(
        _Out_ sai_object_id_t *rpf_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP),rpf_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_rpf_group(
        _In_ sai_object_id_t rpf_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP),rpf_group_id);
}

static sai_status_t stub_set_rpf_group_attribute(
        _In_ sai_object_id_t rpf_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP),rpf_group_id, attr);
}

static sai_status_t stub_get_rpf_group_attribute(
        _In_ sai_object_id_t rpf_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP),rpf_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_rpf_group_member(
        _Out_ sai_object_id_t *rpf_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP_MEMBER),rpf_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_rpf_group_member(
        _In_ sai_object_id_t rpf_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP_MEMBER),rpf_group_member_id);
}

static sai_status_t stub_set_rpf_group_member_attribute(
        _In_ sai_object_id_t rpf_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP_MEMBER),rpf_group_member_id, attr);
}

static sai_status_t stub_get_rpf_group_member_attribute(
        _In_ sai_object_id_t rpf_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_RPF_GROUP_MEMBER),rpf_group_member_id, attr_count, attr_list);
}

static sai_rpf_group_api_t stub_rpf_group = {
    .create_rpf_group = stub_create_rpf_group,
    .remove_rpf_group = stub_remove_rpf_group,
    .set_rpf_group_attribute = stub_set_rpf_group_attribute,
    .get_rpf_group_attribute = stub_get_rpf_group_attribute,
    .create_rpf_group_member = stub_create_rpf_group_member,
    .remove_rpf_group_member = stub_remove_rpf_group_member,
    .set_rpf_group_member_attribute = stub_set_rpf_group_member_attribute,
    .get_rpf_group_member_attribute = stub_get_rpf_group_member_attribute,
};


/* SAI APIS for L2MC_GROUP */

static sai_status_t stub_create_l2mc_group(
        _Out_ sai_object_id_t *l2mc_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP),l2mc_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_l2mc_group(
        _In_ sai_object_id_t l2mc_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP),l2mc_group_id);
}

static sai_status_t stub_set_l2mc_group_attribute(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP),l2mc_group_id, attr);
}

static sai_status_t stub_get_l2mc_group_attribute(
        _In_ sai_object_id_t l2mc_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP),l2mc_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_l2mc_group_member(
        _Out_ sai_object_id_t *l2mc_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER),l2mc_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_l2mc_group_member(
        _In_ sai_object_id_t l2mc_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER),l2mc_group_member_id);
}

static sai_status_t stub_set_l2mc_group_member_attribute(
        _In_ sai_object_id_t l2mc_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER),l2mc_group_member_id, attr);
}

static sai_status_t stub_get_l2mc_group_member_attribute(
        _In_ sai_object_id_t l2mc_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER),l2mc_group_member_id, attr_count, attr_list);
}

static sai_l2mc_group_api_t stub_l2mc_group = {
    .create_l2mc_group = stub_create_l2mc_group,
    .remove_l2mc_group = stub_remove_l2mc_group,
    .set_l2mc_group_attribute = stub_set_l2mc_group_attribute,
    .get_l2mc_group_attribute = stub_get_l2mc_group_attribute,
    .create_l2mc_group_member = stub_create_l2mc_group_member,
    .remove_l2mc_group_member = stub_remove_l2mc_group_member,
    .set_l2mc_group_member_attribute = stub_set_l2mc_group_member_attribute,
    .get_l2mc_group_member_attribute = stub_get_l2mc_group_member_attribute,
};


/* SAI APIS for IPMC_GROUP */

static sai_status_t stub_create_ipmc_group(
        _Out_ sai_object_id_t *ipmc_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),ipmc_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ipmc_group(
        _In_ sai_object_id_t ipmc_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),ipmc_group_id);
}

static sai_status_t stub_set_ipmc_group_attribute(
        _In_ sai_object_id_t ipmc_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),ipmc_group_id, attr);
}

static sai_status_t stub_get_ipmc_group_attribute(
        _In_ sai_object_id_t ipmc_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),ipmc_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_ipmc_group_member(
        _Out_ sai_object_id_t *ipmc_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER),ipmc_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ipmc_group_member(
        _In_ sai_object_id_t ipmc_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER),ipmc_group_member_id);
}

static sai_status_t stub_set_ipmc_group_member_attribute(
        _In_ sai_object_id_t ipmc_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER),ipmc_group_member_id, attr);
}

static sai_status_t stub_get_ipmc_group_member_attribute(
        _In_ sai_object_id_t ipmc_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER),ipmc_group_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_ipmc_groups(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_ipmc_groups(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_set_ipmc_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),object_count, object_id, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_ipmc_groups_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet((sai_object_type_t)(SAI_OBJECT_TYPE_IPMC_GROUP),object_count, object_id, attr_count, attr_list, mode, object_statuses);
}

static sai_ipmc_group_api_t stub_ipmc_group = {
    .create_ipmc_group = stub_create_ipmc_group,
    .remove_ipmc_group = stub_remove_ipmc_group,
    .set_ipmc_group_attribute = stub_set_ipmc_group_attribute,
    .get_ipmc_group_attribute = stub_get_ipmc_group_attribute,
    .create_ipmc_group_member = stub_create_ipmc_group_member,
    .remove_ipmc_group_member = stub_remove_ipmc_group_member,
    .set_ipmc_group_member_attribute = stub_set_ipmc_group_member_attribute,
    .get_ipmc_group_member_attribute = stub_get_ipmc_group_member_attribute,
    .create_ipmc_groups = stub_create_ipmc_groups,
    .remove_ipmc_groups = stub_remove_ipmc_groups,
    .set_ipmc_groups_attribute = stub_set_ipmc_groups_attribute,
    .get_ipmc_groups_attribute = stub_get_ipmc_groups_attribute,
};


/* SAI APIS for MCAST_FDB */

static sai_status_t stub_create_mcast_fdb_entry(
        _In_ const sai_mcast_fdb_entry_t *mcast_fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(mcast_fdb_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_mcast_fdb_entry(
        _In_ const sai_mcast_fdb_entry_t *mcast_fdb_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(mcast_fdb_entry);
}

static sai_status_t stub_set_mcast_fdb_entry_attribute(
        _In_ const sai_mcast_fdb_entry_t *mcast_fdb_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(mcast_fdb_entry, attr);
}

static sai_status_t stub_get_mcast_fdb_entry_attribute(
        _In_ const sai_mcast_fdb_entry_t *mcast_fdb_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(mcast_fdb_entry, attr_count, attr_list);
}

static sai_mcast_fdb_api_t stub_mcast_fdb = {
    .create_mcast_fdb_entry = stub_create_mcast_fdb_entry,
    .remove_mcast_fdb_entry = stub_remove_mcast_fdb_entry,
    .set_mcast_fdb_entry_attribute = stub_set_mcast_fdb_entry_attribute,
    .get_mcast_fdb_entry_attribute = stub_get_mcast_fdb_entry_attribute,
};


/* SAI APIS for BRIDGE */

static sai_status_t stub_create_bridge(
        _Out_ sai_object_id_t *bridge_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_bridge(
        _In_ sai_object_id_t bridge_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id);
}

static sai_status_t stub_set_bridge_attribute(
        _In_ sai_object_id_t bridge_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, attr);
}

static sai_status_t stub_get_bridge_attribute(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, attr_count, attr_list);
}

static sai_status_t stub_get_bridge_stats(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_bridge_stats_ext(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_bridge_stats(
        _In_ sai_object_id_t bridge_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE),bridge_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_bridge_port(
        _Out_ sai_object_id_t *bridge_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_bridge_port(
        _In_ sai_object_id_t bridge_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id);
}

static sai_status_t stub_set_bridge_port_attribute(
        _In_ sai_object_id_t bridge_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, attr);
}

static sai_status_t stub_get_bridge_port_attribute(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, attr_count, attr_list);
}

static sai_status_t stub_get_bridge_port_stats(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_bridge_port_stats_ext(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_bridge_port_stats(
        _In_ sai_object_id_t bridge_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_BRIDGE_PORT),bridge_port_id, number_of_counters, counter_ids);
}

static sai_bridge_api_t stub_bridge = {
    .create_bridge = stub_create_bridge,
    .remove_bridge = stub_remove_bridge,
    .set_bridge_attribute = stub_set_bridge_attribute,
    .get_bridge_attribute = stub_get_bridge_attribute,
    .get_bridge_stats = stub_get_bridge_stats,
    .get_bridge_stats_ext = stub_get_bridge_stats_ext,
    .clear_bridge_stats = stub_clear_bridge_stats,
    .create_bridge_port = stub_create_bridge_port,
    .remove_bridge_port = stub_remove_bridge_port,
    .set_bridge_port_attribute = stub_set_bridge_port_attribute,
    .get_bridge_port_attribute = stub_get_bridge_port_attribute,
    .get_bridge_port_stats = stub_get_bridge_port_stats,
    .get_bridge_port_stats_ext = stub_get_bridge_port_stats_ext,
    .clear_bridge_port_stats = stub_clear_bridge_port_stats,
};


/* SAI APIS for TAM */

static sai_status_t stub_create_tam(
        _Out_ sai_object_id_t *tam_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM),tam_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam(
        _In_ sai_object_id_t tam_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM),tam_id);
}

static sai_status_t stub_set_tam_attribute(
        _In_ sai_object_id_t tam_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM),tam_id, attr);
}

static sai_status_t stub_get_tam_attribute(
        _In_ sai_object_id_t tam_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM),tam_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_math_func(
        _Out_ sai_object_id_t *tam_math_func_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_MATH_FUNC),tam_math_func_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_math_func(
        _In_ sai_object_id_t tam_math_func_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_MATH_FUNC),tam_math_func_id);
}

static sai_status_t stub_set_tam_math_func_attribute(
        _In_ sai_object_id_t tam_math_func_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_MATH_FUNC),tam_math_func_id, attr);
}

static sai_status_t stub_get_tam_math_func_attribute(
        _In_ sai_object_id_t tam_math_func_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_MATH_FUNC),tam_math_func_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_report(
        _Out_ sai_object_id_t *tam_report_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_REPORT),tam_report_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_report(
        _In_ sai_object_id_t tam_report_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_REPORT),tam_report_id);
}

static sai_status_t stub_set_tam_report_attribute(
        _In_ sai_object_id_t tam_report_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_REPORT),tam_report_id, attr);
}

static sai_status_t stub_get_tam_report_attribute(
        _In_ sai_object_id_t tam_report_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_REPORT),tam_report_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_event_threshold(
        _Out_ sai_object_id_t *tam_event_threshold_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD),tam_event_threshold_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_event_threshold(
        _In_ sai_object_id_t tam_event_threshold_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD),tam_event_threshold_id);
}

static sai_status_t stub_set_tam_event_threshold_attribute(
        _In_ sai_object_id_t tam_event_threshold_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD),tam_event_threshold_id, attr);
}

static sai_status_t stub_get_tam_event_threshold_attribute(
        _In_ sai_object_id_t tam_event_threshold_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_THRESHOLD),tam_event_threshold_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_int(
        _Out_ sai_object_id_t *tam_int_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_INT),tam_int_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_int(
        _In_ sai_object_id_t tam_int_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_INT),tam_int_id);
}

static sai_status_t stub_set_tam_int_attribute(
        _In_ sai_object_id_t tam_int_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_INT),tam_int_id, attr);
}

static sai_status_t stub_get_tam_int_attribute(
        _In_ sai_object_id_t tam_int_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_INT),tam_int_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_tel_type(
        _Out_ sai_object_id_t *tam_tel_type_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TEL_TYPE),tam_tel_type_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_tel_type(
        _In_ sai_object_id_t tam_tel_type_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TEL_TYPE),tam_tel_type_id);
}

static sai_status_t stub_set_tam_tel_type_attribute(
        _In_ sai_object_id_t tam_tel_type_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TEL_TYPE),tam_tel_type_id, attr);
}

static sai_status_t stub_get_tam_tel_type_attribute(
        _In_ sai_object_id_t tam_tel_type_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TEL_TYPE),tam_tel_type_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_transport(
        _Out_ sai_object_id_t *tam_transport_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TRANSPORT),tam_transport_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_transport(
        _In_ sai_object_id_t tam_transport_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TRANSPORT),tam_transport_id);
}

static sai_status_t stub_set_tam_transport_attribute(
        _In_ sai_object_id_t tam_transport_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TRANSPORT),tam_transport_id, attr);
}

static sai_status_t stub_get_tam_transport_attribute(
        _In_ sai_object_id_t tam_transport_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TRANSPORT),tam_transport_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_telemetry(
        _Out_ sai_object_id_t *tam_telemetry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TELEMETRY),tam_telemetry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_telemetry(
        _In_ sai_object_id_t tam_telemetry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TELEMETRY),tam_telemetry_id);
}

static sai_status_t stub_set_tam_telemetry_attribute(
        _In_ sai_object_id_t tam_telemetry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TELEMETRY),tam_telemetry_id, attr);
}

static sai_status_t stub_get_tam_telemetry_attribute(
        _In_ sai_object_id_t tam_telemetry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_TELEMETRY),tam_telemetry_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_collector(
        _Out_ sai_object_id_t *tam_collector_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COLLECTOR),tam_collector_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_collector(
        _In_ sai_object_id_t tam_collector_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COLLECTOR),tam_collector_id);
}

static sai_status_t stub_set_tam_collector_attribute(
        _In_ sai_object_id_t tam_collector_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COLLECTOR),tam_collector_id, attr);
}

static sai_status_t stub_get_tam_collector_attribute(
        _In_ sai_object_id_t tam_collector_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COLLECTOR),tam_collector_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_event_action(
        _Out_ sai_object_id_t *tam_event_action_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_ACTION),tam_event_action_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_event_action(
        _In_ sai_object_id_t tam_event_action_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_ACTION),tam_event_action_id);
}

static sai_status_t stub_set_tam_event_action_attribute(
        _In_ sai_object_id_t tam_event_action_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_ACTION),tam_event_action_id, attr);
}

static sai_status_t stub_get_tam_event_action_attribute(
        _In_ sai_object_id_t tam_event_action_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT_ACTION),tam_event_action_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_event(
        _Out_ sai_object_id_t *tam_event_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT),tam_event_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_event(
        _In_ sai_object_id_t tam_event_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT),tam_event_id);
}

static sai_status_t stub_set_tam_event_attribute(
        _In_ sai_object_id_t tam_event_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT),tam_event_id, attr);
}

static sai_status_t stub_get_tam_event_attribute(
        _In_ sai_object_id_t tam_event_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_EVENT),tam_event_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_counter_subscription(
        _Out_ sai_object_id_t *tam_counter_subscription_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),tam_counter_subscription_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_tam_counter_subscription(
        _In_ sai_object_id_t tam_counter_subscription_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),tam_counter_subscription_id);
}

static sai_status_t stub_set_tam_counter_subscription_attribute(
        _In_ sai_object_id_t tam_counter_subscription_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),tam_counter_subscription_id, attr);
}

static sai_status_t stub_get_tam_counter_subscription_attribute(
        _In_ sai_object_id_t tam_counter_subscription_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),tam_counter_subscription_id, attr_count, attr_list);
}

static sai_status_t stub_create_tam_counter_subscriptions(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_tam_counter_subscriptions(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_TAM_COUNTER_SUBSCRIPTION),object_count, object_id, mode, object_statuses);
}

static sai_tam_api_t stub_tam = {
    .create_tam = stub_create_tam,
    .remove_tam = stub_remove_tam,
    .set_tam_attribute = stub_set_tam_attribute,
    .get_tam_attribute = stub_get_tam_attribute,
    .create_tam_math_func = stub_create_tam_math_func,
    .remove_tam_math_func = stub_remove_tam_math_func,
    .set_tam_math_func_attribute = stub_set_tam_math_func_attribute,
    .get_tam_math_func_attribute = stub_get_tam_math_func_attribute,
    .create_tam_report = stub_create_tam_report,
    .remove_tam_report = stub_remove_tam_report,
    .set_tam_report_attribute = stub_set_tam_report_attribute,
    .get_tam_report_attribute = stub_get_tam_report_attribute,
    .create_tam_event_threshold = stub_create_tam_event_threshold,
    .remove_tam_event_threshold = stub_remove_tam_event_threshold,
    .set_tam_event_threshold_attribute = stub_set_tam_event_threshold_attribute,
    .get_tam_event_threshold_attribute = stub_get_tam_event_threshold_attribute,
    .create_tam_int = stub_create_tam_int,
    .remove_tam_int = stub_remove_tam_int,
    .set_tam_int_attribute = stub_set_tam_int_attribute,
    .get_tam_int_attribute = stub_get_tam_int_attribute,
    .create_tam_tel_type = stub_create_tam_tel_type,
    .remove_tam_tel_type = stub_remove_tam_tel_type,
    .set_tam_tel_type_attribute = stub_set_tam_tel_type_attribute,
    .get_tam_tel_type_attribute = stub_get_tam_tel_type_attribute,
    .create_tam_transport = stub_create_tam_transport,
    .remove_tam_transport = stub_remove_tam_transport,
    .set_tam_transport_attribute = stub_set_tam_transport_attribute,
    .get_tam_transport_attribute = stub_get_tam_transport_attribute,
    .create_tam_telemetry = stub_create_tam_telemetry,
    .remove_tam_telemetry = stub_remove_tam_telemetry,
    .set_tam_telemetry_attribute = stub_set_tam_telemetry_attribute,
    .get_tam_telemetry_attribute = stub_get_tam_telemetry_attribute,
    .create_tam_collector = stub_create_tam_collector,
    .remove_tam_collector = stub_remove_tam_collector,
    .set_tam_collector_attribute = stub_set_tam_collector_attribute,
    .get_tam_collector_attribute = stub_get_tam_collector_attribute,
    .create_tam_event_action = stub_create_tam_event_action,
    .remove_tam_event_action = stub_remove_tam_event_action,
    .set_tam_event_action_attribute = stub_set_tam_event_action_attribute,
    .get_tam_event_action_attribute = stub_get_tam_event_action_attribute,
    .create_tam_event = stub_create_tam_event,
    .remove_tam_event = stub_remove_tam_event,
    .set_tam_event_attribute = stub_set_tam_event_attribute,
    .get_tam_event_attribute = stub_get_tam_event_attribute,
    .create_tam_counter_subscription = stub_create_tam_counter_subscription,
    .remove_tam_counter_subscription = stub_remove_tam_counter_subscription,
    .set_tam_counter_subscription_attribute = stub_set_tam_counter_subscription_attribute,
    .get_tam_counter_subscription_attribute = stub_get_tam_counter_subscription_attribute,
    .create_tam_counter_subscriptions = stub_create_tam_counter_subscriptions,
    .remove_tam_counter_subscriptions = stub_remove_tam_counter_subscriptions,
};


/* SAI APIS for SRV6 */

static sai_status_t stub_create_srv6_sidlist(
        _Out_ sai_object_id_t *srv6_sidlist_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_srv6_sidlist(
        _In_ sai_object_id_t srv6_sidlist_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id);
}

static sai_status_t stub_set_srv6_sidlist_attribute(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, attr);
}

static sai_status_t stub_get_srv6_sidlist_attribute(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, attr_count, attr_list);
}

static sai_status_t stub_create_srv6_sidlists(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_srv6_sidlists(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_get_srv6_sidlist_stats(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_srv6_sidlist_stats_ext(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_srv6_sidlist_stats(
        _In_ sai_object_id_t srv6_sidlist_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_SRV6_SIDLIST),srv6_sidlist_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_my_sid_entry(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(my_sid_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_my_sid_entry(
        _In_ const sai_my_sid_entry_t *my_sid_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(my_sid_entry);
}

static sai_status_t stub_set_my_sid_entry_attribute(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(my_sid_entry, attr);
}

static sai_status_t stub_get_my_sid_entry_attribute(
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(my_sid_entry, attr_count, attr_list);
}

static sai_status_t stub_create_my_sid_entries(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, my_sid_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_my_sid_entries(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, my_sid_entry, mode, object_statuses);
}

static sai_status_t stub_set_my_sid_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, my_sid_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_my_sid_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_my_sid_entry_t *my_sid_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, my_sid_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_srv6_api_t stub_srv6 = {
    .create_srv6_sidlist = stub_create_srv6_sidlist,
    .remove_srv6_sidlist = stub_remove_srv6_sidlist,
    .set_srv6_sidlist_attribute = stub_set_srv6_sidlist_attribute,
    .get_srv6_sidlist_attribute = stub_get_srv6_sidlist_attribute,
    .create_srv6_sidlists = stub_create_srv6_sidlists,
    .remove_srv6_sidlists = stub_remove_srv6_sidlists,
    .get_srv6_sidlist_stats = stub_get_srv6_sidlist_stats,
    .get_srv6_sidlist_stats_ext = stub_get_srv6_sidlist_stats_ext,
    .clear_srv6_sidlist_stats = stub_clear_srv6_sidlist_stats,
    .create_my_sid_entry = stub_create_my_sid_entry,
    .remove_my_sid_entry = stub_remove_my_sid_entry,
    .set_my_sid_entry_attribute = stub_set_my_sid_entry_attribute,
    .get_my_sid_entry_attribute = stub_get_my_sid_entry_attribute,
    .create_my_sid_entries = stub_create_my_sid_entries,
    .remove_my_sid_entries = stub_remove_my_sid_entries,
    .set_my_sid_entries_attribute = stub_set_my_sid_entries_attribute,
    .get_my_sid_entries_attribute = stub_get_my_sid_entries_attribute,
};


/* SAI APIS for MPLS */

static sai_status_t stub_create_inseg_entry(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(inseg_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_inseg_entry(
        _In_ const sai_inseg_entry_t *inseg_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(inseg_entry);
}

static sai_status_t stub_set_inseg_entry_attribute(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(inseg_entry, attr);
}

static sai_status_t stub_get_inseg_entry_attribute(
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(inseg_entry, attr_count, attr_list);
}

static sai_status_t stub_create_inseg_entries(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, inseg_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_inseg_entries(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, inseg_entry, mode, object_statuses);
}

static sai_status_t stub_set_inseg_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, inseg_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_inseg_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_inseg_entry_t *inseg_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, inseg_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_mpls_api_t stub_mpls = {
    .create_inseg_entry = stub_create_inseg_entry,
    .remove_inseg_entry = stub_remove_inseg_entry,
    .set_inseg_entry_attribute = stub_set_inseg_entry_attribute,
    .get_inseg_entry_attribute = stub_get_inseg_entry_attribute,
    .create_inseg_entries = stub_create_inseg_entries,
    .remove_inseg_entries = stub_remove_inseg_entries,
    .set_inseg_entries_attribute = stub_set_inseg_entries_attribute,
    .get_inseg_entries_attribute = stub_get_inseg_entries_attribute,
};


/* SAI APIS for DTEL */

static sai_status_t stub_create_dtel(
        _Out_ sai_object_id_t *dtel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL),dtel_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dtel(
        _In_ sai_object_id_t dtel_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL),dtel_id);
}

static sai_status_t stub_set_dtel_attribute(
        _In_ sai_object_id_t dtel_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL),dtel_id, attr);
}

static sai_status_t stub_get_dtel_attribute(
        _In_ sai_object_id_t dtel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL),dtel_id, attr_count, attr_list);
}

static sai_status_t stub_create_dtel_queue_report(
        _Out_ sai_object_id_t *dtel_queue_report_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT),dtel_queue_report_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dtel_queue_report(
        _In_ sai_object_id_t dtel_queue_report_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT),dtel_queue_report_id);
}

static sai_status_t stub_set_dtel_queue_report_attribute(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT),dtel_queue_report_id, attr);
}

static sai_status_t stub_get_dtel_queue_report_attribute(
        _In_ sai_object_id_t dtel_queue_report_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_QUEUE_REPORT),dtel_queue_report_id, attr_count, attr_list);
}

static sai_status_t stub_create_dtel_int_session(
        _Out_ sai_object_id_t *dtel_int_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_INT_SESSION),dtel_int_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dtel_int_session(
        _In_ sai_object_id_t dtel_int_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_INT_SESSION),dtel_int_session_id);
}

static sai_status_t stub_set_dtel_int_session_attribute(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_INT_SESSION),dtel_int_session_id, attr);
}

static sai_status_t stub_get_dtel_int_session_attribute(
        _In_ sai_object_id_t dtel_int_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_INT_SESSION),dtel_int_session_id, attr_count, attr_list);
}

static sai_status_t stub_create_dtel_report_session(
        _Out_ sai_object_id_t *dtel_report_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_REPORT_SESSION),dtel_report_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dtel_report_session(
        _In_ sai_object_id_t dtel_report_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_REPORT_SESSION),dtel_report_session_id);
}

static sai_status_t stub_set_dtel_report_session_attribute(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_REPORT_SESSION),dtel_report_session_id, attr);
}

static sai_status_t stub_get_dtel_report_session_attribute(
        _In_ sai_object_id_t dtel_report_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_REPORT_SESSION),dtel_report_session_id, attr_count, attr_list);
}

static sai_status_t stub_create_dtel_event(
        _Out_ sai_object_id_t *dtel_event_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_EVENT),dtel_event_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dtel_event(
        _In_ sai_object_id_t dtel_event_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_EVENT),dtel_event_id);
}

static sai_status_t stub_set_dtel_event_attribute(
        _In_ sai_object_id_t dtel_event_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_EVENT),dtel_event_id, attr);
}

static sai_status_t stub_get_dtel_event_attribute(
        _In_ sai_object_id_t dtel_event_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DTEL_EVENT),dtel_event_id, attr_count, attr_list);
}

static sai_dtel_api_t stub_dtel = {
    .create_dtel = stub_create_dtel,
    .remove_dtel = stub_remove_dtel,
    .set_dtel_attribute = stub_set_dtel_attribute,
    .get_dtel_attribute = stub_get_dtel_attribute,
    .create_dtel_queue_report = stub_create_dtel_queue_report,
    .remove_dtel_queue_report = stub_remove_dtel_queue_report,
    .set_dtel_queue_report_attribute = stub_set_dtel_queue_report_attribute,
    .get_dtel_queue_report_attribute = stub_get_dtel_queue_report_attribute,
    .create_dtel_int_session = stub_create_dtel_int_session,
    .remove_dtel_int_session = stub_remove_dtel_int_session,
    .set_dtel_int_session_attribute = stub_set_dtel_int_session_attribute,
    .get_dtel_int_session_attribute = stub_get_dtel_int_session_attribute,
    .create_dtel_report_session = stub_create_dtel_report_session,
    .remove_dtel_report_session = stub_remove_dtel_report_session,
    .set_dtel_report_session_attribute = stub_set_dtel_report_session_attribute,
    .get_dtel_report_session_attribute = stub_get_dtel_report_session_attribute,
    .create_dtel_event = stub_create_dtel_event,
    .remove_dtel_event = stub_remove_dtel_event,
    .set_dtel_event_attribute = stub_set_dtel_event_attribute,
    .get_dtel_event_attribute = stub_get_dtel_event_attribute,
};


/* SAI APIS for BFD */

static sai_status_t stub_create_bfd_session(
        _Out_ sai_object_id_t *bfd_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_bfd_session(
        _In_ sai_object_id_t bfd_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id);
}

static sai_status_t stub_set_bfd_session_attribute(
        _In_ sai_object_id_t bfd_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, attr);
}

static sai_status_t stub_get_bfd_session_attribute(
        _In_ sai_object_id_t bfd_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, attr_count, attr_list);
}

static sai_status_t stub_get_bfd_session_stats(
        _In_ sai_object_id_t bfd_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_bfd_session_stats_ext(
        _In_ sai_object_id_t bfd_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_bfd_session_stats(
        _In_ sai_object_id_t bfd_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_BFD_SESSION),bfd_session_id, number_of_counters, counter_ids);
}

static sai_bfd_api_t stub_bfd = {
    .create_bfd_session = stub_create_bfd_session,
    .remove_bfd_session = stub_remove_bfd_session,
    .set_bfd_session_attribute = stub_set_bfd_session_attribute,
    .get_bfd_session_attribute = stub_get_bfd_session_attribute,
    .get_bfd_session_stats = stub_get_bfd_session_stats,
    .get_bfd_session_stats_ext = stub_get_bfd_session_stats_ext,
    .clear_bfd_session_stats = stub_clear_bfd_session_stats,
};


/* SAI APIS for ISOLATION_GROUP */

static sai_status_t stub_create_isolation_group(
        _Out_ sai_object_id_t *isolation_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP),isolation_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_isolation_group(
        _In_ sai_object_id_t isolation_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP),isolation_group_id);
}

static sai_status_t stub_set_isolation_group_attribute(
        _In_ sai_object_id_t isolation_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP),isolation_group_id, attr);
}

static sai_status_t stub_get_isolation_group_attribute(
        _In_ sai_object_id_t isolation_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP),isolation_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_isolation_group_member(
        _Out_ sai_object_id_t *isolation_group_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER),isolation_group_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_isolation_group_member(
        _In_ sai_object_id_t isolation_group_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER),isolation_group_member_id);
}

static sai_status_t stub_set_isolation_group_member_attribute(
        _In_ sai_object_id_t isolation_group_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER),isolation_group_member_id, attr);
}

static sai_status_t stub_get_isolation_group_member_attribute(
        _In_ sai_object_id_t isolation_group_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ISOLATION_GROUP_MEMBER),isolation_group_member_id, attr_count, attr_list);
}

static sai_isolation_group_api_t stub_isolation_group = {
    .create_isolation_group = stub_create_isolation_group,
    .remove_isolation_group = stub_remove_isolation_group,
    .set_isolation_group_attribute = stub_set_isolation_group_attribute,
    .get_isolation_group_attribute = stub_get_isolation_group_attribute,
    .create_isolation_group_member = stub_create_isolation_group_member,
    .remove_isolation_group_member = stub_remove_isolation_group_member,
    .set_isolation_group_member_attribute = stub_set_isolation_group_member_attribute,
    .get_isolation_group_member_attribute = stub_get_isolation_group_member_attribute,
};


/* SAI APIS for NAT */

static sai_status_t stub_create_nat_entry(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(nat_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_nat_entry(
        _In_ const sai_nat_entry_t *nat_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(nat_entry);
}

static sai_status_t stub_set_nat_entry_attribute(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(nat_entry, attr);
}

static sai_status_t stub_get_nat_entry_attribute(
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(nat_entry, attr_count, attr_list);
}

static sai_status_t stub_create_nat_entries(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, nat_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_nat_entries(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, nat_entry, mode, object_statuses);
}

static sai_status_t stub_set_nat_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const sai_attribute_t *attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkSet(object_count, nat_entry, attr_list, mode, object_statuses);
}

static sai_status_t stub_get_nat_entries_attribute(
        _In_ uint32_t object_count,
        _In_ const sai_nat_entry_t *nat_entry,
        _In_ const uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkGet(object_count, nat_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_create_nat_zone_counter(
        _Out_ sai_object_id_t *nat_zone_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_NAT_ZONE_COUNTER),nat_zone_counter_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_nat_zone_counter(
        _In_ sai_object_id_t nat_zone_counter_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_NAT_ZONE_COUNTER),nat_zone_counter_id);
}

static sai_status_t stub_set_nat_zone_counter_attribute(
        _In_ sai_object_id_t nat_zone_counter_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_NAT_ZONE_COUNTER),nat_zone_counter_id, attr);
}

static sai_status_t stub_get_nat_zone_counter_attribute(
        _In_ sai_object_id_t nat_zone_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_NAT_ZONE_COUNTER),nat_zone_counter_id, attr_count, attr_list);
}

static sai_nat_api_t stub_nat = {
    .create_nat_entry = stub_create_nat_entry,
    .remove_nat_entry = stub_remove_nat_entry,
    .set_nat_entry_attribute = stub_set_nat_entry_attribute,
    .get_nat_entry_attribute = stub_get_nat_entry_attribute,
    .create_nat_entries = stub_create_nat_entries,
    .remove_nat_entries = stub_remove_nat_entries,
    .set_nat_entries_attribute = stub_set_nat_entries_attribute,
    .get_nat_entries_attribute = stub_get_nat_entries_attribute,
    .create_nat_zone_counter = stub_create_nat_zone_counter,
    .remove_nat_zone_counter = stub_remove_nat_zone_counter,
    .set_nat_zone_counter_attribute = stub_set_nat_zone_counter_attribute,
    .get_nat_zone_counter_attribute = stub_get_nat_zone_counter_attribute,
};


/* SAI APIS for COUNTER */

static sai_status_t stub_create_counter(
        _Out_ sai_object_id_t *counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_counter(
        _In_ sai_object_id_t counter_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id);
}

static sai_status_t stub_set_counter_attribute(
        _In_ sai_object_id_t counter_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, attr);
}

static sai_status_t stub_get_counter_attribute(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, attr_count, attr_list);
}

static sai_status_t stub_get_counter_stats(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_counter_stats_ext(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_counter_stats(
        _In_ sai_object_id_t counter_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_COUNTER),counter_id, number_of_counters, counter_ids);
}

static sai_counter_api_t stub_counter = {
    .create_counter = stub_create_counter,
    .remove_counter = stub_remove_counter,
    .set_counter_attribute = stub_set_counter_attribute,
    .get_counter_attribute = stub_get_counter_attribute,
    .get_counter_stats = stub_get_counter_stats,
    .get_counter_stats_ext = stub_get_counter_stats_ext,
    .clear_counter_stats = stub_clear_counter_stats,
};


/* SAI APIS for DEBUG_COUNTER */

static sai_status_t stub_create_debug_counter(
        _Out_ sai_object_id_t *debug_counter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DEBUG_COUNTER),debug_counter_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_debug_counter(
        _In_ sai_object_id_t debug_counter_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DEBUG_COUNTER),debug_counter_id);
}

static sai_status_t stub_set_debug_counter_attribute(
        _In_ sai_object_id_t debug_counter_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DEBUG_COUNTER),debug_counter_id, attr);
}

static sai_status_t stub_get_debug_counter_attribute(
        _In_ sai_object_id_t debug_counter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DEBUG_COUNTER),debug_counter_id, attr_count, attr_list);
}

static sai_debug_counter_api_t stub_debug_counter = {
    .create_debug_counter = stub_create_debug_counter,
    .remove_debug_counter = stub_remove_debug_counter,
    .set_debug_counter_attribute = stub_set_debug_counter_attribute,
    .get_debug_counter_attribute = stub_get_debug_counter_attribute,
};


/* SAI APIS for MACSEC */

static sai_status_t stub_create_macsec(
        _Out_ sai_object_id_t *macsec_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC),macsec_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_macsec(
        _In_ sai_object_id_t macsec_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC),macsec_id);
}

static sai_status_t stub_set_macsec_attribute(
        _In_ sai_object_id_t macsec_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC),macsec_id, attr);
}

static sai_status_t stub_get_macsec_attribute(
        _In_ sai_object_id_t macsec_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC),macsec_id, attr_count, attr_list);
}

static sai_status_t stub_create_macsec_port(
        _Out_ sai_object_id_t *macsec_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_macsec_port(
        _In_ sai_object_id_t macsec_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id);
}

static sai_status_t stub_set_macsec_port_attribute(
        _In_ sai_object_id_t macsec_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, attr);
}

static sai_status_t stub_get_macsec_port_attribute(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, attr_count, attr_list);
}

static sai_status_t stub_get_macsec_port_stats(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_macsec_port_stats_ext(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_macsec_port_stats(
        _In_ sai_object_id_t macsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_PORT),macsec_port_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_macsec_flow(
        _Out_ sai_object_id_t *macsec_flow_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_macsec_flow(
        _In_ sai_object_id_t macsec_flow_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id);
}

static sai_status_t stub_set_macsec_flow_attribute(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, attr);
}

static sai_status_t stub_get_macsec_flow_attribute(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, attr_count, attr_list);
}

static sai_status_t stub_get_macsec_flow_stats(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_macsec_flow_stats_ext(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_macsec_flow_stats(
        _In_ sai_object_id_t macsec_flow_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_FLOW),macsec_flow_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_macsec_sc(
        _Out_ sai_object_id_t *macsec_sc_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_macsec_sc(
        _In_ sai_object_id_t macsec_sc_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id);
}

static sai_status_t stub_set_macsec_sc_attribute(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, attr);
}

static sai_status_t stub_get_macsec_sc_attribute(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, attr_count, attr_list);
}

static sai_status_t stub_get_macsec_sc_stats(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_macsec_sc_stats_ext(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_macsec_sc_stats(
        _In_ sai_object_id_t macsec_sc_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SC),macsec_sc_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_macsec_sa(
        _Out_ sai_object_id_t *macsec_sa_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_macsec_sa(
        _In_ sai_object_id_t macsec_sa_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id);
}

static sai_status_t stub_set_macsec_sa_attribute(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, attr);
}

static sai_status_t stub_get_macsec_sa_attribute(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, attr_count, attr_list);
}

static sai_status_t stub_get_macsec_sa_stats(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_macsec_sa_stats_ext(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_macsec_sa_stats(
        _In_ sai_object_id_t macsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_MACSEC_SA),macsec_sa_id, number_of_counters, counter_ids);
}

static sai_macsec_api_t stub_macsec = {
    .create_macsec = stub_create_macsec,
    .remove_macsec = stub_remove_macsec,
    .set_macsec_attribute = stub_set_macsec_attribute,
    .get_macsec_attribute = stub_get_macsec_attribute,
    .create_macsec_port = stub_create_macsec_port,
    .remove_macsec_port = stub_remove_macsec_port,
    .set_macsec_port_attribute = stub_set_macsec_port_attribute,
    .get_macsec_port_attribute = stub_get_macsec_port_attribute,
    .get_macsec_port_stats = stub_get_macsec_port_stats,
    .get_macsec_port_stats_ext = stub_get_macsec_port_stats_ext,
    .clear_macsec_port_stats = stub_clear_macsec_port_stats,
    .create_macsec_flow = stub_create_macsec_flow,
    .remove_macsec_flow = stub_remove_macsec_flow,
    .set_macsec_flow_attribute = stub_set_macsec_flow_attribute,
    .get_macsec_flow_attribute = stub_get_macsec_flow_attribute,
    .get_macsec_flow_stats = stub_get_macsec_flow_stats,
    .get_macsec_flow_stats_ext = stub_get_macsec_flow_stats_ext,
    .clear_macsec_flow_stats = stub_clear_macsec_flow_stats,
    .create_macsec_sc = stub_create_macsec_sc,
    .remove_macsec_sc = stub_remove_macsec_sc,
    .set_macsec_sc_attribute = stub_set_macsec_sc_attribute,
    .get_macsec_sc_attribute = stub_get_macsec_sc_attribute,
    .get_macsec_sc_stats = stub_get_macsec_sc_stats,
    .get_macsec_sc_stats_ext = stub_get_macsec_sc_stats_ext,
    .clear_macsec_sc_stats = stub_clear_macsec_sc_stats,
    .create_macsec_sa = stub_create_macsec_sa,
    .remove_macsec_sa = stub_remove_macsec_sa,
    .set_macsec_sa_attribute = stub_set_macsec_sa_attribute,
    .get_macsec_sa_attribute = stub_get_macsec_sa_attribute,
    .get_macsec_sa_stats = stub_get_macsec_sa_stats,
    .get_macsec_sa_stats_ext = stub_get_macsec_sa_stats_ext,
    .clear_macsec_sa_stats = stub_clear_macsec_sa_stats,
};


/* SAI APIS for SYSTEM_PORT */

static sai_status_t stub_create_system_port(
        _Out_ sai_object_id_t *system_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SYSTEM_PORT),system_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_system_port(
        _In_ sai_object_id_t system_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SYSTEM_PORT),system_port_id);
}

static sai_status_t stub_set_system_port_attribute(
        _In_ sai_object_id_t system_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SYSTEM_PORT),system_port_id, attr);
}

static sai_status_t stub_get_system_port_attribute(
        _In_ sai_object_id_t system_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SYSTEM_PORT),system_port_id, attr_count, attr_list);
}

static sai_system_port_api_t stub_system_port = {
    .create_system_port = stub_create_system_port,
    .remove_system_port = stub_remove_system_port,
    .set_system_port_attribute = stub_set_system_port_attribute,
    .get_system_port_attribute = stub_get_system_port_attribute,
};


/* SAI APIS for MY_MAC */

static sai_status_t stub_create_my_mac(
        _Out_ sai_object_id_t *my_mac_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_MY_MAC),my_mac_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_my_mac(
        _In_ sai_object_id_t my_mac_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_MY_MAC),my_mac_id);
}

static sai_status_t stub_set_my_mac_attribute(
        _In_ sai_object_id_t my_mac_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_MY_MAC),my_mac_id, attr);
}

static sai_status_t stub_get_my_mac_attribute(
        _In_ sai_object_id_t my_mac_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_MY_MAC),my_mac_id, attr_count, attr_list);
}

static sai_my_mac_api_t stub_my_mac = {
    .create_my_mac = stub_create_my_mac,
    .remove_my_mac = stub_remove_my_mac,
    .set_my_mac_attribute = stub_set_my_mac_attribute,
    .get_my_mac_attribute = stub_get_my_mac_attribute,
};


/* SAI APIS for IPSEC */

static sai_status_t stub_create_ipsec(
        _Out_ sai_object_id_t *ipsec_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC),ipsec_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ipsec(
        _In_ sai_object_id_t ipsec_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC),ipsec_id);
}

static sai_status_t stub_set_ipsec_attribute(
        _In_ sai_object_id_t ipsec_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC),ipsec_id, attr);
}

static sai_status_t stub_get_ipsec_attribute(
        _In_ sai_object_id_t ipsec_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC),ipsec_id, attr_count, attr_list);
}

static sai_status_t stub_create_ipsec_port(
        _Out_ sai_object_id_t *ipsec_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ipsec_port(
        _In_ sai_object_id_t ipsec_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id);
}

static sai_status_t stub_set_ipsec_port_attribute(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, attr);
}

static sai_status_t stub_get_ipsec_port_attribute(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, attr_count, attr_list);
}

static sai_status_t stub_get_ipsec_port_stats(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_ipsec_port_stats_ext(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_ipsec_port_stats(
        _In_ sai_object_id_t ipsec_port_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_PORT),ipsec_port_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_ipsec_sa(
        _Out_ sai_object_id_t *ipsec_sa_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ipsec_sa(
        _In_ sai_object_id_t ipsec_sa_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id);
}

static sai_status_t stub_set_ipsec_sa_attribute(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, attr);
}

static sai_status_t stub_get_ipsec_sa_attribute(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, attr_count, attr_list);
}

static sai_status_t stub_get_ipsec_sa_stats(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_ipsec_sa_stats_ext(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_ipsec_sa_stats(
        _In_ sai_object_id_t ipsec_sa_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_IPSEC_SA),ipsec_sa_id, number_of_counters, counter_ids);
}

static sai_ipsec_api_t stub_ipsec = {
    .create_ipsec = stub_create_ipsec,
    .remove_ipsec = stub_remove_ipsec,
    .set_ipsec_attribute = stub_set_ipsec_attribute,
    .get_ipsec_attribute = stub_get_ipsec_attribute,
    .create_ipsec_port = stub_create_ipsec_port,
    .remove_ipsec_port = stub_remove_ipsec_port,
    .set_ipsec_port_attribute = stub_set_ipsec_port_attribute,
    .get_ipsec_port_attribute = stub_get_ipsec_port_attribute,
    .get_ipsec_port_stats = stub_get_ipsec_port_stats,
    .get_ipsec_port_stats_ext = stub_get_ipsec_port_stats_ext,
    .clear_ipsec_port_stats = stub_clear_ipsec_port_stats,
    .create_ipsec_sa = stub_create_ipsec_sa,
    .remove_ipsec_sa = stub_remove_ipsec_sa,
    .set_ipsec_sa_attribute = stub_set_ipsec_sa_attribute,
    .get_ipsec_sa_attribute = stub_get_ipsec_sa_attribute,
    .get_ipsec_sa_stats = stub_get_ipsec_sa_stats,
    .get_ipsec_sa_stats_ext = stub_get_ipsec_sa_stats_ext,
    .clear_ipsec_sa_stats = stub_clear_ipsec_sa_stats,
};


/* SAI APIS for GENERIC_PROGRAMMABLE */

static sai_status_t stub_create_generic_programmable(
        _Out_ sai_object_id_t *generic_programmable_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_GENERIC_PROGRAMMABLE),generic_programmable_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_generic_programmable(
        _In_ sai_object_id_t generic_programmable_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_GENERIC_PROGRAMMABLE),generic_programmable_id);
}

static sai_status_t stub_set_generic_programmable_attribute(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_GENERIC_PROGRAMMABLE),generic_programmable_id, attr);
}

static sai_status_t stub_get_generic_programmable_attribute(
        _In_ sai_object_id_t generic_programmable_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_GENERIC_PROGRAMMABLE),generic_programmable_id, attr_count, attr_list);
}

static sai_generic_programmable_api_t stub_generic_programmable = {
    .create_generic_programmable = stub_create_generic_programmable,
    .remove_generic_programmable = stub_remove_generic_programmable,
    .set_generic_programmable_attribute = stub_set_generic_programmable_attribute,
    .get_generic_programmable_attribute = stub_get_generic_programmable_attribute,
};


/* SAI APIS for ARS */

static sai_status_t stub_create_ars(
        _Out_ sai_object_id_t *ars_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ARS),ars_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ars(
        _In_ sai_object_id_t ars_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ARS),ars_id);
}

static sai_status_t stub_set_ars_attribute(
        _In_ sai_object_id_t ars_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ARS),ars_id, attr);
}

static sai_status_t stub_get_ars_attribute(
        _In_ sai_object_id_t ars_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ARS),ars_id, attr_count, attr_list);
}

static sai_ars_api_t stub_ars = {
    .create_ars = stub_create_ars,
    .remove_ars = stub_remove_ars,
    .set_ars_attribute = stub_set_ars_attribute,
    .get_ars_attribute = stub_get_ars_attribute,
};


/* SAI APIS for ARS_PROFILE */

static sai_status_t stub_create_ars_profile(
        _Out_ sai_object_id_t *ars_profile_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ARS_PROFILE),ars_profile_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ars_profile(
        _In_ sai_object_id_t ars_profile_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ARS_PROFILE),ars_profile_id);
}

static sai_status_t stub_set_ars_profile_attribute(
        _In_ sai_object_id_t ars_profile_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ARS_PROFILE),ars_profile_id, attr);
}

static sai_status_t stub_get_ars_profile_attribute(
        _In_ sai_object_id_t ars_profile_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ARS_PROFILE),ars_profile_id, attr_count, attr_list);
}

static sai_ars_profile_api_t stub_ars_profile = {
    .create_ars_profile = stub_create_ars_profile,
    .remove_ars_profile = stub_remove_ars_profile,
    .set_ars_profile_attribute = stub_set_ars_profile_attribute,
    .get_ars_profile_attribute = stub_get_ars_profile_attribute,
};


/* SAI APIS for TWAMP */

static sai_status_t stub_create_twamp_session(
        _Out_ sai_object_id_t *twamp_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_twamp_session(
        _In_ sai_object_id_t twamp_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id);
}

static sai_status_t stub_set_twamp_session_attribute(
        _In_ sai_object_id_t twamp_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, attr);
}

static sai_status_t stub_get_twamp_session_attribute(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, attr_count, attr_list);
}

static sai_status_t stub_get_twamp_session_stats(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_twamp_session_stats_ext(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_twamp_session_stats(
        _In_ sai_object_id_t twamp_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_TWAMP_SESSION),twamp_session_id, number_of_counters, counter_ids);
}

static sai_twamp_api_t stub_twamp = {
    .create_twamp_session = stub_create_twamp_session,
    .remove_twamp_session = stub_remove_twamp_session,
    .set_twamp_session_attribute = stub_set_twamp_session_attribute,
    .get_twamp_session_attribute = stub_get_twamp_session_attribute,
    .get_twamp_session_stats = stub_get_twamp_session_stats,
    .get_twamp_session_stats_ext = stub_get_twamp_session_stats_ext,
    .clear_twamp_session_stats = stub_clear_twamp_session_stats,
};


/* SAI APIS for POE */

static sai_status_t stub_create_poe_device(
        _Out_ sai_object_id_t *poe_device_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_POE_DEVICE),poe_device_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_poe_device(
        _In_ sai_object_id_t poe_device_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_POE_DEVICE),poe_device_id);
}

static sai_status_t stub_set_poe_device_attribute(
        _In_ sai_object_id_t poe_device_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_POE_DEVICE),poe_device_id, attr);
}

static sai_status_t stub_get_poe_device_attribute(
        _In_ sai_object_id_t poe_device_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_POE_DEVICE),poe_device_id, attr_count, attr_list);
}

static sai_status_t stub_create_poe_pse(
        _Out_ sai_object_id_t *poe_pse_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PSE),poe_pse_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_poe_pse(
        _In_ sai_object_id_t poe_pse_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PSE),poe_pse_id);
}

static sai_status_t stub_set_poe_pse_attribute(
        _In_ sai_object_id_t poe_pse_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PSE),poe_pse_id, attr);
}

static sai_status_t stub_get_poe_pse_attribute(
        _In_ sai_object_id_t poe_pse_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PSE),poe_pse_id, attr_count, attr_list);
}

static sai_status_t stub_create_poe_port(
        _Out_ sai_object_id_t *poe_port_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PORT),poe_port_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_poe_port(
        _In_ sai_object_id_t poe_port_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PORT),poe_port_id);
}

static sai_status_t stub_set_poe_port_attribute(
        _In_ sai_object_id_t poe_port_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PORT),poe_port_id, attr);
}

static sai_status_t stub_get_poe_port_attribute(
        _In_ sai_object_id_t poe_port_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_POE_PORT),poe_port_id, attr_count, attr_list);
}

static sai_poe_api_t stub_poe = {
    .create_poe_device = stub_create_poe_device,
    .remove_poe_device = stub_remove_poe_device,
    .set_poe_device_attribute = stub_set_poe_device_attribute,
    .get_poe_device_attribute = stub_get_poe_device_attribute,
    .create_poe_pse = stub_create_poe_pse,
    .remove_poe_pse = stub_remove_poe_pse,
    .set_poe_pse_attribute = stub_set_poe_pse_attribute,
    .get_poe_pse_attribute = stub_get_poe_pse_attribute,
    .create_poe_port = stub_create_poe_port,
    .remove_poe_port = stub_remove_poe_port,
    .set_poe_port_attribute = stub_set_poe_port_attribute,
    .get_poe_port_attribute = stub_get_poe_port_attribute,
};


/* SAI APIS for ICMP_ECHO */

static sai_status_t stub_create_icmp_echo_session(
        _Out_ sai_object_id_t *icmp_echo_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_icmp_echo_session(
        _In_ sai_object_id_t icmp_echo_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id);
}

static sai_status_t stub_set_icmp_echo_session_attribute(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, attr);
}

static sai_status_t stub_get_icmp_echo_session_attribute(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, attr_count, attr_list);
}

static sai_status_t stub_get_icmp_echo_session_stats(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_icmp_echo_session_stats_ext(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_icmp_echo_session_stats(
        _In_ sai_object_id_t icmp_echo_session_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_ICMP_ECHO_SESSION),icmp_echo_session_id, number_of_counters, counter_ids);
}

static sai_icmp_echo_api_t stub_icmp_echo = {
    .create_icmp_echo_session = stub_create_icmp_echo_session,
    .remove_icmp_echo_session = stub_remove_icmp_echo_session,
    .set_icmp_echo_session_attribute = stub_set_icmp_echo_session_attribute,
    .get_icmp_echo_session_attribute = stub_get_icmp_echo_session_attribute,
    .get_icmp_echo_session_stats = stub_get_icmp_echo_session_stats,
    .get_icmp_echo_session_stats_ext = stub_get_icmp_echo_session_stats_ext,
    .clear_icmp_echo_session_stats = stub_clear_icmp_echo_session_stats,
};


/* SAI APIS for PREFIX_COMPRESSION */

static sai_status_t stub_create_prefix_compression_table(
        _Out_ sai_object_id_t *prefix_compression_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE),prefix_compression_table_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_prefix_compression_table(
        _In_ sai_object_id_t prefix_compression_table_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE),prefix_compression_table_id);
}

static sai_status_t stub_set_prefix_compression_table_attribute(
        _In_ sai_object_id_t prefix_compression_table_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE),prefix_compression_table_id, attr);
}

static sai_status_t stub_get_prefix_compression_table_attribute(
        _In_ sai_object_id_t prefix_compression_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_PREFIX_COMPRESSION_TABLE),prefix_compression_table_id, attr_count, attr_list);
}

static sai_status_t stub_create_prefix_compression_entry(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(prefix_compression_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_prefix_compression_entry(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(prefix_compression_entry);
}

static sai_status_t stub_set_prefix_compression_entry_attribute(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(prefix_compression_entry, attr);
}

static sai_status_t stub_get_prefix_compression_entry_attribute(
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(prefix_compression_entry, attr_count, attr_list);
}

static sai_status_t stub_create_prefix_compression_entries(
        _In_ uint32_t object_count,
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, prefix_compression_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_prefix_compression_entries(
        _In_ uint32_t object_count,
        _In_ const sai_prefix_compression_entry_t *prefix_compression_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, prefix_compression_entry, mode, object_statuses);
}

static sai_prefix_compression_api_t stub_prefix_compression = {
    .create_prefix_compression_table = stub_create_prefix_compression_table,
    .remove_prefix_compression_table = stub_remove_prefix_compression_table,
    .set_prefix_compression_table_attribute = stub_set_prefix_compression_table_attribute,
    .get_prefix_compression_table_attribute = stub_get_prefix_compression_table_attribute,
    .create_prefix_compression_entry = stub_create_prefix_compression_entry,
    .remove_prefix_compression_entry = stub_remove_prefix_compression_entry,
    .set_prefix_compression_entry_attribute = stub_set_prefix_compression_entry_attribute,
    .get_prefix_compression_entry_attribute = stub_get_prefix_compression_entry_attribute,
    .create_prefix_compression_entries = stub_create_prefix_compression_entries,
    .remove_prefix_compression_entries = stub_remove_prefix_compression_entries,
};


/* SAI APIS for SYNCE */

static sai_status_t stub_create_synce_clock(
        _Out_ sai_object_id_t *synce_clock_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_SYNCE_CLOCK),synce_clock_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_synce_clock(
        _In_ sai_object_id_t synce_clock_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_SYNCE_CLOCK),synce_clock_id);
}

static sai_status_t stub_set_synce_clock_attribute(
        _In_ sai_object_id_t synce_clock_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_SYNCE_CLOCK),synce_clock_id, attr);
}

static sai_status_t stub_get_synce_clock_attribute(
        _In_ sai_object_id_t synce_clock_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_SYNCE_CLOCK),synce_clock_id, attr_count, attr_list);
}

static sai_synce_api_t stub_synce = {
    .create_synce_clock = stub_create_synce_clock,
    .remove_synce_clock = stub_remove_synce_clock,
    .set_synce_clock_attribute = stub_set_synce_clock_attribute,
    .get_synce_clock_attribute = stub_get_synce_clock_attribute,
};


/* SAI APIS for BMTOR */

static sai_status_t stub_create_table_bitmap_classification_entry(
        _Out_ sai_object_id_t *table_bitmap_classification_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_table_bitmap_classification_entry(
        _In_ sai_object_id_t table_bitmap_classification_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id);
}

static sai_status_t stub_set_table_bitmap_classification_entry_attribute(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, attr);
}

static sai_status_t stub_get_table_bitmap_classification_entry_attribute(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, attr_count, attr_list);
}

static sai_status_t stub_get_table_bitmap_classification_entry_stats(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_table_bitmap_classification_entry_stats_ext(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_table_bitmap_classification_entry_stats(
        _In_ sai_object_id_t table_bitmap_classification_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_CLASSIFICATION_ENTRY),table_bitmap_classification_entry_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_table_bitmap_router_entry(
        _Out_ sai_object_id_t *table_bitmap_router_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_table_bitmap_router_entry(
        _In_ sai_object_id_t table_bitmap_router_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id);
}

static sai_status_t stub_set_table_bitmap_router_entry_attribute(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, attr);
}

static sai_status_t stub_get_table_bitmap_router_entry_attribute(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, attr_count, attr_list);
}

static sai_status_t stub_get_table_bitmap_router_entry_stats(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_table_bitmap_router_entry_stats_ext(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_table_bitmap_router_entry_stats(
        _In_ sai_object_id_t table_bitmap_router_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_BITMAP_ROUTER_ENTRY),table_bitmap_router_entry_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_table_meta_tunnel_entry(
        _Out_ sai_object_id_t *table_meta_tunnel_entry_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_table_meta_tunnel_entry(
        _In_ sai_object_id_t table_meta_tunnel_entry_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id);
}

static sai_status_t stub_set_table_meta_tunnel_entry_attribute(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, attr);
}

static sai_status_t stub_get_table_meta_tunnel_entry_attribute(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, attr_count, attr_list);
}

static sai_status_t stub_get_table_meta_tunnel_entry_stats(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_table_meta_tunnel_entry_stats_ext(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_table_meta_tunnel_entry_stats(
        _In_ sai_object_id_t table_meta_tunnel_entry_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_TABLE_META_TUNNEL_ENTRY),table_meta_tunnel_entry_id, number_of_counters, counter_ids);
}

static sai_bmtor_api_t stub_bmtor = {
    .create_table_bitmap_classification_entry = stub_create_table_bitmap_classification_entry,
    .remove_table_bitmap_classification_entry = stub_remove_table_bitmap_classification_entry,
    .set_table_bitmap_classification_entry_attribute = stub_set_table_bitmap_classification_entry_attribute,
    .get_table_bitmap_classification_entry_attribute = stub_get_table_bitmap_classification_entry_attribute,
    .get_table_bitmap_classification_entry_stats = stub_get_table_bitmap_classification_entry_stats,
    .get_table_bitmap_classification_entry_stats_ext = stub_get_table_bitmap_classification_entry_stats_ext,
    .clear_table_bitmap_classification_entry_stats = stub_clear_table_bitmap_classification_entry_stats,
    .create_table_bitmap_router_entry = stub_create_table_bitmap_router_entry,
    .remove_table_bitmap_router_entry = stub_remove_table_bitmap_router_entry,
    .set_table_bitmap_router_entry_attribute = stub_set_table_bitmap_router_entry_attribute,
    .get_table_bitmap_router_entry_attribute = stub_get_table_bitmap_router_entry_attribute,
    .get_table_bitmap_router_entry_stats = stub_get_table_bitmap_router_entry_stats,
    .get_table_bitmap_router_entry_stats_ext = stub_get_table_bitmap_router_entry_stats_ext,
    .clear_table_bitmap_router_entry_stats = stub_clear_table_bitmap_router_entry_stats,
    .create_table_meta_tunnel_entry = stub_create_table_meta_tunnel_entry,
    .remove_table_meta_tunnel_entry = stub_remove_table_meta_tunnel_entry,
    .set_table_meta_tunnel_entry_attribute = stub_set_table_meta_tunnel_entry_attribute,
    .get_table_meta_tunnel_entry_attribute = stub_get_table_meta_tunnel_entry_attribute,
    .get_table_meta_tunnel_entry_stats = stub_get_table_meta_tunnel_entry_stats,
    .get_table_meta_tunnel_entry_stats_ext = stub_get_table_meta_tunnel_entry_stats_ext,
    .clear_table_meta_tunnel_entry_stats = stub_clear_table_meta_tunnel_entry_stats,
};


/* SAI APIS for DASH_ACL */

static sai_status_t stub_create_dash_acl_group(
        _Out_ sai_object_id_t *dash_acl_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),dash_acl_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_acl_group(
        _In_ sai_object_id_t dash_acl_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),dash_acl_group_id);
}

static sai_status_t stub_set_dash_acl_group_attribute(
        _In_ sai_object_id_t dash_acl_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),dash_acl_group_id, attr);
}

static sai_status_t stub_get_dash_acl_group_attribute(
        _In_ sai_object_id_t dash_acl_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),dash_acl_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_acl_groups(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_acl_groups(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_GROUP),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_dash_acl_rule(
        _Out_ sai_object_id_t *dash_acl_rule_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),dash_acl_rule_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_acl_rule(
        _In_ sai_object_id_t dash_acl_rule_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),dash_acl_rule_id);
}

static sai_status_t stub_set_dash_acl_rule_attribute(
        _In_ sai_object_id_t dash_acl_rule_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),dash_acl_rule_id, attr);
}

static sai_status_t stub_get_dash_acl_rule_attribute(
        _In_ sai_object_id_t dash_acl_rule_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),dash_acl_rule_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_acl_rules(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_acl_rules(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_ACL_RULE),object_count, object_id, mode, object_statuses);
}

static sai_dash_acl_api_t stub_dash_acl = {
    .create_dash_acl_group = stub_create_dash_acl_group,
    .remove_dash_acl_group = stub_remove_dash_acl_group,
    .set_dash_acl_group_attribute = stub_set_dash_acl_group_attribute,
    .get_dash_acl_group_attribute = stub_get_dash_acl_group_attribute,
    .create_dash_acl_groups = stub_create_dash_acl_groups,
    .remove_dash_acl_groups = stub_remove_dash_acl_groups,
    .create_dash_acl_rule = stub_create_dash_acl_rule,
    .remove_dash_acl_rule = stub_remove_dash_acl_rule,
    .set_dash_acl_rule_attribute = stub_set_dash_acl_rule_attribute,
    .get_dash_acl_rule_attribute = stub_get_dash_acl_rule_attribute,
    .create_dash_acl_rules = stub_create_dash_acl_rules,
    .remove_dash_acl_rules = stub_remove_dash_acl_rules,
};


/* SAI APIS for DASH_DIRECTION_LOOKUP */

static sai_status_t stub_create_direction_lookup_entry(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(direction_lookup_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_direction_lookup_entry(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(direction_lookup_entry);
}

static sai_status_t stub_set_direction_lookup_entry_attribute(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(direction_lookup_entry, attr);
}

static sai_status_t stub_get_direction_lookup_entry_attribute(
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(direction_lookup_entry, attr_count, attr_list);
}

static sai_status_t stub_create_direction_lookup_entries(
        _In_ uint32_t object_count,
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, direction_lookup_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_direction_lookup_entries(
        _In_ uint32_t object_count,
        _In_ const sai_direction_lookup_entry_t *direction_lookup_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, direction_lookup_entry, mode, object_statuses);
}

static sai_dash_direction_lookup_api_t stub_dash_direction_lookup = {
    .create_direction_lookup_entry = stub_create_direction_lookup_entry,
    .remove_direction_lookup_entry = stub_remove_direction_lookup_entry,
    .set_direction_lookup_entry_attribute = stub_set_direction_lookup_entry_attribute,
    .get_direction_lookup_entry_attribute = stub_get_direction_lookup_entry_attribute,
    .create_direction_lookup_entries = stub_create_direction_lookup_entries,
    .remove_direction_lookup_entries = stub_remove_direction_lookup_entries,
};


/* SAI APIS for DASH_ENI */

static sai_status_t stub_create_eni_ether_address_map_entry(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(eni_ether_address_map_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_eni_ether_address_map_entry(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(eni_ether_address_map_entry);
}

static sai_status_t stub_set_eni_ether_address_map_entry_attribute(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(eni_ether_address_map_entry, attr);
}

static sai_status_t stub_get_eni_ether_address_map_entry_attribute(
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(eni_ether_address_map_entry, attr_count, attr_list);
}

static sai_status_t stub_create_eni_ether_address_map_entries(
        _In_ uint32_t object_count,
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, eni_ether_address_map_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_eni_ether_address_map_entries(
        _In_ uint32_t object_count,
        _In_ const sai_eni_ether_address_map_entry_t *eni_ether_address_map_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, eni_ether_address_map_entry, mode, object_statuses);
}

static sai_status_t stub_create_eni(
        _Out_ sai_object_id_t *eni_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_eni(
        _In_ sai_object_id_t eni_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id);
}

static sai_status_t stub_set_eni_attribute(
        _In_ sai_object_id_t eni_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, attr);
}

static sai_status_t stub_get_eni_attribute(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, attr_count, attr_list);
}

static sai_status_t stub_get_eni_stats(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_eni_stats_ext(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_eni_stats(
        _In_ sai_object_id_t eni_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),eni_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_enis(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_enis(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_ENI),object_count, object_id, mode, object_statuses);
}

static sai_dash_eni_api_t stub_dash_eni = {
    .create_eni_ether_address_map_entry = stub_create_eni_ether_address_map_entry,
    .remove_eni_ether_address_map_entry = stub_remove_eni_ether_address_map_entry,
    .set_eni_ether_address_map_entry_attribute = stub_set_eni_ether_address_map_entry_attribute,
    .get_eni_ether_address_map_entry_attribute = stub_get_eni_ether_address_map_entry_attribute,
    .create_eni_ether_address_map_entries = stub_create_eni_ether_address_map_entries,
    .remove_eni_ether_address_map_entries = stub_remove_eni_ether_address_map_entries,
    .create_eni = stub_create_eni,
    .remove_eni = stub_remove_eni,
    .set_eni_attribute = stub_set_eni_attribute,
    .get_eni_attribute = stub_get_eni_attribute,
    .get_eni_stats = stub_get_eni_stats,
    .get_eni_stats_ext = stub_get_eni_stats_ext,
    .clear_eni_stats = stub_clear_eni_stats,
    .create_enis = stub_create_enis,
    .remove_enis = stub_remove_enis,
};


/* SAI APIS for DASH_INBOUND_ROUTING */

static sai_status_t stub_create_inbound_routing_entry(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(inbound_routing_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_inbound_routing_entry(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(inbound_routing_entry);
}

static sai_status_t stub_set_inbound_routing_entry_attribute(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(inbound_routing_entry, attr);
}

static sai_status_t stub_get_inbound_routing_entry_attribute(
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(inbound_routing_entry, attr_count, attr_list);
}

static sai_status_t stub_create_inbound_routing_entries(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, inbound_routing_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_inbound_routing_entries(
        _In_ uint32_t object_count,
        _In_ const sai_inbound_routing_entry_t *inbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, inbound_routing_entry, mode, object_statuses);
}

static sai_dash_inbound_routing_api_t stub_dash_inbound_routing = {
    .create_inbound_routing_entry = stub_create_inbound_routing_entry,
    .remove_inbound_routing_entry = stub_remove_inbound_routing_entry,
    .set_inbound_routing_entry_attribute = stub_set_inbound_routing_entry_attribute,
    .get_inbound_routing_entry_attribute = stub_get_inbound_routing_entry_attribute,
    .create_inbound_routing_entries = stub_create_inbound_routing_entries,
    .remove_inbound_routing_entries = stub_remove_inbound_routing_entries,
};


/* SAI APIS for DASH_METER */

static sai_status_t stub_create_meter_bucket_entry(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(meter_bucket_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_meter_bucket_entry(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(meter_bucket_entry);
}

static sai_status_t stub_set_meter_bucket_entry_attribute(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(meter_bucket_entry, attr);
}

static sai_status_t stub_get_meter_bucket_entry_attribute(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(meter_bucket_entry, attr_count, attr_list);
}

static sai_status_t stub_get_meter_bucket_entry_stats(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats(meter_bucket_entry, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_meter_bucket_entry_stats_ext(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt(meter_bucket_entry, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_meter_bucket_entry_stats(
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats(meter_bucket_entry, number_of_counters, counter_ids);
}

static sai_status_t stub_create_meter_bucket_entries(
        _In_ uint32_t object_count,
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, meter_bucket_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_meter_bucket_entries(
        _In_ uint32_t object_count,
        _In_ const sai_meter_bucket_entry_t *meter_bucket_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, meter_bucket_entry, mode, object_statuses);
}

static sai_status_t stub_create_meter_policy(
        _Out_ sai_object_id_t *meter_policy_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),meter_policy_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_meter_policy(
        _In_ sai_object_id_t meter_policy_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),meter_policy_id);
}

static sai_status_t stub_set_meter_policy_attribute(
        _In_ sai_object_id_t meter_policy_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),meter_policy_id, attr);
}

static sai_status_t stub_get_meter_policy_attribute(
        _In_ sai_object_id_t meter_policy_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),meter_policy_id, attr_count, attr_list);
}

static sai_status_t stub_create_meter_policys(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_meter_policys(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_METER_POLICY),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_meter_rule(
        _Out_ sai_object_id_t *meter_rule_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),meter_rule_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_meter_rule(
        _In_ sai_object_id_t meter_rule_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),meter_rule_id);
}

static sai_status_t stub_set_meter_rule_attribute(
        _In_ sai_object_id_t meter_rule_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),meter_rule_id, attr);
}

static sai_status_t stub_get_meter_rule_attribute(
        _In_ sai_object_id_t meter_rule_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),meter_rule_id, attr_count, attr_list);
}

static sai_status_t stub_create_meter_rules(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_meter_rules(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_METER_RULE),object_count, object_id, mode, object_statuses);
}

static sai_dash_meter_api_t stub_dash_meter = {
    .create_meter_bucket_entry = stub_create_meter_bucket_entry,
    .remove_meter_bucket_entry = stub_remove_meter_bucket_entry,
    .set_meter_bucket_entry_attribute = stub_set_meter_bucket_entry_attribute,
    .get_meter_bucket_entry_attribute = stub_get_meter_bucket_entry_attribute,
    .get_meter_bucket_entry_stats = stub_get_meter_bucket_entry_stats,
    .get_meter_bucket_entry_stats_ext = stub_get_meter_bucket_entry_stats_ext,
    .clear_meter_bucket_entry_stats = stub_clear_meter_bucket_entry_stats,
    .create_meter_bucket_entries = stub_create_meter_bucket_entries,
    .remove_meter_bucket_entries = stub_remove_meter_bucket_entries,
    .create_meter_policy = stub_create_meter_policy,
    .remove_meter_policy = stub_remove_meter_policy,
    .set_meter_policy_attribute = stub_set_meter_policy_attribute,
    .get_meter_policy_attribute = stub_get_meter_policy_attribute,
    .create_meter_policys = stub_create_meter_policys,
    .remove_meter_policys = stub_remove_meter_policys,
    .create_meter_rule = stub_create_meter_rule,
    .remove_meter_rule = stub_remove_meter_rule,
    .set_meter_rule_attribute = stub_set_meter_rule_attribute,
    .get_meter_rule_attribute = stub_get_meter_rule_attribute,
    .create_meter_rules = stub_create_meter_rules,
    .remove_meter_rules = stub_remove_meter_rules,
};


/* SAI APIS for DASH_OUTBOUND_CA_TO_PA */

static sai_status_t stub_create_outbound_ca_to_pa_entry(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(outbound_ca_to_pa_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_outbound_ca_to_pa_entry(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(outbound_ca_to_pa_entry);
}

static sai_status_t stub_set_outbound_ca_to_pa_entry_attribute(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(outbound_ca_to_pa_entry, attr);
}

static sai_status_t stub_get_outbound_ca_to_pa_entry_attribute(
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(outbound_ca_to_pa_entry, attr_count, attr_list);
}

static sai_status_t stub_create_outbound_ca_to_pa_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, outbound_ca_to_pa_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_outbound_ca_to_pa_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_ca_to_pa_entry_t *outbound_ca_to_pa_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, outbound_ca_to_pa_entry, mode, object_statuses);
}

static sai_dash_outbound_ca_to_pa_api_t stub_dash_outbound_ca_to_pa = {
    .create_outbound_ca_to_pa_entry = stub_create_outbound_ca_to_pa_entry,
    .remove_outbound_ca_to_pa_entry = stub_remove_outbound_ca_to_pa_entry,
    .set_outbound_ca_to_pa_entry_attribute = stub_set_outbound_ca_to_pa_entry_attribute,
    .get_outbound_ca_to_pa_entry_attribute = stub_get_outbound_ca_to_pa_entry_attribute,
    .create_outbound_ca_to_pa_entries = stub_create_outbound_ca_to_pa_entries,
    .remove_outbound_ca_to_pa_entries = stub_remove_outbound_ca_to_pa_entries,
};


/* SAI APIS for DASH_OUTBOUND_ROUTING */

static sai_status_t stub_create_outbound_routing_entry(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(outbound_routing_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_outbound_routing_entry(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(outbound_routing_entry);
}

static sai_status_t stub_set_outbound_routing_entry_attribute(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(outbound_routing_entry, attr);
}

static sai_status_t stub_get_outbound_routing_entry_attribute(
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(outbound_routing_entry, attr_count, attr_list);
}

static sai_status_t stub_create_outbound_routing_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, outbound_routing_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_outbound_routing_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_routing_entry_t *outbound_routing_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, outbound_routing_entry, mode, object_statuses);
}

static sai_status_t stub_create_outbound_routing_group(
        _Out_ sai_object_id_t *outbound_routing_group_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),outbound_routing_group_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_outbound_routing_group(
        _In_ sai_object_id_t outbound_routing_group_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),outbound_routing_group_id);
}

static sai_status_t stub_set_outbound_routing_group_attribute(
        _In_ sai_object_id_t outbound_routing_group_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),outbound_routing_group_id, attr);
}

static sai_status_t stub_get_outbound_routing_group_attribute(
        _In_ sai_object_id_t outbound_routing_group_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),outbound_routing_group_id, attr_count, attr_list);
}

static sai_status_t stub_create_outbound_routing_groups(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_outbound_routing_groups(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_ROUTING_GROUP),object_count, object_id, mode, object_statuses);
}

static sai_dash_outbound_routing_api_t stub_dash_outbound_routing = {
    .create_outbound_routing_entry = stub_create_outbound_routing_entry,
    .remove_outbound_routing_entry = stub_remove_outbound_routing_entry,
    .set_outbound_routing_entry_attribute = stub_set_outbound_routing_entry_attribute,
    .get_outbound_routing_entry_attribute = stub_get_outbound_routing_entry_attribute,
    .create_outbound_routing_entries = stub_create_outbound_routing_entries,
    .remove_outbound_routing_entries = stub_remove_outbound_routing_entries,
    .create_outbound_routing_group = stub_create_outbound_routing_group,
    .remove_outbound_routing_group = stub_remove_outbound_routing_group,
    .set_outbound_routing_group_attribute = stub_set_outbound_routing_group_attribute,
    .get_outbound_routing_group_attribute = stub_get_outbound_routing_group_attribute,
    .create_outbound_routing_groups = stub_create_outbound_routing_groups,
    .remove_outbound_routing_groups = stub_remove_outbound_routing_groups,
};


/* SAI APIS for DASH_VNET */

static sai_status_t stub_create_vnet(
        _Out_ sai_object_id_t *vnet_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),vnet_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_vnet(
        _In_ sai_object_id_t vnet_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),vnet_id);
}

static sai_status_t stub_set_vnet_attribute(
        _In_ sai_object_id_t vnet_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),vnet_id, attr);
}

static sai_status_t stub_get_vnet_attribute(
        _In_ sai_object_id_t vnet_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),vnet_id, attr_count, attr_list);
}

static sai_status_t stub_create_vnets(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_vnets(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_VNET),object_count, object_id, mode, object_statuses);
}

static sai_dash_vnet_api_t stub_dash_vnet = {
    .create_vnet = stub_create_vnet,
    .remove_vnet = stub_remove_vnet,
    .set_vnet_attribute = stub_set_vnet_attribute,
    .get_vnet_attribute = stub_get_vnet_attribute,
    .create_vnets = stub_create_vnets,
    .remove_vnets = stub_remove_vnets,
};


/* SAI APIS for DASH_PA_VALIDATION */

static sai_status_t stub_create_pa_validation_entry(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(pa_validation_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_pa_validation_entry(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(pa_validation_entry);
}

static sai_status_t stub_set_pa_validation_entry_attribute(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(pa_validation_entry, attr);
}

static sai_status_t stub_get_pa_validation_entry_attribute(
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(pa_validation_entry, attr_count, attr_list);
}

static sai_status_t stub_create_pa_validation_entries(
        _In_ uint32_t object_count,
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, pa_validation_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_pa_validation_entries(
        _In_ uint32_t object_count,
        _In_ const sai_pa_validation_entry_t *pa_validation_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, pa_validation_entry, mode, object_statuses);
}

static sai_dash_pa_validation_api_t stub_dash_pa_validation = {
    .create_pa_validation_entry = stub_create_pa_validation_entry,
    .remove_pa_validation_entry = stub_remove_pa_validation_entry,
    .set_pa_validation_entry_attribute = stub_set_pa_validation_entry_attribute,
    .get_pa_validation_entry_attribute = stub_get_pa_validation_entry_attribute,
    .create_pa_validation_entries = stub_create_pa_validation_entries,
    .remove_pa_validation_entries = stub_remove_pa_validation_entries,
};


/* SAI APIS for DASH_VIP */

static sai_status_t stub_create_vip_entry(
        _In_ const sai_vip_entry_t *vip_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(vip_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_vip_entry(
        _In_ const sai_vip_entry_t *vip_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(vip_entry);
}

static sai_status_t stub_set_vip_entry_attribute(
        _In_ const sai_vip_entry_t *vip_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(vip_entry, attr);
}

static sai_status_t stub_get_vip_entry_attribute(
        _In_ const sai_vip_entry_t *vip_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(vip_entry, attr_count, attr_list);
}

static sai_status_t stub_create_vip_entries(
        _In_ uint32_t object_count,
        _In_ const sai_vip_entry_t *vip_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, vip_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_vip_entries(
        _In_ uint32_t object_count,
        _In_ const sai_vip_entry_t *vip_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, vip_entry, mode, object_statuses);
}

static sai_dash_vip_api_t stub_dash_vip = {
    .create_vip_entry = stub_create_vip_entry,
    .remove_vip_entry = stub_remove_vip_entry,
    .set_vip_entry_attribute = stub_set_vip_entry_attribute,
    .get_vip_entry_attribute = stub_get_vip_entry_attribute,
    .create_vip_entries = stub_create_vip_entries,
    .remove_vip_entries = stub_remove_vip_entries,
};


/* SAI APIS for DASH_HA */

static sai_status_t stub_create_ha_set(
        _Out_ sai_object_id_t *ha_set_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ha_set(
        _In_ sai_object_id_t ha_set_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id);
}

static sai_status_t stub_set_ha_set_attribute(
        _In_ sai_object_id_t ha_set_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, attr);
}

static sai_status_t stub_get_ha_set_attribute(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, attr_count, attr_list);
}

static sai_status_t stub_get_ha_set_stats(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStats((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, number_of_counters, counter_ids, counters);
}

static sai_status_t stub_get_ha_set_stats_ext(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    return stub->getStatsExt((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, number_of_counters, counter_ids, mode, counters);
}

static sai_status_t stub_clear_ha_set_stats(
        _In_ sai_object_id_t ha_set_id,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids)
{
    SWSS_LOG_ENTER();

    return stub->clearStats((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),ha_set_id, number_of_counters, counter_ids);
}

static sai_status_t stub_create_ha_sets(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_ha_sets(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SET),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_ha_scope(
        _Out_ sai_object_id_t *ha_scope_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),ha_scope_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_ha_scope(
        _In_ sai_object_id_t ha_scope_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),ha_scope_id);
}

static sai_status_t stub_set_ha_scope_attribute(
        _In_ sai_object_id_t ha_scope_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),ha_scope_id, attr);
}

static sai_status_t stub_get_ha_scope_attribute(
        _In_ sai_object_id_t ha_scope_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),ha_scope_id, attr_count, attr_list);
}

static sai_status_t stub_create_ha_scopes(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_ha_scopes(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_HA_SCOPE),object_count, object_id, mode, object_statuses);
}

static sai_dash_ha_api_t stub_dash_ha = {
    .create_ha_set = stub_create_ha_set,
    .remove_ha_set = stub_remove_ha_set,
    .set_ha_set_attribute = stub_set_ha_set_attribute,
    .get_ha_set_attribute = stub_get_ha_set_attribute,
    .get_ha_set_stats = stub_get_ha_set_stats,
    .get_ha_set_stats_ext = stub_get_ha_set_stats_ext,
    .clear_ha_set_stats = stub_clear_ha_set_stats,
    .create_ha_sets = stub_create_ha_sets,
    .remove_ha_sets = stub_remove_ha_sets,
    .create_ha_scope = stub_create_ha_scope,
    .remove_ha_scope = stub_remove_ha_scope,
    .set_ha_scope_attribute = stub_set_ha_scope_attribute,
    .get_ha_scope_attribute = stub_get_ha_scope_attribute,
    .create_ha_scopes = stub_create_ha_scopes,
    .remove_ha_scopes = stub_remove_ha_scopes,
};


/* SAI APIS for DASH_TUNNEL */

static sai_status_t stub_create_dash_tunnel(
        _Out_ sai_object_id_t *dash_tunnel_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),dash_tunnel_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_tunnel(
        _In_ sai_object_id_t dash_tunnel_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),dash_tunnel_id);
}

static sai_status_t stub_set_dash_tunnel_attribute(
        _In_ sai_object_id_t dash_tunnel_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),dash_tunnel_id, attr);
}

static sai_status_t stub_get_dash_tunnel_attribute(
        _In_ sai_object_id_t dash_tunnel_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),dash_tunnel_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_tunnels(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_tunnels(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_dash_tunnel_member(
        _Out_ sai_object_id_t *dash_tunnel_member_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),dash_tunnel_member_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_tunnel_member(
        _In_ sai_object_id_t dash_tunnel_member_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),dash_tunnel_member_id);
}

static sai_status_t stub_set_dash_tunnel_member_attribute(
        _In_ sai_object_id_t dash_tunnel_member_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),dash_tunnel_member_id, attr);
}

static sai_status_t stub_get_dash_tunnel_member_attribute(
        _In_ sai_object_id_t dash_tunnel_member_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),dash_tunnel_member_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_tunnel_members(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_tunnel_members(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_MEMBER),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_dash_tunnel_next_hop(
        _Out_ sai_object_id_t *dash_tunnel_next_hop_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),dash_tunnel_next_hop_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_tunnel_next_hop(
        _In_ sai_object_id_t dash_tunnel_next_hop_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),dash_tunnel_next_hop_id);
}

static sai_status_t stub_set_dash_tunnel_next_hop_attribute(
        _In_ sai_object_id_t dash_tunnel_next_hop_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),dash_tunnel_next_hop_id, attr);
}

static sai_status_t stub_get_dash_tunnel_next_hop_attribute(
        _In_ sai_object_id_t dash_tunnel_next_hop_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),dash_tunnel_next_hop_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_tunnel_next_hops(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_tunnel_next_hops(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_TUNNEL_NEXT_HOP),object_count, object_id, mode, object_statuses);
}

static sai_dash_tunnel_api_t stub_dash_tunnel = {
    .create_dash_tunnel = stub_create_dash_tunnel,
    .remove_dash_tunnel = stub_remove_dash_tunnel,
    .set_dash_tunnel_attribute = stub_set_dash_tunnel_attribute,
    .get_dash_tunnel_attribute = stub_get_dash_tunnel_attribute,
    .create_dash_tunnels = stub_create_dash_tunnels,
    .remove_dash_tunnels = stub_remove_dash_tunnels,
    .create_dash_tunnel_member = stub_create_dash_tunnel_member,
    .remove_dash_tunnel_member = stub_remove_dash_tunnel_member,
    .set_dash_tunnel_member_attribute = stub_set_dash_tunnel_member_attribute,
    .get_dash_tunnel_member_attribute = stub_get_dash_tunnel_member_attribute,
    .create_dash_tunnel_members = stub_create_dash_tunnel_members,
    .remove_dash_tunnel_members = stub_remove_dash_tunnel_members,
    .create_dash_tunnel_next_hop = stub_create_dash_tunnel_next_hop,
    .remove_dash_tunnel_next_hop = stub_remove_dash_tunnel_next_hop,
    .set_dash_tunnel_next_hop_attribute = stub_set_dash_tunnel_next_hop_attribute,
    .get_dash_tunnel_next_hop_attribute = stub_get_dash_tunnel_next_hop_attribute,
    .create_dash_tunnel_next_hops = stub_create_dash_tunnel_next_hops,
    .remove_dash_tunnel_next_hops = stub_remove_dash_tunnel_next_hops,
};


/* SAI APIS for DASH_FLOW */

static sai_status_t stub_create_flow_table(
        _Out_ sai_object_id_t *flow_table_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),flow_table_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_flow_table(
        _In_ sai_object_id_t flow_table_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),flow_table_id);
}

static sai_status_t stub_set_flow_table_attribute(
        _In_ sai_object_id_t flow_table_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),flow_table_id, attr);
}

static sai_status_t stub_get_flow_table_attribute(
        _In_ sai_object_id_t flow_table_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),flow_table_id, attr_count, attr_list);
}

static sai_status_t stub_create_flow_tables(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_flow_tables(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_TABLE),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_flow_entry(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(flow_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_flow_entry(
        _In_ const sai_flow_entry_t *flow_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(flow_entry);
}

static sai_status_t stub_set_flow_entry_attribute(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(flow_entry, attr);
}

static sai_status_t stub_get_flow_entry_attribute(
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(flow_entry, attr_count, attr_list);
}

static sai_status_t stub_create_flow_entries(
        _In_ uint32_t object_count,
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, flow_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_flow_entries(
        _In_ uint32_t object_count,
        _In_ const sai_flow_entry_t *flow_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, flow_entry, mode, object_statuses);
}

static sai_status_t stub_create_flow_entry_bulk_get_session_filter(
        _Out_ sai_object_id_t *flow_entry_bulk_get_session_filter_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),flow_entry_bulk_get_session_filter_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_flow_entry_bulk_get_session_filter(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),flow_entry_bulk_get_session_filter_id);
}

static sai_status_t stub_set_flow_entry_bulk_get_session_filter_attribute(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),flow_entry_bulk_get_session_filter_id, attr);
}

static sai_status_t stub_get_flow_entry_bulk_get_session_filter_attribute(
        _In_ sai_object_id_t flow_entry_bulk_get_session_filter_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),flow_entry_bulk_get_session_filter_id, attr_count, attr_list);
}

static sai_status_t stub_create_flow_entry_bulk_get_session_filters(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_flow_entry_bulk_get_session_filters(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION_FILTER),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_flow_entry_bulk_get_session(
        _Out_ sai_object_id_t *flow_entry_bulk_get_session_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),flow_entry_bulk_get_session_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_flow_entry_bulk_get_session(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),flow_entry_bulk_get_session_id);
}

static sai_status_t stub_set_flow_entry_bulk_get_session_attribute(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),flow_entry_bulk_get_session_id, attr);
}

static sai_status_t stub_get_flow_entry_bulk_get_session_attribute(
        _In_ sai_object_id_t flow_entry_bulk_get_session_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),flow_entry_bulk_get_session_id, attr_count, attr_list);
}

static sai_status_t stub_create_flow_entry_bulk_get_sessions(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_flow_entry_bulk_get_sessions(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_FLOW_ENTRY_BULK_GET_SESSION),object_count, object_id, mode, object_statuses);
}

static sai_dash_flow_api_t stub_dash_flow = {
    .create_flow_table = stub_create_flow_table,
    .remove_flow_table = stub_remove_flow_table,
    .set_flow_table_attribute = stub_set_flow_table_attribute,
    .get_flow_table_attribute = stub_get_flow_table_attribute,
    .create_flow_tables = stub_create_flow_tables,
    .remove_flow_tables = stub_remove_flow_tables,
    .create_flow_entry = stub_create_flow_entry,
    .remove_flow_entry = stub_remove_flow_entry,
    .set_flow_entry_attribute = stub_set_flow_entry_attribute,
    .get_flow_entry_attribute = stub_get_flow_entry_attribute,
    .create_flow_entries = stub_create_flow_entries,
    .remove_flow_entries = stub_remove_flow_entries,
    .create_flow_entry_bulk_get_session_filter = stub_create_flow_entry_bulk_get_session_filter,
    .remove_flow_entry_bulk_get_session_filter = stub_remove_flow_entry_bulk_get_session_filter,
    .set_flow_entry_bulk_get_session_filter_attribute = stub_set_flow_entry_bulk_get_session_filter_attribute,
    .get_flow_entry_bulk_get_session_filter_attribute = stub_get_flow_entry_bulk_get_session_filter_attribute,
    .create_flow_entry_bulk_get_session_filters = stub_create_flow_entry_bulk_get_session_filters,
    .remove_flow_entry_bulk_get_session_filters = stub_remove_flow_entry_bulk_get_session_filters,
    .create_flow_entry_bulk_get_session = stub_create_flow_entry_bulk_get_session,
    .remove_flow_entry_bulk_get_session = stub_remove_flow_entry_bulk_get_session,
    .set_flow_entry_bulk_get_session_attribute = stub_set_flow_entry_bulk_get_session_attribute,
    .get_flow_entry_bulk_get_session_attribute = stub_get_flow_entry_bulk_get_session_attribute,
    .create_flow_entry_bulk_get_sessions = stub_create_flow_entry_bulk_get_sessions,
    .remove_flow_entry_bulk_get_sessions = stub_remove_flow_entry_bulk_get_sessions,
};


/* SAI APIS for DASH_APPLIANCE */

static sai_status_t stub_create_dash_appliance(
        _Out_ sai_object_id_t *dash_appliance_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),dash_appliance_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_dash_appliance(
        _In_ sai_object_id_t dash_appliance_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),dash_appliance_id);
}

static sai_status_t stub_set_dash_appliance_attribute(
        _In_ sai_object_id_t dash_appliance_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),dash_appliance_id, attr);
}

static sai_status_t stub_get_dash_appliance_attribute(
        _In_ sai_object_id_t dash_appliance_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),dash_appliance_id, attr_count, attr_list);
}

static sai_status_t stub_create_dash_appliances(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_dash_appliances(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_DASH_APPLIANCE),object_count, object_id, mode, object_statuses);
}

static sai_dash_appliance_api_t stub_dash_appliance = {
    .create_dash_appliance = stub_create_dash_appliance,
    .remove_dash_appliance = stub_remove_dash_appliance,
    .set_dash_appliance_attribute = stub_set_dash_appliance_attribute,
    .get_dash_appliance_attribute = stub_get_dash_appliance_attribute,
    .create_dash_appliances = stub_create_dash_appliances,
    .remove_dash_appliances = stub_remove_dash_appliances,
};


/* SAI APIS for DASH_OUTBOUND_PORT_MAP */

static sai_status_t stub_create_outbound_port_map(
        _Out_ sai_object_id_t *outbound_port_map_id,
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),outbound_port_map_id, switch_id, attr_count, attr_list);
}

static sai_status_t stub_remove_outbound_port_map(
        _In_ sai_object_id_t outbound_port_map_id)
{
    SWSS_LOG_ENTER();

    return stub->remove((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),outbound_port_map_id);
}

static sai_status_t stub_set_outbound_port_map_attribute(
        _In_ sai_object_id_t outbound_port_map_id,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),outbound_port_map_id, attr);
}

static sai_status_t stub_get_outbound_port_map_attribute(
        _In_ sai_object_id_t outbound_port_map_id,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),outbound_port_map_id, attr_count, attr_list);
}

static sai_status_t stub_create_outbound_port_maps(
        _In_ sai_object_id_t switch_id,
        _In_ uint32_t object_count,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_object_id_t *object_id,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),switch_id, object_count, attr_count, attr_list, mode, object_id, object_statuses);
}

static sai_status_t stub_remove_outbound_port_maps(
        _In_ uint32_t object_count,
        _In_ const sai_object_id_t *object_id,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove((sai_object_type_t)(SAI_OBJECT_TYPE_OUTBOUND_PORT_MAP),object_count, object_id, mode, object_statuses);
}

static sai_status_t stub_create_outbound_port_map_port_range_entry(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(outbound_port_map_port_range_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_outbound_port_map_port_range_entry(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(outbound_port_map_port_range_entry);
}

static sai_status_t stub_set_outbound_port_map_port_range_entry_attribute(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(outbound_port_map_port_range_entry, attr);
}

static sai_status_t stub_get_outbound_port_map_port_range_entry_attribute(
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(outbound_port_map_port_range_entry, attr_count, attr_list);
}

static sai_status_t stub_create_outbound_port_map_port_range_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, outbound_port_map_port_range_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_outbound_port_map_port_range_entries(
        _In_ uint32_t object_count,
        _In_ const sai_outbound_port_map_port_range_entry_t *outbound_port_map_port_range_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, outbound_port_map_port_range_entry, mode, object_statuses);
}

static sai_dash_outbound_port_map_api_t stub_dash_outbound_port_map = {
    .create_outbound_port_map = stub_create_outbound_port_map,
    .remove_outbound_port_map = stub_remove_outbound_port_map,
    .set_outbound_port_map_attribute = stub_set_outbound_port_map_attribute,
    .get_outbound_port_map_attribute = stub_get_outbound_port_map_attribute,
    .create_outbound_port_maps = stub_create_outbound_port_maps,
    .remove_outbound_port_maps = stub_remove_outbound_port_maps,
    .create_outbound_port_map_port_range_entry = stub_create_outbound_port_map_port_range_entry,
    .remove_outbound_port_map_port_range_entry = stub_remove_outbound_port_map_port_range_entry,
    .set_outbound_port_map_port_range_entry_attribute = stub_set_outbound_port_map_port_range_entry_attribute,
    .get_outbound_port_map_port_range_entry_attribute = stub_get_outbound_port_map_port_range_entry_attribute,
    .create_outbound_port_map_port_range_entries = stub_create_outbound_port_map_port_range_entries,
    .remove_outbound_port_map_port_range_entries = stub_remove_outbound_port_map_port_range_entries,
};


/* SAI APIS for DASH_TRUSTED_VNI */

static sai_status_t stub_create_global_trusted_vni_entry(
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(global_trusted_vni_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_global_trusted_vni_entry(
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(global_trusted_vni_entry);
}

static sai_status_t stub_set_global_trusted_vni_entry_attribute(
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(global_trusted_vni_entry, attr);
}

static sai_status_t stub_get_global_trusted_vni_entry_attribute(
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(global_trusted_vni_entry, attr_count, attr_list);
}

static sai_status_t stub_create_global_trusted_vni_entries(
        _In_ uint32_t object_count,
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, global_trusted_vni_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_global_trusted_vni_entries(
        _In_ uint32_t object_count,
        _In_ const sai_global_trusted_vni_entry_t *global_trusted_vni_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, global_trusted_vni_entry, mode, object_statuses);
}

static sai_status_t stub_create_eni_trusted_vni_entry(
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->create(eni_trusted_vni_entry, attr_count, attr_list);
}

static sai_status_t stub_remove_eni_trusted_vni_entry(
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry)
{
    SWSS_LOG_ENTER();

    return stub->remove(eni_trusted_vni_entry);
}

static sai_status_t stub_set_eni_trusted_vni_entry_attribute(
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry,
        _In_ const sai_attribute_t *attr)
{
    SWSS_LOG_ENTER();

    return stub->set(eni_trusted_vni_entry, attr);
}

static sai_status_t stub_get_eni_trusted_vni_entry_attribute(
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry,
        _In_ uint32_t attr_count,
        _Inout_ sai_attribute_t *attr_list)
{
    SWSS_LOG_ENTER();

    return stub->get(eni_trusted_vni_entry, attr_count, attr_list);
}

static sai_status_t stub_create_eni_trusted_vni_entries(
        _In_ uint32_t object_count,
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry,
        _In_ const uint32_t *attr_count,
        _In_ const sai_attribute_t **attr_list,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkCreate(object_count, eni_trusted_vni_entry, attr_count, attr_list, mode, object_statuses);
}

static sai_status_t stub_remove_eni_trusted_vni_entries(
        _In_ uint32_t object_count,
        _In_ const sai_eni_trusted_vni_entry_t *eni_trusted_vni_entry,
        _In_ sai_bulk_op_error_mode_t mode,
        _Out_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    return stub->bulkRemove(object_count, eni_trusted_vni_entry, mode, object_statuses);
}

static sai_dash_trusted_vni_api_t stub_dash_trusted_vni = {
    .create_global_trusted_vni_entry = stub_create_global_trusted_vni_entry,
    .remove_global_trusted_vni_entry = stub_remove_global_trusted_vni_entry,
    .set_global_trusted_vni_entry_attribute = stub_set_global_trusted_vni_entry_attribute,
    .get_global_trusted_vni_entry_attribute = stub_get_global_trusted_vni_entry_attribute,
    .create_global_trusted_vni_entries = stub_create_global_trusted_vni_entries,
    .remove_global_trusted_vni_entries = stub_remove_global_trusted_vni_entries,
    .create_eni_trusted_vni_entry = stub_create_eni_trusted_vni_entry,
    .remove_eni_trusted_vni_entry = stub_remove_eni_trusted_vni_entry,
    .set_eni_trusted_vni_entry_attribute = stub_set_eni_trusted_vni_entry_attribute,
    .get_eni_trusted_vni_entry_attribute = stub_get_eni_trusted_vni_entry_attribute,
    .create_eni_trusted_vni_entries = stub_create_eni_trusted_vni_entries,
    .remove_eni_trusted_vni_entries = stub_remove_eni_trusted_vni_entries,
};


/* ==== API QUERY === */

sai_status_t sai_api_query(
        _In_ sai_api_t api,
        _Out_ void** api_method_table)
{
    SWSS_LOG_ENTER();

    if (api_method_table == NULL)
    {
        SWSS_LOG_ERROR("NULL method table passed to SAI API initialize");

        return SAI_STATUS_INVALID_PARAMETER;
    }

    if (api == SAI_API_UNSPECIFIED)
    {
        SWSS_LOG_ERROR("api ID is unspecified api");

        return SAI_STATUS_INVALID_PARAMETER;
    }

    switch((int)api)
    {
        case SAI_API_SWITCH:
            *api_method_table = (void**)&stub_switch;
            return SAI_STATUS_SUCCESS;
        case SAI_API_PORT:
            *api_method_table = (void**)&stub_port;
            return SAI_STATUS_SUCCESS;
        case SAI_API_FDB:
            *api_method_table = (void**)&stub_fdb;
            return SAI_STATUS_SUCCESS;
        case SAI_API_VLAN:
            *api_method_table = (void**)&stub_vlan;
            return SAI_STATUS_SUCCESS;
        case SAI_API_VIRTUAL_ROUTER:
            *api_method_table = (void**)&stub_virtual_router;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ROUTE:
            *api_method_table = (void**)&stub_route;
            return SAI_STATUS_SUCCESS;
        case SAI_API_NEXT_HOP:
            *api_method_table = (void**)&stub_next_hop;
            return SAI_STATUS_SUCCESS;
        case SAI_API_NEXT_HOP_GROUP:
            *api_method_table = (void**)&stub_next_hop_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ROUTER_INTERFACE:
            *api_method_table = (void**)&stub_router_interface;
            return SAI_STATUS_SUCCESS;
        case SAI_API_NEIGHBOR:
            *api_method_table = (void**)&stub_neighbor;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ACL:
            *api_method_table = (void**)&stub_acl;
            return SAI_STATUS_SUCCESS;
        case SAI_API_HOSTIF:
            *api_method_table = (void**)&stub_hostif;
            return SAI_STATUS_SUCCESS;
        case SAI_API_MIRROR:
            *api_method_table = (void**)&stub_mirror;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SAMPLEPACKET:
            *api_method_table = (void**)&stub_samplepacket;
            return SAI_STATUS_SUCCESS;
        case SAI_API_STP:
            *api_method_table = (void**)&stub_stp;
            return SAI_STATUS_SUCCESS;
        case SAI_API_LAG:
            *api_method_table = (void**)&stub_lag;
            return SAI_STATUS_SUCCESS;
        case SAI_API_POLICER:
            *api_method_table = (void**)&stub_policer;
            return SAI_STATUS_SUCCESS;
        case SAI_API_WRED:
            *api_method_table = (void**)&stub_wred;
            return SAI_STATUS_SUCCESS;
        case SAI_API_QOS_MAP:
            *api_method_table = (void**)&stub_qos_map;
            return SAI_STATUS_SUCCESS;
        case SAI_API_QUEUE:
            *api_method_table = (void**)&stub_queue;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SCHEDULER:
            *api_method_table = (void**)&stub_scheduler;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SCHEDULER_GROUP:
            *api_method_table = (void**)&stub_scheduler_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_BUFFER:
            *api_method_table = (void**)&stub_buffer;
            return SAI_STATUS_SUCCESS;
        case SAI_API_HASH:
            *api_method_table = (void**)&stub_hash;
            return SAI_STATUS_SUCCESS;
        case SAI_API_UDF:
            *api_method_table = (void**)&stub_udf;
            return SAI_STATUS_SUCCESS;
        case SAI_API_TUNNEL:
            *api_method_table = (void**)&stub_tunnel;
            return SAI_STATUS_SUCCESS;
        case SAI_API_L2MC:
            *api_method_table = (void**)&stub_l2mc;
            return SAI_STATUS_SUCCESS;
        case SAI_API_IPMC:
            *api_method_table = (void**)&stub_ipmc;
            return SAI_STATUS_SUCCESS;
        case SAI_API_RPF_GROUP:
            *api_method_table = (void**)&stub_rpf_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_L2MC_GROUP:
            *api_method_table = (void**)&stub_l2mc_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_IPMC_GROUP:
            *api_method_table = (void**)&stub_ipmc_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_MCAST_FDB:
            *api_method_table = (void**)&stub_mcast_fdb;
            return SAI_STATUS_SUCCESS;
        case SAI_API_BRIDGE:
            *api_method_table = (void**)&stub_bridge;
            return SAI_STATUS_SUCCESS;
        case SAI_API_TAM:
            *api_method_table = (void**)&stub_tam;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SRV6:
            *api_method_table = (void**)&stub_srv6;
            return SAI_STATUS_SUCCESS;
        case SAI_API_MPLS:
            *api_method_table = (void**)&stub_mpls;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DTEL:
            *api_method_table = (void**)&stub_dtel;
            return SAI_STATUS_SUCCESS;
        case SAI_API_BFD:
            *api_method_table = (void**)&stub_bfd;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ISOLATION_GROUP:
            *api_method_table = (void**)&stub_isolation_group;
            return SAI_STATUS_SUCCESS;
        case SAI_API_NAT:
            *api_method_table = (void**)&stub_nat;
            return SAI_STATUS_SUCCESS;
        case SAI_API_COUNTER:
            *api_method_table = (void**)&stub_counter;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DEBUG_COUNTER:
            *api_method_table = (void**)&stub_debug_counter;
            return SAI_STATUS_SUCCESS;
        case SAI_API_MACSEC:
            *api_method_table = (void**)&stub_macsec;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SYSTEM_PORT:
            *api_method_table = (void**)&stub_system_port;
            return SAI_STATUS_SUCCESS;
        case SAI_API_MY_MAC:
            *api_method_table = (void**)&stub_my_mac;
            return SAI_STATUS_SUCCESS;
        case SAI_API_IPSEC:
            *api_method_table = (void**)&stub_ipsec;
            return SAI_STATUS_SUCCESS;
        case SAI_API_GENERIC_PROGRAMMABLE:
            *api_method_table = (void**)&stub_generic_programmable;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ARS:
            *api_method_table = (void**)&stub_ars;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ARS_PROFILE:
            *api_method_table = (void**)&stub_ars_profile;
            return SAI_STATUS_SUCCESS;
        case SAI_API_TWAMP:
            *api_method_table = (void**)&stub_twamp;
            return SAI_STATUS_SUCCESS;
        case SAI_API_POE:
            *api_method_table = (void**)&stub_poe;
            return SAI_STATUS_SUCCESS;
        case SAI_API_ICMP_ECHO:
            *api_method_table = (void**)&stub_icmp_echo;
            return SAI_STATUS_SUCCESS;
        case SAI_API_PREFIX_COMPRESSION:
            *api_method_table = (void**)&stub_prefix_compression;
            return SAI_STATUS_SUCCESS;
        case SAI_API_SYNCE:
            *api_method_table = (void**)&stub_synce;
            return SAI_STATUS_SUCCESS;
        case SAI_API_BMTOR:
            *api_method_table = (void**)&stub_bmtor;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_ACL:
            *api_method_table = (void**)&stub_dash_acl;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_DIRECTION_LOOKUP:
            *api_method_table = (void**)&stub_dash_direction_lookup;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_ENI:
            *api_method_table = (void**)&stub_dash_eni;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_INBOUND_ROUTING:
            *api_method_table = (void**)&stub_dash_inbound_routing;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_METER:
            *api_method_table = (void**)&stub_dash_meter;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_OUTBOUND_CA_TO_PA:
            *api_method_table = (void**)&stub_dash_outbound_ca_to_pa;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_OUTBOUND_ROUTING:
            *api_method_table = (void**)&stub_dash_outbound_routing;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_VNET:
            *api_method_table = (void**)&stub_dash_vnet;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_PA_VALIDATION:
            *api_method_table = (void**)&stub_dash_pa_validation;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_VIP:
            *api_method_table = (void**)&stub_dash_vip;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_HA:
            *api_method_table = (void**)&stub_dash_ha;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_TUNNEL:
            *api_method_table = (void**)&stub_dash_tunnel;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_FLOW:
            *api_method_table = (void**)&stub_dash_flow;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_APPLIANCE:
            *api_method_table = (void**)&stub_dash_appliance;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_OUTBOUND_PORT_MAP:
            *api_method_table = (void**)&stub_dash_outbound_port_map;
            return SAI_STATUS_SUCCESS;
        case SAI_API_DASH_TRUSTED_VNI:
            *api_method_table = (void**)&stub_dash_trusted_vni;
            return SAI_STATUS_SUCCESS;
        default:
            break;
    }

    SWSS_LOG_ERROR("Invalid API type %d", api);

    return SAI_STATUS_INVALID_PARAMETER;
}


/* ==== GLOBAL APIS === */

sai_object_id_t sai_switch_id_query(
        _In_ sai_object_id_t object_id)
{
    SWSS_LOG_ENTER();

    return stub->switchIdQuery(object_id);
}

sai_object_type_t sai_object_type_query(
        _In_ sai_object_id_t object_id)
{
    SWSS_LOG_ENTER();

    return stub->objectTypeQuery(object_id);
}

sai_status_t sai_api_initialize(
        _In_ uint64_t flags,
        _In_ const sai_service_method_table_t *services)
{
    SWSS_LOG_ENTER();

    return stub->apiInitialize(flags, services);
}

sai_status_t sai_api_uninitialize(void)
{
    SWSS_LOG_ENTER();

    return stub->apiUninitialize();
}

sai_status_t sai_bulk_get_attribute(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t object_count,
        _In_ const sai_object_key_t *object_key,
        _Inout_ uint32_t *attr_count,
        _Inout_ sai_attribute_t **attr_list,
        _Inout_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for bulkGetAttribute!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_bulk_object_clear_stats(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t object_count,
        _In_ const sai_object_key_t *object_key,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Inout_ sai_status_t *object_statuses)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for bulkObjectClearStats!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_bulk_object_get_stats(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t object_count,
        _In_ const sai_object_key_t *object_key,
        _In_ uint32_t number_of_counters,
        _In_ const sai_stat_id_t *counter_ids,
        _In_ sai_stats_mode_t mode,
        _Inout_ sai_status_t *object_statuses,
        _Out_ uint64_t *counters)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for bulkObjectGetStats!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_dbg_generate_dump(
        _In_ const char *dump_file_name)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for dbgGenerateDump!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_get_maximum_attribute_count(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _Out_ uint32_t *count)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for getMaximumAttributeCount!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_get_object_count(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _Out_ uint32_t *count)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for getObjectCount!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_get_object_key(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _Inout_ uint32_t *object_count,
        _Inout_ sai_object_key_t *object_list)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for getObjectKey!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_log_set(
        _In_ sai_api_t api,
        _In_ sai_log_level_t log_level)
{
    SWSS_LOG_ENTER();

    return stub->logSet(api, log_level);
}

sai_status_t sai_object_type_get_availability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ uint64_t *count)
{
    SWSS_LOG_ENTER();

    return stub->objectTypeGetAvailability(switch_id, object_type, attr_count, attr_list, count);
}

sai_status_t sai_query_api_version(
        _Out_ sai_api_version_t *version)
{
    SWSS_LOG_ENTER();

    return stub->queryApiVersion(version);
}

sai_status_t sai_query_attribute_capability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ sai_attr_id_t attr_id,
        _Out_ sai_attr_capability_t *attr_capability)
{
    SWSS_LOG_ENTER();

    return stub->queryAttributeCapability(switch_id, object_type, attr_id, attr_capability);
}

sai_status_t sai_query_attribute_enum_values_capability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ sai_attr_id_t attr_id,
        _Inout_ sai_s32_list_t *enum_values_capability)
{
    SWSS_LOG_ENTER();

    return stub->queryAttributeEnumValuesCapability(switch_id, object_type, attr_id, enum_values_capability);
}

sai_status_t sai_query_object_stage(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list,
        _Out_ sai_object_stage_t *stage)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for queryObjectStage!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
sai_status_t sai_query_stats_capability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _Inout_ sai_stat_capability_list_t *stats_capability)
{
    SWSS_LOG_ENTER();

    return stub->queryStatsCapability(switch_id, object_type, stats_capability);
}

sai_status_t sai_query_stats_st_capability(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_type_t object_type,
        _Inout_ sai_stat_st_capability_list_t *stats_capability)
{
    SWSS_LOG_ENTER();

    return stub->queryStatsStCapability(switch_id, object_type, stats_capability);
}

sai_status_t sai_tam_telemetry_get_data(
        _In_ sai_object_id_t switch_id,
        _In_ sai_object_list_t obj_list,
        _In_ bool clear_on_read,
        _Inout_ sai_size_t *buffer_size,
        _Out_ void *buffer)
{
    SWSS_LOG_ENTER();

    SWSS_LOG_ERROR("FIXME, no implementation for tamTelemetryGetData!");
    return SAI_STATUS_NOT_IMPLEMENTED;
}
