# veth
```
# ip link add veth0 type veth peer name veth1
# ip addr add 192.168.1.1/24 dev veth0
# ip addr add 192.168.1.2/24 dev veth1
# ip link set veth0 up
# ip link set veth1 up
# echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
# echo 0 > /proc/sys/net/ipv4/conf/veth0/rp_filter
# echo 0 > /proc/sys/net/ipv4/conf/veth1/rp_filter
# echo 1 > /proc/sys/net/ipv4/conf/veth0/accept_local
# echo 1 > /proc/sys/net/ipv4/conf/veth1/accept_local
# ping 192.168.1.2 -I veth0
```
