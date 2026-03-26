# Input Multiplexing Table

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 53-54 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 53 -->
53TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Terminal Configuration andFunctions Copyright ©2014 -2016, Texas Instruments Incorporated4.2.2.2 Input Multiplexing
Some signals areconnected tomore than one terminals, sothat theinputs forthese signals can come
from either ofthese terminals. Amultiplexor isimplemented tolettheapplication choose theterminal that
willbeused forproviding theinput signal from among theavailable options. The input path selection is
done based ontwobitsinthePINMMR control registers aslisted inTable 4-29.
Table 4-29. Input Multiplexing
Address
OffsetSignal Name Default Terminal Terminal 1Input
Multiplex ControlAlternate Terminal Terminal 2Input
Multiplex Control
250h AD2EVT T10 PINMMR80[0] V10 PINMMR80[1]
25Ch GIOA[0] A5 PINMMR83[24] R5 PINMMR83[25]
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
280h N2HET1[17] A13 PINMMR92[0] F3 PINMMR92[1]
N2HET1[19] B13 PINMMR92[8] G3 PINMMR92[9]
N2HET1[21] H4 PINMMR92[16] J3 PINMMR92[17]
N2HET1[23] J4 PINMMR92[24] G19 PINMMR92[25]
284h N2HET1[25] M3 PINMMR93[0] V5 PINMMR93[1]
N2HET1[27] A9 PINMMR93[8] B2 PINMMR93[9]
N2HET1[29] A3 PINMMR93[16] C3 PINMMR93[17]
N2HET1[31] J17 PINMMR93[24] W9 PINMMR93[25]

<!-- Page 54 -->
54TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Terminal Configuration andFunctions Copyright ©2014 -2016, Texas Instruments IncorporatedTable 4-29. Input Multiplexing (continued)
Address
OffsetSignal Name Default Terminal Terminal 1Input
Multiplex ControlAlternate Terminal Terminal 2Input
Multiplex Control
288h N2HET2[0] D6 PINMMR94[0] C1 PINMMR94[1]
N2HET2[1] D8 PINMMR94[8] D4 PINMMR94[9]
N2HET2[2] D7 PINMMR94[16] E1 PINMMR94[17]
N2HET2[3] E2 PINMMR94[24] D5 PINMMR94[25]
28Ch N2HET2[4] D13 PINMMR95[0] H3 PINMMR95[1]
N2HET2[5] D12 PINMMR95[8] D16 PINMMR95[9]
N2HET2[6] D11 PINMMR95[16] M1 PINMMR95[17]
N2HET2[7] N3 PINMMR95[24] N17 PINMMR95[25]
290h N2HET2[8] K16 PINMMR96[0] V2 PINMMR96[1]
N2HET2[9] L16 PINMMR96[8] K17 PINMMR96[9]
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
4.2.2.2.1 Notes onInput Multiplexing
*The Terminal xInput Multiplex Control column inTable 4-29 lists themultiplexing control register and thebitthat
must besetinorder toselect theterminal forproviding theinput signal tothesystem. Forexample, N2HET2[22]
can appears ontwo different terminals atterminal number T7and N1. Bydefault PINMMR98[24] issetand
PINMMR98[25] iscleared toselect T7forproviding N2HET2[22] tothesystem. Iftheapplication chooses touse
N1forproviding N2HET2[22] then PINMMR98[24] must becleared andPINMMR98[25] must beset.
*Base address oftheIOMM module starts at0xFFFF_1C00. Input mux control registers with thefirst register
PINMMR80 starts attheoffset address 0x250 within theIOMM module.
4.2.2.2.2 General Rules forMultiplexing Control Registers
*The PINMMR control registers can only bewritten inprivileged mode. Awrite inanonprivileged mode will
generate anerror response.
*Iftheapplication writes all9'stoany PINMMR control register, then thedefault functions areselected forthe
affected pads.
*Each byte inaPINMMR control register isused toselect thefunctionality foragiven pad. Iftheapplication sets
more than onebitwithin abyte foranypad, then thedefault function isselected forthispad.
*Several bits inthePINMMR control registers arereserved and arenotused toenable any functions. Ifthe
application sets only these bitsand clears theother bits, then thedefault functions areselected fortheaffected
pads.