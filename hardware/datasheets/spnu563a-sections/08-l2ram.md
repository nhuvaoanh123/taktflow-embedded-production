# Level 2 RAM (L2RAMW) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 387-404

---


<!-- Page 387 -->

387 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) ModuleChapter 8
SPNU563A -March 2018
Level 2RAM (L2RAMW) Module
This chapter describes theLevel IIRAM (L2RAM) module.
Topic ........................................................................................................................... Page
8.1 Overview ......................................................................................................... 388
8.2 Module Operation ............................................................................................. 388
8.3 Control andStatus Registers ............................................................................. 393

<!-- Page 388 -->

Overview www.ti.com
388 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.1 Overview
The Level 2RAM (L2RAM) module controls anddecodes RAM memory accesses onthisdevice.
Features oftheL2RAM are:
*Controls read/write accesses tothedata RAM
*Decodes addresses within thememory region allocated fortheRAM
*Performs ECC check onallincoming CPU writes toensure thatdata isintact
*Supports read andwrite accesses in64-bit, 32-bit, 16-bit, or8-bit access sizes
-Performs redundant ECC check onmerged data during read-modify-write operations
-Does notsupport bit-wise operations
*Safety Features:
-Single-Error-Correction Double-Error-Detection (SECDED) ondata
*Uses theCPU's Event busandmaintains theSECDED status inmemory-mapped registers
*Captures thenumber ofoccurrences ofsingle-bit ormulti-bit errors aswell astheRAM address
thathasthefault
*Generates error signals forsingle-bit andmulti-bit errors totheError Signaling Module (ESM)
-Performs Memory Scrubbing toIdentify andCorrect Single BitRAM errors intheL2RAM memory
-SECDED Malfunction Checking toVerify thatL2RAMW ECC isfunctioning correctly
-Parity Protection oftheAddress Bus andControl Signals
*Generates error signals forparity error totheError Signaling Module (ESM)
-Redundant Address Decode Scheme
*Checks thedecoding ofCPU address lines andgeneration ofcorrect memory selects forthe
RAM banks
*Exclusive access support
*Supports auto-initialization oftheCPU data RAM banks
*Supports theRAM Trace Port (RTP) Interface
-Traces outallRAM read andwrite accesses totheRTP module
8.2 Module Operation
8.2.1 RAM Memory Map
The L2RAMW decodes 8MB ofdata space. Upto512kB ofimplemented data space issupported. Check
thespecific part's datasheet toidentify theactual amount ofRAM supported onthedevice. This RAM is
protected byECC, allowing theCPU tocorrect anysingle-bit errors anddetect multi-bit errors within a64-
bitvalue. The error correction code (ECC) values arestored intheRAM memory space aswell. The
memory map fortheRAM andthecorresponding ECC space isshown inFigure 8-1.Any access toan
unimplemented RAM location results inanerror response from theL2RAMW module.
Each RAM data word is64-bits wide. These 64bitsaredivided into32bitsperRAM bank. The 8bitsof
ECC arealso divided into4bitsperRAM bank.
Forevery 64-bit read from theRAM, an8-bit ECC isalso read bytheCPU onitsECC bus. Similarly, for
every 64-bit write totheRAM, theCPU also writes an8-bit ECC using thesame ECC bus.
The ECC memory canalso bedirectly accessed viamemory-mapped offset addresses. Aread from the
ECC space results inthe8-bit ECC value appearing oneach byte ofthe64-bit CPU data. Writes toECC
memory must be64-bit aligned. Writes totheECC space must also firstbeenabled viatheRAM Control
Register (RAMCTRL).
Accesses totheECC space arenottraced outtotheRAM Trace Port (RTP).

<!-- Page 389 -->

Implemented data spaceImplemented ECC space
Illegal address8 MB
4MB4MB + 512KB
512KB
0x0Illegal address
www.ti.com Module Operation
389 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) ModuleNOTE: NoECC Error Generated forAccesses toECC Memory: Aread from theECC memory
itself would generate anECC value onboth theread data busaswell asthe8-bit ECC bus.
This could result inthedetection ofamulti-bit error bytheSECDED logic inside theCPU.
The L2RAMW interface module ignores theECC errors thatareindicated bytheCPU when
accessing ECC space.
Figure 8-1.RAM Memory Map
8.2.2 Safety Features
The L2RAMW module incorporates some features thataredesigned specifically with safety
considerations.
8.2.2.1 ECC Handling on8-,16-,and32-Bit Writes
ECC calculation ishandled bytheR5F CPU except inthecase ofsub-64bit writes. Ifan8-,16-, ora32-
bitwrite isperformed, L2RAMW handles theECC calculation along with read-modify-write operation. This
istominimize thelatency between CPU andL2RAMW inthecase ofsub-64bit write.
When asub-64 bitwrite isperformed with ECC enabled, theRAM Error Status Register flags anyerrors
thataredetected bytheECC logic oftheL2RAMW.
NOTE: The RAM Error Status Register does notindicate ECC errors thataredetected bytheCortex
R5F CPU. These errors andhandled andflagged intheR5F registers

<!-- Page 390 -->

Module Operation www.ti.com
390 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.2.2.2 Memory Scrubbing
Toincrease memory reliability, theL2RAMW hasanoptional "memory scrubbing" feature, which
automatically corrects single biterrors whenever they aredetected during anyRAM read. The reason for
performing thisaction isthatifasingle biterror occurs ontheRAM, andnoimmediate action istaken to
correct it,itispossible thatanearby bitcellwillbecorrupted aswell atsome point. Ifthiswere tooccur,
thetwocorrupted bitswould result inadouble-bit error thatcannolonger becorrected bytheSECDED
algorithm.
Memory scrubbing canbeenabled bysetting theMemory Scrubbing Enable (MSE) bitintheL2RAMW
Module Control Register (RAMCTRL). Note thattheECC Detect Enable (ECC_DETECT_EN) field in
RAMCTRL must besettoAhbefore enabling memory scrubbing, since memory scrubbing uses the
L2RAMW SECDED logic.
8.2.2.3 SECDED Malfunction
Toenhance device safety, theL2RAMW hasaSECDED malfunction detection feature toensure thatthe
SECDED logic isfunctioning correctly. Every time ECC iscalculated foraCPU write data oraread data
foraread-modify-write operation, theresults oftheECC correction arecompared back again tothe
original data value toensure thattheSECDED logic isworking correctly. Ifanerror intheSECDED logic
isdetected, itwillbeflagged intheRAMERRSTATUS Register (RAM Error Status).
8.2.2.4 L2RAMW Error Types andResponses
Table 8-1.L2RAMW Error Types
Error Source Corresponding RAMERRSTATUS Bit ESM Group
CPU Write ECC single error (correctable) CPUWE (0) Group 1
ECC double biterrors: Group 3,buserror
Read-Modify-Write (RMW) ECC double biterror RMWDE (7)
CPU Write ECC double biterror CPUWDE (5)
Uncorrectable error Type A: Group 3,buserror
Write SECDED malfunction error WEME (3)
Redundant address decode error ADE (2)
Read SECDED malfunction error REME (1)
Uncorrectable error Type B: Group 2
Memory scrubbing SECDED malfunction error MSSM (18)
Memory scrubbing redundant address decode error MSRA (17)
Memory scrubbing address /control parity error MSACP (16)
ECC single bitanddouble bitdiagnostic errors DRDE(22), DRSE(21), DWDE(20), DWSE(19)
Merged mux diagnostic error MMDE (12)
Write SECDED malfunction diagnostic error WEMDE (11)
Read SECDED malfunction diagnostic error REMDE (10)
Write data merged mux error MME (9)
Redundant address decode diagnostic error ADDE (4)
Command parity error onidle CPEOI (15)
Address /Control parity error PACE(8) Group 3,buserror
Level 2RAM illegal address error n/a n/a(bus error only)
Memory initialization error MIE (13) n/a(bus error only)

<!-- Page 391 -->

www.ti.com Module Operation
391 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.2.2.5 Support forCortex-R5F CPU'sAddress andControl Bus Parity Checking
The Cortex-R5F CPU provides parity bitsfortheaddress andcontrol signals going toL2RAMW. The
L2RAMW module also computes theparity bitsbased ontheCPU's address busandcontrol signals. The
computed parity bitsarecompared against theparity bitsreceived from theCPU. Amismatch isrecorded
asAddress/Control parity error (bit8)intheRAMERRSTATUS register andsignaled asanAddress Parity
Failure totheError Signaling Module (ESM). Italso generates abuserror.
The error flagintheRAMERRSTATUS register must becleared bytheapplication inorder forthe
L2RAMW interface module tocontinue capturing subsequent errors anderror addresses.
NOTE: NoChange OfParity Scheme On-The-Fly: The L2RAMW interface module does not
support on-the-fly change totheparity scheme being used forchecking theCPU address
busandcontrol bus. The application must ensure thattheparity polarity (odd oreven) isnot
changed while there isanongoing access totheL2RAM.
8.2.2.6 Redundant Address Decode
The L2RAMW module generates thememory selects foreach oftheL2RAMW banks aswell astheECC
memory based ontheCPU address. The logic togenerate these memory selects isduplicated andthe
outputs compared todetect anyaddress decode errors. Amismatch isindicated asanAddress Error to
theError Signaling Module (ESM). The L2RAMW orECC address thatcaused thefault iscaptured inthe
RAMUERRADDR register. This isa64-bit address thatisstored asanoffset from thebase ofthe
L2RAMW orECC memory.
Asdescribed earlier, each individual physical RAM bank is36bitswide. Each RAM bank contributes 32
bitsofdata and4bitsofECC when thebusmaster performs a64-bit read from theL2RAM. Each
L2RAMW bank receives amemory select andtheaddress from theL2RAMW interface module. Any
difference between theaddress andthememory selects results inwrong data andECC pairbeing sent to
theCPU. The CPU's SECDED block willdetect thisdata error.
The L2RAMW interface module also supports amechanism totesttheoperation oftheredundant address
decode logic andthecompare logic. This testing issupported byproviding ateststimulus, andcanbe
triggered bytheapplication byconfiguring theRAMTEST register. The address ofanyerror identified
during testing oftheredundant address decode andcompare logic isnotcaptured inthe
RAMUERRADDR register.
NOTE: Address decode checking when incompare logic testmode: When theaddress decode
andcompare logic testmode isenabled, theredundant address decode andcompare logic
isnotavailable forchecking theproper generation ofthememory selects fortheL2RAMW
andECC memory.

<!-- Page 392 -->

Module Operation www.ti.com
392 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.2.3 L2RAMW Auto-Initialization
The RAM memory canbeinitialized byusing thededicated auto-initialization hardware. The L2RAMW
module initializes theentire memory when theauto-init isenabled fortheRAM. AllRAM data memory is
initialized tozeros andtheECC memory isinitialized tothecorrect ECC value forzeros, thatis,0Ch.
8.2.4 Trace Module Support
The L2RAMW module traces outthefollowing signals totheRAM Trace Port (RTP) module, thereby
providing RAM dataport trace capability.
*18-bit address line
*64-bit data bus
*Byte strobe information
*Current access master identification number
*Access type: Opcode ordata fetch
*Read orWrite access
Nodata istraced foranaccess toECC memory.
8.2.5 Emulation/Debug Mode Behavior
The following describes thebehavior oftheL2RAMW Module when indebug mode:
*Nosingle-bit error interrupt isgenerated norisanysingle-bit error address captured even when the
RAMOCCUR counter reaches theprogrammed single-bit error correction threshold.
*Nouncorrectable error interrupt isgenerated norisanydouble-bit error address captured.
*Noaddress parity error interrupt isgenerated norisanyparity error address captured.
*The RAMUERRADDR register isnotcleared byaread indebug mode.
-That is,ifadouble-bit error address iscaptured andisnotread bytheCPU before entering debug
mode, then itremains frozen during debug mode even ifitisread.
*The RAMPERRADDR register isnotcleared byaread indebug mode.
8.2.6 Diagnostic Test Procedure
1.Write testvectors DIAG_DATA_VECTOR_H, DIAG_DATA_VECTOR_L, DIAG_ECC, and
RAMADDRDEC_VECT with desire testirritants.
2.InRAMTEST, write TEST_ENABLE field with AhandTEST_MODE field with thechoice ofinequality
orequality testing forredundant address decoding andSECDED multifunction diagnostics. Setup
proper values inDIAG_ECC, DIAG_DATA_VECTOR_L andDIAG_DATA_VECTOR_H registers. ECC
single bitordouble bitread andwrite diagnostic errors willbegenerated ifthevalues donotmatch.
3.InRAMTEST, write TRIGGER bit.Remember thetrigger canonly beenabled when TEST_ENABLE is
equal toAhandRAMERRSTATUS[22,21,20,19,12,11,10, 4]bitsarezero. Triggering diagnostic test
while thememory banks arebusy willforce thetesttowait until thebanks arefree. Note alldiagnostic
testing fortwoSECDEDs andcompare logics ofredundant address decode, twoSECDED
malfunctions, data merging block arecompleted inoneHCLK cycle even though theTRIGGER bitcan
lastoneVCLK cycle.
4.Read back register bitsRAMERRSTATUS[22,21,20,19,12,11,10, 4]andobserve pass/fail status. No
error bitwillbesetifnoerror isdetected inthediagnostic test. The diagnostic errors willalso besent
toESM group 2as"uncorrectable error type B".

<!-- Page 393 -->

www.ti.com Control andStatus Registers
393 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3 Control andStatus Registers
The L2RAMW Module registers listed inTable 8-2areaccessed through thesystem module register
space intheCortex-R5F CPUs memory map. Allregisters are32-bit wide andarelocated ona32-bit
boundary. Reads andwrites toregisters aresupported in8-,16-, and32-bit accesses. The base address
fortheL2RAMW control registers isFFFF F900h.
Table 8-2.L2RAMW Module Control andStatus Registers
Offset Acronym Register Description Section
00h RAMCTRL L2RAMW Module Control Register Section 8.3.1
10h RAMERRSTATUS L2RAMW Module Error Status Register Section 8.3.2
24h DIAG_DATA_VECTOR_H Diagnostic Data Vector High Register Section 8.3.3
28h DIAG_DATA_VECTOR_L Diagnostic Data Vector Low Register Section 8.3.4
2Ch DIAG_ECC Diagnostic ECC Vector Register Section 8.3.5
30h RAMTEST L2RAMW RAM Test Register Section 8.3.6
38h RAMADDRDEC_VECT L2RAMW RAM Address Decode Vector Test Register Section 8.3.7
3Ch MEMINIT_DOMAIN L2RAMW Memory Initialization Domain Register Section 8.3.8
44h BANK_DOMAIN_MAP0 Bank toDomain Mapping Register 0 Section 8.3.9
48h BANK_DOMAIN_MAP1 Bank toDomain Mapping Register 1 Section 8.3.10
8.3.1 L2RAMW Module Control Register (RAMCTRL)
The RAMCTRL register, shown inFigure 8-2anddescribed inTable 8-3,controls thesafety features
supported bytheL2RAMW Module.
Figure 8-2.L2RAMW Module Control Register (RAMCTRL) (offset =00h)
31 30 29 28 27 24
Reserved EMU_TRACE_DIS Reserved ADDR_PARITY_OVERRIDE
R-0 R/WP-0 R-0 R/WP-5h
23 21 20 19 16
Reserved MSE ADDR_PARITY_DISABLE
R-0 R/WP-0 R/WP-5h
15 13 12 11 7 8
Reserved EEMMS Reserved ECC_WR_EN
R-0 R/WP-0 R-0 R/WP-0
7 5 4 3 0
Reserved CPUWSC ECC_DETECT_EN
R-0 R/WP-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write allowed inprivileged mode only; -n=value after reset
Table 8-3.L2RAMW Module Control Register (RAMCTRL) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30 EMU_TRACE_DIS Emulation Mode Trace Disable. This bit,when set,disables thetracing ofread
data toRAM Trace Port (RTP) module during emulation mode access.
0 Data isallowed tobetraced outtothetrace modules foremulation mode
accesses.
1 Data isblocked from being traced outtothetrace modules foremulation mode
accesses.
29-28 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 394 -->

Control andStatus Registers www.ti.com
394 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) ModuleTable 8-3.L2RAMW Module Control Register (RAMCTRL) Field Descriptions (continued)
Bit Field Value Description
27-24 ADDR_PARITY_OVERRIDE Address Parity Override. This field, when settoAh,willinvert theparity scheme
selected bythedevice global parity selection. The address parity checker would
then work ontheinverted parity scheme. Bydefault, theparity scheme isthe
same astheglobal device parity scheme.
Ah Parity scheme isopposite tothedevice global parity scheme.
Allother values Parity scheme isthesame asthedevice global parity scheme.
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 MSE MSE: Memory Scrubbing Enable. This bitenables ordisables memory
scrubbing ofsingle-bit errors onread operations.
Note: TheECC_DETECT_EN field must besettoAhbefore enabling
memory scrubbing, since memory scrubbing uses theL2RAMW SECDED
logic.
0 Memory scrubbing isdisabled.
1 Memory scrubbing isenabled.
19-16 ADDR_PARITY_DISABLE Address/Control Bus Parity Detect Disable. This field, when settoAh,disables
theparity checking fortheaddress andcontrol bus. The parity checking is
enabled when thisfield issettoanyother value.
Note: Theapplication must ensure thatPACE field inRAMERRSTATUS
register iscleared before enabling address/control busparity checking.
Ah Address parity checking isdisabled.
Allother values Address parity checking isenabled.
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12 EEMMS Enable ESM notification (Parity, Redundant Address Decode, SECDED
malfunction) forwrite back during memory scrubbing.
0 ESM willnotbesignaled when anerror occurs during memory scrubbing write
back.
1 ESM willbesignaled when anerror occurs during memory scrubbing write back.
11-9 Reserved 0 Reads return 0.Writes have noeffect.
8 ECC_WR_EN ECC Memory Write Enable. This bitisprovided toprevent accidental writes to
theECC memory. Awrite access totheECC memory isallowed only when the
ECC_WR_EN bitissetto1.Ifthisbitiscleared, then anywrites toECC
memory areignored.
Note: Reads areallowed from theECC memory regardless ofthestate ofthe
ECC_WR_EN.
0 ECC memory writes aredisabled.
1 ECC memory writes areenabled.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 CPUWSC CPUWSC: CPU Write SERR Capture. Bydefault, single biterror arenot
signaled toESM module. This bitallows theoption tocapture thestatus and
notify ESM.
Note: This feature isonly applicable toCPU write data.
0 Disable single biterror status capture andESM notification.
1 Enable single biterror status capture andESM notification.
3-0 ECC_DETECT_EN ECC Detect Enable. This isa4-bit keytoenable theECC detection feature in
theL2RAMW Module. Error detection, status updates, anddata correction are
performed bytheL2RAMW logic only ifECC detection isenabled. ECC
detection isenabled bydefault after reset.
Note: Disabling ECC ontheL2RAMW module willdisable ECC error
checking only fortheECC functions thattheL2RAM handles (sub 64-bit
Write Operations). Allother ECC handling isdone bytheR5F CPU. ECC
error checking cannot bedisabled ontheR5F CPU.
5h ECC detection isdisabled.
Allother values ECC detection isenabled.

<!-- Page 395 -->

www.ti.com Control andStatus Registers
395 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.2 L2RAMW Error Status Register (RAMERRSTATUS)
The RAMERRSTATUS register, shown inFigure 8-3anddescribed inTable 8-4,indicates thestatus of
thevarious error conditions monitored bytheL2RAMW Module.
Figure 8-3.L2RAMW Module Error Status Register (RAMERRSTATUS) (offset =10h)
31 24
Reserved
R-0
23 22 21 20 19 18 17 16
Reserved DRDE DRSE DWDE DWSE MSSM MSRA MSACP
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
15 14 13 12 11 10 9 8
CPEOI Reserved MIE MMDE WEMDE REMDE MME PACE
R/W1CP-0 R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
7 6 5 4 3 2 1 0
RMWDE Reserved CPUWDE ADDE WEME ADE REME CPUWE
R/W1CP-0 R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Privilege Write 1toClear; -n=value after reset
Table 8-4.L2RAMW Module Error Status Register (RAMERRSTATUS) Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22 DRDE Diagnostic Read Double-bit Error. This bitindicates thatadouble-bit error hasoccurred during
diagnostic oftheL2RAMW SECDED logic thatisused tohandle read ofread-modify write operations.
This bitmust becleared bywriting a1toitbefore anynew error canbegenerated.
0 Adouble-bit error didnotoccur during diagnostic.
1 Adouble-bit error occurred during diagnostic.
21 DRSE Diagnostic Read Single-bit Error. This bitindicates thatasingle-bit error hasoccurred during diagnostic
oftheL2RAMW SECDED logic thatisused tohandle read ofread-modify write operations. This bit
must becleared bywriting a1toitbefore anynew error canbegenerated.
0 Asingle-bit error didnotoccur during diagnostic.
1 Asingle-bit error occurred during diagnostic.
20 DWDE Diagnostic Write Double-bit Error. This bitindicates thatadouble-bit error hasoccurred during
diagnostic oftheL2RAMW SECDED logic thathandles write operations. This bitmust becleared by
writing a1toitbefore anynew error canbegenerated.
0 Adouble-bit error didnotoccur during diagnostic.
1 Adouble-bit error occurred during diagnostic.
19 DWSE Diagnostic Write Single-bit Error. This bitindicates thatasingle-bit error hasoccurred during diagnostic
oftheL2RAMW SECDED logic thathandles write operations. This bitmust becleared bywriting a1to
itbefore anynew error canbegenerated.
0 Asingle-bit error didnotoccur during diagnostic.
1 Asingle-bit error occurred during diagnostic.
18 MSSM Memory Scrubbing write back SECDED Malfunction. This indicates thataSECDED malfunction
occurred during memory scrubbing write back. This bitmust becleared bywriting a1toitbefore any
new error canbegenerated.
0 ASECDED malfunction didnotoccur during scrubbing write back.
1 ASECDED malfunction occurred during scrubbing write back.
17 MSRA Memory Scrubbing write back Redundant Address decode error. This bitindicates thataredundant
address decode error occurred during memory scrubbing write back. This bitmust becleared bywriting
a1toitbefore anynew error canbegenerated.
0 Anaddress decode error didnotoccur during scrubbing write back.
1 Anaddress decode error occurred during scrubbing write back.

<!-- Page 396 -->

Control andStatus Registers www.ti.com
396 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) ModuleTable 8-4.L2RAMW Module Error Status Register (RAMERRSTATUS) Field Descriptions (continued)
Bit Field Value Description
16 MSACP Memory Scrubbing write back Redundant Address decode error. This bitindicates thatanaddress-
control parity error occurred during memory scrubbing write back. This bitmust becleared bywriting a1
toitbefore anynew error canbegenerated.
0 Anaddress control parity error didnotoccur during memory scrubbing write back.
1 Anaddress control parity error occurred during memory scrubbing write back.
15 CPEOI Command Parity Error onIdle. This bitindicates anerror occurred foranidlecommand with parity error.
This bitmust becleared bywriting a1toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
14 Reserved 0 Reads return 0.Writes have noeffect.
13 MIE Memory Initialization Error. This bitindicated anerror occurred foranaccess toabank under memory
initialization. Access toabank under memory initialization isnotallowed. Itwillresult inafalse double
biterror. This bitmust becleared bywriting a1toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
12 MMDE Merged MUX Diagnostic Error. This bitindicates aerror was detected onthecompare logic ofthemux
logic used fordata merging ofaread modify write operation during diagnostic test. This bitmust be
cleared bywriting a1toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
11 WEMDE Write ECC Malfunction Diagnostic Error. This bitindicated anerror was detected onthecompare logic
ofthewrite ECC malfunction during diagnostic test. This bitmust becleared bywriting a1toitbefore
anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
10 REMDE Read ECC Malfunction Diagnostic Error. This bitindicated anerror was detected onthecompare logic
oftheread ECC malfunction during diagnostic test. This bitmust becleared bywriting a1toitbefore
anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
9 MME Merged Mux Error. This bitindicates anerror was detected onthemux logic thatisused tomerge the
corrected read andwrite data foraread modify write operation. This bitmust becleared bywriting a1
toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
8 PACE Address and/or Control busParity Error. This bitmust cleared bywriting a1toitbefore anynew error
canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
7 RMWDE Read-Modify-Write Double BitError. This bitindicates thatanECC uncorrectable (double bit)error was
detected during read access oftheread modify write operation. This bitmust becleared bywriting a1
toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
6 Reserved 0 Reads return 0.Writes have noeffect.
5 CPUWDE CPU Write Double-bit Error. This bitindicates thatanECC uncorrectable (double bit)error was detected
during write access. This bitmust becleared bywriting a1toitbefore anynew error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.

<!-- Page 397 -->

www.ti.com Control andStatus Registers
397 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) ModuleTable 8-4.L2RAMW Module Error Status Register (RAMERRSTATUS) Field Descriptions (continued)
Bit Field Value Description
4 ADDE Redundant address decoding diagnostic error. This bitindicates thattheredundant address decode
logic diagnostic testhasdetected thatacompare element hasmalfunctioned during thetesting ofthe
logic. This bitisonly setintestmode. This bitmust becleared bywriting a1toitforgeneration ofany
new uncorrectable error interrupt innon-test mode.
0 Anerror didnotoccur.
1 Anerror occurred.
3 WEME Write ECC Malfunction Error. This bitIndicates thattheSECDED logic failed tocorrect asingle biterror
during aCPU write operation. This bitmust becleared bywriting a1toitbefore anynew error canbe
generated.
0 Anerror didnotoccur.
1 Anerror occurred.
2 ADE Address Decode Error. This bitindicates than anaddress error was generated bytheredundant
address decode logic duetoafunctional failure. This bitmust becleared bywriting a1toitbefore any
new error canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
1 REME Read ECC Malfunction Error. Indicates thattheSECDED logic failed tocorrect asingle biterror onthe
read ofaread-modify-write operation. This bitmust becleared bywriting a1toitbefore anynew error
canbegenerated.
0 Anerror didnotoccur.
1 Anerror occurred.
0 CPUWE CPU Write Single Error. This bitindicates thatasingle-bit error occurred during write access. This bit
must becleared bywriting 1toitinorder toclear theinterrupt request andtoenable subsequent single-
biterror interrupt generation.
0 Anerror didnotoccur.
1 Anerror occurred.

<!-- Page 398 -->

Control andStatus Registers www.ti.com
398 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.3 L2RAMW Diagnostic Data Vector High Register (DIAG_DATA_VECTOR_H)
The DIAG_DATA_VECTOR_H register, shown inFigure 8-4anddescribed inTable 8-5,isused in
conjunction with theRAMTEST register toperform diagnostic tests.
Figure 8-4.L2RAMW Diagnostic Data Vector High Register (DIAG_DATA_VECTOR_H)
(offset =24h)
31 0
DIAGNOSTIC_VECTOR[63:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 8-5.L2RAMW Diagnostic Data Vector High Register (DIAG_DATA_VECTOR_H)
Field Descriptions
Bit Field Description
31-0 DIAGNOSTIC_VECTOR Used inconjunction with DIAG_DATA_VECTOR_L toform a64-bit testvector used for
diagnostic testoftwoSECDEDs (read andwrite) andcompare logic ofthetwoSECDED
malfunctions andmerged mux. This register istheupper 32bits. This register isused in
conjunction with theRAMTEST register toperform diagnostic tests. See Section 8.2.6 for
details onhow tostart adiagnostic test.
8.3.4 L2RAMW Diagnostic Data Vector Low Register (DIAG_DATA_VECTOR_L)
The DIAG_DATA_VECTOR_L, shown inFigure 8-5anddescribed inTable 8-6,isused inconjunction
with theRAMTEST register toperform diagnostic tests.
Figure 8-5.L2RAMW Diagnostic Vector Low Register (DIAG_DATA_VECTOR_L)
(offset =28h)
31 0
DIAGNOSTIC_VECTOR[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 8-6.L2RAMW Diagnostic Vector Low Register (DIAG_DATA_VECTOR_L)
Field Descriptions
Bit Field Description
31-0 DIAGNOSTIC_VECTOR Used inconjunction with DIAG_DATA_VECTOR_H toform a64-bit testvector used for
diagnostic testoftwoSECDEDs (read andwrite) andcompare logic ofthetwoSECDED
malfunctions andmerged mux. This register isthelower 32bits. This register isused in
conjunction with theRAMTEST register toperform diagnostic tests. See Section 8.2.6 for
details onhow tostart adiagnostic test.

<!-- Page 399 -->

www.ti.com Control andStatus Registers
399 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.5 L2RAMW Diagnostic ECC Vector Register (DIAG_ECC)
The DIAG_ECC register, shown inFigure 8-6anddescribed inTable 8-7,captures theaddress forwhich
theCortex-R5F CPU detected amulti-bit error.
Figure 8-6.L2RAMW Diagnostic ECC Vector Register (DIAG_ECC) (offset =2Ch)
31 16
Reserved
R-0
15 8 7 0
Reserved DIAG_ECC_VECTOR
R-0 R/WP-U
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; U=Unknown; -n=value after reset
Table 8-7.L2RAMW Diagnostic ECC Vector Register (DIAG_ECC) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 DIAG_ECC_VECTOR 0-FFh Diagnostic ECC Vector. This field provides an8-bit ECC testvector used fordiagnostic
testofthetwoSECDEDs andcompare logic fortwoSECDED malfunctions andmerged
mux. This register isused inconjunction with DIAG_DATA_VECTOR_H and
DIAG_DATA_VECTOR_L registers toform adata/ECC pairinthediagnostic ECC
checking test. See Section 8.2.6 fordetails onhow tostart adiagnostic test.

<!-- Page 400 -->

Control andStatus Registers www.ti.com
400 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.6 L2RAMW RAM Test Mode Control Register (RAMTEST)
The RAMTEST register, shown inFigure 8-7anddescribed inTable 8-8,controls thetestmode ofthe
L2RAMW Module.
Figure 8-7.L2RAMW Module Test Mode Control Register (RAMTEST) (offset =30h)
31 16
Reserved
R-0
15 9 8 7 6 5 4 3 0
Reserved TRIGGER TEST_MODE Reserved TEST_ENABLE
R-0 R/WP-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 8-8.L2RAMW Module Test Mode Control Register (RAMTEST) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 TRIGGER Test Trigger. This isanauto clear testtrigger used totesttheredundant address decode,
data merging mux, SECDED malfunction compare logic, andECC checking logics. The
diagnostic testisexecuted when testmode isenabled andthetesttrigger isapplied by
writing a1tothisbit.The trigger isvalid only iftestmode isenabled, thecorrect mode is
configured intheTEST_MODE field, andalldiagnostic error bitsintheRAMERRSTATUS
register areinthecleared state. The trigger bitisauto clear after thetestandhastobe
written again foranew test.
7-6 TEST_MODE Test Mode. This field selects either equality orinequality testing schemes forredundant
address decoding andSECDED malfunction diagnostics.
IfTEST_MODE issetto2h,equality check isdone. The teststimulus stored in
RAMADDRDEC_VECT register isfeddirectly toboth thechannels ofthecomparator. If
theXOR ofthese twoinputs isnotzero ,then UERR interrupt isgenerated andADDE
flagissetinRAMERRSTATUS register.
IfTEST_MODE issetto1h,inequality check isdone. The teststimulus stored in
RAMADDRDEC_VECT register isinverted andfedintoonechannel andthenon-inverted
vector isfedintotheother channel. IftheXOR ofthese inputs iszero ,then theUERR
interrupt isgenerated andADDE flagissetinRAMERRSTATUS register.
5-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 TEST_ENABLE Test Enable. This isa4-bit keytoenable theredundant address decode, SECDED
malfunction, data merging mux andECC checking diagnostics. Ifthetestscheme is
enabled, then thecompare logic uses thetestvector inputs from the
RAMADDRDEC_VECT, DIAG_ECC, DIAG_DATA_VECTOR_L, and
DIAG_DATA_VECTOR_H registers. The functional path comparison isdisabled when test
mode isenabled.
Ah Test mode isenabled.
Allother values Test mode isdisabled.

<!-- Page 401 -->

www.ti.com Control andStatus Registers
401 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.7 L2RAMW RAM Address Decode Vector Test Register (RAMADDRDEC_VECT)
The RAMADDRDEC_VECT register, shown inFigure 8-8anddescribed inTable 8-9,isused fortesting
theredundant address decode andcompare logic oftheL2RAMW Module.
Figure 8-8.L2RAMW RAM Address Decode Vector Test Register (RAMADDRDEC_VECT)
(offset =38h)
31 27 26 25 16
Reserved DESV Reserved
R-0 R/WP-0 R-0
15 0
RAM_CHIP_SELECT
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 8-9.L2RAMW RAM Address Decode Vector Test Register (RAMADDRDEC_VECT)
Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 DESV Diagnostic ECC Select Vector. This bitisused tostore theECC select testvector forthe
redundant address decode testlogic. The stored value ispassed asteststimulant forthe
built intestscheme.
25-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 RAM_CHIP_SELECT 0-FFFFh RAM Chip Select. This field isused tostore theRAM chip select value fortheredundant
address decode andcompare logic. The stored value ispassed asteststimulus forthe
built-in testscheme.

<!-- Page 402 -->

Control andStatus Registers www.ti.com
402 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.8 L2RAMW Memory Initialization Domain Register (MEMINIT_DOMAIN)
The MEMINIT_DOMAIN register, shown inFigure 8-9anddescribed inTable 8-10 ,stores theaddress for
which anaddress-parity error was detected.
Figure 8-9.L2RAMW Memory Initialization Domain Register (MEMINIT_DOMAIN) (offset =3Ch)
31 16
Reserved
R-0
15 8 7 0
Reserved MEMINIT_ENA
R-0 R/WP-FFh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 8-10. L2RAMW Memory Initialization Domain Register (MEMINIT_DOMAIN) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 MEMINIT_ENA[ n] Memory Initialization Enable. Each bitncorresponds toanindividual memory domain. If
thecorresponding bitissetto1when aninitialization oftheRAM memory isexecuted,
then thatsection oftheRAM memory willbeinitialized. Ifthecorresponding bitiscleared
to0when aninitialization oftheRAM memory isexecuted, then thatsection oftheRAM
memory willnotbeaffected. After reset, allmemory power domains areenabled (setto1)
bydefault.
Bit0:enable bitforpower domain 0.
Bit1:enable bitforpower domain 1.
:
Bit7:enable bitforpower domain 7.
1 Enable thememory inthispower domain tobeinitialized.
0 Disable thememory inthispower domain from being initialized.

<!-- Page 403 -->

www.ti.com Control andStatus Registers
403 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.9 L2RAMW Bank toDomain Mapping Register0 (BANK_DOMAIN_MAP0)
The BANK_DOMAIN_MAP0 register, shown inFigure 8-10 anddescribed inTable 8-11,stores the
address forwhich anaddress-parity error was detected.
Figure 8-10. L2RAMW Bank toDomain Mapping Register0 (BANK_DOMAIN_MAP0)
(offset =44h)
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd BANK7_MAP Rsvd BANK6_MAP Rsvd BANK5_MAP Rsvd BANK4_MAP
R-0 R-DS R-0 R-DS R-0 R-DS R-0 R-DS
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd BANK3_MAP Rsvd BANK2_MAP Rsvd BANK1_MAP Rsvd BANK0_MAP
R-0 R-DS R-0 R-DS R-0 R-DS R-0 R-DS
LEGEND: R=Read only; DS=Device Specific; -n=value after reset
Table 8-11. L2RAMW Bank toDomain Mapping Register0 (BANK_DOMAIN_MAP0)
Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 BANK7_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 7is
associated.
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 BANK6_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 6is
associated.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 BANK5_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 5is
associated.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 BANK4_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 4is
associated.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 BANK3_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 3is
associated.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 BANK2_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 2is
associated.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 BANK1_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 1is
associated.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 BANK0_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 0is
associated.

<!-- Page 404 -->

Control andStatus Registers www.ti.com
404 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedLevel 2RAM (L2RAMW) Module8.3.10 L2RAMW Bank toDomain Mapping Register1 (BANK_DOMAIN_MAP1)
The BANK_DOMAIN_MAP1 register, shown inFigure 8-11 anddescribed inTable 8-12,stores the
address forwhich anaddress-parity error was detected.
Figure 8-11. L2RAMW Bank toDomain Mapping Register1 (BANK_DOMAIN_MAP1)
(offset =48h)
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd BANK15_MAP Rsvd BANK14_MAP Rsvd BANK13_MAP Rsvd BANK12_MAP
R-0 R-DS R-0 R-DS R-0 R-DS R-0 R-DS
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd BANK11_MAP Rsvd BANK10_MAP Rsvd BANK9_MAP Rsvd BANK8_MAP
R-0 R-DS R-0 R-DS R-0 R-DS R-0 R-DS
LEGEND: R=Read only; DS=Device Specific; -n=value after reset
Table 8-12. L2RAMW Bank toDomain Mapping Register1 (BANK_DOMAIN_MAP1)
Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 BANK15_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 15
isassociated.
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 BANK14_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 14
isassociated.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 BANK13_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 13
isassociated.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 BANK12_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 12
isassociated.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 BANK11_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 11
isassociated.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 BANK10_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 10
isassociated.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 BANK9_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 9is
associated.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 BANK8_MAP 0-7h This 3-bit field allows thesoftware toread thememory power domain number thatbank 8is
associated.