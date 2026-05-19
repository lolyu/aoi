# dualtor testbed report

## purpose
This note captures the moving parts that matter in a SONiC dual ToR testbed and summarizes the route-check behavior recorded in `scripts/route_check/route_check_dualtor.txt`.

## topology summary
* two ToRs share the same server-facing links through a MUX/Y-cable design
* only one ToR should be active for a given server-facing port at a time
* when the active side changes, the standby ToR should steer server traffic through the tunnel path instead of a directly programmed neighbor

## control-plane components to watch
* `MuxOrch`, `MuxCableOrch`, and `MuxStateOrch` coordinate the configured state, requested state, and hardware-confirmed state for each MUX cable
* `MuxCable::setState` drives state transitions and updates the hardware MUX state tables
* `NeighOrch` programs or removes ASIC neighbor and nexthop objects as reachability changes
* route programming is reflected in `APPL_DB` `ROUTE_TABLE` and `ASIC_DB` `ASIC_STATE:SAI_OBJECT_TYPE_ROUTE_ENTRY`

## expected forwarding behavior
### active
* the active ToR keeps the server neighbor programmed in ASIC
* the host route should not remain as a tunnel route in `APPL_DB`

### standby
* the standby ToR removes the direct ASIC neighbor entry
* the host route is expected to point to `tun0` in `APPL_DB`
* the ASIC route should reference the tunnel nexthop instead of a directly attached neighbor path

## observed route-check behavior
The captured failure shows a transient mismatch during repeated `active -> standby -> active` state changes on `Ethernet4` for `192.168.0.101/32`.

### sequence seen in the capture
1. `orchagent` logs `Set MUX state from active to standby`
2. the ASIC neighbor entry for `192.168.0.101` is deleted
3. an ASIC route for `192.168.0.101/32` is installed with a tunnel nexthop
4. `APPL_DB` updates `ROUTE_TABLE:192.168.0.101` with `ifname=tun0` and `nexthop=0.0.0.0`
5. on `standby -> active`, the direct ASIC neighbor is restored, the ASIC route is deleted, and the `APPL_DB` route entry is removed

### reported mismatch
The route-check output reports:

* `APPL_DB` had no matching route snapshot at the moment of inspection
* `ASIC_DB` still contained `192.168.0.101/32`
* the failure was flagged as an `Unaccounted_ROUTE_ENTRY_TABLE_entries` mismatch

This indicates the testbed can briefly expose an inconsistent view between the application-facing route table and the ASIC-programmed route during MUX transitions.

## practical validation points
* correlate `orchagent` MUX transition notices with `APPL_DB` route updates
* confirm neighbor delete/add timing in `ASIC_DB`
* verify whether the route-check snapshot is taken before `ROUTE_TABLE` converges after a state change
* focus on host routes that should flip between direct neighbor resolution and tunnel forwarding

## references
* [muxorch](sonic-swss/muxorch.md)
* [SONiC neighbor system](neighbor.md)
* [Dualtor testbed setup](https://github.com/sonic-net/sonic-mgmt/blob/master/docs/testbed/README.testbed.DualtorSetup.md)
* [Dual ToR architecture](https://azure.github.io/SONiC/SONiC_Arch/DualToR/)
