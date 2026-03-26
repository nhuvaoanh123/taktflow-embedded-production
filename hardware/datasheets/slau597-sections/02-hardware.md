# Hardware

> **Source**: `slau597-launchxl2-570lc43-user-guide.pdf` | Pages 5-19
>
> Auto-extracted via PyPDF2. Formatting is approximate.

---

<!-- Page 5 -->
Target Device
MSP432P401RCrystal
48 MHzMicro‐B
USB
EnergyTrace+
Current
Measure HW
LDO
5 V, 3.3 VESD
Protection
Power
SwitchDebug
MCULED
Red, Green
Power, UART, JTAG to Target
User Interface
Buttons and LEDs40‐pin LaunchPad
standard headers
{EnergyTrace Technology
Real-time power consumption
readings and state updates from the
MSP432P401R MCU viewable
through the EnergyTrace GUI
40-pin BoosterPack
plug-in module connector
(J1-J4){Button/Switch
S2
User LEDs
LED1 and LED2Button/Switch
S1
Fanout of Unused Pins
- Access to unused pins on the
MSP432P401R device
- Support for bread-board connectionMSP432P401R Microcontroller
MSP1
{
Jumper Isolation Block
- J101
- Power
- GND, 5V, and 3V3
- Back-channel UART to the PC
- RXD, TXD
- JTAG
- RST, TMS, TCK, TDO, TDIXDS110 onboard debug probe
Enables debugging and programming
as well as communication to the PC.
The XDS110 can also provide power
to the target MCU.Reset
MSP432P401R Reset
www.ti.com Hardware
5 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2 Hardware
Figure 2shows anoverview oftheEVM hardware.
Figure 2.MSP-EXP432P401R Overview
2.1 Block Diagram
Figure 3shows theblock diagram.
Figure 3.Block Diagram

---

<!-- Page 6 -->
1 P10.1/UCB3CLK
2 P10.2/UCB3SIMO/UCB3SDA
3 P10.3/UCB3SOMI/UCB3SCL
4 P1.0/UCA0STE
5 P1.1/UCA0CLK
6 P1.2/UCA0RXD/UCA0SOMI
7 P1.3/UCA0TXD/UCA0SIMO
8 P1.4/UCB0STE
9 P1.5/UCB0CLK
10 P1.6/UCB0SIMO/UCB0SDA
11 P1.7/UCB0SOMI/UCB0SCL
12 VCORE
13 DVCC1
14 VSW
15 DVSS1
16 P2.0/PM_UCA1STE
17 P2.1/PM_UCA1CLK
18 P2.2/PM_UCA1RXD/PM_UCA1SOMI
19 P2.3/PM_UCA1TXD/PM_UCA1SIMO
20 P2.4/PM_TA0.1
21 P2.5/PM_TA0.2
22 P2.6/PM_TA0.3
23 P2.7/PM_TA0.4
24 P10.4/TA3.0/C0.7
25 P10.5/TA3.1/C0.6
26
P7.4/PM_TA1.4/C0.527
P7.5/PM_TA1.3/C0.428
P7.6/PM_TA1.2/C0.329
P7.7/PM_TA1.1/C0.230
P8.0/UCB3STE/TA1.0/C0.131
P8.1/UCB3CLK/TA2.0/C0.032
P3.0/PM_UCA2STE33
P3.1/PM_UCA2CLK34
P3.2/PM_UCA2RXD/PM_UCA2SOMI35
P3.3/PM_UCA2TXD/PM_UCA2SIMO36
P3.4/PM_UCB2STE37
P3.5/PM_UCB2CLK38
P3.6/PM_UCB2SIMO/PM_UCB2SDA39
P3.7/PM_UCB2SOMI/PM_UCB2SCL40
AVSS341
PJ.0/LFXIN42
PJ.1/LFXOUT43
AVSS144
DCOR45
AVCC146
P8.2/TA3.2/A2347
P8.3/TA3CLK/A2248
P8.4/A2149
P8.5/A2050
P8.6/A1951 P8.7/A1852 P9.0/A1753 P9.1/A1654 P6.0/A1555 P6.1/A1456 P4.0/A1357 P4.1/A1258 P4.2/ACLK/TA2CLK/A1159 P4.3/MCLK/RTCCLK/A1060 P4.4/HSMCLK/SVMHOUT/A961 P4.5/A862 P4.6/A763 P4.7/A664 P5.0/A565 P5.1/A466 P5.2/A367 P5.3/A268 P5.4/A169 P5.5/A070 P5.6/TA2.1/VREF+/VeREF+/C1.771 P5.7/TA2.2/VREF-/VeREF-/C1.672 DVSS273 DVCC274 P9.2/TA3.375 P9.3/TA3.476
P6.2/UCB1STE/C1.5
77
P6.3/UCB1CLK/C1.4
78
P6.4/UCB1SIMO/UCB1SDA/C1.3
79
P6.5/UCB1SOMI/UCB1SCL/C1.2
80
P6.6/TA2.3/UCB3SIMO/UCB3SDA/C1.1
81
P6.7/TA2.4/UCB3SOMI/UCB3SCL/C1.0
82
DVSS3
83
RSTn/NMI
84
AVSS2
85
PJ.2/HFXOUT
86
PJ.3/HFXIN
87
AVCC2
88
P7.0/PM_SMCLK/PM_DMAE0
89
P7.1/PM_C0OUT/PM_TA0CLK
90
P7.2/PM_C1OUT/PM_TA1CLK
91
P7.3/PM_TA0.0
92
PJ.4/TDI
93
PJ.5/TDO/SWO
94
SWDIOTMS
95
SWCLKTCK
96
P9.4/UCA3STE
97
P9.5/UCA3CLK
98
P9.6/UCA3RXD/UCA3SOMI
99
P9.7/UCA3TXD/UCA3SIMO
100
P10.0/UCB3STE
Hardware www.ti.com
6 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.2 SimpleLink MSP432P401R MCU
The MSP432P401R isthefirstMSP432 family device featuring low-power performance with anARM
Cortex-M4F core. Device features include:
-Low-power ARM Cortex-M4F MSP432P401R
-Upto48-MHz system clock
-256KB offlash memory, 64KB ofSRAM, and32KB ofROM with SimpleLink MSP432 SDK libraries
-Four 16-bit timers with capture, compare, orPWM, two32-bit timers, andanRTC
-Uptoeight serial communication channels (I2C,SPI, UART, andIrDA)
-Analog: 14-bit SAR ADC, capacitive touch, comparator
-Digital: AES256, CRC, µDMA
Figure 4.MSP432P401RIPZ Pinout

---

<!-- Page 7 -->
www.ti.com Hardware
7 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.3 XDS110-ET Onboard Debug Probe
Tokeep development easy andcost effective, TI'sLaunchPad development kitsintegrate anonboard
debug probe, which eliminates theneed forexpensive programmers. The MSP-EXP432P401R hasthe
XDS110-ET debug probe, which isasimple low-cost debug probe thatsupports nearly allTIARM device
derivatives.
Figure 5.XDS110-ET Debug Probe
The XDS110-ET hardware canbefound intheschematics inSection 6andintheMSP-EXP432P401R
Hardware Design Files .
2.3.1 XDS110-ET Isolation Block J101
The J101 isolation block iscomposed ofJ101 jumpers shown inTable 1.The J101 isolation block allows
theuser toconnect ordisconnect signals thatcross from theXDS110-ET domain intotheMSP432P401R
target domain. This crossing isshown bythesilkscreen dotted lineacross theLaunchPad development kit
through J101. Noother signals cross thedomain, sotheXDS110-ET canbecompletely decoupled from
theMSP432P401R target side. This includes XDS110-ET power andGND signals, UART, andJTAG
signals.
Table 1lists thesignals thatarecontrolled attheisolation block.
Table 1.Isolation Block Connections
Signal Description
GNDGND power connection between XDS110 andMSP432 target GND planes. The GND jumper is
populated toconnect theseparate GND planes. This connection isrequired forproper operation with
3V3, 5V,UART, andJTAG.
5V 5-Vpower rail,VBUS from USB
3V3 3.3-V power rail,derived from VBUS byanLDO intheXDS110-ET domain
RXD <<Backchannel UART: The target MCU receives data through thissignal. The arrows indicate thedirection
ofthesignal.
TXD >>Backchannel UART: The target MCU sends data through thissignal. The arrows indicate thedirection of
thesignal.
RST MCU RST signal (active low)
TCK_SWCLK Serial wire clock input (SWCLK) /JTAG clock input (TCK)
TMS_SWDIO Serial wire data input/output (SWDIO) /JTAG testmode select (TMS)
TDO_SWO Serial wire trace output (SWO) /JTAG trace output (TWO) (Also PJ.5)
TDI JTAG testdata input (Also PJ.4)

---

<!-- Page 8 -->
XDS110-ET 
Debug Probe 
MCU
J101
Isolation 
BlockJTAG and SWD Application UART3.3V Power5V Power
MSP432P401R 
Target MCUXDS110-ET MSP432 TargetUSB Connector
in       out
LDOBoosterPack Header
BoosterPack HeaderUSB
EnergyTrace
eUSCI_A0
Hardware www.ti.com
8 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Reasons toopen these connections:
-Toremove anyandallinfluence from theXDS110-ET debug probe forhigh accuracy target power
measurements
-Tocontrol 3-Vand5-Vpower flow between theXDS110-ET andtarget domains
-Toexpose thetarget MCU pins forother usethan onboard debugging andapplication UART
communication
-Toexpose theUART interface oftheXDS110-ET sothatitcanbeused fordevices other than the
onboard MCU.
Figure 6.XDS110-ET Isolation Block
2.3.2 Application (or"Backchannel ")UART
The XDS110-ET provides a"backchannel "UART-over-USB connection with thehost, which canbevery
useful during debugging andforeasy communication with aPC.
The backchannel UART allows communication with theUSB host thatisnotpart ofthetarget application 's
main functionality. This isvery useful during development, andalso provides acommunication channel to
thePChost side. This canbeused tocreate GUIs andother programs onthePCthatcommunicate with
theLaunchPad development kit.
The pathway ofthebackchannel UART isshown inFigure 7.The backchannel UART eUSCI_A0 is
independent oftheUART onthe40-pin BoosterPack plug-in module connector eUSCI_A2.

---

<!-- Page 9 -->
www.ti.com Hardware
9 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Onthehost side, avirtual COM port fortheapplication backchannel UART isgenerated when the
LaunchPad development kitenumerates onthehost. You canuseanyPCapplication thatinterfaces with
COM ports, including terminal applications likeHyperterminal orDocklight, toopen thisport and
communicate with thetarget application. You need toidentify theCOM port forthebackchannel. On
Windows PCs, Device Manager canassist.
Figure 7.Application Backchannel UART inDevice Manager
The backchannel UART istheXDS110 Class Application/User UART port. Inthiscase, Figure 7shows
COM156, butthisport canvary from onehost PCtothenext. After youidentify thecorrect COM port,
configure itinyour host application according toitsdocumentation. You canthen open theport andbegin
communication toitfrom thehost.
The XDS110-ET hasaconfigurable baud rate; therefore, itisimportant thatthePCapplication configures
thebaud rate tobethesame aswhat isconfigured ontheeUSCI_A0 backchannel UART.

---

<!-- Page 10 -->
Hardware www.ti.com
10 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.3.3 Using anExternal Debug Probe Instead oftheOnboard XDS110-ET
Many users have aspecific external debug probe thatthey prefer touse, andmay wish tobypass the
XDS110-ET debug probe toprogram theMSP432 target MCU. This isenabled byjumpers onisolation
block J101, andtheconnector J8.Using anexternal debug probe issimple, andfullJTAG access is
provided through J8.
1.Remove jumpers ontheJTAG signals ontheJ101 isolation block, including RST, TMS, TCK, TDO,
andTDI.
2.Plug anyARM debug probe intoJ8.
(a)J8follows theARM Cortex Debug Connector standard outlined inCortex-M Debug Connectors .
3.Plug USB power intotheLaunchPad development kit,orpower itexternally.
(a)Ensure thatthejumpers across 3V3 andGND areconnected ifusing USB power.
(b)External debug probes donotprovide power, theVCC pinisapower sense pin.
(c)More details onpowering theLaunchPad development kitcanbefound inSection 2.4.
2.3.4 Using theXDS110-ET Debug Probe With aDifferent Target
The XDS110-ET debug probe ontheLaunchPad development kitcaninterface tomost ARM Cortex-M
devices, notjusttheonboard target MSP432P410R device. This functionality isenabled bytheJ102 10-
pinCortex-M JTAG connector. The 10-pin cable canbepurchased from Digi-Key Electronics (sold
separately from theLaunchPad development kit).
Header J102 follows theCortex-M ARM standard; however, pin1isnotavoltage sense pin.The XDS110-
EToutputs only 3.3-V JTAG signals. Ifanother voltage level isneeded, theuser must provide level
shifters totranslate theJTAG signal voltages. Additionally, 3.3Vofoutput power canbesourced from the
XDS110-ET when jumper JP102 isconnected. This allows theXDS110-ET topower theexternal target at
3.3Vthrough pin1.Bydefault JP102 isnotpopulated asitdoes notexplicitly follow thestandard.
1.Remove jumpers ontheJTAG signals ontheJ101 isolation block, including RST, TMS, TCK, TDO,
andTDI.
2.Plug the10-pin cable intoJ102, andconnect toanexternal target a.J102 follows theARM Cortex
Debug Connector standard outlined inCortex-M Debug Connectors .
3.Plug USB power intotheLaunchPad development kit,orpower itexternally
(a)JTAG levels are3.3VONLY.
(b)3.3-V power canbesourced through J102 byshorting theJP102 jumper.
2.3.5 EnergyTrace+ Technology
EnergyTrace ™technology isanenergy-based code analysis toolthatmeasures anddisplays the
application 'senergy profile andhelps tooptimize itforultra-low power consumption.
MSP432 devices with built-in EnergyTrace+[CPU State] (orinshort EnergyTrace+) technology allow real-
time monitoring ofinternal device states while user program code executes.
EnergyTrace+ technology issupported ontheLaunchPad development kitMSP432P401R device +
XDS110-ET debug probe. EnergyTrace technology isavailable aspart ofTI'sCode Composer Studio IDE.
During application debug, additional windows areavailable forEnergyTrace technology.
Toenable EnergyTrace technology, goto:
-Window >Preferences >Code Composer Studio >Advanced Tools >EnergyTrace ™Technology
-Check theEnable Auto-Launch ontarget connect box

---

<!-- Page 11 -->
www.ti.com Hardware
11 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 8.EnergyTrace ™Technology Preferences

---

<!-- Page 12 -->
Hardware www.ti.com
12 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Starting adebug session willnow open EnergyTrace technology windows. These windows show energy,
power, profile, andstates togive theuser afullview oftheenergy profile oftheir application.
Figure 9.EnergyTrace ™Windows
This data allows theuser toseeexactly where andhow energy isconsumed intheir application.
Optimizations forenergy canbequickly made forthelowest power application possible.
OntheLaunchPad development kit,EnergyTrace technology measures thecurrent thatenters thetarget
side oftheLaunchPad development kit.This includes allBoosterPack plug-in modules plugged in,and
anything else connected tothe3V3 power rail.Formore information about powering theLaunchPad
development kit,seeSection 2.4.
Formore information about EnergyTrace technology, seewww.ti.com/tool/energytrace .
Formore details andquestions about setting upandusing EnergyTrace technology with the
MSP432P401R MCU, seetheCode Composer Studio ™IDE7.1+ forSimpleLink ™MSP432 ™
Microcontrollers User'sGuide .

---

<!-- Page 13 -->
www.ti.com Hardware
13 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.4 Power
The board was designed toaccommodate various powering methods, including through theonboard
XDS110-ET andfrom anexternal source orBoosterPack plug-in module.
Figure 10.MSP--EXP432P401R Power Block Diagram
2.4.1 XDS110-ET USB Power
The most common power-supply scenario isfrom USB through theXDS110-ET debug probe. This
provides 5-Vpower from theUSB andalso regulates thispower railto3.3VforXDS110-ET operation and
3.3Vtothetarget side oftheLaunchPad development kit.Power from theXDS110-ET iscontrolled by
theisolation block 3V3 jumper, ensure thisjumper isconnected forpower tobeprovided tothetarget
MCU side.
Under normal operation, theLDO ontheXDS110-ET cansupply upto500mAofcurrent tothetarget side
including anyBoosterPack plug-in modules plugged in.However, when debugging andusing the
EnergyTrace technology tool, thiscurrent islimited to75mAtotal. Beaware ofthiscurrent limitation when
using EnergyTrace technology.
2.4.2 BoosterPack Plug-in Module andExternal Power Supply
Header J6ispresent ontheboard tosupply external power directly. Itisimportant tocomply with the
device voltage operation specifications when supplying external power. The MSP432P401R hasan
operating range of1.62 Vto3.7V.More information canbefound inMSP432P401xx SimpleLink ™
Mixed-Signal Microcontrollers .

---

<!-- Page 14 -->
Hardware www.ti.com
14 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.5 Measure Current Draw ofMSP432 MCU
Tomeasure thecurrent draw oftheMSP432P401R MCU, usethe3V3 jumper onthejumper isolation
block. The current measured includes thetarget device andanycurrent drawn through theBoosterPack
plug-in module headers.
Tomeasure ultra-low power, follow these steps:
1.Remove the3V3 jumper intheisolation block, andattach anammeter across thisjumper.
2.Consider theeffect thatthebackchannel UART andanycircuitry attached totheMSP432P401R may
have oncurrent draw. Disconnect these attheisolation block ifpossible, oratleast consider their
current sinking andsourcing capability inthefinal measurement.
3.Make sure there arenofloating input I/Os. These cause unnecessary extra current draw. Every I/O
should either bedriven outor,ifitisaninput, should bepulled ordriven toahigh orlowlevel.
4.Begin target execution.
5.Measure thecurrent. Keep inmind thatifthecurrent levels arefluctuating, itmay bedifficult togeta
stable measurement. Itiseasier tomeasure quiescent states.
Forabetter look atthepower consumed intheapplication, useEnergyTrace+ Technology. EnergyTrace+
Technology allows theuser toseeenergy consumed astheapplication progresses. Formore details
about EnergyTrace+ Technology, seeSection 2.3.5 .
2.6 Clocking
The MSP-EXP432P401R provides external clocks inaddition totheinternal clocks inthedevice.
-Q1:32-kHz crystal (LFXTCLK)
-Q2:48-MHz crystal (HFXTCLK)
The 32-kHz crystal allows forlower LPM3 sleep currents andahigher-precision clock source than the
default internal 32-kHz REFOCLK. Therefore, thepresence ofthecrystal allows thefullrange oflow-
power modes tobeused.
The 48-MHz crystal allows thedevice torunatitsmaximum operating speed forMCLK andHSMCLK.
The MSP432P401R device hasseveral internal clocks thatcanbesourced from many clock sources.
Most peripherals onthedevice canselect which oftheinternal clocks tousetooperate atthedesired
speed.
The internal clocks inthedevice default totheconfiguration listed inTable 2.
Table 2.Default Clock Operation
Clock Default Clock Source Default Clock
FrequencyDescription
MCLK DCO 3MHzMaster Clock
Sources CPU andperipherals
HSMCLK DCO 3MHzSubsystem Master Clock
Sources peripherals
SMCLK DCO 3MHzLow-speed subsystem master clock
Sources peripherals
ACLKLFXT (orREFO ifno
crystal present)32.768 kHzAuxiliary clock
Sources peripherals
BCLKLFXT (orREFO ifno
crystal present)32.768 kHzLow-speed backup domain clock
Sources LPM peripherals
Formore information about configuring internal clocks andusing theexternal oscillators, seethe
MSP432P4xx SimpleLink ™Microcontrollers Technical Reference Manual .

---

<!-- Page 15 -->
www.ti.com Hardware
15 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.7 BoosterPack Plug-in Module Pinout
The MSP-EXP432P401R LaunchPad development kitadheres tothe40-pin LaunchPad development kit
pinout standard. Astandard was created toaidcompatibility between LaunchPad development kitand
BoosterPack plug-in module tools across theTIecosystem.
The 40-pin standard iscompatible with the20-pin standard thatisused byother LaunchPad development
kitsliketheMSP-EXP430FR4133 .This allows some subset offunctionality of40-pin BoosterPack plug-in
modules tobeused with 20-pin LaunchPad development kits.
While most BoosterPack plug-in modules arecompliant with thestandard, some arenot.The
MSP-EXP432P401R LaunchPad development kitiscompatible with all20-pin and40-pin BoosterPack
plug-in modules thatarecompliant with thestandard. Ifthereseller orowner oftheBoosterPack plug-in
module does notexplicitly indicate compatibility with theMSP-EXP432P401R LaunchPad development kit,
compare theschematic ofthecandidate BoosterPack plug-in module with theLaunchPad development kit
toensure compatibility. Keep inmind thatsometimes conflicts canberesolved bychanging the
MSP432P401R device pinfunction configuration insoftware. More information about compatibility can
also befound atwww.ti.com/launchpad .
Figure 11shows the40-pin pinout oftheMSP-EXP432P401R LaunchPad development kit.
Note thatsoftware configuration ofthepinfunctions plays aroleincompatibility. The MSP-EXP432P401R
LaunchPad development kitside ofthedashed lineinFigure 11shows allofthefunctions forwhich the
MSP432P401R device 'spins canbeconfigured. This canalso beseen intheMSP432P401R data sheet.
The BoosterPack plug-in module side ofthedashed lineshows thestandard. The MSP432P401R function
whose color matches theBoosterPack plug-in module function shows thespecific software-configurable
function bywhich theMSP-EXP432P401R LaunchPad development kitadheres tothestandard.

---

<!-- Page 16 -->
Hardware www.ti.com
16 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 11.LaunchPad ™Development KittoBoosterPack ™Plug-in Module Connector Pinout

---

<!-- Page 17 -->
www.ti.com Hardware
17 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.8 Design Files
2.8.1 Hardware Design Files
Schematics canbefound inSection 6.Alldesign files including schematics, layout, billofmaterials
(BOM), Gerber files, anddocumentation areavailable intheMSP-EXP432P401R Hardware Design Files .
2.9 Hardware Change Log
Table 3lists thehardware revisions.
Table 3.Hardware Change Log
PCB
RevisionDate Description MSP432P401R Device RevisionMSP-EXP432P401R Hardware and
Software Download Version (for
Downloading ZipPackages Only)
Rev 1.0 March 2015Preproduction
ReleaseXMS432P401R Rev B 2_00_00_03
Rev 2.0 June 2016Production Silicon
ReleaseXMS432P401R Rev Cor
MSP432P401R Rev C
(check device markings to
determine your version)3_00_00_03
2.9.1 MSP-EXP432P401R Rev1.0(Black) LaunchPad Development Kit
Asshown inTable 3,thiswas theinitially released LaunchPad development kit,with XMS432P401R Rev
Bsilicon. Connecting anydebugger tothisversion oftheMSP432 MCU willgenerate awarning telling the
user toupdate their silicon. TIwillcontinue tosupport thisboard fortheforeseeable future. However,
upgrading tothelatest LaunchPad development kitgets youallthesilicon upgrades andthefinal
production version ofDriver Library inROM.
Inaddition tothehardware being different, thedevice revision also changes with theLaunchPad
development kitrevisions. Because ofthis, youmust download asoftware package thatmatches your
exact hardware. The software example files forolder versions oftheLaunchPad development kitare
available from MSP-EXP432P401R Software Examples -navigate toprevious release versions according
toTable 3.
2.9.2 MSP-EXP432P401R Rev2.0(Red) LaunchPad Development Kit
Asshown inTable 3,thisistheupdated LaunchPad development kitfortheproduction silicon, with
XMS432P401R Rev CorMSP432P401R Rev Csilicon. Inaddition tothelatest silicon, several updates
were made totheLaunchPad development kithardware toenhance theuser experience (see
Section 2.9.2.1 ).

---

<!-- Page 18 -->
Hardware www.ti.com
18 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.9.2.1 MSP-EXP432P401R Updates
From theperspective oftheboard, allofthechanges areaesthetic ormake thekiteasier touse(see
Figure 12).Forexample, moving theuser buttons totheside oftheboard makes them easier toreach
when youhave aBoosterPack plug-in module connected tothetopoftheLaunchPad development kit.
The button placement haschanged, butthephysical button connections arethesame. The most
significant change istheaddition ofanextra 10-pin ARM JTAG connector. This connector letsyouusethe
LaunchPad development kitasastand-alone XDS110 debug probe.
Figure 12.Differences Between Rev1.0(Black) andRev2.0(Red)

---

<!-- Page 19 -->
www.ti.com Hardware
19 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)2.9.2.2 MSP432P401R Device Revision Differences
The primary MSP432P401R silicon differences arethedifferences between Rev. BandRev. Cdevices.
Fordetails ofthedifferences, seeMoving From Evaluation toProduction With SimpleLink ™
MSP432P401x Microcontrollers .
The firstshipments ofRev 2.0(Red) LaunchPad development kithave XMS432P401R Rev C.
preproduction silicon before thefinal production version ofMSP432P401R Rev C.silicon isreleased.
Which device isonaparticular LaunchPad development kitcanbedetermined bylooking atthemarkings
ontheMSP432P401R device. The XMS version have amarking of"XMS "instead of"MSP ".Fordetails
onthedifferences between thepreproduction andproduction silicon, seeXMS432P401x Rev. C
Preproduction Advisories .
Towork around Advisory 1inthedocument above, andasageneral way toupdate tothelatest device
BSL, TIprovides autility todownload thelatest BSL. This utility isavailable inside ofTIResource Explorer
(see Figure 13).Alternatively, theBSL canbeupdated byrunning Program_MSP432_BSL.bat inthe
source files forMSP432P401R BSL update .
Figure 13.BSL Update Utility inTIResource Explorer
