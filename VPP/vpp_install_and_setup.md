# Install and setup VPP

## install VPP

```
$ curl -s https://packagecloud.io/install/repositories/fdio/2206/script.deb.sh | sudo bash
$ sudo apt-get update
$ sudo apt-get install vpp vpp-plugin-core vpp-plugin-dpdk
$ sudo apt-get install python3-vpp-api vpp-dbg vpp-dev
```

## running VPP
```
$ sudo usermod -a -G vpp lolv
$ newgrp vpp
```

* a reboot is needed to apply the hugepage config
