# Error Signaling Module - Channel Assignments

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 129-137 (9 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 129 -->
129TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.19 Error Signaling Module
The Error Signaling Module (ESM) manages the various error conditions onthe TMS570LCx
microcontroller. The error condition ishandled based onafixed severity level assigned toit.Any severe
error condition canbeconfigured todrive alowlevel onadedicated device terminal called nERROR. The
nERROR canbeused asanindicator toanexternal monitor circuit toputthesystem intoasafe state.
6.19.1 ESM Features
The features oftheESM are:
*160interrupt/error channels aresupported, divided intothree groups
-96channels with maskable interrupt andconfigurable error terminal behavior
-32error channels with nonmaskable interrupt andpredefined error terminal behavior
-32channels with predefined error terminal behavior only
*Error terminal tosignal severe device failure
*Configurable time base forerror signal
*Error forcing capability
6.19.2 ESM Channel Assignments
The ESM integrates allthedevice error conditions and groups them intheorder ofseverity. Group1 is
used forerrors ofthelowest severity while Group3 isused forerrors ofthehighest severity. The device
response toeach error isdetermined bytheseverity group towhich theerror isconnected. Table 6-45
lists thechannel assignment foreach group.
Table 6-44. ESM Groups
ERROR GROUP INTERRUPT CHARACTERISTICSINFLUENCE ON
ERROR
TERMINAL
Group1 Maskable, loworhigh priority Configurable
Group2 Nonmaskable, high priority Fixed
Group3 Nointerrupt generated Fixed
Table 6-45. ESM Channel Assignments
ESM ERROR SOURCES GROUP CHANNELS
Group1
Reserved Group1 0
MibADC2 -parity Group1 1
DMA -MPU error forCPU (DMAOCP_MPVINT(0)) Group1 2
DMA -ECC uncorrectable error Group1 3
EPC -Correctable Error Group1 4
Reserved Group1 5
L2FMC -correctable error (implicit OTP read). Group1 6
NHET1 -parity Group1 7
HET TU1/HET TU2 -parity Group1 8
HET TU1/HET TU2 -MPU Group1 9
PLL1 -slip Group1 10
LPO Clock Monitor -interrupt Group1 11
FlexRay RAM -ECC uncorrectable error Group1 12
Reserved Group1 13
FlexRay TURAM -ECC uncorrectable error (TU_UCT_err) Group1 14

<!-- Page 130 -->
130TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-45. ESM Channel Assignments (continued)
ESM ERROR SOURCES GROUP CHANNELS
VIM RAM -ECC uncorrectable error Group1 15
FlexRay TU-MPU violation (TU_MPV_err) Group1 16
MibSPI1 -ECC uncorrectable error Group1 17
MibSPI3 -ECC uncorrectable error Group1 18
MibADC1 -parity Group1 19
DMA -Bus Error Group1 20
DCAN1 -ECC uncorrectable error Group1 21
DCAN3 -ECC uncorrectable error Group1 22
DCAN2 -ECC uncorrectable error Group1 23
MibSPI5 -ECC uncorrectable error Group1 24
Reserved Group1 25
L2RAMW -correctable error Group1 26
Cortex-R5F CPU -self-test Group1 27
Reserved Group1 28
Reserved Group1 29
DCC1 -error Group1 30
CCM-R5F -self-test Group1 31
Reserved Group1 32
Reserved Group1 33
NHET2 -parity Group1 34
Reserved Group1 35
Reserved Group1 36
IOMM -Mux configuration error Group1 37
Power domain compare error Group1 38
Power domain self-test error Group1 39
eFuse farm -EFC error Group1 40
eFuse farm -self-test error Group1 41
PLL2 -slip Group1 42
Ethernet Controller master interface Group1 43
Reserved Group1 44
Reserved Group1 45
Cortex-R5F Core -cache correctable error event Group1 46
ACP d-cache invalidate Group1 47
Reserved Group1 48
MibSPI2 -ECC uncorrectable error Group1 49
MibSPI4 -ECC uncorrectable error Group1 50
DCAN4 -ECC uncorrectable error Group1 51
CPU Interconnect Subsystem -Global error Group1 52
CPU Interconnect Subsystem -Global Parity Error Group1 53
NHET1/2 -self-test error Group1 54
NMPU -EMAC MPU Error Group1 55
Reserved Group1 56
Reserved Group1 57
Reserved Group1 58
Reserved Group1 59
Reserved Group1 60
NMPU -PS_SCR_S MPU Error Group1 61

<!-- Page 131 -->
131TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-45. ESM Channel Assignments (continued)
ESM ERROR SOURCES GROUP CHANNELS
DCC2 -error Group1 62
Reserved Group1 63
Reserved Group1 64
Reserved Group1 65
Reserved Group1 66
Reserved Group1 67
Reserved Group1 68
NMPU -DMA Port AMPU Error Group1 69
DMA -Transaction Bus Parity Error Group1 70
FlexRay TURAM- ECC single biterror (TU_SBE_err) Group1 71
FlexRay -ECC single biterror Group1 72
DCAN1 -ECC single biterror Group1 73
DCAN2 -ECC single biterror Group1 74
DCAN3 -ECC single biterror Group1 75
DCAN4 -ECC single biterror Group1 76
MIBSPI1 -ECC single biterror Group1 77
MIBSPI2 -ECC single biterror Group1 78
MIBSPI3 -ECC single biterror Group1 79
MIBSPI4 -ECC single biterror Group1 80
MIBSPI5 -ECC single biterror Group1 81
DMA -ECC single biterror Group1 82
VIM -ECC single biterror Group1 83
EMIF 64-bit Bridge I/FECC uncorrectable error Group1 84
EMIF 64-bit Bridge I/FECC single biterror Group1 85
Reserved Group1 86
Reserved Group1 87
DMA -Register Soft Error Group1 88
L2FMC -Register Soft Error Group1 89
SYS -Register Soft Error Group1 90
SCM -Time-out Error Group1 91
CCM-R5F -Operating status Group1 92
Reserved Group1 93-95
Group2
Reserved Group2 0
Reserved Group2 1
CCM-R5F -CPU compare error Group2 2
Cortex-R5F Core -Allfatal buserror events. [Commonly caused byimproper
orincomplete ECC values inFlash.]
Group2 3Event Reference Event Description EVNTBUSm bit
0x71 Bus ECC 48
Reserved Group2 4
Reserved Group2 5
Reserved Group2 6
L2RAMW -Uncorrectable error type B Group2 7
Reserved Group2 8
Reserved Group2 9
Reserved Group2 10
Reserved Group2 11

<!-- Page 132 -->
132TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-45. ESM Channel Assignments (continued)
ESM ERROR SOURCES GROUP CHANNELS
Reserved Group2 12
Reserved Group2 13
Reserved Group2 14
Reserved Group2 15
Reserved Group2 16
L2FMC -parity error
*Mcmd parity error onIdlecommand
*POM idlestate parity error
*Port A/BIdlestate parity errorGroup2 17
Reserved Group2 18
L2FMC -double bitECC error-error duetoimplicit OTP reads Group2 19
Reserved Group2 20
EPC -Uncorrectable Error Group2 21
Reserved Group2 22
Reserved Group2 23
RTI_WWD_NMI Group2 24
CCM-R5F VIM compare error Group2 25
CPU1 AXIM Bus Monitor failure Group2 26
Reserved Group2 27
CCM-R5F -Power Domain monitor error Group2 28
Reserved Group2 29
Reserved Group2 30
Reserved Group2 31
Group3
Reserved Group3 0
eFuse Farm -autoload error Group3 1
Reserved Group3 2
L2RAMW -double bitECC uncorrectable error Group3 3
Reserved Group3 4
Reserved Group3 5
Reserved Group3 6
Reserved Group3 7
Reserved Group3 8
Cortex-R5F Core -Allfatal events (OR of:
Group3 9Event Reference
ValueEvent Description EVNTBUSm Bit
0x60 Data Cache 33
0x61 Data Cache tag/dirty 34
Reserved Group3 10
Reserved Group3 11
CPU Interconnect Subsystem -Diagnostic Error Group3 12
L2FMC -uncorrectable error dueto:
*address parity/internal parity error
*address tag
*internal switch time-outGroup3 13
L2RAMW -Uncorrectable error Type A Group3 14
L2RAMW -Address/Control parity error Group3 15
Reserved Group3 16

<!-- Page 133 -->
133TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-45. ESM Channel Assignments (continued)
ESM ERROR SOURCES GROUP CHANNELS
Reserved Group3 17
Reserved Group3 18
Reserved Group3 19
Reserved Group3 20
Reserved Group3 21
Reserved Group3 22
Reserved Group3 23
Reserved Group3 24

<!-- Page 134 -->
134TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The Undefined Instruction TRAP isnotdetectable outside theCPU. The trap istaken only iftheinstruction reaches theexecute stage of
theCPU.6.20 Reset /Abort /Error Sources
Table 6-46. Reset/Abort/Error Sources
ERROR SOURCE SYSTEM MODE ERROR RESPONSEESM HOOKUP
GROUP.CHANNE
L
CPU TRANSACTIONS
Precise write error (NCNB/Strongly Ordered) User/Privilege Precise Abort (CPU) N/A
Precise read error (NCB/Device orNormal) User/Privilege Precise Abort (CPU) N/A
Imprecise write error (NCB/Device orNormal) User/Privilege Imprecise Abort (CPU) N/A
Illegal instruction User/PrivilegeUndefined Instruction Trap
(CPU)(1) N/A
MPU access violation User/Privilege Abort (CPU) N/A
Correctable error User/Privilege ESM 1.4
Uncorrectable error User/Privilege ESM =>NMI 2.21
LEVEL 2SRAM
CPU Write ECC single error (correctable) User/Privilege ESM 1.26
ECC double biterror:
Read-Modify-Write (RMW) ECC double error
CPU Write ECC double errorUser/Privilege Bus Error, ESM =>nERROR 3.3
Uncorrectable error Type A:
Write SECDED malfunction error
Redundant address decode error
Read SECDED malfunction errorUser/Privilege Bus Error, ESM =>nERROR 3.14
Uncorrectable error type B:
Memory scrubbing SECDED malfunction error
Memory scrubbing Redundant address decode error
Memory scrubbing address/control parity error
Write data merged mux diagnostic error
Write SECDED malfunction diagnostic error
Read SECDED malfunction diagnostic error
Write ECC correctable anduncorrectable diagnostic error
Read ECC correctable anduncorrectable diagnostic error
Write data merged mux error
Redundant address decode diagnostic error
Command parity error onidleUser/Privilege ESM =>NMI 2.7
Address/Control parity error User/Privilege Bus Error, ESM =>nERROR 3.15
Level 2RAM illegal address error Memory initialization error User/Privilege Bus Error N/A
FLASH
L2FMC correctable error -single bitECC error forimplicit OTP
readUser/Privilege ESM 1.6
L2FMC uncorrectable error -double bitECC error forimplicit
OTP readUser/Privilege ESM =>NMI 2.19
L2FMC fatal uncorrectable error:
address parity error/internal parity error
address tagerror
Internal switch time-outUser/Privilege Bus Error, ESM =>nERROR 3.13
L2FMC parity error:
Mcmd parity error onIdlecommand
POM idlestate parity error
Port A/BIdlestate parity errorUser/Privilege ESM =>NMI 2.17
L2FMC nonfatal uncorrectable error:
Response error onPOM
Response parity error onPOM
Bank accesses during special operation (program/erase) bythe
FSM
Bank/Pump insleep
Unimplemented special/unavailable spaceUser/Privilege Bus Error N/A

<!-- Page 135 -->
135TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-46. Reset/Abort/Error Sources (continued)
ERROR SOURCE SYSTEM MODE ERROR RESPONSEESM HOOKUP
GROUP.CHANNE
L
L2FMC register softerror. User/Privilege ESM 1.89
DMA TRANSACTIONS
Memory access permission violation User/Privilege ESM 1.2
Memory ECC uncorrectable error User/Privilege ESM 1.3
Transaction Error:
thatis,Bus Parity ErrorUser/Privilege ESM 1.70
Memory ECC single biterror User/Privilege ESM 1.82
DMA register softerror User/Privilege ESM 1.88
DMA buserror User/Privilege ESM 1.20
EMIF_ECC
64-bit Bridge I/FECC uncorrectable error User/Privilege ESM 1.84
64-bit Bridge I/FECC single error User/Privilege ESM 1.85
HET TU1 (HTU1)
NCNB (Strongly Ordered) transaction with slave error response User/Privilege Interrupt =>VIM N/A
External imprecise error (Illegal transaction with okresponse) User/Privilege Interrupt =>VIM N/A
Memory access permission violation User/Privilege ESM 1.9
Memory parity error User/Privilege ESM 1.8
HET TU2 (HTU2)
NCNB (Strongly Ordered) transaction with slave error response User/Privilege Interrupt =>VIM N/A
External imprecise error (Illegal transaction with okresponse) User/Privilege Interrupt =>VIM N/A
Memory access permission violation User/Privilege ESM 1.9
Memory parity error User/Privilege ESM 1.8
N2HET1
Memory parity error User/Privilege ESM 1.7
N2HET2
Memory parity error User/Privilege ESM 1.34
MibSPI
MibSPI1 memory ECC uncorrectable error User/Privilege ESM 1.17
MibSPI2 memory ECC uncorrectable error User/Privilege ESM 1.49
MibSPI3 memory ECC uncorrectable error User/Privilege ESM 1.18
MibSPI4 memory ECC uncorrectable error User/Privilege ESM 1.50
MibSPI5 memory ECC uncorrectable error User/Privilege ESM 1.24
MibSPI1 memory ECC single error User/Privilege ESM 1.77
MibSPI2 memory ECC single error User/Privilege ESM 1.78
MibSPI3 memory ECC single error User/Privilege ESM 1.79
MibSPI4 memory ECC single error User/Privilege ESM 1.80
MibSPI5 memory ECC single error User/Privilege ESM 1.81
MibADC
MibADC1 Memory parity error User/Privilege ESM 1.19
MibADC2 Memory parity error User/Privilege ESM 1.1
DCAN
DCAN1 memory ECC uncorrectable error User/Privilege ESM 1.21
DCAN2 memory ECC uncorrectable error User/Privilege ESM 1.23
DCAN3 memory ECC uncorrectable error User/Privilege ESM 1.22
DCAN4 memory ECC uncorrectable error User/Privilege ESM 1.51
DCAN1 memory ECC single error User/Privilege ESM 1.73

<!-- Page 136 -->
136TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-46. Reset/Abort/Error Sources (continued)
ERROR SOURCE SYSTEM MODE ERROR RESPONSEESM HOOKUP
GROUP.CHANNE
L
DCAN2 memory ECC single error User/Privilege ESM 1.74
DCAN3 memory ECC single error User/Privilege ESM 1.75
DCAN4 memory ECC single error User/Privilege ESM 1.76
PLL
PLL1 sliperror User/Privilege ESM 1.10
PLL2 sliperror User/Privilege ESM 1.42
Clock Monitor
Clock monitor interrupt User/Privilege ESM 1.11
DCC
DCC1 error User/Privilege ESM 1.30
DCC2 error User/Privilege ESM 1.62
CCM-R5F
Self-test failure User/Privilege ESM 1.31
CPU Bus Compare failure User/Privilege ESM =>NMI 2.2
VIM Bus Compare failure User/Privilege ESM =>NMI 2.25
Power Domain Monitor failure User/Privilege ESM =>NMI 2.28
CCM-R5F operating status (asserted when notinlockstep or
CCM-R5F isinself-test mode)User/Privilege ESM 1.92
EPC (Error Profiling Controller)
Correctable Error User/Privilege ESM 1.4
Uncorrectable Error User/Privilege ESM =>NMI 2.21
SCM (SCR Control module)
Time-out Error User/Privilege ESM 1.91
FlexRay
Memory ECC uncorrectable error User/Privilege ESM 1.12
Memory ECC single error User/Privilege ESM 1.72
FlexRay TU
NCNB (Strongly Ordered) transaction with slave error response User/Privilege Interrupt =>VIM N/A
External imprecise error (Illegal transaction with okresponse) User/Privilege Interrupt =>VIM N/A
Memory access permission violation User/Privilege ESM 1.16
Memory ECC uncorrectable error User/Privilege ESM 1.14
Memory ECC single biterror User/Privilege ESM 1.71
Ethernet master interface
Any error reported byslave being accessed User/Privilege ESM 1.43
VIM
Memory ECC uncorrectable error User/Privilege ESM 1.15
Memory ECC single biterror User/Privilege ESM 1.83
Voltage Monitor
VMON outofvoltage range N/A Reset N/A
Self-Test (LBIST)
Cortex-R5F CPU self-test (LBIST) error User/Privilege ESM 1.27
NHET Self-test (LBIST) error User/Privilege ESM 1.54
IOMM (terminal multiplexing control)
Mux configuration error User/Privilege ESM 1.37
Power Domain Control
Power Domain control access privilege error User Imprecise Abort (CPU) N/A

<!-- Page 137 -->
137TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-46. Reset/Abort/Error Sources (continued)
ERROR SOURCE SYSTEM MODE ERROR RESPONSEESM HOOKUP
GROUP.CHANNE
L
(2) Oscillator fail/PLL slipcanbeconfigured inthesystem register (SYS.PLLCTL1) togenerate areset.PSCON compare error User/Privilege ESM 1.38
PSCON self-test error User/Privilege ESM 1.39
Efuse farm
eFuse farm autoload error User/Privilege ESM 3.1
eFuse farm error User/Privilege ESM 1.40
eFuse farm self-test error User/Privilege ESM 1.41
WIndowed Watchdog
WWD Nonmaskable Interrupt Exception N/A ESM 2.24
Errors Reflected intheSYSESR Register
Power-Up Reset N/A Reset N/A
Oscillator fail/PLL slip(2)N/A Reset N/A
Watchdog exception N/A Reset N/A
CPUx Reset N/A Reset N/A
Software Reset N/A Reset N/A
External Reset N/A Reset N/A
Register Soft Error User/Privilege ESM 1.90
CPU Interconnect Subsystem
Diagnostic error User/Privilege ESM =>Error terminal 3.12
Global error User/Privilege ESM 1.52
Global Parity error User/Privilege ESM 1.53
NMPU forEMAC
MPU Access violation error User/Privilege ESM 1.55
NMPU forPS_SCR_S
MPU Access violation error User/Privilege ESM 1.61
NMPU forDMA Port A
MPU Access violation error User/Privilege ESM 1.69
PCR1
MasterID filtering MPU Access violation error User/Privilege Bus Error N/A
PCR2
MasterID filtering MPU Access violation error User/Privilege Bus Error N/A
PCR3
MasterID filtering MPU Access violation error User/Privilege Bus Error N/A