# RouteOrch

```
swss.rec.11.gz:2023-12-17.08:08:00.289083|ROUTE_TABLE:193.3.224.0/25|SET|nexthop:10.0.0.57,10.0.0.59,10.0.0.61,10.0.0.63|ifname:PortChannel101,PortChannel102,PortChannel103,PortChannel104|weight:1,1,1,1
```
```
127.0.0.1:6379> hgetall "ROUTE_TABLE:192.180.128.128/25"
1) "nexthop"
2) "10.0.0.57,10.0.0.59,10.0.0.61,10.0.0.63"
3) "ifname"
4) "PortChannel101,PortChannel102,PortChannel103,PortChannel104"
5) "weight"
6) "1,1,1,1"
127.0.0.1:6379>
```

## RouteOrch::doTask
* `RouteOrch::doTask` does the following:
    1. parse the `ROUTE_TABLE` entries
    2. create the nexthop group key(`NextHopGroupKey`)

* parse the nexthops
```cpp
                    else if (overlay_nh == false)
                    {
                        for (uint32_t i = 0; i < ipv.size(); i++)
                        {
                            if (i) nhg_str += NHG_DELIMITER;
                            if (alsv[i] == "tun0" && !(IpAddress(ipv[i]).isZero()))
                            {
                                alsv[i] = gIntfsOrch->getRouterIntfsAlias(ipv[i]);
                            }
                            if (!mpls_nhv.empty() && mpls_nhv[i] != "na")
                            {
                                nhg_str += mpls_nhv[i] + LABELSTACK_DELIMITER;
                            }
                            nhg_str += ipv[i] + NH_DELIMITER + alsv[i];
                        }

                        nhg = NextHopGroupKey(nhg_str, weights);
                    }
```

* add the route(`addRoute`) or remove the route 
    * what if 
```cpp
            if (op == SET_COMMAND)
                ...
                else if (m_syncdRoutes.find(vrf_id) == m_syncdRoutes.end() ||
                    m_syncdRoutes.at(vrf_id).find(ip_prefix) == m_syncdRoutes.at(vrf_id).end() ||
                    m_syncdRoutes.at(vrf_id).at(ip_prefix) != RouteNhg(nhg, ctx.nhg_index) ||
                    gRouteBulker.bulk_entry_pending_removal(route_entry) ||
                    ctx.using_temp_nhg)
                {
                    if (addRoute(ctx, nhg))
                        it = consumer.m_toSync.erase(it);
                    else
                        it++;
                }
            }
            else if (op == DEL_COMMAND)
            {
                if (removeRoute(ctx))
                    it = consumer.m_toSync.erase(it);
                else
                    it++;
            }

        // Flush the route bulker, so routes will be written to syncd and ASIC
        gRouteBulker.flush();
```

* perform add/remove route post actions(`adRoutePost` or `removeRoutePost`)


### addRoute
* `addRoute` does the following:
    * check the nexthop:
        * if the nexthop is interface, check if the interface exists
        * if the nexthop is ip neighbor, check if the neighbor is resolvable
    * add the route to the route bulker
    * flush the unfinished ops stored in the route bulker

```cpp
    if (it_route == m_syncdRoutes.at(vrf_id).end() || gRouteBulker.bulk_entry_pending_removal(route_entry))
    {
        if (blackhole)
        {
            route_attr.id = SAI_ROUTE_ENTRY_ATTR_PACKET_ACTION;
            route_attr.value.s32 = SAI_PACKET_ACTION_DROP;
        }
        else
        {
            route_attr.id = SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID;
            route_attr.value.oid = next_hop_id;
        }

        /* Default SAI_ROUTE_ATTR_PACKET_ACTION is SAI_PACKET_ACTION_FORWARD */
        object_statuses.emplace_back();
        sai_status_t status = gRouteBulker.create_entry(&object_statuses.back(), &route_entry, 1, &route_attr);
        if (status == SAI_STATUS_ITEM_ALREADY_EXISTS)
        {
            SWSS_LOG_ERROR("Failed to create route %s with next hop(s) %s: already exists in bulker",
                    ipPrefix.to_string().c_str(), nextHops.to_string().c_str());
            return false;
        }
    }
```

### addRoutePost

### removeRoute
### remoteRoutePost
