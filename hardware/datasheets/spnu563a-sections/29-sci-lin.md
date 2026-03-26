# Serial Communication Interface (SCI) / LIN Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1621-1716

---


<!-- Page 1621 -->

1621 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleChapter 29
SPNU563A -March 2018
Serial Communication Interface (SCI)/
Local Interconnect Network (LIN) Module
This chapter describes theserial communication interface (SCI) /local interconnect network (LIN) module.
The SCI/LIN iscompliant totheLIN2.1protocol specified intheLINSpecification Package .This module
canbeconfigured tooperate ineither SCI(UART) orLINmode.
NOTE: This chapter describes asuperset implementation oftheLIN/SCI module thatincludes
features andfunctionality thatrequire DMA. Since notalldevices have DMA capability,
consult your device-specific datasheet todetermine applicability ofthese features and
functions toyour device being used.
Topic ........................................................................................................................... Page
29.1 Introduction andFeatures ................................................................................ 1622
29.2 SCI................................................................................................................ 1627
29.3 LIN................................................................................................................ 1642
29.4 Low-Power Mode ............................................................................................ 1662
29.5 Emulation Mode .............................................................................................. 1664
29.6 GPIO Functionality .......................................................................................... 1665
29.7 SCI/LIN Control Registers ................................................................................ 1667

<!-- Page 1622 -->

Introduction andFeatures www.ti.com
1622 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.1 Introduction andFeatures
The SCI/LIN module canbeprogrammed towork either asanSCIorasaLIN. The core ofthemodule is
anSCI. The SCI'shardware features areaugmented toachieve LINcompatibility.
The SCImodule isauniversal asynchronous receiver-transmitter thatimplements thestandard nonreturn
tozero format. The SCIcanbeused tocommunicate, forexample, through anRS-232 port orover aK-
line.
The LINstandard isbased ontheSCI(UART) serial data linkformat. The communication concept is
single-master/multiple-slave with amessage identification formulti-cast transmission between anynetwork
nodes.
Throughout thechapter Compatibility Mode refers toSCIMode functionary ofSCI/LIN Module.
Section 29.2 explains about theSCIfunctionality andSection 29.3 explains about theLINfunctionality.
Though theregisters arecommon forLINandSCI, theregister descriptions hasnotes toidentify the
register/bit usage indifferent modes.
29.1.1 SCIFeatures
The following arethefeatures oftheSCImodule:
*Standard universal asynchronous receiver-transmitter (UART) communication
*Supports full-orhalf-duplex operation
*Standard nonreturn tozero (NRZ) format
*Double-buffered receive andtransmit functions incompatibility mode
*Supports twoindividually enabled interrupt lines: level 0andlevel 1
*Configurable frame format of3to13bitspercharacter based onthefollowing:
-Data word length programmable from onetoeight bits
-Additional address bitinaddress-bit mode
-Parity programmable forzero oroneparity bit,oddoreven parity
-Stop programmable foroneortwostop bits
*Asynchronous orisosynchronous communication modes
*Two multiprocessor communication formats allow communication between more than twodevices
*Sleep mode isavailable tofree CPU resources during multiprocessor communication andthen wake
uptoreceive anincoming message
*The 24-bit programmable baud rate supports 224different baud rates provide high accuracy baud rate
selection
*At100-MHz peripheral clock, 3.125 Mbits/s istheMax Baud Rate achievable
*Capability touseDirect Memory Access (DMA) fortransmit andreceive data
*Five error flags andSeven status flags provide detailed information regarding SCIevents
*Two external pins: LINRX andLINTX
*Multi-buffered receive andtransmit units
NOTE: SCI/LIN module does notsupport UART hardware flow control. This feature canbe
implemented insoftware using ageneral purpose I/Opin.

<!-- Page 1623 -->

www.ti.com Introduction andFeatures
1623 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.1.2 LINFeatures
The following arethefeatures oftheLINmodule:
*Compatibility with LIN1.3,2.0,and2.1protocols
*Configurable Baud Rate upto20Kbits/s
*Two external pins: LINRX andLINTX.
*Multi-buffered receive andtransmit units
*Identification masks formessage filtering
*Automatic master header generation
-Programmable synchronization break field
-Synchronization field
-Identifier field
*Slave automatic synchronization
-Synchronization break detection
-Optional baud rate update
-Synchronization validation
*231programmable transmission rates with 7fractional bits
*Wakeup onLINRX dominant level from transceiver
*Automatic wakeup support
-Wakeup signal generation
-Expiration times onwakeup signals
*Automatic busidledetection
*Error detection
-Biterror
-Bus error
-No-response error
-Checksum error
-Synchronization field error
-Parity error
*Capability touseDirect Memory Access (DMA) fortransmit andreceive data.
*2Interrupt lines with priority encoding for:
-Receive
-Transmit
-ID,error, andstatus
*Support forLIN2.0checksum
*Enhanced synchronizer finite state machine (FSM) support forframe processing
*Enhanced handling ofextended frames
*Enhanced baud rate generator
*Update wakeup/go tosleep

<!-- Page 1624 -->

Introduction andFeatures www.ti.com
1624 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.1.3 Block Diagram
The SCI/LIN module contains core SCIblock with added sub-blocks tosupport LINprotocol.
Three Major components oftheSCIModule are:
*Transmitter
*Baud Clock Generator
*Receiver
Transmitter (TX) contains twomajor registers toperform thedouble- buffering:
*The transmitter data buffer register (SCITD) contains data loaded bytheCPU tobetransferred tothe
shift register fortransmission.
*The transmitter shift register (SCITXSHF) loads data from thedata buffer (SCITD) andshifts data onto
theLINTX pin,onebitatatime.
Baud Clock Generator
*Aprogrammable baud generator produces either abaud clock scaled from VBUSP CLK.
Receiver (RX) contains twomajor registers toperform thedouble- buffering:
*The receiver shift register (SCIRXSHF) shifts data infrom theLINRX pinonebitatatime andtransfers
completed data intothereceive data buffer.
*The receiver data buffer register (SCIRD) contains received data transferred from thereceiver shift
register
The SCIreceiver andtransmitter aredouble-buffered, andeach hasitsown separate enable andinterrupt
bits. The receiver andtransmitter may each beoperated independently orsimultaneously infullduplex
mode.
Toensure data integrity, theSCIchecks thedata itreceives forbreaks, parity, overrun, andframing
errors. The bitrate (baud) isprogrammable toover 16million different rates through a24-bit baud-select
register. Figure 29-1 shows thedetailed SCIblock diagram.
The SCI/LIN module isbased onthestandalone SCIwith theaddition ofanerror detector (parity
calculator, checksum calculator, andbitmonitor), amask filter, asynchronizer, andamulti-buffered
receiver andtransmitter. The SCIinterface, theDMA control subblocks andthebaud generator are
modified aspart ofthehardware enhancements forLINcompatibility. Figure 29-2 shows theSCI/LIN
block diagram.

<!-- Page 1625 -->

TXRDYTX EMPTY
SCIFLR.8SCIFLR.11
TX INT ENATX INT
TXWAKEAddress bit†Shift register
Transmit buffer18
LINRX
SCIGCR1.24
8
Receive buffer
SCIRDRXRDYBRKDT
SCIFLR.9SCIFLR.0
RX INT ENARXWAKE
SCIFLR.12RX INTShift register
WAKEUP
SCIFLR.1SCITDSCITXSHF
SCIRXSHF
ERR INT
SCIFLR24:26SCIGCR1.25TXENA
SCIFLR.10
BRKDT INT ENA
WAKEUP INT ENA
PEOE  FERECEIVERTRANSMITTER
CLOCKBaud clock
SCIBAUDgenerator
Baud rate
registersLINTX
RXENA
SCISETINT.9SCISETINT.0
SCISETINT.1SCIGCR1.5SCISETINT.8
SCIVCLK
Peripheral
www.ti.com Introduction andFeatures
1625 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-1. SCIBlock Diagram

<!-- Page 1626 -->

Introduction andFeatures www.ti.com
1626 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-2. SCI/LIN Block Diagram

<!-- Page 1627 -->

Start0
(LSBit)1 2 3 4 5 67
(MSBit)Addr ParityStart0
(LSBit)1 2 3 4 5 67
(MSBit)ParityIdle-line mode
Address bit mode
Address bitStop
Stop
www.ti.com SCI
1627 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2 SCI
29.2.1 SCICommunication Formats
The SCImodule canbeconfigured tomeet therequirements ofmany applications. Because
communication formats vary depending onthespecific application, many attributes oftheSCI/LIN areuser
configurable. The listbelow describes these configuration options:
*SCIFrame format
*SCITiming modes
*SCIBaud rate
*SCIMultiprocessor modes
29.2.1.1 SCIFrame Formats
The SCIuses aprogrammable frame format. Allframes consist ofthefollowing:
*One start bit
*One toeight data bits
*Zero oroneaddress bit
*Zero oroneparity bit
*One ortwostop bits
The frame format forboth thetransmitter andreceiver isprogrammable through thebitsintheSCIGCR1
register. Both receive andtransmit data isinnonreturn tozero (NRZ) format, which means thatthe
transmit andreceive lines areatlogic high when idle. Each frame transmission begins with astart bit,in
which thetransmitter pulls theSCIlinelow(logic low). Following thestart bit,theframe data issent and
received least significant bitfirst(LSB).
Anaddress bitispresent ineach frame iftheSCIisconfigured tobeinaddress-bit mode butisnot
present inanyframe iftheSCIisconfigured foridle-line mode. The format offrames with andwithout the
address bitisillustrated inFigure 29-3.
Aparity bitispresent inevery frame when thePARITY ENA bitisset.The value oftheparity bitdepends
onthenumber ofonebitsintheframe andwhether oddoreven parity hasbeen selected viathePARITY
ENA bit.Both examples inFigure 29-3 have parity enabled.
Allframes include onestop bit,which isalways ahigh level. This high level attheendofeach frame is
used toindicate theendofaframe toensure synchronization between communicating devices. Two stop
bitsaretransmitted iftheSTOP bitinSCIGCR1 register isset.The examples shown inFigure 29-3 use
onestop bitperframe.
Figure 29-3. Typical SCIData Frame Formats

<!-- Page 1628 -->

16 SCI baud clock periods/bitMajority
vote
LSB of data Start bitLINRXFalling edge
detected
1  2  3  4  5  6  7  8  9 10 11 1213 14 15 16 1 2 3 4 5 6  7  8  9 10 11 12 13 14 15 161 2 3 4 5
SCI www.ti.com
1628 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.1.2 SCITiming Mode
The SCIcanbeconfigured touseasynchronous orisosynchronous timing using TIMING MODE bitin
SCIGCR1 register.
29.2.1.2.1 Asynchronous Timing Mode
The asynchronous timing mode uses only thereceive andtransmit data lines tointerface with devices
using thestandard universal asynchronous receiver- transmitter (UART) protocol.
Intheasynchronous timing mode, each bitinaframe hasaduration of16SCIbaud clock periods. Each
bittherefore consists of16samples (one foreach clock period). When theSCIisusing asynchronous
mode, thebaud rates ofallcommunicating devices must match asclosely aspossible. Receive errors
result from devices communicating atdifferent baud rates.
With thereceiver intheasynchronous timing mode, theSCIdetects avalid start bitifthefirstfour samples
after afalling edge ontheLINRX pinareoflogic level 0.Assoon asafalling edge isdetected onLINRX,
theSCIassumes thataframe isbeing received andsynchronizes itself tothebus.
Toprevent interpreting noise asStart bitSCIexpects LINRX linetobelowforatleast four contiguous SCI
baud clock periods todetect avalid start bit.The busisconsidered idleifthiscondition isnotmet. When a
valid start bitisdetected, theSCIdetermines thevalue ofeach bitbysampling theLINRX linevalue
during theseventh, eighth, andninth SCIbaud clock periods. Amajority vote ofthese three samples is
used todetermine thevalue stored intheSCIreceiver shift register. Bysampling inthemiddle ofthebit,
theSCIreduces errors caused bypropagation delays andriseandfalltimes anddata linenoises.
Figure 29-4 illustrates how thereceiver samples astart bitandadata bitinasynchronous timing mode.
The transmitter transmits each bitforaduration of16SCIbaud clock periods. During thefirstclock period
forabit,thetransmitter shifts thevalue ofthatbitonto theLINTX pin.The transmitter then holds the
current bitvalue onLINTX for16SCIbaud clock periods.
Figure 29-4. Asynchronous Communication BitTiming
29.2.1.2.2 Isosynchronous Timing Mode
Inisosynchronous timing mode, each bitinaframe hasaduration ofexactly 1baud clock period and
therefore consists ofasingle sample. With thistiming configuration, thetransmitter andreceiver are
required tomake useoftheSCICLK pintosynchronize communication with other SCI. This mode isnot
supported onthisdevice because SCICLK pinisnotavailable.
29.2.1.3 SCIBaud Rate
The SCI/LIN hasaninternally generated serial clock determined bytheperipheral VCLK andthe
prescalers PandMinthisregister. The SCIuses the24-bit integer prescaler Pvalue intheBRS register
toselect therequired baud rates. The additional 4-bit fractional divider Mrefines thebaud rate selection.

<!-- Page 1629 -->

V C L KaTdMP bitT/c250/c251/c249
/c234/c235/c233/c43/c247
/c248/c246/c231
/c232/c230/c43/c43 /c61161 16
VCLKiTdMP bitT/c250/c251/c249
/c234/c235/c233/c43/c247
/c248/c246/c231
/c232/c230/c43/c43 /c61161 16
SCICLK Frequency =
Asynchronous baud value =
For P = 0,
Asynchronous baud value =VCLK Frequency
P 1M
16- - - - - - + +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SCICLK Frequency
16- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
VCLK Frequency
32- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
www.ti.com SCI
1629 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleInasynchronous timing mode, theSCIgenerates abaud clock according tothefollowing formula:
(42)
29.2.1.3.1 Superfractional Divider, SCIAsynchronous Mode
The superfractional divider isavailable inSCIasynchronous mode (idle-line andaddress-bit mode).
Building onthe4-bit fractional divider M(BRS[27:24]), thesuperfractional divider uses anadditional 3-bit
modulating value (see Table 29-2).The bitswith a1inthetable willhave anadditional VCLK period
added totheir Tbit.Ifthecharacter length ismore than 10,then themodulation table willbearolled-over
version oftheoriginal table (Table 29-1),asshown inTable 29-2.
The baud rate willvary over adata field toaverage according totheBRS[30:28] value bya"d"fraction of
theperipheral internal clock: 0<d<1.See Figure 29-5 forasimple Average "d'calculation based on"U"
value (BRS[30:28]).
The instantaneous bittime isexpressed interms ofTVCLKasfollows:
ForallPother than 0,andallMandd(0or1),
(43)
ForP=0Tbit=32TVCLK
The averaged bittime isexpressed interms ofTVCLKasfollows:
ForallPother than 0,andallMandd(0<d<1),
(44)
ForP=0Tbit=32TVCLK
(1)Normal configuration =Start +8Data Bits+Stop BitTable 29-1. Superfractional BitModulation forSCIMode (Normal Configuration)(1)
BRS[30:28] Start Bit D[0] D[1] D[2] D[3] D[4] D[5] D[6] D[7] Stop Bit
0h 0 0 0 0 0 0 0 0 0 0
1h 1 0 0 0 0 0 0 0 1 0
2h 1 0 0 0 1 0 0 0 1 0
3h 1 0 1 0 1 0 0 0 1 0
4h 1 0 1 0 1 0 1 0 1 0
5h 1 1 1 0 1 0 1 0 1 1
6h 1 1 1 0 1 1 1 0 1 1
7h 1 1 1 1 1 1 1 0 1 1

<!-- Page 1630 -->

ST 0 1 2 3 4 5 6 7 STOPNormal Data Frame with BRS[31:28] = 0
Normal Data Frame with BRS[31:28] = 1ST 0 1 2 3 4 5 6 7 STOP
TbitTbit
Tvclk+d = Number of Vclk Added / Total Number of Bits
= 2 / 10 = 0.2
SCI www.ti.com
1630 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module(1)Maximum configuration =Start +8Data Bits+Addr Bit+Parity Bit+Stop Bit0+Stop Bit1Table 29-2. Superfractional BitModulation forSCIMode (Maximum Configuration)(1)
BRS[30:28]Start
Bit D[0] D[1] D[2] D[3] D[4] D[5] D[6] D[7] Addr Parity Stop0 Stop1
0h 0 0 0 0 0 0 0 0 0 0 0 0 0
1h 1 0 0 0 0 0 0 0 1 0 0 0 0
2h 1 0 0 0 1 0 0 0 1 0 0 0 1
3h 1 0 1 0 1 0 0 0 1 0 1 0 1
4h 1 0 1 0 1 0 1 0 1 0 1 0 1
5h 1 1 1 0 1 0 1 0 1 1 1 0 1
6h 1 1 1 0 1 1 1 0 1 1 1 0 1
7h 1 1 1 1 1 1 1 0 1 1 1 1 1
(1)Minimum configuration =Start +1Data Bits+Stop BitTable 29-3. SCIMode (Minimum Configuration)(1)
BRS[30:28] Start Bit D[0] Stop
0h 0 0 0
1h 1 0 0
2h 1 0 0
3h 1 0 1
4h 1 0 1
5h 1 1 1
6h 1 1 1
7h 1 1 1
Figure 29-5. Superfractional Divider Example

<!-- Page 1631 -->

www.ti.com SCI
1631 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.1.4 SCIMultiprocessor Communication Modes
Insome applications, theSCImay beconnected tomore than oneserial communication device. Insuch a
multiprocessor configuration, several frames ofdata may besent toallconnected devices ortoan
individual device. Inthecase ofdata sent toanindividual device, thereceiving devices must determine
when they arebeing addressed. When amessage isnotintended forthem, thedevices canignore the
following data. When only twodevices make uptheSCInetwork, addressing isnotneeded, so
multiprocessor communication schemes arenotrequired.
SCIsupports twomultiprocessor communication modes which canbeselected using COMM MODE bit:
*Idle-Line Mode
*Address BitMode
When theSCIisnotused inamultiprocessor environment, software canconsider allframes asdata
frames. Inthiscase, theonly distinction between theidle-line andaddress-bit modes isthepresence ofan
extra bit(the address bit)ineach frame sent with theaddress-bit protocol.
The SCIallows full-duplex communication where data canbesent andreceived viathetransmit and
receive pins simultaneously. However, theprotocol used bytheSCIassumes thatonly onedevice
transmits data onthesame buslineatanyonetime. Noarbitration isdone bytheSCI.
29.2.1.4.1 Idle-Line Multiprocessor Modes
Inidle-line multiprocessor mode, aframe thatispreceded byanidleperiod (10ormore idlebits) isan
address frame. Aframe thatispreceded byfewer than 10idlebitsisadata frame. Figure 29-6 illustrates
theformat ofseveral blocks andframes with idle-line mode.
There aretwoways totransmit anaddress frame using idle-line mode:
Method 1:Insoftware, deliberately leave anidleperiod between thetransmission ofthelastdata frame of
theprevious block andtheaddress frame ofthenew block.
Method 2:Configure theSCItoautomatically send anidleperiod between thelastdata frame ofthe
previous block andtheaddress frame ofthenew block.
Although Method 1isonly accomplished byadelay loop insoftware, Method 2canbeimplemented by
using thetransmit buffer andtheTXWAKE bitinthefollowing manner:
Step 1:Write a1totheTXWAKE bit.
Step 2:Write adummy data value totheSCITD register. This triggers theSCItobegin theidleperiod as
soon asthetransmitter shift register isempty.
Step 3:Wait fortheSCItoclear theTXWAKE flag.
Step 4:Write theaddress value toSCITD.
Asindicated byStep 3,software should wait fortheSCItoclear theTXWAKE bit.However, theSCI
clears theTXWAKE bitatthesame time itsets TXRDY (that is,transfers data from SCITD into
SCITXSHF). Therefore, iftheTXINTENA bitisset,thetransfer ofdata from SCITD toSCITXSHF causes
aninterrupt tobegenerated atthesame time thattheSCIclears theTXWAKE bit.Ifthisinterrupt method
isused, software isnotrequired topolltheTXWAKE bitwaiting fortheSCItoclear it.
When idle-line multiprocessor communications areused, software must ensure thattheidletime exceeds
10bitperiods before addresses (using oneofthemethods mentioned above), andsoftware must also
ensure thatdata frames arewritten tothetransmitter quickly enough tobesent without adelay of10bit
periods between frames. Failure tocomply with these conditions willresult indata interpretation errors by
other devices receiving thetransmission.

<!-- Page 1632 -->

Addr Data Addr Start Start
Address frame
(address bit = 1)Data frame
(address bit = 0)Idle time
is of no
significance
One blockSeveral blocks of frames
Data format
(pins LINRX,
LINTX)
Data format
expandedStart 1 0 1Idle time is not significant
ParityStop ParityStopParityStop
Idle time
is of no
significanceAddress frame
(address bit = 1)
Address Data Last data Start Start Start
Address frame Data frame Fewer than
10 idle bitsIdle periodOne block of framesBlocks separated by 10 or more idle bitsBlocks of frames
Data format
(pins LINRX,
LINTX)
Data format
expanded
Stop Stop Stop
Data frameParity Parity Parity
SCI www.ti.com
1632 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-6. Idle-Line Multiprocessor Communication Format
29.2.1.4.2 Address-Bit Multiprocessor Mode
Intheaddress-bit protocol, each frame hasanextra bitimmediately following thedata field called an
address bit.Aframe with theaddress bitsetto1isanaddress frame; aframe with theaddress bitsetto0
isadata frame. The idleperiod timing isirrelevant inthismode. Figure 29-7 illustrates theformat of
several blocks andframes with theaddress-bit mode.
When address-bit mode isused, thevalue oftheTXWAKE bitisthevalue sent astheaddress bit.To
send anaddress frame, software must settheTXWAKE bit.This bitiscleared asthecontents ofthe
SCITD areshifted from theTXWAKE register sothatallframes sent aredata except when theTXWAKE
bitiswritten asa1.
Nodummy write toSCITD isrequired before anaddress frame issent inaddress-bit mode. The firstbyte
written toSCITD after theTXWAKE bitiswritten to1istransmitted with theaddress bitsetwhen address-
bitmode isused.
Figure 29-7. Address-Bit Multiprocessor Communication Format

<!-- Page 1633 -->

RD0
RD1
RD2
RD3
RD4
RD5
RD6
RD70 7
0 7
0 7
0 7
0 7
0 7
0 7
0 73-bit
CounterCE Flag
3-bit
Compare
=RX DMA RequestCHECKSUM
CALCULATOR
MBUF MODERX Ready Flag
Not
MBUF MODENo
Receive
ErrorsSCIRXSHF0 7 RX
www.ti.com SCI
1633 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.1.5 SCIMulti Buffered Mode
Toreduce CPU load when Receiving orTransmitting data ininterrupt mode orDMA mode, theSCI/LIN
module haseight separate Receive andtransmit buffers. Multi buffered mode isenabled bysetting the
MBUF MODE bit.
The multi-buffer 3-bit counter counts thedata bytes transferred from theSCIRXSHF register totheRDy
receive buffers andTDy transmit buffers register toSCITXSHF register. The 3-bit compare register
contains thenumber ofdata bytes expected tobereceived ortransmitted. theLENGTH value in
SCIFORMAT register indicates theexpected length andisused toload the3-bit compare register.
Areceive interrupt (RX interrupt; seetheSCIINTVECT0 andSCIINTVECT1registers), andareceive ready
RXRDY flagsetinSCIFLR register, aswell asaDMA request (RXDMA) could occur after receiving a
response ifthere arenoresponse receive errors fortheframe (such as,there is,frame error, andoverrun
error).
Atransmit interrupt (TXinterrupt), andatransmit ready flag(TXRDY flaginSCIFLR register), andaDMA
request (TXDMA) could occur after transmitting aresponse.
Figure 29-8 andFigure 29-9 shows thereceive andtransmit multi-buffer functional block diagram.
Figure 29-8. Receive Buffers

<!-- Page 1634 -->

TD0
TD1
TD2
TD3
TD4
TD5
TD6
TD70 7SCITXSHF0 7
0 7
0 7
0 7
0 7
0 7
0 7
0 73-bit
CounterCE Flag
3-bit
Compare
=TX DMA
RequestCHECKSUM
CALCULATORTX
MBUF MODETX Ready Flag
Not
MBUF MODE
SCI www.ti.com
1634 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-9. Transmit Buffers

<!-- Page 1635 -->

INT0
INT1
INT2
INT3
INT4
INT5
INT6
INT7
INT8
INT9Priority Encoder 1
Priority Encoder 0
SCIINTFLRSCISETINT
SCICLRINT
SCISETINTL
SCICLRLINT 1
INT 0
SCIINTVECT1SCIINTVECT0INT10
INT11
INT12
INT13
INT14
INT15
INT16
www.ti.com SCI
1635 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.2 SCIInterrupts
The SCI/LIN module hastwointerrupt lines, level 0andlevel 1,tothevectored interrupt manager (VIM)
module (see Figure 29-10 ).Two offset registers SCIINTVECT0 andSCIINTVECT1 determine which flag
triggered theinterrupt according totherespective priority encoders. Each interrupt condition hasabitto
enable/disable theinterrupt intheSCISETINT andSCICLRINT registers respectively.
Each interrupt also hasabitthatcanbesetasinterrupt level 0(INT0) orasinterrupt level 1(INT1). By
default, interrupts areininterrupt level 0.SCISETINTLVL sets agiven interrupt tolevel1.
SCICLEARINTLVL resets agiven interrupt level tothedefault level 0.
The interrupt vector registers SCIINTVECT0 andSCIINTVECT1 return thevector ofthepending interrupt
lineINT0 orINT1. Ifmore than oneinterrupt ispending, theinterrupt vector register holds thehighest
priority interrupt.
Figure 29-10. General Interrupt Scheme

<!-- Page 1636 -->

ENA INTx
INT1
...INT0
FLAGxINTxLVL......Priority
Encoder 0
...
Priority
Encoder 15-bit
INTVECT0
5-bit
INTVECT1
SCI www.ti.com
1636 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-11. Interrupt Generation forGiven Flags
29.2.2.1 Transmit Interrupt
Tousetransmit interrupt functionality, SET TXINTbitmust beenabled andSET TXDMA bitmust be
cleared. The transmit ready (TXRDY) flagissetwhen theSCItransfers thecontents ofSCITD totheshift
register, SCITXSHF. The TXRDY flagindicates thatSCITD isready tobeloaded with more data. In
addition, theSCIsets theTXEMPTY bitifboth theSCITD andSCITXSHF registers areempty. IftheSET
TXINTbitisset,then atransmit interrupt isgenerated when theTXRDY flaggoes high. Transmit Interrupt
isnotgenerated immediately after setting theSET TXINTbitunlike transmit DMA request. Transmit
Interrupt isgenerated only after thefirsttransfer from SCITD toSCITXSHF, thatisfirstdata hastobe
written toSCITD byyoubefore anyinterrupt gets generated. Totransmit further data, youcanwrite data
toSCITD inthetransmit Interrupt service routine.
Writing data totheSCITD register clears theTXRDY bit.When thisdata hasbeen moved tothe
SCITXSHF register, theTXRDY bitissetagain. The interrupt request canbesuspended bysetting the
CLR TXINTbit;however, when theSET TXINTbitisagain setto1,theTXRDY interrupt isasserted
again. The transmit interrupt request canbeeliminated until thenext series ofvalues iswritten toSCITD,
bydisabling thetransmitter viatheTXENA bit,byasoftware reset SWnRST, orbyadevice hardware
reset.
29.2.2.2 Receive Interrupt
The receive ready (RXRDY) flagissetwhen theSCItransfers newly received data from SCIRXSHF to
SCIRD. The RXRDY flagtherefore indicates thattheSCIhasnew data toberead. Receive interrupts are
enabled bytheSET RXINTbit.IftheSET RXINTissetwhen theSCIsets theRXRDY flag, then a
receive interrupt isgenerated. The received data canberead intheInterrupt Service routine.
Onadevice with both SCIandaDMA controller, thebitsSET RXDMA ALL andSET RXDMA must be
cleared toselect interrupt functionality.
29.2.2.3 WakeUp Interrupt
SCIsets theWAKEUP flagifbusactivity ontheRXlineeither prevents power-down mode from being
entered, orRXlineactivity causes anexitfrom power-down mode. Ifenabled (SET WAKEUP INT),
wakeup interrupt istriggered once WAKEUP flagisset.

<!-- Page 1637 -->

www.ti.com SCI
1637 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.2.4 Error Interrupts
The following error detection aresupported with Interrupt bytheSCImodule:
*Parity errors (PE)
*Frame errors (FE)
*Break Detect errors (BRKDT)
*Overrun errors (OE)
*Biterrors (BE)
Ifallofthese errors (PE, FE,BRKDT, OE, BE)areflagged, aninterrupt fortheflagged errors willbe
generated ifenabled. Amessage isvalid forboth thetransmitter andthereceiver ifthere isnoerror
detected until theendoftheframe. Each ofthese flags islocated inthereceiver status (SCIFLR) register.
There are16interrupt sources intheSCI/LIN module, InSCImode 8interrupts aresupported, asseen in
Table 29-4.
(1)Offset 1isthehighest priority. Offset 16isthelowest priority.Table 29-4. SCI/LIN Interrupts
Offset(1)Interrupt Applicable toSCI Applicable toLIN
0 Nointerrupt
1 Wakeup Yes Yes
2 Inconsistent-synch-field error No Yes
3 Parity error Yes Yes
4 ID No Yes
5 Physical buserror No Yes
6 Frame error Yes Yes
7 Break detect Yes No
8 Checksum error No Yes
9 Overrun error Yes Yes
10 Biterror Yes Yes
11 Receive Yes Yes
12 Transmit Yes Yes
13 No-response error No Yes
14 Timeout after wakeup signal (150 ms) No Yes
15 Timeout after three wakeup signals (1.5 s) No Yes
16 Timeout (Bus Idle, 4s) No Yes

<!-- Page 1638 -->

SCI www.ti.com
1638 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.3 SCIDMA Interface
DMA requests forreceive (RXDMA request) andtransmit (TXDMA request) areavailable fortheSCI/LIN
module. The DMA transfers depending onwhether multi-buffer mode bit(MBUF MODE) isenabled ornot
enabled. See Chapter 30formore information.
29.2.3.1 Receive DMA Requests
This DMA functionality isenabled/disabled bytheCPU using theSET RXDMA/CLR RXDMA bits,
respectively.
InMulti-Buffered SCImode with DMA enabled, thereceiver loads theRDy buffers foreach received
character. RXDMA request istriggered once thelastcharacter oftheprogrammed number ofcharacters
(LENGTH) arereceived andcopied tothecorresponding RDy buffer successfully.
Ifthemulti-buffer option isdisabled, then DMA requests willbegenerated onabyte-per-byte basis.
Inmultiprocessor mode, theSCIcangenerate receiver interrupts foraddress frames andDMA requests
fordata frames orDMA requests forboth. This iscontrolled bytheSET RXDMA ALL bit.
Inmultiprocessor mode with theSLEEP bitset,noDMA isgenerated forreceived data frames. The
software must clear theSLEEP bitbefore data frames canbereceived.
29.2.3.2 Transmit DMA Requests
DMA functionality isenabled/disabled bytheCPU with SET TXDMA/CLR TXDMA bits, respectively.
InMulti-Buffered SCImode once TXRDY bitissetorafter atransmission ofprogrammed number of
characters (LENGTH) (uptoeight data bytes stored inthetransmit buffer(s) TDy intheLINTD0 and
LINTD1 registers), aDMA request isgenerated inorder toreload thetransmit buffer forthenext
transmission. Ifthemulti-buffer option isdisabled, then DMA requests willbegenerated onabyte-per-byte
basis.

<!-- Page 1639 -->

www.ti.com SCI
1639 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.4 SCIConfigurations
Before theSCIsends orreceives data, itsregisters should beproperly configured. Upon power-up ora
system-level reset, each bitintheSCIregisters issettoadefault state. The registers arewritable only
after theRESET bitintheSCIGCR0 register issetto1.Ofparticular importance istheSWnRST bitinthe
SCIGCR1 register. The SWnRST isanactive-low bitinitialized to0andkeeps theSCIinareset state
until itisprogrammed to1.Therefore, allSCIconfiguration should becompleted before a1iswritten to
theSWnRST bit.
The following listdetails theconfiguration steps thatsoftware should perform prior tothetransmission or
reception ofdata. Aslong astheSWnRST bitiscleared to0theentire time thattheSCIisbeing
configured, theorder inwhich theregisters areprogrammed isnotimportant.
*Enable SCIbysetting theRESET bitto1.
*Clear theSWnRST bitto0before SCIisconfigured.
*Select thedesired frame format byprogramming theSCIGCR1 register.
*Setboth theRXFUNC andTXFUNC bitsinSCIPIO0 to1toconfigure theLINRX andLINTX pins for
SCIfunctionality.
*Select thebaud rate tobeused forcommunication byprogramming theBRS register.
*SettheCLOCK bitinSCIGCR1 to1toselect theinternal clock.
*SettheCONT bitinSCIGCR1 to1tomake SCInothaltforanemulation breakpoint until itscurrent
reception ortransmission iscomplete (this bitisused only inanemulation environment).
*SettheLOOP BACK bitinSCIGCR1 to1toconnect thetransmitter tothereceiver internally (this
feature isused toperform aself-test).
*SettheRXENA bitinSCIGCR1 to1,ifdata istobereceived.
*SettheTXENA bitin SCIGCR1 to1,ifdata istobetransmitted.
*SettheSWnRST bitto1after SCIisconfigured.
*Perform receiving ortransmitting data (see Section 29.2.4.1 orSection 29.2.4.2 ).
29.2.4.1 Receiving Data
The SCIreceiver isenabled toreceive messages ifboth theRXFUNC bitandtheRXENA bitaresetto1.
IftheRXFUNC bitisnotset,theLINRX pinfunctions asageneral-purpose I/Opinrather than asanSCI
function pin.
SCImodule canreceive data inoneofthefollowing modes:
*Single-Buffer (Normal) Mode
*Multi-Buffer Mode
After avalid idleperiod isdetected, data isautomatically received asitarrives ontheLINRX pin.
29.2.4.1.1 Receiving Data inSingle-Buffer Mode
Single-buffer mode isselected when theMBUF MODE bitinSCIGCR1 iscleared to0.Inthismode, SCI
sets theRXRDY bitwhen ittransfers newly received data from SCIRXSHF toSCIRD. The SCIclears the
RXRDY bitafter thenew data inSCIRD hasbeen read. Also, asdata istransferred from SCIRXSHF to
SCIRD, theSCIsets theFE,OE, orPEflags ifanyofthese error conditions were detected inthereceived
data. These error conditions aresupported with configurable interrupt capability. The wake-up andbreak-
detect status bitsarealso setifoneofthese errors occurs, butthey donotnecessarily occur atthesame
time thatnew data isbeing loaded intoSCIRD.
You canreceive data by:
1.Polling Receive Ready Flag
2.Receive Interrupt
3.DMA

<!-- Page 1640 -->

SCI www.ti.com
1640 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleInpolling method, software canpollfortheRXRDY bitandread thedata from theSCIRD register once
theRXRDY bitissethigh. The CPU isunnecessarily overloaded byselecting thepolling method. Toavoid
this, youcanuseeither theinterrupt orDMA method. Tousetheinterrupt method, theSET RXINTbitis
set.TousetheDMA method, theSET RXDMA bitisset.Either aninterrupt oraDMA request is
generated themoment theRXRDY bitisset.
29.2.4.1.2 Receiving Data inMulti-Buffer Mode
Multi-buffer mode isselected when theMBUF MODE bitinSCIGCR1 issetto1.Inthismode, SCIsets
theRXRDY bitafter receiving theprogrammed number ofdata inthereceive buffer, thecomplete frame.
The error condition detection logic issimilar tothesingle-buffer mode, except thatitmonitors forthe
complete frame. Like single-buffer mode, youcanusethepolling, interrupt, orDMA method toread the
data. The SCIclears theRXRDY bitafter thenew data inSCIRD hasbeen read.
29.2.4.2 Transmitting Data
The SCItransmitter isenabled ifboth theTXFUNC bitandtheTXENA bitaresetto1.IftheTXFUNC bit
isnotset,theLINTX pinfunctions asageneral-purpose I/Opinrather than asanSCIfunction pin.Any
value written totheSCITD before TXENA issetto1isnottransmitted. Both ofthese control bitsallow for
theSCItransmitter tobeheld inactive independently ofthereceiver.
SCImodule cantransmit data inoneofthefollowing modes:
*Single-Buffer (Normal) Mode
*Multi-Buffered orBuffered SCIMode
29.2.4.2.1 Transmitting Data inSingle-Buffer Mode
Single-buffer mode isselected when theMBUF MODE bitinSCIGCR1 iscleared to0.Inthismode, SCI
waits fordata tobewritten toSCITD, transfers ittoSCITXSHF, andtransmits thedata. The TXRDY and
TXEMPTY bitsindicate thestatus ofthetransmit buffers. That is,when thetransmitter isready fordata to
bewritten toSCITD, theTXRDY bitisset.Additionally, ifboth SCITD andSCITXSHF areempty, then the
TXEMPTY bitisalso set.
You cantransmit data by:
1.Polling Transmit Ready Flag
2.Transmit Interrupt
3.DMA
Inpolling method, software canpollfortheTXRDY bittogohigh before writing thedata totheSCITD
register. The CPU isunnecessarily overloaded byselecting thepolling method. Toavoid this, youcanuse
either theinterrupt orDMA method. Tousetheinterrupt method, theSET TXINTbitisset.Tousethe
DMA method, theSET TXDMA bitisset.Either aninterrupt oraDMA request isgenerated themoment
theTXRDY bitisset.When theSCIhascompleted transmission ofallpending frames, theSCITXSHF
register andSCITD areempty, theTXRDY bitisset,andaninterrupt/DMA request isgenerated, if
enabled. Because alldata hasbeen transmitted, theinterrupt/DMA request should behalted. This can
either bedone bydisabling thetransmit interrupt (CLR TXINT) /DMA request (CLR TXDMA bit)orby
disabling thetransmitter (clear TXENA bit).
NOTE: The TXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0 orSCIINTVECT1 register.
29.2.4.2.2 Transmitting Data inMulti-Buffer Mode
Multi-buffer mode isselected when theMBUF MODE bitinSCIGCR1 issetto1.Like single-buffer mode,
youcanusethepolling, interrupt, orDMA method towrite thedata tobetransmitted. The transmitted data
hastobewritten totheSCITD registers. SCIwaits fordata tobewritten totheSCITD register and
transfers theprogrammed number ofbytes toSCITXSHF totransmit onebyoneautomatically.

<!-- Page 1641 -->

www.ti.com SCI
1641 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.2.5 SCILow-Power Mode
The SCI/LIN canbeputineither local orglobal low-power mode. Global low-power mode isasserted by
thesystem andisnotcontrolled bytheSCI/LIN. During global low-power mode, allclocks totheSCI/LIN
areturned offsothemodule iscompletely inactive.
Local low-power mode isasserted bysetting thePOWERDOWN bit;setting thisbitstops theclocks tothe
SCI/LIN internal logic andthemodule registers. Setting thePOWERDOWN bitcauses theSCItoenter
local low-power mode andclearing thePOWERDOWN bitcauses SCI/LIN toexitfrom local low-power
mode. Alltheregisters areaccessible during local power-down mode asanyregister access enables the
clock toSCIforthatparticular access alone.
The wake-up interrupt isused toallow theSCItoexitlow-power mode automatically when alowlevel is
detected ontheLINRX pinandalso thisclears thePOWERDOWN bit.Ifwake-up interrupt isdisabled,
then theSCI/LIN immediately enters low-power mode whenever itisrequested andalso anyactivity on
theLINRX pindoes notcause theSCItoexitlow-power mode.
NOTE: Enabling Local Low-Power Mode During Receive andTransmit
Ifthewake-up interrupt isenabled andlow-power mode isrequested while thereceiver is
receiving data, then theSCIimmediately generates awake-up interrupt toclear the
powerdown bitandprevents theSCIfrom entering low-power mode andthus completes the
current reception. Otherwise, ifthewake-up interrupt isdisabled, then theSCIcompletes the
current reception andthen enters thelow-power mode.
29.2.5.1 Sleep Mode forMultiprocessor Communication
When theSCIreceives data andtransfers thatdata from SCIRXSHF toSCIRD, theRXRDY bitissetand
ifRXINTENA isset,theSCIalso generates aninterrupt. The interrupt triggers theCPU toread thenewly
received frame before another oneisreceived. Inmultiprocessor communication modes, thisdefault
behavior may beenhanced toprovide selective indication ofnew data. When SCIreceives anaddress
frame thatdoes notmatch itsaddress, thedevice canignore thedata following thisnon-matching address
until thenext address frame byusing sleep mode. Sleep mode canbeused with both idle-line and
address-bit multiprocessor modes.
Ifsleep mode isenabled bytheSLEEP bit,then theSCItransfers data from SCIRXSHF toSCIRD only for
address frames. Therefore, insleep mode, alldata frames areassembled intheSCIRXSHF register
without being shifted intotheSCIRD andwithout initiating areceive interrupt orDMA request. Upon
reception ofanaddress frame, thecontents oftheSCIRXSHF aremoved intoSCIRD, andthesoftware
must read SCIRD anddetermine iftheSCIisbeing addressed bycomparing thereceived address against
theaddress previously setinthesoftware andstored somewhere inmemory (the SCIdoes nothave
hardware available foraddress comparison). IftheSCIisbeing addressed, thesoftware must clear the
SLEEP bitsothattheSCIwillload SCIRD with thedata ofthedata frames thatfollow theaddress frame.
When theSCIhasbeen addressed andsleep mode hasbeen disabled (insoftware) toallow thereceipt of
data, theSCIshould check theRXWAKE bit(SCIFLR.12) todetermine when thenext address hasbeen
received. This bitissetto1ifthecurrent value inSCIRD isanaddress andsetto0ifSCIRD contains
data. IftheRXWAKE bitisset,then software should check theaddress inSCIRD against itsown address.
Ifitisstillbeing addressed, then sleep mode should remain disabled. Otherwise, theSLEEP bitshould be
setagain.

<!-- Page 1642 -->

LIN www.ti.com
1642 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFollowing isasequence ofevents typical ofsleep mode operation:
*The SCIisconfigured andboth sleep mode andreceive actions areenabled.
*Anaddress frame isreceived andareceive interrupt isgenerated.
*Software compares thereceived address frame against thatsetbysoftware anddetermines thatthe
SCIisnotbeing addressed, sothevalue oftheSLEEP bitisnotchanged.
*Several data frames areshifted intoSCIRXSHF, butnodata ismoved toSCIRD andnoreceive
interrupts aregenerated.
*Anew address frame isreceived andareceive interrupt isgenerated.
*Software compares thereceived address frame against thatsetbysoftware anddetermines thatthe
SCIisbeing addressed andclears theSLEEP bit.
*Data shifted intoSCIRXSHF istransferred toSCIRD, andareceive interrupt isgenerated after each
data frame isreceived.
*Ineach interrupt routine, software checks RXWAKE todetermine ifthecurrent frame isanaddress
frame.
*Another address frame isreceived, RXWAKE isset,software determines thattheSCIisnotbeing
addressed andsets theSLEEP bitback to1.Noreceive interrupts aregenerated forthedata frames
following thisaddress frame.
Byignoring data frames thatarenotintended forthedevice, fewer interrupts aregenerated. These
interrupts would otherwise require CPU intervention toread data thatisofnosignificance tothisspecific
device. Using sleep mode canhelp free some CPU resources.
Except fortheRXRDY flag, theSCIcontinues toupdate thereceiver status flags (see Table 29-13 )while
sleep mode isactive. Inthisway, ifanerror occurs onthereceive line, anapplication canimmediately
respond totheerror andtake theappropriate corrective action.
Because theRXRDY bitisnotupdated fordata frames when sleep mode isenabled, theSCIcanenable
sleep mode anduseapolling algorithm ifdesired. Inthiscase, when RXRDY isset,software knows thata
new address hasbeen received. IftheSCIisnotbeing addressed, then thesoftware should notchange
thevalue oftheSLEEP bitandshould continue topollRXRDY.
29.3 LIN
29.3.1 LINCommunication Formats
The SCI/LIN module canbeused inLINmode orSCImode. The enhancements forbaud generation,
DMA controls andadditional receive/transmit buffers necessary forLINmode operation arealso part of
theenhanced buffered SCImodule. LINmode isselected byenabling LINMODE bitinSCIGCR1 register.
NOTE: The SCI/LIN isbuilt around theSCIplatform anduses asimilar sampling scheme: 16
samples foreach bitwith majority vote onsamples 8,9,and10.
The SCI/LIN control registers arelocated attheSCI/LIN base address. Foradetailed description ofeach
register, seeSection 29.7.
29.3.1.1 LINStandards
Forcompatibility with LIN2.0 standard thefollowing additional features areimplemented over LIN1.3:
i.Support forLIN2.0checksum
ii.Enhanced synchronizer FSM support forframe processing
iii.Enhanced handling ofextended frames
iv.Enhanced baud rate generator
v.Update wakeup/go tosleep
The LINmodule covers theCPU performance-consuming features, defined intheLINSpecification
Package Revision 1.3and2.0byhardware.

<!-- Page 1643 -->

SYNCH BREAK FIELD SYNCH FIELD ID FIELD
Sync Break Sync Field ID Field 1, 2, 3, ...8 Data Fields ChecksumMaster Header Slave ResponseMessage Frame
In-frame spaceMaster or Slave
www.ti.com LIN
1643 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleThe Master Mode ofLINmodule iscompatible with LIN2.1standard.
29.3.1.2 Message Frame
The LINprotocol defines amessage frame format, illustrated inFigure 29-12 .Each frame includes one
master header, oneresponse, onein-frame response space, andinter-byte spaces. In-frame-response
andinter-byte spaces may be0.
Figure 29-12. LINProtocol Message Frame Format: Master Header andSlave Response
There isnoarbitration inthedefinition oftheLINprotocol; therefore, multiple slave nodes responding toa
header might bedetected asanerror.
The LINbusisasingle channel wired-AND. The bushasabinary level: either dominant foravalue of0,
orrecessive foravalue of1.
29.3.1.2.1 Message Header
The header ofamessage isinitiated byamaster (see Figure 29-13 )andconsists ofathree field-
sequence:
*The synch break field signaling thebeginning ofamessage
*The synch field conveying bitrate information oftheLINbus
*The IDfield denoting thecontent ofamessage
Figure 29-13. Header 3Fields: Synch Break, Synch, andID

<!-- Page 1644 -->

1 to 8 Data FieldsResponse
Checksum
Field
interbyte spaces
LIN www.ti.com
1644 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.2.2 Response
The format oftheresponse isasillustrated inFigure 29-14 .There aretwotypes offields inaresponse:
data andchecksum. The data field consists ofexactly onedata byte, onestart bit,andonestop bit,fora
total of10bits. The LSB istransmitted first. The checksum field consists ofonechecksum byte, onestart
bitandonestop bit.The checksum byte istheinverted modulo-256 sum over alldata bytes inthedata
fields oftheresponse.
Figure 29-14. Response Format ofLINMessage Frame
The format oftheresponse isastream ofNdata fields andonechecksum field. Typically Nisfrom 1to8,
with theexception oftheextended command frames (Section 29.3.1.6 ).The length Noftheresponse is
indicated either with theoptional length control bitsoftheIDField (this isused instandards earlier than
LIN1.x); seeTable 29-5,orbyLENGTH value inSCIFORMAT[18:16] register; seeTable 29-6 .The
SCI/LIN module supports response lengths from 1to8bytes incompliance with LIN2.0.
Table 29-5. Response Length InfoUsing IDBYTE Field Bits [5:4] forLINStandards Earlier than 1.3
ID5 ID4 Number ofData bytes
0 0 2
0 1 2
1 0 4
1 1 8
Table 29-6. Response Length with SCIFORMAT[18:16] Programming
SCIFORMAT[18:16] No.ofBytes
000 1
001 2
010 3
011 4
100 5
101 6
110 7
111 8

<!-- Page 1645 -->

FLINCLKFVCLK
16(P +1 + M )
16- - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - For all P other than zero =
FLINCLKFVCLK
32- - - - - - - - - - - - - - - - - - - - For P = 0 =
VCLK bit TMP T /c247
/c248/c246/c231
/c232/c230/c43/c43 /c61161 16
www.ti.com LIN
1645 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.3 Synchronizer
The synchronizer hasthree major functions inthemessaging between master andslave nodes. It
generates themaster header data stream, itsynchronizes totheLINbusforresponding, anditlocally
detects timeouts. Abitrate isprogrammed using theprescalers intheBRS register tomatch theindicated
LIN_speed value intheLINdescription file.
The LINsynchronizer willperform thefollowing functions: master header signal generation, slave
detection andsynchronization tomessage header with optional baud rate adjustment, response
transmission timing andtimeout control.
The LINsynchronizer iscapable ofdetecting anincoming break andinitializing communication atall
times.
29.3.1.4 Baud Rate
The transmission baud rate ofanynode isconfigured bytheCPU atthebeginning; thisdefines thebit
time Tbit.The bittime isderived from thefields PandMinthebaud rate selection register (BRS). There is
anadditional 3-bit fractional divider value, field UintheBRS register, which further fine-tunes thedata
field baud rate.
The ranges fortheprescaler values intheBRS register are:
P=0,1,2,3,...,224-1
M=0,1,2,...,15
U=0,1,2,3,4,5,6,7
The P,M,andUvalues intheBRS register areuser programmable. The PandMdividers could beused
forboth SCImode andLINmode toselect abaud rate. The Uvalue isanadditional 3-bit value
determining that"aTVCLK "(with a=0,1) isadded toeach Tbitasexplained inSection 29.3.1.4.2 .Ifthe
ADAPT bitissetandtheLINslave isinadaptive baud rate mode, then allthese divider values are
automatically obtained during header reception when thesynchronization field ismeasured.
The LINprotocol defines baud rate boundaries asfollows:
1kHz≤FLINCLK≤20kHz
Alltransmitted bitsareshifted inandoutatTbitperiods.
29.3.1.4.1 Fractional Divider
The Mfield oftheBRS register modifies theinteger prescaler Pforfinetuning ofthebaud rate. The M
value adds inincrements of1/16 ofthePvalue.
The bittime, Tbitisexpressed interms oftheVCLK period TVCLKasfollows:
ForallPother than 0,andallM,
(45)
ForP=0:Tbit=32TVCLK
Therefore, theLINCLK frequency isgiven by:
(46)

<!-- Page 1646 -->

V C L KaTdMP bitT/c250/c251/c249
/c234/c235/c233/c43/c247
/c248/c246/c231
/c232/c230/c43/c43 /c61161 16
VCLKiTdMP bitT/c250/c251/c249
/c234/c235/c233/c43/c247
/c248/c246/c231
/c232/c230/c43/c43 /c61161 16
LIN www.ti.com
1646 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.4.2 Superfractional Divider
The superfractional divider scheme applies tothefollowing modes:
*LINmaster mode (synch field +identifier field +response field +checksum field)
*LINslave mode (response field +checksum field)
29.3.1.4.3 Superfractional Divider InLINMode
Building onthe4-bit fractional divider M(BRS[27:24], thesuperfractional divider uses anadditional 3-bit
modulating value, illustrated inTable 29-7.The sync field (0x55), theidentifier field andtheresponse field
canallbeseen as8-bit data bytes flanked byastart bitandastop bit.The bitswith a1inthetable will
have anadditional VCLK period added totheir Tbit.
(1)
1.InLINmaster mode bitmodulation applies tosynch field +identifier field +response field
2.InLINslave mode bitmodulation applies toidentifier field +response fieldTable 29-7. Superfractional BitModulation forLINMaster Mode andSlave Mode(1)
BRS[30:28] Start Bit D[0] D[1] D[2] D[3] D[4] D[5] D[6] D[7] Stop Bit
0h 0 0 0 0 0 0 0 0 0 0
1h 1 0 0 0 0 0 0 0 1 0
2h 1 0 0 0 1 0 0 0 1 0
3h 1 0 1 0 1 0 0 0 1 0
4h 1 0 1 0 1 0 1 0 1 0
5h 1 1 1 0 1 0 1 0 1 1
6h 1 1 1 0 1 1 1 0 1 1
7h 1 1 1 1 1 1 1 0 1 1
The baud rate willvary over aLINdata field toaverage according totheBRS[30:28] value byadfraction
oftheperipheral internal clock: 0<d<1.
The instantaneous bittime isexpressed interms ofTVCLKasfollows:
ForallPother than 0,andallMandd(0or1),
(47)
ForP=0Tbit=32TVCLK
The averaged bittime isexpressed interms ofTVCLKasfollows :
ForallPother than 0,andallMandd(0<d<1),
(48)
ForP=0Tbit=32TVCLK
With thesuperfractional divider, aLINbaud rate of20kbps isachievable with aninternal clock VCLK of
726kHz. Furthermore, arate of400kbps isachievable with anVCLK of14.6 MHz.

<!-- Page 1647 -->

ID0 ID1 ID2 ID3 ID4 ID5 P0 P1StartBitStopBit
Optional
Length
Contro l
BitsID Field
Option al
Parity
Bits
SDEL
1..4 TbitSYNCH BREAK
13+(0...7) TbitSYNCH FIELD
10 TbitID FIELD
10 Tbit
START BITSTOP BIT
010 1 010 1
www.ti.com LIN
1647 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.5 Header Generation
Automatic generation oftheLINprotocol header data stream issupported without CPU interaction. The
CPU prtheDMA willtrigger amessage header generation andtheLINstate machine willhandle the
generation itself. Amaster node initiates header generation onCPU orDMA writes totheIDBYTE inthe
LINID register. The header isalways sent bythemaster toinitiate aLINcommunication andconsists of
three fields: break field, synchronization field, andidentification field, asseen inFigure 29-15 .
Figure 29-15. Message Header inTerms ofTbit
*The break field consists oftwocomponents:
-The synchronization break (SYNCH BREAK) consists ofaminimum of13(dominant) lowbitstoa
maximum of20dominant bits. The synch break length may beextended from theminimum with the
3-bit SBREAK value intheLINCOMP register.
-The synchronization break delimiter (SDEL) consists ofaminimum of1(recessive) high bittoa
maximum of4recessive bits. The delimiter marks theendofthesynchronization break field. The
synch break delimiter length depends onthe2-bit SDEL value intheLINCOMP register.
*The synchronization field (SYNCH FIELD) consists ofonestart bit,byte 0x55, andastop bit.Itisused
toconvey Tbitinformation andresynchronize LINbusnodes.
*The identifier field'sIDbyte may usesixbitsasanidentifier, with optional length control (see
Note:Optional Control Length Bits),andtwooptional bitsasparity oftheidentifier. The identifier parity
isused andchecked ifthePARITY ENA bitisset.Iflength control bitsarenotused, then there canbe
atotal of64identifiers plus parity. Ifneither length control orparity areused there canbeupto256
identifiers. See Figure 29-16 foranillustration oftheIDfield.
Figure 29-16. IDField
NOTE: Optional Control Length Bits
The control length bitsonly apply toLINstandards prior toLIN1.3.IDBYTE field conveys
response length information ifcompliant tostandards earlier than LIN1.3. The SCIFORMAT
register stores thelength oftheresponse forlater versions oftheLINprotocol.

<!-- Page 1648 -->

LIN www.ti.com
1648 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleNOTE: IftheBLIN module, configured asSlave inmulti-buffer mode, isintheprocess of
transmitting data while anew header comes in,themodule might endupinresponding with
thedata from theprevious interrupted response (not thedata corresponding tothenew ID).
Toavoid thisscenario thefollowing procedure could beused:
1.Check fortheBitError (BE) during theresponse transmission. IftheBEflagisset,this
indicates thatacollision hashappened ontheLINbus(here because ofthenew Synch
Break).
2.IntheBitError ISR, configure theTD0 andTD1 registers with thenext setofdata tobe
transmitted onaTXMatch fortheincoming ID.Before writing toTD0/TD1 make sure
thatthere was notalready anupdate because ofaBitError; otherwise TD0/TD1 might
bewritten twice foroneID.
3.Once thecomplete IDisreceived, based onthematch, thenewly configured data willbe
transmitted bythenode.
29.3.1.5.1 Event Triggered Frame Handling Proposal
The LIN2.0protocol uses event-triggered frames thatmay occasionally cause collisions. Event-triggered
frames have tobehandled insoftware.
Ifnoslave answers toanevent triggered frame header, themaster node willsettheNRE flag, andaNRE
interrupt willoccur ifenabled. Ifacollision occurs, aframe error andchecksum error may arise before the
NRE error. Those errors areflagged andtheappropriate interrupts willoccur, ifenabled.
Frame errors andchecksum errors depend onthebehavior andsynchronization oftheresponding slaves.
Iftheslaves aretotally synchronized andstop transmission once thecollision occurred, itispossible that
only theNRE error isflagged despite theoccurrence ofacollision. Todetect ifthere hasbeen areception
ofonebyte before theNRE error isflagged, theBUS BUSY flagcanbeused asanindicator.
The busbusy flagissetonthereception ofthefirstbitoftheheader andremains setuntil theheader
reception iscomplete, andagain issetonthereception ofthefirstbitoftheresponse. Inthecase ofa
collision theflagiscleared inthesame cycle astheNRE flagisset.
Software could implement thefollowing sequence:
*Once thereception oftheheader isdone (poll forRXID flag), wait forthebusbusy flagtogetsetor
NRE flagtogetset.
*Ifbusbusy flagisnotsetbefore NRE flag, then itisatrue noresponse case (nodata hasbeen
transmitted onto thebus).
*Ifbusbusy flaggets set,then wait forNRE flagtogetsetorforsuccessful reception. IfNRE flagisset,
then inthiscase acollision hasoccurred onthebus.
Even inthecase ofacollision, thereceived (corrupted) data isaccessible intheRXbuffers; registers
LINRD0 andLINRD1.

<!-- Page 1649 -->

BRK_countSDEL
BAUD_countSYNCH BREAKSYNCH FIELD
10 TbitID FIELD
10 Tbit
START BIT STOP BIT
010 1 0 1 0 1
1 2 3 4
www.ti.com LIN
1649 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.5.2 Header Reception andAdaptive Baud Rate
Aslave node baud rate canoptionally beadjusted tothedetected bitrate asanoption totheLINmodule.
The adaptive baud rate option isenabled bysetting theADAPT bit.During header reception, aslave
measures thebaud rate during detection ofthesynch field. IfADAPT bitisset,then themeasured baud
rate iscompared totheslave node 'sprogrammed baud rate andadjusted totheLINbusbaud rate if
necessary.
The LINsynchronizer determines twomeasurements: BRK_count andBAUD_count (Figure 29-17 ).These
values arealways calculated during theHeader reception forsynch field validation (Figure 29-18 ).
Figure 29-17. Measurements forSynchronization
Bymeasuring thevalues BRK_count andBAUD_count, avalid synch break sequence canbedetected as
described inFigure 29-18 .The four numbered events inFigure 29-17 signal thestart/stop ofthe
synchronizer counter. The synchronizer counter uses VCLK asthetime base.
The synchronizer counter isused tomeasure thesynch break relative tothedetecting node Tbit.Fora
slave node receiving thesynch break, athreshold of11Tbitisused asrequired bytheLINprotocol. For
detection ofthedominant data stream ofthesynch break, thesynchronizer counter isstarted onafalling
edge andstopped onarising edge oftheLINRX. Ondetection ofthesynch break delimiter, the
synchronizer counter value issaved andthen reset.
Ondetection offiveconsecutive falling edges, theBAUD_count ismeasured. Bittiming calculation and
consistency torequired accuracy isimplemented following therecommendations ofLINrevision 2.0.A
slave node cancalculate asingle Tbittime bydivision ofBAUD_count by8.Inaddition, forconsistency
between thedetected edges thefollowing isevaluated:
BAUD_count +BAUD_count »2+BAUD_count »3≤BRK_count
The BAUD_count value isshifted 3times totheright androunded using thefirstinsignificant bittoobtain
aTbitunit. IftheADAPT bitisset,then thedetected baud rate iscompared totheprogrammed baud rate.
During theheader reception processing asillustrated inFigure 29-18 ,ifthemeasured BRK_count value is
less than 11Tbit,thesynch break isnotvalid according totheprotocol forafixed rate. IftheADAPT bitis
set,then theMBRS register isused formeasuring BRK_count andBAUD_count values andautomatically
adjusts toanyallowed LINbusrate (refer toLINSpecification Package 2.0).
NOTE: Inadaptive mode theMBRS divider should besettoallow amaximum baud rate thatisnot
more than 10% above theexpected operating baud rate intheLINnetwork. Otherwise, a
0x00 data byte could mistakenly bedetected asasynch break.
The break-threshold relative totheslave node is11Tbit.The break is13Tbitasspecified in
LINversion 1.3.

<!-- Page 1650 -->

Wait for falling
edge
Increment
counterwhile
LINRX=0LINRX=1
No
Yes
Increment
counter LINRX=0
Save counter
( SBRK_count)
and reset itOn LINRX11 ?bitcounter ≥
T
increment
counterOn 1st LINRX falling edge
Receive ID Field
Wait for ResponseOn 5th LINRX falling edgeOn LINRX
o Reset counter
2
31
4
Save counter ( BAUD_count)
Verify valid Synch Field
If ADAPT=1, compare baud rate and Baud
Update flag is set if baudrates differ
LIN www.ti.com
1650 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-18. Synchronization Validation Process andBaud Rate Adjustment
Ifthesynch field isnotdetected within thegiven tolerances, theinconsistent-synch-field-error (ISFE) flag
willbeset.AnISFE interrupt willbegenerated, ifenabled byitsrespective bitintheSCISETINT register.
The IDbyte should bereceived after thesynch field validation was successful. Any time avalid break
(larger than 11Tbit)isdetected, thereceiver 'sstate machine should reset toreception ofthisnew frame.
This reset condition isonly valid during response state, notifanadditional synch break occurs during
header reception.

<!-- Page 1651 -->

Sync Break Sync Field ID Field ChecksumMaster Header N Data Fields ResponseExtended Frame With Embedded Checksum Bytes
In-frame space
Checksum Checksum
www.ti.com LIN
1651 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleNOTE: When aninconsistent synch field (ISFE) error occurs, suggested action fortheapplication is
toReset theSWnRST bitandsettheSWnRST bittomake sure thattheinternal state
machines areback totheir normal states
29.3.1.6 Extended Frames Handling
The LINprotocol 2.0andprior includes twoextended frames with identifiers 62(user-defined) and63
(reserved extended). The response data length oftheuser-defined frame (ID62,or0x3E) isunlimited.
The length forthisidentifier willbesetatnetwork configuration time tobeshared with theLINbusnodes.
Extended frame communication istriggered onreception ofaheader with identifier 0x3E; seeFigure 29-
19.Once theextended frame communication istriggered, unlike normal frames, thiscommunication
needs tobestopped before issuing another header. Tostop theextended frame communication the
STOP EXT FRAME bitmust beset.
Figure 29-19. Optional Embedded Checksum inResponse forExtended Frames
AnIDinterrupt willbegenerated (ifenabled andthere isamatch) onreception ofID62(0x3E). This
interrupt allows theCPU using asoftware counter tokeep track ofthebytes thatarebeing sent outand
decides when tocalculate andinsert achecksum byte (recommended atperiodic rates). Tohandle this
procedure, SCbitisused. Awrite tothesend checksum bitSCwillinitiate anautomatic send ofthe
checksum byte. The lastdata field should always beachecksum incompliance with theLINprotocol.
The periodicity ofthechecksum insertion, defined atnetwork configuration time, isused bythereceiving
node toevaluate thechecksum oftheongoing message, andhasthebenefit ofenhanced reliability.
Forthesending node, thechecksum isautomatically embedded each time thesend checksum bitSCis
set.Forthereceiving node, thechecksum iscompared each time thecompare checksum bitCCisset;
seeFigure 29-20 .
NOTE: The LIN2.0enhanced checksum does notapply tothereserved identifiers. The reserved
identifiers always usetheclassic checksum.

<!-- Page 1652 -->

SCIRXSHF7 0
SCITXSHF7 0RX
TXCHECKSUM CALCULATORCompare
Checksum
Send
Checksum
LIN www.ti.com
1652 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-20. Checksum Compare andSend forExtended Frames
29.3.1.7 Timeout Control
Any LINnode listening tothebusandexpecting aresponse initiated from amaster node could flagano-
response error timeout event. The LINprotocol defines four types oftimeout events, which areallhandled
bythehardware oftheLINmodule. The four LINprotocol events are:
*No-response timeout error
*Bus idledetection
*Timeout after wakeup signal
*Timeout after three wakeup signals
29.3.1.7.1 No-Response Error (NRE)
The no-response error willoccur when anynode expecting aresponse waits forTFRAME_MAX time andthe
message frame isnotfully completed within themaximum length allowed, TFRAME_MAX .After thistime ano-
response error (NRE) isflagged intheNRE bitoftheSCIFLR register. Aninterrupt istriggered ifenabled.
Asspecified intheLIN1.3standard, theminimum time totransmit aframe is:
TFRAME_MIN =THEADER_MIN +TDATA_FIELD +TCHECKSUM_FIELD
=44+10N
where N=number ofdata fields.
And themaximum time frame isgiven by:
TFRAME_MAX =TFRAME_MIN *1.4
=(44+10N) *1.4
The timeout value TFRAME_MAX isderived from theNnumber ofdata fields value. TheNvalue iseither
embedded intheheader 'sIDfield formessages orispart ofthedescription file.Inthelatter case, the3-
bitCHAR value inSCIFORMAT register, willindicate thevalue forN.
NOTE: The length coding oftheIDfield does notapply totwoextended frame identifiers, IDfields of
0x3E (62) and0x3F (63). Inthese cases, theIDfield canbefollowed byanarbitrary number
ofdata byte fields. Also, theLIN2.0protocol specification mentions thatIDfield 0x3F (63)
cannot beused. Forthese twocases, theNRE willnotbehandled bytheLINcontroller
hardware.

<!-- Page 1653 -->

www.ti.com LIN
1653 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-8. Timeout Values inTbitUnits
N TDATA_FIELD TFRAME_MIN TFRAME_MAX
1 10 54 76
2 20 64 90
3 30 74 104
4 40 84 118
5 50 94 132
6 60 104 146
7 70 114 160
8 80 124 174
29.3.1.7.2 Bus IdleDetection
The second type oftimeout canoccur when anode detects aninactive LINbus: notransitions between
recessive anddominant values aredetected onthebus. This happens after aminimum of4s(this is
80,000 FLINCLK cycles with thefastest busrate of20kbps). Ifanode detects noactivity inthebusasthe
TIMEOUT bitisset,then itcanbeassumed thattheLINbusisinsleep mode. Application software can
usetheTimeout flagtodetermine when theLINbusisinactive andputtheLINintosleep mode bywriting
thePOWERDOWN bit.
NOTE: After thetimeout was flagged, aSWnRESET should beasserted before entering Low-
Power Mode. This isrequired toreset thereceiver incase thatanincomplete frame was on
thebusbefore theidleperiod.
29.3.1.7.3 Timeout after Wakeup Signal andTimeout after Three Wakeup Signals
The third andfourth types oftimeout arerelated tothewakeup signal. Anode initiating awakeup should
expect aheader from themaster within adefined amount oftime: timeout after wakeup signal. See
Section 29.4.3 formore details.
29.3.1.8 TXRX Error Detector (TED)
The following sources oferror aredetected bytheTXRX error detector logic (TED). The TED logic
consists ofabitmonitor, anIDparity checker, andachecksum error. The following errors aredetected:
*Biterrors (BE)
*Physical buserrors (PBE)
*Identifier parity errors (PE)
*Checksum errors (CE)
Allofthese errors (BE, PBE, PE,CE) areflagged. Aninterrupt fortheflagged errors willbegenerated if
enabled. Amessage isvalid forboth thetransmitter andthereceiver ifthere isnoerror detected until the
endoftheframe.

<!-- Page 1654 -->

) (5 4 3 1 1) ( 4 2 1 0 0
P arity odd ID ID ID ID PP arity even ID ID ID ID P
/c197 /c197 /c197 /c61/c197 /c197 /c197 /c61
BIT MONITOR
SCITXSHF0 7 T
X
GNDVBAT
SCIRXSHF
=RX0 7
LIN BUSBit-Error Flag
Bit-Error Int. (if enabled)
Bus-Error Flag
Bus-Error Int. (if enabled)ID
PARITY
CHECKER
CHECKSUM
CALCULATORChecksum-Error Flag
Checksum-Error Int.
(if enabled)ID-Parity-Error
ID-Parity Error Flag
ID-Pairty Interrupt
(if enabled)
LIN www.ti.com
1654 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.8.1 BitErrors
Abiterror (BE) isdetected atthebittime when thebitvalue thatismonitored isdifferent from thebitvalue
thatissent. Abiterror isindicated bytheBEflaginSCIFLR. After signaling aBE,thetransmission is
aborted nolater than thenext byte. The bitmonitor ensures thatthetransmitted bitinLINTX isthecorrect
value ontheLINbusbyreading back ontheLINRX pinasshown inFigure 29-21 .
NOTE: IfBEOccurs duetoNew Header reception during aSlave Response, NRE/TIMEOUT flag
willnotbesetforthenew Frame.
Figure 29-21. TXRX Error Detector
29.3.1.8.2 Physical Bus Errors
APhysical Bus Error (PBE) hastobedetected byamaster ifnovalid message canbegenerated onthe
bus(Bus shorted toGND orVBAT). The bitmonitor detects aPBE during theheader transmission, ifno
Synch Break canbegenerated (forexample, because ofabusshortage toVBAT) orifnoSynch break
Delimiter canbegenerated (forexample, because ofabusshortage toGND). Once theSynch Break
Delimiter was validated, allother deviations between themonitored andthesent bitvalue areflagged as
BitErrors (BE) forthisframe.
29.3.1.8.3 IDParity Errors
Ifparity isenabled, anIDparity error (PE) isdetected ifanyofthetwoparity bitsofthesent IDbyte are
notequal tothecalculated parity onthereceiver node. The twoparity bitsaregenerated using the
following mixed parity algorithm.
(49)
IfanID-parity error isdetected, theID-parity error isflagged, andthereceived IDisnotvalid. See
Section 29.3.1.9 fordetails.

<!-- Page 1655 -->

1 to 8 Data FieldsChecksum
FieldResponse
+ INVERT Modulo-256 sumCheckbyteID FieldFrom
Header
1 to 8 Data FieldsChecksum
FieldResponse
+ INVERT Modulo-256 sumCheckbyte
www.ti.com LIN
1655 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.8.4 Checksum Errors
Achecksum error (CE) isdetected andflagged atthereceiving endifthecalculated modulo-256 sum over
allreceived data bytes (including theIDbyte ifitistheenhanced checksum type) plus thechecksum byte
does notresult in0xFF. The modulo-256 sum iscalculated over each byte byadding with carry, where the
carry bitofeach addition isadded totheLSB ofitsresulting sum.
Forthetransmitting node, thechecksum byte sent attheendofamessage istheinverted sum ofallthe
data bytes (see Figure 29-22 )forclassic checksum implementation. The checksum byte istheinverted
sum oftheidentifier byte andallthedata bytes (see Figure 29-23 )fortheLIN2.0compliant enhanced
checksum implementation. The classic checksum implementation should always beused forreserved
identifiers 60to63;therefore, theCTYPE bitwillbeoverridden inthiscase. Forsignal-carrying-frame
identifiers (0to59)thetype ofchecksum used depends ontheCTYPE bit.
Figure 29-22. Classic Checksum Generation atTransmitting Node
Figure 29-23. LIN2.0-Compliant Checksum Generation atTransmitting Node

<!-- Page 1656 -->

070 200)250 260 ( x x x x /c61 /c197 /c43
LIN www.ti.com
1656 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.9 Message Filtering andValidation
Message filtering uses theentire identifier todetermine which nodes willparticipate inaresponse, either
receiving ortransmitting aresponse. Therefore, twoacceptance masks areused asshown inFigure 29-
24.During header reception, allnodes filter theID-Field (ID-Field isthepart oftheheader explained in
Figure 29-16 )todetermine whether they transmit aresponse orreceive aresponse forthecurrent
message. There aretwomasks formessage IDfiltering: onetoaccept aresponse reception, theother to
initiate aresponse transmission. See Figure 29-24 .Allnodes compare thereceived IDtotheidentifier
stored intheID-SlaveTask BYTE oftheLINID register andusetheRXIDMASK andtheTXIDMASK
fields intheLINMASK register tofilter thebitsoftheidentifier thatshould notbecompared.
Ifthere isanRXmatch with noparity error andtheRXENA bitisset,there willbeanIDRXflagandan
interrupt willbetriggered ifenabled. Ifthere isaTXmatch with noparity error andtheTXENA bitisset,
there willbeanIDTXflagandaninterrupt willbetriggered ifenabled intheSCISETINT register.
The masked bitsbecome don'tcares forthecomparison. Tobuild amask forasetofidentifiers, anXOR
function could beused.
Forexample, tobuild amask toaccept IDs0x26 and0x25 using LINID[7:0] =0x20; thatis,compare 5
most significant bits(MSBs) andfilter 3least significant bits(LSBs), theacceptance mask could be:
(50)
Amask ofallzeros willcompare allbitsofthereceived identifier intheshift register with theID-BYTE in
LINID[7:0]. IfHGEN CTRL issetto1,amask of0xFF willalways cause amatch. Amask ofall1swill
filter allbitsofthereceived identifier, andthus there willbeanIDmatch regardless ofthecontent ofthe
ID-SlaveTask BYTE field intheLINID register.
NOTE: When theHGEN CTRL bit=0,theLINnodes compare thereceived IDtotheID-BYTE field
intheLINID register, andusetheRXIDMASK andtheTXIDMASK intheLINMASK
register tofilter thebitsoftheidentifier thatshould notbecompared.
Ifthere isanRXmatch with noparity error andtheRXENA bitisset,there willbeanIDRX
flagandaninterrupt willbetriggered ifenabled. Amask ofall0swillcompare allbitsofthe
received identifier intheshift register with theID-BYTE field inLINID[7:0]. Amask ofall1s
willfilter allbitsofthereceived identifier andthere willbenomatch.
During header reception, thereceived identifier iscopied totheReceived IDfield LINID[23:16]. Ifthere is
noparity error andthere iseither aTXmatch oranRXmatch, then thecorresponding TXorRXIDflagis
set.IftheIDinterrupt isenabled, then anIDinterrupt isgenerated.
After theIDinterrupt isgenerated, theCPU may read theReceived IDfield LINID[23:16] anddetermine
what response toload intothetransmit buffers.
NOTE: When byte 0iswritten toTD0 (LINTD0[31:24]), theresponse transmission isautomatically
generated.
Inmulti-buffer mode, theTXRDY flagwillbesetwhen alltheresponse data bytes andchecksum byte are
copied totheshift register SCITXSHF. Innon-multi-buffer mode, theTXRDY flagisseteach time abyte is
copied totheSCITXSHF register, andalso forthelastbyte oftheframe after thechecksum byte iscopied
totheSCITXSHF register.
Inmulti-buffer mode, theTXEMPTY flagissetwhen both thetransmit buffer(s) TDy andtheSCITXSHF
shift register areemptied andthechecksum hasbeen sent. Innonmulti-buffer mode, TXEMPTY isset
each time TD0 andSCITXSHF areemptied, except forthelastbyte oftheframe where thechecksum
byte must also betransmitted.
Ifparity isenabled, allslave receiving nodes willvalidate theidentifier using alleight bitsofthereceived
IDbyte. The SCI/LIN willflagacorrupted identifier ifanID-parity error isdetected.

<!-- Page 1657 -->

ID
INT23 L I NID 16
7 SCIRXSHF 0
7 RX ID M ask 0
7 I D-Byte 0
7 ID-Slave Task Byte 0
7 T X ID Mask 0
7 ID-Byte 0
7 ID-SlaveTaskByte 0ID
Parity
Checker
0
1
HGEN CTRL0
1=
=From
SCIRXSHFAND
AN DRXNo
ID-Parity
Error
RX
Match
TX
Matc hID-RX
Flag
ID- TX
Fla gParity Enable
ID Parity Error
No
ID-Parity
Error HGEN CTRLID
INT
0
10
1=
=FromAND
AN DRXNo
ID-Parity
Error RXENA
RX
Match
TX
Matc hID-RX
Flag
ID- TX
Fla gNo
ID-Parity
Error HGEN CTRL
www.ti.com LIN
1657 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleFigure 29-24. IDReception, Filtering andValidation

<!-- Page 1658 -->

LIN www.ti.com
1658 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.1.10 Receive Buffers
Toreduce CPU load when receiving aLINN-byte (with N=1-8)response ininterrupt mode orDMA
mode, theSCI/LIN module haseight receive buffers. These buffers canstore anentire LINresponse in
theRDy receive buffers. Figure 29-8 illustrates thereceive buffers.
The checksum byte following thedata bytes isvalidated bytheinternal checksum calculator. The
checksum error (CE) flagindicates achecksum error andaCEinterrupt willbegenerated ifenabled inthe
SCISETINT register.
The multi-buffer 3-bit counter counts thedata bytes transferred from theSCIRXSHF register totheRDy
receive buffers ifmulti-buffer mode isenabled, ortoRD0 ifmulti-buffer mode isdisabled. The 3-bit
compare register contains thenumber ofdata bytes expected tobereceived. Incases where theIDBYTE
field does notconvey message length (see Note:Optional Control Length BitsinSection 29.3.1.5 ),the
LENGTH value, indicates theexpected length andisused toload the3-bit compare register. Whether the
length control field ortheLENGTH value isused isselectable with theCOMM MODE bit.
Areceive interrupt, andareceive ready RXRDY flagsetaswell asaDMA request (RXDMA) could occur
after receiving aresponse ifthere arenoresponse receive errors fortheframe (such as,there isno
checksum error, frame error, andoverrun error). The checksum byte willbecompared before
acknowledging areception. ADMA request canbegenerated foreach received byte orfortheentire
response depending onwhether themulti-buffer mode isenabled ornot(MBUF MODE bit).
NOTE: Inmulti-buffer mode following arethescenarios associated with clearing the"RXRDY" flag
bit:
1.The RXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0/1 register.
2.ForLENGTH less than orequal to4,Read toRD0 register willclear the"RXRDY "flag.
3.ForLENGTH greater than 4,Read toRD1 register willclear the"RXRDY "flag.
29.3.1.11 Transmit Buffers
Toreduce theCPU load when transmitting aLINN-byte (with N=1-8)response ininterrupt mode or
DMA mode, theSCI/LIN module haseight transmit buffers, TD0-TD7 inLINTD0 andLINTD1. With these
transmit buffers, anentire LINresponse field canbepreloaded intheTXy transmit buffers. Optionally, a
DMA transfer could bedone onabyte-per-byte basis when multi-buffer mode isnotenabled (MBUF
MODE bit).Figure 29-9 illustrates thetransmit buffers.
The multi-buffer 3-bit counter counts thedata bytes transferred from theTDy transmit buffers register if
multi-buffer mode isenabled, orfrom TD0 toSCITXSHF ifmulti-buffer mode isdisabled. The 3-bit
compare register contains thenumber ofdata bytes expected tobetransmitted. IftheIDfield isnotused
toconvey message length (see Note:Optional Control Length BitsinSection 29.3.1.5 ),theLENGTH value
indicates theexpected length andisused instead toload the3-bit compare register. Whether thelength
control field ortheLENGTH value isused isselectable with theCOMM MODE bit.
Atransmit interrupt (TXinterrupt), andatransmit ready flag(TXRDY flag), andaDMA request (TXDMA)
could occur after transmitting aresponse. ADMA request canbegenerated foreach transmitted byte or
fortheentire response depending onwhether multi-buffer mode isenabled ornot(MBUF MODE bit).
The checksum byte willbeautomatically generated bythechecksum calculator andsent after thedata-
fields transmission isfinished. The multi-buffer 3-bit counter counts thedata bytes transferred from the
TDy buffers intotheSCITXSHF register.
NOTE: The transmit interrupt request canbeeliminated until thenext series ofdata iswritten into
thetransmit buffers LINTD0 andLINTD1, bydisabling thecorresponding interrupt viathe
SCICLRINT register orbydisabling thetransmitter viatheTXENA bit.

<!-- Page 1659 -->

Sync Break Sync Field ID Field 1, 2, 3, ...8 Data Fields ChecksumMaster Header Slave Response
In-frame spaceTFrameMax
TBus Idle (4s)
Parity Error Int.
ID InterruptNo Response Error Int. Timeout - Bus Idle (4s) Int.ISF Error Int.
Physical Bus
Error Int.(Master)FE FE
Checksum Error Int.
RX Int. (multi-buffer/
last byte single buffer)RX Int. (single buffer) RX Int. (single buffer)
Overrun Error Int.Bit Error Int.FE FE
TX Int. (single buffer) TX Int. (single buffer) TX Int. (single buffer) TX Int. (multi buffer/
last byte single buffer)Frame Error Int. (FE)
www.ti.com LIN
1659 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.2 LINInterrupts
LINandSCImode have acommon Interrupt block asexplained inSection 29.2.2 .There are16interrupt
sources intheSCI/LIN module, with 8ofthem being LINmode only, asseen inTable 29-4 .
ALINmessage frame indicating thetiming andsequence oftheLINinterrupts thatcould occur isshown in
Figure 29-25 .
Figure 29-25. LINMessage Frame Showing LINInterrupt Timing andSequence
29.3.3 LINDMA Interface
LINDMA Interface uses theSCIDMA interface logic. DMA requests forreceive (RXDMA request) and
transmit (TXDMA request) areavailable fortheSCI/LIN module. There aretwomodes forDMA transfers
depending onwhether multi-buffer mode isenabled ornotviathemulti-buffer enable control bit(MBUF
MODE).
29.3.3.1 LINReceive DMA Requests
InLINmode, when themulti-buffer option isenabled, ifareceived response (uptoeight data bytes) is
transferred tothereceive buffers (RDy), then aDMA request isgenerated. Ifthemulti-buffer option is
disabled, then DMA requests willbegenerated onabyte-per-byte basis until alltheexpected response
data fields arereceived. This DMA functionality isenabled anddisabled using theSET RXDMA andCLR
RXDMA bits, respectively.
29.3.3.2 LINTransmit DMA Requests
InLINmode with themulti-buffer option enabled, after atransmission (uptoeight data bytes stored inthe
transmit buffer(s) TDy intheLINTD0 andLINTD1 registers), aDMA request isgenerated inorder to
reload thetransmit buffer forthenext transmission. Ifthemulti-buffer option isdisabled, then DMA
requests willbegenerated onabyte-per-byte basis until allbytes aretransferred. This DMA functionality
isenabled anddisabled using theSET TXDMA andCLR TXDMA bits, respectively.

<!-- Page 1660 -->

LIN www.ti.com
1660 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.4 LINConfigurations
The following listdetails theconfiguration steps thatsoftware should perform prior tothetransmission or
reception ofdata inLINmode. Aslong astheSWnRST bitintheSCIGCR1 register iscleared to0the
entire time thattheLINisbeing configured, theorder inwhich theregisters areprogrammed isnot
important.
*Enable LINbysetting theRESET bitinSCIGCR0 to1.
*Clear theSWnRST bitto0before LINisconfigured.
*Select thedesired frame format byprogramming theSCIGCR1 register.
*Setboth theRXFUNC andTXFUNC bitsinSCIPIO0 to1toconfigure theLINRX andLINTX pins for
LINfunctionality.
*SettheLINMODE bitinSCIGCR1 to1toenable LINmode.
*Select Master orSlave mode byprogramming theCLOCK bitinSCIGCR1.
*SettheMBUF MODE bitinSCIGCR1 to1toselect multi-buffer mode.
*Select thebaud rate tobeused forcommunication byprogramming theBRS register.
*Setthemaximum baud rate tobeused forcommunication byprogramming theBMRS register.
*SettheCONT bitinSCIGCR1 to1tomake LINnothaltforanemulation breakpoint until itscurrent
reception ortransmission iscomplete (this bitisused only inanemulation environment).
*SettheLOOP BACK bitinSCIGCR1 to1toconnect thetransmitter tothereceiver internally (this
feature isused toperform aself-test).
*SettheRXENA bitinSCIGCR1 to1,ifdata istobereceived.
*SettheTXENA bitinSCIGCR1 to1,ifdata istobetransmitted.
*Select theRXIDMASK andtheTXIDMASK fields intheLINMASK register.
*SettheSWnRST bitto1after LINisconfigured.
*Perform receiving ortransmitting data (see Section 29.3.4.1 orSection 29.3.4.2 ).
29.3.4.1 Receiving Data
The LINreceiver isenabled toreceive messages ifboth theRXFUNC bitandtheRXENA bitaresetto1.
IftheRXFUNC bitisnotset,theLINRX pinfunctions asageneral-purpose I/Opinrather than asaLIN
function pin.
The IDRXFLAG issetafter avalid LINIDisreceived with RXMatch. AnIDinterrupt isgenerated, if
enabled.
29.3.4.1.1 Receiving Data inSingle-Buffer Mode
Single-buffer mode isselected when theMBUF MODE bitiscleared to0.Inthismode, LINsets the
RXRDY bitwhen ittransfers newly received data from SCIRXSHF toRD0. The SCIclears theRXRDY bit
after thenew data inRD0 hasbeen read. Also, asdata istransferred from SCIRXSHF toRD0, theLIN
sets theFE,OE, orPEflags ifanyofthese error conditions were detected inthereceived data. These
error conditions aresupported with configurable interrupt capability.
You canreceive data by:
1.Polling Receive Ready Flag
2.Receive Interrupt
3.DMA
Inpolling method, software canpollfortheRXRDY bitandread thedata from RD0 byte oftheLINRD0
register once theRXRDY bitissethigh. The CPU isunnecessarily overloaded byselecting thepolling
method. Toavoid this, youcanuseeither theinterrupt orDMA method. Tousetheinterrupt method, the
SET RXINTbitisset.TousetheDMA method, theSET RXDMA bitisset.Either aninterrupt oraDMA
request isgenerated themoment theRXRDY bitisset.Ifthechecksum scheme isenabled bysetting the
Compare Checksum (CC) bitto1,thechecksum willbecompared onthebyte thatiscurrently being
received, which isexpected tobethechecksum byte. The CCbitwillbecleared once thechecksum is
received. ACEwillimmediately beflagged ifthere isachecksum error.

<!-- Page 1661 -->

www.ti.com LIN
1661 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.4.1.2 Receiving Data inMulti-Buffer Mode
Multi-buffer mode isselected when theMBUF MODE bitissetto1.Inthismode, LINsets theRXRDY bit
after receiving theprogrammed number ofdata inthereceive buffer andthechecksum field, thecomplete
frame. The error condition detection logic issimilar tothesingle-buffer mode, except thatitmonitors for
thecomplete frame. Like single-buffer mode, youcanusethepolling, interrupt, orDMA method toread
thedata. The received data hastoberead from theLINRD0 andLINRD1 registers, based onthenumber
ofbytes. ForaLENGTH less than orequal to4,aread from theLINRD0 register clears theRXRDY flag.
ForaLENGTH greater than 4,aread from theLINRD1 register clears theRXRDY flag. Ifthechecksum
scheme isenabled bysetting theCompare Checksum (CC) bitto1during thereception ofthedata, then
thebyte thatisreceived after thereception oftheprogrammed number ofdata bytes indicated bythe
LENGTH field istreated asachecksum byte. The CCbitwillbecleared once thechecksum isreceived
andcompared.
29.3.4.2 Transmitting Data
The LINtransmitter isenabled ifboth theTXFUNC bitandtheTXENA bitaresetto1.IftheTXFUNC bit
isnotset,theLINTX pinfunctions asageneral-purpose I/Opinrather than asaLINfunction pin.Any
value written totheTD0 before theTXENA bitissetto1isnottransmitted. Both ofthese control bitsallow
fortheLINtransmitter tobeheld inactive independently ofthereceiver.
The IDTXflagissetafter avalid LINIDisreceived with TXMatch. AnIDinterrupt isgenerated, if
enabled.
29.3.4.2.1 Transmitting Data inSingle-Buffer Mode
Single-buffer mode isselected when theMBUF MODE bitiscleared to0.Inthismode, LINwaits fordata
tobewritten toTD0, transfers ittoSCITXSHF, andtransmits thedata. The TXRDY andTXEMPTY bits
indicate thestatus ofthetransmit buffers. That is,when thetransmitter isready fordata tobewritten to
TD0, theTXRDY bitisset.Additionally, ifboth TD0 andSCITXSHF areempty, then theTXEMPTY bitis
also set.
You cantransmit data by:
1.Polling Transmit Ready Flag
2.Transmit Interrupt
3.DMA
Inpolling method, software canpollfortheTXRDY bittogohigh before writing thedata totheTD0. The
CPU isunnecessarily overloaded byselecting thepolling method. Toavoid this, youcanuseeither the
interrupt orDMA method. Tousetheinterrupt method, theSET TXINTbitisset.TousetheDMA
method, theSET TXDMA bitisset.Either aninterrupt oraDMA request isgenerated themoment the
TXRDY bitisset.When theLINhascompleted transmission ofallpending frames, theSCITXSHF register
andtheTD0 areempty, theTXRDY bitisset,andaninterrupt/DMA request isgenerated, ifenabled.
Because alldata hasbeen transmitted, theinterrupt/DMA request should behalted. This caneither be
done bydisabling thetransmit interrupt (CLR TXINT) /DMA request (CLR TXDMA bit)orbydisabling
thetransmitter (clear TXENA bit).Ifthechecksum scheme isenabled bysetting theSend Checksum (SC)
bitto1,thechecksum byte willbesent after thecurrent byte transmission. The SCbitwillbecleared after
thechecksum byte hasbeen transmitted.
NOTE: The TXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0 orSCIINTVECT1 register.

<!-- Page 1662 -->

Low-Power Mode www.ti.com
1662 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.3.4.2.2 Transmitting Data inMulti-Buffer Mode
Multi-buffer mode isselected when theMBUF MODE bitissetto1.Like single-buffer mode, youcanuse
thepolling, interrupt, orDMA method towrite thedata tobetransmitted. The transmitted data hastobe
written totheLINTD0 andLINTD1 registers, based onthenumber ofbytes. LINwaits fordata tobe
written toByte 0(TD0) oftheLINTD0 register andtransfers theprogrammed number ofbytes to
SCITXSHF totransmit onebyoneautomatically. Ifthechecksum scheme isenabled bysetting theSend
Checksum (SC) bitto1,thechecksum willbesent after transmission ofthelastbyte oftheprogrammed
number ofdata bytes, indicated bytheLENGTH field. The SCbitwillbecleared after thechecksum byte
hasbeen transmitted.
29.4 Low-Power Mode
The SCI/LIN module canbeputineither local orglobal low-power mode. Global low-power mode is
asserted bythesystem andisnotcontrolled bytheSCI/LIN module. During global low-power mode, all
clocks totheSCI/LIN areturned offsothemodule iscompletely inactive. Ifglobal low-power mode is
requested while thereceiver isreceiving data, then theSCI/BLIN completes thecurrent reception and
then enters thelow-power mode, thatis,module enters low-power mode only when Busy bit(SCIFLR.3) is
cleared.
The BLIN module may enter low-power mode either when there was noactivity ontheLINRX pinformore
than 4s(this canbeeither aconstant recessive ordominant level) orwhen aSleep Command frame was
received. Once theTimeout flag(SCIFLR.4) was setoronce aSleep Command was received, the
POWERDOWN bit(SCIGCR2.0) must besetbytheapplication software tomake themodule enter local
low-power mode. Awakeup signal willterminate thesleep mode oftheLINbus.
NOTE: Enabling Local Low-Power Mode During Receive andTransmit
Ifthewakeup interrupt isenabled andlow-power mode isrequested while thereceiver is
receiving data, then theSCI/LIN immediately generates awake-up interrupt toclear the
powerdown bit.Thus, theSCI/LIN isprevented from entering low-power mode and
completes thecurrent reception. Otherwise, ifthewakeup interrupt isdisabled, theSCI/LIN
completes thecurrent reception andthen enters thelow-power mode.
29.4.1 Entering Sleep Mode
InLINprotocol, asleep command isused tobroadcast thesleep mode toallnodes. The sleep command
consists ofadiagnostic master request frame with identifier 0x3C (60), with thefirstdata field as0x00.
There should benoactivity inthebusonce allnodes receive thesleep command: thebusisinsleep
mode.
Local low-power mode isasserted bysetting thePOWERDOWN bit;setting thisbitstops theclocks tothe
SCI/LIN internal logic andregisters. Clearing thePOWERDOWN bitcauses SCI/LIN toexitfrom local low-
power mode. Alltheregisters areaccessible during local power-down mode. Ifaregister isaccessed in
low-power mode, thisaccess results inenabling theclock tothemodule forthatparticular access alone.

<!-- Page 1663 -->

0.25ms T 5ms ≤ ≤WUSIG
WAKEUP SIGNAL
BUS IN SLEEP MODE
TWUSIGSYNCH BREAK
TINITIALIZEBUS IN
OPERATIONAL MODE
www.ti.com Low-Power Mode
1663 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.4.2 Wakeup
The wakeup interrupt isused toallow theSCI/LIN module toautomatically exitlow-power mode. A
SCI/LIN wakeup istriggered when alowlevel isdetected onthereceive RXpin,andthisclears the
POWERDOWN bit.
NOTE: Ifthewakeup interrupt isdisabled then theSCI/LIN enters low-power mode whenever itis
requested todoso,butalowlevel onthereceive RXpindoes NOT cause theSCI/LIN to
exitlow-power mode.
InLINmode, anynode canterminate sleep mode bysending awakeup signal; seeFigure 29-26 .Aslave
node thatdetects thebusinsleep mode, andwith awakeup request pending, willsend awakeup signal.
The wakeup signal isadominant value ontheLINbusforTWUSIG ;thisisatleast 5TbitsfortheLINbus
baud rates. The wakeup signal isgenerated bysending an0xF0 byte containing 5dominant Tbitsand5
recessive Tbits.
Figure 29-26. Wakeup Signal Generation
(51)
Assuming aperfect buswith nonoise orloading effects, awrite of0xF0 toTD0 willload thetransmitter to
meet thewakeup signal timing requirement forTWUSIG .Then, setting theGENWU bitwilltransmit the
preloaded value inTD0 forawakeup signal transmission.
NOTE: The GENWU bitcanbeset/reset only when SWnRST issetto'1'andthenode isinpower
down mode. The bitwillbecleared onavalid synch break detection. Amaster sending a
wakeup request, willexitpower down mode upon reception ofthewakeup pulse. The bitwill
becleared onaSWnRST. This canbeused tostop amaster from sending further wakeup
requests.
The TITPIC1021 LINtransceiver, upon receiving awakeup signal, willtranslate ittothemicrocontroller
forwakeup with adominant level ontheRXpin,orasignal tothevoltage regulator. While the
POWERDOWN bitisset,iftheLINmodule detects arecessive-to-dominant edge (falling edge) ontheRX
pin,itwillgenerate awakeup interrupt ifenabled intheSCISETINT register.
According toLINprotocol 2.0,theTITPIC1021 LINtransceiver detecting adominant level onthebus
longer than 150mswilldetect itasawakeup request. The LINcontroller 'sslave isready tolisten tothe
businless than 100ms(TINITIALIZE <100ms) after adominant-to-recessive edge (end-of-wakeup signal).

<!-- Page 1664 -->

Low-Power Mode www.ti.com
1664 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.4.3 Wakeup Timeouts
The LINprotocol defines thefollowing timeouts forawakeup sequence. After awakeup signal hasbeen
sent tothebus, allnodes wait forthemaster tosend aheader. Ifnosynch field isdetected before 150ms
(3,000 cycles at20kHz) after wakeup signal istransmitted, anew wakeup issent bythesame node that
requested thefirstwakeup. This sequence isnotrepeated more than twotimes. After three attempts to
wake uptheLINbus, wakeup signal generation issuspended fora1.5s(30,000 cycles at20kHz) period
after three breaks.
NOTE: Toachieve compatibility toLIN1.3 timeout conditions, theMBRS register must besetto
assure thattheLIN2.0(real-time-based) timings meet theLIN1.3bittime base. Anode
triggering thewakeup should settheMBRS register accordingly tomeet thetargeted time as
128Tbits ×programmed prescaler.
The LINcontroller handles thewakeup expiration times defined bytheLINprotocol with a
hardware implementation.
29.5 Emulation Mode
Inemulation mode, theCONT bitdetermines how theSCI/LIN operates when theprogram issuspended.
The SCI/LIN counters areaffected bythisbitduring debug mode. when set,thecounters arenotstopped
andwhen cleared, thecounters arestopped debug mode.
Any reads inemulation mode toaSCI/LIN register willnothave anyeffect ontheflags intheSCIFLR
register.
NOTE: When emulation mode isentered during theFrame transmission orreception oftheframe
andCONT bitisnotset,Communication isnotexpected tobesuccessful. The suggested
usage istosetCONT bitduring emulation mode forsuccessful communication.

<!-- Page 1665 -->

Output enable
Data out
Data in
Pull control disable
Pull selectDevice pin
Pull control
logicInput enable
Output enable
Data out
Data in
Pull control disableDevice pin
Pull control
logicInput enable
www.ti.com GPIO Functionality
1665 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.6 GPIO Functionality
The following section applies toalldevice pins thatcanbeconfigured asfunctional orgeneral-purpose I/O
pins.
29.6.1 GPIO Functionality
Figure 29-27 illustrates theGPIO functionality.
Figure 29-27. GPIO Functionality
29.6.2 Under Reset
The following apply ifadevice isunder reset:
*Pullcontrol. The reset pullcontrol onthepins isenabled.
*Input buffer. The input buffer isenabled.
*Output buffer. The output buffer isdisabled.

<!-- Page 1666 -->

GPIO Functionality www.ti.com
1666 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.6.3 OutofReset
The following apply ifthedevice isoutofreset:
*Pullcontrol. The pullcontrol isenabled byclearing thePD(pull control disable) bitintheSCIPIO7
register (Section 29.7.21 ).Inthiscase, ifthePSL (pull select) bitintheSCIPIO8 register
(Section 29.7.22 )isset,thepinwillhave apull-up. IfthePSL bitiscleared, thepinwillhave apull-
down. IfthePDbitissetinthecontrol register, there isnopull-up orpull-down onthepin.
*Input buffer. The input buffer ispermanently enabled inthisdevice.
NOTE: The pull-disable logic depends onthepindirection. Itisindependent ofwhether thedevice is
inI/Oorfunctional mode. Ifthepinisconfigured asoutput ortransmit, then thepulls are
disabled automatically.
*Output buffer. Apincanbedriven asanoutput piniftheTXDIR bitissetinthepindirection control
register (SCIPIO1; Section 29.7.15 )AND theopen-drain feature isnotenabled intheSCIPIO6 register
(Section 29.7.20 ).
29.6.4 Open-Drain Feature Enabled onaPin
The following apply iftheopen-drain feature isenabled onapin:
*The output buffer isenabled ifalowsignal isbeing driven ontothepin.
*The output buffer isdisabled (the direction control signal DIR isinternally forced low) ifahigh signal is
being driven ontothepin.
NOTE: The open-drain feature isavailable only inI/Omode (SCIPIO0; Section 29.7.14 ).
29.6.5 Summary
The behavior oftheinput buffer, output buffer, andthepullcontrol issummarized inTable 29-9 .
(1)X=Don'tcare
(2)DIR =0forinput, =1foroutput
(3)PULDIS =0forenabling pullcontrol, =1fordisabling pullcontrol
(4)PULSEL= 0forpull-down functionality, =1forpull-up functionalityTable 29-9. Input Buffer, Output Buffer, andPullControl Behavior asGPIO Pins(1)
Device
under
Reset?PinDirection
(DIR)(2)PullDisable
(PULDIS)(3)PullSelect
(PULSEL)(4)PullControl Output Buffer Input Buffer
Yes X X X Enabled Disabled Enabled
No 0 0 0 Pulldown Disabled Enabled
No 0 0 1 Pullup Disabled Enabled
No 0 1 0 Disabled Disabled Enabled
No 0 1 1 Disabled Disabled Enabled
No 1 X X Disabled Enabled Enabled

<!-- Page 1667 -->

www.ti.com SCI/LIN Control Registers
1667 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7 SCI/LIN Control Registers
The SCI/LIN module registers arebased ontheSCIregisters, with added functionality registers enabled
bytheLINMODE bitintheSCIGCR1 register.
These registers areaccessible in8-,16-, and32-bit reads orwrites. The SCI/LIN iscontrolled and
accessed through theregisters listed inTable 29-10 .Among thefeatures thatcanbeprogrammed arethe
LINprotocol mode, communication andtiming modes, baud rate value, frame format, DMA requests, and
interrupt configuration. The base address forthecontrol registers isFFF7 E400h forLIN1/SCI1 and
FFF7 E600h forLIN2/SCI2.
Table 29-10. SCI/LIN Control Registers
Offset Acronym Register Description Section
00h SCIGCR0 SCIGlobal Control Register 0 Section 29.7.1
04h SCIGCR1 SCIGlobal Control Register 1 Section 29.7.2
08h SCIGCR2 SCIGlobal Control Register 2 Section 29.7.3
0Ch SCISETINT SCISetInterrupt Register Section 29.7.4
10h SCICLEARINT SCIClear Interrupt Register Section 29.7.5
14h SCISETINTLVL SCISetInterrupt Level Register Section 29.7.6
18h SCICLEARINTLVL SCIClear Interrupt Level Register Section 29.7.7
1Ch SCIFLR SCIFlags Register Section 29.7.8
20h SCIINTVECT0 SCIInterrupt Vector Offset 0 Section 29.7.9
24h SCIINTVECT1 SCIInterrupt Vector Offset 1 Section 29.7.10
28h SCIFORMAT SCIFormat Control Register Section 29.7.11
2Ch BRS Baud Rate Selection Register Section 29.7.12
30h SCIED Receiver Emulation Data Buffer Section 29.7.13.1
34h SCIRD Receiver Data Buffer Section 29.7.13.2
38h SCITD Transmit Data Buffer Section 29.7.13.3
3Ch SCIPIO0 SCIPinI/OControl Register 0 Section 29.7.14
40h SCIPIO1 SCIPinI/OControl Register 1 Section 29.7.15
44h SCIPIO2 SCIPinI/OControl Register 2 Section 29.7.16
48h SCIPIO3 SCIPinI/OControl Register 3 Section 29.7.17
4Ch SCIPIO4 SCIPinI/OControl Register 4 Section 29.7.18
50h SCIPIO5 SCIPinI/OControl Register 5 Section 29.7.19
54h SCIPIO6 SCIPinI/OControl Register 6 Section 29.7.20
58h SCIPIO7 SCIPinI/OControl Register 7 Section 29.7.21
5Ch SCIPIO8 SCIPinI/OControl Register 8 Section 29.7.22
60h LINCOMPARE LINCompare Register Section 29.7.23
64h LINRD0 LINReceive Buffer 0Register Section 29.7.24
68h LINRD1 LINReceive Buffer 1Register Section 29.7.25
6Ch LINMASK LINMask Register Section 29.7.26
70h LINID LINIdentification Register Section 29.7.27
74h LINTD0 LINTransmit Buffer 0 Section 29.7.28
78h LINTD1 LINTransmit Buffer 1 Section 29.7.29
7Ch MBRS Maximum Baud Rate Selection Register Section 29.7.30
90h IODFTCTRL Input/Output Error Enable Register Section 29.7.31

<!-- Page 1668 -->

SCI/LIN Control Registers www.ti.com
1668 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.1 SCIGlobal Control Register 0(SCIGCR0)
The SCIGCR0 register defines themodule reset. Figure 29-28 andTable 29-11 illustrate thisregister.
Figure 29-28. SCIGlobal Control Register 0(SCIGCR0) (offset =00)
31 16
Reserved
R-0
15 1 0
Reserved RESET
R-0 R/WP-0
LEGEND: R=Read only; R/WP =Read/Write inprivileged mode only; -n=value after reset
Table 29-11. SCIGlobal Control Register 0(SCIGCR0) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RESET This bitresets theSCI/LIN module. This bitiseffective inSCIandLINmode.
0 SCI/LIN module isinreset.
1 SCI/LIN module isoutofreset.
Note: Read/Write inprivileged mode only.

<!-- Page 1669 -->

www.ti.com SCI/LIN Control Registers
1669 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.2 SCIGlobal Control Register 1(SCIGCR1)
The SCIGCR1 register defines theframe format, protocol, andcommunication mode used bytheSCI.
Figure 29-29 andTable 29-12 illustrate thisregister.
Figure 29-29. SCIGlobal Control Register 1(SCIGCR1) (offset =04h)
31 26 25 24
Reserved TXENA RXENA
R-0 R/W-0 R/W-0
23 18 17 16
Reserved CONT LOOP BACK
R-0 R/W-0 R/W-0
15 14 13 12 11 10 9 8
ReservedSTOP EXT
FRAMEHGEN CTRL CTYPE MBUF MODE ADAPT SLEEP
R-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/WL-0 R/W-0
7 6 5 4 3 2 1 0
SWnRST LINMODE CLOCK STOP PARITY PARITY ENA TIMING MODE COMM MODE
R/W-0 R/W-0 R/W-0 R/WC-0 R/WC-0 R/W-0 R/WC-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-12. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25 TXENA Transmit enable. This bitiseffective inLINandSCImodes. Data istransferred from SCITD, or
theTDy (with y=0, 1,...7) buffers inLINmode totheSCITXSHF shift outregister only when the
TXENA bitisset.
0 Transfers from SCITD orTDy toSCITXSHF aredisabled.
1 Transfers from SCITD orTDy toSCITXSHF areenabled.
Note: Data written toSCITD orthetransmit multi-buffer before TXENA issetisnot
transmitted. IfTXENA iscleared while transmission isongoing, thedata previously
written toSCITD issent (including thechecksum byte inLINmode).
24 RXENA Receive enable. This bitiseffective inLINandSCImodes. RXENA allows orprevents the
transfer ofdata from SCIRXSHF toSCIRD orthereceive multi-buffers.
0 The receiver willnottransfer data from theshift buffer tothereceive buffer ormulti-buffers.
1 The receiver willtransfer data from theshift buffer tothereceive buffer ormulti-buffers.
Note: Clearing RXENA stops received characters from being transferred intothereceive
buffer ormulti-buffers, prevents theRXstatus flags from being updated byreceive data,
andinhibits both receive anderror interrupts. However, theshift register continues to
assemble data regardless ofthestate ofRXENA.
Note: IfRXENA iscleared before aframe iscompletely received, thedata from theframe
isnottransferred intothereceive buffer.
Note: IfRXENA issetbefore aframe iscompletely received, thedata from theframe is
transferred intothereceive buffer. IfRXENA issetwhile SCIRXSHF isintheprocess of
assembling aframe, thestatus flags arenotguaranteed tobeaccurate forthatframe. To
ensure thatthestatus flags correctly reflect what was detected onthebusduring a
particular frame, RXENA should besetbefore thedetection ofthatframe.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 CONT Continue onsuspend. This bitiseffective inLINandSCImodes. This bithasaneffect only
when aprogram isbeing debugged with anemulator, anditdetermines how theSCI/LIN
operates when theprogram issuspended. The SCI/LIN counters areaffected bythisbit:when
thebitissetthecounters arenotstopped, when thebitiscleared thecounters arestopped
during debug mode.
0 When debug mode isentered, theSCI/LIN state machine isfrozen. Transmissions andLIN
counters arehalted andresume when debug mode isexited.
1 When debug mode isentered, theSCI/LIN continues tooperate until thecurrent transmit and
receive functions arecomplete.

<!-- Page 1670 -->

SCI/LIN Control Registers www.ti.com
1670 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-12. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions (continued)
Bit Field Value Description
16 LOOP BACK Loopback bit.This bitiseffective inLINandSCImodes. The self-checking option forthe
SCI/LIN canbeselected with thisbit.IftheLINITX andLINRX pins areconfigured with SCI/LIN
functionality, then theLINTX pinisinternally connected totheLINRX pin.Externally, during loop
back operation, theLINTX pinoutputs ahigh value andtheLINRX pinisinahigh-impedance
state. Ifthisbitvalue ischanged while theSCI/LIN istransmitting orreceiving data, errors may
result.
0 Loop back mode isdisabled.
1 Loop back mode isenabled.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13 STOP EXT FRAME Stop extended frame communication. This bitiseffective inLINmode only. This bitcanbe
written only during extended frame communication. When theextended frame communication is
stopped, thisbitiscleared automatically.
0 This bithasnoeffect.
1 Extended frame communication willbestopped when current frame transmission/reception is
completed.
12 HGEN CTRL HGEN control. This bitiseffective inLINmode only. This bitcontrols thetype ofmask filtering
comparison.
0 IDfiltering using theID-BYTE field inLINIdentification Register (LINID) occurs.
Mask ofFFh inLINMask Register (LINMASK) register willresult innomatch.
1 IDfiltering uses ID-SlaveTask BYTE (recommended).
Mask ofFFh inLINMask Register (LINMASK) register willresult inALWAYS match.
Note: Forsoftware compatibility with future LINmodules theHGEN CTRL bitmust beset
to1,theRXIDMASK must besettoFFhandtheTXIDMASK must besettoFFh.
11 CTYPE Checksum type. This bitiseffective inLINmode only. This bitcontrols thetype ofchecksum to
beused: classic orenhanced.
0 Classic checksum isused.
1 Enhanced checksum isused.
10 MBUF MODE Multi-buffer mode. This bitiseffective inLINandSCImodes. This bitcontrols receive/transmit
buffer usage, thatis,whether theRX/TX multi-buffers areused orasingle register, RD0/TD0, is
used.
0 The multi-buffer mode isdisabled.
1 The multi-buffer mode isenabled.
9 ADAPT Adapt. This mode iseffective inLINmode only. This bithasaneffect during thedetection ofthe
synch field. Two LINprotocol bitrate modes could beenabled with thisbitaccording tothe
node capability filedefinition: automatic orselect. The software andnetwork configuration will
decide which ofthese twomodes areenabled. When thisbitiscleared, theLIN2.0protocol
fixed bitrate should beused. IftheADAPT bitisset,aSCI/LIN slave node detecting thebaud
rate willcompare ittotheprescalers inBRS register andupdate itifthey aredifferent. The BRS
register willbeupdated with thenew value. Ifthisbitisnotsetthere willbenoadjustment to
theBRS register.
0 Automatic baud rate adjustment isdisabled.
1 Automatic baud rate adjustment isenabled.
8 SLEEP SCIsleep. This bitiseffective inSCImode only. Inamultiprocessor configuration, thisbit
controls thereceive sleep function. Clearing thisbitbrings theSCI/LIN outofsleep mode.
0 Sleep mode isdisabled.
1 Sleep mode isenabled.
Note: Thereceiver stilloperates when theSLEEP bitisset;however, RXRDY isupdated
andSCIRD isloaded with new data only when anaddress frame isdetected. The
remaining receiver status flags areupdated andanerror interrupt isrequested ifthe
corresponding interrupt enable bitisset,regardless ofthevalue oftheSLEEP bit.Inthis
way, ifanerror isdetected onthereceive data linewhile theSCIisasleep, software can
promptly deal with theerror condition.
Note: TheSLEEP bitisnotautomatically cleared when anaddress byte isdetected.
See Section 29.2.5.1 formore information onusing theSLEEP bitformultiprocessor
communication.

<!-- Page 1671 -->

www.ti.com SCI/LIN Control Registers
1671 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-12. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions (continued)
Bit Field Value Description
7 SWnRST Software reset (active low). This bitiseffective inLINandSCImodes.
0 The SCI/LIN isinitsreset state; nodata willbetransmitted orreceived. Writing a0tothisbit
initializes theSCI/LIN state machines andoperating flags asdefined inTable 29-13 and
Table 29-14 .Allaffected logic isheld inthereset state until a1iswritten tothisbit.
1 The SCI/LIN isinitsready state; transmission andreception canbedone. After thisbitissetto
1,theconfiguration ofthemodule should notchange.
Note: TheSCI/LIN should only beconfigured while SWnRST =0.
6 LINMODE LINmode. This bitiseffective inLINandSCImode. This bitcontrols themodule mode of
operation.
0 LINmode isdisabled; SCImode isenabled.
1 LINmode isenabled; SCImode isdisabled.
5 CLOCK SCIinternal clock enable. The CLOCK bitdetermines thesource ofthemodule clock onthe
SCICLK pin.Italso determines whether aLINnode isaslave ormaster.
SCImode:
0 The external SCICLK istheclock source.
Note: Ifanexternal clock isselected, then theinternal baud rategenerator andbaud rate
registers arebypassed. Themaximum frequency allowed foranexternally sourced SCI
clock isVCLK/16.
1 The internal SCICLK istheclock source.
LINmode:
0 The node isinslave mode.
1 The node isinmaster mode.
4 STOP SCInumber ofstop bitsperframe. This bitiseffective inSCImode only.
0 One stop bitisused.
1 Two stop bitsareused.
Note: Thereceiver checks foronly onestop bit.However inidle-line mode, thereceiver
waits until theendofthesecond stop bit(ifSTOP =1)tobegin checking foranidle
period .
3 PARITY SCIparity odd/even selection. This bitiseffective inSCImode only. IfthePARITY ENA bitis
set,PARITY designates oddoreven parity.
0 Odd parity isused.
1 Even parity isused.
Theparity bitiscalculated based onthedata bitsineach frame andtheaddress bit(in
address-bit mode). Thestart andstop fields intheframe arenotincluded intheparity
calculation.
Foroddparity, theSCItransmits andexpects toreceive avalue intheparity bitthat
makes oddthetotal number ofbitsintheframe with thevalue of1.
Foreven parity, theSCItransmits andexpects toreceive avalue intheparity bitthat
makes even thetotal number ofbitsintheframe with thevalue of1.
2 PARITY ENA Parity enable. This bitenables ordisables theparity function.
SCIorbuffered SCImode:
0 Parity isdisabled. Noparity bitisgenerated during transmission orisexpected during reception.
1 Parity isenabled. Aparity bitisgenerated during transmission andisexpected during reception.
LINmode:
0 IDfield parity verification isdisabled.
1 IDfield parity verification isenabled.
1 TIMING MODE SCItiming mode bit.This bitiseffective inSCImode only. itselects theSCItiming mode.
0 Synchronous timing isused.
1 Asynchronous timing isused.

<!-- Page 1672 -->

SCI/LIN Control Registers www.ti.com
1672 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-12. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions (continued)
Bit Field Value Description
0 COMM MODE SCI/LIN communication mode bit.Incompatibility mode itselects theSCIcommunication mode.
InLINmode itselects length control option forID-field bitsID4andID5.
SCImode:
0 Idle-line mode isused.
1 Address-bit mode isused.
LINmode:
0 ID4andID5arenotused forlength control.
1 ID4andID5areused forlength control.
(1)The flags arefrozen with their reset value while SWnRST =0.Table 29-13. SCIReceiver Status Flags
SCIFlag Register Bit Value After Reset(1)
CE SCIFLR 29 0
ISFE SCIFLR 28 0
NRE SCIFLR 27 0
FE SCIFLR 26 0
OE SCIFLR 25 0
PE SCIFLR 24 0
RXWAKE SCIFLR 12 0
RXRDY SCIFLR 9 0
BUSY SCIFLR 3 0
IDLE SCIFLR 2 0
WAKE UP SCIFLR 1 0
BRKDT SCIFLR 0 0
(1)The flags arefrozen with their reset value while SWnRST =0.Table 29-14. SCITransmitter Status Flags
SCIFlag Register Bit Value After Reset(1)
BE SCIFLR 31 0
PBE SCIFLR 30 0
TXWAKE SCIFLR 10 0
TXEMPTY SCIFLR 11 1
TXRDY SCIFLR 8 1

<!-- Page 1673 -->

www.ti.com SCI/LIN Control Registers
1673 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.3 SCIGlobal Control Register 2(SCIGCR2)
The SCIGCR2 register isused tosend orcompare achecksum byte during extended frames, togenerate
awakeup andforlow-power mode control oftheLINmodule. Figure 29-30 andTable 29-15 illustrate this
register.
Figure 29-30. SCIGlobal Control Register 2(SCIGCR2) (offset =08h)
31 18 17 16
Reserved CC SC
R-0 R/WL-0 R/WL-0
15 9 8 7 1 0
Reserved GEN WU Reserved POWERDOWN
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; -n=value after reset
Table 29-15. SCIGlobal Control Register 2(SCIGCR2) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17 CC Compare checksum. LINmode only. This bitisused bythereceiver forextended frames totrigger
achecksum compare. You initiate thistransaction bywriting aonetothisbit.CCbithastobeset
only after RX_RDY flagissetforthelastreceived data.
Innon-multi-buffer mode, when theCCbitisset,thechecksum willbecompared onthebyte thatis
expected tobethechecksum byte.
During multi-buffer mode, thefollowing scenarios areassociated with theCCbit:
a)IftheCCbitissetduring thereception ofthedata, then thebyte thatisreceived after the
reception oftheprogrammed number ofdata bytes asindicated bySCIFORMAT[18:16] istreated
asachecksum byte.
b)IftheCCbitissetduring theidleperiod (that is,during theinter-frame space), then the
immediate next byte willbetreated asachecksum byte.
c)CCbitwillbeauto cleared after thecheckbyte hasbeen received andcompared. Checksum
reception isnotguaranteed ifCCbitiswrite cleared bysoftware during thechecksum reception.
See Section 29.3.1.6 formore details.
0 Nochecksum compare willoccur.
1 Compare checksum onexpected checksum byte.
16 SC Send checksum byte. This bitiseffective inLINmode only. This bitisused bythetransmitter with
extended frames tosend achecksum byte. Innon-multi-buffer mode, thechecksum byte willbe
sent after thecurrent byte transmission. Inmulti-buffer mode, thechecksum byte willbesent after
thelastbyte count, indicated bytheSCIFORMAT[18:16]). See Section 29.3.1.6 formore details.
This byte willbecleared after thechecksum byte hasbeen transmitted.
Innon-multi-buffer mode, thechecksum byte willbesent after thecurrent byte transmission.
During multi-buffer mode, thefollowing scenarios areassociated with theSCbit:
a)The checkbyte willbesent after thelastbyte count, indicated bytheSCIFORMAT[18:16]).
b)Checksum willnotbesent ifSCissetbefore transmitting thevery firstbyte(that is,during
interframe space).
c)SCbitwillbeauto cleared after thecheckbyte hasbeen transmitted. Checksum transmission is
notguaranteed ifSCbitiswrite cleared bysoftware during thechecksum transmission. See
Section 29.3.1.6 formore details.
0 Nochecksum byte willbesent.
1 Achecksum byte willbesent.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 GEN WU Generate wakeup signal. This bitiseffective inLINmode only. This bitcontrols thegeneration ofa
wakeup signal, bytransmitting theTDO buffer value. The LINprotocol specifies thatthissignal
should beadominant forTWUSIG .This bitiscleared onreception ofavalid synch break.
0 Nowakeup signal willbegenerated.
1 The TDO buffer value willbetransmitted forawakeup signal. The bitwillbecleared onaSWnRST.
7-1 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1674 -->

SCI/LIN Control Registers www.ti.com
1674 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-15. SCIGlobal Control Register 2(SCIGCR2) Field Descriptions (continued)
Bit Field Value Description
0 POWERDOWN Power down. This bitiseffective inLINorSCImode. When thisbitisset,theSCI/LIN module
attempts toenter local low-power mode. IfthePOWERDOWN bitissetwhile thereceiver isactively
receiving data andthewakeup interrupt isdisabled, then theSCI/LIN willdelay entering low-power
mode until thereception iscompleted. InLINmode, youmay setthePOWERDOWN bitafter
receiving asleep command oronidlebusdetection (more than 4seconds). See Section 29.4 for
more information onlow-power mode.
0 The SCI/LIN module isinnormal operation.
1 The SCI/LIN module enters local low-power mode.

<!-- Page 1675 -->

www.ti.com SCI/LIN Control Registers
1675 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.4 SCISetInterrupt Register (SCISETINT)
Figure 29-31 andTable 29-16 illustrate thisregister. Refer toFigure 29-31 fordetails onwhen different
interrupt flags getsetinaframe during LINMode.
Figure 29-31. SCISetInterrupt Register (SCISETINT) (offset =0Ch)
31 30 29 28 27 26 25 24
SETBE INT SET PBE INT SET CEINT SET ISFE INT SET NRE INT SET FEINT SET OEINT SET PEINT
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedSET
RXDMA ALLSET
RXDMASET
TXDMA
R-0 R/WC-0 R/W-0 R/W-0
15 14 13 12 10 9 8
Reserved SET IDINT Reserved SET RXINT SET TXINT
R-0 R/WL-0 R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
SET
TOA3WUS INTSET
TOAWUS INTReservedSET
TIMEOUT INTReservedSET
WAKEUP INTSET
BRKDT INT
R/WL-0 R/WL-0 R-0 R/WL-0 R-0 R/W-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-16. SCISetInterrupt Register (SCISETINT) Field Descriptions
Bit Field Value Description
31 SET BEINT Setbiterror interrupt. This bitiseffective inLINmode only. Setting thisbitenables theSCI/LIN
module togenerate aninterrupt when there isabiterror.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
30 SET PBE INT Setphysical buserror interrupt. This bitiseffective inLINmode only. Setting thisbitenables
theSCI/LIN module togenerate aninterrupt when aphysical buserror occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
29 SET CEINT Setchecksum-error interrupt. This bitiseffective inLINmode only. Setting thisbitenables the
SCI/LIN module togenerate aninterrupt when there isachecksum error.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
28 SET ISFE INT Setinconsistent-synch-field-error interrupt. This bitiseffective inLINmode only. Setting thisbit
enables theSCI/LIN module togenerate aninterrupt when there isaninconsistent synch field
error.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
27 SET NRE INT Setno-response-error interrupt. This bitiseffective inLINmode only. Setting thisbitenables
theSCI/LIN module togenerate aninterrupt when ano-response error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.

<!-- Page 1676 -->

SCI/LIN Control Registers www.ti.com
1676 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-16. SCISetInterrupt Register (SCISETINT) Field Descriptions (continued)
Bit Field Value Description
26 SET FEINT Setframing-error interrupt. This bitiseffective inLINorSCI-compatible mode. Setting thisbit
enables theSCI/LIN module togenerate aninterrupt when aframing error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
25 SET OEINT Setoverrun-error interrupt. This bitiseffective inLINorSCI-compatible mode. Setting thisbit
enables theSCI/LIN module togenerate aninterrupt when anoverrun error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
24 SET PEINT Setparity interrupt. This bitiseffective inLINorSCI-compatible mode. Setting thisbitenables
theSCI/LIN module togenerate aninterrupt when aparity error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 SET RXDMA ALL Setreceive DMA all.This bitiseffective inSCI-compatible mode only. This bitdetermines ifa
separate interrupt isgenerated fortheaddress frames sent inmultiprocessor communications.
When thisbitis0,RXinterrupt requests aregenerated foraddress frames andDMA requests
aregenerated fordata frames. When thisbitis1,RXDMA requests aregenerated forboth
address anddata frames.
0 Read: The DMA request isdisabled foraddress frames (the receive interrupt request isenabled
foraddress frames).
Write: Noeffect.
1 Read andwrite: The DMA request isenabled foraddress anddata frames.
17 SET RXDMA Setreceiver DMA. This bitiseffective inLINorSCI-compatible mode. Toenable receiver DMA
requests, thisbitmust beset.Ifitiscleared, interrupt requests aregenerated depending on
SET RXINTbit(SCISETINT).
0 Read: The DMA request isdisabled.
Write: Noeffect.
1 Read andwrite: The DMA request isenabled foraddress anddata frames.
16 SET TXDMA Settransmit DMA. This bitiseffective inLINorSCI-compatible mode. Toenable DMA requests
forthetransmitter, thisbitmust beset.Ifitiscleared, interrupt requests aregenerated
depending onSET TXINTbit(SCISETINT).
0 Read: Transmit DMA request isdisabled.
Write: Noeffect.
1 Read andwrite: Transmit DMA request isenabled.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13 SET IDINT Setidentification interrupt. This bitiseffective inLINmode only. This bitissettoenable an
interrupt when avalid matching identifier isreceived. See Section 29.3.1.9 formore details.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
12-10 Reserved 0 Reads return 0.Writes have noeffect.
9 SET RXINT Receiver interrupt enable. This bitiseffective inLINorSCI-compatible mode. Setting thisbit
enables theSCI/LIN togenerate areceive interrupt after aframe hasbeen completely received
andthedata isbeing transferred from SCIRXSHF toSCIRD.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.

<!-- Page 1677 -->

www.ti.com SCI/LIN Control Registers
1677 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-16. SCISetInterrupt Register (SCISETINT) Field Descriptions (continued)
Bit Field Value Description
8 SET TXINT Settransmitter interrupt. This bitiseffective inLINorSCI-compatible mode. Setting thisbit
enables theSCI/LIN togenerate atransmit interrupt asdata isbeing transferred from SCITD to
SCITXSHF andtheTXRDY bitisbeing set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
7 SET TOA3WUS INT Settimeout after three wakeup signals interrupt. This bitiseffective inLINmode only. Setting
thisbitenables theSCI/LIN togenerate aninterrupt when atimeout occurs after three wakeup
signals have been sent.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
6 SET TOAWUS INT Settimeout after wakeup signal interrupt. This bitiseffective inLINmode only. Setting thisbit
enables theSCI/LIN togenerate aninterrupt when atimeout occurs after onewakeup signal
hasbeen sent.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 SET TIMEOUT INT Settimeout interrupt. This bitiseffective inLINmode only. Setting thisbitenables theSCI/LIN
togenerate aninterrupt when there isnoLINbusactivity (bus idle) foratleast four seconds.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 SET WAKEUP INT Setwake-up interrupt. This bitiseffective inLINorSCI-compatible mode. Setting thisbit
enables theSCI/LIN togenerate awakeup interrupt andthereby exitlow-power mode. If
enabled, thewakeup interrupt isasserted when local low-power mode isrequested while the
receiver isbusy orifalowlevel isdetected ontheLINRX pinduring low-power mode.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
0 SET BRKDT INT Setbreak-detect interrupt. This bitiseffective inSCI-compatible mode only. Setting thisbit
enables theSCI/LIN togenerate anerror interrupt ifabreak condition isdetected ontheLINRX
pin.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.

<!-- Page 1678 -->

SCI/LIN Control Registers www.ti.com
1678 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.5 SCIClear Interrupt Register (SCICLEARINT)
Figure 29-32 andTable 29-17 illustrate thisregister. SCICLEARINT register isused toclear theenabled
interrupts without accessing SCISETINT register.
Figure 29-32. SCIClear Interrupt Register (SCICLEARINT) (offset =10h)
31 30 29 28 27 26 25 24
CLR BEINT CLR PBE INT CLR CEINT CLR ISFE INT CLR REINT CLR FEINT CLR OEINT CLR PEINT
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedCLR
RXDMA ALLCLR
RXDMACLR
TXDMA
R-0 R/WC-0 R/W-0 R/W-0
15 14 13 12 10 9 8
Reserved CLR IDINT Reserved CLR RXINT CLR TXINT
R-0 R/WL-0 R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
CLR
TOA3WUS INTCLR
TOAWUS INTReservedCLR
TIMEOUT INTReservedCLR
WAKEUP INTCLR
BRKDT INT
R/WL-0 R/WL-0 R-0 R/WL-0 R-0 R/W-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-17. SCIClear Interrupt Register (SCICLEARINT) Field Descriptions
Bit Field Value Description
31 CLR BEINT Clear biterror interrupt. This bitiseffective inLINmode only. This bitdisables thebiterror
interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
30 CLR PBE INT Clear physical buserror interrupt. This bitiseffective inLINmode only. This bitdisables the
physical-bus error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
29 CLR CEINT Clear checksum-error interrupt. This bitiseffective inLINmode only. This bitdisables the
checksum interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
28 CLR ISFE INT Clear inconsistent-synch-field-error (ISFE) interrupt. This bitiseffective inLINmode only. This
bitdisables theISFE interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.

<!-- Page 1679 -->

www.ti.com SCI/LIN Control Registers
1679 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-17. SCIClear Interrupt Register (SCICLEARINT) Field Descriptions (continued)
Bit Field Value Description
27 CLR NRE INT Clear no-response-error interrupt. This bitiseffective inLINmode only. This bitdisables the
NRE interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
26 CLR FEINT Clear framing-error interrupt. This bitiseffective inLINorSCImode. This bitdisables the
framing-error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
25 CLR OEINT Clear overrun-error interrupt. This bitiseffective inLINorSCImode. This bitdisables the
SCI/LIN overrun error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
24 CLR PEINT Clear parity interrupt. This bitiseffective inLINorSCImode. This bitdisables theparity error
interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 CLR RXDMA ALL Clear receive DMA all.This bitiseffective inSCImode only. This bitclears thereceive DMA
request foraddress frames when set.Only receive data frames generate aDMA request.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The receive DMA request foraddress anddata frames isenabled.
Write: The receive DMA request foraddress anddata frames isdisabled.
17 CLR RXDMA Clear receive DMA request. This bitiseffective inLINorSCImode. This bitdisables the
receive DMA request when set.
0 Read: The receive DMA request isdisabled.
Write: Noeffect.
1 Read: The receive DMA request isenabled.
Write: The receive DMA request forisdisabled.
16 CLR TXDMA Clear transmit DMA request. This bitiseffective inLINorSCImode. This bitdisables the
transmit DMA request when set.
0 Read: The transmit DMA request isdisabled.
Write: Noeffect.
1 Read: The transmit DMA request isenabled.
Write: The transmit DMA request forisdisabled.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13 CLR IDINT Clear IDinterrupt. This bitiseffective inLINmode only. This bitdisables theIDinterrupt when
set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.

<!-- Page 1680 -->

SCI/LIN Control Registers www.ti.com
1680 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-17. SCIClear Interrupt Register (SCICLEARINT) Field Descriptions (continued)
Bit Field Value Description
12-10 Reserved 0 Reads return 0.Writes have noeffect.
9 CLR RXINT Clear receiver interrupt. This bitiseffective inLINorSCImode. This bitdisables thereceiver
interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
8 CLR TXINT Clear transmitter interrupt. This bitiseffective inLINorSCImode. This bitdisables the
transmitter interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
7 CLR TOA3WUS INT Clear timeout after three wakeup signals interrupt. This bitiseffective inLINmode only. This bit
disables thetimeout after three wakeup signals interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
6 CLR TOAWUS INT Clear timeout after wakeup signal interrupt. This bitiseffective inLINmode only. This bit
disables thetimeout after onewakeup signal interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 CLR TIMEOUT INT Clear timeout interrupt. This bitiseffective inLINmode only. This bitdisables thetimeout (LIN
busidle) interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CLR WAKEUP INT Clear wake-up interrupt. This bitiseffective inLINorSCI-compatible mode. This bitdisables
thewakeup interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
0 CLR BRKDT INT Clear break-detect interrupt. This bitiseffective inSCI-compatible mode only. This bitdisables
thebreak-detect interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.

<!-- Page 1681 -->

www.ti.com SCI/LIN Control Registers
1681 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.6 SCISetInterrupt Level Register (SCISETINTLVL)
Figure 29-33 andTable 29-18 illustrate thisregister.
Figure 29-33. SCISetInterrupt Level Register (SCISETINTLVL) (offset =14h)
31 30 29 28 27 26 25 24
SET BE
INTLVLSET PBE
INTLVLSET CE
INTLVLSET ISFE
INTLVLSET NRE
INTLVLSET FE
INTLVLSET OE
INTLVLSET PE
INTLVL
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedSET RXDMA
ALL INTLVLReserved
R-0 R/WC-0 R-0
15 14 13 12 10 9 8
ReservedSET ID
INTLVLReservedSET RX
INTLVLSET TX
INTLVL
R-0 R/WL-0 R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
SET
TOA3WUS
INTLVLSET TOAWUS
INTLVLReservedSET TIMEOUT
INTLVLReservedSET WAKEUP
INTLVLSET BRKDT
INTLVL
R/WL-0 R/WL-0 R-0 R/WL-0 R-0 R/W-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-18. SCISetInterrupt Level Register (SCISETINTLVL) Field Descriptions
Bit Field Value Description
31 SET BEINTLVL Setbiterror interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
30 SET PBE INTLVL Setphysical buserror interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
29 SET CEINTLVL Setchecksum-error interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
28 SET ISFE INTLVL Setinconsistent-synch-field-error interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
27 SET NRE INTLVL Setno-response-error interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
26 SET FEINTLVL Setframing-error interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.

<!-- Page 1682 -->

SCI/LIN Control Registers www.ti.com
1682 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-18. SCISetInterrupt Level Register (SCISETINTLVL) Field Descriptions (continued)
Bit Field Value Description
25 SET OEINTLVL Setoverrun-error interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
24 SET PEINTLVL Setparity error interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 SET RXDMA ALL LVL Setreceive DMA allinterrupt levels. This bitiseffective inSCImode only.
0 Read: The receive interrupt request foraddress frames ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The receive interrupt request foraddress frames ismapped totheINT1 line.
17-14 Reserved 0 Reads return 0.Writes have noeffect.
13 SET IDINTLVL SetIDinterrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
12-10 Reserved 0 Reads return 0.Writes have noeffect.
9 SET RXINTLVL Setreceiver interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
8 SET TXINTLVL Settransmitter interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
7 SET TOA3WUS INTLVL Settimeout after three wakeup signals interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
6 SET TOAWUS INTLVL Settimeout after wakeup signal interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 SET TIMEOUT INTLVL Settimeout interrupt level. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 SET WAKEUP INTLVL Setwake-up interrupt level. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.

<!-- Page 1683 -->

www.ti.com SCI/LIN Control Registers
1683 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-18. SCISetInterrupt Level Register (SCISETINTLVL) Field Descriptions (continued)
Bit Field Value Description
0 SET BRKDT INTLVL Setbreak-detect interrupt level. This bitiseffective inSCI-compatible mode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.

<!-- Page 1684 -->

SCI/LIN Control Registers www.ti.com
1684 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.7 SCIClear Interrupt Level Register (SCICLEARINTLVL)
Figure 29-34 andTable 29-19 illustrate thisregister.
Figure 29-34. SCIClear Interrupt Level Register (SCICLEARINTLVL) (offset =18h)
31 30 29 28 27 26 25 24
CLR BE
INTLVLCLR PBE
INTLVLCLR CE
INTLVLCLR ISFE
INTLVLCLR NRE
INTLVLCLR FE
INTLVLCLR OE
INTLVLCLR PE
INTLVL
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedCLR RXDMA
ALL INTLVLReserved
R-0 R/WC-0 R-0
15 14 13 12 10 9 8
ReservedCLR ID
INTLVLReservedCLR RX
INTLVLCLR TX
INTLVL
R-0 R/WL-0 R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
CLR
TOA3WUS
INTLVLCLR TOAWUS
INTLVLReservedCLR TIMEOUT
INTLVLReservedCLR WAKEUP
INTLVLCLR BRKDT
INTLVL
R/WL-0 R/WL-0 R-0 R/WL-0 R-0 R/W-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-19. SCIClear Interrupt Level Register (SCICLEARINTLVL) Field Descriptions
Bit Field Value Description
31 CLR BEINTLVL Clear biterror interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
30 CLR PBE INTLVL Clear physical buserror interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
29 CLR CEINTLVL Clear checksum-error interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
28 CLR ISFE INTLVL Clear inconsistent-synch-field-error (ISFE) interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
27 CLR NRE INTLVL Clear no-response-error interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.

<!-- Page 1685 -->

www.ti.com SCI/LIN Control Registers
1685 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-19. SCIClear Interrupt Level Register (SCICLEARINTLVL) Field Descriptions (continued)
Bit Field Value Description
26 CLR FEINTLVL Clear framing-error interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
25 CLR OEINTLVL Clear overrun-error interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
24 CLR PEINTLVL Clear parity interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 CLR RXDMA ALL LVL Clear receive DMA interrupt level. This bitiseffective inSCI-compatible mode only.
0 Read: The receive interrupt request foraddress frames ismapped totheINT0 line.
Write: Noeffect.
1 Read: The receive interrupt request foraddress frames ismapped totheINT1 line.
Write: The receive interrupt request foraddress frames ismapped totheINT0 line.
17-14 Reserved 0 Reads return 0.Writes have noeffect.
13 CLR IDINTLVL Clear IDinterrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
12-10 Reserved 0 Reads return 0.Writes have noeffect.
9 CLR RXINTLVL Clear receiver interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
8 CLR TXINTLVL Clear transmitter interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
7 CLR TOA3WUS INTLVL Clear timeout after three wakeup signals interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.

<!-- Page 1686 -->

SCI/LIN Control Registers www.ti.com
1686 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-19. SCIClear Interrupt Level Register (SCICLEARINTLVL) Field Descriptions (continued)
Bit Field Value Description
6 CLR TOAWUS INTLVL Clear timeout after wakeup signal interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 CLR TIMEOUT INTLVL Clear timeout interrupt. This bitiseffective inLINmode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
3-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CLR WAKEUP INTLVL Clear wake-up interrupt. This bitiseffective inLINorSCI-compatible mode.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
0 CLR BRKDT INTLVL Clear break-detect interrupt. This bitiseffective inSCI-compatible mode only.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.

<!-- Page 1687 -->

www.ti.com SCI/LIN Control Registers
1687 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.8 SCIFlags Register (SCIFLR)
Figure 29-35 andTable 29-20 illustrate thisregister.
Figure 29-35. SCIFlags Register (SCIFLR) (offset =1Ch)
31 30 29 28 27 26 25 24
BE PBE CE ISFE NRE FE OE PE
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/WL-0 R/W-0 R/W-0 R/W-0
23 16
Reserved
R-0
15 14 13 12 11 10 9 8
Reserved IDRX IDTX RXWAKE TXEMPTY TXWAKE RXRDY TXRDY
R-0 R/WL-0 R/WL-0 R/WC-0 R/W-1 R/WC-0 R/W-0 R/W-1
7 6 5 4 3 2 1 0
TOA3WUS TOAWUS Reserved TIMEOUT BUSY IDLE WAKE UP BRKDT
R/WL-0 R/WL-0 R-0 R/WL-0 R/W-0 R-0 R/WL-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WC=Write inSCI-compatible mode only; WL=Write inLINmode only; -n=value after reset
Table 29-20. SCIFlags Register (SCIFLR) Field Descriptions
Bit Field Value Description
31 BE Biterror flag. This bitiseffective inLINmode only. This bitissetwhen abiterror hasoccurred.
This isdetected bytheinternal bitmonitor. See Section 29.3.1.8 formore information. The biterror
flagiscleared byanyofthefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Onreception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Noerror hasbeen detected since thisbitwas lastcleared.
Write: Noeffect.
1 Read: Anerror hasbeen detected since thisbitwas lastcleared.
Write: The bitiscleared to0.
30 PBE Physical buserror flag. This bitiseffective inLINmode only. This bitissetwhen aphysical bus
error hasbeen detected bythebitmonitor inTED. See Section 29.3.1.8 formore information. The
physical buserror flagiscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Onreception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
Note: ThePBE willonly beflagged, ifnoSynch Break canbegenerated (forexample,
because ofabusshortage toVBAT) orifnoSynch Break Delimiter canbegenerated (for
example, because ofabusshortage toGND).
0 Read: Noerror hasbeen detected since thisbitwas lastcleared.
Write: Noeffect.
1 Read: Anerror hasbeen detected since thisbitwas lastcleared.
Write: The bitiscleared to0.

<!-- Page 1688 -->

SCI/LIN Control Registers www.ti.com
1688 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
29 CE Checksum error flag. This bitiseffective inLINmode only. This bitissetwhen achecksum error
hasbeen detected byareceiving node. This error isdetected bytheTED logic. See
Section 29.3.1.8 formore information. The type ofchecksum tobeused depends ontheCTYPE bit
inSCIGCR1. The checksum error flagiscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Noerror hasbeen detected since thisbitwas lastcleared.
Write: Noeffect.
1 Read: Anerror hasbeen detected since thisbitwas lastcleared.
Write: The bitiscleared to0.
28 ISFE Inconsistent synch field error flag. This bitiseffective inLINmode only. This bitissetwhen an
inconsistent synch field error hasbeen detected bythesynchronizer during header reception. See
Section 29.3.1.5.2 formore information. The inconsistent synch field error flagiscleared bythe
following:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Noinconsistent synch field error hasbeen detected.
Write: Noeffect.
1 Read: Aninconsistent synch field error hasbeen detected.
Write: The bitiscleared to0.
27 NRE No-response error flag. This bitiseffective inLINmode only. This bitissetwhen there isno
response toamaster 'sheader completed within TFRAME_MAX. This timeout period isapplied for
message frames ofknown length (identifiers 0to61). This error isdetected bythesynchronizer.
See Section 29.3.1.7 formore information. The no-response error flagiscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Nono-response error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Ano-response error hasbeen detected.
Write: The bitiscleared to0.

<!-- Page 1689 -->

www.ti.com SCI/LIN Control Registers
1689 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
26 FE Framing error flag. This bitiseffective inLINorSCI-compatible mode. This bitissetwhen an
expected stop bitisnotfound. InSCIcompatibility mode, only thefirststop bitischecked. The
missing stop bitindicates thatsynchronization with thestart bithasbeen lostandthatthecharacter
isincorrectly framed. Detection ofaframing error causes theSCI/LIN togenerate anerror interrupt
iftheSET FEINTbitissetintheregister SCISETINT. The framing error flagiscleared bythe
following:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
*Reception ofanew character/frame, depending onwhether themodule isinSCIcompatible or
LINmode
Inmulti-buffer mode theframe isdefined intheSCIFORMAT register.
0 Read: Noframing error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Aframing error hasbeen detected since thelastclear.
Write: The bitiscleared to0.
25 OE Overrun error flag. This bitiseffective inLINorSCI-compatible mode. This bitissetwhen the
transfer ofdata from SCIRXSHF toSCIRD overwrites unread data already inSCIRD ortheRDy
buffers inLINRD0 andLINRD1. Detection ofanoverrun error causes theLINtogenerate anerror
interrupt iftheSET OEINTbit=1.The OEflagisreset bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Nooverrun error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Anoverrun error hasbeen detected.
Write: The bitiscleared to0.
24 PE Parity error flag. This bitiseffective inLINorSCI-compatible mode. This bitissetwhen aparity
error isdetected inthereceived data. InSCIaddress-bit mode, theparity iscalculated onthedata
andaddress bitfields ofthereceived frame. Inidle-line mode, only thedata isused tocalculate
parity. Anerror isgenerated when acharacter isreceived with amismatch between thenumber of
1sanditsparity bit.Iftheparity function isdisabled (SCIGCR[2] =0),thePEflagisdisabled and
read as0.Detection ofaparity error causes theLINtogenerate anerror interrupt iftheSET PE
INTbit=1.The PEbitisreset bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reception ofanew character orframe, depending onwhether themodule isinSCIcompatible
orLINmode, respectively.
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Noparity error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Aparity error hasbeen detected.
Write: The bitiscleared to0.
23-15 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1690 -->

SCI/LIN Control Registers www.ti.com
1690 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
14 IDRX Identifier onreceive flag. This bitiseffective inLINmode only. This flagissetonce anidentifier is
received with anreceive match andnoID-parity error. See Section 29.3.1.9 formore details. This
flagindicates thatanew valid identifier hasbeen received onanRXmatch. This bitiscleared by
thefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading theLINID register
*Reception ofanew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Novalid IDhasbeen received since thelastclear.
Write: Noeffect.
1 Read: Avalid IDRXhasbeen received inLINID[23:16] onanRXmatch.
Write: The bitiscleared to0.
13 IDTX Identifier ontransmit flag. This bitiseffective inLINmode only. This flagissetwhen anidentifier is
received with atransmit match andnoID-parity error. See Section 29.3.1.9 formore details. This
flagindicates thatanew valid identifier hasbeen received onaTXmatch. This bitiscleared bythe
following:
*Setting theSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading theLINID register
*Receiving anew synch break
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Novalid IDhasbeen received since thelastclear.
Write: Noeffect.
1 Read: Avalid IDTXhasbeen received inLINID[23:16] onanTXmatch.
Write: The bitiscleared to0.
12 RXWAKE Receiver wakeup detect flag. This bitiseffective inSCI-compatible mode only. The SCIsets thisbit
toindicate thatthedata currently inSCIRD isanaddress. RXWAKE iscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Upon receipt ofadata frame.
0 The data inSCIRD isnotanaddress.
1 The data inSCIRD isanaddress.
11 TXEMPTY Transmitter empty flag. This flagindicates thetransmitter 'sbuffer register(s) (SCITD/TDy) andshift
register (SCITXSHF) areempty. Inmulti-buffer mode, thisflagindicates theTDx registers andshift
register (SCITXSHF) areempty. Innon-multi-buffer mode, thisflagindicates theLINTD0 byte and
theshift register (SCITXSHF) areempty.
Note: TheRESET bit,anactive SWnRST (SCIGCR1[7]) orasystem reset sets thisbit.This
bitdoes notcause aninterrupt request.
SCImode orLINnon-multi-buffer mode:
0 Transmitter buffer orshift register (orboth) areloaded with data.
1 Transmitter buffer andshift registers areboth empty.
InLINmode using multi-buffer mode:
0 Multi-buffer orshift register (orall)areloaded with data
1 Multi-buffer andshift registers areallempty.

<!-- Page 1691 -->

www.ti.com SCI/LIN Control Registers
1691 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
10 TXWAKE Transmitter wakeup method select. This bitiseffective inSCImode only. The TXWAKE bit
controls whether thedata inSCITD should besent asanaddress ordata frame using
multiprocessor communication format. This bitissetto1or0bysoftware before abyte iswritten to
SCITD andiscleared bytheSCIwhen data istransferred from SCITD toSCITXSHF orbya
system reset.
Note: TXWAKE isnotcleared bytheSWnRST bit.
Address-bit mode :
0 Frame tobetransmitted willbedata (address bit=0).
1 Frame tobetransmitted willbeanaddress (address bit=1).
Idle-line mode :
0 The frame tobetransmitted willbedata.
1 The following frame tobetransmitted willbeanaddress (writing a1tothisbitfollowed bywriting
dummy data totheSCITD willresult inaidleperiod of11bitperiods before thenext frame is
transmitted).
9 RXRDY Receiver ready flag. InSCI-compatible mode, thereceiver sets thisbittoindicate thattheSCIRD
contains new data andisready toberead bytheCPU. InLINmode, RXRDY issetonce avalid
frame isreceived inmulti-buffer mode, avalid frame being amessage frame received with no
errors. Innon-multi-buffer mode, RXRDY issetforeach received byte andwillbesetforthelast
byte oftheframe ifthere arenoerrors. The SCI/LIN generates areceive interrupt when RXRDY
flagbitissetiftheSET RXINTbitisset(SCISETINT[9]); RXRDY iscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading theSCIRD register incompatibility mode
*Reading thelastdata byte RDy oftheresponse inLINmode
Note: TheRXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0/1 register.
0 Read: Nonew data isinSCIRD.
Write: Noeffect.
1 Read: New data isready toberead from SCIRD.
Write: The bitiscleared to0.
8 TXRDY Transmitter buffer register ready flag. When set,thisbitindicates thatthetransmit buffer(s)
register(s) (SCITD incompatibility mode andLINTD0/LINTD1 inmulti-buffer mode) areready toget
another character from aCPU write.
InSCI, writing data toSCITD automatically clears thisbit.InLINmode, thisbitiscleared once byte
0(TD0) iswritten toLINTD0. This bitissetafter thedata oftheTXbuffer isshifted intothe
SCITXSHF register. This event cantrigger atransmit interrupt after data iscopied totheTXshift
register SCITXSHF, iftheSET TXINTisset.
Note:
1)TXRDY isalso setto1either bysetting oftheRESET bit,enabling SWnRST orbya
system reset.
2)TheTXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0/1 register.
3)Thetransmit interrupt request canbeeliminated until thenext series ofdata written into
thetransmit buffers LINTD0 andLINTD1, bydisabling thecorresponding interrupt viathe
SCICLEARINT register orbydisabling thetransmitter viatheTXENA bit.
SCImode:
0 SCITD isfull.
1 SCITD isready toreceive thenext character.
LINmode:
0 Multi-buffers arefull.
1 Multi-buffers areready toreceive thenext character.
Formore information ontransmit interrupt handling, seetheSCIdocument forcompatibility mode
andSection 29.3.1.9 forLINmode.

<!-- Page 1692 -->

SCI/LIN Control Registers www.ti.com
1692 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
7 TOA3WUS Timeout after three wakeup signals flag. This bitiseffective inLINmode only. This flagissetif
there isnosynch break received after three wakeup signals andaperiod of1.5seconds has
passed. Such expiration time isused before issuing another round ofwakeup signals. This bitis
cleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
See Section 29.4.3 formore information.
0 Read: Notimeout occurred after three wakeup signals.
Write: Noeffect.
1 Read: Timeout occurred after three wakeup signals and1.5seconds time.
Write: The bitiscleared to0.
6 TOAWUS Timeout after wakeup signal flag. This bitiseffective inLINmode only. This bitissetifthere isno
synch break received after awakeup signal hasbeen sent. Aminimum of150msexpiration time is
used before issuing another wakeup signal. This bitiscleared bythefollowing:
*Setting theSWnRST bit
*Setting oftheRESET bit
*Asystem reset occurring
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
See Section 29.4.3 formore information.
0 Read: Notimeout occurred after onewakeup signal (150 ms).
Write: Noeffect.
1 Read: Timeout occurred after onewakeup signal.
Write: The bitiscleared to0.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 TIMEOUT LINbusidletimeout flag. This bitiseffective inLINmode only. This bitissetearliest after atleast
four seconds ofbusinactivity. Bus inactivity isdefined asnotransactions between recessive and
dominant (and vice versa). This bitiscleared bythefollowing:
*Setting theSWnRST bit
*Setting oftheRESET bit
*Asystem reset occurring
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
See Section 29.3.1.7 formore information.
0 Read: Nobusidlehasbeen detected since thisbitwas lastcleared.
Write: Noeffect.
1 Read: ALINbusidlehasbeen detected.
Write: The bitiscleared to0.
3 BUSY Bus busy flag. This bitiseffective inLINmode andSCI-compatible mode. This bitindicates
whether thereceiver isintheprocess ofreceiving aframe. Assoon asthereceiver detects the
beginning ofastart bit,theBUSY bitissetto1.When thereception ofaframe iscomplete, the
SCI/LIN clears theBUSY bit.IfSET WAKEUP INTissetandpower down isrequested while thisbit
isset,theSCI/LIN automatically prevents low-power mode from being entered andgenerates
wakeup interrupt. The BUSY bitiscontrolled directly bytheSCI/LIN receiver, butthisbitcanalso
becleared bythefollowing:
*Setting theSWnRST bit
*Setting oftheRESET bit
*Asystem reset occurring
0 The receiver isnotcurrently receiving aframe.
1 The receiver iscurrently receiving aframe.

<!-- Page 1693 -->

www.ti.com SCI/LIN Control Registers
1693 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-20. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
2 IDLE SCIreceiver inidlestate. This bitiseffective inSCI-compatible mode only. While thisbitisset,the
SCIlooks foranidleperiod toresynchronize itself with thebitstream. The receiver does not
receive anydata while thebitisset.The busmust beidlefor11bitperiods toclear thisbit.The
SCIenters theidlestate ifoneofthefollowing events occurs:
*Asystem reset
*AnSCIsoftware reset
*Apower down
*The RXpinisconfigured asageneral I/Opin
0 The idleperiod hasbeen detected; theSCIisready toreceive.
1 The idleperiod hasnotbeen detected; theSCIwillnotreceive anydata.
1 WAKEUP Wakeup flag. This bitiseffective inLINmode only. This bitissetbytheSCI/LIN when receiver or
transmitter activity hastaken themodule outofpower-down mode. Aninterrupt isgenerated ifthe
SET WAKEUP INTbit(SCISETINT[2]) isset.Itiscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
Forcompatibility mode, seetheSCIdocument formore information onlow-power mode.
0 Read: The module willnotwake upfrom power-down mode.
Write: Noeffect.
1 Read: Wake upfrom power-down mode.
Write: The bitiscleared to0.
0 BRKDT SCIbreak-detect flag. This bitiseffective inSCI-compatible mode only. This bitissetwhen theSCI
detects abreak condition ontheLINRX pin.Abreak condition occurs when theLINRX pinremains
continuously lowforatleast 10bitsafter amissing firststop bit,thatis,after aframing error.
Detection ofabreak condition causes theSCItogenerate anerror interrupt iftheSET BRKDT INT
bitisset.The BRKDT bitisreset bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Nobreak condition hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Abreak condition hasbeen detected.
Write: The bitiscleared to0.

<!-- Page 1694 -->

SCI/LIN Control Registers www.ti.com
1694 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.9 SCIInterrupt Vector Offset 0(SCIINTVECT0)
Figure 29-36 andTable 29-21 illustrate thisregister.
Figure 29-36. SCIInterrupt Vector Offset 0(SCIINTVECT0) (offset =20h)
31 16
Reserved
R-0
15 5 4 0
Reserved INTVECT0
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-21. SCIInterrupt Vector Offset 0(SCIINTVECT0) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 INVECT0 0-1Fh Interrupt vector offset forINT0. This register indicates theoffset forinterrupt lineINT0. Aread to
thisregister updates itsvalue tothenext highest priority pending interrupt inSCIFLR andclears
theflaginSCIFLR corresponding totheoffset thatwas read. See Table 29-4 foralistofthe
interrupts.
Note: Theflags forthereceive (SCIFLR[9]) andthetransmit (SCIFLR[8]) interrupt cannot
becleared byreading thecorresponding offset vector inthisregister (see detailed
description inSCIFLR register).
29.7.10 SCIInterrupt Vector Offset 1(SCIINTVECT1)
Figure 29-37 andTable 29-22 illustrate thisregister.
Figure 29-37. SCIInterrupt Vector Offset 1(SCIINTVECT1) (offset =24h)
31 16
Reserved
R-0
15 5 4 0
Reserved INTVECT1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-22. SCIInterrupt Vector Offset 1(SCIINTVECT1) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 INVECT1 0-1Fh Interrupt vector offset forINT1. This register indicates theoffset forinterrupt lineINT1. Aread to
thisregister updates itsvalue tothenext highest priority pending interrupt inSCIFLR andclears
theflaginSCIFLR corresponding totheoffset thatwas read. See Table 29-4 forlistof
interrupts.
Note: Theflags forthereceive (SCIFLR[9]) andthetransmit (SCIFLR[8]) interrupt cannot
becleared byreading thecorresponding offset vector inthisregister (see detailed
description inSCIFLR register).

<!-- Page 1695 -->

www.ti.com SCI/LIN Control Registers
1695 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.11 SCIFormat Control Register (SCIFORMAT)
Figure 29-38 andTable 29-23 illustrate thisregister.
Figure 29-38. SCIFormat Control Register (SCIFORMAT) (offset =28h)
31 19 18 16
Reserved LENGTH
R-0 R/W-0
15 3 2 0
Reserved CHAR
R-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; WC=Write inSCI-compatible mode only; -n=value after reset
Table 29-23. SCIFormat Control Register (SCIFORMAT) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 LENGTH 0-3h Frame length control bits. InLINmode ,these bitsindicate thenumber ofbytes intheresponse
field from 1to8bytes. Inbuffered SCImode ,these bitsindicate thenumber ofcharacters, with the
number ofbitspercharacter specified inCHAR (SCIFORMAT[2:0]).
When these bitsareused toindicate LINresponse length (SCIGCR1[0] =1),then when there is
anIDRXmatch, thisvalue should beupdated with theexpected length oftheresponse. In
buffered SCImode, these bitsindicate thetransmitter/receiver format forthenumber ofcharacters:
1to8.There canbeuptoeight characters with eight bitseach.
0 The response field has1byte/character.
1h The response field has2bytes/characters.
2h The response field has3bytes/characters.
3h The response field has4bytes/characters.
4h The response field has5bytes/characters.
5h The response field has6bytes/characters.
6h The response field has7bytes/characters.
7h The response field has8bytes/characters.
15-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CHAR 0-7h Character length control bits. These bitsareeffective innon-buffered SCIandbuffered SCImodes
only. These bitssettheSCIcharacter length from 1to8bits.
Innon-buffered SCIandbuffered SCImode, when data offewer than eight bitsinlength is
received, itisleftjustified inSCIRD/RDy andpadded with trailing zeros. Data read from the
SCIRD should beshifted bysoftware tomake thereceived data right justified.
Data written totheSCITD should beright justified butdoes notneed tobepadded with
leading zeros.
0 The character is1bitlong.
1h The character is2bitslong.
2h The character is3bitslong.
3h The character is4bitslong.
4h The character is5bitslong.
5h The character is6bitslong.
6h The character is7bitslong.
7h The character is8bitslong.

<!-- Page 1696 -->

Isosynchronous baud value = ) (VCLK Frequency
2
Asynchronous baud value = ) (VCLK Frequency
32
Isosynchronous baud value = ) (VCLK Frequency
P + 1
Asynchronous baud value =( )VCLK Frequency
16P + 1 +M
16)(
SCI/LIN Control Registers www.ti.com
1696 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.12 Baud Rate Selection Register (BRS)
This section describes thebaud rate selection register. Figure 29-39 andTable 29-24 illustrate this
register.
Figure 29-39. Baud Rate Selection Register (BRS) (offset =2Ch)
31 30 28 27 24 23 16
Rsvd U M PRESCALER P
R-0 R/W-0 R/W-0 R/W-0
15 0
PRESCALER P
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-24. Baud Rate Selection Register (BRS) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads return 0.Writes have noeffect.
30-28 U 0-2h SCI/LIN super fractional divider selection. These bitsareeffective inLINorSCIasynchronous
mode. These bitsareanadditional fractional part forthebaud rate specification. These bits
allow asuper-fine tuning ofthefractional baud rate with seven more intermediate values for
each oftheMfractional divider values. See Section 29.3.1.4.1 formore details.
27-24 M 0-3h SCI/LIN 4-bit fractional divider selection. These bitsareeffective inLINorSCIasynchronous
mode. These bitsareused toselect abaud rate fortheSCI/LIN module, andthey area
fractional part forthebaud rate specification. The Mdivider allows fine-tuning ofthebaud rate
over thePprescaler with 15additional intermediate values foreach ofthePinteger values. See
Section 29.3.1.4.1 formore details.
23-0 PRESCALER P 0-FF FFFFh These bitsareused toselect abaud rate fortheSCI/LIN module. These bitsareeffective in
LINmode andSCIcompatibility.
The SCI/LIN hasaninternally generated serial clock determined bytheVCLK andthe
prescalers PandMinthisregister. The LINuses the24-bit integer prescaler Pvalue ofthis
register toselect oneofover 16,700,000. The additional 4-bit fractional divider Mrefines the
baud rate selection PRESCALER[27:24].
NOTE: InLINmode, ONLY theasynchronous mode andbaud ratevalues areused.
The baud rate canbecalculated using thefollowing formulas:
(52)
(53)
ForP=0,
(54)
(55)
Table 29-25 contains comparative baud values fordifferent Pvalues, with VCLK =50MHz, for
asynchronous mode.

<!-- Page 1697 -->

www.ti.com SCI/LIN Control Registers
1697 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module(1)VCLK =50MHz
(2)Values areindecimal except forcolumn 2.Table 29-25. Comparative Baud Values forDifferent PValues, Asynchronous Mode(1)(2)
24-Bit Register Value Baud Selected
Percent Error
Decimal Hex Ideal Actual
26 00001A 115200 115740 0.47
53 000035 57600 57870 0.47
80 000050 38400 38580 0.47
162 0000A2 19200 19172 -0.15
299 00012B 10400 10417 0.16
325 000145 9600 9586 -0.15
399 00018F 7812.5 7812.5 0.00
650 00028A 4800 4800 0.00
15624 003BA0 200 200 0.00
624999 098967 5 5 0.00

<!-- Page 1698 -->

SCI/LIN Control Registers www.ti.com
1698 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.13 SCIData Buffers (SCIED, SCIRD, SCITD)
The SCIhasthree addressable registers inwhich transmit andreceive data isstored. These three
registers areavailable inSCImode only.
29.7.13.1 Receiver Emulation Data Buffer (SCIED)
The SCIED register isaddressed atalocation different from SCIRD, butisphysically thesame register.
Figure 29-40 andTable 29-26 illustrate thisregister.
Figure 29-40. Receiver Emulation Data Buffer (SCIED) (offset =30h)
31 16
Reserved
R-0
15 8 7 0
Reserved ED
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-26. Receiver Emulation Data Buffer (SCIED) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 ED 0-FFh Emulator data. This bitiseffective inSCI-compatible mode only. Reading SCIED[7:0] does not
clear theRXRDY flag, unlike reading SCIRD. This register should beused only byanemulator
thatmust continually read thedata buffer without affecting theRXRDY flag.
29.7.13.2 Receiver Data Buffer (SCIRD)
This register provides alocation forthereceiver data. Figure 29-41 andTable 29-27 illustrate thisregister.
Figure 29-41. Receiver Data Buffer (SCIRD) (offset =34h)
31 16
Reserved
R-0
15 8 7 0
Reserved RD
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-27. Receiver Data Buffer (SCIRD) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 RD 0-FFh Receiver data. This bitiseffective inSCI-compatible mode only. When aframe hasbeen
completely received, thedata intheframe istransferred from thereceiver shift register SCIRXSHF
tothisregister. Asthistransfer occurs, theRXRDY flagissetandareceive interrupt isgenerated
ifSET RXINTisset.
Note: When thedata isread from SCIRD, theRXRDY flagisautomatically cleared.
NOTE: When theSCIreceives data thatisfewer than eight bitsinlength, itloads thedata intothis
register inaleftjustified format padded with trailing zeros. Therefore, your software should
perform alogical shift onthedata bythecorrect number ofpositions tomake itright justified.

<!-- Page 1699 -->

www.ti.com SCI/LIN Control Registers
1699 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.13.3 Transmit Data Buffer Register (SCITD)
Data tobetransmitted iswritten totheSCITD register. Figure 29-42 andTable 29-28 illustrate this
register.
Figure 29-42. Transmit Data Buffer Register (SCITD) (offset =38h)
31 16
Reserved
R-0
15 8 7 0
Reserved TD
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-28. Transmit Data Buffer Register (SCITD) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 TD 0-FFh Transmit data. This bitiseffective inSCI-compatible mode only. Data tobetransmitted iswritten
totheSCITD register. The transfer ofdata from thisregister tothetransmit shift register
SCITXSHF sets theTXRDY flag, which indicates thatSCITD isready tobeloaded with another
byte ofdata.
Note: IfSET TXINTisset,thisdata transfer also causes aninterrupt.
NOTE: Data written totheSCITD register thatisfewer than eight bitslong must beright justified, but
itdoes notneed tobepadded with leading zeros.
29.7.14 SCIPinI/OControl Register 0(SCIPIO0)
Figure 29-43 andTable 29-29 illustrate thisregister.
Figure 29-43. SCIPinI/OControl Register 0(SCIPIO0) (offset =3Ch)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXFUNC RXFUNC Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-29. SCIPinI/OControl Register 0(SCIPIO0) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXFUNC Transfer function. This bitiseffective inLINorSCImode. This bitdefines thefunction ofpinLINTX.
0 LINTX isageneral-purpose digital I/Opin.
1 LINTX istheSCI/LIN transmit pin.
1 RXFUNC Receive function. This bitiseffective inLINorSCImode. This bitdefines thefunction ofpinLINRX.
0 LINRX isageneral-purpose digital I/Opin.
1 LINRX istheSCI/LIN receive pin.
0 Reserved 0 Writes have noeffect.

<!-- Page 1700 -->

SCI/LIN Control Registers www.ti.com
1700 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.15 SCIPinI/OControl Register 1(SCIPIO1)
Figure 29-44 andTable 29-30 illustrate thisregister.
Figure 29-44. SCIPinI/OControl Register 1(SCIPIO1) (offset =40h)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXDIR RXDIR Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-30. SCIPinI/OControl Register 1(SCIPIO1) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXDIR Transmit pindirection. This bitiseffective inLINorSCImode. This bitdetermines thedata direction on
theLINTX pinifitisconfigured with general-purpose I/Ofunctionality (TXFUNC =0).See Table 29-31
fortheLINTX pincontrol with thisbitandothers.
0 LINTX isageneral-purpose input pin.
1 LINTX isageneral-purpose output pin.
1 RXDIR Receive pindirection. This bitiseffective inLINorSCImode. This bitdetermines thedata direction on
theLINRX pinifitisconfigured with general-purpose I/Ofunctionality (RX FUNC =0).See Table 29-32
fortheLINRX pincontrol with thisbitandothers.
0 LINRX isageneral-purpose input pin.
1 LINRX isageneral-purpose output pin.
0 Reserved 0 Writes have noeffect.
(1)TXINisaread-only bit.Itsvalue always reflects thelevel oftheSCITX pin.Table 29-31. LINTX PinControl
Function TXIN(1)TXOUT TXFUNC TXDIR
LINTX X X 1 X
General purpose input X X 0 0
General purpose output, high X 1 0 1
General purpose output, low X 0 0 1
(1)RXINisaread-only bit.Itsvalue always reflects thelevel oftheSCIRX pin.Table 29-32. LINRX PinControl
Function RXIN(1)RXOUT RXFUNC RXDIR
LINRX X X 1 X
General purpose input X X 0 0
General purpose output, high X 1 0 1
General purpose output, low X 0 0 1

<!-- Page 1701 -->

www.ti.com SCI/LIN Control Registers
1701 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.16 SCIPinI/OControl Register 2(SCIPIO2)
Figure 29-45 andTable 29-33 illustrate thisregister.
Figure 29-45. SCIPinI/OControl Register 2(SCIPIO2) (offset =44h)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXIN RXIN Reserved
R-0 R-X R-X R-X
LEGEND: R=Read only; X=value isindeterminate; -n=value after reset
Table 29-33. SCIPinI/OControl Register 2(SCIPIO2) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXIN Transmit pinin.This bitiseffective inLINorSCImode. This bitcontains thecurrent value ontheLINTX
pin.
0 The LINTX pinisatlogic low(0).
1 The LINTX pinisatlogic high (1).
1 RXIN Receive pinin.This bitiseffective inLINorSCImode. This bitcontains thecurrent value ontheLINRX
pin.
0 The LINRX pinisatlogic low(0).
1 The LINRX pinisatlogic high (1).
0 Reserved 0 Writes have noeffect.

<!-- Page 1702 -->

SCI/LIN Control Registers www.ti.com
1702 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.17 SCIPinI/OControl Register 3(SCIPIO3)
Figure 29-46 andTable 29-34 illustrate thisregister.
Figure 29-46. SCIPinI/OControl Register 3(SCIPIO3) (offset =48h)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXOUT RXOUT Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-34. SCIPinI/OControl Register 3(SCIPIO3) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXOUT Transmit pinout.This bitiseffective inLINorSCImode. This pinspecifies thelogic tobeoutput onpin
LINTX ifthefollowing conditions aremet:
*TXFUNC =0(LINTX pinisageneral-purpose I/O.)
*TXDIR =1(LINTX pinisageneral-purpose output.)
See Table 29-31 foranexplanation ofthisbit'seffect incombination with other bits.
0 The output ontheLINTX isatlogic low(0).
1 The output ontheLINTX pinisatlogic high (1).(Output voltage isVOHorhigher ifTXPDR =0and
output isinhigh impedance state ifTXPDR =1.)
1 RXOUT Receive pinout.This bitiseffective inLINorSCImode. This bitspecifies thelogic tobeoutput onpin
LINRX ifthefollowing conditions aremet:
*RXFUNC =0(LINRX pinisageneral-purpose I/O.)
*RXDIR =1(LINRX pinisageneral-purpose output.)
See Table 29-32 foranexplanation ofthisbit'seffect incombination with theother bits.
0 The output ontheLINRX pinisatlogic low(0).
1 The output ontheLINRX pinisatlogic high (1).(Output voltage isVOHorhigher ifRXPDR =0,and
output isinhigh impedance state ifRXPDR =1.)
0 Reserved 0 Writes have noeffect.

<!-- Page 1703 -->

www.ti.com SCI/LIN Control Registers
1703 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.18 SCIPinI/OControl Register 4(SCIPIO4)
Figure 29-47 andTable 29-35 illustrate thisregister.
Figure 29-47. SCIPinI/OControl Register 4(SCIPIO4) (offset =4Ch)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXSET RXSET Reserved
R-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-35. SCIPinI/OControl Register 4(SCIPIO4) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXSET Transmit pinset.This bitiseffective inLINorSCImode. This bitsets thelogic tobeoutput onpin
LINTX ifthefollowing conditions aremet:
*TXFUNC =0(LINTX pinisageneral-purpose I/O.)
*TXDIR =1(LINTX pinisageneral-purpose output.)
See Table 29-31 foranexplanation ofthisbit'seffect incombination with other bits.
0 Read: The output onLINTX isatlogic low(0).
Write: Noeffect.
1 Read andwrite: The output onLINTX isatlogic high (1).
1 RXSET Receive pinset.This bitiseffective inLINorSCImode. This bitsets thedata tobeoutput onpin
LINRX ifthefollowing conditions aremet:
*RXFUNC =0(LINRX pinisageneral-purpose I/O.)
*RXDIR =1(LINRX pinisageneral-purpose output.)
See Table 29-32 foranexplanation ofthisbit'seffect incombination with theother bits.
0 Read: The output onLINRX isatlogic low(0).
Write: Noeffect.
1 Read andwrite: The output onLINRX isatlogic high (1).
0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1704 -->

SCI/LIN Control Registers www.ti.com
1704 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.19 SCIPinI/OControl Register 5(SCIPIO5)
Figure 29-48 andTable 29-36 illustrate thisregister.
Figure 29-48. SCIPinI/OControl Register 5(SCIPIO5) (offset =50h)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXCLR RXCLR Reserved
R-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-36. SCIPinI/OControl Register 5(SCIPIO5) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXCLR Transmit pinclear. This bitiseffective inLINorSCImode. This bitclears thelogic tobeoutput onpin
LINTX ifthefollowing conditions aremet:
*TXFUNC =0(LINTX pinisageneral-purpose I/O.)
*TXDIR =1(LINTX pinisageneral-purpose output.)
0 Read: The output onLINTX isatlogic low(0).
Write: Noeffect.
1 Read: The output onLINTX isatlogic high (1).
Write: The output onLINTX isatlogic low(0).
1 RXCLR Receive pinclear. This bitiseffective inLINorSCImode. This bitclears thelogic tobeoutput onpin
LINRX ifthefollowing conditions aremet:
*RXFUNC =0(LINRX pinisageneral-purpose I/O.)
*RXDIR =1(LINRX pinisageneral-purpose output.)
0 Read: The output onLINRX isatlogic low(0).
Write: Noeffect.
1 Read: The output onLINRX isatlogic high (1).
Write: The output onLINRX isatlogic low(0).
0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 1705 -->

www.ti.com SCI/LIN Control Registers
1705 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.20 SCIPinI/OControl Register 6(SCIPIO6)
Figure 29-49 andTable 29-37 illustrate thisregister.
Figure 29-49. SCIPinI/OControl Register 6(SCIPIO6) (offset =54h)
31 8
Reserved
R-0
3 2 1 0
Reserved TXPDR RXPDR Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 29-37. SCIPinI/OControl Register 6(SCIPIO6) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPDR Transmit pinopen drain enable. This bitiseffective inLINorSCImode. This bitenables open-drain
capability intheoutput pinLINTX, ifthefollowing conditions aremet:
*TXFUNC =0(LINTX pinisageneral-purpose I/O.)
*TXDIR =1(LINTX pinisageneral-purpose output.)
0 Open-drain functionality isdisabled; theoutput voltage isVOLorlower ifTXOUT =0,andisVOHor
higher ifTXOUT =1.
1 Open-drain functionality isenabled; theoutput voltage isVOLorlower ifTXOUT =0,andishigh-
impedance ifTXOUT =1.
1 RXPDR Receive pinopen drain enable. This bitiseffective inLINorSCImode. This bitenables open-drain
capability intheoutput pinLINRX, ifthefollowing conditions aremet:
*RXFUNC =0(LINRX pinisageneral-purpose I/O.)
*RXDIR =1(LINRX pinisageneral-purpose output.)
0 Open-drain functionality isdisabled; theoutput voltage isVOLorlower ifRXOUT =0,andisVOHor
higher ifRXOUT =1.
1 Open-drain functionality isenabled; theoutput voltage isVOLorlower ifRXOUT =0,andishigh-
impedance ifRXOUT =1.
0 Reserved 0 Writes have noeffect.

<!-- Page 1706 -->

SCI/LIN Control Registers www.ti.com
1706 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.21 SCIPinI/OControl Register 7(SCIPIO7)
Figure 29-50 andTable 29-38 illustrate thisregister.
Figure 29-50. SCIPinI/OControl Register 7(SCIPIO7) (offset =58h)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXPD RXPD Reserved
R-0 R/W-n R/W-n R/W-n
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset, Refer totheTerminal Functions inthedevice datasheet fordefault pin
settings.
Table 29-38. SCIPinI/OControl Register 7(SCIPIO7) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPD Transmit pinpullcontrol disable. This bitiseffective inLINorSCImode. This bitdisables pullcontrol
capability ontheinput pinLINTX.
0 The pullcontrol ontheLINTX pinisenabled.
1 The pullcontrol ontheLINTX pinisdisabled.
1 RXPD Receive pinpullcontrol disable. This bitiseffective inLINorSCImode. This bitdisables pullcontrol
capability ontheinput pinLINRX.
0 Pullcontrol ontheLINRX pinisenabled.
1 Pullcontrol ontheLINRX pinisdisabled.
0 Reserved 0 Writes have noeffect.

<!-- Page 1707 -->

www.ti.com SCI/LIN Control Registers
1707 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.22 SCIPinI/OControl Register 8(SCIPIO8)
Figure 29-51 andTable 29-39 illustrate thisregister.
Figure 29-51. SCIPinI/OControl Register 8(SCIPIO8) (offset =5Ch)
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXPSL RXPSL Reserved
R-0 R/W-n R/W-n R/W-n
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset, Refer totheTerminal Functions inthedevice datasheet fordefault pin
settings.
Table 29-39. SCIPinI/OControl Register 8(SCIPIO8) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPSL TXpinpullselect. This bitiseffective inLINorSCImode. This bitselects pulltype intheinput pin
LINTX.
0 The LINTX pinisapulldown.
1 The LINTX pinisapullup.
1 RXPSL RXpinpullselect. This bitiseffective inLINorSCImode. This bitselects pulltype intheinput pin
LINRX.
0 The LINRX pinisapulldown.
1 The LINRX pinisapullup.
0 Reserved 0 Writes have noeffect.

<!-- Page 1708 -->

SCI/LIN Control Registers www.ti.com
1708 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.23 LINCompare Register (LINCOMPARE)
Figure 29-52 andTable 29-40 illustrate thisregister.
Figure 29-52. LINCompare Register (LINCOMPARE) (offset =60h)
31 16
Reserved
R-0
15 10 9 8 7 3 2 0
Reserved SDEL Reserved SBREAK
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; R/WP =Read/Write inprivileged mode only; -n=value after reset
Table 29-40. LINCompare Register (LINCOMPARE) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 SDEL 2-bit synch delimiter compare. These bitsareeffective inLINmode only. These bitsareused to
configure thenumber ofTbitforthesynch delimiter inthesynch field. The default value is0.
The formula toprogram thevalue (inTbits)forthesynchronization delimiter is:
TSDEL=(SDEL +1)Tbit
0 The synch delimiter has1Tbit.
1h The synch delimiter has2Tbit.
2h The synch delimiter has3Tbit.
3h The synch delimiter has4Tbit.
15-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 SBREAK Synch break extend. These bitsareeffective inLINmode only. These bitsareused toconfigure the
number ofTbitforthesynch break toextend theminimum 13Tbitbreak field toamaximum of20Tbit
long.
Note: The default value is0,which adds nothing totheautomatically generated SYNCH BREAK.
The formula toprogram thevalue (inTbits)fortheSYNCH BREAK is:
TSYNBRK =13Tbit+(SBREAK ×Tbit)
0 The synch break hasnoadditional Tbit.
1h The synch break has1additional Tbit.
2h The synch break has2additional Tbit.
3h The synch break has3additional Tbit.
4h The synch break has4additional Tbit.
5h The synch break has5additional Tbit.
6h The synch break has6additional Tbit.
7h The synch break has7additional Tbit.

<!-- Page 1709 -->

www.ti.com SCI/LIN Control Registers
1709 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.24 LINReceive Buffer 0Register (LINRD0)
Figure 29-53 andTable 29-41 illustrate thisregister.
Figure 29-53. LINReceive Buffer 0Register (LINRD0) (offset =64h)
31 24 23 16
RD0 RD1
R-0 R-0
15 8 7 0
RD2 RD3
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-41. LINReceive Buffer 0Register (LINRD0) Field Descriptions
Bit Field Value Description
31-24 RD0 0-FFh Receive buffer 0.Byte 0oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding RDy bitfield according tothenumber
ofbytes received. Aread ofthisbyte clears theRXDY byte.
Note: RD<x-1>isequivalent todata byte <x>oftheLINframe.
23-16 RD1 0-FFh Receive buffer 1.Byte 1oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding RDy register according tothenumber
ofbytes received.
15-8 RD2 0-FFh Receive buffer 2.Byte 2oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding RDy register according tothenumber
ofbytes received.
7-0 RD3 0-FFh Receive buffer 3.Byte 3oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding RDy register according tothenumber
ofbytes received.

<!-- Page 1710 -->

SCI/LIN Control Registers www.ti.com
1710 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.25 LINReceive Buffer 1Register (LINRD1)
Figure 29-54 andTable 29-42 illustrate thisregister.
Figure 29-54. LINReceive Buffer 1Register (RD1) (offset =68h)
31 24 23 16
RD4 RD5
R-0 R-0
15 8 7 0
RD6 RD7
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 29-42. LINReceive Buffer 1Register (RD1) Field Descriptions
Bit Field Value Description
31-24 RD4 0-FFh Receive buffer 4.Byte 4oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding register according tothenumber of
bytes received.
Note: RD<x-1>isequivalent todata byte <x>oftheLINframe.
23-16 RD5 0-FFh Receive buffer 5.Byte 5oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding register according tothenumber of
bytes received.
15-8 RD6 0-FFh Receive buffer 6.Byte 6oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding register according tothenumber of
bytes received.
7-0 RD7 0-FFh Receive buffer 7.Byte 7oftheresponse data byte. Each response data-byte thatisreceived in
theSCIRXSHFT register istransferred tothecorresponding register according tothenumber of
bytes received.

<!-- Page 1711 -->

www.ti.com SCI/LIN Control Registers
1711 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.26 LINMask Register (LINMASK)
Figure 29-55 andTable 29-43 illustrate thisregister.
Figure 29-55. LINMask Register (LINMASK) (offset =6Ch)
31 24 23 16
Reserved RXIDMASK
R-0 R/WL-0
15 8 7 0
Reserved TXIDMASK
R-0 R/WL-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; -n=value after reset
Table 29-43. LINMask Register (LINMASK) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 RXIDMASK 0-FFh Receive IDmask. These bitsareeffective inLINmode only. This 8-bit mask isused forfiltering an
incoming IDmessage andcomparing ittotheID-byte. Acompare match ofthereceived IDwith
theRXIDMASK willsettheIDRXflagandtrigger anIDinterrupt ifenabled (SET IDINTin
SCISETINT). A0bitinthemask indicates thatbitiscompared totheID-byte. A1bitinthemask
indicates thatbitisfiltered andtherefore isnotused inthecompare.
15-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 TXIDMASK 0-FFh Transmit IDmask. These bitsareeffective inLINmode only. This 8-bit mask isused forfiltering
anincoming IDmessage andcomparing ittotheID-byte. Acompare match ofthereceived IDwith
theTXIDMASK willsettheIDTXflagandtrigger anIDinterrupt ifenabled (SET IDINTin
SCISETINT). A0bitinthemask indicates thatbitiscompared totheID-byte. A1bitinthemask
indicates thatbitisfiltered andtherefore isnotused forthecompare.

<!-- Page 1712 -->

SCI/LIN Control Registers www.ti.com
1712 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.27 LINIdentification Register (LINID)
Figure 29-56 andTable 29-44 illustrate thisregister.
Figure 29-56. LINIdentification Register (LINID) (offset =70h)
31 24 23 16
Reserved RECEIVED ID
R-0 R-0
15 8 7 0
ID-SlaveTask BYTE IDBYTE
R/WL-0 R/WL-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; -n=value after reset
Table 29-44. LINIdentification Register (LINID) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 RECEIVED ID 0-FFh Received identification. These bitsareeffective inLINmode only. This byte contains the
current message identifier. During header reception thereceived IDiscopied from the
SCIRXSHF register tothisbyte ifthere isnoID-parity error andthere hasbeen anRX/TX
match.
15-8 ID-SLAVETASK BYTE 0-FFh ID-SlaveTask Byte. These bitsareeffective inLINmode only. This field contains the
identifier towhich thereceived IDofanincoming header willbecompared todecide
whether areceive response, atransmit response, ornoaction needs tobeperformed by
theLINnode when aheader with thatparticular IDisreceived.
7-0 IDBYTE 0-FFh IDbyte. This field iseffective inLINmode only. This byte istheLINmode message ID.
Onamaster node, awrite tothisregister bytheCPU initiates aheader transmission. For
aslave task, thisbyte isused formessage filtering when HGEN CTRL =0.
NOTE: Forsoftware compatibility with future LINmodules, theHGEN CTRL bitmust besetto1,the
RXIDMASK field must besettoFFh, andtheTXIDMASK field must besettoFFh.

<!-- Page 1713 -->

www.ti.com SCI/LIN Control Registers
1713 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.28 LINTransmit Buffer 0Register (LINTD0)
Figure 29-57 andTable 29-45 illustrate thisregister.
Figure 29-57. LINTransmit Buffer 0Register (LINTD0) (offset =74h)
31 24 23 16
TD0 TD1
R/W-0 R/W-0
15 8 7 0
TD2 TD3
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 29-45. LINTransmit Buffer 0Register (LINTD0) Field Descriptions
Bit Field Value Description
31-24 TD0 0-FFh 8-Bit transmit buffer 0.Byte 0tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission. Once byte 0iswritten inTD0 buffer, transmission willbeinitiated.
Note: TD<x-1>isequivalent todata byte <x>oftheLINframe.
23-16 TD1 0-FFh 8-Bit transmit buffer 1.Byte 1tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
15-8 TD2 0-FFh 8-Bit transmit buffer 2.Byte 2tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
7-0 TD3 0-FFh 8-Bit transmit buffer 3.Byte 3tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
29.7.29 LINTransmit Buffer 1Register (LINTD1)
Figure 29-58 andTable 29-46 illustrate thisregister.
Figure 29-58. LINTransmit Buffer 1Register (LINTD1) (offset =78h)
31 24 23 16
TD4 TD5
R/W-0 R/W-0
15 8 7 0
TD6 TD7
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 29-46. LINTransmit Buffer 1Register (LINTD1) Field Descriptions
Bit Field Value Description
31-24 TD4 0-FFh 8-Bit transmit buffer 4.Byte 4tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
Note: TD<x-1>isequivalent todata byte <x>oftheLINframe.
23-16 TD5 0-FFh 8-Bit transmit buffer 5.Byte 5tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
15-8 TD6 0-FFh 8-Bit transmit buffer 6.Byte 6tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.
7-0 TD7 0-FFh 8-Bit transmit buffer 7.Byte 7tobetransmitted iswritten intothisregister andthen copied to
SCITXSHF fortransmission.

<!-- Page 1714 -->

MBR =0.9x VCLK
maxbaudrate
SCI/LIN Control Registers www.ti.com
1714 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.30 Maximum Baud Rate Selection Register (MBRS)
Figure 29-59 andTable 29-47 illustrate thisregister.
Figure 29-59. Maximum Baud Rate Selection Register (MBRS) (offset =7Ch)
31 16
Reserved
R-0
15 13 12 0
Reserved MBR
R-0 R/WL-DACh
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; -n=value after reset
Table 29-47. Maximum Baud Rate Selection Register (MBRS) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12-0 MBR 0-1FFFh Maximum baud rate prescaler. This bitiseffective inLINmode only. This 13-bit prescaler is
used during thesynchronization phase (see Section 29.3.1.5.2 )ofaslave module iftheADAPT
bitisset.Inthisway, aSCI/LIN slave using anautomatic orselect bitrate modes detects any
LINbuslegal rate automatically.
The MBR value should beprogrammed toallow amaximum baud rate thatisnotmore than
10% above theexpected operating baud rate intheLINnetwork. Otherwise, a00hdata byte
could mistakenly bedetected asasync break.
The default value fora70-MHz VCLK isDACh.
This MBR prescaler isused bythewake-up andidletime counters foraconstant expiration
time relative toa20-kHz rate.
(56)

<!-- Page 1715 -->

www.ti.com SCI/LIN Control Registers
1715 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
Module29.7.31 Input/Output Error Enable (IODFTCTRL) Register
Figure 29-60 andTable 29-48 illustrate thisregister. After thebasic SCI/LIN module configuration, enable
therequired Error mode tobecreated followed byIODFT Key enable.
NOTE: 1)Allthebitsareused inIODFT mode only.
2)Each IODFT areexpected tobechecked individually.
3)ISFE Error willnotbeFlagged during IODFT mode.
Figure 29-60. Input/Output Error Enable Register (IODFTCTRL) (offset =90h)
31 30 29 28 27 26 25 24
BEN PBEN CEN ISFE Reserved FEN PEN BRKDT ENA
R/WL-0 R/WL-0 R/WL-0 R/WL-0 R-0 R/W-0 R/WC-0 R/WC-0
23 21 20 19 18 16
Reserved PINSAMPLE MASK TXSHIFT
R-0 R/W-0 R/W-0
15 12 11 8
Reserved IODFTENA
R-0 R/WP-5h
7 2 1 0
Reserved LPB ENA RXP ENA
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WL=Write inLINmode only; WC=Write inSCI-compatible mode only; WP=Write in
privilege mode only; -n=value after reset
Table 29-48. Input/Output Error Enable Register (IODFTCTRL) Field Descriptions
Bit Field Value Description
31 BEN Biterror enable. This bitiseffective inLINmode only. This bitisused tocreate abiterror.
0 Nobiterror iscreated.
1 The bitreceived isORed with 1andpassed tothebitmonitor circuitry.
30 PBEN Physical buserror enable. This bitiseffective inLINmode only. This bitisused tocreate a
physical buserror.
0 Noerror iscreated.
1 The bitreceived during synch break field transmission isORed with 1andpassed tothebit
monitor circuitry.
29 CEN Checksum error enable. This bitiseffective inLINmode only. This bitisused tocreate a
checksum error.
0 Noerror iscreated.
1 The polarity oftheCTYPE (checksum type) inthereceive checksum calculator ischanged
sothatachecksum error isoccurred.
28 ISFE Inconsistent synch field (ISF) error enable. This bitiseffective inLINmode only. This bitis
used tocreate anISFerror.
0 Noerror iscreated.
1 The bitwidths inthesynch field arevaried sothattheISFcheck fails andtheerror flagis
set.
27 Reserved 0 Reads return 0.Writes have noeffect.
26 FEN Frame error enable. This bitisused tocreate aframe error.
0 Noerror iscreated.
1 The stop bitreceived isANDed with 0andpassed tothestop bitcheck circuitry.

<!-- Page 1716 -->

SCI/LIN Control Registers www.ti.com
1716 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI)/ Local Interconnect Network (LIN)
ModuleTable 29-48. Input/Output Error Enable Register (IODFTCTRL) Field Descriptions (continued)
Bit Field Value Description
25 PEN Parity error enable. This bitiseffective inSCI-compatible mode only. This bitisused to
create aparity error.
0 Noparity error occurs.
1 The parity bitreceived istoggled sothataparity error occurs.
24 BRKDT ENA Break detect error enable. This bitiseffective inSCI-compatible mode only. This bitisused
tocreate aBRKDT error.
0 Noerror iscreated.
1 The stop bitoftheframe isANDed with 0andpassed totheRSM sothataframe error
occurs. Then theRXpinisforced tocontinuous lowfor10TbitsothataBRKDT error
occurs.
32-21 Reserved 0 Reads return 0.Writes have noeffect.
20-19 PINSAMPLE MASK Pinsample mask. These bitsdefine thesample number atwhich theTXpinvalue thatis
being transmitted willbeinverted toverify thereceive pinsamples majority detection
circuitry.
Note: InIODFT mode testing forpin_sample mask must bedone with prescalar P
programmed greater than 2(P>2).
0 Nomask isused.
1h Invert theTXPinvalue atTBIT_CENTER.
2h Invert theTXPinvalue atTBIT_CENTER +SCLK.
3h Invert theTXPinvalue atTBIT_CENTER +2SCLK.
18-16 TXSHIFT Transmit shift. These bitsdefine theamount bywhich thevalue onTXpinisdelayed sothat
thevalue ontheRXpinisasynchronous. This feature isnotapplicable tothestart bit.
0 Nodelay occurs.
1h The value isdelayed by1SCLK.
2h The value isdelayed by2SCLK.
3h The value isdelayed by3SCLK.
4h The value isdelayed by4SCLK.
5h The value isdelayed by5SCLK.
6h The value isdelayed by6SCLK.
7h The value isdelayed by7SCLK.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 IODFTENA IODFT enable key. Write access permitted inPrivilege mode only.
Ah IODFT isenabled.
Allother
valuesIODFT isdisabled.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 LPB ENA Module loopback enable. Write access permitted inPrivilege mode only.
Note: Inanalog loopback mode thecomplete communication path through theI/Os
canbetested, whereas indigital loopback mode theI/Obuffers areexcluded from
thispath.
0 Digital loopback isenabled.
1 Analog loopback isenabled inmodule I/ODFT mode when IODFTENA =1010.
0 RXP ENA Module analog loopback through receive pinenable. Write access permitted inPrivilege
mode only. This bitdefines whether theI/Obuffers forthetransmit orthereceive pinare
included inthecommunication path (inanalog loopback mode).
0 Analog loopback through thetransmit pinisenabled.
1 Analog loopback through thereceive pinisenabled.