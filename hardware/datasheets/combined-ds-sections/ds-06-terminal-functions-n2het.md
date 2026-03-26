# Terminal Functions - N2HET

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 14-17 (4 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 14 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435714TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.2 Enhanced High-End Timer Modules (N2HET)
Table 4-2.ZWT Enhanced High-End Timer Modules (N2HET)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET1[0] /MIBSPI4CLK/ePWM2B K18 I/O PulldownProgrammable,
20µA2mAZDN2HET1 time input capture or
output compare, orGIO
N2HET1[1] /MIBSPI4NENA/N2HET2[8]/eQEP2A V2 I/O PulldownProgrammable,
20µA2mAZDN2HET1 time input capture or
output compare, orGIO
N2HET1[2] /MIBSPI4SIMO/ePWM3A W5 I/O PulldownProgrammable,
20µA2mAZDN2HET1 time input capture or
output compare, orGIO
N2HET1[3] /MIBSPI4NCS[0]/N2HET2[10]/eQEP2B U1 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[4] /MIBSPI4NCS[1]/ePWM4B B12 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[5] /MIBSPI4SOMI/N2HET2[12]/ePWM3B V6 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[6] /SCI3RX/ePWM5A W3 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[7] /MIBSPI4NCS[2]/N2HET2[14]/ePWM7B T1 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[8] /MIBSPI1SIMO[1]/MII_TXD[3] E18 I/O PulldownProgrammable,
20µA8mAN2HET1 time input capture or
output compare, orGIO
N2HET1[9] /MIBSPI4NCS[3]/N2HET2[16]/ePWM7A V7 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[10] /MIBSPI4NCS[4]/MII_TX_CLK/nTZ1_3 D19 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[11] /MIBSPI3NCS[4]/N2HET2[18]/ePWM1SYNCO E3 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[12] /MIBSPI4NCS[5]/MII_CRS/RMII_CRS_DV B4 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[13] /SCI3TX/N2HET2[20]/ePWM5B N2 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[14] A11 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[15] /MIBSPI1NCS[4]/N2HET2[22]/ECAP1 N1 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[16] /ePWM1SYNCI/ePWM1SYNCO A4 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO

<!-- Page 15 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435715TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-2.ZWT Enhanced High-End Timer Modules (N2HET) (continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET1[17] /EMIF_nOE/SCI4RX A13
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI1NCS[1]/MII_COL/ N2HET1[17] /eQEP1S F3(1)
N2HET1[18] /EMIF_RNW/ePWM6A J1 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[19] /EMIF_nDQM[0]/SCI4TX B13
I/O PulldownProgrammable,
20µA2mAZDN2HET1 time input capture or
output compare, orGIO MIBSPI1NCS[2] /MDIO/ N2HET1[19] G3(1)
N2HET1[20] /EMIF_nDQM[1]/ePWM6B P2 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[21] /EMIF_nDQM[2] H4
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI1NCS[3]/ N2HET1[21] /nTZ1_3 J3(1)
N2HET1[22] /EMIF_nDQM[3] B3 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[23] /EMIF_BA[0] J4
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI1NENA/MII_RXD[2]/ N2HET1[23] /ECAP4 G19(1)
N2HET1[24] /MIBSPI1NCS[5]/MII_RXD[0]/RMII_RXD[0] P1 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[25] M3
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI3NCS[1] /MDCLK/ N2HET1[25] V5(1)
N2HET1[26] /MII_RXD[1]/RMII_RXD[1] A14 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[27] A9
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI3NCS[2]/I2C1_SDA/ N2HET1[27] /nTZ1_2 B2(1)
N2HET1[28] /MII_RXCLK/RMII_REFCLK K19 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[29] A3
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI3NCS[3]/I2C1_SCL/ N2HET1[29] /nTZ1_1 C3(1)
N2HET1[30] /MII_RX_DV/eQEP2S B11 I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO
N2HET1[31] J17
I/O PulldownProgrammable,
20µA2mA ZDN2HET1 time input capture or
output compare, orGIO MIBSPI3NENA/MIBSPI3NCS[5]/ N2HET1[31] /eQEP1B W9(1)
N2HET2[0] D6
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO GIOA[2]/ N2HET2[0] /eQEP2I C1(1)
N2HET2[1] /N2HET1_NDIS D8
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_ADDR[0]/ N2HET2[1] D4(1)

<!-- Page 16 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435716TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-2.ZWT Enhanced High-End Timer Modules (N2HET) (continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET2[2] /N2HET2_NDIS D7
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO GIOA[3]/ N2HET2[2] E1(1)
N2HET2[3] /MIBSPI2CLK E2
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_ADDR[1]/ N2HET2[3] D5(1)
N2HET2[4] D13
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO GIOA[6]/ N2HET2[4] /ePWM1B H3(1)
N2HET2[5] D12
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_BA[1]/ N2HET2[5] D16(1)
N2HET2[6] D11
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO GIOA[7]/ N2HET2[6] /ePWM2A M1(1)
N2HET2[7] /MIBSPI2NCS[0] N3
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_nCS[0]/RTP_DATA[15]/ N2HET2[7] N17(1)
N2HET2[8] K16
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[1]/MIBSPI4NENA/ N2HET2[8] /eQEP2A V2(1)
N2HET2[9] L16
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_nCS[3]/RTP_DATA[14]/ N2HET2[9] K17(1)
N2HET2[10] M16
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[3]/MIBSPI4NCS[0]/ N2HET2[10] /eQEP2B U1(1)
N2HET2[11] N16
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_ADDR[6]/RTP_DATA[13]/ N2HET2[11] C4(1)
N2HET2[12] /MIBSPI2NENA/MIBSPI2NCS[1] D3
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[5]/MIBSPI4SOMI/ N2HET2[12] /ePWM3B V6(1)
N2HET2[13] /MIBSPI2SOMI D2
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_ADDR[7]/RTP_DATA[12]/ N2HET2[13] C5(1)
N2HET2[14] /MIBSPI2SIMO D1
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[7]/MIBSPI4NCS[2]/ N2HET2[14] /ePWM7B T1(1)
N2HET2[15] K4
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO EMIF_ADDR[8]/RTP_DATA[11]/ N2HET2[15] C6(1)
N2HET2[16] L4
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[9]/MIBSPI4NCS[3]/ N2HET2[16] /ePWM7A V7(1)
N2HET2[17] M4 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO

<!-- Page 17 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435717TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-2.ZWT Enhanced High-End Timer Modules (N2HET) (continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET2[18] N4
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[11]/MIBSPI3NCS[4]/ N2HET2[18] /ePWM1SYNCO E3(1)
N2HET2[19] /LIN2RX P4 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
N2HET2[20] /LIN2TX T5
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[13]/SCI3TX/ N2HET2[20] /ePWM5B N2(1)
N2HET2[21] T6 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
N2HET2[22] T7
I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO N2HET1[15]/MIBSPI1NCS[4]/ N2HET2[22] /ECAP1 N1(1)
N2HET2[23] T8 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[24] /EMIF_DATA[8]/ N2HET2[24] /MIBSPI5NCS[4] L5 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[25] /EMIF_DATA[9]/ N2HET2[25] /MIBSPI5NCS[5] M5 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[26] /EMIF_DATA[10]/ N2HET2[26] N5 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[27] /EMIF_DATA[11]/ N2HET2[27] P5 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[28]/ EMIF_DATA[12]/ N2HET2[28] /GIOA[0] R5 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[29]/ EMIF_DATA[13]/ N2HET2[29] /GIOA[1] R6 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[30]/ EMIF_DATA[14]/ N2HET2[30] /GIOA[3] R7 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
ETMDATA[31]/ EMIF_DATA[15]/ N2HET2[31] /GIOA[4] R8 I/O PulldownProgrammable,
20µA2mA ZDN2HET2 time input capture or
output compare, orGIO
N2HET2[1]/ N2HET1_NDIS D8 Input Pulldown Fixed, 20µA 2mA ZD N2HET1 Disable
N2HET2[2]/ N2HET2_NDIS D7 Input Pulldown Fixed, 20µA 2mA ZD N2HET2 Disable
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.