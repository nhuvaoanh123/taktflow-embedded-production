# Debug Subsystem, JTAG

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 139-155 (17 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 139 -->
TRST
TMS
TCK
TDI
TDORTCK
ICEPICK_CBoundary Scan
BSR/BSDLBoundary Scan
Interface
Debug Tap 0DAPDebug APB
APB Mux
AHB-AP
POM
Test Tap 0
eFuse FarmDebug Tap 2
AJSM
Test Tap 1
PSCONR5F
CPUR5F
ETMDebug
ROM
CTI1 CTI3 CTI4
CTM1 CTM2
CSTF TPIUPS_SCR
OCP2_
BVUSPVBUSP2
APBv3
PCR3
Debug Tap 1RTP
DMM
139TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22 Debug Subsystem
6.22.1 Block Diagram
The device contains anICEPICK module (version C)toallow JTAG access tothescan chains (see
Figure 6-23).
Figure 6-23. Debug Subsystem Block Diagram

<!-- Page 140 -->
CTI1
ReservedCTI3
CTI4CTM1 CTM2ch0
ch1ch2
ch3ch0
ch1ch2
ch3tieoff
tieoff
140TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.2 Debug Components Memory Map
Table 6-47. Debug Components Memory Map
MODULE
NAMEFRAME CHIP
SELECTFRAME ADDRESS RANGEFRAME
SIZEACTUAL
SIZERESPONSE FOR ACCESS
TOUNIMPLEMENTED LOCATIONS
INFRAME START END
CoreSight Debug ROM CSCS0 0xFFA0_0000 0xFFA0_0FFF 4KB 4KB Reads return zeros, writes have noeffect
Cortex-R5F Debug CSCS1 0xFFA0_1000 0xFFA0_1FFF 4KB 4KB Reads return zeros, writes have noeffect
ETM-R5 CSCS2 0xFFA0_2000 0xFFA0_2FFF 4KB 4KB Reads return zeros, writes have noeffect
CoreSight TPIU CSCS3 0xFFA0_3000 0xFFA0_3FFF 4KB 4KB Reads return zeros, writes have noeffect
POM CSCS4 0xFFA0_4000 0xFFA0_4FFF 4KB 4KB Reads return zeros, writes have noeffect
CTI1 CSCS7 0xFFA0_7000 0xFFA0_7FFF 4KB 4KB Reads return zeros, writes have noeffect
CTI3 CSCS9 0xFFA0_9000 0xFFA0_9FFF 4KB 4KB Reads return zeros, writes have noeffect
CTI4 CSCS10 0xFFA0_A000 0xFFA0_AFFF 4KB 4KB Reads return zeros, writes have noeffect
CSTF CSCS11 0xFFA0_B000 0xFFA0_BFFF 4KB 4KB Reads return zeros, writes have noeffect
6.22.3 Embedded Cross Trigger
The Embedded Cross Trigger (ECT) isamodular component that supports the interaction and
synchronization ofmultiple triggering events within aSoC.
The ECT consists oftwomodules:
*A(Cross Trigger Interface) CTI. The CTIprovides theinterface between acomponent orsubsystem andtheCross
Trigger Matrix (CTM).
*ACTM. The CTM combines thetrigger requests generated from CTIs andbroadcasts them toallCTIs aschannel
triggers. This enables subsystems tointeract, cross trigger, with oneanother.
Figure 6-24. CTI/CTM Integration

<!-- Page 141 -->
ETM-R5
EXTOUT[1:0]
TRIGSBYPASS
TRIGGER
TRIGGERACK
EXTIN[1:0]
ETMDBGRQ
EDBGRQ
DBTRIGGER
DBGRESTART
DBGRESTARTED
COMMRX
COMMTX
nPMUIRQ
nIRQR5F
100CTI1
CTITRIGIN[3:2]
CTITRIGIN[6]
CTITRIGINACK[6]
CTITRIGOUT[2:1]TIHSBYPASS[2:1]
CTITRIGOUTACK[2:1]1
0
CTITRIOUT[0]
CTITRIGIN[0]
TIHSBYPASS[7]CTITRIGOUT[7]
CTITRIGOUTACK[7]
CTITRIGIN[4]
CTITRIGIN[5]
CTITRIGIN[1]
CTITRIGOUT]3]TIHSBYPASS[3]
CTITRIGOUTACK[3]0
0
nIRQ from VIMCTITRIGOUTACK[0]TIHSBYPASS[0] 0
00
CTITRIGINACK[0]
GCLK1GCLK1CTITRIGIN[7]
141TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-25. CTI1 Mapping

<!-- Page 142 -->
142TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
ETM-R5, Cortex-R5F andCTI1 runatsame frequency.
Table 6-48. CTI1 Mapping
CTITRIGGER Module Signal
Trigger Input 0 From Cortex-R5F DBTRIGGER
Trigger Input 1 From Cortex-R5F nPMUIRQ
Trigger Input 2 From ETM-R5 EXTOUT[0]
Trigger Input 3 From ETM-R5 EXTOUT[1]
Trigger Input 4 From Cortex-R5F COMMRX
Trigger Input 5 From Cortex-R5F COMMTX
Trigger Input 6 From ETM-R5 TRIGGER
Trigger Input 7 From Cortex-R5F DBTRIGGER
Trigger Output 0 ToCortex-R5F EDBGRQ
Trigger Output 1 ToETM-R5 EXTIN[0]
Trigger Output 2 ToETM-R5 EXTIN[1]
Trigger Output 3 ToCortex-R5F nIRQ
Trigger Output 4 Reserved
Trigger Output 5 Reserved
Trigger Output 6 Reserved
Trigger Output 7 ToCortex-R5F DBGRESTARTED

<!-- Page 143 -->
CTI3
TIHSBYPASS[7:2]
CTITRIGOUTACK[7:2]1
0CTITRIGOUT[1] FLUSHIN
CTITRIGOUT[0] TRIGINTPIU
CTITRIGOUTACK[1] FLUSHINACK
CTITRIGOUTACK[0] TRIGINACK
143TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-26. CTI3 Mapping
NOTE
TPIU andCTI3 runatdifferent frequencies.
Table 6-49. CTI3 Mapping
CTITRIGGER Module Signal
Trigger Input 0 Reserved
Trigger Input 1 Reserved
Trigger Input 2 Reserved
Trigger Input 3 Reserved
Trigger Input 4 Reserved
Trigger Input 5 Reserved
Trigger Input 6 Reserved
Trigger Input 7 Reserved
Trigger Output 0 ToTPIU TRIGIN
Trigger Output 1 ToTPIU FLUSHIN
Trigger Output 2 Reserved
Trigger Output 3 Reserved
Trigger Output 4 Reserved
Trigger Output 5 Reserved
Trigger Output 6 Reserved
Trigger Output 7 Reserved

<!-- Page 144 -->
CTI4
CTITRIGIN[1]
CTITRIGOUT[1]CTITRIGOUT[0]
CTITRIGOUTACK[7:4] 0CTITRIGIN[0]
CTITRIGIN[3]CTITRIGIN[2]
CTITRIGIN[4]DMA_DBGREQ
NHET1_DBGREQ
NHET2_DBGREQ
HTU1_DBGREQ
HTU2_DBGREQ
CTITRIGOUT[2]SYS_MODULE_TRIGGER
USER_PERIPHERAL_TRIGGER1
USER_PERIPHERAL_TRIGGER2Sync_Input
Sync_Input
Sync_InputCTITRIGOUTACK[1]CTITRIGOUTACK[0]
CTITRIGOUTACK[2]
CTITRIGOUT[3] USER_PERIPHERAL_TRIGGER3Sync_InputCTITRIGOUTACK[3]Sync_Output Pulse□Creator
Sync_Output Pulse□Creator
Sync_Output Pulse□Creator
Sync_Output Pulse□Creator
Sync_Output Pulse□CreatorCTITRIGINACK[0]
CTITRIGINACK[1]
CTITRIGINACK[2]
CTITRIGINACK[3]
CTITRIGINACK[4]
CTITRIGIN[5]
CTITRIGIN[6]
CTITRIGIN[7]
CTITRIGOUT[4] IcePick□Debug_AttentionSync_InputCTITRIGOUTACK[4]
144TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-27. CTI4 Mapping

<!-- Page 145 -->
145TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-50. CTI4 Mapping
CTITRIGGER Module Signal
Trigger Input 0 From DMA_DBGREQ
Trigger Input 1 From N2HET1_DBGREQ
Trigger Input 2 From N2HET2_DBGREQ
Trigger Input 3 From HTU1_DBGREQ
Trigger Input 4 From HTU2_DBGREQ
Trigger Input 5 From DMA_DBGREQ
Trigger Input 6 From N2HET1_DBGREQ orHTU1_DBGREQ
Trigger Input 7 From N2HET2_DBGREQ orHTU2_DBGREQ
Trigger Output 0 ToSYS_MODULE_TRIGGER
Trigger Output 1 ToUSER_PERIPHERAL_TRIGGER1
Trigger Output 2 ToUSER_PERIPHERAL_TRIGGER2
Trigger Output 3 ToUSER_PERIPHERAL_TRIGGER3
Trigger Output 4 ToIcePick Debug_Attention
Trigger Output 5 Reserved
Trigger Output 6 Reserved
Trigger Output 7 Reserved
Table 6-51. Peripheral Suspend Generation
TRIGGER OUTPUT MODULE SIGNAL CONNECTED DESCRIPTION
SYS_MODULE_TRIGGERL2FMC_CPU_EMUSUSP L2FMC Wrapper Suspend
CCM_R5_CPU_EMUSUSP CCM_R5 module suspend
CRC_CPU_EMUSUSP CRC1 /CRC2 module suspend
SYS_CPU_EMUSUSP SYS module Suspend
USER_PERIPHERAL_TRIGGER1 DMA_SUSPEND DMA Suspend
RTI_CPU_SUSPEND RTI1 /RTI2 Suspend
AWM_CPU_SUSPEND AWM1 /AWM2 Suspend
HTU_CPU_EMUSUSP HTU1 /HTU2 Suspend
SCI_CPU_EMUSUSP SCI3 /SCI4 Suspend
LIN_CPU_EMUSUSP LIN1 /LIN2 Suspend
I2C_CPU_EMUSUSP I2C1 /I2C2 Suspend
EMAC_CPU_EMUSUSP EMAC Suspend
EQEP_CPU_EMUSUSP EQEP Suspend
ECAP_CPU_EMUSUSP ECAP Suspend
DMM_CPU_EMUSUSP DMM Suspend
DCC_CPU_EMUSUSP DCC1 /DCC2 Suspend
USER_PERIPHERAL_TRIGGER2 DCAN_CPU_EMUSUSPDCAN1 /DCAN2 /DCAN3 /DCAN4
Suspend
USER_PERIPHERAL_TRIGGER3 ePWM_CPU_EMUSUSPePWM1..7 Trip Zone TZ6n andePWM1..7
Suspend

<!-- Page 146 -->
146TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.4 JTAG Identification Code
The JTAG IDcode forthisdevice isthesame asthedevice ICEPick Identification Code. FortheJTAG ID
Code persilicon revision, seeTable 6-52.
Table 6-52. JTAG IDCode
SILICON REVISION ID
Rev A 0x0B95A02F
Rev B 0x1B95A02F
6.22.5 Debug ROM
The Debug ROM stores thelocation ofthecomponents ontheDebug APB bus(see Table 6-53).
Table 6-53. Debug ROM Table
ADDRESS DESCRIPTION VALUE
0x000 Cortex-R5F 0x00001003
0x004 ETM-R5 0x00002003
0x008 TPIU 0x00003003
0x00C POM 0x00004003
0x018 CTI1 0x00007003
0x020 CTI3 0x00009003
0x024 CTI4 0x0000A003
0x028 CSTF 0x0000B003
0x02C endoftable 0x00000000

<!-- Page 147 -->
1 1
2
3
4
5TMS
TDI
TDORTCKTCK
147TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Timings forTDO arespecified foramaximum of50-pF load onTDO.6.22.6 JTAG Scan Interface Timings
Table 6-54. JTAG Scan Interface Timing(1)
NO. PARAMETER MIN MAX UNIT
fTCK TCK frequency (atHCLKmax) 12 MHz
fRTCK RTCK frequency (atTCKmax andHCLKmax) 10 MHz
1 td(TCK -RTCK) Delay time, TCK toRTCK 24 ns
2 tsu(TDI/TMS -RTCKr) Setup time, TDI, TMS before RTCK rise(RTCKr) 26 ns
3 th(RTCKr -TDI/TMS) Hold time, TDI, TMS after RTCKr 0 ns
4 th(RTCKr -TDO) Hold time, TDO after RTCKf 0 ns
5 td(TCKf -TDO) Delay time, TDO valid after RTCK fall(RTCKf) 12 ns
Figure 6-28. JTAG Timing

<!-- Page 148 -->
H L H L H L L H
.□.□..□.□.
128-bit□comparator
H L L H H L L HUNLOCKFlash□Module□Output
OTP Contents
Unlock□By□Scan
Register
Internal□Tie-Offs
(example□only)(example)
L H H L L Internal□Tie-Offs
(example□only)L H H
148TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.7 Advanced JTAG Security Module
This device includes aanAdvanced JTAG Security Module (AJSM) module. The AJSM provides
maximum security tothe memory content ofthe device byletting users secure the device after
programming.
Figure 6-29. AJSM Unlock
The device isunsecure bydefault byvirtue ofa128-bit visible unlock code programmed intheOTP
address 0xF0000000. The OTP contents areXOR-ed with thecontents ofthe"Unlock ByScan" register.
The outputs ofthese XOR gates areagain combined with asetofsecret internal tie-offs. The output of
this combinational logic iscompared against asecret hard-wired 128-bit value. Amatch results inthe
UNLOCK signal being asserted, sothatthedevice isnow unsecure.
Auser cansecure thedevice bychanging atleast 1bitinthevisible unlock code from 1to0.Changing a
0to1isnotpossible because thevisible unlock code isstored intheOne Time Programmable (OTP)
flash region. Also, changing all128 bitstozeros isnotavalid condition and willpermanently secure the
device.
Once secured, auser can unsecure thedevice byscanning anappropriate value into the"Unlock By
Scan" register oftheAJSM. The value tobescanned issuch that theXOR oftheOTP contents and the
Unlock-By-Scan register contents results intheoriginal visible unlock code.
The Unlock-By-Scan register isreset only upon asserting power-on reset (nPORRST).
Asecure device only permits JTAG accesses totheAJSM scan chain through theSecondary Tap 2ofthe
ICEPick module. Allother secondary taps, testtaps, andtheboundary scan interface arenotaccessible in
thisstate.

<!-- Page 149 -->
tr(ETM)th(ETM)tl(ETM)
tf(ETM)
tcyc(ETM)
149TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.8 Embedded Trace Macrocell (ETM-R5)
The device contains aETM-R5 module with a32-bit internal data port. The ETM-R5 module isconnected
toaTrace Port Interface Unit (TPIU) with a32-bit data bus. The TPIU provides a35-bit (32-bit data, 3-bit
control) external interface fortrace. The ETM-R5 isCoreSight compliant and follows the ETM v3
specification. Formore details, seetheARM CoreSight ETM-R5 TRM specification.
6.22.8.1 ETM TRACECLKIN Selection
The ETM clock source canbeselected aseither VCLK ortheexternal ETMTRACECLKIN terminal. The
selection ischosen bytheEXTCTLOUT[1:0] control bitsoftheTPIU (default is'00'). The address ofthis
register istheTPIU base address +0x404.
Before theuser begins accessing TPIU registers, theTPIU should beunlocked through theCoreSight key
and1or2written tothisregister.
Table 6-55. TPIU /TRACECLKIN Selection
EXTCTLOUT[1:0] TPIU/TRACECLKIN
00 Tied-zero
01 VCLK
10 ETMTRACECLKIN
11 Tied-zero
6.22.8.2 Timing Specifications
Figure 6-30. ETMTRACECLKOUT Timing
Table 6-56. ETMTRACECLK Timing
PARAMETER MIN MAX UNIT
tcyc(ETM) Clock period 18.18 ns
tl(ETM) Low pulse width 6 ns
th(ETM) High pulse width 6 ns
tr(ETM) Clock anddata risetime 3 ns
tf(ETM) Clock anddata falltime 3 ns

<!-- Page 150 -->
tsu(ETM) th(ETM)ETMTRACECLK
ETMDA T A
tsu(ETM) th(ETM)
150TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-31. ETMDATA Timing
Table 6-57. ETMDATA Timing
PARAMETER MIN MAX UNIT
tsu(ETM) Data setup time 2.5 ns
th(ETM) Data hold time 1.5 ns
NOTE
The ETMTRACECLK and ETMDATA timing isbased ona15-pF load and forambient
temperatures lower than 85°C.

<!-- Page 151 -->
tcyc(RTP)tr tfth(RTP)tl(RTP)
151TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.9 RAM Trace Port (RTP)
The RTP provides theability todatalog theRAM contents oftheTMS570 devices oraccesses to
peripherals without program intrusion. Itcan trace alldata write orread accesses tointernal RAM. In
addition, itprovides thecapability todirectly transfer data toaFIFO tosupport aCPU-controlled
transmission ofthedata. The trace data istransmitted over adedicated external interface.
6.22.9.1 RTP Features
The RTP offers thefollowing features:
*Two modes ofoperation -Trace Mode andDirect Data Mode
-Trace Mode
*Nonintrusive data trace onwrite orread operation
*Visibility ofRAM content atanytime onexternal capture hardware
*Trace ofperipheral accesses
*2configurable trace regions foreach RAM module tolimit amount ofdata tobetraced
*FIFO tostore data andaddress ofdata ofmultiple read/write operations
*Trace ofCPU and/or DMA accesses with indication ofthemaster inthetransmitted data packet
-Direct Data Mode
*Directly write data with theCPU ortrace read operations toaFIFO, without transmitting header
andaddress information
*Dedicated synchronous interface totransmit data toexternal devices
*Free-running clock generation orclock stop mode between transmissions
*Upto100Mbps terminal transfer rate fortransmitting data
*Pins notused infunctional mode canbeused asGIOs
6.22.9.2 Timing Specifications
Figure 6-32. RTPCLK Timing
Table 6-58. RTPCLK Timing
PARAMETER MIN MIN UNIT
tcyc(RTP) Clock period 9.09 (=110MHz) ns
th(RTP) High pulse width ((tcyc(RTP) )/2)-((tr+tf)/2) ns
tl(RTP) Low pulse width ((tcyc(RTP) )/2)-((tr+tf)/2) ns

<!-- Page 152 -->
HCLK
RTPCLK
RTPENA
RTPSYNC
RTPDATAHCLK
RTPCLK
RTPENA
RTPSYNC
RTPDATAtena(RTP) tdis(RTP)
HCLK
RTPCLK
RTPnENA
RTPSYNC
RTPDA T A1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
d1 d2 d3 d4 d5 d6 d7 d8
Divide by 1
RTPSYNC
RTPCLK
RTPDATAtssu(RTP) tsh(RTP)
tdsu(RTP) tdh(RTP)
152TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-33. RTPDATA Timing
Table 6-59. RTPDATA Timing
PARAMETER MIN MAX UNIT
tdsu(RTP) Data setup time 3 ns
tdh(RTP) Data hold time 1 ns
tssu(RTP) SYNC setup time 3 ns
tsh(RTP) SYNC hold time 1 ns
Figure 6-34. RTPnENA timing
Table 6-60. RTPnENA timing
PARAMETER MIN MAX UNIT
tdis(RTP) Disable time, time RTPnENA must gohigh before what
would bethenext RTPSYNC, toensure delaying thenext
packet3tc(HCLK) +tr(RTPSYNC)
+12 ns
tena(RTP) Enable time, time after RTPnENA goes lowbefore a
packet thathasbeen halted, resumes4tc(HCLK) +tr(RTPSYNC) 5tc(HCLK) +tr(RTPSYNC)
+12ns

<!-- Page 153 -->
tcyc(DMM)tr tfth(DMM)tl(DMM)
153TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.10 Data Modification Module (DMM)
The Data Modification Module (DMM) provides thecapability tomodify data intheentire 4GB address
space oftheTMS570devices from anexternal peripheral, with minimal interruption oftheapplication.
6.22.10.1 DMM Features
The DMM module hasthefollowing features:
*Acts asabusmaster, enabling direct writes tothe4GB address space without CPU intervention
*Writes tomemory locations specified inthereceived packet (leverages packets defined bytrace mode
oftheRAM Trace Port (RTP) module
*Writes received data toconsecutive addresses, which arespecified bytheDMM module (leverages
packets defined bydirect data mode oftheRTP module)
*Configurable port width (1-,2-,4-,8-,16-pins)
*Upto100Mbps terminal data rate
*Unused pins configurable asGIO pins
6.22.10.2 Timing Specifications
Table 6-61. DMMCLK Timing (see Figure 6-35)
PARAMETER MIN MAX UNIT
tcyc(DMM) Cycle time, DMMCLK clock period 9.09 ns
th(DMM) High-pulse width ((tcyc(DMM) )/2)-((tr+tf)/2) ns
tl(DMM) Low-pulse width ((tcyc(DMM) )/2)-((tr+tf)/2) ns
Figure 6-35. DMMCLK Timing
Table 6-62. DMMDATA Timing (see Figure 6-36)
PARAMETER MIN MAX UNIT
tssu(DMM) Setup time, SYNC active before clkfalling edge 2 ns
tsh(DMM) Hold time, clkfalling edge after SYNC deactive 3 ns
tdsu(DMM) Setup time, DATA before clkfalling edge 2 ns
tdh(DMM) Hold time, clkfalling edge after DATA hold time 3 ns

<!-- Page 154 -->
HCLK
DMMCLK
DMMSYNC
DMMDA T A
DMMnENAD00 D01 D10 D1 1 D20 D21 D30 D31 D40 D41 D50
DMMSYNC
DMMCLK
DMMDATAtssu(DMM) tsh(DMM)
tdsu(DMM) tdh(DMM)
154TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-36. DMMDATA Timing
Figure 6-37 shows acase with 1DMM packet per2DMMCLK cycles (Mode =Direct Data Mode, data
width =8,portwidth =4)where none ofthepackets received bytheDMM aresent out,leading tofilling up
oftheinternal buffers. The DMMnENA signal isshown asserted, after thefirst twopackets have been
received andsynchronized totheHCLK domain. Here, theDMM hasthecapacity toaccept packets D4x,
D5x, D6x, D7x. Packet D8would result inanoverflow. Once DMMnENA isasserted, theDMM expects to
stop receiving packets after 4HCLK cycles; once DMMnENA isdeasserted, theDMM canhandle packets
immediately (after 0HCLK cycles).
Figure 6-37. DMMnENA Timing

<!-- Page 155 -->
TRST
TMS
TCK
TDI
TDORTCKIC EPICKBoundary
BSDLBoundary Scan InterfaceScanDevice Pins (conceptual)
TDI
TDO
155TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.22.11 Boundary Scan Chain
The device supports BSDL-compliant boundary scan fortesting pin-to-pin compatibility. The boundary
scan chain isconnected totheBoundary Scan Interface oftheICEPICK module (see Figure 6-38 ).
Figure 6-38. Boundary Scan Implementation (Conceptual Diagram)
Data isserially shifted intoallboundary-scan buffers through TDI, andoutthrough TDO.