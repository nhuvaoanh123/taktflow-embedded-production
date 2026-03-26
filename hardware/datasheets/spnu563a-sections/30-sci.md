# Serial Communication Interface (SCI) Standalone Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1717-1764

---


<!-- Page 1717 -->

1717 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleChapter 30
SPNU563A -March 2018
Serial Communication Interface (SCI) Module
This chapter contains thedescription oftheserial communication interface (SCI) module.
Topic ........................................................................................................................... Page
30.1 Introduction ................................................................................................... 1718
30.2 SCICommunication Formats ............................................................................ 1720
30.3 SCIInterrupts ................................................................................................. 1725
30.4 SCIDMA Interface ........................................................................................... 1728
30.5 SCIConfigurations .......................................................................................... 1729
30.6 SCILow-Power Mode ...................................................................................... 1731
30.7 SCIControl Registers ..................................................................................... 1733
30.8 GPIO Functionality .......................................................................................... 1763

<!-- Page 1718 -->

Introduction www.ti.com
1718 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.1 Introduction
The SCImodule isauniversal asynchronous receiver-transmitter thatimplements thestandard nonreturn
tozero format. The SCIcanbeused tocommunicate, forexample, through anRS-232 port orover aK-
line.
30.1.1 SCIFeatures
The following arethefeatures oftheSCImodule:
*Standard universal asynchronous receiver-transmitter (UART) communication
*Supports full-orhalf-duplex operation
*Standard nonreturn tozero (NRZ) format
*Double-buffered receive andtransmit functions
*Supports twoindividually enabled interrupt lines: level 0andlevel 1
*Configurable frame format of3to13bitspercharacter based onthefollowing:
-Data word length programmable from onetoeight bits
-Additional address bitinaddress-bit mode
-Parity programmable forzero oroneparity bit,oddoreven parity
-Stop programmable foroneortwostop bits
*Asynchronous orisosynchronous communication modes with noCLK pin
*Two multiprocessor communication formats allow communication between more than twodevices
*Sleep mode isavailable tofree CPU resources during multiprocessor communication andthen wake
uptoreceive anincoming message
*The 24-bit programmable baud rate supports 224different baud rates provide high accuracy baud rate
selection
*Capability touseDirect Memory Access (DMA) fortransmit andreceive data
*Four error flags andFive status flags provide detailed information regarding SCIevents
*Two external pins: SCIRX andSCITX
NOTE: SCImodule does notsupport UART Hardware Flow Control. This feature canbe
implemented inSoftware using aGeneral Purpose I/Opin.
30.1.2 Block Diagram
Three Major components oftheSCIModule are:
*Transmitter
*Baud Clock Generator
*Receiver
Transmitter (TX) contains twomajor registers toperform double buffering:
*The transmitter data buffer register (SCITD) contains data loaded bytheCPU tobetransferred tothe
shift register fortransmission.
*The transmitter shift register (SCITXSHF) loads data from thedata buffer (SCITD) andshifts data onto
theSCITX pin,onebitatatime.
Baud Clock Generator
*Aprogrammable baud generator produces abaud clock scaled from VCLK.
Receiver (RX) contains twomajor registers toperform double buffering:
*The receiver shift register (SCIRXSHF) shifts data infrom theSCIRX pinonebitatatime and
transfers completed data intothereceive data buffer.
*The receiver data buffer register (SCIRD) contains received data transferred from thereceiver shift
register

<!-- Page 1719 -->

TXRDYTX EMPTY
SCIFLR.8SCIFLR.11
TX INT ENATX INT
TXWAKEAddress bit†Shift register
Transmit buffer18
SCIRX
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
registersSCITX
RXENA
SCISETINT.9SCISETINT.0
SCISETINT.1SCIGCR1.5SCISETINT.8
SCIVCLK
Peripheral
www.ti.com Introduction
1719 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleThe SCIreceiver andtransmitter aredouble-buffered, andeach hasitsown separate enable andinterrupt
bits. The receiver andtransmitter may each beoperated independently orsimultaneously infullduplex
mode.
Toensure data integrity, theSCIchecks thedata itreceives forbreaks, parity, overrun, andframing
errors. The bitrate (baud) isprogrammable toover 16million different rates through a24-bit baud-select
register. Figure 30-1 shows thedetailed SCIblock diagram.
Figure 30-1. Detailed SCIBlock Diagram

<!-- Page 1720 -->

Start0
(LSBit)1 2 3 4 5 67
(MSBit)Addr ParityStart0
(LSBit)1 2 3 4 5 67
(MSBit)ParityIdle-line mode
Address bit mode
Address bitStop
Stop
SCICommunication Formats www.ti.com
1720 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.2 SCICommunication Formats
The SCImodule canbeconfigured tomeet therequirements ofmany applications. Because
communication formats vary depending onthespecific application, many attributes oftheSCIareuser
configurable. The listbelow describes these configuration options:
*SCIFrame format
*SCITiming modes
*SCIBaud rate
*SCIMultiprocessor modes
30.2.1 SCIFrame Formats
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
address bitisillustrated inFigure 30-2.
Aparity bitispresent inevery frame when thePARITY ENA bitisset.The value oftheparity bitdepends
onthenumber ofonebitsintheframe andwhether oddoreven parity hasbeen selected viathePARITY
ENA bit.Both examples inFigure 30-2 have parity enabled.
Allframes include onestop bit,which isalways ahigh level. This high level attheendofeach frame is
used toindicate theendofaframe toensure synchronization between communicating devices. Two stop
bitsaretransmitted iftheSTOP bitinSCIGCR1 register isset.The examples shown inFigure 30-2 use
onestop bitperframe.
Figure 30-2. Typical SCIData Frame Formats

<!-- Page 1721 -->

16 SCI baud clock periods/bitMajority
vote
LSB of data Start bitSCIRXFalling edge
detected
1  2  3  4  5  6  7  8  9 10 11 1213 14 15 16 1 2 3 4 5 6  7  8  9 10 11 12 13 14 15 161 2 3 4 5
www.ti.com SCICommunication Formats
1721 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.2.2 SCITiming Mode
The SCIcanbeconfigured touseasynchronous orisosynchronous timing using TIMING MODE bitin
SCIGCR1 register.
30.2.2.1 Asynchronous Timing Mode
The asynchronous timing mode uses only thereceive andtransmit data lines tointerface with devices
using thestandard universal asynchronous receiver- transmitter (UART) protocol.
Intheasynchronous timing mode, each bitinaframe hasaduration of16SCIbaud clock periods. Each
bittherefore consists of16samples (one foreach clock period). When theSCIisusing asynchronous
mode, thebaud rates ofallcommunicating devices must match asclosely aspossible. Receive errors
result from devices communicating atdifferent baud rates.
With thereceiver intheasynchronous timing mode, theSCIdetects avalid start bitifthefirstfour samples
after afalling edge ontheSCIRX pinareoflogic level 0.Assoon asafalling edge isdetected onSCIRX,
theSCIassumes thataframe isbeing received andsynchronizes itself tothebus.
Toprevent interpreting noise asStart bitSCIexpects SCIRX linetobelowforatleast four contiguous SCI
baud clock periods todetect avalid start bit.The busisconsidered idleifthiscondition isnotmet. When a
valid start bitisdetected, theSCIdetermines thevalue ofeach bitbysampling theSCIRX linevalue
during theseventh, eighth, andninth SCIbaud clock periods. Amajority vote ofthese three samples is
used todetermine thevalue stored intheSCIreceiver shift register. Bysampling inthemiddle ofthebit,
theSCIreduces errors caused bypropagation delays andriseandfalltimes anddata linenoises.
Figure 30-3 illustrates how thereceiver samples astart bitandadata bitinasynchronous timing mode.
The transmitter transmits each bitforaduration of16SCIbaud clock periods. During thefirstclock period
forabit,thetransmitter shifts thevalue ofthatbitonto theSCITX pin.The transmitter then holds the
current bitvalue onSCITX for16SCIbaud clock periods.
Figure 30-3. Asynchronous Communication BitTiming
30.2.2.2 Isosynchronous Timing Mode
Inisosynchronous timing mode, each bitinaframe hasaduration ofexactly 1baud clock period and
therefore consists ofasingle sample. With thistiming configuration, thetransmitter andreceiver are
required tomake useoftheSCICLK pintosynchronize communication with other SCI. This mode isnot
fully supported onthisdevice because SCICLK pinisnotavailable.

<!-- Page 1722 -->

Isosynchronous baud value =
For BAUD = 0,
Isosynchronous baud value =VCLK Frequency
BAUD 1 +- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
VCLK Frequency
32- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Asynchronous baud value =
For BAUD = 0,
Asynchronous baud value =VCLK Frequency
16 * (BAUD + 1)- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
VCLK Frequency
32- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SCICommunication Formats www.ti.com
1722 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.2.3 SCIBaud Rate
The SCIhasaninternally generated serial clock determined bytheperipheral VCLK andtheprescalers
BAUD. The SCIuses the24-bit integer prescaler BAUD value intheBRS register toselect therequired
baud rates.
Inasynchronous timing mode, theSCIgenerates abaud clock according tothefollowing formula:
(57)
Inisosynchronous timing mode, theSCIgenerates abaud clock according tothefollowing formula:
(58)
30.2.4 SCIMultiprocessor Communication Modes
Insome applications, theSCImay beconnected tomore than oneserial communication device. Insuch a
multiprocessor configuration, several frames ofdata may besent toallconnected devices ortoan
individual device. Inthecase ofdata sent toanindividual device, thereceiving devices must determine
when they arebeing addressed. When amessage isnotintended forthem, thedevices canignore the
following data. When only twodevices make uptheSCInetwork, addressing isnotneeded, so
multiprocessor communication schemes arenotrequired.
SCIsupports twomultiprocessor Communication Modes which canbeselected using COMM MODE bit:
*Idle-Line Mode
*Address BitMode
When theSCIisnotused inamultiprocessor environment, software canconsider allframes asdata
frames. Inthiscase, theonly distinction between theidle-line andaddress-bit modes isthepresence ofan
extra bit(the address bit)ineach frame sent with theaddress-bit protocol.
The SCIallows full-duplex communication where data canbesent andreceived viathetransmit and
receive pins simultaneously. However, theprotocol used bytheSCIassumes thatonly onedevice
transmits data onthesame buslineatanyonetime. Noarbitration isdone bytheSCI.
NOTE: Avoid Transmitting Simultaneously ontheSame Serial Bus
The system designer must ensure thatdevices connected tothesame serial buslinedonot
attempt totransmit simultaneously. Iftwodevices aretransmitting different data, theresulting
busconflict could damage thedevice..

<!-- Page 1723 -->

Address Data Last data Start Start Start
Address frame Data frame Fewer than
10 idle bitsIdle periodOne block of framesBlocks separated by 10 or more idle bitsBlocks of frames
Data format
(pins SCIRX,
SCITX)
Data format
expanded
Stop Stop Stop
Data frameParity Parity Parity
www.ti.com SCICommunication Formats
1723 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.2.4.1 Idle-Line Multiprocessor Modes
Inidle-line multiprocessor mode, aframe thatispreceded byanidleperiod (10ormore idlebits) isan
address frame. Aframe thatispreceded byfewer than 10idlebitsisadata frame. Figure 30-4 illustrates
theformat ofseveral blocks andframes with idle-line mode.
There aretwoways totransmit anaddress frame using idle-line mode:
Method 1:Insoftware, deliberately leave anidleperiod between thetransmission ofthelastdata frame of
theprevious block andtheaddress frame ofthenew block.
Method 2:Configure theSCItoautomatically send anidleperiod between thelastdata frame ofthe
previous block andtheaddress frame ofthenew block.
Although Method 1isonly accomplished byadelay loop insoftware, Method 2canbeimplemented by
using thetransmit buffer andtheTXWAKE bitinthefollowing manner:
Step1 :Write a1totheTXWAKE bit.
Step2 :Write adummy data value totheSCITD register. This triggers theSCItobegin theidleperiod as
soon asthetransmitter shift register isempty.
Step3 :Wait fortheSCItoclear theTXWAKE flag.
Step4 :Write theaddress value toSCITD.
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
Figure 30-4. Idle-Line Multiprocessor Communication Format

<!-- Page 1724 -->

Addr Data Addr Start Start
Address frame
(address bit = 1)Data frame
(address bit = 0)Idle time
is of no
significance
One blockSeveral blocks of frames
Data format
(pins SCIRX,
Data format
expandedStart 1 0 1Idle time is not significant
ParityStop ParityStopParityStop
Idle time
is of no
significanceAddress frame
(address bit = 1)SCITX
SCICommunication Formats www.ti.com
1724 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.2.4.2 Address-Bit Multiprocessor Mode
Intheaddress-bit protocol, each frame hasanextra bitimmediately following thedata field called an
address bit.Aframe with theaddress bitsetto1isanaddress frame; aframe with theaddress bitsetto0
isadata frame. The idleperiod timing isirrelevant inthismode. Figure 30-5 illustrates theformat of
several blocks andframes with theaddress-bit mode.
When address-bit mode isused, thevalue oftheTXWAKE bitisthevalue sent astheaddress bit.To
send anaddress frame, software must settheTXWAKE bit.This bitiscleared asthecontents ofthe
SCITD areshifted from theTXWAKE register sothatallframes sent aredata except when theTXWAKE
bitiswritten asa1.
Nodummy write toSCITD isrequired before anaddress frame issent inaddress-bit mode. The firstbyte
written toSCITD after theTXWAKE bitiswritten to1istransmitted with theaddress bitsetwhen address-
bitmode isused.
Figure 30-5. Address-Bit Multiprocessor Communication Format

<!-- Page 1725 -->

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
www.ti.com SCIInterrupts
1725 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.3 SCIInterrupts
The SCImodule hastwointerrupt lines, level 0andlevel 1,tothevectored interrupt manager (VIM)
module (see Figure 30-6).Two offset registers SCIINTVECT0 andSCIINTVECT1 determine which flag
triggered theinterrupt according totherespective priority encoders. Each interrupt condition hasabitto
enable/disable theinterrupt intheSCISETINT andSCICLRINT registers, respectively.
Each interrupt also hasabitthatcanbesetasinterrupt level 0(INT0) orasinterrupt level 1(INT1). By
default, interrupts areininterrupt level 0.SCISETINTLVL sets agiven interrupt tolevel1.
SCICLEARINTLVL resets agiven interrupt level tothedefault level 0.
The interrupt vector registers SCIINTVECT0 andSCIINTVECT1 return thevector ofthepending interrupt
lineINT0 orINT1. Ifmore than oneinterrupt ispending, theinterrupt vector register holds thehighest
priority interrupt.
Figure 30-6. General Interrupt Scheme

<!-- Page 1726 -->

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
SCIInterrupts www.ti.com
1726 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleFigure 30-7. Interrupt Generation forGiven Flags
30.3.1 Transmit Interrupt
Tousetransmit interrupt functionality, SET TXINTbitmust beenabled andSET TXDMA bitmust be
cleared. The transmit ready (TXRDY) flagissetwhen theSCItransfers thecontents ofSCITD totheshift
register, SCITXSHF. The TXRDY flagindicates thatSCITD isready tobeloaded with more data. In
addition, theSCIsets theTXEMPTY bitifboth theSCITD andSCITXSHF registers areempty. IftheSET
TXINTbitisset,then atransmit interrupt isgenerated when theTXRDY flaggoes high. Transmit Interrupt
isnotgenerated immediately after setting theSET TXINTbitunlike transmit DMA request. Transmit
Interrupt isgenerated only after thefirsttransfer from SCITD toSCITXSHF, thatisfirstdata hastobe
written toSCITD bytheUser before anyinterrupt gets generated. Totransmit further data theuser can
write data toSCITD inthetransmit Interrupt service routine.
Writing data totheSCITD register clears theTXRDY bit.When thisdata hasbeen moved tothe
SCITXSHF register, theTXRDY bitissetagain. The interrupt request canbesuspended bysetting the
CLR TXINTbit;however, when theSET TXINTbitisagain setto1,theTXRDY interrupt isasserted
again. The transmit interrupt request canbeeliminated until thenext series ofvalues iswritten toSCITD,
bydisabling thetransmitter viatheTXENA bit,byasoftware reset SWnRST, orbyadevice hardware
reset.
30.3.2 Receive Interrupt
The receive ready (RXRDY) flagissetwhen theSCItransfers newly received data from SCIRXSHF to
SCIRD. The RXRDY flagtherefore indicates thattheSCIhasnew data toberead. Receive interrupts are
enabled bytheSET RXINTbit.IftheSET RXINTissetwhen theSCIsets theRXRDY flag, then a
receive interrupt isgenerated. The received data canberead intheInterrupt Service routine.
Onadevice with both SCIandaDMA controller, thebitsSET RXDMA ALL andSET RXDMA must be
cleared toselect interrupt functionality.
30.3.3 WakeUp Interrupt
SCIsets theWAKEUP flagifbusactivity ontheRXlineeither prevents power-down mode from being
entered, orRXlineactivity causes anexitfrom power-down mode. Ifenabled (SET WAKEUP INT),
wakeup interupt istriggered once WAKEUP flagisset.

<!-- Page 1727 -->

www.ti.com SCIInterrupts
1727 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.3.4 Error Interrupts
The following error detection features aresupported with Interrupt bytheSCImodule:
*Parity errors (PE)
*Frame errors (FE)
*Break Detect errors (BRKDT)
*Overrun errors (OE)
Ifanyofthese errors (PE, FE,BRKDT, OE) isflagged, aninterrupt fortheflagged errors willbegenerated
ifenabled. Amessage isvalid forboth thetransmitter andthereceiver ifthere isnoerror detected until
theendoftheframe. Each ofthese flags islocated inthereceiver status (SCIFLR) register. Further
details onthese flags areexplained inSCIFLR register description.
The SCImodule supports following 7interrupts asseen inTable 30-1 .
(1)Offset 1isthehighest priority. Offset 16isthelowest priority.Table 30-1. SCIInterrupts
Offset(1)Interrupt
0 Reserved
1 Wakeup
2 Reserved
3 Parity error
4 Reserved
5 Reserved
6 Frame error
7 Break detect error
8 Reserved
9 Overrun error
10 Reserved
11 Receive
12 Transmit
13-15 Reserved

<!-- Page 1728 -->

SCIDMA Interface www.ti.com
1728 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.4 SCIDMA Interface
DMA requests forreceive (RXDMA request) andtransmit (TXDMA request) areavailable fortheSCI
module. Refer totheDMA module chapter forDMA module configurations.
30.4.1 Receive DMA Requests
This DMA functionality isenabled/disabled bytheCPU using theSET RXDMA/CLR RXDMA bits,
respectively.
The receiver DMA request issetwhen aframe isreceived successfully andDMA functionality hasbeen
previously enabled. The RXRDY flagissetwhen theSCItransfers newly received data from the
SCIRXSHF register totheSCIRD buffer. The RXRDY flagtherefore indicates thattheSCIhasnew data to
beread. Receive DMA requests areenabled bytheSET RXINTbit.
Parity, overrun, break detect, wake-up, andframing errors generate anerror interrupt request immediately
upon detection, ifenabled, even ifthedevice isintheprocess ofaDMA data transfer. The DMA transfer
ispostponed until theerror interrupt isserved. The error interrupt candelete thisparticular DMA request
byreading thereceive buffer.
Inmultiprocessor mode, theSCIcangenerate receiver interrupts foraddress frames andDMA requests
fordata frames. This iscontrolled byanextra select bitSET RXDMA ALL.
IftheSET RXDMA ALL bitissetandtheSET RXDMA bitissetwhen theSCIsets theRXRDY flag, then
areceive DMA request isgenerated foraddress anddata frames.
IftheSET RXDMA ALL bitiscleared andtheSET RXDMA bitissetwhen theSCIsets theRXRDY flag
upon receipt ofadata frame, then areceive DMA request isgenerated. Receive interrupt requests are
generated foraddress frames.
Inmultiprocessor mode with theSLEEP bitset,noDMA isgenerated forreceived data frames. The
software must clear theSLEEP bitbefore data frames canbereceived. Table 30-2 specifies thebitvalues
forDMA requests inmultiprocessor modes.
Inmultiprocessor mode, theSCIcangenerate receiver interrupts foraddress frames andDMA requests
fordata frames orDMA requests forboth. This iscontrolled bytheSET RXDMA ALL bit.
Inmultiprocessor mode with theSLEEP bitset,noDMA isgenerated forreceived data frames. The
software must clear theSLEEP bitbefore data frames canbereceived.
Table 30-2. DMA andInterrupt Requests inMultiprocessor Modes
SET RXINT SET RXDMASET RXDMA
ALLADDR FRAME
INTADDR FRAME
DMADATA FRAME
INTDATA FRAME
DMA
0 0 x N N N N
0 1 0 Y N N Y
0 1 1 N Y N Y
1 0 x Y N Y N
1 1 0 Y N Y Y
1 1 1 Y Y Y Y

<!-- Page 1729 -->

www.ti.com SCIDMA Interface
1729 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.4.2 Transmit DMA Requests
DMA functionality isenabled/disabled bytheCPU with SET TXDMA/CLR TXDMA bits, respectively.
The TXRDY flagissetwhen theSCItransfers thecontents ofSCITD toSCITXSHF. The TXRDY flag
indicates thatSCITD isready tobeloaded with more data. Inaddition, theSCIsets theTXEMPTY bitif
both theSCITD andSCITXSHF registers areempty.
Transmit DMA requests areenabled bythesetting SET TXDMA andSET TXINTbits. IftheSET TX
DMA bitisset,then aTXDMA request issent totheDMA when data iswritten toSCITD andTXRDY is
set.Inother words, CPU needs towrite thefirstdata tostart aDMA block transfer. Forexample, wewant
totransmit adata buffer of20bytes. DMA willbesetuptotransmit 19bytes. The firstdata forDMA to
transfer isthesecond byte inthebuffer. CPU willhave towrite thefirstbyte inthebuffer totheSCITD
register tostart thetransfer.
.
30.5 SCIConfigurations
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
*Setboth theRXFUNC andTXFUNC bitsinSCIPIO0 to1toconfigure theSCIRX andSCITX pins for
SCIfunctionality.
*Select thebaud rate tobeused forcommunication byprogramming theBRS register.
*SettheCLOCK bitinSCIGCR1 to1toselect theinternal clock.
*SettheCONT bitinSCIGCR1 to1tomake SCInothaltforanemulation breakpoint until itscurrent
reception ortransmission iscomplete (this bitisused only inanemulation environment).
*SetLOOP BACK bitinSCIGCR1 to1toconnect thetransmitter tothereceiver internally (this feature
isused toperform aself-test).
*SettheRXENA bitinSCIGCR1 to1,ifdata istobereceived.
*SettheTXENA bitin SCIGCR1 to1,ifdata istobetransmitted.
*SettheSWnRST bitto1after SCIisconfigured.
*Perform receiving ortransmitting data (see Section 30.5.1 orSection 30.5.2 ).

<!-- Page 1730 -->

SCIConfigurations www.ti.com
1730 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.5.1 Receiving Data
The SCIreceiver isenabled toreceive messages ifboth theRXFUNC bitandtheRXENA bitaresetto1.
IftheRXFUNC bitisnotset,theSCIRX pinfunctions asageneral-purpose I/Opinrather than asanSCI
function pin.After avalid idleperiod isdetected, data isautomatically received asitarrives ontheSCIRX
pin.
SCIsets theRXRDY bitwhen ittransfers newly received data from SCIRXSHF toSCIRD. The SCIclears
theRXRDY bitafter thenew data inSCIRD hasbeen read. Also, asdata istransferred from SCIRXSHF
toSCIRD, theSCIsets theFE,OE, orPEflags ifanyofthese error conditions were detected inthe
received data. These error conditions aresupported with configurable interrupt capability. The wake-up
andbreak-detect status bitsarealso setifoneofthese errors occurs, butthey donotnecessarily occur at
thesame time thatnew data isbeing loaded intoSCIRD.
You canreceive data by:
1.Polling Receive Ready Flag
2.Receive Interrupt
3.DMA
Inpolling method, software canpollfortheRXRDY bitandread thedata from SCIRD register once
RXRDY issethigh. The CPU isunnecessarily overloaded byselecting thepolling method. Toavoid this,
youcanuseeither theinterrupt orDMA method. Tousetheinterrupt method, theSET RXINTbitisset.
TousetheDMA method, theSET RXDMA bitisset.Either aninterrupt oraDMA request isgenerated
themoment theRXRDY bitisset.
30.5.2 Transmitting Data
The SCItransmitter isenabled ifboth theTXFUNC bitandtheTXENA bitaresetto1.IftheTXFUNC bit
isnotset,theSCITX pinfunctions asageneral-purpose I/Opinrather than asanSCIfunction pin.Any
value written totheSCITD before TXENA issetto1isnottransmitted. Both ofthese control bitsallow for
theSCItransmitter tobeheld inactive independently ofthereceiver.
SCIwaits fordata tobewritten toSCITD, transfers ittoSCITXSHF, andtransmits thedata. The TXRDY
andTXEMPTY bitsindicate thestatus ofthetransmit buffers. That is,when thetransmitter isready for
data tobewritten toSCITD, theTXRDY bitisset.Additionally, ifboth SCITD andSCITXSHF areempty,
then theTXEMPTY bitisalso set.
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

<!-- Page 1731 -->

www.ti.com SCILow-Power Mode
1731 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.6 SCILow-Power Mode
The SCIcanbeputineither local orglobal low-power mode. Global low-power mode isasserted bythe
system andisnotcontrolled bytheSCI. During global low-power mode, allclocks totheSCIareturned off
sothemodule iscompletely inactive.
Local low-power mode isasserted bysetting thePOWERDOWN bit;setting thisbitstops theclocks tothe
SCIinternal logic andthemodule registers. Setting thePOWERDOWN bitcauses theSCItoenter local
low-power mode andclearing thePOWERDOWN bitcauses SCItoexitfrom local low-power mode. All
theregisters areaccessible during local power-down mode asanyregister access enables theclock to
SCIforthatparticular access alone.
The wake-up interrupt isused toallow theSCItoexitlow-power mode automatically when alowlevel is
detected ontheSCIRX pinandalso thisclears thePOWERDOWN bit.Ifwake-up interrupt isdisabled,
then theSCIimmediately enters low-power mode whenever itisrequested andalso anyactivity onthe
SCIRX pindoes notcause theSCItoexitlow-power mode.
NOTE: Enabling Local Low-Power Mode During Receive andTransmit
Ifthewake-up interrupt isenabled andlow-power mode isrequested while thereceiver is
receiving data, then theSCIimmediately generates awake-up interrupt toclear the
powerdown bitandprevents theSCIfrom entering low-power mode andthus completes the
current reception. Otherwise, ifthewake-up interrupt isdisabled, then theSCIcompletes the
current reception andthen enters thelow-power mode.

<!-- Page 1732 -->

SCILow-Power Mode www.ti.com
1732 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.6.1 Sleep Mode forMultiprocessor Communication
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
Following isasequence ofevents typical ofsleep mode operation:
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
Except fortheRXRDY flag, theSCIcontinues toupdate thereceiver status flags (see Table 30-11 )while
sleep mode isactive. Inthisway, ifanerror occurs onthereceive line, anapplication canimmediately
respond totheerror andtake theappropriate corrective action.
Because theRXRDY bitisnotupdated fordata frames when sleep mode isenabled, theSCIcanenable
sleep mode anduseapolling algorithm ifdesired. Inthiscase, when RXRDY isset,software knows thata
new address hasbeen received. IftheSCIisnotbeing addressed, then thesoftware should notchange
thevalue oftheSLEEP bitandshould continue topollRXRDY.

<!-- Page 1733 -->

www.ti.com SCIControl Registers
1733 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7 SCIControl Registers
These registers areaccessible in8-,16-, and32-bit reads orwrites. The SCIiscontrolled andaccessed
through theregisters listed inTable 30-3.Among thefeatures thatcanbeprogrammed aretheSCI
communication andtiming modes, baud rate value, frame format, DMA requests, andinterrupt
configuration. The base address forthecontrol registers isFFF7 E500h forSCI3 andFFF7 E700h for
SCI4.
Table 30-3. SCIControl Registers Summary
Offset Acronym Register Description Section
00h SCIGCR0 SCIGlobal Control Register 0 Section 30.7.1
04h SCIGCR1 SCIGlobal Control Register 1 Section 30.7.2
0Ch SCISETINT SCISetInterrupt Register Section 30.7.3
10h SCICLEARINT SCIClear Interrupt Register Section 30.7.4
14h SCISETINTLVL SCISetInterrupt Level Register Section 30.7.5
18h SCICLEARINTLVL SCIClear Interrupt Level Register Section 30.7.6
1Ch SCIFLR SCIFlags Register Section 30.7.7
20h SCIINTVECT0 SCIInterrupt Vector Offset 0 Section 30.7.8
24h SCIINTVECT1 SCIInterrupt Vector Offset 1 Section 30.7.9
28h SCIFORMAT SCIFormat Control Register Section 30.7.10
2Ch BRS Baud Rate Selection Register Section 30.7.11
30h SCIED Receiver Emulation Data Buffer Section 30.7.12.1
34h SCIRD Receiver Data Buffer Section 30.7.12.2
38h SCITD Transmit Data Buffer Section 30.7.12.3
3Ch SCIPIO0 SCIPinI/OControl Register 0 Section 30.7.13
40h SCIPIO1 SCIPinI/OControl Register 1 Section 30.7.14
44h SCIPIO2 SCIPinI/OControl Register 2 Section 30.7.15
48h SCIPIO3 SCIPinI/OControl Register 3 Section 30.7.16
4Ch SCIPIO4 SCIPinI/OControl Register 4 Section 30.7.17
50h SCIPIO5 SCIPinI/OControl Register 5 Section 30.7.18
54h SCIPIO6 SCIPinI/OControl Register 6 Section 30.7.19
58h SCIPIO7 SCIPinI/OControl Register 7 Section 30.7.20
5Ch SCIPIO8 SCIPinI/OControl Register 8 Section 30.7.21
90h IODFTCTRL Input/Output Error Enable Register Section 30.7.22

<!-- Page 1734 -->

SCIControl Registers www.ti.com
1734 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.1 SCIGlobal Control Register 0(SCIGCR0)
The SCIGCR0 register defines themodule reset. Figure 30-8 andTable 30-4 illustrate thisregister.
Figure 30-8. SCIGlobal Control Register 0(SCIGCR0) [offset =00]
31 16
Reserved
R-0
15 1 0
Reserved RESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; R/WP =Read/Write inprivileged mode only; -n=value after reset
Table 30-4. SCIGlobal Control Register 0(SCIGCR0) Fied Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RESET This bitresets theSCImodule.
0 SCImodule isinreset.
1 SCImodule isoutofreset.
Note: Read/Write inprivileged mode only.

<!-- Page 1735 -->

www.ti.com SCIControl Registers
1735 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.2 SCIGlobal Control Register 1(SCIGCR1)
The SCIGCR1 register defines theframe format, protocol, andcommunication mode used bytheSCI.
Figure 30-9 andTable 30-5 illustrate thisregister.
Figure 30-9. SCIGlobal Control Register 1(SCIGCR1) [offset =04h]
31 26 25 24
Reserved TXENA RXENA
R-0 R/W-0 R/W-0
23 18 17 16
Reserved CONT LOOP BACK
R-0 R/W-0 R/W-0
15 10 9 8
Reserved POWERDOWN SLEEP
R-0 R/WP-0 R/W-0
7 6 5 4 3 2 1 0
SWnRST Reserved CLOCK STOP PARITY PARITY ENA TIMING MODE COMM MODE
R/W-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
NOTE: The SCIGCR1 Control Register Bitsshould notbechanged during Frame Transmission or
Reception.
Table 30-5. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25 TXENA Transmit enable. Data istransferred from SCITD totheSCITXSHF shift outregister only when the
TXENA bitisset.
0 Disable transfers from SCITD toSCITXSHF.
1 Enable SCItotransfer data from SCITD toSCITXSHF.
Note: Data written toSCITD orthetransmit multi-buffer before TXENA issetisnot
transmitted. IfTXENA iscleared while transmission isongoing, thedata previously written
toSCITD issent.
24 RXENA Receive enable. RXENA allows orprevents thetransfer ofdata from SCIRXSHF toSCIRD.
0 The receiver willnottransfer data from theshift buffer tothereceive buffer.
1 The receiver willtransfer data from theshift buffer tothereceive buffer.
Note: Clearing RXENA stops received characters from being transferred intothereceive
buffer ormulti-buffers, prevents theRXstatus flags from being updated byreceive data, and
inhibits both receive anderror interrupts. However, theshift register continues toassemble
data regardless ofthestate ofRXENA.
Note: IfRXENA iscleared before aframe iscompletely received, thedata from theframe is
nottransferred intothereceive buffer.
Note: IfRXENA issetbefore aframe iscompletely received, thedata from theframe is
transferred intothereceive buffer. IfRXENA issetwhile SCIRXSHF isintheprocess of
assembling aframe, thestatus flags arenotassured tobeaccurate forthatframe. Toensure
thatthestatus flags correctly reflect what was detected onthebusduring aparticular frame,
RXENA should besetbefore thedetection ofthatframe.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 CONT Continue onsuspend. This bithasaneffect only when aprogram isbeing debugged with an
emulator, anditdetermines how theSCIoperates when theprogram issuspended. The
0 When debug mode isentered, theSCIstate machine isfrozen. Transmissions arehalted and
resume when debug mode isexited.
1 When debug mode isentered, theSCIcontinues tooperate until thecurrent transmit andreceive
functions arecomplete.

<!-- Page 1736 -->

SCIControl Registers www.ti.com
1736 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-5. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions (continued)
Bit Field Value Description
16 LOOP BACK Loopback bit.The self-checking option fortheSCIcanbeselected with thisbit.IftheSCITX and
SCIRX pins areconfigured with SCIfunctionality, then theSCITX pinisinternally connected tothe
SCIRX pin.Externally, during loop back operation, theSCITX pinoutputs ahigh value andthe
SCIRX pinisinahigh-impedance state. Ifthisbitvalue ischanged while theSCIistransmitting or
receiving data, errors may result.
0 Loop back mode isdisabled.
1 Loop back mode isenabled.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9 POWERDOWN Power down. When thePOWERDOWN bitisset,theSCIattempts toenter local low-power mode.
IfthePOWERDOWN bitissetwhile thereceiver isactively receiving data andthewake-up
interrupt isenabled, then theSCIimmediately asserts anerror interrupt toprevent low-power mode
from being entered. Only Privilege mode writes allowed.
0 Normal operation.
1 Low-power mode isenabled.
8 SLEEP SCIsleep. Inamultiprocessor configuration, thisbitcontrols thereceive sleep function. Clearing
thisbitbrings theSCIoutofsleep mode.
0 Sleep mode isdisabled.
1 Sleep mode isenabled.
Note: Thereceiver stilloperates when theSLEEP bitisset;however, RXRDY isupdated and
SCIRD isloaded with new data only when anaddress frame isdetected. Theremaining
receiver status flags areupdated andanerror interrupt isrequested ifthecorresponding
interrupt enable bitisset,regardless ofthevalue oftheSLEEP bit.Inthisway, ifanerror is
detected onthereceive data linewhile theSCIisasleep, software canpromptly deal with the
error condition.
Note: TheSLEEP bitisnotautomatically cleared when anaddress byte isdetected.
See Section 30.6.1 formore information onusing theSLEEP bitformultiprocessor communication.
7 SWnRST Software reset (active low). This bitiseffective inLINandSCImodes.
0 The SCIisinitsreset state; nodata willbetransmitted orreceived. Writing a0tothisbitinitializes
theSCIstate machines andoperating flags asdefined inTable 30-11 andTable 30-12 .Allaffected
logic isheld inthereset state until a1iswritten tothisbit.
1 The SCIisinitsready state; transmission andreception canbedone. After thisbitissetto1,the
configuration ofthemodule should notchange.
Note: TheSCIshould only beconfigured while SWnRST =0.
6 Reserved 0 Reads return 0.Writes have noeffect.
5 CLOCK SCIinternal clock enable. The CLOCK bitdetermines thesource ofthemodule clock onthe
SCICLK pin.
0 The external SCICLK istheclock source.
1 The internal SCICLK istheclock source.
Note: Ifanexternal clock isselected, then theinternal baud rategenerator andbaud rate
registers arebypassed. Themaximum frequency allowed foranexternally sourced SCIclock
isVCLK/16.
4 STOP SCInumber ofstop bitsperframe.
0 One stop bitisused.
1 Two stop bitsareused.
Note: Thereceiver checks foronly onestop bit.However inidle-line mode, thereceiver
waits until theendofthesecond stop bit(ifSTOP =1)tobegin checking foranidleperiod .

<!-- Page 1737 -->

www.ti.com SCIControl Registers
1737 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-5. SCIGlobal Control Register 1(SCIGCR1) Field Descriptions (continued)
Bit Field Value Description
3 PARITY SCIparity odd/even selection. IfthePARITY ENA bitisset,PARITY designates oddoreven parity.
0 Odd parity isused.
1 Even parity isused.
Theparity bitiscalculated based onthedata bitsineach frame andtheaddress bit(in
address-bit mode). Thestart andstop fields intheframe arenotincluded intheparity
calculation.
Foroddparity, theSCItransmits andexpects toreceive avalue intheparity bitthatmakes
oddthetotal number ofbitsintheframe with thevalue of1.
Foreven parity, theSCItransmits andexpects toreceive avalue intheparity bitthatmakes
even thetotal number ofbitsintheframe with thevalue of1.
2 PARITY ENA Parity enable. This bitenables ordisables theparity function.
0 Parity isdisabled; noparity bitisgenerated during transmission orisexpected during reception.
1 Parity isenabled. Aparity bitisgenerated during transmission andisexpected during reception.
1 TIMING MODE SCItiming mode bit.
0 Synchronous timing isused.
1 Asynchronous timing isused.
0 COMM MODE SCIcommunication mode bit.
0 Idle-line mode isused.
1 Address-bit mode isused.

<!-- Page 1738 -->

SCIControl Registers www.ti.com
1738 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.3 SCISetInterrupt Register (SCISETINT)
Figure 30-10 andTable 30-6 illustrate thisregister. SCISETINT register isused toenable therequired
interrupts supported bythemodule.
Figure 30-10. SCISetInterrupt Register (SCISETINT) [offset =0Ch]
31 27 26 25 24
Reserved SET FEINT SET OEINT SET PEINT
R-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
Reserved SET
RXDMA ALLSET
RXDMASET
TXDMA
R-0 R/W-0 R/W-0 R/W-0
15 10 9 8
Reserved SET RXINT SET TXINT
R-0 R/W-0 R/W-0
7 2 1 0
Reserved SET
WAKEUP INTSET
BRKDT INT
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-6. SCISetInterrupt Register (SCISETINT) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 SET FEINT Setframing-error interrupt. Setting thisbitenables theSCImodule togenerate aninterrupt
when aframing error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
25 SET OEINT Setoverrun-error interrupt. Setting thisbitenables theSCImodule togenerate aninterrupt
when anoverrun error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
24 SET PEINT Setparity interrupt. Setting thisbitenables theSCImodule togenerate aninterrupt when a
parity error occurs.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 SET RXDMA ALL Setreceive DMA all.This bitdetermines ifaseparate interrupt isgenerated fortheaddress
frames sent inmultiprocessor communications. When thisbitis0,RXinterrupt requests are
generated foraddress frames andDMA requests aregenerated fordata frames. When thisbit
is1,RXDMA requests aregenerated forboth address anddata frames.
0 Read: The DMA request isdisabled foraddress frames (the receive interrupt request isenabled
foraddress frames).
Write: Noeffect.
1 Read orwrite: The DMA request isenabled foraddress anddata frames
17 SET RXDMA Setreceiver DMA. Toenable receiver DMA requests, thisbitmust beset.Ifitiscleared,
interrupt requests aregenerated depending onbitSCISETINT.
0 Read: The DMA request isdisabled.
Write: Noeffect.
1 Read orwrite: The DMA request isenabled foraddress anddata frames.

<!-- Page 1739 -->

www.ti.com SCIControl Registers
1739 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-6. SCISetInterrupt Register (SCISETINT) Field Descriptions (continued)
Bit Field Value Description
16 SET TXDMA Settransmit DMA. Toenable DMA requests forthetransmitter, thisbitmust beset.Ifitis
cleared, interrupt requests aregenerated depending onSET TXINTbit(SCISETINT).
0 Read: Transmit DMA request isdisabled.
Write: Noeffect.
1 Read orwrite: Transmit DMA request isenabled.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9 SET RXINT Receiver interrupt enable. Setting thisbitenables theSCItogenerate areceive interrupt after a
frame hasbeen completely received andthedata isbeing transferred from SCIRXSHF to
SCIRD.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
8 SET TXINT Settransmitter interrupt. Setting thisbitenables theSCItogenerate atransmit interrupt asdata
isbeing transferred from SCITD toSCITXSHF andtheTXRDY bitisbeing set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 SET WAKEUP INT Setwakeup interrupt. Setting thisbitenables theSCItogenerate awakeup interrupt and
thereby exitlow-power mode. Ifenabled, thewakeup interrupt isasserted when local low-power
mode isrequested while thereceiver isbusy orifalowlevel isdetected ontheSCIRX pin
during low-power mode.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.
0 SET BRKDT INT Setbreakdetect interrupt. Setting thisbitenables theSCItogenerate anerror interrupt ifa
break condition isdetected ontheSCIRX pin.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read orwrite: The interrupt isenabled.

<!-- Page 1740 -->

SCIControl Registers www.ti.com
1740 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.4 SCIClear Interrupt Register (SCICLEARINT)
Figure 30-11 andTable 30-7 illustrate thisregister. SCICLEARINT register isused toclear theselected
enabled interrupts with outaccessing SCISETINT register.
Figure 30-11. SCIClear Interrupt Register (SCICLEARINT) [offset =10h]
31 27 26 25 24
Reserved CLR FEINT CLR OEINT CLR PEINT
R-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
Reserved CLR
RXDMA ALLCLR
RXDMACLR
TXDMA
R-0 R/W-0 R/W-0 R/W-0
15 10 9 8
Reserved CLR RXINT CLR TXINT
R-0 R/W-0 R/W-0
7 2 1 0
Reserved CLR
WAKEUP INTCLR
BRKDT INT
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-7. SCIClear Interrupt Register (SCICLEARINT) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 CLR FEINT Clear framing-error interrupt. This bitdisables theframing-error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
25 CLR CEINT Clear overrun-error interrupt. This bitdisables theSCIoverrun error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
24 CLR PEINT Clear parity interrupt. This bitdisables theparity error interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 CLR RXDMA ALL Clear receive DMA all.This bitclears thereceive DMA request foraddress frames when set.
Only receive data frames generate aDMA request.
0 Read: Receive DMA request foraddress frames isdisabled; Instead, RXinterrupt requests are
enabled foraddress frames. Receive DMA requests arestillenabled fordata frames.
Write: Noeffect.
1 Read: The receive DMA request foraddress anddata frames isenabled.
Write: The receive DMA request foraddress anddata frames isdisabled.

<!-- Page 1741 -->

www.ti.com SCIControl Registers
1741 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-7. SCIClear Interrupt Register (SCICLEARINT) Field Descriptions (continued)
Bit Field Value Description
17 CLR RXDMA Clear receive DMA request. This bitdisables thereceive DMA request when set.
0 Read: The DMA request isdisabled.
Write: Noeffect.
1 Read: The receive DMA request isenabled.
Write: The receive DMA request forisdisabled.
16 CLR TXDMA Clear transmit DMA request. This bitdisables thetransmit DMA request when set.
0 Read: Transmit DMA request isdisabled.
Write: Noeffect.
1 Read: The transmit DMA request isenabled.
Write: The transmit DMA request forisdisabled.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9 CLR RXINT Clear receiver interrupt. This bitdisables thereceiver interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
8 CLR TXINT Clear transmitter interrupt. This bitdisables thetransmitter interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CLR WAKEUP INT Clear wakeup interrupt. This bitdisables thewakeup interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.
0 CLR BRKDT INT Clear breakdetect interrupt. This bitdisables thebreak-detect interrupt when set.
0 Read: The interrupt isdisabled.
Write: Noeffect.
1 Read: The interrupt isenabled.
Write: The interrupt isdisabled.

<!-- Page 1742 -->

SCIControl Registers www.ti.com
1742 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.5 SCISetInterrupt Level Register (SCISETINTLVL)
Figure 30-12 andTable 30-8 illustrate thisregister. This register isused tosettheinterrupt level forthe
supported interrupts.
Figure 30-12. SCISetInterrupt Level Register (SCISETINTLVL) [offset =14h]
31 27 26 25 24
Reserved SET FE
INTLVLSET OE
INTLVLSET PE
INTLVL
R-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedSET RXDMA
ALL INTLVLReserved
R-0 R/W-0 R-0
15 10 9 8
Reserved SET RX
INTLVLSET TX
INTLVL
R-0 R/W-0 R/W-0
7 2 1 0
ReservedSET WAKEUP
INTLVLSET BRKDT
INTLVL
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-8. SCISetInterrupt Level Register (SCISETINTLVL) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 SET FEINTLVL Setframing-error interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
25 SET CEINTLVL Setoverrun-error interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
24 SET PEINTLVL Setparity error interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 SET RXDMA ALL LVL Setreceive DMA allinterrupt levels.
0 Read: The receive interrupt request foraddress frames ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The receive interrupt request foraddress frames ismapped totheINT1 line.
17-10 Reserved 0 Reads return 0.Writes have noeffect.
9 SET RXINTLVL Setreceiver interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.

<!-- Page 1743 -->

www.ti.com SCIControl Registers
1743 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-8. SCISetInterrupt Level Register (SCISETINTLVL) Field Descriptions (continued)
Bit Field Value Description
8 SET TXINTLVL Settransmitter interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 SET WAKEUP INTLVL Setwakeup interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
0 SET BRKDT INTLVL Setbreakdetect interrupt level.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read orwrite: The interrupt level ismapped totheINT1 line.
30.7.6 SCIClear Interrupt Level Register (SCICLEARINTLVL)
Figure 30-13 andTable 30-9 illustrate thisregister.
Figure 30-13. SCIClear Interrupt Level Register (SCICLEARINTLVL) [offset =18h]
31 27 26 25 24
Reserved CLR FE
INTLVLCLR OE
INTLVLCLR PE
INTLVL
R-0 R/W-0 R/W-0 R/W-0
23 19 18 17 16
ReservedCLR RXDMA
ALL INTLVLReserved
R-0 R/W-0 R-0
15 10 9 8
Reserved CLR RX
INTLVLCLR TX
INTLVL
R-0 R/W-0 R/W-0
7 2 1 0
ReservedCLR WAKEUP
INTLVLCLR BRKDT
INTLVL
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-9. SCIClear Interrupt Level Register (SCICLEARINTLVL) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 CLR FEINTLVL Clear framing-error interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.

<!-- Page 1744 -->

SCIControl Registers www.ti.com
1744 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-9. SCIClear Interrupt Level Register (SCICLEARINTLVL) Field Descriptions (continued)
Bit Field Value Description
25 CLR CEINTLVL Clear overrun-error interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
24 CLR PEINTLVL Clear parity interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18 CLR RXDMA ALL LVL Clear receive DMA interrupt level.
0 Read: The receive interrupt request foraddress frames ismapped totheINT0 line.
Write: Noeffect.
1 Read: The receive interrupt request foraddress frames ismapped totheINT1 line.
Write: The receive interrupt request foraddress frames ismapped totheINT0 line.
17-10 Reserved 0 Reads return 0.Writes have noeffect.
9 CLR RXINTLVL Clear receiver interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
8 CLR TXINTLVL Clear transmitter interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 CLR WAKEUP INTLVL Clear wakeup interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.
0 CLR BRKDT INTLVL Clear breakdetect interrupt.
0 Read: The interrupt level ismapped totheINT0 line.
Write: Noeffect.
1 Read: The interrupt level ismapped totheINT1 line.
Write: The interrupt level ismapped totheINT0 line.

<!-- Page 1745 -->

www.ti.com SCIControl Registers
1745 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.7 SCIFlags Register (SCIFLR)
Figure 30-14 andTable 30-10 illustrate thisregister.
Figure 30-14. SCIFlags Register (SCIFLR) [offset =1Ch]
31 27 26 25 24
Reserved FE OE PE
R-0 R/W-0 R/W-0 R/W-0
23 16
Reserved
R-0
15 13 12 11 10 9 8
Reserved RXWAKE TXEMPTY TXWAKE RXRDY TXRDY
R-0 R/W-0 R/W-1 R/W-0 R/W-0 R/W-1
7 4 3 2 1 0
Reserved BUSY IDLE WAKE UP BRKDT
R-0 R/W-0 R-0 R/WL-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-10. SCIFlags Register (SCIFLR) Field Descriptions
Bit Field Value Description
31-27 Reserved Reads return 0.Writes have noeffect.
26 FE Framing error flag. This bitiseffective inLINorSCI-compatible mode. This bitissetwhen an
expected stop bitisnotfound. InSCIcompatibility mode, only thefirststop bitischecked. The
missing stop bitindicates thatsynchronization with thestart bithasbeen lostandthatthecharacter
isincorrectly framed. Detection ofaframing error causes theSCI/LIN togenerate anerror interrupt
iftheSET FEINTbit(SCISETINT[26]). The framing error flagiscleared bythefollowing:
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
25 OE Overrun error flag. This bitissetwhen thetransfer ofdata from SCIRXSHF toSCIRD overwrites
unread data already inSCIRD. Detection ofanoverrun error causes theLINtogenerate anerror
interrupt iftheSET OEINTbit(SCISETINT[25]) isset.The OEflagisreset bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Nooverrun error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Anoverrun error hasbeen detected since thelastclear.
Write: The bitiscleared to0.

<!-- Page 1746 -->

SCIControl Registers www.ti.com
1746 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-10. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
24 PE Parity error flag. This bitissetwhen aparity error isdetected inthereceived data. InSCIaddress-
bitmode, theparity iscalculated onthedata andaddress bitfields ofthereceived frame. Inidle-
linemode, only thedata isused tocalculate parity. Anerror isgenerated when acharacter is
received with amismatch between thenumber of1sanditsparity bit.Iftheparity function is
disabled (SCIGCR[2] =0),thePEflagisdisabled andread as0.Detection ofaparity error causes
theLINtogenerate anerror interrupt iftheSET PEINTbit(SCISETINT[24]) isset.The PEbitis
reset bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reception ofanew character orframe, depending onwhether themodule isinSCIcompatible
orLINmode, respectively
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Noparity error hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Aparity error hasbeen detected since thelastclear.
Write: The bitiscleared to0.
23-13 Reserved 0 Reads return 0.Writes have noeffect.
12 RXWAKE Receiver wakeup detect flag. The SCIsets thisbittoindicate thatthedata currently inSCIRD isan
address. RXWAKE iscleared bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Upon receipt ofadata frame
0 The data inSCIRD isnotanaddress.
1 The data inSCIRD isanaddress.
11 TXEMPTY Transmitter empty flag. This flagindicates thetransmitter 'sbuffer register(s) (SCITD/TDy) andshift
register (SCITXSHF) areempty.
Note: TheRESET bit,anactive SWnRST (SCIGCR1[7]), orasystem reset sets thisbit.This
bitdoes notcause aninterrupt request.
0 Transmitter buffer orshift register (orboth) areloaded with data.
1 Transmitter buffer andshift registers areboth empty.
10 TXWAKE Transmitter wakeup method select. The TXWAKE bitcontrols whether thedata inSCITD should be
sent asanaddress ordata frame using multiprocessor communication format. This bitissetto1or
0bysoftware before abyte iswritten toSCITD andiscleared bytheSCIwhen data istransferred
from SCITD toSCITXSHF orbyasystem reset.
Note: TXWAKE isnotcleared bytheSWnRST bit.
Address-bit mode
0 Frame tobetransmitted willbedata (address bit=0).
1 Frame tobetransmitted willbeanaddress (address bit=1).
Idle-line mode
0 The frame tobetransmitted willbedata.
1 The following frame tobetransmitted willbeanaddress (writing a1tothisbitfollowed bywriting
dummy data totheSCITD willresult inaidleperiod of11bitperiods before thenext frame is
transmitted).

<!-- Page 1747 -->

www.ti.com SCIControl Registers
1747 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-10. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
9 RXRDY Receiver ready flag. The receiver sets thisbittoindicate thattheSCIRD contains new data andis
ready toberead bytheCPU orDMA. The SCIgenerates areceive interrupt when RXRDY flagbit
issetiftheSET RXINTbit(SCISETINT[9]) isset.RXRDY iscleared bythefollowing:
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
8 TXRDY Transmitter buffer register ready flag. When set,thisbitindicates thatthetransmit buffer isready to
getanother character from aCPU orDMA write.
Writing data toSCITD automatically clears thisbit.This bitissetafter thedata oftheTXbuffer is
shifted intotheSCITXSHF register. This event cantrigger atransmit interrupt after data iscopied to
theTXshift register SCITXSHF, iftheinterrupt enable bitTXINT isset.
Note: 1)TXRDY isalso setto1bysetting oftheRESET bit,enabling SWnRST, orbya
system reset.
2)TheTXRDY flagcannot becleared byreading thecorresponding interrupt offset inthe
SCIINTVECT0/1 register.
3)Thetransmit interrupt request canbeeliminated until thenext series ofdata written into
thetransmit buffers LINTD0 andLINTD1, bydisabling thecorresponding interrupt viathe
SCICLEARINT register orbydisabling thetransmitter viatheTXENA bit(SCIGCR1[25]).
0 SCITD isfull.
1 SCITD isready toreceive thenext character.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3 BUSY Bus busy flag. TThis bitindicates whether thereceiver isintheprocess ofreceiving aframe. As
soon asthereceiver detects thebeginning ofastart bit,theBUSY bitissetto1.When the
reception ofaframe iscomplete, theSCIclears theBUSY bit.IfSET WAKEUP INTbit
(SCISETINT[2]) issetandpower down isrequested while thisbitisset,theSCIautomatically
prevents low-power mode from being entered andgenerates wakeup interrupt. The BUSY bitis
controlled directly bytheSCIreceiver, butthisbitcanalso becleared bythefollowing:
*Setting theSWnRST bit
*Setting oftheRESET bit
*Asystem reset occurring
0 The receiver isnotcurrently receiving aframe.
1 The receiver iscurrently receiving aframe.
2 IDLE SCIreceiver inidlestate. While thisbitisset,theSCIlooks foranidleperiod toresynchronize itself
with thebitstream. The receiver does notreceive anydata while thebitisset.The busmust beidle
for11bitperiods toclear thisbit.The SCIenters theidlestate ifoneofthefollowing events occurs:
*Asystem reset
*AnSCIsoftware reset
*Apower down
*The RXpinisconfigured asageneral I/Opin
0 The idleperiod hasbeen detected; theSCIisready toreceive.
1 The idleperiod hasnotbeen detected; theSCIwillnotreceive anydata.

<!-- Page 1748 -->

SCIControl Registers www.ti.com
1748 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-10. SCIFlags Register (SCIFLR) Field Descriptions (continued)
Bit Field Value Description
1 WAKEUP Wakeup flag. This bitissetbytheSCIwhen receiver ortransmitter activity hastaken themodule
outofpower-down mode. Aninterrupt isgenerated iftheSET WAKEUP INTbit(SCISETINT[2]) is
set.Itiscleared bythefollowing:
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
0 BRKDT SCIbreak-detect flag. This bitissetwhen theSCIdetects abreak condition ontheLINRX pin.A
break condition occurs when theSCIRX pinremains continuously lowforatleast 10bitsafter a
missing firststop bit,thatis,after aframing error. Detection ofabreak condition causes theSCIto
generate anerror interrupt iftheSET BRKDT INTbit(SCISETINT[0]) isset.The BRKDT bitisreset
bythefollowing:
*Setting oftheSWnRST bit
*Setting oftheRESET bit
*Asystem reset
*Writing a1tothisbit
*Reading thecorresponding interrupt offset inSCIINTVECT0/1
0 Read: Nobreak condition hasbeen detected since thelastclear.
Write: Noeffect.
1 Read: Abreak condition hasbeen detected.
Write: The bitiscleared to0.
(1)The flags arefrozen with their reset value while SWnRST =0.Table 30-11. SCIReceiver Status Flags
SCIFlag Register Bit Value After Reset(1)
FE SCIFLR 26 0
OE SCIFLR 25 0
PE SCIFLR 24 0
RXWAKE SCIFLR 12 0
RXRDY SCIFLR 9 0
BRKDT SCIFLR 0 0
(1)The flags arefrozen with their reset value while SWnRST =0.Table 30-12. SCITransmitter Status Flags
SCIFlag Register Bit Value After Reset(1)
TXEMPTY SCIFLR 11 1
TXRDY SCIFLR 8 1

<!-- Page 1749 -->

www.ti.com SCIControl Registers
1749 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.8 SCIInterrupt Vector Offset 0(SCIINTVECT0)
Figure 30-15 andTable 30-13 illustrate thisregister.
Figure 30-15. SCIInterrupt Vector Offset 0(SCIINTVECT0) [offset =20h]
31 16
Reserved
R-0
15 4 3 0
Reserved INTVECT0
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 30-13. SCIInterrupt Vector Offset 0(SCIINTVECT0) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 INVECT0 0-Fh Interrupt vector offset forINT0. This register indicates theoffset forinterrupt lineINT0. Aread to
thisregister updates itsvalue tothenext highest priority pending interrupt inSCIFLR andclears
theflaginSCIFLR corresponding totheoffset thatwas read. See Table 30-1 foralistofthe
interrupts.
Note: Theflags forthereceive (SCIFLR[9]) andthetransmit (SCIFLR[8]) interrupt cannot be
cleared byreading thecorresponding offset vector inthisregister (see detailed description
inSCIFLR register).
30.7.9 SCIInterrupt Vector Offset 1(SCIINTVECT1)
Figure 30-16 andTable 30-14 illustrate thisregister.
Figure 30-16. SCIInterrupt Vector Offset 1(SCIINTVECT1) [offset =24h]
31 16
Reserved
R-0
15 4 3 0
Reserved INTVECT1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 30-14. SCIInterrupt Vector Offset 1(SCIINTVECT1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 INVECT1 0-Fh Interrupt vector offset forINT1. This register indicates theoffset forinterrupt lineINT1. Aread to
thisregister updates itsvalue tothenext highest priority pending interrupt inSCIFLR andclears
theflaginSCIFLR corresponding totheoffset thatwas read. See Table 30-1 forlistofinterrupts.
Note: Theflags forthereceive (SCIFLR[9]) andthetransmit (SCIFLR[8]) interrupt cannot be
cleared byreading thecorresponding offset vector inthisregister (see detailed description
inSCIFLR register).

<!-- Page 1750 -->

SCIControl Registers www.ti.com
1750 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.10 SCIFormat Control Register (SCIFORMAT)
Figure 30-17 andTable 30-15 illustrate thisregister.
Figure 30-17. SCIFormat Control Register (SCIFORMAT) [offset =28h]
31 16
Reserved
R-0
15 3 2 0
Reserved CHAR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-15. SCIFormat Control Register (SCIFORMAT) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 CHAR Character length control bits. These bitssettheSCIcharacter length from 1to8bits.
When data offewer than eight bitsinlength isreceived, itisleft-justified inSCIRD and
padded with trailing zeros.
Data read from theSCIRD should beshifted bysoftware tomake thereceived data right-
justified.
Data written totheSCITD should beright-justified butdoes notneed tobepadded with
leading zeros.
0 The character is1bitlong.
1h The character is2bitslong.
2h The character is3bitslong.
3h The character is4bitslong.
4h The character is5bitslong.
5h The character is6bitslong.
6h The character is7bitslong.
7h The character is8bitslong.

<!-- Page 1751 -->

Isosynchronous baud value = ) (VCLK Frequency
2
Asynchronous baud value = ) (VCLK Frequency
32
Isosynchronous baud value = ) (VCLK Frequency
Baud + 1
Asynchronous baud value = ) (VCLK Frequency
16(Baud + 1)
www.ti.com SCIControl Registers
1751 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.11 Baud Rate Selection Register (BRS)
This section describes thebaud rate selection register. Figure 30-18 andTable 30-16 illustrate this
register.
Figure 30-18. Baud Rate Selection Register (BRS) [offset =2Ch]
31 24 23 16
Reserved BAUD
R-0 R/W-0
15 0
BAUD
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-16. Baud Rate Selection Register (BRS) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 BAUD 0-FF FFFFh SCI24-bit baud selection.
The SCIhasaninternally generated serial clock determined bytheVCLK andtheprescalers
BAUD inthisregister. The SCIuses the24-bit integer prescaler BAUD value ofthisregister to
select oneofover 16,700,000.
The baud rate canbecalculated using thefollowing formulas:
(59)
(60)
ForBAUD =0,
(61)
(62)
Table 30-17 contains comparative baud values fordifferent Pvalues, with VCLK =50MHz, for
asynchronous mode..
(1)VCLK =50MHz
(2)Values areindecimal except forcolumn 2.Table 30-17. Comparative Baud Values forDifferent PValues, Asynchronous Mode(1)(2)
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

<!-- Page 1752 -->

SCIControl Registers www.ti.com
1752 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.12 SCIData Buffers (SCIED, SCIRD, SCITD)
The SCIhasthree addressable registers inwhich transmit andreceive data isstored.
30.7.12.1 Receiver Emulation Data Buffer (SCIED)
The SCIED register isaddressed atalocation different from SCIRD, butisphysically thesame register.
Figure 30-19 andTable 30-18 illustrate thisregister.
Figure 30-19. Receiver Emulation Data Buffer (SCIED) [offset =30h]
31 16
Reserved
R-0
15 8 7 0
Reserved ED
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 30-18. Receiver Emulation Data Buffer (SCIED) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 ED 0-FFh Emulator data. Reading SCIED[7:0] does notclear theRXRDY flag(SCIFLR[9]), unlike reading
SCIRD. This register should beused only byanemulator thatmust continually read thedata
buffer without affecting theRXRDY flag.
30.7.12.2 Receiver Data Buffer (SCIRD)
This register provides alocation forthereceiver data. Figure 30-20 andTable 30-19 illustrate thisregister.
Figure 30-20. Receiver Data Buffer (SCIRD) [offset =34h]
31 16
Reserved
R-0
15 8 7 0
Reserved RD
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 30-19. Receiver Data Buffer (SCIRD) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 RD 0-FFh Receiver data. When aframe hasbeen completely received, thedata intheframe istransferred
from thereceiver shift register SCIRXSHF tothisregister. Asthistransfer occurs, theRXRDY flag
(SCIFLR[9]) issetandareceive interrupt isgenerated ifSET RXINTbit(SCISETINT[9]) isset.
Note: When thedata isread from SCIRD, theRXRDY flag(SCIFLR[9]) isautomatically
cleared.
NOTE: When theSCIreceives data thatisfewer than eight bitsinlength, itloads thedata intothis
register inaleft-justified format padded with trailing zeros. Therefore, theuser software
should perform alogical shift onthedata bythecorrect number ofpositions tomake itright
justified.

<!-- Page 1753 -->

www.ti.com SCIControl Registers
1753 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.12.3 Transmit Data Buffer Register (SCITD)
Data tobetransmitted iswritten totheSCITD register. Figure 30-21 andTable 30-20 illustrate this
register.
Figure 30-21. Transmit Data Buffer Register (SCITD) [offset =38h]
31 16
Reserved
R-0
15 8 7 0
Reserved TD
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-20. Transmit Data Buffer Register (SCITD) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 TD 0-FFh Transmit data. Data tobetransmitted iswritten totheSCITD register. The transfer ofdata from
thisregister tothetransmit shift register SCITXSHF sets theTXRDY flag(SCIFLR[8]), which
indicates thatSCITD isready tobeloaded with another byte ofdata.
Note: IfSET TXINTbit(SCISETINT[8] isset,thisdata transfer also causes aninterrupt.
NOTE: Data written totheSCITD register thatisfewer than eight bitslong must beright-justified,
butitdoes notneed tobepadded with leading zeros.
30.7.13 SCIPinI/OControl Register 0(SCIPIO0)
Figure 30-22 andTable 30-21 illustrate thisregister.
Figure 30-22. SCIPinI/OControl Register 0(SCIPIO0) [offset =3Ch]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXFUNC RXFUNC Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-21. SCIPinI/OControl Register 0(SCIPIO0) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXFUNC Transfer function. This bitdefines thefunction ofpinSCITX.
0 SCITX isageneral-purpose digital I/Opin.
1 SCITX istheSCItransmit pin.
1 RXFUNC Receive function. This bitdefines thefunction ofpinSCIRX.
0 SCIRX isageneral-purpose digital I/Opin.
1 SCIRX istheSCIreceive pin.
0 Reserved 0 Writes have noeffect.

<!-- Page 1754 -->

SCIControl Registers www.ti.com
1754 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.14 SCIPinI/OControl Register 1(SCIPIO1)
Figure 30-23 andTable 30-22 illustrate thisregister.
Figure 30-23. SCIPinI/OControl Register 1(SCIPIO1) [offset =40h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXDIR RXDIR Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-22. SCIPinI/OControl Register 1(SCIPIO1) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXDIR Transmit pindirection. This bitdetermines thedata direction ontheSCITX pinifitisconfigured with
general-purpose I/Ofunctionality (TXFUNC =0).See Table 30-23 fortheSCITX pincontrol with thisbit
andothers.
0 SCITX isageneral-purpose input pin.
1 SCITX isageneral-purpose output pin.
1 RXDIR Receive pindirection. This bitdetermines thedata direction ontheSCIRX pinifitisconfigured with
general-purpose I/Ofunctionality (RX FUNC =0).See Table 30-24 fortheSCIRX pincontrol with this
bitandothers.
0 SCIRX isageneral-purpose input pin.
1 SCIRX isageneral-purpose output pin.
0 Reserved 0 Writes have noeffect.
(1)TXINisaread-only bit.Itsvalue always reflects thelevel oftheSCITX pin.Table 30-23. SCITX PinControl
Function TXIN(1)TXOUT TXFUNC TXDIR
SCITX X X 1 X
General-purpose input X X 0 0
General-purpose output, high X 1 0 1
General-purpose output, low X 0 0 1
(1)RXINisaread-only bit.Itsvalue always reflects thelevel oftheSCIRX pin.Table 30-24. SCIRX PinControl
Function RXIN(1)RXOUT RXFUNC RXDIR
SCIRX X X 1 X
General-purpose input X X 0 0
General-purpose output, high X 1 0 1
General-purpose output, low X 0 0 1

<!-- Page 1755 -->

www.ti.com SCIControl Registers
1755 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.15 SCIPinI/OControl Register 2(SCIPIO2)
Figure 30-24 andTable 30-25 illustrate thisregister.
Figure 30-24. SCIPinI/OControl Register 2(SCIPIO2) [offset =44h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXIN RXIN Reserved
R-0 R-X R-X R-X
LEGEND: R=Read only; -n=value after reset; -X=value isindeterminate
Table 30-25. SCIPinI/OControl Register 2(SCIPIO2) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXIN Transmit pinin.This bitcontains thecurrent value ontheSCITX pin.
0 The SCITX pinisatlogic low(0).
1 The SCITX pinisatlogic high (1).
1 RXIN Receive pinin.This bitcontains thecurrent value ontheSCIRX pin.
0 The SCIRX pinisatlogic low(0).
1 The SCIRX pinisatlogic high (1).
0 Reserved 0 Writes have noeffect.

<!-- Page 1756 -->

SCIControl Registers www.ti.com
1756 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.16 SCIPinI/OControl Register 3(SCIPIO3)
Figure 30-25 andTable 30-26 illustrate thisregister.
Figure 30-25. SCIPinI/OControl Register 3(SCIPIO3) [offset =48h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXOUT RXOUT Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-26. SCIPinI/OControl Register 3(SCIPIO3) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXOUT Transmit pinout.This pinspecifies thelogic tobeoutput onpinSCITX, ifthefollowing conditions are
met:
*TXFUNC =0(SCITX pinisageneral-purpose I/O.)
*TXDIR =1(SCITX pinisageneral-purpose output.)
See Table 30-23 foranexplanation ofthisbit'seffect incombination with other bits.
0 The output ontheSCITX isatlogic low(0).
1 The output ontheSCITX pinisatlogic high (1).(Output voltage isVOHorhigher ifTXPDR =0and
output isinhigh impedance state ifTXPDR =1.)
1 RXOUT Receive pinout.This bitspecifies thelogic tobeoutput onpinSCIRX, ifthefollowing conditions are
met:
*RXFUNC =0(SCIRX pinisageneral-purpose I/O.)
*RXDIR =1(SCIRX pinisageneral-purpose output.)
See Table 30-24 foranexplanation ofthisbit'seffect incombination with theother bits.
0 The output ontheSCIRX pinisatlogic low(0).
1 The output ontheSCIRX pinisatlogic high (1).(Output voltage isVOHorhigher ifRXPDR =0,and
output isinhigh impedance state ifRXPDR =1.)
0 Reserved 0 Writes have noeffect.

<!-- Page 1757 -->

www.ti.com SCIControl Registers
1757 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.17 SCIPinI/OControl Register 4(SCIPIO4)
Figure 30-26 andTable 30-27 illustrate thisregister.
Figure 30-26. SCIPinI/OControl Register 4(SCIPIO4) [offset =4Ch]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXSET RXSET Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-27. SCIPinI/OControl Register 4(SCIPIO4) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXSET Transmit pinset.This bitsets thelogic tobeoutput onpinSCITX, ifthefollowing conditions aremet:
*TXFUNC =0(SCITX pinisageneral-purpose I/O.)
*TXDIR =1(SCITX pinisageneral-purpose output.)
See Table 30-23 foranexplanation ofthisbit'seffect incombination with other bits.
0 Read: The output onSCITX isatlogic low(0).
Write: Noeffect.
1 Read orwrite: The output onSCITX isatlogic high (1).
1 RXSET Receive pinset.This bitsets thedata tobeoutput onpinSCIRX, ifthefollowing conditions aremet:
*RXFUNC =0(SCIRX pinisageneral-purpose I/O.)
*RXDIR =1(SCIRX pinisageneral-purpose output.)
See Table 30-24 foranexplanation ofthisbit'seffect incombination with theother bits.
0 Read: The output onSCIRX isatlogic low(0).
Write: Noeffect.
1 Read orwrite: The output onSCIRX isatlogic high (1).
0 Reserved 0 Writes have noeffect.

<!-- Page 1758 -->

SCIControl Registers www.ti.com
1758 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.18 SCIPinI/OControl Register 5(SCIPIO5)
Figure 30-27 andTable 30-28 illustrate thisregister.
Figure 30-27. SCIPinI/OControl Register 5(SCIPIO5) [offset =50h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXCLR RXCLR Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-28. SCIPinI/OControl Register 5(SCIPIO5) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXCLR Transmit pinclear. This bitclears thelogic tobeoutput onpinSCITX, ifthefollowing conditions are
met:
*TXFUNC =0(SCITX pinisageneral-purpose I/O.)
*TXDIR =1(SCITX pinisageneral-purpose output.)
0 Read: The output onSCITX isatlogic low(0).
Write: Noeffect.
1 Read: The output onSCITX isatlogic high (1).
Write: The output onSCITX isatlogic low(0).
1 RXCLR Receive pinclear. This bitclears thelogic tobeoutput onpinSCIRX, ifthefollowing conditions aremet:
*RXFUNC =0(SCIRX pinisageneral-purpose I/O.)
*RXDIR =1(SCIRX pinisageneral-purpose output.)
0 Read: The output onSCIRX isatlogic low(0).
Write: Noeffect.
1 Read: The output onSCIRX isatlogic high (1).
Write: The output onSCIRX isatlogic low(0).
0 Reserved 0 Writes have noeffect.

<!-- Page 1759 -->

www.ti.com SCIControl Registers
1759 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.19 SCIPinI/OControl Register 6(SCIPIO6)
Figure 30-28 andTable 30-29 illustrate thisregister.
Figure 30-28. SCIPinI/OControl Register 6(SCIPIO6) [offset =54h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXPDR RXPDR Reserved
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 30-29. SCIPinI/OControl Register 6(SCIPIO6) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPDR Transmit pinopen drain enable. This bitenables open-drain capability intheoutput pinSCITX, ifthe
following conditions aremet:
*TXFUNC =0(SCITX pinisageneral-purpose I/O.)
*TXDIR =1(SCITX pinisageneral-purpose output.)
0 Open drain functionality isdisabled; theoutput voltage isVOLorlower ifTXOUT =0andVOHorhigher if
TXOUT =1.
1 Open drain functionality isenabled; theoutput voltage isVOLorlower ifTXOUT =0andhigh impedance
ifTXOUT =1.
1 RXPDR Receive pinopen drain enable. This bitenables open-drain capability intheoutput pinSCIRX, ifthe
following conditions aremet:
*RXFUNC =0(SCIRX pinisageneral-purpose I/O.)
*RXDIR =1(SCIRX pinisageneral-purpose output.)
0 Open drain functionality isdisabled; theoutput voltage isVOLorlower ifRXOUT =0andVOHorhigher if
RXOUT =1.
1 Open drain functionality isenabled; theoutput voltage isVOLorlower ifRXOUT =0andhigh
impedance ifRXOUT =1.
0 Reserved 0 Writes have noeffect.

<!-- Page 1760 -->

SCIControl Registers www.ti.com
1760 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.20 SCIPinI/OControl Register 7(SCIPIO7)
Figure 30-29 andTable 30-30 illustrate thisregister.
Figure 30-29. SCIPinI/OControl Register 7(SCIPIO7) [offset =58h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXPD RXPD Reserved
R-0 R/W-n R/W-n R/W-n
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset, Refer totheTerminal Functions inthedevice datasheet fordefault pin
settings.
Table 30-30. SCIPinI/OControl Register 7(SCIPIO7) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPD Transmit pinpullcontrol disable. This bitdisables pullcontrol capability ontheinput pinSCITX.
0 Pullcontrol ontheSCITX pinisenabled.
1 Pullcontrol ontheSCITX pinisdisabled.
1 RXPD Receive pinpullcontrol disable. This bitdisables pullcontrol capability ontheinput pinSCIRX.
0 Pullcontrol ontheSCIRX pinisenabled.
1 Pullcontrol ontheSCIRX pinisdisabled.
0 Reserved 0 Writes have noeffect.
30.7.21 SCIPinI/OControl Register 8(SCIPIO8)
Figure 30-30 andTable 30-31 illustrate thisregister.
Figure 30-30. SCIPinI/OControl Register 8(SCIPIO8) [offset =5Ch]
31 8
Reserved
R-0
7 3 2 1 0
Reserved TXPSL RXPSL Reserved
R-0 R/W-n R/W-n R/W-n
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset, Refer totheTerminal Functions inthedevice datasheet fordefault pin
settings.
Table 30-31. SCIPinI/OControl Register 8(SCIPIO8) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2 TXPSL TXpinpullselect. This bitselects pulltype intheinput pinSCITX.
0 The SCITX pinisapulldown.
1 The SCITX pinisapullup.
1 RXPSL RXpinpullselect. This bitselects pulltype intheinput pinSCIRX.
0 The SCIRX pinisapulldown.
1 The SCIRX pinisapullup.
0 Reserved 0 Writes have noeffect.

<!-- Page 1761 -->

www.ti.com SCIControl Registers
1761 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.7.22 Input/Output Error Enable (IODFTCTRL) Register
Figure 30-31 andTable 30-32 illustrate thisregister. After thebasic SCImodule configuration, enable the
required Error mode tobecreated followed byIODFT Key enable.
NOTE:
1.Allthebitsareused inIODFT mode only.
2.Each IODFT areexpected tobechecked individually.
Figure 30-31. Input/Output Error Enable Register (IODFTCTRL) [offset =90h]
31 27 26 25 24
Reserved FEN PEN BRKDTENA
R-0 R/W-0 R/W-0 R/W-0
23 21 20 19 18 16
Reserved PINSAMPLE MASK TXSHIFT
R-0 R/W-0 R/W-0
15 12 11 8
Reserved IODFTENA
R-0 R/WP-0 R/WP-1 R/WP-0 R/WP-1
7 2 1 0
Reserved LPB ENA RXPENA
R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 30-32. Input/Output Error Enable Register (IODFTCTRL) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 FEN Frame error enable. This bitisused tocreate aframe error.
0 Noerror iscreated.
1 The stop bitreceived isANDed with 0andpassed tothestop bitcheck circuitry.
25 PEN Parity error enable. This bitisused tocreate aparity error.
0 Noparity error occurs.
1 The parity bitreceived istoggled sothataparity error occurs.
24 BRKD TENA Break detect error enable. This bitisused tocreate aBRKDT error.
0 Noerror iscreated.
1 The stop bitoftheframe isANDed with 0andpassed totheRSM sothataframe error
occurs. Then theRXpinisforced tocontinuous lowfor10TBITSsothataBRKDT error
occurs.
32-21 Reserved 0 Reads return 0.Writes have noeffect.
20-19 PINSAMPLE MASK Pinsample mask. These bitsdefine thesample number atwhich theTXpinvalue thatis
being transmitted willbeinverted toverify thereceive pinsamples majority detection
circuitry.
0 Nomask isused.
1h Invert theTXPinvalue at7thSCLK.
2h Invert theTXPinvalue at8thSCLK.
3h Invert theTXPinvalue at9thSCLK.

<!-- Page 1762 -->

SCIControl Registers www.ti.com
1762 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) ModuleTable 30-32. Input/Output Error Enable Register (IODFTCTRL) Field Descriptions (continued)
Bit Field Value Description
18-16 TXSHIFT Transmit shift. These bitsdefine theamount bywhich thevalue onTXpinisdelayed sothat
thevalue ontheRXpinisasynchronous. This feature isnotapplicable tothestart bit.
0 Nodelay occurs.
1h The value isdelayed by1SCLK.
2h The value isdelayed by2SCLK.
3h The value isdelayed by3SCLK.
4h The value isdelayed by4SCLK.
5h The value isdelayed by5SCLK.
6h The value isdelayed by6SCLK.
7h Nodelay occurs.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 IODFTENA IODFT enable key. Write access permitted inPrivilege mode only.
Ah IODFT isenabled.
AllOthers IODFT isdisabled.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 LPBENA Module loopback enable. Write access permitted inPrivilege mode only.
Note: Inanalog loopback mode thecomplete communication path through theI/Os
canbetested, whereas indigital loopback mode theI/Obuffers areexcluded from
thispath.
0 Digital loopback isenabled.
1 Analog loopback isenabled inmodule I/ODFT mode when IODFTENA =1010.
0 RXPENA Module analog loopback through receive pinenable. Write access permitted inPrivilege
mode only.
This bitdefines whether theI/Obuffers forthetransmit orthereceive pinareincluded inthe
communication path (inanalog loopback mode).
0 Analog loopback through thetransmit pinisenabled.
1 Analog loopback through thereceive pinisenabled.

<!-- Page 1763 -->

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
1763 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.8 GPIO Functionality
The following sections apply toalldevice pins thatcanbeconfigured asfunctional orgeneral-purpose I/O
pins.
30.8.1 GPIO Functionality
Figure 30-32 illustrates theGPIO functionality.
Figure 30-32. GPIO Functionality
30.8.2 Under Reset
The following apply ifadevice isunder reset:
*Pullcontrol. The reset pullcontrol onthepins isenabled.
*Input buffer. The input buffer isenabled.
*Output buffer. The output buffer isdisabled.

<!-- Page 1764 -->

GPIO Functionality www.ti.com
1764 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSerial Communication Interface (SCI) Module30.8.3 OutofReset
The following apply ifthedevice isoutofreset:
*Pullcontrol. The pullcontrol isenabled byclearing thePD(pull control disable) bitintheSCIPIO7
register (Section 30.7.20 ).Inthiscase, ifthePSL (pull select) bitintheSCIPIO8 register
(Section 30.7.21 )isset,thepinwillhave apull-up. IfthePSL bitiscleared, thepinwillhave apull-
down. IfthePDbitissetinthecontrol register, there isnopull-up orpull-down onthepin.
*Input buffer. The input buffer isalways enabled infunctional mode.
NOTE: The pull-disable logic depends onthepindirection. Itisindependent ofwhether thedevice is
inI/Oorfunctional mode. Ifthepinisconfigured asoutput ortransmit, then thepulls are
disabled automatically. Ifthepinisconfigured asinput orreceive, thepulls areenabled or
disabled depending onbitPDinthepulldisable register SCIPIO7 (Section 30.7.20 ).
*Output buffer. Apincanbedriven asanoutput piniftheTXDIR bitissetinthepindirection control
register (SCIPIO1; Section 30.7.14 )AND theopen-drain feature isnotenabled intheSCIPIO6 register
(Section 30.7.19 ).
30.8.4 Open-Drain Feature Enabled onaPin
The following apply iftheopen-drain feature isenabled onapin:
*The output buffer isenabled ifalowsignal isbeing driven ontothepin.
*The output buffer isdisabled (the direction control signal DIR isinternally forced low) ifahigh signal is
being driven ontothepin.
NOTE: The open-drain feature isavailable only inI/Omode (SCIPIO0; Section 30.7.13 ).
30.8.5 Summary
The behavior oftheinput buffer, output buffer, andthepullcontrol issummarized inTable 30-33 .
(1)X=Don'tcare
(2)DIR =0forinput, =1foroutput
(3)PULDIS =0forenabling pullcontrol
=1fordisabling pullcontrol
(4)PULSEL= 0forpull-down functionality
=1forpull-up functionalityTable 30-33. Input Buffer, Output Buffer, andPullControl Behavior asGPIO Pins
Device
under
Reset?PinDirection
(DIR)(1)(2)PullDisable
(PULDIS)(1)(3)PullSelect
(PULSEL)(1)(4)PullControl Output Buffer Input Buffer
Yes X X X Enabled Disabled Enabled
No 0 0 0 Pulldown Disabled Enabled
No 0 0 1 Pullup Disabled Enabled
No 0 1 0 Disabled Disabled Enabled
No 0 1 1 Disabled Disabled Enabled
No 1 X X Disabled Enabled Enabled