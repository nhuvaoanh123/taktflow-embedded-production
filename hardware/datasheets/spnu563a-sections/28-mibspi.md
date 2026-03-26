# Multi-Buffered Serial Peripheral Interface (MibSPI)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1497-1620

---


<!-- Page 1497 -->

1497 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Chapter 28
SPNU563A -March 2018
Multi-Buffered Serial Peripheral Interface Module (MibSPI)
with Parallel PinOption (MibSPIP)
This chapter provides thespecifications fora16-bit configurable synchronous multi-buffered multi-pin
serial peripheral interface (MibSPI). This chapter also provides thespecifications forMibSPI with Parallel
Pin Option (MibSPIP). The MibSPI isaprogrammable-length shift register used forhigh-speed
communication between external peripherals orother microcontrollers.
Throughout thischapter, allreferences toSPIalso apply toMibSPI/MibSPIP, unless otherwise noted.
NOTE: This chapter describes asuperset implementation oftheMibSPI/SPI modules thatincludes
features andfunctionality thatmay notbeavailable onsome devices. Device-specific content
thatshould bedetermined byreferencing thedatasheet includes DMA functionality, MibSPI
RAM size, number oftransfer groups, number ofchip selects, parallel mode support, and
availability of5-pin operation (SPInENA).
Topic ........................................................................................................................... Page
28.1 Overview ........................................................................................................ 1498
28.2 Basic Operation .............................................................................................. 1500
28.3 Control Registers ............................................................................................ 1535
28.4 Multi-buffer RAM............................................................................................. 1605
28.5 Parity\ECC Memory ......................................................................................... 1612
28.6 MibSPI PinTiming Parameters ......................................................................... 1617

<!-- Page 1498 -->

Overview www.ti.com
1498 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.1 Overview
28.1.1 Features
The MibSPI/SPI isahigh-speed synchronous serial input/output port thatallows aserial bitstream of
programmed length (two to16bits) tobeshifted intoandoutofthedevice ataprogrammed bit-transfer
rate. The MibSPI/SPI isnormally used forcommunication between themicrocontroller andexternal
peripherals oranother microcontroller. Typical applications include interface toexternal I/Oorperipheral
expansion viadevices such asshift registers, display drivers, andanalog-to-digital converters. MibSPI is
anExtension ofSPI. MibSPI works in2modes.
*Compatibility Mode
*Multi-buffer Mode
The Compatibility mode ofMibSPI makes itbehave exactly likethatofSPIandensures fullcompatibility
with thesame. Everything described about compatibility mode ofMibSPI ,inthisdocument, isdirectly
applicable toSPI.
The Multi-buffer mode ofoperation isspecific toMibSPI alone. This feature isnotavailable inSPI.
The MibSPI supports memory fault detection/correction viainternal Parity/ECC circuit. MibSPI is
configurable toinclude ornotinclude Memory Parity/ECC logic during circuit synthesis.
The SPI/MibSPI canbeconfigured inthree pin,four pinorfivepinmode ofoperation. The SPI/MibSPI
allows multiple programmable chip-selects.
The MibSPI hasaprogrammable Multi-buffer array thatenables programed transmission tobecompleted
without CPU intervention. The buffers arecombined indifferent transfer groups thatcould betriggered by
external events (Timers, I/O,andsoon)orbytheinternal tickcounter. The internal tickcounter can
support periodic trigger events. Each buffer oftheMibSPI canbeassociated with different DMA channels
indifferent transfer group, allowing theuser tomove data from/to internal memory to/from external slave
with aminimal CPU interaction.
The SPICLK, SPISIMO, andSPISOMI pins areused inallMibSPI pinmodes. The SPIENA andSPICS
pins areoptional andmay beused ifthepinarepresent onagiven device.
The SPIhasthefollowing attributes:
*16-bit shift register
*Receive buffer register
*8-bit baud clock generator
*Serial clock (SPICLK) I/Opin
*Upto8Slave out,Master in(SPISOMI) I/Opins forfaster data transfers
*SPIenable (SPIENA) pin(4or5-pin mode only)
*Upto6slave chip select (SPICS) pins (4or5-pin mode only)
*SPIpins canbeused asfunctional ordigital Input/Output pins (GIOs)
The SPI/MibSPI allows software toprogram thefollowing options:
*SPISOMI/SPISIMO pindirection configuration
*SPICLK pinsource (external/internal)
*MibSPI pins asfunctional ordigital I/Opins. Foreach Buffer, thefollowing features canbeselected
from four different combinations offormats using thecontrol fields inthebuffer:
-SPICLK frequency
-Character length
-Phase
-Polarity
-Enable/Disable parity fortransmit andreceive
-Enable/Disable timers forChip Select Hold andSetup timers
-Direction ofshifting, MSBit firstorLSBit first

<!-- Page 1499 -->

www.ti.com Overview
1499 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)-Configurable Parallel modes tousemultiple SIMO/SOMI pin
-Configurable number ofChip Selects
InMulti-buffer Mode, inaddition totheprevious features, many other features areconfigurable:
-Number ofbuffers foreach peripheral (ordata source/destination, upto256buffers supported) or
group (upto8groupings)
-Number ofDMA controlled buffers andnumber ofDMA request channels (upto8foreach of
transmit andreceive)
-Triggers foreach groups, trigger types, trigger sources forindividual groups(up to14external
trigger sources and1internal trigger source)
-Number ofDMA transfers foreach buffer (upto65536 forupto8buffers)
-Un-interrupted DMA buffer transfer (NOBREAK buffer)
NOTE: SIMO -Slave InMaster OutPin
SOMI -Slave OutMaster InPin
SPICS -SPIChip Select Pin
SPIENA -SPIEnable Pin.
28.1.2 PinConfigurations
The SPIsupports data connections asshown inTable 28-1.
Table 28-1. PinConfigurations
Pin Master Mode Slave Mode
SPICLK Drives theclock toexternal devices Receives theclock from theexternal master
SPISOMI Receives data from theexternal slave Sends data totheexternal master
SPISIMO Transmits data totheexternal slave Receives data from theexternal master
SPIENA SPIENA disabled:
GIOSPIENA enabled:
Receives ENA signal from
theexternal slaveSPIENA disabled:
GIOSPIENA enabled:
Drives ENA signal from the
external master
SPICS SPICS disabled:
GIOSPICS enabled:
Selects oneormore slave
devicesSPICS disabled:
GIOSPICS enabled:
Receives theCSsignal
from theexternal master
NOTE:
1.When theSPICS signals aredisabled, thechip-select field inthetransmit data isnot
used.
2.When theSPIENA signal isdisabled, theSPIENA pinisignored inmaster mode, and
notdriven aspart oftheSPItransaction inslave mode.

<!-- Page 1500 -->

Overview www.ti.com
1500 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.1.3 MibSPI /SPI Configurations
Table 28-2. MibSPI/SPI Configurations
MibSPIx/SPIx I/Os
MibSPI1 MIBSPI1SIMO[1:0], MIBSPI1SOMI[1:0], MIBSPI1CLK, MIBSPI1nCS[5:0], MIBSPI1nENA
MibSPI2 MIBSPI2SIMO[1:0], MIBSPI2SOMI[1:0], MIBSPI2CLK, MIBSPI2nCS[5:0], MIBSPI2nENA
MibSPI3 MIBSPI3SIMO[1:0], MIBSPI3SOMI[1:0], MIBSPI3CLK, MIBSPI3nCS[5:0], MIBSPI3nENA
MibSPI4 MIBSPI4SIMO[1:0], MIBSPI4SOMI[1:0], MIBSPI4CLK, MIBSPI4nCS[5:0], MIBSPI4nENA
MibSPI5 MIBSPI5SIMO[1:0], MIBSPI5SOMI[1:0], MIBSPI51CLK, MIBSPI5nCS[5:0], MIBSP5nENA
SPI1 SPI1SIMO, ZSPI1SOMI, SPI1CLK, SPI2nCS[1:0], SPI1nENA
SPI2 SPI2SIMO, ZSPI2SOMI, SPI2CLK, SPI2nCS[1:0], SPI2nENA
SPI3 SPI3SIMO, ZSPI3SOMI, SPI3CLK, SPI3nCS[1:0], SPI3nENA
28.2 Basic Operation
This section details thebasic operation principle oftheSPImode andtheMibSPI mode operation ofthe
device.
28.2.1 SPIMode
The SPIcanbeconfigured viasoftware tooperate aseither amaster oraslave. The MASTER bit
(SPIGCR1[0]) selects theconfiguration oftheSPISIMO andSPISOMI pins. CLKMOD bit(SPIGCR1[1])
determines whether aninternal orexternal clock source willbeused.
The slave chip select (SPICS) pins areused when communicating with multiple slave devices or,with a
single slave, todelimit messages containing aleading register address. When awrite occurs toSPIDAT1
inmaster mode, theSPICS pins areautomatically driven toselect thespecified slave.
Handshaking mechanism, provided bytheSPIENA pin,enables aslave SPItodelay thegeneration ofthe
clock signal supplied bythemaster ifitisnotprepared forthenext exchange ofdata.
28.2.1.1 SPIMode Operation Block Diagram
Figure 28-1 shows theSPItransaction hardware. TXBUF andRXBUF areinternal buffers thatare
intended toimprove theoverall throughput ofdata transfer. TXBUF isatransmit buffer, while RXBUF isa
receive buffer.

<!-- Page 1501 -->

1 This is a representative diagram, which shows three-pin mode h ardware.
2 TXBUF, RXBUF, and SHIFT_REGISTER are user-invisible regi sters.
3 SPIDAT0 and SPIDAT1 are user-visible, and are physically mapp ed to the contents of TXBUF.
4 SPISIMO, SPISOMI, SPICLK pin directions depend on the Mast er or Slave Mode.TX shift registerSPIBUF
RXBUFTXBUFPeripheral Write Peripheral Read
TXFULLRXOVRN
INT0RXOVR INT
1616
16RXEMPTYRX INT ENA
Clock polarity Clock phasePrescaleCharlen
SPISIMOSPISOMI
Peripheral clockSPI clock generation logicSPIDAT0/SPIDAT1
TX INT ENA
Kernel FSM
SPICLKMode
generation
logic
CLKMODINT1INT_LVL
RX shift register16
Pin Directions in Slave ModeENA
www.ti.com Basic Operation
1501 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-1. SPIFunctional Logic Diagram
28.2.1.2 Data Flow andHandling forTXandRX
28.2.1.2.1 Data Sequencing when SPIDAT0 orSPIDAT1 isWritten
*Ifboth theTXshift register andTXBUF areempty, then thedata isdirectly copied totheTXshift
register. Fordevices with DMA, ifDMA isenabled, atransmit DMA request (TX_DMA_REQ) is
generated tocause thenext word tobefetched. Iftransmit interrupts areenabled, atransmitter-empty
interrupt isgenerated.
*IftheTXshift register isalready fullorisintheprocess ofshifting andifTXBUF isexpty then thedata
written toSPIDAT0 /SPIDAT1 iscopied toTXBUF andTXFULL flagissetto1atthesame time.
*When ashift operation iscomplete, data from theTXBUF (ifitisfull)iscopied intoTXshift register
andtheTXFULL flagiscleared to0toindicate thatnext data canbefetched. Atransmit DMA request
(ifenabled) oratransmitter-empty interrupt (ifenabled) isgenerated atthesame time.

<!-- Page 1502 -->

Ctrl Field
Kernel FSMRX
BufferStat
FieldINTREQ[1:0]
2TX
BufferCtrl
Field
SPIBUF
Clock polarity Clock PhasePrescaleCharlen
SPISIMOSPISOMISPISCS [7:0]
SPIENA
SPICLK
SPICLK GENERATION LOGICCLKMODStatusMultiBuffer Control
Sequencer FSM
16
161616TRG_SRC[13:0]DMA_REQ[15:0]
1616
16MultiBuffer Logic
RX Shift RegisterTX Shift RegisterSCS_TRIG[14:0]VBUS
Interrupt
Generator
Mode
General
LogicTick
Counter
SPI KernelDMA Control Logic
Trigger Control Logic
VBUS CLOCKMultibuffer Ram
Basic Operation www.ti.com
1502 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.1.2.2 Data Sequencing when AllBits Shifted into RXSHIFT Register
*Ifboth SPIBUF andRXBUF areempty, thereceived data inRXshift register isdirectly copied into
SPIBUF andthereceive DMA request (ifenabled) isgenerated andthereceive-interrupt (ifenabled) is
generated. The RXEMPTY flaginSPIBUF iscleared atthesame time.
*IfSPIBUF isalready fullattheendofreceive completion, theRXshift register contents iscopied to
RXBUF. Areceive DMA request isgenerated, ifenabled. The receive complete interrupt lineremains
high.
*IfSPIBUF isread bytheCPU orDMA andifRXBUF isfull,then thecontents ofRXBUF arecopied to
SPIBUF assoon asSPIBUF isread. RXEMPTY flagremains cleared, indicating thatSPIBUF isstill
full.
*Ifboth SPIBUF andRXBUF arefull,then RXBUF willbeoverwritten andtheRXOVR interrupt flagis
setandaninterrupt isgenerated, ifenabled.
NOTE: Prefetching isdone only inMaster mode. InSlave mode, since theTGtobeserviced is
known only after avalid ChipSelect assertion, noprefetching isdone.
28.2.2 MibSPI Mode
Figure 28-2 shows multi-buffered mode operation. InMulti-buffer mode thetransmit data hastobewritten
totheTXRAM locations andthereceive data hastoberead from RXRAM locations ofthemulti-buffer
RAM. AMibSPI supports upto256locations each forTransmit andReceive Data.
Figure 28-2. MibSPI Functional Logic Diagram

<!-- Page 1503 -->

www.ti.com Basic Operation
1503 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.2.1 Data Handling forTXandRXTransfer Groups
28.2.2.1.1 Data Sequencing ofaTransmit Data
Inmulti-buffer mode, anybuffer thatneeds tobetransmitted over bytheSPI, should beassociated with a
Transfer Group. Each TG(Transfer Group) willhave aTrigger Source based onwhich it'llbetriggered.
Once aTGistriggered, thebuffers belonging toitwillbetransmitted.
Sequencer (FSM) controls thedata flow from themulti-buffer RAM totheShift Register. The Multi-buffer
Control Logic hasarbitration logic between VBUS andtheSequencer accessing themulti-buffer RAM.
Sequencer picks upahighest priority Transfer Group from among theactive TGs tobeserviced. Forthe
selected TGthestarting buffer tobetransferred isobtained from thePSTART oftherespective TGxCTRL
register.
Sequencer requests fortheselected buffer through theMulti-buffer Control Logic, andonce itreceives the
data, itreads thecontrol fields todetermine thesubsequent action. Once thebuffer isdetermined tobe
ready fortransfer, thedata iswritten totheTXSHIFT REGISTER bytheSequencer. This triggers the
Kernel FSM toinitiate theSPItransfer.
Once theSequencer isfinished writing totheTXSHIFT REGISTER, itprefetches thenext buffer tobe
transferred from themulti-buffer RAM andstores theData.
Once theSequencer isfinished writing totheTXSHIFT REGISTER, itprefetches thenext buffer tobe
transferred from themulti-buffer RAM andstores theData.
Sequencer writes theprefetched Transmit Data totheShift Register immediately upon request bythe
Kernel. This way, thethroughput oftheSPItransfer isincreased inMaster mode ofoperation. Incase of
Slave mode, after theReceive data iscopied totheRXRAM, Sequencer waits forthenext active Chip
Select trigger tofetch thenext data.
28.2.2.1.2 Data Sequencing oftheReceived Data
AttheendofaSPItransfer, thereceived Data iscopied toSPIBUF register andthen forwarded tothe
Sequencer. The Sequencer then, requests theMulti-buffer Control Logic towrite thereceived data tothe
respective RXRAM location. Along with Received Data, theStatus fields likeTransmission Error Flags and
theLast Chip Select Number (LCSNR) areforwarded tobeupdated intheStatus Field oftheRXRAM.
Sequencer clears theRXEMPTY bitwhile writing anew Received Data intheRXRAM. IftheRXEMPTY
bitisalready 0,then theSequencer sets theRCVR_OVRN bitto1toindicate thatthisparticular location
hasbeen overwritten intheRXRAM.
28.2.3 DMA Requests
Inorder toreduce CPU overhead inhandling SPImessage traffic onacharacter-by-character basis, SPI
canusetheDMA controller totransfer thedata
28.2.3.1 SPI/MibSPI Compatibility Mode DMA Requests
.The DMA request enable bit(DMA REQ EN) controls theassertion ofrequests totheDMA controller
module. When acharacter isbeing transmitted orreceived, theSPIwillsignal theDMA viatheDMA
request signals, TX_DMA_REQ andRX_DMA_REQ. The DMA controller willthen perform therequired
data transfer.
Forefficient behavior during DMA operations, thetransmitter empty andreceive-buffer fullinterrupts can
bedisabled. Forspecific DMA features, seetheDMA controller specification.
The SPIgenerates arequest ontheTX_DMA_REQ lineeach time theTXdata iscopied totheTXshift
register either from theTXBUF orfrom peripheral data bus(when TXBUF isempty).
The firstTX_DMA_REQ pulse isgenerated when either ofthefollowing istrue:
*DMA REQ EN(SPIINT0[16]) issetto1while SPIEN (SPIGCR1[24]) isalready 1.
*SPIEN (SPIGCR1[24]) issetto1while DMA REQ EN(SPIINT0[16]) isalready 1.

<!-- Page 1504 -->

DMA CHANNEL x
BUFIDx
RXDMA_MAPx TXDMA_MAPx
DMA_REQ(0)
DMA_REQ(15)4x16 Decoder 4x16 Decoder
COMBINE LOGIC0 150 15
0
154 4TX RAM RX RAM
0 15Control LogicTXDMA_ENAx
RXDMA_ENAx
(combines all 8 Channel O/Ps)
Basic Operation www.ti.com
1504 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)The SPIgenerates arequest ontheRX_DMA_REQ lineeach time thereceived data iscopied tothe
SPIBUF.
28.2.3.2 DMA inMulti-Buffer Mode
The MibSPI provides sophisticated programmable DMA control logic thatcompletely eliminates the
necessity ofCPU intervention fordata transfers, once programmed. When themulti-buffer mode isused,
theDMA enable bitintheSPIINT0 register isignored. DMA source ordestination should beonly themulti-
buffer RAM andnotSPIDAT0 /SPIDAT1 orSPIBUF register asincase ofcompatibility mode DMA.
The MibSPI offers uptoeight DMA channels (forSEND andRECEIVE). AlloftheDMA channels are
programmable individually andcanbehooked toanybuffer. The MibSPI provides upto16DMA request
lines, andDMA requests from anychannel canbeprogrammed toberouted through anyofthese 16
lines. ADMA transfer cantrigger both transmit andreceive.
Each DMA channel hasthecapability totransfer ablock ofupto32data words without interruption using
only onebuffer ofthearray byconfiguring theDMAxCTRL register. Using theDMAxCOUNT and
DMACTNTLEN register, upto65535 (64K) words ofdata canbetransferred without anyinterruption using
justonebuffer ofthearray. This enables thetransfer ofmemory blocks from orintoanexternal SPI
memory.
Figure 28-3. DMA Channel andRequest Line (Logical) Structure inMulti-buffer Mode

<!-- Page 1505 -->

www.ti.com Basic Operation
1505 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.4 Interrupts
There aretwolevels ofvectorized interrupts supported bytheSPI. These interrupts canbecaused under
thefollowing circumstances:
*Transmission error
*Receive overrun
*Receive complete (receive buffer full)
*Transmit buffer empty
These interrupts may beenabled ordisabled viatheSPIINT0 register.
During transmission, ifoneofthefollowing errors occurs: BITERR, DESYNC, DLENERR, PARITYERR, or
TIMEOUT, thecorresponding bitintheSPIFLG register isset.Ifthecorresponding enable bitisset,then
aninterrupt isgenerated. The level ofalltheabove interrupts issetbythebitfields intheSPILVL register.
The error interrupts areenabled andprioritized independently from each other, buttheinterrupt generated
willbethesame ifmultiple errors areenabled onthesame level. The SPIFLG register should beused to
determine theactual cause ofanerror.
NOTE: Since there aretwointerrupt lines, oneeach forLevel 0andLevel 1,itispossible fora
programmer toseparate outtheinterrupts forreceive buffer fullandtransmit buffer empty.
Byprogramming onetoLevel 0andtheother toLevel 1,itispossible toavoid acheck on
whether aninterrupt occurred fortransmit orforreceive. Aprogrammer canalso choose to
group alloftheerror interrupts intooneinterrupt lineandboth TX-empty andRX-full
interrupts intoanother interrupt lineusing theLVL control register. Inthisway, itispossible
toseparate error-checking from normal data handling.
28.2.4.1 Interrupts inMulti-Buffer Mode
Inmulti-buffer mode, theSPIcangenerate interrupts ontwolevels.
Innormal multi-buffer operation, thereceive andtransmit arenotused andtherefore theenable bitsof
SPIINT0 arenotused.
The interrupts available inmulti-buffer mode are:
*Transmission error interrupt
*Receive overrun interrupt
*TGsuspended interrupt
*TGcompleted interrupt
When aTGhasfinished andthecorresponding enable bitintheTGINTENA register isset,atransfer-
finished interrupt isgenerated. The level ofpriority oftheinterrupt isdetermined bythecorresponding bit
intheTGINTLVL register.
When aTGissuspended byabuffer thathasbeen setassuspend towait until TXFULL flagor/and
RXEMPTY flagareset,andifthecorresponding bitintheTGINTENA register isset,antransfer-
suspended interrupt isgenerated. The level ofpriority oftheinterrupt isdetermined bythecorresponding
bitintheTGINTLVL register.
Figure 28-4 illustrates theTGinterrupts.
During transmission, ifoneofthefollowing errors occurs, BITERR, DESYNC, PARITYERR, TIMEOUT,
DLENERR, thecorresponding flagintheSPIFLG register isset.Iftheenable bitisset,then aninterrupt is
generated. The level oftheinterrupts could begenerated according tothebitfield inSPILVL register.

<!-- Page 1506 -->

BITERR
DESYNC
PARITYERR
TIMEOUTLVL 0
LVL 1
DLENERR
ENAx LVLxRXOVRN0
1
0
1
0
1
0
1
0
1
0
1
Finished
SuspendedLVL 0
LVL 1ENAx LVLx
ENAx LVLxTG x
Vector X +1
Bit 00
1
0
1
Basic Operation www.ti.com
1506 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-4. TGInterrupt Structure
The RXOVRN interrupt isgenerated when abuffer intheRXRAM isoverwritten byanew received word.
While writing newly received data toaRXRAM location, iftheRXEMPTY bitofthecorresponding location
is0,then theRXOVR bitwillbesetto1during thewrite operation, sothatthebuffer starts toindicate an
overrun. This RXOVR flagisalso reflected inSPIFLG register asRXOVRNINTFLG andthecorresponding
vector number isupdated inTGINTVECT0/TGINTVECT1 register. Ifanoverrun interrupt isenabled, then
aninterrupt willbegenerated indicating anoverrun condition.
The error interrupts areenabled andprioritized independently from each other, butthevector generated
bytheSPIwillbethesame ifmultiple errors areenabled onthesame level.
Figure 28-5. SPIFLG Interrupt Structure
Since thepriority ofanerror interrupt islower than acompletion/suspend interrupt foraTG,theinterrupts
canbesplit intotwolevels. Byprogramming alltheerror interrupts intoLevel 0andTG-complete /TG-
suspend interrupts intoLevel 1,itispossible togetaclear indication ofthesource oferror interrupts.
However, when avector register shows anerror interrupt, theactual buffer forwhich theerror has
occurred isnotreadily identifiable. Since each buffer inthemulti-buffer RAM isstored along with its
individual status flags, each buffer should beread until abuffer with anyerror flagsetisfound.
Aseparate interrupt lineisprovided toindicate theuncorrectable error condition intheMibSPI. This lineis
available (and valid) only inthemulti-buffer mode oftheMibSPI module andiftheparity error detection
feature formulti-buffer RAM isenabled.

<!-- Page 1507 -->

Master Slave
(Master = 1; CLKMOD = 1) (Master = 0; CLKMOD = 0)
SPIDAT0 SPIDAT0MSB LSB MSB LSB
Write to SPIDAT0SPISOMISPISIMO
SPISOMISPISIMO
SPICLK SPICLK
Write to SPIDAT
SPICLK
SPISIMO
SPISOMI
www.ti.com Basic Operation
1507 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.5 Physical Interface
The SPIcanbeconfigured viasoftware tooperate aseither amaster oraslave. The MASTER bit
(SPIGCR1[0]) selects theconfiguration oftheSPISIMO andSPISOMI pins. The CLKMOD bit
(SPIGCR1[1]) determines whether aninternal orexternal clock source isused.
The slave chip select (SPICS) pins, areused when communicating with multiple slave devices. When the
awrite occurs toSPIDAT1 inmaster mode, theSPICS pins areautomatically driven toselect thespecified
slave.
Handshaking mechanism, provided bytheSPIENA pin,enables aslave SPItodelay thegeneration ofthe
clock signal supplied bythemaster ifitisnotprepared forthenext exchange ofdata.
28.2.5.1 Three-Pin Mode
Inmaster mode configuration (MASTER =1andCLKMOD =1),theSPIprovides theserial clock onthe
SPICLK pin.Data istransmitted ontheSPISIMO pinandreceived ontheSPISOMI pin(see Figure 28-6 ).
Data written totheshift register (SPIDAT0 /SPIDAT1) initiates data transmission ontheSPISIMO pin,
MSB first. Simultaneously, received data isshifted through theSPISOMI pinintotheLSB oftheSPIDAT0
register. When theselected number ofbitshave been transmitted, thereceived data intheshift register is
transferred totheSPIBUF register fortheCPU toread. Data isstored right-justified inSPIBUF.
See Section 28.2.1.2.2 andSection 28.2.2 fordetails about thedata handling fortransmit andreceive
operations.
Inslave mode configuration (MASTER =0andCLKMOD =0),data shifts outontheSPISOMI pinandin
ontheSPISIMO pin.The SPICLK pinisused astheinput fortheserial shift clock, which issupplied from
theexternal network master. The transfer rate isdefined bythisclock.
Data written totheSPIDAT0 orSPIDAT1 register istransmitted tothenetwork when theSPICLK signal is
received from thenetwork master. Toreceive data, theSPIwaits forthenetwork master tosend the
SPICLK signal andthen shifts data ontheSPISIMO pinintotheRXshift register. Ifdata istobe
transmitted bytheslave simultaneously, itmust bewritten totheSPIDAT0 orSPIDAT1register before the
beginning oftheSPICLK signal.
Figure 28-6. SPIThree-Pin Operation

<!-- Page 1508 -->

Master Slave
(Master = 1; CLKMOD = 1) (Master = 0; CLKMOD = 0)
SPIDAT1 SPIDAT0MSB LSB MSB LSB
Write to SPIDAT1SPISOMI SPISOMISPISIMO
SPICLK SPICLK
SPICS SPICS
Write to SPIDAT1
SPICLK
SPISIMO
SPISOMISPICSSPISIMO
Basic Operation www.ti.com
1508 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.5.2 Four-Pin Mode with Chip Select
The three-pin option andthefour-pin option oftheSPI/MibSPI areidentical inthemaster mode
(CLKMOD =1),except thatthefour-pin option uses either SPIENA orSPICS pins. The I/Odirections of
these pins aredetermined bytheCLKMOD control bitasSPI/MibSPI andisnotgeneral-purpose I/O.
28.2.5.2.1 Four-Pin Option with SPICS
Inmaster mode, each chip select signal isused toselect aspecific slave. Inslave mode, thechip select
signal isused toenable anddisable thetransfer. Chip-select functionality isenabled bysetting oneofthe
SPICS pins asachip select. Itisdisabled bysetting allSPICS pins asGIOs inSPIPC0.
28.2.5.2.1.1 Multiple Chip Selects
The SPICS pins thatareused must beconfigured asfunctional pins intheSPIPC0 register. The default
pattern tobeputontheSPICS when alltheslaves aredeactivated issetintheSPIDEF register. This
pattern allows different slaves with different chip-select polarity tobeactivated bytheSP/MibSPI.
The master-mode SPIiscapable ofdriving either 0or1astheactive value foranySPICS output pin.The
drive state fortheSPICS pins iscontrolled bytheCSNR field ofSPIDAT1. The pattern thatisdriven will
select theslave towhich thetransmission isdedicated.
Inslave mode, theSPIcanonly beselected byanactive value of0onanyofitsselected SPICS input
pins.
Figure 28-7. Operation with SPICS

<!-- Page 1509 -->

Master Slave
(Master = 1; CLKMOD = 1) (Master = 0; CLKMOD = 0)
SPIDAT0 SPIDAT0MSB LSB MSB LSB
Write to SPIDAT0SPISOMISPISIMO
SPISOMISPISIMO
SPICLK SPICLK
SPIENA SPIENA Write to SPIDAT0
Write to SPIDAT0 (SLAVE)
Write to SPIDAT0 (master)
SPICLK
SPISIMO
SPISOMISPIENA
www.ti.com Basic Operation
1509 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.5.2.2 Four-Pin Option with SPIENA
The SPIENA operates asaWAIT signal pin.Forboth theslave andthemaster, theSPIENA pinmust be
configured tobefunctional (SPIPC0[8] =1).Inthismode, anactive-low signal from theslave onthe
SPIENA pinallows themaster SPItodrive theclock pulse stream. Ahigh signal tells themaster tohold
theclock signal (and delay SPIactivity).
IftheSPIENA pinisinhigh-impedance mode (ENABLE_HIGHZ =1),theslave willputSPIENA intothe
high-impedance once itcompletes receiving anew character. IftheSPIENA pinisinpush-pull mode
(ENABLE_HIGHZ =0),theslave willdrive SPIENA to1once itcompletes receiving anew character. The
slave willdrive SPIENA lowagain forthenext word totransfer, after new data iswritten totheslave TX
shift register.
Inmaster mode (CLKMOD =1),iftheSPIENA pinisconfigured asfunctional, then thepinacts asan
input pin.Ifconfigured asaslave SPIandasfunctional, theSPIENA pinacts asanoutput pin.
NOTE: During atransfer, ifaslave-mode SPIdetects adeassertion ofitschip select before its
internal character length counter overflows, then itplaces SPISOMI andSPIENA (if
ENABLE_HIGHZ bitissetto1)inhigh-impedance mode. Once thiscondition hasoccurred,
ifaSPICLK edge isdetected while thechip select isdeasserted, then theSPIstops that
transfer andsets anDLENERR error flagandgenerates aninterrupt (ifenabled).
Figure 28-8. Operation with SPIENA

<!-- Page 1510 -->

Master Slave
(Master = 1; CLKMOD = 1) (Master = 0; CLKMOD = 0)
SPIDAT1 SPIDAT0MSB LSB MSB LSB
Write to SPIDAT1SPISOMISPISIMO
SPISOMISPISIMO
SPICLK SPICLK
SPICS SPICS Write to SPIDAT0
Write to SPIDAT0 (SLAVE)Write to SPIDAT1 (MASTER)
SPISIMO
SPISOMISPIENASPIENA SPIENA
SPICS
SPICLK
Basic Operation www.ti.com
1510 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.5.3 Five-Pin Operation (Hardware Handshaking)
Five-pin operation combines thefunctionality ofthree-pin mode, plus theenable pinandoneormore chip
select pins. The result isfullhardware handshaking. Tousethismode, both theSPIENA pinandthe
required number ofSPICS pins must beconfigured asfunctional pins.
IftheSPIENA pinisinhigh-impedance mode (ENABLE_HIGHZ =1),theslave SPIwillputthissignal into
thehigh-impedance state bydefault. The slave willdrive thesignal SPIENA lowwhen new data iswritten
totheslave shift register andtheslave hasbeen selected bythemaster (SPICS islow).
IftheSPIENA pinisinpush-pull mode (ENABLE_HIGHZ =0),theslave SPIdrives thispinhigh bydefault
when itisinfunctional mode. The slave SPIwilldrive theSPIENA signal lowwhen new data iswritten to
theslave shift register (SPIDAT0/SPIDAT1) andtheslave isselected bythemaster (SPICS islow). Ifthe
slave isdeselected bythemaster (SPICS goes high), theslave SPIENA signal isdriven high.
NOTE: Push-pull mode oftheSPIENA pincanbeused only when there isasingle slave inthe
system. When multiple SPIslave devices areconnected tothecommon SPIENA pin,allof
theslaves should configure their SPIENA pins inhigh-impedance mode.
Inmaster mode, iftheSPICS pins areconfigured asfunctional pins, then thepins willbeinoutput mode.
Awrite tothemaster 'sSPIDAT1/SPIDAT0 register willautomatically drive theSPICS signals low. The
master willdrive theSPICS signals high again after completing thetransfer ofthebitsofthedata.
Inslave mode (CLKMOD =0),theSPICS pins actasSPIfunctional inputs.
Figure 28-9. SPIFive-Pin Option with SPIENA andSPICS

<!-- Page 1511 -->

www.ti.com Basic Operation
1511 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6 Advanced Module Configuration Options
28.2.6.1 Data Formats
Tosupport multiple different types ofslaves inoneSPInetwork, four independent data word formats are
implemented thatallow configuration ofindividual data word length, polarity, phase, andbitrate. Each
word transmitted canselect which data format touseviathebitsDFSEL[1:0] initscontrol field from oneof
thefour data word formats. Same data format canbesupported onmultiple chip selects.
Data formats 0,1,2,and3canbeconfigured through SPIFMTx control registers.
Each SPIdata format includes thestandard SPIdata format with enhanced features:
*Individually-configurable shift direction canbeused toselect MSB firstorLSB first, whereas the
position oftheMSB depends ontheconfigured data word length.
*Receive data isautomatically right-aligned, independent ofshift direction anddata word length.
Transmit data hastobewritten right-aligned intotheSPIandtheinternal shift register willtransmit
according totheselected shift direction anddata word length forcorrect transfer.
*Toincrease fault detection ofdata transmission andreception, anoddoreven parity bitcanbeadded
attheendofadata word. The parity generator canbeenabled ordisabled individually foreach data
format. Ifareceived parity bitdoes notmatch with thelocally calculated parity bit,theparity error flag
(PARITYERR) issetandaninterrupt isasserted (ifenabled).
Since themaster-mode SPIcandrive twoconsecutive accesses tothesame slave, an8-bit delay counter
isavailable tosatisfy thedelay time fordata toberefreshed intheaccessed slave. The delay counter can
beprogrammed aspart ofthedata format.
CHARLEN[4:0] specifies thenumber ofbits(2to16)inthedata word. The CHARLEN[4:0] value directs
thestate control logic tocount thenumber ofbitsreceived ortransmitted todetermine when acomplete
word istransferred.
Data word length must beprogrammed tothesame length forboth themaster andtheslave .However,
when chip selects areused, there may bemultiple targets with different lengths inthesystem.
NOTE: Data must beright-justified when itiswritten totheSPIfortransmission irrespective ofits
character length orword length.
Figure 28-10 shows how a12-bit word (0xEC9) needs tobewritten tothetransmit buffer tobetransmitted
correctly.
Figure 28-10. Format forTransmitting an12-Bit Word
D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
x x x x 1 1 1 0 1 1 0 0 1 0 0 1
NOTE: The received data isalways stored right-justified regardless ofthecharacter length or
direction ofshifting andispadded with leading 0swhen thecharacter length isless than 16
bits.
Figure 28-11 shows how a10-bit word (0x0A2) isstored inthebuffer once itisreceived.
Figure 28-11. Format forReceiving an10-Bit Word
D15 D14 D13 D12 D11 D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
0 0 0 0 0 0 0 0 1 0 1 0 0 0 1 0

<!-- Page 1512 -->

Data is output one-half cycle before the first rising edge of SPICLK and on subsequent falling edges of SPICLK
Input data is latched on the rising edge of SPICLKWrite SPIDAT
SPISIMO
SPISOMI
receive sampleMSB D6 D5 D4          D3           D2 D1 LSB
D6 D5 D4            D3 D2 D1 D71 2 3 4 5 6 7 8
D0SPICLK
Data is output on the rising edge of SPICLK.
Input data is latched on the falling edge of SPICLK.Write SPIDAT
SPICLK
SPISIMO
receive sampleMSB D6 D5 D4 D3 D2 D1
D0LSB
D6 D5          D4 D3 D2 D1 D71 2 3 4 5 6 7 8
SPISOMI
Basic Operation www.ti.com
1512 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.2 Clocking Modes
SPICLK may operate infour different modes, depending onthechoice ofphase (delay/no delay) andthe
polarity (rising edge/falling edge) oftheclock.
The data input andoutput edges depend onthevalues ofboth POLARITY andPHASE asshown in
Table 28-3.
Table 28-3. Clocking Modes
POLARITY PHASE Action
0 0 Data isoutput ontherising edge ofSPICLK. Input data islatched onthefalling edge.
0 1 Data isoutput onehalf-cycle before thefirstrising edge ofSPICLK andonsubsequent
falling edges. Input data islatched ontherising edge ofSPICLK.
1 0 Data isoutput onthefalling edge ofSPICLK. Input data islatched ontherising edge.
1 1 Data isoutput onehalf-cycle before thefirstfalling edge ofSPICLK andonsubsequent
rising edges. Input data islatched onthefalling edge ofSPICLK.
Figure 28-12 toFigure 28-15 illustrate thefour possible configurations ofSPICLK corresponding toeach
mode. Having four signal options allows theSPItointerface with many different types ofserial devices.
Figure 28-12. Clock Mode with Polarity =0andPhase =0
Figure 28-13. Clock Mode with Polarity =0andPhase =1

<!-- Page 1513 -->

Data is output one-half cycle before the first falling edge o f SPICLK and on the subsequent rising edges of SPICLK.
Input data is latched on the falling edge of SPICLK.Write SPIDAT
SPICLK
SPISIMO
SPISOMI
receive sampleMSB D6 D5 D4 D3         D2          D1
D0LSB
D6 D5         D4 D3 D2         D1 D71 2 3 4 5 6 7 8
Data is output on the falling edge of SPICLK.
Input data is latched on the rising edge of SPICLK.Write SPIDAT
SPISIMO
SPISOMI
receive sampleMSB D6 D5          D4 D3 D2 D1
D0LSB
D6 D5          D4         D3 D2 D1 D71 2 3 4 5 6 7 8SPICLK
www.ti.com Basic Operation
1513 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-14. Clock Mode with Polarity =1andPhase =0
Figure 28-15. Clock Mode with Polarity =1andPhase =1

<!-- Page 1514 -->

4 3 2 1 0 4 3 0 1 24 3 2 1 0 4 3 0 1 2
Master SPI
Interrupt flag
Slave SPI
Interrupt flagSPISOMI
from slave
SPISIMO
from masterClock polarity = 1
Clock phase = 1
SPICSSPICLK signal options:
SPIENAClock polarity = 1
Clock phase = 0Clock polarity = 0Clock polarity = 0
Clock phase = 0
Clock phase = 1
Basic Operation www.ti.com
1514 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.2.1 Data Transfer Example
Figure 28-16 illustrates aSPIdata transfer between twodevices using acharacter length offivebits.
Figure 28-16. Five Bits perCharacter (5-Pin Option)

<!-- Page 1515 -->

SPICS
SPICLK
SPISOMI
VCLK
tC2TDELAY
www.ti.com Basic Operation
1515 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.3 Decoded andEncoded Chip Select (Master Only)
Inthisdevice, theSPIcanconnect toupto6individual slave devices using chip-selects byrouting one
wire toeach slave. The 6chip selects inthecontrol field aredirectly connected tothe6pins. The default
value ofeach chip select (not active) canbeconfigured viatheregister CSDEF. During atransmission,
thevalue ofthechip select control field (CSNR) oftheSPIDAT1 register isdriven ontheSPICS pins.
When thetransmission finishes, thedefault chip-select value (defined bytheCSDEF register) isputonthe
SPICS pins.
The SPIcansupport more than 6slaves byusing encoded chip selects. Toconnect theSPIwith encoded
slaves devices, theCSNR field allows multiple active SPICS pins atthesame time, which enables
encoded chip selects from 0to16.Touseencoded chip selects, all6chip select lines have tobe
connected toeach slave device andeach slave needs tohave aunique chip-select address. The CSDEF
register isused toprovide theaddress atwhich slaves devices areallde-selected.
Users cancombine decoded andencoded chip selects. Forexample, nSPICS pins canbeused for
encoding ann-bitaddress andtheremaining pins canbeconnected todecoded-mode slaves.
28.2.6.4 Chip Select Timing Control
This section describes fields ofthecontrol register SPIDELAY. This register decides thechip select and
timing control forthedevice.
28.2.6.4.1 Chip-Select-Active-to-Transmit-Start-Delay (C2TDELAY)
C2TDELAY isused inmaster mode only. Itdefines asetup time fortheslave device thatdelays thedata
transmission from thechip select active edge byamultiple ofVCLK cycles. Chip Select-active-to-
transmission delays between 2to257VCLK cycles canbeachieved.
The setup time value iscalculated as:
tC2TDELAY =(C2TDELAY +2)×VCLK Period
Figure 28-17 isthetiming diagram when C2TDELAY of8VCLK Cycles.
Figure 28-17. Example: tC2TDELAY =8VCLK Cycles

<!-- Page 1516 -->

SPICS
SPIENA
SPICLK
SPISOMI
tT2EDELAY
SPICS
SPICLK
SPISOMI
VCLK
tT2CDELAY
Basic Operation www.ti.com
1516 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.4.2 Transmit-End-to-Chip-Select-Inactive-Delay (T2CDELAY)
T2CDELAY isused inmaster mode only. Itdefines ahold time fortheslave device thatdelays thechip
select deactivation byamultiple ofVCLK cycles after thelastbitistransferred. T2CDELAY canbe
configured between 2and256VCLK cycles.
The hold time value iscalculated as:
tT2CDELAY =(T2CDELAY +1)×VCLK Period
Figure 28-18 isthetiming diagram when T2CDELAY of4VCLK Cycles.
Figure 28-18. Example: tT2CDELAY =4VCLK Cycles
28.2.6.4.3 Transmit-Data-Finished-to-ENA-Pin-Inactive-Time-Out (T2EDELAY)
T2EDELAY isused inmaster mode only. Itdefines atime-out value asamultiple ofSPIclock before the
ENAble signal hastobecome inactive andafter theCSbecomes inactive. The SPIclock depends on
which data format isselected. Iftheslave device ismissing oneormore clock edges, itisbecoming de-
synchronized. Although themaster hasfinished thedata transfer the
The T2EDELAY defines atime-out value thattriggers theDESYNC flag, iftheENA signal isnot
deactivated intime. DESYNC flagissettoindicate thattheSlave device didnotdeassert itsSPIENA pin
intime toacknowledge thatithasreceived allthebitsofthesent character.
The timeout value iscalculated as:
tT2EDELAY =T2EDELAY/SPIclock
Figure 28-19. Transmit-Data-Finished-to-ENA-Inactive-Timeout
NOTE: IfT2CDELAY isprogrammed anon-zero value, then T2EDELAY willstart only after the
T2CDELAY completes. This should betaken intoconsideration todetermine anoptimum
value ofT2EDELAY.

<!-- Page 1517 -->

SPICS
SPIENA
SPICLK
SPISOMI
tC2EDELAY
www.ti.com Basic Operation
1517 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.4.4 Chip-Select-Active-to-ENA-Signal-Active-Time-Out (C2EDELAY)
C2EDELAY isused inmaster mode only anditapplies only iftheaddressed slave generates anENAble
signal asahardware handshake response. C2EDELAY defines themaximum time between theSPI/
MibSPI activating thechip select signal andtheaddressed slave responding byactivating theENA signal.
C2EDELAY defines atime-out value asamultiple ofSPIclocks. The SPIclock depends onwhether data
format 0ordata format 1isselected.
The timeout value iscalculated as:
tC2EDELAY =C2EDELAY/SPIclock
Figure 28-20. Chip-Select-Active-to-ENA-Signal-Active-Timeout
NOTE:
*Iftheslave device isnotresponding with theENA signal before thetime-out
value isreached, theTIMEOUT flaginSPIFLG register issetandaninterrupt is
asserted ifenabled.
*Ifatime-out occurs theMibSPI clears thetransmit request ofthetimed-out
buffer, sets theTIMEOUT flagforthecurrent buffer andcontinues with the
transfer ofthenext buffer inthesequence thatisenabled.
*IfC2TDELAY isprogrammed anon-zero value, then C2EDELAY willstart only
after theC2TDELAY completes. This should betaken intoconsideration to
determine anoptimum value ofC2EDELAY.
28.2.6.5 Multiple Transfers toSame Slave andVariable Chip Select Setup andHold Timing
This section gives information onthevariable chip select setup anditshows how theCSHOLD bitisused
andhow themultiple transfers tosame slave isenabled inthedevice.
28.2.6.5.1 Variable Chip Select Setup andHold Timing (Master Only)
Inorder tosupport slow slave devices, adelay counter canbeconfigured todelay data transmission after
thechip select isactivated. Asecond delay counter canbeconfigured todelay thechip select deactivation
after thelastdata bitistransferred. Both delay counters areclocked with theperipheral clock (VCLK).
Ifaparticular data format specifically does notrequire these additional set-up orhold times forthechip
select pins, then they canbedisabled inthecorresponding SPIFMTx register.
28.2.6.5.2 Hold Chip-Select Active
Some slave devices require thechip select signal tobeheld continuously active during several
consecutive data word transfers. Other slave devices require thechip select signal tobedeactivated
between consecutive data word transfers.
CSHOLD isprogrammable inboth master andslave modes ofthemulti-buffer mode ofSPI. However, the
meaning ofCSHOLD inmaster mode andslave mode aredifferent.
NOTE: IftheCSHOLD bitissetwithin thecurrent data control field, theprogrammed hold time and
thefollowing programmed set-up time willnotbeapplied between transactions.

<!-- Page 1518 -->

Write to SPIDAT0 (SLAVE)Write to SPIDAT1
SPICLK
SPISIMO
SPISOMISPIENASPISCSWrite to SPIDAT1
Write to SPIDAT0 (SLAVE)WORD1 WORD2
CSHOLD = 1 CSHOLD = 0
Basic Operation www.ti.com
1518 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.5.2.1 CSHOLD BitinMaster Mode
Each word inamaster-mode SPIcanbeindividually initialized foroneofthetwomodes viatheCSHOLD
bitinitscontrol field.
IftheCSHOLD bitissetinthecontrol field ofaword, thechip select signal willnotbedeactivated until the
next control field isloaded with new chip select information. Since thechip-select ismaintained active
between twotransfers, thechip-select hold delay (T2CDELAY) isnotapplied attheendofthecurrent
transaction, andthechip-select set-up time delay (C2TDELAY) isnotapplied aswell atthebeginning of
thefollowing transaction. However, thewait delay (WDELAY) willbestillapplied between thetwo
transactions, iftheWDEL bitissetwithin thecontrol field.
Figure 28-21 shows theSPIpins when amaster-mode SPItransfers aword thathasitsCSHOLD bitset.
The chip-select pins willnotbedeasserted after thecompletion ofthisword. Ifthenext word totransmit
hasthesame chip-select number (CSNR) value, thechip select pins willbemaintained until the
completion ofthesecond word, regardless ofwhether theCSHOLD bitissetornot.
Figure 28-21. Typical Diagram when aBuffer inMaster isinCSHOLD Mode (SPI-SPI)
28.2.6.5.2.2 CSHOLD BitinSlave Mode (Multi-buffered Mode)
IftheCSHOLD bitinabuffer issetto1,then theMibSPI does notwait fortheSPICS pins tobede-
activated attheendoftheshift operation tocopy thereceived data tothereceive RAM. With thisfeature,
itispossible foraslave inmulti-buffer mode todomultiple data transfers without requiring theSPICS pins
tobedeasserted between twobuffer transfers.
IftheCSHOLD bitinabuffer iscleared to0inaslave MibSPI, even after theshift operation isdone, the
MibSPI waits until theSPICS pin(iffunctional) isdeasserted tocopy thereceived data totheRXRAM.
IftheCSHOLD bitismaintained as0across allthebuffers, then theslave inmulti-buffer mode requires its
SPICS pins tobedeasserted between anytwobuffer transfers; otherwise, theSlave SPIwillbeunable to
respond tothenext data transfer.
NOTE: Incompatibility mode, theslave does notrequire theSPICS pintobedeasserted between
twobuffer transfers. The CSHOLD bitoftheslave willbeignored incompatibility mode.

<!-- Page 1519 -->

www.ti.com Basic Operation
1519 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6 Parallel Mode (Multiple SIMO/SOMI Support, notavailable onalldevices)
Inorder toincrease throughput, theparallel mode oftheSPIenables themodule tosend data over more
than onedata line(parallel 2,4,or8).When parallel mode isused, thedata length must besetas16bits.
Only module MIBSPIP5 supports Parallel Mode.
This feature increases throughput by2for2pins, by4for4pins, orby8for8pins.
Parallel mode supports thefollowing features:
*Scalable data lines (1,2,4,8)perdirection. (SOMI andSIMO lines)
*Allclock schemes aresupported (clock phase andpolarity)
*Parity issupported. The parity bitwillbetransmitted onbit0oftheSIMO/SOMI lines. The receive parity
isexpected onbit0oftheSOMI/SIMO pins.
Parallel mode canbeprogrammed using thePMODEx bitsofSPIPMCTRL register. See Section 28.3.25
fordetails about thisregister.
After reset theparallel mode selection bitsarecleared (single SIMO/SOMI lines).

<!-- Page 1520 -->

15 14 13 12 11  10 9 8 7 6 5 4 3 2 1SIMO[7:0]
SOMI[7:0]SPI Shift register 0SIMO7 SIMO6 SIMO5 SIMO4 SIMO3 SIMO2 SIMO1 SIMO0
MULTIPLEXERParallel mode
SOMI7 SOMI6 SOMI5 SOMI4 SOMI3 SOMI2 SOMI1 SOMI0DEMULTIPLEXER
15 14  13  12  11 10 9 8 7 6 5 4 3 2 1SIMO[7:0]
SOMI[7:0]SPI Shift registerSIMO7 SIMO6 SIMO5 SIMO4 SIMO3 SIMO2 SIMO1 SIMO0
MULTIPLEXERParallel mode
0
SOMI7 SOMI6 SOMI5 SOMI4 SOMI3 SOMI2 SOMI1 SOMI0DEMULTIPLEXER
Basic Operation www.ti.com
1520 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.1 Parallel Mode Block Diagram
Figure 28-22 andFigure 28-23 show theparallel connections totheSPIshift register.
Figure 28-22. Block Diagram Shift Register, MSB First
Figure 28-23. Block Diagram Shift Register, LSB First

<!-- Page 1521 -->

www.ti.com Basic Operation
1521 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.2 Parallel Mode PinMapping, MSB First
Table 28-4 andTable 28-5 describe theSOMI andSIMO pinmapping when theSPIisused inparallel
mode (1,2,4,8)pinmode, MSB first.
NOTE: MSB-first orLSB-first canbeconfigured using theSHIFTDIRx bitoftheSPIFMTx registers.
Table 28-4. PinMapping forSIMO Pinwith MSB First
Parallel Mode Shift Register Bit SIMO[7:0]
1 15 0
2 15 1
7 0
4 15 3
11 2
7 1
3 0
8 15 7
13 6
11 5
9 4
7 3
5 2
3 1
1 0
Table 28-5. PinMapping forSOMI Pinwith MSB First
Parallel Mode Shift Register Bit SOMI[7:0]
1 0 0
2 0 0
8 1
4 0 0
4 1
8 2
12 3
8 0 0
2 1
4 2
6 3
8 4
10 5
12 6
14 7

<!-- Page 1522 -->

Basic Operation www.ti.com
1522 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.3 Parallel Mode PinMapping, MSB-First, LSB-First
Table 28-6 andTable 28-7 describe theSIMO andSOMI pinmapping when SPIisused inparallel mode
(1,2,4,8)pinmode, LSB first.
Table 28-6. PinMapping forSIMO Pinwith LSB First
Parallel Mode Shift Register Bit SIMO[7:0]
1 0 0
2 8 1
0 0
4 12 3
8 2
4 1
0 0
8 14 7
12 6
10 5
8 4
6 3
4 2
2 1
0 0
Table 28-7. PinMapping forSOMI Pinwith LSB First
Parallel Mode Shift Register Bit SOMI[7:0]
1 15 0
2 7 0
15 1
4 3 0
7 1
11 2
15 3
8 1 0
3 1
5 2
7 3
9 4
11 5
13 6
15 7

<!-- Page 1523 -->

15 14 13 12 11 10 9 8
7 6 5 4 3 2 1 0
15 14 13 12 11 10 9 8
7 6 5 4 3 2 1 0VCLK
SPICLK
SIMO[1]
SIMO[0]
SOMI[1]
SOMI[0]
Shift register 15 14 13  12  11 10 9 8 7 6 5 4 3 2 1 0SIMO[1] SIMO[0]
SOMI[0] SOMI[1]Conceptual Block Diagram
www.ti.com Basic Operation
1523 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.4 2-Data Line Mode (MSB First, Phase 0,Polarity 0)
In2-data linemode (master mode) theshift register bits15and7willbeconnected tothepins SIMO[1]
andSIMO[0], andtheshift register bits8and0willbeconnected tothepins SOMI[1] andSOMI[0] orvice
versa inslave mode. After writing totheSPIDAT0/SPIDAT1 register, thebits15and7willbeoutput on
SIMO[1] andSIMO[0] ontherising edge ifSPICLK. With thefalling clock edge oftheSPICLK, the
received data onSOMI[1] andSOMI[0] willbelatched totheshift register bits8and0.The subsequent
rising edge ofSPICLK willshift thedata intheshift register by1bittotheleft.(SIMO[1] willshift thedata
outfrom bit15to8,SIMO[0] willshift thedata outfrom bit7to0).After eight SPICLK cycles, when thefull
data word istransferred, theshift register (16bits) iscopied tothereceive buffer, andtheRXINT flagwill
beset.Figure 28-24 shows theclock /data diagram ofthe2-data linemode. Figure 28-25 shows the
timing ofatwo-pin parallel transfer.
Figure 28-24. 2-data Line Mode (Phase 0,Polarity 0)
Figure 28-25. Two-Pin Parallel Mode Timing Diagram (Phase 0,Polarity 0)

<!-- Page 1524 -->

15 14 13 12
11 10 9 8
7 6 5 4
3 2 1 0
15 14 13 12
11 10 9 8
7 6 5 4
3 2 1 0VCLK
SPICLK
SIMO[3]
SIMO[2]
SIMO[1]
SIMO[0]
SOMI[3]
SOMI[2]
SOMI[1]
SOMI[0]
Shift registerSIMO[3]
SOMI[0] SOMI[1]SIMO[1] SIMO[0]
SOMI[2] SOMI[3]SIMO[2]
Conceptual Block Diagram15  14  13  12 11  10   9   8    7    6   5    4    3   2    1   0
Basic Operation www.ti.com
1524 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.5 4-Data Line Mode (MSB First, Phase 0,Polarity 0)
In4-data linemode (master mode) theshift register bits15,11,7,and3willbeconnected tothepins
SIMO[3], SIMO[2], SIMO[1], andSIMO[0], andtheshift register bits12,8,4,and0willbeconnected to
thepins SOMI[3], SOMI[2], SOMI[1], andSOMI[0] (orvice versa inslave mode). After writing to
SPIDAT1/SPIDAT0, thebits15,11,7,and3willbeoutput onSIMO[3], SIMO[2], SIMO[1], andSIMO[0]
ontherising edge ofSPICLK. With thefalling clock edge oftheSPICLK, thereceived data onSOMI[3],
SOMI[2], SOMI[1] andSOMI[0] willbelatched toshift register bits12,8,4,and0.The subsequent rising
edge ofSPICLK willshift data intheshift register by1bittotheleft(SIMO[3] willshift thedata outfrom bit
15to12,SIMO[2] willshift thedata outfrom bit11to8,SIMO[1] willshift thedata outfrom bit7to4,
SIMO[0] willshift thedata outfrom bit3to0).After four SPICLK cycles, when thefulldata word is
transferred, theshift register (16bits) iscopied tothereceive buffer, andtheRXINT flagwillbeset.
Figure 28-26 shows theclock/data diagram ofthefour-data linemode. Figure 28-27 ,shows thetiming
diagram forfour-data linemode.
Figure 28-26. 4-Data Line Mode (Phase 0,Polarity 0)
Figure 28-27. 4Pins Parallel Mode Timing Diagram (Phase 0,Polarity 0)

<!-- Page 1525 -->

Shift register 1514 1312 11 10 9 8 7 6 5 4 3 2 1 0
SOMI[0]SIMO[7] SIMO[6] SIMO[5] SIMO[4] SIMO[3] SIMO[2] SIMO[1] SIMO[0]
SOMI[1] SOMI[2] SOMI[3] SOMI[4] SOMI[5] SOMI[6] SOMI[7]Conceptual block diagram
www.ti.com Basic Operation
1525 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.6.6 8-Data Line Mode (MSB First, Phase 0,Polarity 0)
In8-data linemode (master mode) theshift register bits15,13,11,9,7,5and3willbeconnected tothe
pins SIMO[7], SIMO[6], SIMO[5], SIMO[4], SIMO[3], SIMO[2], SIMO[1], andSIMO[0], andtheshift-register
bits14,12,10,8,6,4,and0willbeconnected tothepins SOMI[7], SOMI[6], SOMI[5], SOMI[4], SOMI[3],
SOMI[2], SOMI[1], andSOMI[0] (orvice versa inslave mode).
After writing toSPIDAT0/SPIDAT1, thebits15,13,11,9,7,5,3,and1willbeoutput onSIMO[7],
SIMO[6], SIMO[5], SIMO[4], SIMO[3], SIMO[2], SIMO[1], andSIMO[0], ontherising edge ofSPICLK. On
thefalling clock edge oftheSPICLK, thereceived data onSOMI[8], SOMI[7], SOMI[6],SOMI[5], SOMI[4],
SOMI[3], SOMI[2], SOMI[1], andSOMI[0] willbelatched totheshift register bits14,12,10,8,6,4,2,and
0.
The subsequent rising edge ofSPICLK willshift thedata intheshift register by1bittotheleft.After two
SPICLK cycles, when thefulldata word istransferred theshift register (16bits) iscopied tothereceive
buffer, andtheRXINT flagwillbeset.Figure 28-28 shows theclock/data diagram ofthe8-data linemode.
Figure 28-29 shows thepintimings for8-data linemode.
Figure 28-28. 8-data Line Mode (Phase 0,Polarity 0)
NOTE: Parity Support
Using theparity support inparallel mode may seriously affect throughput. Foraneight-line
mode totransfer 16bitsofdata, only twoSPICLK pulses areenough. Ifparity isenabled,
oneextra SPICLK pulse willbeused totransfer andreceive theparity bit.Parity willbe
transmitted andreceived onthe0thlineregardless of1/2/4/8-line modes. During theparity
bittransfer, other data bitsarenotvalid.

<!-- Page 1526 -->

15 14
13 12
11 10
9 8
7 6
5 4
3 2
1 0
15 14
13 12
11 10
9 8
7 6
5 4
3 2
1 0VCLK
SPICLK
SIMO[7]
SIMO[6]
SIMO[5]
SIMO[4]
SIMO[3]
SIMO[2]
SIMO[1]
SIMO[0]
SOMI[7]
SOMI[6]
SOMI[5]
SOMI[4]
SOMI[3]
SOMI[2]
SOMI[1]
SOMI[0]
Basic Operation www.ti.com
1526 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-29. 8Pins Parallel Mode Timing Diagram (Phase 0,Polarity 0)
NOTE: Modulo Count Parallel Mode isnotsupported inthisdevice.
28.2.6.7 MibSPI Slave inMulti-buffer Configuration
When operating inslave mode, theMibSPI uses thechip-select pins 0to3togenerate atrigger tothe
corresponding Transfer Group. Forexample, putting 0000 onthechip-select pins triggers Transfer Groups
0andputting 0001 triggers TG1. When thevalue 1111 issettothechip-select, theMibSPI isdeselected,
thatisTransfer Group 15isnotavailable inslave mode. The remaining chip-select pins should stay in
GPIO mode. Inslave mode, thefields liketrigger source andtrigger event arenottaken intoaccount by
thesequencer. Only theSPICS pins cantrigger aTransfer Group. The chip-select trigger operates asa
level-sensitive trigger. However, when theMibSPI isin3-pin or4-pin with SPIENA mode, justone
Transfer Group canbetriggered anditisrestricted toTransfer Group 0(TG0). Inslave mode, thePRST
field should becleared to0.Ifthecorresponding Transfer Group isenabled, themulti-buffer reads the
current buffer oftheTGandwrites itintoSPIDAT1. IfTransfer Group isdisabled, themulti-buffer does not
update theSPIDAT1 register.
NOTE: Iftheselected Transfer Group isdisabled andnoupdate oftheSPIDAT1 register hasbeen
done, thedata tobetransferred ismeaningless. Even thereceived data willnotbecopied to
themulti-buffer RAM. However itwillbeavailable onSPIBUF register until itisoverwritten by
thesubsequent receive data.

<!-- Page 1527 -->

CTRL & Tx          STAT & Rx
Seq.
SPISCS [3:0]TG 0 Trigger
TG 14 Trigger
CS decoderRXBUF
SPISOMI SPISIMO SPICLK SPIENABuffer RAM
Shift Register Parity
www.ti.com Basic Operation
1527 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-30. Multi-buffer inSlave Mode
When theSPIDAT1 register isupdated, theenable signal isreleased, andthetransaction could begin. If
theenable signal isnotused, themaster should wait for6VCLK cycles before sending theclock tobegin
thetransaction. This time allows theMibSPI toupdate theSPIDAT1 register.
Once thetransaction isfinished, theMibSPI writes back thecontent oftheshift-register intotheRxbuffer
andupdates thestatus field.
NOTE: IfalltheTransfer Groups arenotneeded, thenumber ofSPICS pins thatneed tobein
functional mode could bereduced to3,2,or1byusing theSPIPC0 register. Inthese cases,
themaximum number ofTransfer Group accessible are, respectively 7,3,and1.The pins
thataresetinGPIO mode arenotdecoded.
MibSPI in3-pin and4-pin (with SPIENA) mode also supports multi-buffer mode. However, itisrestricted to
having justonetransfer group, Transfer Group 0(TG0). The entire multi-buffer RAM canbeconfigured for
TG0 alone. The PSTART field inTG1CTRL register should beused toconfigure thesize ofthemulti-
buffer (end ofthebuffers) forTG0.
NOTE: The maximum input frequency ontheSPICLK pinwhen inslave mode isVCLK frequency /2.
IftheSlave isconfigured ineither 3-pin or4-pin (without SPIENA) modes, then, between end
oflastSPICLK andthestart ofSPICLK fornext buffer, there should beatleast 6VCLK
cycles ofdelay.

<!-- Page 1528 -->

Basic Operation www.ti.com
1528 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.6.8 Transfer Groups
The size ofthemulti-buffer RAM depends ontheimplementation. Itcomprises 0to128/256 buffers,
whereas 0buffers considers thespecial case ofnomulti-buffer RAM. Each entry inthemulti-buffer RAM
consists of4parts: a16-bit transmit field, a16-bit receive field, a16-bit control field anda16-bit status
field. The multi-buffer RAM canbepartitioned intomultiple transfer group with variable number ofbuffers
each.
28.2.6.8.1 Configuring Transfer Groups andTrigger Events
Each TGcanbeconfigured viaonededicated control register, TGxCTRL. This register even configures
thetrigger events forthetransfer group. The register isdescribed inSection 28.3.34 .The actual number
ofavailable control registers varies bydevice.
28.2.6.8.2 Sequencer-Which Handled theSequencing ofTriggered Transfer Groups
Sequencer(FSM) controls thedata flow from themulti-buffer RAM totheShift Register. The Multi-buffer
Control Logic hasarbitration logic between VBUS andtheSequencer accessing themulti-buffer RAM.
Sequencer picks upahighest priority Transfer Group from among theactive TGs tobeserviced. Forthe
selected TGthestarting buffer tobetransferred isobtained from thePSTART oftherespective TGxCTRL
register.
Sequencer requests fortheselected buffer through theMulti-buffer Control Logic, andonce itreceives the
data, itreads thecontrol fields todetermine thesubsequent action. Once thebuffer isdetermined tobe
ready fortransfer, thedata iswritten totheTXSHIFT REGISTER bytheSequencer. This triggers the
Kernel FSM toinitiate theSPItransfer.
28.2.6.8.3 Inter-group Prioritization andArbitration
Transfer Group0 (TG0) hasthehighest priority andTG15 hasthelowest priority among thetransfer
groups TG0 toTG15.Where asunder thefollowing conditions under thefollowing conditions alower
priority Transfer Group cannot beinterrupted byahigher priority TG.
*When there 'saCSHOLD orLOCK buffer, until thecompletion ofthenext buffer transfer which isa
non-CSHOLD ornon-LOCK buffer, theTransfer Group cannot beinterrupted byanyhigher priority
TGs.
*Anentire sequence ofbuffer transfer forNOBRK DMA buffer cannot beinterrupted byanyhigher
priority TG.
*Once thelastbuffer inaTransfer Group isprefetched, ahigher priority TGcannot interrupt ituntil the
completion oftheTransfer Group.
These prioritizations made among thetransfer groups also decide thearbitration logic among themultiple
transfer groups which areactive
28.2.6.8.4 Transmission Lock Capability
Some slave devices require tohave "command "followed by"data".Inthiscase theSPItransaction should
notbeinterrupted byanother group transfer. The LOCK bitwithin each buffer allows consecutive transfer
tohappen without being interrupted byanother higher priority group transfer.

<!-- Page 1529 -->

www.ti.com Basic Operation
1529 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.7 General-Purpose I/O
AlloftheSPIpins may beprogrammed viatheSPIPCx control registers tobeeither functional orgeneral-
purpose I/Opins.
IftheSPIfunction istobeused, application software must ensure thatatleast theSPICLK pinandthe
SOMI and/or SIMO pins areconfigured asSPIfunctional pins, andnotasGIO pins, orelse theSPIstate
machine willbeheld inreset, preventing SPItransactions.
SPIpins support:
*internal pull-up resistors
*internal pull-down resistors
*open-drain orpush-pull mode
*input-buffer enabling/disabling (controlled bythePULDIS andPSEL bits)
28.2.8 Low-Power Mode
The SPIcanbeputintoeither local orglobal low-power mode. Global low-power mode isasserted bythe
system andisnotcontrolled bytheSPI. During global low-power mode, allclocks totheSPIareturned
off,making themodule completely inactive.
Local low-power mode isasserted bysetting thePOWERDOWN (SPIGCR1[8]) bit;setting thisbitstops
theclocks totheSPIinternal logic andregisters. Setting thePOWERDOWN bitcauses theSPItoenter
local low-power mode andclearing thePOWERDOWN bitcauses SPItoexitfrom local low-power mode.
Allregisters remain accessible during local power-down mode, since theclock totheSPIregisters is
temporarily re-enabled foreach access. RAM buffers arealso accessible during lowpower mode.
NOTE: Since entering alow-power mode hastheeffect ofsuspending allstate-machine activities,
care must betaken when entering such modes toensure thatavalid state isentered when
low-power mode isactive. Application software must ensure thatalowpower mode isnot
entered during adata transfer.
28.2.9 Safety Features
28.2.9.1 Detection ofSlave Desynchronization (Master Only)
When aslave supports generation ofanenable signal (ENA), desynchronization canbedetected. With
theenable signal aslave indicates tothemaster thatitisready toexchange data. Adesynchronization
canoccur ifoneormore clock edges aremissed bytheslave. Inthiscase theslave may block theSOMI
lineuntil itdetects clock edges corresponding tothenext data word. This would corrupt thedata word of
thedesynchronized slave andtheconsecutive data word. Aconfigurable 8-bit time-out counter
(T2EDELAY), which isclocked with SPICLK, isimplemented todetect thisslave malfunction. After the
transmission hasfinished (end oflastbittransferred: either lastdata bitorparity bit)thecounter isstarted.
IftheENA signal generated bytheslave does notbecome inactive before thecounter overflows, the
DESYNC flagissetandaninterrupt isasserted (ifenabled).
NOTE: Inconsistency ofDesynchronization Flag inCompatibility Mode MibSPI
Because ofthenature ofthiserror, under some circumstances itispossible foradesync
error detected fortheprevious buffer tobevisible inthecurrent buffer. This isduetothefact
thatreceive completion flag/interrupt willbegenerated when thebuffer transfer iscompleted.
Butdesync willbedetected after thebuffer transfer iscompleted. So,ifVBUS master reads
thereceived data quickly when anRXINT isdetected, then thestatus flagmay notreflect the
correct desynchronization condition. This inconsistency inthedesync flagisvalid only in
compatibility mode ofMibSPI. Inmulti-buffer mode, thedesync flagisalways assured tobe
forthecurrent buffer.

<!-- Page 1530 -->

Basic Operation www.ti.com
1530 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.9.2 ENA Signal Time-Out (Master Only)
The SPIinmaster mode waits forthehardware handshake signal (ENA) coming from theaddressed slave
before performing adata transfer. Toavoid stalling theSPIbyanon-responsive slave device, atime-out
value canbeconfigured using C2EDELAY. Ifthetime-out counter overflows before anactive ENA signal
issampled, theTIMEOUT flaginthestatus register SPIFLG issetandtheTIMEOUT flaginthestatus
field ofthecorresponding buffer isset.
NOTE: When thechip select signal becomes active, nobreaks intransmission areallowed. The
next arbitration isperformed while waiting forthetime-out tooccur.
28.2.9.3 Data-Length Error
AnSPIcangenerate anerror flagbydetecting anymismatch inlength ofreceived ortransmitted data and
theprogrammed character length under certain conditions.
Data-Length Error inMaster Mode :During adata transfer, iftheSPIdetects ade-assertion ofthe
SPIENA pin(bytheslave) while thecharacter counter isnotoverflowed, then anerror flagissetto
indicate adata-length error. This canbecaused byaslave receiving extra clocks (forexample, dueto
noise ontheSPICLK line).
NOTE: Inamaster mode SPI, thedata length error willbegenerated only iftheSPIENA pinis
enabled asafunctional pin.
Data-Length Error inSlave Mode :During atransfer, iftheSPIdetects ade-assertion oftheSPICS pin
before itscharacter length counter overflows, then anerror flagissettoindicate adata-length error. This
situation canarise Iftheslave SPImisses oneormore SPICLK pulses from themaster. This error inslave
mode implies thatboth thetransmitted andreceived data were notcomplete.
NOTE: Inaslave-mode SPI, thedata-length error flagwillbegenerated only ifatleast oneofthe
SPICS pins areconfigured asfunctional, andarebeing used forselecting theslave.
28.2.9.4 Continuous Self-Test (Master/Slave)
During data transfer, theSPIcompares itsown internal transmit data with itstransmit data onthebus. The
sample point forthecompare isatone-half SPIclock after transmit point. Ifthedata onthebusdoes not
match theexpected value, thebit-error (BITERR) flagissetandaninterrupt isasserted ifenabled.
NOTE: The compare ismade from theoutput pinusing itsinput buffer.

<!-- Page 1531 -->

www.ti.com Basic Operation
1531 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.10 Test Features
28.2.10.1 Internal Loop-Back Test Mode (Master Only)
The internal loop-back self-test mode canbeutilized totesttheSPItransmit andreceive paths, including
theshift registers, theSPIbuffer registers, andtheparity generator. Inthismode thetransmit signal is
internally feedback tothereceiver, whereas theSIMO, SOMI, andCLK pinaredisconnected; thatis,the
transmitted data isinternally transferred tothecorresponding receive buffer while external signals remain
unchanged.
This mode allows theCPU towrite intothetransmit buffer, andcheck thatthereceive buffer contains the
correct transmit data. Ifanerror occurs thecorresponding error issetwithin thestatus field.
NOTE: This mode cannot bechanged during transmission.
28.2.10.2 Input/Output Loopback Test Mode
Input/Output Loopback Test mode supports thetesting ofallInput/Output pins without theaidofan
external interface. Loopback canbeconfigured aseither analog-loopback (loopback through thepin-level
input/output buffers) ordigital loopback (internal totheSPImodule). With Input/Output Loopback, all
functional features oftheSPIcanbetested. Transmit data isfedback through thereceive-data line(s).
See Figure 28-31 foradiagram ofthetypes offeedback available. The IOLPBKTSTCR register defines all
oftheavailable control fields.
Inloopback mode, itisalso possible toinduce various error conditions. See Section 28.3.43 fordetails of
theregister field controlling these features.
InInput/Output loopback testmodes, even when themodule isinslave mode, theSPICLK isgenerated
internally. This SPICLK isused forallloopback-mode SPItransactions. Slave-mode features canbe
tested without thehelp ofanother master SPI, using theinternally-generated SPICLK. Chip selects are
also generated bytheslave itself while itisinInput/Output loopback mode.
InInput/Output loopback testmodes, ifthemodule isinmaster mode, theENA signal isalso generated by
internal logic sothatanexternal interface isnotrequired.
NOTE: Usage Guideline forInput/Output Loopback
Input/Output Loopback mode should beused with caution because, insome configurations,
even thereceive pins willbedriven with transmit data. During testing, itshould beensured
thatnone oftheSPIpins aredriven byanyother device connected tothem. Otherwise, if
analog loopback isselected inI/OLoopback mode, then testing may damage thedevice.

<!-- Page 1532 -->

This diagram is intended to illustrate loopback paths and therefore m ay omit some normal-mode paths.tr
ReeeeeeeeeeTX
RXTX SHIFT REG
RX SHIFT REG
Checks the analog loopback path through the transmit bufferChecks the analog loopback path through the receive buffer
Digital loopback pathRXP_ENALPBK_TYPE
Basic Operation www.ti.com
1532 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-31. I/OPaths During I/OLoopback Modes
28.2.10.2.1 Input/Output Loopback Mode Operation inSlave Mode
Inmulti-buffer slave mode, there aresome additional requirements forusing I/Oloopback mode (IOLPBK).
Inmulti-buffer slave mode, thechip-select pins arethetriggers forvarious TGs. Enabling theIOLPBK
mode bywriting 0xA totheIOLPBTSTENA bitsoftheIOLPBKTSTCR register triggers TG0 bydriving
SPICS to0.The actual number ofchip selects canbeprogrammed tohave anyoralloftheSPICS pins
asfunctional. Allother configurations should becompleted before enabling theIOLPBK mode inmulti-
buffer slave mode since ittriggers TG0.
After thefirstbuffer transfer iscompleted, theCSNR field ofthecurrent buffer isused totrigger thenext
buffer. So,ifmultiple TGs aredesired tobetested, then theCSNR field ofthefinal buffer ineach TG
should hold thenumber ofthenext TGtobetriggered. Aslong asTGboundaries arewell defined andare
enabled, thecompletion ofoneTGwilltrigger thenext TG.
Tostop thetransfer inmulti-buffer slave mode inI/OLoopback configuration, either IOLPBK mode canbe
disabled bywriting 0x5totheIOLPBTSTENA bitsoralloftheTGs canbedisabled.

<!-- Page 1533 -->

www.ti.com Basic Operation
1533 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.11 Module Configuration
MibSPI/MibSPIP canbeconfigured tofunction asNormal SPIandMulti-buffered SPI. Upon power-up ora
system-level reset, each bitinthemodule registers issettoadefault state. The registers arewritable only
after theRESET bitissetto1.
28.2.11.1 Compatibility (SPI) Mode Configuration
The following listdetails theconfiguration steps thatsoftware should perform prior tothetransmission or
reception ofdata. Aslong astheSPIEN bitintheGlobal Control Register 1(SPIGCR1) iscleared to0the
entire time thattheSPIisbeing configured, theorder inwhich theregisters areprogrammed isnot
important.
*Enable SPIbysetting RESET bit.
*Configure theSIMO, SOMI, CLK, andoptional SPICS andSPIENA pins forSPIfunctionality bysetting
thecorresponding bitinSPIPC0 register.
*Configure themodule tofunction asMaster orSlave using CLKMOD andMASTER bits.
*Configure therequired SPIdata format using SPIFMTx register.
*Ifthemodule isselected tofunction asMaster, thedelay parameters canbeconfigured using
SPIDELAY register.
*Enable theInterrupts using SPIINT0 register ifrequired.
*Select thechip select tobeused bysetting CSNR bitsinSPIDAT1 register.
*Configure CSHOLD andWDEL bitsinSPIDAT1 register ifrequired.
*Select theData word format bysetting DFSEL bits. Select theNumber oftheconfigured SPIFMTx
register (0to3)toused forthecommunication.
*SetLOOPBACK bittoconnect thetransmitter tothereceiver internally. (This feature isused toperform
aself-test. Donotconfigure fornormal communication toexternal devices).
*SetSPIEN bitto1after theSPIisconfigured.
*Perform Transmit andreceive data, using SPIDAT1 andSPIBUF register.
*You must wait forTXFULL toreset orTXINT before writing next data toSPIDAT1 register.
*You must wait forRXEMPTY toreset orRXINT before reading thedata from SPIBUF register.

<!-- Page 1534 -->

Basic Operation www.ti.com
1534 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.2.11.2 MibSPI Mode Configuration
The following listdetails theconfiguration steps thatsoftware should perform prior tothetransmission or
reception ofdata inMIBSPI mode. Aslong astheSPIEN bitintheGlobal Control Register 1(SPIGCR1)
iscleared to0theentire time thattheSPIisbeing configured, theorder inwhich theregisters are
programmed isnotimportant.
*Enable SPIbysetting RESET bit.
*SetMSPIENA bitto1togetaccess tomulti-buffer mode registers.
*Configure theSIMO, SOMI, CLK, andoptional SPICS andSPIENA pins forSPIfunctionality bysetting
thecorresponding bitinSPIPC0 register.
*Configure themodule tofunction asMaster orSlave using CLKMOD andMASTER bits.
*Configure therequired SPIdata format using SPIFMTx register.
*Ifthemodule isselected tofunction asMaster, thedelay parameters canbeconfigured using
SPIDELAY register.
*Check forBUFINITACTIVE bittobeactive before configuring MIBSPI RAM. (From Device Power Onit
take Number ofBuffers ×Peripheral clock period toinitialize complete RAM.)
*Enable theTransfer Group interrupts using TGITENST register ifrequired.
*Enable error interrupts using SPIINT0 register ifrequired.
*SetSPIEN bitto1after theSPIisconfigured.
*The Trigger Source, Trigger Event, Transfer Group start address forthecorresponding Transfer
groups canbeconfigured using thecorresponding TGxCTRL register.
*Configure LPEND tospecify theendaddress ofthelastTG.
*Similar toSPIDAT1 register, the16bitcontrol fields inevery TXRAM buffer intheTGhave tobe
configured.
*Configure oneoftheeight BUFMODE available foreach buffer.
*Fillthedata tobetransmitted inTXDATA field inTXRAM buffers.
*Configure TGENA bittoenable therequired Transfer groups. (Incase ofTrigger event always setting
TGENA willtrigger thetransfer group).
*Attheoccurrence ofthecorrect trigger event, theTransfer group willbetriggered anddata gets
transmitted andreceived oneafter theother with outanyCPU intervention.
*You canpollTransfer group interrupt flagorwait foratransfer-completed interrupt toread andwrite
new data tothebuffers.

<!-- Page 1535 -->

www.ti.com Control Registers
1535 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3 Control Registers
This section describes theSPIcontrol, data, andpinregisters. The registers support 8-bit, 16-bit and32-
bitwrites. The offset isrelative totheassociated base address ofthismodule inasystem. The base
address forthecontrol registers isFFF7 F400h forMibSPI1, FFF7 F600h forMibSPI2, FFF7 F800h for
MibSPI3, FFF7 FA00h forMibSPI4, andFFF7 FC00h forMibSPI5.
NOTE: TIhighly recommends thatwrite values corresponding tothereserved locations ofregisters
bemaintained as0consistently. This allows future enhancements tousethese reserved bits
ascontrol bitswithout affecting thefunctionality ofthemodule with anyolder versions of
software.
(1)SPIPC9 only applies toSPI2.Table 28-8. SPIRegisters
Offset Acronym Register Description Section
00h SPIGCR0 SPIGlobal Control Register 0 Section 28.3.1
04h SPIGCR1 SPIGlobal Control Register 1 Section 28.3.2
08h SPIINT0 SPIInterrupt Register Section 28.3.3
0Ch SPILVL SPIInterrupt Level Register Section 28.3.4
10h SPIFLG SPIFlag Register Section 28.3.5
14h SPIPC0 SPIPinControl Register 0 Section 28.3.6
18h SPIPC1 SPIPinControl Register 1 Section 28.3.7
1Ch SPIPC2 SPIPinControl Register 2 Section 28.3.8
20h SPIPC3 SPIPinControl Register 3 Section 28.3.9
24h SPIPC4 SPIPinControl Register 4 Section 28.3.10
28h SPIPC5 SPIPinControl Register 5 Section 28.3.11
2Ch SPIPC6 SPIPinControl Register 6 Section 28.3.12
30h SPIPC7 SPIPinControl Register 7 Section 28.3.13
34h SPIPC8 SPIPinControl Register 8 Section 28.3.14
38h SPIDAT0 SPITransmit Data Register 0 Section 28.3.15
3Ch SPIDAT1 SPITransmit Data Register 1 Section 28.3.16
40h SPIBUF SPIReceive Buffer Register Section 28.3.17
44h SPIEMU SPIEmulation Register Section 28.3.18
48h SPIDELAY SPIDelay Register Section 28.3.19
4Ch SPIDEF SPIDefault Chip Select Register Section 28.3.20
50h-5Ch SPIFMT0-SPIFMT3 SPIData Format Registers Section 28.3.21
60h INTVECT0 Interrupt Vector 0 Section 28.3.22
64h INTVECT1 Interrupt Vector 1 Section 28.3.23
68h SPIPC9(1)SPIPinControl Register 9 Section 28.3.24
6Ch SPIPMCTRL Parallel/Modulo Mode Control Register Section 28.3.25
70h MIBSPIE Multi-buffer Mode Enable Register Section 28.3.26
74h TGITENST TGInterrupt Enable SetRegister Section 28.3.27
78h TGITENCR TGInterrupt Enable Clear Register Section 28.3.28
7Ch TGITLVST Transfer Group Interrupt Level SetRegister Section 28.3.29
80h TGITLVCR Transfer Group Interrupt Level Clear Register Section 28.3.30
84h TGINTFLG Transfer Group Interrupt Flag Register Section 28.3.31
90h TICKCNT Tick Count Register Section 28.3.32
94h LTGPEND Last TGEnd Pointer Section 28.3.33
98h-D4h TGxCTRL TGx Control Registers Section 28.3.34
D8h-F4h DMAxCTRL DMA Channel Control Registers Section 28.3.35
F8h-114h ICOUNT DMAxCOUNT Registers Section 28.3.36

<!-- Page 1536 -->

Control Registers www.ti.com
1536 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-8. SPIRegisters (continued)
Offset Acronym Register Description Section
118h DMACNTLEN DMA Large Count Register Section 28.3.37
120h PAR_ECC_CTRL Parity/ECC Control Register Section 28.3.38
124h PAR_ECC_STAT Parity/ECC Status Register Section 28.3.39
128h UERRADDR1 Uncorrectable Parity orDouble-Bit ECC Error
Address Register -RXRAMSection 28.3.40
12Ch UERRADDR0 Uncorrectable Parity orDouble-Bit ECC Error
Address Register -TXRAMSection 28.3.41
130h RXOVRN_BUF_ADDR RXRAM Overrun Buffer Address Register Section 28.3.42
134h IOLPBKTSTCR I/OLoopback Test Control Register Section 28.3.43
138h EXTENDED_PRESCALE1 SPIExtended Prescale Register 1 Section 28.3.44
13Ch EXTENDED_PRESCALE2 SPIExtended Prescale Register 2 Section 28.3.45
140h ECCDIAG_CTRL ECC Diagnostic Control Register Section 28.3.46
144h ECCDIAG_STAT ECC Diagnostic Status Register Section 28.3.47
148h SBERRADDR1 Single-Bit Error Address Register -RXRAM Section 28.3.48
152h SBERRADDR0 Single-Bit Error Address Register -TXRAM Section 28.3.49
28.3.1 SPIGlobal Control Register 0(SPIGCR0)
Figure 28-32. SPIGlobal Control Register 0(SPIGCR0) [offset =00h]
31 16
Reserved
R-0
15 1 0
Reserved nRESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-9. SPIGlobal Control Register 0(SPIGCR0) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 nRESET This isthelocal reset control forthemodule. This bitneeds tobesetto1before anyoperation onSPI/
MibSPI canbedone. Only after setting thisbitto1,theAuto Initialization ofMulti-buffer RAM starts.
Clearing thisbitto0willresult inallofthecontrol andstatus register values toreturn totheir default
values..
0 SPIisinthereset state.
1 SPIisoutofthereset state.

<!-- Page 1537 -->

www.ti.com Control Registers
1537 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.2 SPIGlobal Control Register 1(SPIGCR1)
Figure 28-33. SPIGlobal Control Register 1(SPIGCR1) [offset =04h]
31 25 24 23 17 16
Reserved SPIEN Reserved LOOPBACK
R-0 R/W-0 R-0 R/WP-0
15 9 8 7 2 1 0
Reserved POWERDOWN Reserved CLKMOD MASTER
R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-10. SPIGlobal Control Register 1(SPIGCR1) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 SPIEN SPIenable. This bitenables SPItransfers. This bitmust besetto1after allother SPIconfiguration
bitshave been written. When theSPIEN bitis0orcleared to0,thefollowing SPIregisters get
forced totheir default states:
*Both TXandRXshift registers
*The TXDATA fields oftheSPITransmit Data Register 0(SPIDAT0) andtheSPITransmit Data
Register 1(SPIDAT1)
*Allthefields oftheSPIFlag Register (SPIFLG)
*Contents ofSPIBUF andtheinternal RXBUF registers
0 The SPIisnotactivated fortransfers.
1 Activates SPI.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 LOOPBACK Internal loop-back testmode. The internal self-test option canbeenabled bysetting thisbit.Ifthe
SPISIMO andSPISOMI pins areconfigured with SPIfunctionality, then theSPISIMO[7:0] pins are
internally connected totheSPISOMI[7:0] pins (transmit data islooped back asreceive data). GIO
mode forthese pins isnotsupported inloopback mode. Externally, during loop-back operation, the
SPICLK pinoutputs aninactive value andSPISOMI[7:0] remains inthehigh-impedance state. If
theSPIisinitialized inslave mode oradata transfer isongoing, errors may result.
Note: This loopback mode canonly beused inmaster mode. Master mode must beselected
before setting LOOPBACK. When thismode isselected, theCLKMOD bitshould besetto1,
meaning thatSPICLK isinternally generated.
0 Internal loop-back testmode isdisabled.
1 Internal loop-back testmode isenabled.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 POWERDOWN When active, theSPIstate machine enters apower-down state.
0 The SPIisinactive mode.
1 The SPIisinpower-down mode.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CLKMOD Clock mode. This bitselects either aninternal orexternal clock source. This bitalso determines the
I/Odirection oftheSPIENA andSPICS pins infunctional mode.
0 Clock isexternal.
*SPIENA isanoutput.
*SPICS areinputs.
1 Clock isinternally-generated.
*SPIENA isaninput.
*SPICS areoutputs.

<!-- Page 1538 -->

Control Registers www.ti.com
1538 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-10. SPIGlobal Control Register 1(SPIGCR1) Field Descriptions (continued)
Bit Field Value Description
0 MASTER SPISIMO/SPISOMI pindirection determination. Sets thedirection oftheSPISIMO andSPISOMI
pins.
Note: Formaster-mode operation oftheSPI, MASTER bitshould besetto1andCLKMOD
bitcanbeseteither 1or0.Themaster-mode SPIcanrunonanexternal clock onSPICLK.
Forslave mode operation, both theMASTER andCLKMOD bitsshould becleared to0.Any
other combinations may result inunpredictable behavior oftheSPI. Inslave mode. SPICLK
willnotbegenerated internally inslave mode.
0 SPISIMO[7:0] pins areinputs, SPISOMI[7:0] pins areoutputs.
1 SPISOMI[7:0] pins areinputs, SPISIMO[7:0] pins areoutputs.
28.3.3 SPIInterrupt Register (SPIINT0)
Figure 28-34. SPIInterrupt Register (SPIINT0) [offset =08h]
31 25 24
Reserved ENABLEHIGHZ
R-0 R/W-0
23 17 16
Reserved DMAREQEN
R-0 R/W-0
15 10 9 8
Reserved TXINTENA RXINTENA
R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
Reserved RXOVRNINT
ENAReserved BITERR
ENADESYNC
ENAPARERR
ENATIMEOUT
ENADLENERR
ENA
R-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-11. SPIInterrupt Register (SPIINT0) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ENABLEHIGHZ SPIENA pinhigh-impedance enable. When active, theSPIENA pin(when itisconfigured asa
WAIT functional output signal inaslave SPI) isforced tohigh-impedance when notdriving a
lowsignal. Ifinactive, then thepinwilloutput both ahigh andalowsignal.
0 SPIENA pinispulled high when notactive.
1 SPIENA pinremains high-impedance when notactive.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 DMAREQEN DMA request enable. Enables theDMA request signal tobegenerated forboth receive and
transmit channels. Enable DMA REQ only after setting theSPIEN bitto1.
0 DMA isnotused.
1 DMA requests willbegenerated.
Note: ADMA request willbegenerated ontheTXDMA REQ lineeach time aword is
copied totheshift register either from TXBUF ordirectly from SPIDAT0/SPIDAT1 writes.
Note: ADMA request willbegenerated ontheRXDMA REQ lineeach time aword is
copied totheSPIBUF register either from RXBUF ordirectly from theshift register.
15-10 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1539 -->

www.ti.com Control Registers
1539 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-11. SPIInterrupt Register (SPIINT0) Field Descriptions (continued)
Bit Field Value Description
9 TXINTENA Causes aninterrupt tobegenerated every time data iswritten totheshift register, sothatthe
next word canbewritten toTXBUF. Setting thisbitwillgenerate aninterrupt iftheTXINTFLG
bit(SPI Flag Register (SPIFLG)[9]) issetto1.
0 Nointerrupt willbegenerated upon TXINTFLG being setto1.
1 Aninterrupt willbegenerated upon TXINTFLG being setto1.
The transmitter empty interrupt isvalid incompatibility mode ofSPIonly. Inmulti-buffered
mode, thisinterrupts willnotbegenerated, even ifitisenabled.
Note: Aninterrupt request willbegenerated assoon asthisbitissetto1.Bydefault it
willbegenerated ontheINT0 line. TheSPILVL register canbeprogrammed tochange
theinterrupt line.
8 RXINTENA Causes aninterrupt tobegenerated when theRXINTFLAG bit(SPI Flag Register (SPIFLG)[8])
issetbyhardware.
0 Interrupt willnotbegenerated.
1 Interrupt willbegenerated.
The receiver fullinterrupt isvalid incompatibility mode ofSPIonly. Inmulti-buffered mode, this
interrupts willnotbegenerated, even ifitisenabled.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 RXOVRNINTENA Overrun interrupt enable.
0 Overrun interrupt willnotbegenerated.
1 Overrun interrupt willbegenerated.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 BITERRENA Enables interrupt onbiterror.
0 Nointerrupt asserted upon biterror.
1 Enables interrupt onbiterror.
3 DESYNCENA Enables interrupt ondesynchronized slave. DESYNCENA isused inmaster mode only.
0 Nointerrupt asserted upon desynchronization error.
1 Aninterrupt isasserted ondesynchronization oftheslave (DESYNC =1).
2 PARERRENA Enables interrupt-on-parity-error.
0 Nointerrupt asserted onparity error.
1 Aninterrupt isasserted onaparity error.
1 TIMEOUTENA Enables interrupt onENA signal time-out.
0 Nointerrupt asserted upon ENA signal time-out.
1 Aninterrupt isasserted onatime-out oftheENA signal.
0 DLENERRENA Data length error interrupt enable. Adata length error occurs under thefollowing conditions.
Master: When SPIENA isused, iftheSPIENA pinfrom theslave isdeasserted before the
master hascompleted itstransfer, thedata length error isset.That is,ifthecharacter length
counter hasnotoverflowed while SPIENA deassertion isdetected, then itmeans thattheslave
hasneither received fulldata from themaster norhasittransmitted complete data.
Slave: When SPICS pins areused, iftheincoming valid SPICS pinisdeactivated before the
character length counter overflows, then thedata length error isset.
0 Nointerrupt isgenerated upon data length error.
1 Aninterrupt isasserted when adata-length error occurs.

<!-- Page 1540 -->

Control Registers www.ti.com
1540 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.4 SPIInterrupt Level Register (SPILVL)
Figure 28-35. SPIInterrupt Level Register (SPILVL) [offset =0Ch]
31 16
Reserved
R-0
15 10 9 8
Reserved TXINTLVL RXINTLVL
R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
Reserved RXOVRNINTL Reserved BITERRLVL DESYNCLVL PARERRLVL TIMEOUTLVL DLENERRLVL
R-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-12. SPIInterrupt Level Register (SPILVL) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9 TXINTLVL Transmit interrupt level.
0 Transmit interrupt ismapped tointerrupt lineINT0.
1 Transmit interrupt ismapped tointerrupt lineINT1.
8 RXINTLVL Receive interrupt level.
0 Receive interrupt ismapped tointerrupt lineINT0.
1 Receive interrupt ismapped tointerrupt lineINT1.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 RXOVRNINTLVL Receive overrun interrupt level.
0 Receive overrun interrupt ismapped tointerrupt lineINT0.
1 Receive overrun interrupt ismapped tointerrupt lineINT1.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 BITERRLVL Biterror interrupt level.
0 Biterror interrupt ismapped tointerrupt lineINT0.
1 Biterror interrupt ismapped tointerrupt lineINT1.
3 DESYNCLVL Desynchronized slave interrupt level. (master mode only).
0 Aninterrupt caused bydesynchronization oftheslave ismapped tointerrupt lineINT0.
1 Aninterrupt caused bydesynchronization oftheslave ismapped tointerrupt lineINT1.
2 PARERRLVL Parity error interrupt level.
0 Aparity error interrupt ismapped tointerrupt lineINT0.
1 Aparity error interrupt ismapped tointerrupt lineINT1.
1 TIMEOUTLVL SPIENA pintime-out interrupt level.
0 Aninterrupt onatime-out oftheENA signal (TIMEOUT =1)ismapped tointerrupt lineINT0.
1 Aninterrupt onatime-out oftheENA signal (TIMEOUT =1)ismapped tointerrupt lineINT1.
0 DLENERRLVL Data length error interrupt level (line) select.
0 Aninterrupt ondata length error ismapped tointerrupt lineINT0.
1 Aninterrupt ondata length error ismapped tointerrupt lineINT1.

<!-- Page 1541 -->

www.ti.com Control Registers
1541 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.5 SPIFlag Register (SPIFLG)
Software must check allflagbitswhen reading thisregister.
Figure 28-36. SPIFlag Register (SPIFLG) [offset =10h]
31 25 24 23 16
Reserved BUFINIT
ACTIVEReserved
R-0 R-0 R-0
15 10 9 8
Reserved TXINTFLG RXINTFLG
R-0 R-0 R/W1C-0
7 6 5 4 3 2 1 0
Reserved RXOVRNINT
FLGReserved BITERR
FLGDESYNC
FLGPARERR
FLGTIMEOUT
FLGDLENERR
FLG
R-0 R/W1C-0 R-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 28-13. SPIFlag Register (SPIFLG) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 BUFINITACTIVE Indicates thestatus ofmulti-buffer initialization process. Software canpollforthisbitto
determine ifitcanproceed with theregister configuration ofmulti-buffer mode registers orbuffer
handling.
Note: IftheSPIFLG register isread while themulti-buffer RAM isbeing initialized, the
BUF INIT ACTIVE bitwillberead as1.IfSPIFLG isread after theinternal automatic
buffer initialization iscomplete, thisbitwillberead as0.This bitwillshow avalue of1
aslong asthenRESET bitis0,butdoes notreally indicate thatbuffer initialization is
underway. Buffer initialization starts only when thenRESET bitissetto1.
0 Multi-buffer RAM initialization iscomplete.
1 Multi-buffer RAM isstillbeing initialized. Donotattempt towrite toeither multi-buffer RAM or
anymulti-buffer mode registers.
23-10 Reserved 0 Reads return 0.Writes have noeffect.
9 TXINTFLG Transmitter-empty interrupt flag. Serves asaninterrupt flagindicating thatthetransmit buffer
(TXBUF) isempty andanew word canbewritten toit.This flagissetwhen aword iscopied to
theshift register either directly from SPIDAT0/SPIDAT1 orfrom theTXBUF register. This bitis
cleared byoneoffollowing methods:
*Writing anew data toeither SPIDAT0 orSPIDAT1
*Writing a0toSPIEN (SPIGCR1[24])
0 Transmit buffer isnow full.Nointerrupt pending fortransmitter empty.
1 Transmit buffer isempty. Aninterrupt ispending tofillthetransmitter.

<!-- Page 1542 -->

Control Registers www.ti.com
1542 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-13. SPIFlag Register (SPIFLG) Field Descriptions (continued)
Bit Field Value Description
8 RXINTFLG Receiver-full interrupt flag. This flagissetwhen aword isreceived andcopied intothebuffer
register (SPIBUF). IfRXINTEN isenabled, aninterrupt isalso generated. This bitiscleared
under thefollowing methods:
*Reading theSPIBUF register
*Reading TGINTVECT0 orTGINTVECT1 register when there isareceive buffer fullinterrupt
*Writing a1tothisbit
*Writing a0toSPIEN (SPIGCR1[24])
*System reset
During emulation mode, however, aread totheemulation register (SPIEMU) does notclear this
flagbit.
0 Nonew received data pending. Receive buffer isempty.
1 Anewly received data isready toberead. Receive buffer isfull.
Note: Clearing RXINTFLG bitbywriting a1before reading theSPIBUF sets theRXEMPTY
bitoftheSPIBUF register too.Inthisway, onecanignore areceived word. However, if
theinternal RXBUF isalready full,thedata from RXBUF willbecopied toSPIBUF andthe
RXEMPTY bitwillbecleared again. TheSPIBUF contents should beread first ifthis
situation needs tobeavoided.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 RXOVRNINTFLG Receiver overrun flag. The SPIhardware sets thisbitwhen areceive operation completes
before theprevious character hasbeen read from thereceive buffer. The bitindicates thatthe
lastreceived character hasbeen overwritten andtherefore lost. The SPIwillgenerate an
interrupt request ifthisbitissetandtheRXOVRN INTEN bit(SPIINT0.6) issethigh. This bitis
cleared under thefollowing conditions incompatibility mode ofMibSPI:
*Reading TGINTVECT0 orTGINTVECT1 register when there isareceive-buffer-overrun
interrupt
*Writing a1toRXOVRNINTFLG intheSPIFlag Register (SPIFLG) itself
*Writing a0toSPIEN
*Reading thedata field oftheSPIBUF register
Note: Reading theSPIBUF register does notclear thisRXOVRNINTFLG bit.IfanRXOVRN
interrupt isdetected, then theSPIBUF may need toberead twice togettotheoverrun
buffer. This isduetothefactthattheoverrun willalways occur totheinternal RXBUF.
Each read totheSPIBUF willresult inRXBUF contents (ifitisfull)getting copied to
SPIBUF.
Note: There isaspecial condition under which theRXOVRNINTFLG flaggets set.Ifboth
SPIBUF andRXBUF arealready fullandwhile another reception isunderway, ifany
errors (TIMEOUT, BITERR, andDLEN_ERR) occur, then RXOVR inRXBUF and
RXOVRNINTFLG inSPIFLG registers willbesettoindicate thatthestatus flags are
getting overwritten bythenew transfer. This overrun should betreated likeareceive
overrun.
Inmulti-buffer mode ofMibSPI, thisbitiscleared under thefollowing conditions:
*Reading theRXOVRN_BUF_ADDR register
*Writing a1toRXOVRNINTFLG intheSPIFlag Register (SPIFLG) itself
Inmulti-buffer mode, ifRXOVRNINTFLG isset,then theaddress ofthebuffer which
experienced theoverrun isavailable inRXOVRN_BUF_ADDR.
0 Overrun condition didnotoccur.
1 Overrun condition hasoccurred.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 BITERRFLG Mismatch ofinternal transmit data andtransmitted data. This flagcanbecleared byoneofthe
following methods:
*Write a1tothisbit.
*Clear theSPIEN bitto0.
0 Nobiterror occurred.
1 Abiterror occurred. The SPIsamples thesignal ofthetransmit pin(master: SIMO, slave:
SOMI) atthereceive point (half clock cycle after transmit point). Ifthesampled value differs
from thetransmitted value abiterror isdetected andtheflagBITERRFLG isset.IfBITERRENA
issetaninterrupt isasserted. Possible reasons forabiterror canbeanexcessively high bit
rate, capacitive load, oranother master/slave trying totransmit atthesame time.

<!-- Page 1543 -->

www.ti.com Control Registers
1543 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-13. SPIFlag Register (SPIFLG) Field Descriptions (continued)
Bit Field Value Description
3 DESYNCFLG Desynchronization ofslave device. Desynchronization monitor isactive inmaster mode only.
This flagcanbecleared byoneofthefollowing methods:
*Write a1tothisbit.
*Clear theSPIEN bitto0.
0 Noslave desynchronization detected.
1 Aslave device isdesynchronized. The master monitors theENAble signal coming from the
slave device andsets theDESYNC flagafter thelastbitistransmitted plus tT2EDELAY .If
DESYNCENA issetaninterrupt isasserted. Desynchronization canoccur ifaslave device
misses aclock edge coming from themaster.
2 PARERRFLG Calculated parity differs from received parity bit.Iftheparity generator isenabled (can be
selected individually foreach buffer) aneven oroddparity bitisadded attheendofadata
word. During reception ofthedata word theparity generator calculates thereference parity and
compares ittothereceived parity bit.Intheevent ofamismatch thePARITYERR flagisset
andaninterrupt isasserted ifPARERRENA isset.This flagcanbecleared byoneofthe
following methods:
*Write a1tothisbit.
*Clear theSPIEN bitto0.
0 Noparity error detected.
1 Aparity error occurred.
1 TIMEOUTFLG Time-out caused bynonactivation ofENA signal. This flagcanbecleared byoneofthe
following methods:
*Write a1tothisbit.
*Clear theSPIEN bitto0.
0 NoENA-signal time-out occurred.
1 AnENA signal time-out occurred. The SPIgenerates atime-out because theslave hasnot
responded intime byactivating theENA signal after thechip select signal hasbeen activated. If
atime-out condition isdetected thecorresponding chip select isdeactivated immediately and
theTIMEOUT flagisset.Inaddition theTIMEOUT flaginthestatus field ofthecorresponding
buffer isset.The transmit request oftheconcerned buffer iscleared, thatis,theSPIdoes not
re-start adata transfer from thisbuffer.
0 DLENERRFLG Data-length error flag. This flagcanbecleared byoneofthefollowing methods:
*Write a1tothisbit.
*Clear theSPIEN bitto0.
Note: Whenever anytransmission errors (TIMEOUT, BITERR, DLEN_ERR, PARITY_ERR,
DESYNC) aredetected andtheerror flags arecleared bywriting totheerror bitinthe
SPIFLG register, thecorresponding error flaginSPIBUF does notgetcleared. Software
needs toread theSPIBUF until itbecomes empty before proceeding. This ensures that
alloftheoldstatus bitsinSPIBUF arecleared before starting thenext transfer.
0 Nodata length error hasoccurred.
1 Adata length error hasoccurred.

<!-- Page 1544 -->

Control Registers www.ti.com
1544 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.6 SPIPinControl Register 0(SPIPC0)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofSPIPC0 toSPIPC9 reflect thenumber ofSIMO/SOMI data
lines perdevice. Ondevices with 8data-line support, allofbits31to16areimplemented.
Ondevices with less than 8data lines, only asubset ofthese bitsareavailable.
Unimplemented bitsreturn 0upon read andarenotwritable.
Figure 28-37. SPIPinControl Register 0(SPIPC0) [offset =14h]
31 24 23 16
SOMIFUN SIMOFUN
R/W-0 R/W-0
15 12 11 10 9 8
Reserved SOMIFUN0 SIMOFUN0 CLKFUN ENAFUN
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 0
SCSFUN
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-14. SPIPinControl (SPIPC0) Field Descriptions
Bit Field Value Description
31-24 SOMIFUN Slave out,master infunction. Determines whether SPISOMI[x] istobeused asageneral-purpose I/O
pinorasaSPIfunctional pin.
Note: Duplicate Control Bits forSPISOMI[0]. Bit24isnotphysically implemented. Itisamirror
ofBit11.Any write tobit24willbereflected onbit11.When bit24andbit11are
simultaneously written, thevalue ofbit11willcontrol theSPISOMI[0] pin.Theread value of
Bit24always reflects thevalue ofbit11.
0 SPISOMI[x] pinisaGIO pin.
1 SPISOMI[x] pinisaSPIfunctional pin.
23-16 SIMOFUN Slave in,master outfunction. Determines whether SPISIMO[x] istobeused asageneral-purpose I/O
pinorasaSPIfunctional pin.
Note: Duplicate Control Bits forSPISIMO[x]. Bit16isnotphysically implemented. Itisamirror
ofBit10.Any write tobit16willbereflected onbit10.When bit16andbit10are
simultaneously written, thevalue ofbit10willcontrol theSPISOMI[x] pin.Theread value of
Bit16always reflects thevalue ofbit10.
0 SPISIMOx pinisaGIO pin.
1 SPISIMOx pinisaSPIfunctional pin.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIFUN0 Slave out,master infunction. This bitdetermines whether theSPISOMI0 pinistobeused asa
general-purpose I/OpinorasaSPIfunctional pin.
0 SPISOMI0 pinisaGIO pin.
1 SPISOMI0 pinisaSPIfunctional pin.
Note: Regardless ofthenumber ofparallel pins used, theSPISOMI0 pinwillalways have tobe
programmed asfunctional pins foranySPItransfers.
10 SIMOFUN0 Slave in,master outfunction. This bitsdetermine whether each SPISIMO0 pinistobeused asa
general-purpose I/OpinorasaSPIfunctional pin.
0 SPISIMO0 pinisaGIO pin.
1 SPISIMO0 pinisaSPIfunctional pin.
Note: Regardless ofthenumber ofparallel pins used, theSPISIMO0 pinwillalways have tobe
programmed asfunctional pins foranySPItransfers.

<!-- Page 1545 -->

www.ti.com Control Registers
1545 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-14. SPIPinControl (SPIPC0) Field Descriptions (continued)
Bit Field Value Description
9 CLKFUN SPIclock function. This bitdetermines whether theSPICLK pinistobeused asageneral-purpose
I/Opin,orasaSPIfunctional pin.
0 SPICLK pinisaGIO pin.
1 SPICLK pinisaSPIfunctional pin.
8 ENAFUN SPIENA function. This bitdetermines whether theSPIENA pinistobeused asageneral-purpose I/O
pinorasaSPIfunctional pin.
0 SPIENA pinisaGIO pin.
1 SPIENA pinisaSPIfunctional pin.
7-0 SCSFUN SPICS function. Determines whether each SPICS pinistobeused asageneral-purpose I/Opinor
asaSPIfunctional pin.Iftheslave SPICS pins areinfunctional mode andreceive aninactive-high
signal, theslave SPIwillplace itsoutput inhigh-impedance anddisable shifting.
0 SPICS pinisaGIO pin.
1 SPICS pinisaSPIfunctional pin.
28.3.7 SPIPinControl Register 1(SPIPC1)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
Figure 28-38. SPIPinControl Register 1(SPIPC1) [offset =18h]
31 24 23 16
SOMIDIR SIMODIR
R/W-0 R/W-0
15 12 11 10 9 8
Reserved SOMIDIR0 SIMODIR0 CLKDIR ENADIR
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 0
SCSDIR
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-15. SPIPinControl Register (SPIPC1) Field Descriptions
Bit Field Value Description
31-24 SOMIDIR SPISOMIx direction. Controls thedirection ofSPISOMIx when used forgeneral-purpose I/O.If
SPISOMIx pinisused asaSPIfunctional pin,theI/Odirection isdetermined bytheMASTER bitin
theSPIGCR1 register.
Note: Duplicate Control Bits forSPISOMI0. Bit24isnotphysically implemented. Itisamirror
ofBit11.Any write tobit24willbereflected onbit11.When bit24andbit11are
simultaneously written, thevalue ofbit11willcontrol theSPISOMI pin.Theread value ofBit
24always reflects thevalue ofbit11.
0 SPISOMIx pinisaninput.
1 SPISOMIx pinisanoutput.

<!-- Page 1546 -->

Control Registers www.ti.com
1546 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-15. SPIPinControl Register (SPIPC1) Field Descriptions (continued)
Bit Field Value Description
23-16 SIMODIR SPISIMOx direction. Controls thedirection ofSPISIMOx when used forgeneral-purpose I/O.If
SPISIMOx pinisused asaSPIfunctional pin,theI/Odirection isdetermined bytheMASTER bitin
theSPIGCR1 register.
Note: Duplicate Control Bits forSPISIMO. Bit16isnotphysically implemented. Itisamirror of
Bit10.Any write tobit16willbereflected onbit10.When bit16andbit10aresimultaneously
written, thevalue ofbit10willcontrol theSPISOMI pin.Theread value ofBit16always
reflects thevalue ofbit10.
0 SPISOMIOx pinisaninput.
1 SPISOMIOx pinisanoutput.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIDIR0 SPISOMI0 direction. This bitcontrols thedirection oftheSPISOMI0 pinwhen itisused asageneral-
purpose I/Opin.IftheSPISOMI0 pinisused asaSPIfunctional pin,theI/Odirection isdetermined
bytheMASTER bitintheSPIGCR1 register.
0 SPISOMI0 pinisaninput.
1 SPISOMI0 pinisanoutput.
10 SIMODIR0 SPISIMO0 direction. This bitcontrols thedirection oftheSPISIMO0 pinwhen itisused asageneral-
purpose I/Opin.IftheSPISIMO0 pinisused asaSPIfunctional pin,theI/Odirection isdetermined
bytheMASTER bitintheSPIGCR1 register.
0 SPISIMO0 pinisaninput.
1 SPISIMO0 pinisanoutput.
9 CLKDIR SPICLK direction. This bitcontrols thedirection oftheSPICLK pinwhen itisused asageneral-
purpose I/Opin.Infunctional mode, theI/Odirection isdetermined bytheCLKMOD bit.
0 SPICLK pinisaninput.
1 SPICLK pinisanoutput.
8 ENADIR SPIENA direction. This bitcontrols thedirection oftheSPIENA pinwhen itisused asageneral-
purpose I/O.IftheSPIENA pinisused asafunctional pin,then theI/Odirection isdetermined bythe
CLKMOD bit(SPIGCR1[1]).
0 SPIENA pinisaninput.
1 SPIENA pinisanoutput.
7-0 SCSDIR SPICS direction. These bitscontrol thedirection ofeach SPICS pinwhen itisused asageneral-
purpose I/Opin.Each pincould beconfigured independently from theothers iftheSPICS isused as
aSPIfunctional pin.The I/Odirection isdetermined bytheCLKMOD bit(SPIGCR1[1]).
0 SPICS pinisaninput.
1 SPICS pinisanoutput.

<!-- Page 1547 -->

www.ti.com Control Registers
1547 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.8 SPIPinControl Register 2(SPIPC2)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
Figure 28-39. SPIPinControl Register 2(SPIPC2) [offset =1Ch]
31 24 23 16
SOMIDIN SIMODIN
R/W-U R/W-U
15 12 11 10 9 8
Reserved SOMIDIN0 SIMODIN0 CLKDIN ENADIN
R-0 R-U R-U R-U R-U
7 0
SCSDIN
R/W-U
LEGEND: R/W =Read/Write; R=Read only; U=value isundefined; -n=value after reset
Table 28-16. SPIPinControl Register 2(SPIPC2) Field Descriptions
Bit Field Value Description
31-24 SOMIDIN SPISOMIx data in.The value oftheSPISOMIx pins.
0 SPISOMIx pinislogic 0.
1 SPISOMIx pinislogic 1.
23-16 SIMODIN SPISIMOx data in.The value oftheSPISIMOx pins.
0 SPISIMOx pinislogic 0.
1 SPISIMOx pinislogic 1.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIDIN0 SPISOMI0 data in.The value oftheSPISOMI0 pin.
0 SPISOMI0 pinislogic 0.
1 SPISOMI0 pinislogic 1.
10 SIMODIN0 SPISIMO0 data in.The value oftheSPISIMO0 pin.
0 SPISIMO0 pinislogic 0.
1 SPISIMO0 pinislogic 1.
9 CLKDIN Clock data in.The value oftheSPICLK pin.
0 SPICLK pinislogic 0.
1 SPICLK pinislogic 1.
8 ENADIN SPIENA data in.The value oftheSPIENA pin.
0 SPIENA pinislogic 0.
1 SPIENA pinislogic 1.
7-0 SCSDIN SPICS data in.The value ofeach SPICS pin.
0 SPICS pinislogic 0.
1 SPICS pinislogic 1.

<!-- Page 1548 -->

Control Registers www.ti.com
1548 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.9 SPIPinControl Register 3(SPIPC3)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
Figure 28-40. SPIPinControl Register 3(SPIPC3) [offset =20h]
31 24 23 16
SOMIDOUT SIMODOUT
R/W-U R/W-U
15 12 11 10 9 8
Reserved SOMIDOUT0 SIMODOUT0 CLKDOUT ENADOUT
R-0 R/W-U R/W-U R/W-U R/W-U
7 0
SCSDOUT
R/W-U
LEGEND: R/W =Read/Write; R=Read only; U=value isundefined; -n=value after reset
Table 28-17. SPIPinControl Register 3(SPIPC3) Field Descriptions
Bit Field Value Description
31-24 SOMIDOUT SPISOMIx data outwrite. This bitisonly active when theSPISOMIx pinisconfigured asageneral-
purpose I/Opinandconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothe
pin.
Bit11orbit24canbeused tosetthedirection forpinSPISOMI0. Ifa32-bit write is
performed, bit11willhave priority over bit24.
0 Current value onSPISOMIx pinislogic 0.
1 Current value onSPISOMIx pinislogic 1
23-16 SIMODOUT SPISIMOx data outwrite. This bitisonly active when theSPISIMOx pinisconfigured asageneral-
purpose I/Opinandconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothe
pin.
Bit10orbit16canbeused tosetthedirection forpinSPISOMI0. Ifa32-bit write is
performed, bit10willhave priority over bit16.
0 Current value onSPISIMOx pinislogic 0.
1 Current value onSPISIMOx pinislogic 1.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIDOUT0 SPISOMI0 data outwrite. This bitisonly active when theSPISOMI0 pinisconfigured asageneral-
purpose I/Opinandconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothe
pin.
0 Current value onSPISOMI0 pinislogic 0.
1 Current value onSPISOMI0 pinislogic 1.
10 SIMODOUT0 SPISIMO0 data outwrite. This bitisonly active when theSPISIMO0 pinisconfigured asageneral-
purpose I/Opinandconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothe
pin.
0 Current value onSPISIMO0 pinislogic 0.
1 Current value onSPISIMO0 pinislogic 1.
9 CLKDOUT SPICLK data outwrite. This bitisonly active when theSPICLK pinisconfigured asageneral-
purpose I/Opinandconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothe
pin.
0 Current value onSPICLK pinislogic 0.
1 Current value onSPICLK pinislogic 1.

<!-- Page 1549 -->

www.ti.com Control Registers
1549 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-17. SPIPinControl Register 3(SPIPC3) Field Descriptions (continued)
Bit Field Value Description
8 ENADOUT SPIENA data outwrite. Only active when theSPIENA pinisconfigured asageneral-purpose I/Opin
andconfigured asanoutput pin.The value ofthisbitindicates thevalue sent tothepin.
0 Current value onSPIENA pinislogic 0.
1 Current value onSPIENA pinislogic 1.
7-0 SCSDOUT SPICS data outwrite. Only active when theSPICS pins areconfigured asageneral-purpose I/O
pins andconfigured asoutput pins. The value ofthese bitsindicates thevalue sent tothepins.
0 Current value onSPICS pinislogic 0.
1 Current value onSPICS pinislogic 1.
28.3.10 SPIPinControl Register 4(SPIPC4)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
Figure 28-41. SPIPinControl Register 4(SPIPC4) [offset =24h]
31 24 23 16
SOMISET SIMOSET
R/W-U R/W-U
15 12 11 10 9 8
Reserved SOMISET0 SIMOSET0 CLKSET ENASET
R-0 R/W-U R/W-U R/W-U R/W-U
7 0
SCSSET
R/W-U
LEGEND: R/W =Read/Write; R=Read only; U=value isundefined; -n=value after reset
Table 28-18. SPIPinControl Register 4(SPIPC4) Field Descriptions
Bit Field Value Description
31-24 SOMISET SPISOMIx data outset.This pinisonly active when theSPISOMIx pinisconfigured asageneral-
purpose output pin.
Bit11orbit24canbeused tosettheSOMI0 pin.Ifa32-bit write isperformed, bit11will
have priority over bit24.
0 Read: SPISIMOx islogic 0.
Write: Noeffect.
1 Read: SPISOMIx islogic 1.
Write: Logic 1isplaced onSPISOMIx pin,ifitisingeneral-purpose output mode.
23-16 SIMOSET SPISIMOx data outset.This bitisonly active when theSPISIMOx pinisconfigured asageneral-
purpose output pin.
Bit10orbit16canbeused tosettheSOMI0 pin.Ifa32-bit write isperformed, bit10will
have priority over bit16.
0 Read: SPISIMIx islogic 0.
Write: Noeffect.
1 Read: SPISIMIx islogic 1.
Write: Logic 1isplaced onSPISIMIx pin,ifitisingeneral-purpose output mode.

<!-- Page 1550 -->

Control Registers www.ti.com
1550 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-18. SPIPinControl Register 4(SPIPC4) Field Descriptions (continued)
Bit Field Value Description
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMISET0 SPISOMI0 data outset.This pinisonly active when theSPISOMI0 pinisconfigured asageneral-
purpose output pin.
0 Read: SPISOMI0 islogic 0.
Write: Noeffect.
1 Read: SPISOMI0 islogic 1.
Write: Logic 1isplaced onSPISOMI0 pin,ifitisingeneral-purpose output mode.
10 SIMOSET0 SPISIMO0 data outset.This pinisonly active when theSPISIMO0 pinisconfigured asageneral-
purpose output pin.
0 Read: SPISIMO0 islogic 0.
Write: Noeffect.
1 Read: SPISIMO0 islogic 1.
Write: Logic 1isplaced onSPISIMO0 pin,ifitisingeneral-purpose output mode.
9 CLKSET SPICLK data outset.This bitisonly active when theSPICLK pinisconfigured asageneral-purpose
output pin.
0 Read: SPICLK islogic 0.
Write: Noeffect.
1 Read: SPICLK islogic 1.
Write: Logic 1isplaced onSPICLK pin,ifitisingeneral-purpose output mode.
8 ENASET SPIENA data outset.This bitisonly active when theSPIENA pinisconfigured asageneral-
purpose output pin.
0 Read: SPIENA islogic 0.
Write: Noeffect.
1 Read: SPIENA islogic 1.
Write: Logic 1isplaced onSPIENA pin,ifitisingeneral-purpose O/P mode.
7-0 SCSSET SPICS data outset.This bitisonly active when theSPICS pinisconfigured asageneral-purpose
output pin.Avalue of1written tothisbitsets thecorresponding SCSDOUT bitto1.
0 Read: SPICS islogic 0.
Write: Noeffect.
1 Read: SPICS islogic 1.
Write: Logic 1isplaced onSPICS pin,ifitisingeneral-purpose output mode.

<!-- Page 1551 -->

www.ti.com Control Registers
1551 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.11 SPIPinControl Register 5(SPIPC5)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
Figure 28-42. SPIPinControl Register 5(SPIPC5) [offset =28h]
31 24 23 16
SOMICLR SIMOCLR
R/W-U R/W-U
15 12 11 10 9 8
Reserved SOMICLR0 SIMOCLR0 CLKCLR ENACLR
R-0 R/W-U R/W-U R/W-U R/W-U
7 0
SCSCLR
R/W-U
LEGEND: R/W =Read/Write; R=Read only; U=value isundefined; -n=value after reset
Table 28-19. SPIPinControl Register 5(SPIPC5) Field Descriptions
Bit Field Value Description
31-24 SOMICLR SPISOMIx data outclear. This pinisonly active when theSPISOMIx pinisconfigured asageneral-
purpose output pin.
Bit11orbit24canbeused tosettheSOMI0 pin.Ifa32-bit write isperformed, bit11willhave
priority over bit24.
0 Read: The current value onSOMIDOUTx is0.
Write: Noeffect.
1 Read: The current value onSOMIDOUTx is1.
Write: Logic 0isplaced onSPISOMIx pin,ifitisingeneral-purpose output mode.
23-16 SIMOCLR SPISIMOx data outclear. This bitisonly active when theSPISIMOx pinisconfigured asageneral-
purpose output pin.
Bit10orbit16canbeused tosettheSOMI0 pin.Ifa32-bit write isperformed, bit10willhave
priority over bit16.
0 Read: The current value onSOMODOUTx is0.
Write: Noeffect.
1 Read: The current value onSOMODOUTx is1.
Write: Logic 0isplaced onSPISIMIx pin,ifitisingeneral-purpose output mode.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMICLR0 SPISOMI0 data outcleart. This pinisonly active when theSPISOMI0 pinisconfigured asageneral-
purpose output pin.
0 Read: The current value onSPISOMI0 is0.
Write: Noeffect.
1 Read: The current value onSPISOMI0 is1.
Write: Logic 0isplaced onSPISOMI0 pin,ifitisingeneral-purpose output mode.
10 SIMOCLR0 SPISIMO0 data outclear. This pinisonly active when theSPISIMO0 pinisconfigured asageneral-
purpose output pin.
0 Read: The current value onSPISIMO0 is0.
Write: Noeffect.
1 Read: The current value onSPISIMO0 is1.
Write: Logic 0isplaced onSPISIMO0 pin,ifitisingeneral-purpose output mode.

<!-- Page 1552 -->

Control Registers www.ti.com
1552 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-19. SPIPinControl Register 5(SPIPC5) Field Descriptions (continued)
Bit Field Value Description
9 CLKCLR SPICLK data outclear. This bitisonly active when theSPICLK pinisconfigured asageneral-
purpose output pin.
0 Read: The current value onSPICLK is0.
Write: Noeffect.
1 Read: The current value onSPICLK is1.
Write: Logic 0isplaced onSPICLK pin,ifitisingeneral-purpose output mode.
8 ENACLR SPIENA data outclear. This bitisonly active when theSPIENA pinisconfigured asageneral-
purpose output pin.Avalue of1written tothisbitclears thecorresponding ENABLEDOUT bitto0.
0 Read: The current value onSPIENA is0.
Write: Noeffect.
1 Read: The current value onSPIENA is1.
Write: Logic 0isplaced onSPIENA pin,ifitisingeneral-purpose output mode.
7-0 SCSCLR SPICS data outclear. This bitisonly active when theSPICS pinisconfigured asageneral-purpose
output pin.
0 Read: The current value onSCSDOUT is0.
Write: Noeffect.
1 Read: The current value onSCSDOUT is1.
Write: Logic 0isplaced onSPICS pin,ifitisingeneral-purpose output mode.
28.3.12 SPIPinControl Register 6(SPIPC6)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofSPIPC0 toSPIPC9 reflect thenumber ofSIMO/SOMI data
lines perdevice. Ondevices with 8data-line support, allofbits31to16areimplemented.
Ondevices with less than 8data lines, only asubset ofthese bitsareavailable.
Unimplemented bitsreturn 0upon read andarenotwritable.
Figure 28-43. SPIPinControl Register 6(SPIPC6) [offset =2Ch]
31 24 23 16
SOMIPDR SIMOPDR
R/W-0 R/W-0
15 12 11 10 9 8
Reserved SOMIPDR0 SIMOPDR0 CLKPDR ENAPDR
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 0
SCSPDR
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset

<!-- Page 1553 -->

www.ti.com Control Registers
1553 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-20. SPIPinControl Register 6(SPIPC6) Field Descriptions
Bit Field Value Description
31-24 SOMIPDR SPISOMIx open drain enable. This bitenables open drain capability fortheSPISOMIx pinifthe
following conditions aremet:
*SOMIDIRx =1(SPISOMIx pinconfigured inGIO mode asanoutput)
*SOMIDOUTx =1
Bit11orbit24canboth beused toenable open-drain forSOMI0. Ifa32-bit write isperformed,
bit11willhave priority over bit24.
0 The output value ontheSPISOMIx pinislogic 1.
1 Output pinSPISOMIx isinahigh-impedance state.
23-16 SIMOPDR SPISIMOx open drain enable. This bitenables open drain capability fortheSPISIMOx pinifthe
following conditions aremet:
*SIMODIRx =1(SPISIMOx pinconfigured inGIO mode asanoutput)
*SIMODOUTx =1
Bit10orbit16canboth beused toenable open-drain forSIMO0. Ifa32-bit write isperformed,
bit10willhave priority over bit16.
0 The output value onSPISIMOx pinislogic 1.
1 Output pinSPISIMOx isinahigh-impedance state.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIPDR0 SOMI0 open-drain enable. This bitenables open-drain capability forSOMI0 ifthefollowing conditions
aremet.
*SOMI0 pinconfigured inGIO mode asoutput pin
*Output value onSPISOMI0 pinislogic 1.
0 Output value 1ofSPISOMI0 pinislogic 1.
1 Output value 1ofSPISOMI0 ishigh-impedance.
10 SIMOPDR0 SPISIMO0 open-drain enable. This bitenables open -drain capability fortheSPISIMO0 pinifthe
following conditions aremet.
*SIMO0 pinconfigured inGIO mode asoutput pin
*Output value onSPISIMO0 pinislogic 1.
0 Output value 1ofSPISIMO0 pinislogic 1.
1 Output value 1ofSPISIMO0 ishigh-impedance.
9 CLKPDR CLK open drain enable. This bitenables open drain capability forthepinCLK ifthefollowing
conditions aremet:
*SPICLK pinconfigured inGIO mode asanoutput pin
*SPICLKDOUT =1
0 Output value onCLK pinislogic 1.
1 Output pinCLK isinahigh-impedance state.
8 ENAPDR SPIENA open drain enable. This bitenables open drain capability fortheSPIENA pin,ifthefollowing
conditions aremet:
*SPIENA pinconfigured inGIO mode asanoutput pin
*SPIENADOUT =1
0 Output value ontheSPIENA pinislogic 1.
1 Output pinSPIENA isinahigh-impedance state.
7-0 SCSPDR SPICS open drain enable. This bitenables open drain capability fortheSPICS pin,ifthefollowing
conditions aremet:
*SPICS pinconfigured inGIO mode asanoutput pin
*SCSDOUT =1
0 Output value ontheSPICS pinislogic 1.
1 Output pinSPICS isinahigh-impedance state.

<!-- Page 1554 -->

Control Registers www.ti.com
1554 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.13 SPIPinControl Register 7(SPIPC7)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
NOTE: Default Register Value
The default values ofthese register bitsvary bydevice. See your device datasheet for
information about default pinstates, which correspond totheregister reset values (see the
pin-list table).
Figure 28-44. SPIPinControl Register 7(SPIPC7) [offset =30h]
31 24 23 16
SOMIDIS SIMODIS
R/W-x R/W-x
15 12 11 10 9 8
Reserved SOMIPDIS0 SIMOPDIS0 CLKPDIS ENAPDIS
R-0 R/W-x R/W-x R/W-x R/W-x
7 0
SCSPDIS
R/W-x
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; -x=value varies bydevice
Table 28-21. SPIPinControl Register 7(SPIPC7) Field Descriptions
Bit Field Value Description
31-24 SOMIDIS SOMIx pullcontrol disable. This bitdisables pullcontrol capability foreach SOMIx pinifitisininput
mode, regardless ofwhether itisinfunctional orGIO mode.
Note: Bit11orbit24canbeused tosetpull-disable forSOMIO. Ifa32-bit write isperformed,
bit11willhave priority over bit24.
0 Pullcontrol ontheSPISOMIx pinisenabled.
1 Pullcontrol ontheSPISOMIx pinisdisabled.
23-16 SIMODIS SIMOx pullcontrol disable. This bitdisables pullcontrol capability foreach SIMOx pinifitisininput
mode, regardless ofwhether itisinfunctional orGIO mode.
Note: Bit10orbit16canbeused tosetpull-disable forSIMO0. Ifa32-bit write isperformed,
bit10willhave priority over bit16.
0 Pullcontrol onSPISIMOx pinisenabled.
1 Pullcontrol onSPISIMOx pinisdisabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIPDIS0 SPISOMI0 pullcontrol disable. This bitdisables pullcontrol capability fortheSPISOMI0 pinifitisin
input mode, regardless ofwhether itisinfunctional orGIO mode.
0 Pullcontrol ontheSPISOMI0 pinisenabled.
1 Pullcontrol ontheSPISOMI0 pinisdisabled.
10 SIMOPDIS0 SPISIMO0 pullcontrol disable. This bitdisables pullcontrol capability fortheSPISIMO0 pinifitisin
input mode, regardless ofwhether itisinfunctional orGIO mode.
0 Pullcontrol ontheSPISIMO0 pinisenabled.
1 Pullcontrol ontheSPISIMO0 pinisdisabled.

<!-- Page 1555 -->

www.ti.com Control Registers
1555 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-21. SPIPinControl Register 7(SPIPC7) Field Descriptions (continued)
Bit Field Value Description
9 CLKPDIS CLK pullcontrol disable. This bitdisables pullcontrol capability fortheSPICLK pinifitisininput
mode, regardless ofwhether itisinfunctional orGIO mode.
0 Pullcontrol ontheCLK pinisenabled.
1 Pullcontrol ontheCLK pinisdisabled.
8 ENAPDIS SPIENA pullcontrol disable. This bitdisables pullcontrol capability fortheSPIENA pinifitisininput
mode, regardless ofwhether itisinfunctional orGIO mode.
0 Pullcontrol ontheSPIENA pinisenabled.
1 Pullcontrol ontheSPIENA pinisdisabled.
7-0 SCSPDIS SPICS pullcontrol disable. This bitdisables pullcontrol capability fortheSPICS pinifitisininput
mode, regardless ofwhether itisinfunctional orGIO mode.
0 Pullcontrol ontheSPICS pinisenabled.
1 Pullcontrol ontheSPICS pinisdisabled.
28.3.14 SPIPinControl Register 8(SPIPC8)
NOTE: Register bitsvary bydevice
Register bits31:24 and23:16 ofthisregister reflect thenumber ofSIMO/SOMI data lines per
device. Ondevices with 8data-line support, allofbits31to16areimplemented. Ondevices
with less than 8data lines, only asubset ofthese bitsareavailable. Unimplemented bits
return 0upon read andarenotwritable.
NOTE: Default Register Value
The default values ofthese register bitsvary bydevice. See your device datasheet for
information about default pinstates, which correspond totheregister reset values (see the
pin-list table).
Figure 28-45. SPIPinControl Register 8(SPIPC8) [offset =34h]
31 24 23 16
SOMIPSEL SIMOPSEL
R/W-x R/W-x
15 12 11 10 9 8
Reserved SOMIPSEL0 SIMOPSEL0 CLKPSEL ENAPSEL
R-0 R/W-x R/W-x R/W-x R/W-x
7 0
SCSPSEL
R/W-x
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; -x=value varies bydevice
Table 28-22. SPIPinControl Register 8(SPIPC8) Field Descriptions
Bit Field Value Description
31-24 SOMIPSEL SPISOMIx pullselect. This bitselects thetype ofpulllogic attheSOMIx pin.
Note: Bit11orbit24canbeused tosetpull-select forSPISOMI0. Ifa32-bit write is
performed, bit11willhave priority over bit24.
0 Pulldown ontheSOMIx pin.
1 PullupontheSOMIx pin.

<!-- Page 1556 -->

Control Registers www.ti.com
1556 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-22. SPIPinControl Register 8(SPIPC8) Field Descriptions (continued)
Bit Field Value Description
23-16 SIMOPSEL SPISIMOx pullselect. This bitselects thetype ofpulllogic attheSPISIMOx pin.
Note: Bit10orbit16canbeused tosetpull-select forSPISOMI0. Ifa32-bit write is
performed, bit10willhave priority over bit16.
0 Pulldown ontheSPISIMOx pin.
1 PullupontheSPISIMOx pin.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMIPSEL0 SOMI pullselect. This bitselects thetype ofpulllogic attheSOMI pin.
0 Pulldown ontheSPISOMI pin.
1 PullupontheSPISOMI pin.
10 SIMOPSEL0 SPISIMO pullselect. This bitselects thetype ofpulllogic attheSPISIMO pin.
0 Pulldown ontheSPISIMO pin.
1 PullupontheSPISIMO pin.
9 CLKPSEL SPICLK pullselect. This bitselects thetype ofpulllogic attheSPICLK pin.
0 Pulldown ontheSPICLK pin.
1 PullupontheSPICLK pin.
8 ENAPSEL SPIENA pullselect. This bitselects thetype ofpulllogic attheSPIENA pin.
0 Pulldown ontheSPIENA pin.
1 PullupontheSPIENA pin.
7-0 SCSPSEL SPICS pullselect. This bitselects thetype ofpulllogic attheSPICS pin.
0 Pulldown ontheSPICS pin.
1 PullupontheSPICS pin.
28.3.15 SPITransmit Data Register 0(SPIDAT0)
Figure 28-46. SPITransmit Data Register 0(SPIDAT0) [offset =38h]
31 16
Reserved
R-0
15 0
TXDATA
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-23. SPITransmit Data Register 0(SPIDAT0) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 TXDATA 0-FFFFh SPItransmit data. When written, these bitswillbecopied totheshift register ifitisempty. Ifthe
shift register isnotempty, TXBUF holds thewritten data. SPIEN (SPICGR1[24]) must besetto
1before thisregister canbewritten to.Writing a0totheSPIEN register forces thelower 16bits
oftheSPIDAT0 to0x00.
Note: When thisregister isread, thecontents TXBUF, which holds thelatest written data,
willbereturned.
Note: Regardless ofcharacter length, thetransmit word should beright-justified before
writing totheSPIDAT1 register.
Note: Thedefault data format control register forSPIDAT0 isSPIFMT0. However, itis
possible toreprogram theDFSEL[1:0] fields ofSPIDAT1 before using SPIDAT0, toselect
adifferent SPIFMTx register.
Note: Itishighly recommended touseSPIDAT1 register, SPIDAT0 issupported for
compatibility reasons.

<!-- Page 1557 -->

www.ti.com Control Registers
1557 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.16 SPITransmit Data Register 1(SPIDAT1)
NOTE: Writing toonly thecontrol fields, bits28through 16,does notinitiate anySPItransfer in
master mode. This feature canbeused tosetupSPICLK phase orpolarity before actually
starting thetransfer byonly updating theDFSEL bitfield toselect therequired phase and
polarity combination.
Figure 28-47. SPITransmit Data Register 1(SPIDAT1) [offset =3Ch]
31 29 28 27 26 25 24 23 16
Reserved CSHOLD Rsvd WDEL DFSEL CSNR
R-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
15 0
TXDATA
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-24. SPITransmit Data Register 1(SPIDAT1) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28 CSHOLD Chip select hold mode. The CSHOLD bitissupported inmaster mode only incompatibility-mode of
SPI, (itisignored inslave mode). CSHOLD defines thebehavior ofthechip select lineattheendofa
data transfer.
0 The chip select signal isdeactivated attheendofatransfer after theT2CDELAY time haspassed. If
twoconsecutive transfers arededicated tothesame chip select thischip select signal willbe
deactivated foratleast 2VCLK cycles before itisactivated again.
1 The chip select signal isheld active attheendofatransfer until acontrol field with new data and
control information isloaded intoSPIDAT1. Ifthenew chip select number equals theprevious one,
theactive chip select signal isextended until theendoftransfer with CSHOLD cleared, oruntil the
chip-select number changes.
27 Reserved 0 Reads return 0.Writes have noeffect.
26 WDEL Enable thedelay counter attheendofthecurrent transaction.
Note: TheWDEL bitissupported inmaster mode only. Inslave mode, thisbitwillbeignored.
0 Nodelay willbeinserted. However, theSPICS pins willstillbede-activated foratleast for2VCLK
cycles ifCSHOLD =0.
Note: Theduration forwhich theSPICS pinremains deactivated depends upon thetime taken
tosupply anew word after completing theshift operation. IfTXBUF isalready full,then the
SPICS pinwillbedeasserted foratleast twoVCLK cycles (ifWDEL =0).
1 After atransaction, WDELAY ofthecorresponding data format willbeloaded intothedelay counter.
Notransaction willbeperformed until theWDELAY counter overflows. The SPICS pins willbede-
activated foratleast (WDELAY +2)×VCLK_Period duration.
25-24 DFSEL Data word format select.
0 Data word format 0isselected.
1h Data word format 1isselected.
2h Data word format 2isselected.
3h Data word format 3isselected.
23-16 CSNR 0-FFh Chip select (CS) number. CSNR defines thechip select pins thatwillbeactivated during thedata
transfer. CSNR isabit-mask thatcontrols allchip select pins. See Table 28-25 .
Note: Ifyour MibSPI hasless than 8chip select pins, allunused upper bitswillbe0.For
example, MiBSPI3 has6chip select pins, ifyouwrite FFhtoCSNR, theactual number stored
inCSNR is3Fh.

<!-- Page 1558 -->

Control Registers www.ti.com
1558 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-24. SPITransmit Data Register 1(SPIDAT1) Field Descriptions (continued)
Bit Field Value Description
15-0 TXDATA 0-FFFFh Transfer data. When written, these bitsarecopied totheshift register ifitisempty. Iftheshift register
isnotempty, then they areheld inTXBUF.
SPIEN must besetto1before thisregister canbewritten to.Writing a0toSPIEN forces thelower
16bitsofSPIDAT1 to0x0000.
Awrite tothisregister (ortotheTXDATA field only) drives thecontents oftheCSNR field onthe
SPICS pins, ifthepins areconfigured asfunctional pins (automatic chip select, seeSection 28.2.1 ).
When thisregister isread, thecontents ofTXBUF, which holds thelatest data written, willbe
returned.
Note: Regardless ofthecharacter length, thetransmit data should beright-justified before
writing totheSPIDAT1 register.

<!-- Page 1559 -->

www.ti.com Control Registers
1559 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)(1)Ifyour MibSPI does nothave thischip select pin,thisbitis0.Table 28-25. Chip Select Number Active
CSNR
ValueChip Select Active: CSNR
ValueChip Select Active:
CS[5](1)CS[4](1)CS[3](1)CS[2](1)CS[1](1)CS[0] CS[5](1)CS[4](1)CS[3](1)CS[2](1)CS[1](1)CS[0]
0h Nochip select pinisactive. 20h x
1h x 21h x x
2h x 22h x x
3h x x 23h x x x
4h x 24h x x
5h x x 25h x x x
6h x x 26h x x x
7h x x x 27h x x x x
8h x 28h x x
9h x x 29h x x x
Ah x x 2Ah x x x
Bh x x x 2Bh x x x x
Ch x x 2Ch x x x
Dh x x x 2Dh x x x x
Eh x x x 2Eh x x x x
Fh x x x x 2Fh x x x x x
10h x 30h x x
11h x x 31h x x x
12h x x 32h x x x
13h x x x 33h x x x x
14h x x 34h x x x
15h x x x 35h x x x x
16h x x x 36h x x x x
17h x x x x 37h x x x x x
18h x x 38h x x x
19h x x x 39h x x x x
1Ah x x x 3Ah x x x x
1Bh x x x x 3Bh x x x x x
1Ch x x x 3Ch x x x x
1Dh x x x x 3Dh x x x x x
1Eh x x x x 3Eh x x x x x
1Fh x x x x x 3Fh x x x x x x

<!-- Page 1560 -->

Control Registers www.ti.com
1560 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.17 SPIReceive Buffer Register (SPIBUF)
Figure 28-48. SPIReceive Buffer Register (SPIBUF) [offset =40h]
31 30 29 28 27 26 25 24
RXEMPTY RXOVR TXFULL BITERR DESYNC PARITYERR TIMEOUT DLENERR
R-1 R-0 R-0 R-0 R-0 R-0 R-0 R-0
23 16
LCSNR
R-0
15 0
RXDATA
R-0
LEGEND: R=Read only; -n=value after reset
Table 28-26. SPIReceive Buffer Register (SPIBUF) Field Descriptions
Bit Field Value Description
31 RXEMPTY Receive data buffer empty. When thehost reads theRXDATA field ortheentire SPIBUF register,
itautomatically sets theRXEMPTY flag. When adata transfer iscompleted, thereceived data is
copied intoRXDATA andtheRXEMPTY flagiscleared.
0 New data hasbeen received andcopied intoRXDATA.
1 Nodata hasbeen received since thelastread ofRXDATA.
This flaggets setto1under thefollowing conditions:
*Reading theRXDATA field oftheSPIBUF register.
*Writing a1toclear theRXINTFLG bitintheSPIFlag Register (SPIFLG).
Write-clearing theRXINTFLG bitbefore reading theSPIBUF indicates thereceived data isbeing
ignored. Conversely, RXINTFLG canbecleared byreading theRXDATA field ofSPIBUF (orthe
entire register).
30 RXOVR Receive data buffer overrun. When adata transfer iscompleted andthereceived data iscopied
intoRXBUF while itisalready full,RXOVR isset.Overruns always occur toRXBUF, notto
SPIBUF; thecontents ofSPIBUF areoverwritten only after itisread bythePeripheral(VBUSP)
master (CPU, DMA, orother host processor).
Ifenabled, theRXOVRN interrupt isgenerated when RXBUF isoverwritten, andreading either SPI
Flag Register (SPIFLG) orSPIVECTx shows theRXOVRN condition. Two read operations from
theSPIBUF register arerequired toreach theoverwritten buffer word (one toread SPIBUF, which
then transfers RXDATA intoSPIBUF forthesecond read).
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
Note: Aspecial condition under which RXOVR flaggets set.Ifboth SPIBUF andRXBUF are
already fullandwhile another buffer receive isunderway, ifanyerrors such asTIMEOUT,
BITERR andDLEN_ERR occur, then RXOVR inRXBUF andSPIFlag Register (SPIFLG) will
besettoindicate thatthestatus flags aregetting overwritten bythenew transfer. This
overrun should betreated likeanormal receive overrun.
0 Noreceive data overrun condition occurred since lastread ofthedata field.
1 Areceive data overrun condition occurred since lastread ofthedata field.
29 TXFULL Transmit data buffer full.This flagisaread-only flag. Writing intotheSPIDAT0 orSPIDAT1 field
while theTXshift register isfullwillautomatically settheTXFULL flag. Once theword iscopied to
theshift register, theTXFULL flagwillbecleared. Writing toSPIDAT0 orSPIDAT1 when both
TXBUF andtheTXshift register areempty does notsettheTXFULL flag.
0 The transmit buffer isempty; SPIDAT0/SPIDAT1 isready toaccept anew data.
1 The transmit buffer isfull;SPIDAT0/SPIDAT1 isnotready toaccept new data.

<!-- Page 1561 -->

www.ti.com Control Registers
1561 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-26. SPIReceive Buffer Register (SPIBUF) Field Descriptions (continued)
Bit Field Value Description
28 BITERR Biterror. There was amismatch ofinternal transmit data andtransmitted data.
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
0 Nobiterror occurred.
1 Abiterror occurred. The SPIsamples thesignal ofthetransmit pins (master: SIMOx, slave:
SOMIx) atthereceive point (one-half clock cycle after thetransmit point). Ifthesampled value
differs from thetransmitted value, abiterror isdetected andtheBITERR flagisset.Possible
reasons forabiterror include noise, anexcessively high bitrate, capacitive load, oranother
master/slave trying totransmit atthesame time.
27 DESYNC Desynchronization ofslave device. This bitisvalid inmaster mode only.
The master monitors theENA signal coming from theslave device andsets theDESYNC flagif
ENA isdeactivated before thelastreception point orafter thelastbitistransmitted plus tT2EDELAY .
IfDESYNCENA isset,aninterrupt isasserted. Desynchronization canoccur ifaslave device
misses aclock edge coming from themaster.
Note: IntheCompatibility Mode MibSPI, under some circumstances itispossible fora
desync error detected fortheprevious buffer tobevisible inthecurrent buffer. This is
because thereceive completion flag/interrupt isgenerated when thebuffer transfer is
completed. Butdesynchronization isdetected after thebuffer transfer iscompleted. So,if
theVBUS master reads thereceived data quickly when anRXINT isdetected, then the
status flagmay notreflect thecorrect desync condition. Inmulti-buffer mode, thedesync
flagisalways guaranteed tobeforthecurrent buffer.
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
0 Noslave desynchronization isdetected.
1 Aslave device isdesynchronized.
26 PARITYERR Parity error. The calculated parity differs from thereceived parity bit.
Iftheparity generator isenabled (selected individually foreach buffer) aneven oroddparity bitis
added attheendofadata word. During reception ofthedata word, theparity generator calculates
thereference parity andcompares ittothereceived parity bit.Ifamismatch isdetected, the
PARITYERR flagisset.
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
0 Noparity error isdetected.
1 Aparity error occurred.
25 TIMEOUT Time-out because ofnon-activation ofSPIENA pin.
The SPIgenerates atime-out when theslave does notrespond intime byactivating theENA
signal after thechip select signal hasbeen activated. Ifatime-out condition isdetected, the
corresponding chip select isdeactivated immediately andtheTIMEOUT flagisset.Inaddition, the
TIMEOUT flaginthestatus field ofthecorresponding buffer andintheSPIFlag Register
(SPIFLG) isset.
This bitisvalid only inmaster mode.
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
0 NoSPIENA pintime-out occurred.
1 AnSPIENA signal time-out occurred.
24 DLENERR Data length error flag.
Note: This flagiscleared to0when theRXDATA field oftheSPIBUF register isread.
0 Nodata-length error occurred.
1 Adata length error occurred.
23-16 LCSNR 0-FFh Last chip select number. LCSNR inthestatus field isacopy ofCSNR inthecorresponding control
field. Itcontains thechip select number thatwas activated during thelastword transfer.
15-0 RXDATA 0-FFFFh SPIreceive data. This isthereceived word, transferred from thereceive shift-register attheendof
atransfer. Regardless oftheprogrammed character length andthedirection ofshifting, the
received data isstored right-justified intheregister.

<!-- Page 1562 -->

Control Registers www.ti.com
1562 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.18 SPIEmulation Register (SPIEMU)
Figure 28-49. SPIEmulation Register (SPIEMU) [offset =44h]
31 16
Reserved
R-8000h
15 0
EMU_RXDATA
R-0
LEGEND: R=Read only; -n=value after reset
Table 28-27. SPIEmulation Register (SPIEMU) Field Descriptions
Bit Field Value Description
31-16 Reserved 8000h Reserved
15-0 EMU_RXDATA 0-FFFFh SPIreceive data. The SPIemulation register isamirror oftheSPIBUF register. The only
difference between SPIEMU andSPIBUF isthataread from SPIEMU does notclear any
ofthestatus flags.
28.3.19 SPIDelay Register (SPIDELAY)
Figure 28-50. SPIDelay Register (SPIDELAY) [offset =48h]
31 24 23 16
C2TDELAY T2CDELAY
R/W-0 R/W-0
15 8 7 0
T2EDELAY C2EDELAY
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-28. SPIDelay Register (SPIDELAY) Field Descriptions
Bit Field Value Description
31-24 C2TDELAY 0-FFh Chip-select-active totransmit-start delay. See Figure 28-51 foranexample. C2TDELAY isused
only inmaster mode. Itdefines asetup time (fortheslave device) thatdelays thedata
transmission from thechip select active edge byamultiple ofVCLK cycles.
The setup time value iscalculated asfollows.
tC2TDELAY =(C2TDELAY +2)×VCLK Period
Example: VCLK =25MHz ->VCLK Period =40ns; C2TDELAY =07h;
>tC2TDELAY =360ns
When thechip select signal becomes active, theslave hastoprepare data transfer within 360ns.
Note: Ifphase =1,thedelay between SPICS falling edge tothefirst edge ofSPICLK will
have anadditional 0.5SPICLK period delay. This delay isaspertheSPIprotocol.

<!-- Page 1563 -->

www.ti.com Control Registers
1563 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-28. SPIDelay Register (SPIDELAY) Field Descriptions (continued)
Bit Field Value Description
23-16 T2CDELAY 0-FFh Transmit-end-to-chip-select-inactive-delay. See Figure 28-52 foranexample. T2CDELAY isused
only inmaster mode. Itdefines ahold time fortheslave device thatdelays thechip select
deactivation byamultiple ofVCLK cycles after thelastbitistransferred. The hold time value is
calculated asfollows:
tT2CDELAY =(T2CDELAY +1)×VCLK Period
Example: VCLK =25MHz ->VCLK Period =40ns; T2CDELAY =03h;
>tT2CDELAY =160ns
After thelastdata bit(orparity bit)isbeing transferred thechip select signal isheld active for160
ns.
Note: Ifphase =0,then between thelastedge ofSPICLK andrise-edge ofSPICS there will
beanadditional delay of0.5SPICLK period. This isaspertheSPIprotocol.
Both C2TDELAY andT2CDELAY counters donothave anydependency ontheSPIENA pin
value. Even iftheSPIENA pinisasserted bytheslave, themaster willcontinue todelay thestart
ofSPICLK until theC2TDELAY counter overflows.
Similarly, even iftheSPIENA pinisdeasserted bytheslave, themaster willcontinue tohold the
SPICS pins active until theT2CDELAY counter overflows. Inthisway, itisguaranteed thatthe
setup andhold times oftheSPICS pins aredetermined bythedelay timers alone. Toachieve
better throughput, itshould beensured thatthese twotimers arekept attheminimum possible
values.
15-8 T2EDELAY 0-FFh Transmit-data-finished toENA-pin-inactive time-out. T2EDELAY isused inmaster mode only. It
defines atime-out value asamultiple ofSPIclock before SPIENA signal hastobecome inactive
andafter SPICS becomes inactive. SPICLK depends onwhich data format isselected. Iftheslave
device ismissing oneormore clock edges, itbecomes de-synchronized. Inthiscase, although the
master hasfinished thedata transfer, theslave isstillwaiting forthemissed clock pulses andthe
ENA signal isnotdisabled.
The T2EDELAY defines atime-out value thattriggers theDESYNC flag, iftheSPIENA signal is
notdeactivated intime. The DESYNC flagissettoindicate thattheslave device didnotde-assert
itsSPIENA pinintime toacknowledge thatitreceived allbitsofthesent word. See Figure 28-53
foranexample ofthiscondition.
Note: DESYNC isalso setiftheSPIdetects ade-assertion ofSPIENA before theendofthe
transmission. Thetime-out value iscalculated asfollows:
tT2EDELAY =T2EDELAY/SPIclock
Example: SPIclock =8Mbit/s; T2EDELAY =10h;
>tT2EDELAY =2µs
The slave device hastodisable theENA signal within 2,otherwise DESYNC issetandan
interrupt isasserted (ifenabled).
7-0 C2EDELAY 0-FFh Chip-select-active toENA-signal-active time-out. C2EDELAY isused only inmaster mode andit
applies only iftheaddressed slave generates anENA signal asahardware handshake response.
C2EDELAY defines themaximum time between when theSPIactivates thechip-select signal and
theaddressed slave hastorespond byactivating theENA signal. C2EDELAY defines atime-out
value asamultiple ofSPIclocks. The SPIclock depends onwhether data format 0ordata format
1isselected. See Figure 28-54 foranexample ofthiscondition.
Note: Iftheslave device does notrespond with theENA signal before thetime-out value is
reached, theTIMEOUT flagintheSPIFLG register issetandainterrupt isasserted (if
enabled).
Ifatime-out occurs, theSPIclears thetransmit request ofthetimed-out buffer, sets theTIMEOUT
flagforthecurrent buffer, andcontinues with thetransfer ofthenext buffer inthesequence thatis
enabled.
The timeout value iscalculated asfollows: tC2EDELAY =C2EDELAY/SPIclock
Example: SPIclock =8Mbit/s; C2EDELAY =30h;
>tC2EDELAY =6ms
The slave device hastoactivate theENA signal within 6msafter theSPIhasactivated thechip
select signal (SPICS), otherwise theTIMEOUT flagissetandaninterrupt isasserted (ifenabled).

<!-- Page 1564 -->

SPICS
SPIENA
SPICLK
SPISOMI
tC2EDELAY
SPICS
SPIENA
SPICLK
SPISOMI
tT2EDELAY
SPICS
SPICLK
SPISOMI
VCLK
tT2CDELAY
SPICS
SPICLK
SPISOMI
VCLK
tC2TDELAY
Control Registers www.ti.com
1564 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-51. Example: tC2TDELAY =8VCLK Cycles
Figure 28-52. Example: tT2CDELAY =4VCLK Cycles
Figure 28-53. Transmit-Data-Finished-to-ENA-Inactive-Timeout
Figure 28-54. Chip-Select-Active-to-ENA-Signal-Active-Timeout

<!-- Page 1565 -->

www.ti.com Control Registers
1565 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.20 SPIDefault Chip Select Register (SPIDEF)
Figure 28-55. SPIDefault Chip Select Register (SPIDEF) [offset =4Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved CSDEF
R-0 R/W-FFh
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-29. SPIDefault Chip Select Register (SPIDEF) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 CDEF Chip select default pattern. Master-mode only.
The CSDEF bitsareoutput totheSPICS pins when notransmission isbeing performed. Itallows the
user tosetaprogrammable chip-select pattern thatdeselects alloftheSPIslaves.
0 SPICS iscleared to0when notransfer isactive.
1 SPICS issetto1when notransfer isactive.

<!-- Page 1566 -->

Control Registers www.ti.com
1566 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.21 SPIData Format Registers (SPIFMT[3:0])
Figure 28-56. SPIData Format Registers (SPIFMTn) [offset =5Ch-50h]
31 24
WDELAY
R/WP-0
23 22 21 20 19 18 17 16
PARPOL PARITYENA WAITENA SHIFTDIR HDUPLEX_
ENAxDIS_CS_
TIMERSPOLARITY PHASE
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 8 7 5 4 0
PRESCALE Reserved CHARLEN
R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-30. SPIData Format Registers (SPIFMTn) Field Descriptions
Bit Field Value Description
31-24 WDELAY 0-FFh Delay inbetween transmissions fordata format x(x=0,1,2,3).Idle time thatwillbeapplied
attheendofthecurrent transmission ifthebitWDEL issetinthecurrent buffer. The
delay tobeapplied isequal to:
WDELAY ×PVCLK+2×PVCLK
PVCLK->Period ofVCLK.
23 PARPOL Parity polarity: even orodd. PARPOLx canbemodified inprivilege mode only. Itcanbe
used fordata format x(x=0,1,2,3).
0 Aneven parity flagisadded attheendofthetransmit data stream.
1 Anoddparity flagisadded attheendofthetransmit data stream.
22 PARITYENA Parity enable fordata format x.
Noparity generation/ verification isperformed forthisdata format.
0 Aparity bitistransmitted attheendofeach transmitted word. Attheendofatransfer the
parity generator compares thereceived parity bitwith thelocally-calculated parity flag. If
theparity bitsdonotmatch theRXERR flagissetinthecorresponding control field. The
parity type (even orodd) canbeselected viathePARPOL bit.
1 Note: Ifanuncorrectable error flagissetinaslave-mode SPI, then thewrong parity
bitwillbetransmitted toindicate tothemaster thatthere hasbeen some issue with
thedata parity. TheSOMI pins willbeforced totransmit all0s,andtheparity bitwill
betransmitted as1ifeven parity isselected andas0ifoddparity isselected
(using thePARPOLx bitofthisregister). This behavior occurs regardless ofan
uncorrectable parity error oneither TXRAM orRXRAM.
21 WAITENA The master waits fortheENA signal from slave fordata format x.WAITENA isvalid in
master mode only. WAITENA enables aflexible SPInetwork where slaves with ENA
signal andslaves without ENA signal canbemixed. WAITENA defines, foreach
transferred word, whether theaddressed slave generates theENA signal ornot.
0 The SPIdoes notwait fortheENA signal from theslave anddirectly starts thetransfer.
1 Before theSPIstarts thedata transfer itwaits fortheENA signal tobecome low. Ifthe
ENA signal isnotpulled down bytheaddressed slave before theinternal time-out counter
(C2EDELAY) overflows, then themaster aborts thetransfer andsets theTIMEOUT error
flag.
20 SHIFTDIR Shift direction fordata format x.With bitSHIFTDIRx, theshift direction fordata format x
(x=0,1,2,3) canbeselected.
0 MSB isshifted outfirst.
1 LSB isshifted outfirst.

<!-- Page 1567 -->

www.ti.com Control Registers
1567 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-30. SPIData Format Registers (SPIFMTn) Field Descriptions (continued)
Bit Field Value Description
19 HDUPLEX_ENAx Half Duplex transfer mode enable forData Format x.This bitcontrols theI/Ofunction of
SOMI/SIMO lines foraspecific requirement where inthecase ofMaster mode, TXpin-
SIMO willactasanRXpin,andinthecase ofSlave mode, RXpin-SIMO willactasa
TXpin..
0 Normal FullDuplex transfer.
1 IfMASTER =1,SIMO pinwillactasanRXpin(NoTXpossible) IfMASTER =0,SIMO
pinwillactasaTXpin(NoRXpossible).
Forallnormal operations, HDUPLEX_ENAx bitsshould always remain 0.Itisintended for
theusage when theSIMO pinisused forboth TX&RXoperations atdifferent times.
18 DIS_CS_TIMERS Disable chip-select timers forthisformat. The C2TDELAY andT2CDELAY timers areby
default enabled forallthedata format registers. Using thisbit,these timers canbe
disabled foraparticular data format, ifthey arenotrequired. When amaster ishandling
multiple slaves, with varied set-up hold requirement, theapplication canselectively
choose toinclude ornotinclude thechip-select delay timers foranyslaves.
0 Both C2TDELAY andT2CDELAY counts areinserted forthechip selects.
1 NoC2TDELAY orT2CDELAY isinserted inthechip select timings.
17 POLARITY SPIdata format xclock polarity. POLARITYx defines theclock polarity ofdata format x.
The following restrictions apply when switching clock phase and/or polarity:
*In3-pin/4-pin with nENA pinconfiguration ofaslave SPI, theclock phase andpolarity
cannot bechanged on-the-fly between twotransfers. The slave should bereset and
reconfigured ifclock phase/polarity needs tobeswitched. Insummary, SPIformat
switching isnotfully supported inslave mode.
*Even while using chip select pins, thepolarity ofSPICLK canbeswitched only while
theslave isnotselected byavalid chip select. The master SPIshould ensure that
while switching SPICLK polarity, ithasdeselected allofitsslaves. Otherwise, the
switching ofSPICLK polarity may beincorrectly treated asaclock edge bysome
slaves.
0 IfPOLARITYx iscleared to0,theSPIclock signal islow-inactive, thatis,before andafter
data transfer theclock signal islow.
1 IfPOLARITYx issetto1,theSPIclock signal ishigh-inactive, thatis,before andafter
data transfer theclock signal ishigh.
16 PHASE SPIdata format xclock delay. PHASEx defines theclock delay ofdata format x.
0 IfPHASEx iscleared to0,theSPIclock signal isnotdelayed versus thetransmit/receive
data stream. The firstdata bitistransmitted with thefirstclock edge andthefirstbitis
received with thesecond (inverse) clock edge.
1 IfPHASEx issetto1,theSPIclock signal isdelayed byahalfSPIclock cycle versus the
transmit/receive data stream. The firsttransmit bithastooutput prior tothefirstclock
edge. The master andslave receive thefirstbitwith thefirstedge.
15-8 PRESCALE SPIdata format xprescaler. PRESCALEx determines thebittransfer rate ofdata format x
iftheSPIisthenetwork master. PRESCALEx isusetoderive SPICLK from VCLK. Ifthe
SPIisconfigured asslave, PRESCALEx does notneed tobeconfigured. The clock rate
fordata format xcanbecalculated as:
BRFormatx =VCLK /(PRESCALEx +1)
Note: When PRESCALEx iscleared to0,theSPIclock ratedefaults toVCLK/2.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 CHARLEN 0-1Fh SPIdata format xdata-word length. CHARLENx defines theword length ofdata format x.
Legal values are0x02 (data word length =2bit)to10h(data word length =16). Illegal
values, such as00or1Fh arenotallowed; their effect isindeterminate.

<!-- Page 1568 -->

Control Registers www.ti.com
1568 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.22 Interrupt Vector 0(INTVECT0)
NOTE: The TGinterrupt isnotavailable inMibSPI incompatibility mode. Therefore, there isno
possibility toaccess thisregister incompatibility mode.
Figure 28-57. Interrupt Vector 0(NTVECT0) [offset =60h]
31 16
Reserved
R-0
15 6 5 1 0
Reserved INTVECT0 SUSPEND0
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 28-31. Transfer Group Interrupt Vector 0(INTVECT0)
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-1 INTVECT0 INTVECT0. Interrupt vector forinterrupt lineINT0.
Returns thevector ofthepending interrupt atinterrupt lineINT0. Ifmore than oneinterrupt is
pending, INTVECT0 always references thehighest prior interrupt source first.
Note: This field reflects thestatus oftheSPIFLG register invector format. Any updates to
theSPIFLG register willautomatically cause updates tothisfield.
0 There isnopending interrupt.
1h÷x Transfer group x(x=0,..,15) hasapending interrupt. SUSPEND0 reflects thetype ofinterrupt
(suspended orfinished ).
11h Error Interrupt pending. The lower halfofSPIFLG contains more details about thetype oferror.
13h The pending interrupt isaReceive Buffer Overrun interrupt.
12h SPImode: The pending interrupt isaReceive Buffer Fullinterrupt.
Mibmode: Reserved. This bitcombination should notoccur.
14h SPImode: The pending interrupt isaTransmit Buffer Empty interrupt.
Mibmode: Reserved. This bitcombination should notoccur.
AllOther
CombinationsSPImode: Reserved. These bitcombinations should notoccur.
0 SUSPEND0 Transfer suspended /Transfer finished interrupt flag.
Every time INTVECT0 isread bythehost, thecorresponding interrupt flagofthereferenced
transfer group iscleared andINTVECT0 isupdated with thevector coming next inthepriority
chain.
0 The interrupt type isatransfer finished interrupt. Inother words, thebuffer array referenced by
INTVECT0 hasasserted aninterrupt because allofdata from thetransfer group hasbeen
transferred.
1 The interrupt type isatransfer suspended interrupt. Inother words, thetransfer group referenced
byINTVECT0 hasasserted aninterrupt because thebuffer tobetransferred next isinsuspend-to-
wait mode.
NOTE: Reading from theINTVECT0 register when Transmit Empty isindicated does notclear the
TXINTFLG flagintheSPIFlag Register (SPIFLG). Writing anew word totheSPIDATx
register clears theTransmit Empty interrupt.

<!-- Page 1569 -->

www.ti.com Control Registers
1569 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)NOTE: Inmulti-buffer mode, INTVECT0 contains theinterrupt forthehighest priority transfer group.
Aread from INTVECT0 automatically causes thenext-highest priority transfer group's
interrupt status togetloaded intoINTVECT0 anditscorresponding SUSPEND flagtoget
loaded intoSUSPEND0. The transfer group with thelowest number hasthehighest priority,
andthetransfer group with thehighest number hasthelowest priority.
Reading theINTVECT0 register when theRXOVRN interrupt isindicated inmulti-buffer
mode does notclear theRXOVRN flagandhence does notclear thevector. The RXOVRN
interrupt vector may becleared inmulti-buffer mode either bywrite-clearing theRXOVRN
flagintheSPIFlag Register (SPIFLG) orbyreading theRXRAM Overrun Buffer Address
Register (RXOVRN_BUF_ADDR).
28.3.23 Interrupt Vector 1(INTVECT1)
NOTE: The TGinterrupt isnotavailable inSPIincompatibility mode compatibility mode. Therefore,
there isnopossibility toaccess thisregister incompatibility mode.
Figure 28-58. Interrupt Vector 1(INTVECT1) [offset =64h]
31 16
Reserved
R-0
15 6 5 1 0
Reserved INTVECT1 SUSPEND1
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 28-32. Transfer Group Interrupt Vector 1(INTVECT1)
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-1 INTVECT1 INTVECT1. Interrupt vector forinterrupt lineINT1.
Returns thevector ofthepending interrupt atinterrupt lineINT1. Ifmore than oneinterrupt is
pending, INTVECT1 always references thehighest prior interrupt source first.
Note: This field reflects thestatus oftheSPIFLG register invector format. Any updates to
theSPIFLG register willautomatically cause updates tothisfield.
0 There isnopending interrupt. SPImode only.
11h Error Interrupt pending. The lower halfofSPIINT1 contains more details about thetype oferror.
SPImode only.
13h The pending interrupt isaReceive Buffer Overrun interrupt. SPImode only.
12h The pending interrupt isaReceive Buffer Fullinterrupt. SPImode only.
14h The pending interrupt isaTransmit Buffer Empty interrupt. SPImode only.
AllOther
CombinationsReserved. These bitcombinations should notoccur. SPImode only.
0 SUSPEND1 Transfer suspended /Transfer finished interrupt flag.
Every time INTVECT1 isread bythehost, thecorresponding interrupt flagofthereferenced
transfer group iscleared andINTVECT1 isupdated with thevector coming next inthepriority
chain.
0 The interrupt type isatransfer finished interrupt. Inother words, thebuffer array referenced by
INTVECT1 hasasserted aninterrupt because allofdata from thetransfer group hasbeen
transferred.
1 The interrupt type isatransfer suspended interrupt. Inother words, thetransfer group referenced
byINTVECT1 hasasserted aninterrupt because thebuffer tobetransferred next isinsuspend-to-
wait mode.

<!-- Page 1570 -->

Control Registers www.ti.com
1570 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)NOTE: Reading from theINTVECT1 register when Transmit Empty isindicated does notclear the
TXINTFLG flagintheSPIFlag Register (SPIFLG). Writing anew word totheSPIDATx
register clears theTransmit Empty interrupt.
NOTE: Inmulti-buffer mode, INTVECT1 contains theinterrupt forthehighest priority transfer group.
Aread from INTVECT1 automatically causes thenext-highest priority transfer group's
interrupt status togetloaded intoINTVECT1 anditscorresponding SUSPEND flagtoget
loaded intoSUSPEND1. The transfer group with thelowest number hasthehighest priority,
andthetransfer group with thehighest number hasthelowest priority.
Reading theINTVECT1 register when theRXOVRN interrupt isindicated inmulti-buffer
mode does notclear theRXOVRN flagandhence does notclear thevector. The RXOVRN
interrupt vector may becleared inmulti-buffer mode either bywrite-clearing theRXOVRN
flagintheSPIFlag Register (SPIFLG) orbyreading theRXRAM Overrun Buffer Address
Register (RXOVRN_BUF_ADDR).

<!-- Page 1571 -->

www.ti.com Control Registers
1571 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.24 SPIPinControl Register 9(SPIPC9)
SPIPC9 only applies toSPI2.
Figure 28-59. SPIPinControl Register 9(SPIPC9) [offset =68h]
31 25 24 23 17 16
Reserved SOMISRS0 Reserved SIMOSRS0
R-0 R/W-0 R-0 R/W-0
15 12 11 10 9 8 0
Reserved SOMISRS0 SIMOSRS0 CLKSRS Reserved
R-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-33. SPIPinControl Register 9(SPIPC9) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return thevalue thatwas lastwritten. Writes have noeffect.
24 SOMISRS0 SPI2 SOMI[0] slew control. This bitcontrols between thefastorslow slew mode.
Note: Duplicate Control Bits forSPI2 SOMI[0]. Bit24isnotphysically implemented. Itisa
mirror ofbit11.Any write tobit24willbereflected onbit11.When bit24andbit11are
simultaneously written, thevalue ofbit11willcontrol theSPI2 SOMI[0] pin.Theread value
ofbit24always reflects thevalue ofbit11.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.
23-17 Reserved 0 Reads return thevalue thatwas lastwritten. Writes have noeffect.
16 SIMOSRS0 SPI2 SPISIMO[0] slew control. This bitcontrols between thefastorslow slew mode.
Note: Duplicate Control Bits forSPI2 SIMO[0]. Bit16isnotphysically implemented. Itisa
mirror ofbit10.Any write tobit16willbereflected onbit10.When bit16andbit10are
simultaneously written, thevalue ofbit10willcontrol theSPI2 SOMI[0] pin.Theread value
ofbit16always reflects thevalue ofbit10.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SOMISRS0 SPI2 SOMI[0] slew control. This bitcontrols between thefastorslow slew mode.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.
10 SIMOSRS0 SPI2 SPISIMO[0] slew control. This bitcontrols between thefastorslow slew mode.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.
9 CLKSRS SPI2 CLK slew control. This bitcontrols between thefastorslow slew mode.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.
8-0 Reserved 0 Reads return thevalue thatwas lastwritten. Writes have noeffect.

<!-- Page 1572 -->

Control Registers www.ti.com
1572 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.25 Parallel/Modulo Mode Control Register (SPIPMCTRL)
NOTE: Donotconfigure MODCLKPOLx andMMODEx bitssince thisdevice does notsupport
modulo mode.
NOTE: The bitsofthisregister areused inconjunction with theSPIFMTx registers. Each byte ofthis
register corresponds tooneoftheSPIFMTx registers.
1.Byte0 (Bits 7:0) areused when SPIFMT0 register isselected byDFSEL[1:0] =00inthe
control field ofabuffer.
2.Byte1 (Bits 15:8) areused when SPIFMT1 register isselected byDFSEL[1:0] =01inthe
control field ofabuffer.
3.Byte2 (Bits 23:16) areused when SPIFMT2 register isselected byDFSEL[1:0] =10inthe
control field ofabuffer.
4.Byte3 (Bits31:24) areused when SPIFMT3 register isselected byDFSEL[1:0] =11inthe
control field ofabuffer.
Figure 28-60. Parallel/Modulo Mode Control Register (SPIPMCTRL) [offset =6Ch]
31 30 29 28 26 25 24
Reserved MODCLKPOL3 MMODE3 PMODE3
R-0 R/WP-0 R/WP-0 R/WP-0
23 22 21 20 18 17 16
Reserved MODCLKPOL2 MMODE2 PMODE2
R-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 12 10 9 8
Reserved MODCLKPOL1 MMODE1 PMODE1
R-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 2 1 0
Reserved MODCLKPOL0 MMODE0 PMODE0
R-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-34. SPIParallel/Modulo Mode Control Register (SPIPMCTRL) Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29 MODCLKPOL3 Modulo mode SPICLK polarity. This bitdetermines thepolarity oftheSPICLK inmodulo
mode only. IftheMMODE3 bitsare000, thisbitwillbeignored.
0 Normal SPICLK inallthemodes.
1 Polarity oftheSPICLK willbeinverted ifModulo mode isselected.
28-26 MMODE3 These bitsdetermine whether theSPI/MibSPI operates with 1,2,4,5,or6data lines (if
modulo option issupported bythemodule).
0 Normal single data linemode (default). (PMODE3 should besetto00).
1h 2-data linemode (PMODE3 should besetto00).
2h 3-data linemode (PMODE3 should besetto00).
3h 4-data linemode (PMODE3 should besetto00).
4h 5-data linemode (PMODE3 should besetto00).
5h 6-data linemode (PMODE3 should besetto01).
6h-7h Reserved

<!-- Page 1573 -->

www.ti.com Control Registers
1573 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-34. SPIParallel/Modulo Mode Control Register (SPIPMCTRL) Field Descriptions (continued)
Bit Field Value Description
25-24 PMODE3 Parallel mode bitsdetermine whether theSPI/MibSPI operates with 1,2,4,or8data lines.
0 Normal operation/1-data line(MMODE3 should besetto000).
1h 2-data linemode (MMODE3 should besetto000).
2h 4-data linemode (MMODE3 should besetto000).
3h 8-data linemode (MMODE3 should besetto000).
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21 MODCLKPOL2 Modulo mode SPICLK polarity. This bitdetermines thepolarity oftheSPICLK inmodulo
mode only. IftheMMODE2 bitsare000, thisbitwillbeignored.
0 Normal SPICLK inallthemodes.
1 Polarity oftheSPICLK willbeinverted ifModulo mode isselected.
20-18 MMODE2 These bitsdetermine whether theSPI/MibSPI operates with 1,2,4,5,or6data lines (if
modulo option issupported bythemodule).
0 1-data linemode (default). (PMODE2 should besetto00).
1h 2-data linemode (PMODE2 should besetto00).
2h 3-data linemode (PMODE2 should besetto00).
3h 4-data linemode (PMODE2 should besetto00).
4h 5-data linemode (PMODE2 should besetto00).
5h 6-data linemode (PMODE2 should besetto01).
6h-7h Reserved
17-16 PMODE2 Parallel mode bitsdetermine whether theSPI/MibSPI operates with 1,2,4,or8data lines.
0 Normal operation/1-data line(MMODE2 should besetto000).
1h 2-data linemode (MMODE2 should besetto000).
2h 4-data linemode (MMODE2 should besetto000).
3h 8-data linemode (MMODE2 should besetto000).
15-12 Reserved 0 Reads return 0.Writes have noeffect.
13 MODCLKPOL1 Modulo mode SPICLK polarity. This bitdetermines thepolarity oftheSPICLK inmodulo
mode only. IftheMMODE1 bitsare000, thisbitwillbeignored.
0 Normal SPICLK inallthemodes.
1 Polarity oftheSPICLK willbeinverted ifModulo mode isselected.
12-10 MMODE1 These bitsdetermine whether theSPI/MibSPI operates with 1,2,4,5,or6data lines (if
modulo option issupported bythemodule).
0 1-data linemode (default). (PMODE1 should besetto00).
1h 2-data linemode (PMODE1 should besetto00).
2h 3-data linemode (PMODE1 should besetto00).
3h 4-data linemode (PMODE1 should besetto00).
4h 5-data linemode (PMODE1 should besetto00).
5h 6-data linemode (PMODE1 should besetto01).
6h-7h Reserved
9-8 PMODE1 Parallel mode bitsdetermine whether theSPI/MibSPI operates with 1,2,4,or8data lines.
0 Normal operation/1-data line(MMODE1 should besetto000).
1h 2-data linemode (MMODE1 should besetto000).
2h 4-data linemode (MMODE1 should besetto000).
3h 8-data linemode (MMODE1 should besetto000).
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5 MODCLKPOL0 Modulo mode SPICLK polarity. This bitdetermines thepolarity oftheSPICLK inmodulo
mode only. IftheMMODE0 bitsare000, thisbitwillbeignored.
0 Normal SPICLK inallthemodes.
1 Polarity oftheSPICLK willbeinverted ifModulo mode isselected.

<!-- Page 1574 -->

Control Registers www.ti.com
1574 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-34. SPIParallel/Modulo Mode Control Register (SPIPMCTRL) Field Descriptions (continued)
Bit Field Value Description
4-2 MMODE0 These bitsdetermine whether theSPI/MibSPI operates with 1,2,4,5,or6data lines (if
modulo option issupported bythemodule).
0 1-data linemode (default). (PMODE0 should besetto00).
1h 2-data linemode (PMODE0 should besetto00).
2h 3-data linemode (PMODE0 should besetto00).
3h 4-data linemode (PMODE0 should besetto00).
4h 5-data linemode (PMODE0 should besetto00).
5h 6-data linemode (PMODE0 should besetto01).
6h-7h Reserved
1-0 PMODE0 Parallel mode bitsdetermine whether theSPI/MibSPI operates with 1,2,4,or8data lines.
0 Normal operation/1-data line(MMODE0 should besetto000).
1h 2-data linemode (MMODE0 should besetto000).
2h 4-data linemode (MMODE0 should besetto000).
3h 8-data linemode (MMODE0 should besetto000).

<!-- Page 1575 -->

www.ti.com Control Registers
1575 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)NOTE: Accessibility ofRegisters
Registers from thisoffset address onwards arenotaccessible inSPIcompatibility mode.
They areaccessible only inthemulti-buffer mode.
28.3.26 Multi-buffer Mode Enable Register (MIBSPIE)
NOTE: Accessibility ofMulti-Buffer RAM
The multi-buffer RAM isnotaccessible unless theMSPIENA bitissetto1.The only
exception tothisisintestmode, where, bysetting RXRAMACCESS to1,themulti-buffer
RAM canbefully accessed forboth read andwrite.
Figure 28-61. Multi-buffer Mode Enable Register (MIBSPIE) [offset =70h]
31 17 16
Reserved RXRAM_ACCESS
R-0 R/WP-0
15 12 11 10 9 8 7 1 0
Reserved EXTENDED_BUF_ENA Reserved MSPIENA
R-0 R/WP-5h R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-35. Multi-buffer Mode Enable Register (MIBSPIE) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 RXRAM_ACCESS Receive-RAM access control. During normal operating mode ofSPI, thereceive
data/status portion ofmulti-buffer RAM isread-only. Toenable testing ofreceive RAM,
direct read/write access isenabled bysetting thisbit.
0 The RXportion ofmulti-buffer RAM isnotwritable bytheCPU.
1 The whole ofmulti-buffer RAM isfully accessible forread/write bytheCPU.
Note: TheRXRAM ACCESS bitremains 0after reset anditshould remain setto0
atalltimes, except when testing theRAM. SPIshould begiven alocal reset by
using thenRESET (SPIGCR0[0]) bitafter RAM testing isperformed sothatthe
multi-buffer RAM gets re-initialized.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 EXTENDED_BUF_ENAEnables thesupport for256buffers. Bydefault MibSPI supports upto128buffers forboth
TXandRX.Refer tothedevice specific datasheet if256buffer extension isimplemented
forthespecific MibSPI instance inthedevice.
5h Write: Disables theExtended Buffer mode -MibSPI supports only 128buffers (default).
Ah Write: Enables theExtended Buffer mode -upto256buffers canbeused.
allothers Allother values -writes areignored andthevalues arenotupdated intothisfield. The
state ofthefeature remains unchanged.
Read: Returns thecurrent value ofthisfield.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 MSPIENA Multi-buffer mode enable. After power-up orreset, MSPIENA remains cleared, which
means thattheSPIruns incompatibility mode bydefault. Ifmulti-buffer mode isdesired,
thisregister should beconfigured firstafter configuring theSPIGCR0 register. IfMSPIENA
isnotsetto1,themulti-buffer mode registers arenotwritable.
0 The SPIruns incompatibility mode, thatis,inthismode theMibSPI isfully code-
compliant tothestandard device SPI. Nomulti-buffered-mode features aresupported.
1 The SPIisconfigured toruninmulti-buffer mode.

<!-- Page 1576 -->

Control Registers www.ti.com
1576 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.27 TGInterrupt Enable SetRegister (TGITENST)
The register TGITENST contains theTGinterrupt enable flags fortransfer-finished andfortransfer-
suspended events. Each oftheenable bitsinthehigher half-word andthelower half-word ofTGITENST
belongs tooneTG.
The register map shown inFigure 28-62 andTable 28-36 represents asuper-set device with the
maximum number ofTGs (16) assumed. The actual number ofbitsavailable varies perdevice.
Figure 28-62. TGInterrupt Enable SetRegister (TGITENST) [offset =74h]
31 16
SETINTENRDY[15:0]
R/W-0
15 0
SETINTENSUS[15:0]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-36. TGInterrupt Enable SetRegister (TGITENST) Field Descriptions
Bit Field Value Description
31-16 SETINTENRDY[ n] TGinterrupt set(enable) when transfer finished. Bit16corresponds toTG0, bit17corresponds
toTG1, andsoon.
0 Read: The TGx-completed interrupt isdisabled. This interrupt does notgetgenerated when
TGx completes.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt isenabled. The interrupt gets generated when TGx
completes.
Write: Enable theTGx-completed interrupt. The interrupt gets generated when TGx completes.
15-0 SETINTENSUS[ n] TGinterrupt set(enabled) when transfer suspended. Bit0corresponds toTG0, bit1
corresponds toTG1, andsoon.
0 Read: The TGx-completed interrupt isdisabled. This interrupt does notgetgenerated when
TGx issuspended.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt isenabled. The interrupt gets generated when TGx is
suspended.
Write: Enable theTGx-completed interrupt. The interrupt gets generated when TGx is
suspended.

<!-- Page 1577 -->

www.ti.com Control Registers
1577 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.28 TGInterrupt Enable Clear Register (TGITENCR)
The register TGITENCR isused toclear theinterrupt enables fortheTG-completed interrupt andtheTG-
suspended interrupts.
The register map shown inFigure 28-63 andTable 28-37 represents asuper-set device with the
maximum number ofTGs (16) assumed. The actual number ofbitsavailable varies perdevice.
Figure 28-63. TGInterrupt Enable Clear Register (TGITENCR) [offset =78h]
31 16
CLRINTENRDY[15:0]
R/W-0
15 0
CLRINTENSUS[15:0]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-37. TGInterrupt Enable Clear Register (TGITENCR) Field Descriptions
Bit Field Value Description
31-16 CLRINTENRDY[ n] TGinterrupt clear (disabled) when transfer finished. Bit16corresponds toTG0, bit17
corresponds toTG1, andsoon.
0 Read: The TGx-completed interrupt isdisabled. This interrupt does notgetgenerated when
TGx completes.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt isenabled. The interrupt gets generated when TGx
completes.
Write: Disable theTGx-completed interrupt. The interrupt does notgetgenerated when TGx
completes.
15-0 CLRINTENSUS[ n] TGinterrupt clear (disabled) when transfer suspended. Bit0corresponds toTG0, bit1
corresponds toTG1, andsoon.
0 Read: The TGx-completed interrupt isdisabled. This interrupt does notgetgenerated when
TGx issuspended.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt isenabled. The interrupt gets generated when TGx is
suspended.
Write: Disable theTGx-completed interrupt. The interrupt does notgetgenerated when TGx is
suspended.

<!-- Page 1578 -->

Control Registers www.ti.com
1578 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.29 Transfer Group Interrupt Level SetRegister (TGITLVST)
The register TGITLVST sets thelevel ofinterrupts fortransfer completed interrupt andfortransfer
suspended interrupt tolevel 1.
The register map shown inFigure 28-64 andTable 28-38 represents asuper-set device with themaximum
number ofTGs (16) assumed. The actual number ofbitsavailable varies perdevice.
Figure 28-64. Transfer Group Interrupt Level SetRegister (TGITLVST) [offset =7Ch]
31 16
SETINTLVLRDY[15:0]
R/W-0
15 0
SETINTLVLSUS[15:0]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-38. Transfer Group Interrupt Level SetRegister (TGITLVST) Field Descriptions
Bit Field Value Description
31-16 SETINTLVLRDY[ n] Transfer-group completed interrupt level set.Bit16corresponds toTG0, bit17corresponds to
TG1, andsoon.
0 Read: The TGx-completed interrupt issettoINT0.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt issettoINT1.
Write: SettheTGx-completed interrupt toINT1.
15-0 SETINTLVLSUS[ n] Transfer-group suspended interrupt level set.Bit0corresponds toTG0, bit1corresponds to
TG1, andsoon.
0 Read: The TGx-suspended interrupt issettoINT0.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-suspended interrupt issettoINT1.
Write: SettheTG-x suspended interrupt toINT1.

<!-- Page 1579 -->

www.ti.com Control Registers
1579 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.30 Transfer Group Interrupt Level Clear Register (TGITLVCR)
The register TGITLVCR clears thelevel ofinterrupts fortransfer completed interrupt andfortransfer
suspended interrupt tolevel 0.
The register map shown inFigure 28-65 andTable 28-39 represents asuper-set device with the
maximum number ofTGs (16) assumed. The actual number ofbitsavailable varies perdevice.
Figure 28-65. Transfer Group Interrupt Level Clear Register (TGITLVCR) [offset =80h]
31 16
CLRINTLVLRDY[15:0]
R/W-0
15 0
CLRINTLVLSUS[15:0]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-39. Transfer Group Interrupt Level Clear Register (TGITLVCR) Field Descriptions
Bit Field Value Description
31-16 CLRINTLVLRDY[ n] Transfer-group completed interrupt level clear. Bit16corresponds toTG0, bit17corresponds to
TG1, andsoon.
0 Read: The TGx-completed interrupt issettoINT0.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-completed interrupt issettoINT1.
Write: Clear theTGx-completed interrupt toINT0.
15-0 CLRINTLVLSUS[ n] Transfer group suspended interrupt level clear. Bit0corresponds toTG0, bit1corresponds to
TG1, andsoon.
0 Read: TGx-suspended interrupt issettoINT0.
Write: Awrite of0tothisbithasnoeffect.
1 Read: The TGx-suspended interrupt issettoINT1.
Write: Clear theTG-x suspended interrupt toINT0.

<!-- Page 1580 -->

Control Registers www.ti.com
1580 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.31 Transfer Group Interrupt Flag Register (TGINTFLAG)
The TGINTFLAG register comprises thetransfer group interrupt flags fortransfer-completed interrupts
(INTFLGRDYx) andfortransfer-suspended interrupts (INTFLGSUSx). Each oftheinterrupt flags inthe
higher half-word andthelower half-word ofTGINTFLAG belongs tooneTG.
The register map shown inFigure 28-66 andTable 28-40 represents asuper-set device with the
maximum number ofTGs (16) assumed. The actual number ofbitsavailable varies perdevice.
Figure 28-66. Transfer Group Interrupt Flag Register (TGINTFLAG) [offset =84h]
31 16
INTFLGRDY[15:0]
R/W1C-0
15 0
INTFLGSUS[15:0]
R/W1C-0
LEGEND: R/W =Read/Write; W1C =Write 1toclear; -n=value after reset
Table 28-40. Transfer Group Interrupt Level Clear Register (TGITLVCR) Field Descriptions
Bit Field Value Description
31-16 INTFLGRDY[
n]Transfer-group interrupt flagforatransfer-completed interrupt. Bit16corresponds toTG0, bit17
corresponds toTG1, andsoon.
Note: Read Clear Behavior. Reading theinterrupt vector registers TGINTVECT0 orTGINTVECT1
automatically clears theinterrupt flagbitINTFLGRDYx referenced bythevector number given by
INTVECT0/INTVECT1 bits, iftheSUSPEND[0:1] bitinthevector registers is0.
0 Read: Notransfer-completed interrupt occurred since lastclearing oftheINTFLGRDYx flag.
Write: Awrite of0tothisbithasnoeffect.
1 Read: Atransfer finished interrupt from transfer group xoccurred. Nomatter whether theinterrupt is
enabled ordisabled (INTENRDYx =don'tcare) orwhether theinterrupt ismapped toINT0 orINT1,
INTFLGRDYx issetright after thetransfer from TGx isfinished.
Write: The corresponding bitflagiscleared.
15-0 INTFLGSUS[
n]Transfer-group interrupt flagforatransfer-suspend interrupt. Bit0corresponds toTG0, bit1
corresponds toTG1, andsoon.
Note: Read Clear Behavior. Reading theinterrupt vector registers TGINTVECT0 orTGINTVECT1
automatically clears theinterrupt flagbitINTFLGSUSx referenced bythevector number given by
INTVECT0/INTVECT1 bits, iftheSUSPEND[0:1] bitinthecorresponding vector registers is1.
0 Read: Notransfer-suspended interrupt occurred since thelastclearing oftheINTFLGSUSx flag.
Write: Awrite of0tothisbithasnoeffect.
1 Read: Atransfer-suspended interrupt from TGx occurred. Nomatter whether theinterrupt isenabled or
disabled (INTENSUSx =don'tcare) orwhether theinterrupt ismapped toINT0 orINT1, INTFLGSUSx
issetright after thetransfer from transfer group xissuspended.
Write: The corresponding bitflagiscleared.

<!-- Page 1581 -->

Tick counter
Trigger signal
Counter reload
www.ti.com Control Registers
1581 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.32 Tick Count Register (TICKCNT)
One ofthetrigger sources forTGs isaninternal periodic time trigger. This time trigger iscalled atick
counter andisbasically adown-counter with apreload/reload value. Every time thetickcounter detects an
underflow itreloads theinitial value andtoggles thetrigger signal provided totheTGs.
The trigger signal, shown inFigure 28-67 asasquare wave, illustrates thedifferent trigger event types for
theTGs (forexample, rising edge, falling edge, andboth edges).
Figure 28-67. Tick Counter Operation
This register isshown inFigure 28-68 anddescribed inTable 28-41 .
Figure 28-68. Tick Count Register (TICKCNT) [offset =90h]
31 30 29 28 27 16
TICKENA RELOAD CLKCTRL Reserved
R/W-0 R/S-0 R/W-0 R-0
15 0
TICKVALUE
R/W-0
LEGEND: R=Read only; R/W =Read/Write; S=Set; -n=value after reset
Table 28-41. Tick Count Register (TICKCNT) Field Descriptions
Bit Field Value Description
31 TICKENA Tick counter enable.
0 The internal tickcounter isdisabled. The counter value remains unchanged.
Note: When thetickcounter isdisabled, thetrigger signal isforced low.
1 The internal tickcounter isenabled andisclocked bytheclock source selected byCLKCTRL.
When TICKENA goes from 0to1,thetickcounter isautomatically loaded with thecontents of
TICKVALUE.
30 RELOAD Pre-load thetickcounter. RELOAD isaset-only bit;writing a1reloads thetickcounter with the
value stored inTICKVALUE. Reading RELOAD always returns a0.
Note: When thetickcounter isreloaded bytheRELOAD bit,thetrigger signal isnot
toggled.
29-28 CLKCTRL Tick counter clock source control. CLKCTRL defines theclock source thatisused toclock the
internal tickcounter.
0 SPICLK ofdata word format 0isselected astheclock source ofthetickcounter.
1h SPICLK ofdata word format 1isselected astheclock source ofthetickcounter.
2h SPICLK ofdata word format 2isselected astheclock source ofthetickcounter.
3h SPICLK ofdata word format 3isselected astheclock source ofthetickcounter.
27-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 TICKVALUE 0-FFFFh Initial value forthetickcounter. TICKVALUE stores theinitial value forthetickcounter. The tick
counter isloaded with thecontents ofTICKVALUE every time anunderflow condition occurs and
every time theRELOAD flagissetbythehost.

<!-- Page 1582 -->

Control Registers www.ti.com
1582 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.33 Last TGEnd Pointer (LTGPEND)
Figure 28-69. Last TGEnd Pointer (LTGPEND) [offset =94h]
31 29 28 24 23 16
Reserved TGINSERVICE Reserved
R-0 R-0 R-0
15 8 7 0
LPEND Reserved
R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-42. Last TGEnd Pointer (LTGPEND) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reads return 0.Writes have noeffect.
28-24 TGINSERVICE The TGnumber currently being serviced bythesequencer. These bitsindicate thecurrent
TGthatisbeing serviced. This field cangenerally beused forcode debugging.
0 NoTGisbeing serviced bythesequencer.
1h TG0 isbeing serviced bythesequencer.
: :
10h TG15 isbeing serviced bythesequencer.
Note: Thenumber oftransfer groups varies bydevice.
11h-1Fh Invalid values.
23-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 LPEND 0-FFh Last TGendpointer. Usually theTGendaddress (PEND) isinherently defined bythestart
value ofthestarting pointer ofthesubsequent TG(PSTART). The TGends oneword
before thenext TGstarts (PEND[x] =PSTART[x+1] -1).Forafullconfiguration ofMibSPI,
thelastTGhasnosubsequent TG,thatis,noendaddress isdefined. Therefore, LPEND
hastobeprogrammed tospecify explicitly theendaddress ofthelastTG.
Note: ForMibSPI1 thatsupports 256buffers (values from 0-FFh), bit15isused. For
MibSPI2-5 thatsupport 128buffers (values from 0-7Fh), bit15isreserved.
Note: When using all8transfer groups, program theLPEND bitstodefine theendof
thelasttransfer group. When using less than 8transfer groups, leave theLPEND bits
programmed topoint totheendofthebuffer andcreate adummy transfer group that
defines theendofyour lastintentional transfer group andoccupies alltheremaining
buffer space.
7-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1583 -->

www.ti.com Control Registers
1583 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.34 TGx Control Registers (TGxCTRL)
Each TGcanbeconfigured viaonededicated control register. The register description shows onecontrol
register (x)thatisidentical forallTGs. Forexample, thecontrol register forTG2 isnamed TG2CTRL and
islocated atbase address +98h+4×2.The actual number ofavailable control registers varies by
device.
Figure 28-70. MibSPI TGControl Registers (TGxCTRL) [offsets =98h-D4h]
31 30 29 28 27 24 23 20 19 16
TGENA ONESHOT PRST TGTD Reserved TRIGEVT TRIGSRC
R/W-0 R/W-0 R/W-0 R-0 R-0 R/W-0 R/W-0
15 8 7 0
PSTART PCURRENT
R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-43. TGControl Registers (TGxCTRL) Field Descriptions
Bit Field Value Description
31 TGENA TGx enable.
Ifthecorrect event (TRIGEVTx) occurs attheselected source (TRIGSRCx) agroup transfer is
initiated ifnohigher priority TGisinactive transfer mode orifoneormore higher-priority TGs are
intransfer-suspend mode.
Disabling aTGwhile atransfer isongoing willfinish theongoing word transfer butnotthewhole
group transfer.
0 TGx isdisabled.
1 TGx isenabled.
30 ONESHOTx Single transfer forTGx.
0 TGx initiates atransfer every time atrigger event occurs andTGENA isset.
1 Atransfer from TGx willbeperformed only once (one shot) after avalid trigger event atthe
selected trigger source. After thetransfer isfinished theTGENAx control bitwillbecleared and
therefore noadditional transfer canbetriggered before thehost enables theTGagain. This one
shot mode ensures thatafter onegroup transfer thehost hasenough time toread thereceived
data andtoprovide new transmit data.
29 PRSTx TGx pointer reset mode. Configures theway toresolve trigger events during anongoing transfer.
This bitismeaningful only forlevel-triggered TGs. Edge-triggered TGs cannot berestarted before
their completion byanother edge. The PRST bitwillhave noeffect onthisbehavior.
Note: When thePRST bitisset,ifthebuffer being transferred atthetime ofanew trigger
event isaLOCK, CSHOLD orNOBRK buffer, then only after finishing those transfers, the
TGwillberestarted. This means thateven iftheTGisretriggered, theTGwillonly be
restarted after finishing thetransfer ofthefirst non-LOCK ornon-CSHOLD buffer. Inthe
case oftheNOBRK buffer, after completing theICOUNT number oftransfers, theTGwillbe
restarted from itsPSTART.
This means thatTXcontrol fields such asLOCK andCSHOLD, andDMA control fields such as
NOBRK have higher priority over anything else. They have thecapability todelay therestart ofthe
TGeven ifitisretriggered when PRST is1.
0 Ifatrigger event occurs during atransfer from TGx, theevent isignored andisnotstored
internally. The TGx transfer haspriority over additional trigger events.
1 The TGx pointer (PCURRENTx) willbereset tothestart address (PSTARTx) when avalid trigger
event occurs attheselected trigger source while atransfer from thesame TGisongoing. Every
trigger event resets PCURRENTx nomatter whether theconcerned TGisintransfer mode ornot.
The trigger events have priority over theongoing transfer.
28 TGTDx TGtriggered.
0 TGx hasnotbeen triggered orisnolonger waiting forservice.
1 TGx hasbeen triggered andiseither currently being serviced orwaiting forservicing.
27-24 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1584 -->

Control Registers www.ti.com
1584 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-43. TGControl Registers (TGxCTRL) Field Descriptions (continued)
Bit Field Value Description
23-20 TRIGEVTx Type oftrigger event. Alevel-triggered TGcanbestopped byde-activating thelevel trigger.
However, thefollowing restrictions apply.
*Deactivating thelevel trigger foraTGduring aNOBRK transfer does notstop thetransfers until
alloftheICOUNT number ofbuffers aretransferred fortheNOBRK buffer. Once aNOBRK
buffer isprefetched, thetrigger event loses control over theTGuntil theNOBRK buffer transfer
iscompleted.
*Once thetransfer ofabuffer with CSHOLD orLOCK bitsetstarts, deactivating thetrigger level
does notstop thetransfer until thesequencer completes thetransfer ofthenext non-CSHOLD
ornon-LOCK buffer inthesame TG.
*Once thelastbuffer inaTGispre-fetched, de-activating thetrigger level does notstop the
transfer group until thelastbuffer transfer iscompleted. This means even ifthetrigger level is
deactivated atthebeginning ofthepenultimate (one-before-last) buffer transfer, thesequencer
continues with thesame TGuntil itiscompleted.
0 never Never trigger TGx. This isthedefault value after reset.
1h rising
edgeArising edge (0to1)attheselected trigger source (TRIGSRCx) initiates atransfer for
TGx.
2h falling
edgeAfalling edge (1to0)attheselected trigger source (TRIGSRCx) initiates atransfer for
TGx.
3h both
edgesRising andfalling edges attheselected trigger source (TRIGSRCx) initiates atransfer
forTGx.
4h Rsvd Reserved
5h high-
activeWhile theselected trigger source (TRIGSRCx) isatalogic high level (1)thegroup
transfer iscontinued andattheendofonegroup transfer restarted atthebeginning. If
thelogic level changes tolow(0)during anongoing group transfer, thewhole group
transfer willbestopped.
Note: IfONESHOTx issetthetransfer isperformed only once.
6h low-
activeWhile theselected trigger source (TRIGSRCx) isatalogic lowlevel (0)thegroup
transfer iscontinued andattheendofonerestarted atthebeginning. Ifthelogic level
changes tohigh (1)during anongoing group transfer, thewhole group transfer willbe
stopped.
Note: IfONESHOTx issetthetransfer isperformed only once.
7h always Arepetitive group transfer willbeperformed.
Note: Bysetting theTRIGSRC to0,theTRIGEVT to7h(ALWAYS), andthe
ONESHOTx bitto1,software cantrigger thisTG.Upon setting theTGENA bit,the
TGisimmediately triggered.
Note: IfONESHOTx issetthetransfer isperformed only once.
1xxx Rsvd Reserved

<!-- Page 1585 -->

www.ti.com Control Registers
1585 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-43. TGControl Registers (TGxCTRL) Field Descriptions (continued)
Bit Field Value Description
19-16 TRIGSRCx Trigger source. After reset, thetrigger sources ofallTGs aredisabled.
0 Disabled
1h EXT0 External trigger source 0.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
2h EXT1 External trigger source 1.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
3h EXT2 External trigger source 2.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
4h EXT3 External trigger source 3.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
5h EXT4 External trigger source 4.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
6h EXT5 External trigger source 5.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
7h EXT6 External trigger source 6.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
8h EXT7 External trigger source 7.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
9h EXT8 External trigger source 8.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Ah EXT9 External trigger source 9.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Bh EXT10 External trigger source 10.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Ch EXT11 External trigger source 11.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Dh EXT12 External trigger source 12.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Eh EXT13 External trigger source 13.The actual source varies perdevice (forexample, HET I/O
channel, event pin).
Fh TICK Internal periodic event trigger. The tickcounter caninitiate periodic group transfers.
15-8 PSTARTx 0-FFh TGstart address. PSTARTx stores thestart address ofthecorresponding TG.The corresponding
endaddress isinherently defined bythesubsequent TGstart address minus 1(PENDx[TGx] =
PSTARTx[TGx+1]-1). PSTARTx iscopied intoPCURRENTx when:
*The TGisenabled.
*The endoftheTGisreached during atransfer.
*Atrigger event occurs while PRST issetto1.
Note: ForMibSPI1 thatsupports 256buffers (values from 0-FFh), bit15isused. For
MibSPI2-5 thatsupport 128buffers (values from 0-7Fh), bit15isreserved.
7-0 PCURRENTx 0-FFh Pointer tocurrent buffer. PCURRENT isread-only. PCURRENTx stores theaddress ofthebuffer
thatcorresponds tothisTG.IftheTGswitches from active transfer mode tosuspend towait,
PCURRENTx contains theaddress ofthecurrently suspended word. After theTGresumes from
suspend towait mode, thenext buffer willbetransferred; thatis,nobuffer data istransferred
because ofsuspend towait mode.
Note: ForMibSPI1 thatsupports 256buffers (values from 0-FFh), bit7isused. ForMibSPI2-
5thatsupport 128buffers (values from 0-7Fh), bit7isreserved.
NOTE: Register bitsvary bydevice
TG0 hasthehighest priority andTG15 hasthelowest priority. Under thefollowing conditions
alower priority TGcannot beinterrupted byahigher priority TG.
1.When there isaCSHOLD orLOCK buffer, until thecompletion ofthenext buffer
transfer which isanon-CSHOLD ornon-LOCK buffer.
2.Anentire sequence ofwords transferred foraNOBRK DMA buffer.
3.Once thelastword inaTGispre-fetched.

<!-- Page 1586 -->

Control Registers www.ti.com
1586 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.35 DMA Channel Control Register (DMAxCTRL)
Each DMA channel canbeconfigured viaonededicated control register. The register description below
shows oneexemplary control register thatisidentical forallDMA channels; forexample, thecontrol
register forDMA channel 0isnamed DMA0CTRL. The MibSPI supports upto8bidirectional DMA
channels.
The number ofbidirectional DMA channels varies bydevice. The number ofDMA channels andhence the
number ofDMA channel control registers may vary.
Figure 28-71. DMA Channel Control Register (DMAxCTRL) [offset =D8h-F4h]
31 30 24 23 20 19 16
ONESHOT BUFID RXDMA_MAP TXDMA_MAP
R/W-0 R/W-0 R/W-0 R/W-0
15 14 13 12 8
RXDMAENA TXDMAENA NOBRK ICOUNT
R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 0
Reserved COUNT BIT17 COUNT
R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-44. DMA Channel Control Register (DMAxCTRL) Field Descriptions
Bit Field Value Description
31 ONESHOT Auto-disable ofDMA channel after ICOUNT+1 transfers.
Note: This ONESHOT applies totheDMA channel identified byxandwillautodisable
based onICOUNTx.
0 The length oftheblock transfer isfully controlled bytheDMA controller. The enable bits
RXDMAENAx andTXDMAENAx arenotmodified bytheMibSPI.
1 ONESHOT allows ablock transfer ofdefined length (ICOUNTx+1), mainly controlled bythe
MibSPI andnotbytheDMA controller. After ICOUNTx +1transfers, theenable bits
RXDMAENAx andTXDMAENAx areautomatically cleared bytheMibSPI, hence nomore
DMA requests aregenerated. Inconjunction with NOBRKx, aburst transfer canbeinitiated
without anyother transfer through another buffer.
30-24 BUFIDx 0-7Fh Buffer utilized forDMA transfer. BUFIDx defines thebuffer thatisutilized fortheDMA
transfer. Inorder tosynchronize thetransfer with theDMA controller with theNOBRK
condition the"suspend towait until... "modes must beused.
23-20 RXDMA_MAPx 0-Fh Each MibSPI DMA channel canbelinked totwophysical DMA Request lines oftheDMA
controller. One request lineforreceive data andtheother forrequest linefortransmit data.
RXDMA_MAPx defines thenumber ofthephysical DMA Request linethatisconnected to
thereceive path oftheMibSPI DMA channel.
IfRXDMAENAx andTXDMAENAx areboth setto1,then RXDMA_MAPx shall differ from
TXDMA_MAPx andshall differ from anyother used physical DMA Request line. Otherwise
unexpected interference may occur.
19-16 TXDMA_MAPx 0-Fh Each MibSPI DMA channel canbelinked totwophysical DMA Request lines oftheDMA
controller. One request lineforreceive data andtheother forrequest linefortransmit data.
TXDMA_MAPx defines thenumber ofthephysical DMA Request linethatisconnected to
thetransmit path oftheMibSPI DMA channel.
IfRXDMAENAx andTXDMAENAx areboth setthen TXDMA_MAPx shall differ from
RXDMA_MAPx andshall differ from anyother used physical DMA Request line. Otherwise
unexpected interference may occur.
15 RXDMAENAx Receive data DMA channel enable.
0 NoDMA request upon new receive data.
1 The physical DMA channel forthereceive path isenabled. The firstDMA request pulse is
generated after thefirsttransfer from thereferenced buffer (BUFIDx) isfinished. The buffer
should beconfigured inas"skip until RXEMPTY isset"or"suspend towait until RXEMPTY
isset"inorder toensure synchronization between theDMA controller andtheMibSPI
sequencer.

<!-- Page 1587 -->

www.ti.com Control Registers
1587 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-44. DMA Channel Control Register (DMAxCTRL) Field Descriptions (continued)
Bit Field Value Description
14 TXDMAENAx Transmit data DMA channel enable.
0 NoDMA request upon new transmit data.
1 The physical DMA channel forthetransmit path isenabled. The firstDMA request pulse is
generated right after setting TXDMAENAx toload thefirsttransmit data. The buffer should
beconfigured intheas"skip until TXFULL isset"or"suspend towait until TXFULL isset"in
order toensure synchronization between theDMA controller andtheMibSPI sequencer.
13 NOBRKx Non-interleaved DMA block transfer. This bitisavailable inmaster mode only.
Note: Special Conditions during aNOBRK Buffer Transfer. IfaNOBRK DMA buffer is
currently being serviced bythesequencer, then itisnotallowed tobedisabled
prematurely.
During aNOBRK transfer, thefollowing operations arenotallowed:
*Clearing theNOBRKx bitto0
*Clearing theRXDMAENAx to0(ifitisalready 1)
*Clearing theTXDMAENAx to0(ifitisalready 1)
*Clearing theBUFMODE[2:0] bitsto000
Note: Any attempts toperform these actions during aNOBRK transfer willproduce
unpredictable results.
0 DMA transfers through thebuffer referenced byBUFIDx areinterleaved bydata transfers
from other active buffers orTGs. Every time thesequencer checks theDMA buffer, it
performs onetransfer andthen steps tothenext buffer.
1 NOBRKx ensures thatICOUNTx +1data transfers areperformed from thebuffer
referenced byBUFIDx without adata transfer from anyother buffer. The sequencer remains
attheDMA buffer until ICOUNTx +1transfers have been processed.For example, thiscan
beused togenerate aburst transfer toonedevice without disabling thechip select signal
in-between (the concerned buffer hastobeconfigured with CSHOLD =1).Another example
would betohave adefined block data transfer inslave mode, synchronous tothemaster
SPI.
Note: Triggering ofhigher priority TGs orenabling ofhigher priority DMA channels
willnotinterrupt aNOBRK block transfer.
12-8 ICOUNTx 0-1Fh Initial count ofDMA transfers. ICOUNTx isused topreset thetransfer counter COUNTx.
Every time COUNTx hits0,itisreloaded with ICOUNTx. The realnumber oftransfers
equals ICOUNTx plus 1.
IfONESHOTx isset,ICOUNTx defines thenumber ofDMA transfers thatareperformed
before theMibSPI automatically disables theDMA channels. IfNOBRKx isset,ICOUNTx
defines thenumber ofDMA transfers thatareperformed inonesequence without atransfer
from anyother buffer. IfONESHOTx andNOBRKx arenotset,ICOUNTx should be0.
Note: SeeSection 28.3.36 (ICOUNT) andSection 28.3.37 (DMACNTLEN) about how to
increase theICOUNT toa16-bit value. With thisextended capability, MibSPI can
transfer ablock ofupto65535 (65K) words without interleaving (ifNOBRK isused) or
without deasserting thechip select between thebuffers (ifCSHOLD isused).
7 Reserved 0 Reads return 0.Writes have noeffect.
6 COUNT BIT17x The 17th bitoftheCOUNT field ofDMAxCOUNT register.
5-0 COUNTx 0-3Fh Actual number ofremaining DMA transfers. This field contains theactual number ofDMA
transfers thatremain, until theDMA channel isdisabled, ifONESHOTx isset.
Note: IftheTXandRXDMA requests areenabled, theCOUNT register willbe
decremented when theRXhasbeen serviced.

<!-- Page 1588 -->

Control Registers www.ti.com
1588 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.36 DMAxCOUNT Register (ICOUNT)
NOTE: These registers areused only iftheLARGE COUNT bitintheDMACNTLEN register isset.
The number ofbidirectional DMA channels varies bydevice. The number ofDMA channels
andhence thenumber ofDMA registers varies bydevice.
Figure 28-72. DMAxCOUNT Register (ICOUNT) [offset =F8h-114h]
31 16
ICOUNTx
R/W-0
15 0
COUNTx
R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 28-45. MibSPI DMAxCOUNT Register (ICOUNT) Field Descriptions
Bit Field Value Description
31-16 ICOUNTx 0-FFFFh Initial number ofDMA transfers. ICOUNTx isused topreset thetransfer counter COUNTx.
Every time COUNTx hits0,itisreloaded with ICOUNTx. The realnumber oftransfer equals
ICOUNTx plus 1.IfONESHOTx isset,ICOUNTx defines thenumber ofDMA transfers thatare
performed before theMibSPI automatically disables thecorresponding DMA channel. If
NOBRKx isset,ICOUNTx defines thenumber ofDMA transfers thatareperformed inone
sequence without atransfer from anyother buffer
15-0 COUNTx 0-FFFFh Actual number ofremaining DMA transfers. COUNTx Contains theactual number ofDMA
transfers thatremain, until theDMA channel isdisabled, ifONESHOTx isset.Since thereal
counter value isalways ICOUNTx +1,the17th bitofCOUNTx isavailable onDMACTRLx[6]
bit.
Note: Usage TipforBlock Transfer Using aSingle DMA Request. Itispossible tousethe
multi-buffer RAM totransfer chunks ofdata to/from anexternal SPI. ADMA Controller
canbeused tohandle thedata inbursts. Suppose achunk of64bytes ofdata needs to
betransferred andasingle DMA request needs tobegenerated attheendoftransferring
the64bytes. This canbeeasily achieved byconfiguring aTGregister forthe64buffer
locations andusing theDMAxCTRL/DMAxCOUNT registers toconfigure thelastbuffer
(64th) oftheTGastheBUFID andenable RXDMA (NOBRK =0).Attheendofthetransfer
ofthe64th buffer, aDMA request willbegenerated ontheselected DMA request
channel. TheDMA controller candoaburst read ofall64bytes from RXRAM and/or then
doaburst write toall64bytes totheTXRAM forthenext chunk.

<!-- Page 1589 -->

www.ti.com Control Registers
1589 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.37 DMA Large Count (DMACNTLEN)
Figure 28-73. DMA Large Count Register (DMACNTLEN) [offset =118h]
31 16
Reserved
R-0
15 1 0
Reserved LARGE COUNT
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-46. MibSPI DMA Large Count Register (DMACNTLEN) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 LARGE COUNT Select either the16-bit DMAxCOUNT counters orthesmaller counters inDMAxCTRL.
0 Select theDMAxCTRL counters. Writes totheDMAxCTRL register willmodify theICOUNT
value. Reading ICOUNT andCOUNT canbedone from theDMAxCTRL register. The
DMAxCOUNT register should notbeused since anywrite tothisregister willbeoverwritten by
asubsequent write totheDMAxCTRL register tosettheTXDMAENA orRXDMAENA bits.
1 Select theDMAxCOUNT counters. Writes totheDMAxCTRL register willnotmodify the
ICOUNT value. The ICOUNT value must bewritten tointheDMAxCOUNT register before the
RXDMAENA orTXDMAENA bitsaresetintheDMAxCTRL register. The DMAxCOUNT register
should beused forreading COUNT orICOUNT.

<!-- Page 1590 -->

Control Registers www.ti.com
1590 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.38 Parity/ECC Control Register (PAR_ECC_CTRL)
Figure 28-74. Parity/ECC Control Register (PAR_ECC_CTRL) [offset =120]
31 28 27 24 23 20 19 16
Reserved SBE_EVT_EN Reserved EDAC_MODE
R-0 R/W-5h R-0 R/WP-Ah
15 9 8 7 4 3 0
Reserved PTESTEN Reserved EDEN
R-0 R/WP-0 R-0 R/W-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-47. MibSPI Parity/ECC Control Register (PAR_ECC_CTRL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 SBE_EVT_EN Single-Bit Error Event Enable This bitcontrols thegeneration oferror signaling (on
MIBSPI_SBERR port) whenever aSingle-Bit Error (SBE) isdetected onTXRAM/RXRAM.
This signal canbeused togenerate interrupt ifrequired.
5h Write: Disable Error Event indication upon detection ofSBE onTXRAM/RXRAM.
Ah Write: Enable Error Event upon detection ofSBE onTXRAM/RXRAM.
Allother values -writes areignored andthevalues arenotupdated intothisfield. The state
ofthefeature remains unchanged.
Read: Returns thecurrent value ofthefield.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 EDAC_MODE These bitsdetermine whether Single-Bit Errors (SBE) detected bytheSECDED block will
becorrected ornot.
5h Write: Disable correction ofSBE detected bytheSECDED block.
Ah Write: Enable correction ofSBE detected bytheSECDED block.
Allother values -writes areignored andthevalues arenotupdated intothisfield. The state
ofthefeature remains unchanged.
Read: Returns thecurrent value ofthefield.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PTESTEN Parity/ECC memory testenable. This bit,maps theparity/ECC bitscorresponding tomulti-
buffer RAM locations intotheperipheral RAM frame tomake them accessible bytheCPU.
User andprivilege mode (read) :
0 Parity/ECC bitsarenotmemory-mapped.
1 Parity/ECC bitsarememory-mapped.
Privilege mode (write):
0 Disable memory-mapping ofParity/ECC locations.
1 Enable memory-mapping ofParity/ECC locations.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EDEN Error Detection Enable These bitsenable Parity/ECC error detection.
5h Write: Disable Parity/ECC error detection logic (default).
Allother
valuesWrite: Enable Parity/ECC error detection logic.
Read: Returns thecurrent value ofthisfield.

<!-- Page 1591 -->

www.ti.com Control Registers
1591 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.39 Parity/ECC Status Register (PAR_ECC_STAT)
Figure 28-75. Parity/ECC Status Register (PAR_ECC_STAT) [offset =124]
31 16
Reserved
R-0
15 10 9 8
Reserved SBE_FLG1 SBE_FLG0
R-0 R/W1C-0 R/W1C-0
7 2 1 0
Reserved UERR_ FLG1 UERR_ FLG0
R-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 28-48. Parity/ECC Status Register (PAR_ECC_STAT) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9 SBE_FLG1 Single-Bit Error inRXRAM. This flagindicates ifasingle-bit ECC error occurred onreading
RXRAM.
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Single-bit error isdetected inRXRAM andtheaddress iscaptured inSBERRADDR1
register.
Write: Clears thebit.
8 SBE_FLG0 Single-Bit Error inTXRAM. This flagindicates ifasingle-bit ECC error occurred onreading
TXRAM.
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Single-bit error isdetected inTXRAM andtheaddress iscaptured inSBERRADDR0
register
Write: Clears thebit.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 UERR_FLG1 Uncorrectable Parity ordouble-bit ECC error detection flag. This flagindicates ifaParity or
double-bit ECC error occurred onreading RXRAM
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Error detected andtheaddress iscaptured inUERRADDR1 register.
Write: Clears thebit.
0 UERR_FLG0 Uncorrectable Parity ordouble-bit ECC error detection flag. This flagindicates ifaParity or
double-bit ECC error occurred onreading TXRAM
0 Read: Noerror occurred.
Write: Noeffect.
1 Read: Error detected andtheaddress iscaptured inUERRADDR0 register.
Write: Clears thebit.

<!-- Page 1592 -->

Control Registers www.ti.com
1592 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.40 Uncorrectable Parity orDouble-Bit ECC Error Address Register -RXRAM
(UERRADDR1)
Figure 28-76. Uncorrectable Parity orDouble-Bit ECC Error Address Register -RXRAM
(UERRADDR1) [offset =128h]
31 16
Reserved
R-0
15 11 10 0
Reserved UERRADDR1
R-0 RC-x
LEGEND: R/W =Read/Write; R=Read only; RC=Read toclear; -n=value after reset
Table 28-49. Uncorrectable Parity orDouble-Bit ECC Error Address Register -RXRAM
(UERRADDR1) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 UERRADDR1 Uncorrectable Parity ordouble-bit ECC error address This register holds theaddress ofthe
RAM location, ifaparity ordouble-bit ECC error isdetected when reading theMibSPI (Receive)
RXRAM. The address captured isbyte aligned when RAM Parity Check issupported. This error
address isfrozen from being updated until itisread bytheVBUS host.
Reading thisregister clears itscontents tothedefault value. The default value is400h if
Extended Buffer feature isenabled; else, itis200h. Writes tothisregister areignored.
NOTE: UERRADDR1 values
The offset address ofRXRAM canvary from 000h-1FFh, ifEXTENDED_BUF mode is
disabled. IftheEXTENDED_BUF mode isenabled, theoffset address canvary from 000h-
3FFh.
The register does notclear itscontents during andafter anyofthemodule-level resets, System-level
resets, oreven Power-on Reset.
NOTE: Aread toUERRADDR1 register willclear theUERR_FLG1 inPAR_ECC_STAT register.
However, inemulation mode (VBUSP_EMUDBG =1),theread toUERRADDR1 register
does notclear thecorresponding UERR_FLG1.
After apower-on reset thecontents ofthisregister willbeunpredictable. So,aread operation canbe
performed after power-up toclear itscontents ifrequired. Contents ofthisregister aremeaningful only
when UERR_FLG1 issetto1.
IfECC feature isimplemented, theSequencer FSM clearing theTXFULL flag(after aTXRAM location
read outandwritten totheshift register fortransfer) willtrigger read-modify-write operation totheRXRAM.
Similarly, each time FSM reads aTXRAM totransfer itout,thecorresponding RXRAM location isalso
automatically read todetermine thestatus ofthebuffer. Adouble-bit error could bedetected during these
FSM read operations andresult inerror address andflags getting captured.
NOTE: Clearing ofUERR status andaddress registers
After completing amemory testsequence, specifically where parity orECC features are
tested, user must read back thestatus flags inPAR_ECC_STAT andUERRADDRx registers
andensure thatthey areinnormal clear state byreading/writing appropriately. This canbe
performed before thestart ofanormal multi-buffer mode transactions aswell.
IfRAM Parity Check issupported, UERRADDR1[1:0] values willreflect thebyte positions offailed byte
based onthefollowing scheme totake care ofEndianness ofmemory organization.

<!-- Page 1593 -->

www.ti.com Control Registers
1593 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-50. Effect ofBIG_ENDIAN Port onUERRADDR1[1:0] Bits
Endianness
Fault Location isAmong theRAM Bits
1(Big Endian) 0(Little Endian)
UERRADDR1[1:0]00 11 7:0
01 10 15:8
10 01 23:16
11 00 31:24
NOTE: When ECC issupported, UERRADDR0 willindicate only word address. UERRADDR0[1:0]
willalways be00.

<!-- Page 1594 -->

Control Registers www.ti.com
1594 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.41 Uncorrectable Parity orDouble-Bit ECC Error Address Register -TXRAM
(UERRADDR0)
Figure 28-77. Uncorrectable Parity orDouble-Bit ECC Error Address Register -TXRAM
(UERRADDR0) [offset =12Ch]
31 16
Reserved
R-0
15 11 10 0
Reserved UERRADDR0
R-0 RC-x
LEGEND: R/W =Read/Write; R=Read only; RC=Read toclear; -n=value after reset
Table 28-51. Uncorrectable Parity orDouble-Bit ECC Error Address Register -TXRAM
(UERRADDR0) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 UERRADDR0 Uncorrectable Parity ordouble-bit ECC error address. This register holds theaddress when a
parity error isgenerated while reading theMibSPI (Transmit) TXRAM. The TXRAM canberead
either byCPU orbytheMibSPI Sequencer FSM logic fortransmission. The address captured is
byte aligned. This error address isfrozen from being updated until itisread bytheVBUSP host.
Reading thisregister clears itscontents tothedefault value of000. Writes tothisregister are
ignored.
NOTE: UERRADDR0 values
The offset address ofTXRAM canvary from 200h-3FFh, ifEXTENDED_BUF mode is
disabled. IftheEXTENDED_BUF mode isenabled, theoffset address canvary from 400h-
7FFh.
The register does notclear itscontents during andafter anyofthemodule-level resets, System-level
resets, oreven Power-on Reset.
NOTE: ARead toUERRADDR0 register willclear theUERR_FLG0 inPAR_ECC_STAT register.
However, inemulation mode (VBUSP_EMUDBG =1),theread toUERRADDR0 register
does notclear thecorresponding UERR_FLG0.
After apower-on reset thecontents willbeunpredictable. Aread operation canbeperformed after power-
uptokeep theregister atitsdefault value ifrequired. Contents ofthisregister aremeaningful only when
UERR_FLG0 issetto1.
IfECC feature isimplemented, theSequencer FSM clearing theTXFULL flag(after aTXRAM location
read outandwritten totheshift register fortransfer) willtrigger read-modify-write operation totheRXRAM.
Similarly, each time FSM reads aTXRAM totransfer itout,thecorresponding RXRAM location isalso
automatically read todetermine thestatus ofthebuffer. Adouble-bit error could bedetected during these
FSM read operations andresult inerror address andflags getting captured.
NOTE: Clearing ofUERR status andaddress registers
After completing amemory testsequence, specifically where parity orECC features are
tested, user must read back thestatus flags inPAR_ECC_STAT andUERRADDRx registers
andensure thatthey areinnormal clear state byreading/writing appropriately. This canbe
performed before thestart ofanormal multi-buffer mode transactions aswell.
IfRAM Parity Check issupported, UERRADDR0[1:0] values willreflect thebyte positions offailed byte
based onthefollowing scheme totake care ofEndianness ofmemory organization.

<!-- Page 1595 -->

www.ti.com Control Registers
1595 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-52. Effect ofBIG_ENDIAN Port onUERRADDR0[1:0] Bits
Endianness
Fault Location isAmong theRAM Bits
1(Big Endian) 0(Little Endian)
UERRADDR0[1:0]00 11 7:0
01 10 15:8
10 01 23:16
11 00 31:24
NOTE: When ECC issupported, UERRADDR0 willindicate only word address. UERRADDR0[1:0]
willalways be00.
28.3.42 RXRAM Overrun Buffer Address Register (RXOVRN_BUF_ADDR)
Inmulti-buffer mode, ifaparticular RXRAM location iswritten bytheMibSPI sequencer logic after the
completion ofanew transfer when thatlocation already contains valid data, theRX_OVR bitwillbesetto
1while thedata isbeing written. The RXOVRN_BUF_ADDR register captures theaddress oftheRXRAM
location forwhich areceiver overrun condition occurred.
Figure 28-78. RXRAM Overrun Buffer Address Register (RXOVRN_BUF_ADDR) [offset =130h]
31 16
Reserved
R-0
15 10 9 0
Reserved RXOVRN_BUF_ADDR
R-0 R-200h
LEGEND: R=Read only; -n=value after reset
Table 28-53. RXRAM Overrun Buffer Address Register (RXOVRN_BUF_ADDR) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 RXOVRN_BUF_ADDR 200h-3FCh Address inRXRAM atwhich anoverwrite occurred. This address value willshow only
theoffset address oftheRAM location inthemulti-buffer RAM address space. Refer
tothedevice-specific data sheet fortheactual absolute address ofRXRAM.
This word-aligned address canvary from 200h-3FCh. Contents ofthisregister are
valid only when anyoftheINTVECT0 orINTVECT1 andSPIFLG registers show an
RXOVRN error vector while inmulti-buffer mode. Ifthere aremultiple overrun errors,
then thisregister holds theaddress offirstoverrun address until itisread.
Note: Reading thisregister clears theRXOVRN interrupt flagintheSPIFLG
register andtheTGINTVECTx.
Note: Receiver overrun errors inmulti-buffer mode canbecompletely avoided
byusing theSUSPEND until RXEMPTY feature, which canbeprogrammed into
each buffer ofanyTG.However, using theSUSPEND until RXEMPTY feature will
make thesequencer wait until thecurrent RXRAM location isread bytheVBUS
master before itcanstart thetransfer forthesame buffer location again. This
may affect theoverall throughput oftheSPItransfer. Byenabling theinterrupt
onRXOVRN inmulti-buffer mode, theuser canrelyoninterrupts toknow ifa
receiver overrun hasoccurred. Theaddress oftheoverrun inRXRAM is
indicated inthisRXOVRN_BUF_ADDR register .

<!-- Page 1596 -->

Control Registers www.ti.com
1596 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.43 I/O-Loopback Test Control Register (IOLPBKTSTCR)
This register controls testmode forI/Opins. Italso controls whether loop-back should bedigital oranalog.
Inaddition, itcontains control bitstoinduce error conditions intothemodule. These aretobeused only for
module testing.
Allofthecontrol/status bitsinthisregister arevalid only when theIOLPBKTSTENA field issettoAh.
Figure 28-79. I/O-Loopback Test Control Register (IOLPBKTSTCR) [offset =134h]
31 25 24
Reserved SCS FAIL FLG
R-0 R/W1C-0
23 21 20 19 18 17 16
Reserved CTRL
BITERRCTRL
DESYNCCTRL
PARERRCTRL
TIMEOUTCTRL
DLENERR
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 12 11 8
Reserved IOLPBKTSTENA
R-0 R/WP-0
7 6 5 3 2 1 0
Reserved ERR SCS PIN CTRL SCS
PINERRLPBKTYPE RXPENA
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; WP=Write inprivilege mode only; -n=value after reset
Table 28-54. I/O-Loopback Test Control Register (IOLPBKTSTCR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 SCS FAIL FLG Bitindicating afailure onSPICS pincompare during analog loopback.
0 Read: Nomiscompares occurred onanyoftheeight chip select pins (vs.theinternal
chip select number CSNR during transfers).
Write: Noeffect.
1 Read: Acomparison between theinternal CSNR field andtheanalog looped-back
value ofoneormore oftheSPICS pins failed. Astuck-at fault isdetected ononeofthe
SPICS pins. Comparison isdone only onthepins thatareconfigured asfunctional and
during transfer operation.
Write: This flagbitiscleared.
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 CTRL BITERR Controls inducing ofBITERR during I/Oloopback testmode.
0 Donotinterfere with looped-back data.
1 Induces biterrors byinverting thevalue oftheincoming data during loopback.
19 CTRL DESYNC Controls inducing ofthedesync error during I/Oloopback testmode.
0 Donotcause adesync error.
1 Induce adesync error byforcing theincoming SPIENA pin(iffunctional) toremain 0
even after thetransfer iscomplete. This forcing willberetained until thekernel reaches
theidlestate.
18 CTRL PARERR Controls inducing oftheparity errors during I/Oloopback testmode.
0 Donotcause aparity error.
1 Induce aparity error byinverting thepolarity oftheparity bit.
17 CTRL TIMEOUT Controls inducing ofthetimeout error during I/Oloopback testmode.
0 Donotcause atimeout error.
1 Induce atimeout error byforcing theincoming SPIENA pin(iffunctional) toremain 1
when transmission isinitiated. The forcing willberetained until thekernel reaches the
idlestate.

<!-- Page 1597 -->

www.ti.com Control Registers
1597 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-54. I/O-Loopback Test Control Register (IOLPBKTSTCR) Field Descriptions (continued)
Bit Field Value Description
16 CTRL DLENERR Controls inducing ofthedata length error during I/Oloopback testmode.
0 Donotcause adata-length error.
1 Induce adata-length error.
Master mode: The SPIENA pin(iffunctional) isforced to1when themodule starts
shifting data.
Slave mode: The incoming SPICS pin(iffunctional) isforced to1when themodule
starts shifting data.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 IOLPBKSTENA Module I/Oloopback testenable key.
Ah Enable I/Oloopback testmode.
AllOther Values Disable I/Oloopback testmode.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-3 ERR SCS PIN Inject error onchip-select pinnumber x.The value inthisfield isdecoded asthe
number ofthechip select pinonwhich toinject anerror. During analog loopback, if
CTRL SCS PINERR bitissetto1,then thechip select pinselected bythisfield is
forced totheopposite ofitsvalue intheCSNR.
0 Select SPICS[0] forinjecting error.
1h Select SPICS[1] forinjecting error.
: :
7h Select SPICS[7] forinjecting error.
2 CTRL SCS PINERR Enable theinjection ofanerror ontheSPICS pins. The individual SPICS pins canbe
chosen using theERR SCS PINfield.
0 Disable theSPICS error-inducing logic.
1 Enable theSPICS error-inducing logic.
1 LPBK TYPE Module I/Oloopback type (analog/digital). See Figure 28-31 forthedifferent types of
loopback modes.
0 Enable Digital loopback when IOLPBKTSTENA =1010.
1 Enable Analog loopback when IOLPBKTSTENA =1010.
0 RXP ENA Enable analog loopback through thereceive pin.
Note: This bitisvalid only when LPBK TYPE =1,which chooses analog
loopback mode.
0 Analog loopback isthrough thetransmit pin.
1 Analog loopback isthrough thereceive pin.

<!-- Page 1598 -->

Control Registers www.ti.com
1598 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.44 SPIExtended Prescale Register 1(EXTENDED_PRESCALE1 forSPIFMT0 and
SPIFMT1)
This register provides anextended Prescale values forSPICLK generation tobeable tointerface with
much slower SPISlaves. This isanextension ofSPIFMT0 andSPIFMT1 registers. Forexample,
EPRESCALE_FMT1(7:0) ofEXTENDED_PRESCALE1 andPRESCALE1(7:0) ofSPIFMT1 register will
always reflect thesame contents. Similarly EPRESCALE_FMT0(7:0) andPRESCALE0(7:0) ofSPIFMT0
reflect thesame contents.
Figure 28-80. SPIExtended Prescale Register 1(EXTENDED_PRESCALE1 forSPIFMT0 and
SPIFMT1) [offset =138h]
31 27 26 16
Reserved EPRESCALE_FMT1
R-0 R/WP-0
15 11 10 0
Reserved EPRESCALE_FMT0
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-55. SPIExtended Prescale Register 1(EXTENDED_PRESCALE1) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 EPRESCALE_FMT1 0-7FFh EPRESCALE_FMT1. Extended Prescale value forSPIFMT1. EPRESCALE_FMT1
determines thebittransfer rate ofdata format 1iftheSPI/MibSPI isthenetwork
master. EPRESCALE_FMT1 isusetoderive SPICLK from VCLK. IftheSPIis
configured asslave, EPRESCALE_FMT1 does notneed tobeconfigured. These
EPRESCALE_FMT1(7:0) bitsandPRESCALE1 bitsofSPIFMT1 register willpoint to
thesame physically implemented register. The clock rate fordata format 1canbe
calculated as:
BRFormat1 =VCLK /(EPRESCALE_FMT1 +1)
Write: This register field should bewritten ifaSPICLK prescaler ofmore VCLK/256 is
required. This field provides aprescaler ofuptoVCLK/2048 forSPICLK. Writing tothis
register field willalso getreflected inSPIFMT1(15:8).
Read: Reading thisfield willreflect thePRESCALE value based onthelastwritten
register field, thatis,EXTENDED_PRESCALE1(26:16) orSPIFMT1(15:8) register.
Note: IfExtended Prescaler isrequired, itshould beensured that
EXTENDED_PRESCALE1 register isprogrammed after SPIFMT1 register is
programmed. This istoensure thatthefinal SPICLK prescale value iscontrolled
byEXTENDED_PRESCALE1 register when aprescale ofmore 256isintended on
SPICLK. Writing toPRESCALE1 field ofSPIFMT1 willautomatically clear
EPRESCALE_FMT1(10:8) bitsto000sothattheintegrity ofPRESCALE value is
maintained.
15-11 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1599 -->

www.ti.com Control Registers
1599 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-55. SPIExtended Prescale Register 1(EXTENDED_PRESCALE1) Field Descriptions (continued)
Bit Field Value Description
10-0 EPRESCALE_FMT0 0-7FFh EPRESCALE_FMT0. Extended Prescale value forSPIFMT0. EPRESCALE_FMT0
determines thebittransfer rate ofdata format 0iftheSPI/MibSPI isthenetwork
master. EPRESCALE_FMT0 isusetoderive SPICLK from VCLK. IftheSPIis
configured asslave, EPRESCALE_FMT0 does notneed tobeconfigured. These
EPRESCALE_FMT0(7:0) bitsandPRESCALE0 bitsofSPIFMT0 register willpoint to
thesame physically implemented register. The clock rate fordata format 0canbe
calculated as:
BRFormat0 =VCLK /(EPRESCALE_FMT0 +1)
Write: This register field should bewritten ifaSPICLK prescaler ofmore VCLK/256 is
required. This field provides aprescaler ofuptoVCLK/2048 forSPICLK. Writing tothis
register field willalso getreflected inSPIFMT0(15:8).
Read: Reading thisfield willreflect thePRESCALE value based onthelastwritten
register field, thatis,EXTENDED_PRESCALE0(10:0) orSPIFMT0(15:8) register.
Note: IfExtended Prescaler isrequired, itshould beensured that
EXTENDED_PRESCALE1 register isprogrammed after SPIFMT0 register is
programmed. This istoensure thatthefinal SPICLK prescale value iscontrolled
byEXTENDED_PRESCALE1 register when aprescale ofmore 256isintended on
SPICLK. Writing toPRESCALE0 field ofSPIFMT0 willautomatically clear
EPRESCALE_FMT0(10:8) bitsto000sothattheintegrity ofPRESCALE value is
maintained.

<!-- Page 1600 -->

Control Registers www.ti.com
1600 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.45 SPIExtended Prescale Register 2(EXTENDED_PRESCALE2 forSPIFMT2 and
SPIFMT3)
This register provides anextended Prescale values forSPICLK generation tobeable tointerface with
much slower SPISlaves. This isanextension ofSPIFMT2 andSPIFMT3 registers. Forexample,
EPRESCALE_FMT3(7:0) ofEXTENDED_PRESCALE2 andPRESCALE3(7:0) ofSPIFMT3 register will
always reflect thesame contents. Similarly EPRESCALE_FMT2(7:0) andPRESCALE2(7:0) ofSPIFMT2
reflect thesame contents.
Figure 28-81. SPIExtended Prescale Register 2(EXTENDED_PRESCALE2 forSPIFMT2 and
SPIFMT3) [offset =13Ch]
31 27 26 16
Reserved EPRESCALE_FMT3
R-0 R/WP-0
15 11 10 0
Reserved EPRESCALE_FMT2
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-56. SPIExtended Prescale Register 2(EXTENDED_PRESCALE2) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-16 EPRESCALE_FMT3 0-7FFh EPRESCALE_FMT3. Extended Prescale value forSPIFMT3. EPRESCALE_FMT3
determines thebittransfer rate ofdata format 3,iftheSPI/MibSPI isthenetwork
master. EPRESCALE_FMT3 isusetoderive SPICLK from VCLK. IftheSPIis
configured asslave, EPRESCALE_FMT3 does notneed tobeconfigured. These
EPRESCALE_FMT3(7:0) bitsandPRESCALE3 bitsofSPIFMT3 register willpoint to
thesame physically implemented register. The clock rate fordata format 3canbe
calculated as:
BRFormat3 =VCLK /(EPRESCALE_FMT3 +1)
Write: This register field should bewritten ifaSPICLK prescaler ofmore VCLK/256 is
required. This field provides aprescaler ofuptoVCLK/2048 forSPICLK. Writing tothis
register field willalso getreflected inSPIFMT3(15:8).
Read: Reading thisfield willreflect thePRESCALE value based onthelastwritten
register field, thatis,EXTENDED_PRESCALE3(26:16) orSPIFMT3(15:8) register.
Note: IfExtended Prescaler isrequired, itshould beensured that
EXTENDED_PRESCALE2 register isprogrammed after SPIFMT3 register is
programmed. This istoensure thatthefinal SPICLK prescale value iscontrolled
byEXTENDED_PRESCALE2 register when aprescale ofmore 256isintended on
SPICLK. Writing toPRESCALE3 field ofSPIFMT3 willautomatically clear
EPRESCALE_FMT3(10:8) bitsto000sothattheintegrity ofPRESCALE value is
maintained.
15-11 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1601 -->

www.ti.com Control Registers
1601 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-56. SPIExtended Prescale Register 2(EXTENDED_PRESCALE2) Field Descriptions (continued)
Bit Field Value Description
10-0 EPRESCALE_FMT2 0-7FFh EPRESCALE_FMT2. Extended Prescale value forSPIFMT2. EPRESCALE_FMT2
determines thebittransfer rate ofdata format 2,iftheSPI/MibSPI isthenetwork
master. EPRESCALE_FMT2 isusetoderive SPICLK from VCLK. IftheSPIis
configured asslave, EPRESCALE_FMT2 does notneed tobeconfigured. These
EPRESCALE_FMT2(7:0) bitsandPRESCALE2 bitsofSPIFMT2 register willpoint to
thesame physically implemented register. The clock rate fordata format 2canbe
calculated as:
BRFormat2 =VCLK /(EPRESCALE_FMT2 +1)
Write: This register field should bewritten ifaSPICLK prescaler ofmore VCLK/256 is
required. This field provides aprescaler ofuptoVCLK/2048 forSPICLK. Writing tothis
register field willalso getreflected inSPIFMT2(15:8).
Read: Reading thisfield willreflect thePRESCALE value based onthelastwritten
register field, thatis,EXTENDED_PRESCALE2(10:0) orSPIFMT2(15:8) register.
Note: IfExtended Prescaler isrequired, itshould beensured that
EXTENDED_PRESCALE2 register isprogrammed after SPIFMT2 register is
programmed. This istoensure thatthefinal SPICLK prescale value iscontrolled
byEXTENDED_PRESCALE2 register when aprescale ofmore 256isintended on
SPICLK. Writing toPRESCALE2 field ofSPIFMT2 willautomatically clear
EPRESCALE_FMT2(10:8) bitsto000sothattheintegrity ofPRESCALE value is
maintained.
28.3.46 ECC Diagnostic Control Register (ECCDIAG_CTRL)
Figure 28-82. ECC Diagnostic Control Register (ECCDIAG_CTRL) [offset =140h]
31 16
Reserved
R-0
15 4 3 0
Reserved ECCDIAG_EN
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 28-57. ECC Diagnostic Control Register (ECCDIAG_CTRL) Field Descriptions
Bit Field Value Description
31-4 Reserved Reads return 0.Writes have noeffect.
3-0 ECCDIAG_EN ECC Diagnostic mode Enable Key bits.
5h Write: Diagnostic mode isenabled. Writes andreads from ECC bitsallowed from theECC
address space.
Allother
valuesWrite: Diagnostic mode isdisabled. Nowrites toECC bitsareignored.
Read: Returns 0.

<!-- Page 1602 -->

Control Registers www.ti.com
1602 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.47 ECC Diagnostic Status Register (ECCDIAG_STAT)
NOTE: ECCDIAG_STAT Validity
Both SEFLG andDEFLG arevalid only during Diagnostic Mode (when ECCDIAG_EN =5h).
This status register should bewrite-cleared after coming outofDiagnostic Mode.
Figure 28-83. ECC Diagnostic Status Register (ECCDIAG_STAT) [offset =144h]
31 18 17 16
Reserved DEFLG
R-0 R/W1C-0
15 2 1 0
Reserved SEFLG
R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 28-58. ECC Diagnostic Status Register (ECCDIAG_STAT) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17 DEFLG[1] Double-bit error flag.
0 Read: Noerror.
Write: Noeffect.
1 Read: Adouble-bit error isdetected forRXRAM bank during diagnostic mode tests.
Write: Clears thebit.
16 DEFLG[0] Double-bit error flag.
0 Read: Noerror.
Write: Noeffect.
1 Read: Adouble-bit error isdetected forTXRAM bank during diagnostic mode tests.
Write: Clears thebit.
15-2 Reserved 0 Reads return 0.Writes have noeffect.
1 SEFLG[1] Single-bit error flag.
0 Read: Noerror.
Write: Noeffect.
1 Read: Asingle-bit error isdetected forRXRAM bank during diagnostic mode tests.
Write: Clears thebit.
0 SEFLG[0] Single-bit error flag.
0 Read: Noerror.
Write: Noeffect.
1 Read: Asingle-bit error isdetected forTXRAM bank during diagnostic mode tests.
1 Write: Clears thebit.

<!-- Page 1603 -->

www.ti.com Control Registers
1603 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.48 Single-Bit Error Address Register -RXRAM (SBERRADDR1)
Figure 28-84. Single-Bit Error Address Register -RXRAM (SBERRADDR1) [offset =148h]
31 16
Reserved
R-0
15 11 10 0
Reserved SBERRADDR1
R-0 RC-0
LEGEND: R=Read only; RC=Read toclear; -n=value after reset
Table 28-59. Single-Bit Error Address Register -RXRAM (SBERRADDR1) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 SBERRADDR1 This register holds theaddress oftheRAM location when asingle-bit error isgenerated by
SECDED block while reading theMibSPI (Receive) RXRAM. This error address isfrozen from
being updated until itisread bytheVBUS host.
Reading thisregister clears itscontents tothedefault value. The default value is400h if
Extended Buffer feature isenabled; else, itis200h. Writes tothisregister areignored.
NOTE: SBERRADDR1 values
The offset address ofRXRAM canvary from 200h-3FFh, ifEXTENDED_BUF mode is
disabled. IftheEXTENDED_BUF mode isenabled, theoffset address canvary from 400h-
7FFh.
The register does notclear itscontents during andafter anyofthemodule-level resets, System-level
resets, oreven Power-on Reset.
NOTE: ARead toSBERRADDR1 Register willclear theSBE_FLG1 inPAR_ECC_STAT register.
However, inemulation mode (VBUSP_EMUDBG =1),theread toSBERRADDR1 register
does notclear thecorresponding SBE_FLG1.
After apower-on reset thecontents willbeunpredictable. Aread operation canbeperformed after power-
uptokeep theregister atitsdefault value ifrequired. Contents ofthisregister aremeaningful only when
SBE_FLG1 issetto1.
IfECC feature isimplemented, theSequencer FSM clearing theTXFULL flag(after aTXRAM location
read outandwritten totheshift register fortransfer) willtrigger read-modify-write operation totheRXRAM.
Similarly, each time FSM reads aTXRAM totransfer itout,thecorresponding RXRAM location isalso
automatically read todetermine thestatus ofthebuffer. Asingle-bit error could bedetected during these
FSM read operations andresult inerror address andflags getting captured.
NOTE: Clearing ofSBERR status andaddress registers
After completing amemory testsequence, specifically where ECC features aretested, user
must read back thestatus flags inECC_STAT andSBERRADDRx registers andensure that
they areinnormal clear state byreading/writing appropriately. This canbeperformed before
thestart ofanormal multi-buffer mode transactions aswell.
NOTE: When ECC issupported, SBERRADDR1 willindicate only word address.
SBERRADDR1[1:0] willalways be00.

<!-- Page 1604 -->

Control Registers www.ti.com
1604 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.3.49 Single-Bit Error Address Register -TXRAM (SBERRADDR0)
Figure 28-85. Single-Bit Error Address Register -TXRAM (SBERRADDR0) [offset =14Ch]
31 16
Reserved
R-0
15 11 10 0
Reserved SBERRADDR0
R-0 RC-0
LEGEND: R=Read only; RC=Read toclear; -n=value after reset
Table 28-60. Single-Bit Error Address Register -TXRAM (SBERRADDR0) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-0 SBERRADDR0 This register holds theaddress when asingle-bit error isgenerated from SECDED block while
reading theMibSPI (Transmit) TXRAM. The TXRAM canberead either byCPU orbythe
MibSPI Sequencer logic fortransmission. This error address isfrozen from being updated until
itisread bytheVBUSP host.
Reading thisregister clears itscontents tothedefault value of0x000. Writes tothisregister are
ignored.
NOTE: SBERRADDR0 values
The offset address ofTXRAM canvary from 000h-1FFh, ifEXTENDED_BUF mode is
disabled. IftheEXTENDED_BUF mode isenabled, theoffset address canvary from 000h-
3FFh.
The register does notclear itscontents during andafter anyofthemodule-level resets, System-level
resets, oreven Power-on Reset. ARead operation tothisregister clears itscontents toall0s.
NOTE: Aread toSBERRADDR0 register willclear theSBE_FLG0 inPAR_ECC_STAT register.
However, inemulation mode (VBUSP_EMUDBG =1),theread toSBERRADDR0 register
does notclear thecorresponding SBE_FLG0.
After apower-on reset thecontents ofthisregister willbeunpredictable. So,aread operation canbe
performed after power-up toclear itscontents ifrequired. Contents ofthisregister aremeaningful only
when SBE_FLG0 issetto1.
NOTE: Clearing ofSBERR status andaddress registers
After completing amemory testsequence, specifically where ECC features aretested, user
must read back thestatus flags inECC_STAT andSBERRADDRx registers toensure that
they areinnormal clear state byreading/writing appropriately. This canbeperformed before
thestart ofanormal multi-buffer mode transactions aswell.
NOTE: When ECC issupported, SBERRADDR0 willindicate only word address.
SBERRADDR0[1:0] willalways be00.

<!-- Page 1605 -->

ECC0
ECC1
ECC2
ECC3Control0 Transmit0 ECC0 Status0 Receive0
Control1 Transmit1 ECC1 Status1 Receive1
Control2 Transmit2 ECC2 Status2 Receive2
Control3 Transmit3 ECC3 Status3 Receive3
Control126 Transmit126 ECC126 Status126 Receive126
Control127 Transmit127 ECC127 Status127 Receive127ECC126
ECC127Buffer 0
1
2
3
...
126
1270 1516 3132 38 0 1516 3132 38
Optional Optional
Parity0
Parity1
Parity2
Parity3Control0 Transmit0 Parity0 Status0 Receive0
Control1 Transmit1 Parity1 Status1 Receive1
Control2 Transmit2 Parity2 Status2 Receive2
Control3 Transmit3 Parity3 Status3 Receive3
Control126 Transmit126 Parity126 Status126 Receive126
Control127 Transmit127 Parity127 Status127 Receive127Parity126
Parity127Buffer 0
1
2
3
...
126
1270 1516 3132 35 0 1516 3132 35
Optional OptionalTXRAM Bank RXRAM Bank
www.ti.com Multi-buffer RAM
1605 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.4 Multi-buffer RAM
The multi-buffer RAM comprises ofallbuffers, which canbeconfigured identically. The multi-buffer RAM
contains twobanks ofupto128/256 words of32bitsforamaximum configuration, oneeach forTXRAM
(replicating theSPIDAT1 register) andRXRAM (replicating theSPIBUF register). The buffers canbe
partitioned intomultiple transfer groups, each containing avariable number ofbuffers. Each ofthebuffers
canbesub-divided intoa16-bit transmit field, a16-bit receive field, a16-bit control field, anda16-bit
status field. A4-bit parity field perword isalso included ineach RAM bank, asshown inFigure 28-86 .If
ECC support isimplemented forRAM fault detection, then a7-bit ECC field perword isalso included in
each RAM bank, asshown inFigure 28-87 .
Figure 28-86. Multi-buffer RAM Configuration When Parity Check isSupported
Depth willbeupto256buffers, ifEXTENDED_BUF feature isimplemented.
Figure 28-87. Multi-buffer RAM Configuration When ECC Check isSupported
Depth willbeupto256buffers, ifEXTENDED_BUF feature isimplemented.
Allfields canberead andwritten with 8-bit, 16-bit, or32-bit accesses.
The transmit fields canbewritten andread intheaddress range 000h to1FFh. The transmit words
contain data andcontrol fields.
The receive RAM fields areread-only andcanbeaccessed through theaddress range 200h to3FCh. The
receive words contain data andstatus fields.
The chip select number bitfield CSNR[7:0] ofthecontrol field foragiven word ismirrored intothe
corresponding receive-buffer status field after transmission.
The Parity isautomatically calculated andcopied toParity location

<!-- Page 1606 -->

Multi-buffer RAM www.ti.com
1606 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)NOTE: Refer tothespecific device datasheet fortheactual number oftransmit andreceive buffers.
Write tounimplemented buffer isoverwriting thecorresponding implemented buffer. In
MIBSPI, iftheRAM SIZE specified is32buffers, write to33rd buffer overwrites 1stbuffer,
write to34th buffer overwrites 2ndbuffer, andsoon.
28.4.1 Multi-buffer RAM Auto Initialization
When theMIBSPI isoutofreset mode, auto initialization ofmulti-buffer RAM starts. The application code
must check forBUFINITACTIVE bittobe0(multi-buffer RAM initialization iscomplete) before configuring
multi-buffer RAM.
Besides thedefault auto initialization after reset, theauto-initialization sequence canalso bedone by:
1.Enable theglobal hardware memory initialization keybyprogramming avalue of1010b tothebits[3:0]
oftheMINITGCR register oftheSystem module.
2.Setthecontrol bitforthemulti-buffer RAM intheMSINENA System module register. This bitisdevice-
specific foreach memory thatsupport auto-initialization. Please refer tothedevice datasheet toidentify
thecontrol bitforthemulti-buffer RAM. This starts theinitialization process. The BUFINITACTIVE bit
willgetsettoreflect thattheinitialization isongoing.
3.When thememory initialization iscompleted, thecorresponding status bitintheMINISTAT register will
beset.Also, theBUFINITACTIVE bitwillgetcleared.
4.Disable theglobal hardware memory initialization keybyprogramming avalue of0101 tothebits[3:0]
oftheMINITGCR register oftheSystem module.
Please refer totheArchitecture User Guide formore details onthememory auto-initialization process.
NOTE: During Auto Initialization process, allthemulti-buffer mode registers (except MIBSPIE) will
bereset totheir default values. So,itshould beensured thatAuto Initialization iscompleted
before configuring themulti-buffer mode register.
28.4.2 Multi-buffer RAM Register Summary
This section describes themulti-buffer RAM control andtransmit-data fields ofeach word ofTXRAM, and
thestatus andreceive-data fields ofeach word ofRXRAM. The base address formulti-buffer RAM is
FF0E 0000h forMibSPI1 RAM, FF08 0000h forMibSPI2 RAM, FF0C 000h forMibSPI3 RAM, FF06 0000h
forMibSPI4 RAM, andFF0A 0000h forMibSPI5 RAM.
Table 28-61. Multi-buffer RAM Register
Offset Acronym Register Description Section
Base +0h-1FFh TXRAM Multi-buffer RAM Transmit Data Register Section 28.4.3
Base +200h-3FFh RXRAM Multi-buffer RAM Receive Buffer Register Section 28.4.4

<!-- Page 1607 -->

www.ti.com Multi-buffer RAM
1607 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.4.3 Multi-buffer RAM Transmit Data Register (TXRAM)
Each word ofTXRAM isatransmit-buffer register.
NOTE: Writing toonly thecontrol fields, bits28through 16,does notinitiate anySPItransfer in
master mode. This feature canbeused tosetupSPICLK phase orpolarity before actually
starting thetransfer byonly updating theDFSEL bitfield toselect therequired phase and
polarity combination.
Figure 28-88. Multi-buffer RAM Transmit Data Register (TXRAM)
[offset =Base +000-1FFh]
31 29 28 27 26 25 24 23 16
BUFMODE CSHOLD LOCK WDEL DFSEL CSNR
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
15 0
TXDATA
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 28-62. Multi-buffer RAM Transmit Data Register (TXRAM) Field Descriptions
Bit Field Value Description
31-29 BUFMODE Specify conditions thatarerecognized bythesequencer toinitiate transfers ofeach buffer word.
When oneofthe"skip"modes isselected, thesequencer checks thebuffer status every time it
reads from thisbuffer. Ifthecurrent buffer status (TXFULL, RXEMPTY) does notmatch, thebuffer
isskipped without adata transfer.
When oneofthe"suspend "modes isselected, thesequencer checks thebuffer status when it
reads from thisbuffer. IfTXFULL and/or RXEMPTY donotmatch, thesequencer waits until a
match occurs. Nodata transfer isinitiated until thestatus condition ofthisbuffer changes.
0 disabled. The buffer isdisabled.
1h skip single-transfer mode. Skip thisbuffer until thecorresponding TXFULL flagisset(new
transmit data isavailable).
2h skip overwrite-protect mode. Skip thisbuffer until thecorresponding RXEMPTY flagisset(new
receive data canbestored inRXDATA without data loss).
3h skip single-transfer overwrite-protect mode .Skip thisbuffer until both ofthecorresponding
TXFULL andRXEMPTY flags areset.(new transmit data available andprevious data received by
thehost).
4h continuous mode. Initiate atransfer each time thesequencer checks thisbuffer. Data words are
retransmitted ifthebuffer hasnotbeen updated. Receive data isoverwritten, even ifithasnot
been read.
5h suspend single-transfer mode. Suspend-to-wait until thecorresponding TXFULL flagisset(the
sequencer stops atthecurrent buffer until new transmit data iswritten intheTXDATA field).
6h suspend overwrite-protect mode. Suspend-to-wait until thecorresponding RXEMPTY flagisset
(the sequencer stops atthecurrent buffer until thepreviously-received data isread bythehost.
7h suspend single-transfer overwrite-protect mode. Suspend-to-wait until thecorresponding
TXFULL andRXEMPTY flags areset(the sequencer stops atthecurrent buffer until new transmit
data iswritten intotheTXDATA field andthepreviously-received data isread bythehost).
28 CSHOLD Chip select hold mode. The CSHOLD bitissupported inmaster mode only, itisignored inslave
mode. CSHOLD defines thebehavior ofthechip select lineattheendofadata transfer.
0 The chip select signal isdeactivated attheendofatransfer after theT2CDELAY time haspassed.
Iftwoconsecutive transfers arededicated tothesame chip select thischip select signal willbe
deactivated foratleast 2VCLK cycles before itisactivated again.
1 The chip select signal isheld active attheendofatransfer until acontrol field with new data and
control information isloaded intoSPIDAT1. Ifthenew chip select number equals theprevious one,
theactive chip select signal isextended until theendoftransfer with CSHOLD cleared, oruntil the
chip-select number changes.

<!-- Page 1608 -->

Multi-buffer RAM www.ti.com
1608 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-62. Multi-buffer RAM Transmit Data Register (TXRAM) Field Descriptions (continued)
Bit Field Value Description
27 LOCK Lock twoconsecutive buffer words. Donotallow interruption byTG'swith higher priority.
0 Any higher-priority TGcanbegin attheendofthecurrent transaction.
1 Ahigher-priority TGcannot occur until after thenext unlocked buffer word istransferred.
26 WDEL Enable thedelay counter attheendofthecurrent transaction.
Note: TheWDEL bitissupported inmaster mode only. Inslave mode, thisbitwillbe
ignored.
0 Nodelay willbeinserted. However, theSPICS pins willstillbede-activated foratleast for2VCLK
cycles ifCSHOLD =0.
Note: Theduration forwhich theSPICS pinremains deactivated also depends upon thetime
taken tosupply anew word after completing theshift operation (incompatibility mode). If
TXBUF isalready full,then theSPICS pinwillbedeasserted foratleast twoVCLK cycles (if
WDEL =0).
1 After atransaction, WDELAY ofthecorresponding data format willbeloaded intothedelay
counter. Notransaction willbeperformed until theWDELAY counter overflows. The SPICS pins
willbede-activated foratleast (WDELAY +2)×VCLK_Period duration.
25-24 DFSEL Data word format select.
0 Data word format 0isselected.
1h Data word format 1isselected.
2h Data word format 2isselected.
3h Data word format 3isselected.
23-16 CSNR 0-FFh Chip select (CS) number. CSNR defines thechip select pins thatwillbeactivated during thedata
transfer. CSNR isabit-mask thatcontrols allchip select pins. See Table 28-63 .
Note: Ifyour MibSPI hasless than 8chip select pins, allunused upper bitswillbe0.For
example, MiBSPI3 has6chip select pins, ifyouwrite FFhtoCSNR, theactual number
stored inCSNR is3Fh.
15-0 TXDATA 0-7FFFh Transfer data. When written, these bitsarecopied totheshift register ifitisempty. Iftheshift
register isnotempty, then they areheld inTXBUF.
SPIEN must besetto1before thisregister canbewritten to.Writing a0toSPIEN forces thelower
16bitsofTXDATA to0.
Awrite tothisregister (ortotheTXDATA field only) drives thecontents oftheCSNR field onthe
SPICS pins, ifthepins areconfigured asfunctional pins (automatic chip select, see
Section 28.2.1 ).
When thisregister isread, thecontents ofTXBUF, which holds thelatest data written, willbe
returned.
Note: Regardless ofthecharacter length, thetransmit data should beright-justified before
writing totheSPIDAT1 register.

<!-- Page 1609 -->

www.ti.com Multi-buffer RAM
1609 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)(1)Ifyour MibSPI does nothave thischip select pin,thisbitis0.Table 28-63. Chip Select Number Active
CSNR
ValueChip Select Active: CSNR
ValueChip Select Active:
CS[5](1)CS[4](1)CS[3](1)CS[2](1)CS[1](1)CS[0] CS[5](1)CS[4](1)CS[3](1)CS[2](1)CS[1](1)CS[0]
0h Nochip select pinisactive. 20h x
1h x 21h x x
2h x 22h x x
3h x x 23h x x x
4h x 24h x x
5h x x 25h x x x
6h x x 26h x x x
7h x x x 27h x x x x
8h x 28h x x
9h x x 29h x x x
Ah x x 2Ah x x x
Bh x x x 2Bh x x x x
Ch x x 2Ch x x x
Dh x x x 2Dh x x x x
Eh x x x 2Eh x x x x
Fh x x x x 2Fh x x x x x
10h x 30h x x
11h x x 31h x x x
12h x x 32h x x x
13h x x x 33h x x x x
14h x x 34h x x x
15h x x x 35h x x x x
16h x x x 36h x x x x
17h x x x x 37h x x x x x
18h x x 38h x x x
19h x x x 39h x x x x
1Ah x x x 3Ah x x x x
1Bh x x x x 3Bh x x x x x
1Ch x x x 3Ch x x x x
1Dh x x x x 3Dh x x x x x
1Eh x x x x 3Eh x x x x x
1Fh x x x x x 3Fh x x x x x x

<!-- Page 1610 -->

Multi-buffer RAM www.ti.com
1610 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.4.4 Multi-buffer RAM Receive Buffer Register (RXRAM)
Each word ofRXRAM isareceive-buffer register.
Figure 28-89. Multi-buffer RAM Receive Buffer Register (RXRAM)
[offset =RAM Base +200-3FFh]
31 30 29 28 27 26 25 24
RXEMPTY RXOVR TXFULL BITERR DESYNC PARITYERR TIMEOUT DLENERR
RS-1 RC-0 R-0 RC-0 RC-0 RC-0 RC-0 RC-0
23 16
LCSNR
R-0
15 0
RXDATA
R/W-0
LEGEND: R=Read only; R/W =Read/Write; C=Clear; S=Set; -n=value after reset
Table 28-64. Multi-buffer Receive Buffer Register (RXRAM) Field Descriptions
Bit Field Value Description
31 RXEMPTY Receive data buffer empty. When thehost reads theRXDATA field ortheentire RXRAM register,
itautomatically sets theRXEMPTY flag. When adata transfer iscompleted, thereceived data is
copied intoRXDATA, andtheRXEMPTY flagiscleared.
0 New data hasbeen received andcopied intoRXDATA.
1 Nodata hasbeen received since thelastread ofRXDATA.
This flaggets setto1under thefollowing conditions:
*Reading theRXDATA field oftheRXRAM register.
*Writing a1toclear theRXINTFLG bitintheSPIFlag Register (SPIFLG).
Write-clearing theRXINTFLG bitbefore reading RXDATA indicates thereceived data isbeing
ignored. Conversely, RXINTFLG canbecleared byreading theRXDATA field ofRXRAM (orthe
entire register).
30 RXOVR Receive data buffer overrun. When adata transfer iscompleted andthereceived data iscopied
intoRXBUF while itisalready full,RXOVR isset.Overruns always occur toRXBUF, notto
RXRAM; thecontents ofRXRAM areoverwritten only after itisread bythePeripheral (VBUSP)
master (CPU, DMA, orother host processor).
Ifenabled, theRXOVRN interrupt isgenerated when RXBUF isoverwritten, andreading either SPI
Flag Register (SPIFLG) orSPIVECTx shows theRXOVRN condition. Two read operations from
theRXRAM register arerequired toreach theoverwritten buffer word (one toread RXRAM, which
then transfers RXDATA intoRXRAM forthesecond read).
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
Note: Aspecial condition under which RXOVR flaggets set.If both RXRAM andRXBUF are
already fullandwhile another buffer receive isunderway, ifanyerrors such asTIMEOUT,
BITERR andDLEN_ERR occur, then RXOVR inRXBUF andSPIFlag Register (SPIFLG) will
besettoindicate thatthestatus flags aregetting overwritten bythenew transfer. This
overrun should betreated likeanormal receive overrun.
0 Noreceive data overrun condition occurred since lastread ofthedata field.
1 Areceive data overrun condition occurred since lastread ofthedata field.
29 TXFULL Transmit data buffer full.This flagisaread-only flag. Writing intotheSPIDAT0 orSPIDAT1 field
while theTXshift register isfullwillautomatically settheTXFULL flag. Once theword iscopied to
theshift register, theTXFULL flagwillbecleared. Writing toSPIDAT0 orSPIDAT1 when both
TXBUF andtheTXshift register areempty does notsettheTXFULL flag.
0 The transmit buffer isempty; SPIDAT0/SPIDAT1 isready toaccept anew data.
1 The transmit buffer isfull;SPIDAT0/SPIDAT1 isnotready toaccept new data.

<!-- Page 1611 -->

www.ti.com Multi-buffer RAM
1611 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Table 28-64. Multi-buffer Receive Buffer Register (RXRAM) Field Descriptions (continued)
Bit Field Value Description
28 BITERR Biterror. There was amismatch ofinternal transmit data andtransmitted data.
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
0 Nobiterror occurred.
1 Abiterror occurred. The SPIsamples thesignal ofthetransmit pins (master: SIMOx, slave:
SOMIx) atthereceive point (one-half clock cycle after thetransmit point). Ifthesampled value
differs from thetransmitted value, abiterror isdetected andtheBITERR flagisset.Possible
reasons forabiterror include noise, anexcessively high bitrate, capacitive load, oranother
master/slave trying totransmit atthesame time.
27 DESYNC Desynchronization ofslave device. This bitisvalid inmaster mode only.
The master monitors theENA signal coming from theslave device andsets theDESYNC flagif
ENA isdeactivated before thelastreception point orafter thelastbitistransmitted plus tT2EDELAY .
IfDESYNCENA isset,aninterrupt isasserted. Desynchronization canoccur ifaslave device
misses aclock edge coming from themaster.
Note: IntheCompatibility Mode MibSPI, under some circumstances itispossible fora
desync error detected fortheprevious buffer tobevisible inthecurrent buffer. This is
because thereceive completion flag/interrupt isgenerated when thebuffer transfer is
completed. Butdesynchronization isdetected after thebuffer transfer iscompleted. So,if
theVBUS master reads thereceived data quickly when anRXINT isdetected, then the
status flagmay notreflect thecorrect desync condition. Inmulti-buffer mode, thedesync
flagisalways guaranteed tobeforthecurrent buffer.
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
0 Noslave desynchronization isdetected.
1 Aslave device isdesynchronized.
26 PARITYERR Parity error. The calculated parity differs from thereceived parity bit.
Iftheparity generator isenabled (selected individually foreach buffer) aneven oroddparity bitis
added attheendofadata word. During reception ofthedata word, theparity generator calculates
thereference parity andcompares ittothereceived parity bit.Ifamismatch isdetected, the
PARITYERR flagisset.
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
0 Noparity error isdetected.
1 Aparity error occurred.
25 TIMEOUT Time-out because ofnon-activation ofSPIENA pin.
The SPIgenerates atime-out when theslave does notrespond intime byactivating theENA
signal after thechip select signal hasbeen activated. Ifatime-out condition isdetected, the
corresponding chip select isdeactivated immediately andtheTIMEOUT flagisset.Inaddition, the
TIMEOUT flaginthestatus field ofthecorresponding buffer andintheSPIFlag Register
(SPIFLG) isset.
This bitisvalid only inmaster mode.
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
0 NoSPIENA pintime-out occurred.
1 AnSPIENA signal time-out occurred.
24 DLENERR Data length error flag.
Note: This flagiscleared to0when theRXDATA field oftheRXRAM register isread.
0 Nodata-length error occurred.
1 Adata length error occurred.
23-16 LCSNR 0-FFh Last chip select number. LCSNR inthestatus field isacopy ofCSNR inthecorresponding control
field. Itcontains thechip select number thatwas activated during thelastword transfer.
15-0 RXDATA 0-FFFFh SPIreceive data. This isthereceived word, transferred from thereceive shift-register attheendof
atransfer. Regardless oftheprogrammed character length andthedirection ofshifting, the
received data isstored right-justified intheregister.

<!-- Page 1612 -->

Parity\ECC Memory www.ti.com
1612 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.5 Parity\ECC Memory
Parity/ECC portion ofmulti-buffer RAM isnotaccessible bytheCPU during normal operating modes.
However each read orwrite operation totheControl/Data/Status portion ofthemulti-buffer RAM causes
reads/writes totheparity/ECC portion aswell.
*Each write tothemulti-buffer RAM (either from theVBUS interface orbytheMibSPI itself) causes a
write operation totheParity/ECC portion ofRAM simultaneously toupdate theequivalent parity/ECC
bits.
*Each read operation from themulti-buffer RAM (either from theVBUS interface orbytheMibSPI itself)
causes aread operation from theParity/ECC portion oftheRAM forparity/ECC comparison purpose.
*Reads/Writes tomulti-buffer RAM could either becaused byanyCPU/DMA accesses orbythe
Sequencer logic ofMibSPI itself.
Fortesting theParity/ECC portion ofthemulti-buffer RAM thatisa4-bit or7-bit field perword address, a
separate parity/ECC memory testmode isavailable. The parity memory testmode canbeenabled and
disabled bythePTESTEN bitinPAR_ECC_CTRL register andtheECCDIAG_EN bitinECCDIAG_CTRL
register.
During theparity testmode, theparity locations areaddressable attheaddress between
RAM_BASE_ADDR +0x400h andRAM_BASE_ADDR +0x7FFh. Each location corresponds,
sequentially, toeach TXRAM word, then toeach RXRAM word. See Figure 28-90 foradiagram ofthe
memory map ofparity memory during normal operating mode andduring parity testmode while
EXTENDED_BUF mode isdisabled orthefeature isnotimplemented. See Figure 28-91 foradiagram of
thememory map ofparity memory during normal operating mode andduring parity testmode while
EXTENDED_BUF mode isenabled.
During Parity/ECC testmode, after writing theData/Control portion oftheRAM, theParity/ECC locations
canbewritten with wrong parity/ECC bitstointentionally cause Parity/ECC Errors.
See thedevice-specific data sheet togettheactual base address ofthemulti-buffer RAM.
NOTE: The RX_RAM_ACCESS bitcanalso besetto1during theParity/ECC Test mode tobeable
towrite toRXRAM locations fortestpurpose. Both Parity/ECC bitstesting andRXRAM
testing canbedone together.
There are4bitsofparity corresponding toeach ofthe32-bit multi-buffer locations. Individual bitsinthe
parity memory arebyte-addressable inparity testmode. See theexample inSection 28.5.1 forfurther
details.
IfECC isenabled, there are7ECC-bits corresponding toeach ofthe32-bit multi-buffer locations. See the
example inSection 28.5.1 forfurther details.
NOTE: Polarity oftheparity (odd/even) varies bydevice. Insome devices, acontrol register inthe
system module canbeused toselect oddoreven parity.

<!-- Page 1613 -->

.
.
.TXBUF0
TXBUF1
TXBUF126
TXBUF127Parity/ECC0
Parity/ECC10 31Address
BASE+0x000h
BASE+0x1FFh
BASE+0x200h
BASE+0x3FFh.
.
.
.
.
.TXParity/ECC0
TXParity/ECC1
TXParity/ECC126
TXParity/ECC127
RXParity/ECC0
RXParity/ECC1
RXParity/ECC126
RXParity/ECC1270 31
Multibuffer RAMAddress
BASE+0x400h
BASE+0x5FFh
BASE+0x600h
BASE+0x7FFh
Memory organization during Normal Operation
BASE - Base Address of Multibuffer RAM
Refer to specific Device Datasheet
for the actual value of BASE.*.
.
Parity/ECC127Parity/ECC126
.
.
.RXBUF0
RXBUF1
RXBUF126
RXBUF1270 31
(Parity/ECC locations are not accessible by CPU)Parity/ECC0
Parity/ECC1
.
.
Parity/ECC127Parity/ECC126
Parity/ECC memory organization during Test Mode
www.ti.com Parity\ECC Memory
1613 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-90. Memory-Map forParity Locations During Normal andTest Mode While EXTENDED_BUF
Mode isDisabled ortheFeature isNotImplemented

<!-- Page 1614 -->

.
.
.TXBUF0
TXBUF1
TXBUF254
TXBUF255Parity/ECC0
Parity/ECC10 31Address
BASE+0x000h
BASE+0x3FFh
BASE+0x400h
BASE+0x7FFh.
.
.
.
.
.TXParity/ECC0
TXParity/ECC1
TXParity/ECC254
TXParity/ECC255
RXParity/ECC0
RXParity/ECC1
RXParity/ECC254
RXParity/ECC2550 31
Multibuffer RAMAddress
BASE+0x800h
BASE+0xBFFh
BASE+0xC00h
BASE+0xFFFh
Memory organization during Normal Operation
BASE - Base Address of Multibuffer RAM
Refer to specific Device Datasheet
for the actual value of BASE.*.
.
Parity/ECC255Parity/ECC254
.
.
.RXBUF0
RXBUF1
RXBUF254
RXBUF2550 31
(Parity/ECC locations are not accessible by CPU)Parity/ECC0
Parity/ECC1
.
.
Parity/ECC255Parity/ECC254
Parity/ECC memory organization during Test Mode
Parity\ECC Memory www.ti.com
1614 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-91. Memory-Map forParity Locations During Normal andTest Mode While EXTENDED_BUF
Mode isEnabled

<!-- Page 1615 -->

A001AA55
TXBUF50 31
PARITY 53 0
BASE+014h
Memory Organization During Normal Mode
000000031
PARITY 5TXBUF5A001AA5531 0
1 0000000 0000000 0000000 1 1 00 8 16 241 10 1
BASE+014h
BASE+ 400h + 014h
Parity Memory locations during Test Mode (Memory Mapped)
* Shaded areas indicate reads return "0", writes have no effe ct.
* Shaded areas also indicate that they're not physically pre sent
www.ti.com Parity\ECC Memory
1615 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.5.1 Example ofParity Memory Organization
Suppose TXBUF5 (6th location inTXRAM) inthemulti-buffer RAM iswritten with avalue ofA001_AA55.
Ifthepolarity oftheparity issettoodd, thecorresponding parity location parity5 willgetupdated with
equivalent parity of1011 initsfield.
During parity-memory testmode, these bitscanbeindividually byte addressed. The return data willbea
byte adjusted with actual parity bitintheLSB ofthebyte. Ifaword isread from theword-boundary
address ofparity locations, then each bitofthe4-bit parity isbyte-adjusted anda32-bit word isreturned.
0swillbepadded intotheparity bitstogeteach byte. See Figure 28-92 foradiagram.
Figure 28-92. Example ofMemory-Mapped Parity Locations During Test Mode
NOTE: Read Access toParity Memory Locations
Parity memory locations canberead even without entering intoparity memory testmode.
Their address remains asinmemory testmode. Itisonly toenter parity-memory testmode
toenable write access totheparity memory locations.

<!-- Page 1616 -->

ECC bits Data bits
TXBUF50 6
BASE+014h
Memory Organization During Normal Mode
0000000031
00000000 ECC BASE+ 400h + 014h 000000000
ECC-bits Organization During Test Mode24 23 1615 7 6 0031
Parity\ECC Memory www.ti.com
1616 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.5.2 Example ofECC Memory Organization
Suppose TXBUF5 (6th location inTXRAM portion) inthemulti-buffer RAM iswritten with avalue of
A001_AA55, then thecorresponding ECC-bits willbeupdated inECC location.
The ECC bitscanbeaccessed byuser, when Memory Test mode isenabled andadditionally diagnostic
mode isalso enabled. The actual ECC bitswillbealigned asshown inFigure 28-93 .
Figure 28-93. Example ofECC BitLocations During Test Mode
NOTE: Access toECC locations
ECC locations canberead/write only when Parity Memory Test mode anddiagnostic mode
isenabled

<!-- Page 1617 -->

VCLK
SPICS
SPICLK
SPISIMO
SPISOMI
* Dotted vertical lines indicate the receive edgesWrite to SPIDAT
VCLK
Write to SPIDAT
SPICLK
SPISIMO
SPISOMI
* Dotted vertical lines indicate the receive edges
www.ti.com MibSPI PinTiming Parameters
1617 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.6 MibSPI PinTiming Parameters
The pintimings ofSPIcanbeclassified based onitsmode ofoperation. Ineach mode, different
configurations likePhase &Polarity affect thepintimings.
The pindirections arebased onthemode ofoperation.
Master mode SPI:
*SPICLK (SPI Clock) -Output
*SPISIMO (SPI Slave InMaster Out) -Output
*SPICS (SPI Slave Chip Selects) -Output
*SPISOMI (SPI Slave OutMaster In)-Input
*SPIENA (SPI slave ready Enable) -Input
Slave mode SPI:
*SPICLK -Input
*SPISIMO -Input
*SPICS -Input
*SPISOMI -Output
*SPIENA -Output
NOTE: Allthefollowing timing diagrams arewith Phase =0andPolarity =0,unless explicitly stated
otherwise.
28.6.1 Master Mode Timings forSPI/MibSPI
Figure 28-94. SPI/MibSPI Pins During Master Mode 3-Pin Configuration
Figure 28-95. SPI/MibSPI Pins During Master Mode 4-Pin with SPICS Configuation

<!-- Page 1618 -->

* Dotted vertical lines indicate the receive edges for the Master
* ENABLE_HIGHZ is cleared to 0 in Slave SPIVCLK
SPICLK
SPIENASPICS
SPISIMO
SPISOMIMaster
SlaveWrite to SPIDAT
Write to SPIDAT
* De-activation of SPIENA pin is controlled by the Slave.SPISOMISPISIMOSPICLKSPIENAVCLK
* Dotted vertical lines indicate the receive edgesWrite to SPIDAT
MibSPI PinTiming Parameters www.ti.com
1618 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)Figure 28-96. SPI/MibSPI Pins During Master Mode in4-Pin with SPIENA Configuration
Figure 28-97. SPI/MibSPI Pins During Master/Slave Mode with 5-Pin Configuration

<!-- Page 1619 -->

SPICS
* ENABLE_HIGHZ is set to 1 in Slave SPIVCLK
SPIENA
SPICLK
* Diagram shows relationship between the SPICS from a Master to SPIENA from Slave SPI when SPIENA
is configured in High-Impedance modeWrite to SPIDAT
SPICS
* ENABLE_HIGHZ is cleared to 0 in Slave SPIVCLK
SPIENA
SPICLK
* Diagram shows relationship between the SPICS from a Master to SPIENA from Slave SPI when SPIENA
is configured in Push-Pull modeWrite to SPIDAT
VCLKSPIENA
* Diagram shows a relationship between the SPIENA from Slave and SPICLK from MasterWrite to SPIDAT
SPICLK
VCLK
SPICLK
SPISOMI
SPISIMO
* Dotted vertical lines indicate the receive edgesWrite to SPIDAT
www.ti.com MibSPI PinTiming Parameters
1619 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.6.2 Slave Mode Timings forSPI/MibSPI
Figure 28-98. SPI/MibSPI Pins During Slave Mode 3-Pin Configuration
Figure 28-99. SPI/MibSPI Pins During Slave Mode in4-Pin with SPIENA Configuration
Figure 28-100. SPI/MibSPI Pins During Slave Mode in5-Pin Configuration (Single Slave)
Figure 28-101. SPI/MibSPI Pins During Slave Mode in5-Pin Configuration (Single/Multi-Slave)

<!-- Page 1620 -->

MibSPI PinTiming Parameters www.ti.com
1620 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedMulti-Buffered Serial Peripheral Interface Module (MibSPI) with Parallel Pin
Option (MibSPIP)28.6.3 Master Mode Timing Parameter Details
Incase ofMaster, themodule drives outSPICLK. Italso drives outtheTransmit data onSPISIMO with
respect toitsinternal SPICLK. Incase ofMaster mode, theRXdata ontheSPISOMI pinisregistered with
respect toSPICLK received through theinput buffer from theI/Opad.
Ifthechip select pinisfunctional, then theMaster willdrive outtheSPICS pins before starting the
SPICLK. IftheSPIENA pinisfunctional, then theMaster willwait foranactive lowfrom theSlave onthe
input pintostart theSPICLK.
28.6.4 Slave Mode Timing Parameter Details
Incase ofSlave mode, themodule willdrive only theSPISOMI andSPIENA pins. Allother pins areinputs
toit.The RXdata ontheSPISIMO pinwillberegistered with respect totheSPICLK pin.The Slave will
usetheSPICS pintodrive outtheSPIENA pinifboth arefunctional. If4-pin with SPIENA isconfigured,
then theSlave willdrive outanactive-low signal ontheSPIENA pinwhen new data iswritten totheTX
Shift Register. Irrespective of4-pin with SPIENA or5-pin configuration, theSlave willdeassert the
SPIENA pinafter thelastbitisreceived. IfENABLE_HIGHZ (SPIINT0.24) bitis0,thede-asserted value of
theSPIENA pinwillbe1.Otherwise, itwilldepend upon theinternal pulluporpulldown resistor (if
implemented) depending upon theSpecification oftheChip.