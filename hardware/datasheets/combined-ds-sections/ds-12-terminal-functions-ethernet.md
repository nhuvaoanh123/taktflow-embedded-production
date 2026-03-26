# Terminal Functions - Ethernet

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 33-38 (6 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 33 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435733TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.15 Ethernet Controller
Table 4-15. ZWT Ethernet Controller: MDIO Interface
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
MDCLK T9 Output - - 8mA Serial clock output
MIBSPI3NCS[1]/ MDCLK /N2HET1[25] V5(1)
MDIO F4 I/O Pulldown Fixed, 20µA 8mA Serial data input/output
MIBSPI1NCS[2]/ MDIO /N2HET1[19] G3(1)
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.
Table 4-16. ZWT Ethernet Controller: Reduced Media Independent Interface (RMII)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
N2HET1[12]/MIBSPI4NCS[5]/MII_CRS/ RMII_CRS_DV B4 Input Pulldown Fixed, 20µA - RMII carrier sense anddata
valid
N2HET1[28]/MII_RXCLK/ RMII_REFCLK K19 Input Pulldown Fixed, 20µA 8mA EMII synchronous reference
clock forreceive, transmit and
control interface
AD1EVT/MII_RX_ER/ RMII_RX_ER /nTZ1_1 N19 Input Pulldown Fixed, 20µA - RMII receive error
N2HET1[24]/MIBSPI1NCS[5]/MII_RXD[0]/ RMII_RXD[0] P1 Input Pulldown Fixed, 20µA - RMII receive data
N2HET1[26]/MII_RXD[1]/ RMII_RXD[1] A14 Input Pulldown Fixed, 20µA - RMII receive data
MIBSPI5SOMI[0]/DMM_DATA[12]/MII_TXD[0]/ RMII_TXD[0] J18 Output Pullup 20µA 8mA RMII transmit data
MIBSPI5SIMO[0]/DMM_DATA[8]/MII_TXD[1]/ RMII_TXD[1] J19 Output Pullup 20µA 8mA RMII transmit data
MIBSPI5CLK/DMM_DATA[4]/MII_TXEN/ RMII_TXEN H19 Output Pullup 20µA 8mA RMII transmit enable

<!-- Page 34 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435734TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-17. ZWT Ethernet Controller: Media Independent Interface (MII)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
MII_COL W4 Input Pullup Fixed, 20µA - Collision detect
MIBSPI1NCS[1]/ MII_COL/ N2HET1[17] /eQEP1S F3(1)
MII_CRS V4 Input Pulldown Fixed, 20µA - Carrier sense andreceive valid
N2HET1[12] /MIBSPI4NCS[5]/ MII_CRS /RMII_CRS_DV B4(1)
MII_RX_DV U6 Input Pulldown Fixed, 20µA - Received data valid
N2HET1[30]/ MII_RX_DV /eQEP2S B11(1)
MII_RX_ER U5 Input Pulldown Fixed, 20µA - Receive error
AD1EVT/ MII_RX_ER /RMII_RX_ER/nTZ1_1 N19(1)
MII_RXCLK T4 Input Pulldown Fixed, 20µA - Receive clock
N2HET1[28]/ MII_RXCLK /RMII_REFCLK K19(1)
MII_RXD[0] U4 Input Pulldown Fixed, 20µA - Receive data
N2HET1[24]/MIBSPI1NCS[5]/ MII_RXD[0] /RMII_RXD[0] P1(1)
MII_RXD[1] T3 Input Pulldown Fixed, 20µA - Receive data
N2HET1[26]/ MII_RXD[1] /RMII_RXD[1] A14(1)
MII_RXD[2] U3 Input Pulldown Fixed, 20µA - Receive data
MIBSPI1NENA/ MII_RXD[2] /N2HET1[23]/ECAP4 G19(1)
MII_RXD[3] V3 Input Pulldown Fixed, 20µA - Receive data
MIBSPI5NENA/DMM_DATA[7]/ MII_RXD[3] /ECAP5 H18(1)
MII_TX_CLK U7 Input Pulldown Fixed, 20µA - Transmit clock
N2HET1[10]/MIBSPI4NCS[4]/ MII_TX_CLK /nTZ1_3 D19(1)
MII_TXD[0] U8 Output - - 8mA Transmit data
MIBSPI5SOMI[0]/DMM_DATA[12]/ MII_TXD[0] /RMII_TXD[0] J18(1)
MII_TXD[1] R1 Output - - 8mA Transmit data
MIBSPI5SIMO[0]/DMM_DATA[8]/ MII_TXD[1] /RMII_TXD[1] J19(1)
MII_TXD[2] T2 Output - - 8mA Transmit data
MIBSPI1NCS[0]/MIBSPI1SOMI[1]/ MII_TXD[2] /ECAP6 R2(1)
MII_TXD[3] G4 Output - - 8mA Transmit data
N2HET1[8]/MIBSPI1SIMO[1]/ MII_TXD[3] E18(1)

<!-- Page 35 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435735TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-17. ZWT Ethernet Controller: Media Independent Interface (MII) (continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
MII_TXEN E4 Output - - 8mA Transmit enable
MIBSPI5CLK/DMM_DATA[4]/ MII_TXEN /RMII_TXEN H19(1)
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.

<!-- Page 36 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435736TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.16 External Memory Interface (EMIF)
Table 4-18. External Memory Interface (EMIF) (2)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
EMIF_ADDR[0] /N2HET2[1] D4 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[1] /N2HET2[3] D5 Output Pulldown 20µA 8mA EMIF address
ETMDATA[11]/ EMIF_ADDR[2] E6 Output - - 8mA EMIF address
ETMDATA[10]/ EMIF_ADDR[3] E7 Output - - 8mA EMIF address
ETMDATA[9]/ EMIF_ADDR[4] E8 Output - - 8mA EMIF address
ETMDATA[8]/ EMIF_ADDR[5] E9 Output - - 8mA EMIF address
EMIF_ADDR[6] /RTP_DATA[13]/N2HET2[11] C4 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[7] /RTP_DATA[12]/N2HET2[13] C5 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[8] /RTP_DATA[11]/N2HET2[15] C6 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[9] /RTP_DATA[10] C7 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[10] /RTP_DATA[9] C8 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[11] /RTP_DATA[8] C9 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[12] /RTP_DATA[6] C10 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[13] /RTP_DATA[5] C11 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[14] /RTP_DATA[4] C12 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[15] /RTP_DATA[3] C13 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[16] /RTP_DATA[2] D14 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[17] /RTP_DATA[1] C14 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[18] /RTP_DATA[0] D15 Output Pulldown 20µA 8mA EMIF address
EMIF_ADDR[19] /RTP_nENA C15 Output Pullup 20µA 8mA EMIF address
EMIF_ADDR[20] /RTP_nSYNC C16 Output Pullup 20µA 8mA EMIF address
EMIF_ADDR[21] /RTP_CLK C17 Output Pulldown 20µA 8mA EMIF address
ETMDATA[12]/ EMIF_BA[0] E13 Output Pulldown 20µA 8mA EMIF bank address oraddress
lineN2HET1[23]/ EMIF_BA[0] J4(1)
EMIF_BA[1] /N2HET2[5] D16 Output Pulldown 20µA 8mA EMIF bank address oraddress
line

<!-- Page 37 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435737TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-18. External Memory Interface (EMIF) (2)(continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
EMIF_CKE L3 Output - - 8mA EMIF clock enable
EMIF_CLK /ECLK2 K3 Output Pulldown 20µA 8mA EMIF clock
ETMDATA[16]/ EMIF_DATA[0] K15 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[17]/ EMIF_DATA[1] L15 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[18]/ EMIF_DATA[2] M15 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[19]/ EMIF_DATA[3] N15 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[20]/ EMIF_DATA[4] E5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[21]/ EMIF_DATA[5] F5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[22]/ EMIF_DATA[6] G5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[23]/ EMIF_DATA[7] K5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[24]/ EMIF_DATA[8] /N2HET2[24]/MIBSPI5NCS[4] L5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[25]/ EMIF_DATA[9] /N2HET2[25]/MIBSPI5NCS[5] M5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[26]/ EMIF_DATA[10] /N2HET2[26] N5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[27]/ EMIF_DATA[11] /N2HET2[27] P5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[28]/ EMIF_DATA[12] /N2HET2[28]/GIOA[0] R5 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[29]/ EMIF_DATA[13] /N2HET2[29]/GIOA[1] R6 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[30]/ EMIF_DATA[14] /N2HET2[30]/GIOA[3] R7 I/O Pulldown Fixed, 20µA 8mA EMIF data
ETMDATA[31]/ EMIF_DATA[15] /N2HET2[31]/GIOA[4] R8 I/O Pulldown Fixed, 20µA 8mA EMIF data
EMIF_nCAS /GIOB[3] R4 Output Pulldown 20µA 8mA EMIF column address strobe
EMIF_nCS[0] /RTP_DATA[15]/N2HET2[7] N17 Output Pulldown 20µA 8mA EMIF chip select, synchronous
EMIF_nCS[2] /GIOB[4] L17 Output Pulldown 20µA 8mA EMIF chip select, asynchronous
EMIF_nCS[3] /RTP_DATA[14]/N2HET2[9] K17 Output Pulldown 20µA 8mA EMIF chip select, asynchronous
EMIF_nCS[4] /RTP_DATA[7]/GIOB[5] M17 Output Pulldown 20µA 8mA EMIF chip select, asynchronous
ETMDATA[15]/ EMIF_nDQM[0] E10 Output Pulldown 20µA 8mA EMIF byte enable
N2HET1[19]/ EMIF_nDQM[0] /SCI4TX B13(1)
ETMDATA[14]/ EMIF_nDQM[1] E11 Output Pulldown 20µA 8mA EMIF byte enable
N2HET1[20]/ EMIF_nDQM[1] /ePWM6B P2(1)
N2HET1[21]/ EMIF_nDQM[2] H4 Output Pulldown 20µA 8mA EMIF byte enable
N2HET1[22]/ EMIF_nDQM[3] B3 Output Pulldown 20µA 8mA EMIF byte enable

<!-- Page 38 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435738TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-18. External Memory Interface (EMIF) (2)(continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
ETMDATA[13]/ EMIF_nOE E12 Output Pulldown 20µA 8mA EMIF output enable
N2HET1[17]/ EMIF_nOE /SCI4RX A13(1)
EMIF_nRAS /GIOB[6] R3 Output Pulldown 20µA 8mA EMIF rowaddress strobe
EMIF_nWAIT /GIOB[7] P3 Input Pullup Fixed, 20µA - EMIF wait
EMIF_nWE /EMIF_RNW D17 Output - - 8mA EMIF write enable
EMIF_nWE/ EMIF_RNW D17 Output - - 8mA EMIF read-not-write
N2HET1[18]/ EMIF_RNW /ePWM6A J1(1)
(1)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.
(2)Bydefault, theEMIF interface pins aretheprimary pins before configurating theIOMM (IOMuxing Module). The output buffers ofthese pins areforced totri-state until enabled bysetting
PINMMR174[8] =0andPINMMR174[9] =1."