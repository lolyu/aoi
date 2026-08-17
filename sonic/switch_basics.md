# switch basics

<img width="617" height="786" alt="image" src="https://github.com/user-attachments/assets/c139d3da-5a22-4f7e-a644-5df6614d94c4" />


* PHY: physical layer chip
* DSP: digital signal processor

```
PHY
├── Analog front end    (amplifiers, filters)
├── DSP          ◄───── this is just one piece
│   ├── Equalization
│   ├── CDR
│   └── FEC
├── Encoding/decoding   (64b/66b, PAM4)
└── Auto-negotiation
```


## what are LANEs?

* Lanes are individual **parallel signal paths** used to split a high-speed link into multiple lower-speed streams.

```
Single lane at 800G:
  800 billion bits per second on one wire
  Signal integrity nightmare — extremely hard to achieve
  SerDes design becomes impossibly complex

8 lanes at 100G each:
  100G per lane — much more manageable
  All 8 lanes transmit simultaneously in parallel
  Combined = 800G total ✅
```

```
Switch ASIC (transmit side):

  800G data stream
       │
       ▼
  ┌─────────────┐
  │  Splitter   │
  └─────────────┘
    │  │  │  │  │  │  │  │
    ▼  ▼  ▼  ▼  ▼  ▼  ▼  ▼
   L0 L1 L2 L3 L4 L5 L6 L7    ← 8 lanes × 100G each
    │  │  │  │  │  │  │  │
    ▼  ▼  ▼  ▼  ▼  ▼  ▼  ▼
  8 × SerDes (one per lane)
    │  │  │  │  │  │  │  │
    ▼  ▼  ▼  ▼  ▼  ▼  ▼  ▼
  8 × PCB traces → transceiver

Transceiver (receive side):
  Recombine 8 lanes → 800G output to fiber
```

* **Each lane has its own PHY**
    * every lane has independent signal conditioning:

```
Lane 0: SerDes → CDR → equalization → FEC
Lane 1: SerDes → CDR → equalization → FEC
Lane 2: SerDes → CDR → equalization → FEC
...
Lane 7: SerDes → CDR → equalization → FEC
```

* Each lane independently locks its clock, corrects errors, equalizes signal. They don't need to be in sync with each other — the receiver reorders them.

* Lane counts by speed

| Total speed | Lanes | Speed per lane | Encoding |
|---|---|---|---|
| 100G | 4 | 25G | NRZ |
| 400G | 8 | 50G | PAM4 |
| 800G | 8 | 100G | PAM4 |
| 1.6T | 8 | 200G | PAM4 |
