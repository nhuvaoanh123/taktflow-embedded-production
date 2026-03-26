# Data Modification Module (DMM)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 2108-2153

---


<!-- Page 2108 -->

2108 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Chapter 36
SPNU563A -March 2018
Data Modification Module (DMM)
This chapter describes thefunctionality oftheData Modification Module (DMM), which provides the
capability tomodify data intheentire 4GBaddress space ofthedevice from anexternal peripheral, with
minimal interruption oftheapplication.
Topic ........................................................................................................................... Page
36.1 Overview ........................................................................................................ 2109
36.2 Module Operation ........................................................................................... 2110
36.3 Control Registers ............................................................................................ 2115

<!-- Page 2109 -->

SIZE ADDRESS ATA DATA
DATA ADDRESS
BASEADDRSIZETo Main SCR
Control
SIZE STAT DEST ADDR ATA DATA31 063 0
87 0Buffer2
Buffer1
Memory protection
(destination registers)
Deserializer
...
www.ti.com Overview
2109 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.1 Overview
36.1.1 Features
The DMM module hasthefollowing features:
*Acts asabusmaster, thus enabling direct writes tothe4GB address space without CPU intervention
*Writes tomemory locations specified inthereceived packet (leverages packets defined bytrace mode
oftheRAM trace port (RTP) module
*Writes received data toconsecutive addresses, which arespecified bytheDMM module (leverages
packets defined bydirect data mode ofRTP module)
*Configurable port width (1,2,4,8,16pins)
*Upto100Mbit/s pindata rate
*Unused pins configurable asGIO pins
36.1.2 Block Diagram
Figure 36-1 shows theblock diagram fortheDMM.
Figure 36-1. DMM Block Diagram

<!-- Page 2110 -->

Module Operation www.ti.com
2110 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.2 Module Operation
The DMM receives data over theDMM pins from external systems andwrites thereceived data directly to
thebase address programmed inthemodule plus offset address given inthepacket orintoabuffer
specified bystart address andlength. Itleverages theprotocol defined bytheRAM Trace Port (RTP)
module tohave acommon interface definition forexternal systems. Itcanalso beused toconnect anRTP
andDMM module together forfastprocessor intercommunication.
The DMM module provides twomodes ofoperation:
*Trace Mode: Inthismode, theDMM writes thereceived data directly toanaddress thatiscalculated
from thebase address programmed intothedestination register (Section 36.3.12 ;Section 36.3.14 )
plus theoffset address contained inthereceived packet. Aninterrupt canbegenerated when data is
written thelowest address ofaprogrammed region. This capability enables thesender toraise an
interrupt atthereceiver while sending specific information.
*Direct Data Mode: Inthismode, theDMM writes thereceived data intoanaddress range ofthe4GB
address space. The buffer start address (Section 36.3.8 )andblocksize (Section 36.3.9 )is
programmable intheDMM module. When thebuffer reaches itsendaddress, thebuffer pointer wraps
around andpoints tothebeginning ofthebuffer again. The EO_BUFF flag(Section 36.3.5 )willbeset
andifenabled, aninterrupt willbegenerated toindicate abuffer-full condition. Another interrupt, can
beconfigured toindicate different buffer filllevels. This canbeaccomplished byprogramming acertain
filllevel intotheDMMINTPT register (Section 36.3.11 ).The PROG_BUFF flag(Section 36.3.5 )
indicates thatthislevel hasbeen reached.
Data willbecaptured bytheinput buffer andmoved totheappropriate bitfield inthedeseralizer. When
thedeseralizer iscompletely full,thedata willbemoved totheoutput buffer register. Atwo-level buffer is
implemented toavoid overflow conditions iftheinternal busisoccupied byother transactions. Inaddition
theDMMENA signal canbeused tosignal theexternal hardware thatanoverflow might occur ifmore
data issent. The automatic generation oftheDMMENA signal canbeconfigured bysetting theENAFUNC
bit(Section 36.3.16 ).While theDMMENA signal isactive, theDMM module willnotreceive anynew data.
The DMM isabusmaster andforwards thereceived data tothebussystem. The write operation willbe
minimally intrusive totheprogram flow, because theCPU/DMA access willonly beblocked ifthe
CPU/DMA accesses thesame resource astheDMM.
Toprevent anexternal system from overwriting critical data inthememory while configured inTrace
Mode, amemory protection mechanism isimplemented viaaprogrammable start address andblock size
ofaregion. Amaximum offour destinations with tworegions each aresupported.
Forproper operation, atleast DMMCLK, DMMSYNC andDMMDATA[0] need tobeprogrammed in
functional mode (Section 36.3.16 ).Ifalarge amount ofdata should betransmitted inashort time, more
data pins should beused infunctional mode. The module supports 1,2,4,8,or16-pin configurations.
The module canbeconfigured tohandle afree running clock provided onDMMCLK (Section 36.3.1 ).
Clock pulses between twoDMMSYNC pulses thatexceed thenumber ofvalid clock pulses forapacket
willbeignored.
36.2.1 Data Format
Below isadescription ofthepacket andframe format.
36.2.1.1 Clocking Scheme
The DMM supports both continuous andnoncontinuous clocking. The clock received onDMMCLK inthe
continuous clocking scheme isafree-running clock. Innoncontinuous clocking scheme, theclock willstop
after each packet andwillstart with thereception ofaDMMSYNC signal.
36.2.1.2 Trace Mode Packet
Figure 36-2 illustrates thetrace mode packet format. One packet consists of2bits(DEST) denoting the
destination inwhich thedata isstored, 2status bits(STAT), the2-bit SIZE ofthedata, the18-bit address
ofwhere thedata should bewritten to,andavariable data field.

<!-- Page 2111 -->

8, 16, or 32 bit
HWDATA(xx-0)
2+2+2+18+2     x8 bitSIZE
DEST(1-0) STAT(1-0) SIZE(1-0) ADDR(17-0) DATA(xx-0)
www.ti.com Module Operation
2111 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)The DEST bits(Table 36-1)willbeused todetermine which destination register applies tothetransmitted
data andthereceived address determines ifthepacket falls intoavalid region ofthedestination area. If
theaddress isvalid, thebase address, programmed inoneofthedestination registers (Section 36.3.12 ;
Section 36.3.14 )ofthisparticular region willbeapplied tocreate thecomplete 32-bit address forthe
destination. The DMM module only takes action ona"11" setting oftheSTAT bits(Table 36-2 ).This
signals thatanoverflow inthetransmitting hardware module hasoccurred. Ifthisisthecase the
SRC_OVF flag(Section 36.3.5 )willbesetandthereceived data willbewritten totheaddress specified in
thepacket. The size information ofthedata transmitted inthepacket isdenoted intheSIZE bits
(Table 36-3)ofthepacket. Depending ontheSIZE information, themodule expects toreceive only this
amount ofdata.
Figure 36-2. Trace Mode Packet Format
Table 36-1 through Table 36-3 illustrate theencoding ofpacket format intrace mode.
Table 36-1. Encoding ofDestination Bits inTrace Mode Packet Format
DEST[1:0] Destination
00 Dest 0
01 Dest 1
10 Dest 2
11 Dest 3
Table 36-2. Encoding ofStatus Bits inTrace Mode Packet Format
STAT[1:0] Status
00 don'tcare
01 don'tcare
10 don'tcare
11 overflow
Table 36-3. Encoding ofWrite Size inPacket Format
SIZE[1:0] Write Size
00 8bit
01 16bit
10 32bit
11 64bit
36.2.1.3 Direct Data Mode Packet
Figure 36-3 illustrates thedirect data mode packet format.
Figure 36-3. Direct Data Mode Packet Format

<!-- Page 2112 -->

Module Operation www.ti.com
2112 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)The packet consists only ofdata bitsandnoheader information. Itcanbe8-,16-or32-bit wide. Avariable
packet width isnotsupported because theDMM module willcheck thenumber ofincoming bits(DMMCLK
cycles) forerror detection. The DMM willwrite thereceived data tothedestination once theprogrammed
number ofbitshasbeen received.
Iftheprogrammed word width does notcorrespond tothereceived data, thefollowing actions willbe
taken:
*Ifthereceived data isgreater than theprogrammed width, only theconfigured number ofbitsare
transferred intotheRAM buffer, theadditional bitsarediscarded.
*Ifthereceived number ofbitsissmaller than theprogrammed width, nodata willbewritten tothe
buffer, because anew DMMSYNC signal hasbeen received before theexpected number ofbits.
36.2.2 Data Port
The packet willbereceived inseveral subpackets, depending onthewidth oftheexternal data bus
(DMMDATA[y:0]) andtheamount ofdata tobetransmitted. Table 36-4 illustrates thenumber ofclock
cycles required foracomplete packet.
Table 36-4. Number ofClock Cycles perPacket
Port Width/ PinsWrite Size inBits
8 16 32 64
1 32 40 56 88
2 16 20 28 44
4 8 10 14 22
8 4 5 7 11
16 2 3 4 6
The user canprogram theport width intheDMMPC0 register (Section 36.3.16 ).This feature allows pins
thatarenotused forDMM functionality tobeused asGIO pins. Only thepins shown inTable 36-5 canbe
used foradesired port width.
Table 36-5. Pins Used forData Communication
Port Width Pins Used
1 DMMDATA[0]
2 DMMDATA[1:0]
4 DMMDATA[3:0]
8 DMMDATA[7:0]
16 DMMDATA[15:0]
NOTE: Ifpins other than theones specified inTable 36-5 areprogrammed asfunctional pins fora
desired port width, thereceived data willbecorrupted andwillnotbetransferred tothe
deserializer.
NOTE: IfDMMCLK orDMMSYNC areprogrammed asnonfunctional pins, functional operation will
notoccur.

<!-- Page 2113 -->

DEST[1] SIZE[1] ADDR[15] ADDR[11] ADDR[7] ADDR[3] DATA[7] DATA[3]
DEST[0]
ADDR[13]ADDR[6] ADDR[2] DATA[2] DATA[6]
DATA[5]
DATA[4]ADDR[9] ADDR[5] ADDR[1] DATA[1]
ADDR[4] ADDR[0] DATA[0]SIZE[0] ADDR[14] ADDR[10]
ADDR[12] ADDR[8]STAT[1]
STAT[0]ADDR[17]
ADDR[16]DMMCLK
DMMSYNC
DMMDATA[0]
DMMDATA[1]
DMMDATA[2]
DMMDATA[3]
Packet1 Packet2 Packet3 Packet4 Packet1 Packet2 Packet3DMMCLK
DMMSYNC
DMMDATA
www.ti.com Module Operation
2113 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.2.2.1 Signal Description
DMMSYNC This signal hastobeprovided byexternal hardware. Itsignals thestart ofanew
packet. Ithastobeactive (high) foronefullDMMCLK cycle, starting with therising
edge ofDMMCLK. IftheDMMSYNC pulse islonger than asingle DMMCLK cycle
andtwofalling edges ofDMMCLK seeahigh pulse onDMMSYNC, themodule will
treat thesecond DMMSYNC pulse asthestart ofapacket andwillflaga
PACKET_ERR_INT (Section 36.3.5 ).
DMMCLK The clock isexternally generated andcanbesuspended between twopackets. For
thisfeature, CONTCLK must besetto0(Section 36.3.1 ).Iftheclock isnotstopped
between twopackets, CONTCLK must besetto1.Data willbelatched onthe
falling edge oftheDMMCLK signal.
DMMENA This signal ispulled high ifnonew data should bereceived viathedata pins,
because ofapotential overflow situation.
DMMDATA[15:0] These pins receive thepacket information transmitted bytheexternal hardware.
Data islatched onthefalling edge ofDMMCLK.
Figure 36-4 shows anexample ofmultiple packets received during trace mode, innoncontinuous clock
configuration.
Figure 36-4. Packet Sync Signal Example
Figure 36-5 shows anexample ofa4-bit data port with 8-bit receive data (A5h) tobewritten intoDEST1
(address 0001 2345h) onatrace mode packet.
Figure 36-5. Example Single Packet Transmission
36.2.3 Error Handling
The module willgenerate twodifferent kind oferrors. Once anerror condition isrecognized, aninterrupt
willbegenerated ifenabled.

<!-- Page 2114 -->

Level0 Interrupt to VIM
Level1 Interrupt to VIMModule InterruptFlag
Interrupt EnableInterrupt Level
Module Operation www.ti.com
2114 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.2.3.1 Overflow Error
This error issignaled when themodule hasreceived new data before theprevious data was written tothe
destination address. Iftheinternal buffers arefull,theDMMENA signal willgohigh. Ifthesending module
does notevaluate theDMMENA signal andkeeps onsending new frames, thedata thatwas previously
received might beoverwritten, thus resulting insetting theBUFF_OVF flag(Section 36.3.5 ).
36.2.3.2 Packet Error
Noncontinuous Clock Mode
The size oftheincoming packet isdefined bytheSIZE information ofatrace mode packet orthe
programmed size ofadirect data mode packet. Iftoomany orless than thenumber ofbitsarereceived
before thenext sync signal, thePACKET_ERR_INT flagwillbeset(Section 36.3.5 ).Incase ofreceiving a
DMMCLK signal without acorresponding DMMSYNC signal, apacket error willalso begenerated.
Continuous Clock Mode
Ifless than theexpected number ofbitsarereceived, thePACKET_ERR_INT flagwillbeset
(Section 36.3.5 )when thenext DMMSYNC signal isreceived. Packets with more than theexpected
number ofbitscannot bedetected.
The check forpacket error isdone only after thedetection ofthefirstDMMSYNC signal after theDMM is
turned onorcomes outofsuspend mode (with COS =0;Section 36.3.1 ),thatis,before thereception of
firstDMMSYNC, thetoggling ofDMMCLK would beignored.
36.2.3.3 Bus Error
Ifanerror occurs onthemicrocontroller internal bussystem while transferring thedata from theDMM to
thedestination, theBUSERROR flagwillbeset.
36.2.4 Interrupts
The module provides different interrupts. These canbeprogrammed todifferent interrupt levels
independently using DMMINTLVL (Section 36.3.4 ).
Figure 36-6. Interrupt Structure
Interrupts canbedivided intoerror interrupts andfunctional interrupts. The error handling isdescribed in
Section 36.2.3 .Functional interrupts depend onthemode (Trace Mode, Direct Data Mode) theDMM
module isused in.
Trace Mode: Aninterrupt canbeenabled whenever anaccess tothelowest address ofadefined region
isperformed. This address isthestarting address programmed intheDMMDESTxREGy register. An
interrupt foreach oftheregion canbegenerated bysetting theindividual interrupt enable bits.
Direct Data Mode: There aretwointerrupts thatcanbeindividually controlled. One isgenerated when the
buffer pointer reaches theendofthedefined buffer andwraps around (EO_BUFF; Section 36.3.2 ).The
other oneisgenerated when thebuffer pointer matches theprogrammed interrupt threshold
(PROG_BUFF; Section 36.3.2 ).The buffer pointer points tothenext address tobewritten, therefor there
are(interrupt threshold -1)values stored inthebuffer. The interrupt threshold canbeprogrammed inthe
DMMINTPT register (Section 36.3.11 ).

<!-- Page 2115 -->

www.ti.com Control Registers
2115 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3 Control Registers
This section describes theDMM registers. The registers support 8,16,and32-bit writes. The offset is
relative totheassociated peripheral select. Table 36-6 provides asummary oftheregisters andtheir bits.
The base address oftheDMM module registers isFFFF F700h.
Table 36-6. DMM Registers
Offset Acronym Register Description Section
0h DMMGLBCTRL DMM Global Control Register Section 36.3.1
4h DMMINTSET DMM Interrupt SetRegister Section 36.3.2
8h DMMINTCLR DMM Interrupt Clear Register Section 36.3.3
0Ch DMMINTLVL DMM Interrupt Level Register Section 36.3.4
10h DMMINTFLG DMM Interrupt Flag Register Section 36.3.5
14h DMMOFF1 DMM Interrupt Offset 1Register Section 36.3.6
18h DMMOFF2 DMM Interrupt Offset 2Register Section 36.3.7
1Ch DMMDDMDEST DMM Direct Data Mode Destination Register Section 36.3.8
20h DMMDDMBL DMM Direct Data Mode Blocksize Register Section 36.3.9
24h DMMDDMPT DMM Direct Data Mode Pointer Register Section 36.3.10
28h DMMINTPT DMM Direct Data Mode Interrupt Pointer Register Section 36.3.11
2Ch, 3Ch, 4Ch, 5Ch DMMDESTxREG1 DMM Destination xRegion 1 Section 36.3.12
30h, 40h, 50h, 60h DMMDESTxBL1 DMM Destination xBlocksize 1 Section 36.3.13
34h, 44h, 54h, 64h DMMDESTxREG2 DMM Destination xRegion 2 Section 36.3.14
38h, 48h, 58h, 68h DMMDESTxBL2 DMM Destination xBlocksize 2 Section 36.3.15
6Ch DMMPC0 DMM PinControl 0 Section 36.3.16
70h DMMPC1 DMM PinControl 1 Section 36.3.17
74h DMMPC2 DMM PinControl 2 Section 36.3.18
78h DMMPC3 DMM PinControl 3 Section 36.3.19
7Ch DMMPC4 DMM PinControl 4 Section 36.3.20
80h DMMPC5 DMM PinControl 5 Section 36.3.21
84h DMMPC6 DMM PinControl 6 Section 36.3.22
88h DMMPC7 DMM PinControl 7 Section 36.3.23
8Ch DMMPC8 DMM PinControl 8 Section 36.3.24

<!-- Page 2116 -->

Control Registers www.ti.com
2116 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.1 DMM Global Control Register (DMMGLBCTRL)
With thisregister thebasic operation ofthemodule isselected.
Figure 36-7. DMM Global Control Register (DMMGLBCTRL) [offset =00h]
31 25 24
Reserved BUSY
R-0 R-0
23 19 18 17 16
Reserved CONTCLK COS RESET
R-0 R/WP-0 R/WP-0 R/WP-0
15 11 10 9 8
Reserved DDM_WIDTH TM_DDM
R-0 R/WP-0 R/WP-0
7 4 3 0
Reserved ON/OFF
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-7. DMM Global Control Register (DMMGLBCTRL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads returns 0.Writes have noeffect.
24 BUSY Busy indicator.
0 The DMM does notcurrently receive data andhasnodata initsinternal buffers, which
needs tobetransferred.
1 The module iscurrently receiving data, orhasdata initsinternal buffers.
23-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 CONTCLK Continuous DMMCLK input.
User andprivilege mode read, privilege mode write:
0 DMMCLK isexpected tobesuspended between twopackets.
1 DMMCLK isexpected tobefree running between packets.
17 COS Continue onsuspend. Influences behavior ofmodule while indebug mode. Inallcases the
corresponding interrupt willbeset.
User andprivilege mode (read):
0 Packets willnotbereceived during debug mode. Before entering debug mode, theongoing
reception ofapacket willbefinished andthevalue willbewritten tothedestination.
1 Continue receiving packets andupdate destination, while indebug mode.
Privilege mode (write):
0 Disable data reception while indebug mode.
1 Enable data reception while indebug mode.
16 RESET Reset. This bitresets thestate machine andtheregisters toitsreset value, except the
RESET bititself. Itmust becleared bywriting toit.
User andprivilege mode (read):
0 Noreset ofDMM module.
1 Reset ofDMM module.
Privilege mode (write):
0 Noreset ofDMM module.
1 Reset DMM module toitsreset state.
15-11 Reserved 0 Reads returns 0.Writes have noeffect.

<!-- Page 2117 -->

www.ti.com Control Registers
2117 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-7. DMM Global Control Register (DMMGLBCTRL) Field Descriptions (continued)
Bit Field Value Description
10-9 DDM_WIDTH Packet Width indirect data mode.
User andprivilege mode read andprivilege mode write operation:
BitEncoding Transfer Size
0 8bit
1h 16bit
2h 32bit
3h Reserved
8 TM_DMM Packet Format.
User andprivilege mode (read):
0 The DMM module assumes packets intrace mode definition.
1 The DMM module assumes packets indirect data mode definition.
Privilege mode (write):
0 Enable trace mode.
1 Enable direct data mode.
7-4 Reserved 0 Reads returns 0.Writes have noeffect.
3-0 ON/OFF Switch module onoroff
User andprivilege mode (read):
Allother The DMM module does notreceive data.
Ah The DMM module receives data andwrites ittothebuffer.
Privilege mode (write):
Allother Disable receive/write operations. Packets inreception, willstillbefinished.
Ah Enable receive/write operations. Packets willbereceived 1HCLK cycle after enabling the
module.
NOTE: Itisrecommended towrite 5htoON/OFF toavoid having asofterror inadvertently enabling
themodule when asingle bitflips.
NOTE: Registers thataffect theoperation ofthemodule, should beonly programmed when the
BUSY bitis0andtheON/OFF bitsarenotAh.
NOTE: Ifthemodule was inoperation, turned off(ON/OFF =allother than Ah)andthen turned on
(ON/OFF =Ah)again, itisrecommended toperform areset (RESET =1)ofthemodule
before switching iton.This avoids thatthestate machine isheld inanunrecoverable state.
NOTE: Awrite tothese register bitswhile receiving apacket willnothave anyeffect onthereceived
packet. The mode change willbeperformed after thepacket isreceived

<!-- Page 2118 -->

Control Registers www.ti.com
2118 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.2 DMM Interrupt SetRegister (DMMINTSET)
This register contains theinterrupt setbitsforerror interrupts andfunctional interrupts. Only thebitswhich
arerelevant fortheparticular mode (trace mode ordirect data mode) willbetaken intoaccount forthe
interrupt generation.
Figure 36-8. DMM Interrupt SetRegister (DMMINTSET) [offset =04h]
31 24
Reserved
R-0
23 18 17 16
Reserved PROG_BUFF EO_BUFF
R-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DEST3REG2 DEST3REG1 DEST2REG2 DEST2REG1 DEST1REG2 DEST1REG1 DEST0REG2 DEST0REG1
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
BUSERROR BUFF_OVF SRC_OVF DEST3_ERR DEST2_ERR DEST1_ERR DEST0_ERR PACKET_
ERR_INT
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-8. DMM Interrupt SetRegister (DMMINTSET) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads returns 0.Writes have noeffect.
17 PROG_BUFF Programmable Buffer Interrupt Set. This enables theinterrupt generation incase thebuffer
pointer equals theprogrammed value intheDMMINTPT register (Section 36.3.11 ).This bitis
only relevant inDirect Data Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onpointer match.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
16 EO_BUFF End ofBuffer Interrupt Set. This enables theinterrupt generation incase data was written to
thelastentry inthebuffer andthepointer wrapped around tothebeginning ofthebuffer. This
bitisonly relevant inDirect Data Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onwriting tothelastentry.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
15 DEST3REG2 Destination 3Region 2Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 3Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).

<!-- Page 2119 -->

www.ti.com Control Registers
2119 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-8. DMM Interrupt SetRegister (DMMINTSET) Field Descriptions (continued)
Bit Field Value Description
14 DEST3REG1 Destination 3Region 1Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 3Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
13 DEST2REG2 Destination 2Region 2Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 2Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
12 DEST2REG1 Destination 2Region 1Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 2Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
11 DEST1REG2 Destination 1Region 2Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 1Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
10 DEST1REG1 Destination 1Region 1Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 1Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
9 DEST0REG2 Destination 0Region 2Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 0Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).

<!-- Page 2120 -->

Control Registers www.ti.com
2120 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-8. DMM Interrupt SetRegister (DMMINTSET) Field Descriptions (continued)
Bit Field Value Description
8 DEST0REG1 Destination 0Region 1Interrupt Set. This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 0Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
7 BUSERROR Bus Error Response forerrors generated when doing internal bustransfers.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
6 BUFF_OVF Buffer Overflow. This enables theinterrupt generation incase new data isreceived, while the
previous data stillhasnotbeen transmitted.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
5 SRC_OVF Source Overflow. This enables aninterrupt iftheexternal system experienced andoverflow
thatwas signaled intheTrace Mode packet.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
4 DEST3_ERR Destination 3Error. This enables theinterrupt generation incase data should bewritten intoa
address notspecified byDMMDEST3REG1/DMMDEST3BL1 or
DMMDEST3REG2/DMMDEST3BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).

<!-- Page 2121 -->

www.ti.com Control Registers
2121 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-8. DMM Interrupt SetRegister (DMMINTSET) Field Descriptions (continued)
Bit Field Value Description
3 DEST2_ERR Destination 2Error Interrupt Set. This enables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST2REG1/DMMDEST2BL1 or
DMMDEST2REG2/DMMDEST2BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
2 DEST1_ERR Destination 1Error Interrupt Set. This enables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST1REG1/DMMDEST1BL1 or
DMMDEST1REG2/DMMDEST1BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
1 DEST0_ERR Destination 0Error Interrupt Set. This enables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST0REG1/DMMDEST0BL1 or
DMMDEST0REG2/DMMDEST0BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).
0 PACKET_ERR_INT Packet Error. This enables theinterrupt generation incase ofanerror condition inthepacket
reception. Please refer toSection 36.2.3 fortheerror conditions.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Enable interrupt (sets corresponding bitinDMMINTCLR; DMMINTLVL).

<!-- Page 2122 -->

Control Registers www.ti.com
2122 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.3 DMM Interrupt Clear Register (DMMINTCLR)
This register contains theinterrupt clear bitsforerror interrupts andfunctional interrupts. Only thebits
which arerelevant fortheparticular mode (trace mode ordirect data mode) willbetaken intoaccount for
theinterrupt generation
Figure 36-9. DMM Interrupt Clear Register (DMMINTCLR) [offset =08h]
31 24
Reserved
R-0
23 18 17 16
Reserved PROG_BUFF EO_BUFF
R-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DEST3REG2 DEST3REG1 DEST2REG2 DEST2REG1 DEST1REG2 DEST1REG1 DEST0REG2 DEST0REG1
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
BUSERROR BUFF_OVF SRC_OVF DEST3_ERR DEST2_ERR DEST1_ERR DEST0_ERR PACKET_
ERR_INT
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-9. DMM Interrupt Clear Register (DMMINTCLR) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads returns 0.Writes have noeffect.
17 PROG_BUFF Programmable Buffer Interrupt Set.This disables theinterrupt generation incase thebuffer
pointer equals theprogrammed value intheDMMINTPT register (Section 36.3.11 ).This bitis
only relevant inDirect Data Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onpointer match.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
16 EO_BUFF End ofBuffer Interrupt Set.This disables theinterrupt generation incase data was written to
thelastentry inthebuffer andthepointer wrapped around tothebeginning ofthebuffer. This
bitisonly relevant inDirect Data Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onwriting tothelastentry.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).

<!-- Page 2123 -->

www.ti.com Control Registers
2123 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-9. DMM Interrupt Clear Register (DMMINTCLR) Field Descriptions (continued)
Bit Field Value Description
15 DEST3REG2 Destination 3Region 2Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 3Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
14 DEST3REG1 Destination 3Region 1Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 3Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
13 DEST2REG2 Destination 2Region 2Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 2Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
12 DEST2REG1 Destination 2Region 1Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 2Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
11 DEST1REG2 Destination 1Region 2Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 1Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).

<!-- Page 2124 -->

Control Registers www.ti.com
2124 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-9. DMM Interrupt Clear Register (DMMINTCLR) Field Descriptions (continued)
Bit Field Value Description
10 DEST1REG1 Destination 1Region 1Interrupt Set.This enables theinterrupt generation incase data was
accessed atthestart address ofDestination 1Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
9 DEST0REG2 Destination 0Region 2Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 0Region 2.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
8 DEST0REG1 Destination 0Region 1Interrupt Set.This disables theinterrupt generation incase data was
accessed atthestart address ofDestination 0Region 1.This bitisonly relevant inTrace Mode.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated onawrite tothestart address ofthisregion.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
7 BUSERROR Bus Error Response forerrors generated when doing internal bustransfers.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
6 BUFF_OVF Buffer Overflow. This disables theinterrupt generation incase new data isreceived, while the
previous data stillhasnotbeen transmitted.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).

<!-- Page 2125 -->

www.ti.com Control Registers
2125 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-9. DMM Interrupt Clear Register (DMMINTCLR) Field Descriptions (continued)
Bit Field Value Description
5 SRC_OVF Source Overflow. This disables aninterrupt iftheexternal system experienced andoverflow
which was signaled intheTrace Mode packet.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
4 DEST3_ERR Destination 3Error. This disables theinterrupt generation incase data should bewritten intoa
address notspecified byDMMDEST3REG1/DMMDEST3BL1 or
DMMDEST3REG2/DMMDEST3BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
3 DEST2_ERR Destination 2Error Interrupt Set.This disables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST2REG1/DMMDEST2BL1 or
DMMDEST2REG2/DMMDEST2BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
2 DEST1_ERR Destination 1Error Interrupt Set.This disables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST1REG1/DMMDEST1BL1 or
DMMDEST1REG2/DMMDEST1BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).

<!-- Page 2126 -->

Control Registers www.ti.com
2126 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-9. DMM Interrupt Clear Register (DMMINTCLR) Field Descriptions (continued)
Bit Field Value Description
1 DEST0_ERR Destination 0Error Interrupt Set.This disables theinterrupt generation incase data should be
written intoaaddress notspecified byDMMDEST0REG1/DMMDEST0BL1 or
DMMDEST0REG2/DMMDEST0BL2. Ifboth blocksizes areprogrammed to0orareserved
value, theinterrupt willstillbegenerated, thewrite totheinternal RAM however willnottake
place.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).
0 PACKET_ERR_INT Packet Error. This disables theinterrupt generation incase ofanerror condition inthepacket
reception. Please refer toSection 36.2.3 fortheerror conditions.
User andprivilege mode (read):
0 Nointerrupt willbegenerated.
1 Aninterrupt willbegenerated.
Privilege mode (write):
0 Noinfluence onbit.
1 Disable interrupt (clears corresponding bitinDMMINTCLR; DMM Interrupt Level Register
(DMMINTLVL)).

<!-- Page 2127 -->

www.ti.com Control Registers
2127 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.4 DMM Interrupt Level Register (DMMINTLVL)
This register contains theinterrupt level bitsforerror interrupts andnormal interrupts.
Figure 36-10. DMM Interrupt Level Register (DMMINTLVL) [offset =0Ch]
31 24
Reserved
R-0
23 18 17 16
Reserved PROG_BUFF EO_BUFF
R-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DEST3REG2 DEST3REG1 DEST2REG2 DEST2REG1 DEST1REG2 DEST1REG1 DEST0REG2 DEST0REG1
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
BUSERROR BUFF_OVF SRC_OVF DEST3_ERR DEST2_ERR DEST1_ERR DEST0_ERR PACKET_
ERR_INT
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 36-10. DMM Interrupt Level Register (DMMINTLVL) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads returns 0.Writes have noeffect.
17 PROG_BUFF Programmable Buffer Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
16 EO_BUFF End ofBuffer Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
15 DEST3REG2 Destination 3Region 2Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
14 DEST3REG1 Destination 3Region 1Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
13 DEST2REG2 Destination 2Region 2Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
12 DEST2REG1 Destination 2Region 1Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
11 DEST1REG2 Destination 1Region 2Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.

<!-- Page 2128 -->

Control Registers www.ti.com
2128 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-10. DMM Interrupt Level Register (DMMINTLVL) Field Descriptions (continued)
Bit Field Value Description
10 DEST1REG1 Destination 1Region 1Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
9 DEST0REG2 Destination 0Region 2Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
8 DEST0REG1 Destination 0Region 1Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
7 BUSERROR BMM Bus Error Response
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
6 BUFF_OVF Write Buffer Overflow Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
5 SRC_OVF Source Overflow Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
4 DEST3_ERR Destination 3Error Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
3 DEST2_ERR Destination 2Error Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
2 DEST1_ERR Destination 1Error Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
1 DEST0_ERR Destination 0Error Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.
0 PACKET_ERR_INT Packet Error Interrupt Level
User andprivilege mode read, privilege mode write:
0 Interrupt mapped tolevel 0.
1 Interrupt mapped tolevel 1.

<!-- Page 2129 -->

www.ti.com Control Registers
2129 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.5 DMM Interrupt Flag Register (DMMINTFLG)
This register contains theinterrupt level bitsforerror interrupts andnormal interrupts.
Figure 36-11. DMM Interrupt Flag Register (DMMINTFLG) [offset =10h]
31 24
Reserved
R-0
23 18 17 16
Reserved PROG_BUFF EO_BUFF
R-0 R/WPC-0 R/WPC-0
15 14 13 12 11 10 9 8
DEST3REG2 DEST3REG1 DEST2REG2 DEST2REG1 DEST1REG2 DEST1REG1 DEST0REG2 DEST0REG1
R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0
7 6 5 4 3 2 1 0
BUSERROR BUFF_OVF SRC_OVF DEST3_ERR DEST2_ERR DEST1_ERR DEST0_ERR PACKET_
ERR_INT
R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0 R/WPC-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; C=Clear; -n=value after reset
Table 36-11. DMM Interrupt Flag Register (DMMINTFLG) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads returns 0.Writes have noeffect.
17 PROG_BUFF Programmable Buffer Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
16 EO_BUFF End ofBuffer Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
15 DEST3REG2 Destination 3Region 2Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
14 DEST3REG1 Destination 3Region 1Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.

<!-- Page 2130 -->

Control Registers www.ti.com
2130 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-11. DMM Interrupt Flag Register (DMMINTFLG) Field Descriptions (continued)
Bit Field Value Description
13 DEST2REG2 Destination 2Region 2Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
12 DEST2REG1 Destination 2Region 1Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
11 DEST1REG2 Destination 1Region 2Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
10 DEST1REG1 Destination 1Region 1Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
9 DEST0REG2 Destination 0Region 2Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
8 DEST0REG1 Destination 0Region 1Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.

<!-- Page 2131 -->

www.ti.com Control Registers
2131 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-11. DMM Interrupt Flag Register (DMMINTFLG) Field Descriptions (continued)
Bit Field Value Description
7 BUSERROR BMM Bus Error Response.
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
6 BUFF_OVF Write Buffer Overflow Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
5 SRC_OVF Source Overflow Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
4 DEST3_ERR Destination 3Error Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
3 DEST2_ERR Destination 2Error Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
2 DEST1_ERR Destination 1Error Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.

<!-- Page 2132 -->

Control Registers www.ti.com
2132 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-11. DMM Interrupt Flag Register (DMMINTFLG) Field Descriptions (continued)
Bit Field Value Description
1 DEST0_ERR Destination 0Error Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.
0 PACKET_ERR_INT Packet Error Interrupt Flag
User andprivilege mode (read):
0 Nointerrupt occurred.
1 Interrupt occurred.
Privilege mode (write):
0 Noinfluence onbit.
1 Bitwillbecleared.

<!-- Page 2133 -->

www.ti.com Control Registers
2133 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.6 DMM Interrupt Offset 1Register (DMMOFF1)
This register holds theoffset indicating which interrupt occurred oninterrupt level 0.The CPU canread
thisregister todetermine thesource oftheinterrupt without having totestindividual interrupt flags.
Figure 36-12. DMM Interrupt Offset 1Register (DMMOFF1) [offset =14h]
31 16
Reserved
R-0
15 5 4 0
Reserved OFFSET
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 36-12. DMM Interrupt Offset 1Register (DMMOFF1) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Read returns 0.Writes have noeffect.
4-0 OFFSET User andprivilege mode (read):
BitEncoding Interrupt
0 Phantom. Allinterrupt flags have been cleared before theoffset register hasbeen read.
1h Packet Error
2h Destination 0Error
3h Destination 1Error
4h Destination 2Error
5h Destination 3Error
6h Source Overflow
7h Buffer Overflow
8h Bus Error
9h Destination 0Region 1
Ah Destination 0Region 2
Bh Destination 1Region 1
Ch Destination 1Region 2
Dh Destination 2Region 1
Eh Destination 2Region 2
Fh Destination 3Region 1
10h Destination 3Region 2
11h End ofBuffer
12h Programmable Buffer
13h-1Fh Reserved
Reading theoffset willclear thecorresponding flaginDMMINTFLG (Section 36.3.5 ).
Privilege anduser mode writes have noeffect

<!-- Page 2134 -->

Control Registers www.ti.com
2134 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.7 DMM Interrupt Offset 2Register (DMMOFF2)
This register holds theoffset indicating which interrupt occurred oninterrupt level 1.The CPU canread
thisregister todetermine thesource oftheinterrupt without having totestindividual interrupt flags.
Figure 36-13. DMM Interrupt Offset 2Register (DMMOFF2) [offset =18h]
31 16
Reserved
R-0
15 5 4 0
Reserved OFFSET
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 36-13. DMM Interrupt Offset 2Register (DMMOFF1) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Read returns 0.Writes have noeffect.
4-0 OFFSET User andprivilege mode (read):
BitEncoding Interrupt
0 Phantom. Allinterrupt flags have been cleared before theoffset register hasbeen read.
1h Packet Error
2h Destination 0Error
3h Destination 1Error
4h Destination 2Error
5h Destination 3Error
6h Source Overflow
7h Buffer Overflow
8h Bus Error
9h Destination 0Region 1
Ah Destination 0Region 2
Bh Destination 1Region 1
Ch Destination 1Region 2
Dh Destination 2Region 1
Eh Destination 2Region 2
Fh Destination 3Region 1
10h Destination 3Region 2
11h End ofBuffer
12h Programmable Buffer
13h-1Fh Reserved
Reading theoffset willclear thecorresponding flaginDMMINTFLG (Section 36.3.5 ).
Privilege anduser mode writes have noeffect

<!-- Page 2135 -->

www.ti.com Control Registers
2135 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.8 DMM Direct Data Mode Destination Register (DMMDDMDEST)
This register defines thestarting address ofthebuffer used tostore thereceived data inDirect Data
Mode. Bywriting tothisregister, theDMMDDMPT register (Section 36.3.10 )willbesetto0x0000.
Figure 36-14. DMM Direct Data Mode Destination Register (DMMDDMDEST) [offset =1Ch]
31 0
STARTADDR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 36-14. DMM Direct Data Mode Destination Register (DMMDDMDEST) Field Descriptions
Bit Field Description
31-0 STARTADDR These bitsdefine thestarting address ofthebuffer. The starting address hastobeamultiple ofthe
blocksize chosen inDMMDDMBL (Section 36.3.9 ).
User andprivilege mode (read): current start address
Privilege mode (write): sets start address tovalue written
36.3.9 DMM Direct Data Mode Blocksize Register (DMMDDMBL)
This register defines theblocksize ofthebuffer used tostore thereceived data inDirect Data Mode.
Figure 36-15. DMM Direct Data Mode Blocksize Register (DMMDDMBL) [offset =20h]
31 16
Reserved
R-0
15 4 3 0
Reserved BLOCKSIZE
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 36-15. DMM Direct Data Mode Blocksize Register (DMMDDMBL) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Read returns 0.Writes have noeffect.
3-0 BLOCKSIZE These bitsdefine thesize ofthebuffer region
User andprivilege mode (read): current block size
Privilege mode (write):
0 Buffer disabled. Nodata willbestored.
1h 32Byte
2h 64Byte
3h 128Byte
4h 256Byte
5h 512Byte
6h 1KByte
7h 2KByte
8h 4KByte
9h 8KByte
Ah 16KByte
Bh 32KByte
Ch-Fh Reserved

<!-- Page 2136 -->

Control Registers www.ti.com
2136 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.10 DMM Direct Data Mode Pointer Register (DMMDDMPT)
This register shows thepointer intothebuffer programmed byDMMDDMDEST (Section 36.3.8 )and
DMMDDMBL (Section 36.3.9 ).
Figure 36-16. DMM Direct Data Mode Pointer Register (DMMDDMPT) [offset =24h]
31 16
Reserved
R-0
15 14 0
Rsvd POINTER
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 36-16. DMM Direct Data Mode Pointer Register (DMMDDMPT) Field Descriptions
Bit Field Value Description
31-15 Reserved 0 Read returns 0.Writes have noeffect.
14-0 POINTER These bitshold thepointer tothenext entry tobewritten inthebuffer. The pointer points tothebyte
aligned address. Ifin16-bit DDM mode, bit0willbe0.Ifin32-bit DDM mode, bit0and1willbe0.
User andprivilege mode (read): next data entry
Privilege mode (write): writes have noeffect
36.3.11 DMM Direct Data Mode Interrupt Pointer Register (DMMINTPT)
This register canbeprogrammed tohold athreshold towhich theDMMDDMPT register (Section 36.3.10 )
iscompared. Aninterrupt canbegenerated when both match.
Figure 36-17. DMM Direct Data Mode Interrupt Pointer Register (DMMINTPT) [offset =28h]
31 16
Reserved
R-0
15 14 0
Rsvd INTPT
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 36-17. DMM Direct Data Mode Interrupt Pointer Register (DMMINTPT) Field Descriptions
Bit Field Value Description
31-15 Reserved 0 Read returns 0.Writes have noeffect.
14-0 INTPT Interrupt Pointer. When thebuffer pointer (Section 36.3.10 )matches theprogrammed value in
DMMINTPT andthePROG_BUF interrupt (Section 36.3.2 )isset,aninterrupt isgenerated.
User andprivilege mode (read): current interrupt threshold
Privilege mode (write): new interrupt threshold

<!-- Page 2137 -->

www.ti.com Control Registers
2137 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.12 DMM Destination xRegion 1(DMMDESTxREG1)
This register defines thestarting address ofthebuffer used tostore thereceived data inTrace Mode. If
thereceived data does notfallintotheaddress range defined byDMMDESTxREG1 andDMMDESTxBL1,
aninterrupt (DESTx_ERR) canbegenerated. The description below isvalid forfollowing registers:
DMMDEST0REG1, DMMDEST1REG1, DMMDEST2REG1, DMMDEST3REG1.
Figure 36-18. DMM Destination xRegion 1(DMMDESTxREG1) [offset =2Ch, 3Ch, 4Ch, 5Ch]
31 18 17 16
BASEADDR BLOCKADDR
R/WP-0 R/WP-0
15 0
BLOCKADDR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 36-18. DMM Destination xRegion 1(DMMDESTxREG1) Field Descriptions
Bit Field Description
31-18 BASEADDR These bitsdefine thebase address ofthe256kB region where thebuffer islocated.
User andprivilege mode (read): current start address
Privilege mode (write): sets base address tovalue written
17-0 BLOCKADDR These bitsdefine thestarting address ofthebuffer inthe256kB page. The starting address hastobea
multiple oftheblocksize chosen inDMMDESTxBL1 (Section 36.3.13 ).
User andprivilege mode (read): current start address
Privilege mode (write): sets start address tovalue written

<!-- Page 2138 -->

Control Registers www.ti.com
2138 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.13 DMM Destination xBlocksize 1(DMMDESTxBL1)
This register defines theblocksize ofthebuffer used tostore thereceived data inTrace Mode. Ifthe
received data does notfallintotheaddress range defined byDMMDESTxREG1 andDMMDESTxBL1, an
interrupt (DESTx_ERR) canbegenerated. The description below isvalid forfollowing registers:
DMMDEST0BL1, DMMDEST1BL1, DMMDEST2BL1, DMMDEST3BL1.
Figure 36-19. DMM Destination xBlocksize 1(DMMDESTxBL1) [offset =30h, 40h, 50h, 60h]
31 16
Reserved
R-0
15 4 3 0
Reserved BLOCKSIZE
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 36-19. DMM Destination xBlocksize 1(DMMDESTxBL1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Read returns 0.Writes have noeffect.
3-0 BLOCKSIZE These bitsdefine thelength ofthebuffer region. Ifallbitsare0,theregion isdisabled and
nodata willbestored.
User andprivilege mode (read): current block size
Privilege mode (write):
0 Region disabled
1h 1KByte
2h 2KByte
3h 4KByte
4h 8KByte
5h 16KByte
6h 32KByte
7h 64KByte
8h 128KByte
9h 256KByte
Ah-Fh Reserved

<!-- Page 2139 -->

www.ti.com Control Registers
2139 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.14 DMM Destination xRegion 2(DMMDESTxREG2)
This register defines thestarting address ofthebuffer used tostore thereceived data inTrace Mode. If
thereceived data does notfallintotheaddress range defined byDMMDESTxREG2 andDMMDESTxBL2,
aninterrupt (DESTx_ERR) canbegenerated. The description below isvalid forfollowing registers:
DMMDEST0REG2, DMMDEST1REG2, DMMDEST2REG2, DMMDEST3REG2.
Figure 36-20. DMM Destination xRegion 2(DMMDESTxREG2) [offset =34h, 44h, 54h, 64h]
31 18 17 16
BASEADDR BLOCKADDR
R/WP-0 R/WP-0
15 0
BLOCKADDR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 36-20. DMM Destination xRegion 2(DMMDESTxREG2) Field Descriptions
Bit Field Description
31-18 BASEADDR These bitsdefine thebase address ofthe256kB region where thebuffer islocated.
User andprivilege mode (read): current start address
Privilege mode (write): sets base address tovalue written
17-0 BLOCKADDR These bitsdefine thestarting address ofthebuffer inthe256kB page. The starting address hastobea
multiple oftheblocksize chosen inDMMDESTxBL1 (Section 36.3.15 ).
User andprivilege mode (read): current start address
Privilege mode (write): sets start address tovalue written

<!-- Page 2140 -->

Control Registers www.ti.com
2140 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.15 DMM Destination xBlocksize 2(DMMDESTxBL2)
This register defines theblocksize ofthebuffer used tostore thereceived data inTrace Mode. Ifthe
received data does notfallintotheaddress range defined byDMMDESTxREG2 andDMMDESTxBL2, an
interrupt (DESTx_ERR) canbegenerated. The description below isvalid forfollowing registers:
DMMDEST0BL2, DMMDEST1BL2, DMMDEST2BL2, DMMDEST3BL2.
Figure 36-21. DMM Destination xBlocksize 2(DMMDESTxBL2) [offset =38h, 48h, 58h, 68h]
31 16
Reserved
R-0
15 4 3 0
Reserved BLOCKSIZE
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 36-21. DMM Destination xBlocksize 2(DMMDESTxBL2) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Read returns 0.Writes have noeffect.
3-0 BLOCKSIZE These bitsdefine thelength ofthebuffer region. Ifallbitsare0,theregion isdisabled and
nodata willbestored.
User andprivilege mode (read): current block size
Privilege mode (write):
0 Region disabled
1h 1KByte
2h 2KByte
3h 4KByte
4h 8KByte
5h 16KByte
6h 32KByte
7h 64KByte
8h 128KByte
9h 256KByte
Ah-Fh Reserved

<!-- Page 2141 -->

www.ti.com Control Registers
2141 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.16 DMM PinControl 0(DMMPC0)
This register defines iftheDMM pins areused infunctional orGIO mode. Itshould only bewritten when
ON/OFF =0101 andtheBUSY bit=0(Section 36.3.1 ).Ifpins other than thepins specified inTable 36-5
areconfigured, orDMMCLK andDMMSYNC areprogrammed asnon-functional pins, nooperation in
trace mode ordirect data mode ispossible.
Figure 36-22. DMM PinControl 0(DMMPC0) [offset =6Ch]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAFUNC DATA15FUNC DATA14FUNC
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13FUNC DATA12FUNC DATA11FUNC DATA10FUNC DATA9FUNC DATA8FUNC DATA7FUNC DATA6FUNC
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5FUNC DATA4FUNC DATA3FUNC DATA2FUNC DATA1FUNC DATA0FUNC CLKFUNC SYNCFUNC
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-22. DMM PinControl 0(DMMPC0) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAFUNC Functional mode ofDMMENA pin. This bitdefines whether thepinisused infunctional mode orin
GIO mode.
User andprivilege mode (read):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
Privilege mode (write):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
17-2 DATAxFUNC Functional mode ofDMMDATA[x] pin. This bitdefines whether thepinisused infunctional mode or
inGIO mode. Ifpins areconfigured infunctional mode, only pins defined inTable 36-5 have tobeused
forproper operation.
User andprivilege mode (read):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
Privilege mode (write):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
1 CLKFUNC Functional mode ofDMMCLK pin. This bitdefines whether thepinisused infunctional mode orin
GIO mode.
User andprivilege mode (read):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
Privilege mode (write):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.

<!-- Page 2142 -->

Control Registers www.ti.com
2142 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-22. DMM PinControl 0(DMMPC0) Field Descriptions (continued)
Bit Field Value Description
0 SYNCFUNC Functional mode ofDMMSYNC pin. This bitdefines whether thepinisused infunctional mode orin
GIO mode.
User andprivilege mode (read):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
Privilege mode (write):
0 Pinisused inGIO mode.
1 Pinisused inFunctional mode.
36.3.17 DMM PinControl 1(DMMPC1)
The bitsinthisregister define thedirection oftheindividual module pins when inGIO mode.
Figure 36-23. DMM PinControl 1(DMMPC1) [offset =70h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENADIR DATA15DIR DATA14DIR
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13DIR DATA12DIR DATA11DIR DATA10DIR DATA9DIR DATA8DIR DATA7DIR DATA6DIR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5DIR DATA4DIR DATA3DIR DATA2DIR DATA1DIR DATA0DIR CLKDIR SYNCDIR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-23. DMM PinControl 1(DMMPC1) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENADIR Direction ofDMMENA pin.
User andprivilege mode (read):
0 Pinisused asinput.
1 Pinisused asoutput.
Privilege mode (write):
0 Pinissettoinput.
1 Pinissettooutput.
17-2 DATAxDIR Direction ofDMMDATA[x] pin. This bitdefines whether thepinisused asinput oroutput inGIO
mode.
User andprivilege mode (read):
0 Pinisused asinput.
1 Pinisused asoutput.
Privilege mode (write):
0 Pinissettoinput.
1 Pinissettooutput.

<!-- Page 2143 -->

www.ti.com Control Registers
2143 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-23. DMM PinControl 1(DMMPC1) Field Descriptions (continued)
Bit Field Value Description
1 CLKDIR Direction ofDMMCLK pin. This bitdefines whether thepinisused asinput oroutput inGIO mode.
User andprivilege mode (read):
0 Pinisused asinput.
1 Pinisused asoutput.
Privilege mode (write):
0 Pinissettoinput.
1 Pinissettooutput.
0 SYNCDIR Direction ofDMMSYNC pin. This bitdefines whether thepinisused asinput oroutput inGIO mode.
User andprivilege mode (read):
0 Pinisused asinput.
1 Pinisused asoutput.
Privilege mode (write):
0 Pinissettoinput.
1 Pinissettooutput.

<!-- Page 2144 -->

Control Registers www.ti.com
2144 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.18 DMM PinControl 2(DMMPC2)
The bitsinthisregister reflect thedigital representation ofthevoltage level atthemodule pins. Even ifa
pinisconfigured tobeanoutput pin,thelevel canberead back viathisregister.
Figure 36-24. DMM PinControl 2(DMMPC2) [offset =74h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAIN DATA15IN DATA14IN
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13IN DATA12IN DATA11IN DATA10IN DATA9IN DATA8IN DATA7IN DATA6IN
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5IN DATA4IN DATA3IN DATA2IN DATA1IN DATA0IN CLKIN SYNCIN
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-24. DMM PinControl 2(DMMPC2) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAIN DMMENA input. This bitreflects thestate ofthepininallmodes.
User andprivilege mode (read):
0 Logic low(input voltage isVILorlower).
1 Logic high (input voltage isVIHorhigher).
Privilege mode (write): writes tothisbithave noeffect.
17-2 DATAxIN DMMDATA[x] input. This bitreflects thestate ofthepininallmodes.
User andprivilege mode (read):
0 Logic low(input voltage isVILorlower).
1 Logic high (input voltage isVIHorhigher).
Privilege mode (write): writes tothisbithave noeffect.
1 CLKIN DMMCLK input. This bitreflects thestate ofthepininallmodes.
User andprivilege mode (read):
0 Logic low(input voltage isVILorlower).
1 Logic high (input voltage isVIHorhigher).
Privilege mode (write): writes tothisbithave noeffect.
0 SYNCIN DMMSYNC input. This bitreflects thestate ofthepininallmodes.
User andprivilege mode (read):
0 Logic low(input voltage isVILorlower).
1 Logic high (input voltage isVIHorhigher).
Privilege mode (write): writes tothisbithave noeffect.

<!-- Page 2145 -->

www.ti.com Control Registers
2145 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.19 DMM PinControl 3(DMMPC3)
The bitsinthisregister setthepintologic loworhigh level ifthepinisconfigured asoutput
(Section 36.3.17 ).
Figure 36-25. DMM PinControl 3(DMMPC3) [offset =78h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAOUT DATA15OUT DATA14OUT
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13OUT DATA12OUT DATA11OUT DATA10OUT DATA9OUT DATA8OUT DATA7OUT DATA6OUT
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5OUT DATA4OUT DATA3OUT DATA2OUT DATA1OUT DATA0OUT CLKOUT SYNCOUT
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-25. DMM PinControl 3(DMMPC3) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAOUT Output state ofDMMENA pin. This bitsets thepintologic loworhigh level.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 Logic low(output voltage issettoVOLorlower).
1 Logic high (output voltage issettoVOHorhigher).
17-2 DATAxOUT Output state ofDMMDATA[x] pin. This bitsets thepintologic loworhigh level.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 Logic low(output voltage issettoVOLorlower).
1 Logic high (output voltage issettoVOHorhigher).
1 CLKOUT Output state ofDMMCLK pin. This bitsets thepintologic loworhigh level.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 Logic low(output voltage issettoVOLorlower).
1 Logic high (output voltage issettoVOHorhigher).

<!-- Page 2146 -->

Control Registers www.ti.com
2146 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-25. DMM PinControl 3(DMMPC3) Field Descriptions (continued)
Bit Field Value Description
0 SYNCOUT Output state ofDMMSYNC pin. This bitsets thepintologic loworhigh level.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 Logic low(output voltage issettoVOLorlower).
1 Logic high (output voltage issettoVOHorhigher).
36.3.20 DMM PinControl 4(DMMPC4)
This register allows tosetindividual pins toalogic high level without having todoaread-modify-write
operation aswould bethecase with theDMMPC3 register (Section 36.3.19 ).Writing azero toabitwill
notchange thestate ofthepin.
Figure 36-26. DMM PinControl 4(DMMPC4) [offset =7Ch]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENASET DATA15SET DATA14SET
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13SET DATA12SET DATA11SET DATA10SET DATA9SET DATA8SET DATA7SET DATA6SET
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5SET DATA4SET DATA3SET DATA2SET DATA1SET DATA0SET CLKSET SYNCSET
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-26. DMM PinControl 4(DMMPC4) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENASET Sets output state ofDMMENA pintologic high. Value intheENASET bitsets thedata output control
register bitto1regardless ofthecurrent value intheENAOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Logic high (output voltage issettoVOHorhigher).
17-2 DATAxSET Sets output state ofDMMDATA[x] pintologic high. Value intheDATAxSET bitsets thedata output
control register bitto1regardless ofthecurrent value intheDATAxOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Logic high (output voltage issettoVOHorhigher).

<!-- Page 2147 -->

www.ti.com Control Registers
2147 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-26. DMM PinControl 4(DMMPC4) Field Descriptions (continued)
Bit Field Value Description
1 CLKSET Sets output state ofDMMCLK pintologic high. Value intheCLKSET bitsets thedata output control
register bitto1regardless ofthecurrent value intheCLKOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Logic high (output voltage issettoVOHorhigher).
0 SYNCSET Sets output state ofDMMSYNC pinlogic high. Value intheSYNCSET bitsets thedata output
control register bitto1regardless ofthecurrent value intheSYNCOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Logic high (output voltage issettoVOHorhigher).

<!-- Page 2148 -->

Control Registers www.ti.com
2148 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.21 DMM PinControl 5(DMMPC5)
This register allows tosetindividual pins toalogic lowlevel without having todoaread-modify-write
operation aswould bethecase with theDMMPC3 register (Section 36.3.19 ).Writing aonetoabitwill
change theoutput toalogic lowlevel, writing azero willnotchange thestate ofthepin.
Figure 36-27. DMM PinControl 5(DMMPC5) [offset =80h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENACLR DATA15CLR DATA14CLR
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13CLR DATA12CLR DATA11CLR DATA10CLR DATA9CLR DATA8CLR DATA7CLR DATA6CLR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5CLR DATA4CLR DATA3CLR DATA2CLR DATA1CLR DATA0CLR CLKCLR SYNCCLR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-27. DMM PinControl 5(DMMPC5) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENACLR Sets output state ofDMMENA pintologic low. Value intheENACLR bitclears thedata output
control register bitto0,regardless ofthecurrent value intheENAOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Clears thepintologic low(output voltage issettoVOLorlower).
17-2 DATAxCLR Sets output state ofDMMDATA[x] pintologic low. Value intheDATAxCLR bitclears thedata
output control register DATAxOUT bitto0,regardless ofthecurrent value intheDATAxOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Clears thepintologic low(output voltage issettoVOLorlower).
1 CLKCLR Sets output state ofDMMCLK pintologic low. Value intheCLKCLR bitclears thedata output
control register CLKOUT bitto0,regardless ofthecurrent value intheCLKOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Clears thepintologic low(output voltage issettoVOLorlower).

<!-- Page 2149 -->

www.ti.com Control Registers
2149 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-27. DMM PinControl 5(DMMPC5) Field Descriptions (continued)
Bit Field Value Description
0 SYNCCLR Sets output state ofDMMSYNC pintologic low. Value intheSYNCCLR bitclears thedata output
control register SYNCOUT bitto0,regardless ofthecurrent value intheSYNCOUT bit.
User andprivilege mode (read):
0 Logic low(output voltage isVOLorlower).
1 Logic high (output voltage isVOHorhigher).
Privilege mode (write):
0 State ofthepinisunchanged.
1 Clears thepintologic low(output voltage issettoVOLorlower).
36.3.22 DMM PinControl 6(DMMPC6)
These bitsconfigure thepins inpush-pull oropen-drain functionality. Ifconfigured tobeopen-drain, the
module only drives alogic-low level onthepin.Anexternal pull-up resistor needs tobeconnected tothe
pintopullithigh, when thepinisinhigh-impedance mode.
Figure 36-28. DMM PinControl 6(DMMPC6) [offset =84h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAPDR DATA15PDR DATA14PDR
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 11 10 9 8
DATA13PDR DATA12PDR DATA11PDR DATA10PDR DATA9PDR DATA8PDR DATA7PDR DATA6PDR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
DATA5PDR DATA4PDR DATA3PDR DATA2PDR DATA1PDR DATA0PDR CLKPDR SYNCPDR
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-28. DMM PinControl 6(DMMPC6) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAPDR Open Drain enable. Enables open-drain functionality, ifthepinisconfigured asGIO output
(DMMPC0[18] =0;DMMPC1[18] =1).Ifthepinisconfigured asafunctional pin(DMMPC0[18] =1),the
open-drain functionality isdisabled.
User andprivilege mode (read):
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen-drain mode.
Privilege mode (write):
0 Configures pinaspush/pull.
1 Configures pinasopen drain.

<!-- Page 2150 -->

Control Registers www.ti.com
2150 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-28. DMM PinControl 6(DMMPC6) Field Descriptions (continued)
Bit Field Value Description
17-2 DATAxPDR Open Drain enable. Enables open-drain functionality onpin,ifpinisconfigured asGIO output
(DMMPC0[x] =0;DMMPC1[x] =1).Ifthepinisconfigured asafunctional pin(DMMPC0[x] =1),the
open-drain functionality isdisabled.
User andprivilege mode (read):
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen-drain mode.
Privilege mode (write):
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.
1 CLKPDR Open Drain enable. Enables open-drain functionality onpin,ifpinisconfigured asGIO output
(DMMPC0[1] =0;DMMPC1[1] =1).Ifthepinisconfigured asafunctional pin(DMMPC0[1] =1),the
open-drain functionality isdisabled.
User andprivilege mode (read):
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen-drain mode.
Privilege mode (write):
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.
0 SYNCPDR Open Drain enable. Enables open-drain functionality onpin,ifpinisconfigured asGIO output
(DMMPC0[0] =0;DMMPC1[0] =1).Ifthepinisconfigured asafunctional pin(DMMPC0[0] =1),the
open-drain functionality isdisabled.
User andprivilege mode (read):
0 Pinbehaves asnormal push/pull pin.
1 Pinoperates inopen-drain mode.
Privilege mode (write):
0 Configures thepinaspush/pull.
1 Configures thepinasopen drain.

<!-- Page 2151 -->

www.ti.com Control Registers
2151 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)36.3.23 DMM PinControl 7(DMMPC7)
The bitsinregister control thepullup/down functionality ofapin.The internal pullup/down canbeenabled
ordisabled bythisregister. The reset configuration ofthese bitsisdevice implementation dependent.
Please consult thedevice datasheet forthisinformation.
Figure 36-29. DMM PinControl 7(DMMPC7) [offset =88h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAPDIS DATA15PDIS DATA14PDIS
R-0 R/WP-x R/WP-x R/WP-x
15 14 13 12 11 10 9 8
DATA13PDIS DATA12PDIS DATA11PDIS DATA10PDIS DATA9PDIS DATA8PDIS DATA7PDIS DATA6PDIS
R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x
7 6 5 4 3 2 1 0
DATA5PDIS DATA4PDIS DATA3PDIS DATA2PDIS DATA1PDIS DATA0PDIS CLKPDIS SYNCPDIS
R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x R/WP-x
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-29. DMM PinControl 7(DMMPC7) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAPDIS Pulldisable. Removes internal pullup/pulldown functionality from pinwhen configured asinput pin
(DMMPC1[18] =0).
User andprivilege mode (read):
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Privilege mode (write):
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
17-2 DATAxPDIS Pulldisable. Removes internal pullup/pulldown functionality from pinwhen configured asinput pin
(DMMPC1[x] =0).
User andprivilege mode (read):
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Privilege mode (write):
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
1 CLKPDIS Pulldisable. Removes internal pullup/pulldown functionality from pinwhen configured asinput pin
(DMMPC1[1] =0).
User andprivilege mode (read):
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Privilege mode (write):
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.

<!-- Page 2152 -->

Control Registers www.ti.com
2152 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-29. DMM PinControl 7(DMMPC7) Field Descriptions (continued)
Bit Field Value Description
0 SYNCPDIS Pulldisable. Removes internal pullup/pulldown functionality from pinwhen configured asinput pin
(DMMPC1[0] =0).
User andprivilege mode (read):
0 Pullup/pulldown functionality isenabled.
1 Pullup/pulldown functionality isdisabled.
Privilege mode (write):
0 Enables pullup/pulldown functionality.
1 Disables pullup/pulldown functionality.
36.3.24 DMM PinControl 8(DMMPC8)
These bitscontrol iftheinternal pullup orpulldown isconfigured ontheinput pin.
Figure 36-30. DMM PinControl 8(DMMPC8) [offset =8Ch]
31 24
Reserved
R-0
23 19 18 17 16
Reserved ENAPSEL DATA15PSEL DATA14PSEL
R-0 R/WP-1 R/WP-1 R/WP-1
15 14 13 12 11 10 9 8
DATA13PSEL DATA12PSEL DATA11PSEL DATA10PSEL DATA9PSEL DATA8PSEL DATA7PSEL DATA6PSEL
R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1
7 6 5 4 3 2 1 0
DATA5PSEL DATA4PSEL DATA3PSEL DATA2PSEL DATA1PSEL DATA0PSEL CLKPSEL SYNCPSEL
R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 36-30. DMM PinControl 8(DMMPC8) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads returns 0.Writes have noeffect.
18 ENAPSEL Pullselect. Configures pullup orpulldown functionality ifDMMPC7[18] =0.
User andprivilege mode (read):
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Privilege mode (write):
0 Enables pulldown functionality.
1 Enables pullup functionality.
17-2 DATAxPSEL Pullselect. Configures pullup orpulldown functionality ifDMMPC7[x] =0.
User andprivilege mode (read):
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Privilege mode (write):
0 Enables pulldown functionality.
1 Enables pullup functionality.

<!-- Page 2153 -->

www.ti.com Control Registers
2153 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedData Modification Module (DMM)Table 36-30. DMM PinControl 8(DMMPC8) Field Descriptions (continued)
Bit Field Value Description
1 CLKPSEL Pullselect. Configures pullup orpulldown functionality ifDMMPC7[1] =0.
User andprivilege mode (read):
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Privilege mode (write):
0 Enables pulldown functionality.
1 Enables pullup functionality.
0 SYNCPSEL Pullselect. Configures pullup orpulldown functionality ifDMMPC7[0] =0.
User andprivilege mode (read):
0 Pulldown functionality isenabled.
1 Pullup functionality isenabled.
Privilege mode (write):
0 Enables pulldown functionality.
1 Enables pullup functionality.