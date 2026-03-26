# General-Purpose Input/Output (GIO) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1183-1209

---


<!-- Page 1183 -->

1183 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) ModuleChapter 25
SPNU563A -March 2018
General-Purpose Input/Output (GIO) Module
This chapter describes thegeneral-purpose input/output (GIO) module. The GIO module provides the
family ofdevices with input/output (I/O) capability. The I/Opins arebidirectional and bit-programmable.
The GIO module also supports external interrupt capability.
Topic ........................................................................................................................... Page
25.1 Overview ........................................................................................................ 1184
25.2 Quick Start Guide ........................................................................................... 1185
25.3 Functional Description ofGIOModule ............................................................... 1187
25.4 Device Modes ofOperation .............................................................................. 1190
25.5 GIOControl Registers ..................................................................................... 1191
25.6 I/OControl Summary ....................................................................................... 1209

<!-- Page 1184 -->

Overview www.ti.com
1184 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.1 Overview
The GIO module offers general-purpose input andoutput capability. Itsupports uptoeight 8-bit ports fora
total ofupto64GIO terminals. Each ofthese 64terminals canbeindependently configured asinput or
output andconfigured asrequired bytheapplication. The GIO module also supports generation of
interrupts whenever arising edge orfalling edge oranytoggle isdetected onupto32ofthese GIO
terminals. Refer tothedevice datasheet foridentifying thenumber ofGIO ports supported andtheGIO
terminals capable ofgenerating aninterrupt.
The main features oftheGIO module aresummarized asfollows:
*Allows each GIO terminal tobeconfigured forgeneral-purpose input oroutput functions
*Supports programmable pulldirections oneach input GIO terminal
*Supports GIO output inpush/pull oropen-drain modes
*Allows upto32GIO terminals tobeused forgenerating interrupt requests

<!-- Page 1185 -->

Enable pull?Power-On Reset
Configure as input/output?
Clear corresponding bits in GIODIR to 0 Set corresponding bit s in GIODIR to 1
Open drain?
Pull up/down?
DownOutput 1 or 0?Release Peripheral Reset by setting PENA bit in
Clock Control Register (0xFFFFFFD0)
Enable clock to GIO through PCR
 (Check device datasheet for the peripheral select) 
Bring GIO out of reset by writing 1 to GIOGCR0
Input Output
No Yes No Yes
Set corresponding bits in
 to 1 GIOPULDISClear corresponding bits in
 to 0 GIOPULDISSet corresponding bits in
 to 1 GIOPDRClear corresponding bits in
 to 0 GIOPDR
1 0Clear corresponding
bits in GIOPSL to 0  Set corresponding bits
in  to GIOPSL to 1
Read corresponding bits in GIODIN, getting input value  Write 1 to corresponding bits
in   GIODSETWrite 1 to  corresponding bits
in   GIODCLR
www.ti.com Quick Start Guide
1185 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.2 Quick Start Guide
The GIO module comprises twoseparate components: aninput/output (I/O) block andaninterrupt
generation block. Figure 25-1 andFigure 25-2 show what youshould doafter reset toconfigure theGIO
module asI/Oorforgenerating interrupts.
InGIO interrupt service routine, youshall read theGIO offset register (GIOOFF1 orGIOOFF2, depending
onhigh-/low-level interrupt) toclear theflagandfindthepending interrupt GIO channel.
Figure 25-1. I/OFunction Quick Start Flow Chart

<!-- Page 1186 -->

Power-On Reset
Enable GIO through PCR (Check device  datasheet for the periph eral select)
Both rising and falling edge / single edge trigger interrupt ?
Set corresponding bits in   to 1 GIOINTDET Clear corresponding bits in   to 0 GIOINTDET
Rising/Falling edge?Enable the FIQ/IRQ interrupt in CPU (Check CPU User Guide)
Bring GIO out of reset (See register  ) GIOGCR0Enable the FIQ/IRQ interrupt in  VIM (Check VIM User Guide)
Configure as high /low level interrupt?
Write 1 to corresponding bits in  GIOLVLSET Write 1 to corresponding bits in  GIOLVLCLRLow level High level
Write 1 to corresponding bits in  to enable interrupt GIOENASET Write 0xFF to clean the GIO interrupt flag register  GIOFLGEnable Peripherals by setting PENA bit in Clock Control Register (0 xFFFFFFD0)
Initialize vector interrupt table - Map GIO low level interrupt  and / or high level
interrupt service routine to pre-defined device specific interrupt channel. 
(Check device datasheet)   
Both edge Single edge
Rising Falling
Set corresponding bits in
 to 1 GIOPOLClear corresponding bits in
to 0 GIOPOL 
Quick Start Guide www.ti.com
1186 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) ModuleFigure 25-2. Interrupt Generation Function Quick Start Flow Chart

<!-- Page 1187 -->

PIN
CONTROL 
BLOCK
INTERRUPT 
CONTROL 
BLOCKxxxxGIOx[7:0] port
Host InterfaceGIO Module
Interrupt RequestsTo Interrupt Manager
www.ti.com Functional Description ofGIO Module
1187 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.3 Functional Description ofGIOModule
Asshown inFigure 25-3,theGIO module comprises oftwoseparate components: aninput/output (I/O)
block andaninterrupt block.
Figure 25-3. GIOModule Diagram
25.3.1 I/OFunctions
The I/Oblock allows each GIO terminal tobeconfigured foruseasageneral-purpose input oroutput in
theapplication. The GIO module supports multiple registers tocontrol thevarious aspects oftheinput and
output functions. These aredescribed asfollows.
*Data direction (GIODIR)
Configures GIO terminal(s) asinput (default) oroutput through theGIODIRx registers.
*Data input (GIODIN)
Reflects thelogic level onGIO terminals intheGIODINx registers. Ahigh voltage (VIHorgreater)
applied tothepincauses ahigh value (1)inthedata input register (GIODIN[7:0]). When alowvoltage
(VILorless) isapplied tothepin,thedata input register reads alowvalue (0).The VIHandVILvalues
aredevice specific andcanbefound inthedevice datasheet.
*Data output (GIODOUT)
Configures thelogic level tobeoutput onGIO terminal(s) configured asoutputs. Alowvalue (0)written
tothedata output register forces thepintoalowoutput voltage (VOLorlower). Ahigh value (1)written
tothedata output register (GIODOUTx) forces thepintoahigh output voltage (VOHorhigher) ifthe
open drain functionality isdisabled (GIOPDRx[7:0]). Ifopen drain functionality isenabled, ahigh value
(1)written tothedata output register forces thepintoahigh-impedance state (Z).
*Data set(GIODSET)
Allows logic HIGH tobeoutput onGIO terminal(s) configured asoutputs bywriting 1'stotherequired
bitsintheGIODSETx registers. Ifopen drain functionality isenabled, ahigh value (1)written tothe
data output register forces thepintoahigh-impedance state (Z).The GIODSETx registers eliminate
theneed fortheapplication toperform aread-modify-write operation when itneeds tosetoneormore
GIO pin(s).
*Data clear (GIODCLR)
Allows logic LOW tobeoutput onGIO terminal(s) configured asoutputs bywriting 1stotherequired
bitsintheGIODCLRx registers. The GIODCLRx registers eliminate theneed fortheapplication to
perform aread-modify-write operation when itneeds toclear oneormore GIO pin(s).
*Open drain (GIOPDR)
Open drain functionality isenabled ordisabled (default) using theopen drain register GIOPDR[7:0]
register. Ifopen-drain mode output isenabled onapin,ahigh value (1)written tothedata output
register (GIODOUTx[7:0]) forces thepintoahigh impedance state (Z).

<!-- Page 1188 -->

Functional Description ofGIO Module www.ti.com
1188 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module*Pulldisable (GIOPULDIS)
Disables theinternal pullonGIO terminal(s) configured asinputs bywriting totheGIOPULDISx
registers.
*Pullselect (GIOPSL)
Selects internal pulldown (default) orpulluponGIO terminal(s) configured asinputs bywriting tothe
GIOPULSELx registers.
Refer tothespecific device's datasheet toidentify thenumber ofGIO ports aswell astheinput andoutput
functions supported. Some devices may notsupport theprogrammable pullcontrols. Inthatcase, thepull
disable andthepullselect register controls willnotwork.
25.3.2 Interrupt Function
The GIO module supports upto32terminals tobeconfigured forgenerating aninterrupt tothehost
processor through theVectored Interrupt Manager (VIM). The main functions oftheinterrupt block are:
*Select theGIO pin(s) thatis/are used togenerate interrupt(s)
This isdone viatheinterrupt enable setandclear registers, GIOENASET andGIOENACLR.
*Select theedge ontheselected GIO pin(s) thatis/are used togenerate interrupt(s): rising/falling/both
Rising orfalling edge canbeselected viatheGIOPOL register. Ifinterrupt isrequired tobegenerated
onboth rising andfalling edges, thiscanbeconfigured viatheGIOINTDET register.
*Select theinterrupt priority
Low- orhigh-level interrupt canbeselected through theGIOLVLSET andGIOLVLCLR registers.
*Individual interrupt flags aresetintheGIOFLG register
The terminals onGIO ports Athrough Dareallinterrupt-capable andcanbeused tohandle either general
I/Ofunctions orinterrupt requests. Each interrupt request canbeconnected totheVIM atoneoftwo
different levels -High (orA)andLow (orB),depending ontheVIM channel number. The VIM hasan
inherent priority scheme sothatarequest onalower number channel hasahigher priority than arequest
onahigher number channel. Refer thedevice datasheet toidentify theVIM channel numbers fortheGIO
level Aandlevel Binterrupt requests. Also note thattheinterrupt priority oflevel Aandlevel Binterrupt
handling blocks canbere-programmed intheVIM.
25.3.3 GIO Block Diagram
The GIO block diagram (Figure 25-4)represents theflow ofinformation through apin.The shaded area
corresponds totheI/Oblock; theunshaded area corresponds totheinterrupt block.

<!-- Page 1189 -->

High-levelLow-level
Interrupt enableInterrupt disable
GIOFLG      GIOPOLRising edgeFalling edge
 VBUSP (peripheral bus)GIODSETx
GIODOUTx
GIODINxGIODIRx
external pin
GIODCLRxGIOPDRxGIOPULDISPull Select
Pull DisableGIOPSL
GIOINTDETGIOENASET
GIOENACLRGIOLVLSET
GIOLVLCLRLow-level
(level B)
interrupt
handling (1)
High-level
(level A)
interrupt
handling (1)To
 VIM
To
 VIM
www.ti.com Functional Description ofGIO Module
1189 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) ModuleFigure 25-4. GIOBlock Diagram
(1) Asingle low-level-interrupt-handling block andasingle high-level-interrupt-handling block service allofthe
interrupt-capable external pins, butonly onepincanbeserviced byaninterrupt block atatime.

<!-- Page 1190 -->

Device Modes ofOperation www.ti.com
1190 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.4 Device Modes ofOperation
The GIO module behaves differently indifferent modes ofoperation. There aretwomain modes:
*Emulation mode
*Power-down mode (low-power mode)
25.4.1 Emulation Mode
Emulation mode isused bydebugger tools tostop theCPU atbreakpoints toread registers.
NOTE: Emulation Mode andEmulation Registers
Emulation mode isamode ofoperation ofthedevice andisseparate from theGIO
emulation registers (GIOEMU1 andGIOEMU2). The contents ofthese emulation registers
areidentical tothecontents ofGIO offset registers (GIOOFF1 andGIOOFF2). Both
emulation registers andGIO offset registers areNOT cleared when they areread in
emulation mode. GIO offset registers arecleared when they areread innormal mode (other
than emulation mode). The emulation registers areNOT cleared when they areread in
normal mode. The intention fortheemulation registers isthatsoftware canusethem without
clearing theflags.
During emulation mode:
*External interrupts arenotcaptured because theVIM isunable toservice interrupts.
*Any register canberead without affecting thestate ofthesystem.
*Awrite toaregister stilldoes affect thestate ofthesystem.
25.4.2 Power-Down Mode (Low-Power Mode)
Inpower-down mode, theclock signal totheGIO module isdisabled. Thus, there isnoswitching andthe
only current draw comes from leakage current. Inpower-down mode, interrupt pins become level-sensitive
rather than edge-sensitive. The polarity bitchanges function from falling-edge-triggered tolow-level-
triggered andrising-edge-triggered tohigh-level-triggered. Acorresponding level onaninterrupt pinpulls
themodule outoflow-power mode, iftheinterrupt isalso enabled towake upthedevice outofalow-
power mode.
25.4.2.1 Module-Level Power Down
The GIO module canbeplaced intoapower down state bydisabling theGIO peripheral module viathe
appropriate bitintheperipheral power down register. Please refer tothePeripheral Central Resource
Registers (Section 2.5.3 )fordetails.
25.4.2.2 Device-Level Power Down
The entire device canbeplaced inoneofthepre-defined low-power modes: doze, snooze, orsleep using
theclock source andclock domain disable registers inthesystem module.

<!-- Page 1191 -->

www.ti.com GIO Control Registers
1191 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5 GIOControl Registers
Table 25-1 shows thesummary oftheGIO registers. The registers areaccessible in8-,16-, and32-bit
reads orwrites.
The start address fortheGIO module isFFF7 BC00h.
The GIO module supports upto8ports. Refer toyour device-specific data manual toidentify theactual
number ofGIO ports andthenumber ofpins ineach GIO port implemented onthisdevice.
The GIO module supports upto4interrupt-capable ports. Refer tothedevice datasheet toidentify the
actual number ofinterrupt-capable GIO ports andthenumber ofpins ineach GIO port implemented on
thisdevice.
Table 25-1. GIOControl Registers
Offset Acronym Register Description Section
00h GIOGCR0 GIO Global Control Register Section 25.5.1
08h GIOINTDET GIO Interrupt Detect Register Section 25.5.2
0Ch GIOPOL GIO Interrupt Polarity Register Section 25.5.3
10h GIOENASET GIO Interrupt Enable SetRegister Section 25.5.4.1
14h GIOENACLR GIO Interrupt Enable Clear Register Section 25.5.4.2
18h GIOLVLSET GIO Interrupt Priority SetRegister Section 25.5.5.1
1Ch GIOLVLCLR GIO Interrupt Priority Clear Register Section 25.5.5.2
20h GIOFLG GIO Interrupt Flag Register Section 25.5.6
24h GIOOFF1 GIO Offset 1Register Section 25.5.7
28h GIOOFF2 GIO Offset 2Register Section 25.5.8
2Ch GIOEMU1 GIO Emulation 1Register Section 25.5.9
30h GIOEMU2 GIO Emulation 2Register Section 25.5.10
34h GIODIRA GIO Data Direction Register Section 25.5.11
38h GIODINA GIO Data Input Register Section 25.5.12
3Ch GIODOUTA GIO Data Output Register Section 25.5.13
40h GIODSETA GIO Data SetRegister Section 25.5.14
44h GIODCLRA GIO Data Clear Register Section 25.5.15
48h GIOPDRA GIO Open Drain Register Section 25.5.16
4Ch GIOPULDISA GIO PullDisable Register Section 25.5.17
50h GIOPSLA GIO PullSelect Register Section 25.5.18
54h GIODIRB GIO Data Direction Register Section 25.5.11
58h GIODINB GIO Data Input Register Section 25.5.12
5Ch GIODOUTB GIO Data Output Register Section 25.5.13
60h GIODSETB GIO Data SetRegister Section 25.5.14
64h GIODCLRB GIO Data Clear Register Section 25.5.15
68h GIOPDRB GIO Open Drain Register Section 25.5.16
6Ch GIOPULDISB GIO PullDisable Register Section 25.5.17
70h GIOPSLB GIO PullSelect Register Section 25.5.18

<!-- Page 1192 -->

GIO Control Registers www.ti.com
1192 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.1 GIO Global Control Register (GIOGCR0)
The GIOGCR0 register contains onebitthatcontrols themodule reset status. Writing a0tothisbitputs
themodule inareset state. After system reset, thisbitmust besetto1before configuring anyother
register oftheGIO module. Figure 25-5 andTable 25-2 describe thisregister.
Figure 25-5. GIOGlobal Control Register (GIOGCR0) [offset =00h]
31 16
Reserved
R-0
15 1 0
Reserved RESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 25-2. GIOGlobal Control Register (GIOGCR0) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RESET GIO reset.
0 The GIO isinreset state.
1 The GIO isoperating normally.
NOTE: Note thatputting theGIO module inreset state isnotthesame asputting itinalow-power
state.

<!-- Page 1193 -->

www.ti.com GIO Control Registers
1193 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.2 GIO Interrupt Detect Register (GIOINTDET)
The GIO module supports generation ofaninterrupt request toCPU when arising edge, falling edge, or
both edges isdetected ononeormore GIO pin(s). The GIOINTDET register allows both rising andfalling
edges tobedetected, while theGIOPOL register allows theapplication todefine whether arising edge or
afalling edge istobedetected. Figure 25-6 andTable 25-3 describe thisregister.
Figure 25-6. GIOInterrupt Detect Register (GIOINTDET) [offset =08h]
31 24 23 16
GIOINTDET 3 GIOINTDET 2
R/W-0 R/W-0
15 8 7 0
GIOINTDET 1 GIOINTDET 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-3. GIOInterrupt Detect Register (GIOINTDET) Field Descriptions
Bit Field Value Description
31-24 GIOINTDET 3 Interrupt detection select forpins GIOD[7:0]
0 The flagsets oneither afalling orarising edge onthecorresponding pin,depending onthepolarity
setup inthepolarity register (GIOPOL).
1 The flagsets onboth therising andfalling edges onthecorresponding pin.
23-16 GIOINTDET 2 Interrupt detection select forpins GIOC[7:0]
0 The flagsets oneither afalling orarising edge onthecorresponding pin,depending onthepolarity
setup inthepolarity register (GIOPOL).
1 The flagsets onboth therising andfalling edges onthecorresponding pin.
15-8 GIOINTDET 1 Interrupt detection select forpins GIOB[7:0]
0 The flagsets oneither afalling orarising edge onthecorresponding pin,depending onthepolarity
setup inthepolarity register (GIOPOL).
1 The flagsets onboth therising andfalling edges onthecorresponding pin.
7-0 GIOINTDET 0 Interrupt detection select forpins GIOA[7:0]
0 The flagsets oneither afalling orarising edge onthecorresponding pin,depending onthepolarity
setup inthepolarity register (GIOPOL).
1 The flagsets onboth therising andfalling edges onthecorresponding pin.

<!-- Page 1194 -->

GIO Control Registers www.ti.com
1194 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.3 GIO Interrupt Polarity Register (GIOPOL)
The GIOPOL register configures thepolarity oftheedge, rising edge orfalling edge, thatneeds tobe
detected. When thedevice isinlow-power mode, theGIOPOL register controls thelevel ,high orlow,
which willbedetected bytheGIO module. Figure 25-7 andTable 25-4 describe thisregister.
Figure 25-7. GIOInterrupt Polarity Register (GIOPOL) [offset =0Ch]
31 24 23 16
GIOPOL 3 GIOPOL 2
R/W-0 R/W-0
15 8 7 0
GIOPOL 1 GIOPOL 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-4. GIOInterrupt Polarity Register (GIOPOL) Field Descriptions
Bit Field Value Description
31-24 GIOPOL 3 Interrupt polarity select forpins GIOD[7:0]
Normal operation (user orprivileged mode):
0 The flagissetonthefalling edge onthecorresponding pin.
1 The flagissetontherising edge onthecorresponding pin.
Low-power mode (GIO module clocks off):
0 The interrupt istriggered onthelowlevel.
1 The interrupt istriggered onthehigh level.
23-16 GIOPOL 2 Interrupt polarity select forpins GIOC[7:0]
Normal operation (user orprivileged mode):
0 The flagissetonthefalling edge onthecorresponding pin.
1 The flagissetontherising edge onthecorresponding pin.
Low-power mode (GIO module clocks off):
0 The interrupt istriggered onthelowlevel.
1 The interrupt istriggered onthehigh level.
15-8 GIOPOL 1 Interrupt polarity select forpins GIOB[7:0]
Normal operation (user orprivileged mode):
0 The flagissetonthefalling edge onthecorresponding pin.
1 The flagissetontherising edge onthecorresponding pin.
Low-power mode (GIO module clocks off):
0 The interrupt istriggered onthelowlevel.
1 The interrupt istriggered onthehigh level.
7-0 GIOPOL 0 Interrupt polarity select forpins GIOA[7:0]
Normal operation (user orprivileged mode):
0 The flagissetonthefalling edge onthecorresponding pin.
1 The flagissetontherising edge onthecorresponding pin.
Low-power mode (GIO module clocks off):
0 The interrupt istriggered onthelowlevel.
1 The interrupt istriggered onthehigh level.

<!-- Page 1195 -->

www.ti.com GIO Control Registers
1195 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.4 GIO Interrupt Enable Registers (GIOENASET andGIOENACLR)
The GIOENASET andGIOENACLR registers control which interrupt-capable pins areactually configured
asinterrupts. Iftheinterrupt isenabled, therising edge, falling edge, orboth edges ontheselected pin
lead toaninterrupt request.
25.5.4.1 GIOENASET Register
Figure 25-8 andTable 25-5 describe thisregister.
NOTE: Enabling Interrupt attheDevice Level
The interrupt channel intheVectored Interrupt Manager (VIM) must beenabled forthe
interrupt request tobeforwarded totheCPU. Additionally, theARM CPU (CPSR bit7or6)
must becleared torespond tointerrupt requests (IRQ/FIQ).
Figure 25-8. GIOInterrupt Enable SetRegister (GIOENASET) [offset =10h]
31 24 23 16
GIOENASET 3 GIOENASET 2
R/W-0 R/W-0
15 8 7 0
GIOENASET 1 GIOENASET 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-5. GIOInterrupt Enable SetRegister (GIOENASET) Field Descriptions
Bit Field Value Description
31-24 GIOENASET 3 Interrupt enable forpins GIOD[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Enables theinterrupt.
23-16 GIOENASET 2 Interrupt enable forpins GIOC[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Enables theinterrupt.
15-8 GIOENASET 1 Interrupt enable forpins GIOB[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Enables theinterrupt.
7-0 GIOENASET 0 Interrupt enable forpins GIOA[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Enables theinterrupt.

<!-- Page 1196 -->

GIO Control Registers www.ti.com
1196 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.4.2 GIOENACLR Register
This register disables theinterrupt. Figure 25-9 andTable 25-6 describe thisregister.
Figure 25-9. GIOInterrupt Enable Clear Register (GIOENACLR) [offset =14h]
31 24 23 16
GIOENACLR 3 GIOENACLR 2
R/W-0 R/W-0
15 8 7 0
GIOENACLR 1 GIOENACLR 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-6. GIOInterrupt Enable Clear Register (GIOENACLR) Field Descriptions
Bit Field Value Description
31-24 GIOENACLR 3 Interrupt disable forpins GIOD[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Disables theinterrupt.
23-16 GIOENACLR 2 Interrupt disable forpins GIOC[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Disables theinterrupt.
15-8 GIOENACLR 1 Interrupt disable forpins GIOB[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Disables theinterrupt.
7-0 GIOENACLR 0 Interrupt disable forpins GIOA[7:0]
0 Read: The interrupt isdisabled.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt isenabled.
Write: Disables theinterrupt.

<!-- Page 1197 -->

www.ti.com GIO Control Registers
1197 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.5 GIO Interrupt Priority Registers (GIOLVLSET andGIOLVLCLR)
The GIOLVLSET andGIOLVLCLR registers configure theinterrupts ashigh-level (level A)orlow-level
(level B)going totheVectored Interrupt Manager (VIM). Each interrupt isindividually configured.
*The high-level interrupts arerecorded toGIOOFF1 andGIOEMU1.
*The low-level interrupts arerecorded toGIOOFF2 andGIOEMU2.
NOTE: The GIO module cangenerate twointerrupt requests. These areconnected totwoseparate
channels ontheVectored Interrupt Manager (VIM). The lower-numbered VIM channels are
higher priority. The GIO interrupt connected toalower-number channel isthehigh-level (also
called level A)GIO interrupt, while theGIO interrupt connected toahigher-number channel
isthelow-level (also called level B)GIO interrupt.
25.5.5.1 GIOLVLSET Register
The GIOLVLSET register isused toconfigure aninterrupt asahigh-level interrupt going totheVIM. An
interrupt canbeconfigured asahigh-level interrupt bywriting a1intothecorresponding bitofthe
GIOLVLSET register. Writing a0hasnoeffect. Figure 25-10 andTable 25-7 describe thisregister.
Figure 25-10. GIOInterrupt Priority Register (GIOLVLSET) [offset =18h]
31 16
GIOLVLSET 3 GIOLVLSET 2
R/W-0 R/W-0
15 8 7 0
GIOLVLSET 1 GIOLVLSET 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-7. GIOInterrupt Priority Register (GIOLVLSET) Field Descriptions
Bit Field Value Description
31-24 GIOLVLSET 3 GIO high-priority interrupt forpins GIOD[7:0].
0 Read: The interrupt isalow-level interrupt. The low-level interrupts arerecorded toGIOOFF2
andGIOEMU2.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
23-16 GIOLVLSET 2 GIO high-priority interrupt forpins GIOC[7:0].
0 Read: The interrupt isalow-level interrupt. The low-level interrupts arerecorded toGIOOFF2
andGIOEMU2.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
15-8 GIOLVLSET 1 GIO high-priority interrupt forpins GIOB[7:0].
0 Read: The interrupt isalow-level interrupt. The low-level interrupts arerecorded toGIOOFF2
andGIOEMU2.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.

<!-- Page 1198 -->

GIO Control Registers www.ti.com
1198 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) ModuleTable 25-7. GIOInterrupt Priority Register (GIOLVLSET) Field Descriptions (continued)
Bit Field Value Description
7-0 GIOLVLSET 0 GIO high-priority interrupt forpins GIOA[7:0].
0 Read: The interrupt isalow-level interrupt. The low-level interrupts arerecorded toGIOOFF2
andGIOEMU2.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.

<!-- Page 1199 -->

www.ti.com GIO Control Registers
1199 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.5.2 GIOLVLCLR Register
The GIOLVLCLR register isused toconfigure aninterrupt asalow-level interrupt going totheVIM. An
interrupt canbeconfigured asalow-level interrupt bywriting a1intothecorresponding bitofthe
GIOLVLCLR register. Writing a0hasnoeffect. Figure 25-11 andTable 25-8 describe thisregister.
Figure 25-11. GIOInterrupt Priority Register (GIOLVLCLR) [offset =1Ch]
31 16
GIOLVLCLR 3 GIOLVLCLR 2
R/W-0 R/W-0
15 8 7 0
GIOLVLCLR 1 GIOLVLCLR 0
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 25-8. GIOInterrupt Priority Register (GIOLVLCLR) Field Descriptions
Bit Field Value Description
31-24 GIOLVLCLR 3 GIO low-priority interrupt forpins GIOD[7:0]
0 Read: The interrupt isalow-level interrupt.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asalow-level interrupt. The low-level interrupts arerecorded to
GIOOFF2 andGIOEMU2.
23-16 GIOLVLCLR 2 GIO low-priority interrupt forpins GIOC[7:0]
0 Read: The interrupt isalow-level interrupt.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asalow-level interrupt. The low-level interrupts arerecorded to
GIOOFF2 andGIOEMU2.
15-8 GIOLVLCLR 1 GIO low-priority interrupt forpins GIOB[7:0]
0 Read: The interrupt isalow-level interrupt.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asalow-level interrupt. The low-level interrupts arerecorded to
GIOOFF2 andGIOEMU2.
7-0 GIOLVLCLR 0 GIO low-priority interrupt forpins GIOA[7:0]
0 Read: The interrupt isalow-level interrupt.
Write: Writing a0tothisbithasnoeffect.
1 Read: The interrupt issetasahigh-level interrupt. The high-level interrupts arerecorded to
GIOOFF1 andGIOEMU1.
Write: Sets theinterrupt asalow-level interrupt. The low-level interrupts arerecorded to
GIOOFF2 andGIOEMU2.

<!-- Page 1200 -->

GIO Control Registers www.ti.com
1200 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.6 GIO Interrupt Flag Register (GIOFLG)
The GIOFLG register contains flags indicating thatthetransition edge (assetinGIOINTDET andGIOPOL
registers) hasoccurred. The flagcanbecleared bytheCPU writing a1totheflagthatisset.The flagis
also cleared byreading theappropriate interrupt offset register (GIOOFF1 orGIOOFF2). Figure 25-12 and
Table 25-9 describe thisregister.
Figure 25-12. GIOInterrupt Flag Register (GIOFLG) [offset =20h]
31 24 23 16
GIOFLG 3 GIOFLG 2
R/W1C-0 R/W1C-0
15 8 7 0
GIOFLG 1 GIOFLG 0
R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Table 25-9. GIOInterrupt Flag Register (GIOFLG) Field Descriptions
Bit Field Value Description
31-24 GIOFLG 3 GIO flagforpins GIOD[7:0]
0 Read: Atransition hasnotoccurred since thelastclear.
Write: Writing a0tothisbithasnoeffect.
1 Read: The selected transition onthecorresponding pinhasoccurred.
Write: The corresponding bitiscleared to0.
Note: This bitisalso cleared byaread tothecorresponding bitintheappropriate offset
register.
23-16 GIOFLG 2 GIO flagforpins GIOC[7:0]
0 Read: Atransition hasnotoccurred since thelastclear.
Write: Writing a0tothisbithasnoeffect.
1 Read: The selected transition onthecorresponding pinhasoccurred.
Write: The corresponding bitiscleared to0.
Note: This bitisalso cleared byaread tothecorresponding bitintheappropriate offset
register.
15-8 GIOFLG 1 GIO flagforpins GIOB[7:0]
0 Read: Atransition hasnotoccurred since thelastclear.
Write: Writing a0tothisbithasnoeffect.
1 Read: The selected transition onthecorresponding pinhasoccurred.
Write: The corresponding bitiscleared to0.
Note: This bitisalso cleared byaread tothecorresponding bitintheappropriate offset
register.
7-0 GIOFLG 0 GIO flagforpins GIOA[7:0]
0 Read: Atransition hasnotoccurred since thelastclear.
Write: Writing a0tothisbithasnoeffect.
1 Read: The selected transition onthecorresponding pinhasoccurred.
Write: The corresponding bitiscleared to0.
Note: This bitisalso cleared byaread tothecorresponding bitintheappropriate offset
register.
NOTE: Aninterrupt flaggets setwhen theselected transition happens onthecorresponding GIO pin
regardless ofwhether theinterrupt generation isenabled ornot.Itisrecommended to
clear aflagbefore enabling theinterrupt generation foratransition onthecorresponding GIO
pin.

<!-- Page 1201 -->

www.ti.com GIO Control Registers
1201 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.7 GIO Offset Register 1(GIOOFF1)
The GIOOFF1 register provides anumerical offset value thatrepresents thepending external interrupt
with high priority. The offset value canbeused tolocate theposition oftheinterrupt routine inavector
table inapplication software. Figure 25-13 andTable 25-10 describe thisregister.
NOTE: Reading thisregister clears it,GIOEMU1 andthecorresponding flagbitintheGIOFLG
register. However, inemulation mode, aread tothisregister does notclear anyregister or
flag. Ifmore than oneGIO interrupts arepending, then reading theGIOOFF1 register will
change thecontents ofGIOOFF1 andGIOEMU1 toshow theoffset value forthenext
highest-priority pending interrupt. The application canchoose toservice allGIO interrupts
from thesame service routine bycontinuing toread theGIOOFF1 register until itreads
zeros.
Figure 25-13. GIOOffset 1Register (GIOOFF1) [offset =24h]
31 16
Reserved
R-0
15 6 5 0
Reserved GIOOFF1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 25-10. GIOOffset 1Register (GIOOFF1) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 GIOOFF1 GIO offset 1.These bitsindex thecurrently pending high-priority interrupt. This register andthe
flagbit(intheGIOFLG register) arealso cleared when thisregister isread, except inemulation
mode.
0 Nointerrupt ispending.
1h Interrupt 0(corresponding toGIOA0) ispending with ahigh priority.
: :
8h Interrupt 7(corresponding toGIOA7) ispending with ahigh priority.
9h Interrupt 8(corresponding toGIOB0) ispending with ahigh priority.
: :
10h Interrupt 16(corresponding toGIOB7) ispending with ahigh priority.
: :
20h Interrupt 32(corresponding toGIOD7) ispending with ahigh priority.
21h-3Fh Reserved

<!-- Page 1202 -->

GIO Control Registers www.ti.com
1202 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.8 GIO Offset BRegister (GIOOFF2)
The GIOOFF2 register provides anumerical offset value thatrepresents thepending external interrupt
with lowpriority. The offset value canbeused tolocate theposition oftheinterrupt routine inavector
table inapplication software. Figure 25-14 andTable 25-11 describe thisregister.
NOTE: Reading thisregister clears it,GIOEMU2 andthecorresponding flagbitintheGIOFLG
register. However, inemulation mode, aread tothisregister does notclear anyregister or
flag. Ifmore than oneGIO interrupts arepending, then reading theGIOOFF1 register will
change thecontents ofGIOOFF2 andGIOEMU2 toshow theoffset value forthenext
highest-priority pending interrupt. The application canchoose toservice allGIO interrupts
from thesame service routine bycontinuing toread theGIOOFF1 register until itreads
zeros.
Figure 25-14. GIOOffset 2Register (GIOOFF2) [offset =28h]
31 16
Reserved
R-0
15 6 5 0
Reserved GIOOFF2
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 25-11. GIOOffset 2Register (GIOOFF2) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 GIOOFF2 GIO offset 2.These bitsindex thecurrently pending low-priority interrupt. This register andthe
flagbit(intheGIOFLG register) arealso cleared when thisregister isread, except inemulation
mode.
0 Nointerrupt ispending.
1h Interrupt 0(corresponding toGIOA0) ispending with alowpriority.
: :
8h Interrupt 7(corresponding toGIOA7) ispending with alowpriority.
9h Interrupt 8(corresponding toGIOB0) ispending with alowpriority.
: :
10h Interrupt 16(corresponding toGIOB7) ispending with alowpriority.
: :
20h Interrupt 32(corresponding toGIOD7) ispending with alowpriority.
21h-3Fh Reserved

<!-- Page 1203 -->

www.ti.com GIO Control Registers
1203 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.9 GIO Emulation ARegister (GIOEMU1)
The GIOEMU1 register isaread-only register. The contents ofthisregister areidentical tothecontents of
GIOOFF1. The intention forthethisregister isthatsoftware canuseitwithout clearing theflags.
Figure 25-15 andTable 25-12 describe thisregister.
NOTE: The corresponding flagintheGIOFLG register isnotcleared when theGIOEMU1 register is
read.
Figure 25-15. GIOEmulation 1Register (GIOEMU1) [offset =2Ch]
31 16
Reserved
R-0
15 6 5 0
Reserved GIOEMU1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 25-12. GIOEmulation 1Register (GIOEMU1) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 GIOEMU1 GIO offset emulation 1.These bitsindex thecurrently pending high-priority interrupt. Noregister
orflagiscleared byreading thisregister.
0 Nointerrupt ispending.
1h Interrupt 0(corresponding toGIOA0) ispending with ahigh priority.
: :
8h Interrupt 7(corresponding toGIOA7) ispending with ahigh priority.
9h Interrupt 8(corresponding toGIOB0) ispending with ahigh priority.
: :
10h Interrupt 16(corresponding toGIOB7) ispending with ahigh priority.
: :
20h Interrupt 32(corresponding toGIOD7) ispending with ahigh priority.
21h-3Fh Reserved

<!-- Page 1204 -->

GIO Control Registers www.ti.com
1204 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.10 GIO Emulation BRegister (GIOEMU2)
The GIOEMU2 register isaread-only register. The contents ofthisregister areidentical tothecontents of
GIOOFF2. The intention forthethisregister isthatsoftware canuseitwithout clearing theflags.
Figure 25-16 andTable 25-13 describe thisregister.
NOTE: The corresponding flagintheGIOFLG register isnotcleared when theGIOEMU2 register is
read.
Figure 25-16. GIOEmulation 2Register (GIOEMU2) [offset =30h]
31 16
Reserved
R-0
15 6 5 0
Reserved GIOEMU2
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 25-13. GIOEmulation 2Register (GIOEMU2) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 GIOEMU2 GIO offset emulation 2.These bitsindex thecurrently pending low-priority interrupt. Noregister
orflagiscleared byreading thisregister.
0 Nointerrupt ispending.
1h Interrupt 0(corresponding toGIOA0) ispending with alowpriority.
: :
8h Interrupt 7(corresponding toGIOA7) ispending with alowpriority.
9h Interrupt 8(corresponding toGIOB0) ispending with alowpriority.
: :
10h Interrupt 16(corresponding toGIOB7) ispending with alowpriority.
: :
20h Interrupt 32(corresponding toGIOD7) ispending with alowpriority.
21h-3Fh Reserved

<!-- Page 1205 -->

www.ti.com GIO Control Registers
1205 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.11 GIO Data Direction Registers (GIODIR[A-B])
The GIODIR register controls whether thepins ofagiven port areconfigured asinputs oroutputs.
Figure 25-17 andTable 25-14 describe thisregister.
Figure 25-17. GIOData Direction Registers (GIODIR[A-B]) [offset =34h, 54h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIODIR[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-14. GIOData Direction Registers (GIODIR[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIODIR[ n] GIO data direction ofport n,pins [7:0]
0 The GIO pinisaninput. Note: Ifthepindirection issetasaninput, theoutput buffer istristated.
1 The GIO pinisanoutput.
25.5.12 GIO Data Input Registers (GIODIN[A-B])
Values intheGIODIN register reflect thecurrent state (high =1orlow=0)onthepins oftheport.
Figure 25-18 andTable 25-15 describe thisregister.
Figure 25-18. GIOData Input Registers (GIODIN[A-B]) [offset =38h, 58h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIODIN[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-15. GIOData Input Registers (GIODIN[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIODIN[ n] GIO data input forport n,pins [7:0]
0 The pinisatlogic low(0).
1 The pinisatlogic high (1).

<!-- Page 1206 -->

GIO Control Registers www.ti.com
1206 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.13 GIO Data Output Registers (GIODOUT[A-B])
Values intheGIODOUT register specify theoutput state (high =1orlow=0)ofthepins oftheport when
they areconfigured asoutputs. Figure 25-19 andTable 25-16 describe thisregister.
NOTE: Values intheGIODSET register setthedata output control register bitsto1regardless of
thecurrent value intheGIODOUT bits.
Figure 25-19. GIOData Output Registers (GIODOUT[A-B]) [offset =3Ch, 5Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved GIODOUT[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-16. GIOData Output Registers (GIODOUT[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIODOUT[ n] GIO data output ofport n,pins[7:0].
0 The pinisdriven tologic low(0).
1 The pinisdriven tologic high (1).
Note: Output isinhigh impedance state iftheGIOPDRx bit=1andGIODOUTx bit=1.
Note: GIOpinisplaced inoutput mode bysetting theGIODIRx bitto1.
25.5.14 GIO Data SetRegisters (GIODSET[A-B])
Values inthisregister setthedata output control register bitsto1regardless ofthecurrent value inthe
GIODOUT bits. The contents ofthisregister reflect thecontents ofGIODOUT. Figure 25-20 andTable 25-
17describe thisregister.
Figure 25-20. GIOData SetRegisters (GIODSET[A-B]) [offset =40h, 60h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIODSET[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-17. GIOData SetRegisters (GIODSET[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIODSET[ n] GIO data setforport n,pins[7:0]. This bitdrives theoutput ofGIO pinhigh.
0 Write: Writing a0hasnoeffect.
1 Write: The corresponding GIO pinisdriven tologic high (1).
Note: Thecurrent logic state oftheGIODOUT bitwillalso bedisplayed bythisbit.
Note: GIOpinisplaced inoutput mode bysetting theGIODIRx bitto1.

<!-- Page 1207 -->

www.ti.com GIO Control Registers
1207 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.15 GIO Data Clear Registers (GIODCLR[A-B])
Values inthisregister clear thedata output register (GIO Data Output Register [A-H]) bitto0regardless of
itscurrent value. The contents ofthisregister reflect thecontents ofGIODOUT. Figure 25-21 and
Table 25-18 describe thisregister.
Figure 25-21. GIOData Clear Registers (GIODCLR[A-B]) [offset =44h, 64h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIODCLR[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-18. GIOData Clear Registers (GIODCLR[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIODCLR[ n] GIO data clear forport n,pins[7:0]. This bitdrives theoutput ofGIO pinlow.
0 Write: Writing a0hasnoeffect.
1 Write: The corresponding GIO pinisdriven tologic low(0).
Note: Thecurrent logic state oftheGIODOUT bitwillalso bedisplayed bythisbit.
Note: GIOpinisplaced inoutput mode bysetting theGIODIRx bitto1.
25.5.16 GIO Open Drain Registers (GIOPDR[A-B])
Values inthisregister enable ordisable theopen drain capability ofthedata pins. Figure 25-22 and
Table 25-19 describe thisregister.
Figure 25-22. GIOOpen Drain Registers (GIOPDR[A-B]) [offset =48h, 68h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIOPDR[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-19. GIOOpen Drain Registers (GIOPDR[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIOPDR[ n] GIO open drain forport n,pins[7:0]
0 The GIO pinisconfigured inpush/pull (normal GIO) mode. The output voltage isVOLorlower if
GIODOUT bit=0andVOHorhigher ifGIODOUT bit=1.
1 The GIO pinisconfigured inopen drain mode. The GIODOUTx bitcontrols thestate oftheGIO
output buffer: GIODOUTx =0,theGIO output buffer isdriven low; GIODOUTx =1,theGIO output
buffer istristated.

<!-- Page 1208 -->

GIO Control Registers www.ti.com
1208 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.5.17 GIO Pull Disable Registers (GIOPULDIS[A-B])
Values inthisregister enable ordisable thepullcontrol capability ofthepins. Figure 25-23 andTable 25-
20describe thisregister.
Figure 25-23. GIOPullDisable Registers (GIOPULDIS[A-B]) [offset =4Ch, 6Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved GIOPULDIS[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-20. GIOPullDisable Registers (GIOPULDIS[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIOPULDIS[ n] GIO pulldisable forport n,pins[7:0]. Writes tothisbitwillonly take effect when theGIO pin
configured asaninput pin.
0 The pullfunctionality isenabled.
1 The pullfunctionality isdisabled.
Note: TheGIOpinisplaced ininput mode byclearing theGIODIRx bitto0.
25.5.18 GIO Pull Select Registers (GIOPSL[A-B])
Values inthisregister select thepulluporpulldown functionality ofthepins. Figure 25-24 andTable 25-
21describe thisregister.
Figure 25-24. GIOPullSelect Registers (GIOPSL[A-B]) [offset =50h, 70h]
31 16
Reserved
R-0
15 8 7 0
Reserved GIOPSL[7:0]
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 25-21. GIOPullSelect Registers (GIOPSL[A-B]) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 GIOPSL[ n] GIO pullselect forport n,pins[7:0]
0 The pulldown functionality isselect, when pullup/pull down logic isenabled.
1 The pullupfunctionality isselect, when pullup/pull down logic isenabled.
Note: Thepullup/pull down functionality isenabled byclearing corresponding bitin
GIOPULDIS to0.

<!-- Page 1209 -->

www.ti.com I/OControl Summary
1209 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedGeneral-Purpose Input/Output (GIO) Module25.6 I/OControl Summary
The behavior oftheoutput buffer andthepullcontrol issummarized inTable 25-22 .
(1)X=Don'tcare
(2)GIODIR =0forinput; =1foroutput
(3)See Section 25.5.16
(4)GIOPULDIS =0forenabling pullcontrol; =1fordisabling pullcontrol
(5)GIOPSL= 0forpull-down functionality; =1forpull-up functionality
(6)Ifopen drain isenabled, output buffer willbedisabled ifahigh level (1)isbeing output.Table 25-22. Output Buffer andPullControl Behavior forGIOPins
Module under
Reset?PinDirection
(GIODIR)(1)(2)Open Drain
Enable
(GIOPDR)(1)(3)PullDisable
(GIOPULDIS)(1)(4)PullSelect
(GIOPSL)(1)(5) PullControl Output Buffer(6)
Yes X X X X Enabled Disabled
No 0 X 0 0 Pulldown Disabled
No 0 X 0 1 Pullup Disabled
No 0 X 1 0 Disabled Disabled
No 0 X 1 1 Disabled Disabled
No 1 0 X X Disabled Enabled
No 1 1 X X Disabled Enabled