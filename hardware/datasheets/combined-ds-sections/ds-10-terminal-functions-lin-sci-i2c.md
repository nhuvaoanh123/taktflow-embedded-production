# Terminal Functions - LIN, SCI, I2C

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 28-32 (5 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 28 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435728TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.11 Local Interconnect Network Interface Module (LIN)
Table 4-11. ZWT Local Interconnect Network Interface Module (LIN)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
LIN1RX A7 I/O Pullup Programmable,
20µA2mA ZD LINreceive, orGIO
LIN1TX B7 I/O Pullup Programmable,
20µA2mA ZD LINtransmit, orGIO
N2HET2[19]/ LIN2RX P4 I/O Pulldown Programmable,
20µA2mA ZD LINreceive, orGIO
N2HET2[20]/ LIN2TX T5 I/O Pulldown Programmable,
20µA2mA ZD LINtransmit, orGIO

<!-- Page 29 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435729TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.12 Standard Serial Communication Interface (SCI)
Table 4-12. ZWT Standard Serial Communication Interface (SCI)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET1[6]/ SCI3RX /ePWM5A W3 I/O Pulldown Programmable,
20µA2mA ZD SCIreceive, orGIO
N2HET1[13]/ SCI3TX /N2HET2[20] /ePWM5B N2 I/O Pulldown Programmable,
20µA2mA ZD SCItransmit, orGIO
N2HET1[17]/ EMIF_nOE/ SCI4RX A13 I/O Pulldown Programmable,
20µA2mA ZD SCIreceive, orGIO
N2HET1[19]/ EMIF_nDQM[0]/ SCI4TX B13 I/O Pulldown Programmable,
20µA2mA ZD SCItransmit, orGIO

<!-- Page 30 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435730TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.13 Inter-Integrated Circuit Interface Module (I2C)
Table 4-13. ZWT Inter-Integrated Circuit Interface Module (I2C)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
MIBSPI3NCS[3]/ I2C1_SCL /N2HET1[29] /nTZ1_1 C3 I/O Pullup Programmable,
20µA2mA ZD I2Cserial clock, orGIO
MIBSPI3NCS[2]/ I2C1_SDA /N2HET1[27] /nTZ1_2 B2 I/O Pullup Programmable, 20uA 2mA ZD I2Cserial data, orGIO
MIBSPI5SOMI[3]/DMM_DATA[15]/ I2C2_SCL /AD1EXT_ENA G16 I/O Pullup Programmable, 20uA 2mA ZD I2Cserial clock, orGIO
MIBSPI5SIMO[3]/DMM_DATA[11]/ I2C2_SDA /AD1EXT_SEL[2] G17 I/O Pullup Programmable, 20uA 2mA ZD I2Cserial data, orGIO

<!-- Page 31 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435731TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.14 Multibuffered Serial Peripheral Interface Modules (MibSPI)
Table 4-14. ZWT Multibuffered Serial Peripheral Interface Modules (MibSPI)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive StrengthDescription
Signal Name337
ZWT
MIBSPI1CLK F18 I/O Pullup Programmable, 20µA 8mA MibSPI1 clock, orGIO
MIBSPI1NCS[0] /MIBSPI1SOMI[1]/MII_TXD[2]/ECAP6 R2 I/O Pullup Programmable, 20>µA 8mA MibSPI1 chip select, orGIO
MIBSPI1NCS[1] /MII_COL/N2HET1[17]/eQEP1S F3 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 chip select, orGIO
MIBSPI1NCS[2] /MDIO /N2HET1[19] G3 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 chip select, orGIO
MIBSPI1NCS[3] /N2HET1[21]/nTZ1_3 J3 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 chip select, orGIO
MIBSPI1NCS[4] U10 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 chip select, orGIO
N2HET1[15]/ MIBSPI1NCS[4] /N2HET2[22] /ECAP1 N1(1)
MIBSPI1NCS[5] U9 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 chip select, orGIO
N2HET1[24]/ MIBSPI1NCS[5] /MII_RXD[0]/RMII_RXD[0] P1(1)
MIBSPI1NENA /MII_RXD[2]/N2HET1[23]/ECAP4 G19 I/O Pullup Programmable, 20µA 2mA ZD MibSPI1 enable, orGIO
MIBSPI1SIMO[0] F19 I/O Pullup Programmable, 20µA 8mA MibSPI1 slave-in master-out, orGIO
N2HET1[8]/ MIBSPI1SIMO[1] /MII_TXD[3] E18 I/O Pulldown Programmable, 20µA 8mA MibSPI1 slave-in master-out, orGIO
MIBSPI1SOMI[0] G18 I/O Pullup Programmable, 20µA 8mA MibSPI1 slave-out master-in, orGIO
MIBSPI1NCS[0]/ MIBSPI1SOMI[1] /MII_TXD[2]/ECAP6 R2 I/O Pullup Programmable, 20µA 8mA MibSPI1 slave-out master-in, orGIO
N2HET2[3]/ MIBSPI2CLK E2 I/O Pulldown Programmable, 20µA 8mA MibSPI2 clock, orGIO
N2HET2[7]/ MIBSPI2NCS[0] N3 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI2 chip select, orGIO
N2HET2[12]/MIBSPI2NENA/ MIBSPI2NCS[1] D3 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI2 chip select, orGIO
N2HET2[12]/ MIBSPI2NENA /MIBSPI2NCS[1] D3 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI2 enable, orGIO
N2HET2[14]/ MIBSPI2SIMO D1 I/O Pulldown Programmable, 20µA 8mA MibSPI2 slave-in master-out, orGIO
N2HET2[13]/ MIBSPI2SOMI D2 I/O Pulldown Programmable, 20µA 8mA MibSPI2 slave-out master-in, orGIO
MIBSPI3CLK /AD1EXT_SEL[1]/eQEP1A V9 I/O Pullup Programmable, 20µA 8mA MibSPI3 clock, orGIO
MIBSPI3NCS[0] /AD2EVT/eQEP1I V10 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
MIBSPI3NCS[1] /MDCLK/N2HET1[25] V5 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
MIBSPI3NCS[2] /I2C1_SDA/N2HET1[27] /nTZ1_2 B2 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
MIBSPI3NCS[3] /I2C1_SCL/N2HET1[29] /nTZ1_1 C3 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
N2HET1[11]/ MIBSPI3NCS[4] /N2HET2[18] /ePWM1SYNCO E3 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
MIBSPI3NENA/ MIBSPI3NCS[5] /N2HET1[31] /eQEP1B W9 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 chip select, orGIO
MIBSPI3NENA /MIBSPI3NCS[5]/N2HET1[31]/eQEP1B W9 I/O Pullup Programmable, 20µA 2mA ZD MibSPI3 enable, orGIO
MIBSPI3SIMO /AD1EXT_SEL[0]/ECAP3 W8 I/O Pullup Programmable, 20µA 8mA MibSPI3 slave-in master-out, orGIO
MIBSPI3SOMI /AD1EXT_ENA/ECAP2 V8 I/O Pullup Programmable, 20µA 8mA MibSPI3 slave-out master-in, orGIO
N2HET1[0]/ MIBSPI4CLK /ePWM2B K18 I/O Pulldown Programmable, 20µA 8mA MibSPI4 clock, orGIO
N2HET1[3]/ MIBSPI4NCS[0] /N2HET2[10] /eQEP2B U1 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI4 chip select, orGIO

<!-- Page 32 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435732TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-14. ZWT Multibuffered Serial Peripheral Interface Modules (MibSPI) (continued)
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive StrengthDescription
Signal Name337
ZWT
N2HET1[4]/ MIBSPI4NCS[1] /ePWM4B B12 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI4 chip select, orGIO
N2HET1[7]/ MIBSPI4NCS[2] /N2HET2[14] /ePWM7B T1 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI4 chip select, orGIO
N2HET1[9]/ MIBSPI4NCS[3] /N2HET2[16] /ePWM7A V7 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI4 chip select, orGIO
N2HET1[10]/ MIBSPI4NCS[4] /MII_TX_CLK/nTZ1_3 D19 I/O Pulldown Programmable, 20µA 2mA ZD MibSPI4 chip select, orGIO
N2HET1[12]/ MIBSPI4NCS[5] /MII_CRS/RMII_CRS_DV B4 I/O Pulldown Programmable, 20µA 4mA MibSPI4 chip select, orGIO
N2HET1[1]/ MIBSPI4NENA /N2HET2[8] /eQEP2A V2 I/O Pulldown Programmable, 20µA 8mA MibSPI4 enable, orGIO
N2HET1[2]/ MIBSPI4SIMO /ePWM3A W5 I/O Pulldown Programmable, 20µA 8mA MibSPI4 slave-in master-out, orGIO
N2HET1[5]/ MIBSPI4SOMI /N2HET2[12] /ePWM3B V6 I/O Pulldown Programmable, 20µA 8mA MibSPI4 slave-out master-in, orGIO
MIBSPI5CLK /DMM_DATA[4]/MII_TXEN/RMII_TXEN H19 I/O Pullup Programmable, 20µA 8mA MibSPI5 clock, orGIO
MIBSPI5NCS[0] /DMM_DATA[5]/ePWM4A E19 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
MIBSPI5NCS[1] /DMM_DATA[6] B6 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
MIBSPI5NCS[2] /DMM_DATA[2] W6 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
MIBSPI5NCS[3] /DMM_DATA[3] T12 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
ETMDATA[24] /EMIF_DATA[8]/N2HET2[24]/ MIBSPI5NCS[4] L5 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
ETMDATA[25] /EMIF_DATA[9]/N2HET2[25]/ MIBSPI5NCS[5] M5 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 chip select, orGIO
MIBSPI5NENA /DMM_DATA[7] /MII_RXD[3]/ECAP5 H18 I/O Pullup Programmable, 20µA 2mA ZD MibSPI5 enable, orGIO
MIBSPI5SIMO[0] /DMM_DATA[8]/MII_TXD[1]/RMII_TXD[1] J19 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-in master-out, orGIO
MIBSPI5SIMO[1] /DMM_DATA[9]/AD1EXT_SEL[0] E16 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-in master-out, orGIO
MIBSPI5SIMO[2] /DMM_DATA[10]/AD1EXT_SEL[1] H17 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-in master-out, orGIO
MIBSPI5SIMO[3] /DMM_DATA[11]/I2C2_SDA/AD1EXT_SEL[2] G17 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-in master-out, orGIO
MIBSPI5SOMI[0] /DMM_DATA[12]/MII_TXD[0]/RMII_TXD[0] J18 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-out master-in, orGIO
MIBSPI5SOMI[1] /DMM_DATA[13]/AD1EXT_SEL[3] E17 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-out master-in, orGIO
MIBSPI5SOMI[2] /DMM_DATA[14]/AD1EXT_SEL[4] H16 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-out master-in, orGIO
MIBSPI5SOMI[3] /DMM_DATA[15]/I2C2_SCL/AD1EXT_ENA G16 I/O Pullup Programmable, 20µA 8mA MibSPI5 slave-out master-in, orGIO
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.