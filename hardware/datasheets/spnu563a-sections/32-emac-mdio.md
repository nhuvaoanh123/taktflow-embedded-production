# EMAC/MDIO Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1803-1926

---


<!-- Page 1803 -->

1803 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleChapter 32
SPNU563A -March 2018
EMAC/MDIO Module
This chapter describes theEthernet Media Access Controller (EMAC) and physical layer (PHY) device
Management Data Input/Output (MDIO) module.
Topic ........................................................................................................................... Page
32.1 Introduction ................................................................................................... 1804
32.2 Architecture ................................................................................................... 1806
32.3 EMAC Control Module Registers ....................................................................... 1854
32.4 MDIO Registers ............................................................................................... 1867
32.5 EMAC Module Registers .................................................................................. 1880

<!-- Page 1804 -->

Introduction www.ti.com
1804 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.1 Introduction
This document provides afunctional description oftheEthernet Media Access Controller (EMAC) and
physical layer (PHY) device Management Data Input/Output (MDIO) module integrated inthe
microcontroller. Included arethefeatures oftheEMAC andMDIO modules, adiscussion oftheir
architecture andoperation, how these modules connect totheoutside world, andadescription ofthe
registers foreach module.
The EMAC controls theflow ofpacket data from thesystem tothePHY. The MDIO module controls PHY
configuration andstatus monitoring.
Both theEMAC andtheMDIO modules interface tothesystem core through acustom interface that
allows efficient data transmission andreception. This custom interface isreferred toastheEMAC control
module andisconsidered integral totheEMAC/MDIO peripheral.
32.1.1 Purpose ofthePeripheral
The EMAC module isused tomove data between thedevice andanother host connected tothesame
network, incompliance with theEthernet protocol.
32.1.2 Features
The EMAC/MDIO hasthefollowing features:
*Synchronous 10/100 Mbps operation.
*Standard Media Independent Interface (MII) and/or Reduced Media Independent Interface (RMII) to
physical layer device (PHY).
*EMAC acts asDMA master toeither internal orexternal device memory space.
*Eight receive channels with VLAN tagdiscrimination forreceive quality-of-service (QOS) support.
*Eight transmit channels with round-robin orfixed priority fortransmit quality-of-service (QOS) support.
*Ether-Stats and802.3-Stats statistics gathering.
*Transmit CRC generation selectable onaperchannel basis.
*Broadcast frames selection forreception onasingle channel.
*Multicast frames selection forreception onasingle channel.
*Promiscuous receive mode frames selection forreception onasingle channel (allframes, allgood
frames, short frames, error frames).
*Hardware flow control.
*8k-byte local EMAC descriptor memory thatallows theperipheral tooperate ondescriptors without
affecting theCPU. The descriptor memory holds enough information totransfer upto512Ethernet
packets without CPU intervention. (This memory isalso known asCPPI RAM.)
*Programmable interrupt logic permits thesoftware driver torestrict thegeneration ofback-to-back
interrupts, which allows more work tobeperformed inasingle calltotheinterrupt service routine.

<!-- Page 1805 -->

EMAC
Interrupts
DMA
Master8k□CPPI
RAMEMAC
ModuleHost□CPU
Interface
DMA
BusMII/RMII
Bus
Interrupt
CombinerC0
Interrupts
MDIO
InterruptsMDIO
ModuleControl□ModuleEMAC□Sub System
MDIO
Bus
www.ti.com Introduction
1805 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.1.3 Functional Block Diagram
Figure 32-1 shows thethree main functional modules oftheEMAC/MDIO peripheral:
*EMAC control module
*EMAC module
*MDIO module
The EMAC control module isthemain interface between thedevice core processor totheEMAC and
MDIO modules. The EMAC control module controls device interrupts andincorporates an8k-byte internal
RAM tohold EMAC buffer descriptors (also known asCPPI RAM).
The MDIO module implements the802.3 serial management interface tointerrogate andcontrol upto32
Ethernet PHYs connected tothedevice byusing ashared two-wire bus. Host software uses theMDIO
module toconfigure theautonegotiation parameters ofeach PHY attached totheEMAC, retrieve the
negotiation results, andconfigure required parameters intheEMAC module forcorrect operation. The
module isdesigned toallow almost transparent operation oftheMDIO interface, with very little
maintenance from thecore processor.
The EMAC module provides anefficient interface between theprocessor andthenetwork. The EMAC on
thisdevice supports 10Base-T (10Mbits/sec) and100BaseTX (100 Mbits/sec), half-duplex andfull-duplex
mode, andhardware flow control andquality-of-service (QOS) support.
Figure 32-1 shows themain interface between theEMAC control module andtheCPU. The following
connections aremade tothedevice core:
*The DMA busconnection from theEMAC control module allows theEMAC module toread andwrite
both internal andexternal memory through theDMA memory transfer controller.
*The EMAC control, EMAC, andMDIO modules allhave control registers. These registers arememory-
mapped intodevice memory space. Along with these registers, thecontrol module 'sinternal CPPI
RAM ismapped intothissame range.
*The EMAC andMDIO interrupts arecombined intofour interrupt signals within thecontrol module. The
Vectored Interrupt Manager (VIM) receives allfour interrupt signals from thecombiner andsubmits
these interrupt requests totheCPU.
Figure 32-1. EMAC andMDIO Block Diagram

<!-- Page 1806 -->

Introduction www.ti.com
1806 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.1.4 Industry Standard(s) Compliance Statement
The EMAC peripheral conforms totheIEEE 802.3 standard, describing theCarrier Sense Multiple Access
with Collision Detection (CSMA/CD) Access Method andPhysical Layer specifications. The IEEE 802.3
standard hasalso been adopted byISO/IEC andre-designated asISO/IEC 8802-3:2000(E).
However, theEMAC deviates from thestandard intheway ithandles transmit underflow errors. The
EMAC MIIinterface does notusetheTransmit Coding Error signal MTXER. Instead ofdriving theerror pin
when anunderflow condition occurs onatransmitted frame, theEMAC intentionally generates anincorrect
checksum byinverting theframe CRC, sothatthetransmitted frame isdetected asanerror bythe
network.
32.2 Architecture
This section discusses thearchitecture andbasic function oftheEMAC peripheral.
32.2.1 Clock Control
Allinternal EMAC logic isclocked synchronously ontheVCLKA4_DIVR domain. Please refer tothe
Architecture chapter formore details.
The MDIO clock isbased onadivide-down oftheVCLK3 peripheral busclock andisspecified torunupto
2.5MHz (although typical operation would be1.0MHz). Because theVCLK3 peripheral clock frequency is
configurable, theapplication software ordriver must control thedivide-down value.
The transmit andreceive clock sources areprovided bytheexternal PHY totheMII_TXCLK and
MII_RXCLK pins ortotheRMII reference clock pin.Data istransmitted andreceived with respect tothe
reference clocks oftheinterface pins.
The MIIinterface frequencies forthetransmit andreceive clocks arefixed bytheIEEE 802.3 specification
as:
*2.5MHz at10Mbps
*25MHz at100Mbps
The RMII interface frequency forthetransmit andreceive clocks arefixed at50MHz forboth 10Mbps
and100Mbps.
32.2.2 Memory Map
The EMAC peripheral includes internal memory thatisused tohold buffer descriptions oftheEthernet
packets tobereceived andtransmitted. This internal RAM is2K×32bitsinsize. Data canbewritten to
andread from theEMAC internal memory byeither theEMAC ortheCPU. Itisused tostore buffer
descriptors thatare4-words (16-bytes) deep. This 8Klocal memory holds enough information totransfer
upto512Ethernet packets without CPU intervention. This EMAC RAM isalso referred toastheCPPI
buffer descriptor memory because itcomplies with theCommunications Port Programming Interface
(CPPI) v3.0 standard.
The packet buffer descriptors canalso beplaced inother on-andoff-chip memories such astheCPU
RAM. There aresome tradeoffs interms ofinterconnect bandwidth when descriptors areplaced inthe
CPU RAM, versus when they areplaced intheEMAC 'sinternal memory. Ingeneral, theEMAC
throughput isbetter when thedescriptors areplaced inthelocal EMAC CPPI RAM.

<!-- Page 1807 -->

MII_TXCLK
MII_TXD[3−0]
MII_TXEN
MII_COL
MII_CRS
MII_RXCLK
MII_RXD[3−0]
MII_RXDV
MII_RXER
MDIO_CLK
MDIO_DPhysical
layer
device
(PHY)System
core Transformer2.5 MHz
or
25 MHz
RJ−45EMAC MDIO
www.ti.com Architecture
1807 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.3 Signal Descriptions
The microcontrollers support both theMIIandtheRMII interfaces. Only oneofthese twointerfaces canbe
used atatime. Aseparate control register intheI/OMultiplexing Module (IOMM) allows theapplication to
indicate theactual interface being used. This isthebit24ofthePINMMR160 control register. This bitis
setbydefault andselects theRMII interface. The application canselect theMIIinterface byclearing this
bit.Please refer totheI/OMultiplexing andControl Module (IOMM) chapter formore details onthe
procedure toconfigure thePINMMR registers.
Each oftheEMAC andMDIO signals fortheMIIandRMII interfaces aremultiplexed with other I/O
functions onthismicrocontroller. Please refer toSection 32.2.4 forinformation onconfiguration ofthe
multiplexing control registers toenable theMII/RMII connections tothese I/Os.
32.2.3.1 Media Independent Interface (MII) Connections
Figure 32-2 shows adevice with integrated EMAC andMDIO interfaced viaaMIIconnection inatypical
system. The EMAC module does notinclude atransmit error (MTXER) pin.Inthecase oftransmit error,
CRC inversion isused tonegate thevalidity ofthetransmitted frame.
The individual EMAC andMDIO signals fortheMIIinterface aresummarized inTable 32-1 .Formore
information, refer toeither theIEEE 802.3 standard orISO/IEC 8802-3:2000(E).
Figure 32-2. Ethernet Configuration --MIIConnections

<!-- Page 1808 -->

Architecture www.ti.com
1808 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-1. EMAC andMDIO Signals forMIIInterface
Signal Type Description
MII_TXCLK I Transmit clock (MII_TXCLK). The transmit clock isacontinuous clock thatprovides thetiming reference for
transmit operations. The MII_TXD andMII_TXEN signals aretiedtothisclock. The clock isgenerated by
thePHY andis2.5MHz at10Mbps operation and25MHz at100Mbps operation.
MII_TXD[3-0] O Transmit data (MII_TXD). The transmit data pins areacollection of4data signals comprising 4bitsof
data. MTDX0 istheleast-significant bit(LSB). The signals aresynchronized byMII_TXCLK andvalid only
when MII_TXEN isasserted.
MII_TXEN O Transmit enable (MII_TXEN). The transmit enable signal indicates thattheMII_TXD pins aregenerating
nibble data forusebythePHY. Itisdriven synchronously toMII_TXCLK.
MII_COL I Collision detected (MII_COL). Inhalf-duplex operation, theMII_COL pinisasserted bythePHY when it
detects acollision onthenetwork. Itremains asserted while thecollision condition persists. This signal is
notnecessarily synchronous toMII_TXCLK norMII_RXCLK.
Infull-duplex operation, theMII_COL pinisused forhardware transmit flow control. Asserting theMII_COL
pinwillstop packet transmissions; packets intheprocess ofbeing transmitted when MII_COL isasserted
willcomplete transmission. The MII_COL pinshould beheld lowifhardware transmit flow control isnot
used.
MII_CRS I Carrier sense (MII_CRS). Inhalf-duplex operation, theMII_CRS pinisasserted bythePHY when the
network isnotidleineither transmit orreceive. The pinisdeasserted when both transmit andreceive are
idle. This signal isnotnecessarily synchronous toMII_TXCLK norMII_RXCLK.
Infull-duplex operation, theMII_CRS pinshould beheld low.
MII_RXCLK I Receive clock (MII_RXCLK). The receive clock isacontinuous clock thatprovides thetiming reference for
receive operations. The MII_RXD, MII_RXDV, andMII_RXER signals aretiedtothisclock. The clock is
generated bythePHY andis2.5MHz at10Mbps operation and25MHz at100Mbps operation.
MII_RXD[3-0] I Receive data (MII_RXD). The receive data pins areacollection of4data signals comprising 4bitsofdata.
MRDX0 istheleast-significant bit(LSB). The signals aresynchronized byMII_RXCLK andvalid only when
MII_RXDV isasserted.
MII_RXDV I Receive data valid (MII_RXDV). The receive data valid signal indicates thattheMII_RXD pins are
generating nibble data forusebytheEMAC. Itisdriven synchronously toMII_RXCLK.
MII_RXER I Receive error (MII_RXER). The receive error signal isasserted foroneormore MII_RXCLK periods to
indicate thatanerror was detected inthereceived frame. This ismeaningful only during data reception
when MII_RXDV isactive.
MDIO_CLK O Management data clock (MDIO_CLK). The MDIO data clock issourced bytheMDIO module onthe
system. Itisused tosynchronize MDIO data access operations done ontheMDIO pin.The frequency of
thisclock iscontrolled bytheCLKDIV bitsintheMDIO control register (CONTROL).
MDIO_D I/O Management data input output (MDIO_D). The MDIO data pindrives PHY management data intoandout
ofthePHY byway ofanaccess frame consisting ofstart offrame, read/write indication, PHY address,
register address, anddata bitcycles. The MDIO_D pinacts asanoutput forallbutthedata bitcycles at
which time itisaninput forread operations.
NOTE: The MIIinterface ofthisdevice isbonded outtotwodifferent sets ofpackage pins. Inone
setofpackage pins, theinterface ismultiplexed with other functions onthisdevice. The
application must configure thecontrol registers intheI/Omultiplexing module inorder to
enable theMIIfunctionality onthecorresponding I/Os. The IOpins towhich theMIIinterface
isbrought arepincompatible with other devices inthefamily. Please refer toSection 32.2.4
forinformation. This device also hasasecond setofpackage pins thatbrings outtheMII
interface. The second setofpackage pins fortheMIIinterface allows theMIIinterface to
operate inparallel with other non-MII functions inthefirstsetofpackage pins. Please see
thedevice datasheet terminal function table fordetail.

<!-- Page 1809 -->

RMII_TXD[1-0]
RMII_TXEN
RMII_MHZ_50_CLK
RMII_RXD[1-0]
RMII_CRS_DV
RMII_RXER
MDIO_CLK
MDIO_D
MDIOEMACPhysical
Layer
Device
(PHY)Transformer50□MHz
RJ-45
www.ti.com Architecture
1809 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.3.2 Reduced Media Independent Interface (RMII) Connections
Figure 32-3 shows adevice with integrated EMAC andMDIO interfaced viaaRMII connection inatypical
system.
The individual EMAC andMDIO signals fortheRMII interface aresummarized inTable 32-2 .Formore
information, refer toeither theIEEE 802.3 standard orISO/IEC 8802-3:2000(E).
Figure 32-3. Ethernet Configuration --RMII Connections
Table 32-2. EMAC andMDIO Signals forRMII Interface
Signal Type Description
RMII_TXD[1-0] O Transmit data (RMII_TXD). The transmit data pins areacollection of2bitsofdata. RMTDX0 is
theleast-significant bit(LSB). The signals aresynchronized byRMII_MHZ_50_CLK andvalid only
when RMII_TXEN isasserted.
RMII_TXEN O Transmit enable (RMII_TXEN). The transmit enable signal indicates thattheRMII_TXD pins are
generating data forusebythePHY. RMII_TXEN issynchronous toRMII_MHZ_50_CLK.
RMII_MHZ_50_CLK I RMII reference clock (RMII_MHZ_50_CLK). The reference clock isused tosynchronize allRMII
signals. RMII_MHZ_50_CLK must becontinuous andfixed at50MHz.
RMII_RXD[1-0] I Receive data (RMII_RXD). The receive data pins areacollection of2bitsofdata. RMRDX0 isthe
least-significant bit(LSB). The signals aresynchronized byRMII_MHZ_50_CLK andvalid only
when RMII_CRS_DV isasserted andRMII_RXER isdeasserted.
RMII_CRS_DV I Carrier sense/receive data valid (RMII_CRS_DV). Multiplexed signal between carrier sense and
receive data valid.
RMII_RXER I Receive error (RMII_RXER). The receive error signal isasserted toindicate thatanerror was
detected inthereceived frame.
MDIO_CLK O Management data clock (MDIO_CLK). The MDIO data clock issourced bytheMDIO module on
thesystem. Itisused tosynchronize MDIO data access operations done ontheMDIO pin.The
frequency ofthisclock iscontrolled bytheCLKDIV bitsintheMDIO control register (CONTROL).
MDIO_D I/O Management data input output (MDIO_D). The MDIO data pindrives PHY management data into
andoutofthePHY byway ofanaccess frame consisting ofstart offrame, read/write indication,
PHY address, register address, anddata bitcycles. The MDIO_D pinacts asanoutput forallbut
thedata bitcycles atwhich time itisaninput forread operations.

<!-- Page 1810 -->

Architecture www.ti.com
1810 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.4 MII/RMII Signal Multiplexing Control
InEach oftheMIIandRMII interface signals aremultiplexed with other functions onthismicrocontroller.
The application must configure thecontrol registers intheI/Omultiplexing module inorder toenable the
MII/RMII functionality onthecorresponding I/Os. Table 32-3 shows thebyte tobeconfigured toenable the
MDIO functions. Table 32-4 shows thebyte tobeconfigured toenable theMIIorRMII functions. Please
refer totheI/OMultiplexing andControl Module (IOMM) chapter formore details ontheprocedure to
configure thePINMMR registers.
Table 32-3. MDIO Multiplexing Control
MDIO Signal Name Control forSelecting EMAC /MDIO Signal
MDIO_CLK PINMMR21[31:24] =0b00000100
MDIO_D PINMMR23 [7:0] =0b00000100
Table 32-4. MII/RMII Multiplexing Control
MII/RMII Signal Name Control forSelecting MIISignal Control forSelecting RMII Signal
MII_TXCLK PINMMR30 [23:16] =0b00000010 -
MII_TXD[3] PINMMR30 [7:0] =0b00000100 -
MII_TXD[2] PINMMR21 [15:8] =0b00000100 -
MII_TXD[1] /RMII_TXD[1] PINMMR26 [7:0] =0b00000100 PINMMR26 [7:0] =0b00001000
MII_TXD[0] /RMII_TXD[0] PINMMR27 [7:0] =0b00000100 PINMMR27 [7:0] =0b00001000
MII_TXEN /RMII_TXEN PINMMR24 [23:16] =0b00000100 PINMMR24 [23:16] =0b00001000
MII_COL PINMMR21 [23:16] =0b00000100 -
MII_CRS /RMII_CRSDV PINMMR31 [7:0] =0b00000100 PINMMR31 [7:0] =0b00001000
MII_RXCLK /RMII_50MHZ_CLK PINMMR34 [15:8] =0b00000100 PINMMR34 [15:8] =0b00001000
MII_RXD[3] PINMMR25 [31:24] =0b00000100 -
MII_RXD[2] PINMMR22 [15:8] =0b00000100 -
MII_RXD[1] /RMII_RXD[1] PINMMR34 [7:0] =0b00000010 PINMMR34 [7:0] =0b00000100
MII_RXD[0] /RMII_RXD[0] PINMMR33 [31:24] =0b00000100 PINMMR33 [31:24] =0b00001000
MII_RXDV PINMMR34 23:16] =0b00000100 -
MII_RXER /RMII_RXER PINMMR0 [7:0] =0b00000100 PINMMR0 [7:0] =0b00001000

<!-- Page 1811 -->

Preamble SFD Destination Source Len Data7 1 6 6 2 46−1500 4
FCSNumber of bytes
Legend: SFD=Start Frame Delimeter; FCS=Frame Check Sequence (CRC)
www.ti.com Architecture
1811 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.5 Ethernet Protocol Overview
Abrief overview oftheEthernet protocol isgiven inthefollowing subsections. See theIEEE 802.3
standard document forin-depth information ontheCarrier Sense Multiple Access with Collision Detection
(CSMA/CD) Access Method.
32.2.5.1 Ethernet Frame Format
AlltheEthernet technologies usethesame frame structure. The format ofanEthernet frame isshown in
Figure 32-4 anddescribed inTable 32-5.The Ethernet packet, which isthecollection ofbytes
representing thedata portion ofasingle Ethernet frame onthewire, isshown outlined inbold. The
Ethernet frames areofvariable lengths, with noframe smaller than 64bytes orlarger than RXMAXLEN
bytes (header, data, andCRC).
Figure 32-4. Ethernet Frame Format
Table 32-5. Ethernet Frame Description
Field Bytes Description
Preamble 7 Preamble. These 7bytes have afixed value of55handserve towake upthereceiving
EMAC ports andtosynchronize their clocks tothatofthesender 'sclock.
SFD 1 Start ofFrame Delimiter. This field with avalue of5Dh immediately follows thepreamble
pattern andindicates thestart ofimportant data.
Destination 6 Destination address. This field contains theEthernet MAC address oftheEMAC port for
which theframe isintended. Itmay beanindividual ormulticast (including broadcast)
address. When thedestination EMAC port receives anEthernet frame with adestination
address thatdoes notmatch anyofitsMAC physical addresses, andnopromiscuous,
multicast orbroadcast channel isenabled, itdiscards theframe.
Source 6 Source address. This field contains theMAC address oftheEthernet port thattransmits the
frame totheLocal Area Network.
Len 2 Length/Type field. The length field indicates thenumber ofEMAC client data bytes
contained inthesubsequent data field oftheframe. This field canalso beused toidentify
thetype ofdata theframe iscarrying.
Data 46to
(RXMAXLEN -18)Data field. This field carries thedatagram containing theupper layer protocol frame, thatis,
IPlayer datagram. The maximum transfer unit(MTU) ofEthernet is(RXMAXLEN -18)
bytes. This means thatiftheupper layer protocol datagram exceeds (RXMAXLEN -18)
bytes, then thehost hastofragment thedatagram andsend itinmultiple Ethernet packets.
The minimum size ofthedata field is46bytes. This means thatiftheupper layer datagram
isless then 46bytes, thedata field hastobeextended to46bytes byappending extra bits
after thedata field, butprior tocalculating andappending theFCS.
FCS 4 Frame Check Sequence. Acyclic redundancy check (CRC) isused bythetransmit and
receive algorithms togenerate aCRC value fortheFCS field. The frame check sequence
covers the60to1514 bytes ofthepacket data. Note thatthis4-byte field may ormay not
beincluded aspart ofthepacket data, depending onhow theEMAC isconfigured.

<!-- Page 1812 -->

Architecture www.ti.com
1812 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.5.2 Ethernet 'sMultiple Access Protocol
Nodes inanEthernet Local Area Network areinterconnected byabroadcast channel --when anEMAC
port transmits aframe, alltheadapters onthelocal network receive theframe. Carrier Sense Multiple
Access with Collision Detection (CSMA/CD) algorithms areused when theEMAC operates inhalf-duplex
mode. When operating infull-duplex mode, there isnocontention foruseofashared medium because
there areexactly twoports onthelocal network.
Each port runs theCSMA/CD protocol without explicit coordination with theother ports ontheEthernet
network. Within aspecific port, theCSMA/CD protocol works asfollows:
1.The port obtains data from upper layer protocols atitsnode, prepares anEthernet frame, andputs the
frame inabuffer.
2.Iftheport senses thatthemedium isidle, itstarts totransmit theframe. Iftheport senses thatthe
transmission medium isbusy, itwaits until itnolonger senses energy (plus anInter-Packet Gap time)
andthen starts totransmit theframe.
3.While transmitting, theport monitors forthepresence ofsignal energy coming from other ports. Ifthe
port transmits theentire frame without detecting signal energy from other Ethernet devices, theport is
done with theframe.
4.Iftheport detects signal energy from other ports while transmitting, itstops transmitting itsframe and
instead transmits a48-bit jamsignal.
5.After transmitting thejamsignal, theport enters anexponential backoff phase. Ifadata frame
encounters back-to-back collisions, theport chooses arandom value thatisdependent onthenumber
ofcollisions. The port then waits anamount oftime thatisamultiple ofthisrandom value andreturns
tostep 2.
32.2.6 Programming Interface
32.2.6.1 Packet Buffer Descriptors
The buffer descriptor isacentral part oftheEMAC module andishow theapplication software describes
Ethernet packets tobesent andempty buffers tobefilled with incoming packet data. The basic descriptor
format isshown inFigure 32-5 anddescribed inTable 32-6.
Forexample, consider three packets tobetransmitted: Packet Aisasingle fragment (60bytes), Packet B
isfragmented over three buffers (1514 bytes total), andPacket Cisasingle fragment (1514 bytes). The
linked listofdescriptors todescribe these three packets isshown inFigure 32-6.
Figure 32-5. Basic Descriptor Format
Word
OffsetBitFields
31 1615 0
0 Next Descriptor Pointer
1 Buffer Pointer
2 Buffer Offset Buffer Length
3 Flags Packet Length

<!-- Page 1813 -->

SOP | EOP 600 60pBufferpNext
Packet A
60 bytes
0
SOPFragment 1Packet B
512
1514pBufferpNext
512 bytes
EOP00
−−−
Packet B
Fragment 3
500 bytes502
pBuffer
−−−500pNext−−−pBufferpNext
Packet B
Fragment 2
502 bytes
SOP | EOP01514 bytesPacket C
1514pBufferpNext (NULL)
1514
www.ti.com Architecture
1813 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-6. Basic Descriptor Description
Word Offset Field Field Description
0 Next Descriptor
PointerThe next descriptor pointer isused tocreate asingle linked listofdescriptors. Each descriptor
describes apacket orapacket fragment. When adescriptor points toasingle buffer packet orthe
firstfragment ofapacket, thestart ofpacket (SOP) flagissetintheflags field. When adescriptor
points toasingle buffer packet orthelastfragment ofapacket, theendofpacket (EOP) flagisset.
When apacket isfragmented, each fragment must have itsown descriptor andappear sequentially
inthedescriptor linked list.
1 Buffer Pointer The buffer pointer refers totheactual memory buffer thatcontains packet data during transmit
operations, orisanempty buffer ready toreceive packet data during receive operations.
2 Buffer Offset The buffer offset istheoffset from thestart ofthepacket buffer tothefirstbyte ofvalid data. This
field only hasmeaning when thebuffer descriptor points toabuffer thatactually contains data.
Buffer Length The buffer length istheactual number ofvalid packet data bytes stored inthebuffer. Ifthebuffer is
empty andwaiting toreceive data, thisfield represents thesize oftheempty buffer.
3 Flags The flags field contains more information about thebuffer, such as,isitthefirstfragment ina
packet (SOP), thelastfragment inapacket (EOP), orcontains anentire contiguous Ethernet
packet (both SOP andEOP). The flags aredescribed inSection 32.2.6.4 andSection 32.2.6.5 .
Packet Length The packet length only hasmeaning forbuffers thatboth contain data andarethestart ofanew
packet (SOP). Inthecase ofSOP descriptors, thepacket length field contains thelength ofthe
entire Ethernet packet, regardless ifitiscontained inasingle buffer orfragmented over several
buffers.
Figure 32-6. Typical Descriptor Linked List

<!-- Page 1814 -->

Architecture www.ti.com
1814 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.2 Transmit andReceive Descriptor Queues
The EMAC module processes descriptors inlinked lists asdiscussed inSection 32.2.6.1 .The lists used
bytheEMAC aremaintained bytheapplication software through theuseofthehead descriptor pointer
registers (HDP). The EMAC supports eight channels fortransmit andeight channels forreceive. The
corresponding head descriptor pointers are:
*TXnHDP -Transmit Channel nDMA Head Descriptor Pointer Register
*RXnHDP -Receive Channel nDMA Head Descriptor Pointer Register
After anEMAC reset andbefore enabling theEMAC forsend andreceive, all16head descriptor pointer
registers must beinitialized to0.
The EMAC uses asimple system todetermine ifadescriptor iscurrently owned bytheEMAC orbythe
application software. There isaflaginthebuffer descriptor flags called OWNER. When thisflagisset,the
packet thatisreferenced bythedescriptor isconsidered tobeowned bytheEMAC. Note thatownership
isdone onapacket based granularity, notondescriptor granularity, soonly SOP descriptors make useof
theOWNER flag. Aspackets areprocessed, theEMAC patches theSOP descriptor ofthecorresponding
packet andclears theOWNER flag. This isanindication thattheEMAC hasfinished processing all
descriptors uptoandincluding thefirstwith theEOP flagset,indicating theendofthepacket (note this
may only beonedescriptor with both theSOP andEOP flags set).
Toaddadescriptor oralinked listofdescriptors toanEMAC descriptor queue forthefirsttime, the
software application simply writes thepointer tothedescriptor orfirstdescriptor ofalisttothe
corresponding HDP register. Note thatthelastdescriptor inthelistmust have its"next"pointer cleared to
0.This istheonly way theEMAC hasofdetecting theendofthelist.Therefore, inthecase where only a
single descriptor isadded, its"next descriptor "pointer must beinitialized to0.
The HDP must never bewritten towhile alistisactive. Toaddadditional descriptors toadescriptor list
already owned bytheEMAC, theNULL "next"pointer ofthelastdescriptor oftheprevious listispatched
with apointer tothefirstdescriptor ofthenew list.The listofnew descriptors tobeappended tothe
existing listmust itself beNULL terminated before thepointer patch isperformed.
There isapotential race condition where theEMAC may read the"next"pointer ofadescriptor asNULL in
theinstant before anapplication appends additional descriptors tothelistbypatching thepointer. This
case ishandled bythesoftware application always examining thebuffer descriptor flags ofallEOP
packets, looking foraspecial flagcalled endofqueue (EOQ). The EOQ flagissetbytheEMAC onthe
lastdescriptor ofapacket when thedescriptor 's"next"pointer isNULL. This istheway theEMAC
indicates tothesoftware application thatitbelieves ithasreached theendofthelist.When thesoftware
application sees theEOQ flagset,theapplication may atthattime submit thenew list,ortheportion of
theappended listthatwas missed bywriting thenew listpointer tothesame HDP thatstarted the
process.
This process applies when adding packets toatransmit list,andempty buffers toareceive list.Figure 32-
7,Figure 32-8,andFigure 32-9 illustrate transmit operations.

<!-- Page 1815 -->

ADD PACKET(S) TO
TX QUEUE
GENERATE TX PACKET
(SHOWN IN FLOW CHART)
SOFTWARE
TX QUEUE ACTIVE?
LINK NEW PACKET IN
QUEUE BY WRITING
PREVIOUS LAST PACKET
NEXT DESC POINTER
(PREVIOUSLY ZERO)YES
ADD ANOTHER
TX PACKET?YES
SET SOFTWARE
TX QUEUE ACTIVE
TX PACKET(S) ADDEDWRITE TX QUEUE HEAD
DESCRIPTOR POINTER
NONO
www.ti.com Architecture
1815 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleFigure 32-7. Transmit Packet Add Flow Chart
Note: Software TXQUEUE ACTIVE isanindication thatatleast onepacket isintheTXqueue (from thesoftware
viewpoint).

<!-- Page 1816 -->

MORE BD'S NEEDED
FOR PACKET?GENERATE TX PACKET
NO
SET CURRENT
BD EOP BITWRITE CURRENT BD
BUFFER POINTER
WRITE CURRENT BD
OFFSET AND LENGTH
FIRST BD
IN PACKET?
CLEAR CURRENT
BD SOP BITSET CURRENT
BD SOP BITYES
NO
SET CURRENT
BD OWNERSHIP BIT
CLEAR CURRENT
BD EOQ BITGET NEW (NOW CURRENT)
BD ADDRESS
CLEAR CURRENT
BD EOP BITYES
ZERO CURRENT BD
NEXT DESC POINTER
TX PACKET COMPLETEWRITE CURRENT BD
NEXT DESC POINTERSET CURRENT
BD PACKET LENGTH
Architecture www.ti.com
1816 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleFigure 32-8. Generate Transmit Packet Flow Chart
BD=Buffer Descriptor

<!-- Page 1817 -->

PROCESS TX QUEUE
INTERRUPT
READ NEXT QUEUE BD
(SOP BUFFER)
OWNERSHIP BIT
CLEARED?
EOP BIT
SET?RECLAIM BUFFERYES
RECLAIM BUFFER
DESCRIPTOR (BD)NOYES EOQ BIT
SET?
READ NEXT BD IN QUEUERECLAIM BUFFER
DESCRIPTOR (BD)ZERO NEXT
DESC POINTER?YES
YES
EXIT TX QUEUE
INTERRUPTCLEAR SOFTWARE
TX QUEUE ACTIVEWRITE NEXT DESC
POINTER VALUE TO
QUEUE HEAD DESC
POINTER
(MISQUEUED PACKET)NOPROCESS MORE
PACKET(S)?
RECLAIM BUFFER
DESCRIPTOR (BD)
NOYES
NORECLAIM BUFFER
DESCRIPTOR (BD)
www.ti.com Architecture
1817 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleFigure 32-9. Transmit Queue Interrupt Processing Flow Chart
BD=Buffer Descriptor
Note: Whether ornottoprocess more than onepacket isasoftware decision.

<!-- Page 1818 -->

Architecture www.ti.com
1818 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.3 Transmit andReceive EMAC Interrupts
The EMAC processes descriptors inlinked listchains asdiscussed inSection 32.2.6.1 ,using thelinked
listqueue mechanism discussed inSection 32.2.6.2 .
The EMAC synchronizes descriptor listprocessing through theuseofinterrupts tothesoftware
application. The interrupts arecontrolled bytheapplication using theinterrupt masks, global interrupt
enable, andthecompletion pointer register (CP). The CPisalso called theinterrupt acknowledge register.
The EMAC supports eight channels fortransmit andeight channels forreceive. The corresponding
completion pointer registers are:
*TXnCP-Transmit Channel nCompletion Pointer (Interrupt Acknowledge) Register
*RXnCP-Receive Channel nCompletion Pointer (Interrupt Acknowledge) Register
These registers serve twopurposes. When read, they return thepointer tothelastdescriptor thatthe
EMAC hasprocessed. When written bythesoftware application, thevalue represents thelastdescriptor
processed bythesoftware application. When these twovalues donotmatch, theinterrupt isactive.
Interrupts intheEMAC control module arerouted totheVectored Interrupt Manager (VIM) asfour
separate interrupt requests. The interrupt configuration determines whether ornotanactive interrupt
request actually interrupts theCPU. Ingeneral thefollowing settings arerequired forbasic EMAC transmit
andreceive interrupts:
1.EMAC transmit andreceive interrupts areenabled bysetting themask registers RXINTMASKSET and
TXINTMASKSET
2.Global interrupts aresetintheEMAC control module: C0RXEN andC0TXEN
3.The VIM isconfigured toaccept C0_RX_PULSE andC0_TX_PULSE interrupts from theEMAC control
module
4.The normal mode (IRQ) interrupts areenabled intheCortex-R4F CPU
Whether ornottheinterrupt isenabled, thecurrent state ofthereceive ortransmit channel interrupt can
beexamined directly bythesoftware application reading theEMAC receive interrupt status (unmasked)
register (RXINTSTATRAW) andtransmit interrupt status (unmasked) register (TXINTSTATRAW).
After servicing transmit orreceive interrupts, theapplication software must acknowledge both theEMAC
andEMAC control module interrupts.
EMAC interrupts areacknowledged when theapplication software updates thevalue ofTXnCPorRXnCP
with avalue thatmatches theinternal value kept bytheEMAC. This mechanism ensures thatthe
application software never misses anEMAC interrupt because theinterrupt acknowledgment istied
directly tothebuffer descriptor processing.
EMAC control module interrupts areacknowledged when theapplication software writes theappropriate
C0TX orC0RX keytotheEMAC End-Of-Interrupt Vector register (MACEOIVECTOR). The
MACEOIVECTOR behaves asaninterrupt pulse interlock --once theEMAC control module hasissued an
interrupt pulse totheCPU, itwillnotgenerate further pulses ofthesame type until theoriginal pulse has
been acknowledged.
32.2.6.4 Transmit Buffer Descriptor Format
Atransmit (TX) buffer descriptor (Figure 32-10 )isacontiguous block offour 32-bit data words aligned on
a32-bit boundary thatdescribes apacket orapacket fragment. Example 32-1 shows thetransmit buffer
descriptor described byaCstructure.

<!-- Page 1819 -->

www.ti.com Architecture
1819 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleFigure 32-10. Transmit Buffer Descriptor Format
Word 0
31 0
Next Descriptor Pointer
Word 1
31 0
Buffer Pointer
Word 2
31 1615 0
Buffer Offset Buffer Length
Word 3
31 30 29 28 27 26 25 16
SOP EOP OWNER EOQ TDOWNCMPLT PASSCRC Reserved
15 0
Packet Length
Example 32-1. Transmit Buffer Descriptor inCStructure Format
/*
//EMAC Descriptor
//
//The following isthe format ofasingle buffer descriptor
//onthe EMAC.
*/
typedef struct _EMAC_Desc {
struct _EMAC_Desc *pNext; /*Pointer tonext descriptor inchain */
Uint8 *pBuffer; /*Pointer todata buffer */
Uint32 BufOffLen; /*Buffer Offset(MSW) and Length(LSW) */
Uint32 PktFlgLen; /*Packet Flags(MSW) and Length(LSW) */
}EMAC_Desc;
/*Packet Flags */
#define EMAC_DSC_FLAG_SOP 0x80000000u
#define EMAC_DSC_FLAG_EOP 0x40000000u
#define EMAC_DSC_FLAG_OWNER 0x20000000u
#define EMAC_DSC_FLAG_EOQ 0x10000000u
#define EMAC_DSC_FLAG_TDOWNCMPLT 0x08000000u
#define EMAC_DSC_FLAG_PASSCRC 0x04000000u

<!-- Page 1820 -->

Architecture www.ti.com
1820 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.4.1 Next Descriptor Pointer
The next descriptor pointer points tothe32-bit word aligned memory address ofthenext buffer descriptor
inthetransmit queue. This pointer isused tocreate alinked listofbuffer descriptors. Ifthevalue ofthis
pointer is0,then thecurrent buffer isthelastbuffer inthequeue. The software application must setthis
value prior toadding thedescriptor totheactive transmit list.This pointer isnotaltered bytheEMAC.
The value ofpNext should never bealtered once thedescriptor isinanactive transmit queue, unless its
current value isNULL. IfthepNext pointer isinitially NULL, andmore packets need tobequeued for
transmit, thesoftware application may alter thispointer topoint toanewly appended descriptor. The
EMAC willusethenew pointer value andproceed tothenext descriptor unless thepNext value has
already been read. Inthislatter case, thetransmitter willhaltonthetransmit channel inquestion, andthe
software application may restart itatthattime. The software candetect thiscase bychecking foranend
ofqueue (EOQ) condition flagontheupdated packet descriptor when itisreturned bytheEMAC.
32.2.6.4.2 Buffer Pointer
The buffer pointer isthebyte-aligned memory address ofthememory buffer associated with thebuffer
descriptor. The software application must setthisvalue prior toadding thedescriptor totheactive transmit
list.This pointer isnotaltered bytheEMAC.
32.2.6.4.3 Buffer Offset
This 16-bit field indicates how many unused bytes areatthestart ofthebuffer. Forexample, avalue of
0000h indicates thatnounused bytes areatthestart ofthebuffer andthatvalid data begins onthefirst
byte ofthebuffer, while avalue of000Fh indicates thatthefirst15bytes ofthebuffer aretobeignored by
theEMAC andthatvalid buffer data starts onbyte 16ofthebuffer. The software application must setthis
value prior toadding thedescriptor totheactive transmit list.This field isnotaltered bytheEMAC.
Note thatthisvalue isonly checked onthefirstdescriptor ofagiven packet (where thestart ofpacket
(SOP) flagisset). Itcannotbeused tospecify theoffset ofsubsequent packet fragments. Also, since the
buffer pointer may point toanybyte-aligned address, thisfield may beentirely superfluous, depending on
thedevice driver architecture.
The range oflegal values forthisfield is0to(Buffer Length -1).
32.2.6.4.4 Buffer Length
This 16-bit field indicates how many valid data bytes areinthebuffer. Onsingle fragment packets, this
value isalso thetotal length ofthepacket data tobetransmitted. Ifthebuffer offset field isused, theoffset
bytes arenotcounted aspart ofthislength. This length counts only valid data bytes. The software
application must setthisvalue prior toadding thedescriptor totheactive transmit list.This field isnot
altered bytheEMAC.
32.2.6.4.5 Packet Length
This 16-bit field specifies thenumber ofdata bytes intheentire packet. Any leading buffer offset bytes are
notincluded. The sum ofthebuffer length fields ofeach ofthepacket 'sfragments (ifmore than one) must
beequal tothepacket length. The software application must setthisvalue prior toadding thedescriptor to
theactive transmit list.This field isnotaltered bytheEMAC. This value isonly checked onthefirst
descriptor ofagiven packet (where thestart ofpacket (SOP) flagisset).
32.2.6.4.6 Start ofPacket (SOP) Flag
When set,thisflagindicates thatthedescriptor points toapacket buffer thatisthestart ofanew packet.
Inthecase ofasingle fragment packet, both theSOP andendofpacket (EOP) flags areset.Otherwise,
thedescriptor pointing tothelastpacket buffer forthepacket sets theEOP flag. This bitissetbythe
software application andisnotaltered bytheEMAC.

<!-- Page 1821 -->

www.ti.com Architecture
1821 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.4.7 End ofPacket (EOP) Flag
When set,thisflagindicates thatthedescriptor points toapacket buffer thatislastforagiven packet. In
thecase ofasingle fragment packet, both thestart ofpacket (SOP) andEOP flags areset.Otherwise, the
descriptor pointing tothelastpacket buffer forthepacket sets theEOP flag. This bitissetbythesoftware
application andisnotaltered bytheEMAC.
32.2.6.4.8 Ownership (OWNER) Flag
When set,thisflagindicates thatallthedescriptors forthegiven packet (from SOP toEOP) arecurrently
owned bytheEMAC. This flagissetbythesoftware application ontheSOP packet descriptor before
adding thedescriptor tothetransmit descriptor queue. Forasingle fragment packet, theSOP, EOP, and
OWNER flags areallset.The OWNER flagiscleared bytheEMAC once itisfinished with allthe
descriptors forthegiven packet. Note thatthisflagisvalid onSOP descriptors only.
32.2.6.4.9 End ofQueue (EOQ) Flag
When set,thisflagindicates thatthedescriptor inquestion was thelastdescriptor inthetransmit queue
foragiven transmit channel, andthatthetransmitter hashalted. This flagisinitially cleared bythe
software application prior toadding thedescriptor tothetransmit queue. This bitissetbytheEMAC when
theEMAC identifies thatadescriptor isthelastforagiven packet (the EOP flagisset), andthere areno
more descriptors inthetransmit list(next descriptor pointer isNULL).
The software application canusethisbittodetect when theEMAC transmitter forthecorresponding
channel hashalted. This isuseful when theapplication appends additional packet descriptors toatransmit
queue listthatisalready owned bytheEMAC. Note thatthisflagisvalid onEOP descriptors only.
32.2.6.4.10 Teardown Complete (TDOWNCMPLT) Flag
This flagisused when atransmit queue isbeing torn down, oraborted, instead ofallowing ittobe
transmitted. This would happen under device driver reset orshutdown conditions. The EMAC sets thisbit
intheSOP descriptor ofeach packet asitisaborted from transmission.
Note thatthisflagisvalid onSOP descriptors only. Also note thatonly thefirstpacket inanunsent listhas
theTDOWNCMPLT flagset.Subsequent descriptors arenotprocessed bytheEMAC.
32.2.6.4.11 Pass CRC (PASSCRC) Flag
This flagissetbythesoftware application intheSOP packet descriptor before itadds thedescriptor tothe
transmit queue. Setting thisbitindicates totheEMAC thatthe4byte Ethernet CRC isalready present in
thepacket data, andthattheEMAC should notgenerate itsown version oftheCRC.
When theCRC flagiscleared, theEMAC generates andappends the4-byte CRC. The buffer length and
packet length fields donotinclude theCRC bytes. When theCRC flagisset,the4-byte CRC issupplied
bythesoftware application andisalready appended totheendofthepacket data. The buffer length and
packet length fields include theCRC bytes, asthey arepart ofthevalid packet data. Note thatthisflagis
valid onSOP descriptors only.

<!-- Page 1822 -->

Architecture www.ti.com
1822 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.5 Receive Buffer Descriptor Format
Areceive (RX) buffer descriptor (Figure 32-11 )isacontiguous block offour 32-bit data words aligned ona
32-bit boundary thatdescribes apacket orapacket fragment. Example 32-2 shows thereceive buffer
descriptor described byaCstructure.
32.2.6.5.1 Next Descriptor Pointer
This pointer points tothe32-bitword aligned memory address ofthenext buffer descriptor inthereceive
queue. This pointer isused tocreate alinked listofbuffer descriptors. Ifthevalue ofthispointer is0,then
thecurrent buffer isthelastbuffer inthequeue. The software application must setthisvalue prior to
adding thedescriptor totheactive receive list.This pointer isnotaltered bytheEMAC.
The value ofpNext should never bealtered once thedescriptor isinanactive receive queue, unless its
current value isNULL. IfthepNext pointer isinitially NULL, andmore empty buffers canbeadded tothe
pool, thesoftware application may alter thispointer topoint toanewly appended descriptor. The EMAC
willusethenew pointer value andproceed tothenext descriptor unless thepNext value hasalready been
read. Inthislatter case, thereceiver willhaltthereceive channel inquestion, andthesoftware application
may restart itatthattime. The software candetect thiscase bychecking foranendofqueue (EOQ)
condition flagontheupdated packet descriptor when itisreturned bytheEMAC.
32.2.6.5.2 Buffer Pointer
The buffer pointer isthebyte-aligned memory address ofthememory buffer associated with thebuffer
descriptor. The software application must setthisvalue prior toadding thedescriptor totheactive receive
list.This pointer isnotaltered bytheEMAC.

<!-- Page 1823 -->

www.ti.com Architecture
1823 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleFigure 32-11. Receive Buffer Descriptor Format
Word 0
31 0
Next Descriptor Pointer
Word 1
31 0
Buffer Pointer
Word 2
31 1615 0
Buffer Offset Buffer Length
Word 3
31 30 29 28 27 26 25 24
SOP EOP OWNER EOQ TDOWNCMPLT PASSCRC JABBER OVERSIZE
23 22 21 20 19 18 17 16
FRAGMENT UNDERSIZED CONTROL OVERRUN CODEERROR ALIGNERROR CRCERROR NOMATCH
15 0
Packet Length
Example 32-2. Receive Buffer Descriptor inCStructure Format
/*
//EMAC Descriptor
//
//The following isthe format ofasingle buffer descriptor
//onthe EMAC.
*/
typedef struct _EMAC_Desc {
struct _EMAC_Desc *pNext; /*Pointer tonext descriptor inchain */
Uint8 *pBuffer; /*Pointer todata buffer */
Uint32 BufOffLen; /*Buffer Offset(MSW) and Length(LSW) */
Uint32 PktFlgLen; /*Packet Flags(MSW) and Length(LSW) */
}EMAC_Desc;
/*Packet Flags */
#define EMAC_DSC_FLAG_SOP 0x80000000u
#define EMAC_DSC_FLAG_EOP 0x40000000u
#define EMAC_DSC_FLAG_OWNER 0x20000000u
#define EMAC_DSC_FLAG_EOQ 0x10000000u
#define EMAC_DSC_FLAG_TDOWNCMPLT 0x08000000u
#define EMAC_DSC_FLAG_PASSCRC 0x04000000u
#define EMAC_DSC_FLAG_JABBER 0x02000000u
#define EMAC_DSC_FLAG_OVERSIZE 0x01000000u
#define EMAC_DSC_FLAG_FRAGMENT 0x00800000u

<!-- Page 1824 -->

Architecture www.ti.com
1824 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleExample 32-2. Receive Buffer Descriptor inCStructure Format (continued)
#define EMAC_DSC_FLAG_UNDERSIZED 0x00400000u
#define EMAC_DSC_FLAG_CONTROL 0x00200000u
#define EMAC_DSC_FLAG_OVERRUN 0x00100000u
#define EMAC_DSC_FLAG_CODEERROR 0x00080000u
#define EMAC_DSC_FLAG_ALIGNERROR 0x00040000u
#define EMAC_DSC_FLAG_CRCERROR 0x00020000u
#define EMAC_DSC_FLAG_NOMATCH 0x00010000u
32.2.6.5.3 Buffer Offset
This 16-bit field must beinitialized to0bythesoftware application before adding thedescriptor toa
receive queue.
Whether ornotthisfield isupdated depends onthesetting oftheRXBUFFEROFFSET register. When the
offset register issettoanonzero value, thereceived packet iswritten tothepacket buffer atanoffset
given bythevalue oftheregister, andthisvalue isalso written tothebuffer offset field ofthedescriptor.
When apacket isfragmented over multiple buffers because itdoes notfitinthefirstbuffer supplied, the
buffer offset only applies tothefirstbuffer inthelist,which iswhere thestart ofpacket (SOP) flagissetin
thecorresponding buffer descriptor. Inother words, thebuffer offset field isonly updated bytheEMAC on
SOP descriptors.
The range oflegal values fortheBUFFEROFFSET register is0to(Buffer Length -1)forthesmallest
value ofbuffer length foralldescriptors inthelist.

<!-- Page 1825 -->

www.ti.com Architecture
1825 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.5.4 Buffer Length
This 16-bit field isused fortwopurposes:
*Before thedescriptor isfirstplaced onthereceive queue bytheapplication software, thebuffer length
field isfirstinitialized bythesoftware tohave thephysical size oftheempty data buffer pointed toby
thebuffer pointer field.
*After theempty buffer hasbeen processed bytheEMAC andfilled with received data bytes, thebuffer
length field isupdated bytheEMAC toreflect theactual number ofvalid data bytes written tothe
buffer.
32.2.6.5.5 Packet Length
This 16-bit field specifies thenumber ofdata bytes intheentire packet. This value isinitialized to0bythe
software application forempty packet buffers. The value isfilled inbytheEMAC onthefirstbuffer used
foragiven packet. This issignified bytheEMAC setting astart ofpacket (SOP) flag. The packet length is
setbytheEMAC onallSOP buffer descriptors.
32.2.6.5.6 Start ofPacket (SOP) Flag
When set,thisflagindicates thatthedescriptor points toapacket buffer thatisthestart ofanew packet.
Inthecase ofasingle fragment packet, both theSOP andendofpacket (EOP) flags areset.Otherwise,
thedescriptor pointing tothelastpacket buffer forthepacket hastheEOP flagset.This flagisinitially
cleared bythesoftware application before adding thedescriptor tothereceive queue. This bitissetbythe
EMAC onSOP descriptors.
32.2.6.5.7 End ofPacket (EOP) Flag
When set,thisflagindicates thatthedescriptor points toapacket buffer thatislastforagiven packet. In
thecase ofasingle fragment packet, both thestart ofpacket (SOP) andEOP flags areset.Otherwise, the
descriptor pointing tothelastpacket buffer forthepacket hastheEOP flagset.This flagisinitially cleared
bythesoftware application before adding thedescriptor tothereceive queue. This bitissetbytheEMAC
onEOP descriptors.
32.2.6.5.8 Ownership (OWNER) Flag
When set,thisflagindicates thatthedescriptor iscurrently owned bytheEMAC. This flagissetbythe
software application before adding thedescriptor tothereceive descriptor queue. This flagiscleared by
theEMAC once itisfinished with agiven setofdescriptors, associated with areceived packet. The flagis
updated bytheEMAC onSOP descriptor only. Sowhen theapplication identifies thattheOWNER flagis
cleared onanSOP descriptor, itmay assume thatalldescriptors uptoandincluding thefirstwith theEOP
flagsethave been released bytheEMAC. (Note thatinthecase ofsingle buffer packets, thesame
descriptor willhave both theSOP andEOP flags set.)
32.2.6.5.9 End ofQueue (EOQ) Flag
When set,thisflagindicates thatthedescriptor inquestion was thelastdescriptor inthereceive queue for
agiven receive channel, andthatthecorresponding receiver channel hashalted. This flagisinitially
cleared bythesoftware application prior toadding thedescriptor tothereceive queue. This bitissetby
theEMAC when theEMAC identifies thatadescriptor isthelastforagiven packet received (also sets the
EOP flag), andthere arenomore descriptors inthereceive list(next descriptor pointer isNULL).
The software application canusethisbittodetect when theEMAC receiver forthecorresponding channel
hashalted. This isuseful when theapplication appends additional free buffer descriptors toanactive
receive queue. Note thatthisflagisvalid onEOP descriptors only.
32.2.6.5.10 Teardown Complete (TDOWNCMPLT) Flag
This flagisused when areceive queue isbeing torn down, oraborted, instead ofbeing filled with received
data. This would happen under device driver reset orshutdown conditions. The EMAC sets thisbitinthe
descriptor ofthefirstfree buffer when thetear down occurs. Noadditional queue processing isperformed.

<!-- Page 1826 -->

Architecture www.ti.com
1826 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.6.5.11 Pass CRC (PASSCRC) Flag
This flagissetbytheEMAC intheSOP buffer descriptor ifthereceived packet includes the4-byte CRC.
This flagshould becleared bythesoftware application before submitting thedescriptor tothereceive
queue.
32.2.6.5.12 Jabber Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet isajabber frame andwas
notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE. Jabber frames areframes that
exceed theRXMAXLEN inlength, andhave CRC, code, oralignment errors.
32.2.6.5.13 Oversize Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet isanoversized frame and
was notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE.
32.2.6.5.14 Fragment Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet isonly apacket fragment
andwas notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE.
32.2.6.5.15 Undersized Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet isundersized andwas
notdiscarded because theRXCSFEN bitwas setintheRXMBPENABLE.
32.2.6.5.16 Control Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet isanEMAC control frame
andwas notdiscarded because theRXCMFEN bitwas setintheRXMBPENABLE.
32.2.6.5.17 Overrun Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet was aborted duetoa
receive overrun.
32.2.6.5.18 Code Error (CODEERROR) Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet contained acode error
andwas notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE.
32.2.6.5.19 Alignment Error (ALIGNERROR) Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet contained analignment
error andwas notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE.
32.2.6.5.20 CRC Error (CRCERROR) Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet contained aCRC error
andwas notdiscarded because theRXCEFEN bitwas setintheRXMBPENABLE.
32.2.6.5.21 NoMatch (NOMATCH) Flag
This flagissetbytheEMAC intheSOP buffer descriptor, ifthereceived packet didnotpass anyofthe
EMAC 'saddress match criteria andwas notdiscarded because theRXCAFEN bitwas setinthe
RXMBPENABLE. Although thepacket isavalid Ethernet data packet, itwas only received because the
EMAC isinpromiscuous mode.

<!-- Page 1827 -->

Arbiter and
bus switchesCPUDMA Controllers
8K byte
descriptor
memory
Configuration
registers
Interrupt
logicInterrupts
to CPUEMAC interrupts
MDIO interruptsConfiguration busTransmit and Receive
www.ti.com Architecture
1827 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.7 EMAC Control Module
The EMAC control module (Figure 32-12 )interfaces theEMAC andMDIO modules totherestofthe
system, andalso provides alocal memory space tohold EMAC packet buffer descriptors. Local memory
isused tohelp avoid contention with device memory spaces. Other functions include thebusarbiter and
theinterrupt logic control.
Figure 32-12. EMAC Control Module Block Diagram
32.2.7.1 Internal Memory
The EMAC control module includes 8Kbytes ofinternal memory (CPPI buffer descriptor memory). The
internal memory block isessential forallowing theEMAC tooperate more independently oftheCPU. It
also prevents memory underflow conditions when theEMAC issues read orwrite requests todescriptor
memory. (Memory accesses toread orwrite theactual Ethernet packet data areprotected bytheEMAC's
internal FIFOs).
Adescriptor isa16-byte memory structure thatholds information about asingle Ethernet packet buffer,
which may contain afullorpartial Ethernet packet. Thus with the8Kmemory block provided fordescriptor
storage, theEMAC module cansend andreceived uptoacombined 512packets before itneeds tobe
serviced byapplication ordriver software.
32.2.7.2 Interrupt Control
Interrupt conditions generated bytheEMAC andMDIO modules arecombined intofour interrupt signals
thatarerouted totheVectored Interrupt Manager (VIM); theVIM then relays theinterrupt signals tothe
CPU. The EMAC control module uses twosets ofregisters tocontrol theinterrupt signals totheCPU:
*C0RXTHRESHEN, C0RXEN, C0TXEN, andC0MISCEN registers enable thepulse signals thatare
mapped totheVIM
*INTCONTROL, C0RXIMAX, andC0TXIMAX registers enable interrupt pacing tolimit thenumber of
interrupt pulses generated permillisecond
Interrupts must beacknowledged bywriting theappropriate value totheEMAC End-Of-Interrupt Vector
(MACEOIVECTOR). The MACEOIVECTOR behaves asaninterrupt pulse interlock --once theEMAC
control module hasissued aninterrupt pulse totheCPU, itwillnotgenerate further pulses ofthesame
type until theoriginal pulse hasbeen acknowledged.

<!-- Page 1828 -->

EMAC
control
module
Control
registers
and logicPHY
monitoringPeripheral
clockMDIO
clock
generator
USERINTMDIO
interface
pollingPHYMDCLK
MDIO
LINKINT
Configuration bus
Architecture www.ti.com
1828 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.7.3 Bus Arbiter
The EMAC control module busarbiter operates transparently totherestofthesystem. Itisused:
*Toarbitrate between theCPU andEMAC buses foraccess tointernal descriptor memory.
*Toarbitrate between internal EMAC buses foraccess tosystem memory.
32.2.8 MDIO Module
The MDIO module isused tomanage upto32physical layer (PHY) devices connected totheEthernet
Media Access Controller (EMAC). The device supports asingle PHY being connected totheEMAC atany
given time. The MDIO module isdesigned toallow almost transparent operation oftheMDIO interface
with little maintenance from theCPU.
The MDIO module continuously polls 32MDIO addresses inorder toenumerate allPHY devices inthe
system. Once aPHY device hasbeen detected, theMDIO module reads theMDIO PHY linkstatus
register (LINK) tomonitor thePHY linkstate. Link change events arestored intheMDIO module, which
caninterrupt theCPU. This storing oftheevents allows theCPU topollthelinkstatus ofthePHY device
without continuously performing MDIO module accesses. However, when theCPU must access theMDIO
module forconfiguration andnegotiation, theMDIO module performs theMDIO read orwrite operation
independent oftheCPU. This independent operation allows theprocessor topollforcompletion or
interrupt theCPU once theoperation hascompleted.
The MDIO module does notsupport the"Clause 45"interface.
32.2.8.1 MDIO Module Components
The MDIO module (Figure 32-13 )interfaces tothePHY components through twoMDIO pins (MDIO_CLK
andMDIO), andtotheCPU through theEMAC control module andtheconfiguration bus. The MDIO
module consists ofthefollowing logical components:
*MDIO clock generator
*Global PHY detection andlinkstate monitoring
*Active PHY monitoring
*PHY register user access
Figure 32-13. MDIO Module Block Diagram

<!-- Page 1829 -->

www.ti.com Architecture
1829 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.8.1.1 MDIO Clock Generator
The MDIO clock generator controls theMDIO clock based onadivide-down oftheVCLK3 peripheral clock
intheEMAC control module. The MDIO clock isspecified torunupto2.5MHz, although typical operation
would be1.0MHz. Since theVCLK3 peripheral clock frequency isconfigurable, theapplication software
ordriver controls thedivide-down amount. See thedevice datasheet forperipheral clock speed
specifications.
32.2.8.1.2 Global PHY Detection andLink State Monitoring
The MDIO module continuously polls all32MDIO addresses inorder toenumerate thePHY devices inthe
system. The module tracks whether ornotaPHY onaparticular address hasresponded, andwhether or
notthePHY currently hasalink. Using thisinformation allows thesoftware application toquickly
determine which MDIO address thePHY isusing.
32.2.8.1.3 Active PHY Monitoring
Once aPHY candidate hasbeen selected foruse, theMDIO module transparently monitors itslinkstate
byreading theMDIO PHY linkstatus register (LINK). Link change events arestored ontheMDIO device
andcanoptionally interrupt theCPU. This allows thesystem topollthelinkstatus ofthePHY device
without continuously performing costly MDIO accesses.
32.2.8.1.4 PHY Register User Access
When theCPU must access MDIO forconfiguration andnegotiation, thePHY access module performs
theactual MDIO read orwrite operation independent oftheCPU. This allows theCPU topollfor
completion orreceive aninterrupt when theread orwrite operation hasbeen performed. The user access
registers USERACCESS nallows thesoftware tosubmit theaccess requests forthePHY connected tothe
device.

<!-- Page 1830 -->

Architecture www.ti.com
1830 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.8.2 MDIO Module Operational Overview
The MDIO module implements the802.3 serial management interface tointerrogate andcontrol an
Ethernet PHY, using ashared two-wired bus. Itseparately performs autodetection andrecords thecurrent
linkstatus ofupto32PHYs, polling all32MDIO addresses.
Application software uses theMDIO module toconfigure theautonegotiation parameters ofthePHY
attached totheEMAC, retrieve thenegotiation results, andconfigure required parameters intheEMAC.
Inthisdevice, theEthernet PHY attached tothesystem canbedirectly controlled andqueried. The Media
Independent Interface (MII) address ofthisPHY device isspecified inoneofthePHYADRMON bitsinthe
MDIO user PHY select register (USERPHYSEL n).The MDIO module canbeprogrammed totrigger a
CPU interrupt onaPHY linkchange event, bysetting theLINKINTENB bitinUSERPHYSEL n.Reads and
writes toregisters inthisPHY device areperformed using theMDIO user access register
(USERACCESS n).
The MDIO module powers-up inanidlestate until specifically enabled bysetting theENABLE bitinthe
MDIO control register (CONTROL). Atthistime, theMDIO clock divider andpreamble mode selection are
also configured. The MDIO preamble isenabled bydefault, butcanbedisabled when theconnected PHY
does notrequire it.Once theMDIO module isenabled, theMDIO interface state machine continuously
polls thePHY linkstatus (byreading thegeneric status register) ofallpossible 32PHY addresses and
records theresults intheMDIO PHY alive status register (ALIVE) andMDIO PHY linkstatus register
(LINK). The corresponding bitfortheconnected PHY (0-31) issetinALIVE, ifthePHY responded tothe
read request. The corresponding bitissetinLINK, ifthePHY responded andalso iscurrently linked. In
addition, anyPHY register read transactions initiated bytheapplication software using USERACCESS n
causes ALIVE tobeupdated.
The USERPHYSEL nisused totrack thelinkstatus oftheconnected PHY address. Achange inthelink
status ofthePHY being monitored sets theappropriate bitintheMDIO linkstatus change interrupt
registers (LINKINTRAW andLINKINTMASKED), ifenabled bytheLINKINTENB bitinUSERPHYSEL n.
While theMDIO module isenabled, thehost issues aread orwrite transaction over theMIImanagement
interface using theDATA, PHYADR, REGADR, andWRITE bitsinUSERACCESS n.When theapplication
sets theGObitinUSERACCESS n,theMDIO module begins thetransaction without anyfurther
intervention from theCPU. Upon completion, theMDIO module clears theGObitandsets the
corresponding USERINTRAW bit(0or1)intheMDIO user command complete interrupt register
(USERINTRAW) corresponding toUSERACCESS nused. The corresponding USERINTMASKED bit(0or
1)intheMDIO user command complete interrupt register (USERINTMASKED) may also beset,
depending onthemask setting configured intheMDIO user command complete interrupt mask set
register (USERINTMASKSET) andtheMDIO user interrupt mask clear register (USERINTMASKCLEAR).
Around-robin arbitration scheme isused toschedule transactions thatmay bequeued using both
USERACCESS0 andUSERACCESS1. The application software must check thestatus oftheGObitin
USERACCESS nbefore initiating anew transaction, toensure thattheprevious transaction has
completed. The application software canusetheACK bitinUSERACCESS ntodetermine thestatus ofa
read transaction.

<!-- Page 1831 -->

www.ti.com Architecture
1831 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.8.2.1 Initializing theMDIO Module
The following steps areperformed bytheapplication software ordevice driver toinitialize theMDIO
device:
1.Configure thePREAMBLE andCLKDIV bitsintheMDIO control register (CONTROL).
2.Enable theMDIO module bysetting theENABLE bitinCONTROL.
3.The MDIO PHY alive status register (ALIVE) canberead inpolling fashion until aPHY connected to
thesystem responded, andtheMDIO PHY linkstatus register (LINK) candetermine whether thisPHY
already hasalink.
4.Setup theappropriate PHY addresses intheMDIO user PHY select register (USERPHYSEL n),andset
theLINKINTENB bittoenable alinkchange event interrupt ifdesirable.
5.Ifaninterrupt ongeneral MDIO register access isdesired, setthecorresponding bitintheMDIO user
command complete interrupt mask setregister (USERINTMASKSET) tousetheMDIO user access
register (USERACCESS n).Since only onePHY isused inthisdevice, theapplication software canuse
oneUSERACCESS ntotrigger acompletion interrupt; theother USERACCESS nisnotsetup.
32.2.8.2.2 Writing Data ToaPHY Register
The MDIO module includes auser access register (USERACCESS n)todirectly access aspecified PHY
device. Towrite aPHY register, perform thefollowing:
1.Check toensure thattheGObitintheMDIO user access register (USERACCESS n)iscleared.
2.Write totheGO, WRITE, REGADR, PHYADR, andDATA bitsinUSERACCESS ncorresponding tothe
PHY andPHY register youwant towrite.
3.The write operation tothePHY isscheduled andcompleted bytheMDIO module. Completion ofthe
write operation canbedetermined bypolling theGObitinUSERACCESS nfora0.
4.Completion oftheoperation sets thecorresponding USERINTRAW bit(0or1)intheMDIO user
command complete interrupt register (USERINTRAW) corresponding toUSERACCESS nused. If
interrupts have been enabled onthisbitusing theMDIO user command complete interrupt mask set
register (USERINTMASKSET), then thebitisalso setintheMDIO user command complete interrupt
register (USERINTMASKED) andaninterrupt istriggered ontheCPU.
32.2.8.2.3 Reading Data From aPHY Register
The MDIO module includes auser access register (USERACCESS n)todirectly access aspecified PHY
device. Toread aPHY register, perform thefollowing:
1.Check toensure thattheGObitintheMDIO user access register (USERACCESS n)iscleared.
2.Write totheGO, REGADR, andPHYADR bitsinUSERACCESS ncorresponding tothePHY andPHY
register youwant toread.
3.The read data value isavailable intheDATA bitsinUSERACCESS nafter themodule completes the
read operation ontheserial bus. Completion oftheread operation canbedetermined bypolling the
GOandACK bitsinUSERACCESS n.Once theGObithascleared, theACK bitissetonasuccessful
read.
4.Completion oftheoperation sets thecorresponding USERINTRAW bit(0or1)intheMDIO user
command complete interrupt register (USERINTRAW) corresponding toUSERACCESS nused. If
interrupts have been enabled onthisbitusing theMDIO user command complete interrupt mask set
register (USERINTMASKSET), then thebitisalso setintheMDIO user command complete interrupt
register (USERINTMASKED) andaninterrupt istriggered ontheCPU.

<!-- Page 1832 -->

Architecture www.ti.com
1832 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.8.2.4 Example ofMDIO Register Access Code
The MDIO module uses theMDIO user access register (USERACCESS n)toaccess thePHY control
registers. Software functions thatimplement theaccess process may simply bethefollowing four macros:
*PHYREG_read( regadr, phyadr ) Start theprocess ofreading aPHY register
*PHYREG_write( regadr, phyadr, data ) Start theprocess ofwriting aPHY register
*PHYREG_wait( ) Synchronize operation (make sure read/write isidle)
*PHYREG_waitResults( results ) Wait forread tocomplete andreturn data read
Note thatitisnotnecessary towait after awrite operation, aslong asthestatus ischecked before every
operation tomake sure theMDIO hardware isidle. Analternative approach istocallPHYREG_wait() after
every write, andPHYREG_waitResults( )after every read, then thehardware canbeassumed tobeidle
when starting anew operation.
The implementation ofthese macros using thechip support library (CSL) isshown inExample 32-3
(USERACCESS0 isassumed).
Note thatthisimplementation does notcheck theACK bitinUSERACCESS nonPHY register reads (does
notfollow theprocedure outlined inSection 32.2.8.2.3 ).Since theMDIO PHY alive status register (ALIVE)
isused toinitially select aPHY, itisassumed thatthePHY isacknowledging read operations. Itis
possible thataPHY could become inactive atafuture point intime. Anexample ofthiswould beaPHY
thatcanhave itsMDIO addresses changed while thesystem isrunning. Itisnotvery likely, butthis
condition canbetested byperiodically checking thePHY state inALIVE.
Example 32-3. MDIO Register Access Macros
#define PHYREG_read(regadr, phyadr)
MDIO_REGS- >USERACCESS0 =
CSL_FMK(MDIO_USERACCESS0_GO,1u) |/
CSL_FMK(MDIO_USERACCESS0_REGADR,regadr) |/
CSL_FMK(MDIO_USERACCESS0_PHYADR,phyadr)
#define PHYREG_write(regadr, phyadr, data)
MDIO_REGS- >USERACCESS0 =
CSL_FMK(MDIO_USERACCESS0_GO,1u) |/
CSL_FMK(MDIO_USERACCESS0_WRITE,1) |/
CSL_FMK(MDIO_USERACCESS0_REGADR,regadr) |/
CSL_FMK(MDIO_USERACCESS0_PHYADR,phyadr) |/
CSL_FMK(MDIO_USERACCESS0_DATA, data)
#define PHYREG_wait()
while( CSL_FEXT(MDIO_REGS- >USERACCESS0,MDIO_USERACCESS0_GO) )
#define PHYREG_waitResults( results ){
while( CSL_FEXT(MDIO_REGS- >USERACCESS0,MDIO_USERACCESS0_GO) );
results =CSL_FEXT(MDIO_REGS- >USERACCESS0, MDIO_USERACCESS0_DATA); }

<!-- Page 1833 -->

Clock and
reset logic
Receive
DMA engine
Interrupt
controller
Transmit
DMA engine
Control
registersConfiguration bus
EMAC
control
module
Configuration busRAMStateFIFOReceive
FIFOTransmit MAC
transmitterStatisticsreceiverMAC
SYNCMIIaddressReceive
RMII
www.ti.com Architecture
1833 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.9 EMAC Module
This section discusses thearchitecture andbasic function oftheEMAC module.
32.2.9.1 EMAC Module Components
The EMAC module (Figure 32-14 )interfaces totheoutside world through theMedia Independent Interface
(MII) orReduced Media Independent Interface (RMII). The interface between theEMAC module andthe
system core isprovided through theEMAC control module. The EMAC consists ofthefollowing logical
components:
*The receive path includes: receive DMA engine, receive FIFO, andMAC receiver
*The transmit path includes: transmit DMA engine, transmit FIFO, andMAC transmitter
*Statistics logic
*State RAM
*Interrupt controller
*Control registers andlogic
*Clock andreset logic
Figure 32-14. EMAC Module Block Diagram
32.2.9.1.1 Receive DMA Engine
The receive DMA engine istheinterface between thereceive FIFO andthesystem core. Itinterfaces to
theCPU through thebusarbiter intheEMAC control module. This DMA engine istotally independent of
thedevice DMA.
32.2.9.1.2 Receive FIFO
The receive FIFO consists ofthree cells of64-bytes each andassociated control logic. The FIFO buffers
receive data inpreparation forwriting intopacket buffers indevice memory.

<!-- Page 1834 -->

Architecture www.ti.com
1834 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.9.1.3 MAC Receiver
The MAC receiver detects andprocesses incoming network frames, de-frames them, andputs them into
thereceive FIFO. The MAC receiver also detects errors andpasses statistics tothestatistics RAM.
32.2.9.1.4 Transmit DMA Engine
The transmit DMA engine istheinterface between thetransmit FIFO andtheCPU. Itinterfaces tothe
CPU through thebusarbiter intheEMAC control module.
32.2.9.1.5 Transmit FIFO
The transmit FIFO consists ofthree cells of64-bytes each andassociated control logic. The FIFO buffers
data inpreparation fortransmission.
32.2.9.1.6 MAC Transmitter
The MAC transmitter formats frame data from thetransmit FIFO andtransmits thedata using the
CSMA/CD access protocol. The frame CRC canbeautomatically appended, ifrequired. The MAC
transmitter also detects transmission errors andpasses statistics tothestatistics registers.
32.2.9.1.7 Statistics Logic
The Ethernet statistics arecounted andstored inthestatistics logic RAM. This statistics RAM keeps track
of36different Ethernet packet statistics.
32.2.9.1.8 State RAM
State RAM contains thehead descriptor pointers andcompletion pointers registers forboth transmit and
receive channels.
32.2.9.1.9 EMAC Interrupt Controller
The interrupt controller contains theinterrupt related registers andlogic. The 26rawEMAC interrupts are
input tothissubmodule andmasked module interrupts areoutput.
32.2.9.1.10 Control Registers andLogic
The EMAC iscontrolled byasetofmemory-mapped registers. The control logic also signals transmit,
receive, andstatus related interrupts totheCPU through theEMAC control module.
32.2.9.1.11 Clock andReset Logic
The clock andreset submodule generates alltheEMAC clocks andresets. Formore details onreset
capabilities, seeSection 32.2.15.1 .
32.2.9.2 EMAC Module Operational Overview
After reset, initialization, andconfiguration, thehost may initiate transmit operations. Transmit operations
areinitiated byhost writes totheappropriate transmit channel head descriptor pointer contained inthe
state RAM block. The transmit DMA controller then fetches thefirstpacket inthepacket chain from
memory. The DMA controller writes thepacket intothetransmit FIFO inbursts of64-byte cells. When the
threshold number ofcells, configurable using theTXCELLTHRESH bitintheFIFO control register
(FIFOCONTROL), have been written tothetransmit FIFO, oracomplete packet, whichever issmaller, the
MAC transmitter then initiates thepacket transmission. The SYNC block transmits thepacket over theMII
orRMII interfaces inaccordance with the802.3 protocol. Transmit statistics arecounted bythestatistics
block.

<!-- Page 1835 -->

www.ti.com Architecture
1835 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleReceive operations areinitiated byhost writes totheappropriate receive channel head descriptor pointer
after host initialization andconfiguration. The SYNC submodule receives packets andstrips offthe
Ethernet related protocol. The packet data isinput totheMAC receiver, which checks foraddress match
andprocesses errors. Accepted packets arethen written tothereceive FIFO inbursts of64-byte cells.
The receive DMA controller then writes thepacket data tomemory. Receive statistics arecounted bythe
statistics block.
The EMAC module operates independently oftheCPU. Itisconfigured andcontrolled byitsregister set
mapped intodevice memory. Information about data packets iscommunicated byuseof16-byte
descriptors thatareplaced inan8K-byte block ofRAM intheEMAC control module (CPPI buffer
descriptor memory).
Fortransmit operations, each 16-byte descriptor describes apacket orpacket fragment inthesystem's
internal orexternal memory. Forreceive operations, each 16-byte descriptor represents afree packet
buffer orbuffer fragment. Onboth transmit andreceive, anEthernet packet isallowed tospan oneor
more memory fragments, represented byone16-byte descriptor perfragment. Intypical operation, there is
only onedescriptor perreceive buffer, buttransmit packets may befragmented, depending onthe
software architecture.
Aninterrupt isissued totheCPU whenever atransmit orreceive operation hascompleted. However, itis
notnecessary fortheCPU toservice theinterrupt while there areadditional resources available. Inother
words, theEMAC continues toreceive Ethernet packets until itsreceive descriptor listhasbeen
exhausted. Ontransmit operations, thetransmit descriptors need only beserviced torecover their
associated memory buffer. Thus, itispossible todelay servicing oftheEMAC interrupt ifthere arereal-
time tasks toperform.
Eight channels aresupplied forboth transmit andreceive operations. Ontransmit, theeight channels
represent eight independent transmit queues. The EMAC canbeconfigured totreat these channels asan
equal priority "round-robin" queue orasasetofeight fixed-priority queues. Onreceive, theeight channels
represent eight independent receive queues with packet classification. Packets areclassified based onthe
destination MAC address. Each oftheeight channels isassigned itsown MAC address, enabling the
EMAC module toactlikeeight virtual MAC adapters. Also, specific types offrames canbesent tospecific
channels. Forexample, multicast, broadcast, orother (promiscuous, error, etc.), caneach bereceived on
aspecific receive channel queue.
The EMAC keeps track of36different statistics, plus keeps thestatus ofeach individual packet inits
corresponding packet descriptor.
32.2.10 MAC Interface
The following sections discuss theoperation oftheMedia Independent Interface (MII) andReduced Media
Independent Interface (RMII) in10Mbps and100Mbps mode. AnIEEE 802.3 compliant Ethernet MAC
controls theinterface.
32.2.10.1 Data Reception
32.2.10.1.1 Receive Control
Data received from thePHY isinterpreted andoutput totheEMAC receive FIFO. Interpretation involves
detection andremoval ofthepreamble andstart-of-frame delimiter, extraction oftheaddress andframe
length, data handling, error checking andreporting, cyclic redundancy checking (CRC), andstatistics
control signal generation. Address detection andframe filtering isperformed outside theMAC interface.
32.2.10.1.2 Receive Inter-Frame Interval
The 802.3 standard requires aninterpacket gap(IPG), which is96bittimes. However, theEMAC can
tolerate areduced IPG of8bittimes with acorrect preamble andstart frame delimiter. This interval
between frames must comprise (inthefollowing order):
1.AnInterpacket Gap (IPG).
2.A7-byte preamble (allbytes 55h).
3.A1-byte start offrame delimiter (5Dh).

<!-- Page 1836 -->

Architecture www.ti.com
1836 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.10.1.3 Receive Flow Control
When enabled andtriggered, receive flow control isinitiated tolimit theEMAC from further frame
reception. Two forms ofreceive buffer flow control areavailable:
*Collision-based flow control forhalf-duplex mode
*IEEE 802.3x pause frames flow control forfull-duplex mode
Ineither case, receive flow control prevents frame reception byissuing theflow control appropriate forthe
current mode ofoperation. Receive flow control prevents reception offrames ontheEMAC until allofthe
triggering conditions clear, atwhich time frames may again bereceived bytheEMAC.
Receive flow control isenabled bytheRXBUFFERFLOWEN bitintheMAC control register
(MACCONTROL). The EMAC isconfigured forcollision orIEEE 802.3X flow control using the
FULLDUPLEX bitinMACCONTROL. Receive flow control istriggered when thenumber offree buffers in
anyenabled receive channel free buffer count register (RXnFREEBUFFER) isless than orequal tothe
receive channel flow control threshold register (RXnFLOWTHRESH) value. Receive flow control is
independent ofreceive QOS, except thatboth usethefree buffer values.
32.2.10.1.3.1 Collision-Based Receive Buffer Flow Control
Collision-based receive buffer flow control provides ameans ofpreventing frame reception when the
EMAC isoperating inhalf-duplex mode (the FULLDUPLEX bitiscleared inMACCONTROL). When
receive flow control isenabled andtriggered, theEMAC generates collisions forreceived frames. The jam
sequence transmitted isthe12-byte sequence C3.C3.C3.C3.C3.C3.C3.C3.C3.C3.C3.C3h. The jam
sequence begins nolater than approximately asthesource address starts tobereceived. Note thatthese
forced collisions arenotlimited toamaximum of16consecutive collisions, andareindependent ofthe
normal back-off algorithm.
Receive flow control does notdepend onthevalue oftheincoming frame destination address. Acollision
isgenerated foranyincoming packet, regardless ofthedestination address, ifanyEMAC enabled
channel 'sfree buffer register value isless than orequal tothechannel 'sflow threshold value.
32.2.10.1.3.2 IEEE 802.3x-Based Receive Buffer Flow Control
IEEE 802.3x-based receive buffer flow control provides ameans ofpreventing frame reception when the
EMAC isoperating infull-duplex mode (the FULLDUPLEX bitissetinMACCONTROL). When receive
flow control isenabled andtriggered, theEMAC transmits apause frame torequest thatthesending
station stop transmitting fortheperiod indicated within thetransmitted pause frame.
The EMAC transmits apause frame tothereserved multicast address atthefirstavailable opportunity
(immediately ifcurrently idleorfollowing thecompletion oftheframe currently being transmitted). The
pause frame contains themaximum possible value forthepause time (FFFFh). The EMAC counts the
receive pause frame time (decrements FF00h to0)andretransmits anoutgoing pause frame, ifthecount
reaches 0.When theflow control request isremoved, theEMAC transmits apause frame with azero
pause time tocancel thepause request.
Note thattransmitted pause frames areonly arequest totheother endstation tostop transmitting.
Frames thatarereceived during thepause interval arereceived normally (provided thereceive FIFO isnot
full).
Pause frames aretransmitted ifenabled andtriggered, regardless ofwhether ornottheEMAC is
observing thepause time period from anincoming pause frame.
The EMAC transmits pause frames asdescribed below:
*The 48-bit reserved multicast destination address 01.80.C2.00.00.01h.
*The 48-bit source address (setusing theMACSRCADDRLO andMACSRCADDRHI registers).
*The 16-bit length/type field containing thevalue 88.08h.
*The 16-bit pause opcode equal to00.01h.
*The 16-bit pause time value ofFF.FFh. Apause-quantum is512bit-times. Pause frames sent to
cancel apause request have apause time value of00.00h.
*Zero padding to64-byte data length (EMAC transmits only 64-byte pause frames).

<!-- Page 1837 -->

www.ti.com Architecture
1837 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module*The 32-bit frame-check sequence (CRC word).
Allquantities arehexadecimal andaretransmitted most-significant byte first. The least-significant bit(LSB)
istransferred firstineach byte.
IftheRXBUFFERFLOWEN bitinMACCONTROL iscleared to0while thepause time isnonzero, then the
pause time iscleared to0andazero count pause frame issent.
32.2.10.2 Data Transmission
The EMAC passes data tothePHY from thetransmit FIFO (when enabled). Data issynchronized tothe
transmit clock rate. Transmission begins when there areTXCELLTHRESH cells of64bytes each, ora
complete packet, intheFIFO.
32.2.10.2.1 Transmit Control
Ajamsequence isoutput ifacollision isdetected onatransmit packet. Ifthecollision was late(after the
first64bytes have been transmitted), thecollision isignored. Ifthecollision isnotlate, thecontroller will
back offbefore retrying theframe transmission. When operating infull-duplex mode, thecarrier sense
(MII_CRS) andcollision-sensing (MII_COL) modes aredisabled.
32.2.10.2.2 CRC Insertion
IftheSOP buffer descriptor PASSCRC flagiscleared, theEMAC generates andappends a32-bit
Ethernet CRC onto thetransmitted data. FortheEMAC-generated CRC case, aCRC (orplaceholder) at
theendofthedata isallowed butnotrequired. The buffer byte count value should notinclude theCRC
bytes, ifthey arepresent.
IftheSOP buffer descriptor PASSCRC flagisset,then thelastfour bytes ofthetransmit data are
transmitted astheframe CRC. The four CRC data bytes should bethelastfour bytes oftheframe and
should beincluded inthebuffer byte count value. The MAC performs noerror checking ontheoutgoing
CRC.
32.2.10.2.3 Adaptive Performance Optimization (APO)
The EMAC incorporates adaptive performance optimization (APO) logic thatmay beenabled bysetting
theTXPACE bitintheMAC control register (MACCONTROL). Transmission pacing toenhance
performance isenabled when theTXPACE bitisset.Adaptive performance pacing introduces delays into
thenormal transmission offrames, delaying transmission attempts between stations, reducing the
probability ofcollisions occurring during heavy traffic (asindicated byframe deferrals andcollisions),
thereby, increasing thechance ofsuccessful transmission.
When aframe isdeferred, suffers asingle collision, multiple collisions, orexcessive collisions, thepacing
counter isloaded with aninitial value of31.When aframe istransmitted successfully (without
experiencing adeferral, single collision, multiple collision, orexcessive collision), thepacing counter is
decremented by1,down to0.
With pacing enabled, anew frame ispermitted toimmediately (after oneinterpacket gap) attempt
transmission only ifthepacing counter is0.Ifthepacing counter isnonzero, theframe isdelayed bythe
pacing delay ofapproximately four interpacket gap(IPG)delays. APO only affects theIPG preceding the
firstattempt attransmitting aframe; APO does notaffect theback-off algorithm forretransmitted frames.
32.2.10.2.4 Interpacket-Gap (IPG) Enforcement
The measurement reference fortheIPG of96bittimes ischanged depending onframe traffic conditions.
Ifaframe issuccessfully transmitted without collision andMII_CRS isdeasserted within approximately 48
bittimes ofMII_TXEN being deasserted, then 96bittimes ismeasured from MII_TXEN. Iftheframe
suffered acollision orMII_CRS isnotdeasserted until more than approximately 48bittimes after
MII_TXEN isdeasserted, then 96bittimes (approximately, butnotless) ismeasured from MII_CRS.
32.2.10.2.5 Back Off
The EMAC implements the802.3 binary exponential back-off algorithm.

<!-- Page 1838 -->

Architecture www.ti.com
1838 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.10.2.6 Transmit Flow Control
Incoming pause frames areacted upon, when enabled, toprevent theEMAC from transmitting anyfurther
frames. Incoming pause frames areonly acted upon when theFULLDUPLEX andTXFLOWEN bitsinthe
MAC control register (MACCONTROL) areset.Pause frames arenotacted upon inhalf-duplex mode.
Pause frame action istaken ifenabled, butnormally theframe isfiltered andnottransferred tomemory.
MAC control frames aretransferred tomemory, iftheRXCMFEN bitinthereceive
multicast/broadcast/promiscuous channel enable register (RXMBPENABLE) isset.The TXFLOWEN and
FULLDUPLEX bitsaffect whether ornotMAC control frames areacted upon, butthey have noaffect upon
whether ornotMAC control frames aretransferred tomemory orfiltered.
Pause frames areasubset ofMAC control frames with anopcode field of0001h. Incoming pause frames
areonly acted upon bytheEMAC if:
*TXFLOWEN bitissetinMACCONTROL
*The frame 'slength is64toRXMAXLEN bytes inclusive
*The frame contains noCRC error oralign/code errors
The pause time value from valid frames isextracted from thetwobytes following theopcode. The pause
time isloaded intotheEMAC transmit pause timer andthetransmit pause time period begins. Ifavalid
pause frame isreceived during thetransmit pause time period ofaprevious transmit pause frame then:
*Ifthedestination address isnotequal tothereserved multicast address oranyenabled ordisabled
unicast address, then thetransmit pause timer immediately expires, or
*Ifthenew pause time value is0,then thetransmit pause timer immediately expires, else
*The EMAC transmit pause timer immediately issettothenew pause frame pause time value. (Any
remaining pause time from theprevious pause frame isdiscarded).
IftheTXFLOWEN bitinMACCONTROL iscleared, then thepause timer immediately expires.
The EMAC does notstart thetransmission ofanew data frame anysooner than 512bit-times after a
pause frame with anonzero pause time hasfinished being received (MII_RXDV going inactive). No
transmission begins until thepause timer hasexpired (the EMAC may transmit pause frames inorder to
initiate outgoing flow control). Any frame already intransmission when apause frame isreceived is
completed andunaffected.
Incoming pause frames consist of:
*A48-bit destination address equal tooneofthefollowing:
-The reserved multicast destination address 01.80.C2.00.00.01h
-Any EMAC 48-bit unicast address. Pause frames areaccepted, regardless ofwhether thechannel
isenabled ornot.
*The 16-bit length/type field containing thevalue 88.08h.
*The 48-bit source address ofthetransmitting device.
*The 16-bit pause opcode equal to00.01h.
*The 16-bit pause time. Apause-quantum is512bit-times.
*Padding to64-byte data length.
*The 32-bit frame-check sequence (CRC word).
Allquantities arehexadecimal andaretransmitted most-significant byte first. The least-significant bit(LSB)
istransferred firstineach byte.
The padding isrequired tomake uptheframe toaminimum of64bytes. The standard allows pause
frames longer than 64bytes tobediscarded orinterpreted asvalid pause frames. The EMAC recognizes
anypause frame between 64bytes andRXMAXLEN bytes inlength.
32.2.10.2.7 Speed, Duplex, andPause Frame Support
The MAC operates at10Mbps or100Mbps, inhalf-duplex orfull-duplex mode, andwith orwithout pause
frame support asconfigured bythehost.

<!-- Page 1839 -->

www.ti.com Architecture
1839 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.11 Packet Receive Operation
32.2.11.1 Receive DMA Host Configuration
Toconfigure thereceive DMA foroperation thehost must:
*Initialize thereceive addresses.
*Initialize thereceive channel nDMA head descriptor pointer registers (RXnHDP) to0.
*Write theMAC address hash nregisters (MACHASH1 andMACHASH2), ifmulticast addressing is
desired.
*Ifflow control istobeenabled, initialize:
-thereceive channel nfree buffer count registers (RXnFREEBUFFER)
-thereceive channel nflow control threshold register (RXnFLOWTHRESH)
-thereceive filter lowpriority frame threshold register (RXFILTERLOWTHRESH)
*Enable thedesired receive interrupts using thereceive interrupt mask setregister (RXINTMASKSET)
andthereceive interrupt mask clear register (RXINTMASKCLEAR).
*Settheappropriate configuration bitsintheMAC control register (MACCONTROL).
*Write thereceive buffer offset register (RXBUFFEROFFSET) value (typically 0).
*Setup thereceive channel(s) buffer descriptors andinitialize RXnHDP.
*Enable thereceive DMA controller bysetting theRXEN bitinthereceive control register
(RXCONTROL).
*Configure andenable thereceive operation, asdesired, inthereceive
multicast/broadcast/promiscuous channel enable register (RXMBPENABLE) andbyusing thereceive
unicast setregister (RXUNICASTSET) andthereceive unicast clear register (RXUNICASTCLEAR).
32.2.11.2 Receive Channel Enabling
Each oftheeight receive channels hasanenable bit(RXCH nEN) inthereceive unicast setregister
(RXUNICASTSET) thatiscontrolled using RXUNICASTSET andthereceive unicast clear register
(RXUNICASTCLEAR). The RXCH nENbitsdetermine whether thegiven channel isenabled (when setto
1)toreceive frames with amatching unicast ormulticast destination address.
The RXBROADEN bitinthereceive multicast/broadcast/promiscuous channel enable register
(RXMBPENABLE) determines ifbroadcast frames areenabled orfiltered. Ifbroadcast frames areenabled
(when setto1),then they arecopied toonly asingle channel selected bytheRXBROADCH bitin
RXMBPENABLE.
The RXMULTEN bitinRXMBPENABLE determines ifhash matching multicast frames areenabled or
filtered. Incoming multicast addresses (group addresses) arehashed intoanindex inthehash table. Ifthe
indexed bitissetthen theframe hash matches andwillbetransferred tothechannel selected bythe
RXMULTCH bitinRXMBPENABLE when multicast frames areenabled. The multicast hash bitsaresetin
theMAC address hash nregisters (MACHASH1 andMACHASH2).
The RXPROMCH bitinRXMBPENABLE selects thepromiscuous channel toreceive frames selected by
theRXCMFEN, RXCSFEN, RXCEFEN, andRXCAFEN bits. These four bitsallow reception ofMAC
control frames, short frames, error frames, andallframes (promiscuous), respectively.
32.2.11.3 Receive Address Matching
Alleight MAC addresses corresponding totheeight receive channels share theupper 40bits. Only the
lower byte isunique foreach address. Alleight receive addresses should beinitialized, because pause
frames areacted upon regardless ofwhether achannel isenabled ornot.
AMAC address iswritten byfirstwriting theaddress number (channel) tobewritten intotheMAC index
register (MACINDEX). The upper 32bitsofaddress arethen written totheMAC address high bytes
register (MACADDRHI), which isfollowed bywriting thelower 16bitsofaddress totheMAC address low
bytes register (MACADDRLO). Since alleight MAC addresses share theupper 40bitsofaddress,
MACADDRHI needs tobewritten only thefirsttime (forthefirstchannel configured).

<!-- Page 1840 -->

Architecture www.ti.com
1840 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.11.4 Hardware Receive QOS Support
Hardware receive quality ofservice (QOS) issupported, when enabled, bytheTag Protocol Identifier
format andtheassociated Tag Control Information (TCI) format priority field. When theincoming frame
length/type value isequal to81.00h, theEMAC recognizes theframe asanEthernet Encoded Tag
Protocol Type. The twooctets immediately following theprotocol type contain the16-bit TCIfield. Bits15-
13oftheTCIfield contain thereceived frames priority (0to7).The received frame isalow-priority frame,
ifthepriority value is0to3;thereceived frame isahigh-priority frame, ifthepriority value is4to7.All
frames thathave alength/type field value notequal to81.00h arelow-priority frames. Received frames
thatcontain priority information aredetermined bytheEMAC as:
*A48-bit (6bytes) destination address equal to:
-The destination station's individual unicast address.
-The destination station's multicast address (MACHASH1 andMACHASH2).
-The broadcast address ofallones.
*A48-byte (6bytes) source address.
*The 16-bit (2bytes) length/type field containing thevalue 81.00h.
*The 16-bit (2bytes) TCIfield with thepriority field intheupper 3bits.
*Data bytes
*The 4bytes CRC.
The receive filter lowpriority frame threshold register (RXFILTERLOWTHRESH) andthereceive channel
nfree buffer count registers (RXnFREEBUFFER) areused inconjunction with thepriority information to
implement receive hardware QOS. Low-priority frames arefiltered ifthenumber offree buffers
(RXnFREEBUFFER) fortheframe channel isless than orequal tothefilter lowthreshold
(RXFILTERLOWTHRESH) value. Hardware QOS isenabled bytheRXQOSEN bitinthereceive
multicast/broadcast/promiscuous channel enable register (RXMBPENABLE).
32.2.11.5 Host Free Buffer Tracking
The host must track free buffers foreach enabled channel (including unicast, multicast, broadcast, and
promiscuous), ifreceive QOS orreceive flow control isused. Disabled channel free buffer values aredo
notcares. During initialization, thehost should write thenumber offree buffers foreach enabled channel
totheappropriate receive channel nfree buffer count registers (RXnFREEBUFFER). The EMAC
decrements theappropriate channel 'sfree buffer value foreach buffer used. When thehost reclaims the
frame buffers, thehost should write thechannel free buffer register with thenumber ofreclaimed buffers
(write toincrement). There areamaximum of65,535 free buffers available. RXnFREEBUFFER only
needs tobeupdated bythehost ifreceive QOS orflow control isused.
32.2.11.6 Receive Channel Teardown
The host commands areceive channel teardown bywriting thechannel number tothereceive teardown
register (RXTEARDOWN). When ateardown command isissued toanenabled receive channel, the
following occurs:
*Any current frame inreception completes normally.
*The TDOWNCMPLT flagissetinthenext buffer descriptor inthechain, ifthere isone.
*The channel head descriptor pointer iscleared to0.
*Areceive interrupt forthechannel isissued tothehost.
*The corresponding receive channel ncompletion pointer register (RXnCP) contains thevalue FFFF
FFCh.
Channel teardown may becommanded onanychannel atanytime. The host isinformed oftheteardown
completion bythesetteardown complete (TDOWNCMPLT) buffer descriptor bit.The EMAC does not
clear anychannel enables duetoateardown command. Ateardown command toaninactive channel
issues aninterrupt thatsoftware should acknowledge with anFFFF FFFCh acknowledge value toRXnCP
(note thatthere isnobuffer descriptor inthiscase). Software may read RXnCPtodetermine ifthe
interrupt was duetoacommanded teardown. The read value isFFFF FFFCh, iftheinterrupt was duetoa
teardown command.

<!-- Page 1841 -->

www.ti.com Architecture
1841 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.11.7 Receive Frame Classification
Received frames areproper (good) frames, ifthey arebetween 64bytes andthevalue inthereceive
maximum length register (RXMAXLEN) bytes inlength (inclusive) andcontain nocode, align, orCRC
errors.
Received frames arelong frames, iftheir frame count exceeds thevalue inRXMAXLEN. The RXMAXLEN
reset (default) value is5EEh (1518 indecimal). Long received frames areeither oversized orjabber
frames. Long frames with noerrors areoversized frames; long frames with CRC, code, oralignment
errors arejabber frames.
Received frames areshort frames, iftheir frame count isless than 64bytes. Short frames thataddress
match andcontain noerrors areundersized frames; short frames with CRC, code, oralignment errors are
fragment frames. Iftheframe length isless than orequal to20,then theframe CRC ispassed, regardless
ofwhether theRXPASSCRC bitissetorcleared inthereceive multicast/broadcast/promiscuous channel
enable register (RXMBPENABLE).
Areceived long packet always contains RXMAXLEN number ofbytes transferred tomemory (ifthe
RXCEFEN bitissetinRXMBPENABLE), regardless ofthevalue oftheRXPASSCRC bit.Following isan
example with RXMAXLEN setto1518:
*Iftheframe length is1518, then thepacket isnotalong packet andthere are1514 or1518 bytes
transferred tomemory depending onthevalue oftheRXPASSCRC bit.
*Iftheframe length is1519, there are1518 bytes transferred tomemory regardless ofthe
RXPASSCRC bitvalue. The lastthree bytes arethefirstthree CRC bytes.
*Iftheframe length is1520, there are1518 bytes transferred tomemory regardless ofthe
RXPASSCRC bitvalue. The lasttwobytes arethefirsttwoCRC bytes.
*Iftheframe length is1521, there are1518 bytes transferred tomemory regardless ofthe
RXPASSCRC bitvalue. The lastbyte isthefirstCRC byte.
*Iftheframe length is1522, there are1518 bytes transferred tomemory. The lastbyte isthelastdata
byte.
32.2.11.8 Promiscuous Receive Mode
When thepromiscuous receive mode isenabled bysetting theRXCAFEN bitinthereceive
multicast/broadcast/promiscuous channel enable register (RXMBPENABLE), nonaddress matching frames
thatwould normally befiltered aretransferred tothepromiscuous channel. Address matching frames that
would normally befiltered duetoerrors aretransferred totheaddress match channel when theRXCAFEN
andRXCEFEN bitsinRXMBPENABLE areset.Aframe isconsidered tobeanaddress matching frame
only ifitisenabled tobereceived onaunicast, multicast, orbroadcast channel. Frames received to
disabled unicast, multicast, orbroadcast channels areconsidered nonaddress matching.
MAC control frames address match only iftheRXCMFEN bitinRXMBPENABLE isset.The RXCEFEN
andRXCSFEN bitsinRXMBPENABLE determine whether error frames aretransferred tomemory ornot,
butthey donotdetermine whether error frames areaddress matching ornot.Short frames areaspecial
type oferror frames.
Asingle channel isselected asthepromiscuous channel bytheRXPROMCH bitinRXMBPENABLE. The
promiscuous receive mode isenabled bytheRXCMFEN, RXCEFEN, RXCSFEN, andRXCAFEN bitsin
RXMBPENABLE. Table 32-7 shows theeffects ofthepromiscuous enable bits. Proper frames areframes
thatarebetween 64bytes andthevalue inthereceive maximum length register (RXMAXLEN) bytes in
length inclusive andcontain nocode, align, orCRC errors.

<!-- Page 1842 -->

Architecture www.ti.com
1842 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-7. Receive Frame Treatment Summary
Address Match RXCAFEN RXCEFEN RXCMFEN RXCSFEN Receive Frame Treatment
0 0 X X X Noframes transferred.
0 1 0 0 0 Proper frames transferred topromiscuous channel.
0 1 0 0 1 Proper/undersized data frames transferred to
promiscuous channel.
0 1 0 1 0 Proper data andcontrol frames transferred to
promiscuous channel.
0 1 0 1 1 Proper/undersized data andcontrol frames
transferred topromiscuous channel.
0 1 1 0 0 Proper/oversize/jabber/code/align/CRC data frames
transferred topromiscuous channel. Nocontrol or
undersized/fragment frames aretransferred.
0 1 1 0 1 Proper/undersized/fragment/oversize/jabber/code/
align/CRC data frames transferred topromiscuous
channel. Nocontrol frames aretransferred.
0 1 1 1 0 Proper/oversize/jabber/code/align/CRC data and
control frames transferred topromiscuous channel. No
undersized frames aretransferred.
0 1 1 1 1 Allnonaddress matching frames with andwithout
errors transferred topromiscuous channel.
1 X 0 0 0 Proper data frames transferred toaddress match
channel.
1 X 0 0 1 Proper/undersized data frames transferred
toaddress match channel.
1 X 0 1 0 Proper data andcontrol frames transferred toaddress
match channel.
1 X 0 1 1 Proper/undersized data andcontrol frames
transferred toaddress match channel.
1 X 1 0 0 Proper/oversize/jabber/code/align/CRC data frames
transferred toaddress match channel. Nocontrol or
undersized frames aretransferred.
1 X 1 0 1 Proper/oversize/jabber/fragment/undersized/code/
align/CRC data frames transferred toaddress match
channel. Nocontrol frames aretransferred.
1 X 1 1 0 Proper/oversize/jabber/code/align/CRC data and
control frames transferred toaddress match
channel. Noundersized/fragment frames are
transferred.
1 X 1 1 1 Alladdress matching frames with andwithout errors
transferred totheaddress match channel

<!-- Page 1843 -->

www.ti.com Architecture
1843 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.11.9 Receive Overrun
The types ofreceive overrun are:
*FIFO start offrame overrun (FIFO_SOF)
*FIFO middle offrame overrun (FIFO_MOF)
*DMA start offrame overrun (DMA_SOF)
*DMA middle offrame overrun (DMA_MOF)
The statistics counters used totrack these types ofreceive overrun are:
*Receive start offrame overruns register (RXSOFOVERRUNS)
*Receive middle offrame overruns register (RXMOFOVERRUNS)
*Receive DMA overruns register (RXDMAOVERRUNS)
Start offrame overruns happen when there arenoresources available when frame reception begins. Start
offrame overruns increment theappropriate overrun statistic(s) andtheframe isfiltered.
Middle offrame overruns happen when there aresome resources tostart theframe reception, butthe
resources runoutduring frame reception. Innormal operation, aframe thatoverruns after starting the
frame reception isfiltered andtheappropriate statistic(s) areincremented; however, theRXCEFEN bitin
thereceive multicast/broadcast/promiscuous channel enable register (RXMBPENABLE) affects overrun
frame treatment. Table 32-8 shows how theoverrun condition ishandled forthemiddle offrame overrun.
Table 32-8. Middle ofFrame Overrun Treatment
Address Match RXCAFEN RXCEFEN Middle ofFrame Overrun Treatment
0 0 X Overrun frame filtered.
0 1 0 Overrun frame filtered.
0 1 1 Asmuch frame data aspossible istransferred tothepromiscuous channel
until overrun. The appropriate overrun statistic(s) isincremented andthe
OVERRUN andNOMATCH flags aresetintheSOP buffer descriptor. Note
thattheRXMAXLEN number ofbytes cannot bereached foranoverrun to
occur (itwould betruncated andbeajabber oroversize).
1 X 0 Overrun frame filtered with theappropriate overrun statistic(s) incremented.
1 X 1 Asmuch frame data aspossible istransferred totheaddress match
channel until overrun. The appropriate overrun statistic(s) isincremented
andtheOVERRUN flagissetintheSOP buffer descriptor. Note thatthe
RXMAXLEN number ofbytes cannot bereached foranoverrun tooccur (it
would betruncated).

<!-- Page 1844 -->

Architecture www.ti.com
1844 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.12 Packet Transmit Operation
The transmit DMA isaneight channel interface. Priority between theeight queues may beeither fixed or
round-robin asselected bytheTXPTYPE bitintheMAC control register (MACCONTROL). Ifthepriority
type isfixed, then channel 7hasthehighest priority andchannel 0hasthelowest priority. Round-robin
priority proceeds from channel 0tochannel 7.
32.2.12.1 Transmit DMA Host Configuration
Toconfigure thetransmit DMA foroperation thehost must perform:
*Write theMAC source address lowbytes register (MACSRCADDRLO) andtheMAC source address
high bytes register (MACSRCADDRHI) (used forpause frames ontransmit).
*Initialize thetransmit channel nDMA head descriptor pointer registers (TXnHDP) to0.
*Enable thedesired transmit interrupts using thetransmit interrupt mask setregister (TXINTMASKSET)
andthetransmit interrupt mask clear register (TXINTMASKCLEAR).
*Settheappropriate configuration bitsintheMAC control register (MACCONTROL).
*Setup thetransmit channel(s) buffer descriptors inhost memory.
*Enable thetransmit DMA controller bysetting theTXEN bitinthetransmit control register
(TXCONTROL).
*Write theappropriate TXnHDP with thepointer tothefirstdescriptor tostart transmit operations.
32.2.12.2 Transmit Channel Teardown
The host commands atransmit channel teardown bywriting thechannel number tothetransmit teardown
register (TXTEARDOWN). When ateardown command isissued toanenabled transmit channel, the
following occurs:
*Any frame currently intransmission completes normally.
*The TDOWNCMPLT flagissetinthenext SOP buffer descriptor inthechain, ifthere isone.
*The channel head descriptor pointer iscleared to0.
*Atransmit interrupt isissued toinform thehost ofthechannel teardown.
*The corresponding transmit channel ncompletion pointer register (TXnCP) contains thevalue
FFFF FFFCh.
*The host should acknowledge ateardown interrupt with anFFFF FFFCh acknowledge value.
Channel teardown may becommanded onanychannel atanytime. The host isinformed oftheteardown
completion bythesetteardown complete (TDOWNCMPLT) buffer descriptor bit.The EMAC does not
clear anychannel enables duetoateardown command. Ateardown command toaninactive channel
issues aninterrupt thatsoftware should acknowledge with anFFFF FFFCh acknowledge value toTXnCP
(note thatthere isnobuffer descriptor inthiscase). Software may read theinterrupt acknowledge location
(TXnCP) todetermine iftheinterrupt was duetoacommanded teardown. The read value isFFFF FFFCh,
iftheinterrupt was duetoateardown command.

<!-- Page 1845 -->

www.ti.com Architecture
1845 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.13 Receive andTransmit Latency
The transmit andreceive FIFOs each contain three 64-byte cells. The EMAC begins transmission ofa
packet onthewire after TXCELLTHRESH (configurable through theFIFO control register) cells, ora
complete packet, areavailable intheFIFO.
Transmit underrun cannot occur forpacket sizes ofTXCELLTHRESH times 64bytes (orless). Forlarger
packet sizes, transmit underrun occurs ifthememory latency isgreater than thetime required totransmit
a64-byte cellonthewire; thisis5.12μsin100Mbps mode and51.2μsin10Mbps mode. The memory
latency time includes allbuffer descriptor reads fortheentire celldata.
Receive overrun isprevented ifthereceive memory celllatency isless than thetime required totransmit a
64-byte cellonthewire: 5.12μsin100Mbps mode, or51.2μsin10Mbps mode. The latency time
includes anyrequired buffer descriptor reads forthecelldata.
Latency tosystem 'sinternal andexternal RAM canbecontrolled through theuseofthetransfer node
priority allocation register available atthedevice level. Latency todescriptor RAM islowbecause RAM is
local totheEMAC, asitispart oftheEMAC control module.
32.2.14 Transfer Node Priority
The device contains achip-level master priority register thatisused tosetthepriority ofthetransfer node
used inissuing memory transfer requests tosystem memory.
Although theEMAC hasinternal FIFOs tohelp alleviate memory transfer arbitration problems, theaverage
transfer rate ofdata read andwritten bytheEMAC tointernal orexternal processor memory must beat
least thatoftheEthernet wire rate. Inaddition, theinternal FIFO system cannotwithstand asingle
memory latency event greater than thetime ittakes tofillorempty aTXCELLTHRESH number ofinternal
64byte FIFO cells.
For100Mbps operation, these restrictions translate intothefollowing rules:
*The short-term average, each 64-byte memory read/write request from theEMAC must beserviced in
nomore than 5.12μs.
*Any single latency event inrequest servicing canbenolonger than (5.12 ×TXCELLTHRESH) μs.

<!-- Page 1846 -->

Architecture www.ti.com
1846 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.15 Reset Considerations
32.2.15.1 Software Reset Considerations
The peripheral clock iscontrolled bytheGlobal Clock Module (GCM), while thereset totheEMAC, MDIO
andEMAC control module iscontrolled bythesystem module. See the"Architecture" chapter ofthe
Technical Reference Manual formore onhow toenable ordisable theperipheral clock totheEMAC,
MDIO andEMAC control module. Formore onhow theEMAC, MDIO, andEMAC control module are
disabled orplaced inreset atruntime, seeSection 32.2.18 .
Within theperipheral itself, theEMAC component oftheEthernet MAC peripheral canbeplaced inareset
state bywriting tothesoftreset register (SOFTRESET). Writing a1totheSOFTRESET bitcauses the
EMAC logic tobereset andtheregister values tobesettotheir default values. Software reset occurs
when thereceive andtransmit DMA controllers areinanidlestate toavoid locking uptheconfiguration
bus; itistheresponsibility ofthesoftware toverify thatthere arenopending frames tobetransferred.
After writing a1totheSOFTRESET bit,itmay bepolled todetermine ifthereset hasoccurred. Ifa1is
read, thereset hasnotyetoccurred; ifa0isread, then areset hasoccurred.
After asoftware reset operation, alltheEMAC registers need tobereinitialized forproper data
transmission, including theFULLDUPLEX bitsetting intheMAC control register (MACCONTROL).
Unlike theEMAC module, theMDIO andEMAC control modules cannot beplaced inreset from aregister
inside their memory map.
32.2.15.2 Hardware Reset Considerations
When ahardware reset occurs, theEMAC peripheral hasitsregister values reset andallthecomponents
return totheir default state. After thehardware reset, theEMAC needs tobeinitialized before being able
toresume itsdata transmission, asdescribed inSection 32.2.16 .
Ahardware reset istheonly means ofrecovering from theerror interrupts (HOSTPEND), which are
triggered byerrors inpacket buffer descriptors. Before doing ahardware reset, youshould inspect the
error codes intheMAC status register (MACSTATUS) thatgives information about thetype ofsoftware
error thatneeds tobecorrected. Fordetailed information onerror interrupts, seeSection 32.2.17.1.4 .

<!-- Page 1847 -->

www.ti.com Architecture
1847 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.16 Initialization
32.2.16.1 Enabling theEMAC/MDIO Peripheral
When thedevice ispowered on,theEMAC peripheral becomes enabled assoon asthesystem reset is
released, andtheEMAC peripheral registers aresettotheir default values. The application software can
configure theEMAC peripheral registers asrequired.
32.2.16.2 EMAC Control Module Initialization
The EMAC control module isused forglobal interrupt enables andtopace interrupts using 1ms time
windows. There isalso an8Kblock ofCPPI RAM local totheEMAC thatisused tohold packet buffer
descriptors.
Note thatalthough theEMAC control module andtheEMAC module have slightly different functions, in
practice, thetype ofmaintenance performed ontheEMAC control module ismore commonly conducted
from theEMAC module software (asopposed totheMDIO module).
The initialization oftheEMAC control module consists oftwoparts:
1.Configuration oftheinterrupt totheCPU.
2.Initialization oftheEMAC control module:
*Setting theinterrupt pace counts using theEMAC control module registers INTCONTROL,
C0RXIMAX, andC0TXIMAX
*Initializing theEMAC andMDIO modules
*Enabling interrupts intheEMAC control module using theEMAC control module interrupt control
registers C0RXTHRESHEN, C0RXEN, C0TXEN, andC0MISCEN.
The process ofmapping theEMAC interrupts totheCPU isdone through theVectored Interrupt Manager
(VIM). Once theinterrupt ismapped toaCPU interrupt, general masking andunmasking ofinterrupts (to
control reentrancy) should bedone atthechip level bymanipulating theinterrupt core enable mask
registers.
32.2.16.3 MDIO Module Initialization
The MDIO module isused toinitially configure andmonitor oneormore external PHY devices. Other than
initializing thesoftware state machine (details onthisstate machine canbefound intheIEEE 802.3
standard), allthatneeds tobedone fortheMDIO module istoenable theMDIO engine andtoconfigure
theclock divider. Tosettheclock divider, supply anMDIO clock of1MHz. Forexample, iftheperipheral
clock is50MHz, thedivider canbesetto50.
Both thestate machine enable andtheMDIO clock divider arecontrolled through theMDIO control
register (CONTROL). Ifnone ofthepotentially connected PHYs require theaccess preamble, the
PREAMBLE bitinCONTROL canalso besettospeed upPHY register access.
IftheMDIO module istooperate onaninterrupt basis, theinterrupts canbeenabled atthistime using the
MDIO user command complete interrupt mask setregister (USERINTMASKSET) forregister access and
theMDIO user PHY select register (USERPHYSEL n)ifatarget PHY isalready known.
Once theMDIO state machine hasbeen initialized andenabled, itstarts polling all32PHY addresses on
theMDIO bus, looking foranactive PHY. Since itcantake upto50μstoread oneregister, itcanbe
some time before theMDIO module provides anaccurate representation ofwhether aPHY isavailable.
Also, aPHY cantake upto3seconds tonegotiate alink. Thus, itisadvisable toruntheMDIO software
offatime-based event rather than polling.
Formore information onPHY control registers, seeyour PHY device documentation.

<!-- Page 1848 -->

Architecture www.ti.com
1848 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.16.4 EMAC Module Initialization
The EMAC module isused tosend andreceive data packets over thenetwork. This isdone by
maintaining uptoeight transmit andreceive descriptor queues. The EMAC module configuration must
also bekept up-to-date based onPHY negotiation results returned from theMDIO module. Most ofthe
work indeveloping anapplication ordevice driver forEthernet isprogramming thismodule.
The following istheinitialization procedure adevice driver would follow togettheEMAC tothestate
where itisready toreceive andsend Ethernet packets. Some ofthese steps arenotnecessary when
performed immediately after device reset.
1.Ifenabled, clear thedevice interrupt enable bitsintheEMAC control module interrupt control registers
C0RXTHRESHEN, C0RXEN, C0TXEN, andC0MISCEN.
2.Clear theMAC control register (MACCONTROL), receive control register (RXCONTROL), andtransmit
control register (TXCONTROL) (not necessary immediately after reset).
3.Initialize all16header descriptor pointer registers (RXnHDP andTXnHDP) to0.
4.Clear all36statistics registers bywriting 0(not necessary immediately after reset).
5.Setup thelocal Ethernet MAC address byprogramming theMAC index register (MACINDEX), MAC
address high bytes register (MACADDRHI), andMAC address lowbytes register (MACADDRLO). Be
sure toprogram alleight MAC address registers -whether thereceive channel istobeenabled ornot.
Duplicate thesame MAC address across allunused channels. When using more than onereceive
channel, start with channel 0andprogress upwards.
6.Ifbuffer flow control istobeenabled, initialize thereceive channel nfree buffer count registers
(RXnFREEBUFFER), receive channel nflow control threshold register (RXnFLOWTHRESH), and
receive filter lowpriority frame threshold register (RXFILTERLOWTHRESH).
7.Most device drivers open with nomulticast addresses, soclear theMAC address hash registers
(MACHASH1 andMACHASH2) to0.
8.Write thereceive buffer offset register (RXBUFFEROFFSET) value (typically 0).
9.Initially clear allunicast channels bywriting FFh tothereceive unicast clear register
(RXUNICASTCLEAR). Ifunicast isdesired, itcanbeenabled now bywriting thereceive unicast set
register (RXUNICASTSET). Some drivers willdefault tounicast ondevice open while others willnot.
10.Setup thereceive multicast/broadcast/promiscuous channel enable register (RXMBPENABLE) with an
initial configuration. The configuration isbased onthecurrent receive filter settings ofthedevice driver.
Some drivers may enable things likebroadcast andmulticast packets immediately, while others may
not.
11.Settheappropriate configuration bitsinMACCONTROL (donotsettheGMIIEN bityet).
12.Clear allunused channel interrupt bitsbywriting thereceive interrupt mask clear register
(RXINTMASKCLEAR) andthetransmit interrupt mask clear register (TXINTMASKCLEAR).
13.Enable thereceive andtransmit channel interrupt bitsinthereceive interrupt mask setregister
(RXINTMASKSET) andthetransmit interrupt mask setregister (TXINTMASKSET) forthechannels to
beused, andenable theHOSTMASK andSTATMASK bitsusing theMAC interrupt mask setregister
(MACINTMASKSET).
14.Initialize thereceive andtransmit descriptor listqueues.
15.Prepare receive bywriting apointer tothehead ofthereceive buffer descriptor listtoRXnHDP.
16.Enable thereceive andtransmit DMA controllers bysetting theRXEN bitinRXCONTROL andthe
TXEN bitinTXCONTROL. Then settheGMIIEN bitinMACCONTROL.
17.Enable thedevice interrupt inEMAC control module registers C0RXTHRESHEN, C0RXEN, C0TXEN,
andC0MISCEN.

<!-- Page 1849 -->

www.ti.com Architecture
1849 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.17 Interrupt Support
32.2.17.1 EMAC Module Interrupt Events andRequests
The EMAC module generates 26interrupt events:
*TXPEND n:Transmit packet completion interrupt fortransmit channels 0through 7
*RXPEND n:Receive packet completion interrupt forreceive channels 0through 7
*RXTHRESHPEND n:Receive packet completion interrupt forreceive channels 0through 7when flow
control isenabled andthenumber offree buffers isbelow thethreshold
*STATPEND: Statistics interrupt
*HOSTPEND: Host error interrupt
32.2.17.1.1 Transmit Packet Completion Interrupts
The transmit DMA engine haseight channels, with each channel having acorresponding interrupt
(TXPEND n).The transmit interrupts arelevel interrupts thatremain asserted until cleared bytheCPU.
Each oftheeight transmit channel interrupts may beindividually enabled bysetting theappropriate bitin
thetransmit interrupt mask setregister (TXINTMASKSET) to1.Each oftheeight transmit channel
interrupts may beindividually disabled byclearing theappropriate bitbywriting a1tothetransmit
interrupt mask clear register (TXINTMASKCLEAR). The rawandmasked transmit interrupt status may be
read byreading thetransmit interrupt status (unmasked) register (TXINTSTATRAW) andthetransmit
interrupt status (masked) register (TXINTSTATMASKED), respectively.
When theEMAC completes thetransmission ofapacket, theEMAC issues aninterrupt totheCPU (via
theEMAC control module) when itwrites thepacket 'slastbuffer descriptor address totheappropriate
channel queue 'stransmit completion pointer located inthestate RAM block. The interrupt isgenerated by
thewrite when enabled bytheinterrupt mask, regardless ofthevalue written.
Upon interrupt reception, theCPU processes oneormore packets from thebuffer chain andthen
acknowledges aninterrupt bywriting theaddress ofthelastbuffer descriptor processed tothequeue 's
associated transmit completion pointer inthetransmit DMA state RAM.
The data written bythehost (buffer descriptor address ofthelastprocessed buffer) iscompared tothe
data intheregister written bytheEMAC port (address oflastbuffer descriptor used bytheEMAC). Ifthe
twovalues arenotequal (which means thattheEMAC hastransmitted more packets than theCPU has
processed interrupts for), thetransmit packet completion interrupt signal remains asserted. Ifthetwo
values areequal (which means thatthehost hasprocessed allpackets thattheEMAC hastransferred),
thepending interrupt iscleared. The value thattheEMAC isexpecting isfound byreading thetransmit
channel ncompletion pointer register (TXnCP).
The EMAC write tothecompletion pointer actually stores thevalue inthestate RAM. The CPU written
value does notactually change theregister value. The host written value iscompared totheregister
content (which was written bytheEMAC) andifthetwovalues areequal then theinterrupt isremoved;
otherwise, theinterrupt remains asserted. The host may process multiple packets prior toacknowledging
aninterrupt, orthehost may acknowledge interrupts forevery packet.
The application software must acknowledge theEMAC control module after processing packets bywriting
theappropriate C0RX keytotheEMAC End-Of-Interrupt Vector register (MACEOIVECTOR). See
Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.1.2 Receive Packet Completion Interrupts
The receive DMA engine haseight channels, which each channel having acorresponding interrupt
(RXPEND n).The receive interrupts arelevel interrupts thatremain asserted until cleared bytheCPU.
Each oftheeight receive channel interrupts may beindividually enabled bysetting theappropriate bitin
thereceive interrupt mask setregister (RXINTMASKSET) to1.Each oftheeight receive channel
interrupts may beindividually disabled byclearing theappropriate bitbywriting a1inthereceive interrupt
mask clear register (RXINTMASKCLEAR). The rawandmasked receive interrupt status may beread by
reading thereceive interrupt status (unmasked) register (RXINTSTATRAW) andthereceive interrupt
status (masked) register (RXINTSTATMASKED), respectively.

<!-- Page 1850 -->

Architecture www.ti.com
1850 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleWhen theEMAC completes apacket reception, theEMAC issues aninterrupt totheCPU bywriting the
packet's lastbuffer descriptor address totheappropriate channel queue's receive completion pointer
located inthestate RAM block. The interrupt isgenerated bythewrite when enabled bytheinterrupt
mask, regardless ofthevalue written.
Upon interrupt reception, theCPU processes oneormore packets from thebuffer chain andthen
acknowledges oneormore interrupt(s) bywriting theaddress ofthelastbuffer descriptor processed tothe
queue's associated receive completion pointer inthereceive DMA state RAM.
The data written bythehost (buffer descriptor address ofthelastprocessed buffer) iscompared tothe
data intheregister written bytheEMAC (address oflastbuffer descriptor used bytheEMAC). Ifthetwo
values arenotequal (which means thattheEMAC hasreceived more packets than theCPU has
processed interrupts for), thereceive packet completion interrupt signal remains asserted. Ifthetwo
values areequal (which means thatthehost hasprocessed allpackets thattheEMAC hasreceived), the
pending interrupt isde-asserted. The value thattheEMAC isexpecting isfound byreading thereceive
channel ncompletion pointer register (RXnCP).
The EMAC write tothecompletion pointer actually stores thevalue inthestate RAM. The CPU written
value does notactually change theregister value. The host written value iscompared totheregister
content (which was written bytheEMAC) andifthetwovalues areequal then theinterrupt isremoved;
otherwise, theinterrupt remains asserted. The host may process multiple packets prior toacknowledging
aninterrupt, orthehost may acknowledge interrupts forevery packet.
The application software must acknowledge theEMAC control module after processing packets bywriting
theappropriate C0TX keytotheEMAC End-Of-Interrupt Vector register (MACEOIVECTOR). See
Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.1.3 Statistics Interrupt
The statistics level interrupt (STATPEND) isissued when anystatistics value isgreater than orequal to
8000 0000h, ifenabled bysetting theSTATMASK bitintheMAC interrupt mask setregister
(MACINTMASKSET) to1.The statistics interrupt isremoved bywriting todecrement anystatistics value
greater than 8000 0000h. Aslong asthemost-significant bitofanystatistics value isset,theinterrupt
remains asserted.
The application software must akcnowledge theEMAC control module after receiving statistics interrupts
bywriting theappropriate C0MISC keytotheEMAC End-Of-Interrupt Vector register (MACEOIVECTOR).
See Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.1.4 Host Error Interrupt
The host error interrupt (HOSTPEND) isissued, ifenabled, under error conditions dealing with the
handling ofbuffer descriptors, detected during transmit orreceive DMA transactions. The failure ofthe
software application tosupply properly formatted buffer descriptors results inthiserror. The error bitcan
only becleared byresetting theEMAC module inhardware.
The host error interrupt isenabled bysetting theHOSTMASK bitintheMAC interrupt mask setregister
(MACINTMASKSET) to1.The host error interrupt isdisabled byclearing theappropriate bitbywriting a1
intheMAC interrupt mask clear register (MACINTMASKCLEAR). The rawandmasked host error interrupt
status may beread byreading theMAC interrupt status (unmasked) register (MACINTSTATRAW) andthe
MAC interrupt status (masked) register (MACINTSTATMASKED), respectively.
The transmit host error conditions are:
*SOP error
*Ownership bitnotsetinSOP buffer
*Zero next buffer descriptor pointer with EOP
*Zero buffer pointer
*Zero buffer length
*Packet length error

<!-- Page 1851 -->

www.ti.com Architecture
1851 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleThe receive host error conditions are:
*Ownership bitnotsetininput buffer
*Zero buffer pointer
The application software must acknowledge theEMAC control module after receiving host error interrupts
bywriting theappropriate C0MISC keytotheEMAC End-Of-Interrupt Vector (MACEOIVECTOR). See
Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.1.5 Receive Threshold Interrupts
Each oftheeight receive channels have acorresponding receive threshold interrupt (RXnTHRESHPEND).
The receive threshold interrupts arelevel interrupts thatremain asserted until thetriggering condition is
cleared bythehost. Each oftheeight threshold interrupts may beindividually enabled bysetting to1the
appropriate bitintheRXINTMASKSET register. Each oftheeight channel interrupts may beindividually
disabled byclearing to0theappropriate bitbywriting a1inthereceive interrupt mask clear register
(RXINTMASKCLEAR). The rawandmasked interrupt receive interrupt status may beread byreading the
receive interrupt status (unmasked) register (RXINTSTATRAW) andthereceive interrupt status (masked)
register (RXINTSTATMASKED),respectively.
AnRXnTHRESHPEND interrupt bitisasserted when enabled andwhen thechannel 'sassociated free
buffer count (RXnFREEBUFFER) isless than orequal tothechannel 'sassociated flow control threshold
register (RXnFLOWTHRESH). The receive threshold interrupts usethesame free buffer count and
threshold logic asdoes flow control, buttheinterrupts areindependently enabled from flow control. The
threshold interrupts areintended togive thehost anindication thatresources arerunning lowfora
particular channel(s).
The applications software must acknowledge theEMAC control module after receiving threshold interrupts
bywriting theappropriate C0RXTHRESH keytotheEMAC End-Of-Interrupt Vector (MACEOIVECTOR).
See Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.2 MDIO Module Interrupt Events andRequests
The MDIO module generates twointerrupt events:
*LINKINT0: Serial interface linkchange interrupt. Indicates achange inthestate ofthePHY link
selected bytheUSERPHYSEL0 register
*USERINT0: Serial interface user command event complete interrupt selected bytheUSERACCESS0
register
32.2.17.2.1 Link Change Interrupt
The MDIO module asserts alinkchange interrupt (LINKINT0) ifthere isachange inthelinkstate ofthe
PHY corresponding totheaddress inthePHYADRMON bitintheMDIO register USERPHYSEL0, andif
theLINKINTENB bitisalso setinUSERPHYSEL0. This interrupt event isalso captured inthe
LINKINTRAW bitintheMDIO linkstatus change interrupt register (LINKINTRAW). LINKINTRAW bits0
and1correspond toUSERPHYSEL0 andUSERPHYSEL1, respectively.
When theinterrupt isenabled andgenerated, thecorresponding LINKINTMASKED bitisalso setinthe
MDIO linkstatus change interrupt register (LINKINTMASKED). The interrupt iscleared bywriting back the
same bittoLINKINTMASKED (write toclear).
The application software must acknowledge theEMAC control module after receiving MDIO interrupts by
writing theappropriate C0MISC keytotheEMAC End-Of-Interrupt Vector (MACEOIVECTOR). See
Section 32.5.12 fortheacknowledge keyvalues.

<!-- Page 1852 -->

Architecture www.ti.com
1852 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.17.2.2 User Access Completion Interrupt
When theGObitinoneoftheMDIO register USERACCESS0 transitions from 1to0(indicating
completion ofauser access) andthecorresponding USERINTMASKSET bitintheMDIO user command
complete interrupt mask setregister (USERINTMASKSET) corresponding toUSERACCESS0 isset,a
user access completion interrupt (USERINT) isasserted. This interrupt event isalso captured inthe
USERINTRAW bitintheMDIO user command complete interrupt register (USERINTRAW).
USERINTRAW bits0andbit1correspond toUSERACCESS0 andUSERACCESS1, respectively.
When theinterrupt isenabled andgenerated, thecorresponding USERINTMASKED bitisalso setinthe
MDIO user command complete interrupt register (USERINTMASKED). The interrupt iscleared bywriting
back thesame bittoUSERINTMASKED (write toclear).
The application software must acknowledge theEMAC control module after receiving MDIO interrupts by
writing theappropriate C0MISC keytotheEMAC End-Of-Interrupt Vector (MACEOIVECTOR). See
Section 32.5.12 fortheacknowledge keyvalues.
32.2.17.3 Proper Interrupt Processing
Alltheinterrupts signaled from theEMAC andMDIO modules arelevel driven, soifthey remain active,
their level remains constant; theCPU core may require edge- orpulse-triggered interrupts. Inorder to
properly convert thelevel-driven interrupt signal toanedge- orpulse-triggered signal, theapplication
software must make useoftheinterrupt control logic contained intheEMAC control module.
Section 32.2.7.2 discusses theinterrupt control contained intheEMAC control module. Forsafe interrupt
processing, upon entry totheISR, thesoftware application should disable interrupts using theEMAC
control module registers C0RXTHRESHEN, C0RXEN, C0TXEN, C0MISCEN, andthen reenable them
upon leaving theISR. Ifanyinterrupt signals areactive atthattime, thiscreates another rising edge on
theinterrupt signal going totheCPU interrupt controller, thus triggering another interrupt. The EMAC
control module also uses theEMAC control module registers INTCONTROL, C0TXIMAX, andC0RXIMAX
toimplement interrupt pacing. The application software must acknowledge theEMAC control module by
writing theappropriate keytotheEMAC End-Of-Interrupt Vector (MACEOIVECTOR). See Section 32.5.12
fortheacknowledge keyvalues.
32.2.17.4 Interrupt Multiplexing
The EMAC control module combines different interrupt signals from both theEMAC andMDIO modules
intofour interrupt signals (C0RXTHRESHPULSE, C0RXPULSE, C0TXPULSE, C0MISCPULSE) thatare
routed totheVectored Interrupt Manager (VIM). The VIM iscapable ofrelaying allfour interrupt signals to
theCPU.
When aninterrupt isgenerated, thereason fortheinterrupt canberead from theMAC input vector
register (MACINVECTOR) located intheEMAC memory map. MACINVECTOR combines thestatus ofthe
following 28interrupt signals: TXPEND n,RXPEND n,RXTHRESHPEND n,STATPEND, HOSTPEND,
LINKINT0, andUSERINT0.
Formore details ontheinterrupt mapping, seeyour device-specific datasheet andVIM chapter ofthe
Technical Reference Manual.

<!-- Page 1853 -->

www.ti.com Architecture
1853 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.2.18 Power Management
Each ofthethree main components oftheEMAC peripheral canbeplaced inareduced-power mode to
conserve power during periods oflowactivity. The peripheral clock totheEMAC peripheral iscontrolled
bytheprocessor Global Clock Module (GCM). The GCM allows theapplication toenable ordisable the
peripheral clock totheEMAC peripheral.
The power conservation modes available foreach ofthethree components oftheEMAC/MDIO peripheral
are:
*Idle/Disabled state .This mode stops theclocks going totheperipheral, andprevents alltheregister
accesses. After reenabling theperipheral from thisidlestate, alltheregisters values prior tosetting
intothedisabled state arerestored, anddata transmission canproceed. Noreinitialization isrequired.
*System reset. The EMAC peripheral isreset bythesystem reset signal output from theSystem
module. Refer tothe"Architecture" chapter oftheTechnical Reference Manual toidentify thecauses of
asystem reset. Upon asystem reset, theregisters arereset totheir default value. When powering-up
after asystem reset, alltheEMAC submodules need tobereinitialized before anydata transmission
canhappen.
Formore information ontheuseoftheGCM, seeyour device-specific Technical Reference Manual.
32.2.19 Emulation Considerations
EMAC emulation control isimplemented forcompatibility with other peripherals. The SOFT andFREE bits
intheemulation control register (EMCONTROL) allow EMAC operation tobesuspended.
When theemulation suspend state isentered, theEMAC stops processing receive andtransmit frames at
thenext frame boundary. Any frame currently inreception ortransmission iscompleted normally without
suspension. Fortransmission, anycomplete orpartial frame inthetransmit cellFIFO istransmitted. For
receive, frames thataredetected bytheEMAC after thesuspend state isentered areignored. No
statistics arekept forignored frames.
Table 32-9 shows how theSOFT andFREE bitsaffect theoperation oftheemulation suspend.
NOTE: Emulation suspend hasnotbeen tested.
Table 32-9. Emulation Control
SOFT FREE Description
0 0 Normal operation
1 0 Emulation suspend
X 1 Normal operation

<!-- Page 1854 -->

EMAC Control Module Registers www.ti.com
1854 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3 EMAC Control Module Registers
Table 32-10 lists thememory-mapped registers fortheEMAC control module. The base address forthese
registers isFCF7 8800h.
Table 32-10. EMAC Control Module Registers
Offset Acronym Register Description Section
0h REVID EMAC Control Module Revision IDRegister Section 32.3.1
4h SOFTRESET EMAC Control Module Software Reset Register Section 32.3.2
Ch INTCONTROL EMAC Control Module Interrupt Control Register Section 32.3.3
10h C0RXTHRESHEN EMAC Control Module Receive Threshold Interrupt Enable Register Section 32.3.4
14h C0RXEN EMAC Control Module Receive Interrupt Enable Register Section 32.3.5
18h C0TXEN EMAC Control Module Transmit Interrupt Enable Register Section 32.3.6
1Ch C0MISCEN EMAC Control Module Miscellaneous Interrupt Enable Register Section 32.3.7
40h C0RXTHRESHSTAT EMAC Control Module Receive Threshold Interrupt Status Register Section 32.3.8
44h C0RXSTAT EMAC Control Module Receive Interrupt Status Register Section 32.3.9
48h C0TXSTAT EMAC Control Module Transmit Interrupt Status Register Section 32.3.10
4Ch C0MISCSTAT EMAC Control Module Miscellaneous Interrupt Status Register Section 32.3.11
70h C0RXIMAX EMAC Control Module Receive Interrupts PerMillisecond Register Section 32.3.12
74h C0TXIMAX EMAC Control Module Transmit Interrupts PerMillisecond Register Section 32.3.13

<!-- Page 1855 -->

www.ti.com EMAC Control Module Registers
1855 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.1 EMAC Control Module Revision IDRegister (REVID)
The EMAC control module revision IDregister (REVID) isshown inFigure 32-15 anddescribed in
Table 32-11 .
Figure 32-15. EMAC Control Module Revision IDRegister (REVID) (offset =00h)
31 0
REV
R-4EC8 0100h
LEGEND: R=Read only; -n=value after reset
Table 32-11. EMAC Control Module Revision IDRegister (REVID) Field Descriptions
Bit Field Value Description
31-0 REV Identifies theEMAC Control Module revision.
4EC8 0100h Current revision oftheEMAC Control Module.
32.3.2 EMAC Control Module Software Reset Register (SOFTRESET)
The EMAC Control Module Software Reset Register (SOFTRESET) isshown inFigure 32-16 and
described inTable 32-12 .
Figure 32-16. EMAC Control Module Software Reset Register (SOFTRESET) (offset =04h)
31 16
Reserved
R-0
15 1 0
Reserved RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-12. EMAC Control Module Software Reset Register (SOFTRESET)
Bit Field Value Description
31-1 Reserved 0 Reserved
0 RESET Software reset bitfortheEMAC Control Module. Clears theinterrupt status, control registers, andCPPI
Ram ontheclock cycle following awrite of1.
0 Nosoftware reset.
1 Perform asoftware reset.

<!-- Page 1856 -->

EMAC Control Module Registers www.ti.com
1856 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.3 EMAC Control Module Interrupt Control Register (INTCONTROL)
The EMAC control module interrupt control register (INTCONTROL) isshown inFigure 32-17 and
described inTable 32-13 .The settings intheINTCONTROL register areused inconjunction with the
CnRXIMAX andCnTXIMAX registers.
Figure 32-17. EMAC Control Module Interrupt Control Register (INTCONTROL) (offset =0Ch)
31 24
Reserved
R-0
23 18 17 16
Reserved C0TXPACEEN C0RXPACEEN
R-0 R/W-0 R/W-0
15 12 11 0
Reserved INTPRESCALE
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-13. EMAC Control Module Interrupt Control Register (INTCONTROL)
Bit Field Value Description
31-18 Reserved 0 Reserved
17 C0TXPACEEN Enable pacing forTXinterrupt pulse generation.
0 Pacing forTXinterrupts isdisabled.
1 Pacing forTXinterrupts isenabled.
16 C0RXPACEEN Enable pacing forRXinterrupt pulse generation.
0 Pacing forRXinterrupts isdisabled.
1 Pacing forRXinterrupts isenabled.
15-12 Reserved 0 Reserved
11-0 INTPRESCALE 0-7FFh Number ofinternal EMAC module reference clock periods within a4μstime window (see
your device-specific data manual forinformation).

<!-- Page 1857 -->

www.ti.com EMAC Control Module Registers
1857 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.4 EMAC Control Module Receive Threshold Interrupt Enable Registers (C0RXTHRESHEN)
The EMAC control module receive threshold interrupt enable register (C0RXTHRESHEN) isshown in
Figure 32-18 anddescribed inTable 32-14 .
Figure 32-18. EMAC Control Module Receive Threshold Interrupt Enable Register
(C0RXTHRESHEN) (offset =10h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7
THRESHENRXCH6
THRESHENRXCH5
THRESHENRXCH4
THRESHENRXCH3
THRESHENRXCH2
THRESHENRXCH1
THRESHENRXCH0
THRESHEN
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-14. EMAC Control Module Receive Threshold Interrupt Enable Register (C0RXTHRESHEN)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 7.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 7.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 7.
6 RXCH6THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 6.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 6.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 6.
5 RXCH5THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 5.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 5.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 5.
4 RXCH4THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 4.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 4.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 4.
3 RXCH3THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 3.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 3.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 3.
2 RXCH2THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 2.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 2.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 2.
1 RXCH1THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 1.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 1.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 1.
0 RXCH0THRESHEN Enable C0RXTHRESHPULSE interrupt generation forRXChannel 0.
0 C0RXTHRESHPULSE generation isdisabled forRXChannel 0.
1 C0RXTHRESHPULSE generation isenabled forRXChannel 0.

<!-- Page 1858 -->

EMAC Control Module Registers www.ti.com
1858 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.5 EMAC Control Module Receive Interrupt Enable Registers (C0RXEN)
The EMAC control module receive interrupt enable register (C0RXEN) isshown inFigure 32-19 and
described inTable 32-15
Figure 32-19. EMAC Control Module Receive Interrupt Enable Register (C0RXEN) (offset =14h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7EN RXCH6EN RXCH5EN RXCH4EN RXCH3EN RXCH2EN RXCH1EN RXCH0EN
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-15. EMAC Control Module Receive Interrupt Enable Register (C0RXEN)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7EN Enable C0RXPULSE interrupt generation forRXChannel 7.
0 C0RXPULSE generation isdisabled forRXChannel 7.
1 C0RXPULSE generation isenabled forRXChannel 7.
6 RXCH6EN Enable C0RXPULSE interrupt generation forRXChannel 6.
0 C0RXPULSE generation isdisabled forRXChannel 6.
1 C0RXPULSE generation isenabled forRXChannel 6.
5 RXCH5EN Enable C0RXPULSE interrupt generation forRXChannel 5.
0 C0RXPULSE generation isdisabled forRXChannel 5.
1 C0RXPULSE generation isenabled forRXChannel 5.
4 RXCH4EN Enable C0RXPULSE interrupt generation forRXChannel 4.
0 C0RXPULSE generation isdisabled forRXChannel 4.
1 C0RXPULSE generation isenabled forRXChannel 4.
3 RXCH3EN Enable C0RXPULSE interrupt generation forRXChannel 3.
0 C0RXPULSE generation isdisabled forRXChannel 3.
1 C0RXPULSE generation isenabled forRXChannel 3.
2 RXCH2EN Enable C0RXPULSE interrupt generation forRXChannel 2.
0 C0RXPULSE generation isdisabled forRXChannel 2.
1 C0RXPULSE generation isenabled forRXChannel 2.
1 RXCH1EN Enable C0RXPULSE interrupt generation forRXChannel 1.
0 C0RXPULSE generation isdisabled forRXChannel 1.
1 C0RXPULSE generation isenabled forRXChannel 1.
0 RXCH0EN Enable C0RXPULSE interrupt generation forRXChannel 0.
0 C0RXPULSE generation isdisabled forRXChannel 0.
1 C0RXPULSE generation isenabled forRXChannel 0.

<!-- Page 1859 -->

www.ti.com EMAC Control Module Registers
1859 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.6 EMAC Control Module Transmit Interrupt Enable Registers (C0TXEN)
The EMAC control module transmit interrupt enable register (C0TXEN) isshown inFigure 32-20 and
described inTable 32-16
Figure 32-20. EMAC Control Module Transmit Interrupt Enable Register (C0TXEN) (offset =18h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TXCH7EN TXCH6EN TXCH5EN TXCH4EN TXCH3EN TXCH2EN TXCH1EN TXCH0EN
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-16. EMAC Control Module Transmit Interrupt Enable Register (C0TXEN)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TXCH7EN Enable C0TXPULSE interrupt generation forTXChannel 7.
0 C0TXPULSE generation isdisabled forTXChannel 7.
1 C0TXPULSE generation isenabled forTXChannel 7.
6 TXCH6EN Enable C0TXPULSE interrupt generation forTXChannel 6.
0 C0TXPULSE generation isdisabled forTXChannel 6.
1 C0TXPULSE generation isenabled forTXChannel 6.
5 TXCH5EN Enable C0TXPULSE interrupt generation forTXChannel 5.
0 C0TXPULSE generation isdisabled forTXChannel 5.
1 C0TXPULSE generation isenabled forTXChannel 5.
4 TXCH4EN Enable C0TXPULSE interrupt generation forTXChannel 4.
0 C0TXPULSE generation isdisabled forTXChannel 4.
1 C0TXPULSE generation isenabled forTXChannel 4.
3 TXCH3EN Enable C0TXPULSE interrupt generation forTXChannel 3.
0 C0TXPULSE generation isdisabled forTXChannel 3.
1 C0TXPULSE generation isenabled forTXChannel 3.
2 TXCH2EN Enable C0TXPULSE interrupt generation forTXChannel 2.
0 C0TXPULSE generation isdisabled forTXChannel 2.
1 C0TXPULSE generation isenabled forTXChannel 2.
1 TXCH1EN Enable C0TXPULSE interrupt generation forTXChannel 1.
0 C0TXPULSE generation isdisabled forTXChannel 1.
1 C0TXPULSE generation isenabled forTXChannel 1.
0 TXCH0EN Enable C0TXPULSE interrupt generation forTXChannel 0.
0 C0TXPULSE generation isdisabled forTXChannel 0.
1 C0TXPULSE generation isenabled forTXChannel 0.

<!-- Page 1860 -->

EMAC Control Module Registers www.ti.com
1860 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.7 EMAC Control Module Miscellaneous Interrupt Enable Registers (C0MISCEN)
The EMAC control module miscellaneous interrupt enable register (C0MISCEN) isshown inFigure 32-21
anddescribed inTable 32-17
Figure 32-21. EMAC Control Module Miscellaneous Interrupt Enable Register (C0MISCEN)
(offset =1Ch)
31 16
Reserved
R-0
15 4 3 2 1 0
Reserved STATPENDEN HOSTPENDEN LINKINT0EN USERINT0EN
R-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-17. EMAC Control Module Miscellaneous Interrupt Enable Register (C0MISCEN)
Bit Field Value Description
31-4 Reserved 0 Reserved
3 STATPENDEN Enable C0MISCPULSE interrupt generation when EMAC statistics interrupts aregenerated.
0 C0MISCPULSE generation isdisabled forEMAC STATPEND interrupts.
1 C0MISCPULSE generation isenabled forEMAC STATPEND interrupts.
2 HOSTPENDEN Enable C0MISCPULSE interrupt generation when EMAC host interrupts aregenerated.
0 C0MISCPULSE generation isdisabled forEMAC HOSTPEND interrupts.
1 C0MISCPULSE generation isenabled forEMAC HOSTPEND interrupts.
1 LINKINT0EN Enable C0MISCPULSE interrupt generation when MDIO LINKINT0 interrupts (corresponding to
USERPHYSEL0) aregenerated.
0 C0MISCPULSE generation isdisabled forMDIO LINKINT0 interrupts.
1 C0MISCPULSE generation isenabled forMDIO LINKINT0 interrupts.
0 USERINT0EN Enable C0MISCPULSE interrupt generation when MDIO USERINT0 interrupts (corresponding
toUSERACCESS0) aregenerated.
0 C0MISCPULSE generation isdisabled forMDIO USERINT0.
1 C0MISCPULSE generation isenabled forMDIO USERINT0.

<!-- Page 1861 -->

www.ti.com EMAC Control Module Registers
1861 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.8 EMAC Control Module Receive Threshold Interrupt Status Registers
(C0RXTHRESHSTAT)
The EMAC control module receive threshold interrupt status register (C0RXTHRESHSTAT) isshown in
Figure 32-22 anddescribed inTable 32-18
Figure 32-22. EMAC Control Module Receive Threshold Interrupt Status Register
(C0RXTHRESHSTAT) (offset =40h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7THRESH
STATRXCH6THRESH
STATRXCH5THRESH
STATRXCH4THRESH
STATRXCH3THRESH
STATRXCH2THRESH
STATRXCH1THRESH
STATRXCH0THRESH
STAT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-18. EMAC Control Module Receive Threshold Interrupt Status Register
(C0RXTHRESHSTAT)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7THRESHSTAT Interrupt status forRXChannel 7masked bytheC0RXTHRESHEN register.
0 RXChannel 7does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 7satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
6 RXCH6THRESHSTAT Interrupt status forRXChannel 6masked bytheC0RXTHRESHEN register.
0 RXChannel 6does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 6satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
5 RXCH5THRESHSTAT Interrupt status forRXChannel 5masked bytheC0RXTHRESHEN register.
0 RXChannel 5does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 5satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
4 RXCH4THRESHSTAT Interrupt status forRXChannel 4masked bytheC0RXTHRESHEN register.
0 RXChannel 4does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 4satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
3 RXCH3THRESHSTAT Interrupt status forRXChannel 3masked bytheC0RXTHRESHEN register.
0 RXChannel 3does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 3satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
2 RXCH2THRESHSTAT Interrupt status forRXChannel 2masked bytheC0RXTHRESHEN register.
0 RXChannel 2does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 2satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXCH1THRESHSTAT Interrupt status forRXChannel 1masked bytheC0RXTHRESHEN register.
0 RXChannel 1does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 1satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.
0 RXCH0THRESHSTAT Interrupt status forRXChannel 0masked bytheC0RXTHRESHEN register.
0 RXChannel 0does notsatisfy conditions togenerate aC0RXTHRESHPULSE interrupt.
1 RXChannel 0satisfies conditions togenerate aC0RXTHRESHPULSE interrupt.

<!-- Page 1862 -->

EMAC Control Module Registers www.ti.com
1862 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.9 EMAC Control Module Receive Interrupt Status Registers (C0RXSTAT)
The EMAC control module receive interrupt status register (C0RXSTAT) isshown inFigure 32-23 and
described inTable 32-19
Figure 32-23. EMAC Control Module Receive Interrupt Status Register (C0RXSTAT) (offset =44h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7STAT RXCH6STAT RXCH5STAT RXCH4STAT RXCH3STAT RXCH2STAT RXCH1STAT RXCH0STAT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-19. EMAC Control Module Receive Interrupt Status Register (C0RXSTAT)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7STAT Interrupt status forRXChannel 7masked bytheC0RXEN register.
0 RXChannel 7does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 7satisfies conditions togenerate aC0RXPULSE interrupt.
6 RXCH6STAT Interrupt status forRXChannel 6masked bytheC0RXEN register.
0 RXChannel 6does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 6satisfies conditions togenerate aC0RXPULSE interrupt.
5 RXCH5STAT Interrupt status forRXChannel 5masked bytheC0RXEN register.
0 RXChannel 5does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 5satisfies conditions togenerate aC0RXPULSE interrupt.
4 RXCH4STAT Interrupt status forRXChannel 4masked bytheC0RXEN register.
0 RXChannel 4does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 4satisfies conditions togenerate aC0RXPULSE interrupt.
3 RXCH3STAT Interrupt status forRXChannel 3masked bytheC0RXEN register.
0 RXChannel 3does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 3satisfies conditions togenerate aC0RXPULSE interrupt.
2 RXCH2STAT Interrupt status forRXChannel 2masked bytheC0RXEN register.
0 RXChannel 2does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 2satisfies conditions togenerate aC0RXPULSE interrupt.
1 RXCH1STAT Interrupt status forRXChannel 1masked bytheC0RXEN register.
0 RXChannel 1does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 1satisfies conditions togenerate aC0RXPULSE interrupt.
0 RXCH0STAT Interrupt status forRXChannel 0masked bytheC0RXEN register.
0 RXChannel 0does notsatisfy conditions togenerate aC0RXPULSE interrupt.
1 RXChannel 0satisfies conditions togenerate aC0RXPULSE interrupt.

<!-- Page 1863 -->

www.ti.com EMAC Control Module Registers
1863 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.10 EMAC Control Module Transmit Interrupt Status Registers (C0TXSTAT)
The EMAC control module transmit interrupt status register (C0TXSTAT) isshown inFigure 32-24 and
described inTable 32-20
Figure 32-24. EMAC Control Module Transmit Interrupt Status Register (C0TXSTAT) (offset =48h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TXCH7STAT TXCH6STAT TXCH5STAT TXCH4STAT TXCH3STAT TXCH2STAT TXCH1STAT TXCH0STAT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-20. EMAC Control Module Transmit Interrupt Status Register (C0TXSTAT)
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TXCH7STAT Interrupt status forTXChannel 7masked bytheC0TXEN register.
0 TXChannel 7does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 7satisfies conditions togenerate aC0TXPULSE interrupt.
6 TXCH6STAT Interrupt status forTXChannel 6masked bytheC0TXEN register.
0 TXChannel 6does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 6satisfies conditions togenerate aC0TXPULSE interrupt.
5 TXCH5STAT Interrupt status forTXChannel 5masked bytheC0TXEN register.
0 TXChannel 5does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 5satisfies conditions togenerate aC0TXPULSE interrupt.
4 TXCH4STAT Interrupt status forTXChannel 4masked bytheC0TXEN register.
0 TXChannel 4does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 4satisfies conditions togenerate aC0TXPULSE interrupt.
3 TXCH3STAT Interrupt status forTXChannel 3masked bytheC0TXEN register.
0 TXChannel 3does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 3satisfies conditions togenerate aC0TXPULSE interrupt.
2 TXCH2STAT Interrupt status forTXChannel 2masked bytheC0TXEN register.
0 TXChannel 2does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 2satisfies conditions togenerate aC0TXPULSE interrupt.
1 TXCH1STAT Interrupt status forTXChannel 1masked bytheC0TXEN register.
0 TXChannel 1does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 1satisfies conditions togenerate aC0TXPULSE interrupt.
0 TXCH0STAT Interrupt status forTXChannel 0masked bytheC0TXEN register.
0 TXChannel 0does notsatisfy conditions togenerate aC0TXPULSE interrupt.
1 TXChannel 0satisfies conditions togenerate aC0TXPULSE interrupt.

<!-- Page 1864 -->

EMAC Control Module Registers www.ti.com
1864 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.11 EMAC Control Module Miscellaneous Interrupt Status Registers (C0MISCSTAT)
The EMAC control module miscellaneous interrupt status register (C0MISCSTAT) isshown inFigure 32-
25anddescribed inTable 32-21
Figure 32-25. EMAC Control Module Miscellaneous Interrupt Status Register (C0MISCSTAT)
(offset =4Ch)
31 16
Reserved
R-0
15 4 3 2 1 0
Reserved STATPEND
STATHOSTPEND
STATLINKINT0
STATUSERINT0
STAT
R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-21. EMAC Control Module Miscellaneous Interrupt Status Register (C0MISCSTAT)
Bit Field Value Description
31-4 Reserved 0 Reserved
3 STATPENDSTAT Interrupt status forEMAC STATPEND masked bytheC0MISCEN register.
0 EMAC STATPEND does notsatisfy conditions togenerate aC0MISCPULSE interrupt.
1 EMAC STATPEND satisfies conditions togenerate aC0MISCPULSE interrupt.
2 HOSTPENDSTAT Interrupt status forEMAC HOSTPEND masked bytheC0MISCEN register.
0 EMAC HOSTPEND does notsatisfy conditions togenerate aC0MISCPULSE interrupt.
1 EMAC HOSTPEND satisfies conditions togenerate aC0MISCPULSE interrupt.
1 LINKINT0STAT Interrupt status forMDIO LINKINT0 masked bytheC0MISCEN register.
0 MDIO LINKINT0 does notsatisfy conditions togenerate aC0MISCPULSE interrupt.
1 MDIO LINKINT0 satisfies conditions togenerate aC0MISCPULSE interrupt.
0 USERINT0STAT Interrupt status forMDIO USERINT0 masked bytheC0MISCEN register.
0 MDIO USERINT0 does notsatisfy conditions togenerate aC0MISCPULSE interrupt.
1 MDIO USERINT0 satisfies conditions togenerate aC0MISCPULSE interrupt.

<!-- Page 1865 -->

www.ti.com EMAC Control Module Registers
1865 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.12 EMAC Control Module Receive Interrupts PerMillisecond Registers (C0RXIMAX)
The EMAC control module receive interrupts permillisecond register (C0RXIMAX) isshown inFigure 32-
26anddescribed inTable 32-22
Figure 32-26. EMAC Control Module Receive Interrupts PerMillisecond Register (C0RXIMAX)
(offset =70h)
31 16
Reserved
R-0
15 6 5 0
Reserved RXIMAX
R-0 R/W-0
LEGEND: R=Read only; R/W =Read/Write; -n=value after reset
Table 32-22. EMAC Control Module Receive Interrupts PerMillisecond Register (C0RXIMAX)
Bit Field Value Description
31-6 Reserved 0 Reserved
5-0 RXIMAX 2-3Fh RXIMAX isthedesired number ofC0RXPULSE interrupts generated permillisecond when
C0RXPACEEN isenabled inINTCONTROL.
The pacing mechanism canbedescribed bythefollowing pseudo-code:
while(1) {
interrupt_count =0;
/*Count interrupts over a1ms window */
for(i =0;i<INTCONTROL[INTPRESCALE]*250; i++) {
interrupt_count +=NEW_INTERRUPT_EVENTS();
if(i <INTCONTROL[INTPRESCALE]*pace_counter)
BLOCK_EMAC_INTERRUPTS();
else
ALLOW_EMAC_INTERRUPTS();
}
ALLOW_EMAC_INTERRUPTS();
if(interrupt_count >2*RXIMAX)
pace_counter =255;
else if(interrupt_count >1.5*RXIMAX)
pace_counter =previous_pace_counter*2 +1;
else if(interrupt_count >1.0*RXIMAX)
pace_counter =previous_pace_counter +1;
else if(interrupt_count >0.5*RXIMAX)
pace_counter =previous_pace_counter -1;
else if(interrupt_count !=0)
pace_counter =previous_pace_counter/2;
else
pace_counter =0;
previous_pace_counter =pace_counter;
}

<!-- Page 1866 -->

EMAC Control Module Registers www.ti.com
1866 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.3.13 EMAC Control Module Transmit Interrupts PerMillisecond Registers (C0TXIMAX)
The EMAC control module transmit interrupts permillisecond register (C0TXIMAX) isshown in
Figure 32-27 anddescribed inTable 32-23
Figure 32-27. EMAC Control Module Transmit Interrupts PerMillisecond Register (C0TXIMAX)
(offset =74h)
31 16
Reserved
R-0
15 6 5 0
Reserved TXIMAX
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-23. EMAC Control Module Transmit Interrupts PerMillisecond Register (C0TXIMAX)
Bit Field Value Description
31-6 Reserved 0 Reserved
5-0 TXIMAX 2-3Fh TXIMAX isthedesired number ofC0TXPULSE interrupts generated permillisecond when
C0TXPACEEN isenabled inINTCONTROL.
The pacing mechanism canbedescribed bythefollowing pseudo-code:
while(1) {
interrupt_count =0;
/*Count interrupts over a1ms window */
for(i =0;i<INTCONTROL[INTPRESCALE]*250; i++) {
interrupt_count +=NEW_INTERRUPT_EVENTS();
if(i <INTCONTROL[INTPRESCALE]*pace_counter)
BLOCK_EMAC_INTERRUPTS();
else
ALLOW_EMAC_INTERRUPTS();
}
ALLOW_EMAC_INTERRUPTS();
if(interrupt_count >2*TXIMAX)
pace_counter =255;
else if(interrupt_count >1.5*TXIMAX)
pace_counter =previous_pace_counter*2 +1;
else if(interrupt_count >1.0*TXIMAX)
pace_counter =previous_pace_counter +1;
else if(interrupt_count >0.5*TXIMAX)
pace_counter =previous_pace_counter -1;
else if(interrupt_count !=0)
pace_counter =previous_pace_counter/2;
else
pace_counter =0;
previous_pace_counter =pace_counter;
}

<!-- Page 1867 -->

www.ti.com MDIO Registers
1867 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4 MDIO Registers
Table 32-24 lists thememory-mapped registers fortheMDIO module. The base address forthese
registers isFCF7 8900h.
Table 32-24. Management Data Input/Output (MDIO) Registers
Offset Acronym Register Description Section
0h REVID MDIO Revision IDRegister Section 32.4.1
4h CONTROL MDIO Control Register Section 32.4.2
8h ALIVE PHY Alive Status register Section 32.4.3
Ch LINK PHY Link Status Register Section 32.4.4
10h LINKINTRAW MDIO Link Status Change Interrupt (Unmasked) Register Section 32.4.5
14h LINKINTMASKED MDIO Link Status Change Interrupt (Masked) Register Section 32.4.6
20h USERINTRAW MDIO User Command Complete Interrupt (Unmasked) Register Section 32.4.7
24h USERINTMASKED MDIO User Command Complete Interrupt (Masked) Register Section 32.4.8
28h USERINTMASKSET MDIO User Command Complete Interrupt Mask SetRegister Section 32.4.9
2Ch USERINTMASKCLEAR MDIO User Command Complete Interrupt Mask Clear Register Section 32.4.10
80h USERACCESS0 MDIO User Access Register 0 Section 32.4.11
84h USERPHYSEL0 MDIO User PHY Select Register 0 Section 32.4.12
88h USERACCESS1 MDIO User Access Register 1 Section 32.4.13
8Ch USERPHYSEL1 MDIO User PHY Select Register 1 Section 32.4.14
32.4.1 MDIO Revision IDRegister (REVID)
The MDIO revision IDregister (REVID) isshown inFigure 32-28 anddescribed inTable 32-25 .
Figure 32-28. MDIO Revision IDRegister (REVID) (offset =00h)
31 0
REV
R-0007 0105h
LEGEND: R=Read only; -n=value after reset
Table 32-25. MDIO Revision IDRegister (REVID) Field Descriptions
Bit Field Value Description
31-0 REV Identifies theMDIO Module revision.
0007 0105h Current revision oftheMDIO Module.

<!-- Page 1868 -->

MDIO Registers www.ti.com
1868 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.2 MDIO Control Register (CONTROL)
The MDIO control register (CONTROL) isshown inFigure 32-29 anddescribed inTable 32-26 .
Figure 32-29. MDIO Control Register (CONTROL) (offset =04h)
31 30 29 28 24 23 21 20 19 18 17 16
IDLE ENABLE Rsvd HIGHEST_USER_CHANNEL Reserved PREAMBLE FAULT FAULTENB Reserved
R-1 R/W-0 R-0 R-1 R-0 R/W-0 R/W1C-0 R/W-0 R-0
15 0
CLKDIV
R/W-FFh
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-26. MDIO Control Register (CONTROL) Field Descriptions
Bit Field Value Description
31 IDLE State machine IDLE status bit.
0 State machine isnotinidlestate.
1 State machine isinidlestate.
30 ENABLE State machine enable control bit.IftheMDIO state machine isactive atthetime itis
disabled, itwillcomplete thecurrent operation before halting andsetting theidlebit.
0 Disables theMDIO state machine.
1 Enable theMDIO state machine.
29 Reserved 0 Reserved
28-24 HIGHEST_USER_CHANNEL 0-1Fh Highest user channel thatisavailable inthemodule. Itiscurrently setto1.This
implies thatMDIOUserAccess1 isthehighest available user access channel.
23-21 Reserved 0 Reserved
20 PREAMBLE Preamble disable.
0 Standard MDIO preamble isused.
1 Disables thisdevice from sending MDIO frame preambles.
19 FAULT Fault indicator. This bitissetto1iftheMDIO pins failtoread back what thedevice
isdriving onto them. This indicates aphysical layer fault andthemodule state
machine isreset. Writing a1tothisbitclears thisbit,writing a0hasnoeffect.
0 Nofailure.
1 Physical layer fault; theMDIO state machine isreset.
18 FAULTENB Fault detect enable. This bithastobesetto1toenable thephysical layer fault
detection.
0 Disables thephysical layer fault detection.
1 Enables thephysical layer fault detection.
17-16 Reserved 0 Reserved
15-0 CLKDIV 0-FFFFh Clock Divider bits. This field specifies thedivision ratio between theperipheral clock
andthefrequency ofMDIO_CLK. MDIO_CLK isdisabled when CLKDIV iscleared to
0.MDIO_CLK frequency =peripheral clock frequency/(CLKDIV +1).

<!-- Page 1869 -->

www.ti.com MDIO Registers
1869 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.3 PHY Acknowledge Status Register (ALIVE)
The PHY acknowledge status register (ALIVE) isshown inFigure 32-30 anddescribed inTable 32-27 .
Figure 32-30. PHY Acknowledge Status Register (ALIVE) (offset =08h)
31 0
ALIVE
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-27. PHY Acknowledge Status Register (ALIVE) Field Descriptions
Bit Field Value Description
31-0 ALIVE MDIO Alive bits. Each ofthe32bitsofthisregister issetifthemost recent access tothePHY with
address corresponding totheregister bitnumber was acknowledged bythePHY; thebitisreset ifthe
PHY fails toacknowledge theaccess. Both theuser andpolling accesses toaPHY willcause the
corresponding alive bittobeupdated. The alive bitsareonly meant tobeused togive anindication ofthe
presence ornotofaPHY with thecorresponding address. Writing a1toanybitwillclear it,writing a0
hasnoeffect.
0 The PHY fails toacknowledge theaccess.
1 The most recent access tothePHY with anaddress corresponding totheregister bitnumber was
acknowledged bythePHY.
32.4.4 PHY Link Status Register (LINK)
The PHY linkstatus register (LINK) isshown inFigure 32-31 anddescribed inTable 32-28 .
Figure 32-31. PHY Link Status Register (LINK) (offset =0Ch)
31 0
LINK
R-0
LEGEND: R=Read only; -n=value after reset
Table 32-28. PHY Link Status Register (LINK) Field Descriptions
Bit Field Value Description
31-0 LINK MDIO Link state bits. This register isupdated after aread ofthegeneric status register ofaPHY. The bit
issetifthePHY with thecorresponding address haslinkandthePHY acknowledges theread
transaction. The bitisreset ifthePHY indicates itdoes nothave linkorfails toacknowledge theread
transaction. Writes totheregister have noeffect.
0 The PHY indicates itdoes nothave alinkorfails toacknowledge theread transaction.
1 The PHY with thecorresponding address hasalinkandthePHY acknowledges theread transaction.

<!-- Page 1870 -->

MDIO Registers www.ti.com
1870 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.5 MDIO Link Status Change Interrupt (Unmasked) Register (LINKINTRAW)
The MDIO linkstatus change interrupt (unmasked) register (LINKINTRAW) isshown inFigure 32-32 and
described inTable 32-29 .
Figure 32-32. MDIO Link Status Change Interrupt (Unmasked) Register (LINKINTRAW) (offset =
10h)
31 16
Reserved
R-0
15 2 1 0
Reserved USERPHY1 USERPHY0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-29. MDIO Link Status Change Interrupt (Unmasked) Register (LINKINTRAW)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERPHY1 MDIO Link change event, rawvalue. When asserted, thebitindicates thatthere was anMDIO link
change event (that is,change intheLINK register) corresponding tothePHY address in
USERPHYSEL1. Writing a1willclear theevent, writing a0hasnoeffect.
0 NoMDIO linkchange event.
1 AnMDIO linkchange event (change intheLINK register) corresponding tothePHY address in
MDIO user PHY select register USERPHYSEL1.
0 USERPHY0 MDIO Link change event, rawvalue. When asserted, thebitindicates thatthere was anMDIO link
change event (that is,change intheLINK register) corresponding tothePHY address in
USERPHYSEL0. Writing a1willclear theevent, writing a0hasnoeffect.
0 NoMDIO linkchange event.
1 AnMDIO linkchange event (change intheLINK register) corresponding tothePHY address in
MDIO user PHY select register USERPHYSEL0.

<!-- Page 1871 -->

www.ti.com MDIO Registers
1871 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.6 MDIO Link Status Change Interrupt (Masked) Register (LINKINTMASKED)
The MDIO linkstatus change interrupt (masked) register (LINKINTMASKED) isshown inFigure 32-33 and
described inTable 32-30 .
Figure 32-33. MDIO Link Status Change Interrupt (Masked) Register (LINKINTMASKED)
(offset =14h)
31 16
Reserved
R-0
15 2 1 0
Reserved USERPHY1 USERPHY0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-30. MDIO Link Status Change Interrupt (Masked) Register (LINKINTMASKED)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERPHY1 MDIO Link change interrupt, masked value. When asserted, thebitindicates thatthere was an
MDIO linkchange event (that is,change intheLINK register) corresponding tothePHY address in
USERPHYSEL1 andthecorresponding LINKINTENB bitwas set.Writing a1willclear theevent,
writing a0hasnoeffect.
0 NoMDIO linkchange event.
1 AnMDIO linkchange event (change intheLINK register) corresponding tothePHY address in
MDIO user PHY select register USERPHYSEL1 andtheLINKINTENB bitinUSERPHYSEL1 isset
to1.
0 USERPHY0 MDIO Link change interrupt, masked value. When asserted, thebitindicates thatthere was an
MDIO linkchange event (that is,change intheLINK register) corresponding tothePHY address in
USERPHYSEL0 andthecorresponding LINKINTENB bitwas set.Writing a1willclear theevent,
writing a0hasnoeffect.
0 NoMDIO linkchange event.
1 AnMDIO linkchange event (change intheLINK register) corresponding tothePHY address in
MDIO user PHY select register USERPHYSEL0 andtheLINKINTENB bitinUSERPHYSEL0 isset
to1.

<!-- Page 1872 -->

MDIO Registers www.ti.com
1872 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.7 MDIO User Command Complete Interrupt (Unmasked) Register (USERINTRAW)
The MDIO user command complete interrupt (unmasked) register (USERINTRAW) isshown in
Figure 32-34 anddescribed inTable 32-31 .
Figure 32-34. MDIO User Command Complete Interrupt (Unmasked) Register (USERINTRAW)
(offset =20h)
31 16
Reserved
R-0
15 2 1 0
Reserved USERACCESS1 USERACCESS0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-31. MDIO User Command Complete Interrupt (Unmasked) Register (USERINTRAW)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERACCESS1 MDIO User command complete event bit.When asserted, thebitindicates thatthepreviously
scheduled PHY read orwrite command using theUSERACCESS1 register hascompleted.
Writing a1willclear theevent, writing a0hasnoeffect.
0 NoMDIO user command complete event.
1 The previously scheduled PHY read orwrite command using MDIO user access register
USERACCESS1 hascompleted.
0 USERACCESS0 MDIO User command complete event bit.When asserted, thebitindicates thatthepreviously
scheduled PHY read orwrite command using theUSERACCESS0 register hascompleted.
Writing a1willclear theevent, writing a0hasnoeffect.
0 NoMDIO user command complete event.
1 The previously scheduled PHY read orwrite command using MDIO user access register
USERACCESS0 hascompleted.

<!-- Page 1873 -->

www.ti.com MDIO Registers
1873 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.8 MDIO User Command Complete Interrupt (Masked) Register (USERINTMASKED)
The MDIO user command complete interrupt (masked) register (USERINTMASKED) isshown in
Figure 32-35 anddescribed inTable 32-32 .
Figure 32-35. MDIO User Command Complete Interrupt (Masked) Register (USERINTMASKED)
(offset =24h)
31 16
Reserved
R-0
15 2 1 0
Reserved USERACCESS1 USERACCESS0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-32. MDIO User Command Complete Interrupt (Masked) Register (USERINTMASKED)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERACCESS1 Masked value ofMDIO User command complete interrupt. When asserted, The bitindicates
thatthepreviously scheduled PHY read orwrite command using thatparticular
USERACCESS1 register hascompleted. Writing a1willclear theinterrupt, writing a0hasno
effect.
0 NoMDIO user command complete event.
1 The previously scheduled PHY read orwrite command using MDIO user access register
USERACCESS1 hascompleted andthecorresponding bitinUSERINTMASKSET issetto1.
0 USERACCESS0 Masked value ofMDIO User command complete interrupt. When asserted, The bitindicates
thatthepreviously scheduled PHY read orwrite command using thatparticular
USERACCESS0 register hascompleted. Writing a1willclear theinterrupt, writing a0hasno
effect.
0 NoMDIO user command complete event.
1 The previously scheduled PHY read orwrite command using MDIO user access register
USERACCESS0 hascompleted andthecorresponding bitinUSERINTMASKSET issetto1.

<!-- Page 1874 -->

MDIO Registers www.ti.com
1874 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.9 MDIO User Command Complete Interrupt Mask SetRegister (USERINTMASKSET)
The MDIO user command complete interrupt mask setregister (USERINTMASKSET) isshown in
Figure 32-36 anddescribed inTable 32-33 .
Figure 32-36. MDIO User Command Complete Interrupt Mask SetRegister (USERINTMASKSET)
(offset =28h)
31 16
Reserved
R-0
15 2 1 0
Reserved USERACCESS1 USERACCESS0
R-0 R/W1S-0 R/W1S-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-33. MDIO User Command Complete Interrupt Mask SetRegister (USERINTMASKSET)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERACCESS1 MDIO user interrupt mask setforUSERINTMASKED[1]. Setting abitto1willenable MDIO user
command complete interrupts fortheUSERACCESS1 register. MDIO user interrupt for
USERACCESS1 isdisabled ifthecorresponding bitis0.Writing a0tothisbithasnoeffect.
0 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
disabled.
1 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
enabled.
0 USERACCESS0 MDIO user interrupt mask setforUSERINTMASKED[0]. Setting abitto1willenable MDIO user
command complete interrupts fortheUSERACCESS0 register. MDIO user interrupt for
USERACCESS0 isdisabled ifthecorresponding bitis0.Writing a0tothisbithasnoeffect.
0 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
disabled.
1 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
enabled.

<!-- Page 1875 -->

www.ti.com MDIO Registers
1875 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.10 MDIO User Command Complete Interrupt Mask Clear Register (USERINTMASKCLEAR)
The MDIO user command complete interrupt mask clear register (USERINTMASKCLEAR) isshown in
Figure 32-37 anddescribed inTable 32-34 .
Figure 32-37. MDIO User Command Complete Interrupt Mask Clear Register
(USERINTMASKCLEAR) (offset =2Ch)
31 16
Reserved
R-0
15 2 1 0
Reserved USERACCESS1 USERACCESS0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-34. MDIO User Command Complete Interrupt Mask Clear Register (USERINTMASKCLEAR)
Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 USERACCESS1 MDIO user command complete interrupt mask clear forUSERINTMASKED[1]. Setting thebitto
1willdisable further user command complete interrupts forUSERACCESS1. Writing a0tothis
bithasnoeffect.
0 MDIO user command complete interrupts fortheMDIO user access register USERACCESS1 is
enabled.
1 MDIO user command complete interrupts fortheMDIO user access register USERACCESS1 is
disabled.
0 USERACCESS0 MDIO user command complete interrupt mask clear forUSERINTMASKED[0]. Setting thebitto
1willdisable further user command complete interrupts forUSERACCESS0. Writing a0tothis
bithasnoeffect.
0 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
enabled.
1 MDIO user command complete interrupts fortheMDIO user access register USERACCESS0 is
disabled.

<!-- Page 1876 -->

MDIO Registers www.ti.com
1876 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.11 MDIO User Access Register 0(USERACCESS0)
The MDIO user access register 0(USERACCESS0) isshown inFigure 32-38 anddescribed inTable 32-
35.
Figure 32-38. MDIO User Access Register 0(USERACCESS0) (offset =80h)
31 30 29 28 26 25 21 20 16
GO WRITE ACK Reserved REGADR PHYADR
R/W1S-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
15 0
DATA
R/W-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-35. MDIO User Access Register 0(USERACCESS0) Field Descriptions
Bit Field Value Description
31 GO 0-1 Gobit.Writing a1tothisbitcauses theMDIO state machine toperform anMDIO access when it
isconvenient forittodoso;thisisnotaninstantaneous process. Writing a0tothisbithasno
effect. This bitiswriteable only iftheMDIO state machine isenabled. This bitwillselfclear when
therequested access hasbeen completed. Any writes toUSERACCESS0 areblocked when the
GObitis1.
30 WRITE Write enable bit.Setting thisbitto1causes theMDIO transaction tobearegister write; otherwise,
itisaregister read.
0 The user command isaread operation.
1 The user command isawrite operation.
29 ACK 0-1 Acknowledge bit.This bitissetifthePHY acknowledged theread transaction.
28-26 Reserved 0 Reserved
25-21 REGADR 0-1Fh Register address bits. This field specifies thePHY register tobeaccessed forthistransaction.
20-16 PHYADR 0-1Fh PHY address bits. This field specifies thePHY tobeaccessed forthistransaction.
15-0 DATA 0-FFFFh User data bits. These bitsspecify thedata value read from ortobewritten tothespecified PHY
register.

<!-- Page 1877 -->

www.ti.com MDIO Registers
1877 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.12 MDIO User PHY Select Register 0(USERPHYSEL0)
The MDIO user PHY select register 0(USERPHYSEL0) isshown inFigure 32-39 anddescribed in
Table 32-36 .
Figure 32-39. MDIO User PHY Select Register 0(USERPHYSEL0) (offset =84h)
31 16
Reserved
R-0
15 8 7 6 5 4 0
Reserved LINKSEL LINKINTENB Rsvd PHYADRMON
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-36. MDIO User PHY Select Register 0(USERPHYSEL0) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 LINKSEL Link status determination select bit.Default value is0,which implies thatthelinkstatus is
determined bytheMDIO state machine. This istheonly option supported onthisdevice.
0 The linkstatus isdetermined bytheMDIO state machine.
1 Notsupported.
6 LINKINTENB Link change interrupt enable. Setto1toenable linkchange status interrupts forPHY address
specified inPHYADRMON. Link change interrupts aredisabled ifthisbitiscleared to0.
0 Link change interrupts aredisabled.
1 Link change status interrupts forPHY address specified inPHYADDRMON bitsareenabled.
5 Reserved 0 Reserved
4-0 PHYADRMON 0-1Fh PHY address whose linkstatus istobemonitored.

<!-- Page 1878 -->

MDIO Registers www.ti.com
1878 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.13 MDIO User Access Register 1(USERACCESS1)
The MDIO user access register 1(USERACCESS1) isshown inFigure 32-40 anddescribed inTable 32-
37.
Figure 32-40. MDIO User Access Register 1(USERACCESS1) (offset =88h)
31 30 29 28 26 25 21 20 16
GO WRITE ACK Reserved REGADR PHYADR
R/W1S-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
15 0
DATA
R/W-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-37. MDIO User Access Register 1(USERACCESS1) Field Descriptions
Bit Field Value Description
31 GO 0-1 Gobit.Writing 1tothisbitcauses theMDIO state machine toperform anMDIO access when itis
convenient forittodoso;thisisnotaninstantaneous process. Writing 0tothisbithasnoeffect.
This bitiswriteable only iftheMDIO state machine isenabled. This bitwillselfclear when the
requested access hasbeen completed. Any writes toUSERACCESS0 areblocked when theGO
bitis1.
30 WRITE Write enable bit.Setting thisbitto1causes theMDIO transaction tobearegister write; otherwise,
itisaregister read.
0 The user command isaread operation.
1 The user command isawrite operation.
29 ACK 0-1 Acknowledge bit.This bitissetifthePHY acknowledged theread transaction.
28-26 Reserved 0 Reserved
25-21 REGADR 0-1Fh Register address bits. This field specifies thePHY register tobeaccessed forthistransaction.
20-16 PHYADR 0-1Fh PHY address bits. This field specifies thePHY tobeaccessed forthistransaction.
15-0 DATA 0-FFFFh User data bits. These bitsspecify thedata value read from ortobewritten tothespecified PHY
register.

<!-- Page 1879 -->

www.ti.com MDIO Registers
1879 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.4.14 MDIO User PHY Select Register 1(USERPHYSEL1)
The MDIO user PHY select register 1(USERPHYSEL1) isshown inFigure 32-41 anddescribed in
Table 32-38 .
Figure 32-41. MDIO User PHY Select Register 1(USERPHYSEL1) (offset =8Ch)
31 16
Reserved
R-0
15 8 7 6 5 4 0
Reserved LINKSEL LINKINTENB Rsvd PHYADRMON
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-38. MDIO User PHY Select Register 1(USERPHYSEL1) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 LINKSEL Link status determination select bit.Default value is0,which implies thatthelinkstatus is
determined bytheMDIO state machine. This istheonly option supported onthisdevice.
0 The linkstatus isdetermined bytheMDIO state machine.
1 Notsupported.
6 LINKINTENB Link change interrupt enable. Setto1toenable linkchange status interrupts forthePHY address
specified inPHYADRMON. Link change interrupts aredisabled ifthisbitiscleared to0.
0 Link change interrupts aredisabled.
1 Link change status interrupts forPHY address specified inPHYADDRMON bitsareenabled.
5 Reserved 0 PHY address whose linkstatus istobemonitored.
4-0 PHYADRMON 0-1Fh PHY address whose linkstatus istobemonitored.

<!-- Page 1880 -->

EMAC Module Registers www.ti.com
1880 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5 EMAC Module Registers
Table 32-39 lists thememory-mapped registers fortheEMAC. The base address forthese registers is
FCF7 8000h.
Table 32-39. Ethernet Media Access Controller (EMAC) Registers
Offset Acronym Register Description Section
0h TXREVID Transmit Revision IDRegister Section 32.5.1
4h TXCONTROL Transmit Control Register Section 32.5.2
8h TXTEARDOWN Transmit Teardown Register Section 32.5.3
10h RXREVID Receive Revision IDRegister Section 32.5.4
14h RXCONTROL Receive Control Register Section 32.5.5
18h RXTEARDOWN Receive Teardown Register Section 32.5.6
80h TXINTSTATRAW Transmit Interrupt Status (Unmasked) Register Section 32.5.7
84h TXINTSTATMASKED Transmit Interrupt Status (Masked) Register Section 32.5.8
88h TXINTMASKSET Transmit Interrupt Mask SetRegister Section 32.5.9
8Ch TXINTMASKCLEAR Transmit Interrupt Clear Register Section 32.5.10
90h MACINVECTOR MAC Input Vector Register Section 32.5.11
94h MACEOIVECTOR MAC End OfInterrupt Vector Register Section 32.5.12
A0h RXINTSTATRAW Receive Interrupt Status (Unmasked) Register Section 32.5.13
A4h RXINTSTATMASKED Receive Interrupt Status (Masked) Register Section 32.5.14
A8h RXINTMASKSET Receive Interrupt Mask SetRegister Section 32.5.15
ACh RXINTMASKCLEAR Receive Interrupt Mask Clear Register Section 32.5.16
B0h MACINTSTATRAW MAC Interrupt Status (Unmasked) Register Section 32.5.17
B4h MACINTSTATMASKED MAC Interrupt Status (Masked) Register Section 32.5.18
B8h MACINTMASKSET MAC Interrupt Mask SetRegister Section 32.5.19
BCh MACINTMASKCLEAR MAC Interrupt Mask Clear Register Section 32.5.20
100h RXMBPENABLE Receive Multicast/Broadcast/Promiscuous Channel Enable
RegisterSection 32.5.21
104h RXUNICASTSET Receive Unicast Enable SetRegister Section 32.5.22
108h RXUNICASTCLEAR Receive Unicast Clear Register Section 32.5.23
10Ch RXMAXLEN Receive Maximum Length Register Section 32.5.24
110h RXBUFFEROFFSET Receive Buffer Offset Register Section 32.5.25
114h RXFILTERLOWTHRESH Receive Filter Low Priority Frame Threshold Register Section 32.5.26
120h RX0FLOWTHRESH Receive Channel 0Flow Control Threshold Register Section 32.5.27
124h RX1FLOWTHRESH Receive Channel 1Flow Control Threshold Register Section 32.5.27
128h RX2FLOWTHRESH Receive Channel 2Flow Control Threshold Register Section 32.5.27
12Ch RX3FLOWTHRESH Receive Channel 3Flow Control Threshold Register Section 32.5.27
130h RX4FLOWTHRESH Receive Channel 4Flow Control Threshold Register Section 32.5.27
134h RX5FLOWTHRESH Receive Channel 5Flow Control Threshold Register Section 32.5.27
138h RX6FLOWTHRESH Receive Channel 6Flow Control Threshold Register Section 32.5.27
13Ch RX7FLOWTHRESH Receive Channel 7Flow Control Threshold Register Section 32.5.27
140h RX0FREEBUFFER Receive Channel 0Free Buffer Count Register Section 32.5.28
144h RX1FREEBUFFER Receive Channel 1Free Buffer Count Register Section 32.5.28
148h RX2FREEBUFFER Receive Channel 2Free Buffer Count Register Section 32.5.28
14Ch RX3FREEBUFFER Receive Channel 3Free Buffer Count Register Section 32.5.28
150h RX4FREEBUFFER Receive Channel 4Free Buffer Count Register Section 32.5.28
154h RX5FREEBUFFER Receive Channel 5Free Buffer Count Register Section 32.5.28
158h RX6FREEBUFFER Receive Channel 6Free Buffer Count Register Section 32.5.28
15Ch RX7FREEBUFFER Receive Channel 7Free Buffer Count Register Section 32.5.28
160h MACCONTROL MAC Control Register Section 32.5.29

<!-- Page 1881 -->

www.ti.com EMAC Module Registers
1881 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-39. Ethernet Media Access Controller (EMAC) Registers (continued)
Offset Acronym Register Description Section
164h MACSTATUS MAC Status Register Section 32.5.30
168h EMCONTROL Emulation Control Register Section 32.5.31
16Ch FIFOCONTROL FIFO Control Register Section 32.5.32
170h MACCONFIG MAC Configuration Register Section 32.5.33
174h SOFTRESET Soft Reset Register Section 32.5.34
1D0h MACSRCADDRLO MAC Source Address Low Bytes Register Section 32.5.35
1D4h MACSRCADDRHI MAC Source Address High Bytes Register Section 32.5.36
1D8h MACHASH1 MAC Hash Address Register 1 Section 32.5.37
1DCh MACHASH2 MAC Hash Address Register 2 Section 32.5.38
1E0h BOFFTEST Back OffTest Register Section 32.5.39
1E4h TPACETEST Transmit Pacing Algorithm Test Register Section 32.5.40
1E8h RXPAUSE Receive Pause Timer Register Section 32.5.41
1ECh TXPAUSE Transmit Pause Timer Register Section 32.5.42
500h MACADDRLO MAC Address Low Bytes Register Section 32.5.43
504h MACADDRHI MAC Address High Bytes Register Section 32.5.44
508h MACINDEX MAC Index Register Section 32.5.45
600h TX0HDP Transmit Channel 0DMA Head Descriptor Pointer Register Section 32.5.46
604h TX1HDP Transmit Channel 1DMA Head Descriptor Pointer Register Section 32.5.46
608h TX2HDP Transmit Channel 2DMA Head Descriptor Pointer Register Section 32.5.46
60Ch TX3HDP Transmit Channel 3DMA Head Descriptor Pointer Register Section 32.5.46
610h TX4HDP Transmit Channel 4DMA Head Descriptor Pointer Register Section 32.5.46
614h TX5HDP Transmit Channel 5DMA Head Descriptor Pointer Register Section 32.5.46
618h TX6HDP Transmit Channel 6DMA Head Descriptor Pointer Register Section 32.5.46
61Ch TX7HDP Transmit Channel 7DMA Head Descriptor Pointer Register Section 32.5.46
620h RX0HDP Receive Channel 0DMA Head Descriptor Pointer Register Section 32.5.47
624h RX1HDP Receive Channel 1DMA Head Descriptor Pointer Register Section 32.5.47
628h RX2HDP Receive Channel 2DMA Head Descriptor Pointer Register Section 32.5.47
62Ch RX3HDP Receive Channel 3DMA Head Descriptor Pointer Register Section 32.5.47
630h RX4HDP Receive Channel 4DMA Head Descriptor Pointer Register Section 32.5.47
634h RX5HDP Receive Channel 5DMA Head Descriptor Pointer Register Section 32.5.47
638h RX6HDP Receive Channel 6DMA Head Descriptor Pointer Register Section 32.5.47
63Ch RX7HDP Receive Channel 7DMA Head Descriptor Pointer Register Section 32.5.47
640h TX0CP Transmit Channel 0Completion Pointer Register Section 32.5.48
644h TX1CP Transmit Channel 1Completion Pointer Register Section 32.5.48
648h TX2CP Transmit Channel 2Completion Pointer Register Section 32.5.48
64Ch TX3CP Transmit Channel 3Completion Pointer Register Section 32.5.48
650h TX4CP Transmit Channel 4Completion Pointer Register Section 32.5.48
654h TX5CP Transmit Channel 5Completion Pointer Register Section 32.5.48
658h TX6CP Transmit Channel 6Completion Pointer Register Section 32.5.48
65Ch TX7CP Transmit Channel 7Completion Pointer Register Section 32.5.48
660h RX0CP Receive Channel 0Completion Pointer Register Section 32.5.49
664h RX1CP Receive Channel 1Completion Pointer Register Section 32.5.49
668h RX2CP Receive Channel 2Completion Pointer Register Section 32.5.49
66Ch RX3CP Receive Channel 3Completion Pointer Register Section 32.5.49
670h RX4CP Receive Channel 4Completion Pointer Register Section 32.5.49
674h RX5CP Receive Channel 5Completion Pointer Register Section 32.5.49
678h RX6CP Receive Channel 6Completion Pointer Register Section 32.5.49

<!-- Page 1882 -->

EMAC Module Registers www.ti.com
1882 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-39. Ethernet Media Access Controller (EMAC) Registers (continued)
Offset Acronym Register Description Section
67Ch RX7CP Receive Channel 7Completion Pointer Register Section 32.5.49
Network Statistics Registers
200h RXGOODFRAMES Good Receive Frames Register Section 32.5.50.1
204h RXBCASTFRAMES Broadcast Receive Frames Register Section 32.5.50.2
208h RXMCASTFRAMES Multicast Receive Frames Register Section 32.5.50.3
20Ch RXPAUSEFRAMES Pause Receive Frames Register Section 32.5.50.4
210h RXCRCERRORS Receive CRC Errors Register Section 32.5.50.5
214h RXALIGNCODEERRORS Receive Alignment/Code Errors Register Section 32.5.50.6
218h RXOVERSIZED Receive Oversized Frames Register Section 32.5.50.7
21Ch RXJABBER Receive Jabber Frames Register Section 32.5.50.8
220h RXUNDERSIZED Receive Undersized Frames Register Section 32.5.50.9
224h RXFRAGMENTS Receive Frame Fragments Register Section 32.5.50.10
228h RXFILTERED Filtered Receive Frames Register Section 32.5.50.11
22Ch RXQOSFILTERED Receive QOS Filtered Frames Register Section 32.5.50.12
230h RXOCTETS Receive Octet Frames Register Section 32.5.50.13
234h TXGOODFRAMES Good Transmit Frames Register Section 32.5.50.14
238h TXBCASTFRAMES Broadcast Transmit Frames Register Section 32.5.50.15
23Ch TXMCASTFRAMES Multicast Transmit Frames Register Section 32.5.50.16
240h TXPAUSEFRAMES Pause Transmit Frames Register Section 32.5.50.17
244h TXDEFERRED Deferred Transmit Frames Register Section 32.5.50.18
248h TXCOLLISION Transmit Collision Frames Register Section 32.5.50.19
24Ch TXSINGLECOLL Transmit Single Collision Frames Register Section 32.5.50.20
250h TXMULTICOLL Transmit Multiple Collision Frames Register Section 32.5.50.21
254h TXEXCESSIVECOLL Transmit Excessive Collision Frames Register Section 32.5.50.22
258h TXLATECOLL Transmit Late Collision Frames Register Section 32.5.50.23
25Ch TXUNDERRUN Transmit Underrun Error Register Section 32.5.50.24
260h TXCARRIERSENSE Transmit Carrier Sense Errors Register Section 32.5.50.25
264h TXOCTETS Transmit Octet Frames Register Section 32.5.50.26
268h FRAME64 Transmit andReceive 64Octet Frames Register Section 32.5.50.27
26Ch FRAME65T127 Transmit andReceive 65to127Octet Frames Register Section 32.5.50.28
270h FRAME128T255 Transmit andReceive 128to255Octet Frames Register Section 32.5.50.29
274h FRAME256T511 Transmit andReceive 256to511Octet Frames Register Section 32.5.50.30
278h FRAME512T1023 Transmit andReceive 512to1023 Octet Frames Register Section 32.5.50.31
27Ch FRAME1024TUP Transmit andReceive 1024 toRXMAXLEN Octet Frames Register Section 32.5.50.32
280h NETOCTETS Network Octet Frames Register Section 32.5.50.33
284h RXSOFOVERRUNS Receive FIFO orDMA Start ofFrame Overruns Register Section 32.5.50.34
288h RXMOFOVERRUNS Receive FIFO orDMA Middle ofFrame Overruns Register Section 32.5.50.35
28Ch RXDMAOVERRUNS Receive DMA Overruns Register Section 32.5.50.36

<!-- Page 1883 -->

www.ti.com EMAC Module Registers
1883 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.1 Transmit Revision IDRegister (TXREVID)
The transmit revision IDregister (TXREVID) isshown inFigure 32-42 anddescribed inTable 32-40 .
Figure 32-42. Transmit Revision IDRegister (TXREVID) (offset =00h)
31 0
TXREV
R-4EC0 020Dh
LEGEND: R=Read only; -n=value after reset
Table 32-40. Transmit Revision IDRegister (TXREVID) Field Descriptions
Bit Field Value Description
31-0 TXREV Transmit module revision.
4EC0 020Dh Current transmit revision value.
32.5.2 Transmit Control Register (TXCONTROL)
The transmit control register (TXCONTROL) isshown inFigure 32-43 anddescribed inTable 32-41 .
Figure 32-43. Transmit Control Register (TXCONTROL) (offset =04h)
31 16
Reserved
R-0
15 1 0
Reserved TXEN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-41. Transmit Control Register (TXCONTROL) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reserved
0 TXEN Transmit enable.
0 Transmit isdisabled.
1 Transmit isenabled.

<!-- Page 1884 -->

EMAC Module Registers www.ti.com
1884 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.3 Transmit Teardown Register (TXTEARDOWN)
The transmit teardown register (TXTEARDOWN) isshown inFigure 32-44 anddescribed inTable 32-42 .
Figure 32-44. Transmit Teardown Register (TXTEARDOWN) (offset =08h)
31 16
Reserved
R-0
15 3 2 0
Reserved TXTDNCH
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-42. Transmit Teardown Register (TXTEARDOWN) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved
2-0 TXTDNCH 0-7h Transmit teardown channel. The transmit channel teardown iscommanded bywriting theencoded
value ofthetransmit channel tobetorn down. The teardown register isread as0.
0 Teardown transmit channel 0.
1h Teardown transmit channel 1.
2h Teardown transmit channel 2.
3h Teardown transmit channel 3.
4h Teardown transmit channel 4.
5h Teardown transmit channel 5.
6h Teardown transmit channel 6.
7h Teardown transmit channel 7.
32.5.4 Receive Revision IDRegister (RXREVID)
The receive revision IDregister (RXREVID) isshown inFigure 32-45 anddescribed inTable 32-43 .
Figure 32-45. Receive Revision IDRegister (RXREVID) (offset =10h)
31 0
RXREV
R-4EC0 020Dh
LEGEND: R=Read only; -n=value after reset
Table 32-43. Receive Revision IDRegister (RXREVID) Field Descriptions
Bit Field Value Description
31-0 RXREV Receive module revision.
4EC0 020Dh Current receive revision value.

<!-- Page 1885 -->

www.ti.com EMAC Module Registers
1885 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.5 Receive Control Register (RXCONTROL)
The receive control register (RXCONTROL) isshown inFigure 32-46 anddescribed inTable 32-44 .
Figure 32-46. Receive Control Register (RXCONTROL) (offset =14h)
31 16
Reserved
R-0
15 1 0
Reserved RXEN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-44. Receive Control Register (RXCONTROL) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reserved
0 RXEN Receive enable.
0 Receive isdisabled.
1 Receive isenabled.
32.5.6 Receive Teardown Register (RXTEARDOWN)
The receive teardown register (RXTEARDOWN) isshown inFigure 32-47 anddescribed inTable 32-45 .
Figure 32-47. Receive Teardown Register (RXTEARDOWN) (offset =18h)
31 16
Reserved
R-0
15 3 2 0
Reserved RXTDNCH
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-45. Receive Teardown Register (RXTEARDOWN) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved
2-0 RXTDNCH Receive teardown channel. The receive channel teardown iscommanded bywriting theencoded value
ofthereceive channel tobetorn down. The teardown register isread as0.
0 Teardown receive channel 0.
1h Teardown receive channel 1.
2h Teardown receive channel 2.
3h Teardown receive channel 3.
4h Teardown receive channel 4.
5h Teardown receive channel 5.
6h Teardown receive channel 6.
7h Teardown receive channel 7.

<!-- Page 1886 -->

EMAC Module Registers www.ti.com
1886 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.7 Transmit Interrupt Status (Unmasked) Register (TXINTSTATRAW)
The transmit interrupt status (unmasked) register (TXINTSTATRAW) isshown inFigure 32-48 and
described inTable 32-46 .
Figure 32-48. Transmit Interrupt Status (Unmasked) Register (TXINTSTATRAW) (offset =80h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TX7PEND TX6PEND TX5PEND TX4PEND TX3PEND TX2PEND TX1PEND TX0PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-46. Transmit Interrupt Status (Unmasked) Register (TXINTSTATRAW) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TX7PEND 0-1 TX7PEND rawinterrupt read (before mask).
6 TX6PEND 0-1 TX6PEND rawinterrupt read (before mask).
5 TX5PEND 0-1 TX5PEND rawinterrupt read (before mask).
4 TX4PEND 0-1 TX4PEND rawinterrupt read (before mask).
3 TX3PEND 0-1 TX3PEND rawinterrupt read (before mask).
2 TX2PEND 0-1 TX2PEND rawinterrupt read (before mask).
1 TX1PEND 0-1 TX1PEND rawinterrupt read (before mask).
0 TX0PEND 0-1 TX0PEND rawinterrupt read (before mask).

<!-- Page 1887 -->

www.ti.com EMAC Module Registers
1887 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.8 Transmit Interrupt Status (Masked) Register (TXINTSTATMASKED)
The transmit interrupt status (masked) register (TXINTSTATMASKED) isshown inFigure 32-49 and
described inTable 32-47 .
Figure 32-49. Transmit Interrupt Status (Masked) Register (TXINTSTATMASKED) (offset =84h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TX7PEND TX6PEND TX5PEND TX4PEND TX3PEND TX2PEND TX1PEND TX0PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-47. Transmit Interrupt Status (Masked) Register (TXINTSTATMASKED) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TX7PEND 0-1 TX7PEND masked interrupt read.
6 TX6PEND 0-1 TX6PEND masked interrupt read.
5 TX5PEND 0-1 TX5PEND masked interrupt read.
4 TX4PEND 0-1 TX4PEND masked interrupt read.
3 TX3PEND 0-1 TX3PEND masked interrupt read.
2 TX2PEND 0-1 TX2PEND masked interrupt read.
1 TX1PEND 0-1 TX1PEND masked interrupt read.
0 TX0PEND 0-1 TX0PEND masked interrupt read.

<!-- Page 1888 -->

EMAC Module Registers www.ti.com
1888 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.9 Transmit Interrupt Mask SetRegister (TXINTMASKSET)
The transmit interrupt mask setregister (TXINTMASKSET) isshown inFigure 32-50 anddescribed in
Table 32-48 .
Figure 32-50. Transmit Interrupt Mask SetRegister (TXINTMASKSET) (offset =88h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TX7MASK TX6MASK TX5MASK TX4MASK TX3MASK TX2MASK TX1MASK TX0MASK
R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-48. Transmit Interrupt Mask SetRegister (TXINTMASKSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TX7MASK 0-1 Transmit channel 7interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
6 TX6MASK 0-1 Transmit channel 6interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
5 TX5MASK 0-1 Transmit channel 5interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
4 TX4MASK 0-1 Transmit channel 4interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
3 TX3MASK 0-1 Transmit channel 3interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
2 TX2MASK 0-1 Transmit channel 2interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
1 TX1MASK 0-1 Transmit channel 1interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
0 TX0MASK 0-1 Transmit channel 0interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.

<!-- Page 1889 -->

www.ti.com EMAC Module Registers
1889 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.10 Transmit Interrupt Mask Clear Register (TXINTMASKCLEAR)
The transmit interrupt mask clear register (TXINTMASKCLEAR) isshown inFigure 32-51 anddescribed in
Table 32-49 .
Figure 32-51. Transmit Interrupt Mask Clear Register (TXINTMASKCLEAR) (offset =8Ch)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
TX7MASK TX6MASK TX5MASK TX4MASK TX3MASK TX2MASK TX1MASK TX0MASK
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-49. Transmit Interrupt Mask Clear Register (TXINTMASKCLEAR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 TX7MASK 0-1 Transmit channel 7interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
6 TX6MASK 0-1 Transmit channel 6interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
5 TX5MASK 0-1 Transmit channel 5interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
4 TX4MASK 0-1 Transmit channel 4interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
3 TX3MASK 0-1 Transmit channel 3interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
2 TX2MASK 0-1 Transmit channel 2interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
1 TX1MASK 0-1 Transmit channel 1interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
0 TX0MASK 0-1 Transmit channel 0interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.

<!-- Page 1890 -->

EMAC Module Registers www.ti.com
1890 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.11 MAC Input Vector Register (MACINVECTOR)
The MAC input vector register (MACINVECTOR) isshown inFigure 32-52 anddescribed inTable 32-50 .
Figure 32-52. MAC Input Vector Register (MACINVECTOR) (offset =90h)
31 28 27 26 25 24 23 16
Reserved STATPEND HOSTPEND LINKINT0 USERINT0 TXPEND
R-0 R-0 R-0 R-0 R-0 R-0
15 8 7 0
RXTHRESHPEND RXPEND
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-50. MAC Input Vector Register (MACINVECTOR) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reserved
27 STATPEND 0-1 EMAC module statistics interrupt (STATPEND) pending status bit.
26 HOSTPEND 0-1 EMAC module host error interrupt (HOSTPEND) pending status bit.
25 LINKINT0 0-1 MDIO module USERPHYSEL0 (LINKINT0) status bit.
24 USERINT0 0-1 MDIO module USERACCESS0 (USERINT0) status bit.
23-16 TXPEND 0-FFh Transmit channels 0-7interrupt (TXnPEND) pending status. Bit16isTX0PEND.
15-8 RXTHRESHPEND 0-FFh Receive channels 0-7interrupt (RXnTHRESHPEND) pending status. Bit8is
RX0THRESHPEND.
7-0 RXPEND 0-FFh Receive channels 0-7interrupt (RXnPEND) pending status bit.Bit0isRX0PEND.

<!-- Page 1891 -->

www.ti.com EMAC Module Registers
1891 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.12 MAC End OfInterrupt Vector Register (MACEOIVECTOR)
The MAC endofinterrupt vector register (MACEOIVECTOR) isshown inFigure 32-53 anddescribed in
Table 32-51 .
Figure 32-53. MAC End OfInterrupt Vector Register (MACEOIVECTOR) (offset =94h)
31 16
Reserved
R-0
15 5 4 0
Reserved INTVECT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-51. MAC End OfInterrupt Vector Register (MACEOIVECTOR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reserved
4-0 INTVECT Acknowledge EMAC control module interrupts.
0h Acknowledge C0RXTHRESH Interrupt
1h Acknowledge C0RX Interrupt
2h Acknowledge C0TX Interrupt
3h Acknowledge C0MISC Interrupt (STATPEND, HOSTPEND, MDIO LINKINT0, MDIO USERINT0)
4h Acknowledge C1RXTHRESH Interrupt
5h Acknowledge C1RX Interrupt
6h Acknowledge C1TX Interrupt
7h Acknowledge C1MISC Interrupt (STATPEND, HOSTPEND, MDIO LINKINT0, MDIO USERINT0)
8h Acknowledge C2RXTHRESH Interrupt
9h Acknowledge C2RX Interrupt
Ah Acknowledge C2TX Interrupt
Bh Acknowledge C2MISC Interrupt (STATPEND, HOSTPEND, MDIO LINKINT0, MDIO USERINT0)
Ch-1Fh Reserved

<!-- Page 1892 -->

EMAC Module Registers www.ti.com
1892 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.13 Receive Interrupt Status (Unmasked) Register (RXINTSTATRAW)
The receive interrupt status (unmasked) register (RXINTSTATRAW) isshown inFigure 32-54 and
described inTable 32-52 .
Figure 32-54. Receive Interrupt Status (Unmasked) Register (RXINTSTATRAW) (offset =A0h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
RX7THRESH
PENDRX6THRESH
PENDRX5THRESH
PENDRX4THRESH
PENDRX3THRESH
PENDRX2THRESH
PENDRX1THRESH
PENDRX0THRESH
PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
RX7PEND RX6PEND RX5PEND RX4PEND RX3PEND RX2PEND RX1PEND RX0PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-52. Receive Interrupt Status (Unmasked) Register (RXINTSTATRAW) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15 RX7THRESHPEND 0-1 RX7THRESHPEND rawinterrupt read (before mask).
14 RX6THRESHPEND 0-1 RX6THRESHPEND rawinterrupt read (before mask).
13 RX5THRESHPEND 0-1 RX5THRESHPEND rawinterrupt read (before mask).
12 RX4THRESHPEND 0-1 RX4THRESHPEND rawinterrupt read (before mask).
11 RX3THRESHPEND 0-1 RX3THRESHPEND rawinterrupt read (before mask).
10 RX2THRESHPEND 0-1 RX2THRESHPEND rawinterrupt read (before mask).
9 RX1THRESHPEND 0-1 RX1THRESHPEND rawinterrupt read (before mask).
8 RX0THRESHPEND 0-1 RX0THRESHPEND rawinterrupt read (before mask).
7 RX7PEND 0-1 RX7PEND rawinterrupt read (before mask).
6 RX6PEND 0-1 RX6PEND rawinterrupt read (before mask).
5 RX5PEND 0-1 RX5PEND rawinterrupt read (before mask).
4 RX4PEND 0-1 RX4PEND rawinterrupt read (before mask).
3 RX3PEND 0-1 RX3PEND rawinterrupt read (before mask).
2 RX2PEND 0-1 RX2PEND rawinterrupt read (before mask).
1 RX1PEND 0-1 RX1PEND rawinterrupt read (before mask).
0 RX0PEND 0-1 RX0PEND rawinterrupt read (before mask).

<!-- Page 1893 -->

www.ti.com EMAC Module Registers
1893 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.14 Receive Interrupt Status (Masked) Register (RXINTSTATMASKED)
The receive interrupt status (masked) register (RXINTSTATMASKED) isshown inFigure 32-55 and
described inTable 32-53 .
Figure 32-55. Receive Interrupt Status (Masked) Register (RXINTSTATMASKED) (offset =A4h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
RX7THRESH
PENDRX6THRESH
PENDRX5THRESH
PENDRX4THRESH
PENDRX3THRESH
PENDRX2THRESH
PENDRX1THRESH
PENDRX0THRESH
PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
RX7PEND RX6PEND RX5PEND RX4PEND RX3PEND RX2PEND RX1PEND RX0PEND
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-53. Receive Interrupt Status (Masked) Register (RXINTSTATMASKED) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15 RX7THRESHPEND 0-1 RX7THRESHPEND masked interrupt read.
14 RX6THRESHPEND 0-1 RX6THRESHPEND masked interrupt read.
13 RX5THRESHPEND 0-1 RX5THRESHPEND masked interrupt read.
12 RX4THRESHPEND 0-1 RX4THRESHPEND masked interrupt read.
11 RX3THRESHPEND 0-1 RX3THRESHPEND masked interrupt read.
10 RX2THRESHPEND 0-1 RX2THRESHPEND masked interrupt read.
9 RX1THRESHPEND 0-1 RX1THRESHPEND masked interrupt read.
8 RX0THRESHPEND 0-1 RX0THRESHPEND masked interrupt read.
7 RX7PEND 0-1 RX7PEND masked interrupt read.
6 RX6PEND 0-1 RX6PEND masked interrupt read.
5 RX5PEND 0-1 RX5PEND masked interrupt read.
4 RX4PEND 0-1 RX4PEND masked interrupt read.
3 RX3PEND 0-1 RX3PEND masked interrupt read.
2 RX2PEND 0-1 RX2PEND masked interrupt read.
1 RX1PEND 0-1 RX1PEND masked interrupt read.
0 RX0PEND 0-1 RX0PEND masked interrupt read.

<!-- Page 1894 -->

EMAC Module Registers www.ti.com
1894 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.15 Receive Interrupt Mask SetRegister (RXINTMASKSET)
The receive interrupt mask setregister (RXINTMASKSET) isshown inFigure 32-56 anddescribed in
Table 32-54 .
Figure 32-56. Receive Interrupt Mask SetRegister (RXINTMASKSET) (offset =A8h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
RX7THRESH
MASKRX6THRESH
MASKRX5THRESH
MASKRX4THRESH
MASKRX3THRESH
MASKRX2THRESH
MASKRX1THRESH
MASKRX0THRESH
MASK
R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0
7 6 5 4 3 2 1 0
RX7MASK RX6MASK RX5MASK RX4MASK RX3MASK RX2MASK RX1MASK RX0MASK
R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-54. Receive Interrupt Mask SetRegister (RXINTMASKSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15 RX7THRESHMASK 0-1 Receive channel 7threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
14 RX6THRESHMASK 0-1 Receive channel 6threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
13 RX5THRESHMASK 0-1 Receive channel 5threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
12 RX4THRESHMASK 0-1 Receive channel 4threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
11 RX3THRESHMASK 0-1 Receive channel 3threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
10 RX2THRESHMASK 0-1 Receive channel 2threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
9 RX1THRESHMASK 0-1 Receive channel 1threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
8 RX0THRESHMASK 0-1 Receive channel 0threshold mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
7 RX7MASK 0-1 Receive channel 7mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
6 RX6MASK 0-1 Receive channel 6mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
5 RX5MASK 0-1 Receive channel 5mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
4 RX4MASK 0-1 Receive channel 4mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
3 RX3MASK 0-1 Receive channel 3mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
2 RX2MASK 0-1 Receive channel 2mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
1 RX1MASK 0-1 Receive channel 1mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.
0 RX0MASK 0-1 Receive channel 0mask setbit.Write 1toenable interrupt; awrite of0hasnoeffect.

<!-- Page 1895 -->

www.ti.com EMAC Module Registers
1895 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.16 Receive Interrupt Mask Clear Register (RXINTMASKCLEAR)
The receive interrupt mask clear register (RXINTMASKCLEAR) isshown inFigure 32-57 anddescribed in
Table 32-55 .
Figure 32-57. Receive Interrupt Mask Clear Register (RXINTMASKCLEAR) (offset =ACh)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
RX7THRESH
MASKRX6THRESH
MASKRX5THRESH
MASKRX4THRESH
MASKRX3THRESH
MASKRX2THRESH
MASKRX1THRESH
MASKRX0THRESH
MASK
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
7 6 5 4 3 2 1 0
RX7MASK RX6MASK RX5MASK RX4MASK RX3MASK RX2MASK RX1MASK RX0MASK
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-55. Receive Interrupt Mask Clear Register (RXINTMASKCLEAR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15 RX7THRESHMASK 0-1 Receive channel 7threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
14 RX6THRESHMASK 0-1 Receive channel 6threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
13 RX5THRESHMASK 0-1 Receive channel 5threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
12 RX4THRESHMASK 0-1 Receive channel 4threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
11 RX3THRESHMASK 0-1 Receive channel 3threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
10 RX2THRESHMASK 0-1 Receive channel 2threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
9 RX1THRESHMASK 0-1 Receive channel 1threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
8 RX0THRESHMASK 0-1 Receive channel 0threshold mask clear bit.Write 1todisable interrupt; awrite of0hasno
effect.
7 RX7MASK 0-1 Receive channel 7mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
6 RX6MASK 0-1 Receive channel 6mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
5 RX5MASK 0-1 Receive channel 5mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
4 RX4MASK 0-1 Receive channel 4mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
3 RX3MASK 0-1 Receive channel 3mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
2 RX2MASK 0-1 Receive channel 2mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
1 RX1MASK 0-1 Receive channel 1mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.
0 RX0MASK 0-1 Receive channel 0mask clear bit.Write 1todisable interrupt; awrite of0hasnoeffect.

<!-- Page 1896 -->

EMAC Module Registers www.ti.com
1896 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.17 MAC Interrupt Status (Unmasked) Register (MACINTSTATRAW)
The MAC interrupt status (unmasked) register (MACINTSTATRAW) isshown inFigure 32-58 and
described inTable 32-56 .
Figure 32-58. MAC Interrupt Status (Unmasked) Register (MACINTSTATRAW) (offset =B0h)
31 16
Reserved
R-0
15 2 1 0
Reserved HOSTPEND STATPEND
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-56. MAC Interrupt Status (Unmasked) Register (MACINTSTATRAW) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 HOSTPEND 0-1 Host pending interrupt (HOSTPEND); rawinterrupt read (before mask).
0 STATPEND 0-1 Statistics pending interrupt (STATPEND); rawinterrupt read (before mask).
32.5.18 MAC Interrupt Status (Masked) Register (MACINTSTATMASKED)
The MAC interrupt status (masked) register (MACINTSTATMASKED) isshown inFigure 32-59 and
described inTable 32-57 .
Figure 32-59. MAC Interrupt Status (Masked) Register (MACINTSTATMASKED) (offset =B4h)
31 16
Reserved
R-0
15 2 1 0
Reserved HOSTPEND STATPEND
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-57. MAC Interrupt Status (Masked) Register (MACINTSTATMASKED) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 HOSTPEND 0-1 Host pending interrupt (HOSTPEND); masked interrupt read.
0 STATPEND 0-1 Statistics pending interrupt (STATPEND); masked interrupt read.

<!-- Page 1897 -->

www.ti.com EMAC Module Registers
1897 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.19 MAC Interrupt Mask SetRegister (MACINTMASKSET)
The MAC interrupt mask setregister (MACINTMASKSET) isshown inFigure 32-60 anddescribed in
Table 32-58 .
Figure 32-60. MAC Interrupt Mask SetRegister (MACINTMASKSET) (offset =B8h)
31 16
Reserved
R-0
15 2 1 0
Reserved HOSTMASK STATMASK
R-0 R/W1S-0 R/W1S-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-58. MAC Interrupt Mask SetRegister (MACINTMASKSET) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 HOSTMASK 0-1 Host error interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
0 STATMASK 0-1 Statistics interrupt mask setbit.Write 1toenable interrupt, awrite of0hasnoeffect.
32.5.20 MAC Interrupt Mask Clear Register (MACINTMASKCLEAR)
The MAC interrupt mask clear register (MACINTMASKCLEAR) isshown inFigure 32-61 anddescribed in
Table 32-59 .
Figure 32-61. MAC Interrupt Mask Clear Register (MACINTMASKCLEAR) (offset =BCh)
31 16
Reserved
R-0
15 2 1 0
Reserved HOSTMASK STATMASK
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-59. MAC Interrupt Mask Clear Register (MACINTMASKCLEAR) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 HOSTMASK 0-1 Host error interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.
0 STATMASK 0-1 Statistics interrupt mask clear bit.Write 1todisable interrupt, awrite of0hasnoeffect.

<!-- Page 1898 -->

EMAC Module Registers www.ti.com
1898 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.21 Receive Multicast/Broadcast/Promiscuous Channel Enable Register (RXMBPENABLE)
The receive multicast/broadcast/promiscuous channel enable register (RXMBPENABLE) isshown in
Figure 32-62 anddescribed inTable 32-60 .
Figure 32-62. Receive Multicast/Broadcast/Promiscuous Channel Enable Register
(RXMBPENABLE) (offset =100h)
31 30 29 28 27 25 24
Reserved RXPASSCRC RXQOSEN RXNOCHAIN Reserved RXCMFEN
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0
23 22 21 20 19 18 16
RXCSFEN RXCEFEN RXCAFEN Reserved RXPROMCH
R/W-0 R/W-0 R/W-0 R-0 R/W-0
15 14 13 12 11 10 8
Reserved RXBROADEN Reserved RXBROADCH
R-0 R/W-0 R-0 R/W-0
7 6 5 4 3 2 0
Reserved RXMULTEN Reserved RXMULTCH
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-60. Receive Multicast/Broadcast/Promiscuous Channel Enable Register (RXMBPENABLE)
Field Descriptions
Bit Field Value Description
31 Reserved 0 Reserved
30 RXPASSCRC Pass receive CRC enable bit.
0 Received CRC isdiscarded forallchannels andisnotincluded inthebuffer descriptor packet
length field.
1 Received CRC istransferred tomemory forallchannels andisincluded inthebuffer descriptor
packet length.
29 RXQOSEN Receive quality ofservice enable bit.
0 Receive QOS isdisabled.
1 Receive QOS isenabled.
28 RXNOCHAIN Receive nobuffer chaining bit.
0 Received frames canspan multiple buffers.
1 The Receive DMA controller transfers each frame intoasingle buffer, regardless oftheframe or
buffer size. Allremaining frame data after thefirstbuffer isdiscarded. The buffer descriptor buffer
length field willcontain theentire frame byte count (upto65535 bytes).
27-25 Reserved 0 Reserved
24 RXCMFEN Receive copy MAC control frames enable bit.Enables MAC control frames tobetransferred to
memory. MAC control frames arenormally acted upon (ifenabled), butnotcopied tomemory. MAC
control frames thatarepause frames willbeacted upon ifenabled inMACCONTROL, regardless of
thevalue ofRXCMFEN. Frames transferred tomemory duetoRXCMFEN willhave theCONTROL
bitsetintheir EOP buffer descriptor.
0 MAC control frames arefiltered (but acted upon ifenabled).
1 MAC control frames aretransferred tomemory.
23 RXCSFEN Receive copy short frames enable bit.Enables frames orfragments shorter than 64bytes tobe
copied tomemory. Frames transferred tomemory duetoRXCSFEN willhave theFRAGMENT or
UNDERSIZE bitsetintheir EOP buffer descriptor. Fragments areshort frames thatcontain CRC /
align /code errors andundersized areshort frames without errors.
0 Short frames arefiltered.
1 Short frames aretransferred tomemory.

<!-- Page 1899 -->

www.ti.com EMAC Module Registers
1899 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-60. Receive Multicast/Broadcast/Promiscuous Channel Enable Register (RXMBPENABLE)
Field Descriptions (continued)
Bit Field Value Description
22 RXCEFEN Receive copy error frames enable bit.Enables frames containing errors tobetransferred to
memory. The appropriate error bitwillbesetintheframe EOP buffer descriptor.
0 Frames containing errors arefiltered.
1 Frames containing errors aretransferred tomemory.
21 RXCAFEN Receive copy allframes enable bit.Enables frames thatdonotaddress match (includes multicast
frames thatdonothash match) tobetransferred tothepromiscuous channel selected by
RXPROMCH bits. Such frames willbemarked with theNOMATCH bitintheir EOP buffer
descriptor.
0 Frames thatdonotaddress match arefiltered.
1 Frames thatdonotaddress match aretransferred tothepromiscuous channel selected by
RXPROMCH bits.
20-19 Reserved 0 Reserved
18-16 RXPROMCH Receive promiscuous channel select.
0 Select channel 0toreceive promiscuous frames.
1h Select channel 1toreceive promiscuous frames.
2h Select channel 2toreceive promiscuous frames.
3h Select channel 3toreceive promiscuous frames.
4h Select channel 4toreceive promiscuous frames.
5h Select channel 5toreceive promiscuous frames.
6h Select channel 6toreceive promiscuous frames.
7h Select channel 7toreceive promiscuous frames.
15-14 Reserved 0 Reserved
13 RXBROADEN Receive broadcast enable. Enable received broadcast frames tobecopied tothechannel selected
byRXBROADCH bits.
0 Broadcast frames arefiltered.
1 Broadcast frames arecopied tothechannel selected byRXBROADCH bits.
12-11 Reserved 0 Reserved
10-8 RXBROADCH Receive broadcast channel select.
0 Select channel 0toreceive broadcast frames.
1h Select channel 1toreceive broadcast frames.
2h Select channel 2toreceive broadcast frames.
3h Select channel 3toreceive broadcast frames.
4h Select channel 4toreceive broadcast frames.
5h Select channel 5toreceive broadcast frames.
6h Select channel 6toreceive broadcast frames.
7h Select channel 7toreceive broadcast frames.
7-6 Reserved 0 Reserved
5 RXMULTEN RXmulticast enable. Enable received hash matching multicast frames tobecopied tothechannel
selected byRXMULTCH bits.
0 Multicast frames arefiltered.
1 Multicast frames arecopied tothechannel selected byRXMULTCH bits.
4-3 Reserved 0 Reserved

<!-- Page 1900 -->

EMAC Module Registers www.ti.com
1900 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-60. Receive Multicast/Broadcast/Promiscuous Channel Enable Register (RXMBPENABLE)
Field Descriptions (continued)
Bit Field Value Description
2-0 RXMULTCH Receive multicast channel select.
0 Select channel 0toreceive multicast frames.
1h Select channel 1toreceive multicast frames.
2h Select channel 2toreceive multicast frames.
3h Select channel 3toreceive multicast frames.
4h Select channel 4toreceive multicast frames.
5h Select channel 5toreceive multicast frames.
6h Select channel 6toreceive multicast frames.
7h Select channel 7toreceive multicast frames.
32.5.22 Receive Unicast Enable SetRegister (RXUNICASTSET)
The receive unicast enable setregister (RXUNICASTSET) isshown inFigure 32-63 anddescribed in
Table 32-61 .
Figure 32-63. Receive Unicast Enable SetRegister (RXUNICASTSET) (offset =104h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7EN RXCH6EN RXCH5EN RXCH4EN RXCH3EN RXCH2EN RXCH1EN RXCH0EN
R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0 R/W1S-0
LEGEND: R/W =Read/Write; R=Read only; W1S =Write 1toset(writing a0hasnoeffect); -n=value after reset
Table 32-61. Receive Unicast Enable SetRegister (RXUNICASTSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7EN 0-1 Receive channel 7unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
6 RXCH6EN 0-1 Receive channel 6unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
5 RXCH5EN 0-1 Receive channel 5unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
4 RXCH4EN 0-1 Receive channel 4unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
3 RXCH3EN 0-1 Receive channel 3unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
2 RXCH2EN 0-1 Receive channel 2unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
1 RXCH1EN 0-1 Receive channel 1unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.
0 RXCH0EN 0-1 Receive channel 0unicast enable setbit.Write 1tosettheenable, awrite of0hasnoeffect.
May beread.

<!-- Page 1901 -->

www.ti.com EMAC Module Registers
1901 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.23 Receive Unicast Clear Register (RXUNICASTCLEAR)
The receive unicast clear register (RXUNICASTCLEAR) isshown inFigure 32-64 anddescribed in
Table 32-62 .
Figure 32-64. Receive Unicast Clear Register (RXUNICASTCLEAR) (offset =108h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RXCH7EN RXCH6EN RXCH5EN RXCH4EN RXCH3EN RXCH2EN RXCH1EN RXCH0EN
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing a0hasnoeffect); -n=value after reset
Table 32-62. Receive Unicast Clear Register (RXUNICASTCLEAR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7 RXCH7EN 0-1 Receive channel 7unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
6 RXCH6EN 0-1 Receive channel 6unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
5 RXCH5EN 0-1 Receive channel 5unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
4 RXCH4EN 0-1 Receive channel 4unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
3 RXCH3EN 0-1 Receive channel 3unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
2 RXCH2EN 0-1 Receive channel 2unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
1 RXCH1EN 0-1 Receive channel 1unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
0 RXCH0EN 0-1 Receive channel 0unicast enable clear bit.Write 1toclear theenable, awrite of0hasnoeffect.
32.5.24 Receive Maximum Length Register (RXMAXLEN)
The receive maximum length register (RXMAXLEN) isshown inFigure 32-65 anddescribed inTable 32-
63.
Figure 32-65. Receive Maximum Length Register (RXMAXLEN) (offset =10Ch)
31 16
Reserved
R-0
15 0
RXMAXLEN
R/W-5EEh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-63. Receive Maximum Length Register (RXMAXLEN) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-0 RXMAXLEN 0-FFFFh Receive maximum frame length. These bitsdetermine themaximum length ofareceived frame.
The reset value is5EEh (1518). Frames with byte counts greater than RXMAXLEN arelong
frames. Long frames with noerrors areoversized frames. Long frames with CRC, code, or
alignment error arejabber frames.

<!-- Page 1902 -->

EMAC Module Registers www.ti.com
1902 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.25 Receive Buffer Offset Register (RXBUFFEROFFSET)
The receive buffer offset register (RXBUFFEROFFSET) isshown inFigure 32-66 anddescribed in
Table 32-64 .
Figure 32-66. Receive Buffer Offset Register (RXBUFFEROFFSET) (offset =110h)
31 16
Reserved
R-0
15 0
RXBUFFEROFFSET
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-64. Receive Buffer Offset Register (RXBUFFEROFFSET) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-0 RXBUFFEROFFSET 0-FFFFh Receive buffer offset value. These bitsarewritten bytheEMAC intoeach frame SOP
buffer descriptor Buffer Offset field. The frame data begins after theRXBUFFEROFFSET
value ofbytes. Avalue of0indicates thatthere arenounused bytes atthebeginning of
thedata, andthatvalid data begins onthefirstbyte ofthebuffer. Avalue ofFh(15)
indicates thatthefirst15bytes ofthebuffer aretobeignored bytheEMAC andthatvalid
buffer data starts onbyte 16ofthebuffer. This value isused forallchannels.
32.5.26 Receive Filter Low Priority Frame Threshold Register (RXFILTERLOWTHRESH)
The receive filter lowpriority frame threshold register (RXFILTERLOWTHRESH) isshown inFigure 32-67
anddescribed inTable 32-65 .
Figure 32-67. Receive Filter Low Priority Frame Threshold Register (RXFILTERLOWTHRESH)
(offset =114h)
31 16
Reserved
R-0
15 8 7 0
Reserved RXFILTERTHRESH
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-65. Receive Filter Low Priority Frame Threshold Register (RXFILTERLOWTHRESH)
Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7-0 RXFILTERTHRESH 0-FFh Receive filter lowthreshold. These bitscontain thefree buffer count threshold value forfiltering
lowpriority incoming frames. This field should remain 0,ifnofiltering isdesired.

<!-- Page 1903 -->

www.ti.com EMAC Module Registers
1903 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.27 Receive Channel Flow Control Threshold Registers (RX0FLOWTHRESH-
RX7FLOWTHRESH)
The receive channel 0-7flow control threshold register (RXnFLOWTHRESH) isshown inFigure 32-68 and
described inTable 32-66 .
Figure 32-68. Receive Channel nFlow Control Threshold Register (RXnFLOWTHRESH)
(offset =120h-13Ch)
31 16
Reserved
R-0
15 8 7 0
Reserved RXnFLOWTHRESH
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-66. Receive Channel nFlow Control Threshold Register (RXnFLOWTHRESH)
Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved
7-0 RXnFLOWTHRESH 0-FFh Receive flow threshold. These bitscontain thethreshold value forissuing flow control on
incoming frames forchannel n(when enabled).
32.5.28 Receive Channel Free Buffer Count Registers (RX0FREEBUFFER-RX7FREEBUFFER)
The receive channel 0-7free buffer count register (RXnFREEBUFFER) isshown inFigure 32-69 and
described inTable 32-67 .
Figure 32-69. Receive Channel nFree Buffer Count Register (RXnFREEBUFFER)
(offset =140h-15Ch)
31 16
Reserved
R-0
15 0
RXnFREEBUF
WI-0
LEGEND: R=Read only; WI=Write toincrement; -n=value after reset
Table 32-67. Receive Channel nFree Buffer Count Register (RXnFREEBUFFER) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-0 RXnFREEBUF 0-FFh Receive free buffer count. These bitscontain thecount offree buffers available. The
RXFILTERTHRESH value iscompared with thisfield todetermine iflowpriority frames should be
filtered. The RXnFLOWTHRESH value iscompared with thisfield todetermine ifreceive flow
control should beissued against incoming packets (ifenabled). This isawrite-to-increment field.
This field rolls over to0onoverflow.
Ifhardware flow control orQOS isused, thehost must initialize thisfield tothenumber ofavailable
buffers (one register perchannel). The EMAC decrements theassociated channel register foreach
received frame bythenumber ofbuffers inthereceived frame. The host must write thisfield with
thenumber ofbuffers thathave been freed duetohost processing.

<!-- Page 1904 -->

EMAC Module Registers www.ti.com
1904 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.29 MAC Control Register (MACCONTROL)
The MAC control register (MACCONTROL) isshown inFigure 32-70 anddescribed inTable 32-68 .
Figure 32-70. MAC Control Register (MACCONTROL) (offset =160h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
RMIISPEED RXOFFLENBLOCK RXOWNERSHIP Rsvd CMDIDLE TXSHORTGAPEN TXPTYPE Reserved
R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0 R-0
7 6 5 4 3 2 1 0
Reserved TXPACE GMIIEN TXFLOWEN RXBUFFERFLOWEN Reserved LOOPBACK FULLDUPLEX
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-68. MAC Control Register (MACCONTROL) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15 RMIISPEED RMII interface transmit andreceive speed select.
0 Operate RMII interface in10Mbps speed mode.
1 Operate RMII interface in100Mbps speed mode.
14 RXOFFLENBLOCK Receive offset /length word write block.
0 Donotblock theDMA writes tothereceive buffer descriptor offset /buffer length word.
1 Block allEMAC DMA controller writes tothereceive buffer descriptor offset /buffer length
words during packet processing. When thisbitisset,theEMAC willnever write thethird word
toanyreceive buffer descriptor.
13 RXOWNERSHIP Receive ownership write bitvalue.
0 The EMAC writes theReceive ownership bitto0attheendofpacket processing.
1 The EMAC writes theReceive ownership bitto1attheendofpacket processing. Ifyoudonot
usetheownership mechanism, youcansetthismode topreclude thenecessity ofsoftware
having tosetthisbiteach time thebuffer descriptor isused.
12 Reserved 0 Reserved
11 CMDIDLE Command Idlebit.
0 Idleisnotcommanded.
1 Idleiscommanded (read IDLE intheMACSTATUS register).
10 TXSHORTGAPEN Transmit Short Gap enable.
0 Transmit with ashort IPG isdisabled. Normal 96-bit time IPG isinserted between packets.
1 Transmit with ashort IPG isenabled. Shorter 88-bit time IPG isinserted between packets.
9 TXPTYPE Transmit queue priority type.
0 The queue uses around-robin scheme toselect thenext channel fortransmission.
1 The queue uses afixed-priority (channel 7highest priority) scheme toselect thenext channel
fortransmission.
8-7 Reserved 0 Reserved
6 TXPACE Transmit pacing enable bit.
0 Transmit pacing isdisabled.
1 Transmit pacing isenabled.

<!-- Page 1905 -->

www.ti.com EMAC Module Registers
1905 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-68. MAC Control Register (MACCONTROL) Field Descriptions (continued)
Bit Field Value Description
5 GMIIEN GMII enable bit.This bitmust besetbefore theMAC transmits orreceives data inanyofthe
supported interface modes. (forinstance, MII,RMII). This bitdoes notselect theinterface
mode butrather holds orreleases theMAC TXandRXstate machines from reset.
0 The MAC RXandTXstate machines areheld inreset.
1 The MAC RXandTXstate machines arereleased from reset andtransmit andreceive are
enabled.
4 TXFLOWEN Transmit flow control enable bit.This bitdetermines ifincoming pause frames areacted upon
infull-duplex mode. Incoming pause frames arenotacted upon inhalf-duplex mode,
regardless ofthisbitsetting. The RXMBPENABLE bitsdetermine whether ornotreceived
pause frames aretransferred tomemory.
0 Transmit flow control isdisabled. Full-duplex mode: incoming pause frames arenotacted
upon.
1 Transmit flow control isenabled. Full-duplex mode: incoming pause frames areacted upon.
3 RXBUFFERFLOWEN Receive buffer flow control enable bit.
0 Receive flow control isdisabled. Half-duplex mode: noflow control generated collisions are
sent. Full-duplex mode: nooutgoing pause frames aresent.
1 Receive flow control isenabled. Half-duplex mode: collisions areinitiated when receive buffer
flow control istriggered. Full-duplex mode: outgoing pause frames aresent when receive flow
control istriggered.
2 Reserved 0 Reserved
1 LOOPBACK Loopback mode. The loopback mode forces internal full-duplex mode regardless ofthe
FULLDUPLEX bit.The loopback bitshould bechanged only when GMIIEN bitisdeasserted.
0 Loopback mode isdisabled.
1 Loopback mode isenabled.
0 FULLDUPLEX Full-duplex mode.
0 Half-duplex mode isenabled.
1 Full-duplex mode isenabled.

<!-- Page 1906 -->

EMAC Module Registers www.ti.com
1906 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.30 MAC Status Register (MACSTATUS)
The MAC status register (MACSTATUS) isshown inFigure 32-71 anddescribed inTable 32-69 .
Figure 32-71. MAC Status Register (MACSTATUS) (offset =164h)
31 30 24 23 20 19 18 16
IDLE Reserved TXERRCODE Rsvd TXERRCH
R-0 R-0 R-0 R-0 R-0
15 12 11 10 8
RXERRCODE Reserved RXERRCH
R-0 R-0 R-0
7 3 2 1 0
Reserved RXQOSACT RXFLOWACT TXFLOWACT
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-69. MAC Status Register (MACSTATUS) Field Descriptions
Bit Field Value Description
31 IDLE EMAC idlebit.This bitiscleared to0atreset; oneclock after reset, itgoes to1.
0 The EMAC isnotidle.
1 The EMAC isintheidlestate.
30-24 Reserved 0 Reserved
23-20 TXERRCODE Transmit host error code. These bitsindicate thatEMAC detected transmit DMA related host errors.
The host should read thisfield after ahost error interrupt (HOSTPEND) todetermine theerror. Host
error interrupts require hardware reset inorder torecover. A0packet length isanerror, butitisnot
detected.
0 Noerror.
1h SOP error; thebuffer isthefirstbuffer inapacket, buttheSOP bitisnotsetinsoftware.
2h Ownership bitnotsetinSOP buffer.
3h Zero next buffer descriptor pointer without EOP.
4h Zero buffer pointer.
5h Zero buffer length.
6h Packet length error (sum ofbuffers isless than packet length).
7h-Fh Reserved
19 Reserved 0 Reserved
18-16 TXERRCH Transmit host error channel. These bitsindicate which transmit channel thehost error occurred on.
This field iscleared to0onahost read.
0 The host error occurred ontransmit channel 0.
1h The host error occurred ontransmit channel 1.
2h The host error occurred ontransmit channel 2.
3h The host error occurred ontransmit channel 3.
4h The host error occurred ontransmit channel 4.
5h The host error occurred ontransmit channel 5.
6h The host error occurred ontransmit channel 6.
7h The host error occurred ontransmit channel 7.

<!-- Page 1907 -->

www.ti.com EMAC Module Registers
1907 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTable 32-69. MAC Status Register (MACSTATUS) Field Descriptions (continued)
Bit Field Value Description
15-12 RXERRCODE Receive host error code. These bitsindicate thatEMAC detected receive DMA related host errors.
The host should read thisfield after ahost error interrupt (HOSTPEND) todetermine theerror. Host
error interrupts require hardware reset inorder torecover.
0 Noerror.
1h Reserved
2h Ownership bitnotsetinSOP buffer.
3h Reserved
4h Zero buffer pointer.
5h-Fh Reserved
11 Reserved 0 Reserved
10-8 RXERRCH Receive host error channel. These bitsindicate which receive channel thehost error occurred on.
This field iscleared to0onahost read.
0 The host error occurred onreceive channel 0.
1h The host error occurred onreceive channel 1.
2h The host error occurred onreceive channel 2.
3h The host error occurred onreceive channel 3.
4h The host error occurred onreceive channel 4.
5h The host error occurred onreceive channel 5.
6h The host error occurred onreceive channel 6.
7h The host error occurred onreceive channel 7.
7-3 Reserved 0 Reserved
2 RXQOSACT Receive Quality ofService (QOS) active bit.When asserted, indicates thatreceive quality ofservice
isenabled andthatatleast onechannel freebuffer count (RXnFREEBUFFER) isless than orequal
totheRXFILTERLOWTHRESH value.
0 Receive quality ofservice isdisabled.
1 Receive quality ofservice isenabled.
1 RXFLOWACT Receive flow control active bit.When asserted, atleast onechannel freebuffer count
(RXnFREEBUFFER) isless than orequal tothechannel 'scorresponding RXnFILTERTHRESH
value.
0 Receive flow control isinactive.
1 Receive flow control isactive.
0 TXFLOWACT Transmit flow control active bit.When asserted, thisbitindicates thatthepause time period isbeing
observed forareceived pause frame. Nonew transmissions willbegin while thisbitisasserted,
except forthetransmission ofpause frames. Any transmission inprogress when thisbitisasserted
willcomplete.
0 Transmit flow control isinactive.
1 Transmit flow control isactive.

<!-- Page 1908 -->

EMAC Module Registers www.ti.com
1908 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.31 Emulation Control Register (EMCONTROL)
The emulation control register (EMCONTROL) isshown inFigure 32-72 anddescribed inTable 32-70 .
Figure 32-72. Emulation Control Register (EMCONTROL) (offset =168h)
31 16
Reserved
R-0
15 2 1 0
Reserved SOFT FREE
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-70. Emulation Control Register (EMCONTROL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 SOFT Emulation softbit.This bitisused inconjunction with FREE bittodetermine theemulation suspend
mode. This bithasnoeffect ifFREE =1.
0 Soft mode isdisabled. EMAC stops immediately during emulation halt.
1 Soft mode isenabled. During emulation halt, EMAC stops after completion ofcurrent operation.
0 FREE Emulation free bit.This bitisused inconjunction with SOFT bittodetermine theemulation suspend
mode.
0 Free-running mode isdisabled. During emulation halt, SOFT bitdetermines operation ofEMAC.
1 Free-running mode isenabled. During emulation halt, EMAC continues tooperate.
32.5.32 FIFO Control Register (FIFOCONTROL)
The FIFO control register (FIFOCONTROL) isshown inFigure 32-73 anddescribed inTable 32-71 .
Figure 32-73. FIFO Control Register (FIFOCONTROL) (offset =16Ch)
31 16
Reserved
R-0
15 2 1 0
Reserved TXCELLTHRESH
R-0 R/W-2h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-71. FIFO Control Register (FIFOCONTROL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1-0 TXCELLTHRESH Transmit FIFO cellthreshold. Indicates thenumber of64-byte packet cells required tobeinthe
transmit FIFO before thepacket transfer isinitiated. Packets with fewer cells willbeinitiated when
thecomplete packet iscontained intheFIFO. The default value is2,but3isalso valid. 0and1are
notvalid values.
0-1h Notavalid value.
2h Two 64-byte packet cells required tobeinthetransmit FIFO.
3h Three 64-byte packet cells required tobeinthetransmit FIFO.

<!-- Page 1909 -->

www.ti.com EMAC Module Registers
1909 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.33 MAC Configuration Register (MACCONFIG)
The MAC configuration register (MACCONFIG) isshown inFigure 32-74 anddescribed inTable 32-72 .
Figure 32-74. MAC Configuration Register (MACCONFIG) (offset =170h)
31 24 23 16
TXCELLDEPTH RXCELLDEPTH
R-3h R-3h
15 8 7 0
ADDRESSTYPE MACCFIG
R-2h R-2h
LEGEND: R=Read only; -n=value after reset
Table 32-72. MAC Configuration Register (MACCONFIG) Field Descriptions
Bit Field Value Description
31-24 TXCELLDEPTH 3h Transmit celldepth. These bitsindicate thenumber ofcells inthetransmit FIFO.
23-16 RXCELLDEPTH 3h Receive celldepth. These bitsindicate thenumber ofcells inthereceive FIFO.
15-8 ADDRESSTYPE 2h Address type.
7-0 MACCFIG 2h MAC configuration value.
32.5.34 Soft Reset Register (SOFTRESET)
The softreset register (SOFTRESET) isshown inFigure 32-75 anddescribed inTable 32-73 .
Figure 32-75. Soft Reset Register (SOFTRESET) (offset =174h)
31 16
Reserved
R-0
15 1 0
Reserved SOFTRESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-73. Soft Reset Register (SOFTRESET) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reserved
0 SOFTRESET Software reset. Writing a1tothisbitcauses theEMAC logic tobereset. Software reset occurs
when thereceive andtransmit DMA controllers areinanidlestate toavoid locking upthe
Configuration bus. After writing a1tothisbit,itmay bepolled todetermine ifthereset has
occurred. Ifa1isread, thereset hasnotyetoccurred. Ifa0isread, then areset hasoccurred.
0 Asoftware reset hasnotoccurred.
1 Asoftware reset hasoccurred.

<!-- Page 1910 -->

EMAC Module Registers www.ti.com
1910 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.35 MAC Source Address Low Bytes Register (MACSRCADDRLO)
The MAC source address lowbytes register (MACSRCADDRLO) isshown inFigure 32-76 anddescribed
inTable 32-74 .
Figure 32-76. MAC Source Address Low Bytes Register (MACSRCADDRLO) (offset =1D0h)
31 16
Reserved
R-0
15 8 7 0
MACSRCADDR0 MACSRCADDR1
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-74. MAC Source Address Low Bytes Register (MACSRCADDRLO) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-8 MACSRCADDR0 0-FFh MAC source address lower 8-0bits(byte 0).
7-0 MACSRCADDR1 0-FFh MAC source address bits15-8 (byte 1).
32.5.36 MAC Source Address High Bytes Register (MACSRCADDRHI)
The MAC source address high bytes register (MACSRCADDRHI) isshown inFigure 32-77 anddescribed
inTable 32-75 .
Figure 32-77. MAC Source Address High Bytes Register (MACSRCADDRHI) (offset =1D4h)
31 24 23 16
MACSRCADDR2 MACSRCADDR3
R/W-0 R/W-0
15 8 7 0
MACSRCADDR4 MACSRCADDR5
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-75. MAC Source Address High Bytes Register (MACSRCADDRHI) Field Descriptions
Bit Field Value Description
31-24 MACSRCADDR2 0-FFh MAC source address bits23-16 (byte 2).
23-16 MACSRCADDR3 0-FFh MAC source address bits31-24 (byte 3).
15-8 MACSRCADDR4 0-FFh MAC source address bits39-32 (byte 4).
7-0 MACSRCADDR5 0-FFh MAC source address bits47-40 (byte 5).

<!-- Page 1911 -->

www.ti.com EMAC Module Registers
1911 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.37 MAC Hash Address Register 1(MACHASH1)
The MAC hash registers allow group addressed frames tobeaccepted onthebasis ofahash function of
theaddress. The hash function creates a6-bit data value (Hash_fun) from the48-bit destination address
(DA) asfollows:
Hash_fun(0)=DA(0) XOR DA(6) XOR DA(12) XOR DA(18) XOR DA(24) XOR DA(30) XOR DA(36) XOR DA(42);
Hash_fun(1)=DA(1) XOR DA(7) XOR DA(13) XOR DA(19) XOR DA(25) XOR DA(31) XOR DA(37) XOR DA(43);
Hash_fun(2)=DA(2) XOR DA(8) XOR DA(14) XOR DA(20) XOR DA(26) XOR DA(32) XOR DA(38) XOR DA(44);
Hash_fun(3)=DA(3) XOR DA(9) XOR DA(15) XOR DA(21) XOR DA(27) XOR DA(33) XOR DA(39) XOR DA(45);
Hash_fun(4)=DA(4) XOR DA(10) XOR DA(16) XOR DA(22) XOR DA(28) XOR DA(34) XOR DA(40) XOR DA(46);
Hash_fun(5)=DA(5) XOR DA(11) XOR DA(17) XOR DA(23) XOR DA(29) XOR DA(35) XOR DA(41) XOR DA(47);
This function isused asanoffset intoa64-bit hash table stored inMACHASH1 andMACHASH2 that
indicates whether aparticular address should beaccepted ornot.
The MAC hash address register 1(MACHASH1) isshown inFigure 32-78 anddescribed inTable 32-76 .
Figure 32-78. MAC Hash Address Register 1(MACHASH1) (offset =1D8h)
31 0
MACHASH1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 32-76. MAC Hash Address Register 1(MACHASH1) Field Descriptions
Bit Field Description
31-0 MACHASH1 Least-significant 32bitsofthehash table corresponding tohash values 0to31.Ifahash table bitisset,
then agroup address thathashes tothatbitindex isaccepted.
32.5.38 MAC Hash Address Register 2(MACHASH2)
The MAC hash address register 2(MACHASH2) isshown inFigure 32-79 anddescribed inTable 32-77 .
Figure 32-79. MAC Hash Address Register 2(MACHASH2) (offset =1DCh)
31 0
MACHASH2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 32-77. MAC Hash Address Register 2(MACHASH2) Field Descriptions
Bit Field Description
31-0 MACHASH2 Most-significant 32bitsofthehash table corresponding tohash values 32to63.Ifahash table bitisset,
then agroup address thathashes tothatbitindex isaccepted.

<!-- Page 1912 -->

EMAC Module Registers www.ti.com
1912 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.39 Back OffTest Register (BOFFTEST)
The back offtestregister (BOFFTEST) isshown inFigure 32-80 anddescribed inTable 32-78 .
Figure 32-80. Back OffRandom Number Generator Test Register (BOFFTEST) (offset =1E0h)
31 26 25 16
Reserved RNDNUM
R-0 R-0
15 12 11 10 9 0
COLLCOUNT Reserved TXBACKOFF
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-78. Back OffTest Register (BOFFTEST) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reserved
25-16 RNDNUM 0-3FFh Backoff random number generator. This field allows theBackoff Random Number Generator tobe
read. Reading thisfield returns thegenerator 'scurrent value. The value isreset to0andbegins
counting ontheclock after thedeassertion ofreset.
15-12 COLLCOUNT 0-Fh Collision count. These bitsindicate thenumber ofcollisions thecurrent frame hasexperienced.
11-10 Reserved 0 Reserved
9-0 TXBACKOFF 0-3FFh Backoff count. This field allows thecurrent value ofthebackoff counter tobeobserved fortest
purposes. This field isloaded automatically according tothebackoff algorithm, andisdecremented
by1foreach slottime after thecollision.
32.5.40 Transmit Pacing Algorithm Test Register (TPACETEST)
The transmit pacing algorithm testregister (TPACETEST) isshown inFigure 32-81 anddescribed in
Table 32-79 .
Figure 32-81. Transmit Pacing Algorithm Test Register (TPACETEST) (offset =1E4h)
31 16
Reserved
R-0
15 5 4 0
Reserved PACEVAL
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 32-79. Transmit Pacing Algorithm Test Register (TPACETEST) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reserved
4-0 PACEVAL 0-1Fh Pacing register current value. Anonzero value inthisfield indicates thattransmit pacing isactive. A
transmit frame collision ordeferral causes PACEVAL tobeloaded with 1Fh (31); good frame
transmissions (with nocollisions ordeferrals) cause PACEVAL tobedecremented down to0.When
PACEVAL isnonzero, thetransmitter delays four Inter Packet Gaps between new frame transmissions
after each successfully transmitted frame thathadnodeferrals orcollisions. Ifatransmit frame is
deferred orsuffers acollision, theIPG time isnotstretched tofour times thenormal value. Transmit
pacing helps reduce capture effects, which improves overall network bandwidth.

<!-- Page 1913 -->

www.ti.com EMAC Module Registers
1913 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.41 Receive Pause Timer Register (RXPAUSE)
The receive pause timer register (RXPAUSE) isshown inFigure 32-82 anddescribed inTable 32-80 .
Figure 32-82. Receive Pause Timer Register (RXPAUSE) (offset =1E8h)
31 16
Reserved
R-0
15 0
PAUSETIMER
R-0
LEGEND: R=Read only; -n=value after reset
Table 32-80. Receive Pause Timer Register (RXPAUSE) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-0 PAUSETIMER 0-FFh Receive pause timer value. These bitsallow thecontents ofthereceive pause timer tobe
observed. The receive pause timer isloaded with FF00h when theEMAC sends anoutgoing pause
frame (with pause time ofFFFFh). The receive pause timer isdecremented atslottime intervals. If
thereceive pause timer decrements to0,then another outgoing pause frame issent andthe
load/decrement process isrepeated.
32.5.42 Transmit Pause Timer Register (TXPAUSE)
The transmit pause timer register (TXPAUSE) isshown inFigure 32-83 anddescribed inTable 32-81 .
Figure 32-83. Transmit Pause Timer Register (TXPAUSE) (offset =1ECh)
31 16
Reserved
R-0
15 0
PAUSETIMER
R-0
LEGEND: R=Read only; -n=value after reset
Table 32-81. Transmit Pause Timer Register (TXPAUSE) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved
15-0 PAUSETIMER 0-FFh Transmit pause timer value. These bitsallow thecontents ofthetransmit pause timer tobe
observed. The transmit pause timer isloaded byareceived (incoming) pause frame, andthen
decremented atslottime intervals down to0,atwhich time EMAC transmit frames areagain
enabled.

<!-- Page 1914 -->

EMAC Module Registers www.ti.com
1914 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.43 MAC Address Low Bytes Register (MACADDRLO)
The MAC address lowbytes register used inreceive address matching (MACADDRLO), isshown in
Figure 32-84 anddescribed inTable 32-82 .
Figure 32-84. MAC Address Low Bytes Register (MACADDRLO) (offset =500h)
31 21 20 19 18 16
Reserved VALID MATCHFILT CHANNEL
R-0 R/W-x R/W-x R/W-x
15 8 7 0
MACADDR0 MACADDR1
R/W-x R/W-x
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; -x=value isindeterminate after reset
Table 32-82. MAC Address Low Bytes Register (MACADDRLO) Field Descriptions
Bit Field Value Description
31-21 Reserved 0 Reserved
20 VALID Address valid bit.This bitshould becleared to0forunused address channels.
0 Address isnotvalid andwillnotbeused formatching orfiltering incoming packets.
1 Address isvalid andwillbeused formatching orfiltering incoming packets.
19 MATCHFILT Match orfilter bit.
0 The address willbeused (iftheVALID bitisset)tofilter incoming packet addresses.
1 The address willbeused (iftheVALID bitisset)tomatch incoming packet addresses.
18-16 CHANNEL 0-7h Channel select. Determines which receive channel avalid address match willbetransferred to.The
channel isadon'tcare ifMATCHFILT iscleared to0.
15-8 MACADDR0 0-FFh MAC address lower 8-0bits(byte 0).
7-0 MACADDR1 0-FFh MAC address bits15-8 (byte 1).

<!-- Page 1915 -->

www.ti.com EMAC Module Registers
1915 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.44 MAC Address High Bytes Register (MACADDRHI)
The MAC address high bytes register used inreceive address matching (MACADDRHI) isshown in
Figure 32-85 anddescribed inTable 32-83 .
Figure 32-85. MAC Address High Bytes Register (MACADDRHI) (offset =504h)
31 24 23 16
MACADDR2 MACADDR3
R/W-x R/W-x
15 8 7 0
MACADDR4 MACADDR5
R/W-x R/W-x
LEGEND: R/W =Read/Write; -n=value after reset; -x=value isindeterminate after reset
Table 32-83. MAC Address High Bytes Register (MACADDRHI) Field Descriptions
Bit Field Value Description
31-24 MACADDR2 0-FFh MAC source address bits23-16 (byte 2).
23-16 MACADDR3 0-FFh MAC source address bits31-24 (byte 3).
15-8 MACADDR4 0-FFh MAC source address bits39-32 (byte 4).
7-0 MACADDR5 0-FFh MAC source address bits47-40 (byte 5).Bit40isthegroup bit.Itisforced to0andread as0.
Therefore, only unicast addresses arerepresented intheaddress table.
32.5.45 MAC Index Register (MACINDEX)
The MAC index register (MACINDEX) isshown inFigure 32-86 anddescribed inTable 32-84 .
Figure 32-86. MAC Index Register (MACINDEX) (offset =508h)
31 16
Reserved
R-0
15 3 2 0
Reserved MACINDEX
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 32-84. MAC Index Register (MACINDEX) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved
2-0 MACINDEX 0-7h MAC address index. Alleight addresses share theupper 40bits. Only thelower byte isunique foreach
address. Anaddress iswritten byfirstwriting theaddress number (channel) intotheMACINDEX
register. The upper 32bitsoftheaddress arethen written totheMACADDRHI register, which is
followed bywriting thelower 16bitsoftheaddress totheMACADDRLO register. Since alleight
addresses share theupper 40bitsoftheaddress, theMACADDRHI register only needs tobewritten
thefirsttime.

<!-- Page 1916 -->

EMAC Module Registers www.ti.com
1916 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.46 Transmit Channel DMA Head Descriptor Pointer Registers (TX0HDP-TX7HDP)
The transmit channel 0-7DMA head descriptor pointer register (TXnHDP) isshown inFigure 32-87 and
described inTable 32-85 .
Figure 32-87. Transmit Channel nDMA Head Descriptor Pointer Register (TXnHDP)
(offset =600h-61Ch)
31 0
TXnHDP
R/W-x
LEGEND: R/W =Read/Write; -n=value after reset; -x=value isindeterminate after reset
Table 32-85. Transmit Channel nDMA Head Descriptor Pointer Register (TXnHDP)
Field Descriptions
Bit Field Description
31-0 TXnHDP Transmit channel nDMA Head Descriptor pointer. Writing atransmit DMA buffer descriptor address toahead
pointer location initiates transmit DMA operations inthequeue fortheselected channel. Writing tothese
locations when they arenonzero isanerror (except atreset). Host software must initialize these locations to0
onreset.
32.5.47 Receive Channel DMA Head Descriptor Pointer Registers (RX0HDP-RX7HDP)
The receive channel 0-7DMA head descriptor pointer register (RXnHDP) isshown inFigure 32-88 and
described inTable 32-86 .
Figure 32-88. Receive Channel nDMA Head Descriptor Pointer Register (RXnHDP)
(offset =620h-63Ch)
31 0
RXnHDP
R/W-x
LEGEND: R/W =Read/Write; -n=value after reset; -x=value isindeterminate after reset
Table 32-86. Receive Channel nDMA Head Descriptor Pointer Register (RXnHDP)
Field Descriptions
Bit Field Description
31-0 RXnHDP Receive channel nDMA Head Descriptor pointer. Writing areceive DMA buffer descriptor address tothis
location allows receive DMA operations intheselected channel when achannel frame isreceived. Writing to
these locations when they arenonzero isanerror (except atreset). Host software must initialize these
locations to0onreset.

<!-- Page 1917 -->

www.ti.com EMAC Module Registers
1917 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.48 Transmit Channel Completion Pointer Registers (TX0CP-TX7CP)
The transmit channel 0-7completion pointer register (TXnCP) isshown inFigure 32-89 anddescribed in
Table 32-87 .
Figure 32-89. Transmit Channel nCompletion Pointer Register (TXnCP)(offset =640h-65Ch)
31 0
TXnCP
R/W-x
LEGEND: R/W =Read/Write; -n=value after reset; -x=value isindeterminate after reset
Table 32-87. Transmit Channel nCompletion Pointer Register (TXnCP)Field Descriptions
Bit Field Description
31-0 TXnCP Transmit channel ncompletion pointer register iswritten bythehost with thebuffer descriptor address forthelast
buffer processed bythehost during interrupt processing. The EMAC uses thevalue written todetermine ifthe
interrupt should bedeasserted.
32.5.49 Receive Channel Completion Pointer Registers (RX0CP-RX7CP)
The receive channel 0-7completion pointer register (RXnCP) isshown inFigure 32-90 anddescribed in
Table 32-88 .
Figure 32-90. Receive Channel nCompletion Pointer Register (RXnCP)(offset =660h-67Ch)
31 0
RXnCP
R/W-x
LEGEND: R/W =Read/Write; -n=value after reset; -x=value isindeterminate after reset
Table 32-88. Receive Channel nCompletion Pointer Register (RXnCP)Field Descriptions
Bit Field Description
31-0 RXnCP Receive channel ncompletion pointer register iswritten bythehost with thebuffer descriptor address forthelast
buffer processed bythehost during interrupt processing. The EMAC uses thevalue written todetermine ifthe
interrupt should bedeasserted.

<!-- Page 1918 -->

EMAC Module Registers www.ti.com
1918 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50 Network Statistics Registers
The EMAC hasasetofstatistics thatrecord events associated with frame traffic. The statistics values are
cleared to0,38clocks after therising edge ofreset. When theGMIIEN bitintheMACCONTROL register
isset,allstatistics registers (see Figure 32-91 )arewrite-to-decrement. The value written issubtracted
from theregister value with theresult stored intheregister. Ifavalue greater than thestatistics value is
written, then zero iswritten totheregister (writing FFFF FFFFh clears astatistics location). When the
GMIIEN bitiscleared, allstatistics registers areread/write (normal write direct, sowriting 0000 0000h
clears astatistics location). Allwrite accesses must be32-bit accesses.
The statistics interrupt (STATPEND) isissued, ifenabled, when anystatistics value isgreater than or
equal to8000 0000h. The statistics interrupt isremoved bywriting todecrement anystatistics value
greater than 8000 0000h. The statistics aremapped intointernal memory space andare32-bits wide. All
statistics rollover from FFFF FFFFh to0000 0000h.
Figure 32-91. Statistics Register
31 0
COUNT
R/WD-0
LEGEND: R/W =Read/Write; WD=Write todecrement; -n=value after reset
32.5.50.1 Good Receive Frames Register (RXGOODFRAMES) (offset =200h)
The total number ofgood frames received ontheEMAC. Agood frame isdefined ashaving allofthe
following:
*Any data orMAC control frame thatmatched aunicast, broadcast, ormulticast address, ormatched
duetopromiscuous mode
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.2 Broadcast Receive Frames Register (RXBCASTFRAMES) (offset =204h)
The total number ofgood broadcast frames received ontheEMAC. Agood broadcast frame isdefined as
having allofthefollowing:
*Any data orMAC control frame thatwas destined foraddress FF-FF-FF-FF-FF-FFh only
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.3 Multicast Receive Frames Register (RXMCASTFRAMES) (offset =208h)
The total number ofgood multicast frames received ontheEMAC. Agood multicast frame isdefined as
having allofthefollowing:
*Any data orMAC control frame thatwas destined foranymulticast address other than FF-FF-FF-FF-
FF-FFh
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.

<!-- Page 1919 -->

www.ti.com EMAC Module Registers
1919 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.4 Pause Receive Frames Register (RXPAUSEFRAMES) (offset =20Ch)
The total number ofIEEE 802.3X pause frames received bytheEMAC (whether acted upon ornot). A
pause frame isdefined ashaving allofthefollowing:
*Contained anyunicast, broadcast, ormulticast address
*Contained thelength/type field value 88.08h andtheopcode 0001h
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noCRC error, alignment error, orcode error
*Pause-frames hadbeen enabled ontheEMAC (TXFLOWEN bitissetinMACCONTROL).
The EMAC could have been ineither half-duplex orfull-duplex mode. See Section 32.2.6.5 fordefinitions
ofalignment, code, andCRC errors. Overruns have noeffect onthisstatistic.
32.5.50.5 Receive CRC Errors Register (RXCRCERRORS) (offset =210h)
The total number offrames received ontheEMAC thatexperienced aCRC error. Aframe with CRC
errors isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noalignment orcode error
*Had aCRC error. ACRC error isdefined ashaving allofthefollowing:
-Aframe containing aneven number ofnibbles
-Fails theframe check sequence test
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.6 Receive Alignment/Code Errors Register (RXALIGNCODEERRORS) (offset =214h)
The total number offrames received ontheEMAC thatexperienced analignment error orcode error.
Such aframe isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was oflength 64toRXMAXLEN bytes inclusive
*Had either analignment error oracode error
-Analignment error isdefined ashaving allofthefollowing:
*Aframe containing anoddnumber ofnibbles
*Fails theframe check sequence test, ifthefinal nibble isignored
-Acode error isdefined asaframe thathasbeen discarded because theEMACs MII_RXER pinis
driven with a1foratleast onebit-time's duration atanypoint during theframe's reception.
Overruns have noeffect onthisstatistic.
CRC alignment orcode errors canbecalculated bysumming receive alignment errors, receive code
errors, andreceive CRC errors.

<!-- Page 1920 -->

EMAC Module Registers www.ti.com
1920 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.7 Receive Oversized Frames Register (RXOVERSIZED) (offset =218h)
The total number ofoversized frames received ontheEMAC. Anoversized frame isdefined ashaving all
ofthefollowing:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was greater than RXMAXLEN inbytes
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.8 Receive Jabber Frames Register (RXJABBER) (offset =21Ch)
The total number ofjabber frames received ontheEMAC. Ajabber frame isdefined ashaving allofthe
following:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was greater than RXMAXLEN bytes long
*Had aCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.9 Receive Undersized Frames Register (RXUNDERSIZED) (offset =220h)
The total number ofundersized frames received ontheEMAC. Anundersized frame isdefined ashaving
allofthefollowing:
*Was anydata frame thatmatched aunicast, broadcast, ormulticast address, ormatched dueto
promiscuous mode
*Was less than 64bytes long
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.10 Receive Frame Fragments Register (RXFRAGMENTS) (offset =224h)
The total number offrame fragments received ontheEMAC. Aframe fragment isdefined ashaving allof
thefollowing:
*Any data frame (address matching does notmatter)
*Was less than 64bytes long
*Had aCRC error, alignment error, orcode error
*Was nottheresult ofacollision caused byhalfduplex, collision based flow control
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.11 Filtered Receive Frames Register (RXFILTERED) (offset =228h)
The total number offrames received ontheEMAC thattheEMAC address matching process indicated
should bediscarded. Such aframe isdefined ashaving allofthefollowing:
*Was anydata frame (not MAC control frame) destined foranyunicast, broadcast, ormulticast address
*Didnotexperience anyCRC error, alignment error, code error
*The address matching process decided thattheframe should bediscarded (filtered) because itdidnot
match theunicast, broadcast, ormulticast address, anditdidnotmatch duetopromiscuous mode.

<!-- Page 1921 -->

www.ti.com EMAC Module Registers
1921 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO ModuleTodetermine thenumber ofreceive frames discarded bytheEMAC foranyreason, sum thefollowing
statistics (promiscuous mode disabled):
*Receive fragments
*Receive undersized frames
*Receive CRC errors
*Receive alignment/code errors
*Receive jabbers
*Receive overruns
*Receive filtered frames
This may notbeanexact count because thereceive overruns statistic isindependent oftheother
statistics, soifanoverrun occurs atthesame time asoneoftheother discard reasons, then theabove
sum double-counts thatframe.
32.5.50.12 Receive QOS Filtered Frames Register (RXQOSFILTERED) (offset =22Ch)
The total number offrames received ontheEMAC thatwere filtered duetoreceive quality ofservice
(QOS) filtering. Such aframe isdefined ashaving allofthefollowing:
*Any data orMAC control frame thatmatched aunicast, broadcast, ormulticast address, ormatched
duetopromiscuous mode
*The frame destination channel flow control threshold register (RXnFLOWTHRESH) value was greater
than orequal tothechannel's corresponding free buffer register (RXnFREEBUFFER) value
*Was oflength 64toRXMAXLEN
*RXQOSEN bitissetinRXMBPENABLE
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.13 Receive Octet Frames Register (RXOCTETS) (offset =230h)
The total number ofbytes inallgood frames received ontheEMAC. Agood frame isdefined ashaving all
ofthefollowing:
*Any data orMAC control frame thatmatched aunicast, broadcast, ormulticast address, ormatched
duetopromiscuous mode
*Was oflength 64toRXMAXLEN bytes inclusive
*Had noCRC error, alignment error, orcode error
See Section 32.2.6.5 fordefinitions ofalignment, code, andCRC errors. Overruns have noeffect onthis
statistic.
32.5.50.14 Good Transmit Frames Register (TXGOODFRAMES) (offset =234h)
The total number ofgood frames transmitted ontheEMAC. Agood frame isdefined ashaving allofthe
following:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Was anylength
*Had nolateorexcessive collisions, nocarrier loss, andnounderrun

<!-- Page 1922 -->

EMAC Module Registers www.ti.com
1922 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.15 Broadcast Transmit Frames Register (TXBCASTFRAMES) (offset =238h)
The total number ofgood broadcast frames transmitted ontheEMAC. Agood broadcast frame isdefined
ashaving allofthefollowing:
*Any data orMAC control frame destined foraddress FF-FF-FF-FF-FF-FFh only
*Was ofanylength
*Had nolateorexcessive collisions, nocarrier loss, andnounderrun
32.5.50.16 Multicast Transmit Frames Register (TXMCASTFRAMES) (offset =23Ch)
The total number ofgood multicast frames transmitted ontheEMAC. Agood multicast frame isdefined as
having allofthefollowing:
*Any data orMAC control frame destined foranymulticast address other than FF-FF-FF-FF-FF-FFh
*Was ofanylength
*Had nolateorexcessive collisions, nocarrier loss, andnounderrun
32.5.50.17 Pause Transmit Frames Register (TXPAUSEFRAMES) (offset =240h)
The total number ofIEEE 802.3X pause frames transmitted bytheEMAC. Pause frames cannot underrun
orcontain aCRC error because they arecreated inthetransmitting MAC, sothese error conditions have
noeffect onthisstatistic. Pause frames sent bysoftware arenotincluded inthiscount. Since pause
frames areonly transmitted infull-duplex mode, carrier loss andcollisions have noeffect onthisstatistic.
Transmitted pause frames arealways 64-byte multicast frames soappear inthemulticast transmit frames
register and64octect frames register statistics.
32.5.50.18 Deferred Transmit Frames Register (TXDEFERRED) (offset =244h)
The total number offrames transmitted ontheEMAC thatfirstexperienced deferment. Such aframe is
defined ashaving allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*Had nocarrier loss andnounderrun
*Experienced nocollisions before being successfully transmitted
*Found themedium busy when transmission was firstattempted, sohadtowait.
CRC errors have noeffect onthisstatistic.
32.5.50.19 Transmit Collision Frames Register (TXCOLLISION) (offset =248h)
The total number oftimes thattheEMAC experienced acollision. Collisions occur under two
circumstances:
*When atransmit data orMAC control frame hasallofthefollowing:
-Was destined foranyunicast, broadcast, ormulticast address
-Was anysize
-Had nocarrier loss andnounderrun
-Experienced acollision. Ajamsequence issent forevery non-late collision, sothisstatistic
increments oneach occasion ifaframe experiences multiple collisions (and increments onlate
collisions).
*When theEMAC isinhalf-duplex mode, flow control isactive, andaframe reception begins.
CRC errors have noeffect onthisstatistic.

<!-- Page 1923 -->

www.ti.com EMAC Module Registers
1923 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.20 Transmit Single Collision Frames Register (TXSINGLECOLL) (offset =24Ch)
The total number offrames transmitted ontheEMAC thatexperienced exactly onecollision. Such aframe
isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*Had nocarrier loss andnounderrun
*Experienced onecollision before successful transmission. The collision was notlate.
CRC errors have noeffect onthisstatistic.
32.5.50.21 Transmit Multiple Collision Frames Register (TXMULTICOLL) (offset =250h)
The total number offrames transmitted ontheEMAC thatexperienced multiple collisions. Such aframe is
defined ashaving allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*Had nocarrier loss andnounderrun
*Experienced 2to15collisions before being successfully transmitted. None ofthecollisions were late.
CRC errors have noeffect onthisstatistic.
32.5.50.22 Transmit Excessive Collision Frames Register (TXEXCESSIVECOLL) (offset =254h)
The total number offrames when transmission was abandoned duetoexcessive collisions. Such aframe
isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*Had nocarrier loss andnounderrun
*Experienced 16collisions before abandoning allattempts attransmitting theframe. None ofthe
collisions were late.
CRC errors have noeffect onthisstatistic.
32.5.50.23 Transmit Late Collision Frames Register (TXLATECOLL) (offset =258h)
The total number offrames when transmission was abandoned duetoalatecollision. Such aframe is
defined ashaving allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*Had nocarrier loss andnounderrun
*Experienced acollision later than 512bit-times intothetransmission. There may have been upto15
previous (non-late) collisions thathadpreviously required thetransmission tobereattempted. The late
collisions statistic dominates over thesingle, multiple, andexcessive collisions statistics. Ifalate
collision occurs, theframe isnotcounted inanyofthese other three statistics.
CRC errors, carrier loss, andunderrun have noeffect onthisstatistic.
32.5.50.24 Transmit Underrun Error Register (TXUNDERRUN) (offset =25Ch)
The number offrames sent bytheEMAC thatexperienced FIFO underrun. Late collisions, CRC errors,
carrier loss, andunderrun have noeffect onthisstatistic.

<!-- Page 1924 -->

EMAC Module Registers www.ti.com
1924 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.25 Transmit Carrier Sense Errors Register (TXCARRIERSENSE) (offset =260h)
The total number offrames ontheEMAC thatexperienced carrier loss. Such aframe isdefined ashaving
allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address
*Was anysize
*The carrier sense condition was lostornever asserted when transmitting theframe (the frame isnot
retransmitted)
CRC errors andunderrun have noeffect onthisstatistic.
32.5.50.26 Transmit Octet Frames Register (TXOCTETS) (offset =264h)
The total number ofbytes inallgood frames transmitted ontheEMAC. Agood frame isdefined ashaving
allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Was anylength
*Had nolateorexcessive collisions, nocarrier loss, andnounderrun
32.5.50.27 Transmit andReceive 64Octet Frames Register (FRAME64) (offset =268h)
The total number of64-byte frames received andtransmitted ontheEMAC. Such aframe isdefined as
having allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was exactly 64-bytes long. (Iftheframe was being transmitted andexperienced carrier loss that
resulted inaframe ofthissize being transmitted, then theframe isrecorded inthisstatistic).
CRC errors, alignment/code errors, andoverruns donotaffect therecording offrames inthisstatistic.
32.5.50.28 Transmit andReceive 65to127Octet Frames Register (FRAME65T127) (offset =26Ch)
The total number of65-byte to127-byte frames received andtransmitted ontheEMAC. Such aframe is
defined ashaving allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was 65-bytes to127-bytes long
CRC errors, alignment/code errors, underruns, andoverruns donotaffect therecording offrames inthis
statistic.
32.5.50.29 Transmit andReceive 128to255Octet Frames Register (FRAME128T255) (offset =270h)
The total number of128-byte to255-byte frames received andtransmitted ontheEMAC. Such aframe is
defined ashaving allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was 128-bytes to255-bytes long
CRC errors, alignment/code errors, underruns, andoverruns donotaffect therecording offrames inthis
statistic.

<!-- Page 1925 -->

www.ti.com EMAC Module Registers
1925 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.30 Transmit andReceive 256to511Octet Frames Register (FRAME256T511) (offset =274h)
The total number of256-byte to511-byte frames received andtransmitted ontheEMAC. Such aframe is
defined ashaving allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was 256-bytes to511-bytes long
CRC errors, alignment/code errors, underruns, andoverruns donotaffect therecording offrames inthis
statistic.
32.5.50.31 Transmit andReceive 512to1023 Octet Frames Register (FRAME512T1023) (offset =278h)
The total number of512-byte to1023-byte frames received andtransmitted ontheEMAC. Such aframe is
defined ashaving allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was 512-bytes to1023-bytes long
CRC errors, alignment/code errors, andoverruns donotaffect therecording offrames inthisstatistic.
32.5.50.32 Transmit andReceive 1024 toRXMAXLEN Octet Frames Register (FRAME1024TUP)
(offset =27Ch)
The total number of1024-byte toRXMAXLEN-byte frames received andtransmitted ontheEMAC. Such a
frame isdefined ashaving allofthefollowing:
*Any data orMAC control frame thatwas destined foranyunicast, broadcast, ormulticast address
*Didnotexperience latecollisions, excessive collisions, underrun, orcarrier sense error
*Was 1024-bytes toRXMAXLEN-bytes long
CRC/alignment/code errors, underruns, andoverruns donotaffect frame recording inthisstatistic.
32.5.50.33 Network Octet Frames Register (NETOCTETS) (offset =280h)
The total number ofbytes offrame data received andtransmitted ontheEMAC. Each frame counted has
allofthefollowing:
*Was anydata orMAC control frame destined foranyunicast, broadcast, ormulticast address (address
match does notmatter)
*Was ofanysize (including less than 64-byte andgreater than RXMAXLEN-byte frames)
Also counted inthisstatistic is:
*Every byte transmitted before acarrier-loss was experienced
*Every byte transmitted before each collision was experienced (multiple retries arecounted each time)
*Every byte received iftheEMAC isinhalf-duplex mode until ajamsequence was transmitted toinitiate
flow control. (The jamsequence isnotcounted toprevent double-counting).
Error conditions such asalignment errors, CRC errors, code errors, overruns, andunderruns donotaffect
therecording ofbytes inthisstatistic. The objective ofthisstatistic istogive areasonable indication of
Ethernet utilization.

<!-- Page 1926 -->

EMAC Module Registers www.ti.com
1926 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEMAC/MDIO Module32.5.50.34 Receive FIFO orDMA Start ofFrame Overruns Register (RXSOFOVERRUNS) (offset =284h)
The total number offrames received ontheEMAC thathadeither aFIFO orDMA start offrame (SOF)
overrun. AnSOF overrun frame isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was ofanysize (including less than 64-byte andgreater than RXMAXLEN-byte frames)
*The EMAC was unable toreceive itbecause itdidnothave theresources toreceive it(cell FIFO fullor
noDMA buffer available atthestart oftheframe).
CRC errors, alignment errors, andcode errors have noeffect onthisstatistic.
32.5.50.35 Receive FIFO orDMA Middle ofFrame Overruns Register (RXMOFOVERRUNS) (offset =288h)
The total number offrames received ontheEMAC thathadeither aFIFO orDMA middle offrame (MOF)
overrun. AnMOF overrun frame isdefined ashaving allofthefollowing:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was ofanysize (including less than 64-byte andgreater than RXMAXLEN-byte frames)
*The EMAC was unable toreceive itbecause itdidnothave theresources toreceive it(cell FIFO fullor
noDMA buffer available after theframe was successfully started -noSOF overrun).
CRC errors, alignment errors, andcode errors have noeffect onthisstatistic.
32.5.50.36 Receive DMA Overruns Register (RXDMAOVERRUNS) (offset =28Ch)
The total number offrames received ontheEMAC thathadeither aDMA start offrame (SOF) overrun or
aDMA middle offrame (MOF) overrun. Areceive DMA overrun frame isdefined ashaving allofthe
following:
*Was anydata orMAC control frame thatmatched aunicast, broadcast, ormulticast address, or
matched duetopromiscuous mode
*Was ofanysize (including less than 64-byte andgreater than RXMAXLEN-byte frames)
*The EMAC was unable toreceive itbecause itdidnothave theDMA buffer resources toreceive it
(zero head descriptor pointer atthestart orduring themiddle oftheframe reception).
CRC errors, alignment errors, andcode errors have noeffect onthisstatistic.