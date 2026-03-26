# Device Overview

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 1-8 (8 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 1 -->
Product
Folder
Sample &
Buy
Technical
Documents
Tools &
Software
Support &
Community
AnIMPORTANT NOTICE attheend ofthisdata sheet addresses availability, warranty, changes, use insafety-critical applications,
intellectual property matters andother important disclaimers. PRODUCTION DATA.TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
TMS570LC4357 Hercules ™Microcontroller Based ontheARM®Cortex ®-RCore
1Device Overview
11.1 Features
1
*High-Performance Automotive-Grade
Microcontroller forSafety-Critical Applications
-Dual-Core Lockstep CPUs With ECC-Protected
Caches
-ECC onFlash andRAM Interfaces
-Built-In Self-Test (BIST) forCPU, High-End
Timers, andOn-Chip RAMs
-Error Signaling Module (ESM) With Error Pin
-Voltage andClock Monitoring
*ARM®Cortex®-R5F 32-Bit RISC CPU
-1.66 DMIPS/MHz With 8-Stage Pipeline
-FPU With Single- andDouble-Precision
-16-Region Memory Protection Unit (MPU)
-32KB ofInstruction and32KB ofData Caches
With ECC
-Open Architecture With Third-Party Support
*Operating Conditions
-Upto300-MHz CPU Clock
-Core Supply Voltage (VCC): 1.14 to1.32 V
-I/OSupply Voltage (VCCIO): 3.0to3.6V
*Integrated Memory
-4MB ofProgram Flash With ECC
-512KB ofRAM With ECC
-128KB ofData Flash forEmulated EEPROM
With ECC
*16-Bit External Memory Interface (EMIF)
*Hercules ™Common Platform Architecture
-Consistent Memory Map Across Family
-Real-Time Interrupt (RTI) Timer (OS Timer)
-Two 128-Channel Vectored Interrupt Modules
(VIMs) With ECC Protection onVector Table
*VIM1 andVIM2 inSafety Lockstep Mode
-Two 2-Channel Cyclic Redundancy Checker
(CRC) Modules
*Direct Memory Access (DMA) Controller
-32Channels and48Peripheral Requests
-ECC Protection forControl Packet RAM
-DMA Accesses Protected byDedicated MPU
*Frequency-Modulated Phase-Locked Loop
(FMPLL) With Built-In SlipDetector
*Separate Nonmodulating PLL
*IEEE 1149.1 JTAG, Boundary Scan, andARM
CoreSight ™Components
*Advanced JTAG Security Module (AJSM)
*Trace andCalibration Capabilities
-ETM ™,RTP, DMM ,POM*Multiple Communication Interfaces
-10/100 Mbps Ethernet MAC (EMAC)
*IEEE 802.3 Compliant (3.3-V I/OOnly)
*Supports MII,RMII, andMDIO
-FlexRay Controller With 2Channels
*8KB ofMessage RAM With ECC Protection
*Dedicated FlexRay Transfer Unit (FTU)
-Four CAN Controller (DCAN) Modules
*64Mailboxes, Each With ECC Protection
*Compliant toCAN Protocol Version 2.0B
-Two Inter-Integrated Circuit (I2C)Modules
-Five Multibuffered Serial Peripheral Interface
(MibSPI) Modules
*MibSPI1: 256Words With ECC Protection
*Other MibSPIs: 128Words With ECC
Protection
-Four UART (SCI) Interfaces, Two With Local
Interconnect Network (LIN 2.1) Interface
Support
*Two Next Generation High-End Timer (N2HET)
Modules
-32Programmable Channels Each
-256-Word Instruction RAM With Parity
-Hardware Angle Generator forEach N2HET
-Dedicated High-End Timer Transfer Unit (HTU)
forEach N2HET
*Two 12-Bit Multibuffered Analog-to-Digital
Converter (MibADC) Modules
-MibADC1: 32Channels Plus Control forupto
1024 Off-Chip Channels
-MibADC2: 25Channels
-16Shared Channels
-64Result Buffers Each With Parity Protection
*Enhanced Timing Peripherals
-7Enhanced Pulse Width Modulator (ePWM)
Modules
-6Enhanced Capture (eCAP) Modules
-2Enhanced Quadrature Encoder Pulse (eQEP)
Modules
*Three On-Die Temperature Sensors
*Upto145Pins Available forGeneral-Purpose I/O
(GPIO)
*16Dedicated GPIO Pins With External Interrupt
Capability
*Packages
-337-Ball Grid Array (ZWT) [Green]

<!-- Page 2 -->
2TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Overview Copyright ©2014 -2016, Texas Instruments Incorporated1.2 Applications
*Braking Systems (Antilock Brake Systems and
Electronic Stability Control)
*Electric Power Steering (EPS)
*HEV andEVInverter Systems
*Battery-Management Systems*Active Driver Assistance Systems
*Aerospace andAvionics
*Railway Communications
*Off-road Vehicles

<!-- Page 3 -->
3TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Overview Copyright ©2014 -2016, Texas Instruments Incorporated1.3 Description
The TMS570LC4357 device ispart oftheHercules TMS570 series ofhigh-performance automotive-grade
ARM ®Cortex ®-R-based MCUs. Comprehensive documentation, tools, and software areavailable to
assist inthedevelopment ofISO 26262 and IEC 61508 functional safety applications. Start evaluating
today with theHercules TMS570LC43x LaunchPad Development Kit.The TMS570LC4357 device hason-
chip diagnostic features including: dual CPUs inlockstep, Built-In Self-Test (BIST) logic forCPU, the
N2HET coprocessors, and foron-chip SRAMs; ECC protection ontheL1caches, L2flash, and SRAM
memories. The device also supports ECC orparity protection onperipheral memories and loopback
capability onperipheral I/Os.
The TMS570LC4357 device integrates twoARM Cortex-R5F floating-point CPUs, operating inlockstep,
which offer anefficient 1.66 DMIPS/MHz, and can runupto300 MHz providing upto498 DMIPS. The
device supports thebig-endian [BE32] format.
The TMS570LC4357 device has 4MB ofintegrated flash and 512KB ofdata RAM with single-bit error
correction and double-bit error detection. The flash memory onthis device isanonvolatile, electrically
erasable and programmable memory, implemented with a64-bit-wide data bus interface. The flash
operates ona3.3-V supply input (the same level astheI/Osupply) forallread, program, and erase
operations. The SRAM supports read andwrite accesses inbyte, halfword, andword modes.
The TMS570LC4357 device features peripherals forreal-time control-based applications, including two
Next Generation High-End Timer (N2HET) timing coprocessors with upto64total I/Oterminals.
The N2HET isanadvanced intelligent timer that provides sophisticated timing functions forreal-time
applications. The timer issoftware-controlled, with aspecialized timer micromachine and anattached I/O
port. The N2HET can beused forpulse-width-modulated outputs, capture orcompare inputs, orGPIO.
The N2HET isespecially well suited forapplications requiring multiple sensor information ordrive
actuators with complex and accurate time pulses. The High-End Timer Transfer Unit (HTU) canperform
DMA-type transactions totransfer N2HET data toorfrom main memory. AMemory Protection Unit (MPU)
isbuilt intotheHTU.
The Enhanced Pulse Width Modulator (ePWM) module cangenerate complex pulse width waveforms with
minimal CPU overhead orintervention. The ePWM iseasy touseand supports both high-side and low-
side PWM anddeadband generation. With integrated tripzone protection andsynchronization with theon-
chip MibADC, theePWM isideal fordigital motor control applications.
The Enhanced Capture (eCAP) module isessential insystems where theaccurately timed capture of
external events isimportant. The eCAP canalso beused tomonitor theePWM outputs orforsimple PWM
generation when notneeded forcapture applications.
The Enhanced Quadrature Encoder Pulse (eQEP) module directly interfaces with alinear orrotary
incremental encoder togetposition, direction, and speed information from arotating machine asused in
high-performance motion andposition-control systems.
The device has two12-bit-resolution MibADCs with 41total channels and 64words ofparity-protected
buffer RAM. The MibADC channels can beconverted individually orbygroup forspecial conversion
sequences. Sixteen channels are shared between thetwo MibADCs. Each MibADC supports three
separate groupings. Each sequence canbeconverted once when triggered orconfigured forcontinuous
conversion mode. The MibADC hasa10-bit mode forusewhen compatibility with older devices orfaster
conversion time isdesired. One ofthechannels inMibADC1 andtwoofthechannels inMibADC2 canbe
used toconvert temperature measurements from thethree on-chip temperature sensors.
The device hasmultiple communication interfaces: Five MibSPIs; four UART (SCI) interfaces, twowith LIN
support; four CANs; two I2C modules;one Ethernet Controller; and one FlexRay controller. The SPI
provides aconvenient method ofserial interaction forhigh-speed communications between similar shift-
register type devices. The LINsupports theLocal Interconnect standard (LIN 2.1) and canbeused asa
UART infull-duplex mode using thestandard Non-Return-to-Zero (NRZ) format. The DCAN supports the
CAN 2.0B protocol standard and uses aserial, multimaster communication protocol that efficiently
supports distributed real-time control with robust communication rates ofupto1Mbps. The DCAN isideal

<!-- Page 4 -->
4TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Overview Copyright ©2014 -2016, Texas Instruments Incorporatedforapplications operating innoisy and harsh environments (forexample, automotive and industrial fields)
that require reliable serial communication ormultiplexed wiring. The FlexRay controller uses adual-
channel serial, fixed time base multimaster communication protocol with communication rates of10Mbps
perchannel. AFlexRay Transfer Unit (FTU) enables autonomous transfers ofFlexRay data toand from
main CPU memory. HTU transfers areprotected byadedicated, built-in MPU. The Ethernet module
supports MII, RMII, and Management Data I/O(MDIO) interfaces. The I2C module isamultimaster
communication module providing aninterface between themicrocontroller and anI2C-compatible device
through theI2Cserial bus. The I2Cmodule supports speeds of100and400kbps.
The Frequency-Modulated Phase-Locked Loop (FMPLL) clock module multiplies theexternal frequency
reference toahigher frequency forinternal use. The Global Clock Module (GCM) manages themapping
between theavailable clock sources andtheinternal device clock domains.
The device also has twoExternal Clock Prescaler (ECP) modules. When enabled, theECPs output a
continuous external clock ontheECLK1 and ECLK2 balls. The ECLK frequency isauser-programmable
ratio oftheperipheral interface clock (VCLK) frequency. This low-frequency output can bemonitored
externally asanindicator ofthedevice operating frequency.
The Direct Memory Access (DMA) controller has32channels, 48peripheral requests, andECC protection
onitsmemory. AnMPU isbuilt intotheDMA toprotect memory against erroneous transfers.
The Error Signaling Module (ESM) monitors on-chip device errors anddetermines whether aninterrupt or
external Error pin/ball (nERROR) istriggered when afault isdetected. The nERROR signal can be
monitored externally asanindicator ofafault condition inthemicrocontroller.
The External Memory Interface (EMIF) provides amemory extension toasynchronous and synchronous
memories orother slave devices.
AParameter Overlay Module (POM) isincluded toenhance thedebugging capabilities ofapplication code.
The POM can reroute flash accesses tointernal RAM ortotheEMIF, thus avoiding thereprogramming
steps necessary forparameter updates inflash. This capability isparticularly helpful during real-time
system calibration cycles.
Several interfaces areimplemented toenhance thedebugging capabilities ofapplication code. Inaddition
tothebuilt-in ARM Cortex-R5F CoreSight debug features, theEmbedded Cross Trigger (ECT) supports
theinteraction and synchronization ofmultiple triggering events within theSoC. AnExternal Trace
Macrocell (ETM) provides instruction and data trace ofprogram execution. Forinstrumentation purposes,
aRAM Trace Port (RTP) module isimplemented tosupport high-speed tracing ofRAM and peripheral
accesses bytheCPU orany other master. AData Modification Module (DMM) gives theability towrite
external data intothedevice memory. Both theRTP andDMM have noorminimal impact ontheprogram
execution time oftheapplication code.
With integrated safety features and awide choice ofcommunication and control peripherals, the
TMS570LC4357 device isanideal solution forhigh-performance real-time control applications with safety-
critical requirements.
(1) Formore information onthese devices, seeSection 10,Mechanical Packaging andOrderable
Information .Device Information(1)
PART NUMBER PACKAGE BODY SIZE
TMS570LC4357ZWT NFBGA (337) 16.00 mm×16.00 mm

<!-- Page 5 -->
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
5TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Overview Copyright ©2014 -2016, Texas Instruments Incorporated1.4 Functional Block Diagram
Figure 1-1shows thefunctional block diagram ofthedevice.
Figure 1-1.Functional Block Diagram

<!-- Page 6 -->
6TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Table ofContents Copyright ©2014 -2016, Texas Instruments IncorporatedTable ofContents
1Device Overview ......................................... 1
1.1 Features .............................................. 1
1.2 Applications ........................................... 2
1.3 Description ............................................ 3
1.4 Functional Block Diagram ............................ 5
2Revision History ......................................... 7
3Device Comparison ..................................... 8
4Terminal Configuration andFunctions .............. 9
4.1 ZWT BGA Package Ball-Map (337 Terminal Grid
Array)................................................. 9
4.2 Terminal Functions .................................. 10
5Specifications .......................................... 55
5.1 Absolute Maximum Ratings ......................... 55
5.2 ESD Ratings ........................................ 55
5.3 Power-On Hours (POH)............................. 55
5.4 Device Recommended Operating Conditions ....... 56
5.5 Switching Characteristics over Recommended
Operating Conditions forClock Domains ........... 57
5.6 Wait States Required -L2Memories ............... 57
5.7 Power Consumption Summary ...................... 59
5.8 Input/Output Electrical Characteristics Over
Recommended Operating Conditions ............... 60
5.9 Thermal Resistance Characteristics fortheBGA
Package (ZWT) ..................................... 61
5.10 Timing andSwitching Characteristics ............... 61
6System Information andElectrical
Specifications ........................................... 64
6.1 Device Power Domains ............................. 64
6.2 Voltage Monitor Characteristics ..................... 65
6.3 Power Sequencing andPower-On Reset........... 66
6.4 Warm Reset (nRST) ................................. 68
6.5 ARM Cortex-R5F CPU Information ................. 69
6.6 Clocks ............................................... 76
6.7 Clock Monitoring .................................... 87
6.8 Glitch Filters......................................... 89
6.9 Device Memory Map................................ 90
6.10 Flash Memory ...................................... 101
6.11 L2RAMW (Level 2RAM Interface Module) ........ 104
6.12 ECC /Parity Protection forAccesses toPeripheral
RAMs .............................................. 1046.13 On-Chip SRAM Initialization andTesting .......... 105
6.14 External Memory Interface (EMIF) ................. 109
6.15 Vectored Interrupt Manager ........................ 117
6.16 ECC Error Event Monitoring andProfiling ......... 121
6.17 DMA Controller ..................................... 123
6.18 Real-Time Interrupt Module ........................ 127
6.19 Error Signaling Module ............................. 129
6.20 Reset /Abort /Error Sources ...................... 134
6.21 Digital Windowed Watchdog ....................... 138
6.22 Debug Subsystem ................................. 139
7Peripheral Information andElectrical
Specifications ......................................... 156
7.1 Enhanced Translator PWM Modules (ePWM) ..... 156
7.2 Enhanced Capture Modules (eCAP) ............... 161
7.3 Enhanced Quadrature Encoder (eQEP) ........... 164
7.4 12-bit Multibuffered Analog-to-Digital Converter
(MibADC) ........................................... 166
7.5 General-Purpose Input/Output ..................... 179
7.6 Enhanced High-End Timer (N2HET) .............. 180
7.7 FlexRay Interface .................................. 185
7.8 Controller Area Network (DCAN) .................. 187
7.9 Local Interconnect Network Interface (LIN)........ 188
7.10 Serial Communication Interface (SCI)............. 189
7.11 Inter-Integrated Circuit (I2C)....................... 190
7.12 Multibuffered /Standard Serial Peripheral
Interface ............................................ 193
7.13 Ethernet Media Access Controller ................. 207
8Applications, Implementation, andLayout ...... 211
8.1 TIDesign orReference Design .................... 211
9Device andDocumentation Support .............. 212
9.1 Device Support ..................................... 212
9.2 Documentation Support ............................ 214
9.3 Trademarks ........................................ 214
9.4 Electrostatic Discharge Caution ................... 214
9.5 Glossary ............................................ 214
9.6 Device Identification ................................ 215
9.7 Module Certifications ............................... 217
10Mechanical Data...................................... 223
10.1 Packaging Information ............................. 223

<!-- Page 7 -->
7TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Revision History Copyright ©2014 -2016, Texas Instruments Incorporated2Revision History
This data manual revision history highlights thetechnical changes made totheSPNS195B device-specific
data manual tomake itanSPNS195C revision. These devices arenow intheProduction Data (PD) stage
ofdevelopment.
Changes from January 31,2016 toJune 25,2016 (from BRevision (January 2016) toCRevision) Page
*Global: Updated/Changed theproduct status from Product Preview toProduction Data................................... 1
*Table 6-13 (LPO Specifications): Updated/Changed LPO -HFoscillator, Untrimmed frequency TYP value from
"9.6"to"9"MHz..................................................................................................................... 78
*Section 6.9.3 (Special Consideration forCPU Access Errors Resulting inImprecise Aborts): Add missing
subsection ............................................................................................................................ 96
*Section 6.14.1 (External Memory Interface (EMIF), Features): Updated/Changed theEMIF asynchronous
memory maximum addressable size from "32KB "to"16MB "each......................................................... 109
*Section 6.14 (External Memory Interface (EMIF)): Added 32-bit access note using a16-bit EMIF interface. ........ 109
*Added "Commonly caused by..."statement forclarification ................................................................. 131
*Table 6-56 (ETMTRACECLK Timing): Restructured timing table formatting tostandards ............................... 149
*Table 7-7,(eCAPx Clock Enable Control): Updated/Changed "ePWM "to"eCAP "intheMODULE INSTANCE
column ............................................................................................................................... 162
*Table 7-11,(eQEPx Clock Enable Control): Updated/Changed "ePWM "to"eQEP "intheMODULE INSTANCE
column ............................................................................................................................... 164
*Table 7-16 (MibADC1 Event Trigger Selection): Added lead-in paragraph referencing theable....................... 166
*(MibADC1 Event Trigger Hookup): NOTE: Added new paragraph ......................................................... 168
*Table 7-17 (MibADC2 Event Trigger Selection): Added lead-in paragraph referencing theable....................... 168
*Section 7.4.2.3 (Controlling ADC1 andADC2 Event Trigger Options Using SOC Output from ePWM Modules):
Updated/Changed thenames ofthefour ePWM signals thatevent trigger theADC.................................... 170
*Table 7-22 (MibADC Operating Characteristics Over 3.0Vto3.6VOperating Conditions): Updated/Changed
the10-and12-bit mode formulas tobesuperscript power of2values .................................................... 175
*Table 7-23 (MibADC Operating Characteristics Over 3.6Vto5.25 VOperating Conditions): Updated/Changed
the10-and12-bit mode formulas tobesuperscript power of2values .................................................... 175

<!-- Page 8 -->
8TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Comparison Copyright ©2014 -2016, Texas Instruments Incorporated(1) Foradditional device variants, seewww.ti.com/tms570 .
(2) This table reflects themaximum configuration foreach peripheral. Some functions aremultiplexed andnotallpins areavailable atthe
same time.
(3) Superset device
(4) Total number ofpins thatcanbeused asgeneral-purpose input oroutput when notused aspart ofaperipheral.3Device Comparison
Table 3-1lists thefeatures oftheTMS570LC4357 devices.
Table 3-1.TMS570LC4357 Device Comparison(1)(2)
FEATURES DEVICES
Generic Part Number TMS570LC4357ZWT(3)TMS570LS3137ZWT(3)TMS570LS3135ZWT TMS570LS1227ZWT(3)
Package 337BGA 337BGA 337BGA 337BGA
CPU ARM Cortex-R5F ARM Cortex-R4F ARM Cortex-R4F ARM Cortex-R4F
Frequency (MHz) 300 180 180 180
Cache (KB)32I
32D- - -
Flash (KB) 4096 3072 3072 1280
RAM (KB) 512 256 256 192
Data Flash [EEPROM] (KB) 128 64 64 64
EMAC 10/100 10/100 - 10/100
FlexRay 2-ch 2-ch 2-ch 2-ch
CAN 4 3 3 3
MibADC
12-bit (Ch)2(41ch) 2(24ch) 2(24ch) 2(24ch)
N2HET (Ch) 2(64) 2(44) 2(44) 2(44)
ePWM Channels 14 - - 14
eCAP Channels 6 - - 6
eQEP Channels 2 - - 2
MibSPI (CS) 5(4x6+2) 3(6+6+4) 3(6+6+4) 3(6+6+4)
SPI(CS) - 2(2+1) 2(2+1) 2(2+1)
SCI(LIN) 4(2with LIN) 2(1with LIN) 2(1with LIN) 2(1with LIN)
I2C 2 1 1 1
GPIO (INT)(4)168(with 16interrupt capable) 144(with 16interrupt capable) 144(with 16interrupt capable) 101(with 16interrupt capable)
EMIF 16-bit data 16-bit data 16-bit data 16-bit data
ETM (Trace) 32-bit 32-bit 32-bit -
RTP/DMM 16/16 16/16 16/16 -
Operating
Temperature-40ºCto125ºC -40ºCto125ºC -40ºCto125ºC -40ºCto125ºC
Core Supply (V) 1.14 V-1.32 V 1.14 V-1.32 V 1.14 V-1.32 V 1.14 V-1.32 V
I/OSupply (V) 3.0V-3.6V 3.0V-3.6V 3.0V-3.6V 3.0V-3.6V