# Running BGP in data center at scale

## topology design

<img width="398" height="212" alt="image" src="https://github.com/user-attachments/assets/ffc58d7a-c7e8-4d7b-9a12-5b7b326c9ee5" />

* components:
    * RSW (rack switch)
    * FSW (fabric switch)
    * SSW (spine switch)
* different BGP configurations at different layers
* each T0 in the same pod connects to the same set of T1s
* each T1 in the same pod connects to separate set of T2s (spine plane)
* how many T2s in one spine plane?
    

## AS numbering

<img width="403" height="167" alt="image" src="https://github.com/user-attachments/assets/9d23376a-c7b1-471e-8127-267b4b408f61" />


* facebook
    * the T0/T1 in every pod has the same AS numbering structure with BGP confederations
        * T0 ASNs starts from 65401
        * T1 ASNs starts from 65301...65304
        * BGP confederation indentifier starts from 65101
    * T2s in the same spine plane have the same ASN

* azure
    * T0 has different ASN
    * T1s in the same cluster have the same ASN
    * T2s in the same spine plane have the same ASN
 
* What's the purpose of spine plane?
    * add inter-pods bandwidth -> add a new spine plane
    * each spine plane is a fault-domain
* **why T2s in the same spine plane share the same ASN?**
    * **to prevent east-west traffic within the same spine plane with the BGP loop prevention**

## route summarization
* two types of routes:
    * infrastructure
    * production
 
## routing policy

### reliability
* similar to bounce-back, if the T0-T1 link is down, the T1 can access the T0/server via bounced-back routes.
<img width="394" height="131" alt="image" src="https://github.com/user-attachments/assets/b06ff64b-5b92-437d-b665-869c20f6cde6" />

### maintainability


```
LIVE    → carries full production traffic
           normal BGP advertisements

     ↓ operator initiates drain

WARM    → maintains full RIB and FIB
           stops attracting NEW traffic (lowers LOCAL_PREF)
           still forwards in-flight traffic
           peers know it's going away → reroute gradually

     ↓ traffic drains off (avg 36 seconds)

DRAINED → no production traffic
           only infrastructure/diagnostic prefixes allowed
           safe to update/reboot/replace

     ↑ undrain reverses the sequence
```

* The device lowers `LOCAL_PREF` on all routes it advertises to peers:
```
LIVE state:
  FSW advertises pod aggregate with LOCAL_PREF = 100
  Peers prefer this FSW equally with others → ECMP

WARM state:
  FSW advertises same routes with LOCAL_PREF = 10
  Peers now prefer all other FSWs (still at 100)
  This FSW becomes last resort → traffic steers away
```

#### device/bgp isolation issue
* hard shutdown a bgp neighbor will result in traffic loss.
* egress: if a T1 shutdown a T0 neighbor, the routes learnt from the T0 will be removed from the RIB/FIB [1], and the withdraw of routes learnt from the T0 will be propagated to T2s [2]. [1] happens before [2], so T2s will still forward downstream traffic to the T1 that goes to the T0 neighbor, which will be **dropped**.
* ingress: if a T1 shutdown a T0 neighbor, the T0 will take some time processing the neighbor down event and remove the routes to T1 from the RIB/FIB. The removal takes some time and there will be traffic forwarded to the T1, and the upstream traffic ingressing the T1 will be forwarded eventually, **no ingress drop**.

### service reachability
* similar to Azure VIP/SLB

## policy configuration
