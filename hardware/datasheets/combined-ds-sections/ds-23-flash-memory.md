# Flash Memory

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 101-103 (3 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 101 -->
101TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.10 Flash Memory
6.10.1 Flash Memory Configuration
Flash Bank: Aseparate block oflogic consisting of1to16sectors. Each flash bank normally has a
customer-OTP and aTI-OTP area. These flash sectors share input/output buffers, data paths, sense
amplifiers, andcontrol logic.
Flash Sector: Acontiguous region offlash memory which must beerased simultaneously duetophysical
construction constraints.
Flash Pump: Acharge pump which generates allthevoltages required forreading, programming, or
erasing theflash banks.
Flash Module: Interface circuitry required between thehost CPU andtheflash banks andpump module.
Table 6-29. Flash Memory Banks andSectors
MEMORY ARRAYS (ORBANKS)SECTOR
NO.SEGMENT LOW ADDRESS HIGH ADDRESS
BANK0 (2.0MB)0 16KB 0x0000_0000 0x0000_3FFF
1 16KB 0x0000_4000 0x0000_7FFF
2 16KB 0x0000_8000 0x0000_BFFF
3 16KB 0x0000_C000 0x0000_FFFF
4 16KB 0x0001_0000 0x0001_3FFF
5 16KB 0x0001_4000 0x0001_7FFF
6 32KB 0x0001_8000 0x0001_FFFF
7 128KB 0x0002_0000 0x0003_FFFF
8 128KB 0x0004_0000 0x0005_FFFF
9 128KB 0x0006_0000 0x0007_FFFF
10 256KB 0x0008_0000 0x000B_FFFF
11 256KB 0x000C_0000 0x000F_FFFF
12 256KB 0x0010_0000 0x0013_FFFF
13 256KB 0x0014_0000 0x0017_FFFF
14 256KB 0x0018_0000 0x001B_FFFF
15 256KB 0x001C_0000 0x001F_FFFF
BANK1 (2.0MB)0 128KB 0x0020_0000 0x0021_FFFF
1 128KB 0x0022_0000 0x0023_FFFF
2 128KB 0x0024_0000 0x0025_FFFF
3 128KB 0x0026_0000 0x0027_FFFF
4 128KB 0x0028_0000 0x0029_FFFF
5 128KB 0x002A_0000 0x002B_FFFF
6 128KB 0x002C_0000 0x002D_FFFF
7 128KB 0x002E_0000 0x002F_FFFF
8 128KB 0x0030_0000 0x0031_FFFF
9 128KB 0x0032_0000 0x0033_FFFF
10 128KB 0x0034_0000 0x0035_FFFF
11 128KB 0x0036_0000 0x0037_FFFF
12 128KB 0x0038_0000 0x0039_FFFF
13 128KB 0x003A_0000 0x003B_FFFF
14 128KB 0x003C_0000 0x003D_FFFF
15 128KB 0x003E_0000 0x003F_FFFF

<!-- Page 102 -->
MRC p15,#0,r1,c9,c12,#0  ;Enabling Event monitor states
ORR r1, r1, #0x00000010
MCR p15,#0,r1,c9,c12,#0  ;Set 4th bit ('X') of PMNC register
MRC p15,#0,r1,c9,c12,#0
102TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-30. EEPROM Flash Bank
MEMORY ARRAYS (ORBANKS)SECTOR
NO.SEGMENT LOW ADDRESS HIGH ADDRESS
BANK7 (128KB) forEEPROM emulation0 4KB 0xF020_0000 0xF020_0FFF
"
"
""
"
""
"
""
"
"
31 4KB 0xF021_F000 0xF021_FFFF
6.10.2 Main Features ofFlash Module
*Support formultiple flash banks forprogram and/or data storage
*Simultaneous read accesses ontwobanks while performing program orerase operation onanyother bank
*Integrated state machines toautomate flash erase andprogram operations
*Software interface forflash program anderase operations
*Pipelined mode operation toimprove instruction access interface bandwidth
*Support forSingle Error Correction Double Error Detection (SECDED) block inside Cortex-R5F CPU
*Support forarichsetofdiagnostic features
6.10.3 ECC Protection forFlash Accesses
Allaccesses totheL2program flash memory areprotected bySECDED logic embedded inside theCPU.
The flash module provides 8bitsofECC code for64bitsofinstructions ordata fetched from theflash
memory. The CPU calculates theexpected ECC code based onthe64bitsdata received andcompares it
with theECC code returned bytheflash module. Asingle-bit error iscorrected and flagged bytheCPU,
while amultibit error isonly flagged. The CPU signals anECC error through itsEvent bus. This signaling
mechanism isnotenabled bydefault and must beenabled bysetting the'X'bitofthePerformance
Monitor Control Register, c9.
NOTE
ECC ispermanently enabled intheCPU L2interface.
6.10.4 Flash Access Speeds
Forinformation onflash memory access speeds andtherelevant wait states required, refer toSection 5.6.

<!-- Page 103 -->
103TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) This programming time includes overhead ofstate machine, butdoes notinclude data transfer time. The programming time assumes
programming 288bitsatatime atthemaximum specified operating frequency.6.10.5 Flash Program andErase Timings
6.10.5.1 Flash Program andErase Timings forProgram Flash
Table 6-31. Timing Requirements forProgram Flash
MIN NOM MAX UNIT
tprog(288bits) Wide Word (288-bits) programming time 40 300 µs
tprog(Total) 4.0MB programming time(1)-40°Cto125°C 21.3 s
0°Cto60°C,forfirst
25cycles5.3 10.6 s
terase Sector/Bank erase time-40°Cto125°C 0.3 4 s
0°Cto60°C,forfirst
25cycles100 ms
twecWrite/erase cycles with 15-year Data Retention
requirement-40°Cto125°C 1000 cycles
(1) This programming time includes overhead ofstate machine, butdoes notinclude data transfer time. The programming time assumes
programming 72bitsatatime atthemaximum specified operating frequency.6.10.5.2 Flash Program andErase Timings forData Flash
Table 6-32. Timing Requirements forData Flash
MIN NOM MAX UNIT
tprog(72bits) Wide Word (72-bits) programming time 47 300 µs
tprog(Total)EEPROM Emulation (bank 7)128KB
programming time(1)-40°Cto125°C 2.6 s
0°Cto60°C,forfirst
25cycles775 1320 ms
EEPROM Emulation (bank 7)Sector/Bank erase time terase(bank7)-40°Cto125°C 0.2 8 s
0°Cto60°C,forfirst
25cycles14 100 ms
twecWrite/erase cycles with 15-year Data Retention
requirement-40°Cto125°C 100000 cycles