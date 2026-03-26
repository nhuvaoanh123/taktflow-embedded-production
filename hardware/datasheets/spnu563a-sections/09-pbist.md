# Programmable Built-In Self-Test (PBIST)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 405-427

---


<!-- Page 405 -->

405 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) ModuleChapter 9
SPNU563A -March 2018
Programmable Built-In Self-Test (PBIST) Module
This chapter describes theprogrammable built-in self-test (PBIST) controller module used fortesting the
on-chip memories.
Topic ........................................................................................................................... Page
9.1 Overview ......................................................................................................... 406
9.2 RAM Grouping andAlgorithm ............................................................................ 407
9.3 PBIST Flow ...................................................................................................... 408
9.4 Memory Test Algorithms ontheOn-chip ROM .................................................... 411
9.5 PBIST Control Registers ................................................................................... 412
9.6 PBIST Configuration Example ............................................................................ 426

<!-- Page 406 -->

Memory
Configurations,
Algorithms,
BackgrounsHost CPU
Control Interface
System
and
Peripheral
Memories
Data LoggerPBIST
Controller
Memory
Data
PathPBIST
ROM
Overview www.ti.com
406 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.1 Overview
The PBIST (Programmable Built-In Self-Test) controller architecture provides arun-time-programmable
memory BIST engine forvarying levels ofcoverage across many embedded memory instances.
9.1.1 Features ofPBIST
*Information regarding on-chip memories, memory groupings, memory background patterns andtest
algorithms stored indedicated on-chip PBIST ROM
*Host processor interface toconfigure andstart BIST ofmemories
*Supports testing ofPBIST ROM itself aswell
*Supports testing ofeach memory atitsmaximum access speed inapplication
*Implements intelligent clock gating toconserve power
*Execution ofmicrocode from PBIST ROM supported forROM clock speeds upto100MHz
9.1.2 PBIST vs.Application Software-Based Testing
The PBIST architecture consists ofasmall coprocessor with adedicated instruction settargeted
specifically toward testing memories. This coprocessor executes testroutines stored inthePBIST ROM
andruns them onmultiple on-chip memory instances. The on-chip memory configuration information is
also stored inthePBIST ROM.
The PBIST Controller architecture offers significant advantages over tests running onthemain Cortex-
R5F processor (application software-based testing):
*Embedded CPUs have along access path tomemories outside thetightly-couple memory sub-system,
while thePBIST controller hasadedicated path tothememories specifically fortheself-test
*Embedded CPUs aredesigned fortheir targeted useandareoften noteasily programmed formemory
testalgorithms.
*The memory testalgorithm code onembedded CPUs istypically significantly larger than thatneeded
forPBIST.
*The embedded CPU issignificantly larger than thePBIST controller.
9.1.3 PBIST Block Diagram
Figure 9-1illustrates thebasic PBIST blocks anditswrapper logic forthedevice.
Figure 9-1.PBIST Block Diagram

<!-- Page 407 -->

www.ti.com RAM Grouping andAlgorithm
407 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.1.3.1 On-chip ROM
The on-chip ROM contains theinformation regarding thealgorithms andmemories tobetested.
9.1.3.2 Host Processor Interface tothePBIST Controller Registers
The Cortex-R5F CPU canselect thealgorithm andRAM groups forthememories' self-test from theon-
chip ROM based ontheapplication requirements. Once theself-test hasexecuted, theCPU canquery the
PBIST controller registers toidentify anymemories thatfailed theself-test andtothen take appropriate
next steps asrequired bytheapplication's author.
9.1.3.3 Memory Data Path
This istheread andwrite data path logic between different system andperipheral memories tightly
coupled tothePBIST memory interface. The PBIST controller executes each selected algorithm oneach
valid memory group sequentially until allthealgorithms areexecuted.
NOTE: Notallalgorithms aredesigned torunonallRAM groups. Ifanalgorithm isselected torun
onanincompatible memory, thiswillresult inafailure. Refer toTable 2-5andTable 2-6for
RAM grouping andalgorithm information.
9.2 RAM Grouping andAlgorithm
Table 2-5gives thelistofRAM groups andtheir types supported onthedevice. Table 2-6maps the
different algorithms supported inapplication mode fortheRAM groups with thebackground patterns used
fortheparticular algorithm.
NOTE: March13 isthemost recommended algorithm forthememory self-test.

<!-- Page 408 -->

Is system in Y es
Setup memories, peripheral and clock tree like
clock by writing PACT = 0x03
Select the RAM group and
algorithm using RINFO and
ALGO registers
Program OVER = 0 for self test without Override
Write ROM = 0x03 to enable the
microcode load of the algorithm
and RAM info groups from the
on Chip ROM
Write 0x14 to DLR register to
configure PBIST in ROM mode
Is (MSTDONE = 1) ?
Is FSRF0 = 1 ?
PBIST Selftest DoneEnable pbist clocks and ROM
Read RAMT reg forRead FSRD and FSRA datalog
reg. for Fail data and address valuesNo
Y es
NoY esWait for approximately N
vbus clocks.Reset the PBIST controller by
writing MSTGCR = 0x0A
Disable PBIST TestResume PBIST self test by writing
0x02 to the STR register
by writing MSTGCR = 0x05Disable pbist clocks and ROMreset = 1?
clock by writing PACT = 0or OVER = 1 for RINFO OverrideNo
RGS/RDS infoHCLK, VCLK peripheral and ROMCLK as required
for the PBIST test.
and start the TestEnable PBIST controller by
by writing MSIENA = 0x01
PBIST Flow www.ti.com
408 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.3 PBIST Flow
Figure 9-2illustrates thememory self-test flow.
Figure 9-2.PBIST Memory Self-Test Flow Diagram

<!-- Page 409 -->

www.ti.com PBIST Flow
409 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.3.1 PBIST Sequence
Before starting thePBIST sequence, youshould ensure thatboth theinstruction cache anddata cache
aredisabled. Bydefault, PBIST willtestallon-chip SRAMs including both theinstruction anddata cache
memories. After reset, cache isdisabled bydefault. Ifcache hasbeen enabled, usethefollowing code
example todisable thecache.
MRC p15, #0, R1, c1, c0, #0 ;Read System Control Register configuration data
BIC R1, R1, #0x1 <<12 ;instruction cache disable
BIC R1, R1, #0x1 <<2 ;data cache disable
DSB
MCR p15, #0, R1, c1, c0, #0 ;disabled cache RAMs ISB
1.Configure thedevice clock sources anddomains sothatthey arerunning attheir target frequencies.
2.Program theGCLK1 toPBIST ROM clock ratio byconfiguring theROM_DIV field (bits 9:8) ofthe
MSTGCR register ofthesystem module. This device supports amax PBIST ROM clock frequency of
82.5MHz.
3.Enable PBIST Controller bysetting bit1ofMSIENA register insystem module.
4.Enable thePBIST self-test bywriting avalue of0x0A tobits3:0oftheMSTGCR inthesystem module.
5.Wait forNVBUS clock cycles based ontheHCLK toPBIST ROM clock ratio:
N=16when GCLK1:PBIST ROM clock is1:1
N=32when GCLK1:PBIST ROM clock is1:2
N=64when GCLK1:PBIST ROM clock is1:4
N=64when GCLK1:PBIST ROM clock is1:8
6.Write 1htoPACT register toenable thePBIST internal clocks.
7.Program theALGO register todecide which algorithm from theinstruction ROM must beselected (the
default value ofALGO register isall1's,meaning allalgorithms areselected). Similarly, program the
RINFOL andRINFOU registers toindicate whether aparticular RAM group intheinstruction ROM
would getexecuted ornot.
NOTE: Incase ofRAM Override (Override Register (OVER) =00), theuser should make sure that
only thealgorithms thatrunonsimilar RAMs areselected. Ifasingle port algorithm is
selected inROM Algorithm Mask Register (ALGO), theRAM InfoMask Lower Register
(RINFOL) andRAM InfoMask Upper Register (RINFOU) must select only thesingle port
RAM 's.The same applies fortwoport RAM 's.Check Table 2-5forinformation onthe
memory types.
8.Program OVER =1htorunPBIST self-test without RAM override. Program OVER =0torunPBIST
self-test with RAM Override.
9.Write avalue of3htotheROM mask register should themicrocode fortheAlgorithms aswell asthe
RAM groups loaded from theon-chip PBIST ROM.
10.Write DLR (Data Logger register) with 14htoconfigure thePBIST runinROM mode andtoenable the
configuration access. This starts thememory self-tests.
11.Wait forthePBIST self-test done bypolling MSTDONE bitofMSTCGSTAT register inSystem
Module.
12.Once self-test iscompleted, check theFailStatus register FSRF0.
Incase there isafailure (FSRF0 =1h):
a.Read RAMT register thatindicates theRGS andRDS values ofthefailure RAM.
b.Read FSRC0 andFSRC1 registers thatcontain thefailure count.
c.Read FSRA0 andFSRA1 registers thatcontain theaddress offirstfailure.
d.Read FSRDL0 andFSRDL1 registers thatcontain thefailure data.
e.Write avalue of2htotheSTR register toresume thetest.
Incase there isnofailure (FSRF0 =0),thememory self-test iscompleted.
a.Disable thePBIST internal clocks bywriting a0tothePACT register.
b.Disable thePBIST self-test bywriting avalue of5htobits3:0oftheMSTGCR inthesystem
module.

<!-- Page 410 -->

PBIST Flow www.ti.com
410 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module13.Repeat steps 2through 9forsubsequent runs with different RAM group andalgorithm configurations.
14.After required Memory tests arecompleted, Resume orStart theNormal Application software.
NOTE: The contents oftheselected memory before thetestwillbecompletely lost. User software
must take care ofdata backup ifrequired. Typically thePBIST tests arecarried outatthe
beginning ofApplication software.
NOTE: Memory testfailinformation isreported interms ofRGS:RDS andnotRAM GROUP. Check
Table 2-5forinformation ontheRGS:RDS information applicable toeach memory being
tested.
Ifcache memory isselected tobepart ofthePBIST testthen thecontents willbecome incoherent with
respect tothelevel 2memory after thePBIST test. The cache willneed tobeinvalidated before cache
canbeenabled forusebytheCPU. Inaddition, ifyouareusing ECC error checking scheme inthecache,
youmust enable thisbyprogramming theCEC bitsintheAuxiliary Control Register before invalidating the
cache, toensure thatthecorrect error code bitsarecalculated when thecache isinvalidated. Formore
information ontheCEC bitsintheAuxiliary Control Register, refer totheARM ®Cortex ®-R5F Technical
Reference Manual .
Use thefollowing example code toinvalidate cache andenable cache.
MRC p15, #0, R1, c1, c0, #1 ;Read auxiliary control register
BIC R1, R1, #0x1, <<5 ;bit isdefault set todisable ECC. Clearing bit 5
MCR p15, #0, R1, c1, c0, #1 ;enable ECC, generate abort onECC errors, enable
;hardware recovery
MRC p15, #0, R1, c1, c0, #0 ;Read system control register configuration data
ORR R1, R1, #0x1 <<12 ;instruction cache enable
ORR R1, R1, #0x1 <<2 ;data cache enable
DSB
MCR p15, #0, R0, c15, c5, #0 ;invalidate entire data cache
MCR p15, #0, R0, c7, c5, #0 ;invalidate entire instruction cache
MCR p15, #0, R1, c1, c0, #0 ;enable cache RAM
ISB ;You must issue anISB instruction toflush the pipeline.
;This ensures that all subsequent instruction fetches
;see the effect ofenabling the instruction cache

<!-- Page 411 -->

www.ti.com Memory Test Algorithms ontheOn-chip ROM
411 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.4 Memory Test Algorithms ontheOn-chip ROM
This section provides abrief description forsome ofthetestalgorithms used formemory self-test.
1.March13N:
*March13N isthebaseline testalgorithm forSRAM testing. Itprovides thehighest overall coverage.
The other algorithms provide additional coverage ofotherwise missed boundary conditions ofthe
SRAM operation.
*The concept behind thegeneral march algorithm istoindicate:
-The bitcellcanbewritten andread asboth a1anda0.
-The bitsaround thebitcelldonotaffect thebitcell.
*The basic operation ofthemarch istoinitialize thearray toaknow pattern, then march adifferent
pattern through thememory.
*Type offaults detected bythisalgorithm:
-Address decoder faults
-Stuck-At faults
-Coupled faults
-State coupling faults
-Parametric faults
-Write recovery faults
-Read/write logic faults

<!-- Page 412 -->

PBIST Control Registers www.ti.com
412 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5 PBIST Control Registers
PBIST controller uses configuration registers forprogramming thealgorithm anditsexecution. Allthe
configuration registers arememory mapped foraccess bytheCPU through thePeripheral Bus interface.
The base address forthecontrol registers isFFFF E400h.
NOTE: There isnowatchdog functionality implemented inthePBIST controller. Ifabadcode is
executed, thePBIST runs forever. The PBIST controller does notguard against this
situation.
Registers areaccessible only when theclock tothePBIST controller isactive. The clock is
activated byfirstwriting 1htothePACT register.
Table 9-1.PBIST Registers
Offset Acronym Register Description Section
160h RAMT RAM Configuration Register Section 9.5.1
164h DLR Datalogger Register Section 9.5.2
180h PACT PBIST Activate/Clock Enable Register Section 9.5.3
184h PBISTID PBIST IDRegister Section 9.5.4
188h OVER Override Register Section 9.5.5
190h FSRF0 FailStatus FailRegister 0 Section 9.5.6
198h FSRC0 FailStatus Count Register 0 Section 9.5.7
19Ch FSRC1 FailStatus Count Register 1 Section 9.5.7
1A0h FSRA0 FailStatus Address Register 0 Section 9.5.8
1A4h FSRA1 FailStatus Address Register 1 Section 9.5.8
1A8h FSRDL0 FailStatus Data Register 0 Section 9.5.9
1B0h FSRDL1 FailStatus Data Register 1 Section 9.5.9
1C0h ROM ROM Mask Register Section 9.5.10
1C4h ALGO ROM Algorithm Mask Register Section 9.5.11
1C8h RINFOL RAM InfoMask Lower Register Section 9.5.12
1CCh RINFOU RAM InfoMask Upper Register Section 9.5.13

<!-- Page 413 -->

www.ti.com PBIST Control Registers
413 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.1 RAM Configuration Register (RAMT)
This register isdivided intofollowing internal registers, none ofwhich have adefault value after reset.
Figure 9-3andTable 9-2illustrate thisregister.
This register provides theinformation regarding thememory being currently tested. Incase ofaPBIST
failure, theapplication canread thisregister toidentify theRGS:RDS values forthememory thatfailed the
self-test.
Figure 9-3.RAM Configuration Register (RAMT) [offset =0160h]
31 24 23 16
RGS RDS
R/W-X R/W-X
15 8 7 6 5 2 1 0
DWR SMS PLS RLS
R/W-X R/W-X R/W-X R/W-X
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-2.RAM Configuration Register (RAMT) Field Descriptions
Bit Field Description
31-24 RGS Ram Group Select. Refer toTable 2-5forinformation ontheRGS value foreach memory.
23-16 RDS Return Data Select. Refer toTable 2-5forinformation ontheRDS values foreach memory.
Note: Inthecurrent version ofthePBIST, only 5bitsareused forRDS.
15-8 DWR Data Width Register
7-6 SMS Sense Margin Select Register
5-2 PLS Pipeline Latency Select
1-0 RLS RAM Latency Select

<!-- Page 414 -->

PBIST Control Registers www.ti.com
414 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.2 Datalogger Register (DLR)
This register puts thePBIST controller intotheappropriate comparison modes fordata logging. Figure 9-4
andTable 9-3illustrate thisregister.
Figure 9-4.Datalogger Register (DLR) [offset =0164h]
31 16
Reserved
R-0
15 5 4 3 2 1 0
Reserved DLR4 Rsvd DLR2 Reserved
R-0 R/W-0 R/W-1 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-3.Datalogger Register (DLR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Donotchange these bitsfrom their default value.
4 DLR4 Config access: setting thisbitallows thehost processor toconfigure thePBIST controller registers.
3 Reserved 1 Donotchange thisbitfrom itsdefault value of1.
2 DLR2 ROM-based testing: setting thisbitenables thePBIST controller toexecute testalgorithms thatare
stored inthePBIST ROM.
1-0 Reserved 00 Donotchange these bitsfrom their default value of00.
*DLR2: ROM-based testing mode
Writing a1tothisregister starts theROM-based testing. This register isused toinitiate ROM-based
testing from Config andATE interfaces. Also, since a1inthisbitposition means theinstruction ROM is
used formemory testing, alltheintermediate interrupts andPBIST done signal after each memory testare
masked until alltheselected algorithms intheROM areexecuted forallRAM groups. However, afailure
would stop thetestandreport thestatus immediately.
*DLR4: Config access mode
This mode, when set,indicates theCPU isbeing used toaccess PBIST.

<!-- Page 415 -->

www.ti.com PBIST Control Registers
415 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.3 PBIST Activate/Clock Enable Register (PACT)
This isthefirstregister thatneeds tobeprogrammed toactivate thePBIST controller. Bit[0]isused for
static clock gating, andunless a1iswritten tothisbit,alltheinternal PBIST clocks areshut off.Figure 9-5
andTable 9-4illustrate thisregister.
NOTE: This register must beprogrammed to1hduring application self-test.
Figure 9-5.PBIST Activate/ROM Clock Enable Register (PACT) [offset =0180h]
31 16
Reserved
R-0
15 1 0
Reserved PACT0
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-4.PBIST Activate/ROM Clock Enable Register (PACT) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PACT0 PBIST internal clocks enable.
0 Disable PBIST internal clocks.
1 Enable PBIST internal clocks.
*PACT0
This bitmust besetto1toturn onthePBIST internal clocks. Setting thisbitasserts aninternal signal that
isused astheclock gate enable. Aslong asthisbitis0,anyaccess tothePBIST willnotgothrough and
thePBIST willremain inanalmost zero-power mode.

<!-- Page 416 -->

PBIST Control Registers www.ti.com
416 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.4 PBIST IDRegister
Functionality oftheregister isdescribed inFigure 9-6andTable 9-5.
Figure 9-6.PBIST IDRegister [offset =184h]
31 16
Reserved
R-0
15 8 7 0
Reserved PBIST ID
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-5. PBIST IDRegister Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 PBIST ID This isaunique IDassigned toeach PBIST controller inadevice with multiple PBIST controllers.

<!-- Page 417 -->

www.ti.com PBIST Control Registers
417 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.5 Override Register (OVER)
Functionality oftheregister isdescribed inFigure 9-7andTable 9-6.
Figure 9-7.Override Register (OVER) [offset =0188h]
31 16
Reserved
R-0
15 3 2 1 0
Reserved Reserved OVER0
R-0 R-0 R/W-1
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-6.Override Register (OVER) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-1 Reserved 0 Reserved. This bitmust notbechanged from itsdefault value of0.
0 OVER0 RINFO Override Bit
0 The RAM inforegisters RINFOL andRINFOU areused toselect thememories fortest.
1 The memory information available from ROM willoverride theRAM selection from theRAM info
registers RINFOL andRINFOU.
*OVER0
While doing ROM-based testing, each algorithm downloaded from theROM hasamemory mask
associated with itthatdefines theapplicable memory groups thealgorithm willberunon.Bydefault, this
bitissetto1,which means thememory mask thatisdownloaded from theROM willoverwrite theRAM
inforegisters. The override bitcanbereset bywriting a0toit.Inthiscase, theapplication canselect the
RAM groups tobetested byconfiguring theRAM inforegisters.
NOTE: When thisoverride bit=0,each algorithm selected inALGO register willrunoneach RAM
selected inRINFOL andRINFOU register. Itmust beensured that:
1.Only thesame type ofmemories (single port ortwoport) areselected, and
2.Only memories thatarevalid forallalgorithms enabled viatheALGO register
areselected.
Iftheabove tworequirements arenotmet, thememory self-test willfail.

<!-- Page 418 -->

PBIST Control Registers www.ti.com
418 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.6 FailStatus FailRegister (FSRF0)
This register indicates ifafailure occurred during amemory self-test. Bit[0]gets setwhenever afailure
occurs. Figure 9-8andTable 9-7illustrate theFSRF0 register.
Figure 9-8.FailStatus FailRegister 0(FSRF0) [offset =0190h]
31 16
Reserved
R-0
15 1 0
Reserved FSRF0
R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-7.FailStatus FailRegister 0(FSRF0) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 FSRF0 FailStatus 0.This bitwould becleared byreset ofthemodule using MSTGCR register insystem
module.
0 Nofailure occurred.
1 Failure occurred onport 0.

<!-- Page 419 -->

www.ti.com PBIST Control Registers
419 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.7 FailStatus Count Registers (FSRC0 andFSRC1)
These registers keep count ofthenumber offailures observed during thememory self-test. The PBIST
controller stops executing thememory self-test whenever afailure occurs inanymemory instance forany
ofthetestalgorithms. The value inFSRC0 /FSRC1 gets incremented byonewhenever afailure occurs
andgets decremented byonewhen thefailure isprocessed. FSRC0 isforPort 0andFSRC1 isforPort 1.
Figure 9-9andTable 9-8illustrate theFSRC0 register, while Figure 9-10 andTable 9-9illustrate the
FSRC1 register.
Figure 9-9.FailStatus Count 0Register (FSRC0) [offset =0198h]
31 16
Reserved
R-0
15 8 7 0
Reserved FSRC0
R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-8.FailStatus Count 0Register (FSRC0) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 FSRC0 FailStatus Count 0.Indicates thenumber offailures onport 0.
Figure 9-10. FailStatus Count Register 1(FSRC1) [offset =019Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved FSRC1
R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-9.FailStatus Count Register 1(FSRC1) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 FSRC1 FailStatus Count 1.Indicates thenumber offailures onport 1.

<!-- Page 420 -->

PBIST Control Registers www.ti.com
420 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.8 FailStatus Address Registers (FSRA0 andFSRA1)
These registers capture thememory address ofthefirstfailure onport 0andport 1,respectively. Figure 9-
11andTable 9-10 illustrate theFSRA0 register, while Figure 9-12 andTable 9-11 illustrate theFSRA1
register.
Figure 9-11. FailStatus Address Register 0(FSRA0) [offset =01A0h]
31 16
Reserved
R-0
15 0
FSRA0
R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-10. FailStatus Address Register 0(FSRA0) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 FSRA0 FailStatus Address 0.Contains theaddress ofthefirstfailure.
Figure 9-12. FailStatus Address Register 1(FSRA1) [offset =01A4h]
31 16
Reserved
R-0
15 0
FSRA1
R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-11. FailStatus Address Register 1(FSRA1) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 FSRA1 FailStatus Address 1.Contains theaddress ofthefirstfailure.

<!-- Page 421 -->

www.ti.com PBIST Control Registers
421 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.9 FailStatus Data Registers (FSRDL0 andFSRDL1)
These registers areused tocapture thefailure data incase ofamemory self-test failure. FSRDL0
corresponds toPort 0,while FSRDL1 corresponds toPort 1.Figure 9-13 andTable 9-12 illustrate the
FSRDL0 register, while Figure 9-14 andTable 9-13 illustrate theFSRDL1 register.
Figure 9-13. FailStatus Data Register 0(FSRDL0) [offset =01A8h]
31 16
FSRDL0
R-AAAAh
15 0
FSRDL0
R-AAAAh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-12. FailStatus Data Register 0(FSRDL0) Field Descriptions
Bit Field Description
31-0 FSRDL0 Failure data onport 0.
Figure 9-14. FailStatus Data Register 1(FSRDL1) [offset =01B0h]
31 16
FSRDL1
R-AAAAh
15 0
FSRDL1
R-AAAAh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-13. FailStatus Data Register 1(FSRDL1) Field Descriptions
Bit Field Description
31-0 FSRDL1 Failure data onport 1.

<!-- Page 422 -->

PBIST Control Registers www.ti.com
422 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.10 ROM Mask Register (ROM)
This two-bit register sets appropriate ROM access modes forthePBIST controller. The default value is
11b. This register isillustrated inFigure 9-15.Itcanbeprogrammed according toTable 9-14.
Figure 9-15. ROM Mask Register (ROM) [offset =01C0h]
31 16
Reserved
R-0
15 2 1 0
Reserved ROM
R-0 R/W-3h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-14. ROM Mask Register (ROM) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 ROM ROM Mask
0 Noinformation isused from ROM.
1h Only RAM Group information from ROM.
2h Only Algorithm information from ROM.
3h Both Algorithm andRAM Group information from ROM. This option should beselected forapplication
self-test.

<!-- Page 423 -->

www.ti.com PBIST Control Registers
423 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.11 ROM Algorithm Mask Register (ALGO)
This register isused toindicate thealgorithm(s) tobeused forthememory self-test routine. Each bit
corresponds toaspecific algorithm. Forexample, bit[0]controls whether algorithm 1isenabled ornot.
Figure 9-16 andTable 9-15 illustrate thisregister.
Figure 9-16. ROM Algorithm Mask Register (ALGO) [offset =01C4h]
31 24 23 16
ALGO3 ALGO2
R/W-FFh R/W-FFh
15 8 7 0
ALGO1 ALGO0
R/W-FFh R/W-FFh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-15. Algorithm Mask Register (ALGO) Field Descriptions
Bit Field Value Description
31 0 Algorithm 32isnotselected.
1 Selects algorithm 32forPBIST run.
30 0 Algorithm 31isnotselected.
1 Selects algorithm 31forPBIST run.
: :
0 0 Algorithm 1isnotselected.
1 Selects algorithm 1forPBIST run.
31-0 0 None ofthealgorithms areselected.
NOTE: Please refer toTable 2-6foravailable algorithms andthememories onwhich each algorithm
canberun.

<!-- Page 424 -->

PBIST Control Registers www.ti.com
424 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.12 RAM Info Mask Lower Register (RINFOL)
This register isused toselect theRAM groups 1to32torunthealgorithms selected intheALGO register.
Foranalgorithm tobeexecuted onaparticular RAM group, thecorresponding bitinthisregister must be
setto1.The default value ofthisregister isall1s,which means alltheRAM Groups areselected.
Figure 9-17 andTable 9-16 illustrate thisregister.
The information from thisregister isused only when bit0inOVER register isnotset.
Figure 9-17. RAM InfoMask Lower Register (RINFOL) [offset =01C8h]
31 24 23 16
RINFOL3 RINFOL2
R/W-FFh R/W-FFh
15 8 7 0
RINFOL1 RINFOL0
R/W-FFh R/W-FFh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-16. RAM InfoMask Lower Register (RINFOL) Field Descriptions
Bit Field Value Description
31 0 RAM Group 32isnotselected.
1 Selects group 32forPBIST run.
30 0 RAM Group 31isnotselected.
1 Selects RAM group 31forPBIST run.
: :
0 0 RAM Group 1isnotselected.
1 Selects RAM Group 1forPBIST run.
31-0 0 None oftheRAM Groups 1to32areselected.
NOTE: Please refer toTable 2-5forRAM infogroups.

<!-- Page 425 -->

www.ti.com PBIST Control Registers
425 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.5.13 RAM Info Mask Upper Register (RINFOU)
This register isused toselect theRAM groups 33to64torunthealgorithms selected intheALGO
register. Foranalgorithm tobeexecuted onaparticular RAM group, thecorresponding bitinthisregister
should besetto1.The default value ofthisregister isall1s,which means alltheRAM InfoGroups would
beselected. Figure 9-18 andTable 9-17 illustrate thisregister.
Figure 9-18. RAM InfoMask Upper Register (RINFOU) [offset =01CCh]
31 24 23 16
RINFOU3 RINFOU2
R/W-FFh R/W-FFh
15 8 7 0
RINFOU1 RINFOU0
R/W-FFh R/W-FFh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 9-17. RAM InfoMask Upper Register (RINFOU) Field Descriptions
Bit Field Value Description
31 0 RAM Group 64isnotselected.
1 Selects group 64forPBIST run.
30 0 RAM Group 63isnotselected.
1 Selects RAM group 63forPBIST run.
: :
0 0 RAM Group 33isnotselected.
1 Selects RAM Group 33forPBIST run.
31-0 0 None ofRAM Groups 33to64areselected.

<!-- Page 426 -->

PBIST Configuration Example www.ti.com
426 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.6 PBIST Configuration Example
The following examples assume thatthePLL islocked andselected asclock source with
GCLK1 =300MHz andVCLK =75MHz.
9.6.1 Example 1:Configuration ofPBIST Controller toRun Self-Test onDCAN1 RAM
This example explains theconfigurations forrunning March13 algorithm onDCAN1.
1.Program theGCLK1 toPBIST ROM clock ratio to1:4inSystem Module.
MSTGCR[9:8] =2
2.Enable PBIST Controller inSystem Module.
MSIENA[31:0] =0x00000001
3.Enable thePBIST self-test inSystem Module.
MSTGCR[3:0] =0xA
4.Wait foratleast 64VCLK cycles inasoftware loop.
5.Enable thePBIST internal clocks.
PACT =0x1
6.Disable RAM Override. This willmake thePBIST controller usetheinformation provided bythe
application intheRINFOx andALGO registers forthememory self-test.
OVER =0x0
7.Select theAlgorithm (refer toTable 2-6).
ALGO =0x00000004 (Algo 3=March13N fortwo-port DCAN1 RAM)
8.Program theRAM group Infotoselect DCAN1 (DCAN1 RAM isGroup 3,refer toTable 2-5).
RINFOL =0x00000004 (select RAM Group 3)
RINFOU =0x00000000 (since wearetesting only DCAN1)
9.Select both Algorithm andRAM information from on-chip PBIST ROM.
ROM =0x3
10.Configure PBIST toruninROM Mode andstart PBIST run.
DLR =0x14
11. Wait forPBIST testtocomplete bypolling MSTDONE bitinSystem Module.
while (MSTDONE !=1)
12. Once self-test iscompleted, check theFailStatus register FSRF0.
Incase there isafailure (FSRF0 =1):
a.Read RAMT register thatindicates theRGS andRDS values ofthefailure RAM.
b.Read FSRC0 andFSRC1 registers thatcontain thefailure count.
c.Read FSRA0 andFSRA1 registers thatcontain theaddress offirstfailure.
d.Read FSRDL0 andFSRDL1 registers thatcontain thefailure data.
e.Resume theTest ifrequired using Program Control register (offset =0x16C) STR =2.
Incase there isnofailure (FSRF0 =0),thememory self-test iscompleted.
a.Disable thePBIST internal clocks.
PACT =0
b.Disable thePBIST self-test.
MSTGCR[3:0] =0x5

<!-- Page 427 -->

www.ti.com PBIST Configuration Example
427 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedProgrammable Built-In Self-Test (PBIST) Module9.6.2 Example 2:Configuration ofPBIST Controller toRun Self-Test onALL RAM Groups
This example explains theconfigurations forrunning March13 algorithm onallRAM groups defined inthe
PBIST ROM.
1.Program theGCLK1 toPBIST ROM clock ratio to1:4inSystem Module.
MSTGCR[9:8] =2
2.Enable PBIST Controller inSystem Module.
MSIENA[31:0] =0x00000001
3.Enable thePBIST self-test inSystem Module.
MSTGCR[3:0] =0xA
4.Wait foratleast 64VCLK cycles inasoftware loop.
5.Enable thePBIST internal clocks.
PACT =0x1
6.Enable RAM Override.
OVER =0x1
7.Select theAlgorithms toberun(refer toTable 2-6).
ALGO =0x0000000C (select March13N forsingle-port andtwo-port RAMs)
8.Select both Algorithm andRAM information from on-chip PBIST ROM.
ROM =0x3
9.Configure PBIST toruninROM Mode andkickoff PBIST test.
DLR =0x14
10. Wait forPBIST testtocomplete bypolling MSTDONE bitinSystem Module.
while (MSTDONE !=1)
11. Once self-test iscompleted, check theFailStatus register FSRF0.
Incase there isafailure (FSRF0 =1):
a.Read RAMT register thatindicates theRGS andRDS values ofthefailure RAM.
b.Read FSRC0 andFSRC1 registers thatcontain thefailure count.
c.Read FSRA0 andFSRA1 registers thatcontain theaddress offirstfailure.
d.Read FSRDL0 andFSRDL1 registers thatcontain thefailure data.
e.Resume theTest ifrequired using Program Control register (offset =0x16C) STR =2.
Incase there isnofailure (FSRF0 =0),theMemory self-test iscompleted.
a.Disable thePBIST internal clocks.
PACT =0
b.Disable thePBIST self-test.
MSTGCR[3:0] =0x5