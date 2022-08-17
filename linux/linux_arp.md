# linux ARP protocol
* neighbor state could be:
    * permanent
    * noarp
    * reachable
    * stale
    * none
    * incomplete
    * delay
    * probe
    * failed

# when does a valid entry becomes stale?
```
       base_reachable_time (since Linux 2.2)
              Once a neighbor has been found, the entry is considered to
              be valid for at least a random value between
              base_reachable_time/2 and 3*base_reachable_time/2.  An
              entry's validity will be extended if it receives positive
              feedback from higher level protocols.  Defaults to 30
              seconds.  This file is now obsolete in favor of
              base_reachable_time_ms.
```

## references
* https://linux.die.net/man/7/arp
* https://man7.org/linux/man-pages/man8/ip-neighbour.8.html
* https://man7.org/linux/man-pages/man7/arp.7.html
