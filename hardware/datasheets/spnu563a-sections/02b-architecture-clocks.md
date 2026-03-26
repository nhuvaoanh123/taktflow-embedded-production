# Architecture - Clocks

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 142-150

---


<!-- Page 142 -->

Clocks www.ti.com
142 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4 Clocks
This section describes theclocking structure oftheTMS570LC43x microcontrollers.
2.4.1 Clock Sources
The devices support upto7clock sources. These areshown inTable 2-9.The electrical specifications as
well astiming requirements foreach oftheclock sources arespecified inthedevice data manual.
Table 2-9.Clock Sources
Clock Source # Clock Source Name Description
0 OSCINMain oscillator. This istheprimary clock forthemicrocontroller andisthe
only clock thatisinput tothephase-locked loops. The oscillator frequency
must bebetween 5and20MHz.
1 PLL1This istheoutput ofthemain PLL. The PLL iscapable ofmodulating its
output frequency inacontrolled manner toreduce theradiated emissions.
2 ReservedThis clock source isnotavailable andmust notbeenabled orused as
source foranyclock domain.
3 EXTCLKIN1External clock input 1.Asquare wave input canbeapplied tothisdevice
input andused asaclock source inside thedevice.
4LFLPO
(Low-Frequency LPO)
(CLK80K)This isthelow-frequency output oftheinternal reference oscillator. This is
typically an80KHz signal (CLK80K) thatisused bythereal-time interrupt
module forgenerating periodic interrupts towake upfrom alowpower
mode.
5HFLPO
(High-Frequency LPO)
(CLK10M)This isthehigh-frequency output oftheinternal reference oscillator. This is
typically a10MHz signal (CLK10M) thatisused bytheclock monitor
module asareference clock tomonitor themain oscillator frequency.
6 PLL2This istheoutput ofthesecond PLL. There isnooption ofmodulating this
PLL'soutput signal. This separate non-modulating PLL allows the
generation ofanasynchronous clock source thatisindependent ofthe
CPU clock frequency.
7 EXTCLKIN2External clock input 2.Asquare wave input canbeapplied tothisdevice
input andused asaclock source inside thedevice.
2.4.1.1 Enabling /Disabling Clock Sources
Each clock source canbeindependently enabled ordisabled using thesetofClock Source Disable
registers -CSDIS, CSDISSET andCSDISCLR.
Each bitinthese registers corresponds totheclock source number indicated inTable 2-9.Forexample,
setting bit1intheCSDIS orCSDISSET registers disables thePLL#1.
NOTE: Disabling theMain Oscillator orHFLPO
Bydefault, theclock monitoring circuit isenabled andchecks forthemain oscillator
frequency tobewithin acertain range using theHFLPO asareference. Ifthemain oscillator
and/or theHFLPO aredisabled with theclock monitoring stillenabled, theclock monitor will
indicate anoscillator fault. The clock monitoring must bedisabled before disabling themain
oscillator ortheHFLPO clock source(s).
The clock source isonly disabled once there isnoactive clock domain thatisusing thatclock source. Also
check the"Oscillator andPLL"user guide formore information onenabling /disabling theoscillator and
PLL.
OntheTMS570LC43x microcontrollers, theclock sources 0,4,and5areenabled bydefault.

<!-- Page 143 -->

www.ti.com Clocks
143 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.1.2 Checking forValid Clock Sources
The application cancheck whether aclock source isvalid ornotbychecking thecorresponding bittobe
setintheClock Source Valid Status (CSVSTAT) register. Forexample, theapplication cancheck ifbit1in
CSVSTAT issetbefore using theoutput ofPLL#1 asthesource foranyclock domain.
2.4.2 Clock Domains
The clocking onthisdevice isdivided intomultiple clock domains forflexibility incontrol aswell asclock
source selection. There are10clock domains onthisdevice. Each ofthese aredescribed inTable 2-10 .
Each ofthecontrol registers listed inTable 2-10 aredefined inSection 2.5.The ACtiming characteristics
foreach clock domain arespecified inthedevice data manual.
Table 2-10. Clock Domains
Clock DomainClock Disable
BitDefault
SourceSource Selection
Register Special Considerations
GCLK1 CDDIS.0 OSCIN GHVSRC[3:0]*This themain clock from which HCLK isdivided
down
*Inphase with HCLK
*Isdisabled separately from HCLK viatheCDDISx
registers bit0
*Can bedivided by1upto8when running CPU self-
test(LBIST) using theCLKDIV field ofthe
STCCLKDIV register ataddress 0xFFFFE108
HCLK CDDIS.1 OSCIN GHVSRC[3:0]*Divided from GCLK1 viaHCLKCNTL register
*Allowable clock ratio from 1:1to4:1
*Isdisabled viatheCDDISx registers bit1
VCLK CDDIS.2 OSCIN GHVSRC[3:0]*Divided down from HCLK viaCLKCNTL register
*Can beHCLK/1, HCLK/2,... orHCLK/16
*Isdisabled separately from HCLK viatheCDDISx
registers bit2
*HCLK:VCLK2:VCLK must beinteger ratios ofeach
other
VCLK2 CDDIS.3 OSCIN GHVSRC[3:0]*Divided down from HCLK
*Can beHCLK/1, HCLK/2,... orHCLK/16
*Frequency must beaninteger multiple ofVCLK
frequency
*Isdisabled separately from HCLK viatheCDDISx
registers bit3
VCLK3 CDDIS.8 OSCIN GHVSRC[3:0]*Divided down from HCLK
*Can beHCLK/1, HCLK/2,... orHCLK/16
*HCLK:VCLK3 must beinteger ratios ofeach other
*Isdisabled separately from HCLK viatheCDDISx
registers bit8
VCLKA1 CDDIS.4 VCLK VCLKASRC[3:0]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit4
VCLKA2 CDDIS.5 VCLK VCLKASRC[3:0]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit5
VCLKA4 CDDIS.11 VCLK VCLKACON1[19:16]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit11
VCLKA4_DIVR VCLKACON1.20 VCLK VCLKACON1[19:16]*Divided down from VCLKA4 using theVCLKA4R
field oftheVCLKACON1 register
*Frequency canbeVCLKA4/1, VCLKA4/2, ...,or
VCLKA4/8
*Default frequency isVCLKA4/2
*Isdisabled separately viatheVCLKACON1
register 'sVCLKA4_DIV_CDDIS bit,iftheVCLKA4 is
notalready disabled

<!-- Page 144 -->

Clocks www.ti.com
144 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-10. Clock Domains (continued)
Clock DomainClock Disable
BitDefault
SourceSource Selection
Register Special Considerations
RTICLK1 CDDIS.6 VCLK RCLKSRC[3:0]*Defaults toVCLK asthesource
*Ifaclock source other than VCLK isselected for
RTICLK1, then theRTICLK1 frequency must beless
than orequal toVCLK/3
*Application canensure thisbyprogramming the
RTI1DIV field oftheRCLKSRC register, ifnecessary
*Isdisabled viatheCDDISx registers bit6
2.4.2.1 Enabling /Disabling Clock Domains
Each clock domain canbeindependently enabled ordisabled using thesetofClock Domain Disable
registers -CDDIS, CDDISSET, andCDDISCLR.
Each bitinthese registers corresponds totheclock domain number indicated inTable 2-10.Forexample,
setting bit1intheCDDIS orCDDISSET registers disables theHCLK clock domain. The clock domain will
beturned offonly when every module thatuses theHCLK domain gives the"permission "forHCLK tobe
turned off.
Allclock domains areenabled bydefault, orupon asystem reset, orwhenever awake upcondition is
detected.
2.4.2.2 Mapping Clock Sources toClock Domains
Each clock domain needs tobemapped toavalid clock source. There arecontrol registers thatallow an
application tochoose theclock sources foreach clock domain.
*Selecting clock source forGCLK1, HCLK, andVCLKx domains
The CPU clock (GCLK1), thesystem module clock (HCLK), andtheperipheral busclocks (VCLKx) alluse
thesame clock source. This clock source isselected viatheGHVSRC register. The default source forthe
GCLK1, HCLK, andVCLKx isthemain oscillator. That is,after power up,theGCLK1 andHCLK are
running attheOSCIN frequency, while theVCLKx frequency istheOSCIN frequency divided by2.
*Selecting clock source forVCLKA1 andVCLKA2 domains
The clock source forVCLKA1 andVCLKA2 domains isselected viatheVCLKASRC register. The default
source fortheVCLKA1 andVCLKA2 domains istheVCLK.
*Selecting clock source forVCLKA4 domain
The clock source forVCLKA4 domain isselected viatheVCLKACON1 register. The default source forthe
VCLKA4 domain istheVCLK.
*Selecting clock source forRTICLK1 domain
The clock source forRTICLK1 domain isselected viatheRCLKSRC register. The default source forthe
RTICLK1 domain istheVCLK.
NOTE: Selecting aclock source forRTICLK1 thatisnotVCLK
When theapplication chooses aclock source forRTICLK1 domain thatisnotVCLK, then the
application must ensure thattheresulting RTICLK1 frequency must beless than orequal to
VCLK frequency divided by3.The application canconfigure theRTI1DIV field ofthe
RCLKSRC register fordividing theselected clock source frequency by1,2,4or8tomeet
thisrequirement.

<!-- Page 145 -->

www.ti.com Clocks
145 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.3 Low Power Modes
Allclock domains areactive inthenormal operating mode. This isthedefault mode ofoperation. As
described inSection 2.4.1.1 andSection 2.4.2.1 ,theapplication canchoose todisable anyparticular clock
source anddomain thatitdoes notplan touse. Also, theperipheral central resource controller (PCR) has
control registers toenable /disable theperipheral clock (VCLK) foreach peripheral select. This offers the
application alarge number ofchoices forenabling /disabling clock sources, orclock domains, orclocks to
specific peripherals.
This section describes three particular low-power modes andtheir typical characteristics. They arenotthe
only low-power modes configurable bytheapplication, asjustdescribed.
Table 2-11. Typical Low-Power Modes
Mode
NameActive Clock
Source(s)Active
Clock
Domain(s)Wake UpOptionsSuggested
Wake Up
Clock
Source(s)Wake UpTime(wake updetected -to-CPU
code execution start)
Doze Main oscillator RTICLK1RTIinterrupt,
GIO interrupt,
CAN message,
SCImessageMain oscillatorFlash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time
Snooze LFLPO RTICLK1RTIinterrupt,
GIO interrupt,
CAN message,
SCImessageHFLPOHFLPO warm start-up time
+
Flash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time
Sleep None NoneGIO interrupt,
CAN message,
SCImessageHFLPOHFLPO warm start-up time
+
Flash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time

<!-- Page 146 -->

Clocks www.ti.com
146 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.3.1 Typical Software Sequence toEnter aLow-Power Mode
1.Disable allnon-CPU busmasters sothey donotcarry outanyfurther bustransactions.
2.Program theflash banks andflash pump fall-back modes tobe"sleep ".
The flash pump transitions from active tosleep mode only after alltheflash banks have switched from
active tosleep mode.
3.Disable theclock sources thatarenotrequired tobekept active.
Aclock source does notgetdisabled until allclock domains using thatclock source aredisabled first,
orareconfigured touseanalternate clock source.
4.Disable theclock domains thatarenotrequired tobekept active.
Aclock domain does notgetdisabled until allmodules using thatclock domain "give their permission "
forthatclock domain tobeturned off.
5.IdletheCortex-R5F core.
The ARM Cortex-R5F CPU hasinternal power management logic, andrequires adedicated instruction
tobeused inorder toenter alowpower mode. This istheWait ForInterrupt (WFI) instruction.
When aWFI instruction isexecuted, theCortex-R5F core flushes itspipeline, flushes allwrite buffers,
andcompletes allpending bustransactions. Atthistime thecore indicates tothesystem thattheclock
tothecore canbestopped. This indication isused bytheGlobal Clock Module (GCM) toturn offthe
CPU clock domain (GCLK1) iftheCDDIS register bit0isset.
2.4.3.2 Special Considerations forEntry toLow Power Modes
Some busmaster modules -DMA, High-End Timer Transfer Units (HTUx), FlexRay Transfer Unit (FTU),
andParameter Overlay Module (POM), canhave ongoing transactions when theapplication wants to
enter alowpower mode toturn offtheclocks tothose modules. This isnotrecommended asitcould
leave thedevice inanunpredictable state. Refer totheindividual module user guides formore information
about thesequence tobefollowed tosafely enter alow-power mode.
2.4.3.3 Selecting Clock Source Upon Wake Up
The domains forCPU clock (GCLK1), thesystem clock (HCLK) andtheperipheral clock (VCLKx) usethe
same clock source selected viatheGHVSRC field oftheGHVSRC register. The GHVSRC register also
allows theapplication tochoose theclock source after wake upviatheGHVWAKE field.
When awake upcondition isdetected, iftheselected wake upclock source isnotalready active, the
global clock module (GCM) willenable thisselected clock source, wait forittobecome valid, andthen use
itfortheGCLK1, HCLK, andVCLKx domains. The other clock domains VCLKAx andRTICLK1 retain the
configuration fortheir clock source selection registers -VCLKASRC, VCLKACON1 andRCLKSRC.
2.4.4 Clock Test Mode
The TMS570LC43x microcontrollers support atestmode which allows auser tobring outseveral different
clock sources andclock domains ontotheECLK1 terminal inaddition tooutputting theexternal clock.
This isvery useful information fordebug purposes. Each clock source also hasacorresponding clock
source valid status flagintheClock Source Valid Status (CSVSTAT) register. The clock source valid
status flags canalso bebrought outontotheN2HET1[12] terminal inthisclock testmode.
The clock testmode iscontrolled bytheCLKTEST register inthesystem module register frame (see
Section 2.5.1.31 ).
The clock testmode isenabled bywriting 0x5totheCLK_TEST_EN field.

<!-- Page 147 -->

www.ti.com Clocks
147 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureThe signal tobebrought outontotheECLK1 terminal isdefined bytheSEL_ECP_PIN field, andthe
signal tobebrought outontotheN2HET1[12] terminal isdefined bytheSEL_GIO_PIN field. The choices
forthese selections aredefined inTable 2-12.
Table 2-12. Clock Test Mode Options
SEL_ECP_PIN Signal onECLK SEL_GIO_PIN Signal onN2HET1[12]
00000 Oscillator clock 0000 Oscillator Valid Status
00001 PLL1 clock output 0001 PLL1 Valid Status
00010 Reserved 0010 Reserved
00011 EXTCLKIN1 0011 Reserved
00100Low-frequency LPO (Low-Power
Oscillator) clock [CLK80K]0100 Reserved
00101High-frequency LPO (Low-Power
Oscillator) clock [CLK10M]0101HFLPO Clock Output Valid Status
[CLK10M]
00110 PLL2 clock output 0110 PLL2 Valid Status
00111 EXTCLKIN2 0111 Reserved
01000 GCLK1 1000LFLPO Clock Output Valid Status
[CLK80K]
01001 RTI1 Base 1001 Oscillator Valid Status
01010 Reserved 1010 Oscillator Valid Status
01011 VCLKA1 1011 Oscillator Valid Status
01100 VCLKA2 1100 Oscillator Valid Status
01101 Reserved 1101 Reserved
01110 VCLKA4_DIVR 1110 VCLKA4
01111 Flash HDPump Oscillator 1111 Oscillator Valid Status
10000 Reserved
10001 HCLK
10010 VCLK
10011 VCLK2
10100 VCLK3
10101-10110 Reserved
10111 EMAC clock output
11000-11111 Reserved

<!-- Page 148 -->

Clocks www.ti.com
148 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.5 Embedded Trace Macrocell (ETM-R5)
The TMS570LC43x microcontrollers contain anETM-R5 module with a32-bit internal data port. The ETM-
R5module isconnected toaTrace Port Interface Unit (TPIU) with a32-bit data bus; theTPIU provides a
35-bit (32-bit data and3-bit control) external interface fortrace. The ETM-R5 isCoreSight compliant and
follows theETM v3specification. Formore details ontheETM-R5 specification, refer totheEmbedded
Trace Macrocell Architecture Specification .
The ETM clock source isselected aseither VCLK ortheexternal ETMTRACECLKIN pin.The selection is
done bytheEXTCTLOUT control bitsoftheTPIU EXTCTL_Out_Port register. The address ofthisregister
isTPIU base address +0x404.
Before youbegin accessing TPIU registers, theTPIU should beunlocked viatheCoreSight keyand1hor
2hshould bewritten tothisregister.
Figure 2-6.EXTCTL_Out_Port Register [offset =404h]
31 16
Reserved
R-0
15 2 1 0
Reserved EXTCTLOUT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-13. EXTCTL_Out_Port Register Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 EXTCTLOUT EXTCTL output control.
0 Tied-zero
1h VCLK
2h ETMTRACECLKIN
3h Tied-zero
2.4.6 Safety Considerations forClocks
The TMS570LC43x microcontrollers aretargeted foruseinseveral safety-critical applications. The
following sections describe theinternal orexternal monitoring mechanisms thatdetect andsignal clock
source failures.

<!-- Page 149 -->

guaranteed faillower
thresholdguaranteed passupper
thresholdguaranteed fail
1.375 4.875 22 78f[MHz]
www.ti.com Clocks
149 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.6.1 Oscillator Monitor
The oscillator clock frequency ismonitored byadedicated circuitry called CLKDET using theHFLPO as
thereference clock. The CLKDET flags anoscillator failcondition whenever theOSCIN frequency falls
outside ofarange which isdefined bytheHFLPO frequency.
Thevalid OSCIN range isdefined asaminimum off(HF LPO)/4toamaximum off(HF LPO)×4.
The untrimmed HFLPO frequency onthisdevice canrange from 5.5MHz to19.5 MHz. This results ina
valid OSCIN frequency range depicted inFigure 2-7.
The application canselect thedevice response toanoscillator failindication. Refer toChapter 14formore
details ontheoscillator monitoring andthesystem response choices.
Figure 2-7.LPO andClock Detection, Untrimmed HFLPO
2.4.6.2 PLL SlipDetector
Both thePLL macros implemented onthemicrocontrollers have anembedded slipdetection circuit. APLL
slipisdetected bytheslipdetector under thefollowing conditions:
1.Reference cycle slip, RFSLIP --theoutput clock isrunning toofastrelative tothereference clock
2.Feedback cycle slip, FBSLIP --theoutput clock isrunning tooslow relative tothereference clock
The device also includes optional filters thatcanbeenabled before aslipindication from thePLL is
actually logged inthesystem module Global Status Register (GLBSTAT). Also, once aPLL slipcondition
islogged inthesystem module global status register, theapplication canchoose thedevice 'sresponse to
theslipindication. Refer toChapter 14formore details onPLL slipandthesystem response choices.
2.4.6.3 External Clock Monitor
The microcontrollers support twoterminals called ECLK1 andECLK2 -External Clock, which areused to
output aslow frequency which isdivided down from thedevice system clock frequency. Anexternal circuit
canmonitor theECLK1 and/or ECLK2 frequency inorder tocheck thatthedevice isoperating atthe
correct frequency.
The frequency ofthesignal output ontheECLKx pincanbedivided down by1to65536 from the
peripheral clock (VCLK) frequency using theExternal Clock Prescaler Control Register (ECPCNTL) for
ECLK1 andECPCNTL1 forECLK2. The actual clock output onECLK1 isenabled bysetting theECP CLK
FUN bitoftheSYSPC1 control register. Bydefault, theECLK1 terminal isinGIO mode. ECLK2
functionality canbeenabled bywriting 5htotheECP_KEY field oftheECPCNTL1 register.
NOTE: ECLK2 ismultiplexed with EMIF_CLK andECLK2 isnotaprimary function after reset. User
willneed toselect ECLK2 tobebrought outtotheterminal using IOMM module.
2.4.6.4 Dual-Clock Comparators
The microcontrollers include twoinstances ofthedual-clock comparator (DCC) module. This module
includes twodown counters which independently count from twoseparate seed values attherate oftwo
independent clock frequencies. One oftheclock inputs isareference clock input, selectable between the
main oscillator ortheHFLPO infunctional mode. The second clock input isselectable from among aset
ofdefined signals asdescribed inSection 2.4.6.4.1 andSection 2.4.6.4.2 .This mechanism canbeused to
useaknown-good clock tomeasure thefrequency ofanother clock.

<!-- Page 150 -->

Clocks www.ti.com
150 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.6.4.1 DCC1
Ascanbeseen, themain oscillator (OSCIN) canbeused forcounter 0asa"known-good "reference
clock. The clock forcounter 1canbeselected from among 8options. Refer totheDCC module chapter
formore details ontheDCC usage.
Table 2-14. DCC1 Counter 0Clock Inputs
Clock Source [3-0] Clock /Signal Name
Allother values oscillator (OSCIN)
5h HFLPO
Ah testclock (TCK)
Table 2-15. DCC1 Counter 1Clock /Signal Inputs
Key[3-0] Clock Source [3-0] Clock /Signal Name
Ah0h PLL1 free-running clock output
1h PLL2 free-running clock output
2h LFLPO
3h HFLPO
4h Flash pump oscillator
5h EXTCLKIN1
6h EXTCLKIN2
7 Reserved
8h-Fh VCLK
Allother values anyvalue N2HET1[31]
2.4.6.4.2 DCC2
Ascanbeseen, themain oscillator (OSCIN) canbeused forcounter 0asa"known-good "reference
clock. The clock forcounter 1canbeselected from among 2options. Refer totheDCC module chapter
formore details ontheDCC usage.
Table 2-16. DCC2 Counter 0Clock Inputs
Clock Source [3-0] Clock /Signal Name
others oscillator (OSCIN)
0xA testclock (TCK)
Table 2-17. DCC2 Counter 1Clock /Signal Inputs
Key[3-0] Clock Source [3-0] Clock /Signal Name
Ah0h Reserved
1h PLL2 post_ODCLK/8
2h PLL2 post_ODCLK/16
3h-7h Reserved
8h-Fh VCLK
Allother values anyvalue N2HET2[0]