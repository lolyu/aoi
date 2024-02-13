# overlay ECMP
* overlay ECMP is to solve the issue that multiple MUXes residing under the same ToR so the traffic to VIP is not ECMPed evenly to the MUXes.

## workflow
![image](https://github.com/lolyu/aoi/assets/35479537/5efad1ad-9d87-4a3d-84ff-c45146b7597a)


## packet signature
<img width="1241" alt="image" src="https://github.com/lolyu/aoi/assets/35479537/922e2c1b-0f49-4f8c-85cd-85242418d1b4">



## references
* https://www.youtube.com/watch?v=FCMVi_QWPRw
* https://github.com/sonic-net/SONiC/blob/master/doc/vxlan/Overlay%20ECMP%20with%20BFD.md
