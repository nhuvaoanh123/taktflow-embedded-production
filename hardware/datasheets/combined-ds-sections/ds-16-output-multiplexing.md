# Output Multiplexing Table

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 47-52 (6 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 47 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435747TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.2 Multiplexing
This microcontroller has several interfaces and uses extensive multiplexing tobring outthefunctions asrequired bythetarget application. The
multiplexing ismostly ontheoutput signals. Afewinputs arealso multiplexed toallow thesame input signal tobedriven infrom analternative
terminal. Formore information onmultiplexing, refer totheIOMM chapter ofthedevice specific technical reference manual.
4.2.2.1 Output Multiplexing
Table 4-28. Output Multiplexing
Address
Offset337
ZWT
BALLDEFAULT
FUNCTIONSelect
BitAlternate
Function 1Select
BitAlternate
Function 2Select
BitAlternate
Function 3Select
BitAlternate
Function 4Select
BitAlternate
Function 5Select
Bit
0x110 N19 AD1EVT 0[0] MII_RX_ER 0[2] RMII_RX_ER 0[3] nTZ1_1 0[5]
D4 EMIF_ADDR[0] 0[8] N2HET2[1] 0[10]
D5 EMIF_ADDR[1] 0[16] N2HET2[3] 0[18]
C4 EMIF_ADDR[6] 0[24] RTP_DATA[13] 0[25] N2HET2[11] 0[26]
0x114 C5 EMIF_ADDR[7] 1[0] RTP_DATA[12] 1[1] N2HET2[13] 1[2]
C6 EMIF_ADDR[8] 1[8] RTP_DATA[11] 1[9] N2HET2[15] 1[10]
C7 EMIF_ADDR[9] 1[16] RTP_DATA[10] 1[17]
C8 EMIF_ADDR[10] 1[24] RTP_DATA[9] 1[25]
0x118 C9 EMIF_ADDR[11] 2[0] RTP_DATA[8] 2[1]
C10 EMIF_ADDR[12] 2[8] RTP_DATA[6] 2[9]
C11 EMIF_ADDR[13] 2[16] RTP_DATA[5] 2[17]
C12 EMIF_ADDR[14] 2[24] RTP_DATA[4] 2[25]
0x11C C13 EMIF_ADDR[15] 3[0] RTP_DATA[3] 3[1]
D14 EMIF_ADDR[16] 3[8] RTP_DATA[2] 3[9]
C14 EMIF_ADDR[17] 3[16] RTP_DATA[1] 3[17]
D15 EMIF_ADDR[18] 3[24] RTP_DATA[0] 3[25]
0x120 C15 EMIF_ADDR[19] 4[0] RTP_nENA 4[1]
C16 EMIF_ADDR[20] 4[8] RTP_nSYNC 4[9]
C17 EMIF_ADDR[21] 4[16] RTP_CLK 4[17]
0x124
-
0x12CReserved
0x130 PINMMR8[23:0] arereserved
D16 EMIF_BA[1] 8[24] 8[25] N2HET2[5] 8[26]
0x134 K3 RESERVED 9[0] EMIF_CLK 9[1] ECLK2 9[2]
R4 EMIF_nCAS 9[8] GIOB[3] 9[10]
N17 EMIF_nCS[0] 9[16] RTP_DATA[15] 9[17] N2HET2[7] 9[18]
L17 EMIF_nCS[2] 9[24] GIOB[4] 9[26]

<!-- Page 48 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435748TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-28. Output Multiplexing (continued)
Address
Offset337
ZWT
BALLDEFAULT
FUNCTIONSelect
BitAlternate
Function 1Select
BitAlternate
Function 2Select
BitAlternate
Function 3Select
BitAlternate
Function 4Select
BitAlternate
Function 5Select
Bit
0x138 K17 EMIF_nCS[3] 10[0] RTP_DATA[14] 10[1] N2HET2[9] 10[2]
M17 EMIF_nCSl[4] 10[8] RTP_DATA[7] 10[9] GIOB[5] 10[10]
R3 EMIF_nRAS 10[16] GIOB[6] 10[18]
P3 EMIF_nWAIT 10[24] GIOB[7] 10[26]
0x13C D17 EMIF_nWE 11[0] EMIF_RNW 11[1]
E9 ETMDATA[8] 11[8] EMIF_ADDR[5] 11[9]
E8 ETMDATA[9] 11[16] EMIF_ADDR[4] 11[17]
E7 ETMDATA[10] 11[24] EMIF_ADDR[3] 11[25]
0x140 E6 ETMDATA[11] 12[0] EMIF_ADDR[2] 12[1]
E13 ETMDATA[12] 12[8] EMIF_BA[0] 12[9]
E12 ETMDATA[13] 12[16] EMIF_nOE 12[17]
E11 ETMDATA[14] 12[24] EMIF_nDQM[1] 12[25]
0x144 E10 ETMDATA[15] 13[0] EMIF_nDQM[0] 13[1]
K15 ETMDATA[16] 13[8] EMIF_DATA[0] 13[9]
L15 ETMDATA[17] 13[16] EMIF_DATA[1] 13[17]
M15 ETMDATA[18] 13[24] EMIF_DATA[2] 13[25]
0x148 N15 ETMDATA[19] 14[0] EMIF_DATA[3] 14[1]
E5 ETMDATA[20] 14[8] EMIF_DATA[4] 14[9]
F5 ETMDATA[21] 14[16] EMIF_DATA[5] 14[17]
G5 ETMDATA[22] 14[24] EMIF_DATA[6] 14[25]
0x14C K5 ETMDATA[23] 15[0] EMIF_DATA[7] 15[1]
L5 ETMDATA[24] 15[8] EMIF_DATA[8] 15[9] N2HET2[24] 15[10] MIBSPI5NCS[4] 15[11]
M5 ETMDATA[25] 15[16] EMIF_DATA[9] 15[17] N2HET2[25] 15[18] MIBSPI5NCS[5] 15[19]
N5 ETMDATA[26] 15[24] EMIF_DATA[10] 15[25] N2HET2[26] 15[26]
0x150 P5 ETMDATA[27] 16[0] EMIF_DATA[11] 16[1] N2HET2[27] 16[2]
R5 ETMDATA[28] 16[8] EMIF_DATA[12] 16[9] N2HET2[28] 16[10] GIOA[0] 16[11]
R6 ETMDATA[29] 16[16] EMIF_DATA[13] 16[17] N2HET2[29] 16[18] GIOA[1] 16[19]
R7 ETMDATA[30] 16[24] EMIF_DATA[14] 16[25] N2HET2[30] 16[26] GIOA[3] 16[27]
0x154 R8 ETMDATA[31] 17[0] EMIF_DATA[15] 17[1] N2HET2[31] 17[2] GIOA[4] 17[3]
R9 ETMTRACECLKIN 17[8] EXTCLKIN2 17[9] GIOA[5] 17[11]
R10 ETMTRACECLKOUT 17[16] GIOA[6] 17[19]
R11 ETMTRACECTL 17[24] GIOA[7] 17[27]

<!-- Page 49 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435749TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-28. Output Multiplexing (continued)
Address
Offset337
ZWT
BALLDEFAULT
FUNCTIONSelect
BitAlternate
Function 1Select
BitAlternate
Function 2Select
BitAlternate
Function 3Select
BitAlternate
Function 4Select
BitAlternate
Function 5Select
Bit
0x158 B15 FRAYTX1 18[0] GIOA[2] 18[3]
B8 FRAYTX2 18[8] GIOB[0] 18[11]
B16 FRAYTXEN1 18[16] GIOB[1] 18[19]
B9 FRAYTXEN2 18[24] GIOB[2] 18[27]
0x15C C1 GIOA[2] 19[0] N2HET2[0] 19[2] eQEP2I 19[5]
E1 GIOA[3] 19[8] N2HET2[2] 19[10]
B5 GIOA[5] 19[16] EXTCLKIN1 19[19] ePWM1A 19[21]
H3 GIOA[6] 19[24] N2HET2[4] 19[26] ePWM1B 19[29]
0x160 M1 GIOA[7] 20[0] N2HET2[6] 20[2] ePWM2A 20[5]
F2 GIOB[2] 20[8] DCAN4TX 20[11]
W10 GIOB[3] 20[16] DCAN4RX 20[19]
J2 GIOB[6] 20[24] nERROR 20[25]
0x164 F1 GIOB[7] 21[0] RESERVED 21[1] nTZ1_2 21[5]
R2 MIBSPI1NCS[0] 21[8] MIBSPI1SOMI[1] 21[9] MII_TXD[2] 21[10] ECAP6 21[13]
F3 MIBSPI1NCS[1] 21[16] MII_COL 21[18] N2HET1[17] 21[19] eQEP1S 21[21]
G3 MIBSPI1NCS[2] 21[24] MDIO 21[26] N2HET1[19] 21[27]
0x168 J3 MIBSPI1NCS[3] 22[0] N2HET1[21] 22[3] nTZ1_3 22[5]
G19 MIBSPI1NENA 22[8] MII_RXD[2] 22[10] N2HET1[23] 22[11] ECAP4 22[13]
V9 MIBSPI3CLK 22[16] AD1EXT_SEL[1] 22[17] eQEP1A 22[21]
V10 MIBSPI3NCS[0] 22[24] AD2EVT 22[25] eQEP1I 22[29]
0x16C V5 MIBSPI3NCS[1] 23[0] MDCLK 23[2] N2HET1[25] 23[3]
B2 MIBSPI3NCS[2] 23[8] I2C1_SDA 23[9] N2HET1[27] 23[11] nTZ1_2 23[13]
C3 MIBSPI3NCS[3] 23[16] I2C1_SCL 23[17] N2HET1[29] 23[19] nTZ1_1 23[21]
W9 MIBSPI3NENA 23[24] MIBSPI3NCS[5] 23[25] N2HET1[31] 23[27] eQEP1B 23[29]
0x170 W8 MIBSPI3SIMO 24[0] AD1EXT_SEL[0] 24[1] ECAP3 24[5]
V8 MIBSPI3SOMI 24[8] AD1EXT_ENA 24[9] ECAP2 24[13]
H19 MIBSPI5CLK 24[16] DMM_DATA[4] 24[17] MII_TXEN 24[18] RMII_TXEN 24[19]
E19 MIBSPI5NCS[0] 24[24] DMM_DATA[5] 24[25] ePWM4A 24[29]
0x174 B6 MIBSPI5NCS[1] 25[0] DMM_DATA[6] 25[1]
W6 MIBSPI5NCS[2] 25[8] DMM_DATA[2] 25[9]
T12 MIBSPI5NCS[3] 25[16] DMM_DATA[3] 25[17]
H18 MIBSPI5NENA 25[24] DMM_DATA[7] 25[25] MII_RXD[3] 25[26] ECAP5 25[29]

<!-- Page 50 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435750TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-28. Output Multiplexing (continued)
Address
Offset337
ZWT
BALLDEFAULT
FUNCTIONSelect
BitAlternate
Function 1Select
BitAlternate
Function 2Select
BitAlternate
Function 3Select
BitAlternate
Function 4Select
BitAlternate
Function 5Select
Bit
0x178 J19 MIBSPI5SIMO[0] 26[0] DMM_DATA[8] 26[1] MII_TXD[1] 26[2] RMII_TXD[1] 26[3]
E16 MIBSPI5SIMO[1] 26[8] DMM_DATA[9] 26[9] AD1EXT_SEL[0] 26[12]
H17 MIBSPI5SIMO[2] 26[16] DMM_DATA[10] 26[17] AD1EXT_SEL[1] 26[20]
G17 MIBSPI5SIMO[3] 26[24] DMM_DATA[11] 26[25] I2C2_SDA 26[26] AD1EXT_SEL[2] 26[28]
0x17C J18 MIBSPI5SOMI[0] 27[0] DMM_DATA[12] 27[1] MII_TXD[0] 27[2] RMII_TXD[0] 27[3]
E17 MIBSPI5SOMI[1] 27[8] DMM_DATA[13] 27[9] AD1EXT_SEL[3] 27[12]
H16 MIBSPI5SOMI[2] 27[16] DMM_DATA[14] 27[17] AD1EXT_SEL[4] 27[20]
G16 MIBSPI5SOMI[3] 27[24] DMM_DATA[15] 27[25] I2C2_SCL 27[26] AD1EXT_ENA 27[28]
0x180 K18 N2HET1[0] 28[0] MIBSPI4CLK 28[1] ePWM2B 28[5]
V2 N2HET1[1] 28[8] MIBSPI4NENA 28[9] N2HET2[8] 28[11] eQEP2A 28[13]
W5 N2HET1[2] 28[16] MIBSPI4SIMO 28[17] ePWM3A 28[21]
U1 N2HET1[3] 28[24] MIBSPI4NCS[0] 28[25] N2HET2[10] 28[27] eQEP2B 28[29]
0x184 B12 N2HET1[4] 29[0] MIBSPI4NCS[1] 29[1] ePWM4B 29[5]
V6 N2HET1[5] 29[8] MIBSPI4SOMI 29[9] N2HET2[12] 29[11] ePWM3B 29[13]
W3 N2HET1[6] 29[16] SCI3RX 29[17] ePWM5A 29[21]
T1 N2HET1[7] 29[24] MIBSPI4NCS[2] 29[25] N2HET2[14] 29[27] ePWM7B 29[29]
0x188 E18 N2HET1[8] 30[0] MIBSPI1SIMO[1] 30[1] MII_TXD[3] 30[2]
V7 N2HET1[9] 30[8] MIBSPI4NCS[3] 30[9] N2HET2[16] 30[11] ePWM7A 30[13]
D19 N2HET1[10] 30[16] MIBSPI4NCS[4] 30[17] MII_TX_CLK 30[18] RESERVED 30[19] nTZ1_3 30[21]
E3 N2HET1[11] 30[24] MIBSPI3NCS[4] 30[25] N2HET2[18] 30[27] ePWM1SYNCO 30[29]
0x18C B4 N2HET1[12] 31[0] MIBSPI4NCS[5] 31[1] MII_CRS 31[2] RMII_CRS_DV 31[3]
N2 N2HET1[13] 31[8] SCI3TX 31[9] N2HET2[20] 31[11] ePWM5B 31[13]
N1 N2HET1[15] 31[16] MIBSPI1NCS[4] 31[17] N2HET2[22] 31[19] ECAP1 31[21]
A4 N2HET1[16] 31[24] ePWM1SYNCI 31[27] ePWM1SYNCO 31[29]
0x190 A13 N2HET1[17] 32[0] EMIF_nOE 32[1] SCI4RX 32[2]
J1 N2HET1[18] 32[8] EMIF_RNW 32[9] ePWM6A 32[13]
B13 N2HET1[19] 32[16] EMIF_nDQM[0] 32[17] SCI4TX 32[18]
P2 N2HET1[20] 32[24] EMIF_nDQM[1] 32[25] ePWM6B 32[29]
0x194 H4 N2HET1[21] 33[0] EMIF_nDQM[2] 33[1]
B3 N2HET1[22] 33[8] EMIF_nDQM[3] 33[9]
J4 N2HET1[23] 33[16] EMIF_BA[0] 33[17]
P1 N2HET1[24] 33[24] MIBSPI1NCS[5] 33[25] MII_RXD[0] 33[26] RMII_RXD[0] 33[27]

<!-- Page 51 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435751TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-28. Output Multiplexing (continued)
Address
Offset337
ZWT
BALLDEFAULT
FUNCTIONSelect
BitAlternate
Function 1Select
BitAlternate
Function 2Select
BitAlternate
Function 3Select
BitAlternate
Function 4Select
BitAlternate
Function 5Select
Bit
0x198 A14 N2HET1[26] 34[0] MII_RXD[1] 34[2] RMII_RXD[1] 34[3]
K19 N2HET1[28] 34[8] MII_RXCLK 34[10] RMII_REFCLK 34[11] RESERVED 34[12]
B11 N2HET1[30] 34[16] MII_RX_DV 34[18] eQEP2S 34[21]
D8 N2HET2[1] 34[24] N2HET1_NDIS 34[25]
0x19C D7 N2HET2[2] 35[0] N2HET2_NDIS 35[1]
D3 N2HET2[12] 35[8] MIBSPI2NENA 35[12] MIBSPI2NCS[1] 35[13]
D2 N2HET2[13] 35[16] MIBSPI2SOMI 35[20]
D1 N2HET2[14] 35[24] MIBSPI2SIMO 35[28]
0x1A0 P4 N2HET2[19] 36[0] LIN2RX 36[1]
T5 N2HET2[20] 36[8] LIN2TX 36[9]
T4 MII_RXCLK 36[16] RESERVED 36[20]
U7 MII_TX_CLK 36[24] RESERVED 36[28]
0x1A4 E2 N2HET2[3] 37[0] MIBSPI2CLK 37[4]
N3 N2HET2[7] 37[8] MIBSPI2NCS[0] 37[12]

<!-- Page 52 -->
52TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Terminal Configuration andFunctions Copyright ©2014 -2016, Texas Instruments Incorporated4.2.2.1.1 Notes onOutput Multiplexing
Table 4-28 lists theoutput signal multiplexing andcontrol signals forselecting thedesired functionality for
each pad.
*The pads default tothesignal defined bythe"Default Function" inTable 4-28 .
*The CTRL xcolumns inTable 4-28 contain avalue oftype x[y]which indicates thecontrol register PINMMRx, bit
y.Itindicates themultiplexing control register and thebitthat must besetinorder toselect thecorresponding
functionality tobeoutput onanyparticular pad.
-Forexample, consider themultiplexing onpinH3forthe337-ZWT package:
337
ZWT
BALLDEFAULT
FUNCTIONCTRL1 OPTION 2 CTRL2 OPTION 3 CTRL3 OPTION 4 CTRL4 OPTION 5 CTRL5 OPTION 6 CTRL6
H3 GIOA[6] 19[24] N2HET2[4] 19[26] ePWM1B 19[29]
-When GIOA[6] isconfigured asanoutput pinintheGIO module control register, then theprogrammed output
level appears onpinH3bydefault. The PINMMR19[24] issetbydefault toindicate thattheGIOA[6] signal is
selected tobeoutput.
-Ifthe application must output the N2HET2[4] signal onpinH3, itmust clear PINMMR19[24] and set
PINMMR19[26].
-Note that thepinisconnected asinput toboth theGIO and N2HET2 modules. That is,there isnoinput
multiplexing onthispin.
*The base address oftheIOMM module starts at0xFFFF_1C00. The Output mux control registers with thefirst
register PINMMR0 starts attheoffset address 0x110 within theIOMM module.