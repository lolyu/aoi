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
