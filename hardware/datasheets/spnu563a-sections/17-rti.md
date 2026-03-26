# Real-Time Interrupt (RTI) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 583-624

---


<!-- Page 583 -->

583 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleChapter 17
SPNU563A -March 2018
Real-Time Interrupt (RTI) Module
This chapter describes thefunctionality ofthereal-time interrupt (RTI) module. The RTIisdesigned asan
operating system timer tosupport arealtime operating system (RTOS).
NOTE: This chapter describes asuperset implementation oftheRTImodule thatincludes features
andfunctionality related toDMA, FlexRay, andTimbase control. These features are
dependent onthedevice-specific feature content. Consult your device-specific datasheet to
determine theapplicability ofthese features toyour device being used.
Topic ........................................................................................................................... Page
17.1 Overview ......................................................................................................... 584
17.2 Module Operation ............................................................................................. 585
17.3 RTIControl Registers ........................................................................................ 595

<!-- Page 584 -->

Overview www.ti.com
584 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.1 Overview
The real-time interrupt (RTI) module provides timer functionality foroperating systems andfor
benchmarking code. The RTImodule canincorporate several counters thatdefine thetimebases needed
forscheduling intheoperating system.
The timers also allow youtobenchmark certain areas ofcode byreading thevalues ofthecounters atthe
beginning andtheendofthedesired code range andcalculating thedifference between thevalues.
Inaddition theRTIprovides amechanism tosynchronize theoperating system totheFlexRay
communication cycle. Clock supervision allows fordetection ofissues ontheFlexRay buswith an
automatic switch toaninternally generated timebase when afailure with theFlexRay timebase is
detected.
17.1.1 Features
The RTImodule hasthefollowing features:
*Two independent 64bitcounter blocks
*Four configurable compares forgenerating operating system ticks orDMA requests. Each event can
bedriven byeither counter block 0orcounter block 1.
*One counter block usable forapplication synchronization toFlexRay network including clock
supervision
*Fast enabling/disabling ofevents
*Two time stamp (capture) functions forsystem orperipheral interrupts, oneforeach counter block
*Digital windowed watchdog
17.1.2 Industry Standard Compliance Statement
This module isspecifically designed tofulfill therequirements forOSEK (Offene Systeme undderen
Schnittstellen fürdieElektronik imKraftfahrzeug, orOpen Systems andtheCorresponding Interfaces for
Automotive Electronics) aswell asOSEK/time-compliant operating systems, butisnotlimited toit.

<!-- Page 585 -->

fRTIFRCxfRTICLK
RTICPUCx + 1- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - when RTICPUCx 0 ≠
fRTICLK
2 +132- - - - - - - - - - - - - - - - - - - - when RTICPUCx = 0={
RTICLK
RTICLKFlexRay Macrotick (NTU0)
FlexRay Start of Cycle (NTU1)Counter Block 0
64-bit
incl. FlexRay Feature
Counter Block 1
64-bitEvent0VIM REQ[2]
DMA REQ[12]
Event1VIM REQ[3]
DMA REQ[13]
Event2VIM REQ[4]
DMA REQ[18]
Event3VIM REQ[5]
DMA REQ[19]32
32
32
3232
32Compare Unit
Capture FeatureCapture Feature NTU2
NTU3
www.ti.com Module Operation
585 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.2 Module Operation
Figure 17-1 illustrates thehigh level block diagram oftheRTImodule.
The RTImodule hastwoindependent counter blocks forgenerating different timebases: counter block 0
andcounter block 1.The twocounter blocks provide thesame basic functionality, butcounter block 0has
theadditional functionality ofbeing able towork with theFlexRay Macrotick (NTU0) orStart ofCycle
(NTU1) andperform clock supervision todetect amissing signal.
Acompare unitcompares thecounters with programmable values andgenerates four independent
interrupt orDMA requests oncompare matches. Each ofthecompare registers canbeprogrammed tobe
compared toeither counter block 0orcounter block 1.
The following sections describe theindividual functions inmore detail.
Figure 17-1. RTIBlock Diagram
17.2.1 Counter Operation
Each counter block consists ofthefollowing (see Figure 17-2):
*One 32-bit prescale counter (RTIUC0 orRTIUC1)
*One 32-bit free running counter (RTIFRC0 orRTIFRC1)
The RTIUC0/1 isdriven bytheRTICLK andcounts upuntil thecompare value inthecompare upcounter
register (RTICPUC0 orRTICPUC1) isreached. When thecompare matches, RTIFRC0/1 isincremented
andRTIUC0/1 isreset to0.IfRTIFRC0/1 overflows, aninterrupt isgenerated tothevectored interrupt
manager (VIM). The overflow interrupt isnotintended togenerate thetimebase fortheoperating system.
See Section 17.2.2 forthetimebase generation. The upcounter together with thecompare upcounter
value prescale theRTIclock. The resulting formula forthefrequency ofthefree running counter
(RTIFRC0/1) is:
(23)
NOTE: Setting RTICPUCx equal tozero isnotrecommended. Doing sowillhold theUpCounter at
zero fortwoRTICLK cycles after itoverflows from 0xFFFFFFFF tozero.
The counter values canbedetermined byreading therespective counter registers orbygenerating a
hardware event which captures thecounter value intotherespective capture register. Both functions are
described inthefollowing sections.

<!-- Page 586 -->

31 031 0
RTICLK31 0
31 0
31 0
CAP event source 0 from VIM
CAP event source 1 from VIM
=OVLINT0
To Compare
UnitCompare Up
Counter
RTICPUC0
Up Counter
Capture Up
Counter
RTICAUC0Free Running Counter
RTIFRC0
Capture Free Running
Counter
RTICAFRC03232
1
32
11
32
32
Control
RTICAPCTRLNTU0
NTU1Timebase
Control31 0
Up Counter
Register
RTIUC0
31 031 0
RTICLK31 0
31 0
31 0
=OVLINT1
To Compare
UnitCompare Up
Counter
RTICPUC1
Up Counter
Capture Up
Counter
RTICAUC1Free Running Counter
RTIFRC1
Capture Free Running
Counter
RTICAFRC13232
1
321
32
3231 0
Up Counter
Register
RTIUC11Counter Block 0
Counter Block 1NTU2
NTU3
Module Operation www.ti.com
586 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleFigure 17-2. Counter Block Diagram

<!-- Page 587 -->

if RTICPUCy 0, ≠
if RTIUDCPy = 0,tCOMPxtRTICLKx (RTICPUCy + 1) x RTIUDCPy =
tCOMPxt            xRTICLKx RTIUDCPy =
tCOMPxtRTICLK x (RTICPUCy + 1) x 232=(2 +1)32
www.ti.com Module Operation
587 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.2.1.1 Counter andCapture Read Consistency
Portions ofthedevice internal databus are32-bits wide. Iftheapplication wants toread the64-bit counters
orthe64-bit capture values, acertain order of32-bit read operations needs tobefollowed. This isto
prevent onecounter incrementing inbetween thetwoseparate read operations toboth counters.
Reading theCounters
The free running counter (RTIFRCx) must beread first. This priority willensure thatinthecycle when the
CPU reads RTIFRCx, theupcounter value isstored initscounter register (RTIUCx). The second read has
toaccess theupcounter register (RTIUCx), which then holds thevalue which corresponds tothenumber
ofRTICLK cycles thathave elapsed atthetime reading thefree running counter register (RTIFRCx).
NOTE: The upcounters areimplemented asshadow registers. Reading RTIUCx without having
read RTIFRCx firstwillreturn always thesame value. RTIUCx willonly beupdated when
RTIFRCx isread.
Reading theCapture Values
The free running counter capture register (RTICAFRCx) must beread first. This priority willensure thatin
thecycle when theCPU reads RTICAFRCx, theupcounter value isstored initscounter register
(RTICAUCx). The second read hastoaccess theupcounter register (RTICAUCx), which then holds the
value captured atthetime when reading thecapture free running counter register (RTICAFRCx).
NOTE: The capture upcounter registers areimplemented asshadow registers. Reading RTICAUCx
without having read RTICAFRCx firstwillreturn always thesame value. RTICAUCx willonly
beupdated when RTICAFRCx isread.
17.2.1.2 Capture Feature
Both counter blocks also provide acapture feature onexternal events. Two capture sources cantrigger
thecapture event. The source triggering theblock isconfigurable (RTICAPCTRL). The sources originate
from theVectored Interrupt Manager (VIM) andallow thegeneration ofcapture events when aperipheral
modules hasgenerated aninterrupt. Any oftheperipheral interrupts canbeselected asthecapture event
intheVIM.
When anevent isdetected, RTIUCx andRTIFRCx arestored inthecapture upcounter (RTICAUCx) and
capture free running counter (RTICAFRCx) registers. The read order ofthecaptured values must bethe
same astheread order oftheactual counters (see Section 17.2.1.1 ).
17.2.2 Interrupt/DMA Requests
There arefour compare registers (RTICOMPy) togenerate interrupt requests totheVIM orDMA requests
totheDMA controller. The interrupts canbeused togenerate different timebases fortheoperating
system. Each ofthecompare registers canbeconfigured tobecompared toeither RTIFRC0 orRTIFRC1.
When thecounter value matches thecompare value, aninterrupt isgenerated. Toallow periodic
interrupts, acertain value canbeadded tothecompare value inRTICOMPy automatically. This value is
stored intheupdate compare register (RTIUDCPy) andwillbeadded after acompare ismatched. The
period ofthegenerated interrupt/DMA request canbecalculated with:
(24)

<!-- Page 588 -->

+
INT REQnDMA REQy=From counter
block 0
From counter
block 131 0
Update Compare
RTIUDCP0 /RTIUDCP1
RTIUDCP2 /RTIUDCP3
Compare
RTICOMP0 /RTICOMP1
RTICOMP2 /RTICOMP331 0
32
Control
RTICOMPCTRL3232
1
Enable/Disable
RTISETINTENA[3:0]
RTICLEARINTENA[3:0]Enable/Disable
RTISETINTENA[11:8]
RTICLEARINTENA[11:8]
Module Operation www.ti.com
588 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleFigure 17-3. Compare Unit Block Diagram (shows only 1of4blocks forsimplification)
Another interrupt thatcanbegenerated istheoverflow interrupt (OVLINTx) incase theRTIFRCx counter
overflows.
The interrupts/DMA requests canbeenabled intheRTISETINTENA register anddisabled inthe
RTICLEARINTENA register. The RTIINTFLAG register shows thepending interrupts.
17.2.3 RTIClocking
The counter blocks areclocked with RTICLK (fordefinition seeSection 2.4.2 ).Counter block 0canbe
clocked inaddition byeither theFlexRay Macrotick (NTU0) ortheFlexRay Start ofCycle (NTU1).
Aclock supervision fortheNTUx clocking scheme isimplemented toavoid missing operating system ticks.
17.2.4 Synchronizing Timer Events toNetwork Time (NTU)
Forapplications which areparticipating onatime-triggered communication bus, itisoften beneficial to
synchronize theapplication oroperating system tothenetwork time. The RTIprovides afeature to
increment Free Running Counter 0(RTIFRC0) byaperiodic clock provided bythecommunication module.
Inthiscase twodifferent clocks canbechosen. One istheFlexRay module Macrotick (NTU0) andthe
other istheStart ofCycle (NTU1) information ofthesame module.
The application hascontrol over which clock (RTICLK, NTU0, NTU1) should beused forclocking
RTIFRC0. IfNTUx isused, aclock supervision circuit allows tomonitor thisclock andprovides afallback
solution, should theclock benon-functional (missing). Atoofastrunning NTUx cannot bedetected.
RTIUC0 isutilized tomonitor theNTUx signal. Adetection window canbeprogrammed inwhich avalid
NTU clock pulse needs tooccur. Ifnopulse isdetected, theRTIautomatically switches back toclock the
Free Running Counter 0with RTIUC0. Inorder toavoid abigjitter intheoperating ticks, incase aswitch
back toRTIUC0 happens, RTICPUC0 should besettoavalue sotheclock frequency RTIUC0 outputs is
approximately thesame astheNTUx frequency.

<!-- Page 589 -->

timeRTICPUC0
NTUxRTIUC0
RTITBLCOMP
RTITBHCOMP
Active Edge
Detection
31                                            0
Timebase≥ NTU
edge detect
≤Timebase Low Compare
RTITBLCOMPRTIUC0
31                                            0
Timebase High Compare
RTITBHCOMPControl
RTIGCTRLControl
RTITBCTRLNTU0
NTU1RTIFRC0
Control
RTITBCTRLIncrement by 1
Control
RTITBCTRLInterrupt
TBINT
www.ti.com Module Operation
589 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleFigure 17-4. Timebase Control
17.2.4.1 Detecting Clock Edges
Todetect clock edges ontheNTUx signal, thetimebase lowcompare hastobesetlower orequal than
thevalue stored intheRTICPUC0 register andthetimebase high compare hastobesethigher than 0
andlower than thetimebase lowcompare value. This effectively opens awindow inwhich anedge ofthe
NTUx signal isexpected (see Figure 17-5).Outside thiswindow, noedges willbedetected. Ifnoedge will
occur inside thedetection window, themultiplexer isswitched tointernal timebase. The application can
select togenerate atimebase interrupt (TBINT) andiftheINC bitisset,also willautomatically increment
RTIFRC0 byonetocompensate forthemissed clock cycle ofNTUx. Ifanedge occurs inside thewindow,
RTIUC0 willbereset tosynchronize thetwotimebases.
Inorder tomake theedge detection work properly, thevalue inRTICPUC0 needs tobeadapted sothat
RTIUC0 hasasimilar period asNTUx.
NOTE: Toensure theNTUx signal isproperly detected, theNTUx period must beatleast twice as
long astheRTICLK period.
Figure 17-5. Clock Detection Scheme

<!-- Page 590 -->

timeWrite TBEXT = 1
Active edge detection for
one RTICPUC0 + RTITBHCOMPRTICPUC0
NTUxCPUC0 might not be matched
depending on the NTU periodRTIUC0
to switch to ext. timbase
Module Operation www.ti.com
590 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.2.4.2 Switching from Internal Source toExternal Source
Iftheapplication switches from aninternal source toanexternal source, thetwosignals must be
synchronized (see Figure 17-6).The synchronization willoccur when theTBEXT bitisset.RTIUC0 willbe
reset andtheedge detection circuit willbeactive forone(RTICPUC0 +RTITBHCOMP) period oruntil an
edge isdetected. Ifthere isnopulse during thisperiod, thesource willbereset from anexternal clock
source toaninternal clock source. Ifanedge isdetected, thewindowed edge detection behavior willtake
place. Setting theTBEXT bitwillnotincrement free running counter 0.
NOTE: Ifanexternal timebase isused, then thesoftware must ensure thattimebase lowcompare
andtimebase high compare areprogrammed toavalid state before switching TBEXT toan
external source. This state isnecessary toallow thetimebase control circuit tooperate
correctly. The following condition must bemet:
*RTITBHCOMP <RTITBLCOMP +RTICPUC0
RTITBHCOMP must belower than RTICPUC0 because RTIUC0 willbereset ifRTICPUC0 is
reached. RTITBHCOMP willrepresent thenumber ofRTICLK cycles from RTICPUC0 until
thecircuit switches totheinternal timebase when noNTU edge isdetected.
Ifanexternal timebase isused, RTIGCTRL[0] must besetto1(enable RTIUC0) toensure
thatthetimebase control circuit does notwait indefinitely foranincoming signal.
Figure 17-6 shows atiming example forthesynchronization phase when theTBEXT bitisset.
Figure 17-6. Switch toNTUx
17.2.4.3 Switching from External Source toInternal Source
When theedge detection isactive (TBEXT =1)andnoclock edge ofNTUx isdetected inside the
programmed detection window, theRTIwillautomatically switch thetimebase toRTIUC0. Figure 17-7
shows atiming example foramissing NTU signal. Inthecase where theINC bitisset,RTIFRC0 will
automatically beincremented byonetocompensate forthemissed NTU pulse.
Setting TBEXT =0willalso switch theclock source forRTIFRC0 toRTIUC0.

<!-- Page 591 -->

DWD preload11                                         0DWD down counter24                                         0=0To RESET
DWD ctrl31                                        0
DWD hardwired31                                         0
code=RTIWDKEY15                                         0
Compare15                                          0
16 bit out to 2WD Finite State Machine
KEY [1:0] DischargeReset
Suspendlogic
RTICLK
nTRST
RTIDWDPRLD RTIDWDCTRLRTIDWDCNTR
timeRTICPUC0
NTUxUC0 reset
by NTU edgeUC0 reset
by CPUC0
compare matchswitch to internal
timebaseRTIUC0
missing NTU pulse
www.ti.com Module Operation
591 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleFigure 17-7. Missing NTUx Signal Example
17.2.5 Digital Watchdog (DWD)
The digital watchdog (DWD) isanoptional safety diagnostic which candetect arunaway CPU and
generate either areset orNMI (non-maskable interrupt) response. Itgenerates resets orNMIs after a
programmable period, orifnocorrect keysequence was written totheRTIWDKEY register. Figure 17-8
illustrates theDWD.
Figure 17-8. Digital Watchdog

<!-- Page 592 -->

time
CPUDWD
Down
Counter
access
to DWD00x1FFFFFF
Preload
Register
Value left
set DWD
Preloadenable
DWDwrite E51A
to WDKEYwrite A35C
to WDKEYwrite A35C
to WDKEYwrite E51A
to WDKEYReset/NMIshifted 13bits
Module Operation www.ti.com
592 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.2.5.1 Digital Watchdog (DWD)
The DWD isdisabled bydefault. Ifitshould beused, itmust beenabled bywriting a32-bit value tothe
RTIDWDCTRL register.
NOTE: Once theDWD isenabled, itcannot bedisabled except bysystem reset orpower onreset.
Ifthecorrect keysequence iswritten totheRTIWDKEY register (0xE51A followed by0xA35C), the25-bit
DWD down counter isreloaded with theleftjustified 12-bit preload value stored inRTIDWDPRLD. Ifan
incorrect value iswritten, awatchdog reset orNMI willoccur immediately. Areset orNMI willalso be
generated when theDWD down counter isdecremented to0.
While thedevice isinsuspend mode (halting debug mode), theDWD down counter keeps thevalue ithad
when entering suspend mode.
The DWD down counter willbedecremented with theRTICLK frequency.
Figure 17-9. DWD Operation
The expiration time oftheDWD down counter canbedetermined with thefollowing equation:
texp =(DWDPRLD +1)×213/RTICLK
where
DWDPRLD =0...4095
NOTE: Care should betaken toensure thattheCPU write tothewatchdog register ismade allowing
time forthewrite topropagate totheRTI.
17.2.5.2 Digital Windowed Watchdog (DWWD)
Inaddition tothetime-out boundary configurable viathedigital watchdog discussed inSection 17.2.5.1 ,
forenhanced safety metrics itisdesirable tocheck forawatchdog "pet" within atime window rather than
using asingle time threshold. This isenabled bythedigital windowed watchdog (DWWD) feature.
*Functional Behavior
The DWWD opens aconfigurable time window inwhich thewatchdog must beserviced. Any attempt to
service thewatchdog outside thistime window, orafailure toservice thewatchdog inthistime window,
willcause thewatchdog togenerate either areset oraNMI totheCPU. This iscontrolled byconfiguring
theRTIWWDRXNCTRL register. Aswith theDWD, theDWWD isdisabled after power onreset. When the
DWWD isconfigured togenerate anon-maskable interrupt onawindow violation, thewatchdog counter
continues tocount down. The NMI handler needs toclear thewatchdog violation status flag(s) andthen

<!-- Page 593 -->

time
CPUDWD
Down
Counter
access
to DWD00x1FFFFFF
Preload Register
Value left shifted
set DWD
Preloadenable
DWDReset/NMI11bits
Config
DWD
Window25%Write
WD
KeysPreload Register Value
left shifted 13bits
DWD can NOT be
in this period servedOpen
Window
open window open window open window
op.
winopen
windowopen
windowopen
windowopen
windowopen window open window open windowDWD Down
Counter
50% window
open
windowopen
window25% window
12.5% window
6.25% windowop.
winop.
win
o
w3.125% windowo
wo
w100% window
www.ti.com Module Operation
593 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Moduleservice thewatchdog bywriting thecorrect sequence inthewatchdog keyregister. This service willcause
thewatchdog counter togetreloaded from thepreload value andstart counting down. IftheNMI handler
does notservice thewatchdog intime, itcould count down alltheway tozero andwrap around. IftheNMI
Handler does notservice thewatchdog intime, theNMI gets generated continuously, each time the
counter counts to'0'.
The DWWD uses theDigital Watchdog (DWD) preload register (RTIDWDPRLD) setting todefine theend-
time ofthewindow. The start-time ofthewindow isdefined byawindow size configuration
register(RTIWWDSIZECTRL).
The default window size issetto100%, which corresponds totheDWD functionality ofatime-out-only
watchdog. The window size canbeselected (through register RTIWWDSIZECTRL) from among 100%,
50%, 25%, 12.5%, 6.25% and3.125% asshown inFigure 17-10 .The window with therespective size will
beopened before theendoftheDWD expiration. The user hastoserve thewatchdog inthewindow.
Otherwise, areset orNMI willgenerate. Figure 17-11 shows anDWWD operation example (25% window).
*Configuration ofDWWD
The DWWD preload value (same asDWD preload) canonly beconfigured when theDWWD counter is
disabled. The window size andwatchdog reaction toaviolation canbeconfigured even after the
watchdog hasbeen enabled. Any changes tothewindow size andwatchdog reaction configurations will
only take effect after thenext servicing oftheDWWD. This feature canbeutilized todynamically set
windows ofdifferent sizes based ontask execution time, adding aprogram sequence element tothe
diagnostic which canimprove fault coverage.
Figure 17-10. Digital Windowed Watchdog Timing Example
Figure 17-11. Digital Windowed Watchdog Operation Example (25% Window)

<!-- Page 594 -->

Module Operation www.ti.com
594 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.2.6 Low Power Modes
Low power modes allow thetrade offofthecurrent used during lowpower versus functionality andfast
wakeup response. Alllowpower modes have thefollowing characteristics:
*CPU andsystem clocks aredisabled.
*Flash banks andpump areinsleep mode.
*Allperipheral modules areinlowpower modes andtheclocks aredisabled (exceptions tothismay
occur andwould bedocumented inthespecific device data sheet).
Flexibility inenabling anddisabling clocks allows formany different low-power modes (see Section 2.4.3 ).
The operation oftheRTIModule isguaranteed inRun, Doze andSnooze modes. InSleep mode, all
clocks willbeswitched offandtheRTIwillnotwork.
InDoze andSnooze modes, theRTIisactive andisable towake upthedevice with compare, timebase
andoverflow interrupts. The compare interrupts canbeused toperiodically wake upthedevice. The
overflow interrupt canbeused tonotify theoperating system thatacounter overflow hasoccurred.
Capturing events generated bytheVectored Interrupt Module (VIM) isalso possible since, inboth ofthese
lowpower modes, theperipheral modules areable togenerate interrupts thatcantrigger capture events.
Capturing events while inSleep mode isnotsupported astheclock totheRTIisnotactive.
When thedevice isputintolowpower mode, theperipheral which isgenerating theexternal clock NTU is
nolonger active, andthetimebase control circuitry hastoswitch toaninternal clocking scheme when it
detects amissing clock onNTU. The timebase interrupt willwake upthedevice andtheapplication
software hastoadapt theperiodic interrupt generation totheinternal clock source.
DMA transfers willbedisabled, andDMA requests willnotbegenerated after device wakeup since the
DMA controller willbepowered down.
NOTE: RTICLK inDoze Mode
Inthespecial case ofDoze Mode with PLL off,RTICLK might have adifferent period than
with PLL enabled since RTICLK willbederived from theoscillator output. Ithastobe
ensured thattheVCLK toRTICLK ratio isatleast 3:1.
17.2.7 Halting Debug Mode Behaviour
Once thesystem enters halting debug mode, thebehavior oftheRTIdepends ontheCOS (continue on
suspend) bit.Ifthebitiscleared andhalting debug mode isactive, allcounters willstop operation. Ifthe
bitissettoone, allcounters willbeclocked normally andtheRTIwillwork likeinnormal mode. However,
iftheexternal timebase (NTU) isused andthesystem isinhalting debug mode, thetimebase control
circuit willswitch tointernal timebase once itdetects themissing NTU signal ofthesuspended
communication controller. This willbesignaled with anTBINT interrupt sothatsoftware canresynchronize
after thedevice exits halting debug mode.

<!-- Page 595 -->

www.ti.com RTIControl Registers
595 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3 RTIControl Registers
Table 17-1 provides asummary oftheregisters. The registers support 8-bit, 16-bit, and32-bit writes. The
offset isrelative totheassociated peripheral select. See thefollowing sections fordetailed descriptions of
theregisters. The base address forthecontrol registers isFFFF FC00h. The address locations notlisted
arereserved.
Table 17-1. RTIRegisters
Offset Acronym Register Description Section
00h RTIGCTRL RTIGlobal Control Register Section 17.3.1
04h RTITBCTRL RTITimebase Control Register Section 17.3.2
08h RTICAPCTRL RTICapture Control Register Section 17.3.3
0Ch RTICOMPCTRL RTICompare Control Register Section 17.3.4
10h RTIFRC0 RTIFree Running Counter 0Register Section 17.3.5
14h RTIUC0 RTIUpCounter 0Register Section 17.3.6
18h RTICPUC0 RTICompare UpCounter 0Register Section 17.3.7
20h RTICAFRC0 RTICapture Free Running Counter 0Register Section 17.3.8
24h RTICAUC0 RTICapture UpCounter 0Register Section 17.3.9
30h RTIFRC1 RTIFree Running Counter 1Register Section 17.3.10
34h RTIUC1 RTIUpCounter 1Register Section 17.3.11
38h RTICPUC1 RTICompare UpCounter 1Register Section 17.3.12
40h RTICAFRC1 RTICapture Free Running Counter 1Register Section 17.3.13
44h RTICAUC1 RTICapture UpCounter 1Register Section 17.3.14
50h RTICOMP0 RTICompare 0Register Section 17.3.15
54h RTIUDCP0 RTIUpdate Compare 0Register Section 17.3.16
58h RTICOMP1 RTICompare 1Register Section 17.3.17
5Ch RTIUDCP1 RTIUpdate Compare 1Register Section 17.3.18
60h RTICOMP2 RTICompare 2Register Section 17.3.19
64h RTIUDCP2 RTIUpdate Compare 2Register Section 17.3.20
68h RTICOMP3 RTICompare 3Register Section 17.3.21
6Ch RTIUDCP3 RTIUpdate Compare 3Register Section 17.3.22
70h RTITBLCOMP RTITimebase Low Compare Register Section 17.3.23
74h RTITBHCOMP RTITimebase High Compare Register Section 17.3.24
80h RTISETINTENA RTISetInterrupt Enable Register Section 17.3.25
84h RTICLEARINTENA RTIClear Interrupt Enable Register Section 17.3.26
88h RTIINTFLAG RTIInterrupt Flag Register Section 17.3.27
90h RTIDWDCTRL Digital Watchdog Control Register Section 17.3.28
94h RTIDWDPRLD Digital Watchdog Preload Register Section 17.3.29
98h RTIWDSTATUS Watchdog Status Register Section 17.3.30
9Ch RTIWDKEY RTIWatchdog Key Register Section 17.3.31
A0h RTIDWDCNTR RTIDigital Watchdog Down Counter Register Section 17.3.32
A4h RTIWWDRXNCTRL Digital Windowed Watchdog Reaction Control Register Section 17.3.33
A8h RTIWWDSIZECTRL Digital Windowed Watchdog Window Size Control Register Section 17.3.34
ACh RTIINTCLRENABLE RTICompare Interrupt Clear Enable Register Section 17.3.35
B0h RTICOMP0CLR RTICompare 0Clear Register Section 17.3.36
B4h RTICOMP1CLR RTICompare 1Clear Register Section 17.3.37
B8h RTICOMP2CLR RTICompare 2Clear Register Section 17.3.38
BCh RTICOMP3CLR RTICompare 3Clear Register Section 17.3.39

<!-- Page 596 -->

RTIControl Registers www.ti.com
596 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleNOTE: Writes toReserved registers may clear thepending RTIinterrupt.
17.3.1 RTIGlobal Control Register (RTIGCTRL)
The global control register starts/stops thecounters andselects thesignal compared with thetimebase
control circuit. This register isshown inFigure 17-12 anddescribed inTable 17-2.
Figure 17-12. RTIGlobal Control Register (RTIGCTRL) [offset =00]
31 20 19 16
Reserved NTUSEL
R-0 R/WP-0
15 14 2 1 0
COS Reserved CNT1EN CNT0EN
R/WP-0 R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-2. RTIGlobal Control Register (RTIGCTRL) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 NTUSEL Select NTU signal. These bitsdetermine which NTU input signal isused asexternal timebase
0h NTU0
5h NTU1
Ah NTU2
Fh NTU3
Allother
valuesTied to0
15 COS Continue onsuspend. This bitdetermines ifboth counters arestopped when thedevice goes into
halting debug mode orifthey continue counting.
0 Counters arestopped while inhalting debug mode.
1 Counters arerunning while inhalting debug mode.
14-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CNT1EN Counter 1enable. This bitstarts andstops counter block 1(RTIUC1 andRTIFRC1).
0 Counter block 1isstopped.
1 Counter block 1isrunning.
0 CNT0EN Counter 0enable. This bitstarts andstops counter block 0(RTIUC0 andRTIFRC0).
0 Counter block 0isstopped.
1 Counter block 0isrunning.
NOTE: Iftheapplication uses thetimebase circuit forsynchronization between thecommunications
controller andtheoperating system andthedevice enters halting debug mode, the
synchronization may belostdepending ontheCOS setting intheRTImodule andthehalting
debug mode behavior ofthecommunications controller.

<!-- Page 597 -->

www.ti.com RTIControl Registers
597 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.2 RTITimebase Control Register (RTITBCTRL)
The timebase control register selects ifthefree running counter 0isincremented byRTICLK orNTU. This
register isshown inFigure 17-13 anddescribed inTable 17-3.
Figure 17-13. RTITimebase Control Register (RTITBCTRL) [offset =04h]
31 8
Reserved
R-0
7 2 1 0
Reserved INC TBEXT
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-3. RTITimebase Control Register (RTITBCTRL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1 INC Increment free running counter 0.This bitdetermines whether thefree running counter 0(RTIFRC0) is
automatically incremented ifafailing clock ontheNTU signal isdetected.
0 RTIFRC0 willnotbeincremented onafailing external clock.
1 RTIFRC0 willbeincremented onafailing external clock.
0 TBEXT Timebase external. This bitselects whether thefree running counter 0(RTIFRC0) isclocked bythe
internal upcounter 0(RTIUC0) orfrom theexternal signal NTU. Setting theTBEXT bitfrom 0to1will
notincrement RTIFRC0, since RTIUC0 isreset.
When thetimebase supervisor circuit detects amissing clock edge, then theTBEXT bitisreset.
Only thesoftware canselect whether theexternal signal should beused.
0 RTIUC0 clocks RTIFRC0.
1 NTU clocks RTIFRC0.

<!-- Page 598 -->

RTIControl Registers www.ti.com
598 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.3 RTICapture Control Register (RTICAPCTRL)
The capture control register controls thecapture source forthecounters. This register isshown in
Figure 17-14 anddescribed inTable 17-4.
Figure 17-14. RTICapture Control Register (RTICAPCTRL) [offset =08h]
31 8
Reserved
R-0
7 2 1 0
Reserved CAPCNTR1 CAPCNTR0
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-4. RTICapture Control Register (RTICAPCTRL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CAPCNTR1 Capture counter 1.This bitdetermines which external interrupt source triggers acapture event of
RTIUC1 andRTIFRC1.
0 Capture ofRTIUC1/ RTIFRC1 istriggered bycapture event source 0.
1 Capture ofRTIUC1/ RTIFRC1 istriggered bycapture event source 1.
0 CAPCNTR0 Capture counter 0.This bitdetermines which external interrupt source triggers acapture event of
RTIUC0 andRTIFRC0.
0 Capture ofRTIUC0/ RTIFRC0 istriggered bycapture event source 0.
1 Capture ofRTIUC0/ RTIFRC0 istriggered bycapture event source 1.

<!-- Page 599 -->

www.ti.com RTIControl Registers
599 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.4 RTICompare Control Register (RTICOMPCTRL)
The compare control register controls thesource forthecompare registers. This register isshown in
Figure 17-15 anddescribed inTable 17-5.
Figure 17-15. RTICompare Control Register (RTICOMPCTRL) [offset =0Ch]
31 16
Reserved
R-0
15 13 12 11 9 8
Reserved COMPSEL3 Reserved COMPSEL2
R-0 R/WP-0 R-0 R/WP-0
7 5 4 3 1 0
Reserved COMPSEL1 Reserved COMPSEL0
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-5. RTICompare Control Register (RTICOMPCTRL) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 COMPSEL3 Compare select 3.This bitdetermines thecounter with which thecompare value held incompare
register 3(RTICOMP3) iscompared.
0 Value willbecompared with RTIFRC0.
1 Value willbecompared with RTIFRC1.
11-9 Reserved 0 Reads return 0.Writes have noeffect.
8 COMPSEL2 Compare select 2.This bitdetermines thecounter with which thecompare value held incompare
register 2(RTICOMP2) iscompared.
0 Value willbecompared with RTIFRC0.
1 Value willbecompared with RTIFRC1.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 COMPSEL1 Compare select 1.This bitdetermines thecounter with which thecompare value held incompare
register 1(RTICOMP1) iscompared.
0 Value willbecompared with RTIFRC0.
1 Value willbecompared with RTIFRC1.
3-1 Reserved 0 Reads return 0.Writes have noeffect.
0 COMPSEL0 Compare select 0.This bitdetermines thecounter with which thecompare value held incompare
register 0(RTICOMP0) iscompared.
0 Value willbecompared with RTIFRC0.
1 Value willbecompared with RTIFRC1.

<!-- Page 600 -->

RTIControl Registers www.ti.com
600 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.5 RTIFree Running Counter 0Register (RTIFRC0)
The free running counter 0register holds thecurrent value offree running counter 0.This register is
shown inFigure 17-16 anddescribed inTable 17-6.
Figure 17-16. RTIFree Running Counter 0Register (RTIFRC0) [offset =10h]
31 16
FRC0
R/WP-0
15 0
FRC0
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-6. RTIFree Running Counter 0Register (RTIFRC0) Field Descriptions
Bit Field Value Description
31-0 FRC0 0-FFFF FFFFh Free running counter 0.This registers holds thecurrent value ofthefree running counter 0.
Aread ofthiscounter returns thecurrent value ofthecounter.
The counter canbepreset bywriting (inprivileged mode only) tothisregister. The counter
increments then from thiswritten value upwards.
Note: Ifcounters must bepreset, they must bedisabled intheRTIGCTRL register to
ensure consistency between RTIUC0 andRTIFRC0.
17.3.6 RTIUpCounter 0Register (RTIUC0)
The upcounter 0register holds thecurrent value ofprescale counter. This register isshown inFigure 17-
17anddescribed inTable 17-7.
Figure 17-17. RTIUpCounter 0Register (RTIUC0) [offset =14h]
31 16
UC0
R/WP-0
15 0
UC0
R/WP-0
LLEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-7. RTIUpCounter 0Register (RTIUC0) Field Descriptions
Bit Field Value Description
31-0 UC0 0-FFFF FFFFh Upcounter 0.This register holds thecurrent value oftheupcounter 0andprescales theRTI
clock. Itwillbeonly updated byaprevious read offree running counter 0(RTIFRC0). This
method ofupdating effectively gives a64-bit read ofboth counters, without having theproblem
ofacounter being updated between twoconsecutive reads onupcounter 0(RTIUC0) andfree
running counter 0(RTIFRC0).
Aread ofthiscounter returns thevalue ofthecounter atthetime RTIFRC0 was read.
Awrite tothiscounter presets itwith avalue. The counter then increments from thiswritten
value upwards.
Note: Ifcounters must bepreset, they must bedisabled intheRTIGCTRL register toensure
consistency between RTIUC0 andRTIFRC0.
Note: Ifthepreset value isbigger than thecompare value stored inregister RTICPUC0,
then itcantake along time until acompare matches, since RTIUC0 hastocount upuntil
itoverflows.

<!-- Page 601 -->

www.ti.com RTIControl Registers
601 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.7 RTICompare UpCounter 0Register (RTICPUC0)
The compare upcounter 0register holds thevalue tobecompared with prescale counter 0(RTIUC0).
This register isshown inFigure 17-18 anddescribed inTable 17-8.
Figure 17-18. RTICompare UpCounter 0Register (RTICPUC0) [offset =18h]
31 16
CPUC0
R/WP-0
15 0
CPUC0
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-8. RTICompare UpCounter 0Register (RTICPUC0) Field Descriptions
Bit Field Value Description
31-0 CPUC0 0-FFFF FFFFh Compare upcounter 0.This register holds thevalue thatiscompared with theupcounter 0.
When thecompare shows amatch, thefree running counter 0(RTIFRC0) isincremented.
RTIUC0 issetto0when thecounter value matches theRTICPUC0 value. The value setinthis
register prescales theRTIclock.
IfCPUC0 =0,then
fFRC0=RTICLK/(232+1)(Setting CPUC0 equal to0isnotrecommended. Doing sowillhold the
UpCounter at0for2RTICLK cycles after itoverflows from FFFF FFFFh to0.)
IfCPUC0≠0,then
fFRC0=RTICLK/(RTICPUC0+1)
Aread ofthisregister returns thecurrent compare value.
Awrite tothisregister:
*IfTBEXT =0,thecompare value isupdated.
*IfTBEXT =1,thecompare value isunchanged.
17.3.8 RTICapture Free Running Counter 0Register (RTICAFRC0)
The capture free running counter 0register holds thefree running counter 0onexternal events. This
register isshown inFigure 17-19 anddescribed inTable 17-9.
Figure 17-19. RTICapture Free Running Counter 0Register (RTICAFRC0) [offset =20h]
31 16
CAFRC0
R-0
15 0
CAFRC0
R-0
LEGEND: R=Read only; -n=value after reset
Table 17-9. RTICapture Free Running Counter 0Register (RTICAFRC0) Field Descriptions
Bit Field Value Description
31-0 CAFRC0 0-FFFF FFFFh Capture free running counter 0.This register captures thecurrent value ofthefree running
counter 0(RTIFRC0) when anevent occurs, controlled bytheexternal capture control block.
Aread ofthisregister returns thevalue ofRTIFRC0 onacapture event.

<!-- Page 602 -->

RTIControl Registers www.ti.com
602 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.9 RTICapture UpCounter 0Register (RTICAUC0)
The capture upcounter 0register holds thecurrent value ofprescale counter 0onexternal events. This
register isshown inFigure 17-20 anddescribed inTable 17-10 .
Figure 17-20. RTICapture UpCounter 0Register (RTICAUC0) [offset =24h]
31 16
CAUC0
R-0
15 0
CAUC0
R-0
LEGEND: R=Read only; -n=value after reset
Table 17-10. RTICapture UpCounter 0Register (RTICAUC0) Field Descriptions
Bit Field Value Description
31-0 CAUC0 0-FFFF FFFFh Capture upcounter 0.This register captures thecurrent value oftheupcounter 0(RTIUC0)
when anevent occurs, controlled bytheexternal capture control block.
Note: Theread sequence must bethesame aswith RTIUC0 andRTIFRC0. Therefore, the
RTICAFRC0 register must beread before theRTICAUC0 register isread. This sequence
ensures thatthevalue oftheRTICAUC0 register isthecorresponding value tothe
RTICAFRC0 register, even ifanother capture event happens inbetween thetworeads.
Aread ofthisregister returns thevalue ofRTIUC0 onacapture event.
17.3.10 RTIFree Running Counter 1Register (RTIFRC1)
The free running counter 1register holds thecurrent value ofthefree running counter 1.This register is
shown inFigure 17-21 anddescribed inTable 17-11 .
Figure 17-21. RTIFree Running Counter 1Register (RTIFRC1) [offset =30h]
31 16
FRC1
R/WP-0
15 0
FRC1
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-11. RTIFree Running Counter 1Register (RTIFRC1) Field Descriptions
Bit Field Value Description
31-0 FRC1 0-FFFF FFFFh Free running counter 1.This register holds thecurrent value ofthefree running counter 1and
willbeupdated continuously.
Aread ofthisregister returns thecurrent value ofthecounter.
Awrite tothisregister presets thecounter. The counter increments then from thiswritten value
upwards.
Note: Ifcounters must bepreset, they must bedisabled intheRTIGCTRL register to
ensure consistency between RTIUC1 andRTIFRC1.

<!-- Page 603 -->

www.ti.com RTIControl Registers
603 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.11 RTIUpCounter 1Register (RTIUC1)
The upcounter 1register holds thecurrent value oftheprescale counter 1.This register isshown in
Figure 17-22 anddescribed inTable 17-12 .
Figure 17-22. RTIUpCounter 1Register (RTIUC1) [offset =34h]
31 16
UC1
R/WP-0
15 0
UC1
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-12. RTIUpCounter 1Register (RTIUC1) Field Descriptions
Bit Field Value Description
31-0 UC1 0-FFFF FFFFh Upcounter 1.This register holds thecurrent value oftheupcounter 1andprescales theRTI
clock. Itwillbeonly updated byaprevious read offree running counter 1(RTIFRC1). This
method ofupdating effectively gives a64-bit read ofboth counters, without having theproblem
ofacounter being updated between twoconsecutive reads onRTIUC1 andRTIFRC1.
Aread ofthisregister willreturn thevalue ofthecounter when theRTIFRC1 was read.
Awrite tothisregister presets thecounter. The counter then increments from thiswritten value
upwards.
Note: Ifcounters must bepreset, they must bedisabled intheRTIGCTRL register to
ensure consistency between RTIUC1 andRTIFRC1.
Note: Ifthepreset value isbigger than thecompare value stored inregister RTICPUC1,
then itcantake along time until acompare matches, since RTIUC1 hastocount upuntil
itoverflows.

<!-- Page 604 -->

RTIControl Registers www.ti.com
604 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.12 RTICompare UpCounter 1Register (RTICPUC1)
The compare upcounter 1register holds thevalue compared with prescale counter 1.This register is
shown inFigure 17-23 anddescribed inTable 17-13 .
Figure 17-23. RTICompare UpCounter 1Register (RTICPUC1) [offset =38h]
31 16
CPUC1
R/WP-0
15 0
CPUC1
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-13. RTICompare UpCounter 1Register (RTICPUC1) Field Descriptions
Bit Field Value Description
31-0 CPUC1 0-FFFF FFFFh Compare upcounter 1.This register holds thecompare value, which iscompared with theup
counter 1.When thecompare matches, thefree running counter 1(RTIFRC1) isincremented.
The upcounter iscleared to0when thecounter value matches theCPUC1 value. The value
setinthisprescales theRTIclock according tothefollowing formula:
IfCPUC1 =0,then
fFRC1=RTICLK/(232+1)(Setting CPUC1 equal to0isnotrecommended. Doing sowillhold the
UpCounter at0for2RTICLK cycles after itoverflows from FFFF FFFFh to0.)
IfCPUC1≠0,then
fFRC1=RTICLK/(RTICPUC1+1)
Aread ofthisregister returns thecurrent compare value.
Awrite tothisregister updates thecompare value.

<!-- Page 605 -->

www.ti.com RTIControl Registers
605 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.13 RTICapture Free Running Counter 1Register (RTICAFRC1)
The capture free running counter 1register holds thecurrent value offree running counter 1onexternal
events. This register isshown inFigure 17-24 anddescribed inTable 17-14 .
Figure 17-24. RTICapture Free Running Counter 1Register (RTICAFRC1) [offset =40h]
31 16
CAFRC1
R-0
15 0
CAFRC1
R-0
LEGEND: R=Read only; -n=value after reset
Table 17-14. RTICapture Free Running Counter 1Register (RTICAFRC1) Field Descriptions
Bit Field Value Description
31-0 CAFRC1 0-FFFF FFFFh Capture free running counter 1.This register captures thecurrent value ofthefree running
counter 1(RTIFRC1) when anevent occurs, controlled bytheexternal capture control block.
Aread ofthisregister returns thevalue ofRTIFRC1 onacapture event.
17.3.14 RTICapture UpCounter 1Register (RTICAUC1)
The capture upcounter 1register holds thecurrent value ofprescale counter 1onexternal events. This
register isshown inFigure 17-25 anddescribed inTable 17-15 .
Figure 17-25. RTICapture UpCounter 1Register (RTICAUC1) [offset =44h]
31 16
CAUC1
R-0
15 0
CAUC1
R-0
LEGEND: R=Read only; -n=value after reset
Table 17-15. RTICapture UpCounter 1Register (RTICAUC1) Field Descriptions
Bit Field Value Description
31-0 CAUC1 0-FFFF FFFFh Capture upcounter 1.This register captures thecurrent value oftheupcounter 1(RTIUC1)
when anevent occurs, controlled bytheexternal capture control block.
Note: TheRTICAFRC1 register must beread before theRTICAUC1 register isread. This
sequence ensures thatthevalue oftheRTICAUC1 register isthecorresponding value to
theRTICAFRC1 register, even ifanother capture event happens inbetween thetwo
reads.
Aread ofthisregister returns thevalue ofRTIUC1 onacapture event.

<!-- Page 606 -->

RTIControl Registers www.ti.com
606 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.15 RTICompare 0Register (RTICOMP0)
The compare 0register holds thevalue tobecompared with thecounters. This register isshown in
Figure 17-26 anddescribed inTable 17-16 .
Figure 17-26. RTICompare 0Register (RTICOMP0) [offset =50h]
31 16
COMP0
R/WP-0
15 0
COMP0
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-16. RTICompare 0Register (RTICOMP0) Field Descriptions
Bit Field Value Description
31-0 COMP0 0-FFFF FFFFh Compare 0.This registers holds avalue thatiscompared with thecounter selected inthe
compare control logic. IfRTIFRC0 orRTIFRC1, depending onthecounter selected, matches
thecompare value, aninterrupt isflagged. With thisregister itisalso possible toinitiate aDMA
request.
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister (inprivileged mode only) willupdate thecompare register with anew
compare value.
17.3.16 RTIUpdate Compare 0Register (RTIUDCP0)
The update compare 0register holds thevalue tobeadded tothecompare register 0value onacompare
match. This register isshown inFigure 17-27 anddescribed inTable 17-17 .
Figure 17-27. RTIUpdate Compare 0Register (RTIUDCP0) [offset =54h]
31 16
UDCP0
R/WP-0
15 0
UDCP0
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-17. RTIUpdate Compare 0Register (RTIUDCP0) Field Descriptions
Bit Field Value Description
31-0 UDCP0 0-FFFF FFFFh Update compare 0.This register holds avalue thatisadded tothevalue inthecompare 0
(RTICOMP0) register each time acompare matches. This function allows periodic interrupts to
begenerated without software intervention.
Aread ofthisregister willreturn thevalue tobeadded totheRTICOMP0 register onthenext
compare match.
Awrite tothisregister willprovide anew update value.

<!-- Page 607 -->

www.ti.com RTIControl Registers
607 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.17 RTICompare 1Register (RTICOMP1)
The compare 1register holds thevalue tobecompared tothecounters. This register isshown in
Figure 17-28 anddescribed inTable 17-18 .
Figure 17-28. RTICompare 1Register (RTICOMP1) [offset =58h]
31 16
COMP1
R/WP-0
15 0
COMP1
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-18. RTICompare 1Register (RTICOMP1) Field Descriptions
Bit Field Value Description
31-0 COMP1 0-FFFF FFFFh Compare 1.This register holds avalue thatiscompared with thecounter selected inthe
compare control logic. IfRTIFRC0 orRTIFRC1, depending onthecounter selected, matches
thiscompare value, aninterrupt isflagged. With thisregister, itispossible toinitiate aDMA
request.
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister willupdate thecompare register with anew compare value.
17.3.18 RTIUpdate Compare 1Register (RTIUDCP1)
The update compare 1register holds thevalue tobeadded tothecompare register 1value onacompare
match. This register isshown inFigure 17-29 anddescribed inTable 17-19 .
Figure 17-29. RTIUpdate Compare 1Register (RTIUDCP1) [offset =5Ch]
31 16
UDCP1
R/WP-0
15 0
UDCP1
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-19. RTIUpdate Compare 1Register (RTIUDCP1) Field Descriptions
Bit Field Value Description
31-0 UDCP1 0-FFFF FFFFh Update compare 1.This register holds avalue thatisadded tothevalue intheRTICOMP1
register each time acompare matches. This process allows periodic interrupts tobegenerated
without software intervention.
Aread ofthisregister willreturn thevalue tobeadded totheRTICOMP1 register onthenext
compare match.
Awrite tothisregister willprovide anew update value.

<!-- Page 608 -->

RTIControl Registers www.ti.com
608 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.19 RTICompare 2Register (RTICOMP2)
The compare 2register holds thevalue tobecompared tothecounters. This register isshown in
Figure 17-30 anddescribed inTable 17-20 .
Figure 17-30. RTICompare 2Register (RTICOMP2) [offset =60h]
31 16
COMP2
R/WP-0
15 0
COMP2
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-20. RTICompare 2Register (RTICOMP2) Field Descriptions
Bit Field Value Description
31-0 COMP2 0-FFFF FFFFh Compare 2.This register holds avalue thatiscompared with thecounter selected inthe
compare control logic. IfRTIFRC0 orRTIFRC1, depending onthecounter selected, matches
thiscompare value, aninterrupt isflagged. With thisregister, itispossible toinitiate aDMA
request.
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister (inprivileged mode only) willprovide anew compare value.
17.3.20 RTIUpdate Compare 2Register (RTIUDCP2)
The update compare 2register holds thevalue tobeadded tothecompare register 2value onacompare
match. This register isshown inFigure 17-31 anddescribed inTable 17-21 .
Figure 17-31. RTIUpdate Compare 2Register (RTIUDCP2) [offset =64h]
31 16
UDCP2
R/WP-0
15 0
UDCP2
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-21. RTIUpdate Compare 2Register (RTIUDCP2) Field Descriptions
Bit Field Value Description
31-0 UDCP2 0-FFFF FFFFh Update compare 2.This register holds avalue thatisadded tothevalue intheRTICOMP2
register each time acompare matches. This process makes itpossible togenerate periodic
interrupts without software intervention.
Aread ofthisregister willreturn thevalue tobeadded totheRTICOMP2 register onthenext
compare match.
Awrite tothisregister willprovide anew update value.

<!-- Page 609 -->

www.ti.com RTIControl Registers
609 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.21 RTICompare 3Register (RTICOMP3)
The compare 3register holds thevalue tobecompared tothecounters. This register isshown in
Figure 17-32 anddescribed inTable 17-22 .
Figure 17-32. RTICompare 3Register (RTICOMP3) [offset =68h]
31 16
COMP3
R/WP-0
15 0
COMP3
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-22. RTICompare 3Register (RTICOMP3) Field Descriptions
Bit Field Value Description
31-0 COMP3 0-FFFF FFFFh Compare 3.This register holds avalue thatiscompared with thecounter selected inthe
compare control logic. IfRTIFRC0 orRTIFRC1, depending onthecounter selected, matches
thiscompare value, aninterrupt isflagged. With thisregister, itispossible toinitiate aDMA
request.
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister willprovide anew compare value.
17.3.22 RTIUpdate Compare 3Register (RTIUDCP3)
The update compare 3register holds thevalue tobeadded tothecompare register 3value onacompare
match. This register isshown inFigure 17-33 anddescribed inTable 17-23 .
Figure 17-33. RTIUpdate Compare 3Register (RTIUDCP3) [offset =6Ch]
31 16
UDCP3
R/WP-0
15 0
UDCP3
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-23. RTIUpdate Compare 3Register (RTIUDCP3) Field Descriptions
Bit Field Value Description
31-0 UDCP3 0-FFFF FFFFh Update compare 3.This register holds avalue thatisadded tothevalue intheRTICOMP3
register each time acompare matches. This process makes itpossible togenerate periodic
interrupts without software intervention.
Aread ofthisregister willreturn thevalue tobeadded totheRTICOMP3 register onthenext
compare match.
Awrite tothisregister willprovide anew update value.

<!-- Page 610 -->

RTIControl Registers www.ti.com
610 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.23 RTITimebase Low Compare Register (RTITBLCOMP)
The timebase lowcompare register holds thevalue toactivate theedge detection circuit. This register is
shown inFigure 17-34 anddescribed inTable 17-24 .
Figure 17-34. RTITimebase Low Compare Register (RTITBLCOMP) [offset =70h]
31 16
TBLCOMP
R/WP-0
15 0
TBLCOMP
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-24. RTITimebase Low Compare Register (RTITBLCOMP) Field Descriptions
Bit Field Value Description
31-0 TBLCOMP 0-FFFF FFFFh Timebase lowcompare value. This value determines when theedge detection circuit starts
monitoring theNTU signal. Itwillbecompared with RTIUC0.
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister hasthefollowing effects:
IfTBEXT =0:The compare value isupdated.
IfTBEXT =1:The compare value isnotchanged.
17.3.24 RTITimebase High Compare Register (RTITBHCOMP)
The timebase high compare register holds thevalue todeactivate theedge detection circuit. This register
isshown inFigure 17-35 anddescribed inTable 17-25 .
Figure 17-35. RTITimebase High Compare Register (RTITBHCOMP) [offset =74h]
31 16
TBHCOMP
R/WP-0
15 0
TBHCOMP
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-25. RTITimebase High Compare Register (RTITBHCOMP) Field Descriptions
Bit Field Value Description
31-0 TBHCOMP 0-FFFF FFFFh Timebase high compare value. This value determines when theedge detection circuit willstop
monitoring theNTU signal. Itwillbecompared with RTIUC0.
RTITBHCOMP must beless than RTICPUC0 because RTIUC0 willbereset when RTICPUC0
isreached.
Example: The NTU edge detection circuit should beactive ±10RTICLK cycles around
RTICPUC0.
*RTICPUC0 =0050h
*RTITBLCOMP =0046h
*RTITBHCOMP =0009h
Aread ofthisregister willreturn thecurrent compare value.
Awrite tothisregister hasthefollowing effects:
IfTBEXT =0:The compare value isupdated.
IfTBEXT =1:The compare value isnotchanged.

<!-- Page 611 -->

www.ti.com RTIControl Registers
611 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.25 RTISetInterrupt Enable Register (RTISETINTENA)
This register prevents thenecessity ofaread-modify-write operation ifaparticular interrupt should be
enabled. This register isshown inFigure 17-36 anddescribed inTable 17-26 .
Figure 17-36. RTISetInterrupt Control Register (RTISETINTENA) [offset =80h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved SETOVL1INT SETOVL0INT SETTBINT
R-0 R/WP-0 R/WP-0 R/WP-0
15 12 11 10 9 8
Reserved SETDMA3 SETDMA2 SETDMA1 SETDMA0
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 4 3 2 1 0
Reserved SETINT3 SETINT2 SETINT1 SETINT0
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-26. RTISetInterrupt Control Register (RTISETINTENA) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 SETOVL1INT Setfree running counter 1overflow interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
17 SETOVL0INT Setfree running counter 0overflow interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
16 SETTBINT Settimebase interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SETDMA3 Setcompare DMA request 3.
0 Read: DMA request isdisabled.
Write: DMA request isunchanged.
1 Read orWrite: DMA request isenabled.
10 SETDMA2 Setcompare DMA request 2.
0 Read: DMA request isdisabled.
Write: DMA request isunchanged.
1 Read orWrite: DMA request isenabled.
9 SETDMA1 Setcompare DMA request 1.
0 Read: DMA request isdisabled.
Write: DMA request isunchanged.
1 Read orWrite: DMA request isenabled.

<!-- Page 612 -->

RTIControl Registers www.ti.com
612 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleTable 17-26. RTISetInterrupt Control Register (RTISETINTENA) Field Descriptions (continued)
Bit Field Value Description
8 SETDMA0 Setcompare DMA request 0.
0 Read: DMA request isdisabled.
Write: DMA request isunchanged.
1 Read orWrite: DMA request isenabled.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3 SETINT3 Setcompare interrupt 3.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
2 SETINT2 Setcompare interrupt 2.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
1 SETINT1 Setcompare interrupt 1.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.
0 SETINT0 Setcompare interrupt 0.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read orWrite: Interrupt isenabled.

<!-- Page 613 -->

www.ti.com RTIControl Registers
613 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.26 RTIClear Interrupt Enable Register (RTICLEARINTENA)
This register prevents thenecessity ofaread-modify-write operation ifaparticular interrupt should be
disabled. This register isshown inFigure 17-37 anddescribed inTable 17-27 .
Figure 17-37. RTIClear Interrupt Control Register (RTICLEARINTENA) [offset =84h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved CLEAROVL1INT CLEAROVL0INT CLEARTBINT
R-0 R/WP-0 R/WP-0 R/WP-0
15 12 11 10 9 8
Reserved CLEARDMA3 CLEARDMA2 CLEARDMA1 CLEARDMA0
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 4 3 2 1 0
Reserved CLEARINT3 CLEARINT2 CLEARINT1 CLEARINT0
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-27. RTIClear Interrupt Control Register (RTICLEARINTENA) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 CLEAROVL1INT Clear free running counter 1overflow interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
17 CLEAROVL0INT Clear free running counter 0overflow interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
16 CLEARTBINT Clear timebase interrupt.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 CLEARDMA3 Clear compare DMA request 3.
0 Read: DMA request isdisabled.
Write: Corresponding bitisunchanged.
1 Read: DMA request isenabled.
Write: DMA request isdisabled.
10 CLEARDMA2 Clear compare DMA request 2.
0 Read: DMA request isdisabled.
Write: Corresponding bitisunchanged.
1 Read: DMA request isenabled.
Write: DMA request isdisabled.

<!-- Page 614 -->

RTIControl Registers www.ti.com
614 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleTable 17-27. RTIClear Interrupt Control Register (RTICLEARINTENA) Field Descriptions (continued)
Bit Field Value Description
9 CLEARDMA1 Clear compare DMA request 1.
0 Read: DMA request isdisabled.
Write: Corresponding bitisunchanged.
1 Read: DMA request isenabled.
Write: DMA request isdisabled.
8 CLEARDMA0 Clear compare DMA request 0.
0 Read: DMA request isdisabled.
Write: Corresponding bitisunchanged.
1 Read: DMA request isenabled.
Write: DMA request isdisabled.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3 CLEARINT3 Clear compare interrupt 3.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
2 CLEARINT2 Clear compare interrupt 2.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
1 CLEARINT1 Clear compare interrupt 1.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.
0 CLEARINT0 Clear compare interrupt 0.
0 Read: Interrupt isdisabled.
Write: Corresponding bitisunchanged.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.

<!-- Page 615 -->

www.ti.com RTIControl Registers
615 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.27 RTIInterrupt Flag Register (RTIINTFLAG)
The corresponding flags aresetatevery compare match oftheRTIFRCx andRTICOMPx values, whether
theinterrupt isenabled ornot.This register isshown inFigure 17-38 anddescribed inTable 17-28 .
Figure 17-38. RTIInterrupt Flag Register (RTIINTFLAG) [offset =88h]
31 19 18 17 16
Reserved OVL1INT OVL0INT TBINT
R-0 R/W1CP-
0R/W1CP-
0R/W1C
P-0
15 4 3 2 1 0
Reserved INT3 INT2 INT1 INT0
R-0 R/W1C
P-0R/W1C
P-0R/W1C
P-0R/W1C
P-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 17-28. RTIInterrupt Flag Register (RTIINTFLAG) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 OVL1INT Free running counter 1overflow interrupt flag. This bitdetermines ifaninterrupt ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
17 OVL0INT Free running counter 0overflow interrupt flag. This bitdetermines ifaninterrupt ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
16 TBINT Timebase interrupt flag. This flagissetwhen theTBEXT bitiscleared bydetection ofamissing
external clock edge. Itwillnotbesetbyclearing TBEXT bysoftware. Itdetermines ifaninterrupt is
pending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3 INT3 Interrupt flag3.These bitsdetermine ifaninterrupt duetoaCompare 3match ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
2 INT2 Interrupt flag2.These bitsdetermine ifaninterrupt duetoaCompare 2match ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
1 INT1 Interrupt flag1.These bitsdetermine ifaninterrupt duetoaCompare 1match ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.

<!-- Page 616 -->

RTIControl Registers www.ti.com
616 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) ModuleTable 17-28. RTIInterrupt Flag Register (RTIINTFLAG) Field Descriptions (continued)
Bit Field Value Description
0 INT0 Interrupt flag0.These bitsdetermine ifaninterrupt duetoaCompare 0match ispending.
0 Read: Nointerrupt ispending.
Write: Bitisunchanged.
1 Read: Interrupt ispending.
Write: Bitiscleared to0.
17.3.28 Digital Watchdog Control Register (RTIDWDCTRL)
The software hastowrite totheDWDCTRL field inorder toenable theDWD, asdescribed below. Once
enabled, thewatchdog canonly bedisabled byasystem reset. The application cannot disable the
watchdog. However should theRTICLK source bechanged toasource thatisunimplemented itwillhave
thesame effect asdisabling thewatchdog. This register isshown inFigure 17-38 anddescribed in
Table 17-28 .
Figure 17-39. Digital Watchdog Control Register (RTIDWDCTRL) [offset =90h]
31 16
DWDCTRL
R/WP-5312h
15 0
DWDCTRL
R/WP-ACEDh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-29. Digital Watchdog Control Register (RTIDWDCTRL) Field Descriptions
Bit Field Value Description
31-0 DWDCTRL Digital Watchdog Control.
5312 ACEDh Read: DWD counter isdisabled.
Write: State ofDWD counter isunchanged (stays enabled ordisabled).
A985 59DAh Read: DWD counter isenabled.
Write: DWD counter isenabled.
Allother values Read: DWD counter state isunchanged (enabled ordisabled).
Write: State ofDWD counter isunchanged (stays enabled ordisabled).
Note: Once theenable value iswritten, allother future writes areblocked. Inother words, once
DWD isenabled, itcanonly bedisabled bysystem reset orpower onreset. However should
theRTICLK source bechanged toasource thatisunimplemented itwillhave thesame effect
asdisabling thewatchdog.

<!-- Page 617 -->

www.ti.com RTIControl Registers
617 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.29 Digital Watchdog Preload Register (RTIDWDPRLD)
This register sets theexpiration time oftheDWD. This register isshown inFigure 17-38 anddescribed in
Table 17-28 .
Figure 17-40. Digital Watchdog Preload Register (RTIDWDPRLD) [offset =94h]
31 16
Reserved
R-0
15 12 11 0
Reserved DWDPRLD
R-0 R/WP-FFFh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-30. Digital Watchdog Preload Register (RTIDWDPRLD) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0andwrites have noeffect.
11-0 DWDPRLD 0-FFFh Digital Watchdog Preload Value.
Read: The current preload value
Write: Setthepreload value. The DWD preload register canbeconfigured only when theDWD is
disabled. Therefore, theapplication canonly configure theDWD preload register before itenables
theDWD down counter.
The expiration time oftheDWD Down Counter canbedetermined with following equation:
texp =(DWDPRLD+1) x213/RTICLK1
where: DWDPRLD =0...4095

<!-- Page 618 -->

RTIControl Registers www.ti.com
618 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.30 Watchdog Status Register (RTIWDSTATUS)
This register records thestatus oftheDWD. The values ofthefollowing status bitswillnotbeaffected by
asoftreset. These bitsarecleared byapower-on reset, orbyawrite of1.These bitscanbeused for
debug purposes. This register isshown inFigure 17-38 anddescribed inTable 17-28 .
Figure 17-41. Watchdog Status Register (RTIWDSTATUS) [offset =98h]
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved DWWD ST END TIME VIOL START TIME VIOL KEY ST DWD ST Reserved
R-0 R/W1CP-x R/W1CP-x R/W1CP-x R/W1CP-x R/W1CP-x R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 17-31. Watchdog Status Register (RTIWDSTATUS) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5 DWWD ST Windowed Watchdog Status
0 Read: Notime-window violation hasoccurred.
Write: Leaves thecurrent value unchanged.
1 Read: Time-window violation hasoccurred. The watchdog hasgenerated either asystem reset
oranon-maskable interrupt totheCPU inthiscase.
Write: Bitiscleared to0.This willalso clear allother status flags intheRTIWDSTATUS
register. Clearing ofthestatus flags willdeassert thenon-maskable interrupt generated dueto
violation oftheDWWD.
4 END TIME VIOL Windowed Watchdog End Time Violation Status.
This bitindicates whether theWatchdog counter expired.
0 Read: Noend-time window violation hasoccurred.
Write: Leaves thecurrent value unchanged.
1 Read :End-time defined bythewindowed watchdog configuration hasbeen violated.
Write: Bitiscleared to0.
3 START TIME VIOL Windowed Watchdog Start Time Violation Status.
This bitindicates whether thekeyiswritten before thewatchdog window opened up.
0 Read: Nostart-time window violation hasoccurred.
Write: Leaves thecurrent value unchanged.
1 Read: Start-time defined bythewindowed watchdog configuration hasbeen violated.
Write: Bitiscleared to0.
2 KEY ST Watchdog keystatus. This bitindicates areset orNMI generated byawrong keyorkey
sequence written totheRTIWDKEY register.
0 Read: Nowrong keyorkey-sequence written.
Write: Bitisunchanged.
1 Read: Wrong keyorkey-sequence written toRTIWDKEY register.
Write: Bitiscleared to0.
1 DWD ST DWD status.
This bitisequivalent tobitEND TIME VIOL.
0 Read: Noreset orNMI was generated.
Write: Bitisunchanged.
1 Read: Reset orNMI was generated.
Write: Bitiscleared to0.
0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 619 -->

www.ti.com RTIControl Registers
619 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.31 RTIWatchdog Key Register (RTIWDKEY)
This register must bewritten with thecorrect written keyvalues toserve thewatchdog. This register is
shown inFigure 17-42 anddescribed inTable 17-32 .
NOTE: Ithastobetaken intoaccount thatthewrite totheRTIWDKEY register takes 3VCLK cycles.
Figure 17-42. RTIWatchdog KeyRegister (RTIDWDKEY) [offset =9Ch]
31 16
Reserved
R-0
15 0
WDKEY
R/WP-A35Ch
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-32. RTIWatchdog KeyRegister (RTIDWDKEY) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0andwrites have noeffect.
15-0 WDKEY 0-FFFFh Watchdog key. These bitsprovide thekeysequence location.
Reads returns thecurrent WDKEY value.
Awrite ofE51Ah followed byA35Ch intwoseparate write operations defines thekeysequence
andreloads theDWD. Writing anyother value causes areset orNMI, asshown inTable 17-33 .
Writing anyother value willcause theWDKEY toreset toA35Ch.
Table 17-33. Example ofaWDKEY Sequence
Step Value Written toWDKEY Result
1 A35Ch Noaction
2 A35Ch Noaction
3 E51Ah WDKEY isenabled forreset orNMI bynext A35Ch.
4 E51Ah WDKEY isenabled forreset orNMI bynext A35Ch.
5 E51Ah WDKEY isenabled forreset orNMI bynext A35Ch.
6 A35Ch Watchdog isreset.
7 A35Ch Noaction
8 E51Ah WDKEY isenabled forreset orNMI bynext A35Ch.
9 A35Ch Watchdog isreset.
10 E51Ah WDKEY isenabled forreset orNMI bynext A35Ch.
11 2345h System reset orNMI; incorrect value written toWDKEY.

<!-- Page 620 -->

RTIControl Registers www.ti.com
620 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.32 RTIDigital Watchdog Down Counter (RTIDWDCNTR)
This register provides thecurrent value oftheDWD down counter. This register isshown inFigure 17-43
anddescribed inTable 17-34 .
Figure 17-43. RTIWatchdog Down Counter Register (RTIDWDCNTR) [offset =A0h]
31 25 24 16
Reserved DWDCNTR
R-0 R-1FFh
15 0
DWDCNTR
R-FFFFh
LEGEND: R=Read only; -n=value after reset
Table 17-34. RTIWatchdog Down Counter Register (RTIDWDCNTR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0andwrites have noeffect.
24-0 DWDCNTR 0-1FF FFFFh DWD down counter.
Reads return thecurrent counter value.
17.3.33 Digital Windowed Watchdog Reaction Control (RTIWWDRXNCTRL)
This register selects theDWWD reaction ifthewatchdog isserviced outside thetime window. This register
isshown inFigure 17-44 anddescribed inTable 17-35 .
Figure 17-44. Digital Windowed Watchdog Reaction Control (RTIWWDRXNCTRL) [offset =A4h]
31 16
Reserved
R-0
15 4 3 0
Reserved WWDRXN
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-35. Digital Windowed Watchdog Reaction Control (RTIWWDRXNCTRL) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0andwrites have noeffect.
3-0 WWDRXN The DWWD reaction
5h The windowed watchdog willcause areset ifthewatchdog isserviced outside thetime window
defined bytheconfiguration, orifthewatchdog isnotserviced atall.
Ah The windowed watchdog willgenerate anon-maskable interrupt totheCPU ifthewatchdog is
serviced outside thetime window defined bytheconfiguration, orifthewatchdog isnotserviced
atall.
Allother values The windowed watchdog willcause areset ifthewatchdog isserviced outside thetime window
defined bytheconfiguration, orifthewatchdog isnotserviced atall.
Note: The DWWD reaction canbeselected bytheapplication even when theDWWD counter is
already enabled. Ifachange totheWWDRXN ismade before thewatchdog service window is
opened, then thechange intheconfiguration takes effect immediately. Ifachange tothe
WWDRXN ismade when thewatchdog service window isalready open, then thechange in
configuration takes effect only after thewatchdog isserviced.

<!-- Page 621 -->

www.ti.com RTIControl Registers
621 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.34 Digital Windowed Watchdog Window Size Control (RTIWWDSIZECTRL)
This register selects theDWWD window size. This register isshown inFigure 17-45 anddescribed in
Table 17-36 .
Figure 17-45. Digital Windowed Watchdog Window Size Control (RTIWWDSIZECTRL) [offset =A8h]
31 16
WWDSIZE
R/WP-0000
15 0
WWDSIZE
R/WP-0005h
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-36. Digital Windowed Watchdog Window Size Control (RTIWWDSIZECTRL)
Field Descriptions
Bit Field Value Description
31-0 WWDSIZE 0 The DWWD window size
0000 0005h 100% (The functionality isthesame asthestandard time-out digital watchdog.)
0000 0050h 50%
0000 0500h 25%
0000 5000h 12.5%
0005 0000h 6.25%
Allother values 3.125%
Note: The DWWD window size canbeselected bytheapplication even when theDWWD
counter isalready enabled. Ifachange totheWWDSIZE ismade before thewatchdog service
window isopened, then thechange intheconfiguration takes effect immediately. Ifachange to
theWWDSIZE ismade when thewatchdog service window isalready open, then thechange in
configuration takes effect only after thewatchdog isserviced.

<!-- Page 622 -->

RTIControl Registers www.ti.com
622 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.35 RTICompare Interrupt Clear Enable Register (RTIINTCLRENABLE)
When theRTIcompare event isconfigured togenerate aDMA request ortriggers (alltriggered byRTI
compare interrupt request flag) toother peripherals, itisoften desirable toclear theRTIcompare flag
automatically sothattherequests canbegenerated repeatedly without anyCPU intervention. This
register works with theRTIcompare clear registers toenable an"auto-clear" ofthecompare interrupt
enable bitafter acompare equal event. This register isshown inFigure 17-46 anddescribed inTable 17-
37.
Figure 17-46. RTICompare Interrupt Clear Enable Register (RTIINTCLRENABLE) [offset =ACh]
31 28 27 24 23 20 19 16
Reserved INTCLRENABLE3 Reserved INTCLRENABLE2
R-0 R/WP-5h R-0 R/WP-5h
15 12 11 8 7 4 3 0
Reserved INTCLRENABLE1 Reserved INTCLRENABLE0
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 17-37. RTICompare Interrupt Clear Enable Register (RTIINTCLRENABLE) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 INTCLRENABLE3 Enables theauto-clear functionality onthecompare 3interrupt.
5h Read: Auto-clear forcompare 3interrupt isdisabled.
Privileged Write: Auto-clear forcompare 3interrupt becomes disabled.
Allother values Read: Auto-clear forcompare 3interrupt isenabled.
Privileged Write: Auto-clear forcompare 3interrupt becomes enabled.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 INTCLRENABLE2 Enables theauto-clear functionality onthecompare 2interrupt.
5h Read: Auto-clear forcompare 2interrupt isdisabled.
Privileged Write: Auto-clear forcompare 2interrupt becomes disabled.
Allother values Read: Auto-clear forcompare 2interrupt isenabled.
Privileged Write: Auto-clear forcompare 2interrupt becomes enabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 INTCLRENABLE1 Enables theauto-clear functionality onthecompare 1interrupt.
5h Read: Auto-clear forcompare 1interrupt isdisabled.
Privileged Write: Auto-clear forcompare 1interrupt becomes disabled.
Allother values Read: Auto-clear forcompare 1interrupt isenabled.
Privileged Write: Auto-clear forcompare 1interrupt becomes enabled.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 INTCLRENABLE0 Enables theauto-clear functionality onthecompare 0interrupt.
5h Read: Auto-clear forcompare 0interrupt isdisabled.
Privileged Write: Auto-clear forcompare 0interrupt becomes disabled.
Allother values Read: Auto-clear forcompare 0interrupt isenabled.
Privileged Write: Auto-clear forcompare 0interrupt becomes enabled.

<!-- Page 623 -->

www.ti.com RTIControl Registers
623 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.36 RTICompare 0Clear Register (RTICMP0CLR)
This registers holds aninitial value which islarger than thevalue intheRTICompare 0register
Section 17.3.4 .The user needs tochoose thevalue such thatthecompare clear 0event occurs before
next compare 0event. IftheFree Running Counter matches thecompare value, thecompare 0interrupt
request flagiscleared andthevalue intheRTIUDCP0 register Section 17.3.16 isadded tothisregister.
This register isshown inFigure 17-47 anddescribed inTable 17-38 .
Figure 17-47. RTICompare 0Clear Register (RTICMP0CLR) [offset =B0h]
31 16
CMP0CLR
R/WP-0
15 0
CMP0CLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-38. RTICompare 0Clear Register (RTICMP0CLR) Field Descriptions
Bit Field Value Description
31-0 CMP0CLR 0-FFFF FFFFh Compare 0clear. This registers holds acompare value. IftheFree Running Counter matches
thecompare value, thecompare 0interrupt request flagiscleared andthevalue inthe
RTIUDCP0 register Section 17.3.16 isadded tothisregister.
Reads return thecurrent compare clear value.
Aprivileged write tothisregister updates thecompare clear value.
17.3.37 RTICompare 1Clear Register (RTICMP1CLR)
This registers holds aninitial value which islarger than thevalue intheRTICompare 1register
Section 17.3.4 .The user needs tochoose thevalue such thatthecompare clear 1event occurs before
next compare 1event. IftheFree Running Counter matches thecompare value, thecompare 1interrupt
request flagiscleared andthevalue intheRTIUDCP1 register Section 17.3.18 isadded tothisregister.
This register isshown inFigure 17-48 anddescribed inTable 17-39 .
Figure 17-48. RTICompare 1Clear Register (RTICMP1CLR) [offset =B4h]
31 16
CMP1CLR
R/WP-0
15 0
CMP1CLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-39. RTICompare 1Clear Register (RTICMP1CLR) Field Descriptions
Bit Field Value Description
31-0 CMP0CLR 0-FFFF FFFFh Compare 1clear. This registers holds acompare value. IftheFree Running Counter matches
thecompare value, thecompare 1interrupt request flagiscleared andthevalue inthe
RTIUDCP1 register Section 17.3.18 isadded tothisregister.
Reads return thecurrent compare clear value.
Aprivileged write tothisregister updates thecompare clear value.

<!-- Page 624 -->

RTIControl Registers www.ti.com
624 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedReal-Time Interrupt (RTI) Module17.3.38 RTICompare 2Clear Register (RTICMP2CLR)
This registers holds aninitial value which islarger than thevalue intheRTICompare 2register
Section 17.3.4 .The user needs tochoose thevalue such thatthecompare clear 2event occurs before
next compare 2event. IftheFree Running Counter matches thecompare value, thecompare 2interrupt
request flagiscleared andthevalue intheRTIUDCP2 register Section 17.3.20 isadded tothisregister.
This register isshown inFigure 17-49 anddescribed inTable 17-40 .
Figure 17-49. RTICompare 2Clear Register (RTICMP2CLR) [offset =B8h]
31 16
CMP2CLR
R/WP-0
15 0
CMP2CLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-40. RTICompare 2Clear Register (RTICMP2CLR) Field Descriptions
Bit Field Value Description
31-0 CMP2CLR 0-FFFF FFFFh Compare 2clear. This registers holds acompare value. IftheFree Running Counter matches
thecompare value, thecompare 2interrupt request flagiscleared andthevalue inthe
RTIUDCP2 register Section 17.3.20 isadded tothisregister.
Reads return thecurrent compare clear value.
Aprivileged write tothisregister updates thecompare clear value.
17.3.39 RTICompare 3Clear Register (RTICMP3CLR)
This registers holds aninitial value which islarger than thevalue intheRTICompare 3register
Section 17.3.4 .The user needs tochoose thevalue such thatthecompare clear 3event occurs before
next compare 3event. IftheFree Running Counter matches thecompare value, thecompare 3interrupt
request flagiscleared andthevalue intheRTIUDCP3 register Section 17.3.22 isadded tothisregister.
This register isshown inFigure 17-50 anddescribed inTable 17-41 .
Figure 17-50. RTICompare 3Clear Register (RTICMP3CLR) [offset =BCh]
31 16
CMP3CLR
R/WP-0
15 0
CMP3CLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 17-41. RTICompare 3Clear Register (RTICMP3CLR) Field Descriptions
Bit Field Value Description
31-0 CMP3CLR 0-FFFF FFFFh Compare 3clear. This registers holds acompare value. IftheFree Running Counter matches
thecompare value, thecompare 3interrupt request flagiscleared andthevalue inthe
RTIUDCP3 register Section 17.3.22 isadded tothisregister.
Reads return thecurrent compare clear value.
Aprivileged write tothisregister updates thecompare clear value.