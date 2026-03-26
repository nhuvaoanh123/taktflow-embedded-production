# RAM Trace Port (RTP)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 2154-2186

---


<!-- Page 2154 -->

2154 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Chapter 37
SPNU563A -March 2018
RAM Trace Port (RTP)
This chapter describes thefunctionality oftheRAM trace port (RTP) module. Itallows thecapability to
perform data trace ofaCPU orother master accesses totheinternal RAM andperipherals.
Topic ........................................................................................................................... Page
37.1 Overview ........................................................................................................ 2155
37.2 Module Operation ........................................................................................... 2157
37.3 RTP Control Registers ..................................................................................... 2163

<!-- Page 2155 -->

www.ti.com Overview
2155 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.1 Overview
This document describes thefunctionality oftheRAM trace port (RTP) module, which provides the
features todatalog theRAM contents ofthedevices oraccesses toperipherals without program intrusion.
Itcantrace alldata write orread accesses tointernal RAM. Inaddition, itprovides thecapability todirectly
transfer data toaFIFO tosupport aCPU-controlled transmission ofthedata. The trace data istransmitted
over adedicated external interface.
37.1.1 Features
The RTP offers thefollowing features:
*Two modes ofoperation -Trace Mode andDirect Data Mode
-Trace Mode (Section 37.2.1 )
*Non-intrusive data trace onwrite orread operation
*Visibility ofRAM content atanytime onexternal capture hardware
*Trace ofperipheral accesses
*2configurable trace regions foreach RAM module tolimit amount ofdata tobetraced
*FIFO tostore data andaddress ofdata ofmultiple read/write operations
*Trace ofCPU and/or DMA accesses with indication ofthemaster inthetransmitted data packet
-Direct Data Mode (Section 37.2.2 )
*Directly write data with theCPU ortrace read operations toaFIFO, without transmitting header
andaddress information
*Dedicated synchronous interface totransmit data toexternal devices
*Free-running clock generation orclock stop mode between transmissions
*upto100Mbit persec/pin transfer rate fortransmitting data (upto100MB/s; seedevice datasheet for
maximum transmission clock frequency)
*Pins notused infunctional mode canbeused asGIOs

<!-- Page 2156 -->

Per1
Per2
PerNPer1
Per2
PerNPCR1 PCR3Lower 
256k 
L2 SRAM
Upper 
256k 
L2 SRAMCPU Interconnect Subsystem Peripheral Interconnect Subsyste mCPU
FIFO1FIFO2FIFO3FIFO4
RAM Trace Port
SERIALIZER
RTPENARTPSYNCRTPCLKRTPDATA[x]RTPDATA[0]
Overview www.ti.com
2156 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.1.2 Block Diagram
Figure 37-1 isablock diagram oftheRTP.
Figure 37-1. RAM Trace Port Module Block Diagram

<!-- Page 2157 -->

2+2+2+1+17+2     x8 bitSIZE
RAM[1:0] STAT[1:0] SIZE[1:0] WR_DATA[xx:0] ADDR[16:0] REG
2+2+2+18+2     x8 bitSIZE
RAM[1:0] STAT[1:0] SIZE[1:0] ADDR[17:0] WR_DATA[xx:0]
www.ti.com Module Operation
2157 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.2 Module Operation
The RTP module hastwomodes ofoperation: Trace Mode andDirect Data Mode.
37.2.1 Trace Mode
This mode traces allwrite orread accesses ofCPU and/or adifferent master totheinternal RAMs andthe
peripheral bus, iftheaccess falls intooneoftheprogrammed trace regions. The trace regions allow to
restrict theamount ofdata which istraced. This isdone byspecifying thestart address andthesize ofthe
region tobetraced. Itisnotpossible totrace write andread operations inthesame region atthesame
time.
Whenever awrite orread access occurs, theaddress, data, size oftheaccess (8,16,32,64bit),and
which module initiated thewrite orread operation iscaptured intotheFIFO ofthecorresponding RAM
frame. Once new data isintheFIFO andtheserializer isempty, theRTP transmits thedata intothe
serializer andstarts transmitting it.
The FIFOs areshifting data intotheserializer inaround-robin scheme. This means ifdata isavailable in
multiple FIFOs, thesequence forshifting data intotheserializer isFIFO1, FIFO2, FIFO3 andthen FIFO4.
Only oneentry intherespective FIFO isprovided totheserializer before switching tothenext FIFO. Ifa
FIFO does nothold new data, itwillbeskipped. This scheme ensures thattheFIFOs aredrained
uniformly.
NOTE: This device implements Level 1cache memory. Reading andwriting from/to Level 2RAMs
which isdeclared Cacheable canresult inRAM traces thatdonotcorrespond tothe
software's original intent. Reading from Level 2RAMs which isdeclared Cacheable willnot
result inanyload transaction iftheaddress isahitinthelevel 1cache memory. Ifawrite-
through with allocate onreads policy isselected andacache miss happens, thecache
controller willalso allocate (load) thematching cache linefrom level 2RAM after thedata is
written totheLevel 2RAM. This load duetotheallocate onreads policy willresult inthe
read data being traced.
37.2.1.1 Packet Format inTrace Mode
Figure 37-2 andFigure 37-3 illustrate thisformat.
Figure 37-2. Packet Format Trace Mode forRAM Locations
Figure 37-3. Packet Format Trace Mode forPeripheral Locations
When RAM locations aretraced, onepacket consists oftwobitsdenoting theRAM block inwhich thedata
isstored oriftheaccess hasbeen toaperipheral location (Table 37-1),twostatus bitsshowing the
access initiator orifthere was aFIFO overflow (Table 37-2 ),twobitsize (8,16,32,or64bit)information
ofthedata (Table 37-3),the18-bit address forRAM accesses and2SIZE×8bitsofdata. Ifaperipheral
location istraced, then theeffective address reduces to17bits(ADDR[16:0]) andaseparate bit(REG)
between theSIZE information andtheaddress denotes which programmable region hastraced this
peripheral access (Table 37-4).With theregion identifier, theexternal hardware candetermine which
peripheral was traced.

<!-- Page 2158 -->

Module Operation www.ti.com
2158 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-1. Encoding ofRAM Bits inTrace Mode Packet Format
RAM[1:0] RAM
0 Level 2lower 256kB RAM
1h Level 2upper 256kB RAM
2h Peripherals under PCR1
3h Peripherals under PCR3
Table 37-2. Encoding ofStatus Bits inTrace Mode Packet Format
STAT[1:0] Status
0 Normal entry CPU1
1h Normal entry other Master
2h Normal entry CPU2
3h Overflow ofthededicated FIFO
Intheevent ofaFIFO overflow, anoverflow willbesignaled inthestatus bitsofthenext transmitted
packet ofthatparticular FIFO. The lastentry intheFIFO willnotbeoverwritten bythenew data.
Table 37-3. Encoding ofSIZE bitsinTrace Mode Packet Format
SIZE[1:0] Write/Read Size
0 8bit
1h 16bit
2h 32bit
3h 64bit
Table 37-4. Encoding ofREG inTrace Mode Packet Format
REG Region
0 1
1 2
The packet willbesplit upintoseveral subpackets when transmitted over theRTP port pins depending on
theport width configured. The port width isconfigured with bitsPW[1:0] intheRTPGLBCTRL register
(Section 37.3.1 ).Forcertain port width configurations andwrite/read sizes, thenumber ofbitsinapacket
does notexactly match theport width forthelastsubpacket. The remaining bitswillbefilled with zeros.
Table 37-5. Number ofTransfers/Packet
Write/Read Size inbits
Port Width 8 16 32 64
2 16-->16 20-->20 28-->28 44-->44
4 8-->8 10-->10 14-->14 22-->22
8 4-->4 5-->5 7-->7 11-->11
16 2-->2 2.5-->3 3.5-->4 5.5-->6

<!-- Page 2159 -->

8, 16, or 32 bit
WR_DATA[xx:0]
www.ti.com Module Operation
2159 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Example: Fora16-bit port andwith data of16-bit, thelasttransfer hastobepadded with eight 0s.This
effectively results inatransfer of48bitsinstead of40.However thewhole transfer iscompleted in3
RTPCLK cycles.
Foradetailed description oftherepresentation ofthepacket ontheRTP port pins, refer toSection 37.2.5 .
37.2.2 Direct Data Mode (DDM)
Inthismode, data iswritten directly bytheCPU orother master toadedicated capture register
(RTPDDMW). The data isthen transferred from thecapture register totheFIFO. Inadifferent
configuration themodule traces thedata onread operations ontheRAM directly intotheFIFOs. InDirect
Data Mode, noinformation other than theactual data istransmitted. The address ofthewritten data can
only bedetermined bytheorder ofwrites orreads bytheCPU orother master. This mode isespecially
useful ifablock ofdata onconsecutive addresses hastobetransmitted.
The transfer size (8,16,or32bit)isprogrammable, butcannot bedynamically changed. Data not
written/read inthecorrect transfer size willbetruncated/extended. Forexample, ifthetransfer size is
programmed to16bitsanda32-bit write operation isperformed, thedata written totheFIFO willbe32-bit
wide, however only theupper 16bitsoftheFIFO willbetransmitted. Ifan8-bit operation isperformed, bits
8-15 oftheFIFO willbeindeterminate, sotheupper 8bitsofthedata transmitted aredependent onthe
previous contents oftheFIFO RAM.
When themodule isconfigured inDirect Data Mode (TM_DDM =1)totrace write operations (DDM_RW =
1)totheRTPDDMW register, theprogramming ofthetrace regions forallFIFOs willbeignored anddata
tracing, when accessing theaddresses defined bytheregions, willnotoccur. Ifthemodule isconfigured in
read mode (DDM_RW =0),andiftheread access toaRAM block falls intoavalid trace region, thedata
willbetraced intothecorresponding FIFO forthisRAM block. Since noaddress information istransmitted
inDirect Data Mode, theexecuting program hastomake sure thatoneFIFO iscompletely empty
(RTPGSR register), before new data istraced intothenext FIFO.
37.2.2.1 Packet Format inDirect Data Mode
InDirect Data Mode write orread operations, only thedata written totheRTP direct data mode write
register (RTPDDMW) orthedata read from RAM, andtherefore captured intotheFIFO, willbe
transmitted. The packet length isprogrammable (8,16,or32bits). Figure 37-4 illustrates thisformat.
Figure 37-4. Packet Format inDirect Data Mode
37.2.3 Trace Regions
Tolimit theamount ofdata tobetrace, twotrace regions perRAM orperipheral areimplemented. These
canbeprogrammed tospecific start addresses andblock sizes. Depending onthedevice configuration
(number ofRAM blocks), notallregions might beimplemented. Trace regions areused inTrace Mode for
read orwrite trace andinDirect Data Mode forread trace. InDirect Data Mode write configuration, the
data hastobewritten directly totheRTP direct data mode write register (RTPDDMW).
The RAM andperipherals start atfixed addresses inthedevices memory map. With thisthestart address
ofaregion does notneed tobespecified with itsfull32-bit address. ForRAM regions, only thelower 18-
bitneed tobeprogrammed. The peripheral address frame covers awider range andthestart address
needs tobeprogrammed with thelower 24-bit.
The trace regions donotsupport aprogrammable endaddress; however, ablock size needs tobe
specified foreach region. The block size canbechosen from aslowas256Bytes upto256kBytes
(128 kBytes forperipherals).

<!-- Page 2160 -->

0x080000000x080010000x080013FF0x080038000x08003FFF
RTPRAM1REG1 = 0x33001000
RTPRAM1REG2 = 0x740038004GB Address
Space
2kB
Region 2
1kB
Region 12 Trace Regions
* Region 1
- starts at 0x08001000 with size of 1kB
- CPU write access are traced
* Region 2
- starts at 0x08003800 with size of 2kB
- CPU and other master write accesses are traced
Module Operation www.ti.com
2160 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.2.3.1 Inverse Trace Regions
The RTP canbeconfigured totrace accesses which fallinto, oraremade outside ofthespecified regions.
This canbeaccomplished bytheINV_RGN bit.Ifthisbitis0,allaccess which aremade inside aregion
aretraced. Ifthebitis1,allaccesses outside theregion aretraced. The INV_RGN bitaffects allregions
oftheRTP, seetheRTP global control register (RTPGLBCTRL).
There arecertain restrictions when using INV_RGN =1:
*Inthismode upto2regions canbeexcluded from tracing accesses toaparticular RAM.
*Inverse trace regions with oneorboth regions ofaRAM programmed with blocksize =0isnot
supported. Ifonly oneaddress range should beexcluded from thetrace, either theaddress range has
tobecovered byboth regions (e.g. excluding 1kB range with two512B regions), orboth regions have
tobeprogrammed with thesame start address andregion size. Ifthewhole RAM should betraced,
inverse region mode should notbeused, instead the2regions could beprogrammed tocover the
entire address range with INV_RGN =0.
*Both regions have todefine thesame access rights (bits CPU_DMA andRW; seeSection 37.3.4 )for
accesses outside oftheregion ofeach RAM block, otherwise theresult isundefined.
*Peripheral trace ininverse region mode isnotsupported. The 16MByte peripheral address range
cannot becovered entirely bythe17bitaddress definition oftheRTP protocol.
37.2.3.2 Overlapping Trace Regions
When inINV_RGN =0mode with both regions overlapping andanaccess isdone intotheoverlapping
address range, both regions willbechecked fortheir access rights andifoneorboth issatisfied, the
access willbetraced. Inthecase thatboth regions would allow thedata tobetraced, there willstillbe
only oneentry intotheFIFO.
Ifaccesses toperipherals aredone within overlapping regions, theREG bitintheprotocol willbe0,
denoting Region 1(see Section 37.2.1.1 ).
Figure 37-5. Example forTrace Region Setup

<!-- Page 2161 -->

CTRL
11 00 00CTRL CTRLoverflow
www.ti.com Module Operation
2161 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.2.3.3 Cortex-R5 Specifics
Considerations/Restrictions
*Reading andwriting from/to Level 2RAMs which isdeclared Cacheable canresult inRAM traces that
donotcorrespond tothesoftware's original intent.
*Astore instruction toNon-cacheable, orwrite-through Normal memory might notresult inanAXI
transfer totheLevel 2RAMs orperipherals because ofthemerging ofstore intheinternal buffers.
37.2.4 Overflow/Empty Handling
Incase theapplication does RAM accesses faster than theFIFO canbeemptied viatheexternal pin
interface, theFIFO canoverflow. You canchoose whether theprogram execution/data transfer should be
suspended, oranoverflow should besignaled inthestatus bitsofthenext, tobetransmitted, message of
thisparticular FIFO. Ifprogram execution isresumed, thedata willbelost. The overflow willnotbe
signaled inthemessage thatisalready intheserializer andbeing transmitted when theoverflow occurs.
NOTE: The status information willonly betransmitted inTrace Mode, since theDirect Data Mode
packet does notcontain anystatus information.
When anoverflow inaFIFO occurs, thecorresponding bitintheRTP global status register (RTPGSR) will
beset.
Figure 37-6. FIFO Overflow Handling
37.2.5 Signal Description
Table 37-6 lists thesignals oftheRTP.
Table 37-6. RTP Signals
Signal Description
RTPCLK This clock signal isused toclock outthedata oftheserializer. Depending ontheCONTCLK bit,
theclock canbesuspended between packets oritcanbefree running. The RTPCLK frequency
canbeadjusted bythePRESCALER bits(see RTP global control register (RTPGLBCTRL).
RTPSYNC The module provides apacket-sync signal. This signal willgohigh ontherising edge ofRTPCLK
andwillbevalid foroneRTPCLK cycle tosynchronize external hardware tothedata stream. The
RTPSYNC pulse willbegenerated foreach new packet.
RTPENA This signal isaninput andcanbeused byexternal hardware tostop thedata transmission
between packets. When theRTPENA signal goes high, theRTP willfinish thecurrent packet
transmission andthen stop. Once thesignal ispulled lowagain, theRTP willresume thetransfer
ifdata isstillpresent intheserializer orFIFOs. The RTPENA signal does nothave tobeused for
proper module operation. Itcanbeused inGIO mode iftheexternal hardware cannot generate
thissignal. Overflows oftheexternal system cannot behandled inthiscase.
RTPDATA[15:0] These pins areused todotheactual data transfer. Data changes with therising edge ofRTPCLK.
The port canbeconfigured fordifferent widths (PW[1:0]). The minimum port width supported is2
pins. See Table 37-10 which pins areused fortheport.

<!-- Page 2162 -->

DEST[1] SIZE[1] ADDR[15] ADDR[11] ADDR[7] ADDR[3] DATA[7] DATA[3]
DEST[0]
ADDR[13]ADDR[6] ADDR[2] DATA[2] DATA[6]
DATA[5]
DATA[4]ADDR[9] ADDR[5] ADDR[1] DATA[1]
ADDR[4] ADDR[0] DATA[0]SIZE[0] ADDR[14] ADDR[10]
ADDR[12] ADDR[8]STAT[1]
STAT[0]ADDR[17]
ADDR[16]RTPCLK
RTPSYNC
RTPDATA[0]
RTPDATA[1]
RTPDATA[2]
RTPDATA[3]
RTPSYNC
RTPDATARTPCLKRTPENA
Packet1 Packet2 Packet3 Packet4 Packet1 Packet2
Module Operation www.ti.com
2162 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Figure 37-7 shows anexample ofmultiple packet transmissions inTrace Mode with aninterruption
between packets because ofRTPENA pulled high.
Figure 37-7. RTP Packet Transfer with Sync Signal
Figure 37-8 shows anexample ofa4-bit data port with 8-bit write data (A5h) written intoRAM1 (address
12345h) with nooverflow intrace mode.
Figure 37-8. Packet Format inTrace Mode
37.2.6 Data Rate
The module isconfigurable tosupport different RTPCLK frequencies. See thedevice datasheet forthe
maximum supported frequency. HCLK willbeprescaled toachieve thedesired RTPCLK frequency. The
prescaler supports prescale values from 1to8,using theRTP global control register (RTPGLBCTRL).
The effective bandwidth depends ontheconfiguration ofthemodule andtheaverage data width
transmitted inthepackets.

<!-- Page 2163 -->

www.ti.com Module Operation
2163 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.2.7 GIO Function
Pins which arenotused forRTP functionality canbeused asnormal GIO pins. Ifpins should beused in
functional mode orGIO mode, they canbeprogrammed intheRTP pincontrol 0register (RTPPC0). The
direction ofthepins canbechosen intheRTP pincontrol 1register (RTPPC1).
Module pins canhave either aninternal pullup oractive pulldown thatmakes itpossible toleave thepins
unconnected externally when configured asinputs. The pins canbeprogrammed tohave theactive pull
capability bywriting a0tothecorresponding bitintheRTP pincontrol 7register (RTPPC7). Writing a1to
thecorresponding bitdisables theactive pullfunctionality ofthepin.Apullupcanbeconfigured bywriting
1tothecorresponding bitintheRTP pincontrol 8register (RTPPC8). Writing 0willactivate thepulldown
capability. The pullup/pulldown isdeactivated when abidirectional pinisconfigured asanoutput. Ifthe
pullup/down capability isdisabled (RTPPC7) andthepullisconfigured aspulldown (RTPPC8), theinput
buffer willbedisabled.
The GIO pincanbeconfigured toinclude anopen drain functionality when they areconfigured asoutput
pins. This isdone bywriting a1intothecorresponding bitoftheRTP pincontrol 6register (RTPPC6).
When theopen drain functionality isenabled, azero written tothedata output register (RTPPC3) forces
thepintoalowoutput voltage (VOLorlower), whereas writing a1tothedata output register (RTPPC3)
forces thepintoahigh impedance state. The open drain functionality isdisabled when thepinis
configured asaninput pin.
37.3 RTP Control Registers
Table 37-7 lists theRTP module registers. The registers support 8-,16-, and32-bit writes. The base
address oftheRTP module isFFFF FA00h.
Table 37-7. RTP Control Registers
Offset Acronym Register Description Section
00h RTPGLBCTRL RTP Global Control Register Section 37.3.1
04h RTPTRENA RTP Trace Enable Register Section 37.3.2
08h RTPGSR RTP Global Status Register Section 37.3.3
0Ch RTPRAM1REG1 RTP RAM 1Trace Region 1Register Section 37.3.4
10h RTPRAM1REG2 RTP RAM 1Trace Region 2Register Section 37.3.4
14h RTPRAM2REG1 RTP RAM 2Trace Region 1Register Section 37.3.5
18h RTPRAM2REG2 RTP RAM 2Trace Region 2Register Section 37.3.5
1Ch RTPRAM3REG1 RTP RAM 3Trace Region 1Register Section 37.3.6
20h RTPRAM3REG2 RTP RAM 3Trace Region 2Register Section 37.3.6
24h RTPPERREG1 RTP Peripheral Trace Region 1Register Section 37.3.7
28h RTPPERREG2 RTP Peripheral Trace Region 2Register Section 37.3.7
2Ch RTPDDMW RTP Direct Data Mode Write Register Section 37.3.8
34h RTPPC0 RTP PinControl 0Register Section 37.3.9
38h RTPPC1 RTP PinControl 1Register Section 37.3.10
3Ch RTPPC2 RTP PinControl 2Register Section 37.3.11
40h RTPPC3 RTP PinControl 3Register Section 37.3.12
44h RTPPC4 RTP PinControl 4Register Section 37.3.13
48h RTPPC5 RTP PinControl 5Register Section 37.3.14
4Ch RTPPC6 RTP PinControl 6Register Section 37.3.15
50h RTPPC7 RTP PinControl 7Register Section 37.3.16
54h RTPPC8 RTP PinControl 8Register Section 37.3.17

<!-- Page 2164 -->

RTP Control Registers www.ti.com
2164 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.1 RTP Global Control Register (RTPGLBCTRL)
The configuration ofthemodule canbechanged with thisregister. Figure 37-9 andTable 37-8 describe
thisregister.
Figure 37-9. RTP Global Control Register (RTPGLBCTRL) (offset =00h)
31 25 24 23 19 18 16
Reserved TEST Reserved PRESCALER
R-0 R/WP-0 R-0 R/WP-7h
15 14 13 12 11 10 9 8
Reserved DDM_WIDTH DDM_RW TM_DDM PW
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 0
RESET CONTCLK HOVF INV_RGN ON/OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-8. RTP Global Control Register (RTPGLBCTRL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 TEST Bysetting thebit,theFIFO RAM willbemapped intotheSYSTEM Peripheral frame starting at
address 0xFFF83000. Each FIFO starts ata1-kboundary. Each FIFO entry isaligned toa128-
bitboundary. See Table 37-9 foralisting oftheFIFOs andtheir corresponding addresses.
Read:
0 FIFO RAM isnotaccessible inthememory-map.
1 FIFO RAM ismapped toaddress FFF8 3000h.
Write inPrivilege:
0 Disables mapping oftheFIFO RAM.
1 Enables mapping oftheFIFO RAM intoaddress FFF8 3000h.
18-16 PRESCALER The prescaler divides HCLK down tothedesired RTPCLK frequency. The maximum RTPCLK
frequency specified inthedevice datasheet must notbeexceeded. Nodynamic change of
RTPCLK issupported. The module should beswitched offbytheON/OFF bitsinthisregister
before changing theprescaler.
User andprivilege mode read, privilege mode write:
0 Prescaler isHCLK/1.
1h Prescaler isHCLK/2.
2h Prescaler isHCLK/3.
3h Prescaler isHCLK/4.
4h Prescaler isHCLK/5.
5h Prescaler isHCLK/6.
6h Prescaler isHCLK/7.
7h Prescaler isHCLK/8.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-12 DDM_WIDTH Direct data mode word size width. This bitfield configures thenumber ofbitsthatwillbe
transmitted inDirect Data Mode.
User andprivilege mode read, privilege mode write:
0 Word size width is8bits.
1h Word size width is16bits.
2h Word size width is32bits.
3h Reserved

<!-- Page 2165 -->

www.ti.com RTP Control Registers
2165 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-8. RTP Global Control Register (RTPGLBCTRL) Field Descriptions (continued)
Bit Field Value Description
11 DDM_RW Direct data mode.
Read:
0 Read tracing inDirect Data Mode isenabled.
1 Write tracing inDirect Data Mode toDDMW register isenabled.
Write inPrivilege:
0 Enable read tracing inDirect Data Mode. The RWbitsintheRTPRAMxREGy registers tobe
ignored.
1 Write tracing inDirect Data Mode toDDMW register isenabled. The RWbitsinthe
RTPRAMxREGy registers aretobeignored.
10 TM_DDM Trace Mode orDirect Data Mode.
Read:
0 Module isconfigured inTrace Mode.
1 Module isconfigured inDirect Data Mode.
Write inPrivilege:
0 Configure module toTrace Mode.
1 Configure module toDirect Data Mode.
9-8 PW Port width. This bitfield configures theRTP tothedesired port width. Pins thatarenotused for
functional mode canbeused asGIO pins. See Table 37-10 forwhich pins areused fortheport.
0 RTP is2pins wide.
1h RTP is4pins wide.
2h RTP is8pins wide.
3h RTP is16pins wide.
7 RESET This bitresets thestate machine andtheregisters totheir reset value. This reset ensures that
nodata leftintheFIFOs isshifted outafter switching onthemodule with theON/OFF bit.
Read:
0 RTP module isoutofreset.
1 RTP module isinreset.
Write inPrivilege:
0 Donotreset themodule.
1 Reset themodule.
6 CONTCLK Continuous RTPCLK enable.
Read:
0 RTPCLK isstopped between transmissions.
1 RTPCLK isfree running.
Write inPrivilege:
0 Stop RTPCLK between transmissions.
1 Configure RTPCLK asfree running.
5 HOVF Halt onoverflow. This bitindicates whether theCPU orDMA ishalted while only onelocation in
theFIFO isempty inTrace Mode orDirect Data Mode (read).
Read:
0 Current data transfer totheFIFO willnotbesuspended incase ofafullFIFO.
1 Current data transfer totheFIFO willbesuspended incase ofafullFIFO.
Write inPrivilege:
0 The haltonFIFO overflow willbedisabled. The data transfer willnotbesuspended andwillbe
discarded. Data written totheRTPDDMW register willoverwrite theRTPDDMW register.
1 The haltonFIFO overflow willbeenabled. Data written tothealready fullFIFO willbewritten
once theFIFO isemptied again. The data transfer totheFIFO willbesuspended andsignaled
totheCPU orother master while there isstilldata tobeshifted out.When there isanempty
FIFO location again, thetransfer ofthedata totheFIFO willbefinished.

<!-- Page 2166 -->

RTP Control Registers www.ti.com
2166 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-8. RTP Global Control Register (RTPGLBCTRL) Field Descriptions (continued)
Bit Field Value Description
4 INV_RGN Trace inside oroutside ofdefined trace regions.
Read:
0 Accesses inside thetrace regions aretraced.
1 Accesses outside thetrace regions aretraced.
Write inPrivilege:
0 Allow tracing ofaccesses inside theregions setinRTPRAMxREGy.
1 Allow tracing ofaccesses outside theregions setinRTPRAMxREGy.
3-0 ON/OFF ON/Off switch.
Read:
Ah Tracing ofdata isenabled.
Allother
valuesTracing ofdata isdisabled.
Write inPrivilege:
Ah Enable Tracing ofdata. Ifthere isanyprevious captured data remaining, itwillbeshifted out.
Allother
valuesDisable tracing ofdata. Ifthere isstilldata leftintheshift register, itwillbeshifted outbefore
disabling theshift operations. The data captured intheFIFO remains there until theON/OFF
bitsaresettoAh.
NOTE: Itisrecommended towrite 5htodisable themodule toprevent asofterror from
enabling themodule inadvertently byasingle bitflip.
Table 37-9. FIFO Corresponding Addresses
FIFO Address
1 FFF8 3000h
2 FFF8 3400h
3 FFF8 3800h
4 FFF8 3C00h
Table 37-10. Pins Used forData Communication
Port Width (PW) Pins Used
00 RTPDATA[1:0]
01 RTPDATA[3:0]
10 RTPDATA[7:0]
11 RTPDATA[15:0]

<!-- Page 2167 -->

www.ti.com RTP Control Registers
2167 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.2 RTP Trace Enable Register (RTPTRENA)
This register enables/disables tracing ofthedifferent RAM blocks ortheperipherals individually.
Figure 37-10 andTable 37-11 describe thisregister.
Figure 37-10. RTP Trace Enable Register (RTPTRENA) (offset =04h)
31 25 24 23 17 16
Reserved ENA4 Reserved ENA3
R-0 R/WP-0 R-0 R/WP-0
15 9 8 7 1 0
Reserved ENA2 Reserved ENA1
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-11. RTP Trace Enable Register (RTPTRENA) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ENA4 Enable tracing forperipherals under PCR3. This bitenables tracing intoFIFO4 intrace mode
(read/write) ordirect data mode (read) operations. InDirect Data Mode write operations, thisbitis
ignored andtracing intoFIFO4 isdisabled.
Read:
0 Tracing isdisabled.
1 Tracing isenabled.
Write inPrivilege:
0 Disable tracing. IfRTPGLBCTRL.ON/OFF =Ah,data already captured inFIFO4 isstilltransmitted
(RTPGLBCTRL).
1 Enable tracing.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 ENA3 Enable tracing forperipherals under PCR1. This bitenables tracing intoFIFO3 inTrace Mode
(read/write) orDirect Data Mode (read) operations. InDirect Data Mode write operations, thisbitis
ignored andtracing intoFIFO3 isdisabled.
Read:
0 Tracing isdisabled.
1 Tracing isenabled.
Write inPrivilege:
0 Disable tracing. IfRTPGLBCTRL.ON/OFF =Ah,data already captured inFIFO3 isstilltransmitted
(RTPGLBCTRL).
1 Enable tracing.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 ENA2 Enable tracing forRAM block 2.This bitenables tracing intoFIFO2 inTrace Mode (read/write) orDirect
Data Mode (read) operations. InDirect Data Mode write operations, thisbitisignored andtracing into
FIFO2 isdisabled.
Read:
0 Tracing isdisabled.
1 Tracing isenabled.
Write inPrivilege:
0 Disable tracing. IfRTPGLBCTRL.ON/OFF =Ah,data already captured inFIFO2 isstilltransmitted.
1 Enable tracing.
7-1 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 2168 -->

RTP Control Registers www.ti.com
2168 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-11. RTP Trace Enable Register (RTPTRENA) Field Descriptions (continued)
Bit Field Value Description
0 ENA1 Enable tracing forRAM block 1.This bitenables tracing intoFIFO1 inTrace Mode (read/write) orDirect
Data Mode (read) operations. InDirect Data Mode write operations, thisbitisignored andtracing into
FIFO1 isdisabled.
Read:
0 Tracing isdisabled.
1 Tracing isenabled.
Write inPrivilege:
0 Disable tracing. IfRTPGLBCTRL.ON/OFF =Ah,data already captured inFIFO1 isstilltransmitted.
1 Enable tracing.

<!-- Page 2169 -->

www.ti.com RTP Control Registers
2169 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.3 RTP Global Status Register (RTPGSR)
This register provides status information ofthemodule. Figure 37-11 andTable 37-12 describe this
register.
Figure 37-11. RTP Global Status Register (RTPGSR) (offset =08h)
31 16
Reserved
R-0
15 13 12 11 10 9 8
Reserved EMPTYSER EMPTYPER2 EMPTYPER1 EMPTY2 EMPTY1
R-0 R-1 R-1 R-1 R-1 R-1
7 4 3 2 1 0
Reserved OVFPER2 OVFPER1 OVF2 OVF1
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 37-12. RTP Global Status Register (RTPGSR) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 EMPTYSER Serializer empty. This bitdetermines ifthere isdata leftintheserializer.
0 Serializer holds data thatisshifted out.
1 Serializer isempty.
11 EMPTYPER2 Peripheral FIFO empty. This bitdetermines ifthere areentries leftintheFIFO. FIFO4 isused fortracing
peripherals under PCR3.
0 FIFO4 contains entries.
1 FIFO4 isempty.
10 EMPTYPER1 Peripheral FIFO empty. This bitdetermines ifthere areentries leftintheFIFO. FIFO3 isused fortracing
peripherals under PCR1.
0 FIFO3 contains entries.
1 FIFO3 isempty.
9 EMPTY2 RAM block 2FIFO empty. This bitdetermines ifthere areentries leftintheFIFO. FIFO2 isused for
tracing theupper 256kB RAM.
0 FIFO2 contains entries.
1 FIFO2 isempty.
8 EMPTY1 RAM block 1FIFO empty. This bitdetermines ifthere areentries leftintheFIFO. FIFO1 isused for
tracing thelower 256kB RAM.
0 FIFO1 contains entries.
1 FIFO1 isempty.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3 OVFPER Overflow peripheral FIFO. This flagindicates thatFIFO4 hadalllocations fullandanother attempt to
write data toitoccurred. The bitwillnotbecleared automatically iftheFIFO isemptied again. The bit
willstay setuntil theCPU clears it.
Read:
0 Nooverflow occurred.
1 Anoverflow occurred.
Write inPrivilege:
0 Noeffect.
1 Clears thebit.
2 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 2170 -->

RTP Control Registers www.ti.com
2170 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-12. RTP Global Status Register (RTPGSR) Field Descriptions (continued)
Bit Field Value Description
1 OVF2 Overflow RAM block 2FIFO. This flagindicates thatFIFO2 hadalllocations fullandanother attempt to
write data toitoccurred. The bitwillnotbecleared automatically iftheFIFO isemptied again. The bit
willstay setuntil theCPU clears it.
Read:
0 Nooverflow occurred.
1 Anoverflow occurred.
Write inPrivilege:
0 Noeffect.
1 Clears thebit.
0 OVF1 Overflow RAM block 1FIFO. This flagindicates thatFIFO1 hadalllocations fullandanother attempt to
write data toitoccurred. The bitwillnotbecleared automatically iftheFIFO isemptied again. The bit
willstay setuntil theCPU clears it.
Read:
0 Nooverflow occurred.
1 Anoverflow occurred.
Write inPrivilege:
0 Noeffect.
1 Clears thebit.

<!-- Page 2171 -->

www.ti.com RTP Control Registers
2171 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.4 RTP RAM 1Trace Region Registers (RTPRAM1REG[1:2])
Figure 37-12 andTable 37-13 illustrate these registers.
Figure 37-12. RTP RAM 1Trace Region Registers (RTPRAM1REGn) (offset =0Ch and10h)
31 29 28 27 24 23 18 17 16
CPU_DMA RW BLOCKSIZE Reserved STARTADDR
R/WP-0 R/WP-0 R/WP-0 R-0 R/WP-0
15 0
STARTADDR
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-13. RTP RAM 1Trace Region Registers (RTPRAM1REGn) Field Descriptions
Bit Field Value Description
31-29 CPU_DMA When thedevice isconfigured inlock-step mode, bit31willreturn 0andawrite hasnoeffect.
This bitfield indicates ifread orwrite operations aretraced either coming from theCPU and/or
from theother master.
User andprivilege mode read, privilege mode write:
0 Read orwrite operations aretraced when coming from theCPU andtheother master.
1h Read orwrite operations aretraced only when coming from theCPU.
2h Read orwrite operations aretraced only when coming from theother non-CPU master.
3h Reserved
28 RW Read/Write. This bitindicates ifread orwrite operations aretraced inTrace Mode orDirect
Data Mode (read operation). Ifconfigured forwrite inDirect Data Mode (RTPGLBCTRL), the
data captured willbediscarded. Awrite operation inDirect Data Mode hastobedirectly tothe
RTP direct data mode write register (RTPDDMW) instead oftoRAM. Depending onthe
INV_RGN bitsetting, accesses intooroutside theregion willbetraced.
Read:
0 Read operations willbecaptured.
1 Write operations willbecaptured.
Write inPrivilege:
0 Trace read accesses.
1 Trace write accesses.
27-24 BLOCKSIZE These bitsdefine thelength ofthetrace region. Depending onthesetting ofINV_RGN
(RTPGLBCTRL), accesses inside oroutside theregion defined bythestart address and
blocksize willbetraced. IfallbitsofBLOCKSIZE are0,theregion isdisabled andnodata will
becaptured.
Region size (inbytes):
0 0
1h 256
2h 512
3h 1K
4h 2K
Ah 128K
Bh 256K
Ch-Fh Reserved
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17-0 STARTADDR 0-3FFFFh These bitsdefine thestarting address oftheaddress region thatshould betraced. The start
address hastobeamultiple oftheblock size chosen. Ifthestart address isnotamultiple of
theblock size, thestart oftheregion willbegin atthenext lower block size boundary.

<!-- Page 2172 -->

RTP Control Registers www.ti.com
2172 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.5 RTP RAM 2Trace Region Registers (RTPRAM2REG[1:2])
Figure 37-13 andTable 37-14 illustrate these registers.
Figure 37-13. RTP RAM 2Trace Region Registers (RTPRAM2REGn) (offset =14hand18h)
31 29 28 27 24 23 18 17 16
CPU_DMA RW BLOCKSIZE Reserved STARTADDR
R/WP-0 R/WP-0 R/WP-0 R-0 R/WP-0
15 0
STARTADDR
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-14. RTP RAM 2Trace Region Registers (RTPRAM2REGn) Field Descriptions
Bit Field Value Description
31-29 CPU_DMA When thedevice isconfigured inlock-step mode, bit31willreturn 0andawrite hasnoeffect.
This bitfield indicates ifread orwrite operations aretraced either coming from theCPU and/or
from theother master.
User andprivilege mode read, privilege mode write:
0 Read orwrite operations aretraced when coming from theCPU andtheother master.
1h Read orwrite operations aretraced only when coming from theCPU.
2h Read orwrite operations aretraced only when coming from theother master.
3h Reserved
28 RW Read/Write. This bitindicates ifread orwrite operations aretraced inTrace Mode orDirect
Data Mode (read operation). Ifconfigured forwrite inDirect Data Mode (RTPGLBCTRL), the
data captured willbediscarded. Awrite operation inDirect Data Mode hastobedirectly tothe
RTP direct data mode write register (RTPDDMW) instead oftoRAM. Depending onthe
INV_RGN bitsetting, accesses intooroutside theregion willbetraced.
Read:
0 Read operations willbecaptured.
1 Write operations willbecaptured.
Write inPrivilege:
0 Trace read accesses.
1 Trace write accesses.
27-24 BLOCKSIZE These bitsdefine thelength ofthetrace region. Depending onthesetting ofINV_RGN
(RTPGLBCTRL), accesses inside oroutside theregion defined bythestart address and
blocksize willbetraced. IfallbitsofBLOCKSIZE are0,theregion isdisabled andnodata will
becaptured.
Region size (inbytes):
0 0
1h 256
2h 512
3h 1K
4h 2K
Ah 128K
Bh 256K
Ch-Fh Reserved
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17-0 STARTADDR 0-3FFFFh These bitsdefine thestarting address oftheaddress region thatshould betraced. The start
address hastobeamultiple oftheblock size chosen. Ifthestart address isnotamultiple of
theblock size, thestart oftheregion willbegin atthenext lower block size boundary.

<!-- Page 2173 -->

www.ti.com RTP Control Registers
2173 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.6 RTP RAM 3Trace Region Registers (RTPRAM3REG[1:2])
FIFO3 was originally designed tosupport RAM trace limiting toamaximum trace range of256kB. Inthis
device, FIFO3 isdedicated fortracing thePCR1 peripheral accesses. Peripherals inPCR1 occupy atotal
address range of512kB ofspace. Therefore, itisnotpossible totrace theentire range of512kB since
there areonly 18bitsofaddress being traced outinthepacket. You canusethetwotrace regions to
trace anytwoareas inthelower halfofthePCR1's space from 0xFFF80000 to0xFFFBFFFF provided
there isnotanintentional orun-intentional access totheupper halfofthespace. Ifyouwant totrace the
upper halfofPCR1's space from 0xFFFC0000 to0xFFFFFFFF then theexternal hardware/software must
reconstruct thefull32-bit address byforcing address bit18high andalso ensures thatthere isno
intentional orun-intentional accesses tothelower halfofthespace. Since theexternal hardware isunable
todistinguish between thelower halfandtheupper halfofPCR1, youcannottrace both ofthehalves at
thesame time.
NOTE: BitREG (Section 37.2.1.1 )intheprotocol forperipheral trace willbenotbeapplicable tothe
PCR1 trace. PCR1 trace follows theRAM trace protocol with 18bitsofaddress trace out.
Figure 37-14 andTable 37-15 illustrate these registers.
Figure 37-14. RTP RAM 3Trace Region Registers (RTPRAM3REGn) (offset =1Ch and20h)
31 29 28 27 24 23 18 17 16
CPU_DMA RW BLOCKSIZE Reserved STARTADDR
R/WP-0 R/WP-0 R/WP-0 R-0 R/WP-0
15 0
STARTADDR
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-15. RTP RAM 3Trace Region Registers (RTPRAM3REGn) Field Descriptions
Bit Field Value Description
31-29 CPU_DMA When thedevice isconfigured inlock-step mode, bit31willreturn 0andawrite hasnoeffect.
This bitfield indicates ifread orwrite operations aretraced either coming from theCPU and/or
from theother master.
User andprivilege mode read, privilege mode write:
0 Read orwrite operations aretraced when coming from theCPU andtheother master.
1h Read orwrite operations aretraced only when coming from theCPU.
2h Read orwrite operations aretraced only when coming from theother master.
3h Reserved
28 RW Read/Write. This bitindicates ifread orwrite operations aretraced inTrace Mode orDirect
Data Mode (read operation). Ifconfigured forwrite inDirect Data Mode (RTPGLBCTRL), the
data captured willbediscarded. Awrite operation inDirect Data Mode hastobedirectly tothe
RTP direct data mode write register (RTPDDMW) instead oftoRAM. Depending onthe
INV_RGN bitsetting, accesses intooroutside theregion willbetraced.
Read:
0 Read operations willbecaptured.
1 Write operations willbecaptured.
Write inPrivilege:
0 Trace read accesses.
1 Trace write accesses.

<!-- Page 2174 -->

RTP Control Registers www.ti.com
2174 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-15. RTP RAM 3Trace Region Registers (RTPRAM3REGn) Field Descriptions (continued)
Bit Field Value Description
27-24 BLOCKSIZE These bitsdefine thelength ofthetrace region. Depending onthesetting ofINV_RGN
(RTPGLBCTRL), accesses inside oroutside theregion defined bythestart address and
blocksize willbetraced. IfallbitsofBLOCKSIZE are0,theregion isdisabled andnodata will
becaptured.
Region size (inbytes):
0 0
1h 256
2h 512
3h 1K
4h 2K
Ah 128K
Bh 256K
Ch-Fh Reserved
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17-0 STARTADDR 0-3FFFFh These bitsdefine thestarting address oftheaddress region thatshould betraced. The start
address hastobeamultiple oftheblock size chosen. Ifthestart address isnotamultiple of
theblock size, thestart oftheregion willbegin atthenext lower block size boundary.

<!-- Page 2175 -->

www.ti.com RTP Control Registers
2175 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.7 RTP Peripheral Trace Region Registers (RTPPERREG[1:2])
FIFO4 isdedicated fortracing thePCR3 peripheral accesses. Since theperipheral frame is16Mbytes,
thestart address hastobedefined asa24-bit value. However, only bits16to0willbetransmitted inthe
protocol. BitREG (Section 37.2.1.1 )intheprotocol willbe0ifthere was anaccess totherange defined
byRTPPERREG1. REG willbe1iftheaccess was intotherange defined byRTPPERREG2. Figure 37-
15andTable 37-16 illustrate these registers.
Figure 37-15. RTP Peripheral Trace Region Registers (RTPPERREGn) (offset =24hand28h)
31 29 28 27 24 23 16
CPU_DMA RW BLOCKSIZE STARTADDR
R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 0
STARTADDR
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 37-16. RTP Peripheral Trace Region Registers (RTPPERREGn) Field Descriptions
Bit Field Value Description
31-29 CPU_DMA When thedevice isconfigured inlock-step mode, bit31willreturn 0andawrite hasnoeffect.
This bitfield indicates ifread orwrite operations aretraced either coming from theCPU and/or
from theother master.
User andprivilege mode read, privilege mode write:
0 Read orwrite operations aretraced when coming from theCPU andtheother master.
1h Read orwrite operations aretraced only when coming from theCPU.
2h Read orwrite operations aretraced only when coming from theother master.
3h Reserved
28 RW Read/Write. This bitindicates ifread orwrite operations aretraced inTrace Mode orDirect
Data Mode (read operation). Ifconfigured forwrite inDirect Data Mode (RTPGLBCTRL), the
data captured willbediscarded. Awrite operation inDirect Data Mode hastobedirectly tothe
RTP direct data mode write register (RTPDDMW) instead oftoRAM. Depending onthe
INV_RGN bitsetting, accesses intooroutside theregion willbetraced.
Read:
0 Read operations willbecaptured.
1 Write operations willbecaptured.
Write inPrivilege:
0 Trace read accesses.
1 Trace write accesses.
27-24 BLOCKSIZE These bitsdefine thelength ofthetrace region. Depending onthesetting ofINV_RGN
(RTPGLBCTRL), accesses inside oroutside theregion defined bythestart address and
blocksize willbetraced. IfallbitsofBLOCKSIZE are0,theregion isdisabled andnodata will
becaptured.
Region size (inbytes):
0 0
1h 256
2h 512
3h 1K
4h 2K
Ah 128K
Bh 256K
Ch-Fh Reserved
23-0 STARTADDR 0-FF FFFFh These bitsdefine thestarting address oftheaddress region thatshould betraced. The start
address hastobeamultiple oftheblock size chosen. Ifthestart address isnotamultiple of
theblock size, thestart oftheregion willbegin atthenext lower block size boundary.

<!-- Page 2176 -->

RTP Control Registers www.ti.com
2176 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.8 RTP Direct Data Mode Write Register (RTPDDMW)
The CPU hastowrite data tothisregister ifthemodule isused inDirect Data Mode write configuration.
Figure 37-16 andTable 37-17 describe thisregister.
Figure 37-16. RTP Direct Data Mode Write Register (RTPDDMW) (offset =2Ch)
31 0
DATA
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 37-17. RTP Direct Data Mode Write Register (RTPDDMW) Field Descriptions
Bit Field Description
31-0 DATA This register must bewritten toinaDirect Data Mode write operation tostore thedata intoFIFO1. Data written
must beright-aligned. IftheFIFO isfull,thereaction depends onthesetting oftheHOVF bit(RTPGLBCTRL).
Ifthebitisset,themaster writing thedata willbewait-stated. Ifthebitiscleared, previous data written tothe
register willbeoverwritten.
Reads ofthisregister always return 0.

<!-- Page 2177 -->

www.ti.com RTP Control Registers
2177 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.9 RTP PinControl 0Register (RTPPC0)
This register configures theRTP pins asfunctional orGIO pins. Once thepinisconfigured infunctional
mode, itoverrides thesettings intheRTPPC1 register. Writing totheRTPPC3, RTPPC4, andRTPPC5
registers willhave noeffect forpins configured asfunctional pins. Figure 37-17 andTable 37-18 describe
thisregister.
Figure 37-17. RTP PinControl 0Register (RTPPC0) (offset =34h)
31 19 18 17 16
Reserved ENAFUNC CLKFUNC SYNCFUNC
R-0 R/W-0 R/W-0 R/W-0
15 0
DATAFUNC[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-18. RTP PinControl 0Register (RTPPC0) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENAFUNC Functional mode ofRTPENA pin.
Read:
0 Pinisused inGIO mode.
1 Pinisused infunctional mode.
Write:
0 Configure pintoGIO mode.
1 Configure pintofunctional mode.
17 CLKFUNC Functional mode ofRTPCLK pin.
Read:
0 Pinisused inGIO mode.
1 Pinisused infunctional mode.
Write:
0 Configure pintoGIO mode.
1 Configure pintofunctional mode.
16 SYNCFUNC Functional mode ofRTPSYNC pin.
Read:
0 Pinisused inGIO mode.
1 Pinisused infunctional mode.
Write:
0 Configure pintoGIO mode.
1 Configure pintofunctional mode.
15-0 DATAFUNC[ n] Functional mode ofRTPDATA[15:0] pins. These bitsdefine whether thepins areused infunctional
mode orinGIO mode. Each bit[n]represents asingle pin.
Read:
0 Pinisused inGIO mode.
1 Pinisused infunctional mode.
Write:
0 Configure pintoGIO mode.
1 Configure pintofunctional mode.

<!-- Page 2178 -->

RTP Control Registers www.ti.com
2178 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.10 RTP PinControl 1Register (RTPPC1)
Once thepinisconfigured infunctional mode (using RTPPC0 register), configuring thecorresponding bit
inRTPPC1 to0willnotdisable theoutput driver. Figure 37-18 andTable 37-19 describe thisregister.
Figure 37-18. RTP PinControl 1Register (RTPPC1) (offset =38h)
31 19 18 17 16
Reserved ENADIR CLKDIR SYNCDIR
R-0 R/W-0 R/W-0 R/W-0
15 0
DATADIR[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-19. RTP PinControl 1Register (RTPPC1) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENADIR Direction ofRTPENA pin.This bitdefines whether thepinisused asinput oroutput inGIO mode.
This bithasnoeffect when thepinisconfigured infunctional mode.
Read:
0 Pinisused asinput.
1 Pinisused asoutput.
Write:
0 Configure pintoinput mode.
1 Configure pintooutput mode.
17 CLKDIR Direction ofRTPCLK pin.This bitdefines whether thepinisused asinput oroutput inGIO mode.
This bithasnoeffect when thepinisconfigured infunctional mode.
Read:
0 Pinisused asinput.
1 Pinisused asoutput.
Write:
0 Configure pintoinput mode.
1 Configure pintooutput mode.
16 SYNCDIR Direction ofRTPSYNC pin.This bitdefines whether thepinisused asinput oroutput inGIO mode.
This bithasnoeffect when thepinisconfigured infunctional mode.
Read:
0 Pinisused asinput.
1 Pinisused asoutput.
Write:
0 Configure pintoinput mode.
1 Configure pintooutput mode.
15-0 DATADIR[ n] Direction ofRTPDATA[15:0] pins. These bitsdefine whether thepins areused asinput oroutput in
GIO mode. These bitshave noeffect when thepins areconfigured infunctional mode. Each bit[n]
represents asingle pin.
Read:
0 Pinisused asinput.
1 Pinisused asoutput.
Write:
0 Configure pintoinput mode.
1 Configure pintooutput mode.

<!-- Page 2179 -->

www.ti.com RTP Control Registers
2179 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.11 RTP PinControl 2Register (RTPPC2)
This register represents theinput value ofthepins when inGIO orfunctional mode. Figure 37-19 and
Table 37-20 describe thisregister.
Figure 37-19. RTP PinControl 2Register (RTPPC2) (offset =3Ch)
31 19 18 17 16
Reserved ENAIN CLKIN SYNCIN
R-0 R-x R-x R-x
15 0
DATAIN[15:0]
R-x
LEGEND: R=Read only; -n=value after reset; -x=value isindeterminate
Table 37-20. RTP PinControl 2Register (RTPPC2) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENAIN RTPENA input. This bitreflects thestate ofthepininallmodes. Writes tothisbithave noeffect.
0 The pinisatlogic low(0)(input voltage isVILorlower).
1 The pinisatlogic high (1)(input voltage isVIHorhigher).
17 CLKIN RTPCLK input. This bitreflects thestate ofthepininallmodes. Writes tothisbithave noeffect.
0 The pinisatlogic low(0)(input voltage isVILorlower).
1 The pinisatlogic high (1)(input voltage isVIHorhigher).
16 SYNCIN RTPSYNC input. This bitreflects thestate ofthepininallmodes. Writes tothisbithave noeffect.
0 The pinisatlogic low(0)(input voltage isVILorlower).
1 The pinisatlogic high (1)(input voltage isVIHorhigher).
15-0 DATAIN[ n] RTPDATA[15:0] input. These bitsreflect thestate ofthepins inallmodes. Each bit[n]represents a
single pin.Writes tothisbithave noeffect.
0 The pinisatlogic low(0)(input voltage isVILorlower).
1 The pinisatlogic high (1)(input voltage isVIHorhigher).

<!-- Page 2180 -->

RTP Control Registers www.ti.com
2180 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.12 RTP PinControl 3Register (RTPPC3)
This register defines thestate ofthepins when configured inGIO mode asoutput pins. Once apinis
configured infunctional mode (using RTPPC0 register), changing thestate ofthecorresponding bitin
RTPPC3 willnotaffect thepin's state. Figure 37-20 andTable 37-21 describe thisregister.
Figure 37-20. RTP PinControl 3Register (RTPPC3) (offset =40h)
31 19 18 17 16
Reserved ENAOUT CLKOUT SYNCOUT
R-0 R/W-0 R/W-0 R/W-0
15 0
DATAOUT[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-21. RTP PinControl 3Register (RTPPC3) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENAOUT RTPENA output. This pinsets theoutput state oftheRTPENA pin.
Read:
0 The pinisconfigured tooutput logic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Setpintologic low(0)(output voltage isVOLorlower).
1 Setpintologic high (1)(output voltage isVOHorhigher).
17 CLKOUT RTPCLK output. This pinsets theoutput state oftheRTPCLK pin.
Read:
0 The pinisconfigured tooutput logic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Setpintologic low(0)(output voltage isVOLorlower).
1 Setpintologic high (1)(output voltage isVOHorhigher).
16 SYNCOUT RTPSYNC output. This pinsets theoutput state oftheRTPSYNC pin.
Read:
0 The pinisconfigured tooutput logic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Setpintologic low(0)(output voltage isVOLorlower).
1 Setpintologic high (1)(output voltage isVOHorhigher).
15-0 DATAOUT[ n] RTPDATA[15:0] output. These bitssettheoutput state oftheRTPDATA[15:0] pins. Each bit[n]
represents asingle pin.
Read:
0 The pinisconfigured tooutput logic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Setpintologic low(0)(output voltage isVOLorlower).
1 Setpintologic high (1)(output voltage isVOHorhigher).

<!-- Page 2181 -->

www.ti.com RTP Control Registers
2181 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.13 RTP PinControl 4Register (RTPPC4)
This register provides theoption tosetpins toalogic 1level without influencing thestate ofother pins. It
eliminates theread-modify-write operation necessary with theRTPPC3 register. Once thepinis
configured infunctional mode (using RTPPC0 register), setting thecorresponding bitto1inRTPPC4 will
notaffect thepin's state. Figure 37-21 andTable 37-22 describe thisregister.
Figure 37-21. RTP PinControl 4Register (RTPPC4) (offset =44h)
31 19 18 17 16
Reserved ENASET CLKSET SYNCSET
R-0 R/W-0 R/W-0 R/W-0
15 0
DATASET[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-22. RTP PinControl 4Register (RTPPC4) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENASET Sets theoutput state ofRTPENA pintologic high. Value intheENASET bitsets thedata output
control register bitto1regardless ofthecurrent value intheENAOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic high (1)(output voltage isVOHorhigher).
17 CLKSET Sets theoutput state ofRTPCLK pintologic high. Value intheCLKSET bitsets thedata output
control register bitto1regardless ofthecurrent value intheCLKOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic high (1)(output voltage isVOHorhigher).
16 SYNCSET Sets output state ofRTPSYNC pinlogic high. Value intheSYNCSET bitsets thedata output
control register bitto1regardless ofthecurrent value intheSYNCOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic high (1)(output voltage isVOHorhigher).
15-0 DATASET[ n] Sets output state ofRTPDATA[15:0] pins tologic high. Value intheDATAxSET bitsets thedata
output control register bitto1regardless ofthecurrent value intheDATAxOUT bit.Each bit[n]
represents asingle pin.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic high (1)(output voltage isVOHorhigher).

<!-- Page 2182 -->

RTP Control Registers www.ti.com
2182 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.14 RTP PinControl 5Register (RTPPC5)
This register provides theoption tosetpins toalogic 0level without influencing thestate ofother pins. It
eliminates theread-modify-write operation necessary with theRTPPC3 register. Once thepinis
configured infunctional mode (using RTPPC0 register), setting thecorresponding bitto1inRTPPC5 will
notaffect thepinstate. Figure 37-22 andTable 37-23 describe thisregister.
Figure 37-22. RTP PinControl 5Register (RTPPC5) (offset =48h)
31 19 18 17 16
Reserved ENACLR CLKCLR SYNCCLR
R-0 R/W-0 R/W-0 R/W-0
15 0
DATACLR[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-23. RTP PinControl 5Register (RTPPC5) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENACLR Sets theoutput state ofRTPENA pintologic high. Value intheENASET bitsets thedata output
control register bitto1regardless ofthecurrent value intheENAOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic low(0)(output voltage isVOLorlower).
17 CLKCLR Sets output state ofRTPCLK pintologic low. Value intheCLKCLR bitsets thedata output control
register bitto0regardless ofthecurrent value intheCLKOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic low(0)(output voltage isVOLorlower).
16 SYNCCLR Sets output state ofRTPSYNC pinlogic low. Value intheSYNCCLR bitclears thedata output
control register bitto0regardless ofthecurrent value intheSYNCOUT bit.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic low(0)(output voltage isVOLorlower).
15-0 DATACLR[ n] Sets output state ofRTPDATA[15:0] pins tologic low. Value intheDATAxCLR bitclears thedata
output control register bitto0regardless ofthecurrent value intheDATAxOUT bit.Each bit[n]
represents asingle pin.
Read:
0 The pinisconfigured tooutput alogic low(0)(output voltage isVOLorlower).
1 The pinisconfigured tooutput logic high (1)(output voltage isVOHorhigher).
Write:
0 Noeffect.
1 Setpintologic low(0)(output voltage isVOLorlower).

<!-- Page 2183 -->

www.ti.com RTP Control Registers
2183 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.15 RTP PinControl 6Register (RTPPC6)
This register configures thepins inpush-pull oropen-drain functionality. Ifconfigured tobeopen-drain, the
module only drives alogic lowlevel onthepin.Anexternal pull-up resistor needs tobeconnected tothe
pintopullithigh when thepinisinhigh-impedance mode. Figure 37-23 andTable 37-24 describe this
register.
Figure 37-23. RTP PinControl 6Register (RTPPC6) (offset =4Ch)
31 19 18 17 16
Reserved ENAPDR CLKPDR SYNCPDR
R-0 R/W-0 R/W-0 R/W-0
15 0
DATAPDR[15:0]
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-24. RTP PinControl 6Register (RTPPC6) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENAPDR RTPENA Open drain enable. This bitenables open drain functionality onthepinifitisconfigured
asaGIO output (RTPPC0[18] =0;RTPPC1[18] =1).Ifthepinisconfigured asafunctional pin
(RTPPC0[18] =1),theopen drain functionality isdisabled.
Read:
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen drain mode.
Write:
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.
17 CLKPDR RTPCLK Open drain enable. This bitenables open drain functionality onthepinifitisconfigured
asGIO output (RTPPC0[17] =0;RTPPC1[17] =1).Ifthepinisconfigured asfunctional pin
(RTPPC0[17] =1),theopen drain functionality isdisabled.
Read:
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen drain mode.
Write:
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.
16 SYNCPDR RTPSYNC Open drain enable. This bitenables open drain functionality onthepinifitisconfigured
asaGIO output (RTPPC0[16] =0;RTPPC1[16] =1).Ifpinisconfigured asfunctional pin
(RTPPC0[16] =1),theopen drain functionality isdisabled.
Read:
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen drain mode.
Write:
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.

<!-- Page 2184 -->

RTP Control Registers www.ti.com
2184 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)Table 37-24. RTP PinControl 6Register (RTPPC6) Field Descriptions (continued)
Bit Field Value Description
15-0 DATAPDR[ n] RTPDATA[15:0] Open drain enable. These bitsenable open drain functionality onthepins ifthey
areconfigured asaGIO output (RTPPC0[15:0] =0;RTPPC1[15:0] =1).Ifthepins areconfigured
asafunctional pins (RTPPC0[15:0] =1),theopen drain functionality isdisabled. Each bit[n]
represents asingle pin.
Read:
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen drain mode.
Write:
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.

<!-- Page 2185 -->

www.ti.com RTP Control Registers
2185 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.16 RTP PinControl 7Register (RTPPC7)
This register controls thepullup/down functionality ofapin.The internal pullup/down canbeenabled or
disabled bythisregister. The reset configuration ofthese bitsisdevice dependent, consult thedevice
datasheet forthisinformation. Figure 37-24 andTable 37-25 describe thisregister.
Figure 37-24. RTP PinControl 7Register (RTPPC7) (offset =50h)
31 19 18 17 16
Reserved ENADIS CLKDIS SYNCDIS
R-0 R/W-x R/W-x R/W-x
15 0
DATADIS[15:0]
R/W-x
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; -x=value isindeterminate
Table 37-25. RTP PinControl 7Register (RTPPC7) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENADIS RTPENA Pulldisable. This bitremoves internal pullup/pulldown functionality from thepinwhen itis
configured asaninput pin(RTPPC1[18] =0).
Read:
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Write:
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
17 CLKDIS RTPCLK Pulldisable. This bitremoves theinternal pullup/pulldown functionality from thepinwhen
itisconfigured asaninput pin(RTPPC1[17] =0).
Read:
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Write:
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
16 SYNCDIS RTPSYNC Pulldisable. Removes internal pullup/pulldown functionality from thepinwhen
configured asaninput pin(RTPPC1[16] =0).
Read:
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Write:
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
15-0 DATADIS[ n] RTPDATA[15:0] Pulldisable. Removes internal pullup/pulldown functionality from thepins when
configured asinput pins (RTPPC1[15:0] =0).Each bit[n]represents asingle pin.
Read:
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Write:
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.

<!-- Page 2186 -->

RTP Control Registers www.ti.com
2186 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedRAM Trace Port (RTP)37.3.17 RTP PinControl 8Register (RTPPC8)
This register configures theinternal pullup orpulldown ontheinput pin.Asecondary function exists when
thepullconfiguration isdisabled andapulldown isselected. This willdisable theinput buffer. Figure 37-25
andTable 37-26 describe thisregister.
NOTE: Ifthepullup/down isdisabled intheRTPPC7 register andconfigured aspulldown in
RTPPC8, then theinput buffer isdisabled.
Figure 37-25. RTP PinControl 8Register (RTPPC8) (offset =54h)
31 19 18 17 16
Reserved ENAPSEL CLKPSEL SYNCPSEL
R-0 R/W-1 R/W-1 R/W-1
15 0
DATAPSEL[15:0]
R/W-1
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 37-26. RTP PinControl 8Register (RTPPC8) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18 ENAPSEL RTPENA Pullselect. This bitconfigures pullup orpulldown functionality ifRTPPC7[18] =0.
Read:
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Write:
0 Enables pulldown functionality.
1 Enables pullup functionality.
17 CLKPSEL RTPCLK Pullselect. This bitconfigures pullup orpulldown functionality ifRTPPC7[17] =0.
Read:
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Write:
0 Enables pulldown functionality.
1 Enables pullup functionality.
16 SYNCPSEL RTPSYNC Pullselect. This bitconfigures pullup orpulldown functionality ifRTPPC7[16] =0.
Read:
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Write:
0 Enables pulldown functionality.
1 Enables pullup functionality.
15-0 DATAPSEL[ n] RTPDATA[15:0] Pullselect. These bitsconfigure pullup orpulldown functionality ifRTPPC7[15:0] =
0.Each bit[n]represents asingle pin.
Read:
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Write:
0 Enables pulldown functionality.
1 Enables pullup functionality.