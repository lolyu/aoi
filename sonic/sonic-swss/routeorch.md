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
