# iptables

* `iptables` works by interacting with packet filtering hooks in Linux kernel, which is known as `netfilter`.
* `netfilter` hooks:
    * `NF_IP_PRE_ROUTING`: This hook will be triggered by any incoming traffic very soon after entering the network stack. This hook is processed before any routing decisions have been made regarding where to send the packet.
    * `NF_IP_LOCAL_IN`: This hook is triggered after an incoming packet has been routed if the packet is destined for the local system.
    * `NF_IP_FORWARD`: This hook is triggered after an incoming packet has been routed if the packet is to be forwarded to another host.
    * `NF_IP_LOCAL_OUT`: This hook is triggered by any locally created outbound traffic as soon as it hits the network stack.
    * `NF_IP_POST_ROUTING`: This hook is triggered by any outgoing or forwarded traffic after routing has taken place and just before being sent out on the wire.

![image](https://github.com/user-attachments/assets/9d4ac5a7-324f-4c50-8a79-e05ed28dbf88)


## references
