# Controller Area Network (DCAN) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1417-1496

---


<!-- Page 1417 -->

1417 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleChapter 27
SPNU563A -March 2018
Controller Area Network (DCAN) Module
This chapter describes thecontroller area network (DCAN) module.
NOTE: This chapter describes asuperset implementation oftheDCAN module. Consult your
device-specific datasheet toidentify theapplicability oftheDMA-related features, thenumber
ofinstantiations oftheDCAN IP,andthenumber ofmailboxes supported onyour specific
device being used.
Topic ........................................................................................................................... Page
27.1 Overview ........................................................................................................ 1418
27.2 CAN Blocks .................................................................................................... 1419
27.3 CAN BitTiming ............................................................................................... 1421
27.4 CAN Module Configuration .............................................................................. 1425
27.5 Message RAM................................................................................................. 1428
27.6 Message Interface Register Sets....................................................................... 1433
27.7 Message Object Configurations ........................................................................ 1436
27.8 Message Handling ........................................................................................... 1438
27.9 CAN Message Transfer .................................................................................... 1443
27.10 Interrupt Functionality .................................................................................... 1444
27.11 Global Power Down Mode ............................................................................... 1446
27.12 Local Power Down Mode ................................................................................ 1447
27.13 GIOSupport .................................................................................................. 1447
27.14 Test Modes ................................................................................................... 1449
27.15 SECDED Mechanism ...................................................................................... 1453
27.16 Debug/Suspend Mode .................................................................................... 1454
27.17 DCAN Control Registers ................................................................................. 1454

<!-- Page 1418 -->

Overview www.ti.com
1418 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.1 Overview
The Controller Area Network isahigh-integrity, serial, multi-master communication protocol fordistributed
real-time applications. This CAN module isimplemented according toISO 11898-1 andissuitable for
industrial, automotive andgeneral embedded communications.
27.1.1 Features
The DCAN module provides thefollowing features:
Protocol
*Supports CAN protocol version 2.0part A,B
Speed
*Bitrates upto1MBit/s
MailBox
*Configurable Message objects
*Individual identifier masks foreach message object
*Programmable FIFO mode formessage objects
High Speed MailBox Access
*DMA access toMessage RAM.
Power
*Global power down andwakeup support
*Local power down andwakeup support
Debug
*Suspend mode fordebug support
*Programmable loop-back modes forself-test operation
*Direct access toMessage RAM intestmode
*Supports Two interrupt lines -Level 0andLevel 1
Others
*Automatic Message RAM initialization
*Automatic busonafter Bus-Off state byaprogrammable 32-bit timer
*CAN Rx/Txpins configurable asgeneral purpose IOpins
*Software module reset
*Message RAM with SECDED mechanism
*Dual clock source toreduce jitter
27.1.2 Functional Description
The CAN protocol isanISO standard (ISO 11898) forserial data communication. This protocol uses Non-
Return ToZero (NRZ) with bit-stuffing. And thecommunication iscarried over atwo-wire balanced
signaling scheme.
The DCAN data communication happens through theCAN_TX andCAN_RX pins. Anadditional
transceiver hardware isrequired fortheconnection tothephysical layer (CAN bus) CAN_High and
CAN_Low.
The DCAN register setcanbeaccessed directly bytheCPU. These registers areused tocontrol and
configure theCAN module andtheMessage RAM.
Individual CAN message objects should beconfigured forcommunication over aCAN network. The
message objects andidentifier masks arestored intheMessage RAM.

<!-- Page 1419 -->

DCAN
Module InterfaceCAN Core
Message HandlerCAN_TX
CAN_CLKCAN_RX
VCLK
VCLKA Message
RAM
Message
Objects
Registers and
Message Object AccessMessage
RAM
InterfaceMCU
CPU
www.ti.com CAN Blocks
1419 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleThe CAN module internally handles functions such acceptance filtering, transfer ofmessages from andto
theMessage RAM, handling oftransmission requests aswell asthegeneration ofinterrupts orDMA
requests.
27.2 CAN Blocks
The DCAN Module, shown inFigure 27-1,comprises ofthefollowing basic blocks.
27.2.1 CAN Core
The CAN Core consists oftheCAN Protocol Controller andtheRx/Tx Shift Register. Ithandles allISO
11898-1 protocol functions.
27.2.2 Message RAM
The DCAN Message RAM enables storage ofCAN messages. Actual Device datasheet provides the
details oftheMessage RAM address.
27.2.3 Message Handler
The Message Handler isastate machine thatcontrols thedata transfer between thesingle ported
Message RAM andtheCAN Core 'sRx/Tx Shift Register. Italso handles acceptance filtering andthe
interrupt/DMA request generation asprogrammed inthecontrol registers.
Figure 27-1. DCAN Block Diagram

<!-- Page 1420 -->

CAN Blocks www.ti.com
1420 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.2.4 Message RAM Interface
The Interface Register sets control theCPU read andwrite accesses totheMessage RAM. There are
three interface registers IF1, IF2, andIF3:
*IF1andIF2Interface Registers sets forread andwrite access.
*IF3Interface Register setforread access only.
The Interface Registers have thesame word-length astheMessage RAM. Additional information canbe
found inSection 27.6.
27.2.5 Register andMessage Object Access
During normal operation, data consistency ofthemessage objects isguaranteed byindirectly accessing
themessage objects through theinterface registers IF1andIF2.
Inorder tobeable toperform tests onthemessage object memory, adedicated testmode hasbeen
implemented, thatallows direct access byeither theCPU orDMA. During normal operation direct access
hastobeavoided.
27.2.6 Dual Clock Source
Two clock domains areprovided totheDCAN module:
1.VCLK -The peripheral synchronous clock domain asthegeneral module clock source.
2.VCLKA -The peripheral asynchronous clock source domain provided totheCAN core asclock source
(CAN_CLK) forgenerating theCAN BitTiming.
Ifafrequency modulated clock output from FMPLL isused astheVCLK source, then VCLKA should be
derived from anunmodulated clock source (forexample, OSCIN source).
The clock source forVCLKA isselected bythePeripheral Asynchronous Clock Source Register inthe
system module.
Both clock domains canbederived from thesame clock source (sothatVCLK =VCLKA). However, if
frequency modulation intheFMPLL isenabled (spread spectrum clock), then duetothehigh precision
clocking requirements oftheCAN Core, theFMPLL clock source should notbeused forVCLKA.
Alternatively, aseparate clock without anymodulation (forexample, derived directly from theOSCIN
clock) should beused forVCLKA.
Refer tothesystem module reference guide andthedevice datasheet formore information how to
configure therelevant clock source registers inthesystem module.
Between thetwoclock domains, asynchronization mechanism isimplemented intheDCAN module in
order toensure correct data transfer.
NOTE: Ifthedual clock functionality isused, then VCLK must always behigher orequal to
CAN_CLK (derived from theasynchronous clock source), inorder toachieve astable
functionality oftheDCAN. Here also thefrequency shift ofthemodulated VCLK hastobe
considered:
ƒ0,VCLK±ΔƒFM,VCLK≥ƒCANCLK
The CAN Core hastobeprogrammed toatleast 8clock cycles perbittime. Toachieve a
transfer rate of1MBaud when using theasynchronous clock domain astheclock source for
CAN_CLK, anoscillator frequency of8MHz orhigher hastobeused.

<!-- Page 1421 -->

1 time quantum
(tq)Sync_ Prop_Seg Phase_Seg1 Phase_Seg2
Sample pointNominal CAN bit time
Seg
www.ti.com CAN BitTiming
1421 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.3 CAN BitTiming
The DCAN supports bitrates between less than 1kBit/s and1000 kBit/s.
Each member oftheCAN network hasitsown clock generator, typically derived from acrystal oscillator.
The Bittiming parameters canbeconfigured individually foreach CAN node, creating acommon Bitrate
even though theCAN nodes 'oscillator periods (fosc)may bedifferent.
27.3.1 BitTime andBitRate
According totheCAN specification, theBittime isdivided intofour segments (see Figure 27-2 ):
*Synchronization Segment (Sync_Seg)
*Propagation Time Segment (Prop_Seg)
*Phase Buffer Segment 1(Phase_Seg1)
*Phase Buffer Segment 2(Phase_Seg2)
Figure 27-2. BitTiming
Each segment consists ofaspecific number oftime quanta. The length ofonetime quantum, (tq),which is
thebasic time unitofthebittime, isgiven bytheCAN_CLK andtheBaud Rate Prescalers (BRPE and
BRP). With these twoBaud Rate Prescalers combined, divider values from 1to1024 canbeprogrammed:
tq=Baud Rate Prescaler /CAN_CLK
Apart from thefixed length ofthesynchronization segment, these numbers areprogrammable.
Table 27-1 describes theminimum programmable ranges required bytheCAN protocol. Agiven bitrate
may bemetbydifferent Bittime configurations.
NOTE: Forproper functionality oftheCAN network, thephysical delay times andtheoscillator 's
tolerance range have tobeconsidered.
Table 27-1. Parameters oftheCAN BitTime
Parameter Range Remark
Sync_Seg 1tq(fixed) Synchronization ofbusinput toCAN_CLK
Prop_Seg [1...8]tq Compensates forthephysical delay times
Phase_Seg1 [1...8]tq May belengthened temporarily bysynchronization
Phase_Seg2 [1...8]tq May beshortened temporarily bysynchronization
Synchronization Jump Width (SJW) [1...4]tq May notbelonger than either Phase Buffer Segment

<!-- Page 1422 -->

I: df ≤min(Phase_Seg1, Phase_Seg2)
[2x(13xbit_time - )] Phase_Seg2- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
II: df ≤SJW
20xbit_time- - - - - - - - - - - - - - - - - - - - - - - - -
(1-df) x f f (1+df) x fnom osc nom≤ ≤
CAN BitTiming www.ti.com
1422 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.3.1.1 Synchronization Segment
The Synchronization Segment (Sync_Seg) isthepart ofthebittime where edges oftheCAN buslevel are
expected tooccur. Ifanedge occurs outside ofSync_Seg, itsdistance totheSync_Seg iscalled the
phase error ofthisedge.
27.3.1.2 Propagation Time Segment
This part ofthebittime isused tocompensate physical delay times within theCAN network. These delay
times consist ofthesignal propagation time onthebusandtheinternal delay time oftheCAN nodes.
27.3.1.3 Phase Buffer Segments andSynchronization
The Phase Buffer Segments (Phase_Seg1 andPhase_Seg2) andtheSynchronization Jump Width (SJW)
areused tocompensate fortheoscillator tolerance.
The Phase Buffer Segments surround thesample point. The Phase Buffer Segments may belengthened
orshortened bysynchronization.
The Synchronization Jump Width (SJW) defines how fartheresynchronizing mechanism may move the
sample point inside thelimits defined bythePhase Buffer Segments tocompensate foredge phase
errors.
Synchronizations occur onedges from recessive todominant. Their purpose istocontrol thedistance
between edges andsample points.
Edges aredetected bysampling theactual buslevel ineach time quantum andcomparing itwith thebus
level attheprevious sample point. Asynchronization may bedone only ifarecessive bitwas sampled at
theprevious sample point andiftheactual time quantum 'sbuslevel isdominant.
Anedge issynchronous ifitoccurs inside ofSync_Seg, otherwise itsdistance totheSync_Seg isthe
edge phase error, measured intime quanta. Iftheedge occurs before Sync_Seg, thephase error is
negative, else itispositive.
27.3.1.4 Oscillator Tolerance Range
With theintroduction ofCAN protocol version 1.2,theoption tosynchronize onedges from dominant to
recessive became obsolete. Only edges from recessive todominant areconsidered forsynchronization.
The protocol update toversion 2.0(AandB)hadnoinfluence ontheoscillator tolerance.
The tolerance range dfforanoscillator 'sfrequency foscaround thenominal frequency fnomwith:
(35)
depends ontheproportions ofPhase_Seg1, Phase_Seg2, SJW, andthebittime. The maximum tolerance
dfisthedefined bytwoconditions (both shall bemet):
(36)
Ithastobeconsidered thatSJW may notbelarger than thesmaller ofthePhase Buffer Segments and
thatthePropagation Time Segment limits thatpart ofthebittime thatmay beused forthePhase Buffer
Segments.
The combination Prop_Seg =1andPhase_Seg1 =Phase_Seg2 =SJW =4allows thelargest possible
oscillator tolerance of1.58%. This combination with aPropagation Time Segment ofonly 10% ofthebit
time isnotsuitable forshort bittimes; itcanbeused forbitrates ofupto125kBit/s (bittime =8μs)with a
buslength of40m.

<!-- Page 1423 -->

www.ti.com CAN BitTiming
1423 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.3.2 DCAN BitTiming Registers
IntheDCAN, thebittiming configuration isprogrammed intworegister bytes, additionally athird byte for
abaud rate prescaler extension of4bits(BREP) isprovided. The sum ofProp_Seg andPhase_Seg1 (as
TSEG1) iscombined with Phase_Seg2 (asTSEG2) inonebyte, SJW andBRP (plus BRPE inthird byte)
arecombined intheother byte
Inthisbittiming register, thecomponents TSEG1, TSEG2, SJW andBRP have tobeprogrammed toa
numerical value thatisoneless than itsfunctional value; soinstead ofvalues intherange of[1...n],
values intherange of[0...n-1] areprogrammed. That way, SJW (functional range of[1...4])is
represented byonly twobits.
Therefore thelength oftheBittime is(programmed values) [TSEG1 +TSEG2 +3]tqor(functional values)
[Sync_Seg +Prop_Seg +Phase_Seg1 +Phase_Seg2] tq.
The data intheBitTiming Register (BTR) istheconfiguration input oftheCAN protocol controller. The
Baud Rate Prescaler (configured byBRPE/BRP) defines thelength ofthetime quantum (the basic time
unitofthebittime); theBitTiming Logic (configured byTSEG1, TSEG2, andSJW) defines thenumber of
time quanta inthebittime.
27.3.2.1 Calculation oftheBitTiming Parameters
Usually, thecalculation ofthebittiming configuration starts with adesired bitrate orbittime. The resulting
Bittime (1/Bitrate) must beaninteger multiple oftheCAN clock period.
NOTE: 8MHz istheminimum CAN clock frequency required tooperate theDCAN atabitrate of
1MBit/s.
The bittime may consist of8to25time quanta. The length ofthetime quantum tqisdefined bytheBaud
Rate Prescaler with tq=(Baud Rate Prescaler) /CAN_CLK. Several combinations may lead tothedesired
bittime, allowing iterations ofthefollowing steps.
First part ofthebittime tobedefined istheProp_Seg. Itslength depends onthedelay times measured in
thesystem. Amaximum buslength aswell asamaximum node delay hastobedefined forexpandible
CAN bussystems. The resulting time forProp_Seg isconverted intotime quanta (rounded uptothe
nearest integer multiple oftq).
The Sync_Seg is1tqlong (fixed), leaving (bittime -Prop_Seg -1)tqforthetwoPhase Buffer Segments.
Ifthenumber ofremaining tqiseven, thePhase Buffer Segments have thesame length, Phase_Seg2 =
Phase_Seg1, else Phase_Seg2 =Phase_Seg1 +1.
The minimum nominal length ofPhase_Seg2 hastoberegarded aswell. Phase_Seg2 may notbeshorter
than anyCAN controller 'sInformation Processing Time inthenetwork, which isdevice dependent andcan
beintherange of[0...2]tq.
The length oftheSynchronization Jump Width issettoitsmaximum value, which istheminimum of4and
Phase_Seg1.
Ifmore than oneconfigurations arepossible toreach acertain Bitrate, itisrecommended tochoose the
configuration thatallows thehighest oscillator tolerance range.
CAN nodes with different clocks require different configurations tocome tothesame bitrate. The
calculation ofthepropagation time intheCAN network, based onthenodes with thelongest delay times,
isdone once forthewhole network.
The CAN system 'soscillator tolerance range islimited bythenode with thelowest tolerance range.
The calculation may show thatbuslength orbitrate have tobedecreased orthattheoscillator
frequencies 'stability hastobeincreased inorder tofindaprotocol compliant configuration oftheCAN bit
timing.

<!-- Page 1424 -->

0.1µs
[2 x (13 x 1µs - 0.1µs)]- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
min(TSeg1, TSeg2)
[2 x (13 x bit_time - TSeg2)]- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CAN BitTiming www.ti.com
1424 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleThe resulting configuration iswritten intotheBitTiming Register:
Tseg2 =Phase_Seg2 -1
Tseg1 =Phase_Seg1 +Prop_Seg -1
SJW =SynchronizationJumpWidth -1
BRP =Prescaler -1
27.3.2.2 Calculation ofBRP Values
IfBaud andCAN_CLK(VCLK) arealready known, theBRP/BRPE values need tobecalculated tobe
programmed intotheregister. Itiscalculated using thefollowing equation:
BRP =CAN_CLK /(BAUD)(1 +TSEG1 +TSEG2) (37)
27.3.2.3 Example forBitTiming atHigh Baudrate
Inthisexample, thefrequency ofCAN_CLK is10MHz, BRP is0,thebitrate is1MBit/s.
tq 100 ns = tCAN_CLK
delay ofbusdriver 60 ns
delay ofreceiver circuit 40 ns
delay ofbusline(40m) 220 ns
tProp 700 ns = INT(2×delays +1)=7*tq
tSJW 100 ns = 1×tq
tTSeg1 800 ns = tProp+tSJW
tTSeg2 100 ns = Information Processing Time +1*tq
tSync-Seg 100 ns = 1×tq
bittime 1000 ns = tSync-Seg +tTSeg1 +tTSeg2
tolerance forCAN_CLK 1.58 % =
(38)
=
(39)
= 0.38%
Inthisexample, theconcatenated bittime parameters are(1-1)3&(8-1)4&(1-1)2&(1-1)6,sotheBit
Timing Register isprogrammed to0000 0700h.

<!-- Page 1425 -->

3µs
[2 x (13 x 9µs - 3µs)]- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
min(TSeg1, TSeg2)
[2 x (13 x bit_time - TSeg2)]- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
www.ti.com CAN Module Configuration
1425 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.3.2.4 Example forBitTiming atLow Baudrate
Inthisexample, thefrequency ofCAN_CLK is2MHz, BRP is1,thebitrate is100KBit/s.
tq 1 µs = 2×tCAN_CLK
delay ofbusdriver 200 ns
delay ofreceiver circuit 80 ns
delay ofbusline(40m) 220 ns
tProp 1 µs = 1×tq
tSJW 4 µs = 4×tq
tTSeg1 5 µs = tProp+tSJW
tTSeg2 3 µs = Information Processing Time +3×tq
tSync-Seg 1 µs = 1×tq
bittime 9 µs = tSync-Seg +tTSeg1 +tTSeg2
tolerance forCAN_CLK 0.43 % =
(40)
=
(41)
= 1.32%
Inthisexample, theconcatenated bittime parameters are(3-1)3&(5-1)4&(4-1)2&(2-1)6,sotheBit
Timing Register isprogrammed to0000 24C1h.
27.4 CAN Module Configuration
After ahardware reset allCAN protocol functions aredisabled.The CAN module must beinitialized and
configured before itcanparticipate ontheCAN bus.
27.4.1 DCAN RAM Initialization Through Hardware
Tostart with aclean DCAN RAM ,thecomplete DCAN RAM hastobeinitialized with zeros andtheECC
bitssetaccordingly byconfiguring thefollowing registers inthesystem module:
1.Memory Hardware Initialization Global Control Register (MINITGCR)
2.Memory Initialization Enable Register (MSINENA)
Formore details onRAM hardware initialization support, refer tothesystem module reference guide.
27.4.2 CAN Module Initialization
Toinitialize theCAN Controller, youhave tosetuptheCAN Bittiming andthose message objects that
have tobeused forCAN communication. Message objects thatarenotneeded, canbedeactivated.
Sothetwocritical steps are:
1.Configuration ofCAN BitTimings
2.Configuration ofMessage Objects
27.4.2.1 Software Configuration ofCAN BitTimings
This step involves configuring theCAN baud rate register with thecalculated CAN bittiming value. The
calculation procedure ofCAN bittiming values forBTR register arementioned inSection 27.3.Refer to
Figure 27-3 forCAN bittiming software configuration flow.

<!-- Page 1426 -->

Set Init = 1
Set CCE = 1
Wait for Init =1
Write Bit timing values into BTR
Clear CCE and Init
Wait for Init =0CCE = 0 , Init =0Normal Mode
Initialization Mode
Normal Mode
CAN Module Configuration www.ti.com
1426 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleFigure 27-3. CAN Bit-timing Configuration
Step 1:Enter "initialization mode "bysetting theInit(Initialization) bitintheCAN Control Register.
While theInitbitisset,themessage transfer from andtotheCAN busisstopped, andthestatus ofthe
CAN_TX output isrecessive (high).
The CAN error counters arenotupdated. Setting theInitbitdoes notchange anyother configuration
register.
Also note thattheCAN module isalso ininitialization mode onhardware reset andduring Bus-Off.
Step 2:SettheCCE (Configure Change Enable) bitintheCAN Control Register.
The access totheBitTiming Register fortheconfiguration oftheBittiming isenabled when both Initand
CCE bitsintheCAN Control Register areset.
Step 3:Wait fortheInitbittogetset.This would make sure thatthemodule hasentered "Initialization
mode ".
Step 4:Write theBit-Timing values intotheBit-Timing Register (BTR).
Refer toSection 27.3.2.1 forBTR value calculation foragiven bit-timing.
Step 5:Clear theCCE bitfollowed byInitbit.
Step 6:Wait fortheInitbittoclear. This would make sure thatthemodule hascome outof"initialization
mode ".
After step 6(Initbitcleared), themodule willattempt asynchronization ontheCAN bus, provided thatthe
BTR settings aremeeting theCAN busparameters.

<!-- Page 1427 -->

www.ti.com CAN Module Configuration
1427 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleNOTE: The module would notcome outofthe"initialization mode "ifanyincorrect BTR values are
written instep 4.
27.4.2.2 Configuration ofMessage Objects
The whole Message RAM should beconfigured before putting theCAN intooperation. Allthemessage
objects aredeactivated bydefault. You should configure themessage object thataretobeused toa
particular identifier. youcanchange theconfiguration ofanymessage object ordeactivate itwhen
required.
The message objects canbeconfigured only through theInterface registers (IFx) andtheCPU does not
have direct access tothemessage object (Message RAM) when DCAN isinoperation.
Toconfigure themessage objects, youmust know about:
1.The message object structure (Section 27.5)
2.The interface register set(IFx) (Section 27.6)
NOTE: The message objects initialization isindependent ofthebit-timing configuration procedure.

<!-- Page 1428 -->

Message RAM www.ti.com
1428 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.5 Message RAM
The DCAN Message RAM contains message objects andECC bitsforthemessage objects.
27.5.1 Structure ofMessage Objects
Figure 27-4 shows thestructure ofamessage object.
The grayed fields arethose parts ofthemessage object thatarerepresented indedicated registers. For
example, thetransmit request flags ofallmessage objects arerepresented incentralized transmit request
registers.
Figure 27-4. Structure ofaMessage Object
Message Object
UMask Msk[28:0] MXtd MDir EoB unused NewDat MsgLst RxIE TxIE IntPnd RmtEn TxRqst
MsgVal ID[28:0] Xtd Dir DLC[3:0] Data 0 Data 1 Data 2Data 3Data 4Data 5 Data 6 Data 7
Table 27-2. Message Object Field Descriptions
Name Value Description
MsgVal Message valid
0 The message object isignored bytheMessage Handler.
1 The message object istobeused bytheMessage Handler.
Note: The CPU should reset theMsgVal bitofallunused Messages Objects during theinitialization
before itresets bitInitintheCAN Control Register. MsgVal must also bereset ifthemessages object
isnolonger used inoperation. Forreconfiguration ofmessage objects during normal operation see
Section 27.7.6 andSection 27.7.7 .
UMask Use Acceptance Mask
0 Mask bits(Msk[28:0], MXtd andMDir) areignored andnotused foracceptance filtering.
1 Mask bitsareused foracceptance filtering.
Note: IftheUMask bitissetto1,themessage object 'smask bitshave tobeprogrammed during
initialization ofthemessage object before MsgVal issetto1.
ID[28:0] Message Identifier
ID[28:0] 29-bit ("extended ")identifier bits
ID[28:18] 11-bit ("standard ")identifier bits
Msk[28:0] Identifier Mask
0 The corresponding bitinthemessage identifier isnotused foracceptance filtering (don 'tcare).
1 The corresponding bitinthemessage identifier isused foracceptance filtering.
Xtd Extended Identifier
0 The 11-bit ("standard ")identifier willbeused forthismessage object.
1 The 29-bit ("extended ")identifier willbeused forthismessage object.
MXtd Mask Extended Identifier
0 The extended identifier bit(IDE) hasnoeffect ontheacceptance filtering.
1 The extended identifier bit(IDE) isused foracceptance filtering.
Note: When 11-bit ("standard ")identifiers areused foramessage object, theidentifiers ofreceived
data frames arewritten intobitsID[28:18]. Foracceptance filtering, only these bitstogether with mask
bitsMsk[28:18] areconsidered.
Dir Message Direction
0 Direction =receive: OnTxRqst, aremote frame with theidentifier ofthismessage object is
transmitted. Onreception ofadata frame with matching identifier, themessage isstored inthis
message object.
1 Direction =transmit: OnTxRqst, adata frame istransmitted. Onreception ofaremote frame with
matching identifier, theTxRqst bitofthismessage object isset(ifRmtEn =1).
MDir Mask Message Direction
0 The message direction bit(Dir) hasnoeffect ontheacceptance filtering.
1 The message direction bit(Dir) isused foracceptance filtering.

<!-- Page 1429 -->

www.ti.com Message RAM
1429 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-2. Message Object Field Descriptions (continued)
Name Value Description
EOB End ofBlock
0 The message object ispart ofaFIFO Buffer block andisnotthelastmessage object ofthisFIFO
Buffer block.
1 The message object isasingle message object orthelastmessage object inaFIFO Buffer block.
Note: This bitisused toconcatenate multiple message objects tobuild aFIFO Buffer. Forsingle
message objects (not belonging toaFIFO Buffer), thisbitmust always besetto1.
NewDat New Data
0 Nonew data hasbeen written intothedata bytes ofthismessage object bytheMessage Handler
since thelasttime when thisflagwas cleared bytheCPU.
1 The Message Handler ortheCPU haswritten new data intothedata bytes ofthismessage object.
MsgLst Message Lost (only valid formessage objects with direction =receive)
0 Nomessage was lostsince thelasttime when thisbitwas reset bytheCPU.
1 The Message Handler stored anew message intothismessage object when NewDat was stillset,so
theprevious message hasbeen overwritten.
RxIE Receive Interrupt Enable
0 IntPnd willnotbetriggered after thesuccessful reception ofaframe.
1 IntPnd willbetriggered after thesuccessful reception ofaframe.
TxIE Transmit Interrupt Enable
0 IntPnd willnotbetriggered after thesuccessful transmission ofaframe.
1 IntPnd willbetriggered after thesuccessful transmission ofaframe.
IntPnd Interrupt Pending
0 This message object isnotthesource ofaninterrupt.
1 This message object isthesource ofaninterrupt. The Interrupt Identifier intheInterrupt Register will
point tothismessage object ifthere isnoother interrupt source with higher priority.
RmtEn Remote Enable
0 Atthereception ofaRemote Frame, TxRqst isnotchanged.
1 Atthereception ofaRemote Frame, TxRqst isset.
TxRqst Transmit Request
0 This message object isnotwaiting foratransmission.
1 The transmission ofthismessage object isrequested andisnotyetdone.
DLC[3:0] Data Length Code
0-8 Data Frame has0-8data bytes.
9-15 Data Frame has8data bytes.
Note: The Data Length Code ofamessage object must bedefined tothesame value asinthe
corresponding objects with thesame identifier atother nodes. When theMessage Handler stores a
data frame, itwillwrite theDLC tothevalue given bythereceived message.
Data 0 1stdata byte ofaCAN Data Frame
Data 1 2nddata byte ofaCAN Data Frame
Data 2 3rddata byte ofaCAN Data Frame
Data 3 4thdata byte ofaCAN Data Frame
Data 4 5thdata byte ofaCAN Data Frame
Data 5 6thdata byte ofaCAN Data Frame
Data 6 7thdata byte ofaCAN Data Frame
Data 7 8thdata byte ofaCAN Data Frame
Note: Byte Data 0isthefirstdata byte shifted intotheshift register oftheCAN Core during a
reception, byte Data 7isthelast. When theMessage Handler stores adata frame, itwillwrite allthe
eight data bytes intoamessage object. IftheData Length Code isless than 8,theremaining bytes of
themessage object may beoverwritten byundefined values.

<!-- Page 1430 -->

Message RAM www.ti.com
1430 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.5.2 Addressing Message Objects inRAM
The starting location ofaparticular message object inRAM is:
Message RAM base address +(message object number) ×0x20.
This means thatMessage Object 1starts atoffset 0x0020; Message Object 2starts atoffset 0x0040, and
soon.
NOTE: 0isnotavalid message object number. Ataddress 0x0000, message object number 64is
located. Writing totheaddress ofanunimplemented message object may overwrite an
implemented message object.
The base address forDCAN1 RAM isFF1E 0000h, DCAN2 RAM isFF1C 0000h, DCAN3
RAM isFF1A 0000h, andDCAN4 RAM base address isFF18 0000h.
Message Object number 1hasthehighest priority.
Table 27-3. Message RAM Addressing inDebug/Suspend andRDA Mode
Message
Object
NumberBase Address OffsetWord
NumberDebug/Suspend mode,
seeSection 27.5.3RDA mode,
seeSection 27.5.4
1 0x0020 1 Reserved Data Bytes 4-7
0x0024 2 MXtd, MDir, Mask Data Bytes 0-3
0x0028 3 Xtd, Dir,ID ID[27:0], DLC
0x002C 4 Ctrl Mask, Xtd, Dir,ID[28]
0x0030 5 Data Bytes 3-0 Reserved, Ctrl, MXtd, MDir
0x0034 6 Data Bytes 7-4 --
: : : : :
31 0x03E0 1 Reserved Data Bytes 4-7
0x03E4 2 MXtd, MDir, Mask Data Bytes 0-3
0x03E8 3 Xtd, Dir,ID ID[27]:0, DLC
0x03EC 4 Ctrl Mask, Xtd, Dir,ID[28]
0x03F0 5 Data Bytes 3-0 Reserved, Ctrl, MXtd, MDir
0x03F4 6 Data Bytes 7-4 --
: : : : :
63 0x07E0 1 Reserved Data Bytes 4-7
0x07E4 2 MXtd, MDir, Mask Data Bytes 0-3
0x07E8 3 Xtd, Dir,ID ID[27:0], DLC
0x07EC 4 Ctrl Mask, Xtd, Dir,ID[28]
0x07F0 5 Data Bytes 3-0 Reserved, Ctrl, MXtd, MDir
0x07F4 6 Data Bytes 7-4 --
64 0x0000 1 Reserved Data Bytes 4-7
0x0004 2 MXtd, MDir, Mask Data Bytes 0-3
0x0008 3 Xtd, Dir,ID ID[27]:0, DLC
0x000C 4 Ctrl Mask, Xtd, Dir,ID[28]
0x0010 5 Data Bytes 3-0 Reserved, Ctrl, MXtd, MDir
0x0014 6 Data Bytes 7-4 --

<!-- Page 1431 -->

www.ti.com Message RAM
1431 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.5.3 Message RAM Representation inDebug/Suspend Mode
InDebug/Suspend mode, theMessage RAM willbememory mapped. This allows theexternal debug unit
toaccess theMessage RAM.
NOTE: During Debug/Suspend Mode, theMessage RAM cannot beaccessed viatheIFxregister
sets.
Figure 27-5. Message RAM Representation inDebug/Suspend Mode
Bit 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
MsgAddr +0x00Reserved
Reserved Reserved
MsgAddr +0x04MXtd MDir Rsvd Msk[28:16]
Msk[15:0]
MsgAddr +0x08Rsvd Xtd Dir ID[28:16]
ID[15:0]
MsgAddr +0x0CReserved
Rsvd MsgLst Rsvd UMask TxIE RxIE RmtEn Rsvd EOB Reserved DLC[3:0]
MsgAddr +0x10Data 3 Data 2
Data 1 Data 0
MsgAddr +0x14Data 7 Data 6
Data 5 Data 4
27.5.4 Message RAM Representation inDirect Access Mode
When theRDA bitinTest Register issetwhile theDCAN module isinTest Mode (Test bitinCAN control
register isset), theCPU hasdirect access totheMessage RAM. Due tothe32-bit busstructure, theRAM
issplit intoword lines tosupport thisfeature. The CPU hasaccess tooneword lineatatime only.
InRAM Direct Access mode, theRAM isrepresented byacontinuous memory space within theaddress
frame oftheDCAN module, starting attheMessage RAM base address.
NOTE: During Direct Access Mode, theMessage RAM cannot beaccessed viatheIFxregister sets.
Before entering RDA mode, itmust beensured thattheInitbitissettoavoid anyconflicts
with themessage handler accessing themessage RAM.
Any read orwrite totheRAM addresses forRamDirectAccess during normal operation mode
(TestMode bitorRDA bitisnotset)willbeignored.
Writes toReserved bitshave noeffect.
Figure 27-6. Message RAM Representation inRAM Direct Access Mode
Bit 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
MsgAddr +0x00Data 4 Data 5
Data 6 Data 7
MsgAddr +0x04Data 0 Data 1
Data 2 Data 3
MsgAddr +0x08ID[27:12]
ID[11:0] DLC[3:0]
MsgAddr +0x0CMsk[28:13]
Msk[12:0] Xtd Dir ID[28]
MsgAddr +0x10Reserved
Reserved Reserved MsgLst UMask TxIE RxIE RmtEn EOB MXtd MDir

<!-- Page 1432 -->

Message RAM www.ti.com
1432 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.5.5 ECC RAM
Ondevices with SECDED implementation forthemessage RAM, theECC bitsarestored inadedicated
ECC RAM area thatismemory-mapped asfollows: The location oftheECC bitsforaparticular message
object inRAM is:Message RAM base address +0x1000 +(message object number) *0x20.
NOTE: A0isnotavalid message object number. Ataddress 0x1000, theECC bitsofthelast
implemented message object arelocated.
Asshown inFigure 27-7,theECC bitsforthelastimplemented Message Object (here: 128) arelocated at
offset 0x1000; theECC bitsforMessage Object 1arelocated atoffset 0x1020, andtheECC bitsfor
Message Object 127arelocated atoffset 0x1FE0. The ECC RAM isonly memory mapped ifSECDED
diagnostic mode isenabled.
Figure 27-7. ECC RAM Representation
Bit 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
Msg RAM base +
0x1000Reserved
Reserved ECC[8:0] lastimplemented Message Object (here: 128)
Msg RAM base +
0x1020Reserved
Reserved ECC[8:0] Message Object 1
:
Msg RAM base +
0x1FE0Reserved
Reserved ECC[8:0] Message Object 127

<!-- Page 1433 -->

www.ti.com Message Interface Register Sets
1433 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.6 Message Interface Register Sets
Accesses totheMessage RAM areperformed viatheInterface Register sets:
*Interface Register 1and2(IF1 andIF2)
*Interface Register 3(IF3)
The IF3register setcanbeconfigured toautomatically receive control anduser data from theMessage
RAM when amessage object hasbeen updated after reception ofaCAN message. The CPU does not
need toinitiate thetransfer from Message RAM toIF3register set.
The Message Handler avoids potential conflicts between concurrent accesses toMessage RAM andCAN
frame reception/transmission.
There aretwomodes where theMessage RAM canbedirectly accessed bytheCPU:
1.InDebug/Suspend mode (see Section 27.5.3 )
2.InRAM Direct Access (RDA) mode (see Section 27.5.4 )
FortheMessage RAM Base address, refer tothedevice datasheet.
Acomplete message object (see Section 27.5.1 )orparts ofthemessage object may betransferred
between theMessage RAM andtheIF1/IF2 Register set(see Section 27.17.24 )inonesingle transfer.
27.6.1 Message Interface Register Sets 1and2
The Interface Register sets IF1andIF2provide indirect read/write access from theCPU totheMessage
RAM. The IF1andIF2register sets canbuffer control anduser data tobetransferred toandfrom the
message objects.
Table 27-4. Message Interface Register Sets 1and2
Address IF1Register Set Address IF2Register Set
[CAN Base +]31 1615 0[CAN Base +]31 1615 0
0x100 IF1Command MaskIF1Command
Request0x120 IF2Command MaskIF2Command
Request
0x104 IF1Mask 2 IF1Mask 1 0x124 IF2Mask 2 IF2Mask 1
0x108 IF1Arbitration 2 IF1Arbitration 1 0x128 IF2Arbitration 2 IF2Arbitration 1
0x10C Rsvd IF1Message Control 0x12C Rsvd IF2Message Control
0x110 IF1Data A2 IF1Data A1 0x130 IF2Data A2 IF2Data A1
0x114 IF1Data B2 IF1Data B1 0x134 IF2Data B2 IF2Data B1

<!-- Page 1434 -->

START
WR/RD = 1
Busy = 0Busy = 1
Read message object to IF1/IF2
Write IF1/IF2 to message RAMRead message object to IF1/IF2No YesWrite message number to command registerNo
Yes
Message Interface Register Sets www.ti.com
1434 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.6.2 Using Message Interface Register Sets 1and2
The Command Register addresses thedesired message object intheMessage RAM andspecifies
whether acomplete message object oronly parts should betransferred. The data transfer isinitiated by
writing themessage number tothebits[7:0] oftheCommand Register.
When theCPU initiates adata transfer between theIF1/IF2 Registers andMessage RAM, theMessage
Handler sets theBusy bitintherespective Command Register to'1'.After thetransfer hascompleted, the
Busy bitissetback to'0'(see Figure 27-8).
Figure 27-8. Data Transfer Between IF1/IF2Registers andMessage RAM

<!-- Page 1435 -->

www.ti.com Message Interface Register Sets
1435 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.6.3 Message Interface Register 3
The IF3register setcanautomatically beupdated with received message objects without theneed to
initiate thetransfer from Message RAM byCPU. The intention ofthisfeature ofIF3istoprovide an
interface fortheDMA toread packets efficiently.
Table 27-5. Message Interface Register 3
Address IF3Register Set
[CAN Base +] 31 1615 0
0x140 reserved IF3Observation
0x144 IF3Mask 2 IF3Mask 1
0x148 IF3Arbitration 2 IF3Arbitration 1
0x14C reserved IF3Message Control
0x150 IF3Data A2 IF3Data A1
0x154 IF3Data B2 IF3Data B1
: : :
0x160 IF3Update Enable 2 IF3Update Enable 1
0x164 IF3Update Enable 4 IF3Update Enable 3
0x168 IF3Update Enable 6 IF3Update Enable 5
0x16C IF3Update Enable 8 IF3Update Enable 7
The automatic update functionality canbeprogrammed foreach message object (see IF3Update Enable
Register, Section 27.17.33 ).
Allvalid message objects inMessage RAM thatareconfigured forautomatic update, willbechecked for
active NewDat flags. Ifsuch amessage object isfound, itwillbetransferred totheIF3register (ifno
previous DMA transfers areongoing), controlled byIF3Observation register. Ifmore than oneNewDat
flagisactive, themessage object with thelowest number hasthehighest priority forautomatic IF3update.
The NewDat bitinthemessage object willbereset byatransfer toIF3.
IfDCAN internal IF3update iscomplete, aDMA request isgenerated. The DMA request stays active until
firstread access tooneoftheIF3registers. The DMA functionality hastobeenabled bysetting bitDE3 in
CAN Control register. Please refer tothedevice datasheet tofindoutifthisDMA source isavailable.
NOTE: The IF3register setcannotbeused fortransferring data intomessage objects.

<!-- Page 1436 -->

Message Object Configurations www.ti.com
1436 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.7 Message Object Configurations
This section describes thepossible message object configurations forCAN communication.
27.7.1 Configuration ofaTransmit Object forData Frames
Figure 27-9 shows how aTransmit Object canbeinitialized.
Figure 27-9. Initialization ofaTransmit Object
MsgVal Arb Data Mask EoB Dir NewDat MsgLst RxIE TxIE IntPnd RmtEn TxRqst
1 appl. appl. appl. 1 1 0 0 0 appl. 0 appl. 0
The Arbitration bits(ID[28:0] andXtdbit)aregiven bytheapplication. They define theidentifier andtype
oftheoutgoing message. Ifan11-bit Identifier (Standard Frame) isused (Xtd =0),itisprogrammed to
ID[28:18]. Inthiscase, ID[17:0] canbeignored.
The Data Registers (DLC[3:0] andData0-7) aregiven bytheapplication, TxRqst andRmtEn should notbe
setbefore thedata isvalid.
IftheTxIE bitisset,theIntPnd bitwillbesetafter asuccessful transmission ofthemessage object.
IftheRmtEn bitisset,amatching received Remote Frame willcause theTxRqst bittobeset;theRemote
Frame willautonomously beanswered byaData Frame.
The Mask bits(Msk[28:0], UMask, MXtd, andMDir bits) may beused (UMask =1)toallow groups of
Remote Frames with similar identifiers tosettheTxRqst bit.The Dirbitshould notbemasked. Fordetails,
seeSection 27.8.8 .Identifier masking must bedisabled (UMask =0)ifnoRemote Frames areallowed to
settheTxRqst bit(RmtEn =0).
27.7.2 Configuration ofaTransmit Object forRemote Frames
Itisnotnecessary toconfigure Transmit Objects forthetransmission ofRemote Frames. Setting TxRqst
foraReceive Object willcause thetransmission ofaRemote Frame with thesame identifier astheData
Frame forwhich thisreceive Object isconfigured.
27.7.3 Configuration ofaSingle Receive Object forData Frames
Figure 27-10 shows how aReceive Object forData Frames canbeinitialized.
Figure 27-10. Initialization ofaSingle Receive Object forData Frames
MsgVal Arb Data Mask EoB Dir NewDat MsgLst RxIE TxIE IntPnd RmtEn TxRqst
1 appl. appl. appl. 1 0 0 0 appl. 0 0 0 0
The Arbitration bits(ID[28:0] andXtdbit)aregiven bytheapplication. They define theidentifier andtype
ofaccepted received messages. Ifan11-bit Identifier (Standard Frame) isused (Xtd =0),itis
programmed toID[28:18]. Inthiscase, ID[17:0] canbeignored. When aData Frame with an11-bit
Identifier isreceived, ID[17:0] willbesetto0.
The Data Length Code (DLC[3:0]) isgiven bytheapplication. When theMessage Handler stores aData
Frame inthemessage object, itwillstore thereceived Data Length Code andeight data bytes. IftheData
Length Code isless than 8,theremaining bytes ofthemessage object may beoverwritten bynon
specified values.
The Mask bits(Msk[28:0], UMask, MXtd, andMDir bits) may beused (UMask ='1')toallow groups of
Data Frames with similar identifiers tobeaccepted. The Dirbitshould notbemasked intypical
applications. Ifsome bitsoftheMask bitsaresetto"don'tcare",thecorresponding bitsoftheArbitration
Register willbeoverwritten bythebitsofthestored Data Frame.
IftheRxIE bitisset,theIntPnd bitwillbesetwhen areceived Data Frame isaccepted andstored inthe
message object.
IftheTxRqst bitisset,thetransmission ofaRemote Frame with thesame identifier asactually stored in
theArbitration bitswillbetriggered. The content oftheArbitration bitsmay change iftheMask bitsare
used (UMask =1foracceptance filtering.

<!-- Page 1437 -->

www.ti.com Message Object Configurations
1437 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.7.4 Configuration ofaSingle Receive Object forRemote Frames
Figure 27-11 shows how aReceive Object forRemote Frames canbeinitialized.
Figure 27-11. Initialization ofaSingle Receive Object forRemote Frames
MsgVal Arb Data Mask EoB Dir NewDat MsgLst RxIE TxIE IntPnd RmtEn TxRqst
1 appl. appl. appl. 1 1 0 0 appl. 0 0 0 0
Receive Objects forRemote Frames may beused tomonitor Remote Frames ontheCAN bus. The
Remote Frame stored intheReceive Object willnottrigger thetransmission ofaData Frame. Receive
Objects forRemote Frames may beexpanded toaFIFO buffer, seeSection 27.7.5 .
UMask must besetto1.The Mask bits(Msk[28:0], UMask, MXtd, andMDir bits) may besetto"must-
match "orto"don'tcare",toallow groups ofRemote Frames with similar identifiers tobeaccepted. The Dir
bitshould notbemasked intypical applications. Fordetails, seeSection 27.8.8 .
The Arbitration bits(ID[28:0] andXtdbit)may begiven bytheapplication. They define theidentifier and
type ofaccepted received Remote Frames. Ifsome bitsoftheMask bitsaresetto"don'tcare",the
corresponding bitsoftheArbitration bitswillbeoverwritten bythebitsofthestored Remote Frame. Ifan
11-bit Identifier (Standard Frame) isused (Xtd =0),itisprogrammed toID[28:18]. Inthiscase, ID[17:0]
canbeignored. When aRemote Frame with an11-bit Identifier isreceived, ID[17:0] willbesetto0.
The Data Length Code (DLC[3:0]) may begiven bytheapplication. When theMessage Handler stores a
Remote Frame inthemessage object, itwillstore thereceived Data Length Code. The data bytes ofthe
message object willremain unchanged.
IftheRxIE bitisset,theIntPnd bitwillbesetwhen areceived Remote Frame isaccepted andstored in
themessage object.
27.7.5 Configuration ofaFIFO Buffer
With theexception oftheEoB bit,theconfiguration ofReceive Objects belonging toaFIFO Buffer isthe
same astheconfiguration ofasingle Receive Object.
Toconcatenate multiple message objects toaFIFO Buffer, theidentifiers andmasks (ifused) ofthese
message objects have tobeprogrammed tomatching values. Due totheimplicit priority ofthemessage
objects, themessage object with thelowest number willbethefirstmessage object oftheFIFO Buffer.
The EoB bitofallmessage objects ofaFIFO Buffer except thelastonehave tobeprogrammed tozero.
The EoB bitsofthelastmessage object ofaFIFO Buffer issettoone, configuring itastheendofthe
block.
27.7.6 Reconfiguration ofMessage Objects fortheReception ofFrames
Amessage object with Dir='0'isconfigured forthereception ofdata frames, with Dir='1'AND Umask =
'1'AND RmtEn ='0'itisconfigured forthereception ofremote frames.
Itisnecessary toreset MsgVal tonotvalid before changing anyofthefollowing configuration andcontrol
bits: ID[28:0], Xtd, Dir,DLC[3:0], RxIE, TxIE, RmtEn, EoB, Umask, Msk[28:0], MXtd, andMDir.
These parts ofamessage object may bechanged without clearing MsgVal: Data[7:0], TxRqst, NewDat,
MsgLst, andIntPnd.
27.7.7 Reconfiguration ofMessage Objects fortheTransmission ofFrames
Amessage object with Dir='1'AND (Umask ='0'ORRmtEn ='1')isconfigured forthetransmission of
data frames.
Itisnecessary toreset MsgVal tonotvalid before changing anyofthefollowing configuration andcontrol
bits: Dir,RxIE, TxIE, RmtEn, EoB, Umask, Msk[28:0], MXtd, andMDir
These parts ofamessage object may bechanged without clearing MsgVal: ID[28-0], Xtd, DLC[3:0],
Data[7:0], TxRqst, NewDat, MsgLst, andIntPnd.

<!-- Page 1438 -->

Message Handling www.ti.com
1438 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.8 Message Handling
When initialization isfinished, theDCAN module synchronizes itself tothetraffic ontheCAN bus. Itdoes
acceptance filtering onreceived messages andstores those frames thatareaccepted intothedesignated
message objects.
The application hastoupdate thedata ofthemessages tobetransmitted andenable andrequest their
transmission. The transmission isrequested automatically when amatching Remote Frame isreceived.
The application may read messages thatarereceived andaccepted. Messages thatarenotread before
thenext messages isaccepted forthesame message object willbeoverwritten.
Messages may beread based oninterrupts orbypolling.
27.8.1 Message Handler Overview
The Message Handler state machine controls thedata transfer between theRx/Tx Shift Register ofthe
CAN Core andtheMessage RAM. Itperforms thefollowing tasks:
*Data Transfer from Message RAM toCAN Core (messages tobetransmitted).
*Data Transfer from CAN Core totheMessage RAM (received messages).
*Data Transfer from CAN Core totheAcceptance Filtering unit.
*Scanning ofMessage RAM foramatching message object (acceptance filtering).
*Scanning thesame message object after being changed byIF1/IF2 registers when priority issame or
higher asmessage theobject found bylastscanning.
*Handling ofTxRqst flags.
*Handling ofinterrupt flags.
The Message Handler registers contains status flags ofallmessage objects grouped intothefollowing
topics:
*Transmission Request flags
*New Data flags
*Interrupt Pending Flags
*Message Valid Registers
Instead ofcollecting thelisted status information ofeach message object viaIFxregisters separately,
these Message Handler registers provides afastandeasy way togetanoverview (forexample, about all
pending transmission requests).
AllMessage Handler registers areread-only.
27.8.2 Receive/Transmit Priority
The receive/transmit priority forthemessage objects isattached tothemessage number, nottotheCAN
identifier. Message object 1hasthehighest priority, while thelastimplemented message object hasthe
lowest priority. Ifmore than onetransmission request ispending, they areserviced duetothepriority of
thecorresponding message object, somessages with thehighest priority canbeplaced inthemessage
objects with thelowest numbers.
The acceptance filtering forreceived Data Frames orRemote Frames isalso done inascending order of
message objects, soaframe thathasbeen accepted byamessage object cannot beaccepted byanother
message object with ahigher Message Number. The lastmessage object may beconfigured toaccept
anyData Frame orRemote Frame thatwas notaccepted byanyother message object, fornodes that
need tologthecomplete message traffic ontheCAN bus.

<!-- Page 1439 -->

www.ti.com Message Handling
1439 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.8.3 Transmission ofMessages inEvent Driven CAN Communication
Iftheshift register oftheCAN Core isready forloading andifthere isnodata transfer between theIFx
Registers andMessage RAM, thedbitsintheMessage Valid Register andtheTxRqst bitsinthe
Transmission Request Register areevaluated. The valid message object with thehighest priority pending
transmission request isloaded intotheshift register bytheMessage Handler andthetransmission is
started. The message object 'sNewDat bitisreset.
After asuccessful transmission andifnonew data was written tothemessage object (NewDat ='0')since
thestart ofthetransmission, theTxRqst bitwillbereset. IfTxIE isset,IntPnd willbesetafter asuccessful
transmission. IftheDCAN haslostthearbitration orifanerror occurred during thetransmission, the
message willberetransmitted assoon astheCAN busisfree again. Ifmeanwhile thetransmission ofa
message with higher priority hasbeen requested, themessages willbetransmitted intheorder oftheir
priority.
IfAutomatic Retransmission mode isdisabled bysetting theDAR bitintheCAN Control Register ,the
behavior ofbitsTxRqst andNewDat intheMessage Control Register oftheInterface Register setisas
follows:
*When atransmission starts, theTxRqst bitoftherespective Interface Register setisreset, while bit
NewDat remains set.
*When thetransmission hasbeen successfully completed, theNewDat bitisreset.
When atransmission failed (lost arbitration orerror) bitNewDat remains set.Torestart thetransmission,
theapplication hastosetTxRqst again.
Received Remote Frames donotrequire aReceive Object. They willautomatically trigger the
transmission ofaData Frame, ifinthematching Transmit Object theRmtEn bitisset.
27.8.4 Updating aTransmit Object
The CPU may update thedata bytes ofaTransmit Object anytime viatheIF1/IF2 Interface Registers,
neither dnorTxRqst have tobereset before theupdate.
Even ifonly apart ofthedata bytes aretobeupdated, allfour bytes inthecorresponding IF1/IF2 Data A
Register orIF1/IF2 Data BRegister have tobevalid before thecontent ofthatregister istransferred tothe
message object. Either theCPU hastowrite allfour bytes intotheIF1/IF2 Data Register orthemessage
object istransferred totheIF1/IF2 Data Register before theCPU writes thenew data bytes.
When only thedata bytes areupdated, first0x87 canbewritten tobits[23:16] oftheCommand Register
andthen thenumber ofthemessage object iswritten tobits[7:0] oftheCommand Register, concurrently
updating thedata bytes andsetting TxRqst with NewDat.
Toprevent thereset ofTxRqst attheendofatransmission thatmay already beinprogress while thedata
isupdated, NewDat hastobesettogether with TxRqst inevent driven CAN communication. Fordetails,
seeSection 27.8.3 .
When NewDat issettogether with TxRqst, NewDat willbereset assoon asthenew transmission has
started.
27.8.5 Changing aTransmit Object
Ifthenumber ofimplemented message objects isnotsufficient tobeused aspermanent message objects
only, theTransmit Objects may bemanaged dynamically. The CPU canwrite thewhole message
(Arbitration, Control, andData) intotheInterface Register. The bits[23:16] oftheCommand Register can
besetto0xB7 forthetransfer ofthewhole message object content intothemessage object. Before
changing theconfiguration ofamessage object, MsgVal hastobereset (see Section 27.7.7 ).
Ifapreviously requested transmission ofthismessage object isnotcompleted butalready inprogress, it
willbecontinued; however itwillnotberepeated ifitisdisturbed.
Toonly update thedata bytes ofamessage tobetransmitted, bits[23:16] oftheCommand Register
should besetto0x87.

<!-- Page 1440 -->

Message Handling www.ti.com
1440 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleNOTE: After theupdate oftheTransmit Object, theInterface Register setwillcontain acopy ofthe
actual contents oftheobject, including thepart thathadnotbeen updated.
27.8.6 Acceptance Filtering ofReceived Messages
When thearbitration andcontrol bits(Identifier +IDE+RTR +DLC) ofanincoming message is
completely shifted intotheshift register oftheCAN Core, theMessage Handler starts toscan ofthe
Message RAM foramatching valid message object:
*The Acceptance Filtering unitisloaded with thearbitration bitsfrom theCAN Core shift register.
*Then thearbitration andmask bits(including MsgVal, UMask, NewDat, andEoB) ofMessage Object 1
areloaded intotheAcceptance Filtering unitandarecompared with thearbitration bitsfrom theshift
register. This isrepeated forallfollowing message objects until amatching message object isfound, or
until theendoftheMessage RAM isreached.
*Ifamatch occurs, thescanning isstopped andtheMessage Handler proceeds depending onthetype
oftheframe (Data Frame orRemote Frame) received.
27.8.7 Reception ofData Frames
The Message Handler stores themessage from theCAN Core shift register intotherespective message
object intheMessage RAM. Notonly thedata bytes, butallarbitration bitsandtheData Length Code are
stored intothecorresponding message object. This ensures thatthedata bytes stay associated tothe
identifier even ifarbitration mask registers areused.
The NewDat bitissettoindicate thatnew data (not yetseen bytheCPU) hasbeen received. The CPU
should reset theNewDat bitwhen itreads themessage object. Ifatthetime ofthereception theNewDat
bitwas already set,MsgLst issettoindicate thattheprevious data (supposedly notseen bytheCPU) is
lost. IftheRxIE bitisset,theIntPnd bitisset,causing theInterrupt Register topoint tothismessage
object.
The TxRqst bitofthismessage object isreset toprevent thetransmission ofaRemote Frame, while the
requested Data Frame hasjustbeen received.
27.8.8 Reception ofRemote Frames
When aRemote Frame isreceived, three different configurations ofthematching message object have to
beconsidered:
1.Dir=1(direction =transmit), RmtEn =1,UMask =1or0:The TxRqst bitofthismessage object isset
atthereception ofamatching Remote Frame. The restofthemessage object remains unchanged.
2.Dir=1(direction =transmit), RmtEn =0,UMask =0:The Remote Frame isignored, thismessage
object remains unchanged.
3.Dir=1(direction =transmit), RmtEn =0,UMask =1:The Remote Frame istreated similar toa
received Data Frame. Atthereception ofamatching Remote Frame, theTxRqst bitofthismessage
object isreset. The arbitration andcontrol bits(Identifier +IDE+RTR +DLC) from theshift register
arestored inthemessage object intheMessage RAM andtheNewDat bitofthismessage object is
set.The data bytes ofthemessage object remain unchanged.
27.8.9 Reading Received Messages
The CPU may read areceived message anytime viatheIFxInterface Registers, thedata consistency is
guaranteed bytheMessage Handler state machine.
Typically theCPU willwrite first0x7F tobits[23:16] andthen thenumber ofthemessage object tobits
[7:0] oftheCommand Register. That combination willtransfer thewhole received message from the
Message RAM intotheInterface Register set.Additionally, thebitsNewDat andIntPnd arecleared inthe
Message RAM (not intheInterface Register set). The values ofthese bitsintheMessage Control
Register always reflect thestatus before resetting thebits.
Ifthemessage object uses masks foracceptance filtering, thearbitration bitsshow which ofthedifferent
matching messages hasbeen received.

<!-- Page 1441 -->

www.ti.com Message Handling
1441 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleThe actual value ofNewDat shows whether anew message hasbeen received since lasttime when this
message object was read. The actual value ofMsgLst shows whether more than onemessage have been
received since thelasttime when thismessage object was read. MsgLst willnotbeautomatically reset.
27.8.10 Requesting New Data foraReceive Object
Bymeans ofaRemote Frame, theCPU may request another CAN node toprovide new data forareceive
object. Setting theTxRqst bitofareceive object willcause thetransmission ofaRemote Frame with the
receive object 'sidentifier. This Remote Frame triggers theother CAN node tostart thetransmission ofthe
matching Data Frame. Ifthematching Data Frame isreceived before theRemote Frame could be
transmitted, theTxRqst bitisautomatically reset.
Setting theTxRqst bitwithout changing thecontents ofamessage object requires thevalue 0x84 inbits
[23:16] oftheCommand Register.
27.8.11 Storing Received Messages inFIFO Buffers
Several message objects may begrouped toform oneormore FIFO Buffers. Each FIFO Buffer configured
tostore received messages with aparticular (group of)Identifier(s). Arbitration andMask Registers ofthe
FIFO Buffer 'smessage objects areidentical. The EoB (End ofBuffer) bitsofallbutthelastoftheFIFO
Buffer 'smessage objects are'0',inthelastonetheEoB bitis1.
Received messages with identifiers matching toaFIFO Buffer arestored intoamessage object ofthis
FIFO Buffer, starting with themessage object with thelowest message number.
When amessage isstored intoamessage object ofaFIFO Buffer theNewDat bitofthismessage object
isset.Bysetting NewDat while EoB is0themessage object islocked forfurther write accesses bythe
Message Handler until theCPU hascleared theNewDat bit.
Messages arestored intoaFIFO Buffer until thelastmessage object ofthisFIFO Buffer isreached. If
none ofthepreceding message objects isreleased bywriting NewDat to0,allfurther messages forthis
FIFO Buffer willbewritten intothelastmessage object oftheFIFO Buffer (EoB =1)andtherefore
overwrite previous messages inthismessage object.
27.8.12 Reading from aFIFO Buffer
Several messages may beaccumulated inasetofmessage objects thatareconcatenated toform aFIFO
Buffer before theapplication program isrequired (inorder toavoid theloss ofdata) toempty thebuffer.
AFIFO Buffer oflength Nwillstore N-1plus thelastreceived message since lasttime itwas cleared.
AFIFO Buffer iscleared byreading andresetting theNewDat bitsofallitsmessage objects, starting at
theFIFO Object with thelowest message number. This should bedone inasubroutine following the
example shown inFigure 27-12 .
NOTE: Allmessage objects ofaFIFO buffer needs toberead andcleared before thenext batch of
messages canbestored. Otherwise true FIFO functionality cannotbeguaranteed, since the
message objects ofapartly read buffer willbere-filled according tothenormal (descending)
priority.
Reading from aFIFO Buffer message object andresetting itsNewDat bitishandled thesame way as
reading from asingle message object.

<!-- Page 1442 -->

Read interrupt identifierSTART
case interrupt identifier
0x8000 else                          0x0000
Status ChangeEND
IFx command register [31:16] = 0x007F
Write Message Number to IF1/IF2 command register
(Transfer message to IF1/IF2 registers,
clear NewDat and IntPnd)
Read IF1/IF2 message control
NewDat = 1
Read data from IF1/IF2 Data A,B
EoB = 1
Next Message Number in this FIFO BufferYesNo
Yes
NoMessage interrupt
Interrupt Handling
Message Number = interrupt identifier
Message Handling www.ti.com
1442 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleFigure 27-12. CPU Handling ofaFIFO Buffer (Interrupt Driven)

<!-- Page 1443 -->

www.ti.com CAN Message Transfer
1443 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.9 CAN Message Transfer
Once theDCAN isinitialized andInitbitisreset tozero, theCAN Core synchronizes itself totheCAN bus
andisready formessage transfer aspertheconfigured message objects.
The CPU may enable theinterrupt lines (setting IE0andIE1to1)atthesame time when itclears Initand
CCE. The status interrupts EIEandSIEmay beenabled simultaneously.
The CAN communication canbecarried outinanyofthefollowing twomodes:
1.Interrupt mode
2.Polling mode.
The Interrupt Register points tothose message objects with IntPnd =1.Itisupdated even iftheinterrupt
lines totheCPU aredisabled (IE0 /IE1arezero).
The CPU may pollallMessage Object 'sNewDat andTxRqst bitsinparallel from theNewData X
Registers andtheTransmission Request XRegisters. Polling canbemade easier ifallTransmit Objects
aregrouped atthelownumbers, allReceive Objects aregrouped atthehigh numbers.
Received messages arestored intotheir appropriate message objects ifthey pass acceptance filtering.
The whole message (including allarbitration bits, DLC anduptoeight data bytes) isstored intothe
message object. Asaconsequence, when theidentifier mask isused, thearbitration bitsthataremasked
to"don'tcare"may change inthemessage object when areceived message isstored.
The CPU may read orwrite each message atanytime viatheInterface Registers, astheMessage
Handler guarantees data consistency incase ofconcurrent accesses (forreconfiguration, see
Section 27.7.6 )
Ifapermanent message object (arbitration andcontrol bitssetupduring configuration andleaving
unchanged formultiple CAN transfers) exists forthemessage, itispossible toonly update thedata bytes.
Ifseveral transmit messages should beassigned toonemessage object, thewhole message object has
tobeconfigured before thetransmission ofthismessage isrequested.
The transmission ofmultiple message objects may berequested atthesame time. They aresubsequently
transmitted, according totheir internal priority.
Messages may beupdated orsettonotvalid atanytime, even ifarequested transmission isstillpending
(forreconfiguration, seeSection 27.7.7 ).However, thedata bytes willbediscarded ifamessage is
updated before apending transmission hasstarted.
Depending ontheconfiguration ofthemessage object, atransmission may beautomatically requested by
thereception ofaremote frame with amatching identifier.
27.9.1 Automatic Retransmission
According totheCAN Specification (ISO11898), theDCAN provides amechanism toautomatically
retransmit frames thathave lostarbitration orhave been disturbed byerrors during transmission. The
frame transmission service willnotbeconfirmed toyoubefore thetransmission issuccessfully completed.
Bydefault, thisautomatic retransmission isenabled. Itcanbedisabled bysetting bitDAR (Disable
Automatic Retransmission) inCAN Control Register. Further details tothismode areprovided in
Section 27.8.3 .

<!-- Page 1444 -->

CAN Message Transfer www.ti.com
1444 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.9.2 Auto-Bus-On
Perdefault, after theDCAN hasentered Bus-Off state, theCPU canstart aBus-Off-Recovery sequence
byresetting Initbit.Ifthisisnotdone, themodule willstay inBus-Off state.
The DCAN provides anautomatic Auto-Bus-On feature thatisenabled bybitABO inCAN Control
Register. Ifset,theDCAN willautomatically start theBus-Off-Recovery sequence. The sequence canbe
delayed byauser-defined number ofVCLK cycles thatcanbedefined inAuto-Bus-On Time Register.
NOTE: IftheDCAN goes Bus-Off duetomassive occurrence ofCAN buserrors, itstops allbus
activities andautomatically sets theInitbit.Once theInitbithasbeen reset bytheCPU or
duetotheAuto-Bus-On feature, thedevice willwait for129occurrences ofBus Idle(equal to
129×11consecutive recessive bits) before resuming normal operation. Attheendofthe
Bus-Off recovery sequence, theerror counters willbereset.
27.10 Interrupt Functionality
Interrupts canbegenerated ontwointerrupt lines:
1.DCAN0INT line
2.DCAN1INT line
These lines canbeenabled bysetting theIE0andIE1bits, respectively, intheCAN Control Register.
The DCAN provides three groups ofinterrupt sources: Message Object Interrupts, Status Change
Interrupts andError Interrupts (see Figure 27-13 andFigure 27-14 ).
The source ofaninterrupt canbedetermined bytheinterrupt identifiers Int0ID /Int1ID intheInterrupt
Register (see Section 27.17.5 ).When nointerrupt ispending, theregister willhold thevalue zero.
Each interrupt lineremains active until thededicated field intheInterrupt Register DCAN INT(Int0ID /
Int1ID) again reach zero (this means thecause oftheinterrupt isreset), oruntil IE0/IE1arereset.
The value 0x8000 intheInt0ID field indicates thataninterrupt ispending because theCAN Core has
updated (not necessarily changed) theError andStatus Register (Error Interrupt orStatus Interrupt). This
interrupt hasthehighest priority. The CPU canupdate (reset) thestatus bitsWakeUpPnd, RxOk, TxOk
andLEC byreading theError andStatus Register DCAN ES,butawrite access oftheCPU willnever
generate orreset aninterrupt.
Values between 1andthenumber ofthelastmessage object indicates thatthesource oftheinterrupt is
oneofthemessage objects, Int0ID resp. Int1ID willpoint tothepending message interrupt with the
highest priority. The Message Object 1hasthehighest priority, thelastmessage object hasthelowest
priority.
Aninterrupt service routine thatreads themessage thatisthesource oftheinterrupt, may read the
message andreset themessage object 'sIntPnd atthesame time (ClrIntPnd bitintheIF1/IF2 Command
Register). When IntPnd iscleared, theInterrupt Register willpoint tothenext message object with a
pending interrupt.
27.10.1 Message Object Interrupts
Message Object interrupts aregenerated byevents from themessage objects. They arecontrolled bythe
flags IntPND, TxIE andRxIE, thataredescribed inSection 27.5.1 .
Message Object interrupts canberouted toeither DCAN0INT orDCAN1INT line, controlled bythe
Interrupt Multiplexer Register (see Section 27.17.22 ).

<!-- Page 1445 -->

Message
Object 1
Message
ObjectLastReceive OK
Transmit OK
Receive OK
Transmit OKMessage
Object
Interrupt
Bus Off
Error
Single/DoubleWarning
Bit ErrorWakeUpPndLECRX OK
TX OK
EIESIEMessage Object Interrupts Status Change Interrupts
Error InterruptsIE0
DCAN0INTError and Status Change
Interrupts are Routed to
DCAN0INT line
www.ti.com Interrupt Functionality
1445 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.10.2 Status Change Interrupts
The events WakeUpPnd, RxOk, TxOk andLEC inError andStatus Register (DCAN ES)belong tothe
Status Change Interrupts. The Status Change Interrupt group canbeenabled bybitinCAN Control
Register.
IfSIEisset,aStatus Change Interrupt willbegenerated ateach CAN frame, independent ofbuserrors or
valid CAN communication, andalso independent oftheMessage RAM configuration.
Status Change interrupts canonly berouted tointerrupt lineDCAN0INT thathastobeenabled bysetting
IE0intheCAN Control Register.
NOTE: Reading theError andStatus Register willclear theWakeUpPnd flag. Ifinglobal power
down mode, theWakeUpPnd flagiscleared bysuch aread access before theDCAN module
hasbeen waken upbythesystem, theDCAN may re-assert theWakeUpPnd flag, anda
second interrupt may occur (additional information canbefound inSection 27.11.2 ).
27.10.3 Error Interrupts
The events PER, BOff andEWarn (monitored inError andStatus Register, DCAN ES)belong totheError
Interrupts. The Error Interrupt group canbeenabled bysetting bitEIEinCAN Control Register.
Error interrupts canonly berouted tointerrupt lineDCAN0INT thathastobeenabled bysetting IE0inthe
CAN Control Register.
Figure 27-13. CAN Interrupt Topology 1

<!-- Page 1446 -->

Message
Object 1
Message
ObjectLastReceive OK
Transmit OK
Receive OK
Transmit OKMessage Object Interrupts
IE0
DCAN0INTIE1
DCAN1INT
Message Object Interrupts
can be Routed to
DCAN0INT or DCAN1INT LineIntPndMux(n)IntPndMux(1)
To Status InterruptRxIE
TxIE
RxIE
TxIE
Global Power Down Mode www.ti.com
1446 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleFigure 27-14. CAN Interrupt Topology 2
Details ofInterrupt Mapping foractual device willbedescribed inthedevice specific data sheet.
27.11 Global Power Down Mode
The device architecture supports acentralized global power down control over theperipheral modules
through thePeripheral Central Resource (PCR) module (Additional information canbefound inPlatform
Architecture Specification).
27.11.1 Entering Global Power Down Mode
The global power down mode fortheDCAN isrequested bysetting theappropriate Peripheral Power
Down Setbit(PSPWRDWNSETx )inthePCR module.
The DCAN then finishes alltransmit requests ofthemessage objects. When allrequests aredone, the
DCAN waits until abusidlestate isrecognized. Then itwillautomatically settheInitbit toindicate thatthe
global power down mode hasbeen entered.
27.11.2 Wakeup From Global Power Down Mode
When theDCAN module isinglobal power down mode, aCAN busactivity detection circuit exists, which
canbeactive, ifenabled. Ifthiscircuit isactive,on occurrence ofadominant CAN buslevel, theDCAN will
settheWakeUpPnd bitinError andStatus Register (DCAN ES).
IfStatus Interrupts areenabled, also aninterrupt willbegenerated. This interrupt could beused bythe
application towakeup theDCAN. Forthis, theapplication needs tosettheappropriate Peripheral Power
Down Clear bit(PSPWRDWNCLRx )inthePCR module, andtoclear theInitbitinCAN Control Register.
After theInitbithasbeen cleared, theDCAN module waits until itdetects 11consecutive recessive bitson
theCAN_RX pinandthen goes Bus-Active again.
NOTE: The CAN transceiver circuit hastostay active during CAN busactivity detection. The first
CAN message, which initiates thebusactivity, cannot bereceived. This means thatthefirst
message received inpower down mode islost.

<!-- Page 1447 -->

www.ti.com Local Power Down Mode
1447 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.12 Local Power Down Mode
Besides thecentralized power down mechanism controlled bythePCR module (global power down, see
Section 27.15 ),theDCAN supports alocal power down mode thatcanbecontrolled within theDCAN
control registers.
27.12.1 Entering Local Power Down Mode
The local power down mode isrequested bysetting thePDR bitinCAN Control Register.
The DCAN then finishes alltransmit requests ofthemessage objects. When allrequests aredone, DCAN
waits until abusidlestate isrecognized. Then itwillautomatically settheInitbit inCAN Control Register to
prevent anyfurther CAN transfers, anditwillalso setthePDA bitinCAN Error andStatus Register. With
setting thePDA bits, theDCAN module indicates thatthelocal power down mode hasbeen entered.
During local power down mode, theinternal clocks oftheDCAN module areturned off,butthere isawake
uplogic (see Section 27.12.2 )thatcanbeactive, ifenabled. Also theactual contents ofthecontrol
registers canberead back.
NOTE: Inlocal lowpower mode, theapplication should notclear theInitbitwhile PDR isset.Ifthere
areanymessages intheMessage RAM configured astobetransmitted andtheapplication
resets theinitbit,these messages may besent.
27.12.2 Wakeup From Local Power Down
There aretwoways towake uptheDCAN from local power down mode:
1.The application could wake uptheDCAN module manually byclearing thePDR bitandthen clearing
theInitbitinCAN Control Register.
2.Alternatively, aCAN busactivity detection circuit canbeactivated bysetting thewake uponbus
activity bit(WUBA) inCAN Control Register. Ifthiscircuit isactive, onoccurrence ofadominant CAN
buslevel, theDCAN willautomatically start thewake upsequence. Itwillclear thePDR bitinCAN
Control Register andalso clear thePDA bitinError andStatus Register. The WakeUpPnd bitinCAN
Error andStatus Register willbeset.IfStatus Interrupts areenabled, also aninterrupt willbe
generated. Finally theInitbitinCAN control register willbecleared.
After theInitbithasbeen cleared, themodule waits until itdetects 11consecutive recessive bitsonthe
CAN_RX pinandthen goes Bus-Active again.
NOTE: The CAN transceiver circuit hastostay active while CAN busobservation. The firstCAN
message, which initiates thebusactivity, cannot bereceived. This means thatthefirst
message received inpower down andautomatic wake-up mode, islost.
Figure 27-15 shows aflow diagram about entering andleaving local power down mode.
27.13 GIOSupport
The CAN_RX andCAN_TX pins ofeach DCAN module canbeused asgeneral purpose IOpins, ifCAN
functionality isnotneeded. This function iscontrolled bytheCAN TXIOControl register (see
Section 27.17.34 )andtheCAN RXIOControl register (see Section 27.17.35 ).

<!-- Page 1448 -->

Application: set PDR = 1
Handle all open tx_requests,
wait until bus_idle
D_CAN:
set Init bit = 1
set PDA bit = 1
Local power down mode state
Application: set PDR = 0
D_CAN:
set PDA bit = 0
Application: set Init = 0CAN bus activity
WUBA bit?
D_CAN:
set PDA bit = 0
set PDR bit = 0
set WakeUpPnd bit = 1
(CAN_INTR = 1, if enabled)
set Init bit = 0
Wait for 11 recessive bits
END1
GIO Support www.ti.com
1448 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleFigure 27-15. Local Power Down Mode Flow Diagram

<!-- Page 1449 -->

CAN_TX CAN_RX
Tx Rx
CAN Core=1DCAN
www.ti.com Test Modes
1449 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.14 Test Modes
The DCAN provides several testmodes thataremainly intended forproduction tests orselftest.
Foralltestmodes, Test bitintheCAN Control Register needs tobesettoone. This enables write access
totheTest Register.
NOTE: When using anyoftheLoop Back modes, itmust beensured bysoftware thatallmessage
transfers arefinished before setting theInitbitto'1'.
27.14.1 Silent Mode
The Silent Mode may beused toanalyze thetraffic ontheCAN buswithout affecting itbysending
dominant bits(forexample, acknowledge bit,overload flag, active error flag). The DCAN isstillable to
receive valid data frames andvalid remote frames, butitwillnotsend anydominant bits. However, these
areinternally routed totheCAN Core.
Figure 27-16 shows theconnection ofsignals CAN_TX andCAN_RX totheCAN Core inSilent Mode.
Silent Mode canbeactivated bysetting theSilent bitinTest Register to1.
InISO 11898-1, theSilent Mode iscalled theBus Monitoring Mode.
Figure 27-16. CAN Core inSilent Mode

<!-- Page 1450 -->

Tx Rx
CAN CoreDCANCAN_RX CAN_TX
Test Modes www.ti.com
1450 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.14.2 Loop Back Mode
The Loop Back Mode ismainly intended forhardware self-test functions. Inthismode, theCAN Core uses
internal feedback from Txoutput toRxinput. Transmitted messages aretreated asreceived messages,
andcanbestored intomessage objects ifthey pass acceptance filtering. The actual value oftheCAN_RX
input pinisdisregarded bytheCAN Core. Transmitted messages stillcanbemonitored attheCAN_TX
pin.
Inorder tobeindependent from external stimulation, theCAN Core ignores acknowledge errors (recessive
bitsampled intheacknowledge slotofadata/remote frame) inLoop Back Mode.
Figure 27-17 shows theconnection ofsignals CAN_TX andCAN_RX totheCAN Core inLoop Back
Mode.
Loop Back Mode canbeactivated bysetting bitLBack inTest Register to1.
NOTE: InLoop Back mode, thesignal path from CAN Core toTxpin,theTxpinitself, andthesignal
path from Txpinback toCAN Core aredisregarded. Forincluding these intothetesting, see
External Loop Back mode (Section 27.14.3 ).
Figure 27-17. CAN Core inLoop Back Mode

<!-- Page 1451 -->

DCANCAN_TX
CAN CoreRx TxpinCAN_RX
pin
www.ti.com Test Modes
1451 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.14.3 External Loop Back Mode
The External Loop Back Mode issimilar totheLoop Back Mode, however itincludes thesignal path from
CAN Core toTxpin,theTxpinitself, andthesignal path from Txpinback toCAN Core. When External
Loop Back Mode isselected, theinput oftheCAN core isconnected totheinput buffer oftheTxpin.
With thisconfiguration, theTxpinIOcircuit canbetested.
External Loop Back Mode canbeactivated bysetting bitExL inTest Register to1.
Figure 27-18 shows theconnection ofsignals CAN_TX andCAN_RX totheCAN Core inExternal Loop
Back Mode.
NOTE: When Loop Back Mode isactive (LBack bitset), theExL bitwillbeignored.
Figure 27-18. CAN Core inExternal Loop Back Mode

<!-- Page 1452 -->

Tx Rx
CAN Core=1DCANCAN_RX CAN_TX
Test Modes www.ti.com
1452 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.14.4 Loop Back Combined with Silent Mode
Itisalso possible tocombine Loop Back Mode andSilent Mode bysetting bitsLBack andSilent atthe
same time. This mode canbeused fora"HotSelftest ",thatis,theDCAN hardware canbetested without
affecting theCAN network. Inthismode, theCAN_RX pinisdisconnected from theCAN Core andno
dominant bitswillbesent ontheCAN_TX pin.
Figure 27-19 shows theconnection ofthesignals CAN_TX andCAN_RX totheCAN Core incase ofthe
combination ofLoop Back Mode with Silent Mode.
Figure 27-19. CAN Core inLoop Back Combined with Silent Mode
27.14.5 Software Control ofCAN_TX Pin
Four output functions areavailable fortheCAN transmit pinCAN_TX. Additionally toitsdefault function
(serial data output), theCAN_TX pincandrive constant dominant orrecessive values, oritcandrive the
CAN Sample Point signal tomonitor theCAN Core 'sbittiming.
Combined with thereadable value oftheCAN_RX pin,thiscanbeused tocheck thephysical layer ofthe
CAN bus.
The output mode ofpinCAN_TX isselected byprogramming theTest Register bitsTx[1:0] asdescribed
inSection 27.17.6 .
NOTE: The software control forpinCAN_TX interferes with CAN protocol functions. ForCAN
message transfer oranyofthetestmodes Loop Back Mode, External Loop Back Mode or
Silent Mode, theCAN_TX pinshould operate initsdefault functionality.

<!-- Page 1453 -->

www.ti.com SECDED Mechanism
1453 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.15 SECDED Mechanism
The DCAN module provides asingle-bit error correction anddouble-bit error detection (SECDED)
mechanism toensure data integrity ofMessage RAM data. Foreach message object (136 bits) inthe
Message RAM, 9ECC bitswillbecalculated. See Section 27.5.5 .
The ECC bitsarestored inadedicated RAM. They willbegenerated onwrite accesses andwillbe
checked onread accesses.
The SECDED functionality canbeenabled ordisabled byPMD bitfield inCAN Control Register. If
SECDED isenabled, ECC bitswillbeautomatically generated andchecked.
With theECCMODE field intheECC Control andStatus register thesingle-bit error correction canbe
enabled ordisabled (default: enabled).
NOTE: During RAM initialization, noECC check willbedone, butifthePMD bitisset,theECC bits
willbegenerated.
27.15.1 Behavior onSingle-Bit Error
Ifasingle-bit error isdetected with single-bit error correction enabled, thecorrection willbedone andthe
SEFLG intheECC Control andStatus register willbeset.
Ifsingle-bit error correction isdisabled andasingle-bit error isdetected then theSEFLG intheECC
Control andStatus register andthethePER bitintheError andStatus register willbeset.Iferror
interrupts areenabled, also aninterrupt would begenerated. Inorder toavoid thetransmission ofinvalid
data over theCAN bus, theMsgVal bitofthemessage object willbereset.
The message object number where thesingle-bit error hasoccurred willbeindicated intheECC single-bit
Error Code Register.
When single-bit error correction isdisabled themessage object data canberead bythehost CPU,
independently ofsingle-bit errors. Thus, theapplication hastoensure thattheread data isvalid, for
example, byimmediately checking theECC single-bit Error Code Register onsingle-bit error interrupt.
27.15.2 Behavior onDouble-Bit Error
Ifadouble-bit error isdetected, then theDEFLG intheECC Control andStatus register andthePER bit
inError andStatus Register willbeset.Iferror interrupts areenabled, also aninterrupt would be
generated. Inorder toavoid thetransmission ofinvalid data over theCAN bus, theMsgVal bitofthe
message object willbereset. The message object number willbeindicated intheParity Error Code
Register.
The message object data canberead bythehost CPU, independently ofdouble-bit errors. Thus, the
application hastoensure thattheread data isvalid, forexample, byimmediately checking theParity Error
Code register ondouble-bit error interrupt.
27.15.3 SECDED Testing
Testing oftheSECDED mechanism canbeimplemented byusing thediagnostic mode, which isenabled
with theECCDIAG register. The following procedure canbeused:
1.Disable SECDED using DCAN control register. Enable diagnostic mode using theECCDIAG register
2.Write tocorrupt thedata (inRDA mode) orECC bits.
3.Enable SECDED andread data forwhich ECC iscorrupted (either inRDA mode orviaIFxregisters).
4.single-bit error ordouble-bit error flagwillbesetinthediagnostic status register (ECCDIAG STAT) and
intheECC Control andStatus register accordingly. Adouble-bit error orasingle-bit error with single-bit
error correction disabled also triggers thePER flag.
5.Disable diagnostic mode.

<!-- Page 1454 -->

Debug/Suspend Mode www.ti.com
1454 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.16 Debug/Suspend Mode
When theCPU ishalted during debug, allDCAN registers arevisible andcanbeinspected andmodified
bytheCPU.
Inaddition, theMessage RAM isdirectly memory-mapped asdescribed inTable 27-3.
The CAN controller provides twooptions forentering thedebug/suspend state. The options arecontrolled
bytheIDSbitintheCAN Control Register (DCAN CTL). Bydefault, when IDSis0,theDCAN controller
completes anyactive transfers ontheCAN busandwaits until thebusisidlebefore halting. When IDSis
1,theDCAN halts immediately assoon astheCPU ishalted.
The InitDbg bitinDCAN CTL register indicates when theDCAN controller hasactually entered the
debug/suspend state.
NOTE: During Debug/Suspend Mode, theMessage RAM cannot beaccessed viatheIFxregister
sets.
Writing tocontrol registers indebug/suspend mode may influence theCAN state machine
andfurther message handling.
Fordebug support, theauto clear functionality ofthefollowing DCAN registers isdisabled:
*Error andStatus Register (clear ofstatus flags byread)
*IF1/IF2 Command Registers (clear ofDMAActive flagbyread/write)
27.17 DCAN Control Registers
Table 27-6 lists thecontrol registers oftheDCAN. After hardware reset, theregisters oftheDCAN hold
thevalues shown intheregister descriptions. The base address forthecontrol registers isFFF7 DC00h
forDCAN1, FFF7 DE00h forDCAN2, FFF7 E000h forDCAN3, andFFF7 E200h forDCAN4.
Additionally, theBus-Off state isreset andtheCAN_TX pinissettorecessive (HIGH). The Initbitinthe
CAN Control Register issettoenable thesoftware initialization. The DCAN willnotinfluence theCAN bus
until theCPU resets Initto0.
Table 27-6. DCAN Control Registers
Offset Acronym Register Description Section
00h DCAN CTL CAN Control Register Section 27.17.1
04h DCAN ES Error andStatus Register Section 27.17.2
08h DCAN ERRC Error Counter Register Section 27.17.3
0Ch DCAN BTR BitTiming Register Section 27.17.4
10h DCAN INT Interrupt Register Section 27.17.5
14h DCAN TEST Test Register Section 27.17.6
1Ch DCAN PERR Parity Error Code Register Section 27.17.7
20h DCAN REL Core Release Register Section 27.17.8
24h DCAN ECCDIAG ECC Diagnostic Register Section 27.17.9
28h DCAN ECCDIAG STAT ECC Diagnostic Status Register Section 27.17.10
2Ch DCAN ECC CS ECC Control andStatus Register Section 27.17.11
30h DCAN ECC SERR ECC Single-Bit Error Code Register Section 27.17.12
80h DCAN ABOTR Auto-Bus-On Time Register Section 27.17.13
84h DCAN TXRQX Transmission Request XRegister Section 27.17.14
88h DCAN TXRQ12 Transmission Request 12Register Section 27.17.15
8Ch DCAN TXRQ34 Transmission Request 34Register Section 27.17.15
90h DCAN TXRQ56 Transmission Request 56Register Section 27.17.15
94h DCAN TXRQ78 Transmission Request 78Register Section 27.17.15
98h DCAN NWDATX New Data XRegister Section 27.17.16

<!-- Page 1455 -->

www.ti.com DCAN Control Registers
1455 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-6. DCAN Control Registers (continued)
Offset Acronym Register Description Section
9Ch DCAN NWDAT12 New Data 12Register Section 27.17.17
A0h DCAN NWDAT34 New Data 34Register Section 27.17.17
A4h DCAN NWDAT56 New Data 56Register Section 27.17.17
A8h DCAN NWDAT78 New Data 78Register Section 27.17.17
ACh DCAN INTPNDX Interrupt Pending XRegister Section 27.17.18
B0h DCAN INTPND12 Interrupt Pending 12Register Section 27.17.19
B4h DCAN INTPND34 Interrupt Pending 34Register Section 27.17.19
B8h DCAN INTPND56 Interrupt Pending 56Register Section 27.17.19
BCh DCAN INTPND78 Interrupt Pending 78Register Section 27.17.19
C0h DCAN MSGVALX Message Valid XRegister Section 27.17.20
C4h DCAN MSGVAL12 Message Valid 12Register Section 27.17.21
C8h DCAN MSGVAL34 Message Valid 34Register Section 27.17.21
CCh DCAN MSGVAL56 Message Valid 56Register Section 27.17.21
D0h DCAN MSGVAL78 Message Valid 78Register Section 27.17.21
D8h DCAN INTMUX12 Interrupt Multiplexer 12Register Section 27.17.22
DCh DCAN INTMUX34 Interrupt Multiplexer 34Register Section 27.17.22
E0h DCAN INTMUX56 Interrupt Multiplexer 56Register Section 27.17.22
E4h DCAN INTMUX78 Interrupt Multiplexer 78Register Section 27.17.22
100h DCAN IF1CMD IF1Command Register Section 27.17.23
104h DCAN IF1MSK IF1Mask Register Section 27.17.24
108h DCAN IF1ARB IF1Arbitration Register Section 27.17.25
10Ch DCAN IF1MCTL IF1Message Control Register Section 27.17.26
110h DCAN IF1DATA IF1Data ARegister Section 27.17.27
114h DCAN IF1DATB IF1Data BRegister Section 27.17.27
120h DCAN IF2CMD IF2Command Register Section 27.17.23
124h DCAN IF2MSK IF2Mask Register Section 27.17.24
128h DCAN IF2ARB IF2Arbitration Register Section 27.17.25
12Ch DCAN IF2MCTL IF2Message Control Register Section 27.17.26
130h DCAN IF2DATA IF2Data ARegister Section 27.17.27
134h DCAN IF2DATB IF2Data BRegister Section 27.17.27
140h DCAN IF3OBS IF3Observation Register Section 27.17.28
144h DCAN IF3MSK IF3Mask Register Section 27.17.29
148h DCAN IF3ARB IF3Arbitration Register Section 27.17.30
14Ch DCAN IF3MCTL IF3Message Control Register Section 27.17.31
150h DCAN IF3DATA IF3Data ARegister Section 27.17.32
154h DCAN IF3DATB IF3Data BRegister Section 27.17.32
160h DCAN IF3UPD12 IF3Update Enable 12Register Section 27.17.33
164h DCAN IF3UPD34 IF3Update Enable 34Register Section 27.17.33
168h DCAN IF3UPD56 IF3Update Enable 56Register Section 27.17.33
16Ch DCAN IF3UPD78 IF3Update Enable 78Register Section 27.17.33
1E0h DCAN TIOC CAN TXIOControl Register Section 27.17.34
1E4h DCAN RIOC CAN RXIOControl Register Section 27.17.35

<!-- Page 1456 -->

DCAN Control Registers www.ti.com
1456 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.1 CAN Control Register (DCAN CTL)
NOTE: The Bus-Off recovery sequence (see CAN specification) cannot beshortened bysetting or
resetting Initbit.Ifthemodule goes Bus-Off, itwillautomatically settheInitbitandstop all
busactivities.
When theInitbitiscleared bytheapplication again, themodule willthen wait for129
occurrences ofBus Idle(129 ×11consecutive recessive bits) before resuming normal
operation. AttheendoftheBus-Off recovery sequence, theerror counters willbereset.
After theInitbitisreset, each time when asequence of11recessive bitsismonitored, aBit0
error code iswritten totheError andStatus Register, enabling theCPU tocheck whether the
CAN busisstuck atdominant orcontinuously disturbed, andtomonitor theproceeding ofthe
Bus-Off recovery sequence.
Figure 27-20. CAN Control Register (DCAN CTL) [offset =00h]
31 26 25 24
Reserved WUBA PDR
R-0 R/W-0 R/W-0
23 21 20 19 18 17 16
Reserved DE3 DE2 DE1 IE1 InitDbg
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
15 14 13 10 9 8
SWR Reserved PMD ABO IDS
R/WP-0 R-0 R/W-5h R/W-0 R/W-0
7 6 5 4 3 2 1 0
Test CCE DAR Reserved EIE SIE IE0 Init
R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0 R/W-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write protected byInitbit;-n=value after reset
Table 27-7. CAN Control Register (DCAN CTL) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 These bitsarealways read as0.Writes have noeffect.
25 WUBA Automatic wake uponbusactivity when inlocal power down mode.
0 Nodetection ofadominant CAN buslevel while inlocal power down mode.
1 Detection ofadominant CAN buslevel while inlocal power down mode isenabled. On
occurrence ofadominant CAN buslevel, thewake upsequence isstarted. (Additional
information canbefound inSection 27.12 .)
Note: The CAN message, which Initiates thebusactivity, cannot bereceived. This means that
thefirstmessage received inpower down andautomatic wake-up mode, willbelost.
24 PDR Request forlocal lowpower down mode.
0 Noapplication request forlocal lowpower down mode. Iftheapplication hascleared thisbit
while DCAN inlocal power down mode, also theInitbithastobecleared.
1 Local power down mode hasbeen requested byapplication. The DCAN willacknowledge the
local power down mode bysetting bitPDA inError andStatus Register. The local clocks willbe
turned offbyDCAN internal logic (Additional information canbefound inSection 27.12 ).
23-21 Reserved 0 These bitsarealways read as0.Writes have noeffect.
20 DE3 Enable DMA request lineforIF3.
0 Disabled
1 Enabled
Note: Apending DMA request forIF3remains active until firstaccess tooneoftheIF3
registers.

<!-- Page 1457 -->

www.ti.com DCAN Control Registers
1457 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-7. CAN Control Register (DCAN CTL) Field Descriptions (continued)
Bit Field Value Description
19 DE2 Enable DMA request lineforIF2.
0 Disabled
1 Enabled
Note: Apending DMA request forIF2remains active until firstaccess tooneoftheIF2
registers.
18 DE1 Enable DMA request lineforIF1.
0 Disabled
1 Enabled
Note: Apending DMA request forIF1remains active until firstaccess tooneoftheIF1
registers.
17 IE1 Interrupt line1enable.
0 Disabled. Module Interrupt DCAN1INT isalways low.
1 Enabled. Interrupts willassert lineDCAN1INT toone; lineremains active until pending
interrupts areprocessed.
16 InitDbg Internal Initstate while debug access.
0 Notindebug mode, ordebug mode requested butisnotentered.
1 Debug mode requested andisinternally entered; theDCAN isready fordebug accesses.
15 SWR SWreset enable.
0 Normal operation.
1 Module isforced toreset state. This bitwillautomatically getcleared after execution ofSW
reset after oneVBUSP clock cycle.
Note: Toexecute SWreset thefollowing procedure isnecessary:
1.SetInitbittoshut down CAN communication.
2.SetSWR bitadditionally toInitbit.
14 Reserved 0 This bitisalways read as0.Writes have noeffect.
13-10 PMD SECDED enable.
5h SECDED function isdisabled.
Allother
valuesSECDED function isenabled.
9 ABO Auto-Bus-On enable.
0 The Auto-Bus-On feature isdisabled.
1 The Auto-Bus-On feature isenabled.
8 IDS Interruption debug support enable.
0 When Debug/Suspend mode isrequested, DCAN willwait forastarted transmission or
reception tobecompleted before entering Debug/Suspend mode.
1 When Debug/Suspend mode isrequested, DCAN willinterrupt anytransmission orreception,
andenter Debug/Suspend mode immediately.
7 Test Test mode enable.
0 Normal operation.
1 Test mode.
6 CCE Configuration change enable.
0 The CPU hasnowrite access totheBTR Config register.
1 The CPU haswrite access totheBTR configuration register (when Initbitisset).
5 DAR Disable automatic retransmission.
0 Automatic Retransmission ofnotsuccessful messages isenabled.
1 Automatic Retransmission isdisabled.
4 Reserved 0 This bitisalways read as0.Writes have noeffect.

<!-- Page 1458 -->

DCAN Control Registers www.ti.com
1458 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-7. CAN Control Register (DCAN CTL) Field Descriptions (continued)
Bit Field Value Description
3 EIE Error interrupt enable.
0 Disabled. PER, BOff, andEWarn bitscannot generate aninterrupt.
1 Enabled. PER, BOff, andEWarn bitscangenerate aninterrupt atDCAN0INT lineandaffect the
Interrupt Register.
2 SIE Status change interrupt enable.
0 Disabled. WakeUpPnd, RxOk, TxOk, andLEC bitscannot generate aninterrupt.
1 Enabled. WakeUpPnd, RxOk, TxOk, andLEC cangenerate aninterrupt atDCAN0INT lineand
affect theInterrupt Register.
1 IE0 Interrupt line0enable.
0 Disabled. Module Interrupt DCAN0INT isalways low.
1 Enabled. Interrupts willassert lineDCAN0INT toone; lineremains active until pending
interrupts areprocessed.
0 Init Initialization
0 Normal operation.
1 Initialization mode isentered.

<!-- Page 1459 -->

www.ti.com DCAN Control Registers
1459 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.2 Error andStatus Register (DCAN ES)
Interrupts aregenerated bybitsPER, BOff, andEWarn (ifEIEbitinCAN Control Register isset)andby
bitsWakeUpPnd, RxOk, TxOk, andLEC (ifSIEbitinCAN Control Register isset). Achange ofbitEPass
willnotgenerate anInterrupt.
NOTE: Reading theError andStatus Register clears theWakeUpPnd, PER, RxOk andTxOk bits
andsettheLEC tovalue of7.Additionally, theStatus Interrupt value (8000h) intheInterrupt
Register willbereplaced bythenext lower priority interrupt value.
Fordebug support, theauto clear functionality ofError andStatus Register (clear ofstatus
flags byread) isdisabled when inDebug/Suspend mode.
Figure 27-21. Error andStatus Register (DCAN ES)[offset =04h]
31 16
Reserved
R-0
15 11 10 9 8
Reserved PDA WakeUpPnd PER
R-0 R-0 RC-0 RC-0
7 6 5 4 3 2 0
BOff EWarn EPass RxOK TxOK LEC
R-0 R-0 R-0 RC-0 RC-0 RS-7h
LEGEND: R=Read only; C=Clear onread; S=Setonread; -n=value after reset
Table 27-8. Error andStatus Register (DCAN ES)Field Descriptions
Bit Field Value Description
31-11 Reserved 0 These bitsarealways read as0.Writes have noeffect.
10 PDA Local power down mode acknowledge.
0 DCAN isnotinlocal power down mode.
1 Application request forsetting DCAN tolocal power down mode was successful. DCAN isinlocal
power down mode.
9 WakeUp Pnd Wake UpPending.
This bitcanbeused bytheCPU toidentify theDCAN asthesource towake upthesystem.
0 NoWake Upisrequested byDCAN.
1 DCAN hasinitiated awake upofthesystem duetodominant CAN buswhile module power down.
This bitwillbereset ifError andStatus Register isread.
8 PER Single-/Double-bit error detected. This bitissetondouble-bit errors andadditionally onsingle-bit
errors, ifsingle-bit error correction isdisabled with theECCMODE bitfield intheECC Control and
Status register.
0 Nosingle-/double-bit error hasbeen detected since lastread access.
1 The SECDED mechanism hasdetected asingle-/double-bit error intheMessage RAM. This bitwill
bereset ifError andStatus Register isread.
7 BOff Bus-Off State
0 The CAN module isnotBus-Off state.
1 The CAN module isinBus-Off state.
6 EWarn Warning State
0 Both error counters arebelow theerror warning limit of96.
1 Atleast oneoftheerror counters hasreached theerror warning limit of96.
5 EPass Error Passive State
0 OnCAN Bus error, theDCAN could send active error frames.
1 The CAN Core isintheerror passive state asdefined intheCAN Specification.

<!-- Page 1460 -->

DCAN Control Registers www.ti.com
1460 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-8. Error andStatus Register (DCAN ES)Field Descriptions (continued)
Bit Field Value Description
4 RxOK Received amessage successfully.
0 Nomessage hasbeen successfully received since thelasttime when thisbitwas read bytheCPU.
This bitisnever reset byDCAN internal events.
1 Amessage hasbeen successfully received since thelasttime when thisbitwas reset byaread
access oftheCPU (independent oftheresult ofacceptance filtering).This bitwillbereset ifError
andStatus Register isread.
3 TxOK Transmitted amessage successfully.
0 Nomessage hasbeen successfully transmitted since thelasttime when thisbitwas read bythe
CPU. This bitisnever reset byDCAN internal events.
1 Amessage hasbeen successfully transmitted (error free andacknowledged byatleast oneother
node) since thelasttime when thisbitwas reset byaread access oftheCPU. This bitwillbereset
ifError andStatus Register isread.
2-0 LEC Last Error Code
The LEC field indicates thetype ofthelasterror ontheCAN bus. This field willbecleared to0
when amessage hasbeen transferred (reception ortransmission) without error.
0 NoError
1h Stuff Error: More than fiveequal bitsinarowhave been detected inapart ofareceived message
where thisisnotallowed.
2h Form Error: Afixed format part ofareceived frame hasthewrong format.
3h AckError: The message thisCAN Core transmitted was notacknowledged byanother node.
4h Bit1 Error: During thetransmission ofamessage (with theexception ofthearbitration field), the
device wanted tosend arecessive level (bitoflogical value 1),butthemonitored busvalue was
dominant.
5h Bit0 Error: During thetransmission ofamessage (oracknowledge bit,oractive error flag, or
overload flag), thedevice wanted tosend adominant level (logical value 0),butthemonitored bus
level was recessive. During Bus-Off recovery, thisstatus isseteach time asequence of11
recessive bitshasbeen monitored. This enables theCPU tomonitor theproceeding oftheBus-Off
recovery sequence (indicating thebusisnotstuck atdominant orcontinuously disturbed).
6h CRC Error: Inareceived message, theCRC check sum was incorrect. (CRC received foran
incoming message does notmatch thecalculated CRC forthereceived data).
7h NoCAN busevent was detected since thelasttime when CPU hasread theError andStatus
Register. Any read access totheError andStatus Register reinitializes theLEC bitto7.

<!-- Page 1461 -->

www.ti.com DCAN Control Registers
1461 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.3 Error Counter Register (DCAN ERRC)
Figure 27-22. Error Counter Register (DCAN ERRC) [offset =08h]
31 16
Reserved
R-0
15 14 8 7 0
RP REC TEC
R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 27-9. Error Counter Register (DCAN ERRC) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 These bitsarealways read as0.Writes have noeffect.
15 RP Receive Error Passive
0 The Receive Error Counter isbelow theerror passive level.
1 The Receive Error Counter hasreached theerror passive level asdefined intheCAN
Specification.
14-8 REC 0-7Fh Receive Error Counter. Actual state oftheReceive Error Counter. (Values from 0to127).
7-0 TEC 0-FFh Transmit Error Counter. Actual state oftheTransmit Error Counter. (Values from 0to255).

<!-- Page 1462 -->

DCAN Control Registers www.ti.com
1462 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.4 BitTiming Register (DCAN BTR)
NOTE: This register isonly writable ifCCE andInitbitsintheCAN Control Register areset.
The CAN bittime may beprogrammed intherange of8to25time quanta.
The CAN time quantum may beprogrammed intherange of1to1024 CAN_CLK periods.
With aCAN_CLK of8MHz andBRPE =00,thereset value of2301h configures theDCAN forabitrate of
500kBit/s.
Fordetails seeSection 27.3.2.1 .
Figure 27-23. BitTiming Register (DCAN BTR) [offset =0Ch]
31 20 19 16
Reserved BRPE
R-0 R/WP-0
15 14 12 11 8 7 6 5 0
Rsvd TSeg2 TSeg1 SJW BRP
R-0 R/WP-2h R/WP-3h R/WP-0 R/WP-1h
LEGEND: R/W =Read/Write; R=Read only; WP=Write Protected byCCE bit;-n=value after reset
Table 27-10. BitTiming Register (DCAN BTR) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 These bitsarealways read as0.Writes have noeffect.
BRPE 0-Fh Baud Rate Prescaler Extension.
Valid programmed values are0to15.Byprogramming BRPE theBaud Rate Prescaler canbe
extended tovalues upto1024.
15 Reserved 0 This bitisalways read as0.Writes have noeffect.
14-12 TSeg2 0-7h Time segment after thesample point.
Valid programmed values are0to7.The actual TSeg2 value thatisinterpreted fortheBitTiming
willbetheprogrammed TSeg2 value +1.
11-8 TSeg1 1h-Fh Time segment before thesample point.
Valid programmed values are1to15.The actual TSeg1 value interpreted fortheBitTiming willbe
theprogrammed TSeg1 value +1.
7-6 SJW 0-3h Synchronization Jump Width
Valid programmed values are0to3.The actual SJW value interpreted fortheSynchronization will
betheprogrammed SJW value +1.
5-0 BRP 0-3Fh Baud Rate Prescaler
Value bywhich theCAN_CLK frequency isdivided forgenerating thebittime quanta. The bittime
isbuilt upfrom amultiple ofthisquanta. Valid programmed values are0to63.The actual BRP
value interpreted fortheBitTiming willbetheprogrammed BRP value +1.

<!-- Page 1463 -->

www.ti.com DCAN Control Registers
1463 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.5 Interrupt Register (DCAN INT)
Figure 27-24. Interrupt Register (DCAN INT) [offset =10h]
31 24 23 16
Reserved Int1ID
R-0 R-0
15 0
Int0ID
R-0
LEGEND: R=Read only; -n=value after reset
Table 27-11. Interrupt Register (DCAN INT) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 These bitsarealways read as0.Writes have noeffect.
23-16 Int1ID Interrupt 1Identifier (indicates themessage object with thehighest pending interrupt).
0 Nointerrupt ispending.
1h-40h Number ofmessage object thatcaused theinterrupt.
41h-FFh Unused
Ifseveral interrupts arepending, theCAN Interrupt Register willpoint tothepending interrupt
with thehighest priority. The DCAN1INT interrupt lineremains active until Int1ID reaches
value 0(the cause oftheinterrupt isreset) oruntil IE1iscleared.
Amessage interrupt iscleared byclearing themessage object 'sIntPnd bit.
Among themessage interrupts, themessage object 'sinterrupt priority decreases with
increasing message number.
15-0 Int0ID Interrupt Identifier (indicates thesource oftheinterrupt).
0 Nointerrupt ispending.
1h-40h Number ofmessage object thatcaused theinterrupt.
41h-7FFFh Unused
8000h Error andStatus Register value isnot7h.
8001h-FFFFh Unused
Ifseveral interrupts arepending, theCAN Interrupt Register willpoint tothepending interrupt
with thehighest priority. The DCAN0INT interrupt lineremains active until Int0ID reaches
value 0(the cause oftheinterrupt isreset) oruntil IE0iscleared.
The Status Interrupt hasthehighest priority. Among themessage interrupts, themessage
object 'sinterrupt priority decreases with increasing message number.

<!-- Page 1464 -->

DCAN Control Registers www.ti.com
1464 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.6 Test Register (DCAN TEST)
Foralltestmodes, theTest bitinCAN Control Register needs tobesettoone. IfTest bitisset,theRDA,
EXL, Tx1, Tx0, LBack andSilent bitsarewritable. BitRxmonitors thestate ofpinCAN_RX andtherefore
isonly readable. AllTest Register functions aredisabled when Test bitiscleared.
NOTE: The Test Register isonly writable ifTest bitinCAN Control Register isset.
Setting Tx[1:0] other than 00willdisturb message transfer.
When theinternal loop back mode isactive (bitLBack isset), bitEXL willbeignored.
Figure 27-25. Test Register (DCAN TEST) [offset =14h]
31 16
Reserved
R-0
15 10 9 8
Reserved RDA EXL
R-0 R/WP-0 R/WP-0
7 6 5 4 3 2 0
Rx Tx LBack Silent Reserved
R-U R/WP-0 R/WP-0 R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write Protected byTest bit;-n=value after reset; U=Undefined
Table 27-12. Test Register (DCAN TEST) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 These bitsarealways read as0.Writes have noeffect.
9 RDA RAM direct access enable.
0 Normal operation.
1 Direct access totheRAM isenabled while inTest Mode.
8 EXL External loop back mode.
0 Disabled
1 Enabled
7 Rx Receive Pin. Monitors theactual value oftheCAN_RX pin.
0 The CAN busisdominant.
1 The CAN busisrecessive.
6-5 Tx Control ofCAN_TX pin.
0 Normal operation, CAN_TX iscontrolled bytheCAN Core.
1h Sample Point canbemonitored atCAN_TX pin.
2h CAN_TX pindrives adominant value.
3h CAN_TX pindrives arecessive value.
4 LBack Loop back mode.
0 Disabled
1 Enabled
3 Silent Silent mode.
0 Disabled
1 Enabled
2-0 Reserved 0 These bitsarealways read as0.Writes have noeffect.

<!-- Page 1465 -->

www.ti.com DCAN Control Registers
1465 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.7 Parity Error Code Register (DCAN PERR)
Ifadouble-bit error isdetected, thePER flagwillbesetintheError andStatus Register. This bitisnot
reset bytheSECDED mechanism; itmust bereset byreading theError andStatus Register. Inaddition to
thePER flag, theSECDED Error Code Register willindicate thememory area where thedouble-bit error
hasbeen detected (message number). After adouble-bit error hasbeen detected, theregister willhold the
lasterror code until power isremoved.
Figure 27-26. Parity Error Code Register (DCAN PERR) [offset =1Ch]
31 16
Reserved
R-0
15 14 11 10 8 7 0
Reserved Word Number Message Number
R-0 R-U R-U
LEGEND: R=Read only; U=value isundefined; -n=value after reset
Table 27-13. Parity Error Code Register (DCAN PERR) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 These bitsarealways read as0.Writes have noeffect.
10-8 Word Number 0 Word Number isreserved anditwillalways read as0.
7-0 Message Number 1h-FFh Message object number where double-bit error hasbeen detected. Only values 1h-40h arevalid.
Values 41h-FFh areinvalid.
27.17.8 Core Release Register (DCAN REL)
Figure 27-27. Core Release Register (DCAN REL) [offset =20h]
31 28 27 24 23 20 19 16
REL STEP SUBSTEP YEAR
R-Ah R-3h R-1h R-7h
15 8 7 0
MON DAY
R-5h R-4h
LEGEND: R=Read only; -n=value after reset
Table 27-14. Core Release Register (DCAN REL) Field Descriptions
Bit Field Value Description
31-28 REL 0-9h Core Release. One digit, BCD-coded.
27-24 STEP 0-9h Step ofCore Release. One digit, BCD-coded.
23-20 SUBSTEP 0-9h Substep ofCore Release. One digit, BCD-coded.
19-16 YEAR 0-9h Design Time Stamp, Year. One digit, BCD-coded. This field issetbyconstant parameter onDCAN
synthesis.
15-8 MON 0-12h Design Time Stamp, Month. Two digits, BCD-coded. This field issetbyconstant parameter on
DCAN synthesis.
7-0 DAY 0-31h Design Time Stamp, Day. Two digits, BCD-coded. This field issetbyconstant parameter onDCAN
synthesis.

<!-- Page 1466 -->

DCAN Control Registers www.ti.com
1466 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.9 ECC Diagnostic Register (DCAN ECCDIAG)
Figure 27-28. ECC Diagnostic Register (DCAN ECCDIAG) [offset =24h]
31 16
Reserved
R-0
15 4 3 0
Reserved ECCDIAG
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset; U=Undefined
Table 27-15. ECC Diagnostic Register (DCAN ECCDIAG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 These bitsarealways read as0.Writes have noeffect.
3-0 ECCDIAG SECDED diagnostic mode enable.
5h Diagnostic mode isenabled. Single-bit anddouble-bit errors areshown intheECCDIAG
STAT andtheECC Control andStatus register. Adouble-bit error (orsingle-bit error with
single-bit error correction disabled) also triggers theparity interrupt flag(PER). Memory
mapping ofECC RAM isenabled.
Ah Diagnostic mode isdisabled, single-bit anddouble-bit errors areshown only intheECC
Control andStatus register.
Allother values Reserved
27.17.10 ECC Diagnostic Status Register (DCAN ECCDIAG STAT)
Figure 27-29. ECC Diagnostic Status Register (DCAN ECCDIAG STAT) [offset =28h]
31 16
Reserved
R-0
15 9 8 7 1 0
Reserved DEFLG_DIAG Reserved SEFLG_DIAG
R-0 R/W1C-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset; U=Undefined
Table 27-16. ECC Diagnostic Status Register (DCAN ECCDIAG STAT) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 These bitsarealways read as0.Writes have noeffect.
8 DEFLG_DIAG Double-bit error flagdiagnostic.
0 Read: Nodouble-bit error isdetected.
Write: The bitisunchanged.
1 Read: Double-bit error isdetected indiagnostic mode.
Write: The bitiscleared to0.
7-1 Reserved 0 These bitsarealways read as0.Writes have noeffect.
0 SEFLG_DIAG Single-bit error flagdiagnostic.
0 Read: Nosingle-bit error isdetected.
Write: The bitisunchanged.
1 Read: Single-bit error isdetected indiagnostic mode.
Write: The bitiscleared to0.

<!-- Page 1467 -->

www.ti.com DCAN Control Registers
1467 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.11 ECC Control andStatus Register (DCAN ECC CS)
Figure 27-30. ECC Control andStatus Register (DCAN ECC CS)[offset =2Ch]
31 28 27 24 23 20 19 16
Reserved SBE_EVT_EN Reserved ECCMODE
R-0 R/WP-5h R-0 R/WP-Ah
15 9 8 7 1 0
Reserved DEFLG Reserved SEFLG
R-0 R/W1C-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; WP=Write inprivileged mode only; -n=value after reset
Table 27-17. ECC Control andStatus Register (DCAN ECC CS)Field Descriptions
Bit Field Value Description
31-9 Reserved 0 These bitsarealways read as0.Writes have noeffect.
27-24 SBE_EVT_EN Enable SECDED single-bit error event (CAN_SERR signal).
5h SECDED single-bit error event isdisabled, single-bit errors arenotsignaled with ahigh pulse
onDCAN_SERR signal.
Allother values SECDED single-bit error event isenabled, single-bit errors aresignaled with ahigh pulse on
DCAN_SERR signal.
23-20 Reserved 0 These bitsarealways read as0.Writes have noeffect.
19-16 ECCMODE Enable SECDED single-bit error correction.
5h SECDED single-bit error correction isdisabled.
Allother values SECDED single-bit error correction isenabled.
15-9 Reserved 0 These bitsarealways read as0.Writes have noeffect.
8 DEFLG Double-bit error flag.
0 Read: Nodouble-bit error isdetected.
Write: The bitisunchanged.
1 Read: Double-bit error isdetected.
Write: The bitiscleared to0.
7-1 Reserved 0 These bitsarealways read as0.Writes have noeffect.
0 SEFLG Single-bit error flag.
0 Read: Nosingle-bit error isdetected.
Write: The bitisunchanged.
1 Read: Single-bit error isdetected.
Write: The bitiscleared to0.

<!-- Page 1468 -->

DCAN Control Registers www.ti.com
1468 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.12 ECC Single-Bit Error Code Register (DCAN ECC SERR)
IfanECC single-bit error isdetected, theSEFLG flagissetintheECC Control andStatus Register. In
addition totheSEFLG flag, theECC Single-Bit Error Code Register indicates thememory area where the
single-bit error hasbeen detected (message object number only).
Ifmore than oneword with anECC single-bit error isdetected, thehighest word number with anECC
single-bit error isdisplayed.
After anECC single-bit error isdetected, theregister holds thelasterror code until power isremoved.
Figure 27-31. ECC Single-Bit Error Code Register (DCAN ECC SERR) [offset =30h]
31 16
Reserved
R-0
15 8 7 0
Reserved Message Number
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value isundefined
Table 27-18. ECC Single-Bit Error Code Register (DCAN ECC SERR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 These bitsarealways read as0.Writes have noeffect.
7-0 Message Number 1h-FFh Message object number where ECC single-bit error hasbeen detected. Only values 1h-40h are
valid. Values 41h-FFh areinvalid.

<!-- Page 1469 -->

www.ti.com DCAN Control Registers
1469 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.13 Auto-Bus-On Time Register (DCAN ABOTR)
NOTE: Onwrite access totheCAN Control register while Auto-Bus-On timer isrunning, theAuto-
Bus-On procedure willbeaborted.
During Debug/Suspend mode, running Auto-Bus-On timer willbepaused.
Figure 27-32. Auto-Bus-On Time Register (DCAN ABOTR) [offset =80h]
31 0
ABO_TIME
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 27-19. Auto-Bus-On Time Register (DCAN ABOTR) Field Descriptions
Bit Field Description
31-0 ABO_TIME Number ofVBUS clock cycles before aBus-Off recovery sequence isstarted byclearing theInitbit.This
function hastobeenabled bysetting bitABO inCAN Control Register.
The Auto-Bus-On timer isrealized bya32-bit counter thatstarts tocount down to0when themodule goes
Bus-Off.
The counter willbereloaded with thepreload value oftheABO_TIME register after thisphase.
27.17.14 Transmission Request XRegister (DCAN TXRQ X)
With theTransmission Request XRegister, theCPU candetect ifoneormore bitsinthedifferent
Transmission Request Registers areset.Each register bitrepresents agroup ofeight message objects. If
atleast oneoftheTxRqst bitsofthese message objects areset,thecorresponding bitinthe
Transmission Request XRegister willbeset.
Figure 27-33. Transmission Request XRegister (DCAN TXRQ X)[offset =84h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
TxRqstReg8 TxRqstReg7 TxRqstReg6 TxRqstReg5 TxRqstReg4 TxRqstReg3 TxRqstReg2 TxRqstReg1
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Example 1
Bit0oftheTransmission Request XRegister represents byte 0oftheTransmission Request 1Register. If
oneormore bitsinthisbyte areset,bit0oftheTransmission Request XRegister willbeset.

<!-- Page 1470 -->

DCAN Control Registers www.ti.com
1470 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.15 Transmission Request Registers (DCAN TXRQ12 toDCAN TXRQ78)
These registers hold theTxRqst bitsoftheimplemented message objects. Byreading outthese bits, the
CPU cancheck forpending transmission requests. The TxRqst bitinaspecific message object canbe
set/reset bytheCPU viatheIF1/IF2 Message Interface Registers, orbytheMessage Handler after
reception ofaremote frame orafter asuccessful transmission.
Figure 27-34. Transmission Request 12Register (DCAN TXRQ12) [offset =88h]
31 0
TxRqst[32:1]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-35. Transmission Request 34Register (DCAN TXRQ34) [offset =8Ch]
31 0
TxRqst[64:33]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-36. Transmission Request 56Register (DCAN TXRQ56) [offset =90h]
31 0
TxRqst[96:65]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-37. Transmission Request 78Register (DCAN TXRQ78) [offset =94h]
31 0
TxRqst[128:97]
R-0
LEGEND: R=Read only; -n=value after reset
Table 27-20. Transmission Request Registers Field Descriptions
Bit Name Value Description
31-0 TxRqst[128:1] Transmission Request Bits(forallmessage objects).
0 Notransmission hasbeen requested forthismessage object.
1 The transmission ofthismessage object isrequested andisnotyetdone.

<!-- Page 1471 -->

www.ti.com DCAN Control Registers
1471 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.16 New Data XRegister (DCAN NWDAT X)
With theNew Data XRegister, theCPU candetect ifoneormore bitsinthedifferent New Data Registers
areset.Each register bitrepresents agroup ofeight message objects. Ifatleast onoftheNewDat bitsof
these message objects areset,thecorresponding bitintheNew Data XRegister willbeset.
Figure 27-38. New Data XRegister (DCAN NWDAT X)[offset =98h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
NewDatReg8 NewDatReg7 NewDatReg6 NewDatReg5 NewDatReg4 NewDatReg3 NewDatReg2 NewDatReg1
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Equation 1
Bit0oftheNew Data XRegister represents byte 0oftheNew Data 1Register. Ifoneormore bitsinthis
byte areset,bit0oftheNew Data XRegister willbeset.

<!-- Page 1472 -->

DCAN Control Registers www.ti.com
1472 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.17 New Data Registers (DCAN NWDAT12 toDCAN NWDAT78)
These registers hold theNewDat bitsoftheimplemented message objects. Byreading outthese bits, the
CPU cancheck fornew data inthemessage objects. The NewDat bitofaspecific message object canbe
set/reset bytheCPU viatheIF1/IF2 Interface Register sets, orbytheMessage Handler after reception of
adata frame orafter asuccessful transmission.
Figure 27-39. New Data 12Register (DCAN NWDAT12) [offset =9Ch]
31 0
NewDat[32:1]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-40. New Data 34Register (DCAN NWDAT34) [offset =A0h]
31 0
NewDat[64:33]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-41. New Data 56Register (DCAN NWDAT56) [offset =A4h]
31 0
NewDat[96:65]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-42. New Data 78Register (DCAN NWDAT78) [offset =A8h]
31 0
NewDat[128:97]
R-0
LEGEND: R=Read only; -n=value after reset
Table 27-21. New Data Registers Field Descriptions
Bit Name Value Description
31-0 NewDat[128:1] New Data Bits(forallmessage objects).
0Nonew data hasbeen written intothedata portion ofthismessage object bytheMessage Handler
since thelasttime when thisflagwas cleared bytheCPU.
1The Message Handler ortheCPU haswritten new data intothedata portion ofthismessage
object.

<!-- Page 1473 -->

www.ti.com DCAN Control Registers
1473 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.18 Interrupt Pending XRegister (DCAN INTPND X)
With theInterrupt Pending XRegister, theCPU candetect ifoneormore bitsinthedifferent Interrupt
Pending Registers areset.Each bitofthisregister represents agroup ofeight message objects. Ifatleast
oneoftheIntPnd bitsofthese message objects areset,thecorresponding bitintheInterrupt Pending X
Register willbeset.
Figure 27-43. Interrupt Pending XRegister (DCAN INTPND X)[offset =ACh]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
IntPndReg8 IntPndReg7 IntPndReg6 IntPndReg5 IntPndReg4 IntPndReg3 IntPndReg2 IntPndReg1
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Example 2
Bit0oftheInterrupt Pending XRegister represents byte 0oftheInterrupt Pending 1Register. Ifoneor
more bitsinthisbyte areset,bit0oftheInterrupt Pending XRegister willbeset.

<!-- Page 1474 -->

DCAN Control Registers www.ti.com
1474 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.19 Interrupt Pending Registers (DCAN INTPND12 toDCAN INTPND78)
These registers hold theIntPnd bitsoftheimplemented message objects. Byreading outthese bits, the
CPU cancheck forpending interrupts inthemessage objects. The IntPnd bitofaspecific message object
canbeset/reset bytheCPU viatheIF1/IF2 Interface Register sets, orbytheMessage Handler after a
reception orasuccessful transmission.
Figure 27-44. Interrupt Pending 12Register (DCAN INTPND12) [offset =B0h]
31 0
IntPnd[32:1]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-45. Interrupt Pending 34Register (DCAN INTPND34) [offset =B4h]
31 0
IntPnd[64:33]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-46. Interrupt Pending 56Register (DCAN INTPND56) [offset =B8h]
31 0
IntPnd[96:65]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-47. Interrupt Pending 78Register (DCAN INTPND78) [offset =BCh]
31 0
IntPnd[128:97]
R-0
LEGEND: R=Read only; -n=value after reset
Table 27-22. Interrupt Pending Registers Field Descriptions
Bit Name Value Description
31-0 IntPnd[128:1] Interrupt Pending Bits(forallmessage objects).
0 This message object isnotthesource ofaninterrupt.
1 This message object isthesource ofaninterrupt.

<!-- Page 1475 -->

www.ti.com DCAN Control Registers
1475 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.20 Message Valid XRegister (DCAN MSGVAL X)
With theMessage Valid XRegister, theCPU candetect ifoneormore bitsinthedifferent Message Valid
Registers areset.Each bitofthisregister represents agroup ofeight message objects. Ifatleast oneof
theMsgVal bitsofthese message objects areset,thecorresponding bitintheMessage Valid XRegister
willbeset.
Figure 27-48. Message Valid XRegister (DCAN MSGVAL X)[offset =C0h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
MsgValReg8 MsgValReg7 MsgValReg6 MsgValReg5 MsgValReg4 MsgValReg3 MsgValReg2 MsgValReg1
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Example 3
Bit0oftheMessage Valid XRegister represents byte 0oftheMessage Valid 1Register. Ifoneormore
bitsinthisbyte areset,bit0oftheMessage Valid XRegister willbeset.

<!-- Page 1476 -->

DCAN Control Registers www.ti.com
1476 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.21 Message Valid Registers (DCAN MSGVAL12 toDCAN MSGVAL78)
These registers hold theMsgVal bitsoftheimplemented message objects. Byreading outthese bits, the
CPU cancheck which message objects arevalid. The MsgVal bitofaspecific message object canbe
set/reset bytheCPU viatheIF1/IF2 Interface Register sets, orbytheMessage Handler after areception
orasuccessful transmission.
Figure 27-49. Message Valid 12Register (DCAN MSGVAL12) [offset =C4h]
31 0
MsgVal[32:1]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-50. Message Valid 34Register (DCAN MSGVAL34) [offset =C8h]
31 0
MsgVal[64:33]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-51. Message Valid 56Register (DCAN MSGVAL56) [offset =CCh]
31 0
MsgVal[96:65]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 27-52. Message Valid 78Register (DCAN MSGVAL78) [offset =D0h]
31 0
MsgVal[128:97]
R-0
LEGEND: R=Read only; -n=value after reset
Table 27-23. Message Valid Registers Field Descriptions
Bit Name Value Description
31-0 MsgVal[128:1] Message Valid Bits(forallmessage objects).
0 This message object isignored bytheMessage Handler.
1 This message object isconfigured andwillbeconsidered bytheMessage Handler.

<!-- Page 1477 -->

www.ti.com DCAN Control Registers
1477 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.22 Interrupt Multiplexer Registers (DCAN INTMUX12 toDCAN INTMUX78)
The IntMux flagdetermines foreach message object which ofthetwointerrupt lines (DCAN0INT or
DCAN1INT) willbeasserted when theIntPnd ofthismessage object isset.Both interrupt lines canbe
globally enabled ordisabled bysetting orclearing IE0andIE1bitsinCAN Control Register.
The IntPnd bitofaspecific message object canbesetorreset bytheCPU viatheIF1/IF2 Interface
Register sets, orbyMessage Handler after reception orsuccessful transmission ofaframe. This willalso
affect theInt0ID resp Int1ID flags intheInterrupt Register.
Figure 27-53. Interrupt Multiplexer 12Register (DCAN INTMUX12) [offset =D8h]
31 0
IntMux[32:1]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-54. Interrupt Multiplexer 34Register (DCAN INTMUX34) [offset =DCh]
31 0
IntMux[64:33]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-55. Interrupt Multiplexer 56Register (DCAN INTMUX56) [offset =E0h]
31 0
IntMux[96:65]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-56. Interrupt Multiplexer 78Register (DCAN INTMUX78) [offset =E4h]
31 0
IntMux[128:97]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 27-24. Interrupt Multiplexer Registers Field Descriptions
Bit Name Value Description
31-0 IntMux[128:1] Multiplexes IntPnd value toeither DCAN0INT orDCAN1INT interrupt lines. The mapping from the
bitstothemessage objects isasfollows:
Bit0->lastimplemented message object.
Bit1->message object number 1
Bit2->message object number 2
0 DCAN0INT lineisactive ifcorresponding IntPnd flagis1.
1 DCAN1INT lineisactive ifcorresponding IntPnd flagis1.

<!-- Page 1478 -->

DCAN Control Registers www.ti.com
1478 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.23 IF1/IF2 Command Registers (DCAN IF1CMD, DCAN IF2CMD)
The IF1/IF2 Command Register configure andInitiate thetransfer between theIF1/IF2 Register sets and
theMessage RAM. Itisconfigurable which portions ofthemessage object should betransferred.
Atransfer isstarted when theCPU writes themessage number tobits[7:0] oftheIF1/IF2 Command
Register. With thiswrite operation, theBusy bitisautomatically setto1toindicate thatatransfer isin
progress.
After 4to14VBUS clock cycles, thetransfer between theInterface Register andtheMessage RAM will
becompleted andtheBusy bitiscleared. The maximum number ofcycles isneeded when themessage
transfer concurs with aCAN message transmission, acceptance filtering, ormessage storage.
IftheCPU writes toboth IF1/IF2 Command Registers consecutively (request ofasecond transfer while
firsttransfer isstillinprogress), thesecond transfer willstart after thefirstonehasbeen completed.
NOTE: While Busy bitisone, IF1/IF2 Register sets arewrite protected.
Fordebug support, theauto clear functionality oftheIF1/IF2 Command Registers (clear of
DMAactive flagbyr/w)isdisabled during Debug/Suspend mode.
Ifaninvalid Message Number iswritten tobits[7:0] oftheIF1/IF2 Command Register, the
Message Handler may access animplemented (valid) message object instead.
Figure 27-57. IF1Command Registers (DCAN IF1CMD) [offset =100h]
31 24
Reserved
R-0
23 22 21 20 19 18 17 16
WR/RD Mask Arb Control ClrIntPnd TxRqst/NewDat Data A Data B
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 8
Busy DMA Active Reserved
R-0 R/WP/C-0 R-0
7 0
Message Number
R/WP-1h
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);C=Clear byIF1Access; -n=value after reset
Figure 27-58. IF2Command Registers (DCAN IF2CMD) [offset =120h]
31 24
Reserved
R-0
23 22 21 20 19 18 17 16
WR/RD Mask Arb Control ClrIntPnd TxRqst/NewDat Data A Data B
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 14 13 8
Busy DMA Active Reserved
R-0 R/WP/C-0 R-0
7 0
Message Number
R/WP-1h
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);C=Clear byIF1Access; -n=value after reset

<!-- Page 1479 -->

www.ti.com DCAN Control Registers
1479 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-25. IF1/IF2 Command Register Field Descriptions
Bit Field Value Description
31-24 Reserved 0 These bitsarealways read as0.Writes have noeffect.
23 WR/RD Write/Read
0 Direction =Read: Transfer direction isfrom themessage object addressed byMessage
Number (Bits [7:0]) totheIF1/IF2 register set.
1 Direction =Write: Transfer direction isfrom theIF1/IF2 register settothemessage object
addressed byMessage Number (Bits [7:0]).
22 Mask Access Mask bits.
0 Mask bitswillnotbechanged.
1 Direction =Read: The Mask bits(Identifier Mask +MDir +MXtd) willbetransferred from the
message object addressed byMessage Number (Bits [7:0]) totheIF1/IF2 Register set.
Direction =Write: The Mask bits(Identifier Mask +MDir +MXtd) willbetransferred from the
IF1/IF2 Register settothemessage object addressed byMessage Number (Bits [7:0]).
21 Arb Access Arbitration bits.
0 Arbitration bitswillnotbechanged.
1 Direction =Read: The Arbitration bits(Identifier +Dir+Xtd+MsgVal) willbetransferred from
themessage object addressed byMessage Number (Bits [7:0]) tothecorresponding IF1/IF2
Register set.
Direction =Write: The Arbitration bits(Identifier +Dir+Xtd+MsgVal) willbetransferred from
theIF1/IF2 Register settothemessage object addressed byMessage Number (Bits [7:0]).
20 Control Access Control bits.
0 Control bitswillnotbechanged.
1 Direction =Read: The Message Control bitswillbetransferred from themessage object
addressed byMessage Number (Bits [7:0]) tothecorresponding IF1/IF2 Register set.
Direction =Write: The Message Control bitswillbetransferred from theIF1/IF2 Register setto
themessage object addressed byMessage Number (Bits [7:0]).
IftheTxRqst/NewDat bitinthisregister (Bit[18]) isset,theTxRqst/NewDat bitintheIF1/IF2
Message Control Register willbeignored.
19 ClrIntPnd Clear Interrupt Pending bit.
0 IntPnd bitwillnotbechanged.
1 Direction =Read: Clears IntPnd bitinthemessage object.
Direction =Write: This bitisignored. Copying ofIntPnd flagfrom IF1/IF2 Registers toMessage
RAM canbecontrolled byonly theControl flag(Bit[20]).
18 TxRqst/NewDat Access Transmission Request bit.
0 Direction =Read: NewDat bitwillnotbechanged.
Direction =Write: TxRqst/NewDat bitwillbehandled according totheControl bit.
1 Direction =Read: Clears NewDat bitinthemessage object.
Direction =Write: Sets TxRqst/NewDat inthemessage object.
Note: IfaCAN transmission isrequested bysetting TxRqst/NewDat inthisregister, the
TxRqst/NewDat bitsinthemessage object willbesetto1andindependent ofthevalues in
IF1/IF2 Message Control Register.
Aread access toamessage object canbecombined with thereset ofthecontrol bitsIntPnd
andNewDat. The values ofthese bitstransferred totheIF1/IF2 Message Control Register
always reflect thestatus before resetting them.
17 Data A Access Data Bytes 0-3.
0 Data Bytes 0-3willnotbechanged.
1 Direction =Read: The Data Bytes 0-3willbetransferred from themessage object addressed
bytheMessage Number (Bits [7:0]) tothecorresponding IF1/IF2 Register set.
Direction =Write: The Data Bytes 0-3willbetransferred from theIF1/IF2 Register settothe
message object addressed bytheMessage Number (Bits [7:0]).
Note: The duration ofthemessage transfer isindependent ofthenumber ofbytes tobe
transferred.

<!-- Page 1480 -->

DCAN Control Registers www.ti.com
1480 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-25. IF1/IF2 Command Register Field Descriptions (continued)
Bit Field Value Description
16 Data B Access Data Bytes 4-7.
0 Data Bytes 4-7willnotbechanged.
1 Direction =Read: The Data Bytes 4-7willbetransferred from themessage object addressed
bytheMessage Number (Bits [7:0]) tothecorresponding IF1/IF2 Register set.
Direction =Write: The Data Bytes 4-7willbetransferred from theIF1/IF2 Register settothe
message object addressed bytheMessage Number (Bits [7:0]).
Note: The duration ofthemessage transfer isindependent ofthenumber ofbytes tobe
transferred.
15 Busy Busy flag.
0 Notransfer between IF1/IF2 Register setandMessage RAM isinprogress.
1 Transfer between IF1/IF2 Register setandMessage RAM isinprogress.
This bitissetto1after themessage number hasbeen written tobits[7:0]. IF1/IF2 Register set
willbewrite-protected. The bitiscleared after read/write action hasfinished.
14 DMA Active Activation ofDMA feature forsubsequent internal IF1/IF2 update.
0 DMA request lineisindependent ofIF1/IF2 activities.
1 DMA isrequested after completed transfer between IF1/IF2 Register setandMessage RAM.
The DMA request remains active until thefirstread orwrite tooneoftheIF1/IF2 registers. An
exception isawrite toMessage Number (Bits [7:0]) when DMA Active is1.
Note: Due totheauto reset feature oftheDMA Active bit,thisbithastobeseparately setfor
each subsequent DMA cycle.
13-8 Reserved 0 These bitsarealways read as0.Writes have noeffect.
7-0 Message Number Number ofmessage object inMessage RAM thatisused fordata transfer.
0 Invalid message number.
1h-40h Valid message numbers.
41h-FFh Invalid message numbers.
Note: When aninvalid message number iswritten totheIF1/IF2 Command Register thatis
higher than thelastimplemented message object number, amodulo addressing willoccur. For
example, when accessing message object 33inaDCAN module with 32message objects only,
themessage object 1willbeaccessed instead.

<!-- Page 1481 -->

www.ti.com DCAN Control Registers
1481 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.24 IF1/IF2 Mask Registers (DCAN IF1MSK, DCAN IF2MSK)
The bitsoftheIF1/IF2 Mask Registers mirror themask bitsofamessage object. The function ofthe
relevant message objects bitsisdescribed inSection 27.5.1 .
NOTE: While Busy bitofIF1/IF2 Command Register isone, IF1/IF2 Register Setiswrite protected.
Figure 27-59. IF1Mask Register (DCAN IF1MSK) [offset =104h]
31 30 29 28 16
MXtd MDir Rsvd Msk[28:16]
R/WP-1 R/WP-1 R-1 R/WP-1FFFh
15 0
Msk[15:0]
R/WP-FFFFh
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-60. IF2Mask Register (DCAN IF2MSK) [offset =124h]
31 30 29 28 16
MXtd MDir Rsvd Msk[28:16]
R/WP-1 R/WP-1 R-1 R/WP-1FFFh
15 0
Msk[15:0]
R/WP-FFFFh
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);-n=value after reset
Table 27-26. IF1/IF2 Mask Register Field Descriptions
Bit Field Value Description
31 MXtd Mask extended identifier.
0 The extended identifier bit(IDE) hasnoeffect ontheacceptance filtering.
1 The extended identifier bit(IDE) isused foracceptance filtering.
When 11-bit ("standard ")identifiers areused foramessage object, theidentifiers ofreceived Data
Frames arewritten intobitsID[28:18]. Foracceptance filtering, only these bitswith mask bits
Msk[28:18] areconsidered.
30 MDir Mask message direction.
0 The message direction bit(Dir) hasnoeffect ontheacceptance filtering.
1 The message direction bit(Dir) isused foracceptance filtering.
29 Reserved 0 These bitsarealways read as1.Writes have noeffect.
28-0 Msk[ n] Identifier mask.
0 The corresponding bitintheidentifier ofthemessage object isnotused foracceptance filtering
(don'tcare).
1 The corresponding bitintheidentifier ofthemessage object isused foracceptance filtering.

<!-- Page 1482 -->

DCAN Control Registers www.ti.com
1482 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.25 IF1/IF2 Arbitration Registers (DCAN IF1ARB, DCAN IF2ARB)
The bitsoftheIF1/IF2 Arbitration Registers mirror thearbitration bitsofamessage object. The function of
therelevant message objects bitsisdescribed inSection 27.5.1 .
The Arbitration bitsID,Xtd, andDirareused todefine theidentifier andtype ofoutgoing messages and
(together with theMask bitsMsk, MXtd, andMDir) foracceptance filtering ofincoming messages.
Areceived message isstored intothevalid message object with matching identifier andDirection =
receive (Data Frame) orDirection =transmit (Remote Frame).
Extended frames canbestored only inmessage objects with Xtd=1,standard frames inmessage objects
with Xtd=0.
Ifareceived message (Data Frame orRemote Frame) matches more than onevalid message objects, it
isstored intotheonewith thelowest message number.
NOTE: While Busy bitofIF1/IF2 Command Register isone, IF1/IF2 Register Setiswrite protected.
Figure 27-61. IF1Arbitration Register (DCAN IF1ARB) [offset =108h]
31 30 29 28 16
MsgVal Xtd Dir ID[28:16]
R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 0
ID[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-62. IF2Arbitration Register (DCAN IF2ARB) [offset =128h]
31 30 29 28 16
MsgVal Xtd Dir ID[28:16]
R/WP-0 R/WP-0 R/WP-0 R/WP-0
15 0
ID[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset

<!-- Page 1483 -->

www.ti.com DCAN Control Registers
1483 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-27. IF1/IF2 Arbitration Register Field Descriptions
Bit Field Value Description
31 MsgVal Message valid
0 The message object isignored bytheMessage Handler.
1 The message object isused bytheMessage Handler.
Note: The CPU should reset theMsgVal bitofallunused Messages Objects during the
initialization before itresets bitInitintheCAN Control Register. MsgVal must also bereset if
themessages object isnolonger used inoperation. Forreconfiguration ofmessage objects
during normal operation, seeSection 27.7.6 andSection 27.7.7 .
30 Xtd Extended identifier.
0 The 11-bit ("standard ")identifier isused forthismessage object.
1 The 29-bit ("extended ")identifier isused forthismessage object.
29 Dir Message direction.
0 Direction =Receive: OnTxRqst, aRemote Frame with theidentifier ofthismessage object is
transmitted. Onreceiving aData Frame with amatching identifier, thismessage isstored inthis
message object.
1 Direction =Transmit: OnTxRqst, therespective message object istransmitted asaData
Frame. Onreceiving aRemote Frame with amatching identifier, theTxRqst bitofthismessage
object isset(ifRmtEn =1).
28-0 ID Message identifier.
ID[28:0] 29-bit Identifier ("Extended Frame ").
ID[28:18] 11-bit Identifier ("Standard Frame ").

<!-- Page 1484 -->

DCAN Control Registers www.ti.com
1484 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.26 IF1/IF2 Message Control Registers (DCAN IF1MCTL, DCAN IF2MCTL)
The bitsoftheIF1/IF2 Message Control Registers mirror themessage control bitsofamessage object.
The function oftherelevant message objects bitsisdescribed inSection 27.5.1 .
NOTE: While Busy bitofIF1/IF2 Command Register isone, IF1/IF2 Register Setiswrite protected.
Figure 27-63. IF1Message Control Register (DCAN IF1MCTL) [offset =10Ch]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
NewDat MsgLst IntPnd UMask TxIE RxIE RmtEn TxRqst
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 4 3 0
EoB Reserved DLC
R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-64. IF2Message Control Register (DCAN IF2MCTL) [offset =12Ch]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
NewDat MsgLst IntPnd UMask TxIE RxIE RmtEn TxRqst
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 4 3 0
EoB Reserved DLC
R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byBusy bit);-n=value after reset

<!-- Page 1485 -->

www.ti.com DCAN Control Registers
1485 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-28. IF1/IF2 Message Control Register Field Descriptions
Bit Field Value Description
31-16 Reserved 0 These bitsarealways read as0.Writes have noeffect.
15 NewDat New Data
0 Nonew data hasbeen written intothedata portion ofthismessage object bytheMessage Handler
since thelasttime thisflagwas cleared bytheCPU.
1 The Message Handler ortheCPU haswritten new data intothedata portion ofthismessage
object.
14 MsgLst Message Lost (only valid formessage objects with direction =receive).
0 Nomessage lostsince thelasttime when thisbitwas reset bytheCPU.
1 The Message Handler stored anew message intothisobject when NewDat was stillset,sothe
previous message hasbeen overwritten.
13 IntPnd Interrupt Pending
0 This message object isnotthesource ofaninterrupt.
1 This message object isthesource ofaninterrupt. The interrupt identifier intheinterrupt register will
point tothismessage object ifthere isnoother interrupt source with higher priority.
12 UMask Use Acceptance Mask
0 Mask isignored.
1 Use Mask (Msk[28:0], MXtd, andMDir) foracceptance filtering.
IftheUMask bitissetto1,themessage object 'smask bitshave tobeprogrammed during
initialization ofthemessage object before MsgVal issetto1.
11 TxIE Transmit interrupt enable.
0 IntPnd willnotbetriggered after thesuccessful transmission ofaframe.
1 IntPnd willbetriggered after thesuccessful transmission ofaframe.
10 RxIE Receive interrupt enable.
0 IntPnd willnotbetriggered after thesuccessful reception ofaframe.
1 IntPnd willbetriggered after thesuccessful reception ofaframe.
9 RmtEn Remote enable.
0 Atthereception ofaRemote Frame, TxRqst isnotchanged.
1 Atthereception ofaRemote Frame, TxRqst isset.
8 TxRqst Transmit request.
0 This message object isnotwaiting foratransmission.
1 The transmission ofthismessage object isrequested andnotyetdone.
7 EoB End ofBlock
0 The message object ispart ofaFIFO Buffer block andisnotthelastmessage object ofthisFIFO
Buffer block.
1 The message object isasingle message object orthelastmessage object inaFIFO Buffer block.
Note: This bitisused toconcatenate multiple message objects tobuild aFIFO Buffer. Forsingle
message objects (not belonging toaFIFO Buffer), thisbitmust always besetto1.
6-4 Reserved 0 These bitsarealways read as0.Writes have noeffect.
3-0 DLC Data Length Code
0-8h Data Frame has0-8data bits.
9h-Fh Data Frame has8data bytes.
Note: The Data Length Code ofamessage object must bedefined thesame asinallthe
corresponding objects with thesame identifier atother nodes. When themessage handler stores a
data frame, itwillwrite theDLC tothevalue given bythereceived message.

<!-- Page 1486 -->

DCAN Control Registers www.ti.com
1486 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.27 IF1/IF2 Data AandData BRegisters (DCAN IF1DATA/DATB, DCAN IF2DATA/DATB)
The data bytes ofCAN messages arestored intheIF1/IF2 registers inthefollowing order.
InaCAN Data Frame, Data 0isthefirst, andData 7isthelastbyte tobetransmitted orreceived. In
CAN's serial bitstream, theMSB ofeach byte willbetransmitted first
Figure 27-65. IF1Data ARegister (DCAN IF1DATA) [offset =110h]
31 24 23 16
Data 3 Data 2
R/WP-0 R/WP-0
15 8 7 0
Data 1 Data 0
R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-66. IF1Data BRegister (DCAN IF1DATB) [offset =114h]
31 24 23 16
Data 7 Data 6
R/WP-0 R/WP-0
15 8 7 0
Data 5 Data 4
R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-67. IF2Data ARegister (DCAN IF2DATA) [offset =130h]
31 24 23 16
Data 3 Data 2
R/WP-0 R/WP-0
15 8 7 0
Data 1 Data 0
R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset
Figure 27-68. IF2Data BRegister (DCAN IF2DATB) [offset =134h]
31 24 23 16
Data 7 Data 6
R/WP-0 R/WP-0
15 8 7 0
Data 5 Data 4
R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; WP=Protected Write (protected byBusy bit);-n=value after reset

<!-- Page 1487 -->

www.ti.com DCAN Control Registers
1487 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.28 IF3Observation Register (DCAN IF3OBS)
The IF3register setcanautomatically beupdated with received message objects without theneed to
Initiate thetransfer from Message RAM byCPU (Additional information canbefound inSection 27.5.1 ).
The observation flags (Bits [4:0]) intheIF3Observation register areused todetermine, which data
sections oftheIF3Interface Register sethave toberead inorder tocomplete aDMA read cycle. After all
marked data sections areread, theDCAN isenabled toupdate theIF3Interface Register setwith new
data.
Any access order ofsingle bytes orhalf-words issupported. When using byte orhalf-word accesses, a
data section ismarked ascompleted, ifallbytes areread.
NOTE: IfIF3Update Enable isused andnoObservation flagisset,thecorresponding message
objects willbecopied toIF3without activating theDMA request lineandwithout waiting for
DMA read accesses.
Awrite access tothisregister aborts apending DMA cycle byresetting theDMA lineandenables
updating ofIF3Interface Register setwith new data. Toavoid data inconsistency, theDMA controller
should bedisabled before reconfiguring IF3observation register.
The status ofthecurrent read-cycle canbeobserved viastatus flags (Bits [12:8]).
Aninterrupt request may begenerated bytheIF3Upd flagiftheDE3 bitofDCAN CTL register isset.See
thedevice data sheet tofindoutifthisinterrupt source isavailable.
With this, theobservation status bitsandtheIF3Upd bitcould beused bytheapplication torealize the
notification about new IF3content inpolling orinterrupt mode.
Figure 27-69. IF3Observation Register (DCAN IF3OBS) [offset =140h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 5 4 3 2 1 0
IF3Upd Reserved IF3SDB IF3SDA IF3SC IF3SA IF3SM Reserved Data BData A Ctrl Arb Mask
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read; -n=value after reset
Table 27-29. IF3Observation Register (DCAN IF3OBS) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 These bitsarealways read as0.Writes have noeffect.
15 IF3Upd IF3Update Data.
0 Nonew data hasbeen loaded since IF3was lastread.
1 New data hasbeen loaded since IF3was lastread.
14-13 Reserved 0 These bitsarealways read as0.Writes have noeffect
12 IF3SDB IF3Status ofData Bread access.
0 AllData Bbytes arealready read orarenotmarked toberead.
1 Data Bsection stillhasdata toread.
11 IF3SDA IF3Status ofData Aread access.
0 AllData Abytes arealready read orarenotmarked toberead.
1 Data Asection stillhasdata toread.
10 IF3SC IF3Status ofControl bitsread access.
0 AllControl section bytes arealready read orarenotmarked toberead.
1 Control section stillhasdata toread.

<!-- Page 1488 -->

DCAN Control Registers www.ti.com
1488 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-29. IF3Observation Register (DCAN IF3OBS) Field Descriptions (continued)
Bit Field Value Description
9 IF3SA IF3Status ofArbitration data read access.
0 AllArbitration data bytes arealready read orarenotmarked toberead.
1 Arbitration section stillhasdata toread.
8 IF3SM IF3Status ofMask data read access.
0 AllMask data bytes arealready read orarenotmarked toberead.
1 Mask section stillhasdata toread.
7-5 Reserved 0 These bitsarealways read as0.Writes have noeffect
4 Data B Data Bread observation.
0 Data Bsection does notneed toberead.
1 Data Bsection hastoberead toenable next IF3update.
3 Data A Data Aread observation.
0 Data Asection does notneed toberead.
1 Data Asection hastoberead toenable next IF3update.
2 Ctrl Ctrlread observation.
0 Ctrlsection does notneed toberead.
1 Ctrlsection hastoberead toenable next IF3update.
1 Arb Arbitration data read observation.
0 Arbitration data does notneed toberead.
1 Arbitration data hastoberead toenable next IF3update.
0 Mask Mask data read observation.
0 Mask data does notneed toberead.
1 Mask data hastoberead toenable next IF3update.

<!-- Page 1489 -->

www.ti.com DCAN Control Registers
1489 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.29 IF3Mask Register (DCAN IF3MSK)
Figure 27-70. IF3Mask Register (DCAN IF3MSK) [offset =144h]
31 30 29 28 16
MXtd MDir Rsvd Msk[28:16]
R-1 R-1 R-1 R-1FFFh
15 0
Msk[15:0]
R-FFFFh
LEGEND: R=Read; -n=value after reset
Table 27-30. IF3Mask Register (DCAN IF3MSK) Field Descriptions
Bit Field Value Description
31 MXtd Mask extended identifier.
0 The extended identifier bit(IDE) hasnoeffect onacceptance filtering.
1 The extended identifier bit(IDE) isused foracceptance filtering.
Note: When 11-bit ("standard ")identifiers areused foramessage object, theidentifiers ofreceived
Data Frames arewritten intobitsID[28:18]. Foracceptance filtering, only these bits, together with
mask bitsMsk[28:18], areconsidered.
30 MDir Mask message direction.
0 The message direction bit(Dir) hasnoeffect onacceptance filtering.
1 The message direction bit(Dir) isused foracceptance filtering.
29 Reserved 0 These bitsarealways read as0.Writes have noeffect.
28-0 Msk[ n] Identifier mask.
0 The corresponding bitintheidentifier ofthemessage object isnotused foracceptance filtering
(don'tcare).
1 The corresponding bitintheidentifier ofthemessage object isused foracceptance filtering.

<!-- Page 1490 -->

DCAN Control Registers www.ti.com
1490 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.30 IF3Arbitration Register (DCAN IF3ARB)
Figure 27-71. IF3Arbitration Register (DCAN IF3ARB) [offset =148h]
31 30 29 28 16
MsgVal Xtd Dir ID[28:16]
R-0 R-0 R-0 R-0
15 0
ID[15:0]
R-0
LEGEND: R=Read; -n=value after reset
Table 27-31. IF3Arbitration Register (DCAN IF3ARB) Field Descriptions
Bit Field Value Description
31 MsgVal Message valid.
0 The message object isignored bytheMessage Handler.
1 The message object istobeused bytheMessage Handler.
Note: The CPU should reset theMsgVal bitofallunused Messages Objects during the
initialization before itresets bitInitintheCAN Control Register. MsgVal must also bereset if
themessages object isnolonger used inoperation. Forreconfiguration ofmessage objects
during normal operation, seeSection 27.7.6 andSection 27.7.7 .
30 Xtd Extended identifier.
0 The 11-bit ("standard ")identifier isused forthismessage object.
1 The 29-bit ("extended ")identifier isused forthismessage object.
29 Dir Message direction.
0 Direction =Receive: OnTxRqst, aremote frame with theidentifier ofthismessage object is
transmitted. Onreceiving adata frame with amatching identifier, themessage isstored inthis
message object.
1 Direction =Transmit: OnTxRqst, therespective message object istransmitted asadata frame.
Onreceiving aremote frame with amatching identifier, theTxRqst bitofthismessage object is
set(ifRmtEn =1).
28-0 ID Message identifier.
ID[28:0] 29-bit Identifier ("Extended Frame ").
ID[28:18] 11-bit Identifier ("Standard Frame ").

<!-- Page 1491 -->

www.ti.com DCAN Control Registers
1491 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.31 IF3Message Control Register (DCAN IF3MCTL)
Figure 27-72. IF3Message Control Register (DCAN IF3MCTL) [offset =14Ch]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
NewDat MsgLst IntPnd UMask TxIE RxIE RmtEn TxRqst
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 4 3 0
EoB Reserved DLC
R-0 R-0 R-0
LEGEND: R=Read; -n=value after reset
Table 27-32. IF3Message Control Register (DCAN IF3MCTL) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 These bitsarealways read as0.Writes have noeffect.
15 NewDat New Data
0 Nonew data hasbeen written intothedata portion ofthismessage object bytheMessage Handler
since thelasttime thisflagwas cleared bytheCPU.
1 The Message Handler ortheCPU haswritten new data intothedata portion ofthismessage
object.
14 MsgLst Message Lost (only valid formessage objects with direction =receive).
0 Nomessage lostsince thelasttime when thisbitwas reset bytheCPU.
1 The Message Handler stored anew message intothisobject when NewDat was stillset,sothe
previous message hasbeen overwritten.
13 IntPnd Interrupt Pending.
0 This message object isnotthesource ofaninterrupt.
1 This message object isthesource ofaninterrupt. The interrupt identifier intheinterrupt register will
point tothismessage object ifthere isnoother interrupt source with higher priority.
12 UMask Use Acceptance Mask.
0 Mask isignored.
1 Use Mask (Msk[28:0], MXtd, andMDir) foracceptance filtering.
IftheUMask bitissetto1,themessage object 'smask bitshave tobeprogrammed during
initialization ofthemessage object before MsgVal issetto1.
11 TxIE Transmit interrupt enable.
0 IntPnd willnotbetriggered after thesuccessful transmission ofaframe.
1 IntPnd willbetriggered after thesuccessful transmission ofaframe.
10 RxIE Receive interrupt enable.
0 IntPnd willnotbetriggered after thesuccessful transmission ofaframe.
1 IntPnd willbetriggered after thesuccessful transmission ofaframe.
9 RmtEn Remote enable.
0 Atthereception ofaRemote Frame, TxRqst isnotchanged.
1 Atthereception ofaRemote Frame, TxRqst isset.
8 TxRqst Transmit request.
0 This message object isnotwaiting foratransmission.
1 The transmission ofthismessage object isrequested andnotyetdone.

<!-- Page 1492 -->

DCAN Control Registers www.ti.com
1492 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-32. IF3Message Control Register (DCAN IF3MCTL) Field Descriptions (continued)
Bit Field Value Description
7 EoB End ofBlock
0 The message object ispart ofaFIFO Buffer block andisnotthelastmessage object oftheFIFO
Buffer block.
1 The message object isasingle message object orthelastmessage object inaFIFO Buffer block.
Note: This bitisused toconcatenate multiple message objects tobuild aFIFO Buffer. Forsingle
message objects (not belonging toaFIFO Buffer), thisbitmust always besetto1.
6-4 Reserved 0 These bitsarealways read as0.Writes have noeffect.
3-0 DLC Data Length Code
0-8h Data Frame has0-8data bits.
9h-Fh Data Frame has8data bytes.
Note: The Data Length Code ofamessage object must bedefined thesame asinallthe
corresponding objects with thesame identifier atother nodes. When themessage handler stores a
data frame, itwillwrite theDLC tothevalue given bythereceived message.
27.17.32 IF3Data AandData BRegisters (DCAN IF3DATA/DATB)
The data bytes ofCAN messages arestored intheIF3registers inthefollowing order.
InaCAN Data Frame, Data 0isthefirst, andData 7isthelastbyte tobetransmitted orreceived. In
CAN's serial bitstream, theMSB ofeach byte willbetransmitted first.
Figure 27-73. IF3Data ARegister (DCAN IF3DATA) [offset =150h]
31 24 23 16
Data 3 Data 2
R-0 R-0
15 8 7 0
Data 1 Data 0
R-0 R-0
LEGEND: R=Read; -n=value after reset
Figure 27-74. IF3Data BRegister (DCAN IF3DATB) [offset =154h]
31 24 23 16
Data 7 Data 6
R/WP-0 R/WP-0
15 8 7 0
Data 5 Data 4
R-0 R-0
LEGEND: R=Read; -n=value after reset

<!-- Page 1493 -->

www.ti.com DCAN Control Registers
1493 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.33 IF3Update Enable Registers (DCAN IF3UPD12 toDCAN IF3UPD78)
The automatic update functionality oftheIF3register setcanbeconfigured foreach message object. A
message object isenabled forautomatic IF3update, ifthededicated IF3UpdEn flagisset.This means
thatanactive NewDat flagofthismessage object (forexample, duetoreception ofaCAN frame) will
trigger anautomatic copy ofthewhole message object toIF3register set.
NOTE: IF3Update enable should notbesetfortransmit objects.
Figure 27-75. IF3Update Enable 12Register (DCAN IF3UPD12) [offset =160h]
31 0
IF3UpdEn[32:1]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-76. IF3Update Enable 34Register (DCAN IF3UPD34) [offset =164h]
31 0
IF3UpdEn[64:33]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-77. IF3Update Enable 56Register (DCAN IF3UPD56) [offset =168h]
31 0
IF3UpdEn[96:65]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Figure 27-78. IF3Update Enable 78Register (DCAN IF3UPD78) [offset =16Ch]
31 0
IF3UpdEn[128:97]
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 27-33. IF3Update Control Register Field Descriptions
Bit Name Value Description
31-0 IF3UpdEn[128:1] IF3Update Enabled (forallmessage objects).
0 Automatic IF3update isdisabled forthismessage object.
1Automatic IF3update isenabled forthismessage object. Amessage object isscheduled tobe
copied toIF3register set,ifNewDat flagofthemessage object isactive.

<!-- Page 1494 -->

DCAN Control Registers www.ti.com
1494 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) Module27.17.34 CAN TXIOControl Register (DCAN TIOC)
The CAN_TX pinoftheDCAN module canbeused asgeneral-purpose IOpinifCAN function isnot
needed.
NOTE: The values oftheIOControl registers areonly writable ifInitbitofCAN Control Register is
set.
The OD, Func, Dir,andOutbitsoftheCAN TXIOControl register areforced tocertain
values when InitbitofCAN Control Register isreset (see bitdescriptions).
Figure 27-79. CAN TXIOControl Register (DCAN TIOC) [offset =1E0h]
31 19 18 17 16
Reserved PU PD OD
R-0 R/W-D R/W-D R/WP-0
15 4 3 2 1 0
Reserved Func Dir Out In
R-0 R/WP-0 R/WP-0 R/WP-0 R-U
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byInitbit);D=Device-dependent; -n=value after reset
Table 27-34. CAN TXIOControl Register (DCAN TIOC) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 These bitsarealways read as0.Writes have noeffect.
18 PU CAN_TX Pullup/Pulldown select. This bitisonly active when CAN_TX isconfigured tobeaninput.
0 CAN_TX Pulldown isselected, when pulllogic isactive (PD =0).
1 CAN_TX Pullup isselected, when pulllogic isactive (PD =0).
17 PD CAN_TX pulldisable. This bitisonly active when CAN_TX isconfigured tobeaninput.
0 CAN_TX pullisactive.
1 CAN_TX pullisdisabled.
16 OD CAN_TX open drain enable. This bitisonly active when CAN_TX isconfigured tobeinGIO mode
(TIOC.Func =0).
0 The CAN_TX pinisconfigured inpush/pull mode.
1 The CAN_TX pinisconfigured inopen drain mode.
Forced to0,ifInitbitofCAN control register isreset.
15-4 Reserved 0 These bitsarealways read as0.Writes have noeffect.
3 Func CAN_TX function. This bitchanges thefunction oftheCAN_TX pin.
0 CAN_TX pinisinGIO mode.
1 CAN_TX pinisinfunctional mode (asanoutput totransmit CAN data).
Forced to1,ifInitbitofCAN control register isreset.
2 Dir CAN_TX data direction. This bitcontrols thedirection oftheCAN_TX pinwhen itisconfigured tobe
inGIO mode only (TIOC.Func =0).
0 The CAN_TX pinisaninput.
1 The CAN_TX pinisanoutput.
Forced to1,ifInitbitofCAN control register isreset.
1 Out CAN_TX data outwrite. This bitisonly active when CAN_TX pinisconfigured tobeinGIO mode
(TIOC.Func =0)andconfigured tobeanoutput pin(TIOC.Dir =1).The value ofthisbitindicates
thevalue tobeoutput totheCAN_TX pin.
0 The CAN_TX pinisdriven tologic low(0).
1 The CAN_TX pinisatlogic high (1).
Forced toTxoutput oftheCAN Core, ifInitbitofCAN Control register isreset.

<!-- Page 1495 -->

www.ti.com DCAN Control Registers
1495 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-34. CAN TXIOControl Register (DCAN TIOC) Field Descriptions (continued)
Bit Field Value Description
0 In CAN_TX data in.
0 The CAN_TX pinisatlogic low(0).
1 The CAN_TX pinisatlogic high (1).
Note: When CAN_TX pinisconnected toaCAN transceiver, anexternal pullup resistor hastobe
used toensure thattheCAN buswillnotbedisturbed (forexample, while theDCAN module is
reset).
27.17.35 CAN RXIOControl Register (DCAN RIOC)
The CAN_RX pinoftheDCAN module canbeused asgeneral-purpose IOpinifCAN function isnot
needed.
NOTE: The values oftheIOControl registers arewritable only ifInitbitofCAN Control Register is
set.
The OD, Func, andDirbitsoftheCAN RXIOControl register areforced tocertain values
when InitbitofCAN Control Register isreset, seebitdescription.
Figure 27-80. CAN RXIOControl Register (DCAN RIOC) [offset =1E4h]
31 19 18 17 16
Reserved PU PD OD
R-0 R/W-D R/W-D R/WP-0
15 4 3 2 1 0
Reserved Func Dir Out In
R-0 R/WP-0 R/WP-0 R/WP-0 R-U
LEGEND: R/W =Read/Write; R=Read; WP=Protected Write (protected byInitbit);D=value isdevice-dependent; -n=value after reset
Table 27-35. CAN RXIOControl Register (DCAN RIOC) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 These bitsarealways read as0.Writes have noeffect.
18 PU CAN_RX Pullup/Pulldown select. This bitisonly active when CAN_RX isconfigured tobeaninput.
0 CAN_RX Pulldown isselected, when pulllogic isactive (PD =0).
1 CAN_RX Pullup isselected, when pulllogic isactive (PD =0).
17 PD CAN_RX pulldisable. This bitisonly active when CAN_RX isconfigured tobeaninput.
0 CAN_RX pullisactive.
1 CAN_RX pullisdisabled.
16 OD CAN_RX open drain enable. This bitisonly active when CAN_RX isconfigured tobeinGIO mode
(RIOC.Func =0).
0 The CAN_RX pinisconfigured inpush/pull mode.
1 The CAN_RX pinisconfigured inopen drain mode.
Forced to0,ifInitbitofCAN control register isreset.
15-4 Reserved 0 These bitsarealways read as0.Writes have noeffect.
3 Func CAN_RX function. This bitchanges thefunction oftheCAN_RX pin.
0 CAN_RX pinisinGIO mode.
1 CAN_RX pinisinfunctional mode (asaninput toreceive CAN data).
Forced to1,ifInitbitofCAN control register isreset.

<!-- Page 1496 -->

DCAN Control Registers www.ti.com
1496 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedController Area Network (DCAN) ModuleTable 27-35. CAN RXIOControl Register (DCAN RIOC) Field Descriptions (continued)
Bit Field Value Description
2 Dir CAN_RX data direction. This bitcontrols thedirection oftheCAN_RX pinwhen itisconfigured to
beinGIO mode only (RIOC.Func =0).
0 The CAN_RX pinisaninput.
1 The CAN_RX pinisanoutput.
Forced to0,ifInitbitofCAN control register isreset.
1 Out CAN_RX data outwrite. This bitisonly active when CAN_RX pinisconfigured tobeinGIO mode
(RIOC.Func =0)andconfigured tobeanoutput pin(RIOC.Dir =1).The value ofthisbitindicates
thevalue tobeoutput totheCAN_RX pin.
0 The CAN_RX pinisdriven tologic low(0).
1 The CAN_RX pinisatlogic high (1).
0 In CAN_RX data in.
0 The CAN_RX pinisatlogic low(0).
1 The CAN_RX pinisatlogic high (1).
Note: When CAN_RX pinisconnected toaCAN transceiver, anexternal pullup resistor hastobe
used toensure thattheCAN buswillnotbedisturbed (forexample, while theDCAN module is
reset).