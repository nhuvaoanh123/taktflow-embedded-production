# Terminal Functions - RTP, eCAP, eQEP, ePWM

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 18-23 (6 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 18 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435718TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.3 RAM Trace Port (RTP)
Table 4-3.ZWT RAM Trace Port (RTP)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive StrengthDescription
Signal Name337
ZWT
EMIF_ADDR[21]/ RTP_CLK C17 I/O Pulldown Programmable, 20µA 8mA RTP packet clock, orGIO
EMIF_ADDR[18]/ RTP_DATA[0] D15 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[17]/ RTP_DATA[1] C14 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[16]/ RTP_DATA[2] D14 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[15]/ RTP_DATA[3] C13 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[14]/ RTP_DATA[4] C12 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[13]/ RTP_DATA[5] C11 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[12]/ RTP_DATA[6] C10 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_nCS[4]/ RTP_DATA[7] /GIOB[5] M17 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[11]/ RTP_DATA[8] C9 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[10]/ RTP_DATA[9] C8 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[9]/ RTP_DATA[10] C7 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[8]/ RTP_DATA[11] /N2HET2[15] C6 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[7]/ RTP_DATA[12] /N2HET2[13] C5 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[6]/ RTP_DATA[13] /N2HET2[11] C4 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_nCS[3]/ RTP_DATA[14] /N2HET2[9] K17 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_nCS[0]/ RTP_DATA[15] /N2HET2[7] N17 I/O Pulldown Programmable, 20µA 8mA RTP packet data, orGIO
EMIF_ADDR[19]/ RTP_nENA C15 I/O Pullup Programmable, 20µA 8mA RTP packet handshake, orGIO
EMIF_ADDR[20]/ RTP_nSYNC C16 I/O Pullup Programmable, 20µA 8mA RTP synchronization, orGIO
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.

<!-- Page 19 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435719TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.4 Enhanced Capture Modules (eCAP)
Table 4-4.ZWT Enhanced Capture Modules (eCAP)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET1[15]/MIBSPI1NCS[4]/N2HET2[22]/ ECAP1 N1 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 1I/O
MIBSPI3SOMI/AD1EXT_ENA/ ECAP2 V8 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 2I/O
MIBSPI3SIMO/AD1EXT_SEL[0]/ ECAP3 W8 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 3I/O
MIBSPI1NENA/MII_RXD[2]/N2HET1[23]/ ECAP4 G19 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 4I/O
MIBSPI5NENA/DMM_DATA[7]/MII_RXD[3]/ ECAP5 H18 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 5I/O
MIBSPI1NCS[0]/MIBSPI1SOMI[1]/MII_TXD[2]/ ECAP6 R2 I/O Pullup Fixed, 20µA 8mA Enhanced Capture Module 6I/O

<!-- Page 20 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435720TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.5 Enhanced Quadrature Encoder Pulse Modules (eQEP)
Table 4-5.ZWT Enhanced Quadrature Encoder Pulse Modules (eQEP) (1)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
MIBSPI3CLK/AD1EXT_SEL[1]/ eQEP1A V9 Input Pullup Fixed, 20µA - Enhanced QEP1 Input A
MIBSPI3NENA/MIBSPI3NCS[5]/N2HET1[31]/ eQEP1B W9 Input Pullup Fixed, 20µA - Enhanced QEP1 Input B
MIBSPI3NCS[0]/AD2EVT/ eQEP1I V10 I/O Pullup Fixed, 20µA 8mA Enhanced QEP1 Index
MIBSPI1NCS[1]/MII_COL/N2HET1[17]/ eQEP1S F3 I/O Pullup Fixed, 20µA 8mA Enhanced QEP1 Strobe
N2HET1[1]/MIBSPI4NENA/N2HET2[8]/ eQEP2A V2 Input Pullup Fixed, 20µA - Enhanced QEP2 Input A
N2HET1[3]/MIBSPI4NCS[0]/N2HET2[10]/ eQEP2B U1 Input Pullup Fixed, 20µA - Enhanced QEP2 Input B
GIOA[2]/N2HET2[0]/ eQEP2I C1 I/O Pullup Fixed, 20µA 8mA Enhanced QEP2 Index
N2HET1[30]/MII_RX_DV/ eQEP2S B11 I/O Pullup Fixed, 20µA 8mA Enhanced QEP2 Strobe
(1)These signals aredouble-synchronized andthen optionally filtered with a6-cycle VCLK4-based counter.

<!-- Page 21 -->
21TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Terminal Configuration andFunctions Copyright ©2014 -2016, Texas Instruments Incorporated4.2.1.6 Enhanced Pulse-Width Modulator Modules (ePWM)
Table 4-6.ZWT Enhanced Pulse-Width Modulator Modules (ePWM)
TERMINAL
SIGNAL
TYPEDEFAULT
PULL
STATEPULL
TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
ePWM1A D9
Output - - 8mA Enhanced PWM1 Output A
GIOA[5]/EXTCLKIN1/ ePWM1A B5(1)
ePWM1B D10
Output - - 8mA Enhanced PWM1 Output B
GIOA[6]/N2HET2[4]/ ePWM1B H3(1)
N2HET1[16]/ ePWM1SYNCI /ePWM1SYNCO A4 Input PulldownFixed,
20µA-External ePWM Sync Pulse
Input
N2HET1[11]/MIBSPI3NCS[4]/N2HET2[18]/
ePWM1SYNCOE3
Output Pulldown 20µA 2mA ZDExternal ePWM Sync Pulse
Output
N2HET1[16]/ePWM1SYNCI/ ePWM1SYNCO A4(1)
GIOA[7]/N2HET2[6]/ ePWM2A M1 Output Pulldown 20µA 8mA Enhanced PWM2 Output A
N2HET1[0]/MIBSPI4CLK/ ePWM2B K18 Output Pulldown 20µA 8mA Enhanced PWM2 Output B
N2HET1[2]/MIBSPI4SIMO/ ePWM3A W5 Output Pulldown 20µA 8mA Enhanced PWM3 Output A
N2HET1[5]/MIBSPI4SOMI/N2HET2[12]/ ePWM3B V6 Output Pulldown 20µA 8mA Enhanced PWM3 Output B
MIBSPI5NCS[0]/DMM_DATA[5]/ ePWM4A E19 Output Pulldown 20µA 8mA Enhanced PWM4 Output A
N2HET1[4]/MIBSPI4NCS[1]/ ePWM4B B12 Output Pulldown 20µA 8mA Enhanced PWM4 Output B
N2HET1[6]/SCI3RX/ ePWM5A W3 Output Pulldown 20µA 8mA Enhanced PWM5 Output A
N2HET1[13]/SCI3TX/N2HET2[20]/ ePWM5B N2 Output Pulldown 20µA 8mA Enhanced PWM5 Output B
N2HET1[18]/EMIF_RNW/ ePWM6A J1 Output - - 8mA Enhanced PWM6 Output A
N2HET1[20]/EMIF_nDQM[1]/ ePWM6B P2 Output - - 8mA Enhanced PWM6 Output B
N2HET1[9]/MIBSPI4NCS[3]/N2HET2[16]/ ePWM7A V7 Output - - 8mA Enhanced PWM7 Output A
N2HET1[7]/MIBSPI4NCS[2]/N2HET2[14]/ ePWM7B T1 Output - - 8mA Enhanced PWM7 Output B
AD1EVT/MII_RX_ER/RMII_RX_ER/ nTZ1_1 N19
Input PulldownFixed,
20µA- Trip Zone 1Input 1
MIBSPI3NCS[3]/I2C1_SCL/N2HET1[29]/ nTZ1_1 C3(1)
GIOB[7]/ nTZ1_2 F1
Input PulldownFixed,
20µA- Trip Zone 1Input 2
MIBSPI3NCS[2]/I2C1_SDA/N2HET1[27]/ nTZ1_2 B2(1)
MIBSPI1NCS[3]/N2HET1[21]/ nTZ1_3 J3
Input PullupFixed,
20µA- Trip Zone 1Input 3
N2HET1[10]/MIBSPI4NCS[4]/MII_TX_CLK/ nTZ1_3 D19(1)
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between the
available terminals forinput functionality.

<!-- Page 22 -->
22TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Terminal Configuration andFunctions Copyright ©2014 -2016, Texas Instruments Incorporated4.2.1.7 Data Modification Module (DMM)

<!-- Page 23 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435723TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-7.ZWT Data Modification Module (DMM)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive
StrengthDescription
Signal Name337
ZWT
DMM_CLK F17 I/O Pullup Programmable, 20µA 2mA ZD DMM clock, orGIO
DMM_DATA[0] L19 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
DMM_DATA[1] L18 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5NCS[2]/ DMM_DATA[2] W6 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5NCS[3]/ DMM_DATA[3] T12 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5CLK/ DMM_DATA[4] /MII_TXEN/RMII_TXEN H19 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5NCS[0]/ DMM_DATA[5] /ePWM4A E19 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5NCS[1]/ DMM_DATA[6] B6 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5NENA/ DMM_DATA[7] /MII_RXD[3]/ECAP5 H18 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SIMO[0]/ DMM_DATA[8] /MII_TXD[1]/RMII_TXD[1] J19 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SIMO[1]/ DMM_DATA[9] /AD1EXT_SEL[0] E16 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SIMO[2]/ DMM_DATA[10] /AD1EXT_SEL[1] H17 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SIMO[3]/ DMM_DATA[11] /I2C2_SDA/AD1EXT_SEL[2] G17 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SOMI[0]/ DMM_DATA[12] /MII_TXD[0]/RMII_TXD[0] J18 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SOMI[1]/ DMM_DATA[13] /AD1EXT_SEL[3] E17 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SOMI[2]/ DMM_DATA[14] /AD1EXT_SEL[4] H16 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
MIBSPI5SOMI[3]/ DMM_DATA[15] /I2C2_SCL/AD1EXT_ENA G16 I/O Pullup Programmable, 20µA 2mA ZD DMM data, orGIO
DMM_nENA F16 I/O Pullup Programmable, 20µA 2mA ZD DMM handshake, orGIO
DMM_SYNC J16 I/O Pullup Programmable, 20µA 2mA ZD DMM synchronization, orGIO