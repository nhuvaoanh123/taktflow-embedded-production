# Enhanced Quadrature Encoder Pulse (eQEP) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1957-1994

---


<!-- Page 1957 -->

1957 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleChapter 34
SPNU563A -March 2018
Enhanced Quadrature Encoder Pulse (eQEP) Module
The enhanced quadrature encoder pulse (eQEP) module isused fordirect interface with alinear orrotary
incremental encoder togetposition, direction, and speed information from arotating machine foruseina
high-performance motion and position-control system. This microcontroller implements 2instances ofthe
eQEP module.
Topic ........................................................................................................................... Page
34.1 Introduction ................................................................................................... 1958
34.2 Basic Operation .............................................................................................. 1960
34.3 eQEP Registers .............................................................................................. 1978

<!-- Page 1958 -->

T0
0 1 2 3 4 5 6 7 N−6N−5N−4N−3N−2N−1 0
QEPA
QEPB
QEPIClockwise shaft rotation/forward movement
Anti-clockwise shaft rotation/reverse movement
0 N−1N−2N−3N−4N−5N−6N−7 6 5 4 3 2 1 0 N−1N−2
QEPA
QEPB
QEPIT0
Legend: N = lines per revolution
QEPA
QEPB
QEPI
Introduction www.ti.com
1958 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.1 Introduction
Asingle track ofslots patterns theperiphery ofanincremental encoder disk, asshown inFigure 34-1 .
These slots create analternating pattern ofdark andlight lines. The disk count isdefined asthenumber
ofdark/light linepairs thatoccur perrevolution (lines perrevolution). Asarule, asecond track isadded to
generate asignal thatoccurs once perrevolution (index signal: QEPI), which canbeused toindicate an
absolute position. Encoder manufacturers identify theindex pulse using different terms such asindex,
marker, home position, andzero reference.
Figure 34-1. Optical Encoder Disk
Toderive direction information, thelines onthedisk areread outbytwodifferent photo-elements that
"look" atthedisk pattern with amechanical shift of1/4thepitch ofalinepairbetween them. This shift is
realized with areticle ormask thatrestricts theview ofthephoto-element tothedesired part ofthedisk
lines. Asthedisk rotates, thetwophoto-elements generate signals thatareshifted 90°outofphase from
each other. These arecommonly called thequadrature QEPA andQEPB signals. The clockwise direction
formost encoders isdefined astheQEPA channel going positive before theQEPB channel andvise
versa asshown inFigure 34-2.
Figure 34-2. QEP Encoder Output Signal forForward/Reverse Movement
The encoder wheel typically makes onerevolution forevery revolution ofthemotor orthewheel may beat
ageared rotation ratio with respect tothemotor. Therefore, thefrequency ofthedigital signal coming from
theQEPA andQEPB outputs varies proportionally with thevelocity ofthemotor. Forexample, a2000-line
encoder directly coupled toamotor running at5000 revolutions perminute (rpm) results inafrequency of
166.6 KHz, sobymeasuring thefrequency ofeither theQEPA orQEPB output, theprocessor can
determine thevelocity ofthemotor.

<!-- Page 1959 -->

v(k)/C0091X
t(k)/C0042t(k/C00421)/C0043X
/C0068T
v(k)/C0091x(k)/C0042x(k/C00421)
T/C0043/C0068X
T
T0
0.25T0 ±0.1T0
0.5T0 ±0.1T0
T0 ±0.5T0QEPA
QEPB
QEPI
(gated to
A and B)
QEPI
(gated to A)
QEPI
(ungated)
www.ti.com Introduction
1959 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleQuadrature encoders from different manufacturers come with twoforms ofindex pulse (gated index pulse
orungated index pulse) asshown inFigure 34-3.Anonstandard form ofindex pulse isungated. Inthe
ungated configuration, theindex edges arenotnecessarily coincident with AandBsignals. The gated
index pulse isaligned toanyofthefour quadrature edges andwidth oftheindex pulse andcanbeequal
toaquarter, half, orfullperiod ofthequadrature signal.
Figure 34-3. Index Pulse Example
Some typical applications ofshaft encoders include robotics andeven computer input intheform ofa
mouse. Inside your mouse youcanseewhere themouse ballspins apairofaxles (aleft/right, andan
up/down axle). These axles areconnected tooptical shaft encoders thateffectively tellthecomputer how
fastandinwhat direction themouse ismoving.
General Issues: Estimating velocity from adigital position sensor isacost-effective strategy inmotor
control. Two different firstorder approximations forvelocity may bewritten as:
(68)
(69)
where
v(k): Velocity attime instant k
x(k): Position attime instant k
x(k-1): Position attime instant k-1
T:Fixed unittime orinverse ofvelocity calculation rate
ΔX:Incremental position movement inunittime
t(k): Time instant "k"
t(k-1): Time instant "k-1"
X:Fixed unitposition
ΔT:Incremental time elapsed forunitposition movement.
Equation 68istheconventional approach tovelocity estimation anditrequires atime base toprovide unit
time event forvelocity calculation. Unit time isbasically theinverse ofthevelocity calculation rate.

<!-- Page 1960 -->

Basic Operation www.ti.com
1960 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleThe encoder count (position) isread once during each unittime event. The quantity [x(k) -x(k-1)] is
formed bysubtracting theprevious reading from thecurrent reading. Then thevelocity estimate is
computed bymultiplying bytheknown constant 1/T(where Tistheconstant time between unittime
events andisknown inadvance).
Estimation based onEquation 68hasaninherent accuracy limit directly related totheresolution ofthe
position sensor andtheunittime period T.Forexample, consider a500-line perrevolution quadrature
encoder with avelocity calculation rate of400Hz.When used forposition thequadrature encoder gives a
four-fold increase inresolution, inthiscase, 2000 counts perrevolution. The minimum rotation thatcanbe
detected istherefore 0.0005 revolutions, which gives avelocity resolution of12rpm when sampled at400
Hz.While thisresolution may besatisfactory atmoderate orhigh speeds, e.g.1%error at1200 rpm, it
would clearly prove inadequate atlowspeeds. Infact, atspeeds below 12rpm, thespeed estimate would
erroneously bezero much ofthetime.
Atlowspeed, Equation 69provides amore accurate approach. Itrequires aposition sensor thatoutputs a
fixed interval pulse train, such astheaforementioned quadrature encoder. The width ofeach pulse is
defined bymotor speed foragiven sensor resolution. Equation 69canbeused tocalculate motor speed
bymeasuring theelapsed time between successive quadrature pulse edges. However, thismethod
suffers from theopposite limitation, asdoes Equation 68.Acombination ofrelatively large motor speeds
andhigh sensor resolution makes thetime intervalΔTsmall, andthus more greatly influenced bythetimer
resolution. This canintroduce considerable error intohigh-speed estimates.
Forsystems with alarge speed range (that is,speed estimation isneeded atboth lowandhigh speeds),
oneapproach istouseEquation 69atlowspeed andhave theDSP software switch over toEquation 68
when themotor speed rises above some specified threshold.
34.2 Basic Operation
34.2.1 EQEP Inputs
The eQEP inputs include twopins forquadrature-clock mode ordirection-count mode, anindex (or0
marker), andastrobe input.
*QEPA/XCLK andQEPB/XDIR
These twopins canbeused inquadrature-clock mode ordirection-count mode.
-Quadrature-clock Mode
The eQEP encoders provide twosquare wave signals (AandB)90electrical degrees outofphase
whose phase relationship isused todetermine thedirection ofrotation oftheinput shaft and
number ofeQEP pulses from theindex position toderive therelative position information. For
forward orclockwise rotation, QEPA signal leads QEPB signal andvice versa. The quadrature
decoder uses these twoinputs togenerate quadrature-clock anddirection signals.
-Direction-count Mode
Indirection-count mode, direction andclock signals areprovided directly from theexternal source.
Some position encoders have thistype ofoutput instead ofquadrature output. The QEPA pin
provides theclock input andtheQEPB pinprovides thedirection input.
*eQEPI: Index orZero Marker
The eQEP encoder uses anindex signal toassign anabsolute start position from which position
information isincrementally encoded using quadrature pulses. This pinisconnected totheindex
output oftheeQEP encoder tooptionally reset theposition counter foreach revolution. This signal can
beused toinitialize orlatch theposition counter ontheoccurrence ofadesired event ontheindex pin.
*QEPS: Strobe Input
This general-purpose strobe signal caninitialize orlatch theposition counter ontheoccurrence ofa
desired event onthestrobe pin.This signal istypically connected toasensor orlimit switch tonotify
thatthemotor hasreached adefined position.

<!-- Page 1961 -->

QWDTMR
QWDPRD
16
QWDOG UTIMEQUPRDQUTMR
32
UTOUT
WDTOUTQuadrature
capture unit
(QCAP)
QCPRDLATQCTMRLAT16
QFLGQEPSTSQEPCTLRegisters
used by
multiple units
QCLK
QDIR
QI
QS
PHE
PCSOUTQuadrature
decoder
(QDU)QDECCTL
16
Position counter/
control unit
(PCCU) QPOSLAT
QPOSSLAT32
QPOSILATEQEPxAIN
EQEPxBIN
EQEPxIIN
EQEPxIOUT
EQEPxIOE
EQEPxSIN
EQEPxSOUT
EQEPxSOEGPIO
MUXEQEPxA/XCLK
EQEPxB/XDIR
EQEPxSEQEPxI
QPOSCMP QEINT
QFRC32
QCLR
QPOSCTL1632
QPOSCNT
QPOSMAXQPOSINITVIMEQEPxINTEnhanced QEP (eQEP) peripheralIOMM
control registers
QCTMRQCPRD
16 16QCAPCTLEQEPxENCLK
VCLK3
Data busTo CPU
www.ti.com Basic Operation
1961 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2 Functional Description
The eQEP peripheral contains thefollowing major functional units (asshown inFigure 34-4 ):
*Programmable input qualification foreach pin(part oftheGPIO MUX)
*Quadrature decoder unit(QDU)
*Position counter andcontrol unitforposition measurement (PCCU)
*Quadrature edge-capture unitforlow-speed measurement (QCAP)
*Unit time base forspeed/frequency measurement (UTIME)
*Watchdog timer fordetecting stalls (QWDOG)
Figure 34-4. Functional Block Diagram oftheeQEP Peripheral

<!-- Page 1962 -->

Basic Operation www.ti.com
1962 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.1 eQEP Memory Map
Table 34-1 lists theregisters with their memory locations, sizes, andreset values.
Table 34-1. EQEP Memory Map
NameAddress
OffsetSize(x16)/
#shadow Reset Register Description
QPOSCNT 0x00 2/0 0x00000000 eQEP Position Counter Register
QPOSINIT 0x04 2/0 0x00000000 eQEP Initialization Position Count Register
QPOSMAX 0x08 2/0 0x00000000 eQEP Maximum Position Count Register
QPOSCMP 0x0C 2/1 0x00000000 eQEP Position-Compare Register
QPOSILAT 0x10 2/0 0x00000000 eQEP Index Position Latch Register
QPOSSLAT 0x14 2/0 0x00000000 eQEP Strobe Position Latch Register
QPOSLAT 0x18 2/0 0x00000000 eQEP Position Latch Register
QUTMR 0x1C 2/0 0x00000000 eQEP Unit Timer Register
QUPRD 0x20 2/0 0x00000000 eQEP Unit Period Register
QWDPRD 0x24 1/0 0x0000 eQEP Watchdog Period Register
QWDTMR 0x26 1/0 0x0000 eQEP Watchdog Timer Register
QEPCTL 0x28 1/0 0x0000 eQEP Control Register
QDECCTL 0x2A 1/0 0x0000 eQEP Decoder Control Register
QPOSCTL 0x2C 1/0 0x00000 eQEP Position-Compare Control Register
QCAPCTL 0x2E 1/0 0x0000 eQEP Capture Control Register
QFLG 0x30 1/0 0x0000 eQEP Interrupt Flag Register
QEINT 0x32 1/0 0x0000 eQEP Interrupt Enable Register
QFRC 0x34 1/0 0x0000 eQEP Interrupt Force Register
QCLR 0x36 1/0 0x0000 eQEP Interrupt Clear Register
QCTMR 0x38 1/0 0x0000 eQEP Capture Timer Register
QEPSTS 0x3A 1/0 0x0000 eQEP Status Register
QCTMRLAT 0x3C 1/0 0x0000 eQEP Capture Timer Latch Register
QCPRD 0x3E 1/0 0x0000 eQEP Capture Period Register
Reserved 0x40 - - Reserved
QCPRDLAT 0x42 1/0 0x0000 eQEP Capture Period Latch Register

<!-- Page 1963 -->

0
1
10QA
QB1
100
/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0081/C0066/C0080Quadrature
decoder00
01
10
11iCLK
xCLK
xCLK
xCLK
01
111000iDIR
xDIR
1
0
/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0081/C0083/C0082/C00672/C0081/C0070/C0076/C0071/C0058/C0080/C0072/C0069
PHE/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0081/C0065/C0080
x1
x2
x1, x2
/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0088/C0067/C0082
0
110/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0081/C0073/C0080
QDECCTL:IGATEQCLK
QDIR
QI
10
/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0081/C0083/C0080
0
1QDECCTL:SPSEL
QDECCTL:SPSEL
10
QDECCTL:SOENQS
/C0080/C0067/C0083/C0079/C0085/C0084
EQEPxIOE
EQEPxSOE/C0069/C0081/C0069/C0080/C0120/C0073/C0079/C0085/C0084
/C0069/C0081/C0069/C0080/C0120/C0083/C0079/C0085/C0084EQEPxSINEQEPxIINEQEPxBINEQEPxAIN/C0081/C0068/C0069/C0067/C0067/C0084/C0076/C0058/C0083/C0087/C0065/C0080 /C0081/C0069/C0080/C0083/C0084/C0083/C0058/C0081/C0068/C0070
EQEPA
EQEPB
www.ti.com Basic Operation
1963 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.2 Quadrature Decoder Unit (QDU)
Figure 34-5 shows afunctional block diagram oftheQDU.
Figure 34-5. Functional Block Diagram ofDecoder Unit
34.2.2.2.1 Position Counter Input Modes
Clock anddirection input toposition counter isselected using QDECCTL[QSRC] bits, based oninterface
input requirement asfollows:
*Quadrature-count mode
*Direction-count mode
*UP-count mode
*DOWN-count mode

<!-- Page 1964 -->

(00)
(10)(11)
(01)(A,B)=
QEPA
QEPB
eQEP signals10
0100 11Increment
counter
Decrement
counter
Decrement
counter
Increment
counterDecrement
counter
Decrement
counterIncrement
counter
Increment
counter
Basic Operation www.ti.com
1964 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.2.1.1 Quadrature Count Mode
The quadrature decoder generates thedirection andclock totheposition counter inquadrature count
mode.
Direction Decoding --The direction decoding logic oftheeQEP circuit determines which oneofthe
sequences (QEPA, QEPB) istheleading sequence andaccordingly updates thedirection
information inQEPSTS[QDF] bit.Table 34-2 andFigure 34-6 show thedirection decoding logic in
truth table andstate machine form. Both edges oftheQEPA andQEPB signals aresensed to
generate count pulses fortheposition counter. Therefore, thefrequency oftheclock generated by
theeQEP logic isfour times thatofeach input sequence. Figure 34-7 shows thedirection decoding
andclock generation from theeQEP input signals.
Table 34-2. Quadrature Decoder Truth Table
Previous Edge Present Edge QDIR QPOSCNT
QA↑ QB↑ UP Increment
QB↓ DOWN Decrement
QA↓ TOGGLE Increment orDecrement
QA↓ QB↓ UP Increment
QB↑ DOWN Decrement
QA↑ TOGGLE Increment orDecrement
QB↑ QA↑ DOWN Increment
QA↓ UP Decrement
QB↓ TOGGLE Increment orDecrement
QB↓ QA↓ DOWN Increment
QA↑ UP Decrement
QB↑ TOGGLE Increment orDecrement
Figure 34-6. Quadrature Decoder State Machine

<!-- Page 1965 -->

+1 +1 +1+1 +1 +1+1 −1 −1 −1−1 −1 −1−1−1 −1 −1 −1 +1 +1+1
−1 −1 −1 −1 −1 −1 −1 +1 +1 +1 +1 +1 +1 +1 +1+1+1 +1 −1 −1−1QA
QB
QCLK
QDIR
QPOSCNT
QA
QB
QCLK
QDIR
QPOSCNT
www.ti.com Basic Operation
1965 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleFigure 34-7. Quadrature-clock andDirection Decoding
Phase Error Flag--Innormal operating conditions, quadrature inputs QEPA andQEPB willbe90
degrees outofphase. The phase error flag(PHE) issetintheQFLG register when edge transition
isdetected simultaneously ontheQEPA andQEPB signals tooptionally generate interrupts. State
transitions marked bydashed lines inFigure 34-6 areinvalid transitions thatgenerate aphase
error.
Count Multiplication --The eQEP position counter provides 4xtimes theresolution ofaninput clock by
generating aquadrature-clock (QCLK) ontherising/falling edges ofboth eQEP input clocks (QEPA
andQEPB) asshown inFigure 34-7.
Reverse Count --Innormal quadrature count operation, QEPA input isfedtotheQAinput ofthe
quadrature decoder andtheQEPB input isfedtotheQBinput ofthequadrature decoder. Reverse
counting isenabled bysetting theSWAP bitintheQDECCTL register. This willswap theinput to
thequadrature decoder thereby reversing thecounting direction.
34.2.2.2.1.2 Direction-count Mode
Some position encoders provide direction andclock outputs, instead ofquadrature outputs. Insuch cases,
direction-count mode canbeused. QEPA input willprovide theclock forposition counter andtheQEPB
input willhave thedirection information. The position counter isincremented onevery rising edge ofa
QEPA input when thedirection input ishigh anddecremented when thedirection input islow.
34.2.2.2.1.3 Up-Count Mode
The counter direction signal ishard-wired forupcount andtheposition counter isused tomeasure the
frequency oftheQEPA input. Setting oftheQDECCTL[XCR] bitenables clock generation totheposition
counter onboth edges oftheQEPA input, thereby increasing themeasurement resolution by2xfactor.

<!-- Page 1966 -->

Basic Operation www.ti.com
1966 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.2.1.4 Down-Count Mode
The counter direction signal ishardwired foradown count andtheposition counter isused tomeasure the
frequency oftheQEPA input. Setting oftheQDECCTL[XCR] bitenables clock generation totheposition
counter onboth edges ofaQEPA input, thereby increasing themeasurement resolution by2xfactor.
34.2.2.2.2 eQEP Input Polarity Selection
Each eQEP input canbeinverted using QDECCTL[8:5] control bits. Asanexample, setting of
QDECCTL[QIP] bitwillinvert theindex input.
34.2.2.2.3 Position-Compare Sync Output
The enhanced eQEP peripheral includes aposition-compare unitthatisused togenerate theposition-
compare sync signal oncompare match between theposition counter register (QPOSCNT) andthe
position-compare register (QPOSCMP). This sync signal canbeoutput using anindex pinorstrobe pinof
theEQEP peripheral.
Setting theQDECCTL[SOEN] bitenables theposition-compare sync output andtheQDECCTL[SPSEL] bit
selects either aneQEP index pinoraneQEP strobe pin.
34.2.2.3 Position Counter andControl Unit (PCCU)
The position counter andcontrol unitprovides twoconfiguration registers (QEPCTL andQPOSCTL) for
setting upposition counter operational modes, position counter initialization/latch modes andposition-
compare logic forsync signal generation.
34.2.2.3.1 Position Counter Operating Modes
Position counter data may becaptured indifferent manners. Insome systems, theposition counter is
accumulated continuously formultiple revolutions andtheposition counter value provides theposition
information with respect totheknown reference. Anexample ofthisisthequadrature encoder mounted on
themotor controlling theprint head intheprinter. Here theposition counter isreset bymoving theprint
head tothehome position andthen position counter provides absolute position information with respect to
home position.
Inother systems, theposition counter isreset onevery revolution using index pulse andposition counter
provides rotor angle with respect toindex pulse position.
Position counter canbeconfigured tooperate infollowing four modes
*Position Counter Reset onIndex Event
*Position Counter Reset onMaximum Position
*Position Counter Reset onthefirstIndex Event
*Position Counter Reset onUnit Time OutEvent (Frequency Measurement)
Inalltheabove operating modes, position counter isreset to0onoverflow andtoQPOSMAX register
value onunderflow. Overflow occurs when theposition counter counts upafter QPOSMAX value.
Underflow occurs when position counter counts down after "0".Interrupt flagissettoindicate
overflow/underflow inQFLG register.

<!-- Page 1967 -->

F9D
F9E0F9F
321 4 3 1 2F9D
F9EF9F
0F9B
F9C F9AF97
F98F99QA
QB
QCLK
QEPSTS:QDF
QPOSCNT F9C 4 5
F9F 0QI
Index interrupt/ 
index event 
marker
QPOSILAT
QEPSTS:QDLF
www.ti.com Basic Operation
1967 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.3.1.1 Position Counter Reset onIndex Event (QEPCTL[PCRM] =00)
Iftheindex event occurs during theforward movement, then position counter isreset to0onthenext
eQEP clock. Iftheindex event occurs during thereverse movement, then theposition counter isreset to
thevalue intheQPOSMAX register onthenext eQEP clock.
First index marker isdefined asthequadrature edge following thefirstindex edge. The eQEP peripheral
records theoccurrence ofthefirstindex marker (QEPSTS[FIMF]) anddirection onthefirstindex event
marker (QEPSTS[FIDF]) inQEPSTS registers, italso remembers thequadrature edge onthefirstindex
marker sothatsame relative quadrature transition isused forindex event reset operation.
Forexample, ifthefirstreset operation occurs onthefalling edge ofQEPB during theforward direction,
then allthesubsequent reset must bealigned with thefalling edge ofQEPB fortheforward rotation and
ontherising edge ofQEPB forthereverse rotation asshown inFigure 34-8.
The position-counter value islatched totheQPOSILAT register anddirection information isrecorded in
theQEPSTS[QDLF] bitonevery index event marker. The position-counter error flag(QEPSTS[PCEF])
anderror interrupt flag(QFLG[PCE]) aresetifthelatched value isnotequal to0orQPOSMAX. The
position-counter error flag(QEPSTS[PCEF]) isupdated onevery index event marker andaninterrupt flag
(QFLG[PCE]) willbesetonerror thatcanbecleared only through software.
The index event latch configuration QEPCTL[IEL] bitsareignored inthismode andposition counter error
flag/interrupt flagaregenerated only inindex event reset mode.
Figure 34-8. Position Counter Reset byIndex Pulse for1000 Line Encoder (QPOSMAX =3999 orF9Fh)

<!-- Page 1968 -->

QA
QB
QCLK
QDIR
QPOSCNT
OV/UF
QA
QB
QCLK
QDIR
QPOSCNT
OV/UF1 2 3 4 0 1 2 1 0 4 3 2 1 0 4 3 2 1 2 3 4 0
1 0 4 3 2 1 0 1 2 3 4 0 1 2 3 4 0 1 0 4 3
Basic Operation www.ti.com
1968 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.3.1.2 Position Counter Reset onMaximum Position (QEPCTL[PCRM] =01)
Iftheposition counter isequal toQPOSMAX, then theposition counter isreset to0onthenext eQEP
clock forforward movement andposition counter overflow flagisset.Iftheposition counter isequal to
ZERO, then theposition counter isreset toQPOSMAX onthenext QEP clock forreverse movement and
position counter underflow flagisset.Figure 34-9 shows theposition counter reset operation inthismode.
First index marker isdefined asthequadrature edge following thefirstindex edge. The eQEP peripheral
records theoccurrence ofthefirstindex marker (QEPSTS[FIMF]) anddirection onthefirstindex event
marker (QEPSTS[FIDF]) intheQEPSTS registers; italso remembers thequadrature edge onthefirst
index marker sothatthesame relative quadrature transition isused forthesoftware index marker
(QEPCTL[IEL]=11).
Figure 34-9. Position Counter Underflow/Overflow (QPOSMAX =4)

<!-- Page 1969 -->

www.ti.com Basic Operation
1969 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.3.1.3 Position Counter Reset ontheFirst Index Event (QEPCTL[PCRM] =10)
Iftheindex event occurs during forward movement, then theposition counter isreset to0onthenext
eQEP clock. Iftheindex event occurs during thereverse movement, then theposition counter isreset to
thevalue intheQPOSMAX register onthenext eQEP clock. Note thatthisisdone only onthefirst
occurrence andsubsequently theposition counter value isnotreset onanindex event; rather, itisreset
based onmaximum position asdescribed inSection 34.2.2.3.1.2 .
First index marker isdefined asthequadrature edge following thefirstindex edge. The eQEP peripheral
records theoccurrence ofthefirstindex marker (QEPSTS[FIMF]) anddirection onthefirstindex event
marker (QEPSTS[FIDF]) inQEPSTS registers, italso remembers thequadrature edge onthefirstindex
marker sothatsame relative quadrature transition isused forsoftware index marker (QEPCTL[IEL]=11).
34.2.2.3.1.4 Position Counter Reset onUnit Time outEvent (QEPCTL[PCRM] =11)
Inthismode, theQPOSCNT value islatched totheQPOSLAT register andthen theQPOSCNT isreset
(to0orQPOSMAX, depending onthedirection mode selected byQDECCTL[QSRC] bitsonaunittime
event). This isuseful forfrequency measurement.
34.2.2.3.2 Position Counter Latch
The eQEP index andstrobe input canbeconfigured tolatch theposition counter (QPOSCNT) into
QPOSILAT andQPOSSLAT, respectively, onoccurrence ofadefinite event onthese pins.
34.2.2.3.2.1 Index Event Latch
Insome applications, itmay notbedesirable toreset theposition counter onevery index event and
instead itmay berequired tooperate theposition counter infull32-bit mode (QEPCTL[PCRM] =01and
QEPCTL[PCRM] =10modes).
Insuch cases, theeQEP position counter canbeconfigured tolatch onthefollowing events anddirection
information isrecorded intheQEPSTS[QDLF] bitonevery index event marker.
*Latch onRising edge (QEPCTL[IEL]=01)
*Latch onFalling edge (QEPCTL[IEL]=10)
*Latch onIndex Event Marker (QEPCTL[IEL]=11)
This isparticularly useful asanerror checking mechanism tocheck iftheposition counter accumulated
thecorrect number ofcounts between index events. Asanexample, the1000-line encoder must count
4000 times when moving inthesame direction between theindex events.
The index event latch interrupt flag(QFLG[IEL]) issetwhen theposition counter islatched tothe
QPOSILAT register. The index event latch configuration bits(QEPCTZ[IEL]) areignored when
QEPCTL[PCRM] =00.
Latch onRising Edge (QEPCTL[IEL]=01) --The position counter value (QPOSCNT) islatched tothe
QPOSILAT register onevery rising edge ofanindex input.
Latch onFalling Edge (QEPCTL[IEL] =10)--The position counter value (QPOSCNT) islatched tothe
QPOSILAT register onevery falling edge ofindex input.
Latch onIndex Event Marker/Software Index Marker (QEPCTL[IEL] =11--The firstindex marker is
defined asthequadrature edge following thefirstindex edge. The eQEP peripheral records the
occurrence ofthefirstindex marker (QEPSTS[FIMF]) anddirection onthefirstindex event marker
(QEPSTS[FIDF]) intheQEPSTS registers. Italso remembers thequadrature edge onthefirst
index marker sothatsame relative quadrature transition isused forlatching theposition counter
(QEPCTL[IEL]=11).
Figure 34-10 shows theposition counter latch using anindex event marker.

<!-- Page 1970 -->

F9CF9D
F9EF9F
FA0FA1
FA2FA3
FA4FA5FA4
FA3FA2
FA1FA0
F9FF9E
F9DF9C
F9BF9A
F99F98
F97
F9F F9FQA
QB
QS
QCLK
QEPST:QDF
QPOSCNT
QIPOSSLAT
F9CF9D
F9EF9F
FA0FA1
FA2FA3
FA4FA5FA4
FA3FA2
FA1FA0
F9FF9E
F9DF9C
F9BF9A
F99F98
F97
F9F 0QA
QB
QI
QCLK
QEPSTS:QDF
QPOSCNT
Index interrupt/
index event
marker
QPOSILAT
QEPSTS:QDLF
Basic Operation www.ti.com
1970 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleFigure 34-10. Software Index Marker for1000-line Encoder (QEPCTL[IEL] =1)
34.2.2.3.2.2 Strobe Event Latch
The position-counter value islatched totheQPOSSLAT register ontherising edge ofthestrobe input by
clearing theQEPCTL[SEL] bit.
IftheQEPCTL[SEL] bitisset,then theposition counter value islatched totheQPOSSLAT register onthe
rising edge ofthestrobe input forforward direction andonthefalling edge ofthestrobe input forreverse
direction asshown inFigure 34-11 .
The strobe event latch interrupt flag(QFLG[SEL) issetwhen theposition counter islatched tothe
QPOSSLAT register.
Figure 34-11. Strobe Event Latch (QEPCTL[SEL] =1)

<!-- Page 1971 -->

QPOSCTL:PCSPW
8
Pulse
stretcherQFLG:PCM
QPOSCNT32QPOSCMP QFLG:PCR
32QPOSCTL:PCSHDW
QPOSCTL:PCLOAD
0
1QPOSCTL:PCPOL
PCSOUTPCEVENT
www.ti.com Basic Operation
1971 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.3.3 Position Counter Initialization
The position counter canbeinitialized using following events:
*Index event
*Strobe event
*Software initialization
Index Event Initialization (IEI)--The QEPI index input canbeused totrigger theinitialization ofthe
position counter attherising orfalling edge oftheindex input. IftheQEPCTL[IEI] bitsare10,then
theposition counter (QPOSCNT) isinitialized with avalue intheQPOSINIT register ontherising
edge ofindex input. Conversely, iftheQEPCTL[IEI] bitsare11,initialization willbeonthefalling
edge oftheindex input.
Strobe Event Initialization (SEI) --IftheQEPCTL[SEI] bitsare10,then theposition counter isinitialized
with avalue intheQPOSINIT register ontherising edge ofstrobe input.
IfQEPCTL[SEL] bitsare11,then theposition counter isinitialized with avalue intheQPOSINIT
register ontherising edge ofstrobe input forforward direction andonthefalling edge ofstrobe
input forreverse direction.
Software Initialization (SWI) --The position counter canbeinitialized insoftware bywriting a1tothe
QEPCTL[SWI] bit.This bitisnotautomatically cleared. While thebitisstillset,ifa1iswritten toit
again, theposition counter willbere-initialized.
34.2.2.3.4 eQEP Position-compare Unit
The eQEP peripheral includes aposition-compare unitthatisused togenerate async output and/or
interrupt onaposition-compare match. Figure 34-12 shows adiagram. The position-compare
(QPOSCMP) register isshadowed andshadow mode canbeenabled ordisabled using the
QPOSCTL[PSSHDW] bit.Iftheshadow mode isnotenabled, theCPU writes directly totheactive position
compare register.
Figure 34-12. eQEP Position-compare Unit
Inshadow mode, youcanconfigure theposition-compare unit(QPOSCTL[PCLOAD]) toload theshadow
register value intotheactive register onthefollowing events andtogenerate theposition-compare ready
(QFLG[PCR]) interrupt after loading.
*Load oncompare match
*Load onposition-counter zero event
The position-compare match (QFLG[PCM]) issetwhen theposition-counter value (QPOSCNT) matches
with theactive position-compare register (QPOSCMP) andtheposition-compare sync output ofthe
programmable pulse width isgenerated oncompare match totrigger anexternal device.

<!-- Page 1972 -->

DIR
QPOSCMP
QPOSCNT
PCSOUT (active HIGH)PCSPWPCSPWPCSPWPCEVNT
POSCMP=2
01234
3
2
1
01234
3
2
1
0eQEP counter
PCEVNT
PCSOUT (active HIGH)
PCSOUT (active LOW)PCSPW
Basic Operation www.ti.com
1972 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleForexample, ifQPOSCMP =2,theposition-compare unitgenerates aposition-compare event on1to2
transitions oftheeQEP position counter forforward counting direction andon3to2transitions ofthe
eQEP position counter forreverse counting direction (see Figure 34-13 ).
Section 34.3.14 shows thelayout oftheeQEP Position-Compare Control Register (QPOSCTL) and
describes theQPOSCTL bitfields.
Figure 34-13. eQEP Position-compare Event Generation Points
The pulse stretcher logic intheposition-compare unitgenerates aprogrammable position-compare sync
pulse output ontheposition-compare match. Intheevent ofanew position-compare match while a
previous position-compare pulse isstillactive, then thepulse stretcher generates apulse ofspecified
duration from thenew position-compare event asshown inFigure 34-14 .
Figure 34-14. eQEP Position-compare Sync Output Pulse Stretcher

<!-- Page 1973 -->

v(k)/C0043X
t(k)/C0042t(k/C00421)/C0043X
/C0068T
www.ti.com Basic Operation
1973 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.2.4 eQEP Edge Capture Unit
The eQEP peripheral includes anintegrated edge capture unittomeasure theelapsed time between the
unitposition events asshown inFigure 34-15 .This feature istypically used forlowspeed measurement
using thefollowing equation:
(70)
where,
*X-Unit position isdefined byinteger multiple ofquadrature edges (see Figure 34-16 )
*ΔT-Elapsed time between unitposition events
*v(k)-Velocity attime instant "k"
The eQEP capture timer (QCTMR) runs from prescaled VCLK3 andtheprescaler isprogrammed bythe
QCAPCTL[CCPS] bits. The capture timer (QCTMR) value islatched intothecapture period register
(QCPRD) onevery unitposition event andthen thecapture timer isreset, aflagissetin
QEPSTS:UPEVNT toindicate thatnew value islatched intotheQCPRD register. Software cancheck this
status flagbefore reading theperiod register forlowspeed measurement andclear theflagbywriting 1.
Time measurement (ΔT)between unitposition events willbecorrect ifthefollowing conditions aremet:
*Nomore than 65,535 counts have occurred between unitposition events.
*Nodirection change between unitposition events.
The capture unitsets theeQEP overflow error flag(QEPSTS[COEF]) intheevent ofcapture timer
overflow between unitposition events. Ifadirection change occurs between theunitposition events, then
anerror flagissetinthestatus register (QEPSTS[CDEF]).
Capture Timer (QCTMR) andCapture period register (QCPRD) canbeconfigured tolatch onfollowing
events.
*CPU read ofQPOSCNT register
*Unit time-out event
IftheQEPCTL[QCLM] bitiscleared, then thecapture timer andcapture period values arelatched intothe
QCTMRLAT andQCPRDLAT registers, respectively, when theCPU reads theposition counter
(QPOSCNT).
IftheQEPCTL[QCLM] bitisset,then theposition counter, capture timer, andcapture period values are
latched intotheQPOSLAT, QCTMRLAT andQCPRDLAT registers, respectively, onunittime out.
Figure 34-17 shows thecapture unitoperation along with theposition counter.

<!-- Page 1974 -->

X=N x PP
QA
QB
QCLK
UPEVNT
QCAPCTL:CENCapture timer
control unit
(CTCU)QCPRDQCTMR QCTMRLAT
QCPRDLAT
1616
3-bit binary
divider
x1, 1/2, 1/4...,
1/128CAPCLKQCAPCTL:CCPS
3
VCLK3160xFFFF16
QEPSTS:COEF
4-bit binary
divider
x1, 1/2, 1/4...,
1/2048QCLK
Rising/falling
edge detectQDIRQEPSTS:CDEF
QEPCTL:UTE
QUTMR
QUPRDVCLK3QFLG:UTOUTIME4QCAPCTL:UPPS
UTOUTQEPSTS:UPEVNT
UPEVNT
Basic Operation www.ti.com
1974 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleFigure 34-15. eQEP Edge Capture Unit
NOTE: The QCAPCTL[UPPS] prescaler should notbemodified dynamically (such asswitching the
unitevent prescaler from QCLK/4 toQCLK/8). Doing somay result inundefined behavior.
The QCAPCTL[CPPS] prescaler canbemodified dynamically (such asswitching CAPCLK
prescaling mode from SYSCLK/4 toSYSCLK/8) only after thecapture unitisdisabled.
Figure 34-16. Unit Position Event forLow Speed Measurement (QCAPCTL[UPPS] =0010)
A N-Number ofquadrature periods selected using QCAPCTL[UPPS] bits

<!-- Page 1975 -->

v(k)/C0043x(k)/C0042x(k/C00421)
T/C0043/C0068X
Tor
ΔX
x(k−1)
ΔTt(k)
t(k−1)
TQEPA
QEPB
QCLK
QPOSCNT
UPEVNT
QCTMR
UTOUTx(k)
www.ti.com Basic Operation
1975 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleFigure 34-17. eQEP Edge Capture Unit -Timing Details
Velocity Calculation Equations:
(71)
where
v(k): Velocity attime instant k
x(k): Position attime instant k
x(k-1): Position attime instant k-1
T:Fixed unittime orinverse ofvelocity calculation rate
ΔX:Incremental position movement inunittime
X:Fixed unitposition
ΔT:Incremental time elapsed forunitposition movement
t(k): Time instant "k"
t(k-1): Time instant "k-1"
Unit time (T)andunitperiod(X) areconfigured using theQUPRD andQCAPCTL[UPPS] registers.
Incremental position output andincremental time output isavailable intheQPOSLAT andQCPRDLAT
registers.

<!-- Page 1976 -->

QFLG:UTO QUPRD32QUTMR
32QEPCTL:UTEUTIME
VCLK3
UTOUT
QFLG:WTO QWDPRD16QWDTMR
16QEPCTL:WDEQWDOG
RESETVCLK3/64VCLK3
QCLK
WDTOUT
Basic Operation www.ti.com
1976 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleParameter Relevant Register toConfigure orRead theInformation
T Unit Period Register (QUPRD)
ΔX Incremental Position =QPOSLAT(k) -QPOSLAT(K-1)
X Fixed unitposition defined bysensor resolution andZCAPCTL[UPPS] bits
ΔT Capture Period Latch (QCPRDLAT)
34.2.3 eQEP Watchdog
The eQEP peripheral contains a16-bit watchdog timer thatmonitors thequadrature-clock toindicate
proper operation ofthemotion-control system. The eQEP watchdog timer isclocked from VCLK3/64 and
thequadrate clock event (pulse) resets thewatchdog timer. Ifnoquadrature-clock event isdetected until a
period match (QWDPRD =QWDTMR), then thewatchdog timer willtime outandthewatchdog interrupt
flagwillbeset(QFLG[WTO]). The time-out value isprogrammable through thewatchdog period register
(QWDPRD).
Figure 34-18. eQEP Watchdog Timer
34.2.4 Unit Timer Base
The eQEP peripheral includes a32-bit timer (QUTMR) thatisclocked byVCLK3 togenerate periodic
interrupts forvelocity calculations. The unittime outinterrupt isset(QFLG[UTO]) when theunittimer
(QUTMR) matches theunitperiod register (QUPRD).
The eQEP peripheral canbeconfigured tolatch theposition counter, capture timer, andcapture period
values onaunittime outevent sothatlatched values areused forvelocity calculation asdescribed in
Section 34.2.2.4 .
Figure 34-19. eQEP Unit Time Base

<!-- Page 1977 -->

Clr
SetLatch
QFRC:PCE
PCEQCLR:PCE
QFLG:PCEQEINT:PCE
QCLR:UTO
QFRC:UTOQEINT:UTO
setLatchclr
UTO
QFLG:UTO0
10 Pulse
generator
when
input=1QFLG:INT LatchSet Clr QCLR:INT
EQEPxINT
www.ti.com Basic Operation
1977 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.2.5 eQEP Interrupt Structure
Figure 34-20 shows how theinterrupt mechanism works intheEQEP module.
Eleven interrupt events (PCE, PHE, QDC, WTO, PCU, PCO, PCR, PCM, SEL, IELandUTO) canbe
generated. The interrupt control register (QEINT) isused toenable/disable individual interrupt event
sources. The interrupt flagregister (QFLG) indicates ifanyinterrupt event hasbeen latched andcontains
theglobal interrupt flagbit(INT). Aninterrupt pulse isgenerated only tothePIEifanyoftheinterrupt
events isenabled, theflagbitis1andtheINTflagbitis0.The interrupt service routine willneed toclear
theglobal interrupt flagbitandtheserviced event, viatheinterrupt clear register (QCLR), before anyother
interrupt pulses aregenerated. You canforce aninterrupt event byway oftheinterrupt force register
(QFRC), which isuseful fortestpurposes.
Figure 34-20. EQEP Interrupt Generation

<!-- Page 1978 -->

eQEP Registers www.ti.com
1978 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3 eQEP Registers
Table 34-3 lists theregisters oftheeQEP. The base address forthecontrol registers isFCF7 9900h for
eQEP1 andFCF7 9A00h foreQEP2.
Table 34-3. eQEP Registers
Address Offset Acronym Register Description Section
00h QPOSCNT eQEP Position Counter Register Section 34.3.1
04h QPOSINIT eQEP Position Counter Initialization Register Section 34.3.2
08h QPOSMAX eQEP Maximum Position Count Register Section 34.3.3
0Ch QPOSCMP eQEP Position-Compare Register Section 34.3.4
10h QPOSILAT eQEP Index Position Latch Register Section 34.3.5
14h QPOSSLAT eQEP Strobe Position Latch Register Section 34.3.6
18h QPOSLAT eQEP Position Counter Latch Register Section 34.3.7
1Ch QUTMR eQEP Unit Timer Register Section 34.3.8
20h QUPRD eQEP Unit Period Register Section 34.3.9
24h QWDPRD eQEP Watchdog Period Register Section 34.3.10
26h QWDTMR eQEP Watchdog Timer Register Section 34.3.11
28h QEPCTL eQEP Control Register Section 34.3.12
2Ah QDECCTL eQEP Decoder Control Register Section 34.3.13
2Ch QPOSCTL eQEP Position-Compare Control Register Section 34.3.14
2Eh QCAPCTL eQEP Capture Control Register Section 34.3.15
30h QFLG eQEP Interrupt Flag Register Section 34.3.16
32h QEINT eQEP Interrupt Enable Register Section 34.3.17
34h QFRC eQEP Interrupt Force Register Section 34.3.18
36h QCLR eQEP Interrupt Clear Register Section 34.3.19
38h QCTMR eQEP Capture Timer Register Section 34.3.20
3Ah QEPSTS eQEP Status Register Section 34.3.21
3Ch QCTMRLAT eQEP Capture Timer Latch Register Section 34.3.22
3Eh QCPRD eQEP Capture Period Register Section 34.3.23
42h QCPRDLAT eQEP Capture Period Latch Register Section 34.3.24

<!-- Page 1979 -->

www.ti.com eQEP Registers
1979 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.1 eQEP Position Counter Register (QPOSCNT)
Figure 34-21. eQEP Position Counter Register (QPOSCNT) [offset =00h]
31 0
QPOSCNT
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-4. eQEP Position Counter Register (QPOSCNT) Field Descriptions
Bits Name Description
31-0 QPOSCNT This 32-bit position counter register counts up/down onevery eQEP pulse based ondirection input. This
counter acts asaposition integrator whose count value isproportional toposition from agive reference point.
34.3.2 eQEP Position Counter Initialization Register (QPOSINIT)
Figure 34-22. eQEP Position Counter Initialization Register (QPOSINIT) [offset =04h]
31 0
QPOSINIT
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-5. eQEP Position Counter Initialization Register (QPOSINIT) Field Descriptions
Bits Name Description
31-0 QPOSINIT This register contains theposition value thatisused toinitialize theposition counter based onexternal strobe
orindex event. The position counter canbeinitialized through software.
34.3.3 eQEP Maximum Position Count Register (QPOSMAX)
Figure 34-23. eQEP Maximum Position Count Register (QPOSMAX) [offset =08h]
31 0
QPOSMAX
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-6. eQEP Maximum Position Count Register (QPOSMAX) Field Descriptions
Bits Name Description
31-0 QPOSMAX This register contains themaximum position counter value.

<!-- Page 1980 -->

eQEP Registers www.ti.com
1980 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.4 eQEP Position-Compare Register (QPOSCMP)
Figure 34-24. eQEP Position-Compare Register (QPOSCMP) [offset =0Ch]
31 0
QPOSCMP
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-7. eQEP Position-Compare Register (QPOSCMP) Field Descriptions
Bits Name Description
31-0 QPOSCMP The position-compare value inthisregister iscompared with theposition counter (QPOSCNT) togenerate sync
output and/or interrupt oncompare match.
34.3.5 eQEP Index Position Latch Register (QPOSILAT)
Figure 34-25. eQEP Index Position Latch Register (QPOSILAT) [offset =10h]
31 0
QPOSILAT
R-0
LEGEND: R=Read only; -n=value after reset
Table 34-8. eQEP Index Position Latch Register (QPOSILAT) Field Descriptions
Bits Name Description
31-0 QPOSILAT The position-counter value islatched intothisregister onanindex event asdefined bytheQEPCTL[IEL] bits.
34.3.6 eQEP Strobe Position Latch Register (QPOSSLAT)
Figure 34-26. eQEP Strobe Position Latch Register (QPOSSLAT) [offset =14h]
31 0
QPOSSLAT
R-0
LEGEND: R=Read only; -n=value after reset
Table 34-9. eQEP Strobe Position Latch Register (QPOSSLAT) Field Descriptions
Bits Name Description
31-0 QPOSSLAT The position-counter value islatched intothisregister onstrobe event asdefined bytheQEPCTL[SEL] bits.

<!-- Page 1981 -->

www.ti.com eQEP Registers
1981 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.7 eQEP Position Counter Latch Register (QPOSLAT)
Figure 34-27. eQEP Position Counter Latch Register (QPOSLAT) [offset =18h]
31 0
QPOSLAT
R-0
LEGEND: R=Read only; -n=value after reset
Table 34-10. eQEP Position Counter Latch Register (QPOSLAT) Field Descriptions
Bits Name Description
31-0 QPOSLAT The position-counter value islatched intothisregister onunittime outevent.
34.3.8 eQEP Unit Timer Register (QUTMR)
Figure 34-28. eQEP Unit Timer Register (QUTMR) [offset =1Ch]
31 0
QUTMR
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-11. eQEP Unit Timer Register (QUTMR) Field Descriptions
Bits Name Description
31-0 QUTMR This register acts astime base forunittime event generation. When thistimer value matches with unittime
period value, unittime event isgenerated.
34.3.9 eQEP Unit Period Register (QUPRD)
Figure 34-29. eQEP Unit Period Register (QUPRD) [offset =20h]
31 0
QUPRD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-12. eQEP Unit Period Register (QUPRD) Field Descriptions
Bits Name Description
31-0 QUPRD This register contains theperiod count forunittimer togenerate periodic unittime events tolatch theeQEP
position information atperiodic interval andoptionally togenerate interrupt.

<!-- Page 1982 -->

eQEP Registers www.ti.com
1982 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.10 eQEP Watchdog Period Register (QWDPRD)
Figure 34-30. eQEP Watchdog Period Register (QWDPRD) [offset =24h]
15 0
QWDPRD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-13. eQEP Watchdog Period Register (QWDPRD) Field Description
Bits Name Description
15-0 QWDPRD This register contains thetime-out count fortheeQEP peripheral watchdog timer. When thewatchdog timer
value matches thewatchdog period value, awatchdog timeout interrupt isgenerated.
34.3.11 eQEP Watchdog Timer Register (QWDTMR)
Figure 34-31. eQEP Watchdog Timer Register (QWDTMR) [offset =26h]
15 0
QWDTMR
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-14. eQEP Watchdog Timer Register (QWDTMR) Field Descriptions
Bits Name Description
15-0 QWDTMR This register acts astime base forwatchdog todetect motor stalls. When thistimer value matches with
watchdog period value, watchdog timeout interrupt isgenerated. This register isreset upon edge transition in
quadrature-clock indicating themotion.

<!-- Page 1983 -->

www.ti.com eQEP Registers
1983 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.12 eQEP Control Register (QEPCTL)
Figure 34-32. eQEP Control Register (QEPCTL) [offset =28h]
15 14 13 12 11 10 9 8
FREE SOFT PCRM SEI IEI
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
SWI SEL IEL QPEN QCLM UTE WDE
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-15. eQEP Control Register (QEPCTL) Field Descriptions
Bits Name Value Description
15-14 FREE, SOFT Emulation control bits.
QPOSCNT behavior:
0 Position counter stops immediately onemulation suspend.
1h Position counter continues tocount until therollover.
2h-3h Position counter isunaffected byemulation suspend.
QWDTMR behavior:
0 Watchdog counter stops immediately.
1h Watchdog counter counts until WDperiod match rollover.
2h-3h Watchdog counter isunaffected byemulation suspend.
QUTMR behavior:
0 Unit timer stops immediately.
1h Unit timer counts until period rollover.
2h-3h Unit timer isunaffected byemulation suspend.
QCTMR behavior:
0 Capture timer stops immediately.
1h Capture timer counts until next unitperiod event.
2h-3h Capture timer isunaffected byemulation suspend.
13-12 PCRM Position counter reset mode.
0 Position counter reset onanindex event.
1h Position counter reset onthemaximum position.
2h Position counter reset onthefirstindex event.
3h Position counter reset onaunittime event.
11-10 SEI Strobe event initialization ofposition counter.
0 Does nothing (action isdisabled).
1h Does nothing (action isdisabled).
2h Initializes theposition counter onrising edge oftheQEPS signal.
3h Clockwise Direction: Initializes theposition counter ontherising edge ofQEPS strobe.
Counter Clockwise Direction: Initializes theposition counter onthefalling edge ofQEPS strobe.
9-8 IEI Index event initialization ofposition counter.
0 Donothing (action isdisabled).
1h Donothing (action isdisabled).
2h Initializes theposition counter ontherising edge oftheQEPI signal (QPOSCNT =QPOSINIT).
3h Initializes theposition counter onthefalling edge ofQEPI signal (QPOSCNT =QPOSINIT).
7 SWI Software initialization ofposition counter.
0 Donothing (action isdisabled).
1 Initialize position counter (QPOSCNT=QPOSINIT). This bitisnotcleared automatically.

<!-- Page 1984 -->

eQEP Registers www.ti.com
1984 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) ModuleTable 34-15. eQEP Control Register (QEPCTL) Field Descriptions (continued)
Bits Name Value Description
6 SEL Strobe event latch ofposition counter.
0 The position counter islatched ontherising edge ofQEPS strobe (QPOSSLAT =POSCCNT).
Latching onthefalling edge canbedone byinverting thestrobe input using theQSP bitinthe
QDECCTL register.
1 Clockwise Direction: Position counter islatched onrising edge ofQEPS strobe.
Counter Clockwise Direction: Position counter islatched onfalling edge ofQEPS strobe.
5-4 IEL Index event latch ofposition counter (software index marker).
0 Reserved
1h Latches position counter onrising edge oftheindex signal.
2h Latches position counter onfalling edge oftheindex signal.
3h Software index marker. Latches theposition counter andquadrature direction flagonindex event
marker. The position counter islatched totheQPOSILAT register andthedirection flagislatched in
theQEPSTS[QDLF] bit.This mode isuseful forsoftware index marking.
3 QPEN Quadrature position counter enable/software reset.
0 Reset theeQEP peripheral internal operating flags/read-only registers. Control/configuration
registers arenotdisturbed byasoftware reset.
1 eQEP position counter isenabled.
2 QCLM eQEP capture latch mode.
0 Latch onposition counter read byCPU. Capture timer andcapture period values arelatched into
QCTMRLAT andQCPRDLAT registers when CPU reads theQPOSCNT register.
1 Latch onunittime out.Position counter, capture timer andcapture period values arelatched into
QPOSLAT, QCTMRLAT andQCPRDLAT registers onunittime out.
1 UTE eQEP unittimer enable.
0 eQEP unittimer isdisabled.
1 eQEP unittimer isenabled.
0 WDE eQEP watchdog enable.
0 eQEP watchdog timer isdisabled.
1 eQEP watchdog timer isenabled.

<!-- Page 1985 -->

www.ti.com eQEP Registers
1985 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.13 eQEP Decoder Control Register (QDECCTL)
Figure 34-33. eQEP Decoder Control Register (QDECCTL) [offset =2Ah]
15 14 13 12 11 10 9 8
QSRC SOEN SPSEL XCR SWAP IGATE QAP
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 0
QBP QIP QSP Reserved
R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-16. eQEP Decoder Control Register (QDECCTL) Field Descriptions
Bits Name Value Description
15-14 QSRC Position-counter source selection.
0 Quadrature count mode: (QCLK =iCLK, QDIR =iDIR).
1h Direction-count mode: (QCLK =xCLK, QDIR =xDIR).
2h UPcount mode forfrequency measurement :(QCLK =xCLK, QDIR =1).
3h DOWN count mode forfrequency measurement: (QCLK =xCLK, QDIR =0).
13 SOEN Sync output-enable.
0 Position-compare sync output isdisabled.
1 Position-compare sync output isenabled.
12 SPSEL Sync output pinselection.
0 Index pinisused forsync output.
1 Strobe pinisused forsync output.
11 XCR External clock rate.
0 2xresolution: Count therising/falling edge.
1 1xresolution: Count therising edge only.
10 SWAP Swap quadrature clock inputs. This swaps theinput tothequadrature decoder, reversing the
counting direction.
0 Quadrature-clock inputs arenotswapped.
1 Quadrature-clock inputs areswapped.
9 IGATE Index pulse gating option.
0 Disable gating ofIndex pulse.
1 Gate theindex pinwith strobe.
8 QAP QEPA input polarity.
0 Noeffect.
1 Negates QEPA input.
7 QBP QEPB input polarity.
0 Noeffect.
1 Negates QEPB input.
6 QIP QEPI input polarity.
0 Noeffect.
1 Negates QEPI input.
5 QSP QEPS input polarity.
0 Noeffect.
1 Negates QEPS input.
4-0 Reserved 0 Always read as0.

<!-- Page 1986 -->

eQEP Registers www.ti.com
1986 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.14 eQEP Position-Compare Control Register (QPOSCTL)
Figure 34-34. eQEP Position-Compare Control Register (QPOSCTL) [offset =2Ch]
15 14 13 12 11 8
PCSHDW PCLOAD PCPOL PCE PCSPW
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 0
PCSPW
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-17. eQEP Position-Compare Control Register (QPOSCTL) Field Descriptions
Bit Name Value Description
15 PCSHDW Position-compare shadow enable.
0 Shadow isdisabled, load Immediate.
1 Shadow isenabled.
14 PCLOAD Position-compare shadow load mode.
0 Load onQPOSCNT =0.
1 Load when QPOSCNT =QPOSCMP.
13 PCPOL Polarity ofsync output.
0 Active HIGH pulse output.
1 Active LOW pulse output.
12 PCE Position-compare enable.
0 Position-compare unitisdisabled.
1 Position-compare unitisenabled.
11-0 PCSPW Select-position-compare sync output pulse width.
0 1×4×VCLK3 cycles
1h 2×4×VCLK3 cycles
: :
FFFh 4096 ×4×VCLK3 cycles

<!-- Page 1987 -->

www.ti.com eQEP Registers
1987 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.15 eQEP Capture Control Register (QCAPCTL)
Figure 34-35. eQEP Capture Control Register (QCAPCTL) [offset =2Eh]
15 14 7 6 4 3 0
CEN Reserved CCPS UPPS
R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-18. eQEP Capture Control Register (QCAPCTL) Field Descriptions
Bits Name Value Description
15 CEN Enable eQEP capture.
0 eQEP capture unitisdisabled.
1 eQEP capture unitisenabled.
14-7 Reserved 0 Always read as0.
6-4 CCPS eQEP capture timer clock prescaler.
0 CAPCLK =VCLK3/1
1h CAPCLK =VCLK3/2
2h CAPCLK =VCLK3/4
3h CAPCLK =VCLK3/8
4h CAPCLK =VCLK3/16
5h CAPCLK =VCLK3/32
6h CAPCLK =VCLK3/64
7h CAPCLK =VCLK3/128
3-0 UPPS Unit position event prescaler.
0 UPEVNT =QCLK/1
1h UPEVNT =QCLK/2
2h UPEVNT =QCLK/4
3h UPEVNT =QCLK/8
4h UPEVNT =QCLK/16
5h UPEVNT =QCLK/32
6h UPEVNT =QCLK/64
7h UPEVNT =QCLK/128
8h UPEVNT =QCLK/256
9h UPEVNT =QCLK/512
Ah UPEVNT =QCLK/1024
Bh UPEVNT =QCLK/2048
Ch-Fh Reserved

<!-- Page 1988 -->

eQEP Registers www.ti.com
1988 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.16 eQEP Interrupt Flag Register (QFLG)
Figure 34-36. eQEP Interrupt Flag Register (QFLG) [offset =30h]
15 12 11 10 9 8
Reserved UTO IEL SEL PCM
R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
PCR PCO PCU WTO QDC PHE PCE INT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 34-19. eQEP Interrupt Flag Register (QFLG) Field Descriptions
Bits Name Value Description
15-12 Reserved 0 Always read as0.
11 UTO Unit time outinterrupt flag.
0 Nointerrupt isgenerated.
1 SetbyeQEP unittimer period match.
10 IEL Index event latch interrupt flag.
0 Nointerrupt isgenerated.
1 Setafter latching theQPOSCNT toQPOSILAT.
9 SEL Strobe event latch interrupt flag.
0 Nointerrupt isgenerated.
1 Setafter latching theQPOSCNT toQPOSSLAT.
8 PCM Position-compare match interrupt flag.
0 Nointerrupt isgenerated.
1 Setonposition-compare match.
7 PCR Position-compare ready interrupt flag.
0 Nointerrupt isgenerated.
1 Setafter transferring theshadow register value totheactive position compare register.
6 PCO Position counter overflow interrupt flag.
0 Nointerrupt isgenerated.
1 Setonposition counter overflow.
5 PCU Position counter underflow interrupt flag.
0 Nointerrupt isgenerated.
1 Setonposition counter underflow.
4 WTO Watchdog time outinterrupt flag.
0 Nointerrupt isgenerated.
1 Setbywatchdog timeout.
3 QDC Quadrature direction change interrupt flag.
0 Nointerrupt isgenerated.
1 Setduring change ofdirection.
2 PHE Quadrature phase error interrupt flag.
0 Nointerrupt isgenerated.
1 Setonsimultaneous transition ofQEPA andQEPB.
1 PCE Position counter error interrupt flag.
0 Nointerrupt isgenerated.
1 Position counter error.
0 INT Global interrupt status flag.
0 Nointerrupt isgenerated.
1 Interrupt was generated.

<!-- Page 1989 -->

www.ti.com eQEP Registers
1989 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.17 eQEP Interrupt Enable Register (QEINT)
Figure 34-37. eQEP Interrupt Enable Register (QEINT) [offset =32h]
15 12 11 10 9 8
Reserved UTO IEL SEL PCM
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
PCR PCO PCU WTO QDC QPE PCE Reserved
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-20. eQEP Interrupt Enable Register (QEINT) Field Descriptions
Bits Name Value Description
15-12 Reserved 0 Always read as0.
11 UTO Unit time outinterrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
10 IEL Index event latch interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
9 SEL Strobe event latch interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
8 PCM Position-compare match interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
7 PCR Position-compare ready interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
6 PCO Position counter overflow interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
5 PCU Position counter underflow interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
4 WTO Watchdog time outinterrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
3 QDC Quadrature direction change interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
2 QPE Quadrature phase error interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
1 PCE Position counter error interrupt enable.
0 Interrupt isdisabled.
1 Interrupt isenabled.
0 Reserved 0 Always read as0.

<!-- Page 1990 -->

eQEP Registers www.ti.com
1990 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.18 eQEP Interrupt Force Register (QFRC)
Figure 34-38. eQEP Interrupt Force Register (QFRC) [offset =34h]
15 12 11 10 9 8
Reserved UTO IEL SEL PCM
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
PCR PCO PCU WTO QDC PHE PCE Reserved
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-21. eQEP Interrupt Force Register (QFRC) Field Descriptions
Bit Field Value Description
15-12 Reserved 0 Always read as0.
11 UTO Force unittime outinterrupt.
0 Noeffect.
1 Force theinterrupt.
10 IEL Force index event latch interrupt.
0 Noeffect.
1 Force theinterrupt.
9 SEL Force strobe event latch interrupt.
0 Noeffect.
1 Force theinterrupt.
8 PCM Force position-compare match interrupt.
0 Noeffect.
1 Force theinterrupt.
7 PCR Force position-compare ready interrupt.
0 Noeffect.
1 Force theinterrupt.
6 PCO Force position counter overflow interrupt.
0 Noeffect.
1 Force theinterrupt.
5 PCU Force position counter underflow interrupt.
0 Noeffect.
1 Force theinterrupt.
4 WTO Force watchdog time outinterrupt.
0 Noeffect.
1 Force theinterrupt.
3 QDC Force quadrature direction change interrupt.
0 Noeffect.
1 Force theinterrupt.
2 PHE Force quadrature phase error interrupt.
0 Noeffect.
1 Force theinterrupt.
1 PCE Force position counter error interrupt.
0 Noeffect.
1 Force theinterrupt.
0 Reserved 0 Always read as0.

<!-- Page 1991 -->

www.ti.com eQEP Registers
1991 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.19 eQEP Interrupt Clear Register (QCLR)
Figure 34-39. eQEP Interrupt Clear Register (QCLR) [offset =36h]
15 12 11 10 9 8
Reserved UTO IEL SEL PCM
R-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
PCR PCO PCU WTO QDC PHE PCE INT
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-22. eQEP Interrupt Clear Register (QCLR) Field Descriptions
Bit Field Value Description
15-12 Reserved 0 Always read as0.
11 UTO Clear unittime outinterrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
10 IEL Clear index event latch interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
9 SEL Clear strobe event latch interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
8 PCM Clear eQEP compare match event interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
7 PCR Clear position-compare ready interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
6 PCO Clear position counter overflow interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
5 PCU Clear position counter underflow interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
4 WTO Clear watchdog timeout interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
3 QDC Clear quadrature direction change interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
2 PHE Clear quadrature phase error interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
1 PCE Clear position counter error interrupt flag.
0 Noeffect.
1 Clears theinterrupt flag.
0 INT Global interrupt clear flag.
0 Noeffect.
1 Clears theinterrupt flagandenables further interrupts tobegenerated ifanevent flags issetto1.

<!-- Page 1992 -->

eQEP Registers www.ti.com
1992 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.20 eQEP Capture Timer Register (QCTMR)
Figure 34-40. eQEP Capture Timer Register (QCTMR) [offset =38h]
15 0
QCTMR
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-23. eQEP Capture Time Register (QCTMR) Field Descriptions
Bits Name Description
15-0 QCTMR This register provides time base foredge capture unit.

<!-- Page 1993 -->

www.ti.com eQEP Registers
1993 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.21 eQEP Status Register (QEPSTS)
Figure 34-41. eQEP Status Register (QEPSTS) [offset =3Ah]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
UPEVNT FIDF QDF QDLF COEF CDEF FIMF PCEF
R-1 R-0 R-0 R-0 R/W-1 R/W-1 R/W-1 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 34-24. eQEP Status Register (QEPSTS) Field Descriptions
Bit Field Value Description
15-8 Reserved 0 Always read as0.
7 UPEVNT Unit position event flag.
0 Nounitposition event isdetected.
1 Unit position event isdetected. Write 1toclear.
6 FIDF Direction onthefirstindex marker. Status ofthedirection islatched onthefirstindex event marker.
0 Counter-clockwise rotation (orreverse movement) onthefirstindex event.
1 Clockwise rotation (orforward movement) onthefirstindex event.
5 QDF Quadrature direction flag.
0 Counter-clockwise rotation (orreverse movement).
1 Clockwise rotation (orforward movement).
4 QDLF eQEP direction latch flag. Status ofdirection islatched onevery index event marker.
0 Counter-clockwise rotation (orreverse movement) onindex event marker.
1 Clockwise rotation (orforward movement) onindex event marker.
3 COEF Capture overflow error flag.
0 Sticky bit,cleared bywriting 1.
1 Overflow occurred ineQEP Capture timer (QCTMR).
2 CDEF Capture direction error flag.
0 Sticky bit,cleared bywriting 1.
1 Direction change occurred between thecapture position event.
1 FIMF First index marker flag.
0 Sticky bit,cleared bywriting 1.
1 Setbyfirstoccurrence ofindex pulse.
0 PCEF Position counter error flag. This bitisnotsticky anditisupdated forevery index event.
0 Noerror occurred during thelastindex transition.
1 Position counter error.

<!-- Page 1994 -->

eQEP Registers www.ti.com
1994 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Quadrature Encoder Pulse (eQEP) Module34.3.22 eQEP Capture Timer Latch Register (QCTMRLAT)
Figure 34-42. eQEP Capture Timer Latch Register (QCTMRLAT) [offset =3Ch]
15 0
QCTMRLAT
R-0
LEGEND: R=Read only; -n=value after reset
Table 34-25. eQEP Capture Timer Latch Register (QCTMRLAT) Field Descriptions
Bits Name Description
15-0 QCTMRLAT The eQEP capture timer value canbelatched intothisregister ontwoevents viz., unittimeout event, reading
theeQEP position counter.
34.3.23 eQEP Capture Period Register (QCPRD)
Figure 34-43. eQEP Capture Period Register (QCPRD) [offset =3Eh]
15 0
QCPRD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-26. eQEP Capture Period Register (QCPRD) Field Descriptions
Bits Name Description
15-0 QCPRD This register holds theperiod count value between thelastsuccessive eQEP position events.
34.3.24 eQEP Capture Period Latch Register (QCPRDLAT)
Figure 34-44. eQEP Capture Period Latch Register (QCPRDLAT) [offset =42h]
15 0
QCPRDLAT
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 34-27. eQEP Capture Period Latch Register (QCPRDLAT) Field Descriptions
Bits Name Description
15-0 QCPRDLAT eQEP capture period value canbelatched intothisregister ontwoevents viz., unittimeout event, reading the
eQEP position counter.