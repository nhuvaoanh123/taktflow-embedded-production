# Direct Memory Access Controller (DMA) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 696-792

---


<!-- Page 696 -->

696 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleChapter 20
SPNU563A -March 2018
Direct Memory Access Controller (DMA) Module
This chapter describes thedirect memory access (DMA) controller.
Topic ........................................................................................................................... Page
20.1 Overview ......................................................................................................... 697
20.2 Module Operation ............................................................................................. 699
20.3 Control Registers andControl Packets ............................................................... 721

<!-- Page 697 -->

www.ti.com Overview
697 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.1 Overview
The DMA controller isused totransfer data between twolocations inthememory map inthebackground
ofCPU operations. Typically, theDMA isused to:
*Transfer blocks ofdata between external andinternal data memories
*Restructure portions ofinternal data memory
*Continually service aperipheral
*Page program sections tointernal program memory
Since theDMA hastwomaster ports, theselection fortheport should bemade using thetable mentioning
theport tobeused foreach address region.
20.1.1 Main Features
*CPU independent data transfer
*Two master ports -PortA andPortB (each 64-bits wide) thatinterface with theMicrocontroller's Bus
Matrix System.
*Support forconcurrent transfers onuptotwodifferent channels
*FIFO buffer (4entries deep andeach 64-bits wide)
*Channel control information isstored inRAM protected byECC
*Multiple logical channels with individual enable (refer tothedata manual forthenumber ofchannels on
your device)
*Channel chaining capability
*48peripheral DMA requests
*Hardware andSoftware DMA requests
*8-,16-, 32-, or64-bit transactions supported
*Multiple addressing modes forsource/destination (fixed, increment, offset)
*Auto-initiation
*Power-management mode
*Memory Protection fortheaddress range DMA canaccess with multiple configurable memory regions
(refer datasheet fornumber ofmemory regions onyour device)
20.1.1.1 Block Diagram
Figure 20-1 gives atopview oftheDMA internal architecture. DMA data read andwrite access happen
through either Port AorB.Both FIFO buffers areeach 4levels deep and64-bits wide thus allowing a
maximum of32bytes tobebuffered inside theDMA perchannel. DMA requests gointotheDMA thatcan
trigger DMA transfers. Five interrupt request lines gooutoftheDMA tosignal thatacertain transfer status
isreached. Register banks hold thememory mapped DMA configuration registers. Local RAM consists of
DMA control packets andissecured byECC. Alltheprogramming /configuration oftheDMA controller is
done viathePeripheral bus.

<!-- Page 698 -->

Event manager (prioritization, 
arbitration)
DMA req sync 
and polarityFIFO A channel 
processingFIFO B channel 
processing
Port ArbiterControl Packet 
Access Arbiter
Control 
Regs
Control 
Packet 
RAMInterrupt 
Manager
Port A Port B BTC, FTC, BER, 
LFS, HBC, MPV 
interruptsCPU I/FErrors (Single, 
Double Bit Errors)
Hardware Events
Overview www.ti.com
698 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-1. DMA Block Diagram

<!-- Page 699 -->

www.ti.com Overview
699 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.1.2 System Resources Mapping
Table 20-1 shows how thesystem resources aremapped toeither ofthetwoDMA ports. Inorder to
properly transfer data from oneresource toanother, theapplication must setup thePARx register
according toTable 20-1.
Table 20-1. DMA Ports toSystem Resources Mapping
DMA Ports System Resources
Port A*L2Flash
*L2SRAM
*EMIF
Port B*Allperipherals, thatis,MibSPI registers, DCAN registers
*Allperipheral memories, thatis,MibSPI RAM, DCAN RAM
*Example 1:Totransfer data from L2Flash, L2SRAM, orEMIF toanyperipheral registers orperipheral
memories, write 0x1(Port Aread, Port Bwrite )totherespective channels inthePARx registers
*Example 2:Totransfer data from anyperipheral registers orperipheral memories toL2SRAM or
EMIF, write 0x0(Port Bread, Port Awrite )totherespective channels inthePARx registers
*Example 3:Totransfer data from L2Flash toL2SRAM, write 0x2(Port A)totherespective channels
inthePARx registers
*Example 4:Totransfer data from peripherals toanother peripherals, write 0x3(Port B)tothe
respective channels inthePARx registers.
20.2 Module Operation
The DMA acts asanindependent master intheplatform architecture. DMA willattempt toexecute upto
twochannels atthesame time tomaximize system throughput. Each channel canbeconfigured toutilize
either Port AorBorboth fortheread andwrite accesses while storing thedata inoneoftheFIFOs.
Choice ofPort AorPort Bforacertain channel depends ontheaddresses chosen forthetransfer and
should bemade byreferring toTable 20-1.AllDMA memory andregister accesses areperformed inuser
mode. IftheDMA writes toregisters which areonly accessible inprivileged mode, thewrite willnotbe
performed.
The DMA registers anditslocal RAM canonly beaccessed inprivilege mode. Therefore, itisnotpossible
fortheDMA toreprogram itself.
Inorder tofurther explain DMA operation, some terms aredescribed below:
*Arbitration -Achannel may gettemporarily suspended inorder toservice ahigher priority channel or
when thechannel isdisabled onthefly.The channel issaid tohave been "arbitrated"
*Arbitration Boundary -Each time achannel finishes achunk oftransfer which canbeamaximum of32
bytes, itissaid tohave reached anarbitration boundary. The FIFO isempty atanarbitration boundary.
The DMA willutilize thisboundary tore-prioritize channels. Within anarbitration boundary, transfers
cannever beinterrupted.

<!-- Page 700 -->

Block
Element 1 Element 2Frame 1Element Count = 2
Element 3 Element 4Frame 2
Element 5 Element 6Frame 3
Element 7 Element 8Frame 4Frame count = 4
DMAREQTrigger Source = block transfer triggered by DMA request
Block
Element 1 Element 2Frame 1Element Count = 2
Element 3 Element 4Frame 2
Element 5 Element 6Frame 3
Element 7 Element 8Frame 4Frame count = 4
DMAREQ DMAREQ DMAREQ DMAREQTrigger Source = frame transfer triggered by DMA request
Module Operation www.ti.com
700 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.1 Memory Space
The DMA controller makes nodistinction between program memory anddata memory. The DMA
controller cantransfer toandfrom anyspace within the4gigabyte physical address map, byprogramming
theabsolute address forthesource anddestination inthecontrol packet. Control packets store the
transfer information such assource address, destination address, transfer count andcontrol attributes for
each channel.
20.2.2 DMA Data Access
The DMA controller refers todata inthree levels ofgranularity:
*Element: Depending ontheprogrammed data type, an8-bit, 16-bit, 32-bit, ora64-bit value. The type
canbeindividually selected forthesource (read) anddestination (write). See Figure 20-2 and
Figure 20-3 foranexample oftheuseofelements. Anelement transfer cannot beinterrupted.
*Frame: One ormore elements tobetransferred asaunit. Aframe transfer canbeinterrupted between
element transfers. See Figure 20-2 foranexample. Use aframe size ofoneandframe transfer trigger
source fortransfers ofoneelement perrequest.
*Block: One ormore frames tobetransferred asaunit. Each channel cantransfer oneblock ofdata
(once ormultiple times). See Figure 20-3 foranexample.
Figure 20-2. Example ofaDMA Transfer Using Frame Trigger Source
Figure 20-3. Example ofaDMA Transfer Using Block Trigger Source

<!-- Page 701 -->

www.ti.com Module Operation
701 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.3 Addressing Modes
There arethree addressing modes supported bytheDMA controller thatcanbesetup independent forthe
source andthedestination address:
*Constant --source and/or destination addresses donotchange.
*Post incremented --source and/or destination address arepost-incremented bytheelement size.
*Indexed --source and/or destination address ispost-incremented asdefined intheElement Index
Offset Register (Section 20.3.2.5 )andtheFrame Index Offset Register (Section 20.3.2.6 ).
Anunaligned address with respect totheelement size isnotsupported.
20.2.4 DMA Channel Control Packets
Corresponding toeach logical channel isacontrol packet thatismapped infixed numerical order. For
example, control packet 0stores channel information forchannel 0.The DMA requests canbemapped to
theindividual channels asdescribed inSection 20.2.7 .The mapping scheme between DMA requests and
channels isshown inFigure 20-4.Each control packet contains nine fields. The firstsixfields compose the
primary control packet andareprogrammable during DMA setup. The lastthree fields compose working
control packet andareonly readable bytheCPU. The working control packets areused tosupport auto-
initiation andprioritization ofchannels.. The organization ofcontrol packets isshown inFigure 20-5.
The primary control packet contains channel information such assource address, destination address,
transfer count, element/frame offset value andchannel configuration. Source address, destination address
andtransfer count also have their respective working images. The three fields ofworking images compose
aworking control packet andarenotaccessible totheCPU inwrite access.
The firsttime aDMA channel isselected foratransaction, thefollowing process occurs:
1.The primary control packet isfirstread bytheDMA state machine.
2.Once thechannel isarbitrated, thecurrent source address, destination address andtransfer count are
then copied totheir respective working images.
3.When thechannel isserviced again bytheDMA, thestate machine willread both theprimary control
packet andtheworking control packet tocontinue theDMA transaction until theendofanentire block
transfer.
When thesame channel isrequested again, thestate machine willstart again byreading only theprimary
control packet andthen continue thesame process described above. The user software need notsetup
control packets again because thecontents oftheprimary control packet were never lost. The working
images ofthecontrol packets arereducing thesoftware overhead andinteraction with theDMA module to
aminimum.
NOTE: Changing thecontents ofachannel control packet willclear thecorresponding pending bit
(Section 20.3.1.2 )ifthechannel hasapending status. Ifthecontrol packet ofanactive
channel (asindicated inSection 20.3.1.3 )ischanged, then thechannel willstop immediately
atanarbitration boundary. When thesame channel istriggered again, itwillbegin with the
new control packet information.

<!-- Page 702 -->

Frame Offset ValueBase + 0x00
0x20 Initial Source Address
Channel ConfigurationInitial Destination Address
Element Offset ValueInitial Transfer Count
Frame Offset Value
Initial Source Address
Channel Configuration
Current Source AddressInitial Destination Address
Element Offset Value
Current Destination AddressInitial Transfer Count
Frame Offset Value
Current Transfer Count0x30
0x800
Current Source Address
Current Source AddressReserved Reserved Reserved
Current Destination Address
Current Destination AddressCurrent Transfer Count
Current Transfer Count0x1F00x1E0
0x810
0x8F0Base + 0xXXXC
Reserved0x10Primary CP0
Primary CP1
Primary CPnn
Working CP0
Working CP1
Working CPnnBase + 0XXX0 Base + 0xXXX4 Base + 0xXXX8
Initial Source Address
Channel ConfigurationInitial Destination Address Initial Transfer Count
Element Offset Value
}}}}}}
Y...DMAREQ(0)
DMAREQ(1)
DMAREQ(2)
DMAREQ(63)Ch 0
Control Packet 0
Y...CH1ASI[5:0]
DMAREQ(0)
DMAREQ(1)
DMAREQ(2)
DMAREQ(63)CH0ASI[5:0]
Control Packet 1
YY.
Control Packet 31Ch 1
Y...CH31ASI[5:0]
DMAREQ(0)
DMAREQ(1)
DMAREQ(2)
DMAREQ(63)Ch 31Y...
Module Operation www.ti.com
702 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-4. DMA Request Mapping andControl Packet Organization
Figure 20-5. Control Packet Organization andMemory Map

<!-- Page 703 -->

T = E * E * Fsz rsz tc tc
www.ti.com Module Operation
703 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.4.1 Initial Source Address
This field stores theabsolute 32-bit source address oftheDMA transfer.
20.2.4.2 Initial Destination Address
This field stores theabsolute 32-bit destination address oftheDMA transfer.
20.2.4.3 Initial Transfer Count
The transfer count field iscomposed oftwoparts. The frame transfer count value andtheelement transfer
count value. Each count value is13-bits wide. AsaSingle Block transfer maximum of512Mbytes ofdata
canbetransferred. Element count andframe count areprogrammed according tothesource data
structure.
The total transfer size iscalculated as:
(26)
where
Tsz=Total Transfer Size
Ersz=Read Element Size
Etc=Element Transfer Count
Ftc=Frame Transfer Count
NOTE: Azero element count with anon-zero frame count oranon-zero element count with azero
frame count areallconsidered aszero total transfer count. NoDMA transaction isinitiated
with anyofthecounters setto0.
20.2.4.4 Channel Configuration Word
The channel configuration defines thefollowing individual parameters:
*Read element size
*Write element size
*Trigger type (frame orblock)
*Addressing mode forsource
*Addressing mode fordestination
*Auto-initiation mode
*Next control packet tobetriggered atcontrol packet finish (Channel Chaining)
20.2.4.5 Element/Frame Offset Value
There are4offset values thatallow thecreation ofdifferent types ofbuffers inRAM andaddress registers
inastructured manner: anelement offset value forsource anddestination andaframe offset value for
source anddestination.
The element offset value forsource and/or destination defines theoffset tobeadded after each element
transfer tothesource and/or destination address. The frame offset value forsource and/or destination
defines theoffset tobeadded tothesource and/or destination address after theelement count reaches
zero. The element andframe offset values must bedefined interms ofthenumber ofbytes ofoffset. The
DMA controller does notadjust theelement/frame index number according totheelement size. Anindex
of2means increment theaddress by2andnotby16when theelement size is64bits.
20.2.4.6 Current Source Address
The current source address field contains thecurrent working source address during aDMA transaction.
The current source address isincremented during post increment addressing mode orindexing mode.

<!-- Page 704 -->

0x0
0x10
0x20
0x30E1
E2
E3E5
E6
E7
E8E9
E10
E11
E12E13
E14
E15
E16 E4
Element Index = 16
Frame Index = 4
Element Size = 16 bit
Element Count = 4
Frame Count = 4This example can be applied to either source or
destination indexing and assumes the following setup.f1 f2 f3 f4
E1
E4E7
E2E5
E8E3
E6Source Destination
Dest. Element Index = 1
Dest. Frame Index = 0
Dest. Element Index = 0
Dest. Frame Index = 1E1/2 E3/4 E5/6
E1 E2 E3 E4
E5 E6 E7 E8E1 E3 E5
E2 E4 E6 E8E7
Dest. Element Index = 1
Dest. Frame Index = 2Dest. Element Index = 4
Dest. Frame Index = 10x00
0x04
0x08
0x0C
0x0
0x40x0
0x4
0x0
0x40x0 E7/8E1/3/5/7 E2/4/6/8
Source Element Index = 12
Source Frame Index = 1
The example assumes the following setup.
Read Element Size = 8 bit
Write Element Size = 8 bit
Element Count = 2
Frame Count = 4f2 f3 f4 f1
Module Operation www.ti.com
704 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.4.7 Current Destination Address
The current destination address field contains thecurrent working destination address during aDMA
transaction. The current destination address isincremented during post-increment addressing mode or
indexing mode.
20.2.4.8 Current Transfer Count
The current transfer count stores theremaining number ofelements tobetransferred inablock. Itis
decremented byoneforeach element read from thesource location.
Figure 20-6,Figure 20-7,andFigure 20-8 show some examples ofDMA transfers.
Figure 20-6. DMA Transfer Example 1
Figure 20-7. DMA Indexing Example 1

<!-- Page 705 -->

The above figure illustrates that by default Lower the channel number, higher the Priority.Priority Queue
Ch0 Ch2 Ch3 Ch4...Control Packet 0
Control Packet 1
Control Packet 2
Control Packet 3
Control Packet 4
Control Packet 5
Control Packet 6
Control Packet 7
Control Packet 8
Control Packet 9
Control Packet 10
Control Packet 11
Control Packet 12
Control Packet 13
Control Packet 14
Control Packet 15
Triggered ChannelsORDER
OF
PRIORITYHigh
Low
0x0
0x40E1
E2E13 E4
E5E16 E7
E8E19 E10
E11E22
0x800x20
0x60
Element Index = 64
Frame Index = 4
Element Size = 32 bit
Element Count = 3
Frame Count = 8E14 E17 E20 E23
E3 E6 E9 E12 E15 E18 E21 E23
This example can be applied to either source or
destination indexing and assumes the following setup.
www.ti.com Module Operation
705 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-8. DMA Indexing Example 2
20.2.5 Priority Queue
User canassign channels intopriority queues tofacilitate request handling during arbitration. The port
hastwopriority queues: ahigh andalowpriority queue. Each queue canbeconfigured tofollow afixed or
rotating priority scheme. Fixed priority issuch thatthelower thechannel number (Figure 20-9),thehigher
itspriority. Rotating priority isbased onaround-robin scheme. Initially, thepriority listissorted according
tothefixed priority scheme. Channels assigned tothehigh priority queue arealways serviced first
according totheselected priority scheme before channels inthelowpriority queue areserviced. Table 20-
2describes how arbitration isperformed according todifferent priority schemes.
NOTE: Since theDMA controller provides thecapability tomap anyoneofthehardware DMA
request lines toanychannel, thenumerical order ofthehardware DMA request does not
imply anypriority. The priority ofeach hardware DMA request isprogrammed and
determined bysoftware.
Figure 20-9. Fixed Priority Scheme

<!-- Page 706 -->

CH2
CH4
CH5
CH7CH1
CH3
CH6CH1 CH3CH7CH9CH13CH15
CH2 CH4CH6CH8CH10CH14in queue i n use
Pending triggered Start/Stop servingCH2
CH4
CH5
CH7CH1
CH3
CH6CH1 CH3CH7CH9CH13CH15
CH2 CH4CH6CH8CH10CH14in queue i n use
CH2
CH4
CH5
CH7CH1
CH3
CH6CH1 CH3CH7CH9CH13CH15
CH2 CH4CH6CH8CH10CH14High
queue
Low
queueFixed Rotation
in queue in use
in queue in use
Module Operation www.ti.com
706 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-2. Arbitration According toPriority Queues andPriority Schemes
Queue Priority Scheme Remark
High priorityFixedChannels areserviced inanascending order according tothechannel number. The
lower thechannel number, thehigher thepriority. Achannel willbearbitrated out
whenever there isahigher pending channel. Otherwise achannel iscompletely
serviced until itstransfer count reaches zero before thenext highest pending channel is
serviced. When there isnopending channels leftinhigh queue then theDMA switches
toservice lowqueue channels.
RotatingChannels arearbitrated byusing theround-robin scheme. Arbitration isperformed
when theFIFO isempty. When there arenopending channels leftinhigh queue then
theDMA switches toservice lowqueue channels.
Low priorityFixedChannels areserviced inanascending order according tothechannel number. The
lower thechannel number thehigher thepriority. Achannel willbearbitrated out
whenever there isahigher-priority pending channel. Otherwise achannel iscompletely
serviced until itstransfer count reaches zero, before thenext highest pending channel
isserviced. Ifthere isapending channel inthehigh-priority queue while DMA is
servicing alowqueue channel then DMA willswitch back toservice high queue
channel after anarbitration boundary.
RotatingChannels arearbitrated byusing round-robin scheme. Arbitration isperformed when
theFIFO isempty.
ASimple Priority Queues example inboth Fixed andRotation Scheme isshown inFigure 20-10 .
Figure 20-10. Example ofPriority Queues
Foroptimal system performance, thehigh priority channels should beputinfixed arbitration scheme and
lowpriority channels intherotating priority scheme asillustrated inFigure 20-11 .

<!-- Page 707 -->

Port B Priority Queue
high
lowCh0 Ch2 Ch3 Ch4
Ch8, Ch12fixed priority
rotational priorityControl Packet 0
Control Packet 1
Control Packet 2
Control Packet 3
Control Packet 4
Control Packet 5
Control Packet 6
Control Packet 7
Control Packet 8
Control Packet 9
Control Packet 10
Control Packet 11
Control Packet 12
Control Packet 13
Control Packet 14
Control Packet 15
1 The above figure illustrates the channel assignments in a system with 16 channels.
This approach can be scaled dependent on the total channels available.
www.ti.com Module Operation
707 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-11. Example Channel Assignments
20.2.6 Data Packing andUnpacking
The DMA controller automatically performs thenecessary data packing andunpacking when theread
element size differs from thewrite element size. Data packing isrequired when theread element size is
smaller than thewrite element size; data unpacking isrequired when theread element size islarger than
thewrite element size. When theread element size isequal tothewrite element size, nopacking is
performed during read, norisanyunpacking performed during write.
Figure 20-12 shows anexample ofdata unpacking inwhich theDMA isused totransfer 128transmit data
elements totheMibSPI FIFO buffer. Inthisexample, data unpacking isrequired because theread
element size is64while thewrite element size is16.The DMA firstperforms an64-bit read from the
source intoitsFIFO buffer. After the64-bit data isread intotheDMA FIFO buffer, itmust unpack thedata
intofour 16-bit data elements before writing outtothedestination. Therefore theDMA would need to
perform four 16bitwrite operations tothedestination.
NOTE: Examples areshown forbig-endian scheme.
NOTE: Intheexample inFigure 20-12 ,totransmit data atthelower bitsoftheMibSPI, bits15:0, the
destination address should beincremented byafactor of2.
NOTE: 1)The element Count (Section 20.3.2.3 )refers only totheread element.
2)Data unpacking does notrequire theDMA request. Once theDMA request isreceived,
data from Source ismoved intoFIFO andunpacking happens until theFIFO isempty.
3)DMA assumes thedestination isalways ready andwillperform write immediately. Incase
ofdata unpacking andConstant Addressing Mode write (Section 20.3.2.4 (1-0)=0)the
destination data willbeoverwritten bynext data ornext data might beskipped incase the
destination hasoverflow protection (forexample, SCITD register). User should configure
DMA toavoid data unpacking iftheDestination isconfigured asConstant Addressing Mode
write toavoid data loss.

<!-- Page 708 -->

Transmit buf 0
Transmit buf 1
Transmit buf 127
Receive buf 0
Receive buf 1
Receive buf 127E0
E1
E127Control 0
Control 1
Control 127
Status 0
Status 1
Status 127
MIBSPI FIFO organization64-bit memory organization0x0
0x4
0x400E3 E2 E1 E0 0x00x863 0 31 1615 0
E124 E125 E126 E127
E4 E5 E6 E7
In this example, initialization of the MIBSPI FIFO is illust rated and assumes the following setup:
Read Element Size = 64 bit
Write Element Size = 16 bit
Element Count = 32
Frame Count = 1
Source Element Index = n/a, use post increment addressing mo de
Source Frame Index = n/a, use post increment addressing mode
Destination Element Index = 4
Destination Frame Index = 0
Module Operation www.ti.com
708 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-12. Example ofDMA Data Unpacking
When theread element size issmaller than thewrite element size, theDMA controller needs toperform
data packing. The number ofelements topack isequal totheratio between thewrite element size and
read element size. Intheexample inFigure 20-13 ,theread element size is16bitsandthewrite element
size is64bits. The DMA controller would firstpack thefirstfour elements byperforming four consecutive
16-bit read accesses ofE0,E1,E2,andE3intothefirstword oftheDMA's internal FIFO. The DMA
controller would then perform onesingle 64-bit write operation totransfer thedata tothe64-bit destination
memory.
Normally, theDMA controller carries outbustransactions onthebusaccording totheelement size. For
example, theDMA controller would perform a16-bit read transaction iftheread element size is
programmed as16bits, oran8-bit write transaction ifthewrite element size isprogrammed as8bit.The
exception iswhen thetotal transfer size isasdefined inEquation 26isnotamultiple ofthewrite element
size.

<!-- Page 709 -->

Transmit buf 0
Transmit buf 1
Transmit buf 127
Receive buf 0
Receive buf 1
Receive buf 127E0
E1
E127Control 0
Control 1
Control 127
Status 0
Status 1
Status 127
MIBSPI FIFO organization0x0
0x4
0x40031 1615 0
64-bit memory organizationE1 E0 E3 E2 0x00x863 0
E126 E127 E124 E125
E6 E7 E4 E5
In this example, a read of the MIBSPI FIFO is illustrated and a ssumes the following setup:
Read Element Size = 16 bit
Write Element Size = 64 bit
Element Count = 128
Frame Count = 1
Source Element Index = 4
Source Frame Index = 0
Destination Element Index = n/a, use post increment address ing mode
Destination Frame Index = n/a, use post increment addressin g mode
www.ti.com Module Operation
709 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-13. Example ofDMA Data Packing
Forexample, iftheread element size is8bits, theelement transfer count isequal to9,andthewrite
element size is64bit.The DMA controller would firstperform eight 8-bit read transactions from the
source. Itwould then perform a64-bit write tothedestination. When thesame channel wins arbitration
again, theDMA controller would firstperform one8-bit read from thesource, followed byone8-bit write to
thedestination, even though thewrite element size is64bit.
NOTE: Since peripherals areslower, itisadvised tousedata packing feature with caution for
reading data from peripherals. Improper usemight delay servicing other pending DMA
channels.

<!-- Page 710 -->

Module Operation www.ti.com
710 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.7 DMA Request
There arethree ways tostart aDMA transfer:
*Software request: The transfer willbetriggered bywriting toSWChannel Enable SetandStatus
Register (Section 20.3.1.7 ).The software request cantrigger either ablock oraframe transfer
depending onthesetting oftheTTYPE bitintheChannel Control Register (Section 20.3.2.4 ).
*Hardware request :The DMA controller canhandle upto48DMA Request lines. Ahardware request
cantrigger either aframe orablock transfer depending onthesetting oftheTTYPE bitintheChannel
Control Register (Section 20.3.2.4 ).
*Triggered byother control packet: When acontrol packet finishes theprogrammed number of
transfers itcantrigger another channel toinitiate itstransfers.
Each time aDMA request ismade, either oneframe transfer oroneblock transfer canbechosen. An
active DMA request signal willtrigger aDMA transaction.
The DMA controller hasatwo-level buffer tocapture HWrequests perchannel. When aHWrequest is
generated andthechannel isenabled, thecorresponding bitintheDMA Status Register
(Section 20.3.1.3 )isset.The pending register acts asafirst-level buffer. Typically, aperipheral acting asa
source ofatransfer could initiate another request after itsdata registers have been read outbyDMA,
even though thatdata hasnotbeen completely transferred tothedestination. Ifasecond HWrequest is
generated bytheperipheral, theDMA controller hasanextra request buffer tocapture thissecond request
andservice itafter thefirstrequest iscomplete.
NOTE: The DMA cannot capture more than tworequests atthesame time. Additional requests are
ignored until atleast onepending request iscompletely processed.
The DMA controller also supports amixofhardware andsoftware requests onthesame channel. Note
thatsuch interchangeable usage may result intoanoutofsync forDMA channel andperipheral. The
application needs tobecareful astheDMA does nothave abuilt-in mechanism toprotect against thisloss
ofsynchronization.
Ifasoftware request isgenerated, thecorresponding bitintheChannel Pending Register
(Section 20.3.1.2 )issetaccordingly. Ifthepending request isnotcompletely serviced bytheDMA anda
hardware request isgenerated byaperipheral onto thesame channel, theDMA willcapture and
recognize thishardware request intoitsrequest buffer.
NOTE: The DMA controller cannot recognize twosoftware requests onthesame channel ifthefirst
software request isstillpending. Ifsuch arequest occurs, theDMA willdiscard it.Therefore,
theuser software should check thepending register before issuing anew software request.
The DMA module onthismicrocontroller has32channels andupto48hardware DMA requests. The
module contains DREQASIx registers which areused tomap theDMA requests totheDMA channels. By
default, channel 0ismapped torequest 0,channel 1torequest 1,andsoon.
Some DMA requests have multiple sources, seeTable 20-3.The application must ensure thatonly oneof
these DMA request sources isenabled atanytime.
(1)SPI1, SPI2, SPI3, SPI4, SPI5 receive incompatibility mode
(2)SPI1, SPI2, SPI3, SPI4, SPI5 transmit incompatibility modeTable 20-3. DMA Request Line Connection
Modules DMA Request Sources DMA Request
MIBSPI1 MIBSPI1[1](1)DMAREQ[0]
MIBSPI1 MIBSPI1[0](2)DMAREQ[1]
MIBSPI2 MIBSPI2[1](1)DMAREQ[2]
MIBSPI2 MIBSPI2[0](2)DMAREQ[3]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[2] /MIBSPI3[2] /DCAN2 IF3 DMAREQ[4]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[3] /MIBSPI3[3] /DCAN2 IF2 DMAREQ[5]
DCAN1 /MIBSPI5 DCAN1 IF2/MIBSPI5[2] DMAREQ[6]
MIBADC1 /MIBSPI5 MIBADC1 event /MIBSPI5[3] DMAREQ[7]

<!-- Page 711 -->

www.ti.com Module Operation
711 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-3. DMA Request Line Connection (continued)
Modules DMA Request Sources DMA Request
MIBSPI1 /MIBSPI3 /DCAN1 MIBSPI1[4] /MIBSPI3[4] /DCAN1 IF1 DMAREQ[8]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[5] /MIBSPI3[5] /DCAN2 IF1 DMAREQ[9]
MIBADC1 /I2C/MIBSPI5 MIBADC1 G1/I2Creceive /MIBSPI5[4] DMAREQ[10]
MIBADC1 /I2C/MIBSPI5 MIBADC1 G2/I2Ctransmit /MIBSPI5[5] DMAREQ[11]
RTI/MIBSPI1 /MIBSPI3 RTIDMAREQ0 /MIBSPI1[6] /MIBSPI3[6] DMAREQ[12]
RTI/MIBSPI1 /MIBSPI3 RTIDMAREQ1 /MIBSPI1[7] /MIBSPI3[7] DMAREQ[13]
MIBSPI3 /MibADC2 /MIBSPI5 MIBSPI3[1](1)/MibADC2 event /MIBSPI5[6] DMAREQ[14]
MIBSPI3 /MIBSPI5 MIBSPI3[0](2)/MIBSPI5[7] DMAREQ[15]
MIBSPI1 /MIBSPI3 /DCAN1 /MibADC2 MIBSPI1[8] /MIBSPI3[8] /DCAN1 IF3/MibADC2 G1 DMAREQ[16]
MIBSPI1 /MIBSPI3 /DCAN3 /MibADC2 MIBSPI1[9] /MIBSPI3[9] /DCAN3 IF1/MibADC2 G2 DMAREQ[17]
RTI/MIBSPI5 RTIDMAREQ2 /MIBSPI5[8] DMAREQ[18]
RTI/MIBSPI5 RTIDMAREQ3 /MIBSPI5[9] DMAREQ[19]
NHET1 /NHET2 /DCAN3 NHET1 DMAREQ[4] /NHET2 DMAREQ[4] /DCAN3 IF2 DMAREQ[20]
NHET1 /NHET2 /DCAN3 NHET1 DMAREQ[5] /NHET2 DMAREQ[5] /DCAN3 IF3 DMAREQ[21]
MIBSPI1 /MIBSPI3 /MIBSPI5 MIBSPI1[10] /MIBSPI3[10] /MIBSPI5[10] DMAREQ[22]
MIBSPI1 /MIBSPI3 /MIBSPI5 MIBSPI1[11] /MIBSPI3[11] /MIBSPI5[11] DMAREQ[23]
NHET1 /NHET2 /MIBSPI4 /MIBSPI5 NHET1 DMAREQ[6] /NHET2 DMAREQ[6] /MIBSPI4[1](1)/MIBSPI5[12] DMAREQ[24]
NHET1 /NHET2 /MIBSPI4 /MIBSPI5 NHET1 DMAREQ[7] /NHET2 DMAREQ[7] /MIBSPI4[0](2)/MIBSPI5[13] DMAREQ[25]
CRC 1/MIBSPI1 /MIBSPI3 CRC 1DMAREQ[0] /MIBSPI1[12] /MIBSPI3[12] DMAREQ[26]
CRC 1/MIBSPI1 /MIBSPI3 CRC 1DMAREQ[1] /MIBSPI1[13] /MIBSPI3[13] DMAREQ[27]
LIN1 /MIBSPI5 LIN1 receive /MIBSPI5[14] DMAREQ[28]
LIN1 /MIBSPI5 LIN1 transmit /MIBSPI5[15] DMAREQ[29]
MIBSPI1 /MIBSPI3 /SCI3 /MIBSPI5 MIBSPI1[14] /MIBSPI3[14] /SCI3 receive /MIBSPI5[1](1)DMAREQ[30]
MIBSPI1 /MIBSPI3 /SCI3 /MIBSPI5 MIBSPI1[15] /MIBSPI3[15] /SCI3 transmit /MIBSPI5[0](2)DMAREQ[31]
I2C2 /ePWM1 /MIBSPI2 /MIBSPI4 /
GIOAI2C2 receive /ePWM1_SOCA /MIBSPI2[2] /MIBSPI4[2] /GIOA[0] DMAREQ[32]
I2C2 /ePWM 1/MIBSPI2 /MIBSPI4 /
GIOAI2C2 transmit /ePWM1_SOCB /MIBSPI2[3] /MIBSPI4[3] /GIOA[1] DMAREQ[33]
ePWM2 /MIBSPI2 /MIBSPI4 /GIOA ePWM2_SOCA /MIBSPI2[4] /MIBSPI4[4] /GIOA[2] DMAREQ[34]
ePWM2 /MIBSPI2 /MIBSPI4 /GIOA ePWM2_SOCB /MIBSPI2[5] /MIBSPI4[5] /GIOA[3] DMAREQ[35]
ePWM3 /MIBSPI2 /MIBSPI4 /GIOA ePWM3_SOCA /MIBSPI2[6] /MIBSPI4[6] /GIOA[4] DMAREQ[36]
ePWM3 /MIBSPI2 /MIBSPI4 /GIOA ePWM3_SOCB /MIBSPI2[7] /MIBSPI4[7] /GIOA[5] DMAREQ[37]
CRC2 /ePWM4 /MIBSPI2 /MIBSPI4 /
GIOACRC2 DMAREQ[0] /ePWM4_SOCA /MIBSPI2[8] /MIBSPI4[8] /GIOA[6] DMAREQ[38]
CRC2 /ePWM4 /MIBSPI2 /MIBSPI4
/GIOACRC2 DMAREQ[1] /ePWM4_SOCB /MIBSPI2[9] /MIBSPI4[9] /GIOA[7] DMAREQ[39]
LIN2 /ePWM5 /MIBSPI2 /MIBSPI4 /
GIOBLIN2 receive /ePWM5_SOCA /MIBSPI2[10] /MIBSPI4[10] /GIOB[0] DMAREQ[40]
LIN2 /ePWM5 /MIBSPI2 /MIBSPI4 /
GIOBLIN2 transmit /ePWM5_SOCB /MIBSPI2[11] /MIBSPI4[11] /GIOB[1] DMAREQ[41]
SCI4 /ePWM6 /MIBSPI2 /MIBSPI4 /
GIOBSCI4 receive /ePWM6_SOCA /MIBSPI2[12] /MIBSPI4[12] /GIOB[2] DMAREQ[42]
SCI4 /ePWM6 /MIBSPI2 /MIBSPI4 /
GIOBSCI4 transmit /ePWM6_SOCB /MIBSPI2[13] /MIBSPI4[13] /GIOB[3] DMAREQ[43]
ePWM7 /MIBSPI2 /MIBSPI4 /GIOB ePWM7_SOCA /MIBSPI2[14] /MIBSPI4[14] /GIOB[4] DMAREQ[44]
ePWM7 /MIBSPI2 /MIBSPI4 /GIOB /
DCAN4ePWM7_SOCB /MIBSPI2[15] /MIBSPI4[15] /GIOB[5] /DCAN4 IF1 DMAREQ[45]
GIOB /DCAN4 GIOB[6] /DCAN4_IF2 DMAREQ[46]
GIOB /DCAN4 GIOB[7] /DCAN4_IF3 DMAREQ[47]

<!-- Page 712 -->

Module Operation www.ti.com
712 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.8 Auto-Initiation
When Auto-initiation Mode (AIM) bitofChannel Control Register (Section 20.3.2.4 )isenabled fora
channel andthechannel istriggered byasoftware request forablock transfer, thechannel willrestart
again using thesame channel information stored attherespective control packet after oneblock transfer
iscompleted. Inthecase ofHardware Request thechannel needs toberetriggered each time after a
block iscomplete even ifauto-initiation isenabled.
20.2.9 Interrupts
Each channel canbeconfigured togenerate interrupts onseveral transfer conditions:
*Frame transfer complete (FTC) interrupt: aninterrupt isissued after thelastelement ofaframe has
been transferred.
*Last frame transfer started (LFS) interrupt: aninterrupt isissued before thefirstelement ofthelast
frame ofablock transfer hasstarted.
*First halfofblock complete (HBC) interrupt: aninterrupt isissued ifmore than halfoftheblock is
transferred.
-Ifthenumber offrames nisodd, then theHBC interrupt isgenerated attheendoftheframe when
(n+1) /2number offrames areleftintheblock.
-Ifthenumber offrames niseven, then theHBC interrupt isgenerated attheendoftheframe after
n/2number offrames areleftintheblock.
*Block transfer complete (BTC) interrupt: aninterrupt isissued after thelastelement ofthelastframe
hasbeen transferred.
*External imprecise error onread: aninterrupt canbeissued when abuserror (Illegal transaction with
okresponse) isdetected. The imprecise read error isconnected totheESM module.
*External imprecise error onwrite: aninterrupt canbeissued when abuserror (Illegal transaction with
okresponse) isdetected. The imprecise write error isconnected totheESM module.
*Memory Protection Unit error (MPU): aninterrupt isissued when theDMA detects thattheaccess falls
outside ofamemory region programmed intheMPU registers oftheDMA. The MPU interrupt is
connected totheESM module.
*Parity error (PAR): aninterrupt isissued when theDMA detects aparity error when reading oneofthe
control packets. The PAR interrupt isconnected totheESM module.
The DMA outputs 5interrupt lines forcontrol packet handling, aparity interrupt andamemory protection
interrupt (Figure 20-14 ).Each type oftransfer interrupt condition isgrouped together. Forexample, all
block-transfer complete interrupts thatarerouted toaport arecombined (ORed). The channel thatcaused
theinterrupt isgiven inthecorresponding interrupt channel offset register. Priority between interrupts
among thesame interrupt type isresolved byafixed priority scheme. Priority between different interrupt
types isresolved intheVector Interrupt Manager. Figure 20-15 explains theFrame Transfer Complete
Interrupt structure indetail.
NOTE: Each Channel Specific interrupts inDMA module arerouted towards Group AorBto
support twodifferent CPUs individually. Fordevices with Single CPU orDual CPU, where
both CPUs arerunning same code indelayed lock-step assafety feature:
Group A-Interrupts (FTC, LFS, HBC, andBTC) arerouted totheARM CPU.
Group B-Interrupts (FTC, LFS, HBC, andBTC) arenotrouted out.
User software should configure only Group Ainterrupts.

<!-- Page 713 -->

FTCAFrame Transfer
Complete Ch0
FTC0AB
FTC31ABFrame Transfer
Complete Ch31*□*□* *□*□* *□*□*
CHANNEL
SPECIFIC
INTERRUPT
PARITY
ERROR
MPU
ERRORERROR
SIGNALING
MODULE
(ESM)VECTOR
INTERRUPT
MODULE
(VIM)CPUFTCA
LFSA
HBCA
BTCA
PAR
MPUHigh
LowG
R
O
U
P
ADMA
S
C
R
DMA/DMM imprecise read error Group 1.5
DMA/DMM imprecise write error Group 1.13
www.ti.com Module Operation
713 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-14. DMA Interrupts
Figure 20-15. Detailed Interrupt Structure (Frame Transfer Complete Path)
This figure isapplicable fortheHBC, LFS, andBTC interrupt.

<!-- Page 714 -->

Module Operation www.ti.com
714 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.10 Debugging
The DMA supports four different behaviors insuspend mode. These behaviors canbeconfigured bythe
user aspertheapplication requirement.
*Immediate stop ataDMA channel arbitration boundary. Please refer toTable 20-4 andTable 20-5 for
arbitration boundary definition.
*Finish current frame transfer andcontinue after suspend ends.
*Finish current block transfer andcontinue after suspend ends.
*Ignore thesuspend. The DMA continues tobeoperational asinfunctional mode when debug mode is
active.
When theDMA controller enters suspend mode, itcontinues tosample incoming hardware DMA requests,
buttheChannel Pending Register (Section 20.3.1.2 )isfrozen from being updated. After thesuspend
ends, allnew requests thatwere received during suspend mode arereflected intheChannel Pending
Register (Section 20.3.1.2 ).
Except when theDMA controller isconfigured toignore suspend mode, nochannel arbitration is
performed during suspend mode. The current channel under which suspend mode was entered willfinish
itsentire frame orblock-transfer after suspend mode ends, depending how thedebug option was chosen.
Tofacilitate debugging, aWatch Point Register (Section 20.3.1.54 )andaWatch Mask Register
(Section 20.3.1.55 )areused. The watch point register together with thewatch mask register canbe
configured towatch foraunique address orarange ofaddresses. When thecondition towatch istrue, the
DMA freezes itsstate andgenerates adebug request signal tothehost CPU sothestate oftheDMA can
beexamined.
20.2.11 Power Management
The DMA offers twopower-management modes: runandsleep. Inrunmode, theDMA isfully operational.
The sleep mode shuts down theDMA ifnopending channels arewaiting tobeserviced. IfaDMA request
isreceived orasoftware request isgenerated bytheuser software, then theDMA wakes upimmediately.
The sleep mode may beused tooptimize theDMA module power consumption.
When thesystem module issues aglobal lowpower mode request, theDMA willrespond tothesystem
module with anacknowledge assoon asanarbitration boundary isreached. IfnoDMA requests are
pending, itwillrespond with anacknowledge immediately.
NOTE: When theDMA isinglobal lowpower mode, theclock isstopped andtherefore itcannot
detect anyDMA request. The device must bewoken upbefore aperipheral cangenerate a
DMA request.

<!-- Page 715 -->

www.ti.com Module Operation
715 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.12 FIFO Buffer
DMA FIFO is4levels deep and64-bit wide (can hold upto4×64-bits ofdata). They areused forData
packing andunpacking.
The DMA FIFO hastwostates:
*EMPTY: The FIFO contains nodata.
*FULL: The FIFO isfilled ortheelement count hasreached zero; theread operation hastobestopped.
DMA channels canonly beswitched when theFIFO isempty. This also implies thatarbitration between
channels isdone when theFIFO isempty.
The DMA hastwoFIFOs, FIFO AandFIFO B,each executing achannel thatprovides thecapability to
execute amaximum oftwochannels concurrently.
The FIFO buffer may bebypassed through theuseofthebypass feature intheport control register; see
Port Control Register (Section 20.3.1.51 )forregister details. Writing 1tothisbitlimits theFIFO depth to
thesize ofoneelement. That means iftheread element size isequal toorlarger than thewrite element
size, after oneelement isread thewrite outtothedestination starts. Otherwise, thewrite outtothe
destination starts after enough reads have completed todoonewrite ofthewrite element size. This
feature isparticularly useful tominimize switching latency in-between channels. When bypass mode is
enabled, theDMA performs minimal transfers within anarbitration boundary. Inaddition, thebypass
feature allows arbitration between channels thatcanbecarried outatasource element granularity.
However, ithastobeconsidered thatwhile inbypass mode, theDMA controller does notmake optimal
useofthebusbandwidth. Since theread andwrite element sizes canbedifferent, then thenumber of
read andwrite transactions willbedifferent. Table 20-4 andTable 20-5 show acomparison between the
number ofread andwrite transactions performed bytheDMA controller from onechannel toanother
before arbitration innon-bypass andbypass mode.
Table 20-4. Maximum Number ofDMA Transactions perChannel inNon-Bypass Mode
Write
Element
Size8bit 16bit 32bit 64bit
Read
Element
Size8bit 4read 4write 4read 2write 4read 1write 8read 1write
16bit 2read 4write 4read 4write 4read 2write 4read 1write
32bit 1read 4write 2read 4write 4read 4write 4read 2write
64bit 1read 8write 1read 4write 2read 4write 4read 4write
Table 20-5. Maximum Number ofDMA Transactions perChannel inBypass Mode
Write
Element
Size8bit 16bit 32bit 64bit
Read
Element
Size8bit 1read 1write 2read 1write 4read 1write 8read 1write
16bit 1read 2write 1read 1write 2read 1write 4read 1write
32bit 1read 4write 1read 2write 1read 1write 2read 1write
64bit 1read 8write 1read 4write 1read 2write 1read 1write

<!-- Page 716 -->

DMA_REQ[31:0]
CH15ASI[5:0]CH14ASI[5:0]CH1ASI[5:0]CH1ASI[5:0]Ch chain0
Ch Sel0
Ch chain1
Ch Sel1
Ch chain14
Ch Sel14
Ch chain15
Ch Sel15000
0Pending
Register
Bit 0
Bit 2
Bit 14
Bit 15Bit 1
Module Operation www.ti.com
716 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.13 Channel Chaining
Channel chaining isused totrigger asingle ormultiple channels with outanexternal DMA request. This is
possible bychaining onecontrol packet toother. Chain[5:0] field oftheChannel Control Register
(Section 20.3.2.4 )isused toprogram thechaining control packet. Chained control packets follow
arbitration rules within thepending register. Forexample ifCH1, CH2, CH4, CH5 aretriggered together
andCH3 ischained with CH1. The order ofchannels serviced inspite ofchaining willbeCH1 ->CH2 ->
CH3 ->CH4 ->CH5.
Inorder tosetup upchannel chain feature, theChannel Control Register (Section 20.3.2.4 )needs tobe
enabled forallchained channels before triggering firstDMA request.
Figure 20-16 illustrates how internally chained request isgenerated after completing therequired transfers
andstored inpending register. Inthisexample CH1 isChained toCH0. When CH0 istriggered CH1 is
captured aspending intheChannel Pending Register (Section 20.3.1.2 )even when itisnottriggered.
Figure 20-16. Example ofChannel Chaining
20.2.14 Request Polarity
DMA supports both active high andactive lowhardware requests. This isconfigured through theregisters
DMAREQPS1 andDMAREQPS0.
The selection ofrequest polarity should bedone atthestart oftheprogram. Inorder tochange the
request polarity from active high toactive lowforachannel following sequence should befollowed:
1.Disable channel forwhich polarity istobechanged using theHWCHENA bit.
2.Disable theperipheral inorder thatitmay settherequest linetoinactive high state (since bydefault
requests areactive high).
3.Apply software reset totheDMA using theGCTRL register.
4.Program therequest polarity forthechannel.
5.Re-enable theDMA channel.
6.Re-enable theperipheral thattriggers theDMA event.

<!-- Page 717 -->

www.ti.com Module Operation
717 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.15 Memory Protection
The DMA controller iscapable ofaccess tothefulladdress range ofthedevice. The protection
mechanism allows theprotection ofmultiple memory regions torestrict accesses tothose address ranges.
This willallow theapplication toprotect critical application data from unintentionally being accessed bythe
DMA controller.
20.2.15.1 Protection Mechanism
The memory protection mechanism consists oftheaccess privilege foragiven memory region, thestart
andendaddress fortheregion, andnotification ofanaccess violation fortheprotected region.
Each region tobeprotected isconfigured bysoftware bywriting thestart address andendaddress for
each region intotheDMA Memory Protection Registers, DMAMPRxS andDMAMPRxE. The definition of
these registers canbefound starting atSection 20.3.1.64 .Any region inthevalid address space canbe
protected from inappropriate accesses.
The access privileges canbesettooneoffour permission settings asshown below:
*Fullaccess
*Read only access
*Write only access
*Noaccess
The permissions foragiven region areselected bywriting theappropriate values intheDMA Memory
Protection Control Register (Section 20.3.1.64 ).
Aregion ofmemory notconfigured foraccess settings bytheregisters has"Full Access" privileges.
NOTE: Iftheregions defined bythestart andendaddresses overlap, theregion defined firstinthe
register space determines theaccess privilege. Forexample, ifregion 0andregion 1
overlap, theaccess permissions defined forregion 0willtake precedence since region 0
registers arebefore region 1.
Inacase where amemory protection violation occurs, aflagwillbesetandaninterrupt willbegenerated,
ifinterrupts areenabled. The DMA Memory Protection Status Register (Section 20.3.1.65 )contains the
status flags forthememory protection mechanism, andtheDMA Memory Protection Control Register
(Section 20.3.1.64 )contains theinterrupt enable bits. Upon detection ofthememory protection violation,
theDMA Channel thatcaused theviolation willbestopped andthenext available DMA channel willbe
serviced.
Figure 20-17 Illustrates aprotection mechanism.

<!-- Page 718 -->

0x000000000x080000000x08003FFF0xFFFFFFFF
System + peripherals
Region0Region1No access restrictions
Access restrictions
RAMRegion2Region3
0xFFF78000
Module Operation www.ti.com
718 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleFigure 20-17. Example ofProtection Mechanism
20.2.16 ECC Checking
The Control packet RAM isprotected using aSingle Error Correction Double Error Detection (SECDED)
scheme. This scheme isimplemented using atotal of9ECC check bitsforevery 128bitsofdata stored in
theDMA Control Packet RAM.
ECC checking canbeenabled anddisabled within themodule bya4-bit key. The keyislocated inthe
ECC Control Register (Section 20.3.1.62 ).
During write accesses toControl Packet RAM, ECC bitsaregenerated automatically andstored along
with thedata bitstothememory.
During read accesses from theControl Packet RAM, theECC bitsinmemory arechecked against a
computed ECC value forthe128bitsofdata. Following twokinds oferrors canoccur during theread:
*Single-Bit Error -Ifasingle-bit error occurs during thereads tothecontrol packet either bytheCPU or
byDMA logic andtheEDCAMODE[3:0] inDMASECCCTRL register is0xA, theerror isautomatically
corrected. The SBEFLG bitintheregister isalso setto1toindicate asingle-bit error was corrected.
The DMAECCSBE register isupdated toindicate theerror address. Inaddition, ifthe
SBE_EVT_EN[3:0] inDMASECCCTRL register is0xA, theerror isalso indicated toESM.
*Double-Bit Error -Ifadouble-bit error occurs during thereads tothecontrol packet either bytheCPU
ortheDMA logic andtheECC_ENA[3:0] inDMAPECR register is0xA, theerror isindicated toESM.
The EDFLG bitgets setandtheerror address isstored inDMAPAR register.
The DMA module automatically performs read-modify-write operations totheControl Packet RAM which
arerequired during CPU configuration ofthecontrol packet RAM. Errors occuring during these reads are
also covered bytheSECDED scheme. Also, reads totheWorking Packet byCPU orDMA logic and
writes totheWorking Packet bytheDMA logic arealso protected bySECDED.
During double-bit errors, itispossible toconfigure thebehavior ofthechannel using theERRA bitin
DMAPECR register. Two options areavailable:
*IfERRA bitiscleared, errors areignored andchannel operation willresume normally.
*IfERRA bitisset,errors willcause theDMA tobedisabled (DMA_EN bitinGCTRL register is
cleared). Allchannels willstop servicing atthenext arbitration boundary. This action willbetaken
regardless oftheorigin oferror being aCPU read oraDMA logic read.

<!-- Page 719 -->

www.ti.com Module Operation
719 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.17 ECC Testing
The ECC RAM isaccessible toallow manually inserting faults sothattheECC checking feature canbe
tested. Test mode isentered byasserting theTEST bitintheECC Control Register (Section 20.3.1.62 ).
Once thebitisset,theECC bitsaremapped tothecontrol packet RAM starting address A00h. The
sequence totesttheECC is:
1.Write thedata location oftheControl Packet RAM while keeping ECC_ENA active. The ECC bitswill
getautomatically written with thecorrect values inthisstep.
2.Enable ECC testmode bysetting theTEST bitoftheDMAPECR register.
3.Totestsingle-bit error correction capability, read back oneofthedata written earlier, fliponeofthebits
andwrite itback. The same could bedone fortheECC bitsaswell.
4.Similarly, totestdouble-bit detection capability, read back oneofthedata written earlier, fliptwobits
andwrite itback. The same could bedone fortheECC bitsaswell.
5.Now read back thesame data bitsthatwere corrupted orforwhich theECC was corrupted inthe
earlier steps 3-4.
6.Depending onthekind ofcorruption created, fordouble-bit error, read EDFLG anderror address
captured inDMAPAR; similarly forsingle-bit error, read SBERR inDMASECCCTRL anderror address
inDMAECCSBE.
7.The check issuccessful iftheflaganderror address areupdated successfully.
8.Clear theflags (EDFLG orSBERR asapplicable) andread theerror address.
9.Toexitthetestmode, initialize thedata andECC thatwere corrupted earlier, back totheir original
values.
10.Finally, clear theTEST bitoftheDMAPECR register.
NOTE: When intestmode, noECC checking willbedone when reading from ECC memory, but
ECC checking willbeperformed onthenormal memory.
This offsets inTable 20-6 must beused toruntheECC diagnostics.
Table 20-6. ECC Mapping
Offset ECC ofControl Packet (Only 9bitsarevalid intheread)
A00h 0(Lower 128bits)
A04h 0(Upper 128bits)
A08h 1(Lower 128bits)
: :
AFCh 31(Upper 128bits)
20.2.18 Initializing RAM with ECC
After power up,theRAM content including theECC bitscannot beguaranteed. Toavoid ECC failures
when reading RAM, theRAM hastobeinitialized. The RAM canbeinitialized bywriting known values into
it.When theknown value iswritten, thecorresponding ECC bitwillbeautomatically calculated and
updated.
Another possibility toinitialize thememory istofollow theAuto-Initialization ofOn-Chip SRAM Modules
subsection intheArchitecture chapter. The RAM willbeinitialized to0.Depending ontheeven/odd parity
selection, theparity bitwillbecalculated accordingly.
Toallow forECC calculation during initialization, theECC functionality hastobeenabled asdiscussed in
Section 20.2.16 .

<!-- Page 720 -->

PortAAddr.
Addr. Parity
Controls
Control Parity
Response
Response Parity Parity CheckersDMA
DMA_TER_ERR
(to ESM)PortBAddr.
Controls
Response
TERECTRL
TEROFFSETTER_EN
NOTE: Only PortA supports transaction parityExt. 
Parity 
Checkers
Module Operation www.ti.com
720 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.2.19 Transaction Errors
DMA generates parity foralltransactions andchecks parity forresponses tothetransactions. Note that
thisfeature isdistinct from theECC checking fortheControl Packet RAM.
Ifaparity error isdetected inthese transactions andTER_EN bitinTERECTRL register isenabled, DMA
willstop processing thecurrent channel atthearbitration boundary andwillupdate TER_ERR flag. The
offset ofthechannel during which theparity failure was detected willgetcaptured intheTERROFFSET
register. Also, theerror isindicated totheESM module. This isshown inFigure 20-18 .
Since thechannel stops duetoanerror andlikely theperipheral andtheDMA areoutofsynchronization,
itisrecommended tofollow thesequence below toresume thechannel:
1.Read theTEROFFSET register tofindthechannel number causing thetransaction error. The register
automatically clears to0once read.
2.Clear theTER_ERR flagbywriting 1totheflag.
3.Disable theperipheral thattriggered theDMA event.
4.Reinitialize thecontrol packet. Note thatthisdoes notchange thechannel's HWCHEN bit.
5.Re-enable theperipheral totrigger theDMA event.
6.Re-enable theDMA channel (which was previously cleared bytheDMA logic duetotheerror).
Incertain cases, itispossible thatDMA sets theTER_ERR flagwithout updating theTEROFFSET
register. This occurs duetoparity errors when nochannels areactive. The recovery sequence inthiscase
istoclear theTER_ERR flag.
NOTE: Handling ofaparity error atasystem level may require additional operations thatarenot
detailed here.
Figure 20-18. DMA Transaction Parity

<!-- Page 721 -->

www.ti.com Control Registers andControl Packets
721 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3 Control Registers andControl Packets
The DMA control registers aresummarized inTable 20-7.The base address forthecontrol registers is
FFFF F000h. The control packets aresummarized inTable 20-8 .The base address forthecontrol
packets isFFF8 0000h. Each register begins onaword boundary. Allregisters andcontrol packets are
accessible in8,16,and32bit.
NOTE: The register definitions aregiven forafullDMA module configuration (32channels, 64
requests, 2Ports, Dual CPU support). Writes andReads ofbitspertaining tofeatures not
included intheDMA implementation asdefined inthedevice-specific data manual are
possible without error; however, they willhave noaffect ondevice operation.
Table 20-7. DMA Control Registers
Offset Acronym Register Description Section
00h GCTRL Global Control Register Section 20.3.1.1
04h PEND Channel Pending Register Section 20.3.1.2
0Ch DMASTAT DMA Status Register Section 20.3.1.3
10h DMAREVID DMA revision IDRegister Section 20.3.1.4
14h HWCHENAS HWChannel Enable SetandStatus Register Section 20.3.1.4
1Ch HWCHENAR HWChannel Enable Reset andStatus Register Section 20.3.1.6
24h SWCHENAS SWChannel Enable SetandStatus Register Section 20.3.1.7
2Ch SWCHENAR SWChannel Enable Reset andStatus Register Section 20.3.1.8
34h CHPRIOS Channel Priority SetRegister Section 20.3.1.9
3Ch CHPRIOR Channel Priority Reset Register Section 20.3.1.10
44h GCHIENAS Global Channel Interrupt Enable SetRegister Section 20.3.1.11
4Ch GCHIENAR Global Channel Interrupt Enable Reset Register Section 20.3.1.12
54h DREQASI0 DMA Request Assignment Register 0 Section 20.3.1.13
58h DREQASI1 DMA Request Assignment Register 1 Section 20.3.1.14
5Ch DREQASI2 DMA Request Assignment Register 2 Section 20.3.1.15
60h DREQASI3 DMA Request Assignment Register 3 Section 20.3.1.16
64h DREQASI4 DMA Request Assignment Register 4 Section 20.3.1.13
68h DREQASI5 DMA Request Assignment Register 5 Section 20.3.1.13
6ch DREQASI6 DMA Request Assignment Register 6 Section 20.3.1.13
70h DREQASI7 DMA Request Assignment Register 7 Section 20.3.1.13
94h PAR0 Port Assignment Register 0 Section 20.3.1.21
98h PAR1 Port Assignment Register 1 Section 20.3.1.22
9Ch PAR2 Port Assignment Register 2 Section 20.3.1.23
A0h PAR3 Port Assignment Register 3 Section 20.3.1.24
B4h FTCMAP FTC Interrupt Mapping Register Section 20.3.1.25
BCh LFSMAP LFS Interrupt Mapping Register Section 20.3.1.26
C4h HBCMAP HBC Interrupt Mapping Register Section 20.3.1.27
CCh BTCMAP BTC Interrupt Mapping Register Section 20.3.1.28
DCh FTCINTENAS FTC Interrupt Enable SetRegister Section 20.3.1.29
E4h FTCINTENAR FTC Interrupt Enable Reset Register Section 20.3.1.30
ECh LFSINTENAS LFS Interrupt Enable SetRegister Section 20.3.1.31
F4h LFSINTENAR LFS Interrupt Enable Reset Register Section 20.3.1.32
FCh HBCINTENAS HBC Interrupt Enable SetRegister Section 20.3.1.33
104h HBCINTENAR HBC Interrupt Enable Reset Register Section 20.3.1.34
10Ch BTCINTENAS BTC Interrupt Enable SetRegister Section 20.3.1.35
114h BTCINTENAR BTC Interrupt Enable Reset Register Section 20.3.1.36
11Ch GINTFLAG Global Interrupt Flag Register Section 20.3.1.37

<!-- Page 722 -->

Control Registers andControl Packets www.ti.com
722 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-7. DMA Control Registers (continued)
Offset Acronym Register Description Section
124h FTCFLAG FTC Interrupt Flag Register Section 20.3.1.38
12Ch LFSFLAG LFS Interrupt Flag Register Section 20.3.1.39
134h HBCFLAG HBC Interrupt Flag Register Section 20.3.1.40
13Ch BTCFLAG BTC Interrupt Flag Register Section 20.3.1.41
144h BERFLAG BER Interrupt Flag Register Section 20.3.1.42
14Ch FTCAOFFSET FTCA Interrupt Channel Offset Register Section 20.3.1.43
150h LFSAOFFSET LFSA Interrupt Channel Offset Register Section 20.3.1.44
154h HBCAOFFSET HBCA Interrupt Channel Offset Register Section 20.3.1.45
158h BTCAOFFSET BTCA Interrupt Channel Offset Register Section 20.3.1.46
160h FTCBOFFSET FTCB Interrupt Channel Offset Register Section 20.3.1.47
164h LFSBOFFSET LFSB Interrupt Channel Offset Register Section 20.3.1.48
168h HBCBOFFSET HBCB Interrupt Channel Offset Register Section 20.3.1.49
16Ch BTCBOFFSET BTCB Interrupt Channel Offset Register Section 20.3.1.50
178h PTCRL Port Control Register Section 20.3.1.51
17Ch RTCTRL RAM Test Control Register Section 20.3.1.52
180h DCTRL Debug Control Register Section 20.3.1.53
184h WPR Watch Point Register Section 20.3.1.54
188h WMR Watch Mask Register Section 20.3.1.55
18Ch FAACSADDR FIFO AActive Channel Source Address Register
190h FAACDADDR FIFO AActive Channel Destination Address Register
194h FAACTC FIFO AActive Channel Transfer Address Register
198h FBACSADDR FIFO BActive Channel Source Address Register Section 20.3.1.56
19Ch FBACDADDR FIFO BActive Channel Destination Address Register Section 20.3.1.57
1A0h FBACTC FIFO BActive Channel Transfer Address Register Section 20.3.1.58
1A8h DMAPECR Parity Control Register Section 20.3.1.62
1ACh DMAPAR DMA Parity Error Address Register Section 20.3.1.63
1B0h DMAMPCTRL1 DMA Memory Protection Control Register 1 Section 20.3.1.64
1B4h DMAMPST1 DMA Memory Protection Status Register 1 Section 20.3.1.65
1B8h DMAMPR0S DMA Memory Protection Region 0Start Address Register Section 20.3.1.66
1BCh DMAMPR0E DMA Memory Protection Region 0End Address Register Section 20.3.1.67
1C0h DMAMPR1S DMA Memory Protection Region 1Start Address Register Section 20.3.1.68
1C4h DMAMPR1E DMA Memory Protection Region 1End Address Register Section 20.3.1.69
1C8h DMAMPR2S DMA Memory Protection Region 2Start Address Register Section 20.3.1.70
1CCh DMAMPR2E DMA Memory Protection Region 2End Address Register Section 20.3.1.71
1D0h DMAMPR3S DMA Memory Protection Region 3Start Address Register Section 20.3.1.72
1D4h DMAMPR3E DMA Memory Protection Region 3End Address Register Section 20.3.1.73
1D8h DMAMPCTRL DMA Memory Protection Control Register Section 20.3.1.74
1DCh DMAMPST2 DMA Memory Protection Status Register 2 Section 20.3.1.75
1E0h DMAMPR4S DMA Memory Protection Region 4Start Address Register Section 20.3.1.76
1E4h DMAMPR4E DMA Memory Protection Region 4End Address Register Section 20.3.1.77
1E8h DMAMPR5S DMA Memory Protection Region 5Start Address Register Section 20.3.1.78
1ECh DMAMPR5E DMA Memory Protection Region 5End Address Register Section 20.3.1.79
1F0h DMAMPR6S DMA Memory Protection Region 6Start Address Register Section 20.3.1.80
1F4h DMAMPR6E DMA Memory Protection Region 6End Address Register Section 20.3.1.81
1F8h DMAMPR7S DMA Memory Protection Region 7Start Address Register Section 20.3.1.82
1FCh DMAMPR7E DMA Memory Protection Region 7End Address Register Section 20.3.1.83
228h DMASECCCTRL DMA Single-bit ECC Control Register Section 20.3.1.84

<!-- Page 723 -->

www.ti.com Control Registers andControl Packets
723 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-7. DMA Control Registers (continued)
Offset Acronym Register Description Section
230h DMAECCSBE DMA ECC Single-bit Error Address Register Section 20.3.1.85
240h FIFOASTATREG FIFO AStatus Register Section 20.3.1.86
244h FIFOBSTATREG FIFO BStatus Register Section 20.3.1.87
330h DMAREQPS1 DMA Request Polarity Select Register 1 Section 20.3.1.88
334h DMAREQPS0 DMA Request Polarity Select Register 0 Section 20.3.1.89
340h TERECTRL TER Event Control Register Section 20.3.1.90
344h TERFLAG TER Event Flag Register Section 20.3.1.91
348h TERROFFSET TER Event Channel Offset Register Section 20.3.1.92
Table 20-8. Control Packet Memory Map
Offset Acronym Register Description Section
Primary Control Packet 0
00h ISADDR Initial Source Address Register Section 20.3.2.1
04h IDADDR Initial Destination Address Register Section 20.3.2.2
08h ITCOUNT Initial Transfer Count Register Section 20.3.2.3
10h CHCTRL Channel Control Register Section 20.3.2.4
14h EIOFF Element Index Offset Register Section 20.3.2.5
18h FIOFF Frame Index Offset Register Section 20.3.2.6
Working Control Packet 0
800h CSADDR Current Source Address Register Section 20.3.2.7
804h CDADDR Current Destination Address Register Section 20.3.2.8
808h CTCOUNT Current Transfer Count Register Section 20.3.2.9

<!-- Page 724 -->

Control Registers andControl Packets www.ti.com
724 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1 Global Configuration Registers
These registers control theoverall behavior oftheDMA controller.
20.3.1.1 Global Control Register (GCTRL)
Figure 20-19. Global Control Register (GCTRL) [offset =00]
31 17 16
Reserved DMA_EN
R-0 R/WP-0
15 14 13 10 9 8
Reserved BUS_BUSY Reserved DEBUGMODE
R-0 R-0 R-0 R/WP-0
7 1 0
Reserved DMARES
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-9. Global Control Register (GCTRL) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 DMA_EN DMA enable bit.The configuration registers andchannel control packets should besetup first
before DMA_EN bitissettoonetoprevent state machines from carrying outbustransactions.
IfDMA_EN bitiscleared inthemiddle ofanbustransaction, thestate machine willstop atan
arbitration boundary.
0 The DMA isdisabled.
1 The DMA isenabled.
15 Reserved 0 Reads return 0.Writes have noeffect.
14 BUS_BUSY This bitindicates status ofDMA external AHB busstatus.
0 DMAs external busisnotbusy indata transfers.
1 DMAs external busisbusy indata transfers.
13-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 DEBUGMODE Debug Mode.
0 Ignore suspend.
1h Finish current block transfer.
2h Finish current frame transfer.
3h Immediately stop atanDMA arbitration boundary andcontinue after suspend.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 DMARES DMA software reset.
Note: Intheevent aDMA slave does notrespond, theDMA module willrespond tothe
software reset upon reaching anarbitration boundary.
0 Read: Software reset isdisabled.
Write: Noeffect.
1 Read andwrite: The DMA state machine andallcontrol registers areinsoftware reset. Control
packets arenotreset when DMA software reset isactive.

<!-- Page 725 -->

www.ti.com Control Registers andControl Packets
725 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.2 Channel Pending Register (PEND)
Figure 20-20. Channel Pending Register (PEND) [offset =04h]
31 0
PEND[31:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-10. Channel Pending Register (PEND) Field Descriptions
Bit Field Value Description
31-0 PEND[ n] Channel pending register. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
Reading from PEND gives thechannel pending information nomatter ifthechannel was initiated bySW
orHW. Once set,itremains seteven ifthecorresponding channel isdisabled viaHWCHENA or
SWCHENA. The pending bitisautomatically cleared forthefollowing conditions:
*Attheendofaframe orablock transfer depending onhow thechannel istriggered asprogrammed
intheTTYPE bitfield ofCHCTRL.
*The control packet ismodified after thepending bitisset.
*Abuserror occurs.
*Atransaction parity error occurs
0 The corresponding channel isinactive.
1 The corresponding channel ispending andiswaiting forservice.
20.3.1.3 DMA Status Register (DMASTAT)
Figure 20-21. DMA Status Register (DMASTAT) [offset =0Ch]
31 0
STCH[31:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-11. DMA Status Register (DMASTAT) Field Descriptions
Bit Field Value Description
31-0 STCH[ n] Status ofDMA channels. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
0 The channel isnotbeing currently processed.
1 The channel iscurrently being processed using oneoftheFIFOs.
Note: The status ofachannel currently being processed remains active even ifemulation mode is
entered orDMA isdisabled viaDMA_EN bit.Since there aretwoFIFOs, upto2bitscanbesetinthis
register atanygiven time.

<!-- Page 726 -->

Control Registers andControl Packets www.ti.com
726 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.4 DMA Revision IDRegister (DMAREVID)
Figure 20-22. DMA Revision IDRegister (DMAREVID) [offset =10h]
31 30 29 28 27 16
SCHEME Reserved FUNC
R-1 R-0 R-A0Dh
15 11 10 8 7 6 5 0
Reserved MAJOR Reserved MINOR
R-0 R-0 R-0 R-3h
LEGEND: R=Read only; -n=value after reset
Table 20-12. DMA Revision IDRegister Description
Bit Field Value Description
31-30 SCHEME 1 Identification Scheme ofREVID.
29-28 Reserved 0 Reads return 0.Writes have noeffect.
27-16 FUNC A0Dh Indicates module family.
15-11 Reserved 0 Reserved
10-8 MAJOR 0 Major revision number.
7-6 Reserved 0 Reserved
5-0 MINOR 3h Minor revision number.

<!-- Page 727 -->

www.ti.com Control Registers andControl Packets
727 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.5 HWChannel Enable SetandStatus Register (HWCHENAS)
Figure 20-23. HWChannel Enable SetandStatus Register (HWCHENAS) [offset =14h]
31 0
HWCHENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-13. HWChannel Enable SetandStatus Register (HWCHENAS) Field Descriptions
Bit Field Value Description
31-0 HWCHENA[ n] Hardware channel enable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,and
soon.Anactive hardware DMA request cannot initiate aDMA transfer unless thecorresponding
hardware enable bitisset.
The corresponding hardware enable bitiscleared automatically forthefollowing conditions:
*Attheendofablock transfer iftheauto-initiation bitAIM (see CHCTRL) isnotactive.
*Ifabuserror isdetected foranactive channel.
Reading from HWCHENAS gives thestatus (enabled/disabled) ofallchannels.
0 The corresponding channel isdisabled forhardware triggering.
1 The corresponding channel isenabled forhardware triggering.
20.3.1.6 HWChannel Enable Reset andStatus Register (HWCHENAR)
Figure 20-24. HWChannel Enable Reset andStatus Register (HWCHENAR) [offset =1Ch]
31 0
HWCHDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-14. HWChannel Enable Reset andStatus Register (HWCHENAR) Field Descriptions
Bit Field Value Description
31-0 HWCHDIS[ n] HWchannel disable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
0 Read: The corresponding channel isdisabled forHWtriggering.
Write: Noeffect.
1 Read: The corresponding channel isenabled forHWtriggering.
Write: The corresponding channel isdisabled.

<!-- Page 728 -->

Control Registers andControl Packets www.ti.com
728 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.7 SWChannel Enable SetandStatus Register (SWCHENAS)
Figure 20-25. SWChannel Enable SetandStatus Register (SWCHENAS) [offset =24h]
31 0
SWCHENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-15. SWChannel Enable SetandStatus Register (SWCHENAS) Field Descriptions
Bit Field Value Description
31-0 SWCHENA[ n] SWchannel enable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
Writing a1toabittriggers aSWrequest onthecorresponding channel tostart aDMA transaction.
The corresponding bitisautomatically cleared bythefollowing conditions.
*The corresponding bitiscleared after oneframe transfer iftheTTYPE bitinChannel Control
Register (CHCTRL) isprogrammed forframe transfer.
*The corresponding bitiscleared after oneblock transfer ifthecorresponding TTYPE bitis
programmed forblock transfer andtheauto-initiation bitisnotenabled.
*The control packet ismodified after thepending bitisset.
*The corresponding bitiscleared after oneblock transfer when TTYPE bitisprogrammed for
blocks transfer andifthecorresponding bitinHWchannel enable register (HWCHENAS) is
enabled. When achannel isenabled forboth HWandSW, thestate machine willinitiate
transfers based ontheSWfirst. After oneblock transfer iscomplete, thecorresponding bitinthe
SWCHENA register isthen cleared. The same channel isserviced again byaHWDMA request.
*The corresponding bitiscleared ifabuserror isdetected.
*Atransaction parity error occurs.
Reading from SWCHENAS gives thestatus (enabled/disabled) ofchannels 0through 31.
0 The corresponding channel isnottriggered bySWrequest.
1 The corresponding channel istriggered bySWrequest.
20.3.1.8 SWChannel Enable Reset andStatus Register (SWCHENAR)
Figure 20-26. SWChannel Enable Reset andStatus Register (SWCHENAR) [offset =2Ch]
31 0
SWCHDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-16. SWChannel Enable Reset andStatus Register (SWCHENAR) Field Descriptions
Bit Field Value Description
31-0 SWCHDIS[ n] SWchannel disable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
0 Read: The corresponding channel was nottriggered bySW.
Write: Noeffect.
1 Read: The corresponding channel was triggered bySW.
Write: The corresponding channel isdisabled.

<!-- Page 729 -->

www.ti.com Control Registers andControl Packets
729 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.9 Channel Priority SetRegister (CHPRIOS)
Figure 20-27. Channel Priority SetRegister (CHPRIOS) [offset =34h]
31 0
CPS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-17. Channel Priority SetRegister (CHPRIOS) Field Descriptions
Bit Field Value Description
31-0 CPS[ n] Channel priority setbit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
Writing a1toabitassigns thecorresponding channel tothehigh priority queue.
0 Read: The corresponding channel isassigned tothelowpriority queue.
Write: Noeffect.
1 Read andwrite: The corresponding channel isassigned tohigh priority queue.
20.3.1.10 Channel Priority Reset Register (CHPRIOR)
Figure 20-28. Channel Priority Reset Register (CHPRIOR) [offset =3Ch]
31 0
CPR[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-18. Channel Priority Reset Register (CHPRIOR) Field Descriptions
Bit Field Value Description
31-0 CPR[ n] Channel priority reset bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.
Writing a1toabitassigns theaccording channel tothelowpriority queue.
0 Read: The corresponding channel isassigned tothelowpriority queue.
Write: Noeffect.
1 Read: The corresponding channel isassigned tothehigh priority queue.
Write: The corresponding channel isassigned tothelowpriority queue.

<!-- Page 730 -->

Control Registers andControl Packets www.ti.com
730 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.11 Global Channel Interrupt Enable SetRegister (GCHIENAS)
Figure 20-29. Global Channel Interrupt Enable SetRegister (GCHIENAS) [offset =44h]
31 0
GCHIE[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-19. Global Channel Interrupt Enable SetRegister (GCHIENAS) Field Descriptions
Bit Field Value Description
31-0 GCHIE[ n] Global channel interrupt enable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,and
soon.
0 Read: The corresponding channel isdisabled forinterrupt.
Write: Noeffect.
1 Read andwrite: The corresponding channel isenabled forinterrupt.
20.3.1.12 Global Channel Interrupt Enable Reset Register (GCHIENAR)
Figure 20-30. Global Channel Interrupt Enable Reset Register (GCHIENAR) [offset =4Ch]
31 0
GCHID[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-20. Global Channel Interrupt Enable Reset Register (GCHIENAR) Field Descriptions
Bit Field Value Description
31-0 GCHID[ n] Global channel interrupt disable bit.Bit0corresponds tochannel 0,bit1corresponds tochannel 1,and
soon.
0 Read: The corresponding channel isdisabled forinterrupt.
Write: Noeffect.
1 Read: The corresponding channel isenabled forinterrupt.
Write: The corresponding channel isdisabled forinterrupt.

<!-- Page 731 -->

www.ti.com Control Registers andControl Packets
731 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.13 DMA Request Assignment Register 0(DREQASI0)
Figure 20-31. DMA Request Assignment Register 0(DREQASI0) [offset =54h]
31 30 29 24 23 22 21 16
Reserved CH0ASI Reserved CH1ASI
R-0 R/WP-0 R-0 R/WP-1h
15 14 13 8 7 6 5 0
Reserved CH2ASI Reserved CH3ASI
R-0 R/WP-2h R-0 R/WP-3h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-21. DMA Request Assignment Register 0(DREQASI0) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH0ASI Channel 0assignment. This bitfield chooses theDMA request assignment forchannel 0.
0 DMA request line0triggers channel 0.
: :
2Fh DMA request line47triggers channel 0.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH1ASI Channel 1assignment. This bitfield chooses theDMA request assignment forchannel 1.
0 DMA request line0triggers channel 1.
: :
2Fh DMA request line47triggers channel 1.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH2ASI Channel 2assignment. This bitfield chooses theDMA request assignment forchannel 2.
0 DMA request line0triggers channel 2.
: :
2Fh DMA request line47triggers channel 2.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH3ASI Channel 3assignment. This bitfield chooses theDMA request assignment forchannel 3.
0 DMA request line0triggers channel 3.
: :
2Fh DMA request line47triggers channel 3.
30h-
3FhReserved

<!-- Page 732 -->

Control Registers andControl Packets www.ti.com
732 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.14 DMA Request Assignment Register 1(DREQASI1)
Figure 20-32. DMA Request Assignment Register 1(DREQASI1) [offset =58h]
31 30 29 24 23 22 21 16
Reserved CH4ASI Reserved CH5ASI
R-0 R/WP-4h R-0 R/WP-5h
15 14 13 8 7 6 5 0
Reserved CH6ASI Reserved CH7ASI
R-0 R/WP-6h R-0 R/WP-7h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-22. DMA Request Assignment Register 1(DREQASI1) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH4ASI Channel 4assignment. This bitfield chooses theDMA request assignment forchannel 4.
0 DMA request line0triggers channel 4.
: :
2Fh DMA request line47triggers channel 4.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-26 CH5ASI Channel 5assignment. This bitfield chooses theDMA request assignment forchannel 5.
0 DMA request line0triggers channel 5.
: :
2Fh DMA request line47triggers channel 5.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH6ASI Channel 6assignment. This bitfield chooses theDMA request assignment forchannel 6.
0 DMA request line0triggers channel 6.
: :
2Fh DMA request line47triggers channel 6.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH7ASI Channel 7assignment. This bitfield chooses theDMA request assignment forchannel 7.
0 DMA request line0triggers channel 7.
: :
2Fh DMA request line47triggers channel 7.
30h-
3FhReserved

<!-- Page 733 -->

www.ti.com Control Registers andControl Packets
733 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.15 DMA Request Assignment Register 2(DREQASI2)
Figure 20-33. DMA Request Assignment Register 2(DREQASI2) [offset =5Ch]
31 30 29 24 23 22 21 16
Reserved CH8ASI Reserved CH9ASI
R-0 R/WP-8h R-0 R/WP-9h
15 14 13 8 7 6 5 0
Reserved CH10ASI Reserved CH11ASI
R-0 R/WP-Ah R-0 R/WP-Bh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-23. DMA Request Assignment Register 2(DREQASI2) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH8ASI Channel 8assignment. This bitfield chooses theDMA request assignment forchannel 8.
0 DMA request line0triggers channel 8.
: :
2Fh DMA request line47triggers channel 8.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH9ASI Channel 9assignment. This bitfield chooses theDMA request assignment forchannel 9.
0 DMA request line0triggers channel 9.
: :
2Fh DMA request line47triggers channel 9.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH10ASI Channel 10assignment. This bitfield chooses theDMA request assignment forchannel 10.
0 DMA request line0triggers channel 10.
: :
2Fh DMA request line47triggers channel 10.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH11ASI Channel 11assignment. This bitfield chooses theDMA request assignment forchannel 11.
0 DMA request line0triggers channel 11.
: :
2Fh DMA request line47triggers channel 11.
30h-
3FhReserved

<!-- Page 734 -->

Control Registers andControl Packets www.ti.com
734 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.16 DMA Request Assignment Register 3(DREQASI3)
Figure 20-34. DMA Request Assignment Register 3(DREQASI3) [offset =60h]
31 30 29 24 23 22 21 16
Reserved CH12ASI Reserved CH13ASI
R-0 R/WP-Ch R-0 R/WP-Dh
15 14 13 8 7 6 5 0
Reserved CH14ASI Reserved CH15ASI
R-0 R/WP-Eh R-0 R/WP-Fh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-24. DMA Request Assignment Register 3(DREQASI3) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH12ASI Channel 12assignment. This bitfield chooses theDMA request assignment forchannel 12.
0 DMA request line0triggers channel 12.
: :
2Fh DMA request line47triggers channel 12.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH13ASI Channel 13assignment. This bitfield chooses theDMA request assignment forchannel 13.
0 DMA request line0triggers channel 13.
: :
2Fh DMA request line47triggers channel 13.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH14ASI Channel 14assignment. This bitfield chooses theDMA request assignment forchannel 14.
0 DMA request line0triggers channel 14.
: :
2Fh DMA request line47triggers channel 14.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH15ASI Channel 15assignment. This bitfield chooses theDMA request assignment forchannel 15.
0 DMA request line0triggers channel 15.
: :
2Fh DMA request line47triggers channel 15.
30h-
3FhReserved

<!-- Page 735 -->

www.ti.com Control Registers andControl Packets
735 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.17 DMA Request Assignment Register 4(DREQASI4)
Figure 20-35. DMA Request Assignment Register 4(DREQASI4) [offset =64h]
31 30 29 24 23 22 21 16
Reserved CH16ASI Reserved CH17ASI
R-0 R/WP-10h R-0 R/WP-11h
15 14 13 8 7 6 5 0
Reserved CH18ASI Reserved CH19ASI
R-0 R/WP-12h R-0 R/WP-13h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-25. DMA Request Assignment Register 4(DREQASI4) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH16ASI Channel 16assignment. This bitfield chooses theDMA request assignment forchannel 16.
0 DMA request line0triggers channel 16.
: :
2Fh DMA request line47triggers channel 16.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH17ASI Channel 17assignment. This bitfield chooses theDMA request assignment forchannel 17.
0 DMA request line0triggers channel 17.
: :
2Fh DMA request line47triggers channel 17.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH18ASI Channel 18assignment. This bitfield chooses theDMA request assignment forchannel 18.
0 DMA request line0triggers channel 18.
: :
2Fh DMA request line47triggers channel 18.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH19ASI Channel 19assignment. This bitfield chooses theDMA request assignment forchannel 19.
0 DMA request line0triggers channel 19.
: :
2Fh DMA request line47triggers channel 19.
30h-
3FhReserved

<!-- Page 736 -->

Control Registers andControl Packets www.ti.com
736 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.18 DMA Request Assignment Register 5(DREQASI5)
Figure 20-36. DMA Request Assignment Register 5(DREQASI5) [offset =68h]
31 30 29 24 23 22 21 16
Reserved CH20ASI Reserved CH21ASI
R-0 R/WP-14h R-0 R/WP-15h
15 14 13 8 7 6 5 0
Reserved CH22ASI Reserved CH23ASI
R-0 R/WP-16h R-0 R/WP-17h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-26. DMA Request Assignment Register 5(DREQASI5) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH20ASI Channel 20assignment. This bitfield chooses theDMA request assignment forchannel 20.
0 DMA request line0triggers channel 20.
: :
2Fh DMA request line47triggers channel 20.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-26 CH21ASI Channel 21assignment. This bitfield chooses theDMA request assignment forchannel 21.
0 DMA request line0triggers channel 21.
: :
2Fh DMA request line47triggers channel 21.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH22ASI Channel 22assignment. This bitfield chooses theDMA request assignment forchannel 22.
0 DMA request line0triggers channel 22.
: :
2Fh DMA request line47triggers channel 22.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH23ASI Channel 23assignment. This bitfield chooses theDMA request assignment forchannel 23.
0 DMA request line0triggers channel 23.
: :
2Fh DMA request line47triggers channel 23.
30h-
3FhReserved

<!-- Page 737 -->

www.ti.com Control Registers andControl Packets
737 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.19 DMA Request Assignment Register 6(DREQASI6)
Figure 20-37. DMA Request Assignment Register 6(DREQASI6) [offset =6Ch]
31 30 29 24 23 22 21 16
Reserved CH24ASI Reserved CH25ASI
R-0 R/WP-18h R-0 R/WP-19h
15 14 13 8 7 6 5 0
Reserved CH26ASI Reserved CH27ASI
R-0 R/WP-1Ah R-0 R/WP-1Bh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-27. DMA Request Assignment Register 6(DREQASI6) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH24ASI Channel 24assignment. This bitfield chooses theDMA request assignment forchannel 24.
0 DMA request line0triggers channel 24.
: :
2Fh DMA request line47triggers channel 24.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH25ASI Channel 25assignment. This bitfield chooses theDMA request assignment forchannel 25.
0 DMA request line0triggers channel 25.
: :
2Fh DMA request line47triggers channel 25.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH26ASI Channel 26assignment. This bitfield chooses theDMA request assignment forchannel 26.
0 DMA request line0triggers channel 26.
: :
2Fh DMA request line47triggers channel 26.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH27ASI Channel 27assignment. This bitfield chooses theDMA request assignment forchannel 27.
0 DMA request line0triggers channel 27.
: :
2Fh DMA request line47triggers channel 27.
30h-
3FhReserved

<!-- Page 738 -->

Control Registers andControl Packets www.ti.com
738 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.20 DMA Request Assignment Register 7(DREQASI7)
Figure 20-38. DMA Request Assignment Register 7(DREQASI7) [offset =70h]
31 30 29 24 23 22 21 16
Reserved CH28ASI Reserved CH29ASI
R-0 R/WP-1Ch R-0 R/WP-1Dh
15 14 13 8 7 6 5 0
Reserved CH30ASI Reserved CH31ASI
R-0 R/WP-1Eh R-0 R/WP-1Fh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-28. DMA Request Assignment Register 7(DREQASI7) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 CH28ASI Channel 28assignment. This bitfield chooses theDMA request assignment forchannel 28.
0 DMA request line0triggers channel 28.
: :
2Fh DMA request line47triggers channel 28.
30h-
3FhReserved
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 CH29ASI Channel 29assignment. This bitfield chooses theDMA request assignment forchannel 29.
0 DMA request line0triggers channel 29.
: :
2Fh DMA request line47triggers channel 29.
30h-
3FhReserved
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 CH30ASI Channel 30assignment. This bitfield chooses theDMA request assignment forchannel 30.
0 DMA request line0triggers channel 30.
: :
2Fh DMA request line47triggers channel 30.
30h-
3FhReserved
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 CH31ASI Channel 31assignment. This bitfield chooses theDMA request assignment forchannel 31.
0 DMA request line0triggers channel 31.
: :
2Fh DMA request line47triggers channel 31.
30h-
3FhReserved

<!-- Page 739 -->

www.ti.com Control Registers andControl Packets
739 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.21 Port Assignment Register 0(PAR0)
Figure 20-39. Port Assignment Register 0(PAR0) [offset =94h]
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd CH0PA Rsvd CH1PA Rsvd CH2PA Rsvd CH3PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd CH4PA Rsvd CH5PA Rsvd CH6PA Rsvd CH7PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-29. Port Assignment Register 0(PAR0) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 CH0PA These bitfields determine towhich port(s) channel 0isassigned.
1h Port AandBcombined, Aread/B write
2h Port Aonly
3h Port Bonly
Others Port AandBcombined, Bread/A write
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CH1PA 0-7h These bitfields determine towhich port channel 1isassigned. Refer toCH0PA forbitvalue
descriptions.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 CH2PA 0-7h These bitfields determine towhich port channel 2isassigned. Refer toCH0PA forbitvalue
descriptions.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 CH3PA 0-7h These bitfields determine towhich port channel 3isassigned. Refer toCH0PA forbitvalue
descriptions.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 CH4PA 0-7h These bitfields determine towhich port channel 4isassigned. Refer toCH0PA forbitvalue
descriptions.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 CH5PA 0-7h These bitfields determine towhich port channel 5isassigned. Refer toCH0PA forbitvalue
descriptions.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 CH6PA 0-7h These bitfields determine towhich port channel 6isassigned. Refer toCH0PA forbitvalue
descriptions.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CH7PA 0-7h These bitfields determine towhich port channel 7isassigned. Refer toCH0PA forbitvalue
descriptions.

<!-- Page 740 -->

Control Registers andControl Packets www.ti.com
740 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.22 Port Assignment Register 1(PAR1)
Figure 20-40. Port Assignment Register 1(PAR1) [offset =98h]
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd CH8PA Rsvd CH9PA Rsvd CH10PA Rsvd CH11PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd CH12PA Rsvd CH13PA Rsvd CH14PA Rsvd CH15PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-30. Port Assignment Register 1(PAR1) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 CH8PA These bitfields determine towhich port channel 8isassigned.
1h Port AandBcombined, Aread/B write
2h Port Aonly
3h Port Bonly
Others Port AandBcombined, Bread/A write
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CH9PA 0-7h These bitfields determine towhich port channel 9isassigned. Refer toCH8PA forbitvalue
descriptions.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 CH10PA 0-7h These bitfields determine towhich port channel 10isassigned. Refer toCH8PA forbitvalue
descriptions.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 CH11PA 0-7h These bitfields determine towhich port channel 11isassigned. Refer toCH8PA forbitvalue
descriptions.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 CH12PA 0-7h These bitfields determine towhich port channel 12isassigned. Refer toCH8PA forbitvalue
descriptions.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 CH13PA 0-7h These bitfields determine towhich port channel 13isassigned. Refer toCH8PA forbitvalue
descriptions.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 CH14PA 0-7h These bitfields determine towhich port channel 14isassigned. Refer toCH8PA forbitvalue
descriptions.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CH15PA 0-7h These bitfields determine towhich port channel 15isassigned. Refer toCH8PA forbitvalue
descriptions.

<!-- Page 741 -->

www.ti.com Control Registers andControl Packets
741 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.23 Port Assignment Register 2(PAR2)
Figure 20-41. Port Assignment Register 2(PAR2) [offset =9Ch]
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd CH0PA Rsvd CH1PA Rsvd CH2PA Rsvd CH3PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd CH4PA Rsvd CH5PA Rsvd CH6PA Rsvd CH7PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-31. Port Assignment Register 2(PAR2) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 CH16PA These bitfields determine towhich port(s) channel 16isassigned.
1h Port AandBcombined, Aread/B write
2h Port Aonly
3h Port Bonly
Others Port AandBcombined, Bread/A write
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CH17PA 0-7h These bitfields determine towhich port channel 17isassigned. Refer toCH16PA forbitvalue
descriptions.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 CH18PA 0-7h These bitfields determine towhich port channel 18isassigned. Refer toCH16PA forbitvalue
descriptions.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 CH19PA 0-7h These bitfields determine towhich port channel 19isassigned. Refer toCH16PA forbitvalue
descriptions.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 CH20PA 0-7h These bitfields determine towhich port channel 20isassigned. Refer toCH16PA forbitvalue
descriptions.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 CH21PA 0-7h These bitfields determine towhich port channel 21isassigned. Refer toCH16PA forbitvalue
descriptions.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 CH22PA 0-7h These bitfields determine towhich port channel 22isassigned. Refer toCH16PA forbitvalue
descriptions.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CH23PA 0-7h These bitfields determine towhich port channel 23isassigned. Refer toCH16PA forbitvalue
descriptions.

<!-- Page 742 -->

Control Registers andControl Packets www.ti.com
742 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.24 Port Assignment Register 3(PAR3)
Figure 20-42. Port Assignment Register 3(PAR3) [offset =A0h]
31 30 28 27 26 24 23 22 20 19 18 16
Rsvd CH24PA Rsvd CH25PA Rsvd CH26PA Rsvd CH27PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
15 14 12 11 10 8 7 6 4 3 2 0
Rsvd CH28PA Rsvd CH29PA Rsvd CH30PA Rsvd CH31PA
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-32. Port Assignment Register 3(PAR3) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 CH24PA These bitfields determine towhich port channel 24isassigned.
1h Port AandBcombined, Aread/B write
2h Port Aonly
3h Port Bonly
Others Port AandBcombined, Bread/A write
27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CH25PA 0-7h These bitfields determine towhich port channel 25isassigned. Refer toCH24PA forbitvalue
descriptions.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-20 CH26PA 0-7h These bitfields determine towhich port channel 26isassigned. Refer toCH24PA forbitvalue
descriptions.
19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 CH27PA 0-7h These bitfields determine towhich port channel 27isassigned. Refer toCH24PA forbitvalue
descriptions.
15 Reserved 0 Reads return 0.Writes have noeffect.
14-12 CH28PA 0-7h These bitfields determine towhich port channel 28isassigned. Refer toCH24PA forbitvalue
descriptions.
11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 CH29PA 0-7h These bitfields determine towhich port channel 29isassigned. Refer toCH24PA forbitvalue
descriptions.
7 Reserved 0 Reads return 0.Writes have noeffect.
6-4 CH30PA 0-7h These bitfields determine towhich port channel 30isassigned. Refer toCH24PA forbitvalue
descriptions.
3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CH31PA 0-7h These bitfields determine towhich port channel 31isassigned. Refer toCH24PA forbitvalue
descriptions.

<!-- Page 743 -->

www.ti.com Control Registers andControl Packets
743 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.25 FTC Interrupt Mapping Register (FTCMAP)
Figure 20-43. FTC Interrupt Mapping Register (FTCMAP) [offset =B4h]
31 0
FTCAB[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-33. FTC Interrupt Mapping Register (FTCMAP) Field Descriptions
Bit Field Value Description
31-0 FTCAB[ n] Frame transfer complete (FTC) interrupt toGroup AorGroup B.Bit0corresponds tochannel 0,bit1
corresponds tochannel 1,andsoon.
0 FTC interrupt ofthecorresponding channel isrouted toGroup A.
1 FTC interrupt ofthecorresponding channel isrouted toGroup B.
20.3.1.26 LFS Interrupt Mapping Register (LFSMAP)
Figure 20-44. LFS Interrupt Mapping Register (LFSMAP) [offset =BCh]
31 0
LFSAB[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-34. LFS Interrupt Mapping Register (LFSMAP) Field Descriptions
Bit Field Value Description
31-0 LFSAB[ n] Last frame started (LFS) interrupt toGroup AorGroup B.Bit0corresponds tochannel 0,bit1
corresponds tochannel 1,andsoon.
0 LFS interrupt ofthecorresponding channel isrouted toGroup A.
1 LFS interrupt ofthecorresponding channel isrouted toGroup B.
20.3.1.27 HBC Interrupt Mapping Register (HBCMAP)
Figure 20-45. HBC Interrupt Mapping Register (HBCMAP) [offset =C4h]
31 0
HBCAB[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-35. HBC Interrupt Mapping Register (HBCMAP) Field Descriptions
Bit Field Value Description
31-0 HBCAB[ n] Half block complete (HBC) interrupt toGroup AorGroup B.Bit0corresponds tochannel 0,bit1
corresponds tochannel 1,andsoon.
0 HBC interrupt ofthecorresponding channel isrouted toGroup A.
1 HBC interrupt ofthecorresponding channel isrouted toGroup B.

<!-- Page 744 -->

Control Registers andControl Packets www.ti.com
744 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.28 BTC Interrupt Mapping Register (BTCMAP)
Figure 20-46. BTC Interrupt Mapping Register (BTCMAP) [offset =CCh]
31 0
BTCAB[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-36. BTC Interrupt Mapping Register (BTCMAP) Field Descriptions
Bit Field Value Description
31-0 BTCAB[ n] Block transfer complete (BTC) interrupt toGroup AorGroup B.Bit0corresponds tochannel 0,bit1
corresponds tochannel 1,andsoon.
0 BTC interrupt ofthecorresponding channel isrouted toGroup A.
1 BTC interrupt ofthecorresponding channel isrouted toGroup B.

<!-- Page 745 -->

www.ti.com Control Registers andControl Packets
745 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.29 FTC Interrupt Enable SetRegister (FTCINTENAS)
Figure 20-47. FTC Interrupt Enable SetRegister (FTCINTENAS) [offset =DCh]
31 0
FTCINTENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-37. FTC Interrupt Enable SetRegister (FTCINTENAS) Field Descriptions
Bit Field Value Description
31-0 FTCINTENA[ n] Frame transfer complete (FTC) interrupt enable. Bit0corresponds tochannel 0,bit1corresponds
tochannel 1,andsoon.
0 Read: Corresponding FTC interrupt ofachannel isdisabled.
Write: Noeffect.
1 Read andwrite: FTC interrupt ofthecorresponding channel isenabled.
20.3.1.30 FTC Interrupt Enable Reset Register (FTCINTENAR)
Figure 20-48. FTC Interrupt Enable Reset (FTCINTENAR) [offset =E4h]
31 0
FTCINTDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-38. FTC Interrupt Enable Reset (FTCINTENAR) Field Descriptions
Bit Field Value Description
31-0 FTCINTDIS[ n] Frame transfer complete (FTC) interrupt disable. Bit0corresponds tochannel 0,bit1corresponds
tochannel 1,andsoon.
0 Read: Corresponding FTC interrupt ofachannel isdisabled.
Write: Noeffect.
1 Read: Corresponding FTC interrupt ofachannel isenabled.
Write: Corresponding FTC interrupt isdisabled.

<!-- Page 746 -->

Control Registers andControl Packets www.ti.com
746 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.31 LFS Interrupt Enable SetRegister (LFSINTENAS)
Figure 20-49. LFS Interrupt Enable SetRegister (LFSINTENAS) [offset =ECh]
31 0
LFSINTENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-39. LFS Interrupt Enable SetRegister (LFSINTENAS) Field Descriptions
Bit Field Value Description
31-0 LFSINTENA[ n] Last frame started (LFS) interrupt enable. Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 Read: Corresponding LFS interrupt ofachannel isdisabled.
Write: Noeffect.
1 Read andwrite: LFS interrupt ofthecorresponding channel isdisabled.
20.3.1.32 LFS Interrupt Enable Reset Register (LFSINTENAR)
Figure 20-50. LFS Interrupt Enable Reset Register (LFSINTENAR) [offset =F4h]
31 0
LFSINTDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-40. LFS Interrupt Enable Reset Register (LFSINTENAR) Field Descriptions
Bit Field Value Description
31-0 LFSINTDIS[ n] Last frame started (LFS) interrupt disable. Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 Read: LFS interrupt ofthecorresponding channel isdisabled.
Write: Noeffect.
1 Read: LFS interrupt ofthecorresponding channel isenabled.
Write: LFS interrupt ofthecorresponding channel isdisabled.

<!-- Page 747 -->

www.ti.com Control Registers andControl Packets
747 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.33 HBC Interrupt Enable SetRegister (HBCINTENAS)
Figure 20-51. HBC Interrupt Enable SetRegister (HBCINTENAS) [offset =FCh]
31 0
HBCINTENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-41. HBC Interrupt Enable SetRegister (HBCINTENAS) Field Descriptions
Bit Field Value Description
31-0 HBCINTENA[ n] Half block complete (HBC) interrupt enable. Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 Read: HBC interrupt ofthecorresponding channel isdisabled.
Write: Noeffect.
1 Read andwrite: HBC interrupt ofthecorresponding channel isenabled.
20.3.1.34 HBC Interrupt Enable Reset Register (HBCINTENAR)
Figure 20-52. HBC Interrupt Enable Reset Register (HBCINTENAR) [offset =104h]
31 0
HBCINTDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-42. HBC Interrupt Enable Reset Register (HBCINTENAR) Field Descriptions
Bit Field Value Description
31-0 HBCINTDIS[ n] Half block complete (HBC) interrupt disable. Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 Read: HBC interrupt ofthecorresponding channel isdisabled.
Write: Noeffect.
1 Read: HBC interrupt ofthecorresponding channel isenabled.
Write: HBC interrupt ofthecorresponding channel isdisabled.

<!-- Page 748 -->

Control Registers andControl Packets www.ti.com
748 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.35 BTC Interrupt Enable SetRegister (BTCINTENAS)
Figure 20-53. BTC Interrupt Enable SetRegister (BTCINTENAS) [offset =10Ch]
31 0
BTCINTENA[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-43. BTC Interrupt Enable Reset Register (BTCINTENAS) Field Descriptions
Bit Field Value Description
31-0 BTCINTENA[ n] Block transfer complete (BTC) interrupt enable. Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 Read: BTC interrupt ofthecorresponding channel isdisabled.
Write: Noeffect.
1 Read andwrite: BTC interrupt ofthecorresponding channel isenabled.
20.3.1.36 BTC Interrupt Enable Reset Register (BTCINTENAR)
Figure 20-54. BTC Interrupt Enable Reset Register (BTCINTENAR) [offset =114h]
31 0
BTCINTDIS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-44. BTC Interrupt Enable Reset Register (BTCINTENAR) Field Descriptions
Bit Field Value Description
31-0 BTCINTDIS[ n] Block transfer complete (BTC) interrupt disable. Bit0corresponds tochannel 0,bit1corresponds
tochannel 1,andsoon.
0 Read: BTC interrupt ofthecorresponding channel isdisabled.
Write: Noeffect.
1 Read: BTC interrupt ofthecorresponding channel isenabled.
Write: BTC interrupt ofthecorresponding channel isdisabled.

<!-- Page 749 -->

www.ti.com Control Registers andControl Packets
749 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.37 Global Interrupt Flag Register (GINTFLAG)
Figure 20-55. Global Interrupt Flag Register (GINTFLAG) [offset =11Ch]
31 0
GINT[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-45. Global Interrupt Flag Register (GINTFLAG) Field Descriptions
Bit Field Value Description
31-0 GINT[ n] Global interrupt flags. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andsoon.A
global interrupt flagbitisanORfunction ofFTC, LFS, HBC, andBTC interrupt flags.
0 Nointerrupt ispending onthecorresponding channel.
1 One ormore oftheinterrupt types (FTC, LFS, HBC, orBTC) ispending onthecorresponding channel.
20.3.1.38 FTC Interrupt Flag Register (FTCFLAG)
Figure 20-56. FTC Interrupt Flag Register (FTCFLAG) [offset =124h]
31 0
FTCI[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 20-46. FTC Interrupt Flag Register (FTCFLAG) Field Descriptions
Bit Field Value Description
31-0 FTCI[ n] Frame transfer complete (FTC) flags. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,
andsoon.
Note: Reading from therespective interrupt channel offset register also clears the
corresponding flag(see Section 20.3.1.43 andSection 20.3.1.47 ).
Note: Thestate oftheflagbitcanbepolled even ifthecorresponding interrupt enable bitis
cleared.
0 Read: FTC interrupt ofthecorresponding channel isnotpending.
Write: Noeffect.
1 Read: FTC interrupt ofthecorresponding channel ispending.
Write: The flagiscleared.

<!-- Page 750 -->

Control Registers andControl Packets www.ti.com
750 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.39 LFS Interrupt Flag Register (LFSFLAG)
Figure 20-57. LFS Interrupt Flag Register (LFSFLAG) [offset =12Ch]
31 0
LFSI[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write;W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 20-47. LFS Interrupt Flag Register (LFSFLAG) Field Descriptions
Bit Field Value Description
31-0 LFSI[ n] Last frame started (LFS) flags. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,andso
on.
Note: Reading from therespective interrupt channel offset register also clears the
corresponding flag(see Section 20.3.1.44 andSection 20.3.1.48 ).
Note: Thestate oftheflagbitcanbepolled even ifthecorresponding interrupt enable bitis
cleared.
0 Read: LFS interrupt ofthecorresponding channel isnotpending.
Write: Noeffect.
1 Read: LFS interrupt ofthecorresponding channel ispending.
Write: The flagiscleared.
20.3.1.40 HBC Interrupt Flag Register (HBCFLAG)
Figure 20-58. HBC Interrupt Flag Register (HBCFLAG) [offset =134h]
31 0
HBCI[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 20-48. HBC Interrupt Flag Register (HBCFLAG) Field Descriptions
Bit Field Value Description
31-0 HBCI[ n] Half block transfer (HBC) complete flags. Bit0corresponds tochannel 0,bit1corresponds tochannel
1,andsoon.
Note: Reading from therespective interrupt channel offset register also clears the
corresponding flag(see Section 20.3.1.45 andSection 20.3.1.49 ).
Note: Thestate oftheflagbitcanbepolled even ifthecorresponding interrupt enable bitis
cleared.
0 Read: HBC interrupt ofthecorresponding channel isnotpending.
Write: Noeffect.
1 Read: HBC interrupt ofthecorresponding channel ispending.
Write: The flagiscleared.

<!-- Page 751 -->

www.ti.com Control Registers andControl Packets
751 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.41 BTC Interrupt Flag Register (BTCFLAG)
Figure 20-59. BTC Interrupt Flag Register (BTCFLAG) [offset =13Ch]
31 0
BTCI[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 20-49. BTC Interrupt Flag Register (BTCFLAG) Field Descriptions
Bit Field Value Description
31-0 BTCI[ n] Block transfer complete (BTC) flags. Bit0corresponds tochannel 0,bit1corresponds tochannel 1,
andsoon.
Note: Reading from therespective interrupt channel offset register also clears the
corresponding flag(see Section 20.3.1.46 andSection 20.3.1.50 ).
Note: Thestate oftheflagbitcanbepolled even ifthecorresponding interrupt enable bitis
cleared.
0 Read: BTC interrupt ofthecorresponding channel isnotpending.
Write: Noeffect.
1 Read: BTC interrupt ofthecorresponding channel ispending.
Write: The flagiscleared.
20.3.1.42 BER Interrupt Flag Register (BERFLAG)
The BERFLAG willnever besetinthisdevice. The buserror reporting ishandled bytheDMA Read
Imprecise Error andDMA Write Imprecise Error asserted totheESM module directly, which aredetected
atthedevice level. See theESM error mapping fortheDMA Read/Write Imprecise Error.

<!-- Page 752 -->

Control Registers andControl Packets www.ti.com
752 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.43 FTCA Interrupt Channel Offset Register (FTCAOFFSET)
Figure 20-60. FTCA Interrupt Channel Offset Register (FTCAOFFSET) [offset =14Ch]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz FTCA
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-50. FTCA Interrupt Channel Offset Register (FTCAOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 FTCA Channel causing FTC interrupt Group A.These bitscontain thechannel number ofthepending interrupt
forGroup Aifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.38 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group A.
: :
20h Channel 31iscausing thepending interrupt Group A.
21h-
3FhReserved

<!-- Page 753 -->

www.ti.com Control Registers andControl Packets
753 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.44 LFSA Interrupt Channel Offset Register (LFSAOFFSET)
Figure 20-61. LFSA Interrupt Channel Offset Register (LFSAOFFSET) [offset =150h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz LFSA
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-51. LFSA Interrupt Channel Offset Register (LFSAOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 LFSA Channel causing LFS interrupt Group A.These bitscontain thechannel number ofthepending interrupt
forGroup Aifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.39 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group A.
: :
20h Channel 31iscausing thepending interrupt Group A.
21h-
3FhReserved

<!-- Page 754 -->

Control Registers andControl Packets www.ti.com
754 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.45 HBCA Interrupt Channel Offset Register (HBCAOFFSET)
Figure 20-62. HBCA Interrupt Channel Offset Register (HBCAOFFSET) [offset =154h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz HBCA
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-52. HBCA Interrupt Channel Offset Register (HBCAOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 HBCA Channel causing HBC interrupt Group A.These bitscontain thechannel number ofthepending
interrupt forGroup Aifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.40 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group A.
: :
20h Channel 31iscausing thepending interrupt Group A.
21h-
3FhReserved

<!-- Page 755 -->

www.ti.com Control Registers andControl Packets
755 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.46 BTCA Interrupt Channel Offset Register (BTCAOFFSET)
Figure 20-63. BTCA Interrupt Channel Offset Register (BTCAOFFSET) [offset =158h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz BTCA
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-53. BTCA Interrupt Channel Offset Register (BTCAOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 BTCA Channel causing BTC interrupt Group A.These bitscontain thechannel number ofthepending
interrupt forGroup Aifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.41 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group A.
: :
20h Channel 31iscausing thepending interrupt Group A.
21h-
3FhReserved

<!-- Page 756 -->

Control Registers andControl Packets www.ti.com
756 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.47 FTCB Interrupt Channel Offset Register (FTCBOFFSET)
Figure 20-64. FTCB Interrupt Channel Offset Register (FTCBOFFSET) [offset =160h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz FTCB
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-54. FTCB Interrupt Channel Offset Register (FTCBOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 FTCB Channel causing FTC interrupt Group B.These bitscontain thechannel number ofthepending interrupt
forGroup Bifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.38 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group B.
: :
20h Channel 31iscausing thepending interrupt Group B.
21h-
3FhReserved

<!-- Page 757 -->

www.ti.com Control Registers andControl Packets
757 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.48 LFSB Interrupt Channel Offset Register (LFSBOFFSET)
Figure 20-65. LFSB Interrupt Channel Offset Register (LFSBOFFSET) [offset =164h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz LFSB
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-55. LFSB Interrupt Channel Offset Register (LFSBOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 LFSB Channel causing LFS interrupt Group B.These bitscontain thechannel number ofthepending interrupt
forGroup Bifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.39 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group B.
: :
20h Channel 31iscausing thepending interrupt Group B.
21h-
3FhReserved

<!-- Page 758 -->

Control Registers andControl Packets www.ti.com
758 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.49 HBCB Interrupt Channel Offset Register (HBCBOFFSET)
Figure 20-66. HBCB Interrupt Channel Offset Register (HBCBOFFSET) [offset =168h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz HBCB
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-56. HBCB Interrupt Channel Offset Register (HBCBOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 HBCB Channel causing HBC interrupt Group B.These bitscontain thechannel number ofthepending
interrupt forGroup Bifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.40 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group B.
: :
20h Channel 31iscausing thepending interrupt Group B.
21h-
3FhReserved

<!-- Page 759 -->

www.ti.com Control Registers andControl Packets
759 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.50 BTCB Interrupt Channel Offset Register (BTCBOFFSET)
Figure 20-67. BTCB Interrupt Channel Offset Register (BTCBOFFSET) [offset =16Ch]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz BTCB
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-57. BTCB Interrupt Channel Offset Register (BTCBOFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 BTCB Channel causing BTC interrupt Group B.These bitscontain thechannel number ofthepending
interrupt forGroup Bifthecorresponding interrupt enable isset.
Note: Reading thislocation clears thecorresponding interrupt pending flag(see
Section 20.3.1.41 )with thehighest priority.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt Group B.
: :
20h Channel 31iscausing thepending interrupt Group B.
21h-
3FhReserved

<!-- Page 760 -->

Control Registers andControl Packets www.ti.com
760 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.51 Port Control Register (PTCRL)
Figure 20-68. Port Control Register (PTCRL) [offset =178h]
31 25 24
Reserved PENDB
R-0 R-0
23 19 18 17 16
Reserved BYB Reserved
R-0 R/WP-0 R-0
15 9 8 7 3 2 1 0
Reserved PENDA Reserved BYA PSFRHQ PSFRLQ
R-0 R-0 R-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-58. Port Control Register (PTCRL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 PENDB Transfers pending forPort B.This flagdetermines iftransfers areongoing onport B.The flagwill
becleared ifnotransfers areperformed. Itcanbeused todetermine ifthere isstilldata transferred
while DMA_EN iscleared to0inGCTCRL. Inthiscase, once alltransfers arefinished, theflagwill
becleared to0.
0 Notransfers arepending.
1 Transfers arepending.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 BYB Bypass FIFO B.
0 FIFO Bisnotbypassed.
1 FIFO Bisbypassed. Writing 1tothisbitlimits theFIFO depth tothesize ofoneelement. That
means thatafter oneelement isread, thewrite-out tothedestination willbegin. This feature is
particularly useful tominimize switching latency between channels.
Note: This feature does notmake optimal useofbusbandwidth.
17-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PENDA Transfers pending forPort A.This flagdetermines iftransfers areongoing onport A.The flagwill
becleared ifnotransfers areperformed. Itcanbeused todetermine ifthere isstilldata transferred
while DMA_EN iscleared to0inGCTCRL. Inthiscase, once alltransfers arefinished, theflagwill
becleared to0.
0 Notransfers arepending.
1 Transfers arepending.
7-3 Reserved 0 Reads return 0.Writes have noeffect.
2 BYA Bypass FIFO A.
0 FIFO Aisnotbypassed.
1 FIFO Aisbypassed. Writing 1tothisbitlimits theFIFO depth tothesize ofoneelement. That
means thatafter oneelement isread, thewrite-out tothedestination willbegin. This feature is
particularly useful tominimize switching latency between channels.
Note: This feature does notmake optimal useofbusbandwidth.
1 PSFRHQ Priority scheme fixorrotate forhigh priority queue.
0 Fixed priority isused.
1 Rotation priority isused.
0 PSFRLQ Priority scheme fixorrotate forlowpriority queue.
0 The fixed priority scheme isused.
1 The rotation priority scheme isused.

<!-- Page 761 -->

www.ti.com Control Registers andControl Packets
761 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.52 RAM Test Control Register (RTCTRL)
Figure 20-69. RAM Test Control Register (RTCTRL) [offset =17Ch]
31 16
Reserved
R-0
15 1 0
Reserved RTC
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-59. RAM Test Control Register (RTCTRL) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RTC RAM Test Control. Writing a1tothisbitopens thewrite access tothereserved locations ofcontrol
packet RAM asdefined inthememory-map.
Note: This bitshould becleared to0during normal operation.
0 RAM Test Control isdisabled.
1 RAM Test Control isenabled.

<!-- Page 762 -->

Control Registers andControl Packets www.ti.com
762 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.53 Debug Control Register (DCTRL)
Figure 20-70. Debug Control Register (DCTRL) [offset =180h]
31 29 28 24 23 17 16
Reserved CHNUM Reserved DMADBGS
R-0 R-0 R-0 R/W1C-0
15 1 0
Reserved DBGEN
R-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 20-60. Debug Control Register (DCTRL) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-24 CHNUM 0-1Fh Channel Number. This bitfield indicates thechannel number thatcauses thewatch point tomatch.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 DMADBGS DMA debug status. When awatch point issetuptowatch foraunique busaddress orarange of
addresses istrue ononeofthethree busports, then theDMA debug status bitissetto1anda
debug request signal isasserted tothemain CPU. The CPU must write a1toclear thisbitforthe
DMA controller torelease thedebug request signal.
0 Read: Nowatch point condition isdetected.
Write: Noeffect.
1 Read: The watch point condition isdetected.
Write: The bitiscleared.
15-1 Reserved 0 Reads return 0.Writes have noeffect.
0 DBGEN Debug Enable.
Note: This bitcanonly besetwhen using adebugger.
Note: This bitisreset when Test reset (TRST) islow.
0 Debug isdisabled.
1 The watch point checking logics isenabled.

<!-- Page 763 -->

www.ti.com Control Registers andControl Packets
763 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.54 Watch Point Register (WPR)
Figure 20-71. Watch Point Register (WPR) [offset =184h]
31 0
WP
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 20-61. Watch Point Register (WPR) Field Descriptions
Bit Field Description
31-0 WP Watch point.
Note: These bitscanonly besetwhen using adebugger.
This register isonly reset byatestreset (TRST). A32-bit address canbeprogrammed intothisregister asa
watch point. This register isused with thewatch mask register (WMR).
When theDBGEN bitintheDCTRL register issetandaunique address orarange ofaddresses aredetected
ontheAHB address busofPort B,adebug request signal issent totheARM CPU. The state machine ofthe
port inwhich thewatch point condition istrue isfrozen.
20.3.1.55 Watch Mask Register (WMR)
Figure 20-72. Watch Mask Register (WMR) [offset =188h]
31 0
WM[31:0]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 20-62. Watch Mask Register (WMR) Field Descriptions
Bit Field Value Description
31-0 WM[n] Watch mask.
Note: These bitscanonly besetwhen using adebugger.
This register isonly reset byatestreset (TRST).
0 Allows thebitintheWPR register tobeused foraddress matching forawatch point.
1 Masks thecorresponding bitintheWPR register andisdisregarded inthecomparison.

<!-- Page 764 -->

Control Registers andControl Packets www.ti.com
764 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.56 FIFO AActive Channel Source Address Register (FAACSADDR)
Figure 20-73. FIFO AActive Channel Source Address Register (FAACSADDR) [offset =18Ch]
31 0
FAACSA
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-63. FIFO AActive Channel Source Address Register (FAACSADDR) Field Descriptions
Bit Field Description
31-0 FAACSA FIFO BActive Channel Source Address. This register contains thecurrent source address oftheactive
channel asbroadcasted inSection 20.3.1.3 forFIFO B.
20.3.1.57 FIFO AActive Channel Destination Address Register (FAACDADDR)
Figure 20-74. FIFO AActive Channel Destination Address Register (FAACDADDR) [offset =190h]
31 0
FAACDA
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-64. FIFO AActive Channel Destination Address Register (FAACDADDR)
Field Descriptions
Bit Field Description
31-0 FAACDA FIFO AActive Channel Destination Address. This register contains thecurrent destination address oftheactive
channel asbroadcasted inSection 20.3.1.3 forFIFO A.
20.3.1.58 FIFO AActive Channel Transfer Count Register (FAACTC)
Figure 20-75. FIFO AActive Channel Transfer Count Register (FAACTC) [offset =194h]
31 29 28 16
Reserved FAFTCOUNT
R-0 R-0
15 13 12 0
Reserved FAETCOUNT
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-65. Port BActive Channel Transfer Count Register (FAACTC) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-16 FAFTCOUNT 0-1FFFh FIFO Aactive channel frame count. These bitscontain thecurrent frame count value ofthe
active channel asbroadcasted inSection 20.3.1.3 forFIFO A.
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12-0 FAETCOUNT 0-1FFFh FIFO Aactive channel element count. These bitscontain thecurrent element count value of
theactive channel asbroadcasted inSection 20.3.1.3 forFIFO A.

<!-- Page 765 -->

www.ti.com Control Registers andControl Packets
765 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.59 FIFO BActive Channel Source Address Register (FBACSADDR)
Figure 20-76. FIFO BActive Channel Source Address Register (FBACSADDR) [offset =198h]
31 0
FBACSA
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-66. FIFO BActive Channel Source Address Register (FBACSADDR) Field Descriptions
Bit Field Description
31-0 FBACSA FIFO BActive Channel Source Address. This register contains thecurrent source address oftheactive
channel asbroadcasted inSection 20.3.1.3 forFIFO B.
20.3.1.60 FIFO BActive Channel Destination Address Register (FBACDADDR)
Figure 20-77. FIFO BActive Channel Destination Address Register (FBACDADDR) [offset =19Ch]
31 0
FBACDA
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-67. FIFO BActive Channel Destination Address Register (FBACDADDR)
Field Descriptions
Bit Field Description
31-0 FBACDA FIFO BActive Channel Destination Address. This register contains thecurrent destination address oftheactive
channel asbroadcasted inSection 20.3.1.3 forFIFO B.
20.3.1.61 FIFO BActive Channel Transfer Count Register (FBACTC)
Figure 20-78. FIFO BActive Channel Transfer Count Register (FBACTC) [offset =1A0h]
31 29 28 16
Reserved FBFTCOUNT
R-0 R-0
15 13 12 0
Reserved FBETCOUNT
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 20-68. FIFO BActive Channel Transfer Count Register (FBACTC) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-16 FBFTCOUNT 0-1FFFh FIFO Bactive channel frame count. These bitscontain thecurrent frame count value ofthe
active channel asbroadcasted inSection 20.3.1.3 forFIFO B.
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12-0 FBETCOUNT 0-1FFFh FIFO Bactive channel element count. These bitscontain thecurrent element count value of
theactive channel asbroadcasted inSection 20.3.1.3 forFIFO B.

<!-- Page 766 -->

Control Registers andControl Packets www.ti.com
766 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.62 ECC Control Register (DMAPECR)
Figure 20-79. ECC Control Register (DMAPECR) [offset =1A8h]
31 15 16
Reserved ERRA
R-0 R/WP-0
15 9 8 7 4 3 0
Reserved TEST Reserved ECC_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-69. ECC Control Register (DMAPECR) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 ERRA Error action.
0 Ifaparity error isdetected oncontrol packet x(x=0,1,...n),then theenable/disable state of
control packet xremains unchanged.
1 Ifaparity error isdetected oncontrol packet x(x=0,1,...n), then theDMA controller isdisabled
immediately. Ifaframe oncontrol packet xisprocessed atthetime theparity error isdetected, then
remaining elements ofthisframe willnotbetransferred anymore. The DMA willbedisabled
regardless ofwhether theerror was detected during aread tothecontrol packet RAM performed by
theDMA state machine orbyadifferent master.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 TEST When thisbitisset,theparity bitsarememory-mapped tomake them accessible bytheCPU.
0 The parity bitsarenotmemory-mapped.
1 The parity bitsarememory-mapped.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 ECC_ENA ECC enable. This bitfield enables ordisables theECC check onread operations andtheECC
calculation onwrite operations. IfECC checking isenabled andanECC double-bit error isdetected
theDMA_UERR signal isactivated.
5h The ECC check isdisabled.
Allother
valuesThe ECC check isenabled.
Note: Itisrecommended towrite Ahtoenable ECC check, toguard against softerror from
flipping ECC_ENA toadisable state.

<!-- Page 767 -->

www.ti.com Control Registers andControl Packets
767 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.63 DMA ECC Error Address Register (DMAPAR)
Figure 20-80. DMA ECC Error Address Register (DMAPAR) [offset =1ACh]
31 25 24 23 16
Reserved EDFLAG Reserved
R-0 R/W1C-0 R-0
15 12 11 0
Reserved ERRORADDRESS
R-0 R-X
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; X=value isundefined; -n=value after reset
Table 20-70. DMA ECC Error Address Register (DMAPAR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 EDFLAG ECC Error Detection Flag. This flagindicates ifanECC error occurred onreading DMA Control
packet RAM.
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Error detected andtheaddress iscaptured inDMAPAR 'sERROR_ADDRESS field.
Write: Clears thebit.
23-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 ERRORADDRESS 0-FFFh Error address. These bitshold theaddress ofthefirstECC error generated intheRAM. This
error address isfrozen from being updated until itisread bytheCPU. During emulation mode
when SUSPEND ishigh, thisaddress isfrozen even when read.
Note: Theerror address register willnotbereset byPORRST norbyanyother reset
source.

<!-- Page 768 -->

Control Registers andControl Packets www.ti.com
768 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.64 DMA Memory Protection Control Register 1(DMAMPCTRL1)
Figure 20-81. DMA Memory Protection Control Register 1(DMAMPCTRL1) [offset =1B0h]
31 29 28 27 26 25 24
Reserved INT3AB INT3ENA REG3AP REG3ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
23 21 20 19 18 17 16
Reserved INT2AB INT2ENA REG2AP REG2ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 13 12 11 10 9 8
Reserved INT1AB INT1ENA REG1AP REG1ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 5 4 3 2 1 0
Reserved INT0AB INT0ENA REG0AP REG0ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-71. DMA Memory Protection Control Register 1(DMAMPCTRL1) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28 INT3AB Interrupt assignment ofregion 3toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
27 INT3ENA Interrupt enable ofregion 3.
0 The interrupt isdisabled.
1 The interrupt isenabled.
26-25 REG3AP Region 3access permission. These bitsdetermine theaccess permission forregion 3.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
24 REG3ENA Region 3enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 INT2AB Interrupt assignment ofregion 2toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
19 INT2ENA Interrupt enable ofregion 2.
0 The interrupt isdisabled.
1 The interrupt isenabled.
18-17 REG2AP Region 2access permission. These bitsdetermine theaccess permission forregion 2.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
16 REG2ENA Region 2enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).

<!-- Page 769 -->

www.ti.com Control Registers andControl Packets
769 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-71. DMA Memory Protection Control Register 1(DMAMPCTRL1) Field Descriptions (continued)
Bit Field Value Description
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12 INT1AB Interrupt assignment ofregion 1toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
11 INT1ENA Interrupt enable ofregion 1.
0 The interrupt isdisabled.
1 The interrupt isenabled.
10-9 REG1AP Region 1access permission. These bitsdetermine theaccess permission forregion 3.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
8 REG1ENA Region 1enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).
7-5 Reserved 0 Reads return zeros andwrites have noeffect.
4 INT0AB Interrupt assignment ofregion 0toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
3 INT0ENA Interrupt enable ofregion 0.
0 The interrupt isdisabled.
1 The interrupt isenabled.
2-1 REG0AP Region 0access permission. These bitsdetermine theaccess permission forregion 0.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
0 REG0ENA Region 0enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).

<!-- Page 770 -->

Control Registers andControl Packets www.ti.com
770 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.65 DMA Memory Protection Status Register 1(DMAMPST1)
Figure 20-82. DMA Memory Protection Status Register 1(DMAMPST1) [offset =1B4h]
31 25 24 23 17 16
Reserved REG3FT Reserved REG2FT
R-0 R/W1C-0 R-0 R/W1C-0
15 9 8 7 1 0
Reserved REG1FT Reserved REG0FT
R-0 R/W1C-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 20-72. DMA Memory Protection Status Register 1(DMAMPST1) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 REG3FT Region 3fault. This bitdetermines whether anaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: The bitwas cleared.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 REG2FT Region 2fault. This bitdetermines whether aaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: The bitwas cleared.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 REG1FT Region 1fault. This bitdetermines whether anaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: The bitwas cleared.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 REG0FT Region 0fault. This bitdetermines whether aaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: The bitwas cleared.

<!-- Page 771 -->

www.ti.com Control Registers andControl Packets
771 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.66 DMA Memory Protection Region 0Start Address Register (DMAMPR0S)
Figure 20-83. DMA Memory Protection Region 0Start Address Register (DMAMPR0S)
[offset =1B8h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-73. DMA Memory Protection Region 0Start Address Register (DMAMPR0S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.67 DMA Memory Protection Region 0End Address Register (DMAMPR0E)
Figure 20-84. DMA Memory Protection Region 0End Address Register (DMAMPR0E)
[offset =1BCh]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-74. DMA Memory Protection Region 0End Address Register (DMAMPR0E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 772 -->

Control Registers andControl Packets www.ti.com
772 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.68 DMA Memory Protection Region 1Start Address Register (DMAMPR1S)
Figure 20-85. DMA Memory Protection Region 1Start Address Register (DMAMPR1S)
[offset =1C0h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-75. DMA Memory Protection Region 1Start Address Register (DMAMPR1S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.69 DMA Memory Protection Region 1End Address Register (DMAMPR1E)
Figure 20-86. DMA Memory Protection Region 1End Address Register (DMAMPR1E)
[offset =1C4h]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-76. DMA Memory Protection Region 1End Address Register (DMAMPR1E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 773 -->

www.ti.com Control Registers andControl Packets
773 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.70 DMA Memory Protection Region 2Start Address Register (DMAMPR2S)
Figure 20-87. DMA Memory Protection Region 2Start Address Register (DMAMPR2S)
[offset =1C8h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-77. DMA Memory Protection Region 2Start Address Register (DMAMPR2S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.71 DMA Memory Protection Region 2End Address Register (DMAMPR2E)
Figure 20-88. DMA Memory Protection Region 2End Address Register (DMAMPR2E)
[offset =1CCh]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-78. DMA Memory Protection Region 2End Address Register (DMAMPR2E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 774 -->

Control Registers andControl Packets www.ti.com
774 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.72 DMA Memory Protection Region 3Start Address Register (DMAMPR3S)
Figure 20-89. DMA Memory Protection Region 3Start Address Register (DMAMPR3S)
[offset =1D0h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-79. DMA Memory Protection Region 3Start Address Register (DMAMPR3S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.73 DMA Memory Protection Region 3End Address Register (DMAMPR3E)
Figure 20-90. DMA Memory Protection Region 3End Address Register (DMAMPR3E)
[offset =1D4h]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-80. DMA Memory Protection Region 3End Address Register (DMAMPR3E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 775 -->

www.ti.com Control Registers andControl Packets
775 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.74 DMA Memory Protection Control Register 2(DMAMPCTRL2)
Figure 20-91. DMA Memory Protection Control Register 2(DMAMPCTRL2) [offset =1D8h]
31 29 28 27 26 25 24
Reserved INT7AB INT7ENA REG7AP REG7ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
23 21 20 19 18 17 16
Reserved INT6AB INT6ENA REG6AP REG6ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 13 12 11 10 9 8
Reserved INT5AB INT5ENA REG5AP REG5ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 5 4 3 2 1 0
Reserved INT4AB INT4ENA REG4AP REG4ENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 20-81. DMA Memory Protection Control Register 2(DMAMPCTRL2) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28 INT7AB Interrupt assignment ofregion 7toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
27 INT7ENA Interrupt enable ofregion 7.
0 The interrupt isdisabled.
1 The interrupt isenabled.
26-25 REG7AP Region 7access permission. These bitsdetermine theaccess permission forregion 7.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
24 REG7ENA Region 7enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 INT6AB Interrupt assignment ofregion 6toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
19 INT6ENA Interrupt enable ofregion 6.
0 The interrupt isdisabled.
1 The interrupt isenabled.
18-17 REG6AP Region 6access permission. These bitsdetermine theaccess permission forregion 6.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
16 REG6ENA Region 6enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).

<!-- Page 776 -->

Control Registers andControl Packets www.ti.com
776 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-81. DMA Memory Protection Control Register 2(DMAMPCTRL2) Field Descriptions (continued)
Bit Field Value Description
15-13 Reserved 0 Reads return 0.Writes have noeffect.
12 INT5AB Interrupt assignment ofregion 5toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
11 INT5ENA Interrupt enable ofregion 5.
0 The interrupt isdisabled.
1 The interrupt isenabled.
10-9 REG5AP Region 5access permission. These bitsdetermine theaccess permission forregion 5.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
8 REG5ENA Region 5enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).
7-5 Reserved 0 Reads return zeros andwrites have noeffect.
4 INT4AB Interrupt assignment ofregion 4toGroup AorGroup B.
0 The interrupt isrouted totheVIM (Group A).
1 The interrupt isrouted tothesecond CPU (Group B).
3 INT4ENA Interrupt enable ofregion 4.
0 The interrupt isdisabled.
1 The interrupt isenabled.
2-1 REG4AP Region 4access permission. These bitsdetermine theaccess permission forregion 4.
0 Allaccesses areallowed.
1h Read only accesses areallowed.
2h Write only accesses areallowed.
3h Noaccesses areallowed.
0 REG4ENA Region 4enable.
0 The region isdisabled (noaddress checking done).
1 The region isenabled (address andaccess permission checking done).

<!-- Page 777 -->

www.ti.com Control Registers andControl Packets
777 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.75 DMA Memory Protection Status Register 2(DMAMPST2)
Figure 20-92. DMA Memory Protection Status Register 2(DMAMPST2) [offset =1DCh]
31 25 24 23 17 16
Reserved REG7FT Reserved REG6FT
R-0 R/W1C-0 R-0 R/W1C-0
15 9 8 7 1 0
Reserved REG5FT Reserved REG4FT
R-0 R/W1C-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 20-82. DMA Memory Protection Status Register 2(DMAMPST2) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 REG7FT Region 7fault. This bitdetermines whether anaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: Clears thebit.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 REG6FT Region 6fault. This bitdetermines whether aaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: Clears thebit.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 REG5FT Region 5fault. This bitdetermines whether anaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: Clears thebit.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 REG4FT Region 4fault. This bitdetermines whether aaccess permission violation was detected inthisregion.
0 Read: Nofault was detected.
Write: Noeffect.
1 Read: Afault was detected.
Write: Clears thebit.

<!-- Page 778 -->

Control Registers andControl Packets www.ti.com
778 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.76 DMA Memory Protection Region 4Start Address Register (DMAMPR4S)
Figure 20-93. DMA Memory Protection Region 4Start Address Register (DMAMPR4S)
[offset =1E0h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-83. DMA Memory Protection Region 4Start Address Register (DMAMPR4S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.77 DMA Memory Protection Region 4End Address Register (DMAMPR4E)
Figure 20-94. DMA Memory Protection Region 4End Address Register (DMAMPR4E)
[offset =1E4h]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-84. DMA Memory Protection Region 4End Address Register (DMAMPR4E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 779 -->

www.ti.com Control Registers andControl Packets
779 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.78 DMA Memory Protection Region 5Start Address Register (DMAMPR5S)
Figure 20-95. DMA Memory Protection Region 5Start Address Register (DMAMPR5S)
[offset =1E8h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-85. DMA Memory Protection Region 5Start Address Register (DMAMPR5S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.79 DMA Memory Protection Region 5End Address Register (DMAMPR5E)
Figure 20-96. DMA Memory Protection Region 5End Address Register (DMAMPR5E)
[offset =1ECh]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-86. DMA Memory Protection Region 5End Address Register (DMAMPR5E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 780 -->

Control Registers andControl Packets www.ti.com
780 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.80 DMA Memory Protection Region 6Start Address Register (DMAMPR6S)
Figure 20-97. DMA Memory Protection Region 6Start Address Register (DMAMPR6S)
[offset =1F0h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-87. DMA Memory Protection Region 6Start Address Register (DMAMPR6S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.81 DMA Memory Protection Region 6End Address Register (DMAMPR6E)
Figure 20-98. DMA Memory Protection Region 6End Address Register (DMAMPR6E)
[offset =1F4h]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-88. DMA Memory Protection Region 6End Address Register (DMAMPR6E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 781 -->

www.ti.com Control Registers andControl Packets
781 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.82 DMA Memory Protection Region 7Start Address Register (DMAMPR7S)
Figure 20-99. DMA Memory Protection Region 7Start Address Register (DMAMPR7S)
[offset =1F8h]
31 0
STARTADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-89. DMA Memory Protection Region 7Start Address Register (DMAMPR7S)
Field Descriptions
Bit Field Description
31-0 STARTADDRESS Start Address defines theaddress atwhich theregion begins. The effective start address istruncated
tothenearest word address, thatis,0x103 =0x100.
20.3.1.83 DMA Memory Protection Region 7End Address Register (DMAMPR7E)
Figure 20-100. DMA Memory Protection Region 7End Address Register (DMAMPR7E)
[offset =1FCh]
31 0
ENDADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-90. DMA Memory Protection Region 7End Address Register (DMAMPR7E)
Field Descriptions
Bit Field Description
31-0 ENDADDRESS End Address defines theaddress atwhich theregion ends. The endaddress usually islarger than the
start address forthisregion; otherwise, theregion willwrap around attheendoftheaddress space.
The endaddress isthestart address plus theregion length minus 1.The effective endaddress is
rounded uptothenearest 32-bit word endaddress, thatis,0x200 =0x203.
Note: When using 64-bit transfers, theaddress isrounded uptothenearest 64-bit word end
address, thatis,0x200 =0x207. Allother transfers arerounded uptothenearest 32-bit word
endaddress.

<!-- Page 782 -->

Control Registers andControl Packets www.ti.com
782 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.84 DMA Single-Bit ECC Control Register (DMASECCCTRL)
Figure 20-101. DMA Single-Bit ECC Control Register (DMASECCCTRL) [offset =228h]
31 17 16
Reserved SBERR
R-0 R/W1CP-0
15 12 11 8 7 4 3 0
Reserved SBE_EVT_EN Reserved EDACMODE
R-0 R/WP-5h R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; WP=Write inprivilege mode only; -n=value
after reset
Table 20-91. DMA Single-Bit ECC Control Register (DMASECCCTRL) Field Description
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 SBERR Error action.
0 Read: NoRAM check error hasoccurred.
Write: Noeffect.
1 Read: Asingle-bit error hasoccurred andwas corrected bytheSECDED logic.
Write: Clears thebit.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 SBE_EVT_EN Single-bit error enable.
5h Disable generation ofsingle-bit error toESM.
Ah Enable generation ofsingle-bit error toESM.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EDACMODE 5h Disable correction ofSBE detected bytheSECDED block.
Ah Enable correction ofSBE detected bytheSECDED block.

<!-- Page 783 -->

www.ti.com Control Registers andControl Packets
783 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.85 DMA ECC Single-Bit Error Address Register (DMAECCSBE)
Figure 20-102. DMA ECC Single-Bit Error Address Register (DMAECCSBE) [offset =230h]
31 16
Reserved
R-0
15 12 11 0
Reserved ERRORADDRESS
R-0 R-X
LEGEND: R=Read only; X=value isundefined; -n=value after reset
.
Table 20-92. DMA ECC Single-Bit Error Address Register (DMAECCSBE) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 ERRORADDRESS 0-FFFh The DMA RAM address (offset from base address word aligned) oftheECC error location.
This register gives theaddress ofthefirstencountered single-bit ECC error since the
SBERR flaghasbeen clear. Subsequent single-bit ECC errors willnotupdate thisregister
until theSBERR flaghasbeen cleared. This register isvalid only when theSBERR flagis
set.
Read: This register clears to0x0000 once itisread bytheCPU. Foraread issued bythe
debugger thisaddress isfrozen even when read.
Write: Noeffect
Note: Theerror address register willnotbereset byPORRST norbyanyother reset
source.

<!-- Page 784 -->

Control Registers andControl Packets www.ti.com
784 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.86 FIFO AStatus Register (FIFOASTAT)
Figure 20-103. FIFO AStatus Register (FIFOASTAT) [offset =240h]
31 0
FFACH[31:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-93. FIFO AStatus Register (FIFOASTAT) Field Descriptions
Bit Field Value Description
31-0 FFACH[ n] Status ofDMA channel running using FIFO A.Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 The channel isnotbeing currently processed.
1 The channel iscurrently being processed using FIFO A.
Note: The status ofachannel currently being processed remains active, even ifemulation mode is
entered orDMA isdisabled byway oftheDMA_EN bit.Upto1bitcanbesetinthisregister atany
given time.
20.3.1.87 FIFO BStatus Register (FIFOBSTAT)
Figure 20-104. FIFO BStatus Register (FIFOBSTAT) [offset =244h]
31 0
FFBCH[31:0]
R-0
LEGEND: R=Read only; -n=value after reset
Table 20-94. FIFO BStatus Register (FIFOBSTAT) Field Descriptions
Bit Field Value Description
31-0 FFBCH[ n] Status ofDMA channel running using FIFO B.Bit0corresponds tochannel 0,bit1corresponds to
channel 1,andsoon.
0 The channel isnotbeing currently processed.
1 The channel iscurrently being processed using FIFO B.
Note: The status ofachannel currently being processed remains active, even ifemulation mode is
entered orDMA isdisabled byway oftheDMA_EN bit.Upto1bitcanbesetinthisregister atany
given time.

<!-- Page 785 -->

www.ti.com Control Registers andControl Packets
785 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.88 DMA Request Polarity Select Register 1(DMAREQPS1)
Figure 20-105. DMA Request Polarity Select Register (DMAREQPS1) [offset =330h]
31 0
DMAREQPS[63:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-95. DMA Request Polarity Select Register (DMAREQPS1) Field Descriptions
Bit Field Value Description
31-0 DMAREQOS[ n] Polarity selection forDMA request lines forupper 32requests, thatis,request lines 63to32.Bit0
corresponds toDMA Request line32,bit1corresponds toDMA Request line33,andsoon.
0 DMA Request polarity isactive high.
1 DMA Request polarity isactive low.
20.3.1.89 DMA Request Polarity Select Register 0(DMAREQPS0)
Figure 20-106. DMA Request Polarity Select Register (DMAREQPS0) [offset =334h]
31 0
DMAREQPS[31:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 20-96. DMA Request Polarity Select Register (DMAREQPS1) Field Descriptions
Bit Field Value Description
31-0 DMAREQOS[ n] Polarity selection forDMA request lines forlower 32requests, thatis,request lines 31to0.Bit0
corresponds toDMA Request line0,bit1corresponds toDMA Request line1,andsoon.
0 DMA Request polarity isactive high.
1 DMA Request polarity isactive low.

<!-- Page 786 -->

Control Registers andControl Packets www.ti.com
786 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.90 Transaction Parity Error Event Control Register (TERECTRL)
Figure 20-107. Transaction Parity Error Event Control Register (TERECTRL) [offset =340h]
31 17 16
Reserved TER_ERR
R-0 R/W1C-0
15 4 3 0
Reserved TER_EN
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; WP=Write inprivilege mode only; -n=value after reset
Table 20-97. Transaction Parity Error Event Control Register (TERECTRL) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 TER_ERR Transactions parity error status.
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Atransaction error hasoccurred
Write: Clears thebit.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 TER_EN Transaction error event detection enable.
5h Write: Disable transaction error event detection byDMA .
Read: Transaction error event willnotbedetected byDMA.
Ah Write: Enable transaction error event detection byDMA.
Read: Transaction error event willbedetected byDMA.
20.3.1.91 TER Event Flag Register (TERFLAG)
Figure 20-108. TER Event Flag Register (TERFLAG) [offset =344h]
31 0
TERE[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 20-98. TER Event Flag Register (TERFLAG) Field Descriptions
Bit Field Value Description
31-0 TERE[ n] Ifthebitisset,aTER event ofthecorresponding channel ispending. Bit0corresponds tochannel
0,bit1corresponds tochannel 1,andsoon.
0 Read: The associated TER Event ofachannel isNOT pending.
Write: Noeffect.
1 Read: The associated TER Event ofachannel ispending.
Write: Clear thisbit.

<!-- Page 787 -->

www.ti.com Control Registers andControl Packets
787 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.1.92 TER Event Channel Offset Register (TERROFFSET)
Figure 20-109. TER Event Channel Offset Register (TERROFFSET) [offset =348h]
31 16
Reserved
R-0
15 8 7 6 5 0
Reserved sbz sbz TER_OFF
R-0 R-0 R-0 R-x
LEGEND: R=Read only; -n=value after reset
Table 20-99. TER Event Channel Offset Register (TERROFFSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-6 sbz 0 These bitsshould always beprogrammed aszero.
5-0 TER_OFF This register provides theoffset ofthefirstchannel number thatencountered busparity failure oneither
port ofDMA. Once thisregister isupdated, itwillnotbechanged bysubsequent busparity failures until
TER_ERR flagiscleared. Writes have noeffect.
0 Nointerrupt ispending.
1h Channel 0iscausing thepending interrupt. (Read clears theregister to0except when issued bya
debugger).
: :
20h Channel 31iscausing thepending interrupt.
21h-
3FhReserved
Note: Ifboth DMA ports encounter busparity failure atthesame time than lower channel number
(assuming higher priority) willbestored andtheother onewillbeignored.

<!-- Page 788 -->

Control Registers andControl Packets www.ti.com
788 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.2 Channel Configuration
The channel configuration isdefined bythechannel control packet: channel control, transfer count, offset
values, source/destination address.
*Itisstored inlocal RAM, which isprotected byparity.
*Each control packet contains atotal ofnine fields.
*The firstsixfields areprogrammable, while thelastthree fields areread only.
*The RAM isaccessible byqueue Aandqueue Bstate machines aswell asCPU.
*When there aresimultaneous accesses, thepriority isresolved inafixed priority scheme with theCPU
having thehighest priority.
Allthecontrol packets look thesame. Following, there isthedetailed layout ofthese registers shown for
control packet 0.
20.3.2.1 Initial Source Address Register (ISADDR)
Figure 20-110. Initial Source Address Register (ISADDR) [offset =00]
31 0
ISADDR
R/WP-X
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset
Table 20-100. Initial Source Address Register (ISADDR) Field Descriptions
Bit Field Description
31-0 ISADDR Initial source address. These bitsgive theabsolute 32-bit source address (physical).
20.3.2.2 Initial Destination Address Register (IDADDR)
Figure 20-111. Initial Destination Address Register (IDADDR) [offset =04h]
31 0
IDADDR
R/WP-X
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset
Table 20-101. Initial Destination Address Register (IDADDR) Field Descriptions
Bit Field Description
31-0 IDADDR Initial destination address. These bitsgive theabsolute 32-bit destination address (physical).

<!-- Page 789 -->

www.ti.com Control Registers andControl Packets
789 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.2.3 Initial Transfer Count Register (ITCOUNT)
Figure 20-112. Initial Transfer Count Register (ITCOUNT) [offset =08h]
31 29 28 16
Reserved IFTCOUNT
R-X R/WP-X
15 13 12 0
Reserved IETCOUNT
R-X R/WP-X
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset
Table 20-102. Initial Transfer Count Register (ITCOUNT) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads areundefined. Writes have noeffect.
28-16 IFTCOUNT 0-1FFFh Initial frame transfer count. These bitsdefine thenumber offrame transfers.
15-13 Reserved 0 Reads areundefined. Writes have noeffect.
12-0 IETCOUNT 0-1FFFh Initial element transfer count. These bitsdefine thenumber ofelement transfers. The block
transfer size willbeIETCOUNT xIFTCOUNT.
20.3.2.4 Channel Control Register (CHCTRL)
Figure 20-113. Channel Control Register (CHCTRL) [offset =10h]
31 22 21 16
Reserved CHAIN
R-X R/WP-X
15 14 13 12 11 9 8 7 5 4 3 2 1 0
RES WES Reserved TTYPE Reserved ADDMR ADDMW AIM
R/WP-X R/WP-X R-X R/WP-X R-X R/WP-X R/WP-X R/WP-X
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset

<!-- Page 790 -->

Control Registers andControl Packets www.ti.com
790 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) ModuleTable 20-103. Channel Control Register (CHCTRL) Field Descriptions
Bit Field Value Description
31-22 Reserved 0 Reads areundefined. Writes have noeffect.
21-16 CHAIN Next channel tobetriggered. Attheendoftheprogrammed number offrames, thespecified
channel willbetriggered.
Note: Theprogrammer must program theCHAIN bitsbefore initiating aDMA transfer.
0 Nochannel isselected.
1h Channel 0isselected.
: :
20h Channel 31isselected.
21h-3Fh Reserved
15-14 RES Read element size.
0 The element isbyte, 8-bit.
1h The element ishalf-word, 16-bit.
2h The element isword, 32-bit.
3h The element isdouble-word, 64-bit.
13-12 WES Write element size.
0 The element isbyte, 8-bit.
1h The element ishalf-word, 16-bit.
2h The element isword, 32-bit.
3h The element isdouble-word, 64-bit.
11-9 Reserved 0 Reads areundefined. Writes have noeffect.
8 TTYPE Transfer type.
0 Arequest triggers oneframe transfer.
1 Arequest triggers oneblock transfer.
7-5 Reserved 0 Reads areundefined. Writes have noeffect.
4-3 ADDMR Addressing mode read.
0 Constant
1h Post-increment
2h Reserved
3h Indexed
2-1 ADDMW Addressing mode write.
0 Constant
1h Post-increment
2h Reserved
3h Indexed
0 AIM Auto-initiation mode.
0 Auto-initiation mode isdisabled.
1 Auto-initiation mode isenabled.

<!-- Page 791 -->

www.ti.com Control Registers andControl Packets
791 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.2.5 Element Index Offset Register (EIOFF)
Figure 20-114. Element Index Offset Register (EIOFF) [offset =14h]
31 29 28 16
Reserved EIDXD
R-X R/WP-X
15 13 12 0
Reserved EIDXS
R-X R/WP-X
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset
Table 20-104. Element Index Offset Register (EIOFF) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads areundefined. Writes have noeffect.
28-16 EIDXD 0-1FFFh Destination address element index. These bitsdefine theoffset tobeadded tothe
destination address after each element transfer.
15-13 Reserved 0 Reads areundefined. Writes have noeffect.
12-0 EIDXS 0-1FFFh Source address element index. These bitsdefine theoffset tobeadded tothesource
address after each element transfer.
20.3.2.6 Frame Index Offset Register (FIOFF)
Figure 20-115. Frame Index Offset Register (FIOFF) [offset =18h]
31 29 28 16
Reserved FIDXD
R-X R/WP-X
15 13 12 0
Reserved FIDXS
R-X R/WP-X
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; X=value isunknown; -n=value after reset
Table 20-105. Frame Index Offset Register (FIOFF) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads areundefined. Writes have noeffect.
28-16 FIDXD 0-1FFFh Destination address frame index. These bitsdefine theoffset tobeadded tothedestination
address after element count reached 1.
15-13 Reserved 0 Reads areundefined. Writes have noeffect.
12-0 FIDXS 0-1FFFh Source address frame index. These bitsdefine theoffset tobeadded tothesource address
after element count reached 1.

<!-- Page 792 -->

Control Registers andControl Packets www.ti.com
792 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedDirect Memory Access Controller (DMA) Module20.3.2.7 Current Source Address Register (CSADDR)
Figure 20-116. Current Source Address Register (CSADDR) [offset =800h]
31 0
CSADDR
R-X
LEGEND: R=Read only; X=value isunknown; -n=value after reset
Table 20-106. Current Source Address Register (CSADDR) Field Descriptions
Bit Field Description
31-0 CSADDR Current source address. These bitscontain thecurrent working absolute 32-bit source address (physical).
These bitsareonly updated after achannel isarbitrated outfrom thepriority queue.
20.3.2.8 Current Destination Address Register (CDADDR)
Figure 20-117. Current Destination Address Register (CDADDR) [offset =804h]
31 0
CDADDR
R-X
LEGEND: R=Read only; X=value isunknown; -n=value after reset
Table 20-107. Current Destination Address Register (CDADDR) Field Descriptions
Bit Field Description
31-0 CDADDR Current destination address. These bitscontain thecurrent working absolute 32-bit destination address
(physical). These bitsareonly updated after achannel isarbitrated outofthepriority queue.
20.3.2.9 Current Transfer Count Register (CTCOUNT)
Figure 20-118. Current Transfer Count Register (CTCOUNT) [offset =808h]
31 29 28 16
Reserved CFTCOUNT
R-X R-X
15 13 12 0
Reserved CETCOUNT
R-X R-X
LEGEND: R=Read only; X=value isunknown; -n=value after reset
Table 20-108. Current Transfer Count Register (CTCOUNT) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads areundefined. Writes have noeffect.
28-16 CFTCOUNT 0-1FFFh Current frame transfer count. Returned thecurrent remaining frame counts.
15-13 Reserved 0 Reads areundefined. Writes have noeffect.
12-0 CETCOUNT 0-1FFFh Current element transfer count. These bitsreturn thecurrent remaining element counts.
CTCOUNT register isonly updated after achannel isarbitrated outofthepriority queue.