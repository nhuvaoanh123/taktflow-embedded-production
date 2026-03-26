# Interconnect

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 265-278

---


<!-- Page 265 -->

265 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnectChapter 4
SPNU563A -March 2018
Interconnect
This chapter describes thetwointerconnects inthemicrocontroller.
Topic ........................................................................................................................... Page
4.1 Overview ......................................................................................................... 266
4.2 Peripheral Interconnect Subsystem .................................................................... 266
4.3 CPU Interconnect Subsystem ............................................................................ 268
4.4 SDC MMR Registers .......................................................................................... 272

<!-- Page 266 -->

HTU1 FTU HTU2
Peripheral Interconnect SubsystemDMA
PCR1CPU Interconnect SubsystemR5F
POMDMM DAP
PCR2 PCR3 CRC1 CRC2EMAC
Flash EMIF SRAMA BA
B
ACP
SDC MMRPS_SCR_SPS_SCR_MACP-M
ACP-S SDC MMR Port
Overview www.ti.com
266 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.1 Overview
The interconnect isabusmatrix which interconnects theCPU cores, System DMA, other busmasters and
device specific slaves within themicrocontroller. There aretwointerconnects inthemicrocontroller: the
CPU Interconnect Subsystem andthePeripheral Interconnect Subsystem. The interconnects direct the
access requests bythemasters byproviding decoding, arbitration, androuting oftherequests tothe
various slaves.
4.1.1 Block Diagram
Figure 4-1isablock diagram oftheInterconnects implemented inthisfamily ofmicrocontrollers.
Figure 4-1.Interconnect Block Diagram
4.2 Peripheral Interconnect Subsystem
There aremasters andslaves connected tothePeripheral Interconnect Subsystem. The Peripheral
Interconnect Subsystem isnotafullcross-bar. Notallmasters canaccess toallslaves. Table 4-1lists the
implemented point-to-point connections between themasters andslaves.
Table 4-1.Bus Master /Slave Connectivity forPeripheral Interconnect Subsystem
MastersMaster IDto
PCRx Access ModeSlaves onPeripheral Interconnect Subsystem
CRC1 CRC2 PCR1 PCR2 PCR3 PS_SCR_SSDC MMR
Port
CPU
Read/Write0 User/Privilege Yes Yes Yes Yes Yes No Yes
DMA Port B 2 User Yes Yes Yes Yes Yes No No
HTU1 3 Privilege No No No No No Yes No
HTU2 4 Privilege No No No No No Yes No
FTU 5 User No No No No No Yes No
DMM 7 User Yes Yes Yes Yes Yes Yes No
DAP 9 Privilege Yes Yes Yes Yes Yes Yes No
EMAC 10 User No No No Yes Yes Yes No

<!-- Page 267 -->

www.ti.com Peripheral Interconnect Subsystem
267 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.2.1 Accessing PCRx andCRCx Slave
System peripherals canbeaccessed viathePCR1 slave port. User peripherals canbeaccessed via
either thePCR2 orPCR3 slave ports. Refer tothedatasheet forinformation onwhat peripherals are
available through each PCR. Peripheral Central Resource (PCR) isresponsible tofurther decode the
slave address toselect thedesired peripheral.
There aretwoCRC modules implemented inthedevice. Both aredirect slaves tothePeripheral
Interconnect Subsystem.
4.2.2 Accessing SDC MMR Port Slave
Safety Diagnostic Controller (SDC) MMR Port isaslave tothePeripheral Interconnect Subsystem to
access thesafety diagnostic related control andstatus registers oftheCPU Interconnect Subsystem.
Table 4-2lists theCPU Interconnect Subsystem SDC register bitfield mapping.
Table 4-2.CPU Interconnect Subsystem SDC Register BitField Mapping
Register Name Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Remark
ERR_GENERIC_
PARITYPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedEach bitindicates the
transaction processing block
inside theinterconnect
corresponding tothemaster
thatisdetected bythe
interconnect checker tohave a
fault.
Error related toparity
mismatch intheincoming
address.
ERR_UNEXPECTED_
TRANSPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedError related tounexpected
transaction sent bythemaster.
ERR_TRANS_IDPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedError related tomismatch on
thetransaction ID.
ERR_TRANS_
SIGNATUREPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedError related tomismatch on
thetransaction signature.
ERR_TRANS_TYPEPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedError related tomismatch on
thetransaction type.
ERR_USER_PARITYPS_SCR_
MPOMDMA_
PORTACPU
AXI-MReserved ACP-M ReservedError related tomismatch on
theparity.
SERR_UNEXPECTED_
MIDL2SRAM
WrapperL2Flash
Wrapper
Port AL2Flash
Wrapper
Port BEMIF ReservedCPU
AXi-SACP-SEach bitindicates the
transaction processing block
inside theinterconnect
corresponding totheslave that
isdetected bytheinterconnect
checker tohave afault.
Error related tomismatch on
themaster ID.
SERR_ADDR_
DECODEL2SRAM
WrapperL2Flash
Wrapper
Port AL2Flash
Wrapper
Port BEMIF ReservedCPU
AXi-SACP-SError related tomismatch on
themost significant address
bits.
SERR_USER_PARITYL2SRAM
WrapperL2Flash
Wrapper
Port AL2Flash
Wrapper
Port BEMIF ReservedCPU
AXi-SACP-SError related tomismatch on
theparity ofthemost
significant address bits.
4.2.3 Accessing Other Slaves viaPS_SCR_S
Inorder forsome ofthemasters connected tothePeripheral Interconnect Subsystem toaccess theslaves
such asL2Flash andL2SRAM intheCPU Interconnect Subsystem, their requests arefirstfunneled into
thePS_SCR_S slave where itthen becomes amaster ontheCPU Interconnect Subsystem as
PS_SCR_M. The request appearing onthePS_SCR_M isthen decoded androuted totheintended slave
bytheCPU Interconnect Subsystem.

<!-- Page 268 -->

CPU Interconnect Subsystem www.ti.com
268 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.3 CPU Interconnect Subsystem
The masters andslaves areconnected totheCPU Interconnect Subsystem. The CPU Interconnect
Subsystem isnotafullcross-bar. Notallmasters canaccess toallslaves. Table 4-3lists theimplemented
point topoint connections between themasters andslaves. What isalso unique totheCPU Interconnect
Subsystem isthattheinterconnect andallthemasters andslaves thatconnect toitconstitute onesafety
island where alltransactions toandfrom themasters andslaves areprotected onthedata path byECC.
Address andcontrol signals onalltransactions areprotected byparity. Inaddition, theCPU Interconnect
Subsystem contains abuilt-in hardware Safety Diagnostic Checker oneach master andslave interface
where itconstantly monitors theintegrity oftraffics between themasters andslaves. The CPU
Interconnect Subsystem also hasaself-test capability thatwhen enabled willinject teststimulus onto each
master andslave interface anddiagnose theinterconnect itself.
(1)The access mode forPS_SCR_M depends onwhich master ontheperipheral side (HTU1, HTU2, FTU, DMM, DAP, and
EMAC), seeTable 4-1,isaccessing thememories ontheCPU side. The ACP_M access mode reflects thePS_SCR_M access
mode.Table 4-3.Bus Master /Slave Connectivity forCPU Interconnect Subsystem
Masters Access ModeSlaves onCPU Interconnect Subsystem
L2Flash Port A L2Flash Port B L2SRAM CPU AXI-S EMIF ACP-S
CPU Read User/Privilege Yes Yes Yes Yes Yes No
CPU Write User/Privilege No No Yes Yes Yes No
DMA Port A User No Yes Yes No Yes Yes
POM User No No Yes No Yes Yes
PS_SCR_M See(1)No Yes Yes No Yes Yes
ACP_M See(1)No No Yes No No No
4.3.1 Slave Accessing
4.3.1.1 Accessing L2Flash Slave
There aretwoflash slave ports which allow possible parallel requests bythemasters todifferent flash
banks atthesame time. There aretwoflash banks of2Mbytes each implemented inthedevice. Itis
possible forCPU0 toaccess oneflash bank viaFlash PortA while DMA accesses totheother flash bank
viaFlash PortB.
4.3.1.2 Accessing L2SRAM Slave
Inorder fortheDMA PortA, POM andPS_SCR_M toaccess theL2SRAM, their requests arefirst
funneled intoACP-S slave port. Accelerated Coherency Port (ACP) isahardware which provides memory
coherency checking between each CPU intheCortex-R5 group andanexternal master. Accesses made
bytheDMA PortA, POM andPS_SCR_M arefirstchecked bytheACP coherency hardware toseeifthe
write data isalready intheCPU's data cache. When awrite from theDMA PortA, POM andPS_SCR_M
appears ontheACP slave, theACP records some information about itandforward thewrite transactions
totheL2SRAM ontheACP-M master port. When thememory system sends thewrite response onthe
ACP-M master port, theACP records theresponse andrecalls ifthetransaction was coherent. Ifthe
transaction isnotcoherent, theACP forwards theresponse tothebusmaster ontheACP-S slave port. If
thetransaction iscoherent, theACP firstsends coherency maintenance operations totheCPU's data
cache controller fortheaddresses spanned bythewrite transaction, andwait until thecache controller has
acknowledged thatallnecessary coherency maintenance operations have been carried outtoforward the
write response totheACP-S slave port. CPUs have direct access totheL2SRAM.
4.3.1.3 Accessing EMIF Slave
Allbusmasters ontheCPU Interconnect Subsystem have apoint topoint connection totheEMIF slave
without going through ACP forcoherency check. Coherency maintenance ontheEMIF between theCPU
andother masters willneed tobehandled bysoftware.

<!-- Page 269 -->

www.ti.com CPU Interconnect Subsystem
269 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.3.1.4 Accessing Cache Memories
Both theinstruction anddata caches oftheCPU arememory mapped inthedevice andcanbeaccessed
viatheAXI-S slave port. Only theCPU core haspoint topoint connection totheAXI-S slave port.
4.3.2 ECC Generation andEvaluation
CPU core contain sthebuilt-in ECC generation andevaluation logic foritsAXIinterface. Therefore, CPU
willgenerate theECC checksum along with itswrite data. The write data andthecorresponding ECC
checksum aretransported bytheinterconnect totheselected slave such asL2SRAM. When CPU core
performs aread from aslave, theslave returns thedata andthecorresponding ECC checksum. Upon
receiving thedata andtheECC checksum, theCPU willevaluate theintegrity ofthedata byperforming
theECC check. ECC errors detected ontheCPU's AXIinterface areexported bytheCPU toitsevent bus
output. The error signals ifenabled andthecorresponding error addresses arefirstrouted totheError
Profiling Controller (EPC) module. EPC isused torecord different single biterror addresses inaContent
Addressable Memory (CAM). The main purpose oftheEPC module istoenable thesystem totolerate a
certain amount ofECC correctable errors onthesame address repeated inthememory system with
minimal runtime overhead. IfanECC error isgenerated onarepeating address, theEPC willnotraise an
error toESM module. This tolerance avoids theapplication tohandle thesame error when thecode isina
repeating loop. See EPC chapter formore information.
DMA PortA andPS_SCR_M masters donothave built-in ECC generation andevaluation logic. Therefore,
theCPU Interconnect Subsystem contain astandalone ECC generation andevaluation logic foreach
DMA PortA andPS_SCR_M master. Write transactions initiated bytheDMA PortA andPS_SCR_M
masters arefirsttreated bytheECC block togenerate theECC checksum before transporting tothefinal
destination. Forread transactions, thedata andECC checksum returned bytheslaves willpass through
theECC block fordata integrity evaluation.
ECC errors detected arealso routed totheError Profiling Controller (EPC) module. Inorder forthe
standalone ECC block toassert theerror signals totheEPC, theerror enable keymust befirstsetinthe
IP1ECCERREN register oftheSYS2 module.
NOTE: Toenable error signal assertion totheESM forECC errors detected forDMA, theapplication
must write 0xA totheIP1_ECC_KEY bits. Toenable error signal assertion totheESM for
ECC errors detected forPS_SCR_M, theapplication must write 0xA totheIP2_ECC_KEY
bits.
4.3.3 Safety Diagnostic Checker
Foreach master andslave interface intheCPU Interconnect Subsystem, there isaruntime Safety
Diagnostic Checker. The hardware checker continuously watches transactions flowing through the
interconnect andensuring they arenoncorrupted atalltime. Ifamismatch isdetected between an
ongoing transaction andtheexpected transaction flow then anerror isasserted totheESM Group 1.
Types oferrors arerecorded intheSDC MMR registers. See Section 4.4foralltheregisters. Once an
error isdetected andtheerror type islogged, theapplication willclear theruntime diagnostic errors by
writing an0xA keytotheGLOBAL_ERROR_CLR bitsoftheSCMCNTRL register intheSCR Control
Module (SCM). See theSCM Chapter formore information.

<!-- Page 270 -->

CPU Interconnect Subsystem www.ti.com
270 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.3.4 Interconnect Self-test
CPU Interconnect Subsystem canbeputintoself-test. When inself-test, theself-test logic willapply test
stimulus toeach master andslave interface. Ifanerror isdetected, thetype oferror forthecorresponding
interface islogged. Anerror isasserted toESM Group 3iftheself-test does notcomplete successfully.
NOTE: Application must only launch CPU Interconnect Subsystem self-test when there arenobus
transactions from anymasters including theCPU cores. While inself-test, theinterconnect
cannotservice anyrequests. Bus master requests canbelostorcorrupted. Itisrecommend
thattheself-test isonly exercised aspart ofthedevice initialization before anymaster is
setup bytheCPU.
Tolaunch theself-test, theapplicable must follow thebelow sequence:
1.Write 0xA keytotheDTC_ERROR_RESET bitsoftheSCMCNTRL register intheSCM module.
2.CPU executes WFI instruction toputitself inidlestate. The start ofself-test isgated bytheidlestate of
theCPU.
3.When both step 1and2aremet, theself-test willstart. While self-test ison-going, theCPU cores is
forced intoreset. Note thatreset isonly held totheCPU cores while therestofthesystem isnot.
4.When self-test iscomplete, theDTC_ERROR_RESET bitsisautomatically reverted back to0x5asthe
reset value.
5.After theself-test iscomplete, areset isapplied totheCPU Interconnect Subsystem for16HCLK
cycles. During thistime, theCPU isalso held inreset.
6.After theinterconnect andtheCPU comes outofthereset, normal code execution canthen start. CPU
cancheck theself-test status byreading theNT_OK bitandthePT_OK bitoftheSDC_STATUS
register. These twobitsindicate ifthenegative testandpositive self-test sequence have passed. In
addition, iftheself-test hasfailed, theerror isasserted totheESM module.
4.3.5 Interconnect Timeout
The CPU Interconnect Subsystem contains timeout counters tocount theamount oftime itistaking fora
master request tobeaccepted bytheslave andalso tocount theamount oftime ittakes from an
accepted request totheslave response. There aretwoseparate counters permaster interface. When
either therequest-to-accept counter ortheaccept-to-response counter expires bytheslave, atimeout
error isasserted totheESM. The counter threshold value beyond which thetimeout error willbe
generated isprogrammable intheSCM module. When atimeout happens toaninterface, therequest-to-
accept timeout error iscaptured intheSCM's SCMIAERR0STAT register andtheaccept-to-response
timeout error iscaptured intheSCMIAERR1STAT. See Table 4-4forthemapping between each interface
toeach bitfield. Application needs towrite 0xA keytotheTO_CLEAR bitsoftheSCMCNTRL register to
reset thetimeout logic inside theCPU Interconnect Subsystem aspart oftheerror handling intheISR.

<!-- Page 271 -->

www.ti.com CPU Interconnect Subsystem
271 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.3.6 Interconnect Runtime Status
Both theCPU Interconnect Subsystem andthePeripheral Interconnect Subsystem willoutput itsstatus on
each master andslave interface totheSCM indicating iftheinterface iscurrently active. The status are
captured intheSCMIASTAT register forthemaster interfaces andSCMTASTAT fortheslave interfaces.
See Table 4-4forthemapping between each interface toeach bitfield.
Table 4-4.SCM Register BitMapping
Register Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7 Remark
SCMIAERR0
STATPS_SCR
_MPOMDMA
Port AReserved ReservedCPU AXI-M
ReadCPU AXI-M
WriteACP-MEach bitindicates
thetransaction
processing block
inside the
interconnect
corresponding tothe
master thatis
detected bythe
interconnect checker
tohave afault.
Atimeout error
when thetime the
request isissued by
themaster until the
time therequest is
accepted bythe
slave hasexpired
SCMIAERR1
STATPS_SCR
_MPOMDMA
Port AReserved ReservedCPU AXI-M
ReadCPU AXI-M
WriteACP-MAtimeout error
when thetime the
request isaccepted
bytheslave until the
time therequest is
responded bythe
slave hasexpired
SCMIASTATPS_SCR
_MPOMDMA
Port AReserved ReservedCPU AXI-M
ReadCPU AXI-M
WriteACP-MEach bitindicates
thatthere isstill
pending transactions
forthe
corresponding
master tobe
processed bythe
interconnectBit8 Bit9 Bit10 Bit11 Bit12 Bit13
DMA
PortBHTUx /
FTUDAP/
DMMEthernet CPU PP-AXI Reserved
SCMTASTATL2RAML2Flash
Port BL2Flash
Port AEMIF Reserved CPU AXI-S ACP-SPS_SCR
_SEach bitindicates
thatthere isstill
pending transactions
forthe
corresponding slave
tobeprocessed by
theinterconnectBit8 Bit9 Bit10 Bit11 Bit12 Bit13
PCR1 PCR2 PCR3 CRC1 CRC2 SDC MMR

<!-- Page 272 -->

SDC MMR Registers www.ti.com
272 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4 SDC MMR Registers
Table 4-5lists theSafety Diagnostic Checker registers. The registers support only 32-bit reads. The offset
isrelative tothebase address. The base address fortheregisters isFA00 0000h.
Table 4-5.SDC MMR Registers
Offset Acronym Register Description Section
0h SDC_STATUS SDC Status Register Section 4.4.1
4h SDC_CONTROL SDC Control Register Section 4.4.2
8h ERR_GENERIC_PARITY Error Generic Parity Register Section 4.4.3
Ch ERR_UNEXPECTED_TRANS Error Unexpected Transaction Register Section 4.4.4
10h ERR_TRANS_ID Error Transaction IDRegister Section 4.4.5
14h ERR_TRANS_SIGNATURE Error Transaction Signature Register Section 4.4.6
18h ERR_TRANS_TYPE Error Transaction Type Register Section 4.4.7
1Ch ERR_USER_PARITY Error User Parity Register Section 4.4.8
20h SERR_UNEXPECTED_MID Slave Error Unexpected Master IDregister Section 4.4.9
24h SERR_ADDR_DECODE Slave Error Address Decode Register Section 4.4.10
28h SERR_USER_PARITY Slave Error User Parity Register Section 4.4.11

<!-- Page 273 -->

www.ti.com SDC MMR Registers
273 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.1 SDC Status Register (SDC_STATUS)
Figure 4-2.SDC Status Register (SDC_STATUS) (offset =00h)
31 16
Reserved
R-0
15 5 4 3 2 1 0
GLOBAL_ERROR NT_OK NT_RUN PT_OK PT_RUN
R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-6.SDC Status Register (SDC_STATUS) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0andwrites have noeffect.
4 GLOBAL_ERROR This bitindicates thatonesafety diagnostic checker hasasserted anerror input thatis
captured inerror logregisters located ataddress offset from 0x08 to0x28.
0 Noerror isdetected byanychecker.
1 Error isdetected byonechecker. Tofindoutthetype oferror from which checker, read the
error logregisters located ataddress offset from 0x08 to0x28.
3 NT_OK Negative testOKstatus forself-test.
0 Negative testhasfailed.
1 Negative testhaspassed.
2 NT_RUN Negative teston-going status.
0 Negative testhasended.
1 Negative testison-going.
1 PT_OK Positive testOKstatus forself-test.
0 Positive testhasfailed.
1 Positive testhaspassed.
0 PT_RUN Positive teston-going status.
0 Positive testhasended.
1 Positive testison-going.

<!-- Page 274 -->

SDC MMR Registers www.ti.com
274 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.2 SDC Control Register (SDC_CONTROL)
Figure 4-3.SDC Control Register (SDC_STATUS) (offset =04h)
31 16
Reserved
R-0
15 1 0
Reserved MASK_SOFT_RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 4-7.SDC Control Register (SDC_CONTROL) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0andwrites have noeffect.
0 MASK_SOFT_RESET This bitenables theself-test sequence tobelaunched bytheSCM (SCR Control Module)
module. You should always keep thisbitcleared.
0 Enable SCM tolaunch self-test ontheinterconnect.
1 Disable SCM tolaunch self-test ontheinterconnect.
4.4.3 Error Generic Parity Register (ERR_GENERIC_PARITY)
Figure 4-4.Error Generic Parity Register (ERR_GENERIC_PARITY) (offset =08h)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_GENERIC_PARITY
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-8.Error Generic Parity Register (ERR_GENERIC_PARITY) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_GENERIC_
PARITYError related toparity mismatch inthehigher order bitsoftheincoming address getting transmitted
across interconnect incorrectly. When set,each bitindicates thetransaction processing block inside
theinterconnect corresponding tothemaster isdetected bytheinterconnect checker tohave a
fault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master

<!-- Page 275 -->

www.ti.com SDC MMR Registers
275 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.4 Error Unexpected Transaction Register (ERR_UNEXPECTED_TRANS)
Figure 4-5.Error Unexpected Transaction Register (ERR_UNEXPECTED_TRANS) (offset =0Ch)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_UNEXPECTED_TRANS
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-9.Error Unexpected Transaction Register (ERR_UNEXPECTED_TRANS) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_UNEXPECTED_TRANS Error related tounexpected transaction sent bythemaster. When set,each bit
indicates thetransaction processing block inside theinterconnect corresponding to
themaster isdetected bytheinterconnect checker tohave afault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master
4.4.5 Error Transaction IDRegister (ERR_TRANS_ID)
Figure 4-6.Error Transaction IDRegister (ERR_TRANS_ID) (offset =10h)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_TRANS_ID
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-10. Error Transaction IDRegister (ERR_TRANS_ID) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_TRANS_ID Error related tomismatch onthetransaction ID.When set,each bitindicates thetransaction
processing block inside theinterconnect corresponding tothemaster isdetected bythe
interconnect checker tohave afault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master

<!-- Page 276 -->

SDC MMR Registers www.ti.com
276 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.6 Error Transaction Signature Register (ERR_TRANS_SIGNATURE)
Figure 4-7.Error Transaction Signature Register (ERR_TRANS_SIGNATURE) (offset =14h)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_TRANS_SIGNATURE
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-11. Error Transaction Signature Register (ERR_TRANS_SIGNATURE) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_TRANS_
SIGNATUREError related tomismatch onthetransaction signature. The transaction signature isavalue
computed using thelower bitsoftheaddress, number ofbytes andthebyte enables ofthe
transaction. The signature calculated bythemaster issent tothedecoded slave where the
signature iscomputed again andcompared totheoriginal signature. When set,each bitindicates
thetransaction processing block inside theinterconnect corresponding tothemaster isdetected by
theinterconnect checker tohave afault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master
4.4.7 Error Transaction Type Register (ERR_TRANS_TYPE)
Figure 4-8.Error Transaction Type Register (ERR_TRANS_TYPE) (offset =18h)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_TRANS_TYPE
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-12. Error Transaction Type Register (ERR_TRANS_TYPE) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_TRANS_TYPE Error related tomismatch onthetransaction type. When set,each bitindicates thetransaction
processing block inside theinterconnect corresponding tothemaster isdetected bythe
interconnect checker tohave afault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master

<!-- Page 277 -->

www.ti.com SDC MMR Registers
277 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.8 Error User Parity Register (ERR_USER_PARITY)
Figure 4-9.Error User Parity Register (ERR_USER_PARITY) (offset =1Ch)
31 16
Reserved
R-0
15 6 5 0
Reserved ERR_USER_PARITY
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-13. Error User Parity Register (ERR_USER_PARITY) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0andwrites have noeffect.
5-0 ERR_USER_PARITY Error related tomismatch ontheparity. When set,each bitindicates thetransaction processing
block inside theinterconnect corresponding tothemaster isdetected bytheinterconnect
checker tohave afault.
bit0:PS_SCR_M master
bit1:POM master
bit2:DMA PortA master
bit3:Reserved
bit4:Cortex-R5F CPU master.
bit5:ACP-M master
4.4.9 Slave Error Unexpected Master IDRegister (SERR_UNEXPECTED_MID)
Figure 4-10. Slave Error Unexpected Master IDRegister (SERR_UNEXPECTED_MID) (offset =20h)
31 16
Reserved
R-0
15 7 6 0
Reserved SERR_UNEXPECTED_MID
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-14. Slave Error Unexpected Master IDRegister (SERR_UNEXPECTED_MID)
Field Descriptions
Bit Field Value Description
31-7 Reserved 0 Reads return 0andwrites have noeffect.
6-0 SERR_UNEXPECTED_MID Error related tomismatch onthemaster ID.When set,each bitindicates the
transaction processing block inside theinterconnect corresponding totheslave that
isdetected bytheinterconnect checker tohave afault.
bit0:L2SRAM slave
bit1:L2Flash PortB slave
bit2:L2Flash PortA slave
bit3:EMIF slave
bit4:Reserved
bit5:Cortex-R5F CPU AXIslave
bit6:ACP-S slave

<!-- Page 278 -->

SDC MMR Registers www.ti.com
278 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedInterconnect4.4.10 Slave Error Address Decode Register (SERR_ADDR_DECODE)
Figure 4-11. Slave Error Address Decode Register (SERR_ADDR_DECODE) (offset =24h)
31 16
Reserved
R-0
15 7 6 0
Reserved SERR_ADDR_DECODE
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-15. Slave Error Address Decode Register (SERR_ADDR_DECODED) Field Descriptions
Bit Field Value Description
31-7 Reserved 0 Reads return 0andwrites have noeffect.
6-0 SERR_ADDR_ DECODE Error related tomismatch onthemost-significant address bits. When set,each bit
indicates thetransaction processing block inside theinterconnect corresponding to
theslave thatisdetected bytheinterconnect checker tohave afault.
bit0:L2SRAM slave
bit1:L2Flash PortB slave
bit2:L2Flash PortA slave
bit3:EMIF slave
bit4:Reserved
bit5:Cortex-R5F CPU AXIslave
bit6:ACP-S slave
4.4.11 Slave Error User Parity Register (SERR_USER_PARITY)
Figure 4-12. Slave Error User Parity Register (SERR_USER_PARITY) (offset =28h)
31 16
Reserved
R-0
15 7 6 0
Reserved SERR_USER_PARITY
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 4-16. Slave Error User Parity Register (SERR_USER_PARITYID) Field Descriptions
Bit Field Value Description
31-7 Reserved 0 Reads return 0andwrites have noeffect.
6-0 SERR_USER_PARITY Error related tomismatch ontheparity ontheresponse signals fortheslave. When
set,each bitindicates thetransaction processing block inside theinterconnect
corresponding totheslave thatisdetected bytheinterconnect checker tohave a
fault.
bit0:L2SRAM slave
bit1:L2Flash PortB slave
bit2:L2Flash PortA slave
bit3:EMIF slave
bit4:Reserved
bit5:Cortex-R5F CPU AXIslave
bit6:ACP-S slave