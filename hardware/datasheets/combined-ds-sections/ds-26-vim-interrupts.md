# VIM - Interrupt Request Assignments

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 117-120 (4 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 117 -->
VIM1
VIM2Interrupt
RequestsPCR
R5F-0
R5F-1nIRQ/nFIQ/IRQVECADDR
2 cyc
delay CCM-R5F ESM
2 cyc
delayCortex-R5 Processor Group
2 cyc
delay2 cyc
delay
117TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.15 Vectored Interrupt Manager
There aretwo on-chip Vector Interrupt Manager (VIM) modules. The VIM module provides hardware
assistance forprioritizing and controlling themany interrupt sources present onadevice. Interrupts are
caused byevents outside ofthenormal flow ofprogram execution. Normally, these events require atimely
response from theCPU; therefore, when aninterrupt occurs, theCPU switches execution from thenormal
program flow toaninterrupt service routine (ISR).
6.15.1 VIMFeatures
The VIM module hasthefollowing features:
*Supports 128interrupt channels
*Provides programmable priority fortherequest lines
*Manages interrupt channels through masking
*Prioritizes interrupt channels totheCPU
*Provides theCPU with theaddress oftheinterrupt service routine (ISR) foreach interrupt
The twoVIM modules areinlockstep. These twoVIM modules arememory mapped tothesame address
space. From aprogrammer 'smodel point ofview itisonly oneVIM module. Writes toVIM1 registers and
memory willbebroadcasted toboth VIM1 and VIM2. Reads from VIM1 willonly read theVIM1 registers
and memory. Allinterrupt requests which gototheVIM1 module willalso gototheVIM2 module.
Because theVIM1 and VIM2 have theidentical setup, both willresult inthesame output behavior
responding tothesame interrupt requests. The second VIM module acts asadiagnostic checker module
against thefirstVIM module. The output signals ofthetwoVIM modules arerouted toCCM-R5F module
andarecompared constantly. Mis-compare detected willbesignaled asanerror totheESM module. The
lockstep VIM pairtakes care oftheinterrupt generation tothelockstep R5F pair.
6.15.2 Interrupt Generation
Toavoid common mode failures theinput and output signals ofthetwoVIMs aredelayed inadifferent
way asshown inFigure 6-17.
Figure 6-17. Interrupt Generation

<!-- Page 118 -->
118TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.15.3 Interrupt Request Assignments
Table 6-39. Interrupt Request Assignments
MODULES VIMINTERRUPT SOURCESDEFAULT VIM
INTERRUPT CHANNEL
ESM ESM high-level interrupt (NMI) 0
Reserved Reserved 1
RTI RTI1 compare interrupt 0 2
RTI RTI1 compare interrupt 1 3
RTI RTI1 compare interrupt 2 4
RTI RTI1 compare interrupt 3 5
RTI RTI1 overflow interrupt 0 6
RTI RTI1 overflow interrupt 1 7
RTI RTI1 time-base 8
GIO GIO high level interrupt 9
NHET1 NHET1 high-level interrupt (priority level 1) 10
HET TU1 HET TU1 level 0interrupt 11
MIBSPI1 MIBSPI1 level 0interrupt 12
LIN1 LIN1 level 0interrupt 13
MIBADC1 MIBADC1 event group interrupt 14
MIBADC1 MIBADC1 software group 1interrupt 15
DCAN1 DCAN1 level 0interrupt 16
MIBSPI2 MIBSPI2 level 0interrupt 17
FlexRay FlexRay level 0interrupt (CC_int0) 18
CRC1 CRC1 Interrupt 19
ESM ESM low-level interrupt 20
SYSTEM Software interrupt forCortex-R5F (SSI) 21
CPU Cortex-R5F PMU Interrupt 22
GIO GIO lowlevel interrupt 23
NHET1 NHET1 lowlevel interrupt (priority level 2) 24
HET TU1 HET TU1 level 1interrupt 25
MIBSPI1 MIBSPI1 level 1interrupt 26
LIN1 LIN1 level 1interrupt 27
MIBADC1 MIBADC1 software group 2interrupt 28
DCAN1 DCAN1 level 1interrupt 29
MIBSPI2 MIBSPI2 level 1interrupt 30
MIBADC1 MIBADC1 magnitude compare interrupt 31
FlexRay FlexRay level 1interrupt (CC_int1) 32
DMA FTCA interrupt 33
DMA LFSA interrupt 34
DCAN2 DCAN2 level 0interrupt 35
DMM DMM level 0interrupt 36
MIBSPI3 MIBSPI3 level 0interrupt 37
MIBSPI3 MIBSPI3 level 1interrupt 38
DMA HBCA interrupt 39
DMA BTCA interrupt 40
EMIF AEMIFINT 41
DCAN2 DCAN2 level 1interrupt 42
DMM DMM level 1interrupt 43
DCAN1 DCAN1 IF3interrupt 44

<!-- Page 119 -->
119TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-39. Interrupt Request Assignments (continued)
MODULES VIMINTERRUPT SOURCESDEFAULT VIM
INTERRUPT CHANNEL
DCAN3 DCAN3 level 0interrupt 45
DCAN2 DCAN2 IF3interrupt 46
FPU FPU interrupt ofCortex-R5F 47
FlexRay TU FlexRay TUTransfer Status interrupt (TU_Int0) 48
MIBSPI4 MIBSPI4 level 0interrupt 49
MIBADC2 MibADC2 event group interrupt 50
MIBADC2 MibADC2 software group1 interrupt 51
FlexRay FlexRay T0C interrupt (CC_tint0) 52
MIBSPI5 MIBSPI5 level 0interrupt 53
MIBSPI4 MIBSPI4 level 1interrupt 54
DCAN3 DCAN3 level 1interrupt 55
MIBSPI5 MIBSPI5 level 1interrupt 56
MIBADC2 MibADC2 software group2 interrupt 57
FlexRay TU FlexRay TUError interrupt (TU_Int1) 58
MIBADC2 MibADC2 magnitude compare interrupt 59
DCAN3 DCAN3 IF3interrupt 60
L2FMC FSM_DONE interrupt 61
FlexRay FlexRay T1C interrupt (CC_tint1) 62
NHET2 NHET2 level 0interrupt 63
SCI3 SCI3 level 0interrupt 64
NHET TU2 NHET TU2 level 0interrupt 65
I2C1 I2Clevel 0interrupt 66
Reserved Reserved 67-72
NHET2 NHET2 level 1interrupt 73
SCI3 SCI3 level 1interrupt 74
NHET TU2 NHET TU2 level 1interrupt 75
Ethernet C0_MISC_PULSE 76
Ethernet C0_TX_PULSE 77
Ethernet C0_THRESH_PULSE 78
Ethernet C0_RX_PULSE 79
HWAG1 HWA_INT_REQ_H 80
HWAG2 HWA_INT_REQ_H 81
DCC1 DCC1 done interrupt 82
DCC2 DCC2 done interrupt 83
SYSTEM Reserved 84
PBIST PBIST Done 85
Reserved Reserved 86-87
HWAG1 HWA_INT_REQ_L 88
HWAG2 HWA_INT_REQ_L 89
ePWM1INTn ePWM1 Interrupt 90
ePWM1TZINTn ePWM1 Trip Zone Interrupt 91
ePWM2INTn ePWM2 Interrupt 92
ePWM2TZINTn ePWM2 Trip Zone Interrupt 93
ePWM3INTn ePWM3 Interrupt 94
ePWM3TZINTn ePWM3 Trip Zone Interrupt 95
ePWM4INTn ePWM4 Interrupt 96
ePWM4TZINTn ePWM4 Trip Zone Interrupt 97

<!-- Page 120 -->
120TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-39. Interrupt Request Assignments (continued)
MODULES VIMINTERRUPT SOURCESDEFAULT VIM
INTERRUPT CHANNEL
ePWM5INTn ePWM5 Interrupt 98
ePWM5TZINTn ePWM5 Trip Zone Interrupt 99
ePWM6INTn ePWM6 Interrupt 100
ePWM6TZINTn ePWM6 Trip Zone Interrupt 101
ePWM7INTn ePWM7 Interrupt 102
ePWM7TZINTn ePWM7 Trip Zone Interrupt 103
eCAP1INTn eCAP1 Interrupt 104
eCAP2INTn eCAP2 Interrupt 105
eCAP3INTn eCAP3 Interrupt 106
eCAP4INTn eCAP4 Interrupt 107
eCAP5INTn eCAP5 Interrupt 108
eCAP6INTn eCAP6 Interrupt 109
eQEP1INTn eQEP1 Interrupt 110
eQEP2INTn eQEP2 Interrupt 111
Reserved Reserved 112
DCAN4 DCAN4 Level 0interrupt 113
I2C2 I2C2 interrupt 114
LIN2 LIN2 level 0interrupt 115
SCI4 SCI4 level 0interrupt 116
DCAN4 DCAN4 Level 1interrupt 117
LIN2 LIN2 level 1interrupt 118
SCI4 SCI4 level 1interrupt 119
DCAN4 DCAN4 IF3Interrupt 120
CRC2 CRC2 Interrupt 121
Reserved Reserved 122
Reserved Reserved 123
EPC EPC FIFO FULL orCAM FULL interrupt 124
Reserved Reserved 125-127
NOTE
Address location 0x00000000 intheVIM RAM isreserved forthephantom interrupt ISR
entry; therefore only request channels 0..126 canbeused and areoffset byone address in
theVIM RAM.
NOTE
The EMIF_nWAIT signal has apull-up onit.The EMIF module generates a"Wait Rise"
interrupt whenever itdetects arising edge onthe EMIF_nWAIT signal. This interrupt
condition isindicated assoon asthedevice ispowered up.This can beignored ifthe
EMIF_nWAIT signal isnotused intheapplication. IftheEMIF_nWAIT signal isactually used
intheapplication, then theexternal slave memory must always drive theEMIF_nWAIT signal
such thataninterrupt isnotcaused duetothedefault pull-up onthissignal.
NOTE
The lower-order interrupt channels arehigher priority channels than thehigher-order interrupt
channels.