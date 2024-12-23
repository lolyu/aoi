# kernel receive path

## questions
### what's RSS (receive side scaling)?
* `RSS` determines which rx queue receives the packet and hence the CPU that will run the hardware interrupt handler.
	* each receive queue has a separate IRQ associated with it. The NIC triggers this to notify a CPU when new packets arrive on the given queue.

### what's RPS (receive packet steering)?
* `RPS` selects which CPU to perform protocol processing above the interrupt handler.
	* after the `ksoftirqd` on the CPU that handles the IRQ for the rx queue polls packets from the rx queue, it hashes the packets and directs them to the corresponding CPU's backlog queue.

### what's `/proc/net/softnet_stat`?
* `/proc/net/softnet_stat` stores statistics that relate to `softirq`'s handling of network packets.
```
$ cat /proc/net/softnet_stat
0009dbb7 00000000 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
000a9fd6 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000001
000a1744 0000001b 00000008 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000002
000b054e 00000000 00000001 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000003
```
* each line of `/proc/net/softnetstat` corresponds to a per-cpu `struct softnet_data` structure.
	* the first value is `softnet_data.processed`
 	* the second value is `softnet_data.dropped`
	* the third value is `softnet_data.time_squeezed`
* the `softnet_data.dropped` is increased when the CPU's backlog queue has no room to store packets.
	* Refer to [RPS](https://github.com/lolyu/aoi/blob/master/books/understanding_linux_networking/01_kernel_receive_path/RPS.md)

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

### what is interrupt coalesce?
* interrupt coalesce is a technique used to moderate the interrupt rate.
    * batching network data at the network interface card (NIC) in order to increase the amount of data delivered to the kernel with one hardware interrupt.
* the `NiC` interrupt coalesce could be configured/show by `ethtool` [3]:
```
# ethtool -c eth0
Coalesce parameters for eth0:
Adaptive RX: n/a  TX: n/a
stats-block-usecs: 0
sample-interval: n/a
pkt-rate-low: n/a
pkt-rate-high: n/a

rx-usecs: 20                            // number of ms to delay a RX interrupt after packet arrival
rx-frames: 5                            // number of packets to delay a RX interrupt after packet arrival
rx-usecs-irq: 0                         // Number of microseconds to delay a TX interrupt after sending a packet while the host is also servicing an IRQ
rx-frames-irq: 5                        // Number of packets to delay an RX interrupt after packet arrival while the host is also servicing an IRQ

tx-usecs: 72
tx-frames: 53
tx-usecs-irq: 0
tx-frames-irq: 5

rx-usecs-low: n/a
rx-frame-low: n/a
tx-usecs-low: n/a
tx-frame-low: n/a

rx-usecs-high: n/a
rx-frame-high: n/a
tx-usecs-high: n/a
tx-frame-high: n/a
```
* also, `NAPI` helps interrupt coalesce by disable local CPU hard IRQ when polling packets [4]:

```c
// net/core/dev.c
static void net_rx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);
	unsigned long time_limit = jiffies + 2;
	int budget = netdev_budget;
	void *have;

	local_irq_disable();
    ...
```

### how to check kernel receive packet drop?
* possible drop situations:
	* NiC buffer overrun
		* `ethtool -S eth0`
 	* `RPS` CPU backlog queue drop
		* `/proc/net/softnet_stat`
  	* socket queue drop
		* `ss -mnp`


## references
* [1] https://lore.kernel.org/netdev/a5c6e92f-cc59-0214-56f6-66632c5e59c2@bursov.com/T/
* [2] https://docs.kernel.org/networking/scaling.html
* [3] https://knowledge.informatica.com/s/article/80204
* [4] https://stackoverflow.com/questions/28090086/what-are-the-advantages-napi-before-the-irq-coalesce
