# Architecture

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 112-251

---


<!-- Page 112 -->

112 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureChapter 2
SPNU563A -March 2018
Architecture
This chapter consists offive sections. The first section describes specific aspects ofthe device
architecture. The second section describes theclocking structure ofthemicrocontrollers. The third section
gives anoverview ofthedevice memory organization. The fourth section details exceptions onthedevice,
andthelastsection describes thesystem andperipheral control registers ofthemicrocontroller.
Topic ........................................................................................................................... Page
2.1 Introduction ..................................................................................................... 113
2.2 Memory Organization ........................................................................................ 120
2.3 Exceptions ....................................................................................................... 139
2.4 Clocks ............................................................................................................. 142
2.5 System andPeripheral Control Registers ............................................................ 151

<!-- Page 113 -->

www.ti.com Introduction
113 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.1 Introduction
The TMS570LC43x family ofmicrocontrollers isbased ontheTexas Instruments TMS570 Architecture.
This chapter describes specific aspects ofthearchitecture asapplicable totheTMS570LC43x family of
microcontrollers.
2.1.1 Architecture Block Diagram
The TMS570LC43x microcontrollers arebased ontheTMS570 Platform architecture, which defines the
interconnect between thebusmasters andthebusslaves. The architecture consists oftwomain
interconnects which connect allthemasters andslaves together. The separation ofthetwointerconnects
creates aconcept oftwosafety islands. The CPU safety island consists oftheCPU Interconnect
Subsystem which glues themasters andslaves together. The CPU safety island contains high degree of
safety diagnostics onthebussystem andthememories. Memories andbuses areprotected bymeans of
ECC onthedata path using Single-Bit Correction Double-Bit Detection (SECDED) scheme. Parity
detection scheme isused onalladdress andcontrol paths between allmasters andslaves. Safety
diagnostic logic isbuilt intotheCPU Interconnect Subsystem where alltraffics going inandoutare
checked against their expected behaviors during application runtime. Inaddition, self-test logic isbuilt into
theCPU Interconnect Subsystem which canbeenabled todiagnose possible faults. The Peripheral safety
island consists ofthePeripheral Interconnect Subsystem which glues therestofthemasters andslaves in
thedevice. Diagnostic ontheperipheral island isbymeans ofECC orparity protection ontheperipheral
memories andMPU protection.
Figure 2-1shows ahigh-level architectural block diagram forthemicrocontroller.

<!-- Page 114 -->

HTU1 FTU HTU2
Peripheral Interconnect Subsystem
CRC2 PCR2 PCR3
EMAC
SlavesDCAN1
DCAN2
DCAN3
MibSPI1IOMM
PMMLockstep
VIMs
RTI
DCC1DMA
EMIF
eQEP
1,2
eCAP
1..6
ePWM
1..7NMPUEMAC
DCAN4PCR1
EMIF
SlaveCPU Interconnect SubsystemDual Cortex -R5F
CPUs in lockstep32kB Icache
& Dcache w /
ECC
POM
4MB Flash
&
128kB
Flash for
EEPROM
Emulation
w/ ECC512kB
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
R5FMibSPI2
MibSPI3
MibSPI4
LIN1/SCI1
LIN2/SCI2SCI3
SCI4
I2C1
I2C2
FlexRay
GIO
N2HET1
N2HET2
MibADC1
MibADC2ESM
MibSPI5CRC1Dma_portA
uSCU
SDC MMRAcp_s sramFlash
portBFlash
portA emifpom Ps_scr_m Dma
portAAxi-m Acp_mAxi-pp Dma portB dap dmm htu1 ftu htu2 emac
crc2 crc1 pcr3 Sdc mmr port pcr2 pcr1 Ps_scr_s
Introduction www.ti.com
114 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureFigure 2-1.Architectural Block Diagram

<!-- Page 115 -->

www.ti.com Introduction
115 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.1.2 Definitions ofTerms
Table 2-1provides adefinition ofterms used inthearchitectural block diagram.
Table 2-1.Definition ofTerms
Acronym/Term FullForm Description
ADCx Analog-to-Digital Converter The ADC uses theSuccessive Approximation Register architecture. Itfeatures a
selectable 10-bit or12-bit resolution. The ADC module also includes aRAM to
hold theconversion results. Adigital logic wrapper manages accesses tothe
control andstatus registers. There aretwoADC modules onthisdevice.
CCM-R5F CPU Compare Module for
Cortex-R5F coreDuring lockstep mode, theoutputs ofthetwoCPUs arecompared oneach CPU
clock cycle bythismodule. Any miscompare isflagged asanerror ofthehighest
severity level. The outputs ofthetwoVIMs inlockstep arealso compared on
each cycle bythismodule.
Cortex-R5F
CPU- The Cortex-R5F hasoneAXI-M master port ontheCPU Interconnect
Subsystem andanother AXI-PP peripheral port ontheperipheral Interconnect
Subsystem forlowlatency access. Each master port islimited toaccesses on
theresources attached totherespective interconnect.
CPU
Interconnect
SubsystemCPU Side Switched Central
Resource ControllerThis isoneofthetwomain SCRs inthedevice. Itarbitrates between the
accesses from multiple busmasters tothebusslaves using around robin
priority scheme. This interconnect subsystem contains diagnostic logic to
perform parity checking onaddress andcontrol signals from busmasters, parity
checking onresponse signals from slaves, ECC generation andevaluation on
thedatapath fortransactions initiated bythenon-CPU masters andalso selftest
logic todiagnose itself.
CRC x Cyclic Redundancy Checker The CRC module provides twochannels toperform background signature
verification onanymemory region using a64-bit maximum-length linear
feedback shift register (LFSR) .The CRC module isabusslave inthisdevice.
DAP Debug Access Port The DAP allows atoolsuch asadebugger toread from orwrite toanyregion in
thedevice memory-map. The DAP isabusmaster inthisdevice.
DCANx Controller Area Network
controllerThe DCAN supports theCAN 2.0B protocol standard anduses aserial, multi-
master communication protocol thatefficiently supports distributed real-time
control with robust communication rates ofupto1megabit persecond (Mbps).
The DCAN isideal forapplications operating innoisy andharsh environments
(forexample, automotive andindustrial fields) thatrequire reliable serial
communication ormultiplexed wiring.
DCCx Dual Clock Comparator This module isprimarily intended forusetodetermine theaccuracy ofaclock
signal during theexecution ofanapplication. Anadditional useofthismodule is
tomeasure thefrequency ofaselectable clock source, using theinput clock as
areference.
DMA Direct Memory Access The DMA module isused fortransferring 8-,16-, 32-or64-bit data across the
entire device memory-map. The DMA module isoneofthebusmasters onthe
device. That is,itcaninitiate aread orawrite transaction. DMA hastwomaster
ports with DMA_PortA andDMA_PortB. DMA_PortA isconnected totheCPU
Interconnect Subsystem andDMA_PortB isconnected tothePeripheral
Interconnect Subsystem. DMA cantransfer data from resources inCPU
Interconnect Subsystem toresources inthePeripheral Interconnect Subsystem
andvice versa.
DMM Data Modification Module The DMM allows atooltousethespecial DMM I/Ointerface tomodify anydata
value inanyRAM onthedevice. The modification isdone with minimal
interruption totheapplication execution, andcanbeused forcalibration of
application algorithms. theDMM isalso abusmaster inthisdevice.
eCAP Enhanced Capture Module The enhanced Capture (eCAP) module isessential insystems where accurate
timing ofexternal events isimportant.
eFuse Electronically Programmable
Fuse controllerElectrically programmable fuses (eFuses) areused toconfigure thedevice after
deassertion ofPORRST. The eFuse values areread andloaded intointernal
registers aspart ofthepower-on-reset sequence. The eFuse values are
protected with Single-Bit Error Correction Double-Bit Error Detection (SECDED)
codes. These fuses areprogrammed during theinitial factory testofthedevice.
The eFuse controller isdesigned sothatthestate oftheeFuses cannot be
changed once thedevice ispackaged.

<!-- Page 116 -->

Introduction www.ti.com
116 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-1.Definition ofTerms (continued)
Acronym/Term FullForm Description
ePWM Enhanced Pulse Width
ModulatorThe enhanced pulse width modulator (ePWM) peripheral isakeyelement in
controlling many ofthepower electronic systems found inboth commercial and
industrial equipments. These systems include digital motor control, switch mode
power supply control, uninterruptible power supplies (UPS), andother forms of
power conversion. The ePWM peripheral performs adigital toanalog (DAC)
function, where theduty cycle isequivalent toaDAC analog value; itis
sometimes referred toasaPower DAC.
eQEP Enhanced Quadrature
Encoder Pulse ModuleThe enhanced quadrature encoder pulse (eQEP) module isused fordirect
interface with alinear orrotary incremental encoder togetposition, direction,
andspeed information from arotating machine foruseinahigh-performance
motion andposition-control system.
ECC Error Correction Code This isacode thatisused bytheSingle-Bit Error Correction Double-Bit Error
Detection (SECDED) logic inside thetwoCortex-R5F processors (CPUs) and
various modules thatsupport ECC. Depending onthememory configuration, the
number ofECC bitsmay vary. There are8bitsofECC forevery 64bitsofdata
accessed from theCPU level 2memory such asflash andRAM. CPU 'slevel 1
cache system consists ofinstruction cache anddata cache andeach is
additionally composed ofdata RAM, tagRAM ordirty RAM. The number ofECC
bitsused toprotect these RAMs vary. Modules which support ECC protection on
their local RAMs canalso employ different number ofECC bitsdepending onthe
RAM 'sconfiguration. Forexample, DMA module use9bitsofECC toprotect its
local control packet memory.
EMAC Ethernet Media Access
ControllerThe EMAC hasadedicated DMA-type component thatisused totransfer data to
/from theEMAC descriptor memory from /toanother memory inthedevice
memory-map. This DMA-type component oftheEMAC isabusmaster inthis
device.
EMAC slaves Ethernet Media Access
Controller slave portsThere arefour EMAC slaves:
1.EMAC Control Module: thisprovides aninterface between theEMAC and
MDIO modules andthebusmasters. Italso includes 8KB ofRAM tohold
EMAC packet buffer descriptors.
2.EMAC: The EMAC module interfaces totheother devices ontheEthernet
Network using theMedia Independent Interface (MII) orReduced Media
Independent Interface (RMII).
3.Management Data Input /Output (MDIO): The MDIO module isused to
manage thephysical layer (PHY) device connected totheEMAC module.
4.Communications Port Programming Interface (CPPI): This isthe8KB of
RAM used tohold theEMAC packet buffer descriptors.
EMIF slaves External Memory Interface
slave portsThere arefiveEMIF slaves:
*External SDRAM memory: EMIF chip select 0
*External asynchronous memories: EMIF chip selects 2,3and4
*EMIF module control andstatus registers
EPC Error Profiling Controller This module isused toprofile theoccurrences ofsingle-bit anddouble-bit ECC
errors detected bytheCPU andtheCPU Interconnect Subsystem.
ESM Error Signal Module ESM collects andreports thevarious error conditions onthedevice. The error
condition iscategorized based onaseverity level. Error response isthen
generated based onthecategory oftheerror. Possible error responses include
alowpriority interrupt, high priority NMI interrupt andanexternal pinaction.
Flash Memory Level 2Flash Memory There aretwoslave ports (Flash_PortA andFlash_PortB) toaccess theflash
memory consisting ofthree flash banks. The twoports allow twomasters to
access among thethree banks inparallel. There aretwo2Mbyte banks andone
EEPROM bank. The EEPROM bank isaflash bank thatisdedicated foruseas
anemulated EEPROM. This device supports 128KB offlash foremulated
EEPROM.
FlexRay FlexRay communication
controllerThe FlexRay uses adual channel serial, fixed time base multi-master
communication protocol with communication rates of10megabits persecond
(Mbps) perchannel.
FTU FlexRay Transfer Unit The FTU isadedicated transfer unitfortheFlexRay communication interface
controller. The FTU hasanative interface totheFlexRay message RAM andis
used totransfer data to/from theFlexRay message RAM from /toanother
region inthedevice memory-map. The FTU isabusmaster inthisdevice.
GIO General-purpose Input/Output The GIO module allows upto16terminals tobeused asgeneral-purpose Input
orOutput. Each ofthese arealso capable ofgenerating aninterrupt totheCPU.

<!-- Page 117 -->

www.ti.com Introduction
117 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-1.Definition ofTerms (continued)
Acronym/Term FullForm Description
HTUx High-end timer Transfer Unit The HTU isadedicated transfer unitfortheNew Enhanced High-End Timer
module. The HTU hasanative interface totheN2HET RAM, andisused to
transfer data to/from theN2HET RAM from /toanother region inthedevice
memory-map. There isoneHTU perN2HET module, sothatthere are2HTU
modules onthedevice. The HTUx arebusmasters inthisdevice.
I2Cx Inter-Integrated Circuit
controllerThe I2Cmodule isamulti-master communication module providing aninterface
between thedevice andanI2C-compatible device viatheI2Cserial bus. The
I2Csupports both 100Kbps and400Kbps speeds.
IOMM IOMultiplexing Module This module controls themultiplexing onthedevice I/Os. Multiple functions can
bemultiplexed onto thesame device IO.Through IOMM module, user can
enable aspecific function onto adevice pin.
LINx Local Interconnect Network
controllerThe LINmodule supports theLocal Interconnect standard revision 2.1andcan
beused asaUART infull-duplex mode using thestandard Non-Return-to-Zero
(NRZ) format.
Lockstep - This isthemode ofoperation ofthedual ARM Cortex-R5F CPUs. The outputs of
thetwoCPUs arecompared oneach CPU clock cycle. Any miscompare is
flagged asanerror ofthehighest severity level. Inaddition tothelockstep
CPUs, thetwoVector Interrupt Module (VIM) arealso inlockstep.
MibSPIx Multi-Buffered Serial
Peripheral InterfaceThe MibSPIx modules also support thestandard SPIcommunication protocol.
The transfers areallgrouped intotransfer chunks called "transfer groups ".These
transfer groups aremade upofoneoremore buffers intheMibSPIx RAM. The
RAM isused tohold thecontrol information anddata tobetransmitted, aswell
asthestatus information anddata thatisreceived. There arefiveMibSPI
modules inthisdevice.
N2HETx New Enhanced High-End
TimerThe N2HET isanadvanced intelligent timer thatprovides sophisticated timing
functions forreal-time applications. The timer issoftware-controlled, using a
reduced instruction set,with aspecialized timer micromachine andanattached
I/Oport. The N2HET canbeused forpulse width modulated outputs, capture or
compare inputs, orgeneral-purpose I/O.
NMPUx Enhanced Memory Protection
UnitThere arethree standalone NMPUs onthisdevice protecting memory
transactions initiated byDMA, EMAC andother masters onto theresources on
thedevice. Inthisdevice, alltransactions initiated bynon-CPU masters willgo
through twolevels ofMPU protection. The twolevels canbeacombination of
twoNMPU inseries oronestandalone NMPU andonebuild-in MPU aspart of
themaster. One NMPU isdedicated totheDMA port connecting totheCPU
Interconnect Subsystem asthesecond level protection while thebuilt-in MPU
inside theDMA acts asthefirstlevel protection. HTUx andFTU allhave their
built-in MPU acting asthefirstlevel protection. Allaccesses initiated bythe
masters onthePeripheral Interconnect Subsystem side willfunnel through
another NMPU sitting inbetween thepath connecting thePeripheral
Interconnect Subsystem totheCPU Interconnect Subsystem. This willactasthe
second level protection forHTUx ,FTU andEMAC. EMAC does nothave the
built-in MPU andhence astandalone NMPU isinstantiated between theEMAC
andtheinterconnect.
Peripheral
Interconnect
SubsystemPeripheral Side Switched
Central Resource ControllerThis isoneofthetwomain SCRs inthedevice. Itarbitrates between the
accesses from multiple busmasters tothebusslaves using around robin
priority scheme.
PCRx Peripheral Central Resource
controllerThe PCR manages theaccesses totheperipheral registers andperipheral
memories. Itprovides aglobal reset foralltheperipherals. Italso supports the
capability toselectively enable ordisable theclock foreach peripheral
individually. The PCR also manages theaccesses tothesystem module
registers required toconfigure thedevice 'sclocks, interrupts, andsoon.The
system module registers also include status flags forindicating exception
conditions -resets, aborts, errors, interrupts. This device hasthree PCR
modules with each capable toaccess different peripherals asshown intheblock
diagram. The three PCRs areslaves tothePeripheral Interconnect Subsystem.
PMM Power Management Module This module controls theclock gating tothevarious logic power domains inthe
device. Through PMM, user canplace apower domain among Active, IdleorOff
modes. This device does notimplement physical power domains inwhich power
canbeturned off.Trying toturn offapower domain hasnoeffect onthisdevice
interms ofpower consumption butclocks willbegated offtoremove dynamic
power. IdleandOffmodes inthisdevice behave thesame from power
consumption perspective.

<!-- Page 118 -->

Introduction www.ti.com
118 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-1.Definition ofTerms (continued)
Acronym/Term FullForm Description
POM Parameter Overlay Module The parameter overlay module redirects accesses toaprogrammable region in
flash memory (read-only) toaRAM memory, either on-chip orviatheexternal
memory interface (EMIF). This allows auser toevaluate theimpact ofchanging
values ofconstants stored intheflash memory without actually having toerase
andreprogram theflash. The POM isalso abusmaster inthisdevice.
PS_SCR_M Peripheral SCR Master Port Alltransactions toaccess theresources intheCPU Interconnect Subsystem by
HTUx, FTU, EMAC, DMM andDAP willfunnel through thePS_SCR_S slave
port onthePeripheral Interconnect Subsystem. The PS_SCR_S slave isthen
connected tothePS_SCR_M master port ontheCPU Interconnect Subsystem
viaaNMPU inbetween.
RTI Real Time Interrupt module RTImodule provides timer functionality foroperating systems andfor
benchmarking code. The module incorporates several counters, which define the
timebases needed forscheduling intheoperating system.
SCIx Serial Communication
InterfaceThe SCImodule supports thestandard UART infull-duplex mode using the
standard Non-Return-to-Zero (NRZ) format.
SCM SCR Control Module This module isused tochange certain configurations such astimeout counters
oftheCPU Interconnect Subsystem. This module isalso used toinitiate selftest
fortheCPU Interconnect Subsystem.
SDC MMR Safety Diagnostic Checker
Memory-Map Register Port
forCPU Interconnect
SubsystemThere arememory-mapped status registers torecord both therun-time andself-
testdiagnostic oftheCPU Interconnect Subsystem. These registers are
accessed viatheSDC MMR slave port inthePeripheral Interconnect
Subsystem.
SRAM Level 2Static RAM There isoneslave port toaccess theon-chip SRAM.
STCx Selftest Controller There aretwoSTC modules inthisdevice. One isused totesttheCPU
subsystem including both CPU cores and/or theACP component using the
Deterministic Logic BistController asthetestengine. The other STC isused to
testeither orboth theN2HETs inthedevice.
SYS System Module This module contains thehousekeeping logic tocontrol andlogoverall system
functions andstatus such assetting uptheclock sources, clock domains,
generation andreception ofreset sources.
µSCU Micro Snooping Control Unit The µSCU which ispart oftheCortex-R5 processor system contains anACP
(Accelerator Coherency Port) interface which provides snoop capabilities on
write-transactions coming from thenon-CPU masters. Transactions arereceived
ontheACP-S slave port, andtransmitted onthememory system viatheACP-M
master port. The ACP automatically invalidates theappropriate Level 1data-
cache lines attheappropriate time, allowing software maintenance free cache
coherency fordata inwrite-through cache regions, aswell asnon-cached.
VIM Vectored Interrupt Manager VIM provides hardware assistance forprioritizing andcontrolling themany
interrupt sources present onadevice. There aretwoVIMs inthisdevice. When
thedevice isconfigured inlockstep mode, thetwoVIMs arealso inlockstep.
The outputs ofthetwoVIMs arecompared cycle bycycle bytheCCM-R5
module.
2.1.3 Bus Master /Slave Access Privileges
This device implements some restrictions onthebusslave access privileges inorder toimprove the
overall throughput oftheinterconnect shown inFigure 2-1.Table 4-1shows theimplemented point to
point connections between themasters andslaves connected tothePeripheral Interconnect Subsystem.
Table 4-3lists theimplemented point topoint connections between themasters andslaves connected to
theCPU Interconnect Subsystem.
2.1.4 CPU Interconnect Subsystem SDC MMR Port
The CPU Interconnect Subsystem SDC MMR Port isaspecial slave tothePeripheral Interconnect
Subsystem. Itismemory-mapped atstarting address ofFA00 0000h. Various status registers pertaining to
thediagnostics oftheCPU Interconnect Subsystem canbeaccess through thisslave port. The CPU
Interconnect Subsystem contains built-in hardware diagnostic checkers which willconstantly watch
transactions flowing through theinterconnect. There isachecker foreach master andslave attached to
theCPU Interconnect Subsystem. The checker checks theexpected behavior against thegenerated

<!-- Page 119 -->

ID Decode Addr DecodeMasterID Address/Control
4MasterID Protection Register NPeripheral Select N0
1
2
13
14
15
PCRx
www.ti.com Introduction
119 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecturebehavior bytheinterconnect. Forexample, iftheCPU issues aburst read request totheflash, the
checker willensure thattheexpected behavior isindeed aburst read request totheproper slave module.
Iftheinterconnect generates atransaction which isnotaread, ornotaburst ornottotheflash asthe
destination, then thechecker willflagitinoneoftheregisters. The detected error willalso besignaled to
theESM module. Table 4-2lists theCPU Interconnect Subsystem SDC register bitfield mapping.
2.1.5 Interconnect Subsystem Runtime Status
Other than theruntime checker status asdescribed inSection 2.1.4 ,theCPU Interconnect Subsystems
andthePeripheral Interconnect Subsystem inthemicrocontroller also generates several status onto the
system thatarecaptured intheSCM (SCR Control Module). Table 4-4lists theSCM register bitmapping.
2.1.6 Master IDtoPCRx
The master IDassociated with each master port onthePeripheral Interconnect Subsystem contains a4-
bitvalue. The master IDispassed along with theaddress andcontrol signals tothree PCR modules. PCR
decodes theaddress andcontrol signals toselect theperipheral. Inaddition, itdecodes this4-bit master
IDvalue toperform memory protection. With 4-bit ofmaster ID,itallows thePCR todistinguish among 16
different masters toallow ordis-allow access toagiven peripheral. Associated with each peripheral a16-
bitMaster IDaccess protection register isdefined. Each bitgrants ordenies thepermission ofthe
corresponding binary coded decimal masterID. Forexample, ifbit5oftheaccess permission register is
set,itgrants master ID5toaccess theperipheral. Ifbit7isclear, itdenies master ID7toaccess the
peripheral. Figure 2-2illustrates theMaster-ID filtering scheme. The master IDofeach master thatis
capable ofaccessing thePCRx islisted inTable 4-1.Also seeSection 2.5.3 fordetails ontheregisters
definition.
Figure 2-2.PCR MasterID Filtering

<!-- Page 120 -->

Memory Organization www.ti.com
120 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2 Memory Organization
2.2.1 Memory-Map Overview
The Cortex-R5F uses a32-bit address bus, giving itaccess toamemory space of4GB. This space is
divided intoseveral regions, each addressed bydifferent memory selects. Figure 2-3shows thememory-
map ofthemicrocontroller.
The main flash instruction memory isaddressed starting at0x00000000 bydefault. This isalso thereset
vector location -theARM Cortex-R5F processor core starts execution from thereset vector address of
0x00000000 whenever thecore gets reset.
The CPU data RAM isaddressed starting at0x08000000 bydefault.
The device also supports theswapping oftheCPU instruction memory (flash) anddata memory (RAM).
This canbedone byconfiguring theMEM SWAP field oftheBus Matrix Module Control Register 1
(BMMCR1).
After swapping, thedata RAM isaccessed starting from 0x00000000 andtheRAM ECC locations are
accessed starting from 0x00400000. The flash memory isnow accessed starting from 0x08000000.
NOTE: After theswap with theflash memory-mapped to0x08000000, only 512kB oftheflash
memory from 0x08000000 to0x0807FFFF willbeaccessible bythebusmasters.

<!-- Page 121 -->

Flash (4MB)RAM (512kB)
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
0x800000000x9FFFFFFF
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
R5F-0 Cache0x300000000x33FFFFFF
RESERVED
www.ti.com Memory Organization
121 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureFigure 2-3.Memory-Map

<!-- Page 122 -->

Memory Organization www.ti.com
122 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2.2 Memory-Map Table
The control andstatus registers foreach module aremapped within theCPU 's4GB memory space. Some
modules also have associated memories, which arealso mapped within thisspace.
Table 2-2shows thestarting andending addresses ofeach module 'sregister frame andanyassociated
memory. The table also shows theresponse generated bythemodule ortheinterconnect whenever an
access ismade toanunimplemented location inside theregister ormemory frame.
Table 2-2.Module Registers /Memories Memory-Map
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
Level 2Memories
Level 2Flash Data
Space0x0000_0000 0x003F_FFFF 4MB 4MB Abort
Level 2SRAM 0x0800_0000 0x083F_FFFF 4MB 512kB Abort
Level 2SRAM ECC 0x0840_0000 0x087F_FFFF 4MB 512kB
Accelerator Coherency Port
Accelerator
Coherency Port0x0800_0000 0x087F_FFFF 8MB 512kB Abort
Level 1Cache Memories
Cortex-R5F Data
Cache Memory0x3000_0000 0x30FF_FFFF 16MB 32kB Abort
Cortex-R5F
Instruction Cache
Memory0x3100_0000 0x31FF_FFFF 16MB 32kB
External Memory Accesses
EMIF Chip Select 2
(asynchronous)0x6000_0000 0x63FF_FFFF 64MB 16MB Access to
Reserved space
EMIF Chip Select 3
(asynchronous)0x6400_0000 0x67FF_FFFF 64MB 16MB Generates Abort
EMIF Chip Select 4
(asynchronous)0x6800_0000 0x6BFF_FFFF 64MB 16MB
EMIF Chip Select 0
(synchronous)0x8000_0000 0x87FF_FFFF 128MB 128MB
Flash OTP, ECC, EEPROM Bank
Customer OTP,
Bank00xF000_0000 0xF000_1FFF 8kB 4kB Abort
Customer OTP,
Bank10xF000_2000 0xF000_3FFF 8kB 4kB Abort
Customer OTP,
EEPROM Bank0xF000_E000 0xF000_FFFF 8kB 1kB Abort
Customer OTP-ECC,
Bank00xF004_0000 0xF004_03FF 1kB 512B Abort
Customer OTP-ECC,
Bank10xF004_0400 0xF004_07FF 1kB 512B Abort
Customer OTP-ECC,
EEPROM Bank0xF004_1C00 0xF004_1FFF 1kB 128B Abort
TIOTP, Bank0 0xF008_0000 0xF008_1FFF 8kB 4kB Abort
TIOTP, Bank1 0xF008_2000 0xF008_3FFF 8kB 4kB Abort
TIOTP, EEPROM
Bank0xF008_E000 0xF008_FFFF 8kB 1kB Abort
TIOTP-ECC, Bank0 0xF00C_0000 0xF00C_03FF 1kB 512B Abort
TIOTP-ECC, Bank1 0xF00C_0400 0xF00C_07FF 1kB 512B Abort

<!-- Page 123 -->

www.ti.com Memory Organization
123 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
TIOTP-ECC,
EEPROM Bank0xF00C_1C00 0xF00C_1FFF 1kB 128B Abort
EEPROM Bank-ECC 0xF010_0000 0xF01F_FFFF 1MB 16kB Abort
EEPROM Bank 0xF020_0000 0xF03F_FFFF 2MB 128kB Abort
Flash Data Space
ECC0xF040_0000 0xF05F_FFFF 2MB 512kB Abort
Interconnect SDC MMR
Interconnect SDC
MMR0xFA00_0000 0xFAFF_FFFF 16MB 16MB
Registers/Memories under PCR2 (Peripheral Segment 2)
CPPI Memory Slave
(Ethernet RAM)PCS[41] 0xFC52_0000 0xFC52_1FFF 8kB 8kB Abort
CPGMAC Slave
(Ethernet Slave)PS[30]-PS[31] 0xFCF7_8000 0xFCF7_87FF 2kB 2kB NoError
CPGMACSS Wrapper
(Ethernet Wrapper)PS[29] 0xFCF7_8800 0xFCF7_88FF 256B 256B NoError
Ethernet MDIO
InterfacePS[29] 0xFCF7_8900 0xFCF7_89FF 256B 256B NoError
ePWM1 PS[28] 0xFCF7_8C00 0xFCF7_8CFF 256B 256B Abort
ePWM2 0xFCF7_8D00 0xFCF7_8DFF 256B 256B Abort
ePWM3 0xFCF7_8E00 0xFCF7_8EFF 256B 256B Abort
ePWM4 0xFCF7_8F00 0xFCF7_8FFF 256B 256B Abort
ePWM5 PS[27] 0xFCF7_9000 0xFCF7_90FF 256B 256B Abort
ePWM6 0xFCF7_9100 0xFCF7_91FF 256B 256B Abort
ePWM7 0xFCF7_9200 0xFCF7_92FF 256B 256B Abort
eCAP1 0xFCF7_9300 0xFCF7_93FF 256B 256B Abort
eCAP2 PS[26] 0xFCF7_9400 0xFCF7_94FF 256B 256B Abort
eCAP3 0xFCF7_9500 0xFCF7_95FF 256B 256B Abort
eCAP4 0xFCF7_9600 0xFCF7_96FF 256B 256B Abort
eCAP5 0xFCF7_9700 0xFCF7_97FF 256B 256B Abort
eCAP6 PS[25] 0xFCF7_9800 0xFCF7_98FF 256B 256B Abort
eQEP1 0xFCF7_9900 0xFCF7_99FF 256B 256B Abort
eQEP2 0xFCF7_9A00 0xFCF7_9AFF 256B 256B Abort
PCR2 registers PPSE[4]-
PPSE[5]0xFCFF_1000 0xFCFF_17FF 2kB 2kB
NMPU (CPGMAC) PPSE[6] 0xFCFF_1800 0xFCFF_18FF 512B 512B Abort
EMIF Registers PPS[2] 0xFCFF_E800 0xFCFF_E8FF 256B 256B Abort
Cyclic Redundancy Checker (CRC) Module Register Frame
CRC 1 0xFE00_0000 0xFEFF_FFFF 16MB 512kB Accesses above
0xFE000200
generate abort.
CRC2 0xFB00_0000 0xFBFF_FFFF 16MB 512kB Accesses above
0xFB000200
generate abort.
Memories under User PCR3 (Peripheral Segment 3)
MIBSPI5 RAM PCS[5] 0xFF0A_0000 0xFF0B_FFFF 128kB 2kB Abort for
accesses above
2KB

<!-- Page 124 -->

Memory Organization www.ti.com
124 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
MIBSPI4 RAM PCS[3] 0xFF06_0000 0xFF07_FFFF 128kB 2kB Abort for
accesses above
2KB
MIBSPI3 RAM PCS[6] 0xFF0C_0000 0xFF0D_FFFF 128kB 2kB Abort for
accesses above
2KB
MIBSPI2 RAM PCS[4] 0xFF08_0000 0xFF09_FFFF 128kB 2kB Abort for
accesses above
2KB
MIBSPI1 RAM PCS[7] 0xFF0E_0000 0xFF0F_FFFF 128kB 4kB Abort for
accesses above
4KB
DCAN4 RAM PCS[12] 0xFF18_0000 0xFF19_FFFF 128kB 8kB Abort generated
foraccesses
beyond offset
0x2000
DCAN3 RAM PCS[13] 0xFF1A_0000 0xFF1B_FFFF 128kB 8kB Abort generated
foraccesses
beyond offset
0x2000
DCAN2 RAM PCS[14] 0xFF1C_0000 0xFF1D_FFFF 128kB 8kB Abort generated
foraccesses
beyond offset
0x2000
DCAN1 RAM PCS[15] 0xFF1E_0000 0xFF1F_FFFF 128kB 8kB Abort generated
foraccesses
beyond offset
0x2000.
MIBADC2 RAM PCS[29] 0xFF3A_0000 0xFF3B_FFFF 128kB 8kB Wrap around for
accesses to
unimplemented
address offsets
lower than
0x1FFF.
MIBADC2 Look-UP
Table384bytes Look-Up Table
forADC2
wrapper. Starts
ataddress offset
0x2000 and
ends ataddress
offset 0x217F.
Wrap around for
accesses
between offsets
0x0180 and
0x3FFF. Abort
generation for
accesses
beyond offset
0x4000.

<!-- Page 125 -->

www.ti.com Memory Organization
125 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
MIBADC1 RAM PCS[31] 0xFF3E_0000 0xFF3F_FFFF 128kB 8kB Wrap around for
accesses to
unimplemented
address offsets
lower than
0x1FFF.
MIBADC1 Look-UP
Table384bytes Look-Up Table
forADC1
wrapper. Starts
ataddress offset
0x2000 and
ends ataddress
offset 0x217F.
Wrap around for
accesses
between offsets
0x0180 and
0x3FFF. Abort
generation for
accesses
beyond offset
0x4000.
NHET2 RAM PCS[34] 0xFF44_0000 0xFF45_FFFF 128kB 16kB Wrap around for
accesses to
unimplemented
address offsets
lower than
0x3FFF. Abort
generated for
accesses
beyond 0x3FFF.
NHET1 RAM PCS[35] 0xFF46_0000 0xFF47_FFFF 128kB 16kB Wrap around for
accesses to
unimplemented
address offsets
lower than
0x3FFF. Abort
generated for
accesses
beyond 0x3FFF.
HET TU2 RAM PCS[38] 0xFF4C_0000 0xFF4D_FFFF 128kB 1kB Abort
HET TU1 RAM PCS[39] 0xFF4E_0000 0xFF4F_FFFF 128kB 1kB Abort
FlexRay TURAM PCS[40] 0xFF50_0000 0xFF51_FFFF 128kB 1kB Abort
Coresight Debug Components
CoreSight Debug
ROMCSCS[0] 0xFFA0_0000 0xFFA0_0FFF 4kB 4kB Reads return
zeros, writes
have noeffect
Cortex-R5F Debug CSCS[1] 0xFFA0_1000 0xFFA0_1FFF 4kB 4kB Reads return
zeros, writes
have noeffect
ETM-R5 CSCS[2] 0xFFA0_2000 0xFFA0_2FFF 4kB 4kB Reads return
zeros, writes
have noeffect
CoreSight TPIU CSCS[3] 0xFFA0_3000 0xFFA0_3FFF 4kB 4kB Reads return
zeros, writes
have noeffect
POM CSCS[4] 0xFFA0_4000 0xFFA0_4FFF 4kB 4kB Reads return
zeros, writes
have noeffect

<!-- Page 126 -->

Memory Organization www.ti.com
126 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
CTI1 CSCS[7] 0xFFA0_7000 0xFFA0_7FFF 4kB 4kB Reads return
zeros, writes
have noeffect
CTI2 CSCS[8] 0xFFA0_8000 0xFFA0_8FFF 4kB 4kB Reads return
zeros, writes
have noeffect
CTI3 CSCS[9] 0xFFA0_9000 0xFFA0_9FFF 4kB 4kB Reads return
zeros, writes
have noeffect
CTI4 CSCS[10] 0xFFA0_A000 0xFFA0_AFFF 4kB 4kB Reads return
zeros, writes
have noeffect
CSTF CSCS[11] 0xFFA0_B000 0xFFA0_BFFF 4kB 4kB Reads return
zeros, writes
have noeffect
Registers under PCR3 (Peripheral Segment 3)
PCR3 registers PS[31:30] 0xFFF7_8000 0xFFF7_87FF 2kB 2kB Reads return
zeros, writes
have noeffect
FTU PS[23] 0xFFF7_A000 0xFFF7_A1FF 512B 512B Reads return
zeros, writes
have noeffect
HTU1 PS[22] 0xFFF7_A400 0xFFF7_A4FF 256B 256B Abort
HTU2 PS[22] 0xFFF7_A500 0xFFF7_A5FF 256B 256B Abort
NHET1 PS[17] 0xFFF7_B800 0xFFF7_B8FF 256B 256B Reads return
zeros, writes
have noeffect
NHET2 PS[17] 0xFFF7_B900 0xFFF7_B9FF 256B 256B Reads return
zeros, writes
have noeffect
GIO PS[16] 0xFFF7_BC00 0xFFF7_BCFF 256B 256B Reads return
zeros, writes
have noeffect
MIBADC1 PS[15] 0xFFF7_C000 0xFFF7_C1FF 512B 512B Reads return
zeros, writes
have noeffect
MIBADC2 PS[15] 0xFFF7_C200 0xFFF7_C3FF 512B 512B Reads return
zeros, writes
have noeffect
FlexRay PS[12]+PS[13] 0xFFF7_C800 0xFFF7_CFFF 2kB 2kB Reads return
zeros, writes
have noeffect
I2C1 PS[10] 0xFFF7_D400 0xFFF7_D4FF 256B 256B Reads return
zeros, writes
have noeffect
I2C2 PS[10] 0xFFF7_D500 0xFFF7_D5FF 256B 256B Reads return
zeros, writes
have noeffect
DCAN1 PS[8] 0xFFF7_DC00 0xFFF7_DDFF 512B 512B Reads return
zeros, writes
have noeffect
DCAN2 PS[8] 0xFFF7_DE00 0xFFF7_DFFF 512B 512B Reads return
zeros, writes
have noeffect

<!-- Page 127 -->

www.ti.com Memory Organization
127 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
DCAN3 PS[7] 0xFFF7_E000 0xFFF7_E1FF 512B 512B Reads return
zeros, writes
have noeffect
DCAN4 PS[7] 0xFFF7_E200 0xFFF7_E3FF 512B 512B Reads return
zeros, writes
have noeffect
LIN1 PS[6] 0xFFF7_E400 0xFFF7_E4FF 256B 256B Reads return
zeros, writes
have noeffect
SCI3 PS[6] 0xFFF7_E500 0xFFF7_E5FF 256B 256B Reads return
zeros, writes
have noeffect
LIN2 PS[6] 0xFFF7_E600 0xFFF7_E6FF 256B 256B Reads return
zeros, writes
have noeffect
SCI4 PS[6] 0xFFF7_E700 0xFFF7_E7FF 256B 256B Reads return
zeros, writes
have noeffect
MibSPI1 PS[2] 0xFFF7_F400 0xFFF7_F5FF 512B 512B Reads return
zeros, writes
have noeffect
MibSPI2 PS[2] 0xFFF7_F600 0xFFF7_F7FF 512B 512B Reads return
zeros, writes
have noeffect
MibSPI3 PS[1] 0xFFF7_F800 0xFFF7_F9FF 512B 512B Reads return
zeros, writes
have noeffect
MibSPI4 PS[1] 0xFFF7_FA00 0xFFF7_FBFF 512B 512B Reads return
zeros, writes
have noeffect
MibSPI5 PS[0] 0xFFF7_FC00 0xFFF7_FDFF 512B 512B Reads return
zeros, writes
have noeffect
System Modules Control Registers andMemories under PCR1 (Peripheral Segment 1)
DMA RAM PPCS[0] 0xFFF8_0000 0xFFF8_0FFF 4kB 4kB Abort
VIM RAM PPCS[2] 0xFFF8_2000 0xFFF8_2FFF 4kB 4kB Wrap around for
accesses to
unimplemented
address offsets
lower than
0x2FFF.
RTP RAM PPCS[3] 0xFFF8_3000 0xFFF8_3FFF 4kB 4kB Abort
Flash Wrapper PPCS[7] 0xFFF8_7000 0xFFF8_7FFF 4kB 4kB Abort
eFuse Farm
ControllerPPCS[12] 0xFFF8_C000 0xFFF8_CFFF 4kB 4kB Abort
Power Domain
Control (PMM)PPSE[0] 0xFFFF_0000 0xFFFF_01FF 512B 512B Abort
FMTM
Note: This module is
only used byTIduring
testPPSE[1] 0xFFFF_0400 0xFFFF_05FF 512B 512B Reads return
zeros, writes
have noeffect
STC2 (NHET1/2) PPSE[2] 0xFFFF_0800 0xFFFF_08FF 256B 256B Reads return
zeros, writes
have noeffect
SCM PPSE[2] 0xFFFF_0A00 0xFFFF_0AFF 256B 256B Abort

<!-- Page 128 -->

Memory Organization www.ti.com
128 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-2.Module Registers /Memories Memory-Map (continued)
Target NameMemory
SelectAddress Range
Frame Size Actual SizeResponse for
Access to
Unimplemented
Locations in
Frame Start End
EPC PPSE[3] 0xFFFF_0C00 0xFFFF_0FFF 1kB 1kB Abort
PCR1 registers PPSE[4]-
PPSE[5]0xFFFF_1000 0xFFFF_17FF 2kB 2kB Reads return
zeros, writes
have noeffect
NMPU (PS_SCR_S) PPSE[6] 0xFFFF_1800 0xFFFF_19FF 512B 512B Abort
NMPU (DMA Port A) PPSE[6] 0xFFFF_1A00 0xFFFF_1BFF 512B 512B Abort
PinMux Control
(IOMM)PPSE[7] 0xFFFF_1C00 0xFFFF_1FFF 2kB 1kB Reads return
zeros, writes
have noeffect
System Module -
Frame 2(see platform
architecture
specification)PPS[0] 0xFFFF_E100 0xFFFF_E1FF 256B 256B Reads return
zeros, writes
have noeffect
PBIST PPS[1] 0xFFFF_E400 0xFFFF_E5FF 512B 512B Reads return
zeros, writes
have noeffect
STC1 (Cortex-R5F) PPS[1] 0xFFFF_E600 0xFFFF_E6FF 256B 256B Reads return
zeros, writes
have noeffect
DCC1 PPS[3] 0xFFFF_EC00 0xFFFF_ECFF 256B 256B Reads return
zeros, writes
have noeffect
DMA PPS[4] 0xFFFF_F000 0xFFFF_F3FF 1kB 1kB Abort
DCC2 PPS[5] 0xFFFF_F400 0xFFFF_F4FF 256B 256B Reads return
zeros, writes
have noeffect
ESM register PPS[5] 0xFFFF_F500 0xFFFF_F5FF 256B 256B Reads return
zeros, writes
have noeffect
CCM-R5 PPS[5] 0xFFFF_F600 0xFFFF_F6FF 256B 256B Reads return
zeros, writes
have noeffect
DMM PPS[5] 0xFFFF_F700 0xFFFF_F7FF 256B 256B Reads return
zeros, writes
have noeffect
L2RAMW PPS[6] 0xFFFF_F900 0xFFFF_F9FF 256B 256B Abort
RTP PPS[6] 0xFFFF_FA00 0xFFFF_FAFF 256B 256B Reads return
zeros, writes
have noeffect
RTI+DWWD PPS[7] 0xFFFF_FC00 0xFFFF_FCFF 256B 256B Reads return
zeros, writes
have noeffect
VIM PPS[7] 0xFFFF_FD00 0xFFFF_FEFF 512B 512B Reads return
zeros, writes
have noeffect
System Module -
Frame 1(see platform
architecture
specification)PPS[7] 0xFFFF_FF00 0xFFFF_FFFF 256B 256B Reads return
zeros, writes
have noeffect

<!-- Page 129 -->

www.ti.com Memory Organization
129 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2.3 Flash onMicrocontrollers
The TMS570LC43x microcontrollers support upto4MBofflash foruseasprogram memory. The
microcontrollers also support aseparate 128kB flash bank foruseasemulated EEPROM.
Refer tothedevice data manual forelectrical andtiming specifications related totheflash module.
2.2.3.1 Flash Bank Sectoring Configuration
The bank isdivided intomultiple sectors. Aflash sector isthesmallest region intheflash bank thatmust
beerased. The sectoring configuration ofeach flash bank isshown inTable 2-3.
*The Flash banks are288-bit wide bank with ECC support.
*The flash bank7 canbeprogrammed while executing code from flash bank0.
*Code execution isnotallowed from flash bank7.

<!-- Page 130 -->

Memory Organization www.ti.com
130 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-3.Flash Memory Banks andSectors
Sector Number Sector Size Low Address High Address
Bank 0:2.0MBytes
0 16K Bytes 0x0000_0000 0x0000_3FFF
1 16K Bytes 0x0000_4000 0x0000_7FFF
2 16K Bytes 0x0000_8000 0x0000_BFFF
3 16K Bytes 0x0000_C000 0x0000_FFFF
4 16K Bytes 0x0001_0000 0x0001_3FFF
5 16K Bytes 0x0001_4000 0x0001_7FFF
6 32K Bytes 0x0001_8000 0x0001_FFFF
7 128K Bytes 0x0002_0000 0x0003_FFFF
8 128K Bytes 0x0004_0000 0x0005_FFFF
9 128K Bytes 0x0006_0000 0x0007_FFFF
10 256K Bytes 0x0008_0000 0x000B_FFFF
11 256K Bytes 0x000C_0000 0x000F_FFFF
12 256K Bytes 0x0010_0000 0x0013_FFFF
13 256K Bytes 0x0014_0000 0x0017_FFFF
14 256K Bytes 0x0018_0000 0x001B_FFFF
15 256K Bytes 0x001C_0000 0x001F_FFFF
Bank 1:2.0Mbytes
0 128K Bytes 0x0020_0000 0x0021_FFFF
1 128K Bytes 0x0022_0000 0x0023_FFFF
2 128K Bytes 0x0024_0000 0x0025_FFFF
3 128K Bytes 0x0026_0000 0x0027_FFFF
4 128K Bytes 0x0028_0000 0x0029_FFFF
5 128K Bytes 0x002A_0000 0x002B_FFFF
6 128K Bytes 0x002C_0000 0x002D_FFFF
7 128K Bytes 0x002E_0000 0x002F_FFFF
8 128K Bytes 0x0030_0000 0x0031_FFFF
9 128K Bytes 0x0032_0000 0x0033_FFFF
10 128K Bytes 0x0034_0000 0x0035_FFFF
11 128K Bytes 0x0036_0000 0x0037_FFFF
12 128K Bytes 0x0038_0000 0x0039_FFFF
13 128K Bytes 0x003A_0000 0x003B_FFFF
14 128K Bytes 0x003C_0000 0x003D_FFFF
15 128K Bytes 0x003E_0000 0x003F_FFFF
Bank 7:128kBytes
0 4KBytes 0xF020_0000 0xF020_0FFF
: : : :
31 4KBytes 0xF021_F000 0xF021_FFFF

<!-- Page 131 -->

www.ti.com Memory Organization
131 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2.3.2 ECC Protection forFlash Accesses
The TMS570LC43x microcontrollers protect allaccesses totheon-chip level 2flash memory bydedicated
Single-Bit Error Correction Double-Bit Error Detection (SECDED) logic.
The access totheprogram memory -flash bank 0,1and7areprotected bySECDED logic implemented
inside theARM Cortex-R5F CPU.
The SECDED logic implementation uses Error Correction Codes (ECC) forcorrecting single-bit errors and
fordetecting multiple-bit errors inthevalues read from theflash arrays. There isan8-bit ECC forevery 64
bitsofdata. The ECC forthelevel 2flash memory contents needs tobecalculated byanexternal tool
such asnowECC. The ECC canthen beprogrammed intotheflash array along with theactual application
code.
The ECC fortheflash array isstored intheflash itself, andismapped toaregion starting at0xF0400000
forthemain flash bank 0and1,andtoaregion starting at0xF0100000 fortheEEPROM emulation flash
bank 7.
NOTE: The SECDED logic inside theCPU ispermanently enabled fortheAXI-M andAXI-S
interfaces.
Code Example forEnabling ECC Protection forMain Flash Accesses:
When theCPU detects anECC single-, ordouble-bit error onaread from theflash memory, itsignals this
onadedicated "Event "bus. This event bussignaling isalso notenabled bydefault andmust beenabled
bytheapplication. The below code example canbeused toenable theCPU event signaling.
MRC p15,#0,r1,c9,c12,#0 ;Enabling Event monitor states
ORR r1, r1, #0x00000010
MCR p15,#0,r1,c9,c12,#0 ;Set 4th bit ('X') ofPMNC register
MRC p15,#0,r1,c9,c12,#0
The ECC error events exported onto theEvent busisfirstcaptured bytheError Profiling Controller (EPC)
module andinturn generates error signals thatareinput tothecentral Error Signaling Module (ESM).
2.2.3.3 Error Profiling Module (EPC)
The main idea ofEPC istoenable thesystem totolerate acertain amount ofECC correctable errors on
thesame address repeatedly inthememory system with minimal runtime overhead. EPC willrecord
different single-bit error addresses inaContent Addressable Memory (CAM). Ifacorrectable ECC error is
generated onarepeating address, theEPC willnotraise anerror toESM module. This tolerance avoids
theapplication tohandle thesame error when thecode isinarepeating loop. There are4correctable error
interfaces implemented inEPC tocapture correctable errors from 4different sources. There arealso 2
uncorrectable error interfaces implemented inEPC tocapture uncorrectable errors from 2different
sources. Main features ofEPC are:
*Capture theaddresses ofthecorrectable ECC faults from different sources such asCPU cores, L2
SRAM andinterconnect intoa32-entry CAM (Content Addressable Memory)
*Forcorrectable faults, theerror handling depends onthefollowing conditions:
-iftheincoming address isalready inthe32-entry CAM, discard thefail.Noerror generated toESM.
-iftheaddress isnotintheCAM list,andtheCAM hasempty entries, addtheaddress intotheCAM
list.Inaddition, raise theerror signal totheESM group 1ifenabled.
-iftheaddress isnotintheCAM list,andtheCAM hasnoempty entries, always raise theerror
signal totheESM group 1.
*A4-entry FIFO tostore thecorrectable error events andaddresses foreach channel interface.
*Foruncorrectable faults, capture theaddress andassert error signal totheESM group 2.
Each EPC interface corresponds toabitfield insome oftheEPC registers. Table 2-4shows only those
registers thatassociate thebitstoaspecific interface forthisdevice. See EPC chapter forthefulllistof
registers.

<!-- Page 132 -->

CAMFIFO
FIFO
FIFO
FIFOCPU0□Correctable□Error
CPU□SCR□Correctable□ECC□for□DMA I/F
CPU□□SCR□Correctable□ECC□for□PS_SCR_M□I/F
L2RAMW□RMW□Correctable□ErrorLookupFSM
Err□Gen
Err□Stat
ESM Correctable□Error□Capture□Block
CPU□SCR□Uncorrectable□ECC□for□DMA I/F
CPU□SCR□Uncorrectable□ECC□for□PS_SCR_M□I/FErr□Gen
Uncorrectable□Error□Capture□BlockUERR Addr□Reg
UERR Addr□RegErr□Stat
Err□Stat
EPC□ModuleCorrectable□Error□Event□Source
Unorrectable□Error□Event□Sourcech0
ch2
ch3
ch4
ch0
ch1
Memory Organization www.ti.com
132 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureFigure 2-4.EPC Integration Diagram

<!-- Page 133 -->

www.ti.com Memory Organization
133 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-4.EPC Registers BitMapping
Address
OffsetRegister Name Bit# Error Source Remark
8h UERRSTAT0Uncorrectable ECC for
DMA interface*Bitassociates with theUncorrectable ECC error
detected bytheCPU Interconnect Subsystem
fortheDMA interface
*See Interconnect chapter fordetails onthe
ECC generation andevaluation forDMA
interface
1Uncorrectable ECC for
PS_SCR_M interface*Bitassociates with theUncorrectable ECC error
detected bytheCPU Interconnect Subsystem
forthePS_SCR_M interface
*See Interconnect chapter fordetails onthe
ECC generation andevaluation forDMA
interface
10h FIFOFULLSTAT0 CPU Correctable ECC error*Bitassociates with theFIFO fullstatus forthe
interface thatisused tocapture theCPU
correctable error event
*Correctable error event exported byCPU 's
event bus.
1 Reserved
2Correctable ECC for
DMA interface*Bitassociates with theFIFO fullstatus forthe
interface thatisused tocapture theDMA
correctable error event
*Correctable error event detected bytheCPU
Interconnect Subsystem fortheDMA PortA
interface.
3Correctable ECC for
PS_SCR_M interface*Bitassociates with theFIFO fullstatus forthe
interface thatisused tocapture the
PS_SCR_M correctable error event
*Correctable error event detected bytheCPU
Interconnect Subsystem forthePS_SCR_M
interface.
4Correctable ECC error from L2
SRAM*Bitassociates with theFIFO fullstatus forthe
interface thatisused tocapture theL2SRAM
correctable error event
*Correctable error event detected bytheL2
SRAM wrapper during theread phase ofa
Read-Modify-Write operation duetoaless than
64-bit write from thebusmaster.
14h OVRFLWSTAT0 CPU Correctable ECC error*Bitassociates with theFIFO overflow status for
theinterface thatisused tocapture theCPU
correctable error event
1 Reserved
2Correctable ECC for
DMA interface*Bitassociates with theFIFO overflow status for
theinterface thatisused tocapture theDMA
correctable error event
3Correctable ECC for
PS_SCR_M interface*Bitassociates with theFIFO overflow status for
theinterface thatisused tocapture the
PS_SCR_M correctable error event
4Correctable ECC error from L2
SRAM*Bitassociates with theFIFO overflow status for
theinterface thatisused tocapture theL2
SRAM correctable error event
20h UERRADDR0 31:0Uncorrectable ECC for
DMA interface*Uncorrectable error address register forthe
DMA interface
24h UERRADDR1 31:0Uncorrectable ECC for
PS_SCR_M interface*Uncorrectable error address register forthe
PS_SCR_M interface

<!-- Page 134 -->

Memory Organization www.ti.com
134 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2.4 On-Chip SRAM
Several SRAM modules areimplemented onthedevice tosupport thefunctionality ofthemodules
included.
Reads from both thelevel 1andlevel 2SRAM areprotected byECC calculated inside theCPU. Reads
from allother memories areprotected byeither theparity with configurable oddoreven parity scheme or
ECC thatisevaluated inparallel with theactual read.
The TMS570LC43x microcontrollers aretargeted towards safety-critical applications, anditiscritical for
anyfailures intheon-chip SRAM modules tobeidentified before these modules areused forsafety-critical
functions. These microcontrollers support aProgrammable Built-In Self-Test (PBIST) mechanism thatis
used totesteach on-chip SRAM module forfaults. The PBIST isusually runondevice start-up asitisa
destructive testandallcontents ofthetested SRAM module areoverwritten during thetest.
The microcontrollers also support ahardware-based auto-initialization ofon-chip SRAM modules. This
process also takes intoaccount theread protection scheme implemented foreach SRAM module -ECC
orparity.
TIrecommends thatthePBIST routines beexecuted ontheSRAM modules prior totheauto-initialization.
The following sections describe these twoprocesses.
2.2.4.1 PBIST RAM Grouping andAlgorithm Mapping ForOn-Chip SRAM Modules
Table 2-5shows thegroupings ofthevarious on-chip memories forPBIST. Italso lists thememory types
andtheir assigned RAM Group Select (RGS) andReturn Data Select (RDS). Refer tothePBIST chapter
formore details ontheusage oftheRGS andRDS information.
Table 2-5.PBIST Memory Grouping
Module RAM Group # RGS RDS Memory Type
PBIST_ROM 1 1 1 ROM
STC1_1_ROM_R5 2 14 1 ROM
STC1_2_ROM_R5 3 14 2 ROM
STC2_ROM_N2HET 4 15 1 ROM
AWM1 5 2 1 Two-port
DCAN1 6 3 1to6 Two-port
DCAN2 7 4 1to6 Two-port
DMA 8 5 1to6 Two-port
HTU1 9 6 1to6 Two-port
MIBSPI1 10 8 1to4 Two-port
MIBSPI2 11 9 1to4 Two-port
MIBSPI3 12 10 1to4 Two-port
N2HET1 13 11 1to12 Two-port
VIM 14 12 1,2 Two-port
Reserved 15 13 1,2 Two-port
RTP 16 16 1to12 Two-port
ATB 17 17 1to16 Two-port
AWM2 18 18 1 Two-port
DCAN3 19 19 1to6 Two-port
DCAN4 20 20 1to6 Two-port
HTU2 21 21 1to6 Two-port
MIBSPI4 22 22 1to4 Two-port
MIBSPI5 23 23 1to4 Two-port
N2HET2 24 24 1to12 Two-port
FTU 25 25 1 Two-port
FRAY_INBUF_OUTBUF 26 26 1to8 Two-port

<!-- Page 135 -->

www.ti.com Memory Organization
135 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-5.PBIST Memory Grouping (continued)
Module RAM Group # RGS RDS Memory Type
CPGMAC_STATE_RXADDR 27 27 1to3 Two-port
CPGMAC_STAT_FIFO 28 27 4to6 Two-port
L2RAMW 29 7 1 Single-port
6 Single-port
L2RAMW 30 32 1 Single-port
6 Single-port
11 Single-port
16 Single-port
21 Single-port
26 Single-port
R5_ICACHE 31 40 1 Single-port
6 Single-port
11 Single-port
16 Single-port
R5_DCACHE 32 41 1 Single-port
6 Single-port
11 Single-port
16 Single-port
21 Single-port
26 Single-port
Reserved 33 43 1 Single-port
6 Single-port
11 Single-port
16 Single-port
Reserved 34 44 1 Single-port
6 Single-port
11 Single-port
16 Single-port
21 Single-port
26 Single-port
FRAY_TRBUF_MSGRAM 35 26 9to11 Single-port
CPGMAC_CPPI 36 27 7 Single-port
R5_DCACHE_Dirty 37 42 2 Single-port
Reserved 38 45 2 Single-port

<!-- Page 136 -->

Memory Organization www.ti.com
136 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-6maps thedifferent algorithms supported inapplication mode fortheRAM groups. The table also
lists thebackground pattern options available foreach algorithm.
Table 2-6.PBIST Algorithm Mapping
Sr.No.ALGO
Register
Value AlgorithmMemories
Under
TestAvailable
Background
Patterns Valid RAM GroupsValid
RINFOL/RINFOU
Register Value
1 0x00000001 triple_read_slow_read ROM 1,2,3,4 0x0000000F/
0x00000000
2 0x00000002 triple_read_fast_read ROM 1,2,3,4 0x0000000F/
0x00000000
3 0x00000004 march13n Two-port 0x00000000,
0x96699669,
0x0F0F0F0F,
0xAA55AA55,
0xC3C3C3C35,6,7,8,9,10,11,12,13
,
14,16,17,18,19,20,21
,
22,23,24,25,26,27,280x0FFFBFF0/
0x00000000
4 0x00000008 march13n Single-port 0x00000000,
0x96699669,
0x0F0F0F0F,
0xAA55AA55,
0xC3C3C3C329,30,31,32,35,36,37 0xF0000000/
0x0000001C
NOTE: Recommended Memory Test Algorithm
March13 isthemost recommended algorithm forthememory self-test.
ForGCLK1 =300MHz, HCLK =150MHz, VCLK =75MHz, PBIST ROM_CLK =75MHz, theMarch13
algorithm takes about 29.08 mstorunonallon-chip SRAMs.
NOTE: PBIST ROM_CLK canbeprescaled from GCLK1 viaROM_DIV bitsoftheMSTGCR
register. The valid ratio iseither /1,/2or/4or/8.See Section 2.5.1.20 fordetail. Maximum
PBIST ROM_CLK frequency supported is82.5MHz.

<!-- Page 137 -->

VCLK
Write to enable
MINTIGENA key
Write to enable
MSINENAn
SYS_MMISTARTn
DEV_MMIDONEnMemory(where n = 31:0)
(where n = 31:0)
(where n = 31:0)Poll MINIDONE bit,
MSTCGSTAT[8]Poll MIDONEn field ofWhen each enabled module completes
corresponding MIIDONE bit is set.
After all enabled modules' hardware initialization
completes, the MINIDONE bit is set, indicating
all hardware memory initialization is done.(where n = 31:0)
module
hardware
initialization
Black indicates System register activity.
Gray indicates inter-module activity, not accessible via S ystem register.its hardware initialization, the
(from System module
to memory modules)
(from memory modules
to System module)MINISTAT register
www.ti.com Memory Organization
137 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.2.4.2 Auto-Initialization ofOn-Chip SRAM Modules
The device system provides thecapability toperform ahardware initialization onmost memories onthe
system busandontheperipheral bus. The memory used fortheFlexRay message objects isECC
protected butisnotdirectly CPU addressable, hence there isnomemory auto-initialization support forthis
memory.
The intent ofhaving thehardware initialization istoprogram thememory arrays with error detection
capability toaknown state based ontheir error detection scheme -odd/even parity orECC. Forexample,
thecontents oftheCPU level 2SRAM after power-on reset isunknown. Ahardware auto-initialization can
bestarted sothatthere isnoECC error.
NOTE: Effect ofECC orParity onMemory Auto-Initialization
The ECC orparity should beenabled ontheRAMs before hardware auto-initialization starts
ifparity orECC isbeing used.
Auto-Initialization Sequence:
1.Enable theglobal hardware memory initialization keybyprogramming 0xA intoMINITGCR[3:0], the
Memory Initialization Key field (MINITGENA) oftheMemory Hardware Initialization Global Control
Register (MINITGCR) register.
2.Select themodule onwhich thememory hardware initialization hastobeperformed byprogramming
theappropriate value intotheMSINENA(31 -0)bitsintheMSINENA register. See Table 2-7.
3.Iftheglobal auto-initialization scheme isenabled, thecorresponding module willinitialize itsmemories
based onitsmemory error checking scheme (even parity oroddparity orECC).
4.When thememory initialization iscomplete, themodule willsignal "memory initialization done ",which
sets thecorresponding bitinthesystem module MIDONE field oftheMINISTAT register toindicate the
completion ofitsmemory initialization.
5.When thememory hardware initialization completes forallmodules, (indicated byeach module 's
MIDONE bitbeing set), thememory hardware initialization done bit(MINIDONE) issetinthe
MSTCGSTAT register.
Figure 2-5.Hardware Memory Initialization Protocol

<!-- Page 138 -->

Memory Organization www.ti.com
138 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture(1)Ifparity protection isenabled fortheperipheral SRAM modules, then theparity bitswillalso beinitialized along with theSRAM
modules.
(2)IfECC protection isenabled fortheCPU data RAM orperipheral SRAM modules, then theauto-initialization process also
initializes thecorresponding ECC space.
(3)The level 2SRAM range from 128kB to512kB isdivided into6memory regions. Each region hasanassociated control bitto
enable auto-initialization.
(4)The MibSPIx modules perform aninitialization ofthetransmit andreceive RAMs assoon asthemulti-buffered mode isenabled.
This isindependent ofwhether theapplication hasalready initialized these RAMs using theauto-initialization method ornot.The
MibSPIx modules need tobereleased from reset bywriting 1totheir SPIGCR0 registers before starting auto-initialization on
their respective RAMs.Table 2-7.Memory Initialization Select Mapping(1)(2)
Connecting
ModuleMemory
Protection
SchemeAddress Range
SYS.MSINENA
Register Bit#L2RAMW.MEMINT_ENA
Register Bit#(3)Base Address Ending Address
L2SRAM ECC 0x08000000 0x0800FFFF 0 0
L2SRAM ECC 0x08010000 0x0801FFFF 0 1
L2SRAM ECC 0x08020000 0x0802FFFF 0 2
L2SRAM ECC 0x08030000 0x0803FFFF 0 3
L2SRAM ECC 0x08040000 0x0804FFFF 0 4
L2SRAM ECC 0x08050000 0x0805FFFF 0 5
L2SRAM ECC 0x08060000 0x0806FFFF 0 6
L2SRAM ECC 0x08070000 0x0807FFFF 0 7
MIBSPI5 RAM(4)ECC 0xFF0A0000 0xFF0BFFFF 12 n/a
MIBSPI4 RAM(4)ECC 0xFF060000 0xFF07FFFF 19 n/a
MIBSPI3 RAM(4)ECC 0xFF0C0000 0xFF0DFFFF 11 n/a
MIBSPI2 RAM(4)ECC 0xFF080000 0xFF09FFFF 18 n/a
MIBSPI1 RAM(4)ECC 0xFF0E0000 0xFF0FFFFF 7 n/a
DCAN4 RAM ECC 0xFF180000 0xFF19FFFF 20 n/a
DCAN3 RAM ECC 0xFF1A0000 0xFF1BFFFF 10 n/a
DCAN2 RAM ECC 0xFF1C0000 0xFF1DFFFF 6 n/a
DCAN1 RAM ECC 0xFF1E0000 0xFF1FFFFF 5 n/a
MIBADC2 RAM Parity 0xFF3A0000 0xFF3BFFFF 14 n/a
MIBADC1 RAM Parity 0xFF3E0000 0xFF3FFFFF 8 n/a
NHET2 RAM Parity 0xFF440000 0xFF45FFFF 15 n/a
NHET1 RAM Parity 0xFF460000 0xFF47FFFF 3 n/a
HET TU2 RAM Parity 0xFF4C0000 0xFF4DFFFF 16 n/a
HET TU1 RAM Parity 0xFF4E0000 0xFF4FFFFF 4 n/a
DMA RAM ECC 0xFFF80000 0xFFF80FFF 1 n/a
VIM RAM ECC 0xFFF82000 0xFFF82FFF 2 n/a
FlexRay TURAM ECC 0xFF500000 0xFF51FFFF 13 n/a

<!-- Page 139 -->

www.ti.com Exceptions
139 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.3 Exceptions
An"Exception "isanevent thatmakes theprocessor temporarily haltthenormal flow ofprogram
execution, forexample, toservice aninterrupt from aperipheral. Before attempting tohandle an
exception, theprocessor preserves thecritical parts ofthecurrent processor state sothattheoriginal
program canresume when thehandler routine hasfinished.
The following sections describe three exceptions -Reset, Abort andtheSystem Software Interrupts.
Forcomplete details onallexceptions, refer totheARM ®Cortex ®-R5F Technical Reference Manual .
2.3.1 Resets
The TMS570LC43x microcontroller canbereset byeither oftheconditions described inTable 2-8.Each
reset condition isindicated intheSystem Exception Status Register (SYSESR).
The device nRST terminal isanI/O.Itcanbedriven lowbyanexternal circuit toforce awarm reset onthe
microcontroller. This terminal willbedriven lowasanoutput foraminimum of32peripheral clock (VCLK)
cycles foranydevice system reset condition. Asaresult theEXTRST bitintheSYSESR register,
SYSESR[3], gets setforallreset conditions listed inTable 2-8.The nRST isdriven lowasanoutput fora
longer duration during device power-up orwhenever thepower-on reset (nPORRST) isdriven low
externally. Refer thedevice data manual fortheelectrical andtiming specifications forthenRST.
Table 2-8.Causes ofResets
Condition Description
Driving nPORRST pinlow
externallyCold reset, orpower-on reset. This reset signal istypically driven byanexternal voltage
supervisor. This reset isflagged bythePORST bitintheSYSESR register, SYSESR[15].
Voltage Monitor reset The microcontroller hasanembedded voltage monitor thatgenerates apower-on reset when
thecore voltage gets outofavalid range, orwhen theI/Ovoltage falls below athreshold.
This reset isalso flagged bythePORST bitintheSYSESR register, SYSESR[15].
Note: The voltage monitor isnotanalternative foranexternal voltage supervisor.
Driving nRST pinlowexternally Warm reset. This reset input istypically used inasystem with multiple ICsandwhich requires
thatthemicrocontroller also gets reset whenever theother ICdetects afault condition. This
reset isflagged bytheEXTRST bitintheSYSESR, register SYSESR[3].
Oscillator failure This reset isgenerated bythesystem module when theclock monitor detects anoscillator fail
condition. Whether ornotareset isgenerated isalso dictated byaregister inthesystem
module. This reset isflagged bytheOSCRST bitintheSYSESR register, SYSESR[14].
Software reset This reset isgenerated bytheapplication software writing a1tobit15ofSystem Exception
Control Register (SYSECR) ora0tobit14ofSYSECR. Itistypically used byabootloader
type ofcode thatuses asoftware reset toallow thecode execution tobranch tothe
application code once itisprogrammed intotheprogram memory. This reset isflagged bythe
SWRST bitintheSYSESR register, SYSESR[4].
CPU reset This reset isgenerated bytheCPU self-test controller (LBIST) orbychanging thememory
protection (MMU/MPU) configuration intheCPURSTCR register orafter theCPU
Interconnect Subsystem selftest. This reset isflagged bytheCPURST bitintheSYSESR
register, SYSESR[5].
Debug reset The ICEPICK logic implemented onthemicrocontroller allows asystem reset tobegenerated
viathedebug logic. This reset isflagged bytheDBGRST bitintheSYSESR register,
SYSESR[13].
Watchdog reset This reset isgenerated bythedigital windowed watchdog (DWWD) module onthe
microcontroller. The DWWD cangenerate areset whenever thewatchdog service window is
violated. This reset isflagged bytheWDRST bitintheSYSESR register, SYSESR[13].
2.3.2 Aborts
When theARM Cortex-R5F processor's memory system cannot complete amemory access successfully,
anabort isgenerated. Anerror occurring onaninstruction fetch generates aprefetch abort .Errors
occurring ondata accesses generate data aborts .Aborts arealso categorized asbeing either precise or
imprecise .

<!-- Page 140 -->

Exceptions www.ti.com
140 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.3.2.1 Prefetch Aborts
When aPrefetch Abort (PABT) occurs, theprocessor marks theprefetched instruction asinvalid, butdoes
nottake theexception until theinstruction istobeexecuted. Iftheinstruction isnotexecuted, forexample
because abranch occurs while itisinthepipeline, theabort does nottake place.
Allprefetch aborts areprecise aborts.
2.3.2.2 Data Aborts
Anerror occurring onadata memory access cangenerate adata abort. Iftheinstruction generating the
memory access isnotexecuted, forexample, because itfails itscondition codes, orisinterrupted, the
data abort does nottake place.
AData Abort (DABT) canbeeither precise orimprecise, depending onthetype offault thatcaused it.
2.3.2.3 Precise Aborts
Aprecise abort, also known asasynchronous abort, isoneforwhich theexception isguaranteed tobe
taken ontheinstruction thatgenerated theaborting memory access. The abort handler canusethevalue
intheLink Register (r14_abt) todetermine which instruction generated theabort, andthevalue inthe
Saved Program Status Register (SPSR_abt) todetermine thestate oftheprocessor when theabort
occurred.
2.3.2.4 Imprecise Aborts
Animprecise abort, also known asanasynchronous abort, isoneforwhich theexception istaken ona
later instruction totheinstruction thatgenerated theaborting memory access. The abort handler cannot
determine which instruction generated theabort, orthestate oftheprocessor when theabort occurred.
Therefore, imprecise aborts arenormally fatal.
Imprecise aborts canbegenerated bystore instructions tonormal-type ordevice-type memory. When the
store instruction iscommitted, thedata isnormally written intoabuffer thatholds thedata until the
memory system hassufficient bandwidth toperform thewrite access. This gives read accesses higher
priority. The write data canbeheld inthebuffer foralong period, during which many other instructions
cancomplete. Ifanerror occurs when thewrite isfinally performed, thisgenerates animprecise abort.
The TMS570LC43x microcontroller architecture applies techniques atthesystem level tomitigate the
impact ofimprecise aborts. System level adoption ofwrite status sidebands tothedata path allow bus
masters tocomprehend imprecise aborts, turning them intoprecise aborts. Incases where thisapproach
isnotfeasible, buffering bridges orother sources ofimprecision may build aFIFO ofcurrent transactions
such thatanimprecise abort may beregistered atthepoint ofimprecision forlater analysis.
Masking OfImprecise Aborts:
The nature ofimprecise aborts means thatthey canoccur while theprocessor ishandling adifferent
abort. Ifanimprecise abort generates anew exception insuch asituation, thebanked linkregister
(R14_abt) andtheSaved Processor Status Register (SPSR_abt) values areoverwritten. Ifthisoccurs
before thedata ispushed tothestack inmemory, thestate information about thefirstabort islost. To
prevent thisfrom happening, theCurrent Processor Status Register (CPSR) contains amask bitto
indicate thatanimprecise abort cannot beaccepted, theA-bit. When theA-bit isset,anyimprecise abort
thatoccurs isheld pending bytheprocessor until theA-bit iscleared, when theexception isactually
taken. The A-bit isautomatically setwhen abort, IRQ orFIQexceptions aretaken, andonreset. The
application must only clear theA-bit inanabort handler after thestate information haseither been stacked
tomemory, orisnolonger required.
NOTE: Default Behavior forImprecise Aborts
The A-bit intheCPSR issetbydefault. This means thatnoimprecise abort exception will
occur. The application must enable imprecise abort exception generation byclearing theA-
bitoftheCPSR.

<!-- Page 141 -->

www.ti.com Exceptions
141 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.3.2.5 Conditions That Generate Aborts
AnAbort isgenerated under thefollowing conditions ontheTMS570LC43x microcontrollers.
*Access toanillegal address (anon-implemented address)
*Access toaprotected address (protection violation)
*Parity /ECC /Time-out Error onavalid access
Illegal Addresses:
The illegal addresses andtheresponses toanaccess tothese addresses aredefined inTable 2-2.
Addresses Protected ByMPU:
Formore details ontheMPU configuration, refer totheARM ®Cortex ®-R5F Technical Reference Manual .
Amemory access violation islogged asapermission fault intheCPU 'sfault status register andthevirtual
address oftheaccess islogged intotheCPU 'sfault address register.
Protection ofPeripheral Register andMemory Frames:
Accesses totheperipheral register andmemory frames canbeprotected either byconfiguring theMPU or
byconfiguring thePeripheral Central Resource (PCR) controller registers.
The PCR module PPROTSETx registers contain onebitperperipheral select quadrant. These bitsdefine
theaccess permissions totheperipheral register frames. IftheCPU attempts towrite toaperipheral
register forwhich itdoes nothave thecorrect permissions, aprotection violation isdetected andanAbort
occurs.
Some modules also enforce register updates toonly beallowed when theCPU isinaprivileged mode of
operation. IftheCPU writes tothese registers inuser mode, thewrites areignored.
The PCR module PMPROTSETx registers contain onebitperperipheral memory frame. These bitsdefine
theaccess permissions totheperipheral memory frames. IftheCPU attempts towrite toaperipheral
memory forwhich itdoes nothave thecorrect permissions, aprotection violation isdetected andanAbort
occurs.
NOTE: NoAccess Protection forReads
The PCR PPROTSETx andPMPROTSETx registers protect theperipheral registers and
memories against illegal writes bytheCPU. The CPU canread from theperipheral registers
andmemories inboth user andprivileged modes.
2.3.3 System Software Interrupts
The system module provides thecapability ofgenerating uptofour software interrupts. Asoftware
interrupt isgenerated bywriting thecorrect keyvalue toeither ofthefour System Software Interrupt
Registers (SSIRx). The SSIregisters also allow theapplication toprovide alabel forthatsoftware
interrupt. This label isan8-bit value thatcanthen beused bytheinterrupt service routine toperform the
required task based onthevalue provided. The source ofthesystem software interrupt isreflected inthe
system software interrupt vector (SSIVEC) register. The pending interrupt flagiscaptured inSSIF register.
NOTE: The SSIRx, SSIVEC andSSIF registers arebanked registers. This allows thesystem
module tosupport uptotwoCPUs forsystem software interrupt generation. Each CPU will
have itsown banked SSIregisters. Both CPUs willseetheSSIregisters atthesame
address. The system module decodes theunique master IDcorresponding totheCPU's
access tothebanked registers.

<!-- Page 142 -->

Clocks www.ti.com
142 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4 Clocks
This section describes theclocking structure oftheTMS570LC43x microcontrollers.
2.4.1 Clock Sources
The devices support upto7clock sources. These areshown inTable 2-9.The electrical specifications as
well astiming requirements foreach oftheclock sources arespecified inthedevice data manual.
Table 2-9.Clock Sources
Clock Source # Clock Source Name Description
0 OSCINMain oscillator. This istheprimary clock forthemicrocontroller andisthe
only clock thatisinput tothephase-locked loops. The oscillator frequency
must bebetween 5and20MHz.
1 PLL1This istheoutput ofthemain PLL. The PLL iscapable ofmodulating its
output frequency inacontrolled manner toreduce theradiated emissions.
2 ReservedThis clock source isnotavailable andmust notbeenabled orused as
source foranyclock domain.
3 EXTCLKIN1External clock input 1.Asquare wave input canbeapplied tothisdevice
input andused asaclock source inside thedevice.
4LFLPO
(Low-Frequency LPO)
(CLK80K)This isthelow-frequency output oftheinternal reference oscillator. This is
typically an80KHz signal (CLK80K) thatisused bythereal-time interrupt
module forgenerating periodic interrupts towake upfrom alowpower
mode.
5HFLPO
(High-Frequency LPO)
(CLK10M)This isthehigh-frequency output oftheinternal reference oscillator. This is
typically a10MHz signal (CLK10M) thatisused bytheclock monitor
module asareference clock tomonitor themain oscillator frequency.
6 PLL2This istheoutput ofthesecond PLL. There isnooption ofmodulating this
PLL'soutput signal. This separate non-modulating PLL allows the
generation ofanasynchronous clock source thatisindependent ofthe
CPU clock frequency.
7 EXTCLKIN2External clock input 2.Asquare wave input canbeapplied tothisdevice
input andused asaclock source inside thedevice.
2.4.1.1 Enabling /Disabling Clock Sources
Each clock source canbeindependently enabled ordisabled using thesetofClock Source Disable
registers -CSDIS, CSDISSET andCSDISCLR.
Each bitinthese registers corresponds totheclock source number indicated inTable 2-9.Forexample,
setting bit1intheCSDIS orCSDISSET registers disables thePLL#1.
NOTE: Disabling theMain Oscillator orHFLPO
Bydefault, theclock monitoring circuit isenabled andchecks forthemain oscillator
frequency tobewithin acertain range using theHFLPO asareference. Ifthemain oscillator
and/or theHFLPO aredisabled with theclock monitoring stillenabled, theclock monitor will
indicate anoscillator fault. The clock monitoring must bedisabled before disabling themain
oscillator ortheHFLPO clock source(s).
The clock source isonly disabled once there isnoactive clock domain thatisusing thatclock source. Also
check the"Oscillator andPLL"user guide formore information onenabling /disabling theoscillator and
PLL.
OntheTMS570LC43x microcontrollers, theclock sources 0,4,and5areenabled bydefault.

<!-- Page 143 -->

www.ti.com Clocks
143 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.1.2 Checking forValid Clock Sources
The application cancheck whether aclock source isvalid ornotbychecking thecorresponding bittobe
setintheClock Source Valid Status (CSVSTAT) register. Forexample, theapplication cancheck ifbit1in
CSVSTAT issetbefore using theoutput ofPLL#1 asthesource foranyclock domain.
2.4.2 Clock Domains
The clocking onthisdevice isdivided intomultiple clock domains forflexibility incontrol aswell asclock
source selection. There are10clock domains onthisdevice. Each ofthese aredescribed inTable 2-10 .
Each ofthecontrol registers listed inTable 2-10 aredefined inSection 2.5.The ACtiming characteristics
foreach clock domain arespecified inthedevice data manual.
Table 2-10. Clock Domains
Clock DomainClock Disable
BitDefault
SourceSource Selection
Register Special Considerations
GCLK1 CDDIS.0 OSCIN GHVSRC[3:0]*This themain clock from which HCLK isdivided
down
*Inphase with HCLK
*Isdisabled separately from HCLK viatheCDDISx
registers bit0
*Can bedivided by1upto8when running CPU self-
test(LBIST) using theCLKDIV field ofthe
STCCLKDIV register ataddress 0xFFFFE108
HCLK CDDIS.1 OSCIN GHVSRC[3:0]*Divided from GCLK1 viaHCLKCNTL register
*Allowable clock ratio from 1:1to4:1
*Isdisabled viatheCDDISx registers bit1
VCLK CDDIS.2 OSCIN GHVSRC[3:0]*Divided down from HCLK viaCLKCNTL register
*Can beHCLK/1, HCLK/2,... orHCLK/16
*Isdisabled separately from HCLK viatheCDDISx
registers bit2
*HCLK:VCLK2:VCLK must beinteger ratios ofeach
other
VCLK2 CDDIS.3 OSCIN GHVSRC[3:0]*Divided down from HCLK
*Can beHCLK/1, HCLK/2,... orHCLK/16
*Frequency must beaninteger multiple ofVCLK
frequency
*Isdisabled separately from HCLK viatheCDDISx
registers bit3
VCLK3 CDDIS.8 OSCIN GHVSRC[3:0]*Divided down from HCLK
*Can beHCLK/1, HCLK/2,... orHCLK/16
*HCLK:VCLK3 must beinteger ratios ofeach other
*Isdisabled separately from HCLK viatheCDDISx
registers bit8
VCLKA1 CDDIS.4 VCLK VCLKASRC[3:0]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit4
VCLKA2 CDDIS.5 VCLK VCLKASRC[3:0]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit5
VCLKA4 CDDIS.11 VCLK VCLKACON1[19:16]*Defaults toVCLK asthesource
*Isdisabled viatheCDDISx registers bit11
VCLKA4_DIVR VCLKACON1.20 VCLK VCLKACON1[19:16]*Divided down from VCLKA4 using theVCLKA4R
field oftheVCLKACON1 register
*Frequency canbeVCLKA4/1, VCLKA4/2, ...,or
VCLKA4/8
*Default frequency isVCLKA4/2
*Isdisabled separately viatheVCLKACON1
register 'sVCLKA4_DIV_CDDIS bit,iftheVCLKA4 is
notalready disabled

<!-- Page 144 -->

Clocks www.ti.com
144 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-10. Clock Domains (continued)
Clock DomainClock Disable
BitDefault
SourceSource Selection
Register Special Considerations
RTICLK1 CDDIS.6 VCLK RCLKSRC[3:0]*Defaults toVCLK asthesource
*Ifaclock source other than VCLK isselected for
RTICLK1, then theRTICLK1 frequency must beless
than orequal toVCLK/3
*Application canensure thisbyprogramming the
RTI1DIV field oftheRCLKSRC register, ifnecessary
*Isdisabled viatheCDDISx registers bit6
2.4.2.1 Enabling /Disabling Clock Domains
Each clock domain canbeindependently enabled ordisabled using thesetofClock Domain Disable
registers -CDDIS, CDDISSET, andCDDISCLR.
Each bitinthese registers corresponds totheclock domain number indicated inTable 2-10.Forexample,
setting bit1intheCDDIS orCDDISSET registers disables theHCLK clock domain. The clock domain will
beturned offonly when every module thatuses theHCLK domain gives the"permission "forHCLK tobe
turned off.
Allclock domains areenabled bydefault, orupon asystem reset, orwhenever awake upcondition is
detected.
2.4.2.2 Mapping Clock Sources toClock Domains
Each clock domain needs tobemapped toavalid clock source. There arecontrol registers thatallow an
application tochoose theclock sources foreach clock domain.
*Selecting clock source forGCLK1, HCLK, andVCLKx domains
The CPU clock (GCLK1), thesystem module clock (HCLK), andtheperipheral busclocks (VCLKx) alluse
thesame clock source. This clock source isselected viatheGHVSRC register. The default source forthe
GCLK1, HCLK, andVCLKx isthemain oscillator. That is,after power up,theGCLK1 andHCLK are
running attheOSCIN frequency, while theVCLKx frequency istheOSCIN frequency divided by2.
*Selecting clock source forVCLKA1 andVCLKA2 domains
The clock source forVCLKA1 andVCLKA2 domains isselected viatheVCLKASRC register. The default
source fortheVCLKA1 andVCLKA2 domains istheVCLK.
*Selecting clock source forVCLKA4 domain
The clock source forVCLKA4 domain isselected viatheVCLKACON1 register. The default source forthe
VCLKA4 domain istheVCLK.
*Selecting clock source forRTICLK1 domain
The clock source forRTICLK1 domain isselected viatheRCLKSRC register. The default source forthe
RTICLK1 domain istheVCLK.
NOTE: Selecting aclock source forRTICLK1 thatisnotVCLK
When theapplication chooses aclock source forRTICLK1 domain thatisnotVCLK, then the
application must ensure thattheresulting RTICLK1 frequency must beless than orequal to
VCLK frequency divided by3.The application canconfigure theRTI1DIV field ofthe
RCLKSRC register fordividing theselected clock source frequency by1,2,4or8tomeet
thisrequirement.

<!-- Page 145 -->

www.ti.com Clocks
145 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.3 Low Power Modes
Allclock domains areactive inthenormal operating mode. This isthedefault mode ofoperation. As
described inSection 2.4.1.1 andSection 2.4.2.1 ,theapplication canchoose todisable anyparticular clock
source anddomain thatitdoes notplan touse. Also, theperipheral central resource controller (PCR) has
control registers toenable /disable theperipheral clock (VCLK) foreach peripheral select. This offers the
application alarge number ofchoices forenabling /disabling clock sources, orclock domains, orclocks to
specific peripherals.
This section describes three particular low-power modes andtheir typical characteristics. They arenotthe
only low-power modes configurable bytheapplication, asjustdescribed.
Table 2-11. Typical Low-Power Modes
Mode
NameActive Clock
Source(s)Active
Clock
Domain(s)Wake UpOptionsSuggested
Wake Up
Clock
Source(s)Wake UpTime(wake updetected -to-CPU
code execution start)
Doze Main oscillator RTICLK1RTIinterrupt,
GIO interrupt,
CAN message,
SCImessageMain oscillatorFlash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time
Snooze LFLPO RTICLK1RTIinterrupt,
GIO interrupt,
CAN message,
SCImessageHFLPOHFLPO warm start-up time
+
Flash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time
Sleep None NoneGIO interrupt,
CAN message,
SCImessageHFLPOHFLPO warm start-up time
+
Flash pump sleep ->active transition time
+
Flash bank sleep ->standby transition time
+
Flash bank standby ->active transition time

<!-- Page 146 -->

Clocks www.ti.com
146 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.3.1 Typical Software Sequence toEnter aLow-Power Mode
1.Disable allnon-CPU busmasters sothey donotcarry outanyfurther bustransactions.
2.Program theflash banks andflash pump fall-back modes tobe"sleep ".
The flash pump transitions from active tosleep mode only after alltheflash banks have switched from
active tosleep mode.
3.Disable theclock sources thatarenotrequired tobekept active.
Aclock source does notgetdisabled until allclock domains using thatclock source aredisabled first,
orareconfigured touseanalternate clock source.
4.Disable theclock domains thatarenotrequired tobekept active.
Aclock domain does notgetdisabled until allmodules using thatclock domain "give their permission "
forthatclock domain tobeturned off.
5.IdletheCortex-R5F core.
The ARM Cortex-R5F CPU hasinternal power management logic, andrequires adedicated instruction
tobeused inorder toenter alowpower mode. This istheWait ForInterrupt (WFI) instruction.
When aWFI instruction isexecuted, theCortex-R5F core flushes itspipeline, flushes allwrite buffers,
andcompletes allpending bustransactions. Atthistime thecore indicates tothesystem thattheclock
tothecore canbestopped. This indication isused bytheGlobal Clock Module (GCM) toturn offthe
CPU clock domain (GCLK1) iftheCDDIS register bit0isset.
2.4.3.2 Special Considerations forEntry toLow Power Modes
Some busmaster modules -DMA, High-End Timer Transfer Units (HTUx), FlexRay Transfer Unit (FTU),
andParameter Overlay Module (POM), canhave ongoing transactions when theapplication wants to
enter alowpower mode toturn offtheclocks tothose modules. This isnotrecommended asitcould
leave thedevice inanunpredictable state. Refer totheindividual module user guides formore information
about thesequence tobefollowed tosafely enter alow-power mode.
2.4.3.3 Selecting Clock Source Upon Wake Up
The domains forCPU clock (GCLK1), thesystem clock (HCLK) andtheperipheral clock (VCLKx) usethe
same clock source selected viatheGHVSRC field oftheGHVSRC register. The GHVSRC register also
allows theapplication tochoose theclock source after wake upviatheGHVWAKE field.
When awake upcondition isdetected, iftheselected wake upclock source isnotalready active, the
global clock module (GCM) willenable thisselected clock source, wait forittobecome valid, andthen use
itfortheGCLK1, HCLK, andVCLKx domains. The other clock domains VCLKAx andRTICLK1 retain the
configuration fortheir clock source selection registers -VCLKASRC, VCLKACON1 andRCLKSRC.
2.4.4 Clock Test Mode
The TMS570LC43x microcontrollers support atestmode which allows auser tobring outseveral different
clock sources andclock domains ontotheECLK1 terminal inaddition tooutputting theexternal clock.
This isvery useful information fordebug purposes. Each clock source also hasacorresponding clock
source valid status flagintheClock Source Valid Status (CSVSTAT) register. The clock source valid
status flags canalso bebrought outontotheN2HET1[12] terminal inthisclock testmode.
The clock testmode iscontrolled bytheCLKTEST register inthesystem module register frame (see
Section 2.5.1.31 ).
The clock testmode isenabled bywriting 0x5totheCLK_TEST_EN field.

<!-- Page 147 -->

www.ti.com Clocks
147 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureThe signal tobebrought outontotheECLK1 terminal isdefined bytheSEL_ECP_PIN field, andthe
signal tobebrought outontotheN2HET1[12] terminal isdefined bytheSEL_GIO_PIN field. The choices
forthese selections aredefined inTable 2-12.
Table 2-12. Clock Test Mode Options
SEL_ECP_PIN Signal onECLK SEL_GIO_PIN Signal onN2HET1[12]
00000 Oscillator clock 0000 Oscillator Valid Status
00001 PLL1 clock output 0001 PLL1 Valid Status
00010 Reserved 0010 Reserved
00011 EXTCLKIN1 0011 Reserved
00100Low-frequency LPO (Low-Power
Oscillator) clock [CLK80K]0100 Reserved
00101High-frequency LPO (Low-Power
Oscillator) clock [CLK10M]0101HFLPO Clock Output Valid Status
[CLK10M]
00110 PLL2 clock output 0110 PLL2 Valid Status
00111 EXTCLKIN2 0111 Reserved
01000 GCLK1 1000LFLPO Clock Output Valid Status
[CLK80K]
01001 RTI1 Base 1001 Oscillator Valid Status
01010 Reserved 1010 Oscillator Valid Status
01011 VCLKA1 1011 Oscillator Valid Status
01100 VCLKA2 1100 Oscillator Valid Status
01101 Reserved 1101 Reserved
01110 VCLKA4_DIVR 1110 VCLKA4
01111 Flash HDPump Oscillator 1111 Oscillator Valid Status
10000 Reserved
10001 HCLK
10010 VCLK
10011 VCLK2
10100 VCLK3
10101-10110 Reserved
10111 EMAC clock output
11000-11111 Reserved

<!-- Page 148 -->

Clocks www.ti.com
148 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.5 Embedded Trace Macrocell (ETM-R5)
The TMS570LC43x microcontrollers contain anETM-R5 module with a32-bit internal data port. The ETM-
R5module isconnected toaTrace Port Interface Unit (TPIU) with a32-bit data bus; theTPIU provides a
35-bit (32-bit data and3-bit control) external interface fortrace. The ETM-R5 isCoreSight compliant and
follows theETM v3specification. Formore details ontheETM-R5 specification, refer totheEmbedded
Trace Macrocell Architecture Specification .
The ETM clock source isselected aseither VCLK ortheexternal ETMTRACECLKIN pin.The selection is
done bytheEXTCTLOUT control bitsoftheTPIU EXTCTL_Out_Port register. The address ofthisregister
isTPIU base address +0x404.
Before youbegin accessing TPIU registers, theTPIU should beunlocked viatheCoreSight keyand1hor
2hshould bewritten tothisregister.
Figure 2-6.EXTCTL_Out_Port Register [offset =404h]
31 16
Reserved
R-0
15 2 1 0
Reserved EXTCTLOUT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-13. EXTCTL_Out_Port Register Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 EXTCTLOUT EXTCTL output control.
0 Tied-zero
1h VCLK
2h ETMTRACECLKIN
3h Tied-zero
2.4.6 Safety Considerations forClocks
The TMS570LC43x microcontrollers aretargeted foruseinseveral safety-critical applications. The
following sections describe theinternal orexternal monitoring mechanisms thatdetect andsignal clock
source failures.

<!-- Page 149 -->

guaranteed faillower
thresholdguaranteed passupper
thresholdguaranteed fail
1.375 4.875 22 78f[MHz]
www.ti.com Clocks
149 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.6.1 Oscillator Monitor
The oscillator clock frequency ismonitored byadedicated circuitry called CLKDET using theHFLPO as
thereference clock. The CLKDET flags anoscillator failcondition whenever theOSCIN frequency falls
outside ofarange which isdefined bytheHFLPO frequency.
Thevalid OSCIN range isdefined asaminimum off(HF LPO)/4toamaximum off(HF LPO)×4.
The untrimmed HFLPO frequency onthisdevice canrange from 5.5MHz to19.5 MHz. This results ina
valid OSCIN frequency range depicted inFigure 2-7.
The application canselect thedevice response toanoscillator failindication. Refer toChapter 14formore
details ontheoscillator monitoring andthesystem response choices.
Figure 2-7.LPO andClock Detection, Untrimmed HFLPO
2.4.6.2 PLL SlipDetector
Both thePLL macros implemented onthemicrocontrollers have anembedded slipdetection circuit. APLL
slipisdetected bytheslipdetector under thefollowing conditions:
1.Reference cycle slip, RFSLIP --theoutput clock isrunning toofastrelative tothereference clock
2.Feedback cycle slip, FBSLIP --theoutput clock isrunning tooslow relative tothereference clock
The device also includes optional filters thatcanbeenabled before aslipindication from thePLL is
actually logged inthesystem module Global Status Register (GLBSTAT). Also, once aPLL slipcondition
islogged inthesystem module global status register, theapplication canchoose thedevice 'sresponse to
theslipindication. Refer toChapter 14formore details onPLL slipandthesystem response choices.
2.4.6.3 External Clock Monitor
The microcontrollers support twoterminals called ECLK1 andECLK2 -External Clock, which areused to
output aslow frequency which isdivided down from thedevice system clock frequency. Anexternal circuit
canmonitor theECLK1 and/or ECLK2 frequency inorder tocheck thatthedevice isoperating atthe
correct frequency.
The frequency ofthesignal output ontheECLKx pincanbedivided down by1to65536 from the
peripheral clock (VCLK) frequency using theExternal Clock Prescaler Control Register (ECPCNTL) for
ECLK1 andECPCNTL1 forECLK2. The actual clock output onECLK1 isenabled bysetting theECP CLK
FUN bitoftheSYSPC1 control register. Bydefault, theECLK1 terminal isinGIO mode. ECLK2
functionality canbeenabled bywriting 5htotheECP_KEY field oftheECPCNTL1 register.
NOTE: ECLK2 ismultiplexed with EMIF_CLK andECLK2 isnotaprimary function after reset. User
willneed toselect ECLK2 tobebrought outtotheterminal using IOMM module.
2.4.6.4 Dual-Clock Comparators
The microcontrollers include twoinstances ofthedual-clock comparator (DCC) module. This module
includes twodown counters which independently count from twoseparate seed values attherate oftwo
independent clock frequencies. One oftheclock inputs isareference clock input, selectable between the
main oscillator ortheHFLPO infunctional mode. The second clock input isselectable from among aset
ofdefined signals asdescribed inSection 2.4.6.4.1 andSection 2.4.6.4.2 .This mechanism canbeused to
useaknown-good clock tomeasure thefrequency ofanother clock.

<!-- Page 150 -->

Clocks www.ti.com
150 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.4.6.4.1 DCC1
Ascanbeseen, themain oscillator (OSCIN) canbeused forcounter 0asa"known-good "reference
clock. The clock forcounter 1canbeselected from among 8options. Refer totheDCC module chapter
formore details ontheDCC usage.
Table 2-14. DCC1 Counter 0Clock Inputs
Clock Source [3-0] Clock /Signal Name
Allother values oscillator (OSCIN)
5h HFLPO
Ah testclock (TCK)
Table 2-15. DCC1 Counter 1Clock /Signal Inputs
Key[3-0] Clock Source [3-0] Clock /Signal Name
Ah0h PLL1 free-running clock output
1h PLL2 free-running clock output
2h LFLPO
3h HFLPO
4h Flash pump oscillator
5h EXTCLKIN1
6h EXTCLKIN2
7 Reserved
8h-Fh VCLK
Allother values anyvalue N2HET1[31]
2.4.6.4.2 DCC2
Ascanbeseen, themain oscillator (OSCIN) canbeused forcounter 0asa"known-good "reference
clock. The clock forcounter 1canbeselected from among 2options. Refer totheDCC module chapter
formore details ontheDCC usage.
Table 2-16. DCC2 Counter 0Clock Inputs
Clock Source [3-0] Clock /Signal Name
others oscillator (OSCIN)
0xA testclock (TCK)
Table 2-17. DCC2 Counter 1Clock /Signal Inputs
Key[3-0] Clock Source [3-0] Clock /Signal Name
Ah0h Reserved
1h PLL2 post_ODCLK/8
2h PLL2 post_ODCLK/16
3h-7h Reserved
8h-Fh VCLK
Allother values anyvalue N2HET2[0]

<!-- Page 151 -->

www.ti.com System andPeripheral Control Registers
151 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5 System andPeripheral Control Registers
The following sections describe thesystem andperipheral control registers oftheTMS570LC43x
microcontroller.
2.5.1 Primary System Control Registers (SYS)
This section describes theSYSTEM registers. These registers aredivided intotwoseparate frames. The
start address oftheprimary system module frame isFFFF FF00h. The start address ofthesecondary
system module frame isFFFF E100h. The registers support 8-,16-, and32-bit writes. The offset isrelative
tothesystem module frame start address.
Table 2-18 contains alistoftheprimary system control registers.
Table 2-18. Primary System Control Registers
Offset Acronym Register Description Section
00h SYSPC1 SYS PinControl Register 1 Section 2.5.1.1
04h SYSPC2 SYS PinControl Register 2 Section 2.5.1.2
08h SYSPC3 SYS PinControl Register 3 Section 2.5.1.3
0Ch SYSPC4 SYS PinControl Register 4 Section 2.5.1.4
10h SYSPC5 SYS PinControl Register 5 Section 2.5.1.5
14h SYSPC6 SYS PinControl Register 6 Section 2.5.1.6
18h SYSPC7 SYS PinControl Register 7 Section 2.5.1.7
1Ch SYSPC8 SYS PinControl Register 8 Section 2.5.1.8
20h SYSPC9 SYS PinControl Register 9 Section 2.5.1.9
30h CSDIS Clock Source Disable Register Section 2.5.1.10
34h CSDISSET Clock Source Disable SetRegister Section 2.5.1.11
38h CSDISCLR Clock Source Disable Clear Register Section 2.5.1.12
3Ch CDDIS Clock Domain Disable Register Section 2.5.1.13
40h CDDISSET Clock Domain Disable SetRegister Section 2.5.1.14
44h CDDISCLR Clock Domain Disable Clear Register Section 2.5.1.15
48h GHVSRC GCLK1, HCLK, VCLK, andVCLK2 Source Register Section 2.5.1.16
4Ch VCLKASRC Peripheral Asynchronous Clock Source Register Section 2.5.1.17
50h RCLKSRC RTIClock Source Register Section 2.5.1.18
54h CSVSTAT Clock Source Valid Status Register Section 2.5.1.19
58h MSTGCR Memory Self-Test Global Control Register Section 2.5.1.20
5Ch MINITGCR Memory Hardware Initialization Global Control Register Section 2.5.1.21
60h MSINENA Memory Self-Test/Initialization Enable Register Section 2.5.1.22
68h MSTCGSTAT MSTC Global Status Register Section 2.5.1.23
6Ch MINISTAT Memory Hardware Initialization Status Register Section 2.5.1.24
70h PLLCTL1 PLL Control Register 1 Section 2.5.1.25
74h PLLCTL2 PLL Control Register 2 Section 2.5.1.26
78h SYSPC10 SYS PinControl Register 10 Section 2.5.1.27
7Ch DIEIDL DieIdentification Register, Lower Word Section 2.5.1.28
80h DIEIDH DieIdentification Register, Upper Word Section 2.5.1.29
88h LPOMONCTL LPO/CLock Monitor Control Register Section 2.5.1.31
8Ch CLKTEST Clock Test Register Section 2.5.1.31
90h DFTCTRLREG DFT Control Register Section 2.5.1.32
94h DFTCTRLREG2 DFT Control Register 2 Section 2.5.1.33
A0h GPREG1 General Purpose Register Section 2.5.1.34
B0h SSIR1 System Software Interrupt Request 1Register Section 2.5.1.35
B4h SSIR2 System Software Interrupt Request 2Register Section 2.5.1.36
B8h SSIR3 System Software Interrupt Request 3Register Section 2.5.1.37

<!-- Page 152 -->

System andPeripheral Control Registers www.ti.com
152 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-18. Primary System Control Registers (continued)
Offset Acronym Register Description Section
BCh SSIR4 System Software Interrupt Request 4Register Section 2.5.1.38
C0h RAMGCR RAM Control Register Section 2.5.1.39
C4h BMMCR1 Bus Matrix Module Control Register 1 Section 2.5.1.40
CCh CPURSTCR CPU Reset Control Register Section 2.5.1.41
D0h CLKCNTL Clock Control Register Section 2.5.1.42
D4h ECPCNTL ECP Control Register Section 2.5.1.43
DCh DEVCR1 DEV Parity Control Register 1 Section 2.5.1.44
E0h SYSECR System Exception Control Register Section 2.5.1.45
E4h SYSESR System Exception Status Register Section 2.5.1.46
E8h SYSTASR System Test Abort Status Register Section 2.5.1.47
ECh GLBSTAT Global Status Register Section 2.5.1.48
F0h DEVID Device Identification Register Section 2.5.1.49
F4h SSIVEC Software Interrupt Vector Register Section 2.5.1.50
F8h SSIF System Software Interrupt Flag Register Section 2.5.1.51

<!-- Page 153 -->

www.ti.com System andPeripheral Control Registers
153 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.1 SYS PinControl Register 1(SYSPC1)
The SYSPC1 register, shown inFigure 2-8anddescribed inTable 2-19 ,controls thefunction oftheECLK
pin.
Figure 2-8.SYS PinControl Register 1(SYSPC1) (offset =00h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKFUN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-19. SYS PinControl Register 1(SYSPC1) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKFUN ECLK function. This bitchanges thefunction oftheECLK pin.
0 ECLK isinGIO mode.
1 ECLK isinfunctional mode asaclock output.
Note: Proper ECLK duty cycle isnotguaranteed until 1ECLK cycle haselapsed after
switching intofunctional mode.
2.5.1.2 SYS PinControl Register 2(SYSPC2)
The SYSPC2 register, shown inFigure 2-9anddescribed inTable 2-20 ,controls whether thepinisan
input oranoutput when inGIO mode.
Figure 2-9.SYS PinControl Register 2(SYSPC2) (offset =04h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDIR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-20. SYS PinControl Register 2(SYSPC2) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDIR ECLK data direction. This bitcontrols thedirection oftheECLK pinwhen itisconfigured tobe
inGIO mode only.
0 The ECLK pinisaninput.
Note: Ifthepindirection issetasaninput, theoutput buffer istristated.
1 The ECLK pinisanoutput.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register.

<!-- Page 154 -->

System andPeripheral Control Registers www.ti.com
154 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.3 SYS PinControl Register 3(SYSPC3)
The SYSPC3 register, shown inFigure 2-10 anddescribed inTable 2-21,displays thelogic state ofthe
ECLK pinwhen itisinGIO mode.
Figure 2-10. SYS PinControl Register 3(SYSPC3) (offset =08h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDIN
R-0 R-U
LEGEND: R=Read only; U=value isundefined; -n=value after reset
Table 2-21. SYS PinControl Register 3(SYSPC3) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDIN ECLK data in.This bitdisplays thelogic state oftheECLK pinwhen itisconfigured tobein
GIO mode.
0 The ECLK pinisatlogic low(0).
1 The ECLK pinisatlogic high (1).
2.5.1.4 SYS PinControl Register 4(SYSPC4)
The SYSPC4 register, shown inFigure 2-11 anddescribed inTable 2-22,controls thelogic level output
function oftheECLK pinwhen itisconfigured asanoutput inGIO mode.
Figure 2-11. SYS PinControl Register 4(SYSPC4) (offset =0Ch)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDOUT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-22. SYS PinControl Register 4(SYSPC4) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDOUT ECLK data outwrite. This bitisonly active when theECLK pinisconfigured tobeinGIO mode.
Writes tothisbitwillonly take effect when theECLK pinisconfigured asanoutput inGIO
mode. The current logic state oftheECLK pinwillbedisplayed bythisbitinboth input and
output GIO mode.
0 The ECLK pinisdriven tologic low(0).
1 The ECLK pinisdriven tologic high (1).
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.

<!-- Page 155 -->

www.ti.com System andPeripheral Control Registers
155 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.5 SYS PinControl Register 5(SYSPC5)
The SYSPC5 register, shown inFigure 2-12 anddescribed inTable 2-23,controls thesetfunction ofthe
ECLK pinwhen itisconfigured asanoutput inGIO mode.
Figure 2-12. SYS PinControl Register 5(SYSPC5) (offset =10h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKSET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-23. SYS PinControl Register 5(SYSPC5) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKSET ECLK data outset.This bitdrives theoutput oftheECLK pinhigh when setinGIO output
mode.
0 Write: Writing a0hasnoeffect.
1 Write: The ECLK pinisdriven tologic high (1).
Note: Thecurrent logic state oftheECPCLKDOUT bitwillalso bedisplayed bythisbit
when thepinisconfigured inGIOoutput mode.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.
2.5.1.6 SYS PinControl Register 6(SYSPC6)
The SYSPC6 register, shown inFigure 2-13 anddescribed inTable 2-24,controls theclear function ofthe
ECLK pinwhen itisconfigured asanoutput inGIO mode..
Figure 2-13. SYS PinControl Register 6(SYSPC6) (offset =14h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKCLR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-24. SYS PinControl Register 6(SYSPC6) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKCLR ECLK data outclear. This bitdrives theoutput oftheECLK pinlowwhen setinGIO output
mode.
0 Write: The ECLK pinvalue isunchanged.
1 Write: The ECLK pinisdriven tologic low(0).
Note: Thecurrent logic state oftheECPCLKDOUT bitwillalso bedisplayed bythisbit
when thepinisconfigured inGIOoutput mode.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.

<!-- Page 156 -->

System andPeripheral Control Registers www.ti.com
156 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.7 SYS PinControl Register 7(SYSPC7)
The SYSPC7 register, shown inFigure 2-14 anddescribed inTable 2-25,controls theopen drain function
oftheECLK pin.
Figure 2-14. SYS PinControl Register 7(SYSPC7) (offset =18h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKODE
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-25. SYS PinControl Register 7(SYSPC7) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKODE ECLK open drain enable. This bitisonly active when ECLK isconfigured tobeinGIO mode.
0 The ECLK pinisconfigured inpush/pull (normal GIO) mode.
1 The ECLK pinisconfigured inopen drain mode. The ECPCLKDOUT bitintheSYSPC4 register
controls thestate oftheECLK output buffer:
ECPCLKDOUT =0:The ECLK output buffer isdriven low.
ECPCLKDOUT =1:The ECLK output buffer istristated.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register.
2.5.1.8 SYS PinControl Register 8(SYSPC8)
The SYSPC8 register, shown inFigure 2-15 anddescribed inTable 2-26,controls thepullenable function
oftheECLK pinwhen itisconfigured asaninput inGIO mode.
Figure 2-15. SYS PinControl Register 8(SYSPC8) (offset =1Ch)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKPUE
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; D=Device Specific; -n=value after reset
Table 2-26. SYS PinControl Register 8(SYSPC8) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKPUE ECLK pullenable. Writes tothisbitwillonly take effect when theECLK pinisconfigured asan
input inGIO mode.
0 ECLK pullenable isactive.
1 ECLK pullenable isinactive.
Note: Thepulldirection (up/down) isselected bytheECPCLKPS bitintheSYSPC9
register.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced ininput mode byclearing theECPCLKDIR bit
to0intheSYSPC2 register.

<!-- Page 157 -->

www.ti.com System andPeripheral Control Registers
157 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.9 SYS PinControl Register 9(SYSPC9)
The SYSPC9 register, shown inFigure 2-16 anddescribed inTable 2-27,controls thepullup/pull down
configuration oftheECLK pinwhen itisconfigured asaninput inGIO mode.
Figure 2-16. SYS PinControl Register 9(SYSPC9) (offset =20h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKPS
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-27. SYS PinControl Register 9(SYSPC9) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKPS ECLK pullup/pull down select. This bitisonly active when ECLK isconfigured asaninput in
GIO mode andthepullup/pull down logic isenabled.
0 ECLK pulldown isselected, when pullup/pull down logic isenabled.
1 ECLK pullupisselected, when pullup/pull down logic isenabled.
Note: TheECLK pinpullup/pull down logic isenabled byclearing theECPCLKPUE bitto
0intheSYSPC8 register.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced ininput mode byclearing theECPCLKDIR bit
to0intheSYSPC2 register.

<!-- Page 158 -->

System andPeripheral Control Registers www.ti.com
158 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.10 Clock Source Disable Register (CSDIS)
The CSDIS register, shown inFigure 2-17 anddescribed inTable 2-28,controls anddisplays thestate of
thedevice clock sources.
Figure 2-17. Clock Source Disable Register (CSDIS) (offset =30h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLKSR7OFF CLKSR6OFF CLKSR5OFF CLKSR4OFF CLKSR3OFF Reserved CLKSR1OFF CLKSR0OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-28. Clock Source Disable Register (CSDIS) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 CLKSR[7-3]OFF Clock source[7-3] off.
0 Clock source[7-3] isenabled.
1 Clock source[7-3] isdisabled.
Note: Onwakeup, only clock sources 0,4,and5areenabled.
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 CLKSR[1-0]OFF Clock source[1-0] off.
0 Clock source[1-0] isenabled.
1 Clock source[1-0] isdisabled.
Note: Onwakeup, only clock sources 0,4,and5areenabled.
Table 2-29. Clock Sources Table
Clock Source # Clock Source Name
Clock Source 0 Oscillator
Clock Source1 PLL1
Clock Source 2 NotImplemented
Clock Source 3 EXTCLKIN
Clock Source 4 Low Frequency LPO (Low Power Oscillator) clock
Clock Source 5 High frequency LPO (Low Power Oscillator) clock
Clock Source 6 PLL2
Clock Source 7 EXTCLKIN2
NOTE: Non-implemented clock sources should notbeenabled orused.

<!-- Page 159 -->

www.ti.com System andPeripheral Control Registers
159 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.11 Clock Source Disable SetRegister (CSDISSET)
The CSDISSET register, shown inFigure 2-18 anddescribed inTable 2-30,sets clock sources tothe
disabled state.
Figure 2-18. Clock Source Disable SetRegister (CSDISSET) (offset =34h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
SETCLKSR7
OFFSETCLKSR6
OFFSETCLKSR5
OFFSETCLKSR4
OFFSETCLKSR3
OFFReserved SETCLKSR1
OFFSETCLKSR0
OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-30. Clock Source Disable SetRegister (CSDISSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 SETCLKSR[7-3]OFF Setclock source[7-3] tothedisabled state.
0 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] isunchanged.
1 Read: Clock source[7-3] isdisabled.
Write: Clock source[7-3] issettothedisabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 SETCLKSR[1-0]OFF Setclock source[1-0] tothedisabled state.
0 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] isunchanged.
1 Read: Clock source[1-0] isdisabled.
Write: Clock source[1-0] issettothedisabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 160 -->

System andPeripheral Control Registers www.ti.com
160 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.12 Clock Source Disable Clear Register (CSDISCLR)
The CSDISCLR register, shown inFigure 2-19 anddescribed inTable 2-31,clears clock sources tothe
enabled state.
Figure 2-19. Clock Source Disable Clear Register (CSDISCLR) (offset =38h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLRCLKSR7
OFFCLRCLKSR6
OFFCLRCLKSR5
OFFCLRCLKSR4
OFFCLRCLKSR3
OFFReserved CLRCLKSR1
OFFCLRCLKSR0
OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-31. Clock Source Disable Clear Register (CSDISCLR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 CLRCLKSR[7-3]OFF Enables clock source[7-3].
0 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] isunchanged.
1 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] issettotheenabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 CLRCLKSR[1-0]OFF Enables clock source[1-0].
0 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] isunchanged.
1 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] issettotheenabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h) andtheCSDISCLR register (offset 38h).
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 161 -->

www.ti.com System andPeripheral Control Registers
161 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.13 Clock Domain Disable Register (CDDIS)
The CDDIS register, shown inFigure 2-20 anddescribed inTable 2-32,controls thestate oftheclock
domains.
NOTE: Alltheclock domains areenabled onwakeup.
The application should assure thatwhen HCLK andVCLK_sys areturned offthrough the
HCLKOFF bit,theGCLK1 domain isalso turned off.
The register bitsinCDDIS aredesignated ashigh-integrity bitsandhave been implemented
with error-correcting logic such thateach bit,although read andwritten asasingle bit,is
actually amulti-bit keywith error correction capability. Assuch, single-bit flips within the"key"
canbecorrected allowing protection ofthesystem asawhole. Anerror detected issignaled
totheESM module.
Figure 2-20. Clock Domain Disable Register (CDDIS) (offset =3Ch)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved VCLKA4OFF Reserved Reserved VCLK3OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved RTICLK1OFF VCLKA2OFF VCLKA1OFF VCLK2OFF VCLKPOFF HCLKOFF GCLK1OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-32. Clock Domain Disable Register (CDDIS) Field Descriptions
Bit Field Value Description
31-12 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
11 VCLKA4OFF VCLKA4 domain off.
0 The VCLKA4 domain isenabled.
1 The VCLKA4 domain isdisabled.
10-9 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
8 VCLK3OFF VCLK3 domain off.
0 The VCLK3 domain isenabled.
1 The VCLK3 domain isdisabled.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 RTICLK1OFF RTICLK1 domain off.
0 The RTICLK1 domain isenabled.
1 The RTICLK1 domain isdisabled.
5-4 VCLKA[2-1]OFF VCLKA[2-1] domain off.
0 The VCLKA[2-1] domain isenabled.
1 The VCLKA[2-1] domain isdisabled.
3 VCLK2OFF VCLK2 domain off.
0 The VCLK2 domain isenabled.
1 The VCLK2 domain isdisabled.
2 VCLKPOFF VCLK_periph domain off.
0 The VCLK_periph domain isenabled.
1 The VCLK_periph domain isdisabled.

<!-- Page 162 -->

System andPeripheral Control Registers www.ti.com
162 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-32. Clock Domain Disable Register (CDDIS) Field Descriptions (continued)
Bit Field Value Description
1 HCLKOFF HCLK andVCLK_sys domains off.
0 The HCLK andVCLK_sys domains areenabled.
1 The HCLK andVCLK_sys domains aredisabled.
0 GCLK1OFF GCLK1 domain off.
0 The GCLK1 domain isenabled.
1 The GCLK1 domain isdisabled.

<!-- Page 163 -->

www.ti.com System andPeripheral Control Registers
163 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.14 Clock Domain Disable SetRegister (CDDISSET)
This CDDISSET register, shown inFigure 2-21 anddescribed inTable 2-33,sets clock domains tothe
disabled state.
Figure 2-21. Clock Domain Disable SetRegister (CDDISSET) (offset =40h)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved SETVCLKA4
OFFReserved Reserved SETVCLK3
OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved SETRTICLK1
OFFSETVCLKA2
OFFSETVCLKA1
OFFSETVCLK2
OFFSETVCLKP
OFFSETHCLK
OFFSETGCLK1
OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-33. Clock Domain Disable SetRegister (CDDISSET) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SETVCLKA4OFF SetVCLKA4 domain.
0 Read: The VCLKA4 domain isenabled.
Write: The VCLKA4 domain isunchanged.
1 Read: The VCLKA4 domain isdisabled.
Write: The VCLKA4 domain issettotheenabled state.
10-9 Reserved 0 Reads return zero oroneandprivilege mode writes allowed.
8 SETVCLK3OFF SetVCLK3 domain.
0 Read: The VCLK3 domain isenabled.
Write: The VCLK3 domain isunchanged.
1 Read: The VCLK3 domain isdisabled.
Write: The VCLK3 domain issettotheenabled state.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 SETRTICLK1OFF SetRTICLK1 domain.
0 Read: The RTICLK1 domain isenabled.
Write: The RTICLK1 domain isunchanged.
1 Read: The RTICLK1 domain isdisabled.
Write: The RTICLK1 domain issettotheenabled state.
5-4 SETVCLKA[2-1]OFF SetVCLKA[2-1] domain.
0 Read: The VCLKA[2-1] domain isenabled.
Write: The VCLKA[2-1] domain isunchanged.
1 Read: The VCLKA[2-1] domain isdisabled.
Write: The VCLKA[2-1] domain issettotheenabled state.
3 SETVCLK2OFF SetVCLK2 domain.
0 Read: The VCLK2 domain isenabled.
Write: The VCLK2 domain isunchanged.
1 Read: The VCLK2 domain isdisabled.
Write: The VCLK2 domain issettotheenabled state.

<!-- Page 164 -->

System andPeripheral Control Registers www.ti.com
164 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-33. Clock Domain Disable SetRegister (CDDISSET) Field Descriptions (continued)
Bit Field Value Description
2 SETVCLKPOFF SetVCLK_periph domain.
0 Read: The VCLK_periph domain isenabled.
Write: The VCLK_periph domain isunchanged.
1 Read: The VCLK_periph domain isdisabled.
Write: The VCLK_periph domain issettotheenabled state.
1 SETHCLKOFF SetHCLK andVCLK_sys domains.
0 Read: The HCLK andVCLK_sys domain isenabled.
Write: The HCLK andVCLK_sys domain isunchanged.
1 Read: The HCLK andVCLK_sys domain isdisabled.
Write: The HCLK andVCLK_sys domain issettotheenabled state.
0 SETGCLK1OFF SetGCLK1 domain.
0 Read: The GCLK1 domain isenabled.
Write: The GCLK1 domain isunchanged.
1 Read: The GCLK1 domain isdisabled.
Write: The GCLK1 domain issettotheenabled state.

<!-- Page 165 -->

www.ti.com System andPeripheral Control Registers
165 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.15 Clock Domain Disable Clear Register (CDDISCLR)
The CDDISCLR register, shown inFigure 2-22 anddescribed inTable 2-34,clears clock domains tothe
enabled state.
Figure 2-22. Clock Domain Disable Clear Register (CDDISCLR) (offset =44h)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved CLRVCLKA4
OFFReserved Reserved CLRVCLK3
OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved CLRRTICLK1
OFFCLRVCLKA2
OFFCLRVCLKA1
OFFCLRVCLK2
OFFCLRVCLKP
OFFCLRHCLK
OFFCLRGCLK1
OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-34. Clock Domain Disable Clear Register (CDDISCLR) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11 CLRVCLKA4OFF Clear VCLKA4 domain.
0 Read: The VCLKA4 domain isenabled.
Write: The VCLKA4 domain isunchanged.
1 Read: The VCLKA4 domain isdisabled.
Write: The VCLKA4 domain iscleared totheenabled state.
10-9 Reserved 0 Reads return zero oroneandprivilege mode writes allowed.
8 CLRVCLK3OFF Clear VCLK3 domain.
0 Read: The VCLK3 domain isenabled.
Write: The VCLK3 domain isunchanged.
1 Read: The VCLK3 domain isdisabled.
Write: The VCLK3 domain iscleared totheenabled state.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 CLRRTICLK1OFF Clear RTICLK1 domain.
0 Read: The RTICLK1 domain isenabled.
Write: The RTICLK1 domain isunchanged.
1 Read: The RTICLK1 domain isdisabled.
Write: The RTICLK1 domain iscleared totheenabled state.
5-4 CLRVCLKA[2-1]OFF Clear VCLKA[2-1] domain.
0 Read: The VCLKA[2-1] domain isenabled.
Write: The VCLKA[2-1] domain isunchanged.
1 Read: The VCLKA[2-1] domain isdisabled.
Write: The VCLKA[2-1] domain iscleared totheenabled state.
3 CLRVCLK2OFF Clear VCLK2 domain.
0 Read: The VCLK2 domain isenabled.
Write: The VCLK2 domain isunchanged.
1 Read: The VCLK2 domain isdisabled.
Write: The VCLK2 domain iscleared totheenabled state.

<!-- Page 166 -->

System andPeripheral Control Registers www.ti.com
166 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-34. Clock Domain Disable Clear Register (CDDISCLR) Field Descriptions (continued)
Bit Field Value Description
2 CLRVCLKPOFF Clear VCLK_periph domain.
0 Read: The VCLK_periph domain isenabled.
Write: The VCLK_periph domain isunchanged.
1 Read: The VCLK_periph domain isdisabled.
Write: The VCLK_periph domain iscleared totheenabled state.
1 CLRHCLKOFF Clear HCLK andVCLK_sys domains.
0 Read: The HCLK andVCLK_sys domain isenabled.
Write: The HCLK andVCLK_sys domain isunchanged.
1 Read: The HCLK andVCLK_sys domain isdisabled.
Write: The HCLK andVCLK_sys domain iscleared totheenabled state.
0 CLRGCLK1OFF Clear GCLK1 domain.
0 Read: The GCLK1 domain isenabled.
Write: The GCLK1 domain isunchanged.
1 Read: The GCLK1 domain isdisabled.
Write: The GCLK1 domain iscleared totheenabled state.

<!-- Page 167 -->

www.ti.com System andPeripheral Control Registers
167 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.16 GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC)
The GHVSRC register, shown inFigure 2-23 anddescribed inTable 2-35,controls theclock source
configuration fortheGCLK1, HCLK, VCLK andVCLK2 clock domains.
Figure 2-23. GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC) (offset =48h)
31 28 27 24 23 20 19 16
Reserved GHVWAKE Reserved HVLPM
R-0 R/WP-0 R-0 R/WP-0
15 4 3 0
Reserved GHVSRC
R-0 R/WP-0
LEGEND: R=Read only; R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-35. GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 GHVWAKE GCLK1, HCLK, VCLK source onwakeup.
0 Clock source0 isthesource forGCLK1, HCLK, VCLK onwakeup.
1h Clock source1 isthesource forGCLK1, HCLK, VCLK onwakeup.
2h Clock source2 isthesource forGCLK1, HCLK, VCLK onwakeup.
3h Clock source3 isthesource forGCLK1, HCLK, VCLK onwakeup.
4h Clock source4 isthesource forGCLK1, HCLK, VCLK onwakeup.
5h Clock source5 isthesource forGCLK1, HCLK, VCLK onwakeup.
6h Clock source6 isthesource forGCLK1, HCLK, VCLK onwakeup.
7h Clock source7 isthesource forGCLK1, HCLK, VCLK onwakeup.
8h-Fh Reserved
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 HVLPM HCLK, VCLK, VCLK2 source onwakeup when GCLK1 isturned off.
0 Clock source0 isthesource forHCLK, VCLK, VCLK2 onwakeup.
1h Clock source1 isthesource forHCLK, VCLK, VCLK2 onwakeup.
2h Clock source2 isthesource forHCLK, VCLK, VCLK2 onwakeup.
3h Clock source3 isthesource forHCLK, VCLK, VCLK2 onwakeup.
4h Clock source4 isthesource forHCLK, VCLK, VCLK2 onwakeup.
5h Clock source5 isthesource forHCLK, VCLK, VCLK2 onwakeup.
6h Clock source6 isthesource forHCLK, VCLK, VCLK2 onwakeup.
7h Clock source7 isthesource forHCLK, VCLK, VCLK2 onwakeup.
8h-Fh Reserved
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 GHVSRC GCLK1, HCLK, VCLK, VCLK2 current source.
Note: TheGHVSRC[3-0] bitsareupdated with theHVLPM[3-0] setting when GCLK1 is
turned off,andareupdated with theGHVWAKE[3-0] setting onsystem wakeup.
0 Clock source0 isthesource forGCLK1, HCLK, VCLK, VCLK2.
1h Clock source1 isthesource forGCLK1, HCLK, VCLK, VCLK2.
2h Clock source2 isthesource forGCLK1, HCLK, VCLK, VCLK2.
3h Clock source3 isthesource forGCLK1, HCLK, VCLK, VCLK2.
4h Clock source4 isthesource forGCLK1, HCLK, VCLK, VCLK2.
5h Clock source5 isthesource forGCLK1, HCLK, VCLK, VCLK2.
6h Clock source6 isthesource forGCLK1, HCLK, VCLK, VCLK2.
7h Clock source7 isthesource forGCLK1, HCLK, VCLK, VCLK2.
8h-Fh Reserved

<!-- Page 168 -->

System andPeripheral Control Registers www.ti.com
168 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureNOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown intheTable 2-29.
2.5.1.17 Peripheral Asynchronous Clock Source Register (VCLKASRC)
The VCLKASRC register, shown inFigure 2-24 anddescribed inTable 2-36,sets theclock source forthe
asynchronous peripheral clock domains tobeconfigured torunfrom aspecific clock source.
Figure 2-24. Peripheral Asynchronous Clock Source Register (VCLKASRC) (offset =4Ch)
31 28 27 24 23 20 19 16
Reserved Reserved Reserved Reserved
R-0 R/WP-1h R-0 R/WP-1h
15 12 11 8 7 4 3 0
Reserved VCLKA2S Reserved VCLKA1S
R-0 R/WP-9h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-36. Peripheral Asynchronous Clock Source Register (VCLKASRC) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 VCLKA2S Peripheral asynchronous clock2 source.
0 Clock source0 isthesource forperipheral asynchronous clock2.
1h Clock source1 isthesource forperipheral asynchronous clock2.
2h Clock source2 isthesource forperipheral asynchronous clock2.
3h Clock source3 isthesource forperipheral asynchronous clock2.
4h Clock source4 isthesource forperipheral asynchronous clock2.
5h Clock source5 isthesource forperipheral asynchronous clock2.
6h Clock source6 isthesource forperipheral asynchronous clock2.
7h Clock source7 isthesource forperipheral asynchronous clock2.
8h-Fh VCLK isthesource forperipheral asynchronous clock2.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 VCLKA1S Peripheral asynchronous clock1 source.
0 Clock source0 isthesource forperipheral asynchronous clock1.
1h Clock source1 isthesource forperipheral asynchronous clock1.
2h Clock source2 isthesource forperipheral asynchronous clock1.
3h Clock source3 isthesource forperipheral asynchronous clock1.
4h Clock source4 isthesource forperipheral asynchronous clock1.
5h Clock source5 isthesource forperipheral asynchronous clock1.
6h Clock source6 isthesource forperipheral asynchronous clock1.
7h Clock source7 isthesource forperipheral asynchronous clock1.
8h-Fh VCLK isthesource forperipheral asynchronous clock1.
NOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown inTable 2-29.

<!-- Page 169 -->

www.ti.com System andPeripheral Control Registers
169 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.18 RTIClock Source Register (RCLKSRC)
The RCLKSRC register, shown inFigure 2-25 anddescribed inTable 2-37,controls theRTI(Real Time
Interrupt) clock source selection.
NOTE: Important constraint when theRTIclock source isnotVCLK
IftheRTIx clock source ischosen tobeanything other than thedefault VCLK, then theRTI
clock needs tobeatleast three times slower than theVCLK. This canbeachieved by
configuring theRTIxCLK divider inthisregister. This divider isinternally bypassed when the
RTIx clock source isVCLK.
Figure 2-25. RTIClock Source Register (RCLKSRC) (offset =50h)
31 16
Reserved
R-0
15 10 9 8 7 4 3 0
Reserved RTI1DIV Reserved RTI1SRC
R-0 R/WP-1h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-37. RTIClock Source Register (RCLKSRC) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 RTI1DIV RTIclock1 Divider.
0 RTICLK1 divider value is1.
1h RTICLK1 divider value is2.
2h RTICLK1 divider value is4.
3h RTICLK1 divider value is8.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 RTI1SRC RTIclock1 source.
0 Clock source0 isthesource forRTICLK1.
1h Clock source1 isthesource forRTICLK1.
2h Clock source2 isthesource forRTICLK1.
3h Clock source3 isthesource forRTICLK1.
4h Clock source4 isthesource forRTICLK1.
5h Clock source5 isthesource forRTICLK1.
6h Clock source6 isthesource forRTICLK1.
7h Clock source7 isthesource forRTICLK1.
8h-Fh VCLK isthesource forRTICLK1.
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 170 -->

System andPeripheral Control Registers www.ti.com
170 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.19 Clock Source Valid Status Register (CSVSTAT)
The CSVSTAT register, shown inFigure 2-26 anddescribed inTable 2-38,indicates thestatus ofusable
clock sources.
Figure 2-26. Clock Source Valid Status Register (CSVSTAT) (offset =54h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLKSR7V CLKSR6V CLKSR5V CLKSR4V CLKSR3V Reserved CLKSR1V CLKSR0V
R-1 R-0 R-0 R-1 R-1 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 2-38. Clock Source Valid Register (CSVSTAT) Field Descriptions
Bit Field Value Description
31-8 Reserved. 0 Reads return 0.Writes have noeffect.
7-3 CLKSR[7-3]V Clock source[7-0] valid.
0 Clock source[7-0] isnotvalid.
1 Clock source[7-0] isvalid.
Note: Ifthevalid bitofthesource ofaclock domain isnotset(that is,theclock source is
notfully stable), therespective clock domain isdisabled bytheGlobal Clock Module
(GCM).
2 Reserved. 0 Reads return 0.Writes have noeffect.
1-0 CLKSR[1-0]V Clock source[1 -0]valid.
0 Clock source[1 -0]isnotvalid.
1 Clock source[1 -0]isvalid.
Note: Ifthevalid bitofthesource ofaclock domain isnotset(that is,theclock source is
notfully stable), therespective clock domain isdisabled.
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 171 -->

www.ti.com System andPeripheral Control Registers
171 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.20 Memory Self-Test Global Control Register (MSTGCR)
The MSTGCR register, shown inFigure 2-27 anddescribed inTable 2-39,controls several aspects ofthe
PBIST (Programmable Built-In Self Test) memory controller.
Figure 2-27. Memory Self-Test Global Control Register (MSTGCR) (offset =58h)
31 24 23 16
Reserved Reserved
R-0 R/WP-0
15 10 9 8 7 4 3 0
Reserved ROM_DIV Reserved MSTGENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R=Read only; R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-39. Memory Self-Test Global Control Register (MSTGCR) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 Reserved 0-1 Reads return 0or1anddepends onwhat iswritten inprivileged mode. The functionality of
these bitsareunavailable inthisdevice.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 ROM_DIV Prescaler divider bitsforROM clock source.
0 ROM clock source isGCLK1 divided by1.PBIST willreset for16VBUS cycles.
1h ROM clock source isGCLK1 divided by2.PBIST willreset for32VBUS cycles.
2h ROM clock source isGCLK1 divided by4.PBIST willreset for64VBUS cycles.
3h ROM clock source isGCLK1 divided by8.PBIST willreset for96VBUS cycles.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTGENA Memory self-test controller global enable key
Note: Enabling theMSTGENA keywillgenerate areset tothestate machine ofthe
selected PBIST controller.
Ah Memory self-test controller isenabled.
Others Memory self-test controller isdisabled.
Note: Itisrecommended thatavalue ofAhbeused todisable thememory self-test
controller. This value willgive maximum protection from abitflipinducing event that
would inadvertently enable thecontroller.

<!-- Page 172 -->

System andPeripheral Control Registers www.ti.com
172 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.21 Memory Hardware Initialization Global Control Register (MINITGCR)
The MINITGCR register, shown inFigure 2-28 anddescribed inTable 2-40,enables automatic hardware
memory initialization.
Figure 2-28. Memory Hardware Initialization Global Control Register (MINITGCR) (offset =5Ch)
31 16
Reserved
R-0
15 4 3 0
Reserved MINITGENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-40. Memory Hardware Initialization Global Control Register (MINITGCR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MINITGENA Memory hardware initialization global enable key.
Ah Global memory hardware initialization isenabled.
Others Global memory hardware initialization isdisabled.
Note: Itisrecommended thatavalue of5hbeused todisable memory hardware
initialization. This value willgive maximum protection from anevent thatwould
inadvertently enable thecontroller.

<!-- Page 173 -->

www.ti.com System andPeripheral Control Registers
173 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.22 MBIST Controller/ Memory Initialization Enable Register (MSINENA)
The MSINENA register, shown inFigure 2-29 anddescribed inTable 2-41,enables PBIST controllers for
memory selftestandthememory modules initialized during automatic hardware memory initialization.
Figure 2-29. MBIST Controller/Memory Initialization Enable Register (MSINENA) (offset =60h)
31 16
MSIENA
R/WP-0
15 0
MSIENA
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-41. MBIST Controller/Memory Initialization Enable Register (MSINENA) Field Descriptions
Bit Field Value Description
31-0 MSIENA PBIST controller andmemory initialization enable register. Inmemory self-test mode, allthe
corresponding bitsofthememories tobetested should besetbefore enabling theglobal memory self-
testcontroller key(MSTGENA) intheMSTGCR register (offset 58h). The reason forthisisthat
MSTGENA, inaddition tobeing theglobal enable forallindividual PBIST controllers, isthesource for
thereset generation toallthePBIST controller state machines. Disabling theMSTGENA or
MINITGENA key(bywriting from anAhtoanyother value) willreset alltheMSIENA[31-0] bitstotheir
default values.
0 Inmemory self-test mode (MSTGENA =Ah):
PBIST controller [31-0] isdisabled.
Inmemory Initialization mode (MINITGENA =Ah):
Memory module [31-0] auto hardware initialization isdisabled.
1 Inmemory self-test mode (MSTGENA =Ah):
PBIST controller [31-0] isenabled.
Inmemory Initialization mode (MINITGENA =Ah):
Memory module [31-0] auto hardware initialization isenabled.
Note: Software should ensure thatboth thememory self-test global enable key(MSTGENA) and
thememory hardware initialization global key(MINITGENA) arenotenabled atthesame time.

<!-- Page 174 -->

System andPeripheral Control Registers www.ti.com
174 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.23 MSTC Global Status Register (MSTCGSTAT)
The MSTCGSTAT register, shown inFigure 2-30 anddescribed inTable 2-42,shows thestatus ofthe
memory hardware initialization andthememory self-test.
Figure 2-30. MSTC Global Status Register (MSTCGSTAT) (offset =68h)
31 16
Reserved
R-0
15 9 8 7 1 0
Reserved MINIDONE Reserved MSTDONE
R-0 R/WPC-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; WP=Write inprivileged mode only; -n=value after reset
Table 2-42. MSTC Global Status Register (MSTCGSTAT) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 MINIDONE Memory hardware initialization complete status.
Note: Disabling theMINITGENA key(Bywriting from aAhtoanyother value) willclear the
MINIDONE status bitto0.
Note: Individual memory initialization status isshown intheMINISTAT register.
0 Read: Memory hardware initialization isnotcomplete forallmemory.
Write: Awrite of0hasnoeffect.
1 Read: Hardware initialization ofallmemory iscompleted.
Write: The bitiscleared to0.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 MSTDONE Memory self-test runcomplete status.
Note: Disabling theMSTGENA key(bywriting from aAhtoanyother value) willclear the
MSTDONE status bitto0.
0 Read: Memory self-test isnotcompleted.
Write: Awrite of0hasnoeffect.
1 Read: Memory self-test iscompleted.
Write: The bitiscleared to0.

<!-- Page 175 -->

www.ti.com System andPeripheral Control Registers
175 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.24 Memory Hardware Initialization Status Register (MINISTAT)
The MINISTAT register, shown inFigure 2-31 anddescribed inTable 2-43,indicates thestatus of
hardware memory initialization.
Figure 2-31. Memory Hardware Initialization Status Register (MINISTAT) (offset =6Ch)
31 16
MIDONE
R/WP-0
15 0
MIDONE
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-43. Memory Hardware Initialization Status Register (MINISTAT) Field Descriptions
Bit Field Value Description
31-0 MIDONE Memory hardware initialization status bit.
0 Read: Memory module[31-0] hardware initialization isnotcompleted.
Write: Awrite of0hasnoeffect.
1 Read: Memory module[31-0] hardware initialization iscompleted.
Write: The bitiscleared to0.
Note: Disabling theMINITGENA key(bywriting from aAhtoanyother value) willreset allthe
individual status bitsto0.
2.5.1.25 PLL Control Register 1(PLLCTL1)
The PLLCTL1 register, shown inFigure 2-32 anddescribed inTable 2-44,controls theoutput frequency of
PLL1 (Clock Source 1-FMzPLL). Italso controls thebehavior ofthedevice ifaPLL sliporoscillator
failure isdetected.
Figure 2-32. PLL Control Register 1(PLLCTL1) (offset =70h)
31 30 29 28 24
ROS BPOS PLLDIV
R/WP-0 R/WP-1h R/WP-Fh
23 22 21 16
ROF Reserved REFCLKDIV
R/WP-0 R-0 R/WP-3h
15 0
PLLMUL
R/WP-4100h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset

<!-- Page 176 -->

System andPeripheral Control Registers www.ti.com
176 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-44. PLL Control Register 1(PLLCTL1) Field Descriptions
Bit Field Value Description
31 ROS Reset onPLL Slip.
0 Donotreset system when PLL slipisdetected.
1 Reset when PLL slipisdetected.
Note: BPOS (Bits 30-29) must also beenabled forROS tobeenabled.
30-29 BPOS Bypass ofPLL Slip.
2h Bypass onPLL Slipisdisabled. IfaPLL Slipisdetected noaction istaken.
Others Bypass onPLL Slipisenabled. IfaPLL Slipisdetected thedevice willautomatically bypass the
PLL andusetheoscillator toprovide thedevice clock.
Note: IfROS (Bit31)issetto1,thedevice willbereset ifaPLL SlipandthePLL willbe
bypassed after thereset occurs.
28-24 PLLDIV PLL Output Clock Divider
R=PLLDIV +1
fPLL CLK=fpost_ODCLK /R
0 fPLL CLK=fpost-ODCLK /1
1h fPLL CLK=fpost-ODCLK /2
: :
1Fh fPLL CLK=fpost-ODCLK /32
23 ROF Reset onOscillator Fail.
0 Donotreset system when oscillator isoutofrange.
1 The ROF bitenables theOSC_FAIL condition togenerate asystem reset. IftheROF bitinthe
PLLCTL1 register issetwhen theoscillator fails, then asystem reset occurs.
22 Reserved 0 Value hasnoeffect onPLL operation.
21-16 REFCLKDIV Reference Clock Divider
NR=REFCLKDIV +1
fINT CLK=fOSCIN /NR
0 fINT CLK=fOSCIN /1
1h fINT CLK=fOSCIN /2
: :
3Fh fINT CLK=fOSCIN /64
15-0 PLLMUL PLL Multiplication Factor
NF=(PLLMUL /256) +1,valid multiplication factors arefrom 1to256.
fVCO CLK=fINT CLKxNF
0h fVCO CLK=fINT CLKx1
100h fVCO CLK=fINT CLKx2
: :
5B00h fVCO CLK=fINT CLKx92
5C00h fVCO CLK=fINT CLKx93
: :
FF00h fVCO CLK=fINT CLKx256

<!-- Page 177 -->

www.ti.com System andPeripheral Control Registers
177 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.26 PLL Control Register 2(PLLCTL2)
The PLLCTL2 register, shown inFigure 2-33 anddescribed inTable 2-45,controls themodulation
characteristics andtheoutput divider ofthePLL.
Figure 2-33. PLL Control Register 2(PLLCTL2) (offset =74h)
31 30 22 21 20 16
FMENA SPREADINGRATE Rsvd MULMOD
R/WP-0 R/WP-1FFh R-0 R/WP-0
15 12 11 9 8 0
MULMOD ODPLL SPR_AMOUNT
R/WP-7h R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-45. PLL Control Register 2(PLLCTL2) Field Descriptions
Bit Field Value Description
31 FMENA Frequency Modulation Enable.
0 Disable frequency modulation.
1 Enable frequency modulation.
30-22 SPREADINGRATE NS=SPREADINGRATE +1
fmod=fs=fINT CLK/(2×NS)
0 fmod=fs=fINT CLK/(2×1)
1h fmod=fs=fINT CLK/(2×2)
: :
1FFh fmod=fs=fINT CLK/(2×512)
21 Reserved 0 Value hasnoeffect onPLL operation.
20-12 MULMOD Multiplier Correction when Frequency Modulation isenabled.
When FMENA =0,MUL_when_MOD =0;when FMENA =1,MUL_when_MOD =
(MULMOD /256)
0 Noadder toNF.
8h MUL_when_MOD =8/256
9h MUL_when_MOD =9/256
: :
1FFh MUL_when_MOD =511/256
11-9 ODPLL Internal PLL Output Divider
OD=ODPLL +1
fpost-ODCLK =fVCO CLK/OD
Note: PLL output clock isgated off,ifODPLL ischanged while thePLL isactive.
0 fpost-ODCLK =fVCO CLK/1
1h fpost-ODCLK =fVCO CLK/2
: :
7h fpost-ODCLK =fVCO CLK/8
8-0 SPR_AMOUNT Spreading Amount
NV=(SPR_AMOUNT +1)/2048
NVranges from 1/2048 to512/2048
Note thatthePLL output clock isdisabled for1modulation period, iftheSPR_AMOUNT
field ischanged while thefrequency modulation isenabled. Iffrequency modulation is
disabled andSPR_AMOUNT ischanged, there isnoeffect onthePLL output clock.
0 NV=1/2048
1h NV=2/2048
: :
1FFh NV=512/2048

<!-- Page 178 -->

System andPeripheral Control Registers www.ti.com
178 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.27 SYS PinControl Register 10(SYSPC10)
The SYSPC10 register, shown inFigure 2-34 anddescribed inTable 2-46,controls thefunction ofthe
ECPCLK slew mode.
Figure 2-34. SYS PinControl Register 10(SYSPC10) (offset =78h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLK_SLEW
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-46. SYS PinControl Register 10(SYSPC10) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLK_SLEW ECPCLK slew control. This bitcontrols between thefastorslow slew mode.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.

<!-- Page 179 -->

www.ti.com System andPeripheral Control Registers
179 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.28 DieIdentification Register Lower Word (DIEIDL)
The DIEIDL register, shown inFigure 2-35 anddescribed inTable 2-47,contains information about thedie
wafer number, andX,Ywafer coordinates.
Figure 2-35. DieIdentification Register, Lower Word (DIEIDL) [offset =7Ch]
31 24 23 16
WAFER # YWAFER COORDINATE
R-D R-D
15 12 11 0
YWAFER COORDINATE XWAFER COORDINATE
R-D R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-47. DieIdentification Register, Lower Word (DIEIDL) Field Descriptions
Bit Field Description
31-24 WAFER # These read-only bitscontain thewafer number ofthedevice.
23-12 YWAFER COORDINATE These read-only bitscontain theYwafer coordinate ofthedevice.
11-0 XWAFER COORDINATE These read-only bitscontain theXwafer coordinate ofthedevice.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.
2.5.1.29 DieIdentification Register Upper Word (DIEIDH)
The DIEIDH register, shown inFigure 2-36 anddescribed inTable 2-48,contains information about the
dielotnumber.
Figure 2-36. DieIdentification Register, Upper Word (DIEIDH) [offset =80h]
31 24 23 16
Reserved LOT #
R-0 R-D
15 0
LOT #
R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-48. DieIdentification Register, Upper Word (DIEIDH) Field Descriptions
Bit Field Description
31-24 Reserved Reserved forTIuse. Writes have noeffect.
23-0 LOT # This read-only register contains thedevice lotnumber.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.

<!-- Page 180 -->

System andPeripheral Control Registers www.ti.com
180 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.30 LPO/Clock Monitor Control Register (LPOMONCTL)
The LPOMONCTL register, shown inFigure 2-37 anddescribed inTable 2-49,controls theLow
Frequency (Clock Source 4)andHigh Frequency (Clock Source 5)Low Power Oscillator's trimvalues.
Figure 2-37. LPO/Clock Monitor Control Register (LPOMONCTL) (offset =088h)
31 25 24 23 17 16
Reserved BIAS ENABLE Reserved OSCFRQCONFIGCNT
R-0 R/WP-1 R-0 R/WP-0
15 13 12 8 7 5 4 0
Reserved HFTRIM Reserved LFTRIM
R-0 R/WP-10h R-0 R/WP-10h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 BIAS ENABLE Bias enable.
0 The bias circuit inside thelow-power oscillator (LPO) isdisabled.
1 The bias circuit inside thelow-power oscillator (LPO) isenabled.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 OSCFRQCONFIGCNT Configures thecounter based onOSC frequency.
0 Read: OSC freq is≤20MHz.
Write: Awrite of0hasnoeffect.
1 Read: OSC freq is>20MHz and≤80MHz.
Write: Awrite of1hasnoeffect.
15-13 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 181 -->

www.ti.com System andPeripheral Control Registers
181 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions (continued)
Bit Field Value Description
12-8 HFTRIM High-frequency oscillator trimvalue. This four-bit value isused tocenter theHF
oscillator 'sfrequency.
Caution: This value should only bechanged when theHFoscillator isnotthe
source foraclock domain, otherwise asystem failure could result.
The following values aretheratio: f/fointheF021 process.
0 29.52
1h 34.24%
2h 38.85%
3h 43.45%
4h 47.99%
5h 52.55%
6h 57.02%
7h 61.46%
8h 65.92%
9h 70.17
Ah 74.55%
Bh 78.92%
Ch 83.17%
Dh 87.43%
Eh 91.75%
Fh 95.89%
10h 100.00% Default atReset.
11h 104.09
12h 108.17
13h 112.32
14h 116.41
15h 120.67
16h 124.42
17h 128.38
18h 132.24
19h 136.15
1Ah 140.15
1Bh 143.94
1Ch 148.02
1Dh 151.80x
1Eh 155.50x
1Fh 159.35%
7-5 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 182 -->

System andPeripheral Control Registers www.ti.com
182 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions (continued)
Bit Field Value Description
4-0 LFTRIM Low-frequency oscillator trimvalue. This four-bit value isused tocenter theLFoscillator 's
frequency.
Caution: This value should only bechanged when theLFoscillator isnotthe
source foraclock domain, otherwise asystem failure could result.
The following values aretheratio: f/fointheF021 process.
0 20.67
1h 25.76
2h 30.84
3h 35.90
4h 40.93
5h 45.95
6h 50.97
7h 55.91
8h 60.86
9h 65.78
Ah 70.75
Bh 75.63
Ch 80.61
Dh 85.39
Eh 90.23
Fh 95.11
10h 100.00% Default atReset
11h 104.84
12h 109.51
13h 114.31
14h 119.01
15h 123.75
16h 128.62
17h 133.31
18h 138.03
19h 142.75
1Ah 147.32
1Bh 152.02
1Ch 156.63
1Dh 161.38
1Eh 165.90
1Fh 170.42

<!-- Page 183 -->

www.ti.com System andPeripheral Control Registers
183 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.31 Clock Test Register (CLKTEST)
The CLKTEST register, shown inFigure 2-38 anddescribed inTable 2-50,controls theclock signal thatis
supplied totheECLK pinfortestanddebug purposes.
NOTE: Clock Test Register Usage
This register should only beused fortestanddebug purposes.
Figure 2-38. Clock Test Register (CLKTEST) (offset =8Ch)
31 27 26 25 24
Reserved TEST RANGEDET
CTRLRANGEDET
ENASSEL
R-0 R/WP-0 R/WP-0 R/WP-0
23 20 19 16
Reserved CLK_TEST_EN
R-0 R/WP-Ah
15 12 11 8 7 5 4 0
Reserved SEL_GIO_PIN Reserved SEL_ECP_PIN
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-50. Clock Test Register (CLKTEST) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 TEST 0 This bitisused fortestpurposes. Itmust bewritten to0.
25 RANGEDETCTRL Range detection control. This bit'sfunctionality isdependant onthestate ofthe
RANGEDETENSSEL bit(Bit24)oftheCLKTEST register.
0 The clock monitor range detection circuitry (RANGEDETECTENABLE) isdisabled.
1 The clock monitor range detection circuitry (RANGEDETECTENABLE) isenabled.
24 RANGEDETENASSEL Selects range detection enable. This bitresets asynchronously onpower onreset.
0 The range detect enable isgenerated bythehardware intheclock monitor wrapper.
1 The range detect enable iscontrolled bytheRANGEDETCTRL bit(Bit25)ofthe
CLKTEST register.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 CLK_TEST_EN Clock testenable. This bitenables theclock going totheECLK pin.This bitfield enables
ordisables clock going todevice pins. Two pins inadevice cangetclock sources by
enabling CLK_TEST_EN bits. One pinistheECP andsecond pinisadevice specific GIO
pin.These bitsneed toasynchronously reset.
Note: TheECLK pinmust also beplaced intoFunctional mode bysetting the
ECPCLKFUN bitto1intheSYSPC1 register.
5h Clock going toECLK pinisenabled.
Others Clock going toECLK pinisdisabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 184 -->

System andPeripheral Control Registers www.ti.com
184 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-50. Clock Test Register (CLKTEST) Field Descriptions (continued)
Bit Field Value Description
11-8 SEL_GIO_PIN GIOB[0] pinclock source valid, clock source select
0 Oscillator valid status
1h PLL1 valid status
2h-4h Reserved
5h High-frequency LPO (Low-Power Oscillator) clock output valid status [CLK10M]
6h PLL2 valid status
7h Reserved
8h Low-frequency LPO (Low-Power Oscillator) clock output valid status [CLK80K]
9h-Ch Oscillator valid status
Dh Reserved
Eh VCLKA4
Fh Oscillator valid status
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 SEL_ECP_PIN ECLK pinclock source select
Note: Only valid clock sources canbeselected fortheECLK pin.Valid clock
sources aredisplayed bytheCSVSTAT register.
0 Oscillator clock
1h PLL1 clock output
2h Reserved
3h EXTCLKIN1
4h Low-frequency LPO (Low-Power Oscillator) clock [CLK80K]
5h High-frequency LPO (Low-Power Oscillator) clock [CLK10M]
6h PLL2 clock output
7h EXTCLKIN2
8h GCLK1
9h RTI1 Base
Ah Reserved
Bh VCLKA1
Ch VCLKA2
Dh Reserved
Eh VCLKA4_DIVR
Fh Flash HDPump Oscillator
10h Reserved
11h HCLK
12h VCLK
13h VCLK2
14h VCLK3
15h-16h Reserved
17h EMAC clock output
18h-1Fh Reserved
NOTE: Non-implemented clock sources should notbeenabled orused.

<!-- Page 185 -->

www.ti.com System andPeripheral Control Registers
185 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.32 DFT Control Register (DFTCTRLREG)
This register isshown inFigure 2-39 anddescribed inTable 2-51.
Figure 2-39. DFT Control Register (DFTCTRLREG) (offset =90h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 4 3 0
Reserved DFTWRITE Reserved DFTREAD Reserved TEST_MODE_KEY
R-0 R/WP-1h R-0 R/WP-1h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-51. DFT Control Register (DFTCTRLREG) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reads return 0.Writes have noeffect.
13-12 DFTWRITE DFT logic access.
ForF021:
DFTWRITE[0] =0andDFTREAD[0] =0configured instress mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured instress mode.
DFTWRITE[0] =0andDFTREAD[0] =0configured infastmode.
DFTWRITE[1] =1andDFTREAD[1] =1configured infastmode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inslow mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured inslow mode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inscreen mode.
DFTWRITE[1] =1andDFTREAD[1] =1configured inscreen mode.
11-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 DFTREAD DFT logic access.
ForF021:
DFTWRITE[0] =0andDFTREAD[0] =0configured instress mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured instress mode.
DFTWRITE[0] =0andDFTREAD[0] =0configured infastmode.
DFTWRITE[1] =1andDFTREAD[1] =1configured infastmode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inslow mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured inslow mode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inscreen mode.
DFTWRITE[1] =1andDFTREAD[1] =1configured inscreen mode.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 TEST_MODE_KEY Test mode key. This register isforinternal TIuseonly.
0-Fh
(except Ah)Register keydisable. Allbitsinthisregister willmaintain their default value andcannot be
written.
Ah Register keyenable. ALL thebitscanbewritten toonly when thekeyisenabled. Onreset,
these bitswillbesetto5h.

<!-- Page 186 -->

System andPeripheral Control Registers www.ti.com
186 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.33 DFT Control Register 2(DFTCTRLREG2)
This register isshown inFigure 2-40 anddescribed inTable 2-52.Forinformation onfiltering theRFSLIP
seeSection 2.5.2.7 .
Figure 2-40. DFT Control Register 2(DFTCTRLREG2) (offset =94h)
31 16
IMPDF(27:12)
R/WP-0
15 4 3 0
IMPDF(11:0) TEST_MODE_KEY
R/WP-0 R/WP-5h
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-52. DFT Control Register 2(DFTCTRLREG2) Field Descriptions
Bit Field Value Description
31-4 IMPDF[27:0] DFT Implementation defined bits.
0 IMPDF[27:0] isdisabled.
1 IMPDF[27:0] isenabled.
3-0 TEST_MODE_KEY Test mode key. This register isforinternal TIuseonly.
0-Fh
(except Ah)Register keydisable. Allbitsinthisregister willmaintain their default value andcannot be
written.
Ah Register keyenable. ALL thebitscanbewritten toonly when thekeyisenabled.

<!-- Page 187 -->

www.ti.com System andPeripheral Control Registers
187 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.34 General Purpose Register (GPREG1)
This register isshown inFigure 2-41 anddescribed inTable 2-53.Forinformation onfiltering theRFSLIP,
seeSection 2.5.2.7 .
Figure 2-41. General Purpose Register (GPREG1) (offset =A0h)
31 26 25 20 19 16
Reserved PLL1_FBSLIP_FILTER_COUNT PLL1_FBSLIP_FILTER_KEY
R-0 R/WP-0 R/WP-0
15 0
Reserved
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-53. General Purpose Register (GPREG1) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25-20 PLL1_FBSLIP_FILTER_
COUNTFBSLIP down counter programmed value.
Configures thesystem response when aFBSLIP isindicated bythePLL macro.
When PLL1_FBSLIP_FILTER_KEY isnotAh,thedown counter counts from the
programmed value onevery LPO high-frequency clock once PLL macro indicates
FBSLIP. When thecount reaches 0,ifthesynchronized FBSLIP signal isstillhigh, an
FBSLIP condition isindicated tothesystem module andiscaptured intheglobal
status register. When theFBSLIP signal from thePLL macro isde-asserted before
thecount reaches 0,thecounter isreloaded with theprogrammed value.
Onreset, counter value is0.Counter must beprogrammed toanon-zero value and
enabled forthefiltering tobeenabled.
0 Filtering isdisabled.
1h Filtering isenabled. Every slipisrecognized.
2h Filtering isenabled. The slipmust beatleast 2HFLPO cycles wide inorder tobe
recognized asaslip.
: :
3Fh Filtering isenabled. The slipmust beatleast 63HFLPO cycles wide inorder tobe
recognized asaslip.
19-16 PLL1_FBSLIP_FILTER_
KEYEnable theFBSLIP filtering.
5h Onreset, theFBSLIP filter isdisabled andtheFBSLIP passes through.
Fh This isanunsupported value. You should avoid writing thisvalue tothisbitfield.
Allother
valuesFBSLIP filtering isenabled. Recommended toprogram Ahinthisbitfield. Enabling of
theFBSLIP occurs when theKEY isprogrammed andanon-zero value ispresent in
theCOUNT field.
15-0 Reserved 0-1 Reads return 0or1andwrite inprivilege mode. The functionality ofthisbitis
unavailable inthisdevice.

<!-- Page 188 -->

System andPeripheral Control Registers www.ti.com
188 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.35 System Software Interrupt Request 1Register (SSIR1)
The SSIR1 register, shown inFigure 2-42 anddescribed inTable 2-54,isused forsoftware interrupt
generation.
Figure 2-42. System Software Interrupt Request 1Register (SSIR1) (offset =B0h)
31 16
Reserved
R-0
15 8 7 0
SSKEY1 SSDATA1
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-54. System Software Interrupt Request 1Register (SSIR1) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY1 0-FFh System software interrupt request key. A075h written tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY1 field canbewritten intoonly ifthewrite data
matches thekey(75h). The SSDATA1 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY1 field, matches thekey(75h).
7-0 SSDATA1 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA1 field
cannot bewritten intounless thewrite data intotheSSKEY1 field matches thekey(75h);
therefore, byte writes cannot beperformed ontheSSDATA1 field.
NOTE: This register ismirrored atoffset FCh forcompatibility reasons.

<!-- Page 189 -->

www.ti.com System andPeripheral Control Registers
189 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.36 System Software Interrupt Request 2Register (SSIR2)
The SSIR2 register, shown inFigure 2-43 anddescribed inTable 2-55,isused forsoftware interrupt
generation.
Figure 2-43. System Software Interrupt Request 2Register (SSIR2) (offset =B4h)
31 16
Reserved
R-0
15 8 7 0
SSKEY2 SSDATA2
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-55. System Software Interrupt Request 2Register (SSIR2) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY2 0-FFh System software interrupt2 request key. A84hwritten tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY2 field canbewritten intoonly ifthewrite data
matches thekey(84h). The SSDATA2 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY2 field, matches thekey(84h).
7-0 SSDATA2 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA2 field
cannot bewritten intounless thewrite data intotheSSKEY2 field matches thekey(84h);
therefore, byte writes cannot beperformed ontheSSDATA2 field.

<!-- Page 190 -->

System andPeripheral Control Registers www.ti.com
190 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.37 System Software Interrupt Request 3Register (SSIR3)
The SSIR3 register, shown inFigure 2-44 anddescribed inTable 2-56,isused forsoftware interrupt
generation.
Figure 2-44. System Software Interrupt Request 3Register (SSIR3) (offset =B8h)
31 16
Reserved
R-0
15 8 7 0
SSKEY3 SSDATA3
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-56. System Software Interrupt Request 3Register (SSIR3) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY3 0-FFh System software interrupt request key. A93hwritten tothese bitsinitiates IRQ/FIQ interrupts. Data
inthisfield isalways read as0.The SSKEY3 field canbewritten intoonly ifthewrite data
matches thekey(93h). The SSDATA3 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY3 field, matches thekey(93h).
7-0 SSDATA3 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA3 field
cannot bewritten intounless thewrite data intotheSSKEY3 field matches thekey(93h);
therefore, byte writes cannot beperformed ontheSSDATA3 field.

<!-- Page 191 -->

www.ti.com System andPeripheral Control Registers
191 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.38 System Software Interrupt Request 4Register (SSIR4)
The SSIR4 register, shown inFigure 2-45 anddescribed inTable 2-57,isused forsoftware interrupt
generation.
Figure 2-45. System Software Interrupt Request 4Register (SSIR4) (offset =BCh)
31 16
Reserved
R-0
15 8 7 0
SSKEY4 SSDATA4
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-57. System Software Interrupt Request 4Register (SSIR4) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY4 0-FFh System software interrupt2 request key. AA2h written tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY4 field canbewritten intoonly ifthewrite data
matches thekey(A2h). The SSDATA4 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY4 field, matches thekey(A2h).
7-0 SSDATA4 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA4 field
cannot bewritten intounless thewrite data intotheSSKEY4 field matches thekey(A2h);
therefore, byte writes cannot beperformed ontheSSDATA4 field.

<!-- Page 192 -->

System andPeripheral Control Registers www.ti.com
192 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.39 RAM Control Register (RAMGCR)
NOTE: TheRAM_DFT_EN bitsareforTIinternal useonly.
The contents oftheRAM_DFT_EN field should notbechanged.
Figure 2-46. RAM Control Register (RAMGCR) (offset =C0h)
31 20 19 16
Reserved RAM_DFT_EN
R-0 R/WP-5h
15 14 13 12 11 10 9 8
Reserved Reserved Reserved Reserved Reserved Reserved Reserved Reserved
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved Reserved Reserved Reserved Reserved Reserved Reserved Reserved
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-58. RAM Control Register (RAMGCR) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 RAM_DFT_EN Functional mode RAM DFT (Design ForTest) port enable key.
Note: ForTIinternal useonly.
Ah RAM DFT port isenabled.
Others RAM DFT port isdisabled.
Note: Itisrecommended thatavalue of5hbeused todisable theRAM DFT port. This value
willgive maximum protection from abit-flip inducing event thatwould inadvertently enable
thecontroller.
15 Reserved 0 Reads return 0.Writes have noeffect.
14 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
13 Reserved 0 Reads return 0.Writes have noeffect.
12 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
11 Reserved 0 Reads return 0.Writes have noeffect.
10 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
9 Reserved 0 Reads return 0.Writes have noeffect.
8 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
3 Reserved 0 Reads return 0.Writes have noeffect.
2 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
1 Reserved 0 Reads return 0.Writes have noeffect.
0 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.

<!-- Page 193 -->

www.ti.com System andPeripheral Control Registers
193 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.40 Bus Matrix Module Control Register 1(BMMCR1)
The BMMCR1 register, shown inFigure 2-47 anddescribed inTable 2-59,allows RAM andProgram
(Flash) memory addresses tobeswapped.
Figure 2-47. Bus Matrix Module Control Register 1(BMMCR) (offset =C4h)
31 16
Reserved
R-0
15 4 3 0
Reserved MEMSW
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-59. Bus Matrix Module Control Register 1(BMMCR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MEMSW Memory swap key.
Note: ACPU reset must beissued after thememory swap keyhasbeen changed forthe
memory swap tooccur. ACPU reset canbeinitiated bychanging thestate oftheCPU
RESET bitintheCPURSTCR register.
Ah Default memory-map:
Program memory (Flash) starts ataddress 0.eSRAM starts ataddress 8000000h.
5h Swapped memory-map:
eSRAM starts ataddress 0.Program memory (Flash) starts ataddress 8000000h.
Others The device memory-map isunchanged.

<!-- Page 194 -->

System andPeripheral Control Registers www.ti.com
194 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.41 CPU Reset Control Register (CPURSTCR)
The CPURSTCR register shown inFigure 2-48 anddescribed inTable 2-60 allows areset totheCortex-
R5F CPU tobegenerated.
NOTE: The register bitsinCPURSTCR aredesignated ashigh-integrity bitsandhave been
implemented with error-correcting logic such thateach bit,although read andwritten asa
single bit,isactually amulti-bit keywith error correction capability. Assuch, single-bit flips
within the"key"canbecorrected allowing protection ofthesystem asawhole. Anerror
detected issignaled totheESM module.
Figure 2-48. CPU Reset Control Register (CPURSTCR) (offset =CCh)
31 17 16
Reserved Reserved
R-0 R/WP-0
15 1 0
Reserved CPU RESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-60. CPU Reset Control Register (CPURSTGCR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 CPU RESET CPU RESET.
Only theCPU isreset whenever thisbitistoggled. There isnosystem reset.

<!-- Page 195 -->

www.ti.com System andPeripheral Control Registers
195 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.42 Clock Control Register (CLKCNTL)
The CLKCNTL register, shown inFigure 2-49 anddescribed inTable 2-61,controls peripheral reset and
theperipheral clock divide ratios.
NOTE: VCLK andVCLK2 clock ratio restrictions.
The VCLK2 frequency must always begreater than orequal totheVCLK frequency. The
VCLK2 frequency must beaninteger multiple oftheVCLK frequency.
Inaddition, theVCLK andVCLK2 clock ratios must notbechanged simultaneously. When
increasing thefrequency (decreasing thedivider), firstchange theVCLK2R field andthen
change theVCLKR field. When reducing thefrequency (increasing thedivider), firstchange
theVCLKR field andthen change theVCLK2R field.
You should doaread-back between thetwowrites. This assures thatthere areenough clock
cycles between thetwowrites.
Figure 2-49. Clock Control Register (CLKCNTL) (offset =D0h)
31 28 27 24 23 20 19 16
Reserved VCLK2R Reserved VCLKR
R-0 R/WP-1h R-0 R/WP-1h
15 9 8 7 0
Reserved PENA Reserved
R-0 R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-61. Clock Control Register (CLKCNTL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 VCLK2R VBUS clock2 ratio.
Note: TheVCLK2 frequency must always begreater than orequal totheVCLK frequency.
TheVCLK2 frequency must beaninteger multiple oftheVCLK frequency. Inaddition, the
VCLK andVCLK2 clock ratios must notbechanged simultaneously.
0 The VCLK2 speed isHCLK divided by1.
: :
Fh The VCLK2 speed isHCLK divided by16.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 VCLKR VBUS clock ratio.
Note: TheVCLK2 frequency must always begreater than orequal totheVCLK frequency.
TheVCLK2 frequency must beaninteger multiple oftheVCLK frequency. Inaddition, the
VCLK andVCLK2 clock ratios must notbechanged simultaneously.
0 The VCLK speed isHCLK divided by1.
: :
Fh The VCLK speed isHCLK divided by16.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PENA Peripheral enable bit.The application must setthisbitbefore accessing anyperipheral.
0 The global peripheral/peripheral memory frames areinreset.
1 Allperipheral/peripheral memory frames areoutofreset.
7-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 196 -->

/c40 /c41 1 E C P D IVN V C L K o rO S C IE C L K/c43/c61
System andPeripheral Control Registers www.ti.com
196 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.43 ECP Control Register (ECPCNTL)
The ECP register, shown inFigure 2-50 anddescribed inTable 2-62,configures theECLK pinin
functional mode.
NOTE: ECLK Functional mode configuration.
The ECLK pinmust beplaced intoFunctional mode bysetting theECPCLKFUN bitto1in
theSYSPC1 register before aclock source willbevisible ontheECLK pin.
Figure 2-50. ECP Control Register (ECPCNTL) (offset =D4h)
31 25 24 23 22 18 17 16
Reserved ECPSSEL ECPCOS Reserved Reserved
R-0 R/W-0 R/W-0 R-0 R/W-0
15 0
ECPDIV
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-62. ECP Control Register (ECPCNTL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ECPSSEL This bitallows theselection between VCLK andOSCIN astheclock source forECLK.
Note: Other ECLK clock sources areavailable fordebug purposes byconfiguring the
CLKTEST register.
0 VCLK isselected astheECP clock source.
1 OSCIN isselected astheECP clock source.
23 ECPCOS ECP continue onsuspend.
Note: Suspend mode isentered while performing certain JTAG debugging operations.
0 ECLK output isdisabled insuspend mode. ECLK output willbeshut offandwillnotbeseen on
theI/Opinofthedevice.
1 ECLK output isnotdisabled insuspend mode. ECLK output willnotbeshut offandwillbeseen
ontheI/Opinofthedevice.
22-18 Reserved 0 Reads return 0.Writes have noeffect.
17-16 Reserved 0 Reads return 0or1depends onwhat iswritten. The functionality ofthisbitisunavailable inthis
device.
15-0 ECPDIV 0-FFFFh ECP divider value. The value ofECPDIV bitsdetermine theexternal clock (ECP clock) frequency
asaratio ofVBUS clock orOSCIN asshown intheformula:

<!-- Page 197 -->

www.ti.com System andPeripheral Control Registers
197 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.44 DEV Parity Control Register 1(DEVCR1)
This register isshown inFigure 2-51 anddescribed inTable 2-63.
Figure 2-51. DEV Parity Control Register 1(DEVCR1) (offset =DCh)
31 16
Reserved
R-0
15 4 3 0
Reserved DEVPARSEL
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-63. DEV Parity Control Register 1(DEVCR1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 DEVPARSEL Device parity select bitkey.
Note: After anodd(DEVPARSEL =Ah)oreven (DEVPARSEL =5h)scheme isprogrammed
intotheDEVPARSEL register, anyonebitchange canbedetected andwillretain its
programmed scheme. More than onebitchanges inDEVPARSEL willcause adefault toodd
parity scheme.
5h The device parity iseven.
Ah The device parity isodd.
2.5.1.45 System Exception Control Register (SYSECR)
The SYSECR register, shown inFigure 2-52 anddescribed inTable 2-64,isused togenerate asoftware
reset.
NOTE: The register bitsinSYSECR aredesignated ashigh-integrity bitsandhave been
implemented with error-correcting logic such thateach bit,although read andwritten asa
single bit,isactually amulti-bit keywith error correction capability. Assuch, single-bit flips
within the"key"canbecorrected allowing protection ofthesystem asawhole. Anerror
detected issignaled totheESM module.
Figure 2-52. System Exception Control Register (SYSECR) (offset =E0h)
31 16
Reserved
R-0
15 14 13 0
RESET1 RESET0 Reserved
R/WP-0 R/WP-1 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-64. System Exception Control Register (SYSECR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-14 RESET[1-0] Software reset bits. Setting RESET1 orclearing RESET0 causes asystem software reset.
1h Noreset willoccur.
0,2h-3h Aglobal system reset willoccur.
13-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 198 -->

System andPeripheral Control Registers www.ti.com
198 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.46 System Exception Status Register (SYSESR)
The SYSESR register, shown inFigure 2-53 anddescribed inTable 2-65,shows thesource fordifferent
resets encountered. Previous reset source status bitsarenotautomatically cleared ifnew resets occur.
After reading thisregister, thesoftware should clear anyflags thataresetsothatthesource offuture
resets canbedetermined. Any bitinthisregister canbecleared bywriting a1tothebit.
Figure 2-53. System Exception Status Register (SYSESR) (offset =E4h)
31 16
Reserved
R-0
15 14 13 12 11 10 8
PORST OSCRST WDRST Reserved DBGRST Reserved
R/WC-X R/WC-X* R/WC-X* R-0 R/WC-X* R-0
7 6 5 4 3 2 0
ICSTRST Reserved CPURST SWRST EXTRST Reserved
R/WC-X* R/WC-X* R/WC-X* R/WC-X* R/WC-X* R-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; X=value isunchanged after reset; X*=0after PORST butunchanged after other
resets; -n=value after reset
Table 2-65. System Exception Status Register (SYSESR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 PORST Power-on reset. This bitissetwhen apower-on reset occurs, either internally asserted bytheVMON or
externally asserted bythenPORRST pin.
0 Nopower-on reset hasoccurred since thisbitwas lastcleared.
1 Areset was caused byapower-on reset. (This bitshould becleared after being read sothat
subsequent resets canbeproperly identified asnotbeing power-on resets.)
14 OSCRST Reset caused byanoscillator failure orPLL cycle slip. This bitissetwhen areset iscaused byan
oscillator failure orPLL slip. Write 1willclear thisbit.Write 0hasnoeffect.
Note: Theaction taken when anoscillator failure orPLL slipisdetected must configured inthe
PLLCTL1 register.
0 Noreset hasoccurred duetoanoscillator failure oraPLL cycle slip.
1 Areset was caused byanoscillator failure oraPLL cycle slip.
13 WDRST Watchdog reset flag. This bitissetwhen thelastreset was caused bythedigital watchdog (DWD).
Write 1willclear thisbit.Write 0hasnoeffect.
0 Noreset hasoccurred because oftheDWD.
1 Areset was caused bytheDWD.
12 Reserved 0 Reads return 0.Writes have noeffect.
11 DBGRST Debug reset flag. This bitissetwhen thelastreset was caused bythedebugger reset request. Write 1
willclear thisbit.Write 0hasnoeffect.
0 Noreset hasoccurred because ofthedebugger.
1 Areset was caused bythedebugger.
10-8 Reserved 0 Reads return 0.Writes have noeffect.
7 ICSTRST Interconnect reset flag. This bitissetwhen thelastCPU reset was caused bytheentering andexiting
ofinterconnect self-test check. While theinterconnect isunder self-test check, theCPU isalso held in
reset until theinterconnect self-test iscomplete.
0 NoCPUx reset hasoccurred because ofaninterconnect self-test check.
1 Areset hasoccurred totheCPUx because oftheinterconnect self-test check.
6 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 199 -->

www.ti.com System andPeripheral Control Registers
199 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-65. System Exception Status Register (SYSESR) Field Descriptions (continued)
Bit Field Value Description
5 CPURST CPU reset flag. This bitissetwhen theCPU isreset. Write 1willclear thisbit.Write 0hasnoeffect.
Note: ACPU reset canbeinitiated bytheCPU self-test controller (LBIST) orbytoggling theCPU
RESET bitfield inCPURSTCR register.
0 NoCPU reset hasoccurred.
1 ACPU reset occurred.
4 SWRST Software reset flag. This bitissetwhen asoftware system reset hasoccurred. Write 1willclear thisbit.
Write 0hasnoeffect.
Note: Asoftware system reset canbeinitiated bywriting totheRESET bitsintheSYSECR
register.
0 Nosoftware reset hasoccurred.
1 Asoftware reset occurred.
3 EXTRST External reset flag. This bitissetwhen areset iscaused bytheexternal reset pinnRST orbyanyreset
thatalso asserts thenRST pin(PORST, OSCRST, WDRST, WD2RST, andSWRST).
0 The external reset pinhasnotasserted areset.
1 Areset hasbeen caused bytheexternal reset pin.
2-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 200 -->

System andPeripheral Control Registers www.ti.com
200 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.47 System Test Abort Status Register (SYSTASR)
This register isshown inFigure 2-54 anddescribed inTable 2-66.
Figure 2-54. System Test Abort Status Register (SYSTASR) (offset =E8h)
31 16
Reserved
R-0
15 5 4 0
Reserved EFUSE_Abort
R-0 R/WPC-X/0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; C=Clear; -X=value isunchanged after reset; -n=
value after reset
Table 2-66. System Test Abort Status Register (SYSTASR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 EFUSE_Abort Test Abort status flag. These bitsaresetwhen testabort occurred:
0 Read: The lastoperation (ifany) completed successfully. This isalso thevalue thatthe
error/status register issettoafter reset.
1h Read: Controller times outbecause there isnolastrowsent from theFuseROM.
2h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine didnotfindenough FuseROM data
tofillthescan chain.
3h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine starts thescan chain with asignature
itexpects toseeafter thescan chain isfull.The autoload machine was able tofillthescan chain,
butthewrong signature was returned.
4h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine was notable ornotallowed to
complete itsoperation.
Others Read: Reserved.
1Fh Write: These bitsarecleared to0.

<!-- Page 201 -->

www.ti.com System andPeripheral Control Registers
201 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.48 Global Status Register (GLBSTAT)
The GLBSTAT register, shown inFigure 2-55 anddescribed inTable 2-67,indicates theFMzPLL (PLL1)
slipstatus andtheoscillator failstatus.
NOTE: PLL andOSC failbehavior
The device behavior after aPLL sliporanoscillator failure isconfigured inthePLLCTL1
register.
Figure 2-55. Global Status Register (GLBSTAT) (offset =ECh)
31 16
Reserved
R-0
15 10 9 8 7 1 0
Reserved FBSLIP RFSLIP Reserved OSCFAIL
R-0 R/W1C-n R/W1C-n R-0 R/W1C-n
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toClear; -n=value after reset
Table 2-67. Global Status Register (GLBSTAT) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9 FBSLIP PLL over cycle slipdetection. (cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: NoPLL over cycle sliphasbeen detected.
Write: The bitisunchanged.
1 Read: APLL over cycle sliphasbeen detected.
Write: The bitiscleared to0.
8 RFSLIP PLL under cycle slipdetection. (cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: NoPLL under cycle sliphasbeen detected.
Write: The bitisunchanged.
1 Read: APLL under cycle sliphasbeen detected.
Write: The bitiscleared to0.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 OSCFAIL Oscillator failflagbit.(cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: Nooscillator failure hasbeen detected.
Write: The bitisunchanged.
1 Read: Anoscillator failure hasbeen detected.
Write: The bitiscleared to0.

<!-- Page 202 -->

System andPeripheral Control Registers www.ti.com
202 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.49 Device Identification Register (DEVID)
The DEVID isaread-only register. Itcontains device-specific information thatishard-coded during device
manufacture. Fortheinitial silicon version, thedevice identification code value is8044 AD05h. This
register isshown inFigure 2-56 anddescribed inTable 2-68.
Figure 2-56. Device Identification Register (DEVID) (offset =F0h)
31 30 17 16
CP15 UNIQUE ID TECH
R-K R-K R-K
15 13 12 11 10 9 8
TECH I/OVOLTAGE PERIPHERAL
PARITYFLASH ECC RAM ECC
R-K R-K R-K R-K R-K
7 3 2 0
VERSION PLATFORM ID
R-K R-K
LEGEND: R=Read only; K=Constant value; -n=value after reset
Table 2-68. Device Identification Register (DEVID) Field Descriptions
Bit Field Value Description
31 CP15 CP15 CPU. This bitindicates whether theCPU hasacoprocessor 15(CP15).
0 The CPU hasnoCP15 present.
1 The CPU hasaCP15 present. The CPU IDcanberead using theCP15 C0,C0,0 register.
30-17 UNIQUE ID 0-3FFFh Device ID.The device IDisunique bydevice configuration.
16-13 TECH These bitsdefine theprocess technology bywhich thedevice was manufactured.
0 Device manufactured intheC05 process technology.
1h Device manufactured intheF05 process technology.
2h Device manufactured intheC035 process technology.
3h Device manufactured intheF035 process technology.
4h Device manufactured intheC021 process technology.
5h Device manufactured intheF021 process technology.
6h-7h Reserved
12 I/OVOLTAGE Input/output voltage. This bitdefines theI/Ovoltage ofthedevice.
0 The I/Ovoltage is3.3V.
1 The I/Ovoltage is5V.
11 PERIPHERAL
PARITYPeripheral parity. This bitindicates whether ornotperipheral memory parity ispresent.
0 The peripheral memories have noparity.
1 The peripheral memories have parity.
10-9 FLASH ECC These bitsindicate which parity ispresent fortheprogram memory.
0 Nomemory protection ispresent.
1h The program memory (Flash) hassingle-bit parity.
2h The program memory (Flash) hasECC.
3h This combination isreserved.
8 RAM ECC RAM ECC. This bitindicates whether ornotRAM memory ECC ispresent.
0 The RAM memories donothave ECC.
1 The RAM memories have ECC.
7-3 VERSION 0-1Fh Version. These bitsprovide therevision ofthedevice.
2-0 PLATFORM ID 5h The device ispart oftheTMS570Px family. The TMS570Px IDisalways 5h.

<!-- Page 203 -->

www.ti.com System andPeripheral Control Registers
203 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.50 Software Interrupt Vector Register (SSIVEC)
The SSIVEC register, shown inFigure 2-57 anddescribed inTable 2-69,contains information about
software interrupts.
Figure 2-57. Software Interrupt Vector Register (SSIVEC) (offset =F4h)
31 16
Reserved
R-0
15 8 7 0
SSIDATA SSIVECT
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 2-69. Software Interrupt Vector Register (SSIVEC) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSIDATA 0-FFh System software interrupt data key. These bitscontain thedata keyvalue ofthesource forthe
system software interrupt, which isindicated bythevector intheSSIVEC[7-0] field.
7-0 SSIVECT These bitscontain thesource forthesystem software interrupt.
Note: Aread from theSSIVECT bitsclears thecorresponding SSI_FLAG[4-1] bitinthe
SSIF register, corresponding tothesource vector ofthesystem software interrupt.
Note: TheSSIR[4-1] interrupt hasthefollowing priority order:
SSIR1 hasthehighest priority.
SSIR4 hasthelowest priority.
0 Nosoftware interrupt ispending.
1h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR1 register.
2h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR2 register.
3h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR3 register.
4h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR4 register.
5h-FFh Reserved

<!-- Page 204 -->

System andPeripheral Control Registers www.ti.com
204 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.51 System Software Interrupt Flag Register (SSIF)
The SSIF register, shown inFigure 2-58 anddescribed inTable 2-70,contains software interrupt flag
status information.
Figure 2-58. System Software Interrupt Flag Register (SSIF) (offset =F8h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved SSI_FLAG4 SSI_FLAG3 SSI_FLAG2 SSI_FLAG1
R-0 R/WC-0 R/WC-0 R/WC-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 2-70. System Software Interrupt Flag Register (SSIF) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 SSI_FLAG[4-1] System software interrupt flag[4-1]. This flagissetwhen thecorrect SSKEY iswritten tothe
SSIR register[4-1].
Note: Aread from theSSIVEC register clears thecorresponding SSI_FLAG[4-1] bitinthe
SSIF, corresponding tothesource vector ofthesystem software interrupt.
0 Read: NoIRQ/FIQ interrupt was generated since thebitwas lastcleared.
Write: The bitisunchanged.
1 Read: AnIRQ/FIQ interrupt was generated.
Write: The bitiscleared to0.

<!-- Page 205 -->

www.ti.com System andPeripheral Control Registers
205 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2 Secondary System Control Registers (SYS2)
This section describes thesecondary frame ofsystem registers. The start address ofthesecondary
system module frame isFFFF E100h. The registers support 8-,16-, and32-bit writes. The offset isrelative
tothesystem module frame start address.
Table 2-71 contains alistofthesecondary system control registers.
NOTE: Alladditional registers inthesecondary system frame arereserved.
Table 2-71. Secondary System Control Registers
Offset Acronym Register Description Section
00h PLLCTL3 PLL Control Register 3 Section 2.5.2.1
08h STCLKDIV CPU Logic BIST Clock Divider Section 2.5.2.2
24h ECPCNTL ECP Control Register. The ECPCNTL register hasthemirrored
location atthisaddress.Section 2.5.1.43
28h ECPCNTL1 ECP Control Register 1. Section 2.5.2.3
3Ch CLK2CNTRL Clock 2Control Register Section 2.5.2.4
40h VCLKACON1 Peripheral Asynchronous Clock Configuration 1Register Section 2.5.2.5
54h HCLKCNTL HCLK Control Register Section 2.5.2.6
70h CLKSLIP Clock SlipControl Register Section 2.5.2.7
78h IP1ECCERREN IPECC Error Enable Register Section 2.5.2.8
ECh EFC_CTLREG EFUSE Controller Control Register Section 2.5.2.9
F0h DIEIDL_REG0 DieIdentification Register Lower Word Section 2.5.2.10
F4h DIEIDH_REG1 DieIdentification Register Upper Word Section 2.5.2.11
F8h DIEIDL_REG2 DieIdentification Register Lower Word Section 2.5.2.12
FCh DIEIDH_REG3 DieIdentification Register Upper Word Section 2.5.2.13

<!-- Page 206 -->

System andPeripheral Control Registers www.ti.com
206 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.1 PLL Control Register 3(PLLCTL3)
The PLLCTL3 register isshown inFigure 2-59 anddescribed inTable 2-72;controls thesettings ofPLL2
(Clock Source 6-FPLL).
Figure 2-59. PLL Control Register 3(PLLCTL3) (offset =00h)
31 29 28 24 23 22 21 16
ODPLL2 PLLDIV2 Reserved REFCLKDIV2
R/WP-0 R/WP-4h R-0 R/WP-0
15 0
PLLMUL2
R/WP-1300h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-72. PLL Control Register 3(PLLCTL3) Field Descriptions
Bit Field Value Description
31-29 ODPLL2 Internal PLL Output Divider
OD2 =ODPLL2 +1,ranges from 1to8.
fpost_ODCLK2 =foutput_CLK2 /OD2
Note: PLL output clock isgated offifODPLL2 ischanged while thePLL#2 isactive.
0 fpost_ODCLK2 =foutput_CLK2 /1
1h fpost_ODCLK2 =foutput_CLK2 /2
: :
7h fpost_ODCLK2 =foutput_CLK2 /8
28-24 PLLDIV2 PLL2 Output Clock Divider
R2=PLLDIV2 +1,ranges from 1to32.
fPLL2 CLK=fpost_ODCLK2 /R2
0 fPLL2 CLK=fpost_ODCLK2 /1
1h fPLL2 CLK=fpost_ODCLK2 /2
: :
1Fh fPLL2 CLK=fpost_ODCLK2 /32
23-22 Reserved 0 Value hasnoeffect onPLL operation.
21-16 REFCLKDIV2 Reference Clock Divider
NR2 =REFCLKDIV2 +1,ranges from 1to64.
fINTCLK2 =fOSCIN /NR2
Note: This value should notbechanged while thePLL2 isactive.
0 fINTCLK2 =fOSCIN /1
1h fINTCLK2 =fOSCIN /2
: :
3Fh fINTCLK2 =fOSCIN /64
15-0 PLLMUL2 PLL2 Multiplication Factor
NF2 =(PLLMUL2 /256) +1,valid multiplication factors arefrom 1to256.
fVCOCLK2 =fINTCLK2 xNF2
User andprivileged mode (read):
Privileged mode (write):
100h fVCOCLK2 =fINTCLK2 x1
: :
5B00h fVCOCLK2 =fINTCLK2 x92
5C00h fVCOCLK2 =fINTCLK2 x93
: :
FF00h fVCOCLK2 =fINTCLK2 x256

<!-- Page 207 -->

www.ti.com System andPeripheral Control Registers
207 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.2 CPU Logic Bist Clock Divider (STCLKDIV)
This register isshown inFigure 2-60 anddescribed inTable 2-73.
Figure 2-60. CPU Logic BIST Clock Prescaler (STCLKDIV) (offset =08h)
31 27 26 24 23 16
Reserved CLKDIV Reserved
R-0 R/WP-0 R-0
15 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-73. CPU Logic BIST Clock Prescaler (STCLKDIV) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CLKDIV 0 Clock divider/prescaler forCPU clock during logic BIST
23-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 208 -->

/c40 /c41 1 E C P D IVN V C L K o rO S C IE C L K/c43/c61
System andPeripheral Control Registers www.ti.com
208 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.3 ECP Control Register 1(ECPCNTL1)
The ECP register, shown inFigure 2-61 anddescribed inTable 2-74,configures theECLK2 pinin
functional mode.
NOTE: ECLK2 Functional mode configuration.
The ECLK2 pinmust beplaced intoFunctional mode bysetting theECPCLKFUN bitto1in
theSYSPC1 register before aclock source willbevisible ontheECLK pin.
Figure 2-61. ECP Control Register 1(ECPCNTL1) (offset =28h)
31 28 27 25 24 23 22 16
ECP_KEY Reserved ECPSSEL ECPCOS Reserved
R/WP-5h R-0 R/W-0 R/W-0 R-0
15 0
ECPDIV
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-74. ECP Control Register 1(ECPCNTL1) Field Descriptions
Bit Field Value Description
31-28 ECP_KEY Enable ECP clock logic forECLK2.
Ah Clock functionality ofECP clock isenabled.
Others Clock functionality ofECP clock isdisabled.
27-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ECPSSEL This bitallows theselection between VCLK andOSCIN astheclock source forECLK2.
0 VCLK isselected astheECP clock source.
1 OSCIN isselected astheECP clock source.
23 ECPCOS ECP continue onsuspend.
Note: Suspend mode isentered while performing certain JTAG debugging operations.
0 ECLK output isdisabled insuspend mode. ECLK output willbeshut offandwillnotbeseen on
theI/Opinofthedevice.
1 ECLK output isnotdisabled insuspend mode. ECLK output willnotbeshut offandwillbeseen
ontheI/Opinofthedevice.
22-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 ECPDIV 0-FFFFh ECP divider value. The value ofECPDIV bitsdetermine theexternal clock (ECP clock) frequency
asaratio ofVBUS clock orOSCIN asshown intheformula:

<!-- Page 209 -->

www.ti.com System andPeripheral Control Registers
209 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.4 Clock 2Control Register (CLK2CNTRL)
This register isshown inFigure 2-62 anddescribed inTable 2-75.
Figure 2-62. Clock 2Control Register (CLK2CNTRL) (offset =3Ch)
31 16
Reserved
R-0
15 12 11 8 7 4 3 0
Reserved Reserved Reserved VCLK3R
R-0 R/WP-1h R-0 R/WP-1h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-75. Clock 2Control Register (CLK2CNTRL) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 Reserved Reads return value andwrites allowed inprivilege mode.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 VCLK3R VBUS clock3 ratio.
0 The ratio isHCLK divide by1.
: :
Fh The ratio isHCLK divided by16.

<!-- Page 210 -->

System andPeripheral Control Registers www.ti.com
210 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.5 Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1)
This register isshown inFigure 2-63 anddescribed inTable 2-76.
Figure 2-63. Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1) [offset =40h]
31 27 26 24
Reserved VCLKA4R
R-0 R/WP-1h
23 21 20 19 16
Reserved VCLKA4_DIV_
CDDISVCLKA4S
R-0 R/WP-0 R/WP-9h
15 11 10 8 7 5 4 0
Reserved Reserved Reserved Reserved
R-0 R/WP-1h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-76. Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1)
Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 VCLKA4R Clock divider fortheVCLKA4 source. Output willbepresent onVCLKA4_DIVR.
VCLKA4 domain willbeenabled bywriting totheCDDIS register andVCLKA4_DIV_CDDIS bit.
Itcaninferred thatVCLKA4_DIV clock isdisabled when VCLKA4 clock isdisabled.
0 The ratio isVCLKA4 divided by1.
: :
7h The ratio isVCLKA4 divided by8.
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 VCLKA4_DIV_CDDIS Disable theVCLKA4 divider output.
VCLKA4 domain willbeenabled bywriting totheCDDIS register.
0 Enable theprescaled VCLKA4 clock onVCLKA4_DIVR.
1 Disable theprescaled VCLKA4 clock onVCLKA4_DIVR.
19-16 VCLKA4S Peripheral asynchronous clock4 source.
0 Clock source0 isthesource forperipheral asynchronous clock4.
1h Clock source1 isthesource forperipheral asynchronous clock4.
2h Clock source2 isthesource forperipheral asynchronous clock4.
3h Clock source3 isthesource forperipheral asynchronous clock4.
4h Clock source4 isthesource forperipheral asynchronous clock4.
5h Clock source5 isthesource forperipheral asynchronous clock4.
6h Clock source6 isthesource forperipheral asynchronous clock4.
7h Clock source7 isthesource forperipheral asynchronous clock4.
8h-Fh VCLK oradivided VCLK isthesource forperipheral asynchronous clock4. See thedevice-
specific data manual fordetails.
15-0 Reserved 109h Reserved
NOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown intheTable 2-29.

<!-- Page 211 -->

www.ti.com System andPeripheral Control Registers
211 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.6 HCLK Control Register (HCLKCNTL)
This register isshown inFigure 2-64 anddescribed inTable 2-77.
Figure 2-64. HCLK Control Register (HCLKCNTL) (offset =54h)
31 16
Reserved
R-0
15 2 1 0
Reserved HCLKR
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-77. HCLK Control Register (HCLKCNTL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 HCLKR HCLK divider value. The value ofHCLKR bitsdetermine theHCLK frequency asaratio ofGCLK1.
0 HCLK isequal toGCLK1 divide by1.
1h HCLK isequal toGCLK1 divide by2.
2h HCLK isequal toGCLK1 divide by3.
3h HCLK isequal toGCLK1 divide by4.

<!-- Page 212 -->

System andPeripheral Control Registers www.ti.com
212 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.7 Clock SlipControl Register (CLKSLIP)
This register isshown inFigure 2-65 anddescribed inTable 2-78.Forinformation onfiltering theFBSLIP,
seeSection 2.5.1.34 .
Figure 2-65. Clock SlipControl Register (CLKSLIP) (offset =70h)
31 16
Reserved
R-0
15 14 13 8 7 4 3 0
Reserved PLL1_RFSLIP_FILTER_COUNT Reserved PLL1_RFSLIP_FILTER_KEY
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-78. Clock SlipControl Register (CLKSLIP) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 PLL1_RFSLIP_FILTER_COUNT PLL RFSLIP down counter programmed value. Count ison10M clock.
Onreset, counter value is0.Counter must beprogrammed toanon-zero value
andenabled forthefiltering tobeenabled.
0 Filtering isdisabled.
1h Filtering isenabled. Every slipisrecognized.
2h Filtering isenabled. The slipmust beatleast 2HFLPO cycles wide inorder to
berecognized asaslip.
: :
3Fh Filtering isenabled. The RFSLIP must beatleast 63HFLPO cycles wide in
order toberecognized asaslip.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 PLL1_RFSLIP_FILTER_KEY Enable thePLL RFSLIP filtering.
5h Onreset, thePLL RFSLIP filter isdisabled andthePLL RFSLIP passes through.
Fh This isanunsupported value. You should avoid writing thisvalue tothisbitfield.
Others PLL RFSLIP filtering isenabled. Recommended toprogram Ahinthisbitfield.
Enabling ofthePLL RFSLIP occurs when theKEY isprogrammed andanon-
zero value ispresent intheCOUNT field.

<!-- Page 213 -->

www.ti.com System andPeripheral Control Registers
213 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.8 IPECC Error Enable Register (IP1ECCERREN)
This register isshown inFigure 2-66 anddescribed inTable 2-79.
Figure 2-66. IPECC Error Enable Register (IP1ECCERREN) (offset =78h)
31 28 27 24 23 20 19 16
Reserved Reserved Reserved Reserved
R-0 R/WP-5h R/WP-5h
15 12 11 8 7 4 3 0
Reserved IP2_ECC_KEY Reserved IP1_ECC_KEY
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-79. Clock SlipRegister (CLKSLIP) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 Reserved 0-Fh Reads return 0or1anddepends onwhat iswritten inprivileged mode. The
functionality ofthisbitisunavailable inthisdevice.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 Reserved 0-Fh Reads return 0or1anddepends onwhat iswritten inprivileged mode. The
functionality ofthisbitisunavailable inthisdevice.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 IP2_ECC_KEY ECC Error Enable Key forPS_SCR_M master. There isanECC Evaluation block
inside theCPU Interconnect Subsystem responsible forECC correction anddetection
onthedata path fortransactions initiated bythePS_SCR_M master. IfanECC error
(either single-bit ordouble-bit error) isdetected, then thecorresponding error signal is
asserted ifECC enable keywritten toIP2_ECC_KEY isAh.
Others Disable ECC error generation forECC errors detected onPS_SCR_M master bythe
CPU Interconnect Subsystem.
Ah Enable ECC error generation forECC errors detected onPS_SCR_M master bythe
CPU Interconnect Subsystem.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 IP1_ECC_KEY ECC Error Enable Key forDMA Port Amaster. There isanECC Evaluation block
inside theCPU Interconnect Subsystem responsible forECC correction anddetection
onthedata path fortransactions initiated bytheDMA Port Amaster. IfanECC error
(either single-bit ordouble-bit error) isdetected, then thecorresponding error signal is
asserted ifECC enable keywritten toIP1_ECC_KEY isAh.
Others Disable ECC error generation forECC errors detected onDMA Port Amaster bythe
CPU Interconnect Subsystem.
Ah Enable ECC error generation forECC errors detected onDMA Port Amaster bythe
CPU Interconnect Subsystem.

<!-- Page 214 -->

System andPeripheral Control Registers www.ti.com
214 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.9 EFUSE Controller Control Register (EFC_CTLREG)
This register isshown inFigure 2-67 anddescribed inTable 2-80.
Figure 2-67. EFUSE Controller Control Register (EFC_CTLREG) (offset =ECh)
31 16
Reserved
R-0
15 4 3 0
Reserved EFC_INSTR_WEN
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-80. EFUSE Controller Control Register (EFC_CTLREG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EFC_INSTR_WEN Enable user write of4EFUSE controller instructions.
SYS module generates theenable signal thatwillbetiedtoOCP_FROM_WRITE_DISABLE
onefuse controller port.
Ah Writing ofinstructions (Program, ProgramCRA, RunAutoload, andLoadFuseScanchain) to
EFC isallowed.
Others Writing ofinstructions (Program, ProgramCRA, RunAutoload, andLoadFuseScanchain) in
EFC registers isblocked.
2.5.2.10 DieIdentification Register Lower Word (DIEIDL_REG0)
The DIEIDL_REG0 register isaduplicate oftheDIEIDL register, seeSection 2.5.1.28 .The DIEIDL_REG0
register, shown inFigure 2-68 anddescribed inTable 2-81,contains information about thediewafer
number, andX,Ywafer coordinates.
Figure 2-68. DieIdentification Register, Lower Word (DIEIDL_REG0) [offset =F0h]
31 24 23 16
WAFER # YWAFER COORDINATE
R-D R-D
15 12 11 0
YWAFER COORDINATE XWAFER COORDINATE
R-D R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-81. DieIdentification Register, Lower Word (DIEIDL_REG0) Field Descriptions
Bit Field Description
31-24 WAFER # These read-only bitscontain thewafer number ofthedevice.
23-12 YWAFER COORDINATE These read-only bitscontain theYwafer coordinate ofthedevice.
11-0 XWAFER COORDINATE These read-only bitscontain theXwafer coordinate ofthedevice.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.

<!-- Page 215 -->

www.ti.com System andPeripheral Control Registers
215 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.11 DieIdentification Register Upper Word (DIEIDH_REG1)
The DIEIDH_REG1 register isaduplicate oftheDIEIDH register, seeSection 2.5.1.29 .The
DIEIDH_REG1 register, shown inFigure 2-69 anddescribed inTable 2-82,contains information about the
dielotnumber.
Figure 2-69. DieIdentification Register, Upper Word (DIEIDH_REG1) [offset =F4h]
31 24 23 16
Reserved LOT #
R-0 R-D
15 0
LOT #
R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-82. DieIdentification Register, Upper Word (DIEIDH_REG1) Field Descriptions
Bit Field Description
31-24 Reserved Reserved forTIuse. Writes have noeffect.
23-0 LOT # This read-only register contains thedevice lotnumber.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.
2.5.2.12 DieIdentification Register Lower Word (DIEIDL_REG2)
This register isshown inFigure 2-70 anddescribed inTable 2-83.
Figure 2-70. DieIdentification Register, Lower Word (DIEIDL_REG2) [offset =F8h]
31 0
DIEIDL2
R-X
LEGEND: R=Read only; X=value isunchanged after reset; -n=value after reset
Table 2-83. DieIdentification Register, Lower Word (DIEIDL_REG2) Field Descriptions
Bit Field Value Description
31-0 DIEIDL2(95-64) 0-FFFF FFFFh This read-only register contains thelower word (95:64) ofthedieIDinformation. The
contents ofthisregister isreserved.

<!-- Page 216 -->

System andPeripheral Control Registers www.ti.com
216 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.13 DieIdentification Register Upper Word (DIEIDH_REG3)
This register isshown inFigure 2-71 anddescribed inTable 2-84.
Figure 2-71. DieIdentification Register, Upper Word (DIEIDH_REG3) [offset =FCh]
31 0
DIEIDH2
R-X
LEGEND: R=Read only; X=value isunchanged after reset ;-n=value after reset
Table 2-84. DieIdentification Register, Upper Word (DIEIDH_REG3) Field Descriptions
Bit Field Value Description
31-0 DIEIDH2(127-96) 0-FFFF FFFFh This read-only register contains theupper word (127:97) ofthedieIDinformation. The
contents ofthisregister isreserved.

<!-- Page 217 -->

www.ti.com System andPeripheral Control Registers
217 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3 Peripheral Central Resource (PCR) Control Registers
This section describes thePeripheral Central Resource (PCR) control registers. The arethree PCRx in
thismicrocontroller. The start address ofthePCR1 register frame isFFFF 1000h. The start address ofthe
PCR2 register frame isFCFF 1000h. The start address ofthePCR3 register frame isFFF7 8000h.
Table 2-85 lists theregisters inthePCR, which areused toconfigure thefollowing main functionalities:
*Protection control totheperipherals inPCS (Peripheral Memory) andPS(Peripheral) regions.
*Powerdown control totheperipherals inPCS (Peripheral Memory) andPS(Peripheral) regions.
*Powerdown control totheCoreSight debug peripherals indebug frame region from FFA0 0000h to
FFAF FFFFh.
*Master-ID Filtering control totheperipherals inPS(Peripheral), PPS (Privileged Peripheral) ,PPSE
(Privileged Peripheral Extended) regions.
*Master-ID Filtering control totheperipheral memories inPCS (Peripheral Memory), andPPCS
(Privileged Peripheral Memory) regions.
The following sections provide detailed information about these registers. Notallchip selects exist onthis
device.
Table 2-85. Peripheral Central Resource Control Registers
Offset Acronym Register Description Section
00h PMPROTSET0 Peripheral Memory Protection SetRegister 0 Section 2.5.3.1
04h PMPROTSET1 Peripheral Memory Protection SetRegister 1 Section 2.5.3.2
10h PMPROTCLR0 Peripheral Memory Protection Clear Register 0 Section 2.5.3.3
14h PMPROTCLR1 Peripheral Memory Protection Clear Register 1 Section 2.5.3.4
20h PPROTSET0 Peripheral Protection SetRegister 0 Section 2.5.3.5
24h PPROTSET1 Peripheral Protection SetRegister 1 Section 2.5.3.6
28h PPROTSET2 Peripheral Protection SetRegister 2 Section 2.5.3.7
2Ch PPROTSET3 Peripheral Protection SetRegister 3 Section 2.5.3.8
40h PPROTCLR0 Peripheral Protection Clear Register 0 Section 2.5.3.9
44h PPROTCLR1 Peripheral Protection Clear Register 1 Section 2.5.3.10
48h PPROTCLR2 Peripheral Protection Clear Register 2 Section 2.5.3.11
4Ch PPROTCLR3 Peripheral Protection Clear Register 3 Section 2.5.3.12
60h PCSPWRDWNSET0 Peripheral Memory Power-Down SetRegister 0 Section 2.5.3.13
64h PCSPWRDWNSET1 Peripheral Memory Power-Down SetRegister 1 Section 2.5.3.14
70h PCSPWRDWNCLR0 Peripheral Memory Power-Down Clear Register 0 Section 2.5.3.15
74h PCSPWRDWNCLR1 Peripheral Memory Power-Down Clear Register 1 Section 2.5.3.16
80h PSPWRDWNSET0 Peripheral Power-Down SetRegister 0 Section 2.5.3.17
84h PSPWRDWNSET1 Peripheral Power-Down SetRegister 1 Section 2.5.3.18
88h PSPWRDWNSET2 Peripheral Power-Down SetRegister 2 Section 2.5.3.19
8Ch PSPWRDWNSET3 Peripheral Power-Down SetRegister 3 Section 2.5.3.20
A0h PSPWRDWNCLR0 Peripheral Power-Down Clear Register 0 Section 2.5.3.21
A4h PSPWRDWNCLR1 Peripheral Power-Down Clear Register 1 Section 2.5.3.22
A8h PSPWRDWNCLR2 Peripheral Power-Down Clear Register 2 Section 2.5.3.23
ACh PSPWRDWNCLR3 Peripheral Power-Down Clear Register 3 Section 2.5.3.24
C0h PDPWRDWNSET Debug Frame Power-Down SetRegister Section 2.5.3.25
C4h PDPWRDWNCLR Debug Frame Power-Down Clear Register Section 2.5.3.26
200h MSTIDWRENA MasterID Protection Write Enable Register Section 2.5.3.27
204h MSTIDENA MasterID Protection Enable Register Section 2.5.3.28
208h MSTIDDIAGCTRL MasterID Diagnostic Control Register Section 2.5.3.29
300h PS0MSTID_L Peripheral Frame 0Master-ID Protection Register_L Section 2.5.3.30
304h PS0MSTID_H Peripheral Frame 0Master-ID Protection Register_H Section 2.5.3.31
308h PS1MSTID_L Peripheral Frame 1Master-ID Protection Register_L Section 2.5.3.32

<!-- Page 218 -->

System andPeripheral Control Registers www.ti.com
218 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
30Ch PS1MSTID_H Peripheral Frame 1Master-ID Protection Register_H Section 2.5.3.32
310h PS2MSTID_L Peripheral Frame 2Master-ID Protection Register_L Section 2.5.3.32
314h PS2MSTID_H Peripheral Frame 2Master-ID Protection Register_H Section 2.5.3.32
318h PS3MSTID_L Peripheral Frame 3Master-ID Protection Register_L Section 2.5.3.32
31Ch PS3MSTID_H Peripheral Frame 3Master-ID Protection Register_H Section 2.5.3.32
320h PS4MSTID_L Peripheral Frame 4Master-ID Protection Register_L Section 2.5.3.32
324h PS4MSTID_H Peripheral Frame 4Master-ID Protection Register_H Section 2.5.3.32
328h PS5MSTID_L Peripheral Frame 5Master-ID Protection Register_L Section 2.5.3.32
32Ch PS5MSTID_H Peripheral Frame 5Master-ID Protection Register_H Section 2.5.3.32
330h PS6MSTID_L Peripheral Frame 6Master-ID Protection Register_L Section 2.5.3.32
334h PS6MSTID_H Peripheral Frame 6Master-ID Protection Register_H Section 2.5.3.32
338h PS7MSTID_L Peripheral Frame 7Master-ID Protection Register_L Section 2.5.3.32
33Ch PS7MSTID_H Peripheral Frame 7Master-ID Protection Register_H Section 2.5.3.32
340h PS8MSTID_L Peripheral Frame 8Master-ID Protection Register_L Section 2.5.3.32
344h PS8MSTID_H Peripheral Frame 8Master-ID Protection Register_H Section 2.5.3.32
348h PS9MSTID_L Peripheral Frame 9Master-ID Protection Register_L Section 2.5.3.32
34Ch PS9MSTID_H Peripheral Frame 9Master-ID Protection Register_H Section 2.5.3.32
350h PS10MSTID_L Peripheral Frame 10Master-ID Protection Register_L Section 2.5.3.32
354h PS10MSTID_H Peripheral Frame 10Master-ID Protection Register_H Section 2.5.3.32
358h PS11MSTID_L Peripheral Frame 11Master-ID Protection Register_L Section 2.5.3.32
35Ch PS11MSTID_H Peripheral Frame 11Master-ID Protection Register_H Section 2.5.3.32
360h PS12MSTID_L Peripheral Frame 12Master-ID Protection Register_L Section 2.5.3.32
364h PS12MSTID_H Peripheral Frame 12Master-ID Protection Register_H Section 2.5.3.32
368h PS13MSTID_L Peripheral Frame 13Master-ID Protection Register_L Section 2.5.3.32
36Ch PS13MSTID_H Peripheral Frame 13Master-ID Protection Register_H Section 2.5.3.32
370h PS14MSTID_L Peripheral Frame 14Master-ID Protection Register_L Section 2.5.3.32
374h PS14MSTID_H Peripheral Frame 14Master-ID Protection Register_H Section 2.5.3.32
378h PS15MSTID_L Peripheral Frame 15Master-ID Protection Register_L Section 2.5.3.32
37Ch PS15MSTID_H Peripheral Frame 15Master-ID Protection Register_H Section 2.5.3.32
380h PS16MSTID_L Peripheral Frame 16Master-ID Protection Register_L Section 2.5.3.32
384h PS16MSTID_H Peripheral Frame 16Master-ID Protection Register_H Section 2.5.3.32
388h PS17MSTID_L Peripheral Frame 17Master-ID Protection Register_L Section 2.5.3.32
38Ch PS17MSTID_H Peripheral Frame 17Master-ID Protection Register_H Section 2.5.3.32
390h PS18MSTID_L Peripheral Frame 18Master-ID Protection Register_L Section 2.5.3.32
394h PS18MSTID_H Peripheral Frame 18Master-ID Protection Register_H Section 2.5.3.32
398h PS19MSTID_L Peripheral Frame 19Master-ID Protection Register_L Section 2.5.3.32
39Ch PS19MSTID_H Peripheral Frame 19Master-ID Protection Register_H Section 2.5.3.32
3A0h PS20MSTID_L Peripheral Frame 20Master-ID Protection Register_L Section 2.5.3.32
3A4h PS20MSTID_H Peripheral Frame 20Master-ID Protection Register_H Section 2.5.3.32
3A8h PS21MSTID_L Peripheral Frame 21Master-ID Protection Register_L Section 2.5.3.32
3ACh PS21MSTID_H Peripheral Frame 21Master-ID Protection Register_H Section 2.5.3.32
3B0h PS22MSTID_L Peripheral Frame 22Master-ID Protection Register_L Section 2.5.3.32
3B4h PS22MSTID_H Peripheral Frame 22Master-ID Protection Register_H Section 2.5.3.32
3B8h PS23MSTID_L Peripheral Frame 23Master-ID Protection Register_L Section 2.5.3.32
3BCh PS23MSTID_H Peripheral Frame 23Master-ID Protection Register_H Section 2.5.3.32
3C0h PS24MSTID_L Peripheral Frame 24Master-ID Protection Register_L Section 2.5.3.32
3C4h PS24MSTID_H Peripheral Frame 24Master-ID Protection Register_H Section 2.5.3.32

<!-- Page 219 -->

www.ti.com System andPeripheral Control Registers
219 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
3C8h PS25MSTID_L Peripheral Frame 25Master-ID Protection Register_L Section 2.5.3.32
3CCh PS25MSTID_H Peripheral Frame 25Master-ID Protection Register_H Section 2.5.3.32
3D0h PS26MSTID_L Peripheral Frame 26Master-ID Protection Register_L Section 2.5.3.32
3D4h PS26MSTID_H Peripheral Frame 26Master-ID Protection Register_H Section 2.5.3.32
3D8h PS27MSTID_L Peripheral Frame 27Master-ID Protection Register_L Section 2.5.3.32
3DCh PS27MSTID_H Peripheral Frame 27Master-ID Protection Register_H Section 2.5.3.32
3E0h PS28MSTID_L Peripheral Frame 28Master-ID Protection Register_L Section 2.5.3.32
3E4h PS28MSTID_H Peripheral Frame 28Master-ID Protection Register_H Section 2.5.3.32
3E8h PS29MSTID_L Peripheral Frame 29Master-ID Protection Register_L Section 2.5.3.32
3ECh PS29MSTID_H Peripheral Frame 29Master-ID Protection Register_H Section 2.5.3.32
3E0h PS30MSTID_L Peripheral Frame 30Master-ID Protection Register_L Section 2.5.3.32
3F4h PS30MSTID_H Peripheral Frame 30Master-ID Protection Register_H Section 2.5.3.32
3F8h PS31MSTID_L Peripheral Frame 31Master-ID Protection Register_L Section 2.5.3.32
3FCh PS31MSTID_H Peripheral Frame 31Master-ID Protection Register_H Section 2.5.3.32
400h PPS0MSTID_L Privileged Peripheral Frame 0Master-ID Protection Register_L Section 2.5.3.33
404h PPS0MSTID_H Privileged Peripheral Frame 0Master-ID Protection Register_H Section 2.5.3.34
408h PPS1MSTID_L Privileged Peripheral Frame 1Master-ID Protection Register_L Section 2.5.3.35
40Ch PPS1MSTID_H Privileged Peripheral Frame 1Master-ID Protection Register_H Section 2.5.3.35
410h PPS2MSTID_L Privileged Peripheral Frame 2Master-ID Protection Register_L Section 2.5.3.35
414h PPS2MSTID_H Privileged Peripheral Frame 2Master-ID Protection Register_H Section 2.5.3.35
418h PPS3MSTID_L Privileged Peripheral Frame 3Master-ID Protection Register_L Section 2.5.3.35
41Ch PPS3MSTID_H Privileged Peripheral Frame 3Master-ID Protection Register_H Section 2.5.3.35
420h PPS4MSTID_L Privileged Peripheral Frame 4Master-ID Protection Register_L Section 2.5.3.35
424h PPS4MSTID_H Privileged Peripheral Frame 4Master-ID Protection Register_H Section 2.5.3.35
428h PPS5MSTID_L Privileged Peripheral Frame 5Master-ID Protection Register_L Section 2.5.3.35
42Ch PPS5MSTID_H Privileged Peripheral Frame 5Master-ID Protection Register_H Section 2.5.3.35
430h PPS6MSTID_L Privileged Peripheral Frame 6Master-ID Protection Register_L Section 2.5.3.35
434h PPS6MSTID_H Privileged Peripheral Frame 6Master-ID Protection Register_H Section 2.5.3.35
438h PPS7MSTID_L Privileged Peripheral Frame 7Master-ID Protection Register_L Section 2.5.3.35
43Ch PPS7MSTID_H Privileged Peripheral Frame 7Master-ID Protection Register_H Section 2.5.3.35
440h PPSE0MSTID_L Privilege Peripheral Extended Frame 0Master-ID Protection
Register_LSection 2.5.3.36
444h PPSE0MSTID_H Privilege Peripheral Extended Frame 0Master-ID Protection
Register_HSection 2.5.3.37
448h PPSE1MSTID_L Privilege Peripheral Extended Frame 1Master-ID Protection
Register_LSection 2.5.3.38
44Ch PPSE1MSTID_H Privilege Peripheral Extended Frame 1Master-ID Protection
Register_HSection 2.5.3.38
450h PPSE2MSTID_L Privilege Peripheral Extended Frame 2Master-ID Protection
Register_LSection 2.5.3.38
454h PPSE2MSTID_H Privilege Peripheral Extended Frame 2Master-ID Protection
Register_HSection 2.5.3.38
458h PPSE3MSTID_L Privilege Peripheral Extended Frame 3Master-ID Protection
Register_LSection 2.5.3.38
45Ch PPSE3MSTID_H Privilege Peripheral Extended Frame 3Master-ID Protection
Register_HSection 2.5.3.38
460h PPSE4MSTID_L Privilege Peripheral Extended Frame 4Master-ID Protection
Register_LSection 2.5.3.38
464h PPSE4MSTID_H Privilege Peripheral Extended Frame 4Master-ID Protection
Register_HSection 2.5.3.38

<!-- Page 220 -->

System andPeripheral Control Registers www.ti.com
220 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
468h PPSE5MSTID_L Privilege Peripheral Extended Frame 5Master-ID Protection
Register_LSection 2.5.3.38
46Ch PPSE5MSTID_H Privilege Peripheral Extended Frame 5Master-ID Protection
Register_HSection 2.5.3.38
470h PPSE6MSTID_L Privilege Peripheral Extended Frame 6Master-ID Protection
Register_LSection 2.5.3.38
474h PPSE6MSTID_H Privilege Peripheral Extended Frame 6Master-ID Protection
Register_HSection 2.5.3.38
478h PPSE7MSTID_L Privilege Peripheral Extended Frame 7Master-ID Protection
Register_LSection 2.5.3.38
47Ch PPSE7MSTID_H Privilege Peripheral Extended Frame 7Master-ID Protection
Register_HSection 2.5.3.38
480h PPSE8MSTID_L Privilege Peripheral Extended Frame 8Master-ID Protection
Register_LSection 2.5.3.38
484h PPSE8MSTID_H Privilege Peripheral Extended Frame 8Master-ID Protection
Register_HSection 2.5.3.38
488h PPSE9MSTID_L Privilege Peripheral Extended Frame 9Master-ID Protection
Register_LSection 2.5.3.38
48Ch PPSE9MSTID_H Privilege Peripheral Extended Frame 9Master-ID Protection
Register_HSection 2.5.3.38
490h PPSE10MSTID_L Privilege Peripheral Extended Frame 10Master-ID Protection
Register_LSection 2.5.3.38
494h PPSE10MSTID_H Privilege Peripheral Extended Frame 10Master-ID Protection
Register_HSection 2.5.3.38
498h PPSE11MSTID_L Privilege Peripheral Extended Frame 11Master-ID Protection
Register_LSection 2.5.3.38
49Ch PPSE11MSTID_H Privilege Peripheral Extended Frame 11Master-ID Protection
Register_HSection 2.5.3.38
4A0h PPSE12MSTID_L Privilege Peripheral Extended Frame 12Master-ID Protection
Register_LSection 2.5.3.38
4A4h PPSE12MSTID_H Privilege Peripheral Extended Frame 12Master-ID Protection
Register_HSection 2.5.3.38
4A8h PPSE13MSTID_L Privilege Peripheral Extended Frame 13Master-ID Protection
Register_LSection 2.5.3.38
4ACh PPSE13MSTID_H Privilege Peripheral Extended Frame 13Master-ID Protection
Register_HSection 2.5.3.38
4B0h PPSE14MSTID_L Privilege Peripheral Extended Frame 14Master-ID Protection
Register_LSection 2.5.3.38
4B4h PPSE14MSTID_H Privilege Peripheral Extended Frame 14Master-ID Protection
Register_HSection 2.5.3.38
4B8h PPSE15MSTID_L Privilege Peripheral Extended Frame 15Master-ID Protection
Register_LSection 2.5.3.38
4BCh PPSE15MSTID_H Privilege Peripheral Extended Frame 15Master-ID Protection
Register_HSection 2.5.3.38
4C0h PPSE16MSTID_L Privilege Peripheral Extended Frame 16Master-ID Protection
Register_LSection 2.5.3.38
4C4h PPSE16MSTID_H Privilege Peripheral Extended Frame 16Master-ID Protection
Register_HSection 2.5.3.38
4C8h PPSE17MSTID_L Privilege Peripheral Extended Frame 17Master-ID Protection
Register_LSection 2.5.3.38
4CCh PPSE17MSTID_H Privilege Peripheral Extended Frame 17Master-ID Protection
Register_HSection 2.5.3.38
4D0h PPSE18MSTID_L Privilege Peripheral Extended Frame 18Master-ID Protection
Register_LSection 2.5.3.38
4D4h PPSE18MSTID_H Privilege Peripheral Extended Frame 18Master-ID Protection
Register_HSection 2.5.3.38

<!-- Page 221 -->

www.ti.com System andPeripheral Control Registers
221 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
4D8h PPSE19MSTID_L Privilege Peripheral Extended Frame 19Master-ID Protection
Register_LSection 2.5.3.38
4DCh PPSE19MSTID_H Privilege Peripheral Extended Frame 19Master-ID Protection
Register_HSection 2.5.3.38
4E0h PPSE20MSTID_L Privilege Peripheral Extended Frame 20Master-ID Protection
Register_LSection 2.5.3.38
4E4h PPSE20MSTID_H Privilege Peripheral Extended Frame 20Master-ID Protection
Register_HSection 2.5.3.38
4E8h PPSE21MSTID_L Privilege Peripheral Extended Frame 21Master-ID Protection
Register_LSection 2.5.3.38
4ECh PPSE21MSTID_H Privilege Peripheral Extended Frame 21Master-ID Protection
Register_HSection 2.5.3.38
4E0h PPSE22MSTID_L Privilege Peripheral Extended Frame 22Master-ID Protection
Register_LSection 2.5.3.38
4F4h PPSE22MSTID_H Privilege Peripheral Extended Frame 22Master-ID Protection
Register_HSection 2.5.3.38
4F8h PPSE23MSTID_L Privilege Peripheral Extended Frame 23Master-ID Protection
Register_LSection 2.5.3.38
4FCh PPSE23MSTID_H Privilege Peripheral Extended Frame 23Master-ID Protection
Register_HSection 2.5.3.38
500h PPSE24MSTID_L Privilege Peripheral Extended Frame 24Master-ID Protection
Register_LSection 2.5.3.38
504h PPSE24MSTID_H Privilege Peripheral Extended Frame 24Master-ID Protection
Register_HSection 2.5.3.38
508h PPSE25MSTID_L Privilege Peripheral Extended Frame 25Master-ID Protection
Register_LSection 2.5.3.38
50Ch PPSE25MSTID_H Privilege Peripheral Extended Frame 25Master-ID Protection
Register_HSection 2.5.3.38
510h PPSE26MSTID_L Privilege Peripheral Extended Frame 26Master-ID Protection
Register_LSection 2.5.3.38
514h PPSE26MSTID_H Privilege Peripheral Extended Frame 26Master-ID Protection
Register_HSection 2.5.3.38
518h PPSE27MSTID_L Privilege Peripheral Extended Frame 27Master-ID Protection
Register_LSection 2.5.3.38
51Ch PPSE27MSTID_H Privilege Peripheral Extended Frame 27Master-ID Protection
Register_HSection 2.5.3.38
520h PPSE28MSTID_L Privilege Peripheral Extended Frame 28Master-ID Protection
Register_LSection 2.5.3.38
524h PPSE28MSTID_H Privilege Peripheral Extended Frame 28Master-ID Protection
Register_HSection 2.5.3.38
528h PPSE29MSTID_L Privilege Peripheral Extended Frame 29Master-ID Protection
Register_LSection 2.5.3.38
52Ch PPSE29MSTID_H Privilege Peripheral Extended Frame 29Master-ID Protection
Register_HSection 2.5.3.38
530h PPSE30MSTID_L Privilege Peripheral Extended Frame 30Master-ID Protection
Register_LSection 2.5.3.38
534h PPSE30MSTID_H Privilege Peripheral Extended Frame 30Master-ID Protection
Register_HSection 2.5.3.38
538h PPSE31MSTID_L Privilege Peripheral Extended Frame 31Master-ID Protection
Register_LSection 2.5.3.38
53Ch PPSE31MSTID_H Privilege Peripheral Extended Frame 31Master-ID Protection
Register_HSection 2.5.3.38
540h PCS0MSTID Peripheral Memory Frame Master-ID Protection Register0 Section 2.5.3.39
544h PCS1MSTID Peripheral Memory Frame Master-ID Protection Register1 Section 2.5.3.39
548h PCS2MSTID Peripheral Memory Frame Master-ID Protection Register2 Section 2.5.3.39
54Ch PCS3MSTID Peripheral Memory Frame Master-ID Protection Register3 Section 2.5.3.39

<!-- Page 222 -->

System andPeripheral Control Registers www.ti.com
222 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
550h PCS4MSTID Peripheral Memory Frame Master-ID Protection Register4 Section 2.5.3.39
554h PCS5MSTID Peripheral Memory Frame Master-ID Protection Register5 Section 2.5.3.39
558h PCS6MSTID Peripheral Memory Frame Master-ID Protection Register6 Section 2.5.3.39
55Ch PCS7MSTID Peripheral Memory Frame Master-ID Protection Register7 Section 2.5.3.39
560h PCS8MSTID Peripheral Memory Frame Master-ID Protection Register8 Section 2.5.3.39
564h PCS9MSTID Peripheral Memory Frame Master-ID Protection Register9 Section 2.5.3.39
568h PCS10MSTID Peripheral Memory Frame Master-ID Protection Register10 Section 2.5.3.39
56Ch PCS11MSTID Peripheral Memory Frame Master-ID Protection Register11 Section 2.5.3.39
570h PCS12MSTID Peripheral Memory Frame Master-ID Protection Register12 Section 2.5.3.39
574h PCS13MSTID Peripheral Memory Frame Master-ID Protection Register13 Section 2.5.3.39
578h PCS14MSTID Peripheral Memory Frame Master-ID Protection Register14 Section 2.5.3.39
57Ch PCS15MSTID Peripheral Memory Frame Master-ID Protection Register15 Section 2.5.3.39
580h PCS16MSTID Peripheral Memory Frame Master-ID Protection Register16 Section 2.5.3.39
584h PCS17MSTID Peripheral Memory Frame Master-ID Protection Register17 Section 2.5.3.39
588h PCS18MSTID Peripheral Memory Frame Master-ID Protection Register18 Section 2.5.3.39
58Ch PCS19MSTID Peripheral Memory Frame Master-ID Protection Register19 Section 2.5.3.39
590h PCS20MSTID Peripheral Memory Frame Master-ID Protection Register20 Section 2.5.3.39
594h PCS21MSTID Peripheral Memory Frame Master-ID Protection Register21 Section 2.5.3.39
598h PCS22MSTID Peripheral Memory Frame Master-ID Protection Register22 Section 2.5.3.39
59Ch PCS23MSTID Peripheral Memory Frame Master-ID Protection Register23 Section 2.5.3.39
5A0h PCS24MSTID Peripheral Memory Frame Master-ID Protection Register24 Section 2.5.3.39
5A4h PCS25MSTID Peripheral Memory Frame Master-ID Protection Register25 Section 2.5.3.39
5A8h PCS26MSTID Peripheral Memory Frame Master-ID Protection Register26 Section 2.5.3.39
5ACh PCS27MSTID Peripheral Memory Frame Master-ID Protection Register27 Section 2.5.3.39
5B0h PCS28MSTID Peripheral Memory Frame Master-ID Protection Register28 Section 2.5.3.39
5B4h PCS29MSTID Peripheral Memory Frame Master-ID Protection Register29 Section 2.5.3.39
5B8h PCS30MSTID Peripheral Memory Frame Master-ID Protection Register30 Section 2.5.3.39
5BCh PCS31MSTID Peripheral Memory Frame Master-ID Protection Register31 Section 2.5.3.39
5C0h PPCS0MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register0Section 2.5.3.40
5C4h PPCS1MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register1Section 2.5.3.40
5C8h PPCS2MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register2Section 2.5.3.40
5CCh PPCS3MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register3Section 2.5.3.40
5D0h PPCS4MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register4Section 2.5.3.40
5D4h PPCS5MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register5Section 2.5.3.40
5D8h PPCS6MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register6Section 2.5.3.40
5DCh PPCS7MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register7Section 2.5.3.40

<!-- Page 223 -->

www.ti.com System andPeripheral Control Registers
223 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.1 Peripheral Memory Protection SetRegister 0(PMPROTSET0)
This register isshown inFigure 2-72 anddescribed inTable 2-86.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tonon-implemented bitshave noeffect andreads are0.
Figure 2-72. Peripheral Memory Protection SetRegister 0(PMPROTSET0) (offset =00h)
31 0
PCS[31-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-86. Peripheral Memory Protection SetRegister 0(PMPROTSET0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PROTSET Peripheral memory frame protection set.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET0 andPMPROTCLR0 registers issetto1.
2.5.3.2 Peripheral Memory Protection SetRegister 1(PMPROTSET1)
This register isshown inFigure 2-73 anddescribed inTable 2-87.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-73. Peripheral Memory Protection SetRegister 1(PMPROTSET1) (offset =04h)
31 0
PCS[63-32]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-87. Peripheral Memory Protection SetRegister 1(PMPROTSET1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PROTSET Peripheral memory frame protection set.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET1 andPMPROTCLR1 registers issetto1.

<!-- Page 224 -->

System andPeripheral Control Registers www.ti.com
224 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.3 Peripheral Memory Protection Clear Register 0(PMPROTCLR0)
This register isshown inFigure 2-74 anddescribed inTable 2-88.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-74. Peripheral Memory Protection Clear Register 0(PMPROTCLR0) (offset =10h)
31 0
PCS[31-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-88. Peripheral Memory Protection Clear Register 0(PMPROTCLR0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PROTCLR Peripheral memory frame protection clear.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET0 andPMPROTCLR0 registers iscleared to0.
2.5.3.4 Peripheral Memory Protection Clear Register 1(PMPROTCLR1)
This register isshown inFigure 2-75 anddescribed inTable 2-89.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-75. Peripheral Memory Protection Clear Register 1(PMPROTCLR1) (offset =14h)
31 0
PCS[63-32]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-89. Peripheral Memory Protection Clear Register 1(PMPROTCLR1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PROTCLR Peripheral memory frame protection clear.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET1 andPMPROTCLR1 registers iscleared to0.

<!-- Page 225 -->

www.ti.com System andPeripheral Control Registers
225 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.5 Peripheral Protection SetRegister 0(PPROTSET0)
There isonebitforeach quadrant forPS0 toPS7.
The following aretheways thatquadrants areused within aPSframe:
a.The slave uses allthefour quadrants.
Only thebitcorresponding tothequadrant 0ofPSn isimplemented. Itprotects thewhole 1K-byte
frame. The remaining three bitsarenotimplemented.
b.The slave uses twoquadrants.
Each quadrant hastobeinoneofthese groups: (Quad 0andQuad 1)or(Quad 2andQuad 3).
Forthegroup Quad0/Quad1, thebitquadrant 0protects both quadrants 0and1.The bitquadrant 1is
notimplemented.
Forthegroup Quad2/Quad3, thebitquadrant 2protects both quadrants 2and3.The bitquadrant 3is
notimplemented
c.The slave uses only onequadrant.
Inthiscase, thebit,asspecified inTable 2-90 ,protects theslave.
The above arrangement istrue foralltheperipheral selects (PS0 toPS31), presented inSection 2.5.3.6 -
Section 2.5.3.12 .This register holds bitsforPS0 toPS7 andisshown inFigure 2-76 anddescribed in
Table 2-90.
NOTE: Writes tounimplemented bitshave noeffect andreads are0.
Figure 2-76. Peripheral Protection SetRegister 0(PPROTSET0) (offset =20h)
31 0
PS[7-0]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-90. Peripheral Protection SetRegister 0(PPROTSET0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant anbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET0 andPPROTCLR0 registers issetto1.

<!-- Page 226 -->

System andPeripheral Control Registers www.ti.com
226 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.6 Peripheral Protection SetRegister 1(PPROTSET1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-77 anddescribed inTable 2-91 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-77. Peripheral Protection SetRegister 1(PPROTSET1) (offset =24h)
31 0
PS[15-8]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-91. Peripheral Protection SetRegister 1(PPROTSET1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET1 andPPROTCLR1 registers issetto1.
2.5.3.7 Peripheral Protection SetRegister 2(PPROTSET2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-78 anddescribed inTable 2-92 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-78. Peripheral Protection SetRegister 2(PPROTSET2) (offset =28h)
31 0
PS[23-16]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-92. Peripheral Protection SetRegister 2(PPROTSET2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET2 andPPROTCLR2 registers issetto1.

<!-- Page 227 -->

www.ti.com System andPeripheral Control Registers
227 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.8 Peripheral Protection SetRegister 3(PPROTSET3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-79 anddescribed inTable 2-93 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-79. Peripheral Protection SetRegister 3(PPROTSET3) (offset =2Ch)
31 0
PS[31-24]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-93. Peripheral Protection SetRegister 3(PPROTSET3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET3 andPPROTCLR3 registers issetto1.
2.5.3.9 Peripheral Protection Clear Register 0(PPROTCLR0)
There isonebitforeach quadrant forPS0 toPS7. The protection scheme isdescribed inSection 2.5.3.5 .
This register isshown inFigure 2-80 anddescribed inTable 2-94.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-80. Peripheral Protection Clear Register 0(PPROTCLR0) (offset =40h)
31 0
PS[7-0]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-94. Peripheral Protection Clear Register 0(PPROTCLR0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET0 andPPROTCLR0 registers iscleared to0.

<!-- Page 228 -->

System andPeripheral Control Registers www.ti.com
228 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.10 Peripheral Protection Clear Register 1(PPROTCLR1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-81 anddescribed inTable 2-95 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-81. Peripheral Protection Clear Register 1(PPROTCLR1) (offset =44h)
31 0
PS[15-8]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-95. Peripheral Protection Clear Register 1(PPROTCLR1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET1 andPPROTCLR1 registers iscleared to0.
2.5.3.11 Peripheral Protection Clear Register 2(PPROTCLR2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-82 anddescribed inTable 2-96 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-82. Peripheral Protection Clear Register 2(PPROTCLR2) (offset =48h)
31 0
PS[23-16]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-96. Peripheral Protection Clear Register 2(PPROTCLR2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET2 andPPROTCLR2 registers iscleared to0.

<!-- Page 229 -->

www.ti.com System andPeripheral Control Registers
229 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.12 Peripheral Protection Clear Register 3(PPROTCLR3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-83 anddescribed inTable 2-97 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-83. Peripheral Protection Clear Register 3(PPROTCLR3) (offset =4Ch)
31 0
PS[31-24]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-97. Peripheral Protection Clear Register 3(PPROTCLR3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET3 andPPROTCLR3 registers iscleared to0.

<!-- Page 230 -->

System andPeripheral Control Registers www.ti.com
230 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.13 Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0)
Each bitcorresponds toabitatthesame index inthePMPROT register inthatthey both relate tothe
same peripheral. This register isshown inFigure 2-84 anddescribed inTable 2-98.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-84. Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0) (offset =60h)
31 0
PCS[31-0]PWRDNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-98. Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PWRDNSET Peripheral memory clock power-down set.
0 Read: The peripheral memory clock[31-0] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[31-0] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET0 andPCSPWRDWNCLR0 registers
issetto1.
2.5.3.14 Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1)
This register isshown inFigure 2-85 anddescribed inTable 2-99.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-85. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1) (offset =64h)
31 0
PCS[63-32]PWRDNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-99. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PWRDNSET Peripheral memory clock power-down set.
0 Read: The peripheral memory clock[63-32] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[63-32] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET1 andPCSPWRDWNCLR1 registers
issetto1.

<!-- Page 231 -->

www.ti.com System andPeripheral Control Registers
231 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.15 Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
This register isshown inFigure 2-86 anddescribed inTable 2-100 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-86. Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
(offset =70h)
31 0
PCS[31-0]PWRDNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-100. Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PWRDNCLR Peripheral memory clock power-down clear.
0 Read: The peripheral memory clock[31-0] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[31-0] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET0 andPCSPWRDWNCLR0 registers
iscleared to0.
2.5.3.16 Peripheral Memory Power-Down Clear Register 1(PCSPWRDWNCLR1)
This register isshown inFigure 2-87 anddescribed inTable 2-101 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-87. Peripheral Memory Power-Down Clear Register 1(PCSPWRDWNCLR1)
(offset =74h)
31 0
PCS[63-32]PWRDNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-101. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNCLR1)
Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PWRDNCLR Peripheral memory clock power-down clear.
0 Read: The peripheral memory clock[63-32] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[63-32] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET1 andPCSPWRDWNCLR1 registers
iscleared to0.

<!-- Page 232 -->

System andPeripheral Control Registers www.ti.com
232 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.17 Peripheral Power-Down SetRegister 0(PSPWRDWNSET0)
There isonebitforeach quadrant forPS0 toPS7. Each bitofthisregister corresponds tothebitatthe
same index inthecorresponding PPROT register inthatthey relate tothesame peripheral. These bitsare
used topower down/power uptheclock tothecorresponding peripheral.
Forevery bitimplemented inthePPROT register, there isonebitinthePSnPWRDWN register, except
when twoperipherals (both inPSarea) share buses. Inthatcase, only onePower-Down bitis
implemented, attheposition corresponding tothatperipheral whose quadrant comes first(the lower
numbered).
The ways inwhich quadrants canbeused within aframe areidentical towhat isdescribed under
PPROTSET0, Section 2.5.3.5 .
This arrangement isthesame forbitsofPS8 toPS31, presented inSection 2.5.3.18 -Section 2.5.3.24 .
This register holds bitsforPS0 toPS7. This register isshown inFigure 2-88 anddescribed inTable 2-
102.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-88. Peripheral Power-Down SetRegister 0(PSPWRDWNSET0) (offset =80h)
31 0
PS[7-0]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-102. Peripheral Power-Down SetRegister 0(PSPWRDWNSET0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET0 andPSPWRDWNCLR0 registers isset
to1.

<!-- Page 233 -->

www.ti.com System andPeripheral Control Registers
233 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.18 Peripheral Power-Down SetRegister 1(PSPWRDWNSET1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-89 anddescribed inTable 2-103 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-89. Peripheral Power-Down SetRegister 1(PSPWRDWNSET1) (offset =84h)
31 0
PS[15-8]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-103. Peripheral Power-Down SetRegister 1(PSPWRDWNSET1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET1 andPSPWRDWNCLR1 registers isset
to1.
2.5.3.19 Peripheral Power-Down SetRegister 2(PSPWRDWNSET2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-90 anddescribed inTable 2-104 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-90. Peripheral Power-Down SetRegister 2(PSPWRDWNSET2) (offset =88h)
31 0
PS[23-16]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-104. Peripheral Power-Down SetRegister 2(PSPWRDWNSET2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET2 andPSPWRDWNCLR2 registers isset
to1.

<!-- Page 234 -->

System andPeripheral Control Registers www.ti.com
234 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.20 Peripheral Power-Down SetRegister 3(PSPWRDWNSET3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-91 anddescribed inTable 2-105 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-91. Peripheral Power-Down SetRegister 3(PSPWRDWNSET3) (offset =8Ch)
31 0
PS[31-24]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-105. Peripheral Power-Down SetRegister 3(PSPWRDWNSET3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET3 andPSPWRDWNCLR3 registers isset
to1.
2.5.3.21 Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0)
There isonebitforeach quadrant forPS0 toPS7. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-92 anddescribed inTable 2-106 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-92. Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0) (offset =A0h)
31 0
PS[7-0]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-106. Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET0 andPSPWRDWNCLR0 registers is
cleared to0.

<!-- Page 235 -->

www.ti.com System andPeripheral Control Registers
235 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.22 Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-93 anddescribed inTable 2-107 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-93. Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1) (offset =A4h)
31 0
PS[15-8]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-107. Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET1 andPSPWRDWNCLR1 registers is
cleared to0.
2.5.3.23 Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-94 anddescribed inTable 2-108 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-94. Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2) (offset =A8h)
31 0
PS[23-16]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-108. Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET2 andPSPWRDWNCLR2 registers is
cleared to0.

<!-- Page 236 -->

System andPeripheral Control Registers www.ti.com
236 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.24 Peripheral Power-Down Clear Register 3(PSPWRDWNCLR3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-95 anddescribed inTable 2-109 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-95. Peripheral Power-Down Clear Register 3(PSPWRDWNCLR) (offset =ACh)
31 0
PS[31-24]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-109. Peripheral Power-Down Clear Register 3(PSPWRDWNCLR3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET3 andPSPWRDWNCLR3 registers is
cleared to0.
2.5.3.25 Debug Frame Powerdown SetRegister (PDPWRDWNSET)
Figure 2-96. Debug Frame Powerdown SetRegister (PDPWRDWNSET) (offset =C0h)
31 1 0
Reserved PDWRDWNSET
R-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-110. Debug Frame Powerdown SetRegister (PDPWRDWNSET) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PDWRDWNSET Debug Frame Powerdown SetRegister.
0 Read: The clock tothedebug frame isactive.
Write: The bitisunchanged.
1 Read: The clock tothedebug frame isinactive.
Write: Setthebitto1.

<!-- Page 237 -->

www.ti.com System andPeripheral Control Registers
237 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.26 Debug Frame Powerdown Clear Register (PDPWRDWNCLR)
Figure 2-97. Debug Frame Powerdown Clear Register (PDPWRDWNCLR) (offset =C4h)
31 1 0
Reserved PDWRDWNCLR
R-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-111. Debug Frame Powerdown Clear Register (PDPWRDWNCLR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PDWRDWNCLR Debug Frame Powerdown SetRegister.
0 Read: The clock tothedebug frame isactive.
Write: The bitisunchanged.
1 Read: The clock tothedebug frame isinactive.
Write: Clear thebitto0.
2.5.3.27 MasterID Protection Write Enable Register (MSTIDWRENA)
Figure 2-98. MasterID Protection Write Enable Register (MSTIDWRENA) (offset =200h)
31 16
Reserved
R-0
15 4 3 0
Reserved MSTIDREG_WRENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-112. MasterID Protection Write Enable Register (MSTIDWRENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTIDREG_WRENA MasterID Register Write Enable. This isa4-bit keyforenabling writes toallMaster-ID
registers from address offset 0x300-0x5DC. This keymust beprogrammed with 1010 to
unlock writes toallMaster-ID registers.
Ah Read: Allmaster-ID registers areunlocked andavailable forwrites.
Write: Writes tomaster-ID registers areunlocked.
Others Read: Writes toallmaster-ID registers arelocked.
Write: Write tomaster-ID registers arelocked.

<!-- Page 238 -->

System andPeripheral Control Registers www.ti.com
238 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.28 MasterID Enable Register (MSTIDENA)
Figure 2-99. MasterID Enable Register (MSTIDENA) (offset =204h)
31 16
Reserved
R-0
15 4 3 0
Reserved MSTID_CHK_ENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-113. MasterID Enable Register (MSTIDENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTID_CHK_ENA MasterID Check Enable. This isa4-bit keyforenabling Master-ID check. This keymust be
programmed with 1010 toenable Master-ID Check functionality.
Ah Read: The master-ID check isenabled.
Write: Enable master-ID check.
Others Read: The master-ID check isdisabled.
Write: Disable master-ID check.

<!-- Page 239 -->

www.ti.com System andPeripheral Control Registers
239 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.29 MasterID Diagnostic Control Register (MSTIDDIAGCTRL)
Figure 2-100. MasterID Diagnostic Control Register (MSTIDDIAGCTRL) (offset =208h)
31 16
Reserved
R-0
15 12 11 8 7 4 3 0
Reserved DIAG_CMP_VALUE Reserved DIAG_MODE_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-114. MasterID Diagnostic Control Register (MSTIDDIAGCTRL) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 DIAG_CMP_VALUE Diagnostic Compare Value. The value stored inthisregister iscompared against the
programmed master-ID register bitsforallaccesses. Indiagnostic mode, themaster-ID
register selection depends ontheDIAG_CMP_VALUE instead oftheinput 4-bit master-ID
generated bytheinterconnect. Any mismatch willbesignaled tothebusmaster asabus
error. After thediagnostic mode isenabled inDIAG_MODE_ENA register andadiagnostic
compare value isprogrammed intotheDIAG_CMP_VALUE register, theapplication must
issue adummy diagnostic write access toanyoneoftheperipherals tocause adiagnostic
check. Forexample, ifallmaster-ID protection registers listed from address 0x300-0x5DC
areprogrammed toblock master-ID 5from write access totheperipherals, then the
application canprogram theDIAG_CMP_VALUE to5.The application canusetheCPU
(whose master-ID is0)toissue adummy write access toanyperipheral tocause master-ID
violation during diagnostic mode instead ofusing thebusmaster whose master-ID is5to
perform thiswrite access.
Ah Read: The master-ID check isenabled.
Write: Enable master-ID check.
Others Read: The master-ID check isdisabled.
Write: Disable master-ID check.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 DIAG_MODE_ENA Diagnostic Mode Enable. This isa4-bit keyforenabling Diagnostic Mode. This keymust be
programmed with 1010 toenable Diagnostic Mode.
Ah Read: The diagnostic mode isenabled.
Write: Enable diagnostic mode.
Others Read: The diagnostic mode isdisabled.
Write: Disable diagnostic mode.

<!-- Page 240 -->

System andPeripheral Control Registers www.ti.com
240 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.30 Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
There isonebitforeach quadrant forPS0 toPS31.
NOTE: Ifamodule occupies twoquadrants, then only thelower quadrant register isused toenable
ordisable themasterID. The upper quadrant register remains zeros.
The following aretheways thatquadrants areused within aPSframe:
a.The slave uses allthefour quadrants.
Only thebitcorresponding tothequadrant 0ofPSn isimplemented. Itprotects thewhole 1K-byte
frame. The remaining three bitsarenotimplemented.
b.The slave uses twoquadrants.
Each quadrant hastobeinoneofthese groups: (Quad 0andQuad 1)or(Quad 2andQuad 3).
Forthegroup Quad0/Quad1, thebitquadrant 0protects both quadrants 0and1.The bitquadrant 1is
notimplemented.
Forthegroup Quad2/Quad3, thebitquadrant 2protects both quadrants 2and3.The bitquadrant 3is
notimplemented
c.The slave uses only onequadrant.
Inthiscase, thebit,asspecified inTable 2-115 ,protects theslave.
The above arrangement istrue foralltheperipheral selects (PS0 toPS31), presented inSection 2.5.3.31 -
Section 2.5.3.32 .This register holds bitsforPS0 andisshown inFigure 2-101 anddescribed inTable 2-
115.
Figure 2-101. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
(offset =300h)
31 16
PS0_QUAD1_MSTID
R/WP-FFFFh
15 0
PS0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-115. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
Field Descriptions
Bit Field Value Description
31-16 PS0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPS[0]. There are16bitsforeach quadrant inPSframe.
Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds tomaster-ID 0
andbit15corresponds tomaster-ID 15.These bitssetthepermission formaximum of16
masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPS[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPS[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 241 -->

www.ti.com System andPeripheral Control Registers
241 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-115. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
Field Descriptions (continued)
Bit Field Value Description
15-0 PS0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 242 -->

System andPeripheral Control Registers www.ti.com
242 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.31 Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
There isonebitforeach quadrant forPS0 toPS31. The protection scheme isdescribed in
Section 2.5.3.30 .This register isshown inFigure 2-102 anddescribed inTable 2-116 .
Figure 2-102. Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
(offset =304h)
31 16
PS0_QUAD3_MSTID
R/WP-FFFFh
15 0
PS0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-116. Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
Field Descriptions
Bit Field Value Description
31-16 PS0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PS0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 243 -->

www.ti.com System andPeripheral Control Registers
243 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.32 Peripheral Frame nMasterID Protection Register_L/H (PS[1-31]MSTID_L/H)
There isonebitforeach quadrant forPS0 toPS31. The protection scheme isdescribed in
Section 2.5.3.30 .This register isshown inFigure 2-103 anddescribed inTable 2-117 .
Figure 2-103. Peripheral Frame nMasterID Protection Register_L/H (PSnMSTID_L/H)
(offset =308h-3FCh)
31 16
PSn_QUAD3_MSTID orPSn_QUAD1_MSTID
R/WP-FFFFh
15 0
PSn_QUAD2_MSTID orPSn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-117. Peripheral Frame nMasterID Protection Register_L/H (PSnMSTID_L/H)
Field Descriptions
Bit Field Value Description
31-16 PSn_QUAD3_MSTID or
PSn_QUAD1_MSTIDn:1to31.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPS[n] orQuadrant 1ofPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PSn_QUAD2_MSTID or
PSn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPS[n] orQuadrant 0ofPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 244 -->

System andPeripheral Control Registers www.ti.com
244 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.33 Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
Figure 2-104. Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
(offset =400h)
31 16
PPS0_QUAD1_MSTID
R/WP-FFFFh
15 0
PPS0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-118. Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
Field Descriptions
Bit Field Value Description
31-16 PPS0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPPS[0]. There are16bitsforeach quadrant inPPS
frame. Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds to
master-ID 0andbit15corresponds tomaster-ID 15.These bitssetthepermission for
maximum of16masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPPS[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPPS[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
15-0 PPS0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 245 -->

www.ti.com System andPeripheral Control Registers
245 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.34 Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
Figure 2-105. Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
(offset =404h)
31 16
PPS0_QUAD3_MSTID
R/WP-FFFFh
15 0
PPS0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-119. Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
Field Description
Bit Field Value Description
31-16 PPS0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPS0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 246 -->

System andPeripheral Control Registers www.ti.com
246 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.35 Privileged Peripheral Frame nMasterID Protection Register_L/H (PPS[1-7]MSTID_L/H)
Figure 2-106. Privileged Peripheral Frame nMasterID Protection Register_L/H (PPSnMSTID_L/H)
(offset =408h-43Ch)
31 16
PPSn_QUAD3_MSTID orPPSn_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSn_QUAD2_MSTID orPPSn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-120. Privileged Peripheral Frame nMasterID Protection Register_L/H (PPSnMSTID_L/H)
Field Descriptions
Bit Field Value Description
31-16 PPSn_QUAD3_MSTID or
PPSn_QUAD1_MSTIDn:1to7.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPPS[n] orQuadrant 1ofPPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSn_QUAD2_MSTID or
PPSn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPPS[n] orQuadrant 0ofPPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 247 -->

www.ti.com System andPeripheral Control Registers
247 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.36 Privileged Peripheral Extended Frame 0MasterID Protection Register_L (PPSE0MSTID_L)
Figure 2-107. Privileged Peripheral Extended Frame 0MasterID Protection Register_L
(PPSE0MSTID_L) (offset =440h)
31 16
PPSE0_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSE0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-121. Privileged Peripheral Extended Frame 0MasterID Protection Register_L
(PPSE0MSTID_L) Field Descriptions
Bit Field Value Description
31-16 PPSE0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPPSE[0]. There are16bitsforeach quadrant inPPSE
frame. Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds to
master-ID 0andbit15corresponds tomaster-ID 15.These bitssetthepermission for
maximum of16masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPPSE[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPPSE[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
15-0 PPSE0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 248 -->

System andPeripheral Control Registers www.ti.com
248 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.37 Privileged Peripheral Extended Frame 0MasterID Protection Register_H (PPSE0MSTID_H)
Figure 2-108. Privileged Peripheral Extended Frame 0MasterID Protection Register_H
(PPSE0MSTID_H) (offset =444h)
31 16
PPSE0_QUAD3_MSTID
R/WP-FFFFh
15 0
PPSE0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-122. Privileged Peripheral Extended Frame 0MasterID Protection Register_H
(PPSE0MSTID_H) Field Descriptions
Bit Field Value Description
31-16 PPSE0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSE0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 249 -->

www.ti.com System andPeripheral Control Registers
249 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.38 Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSE[1-31]MSTID_L/H)
Figure 2-109. Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSEnMSTID_L/H) (offset =448h-53Ch)
31 16
PPSEn_QUAD3_MSTID orPPSEn_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSEn_QUAD2_MSTID orPPSEn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-123. Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSEnMSTID_L/H) Field Descriptions
Bit Field Value Description
31-16 PPSEn_QUAD3_MSTID
or
PPSEn_QUAD1_MSTIDn:1to31.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPPSE[n] orQuadrant 1ofPPSE[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSEn_QUAD2_MSTID
or
PPSEn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPPSE[n] orQuadrant 0ofPPSE[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 250 -->

System andPeripheral Control Registers www.ti.com
250 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.39 Peripheral Memory Frame MasterID Protection Register (PCS[0-31]MSTID)
Figure 2-110. Peripheral Memory Frame MasterID Protection Register (PCSnMSTID)
(offset =540h-5BCh)
31 16
PCS(2n+1)_MSTID
R/WP-FFFFh
15 0
PCS(2n)_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-124. Peripheral Memory Frame MasterID Protection Register (PCSnMSTID)
Field Descriptions
Bit Field Value Description
31-16 PCS(2n+1)_MSTID MasterID filtering forPCS[2n+1], where n=0to31.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PCS(2n)_MSTID MasterID filtering forPCS[2n], where n=0to31.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 251 -->

www.ti.com System andPeripheral Control Registers
251 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.40 Privileged Peripheral Memory Frame MasterID Protection Register (PPCS[0-7]MSTID)
Figure 2-111. Privileged Peripheral Memory Frame MasterID Protection Register (PPCSnMSTID)
(offset =5C0h-5DCh)
31 16
PPCS(2n+1)_MSTID
R/WP-FFFFh
15 0
PPCS(2n)_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-125. Privileged Peripheral Memory Frame MasterID Protection Register (PPCSnMSTID)
Field Descriptions
Bit Field Value Description
31-16 PPCS(2n+1)_MSTID MasterID filtering forPPCS[2n+1], where n=0to7.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPCS(2n)_MSTID MasterID filtering forPPCS[2n], where n=0to7.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.