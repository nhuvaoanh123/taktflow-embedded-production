# Terminal Functions - DMM, GIO

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 24-25 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 24 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435724TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.8 General-Purpose Input /Output (GIO)
Table 4-8.ZWT General-Purpose Input /Output (GIO)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive
StrengthDescription
Signal Name337
ZWT
GIOA[0] A5 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMDATA[28]/ EMIF_DATA[12]/N2HET2[28]/ GIOA[0] R5(1)
GIOA[1] C2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMDATA[29]/ EMIF_DATA[13]/N2HET2[29]/ GIOA[1] R6(1)
GIOA[2] /N2HET2[0]/eQEP2I C1 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
FRAYTX1/ GIOA[2] B15(1)
GIOA[3] /N2HET2[2] E1 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMDATA[30]/ EMIF_DATA[14]/N2HET2[30]/ GIOA[3] R7(1)
GIOA[4] A6 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMDATA[31]/ EMIF_DATA[15]/N2HET2[31]/ GIOA[4] R8(1)
GIOA[5] /EXTCLKIN1/ePWM1A B5 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMTRACECLKIN/EXTCLKIN2/ GIOA[5] R9(1)
GIOA[6] /N2HET2[4] /ePWM1B H3 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMTRACECLKOUT/ GIOA[6] R10(1)
GIOA[7] /N2HET2[6]/ePWM2A M1 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
ETMTRACECTL/ GIOA[7] R11(1)
GIOB[0] M2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
FRAYTX2/ GIOB[0] B8(1)
GIOB[1] K2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
FRAYTXEN1/ GIOB[1] B16(1)
GIOB[2] /DCAN4TX F2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
FRAYTXEN2/ GIOB[2] B9(1)
GIOB[3] /DCAN4RX W10 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
EMIF_nCAS/ GIOB[3] R4(1)
GIOB[4] G1 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
EMIF_nCS[2]/ GIOB[4] L17(1)
GIOB[5] G2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
EMIF_nCS[4]/RTP_DATA[7]/ GIOB[5] M17(1)

<!-- Page 25 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435725TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-8.ZWT General-Purpose Input /Output (GIO) (continued)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive
StrengthDescription
Signal Name337
ZWT
GIOB[6] /nERROR J2 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
EMIF_nRAS/ GIOB[6] R3(1)
GIOB[7] /nTZ1_2 F1 I/O Pulldown Programmable, 20µA 2mA ZD General-purpose I/O,external interrupt capable
EMIF_nWAIT/ GIOB[7] P3(1)
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.