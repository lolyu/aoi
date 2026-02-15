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

## setup vagrant

```
# sudo apt install -y qemu-kvm libvirt-daemon-system libvirt-dev
# vagrant plugin install vagrant-libvirt
# vagrant up
```

## running VPP

* start VPP instance
```
# sudo /usr/bin/vpp -c startup1.conf
```

* connect VPP instance
```
# sudo vppctl -s /run/vpp/cli-vpp1.sock
```