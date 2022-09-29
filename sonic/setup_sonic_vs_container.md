# setup sonic-vs container
1. run docker-sonic-vs with host work directory mounted and ssh exposed:
```
# docker run --privileged -id -p 10022:22 --name base0 debian bash
# sudo ./create_vnet.sh -n 32 base0
# docker run --privileged -v /var/run/redis-vs/sw:/var/run/redis -v ~/workspace:/data --network container:base0 -d --name vs docker-sonic-vs
```
2. install related dev packages
```
# ls *dev*.deb
libhiredis-dev_0.14.0-3~bpo9+1_amd64.deb  libnl-genl-3-dev_3.5.0-1_amd64.deb   libsaimetadata-dev_1.0.0_amd64.deb  libsnmp-dev_5.7.3+dfsg-5_amd64.deb  libyang-dev_1.0.73_amd64.deb
libnl-3-dev_3.5.0-1_amd64.deb             libnl-nf-3-dev_3.5.0-1_amd64.deb     libsairedis-dev_1.0.0_amd64.deb     libswsscommon-dev_1.0.0_amd64.deb   libyang2-dev_2.0.112-6_amd64.deb
libnl-cli-3-dev_3.5.0-1_amd64.deb         libnl-route-3-dev_3.5.0-1_amd64.deb  libsaivs-dev_1.0.0_amd64.deb        libteam-dev_1.30-1_amd64.deb        sonic-device-data_1.0-1_all.deb
```
3. use vscode to remote ssh into the `docker-sonic-vs` to code

## references
* https://github.com/sonic-net/sonic-swss/tree/master/tests
