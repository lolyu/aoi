# PCIe and IO

## PCIe basics

* PCIe follows OSI model:
    * physical layer
        * handles the transmission and reception of data over the physical link.
    * data link layer
        * two sub layers:
            * LLC (logic link control): manages flow control, error checking and link init.
            * DLC (data link control): packet framing, error detection and retransmission.
    * transaction layer
    * TLP (transaction layer packet)
    * switches and endpoints
    * configuration space

* traffic direction:
    * upstream/inbound traffic: from PCIe device to PCIe core
    * downstream/outbound traffic: from PCIe core to PCIe device

![image](https://github.com/user-attachments/assets/c56bd967-7ea1-43de-a60e-9bdba5230a76)


### TLP (transaction layer packet)
<img width="637" alt="image" src="https://github.com/user-attachments/assets/5a266f6f-fe7c-4e02-a06d-6eb288743408" />

### check PCIe device
* the following MLNX `MT27800` NIC card is inserted in a PCIe slot with x16 lanes - 8GT/s speed
```
$ sudo lspci -vvv
d6f5:00:02.0 Ethernet controller: Mellanox Technologies MT27800 Family [ConnectX-5 Virtual Function] (rev 80)
        Subsystem: Mellanox Technologies MT27800 Family [ConnectX-5 Virtual Function]
        Physical Slot: 1
        ...
                LnkCap: Port #0, Speed 8GT/s, Width x16, ASPM not supported
                        ClockPM- Surprise- LLActRep- BwNot- ASPMOptComp+
                LnkCtl: ASPM Disabled; RCB 64 bytes Disabled- CommClk-
                        ExtSynch- ClockPM- AutWidDis- BWInt- AutBWInt-
                LnkSta: Speed unknown (downgraded), Width x0 (downgraded)
                        TrErr- Train- SlotClk- DLActive- BWMgmt- ABWMgmt-
                DevCap2: Completion Timeout: Range ABC, TimeoutDis+, NROPrPrP-, LTR-
                         10BitTagComp-, 10BitTagReq-, OBFF Not Supported, ExtFmt-, EETLPPrefix-
                         EmergencyPowerReduction Not Supported, EmergencyPowerReductionInit-
                         FRS-, TPHComp-, ExtTPHComp-
                         AtomicOpsCap: 32bit- 64bit- 128bitCAS-
                DevCtl2: Completion Timeout: 50us to 50ms, TimeoutDis-, LTR-, OBFF Disabled
                         AtomicOpsCtl: ReqEn-
                LnkSta2: Current De-emphasis Level: -6dB, EqualizationComplete-, EqualizationPhase1-
                         EqualizationPhase2-, EqualizationPhase3-, LinkEqualizationRequest-
```


## packet forwarding
<img width="260" alt="image" src="https://github.com/user-attachments/assets/c0f354b3-cd66-43c9-a9bc-b3adc9bfee97" />

* receive:
    1. CPU writes the packet buffer address to the descriptor in the rx ring
    2. NIC reads the rx ring to get the packet buffer address - `PCIe downstream`
    3. NIC writes the packet to the packet buffer - `PCIe upstream`
    4. NIC updates the descriptor to signal packet receivement ready - `PCIe upstream`
    5. CPU reads the descriptor in the rx ring to get the packet
* send:
    8. CPU reads the free descriptor (finishes sending) in the tx ring
    9. CPU writes the packet buffer address to the descriptor in the tx ring
    10. NIC reads the packet buffer address from the descriptor - `PCIe downstream`
    11. NIC reads the packet from the packet buffer address - `PCIe downstream`
    12. NIC updates the descriptor to sigal packet send finish - `PCIe upstream`

* Though PCIe has decent bandwidth, the ring operations has overhead.

### optimization

