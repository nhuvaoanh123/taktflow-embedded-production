# Device and Documentation Support

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 212-222 (11 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 212 -->
212TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9Device andDocumentation Support
9.1 Device Support
9.1.1 Development Support
Texas Instruments (TI)offers anextensive lineofdevelopment tools fortheHercules ™Safety generation
ofMCUs, including tools toevaluate theperformance oftheprocessors, generate code, develop algorithm
implementations, andfully integrate anddebug software andhardware modules.
The following products support development ofHercules ™-based applications:
Software Development Tools
*Code Composer Studio ™Integrated Development Environment (IDE)
-C/C++ Compiler
-Code generation tools
-Assembler/Linker
-Cycle Accurate Simulator
*Application algorithms
*Sample applications code
Hardware Development Tools
*Development andevaluation boards
*JTAG-based emulators -XDS100 v2,XDS200, XDS560 ™v2emulator
*Flash programming tools
*Power supply
*Documentation andcables
9.1.2 Device andDevelopment-Support Tool Nomenclature
Todesignate thestages intheproduct development cycle, TIassigns prefixes tothepart numbers ofall
devices. Each commercial family member hasone ofthree prefixes: TMX, TMP, orTMS. These prefixes
represent evolutionary stages ofproduct development from engineering prototypes (TMX) through fully
qualified production devices (TMS).
Device development evolutionary flow:
TMX Experimental device that isnotnecessarily representative ofthefinal device's electrical
specifications.
TMP Final silicon diethat conforms tothedevice's electrical specifications buthasnotcompleted
quality andreliability verification.
TMS Fully-qualified production device.
TMX andTMP devices areshipped against thefollowing disclaimer:
"Developmental product isintended forinternal evaluation purposes."
TMS devices have been characterized fully, and thequality and reliability ofthedevice have been
demonstrated fully. TI'sstandard warranty applies.
Predictions show that prototype devices (TMX orTMP) have agreater failure rate than thestandard
production devices. Texas Instruments recommends that these devices notbeused inany production
system because their expected end-use failure rate stillisundefined. Only qualified production devices are
tobeused.

<!-- Page 213 -->
Full Part # TMS 570 LC 43 5 7 B ZWT Q Q1 R
Orderable Part # TMX 570 43 5 7 B ZWT Q Q1 R
Prefix: TM
TMS = Fully Qualified
TMP = Prototype
TMX = Samples
Core Technology:
570 = Cortex R5F
Architecture:
LC = Dual  CPUs in Lockstep with caches
(not included in orderable part #)
Flash Memory Size:
43 = 4MB
RAM Size:
5 = 512KB
Peripheral Set:
7 = FlexRay, Ethernet
Die Revision:
A = 1st Die Revision
B = 2nd Die Revision
Package Type:
ZWT = 337 BGA Package
Temperature Range:
Q = -40 C to 125 Co o
Quality Designator:
Q1 = Automotive
Shipping Options:
R = Tape and Reel
213TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 9-1shows thenumbering andsymbol nomenclature fortheTMS570LC4357.
Figure 9-1.TMS570LC4357 Device Numbering Conventions

<!-- Page 214 -->
214TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.2 Documentation Support
9.2.1 Related Documentation from Texas Instruments
The following documents describe theTMS570LC4357 microcontroller..
SPNU563 TMS570LC43x 16/32-Bit RISC Flash Microcontroller Technical Reference Manual
details theintegration, theenvironment, thefunctional description, and theprogramming
models foreach peripheral andsubsystem inthedevice.
SPNZ180 TMS570LC4357 Microcontroller, Silicon Revision A,Silicon Errata describes theusage
notes andknown exceptions tothefunctional specifications forthedevice silicon revision(s).
SPNZ232 TMS570LC4x Microcontroller, Silicon Revision B,Silicon Errata describes theusage
notes andknown exceptions tothefunctional specifications forthedevice silicon revision(s).
9.2.2 Receiving Notification ofDocumentation Updates
Toreceive notification ofdocumentation updates, navigate tothedevice product folder onti.com .Inthe
upper right corner, click onAlert metoregister andreceive aweekly digest ofanyproduct information that
haschanged. Forchange details, review therevision history included inanyrevised document.
9.2.3 Community Resources
The following links connect toTIcommunity resources. Linked contents areprovided "AS IS"bythe
respective contributors. They donotconstitute TIspecifications and donotnecessarily reflect TI'sviews;
seeTI'sTerms ofUse.
TIE2E™Online Community TI's Engineer-to-Engineer (E2E) Community. Created tofoster
collaboration among engineers. Ate2e.ti.com, you can ask questions, share knowledge,
explore ideas andhelp solve problems with fellow engineers.
TIEmbedded Processors Wiki Texas Instruments Embedded Processors Wiki. Established tohelp
developers getstarted with Embedded Processors from Texas Instruments and tofoster
innovation and growth ofgeneral knowledge about thehardware and software surrounding
these devices.
9.3 Trademarks
Hercules, Code Composer Studio, XDS560, E2E aretrademarks ofTexas Instruments.
ETM isatrademark ofARM Limited.
ARM, Cortex areregistered trademarks ofARM Limited (oritssubsidiaries) intheEUand/or elsewhere.
Allrights reserved.
CoreSight isatrademark ofARM Limited (oritssubsidiaries) intheEUand/or elsewhere. Allrights
reserved..
Allother trademarks aretheproperty oftheir respective owners.
9.4 Electrostatic Discharge Caution
This integrated circuit can bedamaged byESD. Texas Instruments recommends that allintegrated circuits behandled with
appropriate precautions. Failure toobserve proper handling andinstallation procedures cancause damage.
ESD damage canrange from subtle performance degradation tocomplete device failure. Precision integrated circuits may bemore
susceptible todamage because very small parametric changes could cause thedevice nottomeet itspublished specifications.
9.5 Glossary
TIGlossary This glossary lists andexplains terms, acronyms, anddefinitions.

<!-- Page 215 -->
215TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.6 Device Identification
9.6.1 Device Identification Code Register
The device identification code register ismemory mapped toaddress FFFF FFF0h and identifies several
aspects ofthedevice including thesilicon version. The details ofthedevice identification code register are
provided inTable 9-1.The device identification code register value forthisdevice is:
*Rev A=0x8044AD05
*Rev B=0x8044AD0D
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
CP-15 UNIQUE ID TECH
R-1 R-00000000100010 R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
TECHI/O
VOLTAGEPERIPH
PARITYFLASH ECCRAM
ECCVERSION 1 0 1
R-101 R-0 R-1 R-10 R-1 R-00000 R-1 R-0 R-1
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 9-2.Device IDBitAllocation Register
Table 9-1.Device IDBitAllocation Register Field Descriptions
Bit Field Value Description
31 CP15 Indicates thepresence ofcoprocessor 15
1 CP15 present
30-17 UNIQUE ID 100011Silicon version (revision) bits.
This bitfield holds aunique number foradedicated device configuration (die).
16-13 TECH Process technology onwhich thedevice ismanufactured.
0101 F021
12 I/OVOLTAGE I/Ovoltage ofthedevice.
0 I/Oare3.3v
11 PERIPHERAL
PARITYPeripheral Parity
1 Parity onperipheral memories
10-9 FLASH ECC Flash ECC
10 Program memory with ECC
8 RAM ECC Indicates ifRAM ECC ispresent.
1 ECC implemented
7-3 REVISION Revision oftheDevice.
2-0 101 The platform family IDisalways 0b101

<!-- Page 216 -->
216TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.6.2 DieIdentification Registers
The two dieIDregisters ataddresses 0xFFFFFF7C and 0xFFFFFF80 form a64-bit dieidwith the
information aslisted inTable 9-2.
Table 9-2.Die-ID Registers
Item #ofBits BitLocation
XCoord. onWafer 12 0xFFFFFF7C[11:0]
YCoord. onWafer 12 0xFFFFFF7C[23:12]
Wafer # 8 0xFFFFFF7C[31:24]
Lot# 24 0xFFFFFF80[23:0]
Reserved 8 0xFFFFFF80[31:24]

<!-- Page 217 -->
217TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7 Module Certifications
The following communications modules have received certification ofadherence toastandard.

<!-- Page 218 -->
218TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7.1 FlexRay Certifications
Figure 9-3.FlexRay Certification forZWT Package

<!-- Page 219 -->
219TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7.2 DCAN Certification
Figure 9-4.DCAN Certification

<!-- Page 220 -->
220TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7.3 LINCertification
9.7.3.1 LINMaster Mode
Figure 9-5.LINCertification -Master Mode

<!-- Page 221 -->
221TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7.3.2 LINSlave Mode -Fixed Baud Rate
Figure 9-6.LINCertification -Slave Mode -Fixed Baud Rate

<!-- Page 222 -->
222TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device andDocumentation Support Copyright ©2014 -2016, Texas Instruments Incorporated9.7.3.3 LINSlave Mode -Adaptive Baud Rate
Figure 9-7.LINCertification -Slave Mode -Adaptive Baud Rate