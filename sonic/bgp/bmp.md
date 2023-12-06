# setup BMP on SONiC

1. start `bgpd` with `bmp` module
1.1 change the `bgpd` startup command
```
[program:bgpd]
command=/usr/lib/frr/bgpd -A 127.0.0.1 -M bmp
priority=5
stopsignal=KILL
...
```

1.2 restart `bgpd` and `bgpcfgd`
```
# supervisorctl restart bgpd
# supervisorctl restart bgpcfgd
```

2. configure BMP settings
```
svcstr-7050-acs-1# config
svcstr-7050-acs-1(config)# router bgp 65100
svcstr-7050-acs-1(config-router)# bmp targets demo
svcstr-7050-acs-1(config-bgp-bmp)# bmp listener 127.0.0.1 port 8900
svcstr-7050-acs-1(config-bgp-bmp)# bmp monitor ipv4 unicast post-policy
svcstr-7050-acs-1(config-bgp-bmp)# bmp monitor ipv4 multicast post-policy
svcstr-7050-acs-1(config-bgp-bmp)# bmp monitor ipv6 unicast post-policy
svcstr-7050-acs-1(config-bgp-bmp)# bmp monitor ipv6 multicast post-policy
svcstr-7050-acs-1(config-bgp-bmp)# end
svcstr-7050-acs-1# exit
```

3. use bmp_listener.py to dump 
