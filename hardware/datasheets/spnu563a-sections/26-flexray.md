# FlexRay Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1210-1416

---


<!-- Page 1210 -->

1210 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleChapter 26
SPNU563A -March 2018
FlexRay Module
This chapter provides thespecification forTI'sFlexRay module and itsfeatures from theapplication
programmer 'spoint ofview.
Topic ........................................................................................................................... Page
26.1 Overview ........................................................................................................ 1211
26.2 Module Operation ........................................................................................... 1215
26.3 FlexRay Module Registers ............................................................................... 1277

<!-- Page 1211 -->

www.ti.com Overview
1211 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.1 Overview
The FlexRay module performs communication according totheFlexRay protocol specification v2.1 Rev.
A.The sample clock bitrate canbeprogrammed tovalues upto10Mbit/s. Additional busdriver (BD)
hardware isrequired forconnection tothephysical layer.
Forcommunication onaFlexRay network, individual message buffers with upto254data bytes are
configurable. The message storage consists ofasingle-ported message RAM thatholds upto128
message buffers. Allfunctions concerning thehandling ofmessages areimplemented inthemessage
handler. Those functions aretheacceptance filtering, thetransfer ofmessages between thetwoFlexRay
Channel Protocol Controllers andthemessage RAM, maintaining thetransmission schedule aswell as
providing message status information.
The register setoftheFlexRay module canbeaccessed directly bytheCPU viatheVBUS interface.
These registers areused tocontrol, configure andmonitor theFlexRay channel protocol controllers,
message handler, global time unit, system universal control, frame andsymbol processing, network
management, interrupt control, andtoaccess themessage RAM viatheinput /output buffer.
26.1.1 Feature List
*Conformance with FlexRay protocol specification v2.1 Rev. A
*Data rates ofupto10Mbit/s oneach channel
*Upto128message buffers
*8Kbyte ofmessage RAM forstorage of,forexample, 128message buffers with maximum of48-byte
data section orupto30message buffers with 254-byte data section
*Configuration ofmessage buffers with different payload lengths
*One configurable receive FIFO
*Each message buffer canbeconfigured asreceive buffer, astransmit buffer oraspart ofthereceive
FIFO
*CPU access tomessage buffers viainput andoutput buffer
*Specialized DMA likeFlexRay Transfer Unit (FTU) forautomatic data transfer between data memory
andmessage buffers without CPU interaction
*Filtering forslotcounter, cycle counter, andchannel
*Maskable module interrupts
*Supports Network Management
26.1.2 FlexRay Module Block Diagram
The TIFlexRay module, Figure 26-1,contains thefollowing blocks:
*Peripheral Interface (VBUS IF)
Interface tothePeripheral Bus oftheTMS570 microcontroller architecture. The FlexRay module can
either actasaVBUS master orVBUS slave.
*FlexRay Transfer Unit (FTU)
The internal intelligent state-machine (Transfer Unit State Machine) isable totransfer data between
theinput buffer (IBF) andoutput buffer (OBF) ofthecommunication controller andthesystem memory
without CPU interaction.
NOTE: Since theFlexRay module isaccessed through theFTU, theFTU must bepowered upby
thecorresponding bitinthePeripheral Power Down Registers oftheSystem Module before
accessing anyFlexRay module register. Fordetails, refer totheArchitecture chapter andthe
device-specific data manual.

<!-- Page 1212 -->

Message HandlerRx_A
Tx_A
ControlPRT A
PRT BFlexRay Module
NEM
Message RAMTBF A
GTU
TBF B SUC
IBF
OBFPhysical
Layer
uCRx_B
Tx_B
INTFSP
80MHzInterruptsVBUS IF
PeripheralFTU
SCLK
VBUSCLK(Master)
VBUS IF(Slave) Direct
AccessBus
BCLKTransfer UnitStatemachine
Overview www.ti.com
1212 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-1. FlexRay Module Block Diagram
*Input Buffer (IBF)
Forwrite access tothemessage buffers configured inthemessage RAM, theCPU ortheFTU can
write theheader anddata section foraspecific message buffer totheinput buffer. The message
handler then transfers thedata from theinput buffer totheselected message buffer inthemessage
RAM.
*Output Buffer (OBF)
Forread access toamessage buffer configured inthemessage RAM themessage handler transfers
theselected message buffer totheoutput buffer. After thetransfer hascompleted, theCPU ortheFTU
canread theheader anddata section ofthetransferred message buffer from theoutput buffer.
*Message Handler (MHD)
The message handler controls data transfers between thefollowing components:
-Input /output buffer andmessage RAM
-Transient buffer RAMs ofthetwoFlexRay protocol controllers andmessage RAM
*Message RAM
The message RAM stores upto128FlexRay message buffers together with therelated configuration
data (header anddata partition).
*The Transient Buffer RAM (TBF A/B):
Stores thedata section oftwocomplete messages.

<!-- Page 1213 -->

www.ti.com Overview
1213 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module*FlexRay Channel Protocol Controller (PRT A/B)
The FlexRay channel protocol controllers consist ofashift register andtheFlexRay protocol FSM
(Finite State Machine). They areconnected tothetransient buffer RAMs forintermediate message
storage andtothephysical layer viabusdrivers (BD).
They perform thefollowing functionality:
-Control andcheck ofbittiming
-Reception /transmission ofFlexRay frames andsymbols
-Check ofheader CRC
-Generation /check offrame CRC
-Interfacing tobusdriver
The FlexRay channel protocol controllers have interfaces to:
-Physical layer (bus driver)
-Transient buffer RAM
-Message handler
-Global Time Unit
-System universal control
-Frame andsymbol processing
-Network management
-Interrupt control
*Global time unit(GTU)
The GTU performs thefollowing functions:
-Generation ofmicrotick
-Generation ofmacrotick
-Fault tolerant clock synchronization byFTM algorithm
*rate andoffset correction
*offset correction
-Cycle counter
-Timing control ofstatic segment
-Timing control ofdynamic segment (minislotting)
-Support ofexternal clock correction
*System Universal Control (SUC)
The SUC controls thefollowing functions:
-Configuration
-Wakeup
-Startup
-Normal Operation
-Passive Operation
-Monitor Mode
*Frame andSymbol Processing (FSP)
The frame andsymbol processing controls thefollowing functions:
-Checks thecorrect timing offrames andsymbols
-Tests thesyntactical andsemantic correctness ofreceived frames
-Sets theslotstatus flags
*Network Management (NEM)
Handles thenetwork management vector.

<!-- Page 1214 -->

Communication
ControllerTransfer UnitFlexRay Module
base_TUbase_TU_RAM
base_CCRegister Set
Register SetRAM
offset_CCoffset_TUoffset_TU_RAM
Overview www.ti.com
1214 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module*Interrupt Control (INT)
The interrupt controller performs thefollowing functions:
-Provides error andstatus interrupt flags
-Enable /disable interrupt sources
-Assignment ofinterrupt sources tothetwomodule interrupt lines
-Enable /disable module interrupt lines
-Manages thetwointerrupt timers
-Stop watch time capturing
*80-MHz Clock Signal
NOTE: VCLKA2 isused toprovide the80-MHz clock totheFlexRay Module. The second PLL /
Clock Source 6inthemicrocontroller istypically used assource forVCLKA2.
Clock signal forthesample clock (SCLK) oftheFlexRay module.
*Module Clock (VBUSCLK)
The FlexRay module clock (BCLK) isderived from thePeripheral Clock VBUSCLKofthemicrocontroller.
26.1.3 FlexRay Module Blocks
Figure 26-2 shows thedifferent module blocks oftheFlexRay module: thecommunication controller, the
transfer unit, andthetransfer unitRAM. The RAM ofthecommunication controller isonly memory-
mapped intestmode, where itismapped totheregister setaddress range. The address ranges ofthe
three FlexRay blocks areshown inTable 26-1.
Figure 26-2. FlexRay Module Blocks

<!-- Page 1215 -->

www.ti.com Module Operation
1215 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-1. FlexRay Address Range Table
Module Address Range
FlexRay Communication Controller 0xFFF7_C800 -0xFFF7_CFFF
FlexRay TU 0xFFF7_A000 -0xFFF7_A1FF
FlexRay TURAM 0xFF50_0000 -0xFF51_FFFF
26.2 Module Operation
26.2.1 Transfer Unit
The FlexRay Transfer Unit (FTU), Figure 26-3,hasaninternal intelligent state-machine (Transfer Unit
State Machine) totransfer data between theInput andOutput Buffer Interfaces oftheFlexRay core
module andthesystem memory ofthemicrocontroller without CPU interaction. Itoperates inasimilar
manner toaDMA (Direct Memory Access) module.
The FlexRay Input Buffer (IBF) andFlexRay Output Buffer (OBF) canalso beaccessed directly bythe
CPU. Inthiscase theIBFandOBF are8-,16-, and32-bit accessible. Fortransfers using theTransfer Unit
State Machine only 4×32-bit data packages (4word bursts) aresupported.
The Interface Arbiter controls theaccess totheIBFandOBF. Direct CPU accesses toIBFandOBF are
notpossible, iftheTransfer Unit State Machine isswitched on.Accesses willbeignored andthe
associated error interrupt willbegenerated.
The Transfer Unit State Machine isthehead ofallmanual, event driven andautomatic message transfer
activities. Itcontrols theTransfer Unit interrupt generation related totransfer protocol correctness, status
andviolations ofthemessage transfers.
With theTransfer Configuration RAM (TCR) thetransfer sequence, executed bytheTransfer Unit State
Machine, canbeconfigured.
The usage oftheTransfer Unit allows theuser tosetup amirror oftheFlexRay message RAM inthefast
accessible data RAM ofthemicrocontroller. The Transfer Unit canhandle thedata transfers between the
data RAM andtheFlexRay message RAM inthe'background 'without CPU interaction.

<!-- Page 1216 -->

VBUSP
Interface
Peripheral Bus
Transfer Unit InterruptsTransfer Unit
Slave
VBUSP
InterfaceMasterTransfer
Unit
State
MachineInterface
Arbiter
Transfer
Configuration
RAM (TCR)HandlerMessageFlexRayFlexRay
Input Buffer
FlexRay
Output Buffer(IBF)
(OBF)
Module Operation www.ti.com
1216 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-3. Transfer Unit

<!-- Page 1217 -->

Transfer Base
Address ( TBA)FlexRay
Message RAM
Message Buffer 1
Message Buffer 2
Message Buffer 3
Message Buffer 4
Message Buffer 128FTUData RAM
Header / Data
Message Buffer 4+14 bit offsetTCR 1
TCR 2
TCR 3
TCR 4
TCR 128Transfer
Configuration RAMTransfer Base
Address ( TBA)FTU
++14 bit offsetTCR 1
TCR 2
TCR 3
TCR 4
TCR 128Transfer
Configuration RAM
www.ti.com Module Operation
1217 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1 Transfer Unit Functional Description
Figure 26-4 shows theprinciple oftheTransfer Unit operation.
Each FlexRay message buffer oftheFlexRay message buffer RAM hasoneTransfer Configuration RAM
(TCR) entry assigned toit,thatis,message buffer 1isassigned toTCR1, message buffer 2isassigned to
TCR2, andsoon.
The Transfer Base Address (TBA) register oftheTransfer Unit holds themessage buffer base address in
thedata RAM. Each Transfer Configuration RAM (TCR) entry contains a14bitoffset value tothe
dedicated message buffer area inthedata RAM.
Figure 26-4. FlexRay Transfer Unit Operation Principle
The following twodiagrams show theprinciple oftheTransfer Unit operation including Transfer State
Machine (see Figure 26-5)andEvent State Machine (see Figure 26-6 ).

<!-- Page 1218 -->

Module reset active
or FTU disabled
(GCS.TUE=0)
FTU disabled
(GCS.TUE=0)Wait for FTU being enabled
(GCS.TUE=1, GCS.TUH=0)
Find lowest bit set in TTSM and TTCC
which corresponds to the next message
buffer to be transferred
Set up FTU transfer of the message buffer
with help of configuration in Transfer
Configuration RAM (TCR)
4 word burst by 4 word burst transfer of
message buffer to System Memory (SM)
or to Communication Controller (CC)
Reset bit in TTSM or TTCC which
corresponds to the transferred message
buffer and generate status informationSTATUSIDLE
CHECK
SETUP
XFER
Module Operation www.ti.com
1218 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-5. FlexRay Transfer Unit Operation Principle forTransfer FSM (simplified)

<!-- Page 1219 -->

Module reset active
or FTU disabled
(GCS.TUE=0)
FTU disabled
(GCS.TUE=0)Wait for FTU being enabled
(GCS.TUE=1)
Wait for event signaled from
E-Ray that a message buffer
has been updated
Set up FTU transfer of the message buffer
with help of configuration in Transfer
Configuration RAM (TCR)IDLE
WAIT
UPDATE
ETESMS/R CESMS/RSet bit
TTSMClear bit
ETESMS/R
0
1
1-
0
1False
True
TrueFalse
False
TrueEvent FSM
www.ti.com Module Operation
1219 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-6. FlexRay Transfer Unit Operation Principle forEvent FSM (simplified)
26.2.1.1.1 Transfer Control
26.2.1.1.1.1 Transfer Start andHalt
The Transfer Unit State Machine canbehalted, effectively stopping theTransfer Unit transfer sequence
(after completion ofthecurrent 4word burst transfer cycle). After releasing from haltstate, theTransfer
Unit resumes exactly, where itwas halted without data loss.
NOTE: Itisthesoftware 'sresponsibility toensure data coherency when theFlexRay module
continues toreceive data, buttheTransfer Unit doesn't transfer it.
26.2.1.1.1.2 Transfer Abort
ATransfer Unit transfer willbeaborted andtheTransfer Unit willbedisabled automatically incase of:
*anECC multi-bit error while accessing theTransfer Configuration RAM (TCR)
*anuncorrected biterror while accessing theTransfer Configuration RAM (TCR), when ECC single-bit
error correction isdisabled
*amemory protection error while accessing thedata RAM ofthemicrocontroller. Inthiscase, the
ongoing transfer isaborted buttheTUE bitinGCS/R may notgetreset. User shall clear theTUE bit
manually bysoftware.

<!-- Page 1220 -->

Module Operation www.ti.com
1220 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.1.3 Transfer Reset
The Transfer Unit State Machine canbereset bytheTransfer Unit Enable (TUE) bitintheGlobal Control
register. Though theTransfer Unit State Machine canbereset with theabove, themodule register
contents andtheTransfer Configuration RAM (TCR). So,after re-enabling theTransfer Unit no
reconfiguration oftheTransfer Unit isrequired.
26.2.1.1.1.4 Transfer Modes
Possible transfer sequence modes are:
*Manual bytriggering thedesired transfer bysetting thecorresponding bitintheTrigger Transfer to
System Memory (TTSM) register ortheTrigger Transfer toCommunication Controller (TTCC) register
*Event-Driven (transfers from FlexRay Communication Controller totheSystem Memory only) using the
Enable Transfer onEvent toSystem Memory (ETESM) register.
*Single orcontinuous event driven transfers byusing theClear onEvent toSystem Memory (CESM)
The transfer event trigger ingeneral occurs upon completion ofareception ortransmission ofaframe
through theFlexRay bus. Table 26-2 shows more details: Conditions marked with 'X'perrowmust match
totrigger aFTU transfer event asconfigured intheTransfer Configuration RAM (TCR):
Table 26-2. FlexRay Transfer Unit Event Trigger Conditions
Event on
Channel AEvent on
Channel BFrame belonging
tostatic segment
orfirst slotof
dynamic segmentFrame belonging
todynamic
segment, except
first slotof
dynamic segmentBus activity
detected on
Channel A
(MBS.ESA =0)Bus activity
detected on
Channel B
(MBS.ESB =0)
FTU Event Trigger
forReceive
Message BuffersX X
X X
X X X
X X X
FTU Event Trigger
forTransmit
Message BuffersX X
X X
NOTE: Bysetting thecorresponding bitintheEnable Transfer onEvent toSystem Memory
(ETESM) register prior toanon-demand transfer totheCommunication Controller byway of
theTrigger Transfer toCommunication Controller (TTCC) register, anevent-triggered
transmission back totheSystem Memory canbeinitiated, once thebuffer hasbeen sent out
ontheFlexRay bus. This mechanism canbeused, forinstance, toautomatically read back
theheader status information tothesystem memory after atransmission occurred.
The transmission orreception ofnullframes inthestatic segment ofaFlexRay
communication cycle triggers transfers ofthetransfer unit. The header and/or payload is
transferred tothesystem memory, ifthecorresponding bitsTHTSM and/or TPTSM inthe
Transfer Configuration RAM (TCR) areset.Ifneither THTSM norTPTSM bitissetinTCR,
neither header norpayload gets transferred. The corresponding bitintheTransfer toSystem
Memory Occurred register (TSMO) gets setinallcases.

<!-- Page 1221 -->

Message Ram Data RAM
word x
word x+1
word x+2
word x+3
word x+4
word x+5word x
word x+1
word x+2
word x+3
word x+4
word x+5
undefined x
undefined x+1
undefined x+2word x
word x+1
word x+2
word x+3
word x+4
word x+5
undefined x
undefined x+1
undefined x+24 word burst4 word burstFTU Transferinternal E-R ay
TransferInput Buffer
Registers
Message RAM Data RAM
word x
word x+1
word x+2
word x+3
word x+4
word x+5
word x+6
word x+7word x
word x+1
word x+2
word x+3
word x+4
word x+5
undefined x
undefined x+1
undefined x+2word x
word x+1
word x+2
word x+3
word x+4
word x+5
undefined x
undefined x+1
undefined x+24 word burstFTU Transferinternal E-Ray
TransferOutput Buffer
Registers
4 word burst
www.ti.com Module Operation
1221 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.1.5 Transfer Size andTypes
The data transferred bytheTransfer Unit canbeselected as:
*data andheader section
*header section only
*data section only
The number oftransferred payload words isderived from thePayload Length Configured (PLC)
information configured intheWrite Header Section 2(WRHS2) register.
Asonly 4word bursts aresupported fortheTransfer Unit transfers, only multiple of4x32-bit data packets
aresupported. Additional transferred words areundefined, asindicated inFigure 26-7 andFigure 26-8.
Figure 26-7. Example: FTU Read Transfer of6Words
Figure 26-8. Example: FTU Write Transfer of6Words
Physically theFTU continues reading theadditional words from thesource location itstarted theburst
transfer. Therefore, onreads, theadditional transferred words depend onthecontents ofthe
Communication Controller Output Buffer Registers asindicated inFigure 26-7.Onwrites theadditional
words depend onthecontents ofthedata RAM, asshown inFigure 26-8.The additional data willbe
written totheCommunication Controller's Input Buffer Registers, butnottransferred tothemessage RAM.
NOTE: Itshould beensured thattheallocated data RAM space forFTU transfers ends on4x32 bit
boundary toavoid possible data overwrites ormemory protection issues onFTU reads and
avoid reading theadditional data from thesource location onFTU writes.

<!-- Page 1222 -->

Module Operation www.ti.com
1222 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.1.6 Transfer Status Indication
There are3registers indicating thetransfer status:
*Transfer Status Current Buffer (TSCB) shows thecurrent transfer buffer status
*Last Transferred Buffer toCommunication Controller (LTBCC) indicates thelastcompleted buffer
transfer tothecommunication controller
*Last Transferred Buffer toSystem Memory (LTBSM) shows thelastcompleted buffer transfer to
system memory
26.2.1.1.1.7 Transfer Mirror Function
Inorder toefficiently access thetransfer unitstatus registers inthesystem memory, thefollowing registers
canbemirrored tothesystem memory starting atthebase address defined intheBase Address of
Mirrored Status (BAMS) register:
*Transfer Status Current Buffer (TSCB)
*Last Transferred Buffer toCommunication Controller (LTBCC)
*Last Transferred Buffer toSystem Memory (LTBSM)
*Transfer toSystem Memory Occurred 1/2/3/4 (TSMO1-4)
*Transfer toCommunication Controller Occurred 1/2/3/4 (TCCO1-4)
*Transfer Occurred OFFset (TOOFF)
The mirrored values areupdated after completion ofabuffer transfer.
The mirroring ofthese registers canbedisabled ifnotneeded.
Table 26-3. Mirroring Address Mapping
Address Register
BAMS+0x00 TSCB
BAMS+0x04 LTBCC
BAMS+0x08 LTBSM
BAMS+0x0C TSMO1
BAMS+0x10 TSMO2
BAMS+0x14 TSMO3
BAMS+0x18 TSMO4
BAMS+0x1C TCCO1
BAMS+0x20 TCCO2
BAMS+0x24 TCCO3
BAMS+0x28 TCCO4
BAMS+0x2C TOOFF
26.2.1.1.1.8 Endianness Correction
Forthedata transfer bytheTransfer Unit anEndianness correction mechanism canbeused toswitch big
Endianness data tolittle Endianness data andvice versa.
Formaximum flexibility, 6bitsareavailable intheGlobal Control Set/Reset Register (GCS/R) tocontrol.
*Header Data byte-order
*Payload Data byte-order
*Byte-order oftheFlexRay Core registers andtheTransfer Configuration RAM data oftheTransfer Unit
independently andinboth directions.

<!-- Page 1223 -->

Buffer Addr
TSOTBA NTBA =
+communication
cycle start
www.ti.com Module Operation
1223 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.1.9 Transfer Data Package
Table 26-4 shows thedata ofatransfer data package. Independent ofwhether theheader gets
transferred ornot,thebuffer address always points toelement Header1.
(1)Transferred only from Communication Controller toSystem MemoryTable 26-4. Mirroring Address Mapping
Address Register
0x0000 Header1
0x0004 Header2
0x0008 Header3
0x000C Buffer Status(1)
0x0010 Payload1
0x0014 Payload2
: :
0x010C Payload64
26.2.1.1.1.10 Transfer Start Address toMessage Buffer Number Assignment
The assignment ofaFlexRay message buffer number tothetransfer location insystem memory isdone
bythecombination of:
*theTransfer Start Offset (TSO) field inaTransfer Configuration RAM (TCR) entry
*theTransfer Base Address (TBA) register
Each entry oftheTCR holds a14bitoffset value (TSO). The TSO offset willbeadded tothecontent of
theTBA register. The TBA register holds the32bit base address-pointer toalocation ofthedata RAM.
Avalue written toNext Transfer Base Address (NTBA) willbeloaded intheTBA atthenext
communication cycle start. This allows efficient multi-buffering ofthemessage buffers inthesystem
memory. The Transfer NotReady (NTR) flagintheTransfer Error Interrupt Flag (TEIF) register canbe
used todetermine, ifNTBA canbereloaded bytheCPU.
NOTE: Ifavalue iswritten toTBA, NTBA issettothesame value.
Figure 26-9. Transfer Start Address toMessage Buffer Number Assignment
26.2.1.1.1.11 Transfer Priority
The Transfer Unit willtransfer themessage buffers from lowtohigh message buffer numbers.
Incase thesame buffer ispending inboth theTrigger Transfer toCommunication (TTCC) register andthe
Trigger Transfer toSystem Memory (TTSM) register, thepriority between TTCC andTTSM isdetermined
bytheTransfer Priority bit(GC.PRIO) intheTransfer Unit Global Control Set/Reset Register (GCS/R).

<!-- Page 1224 -->

Module Operation www.ti.com
1224 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.1.12 Read Transfers
Aread transfer isthedata transfer from FlexRay message buffer RAM tothesystem memory ofthe
microcontroller.
Forread transfers theregisters Trigger Transfer toSystem Memory (TTSM), Enable Transfer onEvent to
System Memory (ETESM) andClear onEvent toSystem Memory (CESM) have tobesetup.
The amount andtype ofdata tobetransferred canbeselected as:
*data andheader section
*header section only
*data section only
which canbeconfigured ontheTransmit Configuration RAM (TCR).
The number of32bitwords perbuffer tobetransferred isread from thePayload Length Configured
(RDHS2.PLC) configuration information. This information ispart oftheheader section stored inthe
message RAM oftheCommunication Controller.
26.2.1.1.1.13 Write Transfers
Awrite transfer isthedata transfer from thesystem memory ofthemicrocontroller totheFlexRay
message buffer RAM.
Forwrite transfers theTrigger Transfer toCommunication (TTCC) register hastobesetup.
The amount andtype ofdata transferred canbeselected as:
*data andheader section
*header section only
*data section only
which canbeconfigured ontheTransmit Configuration RAM (TCR).
Itcanbeconfigured intheTCR, ifSetTransmission Request Host (STXRH) bitintheInput Buffer
Command Mask (IBCM) oftheCommunication Controller should beset.This would trigger thetransfer to
theFlexRay bus.
Ifadata andheader section transfer isselected, thenumber of32bitwords tobetransferred isread from
thePayload Length Configured (PLC) configuration information stored inHeader2 word inthesystem
memory.
Ifadata section only transfer isselected, thenumber of32bitwords tobetransferred isread from the
Payload Length Configured (RDHS2.PLC) configuration information. This information ispart oftheheader
section stored inthemessage RAM oftheCommunication Controller.
26.2.1.1.1.14 Transfer Unit Event Interface
The Transfer Unit Event Control generates transfer trigger signals fortransfers inthefollowing cases:
*Fortransmit (TX) message buffers, awrite transfer trigger isgenerated, ifatransmit event occurs. The
configured TXmessage buffers generate atransfer trigger, except when aNullframe instatic segment
ornoframe inthedynamic segment issent.
*Forreceive (RX) message buffers, aread transfer trigger isgenerated, ifareceive event occurs inthe
static segment.
*Forreceive (RX) message buffers, aread transfer trigger isgenerated ifareceive event occurs inthe
dynamic segment, updated inthecurrent cycle andnoNullframe.
Ifabuffer ispart oftheFIFO, notransfer trigger isgenerated!
When theTransfer Unit isdisabled (TUE bitinGlobal Control Register (GCS/R) is0),notransfer trigger is
generated, whereas iftheTransfer Unit isenabled, butinhaltmode (TUH bitandTUE bitinGlobal
Control Register (GCS/R) are1),theoccurring triggers remain pending andgetexecuted when the
Transfer Unit willberesumed from haltmode.

<!-- Page 1225 -->

www.ti.com Module Operation
1225 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.1.1.2 Transfer Configuration RAM
The Transfer Configuration RAM (TCR) consists of128entries, oneentry foreach possible FlexRay
buffer. Entry 1isassigned toFlexRay buffer 1,entry 2toFlexRay buffer 2,..., andentry 128isassigned to
FlexRay buffer 128.
Each TCR entry defines:
*data transfer size (header +data, header only ordata only)
*whether thetransmit request flag(STRXH) should besetforthedata transferred bytheFTU totheCC
tosend outthedata totheFlexRay bus.
*the14-bit buffer address offset, which, incombination with theTransfer Base Address defined inTBA,
specifies thestart ofthecorresponding FlexRay message buffer inthesystem memory RAM.
NOTE: Itisrecommended toclear thewhole TCR before configuring it,inorder toavoid unexpected
transfer behavior duetooldconfiguration contents orrandom TCR RAM contents after
power onreset.
Ifatransfer istriggered butnotransfer size (header ordata) issetup intheTCR, nodata will
betransferred, butthecorresponding flagintheTransfer toCommunication Controller
Occurred (TCCOx) ortheTransfer toSystem Memory Occurred (TSMOx) willbeset.
26.2.1.1.2.1 ECC Protection
The Transfer Configuration RAM (TCR) isECC protected. The ECC multi-bit error interrupt generation is
disabled bydefault andcanbeswitched onbywriting a4bitkeytodedicated ECC lock bitsintheGlobal
Control Set/Reset Register (GCS/R).
The ECC protection supports single-bit error correction anddouble-bit error detection mechanism
(SECDED). The ECC information isstored together with thecorresponding 19-bit data word entry.
The ECC ischecked each time adata word isread from theTCR RAM. IfanECC error isdetected, the
PEerror flagissetintheTransfer Error Interrupt Flag (TEIF) register. The detection ofanECC single-bit
error willbeindicated bytheSEflagintheTCR Single-Bit Error Status (TSBESTAT).
Additionally anuncorrectable RAM error interrupt/event willbegenerated. The uncorrectable RAM error
interrupt/event isnonmaskable andtherefore cannot beswitched off.ForECC single-bit errors, the
uncorrectable RAM error interrupt/event isgenerated, iftheECC single-bit error correction isdisabled.
The uncorrectable RAM error ishooked uptotheESM module (event).
The faulty TCR RAM address canberead from theECC Error Address (PEADR) register. Equivalent
information isavailable forECC single-bit errors intheTCR Single-Bit Error Status (TSBESTAT) register.
Independent oftheECC single-bit error correction being enabled ordisabled, theTSBESTAT isupdated.
See Figure 26-26 formore details.
26.2.1.1.3 Memory Protection Mechanism
This feature allows torestrict accesses tocertain areas inmemory inorder toprotect critical application
data from unintentionally being accessed bytheTransfer Unit State Machine.
One memory section (start andendaddress) canbedefined, which allows read andwrite accesses forthe
Transfer Unit State Machine.
Iftheendaddress issmaller orequal tothestart address, data transfers willbeblocked. Any accesses
performed outside thismemory area bytheTransfer Unit State Machine result innotransfers being
performed. Incase ofaprotection violation aflagwillbesetandtheMemory Protection Violation interrupt
willbeactivated. The Transfer Unit State Machine willbedisabled inthiscase.
The default setting oftheTransfer Unit State Machine memory protection address range setup is
0x00000000 forstart address and0x00000000 forendaddress.
This means avalid address range must besetup, before theTransfer Unit canbeused.

<!-- Page 1226 -->

Static segmentSymbol
windowNIT
Communication cycle xCommunication
cycle x-1Communication
cycle x+1Time base
derived trigger
tTime base
derived trigger
Dynamic
segmentStatic segment Dynamic segment
Module Operation www.ti.com
1226 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.2 Communication Cycle
Acommunication cycle inFlexRay (Figure 26-10 )consists ofthefollowing elements:
*Static segment
*Dynamic segment
*Symbol window
*Network idletime (NIT)
Static segment, dynamic segment, andsymbol window form thenetwork communication time (NCT). For
each communication channel theslotcounter starts at1andcounts upuntil theendofthedynamic
segment isreached. Both channels share thesame arbitration gridwhich means thatthey usethesame
synchronized macrotick.
Figure 26-10. Structure ofCommunication Cycle
26.2.2.1 Static Segment
The Static Segment ischaracterized bythefollowing features:
*Time slots offixed length (optionally protected bybusguardian)
*Start offrame transmission ataction point ofthecorresponding static slot
*Payload length same forallframes onboth channels
Parameters: number ofstatic slots GTUC7.NSS(9-0), static slotlength GTUC7.SSL(9-0), Payload Length
Static MHDC.SFDL(6-0), action point offset GTUC9.APO(5-0)
26.2.2.2 Dynamic Segment
The Dynamic Segment ischaracterized bythefollowing features:
*Allcontrollers have busaccess (nobusguardian protection possible)
*Variable payload length andduration ofslots, different forboth channels
*Start oftransmission atminislot action point
Parameters: number ofminislots GTUC8.NMS(12-0), minislot length GTUC8.MSL(5-0), minislot action
point offset GTUC9.MAPO(4-0), start oflatest transmit (last minislot) MHDC.SLT(12-0)
26.2.2.3 Symbol Window
During thesymbol window only onemedia access testsymbol (MTS) may betransmitted perchannel.
MTS symbols aresent inNORMAL_ACTIVE state totestthebusguardian.
The symbol window ischaracterized bythefollowing features:
*Send single symbol
*Transmission oftheMTS symbol starts atthesymbol windows action point

<!-- Page 1227 -->

GTUC2.MPC = m
GTUC4.NIT = k
GTUC4.OCS = NIT+1
Static / Dynamic Segment Symbol Window NIT0 n n+1 k k+1 m+1
www.ti.com Module Operation
1227 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleParameters: Symbol Window Action Point Offset GTUC9.APO(4-0) (same asforstatic slots), Network
IdleTime Start GTUC4.NIT(13-0)
26.2.2.4 Network IdleTime (NIT)
During network idletime thecommunication controller hastoperform thefollowing tasks:
*Calculate clock correction terms (offset andrate)
*Distribute offset correction over multiple macroticks after offset correction start
*Perform cluster cycle related tasks
Parameters: network idletime start GTUC4.NIT(13-0), offset correction start GTUC4.OCS(13-0)
26.2.2.5 Configuration ofNITStart andOffset Correction Start
Figure 26-11. Configuration ofNITStart andOffset Correction Start
The number ofmacroticks percycle isassumed tobem.Itisconfigured byprogramming GTUC2.MPC =
mintheGTU Configuration register 2.
The static /dynamic segment starts with macrotick 0andends with macrotick n:n=static segment length
+dynamic segment offset +dynamic segment length -1MT
The static segment length isconfigured byGTUC7.SSL andGTUC7.NSS. The dynamic segment length is
configured byGTUC8.MSL andGTUC8.NMS.
The dynamic segment offset isActionPointOffset -MinislotActionPointOffset or0MTiftheresult is
negative. Fordetails, refer totheFlexRay Communications System Protocol Specification from the
FlexRay Consortium.
The NITstarts with macrotick k+1andends with thelastmacrotick ofcycle m-1. Ithastobeconfigured by
setting GTUC4.NIT =k.
ForthisFlexRay module theoffset correction start isrequired tobeGTUC4.OCS >=GTUC4.NIT +1=
k+1.
The length ofsymbol window results from thenumber ofmacroticks between theendofthestatic /
dynamic segment andthebeginning oftheNIT. Itcanbecalculated byk-n.
26.2.3 Communication Modes
The FlexRay protocol specification v2.1 Rev. Adefines theTime-Triggered Distributed (TT-D) mode.
26.2.3.1 Time-Triggered Distributed (TT-D)
InTT-D mode thefollowing configurations arepossible:
*Pure static: minimum 2static slots +symbol window (optional)
*Mixed static/dynamic: minimum 2static slots +dynamic segment +symbol window (optional)
Aminimum oftwocoldstart nodes need tobeconfigured fordistributed time-triggered operation. Two
fault-free coldstart nodes arenecessary forthecluster startup. Each startup frame must beasync frame,
therefore allcoldstart nodes aresync nodes.

<!-- Page 1228 -->

Module Operation www.ti.com
1228 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.4 Clock Synchronization
InTT-D mode adistributed clock synchronization isused. Each node individually synchronizes itself tothe
cluster byobserving thetiming ofreceived sync frames from other nodes.
26.2.4.1 Global Time
Activities inaFlexRay node, including communication, arebased ontheconcept ofaglobal time, even
though each individual node maintains itsown view ofit.Itistheclock synchronization mechanism that
differentiates theFlexRay cluster from other node collections with independent clock mechanisms. The
global time isavector oftwovalues; thecycle (cycle counter) andthecycle time (macrotick counter).
Cluster specific:
*Macrotick (MT) =basic unitoftime measurement inaFlexRay network, amacrotick consists ofan
integer number ofmicroticks (μT)
*Cycle length =duration ofacommunication cycle inunits ofmacroticks (MT)
26.2.4.2 Local Time
Internally, nodes time their behavior with microtick resolution. Microticks aretime units derived from the
oscillator clock tickofthespecific node. Therefore microticks arecontroller-specific units. They may have
different duration indifferent controllers. The precision ofanodes local time difference measurements isa
microtick (μT).
Node specific:
*Sample clock ->prescaler ->microtick (µT);typically 25ns.
*μT=basic unitoftime measurement inacommunication controller, clock correction isdone inunits of
μTs
*Cycle counter +macrotick counter =nodes local view oftheglobal time
26.2.4.3 Synchronization Process
Clock synchronization isperformed bymeans ofsync frames. Only preconfigured nodes (sync nodes) are
allowed tosend sync frames. Inatwo-channel cluster, async node hastosend itssync frame onboth
channels.
Forsynchronization inFlexRay thefollowing constraints have tobeconsidered:
*Max. onesync frame pernode inonecommunication cycle
*Max. 15sync frames percluster inonecommunication cycle
*Every node hastouseapreconfigured number ofsync frames (GTUC2.SNM(3-0)) forclock
synchronization
*Minimum oftwosync nodes required forclock synchronization andstartup
Forclock synchronization thetime difference between expected andobserved arrival time ofsync frames
received during thestatic segment ismeasured. Inatwochannel cluster thesync node hastobe
configured tosend sync frames onboth channels. The calculation ofcorrection terms isdone during NIT
(offset: every cycle, rate: oddcycle) byusing aFTA /FTM algorithm. Fordetails seeFlexRay protocol
specification v2.1 Rev. A.
26.2.4.3.1 Offset (Phase) Correction
*Only deviation values measured andstored inthecurrent cycle used
*Foratwochannel node thesmaller value willbetaken
*Calculation during NITofevery communication cycle
*Offset correction value calculated ineven cycles used forerror checking only
*Checked against limit values
*Correction value isasigned integer number ofμTs

<!-- Page 1229 -->

www.ti.com Module Operation
1229 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module*Correction done inoddnumbered cycles, distributed over themacroticks beginning atoffset correction
start uptocycle end(end ofNIT) toshift nodes next start ofcycle (MTs lengthened /shortened)
26.2.4.3.2 Rate (Frequency) Correction
*Pairs ofdeviation values measured andstored ineven /oddcycle pairused
*Foratwochannel node theaverage ofthedifferences from thetwochannels isused
*Calculated during NITofoddnumbered cycles
*Cluster driftdamping isperformed using global damping value
*Checked against limit values
*Correction value isasigned integer number ofμTs
*Distributed over macroticks comprising thenext even/odd cycle pair (MTs lengthened /shortened)
26.2.4.4 Sync Frame Transmission
Sync frame transmission isonly possible from buffer 0and1.Message buffer 1may beused forsync
frame transmission incase thatsync frames should have different payloads onthetwochannels. Inthis
case bitMRC.SPLM hastobeprogrammed to1.
Message buffers used forsync frame transmission have tobeconfigured with thekeyslotIDandcanbe
(re)configured inDEFAULT_CONFIG orCONFIG state only. Fornodes transmitting sync frames
SUCC1.TXSY must besetto1.
26.2.4.5 External Clock Synchronization
During normal operation, independent clusters candriftsignificantly. Ifsynchronous operation across
independent clusters isdesired, external synchronization isnecessary; even though thenodes within each
cluster aresynchronized. This canbeaccomplished with synchronous application ofhost-deduced rate
andoffset correction terms totheclusters.
*External offset /rate correction value isasigned integer
*External offset /rate correction value isadded tocalculated offset /rate correction value
*Aggregated offset /rate correction term (external +internal) isnotchecked against configured limits
26.2.5 Error Handling
The implemented error handling concept oftheFlexRay protocol isintended toensure thatinthe
presence ofalower layer protocol error inasingle node, communication between non-affected nodes can
bemaintained. Insome cases, higher layer program command activity isrequired forthecommunication
controller toresume normal operation. Achange oftheerror handling state willsetbitEIR.PEMC andcan
trigger aninterrupt tothehost ifenabled. The current error mode issignaled byCCEV.ERRM(1-0).
Table 26-5. Error Modes ofthePOC (Degradation Model)
Error Mode Activity
ACTIVE Fulloperation ,State: NORMAL_ACTIVE
The communication controller isfully synchronized andsupports thecluster wide clock synchronization. The
host isinformed ofanyerror condition(s) orstatus change byinterrupt (ifenabled) orbyreading theerror
andstatus flags from registers EIRandSIR.
PASSIVE Reduced operation ,State: NORMAL_PASSIVE, communication controller selfrescue allowed
The communication controller stops transmitting frames andsymbols, butreceived frames arestill
processed. Clock synchronization mechanisms arecontinued based onreceived frames. Noactive
contribution tothecluster wide clock synchronization. The host isinformed ofanyerror condition(s) orstatus
change byinterrupt (ifenabled) orbyreading theerror andstatus flags from registers EIRandSIR.
COMM_HALT Operation halted ,State: HALT, communication controller selfrescue notallowed
The communication controller stops frame andsymbol processing, clock synchronization processing, and
themacrotick generation. The host hasstillaccess toerror andstatus information byreading theerror and
status flags from registers EIRandSIR. The busdrivers aredisabled.

<!-- Page 1230 -->

Module Operation www.ti.com
1230 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.5.1 Clock Correction Failed Counter
When theClock Correction Failed Counter reaches the"maximum without clock correction passive "limit
defined bySUCC3.WCP(3-0), thePOC transits from NORMAL_ACTIVE toNORMAL_PASSIVE state.
When itreaches the"maximum without clock correction fatal"limit defined bySUCC3.WCF(3-0), ittransits
NORMAL_ACTIVE orNORMAL_PASSIVE totheHALT state.
The Clock Correction Failed Counter CCEV.CCFC(3-0) allows thehost tomonitor theduration ofthe
inability ofanode tocompute clock correction terms after thecommunication controller passed protocol
startup phase. Itwillbeincremented by1attheendofanyoddnumbered communication cycle where
either theMissing Offset Correction flagSFS.MOCS ortheMissing Rate Correction flagSFS.MRCS isset.
The clock correction failed counter isreset to0attheendofanoddcommunication cycle ifneither the
Missing Offset Correction flagSFS.MOCS northeMissing Rate Correction flagSFS.MRCS isset.
The Clock Correction Failed Counter stops incrementing when the"maximum without clock correction
fatal"value SUCC3.WCF(3-0) isreached (incrementing thecounter atitsmaximum value willnotcause it
towraparound back to0).The clock correction failed counter willbeinitialized to0when the
communication controller enters READY state orwhen NORMAL_ACTIVE state isentered.
NOTE: The transition toHALT state isprevented ifSUCC1.HCSE isnotset.
26.2.5.2 Passive toActive Counter
The passive toactive counter controls thetransition ofthePOC from NORMAL_PASSIVE to
NORMAL_ACTIVE state. SUCC1.SUCC1.PTA(4-0) defines thenumber ofconsecutive even /oddcycle
pairs thatmust have valid clock correction terms before thecommunication controller isallowed totransit
from NORMAL_PASSIVE toNORMAL_ACTIVE state. IfSUCC1.PTA(4-0) iscleared to0,the
communication controller isnotallowed totransit from NORMAL_PASSIVE toNORMAL_ACTIVE state.
26.2.5.3 HALT Command
Incase thehost wants tostop FlexRay communication ofthelocal node itcanbring thecommunication
controller intoHALT state byasserting theHALT command. This canbedone bywriting SUCC1.CMD(3-
0)=0110. Inorder toshut down communication onanentire FlexRay network, ahigher layer protocol is
required toassure thatallnodes apply theHALT command atthesame time.
The POC state from which thetransition toHALT state took place canberead from CCSV.PSL(5-0).
When called inNORMAL_ACTIVE orNORMAL_PASSIVE state thePOC transits toHALT state attheend
ofthecurrent cycle. When called inanyother state SUCC1.CMD(3-0) willbereset to0000 =
"command_not_accepted "andbitEIR.CNA intheerror interrupt register issetto1.Ifenabled aninterrupt
tothehost isgenerated.
26.2.5.4 FREEZE Command
Incase thehost detects asevere error condition itcanbring thecommunication controller intoHALT state
byasserting theFREEZE command. This canbedone bywriting SUCC1.CMD(3-0) =0111. The FREEZE
command triggers theentry oftheHALT state immediately regardless ofthecurrent POC state.
The POC state from which thetransition toHALT state took place canberead from CCSV.PSL(5-0).

<!-- Page 1231 -->

T1
T3
T4
T5
T8
T9T6
T7HW Reset
Power On
READY WAKEUP HALTMONITOR
CONFIG
STARTUPMODE
T10
T11T12T13T14
T15T17
T16
Transition triggered by host command
Transition triggered by internal conditions
Transition triggered by host command OR internal condition sDEFAULT_
CONFIG
T2
NORMAL
ACTIVENORMAL
PASSIVE
www.ti.com Module Operation
1231 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.6 Communication Controller States
26.2.6.1 Communication Controller State Diagram
Figure 26-12. Overall State Diagram ofCommunication Controller
State transitions arecontrolled bythereset andFlexRay receive (rxd1, 2)pins, thePOC state machine,
andbytheCHI command vector SUCC1.CMD(3-0).
The Communication Controller exits from allstates toHALT state after application oftheFREEZE
command (SUCC1.CMD(3-0) =0111b).

<!-- Page 1232 -->

Module Operation www.ti.com
1232 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-6. State Transitions ofCommunication Controller Overall State Machine
T# Condition From To
1 Hardware reset AllStates DEFAULT_CONFIG
2 Command CONFIG, SUCC1.CMD(3-0) =0001 DEFAULT_CONFIG CONFIG
3 Unlock sequence followed bycommand MONITOR_MODE,
SUCC1.CMD(3-0) =1011CONFIG MONITOR_MODE
4 Command CONFIG, SUCC1.CMD(3-0) =0001 MONITOR_MODE CONFIG
5 Unlock sequence followed bycommand READY, SUCC1.CMD(3-0) =
0010CONFIG READY
6 Command CONFIG, SUCC1.CMD(3-0) =0001 READY CONFIG
7 Command WAKEUP, SUCC1.CMD(3-0) =0011 READY WAKEUP
8 Complete, non-aborted transmission ofwakeup pattern ORreceived
WUP ORreceived frame header ORcommand READY,
SUCC1.CMD(3-0) =0010WAKEUP READY
9 Command RUN, SUCC1.CMD(3-0) =0100 READY STARTUP
10 Successful startup STARTUP NORMAL_ACTIVE
11 Clock correction failed counter reached "maximum without clock
correction passive "limit configured bySUCC3.WCP(3-0)NORMAL_ACTIVE NORMAL_PASSIVE
12 Number ofvalid correction terms reached thePassive toActive limit
configured bySUCC1.PTA(4-0)NORMAL_PASSIVE NORMAL_ACTIVE
13 Command READY, SUCC1.CMD(3-0) =0010 STARTUP,
NORMAL_ACTIVE,NORMAL
_PASSIVEREADY
14 Clock Correction Failed counter reached "maximum without clock
correction fatal"limit configured bySUCC3.WCF(3-0) AND bit
SUCC1.HCSE setto1ORcommand HALT, SUCC1.CMD(3-0) =0110NORMAL_ACTIVE HALT
15 Clock Correction Failed counter reached "maximum without clock
correction fatal"limit configured bySUCC3.WCF(3-0) AND bit
SUCC1.HCSE setto1ORcommand HALT, SUCC1.CMD(3-0) =0110NORMAL_PASSIVE HALT
16 Command FREEZE, SUCC1.CMD(3-0) =0111 AllStates HALT
17 Command CONFIG, SUCC1.CMD(3-0) =0001 HALT DEFAULT_CONFIG
26.2.6.2 DEFAULT_CONFIG State
InDEFAULT_CONFIG state, thecommunication controller isstopped. Allconfiguration registers are
accessible andthepins tothephysical layer areintheir inactive state.
The communication controller enters thisstate:
*When leaving hardware reset
*When exiting from HALT state
Toleave DEFAULT_CONFIG state thehost hastowrite SUCC1.CMD(3-0) =0001. The communication
controller then transits toCONFIG state.
26.2.6.3 CONFIG State
InCONFIG state, thecommunication controller isstopped. Allconfiguration registers areaccessible and
thepins tothephysical layer areintheir inactive state. This state isused toinitialize thecommunication
controller configuration.
The communication controller enters thisstate:
*When exiting from DEFAULT_CONFIG state
*When exiting from MONITOR_MODE orREADY state
When thestate hasbeen entered byHALT andDEFAULT_CONFIG state, thehost cananalyze status
information andconfiguration. Before leaving CONFIG state thehost hastoassure thattheconfiguration
isfault-free.

<!-- Page 1233 -->

www.ti.com Module Operation
1233 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleToleave CONFIG state, thehost hastoperform theunlock sequence. Directly after unlocking the
CONFIG state thehost hastowrite SUCC1.CMD(3-0) toenter thenext state.
NOTE: The message buffer status registers (MHDS, TXRQ1/2/3/4, NDAT1/2/3/4, MBSC1/2/3/4) and
status data stored inthemessage RAM andarenotaffected bythetransition ofthePOC
from CONFIG toREADY state.
When thecommunication controller isinCONFIG state itisalso possible tobring thecommunication
controller intoapower saving mode byhalting themodule clocks. Todothisthehost hastoassure thatall
Message RAM transfers have finished before turning offtheclocks.
26.2.6.4 MONITOR_MODE
After unlocking CONFIG state andwriting SUCC1.CMD(3-0) =1011 thecommunication controller enters
MONITOR_MODE. Inthismode thecommunication controller isable toreceive FlexRay frames andto
detect wakeup pattern. The temporal integrity ofreceived frames isnotchecked, andtherefore cycle
counter filtering isnotsupported. This mode canbeused fordebugging purposes incase e.g.thatstartup
ofaFlexRay network fails. After writing SUCC1.CMD(3-0) =0001 thecommunication controller transits
back toCONFIG state.
InMONITOR_MODE thepick firstvalid mechanism isdisabled. This means thatareceive message buffer
may only beconfigured toreceive ononechannel. Received frames arestored intomessage buffers
according toframe IDandreceive channel. Null frames arehandled likedata frames. After frame
reception only status bitsMBS.VFRA, MBS.VFRB, MBS.MLST, MBS.RCIS, MBS.SFIS, MBS.SYNS,
MBS.NFIS, MBS.PPIS, MBS.RESS have valid values.
InMONITOR_MODE thecommunication controller isnotable todistinguish between CAS andMTS
symbols. Incase oneofthese symbols isreceived ononeorboth ofthetwochannels, theflags
SIR.MTSA/SIR.MTSB areset.SIR.CAS hasnofunction inMONITOR_MODE.
26.2.6.5 READY State
After unlocking CONFIG state andwriting SUCC1.CMD(3-0) =0010 thecommunication controller enters
READY state. From thisstate thecommunication controller cantransit toWAKEUP state andperform a
cluster wakeup ortoSTARTUP state toperform acoldstart ortointegrate intoarunning cluster.
The communication controller enters thisstate:
*When exiting from CONFIG, WAKEUP, STARTUP, NORMAL_ACTIVE, orNORMAL_PASSIVE state
bywriting SUCC1.CMD(3-0) =0010 (READY command).
The communication controller exits from thisstate:
*ToCONFIG state bywriting SUCC1.CMD(3-0) =0001 (CONFIG command)
*ToWAKEUP state bywriting SUCC1.CMD(3-0) =0011 (WAKEUP command)
*ToSTARTUP state bywriting SUCC1.CMD(3-0) =0100 (RUN command)
Internal counters andthecommunication controller status flags arereset when thecommunication
controller enters STARTUP state.
NOTE: Status bitsMHDS(14-0), registers TXRQ1/2/3/4, andstatus data stored intheMessage RAM
arenotaffected bythetransition ofthePOC from READY toSTARTUP state.

<!-- Page 1234 -->

Module Operation www.ti.com
1234 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.6.6 WAKEUP State
The following description isintended tohelp configuring wakeup fortheFlexRay module. Adetailed
description ofthewakeup procedure canbefound intheFlexRay protocol specification v2.1 Rev. A.
The communication controller enters thisstate:
*When exiting from READY state bywriting SUCC1.CMD(3-0) =0011 (WAKEUP command).
The communication controller exits from thisstate toREADY state:
*After complete non-aborted transmission ofwakeup pattern
*After WUP reception
*After detecting aWUP collision
*After reception ofaframe header
*Bywriting SUCC1.CMD(3-0) =0010 (READY command)
The communication controller exits from thisstate toHALT state:
*Bywriting SUCC1.CMD(3-0) =0111 (FREEZE command)
The cluster wakeup must precede thecommunication startup inorder toensure thatallnodes inacluster
areawake. The minimum requirement foracluster wakeup isthatallbusdrivers aresupplied with power.
Abusdriver hastheability towake uptheother components ofitsnode when itreceives awakeup
pattern onitschannel. Atleast onenode inthecluster needs anexternal wakeup source.
The host completely controls thewakeup procedure. Itisinformed about thestate ofthecluster bythebus
driver andthecommunication controller andconfigures busguardian (ifavailable) andcommunication
controller toperform thecluster wakeup. The communication controller provides tothehost theability to
transmit aspecial wakeup pattern oneach ofitsavailable channels separately. The communication
controller needs torecognize thewakeup pattern only during wakeup andstartup phase.
Wakeup may beperformed ononly onechannel atatime. The host hastoconfigure thewakeup channel
while thecommunication controller isinCONFIG state bywriting bitWUCS intheSUC configuration
register 1.The communication controller ensures thatongoing communication onthischannel isnot
disturbed. The communication controller cannot guarantee thatallnodes connected totheconfigured
channel awake upon thetransmission ofthewakeup pattern, since these nodes cannot give feedback
until thestartup phase. The wakeup procedure enables single-channel devices inatwo-channel system to
trigger thewakeup, byonly transmitting thewakeup pattern onthesingle channel towhich they are
connected. Any coldstart node thatdeems asystem startup necessary willthen wake theremaining
channel before initiating communication startup.
The wakeup procedure tolerates anynumber ofnodes simultaneously trying towakeup asingle channel
andresolves thissituation such thatonly onenode transmits thepattern. Additionally thewakeup pattern
iscollision resilient, soeven inthepresence ofafault causing twonodes tosimultaneously transmit a
wakeup pattern, theresulting collided signal canstillwake theother nodes.
After wakeup thecommunication controller returns toREADY state andsignals thechange ofthewakeup
status tothehost bysetting bitSIR.WST inthestatus interrupt register. The wakeup status vector
CCSV.WSV(2-0) canberead from thecommunication controller status vector register. Ifavalid wakeup
pattern was received also either bitSIR.WUPA orbitSIR.WUPB inthestatus interrupt register isset.

<!-- Page 1235 -->

Tenter
T2T6
T3 T5
WAKEUPWAKEUP
SENDWAKEUP
LISTENWAKEUP
DETECTT4TexitREADY
WAKEUP
STANDBY
T1
www.ti.com Module Operation
1235 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-13. Structure ofPOC State WAKEUP
Table 26-7. State Transitions WAKEUP
T# Condition From To
enter Host commands change toWAKEUP state bywriting SUCC1.CMD(3-
0)=0011 (WAKEUP command)READY WAKEUP
1 CHI command WAKEUP triggers wakeup FSM totransit to
WAKEUP_LISTEN stateWAKEUP_STANDBY WAKEUP_LISTEN
2 Received WUP onwakeup channel selected bybitSUCC1.WUCS OR
frame header oneither available channelWAKEUP_LISTEN WAKEUP_STANDBY
3 Timer event WAKEUP_LISTEN WAKEUP_SEND
4 Complete, non-aborted transmission ofwakeup pattern WAKEUP_SEND WAKEUP_STANDBY
5 Collision detected WAKEUP_SEND WAKEUP_DETECT
6 Wakeup timer expired ORWUP detected onwakeup channel selected
bybitSUCC1.WUCS ORframe header received oneither available
channelWAKEUP_DETECT WAKEUP_STANDBY
exit Wakeup completed (after T2orT4orT6)ORhost commands change
toREADY state bywriting SUCC1.CMD(3-0) =0010 (READY
command). This command also resets thewakeup FSM to
WAKEUP_STANDBY state.WAKEUP READY
The WAKEUP_LISTEN state iscontrolled bythewakeup timer andthewakeup noise timer. The two
timers arecontrolled bytheparameters Listen Timeout SUCC2.LT(20-0) andListen Timeout Noise
SUCC2.LTN(3-0). Listen timeout enables afastcluster wakeup incase ofanoise free environment, while
listen timeout noise enables wakeup under more difficult conditions regarding noise interference.

<!-- Page 1236 -->

Module Operation www.ti.com
1236 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleInWAKEUP_SEND state thecommunication controller transmits thewakeup pattern ontheconfigured
channel andchecks forcollisions. After return from wakeup thehost hastobring thecommunication
controller intoSTARTUP state byCHI command RUN.
InWAKEUP_DETECT state thecommunication controller attempts toidentify thereason forthewakeup
collision detected inWAKEUP_SEND state. The monitoring isbounded bytheexpiration oflisten timeout
asconfigured bySUCC2.LT(20-0) intheSUC configuration register 2.Either thedetection ofawakeup
pattern indicating awakeup attempt byanother node orthereception ofaframe header indication existing
communication, causes thedirect transition toREADY state. Otherwise WAKEUP_DETECT isleftafter
expiration oflisten timeout; inthiscase thereason forwakeup collision isunknown.
The host hastobeaware ofpossible failures ofthewakeup andactaccordingly. Itisadvisable todelay
anypotential startup attempt ofthenode having instigated thewakeup bytheminimal time ittakes
another coldstart node tobecome awake andtobeconfigured.
The FlexRay Protocol Specification recommends thattwodifferent communication controllers shall wake
thetwochannels.
26.2.6.6.1 Host Activities
The host must coordinate thewakeup ofthetwochannels andmust decide whether, ornot,towake a
specific channel. The sending ofthewakeup pattern isinitiated bythehost. The wakeup pattern is
detected bytheremote BDs andsignaled totheir local hosts.
Wakeup procedure controlled byhost (single-channel wakeup):
*Configure thecommunication controller inCONFIG state
-Select wakeup channel byprogramming bitSUCC1.WUCS
*Check local BDs whether aWUP was received
*Activate BDofselected wakeup channel
*Command communication controller toenter READY state
*Command communication controller tostart wakeup ontheconfigured channel bywriting
SUCC1.CMD(3-0) =0011
-communication controller enters WAKEUP_LISTEN
-communication controller returns toREADY state andsignals status ofwakeup attempt tohost
*Wait predefined time toallow theother nodes towakeup andconfigure themselves
*Coldstart node:
-indual channel cluster wait forWUP ontheother channel
-Reset Coldstart Inhibit flagCCSV.CSI bywriting SUCC1.CMD(3-0) =1001 (ALLOW_COLDSTART
command)
*Command communication controller toenter startup bywriting SUCC1.CMD(3-0) =0100 (RUN
command)
Wakeup procedure triggered bythebusdriver:
*Wakeup recognized bybusdriver
*busdriver triggers power-up ofhost (ifrequired)
*busdriver signals wakeup event tohost
*Host configures itslocal communication controller
*Ifnecessary host commands wakeup ofsecond channel andwaits predefined time toallow theother
nodes towakeup andconfigure themselves
*Host commands communication controller toenter STARTUP state bywriting SUCC1.CMD(3-0) =
0100 (RUN command)

<!-- Page 1237 -->

Tx-wakeup Symbol
Rx-wakeup Pattern
(no collision)
Rx-wakeup Pattern
(collision, worst case)TXL = 15-60 bit times TXI = 45-180 bit times
www.ti.com Module Operation
1237 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.6.6.2 Wake UpPattern (WUP)
The wake uppattern (WUP) iscomposed ofatleast twowakeup symbols (WUS). Wakeup symbol and
wakeup pattern areconfigured bythePRT configuration registers 1,2.
*Single channel wakeup, wake upsymbol may notbesent onboth channels atthesame time
*Wakeup symbol collision resilient foruptotwosending nodes (two overlapping wakeup symbols still
recognizable)
*Wakeup symbol must beconfigured identical inallnodes ofacluster
*Wakeup symbol transmit lowtime configured byPRTC2.TXL(5-0)
*Wakeup symbol idletime configured byPRTC2.TXI(7-0), used tolisten foractivity onthebus
*Awakeup pattern composed ofatleast twoTx-wakeup symbols needed forwakeup
*Number ofrepetitions configurable byPRTC1.RWP(5-0) (2to63repetitions)
*Wakeup symbol receive window length configured byPRTC1.RXW(8-0)
*Wakeup symbol receive lowtime configured byPRTC2.RXL(5-0)
*Wakeup symbol receive idletime configured byPRTC2.RXI(5-0)
Figure 26-14. Timing ofWake UpPattern
26.2.6.7 STARTUP State
The following description isintended tohelp configuring startup fortheFlexRay module. Adetailed
description ofthestartup procedure canbefound intheFlexRay protocol specification v2.1 Rev. A.
Any node entering STARTUP state thathascoldstart capability should assure thatboth channels attached
have been awakened before initiating coldstart.
Itcannot beassumed thatallnodes andstars need thesame amount oftime tobecome completely
awake andtobeconfigured. Since atleast twonodes arenecessary tostart upthecluster
communication, itisadvisable todelay anypotential startup attempt ofthenode having instigated the
wakeup bytheminimal amount oftime ittakes another coldstart node tobecome awake, tobeconfigured
andtoenter startup. Itmay require several hundred milliseconds (depending onthehardware used)
before allnodes andstars arecompletely awakened andconfigured.
Startup isperformed onallchannels synchronously. During startup, anode only transmits startup frames.
Startup frames areboth sync frames andnullframes during startup.
Afault-tolerant, distributed startup strategy isspecified forinitial synchronization ofallnodes. Ingeneral, a
node may enter NORMAL_ACTIVE state by:
*Coldstart path initiating theschedule synchronization (leading coldstart node)
*Coldstart path joining other coldstart nodes (following coldstart node)
*Integration path integrating intoanexisting communication schedule (allother nodes)
See also Figure 26-15 formore information.

<!-- Page 1238 -->

Module Operation www.ti.com
1238 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleAcoldstart attempt begins with thetransmission ofacollision avoidance symbol (CAS). Only acoldstart
node thattransmitted theCAS, transmits frames inthefirstfour cycles after theCAS. itisthen joined
firstly bytheother coldstart nodes andafterwards byallother nodes.
Acoldstart node hasbitsSUCC1.TXST andSUCC1.TXSY setto1.Message buffer 0holds thekeyslot
IDwhich defines theslotnumber where thestartup frame issent. The startup frame indicator bitissetin
theframe header ofthestartup frame.
Inclusters consisting ofthree ormore nodes, atleast three nodes shall beconfigured tobecoldstart
nodes. Inclusters consisting oftwonodes, both nodes must becoldstart nodes. Atleast twofault-free
coldstart nodes arenecessary forthecluster tostartup.
Each startup frame must also beasync frame; therefore each coldstart node willalso beasync node.
The number ofcoldstart attempts isconfigured bySUCC1.CSA(4-0) intheSUC configuration register 1.
Anon-coldstart node requires atleast twostartup frames from distinct nodes forintegration. Itmay start
integration before thecoldstart nodes have finished their startup. Itwillnotfinish itsstartup until atleast
twocoldstart nodes have finished their startup.
Both non-coldstart nodes andcoldstart nodes start passive integration through theintegration path as
soon asthey receive sync frames from which toderive theTDMA schedule information. During integration
thenode hastoadapt itsown clock totheglobal clock (rate andoffset) andhastomake itscycle time
consistent with theglobal schedule observable atthenetwork. Afterwards, these settings arechecked for
consistency with allavailable network nodes. The node canonly leave theintegration phase andactively
participate incommunication when these checks arepassed.
26.2.6.7.1 Coldstart Inhibit Mode
Incoldstart inhibit mode, thenode isprevented from initializing theTDMA communication schedule. Ifthe
CCSV.CSI bitinthecommunication controller status vector register isset,thenode isnotallowed to
initialize thecluster communication, thatis,entering thecoldstart path isprohibited. The node isallowed to
integrate toarunning cluster ortotransmit startup frames after another coldstart node starts the
initialization ofthecluster communication.
The coldstart inhibit bitCCSV.CSI issetwhenever thePOC enters READY state. The bithastobe
cleared under control ofthehost byCHI command ALLOW_COLDSTART (SUCC1.CMD(3-0) =1001).

<!-- Page 1239 -->

STARTUPLeading coldstart node
Following coldstart node
Non-coldstart node integrating
COLDSTART
COLLISIONCOLDSTART
LISTENINTEGRATION
LISTEN
RESOLUTION
INTEGRATION
COLDSTART
CHECKABORT
STARTUPABORT
STARTUP
ABORT
STARTUP
ABORT
STARTUPCOLDSTART
GAPSTARTUP
PREPARE
INTEGRATION
CONSISTENCY
CHECKINITIALIZE
SCHEDULE
NORMAL
ACTIVECOLDSTART
CONSISTENCY
CHECK
COLDSTART
JOINABORT
STARTUP
ABORT
STARTUPABORT
STARTUPREADY
www.ti.com Module Operation
1239 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-15. State Diagram Time-Triggered Startup

<!-- Page 1240 -->

Module Operation www.ti.com
1240 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.6.7.2 Startup Timeouts
The communication controller supplies twodifferent μTtimers supporting twotimeout values, startup
timeout andstartup noise timeout. The twotimers arestarted when thecommunication controller enters
theCOLDSTART_LISTEN state. The expiration ofeither ofthese timers causes thenode toleave the
initial sensing phase (COLDSTART_LISTEN state) with theintention ofstarting upcommunication.
NOTE: The startup andstartup noise timers areidentical with thewakeup andwakeup noise timers
andusethesame configuration values SUCC2.LT(20-0) andSUCC2.LTN(3-0) from theSUC
configuration register 2.
26.2.6.7.2.1 Startup Timeout
The startup timeout limits thelisten time used byanode todetermine ifthere isalready communication
between other nodes oratleast onecoldstart node actively requesting theintegration ofothers. The
startup timer isconfigured byprogramming SUCC2.LT(20-0) intheSUC configuration register 2.
The startup timeout time canbecalculated from thecontents ofSUCC2.LT(20-0) (Refer totheFlexRay
Protocol Specification: pdListenTimeout)
The startup timer isrestarted upon:
*Entering theCOLDSTART_LISTEN state
*Both channels reaching idlestate while inCOLDSTART_LISTEN state
The startup timer isstopped:
*Ifcommunication channel activity isdetected ononeoftheconfigured channels while thenode isin
theCOLDSTART_LISTEN state
*When theCOLDSTART_LISTEN state isleft
Once thestartup timeout expires, neither anoverflow noracyclic restart ofthetimer isperformed. The
timer status iskept forfurther processing bythestartup state machine.
26.2.6.7.2.2 Startup Noise Timeout
Atthesame time thestartup timer isstarted forthefirsttime (transition from STARTUP_PREPARE state
toCOLDSTART_LISTEN state), thestartup noise timer isstarted. This additional timeout isused to
improve reliability ofthestartup procedure inthepresence ofnoise. The startup noise timer isconfigured
byprogramming SUCC2.LTN(3-0) intheSUC configuration register 2.
The startup noise timeout time canbecalculated astheproduct ofSUCC2.LT(20-0) *SUCC2.LTN(3-0)
(Refer totheFlexRay Protocol Specification: pdListenTimeout *gListenNoise)
The startup noise timer isrestarted upon:
*Entering theCOLDSTART_LISTEN state
*Reception ofcorrectly decoded headers orCAS symbols while thenode isinCOLDSTART_LISTEN
state
The startup noise timer isstopped when theCOLDSTART_LISTEN state isleft.
Once thestartup noise timeout expires, neither anoverflow noracyclic restart ofthetimer isperformed.
The status iskept forfurther processing bythestartup state machine. Since thestartup noise timer won't
berestarted when random channel activity issensed, thistimeout defines thefall-back solution that
guarantees thatanode willtrytostart upthecommunication cluster even inthepresence ofnoise.
26.2.6.7.3 Path ofLeading Coldstart Node (Initiating Coldstart)
When acoldstart node enters COLDSTART_LISTEN, itlistens toitsattached channels.
Ifnocommunication isdetected, thenode enters theCOLDSTART_COLLISION_RESOLUTION state and
commences acoldstart attempt. The initial transmission ofaCAS symbol issucceeded bythefirstregular
cycle. This cycle hasthenumber 0.

<!-- Page 1241 -->

www.ti.com Module Operation
1241 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFrom cycle 0on,thenode transmits itsstartup frame. Since each coldstart node isallowed toperform a
coldstart attempt, itmay occur thatseveral nodes simultaneously transmit theCAS symbol andenter the
coldstart path. This situation isresolved during thefirstfour cycles after CAS transmission.
Assoon asanode thatinitiates acoldstart attempt receives aCAS symbol oraframe header during
these four cycles, itre-enters theCOLDSTART_LISTEN state. Thereby, only onenode remains inthis
path. Incycle four, other coldstart nodes begin totransmit their startup frames.
After four cycles inCOLDSTART_COLLISION_RESOLUTION state, thenode thatinitiated thecoldstart
enters theCOLDSTART_CONSISTENCY_CHECK state. Itcollects allstartup frames from cycle four and
fiveandperforms clock correction. Iftheclock correction does notdeliver anyerrors andithasreceived at
least onevalid startup frame pair, thenode leaves COLDSTART_CONSISTENCY_CHECK andenters
NORMAL_ACTIVE state.
The number ofcoldstart attempts thatanode isallowed toperform isconfigured bySUCC1.CSA(4-0) in
theSUC configuration register 1.The number ofremaining coldstarts attempts canberead from
CCSV.RCA(4-0) ofcommunication controller status vector register. The number ofremaining attempts is
reduced by1foreach attempted coldstart. Anode may enter theCOLDSTART_LISTEN state only ifthis
value islarger than 1anditmay enter theCOLDSTART_COLLISION_RESOLUTION state only ifthis
value islarger than 0.Ifthenumber ofcoldstart attempts is1,coldstart isinhibited butintegration isstill
possible.
26.2.6.7.4 Path ofFollowing Coldstart Node (Responding toLeading Coldstart Node)
When acoldstart node enters theCOLDSTART_LISTEN state, ittries toreceive avalid pairofstartup
frames toderive itsschedule andclock correction from theleading coldstart node.
Assoon asavalid startup frame hasbeen received, theINITIALIZE_SCHEDULE state isentered. Ifthe
clock synchronization cansuccessfully receive amatching second valid startup frame andderive a
schedule from this, theINTEGRATION_COLDSTART_CHECK state isentered.
InINTEGRATION_COLDSTART_CHECK state, itisassured thattheclock correction canbeperformed
correctly andthatthecoldstart node from which thisnode hasinitialized itsschedule isstillavailable. The
node collects allsync frames andperforms clock correction inthefollowing double-cycle. Ifclock
correction does notsignal anyerrors andifthenode continues toreceive sufficient frames from thesame
node ithasintegrated on,theCOLDSTART_JOIN state isentered.
InCOLDSTART_JOIN state, following coldstart nodes begin totransmit their own startup frames and
continue todosoinsubsequent cycles. Thereby, theleading coldstart node andthenodes joining itcan
check iftheir schedules agree with each other. Iftheclock correction signals anyerror, thenode aborts
theintegration attempt. Ifanode inthisstate sees atleast onevalid startup frame during alleven cycles
inthisstate andatleast onevalid startup frame pairduring alldouble cycles inthisstate, thenode leaves
COLDSTART_JOIN state andenters NORMAL_ACTIVE state. Thereby itleaves STARTUP atleast one
cycle after thenode thatinitiated thecoldstart.
26.2.6.7.5 Path ofNon-Coldstart Node
When anon-coldstart node enters theINTEGRATION_LISTEN state, itlistens toitsattached channels.
Assoon asavalid startup frame hasbeen received theINITIALIZE_SCHEDULE state isentered. Ifthe
clock synchronization cansuccessfully receive amatching second valid startup frame andderive a
schedule from this, theINTEGRATION_CONSISTENCY_CHECK state isentered.
InINTEGRATION_CONSISTENCY_CHECK state itisverified thattheclock correction canbeperformed
correctly andthatenough coldstart nodes (atleast 2)send startup frames thatagree with thenodes own
schedule. Clock correction isactivated, andifanyerrors aresignaled, theintegration attempt isaborted.
During thefirsteven cycle inthisstate, either twovalid startup frames orthestartup frame ofthenode
thatthisnode hasintegrated onmust bereceived; otherwise thenode aborts theintegration attempt.
During thefirstdouble-cycle inthisstate, either twovalid startup frame pairs orthestartup frame pairof
thenode thatthisnode hasintegrated onmust bereceived; otherwise thenode aborts theintegration
attempt.

<!-- Page 1242 -->

Module Operation www.ti.com
1242 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleIfafter thefirstdouble-cycle less than twovalid startup frames arereceived within aneven cycle, orless
than twovalid startup frame pairs arereceived within adouble-cycle, thestartup attempt isaborted.
Nodes inthisstate need toseetwovalid startup frame pairs fortwoconsecutive double-cycles each tobe
allowed toleave STARTUP andenter NORMAL_OPERATION. Consequently, they leave startup atleast
onedouble-cycle after thenode thatinitiated thecoldstart andonly attheendofacycle with anoddcycle
number.
26.2.6.8 NORMAL_ACTIVE State
Assoon asthenode thattransmitted thefirstCAS symbol (resolving thepotential access conflict and
entering STARTUP through thecoldstart path) andoneadditional node have entered the
NORMAL_ACTIVE state, thestartup phase forthecluster hasfinished. IntheNORMAL_ACTIVE state, all
configured messages arescheduled fortransmission. This includes alldata frames aswell asthesync
frames. Rate andoffset measurement isstarted inalleven cycles (even/odd cycle pairs required).
InNORMAL_ACTIVE state thecommunication controller supports regular communication functions:
*The communication controller performs transmissions andreception ontheFlexRay busasconfigured
*Clock synchronization isrunning
*The host interface isoperational
The communication controller exits from thatstate to:
*HALT state bywriting SUCC1.CMD(3-0) =0110 (HALT command, attheendofthecurrent cycle)
*HALT state bywriting SUCC1.CMD(3-0) =0111 (FREEZE command, immediately)
*HALT state duetochange oftheerror state from ACTIVE toCOMM_HALT
*NORMAL_PASSIVE state duetochange oftheerror state from ACTIVE toPASSIVE
*READY state bywriting SUCC1.CMD(3-0) =0010 (READY command)
26.2.6.9 NORMAL_PASSIVE State
NORMAL_PASSIVE state isentered from NORMAL_ACTIVE state when theerror state changes from
ACTIVE toPASSIVE.
InNORMAL_PASSIVE state, thenode isable toreceive allframes (node isfully synchronized and
performs clock synchronization). Contrary totheNORMAL_ACTIVE state, thenode does notactively
participate incommunication, thatis,neither symbols norframes aretransmitted.
InNORMAL_PASSIVE state:
*The communication controller performs reception ontheFlexRay bus
*The communication controller does nottransmit anyframes orsymbols ontheFlexRay bus
*Clock synchronization isrunning
*The host interface isoperational
The communication controller exits from thisstate to
*HALT state bywriting SUCC1.CMD(3-0) =0110 (HALT command, attheendofthecurrent cycle)
*HALT state bywriting SUCC1.CMD(3-0) =0111 (FREEZE command, immediately)
*HALT state duetochange oftheerror state from PASSIVE toCOMM_HALT
*NORMAL_ACTIVE state duetochange oftheerror state from PASSIVE toACTIVE. The transition
takes place when CCEV.PTAC(4-0) equals SUCC1.PTA(4-0) -1.
*ToREADY state bywriting SUCC1.CMD(3-0) =0010 (READY command)
26.2.6.10 HALT State
Inthisstate allcommunication (reception andtransmission) isstopped.
The communication controller enters thisstate:
*Bywriting SUCC1.CMD(3-0) =0110 (HALT command) while thecommunication controller isin
NORMAL_ACTIVE orNORMAL_PASSIVE state

<!-- Page 1243 -->

www.ti.com Module Operation
1243 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module*Bywriting SUCC1.CMD(3-0) =0111 (FREEZE command) from allstates
*When exiting from NORMAL_ACTIVE state because theclock correction failed counter reached the
"maximum without clock correction fatal"limit andSUCC1.HCSE isset
*When exiting from NORMAL_PASSIVE state because theclock correction failed counter reached the
"maximum without clock correction fatal"limit andSUCC1.HCSE isset
The communication controller exits from thisstate toDEFAULT_CONFIG state
*Bywriting SUCC1.CMD(3-0) =0001 (CONFIG command)
When thecommunication controller transits from HALT state toDEFAULT_CONFIG state allconfiguration
andstatus data ismaintained foranalyzing purposes.
When thehost writes SUCC1.CMD(3-0) =0110 (HALT command), thecommunication controller sets bit
CCSV.HRQ andenters HALT state atnext endofcycle.
When thehost writes SUCC1.CMD(3-0) =0111 (FREEZE command), thecommunication controller enters
HALT state immediately andsets theCCSV.FSI bitinthecommunication controller status vector register.
The POC state from which thetransition toHALT state took place canberead from CCSV.PSL(5-0).
26.2.7 Network Management
The accrued network management (NM) vector islocated intheNetwork Management Registers
(NMV1/2/3). The communication controller performs alogical ORoperation over allNMvectors outofall
received valid NMframes with thePayload Preamble Indicator (PPI) bitset.Only astatic frame may be
configured tohold NMinformation. The communication controller updates theNMvector attheendof
each cycle.
The length oftheNMvector canbeconfigured from 0to12bytes byNEMC.NML(3-0). The NMvector
length must beconfigured identically inallnodes ofacluster.
Toconfigure atransmit buffer tosend FlexRay frames with thePPIbitset,thePPIT bitintheheader
section ofthecorresponding transmit buffer hastobesetWRHS1.PPIT. Inaddition thehost hastowrite
theNMinformation tothedata section ofthecorresponding transmit buffer.
The evaluation oftheNMvector hastobedone bytheapplication running onthehost.
NOTE: Incase amessage buffer isconfigured fortransmission /reception ofnetwork management
frames, thepayload length configured inheader 2ofthatmessage buffer should beequal or
greater than thelength oftheNMvector configured byNEMC.NML(3-0).When the
Communication Controller transits toHALT state, thecycle count isnotincremented and
therefore theNMvector isnotupdated. Inthiscase NMV1/2/3 holds thevalue from thecycle
before.
26.2.8 Filtering andMasking
Filtering isdone bycomparison oftheconfiguration ofassigned message buffers against current slotand
cycle counter values andchannel ID(channel A,B).Amessage buffer isonly updated /transmitted ifthe
required matches occur.
Filtering isdone on:
*Slot counter
*Cycle counter
*Channel ID
The following filter combinations foracceptance /transmit filtering areallowed:
*Slot counter +Channel ID
*Slot counter +Cycle counter +Channel ID
Allconfigured filters must match inorder tostore areceived message inamessage buffer.

<!-- Page 1244 -->

Module Operation www.ti.com
1244 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: FortheFIFO theacceptance filter isconfigured bytheFIFO Rejection Filter andtheFIFO
Rejection Filter mask.
Amessage willbetransmitted inthetime slotcorresponding totheconfigured frame IDontheconfigured
channel(s). Ifcycle counter filtering isenabled theconfigured cycle filter value must also match.
26.2.8.1 Slot Counter Filtering
Every transmit andreceive buffer contains aframe IDstored intheheader section. This frame IDis
compared against thecurrent slotcounter value inorder toassign receive andtransmit buffers tothe
corresponding slot.
Iftwoormore message buffers areconfigured with thesame frame IDandchannel ID,andifthey have a
matching cycle counter filter value forthesame slot, then themessage buffer with thelowest message
buffer number isused.
26.2.8.2 Cycle Counter Filtering
Cycle counter filtering isbased onthenotion ofacycle set.Forfiltering purposes, amatch isdetected if
anyoneoftheelements ofthecycle setismatched. The cycle setisdefined bythecycle code field inthe
header section 1ofeach message buffer.
Ifmessage buffer 0or1isconfigured tohold thestartup /sync frame orthesingle slotframe bybits
TXST, TXSY, andTSM ofSUC Configuration Register 1,cycle counter filtering formessage buffer 0or1
respectively shall bedisabled.
NOTE: Sharing ofastatic time slotbycycle counter filtering between different nodes ofaFlexRay
network isnotallowed.
The setofcycle numbers belonging toacycle setisdetermined asdescribed inTable 26-8 .
Table 26-8. Definition ofCycle Set
Cycle Code Matching Cycle Counter Values
0b000000x Allcycles
0b00000 1c Every second cycle at(cycle count)mod2 =c
0b0000 1cc Every fourth cycle at(cycle count)mod4 =cc
0b000 1ccc Every eighth cycle at(cycle count)mod8 =ccc
0b00 1cccc Every sixteenth cycle at(cycle count)mod16 =cccc
0b01ccccc Every thirty-second cycle at(cycle count)mod32 =ccccc
0b1cccccc Every sixty-fourth cycle at(cycle count)mod64 =cccccc
Table 26-9 gives some examples forvalid cycle sets tobeused forcycle counter filtering.
Table 26-9. Examples forValid Cycle Sets
Cycle Code Matching Cycle Counter Values
0b00000 11 1-3-5-7- ....-63↵
0b0000 100 0-4-8-12- ....-60↵
0b000 1110 6-14-22-30- ....-62↵
0b00 11000 8-24-40-56 ↵
0b0100011 3-35↵
0b1001001 9↵

<!-- Page 1245 -->

www.ti.com Module Operation
1245 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleThe received message isstored only ifthecycle counter value ofthecycle during which themessage is
received matches anelement ofthereceive buffer 'scycle set.Other filter criteria must also bemet.
The content ofatransmit buffer istransmitted ontheconfigured channel(s) when anelement ofthecycle
setmatches thecurrent cycle counter value. Other filter criteria must also bemet.
26.2.8.3 Channel IDFiltering
There isa2-bit channel filtering field (CHA, CHB) located intheheader section ofeach message buffer in
themessage RAM. Itserves asafilter forreceive buffers, andasacontrol field fortransmit buffers (see
Table 26-10 ).
Table 26-10. Channel Filtering Configuration
CHA CHBTransmit Buffer Receive Buffer
Transmit frame Store valid receive frame
1 1 Onboth channels (static segment only) Received onchannel AorB(store firstsemantically
valid frame, static segment only)
1 0 Onchannel A Received onchannel A
0 1 Onchannel B Received onchannel B
0 0 Notransmission Ignore frame
The contents ofatransmit buffer istransmitted onthechannels specified inthechannel filtering field when
theslotcounter filtering andcycle counter filtering criteria arealso met. Only instatic segment atransmit
buffer may besetupfortransmission onboth channels (CHA andCHB set).
Valid received frames arestored ifthey arereceived onthechannels specified inthechannel filtering field
when theslotcounter filtering andcycle counter filtering criteria arealso met. Only instatic segment a
receive buffer may besetup forreception onboth channels (CHA andCHB set).
NOTE: Ifamessage buffer isconfigured forthedynamic segment andboth bitsofthechannel
filtering field aresetto1,noframes aretransmitted andreceived frames areignored (same
function asCHA =CHB =0)
26.2.8.4 FIFO Filtering
ForFIFO filtering there isonerejection filter andonerejection filter mask available. The FIFO filter
consists ofchannel filter FRF.CH(1-0), frame IDfilter FRF.FID(10-0), andcycle counter filter FRF.CYF(6-
0).Registers FRF andFRFM canbeconfigured inDEFAULT_CONFIG orCONFIG state only. The filter
configuration intheheader section ofmessage buffers belonging totheFIFO isignored.
The 7-bit cycle counter filter determines thecycle settowhich frame IDandchannel rejection filter are
applied. Incycles notbelonging tothecycle setspecified byFRF.CYF(6-0), allframes arerejected.
Avalid received frame isstored intheFIFO ifchannel ID,frame ID,andcycle counter arenotrejected by
theconfigured rejection filter andrejection filter mask, andifthere isnomatching dedicated receive buffer.

<!-- Page 1246 -->

Module Operation www.ti.com
1246 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.9 Transmit Process
26.2.9.1 Static Segment
Forthestatic segment, ifthere areseveral messages pending fortransmission, themessage with the
frame IDcorresponding tothenext sending slotisselected fortransmission.
The data section oftransmit buffers assigned tothestatic segment canbeupdated until theendofthe
preceding time slot. This means thatatransfer from theinput buffer hastobestarted bywriting tothe
Input Buffer Command Request Register latest atthistime.
26.2.9.2 Dynamic Segment
Inthedynamic segment, ifseveral messages arepending, themessage with thehighest priority (lowest
frame ID)isselected next. Inthedynamic segment different slotcounter sequences onchannel Aand
channel Barepossible (concurrent sending ofdifferent frame IDsonboth channels).
The data section oftransmit buffers assigned tothedynamic segment canbeupdated until theendofthe
preceding slot. This means thatatransfer from theinput buffer hastobestarted bywriting totheInput
Buffer Command Request Register latest atthistime.
The start oflatest transmit configured byMHDC.SLT(12-0) intheMHD configuration register 1defines the
maximum minislot value allowed before inhibiting new frame transmission inthedynamic segment ofthe
current cycle.
26.2.9.3 Transmit Buffers
Communication Controller message buffers canbeconfigured astransmit buffers byprogramming bit
CFG intheheader section ofthecorresponding message buffer to1inWRHS1.
There exist thefollowing possibilities toassign atransmit buffer tothecommunication controller channels:
*Static segment:
-channel Aorchannel B
-channel Aandchannel B
*Dynamic segment:
-channel Aorchannel B
Message buffer 0or1isdedicated tohold thestartup frame, thesync frame, orthedesignated single slot
frame asconfigured bySUCC1.TXST, SUCC1.TXSY, andSUCC1.TSM intheSUC Configuration register
1.Inthiscase itcanbereconfigured inDEFAULT_CONFIG orCONFIG state only. This ensures thatany
node transmits atmost onestartup /sync frame percommunication cycle. Transmission ofstartup /sync
frames from other message buffers isnotpossible.
Allother message buffers configured fortransmission instatic ordynamic segment arereconfigurable
during runtime depending ontheconfiguration ofMRC.SEC(1-0). Due totheorganization ofthedata
partition inthemessage RAM (reference bydata pointer), reconfiguration oftheconfigured payload length
andthedata pointer intheheader section ofamessage buffer may lead toerroneous configurations.
Ifamessage buffer isreconfigured (header section updated) during runtime, itmay happen thatthis
message buffer isnotsent outinthecurrently active communication cycle.
The communication controller does nothave thecapability tocalculate theheader CRC. The host is
supposed toprovide theheader CRCs foralltransmit buffers. Ifnetwork management isrequired thehost
hastosetthePPIT bitintheheader section ofthecorresponding message buffer to1andwrite the
network management information tothedata section ofthemessage buffer.
The payload length field configures thedata payload length in2-byte words. Iftheconfigured payload
length ofastatic transmit buffer isshorter than thepayload length configured forthestatic segment by
MHDC.SFDL(6-0) inthemessage handler configuration register 1,thecommunication controller generates
padding bytes toensure thatframes have proper physical length. The padding pattern islogical 0.

<!-- Page 1247 -->

www.ti.com Module Operation
1247 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: Incase ofanoddpayload length (PLC=1,3,5,...) theapplication needs towrite zeros tothe
last16bitofthemessage buffers data section toensure thatthepadding pattern isallzeros.
Each transmit buffer provides atransmission mode flagTXM thatallows thehost toconfigure the
transmission mode forthetransmit buffer. Ifthisbitisset,thetransmitter operates inthesingle-shot
mode. Ifthisbitiscleared, thetransmitter operates inthecontinuous mode.
Insingle-shot mode theCommunication Controller resets thecorresponding TXR flagafter transmission
hascompleted after which thehost may update thetransmit buffer.
Incontinuous mode, theCommunication Controller does notreset thecorresponding transmission request
flagTXR after successful transmission. Inthiscase aframe issent outeach time thefilter criteria match.
The TXR flagcanbereset bytheHost bywriting thecorresponding message buffer number totheIBCR
register while bitIBCM.STXRH issetto0.
Iftwoormore transmit buffers meet thefilter criteria simultaneously, thetransmit buffer with thelowest
message buffer number willbetransmitted inthecorresponding slot.
26.2.9.4 Frame Transmission
The following steps arerequired toprepare amessage buffer fortransmission:
*Configure thetransmit buffer intheMessage RAM through WRHS1, WRHS2, andWRHS3
*Write thedata section ofthetransmit buffer through WRDSn
*Transfer theconfiguration andmessage data from Input Buffer totheMessage RAM bywriting the
number ofthetarget message buffer toregister IBCR
*Ifconfigured intheInput Buffer Command Mask (IBCM) register theTransmission request flag(TXR)
forthecorresponding message buffer willbesetassoon asthetransfer hascompleted, andthe
message buffer isready fortransmission.
*Check whether themessage buffer hasbeen transmitted bychecking theTXR bits(TXR =0)inthe
Transmission request 1,2,3,4 registers (single-shot mode only).
After transmission hascompleted, thecorresponding TXR flagintheTransmission request 1,2,3,4 register
isreset (single- shot mode), and, ifbitMBI intheheader section ofthemessage buffer isset,flagSIR.TXI
intheStatus Interrupt register issetto1.Ifenabled, aninterrupt isgenerated.
26.2.9.5 Null Frame Transmission
Ifinstatic segment thehost does notsetthetransmission request flagbefore transmit time, andifthere is
noother transmit buffer with matching filter criteria, thecommunication controller transmits anullframe
with thenullframe indication bitsetandthepayload data cleared to0.
Inthefollowing cases thecommunication controller transmits anullframe:
*Ifthemessage buffer with thelowest message buffer number matching thefilter criteria does nothave
itstransmission request flagset(TXR =0).
*Notransmit buffer configured fortheslothasacycle counter filter thatmatches thecurrent cycle. In
thiscase, nomessage buffer status MBS isupdated.
Null frames arenottransmitted inthedynamic segment.

<!-- Page 1248 -->

Module Operation www.ti.com
1248 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.10 Receive Process
26.2.10.1 Dedicated Receive Buffers
Aportion oftheCommunication Controller message buffers canbeconfigured asdedicated receive
buffers byprogramming bitCFG intheheader section ofthecorresponding message buffer to0.This can
bedone through theWrite Header Section 1register.
The following possibilities exist toassign areceive buffer totheCommunication Controller channels:
*Static segment:
-channel Aorchannel B
-channel Aandchannel B(the communication controller stores thefirstsemantically valid frame)
*Dynamic segment:
-channel Aorchannel B
The communication controller transfers payload data ofvalid received messages from theshift registers of
theFlexRay protocol controller (channel AorB)tothereceive buffer with thematching filter configuration.
Areceive buffer stores allframe elements except theframe CRC.
Allmessage buffers configured forreception instatic ordynamic segment arereconfigurable during
runtime depending ontheconfiguration ofMRC.SEC(1-0) oftheMessage RAM Configuration register. Ifa
message buffer isreconfigured (header section updated) during runtime itmay happen thatinthe
currently active communication cycle areceived message islost.
Iftwoormore receive buffers meet thefilter criteria simultaneously, thereceive buffer with thelowest
message buffer number isupdated with thereceived message.
26.2.10.2 Frame Reception
The following steps arerequired toprepare adedicated message buffer forreception:
*Configure thereceive buffer intheMessage RAM through WRHS1, WRHS2, andWRHS3
*Transfer theconfiguration from input buffer tothemessage RAM bywriting thenumber ofthetarget
message buffer totheInput Buffer Command Request (IBCR) register.
Once these steps areperformed, themessage buffer functions asanactive receive buffer andparticipates
intheinternal acceptance filtering process, which takes place every time thecommunication controller
receives amessage. The firstmatching receive buffer isupdated from thereceived message.
Ifavalid payload segment was stored inthedata section ofamessage buffer, thecorresponding NDflag
intheNDAT1,2,3,4 registers isset,and, ifbitMBI intheheader section ofthatmessage buffer isset,flag
SIR.RXI intheStatus Interrupt Register issetto1.Ifenabled, aninterrupt isgenerated.
Incase thatbitNDwas already setwhen theMessage Handler updates themessage buffer, bit
MBS.MLST ofthecorresponding message buffer issetandtheunprocessed message data islost.
Ifnoframe, anullframe, oracorrupted frame isreceived inaslot, thedata section ofthemessage buffer
configured forthisslotisnotupdated. Inthiscase only theflags inthecorresponding message buffer
status (MBS) isupdated.
When theMessage Handler changes themessage buffer status MBS intheheader section ofamessage
buffer, thecorresponding MBC flagintheMessage Buffer Status Changed 1,2,3 or4register isset,andif
bitMBI intheheader section ofthatmessage buffer isset,flagSIR.MBSI intheStatus Interrupt Register
issetto1.Ifenabled aninterrupt isgenerated.
Ifthepayload length ofareceived frame PLR(6-0) islonger than thevalue programmed byPLC(6-0) in
theheader section ofthecorresponding message buffer, thedata field stored inthemessage buffer is
truncated tothatlength.
NOTE: The NDandMBS flags areautomatically cleared bythemessage handler when thepayload
data andtheheader ofareceived message have been transferred totheoutput buffer,
respectively.

<!-- Page 1249 -->

www.ti.com Module Operation
1249 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.10.3 Null Frame Reception
The payload segment ofareceived nullframe isnotcopied intothematching dedicated receive buffer. Ifa
nullframe hasbeen received, only themessage buffer status MBS ofthematching message buffer is
updated from thereceived nullframe. Allbitsinheader 2and3ofthematching message buffer remain
unchanged. They areupdated from received data frames only.
26.2.11 FIFO Function
26.2.11.1 Description
Agroup ofthemessage buffers canbeconfigured asacyclic First-In-First-Out (FIFO) buffer. The group of
message buffers belonging totheFIFO iscontiguous intheregister map starting with themessage buffer
referenced byMRC.FFB(7-0) andending with themessage buffer referenced byMRC.LCB(7-0) inthe
message RAM configuration register. Upto128message buffers canbeassigned totheFIFO.
Every valid incoming message notmatching with anydedicated receive buffer butpassing the
programmable FIFO filter isstored intotheFIFO. Inthiscase frame ID,payload length, receive cycle
count, andthemessage buffer status MBS oftheaddressed FIFO message buffer areoverwritten with
frame ID,payload length, receive cycle count, andthestatus from thereceived frame. BitSIR.RFNE inthe
status interrupt register shows thattheFIFO isnotempty, bitSIR.RFCL issetwhen thereceive FIFO fill
level FSR.RFFL(7-0) isequal orgreater than thecritical level asconfigured byFCL.CL(7-0), bitEIR.RFO
shows thataFIFO overrun hasbeen detected. Ifenabled, interrupts aregenerated.
Ifnullframes arenotrejected bytheFIFO rejection filter, thenullframes willbetreated likedata frames
when they arestored intotheFIFO.
There aretwoindex registers associated with theFIFO. The PUT Index register (PIDX) isanindex tothe
next available location intheFIFO. When anew message hasbeen received itiswritten intothemessage
buffer addressed bythePIDX register. The PIDX register isthen incremented andaddresses thenext
available message buffer. IfthePIDX register isincremented past thehighest numbered message buffer
oftheFIFO, thePIDX register isloaded with thenumber ofthefirst(lowest numbered) message buffer in
theFIFO chain. The GET Index register (GIDX) isused toaddress thenext message buffer oftheFIFO to
beread. The GIDX register isincremented after transfer ofthecontents ofamessage buffer belonging to
theFIFO totheoutput buffer. The PUT Index register andtheGET Index register arenotmemory mapped
andarenotaccessible bythehost CPU.
The FIFO iscompletely filled when thePUT index (PIDX) reaches thevalue oftheGET index (GIDX).
When thenext message iswritten totheFIFO before theoldest message hasbeen read, both PUT index
andGET index areincremented andthenew message overwrites theoldest message intheFIFO. This
willsetFIFO overrun flagEIR.RFO intheerror interrupt register.
AFIFO notempty status isdetected when thePUT index (PIDX) differs from theGET index (GIDX). In
thiscase flagSIR.RFNE isset.This indicates thatthere isatleast onereceived message intheFIFO. The
FIFO empty, FIFO notempty, andtheFIFO overrun states areexplained inFigure 26-16 forathree
message buffer FIFO.
The programmable FIFO Rejection Filter register (FRF) defines afilter pattern formessages tobe
rejected. The FIFO rejection filter consists ofchannel filter, frame IDfilter, andcycle counter filter. Ifbit
FRF.RSS issetto1(default), allmessages received inthestatic segment arerejected bytheFIFO. Ifbit
FRF.RNF issetto1(default), received nullframes arenotstored intheFIFO.
The FIFO Rejection Filter mask register (FRFM) specifies which bitsoftheframe IDfilter intheFIFO
Rejection Filter register aremarked don'tcare forrejection filtering.

<!-- Page 1250 -->

1 2 3
A - -Buffers
Messages1 2 3
A B CBuffers
Messages1 3 2
- - -Buffers
MessagesPIDX
(store next)
GIDX
(read oldest)GIDX
(read oldest)GIDX
(read oldest)PIDX
(store next)PIDX
(store next)
- PIDX incremented last
- Next received message
will be stored into buffer 1
- If buffer 1 has not been read
before message A is lostDFIFO empty FIFO not empty FIFO overrun
Module Operation www.ti.com
1250 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-16. FIFO Status: Empty, NotEmpty, andOverrun
26.2.11.2 Configuration oftheFIFO
(Re)configuration ofmessage buffers belonging totheFIFO isonly possible when theCommunication
Controller isinDEFAULT_CONFIG orCONFIG state. While theCommunication Controller isin
DEFAULT_CONFIG orCONFIG state, theFIFO function isnotavailable.
Forallmessage buffers belonging totheFIFO should have thesame payload length configured in
WRHS2.PLC(6-0) oftheWrite Header Section 2register. The data pointer tothefirst32-bit word inthe
data section ofthecorresponding message buffer hastobeconfigured byWRHS3.DP(10-0).
Allinformation required foracceptance filtering istaken from theFIFO rejection filter andtheFIFO
rejection filter mask. With theexception ofDPandPLC, thevalues configured intheheader sections of
themessage buffers belonging totheFIFO areirrelevant.
NOTE: Itisrecommended toprogram theMBI bitsofthemessage buffers belonging totheFIFO to
0byWRHS1.MBI toavoid RXinterrupts tobegenerated.
Ifthepayload length ofareceived frame islonger than thevalue programmed by
WRHS2.PLC(6-0) intheheader section ofthecorresponding message buffer, thedata field
stored inamessage buffer oftheFIFO istruncated tothatlength.
26.2.11.3 Access totheFIFO
ForFIFO access outside DEFAULT_CONFIG andCONFIG state, theHost hastotrigger atransfer from
theMessage RAM totheOutput Buffer bywriting thenumber ofthefirstmessage buffer oftheFIFO
(referenced byMRC.FFB(7-0)) totheOutput Buffer Command Request (OBCR) register. The message
handler then transfers themessage buffer addressed bytheGET Index register (GIDX) totheoutput
buffer. After thistransfer theGET Index register (GIDX) isincremented.
26.2.12 Message Handling
The message handler controls data transfers between theinput /output buffer andthemessage RAM and
between themessage RAM andthetwotransient buffer RAMs. Allaccesses totheinternal RAMs are32
bitaccesses.

<!-- Page 1251 -->

www.ti.com Module Operation
1251 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleAccess tothemessage buffers stored inthemessage RAM isdone under control ofthemessage handler
state machine. This avoids conflicts between accesses ofthetwoprotocol controllers andthehost CPU to
themessage RAM.
Frame IDsofmessage buffers assigned tothestatic segment have tobeintherange from 1to
GTU7.NSS(9-0) asconfigured intheGTU configuration register 7.Frame IDsofmessage buffers
assigned tothedynamic segment have tobeintherange from GTU7.NSS(9-0) +1to2047.
Received messages with nomatching dedicated receive buffer (static ordynamic segment) arestored in
thereceive FIFO (ifconfigured) ifthey pass theFIFO rejection filter.
26.2.12.1 Reconfiguration ofMessage Buffers
Incase thatanapplication needs tooperate with more than 128different messages, static anddynamic
message buffers may bereconfigured during FlexRay operation. This isdone byupdating theheader
section ofthecorresponding message buffer through Input Buffer registers WRHS1,2,3.
Reconfiguration hastobeenabled through control bitsMRC.SEC(1-0) intheMessage RAM Configuration
register.
Ifamessage buffer hasnotbeen transmitted /updated from areceived frame before reconfiguration
starts, thecorresponding message islost.
The point intime when areconfigured message buffer isready fortransmission /reception according to
thereconfigured frame IDdepends onthecurrent state oftheslotcounter when theupdate oftheheader
section hascompleted. Therefore itmay happen thatareconfigured message buffer isnottransmitted /
updated from areceived frame inthecycle where itwas reconfigured.
The Message RAM isscanned according toTable 26-11 .
Table 26-11. Scan ofMessage RAM
Start ofScan inSlot Scan forSlots
1 2...15, 1(next cycle)
8 16...23, 1(next cycle)
16 24...31, 1(next cycle)
24 32...39, 1(next cycle)
.... ...
AMessage RAM scan isterminated with thestart ofNITirrespective ofit'scompletion. The scan ofthe
Message RAM forslots 2to15starts atthebeginning ofslot1ofthecurrent cycle. The scan ofthe
Message RAM forslot1isdone inthecycle before bychecking inparallel toeach scan oftheMessage
RAM whether there isamessage buffer configured forslot1ofthenext cycle.
The number ofthefirstdynamic message buffer isconfigured byMRC.FDB(7-0) intheMessage RAM
Configuration register. Incase aMessage RAM scan starts while theCommunication Controller isin
dynamic segment, thescan starts with themessage buffer number configured byMRC.FDB(7-0).
Incase amessage buffer needs tobereconfigured tobeused inslot1ofthenext cycle, thefollowing has
tobeconsidered:
*Ifthemessage buffer tobereconfigured forslot1ispart oftheStatic Buffers, itwillonly befound ifit
isreconfigured before thelastMessage RAM scan inthestatic segment ofthecurrent cycle evaluates
thismessage buffer.
*Ifthemessage buffer tobereconfigured forslot1ispart oftheStatic +Dynamic Buffers, itwillbe
found ifitisreconfigured before thelastMessage RAM scan inthecurrent cycle evaluates this
message buffer.
*The start ofNITterminates theMessage RAM scan. Incase theMessage RAM scan hasnot
evaluated thereconfigured message buffer until thispoint intime, themessage buffer willnotbe
considered forthenext cycle.

<!-- Page 1252 -->

Data(31-0)Data(31-0)Address
Data(31-0) Data(31-0)Address-Input Buffer
Message handlerAddressOutput BufferDecoder
and Control
ControlHost CPU
Header Partition
Data Partition
Message RAM[Shadow] [Shadow]
Address
Module Operation www.ti.com
1252 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: Reconfiguration ofmessage buffers may lead totheloss ofmessages andtherefore hasto
beused very carefully. Inworst case (reconfiguration inconsecutive cycles) itmay happen
thatamessage buffer isnever transmitted /updated from areceived frame.
26.2.12.2 Host Access toMessage RAM
The message transfer between input buffer andmessage RAM aswell asbetween message RAM and
output buffer istriggered bythehost CPU bywriting thenumber ofthetarget /source message buffer to
beaccessed totheinput oroutput buffer command request register (IBCR/OBCR).
The input /output buffer command mask registers canbeused towrite /read header anddata section of
theselected message buffer separately.
IfbitIBCM.STXR intheinput buffer command mask register isset(STXR =1),thetransmission request
flagTXR oftheselected message buffer isautomatically setafter themessage buffer hasbeen updated.
IfbitIBCM.STXR intheinput buffer command mask register isreset (STXR =0),thetransmission request
flagTXR oftheselected message buffer isreset. This canbeused tostop transmission from message
buffers operated incontinuous mode.
Input buffer (IBF) andtheoutput buffer (OBF) arebuilt upasadouble buffer structure. One halfofthis
double buffer structure isaccessible bythehost CPU (IBF host /OBF host), while theother half(IBF
shadow /OBF shadow) isaccessed bythemessage handler fordata transfers between IBF/OBF and
message RAM.
Figure 26-17. Host Access toMessage RAM

<!-- Page 1253 -->

IBCM IBCR
swap swap
10 21716 18
10 2 43 52019 21 1716 18 31
15 622
HostMessage
RAMFlexRay
IBFIBF
Sha-
dow
IBF = Input BufferHost
www.ti.com Module Operation
1253 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.12.2.1 Data Transfer from Input Buffer toMessage RAM
Toconfigure /update amessage buffer inthemessage RAM, thehost hastowrite thedata toWRDSn
andtheheader toWRHS1 ...3.The specific action isselected byconfiguring theinput buffer command
mask IBCM.
When thehost writes thenumber ofthetarget message buffer inthemessage RAM toIBCR.IBRH(6-0) in
theinput buffer command request register IBCR, IBFhost andIBFshadow areswapped (Figure 26-18 ).
Figure 26-18. Double Buffer Structure Input Buffer
Figure 26-19. Swapping ofIBCM andIBCR Bits
With thiswrite operation theIBCR.IBSYS bitintheinput buffer command request register issetto1.The
message handler then starts totransfer thecontents ofIBFshadow tothemessage buffer inthemessage
RAM selected byIBCR.IBRS(6-0).
While themessage handler transfers thedata from IBFshadow tothetarget message buffer inthe
message RAM, thehost may write thenext message toIBFhost. After thetransfer between IBFshadow
andthemessage RAM hascompleted, theIBCR.IBSYS bitissetback to0andthenext transfer tothe
message RAM may bestarted bythehost bywriting thecorresponding target message buffer number to
IBCR.IBRH(6-0) intheinput buffer command request register.
Ifawrite access toIBCR.IBRH(6-0) occurs while IBCR.IBSYS is1,IBCR.IBSYH issetto1.After
completion oftheongoing data transfer from IBFshadow tothemessage RAM, IBFhost andIBFshadow
areswapped, IBCR.IBSYH isreset to0,IBCR.IBSYS remains setto1,andthenext transfer tothe
message RAM isstarted. Inaddition themessage buffer numbers stored under IBCR.IBRH(6-0) and
IBCR.IBRS(6-0) andthecommand mask flags arealso swapped.

<!-- Page 1254 -->

Module Operation www.ti.com
1254 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleExample ofa8/16/32-bit host access sequence:
*Configure /update n-th message buffer through IBF
*Wait until IBCR.IBSYH isreset
*Write data section toWRDSn
*Write header section toWRHS1,2,3
*Write command mask: write IBCM.STXRH, IBCM.LHSH, IBCM.LDSH
*Demand data transfer totarget message buffer: write IBCR.IBRH(6-0)
Configure /update further message buffer through IBFinthesame way.
NOTE: Any write access toIBFwhile IBCR.IBSYH is1willseterror flagEIR.IIBA intheError
Interrupt Register to1.Inthiscase thewrite access hasnoeffect.
Table 26-12. Assignment ofInput Buffer Command Mask Bits
Position Access Bit Function
18 r STXRS SetTransmission Request shadow ongoing orfinished
17 r LDSS Load Data Section shadow ongoing orfinished
16 r LHSS Load Header Section shadow ongoing orfinished
2 r/w STXRH SetTransmission Request Host
1 r/w LDSH Load Data Section Host
0 r/w LHSH Load Header Section Host
Table 26-13. Assignment ofInput Buffer Command Request Bits
Position Access Bit Function
31 r IBSYS IBFBusy Shadow, signals ongoing transfer from IBFshadow tomessage RAM
22-16 r IBRS(6-0) IBFRequest Shadow, number ofmessage buffer currently /lastupdated
15 r IBSYH IBFBusy Host, transfer request pending formessage buffer referenced byIBRH(6-0)
6-0 r/w IBRH(6-0) IBFRequest Host, number ofmessage buffer tobeupdated next
26.2.12.2.2 Data Transfer from Message RAM toOutput Buffer
Toread outamessage buffer from themessage RAM, thehost hastowrite totheoutput buffer command
mask andcommand request register totrigger thedata transfer. After atransfer hascompleted thehost
canread thetransferred data from theRDDSn, RDHS1,2,3, andMBS.

<!-- Page 1255 -->

OBCM
OBCR
10 2 43 52019 21 1716 1810
151716
8910 2 43 5view
request10view
request
internal storageinternal storage
622
6
HostMessage
RAMFlexRay
OBFOBF
Sha-
dow
OBF = Output BufferHost
www.ti.com Module Operation
1255 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-20. Double Buffer Structure Output Buffer
OBF host andOBF shadow aswell asbitsOBCM.RHSS, OBCM.RDSS, OBCM.RHSH, OBCM.RDSH
from theoutput buffer command mask register andbitsOBCM.OBRS(6-0), OBCM.OBRH(6-0) from the
output buffer command request register areswapped under control ofbitsOBCR.VIEW andOBCR.REQ
from theoutput buffer command request register.
Writing bitOBCR.REQ intheoutput buffer command request register to1copies bitsOBCM.RHSS,
OBCM.RDSS from theoutput buffer command mask register andbitsOBCR.OBRS(6-0) from theoutput
buffer command request register toaninternal storage (see Figure 26-21 ).
After setting OBCR.REQ to1,OBCR.OBSYS issetto1,andthetransfer ofthemessage buffer selected
byOBCR.OBRS(6-0) from themessage RAM toOBF shadow isstarted. After thetransfer between the
message RAM andOBF shadow hascompleted, theOBCR.OBSYS bitissetback to0.BitsOBCR.REQ
andOBCR.VIEW canonly besetto1while OBCR.OBSYS is0.
Figure 26-21. Swapping ofOBCM andOBCR Bits
OBF host andOBF shadow areswapped bysetting bitOBCR.VIEW intheoutput buffer command request
register to1while bitOBCR.OBSYS is0(see Figure 26-20 ).

<!-- Page 1256 -->

Module Operation www.ti.com
1256 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleInaddition bitsOBCR.OBRH(6-0) areswapped with theoutput buffer command request registers internal
storage andbitsOBCM.RHSH, OBCM.RDSH areswapped with theoutput buffer command mask
registers internal storage thus assuring thatthemessage buffer number stored inOBCR.OBRH(6-0) and
themask configuration stored inOBCM.RHSH, OBCM.RDSH matches thetransferred data stored inOBF
host (see Figure 26-21 ).
Now thehost canread thetransferred message buffer from OBF host while themessage handler may
transfer thenext message from themessage RAM toOBF shadow.
NOTE: IfbitsREQ andVIEW aresetto1with thesame write access while OBSYS is0,OBSYS is
automatically setto1andOBF shadow andOBF host areswapped. Additionally mask bits
OBCM.RDSH andOBCM.RHSH areswapped with theregisters internal storage tokeep
them attached tothecorresponding Output Buffer transfer. Afterwards OBRS (6-0) iscopied
totheregister internal storage, mask bitsOBCM.RDSS andOBCM.RHSS arecopied to
register OBCM internal storage, andthetransfer oftheselected message buffer from the
Message RAM toOBF shadow isstarted. While thetransfer isongoing theHost canread
themessage buffer transferred bytheprevious transfer from OBF Host. When thecurrent
transfer between Message RAM andOBF shadow hascompleted, thisissignaled bysetting
OBSYS back to0.
Example ofan8/16/32-bit host access toasingle message buffer:
Ifasingle message buffer hastoberead out,twoseparate write accesses toOBCR.REQ and
OBCR.VIEW arenecessary:
*Wait until OBCR.OBSYS isreset
*Write Output Buffer Command Mask OBCM.RHSS, OBCM.RDSS
*Request transfer ofmessage buffer toOBF Shadow bywriting OBCR.OBRS(6-0) andOBCR.REQ (in
case ofand8-bit Host interface, OBCR.OBRS(6-0) hastobewritten before OBCR.REQ).
*Wait until OBCR.OBSYS isreset
*Toggle OBF Shadow andOBF Host bywriting OBCR.VIEW =1
*Read outtransferred message buffer byreading RDDSn, RDHS1,2,3, andMBS
Example ofan8/16/32-bit host access sequence:
Request transfer of1stmessage buffer toOBF shadow
*Wait until OBCR.OBSYS isreset
*Write Output Buffer Command Mask OBCM.RHSS, OBCM.RDSS for1stmessage buffer
*Request transfer of1stmessage buffer toOBF Shadow bywriting OBCR.OBRS(6-0) andOBCR.REQ
(incase ofan8-bit Host interface, OBCR.OBRS(6-0) hastobewritten before OBCR.REQ).
Toggle OBF Shadow andOBF Host toread out1sttransferred message buffer andrequest transfer of
2ndmessage buffer:
Request transfer of2ndmessage buffer toOBF shadow, read out1stmessage buffer from OBF host
*Wait until OBCR.OBSYS isreset
*Write Output Buffer Command Mask OBCM.RHSS, OBCM.RDSS for2ndmessage buffer
*Toggle OBF Shadow andOBF Host andstart transfer of2ndmessage buffer toOBF Shadow
simultaneously bywriting OBCR.OBRS(6-0) of2ndmessage buffer, OBCR.REQ, andOBCR.VIEW (in
case ofand8-bit Host interface, OBCR.OBRS(6-0) hastobewritten before OBCR.REQ and
OBCR.VIEW).
*Read out1sttransferred message buffer byreading RDDSn, RDHS13, andMBS
Forfurther transfers continue thesame way.
Demand access tolastrequested message buffer without request ofanother message buffer:
*Wait until OBCR.OBSYS isreset
*Demand access tolasttransferred message buffer bywriting OBCR.VIEW
*Read outlasttransferred message buffer byreading RDDSn, RDHS1,2,3, andMBS

<!-- Page 1257 -->

www.ti.com Module Operation
1257 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-14. Assignment ofOutput Buffer Command Mask Bits
Position Access Bit Function
17 r RDSH Read Data Section Host access
16 r RHSH Read Header Section Host access
1 r/w RDSS Read Data Section Shadow
0 r/w RHSS Read Header Section Shadow
Table 26-15. Assignment ofOutput Buffer Command Request Bits
Position Access Bit Function
22-16 r OBRH(6-0) OBF Request Host, number ofmessage buffer available forhost access
15 r OBSYS OBF Busy Shadow, signals ongoing transfer from message RAM toOBF Shadow
9 r/w REQ Request Transfer from message RAM toOBF Shadow
8 r/w VIEW View OBF Shadow, swap OBF Shadow andOBF Host
6-0 r/w OBRS(6-0) OBF Request Shadow, number ofmessage buffer fornext request
26.2.12.3 FlexRay Protocol Controller Access toMessage RAM
The twotransient buffer RAMs (TBF A,B) areused tobuffer thedata fortransfer between thetwoFlexRay
channel protocol controllers andthemessage RAM.
Each transient buffer RAM isbuilt upasadouble buffer, able tostore twocomplete FlexRay messages.
There isalways onebuffer assigned tothecorresponding protocol controller while theother oneis
accessible bythemessage handler.
If,forexample, themessage handler writes thenext message tobesent totransient buffer Tx,the
FlexRay Channel protocol controller canaccess transient buffer Rxtostore themessage itiscurrently
receiving. During transmission ofthemessage stored intransient buffer Tx,themessage handler transfers
thelastreceived message stored intransient buffer Rxtothemessage RAM (ifitpasses acceptance
filtering) andupdates thecorresponding message buffer.
Data transfers between thetransient buffer RAMs andtheshift registers oftheFlexRay channel protocol
controllers aredone inwords of32bit.This enables theuseofa32bitshift register independent ofthe
length oftheFlexRay messages.

<!-- Page 1258 -->

Address-DecoderTransient Buffer TxTransient Buffer RxShift Register
Control
Address Data(31-0)Data(31-0)Txd1 Rxd1
Control
Address Data(31-0)Data(31-0)Txd2 Rxd2
Shift RegisterFlexRay PRT A FlexRay PRT B
Address-DecoderTransient Buffer TxTransient Buffer Rx
Message HandlerTBF A TBF B
Module Operation www.ti.com
1258 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-22. Access toTransient Buffer RAMs
26.2.13 Module RAMs
The FlexRay module contains thefollowing RAM portions:
*Message RAM
*Transient Buffer RAM Channel A(TBF A)
*Transient Buffer RAM Channel B(TBF B)
*Input Buffer (IBF)
*Input Buffer Shadow (IBFS)
*Output Buffer (OBF)
*Output Buffer Shadow (OBFS)
*Transfer Configuration RAM (TCR)
AllRAMs except theTCR arepart oftheCommunication Controller core.
26.2.13.1 Message RAM
Toavoid conflicts between host access tothemessage RAM andFlexRay message reception /
transmission, thehost CPU cannot directly access themessage buffers inthemessage RAM. These
accesses arehandled through theinput andoutput buffers. The message RAM isable tostore upto128
message buffers depending ontheconfigured payload length.
The message RAM hasastructure asshown inFigure 26-23 .
The data partition isallowed tostart atMessage RAM word number: (MRC.LCB +1)*4

<!-- Page 1259 -->

Message RAM
32 bitHeader MB0
Data MB0Header PartitionHeader MB1
Data PartitionHeader MBn
Data MB1
unused2048
words‚
‚
‚
‚
‚
‚
Data MBn
www.ti.com Module Operation
1259 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-23. Configuration Example ofMessage Buffers intheMessage RAM
Header Partition
Stores header segments ofFlexRay frames:
*Supports amaximum of128message buffers
*Each message buffer hasaheader offour 32bitwords
*Header 3ofeach message buffer holds the11-bit data pointer tothecorresponding data section inthe
data partition
Data Partition
Flexible storage ofdata sections with different length. Some maximum values are:
*30message buffers with 254byte data section each
*Or56message buffers with 128byte data section each
*Or128message buffers with 48byte data section each
Restriction: header partition +data partition may notoccupy more than 2048 x32bitwords.
26.2.13.1.1 Header Partition
The elements used forconfiguration ofamessage buffer aswell asthecurrent message buffer status are
stored intheheader partition ofthemessage RAM asshown inFigure 26-24 .Configuration oftheheader
sections ofthemessage buffers isdone through IBF(Write Header Section 1,2,3). Read access tothe
header sections isdone through OBF (read header section 1,2,3 +message buffer status). The data
pointer hastobecalculated bytheprogrammer todefine thestarting point ofthedata section forthe
corresponding message buffer inthedata partition ofthemessage RAM. The data pointer should notbe
modified during runtime. Formessage buffers belonging tothereceive FIFO (re)configuration should be
done inDEFAULT_CONFIG orCONFIG state only.
The header section ofeach message buffer occupies four 32bitwords intheheader partition ofthe
message RAM. The header ofmessage buffer 0starts with thefirstword inthemessage RAM.
Fortransmit buffers theHeader CRC hastobecalculated bythehost CPU.

<!-- Page 1260 -->

Bit
Word31 30   29  28   27  26   2 5 24 23   22  21   20  19   18   1 7 16   15  14   13  12   1 1 10 9 8 7 6 5     4 3 2     1 0
0M
B
IT
X
MP
P
I
TC
F
GC
H
BC
H
ACycle Code Frame ID
1Payload Length
ReceivedPayload Length
ConfiguredTx Buffer: Header CRC Configured
Rx Buffer: Header CRC Received
2R
E
SP
P
IN
F
IS
Y
NS
F
IR
C
IReceive
Cycle CountData Pointer
3R
E
S
SP
P
I
SN
F
I
SS
Y
N
SS
F
I
SR
C
I
SCycle Count StatusF
T
BF
T
AM
L
S
TE
S
BE
S
AT
C
I
BT
C
I
AS
V
O
BS
V
O
AC
E
O
BC
E
O
AS
E
O
BS
E
O
AV
F
R
BV
F
R
A
: :
: :
Frame Configuration
Filter Configuration
Message Buffer Control
Message RAM Configuration
Updated from received Frame
Message Buffer Status
unused
Module Operation www.ti.com
1260 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModulePayload length received (PLR(6-0)), receive cycle count (RCC(5-0)), Received onChannel Indication
(RCI), Startup Frame Indication bit(SFI), sync bit(SYN), nullframe indication bit(NFI), payload preamble
indication bit(PPI), andreserved bit(RES) areonly updated from received valid data frames only.
Header word 3ofeach configured message buffer holds thecorresponding message buffer status MBS.
Figure 26-24. Header Section ofMessage Buffer inMessage RAM
Header 1(Word 0)
Write access through WRHS1, read access through RDHS1:
*Frame ID-Slot counter filtering configuration
*Cycle Code- Cycle counter filtering configuration
*CHA, CHB- Channel filtering configuration
*CFG- Message buffer configuration: receive /transmit
*PPIT- Payload Preamble Indicator Transmit
*TXM- Transmit mode configuration: single-shot /continuous
*MBI- Message buffer receive /transmit interrupt enable

<!-- Page 1261 -->

www.ti.com Module Operation
1261 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleHeader 2(Word 1)
Write access through WRHS2, read access through RDHS2:
*Header CRC Transmit Buffer: Configured bythehost (calculated from frame header)
-Receive Buffer: Updated from received frame
*Payload Length Configured: Length ofdata section (2-byte words) asconfigured bythehost
*Payload Length Received: Length ofpayload segment (2-byte words) stored from received frame
Header 3(Word 2)
Write access through WRHS3, read access through RDHS3:
*Data Pointer- Pointer tothebeginning ofthecorresponding data section inthedata partition
Read access through RDHS3, valid forreceive buffers only, updated from received frames:
*Receive Cycle Count -Cycle count from received frame
*RCI- Received onChannel Indicator
*SFI- Startup Frame Indicator
*SYN- Sync Frame Indicator
*NFI- Null Frame Indicator
*PPI- Payload Preamble Indicator
*RES- REServed bit
Message Buffer Status MBS (Word 3)
Read access through MBS, updated bythecommunication controller attheendoftheconfigured slot.
*VFRA- Valid Frame received onchannel A
*VFRB- Valid Frame received onchannel B
*SEOA- Syntax Error Observed onchannel A
*SEOB- Syntax Error Observed onchannel B
*CEOA- Content Error Observed onchannel A
*CEOB- Content Error Observed onchannel B
*SVOA- Slot Boundary Violation Observed onchannel A
*SVOB- Slot Boundary Violation Observed onchannel B
*TCIA- Transmission Conflict Indication channel A
*TCIB- Transmission Conflict Indication channel B
*ESA- Empty Slot Channel A
*ESB- Empty Slot Channel B
*MLST- Message Lost
*FTA- Frame Transmitted onChannel A
*FTB- Frame Transmitted onChannel B
*Cycle Count Status- Current cycle count when status was updated
*RCIS- Received onChannel Indicator Status
*SFIS- Startup Frame Indication Status
*SYNS- Sync Frame Indicator Status
*NFIS- Null Frame Indicator Status
*PPIS- Payload Preamble Indicator Status
*RESS- Reserved BitStatus

<!-- Page 1262 -->

Module Operation www.ti.com
1262 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.13.1.2 Data Partition
The data partition ofthemessage RAM stores thedata sections ofthemessage buffers configured for
reception /transmission asdefined intheheader partition. The number ofdata bytes foreach message
buffer canvary from 0to254. Inorder tooptimize thedata transfer between theshift registers ofthetwo
FlexRay protocol controllers andthemessage RAM aswell asbetween thehost interface andthe
message RAM, thephysical width ofthemessage RAM isword wise (4bytes).
The data partition starts right after thelastword oftheheader partition. When configuring themessage
buffers inthemessage RAM theprogrammer hastoassure thatthedata pointers point toaddresses
within thedata partition.
Figure 26-25 shows anexample how thepayload oftheconfigured message buffers canbestored inthe
data partition ofthemessage RAM. Message buffers 0to2arestatic buffers with apayload of3,whereas
message buffers 3tonaredynamic buffers with variable payload.
The beginning ofamessage buffer 'sdata section isdetermined bythedata pointer andthepayload
length configured inthemessage buffer 'sheader section. This enables aflexible usage oftheavailable
RAM space forstorage ofmessage buffers with different data lengths.
The storage ofthepayload data isword aligned. Ifthesize ofamessage buffer payload isanoddnumber
of2-byte words, theremaining 16bitsinthelast32-bit word areunused (see Figure 26-25 ).
Figure 26-25. Example Structure ofData Partition inMessage RAM
Bit/
Word3
13
02
92
82
72
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
09876543210
: MB0 Data3 MB0 Data2 MB0 Data1 MB0 Data0
: unused unused MB0 Data5 MB0 Data4
: MB1 Data3 MB1 Data2 MB1 Data1 MB1 Data0
: unused unused MB1 Data5 MB1 Data4
: MB2 Data3 MB2 Data2 MB2 Data1 MB2 Data0
: unused unused MB2 Data5 MB2 Data4
: MB3 Data3 MB3 Data2 MB3 Data1 MB3 Data0
: º º º º
: MB3 Data(k) MB3 Data(k-1) MB3 Data(k-2) MB3 Data(k-3)
: MBn Data3 MBn Data2 MBn Data1 MBn Data0
: º º º º
: º º º º
: MBn Data(m) MBn Data(m-1) MBn Data(m-2) MBn Data(m-3)
: unused unused unused unused
: unused unused unused unused
2046 unused unused unused unused
2047 unused unused unused unused
26.2.13.2 ECC Check
Inorder toassure theintegrity ofthedata stored inthedifferent RAM blocks ofthemodule (message
RAM, 2transient buffer RAMs, 2input buffer RAMs, 2output buffer RAMs, Transfer Configuration RAM),
theFlexRay module RAMs areoptionally ECC protected.
The ECC protection isswitched offbydefault andECC protection isactivated bywriting a4bitkeytothe
dedicated ECC lock bits(PEL(3-0)) intheGlobal Control Register (GCS/R) oftheTransfer Unit register
frame. ECC single-bit error correction isenabled bydefault andcanbedisabled bythe4-bit keybitsSBEL
intheECC Control Register (ECC_CTRL). Only theTransfer Configuration RAM hastheexceptional
functionality thatECC protection caneither beswitched onoroffbyPEL(3-0). Bydefault theECC
protection isswitched offandtheTCR isnotprotected.
Figure 26-26 shows theECC structure concerning enabling/disabling anderror indication.

<!-- Page 1263 -->

* Transfer Configuration RAM (TCR)* Message RAM
* Transient Buffer RAM A
* Transient Buffer RAM B
* Input Buffer RAM
* Input Buffer Shadow RAM
* Output Buffer RAM
* Output Buffer Shadow RAMSBE correction on
SBE correction offSBEL(3-0)
(ECC_CTRL)
FlexRay RAMsPERR flag
Parity/ECC failure
SBE failureSBE flagEIR
ECC failure
SBE failurePE flag
SE flagTEIFfaulty frame indicationFMB(6-0)
(MHDS)
faulty RAM location FRL(10-0)
(SBESTAT)
faulty address indicationADR(8-0)
(PEADR)
ADR(8-0)
(TSBESTAT)faulty address indicationUCRE
(EIES/R)UCREL
(EILS)
CC_int0CC_int1
TU_UCT_errE-Ray
Transfer UnitParity check
ECC check
TCR ECC off
TCR ECC onPEL(3-0)
(GCS/R)
closed, if
TCR ECC is onSBESTAT
TSBESTAT
TU_SBE_errSBE_EVT_EN(3-0)
(ECC_CTRL)CC_PERR_err
CC_SBE_err
www.ti.com Module Operation
1263 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-26. Parity/ECC Structure
NOTE: There isnoparity protection forFlexRay RAM blocks.
Fortheseven RAM blocks oftheCommunication Controller portion (message RAM, 2
transient buffer RAMs, 2input buffer RAMs and2output buffer RAMs), ECC protection is
added, which canbeselected bytheECC lock bits(PEL(3-0)). ECC protection cannotbe
switched offcompletely. FortheTCR oftheTransfer Unit, actually theECC multi-bit error
generation willbeswitched onandoffbytheECC lock bits, theECC generation itself
remains always on.
ECC should beactivated before initializing allRAMs bytheCLEAR_RAMS command and
should notbeswitched offduring FlexRay usage.
The following paragraphs describe theprotection fortheCommunication Controller related RAM blocks.
Fordetails about theprotection oftheTransfer Configuration RAM (TCR) oftheTransfer Unit, see
Section 26.2.1.1.2.1 .
AlltheCommunication Controller related RAM blocks have anECC generator andanECC checker
attached asshown inFigure 26-27 .When data iswritten toaRAM block, thelocal ECC generator
generates thecorresponding ECC information.
The ECC protection supports single-bit error correction anddouble-bit error detection mechanism
(SECDED). The ECC information isstored together with thecorresponding data word.
The ECC ischecked each time adata word isread from anyoftheRAM blocks. The module internal data
buses have awidth of32bits. IfanECC multi-bit error isdetected, thePERR error flagissetintheerror
interrupt register (EIR). Additionally, thecorrection ofanECC single-bit error willbeindicated bytheSBE
flagintheSingle-Bit Error Status Register (SBESTAT).

<!-- Page 1264 -->

GEN
CHKCHK
GEN
GEN CHK ECC Generator ECC CheckerCHKCHKMessage
RAM Transient
Buffer
RAM A
Transient
Buffer
RAM BPRT A
PRT BOutput
Buffer
RAM 1,2GEN
GENCHK GENInput
Buffer
RAM 1,2
Module Operation www.ti.com
1264 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleAnuncorrectable RAM error interrupt canbegenerated, ifenabled bytheUCRE bitintheerror interrupt
enable register (EIES/R). ForECC single-bit error, theuncorrectable RAM error interrupt isonly
generated, iftheECC single-bit error correction isdisabled andthesingle-bit error indication key
(SBE_EVT_EN inECC_CTRL) isenabled. When single-bit error correction isturned off,theECC
algorithm willdetect upto3bitsinerror inaword.
ForECC multi-bit errors thefaulty message buffer number, together with theinformation ofthefailing
RAM, canberead from themessage handler status (MHDS) register. Equivalent information isavailable
forECC single-bit errors inthesingle-bit error location (SBESTAT) register, irrespective ofECC single-bit
error correction being enabled.
Figure 26-27 shows thedata paths between theRAM blocks andtheECC generators andcheckers.
The ECC generation isdone according totheECC syndrome tables asshown inFigure 26-28 and
Figure 26-29 .
Figure 26-27. ECC Generation andCheck
NOTE: The ECC generator andECC checker arenotpart oftheRAM blocks, butoftheRAM
access logic.

<!-- Page 1265 -->

www.ti.com Module Operation
1265 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-28. ECC Syndrome Table
3
13
02
92
82
72
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
09876543210ECC
xxxxxxxx xxxxxxxx 6
xxxxxxxx xxxxxxxx 5
xx xxxxxx xx xxxxxx 4
xxx xxx xx xxx xxx xx 3
x x xx x xx xx x xx x xx x 2
x x x x x xxx x x x x x xxx 1
x xx x x xxx x x xxxx x x 0
ECC Error Bits forSyndrome Decode
6 5 4 3 2 1 0 ECC
7
x 6
x 5
x 4
x 3
x 2
x 1
x 0
Figure 26-29. ECC Syndrome Table (TCR)
18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 ECC
x x x x x x x x 5
x x x x x x x x x 4
x x x x x x x x x x 3
x x x x x x x x x x x 2
x x x x x x x x x x 1
x x x x x x x x x x x 0
ECC Error Bits forSyndrome Decode
5 4 3 2 1 0 ECC
7
6
x 5
x 4
x 3
x 2
x 1
x 0

<!-- Page 1266 -->

Module Operation www.ti.com
1266 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleWhen anECC error hasbeen detected thefollowing actions willbeperformed:
Inallcases:
*The corresponding error flaginthemessage handler status (MHDS) register issetandthefaulty
message buffer isindicated. OnECC single-bit error equivalent information isavailable inthesingle-bit
error location (SBESTAT) register.
*The error flagEIR.PERR intheerror interrupt register issetand, ifenabled, amodule interrupt tothe
CPU willbegenerated. AnECC single-bit error isindicated bytheSBESTAT.SBE flagirrespective of
ECC single-bit error correction being enabled. Additionally anECC single-bit error cangenerate an
interrupt totheCPU.
Additionally inspecific cases ofECC multi-bit errors:
1.ECC multi-bit error during data transfer from input buffer RAM 1,2⇒message RAM
a.Transfer ofheader and/or data section andECC multi-bit error occurs during header and/ ordata
section transfer tomessage RAM:
*MHDS.PIBF bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points toafaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*Header and/or data section ofthecorresponding message buffer isupdated
*Transmission request forthecorresponding message buffer isnotset(notransfer tothe
FlexRay bus)
b.Transfer ofdata section only andECC multi-bit error occurs when reading header section ofthe
corresponding message buffer from themessage RAM.
*MHDS.PMR bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points toafaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*The data section ofthecorresponding message buffer isnotupdated
*Transmission request forthecorresponding message buffer isnotset(notransfer tothe
FlexRay bus)
2.ECC multi-bit error during host CPU reading input buffer RAM 1,2
*MHDS.PIBF bitisset
3.ECC multi-bit error during scan ofheader sections inmessage RAM
*MHDS.PMR bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points toafaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*Ignore message buffer (the transfer ofthemessage buffer isskipped)
4.ECC multi-bit error during data transfer from message RAM totransient buffer RAM 1,2
*MHDS.PMR bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points tothefaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*Frame nottransmitted, frames already intransmission areinvalidated byclearing theframe CRC
to0
5.ECC multi-bit error during data transfer from transient buffer RAM 1,2toprotocol controller 1,2
*MHDS.PTBF1,2 bitisset
*Frames already intransmission areinvalidated byclearing theframe CRC to0

<!-- Page 1267 -->

www.ti.com Module Operation
1267 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module6.ECC multi-bit error during data transfer from transient buffer RAM 1,2tomessage RAM
a.ECC multi-bit error when reading header section ofcorresponding message buffer from message
RAM
*MHDS.PMR bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points toafaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*The data section ofthecorresponding message buffer isnotupdated
b.ECC multi-bit error when reading transient buffer RAM 1,2:
*MHDS.PTBF1,2 bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points toafaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*The data section ofthecorresponding message buffer isupdated
7.ECC multi-bit error during data transfer from message RAM tooutput buffer RAM
*MHDS.PMR bitisset
*MHDS.FMBD bitissettoindicate thatMHDS.FMB(6-0) points tofaulty message buffer
*MHDS.FMB(6-0) indicates thenumber ofthefaulty message buffer
*Header and/or data section oftheoutput buffer isupdated, butshould notbeused bythehost
CPU
8.ECC multi-bit error during host CPU reading output buffer RAM 1,2
*MHDS.POBF bitisset
9.ECC multi-bit error during data read oftransient buffer RAM 1,2
When anECC multi-bit error occurs during theMessage Handler reads aframe with network
management information (PPI =1)from thetransient buffer RAM 1,2thecorresponding network
management vector register NMV1,2,3 isnotupdated from thatframe.
Additionally inspecific cases ofECC single-bit errors:
1.ECC single-bit error during data transfer from input buffer RAM 1,2⇒message RAM
a.Transfer ofheader and/or data section andECC single-bit error occurs during header and/or data
section transfer tomessage RAM:
*SBESTAT.SIBF bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points toafaulty message
buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
*Header and/or data section ofthecorresponding message buffer isupdated
IfECC single-bit error correction isdisabled:
-Transmission request forthecorresponding message buffer isnotset(notransfer tothe
FlexRay bus)
b.Transfer ofdata section only andECC single-bit error occurs when reading header section of
corresponding message buffer from themessage RAM.
*SBESTAT.SMR bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points toafaulty message
buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
IfECC single-bit error correction isdisabled:
-The data section ofthecorresponding message buffer isnotupdated
-Transmission request forthecorresponding message buffer isnotset(notransfer tothe
FlexRay bus)
2.ECC single-bit error during host CPU reading input buffer RAM 1,2
*SBESTAT.SIBF bitisset

<!-- Page 1268 -->

Module Operation www.ti.com
1268 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module3.ECC single-bit error during scan ofheader sections inmessage RAM
*SBESTAT.SMR bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points toafaulty message buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
IfECC single-bit error correction isdisabled:
-Ignore message buffer (the transfer ofthemessage buffer isskipped)
4.ECC single-bit error during data transfer from message RAM totransient buffer RAM 1,2
*SBESTAT.SMR bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points tothefaulty message buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
IfECC single-bit error correction isdisabled:
-Frame nottransmitted, frames already intransmission areinvalidated byclearing theframe
CRC to0
5.ECC single-bit error during data transfer from transient buffer RAM 1,2toprotocol controller 1,2
*SBESTAT.STBF1,2 bitisset
IfECC single-bit error correction isdisabled:
-Frames already intransmission areinvalidated bysetting theframe CRC to0
6.ECC single-bit error during data transfer from transient buffer RAM 1,2tomessage RAM
a.ECC single-bit error when reading header section ofcorresponding message buffer from message
RAM
*SBESTAT.SMR bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points toafaulty message
buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
IfECC single-bit error correction isdisabled:
-The data section ofthecorresponding message buffer isnotupdated
b.ECC single-bit error when reading transient buffer RAM 1,2:
*SBESTAT.STBF1,2 bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points toafaulty message
buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
*The data section ofthecorresponding message buffer isupdated
7.ECC single-bit error during data transfer from message RAM tooutput buffer RAM
*SBESTAT.SMR bitisset
*SBESTAT.FMBD bitissettoindicate thatSBESTAT.FMB(6-0) points tofaulty message buffer
*SBESTAT.FMB(6-0) indicates thenumber ofthefaulty message buffer
IfECC single-bit error correction isdisabled:
-Header and/or data section oftheoutput buffer isupdated, butshould notbeused bythehost
CPU
8.ECC single-bit error during host CPU reading output buffer RAM 1,2
*SBESTAT.SOBF bitisset
9.ECC single-bit error during data read oftransient buffer RAM 1,2,when single-bit error correction is
disabled.
When anECC single-bit error occurs during when theMessage Handler reads aframe, with network
management information (PPI =1),from thetransient buffer RAM 1,2,thecorresponding network
management vector register NMV1,2,3 isnotupdated from thatframe.

<!-- Page 1269 -->

www.ti.com Module Operation
1269 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.13.2.1 Host Handling ofUncorrectable ECC Multi-bit Errors
Uncorrectable errors caused bytransient bitflips canbefixed by:
Self-healing
Uncorrectable errors located in:
*Input Buffer RAM 1,2
*Output Buffer RAM 1,2
*Data Section ofMessage RAM
*Transient Buffer RAM A
*Transient Buffer RAM B
*Transfer Configuration RAM (TCR)
areoverwritten with thenext write access tothedisturbed bit(s) caused byhost access orbyFlexRay
communication.
CLEAR_RAMS Command
When called inDEFAULT_CONFIG orCONFIG state POC command CLEAR_RAMS initializes all
module-internal RAMs to0andtheECC bitsareinitialized accordingly, depending what mode isenabled.
Temporary Unlocking ofHeader Section
Anuncorrectable error intheheader section ofalocked message buffer canbefixed byatransfer from
theinput buffer tothelocked buffer header section. Forthistransfer, thewrite-access totheIBCR
(specifying themessage buffer number) must beimmediately preceded bytheunlock sequence normally
used toleave CONFIG state. Forthatsingle transfer thecorresponding message buffer header is
unlocked, regardless whether itbelongs totheFIFO orwhether itslocking iscontrolled byMRC.SEC(1-0),
andwillbeupdated with new data.
NOTE: Incase theprevious methods donotwork, itisrecommended toexecute thePBIST testat
device level toconfirm ahard error inthemodule internal RAMs.
26.2.14 Interrupts
This section describes thetransfer unitinterrupts andthecommunication controller interrupts.
26.2.14.1 Transfer Unit Interrupts
26.2.14.1.1 Interrupt Structure
Fortransfer interrupts, oneenable bitisprovided foreach bitinthetransfer occurred status registers.
Maskable error interrupts arepossible forallerror conditions except ECC multi-bit error andmemory
protection error.
The ECC multi-bit error andthememory protection error have separate non-maskable lines. Both turn off
theTransfer Unit after finishing thecurrent word access cycle.
The single-bit error interrupt ismaskable. Onsingle-bit error, ifsingle-bit error correction isturned off,the
Transfer Unit isturned offafter finishing thecurrent word access cycle.
Figure 26-30 shows theinterrupt structure oftheFlexRay Transfer Unit.

<!-- Page 1270 -->

Transfer
Interrupts
Transfer
Buffer 1
Transfer
Buffer 2
Error
Interrupts
Forbidden
Access
Transfer
not readyTU_Int0 to VIM
TU_Int1 to VIMGlobal Interrupt
Mask (GCS/R)Interrupt Mask
(TSMIES/R 0/1/2/3,
TCCIES/R 0/1/2/3)Flags
(TSMO 0/1/2/3,
TCCO 0/1/2/3)
Error Interrupt
Flags (TEIF)Error Interrupt
Mask (TEIRES/R)
Uncorrectable
TCR error
Memory
Protection
ViolationTU_UCT_err to ESM
TU_MPV_err to ESMVBUS
read
VBUS
writeTransfer
Buffer 128
TCR Single Bit
Error Status
(TSBESTAT)
Single Bit
TCR errorTU_SBE_err to ESMECC Control Register
(ECC_CTRL)
Module Operation www.ti.com
1270 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-30. Transfer Unit (TU) Interrupt Structure

<!-- Page 1271 -->

www.ti.com Module Operation
1271 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.14.1.2 Enable Interrupts
TSMIES/R andTCCIES/R control thebuffer transfer interrupts foreach buffer inboth directions. The
TEIRES/R registers controls themaskable error interrupt sources which are:
*VBUS transaction errors
Ifanerror occurs during VBUS read orwrite transfer aerror interrupt willbegenerated.
*Forbidden access toIBForOBF
Since host accesses tocommunication controller through theIBFandtheOBF (0x400-0x7FF) are
forbidden, aslong astheTransfer Unit State Machine isenabled, accesses willbeignored andan
error interrupt willbegenerated.
*Transfer notready when TBA should beloaded
When atransfer isongoing/pending during base address reload onFlexRay communication cycle start
(only occurs ifNTBA !=TBA) theTBA willnotbeloaded andanerror interrupt willbegenerated.
The transfer interrupts useaseparate interrupt line(TU_int0) than theerror interrupts (TU_int1).
26.2.14.1.3 Interrupt Flags
The TSMO andTCCO flags indicate buffer transfer status interrupts whereas theTEIF flags indicate
interrupt sources formaskable andnon-maskable error interrupts.
The error interrupt flags aresetbytheTransfer Unit State Machine andcanbecleared bytheCPU by
writing a1.IftheCPU clears theflag, while theTransfer Unit State Machine sets itatthesame time, the
flagremains set.
26.2.14.1.4 Nonmaskable Error Indication
Memory protection violation anduncorrectable TCR error have their own nonmaskable error lines, which
canbeconnected totheVectored Interrupt Module (VIM) and/or theError Signaling Module (ESM). Refer
tothedevice-specific data manual onthehookup.
*Ifamemory protection violation occurs, theMemory Protection Violation Error (TU_MPV_err) linewill
beactivated.
*Ifanuncorrectable TCR error occurs while accessing theTCR, theECC Error (TU_UCT_err) linewill
beactivated. Anuncorrectable TCR error canbecaused byanECC error inTCR.
26.2.14.2 Communication Controller Interrupts
Ingeneral, interrupts provide aclose linktotheprotocol timing asthey aretriggered almost immediately
when anerror orstatus change isdetected bythecontroller, aframe isreceived ortransmitted, a
configured timer interrupt isactivated, orastop watch event occurred. This enables thehost CPU toreact
very quickly onspecific error conditions, status changes, ortimer events. Toremain flexible though, the
communication controller supports disable /enable controls foreach individual interrupt source separately.
Aninterrupt may betriggered, forexample, when:
*aframe isreceived ortransmitted
*anerror was detected
*astatus flagisset
*atimer reaches apreconfigured value
*amessage transfer from input buffer tomessage RAM orfrom message RAM tooutput buffer has
completed
*astop watch event occurred
NOTE: Forspecific information about error interrupt generation onuncorrectable RAM errors, see
Figure 26-30 .

<!-- Page 1272 -->

Interrupt
Source 10
1CC_int0
CC_int1Status/Error
Interrupt Enable
Set/Clear
(SIES/EIES, SIER/
EIERStatus/Error
Interrupt Line
Select
(SILS/EILS) EINT0
EINT1
Source 1
FlagInterrupt Line
Enable
Interrupt
Source 20
1Status/Error
Interrupt
Register
(SIR/EIRInterrupt
Line 0
Interrupt
Line 1
Source 2
Flag
Timer 0
Interrupt
Timer 1
InterruptCC_tint0
CC_tint1
Module Operation www.ti.com
1272 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-31. Communication Controller (CC) Interrupt Structure
Tracking status andgenerating interrupts when astatus change oranerror occurs aretwoindependent
tasks. Independent ofaninterrupt being enabled, thecorresponding status istracked andindicated bythe
Communication Controller. The host hasaccess tothecurrent status anderror information byreading the
error interrupt register andthestatus interrupt register.
The interrupt lines tothehost, CC_int0 andCC_int1, arecontrolled bytheenabled interrupts. Inaddition
each ofthetwointerrupt lines tothehost CPU canbeenabled /disabled separately byprogramming bit
EINT0 andEINT1 intheInterrupt Line Enable register.
The twotimer interrupts generated byinterrupt timer 0and1areavailable onpins CC_tint0 andCC_tint1.
They canbeconfigured viathetimer 0andtimer 1configuration register.
When atransfer between IBF/OBF andtheMessage RAM hascompleted bitSIR.TIBC orSIR.TOBC is
set.

<!-- Page 1273 -->

www.ti.com Module Operation
1273 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-16. Module Interrupt Flags andInterrupt Line Enable
Register Bit Function
EIR PEMC Protocol error Mode Changed
CNA Command NotValid
SFBM Sync Frames Below Minimum
SFO Sync Frame Overflow
CCF Clock Correction Failure
CCL CHI Command Locked
PERR ECC Error
RFO Receive FIFO Overrun
EFA Empty FIFO Access
IIBA Illegal Input Buffer Access
IOBA Illegal Output Buffer Access
MHF Message Handler Constraints Flag
EDA Error Detected onChannel A
LTVA Latest Transmit Violation Channel A
TABA Transmission Across Boundary Channel A
EDB Error Detected onChannel B
LTVB Latest Transmit Violation Channel B
TABB Transmission Across Boundary Channel B
SIR WST Wakeup Status
CAS Collision Avoidance Symbol
CYCS Cycle Start Interrupt
TXI Transmit Interrupt
RXI Receive Interrupt
RFNE Receive FIFO notEmpty
RFCL Receive FIFO Critical Level
NMVC Network Management Vector Changed
TI0 Timer Interrupt 0
TI1 Timer Interrupt 1
TIBC Transfer Input Buffer Completed
TOBC Transfer Output Buffer Completed
SWE Stop Watch Event
SUCS Startup Completed Successfully
MBSI Message Buffer Status Interrupt
SDS Start ofDynamic Segment
WUPA Wakeup Pattern Channel A
MTSA MTS Received onChannel A
WUPB Wakeup Pattern Channel B
MTSB MTS Received onChannel B
ILE EINT0 Enable Interrupt Line 0
EINT1 Enable Interrupt Line 1

<!-- Page 1274 -->

nss
VBUSclk32 . 22390157 22216 1/c61/c43/c180/c43/c61 /c163/c109
/c40 /c41][71
2 2min 8sVBUScycle VBUScycle VBUScyclet
VBUSfind m ss m tislots
clk/c109/c43 /c180/c43/c163
/c40 /c41
VBUSclkVBUScycles VBUScycles VBUScyclestfind m ss m t
islots/c43 /c180/c43/c1792 2
min 87
Module Operation www.ti.com
1274 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.15 Minimum Peripheral Clock Frequency
Inorder tocalculate theminimum peripheral clock frequency (VBUSclk) theworst case scenario hastobe
considered. The worst case scenario depends onthefollowing parameters:
*maximum payload length
*minimum minislot length
*number ofconfigured message buffers (excluding FIFO)
*used channels (single/dual channel)
Worst case scenario:
*reception ofamessage with amaximum payload length inslotn(nis7,15,23,31,39,...)
*slotn+1ton+7areempty dynamic slots (minislot) andconfigured asreceive buffer
*thefind-sequence (usually started inslot8,16,24,32,40,...) hastoscan themaximum number of
configured buffers
*thenumber ofconcurrent tasks hasitsmaximum value of3
The find-sequence isexecuted each 8slots (slot 8,16,24,32,40,...). Ithastobefinished until thenext find-
sequence isrequested.
The duration ofaTransient Buffer RAM (TBF) transfer totheMessage Buffer RAM (MBF) varies from 4
(header section only) to68(header +maximum data section) time steps plus asetup time of6time steps.
VBUScyclest2m=(number ofconcurrent tasks) x(6+(number of4-byte words))
ASlot Status (SS) transfer totheMessage Buffer RAM (MBF) hasalength of1time step plus asetup
time of4time steps.
VBUSclkss2m=(number ofconcurrent tasks) x5
The findsequence hasamaximum length of128(maximum number ofbuffers) time steps plus asetup
time of2time steps.
VBUSclkfind=(number ofconcurrent tasks) x(2+(number ofconfigured buffers))
Aminislot hasalength of2to63macroticks (MTicks). The minimum nominal macrotick period (MTcycle)
is1μs.Asequence of8minislots hasalength of
t8minislots =8xMTicks xMTcycle
The worst case VCLK cycle period canbecalculated asfollows:
(32)
(33)
minimum t8minislots =8*2*1μs=16μs
maximum VBUScyclet2m=3*(6+68)=222
maximum VBUScycless2m=3*5=15
maximum VBUScyclefind=3*(2+128) =390
(34)
The minimum peripheral clock frequency forthisworst case scenario is44,8125 MHz.

<!-- Page 1275 -->

www.ti.com Module Operation
1275 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.2.16 Assignment ofFlexRay Configuration Parameters
The following table shows theassignment oftheFlexRay parameters asdefined intheFlexRay Protocol
Specification andthecorresponding bitfields oftheFlexRay module.
Table 26-17. Assignment ofFlexRay Configuration Parameters
Parameter Bit(field)
pKeySlotusedForStartup SUCC1.TXST
pKeySlotUsedForSync SUCC1.TXSY
gColdStartAttempts SUCC1.CSA(4-0)
pAllowPassiveToActive SUCC1.PTA(4-0)
pWakeupChannel SUCC1.WUCS
pSingleSlotEnabled SUCC1.TSM
pAllowHaltDueToClock SUCC1.HCSE
pChannels SUCC1.CCHASUCC1.CCHB
pdListenTimeOut SUCC2.LT(20-0)
gListenNoise SUCC2.LTN(3-0)
gMaxWithoutClockCorrectionPassive SUCC3.WCP(3-0)
gMaxWithoutClockCorrectionFatal SUCC3.WCF(3-0)
gNetworkManagementVectorLength NEMC.NML(3-0)
gdTSSTransmitter PRTC1.TSST(3-0)
gdCASRxLowMax PRTC1.CASM(6-0)
gdSampleClockPeriod PRTC1.BRP(1-0)
pSamplesPerMicrotick PRTC1.BRP(1-0)
gdWakeupSymbolRxWindow PRTC1.RXW(8-0)
pWakeupPattern PRTC1.RWP(5-0)
gdWakeupSymbolRxIdle PRTC2.RXI(5-0)
gdWakeupSymbolRxLow PRTC2.RXL(5-0)
gdWakeupSymbolTxIdle PRTC2.TXI(7-0)
gdWakeupSymbolTxLow PRTC2.TXL(5-0)
gPayloadLengthStatic MHDC.SFDL(6-0)
pLatestTx MHDC.SLT(12-0)
pMicroPerCycle GTUC1.UT(19-0)
gMacroPerCycle GTUC2.MPC(13-0)
gSyncNodeMax GTUC2.SNM(3-0)
pMicroInitialOffset[A] GTUC3.UIOA(7-0)
pMicroInitialOffset[B] GTUC3.UIOB(7-0)
pMacroInitialOffset[A] GTUC3.MIOA(6-0)
pMacroInitialOffset[B] GTUC3.MIOB(6-0)
gdNIT GTUC4.NIT(13-0)
gOffsetCorrectionStart GTUC4.OCS(13-0)
pDelayCompensation[A] GTUC5.DCA(7-0)
pDelayCompensation[B] GTUC5.DCB(7-0)
pClusterDriftDamping GTUC5.CDD(4-0)
pDecodingCorrection GTUC5.DEC(7-0)
pdAcceptedStartupRange GTUC6.ASR(10-0)
pdMaxDrift GTUC6.MOD(10-0)
gdStaticSlot GTUC7.SSL(9-0)
gNumberOfStaticSlots GTUC7.NSS(9-0)
gdMinislot GTUC8.MSL(5-0)

<!-- Page 1276 -->

Module Operation www.ti.com
1276 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-17. Assignment ofFlexRay Configuration Parameters (continued)
Parameter Bit(field)
gNumberOfMinislots GTUC8.NMS(12-0)
gdActionPointOffset GTUC9.APO(5-0)
gdMinislotActionPointOffset GTUC9.MAPO(4-0)
gdDynamicSlotIdlePhase GTUC9.DSI(1-0)
pOffsetCorrectionOut GTUC10.MOC(13-0)
pRateCorrectionOut GTUC10.MRC(10-0)
pExternOffsetCorrection GTUC11.EOC(2-0)
pExternRateCorrection GTUC11.ERC(2-0)
26.2.17 Emulation/Debug Support
Foranydebug transactions onthebusinterface (VBUSP orOCP), theresponses arenormal except for
thefollowing:
1.Reads willnotclear "read-clear "type ofbitsduring thesame.
2.User mode writes areallowed to"privilege mode write only" bitsaswell.

<!-- Page 1277 -->

www.ti.com FlexRay Module Registers
1277 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3 FlexRay Module Registers
26.3.1 Transfer Unit Registers
Table 26-18 provides asummary oftheregisters. Allregisters areorganized as32-bit registers. 8-,16-,
and32-bit accesses aresupported. ForFlexRayTU transfers only, 4×32-bit data packages are
supported. The base address fortheTransfer Unit registers isFFF7 A000h.
The Transfer Unit State Machine registers usetheoffset address range 00hto1FCh.
Transfer Configuration RAM uses theoffset address range 00hto1FCh innormal mode and00hto3FCh
inECC testmode.
Table 26-18. Transfer Unit Registers
Offset Address Acronym Register Description Section
000h GSN0 Global Static Number 0 Section 26.3.1.1
004h GSN1 Global Static Number 1 Section 26.3.1.2
010h GCS Global Control Set Section 26.3.1.3
014h GCR Global Control Reset Section 26.3.1.3
018h TSCB Transfer Status Current Buffer Section 26.3.1.4
01Ch LTBCC Last Transferred Buffer toCommunication Controller Section 26.3.1.5
020h LTBSM Last Transferred Buffer toSystem Memory Section 26.3.1.6
024h TBA Transfer Base Address Section 26.3.1.7
028h NTBA Next Transfer Base Address Section 26.3.1.8
02Ch BAMS Base Address ofMirrored Status Section 26.3.1.9
030h SAMP Start Address ofMemory Protection Section 26.3.1.10
034h EAMP End Address ofMemory Protection Section 26.3.1.11
040h TSMO1 Transfer toSystem Memory Occurred 1 Section 26.3.1.12
044h TSMO2 Transfer toSystem Memory Occurred 2 Section 26.3.1.12
048h TSMO3 Transfer toSystem Memory Occurred 3 Section 26.3.1.12
04Ch TSMO4 Transfer toSystem Memory Occurred 4 Section 26.3.1.12
050h TCCO1 Transfer toCommunication Controller Occurred 1 Section 26.3.1.13
054h TCCO2 Transfer toCommunication Controller Occurred 2 Section 26.3.1.13
058h TCCO3 Transfer toCommunication Controller Occurred 3 Section 26.3.1.13
05Ch TCCO4 Transfer toCommunication Controller Occurred 4 Section 26.3.1.13
060h TOOFF Transfer Occurred Offset Section 26.3.1.14
06Ch TSBESTAT TCR ECC Single-Bit Error Status Section 26.3.1.15
070h PEADR ECC Error Address Section 26.3.1.16
074h TEIF Transfer Error Interrupt Section 26.3.1.17
078h TEIRES Transfer Error Interrupt Enable Set Section 26.3.1.18
07Ch TEIRER Transfer Error Interrupt Enable Reset Section 26.3.1.18
080h TTSMS1 Trigger Transfer toSystem Memory Set1 Section 26.3.1.19
084h TTSMR1 Trigger Transfer toSystem Memory Reset 1 Section 26.3.1.19
088h TTSMS2 Trigger Transfer toSystem Memory Set2 Section 26.3.1.19
08Ch TTSMR2 Trigger Transfer toSystem Memory Reset 2 Section 26.3.1.19
090h TTSMS3 Trigger Transfer toSystem Memory Set3 Section 26.3.1.19
094h TTSMR3 Trigger Transfer toSystem Memory Reset 3 Section 26.3.1.19
098h TTSMS4 Trigger Transfer toSystem Memory Set4 Section 26.3.1.19
09Ch TTSMR4 Trigger Transfer toSystem Memory Reset 4 Section 26.3.1.19
0A0h TTCCS1 Trigger Transfer toCommunication Controller Set1 Section 26.3.1.20
0A4h TTCCR1 Trigger Transfer toCommunication Controller Reset 1 Section 26.3.1.20
0A8h TTCCS2 Trigger Transfer toCommunication Controller Set2 Section 26.3.1.20

<!-- Page 1278 -->

FlexRay Module Registers www.ti.com
1278 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-18. Transfer Unit Registers (continued)
Offset Address Acronym Register Description Section
0ACh TTCCR2 Trigger Transfer toCommunication Controller Reset 2 Section 26.3.1.20
0B0h TTCCS3 Trigger Transfer toCommunication Controller Set3 Section 26.3.1.20
0B4h TTCCR3 Trigger Transfer toCommunication Controller Reset 3 Section 26.3.1.20
0B8h TTCCS4 Trigger Transfer toCommunication Controller Set4 Section 26.3.1.20
0BCh TTCCR4 Trigger Transfer toCommunication Controller Reset 4 Section 26.3.1.20
0C0h ETESMS1 Enable Transfer onEvent toSystem Memory Set1 Section 26.3.1.21
0C4h ETESMR1 Enable Transfer onEvent toSystem Memory Reset 1 Section 26.3.1.21
0C8h ETESMS2 Enable Transfer onEvent toSystem Memory Set2 Section 26.3.1.21
0CCh ETESMR2 Enable Transfer onEvent toSystem Memory Reset 2 Section 26.3.1.21
0D0h ETESMS3 Enable Transfer onEvent toSystem Memory Set3 Section 26.3.1.21
0D4h ETESMR3 Enable Transfer onEvent toSystem Memory Reset 3 Section 26.3.1.21
0D8h ETESMS4 Enable Transfer onEvent toSystem Memory Set4 Section 26.3.1.21
0DCh ETESMR4 Enable Transfer onEvent toSystem Memory Reset 4 Section 26.3.1.21
0E0h CESMS1 Clear onEvent toSystem Memory Set1 Section 26.3.1.22
0E4h CESMR1 Clear onEvent toSystem Memory Reset 1 Section 26.3.1.22
0E8h CESMS2 Clear onEvent toSystem Memory Set2 Section 26.3.1.22
0ECh CESMR2 Clear onEvent toSystem Memory Reset 2 Section 26.3.1.22
0F0h CESMS3 Clear onEvent toSystem Memory Set3 Section 26.3.1.22
0F4h CESMR3 Clear onEvent toSystem Memory Reset 3 Section 26.3.1.22
0F8h CESMS4 Clear onEvent toSystem Memory Set4 Section 26.3.1.22
0FCh CESMR4 Clear onEvent toSystem Memory Reset 4 Section 26.3.1.22
100h TSMIES1 Transfer toSystem Memory Interrupt Enable Set1 Section 26.3.1.23
104h TSMIER1 Transfer toSystem Memory Interrupt Enable Reset 1 Section 26.3.1.23
108h TSMIES2 Transfer toSystem Memory Interrupt Enable Set2 Section 26.3.1.23
10Ch TSMIER2 Transfer toSystem Memory Interrupt Enable Reset 2 Section 26.3.1.23
110h TSMIES3 Transfer toSystem Memory Interrupt Enable Set3 Section 26.3.1.23
114h TSMIER3 Transfer toSystem Memory Interrupt Enable Reset 3 Section 26.3.1.23
118h TSMIES4 Transfer toSystem Memory Interrupt Enable Set4 Section 26.3.1.23
11Ch TSMIER4 Transfer toSystem Memory Interrupt Enable Reset 4 Section 26.3.1.23
120h TCCIES1 Transfer toCommunication Controller Interrupt Enable Set1 Section 26.3.1.24
124h TCCIER1 Transfer toCommunication Controller Interrupt Enable Reset 1 Section 26.3.1.24
128h TCCIES2 Transfer toCommunication Controller Interrupt Enable Set2 Section 26.3.1.24
12Ch TCCIER2 Transfer toCommunication Controller Interrupt Enable Reset 2 Section 26.3.1.24
130h TCCIES3 Transfer toCommunication Controller Interrupt Enable Set3 Section 26.3.1.24
134h TCCIER3 Transfer toCommunication Controller Interrupt Enable Reset 3 Section 26.3.1.24
138h TCCIES4 Transfer toCommunication Controller Interrupt Enable Set4 Section 26.3.1.24
13Ch TCCIER4 Transfer toCommunication Controller Interrupt Enable Reset 4 Section 26.3.1.24
0-1FCh TCR Transfer Configuration RAM Section 26.3.1.25
200h-3FCh TCR ECC TCR ECC Test Mode Section 26.3.1.26

<!-- Page 1279 -->

www.ti.com FlexRay Module Registers
1279 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.1 Global Static Number 0(GSN0)
This register contains aconstant tocheck correctness ofdata transfers.
Figure 26-32. Global Static Number 0(GSN0) [offset_TU =00h]
31 16
Data_A
R-5432h
15 0
Data_B
R-ABCDh
LEGEND: R=Read only; -n=value after reset
Table 26-19. Global Static Number 0(GSN0) Field Descriptions
Bit Field Value Description
31-16 Data_A 0-FFFFh Data_A
15-0 Data_B 0-FFFFh Complement ofData_A
26.3.1.2 Global Static Number 1(GSN1)
This register contains aconstant tocheck correctness ofdata transfers.
Figure 26-33. Global Static Number 1(GSN1) [offset_TU =04h]
31 16
Data_C
R-ABCDh
15 0
Data_D
R-5432h
LEGEND: R=Read only; -n=value after reset
Table 26-20. Global Static Number 1(GSN1) Field Descriptions
Bit Field Value Description
31-16 Data_C 0-FFFFh Data_C
15-0 Data_D 0-FFFFh Complement ofData_C

<!-- Page 1280 -->

FlexRay Module Registers www.ti.com
1280 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.3 Global Control Set/Reset (GCS/GCR)
The GCx Registers reflects theconfiguration mode andallows toconfigure thebasic Transfer Unit
behavior.
The GCx registers consist ofasetregister (GCS) andareset register (GCR). Bitsaresetbywriting 1to
GCS andreset bywriting 1toGCR. Writing a0hasnoeffect. Reading from both addresses willresult in
thesame value.
ForGlobal Control Reset (GCR) bitdescriptions, seeTable 26-21 .
Figure 26-34. Global Control Set(GCS) [offset_TU =10h]
31 30 29 28 27 26 25 24
ENDVBM ENDVBS ENDR ENDH ENDP
R/S-0 R/S-0 R/S-0 R/S-0 R/S-0
23 22 21 20 19 16
Reserved PRIO Reserved PEL
R-0 R/S-0 R-0 R/S-5h
15 14 13 12 11 9 8
Reserved CETESM CTTCC CTTSM Reserved ETSM
R-0 R/S-0 R/S-0 R/S-0 R-0 R/S-0
7 6 5 4 3 2 1 0
Reserved SILE EILE Reserved TUH TUE
R-0 R/S-0 R/S-0 R-0 R/S-0 R/S-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Figure 26-35. Global Control Reset (GCR) [offset_TU =14h]
31 30 29 28 27 26 25 24
ENDVBM ENDVBS ENDR ENDH ENDP
R/S-0 R/S-0 R/S-0 R/S-0 R/S-0
23 22 21 20 19 16
Reserved PRIO Reserved PEL
R-0 R/S-0 R-0 R/S-5h
15 14 13 12 11 9 8
Reserved CETESM CTTCC CTTSM Reserved ETSM
R-0 R/S-0 R/S-0 R/S-0 R-0 R/S-0
7 6 5 4 3 2 1 0
Reserved SILE EILE Reserved TUH TUE
R-0 R/S-0 R/S-0 R-0 R/S-0 R/S-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset

<!-- Page 1281 -->

www.ti.com FlexRay Module Registers
1281 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-21. Global Control Set/Reset (GCS/R) Field Descriptions
Bit Field Value Description
31 ENDVBM Endianness Correction onVBusp Master.
0 Endianness correction switched off(Endianness isdefault: Little Endianness equal toBigEndian
word invariant (ARM:BE-32), same asallother peripherals) (Example 32BitWord =ABCD).
1 Endianness correction switched on(E-Ray Register, Header andPayload Endianness isaccording
theconfiguration ofbitsENDR0/1 ENDH0/1, ENDP0/1).
30 ENDVBS Endianness correction onVBusp Slave.
0 Endianness correction switched off(Endianness isdefault: Little Endianness equal toBigEndian
word invariant (ARM:BE-32), same asallother peripherals) (Example 32BitWord =ABCD).
1 Endianness correction switched on(E-Ray Register, Header andPayload Endianness isaccording
theconfiguration ofbitsENDR0/1, ENDH0/1, ENDP0/1).
29-28 ENDR Endianness Correction forNo(header orpayload) Data Sink Access.
Byte-order control ofCPU access toE-Ray register, Transfer Unit register andTransfer Unit ram
data. Data transferred between CPU anddata sink willbecorrected.
0 Remapped toABCDh.
1h Remapped toBADCh.
2h Remapped toCDABh.
3h Remapped toDCBAh.
27-26 ENDH Endianness Correction forHeader.
0 Remapped toABCDh.
1h Remapped toBADCh.
2h Remapped toCDABh.
3h Remapped toDCBAh.
25-24 ENDP Endianness Correction forPayload.
0 Remapped toABCDh.
1h Remapped toBADCh.
2h Remapped toCDABh.
3h Remapped toDCBAh.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21 PRIO Transfer Priority.
0 TTSM gets higher priority than TTCC.
1 TTCC gets higher priority than TTSM.
20 Reserved 0 Reserved
19-16 PEL ECC Lock.
5h ECC interrupt generation forTCR isswitched off.ECC protection formessage RAM, transient
buffer RAMs, input buffer RAMs andoutput buffer RAMs isswitched off.
Others ECC interrupt generation forTCR isswitched on.ECC protection formessage RAM, transient
buffer RAMs, input buffer RAMs andoutput buffer RAMs isswitched on.
Note: ForTCR, PEL enables ordisables theECC multi-bit error interrupt generation. While the
ECC feature isdisabled, theECC generation isstillongoing andtheerror indication bytheECC
interrupt flag(PE) intheTransfer Error Interrupt Flag register (TEIF) remains active. Only theECC
interrupt generation gets disabled.
15 Reserved 0 Reads return 0.Writes have noeffect.
14 CETESM Clear ETESM Register.
Clear allbitsofEnable Transfer onEvent toSystem Memory register.
0 Donotclear theregister.
1 Clear theregister when bitissetfrom 0to1.
13 CTTCC Clear TTCC Register.
0 Donotclear theregister.
1 Clear theregister when bitissetfrom 0to1.

<!-- Page 1282 -->

FlexRay Module Registers www.ti.com
1282 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-21. Global Control Set/Reset (GCS/R) Field Descriptions (continued)
Bit Field Value Description
12 CTTSM Clear TTSM Register.
0 Donotclear theregister.
1 Clear theregister when bitissetfrom 0to1.
11-9 Reserved 0 Reads return 0.Writes have noeffect.
8 ETSM Enable Transfer Status Mirrored.
Mirror technique must beadjustable.
0 Disable mirror function forTSCB, LTBCC, LTBSM, TSMO1-4, TCCO1-4, andTOOFF.
1 Enable mirror function forTSCB, LTBCC, LTBSM, TSMO1-4, TCCO1-4, andTOOFF.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5 SILE Status Interrupt Line Enable.
Enable status lineinterrupt.
0 TU_Int0 isdisabled.
1 TU_Int0 isenabled.
4 EILE Error Interrupt Line Enable.
Enable error interrupt line.
0 TU_Int1 isdisabled.
1 TU_Int1 isenabled.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TUH Transfer Unit Halted.
When halted, theTransfer Unit State Machine finishes theongoing VBUSM access before itstops
working. After deassertion, theTransfer Unit State Machine continues atthepoint itwas halted
before. Noreconfiguration isrequired.
0 Transfer Unit isnothalted.
1 Transfer Unit ishalted.
Note: IftheTransfer Unit State Machine halts, allmirroring registers contained thelast
transfer notthecurrent transfer information.
0 TUE Transfer Unit Enabled.
Enable transfer unit.
0 Transfer Unit isdisabled, reset Transfer Unit State Machine, completion ofthecurrent VBUS
transfer cycle butdata could becorrupt.
1 Transfer Unit isenabled.
Note: Before switching ontheTransfer Unit, theregisters must besetup.After re-enabling
oftheTransfer Unit State Machine thecontents ofthemodule registers andtheTCR isstill
valid (assuming itwas continuously powered).

<!-- Page 1283 -->

www.ti.com FlexRay Module Registers
1283 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.4 Transfer Status Current Buffer (TSCB)
The Transfer Status Current Buffer displays thecurrent buffer inprogress andindicates iftheTransfer
Unit State Machine isidleandishalt. The IDLE flagiscleared bywriting a1toit.
Figure 26-36. Transfer Status Current Buffer (TSCB) [offset_TU =18h]
31 21 20 16
Reserved TSMS
R-0 R-0
15 13 12 11 9 8 7 6 0
Reserved STUH Reserved IDLE RSVD BN
R-0 R-0 R-0 R/W-1 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-22. Transfer Status Current Buffer (TSCB) Field Descriptions
Bit Field Value Description
31-21 Reserved 0 Reads return 0.Writes have noeffect.
20-16 TSMS Transfer State Machine Status.
Reflects thecurrent status ofthetransfer state machine fordebug purpose (only available in
debug mode incombination with adebugger). InNormal Operation Mode, thevalue ofTSMS is
always read as0.
1h IDLE state
Transfer Trigger toSystem Memory:
2h Start state (TTSM_START)
3h Output Buffer Command Mask access state (TTSM_OBCM)
4h Request state (TTSM_REQ)
5h View state (TTSM_VIEW)
6h Check state (TTSM_CHECK)
7h Read Header Section access state (TTSM_RDHS)
8h Read Data Section access state (TTSM_RDDS)
Transfer Trigger toCommunication Controller:
9h Start state (TTCC_START)
Ah Busy state (TTCC_IBUSY)
Bh Check state (TTCC_CHECK)
Ch Write Header Section access state (TTCC_WRHS)
Dh Payload Read state (TTCC_PLC_READ)
Eh Payload Calculation state (TTCC_PLC_CALC)
Fh Write Data Section access state (TTCC_WRDS)
10h Input Buffer Command Mask access state (TTCC_IBCM)
11h Input Buffer Command Request access state (TTCC_IBCR)
12h Mirror state (TTCC_MIRROR)
13h End state (TTSM_END)
14h-1Eh Reserved
1Fh Undefined state
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12 STUH Status ofTransfer Unit State Machine forHalt Detection.
0 NotinHALT status.
1 InHALT status.
11-9 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1284 -->

FlexRay Module Registers www.ti.com
1284 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-22. Transfer Status Current Buffer (TSCB) Field Descriptions (continued)
Bit Field Value Description
8 IDLE Detects Transfer State Machine State IDLE.
Willbesetifthetransfer unitstate machine isinIDLE state andready tostart thenext transfer,
butnothing isrequested.
0 IDLE state isnotreached since lastclear.
1 IDLE state isreached.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-0 BN 0-7Fh Buffer number.
7-bit value ofbuffer number, which iscurrently intransfer. Ifstate machine enters IDLE mode the
lasttransferred buffer number isshown.
26.3.1.5 Last Transferred Buffer toCommunication Controller (LTBCC)
Shows thenumber ofthelastcompletely transferred message buffer from system memory tothe
communication controller.
Figure 26-37. Last Transferred Buffer toCommunication Controller (LTBCC) [offset_TU =1Ch]
31 76 0
Reserved BN
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-23. Last Transferred Buffer toCommunication Controller (LTBCC) Field Descriptions
Bit Field Value Description
31-7 Reserved. 0 Reads return 0.Writes have noeffect.
6-0 BN 0-7Fh Buffer number.
7-bit value oflastcompletely transferred message buffer from system memory tothecommunication
controller.
26.3.1.6 Last Transferred Buffer toSystem Memory (LTBSM)
Shows thenumber ofthelastcompletely transferred message buffer from communication controller tothe
system memory.
Figure 26-38. Last Transferred Buffer toSystem Memory (LTBSM) [offset_TU =20h]
31 76 0
Reserved BN
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-24. Last Transferred Buffer toSystem Memory (LTBSM) Field Descriptions
Bit Field Value Description
31-7 Reserved 0 Reads return 0.Writes have noeffect.
6-0 BN 0-7Fh Buffer number.
7-bit value oflastcompletely transferred message buffer from system memory tothe
communication controller tothesystem memory.

<!-- Page 1285 -->

www.ti.com FlexRay Module Registers
1285 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.7 Transfer Base Address (TBA)
The Transfer Base Address register holds a32-bit aligned 32-bit base-pointer, which defines thebase
address forthedata tobetransferred.
NOTE: Awrite tothisregister also modifies theNTBA register.
Figure 26-39. Transfer Base Address (TBA) [offset_TU =24h]
31 16
TBA
R/W-0
15 0
TBA
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-25. Transfer Base Address (TBA) Field Descriptions
Bit Field Description
31-0 TBA Transfer Base Address. 32-bit base pointer, 2LSB arenotsignificant (32-bit accesses only) andwill
always be0.
26.3.1.8 Next Transfer Base Address (NTBA)
The Next Transfer Base Address hold a32-bit aligned 32-bit base-pointer tobeloaded intoTBA during
next cycle start.
NOTE: Awrite onTBA register also modifies theNTBA register.
Figure 26-40. Next Transfer Base Address (NTBA) [offset_TU =28h]
31 16
NTBA
R/W-0
15 0
NTBA
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-26. Next Transfer Base Address (NTBA) Field Descriptions
Bit Field Description
31-0 NTBA Next Transfer Base Address. 32-bit base pointer, 2LSB arenotsignificant (32-bit accesses only) will
always be0.

<!-- Page 1286 -->

FlexRay Module Registers www.ti.com
1286 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.9 Base Address ofMirrored Status (BAMS)
The Base Address ofMirrored Status hold a32-bit aligned 32-bit base-pointer tobeuseformirror
transactions. Further details about thetransfer mirror mechanism canbefound inSection 26.2.1.1.1.7 .
Figure 26-41. Base Address ofMirrored Status (BAMS) [offset_TU =2Ch]
31 16
BAMS
R/W-0
15 0
BAMS
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-27. Base Address ofMirrored Status (BAMS) Field Descriptions
Bit Field Description
31-0 BAMS Base Address ofMirrored Status. 32-bit base pointer, 2LSB arenotsignificant (32-bit accesses only)
willalways be0.

<!-- Page 1287 -->

www.ti.com FlexRay Module Registers
1287 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.10 Start Address ofMemory Protection (SAMP)
The Start Address ofMemory Protection hold a32-bit address.
Figure 26-42. Start Address ofMemory Protection (SAMP) [offset_TU =30h]
31 16
SAMP
R/W-0
15 0
SAMP
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-28. Start Address ofMemory Protection (SAMP) Field Descriptions
Bit Field Description
31-0 SAMP Start Address Memory Protection.
Start address ofthememory area, which allows read andwrite accesses fortheTransfer Unit State
Machine. 32-bit base pointer, 2LSB arenotsignificant (32-bit accesses only) willalways be0.
26.3.1.11 End Address ofMemory Protection (EAMP)
The End Address ofMemory Protection hold a32-bit address.
Figure 26-43. End Address ofMemory Protection (EAMP) [offset_TU =34h]
31 16
EAMP
R/W-0
15 0
EAMP
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-29. End Address ofMemory Protection (EAMP) Field Descriptions
Bit Field Description
31-0 EAMP End Address Memory Protection.
End address ofthememory area, which allows read andwrite accesses fortheTransfer Unit State
Machine. 32-bit address, 2LSB arenotsignificant (32-bit accesses only) willalways be0.

<!-- Page 1288 -->

FlexRay Module Registers www.ti.com
1288 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.12 Transfer toSystem Memory Occurred (TSMO[1-4])
The Transfer toSystem Memory Occurred register reflects themessage buffer transfer status fora
transfer transaction tothesystem memory. Four 32-bit registers reflect allpossible 128message buffers.
NOTE: Writing 1willclear abit.Writing 0willleave abitunchanged.
Figure 26-44. Transfer toSystem Memory Occurred 1(TSMO1) [offset_TU =40h]
31 16
TSMO1[31-16]
R/W1C-0
15 0
TSMO1[15-0]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-45. Transfer toSystem Memory Occurred 2(TSMO2) [offset_TU =44h]
31 16
TSMO2[63-48]
R/W1C-0
15 0
TSMO2[47-32]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-46. Transfer toSystem Memory Occurred 3(TSMO3) [offset_TU =48h]
31 16
TSMO3[95-80]
R/W1C-0
15 0
TSMO3[79-64]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-47. Transfer toSystem Memory Occurred 4(TSMO4) [offset_TU =4Ch]
31 16
TSMO4[127-112]
R/W1C-0
15 0
TSMO4[111-96]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset

<!-- Page 1289 -->

www.ti.com FlexRay Module Registers
1289 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-30. Transfer toSystem Memory Occurred (TSMOn) Field Descriptions
Bit Field Value Description
31-0 TSMO(1-4)[ n] Transfer toSystem Memory Occurred Register.
The register bitscorrespond tomessage buffers 0to127. Each bitoftheregister reflects afinished
message buffer transfer tothesystem memory.
0 Read: Notransfer occurred.
Write: Bitisunchanged.
1 Read: Transfer occurred.
Write: Clears thebit.

<!-- Page 1290 -->

FlexRay Module Registers www.ti.com
1290 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.13 Transfer toCommunication Controller Occurred (TCCO[1-4])
The Transfer toCommunication Controller Occurred reflects themessage buffer transfer status fora
VBUSP master transfer transaction from thesystem memory. Four 32-bit registers reflect allpossible 128
message buffers.
NOTE: Writing 1willclear abit.Writing 0willleave abitunchanged.
Figure 26-48. Transfer toCommunication Controller Occurred 1(TCCO1) [offset_TU =50h]
31 16
TCCO1[31-16]
R/W1C-0
15 0
TCCO1[15-0]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-49. Transfer toCommunication Controller Occurred 2(TCCO2) [offset_TU =54h]
31 16
TCCO2[63-48]
R/W1C-0
15 0
TCCO2[47-32]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-50. Transfer toCommunication Controller Occurred 3(TCCO3) [offset_TU =58h]
31 16
TCCO3[95-80]
R/W1C-0
15 0
TCCO3[79-64]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Figure 26-51. Transfer toCommunication Controller Occurred 4(TCCO4) [offset_TU =5Ch]
31 16
TCCO4[127-112]
R/W1C-0
15 0
TCCO4[111-96]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset

<!-- Page 1291 -->

www.ti.com FlexRay Module Registers
1291 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-31. Transfer toCommunication Controller Occurred (TCCOn) Field Descriptions
Bit Field Value Description
31-0 TCCO(1-4)[ n] Transfer toCommunication Controller Occurred Register.
The register bitscorrespond tomessage buffers 0to127. Each bitoftheregister reflects afinished
message buffer transfer from thesystem memory.
0 Read: Notransfer occurred.
Write: Bitisunchanged.
1 Read: Transfer occurred.
Write: Clears thebit.

<!-- Page 1292 -->

FlexRay Module Registers www.ti.com
1292 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.14 Transfer Occurred Offset (TOOFF)
The Transfer Occurred Offset register contains theoffset vector tothehighest prior pending transfer
occurred interrupt andthetransfer direction.
After aread access thetransfer occurred flagiscleared andtheregister contents willbeupdated
automatically.
Figure 26-52. Transfer Occurred Offset (TOOFF) [offset_TU =60h]
31 16
Reserved
R-0
15 9 8 7 0
Reserved TDIR OFF
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-32. Transfer Occurred Offset (TOOFF) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 TDIR Transfer Direction. Incase thesame interrupt occurs forcommunication controller andTransfer Unit
State Machine transfers thePRIO bitintheGlobal Control register decides about thehigher priority.
0 Atransfer toSystem Memory occurred.
1 Atransfer totheCommunication Controller occurred.
7-0 OFF Offset Vector
0 Offset notvalid (notransfer occurred, interrupt pending).
1h Interrupt pending forbuffer 0.
2h Interrupt pending forbuffer 1.
3h Interrupt pending forbuffer 2.
: :
80h Interrupt pending forbuffer 127.
81h-
FFhReserved

<!-- Page 1293 -->

www.ti.com FlexRay Module Registers
1293 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.15 TCR Single-Bit Error Status (TSBESTAT)
After anECC single-bit error intheTransfer Configuration RAM (TCR) occurred, theSEflagissetandthe
affected address isstored inthisregister. The register isupdated without regard totheECC single-bit
error correction activation intheECC Control Register (ECC_CTRL).
The contents ofthisregister iscleared automatically when reading theregister.
NOTE: ECC single-bit error canonly beindicated bytheSEbitwhen ADR iscleared. Since the
contents ofADR isundefined after reset, itisrecommended toclear theregister byreading
it.
Figure 26-53. TCR Single-Bit Error Status (TSBESTAT) [offset_TU =6Ch]
31 30 16
SE Reserved
R-0 R-0
15 9 8 0
Reserved ADR
R-0 RC-U
LEGEND: R=Read only; RC=Clear onread; U=value isundefined; -n=value after reset
Table 26-33. TCR Single-Bit Error Status (TSBESTAT) Field Descriptions
Bit Field Value Description
31 SE ECC Single-Bit Error. The flagsignals anECC single-bit error tothehost. The flagissetwhen an
ECC single-bit error inTCR isdetected. The flagissetwithout regard tothesingle-bit error lock
setting ofECC Control Register (ECC_CTRL).
ECC multi-bit errors areindicated byaseparate PEbitintheTransfer Error Interrupt Flag (TEIF)
register.
0 NoECC single-bit error occurred.
1 ECC single-bit error occurred.
30-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 ADR Address offailing TCR word location. ADR[8-2] istheTCR word address where theECC single-bit
error occurred. ADR[1-0] arealways driven as00.

<!-- Page 1294 -->

FlexRay Module Registers www.ti.com
1294 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.16 ECC Error Address (PEADR)
After anECC multi-bit error intheTransfer Configuration RAM occurred, theaffected address isstored in
thisnotresettable register.
The contents oftheECC Error Address register aswell asthePEbitintheTransfer Error Interrupt Flag
(TEIF) register iscleared automatically when reading theECC Error Address register.
NOTE: AnECC multi-bit error canonly beindicated bythePEbitofTEIF register when PEADR is
cleared. Since thecontents ofPEADR isundefined after reset, itisrecommended toclear
theregister byreading it.
Figure 26-54. ECC Error Address (PEADR) [offset_TU =70h]
31 98 0
Reserved ADR
R-0 RC-U
LEGEND: R=Read only; RC=Clear onread; U=value isundefined; -n=value after reset
Table 26-34. ECC Error Address (PEADR) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 ADR 0-1FFh Address offailing TCR location. ADR[8-2] istheTCR word address where theECC multi-bit error
occurred. ADR[1-0] arealways driven as11.

<!-- Page 1295 -->

www.ti.com FlexRay Module Registers
1295 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.17 Transfer Error Interrupt Flag (TEIF)
The Transfer Error Interrupt Flag register includes theTransfer Unit error flags. The bitsintheTEIF are
cleared bywriting a1.
NOTE: Memory Protection Violation (MPV) andECC Error (PE) interrupts arenonmaskable andcan
notbedisabled. Therefore, theMPV andPEbitsarenotpart oftheTransfer Error Interrupt
Enable Set/Reset (TEIRES/R) registers.
Figure 26-55. Transfer Error Interrupt Flag (TEIF) [offset_TU =74h]
31 18 17 16
Reserved MPV PE
R-0 R/W1C-0 R/W1C-0
15 11 10 8 7 6 4 3 2 1 0
Reserved RSTAT RSVD WSTAT Reserved TNR FAC
R-0 R/W1C-0 R-0 R/W1C-0 R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing 0hasnoeffect); -n=value after reset
Table 26-35. Transfer Error Interrupt Flag (TEIF) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17 MPV Memory Protection Violation.
0 NoMPV occurred.
1 MPV occurred.
16 PE ECC Error. The flagsignals anECC multi-bit error tothehost. The flagissetwhen anECC multi-
biterror inTCR isdetected.
Note: ECC single-bit errors inTCR areindicated byaseparate SEbitinTCR Single-Bit Error
Status (TSBESTAT).
0 NoECC multi-bit error occurred.
1 ECC multi-bit error occurred.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 RSTAT Status ofVBUS onread transfers.
0 Success
1h Addressing error
2h Protection error
3h Timeout error
4h Data error
5h Unsupported addressing mode error
6h Reserved
7h Exclusive read failure
Note: Any value other than 000indicates aVBUS read error. The information ofthespecific VBUS
fault isfordebug reasons only andisnotrelevant fornormal usage.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 WSTAT Status ofVBUS onwrite transfers.
0 Success
1h Addressing error
2h Protection error
3h Timeout error
4h Reserved
5h Unsupported addressing mode error
6h Reserved

<!-- Page 1296 -->

FlexRay Module Registers www.ti.com
1296 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-35. Transfer Error Interrupt Flag (TEIF) Field Descriptions (continued)
Bit Field Value Description
7h Exclusive write failure
Note: Any value other than 000indicates aVBUS read error. The information ofthespecific VBUS
fault isfordebug reasons only andisnotrelevant fornormal usage.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TNR Transfer NotReady.
0 Transfer started andNTBA isloaded toTBA.
1 Transfer isnotready oncommunication cycle start andtherefore NTBA isnotloaded toTBA.
0 FAC Forbidden Access.
0 Noforbidden access occurred.
1 Aforbidden CPU access toIBForOBF occurred when theTransfer Unit State Machine isenabled.

<!-- Page 1297 -->

www.ti.com FlexRay Module Registers
1297 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.18 Transfer Error Interrupt Enable Set/Reset (TEIRES/TEIRER)
The Transfer Error Interrupt Enable Setcontrols theinterrupt activation ofinterrupt lineTU_Int1. An
interrupt isgenerated ifboth theinterrupt flaginTEIF andthecorresponding bitinTEIRES areset.
Exceptions arethememory protection violation (MPV) andtheECC (PE) error, which arerelated to
nonmaskable interrupts, andtherefore arenotpart oftheTEIRS/R registers. Those errors have private
error lines (TU_MPV_err andTU_UCT_err), which canbeconnected totheVectored Interrupt Module
(VIM) and/or theError Signaling Module (ESM). Refer tothedevice-specific data manual formore details
about thesignal hookup.
ATransfer Error Interrupt isenabled bywriting 1toTEIRES register anddisabled bywriting 1toTIERER
register. Writing of0hasnoeffect. Reading from both addresses willresult inthesame value.
Figure 26-56. Transfer Error Interrupt Enable Set(TEIRES) [offset_TU =78h]
31 16
Reserved
R-0
15 11 10 8 7 6 4 3 2 1 0
Reserved RSTATE RSVD WSTATE Reserved TNRE FACE
R-0 R/WS-0 R-0 R/WS-0 R-0 R/WS-0 R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-36. Transfer Error Interrupt Enable Set(TEIRES)
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 RSTATE Read Error Interrupt Generation (interrupt generation onVBUS read transfer errors).
0 Interrupt generation onVBUS read transfer error isdisabled.
7h Interrupt generation onVBUS read transfer error isenabled.
Note: Any value different from 111does notassure theinterrupt error generation ofallpossible
VBUS read errors.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 WSTATE Write Error Interrupt Generation (interrupt generation onVBUS write transfer errors).
0 Interrupt generation onVBUS write transfer error isdisabled.
7h Interrupt generation onVBUS write transfer error isenabled.
Note: Any value different from 111does notassure theinterrupt error generation ofallpossible
VBUS read errors.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TNRE Transfer NotReady Enable.
0 TNR interrupt isdisabled.
1 TNR interrupt isenabled.
0 FACE Forbidden Access Enable.
0 FAC interrupt isdisabled.
1 FAC interrupt isenabled.

<!-- Page 1298 -->

FlexRay Module Registers www.ti.com
1298 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-57. Transfer Error Interrupt Enable Reset (TEIRER) [offset_TU =7Ch]
31 16
Reserved
R-0
15 11 10 8 7 6 4 3 2 1 0
Reserved RSTATE RSVD WSTATE Reserved TNRE FACE
R-0 R/WC-0 R-0 R/WC-0 R-0 R/WC-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-37. Transfer Error Interrupt Enable Reset (TEIRER)
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 RSTATE Read Error Interrupt Generation (interrupt generation onVBUS read transfer errors).
0 Interrupt generation onVBUS read transfer error isdisabled.
7h Interrupt generation onVBUS read transfer error isenabled.
Note: Any value different from 111does notassure theinterrupt error generation ofallpossible
VBUS read errors.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 WSTATE Write Error Interrupt Generation (interrupt generation onVBUS write transfer errors).
0 Interrupt generation onVBUS write transfer error isdisabled.
7h Interrupt generation onVBUS write transfer error isenabled.
Note: Any value different from 111does notassure theinterrupt error generation ofallpossible
VBUS read errors.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TNRE Transfer NotReady Enable.
0 TNR interrupt isdisabled.
1 TNR interrupt isenabled.
0 FACE Forbidden Access Enable.
0 FAC interrupt isdisabled.
1 FAC interrupt isenabled.

<!-- Page 1299 -->

www.ti.com FlexRay Module Registers
1299 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.19 Trigger Transfer toSystem Memory Set/Reset (TTSMS[1-4]/TTSMR[1-4])
The Trigger Transfer toSystem Memory register selects thecurrent message buffer foraTransfer Unit
State Machine transfer transaction tosystem memory. Four 32-bit registers reflect allpossible 128
message buffers.
The bitsaresetbywriting 1toTTSMSx andreset bywriting 1toTTSMRx orafter thetransfer occurred.
Writing a0hasnoeffect. Reading from both addresses willresult inthesame value.
Figure 26-58. Trigger Transfer toSystem Memory Set1(TTSMS1) [offset_TU =80h]
31 16
TTSMS1[31-16]
R/WS-0
15 0
TTSMS1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-38. Trigger Transfer toSystem Memory Set1(TTSMS1) Field Descriptions
Bit Field Value Description
31-0 TTSMS1[ n] Trigger Transfer toSystem Memory Set1.The register bits0to31correspond tomessage buffers
0to31.Each bitoftheregister controls themessage buffer transfer tothesystem memory inthe
following manner (not thatonly theleast significant bitofallfour combined TTSM registers will
currently scheduled fortransmission).
0 Notransfer request.
1 Transfer based onaddress defined inTBA
Figure 26-59. Trigger Transfer toSystem Memory Reset 1(TTSMR1) [offset_TU =84h]
31 16
TTSMR1
R/WC-0
15 0
TTSMR1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-39. Trigger Transfer toSystem Memory Reset 1(TTSMR1) Field Descriptions
Bit Field Description
31-0 TTSMR1 Trigger Transfer toSystem Memory Reset 1.The TTSMR1 register shows theidentical values toTTSMS1 if
read.

<!-- Page 1300 -->

FlexRay Module Registers www.ti.com
1300 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-60. Trigger Transfer toSystem Memory Set2(TTSMS2) [offset_TU =88h]
31 16
TTSMS2[63-48]
R/WS-0
15 0
TTSMS2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-40. Trigger Transfer toSystem Memory Set2(TTSMS2) Field Descriptions
Bit Field Value Description
31-0 TTSMS2[ n] Trigger Transfer toSystem Memory Set2.The register bits0to31correspond tomessage buffers
32to63.Each bitoftheregister controls themessage buffer transfer tothesystem memory inthe
following manner (note thatonly theleast-significant bitofallfour combined TTSM registers willbe
currently scheduled fortransmission).
0 Notransfer request.
1 Transfer based onaddress defined inTBA
Figure 26-61. Trigger Transfer toSystem Memory Reset 2(TTSMR2) [offset_TU =8Ch]
31 16
TTSMR2
R/WC-0
15 0
TTSMR2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-41. Trigger Transfer toSystem Memory Reset 2(TTSMR2) Field Descriptions
Bit Field Description
31-0 TTSMR2 Trigger Transfer toSystem Memory Reset 2.The TTSMR2 register shows theidentical values toTTSMS2 if
read.

<!-- Page 1301 -->

www.ti.com FlexRay Module Registers
1301 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-62. Trigger Transfer toSystem Memory Set3(TTSMS3) [offset_TU =90h]
31 16
TTSMS3[95-80]
R/WS-0
15 0
TTSMS3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-42. Trigger Transfer toSystem Memory Set3(TTSMS3) Field Descriptions
Bit Field Value Description
31-0 TTSMS3[ n] Trigger Transfer toSystem Memory Set3.The register bits0to31correspond tomessage buffers
64to95.Each bitoftheregister controls themessage buffer transfer tothesystem memory inthe
following manner (note thatonly theleast-significant bitofallfour combined TTSM registers willbe
currently scheduled fortransmission).
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-63. Trigger Transfer toSystem Memory Reset 3(TTSMR3) [offset_TU =94h]
31 16
TTSMR3
R/WC-0
15 0
TTSMR3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-43. Trigger Transfer toSystem Memory Reset 3(TTSMR3) Field Descriptions
Bit Field Description
31-0 TTSMR3 Trigger Transfer toSystem Memory Reset 3.The TTSMR3 register shows theidentical values toTTSMS3 if
read.

<!-- Page 1302 -->

FlexRay Module Registers www.ti.com
1302 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-64. Trigger Transfer toSystem Memory Set4(TTSMS4) [offset_TU =98h]
31 16
TTSMS4[127-112]
R/WS-0
15 0
TTSMS4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-44. Trigger Transfer toSystem Memory Set4(TTSMS4) Field Descriptions
Bit Field Value Description
31-0 TTSMS4[ n] Trigger Transfer toSystem Memory Set4.The register bits0to31correspond tomessage buffers
96to127. Each bitoftheregister controls themessage buffer transfer tothesystem memory inthe
following manner (note thatonly theleast-significant bitofallfour combined TTSM registers willbe
currently scheduled fortransmission).
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-65. Trigger Transfer toSystem Memory Reset 4(TTSMR4) [offset_TU =9Ch]
31 16
TTSMR4
R/WC-0
15 0
TTSMR4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-45. Trigger Transfer toSystem Memory Reset 4(TTSMR4) Field Descriptions
Bit Field Description
31-0 TTSMR4 Trigger Transfer toSystem Memory Reset 4.The TTSMR4 register shows theidentical values toTTSMS4 if
read.

<!-- Page 1303 -->

www.ti.com FlexRay Module Registers
1303 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.20 Trigger Transfer toCommunication Controller Set/Reset (TTCCS[1-4]/TTCCR[1-4])
The Trigger Transfer toCommunication Controller registers select thecurrent message buffer fora
Transfer Unit State Machine transfer transaction from system memory. Four 32-bit registers reflect all
possible 128message buffers.
The bitsaresetbywriting 1toTTCCSx andreset bywriting 1toTTCCRx orafter thetransfer occurred.
Writing a0hasnoeffect. Reading from both addresses willresult inthesame value.
Figure 26-66. Trigger Transfer toCommunication Controller Set1(TTCCS1) [offset_TU =A0h]
31 16
TTCCS1[31-16]
R/WS-0
15 0
TTCCS1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-46. Trigger Transfer toCommunication Controller Set1(TTCCS1) Field Descriptions
Bit Field Value Description
31-0 TTCCS1[ n] Trigger Transfer toCommunication Controller Set1.
The register bits0to31correspond tomessage buffers 0to31.Each bitoftheregister controls the
message buffer transfer tothecommunication controller inthefollowing manner:
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-67. Trigger Transfer toCommunication Controller Reset 1(TTCCR1) [offset_TU =A4h]
31 16
TTCCR1
R/WC-0
15 0
TTCCR1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-47. Trigger Transfer toCommunication Controller Reset 1(TTCCR1) Field Descriptions
Bit Field Description
31-0 TTCCR1 Trigger Transfer toCommunication Controller Reset 1.The TTCCR1 register shows theidentical values to
TTCCS1 ifread.

<!-- Page 1304 -->

FlexRay Module Registers www.ti.com
1304 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-68. Trigger Transfer toCommunication Controller Set2(TTCCS2) [offset_TU =A8h]
31 16
TTCCS2[63-48]
R/WS-0
15 0
TTCCS2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-48. Trigger Transfer toCommunication Controller Set2(TTCCS2) Field Descriptions
Bit Field Value Description
31-0 TTCCS2[ n] Trigger Transfer toCommunication Controller Set2.The register bits0to31correspond to
message buffers 32to63.Each bitoftheregister controls themessage buffer transfer tothe
communication controller inthefollowing manner.
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-69. Trigger Transfer toCommunication Controller Reset 2(TTCCR2) [offset_TU =ACh]
31 16
TTCCR2
R/WC-0
15 0
TTCCR2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-49. Trigger Transfer toCommunication Controller Reset 2(TTCCR2) Field Descriptions
Bit Field Description
31-0 TTCCR2 Trigger Transfer toCommunication Controller Reset 2.The TTCCR2 register shows theidentical values to
TTCCS2 ifread.

<!-- Page 1305 -->

www.ti.com FlexRay Module Registers
1305 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-70. Trigger Transfer toCommunication Controller Set3(TTCCS3) [offset_TU =B0h]
31 16
TTCCS3[95-80]
R/WS-0
15 0
TTCCS3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-50. Trigger Transfer toCommunication Controller Set3(TTCCS3) Field Descriptions
Bit Field Value Description
31-0 TTCCS3[ n] Trigger Transfer toCommunication Controller Set3.The register bits0to31correspond to
message buffers 64to95.Each bitoftheregister controls themessage buffer transfer tothe
communication controller inthefollowing manner.
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-71. Trigger Transfer toCommunication Controller Reset 3(TTCCR3) [offset_TU =B4h]
31 16
TTCCR3
R/WC-0
15 0
TTCCR3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-51. Trigger Transfer toCommunication Controller Reset 3(TTCCR3) Field Descriptions
Bit Field Description
31-0 TTCCR3 Trigger Transfer toCommunication Controller Reset 3.The TTCCR3 register shows theidentical values to
TTCCS3 ifread.

<!-- Page 1306 -->

FlexRay Module Registers www.ti.com
1306 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-72. Trigger Transfer toCommunication Controller Set4(TTCCS4) [offset_TU =B8h]
31 16
TTCCS4[127-112]
R/WS-0
15 0
TTCCS4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-52. Trigger Transfer toCommunication Controller Set4(TTCCS4) Field Descriptions
Bit Field Value Description
31-0 TTCCS4[ n] Trigger Transfer toCommunication Controller Set4.The register bits0to31correspond to
message buffers 96to127. Each bitoftheregister controls themessage buffer transfer tothe
communication controller inthefollowing manner:
0 Notransfer request.
1 Transfer based onaddress defined inTBA.
Figure 26-73. Trigger Transfer toCommunication Controller Reset 4(TTCCR4) [offset_TU =BCh]
31 16
TTCCR4
R/WC-0
15 0
TTCCR4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-53. Trigger Transfer toCommunication Controller Reset 4(TTCCR4) Field Descriptions
Bit Field Description
31-0 TTCCR4 Trigger Transfer toCommunication Controller Reset 4.The TTCCR4 register shows theidentical values to
TTCCS4 ifread.

<!-- Page 1307 -->

www.ti.com FlexRay Module Registers
1307 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.21 Enable Transfer onEvent toSystem Memory Set/Reset (ETESMS[1-4]/ETESMR[1-4])
The Enable Transfer onEvent toSystem Memory Setregisters enable amessage buffer transfer tothe
system memory after areceive ortransmit event. Four 32-bit registers reflect allpossible 128message
buffers.
The bitsaresetbywriting 1toETESMSx andreset bywriting 1toETESMRx. Writing a0hasnoeffect.
Reading from both addresses willresult inthesame value.
Figure 26-74. Enable Transfer onEvent toSystem Memory Set1(ETESMS1) [offset_TU =C0h]
31 16
ETESMS1[31-16]
R/WS-0
15 0
ETESMS1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-54. Enable Transfer onEvent toSystem Memory Set1Field Descriptions
Bit Field Value Description
31-0 ETESMS1[ n] Enable Transfer onEvent toSystem Memory Set1.The register bits0to31correspond to
message buffers 0to31.Each bitoftheregister enables amessage buffer transfer onevent tothe
system memory:
0 Transfer onevent isdisabled.
1 Transfer onevent isenabled.
Figure 26-75. Enable Transfer onEvent toSystem Memory Reset 1(ETESMR1) [offset_TU =C4h]
31 16
ETESMR1
R/WC-0
15 0
ETESMR1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-55. Enable Transfer onEvent toSystem Memory Reset 1(ETESMR1) Field Descriptions
Bit Field Description
31-0 ETESMR1 Enable Transfer onEvent toSystem Memory Reset 1.The ETESMR1 register shows theidentical values to
ETESMS1 ifread.

<!-- Page 1308 -->

FlexRay Module Registers www.ti.com
1308 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-76. Enable Transfer onEvent toSystem Memory Set2(ETESMS2) [offset_TU =C8h]
31 16
ETESMS2[63-48]
R/WS-0
15 0
ETESMS2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-56. Enable Transfer onEvent toSystem Memory Set2Field Descriptions
Bit Field Value Description
31-0 ETESMS2[ n] Enable Transfer onEvent toSystem Memory Set2.The register bits0to31correspond to
message buffers 32to63.Each bitoftheregister enables amessage buffer transfer onevent to
thesystem memory:
0 Transfer onevent isdisabled.
1 Transfer onevent isenabled.
Figure 26-77. Enable Transfer onEvent toSystem Memory Reset 2(ETESMR2) [offset_TU =CCh]
31 16
ETESMR2
R/WC-0
15 0
ETESMR2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-57. Enable Transfer onEvent toSystem Memory Reset 2(ETESMR2) Field Descriptions
Bit Field Description
31-0 ETESMR2 Enable Transfer onEvent toSystem Memory Reset 2.The ETESMR2 register shows theidentical values to
ETESMS2 ifread.

<!-- Page 1309 -->

www.ti.com FlexRay Module Registers
1309 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-78. Enable Transfer onEvent toSystem Memory Set3(ETESMS3) [offset_TU =D0h]
31 16
ETESMS3[95-80]
R/WS-0
15 0
ETESMS3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-58. Enable Transfer onEvent toSystem Memory Set3Field Descriptions
Bit Field Value Description
31-0 ETESMS3[ n] Enable Transfer onEvent toSystem Memory Set3.The register bits0to31correspond to
message buffers 64to95.Each bitoftheregister enables amessage buffer transfer onevent to
thesystem memory:
0 Transfer onevent isdisabled.
1 Transfer onevent isenabled.
Figure 26-79. Enable Transfer onEvent toSystem Memory Reset 3(ETESMR3) [offset_TU =D4h]
31 16
ETESMR3
R/WC-0
15 0
ETESMR3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-59. Enable Transfer onEvent toSystem Memory Reset 3(ETESMR3) Field Descriptions
Bit Field Description
31-0 ETESMR3 Enable Transfer onEvent toSystem Memory Reset 3.The ETESMR3 register shows theidentical values to
ETESMS3 ifread.

<!-- Page 1310 -->

FlexRay Module Registers www.ti.com
1310 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-80. Enable Transfer onEvent toSystem Memory Set4(ETESMS4) [offset_TU =D8h]
31 16
ETESMS4[127-112]
R/WS-0
15 0
ETESMS4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-60. Enable Transfer onEvent toSystem Memory Set4Field Descriptions
Bit Field Value Description
31-0 ETESMS4[ n] Enable Transfer onEvent toSystem Memory Set4.The register bits0to31correspond to
message buffers 96to127. Each bitoftheregister enables amessage buffer transfer onevent to
thesystem memory:
0 Transfer onevent isdisabled.
1 Transfer onevent isenabled.
Figure 26-81. Enable Transfer onEvent toSystem Memory Reset 4(ETESMR4) [offset_TU =DCh]
31 16
ETESMR4
R/WC-0
15 0
ETESMR4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-61. Enable Transfer onEvent toSystem Memory Reset 4(ETESMR4) Field Descriptions
Bit Field Description
31-0 ETESMR4 Enable Transfer onEvent toSystem Memory Reset 4.The ETESMR4 register shows theidentical values to
ETESMS4 ifread.

<!-- Page 1311 -->

www.ti.com FlexRay Module Registers
1311 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.22 Clear onEvent toSystem Memory Set/Reset (CESMS[1-4]/CESMR[1-4])
The Clear onEvent toSystem Memory registers disables anenabled transfer onevent (enabled in
ETESM) after areceive ortransmit event. Four 32-bit registers reflect allpossible 128message buffers.
The bitsaresetbywriting 1toCESMSx andreset bywriting 1toCESMRx. Writing a0hasnoeffect.
Reading from both addresses willresult inthesame value.
Figure 26-82. Clear onEvent toSystem Memory Set1(CESMS1) [offset_TU =E0h]
31 16
CESMS1[31-16]
R/WS-0
15 0
CESMS1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-62. Clear onEvent toSystem Memory Set1(CESMS1) Field Descriptions
Bit Field Value Description
31-0 CESMS1[ n] Clear onEvent toSystem Memory Set1.The register bits0to31correspond tomessage buffers 0
to31.Each bitoftheregister enables anautomatic clear ofthecorresponding ETESM1 bitafter a
receive ortransmit event:
0 Noclear.
1 Activate clear.
Figure 26-83. Clear onEvent toSystem Memory Reset 1(CESMR1) [offset_TU =E4h]
31 16
CESMR1
R/WC-0
15 0
CESMR1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-63. Clear onEvent toSystem Memory Reset 1(CESMR1) Field Descriptions
Bit Field Description
31-0 CESMR1 Clear onEvent toSystem Memory Reset 1.The CESMR1 register shows theidentical values toCESMS1 if
read.

<!-- Page 1312 -->

FlexRay Module Registers www.ti.com
1312 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-84. Clear onEvent toSystem Memory Set2(CESMS2) [offset_TU =E8h]
31 16
CESMS2[63-48]
R/WS-0
15 0
CESMS2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-64. Clear onEvent toSystem Memory Set2(CESMS2) Field Descriptions
Bit Field Value Description
31-0 CESMS2[ n] Clear onEvent toSystem Memory Set2.The register bits0to31correspond tomessage buffers
32to63.Each bitoftheregister enables anautomatic clear ofthecorresponding ETESM2 bitafter
areceive ortransmit event:
0 Noclear.
1 Activate clear.
Figure 26-85. Clear onEvent toSystem Memory Reset 2(CESMR2) [offset_TU =ECh]
31 16
CESMR2
R/WC-0
15 0
CESMR2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-65. Clear onEvent toSystem Memory Reset 2(CESMR2) Field Descriptions
Bit Field Description
31-0 CESMR2 Clear onEvent toSystem Memory Reset 2.The CESMR2 register shows theidentical values toCESMS2 if
read.

<!-- Page 1313 -->

www.ti.com FlexRay Module Registers
1313 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-86. Clear onEvent toSystem Memory Set3(CESMS3) [offset_TU =F0h]
31 16
CESMS3[95-80]
R/WS-0
15 0
CESMS3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-66. Clear onEvent toSystem Memory Set3(CESMS3) Field Descriptions
Bit Field Value Description
31-0 CESMS3[ n] Clear onEvent toSystem Memory Set3.The register bits0to31correspond tomessage buffers
64to95.Each bitoftheregister enables anautomatic clear ofthecorresponding ETESM3 bitafter
areceive ortransmit event:
0 Noclear.
1 Activate clear.
Figure 26-87. Clear onEvent toSystem Memory Reset 3(CESMR3) [offset_TU =F4h]
31 16
CESMR3
R/WC-0
15 0
CESMR3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-67. Clear onEvent toSystem Memory Reset 3(CESMR3) Field Descriptions
Bit Field Description
31-0 CESMR3 Clear onEvent toSystem Memory Reset 3.The CESMR3 register shows theidentical values toCESMS3 if
read.

<!-- Page 1314 -->

FlexRay Module Registers www.ti.com
1314 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-88. Clear onEvent toSystem Memory Set4(CESMS4) [offset_TU =F8h]
31 16
CESMS4[127-112]
R/WS-0
15 0
CESMS4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-68. Clear onEvent toSystem Memory Set4(CESMS4) Field Descriptions
Bit Field Value Description
31-0 CESMS4[ n] Clear onEvent toSystem Memory Set4.The register bits0to31correspond tomessage buffers
96to127. Each bitoftheregister enables anautomatic clear ofthecorresponding ETESM4 bit
after areceive ortransmit event:
0 Noclear.
1 Activate clear.
Figure 26-89. Clear onEvent toSystem Memory Reset 4(CESMR4) [offset_TU =FCh]
31 16
CESMR4
R/WC-0
15 0
CESMR4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-69. Clear onEvent toSystem Memory Reset 4(CESMR4) Field Descriptions
Bit Field Description
31-0 CESMR4 Clear onEvent toSystem Memory Reset 4.The CESMR4 register shows theidentical values toCESMS4 if
read.

<!-- Page 1315 -->

www.ti.com FlexRay Module Registers
1315 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.23 Transfer toSystem Memory Interrupt Enable Set/Reset (TSMIES[1-4]/TSMIER[1-4])
The Transfer toSystem Memory Interrupt Enable registers enable theinterrupt generation oninterrupt line
TU_Int0, after atransfer tothesystem memory occurred (flagged inTSMO). Four 32-bit Registers reflect
all128MB's.
The bitsaresetbywriting 1toTSMIESx andreset bywriting 1toTSMIERx. Writing a0hasnoeffect.
Reading from both addresses willresult inthesame value.
Figure 26-90. Transfer toSystem Memory Interrupt Enable Set1(TSMIES1) [offset_TU =100h]
31 16
TSMIES1[31-16]
R/WS-0
15 0
TSMIES1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-70. Transfer toSystem Memory Interrupt Enable Set1(TSMIES1) Field Descriptions
Bit Field Value Description
31-0 TTSMIES1[ n] Transfer toSystem Memory Interrupt Enable Set1.The register bits0to31correspond to
message buffers 0to31.Each bitoftheregister enables apotential interrupt, which occurs ifthe
corresponding TSMO1 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-91. Transfer toSystem Memory Interrupt Enable Reset 1(TSMIER1) [offset_TU =104h]
31 16
TSMIER1
R/WC-0
15 0
TSMIER1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-71. Transfer toSystem Memory Interrupt Enable Reset 1(TSMIER1) Field Descriptions
Bit Field Description
31-0 TSMIER1 Transfer toSystem Memory Interrupt Enable Reset 1.The TSMIER1 register shows theidentical values to
TSMIES1 ifread.

<!-- Page 1316 -->

FlexRay Module Registers www.ti.com
1316 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-92. Transfer toSystem Memory Interrupt Enable Set2(TSMIES2) [offset_TU =108h]
31 16
TSMIES2[63-48]
R/WS-0
15 0
TSMIES2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-72. Transfer toSystem Memory Interrupt Enable Set2(TSMIES2) Field Descriptions
Bit Field Value Description
31-0 TSMIES2[ n] Transfer toSystem Memory Interrupt Enable Set2.The register bits0to31correspond to
message buffers 32to63.Each bitoftheregister enables apotential interrupt, which occurs ifthe
corresponding TSMO2 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-93. Transfer toSystem Memory Interrupt Enable Reset 2(TSMIER2) [offset_TU =10Ch]
31 16
TSMIER2
R/WC-0
15 0
TSMIER2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-73. Transfer toSystem Memory Interrupt Enable Reset 2(TSMIER2) Field Descriptions
Bit Field Description
31-0 TSMIER2 Transfer toSystem Memory Interrupt Enable Reset 2.The TSMIER2 register shows theidentical values to
TSMIES2 ifread.

<!-- Page 1317 -->

www.ti.com FlexRay Module Registers
1317 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-94. Transfer toSystem Memory Interrupt Enable Set3(TSMIES3) [offset_TU =110h]
31 16
TSMIES3[95-80]
R/WS-0
15 0
TSMIES3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-74. Transfer toSystem Memory Interrupt Enable Set3(TSMIES3) Field Descriptions
Bit Field Value Description
31-0 TSMIES3[ n] Transfer toSystem Memory Interrupt Enable Set3.The register bits0to31correspond to
message buffers 64to95.Each bitoftheregister enables apotential interrupt, which occurs ifthe
corresponding TSMO3 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-95. Transfer toSystem Memory Interrupt Enable Reset 3(TSMIER3) [offset_TU =114h]
31 16
TSMIER3
R/WC-0
15 0
TSMIER3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-75. Transfer toSystem Memory Interrupt Enable Reset 3(TSMIER3) Field Descriptions
Bit Field Description
31-0 TSMIER3 Transfer toSystem Memory Interrupt Enable Reset 3.The TSMIER3 register shows theidentical values to
TSMIES3 ifread.

<!-- Page 1318 -->

FlexRay Module Registers www.ti.com
1318 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-96. Transfer toSystem Memory Interrupt Enable Set4(TSMIES4) [offset_TU =118h]
31 16
TSMIES4[127-112]
R/WS-0
15 0
TSMIES4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-76. Transfer toSystem Memory Interrupt Enable Set4(TSMIES4) Field Descriptions
Bit Field Value Description
31-0 TSMIES4[ n] Transfer toSystem Memory Interrupt Enable Set4.The register bits0to31correspond to
message buffers 96to127. Each bitoftheregister enables apotential interrupt, which occurs ifthe
corresponding TSMO4 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-97. Transfer toSystem Memory Interrupt Enable Reset 4(TSMIER4) [offset_TU =11Ch]
31 16
TSMIER4
R/WC-0
15 0
TSMIER4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-77. Transfer toSystem Memory Interrupt Enable Reset 4(TSMIER4) Field Descriptions
Bit Field Description
31-0 TSMIER4 Transfer toSystem Memory Interrupt Enable Reset 4.The TSMIER4 register shows theidentical values to
TSMIES4 ifread.

<!-- Page 1319 -->

www.ti.com FlexRay Module Registers
1319 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.24 Transfer toCommunication Controller Interrupt Enable Set/Reset (TCCIES[1-4]/TCCIER[1-4])
The Transfer toCommunication Controller Interrupt Enable registers enables theinterrupt generation on
interrupt lineTU_Int0, after atransfer tothecommunication controller occurred (flagged inTCCO). Four
32-bit Registers reflect all128MBs.
The bitsaresetbywriting 1toTCCIESx andreset bywriting 1toTCCIERx. Writing a0hasnoeffect.
Reading from both addresses willresult inthesame value.
Figure 26-98. Transfer toCommunication Controller Interrupt Enable Set1(TCCIES1)
[offset_TU =120h]
31 16
TCCIES1[31-16]
R/WS-0
15 0
TCCIES1[15-0]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-78. Transfer toCommunication Controller Interrupt Enable Set1(TCCIES1)
Field Descriptions
Bit Field Value Description
31-0 TCCIES1[ n] Transfer toCommunication Controller Interrupt Enable Set1.The register bits0to31correspond
tomessage buffers 0to31.Each bitoftheregister enables apotential interrupt, which occurs ifthe
corresponding TCCO1 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-99. Transfer toCommunication Controller Interrupt Enable Reset 1(TCCIER1)
[offset_TU =124h]
31 16
TCCIER1
R/WC-0
15 0
TCCIER1
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-79. Transfer toCommunication Controller Interrupt Enable Reset 1(TCCIER1)
Field Descriptions
Bit Field Description
31-0 TCCIER1 Transfer toCommunication Controller Interrupt Enable Reset 1.The TCCIER1 register shows theidentical
values toTCCIES1 ifread.

<!-- Page 1320 -->

FlexRay Module Registers www.ti.com
1320 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-100. Transfer toCommunication Controller Interrupt Enable Set2(TCCIES2)
[offset_TU =128h]
31 16
TCCIES2[63-48]
R/WS-0
15 0
TCCIES2[47-32]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-80. Transfer toCommunication Controller Interrupt Enable Set2(TCCIES2)
Field Descriptions
Bit Field Value Description
31-0 TCCIES2[ n] Transfer toCommunication Controller Interrupt Enable Set2.The register bits0to31correspond
tomessage buffers 32to63.Each bitoftheregister enables apotential interrupt, which occurs if
thecorresponding TCCO2 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-101. Transfer toCommunication Controller Interrupt Enable Reset 2(TCCIER2)
[offset_TU =12Ch]
31 16
TCCIER2
R/WC-0
15 0
TCCIER2
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-81. Transfer toCommunication Controller Interrupt Enable Reset 2(TCCIER2)
Field Descriptions
Bit Field Description
31-0 TCCIER2 Transfer toCommunication Controller Interrupt Enable Reset 2.The TCCIER2 register shows theidentical
values toTCCIES2 ifread.

<!-- Page 1321 -->

www.ti.com FlexRay Module Registers
1321 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-102. Transfer toCommunication Controller Interrupt Enable Set3(TCCIES3)
[offset_TU =130h]
31 16
TCCIES3[95-80]
R/WS-0
15 0
TCCIES3[79-64]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-82. Transfer toCommunication Controller Interrupt Enable Set3(TCCIES3)
Field Descriptions
Bit Field Value Description
31-0 TCCIES3[ n] Transfer toCommunication Controller Interrupt Enable Set3.The register bits0to31correspond
tomessage buffers 64to95.Each bitoftheregister enables apotential interrupt, which occurs if
thecorresponding TCCO3 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-103. Transfer toCommunication Controller Interrupt Enable Reset 3(TCCIER3)
[offset_TU =134h]
31 16
TCCIER3
R/WC-0
15 0
TCCIER3
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-83. Transfer toCommunication Controller Interrupt Enable Reset 3(TCCIER3)
Field Descriptions
Bit Field Description
31-0 TCCIER3 Transfer toCommunication Controller Interrupt Enable Reset 3.The TCCIER3 register shows theidentical
values toTCCIES3 ifread.

<!-- Page 1322 -->

FlexRay Module Registers www.ti.com
1322 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleFigure 26-104. Transfer toCommunication Controller Interrupt Enable Set4(TCCIES4)
[offset_TU =138h]
31 16
TCCIES4[127-112]
R/WS-0
15 0
TCCIES4[111-96]
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 26-84. Transfer toCommunication Controller Interrupt Enable Set4(TCCIES4)
Field Descriptions
Bit Field Value Description
31-0 TCCIES4[ n] Transfer toCommunication Controller Interrupt Enable Set4.The register bits0to31correspond
tomessage buffers 96to127. Each bitoftheregister enables apotential interrupt, which occurs if
thecorresponding TCCO4 bitisset:
0 Nointerrupt.
1 Interrupt isgenerated.
Figure 26-105. Transfer toCommunication Controller Interrupt Enable Reset 4(TCCIER4)
[offset_TU =13Ch]
31 16
TCCIER4
R/WC-0
15 0
TCCIER4
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 26-85. Transfer toCommunication Controller Interrupt Enable Reset 4(TCCIER4)
Field Descriptions
Bit Field Description
31-0 TCCIER4 Transfer toCommunication Controller Interrupt Enable Reset 4.The TCCIER4 register shows theidentical
values toTCCIES4 ifread.

<!-- Page 1323 -->

www.ti.com FlexRay Module Registers
1323 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.25 Transfer Configuration RAM (TCR)
The TCR consists of128entries, each 19bitwide. The TCR isECC protected. The ECC protection can
beswitched onoroffbythe4-bit key(PEL(3-0)) intheGlobal Control Set/Reset (GCS/R) registers.
Figure 26-106. Transfer Configuration RAM (TCR) [offset_TU_RAM =0000h -01FFh]
31 19 18 17 16
Reserved STXR THTSM TPTSM
R-0 R/W-0 R/W-0 R/W-0
15 14 13 0
THTCC TPTCC TSO
R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-86. Transfer Configuration RAM (TCR) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 STXR SetTransmit Request.
Control set/reset ofbuffer transmit requests inthecommunication controller.
0 Transfer Unit State Machine willsetIBCM.STXRH to0during atransfer tothecommunication
controller.
1 Transfer Unit State Machine willsetIBCM.STXRH to1during atransfer tothecommunication
controller.
17 THTSM Transfer Header toSystem Memory.
0 Transfer Unit State Machine willnottransfer buffer header tosystem memory.
1 Transfer Unit State Machine willtransfer buffer header tosystem memory.
16 TPTSM Transfer Payload toSystem Memory.
0 Transfer Unit State Machine willnottransfer buffer payload tosystem memory.
1 Transfer Unit State Machine willtransfer buffer payload tosystem memory.
15 THTCC Transfer Header toCommunication Controller.
0 Transfer Unit State Machine willnottransfer buffer header tothecommunication controller.
1 Transfer Unit State Machine willtransfer buffer header tothecommunication controller.
14 TPTCC Transfer Payload toCommunication Controller.
0 Transfer Unit State Machine willnottransfer buffer payload tothecommunication controller.
1 Transfer Unit State Machine willtransfer buffer payload tothecommunication controller.
13-0 TSO Transfer Start Offset.
14-bit buffer address offset insystem memory. The resulting address insystem memory is
computed byadding the32-bit aligned buffer address offset (TSO =buffer address offset bits15:2)
tothebase address defined intheTBA register.
Example: ATSO contents of0x40 results inaTransfer Start Offset of0x40 ×4=0x100

<!-- Page 1324 -->

FlexRay Module Registers www.ti.com
1324 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.1.26 TCR ECC Test Mode
InECC testmode (diagnostic mode isenabled inECC Control Register (ECC_CTRL)) theECC
information isvisible andcanberead orwritten. The corresponding TCR entry canbefound by
subtracting 0x200 from theTCR offset.
Figure 26-107. ECC Information inTCR ECC Test Mode [offset_TU_RAM =200h-3FCh]
31 16
Reserved
R-0
15 6 5 0
Reserved ECCINF(5-0)
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-87. ECC Information inTCR ECC Test Mode Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 ECCINF(5-0) ECC Data ofTCR RAM locations.

<!-- Page 1325 -->

www.ti.com FlexRay Module Registers
1325 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2 Communication Controller Registers
The FlexRay Communication Controller module allocates anaddress space of2Kbytes (0000h to07FFh).
The registers areorganized as32-bit registers. 8/16-bit accesses arealso supported. CPU access tothe
message RAM isdone through theinput andoutput buffers. They buffer data tobetransferred toandfrom
themessage RAM under control ofthemessage handler, avoiding conflicts between CPU accesses and
message reception /transmission. Alternatively toincrease performance theTransfer Unit canbeused for
transferring buffer data.
The testregisters located onaddress 0010h and0014h arewritable only under theconditions.
The assignment ofthemessage buffers isdone according tothescheme shown inFigure 26-108 .The
number Nofavailable message buffers depends onthepayload length oftheconfigured message buffers.
The maximum number ofmessage buffers is128. The maximum payload length supported is254bytes.
The message buffers areseparated intothree consecutive groups; seeFigure 26-108 :
*Static buffers -Transmit /receive buffers assigned tostatic segment
*Static +Dynamic buffers -Transmit /receive buffers assigned tostatic ordynamic segment
*FIFO -Receive FIFO
The message buffer separation configuration canbechanged inDEFAULT_CONFIG orCONFIG state
only byprogramming register MRC.
The firstgroup starts with message buffer 0andconsists ofstatic message buffers only. Message buffer 0
isdedicated tohold thestartup /sync frame orthesingle slotframe, ifthenode transmits one, as
configured bySUCC1.TXST, SUCC1.TXSY, andSUCC1.TSM. Inaddition, message buffer 1may beused
forsync frame transmission incase thatsync frames orsingle-slot frames should have different payloads
onthetwochannels. Inthiscase bitMRC.SPLM hastobeprogrammed to1andmessage buffers 0and1
have tobeconfigured with thekeyslotIDandcanbe(re)configured inDEFAULT_CONFIG orCONFIG
state only.
The second group consists ofmessage buffers assigned tothestatic ortothedynamic segment.
Message buffers belonging tothisgroup may bereconfigured during runtime from dynamic tostatic or
vice versa depending onthestate ofMRC.SEC.
The message buffers belonging tothethird group areconcatenated toasingle receive FIFO.
Figure 26-108. Message Buffer Assignment
Message Buffer 0 ⇓Static Buffers
Message Buffer 1
⇓Static +Dynamic Buffers
...
⇓FIFO
Message Buffer N-1
Message Buffer N
Table 26-88 provides asummary oftheregisters. The base address fortheCommunication Controller
registers isFFF7 C800h.

<!-- Page 1326 -->

FlexRay Module Registers www.ti.com
1326 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-88. Communication Controller Registers
Offset Acronym Register Description Section
Special Registers
00h ECC_CTRL ECC Control Register Section 26.3.2.1.1
04h ECCDSTAT ECC Diagnostic Status Register Section 26.3.2.1.2
08h ECCTEST ECC Test Register Section 26.3.2.1.3
0Ch SBESTAT Single-Bit Error Status Register Section 26.3.2.1.4
10h TEST1 Test Register 1 Section 26.3.2.1.5
14h TEST2 Test Register 2 Section 26.3.2.1.6
1Ch LCK Lock Register Section 26.3.2.1.7
Interrupt Registers
20h EIR Error Interrupt Register Section 26.3.2.2.1
24h SIR Status Interrupt Register Section 26.3.2.2.2
28h EILS Error Interrupt Line Select Register Section 26.3.2.2.3
2Ch SILS Status Interrupt Line Select Register Section 26.3.2.2.4
30h EIES Error Interrupt Enable SetRegister Section 26.3.2.2.5
34h EIER Error Interrupt Enable Reset Register Section 26.3.2.2.5
38h SIES Status Interrupt Enable SetRegister Section 26.3.2.2.6
3Ch SIER Status Interrupt Enable Reset Register Section 26.3.2.2.6
40h ILE Interrupt Line Enable Register Section 26.3.2.2.7
44h T0C Timer 0Configuration Register Section 26.3.2.2.8
48h T1C Timer 1Configuration Register Section 26.3.2.2.9
4Ch STPW1 Stop Watch Register 1 Section 26.3.2.2.10
50h STPW2 Stop Watch Register 2 Section 26.3.2.2.11
Communication Controller Control Registers
80h SUCC1 SUC Configuration Register 1 Section 26.3.2.3.1
84h SUCC2 SUC Configuration Register 2 Section 26.3.2.3.2
88h SUCC3 SUC Configuration Register 3 Section 26.3.2.3.3
8Ch NEMC NEM Configuration Register Section 26.3.2.3.4
90h PRTC1 PRT Configuration Register 1 Section 26.3.2.3.5
94h PRTC2 PRT Configuration Register 2 Section 26.3.2.3.6
98h MHDC MHD Configuration Register 1 Section 26.3.2.3.7
A0h GTUC1 GTU Configuration Register 1 Section 26.3.2.3.8
A4h GTUC2 GTU Configuration Register 2 Section 26.3.2.3.9
A8h GTUC3 GTU Configuration Register 3 Section 26.3.2.3.10
ACh GTUC4 GTU Configuration Register 4 Section 26.3.2.3.11
B0h GTUC5 GTU Configuration Register 5 Section 26.3.2.3.12
B4h GTUC6 GTU Configuration Register 6 Section 26.3.2.3.13
B8h GTUC7 GTU Configuration Register 7 Section 26.3.2.3.14
BCh GTUC8 GTU Configuration Register 8 Section 26.3.2.3.15
C0h GTUC9 GTU Configuration Register 9 Section 26.3.2.3.16
C4h GTUC10 GTU Configuration Register 10 Section 26.3.2.3.17
C8h GTUC11 GTU Configuration Register 11 Section 26.3.2.3.18
Communication Controller Status Registers
100h CCSV Communication Controller Status Vector Register Section 26.3.2.4.1
104h CCEV Communication Controller Error Vector Register Section 26.3.2.4.2
110h SCV Slot Counter Value Register Section 26.3.2.4.3
114h MTCCV Macrotick andCycle Counter Value Register Section 26.3.2.4.4
118h RCV Rate Correction Value Register Section 26.3.2.4.5
11Ch OCV Offset Correction Value Register Section 26.3.2.4.6

<!-- Page 1327 -->

www.ti.com FlexRay Module Registers
1327 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-88. Communication Controller Registers (continued)
Offset Acronym Register Description Section
120h SFS Sync Frame Status Register Section 26.3.2.4.7
124h SWNIT Symbol Window andNITStatus Register Section 26.3.2.4.8
128h ACS Aggregated Channel Status Register Section 26.3.2.4.9
130h-168h ESIDn Even Sync IDRegister [1to15] Section 26.3.2.4.10
170h-1A8h OSIDn Odd Sync IDRegister [1to15] Section 26.3.2.4.11
1B0h-1B8h NMVn Network Management Vector Register [1to3] Section 26.3.2.4.12
Message Buffer Control Registers
300h MRC Message RAM Configuration Register Section 26.3.2.5.1
304h FRF FIFO Rejection Filter Register Section 26.3.2.5.2
308h FRFM FIFO Rejection Filter Mask Register Section 26.3.2.5.3
30Ch FCL FIFO Critical Level Register Section 26.3.2.5.4
Message Buffer Status Registers
310h MHDS Message Handler Status Section 26.3.2.6.1
314h LDTS Last Dynamic Transmit Slot Section 26.3.2.6.2
318h FSR FIFO Status Register Section 26.3.2.6.3
31Ch MHDF Message Handler Constraints Flags Section 26.3.2.6.4
320h TXRQ1 Transmission Request Register 1 Section 26.3.2.6.5
324h TXRQ2 Transmission Request Register 2 Section 26.3.2.6.5
328h TXRQ3 Transmission Request Register 3 Section 26.3.2.6.5
32Ch TXRQ4 Transmission Request Register 4 Section 26.3.2.6.5
330h NDAT1 New Data Register 1 Section 26.3.2.6.6
334h NDAT2 New Data Register 2 Section 26.3.2.6.6
338h NDAT3 New Data Register 3 Section 26.3.2.6.6
33Ch NDAT4 New Data Register 4 Section 26.3.2.6.6
340h MBSC1 Message Buffer Status Changed Register 1 Section 26.3.2.6.7
344h MBSC2 Message Buffer Status Changed Register 2 Section 26.3.2.6.7
348h MBSC3 Message Buffer Status Changed Register 3 Section 26.3.2.6.7
34Ch MBSC4 Message Buffer Status Changed Register 4 Section 26.3.2.6.7
Identification Registers
3F0h CREL Core Release Register Section 26.3.2.7.1
3F4h ENDN Endian Register Section 26.3.2.7.2
Input Buffer
400h-4FCh WRDSn Write Data Section Register [1to64] Section 26.3.2.8.1
500h WRHS1 Write Header Section Register 1 Section 26.3.2.8.2
504h WRHS2 Write Header Section Register 2 Section 26.3.2.8.3
508h WRHS3 Write Header Section Register 3 Section 26.3.2.8.4
510h IBCM Input Buffer Command Mask Register Section 26.3.2.8.5
514h IBCR Input Buffer Command Request Register Section 26.3.2.8.6
Output Buffer
600h-6FCh RDDSn Read Data Section Register [1to64] Section 26.3.2.9.1
700h RDHS1 Read Header Section Register 1 Section 26.3.2.9.2
704h RDHS2 Read Header Section Register 2 Section 26.3.2.9.3
708h RDHS3 Read Header Section Register 3 Section 26.3.2.9.4
70Ch MBS Message Buffer Status Register Section 26.3.2.9.5
710h OBCM Output Buffer Command Mask Register Section 26.3.2.9.6
714h OBCR Output Buffer Command Request Register Section 26.3.2.9.7

<!-- Page 1328 -->

FlexRay Module Registers www.ti.com
1328 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1 Special Registers
26.3.2.1.1 ECC Control Register (ECC_CTRL)
ECC Control Register holds three 4-bit keys. SBEL toturn ECC single-bit error correction onoroff,
SBE_EVT_EN toenable asingle-bit error event andDIAGSEL toenable thediagnostic mode totestthe
ECC single-bit error correction anddouble-bit error detection (SECDED) mechanism. Write access tokey
DIAGSEL isonly possible inprivilege mode.
Figure 26-109 andTable 26-89 illustrate thisregister.
NOTE: Diagnostic mode should beused only forRAM testpurpose inRAM testmode. Therefore,
when entering diagnostic mode, theFlexRay module should beinRAM testmode (TMC(1-0)
setto1inTest Register 1(TEST1)) before performing ECC testing.
Single-bit error correction canonly beactive when ECC isenabled.
Figure 26-109. ECC Control Register (ECC_CTRL) [offset_CC =00h]
31 28 27 24 23 20 19 16
Reserved SBE_EVT_EN Reserved SBEL
R-0 R/W-5h R-0 R/W-Ah
15 4 3 0
Reserved DIAGSEL
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode only; -n=value after reset
Table 26-89. ECC Control Register (ECC_CTRL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 SBE_EVT_EN ECC Single-Bit Error Indication.
5h ECC single-bit error indication isdisabled. OnECC single-bit error detection when reading
from message RAM, transient buffer RAMs, input buffer RAMs andoutput buffer RAMs, the
single-bit error event signal ofthecommunication controller (CC_SBE_err) isactivated. On
ECC single-bit error detection when reading from TCR, thesingle-bit error event signal ofthe
transfer unit(TU_SBE_err) isactivated.
Allother values ECC single-bit error indication isenabled. OnECC single-bit error detection when reading
from message RAM, transient buffer RAMs, input buffer RAMs andoutput buffer RAMs, the
single-bit error event signal ofthecommunication controller (CC_SBE_err) isnotactivated.
OnECC single-bit error detection when reading from TCR, thesingle-bit error event signal of
thetransfer unit(TU_SBE_err) isnotactivated.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 SBEL ECC Single-Bit Error Lock.
5h ECC single-bit error correction isturned off.ECC single-bit errors intheFlexRay RAMs do
notgetcorrected andtheECC algorithm willdetect upto3bitsinerror inaword.
Allother values ECC single-bit error correction isturned on.ECC single-bit errors intheFlexRay RAMs get
corrected.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 DIAGSEL Diagnostic Mode select Key. The 4-bit keyenables ordisables thediagnostic mode.
5h Diagnostic mode isenabled. Double-bit errors willnottrigger theperipheral ECC interrupt.
Allother values Diagnostic mode isdisabled. Double-bit errors willtrigger theperipheral ECC interrupt.

<!-- Page 1329 -->

www.ti.com FlexRay Module Registers
1329 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.2 ECC Diagnostic Status Register (ECCDSTAT)
ECC Diagnostic Status Register holds theECC single-bit error andthedouble-bit error flags when in
diagnostic mode. Aflagiscleared bywriting a1toit.
Figure 26-110 andTable 26-90 illustrate thisregister.
NOTE: Innormal operation mode, double-bit errors areindicated intheMessage Handler Status
(MHDS) register, except forFTU RAM. Adouble-bit error inFTU RAM isindicated bythe
dedicated ECC error flag(PE) intheTransfer Error Interrupt Flag (TEIF) register.
Figure 26-110. ECC Diagnostic Status Register (ECCDSTAT) [offset_CC =04h]
31 24
Reserved
R-0
23 22 21 20 19 18 17 16
DEFH DEFG DEFF DEFE DEFD DEFC DEFB DEFA
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
SEFH SEFG SEFF SEFE SEFD SEFC SEFB SEFA
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 26-90. ECC Diagnostic Status Register (ECCDSTAT) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23 DEFH ECC Double-Bit Error Flag forFTU RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
22 DEFG ECC Double-Bit Error Flag forFlexRay Message RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
21 DEFF ECC Double-Bit Error Flag forTransient Buffer BRAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
20 DEFE ECC Double-Bit Error Flag forTransient Buffer ARAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
19 DEFD ECC Double-Bit Error Flag forOutput Buffer 2RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
18 DEFC ECC Double-Bit Error Flag forOutput Buffer 1RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
17 DEFB ECC Double-Bit Error Flag forInput Buffer 2RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.

<!-- Page 1330 -->

FlexRay Module Registers www.ti.com
1330 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-90. ECC Diagnostic Status Register (ECCDSTAT) Field Descriptions (continued)
Bit Field Value Description
16 DEFA ECC Double-Bit Error Flag forInput Buffer 1RAM.
0 NoECC double-bit error isdetected.
1 ECC double-bit error isdetected anddiagnostic mode isenabled.
15-8 Reserved 0 Reads return 0.Writes have noeffect.
7 SEFH ECC Single-Bit Error Flag forFTU RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
6 SEFG ECC Single-Bit Error Flag forFlexRay Message RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
5 SEFF ECC Single-Bit Error Flag forTransient Buffer BRAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
4 SEFE ECC Single-Bit Error Flag forTransient Buffer ARAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
3 SEFD ECC Single-Bit Error Flag forOutput Buffer 2RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
2 SEFC ECC Single-Bit Error Flag forOutput Buffer 1RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
1 SEFB ECC Single-Bit Error Flag forInput Buffer 2RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.
0 SEFA ECC Single-Bit Error Flag forInput Buffer 1RAM.
0 NoECC single-bit error isdetected.
1 ECC single-bit error isdetected anddiagnostic mode isenabled.

<!-- Page 1331 -->

www.ti.com FlexRay Module Registers
1331 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.3 ECC Test Register (ECCTEST)
The ECC Test Register canbeused indiagnostic mode toread orwrite ECC information ofmessage
RAM, transient buffer RAM, input buffer RAM andoutput buffer RAM locations. Write access tothis
register isonly possible when indiagnostic mode.
Inorder tobeable todirectly access theabove mentioned RAM portions, RAM testmode must be
selected intestregister 1andthecorresponding RAM section must beselected intestregister 2.When
reading acertain RAM location, thecorresponding ECC value isshown inRDECC bitfield. Writing toa
certain ECC location copies thecontents ofWRECC bitfield tothecorresponding ECC location.
Figure 26-111 andTable 26-91 illustrate thisregister.
NOTE: ForFTU RAM, aseparate portion ofmemory-mapped RAM isavailable inTCR ECC test
mode, which canbeaccessed directly forreading orwriting ECC information. See
Section 26.3.1.26 formore details.
Figure 26-111. ECC Test Register (ECCTEST) [offset_CC =08h]
31 23 22 16
Reserved RDECC
R-0 R/W-0
15 7 6 0
Reserved WRECC
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-91. ECC Test Register (ECCTEST) Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 RDECC 0-7Fh Holds ECC bitswhen reading aRAM location.
15-7 Reserved 0 Reads return 0.Writes have noeffect.
6-0 WRECC 0-7Fh ECC bitstobewritten inECC location when writing toaRAM location.

<!-- Page 1332 -->

FlexRay Module Registers www.ti.com
1332 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.4 Single-Bit Error Status Register (SBESTAT)
Innormal operation mode, theSingle-Bit Error Status Register indicates anECC single-bit error bysetting
theSBE flag. Inaddition, itholds thefaulty message buffer number andtheindication ofthebuffer RAM
when anECC single-bit error occurred. The register isupdated without regard tothesingle-bit error lock
setting ofECC Control Register (ECC_CTRL).
Aflagiscleared bywriting a1tothecorresponding bitposition. Writing a0hasnoeffect ontheflag. A
hardware reset orCHI command CLEAR_RAMS willalso clear theregister.
Figure 26-112 andTable 26-92 illustrate thisregister.
NOTE: AnECC single-bit error intheFTU RAM (TCR) isindicated byadedicated TCR Single-Bit
Error Status (TSBESTAT) register.
When oneoftheflags SBEFA, SBEFB, SBEFC, SBEFD, SBEFE, SBEFF andSBEFG
changes from 0to1,theSBE flagissetto1.
Figure 26-112. Single-Bit Error Status Register (SBESTAT) [offset_CC =0Ch]
31 30 16
SBE Reserved
R/W1C-0 R-0
15 14 8
Reserved FMB
R-0 R/W1C-0
7 6 5 4 3 2 1 0
Reserved MFMB FMBD STBF2 STBF1 SMR SOBF SIBF
R-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 26-92. Single-Bit Error Status Register (SBESTAT) Field Descriptions
Bit Field Value Description
31 SBE ECC Single-Bit Error. The flagsignals anECC single-bit error tothehost. The flagissetbythe
ECC logic ofthecommunication controller, when itdetects anECC single-bit error while reading
from oneoftheFlexRay RAM blocks. This flagissetwithout regard totheSBEL bitsetting inthe
ECC Control Register (ECC_CTRL).
Note: ECC multi-bit errors areindicated byaseparate PERR bitintheError Interrupt Register
(EIR).
0 NoECC single-bit error occurred.
1 ECC single-bit error occurred.
30-15 Reserved 0 Reads return 0.Writes have noeffect.
14-8 FMB 0-7Fh Faulty message buffer. AnECC single-bit error occurred when reading from amessage buffer or
when transferring data from Input Buffer orTransient Buffer 1,2tothemessage buffer referenced
byFMB. This value isonly valid when oneoftheflags SIBF, SMR, STBF1, STBF2, andflagFMBD
isset.Itisnotupdated while flagFMBD isset.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 MFMB Multiple message buffers with ECC single-bit error fault detected.
0 Noadditional faulty message buffer.
1 Another faulty message buffer was detected while flagFMBD isset.
5 FMBD Message buffer with ECC single-bit error fault detected.
0 Nofaulty message buffer.
1 Message buffer referenced byFMB holds faulty data duetoanECC single-bit error.
4 STBF2 ECC single-bit error intransient buffer RAM B.
0 NoECC single-bit error.
1 ECC single-bit error occurred when reading transient buffer RAM B.

<!-- Page 1333 -->

www.ti.com FlexRay Module Registers
1333 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-92. Single-Bit Error Status Register (SBESTAT) Field Descriptions (continued)
Bit Field Value Description
3 STBF1 ECC single-bit error intransient buffer RAM A.
0 NoECC single-bit error.
1 ECC single-bit error occurred when reading transient buffer RAM A.
2 SMR ECC single-bit error inmessage RAM.
0 NoECC single-bit error.
1 ECC single-bit error occurred when reading message RAM.
1 SOBF ECC single-bit error inoutput buffer RAM 1,2.
0 NoECC single-bit error.
1 ECC single-bit error occurred when message handler read output buffer RAM 1,2.
0 SIBF ECC single-bit error ininput buffer RAM 1,2.
0 NoECC single-bit error.
1 ECC single-bit error occurred when message handler read input buffer RAM 1,2.

<!-- Page 1334 -->

FlexRay Module Registers www.ti.com
1334 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.5 Test Register 1(TEST1)
Test register 1holds thecontrol bitstoconfigure thetestmodes oftheFlexRay module. Write access to
these bitsisonly possible iftheWRTEN bitisset.Figure 26-113 andTable 26-93 illustrate thisregister.
When theFlexRay module isoperated inoneofitstestmodes thatrequires WRTEN tobeset(RAM Test
Mode, I/OTest Mode, Asynchronous Transmit Mode, andLoop Back Mode) only theselected testmode
functionality isavailable.
NOTE: Toreturn from testmode operation toregular FlexRay operation westrongly recommend to
apply ahardware reset (Power onReset ornReset) toreset allFlexRay internal state
machines totheir initial state.
The testfunctions arenotavailable inaddition tothenormal operational mode functions, they change the
functions ofparts oftheFlexRay module. Therefore, normal operation asspecified outside thischapter
andasrequired bytheFlexRay protocol specification andtheFlexRay conformance testisnotpossible.
Test mode functions may notbecombined with each other orwith FlexRay protocol functions.
NOTE: The FlexRay module should bekept inCONFIG state, while RAM Test Mode TMC =01is
enabled.
The testmode features areintended forhardware testing orforFlexRay busanalyzer tools. They arenot
intended tobeused inFlexRay applications.
Figure 26-113. Test Register 1(TEST1) [offset_CC =10h]
31 28 27 24 23 22 21 20 19 18 17 16
CERB CERA Reserved TXENB TXENA TXB TXA RXB RXA
R-0 R-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R-0
15 10 9 8 7 6 5 4 3 2 1 0
Reserved AOB AOA Reserved TMC Reserved ELBE WRTEN
R-0 R/W-1 R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R=Read only; R/W =Read/Write; -n=value after reset
Table 26-93. Test Register 1(TEST1) Field Descriptions
Bit Field Value Description
31-28 CERB Coding Error Report Channel B.
Setwhen acoding error isdetected onchannel B.Reset to0when register TEST1 isread or
written. Once theCERB issetitwillremain unchanged until theHost accesses theTEST1
register.
0 Nocoding error isdetected.
1h Header CRC error isdetected.
2h Frame CRC error isdetected.
3h Frame Start Sequence FSS toolong.
4h First bitofByte Start Sequence BSS seen LOW.
5h Second bitofByte Start Sequence BSS seen HIGH.
6h First bitofFrame End Sequence FES seen HIGH.
7h Second bitofFrame End Sequence FES seen LOW.
8h CAS /MTS symbol seen tooshort.
9h CAS /MTS symbol seen toolong.
Ah-Fh Reserved

<!-- Page 1335 -->

www.ti.com FlexRay Module Registers
1335 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-93. Test Register 1(TEST1) Field Descriptions (continued)
Bit Field Value Description
27-24 CERA Coding Error Report Channel A.
Setwhen acoding error isdetected onchannel A.Reset to0when register TEST1 isread or
written. Once theCERA issetitwillremain unchanged until theHost accesses theTEST1 register
0 Nocoding error isdetected.
1h Header CRC error isdetected.
2h Frame CRC error isdetected.
3h Frame Start Sequence FSS toolong.
4h First bitofByte Start Sequence BSS seen LOW.
5h Second bitofByte Start Sequence BSS seen HIGH.
6h First bitofFrame End Sequence FES seen HIGH.
7h Second bitofFrame End Sequence FES seen LOW.
8h CAS /MTS symbol seen tooshort.
9h CAS /MTS symbol seen toolong.
Ah-Fh Reserved
Note: Coding errors arealso signaled when thecommunication controller isin
MONITOR_MODE. Theerror codes regarding CAS /MTS symbols concern only the
monitored bitpattern, irrelevant whether those bitpatterns occurred inthesymbol window
orelsewhere.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21 TXENB Control ofchannel Btransmit enable pin.
0 txen2 pindrives a0.
1 txen2 pindrives a1.
20 TXENA Control ofchannel Atransmit enable pin.
0 txen1 pindrives a0.
1 txen1 pindrives a1.
19 TXB Control ofchannel Btransmit pin.
0 txd2 pindrives a0.
1 txd2 pindrives a1.
18 TXA Control ofchannel Atransmit pin.
0 txd1 pindrives a0.
1 txd1 pindrives a1.
17 RXB Monitor channel Breceive pin.
0 rxd2 =0
1 rxd2 =1
16 RXA Monitor channel Areceive pin.
0 rxd1 =0
1 rxd1 =1
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9 AOB Activity onB.The channel idlecondition isspecified intheFlexRay protocol spec v2.1, BITSTRB
process.
0 Noactivity isdetected, channel Bisidle.
1 Activity isdetected, channel Bisnotidle.
8 AOA Activity onA.The channel idlecondition isspecified intheFlexRay protocol spec v2.1, BITSTRB
process.
0 Noactivity isdetected, channel Aisidle.
1 Activity isdetected, channel Aisnotidle.
7-6 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1336 -->

FlexRay Module Registers www.ti.com
1336 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-93. Test Register 1(TEST1) Field Descriptions (continued)
Bit Field Value Description
5-4 TMC Test mode control.
0 Normal operation mode, default.
1h RAM testmode. AllRAM blocks oftheFlexRay module aredirectly accessible bythehost. This
mode isintended toenable testing oftheembedded RAM blocks during production testing.
2h I/Otestmode. The output pins txd1, txd2, txen1, txen2 aredriven tothevalues defined bybits
TXA, TXB, TXENA, TXENB. The values applied totheinput pins rxd1, rxd2 canberead from
register bitsRXA, RXB.
3h Unused. Mapped tonormal operation mode.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 ELBE External Loop Back Enable. There aretwopossibilities toperform aloop back test. External loop
back viaphysical layer orinternal loop back forin-system self-test (default). Incase ofaninternal
loop back pins txen1,2 areintheir inactive state, pins txd1,2 aresettoHIGH, pins rxd1,2 arenot
evaluated. BitELBE isevaluated only when POC isinloop back mode andtestmode control isin
normal operation mode TMC =00.
0 Internal loop back (default).
1 External loop back.
0 WRTEN Write testregister enable. Enables write access tothetestregisters. Tosetthebitfrom 0to1,the
testmode keyhastobewritten asdefined inLock Register (LCK). The unlock sequence isnot
required when WRTEN iskept at1while other bitsoftheregister arechanged. The bitcanbe
reset to0atanytime.
0 Write access tothetestregister isdisabled.
1 Write access tothetestregister isenabled.
26.3.2.1.5.1 Asynchronous Transmit Mode (ATM)
The asynchronous transmit mode isentered bywriting 1110 tothecontroller host interface command
vector CMD intheSUC configuration register 1(controller host interface command: ATM) while the
communication controller isinCONFIG state andbitWRTEN inthetestregister 1issetto1.When called
inanyother state orwhen bitWRTEN isnotset,CMD willbereset to0000 =command_not_accepted.
POCS inthecommunication controller status vector willshow 001110 while theFlexRay module isin
ATM mode.
Asynchronous transmit mode canbeleftbywriting 0001 (controller host interface command: CONFIG) to
thecontroller host interface command vector CMD intheSUC configuration register 1.
InATM mode transmission ofaFlexRay frame istriggered bywriting thenumber ofthecorresponding
message buffer totheinput buffer command request register while bitSTXR intheinput buffer command
mask register issetto1.Inthismode wakeup, startup, andclock synchronization arebypassed, the
controller host interface command SEND_MTS results intheimmediate transmission ofaMTS symbol.
MTS symbols received while operating inATM mode willsetthestatus interrupt flags MTSA,B inthe
Status Interrupt Register likeinmonitor mode.
26.3.2.1.5.2 Loop Back Mode
The loop back mode isentered bywriting 1111 tothecontroller host interface command vector CMD(3-0)
intheSUC configuration register 1(controller host interface command: LOOP_BACK) while the
communication controller isinCONFIG state andbitWRTEN inthetestregister 1issetto1.This write
operation hastobedirectly preceded bytwoconsecutive write accesses totheConfiguration Lock Key
(unlock sequence). When called inanyother state orwhen bitWRTEN isnotset,CMD willbereset to
0000 =command_not_accepted. POCS inthecommunication controller status vector willshow 001101
while theFlexRay module isinloop back mode.
Loop back mode canbeleftbywriting 0001 (controller host interface command: CONFIG) tothecontroller
host interface command vector CMD intheSUC configuration register 1.

<!-- Page 1337 -->

www.ti.com FlexRay Module Registers
1337 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleThe loop back mode isintended tocheck themodules internal data paths. Normal, time triggered
operation isnotpossible inloop back mode.
There aretwopossibilities toperform aloop back test. External loop back through thephysical layer
(TEST1.ELBE =1)orinternal loop back forin-system self-test (TEST1.ELBE =0).Incase ofaninternal
loop back pins txen1,2_n areintheir inactive state, pins txd1,2 areset,pins rxd1,2 arenotevaluated.
When thecommunication controller isinloop back mode, aloop back testisstarted bythehost writing a
message totheinput buffer andrequesting thetransmission bywriting totheinput buffer command
request register. The message handler willtransfer themessage intothemessage RAM andthen intothe
transient buffer oftheselected channel. The channel protocol controller (PRT) willread (in32-bit words)
themessage from thetransmit part ofthetransient buffer andload itintoitsRx/Txshift register. The
serial transmission islooped back intotheshift register; itscontent iswritten intothereceive part ofthe
channels transient buffer before thenext word isloaded.
The PRT andthemessage handler willthen treat thistransmitted message likeareceived message,
perform anacceptance filtering onframe IDandreceive channel, andstore themessage intothe
message RAM (assuming themessage passed theacceptance filter, thus testing theacceptance filter
logic). The loop back testends with thehost requesting thisreceived message from themessage RAM
andthen checking thecontents oftheoutput buffer.
Each FlexRay channel istested separately. The FlexRay module cannot receive messages from the
FlexRay buswhile itisintheloop back mode.
The cycle counter value offrames used inloop back mode canbeprogrammed bywriting totheCCV bits
oftheMTCCV register (writable inATM andloop back mode only).
NOTE: Incase ofanoddpayload thelasttwobytes ofthelooped-back payload willberight aligned
(shifted by16bitstotheright) inside thelast32-bit data word.
The controller host interface command SEND_MTS results intheimmediate transmission ofanMTS
symbol. Transmitted MTS symbols willnotcause status interrupt flags MTSA,B tobesetintheStatus
Interrupt Register. MTS symbols received while operating inloop back mode willsetstatus interrupt flags
MTSA,B inSystem Interrupt Register likeinmonitor mode. The reception ofanMTS symbol canbe
emulated bydriving theFlexRay receive pins RxD1,2 tolowfortheduration ofthesymbol inexternal loop
back mode, orbydriving theFlexRay pins TxD1,2 andTxEN1,2 tolowusing theTXA,B andTXENA,B of
Test Register1 ininternal orexternal loop back mode.

<!-- Page 1338 -->

FlexRay Module Registers www.ti.com
1338 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.6 Test Register 2(TEST2)
Test register 2holds allbitsrequired forRAM testoftheembedded RAM blocks ofthecommunication
controller. Write access tothisregister isonly possible when bitWRTEN inthetestregister 1isset.
Figure 26-114 andTable 26-94 illustrate thisregister.
Figure 26-114. Test Register 2(TEST2) [offset_CC =14h]
31 16
Reserved
R-0
15 14 13 7 6 4 3 1 0
RDPB WRPB Reserved SSEL Rsvd RS
R-0 R/W-0 R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-94. Test Register 2(TEST2) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 RDPB 0-1 When ECC mode isenabled, thisbitisalways read as0.
14 WRPB 0-1 When ECC mode isenabled, thisbithasnoeffect.
13-7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 SSEL Segment select. Toenable access tothecomplete message RAM (8192 byte addresses) the
message RAM issegmented.
0 Access toRAM bytes 0000h to03FFh isenabled.
1h Access toRAM bytes 0400h to07FFh isenabled.
2h Access toRAM bytes 0800h to0BFFh isenabled.
3h Access toRAM bytes 0C00h to0FFFh isenabled.
4h Access toRAM bytes 1000h to13FFh isenabled.
5h Access toRAM bytes 1400h to17FFh isenabled.
6h Access toRAM bytes 1800h to1BFFh isenabled.
7h Access toRAM bytes 1C00h to1FFFh isenabled.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 RS RAM select. InRAM testmode, theRAM blocks selected byRSaremapped tomodule address
400h to7FFh (1024 byte addresses).
0 Input buffer RAM 1
1h Input buffer RAM 2
2h Output buffer RAM 1
3h Output buffer RAM 2
4h Transient buffer RAM A
5h Transient buffer RAM B
6h Message RAM
7h Reserved

<!-- Page 1339 -->

Normal
Operation RAM Test
OBF2TBF1 TBF2RS(2-0) =
SSEL(2-0) =
000000h
3FCh
400h000                 001                010                011                 100                101                  110offset_CC
7FChOBF1 IBF2 IBF1 001
010
011
100
101
110
111
MBFOutput
Register SetInput and
www.ti.com FlexRay Module Registers
1339 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.6.1 RAM Test Mode
InRAM testmode [TMC =01],oneoftheseven RAM blocks canbeselected fordirect read andwrite
access byprogramming theRSfield tothecorresponding value; seeFigure 26-115 .
Forexternal RAM access inRAM testmode, theselected RAM block ismapped totheaddress range
offset_CC 400h to7FFh, which istheaddress space fortheinput andoutput buffer register sets innormal
operation. Hence, thefunctionality oftheinput andoutput buffer register sets isnotavailable inRAM test
mode.
With theavailable address space (offset_CC 400h to7FFh) inRAM testmode, 1024 bytes ofRAM canbe
addressed fordirect access. Since thelength oftheMessage RAM exceeds theavailable address space,
theMessage RAM issegmented intosegments of1024 bytes. The segments canbeselected by
programming thebitsSSEL(2-0) oftestregister 2.
Figure 26-115. Test Mode Access toCommunication Controller RAM Blocks

<!-- Page 1340 -->

FlexRay Module Registers www.ti.com
1340 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.1.7 Lock Register (LCK)
The lock register iswrite-only. Reading theregister willreturn 00.
Figure 26-116 andTable 26-95 illustrate thisregister.
Figure 26-116. Lock Register (LCK) [offset_CC =1Ch]
31 16
Reserved
R-0
15 8 7 0
TMK CLK
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-95. Lock Register (LCK) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 TMK 0-1FFh Test mode key. Towrite bitWRTEN inthetestregister to1,thewrite operation hastobedirectly
preceded bytwoconsecutive write accesses tothetestmode key(unlock sequence). Ifthiswrite
sequence isinterrupted byother write accesses, bitWRTEN isnotsetto1andthesequence has
toberepeated.
First write (LCK.TMK): 75h=0b0111 0101
Second write (LCK.TMK): 8Ah =0b1000 1010
Third write: TEST1.WRTEN =1
7-0 CLK 0-FFh Configuration lock key. Toleave CONFIG state bywriting toCMD intheSUC configuration
register 1(commands READY; MONITOR_MODE; ATM; LOOP_BACK), thewrite operation hasto
bedirectly preceded bytwoconsecutive write accesses totheconfiguration lock key(unlock
sequence). Ifthiswrite sequence isinterrupted byother write accesses, thecommunication
controller remains inCONFIG state andthesequence hastoberepeated.
First write (LCK.CLK): CEh =0b1100 1110
Second write (LCK.CLK): 31h=0b0011 0001
Third write (SUCC.CMD)
NOTE: Incase thattheHost uses 8/16-bit accesses towrite thelisted bitfields, theprogrammer has
toensure thatno"dummy accesses" (forexample, theremaining register bytes /words) are
inserted bythecompiler.

<!-- Page 1341 -->

www.ti.com FlexRay Module Registers
1341 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2 Interrupt Registers
26.3.2.2.1 Error Interrupt Register (EIR)
The flags aresetwhen thecommunication controller detects oneofthelisted error conditions. They
remain setuntil thehost clears them. Aflagiscleared bywriting a1tothecorresponding bitposition.
Writing a0hasnoeffect ontheflag. Areset willalso clear theregister.
Figure 26-117 andTable 26-96 illustrate thisregister.
Figure 26-117. Error Interrupt Register (EIR) [offset_CC =20h]
31 27 26 25 24 23 19 18 17 16
Reserved TABB LTVB EDB Reserved TABA LTVA EDA
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
15 12 11 10 9 8 7 6 5 4 3 2 1 0
Reserved MHF IOBA IIBA EFA RFO PERR CCL CCF SFO SFBM CNA PEMC
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-96. Error Interrupt Register (EIR) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 TABB Transmission Across Boundary Channel B.The flagsignals totheHost thatatransmission across
aslotboundary occurred forchannel B.
0 Notransmission across slotboundary isdetected onchannel B.
1 Transmission across slotboundary isdetected onchannel B.
25 LTVB Latest transmit violation channel B.The flagsignals alatest transmit violation onchannel Btothe
host.
0 Nolatest transmit violation isdetected onchannel B.
1 Latest transmit violation isdetected onchannel B.
24 EDB Error detected onchannel B.This bitissetwhenever oneoftheflags SEDB, CEDB, CIB, SBVB in
theAggregated channel status register isset.
0 Noerror isdetected onchannel B.
1 Error isdetected onchannel B.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 TABA Transmission Across Boundary Channel A.The flagsignals totheHost thatatransmission across
aslotboundary occurred forchannel A.
0 Notransmission across slotboundary isdetected onchannel A.
1 Transmission across slotboundary isdetected onchannel A.
17 LTVA Latest transmit violation channel A.The flagsignals alatest transmit violation onchannel Atothe
host.
0 Nolatest transmit violation isdetected onchannel A.
1 Latest transmit violation isdetected onchannel A.
16 EDA Error detected onchannel A.This bitissetwhenever oneoftheflags SEDA, CEDA, CIA, SBVA in
theAggregated channel status register isset.
0 Noerror isdetected onchannel A.
1 Error isdetected onchannel A.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 MHF Message Handler Constraints Flag. The flagsignals aMessage Handler constraints violation
condition. Itissetwhenever oneoftheflags MHDF.SNUA, MHDF.SNUB, MHDF.FNFA,
MHDF.FNFB, MHDF.TBFA, MHDF.TBFB, MHDF.WAHP changes from 0to1.
0 NoMessage Handler failure isdetected.
1 Message Handler failure isdetected.

<!-- Page 1342 -->

FlexRay Module Registers www.ti.com
1342 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-96. Error Interrupt Register (EIR) Field Descriptions (continued)
Bit Field Value Description
10 IOBA Illegal Output buffer Access. This flagissetbythecommunication controller when theHost
requests thetransfer ofamessage buffer from theMessage RAM totheOutput Buffer while
OBCR.OBSYS issetto1.
0 Noillegal Host access toOutput Buffer occurred.
1 Illegal Host access toOutput Buffer occurred.
9 IIBA Illegal Input Buffer Access. This flagissetbythecommunication controller when theHost wants to
modify amessage buffer viaInput Buffer andoneofthefollowing conditions applies:
*The communication controller isnotinCONFIG orDEFAULT_CONFIG state andtheHost writes
totheInput Buffer Command Request register tomodify thefollowing:
-theHeader section ofmessage buffer 0,1ifconfigured fortransmission inkeyslot
-theHeader section ofstatic message buffers with buffer number <MRC.FDB while
MRC.SEC =01
-theHeader section ofanystatic ordynamic message buffer while MRC.SEC =1x
-Header and/ordata section ofanymessage buffer belonging tothereceive FIFO
*The Host writes toanyregister oftheInput Buffer while IBCR.IBSYH issetto1.
0 Noillegal Host access toInput Buffer occurred.
1 Illegal Host access toInput Buffer occurred.
8 EFA Empty FIFO Access. This flagissetbythecommunication controller when theHost requests the
transfer ofamessage from thereceive FIFO viaOutput Buffer while thereceive FIFO isempty.
0 NoHost access toempty FIFO occurred.
1 Host access toempty FIFO occurred.
7 RFO Receive FIFO overrun. This flagissetbythecommunication controller when areceive FIFO
overrun was detected. The flagiscleared bythenext FIFO read access ofthehost. After thisread
access oneposition intheFIFO isempty again.
0 Noreceive FIFO overrun isdetected.
1 Areceive FIFO overrun isdetected.
6 PERR ECC error. The flagsignals anECC multi-bit error tothehost. The flagissetbytheECC logic of
thecommunication controller, when itdetects anECC multi-bit error while reading from oneofthe
FlexRay RAM blocks.
Note: ECC single-bit errors areindicated byaseparate SBE bitintheSingle-Bit Error Status
Register (SBESTAT).
0 NoECC multi-bit error isdetected.
1 ECC multi-bit error isdetected.
5 CCL CHI Command Locked. The flagsignals thatthewrite access totheCHI command vector
SUCC1.CMD was notsuccessful because itcoincided with aPOC state change triggered by
protocol functions. Inthiscase bitCNA isalso setto1.
0 CHI command isaccepted.
1 CHI command isnotaccepted.
4 CCF Clock correction failure. This flagissetattheendofthecycle whenever oneofthefollowing errors
occurred:
*Missing rate correction signal
*Missing offset correction signal
*Clock correction Failed counter stopped at15
*Clock correction Limit Reached
The clock correction status ismonitored inthecommunication controller error vector andsync
frame status register.
0 Noclock correction error.
1 Clock correction failed.
3 SFO Sync frame overflow. Setwhen either thenumber ofsync frames received during thelast
communication cycle orthetotal number ofdifferent sync frame IDsreceived during thelastdouble
cycle exceeds themaximum number ofsync frames asdefined bySNM intheGTU configuration
register 2.
0 Number ofreceived sync frames intheconfigured range.
1 More sync frames received than configured bySNM.

<!-- Page 1343 -->

www.ti.com FlexRay Module Registers
1343 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-96. Error Interrupt Register (EIR) Field Descriptions (continued)
Bit Field Value Description
2 SFBM Sync frames below minimum. This flagsignals thatthenumber ofsync frames received during the
lastcommunication cycle was below thelimit required bytheFlexRay protocol. The minimum
number ofsync frames percommunication cycle is2.
0 Two ormore sync frames arereceived during lastcommunication cycle.
1 Less than twosync frames arereceived during lastcommunication cycle.
1 CNA Command notaccepted. The flagsignals thatthecontroller host interface command vector CMD in
theSUC configuration register 1was reset to0000 duetoanunaccepted controller host interface
command.
0 Controller host interface command isaccepted.
1 Controller host interface command isnotaccepted.
0 PEMC POC error mode changed. This flagissetwhenever theerror mode signaled byERRM inthe
communication controller error vector register haschanged.
0 Error mode hasnotchanged.
1 Error mode haschanged.
26.3.2.2.2 Status Interrupt Register (SIR)
The flags aresetbythecommunication controller when acorresponding event occurs. They remain set
until thehost clears them. Ifenabled, aninterrupt ispending while oneofthebitsisset.Aflagiscleared
bywriting a1tothecorresponding bitposition. Writing a0hasnoeffect ontheflag. Ahardware reset will
also clear theregister.
Figure 26-118 andTable 26-97 illustrate thisregister.
Figure 26-118. Status Interrupt Register (SIR) [offset_CC =24h]
31 26 25 24 23 18 17 16
Reserved MTSB WUPB Reserved MTSA WUPA
R-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
SDS MBSI SUCS SWE TOBC TIBC TI1 TI0 NMVC RFCL RFNE RXI TXI CYCS CAS WST
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-97. Status Interrupt Register (SIR) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25 MTSB MTS received onchannel B.Media access testsymbol received onchannel Bduring thelast
symbol window. Updated bythecommunication controller foreach channel attheendofthe
symbol window.
0 NoMTS symbol isreceived.
1 MTS symbol isreceived.
24 WUPB Wakeup pattern channel B.This flagissetbythecommunication controller when awakeup pattern
was received onchannel B.
0 Nowakeup pattern isonchannel B.
1 Wakeup pattern isonchannel B.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 MTSA MTS received onchannel A.Media access testsymbol received onchannel Aduring thelast
symbol window. Updated bythecommunication controller foreach channel attheendofthe
symbol window.
0 NoMTS symbol isreceived.
1 MTS symbol isreceived.

<!-- Page 1344 -->

FlexRay Module Registers www.ti.com
1344 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-97. Status Interrupt Register (SIR) Field Descriptions (continued)
Bit Field Value Description
16 WUPA Wakeup pattern channel A.This flagissetbythecommunication controller when awakeup pattern
was received onchannel A.
0 Nowakeup pattern isonchannel A.
1 Wakeup pattern isonchannel A.
15 SDS Start ofDynamic Segment. This flagissetbythecommunication controller when thedynamic
segment starts.
0 Dynamic segment isnotyetstarted.
1 Dynamic segment isstarted.
14 MBSI Message buffer status interrupt. This flagissetbythecommunication controller ifbitMBI ofa
dedicated receive buffer issetto1andwhen thestatus ofthatmessage buffer hasbeen updated
duetoreception ofa:
*valid frame with payload
*valid frame with payload zero
*nullframe
*corrupted frame oranempty slot
0 Nomessage buffer status hasbeen updated.
1 Message buffer status ofatleast onereceive buffer hasbeen updated.
13 SUCS Startup completed successfully. This flagissetwhenever astartup completed successfully andthe
communication controller entered NORMAL_ACTIVE state.
0 Nostartup iscompleted successfully.
1 Startup iscompleted successfully.
12 SWE Stop watch event. Ifenabled bytherespective control bitslocated intheStop watch register, a
detected edge onexternal stop watch pinorasoftware trigger event willgenerate astop watch
event.
0 Nostop watch event.
1 Stop watch event occurred.
11 TOBC Transfer output buffer completed. This flagissetwhenever atransfer from themessage RAM to
theoutput buffer hascompleted andbitOBSYS intheoutput buffer command request register has
been reset bythemessage handler.
0 Notransfer iscompleted since bitwas reset.
1 Transfer between message RAM andoutput buffer iscompleted.
10 TIBC Transfer input buffer completed. This flagissetwhenever atransfer from input buffer tothe
message RAM hascompleted andbitIBSYS intheinput buffer command request register has
been reset bythemessage handler.
0 Notransfer iscompleted since bitwas reset.
1 Transfer between input buffer andmessage RAM iscompleted.
9 TI1 Timer interrupt 1.This flagissetwhenever theconditions programmed inthetimer interrupt 1
configuration register aremet. Atimer interrupt 1isalso signaled onpinCC_tint1.
0 Notimer interrupt 1.
1 Timer interrupt 1occurred.
8 TI0 Timer interrupt 0.This flagissetwhenever theconditions programmed inthetimer interrupt 0
configuration register aremet. Atimer interrupt 0isalso signaled onpinCC_tint0.
0 Notimer interrupt 0.
1 Timer interrupt 0occurred.
7 NMVC Network management vector changed. This interrupt flagsignals achange intheNetwork
management vector visible tothehost.
0 Nochange inthenetwork management vector.
1 Network management vector ischanged.
6 RFCL Receive FIFO critical level. This flagissetwhen thereceive FIFO filllevel FSR.RFFL isequal or
greater than thecritical level asconfigured byFCL.CL.
0 Receive FIFO isbelow critical level.
1 Receive FIFO critical level isreached.

<!-- Page 1345 -->

www.ti.com FlexRay Module Registers
1345 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-97. Status Interrupt Register (SIR) Field Descriptions (continued)
Bit Field Value Description
5 RFNE Receive FIFO notempty. This flagissetbythecommunication controller when areceived valid
frame was stored intotheempty receive FIFO. The actual state ofthereceive FIFO ismonitored in
register FSR.
0 Receive FIFO isempty.
1 Receive FIFO isnotempty.
4 RXI Receive interrupt. This flagissetbythecommunication controller when thepayload segment ofa
received valid frame was stored intothedata section ofamatching dedicated receive buffer andif
bitMBI ofthatmessage buffer issetto1.
0 Nodata section hasbeen updated.
1 Atleast onedata section hasbeen updated.
3 TXI Transmit interrupt. This flagissetbythecommunication controller after successful frame
transmission ifbitMBI intherespective message buffer issetto1.
0 Noframe istransmitted.
1 Atleast oneframe was transmitted successfully.
2 CYCS Cycle start interrupt. This flagissetbythecommunication controller when acommunication cycle
starts.
0 Nocommunication cycle isstarted.
1 Communication cycle isstarted.
1 CAS Collision avoidance symbol. This flagissetbythecommunication controller when aCAS was
received.
0 NoCAS symbol isreceived.
1 CAS symbol isreceived.
0 WST This flagissetwhen WSV inthecommunication controller status vector register changes toavalue
other than UNDEFINED.
0 Wakeup status isunchanged.
1 Wakeup status ischanged.

<!-- Page 1346 -->

FlexRay Module Registers www.ti.com
1346 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.3 Error Interrupt Line Select (EILS)
The settings intheerror interrupt lineselect register assigns aninterrupt generated byaspecific error
interrupt flagtooneofthetwomodule interrupt lines (CC_int0 orCC_int1).
Figure 26-119 andTable 26-98 illustrate thisregister.
Figure 26-119. Error Interrupt Line Select Register (EILS) [offset_CC =28h]
31 27 26 25 24 23 19 18 17 16
Reserved TABBL LTVBL EDBL Reserved TABAL LTVAL EDAL
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
15 12 11 10 9 8 7 6 5 4 3 2 1 0
Reserved MHFL IOBAL IIBAL EFAL RFOL UCREL CCLL CCFL SFOL SFBML CNAL PEMCL
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-98. Error Interrupt Line Select Register (EILS) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 TABBL Transmission across boundary channel Binterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
25 LTVBL Latest transmit violation channel Binterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
24 EDBL Error detected onchannel Binterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 TABAL Transmission across boundary channel Ainterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
17 LTVAL Latest transmit violation channel Ainterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
16 EDAL Error detected onchannel Ainterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 MHFL Message handler constraints flaginterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
10 IOBAL Illegal output buffer access interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
9 IIBAL Illegal output buffer access interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
8 EFAL Empty FIFO access interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.

<!-- Page 1347 -->

www.ti.com FlexRay Module Registers
1347 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-98. Error Interrupt Line Select Register (EILS) Field Descriptions (continued)
Bit Field Value Description
7 RFOL Receive FIFO overrun interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
6 UCREL Uncorrectable RAM error interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
5 CCLL CHI command locked interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
4 CCFL Clock correction failure interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
3 SFOL Sync frame overflow interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
2 SFBML Sync frames below minimum interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
1 CNAL Command notaccepted interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
0 PEMCL POC error mode changed interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.

<!-- Page 1348 -->

FlexRay Module Registers www.ti.com
1348 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.4 Status Interrupt Line Select (SILS)
The settings inthestatus interrupt lineselect register assign aninterrupt generated byaspecific status
interrupt flagtooneofthetwomodule interrupt lines (CC_int0 orCC_int1).
Figure 26-120 andTable 26-99 illustrate thisregister.
Figure 26-120. Status Interrupt Line Select Register (SILS) [offset_CC =2Ch]
31 26 25 24 23 18 17 16
Reserved MTSBL WUPBL Reserved MTSAL WUPAL
R-0 R/W-1 R/W-1 R-0 R/W-1 R/W-1
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
SDSL MBSIL SUCSL SWEL TOBCL TIBCL TI1L TI0L NMVCL RFFL RFNEL RXIL TXIL CYCSL CASL WSTL
R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1 R/W-1
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-99. Status Interrupt Line Select Register (SILS) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25 MTSBL Media access testsymbol channel Binterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
24 WUPBL Wakeup pattern channel Binterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 MTSAL Media access testsymbol channel Ainterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
16 WUPAL Wakeup pattern channel Ainterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
15 SDSL Start ofDynamic Segment Interrupt Line
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
14 MBSIL Message buffer status interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
13 SUCSL Startup completed Successfully interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
12 SWEL Stop watch event interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
11 TOBCL Transfer output buffer completed interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
10 TIBCL Transfer input buffer completed interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.

<!-- Page 1349 -->

www.ti.com FlexRay Module Registers
1349 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-99. Status Interrupt Line Select Register (SILS) Field Descriptions (continued)
Bit Field Value Description
9 TI1L Timer interrupt 1line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
8 TI0L Timer interrupt 0line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
7 NMVCL Network management vector changed interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
6 RFCLL Receive FIFO fullinterrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
5 RFNEL Receive FIFO notempty interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
4 RXIL Receive interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
3 TXIL Transmit interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
2 CYCSL Cycle start interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
1 CASL Collision Avoidance symbol interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.
0 WSTL Wakeup status interrupt line.
0 Interrupt isassigned tointerrupt lineCC_int0.
1 Interrupt isassigned tointerrupt lineCC_int1.

<!-- Page 1350 -->

FlexRay Module Registers www.ti.com
1350 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.5 Error Interrupt Enable Set/Reset (EIES/EIER)
The settings intheerror interrupt enable register determine which status changes intheerror interrupt
register willresult inaninterrupt. The enable bitsaresetbywriting toEIES (address 30h) andreset by
writing toEIER (address 34h). Writing 1sets orresets thespecific enable bit,writing 0hasnoeffect.
Figure 26-121 andTable 26-100 illustrate thisregister.
Figure 26-121. Error Interrupt Enable Set/Reset Register (EIES/EIER) [offset_CC =30h/34h]
31 27 26 25 24 23 19 18 17 16
Reserved TABBE LTVBE EDBE Reserved TABAE LTVAE EDAE
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
15 12 11 10 9 8 7 6 5 4 3 2 1 0
Reserved MHFE IOBAE IIBAE EFAE RFOE UCREE CCLE CCFE SFOE SFBME CNAE PEMCE
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-100. Error Interrupt Set/Reset Register (EIES/EIER) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 TABBE Transmission across boundary channel Binterrupt enable.
0 Interrupt isdisabled.
1 Transmission across boundary channel Binterrupt isenabled.
25 LTVBE Latest transmit violation channel Binterrupt enable.
0 Interrupt isdisabled.
1 Latest transmit violation channel Binterrupt isenabled.
24 EDBE Error detected onchannel Binterrupt enable.
0 Interrupt isdisabled.
1 Error detected onchannel Binterrupt isenabled.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 TABAE Transmission across boundary channel Ainterrupt enable.
0 Interrupt isdisabled.
1 Transmission across boundary channel Ainterrupt isenabled.
17 LTVAE Latest transmit violation channel Ainterrupt enable.
0 Interrupt isdisabled.
1 Latest transmit violation channel Ainterrupt isenabled.
16 EDAE Error detected onchannel Ainterrupt enable.
0 Interrupt isdisabled.
1 Error detected onchannel Ainterrupt isenabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 MHFE Message handler constraints flaginterrupt enable.
0 Interrupt isdisabled.
1 Message handler constraints flaginterrupt isenabled.
10 IOBAE Illegal output buffer access interrupt enable.
0 Interrupt isdisabled.
1 Illegal output buffer access interrupt isenabled.
9 IIBAE Illegal input buffer access interrupt enable.
0 Interrupt isdisabled.
1 Illegal input buffer access interrupt isenabled.
8 EFAE Empty FIFO access interrupt enable.
0 Interrupt isdisabled.
1 Empty FIFO access interrupt isenabled.

<!-- Page 1351 -->

www.ti.com FlexRay Module Registers
1351 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-100. Error Interrupt Set/Reset Register (EIES/EIER) Field Descriptions (continued)
Bit Field Value Description
7 RFOE Receive FIFO overrun interrupt enable.
0 Interrupt isdisabled.
1 Receive FIFO overrun interrupt isenabled.
6 UCREE Uncorrectable RAM error interrupt enable. Anuncorrectable RAM error canbecaused by:
*anECC single-bit error, ifECC single-bit error correction isdisabled
*anECC double-bit error
0 Interrupt isdisabled.
1 Uncorrectable RAM error interrupt isenabled.
5 CCLE CHI command locked interrupt enable.
0 Interrupt isdisabled.
1 CHI command locked interrupt isenabled.
4 CCFE Clock correction failure interrupt enable.
0 Interrupt isdisabled.
1 Clock correction failure interrupt isenabled.
3 SFOE Sync frame overflow interrupt enable.
0 Interrupt isdisabled.
1 Sync frame overflow interrupt isenabled.
2 SFBME Sync frames below minimum interrupt enable.
0 Interrupt isdisabled.
1 Sync frames below minimum interrupt isenabled.
1 CNAE Command notAccepted interrupt enable.
0 Interrupt isdisabled.
1 Command notvalid interrupt isenabled.
0 PEMCE POC error mode changed interrupt enable.
0 Interrupt isdisabled.
1 Protocol error mode changed interrupt isenabled.

<!-- Page 1352 -->

FlexRay Module Registers www.ti.com
1352 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.6 Status Interrupt Enable Set/Reset Register (SIES/SIER)
The settings inthestatus interrupt enable register determine which status changes inthestatus interrupt
register willresult inaninterrupt. The enable bitsaresetbywriting toSIES (address 38h) andreset by
writing toSIER (address 3Ch). Writing 1sets orresets thespecific enable bit,writing 0hasnoeffect.
Figure 26-122 andTable 26-101 illustrate thisregister.
Figure 26-122. Status Interrupt Enable Set/Reset Register (SIES/SIER) [offset_CC =38h/3Ch]
31 26 25 24 23 18 17 16
Reserved MTSBE WUPBE Reserved MTSAE WUPAE
R-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
SDSE MBSIE SUCSE SWEE TOBCE TIBCE TI1E TI0E NMVCE RFFE RFNEE RXIE TXIE CYCSE CASE WSTE
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-101. Status Interrupt Enable Set/Reset Register (SIES/SIER) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25 MTSBE MTS received onchannel Binterrupt enable.
0 Interrupt isdisabled.
1 MTS received onchannel Binterrupt isenabled.
24 WUPBE Wakeup pattern channel Binterrupt enable.
0 Interrupt isdisabled.
1 Wakeup pattern channel Binterrupt isenabled.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 MTSAE MTS received onchannel Ainterrupt enable.
0 Interrupt isdisabled.
1 MTS received onchannel Ainterrupt isenabled.
16 WUPAE Wakeup pattern channel Ainterrupt enable.
0 Interrupt isdisabled.
1 Wakeup pattern channel Ainterrupt isenabled.
15 SDSE Start ofdynamic segment interrupt enable.
0 Interrupt isdisabled.
1 Start ofdynamic segment interrupt isenabled.
14 MBSIE Message buffer status interrupt enable.
0 Interrupt isdisabled.
1 Message buffer status interrupt isenabled.
13 SUCSE Startup completed successfully interrupt enable.
0 Interrupt isdisabled.
1 Startup completed successfully interrupt isenabled.
12 SWEE Stop watch event interrupt enable.
0 Interrupt isdisabled.
1 Stop watch event interrupt isenabled.
11 TOBCE Transfer output buffer completed interrupt enable.
0 Interrupt isdisabled.
1 Transfer output buffer completed interrupt isenabled.
10 TIBCE Transfer input buffer completed interrupt enable.
0 Interrupt isdisabled.
1 Transfer input buffer completed interrupt isenabled.

<!-- Page 1353 -->

www.ti.com FlexRay Module Registers
1353 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-101. Status Interrupt Enable Set/Reset Register (SIES/SIER) Field Descriptions (continued)
Bit Field Value Description
9 TI1E Timer interrupt 1enable.
0 Interrupt isdisabled.
1 Timer interrupt 1isenabled.
8 TI0E Timer interrupt 0enable.
0 Interrupt isdisabled.
1 Timer interrupt 0isenabled.
7 NMVCE Network management vector changed interrupt enable.
0 Interrupt isdisabled.
1 Network management vector changed interrupt isenabled.
6 RFCLE Receive FIFO fullinterrupt enable.
0 Interrupt isdisabled.
1 Receive FIFO overrun interrupt isenabled.
5 RFNEE Receive FIFO notempty interrupt enable.
0 Interrupt isdisabled.
1 Receive FIFO notempty interrupt isenabled.
4 RXIE Receive interrupt enable.
0 Interrupt isdisabled.
1 Receive interrupt isenabled.
3 TXIE Transmit interrupt enable.
0 Interrupt isdisabled.
1 Transmit interrupt isenabled.
2 CYCSE Cycle start interrupt enable.
0 Interrupt isdisabled.
1 Cycle start interrupt isenabled.
1 CASE Collision avoidance symbol interrupt enable.
0 Interrupt isdisabled.
1 Collision Avoidance symbol interrupt isenabled.
0 WSTE Wakeup status interrupt enable.
0 Interrupt isdisabled.
1 Wakeup status interrupt isenabled.

<!-- Page 1354 -->

FlexRay Module Registers www.ti.com
1354 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.7 Interrupt Line Enable Register (ILE)
Each ofthetwointerrupt lines (CC_int0, CC_int1) canbeenabled separately byprogramming bitEINT0
andEINT1.
Figure 26-123 andTable 26-102 illustrate thisregister.
Figure 26-123. Interrupt Line Enable Register (ILE) [offset_CC =40h]
31 16
Reserved
R-0
15 2 1 0
Reserved EINT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-102. Interrupt Line Enable Register (ILE) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 EINT Enable interrupt line(1-0).
0 Interrupt lineCC_int1 andCC_int0 aredisabled.
1h Interrupt lineCC_int1 isdisabled andCC_int0 isenabled.
2h Interrupt lineCC_int1 isenabled andCC_int0 isdisabled.
3h Interrupt lineCC_int1 andCC_int0 areenabled.

<!-- Page 1355 -->

www.ti.com FlexRay Module Registers
1355 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.8 Timer 0Configuration Register (T0C)
This absolute timer specifies, interms ofcycle count andmacrotick, thepoint intime when thetimer 0
interrupt occurs. The timer 0interrupt generates anonmaskable interrupt signal onCC_tint0.
Timer 0canbeactivated aslong asthePOC iseither inNORMAL_ACTIVE state orin
NORMAL_PASSIVE state. Timer 0isdeactivated when leaving NORMAL_ACTIVE state or
NORMAL_PASSIVE state except fortransitions between thetwostates.
Before reconfiguration ofthetimer, thetimer hastobehalted firstbywriting bitT0RC to0.
Figure 26-124 andTable 26-103 illustrate thisregister.
Figure 26-124. Timer 0Configuration Register (T0C) [offset_CC =44h]
31 30 29 16
Reserved TOMO
R-0 R/W-0
15 14 8 7 2 1 0
Rsvd TOCC Reserved TOMS TORC
R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-103. Timer 0Configuration Register (T0C) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-16 TOMO 0-3FFFh Timer 0macrotick offset. Configures themacrotick offset from thebeginning ofthecycle where the
interrupt istooccur. The Timer 0interrupt occurs atthisoffset foreach cycle inthecycle set.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-8 TOCC 0-FFh Timer 0cycle code. The 7-bit timer 0cycle code determines thecycle setused forgeneration of
thetimer 0interrupt.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TOMS Time 0mode select.
0 Single-shot mode.
1 Continuous mode.
0 TORC Timer 0runcontrol.
0 Timer 0ishalted.
1 Timer 0isrunning.

<!-- Page 1356 -->

FlexRay Module Registers www.ti.com
1356 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.9 Timer 1Configuration Register (T1C)
This relative timer generates aninterrupt onthenonmaskable interrupt signal CC_tint1 after thespecified
number ofmacroticks hasexpired.
Timer 1canbeactivated aslong asthePOC iseither inNORMAL_ACTIVE state orin
NORMAL_PASSIVE state. Timer 1isdeactivated when leaving NORMAL_ACTIVE state or
NORMAL_PASSIVE state except fortransitions between thetwostates.
Before reconfiguration ofthetimer, thetimer hastobehalted firstbywriting bitT1RC to0.
Figure 26-125 andTable 26-104 illustrate thisregister.
Figure 26-125. Timer 1Configuration Register (T1C) [offset_CC =48h]
31 30 29 16
Reserved TIMC
R-0 R/W-2h
15 2 1 0
Reserved T1MS T1RC
R-0 R/W-0
LEGEND: R=Read only; -n=value after reset
Table 26-104. Timer 1Configuration Register (T1C) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-16 TIMC Timer 1macrotick count. When theconfigured macrotick count isreached thetimer 1interrupt is
generated. Incase theconfigured macrotick count isnotwithin thevalid range, timer 1willnot
start.
Valid values:
*2to16383 macroticks incontinuous mode
*1to16383 macroticks insingle-shot mode
2h-3FFFh Continuous mode.
1h-3FFFh Single-shot mode.
15-2 Reserved 0 Reads return 0.Writes have noeffect.
1 T1MS Timer 1mode select.
0 Single-shot mode.
1 Continuous mode.
0 T1RC Timer 1runcontrol.
0 Timer 1ishalted.
1 Timer 1isrunning.

<!-- Page 1357 -->

www.ti.com FlexRay Module Registers
1357 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.2.10 Stop Watch Register 1Register (STPW1)
The stop watch isactivated byaninterrupt event (CC_int0 orCC_int1), bywriting bitSSWT to1,orbyan
external event.
With themacrotick counter increment following next tothestop watch activation theactual cycle counter
andmacrotick value isstored inthestop watch register 1(stop watch event) andtheslotcounter values
forchannel Aandchannel Barestored instop watch register 2.
Figure 26-126 andTable 26-105 illustrate thisregister.
Figure 26-126. Stop Watch Register 1(STPW1) [offset_CC =4Ch]
31 30 29 16
Reserved SMTV
R-0 R-0
15 14 13 8 7 6 5 4 3 2 1 0
Reserved SCCV Rsvd EINT1 EINT0 EETP SSWT EDGE SWMS ESWT
R-0 R-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-105. Stop Watch Register 1(STPW1) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-16 SMTV 0-3E80h Stopped macrotick value. State ofthemacrotick counter when thestop watch event occurred.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 SCCV 0-3Fh Stopped cycle counter value. State ofthecycle counter when thestop watch event occurred.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 EINT1 Enable interrupt 1trigger. Enables stop watch trigger byCC_int1 event ifESWT =1.
0 Stop watch trigger byCC_int1 isdisabled.
1 CC_int1 event triggers stop watch.
5 EINT0 Enable interrupt 0trigger. Enables stop watch trigger byCC_int0 event ifESWT =1.
0 Stop watch trigger byCC_int0 isdisabled.
1 CC_int0 event triggers stop watch.
4 EETP Enable external trigger pin.Enables stop watch trigger event from external pin,ifESWT =1.
0 External trigger isdisabled.
1 Stop watch isactivated byexternal trigger.
3 SSWT Software stop watch trigger. When thehost writes thisbitto1,thestop watch isactivated. After
theactual cycle counter andmacrotick value arestored inthestop watch register, thisbitisreset
to0.The bitisonly writable while ESWT =0.
0 Software trigger isreset.
1 Stop watch isactivated bysoftware trigger.
2 EDGE Stop watch trigger edge select.
0 Falling edge.
1 Rising edge.
1 SWMS Stop watch mode select.
0 Single-shot mode.
1 Continuous mode.
0 ESWT External stop watch trigger. Ifenabled, anexternal event activates thestop watch. Insingle-shot
mode, thisbitisreset to0after thestop watch event occurred.
0 External stop watch trigger isdisabled.
1 External stop watch trigger isenabled.

<!-- Page 1358 -->

FlexRay Module Registers www.ti.com
1358 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: BitsESWT andSSWT cannot besetto1simultaneously. Inthiscase thewrite access tothe
register isignored, andboth bitskeep their previous values. Either theexternal stop watch
trigger orthesoftware stop watch trigger may beused.
The availability ofanexternal stop watch pinisdevice dependant. Refer tothedevice data
sheet fordetails.
26.3.2.2.11 Stop Watch Register 2Register (STPW2)
Figure 26-127 andTable 26-106 illustrate thisregister.
Figure 26-127. Stop Watch Register 2(STPW2) [offset_CC =50h]
31 27 26 16
Reserved SSCVB
R-0 R-0
15 11 10 0
Reserved SSCVA
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-106. Stop Watch Register 2(STPW2) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 SSCVB 0-7FFh Stop watch captured slotcounter value channel B.State oftheslotcounter forchannel Bwhen the
stop watch event occurred.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 SSCVA 0-7FFh Stop watch captured slotcounter value channel A.State oftheslotcounter forchannel Awhen the
stop watch event occurred.

<!-- Page 1359 -->

www.ti.com FlexRay Module Registers
1359 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3 Control Registers
This section describes theregisters provided bythecommunication controller toallow thehost tocontrol
theoperation ofthecommunication controller. The FlexRay protocol specification requires thehost towrite
application configuration data inCONFIG state only.
NOTE: Beaware thattheconfiguration registers arenotlocked forwriting inDEFAULT_CONFIG
state.
The configuration data isreset when DEFAULT_CONFIG state isentered from hardware reset. Tochange
POC state from DEFAULT_CONFIG toCONFIG state thehost hastoapply thecontroller host interface
command CONFIG. Ifthehost wants thecommunication controller toleave CONFIG state, thehost hasto
proceed asdescribed inLock Register (LCK).
NOTE: Allbitsmarked with anasterisk (*)canbeupdated inDEFAULT_CONFIG orCONFIG state
only.
26.3.2.3.1 SUC Configuration Register 1(SUCC1)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-128 andTable 26-107 illustrate thisregister.
Figure 26-128. SUC Configuration Register 1(SUCC1) [offset_CC =80h]
31 28 27 26 25 24 23 22 21 20 16
Reserved CCHB* CCHA* MTSB* MTSA* HCSE* TSM* WUCS* PTA*
R-0 R/W-1 R/W-1 R/W-0 R/W-0 R/W-0 R/W-1 R/W-0 R/W-0
15 11 10 9 8 7 6 4 3 0
CSA* Rsvd TXSY* TXST* PBSY Reserved CMD*
R/W-2h R-0 R/W-0 R/W-0 R-1 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-107. SUC Configuration Register 1(SUCC1) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27 CCHB Connected tochannel B.Configures whether thenode isconnected tochannel B.
0 Node isnotconnected tochannel B.
1 Node isconnected tochannel B(default byhardware reset).
26 CCHA Connected tochannel A.Configures whether thenode isconnected tochannel A.
0 Node isnotconnected tochannel A.
1 Node isconnected tochannel A(default byhardware reset).
25 MTSB Select channel BforMTS Transmission. The bitselects channel BforMTS symbol
transmission ifrequested bywriting CMD =8h.The flagisreset bydefault andmay bemodified
only inDEFAULT_CONFIG orCONFIG state.
0 Channel Bisnotselected forMTS transmission.
1 Channel Bisselected forMTS transmission.
Note: MTSB may also bechanged outside DEFAULT_CONFIG orCONFIG state when the
write toSUC Configuration Register 1(SUCC1) isdirectly preceded bytheunlock
sequence fortheConfiguration Lock Keyasdescribed intheLock Register (LCK). This
may becombined with CHIcommand SEND_MTS. Ifboth bitsMTSA andMTSB aresetto
1anMTS symbol willbetransmitted onboth channels when requested bywriting CMD =
8h.

<!-- Page 1360 -->

FlexRay Module Registers www.ti.com
1360 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-107. SUC Configuration Register 1(SUCC1) Field Descriptions (continued)
Bit Field Value Description
24 MTSA Select channel AforMTS Transmission. The bitselects channel AforMTS symbol
transmission ifrequested bywriting CMD =8h.The flagisreset bydefault andmay bemodified
only inDEFAULT_CONFIG orCONFIG state.
0 Channel Aisnotselected forMTS transmission.
1 Channel Aisselected forMTS transmission.
Note: MTSA may also bechanged outside DEFAULT_CONFIG orCONFIG state when the
write toSUC Configuration Register 1(SUCC1) isdirectly preceded bytheunlock
sequence fortheConfiguration Lock Keyasdescribed intheLock Register (LCK). This
may becombined with CHIcommand SEND_MTS. Ifboth bitsMTSA andMTSB aresetto
1anMTS symbol willbetransmitted onboth channels when requested bywriting CMD =
8h.
23 HCSE Halt duetoclock sync error. Controls reaction ofthecommunication controller toaclock
synchronization error. The bitcanbemodified inDEFAULT_CONFIG orCONFIG state only.
0 Communication controller willenter orremain inNORMAL_PASSIVE.
1 Communication controller willenter HALT state.
22 TSM Transmission slotmode. Selects theinitial transmission slotmode. InSINGLE slotmode the
communication controller may only transmit inthepre-configured keyslot. This slotisdefined
bythekeyslotID,which isconfigured intheheader section ofmessage buffer 0.Inallslot
mode thecommunication controller may transmit inallslots. The bitcanbewritten in
DEFAULT_CONFIG orCONFIG state only. The communication controller changes toallslot
mode when thehost successfully applied theALL_SLOTS command bywriting CMD =5hin
POC states NORMAL_ACTIVE orNORMAL_PASSIVE. The actual slotmode ismonitored by
SLM inregister CCSV.
0 Allslotmode.
1 Single slotmode (default byhardware reset).
21 WUCS Wakeup channel select. With thisbitthehost selects thechannel onwhich thecommunication
controller sends theWakeup pattern. The communication controller ignores anyattempt to
change thestatus ofthisbitwhen notinDEFAULT_CONFIG orCONFIG state.
0 Send wakeup pattern onchannel A.
1 Send wakeup pattern onchannel B.
20-16 PTA 0-1Fh
even/odd
cycle pairsPassive toactive. Defines thenumber ofconsecutive even/odd cycle pairs thatmust have valid
clock correction terms before thecommunication controller isallowed totransit from
NORMAL_PASSIVE toNORMAL_ACTIVE state. Ifsetto0,thecommunication controller isnot
allowed totransit from NORMAL_PASSIVE toNORMAL_ACTIVE state. Itcanbemodified in
DEFAULT_CONFIG orCONFIG state only.
15-11 CSA 2h-1Fh Cold start attempts. Configures themaximum number ofattempts thatacold starting node is
permitted totrytostart upthenetwork without receiving anyvalid response from another node.
Itcanbemodified inDEFAULT_CONFIG orCONFIG state only. Must beidentical inallnodes
ofacluster.
10 Reserved 0 Reads return 0.Writes have noeffect.
9 TXSY Transmit sync frame inkeyslot. Defines whether thekeyslotisused totransmit async frame.
The bitcanbemodified inDEFAULT_CONFIG orCONFIG state only.
Note: Theprotocol requires thatboth bitsTXST andTXSY aresetforcoldstart nodes.
0 Nosync frame transmission inkeyslot, node isneither sync norcoldstart node.
1 Key slotused totransmit sync frame, node issync node.
8 TXST Transmit startup frame inkeyslot. Defines whether thekeyslotisused totransmit astartup
frame. The bitcanbemodified inDEFAULT_CONFIG orCONFIG state only.
Note: Theprotocol requires thatboth bitsTXST andTXSY aresetforcoldstart nodes.
0 Nostartup frame transmitted inkeyslot, node isnon-coldstarter.
1 Key slotused totransmit startup frame, node isleading orfollowing coldstarter.
7 PBSY POC busy. Signals thatthePOC isbusy andcannot accept acommand from thehost. CMD is
locked against write accesses.
0 POC isnotbusy, CMD iswritable.
1 POC isbusy, CMD islocked.
6-4 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1361 -->

www.ti.com FlexRay Module Registers
1361 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-107. SUC Configuration Register 1(SUCC1) Field Descriptions (continued)
Bit Field Value Description
3-0 CMD The controller host interface command vector. The host may write anycontroller host interface
command atanytime, butcertain commands areenabled only incertain POC states. Ifa
command isnotenabled, itwillnotbeexecuted, thecontroller host interface command vector
CMD willbereset to0000 =command_not_accepted, andflagCNA intheerror interrupt
register willbesetto1.Incase theprevious controller host interface (CHI) command hasnot
yetcompleted, EIR.CCL issetto1together with EIR.CNA; theCHI command needs tobe
repeated. Except forHALT state, aPOC state change command applied while the
communication controller isalready intherequested POC state neither causes astate change
norwillEIR.CNA beset.
0 command_not_accepted
1h CONFIG
2h READY
3h WAKEUP
4h RUN
5h ALL_SLOTS
6h HALT
7h FREEZE
8h SEND_MTS
9h ALLOW_COLDSTART
Ah RESET_STATUS_INDICATORS
Bh MONITOR_MODE
Ch CLEAR_RAMS
Dh-Eh Reserved
Fh LOOPBACK MODE
Controller Host Interface Command Vector:
The following gives more information about thecontroller host interface commands.
*Reading CMD shows whether thelastcontroller host interface command was accepted.
*The actual POC state ismonitored byPOCS inthecommunication controller status vector
*Inmost cases theHost must check SUCC1.PBSY before writing anew CHI command.
command_not_accepted
CMD isreset to0000 duetooneofthefollowing conditions:
*Illegal command applied bythehost
*Host applied command toleave CONFIG state without preceding configuration lock key
*Host applied new command while execution oftheprevious host command hasnotcompleted
*Host writes command_not_accepted
When CMD isreset to0000 duetoanunaccepted command, bitCNA intheerror interrupt register isset,
and, ifenabled, aninterrupt isgenerated. Commands which arenotaccepted arenotexecuted.
CONFIG
GotoPOC state CONFIG when called inPOC states DEFAULT_CONFIG, READY, orin
MONITOR_MODE. When called inHALT state thecommunication controller transits toPOC state
DEFAULT_CONFIG. When called inanyother state, CMD willbereset to0000 =
command_not_accepted.
READY
GotoPOC state READY when called inPOC states CONFIG, NORMAL_ACTIVE, NORMAL_PASSIVE,
STARTUP, orWAKEUP. When called inanyother state, CMD willbereset to0000 =
command_not_accepted.

<!-- Page 1362 -->

FlexRay Module Registers www.ti.com
1362 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleWAKEUP
GotoPOC state WAKEUP when called inPOC state READY. When called inanyother state, CMD will
bereset to0000 =command_not_accepted.
RUN
GotoPOC state STARTUP when called inPOC state READY. When called inanyother state, CMD will
bereset to0000 =command_not_accepted.
ALL_SLOTS
Leave single slotmode after successful startup /integration atthenext endofcycle when called inPOC
states NORMAL_ACTIVE orNORMAL_PASSIVE. When called inanyother state, CMD willbereset to
0000 =command_not_accepted.
HALT
SettheHalt request HRQ bitinthecommunication controller status vector register andgotoPOC state
HALT atthenext endofcycle when called inPOC states NORMAL_ACTIVE orNORMAL_PASSIVE.
When called inanyother state, CMD willbereset to0000 =command_not_accepted.
FREEZE
GotoPOC state HALT immediately andsettheFreeze status Indicator FSIbitinthecommunication
controller status vector register. Can becalled from anystate.
SEND_MTS
Send single MTS symbol during thesymbol window ofthefollowing cycle onthechannel configured by
MTSA, MTSB, when called inPOC state NORMAL_ACTIVE. When called inanyother state, CMD willbe
reset to0000 =command_not_accepted.
ALLOW_COLDSTART
The command resets bitCSIintheCCSV register toenable coldstarting ofthenode when called inany
POC state except DEFAULT_CONFIG, CONFIG orHALT. When called inthese states, CMD willbereset
to0000 =command_not_accepted.
RESET_STATUS_INDICATORS
Reset status flags FSI, HRQ, CSNI, andCSAI inthecommunication controller status vector register.
CLEAR_RAMS
Sets bitCRAM inthemessage handler status register when called inDEFAULT_CONFIG orCONFIG
state. When called inanyother state, CMD willbereset to0000 =command_not_accepted. BitCRAM is
also setwhen thecommunication controller leaves hardware reset. Bysetting bitCRAM, allinternal RAM
blocks areinitialized to0andtheECC bitsareinitialized accordingly, depending what mode isenabled.
During theinitialization oftheRAMs, PBSY willshow POC busy. Access totheconfiguration andstatus
registers ispossible during execution ofCHI command CLEAR_RAMS.
The initialization oftheCommunication Controller internal RAM blocks takes 2048 VBUS clock cycles.
There should benohost access toIBForOBF during initialization oftheinternal RAM blocks after
hardware reset orafter assertion ofcontroller host interface command CLEAR_RAMS. Before asserting
controller host interface command CLEAR_RAMS thehost should beaware thatnotransfer between
message RAM andIBF/OBF orthetransient buffer RAMs isongoing. This command also resets the
message buffer status registers (MHDS, TXRQ1/2/3/4, NDAT1/2/3/4, MBSC1/2/3/4).
NOTE: Allaccepted commands with exception ofCLEAR_RAMS andSEND_MTS willcause a
change ofthePOC state intheVBUS clock domain after atmost 8cycles oftheslower of
thetwoclocks VBUS clock and80MHz sample clock coming from thePLL. Itisassumed
thatPOC was notbusy when thecommand was applied andthatnoPOC state change was
forced bybusactivity inthattime frame. Reading register Communication Controller Status
Vector (CCSV) willshow data thatisadditionally delayed bysynchronization from sample
clock toVBUS clock domain andbytheCPU interface. The maximum additional delay is12
cycles oftheslower ofthetwoclocks VBUS clock andsample clock.

<!-- Page 1363 -->

www.ti.com FlexRay Module Registers
1363 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleMONITOR_MODE
Enter MONITOR_MODE when called inPOC state CONFIG. Inthismode thecommunication controller is
able toreceive FlexRay frames andCAS /MTS symbols. Itisalso able todetect coding errors. The
temporal integrity ofreceived frames isnotchecked. This mode canbeused fordebugging purposes, for
example, incase thatthestartup ofaFlexRay network fails. When called inanyother state, CMD willbe
reset to0000 =command_not_accepted.
26.3.2.3.2 SUC Configuration Register 2(SUCC2)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-129 andTable 26-108 illustrate thisregister.
Figure 26-129. SUC Configuration Register 2(SUCC2) [offset_CC =84h]
31 28 27 24 23 21 20 16
Reserved LTN* Reserved LT*
R-0 R/W-1h R-0 R/W-504h
15 0
LT*
R/W-504h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only.
Table 26-108. SUC Configuration Register 2(SUCC2) Field Descriptions
Bit Field Value Description
31-28 Reserved. 0 Reads return 0.Writes have noeffect.
27-24 LTN 2-Fh Listen timeout noise. Configures theupper limit forthestartup andwakeup listen timeout inthe
presence ofnoise. Must beidentical inallnodes ofacluster.
The wakeup /startup noise timeout iscalculated asfollows: LT×(LTN +1).
23-21 Reserved. 0 Reads return 0.Writes have noeffect.
20-0 LT 504h-139706h µTListen timeout. Configures theupper limit ofthestartup andwakeup listen timeout.

<!-- Page 1364 -->

FlexRay Module Registers www.ti.com
1364 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.3 SUC Configuration Register 3(SUCC3)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-130 andTable 26-109 illustrate thisregister.
Figure 26-130. SUC Configuration Register 3(SUCC3) [offset_CC =88h]
31 16
Reserved
R-0
15 8 7 4 3 0
Reserved WCF* WCP*
R-0 R/W-1h R/W-1h
LEGEND: R/W =Read/Write; R=Read only; D=Device-specific reset value; -n=value
after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state only
Table 26-109. SUC Configuration Register 3(SUCC3) Field Descriptions
Bit Field Value Description
31-8 Reserved. 0 Reads return 0.Writes have noeffect.
7-4 WCF 1-Fh Maximum without clock correction fatal. These bitsdefine thenumber ofconsecutive even/odd
cycle pairs with missing clock correction terms thatwillcause atransition from NORMAL_ACTIVE
orNORMAL_PASSIVE state. These must beidentical inallnodes ofacluster.
Note: Thetransition toHALT state isprevented ifSUCC1.HCSE isnotset.
3-0 WCP 1-Fh Maximum without clock correction passive. These bitsdefine thenumber ofconsecutive even/odd
cycle pairs with missing clock correction terms thatwillcause atransition from NORMAL_ACTIVE
toNORMAL_PASSIVE toHALT state. These must beidentical inallnodes ofacluster.
26.3.2.3.4 NEM Configuration Register (NEMC)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-131 andTable 26-110 illustrate thisregister.
Figure 26-131. NEM Configuration Register (NEMC) [offset_CC =8Ch]
31 16
Reserved
R-0
15 4 3 0
Reserved NML*
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-110. NEM Configuration Register (NEMC) Field Descriptions
Bit Field Value Description
31-7 Reserved. 0 Reads return 0.Writes have noeffect.
6-0 NML 0-Ch bytes Network management vector length (inbytes).
These bitsconfigure thelength oftheNMvector. The configured length must beidentical inall
nodes ofacluster.

<!-- Page 1365 -->

www.ti.com FlexRay Module Registers
1365 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.5 PRT Configuration Register 1(PRTC1)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-132 andTable 26-111 illustrate thisregister.
Figure 26-132. PRT Configuration Register 1(PRTC1) [offset_CC =90h]
31 26 25 24 16
RPW* Rsvd RXW*
R/W-2h R-0 R/W-4Ch
15 14 13 12 11 10 4 3 0
BRP* SSP* Rsvd CASM* TSST*
R/W-0 R/W-0 R-0 R/W-23h R/W-3h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-111. PRT Configuration Register 1(PRTC1) Field Descriptions
Bit Field Value Description
31-26 RWP 2h-3Fh Repetition oftransmission wakeup pattern. These bitsconfigure thenumber ofrepetitions
(sequences) ofthetransmission wakeup symbol.
25 Reserved 0 Reads return 0.Writes have noeffect.
24-16 RXW 4Ch-12Dh Wakeup symbol receive window length. Configures thenumber ofbittimes used bythenode to
testtheduration ofthereceived wakeup pattern. Must beidentical inallnodes ofacluster.
15-14 BRP Baud rate prescaler. These bitsconfigure thebaud rate ontheFlexRay bus. The baud rates
listed below arevalid with asample clock of80MHz. One bittime always consists of8samples
independent oftheconfigured baud rate.
0 10Mbit/s (Sample Clock Period =12.5ns; 1µT=25ns; Samples perµT=2)
1h 5Mbit/s (Sample Clock Period =25ns; 1µT=25ns; Samples perµT=1)
2h-3h 2.5Mbit/s (Sample Clock Period =50ns; 1µT=50ns; Samples perµT=1)
13-12 SPP Strobe Point Position. Defines thesample count value forstrobing. The strobed bitvalue isset
tothevoted value when thesample count isincremented tothevalue configured bySPP.
Note: Thecurrent revision 2.1oftheFlexRay protocol requires thatSPP =00.The
alternate strobe point positions could beused tocompensate forasymmetries inthe
physical layer.
0,3h Sample 5(default)
1h Sample 4
2h Sample 6
11 Reserved 0 Reads return 0.Writes have noeffect.
10-4 CASM 43h-63h bittimes Collision avoidance symbol max (inbittimes). These bitsconfigure theupper limit ofthe
acceptance window foracollision avoidance symbol (CAS). CASM6 isalways 1.
3-0 TSST 3h-Fh bittimes Transmission start sequence transmitter (inbittimes). These bitsconfigure theduration ofthe
transmission start sequence (TSS) interms ofbittimes (1bittime =4µT=100ns @10Mbps).
Must beidentical inallnodes ofacluster.

<!-- Page 1366 -->

FlexRay Module Registers www.ti.com
1366 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.6 PRT Configuration Register 2(PRTC2)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-133 andTable 26-112 illustrate thisregister.
Figure 26-133. PRT Configuration Register 2(PRTC2) [offset_CC =94h]
31 30 29 24 23 16
Reserved TXL* TXI*
R-0 R/W-Fh R/W-2Dh
15 14 13 8 7 6 5 0
Reserved RXL* Reserved RXI*
R-0 R/W-Ah R-0 R/W-Eh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-112. PRT Configuration Register 2(PRTC2) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 TXL Fh-3Ch bittimes Wakeup symbol transmit low(inbittimes). These bitsconfigure theactive lowphase ofthe
wakeup symbol. The duration must beidentical inallnodes ofacluster.
23-16 TXI 2Dh-B4h bittimes Wakeup symbol transmit idle(inbittimes). These bitsconfigure thenumber ofbittimes used by
thenode totransmit theidlephase ofthewakeup symbol. Durations must beidentical inall
nodes ofacluster.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 RXL Ah-37h bittimes Wakeup symbol receive low(inbittimes). These bitsconfigure thenumber ofbittimes used by
thenode totesttheduration ofthelowphase ofthereceived wakeup symbol. Must beidentical
inallnodes ofacluster.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 RXI Eh-37h bittimes Wakeup symbol receive idle(inbittimes). These bitsconfigure thenumber ofbittimes used by
thenode totesttheduration oftheidlephase ofthereceived wakeup symbol. Must beidentical
inallnodes ofacluster.

<!-- Page 1367 -->

www.ti.com FlexRay Module Registers
1367 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.7 MHD Configuration Register (MHDC)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-134 andTable 26-113 illustrate thisregister.
Figure 26-134. MHD Configuration Register (MHDC) [offset_CC =98h]
31 29 28 16
Reserved SLT*
R-0 R/W-2h
15 7 6 0
Reserved SFDL*
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-113. MHD Configuration Register (MHDC) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-16 SLT 0-1F2Dh
minislotsStart oflatest transmit (inminislots). These bitsconfigure themaximum minislot value allowed
before inhibiting new frame transmissions intheDynamic Segment ofthecycle. There isno
transmission indynamic segment ifSLT iscleared to0.
15-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 SFDL 0-7Fh Static frame data length. These bitsconfigure thecluster-wide payload length forallframes sent
inthestatic segment indouble bytes. The payload length must beidentical inallnodes ofa
cluster.

<!-- Page 1368 -->

FlexRay Module Registers www.ti.com
1368 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.8 GTU Configuration Register 1(GTUC1)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-135 andTable 26-114 illustrate thisregister.
Figure 26-135. GTU Configuration Register 1(GTUC1) [offset_CC =A0h]
31 20 19 16
Reserved UT*
R-0 R/W-0
15 0
UT*
R/W-0280h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-114. GTU Configuration Register 1(GTUC1) Field Descriptions
Bit Field Value Description
31-20 Reserved. 0 Reads return 0.Writes have noeffect.
19-0 UT 280h-9C400h µTMicrotick percycle (inmicroticks).
These bitsconfigure theduration ofthecommunication cycle inmicroticks.
26.3.2.3.9 GTU Configuration Register 2(GTUC2)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-136 andTable 26-115 illustrate thisregister.
Figure 26-136. GTU Configuration Register 2(GTUC2) [offset_CC =A4h]
31 20 19 16
Reserved SNM*
R-0 R/W-2h
15 14 13 0
Reserved MPC*
R-0 R/W-Ah
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-115. GTU Configuration Register 2(GTUC2) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 SNM 2h-Fh frames Sync node max (inframes). These bitsconfigure themaximum number offrames within a
cluster with sync frame indicator bitSYN set.The number offrames must beidentical inall
nodes ofacluster.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-0 MPC Ah-3E80h MT Macrotick percycle (inmacroticks). These bitsconfigure theduration ofonecommunication
cycle inmacroticks. The cycle length must beidentical inallnodes ofacluster.

<!-- Page 1369 -->

www.ti.com FlexRay Module Registers
1369 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.10 GTU Configuration Register 3(GTUC3)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-137 andTable 26-116 illustrate thisregister.
Figure 26-137. GTU Configuration Register 3(GTUC3) [offset_CC =A8h]
31 30 24 23 22 16
Rsvd MIOB* Rsvd MIOA*
R-0 R/W-2h R-0 R/W-2h
15 8 7 0
UIOB* UIOA*
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only.
Table 26-116. GTU Configuration Register 3(GTUC3) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-24 MIOB 2h-48h MT Macrotick initial offset channel B(inmacroticks). These bitsconfigure thenumber ofmacroticks
between thestatic slotboundary andthesubsequent macrotick boundary ofthesecondary time
reference point based onthenominal macrotick duration. Must beidentical inallnodes ofa
cluster.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 MIOA 2h-48h MT Macrotick initial offset channel A(inmacroticks). These bitsconfigure thenumber ofmacroticks
between thestatic slotboundary andthesubsequent macrotick boundary ofthesecondary time
reference point based onthenominal macrotick duration. Must beidentical inallnodes ofa
cluster.
15-8 UIOB 0-F0h µT Microtick initial offset channel B(inmicroticks). These bitsconfigure thenumber ofmicroticks
between theactual time reference point onchannel Bandthesubsequent macrotick boundary of
thesecondary time reference point. The parameter hastobesetforeach channel independently.
7-0 UIOA 0-F0h µT Microtick initial offset channel A(inmicroticks). These bitsconfigure thenumber ofmicroticks
between theactual time reference point onchannel Aandthesubsequent macrotick boundary of
thesecondary time reference point. The parameter hastobesetforeach channel independently.

<!-- Page 1370 -->

FlexRay Module Registers www.ti.com
1370 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.11 GTU Configuration Register 4(GTUC4)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only. Figure 26-138 andTable 26-117 illustrate thisregister.
Figure 26-138. GTU Configuration Register 4(GTUC4) [offset_CC =ACh]
31 30 29 16
Reserved OCS*
R-0 R/W-Ah
15 14 13 0
Reserved NIT*
R-0 R/W-9h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-117. GTU Configuration Register 4(GTUC4) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-16 OCS 8h-3E7Eh MT Offset correction start (inmacroticks). These bitsdetermine thestart oftheoffset correction
within theNITphase, calculated from start ofcycle. Must beidentical inallnodes ofacluster.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-0 NIT 7h-3E7Dh MT Network idletime start (inmacroticks). These bitsconfigure thestarting point ofthenetwork idle
time (NIT) attheendofthecommunication cycle expressed interms ofmacroticks from the
beginning ofthecycle. The number must beidentical inallnodes ofacluster.
26.3.2.3.12 GTU Configuration Register 5(GTUC5)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only. Figure 26-139 andTable 26-118 illustrate thisregister.
Figure 26-139. GTU Configuration Register 5(GTUC5) [offset_CC =B0h]
31 24 23 21 20 16
DEC* Reserved CDD*
R/W-Eh R-0 R/W-0
15 8 7 0
DCB* DCA*
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-118. GTU Configuration Register 5(GTUC5) Field Descriptions
Bit Field Value Description
31-24 DEC Eh-8Fh µT Decoding correction (inmicroticks). These bitsconfigure thedecoding correction value used to
determine theprimary time reference point.
23-21 Reserved. 0 Reads return 0.Writes have noeffect.
20-16 CDD 0-14h µT Cluster driftdamping (inmicroticks). These bitsconfigure thecluster driftdamping value used in
clock synchronization tominimize accumulation ofrounding errors.
15-8 DCB 0-C8h µT Delay compensation channel B(inmicroticks). These bitsareused tocompensate forreception
delays ontheindicated channel. This compensates propagation delays formicroticks inthe
range of0.0125 to0.05s. Inpractice, theminimum propagation delay ofallsync nodes should
beapplied.
7-0 DCA 0-C8h µT Delay compensation channel A(inmicroticks). These bitsareused tocompensate forreception
delays ontheindicated channel. This compensates propagation delays formicroticks inthe
range of0.0125 to0.05s. Inpractice, theminimum propagation delay ofallsync nodes should
beapplied.

<!-- Page 1371 -->

www.ti.com FlexRay Module Registers
1371 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.13 GTU Configuration Register 6(GTUC6)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-140 andTable 26-119 illustrate thisregister.
Figure 26-140. GTU Configuration Register 6(GTUC6) [offset_CC =B4h]
31 27 26 16
Reserved MOD*
R-0 R/W-2h
15 11 10 0
Reserved ASR*
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-119. GTU Configuration Register 6(GTUC6) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 MOD 2h-783h µTMaximum oscillator drift(inmicroticks). Maximum driftoffset between twonodes thatoperate with
unsynchronized clocks over onecommunication cycle inµT.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 ASR 0-753h µT Accepted startup range (inmicroticks). Number ofmicroticks constituting theexpanded range of
measured deviation forstartup frames during integration.
26.3.2.3.14 GTU Configuration Register 7(GTUC7)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-141 andTable 26-120 illustrate thisregister.
Figure 26-141. GTU Configuration Register 7(GTUC7) [offset_CC =B8h]
31 26 25 16
Reserved NSS*
R-0 R/W-2h
15 10 9 0
Reserved SSL*
R-0 R/W-4h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-120. GTU Configuration Register 7(GTUC7) Field Descriptions
Bit Field Value Description
31-26 Reserved. 0 Reads return 0.Writes have noeffect.
25-16 NSS 2h-3FFh Number ofstatic slots. These bitsconfigure thenumber ofstatic slots inacycle. Atleast two
coldstart nodes must beconfigured tostartup aFlexRay network. The number ofstatic slots
must beidentical inallnodes ofacluster.
15-10 Reserved. 0 Reads return 0.Writes have noeffect.
9-0 SSL 4h-293h Static slotlength (inmacroticks). These bitsconfigure theduration ofastatic slot. The static slot
length must beidentical inallnodes ofacluster.

<!-- Page 1372 -->

FlexRay Module Registers www.ti.com
1372 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.15 GTU Configuration Register 8(GTUC8)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-142 andTable 26-121 illustrate thisregister.
Figure 26-142. GTU Configuration Register 8(GTUC8) [offset_CC =BCh]
31 29 28 16
Reserved NMS*
R-0 R/W-0
15 6 5 0
Reserved MSL*
R-0 R/W-2h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-121. GTU Configuration Register 8(GTUC8) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-16 NMS 0-1F32h Number ofminislots. These bitsconfigure thenumber ofminislots inthedynamic segment ofa
cycle. The number ofminislots must beidentical inallnodes ofacluster.
15-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 MSL 2h-3Fh MT Minislot length (inmacroticks). These bitsconfigure theduration ofaminislot. The minislot length
must beidentical inallnodes ofacluster.
26.3.2.3.16 GTU Configuration Register 9(GTUC9)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-143 andTable 26-122 illustrate thisregister.
Figure 26-143. GTU Configuration Register 9(GTUC9) [offset_CC =C0h]
31 18 17 16
Reserved DSI*
R-0 R/W-0
15 13 12 8 7 6 5 0
Reserved MAPO* Reserved APO*
R-0 R/W-1h R-0 R/W-1h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-122. GTU Configuration Register 9(GTUC9) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17-16 DSI 0-2h Dynamic slotidlephase (inminislots). The duration ofthedynamic slotidlephase hastobe
greater orequal than theidledetection time. Must beidentical inallnodes ofacluster.
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12-8 MAPO 1h-1Fh MT Minislot action point offset (inmacroticks). These bitsconfigure theminislot action point offset
within theminislots ofthedynamic segment. The minislot action point offset must beidentical inall
nodes ofacluster.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 APO 1h-3Fh MT Action point offset (inmacroticks). These bitsconfigure theaction point offset within static slots
andsymbol window. The action point offset must beidentical inallnodes ofacluster.

<!-- Page 1373 -->

www.ti.com FlexRay Module Registers
1373 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.17 GTU Configuration Register 10(GTUC10)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-144 andTable 26-123 illustrate thisregister.
Figure 26-144. GTU Configuration Register 10(GTUC10) [offset_CC =C4h]
31 27 26 16
Reserved MRC*
R-0 R/W-2h
15 14 13 0
Reserved MOC*
R-0 R/W-5h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-123. GTU Configuration Register 10(GTUC10) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 MRC 2h-783h µTMaximum rate correction (inmicroticks). Holds themaximum permitted rate correction value tobe
applied bytheinternal clock synchronization algorithm. The communication controller checks the
internal rate correction value against themaximum rate correction value (absolute value).
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-0 MOC 5h-3BA2h µTMaximum offset correction (inmicroticks). Holds themaximum permitted offset correction value to
beapplied bytheinternal clock synchronization algorithm (absolute value). The communication
controller checks theinternal offset correction value against themaximum offset correction value.

<!-- Page 1374 -->

FlexRay Module Registers www.ti.com
1374 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.3.18 GTU Configuration Register 11(GTUC11)
Figure 26-145 andTable 26-124 illustrate thisregister.
Figure 26-145. GTU Configuration Register 11(GTUC11) [offset_CC =C8h]
31 27 26 24 23 19 18 16
Reserved ERC* Reserved EOC*
R-0 R/W-0 R-0 R/W-0
15 10 9 8 7 2 1 0
Reserved ERCC* Reserved EOCC*
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-124. GTU Configuration Register 11(GTUC11) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 ERC 0-7h µT External rate correction (inmicroticks). Holds theexternal clock rate correction value tobeapplied
bytheinternal clock synchronization algorithm. The value issubtracted/added from/to the
calculated rate correction value. The value isapplied during NIT. May bemodified in
DEFAULT_CONFIG orCONFIG state only.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 EOC 0-7h µT External offset correction (inmicroticks). Holds theexternal clock offset correction value tobe
applied bytheinternal clock synchronization algorithm. The value issubtracted/added from/to the
calculated offset correction value. The value isapplied during NIT. May bemodified in
DEFAULT_CONFIG orCONFIG state only.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 ERCC External rate correction control. Bywriting toERCC, theexternal rate correction isenabled as
specified below. Should bemodified only outside NIT.
0,1h Noexternal rate correction.
2h External rate correction value issubtracted from calculated rate correction value.
3h External rate correction value isadded tocalculated rate correction value.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 EOCC External offset correction control. Bywriting toEOCC, theexternal offset correction isenabled as
specified below. Should bemodified only outside NIT.
0-1h Noexternal offset correction.
2h External offset correction value issubtracted from calculated offset correction value.
3h External offset correction value isadded tocalculated offset correction value.

<!-- Page 1375 -->

www.ti.com FlexRay Module Registers
1375 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4 Status Registers
During 8/16-bit accesses tostatus variables coded with more than 8/16-bit, thevariable might beupdated
bythecommunication controller between twoaccesses (non-atomic read accesses). Allinternal counters
andthecommunication controller status flags arereset when thecommunication controller transits from
CONFIG toREADY state.
26.3.2.4.1 Communication Controller Status Vector (CCSV)
Figure 26-146 andTable 26-125 illustrate thisregister.
Figure 26-146. Communication Controller Status Vector Register (CCSV) [offset_CC =100h]
31 30 29 24 23 19 18 16
Reserved PSL RCA WSV
R-0 R-0 R-2h R-0
15 14 13 12 11 10 9 8 7 6 5 0
Rsvd CSI CSAI CSNI Reserved SLM HQR FSI POCS
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-125. Communication Controller Status Vector Register (CCSV) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 PSL POC Status Log. Status ofPOCS immediately before entering HALT state. Setwhen entering
HALT state. SettoHALT when FREEZE command isapplied during HALT state andFSIisnot
already set,thatis,theHALT state was notreached byFREEZE command. Reset to0when
leaving HALT state.
23-19 RCA 0-1Fh Remaining coldstart attempts. Indicates thenumber ofremaining coldstart attempts. The maximum
number ofcoldstart attempts isconfigured byCSA(4-0) intheSUC configuration register 1.
18-16 WSV Wakeup status. Indicates thestatus ofthecurrent wakeup attempt. Reset byCHI command
RESET_STATUS_INDICATORS orbytransition from HALT toEFAULT_CONFIG state.
0 UNDEFINED. Nowakeup attempt since CONFIG state was left.
1h RECEIVED_HEADER. Setwhen thecommunication controller finishes wakeup duetothe
reception ofaframe header without coding violation oneither channel inWAKEUP_LISTEN or
WAKEUP_DETECT state.
2h RECEIVED_WUP. Setwhen thecommunication controller finishes wakeup duetothereception of
avalid wakeup pattern ontheconfigured wakeup channel inWAKEUP_LISTEN or
WAKEUP_DETECT state.
3h COLLISION_HEADER. Setwhen thecommunication controller stops wakeup duetoadetected
collision during wakeup pattern transmission byreceiving avalid header oneither channel.
4h COLLISION_WUP. Setwhen thecommunication controller stops wakeup duetoadetected
collision during wakeup pattern transmission byreceiving avalid wakeup pattern ontheconfigured
wakeup channel.
5h COLLISION_UNKNOWN. Setwhen thecommunication controller stops wakeup byleaving
WAKEUP_DETECT state after expiration ofthewakeup timer without receiving avalid wakeup
pattern oravalid frame header.
6h TRANSMITTED. Setwhen thecommunication controller hassuccessfully completed the
transmission ofthewakeup pattern.
7h Reserved
15 Reserved 0 Reads return 0.Writes have noeffect.
14 CSI Cold start inhibit. Indicates thatthenode isdisabled from cold starting. The flagissetwhenever
thePOC enters READY state. The flaghastobereset under control ofthehost bythecontroller
host interface command ALLOW_COLDSTART (CMD =1001).
0 Cold starting ofnode isenabled.
1 Cold starting ofnode isdisabled.
13 CSAI 0-1 Coldstart abort indicator. Coldstart aborted. Reset byCHI command
RESET_STATUS_INDICATORS orbytransition from HALT toDEFAULT_CONFIG state orfrom
READY toSTARTUP state.

<!-- Page 1376 -->

FlexRay Module Registers www.ti.com
1376 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-125. Communication Controller Status Vector Register (CCSV) Field Descriptions (continued)
Bit Field Value Description
12 CSNI 0-1 Coldstart noise indicator. Indicates thatthecold start procedure occurred under noisy conditions.
Reset byCHI command RESET_STATUS_INDICATORS orbytransition from HALT to
DEFAULT_CONFIG state orfrom READY toSTARTUP state.
11-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 SLM Slot mode. Indicates theactual slotmode ofthePOC instates READY, WAKEUP, STARTUP,
NORMAL_ACTIVE, andNORMAL_PASSIVE. Default isSINGLE. Changes toALL, depending on
SUCC1.TSM. InNORMAL_ACTIVE orNORMAL_PASSIVE state theCHI command ALL_SLOTS
willchange theslotmode from SINGLE over ALL_PENDING toALL. SettoSINGLE inallother
states.
0 SINGLE
1h Reserved
2h ALL_PENDING
3h ALL
7 HRQ 0-1 Halt request. Indicates thatarequest from theHost hasbeen received tohaltthePOC attheend
ofthecommunication cycle. Reset byCHI command RESET_STATUS_INDICATORS orby
transition from HALT toDEFAULT_CONFIG state orwhen entering READY state.
6 FSI 0-1 Freeze status indicator. Indicates thatthePOC hasentered theHALT state duetoCHI command
FREEZE orduetoanerror condition requiring animmediate POC halt. Reset byCHI command
RESET_STATUS_INDICATORS orbytransition from HALT toDEFAULT_CONFIG state.
5-0 POCS Protocol operation control status.
Indicates theactual state ofoperation oftheCommunication Controller Protocol Operation
Control:
0 DEFAULT_CONFIG state
1h READY state
2h NORMAL_ACTIVE state
3h NORMAL_PASSIVE state
4h HALT state
5h MONITOR_MODE state
6h-Ch Reserved
Dh LOOPBACK MODE state
Eh Reserved
Fh CONFIG state
Indicates theactual state ofoperation ofthePOC inthewakeup path:
10h WAKEUP_STANDBY state
11h WAKEUP_LISTEN state
12h WAKEUP_SEND state
13h WAKEUP_DETECT state
14h-1Fh Reserved
Indicates theactual state ofoperation ofthePOC inthestartup path:
20h STARTUP_PREPARE state
21h COLDSTART_LISTEN state
22h COLDSTART_COLLISION_RESOLUTION state
23h COLDSTART_CONSISTENCY_CHECK state
24h COLDSTART_GAP state
25h COLDSTART_JOIN state
26h INTEGRATION_COLDSTART_CHECK state
27h INTEGRATION_LISTEN state
28h INTEGRATION_CONSISTENCY_CHECK state
29h INITIALIZE_SCHEDULE state
2Ah ABORT_STARTUP state
2Bh-3Fh Reserved

<!-- Page 1377 -->

www.ti.com FlexRay Module Registers
1377 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: CHI command RESET_STATUS_INDICATORS (SUCC1.CMD =Ah)resets flags FSI, HRQ,
CSNI, CSAI, theslotmode SLM, andthewakeup status WSV.
26.3.2.4.2 Communication Controller Error Vector (CCEV)
Reset byCHI command RESET_STATUS_INDICATORS orbytransition from HALT to
DEFAULT_CONFIG state orwhen entering READY state.
Figure 26-147 andTable 26-126 illustrate thisregister.
Figure 26-147. Communication Controller Error Vector Register (CCEV) [offset_CC =104h]
31 16
Reserved
R-0
15 13 12 8 7 6 5 4 3 0
Reserved PTAC ERRM Reserved CCFC
R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-126. Communication Controller Error Vector Register (CCEV) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12-8 PTAC 0-1Fh Passive toactive count. Indicates thenumber ofconsecutive even /oddcycle pairs thathave
passed with valid rate andoffset correction terms, while thenode iswaiting totransit from
NORMAL_PASSIVE state toNORMAL_ACTIVE state. The transition takes place when PTAC
equals PTA -1asdefined intheSUC configuration register 1.
7-6 ERRM Error mode. Indicates theactual error mode ofthePOC.
0 ACTIVE
1h PASSIVE
2h COMM_HALT
3h Reserved
5-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 CCFC 0-Fh Clock correction failed counter. The clock correction failed counter isincremented by1attheend
ofanyoddcommunication cycle where either themissing offset correction error ormissing rate
correction error areactive. The clock correction failed counter isreset to0attheendofanodd
communication cycle ifneither theoffset correction failed northerate correction failed errors are
active. The clock correction failed counter stops at15.

<!-- Page 1378 -->

FlexRay Module Registers www.ti.com
1378 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.3 Slot Counter Value (SCV)
This register isreset when theCommunication Controller leaves CONFIG state orenters STARTUP state.
Figure 26-148 andTable 26-127 illustrate thisregister.
Figure 26-148. Slot Counter Vector Register (SCV) [offset_CC =110h]
31 27 26 16
Reserved SCCB
R-0 R-0
15 11 10 0
Reserved SCCA
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-127. Slot Counter Vector Register (SCV) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 SCCB 1h-7FFh Slot counter channel B.Current slotcounter value channel B.The value isincremented bythe
communication controller andreset atthestart ofacommunication cycle.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 SCCA 1h-7FFh Slot counter channel A.Current slotcounter value channel A.The value isincremented bythe
communication controller andreset atthestart ofacommunication cycle.
26.3.2.4.4 Macrotick andCycle Counter Value (MTCCV)
Figure 26-149 andTable 26-128 illustrate thisregister.
Figure 26-149. Macrotick andCycle Counter Register (MTCCV) [offset_CC =114h]
31 22 21 16
Reserved CCV
R-0 R-0
15 14 13 0
Reserved MTV
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-128. Macrotick andCycle Counter Register (MTCCV) Field Descriptions
Bit Field Value Description
31-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CCV 0-3Fh Cycle counter value. Current cycle counter value. The value isincremented bythecommunication
controller atthestart ofacommunication cycle.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-0 MTV 0-3E80h Macrotick value. Current macrotick value. The value isincremented bythecommunication
controller andreset atthestart ofacommunication cycle.

<!-- Page 1379 -->

www.ti.com FlexRay Module Registers
1379 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.5 Rate Correction Value (RCV)
This register isreset when theCommunication Controller leaves CONFIG state orenters STARTUP state.
Figure 26-150 andTable 26-129 illustrate thisregister.
Figure 26-150. Rate Correction Value Register (RCV) [offset_CC =118h]
31 16
Reserved
R-0
15 12 11 0
Reserved RCV
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-129. Rate Correction Value Register (RCV) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 RCV Rate correction value (inmicroticks). Rate correction value (two'scomplement). Calculated internal rate
correction value before limitation. IftheRCV value exceeds thelimits defined byGTUC10.MRC, flag
SFS.RCLR issetto1.
NOTE: The external rate correction value isadded tothelimited rate correction value.
26.3.2.4.6 Offset Correction Value (OCV)
Figure 26-151 andTable 26-130 illustrate thisregister.
Figure 26-151. Offset Correction Value Register (OCV) [offset_CC =11Ch]
31 20 19 16
Reserved OCV
R-0 R-0
15 0
OCV
R-0
LEGEND: R=Read only; -n=value after reset
Table 26-130. Offset Correction Value Register (OCV) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-0 OCV Offset correction value (inmicroticks). Offset correction value (two'scomplement). Calculated internal
offset correction value before limitation. IftheOCV value exceeds thelimits defined byGTUC10.MOC,
flagSFS.OCLR issetto1.
NOTE: The external offset correction value isadded tothelimited offset correction value.

<!-- Page 1380 -->

FlexRay Module Registers www.ti.com
1380 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.7 Sync Frame Status (SFS)
This register isreset when theCommunication Controller leaves CONFIG state orenters STARTUP state.
Figure 26-152 andTable 26-131 illustrate thisregister.
Figure 26-152. Sync Frame Status Register (SFS) [offset_CC =120h]
31 20 19 18 17 16
Reserved RCLR MRCS OCLR MOCS
R-0 R-0 R-0 R-0 R-0
15 12 11 8 7 4 3 0
VSBO VSBE VSAO VSAE
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-131. Sync Frame Status Register (SFS) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19 RCLR Rate correction limit reached. The Rate Correction Limit Reached flagsignals totheHost, thattherate
correction value hasexceeded itslimit asdefined byGTUC10.MRC. The flagisupdated bythe
communication controller atstart ofoffset correction phase.
0 Rate correction isbelow limit.
1 Rate correction limit isreached.
18 MRCS Missing rate correction signal. The missing rate correction signal signals tothehost thatnorate
correction canbeperformed because nopairs ofeven/odd sync frames were received. The flagis
updated bythecommunication controller atstart ofoffset correction phase.
0 Rate correction signal isvalid.
1 Missing rate correction signal.
17 OCLR Offset correction limit reached. The offset correction limit reached flagsignals tothehost thattheoffset
correction value hasreached itslimit asdefined byGTUC10.MOC. The flagisupdated bythe
communication controller atstart ofoffset correction phase.
0 Offset correction isbelow limit.
1 Offset correction limit isreached.
16 MOCS Missing offset correction signal. The missing offset correction signal signals tothehost thatnorate
correction canbeperformed because nopairs ofeven /oddsync frames were received. The flagis
updated bythecommunication controller atstart ofoffset correction phase.
0 Offset correction signal isvalid.
1 Missing offset correction signal.
15-12 VSBO 0-Fh Valid sync frames channel B,oddcommunication cycle. Holds thenumber ofvalid sync frames
received onchannel Bintheoddcommunication cycle. Iftransmission ofsync frames isenabled by
SUCC1.TXSY, thevalue isincremented by1.The value isupdated during theNITofeach odd
communication cycle.
11-8 VSBE 0-Fh Valid synch frames channel B,even communication cycle. Holds thenumber ofvalid sync frames
received andtransmitted onchannel Bintheeven communication cycle. Iftransmission ofsync frames
isenabled bySUCC1.TXSY, thevalue isincremented by1.The value isupdated during theNITof
each even communication cycle.
7-4 VSAO 0-Fh Valid synch frames channel A,oddcommunication cycle. Holds thenumber ofvalid sync frames
received andtransmitted onchannel Aintheoddcommunication cycle. Iftransmission ofsync frames
isenabled bySUCC1.TXSY, thevalue isincremented by1.The value isupdated during theNITof
each oddcommunication cycle.
3-0 VSAE 0-Fh Valid synch frames channel A,even communication cycle. Holds thenumber ofvalid sync frames
received andtransmitted onchannel Aintheeven communication cycle. Iftransmission ofsync frames
isenabled bySUCC1.TXSY, thevalue isincremented by1.The value isupdated during theNITof
each even communication cycle.
NOTE: The bitfields VSBO ,VSBE, VSAO, VSAE areonly valid iftherespective channel is
assigned tothecommunication controller bySUCC1.CCHA orSUCC1.CCHB.

<!-- Page 1381 -->

www.ti.com FlexRay Module Registers
1381 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.8 Symbol Window andNITStatus (SWNIT)
Symbol window related status information. Updated bythecommunication controller attheendofthe
symbol window foreach channel. During startup thestatus data isnotupdated. This register isreset when
theCommunication Controller leaves CONFIG state orenters STARTUP state.
Figure 26-153 andTable 26-132 illustrate thisregister.
Figure 26-153. Symbol Window andNITStatus Register (SWNIT) [offset_CC =124h]
31 16
Reserved
R-0
15 12 11 10 9 8 7 6 5 4 3 2 1 0
Reserved SBNB SENB SBNA SENA MTSB MTSA TCSB SBSB SESB TCSA SBSA SESA
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-132. Symbol Window andNITStatus Register (SWNIT) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SBNB Slot boundary violation during NITchannel B.
0 Noslotboundary violation isdetected.
1 Slot boundary violation during NITisdetected onchannel B.
10 SENB Syntax error during NITchannel B.
0 Nosyntax error isdetected.
1 Syntax error during NITisdetected onchannel B.
9 SBNA Slot boundary violation during NITchannel A.
0 Noslotboundary violation isdetected.
1 Slot boundary violation during NITisdetected onchannel A.
8 SENA Syntax error during NITchannel A.
0 Nosyntax error isdetected.
1 Syntax error during NITisdetected onchannel A.
7 MTSB MTS Received onChannel B.Media Access Test symbol received onchannel Bduring thelastsymbol
window. Updated bythecommunication controller foreach channel attheendofthesymbol window.
When thisbitissetto1,also interrupt flagSIR.MTSB issetto1.
0 NoMTS symbol isreceived onchannel B.
1 MTS symbol isreceived onchannel B.
6 MTSA MTS Received onChannel A.Media Access Test symbol received onchannel Aduring thelastsymbol
window. Updated bythecommunication controller foreach channel attheendofthesymbol window.
When thisbitissetto1,also interrupt flagSIR.MTSB issetto1.
0 NoMTS symbol isreceived onchannel A.
1 MTS symbol isreceived onchannel A.
5 TCSB Transmission conflict insymbol window channel B.
0 Notransmission conflict isdetected.
1 Transmission conflict insymbol window isdetected onchannel B.
4 SBSB Slot boundary violation insymbol window channel B.
0 Noslotboundary violation isdetected.
1 Slot boundary violation during symbol window isdetected onchannel B.
3 SESB Syntax error insymbol window channel B.
0 Nosyntax error isdetected.
1 Syntax error during symbol window isdetected onchannel B.

<!-- Page 1382 -->

FlexRay Module Registers www.ti.com
1382 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-132. Symbol Window andNITStatus Register (SWNIT) Field Descriptions (continued)
Bit Field Value Description
2 TCSA Transmission conflict insymbol window channel A.
0 Notransmission conflict isdetected.
1 Transmission conflict insymbol window isdetected onchannel A.
1 SBSA Slot boundary violation insymbol window channel A.
0 Noslotboundary violation isdetected.
1 Slot boundary violation during symbol window isdetected onchannel A.
0 SESA Syntax error insymbol window channel A.
0 Nosyntax error isdetected.
1 Syntax error during symbol window isdetected onchannel A.
26.3.2.4.9 Aggregated Channel Status (ACS)
The aggregated channel status provides thehost with anaccrued status ofchannel activity forall
communication slots regardless ofwhether they areassigned fortransmission orsubscribed forreception.
The aggregated channel status also includes status data from thesymbol phase andthenetwork idle
time. The status data isupdated (set) after each slotandaggregated until itisreset bythehost. During
startup thestatus data isnotupdated. Aflagiscleared bywriting a1tothecorresponding bitposition.
Writing a0hasnoeffect ontheflag. This register isreset when theCommunication Controller leaves
CONFIG state orenters STARTUP state.
Figure 26-154 andTable 26-133 illustrate thisregister.
Figure 26-154. Aggregated Channel Status Register (ACS) [offset_CC =128h]
31 16
Reserved
R-0
15 13 12 11 10 9 8 7 5 4 3 2 1 0
Reserved SBVB CIB CEDB SEDB VFRB Reserved SBVA CIA CEDA SEDA VFRA
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-133. Aggregated Channel Status Register (ACS) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 SBVB Slot boundary violation onchannel B.One ormore slotboundary violations were observed onchannel
Batanytime during theobservation period (static ordynamic slots including symbol window andNIT).
0 Noslotboundary violation isobserved.
1 Slot boundary violation isobserved onchannel B.
11 CIB Communication indicator channel B.One ormore valid frames were received onchannel Binslots that
also contained anyadditional communication during theobservation period, thatis,oneormore slots
received avalid frame AND hadanycombination ofeither syntax error ORcontent error ORslot
boundary violation.
0 Novalid frame isreceived inslots containing anyadditional communication.
1 Valid frame isreceived onchannel Binslots containing anyadditional communication.
10 CEDB Content error detected onchannel B.One ormore frames with acontent error were received on
channel Binanystatic ordynamic slotduring theobservation period.
0 Noframe with content error isreceived.
1 Frame with content error isreceived onchannel B.

<!-- Page 1383 -->

www.ti.com FlexRay Module Registers
1383 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-133. Aggregated Channel Status Register (ACS) Field Descriptions (continued)
Bit Field Value Description
9 SEDB Syntax error detected onchannel B.One ormore syntax errors instatic ordynamic slots including
symbol window andNITwere observed onchannel B.
0 Nosyntax error isobserved.
1 Syntax error isobserved onchannel B.
8 VFRB Valid frame received onchannel B.One ormore valid frames were received onchannel Binanystatic
ordynamic slotduring theobservation period. Reset isunder control ofthehost.
0 Novalid frame isreceived.
1 Valid frame isreceived onchannel B.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 SBVA Slot boundary violation onchannel A.One ormore slotboundary violations were observed onchannel
Aatanytime during theobservation period (static ordynamic slots including symbol window andNIT).
0 Noslotboundary violation isobserved.
1 Slot boundary violation isobserved onchannel A.
3 CIA Communication indicator channel A.One ormore valid frames were received onchannel Ainslots that
also contained anyadditional communication during theobservation period, thatis,oneormore slots
received avalid frame AND hadanycombination ofeither syntax error ORcontent error ORslot
boundary violation.
0 Novalid frame isreceived inslots containing anyadditional communication.
1 Valid frame isreceived onchannel Ainslots containing anyadditional communication.
2 CEDA Content error detected onchannel A.One ormore frames with acontent error were received on
channel Ainanystatic ordynamic slotduring theobservation period.
0 Noframe with content error isreceived.
1 Frame with content error isreceived onchannel A.
1 SEDA Syntax error detected onchannel A.One ormore syntax errors instatic ordynamic slots including
symbol window andNITwere observed onchannel A.
0 Nosyntax error isobserved.
1 Syntax error isobserved onchannel A.
0 VFRA Valid frame received onchannel A.One ormore valid frames were received onchannel Ainanystatic
ordynamic slotduring theobservation period.
0 Novalid frame isreceived.
1 Valid frame isreceived onchannel A.

<!-- Page 1384 -->

FlexRay Module Registers www.ti.com
1384 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.10 Even Sync IDRegisters (ESID[1-15])
Registers ESID1 toESID15 hold theframe IDsofthesync frames received ineven communication
cycles, assorted inascending order, with register ESID1 holding thelowest received sync frame ID.Ifthe
node transmits async frame inaneven communication cycle byitself, register ESID1 holds therespective
sync frame IDasconfigured inmessage buffer 0.The value isupdated during theNITofeach even
communication cycle. This register isreset when theCommunication Controller leaves CONFIG state or
enters STARTUP state.
Figure 26-155 andTable 26-134 illustrate thisregister.
Figure 26-155. Even Sync IDRegisters (ESIDn) [offset_CC =130h-168h]
31 16
Reserved
R-0
15 14 13 10 9 0
RXEB RXEA Reserved EID
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-134. Even Sync IDRegisters (ESIDn) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 RXEB Received even sync IDonchannel B.Async frame corresponding tothestored even sync IDwas
received onchannel B.
0 Sync frame isnotreceived onchannel B.
1 Sync frame isreceived onchannel B.
14 RXEA Received even sync IDonchannel A.Async frame corresponding tothestored even sync IDwas
received onchannel A.
0 Sync frame isnotreceived onchannel A.
1 Sync frame isreceived onchannel A.
13-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 EID Even Sync ID.Sync frame IDeven communication cycle.

<!-- Page 1385 -->

www.ti.com FlexRay Module Registers
1385 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.11 Odd Sync IDRegisters (OSID[1-15])
Registers OSID1 toOSID15 hold theframe IDsofthesync frames received inodd communication cycles,
assorted inascending order, with register OSID1 holding thelowest received sync frame ID.Ifthenode
transmits async frame inanoddcommunication cycle byitself, register OSID1 holds therespective sync
frame IDasconfigured inmessage buffer 0.The value isupdated during theNITofeach odd
communication cycle. This register isreset when theCommunication Controller leaves CONFIG state or
enters STARTUP state.
Figure 26-156 andTable 26-135 illustrate thisregister.
Figure 26-156. Odd Sync IDRegisters (OSIDn) [offset_CC =170h-1A8h]
31 16
Reserved
R-0
15 14 13 10 9 0
RXOB RXOA Reserved OID
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-135. Odd Sync IDRegisters (OSIDn) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 RXOB Received oddsync IDonchannel B.Async frame corresponding tothestored even sync IDwas
received onchannel B.
0 Sync frame isnotreceived onchannel B.
1 Sync frame isreceived onchannel B.
14 RXOA Received oddsync IDonchannel A.Async frame corresponding tothestored even sync IDwas
received onchannel A.
0 Sync frame isnotreceived onchannel A.
1 Sync frame isreceived onchannel A.
13-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 OID Odd Sync ID.Sync frame IDoddcommunication cycle.

<!-- Page 1386 -->

FlexRay Module Registers www.ti.com
1386 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.4.12 Network Management Vector Registers (NMV[1-3])
The three network management registers hold theaccrued NMvector (configurable 0-12 bytes). The
accrued NMvector isgenerated bythecommunication controller bybit-wise ORing each NMvector
received (valid frames with PPI=1)oneach channel.
The communication controller updates theNMvector attheendofeach communication cycle aslong as
thecommunication controller iseither inNORMAL_ACTIVE orNORMAL_PASSIVE state.
NMVn-bytes exceeding theconfigured NMvector length arenotvalid.
Figure 26-157 illustrates these registers andTable 26-136 shows theassignment ofthedata bytes tothe
network management vector.
Figure 26-157. Network Management Vector Registers (NMVn) [offset_CC =1B0h-1B8h]
31 16
NMI
R-0
15 0
NMI
R-0
LEGEND: R=Read only; -n=value after reset
Table 26-136. Assignment ofData Bytes toNetwork Management Vector
Bit
Word31 24 23 16 15 8 7 0
NMV1 Data3 Data2 Data1 Data0
NMV2 Data7 Data6 Data5 Data4
NMV3 Data11 Data10 Data9 Data8

<!-- Page 1387 -->

www.ti.com FlexRay Module Registers
1387 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.5 Message Buffer Control Registers
26.3.2.5.1 Message RAM Configuration (MRC)
The message RAM Configuration register defines thenumber ofmessage buffers assigned tothestatic
segment, dynamic segment, andFIFO. The register canbewritten during DEFAULT_CONFIG orCONFIG
state only.
Figure 26-158 andTable 26-137 illustrate thisregister.
Figure 26-158. Message RAM Configuration Register (MRC) [offset_CC =300h]
31 27 26 25 24 23 16
Reserved SPLM* SEC* LCB*
R-0 R-1h R-0 R/W-80h
15 8 7 0
FFB* FDB*
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-137. Message RAM Configuration Register (MRC) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 SPLM Sync Frame Payload Multiplex. This bitisonly evaluated ifthenode isconfigured assync node
(SUCC1.TXSY =1)orforsingle slotmode operation (SUCC1.TSM =1).When thisbitissetto1
message buffers 0and1arededicated forsync frame transmission with different payload data on
channel AandB.When thisbitissetto0,sync frames aretransmitted from message buffer 0with
thesame payload data onboth channels. Note thatthechannel filter configuration formessage
buffer 0resp. message buffer 1hastobechosen accordingly.
0 Only message buffer 0islocked against reconfiguration.
1 Both message buffers 0and1arelocked against reconfiguration.
25-24 SEC Secure Buffers. Notevaluated when thecommunication controller isinDEFAULT_CONFIG or
CONFIG state.
0 Reconfiguration ofmessage buffers isenabled with numbers <FFBh enabled.
Exception: Innodes configured forsync frame transmission orforsingle slotmode operation
message buffer 0(and ifSPLM =1,also message buffer 1)isalways locked.
1h Reconfiguration ofmessage buffers with numbers <FDB andwith numbers FFB islocked and
transmission ofmessage buffers forstatic segment with numbers FDB isdisabled.
2h Reconfiguration ofallmessage buffers islocked.
3h Reconfiguration ofallmessage buffers islocked andtransmission ofmessage buffers forstatic
segment with numbers FDB isdisabled.
23-16 LCB Last configured buffer.
0-7Fh Number ofmessage buffers isLCB +1.
≥80h Nomessage buffer isconfigured.
15-8 FFB First buffer ofFIFO.
0 Allmessage buffers areassigned totheFIFO.
0-7Fh Message buffers from FFB toLCB areassigned totheFIFO.
≥80h Nomessage buffer isassigned totheFIFO.
7-0 FDB First dynamic buffer.
0 Nogroup ofpure static buffers isconfigured.
0-7Fh Message buffers 0toFDB -1arereserved forstatic segment.
≥80h Nodynamic buffers areconfigured.

<!-- Page 1388 -->

FlexRay Module Registers www.ti.com
1388 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleNOTE: Incase thenode isconfigured assync node (SUCC1.TXSY =1)orforsingle slotmode
operation (SUCC1.TSM =1),message buffer 0resp. 1isreserved forsync frames orsingle
slotframes andhave tobeconfigured with thenode-specific keyslotID.Incase thenode is
neither configured assync node norforsingle slotoperation message buffer 0resp. 1is
treated likeallother message buffers.
Table 26-138. Buffer Configuration
Message Buffer 0 ↓Static Buffers
Message Buffer 1
... ↓Static +Dynamic Buffers ←FDB
↓FIFO ←FFB FIFO configured: FBB >FDB
NoFIFO configured: FFB
≥128
Message Buffer N-1
Message Buffer N ←LCB LCB≥FDB,
LCB≥FFB
The programmer must ensure thattheconfiguration defined byFDB(7-0), FFB(7-0), andLCB(7-0) isvalid.
NOTE: The communication controller does notcheck forerroneous configurations.
NOTE: Maximum Number ofHeader Sections
The maximum number ofheader sections is128. This means amaximum of128message
buffers canbeconfigured. The maximum length ofthedata sections is254bytes. The length
ofthedata section may beconfigured different foreach message buffer. Incase twoormore
message buffers areassigned toslot1byuseofcycle filtering, allofthem must belocated
either inthe"Static Buffers" oratthebeginning ofthe"Static +Dynamic Buffers" section.
The FlexRay protocol specification requires thateach node hastosend aframe initskey
slot. Therefore atleast message buffer 0isreserved fortransmission inthekeyslot. Due to
thisrequirement amaximum number of127message buffers canbeassigned totheFIFO.
Nevertheless, anonprotocol conform configuration without atransmission slotinthestatic
segment would stillbeoperational. The payload length configured andthelength ofthedata
sections need tobeconfigured identical forallmessage buffers belonging totheFIFO via
WRHS2. PLC andWRHS3.DP. When thecommunication controller isnotin
DEFAULT_CONFIG orCONFIG state reconfiguration ofmessage buffers belonging tothe
FIFO islocked.

<!-- Page 1389 -->

www.ti.com FlexRay Module Registers
1389 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.5.2 FIFO Rejection Filter (FRF)
The FIFO rejection filter defines auser specified sequence ofbitswith which channel, frame ID,andcycle
count oftheincoming frames arecompared. Together with theFIFO rejection filter mask (FRFM), this
register determines whether amessage isrejected bytheFIFO. The FRF register canbewritten during
DEFAULT_CONFIG orCONFIG state only.
Figure 26-159 andTable 26-139 illustrate thisregister.
Figure 26-159. FIFO Rejection Filter Register (FRF) [offset_CC =304h]
31 25 24 23 22 16
Reserved RNF* RSS* CYF*
R-0 R-1h R-1h R/W-0
15 13 12 2 1 0
Reserved FID* CH*
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-139. FIFO Rejection Filter Register (FRF) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 RNF Reject nullframes. Ifthisbitisset,received nullframes arenotstored intheFIFO.
0 Null frames arestored intheFIFO.
1 Reject allnullframes.
23 RSS Reject instatic segment. Ifthisbitisset,theFIFO isused only forthedynamic segment.
0 FIFO also used instatic segment.
1 Reject messages instatic segment.
22-16 CYF Cycle counter filter. The 7-bit cycle counter filter determines thecycle settowhich theframe ID
FIFO rejection filter andthechannel FIFO rejection filter areapplied. Incycles notbelonging tothe
cycle setspecified byCYF, allframes arerejected. Fordetails about theconfiguration ofthecycle
counter filter.
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12-2 FID 0-7FFh Frame IDfilter. Aframe IDfilter value of0means thatnoframe IDisrejected.
1-0 CH Channel filter.
Note: Ifreception onboth channels isconfigured, also inthestatic segment both frames (from
channel AandB)arealways stored intheFIFO, even ifthey areidentical.
0 Receive onboth channels.
1h Receive only onchannel B.
2h Receive only onchannel A.
3h Noreception.

<!-- Page 1390 -->

FlexRay Module Registers www.ti.com
1390 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.5.3 FIFO Rejection Filter Mask (FRFM)
The FIFO rejection filter mask specifies which ofthecorresponding frame IDfilter bitsarerelevant for
rejection filtering. Ifabitisset,itindicates thatthestate ofthecorresponding bitintheFRF register will
notbeconsidered forrejection filtering. The FRFM register canbewritten during DEFAULT_CONFIG or
CONFIG state only.
Figure 26-160 andTable 26-140 illustrate thisregister.
Figure 26-160. FIFO Rejection Filter Mask Register (FRFM) [offset_CC =308h]
31 16
Reserved
R-0
15 13 12 2 1 0
Reserved MFID* Reserved
R-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-140. FIFO Rejection Filter Mask Register (FRFM) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12-2 MFID Mask Frame IDFilter.
0 Corresponding frame IDfilter bitisused forrejection filtering.
1 Ignore corresponding frame IDfilter bit.
1-0 Reserved 0 Reads return 0.Writes have noeffect.
26.3.2.5.4 FIFO Critical Level (FCL)
The communication controller accepts modifications oftheregister inDEFAULT_CONFIG orCONFIG
state only.
Figure 26-161 andTable 26-141 illustrate thisregister.
Figure 26-161. FIFO Critical Level Register (FCL) [offset_CC =30Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved CL*
R-0 R/W-810h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-141. FIFO Critical Level Register (FCL) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 CL Critical Level. When thereceive FIFO filllevel FSR.RFFL isequal orgreater than thecritical level
configured byCL,thereceive FIFO critical level flagFSR.RFCL isset.IfCLisprogrammed tovalues >
128, bitFSR.RFCL isnever set.When FSR.RFCL changes from 0to1bitSIR.RFCL issetto1,andif
enabled, aninterrupt isgenerated.

<!-- Page 1391 -->

www.ti.com FlexRay Module Registers
1391 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6 Message Buffer Status Registers
26.3.2.6.1 Message Handler Status (MHDS)
Aflagiscleared bywriting a1tothecorresponding bitposition. Writing a0hasnoeffect ontheflag. A
hardware reset willalso clear theregister.
Figure 26-162 andTable 26-142 illustrate thisregister.
Figure 26-162. Message Handler Status (MHDS) [offset_CC =310h]
31 30 24 23 22 16
Rsvd MBU Rsvd MBT
R-0 R-0 R-0 R-0
15 14 8 7 6 5 4 3 2 1 0
Rsvd FMB CRAM MFMB FMBD PTFB2 PTFB1 PMR POBF PIBF
R-0 R-0 R-1h R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-142. Message Handler Status (MHDS) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-24 MBU 0-7Fh Message buffer updated. Number ofthemessage buffer thatwas updated lastbythe
communication controller. Forthismessage buffer, therespective NDand/or MBC flaginthenew
data 1...4 (NDAT1...4) andthemessage buffer status changed 1...4 (MBSC1...4) registers arealso
set.
Note: MBU arereset when thecommunication controller leaves CONFIG state orenters
STARTUP state.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 MBT 0-7Fh Message buffer transmitted. Number ofthelastsuccessfully transmitted message buffer. Ifthe
message buffer isconfigured forsingle-shot mode, therespective TXR flagintheTransmission
request register 1...4 (TXRQ1..4) was reset.
Note: MBT arereset when thecommunication controller leaves CONFIG state orenters
STARTUP state.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-8 FMB 0-7Fh Faulty message buffer. AnECC multi-bit error occurred when reading from amessage buffer or
when transferring data from Input Buffer orTransient Buffer 1,2tothemessage buffer referenced
byFMB. This value isonly valid when oneoftheflags PIBF, PMR, PTBF1, PTBF2, andflag
FMBD isset.Isnotupdated while flagFMBD isset.
7 CRAM Clear allinternal RAMs. Signals thatexecution ofthecontroller host interface command
CLEAR_RAMS isongoing (allbitsofallinternal RAM blocks arewritten to0).The bitissetby
hardware reset orbythecontroller host interface command CLEAR_RAMS.
0 Noexecution ofthecontroller host interface command CLEAR_RAMS.
1 Execution ofthecontroller host interface command CLEAR_RAMS isongoing.
6 MFMB Multiple faulty message buffers detected.
0 Noadditional faulty message buffer.
1 Another faulty message buffer was detected while flagFMBD isset.
5 FMBD Faulty message buffer detected.
0 Nofaulty message buffer.
1 Message buffer referenced byFMB holds faulty data duetoanECC multi-bit error.
4 PTBF2 ECC error transient buffer RAM B.
0 NoECC multi-bit error.
1 ECC multi-bit error occurred when reading transient buffer RAM B.
3 PTBF1 ECC error transient buffer RAM A.
0 NoECC multi-bit error.
1 ECC multi-bit error occurred when reading transient buffer RAM A.

<!-- Page 1392 -->

FlexRay Module Registers www.ti.com
1392 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-142. Message Handler Status (MHDS) Field Descriptions (continued)
Bit Field Value Description
2 PMR ECC error message RAM.
0 NoECC multi-bit error.
1 ECC multi-bit error occurred when reading message RAM.
1 POBF ECC error output buffer RAM 1,2.
0 NoECC multi-bit error.
1 ECC multi-bit error occurred when message handler read output buffer RAM 1,2.
0 PIBF ECC error input buffer RAM 1,2.
0 NoECC multi-bit error.
1 ECC multi-bit error occurred when message handler read input buffer RAM 1,2.
NOTE: When oneoftheflags PIBF, POBF, PMR, PTBF1, PTBF2 changes from 0to1,thePERR
flagintheError Interrupt Register (EIR) issetto1.
26.3.2.6.2 Last Dynamic Transmit Slot (LDTS)
The register isreset when thecommunication controller leaves CONFIG state orenters STARTUP state
Figure 26-163 andTable 26-143 illustrate thisregister.
Figure 26-163. Last Dynamic Transmit Slot (LDTS) [offset_CC =314h]
31 27 26 16
Reserved LDTB
R-0 R-0
15 11 10 0
Reserved LDTA
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-143. Last Dynamic Transmit Slot (LDTS) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 LDTB Last Dynamic Transmission Channel B.Value ofSlot Counter Batthetime ofthelastframe
transmission onchannel Ainthedynamic segment ofthisnode. Itisupdated attheendofthedynamic
segment andisreset to0ifnoframe was transmitted during thedynamic segment.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 LDTA Last Dynamic Transmission Channel A.Value ofSlot Counter Aatthetime ofthelastframe
transmission onchannel Ainthedynamic segment ofthisnode. Itisupdated attheendofthedynamic
segment andisreset to0ifnoframe was transmitted during thedynamic segment.

<!-- Page 1393 -->

www.ti.com FlexRay Module Registers
1393 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6.3 FIFO Status Register (FSR)
The register isreset when thecommunication controller leaves CONFIG state, enters STARTUP state, or
byCHI command CLEAR_RAMS..
Figure 26-164 andTable 26-144 illustrate thisregister.
Figure 26-164. FIFO Status Register (FSR) [offset_CC =318h]
31 16
Reserved
R-0
15 8 7 3 2 1 0
RFFL Reserved RFO RFCL RFNE
R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-144. FIFO Status Register (FSR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 RFFL 0-7Fh Receive FIFO FillLevel. Number ofFIFO buffers filled with new data notyetread bytheHost.
7-7 Reserved 0 Reads return 0.Writes have noeffect.
2 RFO Receive FIFO Overrun. The flagissetbythecommunication controller when areceive FIFO
overrun isdetected. When areceive FIFO overrun occurs, theoldest message isoverwritten with
theactual received message. Inaddition, interrupt flagRFO intheError Interrupt Register (EIR) is
set.The flagiscleared bythenext FIFO read access issued bytheHost.
0 Noreceive FIFO overrun isdetected.
1 Areceive FIFO overrun isdetected.
1 RFCL Receive FIFO Critical Level. This flagissetwhen thereceive FIFO filllevel RFFL isequal or
greater than thecritical level asconfigured byCLintheFIFO Critical Level register (FCL). The flag
iscleared bythecommunication controller assoon asRFFL drops below FCL.CL. When RFCL
changes from 0to1,theRFCL flagintheStatus Interrupt register (SIR) issetto1,andifenabled,
aninterrupt isgenerated.
0 Receive FIFO isbelow critical level.
1 Receive FIFO critical level isreached.
0 RFNE Receive FIFO NotEmpty. This flagissetbythecommunication controller when areceived valid
frame (data ornullframe depending onrejection mask) was stored intheFIFO. Inaddition,
interrupt flagRFNE intheStatus Interrupt register (SR) isset.The bitisreset after theHost has
read allmessage from theFIFO.
0 Receive FIFO isempty.
1 Receive FIFO isnotempty.

<!-- Page 1394 -->

FlexRay Module Registers www.ti.com
1394 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6.4 Message Handler Constraints Flags (MHDF)
Some constraints exist fortheMessage Handler regarding VBUSclk frequency, Message RAM
configuration, andFlexRay bustraffic. Inorder tosimplify software development, constraints violations are
reported bysetting flags intheMHDF.
Aflagiscleared bywriting a1tothecorresponding bitposition. Writing a0hasnoeffect ontheflag. A
hardware reset willalso clear theregister. The register isreset when thecommunication controller leaves
CONFIG state, enters STARTUP state, orbyCHI command CLEAR_RAMS.
Figure 26-165 andTable 26-145 illustrate thisregister.
Figure 26-165. Message Handler Constraints Flags (MHDF) [offset_CC =31Ch]
31 16
Reserved
R-0
15 9 8 7 6 5 4 3 2 1 0
Reserved WAHP TNSA TNSB TBFB TBFA FNFB FNFA SNUB SNUA
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-145. Message Handler Constraint Flags (MHDF) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 WAHP Write attempt toheader partition. This flagissetbythecommunication controller when themessage
handler tries towrite message data intotheheader partition oftheMessage RAM duetofaulty
configuration ofamessage buffer. The write attempt isnotexecuted, toprotect theheader partition
from unintended write accesses.
0 Nowrite attempt toheader partition.
1 Write attempt toheader partition.
7 TNSA Transmission NotStarted Channel A.This flagissetbytheCCwhen theMessage Handler was not
ready tostart ascheduled transmission onchannel Aattheaction point oftheconfigured slot.
0 Notransmission isnotstarted onchannel A.
1 Transmission isnotstarted onchannel A.
6 TNSB Transmission NotStarted Channel B.This flagissetbytheCCwhen theMessage Handler was not
ready tostart ascheduled transmission onchannel Battheaction point oftheconfigured slot.
0 Notransmission isnotstarted onchannel B.
1 Transmission isnotstarted onchannel B.
5 TBFB Transient buffer access failure B.This flagissetbythecommunication controller when aread orwrite
access toTBF Brequested byPRT Bcould notcomplete within theavailable time.
0 NoTBF Baccess failure.
1 TBF Baccess failure.
4 TBFA Transient buffer access failure A.This flagissetbythecommunication controller when aread orwrite
access toTBF Arequested byPRT Acould notcomplete within theavailable time.
0 NoTBF Aaccess failure.
1 TBF Aaccess failure.
3 FNFB Find sequence notfinished channel B.This flagissetbythecommunication controller when the
Message Handler, duetooverload condition, was notable tofinish afindsequence (scan ofMessage
RAM formatching message buffer) with respect tochannel B.
0 Nofindsequence isnotfinished forchannel B.
1 Find sequence isnotfinished forchannel B.
2 FNFA Find sequence notfinished channel A.This flagissetbythecommunication controller when the
Message Handler, duetooverload condition, was notable tofinish afindsequence (scan ofMessage
RAM formatching message buffer) with respect tochannel A.
0 Nofindsequence isnotfinished forchannel A.
1 Find sequence isnotfinished forchannel A.

<!-- Page 1395 -->

www.ti.com FlexRay Module Registers
1395 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-145. Message Handler Constraint Flags (MHDF) Field Descriptions (continued)
Bit Field Value Description
1 SNUB Status notupdated channel B.This flagissetbythecommunication controller when theMessage
Handler, duetooverload condition, was notable toupdate amessage buffer 'sstatus MBS with respect
tochannel B.
0 Nooverload condition occurred when updating MBS forchannel B.
1 MBS forchannel Bisnotupdated.
0 SNUA Status notupdated channel A.This flagissetbythecommunication controller when theMessage
Handler, duetooverload condition, was notable toupdate amessage buffer 'sstatus MBS with respect
tochannel A.
0 Nooverload condition occurred when updating MBS forchannel A.
1 MBS forchannel Aisnotupdated.
NOTE: When oneoftheflags SNUA, SNUB, FNFA, FNFB, TBFA, TBFB, WAHP changes from 0to
1,interrupt flagMHF intheError Interrupt register (EIR) issetto1.

<!-- Page 1396 -->

FlexRay Module Registers www.ti.com
1396 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6.5 Transmission Request Registers (TXRQ[1-4])
These four registers reflect thestate oftheTXR flags ofallconfigured message buffers. The flags are
evaluated fortransmit buffers only. Ifthenumber ofconfigured message buffers isless than 128, the
remaining TXR flags have nomeaning.
Figure 26-166 through Figure 26-169 andTable 26-146 illustrate these registers.
Figure 26-166. Transmission Request Register 4(TXRQ4) [offset_CC =32Ch]
31 16
TXR[127:112]
R-0
15 0
TXR[111:96]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-167. Transmission Request Register 3(TXRQ3) [offset_CC =328h]
31 16
TXR[95:80]
R-0
15 0
TXR[79:64]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-168. Transmission Request Register 2(TXRQ2) [offset_CC =324h]
31 16
TXR[63:48]
R-0
15 0
TXR[47:32]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-169. Transmission Request Register 1(TXRQ1) [offset_CC =320h]
31 16
TXR[31:16]
R-0
15 0
TXR[15:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 26-146. Transmission Request Registers (TXRQn) Field Description
Bit Field Value Description
127-0 TXR[ n] Transmission request.
0 The respective message buffer isnotready fortransmission.
1 Iftheflagisset,therespective message buffer isready fortransmission. Respectively, transmission of
thismessage buffer isinprogress. Insingle-shot mode theflags arereset after transmission has
completed.

<!-- Page 1397 -->

www.ti.com FlexRay Module Registers
1397 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6.6 New Data Registers (NDAT[1-4])
The four registers reflect thestate oftheNDflags ofallconfigured message buffers. NDflags
corresponding totransmit buffers have nomeaning. Ifthenumber ofconfigured message buffers isless
than 128, theremaining NDflags have nomeaning. The registers arereset when thecommunication
controller leaves CONFIG state orenters STARTUP state.
Figure 26-170 through Figure 26-173 andTable 26-147 illustrate these registers.
Figure 26-170. New Data Register 4(NDAT4) [offset_CC =33Ch]
31 16
ND[127:112]
R-0
15 0
ND[111:96]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-171. New Data Register 3(NDAT3) [offset_CC =338h]
31 16
ND[95:80]
R-0
15 0
ND[79:64]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-172. New Data Register 2(NDAT2) [offset_CC =334h]
31 16
ND[63:48]
R-0
15 0
ND[47:32]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-173. New Data Register 1(NDAT1) [offset_CC =330h]
31 16
ND[31:16]
R-0
15 0
ND[15:0]
R-0
LEGEND: R=Read only; -n=value after reset

<!-- Page 1398 -->

FlexRay Module Registers www.ti.com
1398 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-147. New Data Registers (NDATn) Field Descriptions
Bit Field Value Description
127-0 ND[n] New data.
0 The flags arereset when theheader section ofthecorresponding message buffer isreconfigured or
when thedata section hasbeen transferred totheoutput buffer.
1 The flags aresetwhen avalid received data frame matches themessage buffer 'sfilter configuration,
independent ofthepayload length received orthepayload length configured forthatmessage buffer.
The flags arenotsetafter reception ofnullframes except formessage buffers belonging tothereceive
FIFO.

<!-- Page 1399 -->

www.ti.com FlexRay Module Registers
1399 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.6.7 Message Buffer Status Changed Registers (MBSC[1-4])
The four registers reflect thestate oftheMBC flags ofallconfigured message buffers. Ifthenumber of
configured message buffers isless than 128, theremaining MBC flags have nomeaning.
Figure 26-174 through Figure 26-177 andTable 26-148 illustrate these registers.
Figure 26-174. Message Buffer Status Changed Register 4(MBSC4) [offset_CC =34Ch]
31 16
MBS[127:112]
R-0
15 0
MBS[111:96]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-175. Message Buffer Status Changed Register 3(MBSC3) [offset_CC =348h]
31 16
MBS[95:80]
R-0
15 0
MBS[79:64]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-176. Message Buffer Status Changed Register 2(MBSC2) [offset_CC =344h]
31 16
MBS[63:48]
R-0
15 0
MBS[47:32]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 26-177. Message Buffer Status Changed Register 1(MBSC1) [offset_CC =340h]
31 16
MBS[31:16]
R-0
15 0
MBS[15:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 26-148. Message Buffer Status Changed Registers (MBSCn) Field Descriptions
Bit Field Value Description
127-0 MBS[ n] Message buffer status changed.
0 Aflagisreset when theheader section ofthecorresponding message buffer isreconfigured orwhen it
hasbeen transferred totheOutput Buffer.
1 The flagissetwhenever theMessage Handler changes oneofthestatus flags VFRA, VFRB, SEOA,
SEOB, CEOA, CEOB, SVOA, SVOB, TCIA, TCIB, ESA, ESB, MLST, FTA, FTB intheheader section
(see Message Buffer Status (MBS)) oftherespective message buffer.

<!-- Page 1400 -->

FlexRay Module Registers www.ti.com
1400 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.7 Identification Registers
26.3.2.7.1 Core Release Register (CREL)
Figure 26-178 andTable 26-149 illustrate thisregister.
Figure 26-178. Core Release Register (CREL) [offset_CC =3F0h]
31 28 27 20 19 16
REL STEP YEAR
R-release info R-release info R-release info
15 8 7 0
MON DAY
R-release info R-release info
LEGEND: R=Read only; -n=value after reset
Table 26-149. Core Release Register (CREL) Field Descriptions
Bit Field Value Description
31-28 REL 0-Fh Core Release. One digit, BCD-coded.
27-20 STEP 0-FFh Step ofCore Release. Two digits, BCD-coded.
19-16 YEAR 0-Fh Design Time Stamp, Year. One digit, BCD-coded.
15-8 MON 0-FFh Design Time Stamp, Month. Two digits, BCD-coded.
7-0 DAY 0-FFh Design Time Stamp, Day. Two digits, BCD-coded.
Table 26-150 shows therelease coding inregister CREL.
Table 26-150. Release Coding
Release Step Sub-StepCore Release Register
ContentsName
1 0 0 1006 0519 Revision 1.0.0
1 0 1 1016 1211 Revision 1.0.1
1 0 2 10271031 Revision 1.0.2
1 0 3 10390206 Revision 1.0.3
26.3.2.7.2 Endian Register (ENDN)
Figure 26-179 andTable 26-151 illustrate thisregister.
Figure 26-179. Endian Register (ENDN) [offset_CC =3F4h]
31 16
ETV
R-8765h
15 0
ETV
R-4321h
LEGEND: R=Read only; -n=value after reset
Table 26-151. Endian Register (ENDN) Field Descriptions
Bit Field Description
31-0 ETV Endianness Test Value. The Endianness testvalue is87654321h.

<!-- Page 1401 -->

www.ti.com FlexRay Module Registers
1401 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.8 Input Buffer
Double buffer structure consisting ofinput buffer host andinput buffer shadow. While thehost canwrite to
input buffer host, thetransfer tothemessage RAM isdone from input buffer shadow. The input buffer
holds theheader anddata sections tobetransferred totheselected message buffer inthemessage RAM.
Itisused toconfigure themessage buffers inthemessage RAM andtoupdate thedata sections of
transmit buffers.
When updating theheader section ofamessage buffer intheMessage RAM from theInput Buffer, the
Message Buffer Status asdescribed inMessage Buffer Status (MBS), Message Buffer Status (MBS) is
automatically reset to0.
The header sections ofmessage buffers belonging tothereceive FIFO canonly be(re)configured when
thecommunication controller isinDEFAULT_CONFIG orCONFIG state. Forthose message buffers only
thepayload length configured andthedata pointer need tobeconfigured bybitsPLC oftheWrite Header
Section 2(WRHS2) andbybitsDPofWrite Header Section 3(WRHS3). Allinformation required for
acceptance filtering istaken from theFIFO rejection filter andtheFIFO rejection filter mask.
26.3.2.8.1 Write Data Section Registers (WRDS[1-64])
Holds thedata words tobetransferred tothedata section oftheaddressed message buffer. The data
words (DWn)arewritten tothemessage RAM intransmission order from DW1(byte0, byte1) toDW
PL(DWPL=number ofdata words asdefined bythepayload length configured inPLC oftheWrite Header
Section 2(WRHS2).
Figure 26-180 andTable 26-152 illustrate thisregister.
Figure 26-180. Write Data Section Registers (WRDSn) [offset_CC =400h-4FCh]
31 16
MD
R/W-0
15 0
MD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-152. Write Data Section Registers (WRDSn) Field Descriptions
Bit Field Description
31-0 MD Message data.
Note: DW127 islocated onWRDS64.MD. Inthiscase WRDS64.MD isunused (novalid data).The input
buffer RAMs areinitialized to0when leaving hardware reset orbythecontroller host interface
command CLEAR_RAMS.
MD(31-24) =DW2n,byte4n-1
MD(23-16) =DW2n,byte4n-2
MD(15-8) =DW2n-1,byte4n-3
MD(7-0) =DW2n-1,byte4n-4

<!-- Page 1402 -->

FlexRay Module Registers www.ti.com
1402 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.8.2 Write Header Section Register 1(WRHS1)
Figure 26-181 andTable 26-153 illustrate thisregister.
Figure 26-181. Write Header Section Register 1(WRHS1) [offset_CC =500h]
31 30 29 28 27 26 25 24 23 22 16
Reserved MBI TXM PPIT CFG CHB CHA Rsvd CYC
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0
15 11 10 0
Reserved FID
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-153. Write Header Section Register 1(WRHS1) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29 MBI Message buffer interrupt. This bitenables thereceive/transmit interrupt forthecorresponding
message buffer. After adedicated receive buffer hasbeen updated bythemessage handler, flag
RXIand/or MBSI inthestatus interrupt register areset.After successful transmission theTXIflag
inthestatus interrupt register isset.
0 The corresponding message buffer interrupt isenabled.
1 The corresponding message buffer interrupt isdisabled.
28 TXM Transmission mode. This bitisused toselect thetransmission mode.
0 Continuous mode.
1 Single-shot mode.
27 PPIT Payload preamble indicator transmit. This bitisused tocontrol thestate ofthePayload Preamble
Indicator intransmit frames. Ifthebitissetinastatic message buffer, therespective message
buffer holds network management information. Ifthebitissetinadynamic message buffer, the
firsttwobytes ofthepayload segment may beused formessage IDfiltering bythereceiver.
Message IDfiltering ofreceived FlexRay frames isnotsupported bytheFlexRay module, butcan
bedone bythehost CPU.
0 Payload Preamble Indicator isnotset.
1 Payload Preamble Indicator isset.
26 CFG Message buffer configuration bit.This bitisused toconfigure thecorresponding buffer astransmit
buffer orasreceive buffer. Formessage buffers belonging tothereceive FIFO thebitisnot
evaluated.
0 The corresponding buffer isconfigured asreceive buffer.
1 The corresponding buffer isconfigured astransmit buffer.
25-24 CHB, CHA 0-3h Channel filter control.
The 2-bit channel filtering field associated with each buffer serves asafilter forreceive buffers and
asacontrol field fortransmit buffers. See Table 26-154 forbitdescriptions.
Note: Ifamessage buffer isconfigured forthedynamic segment andboth bitsofthe
channel filtering field aresetto1,noframes aretransmitted resp. received frames are
ignored (same function asCHA =CHB =0)
23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 CYC 0-7Fh Cycle code. The 7-bit cycle code determines thecycle setused forcycle counter filtering.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 FID 0-7FFh Frame ID.
Frame IDoftheselected message buffer. The frame IDdefines theslotnumber fortransmission /
reception oftherespective message.
Note: Message buffers with frame ID=0areconsidered notvalid.

<!-- Page 1403 -->

www.ti.com FlexRay Module Registers
1403 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-154. Channel Filter Control BitDescriptions
CHA CHBTransmit Buffer
transmit frame onReceive Buffer
store frame received from
1 1both channels
(static segment only)channel AorB
(store firstsemantically valid frame, static segment
only)
1 0 channel A channel A
0 1 channel B channel B
0 0 notransmission ignore frame
26.3.2.8.3 Write Header Section Register 2(WRHS2)
Figure 26-182 andTable 26-155 illustrate thisregister.
Figure 26-182. Write Header Section Register 2(WRHS2) [offset_CC =504h]
31 23 22 16
Reserved PLC
R-0 R/W-0
15 11 10 0
Reserved CRC
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-155. Write Header Section Register 2(WRHS2) Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 PLC 0-7Fh Payload length configured. Length ofdata section (number of2-byte words) asconfigured bythe
host. During static segment thestatic frame data length asconfigured bySFDL intheMHD
configuration register defines thepayload length forallstatic frames. Ifthepayload length
configured byPLC isshorter than thisvalue padding bytes areinserted toensure thatframes have
proper physical length. The padding pattern islogical 0.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 CRC 0-7FFh Header CRC. Receive Buffer: configuration notrequired. Transmit buffer: Header CRC calculated
andconfigured bythehost. Forcalculation oftheheader CRC thepayload length oftheframe
send onthebushastobeconsidered. Instatic segment thepayload length ofallframes is
configured byMHDC.SFDL.

<!-- Page 1404 -->

FlexRay Module Registers www.ti.com
1404 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.8.4 Write Header Section Register 3(WRHS3)
Figure 26-183 andTable 26-156 illustrate thisregister.
Figure 26-183. Write Header Section Register 3(WRHS3) [offset_CC =508h]
31 16
Reserved
R-0
15 11 10 0
Reserved DP
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-156. Write Header Section Register 3(WRHS3) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 DP 1-7FFh Data pointer. Pointer tothefirst32-bit word ofthedata section oftheaddressed message buffer in
themessage RAM.

<!-- Page 1405 -->

www.ti.com FlexRay Module Registers
1405 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.8.5 Input Buffer Command Mask Register (IBCM)
Configures how themessage buffer inthemessage RAM selected bytheinput buffer command request
register isupdated. When IBFhost andIBFshadow areswapped, also mask bitsLHSH, LDSH, and
STXRH areswapped with bitsLHSS, LDSS, andSTXRS tokeep them attached totherespective input
buffer transfer.
Figure 26-184 andTable 26-157 illustrate thisregister.
Figure 26-184. Input Buffer Command Mask Register (IBCM) [offset_CC =510h]
31 19 18 17 16
Reserved STXRS LDSS LHSS
R-0 R-0 R-0 R-0
15 3 2 1 0
Reserved STXRH LDSH LHSH
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-157. Input Buffer Command Mask Register (IBCM) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 STXRS Settransmission request shadow.
0 Reset TXR flag.
1 SetTXR flag; transmit buffer isreleased fortransmission (operation ongoing orfinished).
17 LDSS Load data section shadow.
0 Data section isnotupdated.
1 Data section isselected fortransfer from input buffer tothemessage RAM (transfer ongoing or
finished).
16 LHSS Load header section shadow.
0 Header section isnotupdated.
1 Header section isselected fortransfer from input buffer tothemessage RAM (transfer ongoing or
finished).
15-3 Reserved 0 Reads return 0.Writes have noeffect.
2 STXRH Settransmission request host. Ifthisbitissetto1,thetransmission request flagTXR fortheselected
message buffer issetinthetransmission request registers torelease themessage buffer for
transmission. Insingle-shot mode theflagiscleared bythecommunication controller after transmission
hascompleted. The flags isevaluated fortransmit buffers only.
0 Reset transmission request flag.
1 Settransmission request flag; transmit buffer isreleased fortransmission.
1 LDSH Load data section host.
0 Data section isnotupdated.
1 Data section isselected fortransfer from input buffer tothemessage RAM.
0 LHSH Load header section host.
0 Header section isnotupdated.
1 Header section isselected fortransfer from input buffer tothemessage RAM.

<!-- Page 1406 -->

FlexRay Module Registers www.ti.com
1406 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.8.6 Input Buffer Command Request Register (IBCR)
When thehost writes thenumber ofatarget message buffer inthemessage RAM toIBRH intheinput
buffer command request register, IBFhost andIBFshadow areswapped. Inaddition themessage buffer
numbers stored under IBRH andIBRS arealso swapped.
With thiswrite operation theIBSYS bitintheinput buffer command request register issetto1.The
message handler then starts totransfer thecontents ofIBFshadow tothemessage buffer inthemessage
RAM selected byIBRS.
While themessage handler transfers thedata from IBFshadow tothetarget message buffer inthe
message RAM, thehost may configure thenext message intheIBFhost. After thetransfer between IBF
shadow andthemessage RAM hascompleted, theIBSYS bitissetback to0andthenext transfer tothe
message RAM may bestarted bythehost bywriting therespective target message buffer number to
IBRH.
Ifawrite access toIBRH occurs while IBSYS is1,IBSYH issetto1.After completion oftheongoing data
transfer from IBFshadow tothemessage RAM, IBFhost andIBFshadow areswapped, IBSYH isreset to
0.IBSYS remains setto1,andthenext transfer tothemessage RAM isstarted. Inaddition themessage
buffer numbers stored under IBRH andIBRS arealso swapped.
Any write access toanInput Buffer Register while both IBSYS andIBSYH aresetwillcause theerror flag
IIBA intheError Interrupt Register (EIR) tobeset.Inthiscase theInput Buffer willnotbechanged.
Figure 26-185 andTable 26-158 illustrate thisregister.
Figure 26-185. Input Buffer Command Request Register (IBCR) [offset_CC =514h]
31 30 23 22 16
IBSYS Reserved IBRS
R-0 R-0 R-0
15 14 7 6 0
IBSYH Reserved IBRH
R-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-158. Input Buffer Command Request Register (IBCR) Field Descriptions
Bit Field Value Description
31 IBSYS Input buffer busy shadow. Setto1after writing IBRH. When thetransfer between IBFshadow and
themessage RAM hascompleted, IBSYS issetback to0.
0 Transfer between IBFshadow andmessage RAM iscompleted.
1 Transfer between IBFshadow andmessage RAM isinprogress.
30-23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 IBRS 0-7Fh Input buffer request shadow. Number ofthetarget message buffer actually updated /lately
updated.
15 IBSYH Input buffer busy host. Setto1bywriting IBRH while IBSYS isstill1.After theongoing transfer
between IBFshadow andthemessage RAM hascompleted, theIBSYH issetback to0.
0 Norequest ispending.
1 Request while transfer between IBFshadow andmessage RAM isinprogress.
14-7 Reserved 0 Reads return 0.Writes have noeffect.
6-0 IBRH 0-7Fh Input buffer request host. Selects thetarget message buffer intheMessage RAM fordata transfer
from Input Buffer.

<!-- Page 1407 -->

www.ti.com FlexRay Module Registers
1407 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9 Output Buffer
Double buffer structure consisting ofoutput buffer host andoutput buffer shadow. While thehost canread
from output buffer host, thetransfer from themessage RAM isdone tooutput buffer shadow. The output
buffer holds theheader anddata sections ofrequested message buffers transferred from themessage
RAM. Used toread outmessage buffers from themessage RAM.
26.3.2.9.1 Read Data Section Registers (RDDS[1-64])
Holds thedata words read from thedata section oftheaddressed message buffer. The data words (DWn)
areread from themessage RAM inreception order from DW1(byte0, byte1) toDWPL(DWPL=number of
data words asdefined bythepayload length configured inbitsPLC(6-0) oftheRead Header Section 2
(RDHS2)).
Figure 26-186 andTable 26-159 illustrate thisregister.
Figure 26-186. Read Data Section Registers (RDDSn) [offset_CC =600h-6FCh]
31 16
MD
R/W-0
15 0
MD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 26-159. Read Data Section Registers (RDDSn) Field Descriptions
Bit Field Description
31-0 MD Message data.
Note: DW127 islocated onRDDS64.MD. Inthiscase, RDDS64.MD isunused (novalid data).The input
buffer RAMs areinitialized to0when leaving hardware reset orbythecontroller host interface
command CLEAR_RAMS.
MD(31-24) =DW2n,byte4n-1
MD(23-16) =DW2n,byte4n-2
MD(15-8) =DW2n-1,byte4n-3
MD(7-0) =DW2n-1,byte4n-4

<!-- Page 1408 -->

FlexRay Module Registers www.ti.com
1408 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.2 Read Header Section Register 1(RDHS1)
Figure 26-187 andTable 26-160 illustrate thisregister.
Figure 26-187. Read Header Section Register 1(RDHS1) [offset_CC =700h]
31 30 29 28 27 26 25 24 23 22 16
Reserved MBI TXM PPIT CFG CHB CHA Rsvd CYC
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0
15 11 10 0
Reserved FID
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-160. Read Header Section Register 1(RDHS1) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29 MBI Message buffer interrupt.
0 The corresponding message buffer interrupt isenabled.
1 The corresponding message buffer interrupt isdisabled.
28 TXM Transmission mode. This bitisused toselect thetransmission mode.
0 Continuous mode.
1 Single-shot mode.
27 PPIT Payload preamble indicator transmit.
0 Payload Preamble Indicator isnotset.
1 Payload Preamble Indicator isset.
26 CFG Message buffer configuration bit.
0 The corresponding buffer isconfigured asreceive buffer.
1 The corresponding buffer isconfigured asTransmit buffer.
25-24 CHB, CHA Channel filter control.
See Table 26-154 forbitdescriptions.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 CYC 0-7Fh Cycle code. The 7-bit cycle code determines thecycle setused forcycle counter filtering.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 FID 0-7FFh Frame ID.
Frame IDoftheselected message buffer.
Note: Message buffers with frame ID=0areconsidered notvalid.
NOTE: Incase themessage buffer read from themessage RAM belongs tothereceive FIFO, FID,
andCHA, CHB were updated from thereceived frame while CYC, CFG, PPIT, TXM, and
MBI arereset to0.
Forbitdescription, seealso Section 26.3.2.8.2 .

<!-- Page 1409 -->

www.ti.com FlexRay Module Registers
1409 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.3 Read Header Section Register 2(RDHS2)
Figure 26-188 andTable 26-161 illustrate thisregister.
Figure 26-188. Read Header Section Register 2(RDHS2) [offset_CC =704h]
31 30 24 23 22 16
Rsvd PLR Rsvd PLC
R-0 R-0 R-0 R-0
15 11 10 0
Reserved CRC
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-161. Read Header Section Register 2(RDHS2) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-24 PLR 0-7Fh Payload length received. Payload length value updated from received frame (exception: if
message buffer belongs tothereceive FIFO PLR isalso updated from received nullframes).
When amessage isstored intoamessage buffer thefollowing behavior with respect topayload
length received andpayload length configured isimplemented:
PLR >PLC: The payload data stored inthemessage buffer istruncated tothepayload length
configured ifPLC even orelse truncated toPLC +1.
PLR <=PLC: The received payload data isstored intothemessage buffers data section. The
remaining data bytes ofthedata section asconfigured byPLC arefilled with undefined data.
PLR =zero: The message buffers data section isfilled with undefined data.
PLC =zero: Message buffer hasnodata section configured. Nodata isstored intothemessage
buffers data section.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 PLC 0-7Fh Payload length configured. Length ofdata section (number of2-byte words) asconfigured bythe
host.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 CRC 0-7FFh Header CRC.
Receive buffer: Header CRC isupdated from receive frame.
Transmit buffer: Header CRC iscalculated andconfigured bythehost.
NOTE: The Message RAM isorganized in4-byte words. When received data isstored intoa
message buffer's data section, thenumber of2-byte data words written intothemessage
buffer isPLC rounded tothenext even value. PLC should beconfigured identical forall
message buffers belonging tothereceive FIFO. Header 2isupdated from data frames only.

<!-- Page 1410 -->

FlexRay Module Registers www.ti.com
1410 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.4 Read Header Section Register 3(RDHS3)
Figure 26-189 andTable 26-162 illustrate thisregister.
Figure 26-189. Read Header Section Register 3(RDHS3) [offset_CC =708h]
31 30 29 28 27 26 25 24 23 22 21 16
Reserved RES PPI NFI SYN SFI RCI Reserved RCC
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
15 11 10 0
Reserved DP
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-162. Read Header Section Register 3(RDHS3) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29 RES 0-1 Reserved bit.Reflects thestate ofthereceived reserved bit.The reserved bitistransmitted as0.
28 PPI Payload preamble indicator. The payload preamble indicator defines whether anetwork management
vector ormessage IDiscontained within thepayload segment ofthereceived frame.
0 The payload segment ofthereceived frame does notcontain anetwork management vector ora
message ID.
1 Static segment: Network management vector atthebeginning ofthepayload.
Dynamic segment: Message IDatthebeginning ofthepayload.
27 NFI Null frame indicator. Issetto1after storage ofthefirstreceived data frame.
0 Uptonow nodata frame hasbeen stored intotherespective message buffer.
1 Atleast onedata frame hasbeen stored intotherespective message buffer.
26 SYN Sync frame indicator. Async frame ismarked bythesync frame indicator.
0 The received frame isnotasync frame.
1 The received frame isasync frame.
25 SFI Startup frame indicator. Astartup frame ismarked bythestartup frame indicator.
0 The received frame isnotastartup frame.
1 The received frame isastartup frame.
24 RCI Received onchannel indicator. Indicates thechannel from which thereceived data frame was taken to
update therespective receive buffer.
0 Frame isreceived onchannel B.
1 Frame isreceived onchannel A.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 RCC Receive cycle count. Cycle counter value updated from received frame.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 DP Data pointer. Pointer tothefirst32-bit word ofthedata section oftheaddressed message buffer inthe
message RAM.
NOTE: Header 3isupdated from data frames only.

<!-- Page 1411 -->

www.ti.com FlexRay Module Registers
1411 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.5 Message Buffer Status Register (MBS)
The message buffer status isupdated bythecommunication controller with respect totheassigned
channel(s) latest attheendoftheslotfollowing theslotassigned tothemessage buffer. The flags are
updated only when thecommunication controller isinNORMAL_ACTIVE orNORMAL_PASSIVE state. If
only onechannel (AorB)isassigned toamessage buffer, thechannel-specific status flags oftheother
channel arewritten to0.Ifboth channels areassigned toamessage buffer, thechannel-specific status
flags ofboth channels areupdated. The message buffer status isupdated only when theslotcounter
reached theconfigured frame IDandwhen thecycle counter filter matched. When theHost updates a
message buffer viaInput Buffer, allMBS flags arereset to0independent ofwhich IBCM bitsaresetor
not.
Whenever theMessage Handler changes oneoftheflags VFRA, VFRB, SEOA, SEOB, CEOA, CEOB,
SVOA, SVOB, TCIA, TCIB, ESA, ESB, MLST, FTA, FTB therespective message buffer's MBC flagin
registers MBSC1/2/3/4 isset.
Figure 26-190 andTable 26-163 illustrate thisregister.
Figure 26-190. Message Buffer Status Register (MBS) [offset_CC =70Ch]
31 30 29 28 27 26 25 24 23 22 21 16
Reserved RESS PPIS NFIS SYNS SFIS RCIS Reserved CCS
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
FTB FTA Rsvd MLST ESB ESA TCIB TCIA SVOB SVOA CEOB CEOA SEOB SEOA VFRB VFRA
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 26-163. Message Buffer Status Register (MBS) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29 RESS 0-1 Reserved bitstatus. Reflects thestate ofthereceived reserved bit.The reserved bitistransmitted
as0.
28 PPIS Payload preamble indicator status. The payload preamble indicator defines whether anetwork
management vector ormessage IDiscontained within thepayload segment ofthereceived frame.
0 The payload segment ofthereceived frame does notcontain anetwork management vector ora
message ID.
1 Static segment: Network management vector atthebeginning ofthepayload Dynamic segment:
Message IDatthebeginning ofthepayload.
27 NFIS Null frame indicator status. Ifsetto0,thepayload segment ofthereceived frame contains no
usable data.
0 Received frame isanullframe.
1 Received frame isnotanullframe.
26 SYNS Sync frame indicator status. Async frame ismarked bythesync frame indicator.
0 Nosync frame isreceived.
1 The received frame isasync frame.
25 SFIS Startup frame indicator status. Astartup frame ismarked bythestartup frame indicator.
0 Nostartup frame isreceived.
1 The received frame isastartup frame.
24 RCIS Received onchannel indicator status. Indicates thechannel onwhich theframe was received.
0 Frame isreceived onchannel B.
1 Frame isreceived onchannel A.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CCS 0-3Fh Cycle count status. Actual cycle count when status was updated.

<!-- Page 1412 -->

FlexRay Module Registers www.ti.com
1412 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-163. Message Buffer Status Register (MBS) Field Descriptions (continued)
Bit Field Value Description
15 MTB Frame transmitted onchannel B.Indicates thatthisnode hastransmitted adata frame inthe
configured slotonchannel B.
0 Nodata frame istransmitted onchannel B.
1 Data frame istransmitted onchannel B.
14 MTA Frame transmitted onchannel A.Indicates thatthisnode hastransmitted adata frame inthe
configured slotonchannel A.
0 Nodata frame istransmitted onchannel A.
1 Data frame istransmitted onchannel A.
13 Reserved 0 Reads return 0.Writes have noeffect.
12 MLST Message lost. The flagissetincase theHost didnotread themessage before themessage buffer
was updated from areceived data frame. Notaffected byreception ofnullframes except for
message buffers belonging tothereceive FIFO. The flagisreset byahost write tothemessage
buffer viaIBForwhen anew message isstored intothemessage buffer after themessage buffers
NDflagwas reset byreading outthemessage buffer viaOBF.
0 Nomessage islost.
1 Unprocessed message was overwritten.
11 ESB Empty slotchannel B.Inanempty slotthere isnoactivity onthebus. The condition ischecked in
static anddynamic slots.
0 Bus activity isdetected intheconfigured slotonchannel B.
1 Nobusactivity isdetected intheconfigured slotonchannel B.
10 ESA Empty slotchannel A.Inanempty slotthere isnoactivity onthebus. The condition ischecked in
static anddynamic slots.
0 Bus activity isdetected intheconfigured slotonchannel A.
1 Nobusactivity isdetected intheconfigured slotonchannel A.
9 TCIB Transmission conflict indication channel B.Atransmission conflict indication issetifatransmission
conflict hasoccurred onchannel B.
0 Notransmission conflict occurred onchannel B.
1 Transmission conflict occurred onchannel B.
8 TCIA Transmission conflict indication channel A.Atransmission conflict indication issetifatransmission
conflict hasoccurred onchannel A.
0 Notransmission conflict occurred onchannel A.
1 Transmission conflict occurred onchannel A.
7 SVOB Slot boundary violation observed onchannel B.Aslotboundary violation (channel active atthestart
orattheendoftheassigned slot) was observed onchannel B.
0 Noslotboundary violation isobserved onchannel B.
1 Slot boundary violation isobserved onchannel B.
6 SVOA Slot boundary violation observed onchannel A.Aslotboundary violation (channel active atthestart
orattheendoftheassigned slot) was observed onchannel A.
0 Noslotboundary violation isobserved onchannel A.
1 Slot boundary violation isobserved onchannel A.
5 CEOB Content error observed onchannel B.Acontent error was observed intheconfigured sloton
channel B.
0 Nocontent error isobserved onchannel B.
1 Content error isobserved onchannel B.
4 CEOA Content error observed onchannel A.Acontent error was observed intheconfigured sloton
channel A.
0 Nocontent error isobserved onchannel A.
1 Content error isobserved onchannel A.
3 SEOB Syntax error observed onchannel B.Asyntax error was observed intheassigned slotonchannel
B.
0 Nosyntax error isobserved onchannel B.
1 Syntax error isobserved onchannel B.

<!-- Page 1413 -->

www.ti.com FlexRay Module Registers
1413 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-163. Message Buffer Status Register (MBS) Field Descriptions (continued)
Bit Field Value Description
2 SEOA Syntax error observed onchannel A.Asyntax error was observed intheassigned slotonchannel
A.
0 Nosyntax error isobserved onchannel A.
1 Syntax error isobserved onchannel A.
1 VFRB Valid frame received onchannel B.Avalid frame indication issetifavalid frame was received on
channel B.
0 Novalid frame isreceived onchannel B.
1 Valid frame isreceived onchannel B.
0 VFRA Valid frame received onchannel A.Avalid frame indication issetifavalid frame was received on
channel A.
0 Novalid frame isreceived onchannel A.
1 Valid frame isreceived onchannel A.
NOTE: The status bitsRESS, PPPIS, NFIS, FYNS, SFIS andRCIS areupdated from both valid
data andnullframes. Ifnovalid frame was received, theprevious value ismaintained.
The FlexRay protocol specification requires thatFTA, andFTB canonly bereset bythe
CPU. Therefore theCycle Count Status CCS forthese bitsisonly valid forthecycle where
thebitsaresetto1.

<!-- Page 1414 -->

FlexRay Module Registers www.ti.com
1414 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.6 Output Buffer Command Mask Register (OBCM)
Configures how theOutput Buffer isupdated from themessage buffer intheMessage RAM selected by
bitsOBRS oftheoutput buffer command request register. Mask bitsRDSS andRHSS arecopied tothe
register internal storage when aMessage RAM transfer isrequested byOBCR.REQ. When OBF host and
OBF shadow areswapped, also mask bitsRDSH andRHSH areswapped with bitsRDSS andRHSS to
keep them attached totherespective output buffer transfer.
Figure 26-191 andTable 26-164 illustrate thisregister.
Figure 26-191. Output Buffer Command Mask Register (OBCM) [offset_CC =700h]
31 18 17 16
Reserved RDSH RHSH
R-0 R-0 R-0
15 2 1 0
Reserved RDSS RHSS
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 26-164. Output Buffer Command Mask Register (OBCM) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17 RDSH Read data section host.
0 Data section isnotread.
1 Data section isselected fortransfer from message RAM tooutput buffer.
16 RHSH Read header section host.
0 Header section isnotread.
1 Header section isselected fortransfer from message RAM tooutput buffer.
15-2 Reserved 0 Reads return 0.Writes have noeffect.
1 RDSS Read Data Section shadow.
0 Data section isnotread.
1 Data section isselected fortransfer from message RAM tooutput buffer.
0 RHSS Read header section shadow.
0 Header section isnotread.
1 Header section isselected fortransfer from message RAM tooutput buffer.
NOTE: After thetransfer oftheheader section from themessage RAM toOBF shadow has
completed, themessage buffer status Changed flagMBS oftheselected message buffer in
themessage buffer Changed 1,2,3,4 registers iscleared. After thetransfer ofthedata
section from themessage RAM toOBF shadow hascompleted, theNew Data flagNDofthe
selected message buffer intheNew Data 1,2,3,4 registers iscleared.

<!-- Page 1415 -->

www.ti.com FlexRay Module Registers
1415 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay Module26.3.2.9.7 Output Buffer Command Request Register (OBCR)
After setting bitREQ to1while OBSYS is0,OBSYS isautomatically setto1,OBRS(6-0) iscopied tothe
register internal storage, mask bitsOBCM.RDSS andOBCM.RHSS arecopied toregister OBCM internal
storage, andthetransfer ofthemessage buffer selected byOBRS(6-0) from theMessage RAM toOBF
Shadow isstarted. When thetransfer between theMessage RAM andOBF shadow hascompleted, thisis
signaled bysetting OBSYS back to0.
Bysetting bitVIEW to1while OBSYS is0,OBF Host andOBF shadow areswapped. Additionally mask
bitsOBCM.RDSH andOBCM.RHSH areswapped with theregister OBCM internal storage tokeep them
attached totherespective output buffer transfer. OBRH(6-0) signals thenumber ofthemessage buffer
currently accessible bytheHost.
IfbitsREQ andVIEW aresetto1with thesame write access while OBSYS is0,OBSYS isautomatically
setto1andOBF shadow andOBF host areswapped. Additionally mask bitsOBCM.RDSH and
OBCM.RHSH areswapped with theregisters internal storage tokeep them attached totherespective
output buffer transfer. Afterwards OBRS(6-0) iscopied totheregister
internal storage, andthetransfer oftheselected message buffer from theMessage RAM toOBF shadow
isstarted. While thetransfer isongoing theHost canread themessage buffer transferred bytheprevious
transfer from OBF host. When thecurrent transfer between Message RAM andOBF shadow has
completed, thisissignaled bysetting OBSYS back to0.
Any write access toOBCR(15-8) while OBSYS issetwillcause theerror flagIOBA intheError Interrupt
Register tobeset.
Inthiscase theoutput buffer willnotbechanged.
Figure 26-192 andTable 26-165 illustrate thisregister.
Figure 26-192. Output Buffer Command Mask Register (OBCR) [offset_CC =714h]
31 23 22 16
Reserved OBRH
R-0 R/W-0
15 14 10 9 8 7 6 0
OBSYS Reserved REQ VIEW Rsvd OBRS
R-0 R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; *These bitscanbeupdated inDEFAULT_CONFIG orCONFIG state
only
Table 26-165. Output Buffer Command Mask Register (OBCR) Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22-16 OBRH 0-7Fh Output buffer request host. Number ofmessage buffer currently accessible bytheHost via
RDHS[1..3], MBS, andRDDS[1..64]. Bywriting VIEW to1OBF Shadow andOBF host are
swapped andthetransferred message buffer isaccessible bythehost.
15 OBSYS Output buffer shadow busy. Setto1after setting bitREQ. When thetransfer between the
message RAM andOBF shadow hascompleted, OBSYS issetback to0.
0 Notransfer isinprogress.
1 Transfer between message RAM andOBF shadow isinprogress.
14-10 Reserved 0 Reads return 0.Writes have noeffect.
9 REQ Request message RAM Transfer. Requests transfer ofmessage buffer addressed byOBRS from
message RAM toOBF shadow. Only writable while OBSYS =0.
0 Norequest.
1 Transfer toOBF shadow isrequested.
8 VIEW View shadow buffer. Toggles between OBF shadow andOBF host. Only writable while OBSYS =
0.
0 Noaction.
1 Swap OBF shadow andOBF.

<!-- Page 1416 -->

FlexRay Module Registers www.ti.com
1416 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedFlexRay ModuleTable 26-165. Output Buffer Command Mask Register (OBCR) Field Descriptions (continued)
Bit Field Value Description
7 Reserved 0 Reads return 0.Writes have noeffect.
6-0 OBRS 0-7Fh Output buffer request shadow. Number ofsource message buffer tobetransferred from the
message RAM toOBF shadow. Ifthenumber ofthefirstmessage buffer ofthereceive FIFO is
written tothisregister, themessage handler transfers themessage buffer addressed bytheGET
Index register (GIDX) toOBF shadow.