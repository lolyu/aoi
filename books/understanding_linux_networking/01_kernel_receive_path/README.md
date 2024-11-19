# kernel receive path

## questions
### what's kernel net dev backlog queue?

### will the rx queue IRQ smp affinity cause packet delivery out-of-order?
The question is, if the smp affinity is like the following, will there be any out-of-order packet delivery?
```
# cat /proc/interrupts
            CPU0       CPU1       CPU2       CPU3
 ...
  33:          0          0      15839          0   PCI-MSI 2097152-edge      eth0-tx-0
  34:          0          0          0       4157   PCI-MSI 2097153-edge      eth0-rx-1
  35:          0       3585          0          0   PCI-MSI 2097154-edge      eth0-rx-2
# cat /proc/irq/34/smp_affinity
f
# cat /proc/irq/35/smp_affinity
f
```
* Yes, `f` means any of the four CPU cores could handle IRQ 34 for `eth0-rx-1` and IRQ 35 for `eth0-rx-2`; if there is a single TCP flow received from `eth0`, let's say it is landing on `eth0-rx-1` by `RSS`, but its IRQ 34 could be handled by any of the CPU cores as long as they are available. There is a room for out-of-order packet delivery to socket.
* In sum, packets get queued to rx queue with order, but lose ordering when enqueued to the socket.

## references
* https://lore.kernel.org/netdev/a5c6e92f-cc59-0214-56f6-66632c5e59c2@bursov.com/T/
* https://docs.kernel.org/networking/scaling.html
