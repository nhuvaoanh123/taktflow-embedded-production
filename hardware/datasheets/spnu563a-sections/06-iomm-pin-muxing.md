# I/O Multiplexing and Control Module (IOMM)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 301-337

---


<!-- Page 301 -->

301 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Chapter 6
SPNU563A -March 2018
I/OMultiplexing andControl Module (IOMM)
This chapter describes theI/OMultiplexing andControl Module (IOMM).
Topic ........................................................................................................................... Page
6.1 Overview ........................................................................................................ 302
6.2 Main Features ofI/OMultiplexing Module (IOMM) ................................................. 302
6.3 Control ofMultiplexed Outputs .......................................................................... 302
6.4 Control ofMultiplexed Inputs ............................................................................. 303
6.5 Control ofSpecial Multiplexed Options ............................................................... 312
6.6 Safety Features ................................................................................................ 327
6.7 IOMM Registers ................................................................................................ 328

<!-- Page 302 -->

Overview www.ti.com
302 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.1 Overview
This chapter describes theoverall features ofthemodule thatcontrol theI/Omultiplexing onthedevice.
The mapping ofcontrol registers tomultiplexing options isspecified inSection 6.7.13 .
6.2 Main Features ofI/OMultiplexing Module (IOMM)
The IOMM contains memory-mapped registers (MMR) thatcontrol device-specific multiplexed functions.
The safety anddiagnostic features oftheIOMM are:
*Kicker mechanism toprotect theMMRs from accidental writes
*Error indication foraccess violations
6.3 Control ofMultiplexed Outputs
The signal multiplexing controlled byeach memory-mapped control register (PINMMRn) isdescribed in
Table 6-1.Each byte inthePINMMRs control thefunctionality output onasingle terminal. Consider the
following example forthePINMMR9 control register.
Figure 6-1.PINMMR9 Control Register [Address Offset =134h]
31 27 26 25 24
Reserved GIOB[4] Reserved EMIF_nCS[2]
R/WP-0 R/WP-0 R/WP-0 R/WP-1
23 19 18 17 16
Reserved N2HET2[7] RTP_DATA[15] EMIF nCS[0]
R/WP-0 R/WP-0 R/WP-0 R/WP-1
15 11 10 9 8
Reserved GIOB[3] Reserved EMIF_nCAS
R/WP-0 R/WP-0 R/WP-0 R/WP-1
7 3 2 1 0
Reserved ECLK2 EMIF_CLK Reserved
R/WP-0 R/WP-0 R/WP-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
*Consider themultiplexing controlled byPINMMR9[23 -16].These bitscontrol themultiplexing between
theEMIF_nCS[0], RTP_DATA[15] andN2HET2[7] ontheballN17 ofthe337BGA package forthis
device. The default function ontheN17 ballisEMIF_nCS[0]. This isdictated bybit16ofthePINMMR9
register being set.
*Iftheapplication wants touseN17 asanN2HET2[7] signal, then bit16ofPINMMR9 must becleared
andbit18must beset.Likewise, ifRTP_DATA[15] istobebrought out,then bit16ofPINMMR9 must
becleared andbit17must beset.
*Each feature oftheoutput function isdetermined bythefunction selected tobeoutput onaterminal.
Forexample, theballN17 onthe337BGA package isdriven byanoutput buffer with an8mA drive
strength. This output buffer hasthefollowing signals: A(signal tobeoutput) andGZ(output enable).
Each ofthese signals isanoutput ofamultiplexor thatallows theselected function tocontrol all
available features oftheoutput buffer. Some output buffers may have additional options asoutput
strength, slew rate, andsoon.This options arealso controlled bythemultiplexor output.
*The PINMMR control registers areused toimplement aone-hot encoding scheme forselecting the
multiplexed function.
-Forexample, fortheN17 ballonthe337BGA package forthisdevice only oneoutofbit16,17or
bit18must beset.
-Iftheapplication clears bits16,17and18,then thedefault function, EMIF_nCS[0], willbeselected
foroutput onN17.
-Iftheapplication sets 16,17and18,then thedefault function willbeselected foroutput onN17.

<!-- Page 303 -->

EMIF_nCS[0]_nEN
RTP_DATA[15]_nEN
N2HET2[7]_nEN
EMIF_nCS[0]_OUT
RTP_DATA[15]_OUT
N2HET2[7]_OUTGZ
A YPINMMR9[16,17,18]
pad
www.ti.com Control ofMultiplexed Inputs
303 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)-Iftheapplication sets oneormore reserved bit(s) within thebyte 23-16,then thedefault function
willbeselected foroutput onN17.
Figure 6-2shows themultiplexing between theoutput functions fortheN17 ball. This terminal uses an
8mA output buffer.
Figure 6-2.Output Multiplexing Example
InTable 6-1,thecolumn "Selection Bit"contains avalue oftype x[y]thatcorresponds tothecontrol
register PINMMRx, bity.Itindicates themultiplexing control register andthebitthatmust besetinorder
toselect thecorresponding functionality tobebrought outtotheterminal. Ifanun-implemented alternate
function isselected where aphysical pinisattached, thedefault function isused. When aPINMMRx
register iscompletely reserved, none ofits8-bit fields areattached toanyphysical pin.
6.4 Control ofMultiplexed Inputs
Inthismicrocontroller, some signals areconnected tomore than oneterminal, sothattheinputs forthese
signals cancome from either ofthese terminals. Amultiplexor isimplemented tolettheapplication choose
theterminal thatwillbeused forproviding theinput signal from among theavailable options. The input
path selection isdone based ontwobitsinthePINMMR control register asshown inTable 6-2.
*The input toamodule comes from theDefault Terminal when theassociated bitintheTerminal 1
Input Multiplex Control column issetandthebitintheTerminal 2Input Multiplex Control column
isclear. Bydefault, thebitintheTerminal 1Input Multiplex Control column issetafter reset.
*The input toamodule comes from theAlternate Terminal when theassociated bitintheTerminal 2
Input Multiplex Control column issetandthebitintheTerminal 1Input Multiplex Control column
isclear.
NOTE: Ifmultiple bitsornobitareselected intheInput Multiplex Control, theDefault Function will
then beselected.
Some signals, likeeCAPx andeQEPx, arebydefault mapped toanunavailable ballonthe
337ZWT package. The alternate terminals have tobeused, inthiscase, inorder touse
these signals.

<!-- Page 304 -->

Control ofMultiplexed Inputs www.ti.com
304 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-1.Multiplexing forOutputs on337ZWT Package
Address
Offset337ZWT
BALLDefault Function Selection
BitAlternate Function 1Selection
BitAlternate Function 2Selection
BitAlternate Function 3Selection
BitAlternate Function 4Selection
BitAlternate Function 5Selection
Bit
110h N19 AD1EVT 0[0] MII_RX_ER 0[2] RMII_RX_ER 0[3] nTZ1_1 0[5]
D4 EMIF_ADDR[00] 0[8] N2HET2[01] 0[10]
D5 EMIF_ADDR[01] 0[16] N2HET2[03] 0[18]
C4 EMIF_ADDR[06] 0[24] RTP_DATA[13] 0[25] N2HET2[11] 0[26]
114h C5 EMIF_ADDR[07] 1[0] RTP_DATA[12] 1[1] N2HET2[13] 1[2]
C6 EMIF_ADDR[08] 1[8] RTP_DATA[11] 1[9] N2HET2[15] 1[10]
C7 EMIF_ADDR[09] 1[16] RTP_DATA[10] 1[17]
C8 EMIF_ADDR[10] 1[24] RTP_DATA[09] 1[25]
118h C9 EMIF_ADDR[11] 2[0] RTP_DATA[08] 2[1]
C10 EMIF_ADDR[12] 2[8] RTP_DATA[06] 2[9]
C11 EMIF_ADDR[13] 2[16] RTP_DATA[05] 2[17]
C12 EMIF_ADDR[14] 2[24] RTP_DATA[04] 2[25]
11Ch C13 EMIF_ADDR[15] 3[0] RTP_DATA[03] 3[1]
D14 EMIF_ADDR[16] 3[8] RTP_DATA[02] 3[9]
C14 EMIF_ADDR[17] 3[16] RTP_DATA[01] 3[17]
D15 EMIF_ADDR[18] 3[24] RTP_DATA[00] 3[25]
120h C15 EMIF_ADDR[19] 4[0] RTP_nENA 4[1]
C16 EMIF_ADDR[20] 4[8] RTP_nSYNC 4[9]
C17 EMIF_ADDR[21] 4[16] RTP_CLK 4[17]
124h-12Ch Reserved
130h PINMMR8[23:0] arereserved
D16 EMIF_BA[1] 8[24] 8[25] N2HET2[05] 8[26]
134h K3 RESERVED 9[0] EMIF_CLK 9[1] ECLK2 9[2]
R4 EMIF_nCAS 9[8] GIOB[3] 9[10]
N17 EMIF_nCS[0] 9[16] RTP_DATA[15] 9[17] N2HET2[07] 9[18]
L17 EMIF_nCS[2] 9[24] GIOB[4] 9[26]
138h K17 EMIF_nCS[3] 10[0] RTP_DATA[14] 10[1] N2HET2[09] 10[2]
M17 EMIF_nCS[4] 10[8] RTP_DATA[07] 10[9] GIOB[5] 10[10]
R3 EMIF_nRAS 10[16] GIOB[6] 10[18]
P3 EMIF_nWAIT 10[24] GIOB[7] 10[26]
13Ch D17 EMIF_nWE 11[0] EMIF_RNW 11[1]
E9 ETMDATA[08] 11[8] EMIF_ADDR[05] 11[9]
E8 ETMDATA[09] 11[16] EMIF_ADDR[04] 11[17]
E7 ETMDATA[10] 11[24] EMIF_ADDR[03] 11[25]
140h E6 ETMDATA[11] 12[0] EMIF_ADDR[02] 12[1]
E13 ETMDATA[12] 12[8] EMIF_BA[0] 12[9]
E12 ETMDATA[13] 12[16] EMIF_nOE 12[17]
E11 ETMDATA[14] 12[24] EMIF_nDQM[1] 12[25]

<!-- Page 305 -->

www.ti.com Control ofMultiplexed Inputs
305 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-1.Multiplexing forOutputs on337ZWT Package (continued)
Address
Offset337ZWT
BALLDefault Function Selection
BitAlternate Function 1Selection
BitAlternate Function 2Selection
BitAlternate Function 3Selection
BitAlternate Function 4Selection
BitAlternate Function 5Selection
Bit
144h E10 ETMDATA[15] 13[0] EMIF_nDQM[0] 13[1]
K15 ETMDATA[16] 13[8] EMIF_DATA[00] 13[9]
L15 ETMDATA[17] 13[16] EMIF_DATA[01] 13[17]
M15 ETMDATA[18] 13[24] EMIF_DATA[02] 13[25]
148h N15 ETMDATA[19] 14[0] EMIF_DATA[03] 14[1]
E5 ETMDATA[20] 14[8] EMIF_DATA[04] 14[9]
F5 ETMDATA[21] 14[16] EMIF_DATA[05] 14[17]
G5 ETMDATA[22] 14[24] EMIF_DATA[06] 14[25]
14Ch K5 ETMDATA[23] 15[0] EMIF_DATA[07] 15[1]
L5 ETMDATA[24] 15[8] EMIF_DATA[08] 15[9] N2HET2[24] 15[10] MIBSPI5NCS[4] 15[11]
M5 ETMDATA[25] 15[16] EMIF_DATA[09] 15[17] N2HET2[25] 15[18] MIBSPI5NCS[5] 15[19]
N5 ETMDATA[26] 15[24] EMIF_DATA[10] 15[25] N2HET2[26] 15[26]
150h P5 ETMDATA[27] 16[0] EMIF_DATA[11] 16[1] N2HET2[27] 16[2]
R5 ETMDATA[28] 16[8] EMIF_DATA[12] 16[9] N2HET2[28] 16[10] GIOA[0] 16[11]
R6 ETMDATA[29] 16[16] EMIF_DATA[13] 16[17] N2HET2[29] 16[18] GIOA[1] 16[19]
R7 ETMDATA[30] 16[24] EMIF_DATA[14] 16[25] N2HET2[30] 16[26] GIOA[3] 16[27]
154h R8 ETMDATA[31] 17[0] EMIF_DATA[15] 17[1] N2HET2[31] 17[2] GIOA[4] 17[3]
R9 ETMTRACECLKIN 17[8] EXTCLKIN2 17[9] GIOA[5] 17[11]
R10 ETMTRACECLKOUT 17[16] GIOA[6] 17[19]
R11 ETMTRACECTL 17[24] GIOA[7] 17[27]
158h B15 FRAYTX1 18[0] GIOA[2] 18[3]
B8 FRAYTX2 18[8] GIOB[0] 18[11]
B16 FRAYTXEN1 18[16] GIOB[1] 18[19]
B9 FRAYTXEN2 18[24] GIOB[2] 18[27]
15Ch C1 GIOA[2] 19[0] N2HET2[00] 19[2] eQEP2I 19[5]
E1 GIOA[3] 19[8] N2HET2[02] 19[10]
B5 GIOA[5] 19[16] EXTCLKIN 19[19] ePWM1A 19[21]
H3 GIOA[6] 19[24] N2HET2[04] 19[26] ePWM1B 19[29]
160h M1 GIOA[7] 20[0] N2HET2[06] 20[2] ePWM2A 20[5]
F2 GIOB[2] 20[8] DCAN4TX 20[11]
W10 GIOB[3] 20[16] DCAN4RX 20[19]
J2 GIOB[6] 20[24] nERROR1 20[25]
164h F1 GIOB[7] 21[0] nERROR2 21[1] nTZ1_2 21[5]
R2 MIBSPI1NCS[0] 21[8] MIBSPI1SOMI[1] 21[9] MII_TXD[2] 21[10] ECAP6 21[13]
F3 MIBSPI1NCS[1] 21[16] MII_COL 21[18] N2HET1[17] 21[19] eQEP1S 21[21]
G3 MIBSPI1NCS[2] 21[24] MDIO 21[26] N2HET1[19] 21[27]

<!-- Page 306 -->

Control ofMultiplexed Inputs www.ti.com
306 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-1.Multiplexing forOutputs on337ZWT Package (continued)
Address
Offset337ZWT
BALLDefault Function Selection
BitAlternate Function 1Selection
BitAlternate Function 2Selection
BitAlternate Function 3Selection
BitAlternate Function 4Selection
BitAlternate Function 5Selection
Bit
168h J3 MIBSPI1NCS[3] 22[0] N2HET1[21] 22[3] nTZ1_3 22[5]
G19 MIBSPI1NENA 22[8] MII_RXD[2] 22[10] N2HET1[23] 22[11] ECAP4 22[13]
V9 MIBSPI3CLK 22[16] EXT_SEL[01] 22[17] eQEP1A 22[21]
V10 MIBSPI3NCS[0] 22[24] AD2EVT 22[25] eQEP1I 22[29]
16Ch V5 MIBSPI3NCS[1] 23[0] MDCLK 23[2] N2HET1[25] 23[3]
B2 MIBSPI3NCS[2] 23[8] I2C1_SDA 23[9] N2HET1[27] 23[11] nTZ1_2 23[13]
C3 MIBSPI3NCS[3] 23[16] I2C1_SCL 23[17] N2HET1[29] 23[19] nTZ1_1 23[21]
W9 MIBSPI3NENA 23[24] MIBSPI3NCS[5] 23[25] N2HET1[31] 23[27] eQEP1B 23[29]
170h W8 MIBSPI3SIMO 24[0] EXT_SEL[00] 24[1] ECAP3 24[5]
V8 MIBSPI3SOMI 24[8] EXT_ENA 24[9] ECAP2 24[13]
H19 MIBSPI5CLK 24[16] DMM_DATA[04] 24[17] MII_TXEN 24[18] RMII_TXEN 24[19]
E19 MIBSPI5NCS[0] 24[24] DMM_DATA[05] 24[25] ePWM4A 24[29]
174h B6 MIBSPI5NCS[1] 25[0] DMM_DATA[06] 25[1]
W6 MIBSPI5NCS[2] 25[8] DMM_DATA[02] 25[9]
T12 MIBSPI5NCS[3] 25[16] DMM_DATA[03] 25[17]
H18 MIBSPI5NENA 25[24] DMM_DATA[07] 25[25] MII_RXD[3] 25[26] ECAP5 25[29]
178h J19 MIBSPI5SIMO[0] 26[0] DMM_DATA[08] 26[1] MII_TXD[1] 26[2] RMII_TXD[1] 26[3]
E16 MIBSPI5SIMO[1] 26[8] DMM_DATA[09] 26[9] EXT_SEL[00] 26[12]
H17 MIBSPI5SIMO[2] 26[16] DMM_DATA[10] 26[17] EXT_SEL[01] 26[20]
G17 MIBSPI5SIMO[3] 26[24] DMM_DATA[11] 26[25] I2C2_SDA 26[26] EXT_SEL[02] 26[28]
17Ch J18 MIBSPI5SOMI[0] 27[0] DMM_DATA[12] 27[1] MII_TXD[0] 27[2] RMII_TXD[0] 27[3]
E17 MIBSPI5SOMI[1] 27[8] DMM_DATA[13] 27[9] EXT_SEL[03] 27[12]
H16 MIBSPI5SOMI[2] 27[16] DMM_DATA[14] 27[17] EXT_SEL[04] 27[20]
G16 MIBSPI5SOMI[3] 27[24] DMM_DATA[15] 27[25] I2C2_SCL 27[26] EXT_ENA 27[28]
180h K18 N2HET1[00] 28[0] MIBSPI4CLK 28[1] ePWM2B 28[5]
V2 N2HET1[01] 28[8] MIBSPI4NENA 28[9] N2HET2[08] 28[11] eQEP2A 28[13]
W5 N2HET1[02] 28[16] MIBSPI4SIMO 28[17] ePWM3A 28[21]
U1 N2HET1[03] 28[24] MIBSPI4NCS[0] 28[25] N2HET2[10] 28[27] eQEP2B 28[29]
184h B12 N2HET1[04] 29[0] MIBSPI4NCS[1] 29[1] ePWM4B 29[5]
V6 N2HET1[05] 29[8] MIBSPI4SOMI 29[9] N2HET2[12] 29[11] ePWM3B 29[13]
W3 N2HET1[06] 29[16] SCI3RX 29[17] ePWM5A 29[21]
T1 N2HET1[07] 29[24] MIBSPI4NCS[2] 29[25] N2HET2[14] 29[27] ePWM7B 29[29]
188h E18 N2HET1[08] 30[0] MIBSPI1SIMO[1] 30[1] MII_TXD[3] 30[2]
V7 N2HET1[09] 30[8] MIBSPI4NCS[3] 30[9] N2HET2[16] 30[11] ePWM7A 30[13]
D19 N2HET1[10] 30[16] MIBSPI4NCS[4] 30[17] MII_TX_CLK 30[18] nTZ1_3 30[21]
E3 N2HET1[11] 30[24] MIBSPI3NCS[4] 30[25] N2HET2[18] 30[27] EPWM1SYNCO 30[29]

<!-- Page 307 -->

www.ti.com Control ofMultiplexed Inputs
307 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-1.Multiplexing forOutputs on337ZWT Package (continued)
Address
Offset337ZWT
BALLDefault Function Selection
BitAlternate Function 1Selection
BitAlternate Function 2Selection
BitAlternate Function 3Selection
BitAlternate Function 4Selection
BitAlternate Function 5Selection
Bit
(1)Selecting N2HET1_NDIS orN2HET2_NDIS forces thepintoahigh-impedance state andchanges thepulltype topullup.18Ch B4 N2HET1[12] 31[0] MIBSPI4NCS[5] 31[1] MII_CRS 31[2] RMII_CRS_DV 31[3]
N2 N2HET1[13] 31[8] SCI3TX 31[9] N2HET2[20] 31[11] ePWM5B 31[13]
N1 N2HET1[15] 31[16] MIBSPI1NCS[4] 31[17] N2HET2[22] 31[19] ECAP1 31[21]
A4 N2HET1[16] 31[24] EPWM1SYNCI 31[27] EPWM1SYNCO 31[29]
190h A13 N2HET1[17] 32[0] EMIF_nOE 32[1] SCI4RX 32[2]
J1 N2HET1[18] 32[8] EMIF_RNW 32[9] ePWM6A 32[13]
B13 N2HET1[19] 32[16] EMIF_nDQM[0] 32[17] SCI4TX 32[18]
P2 N2HET1[20] 32[24] EMIF_nDQM[1] 32[25] ePWM6B 32[29]
194h H4 N2HET1[21] 33[0] EMIF_nDQM[2] 33[1]
B3 N2HET1[22] 33[8] EMIF_nDQM[3] 33[9]
J4 N2HET1[23] 33[16] EMIF_BA[0] 33[17]
P1 N2HET1[24] 33[24] MIBSPI1NCS[5] 33[25] MII_RXD[0] 33[26] RMII_RXD[0] 33[27]
198h A14 N2HET1[26] 34[0] MII_RXD[1] 34[2] RMII_RXD[1] 34[3]
K19 N2HET1[28] 34[8] MII_RXCLK 34[10] RMII_REFCLK 34[11]
B11 N2HET1[30] 34[16] MII_RX_DV 34[18] eQEP2S 34[21]
D8 N2HET2[01] 34[24] N2HET1_NDIS(1)34[25]
19Ch D7 N2HET2[02] 35[0] N2HET2_NDIS(1)35[1]
D3 N2HET2[12] 35[8] MIBSPI2NENA 35[12] MIBSPI2NCS[1] 35[13]
D2 N2HET2[13] 35[16] MIBSPI2SOMI 35[20]
D1 N2HET2[14] 35[24] MIBSPI2SIMO 35[28]
1A0h P4 N2HET2[19] 36[0] LIN2RX 36[1]
T5 N2HET2[20] 36[8] LIN2TX 36[9]
T4 MII_RXCLK 36[16]
U7 MII_TX_CLK 36[24]
1A4h E2 N2HET2[03] 37[0] MIBSPI2CLK 37[4]
N3 N2HET2[07] 37[8] MIBSPI2NCS[0] 37[12]

<!-- Page 308 -->

Control ofMultiplexed Inputs www.ti.com
308 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)

<!-- Page 309 -->

www.ti.com Control ofMultiplexed Inputs
309 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-2.Input Multiplexing andControl on337ZWT Package
Address
OffsetSignal Name Default Terminal Terminal 1Input
Multiplex ControlAlternate Terminal Terminal 2Input
Multiplex Control
250h AD2EVT T10 PINMMR80[0] V10 PINMMR80[1]
ECAP1 N/Aon337ZWT PINMMR80[8] N1 PINMMR80[9]
ECAP2 N/Aon337ZWT PINMMR80[16] V8 PINMMR80[17]
ECAP3 N/Aon337ZWT PINMMR80[24] W8 PINMMR80[25]
254h ECAP4 N/Aon337ZWT PINMMR81[0] G19 PINMMR81[1]
ECAP5 N/Aon337ZWT PINMMR81[8] H18 PINMMR81[9]
ECAP6 N/Aon337ZWT PINMMR81[16] R2 PINMMR81[17]
eQEP1A N/Aon337ZWT PINMMR81[24] V9 PINMMR81[25]
258h eQEP1B N/Aon337ZWT PINMMR82[0] W9 PINMMR82[1]
eQEP1I N/Aon337ZWT PINMMR82[8] V10 PINMMR82[9]
eQEP1S N/Aon337ZWT PINMMR82[16] F3 PINMMR82[17]
eQEP2A N/Aon337ZWT PINMMR82[24] V2 PINMMR82[25]
25Ch eQEP2B N/Aon337ZWT PINMMR83[0] U1 PINMMR83[1]
eQEP2I N/Aon337ZWT PINMMR83[8] C1 PINMMR83[9]
eQEP2S N/Aon337ZWT PINMMR83[16] B11 PINMMR83[17]
GIOA[0] A5 PINMMR83[24] R5 PINMMR83[25]
260h GIOA[1] C2 PINMMR84[0] R6 PINMMR84[1]
GIOA[2] C1 PINMMR84[8] B15 PINMMR84[9]
GIOA[3] E1 PINMMR84[16] R7 PINMMR84[17]
GIOA[4] A6 PINMMR84[24] R8 PINMMR84[25]
264h GIOA[5] B5 PINMMR85[0] R9 PINMMR85[1]
GIOA[6] H3 PINMMR85[8] R10 PINMMR85[9]
GIOA[7] M1 PINMMR85[16] R11 PINMMR85[17]
GIOB[0] M2 PINMMR85[24] B8 PINMMR85[25]
268h GIOB[1] K2 PINMMR86[0] B16 PINMMR86[1]
GIOB[2] F2 PINMMR86[8] B9 PINMMR86[9]
GIOB[3] W10 PINMMR86[16] R4 PINMMR86[17]
GIOB[4] G1 PINMMR86[24] L17 PINMMR86[25]
26Ch GIOB[5] G2 PINMMR87[0] M17 PINMMR87[1]
GIOB[6] J2 PINMMR87[8] R3 PINMMR87[9]
GIOB[7] F1 PINMMR87[16] P3 PINMMR87[17]
MDIO F4 PINMMR87[24] G3 PINMMR87[25]
270h MIBSPI1NCS[4] U10 PINMMR88[0] N1 PINMMR88[1]
MIBSPI1NCS[5] U9 PINMMR88[8] P1 PINMMR88[9]
MIBSPI2NCS[1] N/Aon337ZWT PINMMR88[16] D3 PINMMR88[17]
274h MII_COL W4 PINMMR89[16] F3 PINMMR89[17]
MII_CRS V4 PINMMR89[24] B4 PINMMR89[25]
278h MII_RX_DV U6 PINMMR90[0] B11 PINMMR90[1]
MII_RX_ER U5 PINMMR90[8] N19 PINMMR90[9]
MII_RXCLK T4 PINMMR90[16] K19 PINMMR90[17]
MII_RXD[0] U4 PINMMR90[24] P1 PINMMR90[25]
27Ch MII_RXD[1] T3 PINMMR91[0] A14 PINMMR91[1]
MII_RXD[2] U3 PINMMR91[8] G19 PINMMR91[9]
MII_RXD[3] V3 PINMMR91[16] H18 PINMMR91[17]
MII_TX_CLK U7 PINMMR91[24] D19 PINMMR91[25]

<!-- Page 310 -->

Control ofMultiplexed Inputs www.ti.com
310 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-2.Input Multiplexing andControl on337ZWT Package (continued)
Address
OffsetSignal Name Default Terminal Terminal 1Input
Multiplex ControlAlternate Terminal Terminal 2Input
Multiplex Control
280h N2HET1[17] A13 PINMMR92[0] F3 PINMMR92[1]
N2HET1[19] B13 PINMMR92[8] G3 PINMMR92[9]
N2HET1[21] H4 PINMMR92[16] J3 PINMMR92[17]
N2HET1[23] J4 PINMMR92[24] G19 PINMMR92[25]
284h N2HET1[25] M3 PINMMR93[0] V5 PINMMR93[1]
N2HET1[27] A9 PINMMR93[8] B2 PINMMR93[9]
N2HET1[29] A3 PINMMR93[16] C3 PINMMR93[17]
N2HET1[31] J17 PINMMR93[24] W9 PINMMR93[25]
288h N2HET2[00] D6 PINMMR94[0] C1 PINMMR94[1]
N2HET2[01] D8 PINMMR94[8] D4 PINMMR94[9]
N2HET2[02] D7 PINMMR94[16] E1 PINMMR94[17]
N2HET2[03] E2 PINMMR94[24] D5 PINMMR94[25]
28Ch N2HET2[04] D13 PINMMR95[0] H3 PINMMR95[1]
N2HET2[05] D12 PINMMR95[8] D16 PINMMR95[9]
N2HET2[06] D11 PINMMR95[16] M1 PINMMR95[17]
N2HET2[07] N3 PINMMR95[24] N17 PINMMR95[25]
290h N2HET2[08] K16 PINMMR96[0] V2 PINMMR96[1]
N2HET2[09] L16 PINMMR96[8] K17 PINMMR96[9]
N2HET2[10] M16 PINMMR96[16] U1 PINMMR96[17]
N2HET2[11] N16 PINMMR96[24] C4 PINMMR96[25]
294h N2HET2[12] D3 PINMMR97[0] V6 PINMMR97[1]
N2HET2[13] D2 PINMMR97[8] C5 PINMMR97[9]
N2HET2[14] D1 PINMMR97[16] T1 PINMMR97[17]
N2HET2[15] K4 PINMMR97[24] C6 PINMMR97[25]
298h N2HET2[16] L4 PINMMR98[0] V7 PINMMR98[1]
N2HET2[18] N4 PINMMR98[8] E3 PINMMR98[9]
N2HET2[20] T5 PINMMR98[16] N2 PINMMR98[17]
N2HET2[22] T7 PINMMR98[24] N1 PINMMR98[25]
29Ch nTZ1_1 N19 PINMMR99[0] C3 PINMMR99[1]
nTZ1_2 F1 PINMMR99[8] B2 PINMMR99[9]
nTZ1_3 J3 PINMMR99[16] D19 PINMMR99[17]

<!-- Page 311 -->

N2HET1N2HET1[17] /EMIF_nOE/SCI4RX
MIBSPI1NCS[1]/MII_COL/ N2HET1[17] /eQEP1SNot (PINMMR92[0]=0 and PINMMR92[1]=1)
1
0N2HET1[17]_IN
MIBSPI1
Ethernet
eQEP1EMIFSCI4
www.ti.com Control ofMultiplexed Inputs
311 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)NOTE: Inputs arebroadcast toallmultiplexed functions
The input signals arebroadcast toallmodules hooked uptoaterminal. The application must
ensure thatmodules thatarenotbeing used intheapplication donotreact toachange on
their input functions. Forexample, aGIO signal toggle cantrigger aninterrupt request, when
theapplication actually isusing thefunction multiplexed with thisGIO signal.
Figure 6-3.Input Multiplexing Example

<!-- Page 312 -->

Control ofSpecial Multiplexed Options www.ti.com
312 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5 Control ofSpecial Multiplexed Options
Several ofthePINMMR registers areused tocontrol specific functions onthismicrocontroller.
Table 6-3.Special Multiplexed Controls
Control FunctionPINMMR
Register
Address
OffsetPINMMR Register Bits Used Reference
GIOB[2] Select 390h PINMMR160[16] PINMMR160[17] See Section 6.5.6
MII/RMII Select PINMMR160[24] See Section 6.5.3
ADC Alternate Trigger Table Select 394h PINMMR161[0] PINMMR161[1] See Section 6.5.4
ADC1 Alternate Trigger Source for
Trigger Input 2PINMMR161[8] PINMMR161[9]
ADC1 Alternate Trigger Source for
Trigger Input 4PINMMR161[16] PINMMR161[17]
ADC1 Alternate Trigger Source for
Trigger Input 6PINMMR161[24] PINMMR161[25]
ADC1 Alternate Trigger Source for
Trigger Input 7398h PINMMR162[0] PINMMR162[1]
ADC1 Alternate Trigger Source for
Trigger Input 8PINMMR162[8] PINMMR162[9]
ADC2 Alternate Trigger Source for
Trigger Input 2PINMMR162[16] PINMMR162[17]
ADC2 Alternate Trigger Source for
Trigger Input 4PINMMR162[24] PINMMR162[25]
ADC2 Alternate Trigger Source for
Trigger Input 639Ch PINMMR163[0] PINMMR163[1]
ADC2 Alternate Trigger Source for
Trigger Input 7PINMMR163[8] PINMMR163[9]
ADC2 Alternate Trigger Source for
Trigger Input 8PINMMR163[16] PINMMR163[17]
Selecting Start ofConversion
(SOC1A) ofePWM13A0h PINMMR164[0] See Section 6.5.5
Selecting Start ofConversion
(SOC2A) ofePWM2PINMMR164[8]
Selecting Start ofConversion
(SOC3A) ofePWM3PINMMR164[16]
Selecting Start ofConversion
(SOC4A) ofePWM4PINMMR164[24]
Selecting Start ofConversion
(SOC5A) ofePWM53A4h PINMMR165[0]
Selecting Start ofConversion
(SOC6A) ofePWM6PINMMR165[8]
Selecting Start ofConversion
(SOC7A) ofePWM7PINMMR165[16]
ePWM1 SYNCI Select PINMMR165[24] PINMMR165[25] See Section 6.5.8
ePWMx TBCLKSYNC Enable 3A8h PINMMR166[1] See Section 6.5.7
ePWM1 Trip Zone 4Select 3ACh PINMMR167[0] PINMMR167[1] PINMMR167[2] See Section 6.5.9
ePWM2 Trip Zone 4Select PINMMR167[8] PINMMR167[9] PINMMR167[10]
ePWM3 Trip Zone 4Select PINMMR167[16] PINMMR167[17] PINMMR167[18]
ePWM4 Trip Zone 4Select PINMMR167[24] PINMMR167[25] PINMMR167[26]
ePWM5 Trip Zone 4Select 3B0h PINMMR168[0] PINMMR168[1] PINMMR168[2]
ePWM6 Trip Zone 4Select PINMMR168[8] PINMMR168[9] PINMMR168[10]
ePWM7 Trip Zone 4Select PINMMR168[16] PINMMR168[17] PINMMR168[18]

<!-- Page 313 -->

www.ti.com Control ofSpecial Multiplexed Options
313 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-3.Special Multiplexed Controls (continued)
Control FunctionPINMMR
Register
Address
OffsetPINMMR Register Bits Used Reference
eCAP1 Input Filtering Select 3B4h PINMMR169[0] PINMMR169[1] See Section 6.5.10
eCAP2 Input Filtering Select PINMMR169[8] PINMMR169[9]
eCAP3 Input Filtering Select PINMMR169[16] PINMMR169[17]
eCAP4 Input Filtering Select PINMMR169[24] PINMMR169[25]
eCAP5 Input Filtering Select 3B8h PINMMR170[0] PINMMR170[1]
eCAP6 Input Filtering Select PINMMR170[8] PINMMR170[9]
eQEP1A Input Filtering Select PINMMR170[16] PINMMR170[17] See Section 6.5.11
eQEP1B Input Filtering Select PINMMR170[24] PINMMR170[25]
eQEP1I Input Filtering Select 3BCh PINMMR171[0] PINMMR171[1]
eQEP1S Input Filtering Select PINMMR171[8] PINMMR171[9]
eQEP2A Input Filtering Select PINMMR171[16] PINMMR171[17]
eQEP2B Input Filtering Select PINMMR171[24] PINMMR171[25]
eQEP2I Input Filtering Select 3C0h PINMMR172[0] PINMMR172[1]
eQEP2S Input Filtering Select PINMMR172[8] PINMMR172[9]
ePWMx Trip Zone1 (TZ1n) Input
Filtering SelectPINMMR172[16] PINMMR172[17] PINMMR172[18] See Section 6.5.9
ePWMx Trip Zone2 (TZ2n) Input
Filtering SelectPINMMR172[24] PINMMR172[25] PINMMR172[26]
ePWMx Trip Zone3 (TZ3n) Input
Filtering Select3C4h PINMMR173[0] PINMMR173[1] PINMMR173[2]
ePWM SYNCI Input Filtering Select PINMMR173[8] PINMMR173[9] PINMMR173[10]
Temperature Sensor 1Select PINMMR173[16] PINMMR173[17] See Section 6.5.13
Temperature Sensor 2Select PINMMR173[24] PINMMR173[25]
Temperature Sensor 3Select 3C8h PINMMR174[0] PINMMR174[1]
EMIF Output Enable PINMMR174[8] PINMMR174[9] See Section 6.5.2
ESM1 nERROR Select PINMMR174[16] PINMMR174[17] See Section 6.5.11.1
Temperature Sensor Power Down
EnablePINMMR174[24] See Section 6.5.13
GIOA[0] DMA Request Select 3CCh PINMMR175[0] See Section 6.5.12
GIOA[1] DMA Request Select PINMMR175[8]
GIOA[2] DMA Request Select PINMMR175[16]
GIOA[3] DMA Request Select PINMMR175[24]
GIOA[4] DMA Request Select 3D0h PINMMR176[0]
GIOA[5] DMA Request Select PINMMR176[8]
GIOA[6] DMA Request Select PINMMR176[16]
GIOA[7] DMA Request Select PINMMR176[24]
GIOB[0] DMA Request Select 3D4h PINMMR177[0]
GIOB[1] DMA Request Select PINMMR177[8]
GIOB[2] DMA Request Select PINMMR177[16]
GIOB3] DMA Request Select PINMMR177[24]
GIOB[4] DMA Request Select 3D8h PINMMR178[0]
GIOB[5] DMA Request Select PINMMR178[8]
GIOB[6] DMA Request Select PINMMR178[16]
GIOB[7] DMA Request Select PINMMR178[24]
NHET2 PinDisable Select 3DCh PINMMR179[0] PINMMR179[1] See Section 6.5.6
NHET1 PinDisable Select PINMMR179[8] PINMMR179[9]

<!-- Page 314 -->

Control ofSpecial Multiplexed Options www.ti.com
314 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.1 Control ofSDRAM Clock (EMIF_CLK)
Asshown inTable 6-1,PINMMR9[0] issetbydefault. This blocks theEMIF SDRAM clock signal
(EMIF_CLK) from being output from themicrocontroller. IftheEMIF isused toconnect toanexternal
SDRAM module, then theapplication must enable theSDRAM clock output byclearing thePINMMR9[0]
bitandsetthePINMMR9[1].
6.5.2 Control forother EMIF Outputs
There aresome EMIF signals (EMIF_ADDR[00], EMIF_ADDR[01], EMIF_ADDR[06], EMIF_ADDR[07],
EMIF_ADDR[08], EMIF_BA[1], EMIF_nCS[0], EMIF_nCS[3]), thataremultiplexed with N2HET2 signals.
Forapplications thatrequire theuseofthese N2HET2 signals, itisinconvenient iftheEMIF starts driving
these address andcontrol signals asoutput after reset isreleased andbefore theapplication can
configure theI/OMultiplexing Module registers. Therefore, these EMIF signals areblocked from being
output bydefault when PINMMR174[8]=1 andPINMMR174[9]=0. Inthiscondition, these EMIF/N2HET2
terminals areconfigured asinputs andpulled down. Anapplication thatrequires theEMIF functionality
must setPINMMR174[8]=0 andPINMMR174[9]=1. This causes theEMIF address andcontrol signals to
then beoutput ontheEMIF/N2HET2 terminals when theEMIF functionality isselected viatheIOMM
output multiplexing control registers.
6.5.3 Control ofEthernet Controller Mode
PINMMR160[24] issetbydefault. This bitisused toenable theRMII (Reduced Media Independent
Interface oftheEthernet controller). Iftheapplication desires tousetheMII(Media Independent Interface
oftheEthernet controller), then thePINMMR160[24] must becleared.
6.5.4 Control ofADC Trigger Events
The microcontrollers contain twoAnalog-to-Digital Converter modules: ADC1 andADC2. The ADC
conversions canbestarted using arising orfalling orboth edges asthetrigger event. Both theADC
modules support uptoeight event trigger inputs. There aretwosets ofthese 8inputs foreach ADC. The
option foreach ofthese 8inputs arecontrolled byregisters intheI/Omultiplexing module asshown in
Table 6-4andTable 6-5.
Table 6-4.ADC1 Trigger Event Selection
Group Source
Select, G1SRC,
G2SRC orEVSRCEvent # PINMMR161[0] PINMMR161[1] Control Option A Control Option B Trigger Source
000 1 x x NA NA AD1EVT
001 2 1 0 PINMMR161[8] =x PINMMR161[9] =x N2HET1[8]
0 1 PINMMR161[8] =1 PINMMR161[9] =0 N2HET2[5]
0 1 PINMMR161[8] =0 PINMMR161[9] =1 ePWM_B
010 3 1 0 NA NA N2HET1[10]
0 1 NA NA N2HET1[27]
011 4 1 0 PINMMR161[16] =x PINMMR161[17] =x RTI1 Comp0
0 1 PINMMR161[16] =1 PINMMR161[17] =0 RTI1 Comp0
0 1 PINMMR161[16] =0 PINMMR161[17] =1 ePWM_A1
100 5 1 0 NA NA N2HET1[12]
0 1 NA NA N2HET1[17]
101 6 1 0 PINMMR161[24] =x PINMMR161[25] =x N2HET1[14]
0 1 PINMMR161[24] =1 PINMMR161[25] =0 N2HET1[19]
0 1 PINMMR161[24] =0 PINMMR161[25] =1 N2HET2[1]
110 7 1 0 PINMMR162[0] =x PINMMR162[1] =x GIOB[0]
0 1 PINMMR162[0] =1 PINMMR162[1] =0 N2HET1[11]
0 1 PINMMR162[0] =0 PINMMR162[1] =1 ePWM_A2
111 8 1 0 PINMMR162[8] =x PINMMR162[9] =x GIOB[1]
0 1 PINMMR162[8] =1 PINMMR162[9] =0 N2HET2[13]
0 1 PINMMR162[8] =0 PINMMR162[9] =1 ePWM_AB

<!-- Page 315 -->

www.ti.com Control ofSpecial Multiplexed Options
315 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Table 6-5.ADC2 Trigger Event Selection
Group Source
Select, G1SRC,
G2SRC orEVSRCEvent # PINMMR161[0] PINMMR161[1] Control Option A Control Option B Trigger Source
000 1 x x NA NA AD2EVT
001 2 1 0 PINMMR162[16] =x PINMMR162[17] =x N2HET1[8]
0 1 PINMMR162[16] =1 PINMMR162[17] =0 N2HET2[5]
0 1 PINMMR162[16] =0 PINMMR162[17] =1 ePWM_B
010 3 1 0 NA NA N2HET1[10]
0 1 NA NA N2HET1[27]
011 4 1 0 PINMMR162[24] =x PINMMR162[25] =x RTI1 Comp0
0 1 PINMMR162[24] =1 PINMMR162[25] =0 RTI1 Comp0
0 1 PINMMR162[24] =0 PINMMR162[25] =1 ePWM_A1
100 5 1 0 NA NA N2HET1[12]
0 1 NA NA N2HET1[17]
101 6 1 0 PINMMR163[0] =x PINMMR163[1] =x N2HET1[14]
0 1 PINMMR163[0] =1 PINMMR163[1] =0 N2HET1[19]
0 1 PINMMR163[0] =0 PINMMR163[1] =1 N2HET2[1]
110 7 1 0 PINMMR163[8] =x PINMMR163[9] =x GIOB[0]
0 1 PINMMR163[8] =1 PINMMR163[9] =0 N2HET1[11]
0 1 PINMMR163[8] =0 PINMMR163[9] =1 ePWM_A2
111 8 1 0 PINMMR163[16] =x PINMMR163[17] =x GIOB[1]
0 1 PINMMR163[16] =1 PINMMR163[17] =0 N2HET2[13]
0 1 PINMMR163[16] =0 PINMMR163[17] =1 ePWM_AB
6.5.5 Control forADC Event Trigger Signal Generation from ePWMx Modules
This microcontroller implements 7ePWM modules, seeFigure 6-4.Each ofthese modules generate two
outputs, SOCA (Start OfConversion) andSOCB, foruseintriggering theon-chip ADC modules. Registers
from theI/Omultiplexing module areused tocontrol thelogic forgeneration oftheePWM_A1, ePWM_A2,
ePWM_AB, andePWM_B signals from these ePWMx_SOCA andePWMx_SOCB signals.

<!-- Page 316 -->

ePWM2
moduleEPWM2SOCA
EPWM2SOCBePWM1
moduleEPWM1SOCA
EPWM1SOCB
ePWM3
moduleEPWM3SOCA
EPWM3SOCB
ePWM4
moduleEPWM4SOCA
EPWM4SOCB
ePWM5
moduleEPWM5SOCA
EPWM5SOCB
ePWM6
moduleEPWM6SOCA
EPWM6SOCB
EPWM7SOCA
EPWM7SOCBePWM7
module
ePWM_B ePWM_A1 ePWM_A2 ePWM_ABSOCAEN, SOCBEN bits
inside ePWMx modulesControlled by PINMMR
Control ofSpecial Multiplexed Options www.ti.com
316 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Figure 6-4.ADC Trigger Event Signal Generation from ePWMx

<!-- Page 317 -->

www.ti.com Control ofSpecial Multiplexed Options
317 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)The logic equations forthe4outputs from thecombinational logic shown inFigure 6-4are:
*B=SOC1B orSOC2B orSOC3B orSOC4B orSOC5B orSOC6B orSOC7B
*A1=[SOC1A andnot(SOC1A_SEL) ]or
[SOC2A andnot(SOC2A_SEL) ]or
[SOC3A andnot(SOC3A_SEL) ]or
[SOC4A andnot(SOC4A_SEL) ]or
[SOC5A andnot(SOC5A_SEL) ]or
[SOC6A andnot(SOC6A_SEL) ]or
[SOC7A andnot(SOC7A_SEL) ]or
*A2=[SOC1A andSOC1A_SEL ]or
[SOC2A andSOC2A_SEL ]or
[SOC3A andSOC3A_SEL ]or
[SOC4A andSOC4A_SEL ]or
[SOC5A andSOC5A_SEL ]or
[SOC6A andSOC6A_SEL ]or
[SOC7A andSOC7A_SEL ]or
*AB=BorA2
The SOCxA_SEL signals used intheabove logic equations aregenerated using registers intheI/O
multiplexing module.
*PINMMR164[0] defines thevalue ofSOC1A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR164[8] defines thevalue ofSOC2A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR164[16] defines thevalue ofSOC3A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR164[24] defines thevalue ofSOC4A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR165[0] defines thevalue ofSOC5A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR165[8] defines thevalue ofSOC6A_SEL. This bitissetbydefault andcanbecleared bythe
application.
*PINMMR165[16] defines thevalue ofSOC7A_SEL. This bitissetbydefault andcanbecleared bythe
application.

<!-- Page 318 -->

PINMMR179[8]=1 and
PINMMR179[9]=0
N2HET1
GIOPIN_nDISABLE 0
1
1
0
PINMMR85[0]=1 and PINMMR85[1]=0N2HET2[01]/N2HET1_NDIS
GIOA[5]/EXTCLKIN/ePWM1A
ETMTRACECLKIN/EXTCLKIN2/GIOA[5]
Control ofSpecial Multiplexed Options www.ti.com
318 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.6 Control forGenerating Interrupt Upon External Fault Indication toN2HETx
The N2HET module onthismicrocontroller allows theapplication toselectively disable anyPWM output
from theN2HET module whenever afault condition isindicated totheN2HET. This fault condition isinput
totheN2HET module viathePIN_nDISABLE input signal. Itisimportant fortheCPU tobenotified with an
interrupt whenever thisfault condition isindicated totheN2HET module.
The PIN_nDISABLE signal fortheN2HET1 module cancome from twodifferent paths ateither the
GIOA[5] terminals ortheN2HET2[01] terminal. Bydefault with PINMMR179[8]=1 andPINMMR179[9]=0
theGIOA[5] /EXTCLKIN /ePWM1A terminal isselected astheinput forsignaling thefault condition.
Setting PINMMR179[8]=0 andPINMMR179[9]=1 willselect theterminal N2HET2[01] /N2HET1_NDIS for
signaling thefault condition.
Note thatthere aretwoterminals from which tochoose theGIOA[5] signal since GIOA[5] isavailable in
twodifferent terminals. Bydefault with PINMMR85[0]=1 andPINMMR85[1]=0 theterminal shared by
GIOA[5] /EXTCLKIN /ePWM1A isselected. Setting PINMMR85[0]=0 andPINMMR85[1]=1 willselect the
terminal shared byETMTRACECLKIN /EXTCLKIN2 /GIOA[5]. When GIOA[5] ischosen tosignal the
fault condition itcanalso beaninterrupt totheCPU iftheapplication enables theinterrupt generation
whenever theGIOA[5] terminal isdriven low. Figure 6-5illustrates themultiplexing scheme.
NOTE: The default settings willchoose GIOA[5] /EXTCLKIN /ePWM1A terminal forsignaling the
fault condition totheN2HET1 andthiswillbecompatible toother TMS570LSxx family of
microcontrollers which have thisavailable feature.
Figure 6-5.GIOA[5] andN2HET1_NDIS Input Multiplexing Scheme

<!-- Page 319 -->

MIBSPI3NCS[0]/AD2EVT/eQEP1I/
GIOB[2]/N2HET2_NDIS
N2HET2
GIO GIOB[2]/DCAN4TX
PINMMR160[16] = 1 and10PIN_nDISABLE
FRAYTXEN2/GIOB[2]
PINMMR86[8 ]= 1 and PINMMR86[9] = 01
01
0N2HET2[02]/GIOB[2]/N2HET2_NDISPINMMR179[0 ]= 1 and
PINMMR179[1] = 0
PINMMR160[17] = 0To MibSPI3
To MibADC2
www.ti.com Control ofSpecial Multiplexed Options
319 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)The PIN_nDISABLE signal fortheN2HET2 module canalso come from twodifferent paths ateither the
MIBSPI3NCS[0] /AD2EVT /eQEP1I /GIOB[2] /N2HET2_NDIS terminal ortheN2HET2[02] /GIOB[2] /
N2HET2_NDIS terminal. Bydefault with PINMMR179[0]=1 andPINMMR179[1]=0, theMIBSPI3NCS[0] /
AD2EVT /eQEP1I /GIOB[2] /N2HET2_NDIS terminal isselected astheinput forsignaling thefault
condition. Setting PINMMR179[0]=0 andPINMMR179[1]=1 willselect theN2HET2[02] /GIOB[2] /
N2HET2_NDIS terminal forsignaling thefault condition. Bydefault with PINMMR160[16]=1 and
PINMMR160[17]=0, these signals donotoffer thecapability ofgenerating aninterrupt totheGIO module
when they aredriven low. Therefore, theinput from thisterminal canoptionally beconnected tothe
GIOB[2] input. This connection isenabled bysetting PINMMR160[0]=0 andPINMMR160[1]=1.
Note thattheGIO module hasfour sources from which tochoose theGIOB[2] signal. Bydefault with
PINMMR86[8]=1 andPINMMR86[9]=0, theGIOB[2] /DCAN4TX terminal isselected thatisdefined in
Table 6-2,seeregister ataddress FFFF_1E68h. Bysetting PINMMR86[8]=0 andPINMMR86[9]=1, the
terminal shared byFRAYTXEN2 andGIOB[2] isselected. When either oneofthese twoterminals is
selected, itisnotpossible touseGIO module tocause aninterrupt totheCPU when afault condition is
detected. Tocause aninterrupt totheGIO using GIOB[2] orN2HET2_NDIS signal, thePINMMR160[16]
must beclear andPINMMR160[17] must besetwhile either MibSPI3NCS[0] /AD2EVT /GIOB[2] terminal
ortheN2HET2[02] /N2HET2_NDIS terminal isconnected totheexternal monitor circuit. Figure 6-6
illustrates themultiplexing scheme.
Figure 6-6.GIOB[2] andN2HET2_NDIS Input Multiplexing Scheme
NOTE: The default settings willchoose MIBSPI3NCS[0] /AD2EVT /GIOB[2] terminal forsignaling
thefault condition totheN2HET2 andthiswillbecompatible toother TMS570LSxx family of
microcontrollers which have thisavailable feature.

<!-- Page 320 -->

N2HET1_LOOP_SYNC EXT_LOOP_SYNC
N2HET2 N2HET1
2 VCLK3 cycles
Pulse Stretch
SYNCIePWM1PINMMR165[24]=0 and PINMMR165[25]=1
ePWM1_SYNCIDouble
synch
6-bit
counterePWM1SYNCI
Control ofSpecial Multiplexed Options www.ti.com
320 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.7 Control forSynchronizing Time Bases forAllePWMx Modules
The ePWMx modules implement amechanism thatallows their time bases tobesynchronized. This is
done byusing asignal called TBCLKSYNC, which isacommon input toalltheePWMx modules. This
TBCLKSYNC isgenerated byaregister bitintheI/Omultiplexing module. PINMMR166[1] isthe
TBCLKSYNC signal. This bitiscleared (0)bydefault.
When TBCLKSYNC =0,thetime-base clock ofallePWMx modules isstopped. This isthedefault
condition.
When TBCLKSYNC =1,thetime-base clocks ofallePWMx modules arestarted aligned totherising edge
oftheTBCLKSYNC signal.
The correct procedure forenabling andsynchronizing thetime-base clocks ofalltheePWMx modules is:
1.Enable theclocks tothedesired individual ePWMx modules ifthey have been disabled
2.SetTBCLKSYNC =0.This willstop thetime-base clocks ofanyenabled ePWMx module.
3.Configure thetime-base clock prescaler values anddesired ePWM modes.
4.SetTBCLKSYNC =1.
6.5.8 Control forSynchronizing allePWMx Modules toN2HET1 Module Time-Base
Some applications require asynchronized time base forallPWM signals generated bythemicrocontroller.
The N2HET1 module uses atime base thatiscreated byconfiguring thehigh-resolution andloop-
resolution prescalers intheN2HET1 module control registers. The N2HET1 module outputs theloop-
resolution clock signal (N2HET1_LOOP_SYNC) sothatother timer modules onthemicrocontroller can
useittosynchronize their time bases totheN2HET1 loop-resolution clock.
There isadedicated connection between theN2HET1 andN2HET2 modules, which allows theN2HET2
tousetheN2HET1_LOOP_SYNC signal tosynchronize itsown time base tothatofN2HET1.
The seven ePWMx modules canalso optionally usetheN2HET1_LOOP_SYNC fortheir time-base
synchronization using aspecially designed scheme.
Figure 6-7.Synchronizing ePWMx Modules toN2HET1 Time-Base

<!-- Page 321 -->

www.ti.com Control ofSpecial Multiplexed Options
321 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)PINMMR165[24] andPINMMR165[25] areused toselect between theePWM1_SYNCI andthestretched
N2HET1_LOOP_SYNC signals.
*IfPINMMR165[24] =1andPINMMR165[25] =0,theSYNCI input totheePWM1 comes from the
ePWM1_SYNCI which isanoutput from amultiplexor. This isthedefault connection. There arealso
three possible selections from which tochoose theePWM1_SYNC1.
-When PINMMR173[8] =1,theEPWM1SYNCI terminal isconnected directly totheePWM1
module's SYNCI port. This isthedefault connection.
-When PINMMR173[8] =0,PINMMR173[9] =1,andPINMMR173[10] =0,theEPWM1SYNCI
terminal isdouble-synchronized using VCLK3 andthen connected totheePWM1 module's SYNCI
port.
-When PINMMR173[8] =0,PINMMR173[9] =0,andPINMMR173[10] =1,theEPWM1SYNCI
terminal isdouble-synchronized using VCLK3, qualified through a6-cycle counter using VCLK3 and
then connected totheePWM1 module's SYNCI port.
*IfPINMMR165[24]=0 andPINMMR165[25]=1, theSYNCI input totheePWM1 comes from thepulse-
stretched N2HET1_LOO_SYNC signal.
6.5.9 Control forInput Connections toePWMx Modules
The ePWMx modules take thefollowing signals asinput:
*ePWM1_SYNCI: external time-base input totheePWMx
*nTZ1 through nTZ6: trip-zone inputs totheePWMx
Ofthesixtrip-zone inputs, three areinput from device terminals while theother three areconnected to
internal error events. Registers from theI/Omultiplexing module areused tocontrol various aspects of
these input connections totheePWMx modules.
Table 6-6.Controls forePWMx Inputs
Input SignalControl forAsynchronous
Input
(default connection)Control forDouble-VCLK3-
Synchronized InputControl forDouble-VCLK3-
Synchronized and
6-VCLK3-Filtered Input
nTZ1 PINMMR172[16] =1PINMMR172[16] =0
PINMMR172[17] =1PINMMR172[17:16] ="00"
PINMMR172[18] =1
nTZ2 PINMMR172[24] =1PINMMR172[24] =0
PINMMR172[25] =1PINMMR172[25:24] ="00"
PINMMR172[26] =1
nTZ3 PINMMR173[0] =1PINMMR173[0] =0
PINMMR173[1] =1PINMMR173[1:0] ="00"
PINMMR173[2] =1
ePWM1_SYNCI PINMMR173[8] =1PINMMR173[8] =0
PINMMR173[9] =1PINMMR173[9:8] ="00"
PINMMR173[10] =1

<!-- Page 322 -->

Control ofSpecial Multiplexed Options www.ti.com
322 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)Ofthethree internal error events forthetrip-zone inputs, nTZ4 isconnected totheeQEPx error output
signal. There aretwoeQEP modules onthismicrocontroller, andregisters intheI/Omultiplexing module
areused toallow aflexible scheme fortheconnection between theeQEPx error signal andthenTZ4
inputs oftheePWMx modules.
Table 6-7.Controls foreQEPx_ERROR Connection toePWMx nTZ4 Inputs
ePWMx ModuleControl fornTZ4 =
not(eQEP1ERR oreQEP2ERR)
(default connection)Control fornTZ4 =
not(eQEP1ERR)Control fornTZ4 =
not(eQEP2ERR)
ePWM1 PINMMR167[0] =1PINMMR167[0] =0
PINMMR167[1] =1PINMMR167[1:0] ="00"
PINMMR167[2] =1
ePWM2 PINMMR167[8] =1PINMMR167[8] =0
PINMMR167[9] =1PINMMR167[9:8] ="00"
PINMMR167[10] =1
ePWM3 PINMMR167[16] =1PINMMR167[16] =0
PINMMR167[17] =1PINMMR167[17:16] ="00"
PINMMR167[18] =1
ePWM4 PINMMR167[24] =1PINMMR167[24] =0
PINMMR167[25] =1PINMMR167[25:24] ="00"
PINMMR167[26] =1
ePWM5 PINMMR168[0] =1PINMMR168[0] =0
PINMMR168[1] =1PINMMR168[1:0] ="00"
PINMMR168[2] =1
ePWM6 PINMMR168[8] =1PINMMR168[8] =0
PINMMR168[9] =1PINMMR168[9:8] ="00"
PINMMR168[10] =1
ePWM7 PINMMR168[16] =1PINMMR168[16] =0
PINMMR168[17] =1PINMMR168[17:16] ="00"
PINMMR168[18] =1
6.5.10 Control forInput Connections toeCAPx Modules
Each eCAPx module hasasingle input from thedevice terminals. This input canbeconnected tothe
eCAPx module inoneoftwoways:
1.Double-synchronized using VCLK3
2.Double-synchronized using VCLK3 andthen filtered through a6-VCLK3-cycle counter
Registers intheI/Omultiplexing module areused tocontrol these input connections foreach eCAPx
module.
Table 6-8.Controls foreCAPx Inputs
eCAPx InputControl forDouble-VCLK3-
Synchronized Input
(default connection)Control forDouble-VCLK3-
Synchronized and
6-VCLK3-Filtered Input
eCAP1 PINMMR169[0] =0PINMMR169[0] =0
PINMMR169[1] =1
eCAP2 PINMMR169[8] =0PINMMR169[8] =0
PINMMR169[9] =1
eCAP3 PINMMR169[16] =0PINMMR169[16] =0
PINMMR169[17] =1
eCAP4 PINMMR169[24] =0PINMMR169[24] =0
PINMMR169[25] =1
eCAP5 PINMMR170[0] =0PINMMR170[0] =0
PINMMR170[1] =1
eCAP6 PINMMR170[8] =0PINMMR170[8] =0
PINMMR170[9] =1

<!-- Page 323 -->

www.ti.com Control ofSpecial Multiplexed Options
323 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.11 Control forInput Connections toeQEPx Modules
Each eQEPx module hasfour inputs from thedevice terminals. These inputs canbeconnected tothe
eQEPx module inoneoftwoways:
1.Double-synchronized using VCLK3
2.Double-synchronized using VCLK3 andthen filtered through a6-VCLK3-cycle counter
Registers intheI/Omultiplexing module areused tocontrol these input connections foreach eQEPx
module.
Table 6-9.Controls foreQEPx Inputs
eQEPx InputControl forDouble-VCLK3-
Synchronized Input
(default connection)Control forDouble-VCLK3-
Synchronized and
6-VCLK3-Filtered Input
eQEP1A PINMMR170[16] =1PINMMR170[16] =0
PINMMR170[17] =1
eQEP1B PINMMR170[24] =1PINMMR170[24] =0
PINMMR170[25] =1
eQEP1I PINMMR171[0] =1PINMMR171[0] =0
PINMMR171[1] =1
eQEP1S PINMMR171[8] =1PINMMR171[8] =0
PINMMR171[9] =1
eQEP2A PINMMR171[16] =1PINMMR171[16] =0
PINMMR171[17] =1
eQEP2B PINMMR171[24] =1PINMMR171[24] =0
PINMMR171[25] =1
eQEP2I PINMMR172[0] =1PINMMR172[0] =0
PINMMR172[1] =1
eQEP2S PINMMR172[8] =1PINMMR172[8] =0
PINMMR172[9] =1

<!-- Page 324 -->

xxxxxxxxx
xxxx
xxxxxxxxxxxxxESM1
xxxxxxxxxx
xxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxx
ESM2xxxxxx
xxx
xxxxxxxxxAND
xxxxxxxxxxxxxxxx
xxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxx
xxx
xxx
xxxxxxxxx
xxxx
xxxxxxxxxxxx
xxxx
xxxxxxxxxxxx
xxxxxx xxxxxxxx
xx
xxxxxxxxxxxx
xxxxxxxxxxx xxxxxxxxxxxxx
nERROR_OUT
xxxxxxxxxxxxxx
xxxxxxx
xxxxxxxxxxxxxxxx
xxxxxx
xxxxxxxxxxxxxxxxxx
xxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxx
xxxx
xxx
xxxxxxxx
x xxxxxxxxxxxx
xx
xxxxxxxxxxxx
xxxxxx xxxxxxxxxxxxxxxxxx
xxx
xxxxxxxxxxxxxxnERROR_OUT
xxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxx
xxxxxx
xxxxxxxxxxxxxxxxx
xxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxx
xxx
xxx
xxxxxxxxx
xx
xxxxxxxxx
xxx
xxxxxxxxx
xxxxxxx xxxxxxxxxx
xx
xxxxxxxxxxxxxx
xxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxxxx
xxxxx
xxxxxxxx
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxxxxxxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxxxxxxx
xxxxxnERROR_IN
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxnERROR_INnERRORnERROR1
nERROR2xxxxxxxPINMMR174[16]
Control ofSpecial Multiplexed Options www.ti.com
324 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.11.1 nERROR andnERROR1 Input Multiplexing
There aretwoESM modules inthismicrocontroller. When themicrocontroller isinlockstep mode, only
ESM1 isactive.
Bydefault, theESM1 Error PinStatus Register (ESMEPSR) samples thenERROR pininput. The default
isachieved with PINMMR174[16] =1.Bysetting PINMMR174[16] =0,theESM1 Error PinStatus register
willsample from thenERROR1 pininstead asillustrated inFigure 6-8.
Figure 6-8.nERROR andnERROR1 Input Multiplexing

<!-- Page 325 -->

DMAREQ[32]
PINMMR175[0]GIOA[0]I2C2 receive
EPWM1_SOCA
MIBSPI2[2]
MIBSPI4[2]
DMA
DMAREQ[47]
www.ti.com Control ofSpecial Multiplexed Options
325 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.12 Selecting GIO Port forExternal DMA Request
GIOA andGIOB ports canbeused togenerate external DMA request. See thedatasheet fortheDMA
request mapping. The polarity oftheGIO pintotrigger aDMA request canbeselected inside theDMA
module. Inorder touseGIO pinasanexternal DMA request input, thecorresponding pinmust befirst
selected bytheapplication. Bydefault, itisunselected. See Figure 6-9forillustration. Inthisillustration,
theDMQREQ[32] input totheDMA module cancome from several different sources including GIOA[0]. By
default with PINMMR175[0] =1,allother sources except GIOA[0] canbeselected togenerate aDMA
request. Care must betaken bytheapplication thatonly onesource isactive while other sources are
inactive from their respective modules. When PINMMR175[0] =0,only GIOA[0] isselected togenerate the
DMA request.
Figure 6-9.Using GIOPinforExternal DMA Request
Table 6-10. GIODMA Request Select BitMapping
GIOPin GIODMA Request Select Bit BitValue toSelect GIO
GIOA[0] PINMMR175[0] 0
GIOA[1] PINMMR175[8] 0
GIOA[2] PINMMR175[16] 0
GIOA[3] PINMMR175[24] 0
GIOA[4] PINMMR176[0] 0
GIOA[5] PINMMR176[8] 0
GIOA[6] PINMMR176[16] 0
GIOA[7] PINMMR176[24] 0
GIOB[0] PINMMR177[0] 0
GIOB[1] PINMMR177[8] 0
GIOB[2] PINMMR177[16] 0
GIOB[3] PINMMR177[24] 0
GIOB[4] PINMMR178[0] 0
GIOB[5] PINMMR178[8] 0
GIOB[6] PINMMR178[16] 0
GIOB[7] PINMMR178[24] 0

<!-- Page 326 -->

Control ofSpecial Multiplexed Options www.ti.com
326 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.5.13 Temperature Sensor Selection
There arethree instances oftemperature sensors inthismicrocontroller. The measured temperatures are
analog signals. These analog signals areconnected totheon-chip ADCs forconversion. Before the
temperature sensors canbeused, they must beenabled. Bydefault, they aredisabled with
PINMMR174[24] =1.Toenable thetemperature sensors, PINMMR174[24] must becleared to0.
*Temperature sensor 1'soutput ismultiplexed with AD1IN[31].
*Temperature sensor 2'soutput ismultiplexed with AD2IN[31].
*Temperature sensor 3'soutput ismultiplexed with AD2IN[30].
NOTE: TouseAD1IN[31], PINMMR174[24] must becleared to0.
(1)AD1CHNSEL isconfigured inside theMibADC1 Wrapper. AD2CHNSEL isconfigured inside theMibADC2 Wrapper.Table 6-11. Temperature Sensor Selection
Decode(1)Select
AD1CHNSEL(31) =1
PINMMR173(16) =0
PinMMR173(17) =1Temp Sensor 1
AD1CHNSEL(31) =1
PINMMR173(16) =1
PinMMR173(17) =0AD1IN[31]
AD2CHNSEL(31) =1
PINMMR173(24) =0
PINMMR173(25) =1Temp Sensor 2
AD2CHNSEL(31) =1
PINMMR173(24) =1
PINMMR173(25) =0AD2IN[31]
AD2CHNSEL(30) =1
PINMMR174(0) =0
PINMMR174(1) =1Temp Sensor 3
AD2CHNSEL(30) =1
PINMMR174(0) =1
PINMMR174(1) =0AD2IN[30]

<!-- Page 327 -->

www.ti.com Safety Features
327 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.6 Safety Features
The IOMM supports certain safety functions thataredesigned toprevent unintentional changes totheI/O
multiplexing configuration. These aredescribed inthefollowing sections.
6.6.1 Locking Mechanism forMemory-Mapped Registers
The IOMM contains amechanism toprevent anyspurious writes from changing anyofthePINMMR
values. The PINMMRs arelocked bydefault andafter anysystem reset. None oftheIOMM registers can
bewritten under thiscondition. The application canread anyoftheIOMM registers regardless ofthestate
ofthelocking mechanism.
*Enabling Write Access tothePINMMRs
Toenable write access tothePINMMRs, theCPU must write 0x83e70b13 tothekick0 register followed by
awrite of0x95a4f1e0 tothekick1 register.
*Disabling Write Access tothePINMMRs
Itisrecommended todisable write access tothePINMMRs once theI/Omultiplexing configuration is
completed. This canbedone by:
*writing anyother data value toeither ofthekick registers, or
*restarting theunlock sequence bywriting 0x83e70b13 tothekick0 register
NOTE: NoError OnWrite toLocked PINMMRs
There isnoerror response onanywrite accesses tothePINMMRs when write access is
disabled. None ofthePINMMRs change state duetothiswrite.
6.6.2 Error Conditions
The IOMM generates oneerror signal thatismapped totheError Signaling Module 'sGroup 1,
channel 37.This error signal isgenerated under either ofthefollowing twoconditions:
*Address Error -occurs when there isaread orawrite access toanun-implemented memory location
within theIOMM register frame.
*Protection Error -occurs when theCPU writes toanIOMM register while notinaprivileged mode of
operation.

<!-- Page 328 -->

IOMM Registers www.ti.com
328 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7 IOMM Registers
Table 6-12 lists thecontrol registers intheIOMM. The address offset isspecified from thebase address of
FFFF 1C00h.
Table 6-12. IOMM Registers
Offset Acronym Register Description Section
00h REVISION_REG Revision Register Section 6.7.1
20h BOOT_REG Boot Mode Register Section 6.7.2
38h KICK_REG0 Kicker Register 0 Section 6.7.3
3Ch KICK_REG1 Kicker Register 1 Section 6.7.4
E0h ERR_RAW_STATUS_REG Error Raw Status /SetRegister Section 6.7.5
E4h ERR_ENABLED_STATUS_REG Error Enabled Status /Clear Register Section 6.7.6
E8h ERR_ENABLE_REG Error Signaling Enable Register Section 6.7.7
ECh ERR_ENABLE_CLR_REG Error Signaling Enable Clear Register Section 6.7.8
F4h FAULT_ADDRESS_REG Fault Address Register Section 6.7.9
F8h FAULT_STATUS_REG Fault Status Register Section 6.7.10
FCh FAULT_CLEAR_REG Fault Clear Register Section 6.7.11
110h-1A4h PINMMRnn Output PinMultiplexing Control Registers Section 6.7.12
250h-29Ch PINMMRnn Input PinMultiplexing Control Registers Section 6.7.13
390h-3DCh PINMMRnn Special Functionality Control Registers Section 6.7.14
6.7.1 REVISION_REG: Revision Register
Figure 6-10. REVISION_REG: Revision Register (Offset =00h)
31 30 29 28 27 16
REV SCHEME Reserved REV MODULE
R-1 R-0 R-E84h
15 11 10 8 7 6 5 0
REV RTL REV MAJOR REV CUSTOM REV MINOR
R-0 R-1 R-0 R-2h
LEGEND: R=Read only; C=Clear; -n=value after reset
Table 6-13. Revision Register Field Descriptions
Bit Field Value Description
31-30 REV SCHEME 01 Revision Scheme
29-28 Reserved 0 Reads return 0,writes have noeffect.
27-16 REV MODULE E84h Module Id
15-11 REV RTL 0 RTL Revision
10-8 REV MAJOR 001 Major Revision
7-6 REV CUSTOM 0 Custom Revision
5-0 REV MINOR 2h Minor Revision

<!-- Page 329 -->

www.ti.com IOMM Registers
329 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.2 BOOT_REG: Boot Mode Register
Figure 6-11. BOOT_REG: Boot Mode Register (Offset =20h)
31 16
Reserved
R-0
15 1 0
Reserved ENDIAN
R-0 R-D
LEGEND: R=Read only; D=Value read isdetermined byexternal configuration; -n=value after reset
Table 6-14. Boot Mode Register Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0,writes have noeffect.
0 ENDIAN Device endianness.
0 Device isconfigured inlittle-endian mode.
1 Device isconfigured inbig-endian mode.

<!-- Page 330 -->

IOMM Registers www.ti.com
330 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.3 KICK_REG0: Kicker Register 0
This register forms thefirstpart oftheunlock sequence forbeing able toupdate theI/Omultiplexing
control registers (PINMMRnn).
Figure 6-12. KICK_REG0: Kicker Register 0(Offset =38h)
31 16
KICK0
R/W-0
15 0
KICK0
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 6-15. Kicker Register 0Field Descriptions
Bit Field Description
31-0 KICK0 Kicker 0Register. The value 83E7 0B13h must bewritten toKICK0 aspart oftheprocess tounlock theCPU
write access tothePINMMRnn registers.
6.7.4 KICK_REG1: Kicker Register 1
This register forms thesecond part oftheunlock sequence forbeing able toupdate theI/Omultiplexing
control registers (PINMMRnn).
Figure 6-13. KICK_REG1: Kicker Register 1(Offset =3Ch)
31 16
KICK1
R/W-0
15 0
KICK1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 6-16. Kicker Register 1Field Descriptions
Bit Field Description
31-0 KICK1 Kicker 1Register. The value 95A4 F1E0h must bewritten totheKICK1 aspart oftheprocess tounlock the
CPU write access tothePINMMRnn registers.

<!-- Page 331 -->

www.ti.com IOMM Registers
331 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.5 ERR_RAW_STATUS_REG: Error Raw Status /SetRegister
This register shows thestatus oftheerror conditions (before enabling) andallows setting theerror status.
Figure 6-14. ERR_RAW_STATUS_REG: Error Raw Status /SetRegister (Offset =E0h)
31 8
Reserved
R-0
7 2 1 0
Reserved ADDR_ERR PROT_ERR
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-17. Error Raw Status /SetRegister Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0,writes have noeffect.
1 ADDR_ERR Addressing Error Status. AnAddressing Error occurs when anunimplemented location inside theIOMM
register frame isaccessed.
0 Read: Addressing Error hasnotoccurred.
Write: Writing 0hasnoeffect.
1 Read: Addressing Error hasbeen detected.
Write: Addressing Error status isset.
0 PROT_ERR Protection Error Status. AProtection Error occurs when anycontrol register inside theIOMM iswritten
intheCPU 'suser mode ofoperation.
0 Read: Protection Error hasnotoccurred.
Write: Writing 0hasnoeffect.
1 Read: Protection Error hasbeen detected.
Write: Protection Error status isset.

<!-- Page 332 -->

IOMM Registers www.ti.com
332 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.6 ERR_ENABLED_STATUS_REG: Error Enabled Status /Clear Register
This register shows thestatus oftheerror conditions andallows clearing oftheerror status.
Figure 6-15. ERR_ENABLED_STATUS_REG: Error Enabled Status /Clear Register (Offset =E4h)
31 8
Reserved
R-0
7 2 1 0
Reserved ENABLED_
ADDR_ERRENABLED_
PROT_ERR
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-18. Error Signaling Enabled Status /Clear Register Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0,writes have noeffect.
1 ENABLED_ADDR_ERR Addressing Error Signaling Enable andStatus Clear
0 Read: Addressing Error Signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Addressing Error Signaling isenabled.
Write: Addressing Error status iscleared.
0 ENABLED_PROT_ERR Protection Error Signaling Enable andStatus Clear
0 Read: Protection Error Signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Protection Error Signaling isenabled.
Write: Protection Error status iscleared.

<!-- Page 333 -->

www.ti.com IOMM Registers
333 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.7 ERR_ENABLE_REG: Error Signaling Enable Register
This register shows theinterrupt enable status andallows enabling oftheinterrupts.
Figure 6-16. ERR_ENABLE_REG: Error Signaling Enable Register (Offset =E8h)
31 8
Reserved
R-0
7 2 1 0
Reserved ADDR_ERR_EN PROT_ERR_EN
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-19. Error Enable Register Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0,writes have noeffect.
1 ADDR_ERR_EN Addressing Error Signaling Enable
0 Read: Addressing Error Signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Addressing Error Signaling isenabled.
Write: Addressing Error Signaling isenabled.
0 PROT_ERR_EN Protection Error Signaling Enable
0 Read: Protection Error Signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Protection Error Signaling isenabled.
Write: Protection Error Signaling isenabled.

<!-- Page 334 -->

IOMM Registers www.ti.com
334 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.8 ERR_ENABLE_CLR_REG: Error Signaling Enable Clear Register
This register shows theerror signaling enable status andallows disabling oftheerror signaling.
Figure 6-17. ERR_ENABLE_CLR_REG: Error Signaling Enable Clear Register (Offset =ECh)
31 8
Reserved
R-0
7 2 1 0
Reserved ADDR_ERR_
EN_CLRPROT_ERR_
EN_CLR
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-20. Interrupt Enable Clear Register Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0,writes have noeffect.
1 ADDR_ERR_EN_CLR Addressing Error Signaling Enable Clear
0 Read: Addressing Error signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Addressing Error signaling isenabled.
Write: Addressing Error signaling isdisabled.
0 PROT_ERR_EN_CLR Protection Error Signaling Enable Clear
0 Read: Protection Error signaling isdisabled.
Write: Writing 0hasnoeffect.
1 Read: Protection Error signaling isenabled.
Write: Protection Error signaling isdisabled.
6.7.9 FAULT_ADDRESS_REG: Fault Address Register
This register holds theaddress offset ofthefirstfault transfer.
Figure 6-18. FAULT_ADDRESS_REG: Fault Address Register (Offset =F4h)
31 16
Reserved
R-0
15 9 8 0
Reserved FAULT_ADDR
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-21. Fault Address Register Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0,writes have noeffect.
8-0 FAULT_ADDR Fault Address. The fault address offset incase ofanaddress error oraprotection error
condition.

<!-- Page 335 -->

www.ti.com IOMM Registers
335 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.10 FAULT_STATUS_REG: Fault Status Register
This register holds thestatus andattributes ofthefirstfault transfer.
Figure 6-19. FAULT_STATUS_REG: Fault Status Register (Offset =F8h)
31 28 27 24 23 16
Reserved FAULT_ID FAULT_MSTID
R-0 R-0 R-0
15 13 12 9 8 7 6 5 0
Reserved FAULT_PRIVID Rsvd FAULT_NS Rsvd FAULT_TYPE
R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 6-22. Fault Status Register Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0,writes have noeffect.
27-24 FAULT_ID Faulting Transaction ID
23-16 FAULT_MSTID IDofMaster thatinitiated thefaulting transaction
15-13 Reserved 0 Reads return 0,writes have noeffect.
12-9 FAULT_PRIVID Faulting Privilege ID
8 Reserved 0 Reads return 0,writes have noeffect.
7 FAULT_NS Fault: Non-secure access detected
6 Reserved 0 Reads return 0,writes have noeffect.
5-0 FAULT_TYPE Type offault detected.
0 Nofault
1h User execute fault
2h User write fault
4h User read fault
8h Supervisor execute fault
10h Supervisor write fault
20h Supervisor read fault

<!-- Page 336 -->

IOMM Registers www.ti.com
336 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.11 FAULT_CLEAR_REG: Fault Clear Register
This register allows theapplication toclear thecurrent fault sothatanother canbecaptured when 1is
written tothisregister.
Figure 6-20. FAULT_CLEAR_REG: Fault Clear Register (Offset =FCh)
31 16
Reserved
R-0
15 1 0
Reserved FAULT_CLEAR
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 6-23. FAULT_CLEAR_REG: Fault Clear Register Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0,writes have noeffect.
0 FAULT_CLEAR Fault Clear
0 Read: Current value oftheFAULT_CLEAR bitis0.
Write: Writing 0hasnoeffect.
1 Read: Current value oftheFAULT_CLEAR bitis1.
Write: Writing a1clears thecurrent fault.
6.7.12 PINMMRnn: Output PinMultiplexing Control Registers
These registers control theoutput multiplexing ofthefunctionality available oneach padonthe
microcontroller. There are38such registers -PINMMR0 through PINMMR37. Each 8-bit field ofa
PINMMR register controls thefunctionality ofasingle ball/pin. The mapping between thePINMMRx
control registers andthefunctionality selected onagiven terminal isdefined inTable 6-1.
Figure 6-21. PINMMRnn: PinMultiplexing Control Registers (Offset =110h-1A4h)
31 24 23 16
PINMMRx[31-24] PINMMRx[23-16]
R/WP-1 R/WP-1
15 8 7 0
PINMMRx[15-8] PINMMRx[7-0]
R/WP-1 R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 6-24. PinMultiplexing Control Registers Field Descriptions
Bit Field Value Description
31-24 PINMMRx[31-24] 1h Each ofthese byte-fields control thefunctionality onagiven ball/pin. Please refer toTable 6-1
foralistofmultiplexed signals.23-16 PINMMRx[23-16] 1h
15-8 PINMMRx[15-8] 1h
7-0 PINMMRx[7-0] 1h

<!-- Page 337 -->

www.ti.com IOMM Registers
337 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedI/OMultiplexing andControl Module (IOMM)6.7.13 PINMMRnn: Input PinMultiplexing Control Registers
These registers control theinput multiplexing ofthefunctionality available oneach padonthe
microcontroller. There are20such registers -PINMMR80 through PINMMR99. Each 8-bit field ofa
PINMMR register controls thefunctionality ofasingle ball/pin. The mapping between thePINMMRx
control registers andthefunctionality selected onagiven terminal isdefined inTable 6-2.
Figure 6-22. PINMMRnn: PinMultiplexing Control Registers (Offset =250h-29Ch)
31 24 23 16
PINMMRx[31-24] PINMMRx[23-16]
R/WP-1 R/WP-1
15 8 7 0
PINMMRx[15-8] PINMMRx[7-0]
R/WP-1 R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 6-25. PinMultiplexing Control Registers Field Descriptions
Bit Field Value Description
31-24 PINMMRx[31-24] 1h Each ofthese byte-fields control thefunctionality onagiven ball/pin. Please refer toTable 6-2
foralistofmultiplexed signals.23-16 PINMMRx[23-16] 1h
15-8 PINMMRx[15-8] 1h
7-0 PINMMRx[7-0] 1h
6.7.14 PINMMRnn: Special Functionality Multiplexing Control Registers
These registers control thespecial functionalities onthemicrocontroller. There are20such registers -
PINMMR160 through PINMMR179. Each 8-bit field ofaPINMMR register controls onespecial
functionality. The mapping between thePINMMRx control registers andthefunctionality selected ona
given terminal isdefined inTable 6-3.
Figure 6-23. PINMMRnn: PinMultiplexing Control Registers (Offset =390h-3DCh)
31 24 23 16
PINMMRx[31-24] PINMMRx[23-16]
R/WP-1 R/WP-1
15 8 7 0
PINMMRx[15-8] PINMMRx[7-0]
R/WP-1 R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 6-26. PinMultiplexing Control Registers Field Descriptions
Bit Field Value Description
31-24 PINMMRx[31-24] 1h Each ofthese byte-fields control thefunctionality onagiven ball/pin. Please refer toTable 6-3
foralistofmultiplexed signals.23-16 PINMMRx[23-16] 1h
15-8 PINMMRx[15-8] 1h
7-0 PINMMRx[7-0] 1h