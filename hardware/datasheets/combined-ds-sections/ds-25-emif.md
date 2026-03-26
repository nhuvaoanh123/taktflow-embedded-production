# External Memory Interface (EMIF)

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 109-116 (8 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 109 -->
109TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.14 External Memory Interface (EMIF)
6.14.1 Features
The EMIF includes many features toenhance the ease and flexibility ofconnecting toexternal
asynchronous memories orSDRAM devices. The EMIF features includes support for:
*3addressable chip select forasynchronous memories ofupto16MB each
*1addressable chip select space forSDRAMs upto128MB
*8or16-bit data buswidth
*Programmable cycle timings such assetup, strobe, andhold times aswell asturnaround time
*Select strobe mode
*Extended Wait mode
*Data busparking
NOTE
The EMIF isinherently BE8, orbyte invariant bigendian. This device isBE32, orword
invariant bigendian. There isnodifference when interfacing toRAM orusing an8-bit wide
data bus. However, there isanimpact when reading from external ROMs orinterfacing to
hardware registers with a16-bit wide data bus. The EMIF canbemade BE32 byconnecting
EMIF_DATA[7:0] totheROM orASIC DATA[15:8] and EMIF_DATA[15:8] totheROM or
ASIC DATA[7:0].
Alternatively, thecode stored intheROM canbelinked as-be8 instead of-be32.
NOTE
Fora32-bit access onthe16-bit EMIF interface, thelower 16-bits (the EMIF_BA[1] willbe
low) willbeputoutfirstfollowed bytheupper 16-bits (EMIF_BA[1] willbehigh).

<!-- Page 110 -->
EMIF_nCS[3:2]
11
Asserted Deasserted22EMIF_BA[1:0]
EMIF_ADDR[21:0]
EMIF_DATA[15:0]
EMIF_nOE
EMIF_WAITSETUP Extended Due to EMIF_WAIT STROBE HOLD
14STROBE
EMIF_nCS[3:2]
EMIF_BA[1:0]
13
12EMIF_ADDR[21:0]
EMIF_nOE
EMIF_DATA[15:0]
EMIF_nWE105
9
74
8
63
1
EMIF_nDQM[1:0]
30 29
110TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.14.2 Electrical andTiming Specifications
6.14.2.1 Read Timing (Asynchronous RAM)
Figure 6-11. Asynchronous Memory Read Timing
Figure 6-12. EMIFnWAIT Read Timing Requirements

<!-- Page 111 -->
EMIF_nCS[3:2]
25
Asserted22EMIF_BA[1:0]
EMIF_ADDR[21:0]
EMIF_DATA[15:0]
EMIF_nWE
EMIF_WAITSETUP Extended Due to EMIF_WAIT
28
DeassertedSTROBE STROBE HOLD
EMIF_nCS[3:2]
EMIF_BA[1:0]
EMIF_ADDR[21:0]
EMIF_nWE
EMIF_DATA[15:0]
EMIF_nOE15
1
16
18
20
222417
19
21
23
2627EMIF_nDQM[1:0]
111TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) E=EMIF_CLK period inns.6.14.2.2 Write Timing (Asynchronous RAM)
Figure 6-13. Asynchronous Memory Write Timing
Figure 6-14. EMIFnWAIT Write Timing Requirements
6.14.2.3 EMIF Asynchronous Memory Timing
Table 6-35. EMIF Asynchronous Memory Timing Requirements(1)
NO. MIN NOM MAX UNIT
Reads andWrites

<!-- Page 112 -->
112TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-35. EMIF Asynchronous Memory Timing Requirements(1)(continued)
NO. MIN NOM MAX UNIT
(2) Setup before endofSTROBE phase (ifnoextended wait states areinserted) bywhich EMIFnWAIT must beasserted toaddextended
wait states. Figure 6-12 andFigure 6-14 describe EMIF transactions thatinclude extended wait states inserted during theSTROBE
phase. However, cycles inserted aspart ofthisextended wait period should notbecounted; the4Erequirement istothestart ofwhere
theHOLD phase would begin ifthere were noextended wait cycles.2 tw(EM_WAIT) Pulse duration, EMIFnWAIT assertion anddeassertion 2E ns
Reads
12 tsu(EMDV-EMOEH) Setup time, EMIFDATA[15:0] valid before EMIFnOE high 11 ns
13 th(EMOEH-EMDIV) Hold time, EMIFDATA[15:0] valid after EMIFnOE high 0.5 ns
14 tsu(EMOEL-EMWAIT) Setup Time, EMIFnWAIT asserted before endofStrobe Phase(2)4E+14 ns
Writes
28 tsu(EMWEL-EMWAIT) Setup Time, EMIFnWAIT asserted before endofStrobe Phase(2)4E+14 ns
(1) TA=Turn around, RS=Read setup, RST =Read strobe, RH=Read hold, WS=Write setup, WST =Write strobe, WH=Write hold,
MEWC =Maximum external wait cycles. These parameters areprogrammed through theAsynchronous Bank andAsynchronous Wait
Cycle Configuration Registers. These support thefollowing ranges ofvalues: TA[4 -1],RS[16 -1],RST[64 -1],RH[8 -1],WS[16 -1],
WST[64 -1],WH[8 -1],andMEWC[1 -256]. See theEMIF chapter oftheTRM SPNU563 formore information.
(2) E=EMIF_CLK period inns.
(3) EWC =external wait cycles determined byEMIFnWAIT input signal. EWC supports thefollowing range ofvalues. EWC[256 -1].Note
thatthemaximum wait time before time-out isspecified bybitfield MEWC intheAsynchronous Wait Cycle Configuration Register. See
theEMIF chapter oftheTRM SPNU563 formore information.Table 6-36. EMIF Asynchronous Memory Switching Characteristics(1)(2)(3)
NO. PARAMETER MIN TYP MAX UNIT
Reads andWrites
1 td(TURNAROUND) Turn around time (TA)*E -3 (TA)*E (TA)*E +3 ns
Reads
3 tc(EMRCYCLE) EMIF read cycle time (EW =0) (RS+RST+RH)*E-3 (RS+RST+RH)*E (RS+RST+RH)*E +3 ns
EMIF read cycle time (EW =1)(RS+RST+RH+
EWC)*E -3(RS+RST+RH+
EWC)*E(RS+RST+RH+
EWC)*E +3ns
4 tsu(EMCEL-EMOEL)Output setup time, EMIF_nCS[4:2] lowto
EMIF_nOE low(SS=0)(RS)*E-3 (RS)*E (RS)*E+3 ns
Output setup time, EMIFnCS[4:2] lowto
EMIF_nOE low(SS=1)-3 0 3 ns
5 th(EMOEH-EMCEH)Output hold time, EMIF_nOE high to
EMIF_nCS[4:2] high (SS=0)(RH)*E -4 (RH)*E (RH)*E +3 ns
Output hold time, EMIF_nOE high to
EMIF_nCS[4:2] high (SS=1)-4 0 3 ns
6 tsu(EMBAV-EMOEL)Output setup time, EMIF_BA[1:0] valid to
EMIF_nOE low(RS)*E-3 (RS)*E (RS)*E+3 ns
7 th(EMOEH-EMBAIV)Output hold time, EMIF_nOE high to
EMIF_BA[1:0] invalid(RH)*E-4 (RH)*E (RH)*E+3 ns
8 tsu(EMBAV-EMOEL)Output setup time, EMIF_ADDR[21:0] valid to
EMIF_nOE low(RS)*E-3 (RS)*E (RS)*E+3 ns
9 th(EMOEH-EMAIV)Output hold time, EMIF_nOE high to
EMIF_ADDR[21:0] invalid(RH)*E-4 (RH)*E (RH)*E+3 ns
10 tw(EMOEL) EMIF_nOE active lowwidth (EW =0) (RST)*E-3 (RST)*E (RST)*E+3 ns
EMIF_nOE active lowwidth (EW =1) (RST+EWC) *E-3 (RST+EWC)*E (RST+EWC) *E+3 ns
11 td(EMWAITH-EMOEH)Delay time from EMIF_nWAIT deasserted to
EMIF_nOE high3E-3 4E 4E+5 ns
29 tsu(EMDQMV-EMOEL)Output setup time, EMIF_nDQM[1:0] valid to
EMIF_nOE low(RS)*E-5 (RS)*E (RS)*E+3 ns
30 th(EMOEH-EMDQMIV)Output hold time, EMIF_nOE high to
EMIF_nDQM[1:0] invalid(RH)*E-4 (RH)*E (RH)*E+5 ns
Writes
15 tc(EMWCYCLE) EMIF write cycle time (EW =0) (WS+WST+WH)* E-3 (WS+WST+WH)*E (WS+WST+WH)* E+3 ns
EMIF write cycle time (EW =1)(WS+WST+WH+
EWC)*E -3(WS+WST+WH+
EWC)*E(WS+WST+WH+
EWC)*E +3ns

<!-- Page 113 -->
113TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-36. EMIF Asynchronous Memory Switching Characteristics(1)(2)(3)(continued)
NO. PARAMETER MIN TYP MAX UNIT
16 tsu(EMCEL-EMWEL)Output setup time, EMIF_nCS[4:2] lowto
EMIF_nWE low(SS=0)(WS)*E -3 (WS)*E (WS)*E +3 ns
Output setup time, EMIF_nCS[4:2] lowto
EMIF_nWE low(SS=1)-3 0 3 ns
17 th(EMWEH-EMCEH)Output hold time, EMIF_nWE high to
EMIF_nCS[4:2] high (SS=0)(WH)*E-3 (WH)*E (WH)*E+3 ns
Output hold time, EMIF_nWE high to
EMIF_CS[4:2] high (SS=1)-3 0 3 ns
18 tsu(EMDQMV-EMWEL)Output setup time, EMIF_nDQM[1:0] valid to
EMIF_nWE low(WS)*E-3 (WS)*E (WS)*E+3 ns
19 th(EMWEH-EMDQMIV)Output hold time, EMIF_nWE high to
EMIF_nDQM[1:0] invalid(WH)*E-3 (WH)*E (WH)*E+3 ns
20 tsu(EMBAV-EMWEL)Output setup time, EMIF_BA[1:0] valid to
EMIF_nWE low(WS)*E-3 (WS)*E (WS)*E+3 ns
21 th(EMWEH-EMBAIV)Output hold time, EMIF_nWE high to
EMIF_BA[1:0] invalid(WH)*E-3 (WH)*E (WH)*E+3 ns
22 tsu(EMAV-EMWEL)Output setup time, EMIF_ADDR[21:0] valid to
EMIF_nWE low(WS)*E-3 (WS)*E (WS)*E+3 ns
23 th(EMWEH-EMAIV)Output hold time, EMIF_nWE high to
EMIF_ADDR[21:0] invalid(WH)*E-3 (WH)*E (WH)*E+3 ns
24 tw(EMWEL) EMIF_nWE active lowwidth (EW =0) (WST)*E-3 (WST)*E (WST)*E+3 ns
EMIF_nWE active lowwidth (EW =1) (WST+EWC) *E-3 (WST+EWC)*E (WST+EWC) *E+3 ns
25 td(EMWAITH-EMWEH)Delay time from EMIF_nWAIT deasserted to
EMIF_nWE high3E+3 4E 4E+14 ns
26 tsu(EMDV-EMWEL)Output setup time, EMIF_DATA[15:0] valid to
EMIF_nWE low(WS)*E-3 (WS)*E (WS)*E+3 ns
27 th(EMWEH-EMDIV)Output hold time, EMIF_nWE high to
EMIF_DATA[15:0] invalid(WH)*E-3 (WH)*E (WH)*E+3 ns

<!-- Page 114 -->
EMIF_CLK
EMIF_BA[1:0]
EMIF_ADDR[21:0]
EMIF_DATA[15:0]1
2 2
4
6
8
8
12
1419
203
5
7
7
11
1317 182 EM_CLK DelayBASIC SDRAM
READ OPERATION
EMIF_nCS[0]
EMIF_nDQM[1:0]
EMIF_nRAS
EMIF_nCAS
EMIF_nWE
114TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.14.2.4 Read Timing (Synchronous RAM)
Figure 6-15. Basic SDRAM Read Operation

<!-- Page 115 -->
EMIF_CLK
EMIF_BA[1:0]
EMIF_ADDR[21:0]
EMIF_DATA[15:0]1
2 2
4
6
8
8
1210
163
5
7
7
11
13
159BASIC SDRAM
WRITE OPERATION
EMIF_CS[0]
EMIF_DQM[1:0]
EMIF_nRAS
EMIF_nCAS
EMIF_nWE
115TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.14.2.5 Write Timing (Synchronous RAM)
Figure 6-16. Basic SDRAM Write Operation
EMIF Synchronous Memory Timing
Table 6-37. EMIF Synchronous Memory Timing Requirements
NO. MIN MAX UNIT
19 tsu(EMIFDV-EM_CLKH)Input setup time, read data valid on
EMIF_DATA[15:0] before EMIF_CLK rising1 ns
20 th(CLKH-DIV)Input hold time, read data valid on
EMIF_DATA[15:0] after EMIF_CLK rising2.2 ns
Table 6-38. EMIF Synchronous Memory Switching Characteristics
NO. PARAMETER MIN MAX UNIT
1 tc(CLK) Cycle time, EMIF clock EMIF_CLK 10 ns
2 tw(CLK) Pulse width, EMIF clock EMIF_CLK high orlow 3 ns
3 td(CLKH-CSV) Delay time, EMIF_CLK rising toEMIF_nCS[0] valid 7 ns
4 toh(CLKH-CSIV) Output hold time, EMIF_CLK rising toEMIF_nCS[0] invalid 1 ns
5 td(CLKH-DQMV) Delay time, EMIF_CLK rising toEMIF_nDQM[1:0] valid 7 ns
6 toh(CLKH-DQMIV) Output hold time, EMIF_CLK rising toEMIF_nDQM[1:0] invalid 1 ns
7 td(CLKH-AV)Delay time, EMIF_CLK rising toEMIF_ADDR[21:0] andEMIF_BA[1:0]
valid7 ns
8 toh(CLKH-AIV)Output hold time, EMIF_CLK rising toEMIF_ADDR[21:0] and
EMIF_BA[1:0] invalid1 ns
9 td(CLKH-DV) Delay time, EMIF_CLK rising toEMIF_DATA[15:0] valid 7 ns

<!-- Page 116 -->
116TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-38. EMIF Synchronous Memory Switching Characteristics (continued)
NO. PARAMETER MIN MAX UNIT
10 toh(CLKH-DIV) Output hold time, EMIF_CLK rising toEMIF_DATA[15:0] invalid 1 ns
11 td(CLKH-RASV) Delay time, EMIF_CLK rising toEMIF_nRAS valid 7 ns
12 toh(CLKH-RASIV) Output hold time, EMIF_CLK rising toEMIF_nRAS invalid 1 ns
13 td(CLKH-CASV) Delay time, EMIF_CLK rising toEMIF_nCAS valid 7 ns
14 toh(CLKH-CASIV) Output hold time, EMIF_CLK rising toEMIF_nCAS invalid 1 ns
15 td(CLKH-WEV) Delay time, EMIF_CLK rising toEMIF_nWE valid 7 ns
16 toh(CLKH-WEIV) Output hold time, EMIF_CLK rising toEMIF_nWE invalid 1 ns
17 tdis(CLKH-DHZ) Delay time, EMIF_CLK rising toEMIF_DATA[15:0] tri-stated 7 ns
18 tena(CLKH-DLZ) Output hold time, EMIF_CLK rising toEMIF_DATA[15:0] driving 1 ns