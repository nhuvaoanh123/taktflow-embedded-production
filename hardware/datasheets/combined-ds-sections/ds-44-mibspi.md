# Multi-Buffered SPI (MibSPI)

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 193-206 (14 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 193 -->
193TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.12 Multibuffered /Standard Serial Peripheral Interface
The MibSPI isahigh-speed synchronous serial input/output port that allows aserial bitstream of
programmed length (2to16bits) tobeshifted inandoutofthedevice ataprogrammed bit-transfer rate.
Typical applications fortheSPIinclude interfacing toexternal peripherals, such asI/Os, memories, display
drivers, andanalog-to-digital converters.
7.12.1 Features
Both Standard andMibSPI modules have thefollowing features:
*16-bit shift register
*Receive buffer register
*11-bit baud clock generator
*SPICLK can beinternally-generated (master mode) orreceived from anexternal clock source (slave
mode)
*Each word transferred canhave aunique format
*SPII/Os notused inthecommunication canbeused asdigital input/output signals
Table 7-31. MibSPI Configurations
MibSPIx/SPIx I/Os
MibSPI1 MIBSPI1SIMO[1:0], MIBSPI1SOMI[1:0], MIBSPI1CLK, MIBSPI1nCS[5:0], MIBSPI1nENA
MibSPI3 MIBSPI3SIMO, MIBSPI3SOMI, MIBSPI3CLK, MIBSPI3nCS[5:0], MIBSPI3nENA
MibSPI5 MIBSPI5SIMO[3:0], MIBSPI5SOMI[3:0], MIBSPI5CLK, MIBSPI5nCS[5:0], MIBSPI5nENA
MibSPI2 MIBSPI2SIMO,MIBSPI2SOMI,MIBSPI2CLK,MIBSPI2nCS[1:0],MIBSPI2nENA
MibSPI4 MIBSPI4SIMO,MIBSPI4SOMI,MIBSPI4CLK,MIBSPI4nCS[5:0],MIBSPI4nENA
7.12.2 MibSPI Transmit andReceive RAM Organization
The Multibuffer RAM iscomprised of256 buffers forMibSPI1 and 128 buffers forallother MibSPI. Each
entry intheMultibuffer RAM consists of4parts: a16-bit transmit field, a16-bit receive field, a16-bit
control field anda16-bit status field. The Multibuffer RAM canbepartitioned intomultiple transfer groups
with avariable number ofbuffers each.
Multibuffered RAM Transfer Groups
MibSPIx/SPIx
MODULESNOOFCHIP
SELECTSMIBSPIxnCS[x]NO.OFRAM
BUFFERSNO.OFTRANSFER
GROUPS
MibSPI1 6 MIBSPI1nCS[5:0] 256 8
MibSPI2 2 MIBSPI2nCS[1:0] 128 8
MibSPI3 6 MIBSPI3nCS[5:0] 128 8
MibSPI4 6 MIBSPI4nCS[5:0] 128 8
MibSPI5 6 MIBSPI5nCS[5:0] 128 8
7.12.3 MibSPI Transmit Trigger Events
Each ofthetransfer groups canbeconfigured individually. Foreach ofthetransfer groups atrigger event
andatrigger source canbechosen. Atrigger event canbeforexample arising edge orapermanent low
level ataselectable trigger source. Forexample, upto15trigger sources areavailable which canbeused
byeach transfer group.

<!-- Page 194 -->
194TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.12.3.1 MIBSPI1 Event Trigger Hookup
Table 7-32. MIBSPI1 Event Trigger Hookup
Event # TGxCTRL TRIGSRC[3:0] Trigger
Disabled 0000 Notrigger source
EVENT0 0001 GIOA[0]
EVENT1 0010 GIOA[1]
EVENT2 0011 GIOA[2]
EVENT3 0100 GIOA[3]
EVENT4 0101 GIOA[4]
EVENT5 0110 GIOA[5]
EVENT6 0111 GIOA[6]
EVENT7 1000 GIOA[7]
EVENT8 1001 N2HET1[8]
EVENT9 1010 N2HET1[10]
EVENT10 1011 N2HET1[12]
EVENT11 1100 N2HET1[14]
EVENT12 1101 N2HET1[16]
EVENT13 1110 N2HET1[18]
EVENT14 1111 Intern Tick counter
NOTE
ForN2HET1 trigger sources, theconnection totheMibSPI1 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HET1 module boundary). This way, a
trigger condition canbegenerated even iftheN2HET1 signal isnotselected tobeoutput on
thepad.
NOTE
ForGIOx trigger sources, theconnection totheMibSPI1 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
selecting theGIOx pinasanoutput pinplus selecting thepintobeaGIOx pin,orbydriving
theGIOx pinfrom anexternal trigger source. Ifthemux control module isused toselect
different functionality instead oftheGIOx signal, then care must betaken todisable GIOx
from triggering MibSPI1 transfers; there isnomultiplexing ontheinput connections.

<!-- Page 195 -->
195TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.12.3.2 MIBSPI2 Event Trigger Hookup
Table 7-33. MIBSPI2 Event Trigger Hookup
Event # TGxCTRL TRIGSRC[3:0] Trigger
Disabled 0000 Notrigger source
EVENT0 0001 GIOA[0]
EVENT1 0010 GIOA[1]
EVENT2 0011 GIOA[2]
EVENT3 0100 GIOA[3]
EVENT4 0101 GIOA[4]
EVENT5 0110 GIOA[5]
EVENT6 0111 GIOA[6]
EVENT7 1000 GIOA[7]
EVENT8 1001 N2HET1[8]
EVENT9 1010 N2HET1[10]
EVENT10 1011 N2HET1[12]
EVENT11 1100 N2HET1[14]
EVENT12 1101 N2HET1[16]
EVENT13 1110 N2HET1[18]
EVENT14 1111 Intern Tick counter
NOTE
ForN2HET1 trigger sources, theconnection totheMibSPI1 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HET1 module boundary). This way, a
trigger condition canbegenerated even iftheN2HET1 signal isnotselected tobeoutput on
thepad.
NOTE
ForGIOx trigger sources, theconnection totheMibSPI1 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
selecting theGIOx pinasanoutput pinplus selecting thepintobeaGIOx pin,orbydriving
theGIOx pinfrom anexternal trigger source. Ifthemux control module isused toselect
different functionality instead oftheGIOx signal, then care must betaken todisable GIOx
from triggering MibSPI1 transfers; there isnomultiplexing ontheinput connections.
7.12.3.3 MIBSPI3 Event Trigger Hookup
Table 7-34. MIBSPI3 Event Trigger Hookup
Event # TGxCTRL TRIGSRC[3:0] Trigger
Disabled 0000 Notrigger source
EVENT0 0001 GIOA[0]
EVENT1 0010 GIOA[1]
EVENT2 0011 GIOA[2]
EVENT3 0100 GIOA[3]
EVENT4 0101 GIOA[4]
EVENT5 0110 GIOA[5]
EVENT6 0111 GIOA[6]
EVENT7 1000 GIOA[7]
EVENT8 1001 H2ET1[8]

<!-- Page 196 -->
196TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 7-34. MIBSPI3 Event Trigger Hookup (continued)
Event # TGxCTRL TRIGSRC[3:0] Trigger
EVENT9 1010 N2HET1[10]
EVENT10 1011 N2HET1[12]
EVENT11 1100 N2HET1[14]
EVENT12 1101 N2HET1[16]
EVENT13 1110 N2HET1[18]
EVENT14 1111 Intern Tick counter
NOTE
ForN2HET1 trigger sources, theconnection totheMibSPI3 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HET1 module boundary). This way, a
trigger condition canbegenerated even iftheN2HET1 signal isnotselected tobeoutput on
thepad.
NOTE
ForGIOx trigger sources, theconnection totheMibSPI3 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
selecting theGIOx pinasanoutput pinplus selecting thepintobeaGIOx pin,orbydriving
theGIOx pinfrom anexternal trigger source. Ifthemux control module isused toselect
different functionality instead oftheGIOx signal, then care must betaken todisable GIOx
from triggering MibSPI3 transfers; there isnomultiplexing ontheinput connections.

<!-- Page 197 -->
197TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.12.3.4 MIBSPI4 Event Trigger Hookup
Table 7-35. MIBSPI4 Event Trigger Hookup
Event # TGxCTRL TRIGSRC[3:0] Trigger
Disabled 0000 Notrigger source
EVENT0 0001 GIOA[0]
EVENT1 0010 GIOA[1]
EVENT2 0011 GIOA[2]
EVENT3 0100 GIOA[3]
EVENT4 0101 GIOA[4]
EVENT5 0110 GIOA[5]
EVENT6 0111 GIOA[6]
EVENT7 1000 GIOA[7]
EVENT8 1001 N2HET1[8]
EVENT9 1010 N2HET1[10]
EVENT10 1011 N2HET1[12]
EVENT11 1100 N2HET1[14]
EVENT12 1101 N2HET1[16]
EVENT13 1110 N2HET1[18]
EVENT14 1111 Intern Tick counter
NOTE
ForN2HET1 trigger sources, theconnection totheMibSPI1 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HET1 module boundary). This way, a
trigger condition canbegenerated even iftheN2HET1 signal isnotselected tobeoutput on
thepad.
NOTE
ForGIOx trigger sources, theconnection totheMibSPI1 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
selecting theGIOx pinasanoutput pinplus selecting thepintobeaGIOx pin,orbydriving
theGIOx pinfrom anexternal trigger source. Ifthemux control module isused toselect
different functionality instead oftheGIOx signal, then care must betaken todisable GIOx
from triggering MibSPI1 transfers; there isnomultiplexing ontheinput connections.
7.12.3.5 MIBSPI5 Event Trigger Hookup
Table 7-36. MIBSPI5 Event Trigger Hookup
Event # TGxCTRL TRIGSRC[3:0] Trigger
Disabled 0000 Notrigger source
EVENT0 0001 GIOA[0]
EVENT1 0010 GIOA[1]
EVENT2 0011 GIOA[2]
EVENT3 0100 GIOA[3]
EVENT4 0101 GIOA[4]
EVENT5 0110 GIOA[5]
EVENT6 0111 GIOA[6]
EVENT7 1000 GIOA[7]
EVENT8 1001 N2HET1[8]

<!-- Page 198 -->
198TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 7-36. MIBSPI5 Event Trigger Hookup (continued)
EVENT9 1010 N2HET1[10]
EVENT10 1011 N2HET1[12]
EVENT11 1100 N2HET1[14]
EVENT12 1101 N2HET1[16]
EVENT13 1110 N2HET1[18]
EVENT14 1111 Intern Tick counter
NOTE
ForN2HET1 trigger sources, theconnection totheMibSPI5 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HET1 module boundary). This way, a
trigger condition canbegenerated even iftheN2HET1 signal isnotselected tobeoutput on
thepad.
NOTE
ForGIOx trigger sources, theconnection totheMibSPI5 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
selecting theGIOx pinasanoutput pin+selecting thepintobeaGIOx pin,orbydriving the
GIOx pinfrom anexternal trigger source. Ifthemux control module isused toselect different
functionality instead oftheGIOx signal, then care must betaken todisable GIOx from
triggering MibSPI5 transfers; there isnomultiplexing ontheinput connections.

<!-- Page 199 -->
199TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The MASTER bit(SPIGCR1.0) issetandtheCLOCK PHASE bit(SPIFMTx.16) iscleared.
(2) tc(VCLK) =interface clock cycle time =1/f(VCLK)
(3) Forriseandfalltimings, seeTable 5-5.
(4) When theSPIisinMaster mode, thefollowing must betrue:
ForPSvalues from 1to255: tc(SPC)M≥(PS+1)t c(VCLK)≥40ns, where PSistheprescale value setintheSPIFMTx.[15:8] register bits.
ForPSvalues of0:tc(SPC)M =2tc(VCLK)≥40ns.
The external load ontheSPICLK pinmust beless than 60pF.
(5) The active edge oftheSPICLK signal referenced iscontrolled bytheCLOCK POLARITY bit(SPIFMTx.17).
(6) C2TDELAY andT2CDELAY isprogrammed intheSPIDELAY register7.12.4 MibSPI/SPI Master Mode I/OTiming Specifications
Table 7-37. SPIMaster Mode External Timing Parameters (CLOCK PHASE =0,SPICLK =output, SPISIMO
=output, andSPISOMI =input)(1)(2)(3)
NO. Parameter MIN MAX Unit
1 tc(SPC)M Cycle time, SPICLK(4)40 256t c(VCLK) ns
2(5)tw(SPCH)M Pulse duration, SPICLK high (clock
polarity =0)0.5t c(SPC)M -tr(SPC)M -3 0.5t c(SPC)M +3 ns
tw(SPCL)M Pulse duration, SPICLK low(clock
polarity =1)0.5t c(SPC)M -tf(SPC)M -3 0.5t c(SPC)M +3
3(5)tw(SPCL)M Pulse duration, SPICLK low(clock
polarity =0)0.5t c(SPC)M -tf(SPC)M -3 0.5t c(SPC)M +3 ns
tw(SPCH)M Pulse duration, SPICLK high (clock
polarity =1)0.5t c(SPC)M -tr(SPC)M -3 0.5t c(SPC)M +3
4(5)td(SPCH-SIMO)M Delay time, SPISIMO valid before
SPICLK low(clock polarity =0)0.5t c(SPC)M -6 ns
td(SPCL-SIMO)M Delay time, SPISIMO valid before
SPICLK high (clock polarity =1)0.5t c(SPC)M -6
5(5)tv(SPCL-SIMO)M Valid time, SPISIMO data valid after
SPICLK low(clock polarity =0)0.5t c(SPC)M -tf(SPC) -4 ns
tv(SPCH-SIMO)M Valid time, SPISIMO data valid after
SPICLK high (clock polarity =1)0.5t c(SPC)M -tr(SPC) -4
6(5)tsu(SOMI-SPCL)M Setup time, SPISOMI before SPICLK
low(clock polarity =0)tf(SPC) +2.2 ns
tsu(SOMI-SPCH)M Setup time, SPISOMI before SPICLK
high (clock polarity =1)tr(SPC) +2.2
7(5)th(SPCL-SOMI)M Hold time, SPISOMI data valid after
SPICLK low(clock polarity =0)10 ns
th(SPCH-SOMI)M Hold time, SPISOMI data valid after
SPICLK high (clock polarity =1)10
8(6)tC2TDELAY Setup time CSactive
until SPICLK high
(clock polarity =0)CSHOLD =0C2TDELAY*t c(VCLK) +2*tc(VCLK)
-tf(SPICS) +tr(SPC) -7(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tr(SPC) +5.5ns
CSHOLD =1C2TDELAY*t c(VCLK) +3*tc(VCLK)
-tf(SPICS) +tr(SPC) -7(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tr(SPC) +5.5
Setup time CSactive
until SPICLK low
(clock polarity =1)CSHOLD =0C2TDELAY*t c(VCLK) +2*tc(VCLK)
-tf(SPICS) +tf(SPC) -7(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tf(SPC) +5.5ns
CSHOLD =1C2TDELAY*t c(VCLK) +3*tc(VCLK)
-tf(SPICS) +tf(SPC) -7(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tf(SPC) +5.5
9(6)tT2CDELAY Hold time SPICLK lowuntil CSinactive
(clock polarity =0)0.5*t c(SPC)M +
T2CDELAY*t c(VCLK) +tc(VCLK) -
tf(SPC) +tr(SPICS) -70.5*t c(SPC)M +
T2CDELAY*t c(VCLK) +tc(VCLK) -
tf(SPC) +tr(SPICS) +11ns
Hold time SPICLK high until CS
inactive (clock polarity =1)0.5*t c(SPC)M +
T2CDELAY*t c(VCLK) +tc(VCLK) -
tr(SPC) +tr(SPICS) -70.5*t c(SPC)M +
T2CDELAY*t c(VCLK) +tc(VCLK) -
tr(SPC) +tr(SPICS) +11ns
10 tSPIENA SPIENAn Sample point (C2TDELAY+1) *tc(VCLK) -
tf(SPICS) -29(C2TDELAY+1)*t c(VCLK) ns
11 tSPIENAW SPIENAn Sample point from write to
buffer(C2TDELAY+2)*t c(VCLK) ns

<!-- Page 200 -->
SPICLK
(clock polarity=0)
SPISIMO
SPICSnMaster Out Data Is Valid
9SPICLK
(clock polarity=1)
SPIENAn10Write to buffer
118
SPISOMISPISIMOSPICLK
(clock polarity = 1)SPICLK
(clock polarity = 0)
Master In Data
Must Be ValidMaster Out Data Is Valid321
5 4
66
7
200TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-19. SPIMaster Mode External Timing (CLOCK PHASE =0)
Figure 7-20. SPIMaster Mode Chip Select Timing (CLOCK PHASE =0)

<!-- Page 201 -->
201TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The MASTER bit(SPIGCR1.0) issetandtheCLOCK PHASE bit(SPIFMTx.16) isset.
(2) tc(VCLK) =interface clock cycle time =1/f(VCLK)
(3) Forriseandfalltimings, seetheTable 5-5.
(4) When theSPIisinMaster mode, thefollowing must betrue:
ForPSvalues from 1to255: tc(SPC)M≥(PS+1)t c(VCLK)≥40ns, where PSistheprescale value setintheSPIFMTx.[15:8] register bits.
ForPSvalues of0:tc(SPC)M =2tc(VCLK)≥40ns.
The external load ontheSPICLK pinmust beless than 60pF.
(5) The active edge oftheSPICLK signal referenced iscontrolled bytheCLOCK POLARITY bit(SPIFMTx.17).
(6) C2TDELAY andT2CDELAY isprogrammed intheSPIDELAY registerTable 7-38. SPIMaster Mode External Timing Parameters (CLOCK PHASE =1,SPICLK =output, SPISIMO
=output, andSPISOMI =input)(1)(2)(3)
NO. Parameter MIN MAX Unit
1 tc(SPC)M Cycle time, SPICLK(4)40 256t c(VCLK) ns
2(5)tw(SPCH)M Pulse duration, SPICLK high (clock
polarity =0)0.5t c(SPC)M -tr(SPC)M -3 0.5t c(SPC)M +3 ns
tw(SPCL)M Pulse duration, SPICLK low(clock
polarity =1)0.5t c(SPC)M -tf(SPC)M -3 0.5t c(SPC)M +3
3(5)tw(SPCL)M Pulse duration, SPICLK low(clock
polarity =0)0.5t c(SPC)M -tf(SPC)M -3 0.5t c(SPC)M +3 ns
tw(SPCH)M Pulse duration, SPICLK high (clock
polarity =1)0.5t c(SPC)M -tr(SPC)M -3 0.5t c(SPC)M +3
4(5)tv(SIMO-SPCH)M Valid time, SPICLK high after
SPISIMO data valid (clock polarity =
0)0.5t c(SPC)M -6 ns
tv(SIMO-SPCL)M Valid time, SPICLK lowafter
SPISIMO data valid (clock polarity =
1)0.5t c(SPC)M -6
5(5)tv(SPCH-SIMO)M Valid time, SPISIMO data valid after
SPICLK high (clock polarity =0)0.5t c(SPC)M -tr(SPC) -4 ns
tv(SPCL-SIMO)M Valid time, SPISIMO data valid after
SPICLK low(clock polarity =1)0.5t c(SPC)M -tf(SPC) -4
6(5)tsu(SOMI-SPCH)M Setup time, SPISOMI before
SPICLK high (clock polarity =0)tr(SPC) +2.2 ns
tsu(SOMI-SPCL)M Setup time, SPISOMI before
SPICLK low(clock polarity =1)tf(SPC) +2.2
7(5)tv(SPCH-SOMI)M Valid time, SPISOMI data valid after
SPICLK high (clock polarity =0)10 ns
tv(SPCL-SOMI)M Valid time, SPISOMI data valid after
SPICLK low(clock polarity =1)10
8(6)tC2TDELAY Setup time CS
active until SPICLK
high (clock polarity =
0)CSHOLD =0 0.5*t c(SPC)M +
(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tr(SPC) -70.5*t c(SPC)M +
(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tr(SPC) +5.5ns
CSHOLD =1 0.5*t c(SPC)M +
(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tr(SPC) -70.5*t c(SPC)M +
(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tr(SPC) +5.5
Setup time CS
active until SPICLK
low(clock polarity =
1)CSHOLD =0 0.5*t c(SPC)M +
(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tf(SPC) -70.5*t c(SPC)M +
(C2TDELAY+2) *tc(VCLK) -
tf(SPICS) +tf(SPC) +5.5ns
CSHOLD =1 0.5*t c(SPC)M +
(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tf(SPC) -70.5*t c(SPC)M +
(C2TDELAY+3) *tc(VCLK) -
tf(SPICS) +tf(SPC) +5.5
9(6)tT2CDELAY Hold time SPICLK lowuntil CS
inactive (clock polarity =0)T2CDELAY*t c(VCLK) +
tc(VCLK) -tf(SPC) +tr(SPICS) -
7T2CDELAY*t c(VCLK) +
tc(VCLK) -tf(SPC) +tr(SPICS) +
11ns
Hold time SPICLK high until CS
inactive (clock polarity =1)T2CDELAY*t c(VCLK) +
tc(VCLK) -tr(SPC) +tr(SPICS) -
7T2CDELAY*t c(VCLK) +
tc(VCLK) -tr(SPC) +tr(SPICS) +
11ns
10 tSPIENA SPIENAn Sample Point (C2TDELAY+1)* tc(VCLK) -
tf(SPICS) -29(C2TDELAY+1)*t c(VCLK) ns
11 tSPIENAW SPIENAn Sample point from write to
buffer(C2TDELAY+2)*t c(VCLK) ns

<!-- Page 202 -->
SPICLK
(clock polarity=0)
SPISIMO
SPICSnMaster Out Data Is Valid
9SPICLK
(clock polarity=1)
SPIENAn10Write to buffer
118
SPISOMISPISIMOSPICLK
(clock polarity = 1)SPICLK
(clock polarity = 0)
Data Valid
Master In Data
Must Be ValidMaster Out Data Is Valid321
5 4
7 6
202TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-21. SPIMaster Mode External Timing (CLOCK PHASE =1)
Figure 7-22. SPIMaster Mode Chip Select Timing (CLOCK PHASE =1)

<!-- Page 203 -->
203TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The MASTER bit(SPIGCR1.0) iscleared andtheCLOCK PHASE bit(SPIFMTx.16) iscleared.
(2) IftheSPIisinslave mode, thefollowing must betrue: tc(SPC)S≥(PS+1)tc(VCLK) ,where PS=prescale value setinSPIFMTx.[15:8].
(3) Forriseandfalltimings, seeTable 5-5.
(4) tc(VCLK) =interface clock cycle time =1/f(VCLK)
(5) When theSPIisinSlave mode, thefollowing must betrue:
ForPSvalues from 1to255: tc(SPC)S≥(PS+1)t c(VCLK)≥40ns, where PSistheprescale value setintheSPIFMTx.[15:8] register bits.
ForPSvalues of0:tc(SPC)S =2tc(VCLK)≥40ns.
(6) The active edge oftheSPICLK signal referenced iscontrolled bytheCLOCK POLARITY bit(SPIFMTx.17).7.12.5 SPISlave Mode I/OTimings
Table 7-39. SPISlave Mode External Timing Parameters (CLOCK PHASE =0,SPICLK =input, SPISIMO =
input, andSPISOMI =output)(1)(2)(3)(4)
NO. Parameter MIN MAX Unit
1 tc(SPC)S Cycle time, SPICLK(5)40 ns
2(6)tw(SPCH)S Pulse duration, SPICLK high (clock polarity =0) 14 ns
tw(SPCL)S Pulse duration, SPICLK low(clock polarity =1) 14
3(6)tw(SPCL)S Pulse duration, SPICLK low(clock polarity =0) 14 ns
tw(SPCH)S Pulse duration, SPICLK high (clock polarity =1) 14
4(6)td(SPCH-SOMI)S Delay time, SPISOMI valid after SPICLK high (clock
polarity =0)trf(SOMI) +20 ns
td(SPCL-SOMI)S Delay time, SPISOMI valid after SPICLK low(clock polarity
=1)trf(SOMI) +20
5(6)th(SPCH-SOMI)S Hold time, SPISOMI data valid after SPICLK high (clock
polarity =0)2 ns
th(SPCL-SOMI)S Hold time, SPISOMI data valid after SPICLK low(clock
polarity =1)2
6(6)tsu(SIMO-SPCL)S Setup time, SPISIMO before SPICLK low(clock polarity =
0)4 ns
tsu(SIMO-SPCH)S Setup time, SPISIMO before SPICLK high (clock polarity =
1)4
7(6)th(SPCL-SIMO)S Hold time, SPISIMO data valid after SPICLK low(clock
polarity =0)2 ns
th(SPCH-SIMO)S Hold time, SPISIMO data valid after SPICLK high (clock
polarity =1)2
8 td(SPCL-SENAH)S Delay time, SPIENAn high after lastSPICLK low(clock
polarity =0)1.5t c(VCLK) 2.5t c(VCLK) +tr(ENAn) +
22ns
td(SPCH-SENAH)S Delay time, SPIENAn high after lastSPICLK high (clock
polarity =1)1.5t c(VCLK) 2.5t c(VCLK) +tr(ENAn) +
22
9 td(SCSL-SENAL)S Delay time, SPIENAn lowafter SPICSn low(ifnew data
hasbeen written totheSPIbuffer)tf(ENAn) tc(VCLK) +tf(ENAn) +27 ns

<!-- Page 204 -->
SPICLK
(clock polarity=0)
SPICSn8SPICLK
(clock polarity=1)
SPIENAn
9
SPISOMISPICLK
(clock□polarity□=□1)SPICLK
(clock□polarity□=□0)
321
5
4
7
SPISIMO□Data
Must□Be□ValidSPISOMI□Data□Is□Valid
666
SPISIMO
204TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-23. SPISlave Mode External Timing (CLOCK PHASE =0)
Figure 7-24. SPISlave Mode Enable Timing (CLOCK PHASE =0)

<!-- Page 205 -->
205TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The MASTER bit(SPIGCR1.0) iscleared andtheCLOCK PHASE bit(SPIFMTx.16) isset.
(2) IftheSPIisinslave mode, thefollowing must betrue: tc(SPC)S ≤(PS+1)tc(VCLK), where PS=prescale value setinSPIFMTx.[15:8].
(3) Forriseandfalltimings, seeTable 5-5.
(4) tc(VCLK) =interface clock cycle time =1/f(VCLK)
(5) When theSPIisinSlave mode, thefollowing must betrue:
ForPSvalues from 1to255: tc(SPC)S≥(PS+1)t c(VCLK)≥40ns, where PSistheprescale value setintheSPIFMTx.[15:8] register bits.
ForPSvalues of0:tc(SPC)S =2tc(VCLK)≥40ns.
(6) The active edge oftheSPICLK signal referenced iscontrolled bytheCLOCK POLARITY bit(SPIFMTx.17).Table 7-40. SPISlave Mode External Timing Parameters (CLOCK PHASE =1,SPICLK =input, SPISIMO =
input, andSPISOMI =output)(1)(2)(3)(4)
NO. Parameter MIN MAX Unit
1 tc(SPC)S Cycle time, SPICLK(5)40 ns
2(6)tw(SPCH)S Pulse duration, SPICLK high (clock polarity =0) 14 ns
tw(SPCL)S Pulse duration, SPICLK low(clock polarity =1) 14
3(6)tw(SPCL)S Pulse duration, SPICLK low(clock polarity =0) 14 ns
tw(SPCH)S Pulse duration, SPICLK high (clock polarity =1) 14
4(6)td(SOMI-SPCL)S Dealy time, SPISOMI data valid after SPICLK low
(clock polarity =0)trf(SOMI) +20 ns
td(SOMI-SPCH)S Delay time, SPISOMI data valid after SPICLK high
(clock polarity =1)trf(SOMI) +20
5(6)th(SPCL-SOMI)S Hold time, SPISOMI data valid after SPICLK high
(clock polarity =0)2 ns
th(SPCH-SOMI)S Hold time, SPISOMI data valid after SPICLK low(clock
polarity =1)2
6(6)tsu(SIMO-SPCH)S Setup time, SPISIMO before SPICLK high (clock
polarity =0)4 ns
tsu(SIMO-SPCL)S Setup time, SPISIMO before SPICLK low(clock polarity
=1)4
7(6)tv(SPCH-SIMO)S High time, SPISIMO data valid after SPICLK high
(clock polarity =0)2 ns
tv(SPCL-SIMO)S High time, SPISIMO data valid after SPICLK low(clock
polarity =1)2
8 td(SPCH-SENAH)S Delay time, SPIENAn high after lastSPICLK high
(clock polarity =0)1.5t c(VCLK) 2.5t c(VCLK) +tr(ENAn) +22 ns
td(SPCL-SENAH)S Delay time, SPIENAn high after lastSPICLK low(clock
polarity =1)1.5t c(VCLK) 2.5t c(VCLK) +tr(ENAn) +22
9 td(SCSL-SENAL)S Delay time, SPIENAn lowafter SPICSn low(ifnew data
hasbeen written totheSPIbuffer)tf(ENAn) tc(VCLK) +tf(ENAn) +27 ns
10 td(SCSL-SOMI)S Delay time, SOMI valid after SPICSn low(ifnew data
hasbeen written totheSPIbuffer)tc(VCLK) 2tc(VCLK) +trf(SOMI) +28 ns

<!-- Page 206 -->
SPISOMISlave Out Data Is ValidSPICLK
(clock polarity=0)
SPICSn8SPICLK
(clock polarity=1)
SPIENAn
9
10
SPISIMOSPISOMI5
7
SPISIMO Data
Must Be ValidSPISOMI Data Is Valid
666SPICLK
(clock polarity = 1)SPICLK
(clock polarity = 0)
321
4
206TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-25. SPISlave Mode External Timing (CLOCK PHASE =1)
Figure 7-26. SPISlave Mode Enable Timing (CLOCK PHASE =1)