# On-Chip SRAM, PBIST

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 104-108 (5 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 104 -->
104TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.11 L2RAMW (Level 2RAM Interface Module)
L2RAMW istheTMS570 level twoRAM wrapper. Major features implemented inthisdevice include:
*Supports 512KB ofL2SRAMs
*One 64-bit OCP interface
*Built-in ECC generation andevaluation logic
-The ECC logic isenabled bydefault.
-When enabled, automatic ECC correction onwrite data from masters onanywrite sizes (8-,16-,32-,or 64-bit)
-Less than 64-bit write forces built inread-modify-write
-When enabled, reads due toread-modify-write gothrough ECC correction before data merging with the
incoming write data
*Redundant address decoding. Same address decode logic block isduplicated andcompared toeach other
*Data Trace
-Support tracing ofboth read andwrite accesses through RTP module
*Auto initialization ofmemory banks toknown values forboth data andtheir corresponding ECC checksum
6.11.1 L2SRAM Initialization
The entire L2SRAM canbeglobally initialized bysetting thecorresponding bitinSYS.MSINENA register.
When initialized, thememory arrays arewritten with allzeros forthe64-bit data andthecorresponding 8-
bitECC checksum. Hardware memory initialization eliminates ECC error when theCPU reads from anun-
initialized memory location which cancause anECC error. Formore information, seethedevice-specific
Technical Reference Manual.
6.12 ECC /Parity Protection forAccesses toPeripheral RAMs
Accesses tosome peripheral RAMs areprotected byeither odd/even parity checking orECC checking.
During aread access theparity orECC iscalculated based onthedata read from theperipheral RAM and
compared with thegood parity orECC value stored intheperipheral RAM forthatperipheral. Ifanyword
fails theparity orECC check, themodule generates aECC/parity error signal thatismapped totheError
Signaling Module. The module also captures theperipheral RAM address thatcaused theparity error.
The parity orECC protection forperipheral RAMs isnotenabled bydefault and must beenabled bythe
application. Each individual peripheral contains control registers toenable theparity orECC protection for
accesses toitsRAM.
NOTE
Forperipherals with parity protection theCPU read access gets theactual data from the
peripheral. The application canchoose togenerate aninterrupt whenever aperipheral RAM
parity error isdetected.

<!-- Page 105 -->
105TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) March13N istheonly algorithm recommended forapplication testing ofRAM.
(2) ATB RAM ispart oftheETM module. PBIST testing ofthisRAM islimited to85ºCorlower.6.13 On-Chip SRAM Initialization andTesting
6.13.1 On-Chip SRAM Self-Test Using PBIST
6.13.1.1 Features
*Extensive instruction settosupport various memory testalgorithms
*ROM-based algorithms allow application torunTIproduction-level memory tests
*Independent testing ofallon-chip SRAM
6.13.1.2 PBIST RAM Groups
Table 6-33. PBIST RAM Grouping
MEMORYRAM
GROUPTEST CLOCK RGS RDSMEM
TYPENO.
BANKSTEST PATTERN
(ALGORITHM)
TRIPLE
READ
SLOW READTRIPLE
READ
FAST READMarch 13N(1)
TWO PORT
(cycles)March 13N(1)
SINGLE
PORT
(cycles)
ALGO MASK
0x1ALGO MASK
0x2ALGO MASK
0x4ALGO MASK
0x8
PBIST_ROM 1GCM_PBIST_R
OM1 1 ROM 1 24578 8194
STC1_1_ROM_R5 2GCM_PBIST_R
OM14 1 ROM 1 49154 16386
STC1_2_ROM_R5 3GCM_PBIST_R
OM14 2 ROM 1 49154 16386
STC2_ROM_NHET 4GCM_PBIST_R
OM15 1 ROM 1 46082 15362
AWM1 5 GCM_VCLKP 2 1 2P 1 4210
DCAN1 6 GCM_VCLKP 3 1..6 2P 2 25260
DCAN2 7 GCM_VCLKP 4 1..6 2P 2 25260
DMA 8 GCM_HCLK 5 1..6 2P 2 37740
HTU1 9 GCM_VCLK2 6 1..6 2P 2 6540
MIBSPI1 10 GCM_VCLKP 8 1..4 2P 2 66760
MIBSPI2 11 GCM_VCLKP 9 1..4 2P 2 33480
MIBSPI3 12 GCM_VCLKP 10 1..4 2P 2 33480
NHET1 13 GCM_VCLK2 11 1..12 2P 4 50520
VIM 14 GCM_VCLK 12 1..2 2P 1 16740
Reserved 15 - - - - - -
RTP 16 GCM_HCLK 16 1..12 2P 4 50520
ATB(2)17 GCM_GCLK1 17 1..16 2P 8 133920
AWM2 18 GCM_VCLKP 18 1 2P 1 4210
DCAN3 19 GCM_VCLKP 19 1..6 2P 2 25260
DCAN4 20 GCM_VCLKP 20 1..6 2P 2 25260
HTU2 21 GCM_VCLK2 21 1..6 2P 2 6540
MIBSPI4 22 GCM_VCLKP 22 1..4 2P 2 33480
MIBSPI5 23 GCM_VCLKP 23 1..4 2P 2 33480
NHET2 24 GCM_VCLK2 24 1..12 2P 4 50520
FTU 25 GCM_VCLKP 25 1 2P 1 8370
FRAY_INBUF_OUTB
UF26 GCM_VCLKP 26 1..8 2P 4 33680
CPGMAC_STATE_R
XADDR27 GCM_VCLK3 27 1..3 2P 2 6390
CPGMAC_STAT_FIF
O28 GCM_VCLK3 27 4..6 2P 3 8730

<!-- Page 106 -->
106TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-33. PBIST RAM Grouping (continued)
MEMORYRAM
GROUPTEST CLOCK RGS RDSMEM
TYPENO.
BANKSTEST PATTERN
(ALGORITHM)
TRIPLE
READ
SLOW READTRIPLE
READ
FAST READMarch 13N(1)
TWO PORT
(cycles)March 13N(1)
SINGLE
PORT
(cycles)
ALGO MASK
0x1ALGO MASK
0x2ALGO MASK
0x4ALGO MASK
0x8
L2RAMW 29 GCM_HCLK 71 SP 4
532580
6 SP 4
L2RAMW 30 GCM_HCLK 321 SP 4
15977406 SP 4
11 SP 4
16 SP 4
21 SP 4
26 SP 4
R5_ICACHE 31 GCM_GCLK1 401 SP 4
1666006 SP 4
11 SP 4
16 SP 4
R5_DCACHE 32 GCM_GCLK1 411 SP 4
2998206 SP 4
11 SP 4
16 SP 4
21 SP 4
26 SP 4
Reserved 33 GCM_GCLK2 431 SP 4
1666006 SP 4
11 SP 4
16 SP 4
Reserved 34 GCM_GCLK2 441 SP 4
2998206 SP 4
11 SP 4
16 SP 4
21 SP 4
26 SP 4
FRAY_TRBUF_MSG
RAM35 GCM_VCLKP 26 9..11 SP 3 149910
CPGMAC_CPPI 36 GCM_VCLK3 27 7 SP 1 133170
R5_DCACHE_Dirty 37 GCM_GCLK1 42 2 SP 1 16690
Reserved 38 - - - - - -
Several memory testing algorithms arestored inthePBIST ROM. However, TIonly recommends the
March13N algorithm forapplication testing ofRAM.
The PBIST ROM clock frequency islimited tothemaximum frequency of82.5 MHz.
The PBIST ROM clock isdivided down from HCLK. The divider isselected byprogramming theROM_DIV
field oftheMemory Self-Test Global Control Register (MSTGCR) ataddress 0xFFFFFF58.

<!-- Page 107 -->
107TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Ifparity protection isenabled fortheperipheral SRAM modules, then theparity bitswillalso beinitialized along with theSRAM modules.
(2) IfECC protection isenabled fortheCPU data RAM orperipheral SRAM modules, then theauto-initialization process also initializes the
corresponding ECC space.
(3) The L2SRAM from range 128KB to512KB isdivided into8memory regions. Each region hasanassociated control bittoenable auto-
initialization.
(4) The MibSPIx modules perform aninitialization ofthetransmit andreceive RAMs assoon asthemultibuffered mode isenabled. This is
independent ofwhether theapplication hasalready initialized these RAMs using theauto-initialization method ornot.The MibSPIx
modules must bereleased from reset bywriting a1totheSPIGCR0 registers before starting auto-initialization ontherespective RAMs.6.13.2 On-Chip SRAM Auto Initialization
This microcontroller allows some oftheon-chip memories tobeinitialized through theMemory Hardware
Initialization mechanism inthesystem module. This hardware mechanism allows anapplication to
program thememory arrays with error detection capability toaknown state based ontheir error detection
scheme (odd/even parity orECC).
The MINITGCR register enables thememory initialization sequence, and theMSINENA register selects
thememories thataretobeinitialized.
Formore information onthese registers, seethedevice-specific Technical Reference Manual.
The mapping ofthedifferent on-chip memories tothespecific bitsoftheMSINENA registers isprovided in
Table 6-34.
Table 6-34. Memory Initialization(1)(2)
CONNECTING MODULEADDRESS RANGE SYS.MSINENA Register
Bit#L2RAMW.MEMINT_ENA
Register Bit#(3)BASE ADDRESS ENDING ADDRESS
L2SRAM 0x08000000 0x0800FFFF 0 0
L2SRAM 0x08010000 0x0801FFFF 0 1
L2SRAM 0x08020000 0x0802FFFF 0 2
L2SRAM 0x08030000 0x0803FFFF 0 3
L2SRAM 0x08040000 0x0804FFFF 0 4
L2SRAM 0x08050000 0x0805FFFF 0 5
L2SRAM 0x08060000 0x0806FFFF 0 6
L2SRAM 0x08070000 0x0807FFFF 0 7
MIBSPI5 RAM(4)0xFF0A0000 0xFF0BFFFF 12 n/a
MIBSPI4 RAM(4)0xFF060000 0xFF07FFFF 19 n/a
MIBSPI3 RAM(4)0xFF0C0000 0xFF0DFFFF 11 n/a
MIBSPI2 RAM(4)0xFF080000 0xFF09FFFF 18 n/a
MIBSPI1 RAM(4)0xFF0E0000 0xFF0FFFFF 7 n/a
DCAN4 RAM 0xFF180000 0xFF19FFFF 20 n/a
DCAN3 RAM 0xFF1A0000 0xFF1BFFFF 10 n/a
DCAN2 RAM 0xFF1C0000 0xFF1DFFFF 6 n/a
DCAN1 RAM 0xFF1E0000 0xFF1FFFFF 5 n/a
MIBADC2 RAM 0xFF3A0000 0xFF3BFFFF 14 n/a
MIBADC1 RAM 0xFF3E0000 0xFF3FFFFF 8 n/a
NHET2 RAM 0xFF440000 0xFF45FFFF 15 n/a
NHET1 RAM 0xFF460000 0xFF47FFFF 3 n/a
HET TU2 RAM 0xFF4C0000 0xFF4DFFFF 16 n/a
HET TU1 RAM 0xFF4E0000 0xFF4FFFFF 4 n/a
DMA RAM 0xFFF80000 0xFFF80FFF 1 n/a
VIM RAM 0xFFF82000 0xFFF82FFF 2 n/a
FlexRay TURAM 0xFF500000 0xFF51FFFF 13 n/a

<!-- Page 108 -->
108TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
Peripheral memories notlisted inthetable either donotsupport auto-initialization orhave
implemented auto-initialization controlled directly bytheir respective peripherals.