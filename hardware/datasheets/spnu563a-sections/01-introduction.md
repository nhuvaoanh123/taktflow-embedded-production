# Introduction

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 106-111

---


<!-- Page 106 -->

106 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroductionChapter 1
SPNU563A -March 2018
Introduction
Topic ........................................................................................................................... Page
1.1 Designed forSafety Applications ....................................................................... 107
1.2 Family Description ............................................................................................ 108
1.3 Endianism Considerations ................................................................................. 111

<!-- Page 107 -->

www.ti.com Designed forSafety Applications
107 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroduction1.1 Designed forSafety Applications
The TMS570LC43x device architecture hasbeen designed from theground uptosimplify development of
functionally safe systems. The basic architectural concept isknown asasafe island approach. Power,
clock, reset, andbasic processing function areprotected toahigh level ofdiagnostic coverage in
hardware. Some ofthekeyfeatures ofthesafe island region are:
*Lockstep safety concept isalso extended totheVector Interrupt Module (VIM). Dual VIMs inlockstep
thatdetect failures atthecontroller's boundary onacycle bycycle basis. VIMs internal RAM that
stores thevector addresses isalso ECC protected.
*ECC diagnostic forthedatapath ontheLevel 1cache memories aswell asECC ontheLevel 2SRAM
andflash memories oftheR5F core. The ECC controllers arelocated inside theCPU foreach
respective memory interface. This approach hastwokeyadvantages:
-The interconnect between CPU andthememory isalso covered bythediagnostic.
-The ECC logic itself ischecked onacycle bycycle basis.
*Hardware BIST controllers thatprovide anextremely high level ofdiagnostic coverage forthelockstep
CPUs andSRAMs inthesystem, while executing faster andconsuming less memory than equivalent
software-based self-test solutions.
*Hardware BIST diagnostic also forboth theN2HET timer coprocessors.
*Interconnect between themasters andthelevel 2memories contain built-in hardware safety diagnostic
logic thatmonitors theintegrity oftraffics ineach cycle
-Continuous monitoring oftransactions going inandoutoftheinterconnect.
-Parity diagnostic ontheaddress andcontrol paths between allmasters andslaves
-BIST mode fordiagnostic coverage oftheinterconnect.
-ECC generation andevaluation fortransactions onthedatapath generated forsome ofthebus
masters.
*Onboard voltage andreset monitoring logic
*Onboard oscillator andPLL failure detection logic including abackup RCoscillator thatcanbeutilized
upon failure
The TMS570LC43x device architecture also includes many features tosimplify diagnostics ofremaining
logic such as:
*Continuous parity orECC diagnostics onallperipheral memories.
*Analog anddigital loopback totestforshorts onI/O.
*HWself-test anddiagnostics ontheADC module tocheck integrity ofboth analog inputs andtheADC
core conversion function.
*ADMA driven hardware engine forthebackground calculation ofCRC signatures during data
transfers.
*Acentralized error reporting function including astatus output pintoenable external monitoring ofthe
device status.

<!-- Page 108 -->

Family Description www.ti.com
108 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroduction1.2 Family Description
The TMS570LC43x family ofmicrocontrollers arecache-based architecture based ontheARM ®Cortex ®-
R5F Floating Point CPU thatoffers anefficient 1.66 DMIPS/MHz performance andhasconfigurations that
canrunupto330MHz providing upto498DMIPS. The device supports thebig-endian [BE32] format.
The TMS570LC43x hasupto4MB integrated Flash andupto512KB data RAM configurations with single
biterror correction anddouble biterror detection. The flash memory onthisdevice isanonvolatile,
electrically erasable andprogrammable memory implemented with a64-bit-wide data businterface. The
flash operates ona3.3V supply input (same level asI/Osupply) forallread, program anderase
operations. The SRAM operates with asystem clock frequency ofupto150MHz. The SRAM supports
read/write accesses inbyte, halfword, andword modes.
The TMS570LC43x device features peripherals forreal-time control-based applications, including twoNext
Generation High End Timer (N2HET) timing coprocessors with upto64total IOterminals andtwo12-bit A
toDconverters supporting upto41inputs.
The N2HET isanadvanced intelligent timer thatprovides sophisticated timing functions forreal-time
applications. The timer issoftware-controlled, using areduced instruction set,with aspecialized timer
micromachine andanattached I/Oport. The N2HET canbeused forpulse width modulated outputs,
capture orcompare inputs, orgeneral-purpose I/O.Itisespecially well suited forapplications requiring
multiple sensor information anddrive actuators with complex andaccurate time pulses. AHigh End Timer
Transfer Unit (HET-TU) canperform DMA type transactions totransfer N2HET data toorfrom main
memory. AMemory Protection Unit (MPU) isbuilt intotheHET-TU.
The device hastwo12-bit-resolution MibADCs with 41total channels and64words ofparity protected
buffer RAM each. The MibADC channels canbeconverted individually orcanbegrouped bysoftware for
sequential conversion sequences. Sixteen channels areshared between thetwoMibADCs. There are
three separate groupings. Each sequence canbeconverted once when triggered orconfigured for
continuous conversion mode.
There arethree on-die temperature sensors onthisdevice. The temperature measurements ofthethree
temperature sensors arerouted totheMibADC forconversion intodigital values. CPU canread outthe
digital values andcompare with thecalibrated temperature value stored inthedevice's OTP.
The device hasmultiple communication interfaces: Five MibSPIs, twoLINs, twoSCIs, four DCANs, two
I2C,oneEthernet ,andoneFlexRay controller. The MibSPI provides aconvenient method ofserial
interaction forhigh-speed communications between similar shift-register type devices. Data stored inthe
MibSPI's buffer RAM areprotected with ECC. The LINsupports theLocal Interconnect standard 2.0and
canbeused asaUART infull-duplex mode using thestandard Non-Return-to-Zero (NRZ) format. The
DCAN supports theCAN 2.0B protocol standard anduses aserial, multimaster communication protocol
thatefficiently supports distributed real-time control with robust communication rates ofupto1megabit
persecond (Mbps). The DCAN isideal forapplications operating innoisy andharsh environments (for
example, automotive andindustrial fields) thatrequire reliable serial communication ormultiplexed wiring.
Messages stored attheDCAN's RAM areprotected with ECC. The FlexRay uses adual channel serial,
fixed time base multimaster communication protocol with communication rates of10megabits persecond
(Mbps) perchannel. Messages stored attheFlexRay's RAM areECC protected. AFlexRay Transfer Unit
(FTU) enables autonomous transfers ofFlexRay data toandfrom main CPU memory. Transfers are
protected byadedicated, built-in Memory Protection Unit (MPU). The Ethernet module supports MIIand
MDIO interfaces. Transfers areprotected byastandalone Enhanced Memory Protection Unit (NMPU)
The I2Cmodule isamulti-master communication module providing aninterface between the
microcontroller andanI2Ccompatible device viatheI2Cserial bus. The I2Csupports both 100Kbps and
400Kbps speeds.
The frequency-modulated phase-locked loop (FMPLL) clock module isused tomultiply theexternal
frequency reference toahigher frequency forinternal use. The FMPLL provides oneoftheseven possible
clock source inputs totheglobal clock module (GCM). The GCM module manages themapping between
theavailable clock sources andthedevice clock domains.
The device also hastwoexternal clock prescaler (ECP) modules thatwhen enabled, outputs acontinuous
external clock ontheECLK1 andECLK2 terminals. The ECLK frequency isauser-programmable ratio of
theperipheral interface clock (VCLK) frequency. This lowfrequency output canbemonitored externally as
anindicator ofthedevice operating frequency.

<!-- Page 109 -->

www.ti.com Family Description
109 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroductionThe Direct Memory Access Controller (DMA) iscapable ofissuing multi-threaded transactions atthesame
time. Itcansupport upto48DMA requests thatcanbemapped toanyoneofthe32channels. 32control
packets implemented inRAM tostore channel control information areECC protected. Afirstlevel Memory
Protection Unit (MPU) isbuilt intotheDMA andasecond level standalone Enhanced Memory Protection
Unit (NMPU) further protect memory against erroneous transfers.
The Error Signaling Module (ESM) monitors alldevice errors anddetermines whether aninterrupt or
external Error pin/ball istriggered when afault isdetected. The nERROR canbemonitored externally as
anindicator ofafault condition inthemicrocontroller.
The External Memory Interface (EMIF) provides amemory extension toasynchronous andsynchronous
memories orother slave devices.
Several interfaces areimplemented toenhance thedebugging capabilities ofapplication code. Inaddition
tothebuilt inARM ®Cortex ®-R5F CoreSight ™debug features. Embedded Cross Trigger (ECT) supports
theinteraction andsynchronization ofmultiple triggering events within theSoC. AnExternal Trace
Macrocell (ETM) provides instruction anddata trace ofprogram execution. Forinstrumentation purposes,
aRAM Trace Port Module (RTP) isimplemented tosupport high-speed tracing ofRAM andperipheral
accesses bytheCPU oranyother master. AData Modification Module (DMM) gives theability towrite
external data intothedevice memory. Both theRTP andDMM have nooronly minimum impact onthe
program execution time oftheapplication code. AParameter Overlay Module (POM) isincluded to
enhance thecalibration capabilities ofapplication code. The POM canre-route Flash accesses tointernal
memory ortotheEMIF, thus avoiding there-programming steps necessary forparameter updates in
Flash.
With integrated safety features andawide choice ofcommunication andcontrol peripherals, the
TMS570LC43x isanideal solution forhigh performance realtime control applications with safety critical
requirements.

<!-- Page 110 -->

HTU1 FTU HTU2
Peripheral Interconnect Subsystem
CRC
1,2PCR2 PCR3
EMAC SlavesDCAN1
DCAN2
DCAN3
LIN1/
SCI1MibSPI4MDIO
MIIMibSPI1CAN1_RX
CAN1_TX
CAN2_RX
CAN2_TX
CAN3_RX
CAN3_TX
MIBSPI1_CLK
MIBSPI1_SIMO[1:0]
MIBSPI1_SOMI[1:0]
MIBSPI1_nCS[5:0]
MIBSPI1_nENA
MibSPI2MIBSPI2_CLK
MIBSPI2_SIMO
MIBSPI2_SOMI
MIBSPI2_nCS[1:0]
MIBSPI2_nENA
MibSPI3MIBSPI3_CLK
MIBSPI3_SIMO
MIBSPI3_SOMI
MIBSPI3_nCS[5:0]
MIBSPI3_nENA
MIBSPI4_CLK
MIBSPI4_SIMO
MIBSPI4_SOMI
MIBSPI4_nCS[5:0]
MIBSPI4_nENA
MibSPI5MIBSPI5_SIMO[3:0]
MIBSPI5_SOMI[3:0]
MIBSPI5_nCS[5:0]
MIBSPI5_nENA
LIN1_RX
LIN1_TXIOMM
PMM
Lockstep
VIMs
RTI
DCC1DMA
MDCLK
MDIO
MII_RXD[3:0]
MII_RXER
MII_TXD[3:0]
MII_TXEN
MII_TXCLK
MII_RXCLK
MII_CRS
MII_RXDV
MII_COLEMIFEMIF_CLK
EMIF_CKE
EMIF_nCS[4:2]
EMIF_nCS[0]EMIF_ADDR[21:0]
EMIF_BA[1:0]EMIF_DATA[15:0]
EMIF_nDQM[1:0]
EMIF_nOE
EMIF_nWE
EMIF_nRAS
EMIF_nCAS
EMIF_nRWEMIF_nWAIT
SYSnPORRST
nRST
ECLK[2:1]
ESMeQEP
1,2eQEPxA
eQEPxB
eQEPxS
eQEPxI
eCAP
1..6eCAP[6:1]
ePWM
1..7nTZ[3:1]
SYNCO
SYNCI
ePWMxA
ePWMxB
N2HET1 FlexRay GIO N2HET2
FRAY_RX1FRAY_TX1
FRAY_TXEN1FRAY_RX2FRAY_TX2
FRAY_TXEN2GIOB[7:0]GIOA[7:0]
N2HET1[31:0]
N2HET1_PIN_nDIS N2HET2_PIN_nDISMibADC1 MibADC2
VSSADVCCAD
ADREFHIADREFLONMPUEMAC
# 2
# 3
# 4#6#1 always onCore/RAM Core
#5Color Legend for
Power DomainsDCAN4CAN4_RX
CAN4_TXPCR1
LIN2/
SCI2LIN2_RX
LIN2_TX
SCI3SCI3_RX
SCI3_TX
SCI4SCI4_RX
SCI4_TX
I2C1I2C1_SDA
I2C1_SCL
I2C2I2C2_SDA
I2C2_SCLEMIF
SlaveCPU Interconnect SubsystemDual Cortex -R5F
CPUs in lockstep32KB Icache
& Dcache w /
ECC
POM
4MB Flash
&
128KB
Flash for
EEPROM
Emulation
w/ ECC512KB
SRAM
w/
ECCNMPU
NMPU
STC1EPC
SCM
SYS
DCC2
STC2DMM DAP
CCM-
R5FRTPnTRST
TMS
TCK
RTCKTDI
TDODMMnENA
DMMSYNC
DMMCLK
DMMDATA[15:0]TPIURTPnENA
RTPSYNCRTPCLK
RTPDATA[15:0]ETMDATA[31:0]]
ETMTRACECTL
ETMTRACECLK
ETMTRACECLKIN
MIBSPI5_CLK
N2HET2[31:0]
AD1IN[15:8]/AD2IN[15:8]AD2EVT
AD1IN[23:16]/AD2IN[7:0]AD1IN[7:0]AD1EVT
AD1EXT_SEL[4:0]AD1EXT_ENA_
AD2IN[24:16] AD1IN[31:24]nERRORuSCU
Copyright © 2016, Texas Instruments Incorporated
Family Description www.ti.com
110 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroductionFigure 1-1.Block Diagram

<!-- Page 111 -->

www.ti.com Endianism Considerations
111 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedIntroduction1.3 Endianism Considerations
1.3.1 TMS570: BigEndian (BE32)
The TMS570LC43x family isbased ontheARM ®Cortex ®-R5F core. ARM hasdesigned thiscore tobe
used inbig-endian andlittle-endian systems. FortheTITMS570LC43x family, theendianness hasbeen
configured toBE32. Big-endian systems store themost-significant byte ofamulti-byte data field inthe
lowest memory address. Also, theaddress ofthemulti-byte data field isthelowest address. Following is
anexample ofthephysical addresses ofindividual bytes.
Figure 1-2.Example: SPIDELAY -0xFFF7F448
31 24 23 16
C2TDELAY[7:0] T2CDELAY[7:0]
Byte 3-0xFFF7F448 Byte 2-0xFFF7F449
15 8 7 0
T2EDELAY[7:0] C2EDELAY[7:0]
Byte 1-0xFFF7F44A Byte 0-0xFFF7F44B
32-bit accesses tothisregister should usethelowest address, thatis,0xFFF7F448. Writing 0x11223344
toaddress 0xFFF7F448 shows thefollowing when viewing thememory in8-bit and32-bit modes.
Assuch theheaders provided aspart ofHALCoGen dotake theendianness intoaccount andprovide
header structures thatareagnostic toendianness. This isachieved byusing Cdirectives forthecompiler
thatmake useofthecompile options configured fortheproject bytheuser (__little_endian__ used in
Code Composer Studio codegen tools). This directive may need tobeadapted forother compilers.
#ifdef __little_endian__
char C2EDELAY :8U; /**lt; 0xF448: CStoENA */
char T2EDELAY :8U; /**lt; 0xF449: Transmit toENA */
char T2CDELAY :8U; /**lt; 0xF44A: Transmit toCS */
char C2TDELAY :8U; /**lt; 0xF44B: CStoTransmit */
#else
char C2TDELAY :8U; /**lt; 0xF448: CStoTransmit */
char T2CDELAY :8U; /**lt; 0xF449: Transmit toCS */
char T2EDELAY :8U; /**lt; 0xF44A: Transmit toENA */
char C2EDELAY :8U; /**lt; 0xF44B: CStoENA */