# networking virtualization

## how many softirq will be raised when sending a packet via a `veth` pair?
* one soft IRQ will be raised to notify the receiver the packet arrival.

## how many soft IRQ will be raised when sending a packet betwee two netns that are connected via bridge with veth devices?
* two soft IRQ will be raised:
    * one soft IRQ is raised during sending packet from the source netns
    * one soft IRQ is raised during the bridge passing the packet to the target netns

![image](https://github.com/user-attachments/assets/b6ced7ce-bcc6-4d57-8ed7-b319ed6f7239)


## references
