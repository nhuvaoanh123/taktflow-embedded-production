# Device Memory Map

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 90-100 (11 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 90 -->
Flash (4MB)RAM (512KB)
0x000000000x003FFFFF0x080000000x0807FFFFCRC10xFE0000000xFEFFFFFFSYSTEM Peripherals - Frame 10xFFFFFFFF
RAM - ECC0x084000000x0847FFFFRESERVED
RESERVED
RESERVED0xF0000000
EMIF (16MB * 3)
0x600000000x6FFFFFFF
CS2RESERVED
CS3
RESERVEDCS4Peripherals - Frame 2
0xFC0000000xFCFFFFFF
EMIF (128MB)
0x800000000x87FFFFFF
CS0RESERVED
reserved
Async RAMSDRAM
0x640000000x680000000x6C000000Flash
(Flash ECC, OTP and EEPROM accesses)0xF047FFFFPeripherals - Frame 30xFFF80000
0xFFF7FFFF
0xFF000000
0xFB0000000xFBFFFFFF
CRC2
RESERVED
R5F Cache0x300000000x33FFFFFF
RESERVEDRESERVED0x340000000x37FFFFFF
90TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.9 Device Memory Map
6.9.1 Memory Map Diagram
Figure 6-9shows thedevice memory map.
Figure 6-9.Memory Map

<!-- Page 91 -->
91TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.9.2 Memory Map Table
Table 6-25. Module Registers /Memories Memory Map
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
Level 2Memories
Level 2Flash Data Space 0x0000_0000 0x003F_FFFF 4MB 4MB Abort
Level 2RAM 0x0800_0000 0x083F_FFFF 4MB 512KB Abort
Level 2RAM ECC 0x0840_0000 0x087F_FFFF 4MB 512KB
Accelerator Coherency Port
Accelerator Coherency Port 0x0800_0000 0x087F_FFFF 8MB 512KB Abort
Level 1Cache Memories
Cortex-R5F Data Cache
Memory0x3000_0000 0x30FF_FFFF 16MB 32KBAbort
Cortex-R5F Instruction Cache
Memory0x3100_0000 0x31FF_FFFF 16MB 32KB
External Memory Accesses
EMIF Chip Select 2
(asynchronous)0x6000_0000 0x63FF_FFFF 64MB 16MBAccess to
"Reserved" space
willgenerate AbortEMIF Chip Select 3
(asynchronous)0x6400_0000 0x67FF_FFFF 64MB 16MB
EMIF Chip Select 4
(asynchronous)0x6800_0000 0x6BFF_FFFF 64MB 16MB
EMIF Chip Select 0
(synchronous)0x8000_0000 0x87FF_FFFF 128MB 128MB
Flash OTP, ECC, EEPROM Bank
Customer OTP, Bank0 0xF000_0000 0xF000_1FFF 8KB 4KB Abort
Customer OTP, Bank1 0xF000_2000 0xF000_3FFF 8KB 4KB
Customer OTP, EEPROM
Bank0xF000_E000 0xF000_FFFF 8KB 1KB
Customer OTP-ECC, Bank0 0xF004_0000 0xF004_03FF 1KB 512B
Customer OTP-ECC, Bank1 0xF004_0400 0xF004_07FF 1KB 512B
Customer OTP-ECC,
EEPROM Bank0xF004_1C00 0xF004_1FFF 1KB 128B
TIOTP, Bank0 0xF008_0000 0xF008_1FFF 8KB 4KB
TIOTP, Bank1 0xF008_2000 0xF008_3FFF 8KB 4KB
TIOTP, EEPROM Bank 0xF008_E000 0xF008_FFFF 8KB 1KB
TIOTP-ECC, Bank0 0xF00C_0000 0xF00C_03FF 1KB 512B
TIOTP-ECC, Bank1 0xF00C_0400 0xF00C_07FF 1KB 512B Abort
TIOTP-ECC, EEPROM Bank 0xF00C_1C00 0xF00C_1FFF 1KB 128B
EEPROM Bank-ECC 0xF010_0000 0xF01F_FFFF 1MB 16KB
EEPROM Bank 0xF020_0000 0xF03F_FFFF 2MB 128KB
Flash Data Space ECC 0xF040_0000 0xF05F_FFFF 2MB 512KB
Interconnect SDC MMR
Interconnect SDC MMR 0xFA00_0000 0xFAFF_FFFF 16MB 16MB
Registers/Memories under PCR2 (Peripheral Segment 2)
CPPI Memory Slave (Ethernet
RAM)PCS[41] 0xFC52_0000 0xFC52_1FFF 8KB 8KBAbort
CPGMAC Slave (Ethernet
Slave)PS[30]-PS[31] 0xFCF7_8000 0xFCF7_87FF 2KB 2KBNoError
CPGMACSS Wrapper
(Ethernet Wrapper)PS[29] 0xFCF7_8800 0xFCF7_88FF 256B 256BNoError
Ethernet MDIO Interface PS[29] 0xFCF7_8900 0xFCF7_89FF 256B 256B NoError

<!-- Page 92 -->
92TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-25. Module Registers /Memories Memory Map (continued)
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
ePWM1
PS[28]0xFCF7_8C00 0xFCF7_8CFF 256B 256B Abort
ePWM2 0xFCF7_8D00 0xFCF7_8DFF 256B 256B Abort
ePWM3 0xFCF7_8E00 0xFCF7_8EFF 256B 256B Abort
ePWM4 0xFCF7_8F00 0xFCF7_8FFF 256B 256B Abort
ePWM5
PS[27]0xFCF7_9000 0xFCF7_90FF 256B 256B Abort
ePWM6 0xFCF7_9100 0xFCF7_91FF 256B 256B Abort
ePWM7 0xFCF7_9200 0xFCF7_92FF 256B 256B Abort
eCAP1 0xFCF7_9300 0xFCF7_93FF 256B 256B Abort
eCAP2
PS[26]0xFCF7_9400 0xFCF7_94FF 256B 256B Abort
eCAP3 0xFCF7_9500 0xFCF7_95FF 256B 256B Abort
eCAP4 0xFCF7_9600 0xFCF7_96FF 256B 256B Abort
eCAP5 0xFCF7_9700 0xFCF7_97FF 256B 256B Abort
eCAP6
PS[25]0xFCF7_9800 0xFCF7_98FF 256B 256B Abort
eQEP1 0xFCF7_9900 0xFCF7_99FF 256B 256B Abort
eQEP2 0xFCF7_9A00 0xFCF7_9AFF 256B 256B Abort
PCR2 registersPPSE[4] -PPSE[5] 0xFCFF_1000 0xFCFF_17FF 2KB 2KBReads return zeros,
writes have noeffect
NMPU (EMAC) PPSE[6] 0xFCFF_1800 0xFCFF_18FF 512B 512B Abort
EMIF Registers PPS[2] 0xFCFF_E800 0xFCFF_E8FF 256B 256B Abort
Cyclic Redundancy Checker (CRC) Module Register Frame
CRC 1
0xFE00_0000 0xFEFF_FFFF 16MB 512KBAccesses above
0xFE000200
generate abort.
CRC2
0xFB00_0000 0xFBFF_FFFF 16MB 512KBAccesses above
0xFB000200
generate abort.

<!-- Page 93 -->
93TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-25. Module Registers /Memories Memory Map (continued)
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
Memories under User PCR3 (Peripheral Segment 3)
MIBSPI5 RAMPCS[5] 0xFF0A_0000 0xFF0B_FFFF 128KB 2KBAbort foraccesses
above 2KB
MIBSPI4 RAMPCS[3] 0xFF06_0000 0xFF07_FFFF 128KB 2KBAbort foraccesses
above 2KB
MIBSPI3 RAMPCS[6] 0xFF0C_0000 0xFF0D_FFFF 128KB 2KBAbort foraccesses
above 2KB
MIBSPI2 RAMPCS[4] 0xFF08_0000 0xFF09_FFFF 128KB 2KBAbort foraccesses
above 2KB
MIBSPI1 RAMPCS[7] 0xFF0E_0000 0xFF0F_FFFF 128KB 4KBAbort foraccesses
above 4KB
DCAN4 RAM
PCS[12] 0xFF18_0000 0xFF19_FFFF 128KB 8KBAbort generated for
accesses beyond
offset 0x2000
DCAN3 RAM
PCS[13] 0xFF1A_0000 0xFF1B_FFFF 128KB 8KBAbort generated for
accesses beyond
offset 0x2000
DCAN2 RAM
PCS[14] 0xFF1C_0000 0xFF1D_FFFF 128KB 8KBAbort generated for
accesses beyond
offset 0x2000
DCAN1 RAM
PCS[15] 0xFF1E_0000 0xFF1F_FFFF 128KB 8KBAbort generated for
accesses beyond
offset 0x2000.
MIBADC2 RAM
PCS[29] 0xFF3A_0000 0xFF3B_FFFF 128KB 8KBWrap around for
accesses to
unimplemented
address offsets
lower than 0x1FFF.
MIBADC1 RAM
PCS[31] 0xFF3E_0000 0xFF3F_FFFF 128KB8KBWrap around for
accesses to
unimplemented
address offsets
lower than 0x1FFF.
MIBADC1 Look-UP Table
384bytesLook-Up Table for
ADC1 wrapper.
Starts ataddress
offset 0x2000 and
ends ataddress
offset 0x217F. Wrap
around foraccesses
between offsets
0x0180 and0x3FFF.
Abort generation for
accesses beyond
offset 0x4000.
NHET2 RAM
PCS[34] 0xFF44_0000 0xFF45_FFFF 128KB 16KBWrap around for
accesses to
unimplemented
address offsets
lower than 0x3FFF.
Abort generated for
accesses beyond
0x3FFF.
NHET1 RAM
PCS[35] 0xFF46_0000 0xFF47_FFFF 128KB 16KBWrap around for
accesses to
unimplemented
address offsets
lower than 0x3FFF.
Abort generated for
accesses beyond
0x3FFF.
HET TU2 RAM PCS[38] 0xFF4C_0000 0xFF4D_FFFF 128KB 1KB Abort
HET TU1 RAM PCS[39] 0xFF4E_0000 0xFF4F_FFFF 128KB 1KB Abort
FlexRay TURAM PCS[40] 0xFF50_0000 0xFF51_FFFF 128KB 1KB Abort

<!-- Page 94 -->
94TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-25. Module Registers /Memories Memory Map (continued)
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
CoreSight Debug Components
CoreSight Debug ROMCSCS[0] 0xFFA0_0000 0xFFA0_0FFF 4KB 4KBReads return zeros,
writes have noeffect
Cortex-R5F DebugCSCS[1] 0xFFA0_1000 0xFFA0_1FFF 4KB 4KBReads return zeros,
writes have noeffect
ETM-R5CSCS[2] 0xFFA0_2000 0xFFA0_2FFF 4KB 4KBReads return zeros,
writes have noeffect
CoreSight TPIUCSCS[3] 0xFFA0_3000 0xFFA0_3FFF 4KB 4KBReads return zeros,
writes have noeffect
POMCSCS[4] 0xFFA0_4000 0xFFA0_4FFF 4KB 4KBReads return zeros,
writes have noeffect
CTI1CSCS[7] 0xFFA0_7000 0xFFA0_7FFF 4KB 4KBReads return zeros,
writes have noeffect
CTI3CSCS[9] 0xFFA0_9000 0xFFA0_9FFF 4KB 4KBReads return zeros,
writes have noeffect
CTI4CSCS[10] 0xFFA0_A000 0xFFA0_AFFF 4KB 4KBReads return zeros,
writes have noeffect
CSTFCSCS[11] 0xFFA0_B000 0xFFA0_BFFF 4KB 4KBReads return zeros,
writes have noeffect
Registers under PCR3 (Peripheral Segment 3)
PCR3 registersPS[31:30] 0xFFF7_8000 0xFFF7_87FF 2KB 2KBReads return zeros,
writes have noeffect
FTUPS[23] 0xFFF7_A000 0xFFF7_A1FF 512B 512BReads return zeros,
writes have noeffect
HTU1 PS[22] 0xFFF7_A400 0xFFF7_A4FF 256B 256B Abort
HTU2 PS[22] 0xFFF7_A500 0xFFF7_A5FF 256B 256B Abort
NHET1PS[17] 0xFFF7_B800 0xFFF7_B8FF 256B 256BReads return zeros,
writes have noeffect
NHET2PS[17] 0xFFF7_B900 0xFFF7_B9FF 256B 256BReads return zeros,
writes have noeffect
GIOPS[16] 0xFFF7_BC00 0xFFF7_BCFF 256B 256BReads return zeros,
writes have noeffect
MIBADC1PS[15] 0xFFF7_C000 0xFFF7_C1FF 512B 512BReads return zeros,
writes have noeffect
MIBADC2PS[15] 0xFFF7_C200 0xFFF7_C3FF 512B 512BReads return zeros,
writes have noeffect
FlexRayPS[12]+PS[13] 0xFFF7_C800 0xFFF7_CFFF 2KB 2KBReads return zeros,
writes have noeffect
I2C1PS[10] 0xFFF7_D400 0xFFF7_D4FF 256B 256BReads return zeros,
writes have noeffect
I2C2PS[10] 0xFFF7_D500 0xFFF7_D5FF 256B 256BReads return zeros,
writes have noeffect
DCAN1PS[8] 0xFFF7_DC00 0xFFF7_DDFF 512B 512BReads return zeros,
writes have noeffect
DCAN2PS[8] 0xFFF7_DE00 0xFFF7_DFFF 512B 512BReads return zeros,
writes have noeffect
DCAN3PS[7] 0xFFF7_E000 0xFFF7_E1FF 512B 512BReads return zeros,
writes have noeffect
DCAN4PS[7] 0xFFF7_E200 0xFFF7_E3FF 512B 512BReads return zeros,
writes have noeffect
LIN1PS[6] 0xFFF7_E400 0xFFF7_E4FF 256B 256BReads return zeros,
writes have noeffect
SCI3PS[6] 0xFFF7_E500 0xFFF7_E5FF 256B 256BReads return zeros,
writes have noeffect
LIN2PS[6] 0xFFF7_E600 0xFFF7_E6FF 256B 256BReads return zeros,
writes have noeffect
SCI4PS[6] 0xFFF7_E700 0xFFF7_E7FF 256B 256BReads return zeros,
writes have noeffect

<!-- Page 95 -->
95TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-25. Module Registers /Memories Memory Map (continued)
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
MibSPI1PS[2] 0xFFF7_F400 0xFFF7_F5FF 512B 512BReads return zeros,
writes have noeffect
MibSPI2PS[2] 0xFFF7_F600 0xFFF7_F7FF 512B 512BReads return zeros,
writes have noeffect
MibSPI3PS[1] 0xFFF7_F800 0xFFF7_F9FF 512B 512BReads return zeros,
writes have noeffect
MibSPI4PS[1] 0xFFF7_FA00 0xFFF7_FBFF 512B 512BReads return zeros,
writes have noeffect
MibSPI5PS[0] 0xFFF7_FC00 0xFFF7_FDFF 512B 512BReads return zeros,
writes have noeffect
System Modules Control Registers andMemories under PCR1 (Peripheral Segment 1)
DMA RAM PPCS[0] 0xFFF8_0000 0xFFF8_0FFF 4KB 4KB Abort
VIM RAM
PPCS[2] 0xFFF8_2000 0xFFF8_2FFF 4KB 4KBWrap around for
accesses to
unimplemented
address offsets
lower than 0x2FFF.
RTP RAM PPCS[3] 0xFFF8_3000 0xFFF8_3FFF 4KB 4KB Abort
Flash Wrapper PPCS[7] 0xFFF8_7000 0xFFF8_7FFF 4KB 4KB Abort
eFuse Farm Controller PPCS[12] 0xFFF8_C000 0xFFF8_CFFF 4KB 4KB Abort
Power Domain Control (PMM) PPSE[0] 0xFFFF_0000 0xFFFF_01FF 512B 512B Abort
FMTM
Note: This module isonly used
byTIduring testPPSE[1] 0xFFFF_0400 0xFFFF_05FF 512B 512BReads return zeros,
writes have noeffect
STC2 (NHET1/2)PPSE[2] 0xFFFF_0800 0xFFFF_08FF 256B 256BReads return zeros,
writes have noeffect
SCM PPSE[2] 0xFFFF_0A00 0xFFFF_0AFF 256B 256B Abort
EPC PPSE[3] 0xFFFF_0C00 0xFFFF_0FFF 1KB 1KB Abort
PCR1 registersPPSE[4] -PPSE[5] 0xFFFF_1000 0xFFFF_17FF 2KB 2KBReads return zeros,
writes have noeffect
NMPU (PS_SCR_S) PPSE[6] 0xFFFF_1800 0xFFFF_19FF 512B 512B Abort
NMPU (DMA Port A) PPSE[6] 0xFFFF_1A00 0xFFFF_1BFF 512B 512B Abort
PinMux Control (IOMM)PPSE[7] 0xFFFF_1C00 0xFFFF_1FFF 2KB 1KBReads return zeros,
writes have noeffect
System Module -Frame 2(see
theTRM SPNU563 )PPS[0] 0xFFFF_E100 0xFFFF_E1FF 256B 256BReads return zeros,
writes have noeffect
PBISTPPS[1] 0xFFFF_E400 0xFFFF_E5FF 512B 512BReads return zeros,
writes have noeffect
STC1 (Cortex-R5F)PPS[1] 0xFFFF_E600 0xFFFF_E6FF 256B 256BReads return zeros,
writes have noeffect
DCC1PPS[3] 0xFFFF_EC00 0xFFFF_ECFF 256B 256BReads return zeros,
writes have noeffect
DMA PPS[4] 0xFFFF_F000 0xFFFF_F3FF 1KB 1KB Abort
DCC2PPS[5] 0xFFFF_F400 0xFFFF_F4FF 256B 256BReads return zeros,
writes have noeffect
ESM registerPPS[5] 0xFFFF_F500 0xFFFF_F5FF 256B 256BReads return zeros,
writes have noeffect
CCM-R5FPPS[5] 0xFFFF_F600 0xFFFF_F6FF 256B 256BReads return zeros,
writes have noeffect
DMMPPS[5] 0xFFFF_F700 0xFFFF_F7FF 256B 256BReads return zeros,
writes have noeffect
L2RAMW PPS[6] 0xFFFF_F900 0xFFFF_F9FF 256B 256B Abort
RTPPPS[6] 0xFFFF_FA00 0xFFFF_FAFF 256B 256BReads return zeros,
writes have noeffect
RTI+DWWDPPS[7] 0xFFFF_FC00 0xFFFF_FCFF 256B 256BReads return zeros,
writes have noeffect

<!-- Page 96 -->
96TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-25. Module Registers /Memories Memory Map (continued)
TARGET NAMEMEMORY
SELECTADDRESS RANGE
FRAME
SIZEACTUAL
SIZERESPONSE FOR
ACCESS TO
UNIMPLEMENTED
LOCATIONS IN
FRAMESTART END
VIMPPS[7] 0xFFFF_FD00 0xFFFF_FEFF 512B 512BReads return zeros,
writes have noeffect
System Module -Frame 1(see
theTRM SPNU563 )PPS[7] 0xFFFF_FF00 0xFFFF_FFFF 256B 256BReads return zeros,
writes have noeffect
6.9.3 Special Consideration forCPU Access Errors Resulting inImprecise Aborts
Any CPU write access toaNormal orDevice type memory, which generates afault, willgenerate an
imprecise abort. The imprecise abort exception isdisabled bydefault andmust beenabled fortheCPU to
handle thisexception. The imprecise abort handling isenabled byclearing the"A"bitintheCPU program
status register (CPSR).

<!-- Page 97 -->
97TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.9.4 Master/Slave Access Privileges
Table 6-26 andTable 6-27 listtheaccess permissions foreach busmaster onthedevice. Abusmaster is
amodule thatcaninitiate aread orawrite transaction onthedevice.
Each slave module oneither theCPU Interconnect Subsystem orthePeripheral Interconnect Subsystem
islisted inTable 6-27.Allowed indicates that themodule listed intheMASTERS column canaccess that
slave module.
Table 6-26. Bus Master /Slave Access Matrix forCPU Interconnect Subsystem
MASTERSSLAVES ONCPU INTERCONNECT SUBSYSTEM
L2Flash OTP, ECC,
Bank 7(EEPROM)L2FLASH L2SRAM CACHE MEMORY EMIF
CPU Read Allowed Allowed Allowed Allowed Allowed
CPU Write Notallowed Notallowed Allowed Allowed Allowed
DMA PortA Allowed Allowed Allowed Notallowed Allowed
POM Notallowed Notallowed Allowed Notallowed Allowed
PS_SCR_M Allowed Allowed Allowed Notallowed Allowed
ACP_M Notallowed NotAllowed Allowed Notallowed Notallowed
Table 6-27. Bus Master /Slave Access Matrix forPeripheral Interconnect Subsystem
MASTER IDTO
PCRxMASTERSSLAVES ONPERIPHERAL INTERCONNECT SUBSYSTEM
CRC1/CRC2Resources Under
PCR2 andPCR3Resources Under
PCR1CPU Interconnect
Subsystem SDC
MMR Port (see
Section 6.9.6 )
0CPU Read Allowed Allowed Allowed Allowed
CPU Write Allowed Allowed Allowed Allowed
1 Reserved - - - -
2 DMA PortB Allowed Allowed Allowed Notallowed
3 HTU1 Notallowed Notallowed Notallowed Notallowed
4 HTU2 Notallowed Notallowed Notallowed Notallowed
5 FTU Notallowed Notallowed Notallowed Notallowed
7 DMM Allowed Allowed Allowed Allowed
9 DAP Allowed Allowed Allowed Allowed
10 EMAC Notallowed Allowed Notallowed Notallowed
6.9.4.1 Special Notes onAccesses toCertain Slaves
Bydesign only theCPU and debugger canhave privileged write access toperipherals under thePCR1
segment. The other masters canonly read from these registers.
The master-id filtering check isimplemented inside each PCR module ofeach peripheral segment and
canbeused toblock certain masters from write accesses tocertain peripherals. Anunauthorized master
write access detected bythePCR willresult inthetransaction being discarded and anerror being
generated totheESM module.
The device contains dedicated logic togenerate abuserror response onanyaccess toamodule thatisin
apower domain thathasbeen turned off.

<!-- Page 98 -->
ID Decode Addr DecodeMasterID Address/Control
4MasterID Protection Register NPeripheral Select N0
1
2
13
14
15
PCRx
98TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.9.5 MasterID toPCRx
The MasterID associated with each master port onthePeripheral Interconnect Subsystem contains a4-bit
value. The MasterID ispassed along with theaddress and control signals tothree PCR modules. PCR
decodes theaddress andcontrol signals toselect theperipheral. Inaddition, itdecodes this4-bit MasterID
value toperform memory protection. With 4-bit ofMasterID, itallows thePCR todistinguish among 16
different masters toallow ordisallow access toagiven peripheral. Associated with each peripheral a16-
bitMasterID access protection register isdefined. Each bitgrants ordenies thepermission ofthe
corresponding binary coded decimal MasterID. Forexample, ifbit5oftheaccess permission register is
set, itgrants MasterID 5toaccess theperipheral. Ifbit7isclear, itdenies MasterID 7toaccess the
peripheral. Figure 6-10 shows theMasterID filtering scheme. Table 6-27 lists theMasterID ofeach master,
which canaccess thePCRx.
Figure 6-10. PCR MasterID Filtering
6.9.6 CPU Interconnect Subsystem SDC MMR Port
The CPU Interconnect Subsystem SDC MMR Port isaspecial slave tothePeripheral Interconnect
Subsystem. Itismemory mapped atstarting address of0xFA00_0000. Various status registers pertaining
tothediagnostics oftheCPU Interconnect Subsystem can beaccess through thisslave port. The CPU
Interconnect Subsystem contains built-in hardware diagnostic checkers which will constantly watch
transactions flowing through theinterconnect. There isachecker foreach master and slave attached to
theCPU Interconnect Subsystem. The checker checks theexpected behavior against thegenerated
behavior bytheinterconnect. For example, iftheCPU issues aburst read request totheflash, the
checker willensure thattheexpected behavior isindeed aburst read request totheproper slave module.
Iftheinterconnects generates atransaction which isnotaread, ornotaburst ornottotheflash asthe
destination, then thechecker willflagitoneoftheregisters. The detected error willalso besignaled tothe
ESM module. Refer totheInterconnect chapter oftheTRM SPNU563 fordetails ontheregisters.

<!-- Page 99 -->
99TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-28. CPU Interconnect Subsystem SDC Register BitField Mapping
Register name bit0 bit1 bit2 bit3 bit4 bit5 bit6 Remark
ERR_GENERIC_PARITY PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* Each bitindicates the
transaction processing block
inside theinterconnect
corresponding tothemaster
thatisdetected bythe
interconnect checker tohave
afault.
* error related toparity
mismatch intheincoming
address
ERR_UNEXPECTED_TRANS PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* error related tounexpected
transaction sent bythe
master
ERR_TRANS_ID PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* error related tomismatch on
thetransaction ID
ERR_TRANS_SIGNATURE PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* error related tomismatch on
thetransaction signature
ERR_TRANS_TYPE PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* error related tomismatch on
thetransaction type
ERR_USER_PARITY PS_SCR_M POM DMA_PORTA Reserved CPU AXI-M ACP-M Reserved* error related tomismatch on
theparity
SERR_UNEXPECTED_MID L2RAM WrapperL2Flash
Wrapper Port AL2Flash Wrapper
Port BEMIF Reserved CPU AXi-S ACP-S* Each bitindicates the
transaction processing block
inside theinterconnect
corresponding totheslave
thatisdetected bythe
interconnect checker tohave
afault.
* error related tomismatch on
themaster ID
SERR_ADDR_DECODE L2RAM WrapperL2Flash
Wrapper Port AL2Flash Wrapper
Port BEMIF Reserved CPU AXi-S ACP-S* error related tomismatch on
themost significant address
bits
SERR_USER_PARITY L2RAM WrapperL2Flash
Wrapper Port AL2Flash Wrapper
Port BEMIF Reserved CPU AXi-S ACP-S* error related tomismatch on
theparity ofthemost
significant address bits

<!-- Page 100 -->
100TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.9.7 Parameter Overlay Module (POM) Considerations
The Parameter Overlay Module (POM) isimplemented aspart oftheL2FMC module. Itisused toredirect
flash memory accesses toexternal memory interfaces orinternal SRAM. The POM hasanOCP master
port toredirect accesses. The POM MMRs arelocated inaseparate block and read/writes willhappen
through theDebug APB port ontheL2FMC. The POM master port iscapable ofread accesses only.
Inside theCPU Subsystem SCR, thePOM master port isconnected toboth theL2RAMW and EMIF
slaves. The primary roles ofthePOM are:
*The POM snoops theaccess onthetwoflash slave ports todetermine ifaccess should beremapped ornot. It
supports 32regions among thetwoslave ports.
*Ifaccess istoberemapped, then thePOM kills theaccess totheflash bank, and instead redirects theaccess
through itsown master.
*Upon obtaining response, thePOM populates theresponse FIFO oftherespective port sothat theresponse is
delivered back totheoriginal requester.
*The access isunaffected iftherequest isnotmapped toanyregion, orifthePOM isdisabled.
*The POM does notaddanylatency totheflash access when itisturned off.
*The POM does notaddanylatency totheremapped access (except thelatency, ifany, associated with thegetting
theresponse from theanalternate slave)