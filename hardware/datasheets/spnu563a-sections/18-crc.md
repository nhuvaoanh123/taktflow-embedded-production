# Cyclic Redundancy Check (CRC) Controller

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 625-661

---


<!-- Page 625 -->

625 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleChapter 18
SPNU563A -March 2018
Cyclic Redundancy Check (CRC) Controller Module
This chapter describes thecyclic redundancy check (CRC) controller module.
NOTE: This chapter describes asuperset implementation oftheCRC module thatincludes features
andfunctionality thatrequire DMA. Since notalldevices have DMA capability, consult your
device-specific datasheet todetermine applicability ofthese features andfunctions toyour
device being used.
Topic ........................................................................................................................... Page
18.1 Overview ......................................................................................................... 626
18.2 Module Operation ............................................................................................. 628
18.3 Example .......................................................................................................... 638
18.4 CRC Control Registers ...................................................................................... 641

<!-- Page 626 -->

Overview www.ti.com
626 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.1 Overview
The CRC controller isamodule thatisused toperform CRC (Cyclic Redundancy Check) toverify the
integrity ofmemory system. Asignature representing thecontents ofthememory isobtained when the
contents ofthememory areread intoCRC controller. The responsibility ofCRC controller istocalculate
thesignature forasetofdata andthen compare thecalculated signature value against apre-determined
good signature value. CRC controller supports twochannels toperform CRC calculation onmultiple
memories inparallel andcanbeused onanymemory system.
18.1.1 Features
The CRC controller offers:
*Two channels toperform background signature verification onanymemory sub-system.
*Data compression on8,16,32,and64bitdata size.
*Maximum-length PSA (Parallel Signature Analysis) register constructed based on64bitprimitive
polynomial.
*Each channel hasaCRC Value Register thatcontains thepre-determined CRC value.
*Use timed base event trigger from timer toinitiate DMA data transfer.
*Programmable 20-bit pattern counter perchannel tocount thenumber ofdata patterns for
compression.
*Three modes ofoperation. Auto, Semi-CPU andFull-CPU.
*Foreach channel, CRC canbeperformed either byCRC Controller orbyCPU.
*Automatically perform signature verification without CPU intervention inAUTO mode.
*Generate interrupt toCPU inSemi-CPU mode toallow CPU toperform signature verification itself.
*Generate CRC failinterrupt inAUTO mode ifsignature verification fails.
*Generate Timeout interrupt ifCRC isnotperformed within thetime limit.
*Generate DMA request perchannel toinitiate CRC value transfer.
18.1.2 Block Diagram
Figure 18-1 shows ablock diagram oftheCRC controller.

<!-- Page 627 -->

Write Data
PSA Signature Register
CRC Value Register
=
CRC Fail Interrupt
CRC Overrun Interrupt
CRC Underrun Interrupt
CRC Timeout InterruptTrace Enable
DMA Request64
64
Mode RegRegister File
FSM & Control
HBSTRB[7:0]
CH1_INT
CH2_INT
CH3_INT
CH4_INTCRC_INTRaw Data RegisterBus Matrix Module
64
PSA Sector Signature
Register
20 Bit
Pattern
Count
Preload20 Bit
Pattern
Counter
CRC
Status Bit
24 Bit
Timeout
Preload
Register24 Bit
Time
Out
Counter
16 Bit
Sector
Count
Preload16 Bit
Sector
CounterCRC Interrupt
Generation
LogicDMA
Request
Logic
www.ti.com Overview
627 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleFigure 18-1. CRC Controller Block Diagram ForOne Channel

<!-- Page 628 -->

Module Operation www.ti.com
628 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2 Module Operation
18.2.1 General Operation
There aretwochannels inCRC controller andforeach channel there isamemory mapped PSA (Parallel
Signature Analysis) Signature Register andamemory mapped CRC (Cyclic Redundancy Check) Value
register. Amemory canbeorganized intomultiple sectors with each sector consisting ofmultiple data
patterns. Adata pattern canbe8-,16-, 32-, or64-bit data. CRC module performs thesignature calculation
andcompares thesignature toapre-determined value. The PSA Signature Register compresses an
incoming data pattern intoasignature when itiswritten. When onesector ofdata patterns arewritten into
PSA Signature Register, afinal signature corresponding tothesector isobtained. CRC Value Register
stores thepre-determined signature corresponding toonesector ofdata patterns. The calculated
signature andthepre-determined signature arethen compared toeach other forsignature verification. To
minimize CPU 'sinvolvement, data patterns transfer canbecarried outatthebackground ofCPU using
DMA controller. DMA issetup totransfer data from memory from which thecontents tobeverified tothe
memory mapped PSA Signature Register. When DMA transfers data tothememory mapped PSA
Signature Register, asignature isgenerated. Aprogrammable 20-bit data pattern counter isused foreach
channel todefine thenumber ofdata patterns tocalculate foreach sector. Signature verification canbe
performed automatically byCRC controller inAUTO mode orbyCPU itself inSemi-CPU orFull-CPU
mode. InAUTO mode, aselfsustained CRC signature calculation canbeachieved without anyCPU
intervention.
18.2.2 CRC Modes ofOperation
CRC Controller canoperate inAUTO, Semi-CPU, andFull-CPU modes.
18.2.2.1 AUTO Mode
InAUTO mode, CRC Controller inconjunction with DMA controller canperform CRC totally without CPU
intervention. Asustained transfer ofdata toboth thePSA Signature Register andCRC Value Register are
performed inthebackground ofCPU. When amismatch isdetected, aninterrupt isgenerated toCPU. A
16bitcurrent sector IDregister isprovided toidentify which sector causes aCRC failure.
18.2.2.2 Semi-CPU Mode
InSemi-CPU mode, DMA controller isalso utilized toperform data patterns transfer toPSA Signature
Register. Instead ofperforming signature verification automatically, theCRC controller generates an
compression complete interrupt toCPU after each sector iscompressed. Upon responding totheinterrupt
theCPU performs thesignature verification byreading thecalculated signature stored atthePSA Sector
Signature Register andcompare ittoapre-determined CRC value.
18.2.2.3 FullCPU Mode
InFull-CPU mode, theCPU does thedata patterns transfer andsignature verification allbyitself. When
CPU hasenough throughput, itcanperform data patterns transfer byreading data from thememory
system tothePSA Signature Register. After certain number ofdata patterns arecompressed, theCPU
canread from thePSA Signature Register andcompare thecalculated signature tothepre-determined
CRC signature value. InFull-CPU mode, neither interrupt norDMA request isgenerated. Allcounters are
also disabled.

<!-- Page 629 -->

DataDQ
X0DQ
X2DQ
X1DQ
X3DQ
X4DQ
X63
f(x) = x64+ x4+ x3+ x + 1
www.ti.com Module Operation
629 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.3 PSA Signature Register
The 64-bit PSA Signature Register isbased ontheprimitive polynomial (asinthefollowing equation) to
produce themaximum length LFSR (Linear Feedback Shift Register), asshown inFigure 18-2.
(25)
Figure 18-2. Linear Feedback Shift Register (LFSR)
The serial implementation ofLFSF hasalimitation that, itrequires 'n'clock cycles tocalculate theCRC
values foran'n'bitdata stream. The idea istoproduce thesame CRC value operating onamulti-bit data
stream, aswould occur iftheCRC were computed onebitatatime over thewhole data stream. The
algorithm involves looping tosimulate theshifting, andconcatenating strings tobuild theequations after 'n'
shift.
The parallel CRC calculation based onthepolynomial canbeillustrated inthefollowing HDL code:
for iin63to0loop
NEXT_CRC_VAL(0) :=CRC_VAL(63) xor DATA(i);
for jin1to63loop
case jis
when 1|3|4 =>
NEXT_CRC_VAL(j) :=
CRC_VAL(j -1)xor CRC_VAL(63) xor DATA(i);
when others =>
NEXT_CRC_VAL(j) :=CRC_VAL(j -1);
end case;
end loop;
CRC_VAL :=NEXT_CRC_VAL;
end loop;
NOTE: 1)The inner loop istocalculate thenext value ofeach shift register bitafter onecycle
2)The outer loop istosimulate 64cycles ofshifting. The equation foreach shift register bit
isthus built before itiscompressed intotheshift register.
3)MSB oftheDATA isshifted infirst
There isonePSA Signature Register perCRC channel. PSA Signature Register canbeboth read and
written. When itiswritten, itcaneither compress thedata orjustcapture thedata depending onthestate
ofCHx_MODE bits. IfCHx_MODE=Data Capture, aseed value canbeplanted inthePSA Signature
Register without compression. Other modes other than Data Capture willresult with thedata compressed
byPSA Signature Register when itiswritten. Each channel canbeplanted with different seed value
before compression starts. When PSA Signature Register isread, itgives thecalculated signature.
CRC Controller should beused inconjunction with theonchip DMA controller toproduce optimal system
performance. The incoming data pattern toPSA Signature Register istypically initiated bytheDMA
master. When DMA isproperly setup, itwould read data from thepre-determined memory system and
write them tothememory mapped PSA Signature Register. Each time PSA Signature Register iswritten a
signature isgenerated. CPU itself canalso perform data transfer byreading from thememory system and
perform write operation toPSA Signature Register ifCPU hasenough throughput tohandle data patterns
transfer.

<!-- Page 630 -->

Module Operation www.ti.com
630 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleAfter system reset andwhen AUTO mode isenabled, CRC Controller automatically generates aDMA
request torequest thepre-determined CRC value corresponding tothefirstsector ofmemory tobe
checked.
InAUTO mode, when onesector ofdata patterns iscompressed, thesignature stored atthePSA
Signature Register isfirstcopied tothePSA Sector Signature Register andPSA Signature Register is
then cleared outtoallzeros. Anautomatic signature verification isthen performed bycomparing the
signature stored atthePSA Sector Signature Register totheCRC Value Register. After thecomparison
theCRC Controller cangenerate aDMA request. Upon receiving theDMA request theDMA controller will
update theCRC Value Register bytransferring thenext pre-determined signature value associated with
thenext sector ofmemory system. Ifthesignature verification fails then CRC Controller cangenerate a
CRC failinterrupt.
InFull-CPU mode, noDMA request andinterrupt aregenerated atall.The number ofdata patterns tobe
compressed isdetermined byCPU itself. Full-CPU mode isuseful when DMA controller isnotavailable to
perform background data patterns transfer. The OScanperiodically generate asoftware interrupt toCPU
anduseCPU toaccomplish data transfer andsignature verification.
CRC Controller supports doubleword, word, halfword andbyte access tothePSA Signature Register.
During anon-doubleword write access, allunwritten byte lanes arepadded with zero'sbefore
compression. Note thatcomparison between PSA Sector Signature Register andCRC Value Register is
always in64bitbecause acompressed value isalways expressed in64bit.
There isasoftware reset perchannel forPSA Signature Register. When set,thePSA Signature Register
isreset toallzeros.
PSA Signature Register isreset tozero under thefollowing conditions:
*System reset
*PSA Software reset
*One sector ofdata patterns arecompressed
18.2.4 PSA Sector Signature Register
After onesector ofdata iscompressed, thefinal resulting signature calculated byPSA Signature Register
istransferred tothePSA Sector Signature Register. PSA Signature Register isaread only register.
During Semi-CPU mode, thehost CPU should read from thePSA Sector Signature Register instead of
reading from PSA Signature Register forsignature verification toavoid data coherency issue. The PSA
Signature Register canbeupdated with new signature before thehost CPU isable toretrieve it.
InSemi-CPU mode, noDMA request isgenerated. When onesector ofdata patterns iscompressed, CRC
controller firstgenerates acompression complete interrupt. Responding totheinterrupt, CPU willinthe
ISRread thePSA Sector Signature Register andcompare ittotheknown good signature orwrite the
signature value toanother memory location tobuild asignature file.InSemi-CPU mode, CPU must
perform thesignature verification inamanner toprevent anyoverrun condition. The overrun condition
occurs when thecompression complete interrupt isgenerated after onesector ofdata patterns is
compressed andCPU hasnotread from thePSA Sector Signature Register toperform necessary
signature verification before PSA Sector Signature Register isoverridden with anew value. Anoverrun
interrupt canbeenable togenerate when overrun condition occurs. During Semi-CPU mode, thehost
CPU should read from thePSA Sector Signature Register instead ofreading from PSA Signature Register
forsignature verification toavoid data coherency issue. The PSA Signature Register canbeupdated with
new signature before thehost CPU isable toretrieve it.

<!-- Page 631 -->

www.ti.com Module Operation
631 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.5 CRC Value Register
Associated with each channel there isaCRC Value Register. The CRC Value Register stores thepre-
determined CRC value. After onesector ofdata patterns iscompressed byPSA Signature Register, CRC
Controller canautomatically compare theresulting signature stored atthePSA Sector Signature Register
with thepre-determined value stored attheCRC Value Register ifAUTO mode isenabled. Ifthesignature
verification fails, CRC Controller canbeenabled togenerate anCRC failinterrupt. When thechannel is
setupforSemi-CPU mode, CRC controller firstgenerates acompression complete interrupt toCPU.
Upon servicing theinterrupt, CPU willthen read thePSA Sector Signature Register andthen read the
corresponding CRC value stored atanother location andcompare them. CPU should notread from the
CRC Value Register during Semi-CPU orFull-CPU mode because theCRC Value Register isnotupdated
during these twomodes.
InAUTO mode, forfirstsector 'ssignature, DMA request isgenerated when mode isprogrammed to
AUTO. Forsubsequent sectors, DMA request isgenerated after each sector iscompressed. Responding
totheDMA request, DMA controller reloads theCRC Value Register forthenext sector ofmemory system
tobechecked.
When CRC Value Register isupdated with anew CRC value, aninternal flagissettoindicate thatCRC
Value Register contains themost current value. This flagiscleared when CRC comparison isperformed.
Each time attheendofthefinal data pattern compression ofasector, CRC Controller firstchecks tosee
ifthecorresponding CRC Value Register hasthemost current CRC value stored initbypolling theflag. If
theflagissetthen theCRC comparison canbeperformed. Iftheflagisnotsetthen itmeans theCRC
Value Register contains stale information. ACRC underrun interrupt isgenerated. When anunderrun
condition isdetected, signature verification isnotperformed.
CRC Controller supports doubleword, word, halfword andbyte access totheCRC Value Register. As
noted before comparison between PSA Sector Signature Register andCRC Value Register during AUTO
mode iscarried outin64bit.
18.2.6 Raw Data Register
The raworun-compressed data written tothePSA Signature Register isalso saved intheRaw Data
Register. This register isread only.
18.2.7 Example DMA Controller Setup
DMA controller needs tobesetup properly ineither either AUTO orSemi-CPU mode asDMA controller is
used totransfer data patterns. Hardware oracombination ofhardware andsoftware DMA triggering are
supported.
18.2.7.1 AUTO Mode Using Hardware Timer Trigger
There aretwoDMA channels associated with each CRC channel when inAUTO mode. One DMA channel
issetup totransfer data patterns from thesource memory tothePSA Signature Register. The second
DMA channel issetup totransfer thepre-determined signature totheCRC Value Register. The trigger
source forthefirstDMA channel canbeeither byhardware orbysoftware. Asillustrated inFigure 18-3 a
timer canbeused totrigger aDMA request toinitiate transfer from thesource memory system toPSA
Signature Register. InAUTO mode, CRC Controller also generates DMA request after onesector ofdata
patterns iscompressed toinitiate transfer ofthenext CRC value corresponding tothenext sector of
memory. Thus anew CRC value isalways updated intheCRC Value Register byDMA synchronized to
each sector ofmemory.
Ablock ofmemory system isusually divided intomany sectors. Allsectors arethesame size. The sector
size isprogrammed intheCRC_PCOUNT_REGx andthenumber ofsectors inoneblock isprogrammed
intheCRC_SCOUNT_REGx oftherespective channel. CRC_PCOUNT_REGx multiplies
CRC_SCOUNT_REGx andmultiplies transfer size ofeach data pattern should give thetotal block size in
number ofbytes.

<!-- Page 632 -->

DMA Controller CPU
DMA channel 0
DMA channel 15Ch1PSA Sig Reg
CRC Value Reg
Ch4
CRC Value RegPSA Sig RegDMA channel p
DMA channel q.DMA Request Event Sync.
CRC
ControllerSWDMA Req
HW DMA ReqMemory System
Sector 1
Sector 2
Sector n
Sector 1 CRC value
Sector 2 CRC value
Sector n CRC valueone
block
DMA Controller Timer
DMA channel 0
DMA channel 15Ch1PSA Sig Reg
CRC Value Reg
Ch4
CRC Value RegPSA Sig RegDMA channel p
DMA channel q.DMA Request Event Sync.
CRC
ControllerHWDMA Req
HW DMA ReqMemory System
Sector 1
Sector 2
Sector n
Sector 1 CRC value
Sector 2 CRC value
Sector n CRC valueone
block
Module Operation www.ti.com
632 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleThe total size ofthememory system tobeexamined isalso programmed intherespective transfer count
register inside DMA module. The DMA transfer count register isdivided intotwoparts. They areelement
count andframe count. Note thatanHWDMA request canbeprogrammed totrigger either oneframe or
oneentire block transfer. InFigure 18-3,anHWDMA request from atimer isused asatrigger source to
initiate DMA transfer. Ifallfour CRC channels areactive inAUTO mode then atotal offour DMA requests
would begenerated byCRC Controller.
Figure 18-3. AUTO Mode Using Hardware Timer Trigger
18.2.7.2 AUTO Mode Using Software Trigger
The data patterns transfer canalso beinitiated bysoftware. CPU cangenerate asoftware DMA request to
activate theDMA channel totransfer data patterns from source memory system tothePSA Signature
Register. Togenerate asoftware DMA request CPU needs tosetthecorresponding DMA channel inthe
DMA software trigger register. Note thatjustonesoftware DMA request from CPU isenough tocomplete
theentire data patterns transfer forallsectors. See Figure 18-4 foranillustration.
Figure 18-4. AUTO Mode With Software CPU Trigger

<!-- Page 633 -->

Memory System DMA Controller Timer
DMA channel 0
DMA channel 31Ch1PSA Reg
CRC Reg
Ch4
CRC RegPSA RegSector 1
Sector 2
Sector nDMA channel p
DMA channel q.DMA Request Event Sync.
CRC
ControllerHWDMA Req
one
block
www.ti.com Module Operation
633 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.7.3 Semi-CPU Mode Using Hardware Timer Trigger
During semi-CPU mode, noDMA request isgenerated byCRC controller. Therefore, noDMA channel is
allocated toupdate CRC Value Register. CPU should notread from CRC Value Register insemi-CPU
mode asitcontains stale value. Note thatnosignature verification isperformed atallduring thismode.
Similar toAUTO mode, either byhardware orbysoftware DMA request canbeused asatrigger fordata
patterns transfer. Figure 18-5 illustrates theDMA setup using semi-CPU mode with hardware timer trigger.
Figure 18-5. Semi-CPU Mode With Hardware Timer Trigger
Table 18-1. CRC Modes inWhich DMA Request andCounter Logic areActive orInactive
Mode DMA Request Pattern Counter Sector Counter Timeout Counter
AUTO Active Active Active Active
Semi-CPU Inactive Active Active Active
Full-CPU Inactive Inactive Inactive Inactive
18.2.8 Pattern Count Register
There isa20-bit data pattern counter forevery CRC channel. The data pattern counter isadown counter
andcanbepre-loaded with aprogrammable value stored inthePattern Count Register. When thedata
pattern counter reaches zero, acompression complete interrupt isgenerated inSemi-CPU mode andan
automatic signature verification isperformed inAUTO mode. InAUTO only, DMA request isgenerated to
trigger theDMA controller toupdate theCRC Value Register.
NOTE: The data pattern count should bedivisible bythetotal transfer count asprogrammed inDMA
controller. The total transfer count istheproduct ofelement count andframe count.
18.2.9 Sector Count Register/Current Sector Register
Each channel contains a16bitsector counter. The sector count register stores thenumber ofsectors.
Sector counter isafree running counter andisincremented byoneeach time when onesector ofdata
patterns iscompressed. When thesignature verification fails, thecurrent value stored inthesector
counter issaved intocurrent sector register. Ifsignature verification fails, CPU canread from thecurrent
sector register toidentify thesector which causes theCRC mismatch. Toaidandfacilitate theCPU in
determining thecause ofaCRC failure, itisadvisable tousethefollowing equation during CRC andDMA
setup:
CRC Pattern Count ×CRC Sector Count =DMA Element Count ×DMA Frame Count

<!-- Page 634 -->

Module Operation www.ti.com
634 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleThe current sector register isfrozen from being updated until both thecurrent sector register isread and
CRC failstatus bitiscleared byCPU. IfCPU does notrespond totheCRC failure inatimely manner
before another sector produces asignature verification failure, thecurrent sector register isnotupdated
with thenew sector number. Anoverrun interrupt isgenerate instead. Ifcurrent sector register isalready
frozen with anerroneous sector andemulation isentered with SUSPEND signal goes tohigh then the
register stillremains frozen even itisread.
InSemi-CPU mode, thecurrent sector register isused toindicate thesector forwhich thecompression
complete haslasthappened.
The current sector register isreset when thePSA software reset isenabled.
NOTE: Both data pattern count andsector count registers must begreater than orequal toonefor
thecounters tocount. After reset, pattern count andsector count registers default tozero
andtheassociated counters areinactive.
18.2.10 Interrupt
The CRC controller generates several types ofinterrupts perchannel. Associated with each interrupt,
there isaninterrupt enable bit.Nointerrupt isgenerated inFull-CPU mode.
*Compression complete interrupt
*CRC failinterrupt
*Overrun interrupt
*Underrun interrupt
*Timeout interrupt
Table 18-2. Modes inWhich Interrupt Condition Can Occur
AUTO Semi-CPU Full-CPU
Compression Complete no yes no
CRC Fail yes no no
Overrun yes yes no
Underrun yes no no
Timeout yes yes no
18.2.10.1 Compression Complete Interrupt
Compression complete interrupt isgenerated inSemi-CPU mode only. When thedata pattern counter
reaches zero, thecompression complete flagissetandtheinterrupt isgenerated.
18.2.10.2 CRC FailInterrupt
CRC failinterrupt isgenerated inAUTO mode only. When thesignature verification fails, theCRC failflag
isset,. CPU should take action toaddress thefailcondition andclear theCRC failflagafter itresolves the
CRC mismatch.
18.2.10.3 Overrun Interrupt
Overrun interrupt isgenerated ineither AUTO orSemi-CPU mode. During AUTO mode, ifaCRC failis
detected then thecurrent sector number isrecorded inthecurrent sector register. IfCRC failstatus bitis
notcleared andcurrent sector register isnotread bythehost CPU before another CRC failisdetected for
another sector then anoverrun interrupt isgenerated. During Semi-CPU mode, when thedata pattern
counter finishes counting, itgenerates acompression complete interrupt. Atthesame time thesignature is
copied intothePSA Sector Signature Register. Ifthehost CPU does notread thesignature from PSA
Sector Signature Register before itisupdated again with anew signature value then anoverrun interrupt
isgenerated.

<!-- Page 635 -->

Timer
Time scaleHW DMA req every 10 ms
Data
1098765434321098765443210987654432109876540 ms 10 ms 20 ms 30 ms
3 ms 13 ms 23 ms
Timeout
Counter
WD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-load
Note: No timeout interrupt is generated in this example since each block of data patterns are compressed in 3 ms and DMA does
initiate a block transfer every 10 ms.WD pre-load = watchdog timeout pre-load (CRC_WDTOPLDx)
BC pre-load = block complete timeout pre-load (CRC_BCTOPLDx)
www.ti.com Module Operation
635 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.10.4 Underrun Interrupt
Underrun interrupt only occurs inAUTO mode. The interrupt isgenerated when theCRC Value Register is
notupdated with thecorresponding signature when thedata pattern counter finishes counting. During
AUTO mode, CRC Controller generates DMA request toupdate CRC Value Register insynchronization to
thecorresponding sector ofthememory. Signature verification isalso performed ifunderrun condition is
detected. And CRC failinterrupt isgenerated atthesame time astheunderrun interrupt.
18.2.10.5 Timeout Interrupt
Toensure thatthememory system isexamined within apre-defined time frame andnoloss ofincoming
data there isa24bittimeout counter perCRC channel. The 24bittimeout down counter canbepre-
loaded with twodifferent pre-load values, watchdog timeout pre-load value (CRC_WDTOPLDx) andblock
complete timeout pre-load value (CRC_BCTOPLDx). The timeout counter isclocked byaprescaler clock
which ispermanently running atdivision 64ofHCLK clock.
First pattern ofdata must betransferred bytheDMA before thetimeout counter expires, Watchdog
timeout pre-load register (CRC_WDTOPLDx) isused astimeout counter. Block complete timeout pre-load
register (CRC_BCTOPLDx) isused tocheck ifonecomplete block ofdata patterns arecompressed within
aspecific time frame. The timeout counter isfirstpre-loaded with CRC_WDTOPLDx after either AUTO or
Semi-CPU mode isselected andstarts todown count. Ifthetimeout counter expires before DMA transfers
anydata pattern toPSA Signature Register then atimeout interrupt isgenerated. Anincoming data
pattern before thetimeout counter expires willautomatically pre-load thetimeout counter with
CRC_BCTOPLDx theblock complete timeout pre-load value.
Block complete timeout pre-load value isused tocheck itoneblock ofdata patterns arecompressed
within agiven time limit. Ifthetimeout counter pre-loaded with CRC_BCTOPLDx value expires before one
block ofdata patterns arecompressed atimeout interrupt isgenerated. When oneblock (pattern count x
sector count) ofdata patterns arecompressed before thecounter hasexpired, thecounter ispre-loaded
with CRC_WDTOPLDx value again. Ifthetimeout counter ispre-loaded with zero then thecounter is
disable andnotimeout interrupt isgenerated.
InFigure 18-6,atimer generates DMA request every 10ms totrigger oneblock (pattern count xsector
count) transfer. Since wewant tomake sure thatDMA does start totransfer ablock every 10mswe
would setthefirstpre-load value to10ms inCRC_WDTOPLDx. Wealso want tomake sure thatoneblock
ofdata patterns arecompressed within 4ms. With such arequirement, wewould setthesecond pre-load
value to4ms inCRC_BCTOPLDx register.
Figure 18-6. Timeout Example 1

<!-- Page 636 -->

Timer
Time scaleHW DMA req every 10 ms
Data
1098765434321098765432104432109876544321090 ms 10 ms 20 ms 30 ms
3 ms 15 ms 25 ms
Timeout
Counter
WD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-load
timeout
interrupt
WD pre-load = watchdog timeout pre-load (CRC_WDTOPLDx)
BC pre-load = block complete timeout pre-load (CRC_BCTOPLDx)
Note: Timeout interrupt is generated in this example since DMA can not transfer the second block of data within 10ms time
limit and the reason may be that DMA is set up in fixed priority scheme and DMA is serving other higher priority channels
at the time before it can service the timer request.
Timer
Time scaleHW DMA req every 10 ms
Data
1098765434321010987643210109876432101098760 ms 10 ms 20 ms 30 ms
6 ms 16 ms 26 ms
Timeout
Counter
WD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-loadBC
pre-loadWD
pre-load
time out
interruptime out
interruptime out
interrup
WD pre-load = watchdog timeout pre-load (CRC_WDTOPLDx)
BC pre-load = block complete timeout pre-load (CRC_BCTOPLDx)
Note: Timeout interrupt is generated in this example since each block of data patterns are compressed in 6 ms and this is
out of the 4ms time frame.
Module Operation www.ti.com
636 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleFigure 18-7. Timeout Example 2
Figure 18-8. Timeout Example 3

<!-- Page 637 -->

www.ti.com Module Operation
637 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.10.6 Interrupt Offset Register
CRC Controller only generates oneinterrupt request tointerrupt manager. Ainterrupt offset register is
provided toindicate thesource ofthepending interrupt with highest priority. Table 18-3 shows theoffset
interrupt vector address ofeach interrupt condition inanascending order ofpriority.
Table 18-3. Interrupt Offset Mapping
Offset Value Interrupt Condition
0 Phantom
1h Ch1 CRC Fail
2h Ch2 CRC Fail
3h-8h Reserved
9h Ch1 Compression Complete
Ah Ch2 Compression Complete
Bh-10h Reserved
11h Ch1 Overrun
12h Ch2 Overrun
13h-18h Reserved
19h Ch1 Underrun
1Ah Ch2 Underrun
1Bh-20h Reserved
21h Ch1 Timeout
22h Ch2 Timeout
23h-24h Reserved
18.2.10.7 Error Handling
When aninterrupt isgenerated, host CPU should take appropriate actions toidentify thesource oferror
andrestart therespective channel inDMA andCRC module. Torestart aCRC channel, theuser should
perform thefollowing steps intheISR:
1.Write tosoftware reset bitinCRC_CTRL register toreset therespective PSA Signature Register.
2.Reset theCHx_MODE bitsto00inCRC_CTRL register asData capture mode.
3.SettheCHx_MODE bitsinCRC_CTRL register todesired new mode again.
4.Release software reset.
The host CPU should usebyte write torestart each individual channel.
18.2.11 Power Down Mode
CRC module canbeputintopower down mode when thepower down control bitPWDN isset.The
module wakes upwhen thePWDN bitiscleared.
18.2.12 Emulation
Aread access from aregister infunctional mode cansometimes trigger acertain internal event tofollow.
Forexample, reading aninterrupt offset register triggers anevent toclear thecorresponding interrupt
status flag. During emulation when SUSPEND signal ishigh, aread access from anyregister should only
return theregister contents tothebusandshould nottrigger ormask anyevent asitwould have in
functional mode. This istoprevent debugger from reading theinterrupt offset register during refreshing
screen andcause thecorresponding interrupt status flagtogetcleared. Timeout counters arestopped to
generate timeout interrupts inemulation mode. NoPeripheral Master buserror should begenerated if
reading from theunimplemented locations.

<!-- Page 638 -->

Module Operation www.ti.com
638 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.2.13 Peripheral Bus Interface
CRC isaPeripheral slave module. The register interface issimilar toother peripheral modules. CRC
supports following features:
*Different sizes ofburst operation.
*Aligned andunaligned accesses.
*Abort isgenerated foranyillegal address accesses.
18.3 Example
This section illustrates several oftheways inwhich theCRC Controller canbeutilized toperform CRC.
18.3.1 Example: Auto Mode Using Time Based Event Triggering
Alarge memory area with 2Mbyte (256k doubleword) istobechecked inthebackground ofCPU. CRC is
tobeperformed every 1Kbyte (128 doubleword). Therefore there should be2048 pre-recorded CRC
values. Forillustration purpose, wemap channel 1CRC Value Register toDMA channel 1andchannel 1
PSA Signature Register toDMA channel 2.Assume allDMA transfers arecarried outin64-bit transfer
size.
18.3.1.1 DMA Setup
*SetupDMA channel 1with thestarting address from which thepre-determined CRC values are
stored. Setupthedestination address tothememory mapped channel 1CRC Value Register. Putthe
source address atpost increment addressing mode andputthedestination address atconstant
addressing mode. Usehardware DMA request forchannel 1totrigger aframe transfer.
*SetupDMA channel 2with thesource address from which thecontents ofmemory tobeverified. Set
upthedestination address tothememory mapped channel 1PSA Signature Register. Program the
element transfer count to128andtheframe transfer count to2048. Putthesource address atpost
increment addressing mode andputthedestination address atconstant address mode. Usehardware
DMA request forchannel 2totrigger anentire block transfer.
18.3.1.2 Timer Setup
The timer canbeanygeneral purpose timer which iscapable ofgenerating atime-based DMA request.
*Setuptimer togenerate DMA request associated with DMA channel 2.Forexample, anOScansetup
thetimer togenerate aDMA request every 10ms.

<!-- Page 639 -->

www.ti.com Example
639 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.3.1.3 CRC Setup
*Program thepattern count to128.
*Program thesector count to2048.
*Forexample, wewant theentire 2Mbytes tobecompressed within 5ms. Wecanprogram theblock
complete timeout pre-load (CRC_BCTOPLDx) value to15625 (5ms/(1HCLK period ×64)) ifCRC is
operating at200MHz.
*Enable AUTO mode andallinterrupts.
After AUTO mode isselected, CRC Controller automatically generates aDMA request onchannel 1.
Around thesame time thetimer module also generates aDMA request onDMA channel 2.When thefirst
incoming data pattern arrives atthePSA Signature Register, theCRC Controller willcompress it.After
some time, theDMA controller would update theCRC Value Register with apre-determined value
matching thecalculated signature forthefirstsector of12864bitdata patterns. After onesector ofdata
patterns arecompressed, theCRC Controller generate aCRC failinterrupt ifsignature stored atthePSA
Sector Signature Register does notmatch theCRC Value Register. CRC Controller generates aDMA
request onDMA channel 1when onesector ofdata patterns arecompressed. This routine willcontinue
until theentire 2Mbyte areconsumed. Ifthetimeout counter reached zero before theentire 2Mbytes are
compressed atimeout interrupt isgenerated. After 2MBytes aretransferred, theDMA cangenerate an
interrupt toCPU. The entire operation willcontinue again when DMA responds totheDMA request from
both thetimer andCRC Controller. The CRC isperformed totally without anyCPU intervention.
18.3.2 Example: Auto Mode Without Using Time Based Triggering
Asmall buthighly secured memory area with 1kbytes istobechecked inthebackground ofCPU. CRC is
tobeperformed every 1Kbytes. Therefore there isonly onepre-recorded CRC value. Forillustration
purpose, wemap channel 1CRC Value Register toDMA channel 1andchannel 1PSA Signature
Register toDMA channel 2.Assume alltransfers carried outbyDMA arein64bittransfer size.
18.3.2.1 DMA Setup
*SetupDMA channel 1with thesource address from which thepre-determined CRC value isstored.
Setupthedestination address tothememory mapped channel 1CRC Value Register. Putthesource
address atconstant addressing mode andputthedestination address atconstant addressing mode.
Usehardware DMA request forchannel 1.
*SetupDMA channel 2with thesource address from which thememory area tobeverified. Setupthe
destination address tothememory mapped channel 1PSA Signature Register. Program theelement
transfer count to128andtheframe transfer count to1.Putthesource address atpost increment
addressing mode andputthedestination address atconstant address mode. Generate asoftware
DMA request onchannel 2after CRC hascompleted itssetup. Enable autoinitiation forDMA channel
2.
18.3.2.2 CRC Setup
*Program thepattern count to128.
*Program thesector count to1.
*Leaving thetimeout count register with thereset value ofzero means notimeout interrupt isgenerated.
*Enable AUTO mode andallinterrupts.
After AUTO mode isselected, theCRC Controller automatically generates aDMA request onchannel 1.
Atthesame time theCPU generates asoftware DMA request onDMA channel 2.When thefirst
incoming data pattern arrives atthePSA Signature Register, theCRC Controller willcompress it.After
some time, theDMA controller would update theCRC Value Register with apre-determined value
matching thecalculated signature forthefirstsector of12864bitdata patterns. After onesector ofdata
patterns arecompressed, theCRC Controller generates aCRC failinterrupt ifsignature stored atthePSA
Sector Signature Register does notmatch theCRC Value Register. CRC Controller generates aDMA
request onDMA channel 1again after onesector iscompressed. After 1kbytes aretransferred, theDMA
cangenerate aninterrupt toCPU. Responding totheDMA interrupt CPU canrestart theCRC routine by
generating asoftware DMA request onto channel 2again.

<!-- Page 640 -->

Example www.ti.com
640 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.3.3 Example: Semi-CPU Mode
IfDMA controller isavailable inasystem, theCRC module canalso operate insemi-CPU mode. This
means thatCPU canstillmake useoftheDMA toperform data patterns transfer toCRC controller inthe
background. The difference between semi-CPU mode andAUTO mode isthatCRC controller does not
automatically perform thesignature verification. CRC controllers generates acompression complete
interrupt toCPU when theonesector ofdata patterns arecompressed. CPU needs toperform the
signature verification itself.
Amemory area with 2Mbyte istobeverified with thehelp oftheCPU. CRC operation istobeperformed
every 1Kbyte. Since there are2Mbyte (256k doublewords) ofmemory tobecheck andwewant to
perform aCRC every 1Kbyte (128 doublewords) andtherefore there should be2048 pre-recorded CRC
values. InSemi-CPU mode, theCRC Value Register isnotupdated andcontains indeterminate data.
18.3.3.1 DMA Setup
SetupDMA channel 1with thesource address from which thememory area tobeverified aremapped.
Setupthedestination address tothememory mapped channel 1PSA Signature Register. Putthestarting
address atpost increment addressing mode andputthedestination address atconstant address mode.
Use hardware DMA request totrigger anentire block transfer forchannel 1.Disable autoinitiation forDMA
channel 1.
18.3.3.2 Timer Setup
The timer canbeanygeneral purpose timer which iscapable ofgenerating atime based DMA request.
Setuptimer togenerate DMA request associated with DMA channel 1.Forexample, anOScansetup
thetimer togenerate aDMA request every 10ms.
18.3.3.3 CRC Setup
*Program thepattern count to128.
*Program thesector count to2048.
*Forexample, wewant theentire 2Mbytes tobecompressed within 5ms. Wecanprogram theblock
complete timeout pre-load value to15625 (5ms/(1HCLK period ×64)) ifCRC isoperating at200
MHz.
*Enable Semi-CPU mode andenable allinterrupts.
The timer module firstgenerates aDMA request onDMA channel 1when itisenabled. When thefirst
incoming data pattern arrives atthePSA Signature Register, theCRC controller willcompress it.After one
sector ofdata patterns arecompressed, theCRC controller generate acompression complete interrupt.
Upon responding totheinterrupt theCPU would read from thePSA Sector Signature Register. Itisupto
theCPU onhow todeal with thePSA value justread. Itcancompare ittoaknown signature value orit
canwrite ittoanother memory location tobuild asignature fileoreven transfer thesignature outofthe
device viaSCIorSPI. This routine willcontinue until theentire 2Mbyte areconsumed. The latency ofthe
interrupt response from CPU cancause overrun condition. IfCPU does notread from PSA Sector
Signature Register before thePSA value isoverridden with thesignature ofthenext sector ofmemory, an
overrun interrupt willbegenerated byCRC controller.
18.3.4 Example: Full-CPU Mode
Inasystem without theavailability ofDMA controller, theCRC routine canbeoperated byCPU provided
theCPU hasenough throughput. CPU needs toread from thememory area from which CRC istobe
performed.
Amemory area with 2Mbyte istobechecked with thehelp oftheCPU. CRC verification istobe
performed every 1Kbyte. InCPU mode, theCRC Value Register isnotupdated andcontains
indeterminate data.

<!-- Page 641 -->

www.ti.com CRC Control Registers
641 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.3.4.1 CRC Setup
*Allcontrol registers canbeleftintheir reset state. Only enable Full-CPU mode.
CPU itself reads from thememory andwrite thedata tothePSA Signature Register inside CRC
Controller. When thefirstincoming data pattern arrives atthePSA Signature Register, theCRC Controller
willcompress it.After 2MBytes data patterns arecompressed, CPU canread from thePSA Signature
Register. ItisuptotheCPU onhow todeal with thePSA signature value justread. Itcancompare ittoa
known signature value stored atanother memory location.
18.4 CRC Control Registers
Allregisters areinword boundary. 64,32,16,and8bitwrite accesses aresupported toallregisters. The
base address forthecontrol registers isFE00 0000h forCRC1 andFB00 0000h forCRC2.
Table 18-4. CRC Control Registers
Offset Acronym Register Description Section
0h CRC_CTRL0 CRC Global Control Register Section 18.4.1
8h CRC_CTRL1 CRC Global Control Register 1 Section 18.4.2
10h CRC_CTRL2 CRC Global Control Register 2 Section 18.4.3
18h CRC_INTS CRC Interrupt Enable SetRegister Section 18.4.4
20h CRC_INTR CRC Interrupt Enable Reset Register Section 18.4.5
28h CRC_STATUS CRC Interrupt Status Register Section 18.4.6
30h CRC_INT_OFFS_ET_REG CRC Interrupt Offset Register Section 18.4.7
38h CRC_BUSY CRC Busy Register Section 18.4.8
40h CRC_PCOUNT_REG1 CRC Channel 1Pattern Counter Preload Register Section 18.4.9
44h CRC_SCOUNT_REG1 CRC Channel 1Sector Counter Preload Register Section 18.4.10
48h CRC_CURSEC_REG1 CRC Channel 1Current Sector Register Section 18.4.11
4Ch CRC_WDTOPLD1 CRC Channel 1Watchdog Timeout Preload Register Section 18.4.12
50h CRC_BCTOPLD1 CRC Channel 1Block Complete Timeout Preload Register Section 18.4.13
60h PSA_SIGREGL1 Channel 1PSA Signature Low Register Section 18.4.14
64h PSA_SIGREGH1 Channel 1PSA Signature High Register Section 18.4.15
68h CRC_REGL1 Channel 1CRC Value Low Register Section 18.4.16
6Ch CRC_REGH1 Channel 1CRC Value High Register Section 18.4.17
70h PSA_SECSIGREGL1 Channel 1PSA Sector Signature Low Register Section 18.4.18
74h PSA_SECSIGREGH1 Channel 1PSA Sector Signature High Register Section 18.4.19
78h RAW_DATAREGL1 Channel 1Raw Data Low Register Section 18.4.20
7Ch RAW_DATAREGH1 Channel 1Raw Data High Register Section 18.4.21
80h CRC_PCOUNT_REG2 CRC Channel 2Pattern Counter Preload Register Section 18.4.22
84h CRC_SCOUNT_REG2 CRC Channel 2Sector Counter Preload Register Section 18.4.23
88h CRC_CURSEC_REG2 CRC Current Sector Register 2 Section 18.4.24
8Ch CRC_WDTOPLD2 CRC Channel 2Watchdog Timeout Preload Register A Section 18.4.25
90h CRC_BCTOPLD2 CRC Channel 2Block Complete Timeout Preload Register B Section 18.4.26
A0h PSA_SIGREGL2 Channel 2PSA Signature Low Register Section 18.4.27
A4h PSA_SIGREGH2 Channel 2PSA Signature High Register Section 18.4.28
A8h CRC_REGL2 Channel 2CRC Value Low Register Section 18.4.29
ACh CRC_REGH2 Channel 2CRC Value High Register Section 18.4.30
B0h PSA_SECSIGREGL2 Channel 2PSA Sector Signature Low Register Section 18.4.31
B4h PSA_SECSIGREGH2 Channel 2PSA Sector Signature High Register Section 18.4.32
B8h RAW_DATAREGL2 Channel 2Raw Data Low Register Section 18.4.33
BCh RAW_DATAREGH2 Channel 2Raw Data High Register Section 18.4.34

<!-- Page 642 -->

CRC Control Registers www.ti.com
642 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.1 CRC Global Control Register 0(CRC_CTRL0)
Figure 18-9. CRC Global Control Register 0(CRC_CTRL0) [offset =00h]
31 16
Reserved
R-0
15 9 8
Reserved CH2_PSA_SWREST
R-0 R/W-0
7 1 0
Reserved CH1_PSA_SWREST
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-5. CRC Global Control Register 0(CRC_CTRL0) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 CH2_PSA_SWREST Channel 2PSA Software Reset. When set,thePSA Signature Register isreset toallzero.
Software reset does notreset software reset bititself. Therefore, CPU isrequired toclear
thisbitbywriting a0.
0 PSA Signature Register isnotreset.
1 PSA Signature Register isreset.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 CH1_PSA_SWREST Channel 1PSA Software Reset. When set,thePSA Signature Register isreset toallzero.
Software reset does notreset software reset bititself. Therefore, CPU isrequired toclear
thisbitbywriting a0.
0 PSA Signature Register isnotreset.
1 PSA Signature Register isreset.
18.4.2 CRC Global Control Register (CRC_CTRL1)
Figure 18-10. CRC Global Control Register 1(CRC_CTRL1) [offset =08h]
31 16
Reserved
R-0
15 1 0
Reserved PWDN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-6. CRC Global Control Register 1(CRC_CTRL1) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PWDN Power Down. When set,CRC module isputinpower-down mode.
0 CRC isnotinpower-down mode.
1 CRC isinpower-down mode.

<!-- Page 643 -->

www.ti.com CRC Control Registers
643 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.3 CRC Global Control Register 2(CRC_CTRL2)
Figure 18-11. CRC Global Control Register 2(CRC_CTRL2) [offset =10h]
31 16
Reserved
R-0
15 10 9 8
Reserved CH2_MODE
R-0 R/WP-0
7 2 1 0
Reserved CH1_MODE
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 18-7. CRC Global Control Register 2(CRC_CTRL2) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 CH2_MODE Channel 2Mode Selection.
0 Data Capture mode. Inthismode, thePSA Signature Register does notcompress data when it
iswritten. Any data written toPSA Signature Register issimply captured byPSA Signature
Register without anycompression. This mode canbeused toplant seed value intothePSA
register.
1h AUTO Mode
2h Semi-CPU Mode
3h Full-CPU Mode
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 CH1_MODE Channel 1Mode Selection.
0 Data Capture mode. Inthismode, thePSA Signature Register does notcompress data when it
iswritten. Any data written toPSA Signature Register issimply captured byPSA Signature
Register without anycompression. This mode canbeused toplant seed value intothePSA
register.
1h AUTO Mode
2h Semi-CPU Mode
3h Full-CPU Mode

<!-- Page 644 -->

CRC Control Registers www.ti.com
644 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.4 CRC Interrupt Enable SetRegister (CRC_INTS)
Figure 18-12. CRC Interrupt Enable SetRegister (CRC_INTS) [offset =18h]
31 16
Reserved
R-0
15 13 12 11 10 9 8
Reserved CH2_
TIMEOUTENSCH2_
UNDERENSCH2_
OVERENSCH2_
CRCFAILENSCH2_
CCITENS
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 5 4 3 2 1 0
Reserved CH1_
TIMEOUTENSCH1_
UNDERENSCH1_
OVERENSCH1_
CRCFAILENSCH1_
CCITENS
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 18-8. CRC Interrupt Enable SetRegister (CRC_INTS) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 CH2_TIMEOUTENS Channel 2Timeout Interrupt Enable Bit.
User andPrivileged mode (read):
0 Timeout Interrupt isdisabled.
1 Timeout Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Timeout Interrupt isenabled.
11 CH2_UNDERENS Channel 2Underrun Interrupt Enable Bit.
User andPrivileged mode (read):
0 Underrun Interrupt isdisabled.
1 Underrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Underrun Interrupt isenabled.
10 CH2_OVERENS Channel 2Overrun Interrupt Enable Bit.
User andPrivileged mode (read):
0 Overrun Interrupt isdisabled.
1 Overrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Overrun Interrupt isenabled.
9 CH2_CRCFAILENS Channel 2CRC Compare FailInterrupt Enable Bit.
User andPrivileged mode (read):
0 CRC FailInterrupt isdisabled.
1 CRC FailInterrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 CRC FailInterrupt isenabled.

<!-- Page 645 -->

www.ti.com CRC Control Registers
645 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleTable 18-8. CRC Interrupt Enable SetRegister (CRC_INTS) Field Descriptions (continued)
Bit Field Value Description
8 CH2_CCITENS Channel 2Compression Complete Interrupt Enable Bit.
User andPrivileged mode (read):
0 Compression Complete Interrupt isdisabled.
1 Compression Complete Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Compression Complete Interrupt isenabled.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 CH1_TIMEOUTENS Channel 1Timeout Interrupt Enable Bit.
User andPrivileged mode (read):
0 Timeout Interrupt isdisabled.
1 Timeout Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Timeout Interrupt isenabled.
3 CH1_UNDERENS Channel 1Underrun Interrupt Enable Bit.
User andPrivileged mode (read):
0 Underrun Interrupt isdisabled.
1 Underrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Underrun Interrupt isenabled.
2 CH1_OVERENS Channel 1Overrun Interrupt Enable Bit.
User andPrivileged mode (read):
0 Overrun Interrupt isdisabled.
1 Overrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Overrun Interrupt isenabled.
1 CH1_CRCFAILENS Channel 1CRC Compare FailInterrupt Enable Bit.
User andPrivileged mode (read):
0 CRC FailInterrupt isdisabled.
1 CRC FailInterrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 CRC FailInterrupt isenabled.
0 CH1_CCITENS Channel 1Compression Complete Interrupt Enable Bit.
User andPrivileged mode (read):
0 Compression Complete Interrupt isdisabled.
1 Compression Complete Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Compression Complete Interrupt isenabled.

<!-- Page 646 -->

CRC Control Registers www.ti.com
646 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.5 CRC Interrupt Enable Reset Register (CRC_INTR)
Figure 18-13. CRC Interrupt Enable Reset Register (CRC_INTR) [offset =20h]
31 16
Reserved
R-0
15 13 12 11 10 9 8
Reserved CH2_
TIMEOUTENRCH2_
UNDERENRCH2_
OVERENRCH2_
CRCFAILENRCH2_
CCITENR
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 5 4 3 2 1 0
Reserved CH1_
TIMEOUTENRCH1_
UNDERENRCH1_
OVERENRCH1_
CRCFAILENRCH1_
CCITENR
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 18-9. CRC Interrupt Enable Reset Register (CRC_INTR) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 CH2_TIMEOUTENR Channel 2Timeout Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Timeout Interrupt isdisabled.
1 Timeout Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Timeout Interrupt isdisabled.
11 CH2_UNDERENR Channel 2Underrun Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Underrun Interrupt isdisabled.
1 Underrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Underrun Interrupt isdisabled.
10 CH2_OVERENR Channel 2Overrun Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Overrun Interrupt isdisabled.
1 Overrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Overrun Interrupt isdisabled.
9 CH2_CRCFAILENR Channel 2CRC Compare FailInterrupt Enable Reset Bit.
User andPrivileged mode (read):
0 CRC FailInterrupt disabled.
1 CRC FailInterrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 CRC FailInterrupt isdisabled.

<!-- Page 647 -->

www.ti.com CRC Control Registers
647 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleTable 18-9. CRC Interrupt Enable Reset Register (CRC_INTR) Field Descriptions (continued)
Bit Field Value Description
8 CH2_CCITENR Channel 2Compression Complete Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Compression Complete Interrupt isdisabled.
1 Compression Complete Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Compression Complete Interrupt isdisabled.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 CH1_TIMEOUTENR Channel 1Timeout Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Timeout Interrupt isdisabled.
1 Timeout Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Timeout Interrupt isdisabled.
3 CH1_UNDERENR Channel 1Underrun Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Underrun Interrupt isdisabled.
1 Underrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Underrun Interrupt isdisabled.
2 CH1_OVERENR Channel 1Overrun Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Overrun Interrupt isdisabled.
1 Overrun Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Overrun Interrupt isdisabled.
1 CH1_CRCFAILENR Channel 1CRC Compare FailInterrupt Enable Reset Bit.
User andPrivileged mode (read):
0 CRC FailInterrupt isdisabled.
1 CRC FailInterrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 CRC FailInterrupt isdisabled.
0 CH1_CCITENR Channel 1Compression Complete Interrupt Enable Reset Bit.
User andPrivileged mode (read):
0 Compression Complete Interrupt isdisabled.
1 Compression Complete Interrupt isenabled.
Privileged mode (write):
0 Noeffect.
1 Compression Complete Interrupt isdisabled.

<!-- Page 648 -->

CRC Control Registers www.ti.com
648 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.6 CRC Interrupt Status Register (CRC_STATUS)
Figure 18-14. CRC Interrupt Status Register (CRC_STATUS) [offset =28h]
31 16
Reserved
R-0
15 13 12 11 10 9 8
Reserved CH2_TIMEOUT CH2_UNDER CH2_OVER CH2_CRCFAIL CH2_CCIT
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
7 5 4 3 2 1 0
Reserved CH1_TIMEOUT CH1_UNDER CH1_OVER CH1_CRCFAIL CH1_CCIT
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 18-10. CRC Interrupt Status Register (CRC_STATUS) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12 CH2_TIMEOUT Channel 2CRC Timeout Interrupt Status Flag. This bitissetinboth AUTO andSemi-CPU
mode.
User andPrivileged mode (read):
0 Notimeout interrupt isactive.
1 Timeout interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
11 CH2_UNDER Channel 2CRC Underrun Interrupt Status Flag. This bitissetinAUTO mode only.
User andPrivileged mode (read):
0 NoUnderrun Interrupt isactive.
1 Underrun Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
10 CH2_OVER Channel 2CRC Overrun Interrupt Status Flag. This bitissetineither AUTO orSemi-CPU
mode.
User andPrivileged mode (read):
0 NoOverrun Interrupt isactive.
1 Overrun Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
9 CH2_CRCFAIL Channel 2CRC Compare FailInterrupt Status Flag. This bitissetinAUTO mode only.
User andPrivileged mode (read):
0 NoCRC FailInterrupt isactive
1 CRC FailInterrupt isactive
Privileged mode (write):
0 Noeffect
1 Bitiscleared

<!-- Page 649 -->

www.ti.com CRC Control Registers
649 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller ModuleTable 18-10. CRC Interrupt Status Register (CRC_STATUS) Field Descriptions (continued)
Bit Field Value Description
8 CH2_CCIT Channel 2CRC Pattern Compression Complete Interrupt Status Flag. This bitisonly setin
Semi-CPU mode.
User andPrivileged mode (read):
0 NoCompression Complete Interrupt isactive.
1 Compression Complete Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 CH1_TIMEOUT Channel 1CRC Timeout Interrupt Status Flag.
User andPrivileged mode (read):
0 Notimeout interrupt isactive.
1 Timeout interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
3 CH1_UNDER Channel 1Underrun Interrupt Status Flag.
User andPrivileged mode (read):
0 NoUnderrun Interrupt isactive.
1 Underrun Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
2 CH1_OVER Channel 1Overrun Interrupt Status Flag.
User andPrivileged mode (read):
0 NoOverrun Interrupt isactive.
1 Overrun Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
1 CH1_CRCFAIL Channel 1CRC Compare FailInterrupt Status Flag.
User andPrivileged mode (read):
0 NoCRC FailInterrupt isactive.
1 CRC FailInterrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.
0 CH1_CCIT Channel 1CRC Pattern Compression Complete Interrupt Status Flag.
User andPrivileged mode (read):
0 NoCompression Complete Interrupt isactive.
1 Compression Complete Interrupt isactive.
Privileged mode (write):
0 Noeffect.
1 Bitiscleared.

<!-- Page 650 -->

CRC Control Registers www.ti.com
650 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.7 CRC Interrupt Offset (CRC_INT_OFFSET_REG)
Figure 18-15. CRC Interrupt Offset (CRC_INT_OFFSET_REG) [offset =30h]
31 16
Reserved
R-0
15 8 7 0
Reserved OFSTREG
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 18-11. CRC Interrupt Offset (CRC_INT_OFFSET_REG) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 OFSTREG CRC Interrupt Offset. This register indicates thehighest priority pending interrupt vector address.
Reading theoffset register automatically clears therespective interrupt flag.
0 Phantom
1h Ch1 CRC Fail
2h Ch2 CRC Fail
3h-8h Reserved
9h Ch1 Compression Complete
Ah Ch2 Compression Complete
Bh-10h Reserved
11h Ch1 Overrun
12h Ch2 Overrun
13h-18h Reserved
19h Ch1 Underrun
1Ah Ch2 Underrun
1Bh-20h Reserved
21h Ch1 Timeout
22h Ch2 Timeout
23h-FFh Reserved

<!-- Page 651 -->

www.ti.com CRC Control Registers
651 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.8 CRC Busy Register (CRC_BUSY)
Figure 18-16. CRC Busy Register (CRC_BUSY) [offset =38h]
31 16
Reserved
R-0
15 9 8 7 1 0
Reserved CH2_BUSY Reserved CH1_BUSY
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 18-12. CRC Busy Register (CRC_BUSY) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 CH2_BUSY CH2_BUSY. During AUTO orSemi-CPU mode, thebusy flagissetwhen thefirstdata pattern of
theblock iscompressed andremains setuntil thelastdata pattern oftheblock iscompressed. The
flagiscleared when thelastdata pattern oftheblock iscompressed.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 CH1_BUSY CH1_BUSY. During AUTO orSemi-CPU mode, thebusy flagissetwhen thefirstdata pattern of
theblock iscompressed andremains setuntil thelastdata pattern oftheblock iscompressed. The
flagiscleared when thelastdata pattern oftheblock iscompressed.
18.4.9 CRC Pattern Counter Preload Register 1(CRC_PCOUNT_REG1)
Figure 18-17. CRC Pattern Counter Preload Register 1(CRC_PCOUNT_REG1) [offset =40h]
31 20 19 16
Reserved CRC_PAT_COUNT1
R-0 R/W-0
15 0
CRC_PAT_COUNT1
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-13. CRC Pattern Counter Preload Register 1(CRC_PCOUNT_REG1) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-0 CRC_PAT_COUNT1 Channel 1Pattern Counter Preload Register. This register contains thenumber ofdata
patterns inonesector tobecompressed before aCRC isperformed.

<!-- Page 652 -->

CRC Control Registers www.ti.com
652 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.10 CRC Sector Counter Preload Register 1(CRC_SCOUNT_REG1)
Figure 18-18. CRC Sector Counter Preload Register 1(CRC_SCOUNT_REG1) [offset =44h]
31 16
Reserved
R-0
15 0
CRC_SEC_COUNT1
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-14. CRC Sector Counter Preload Register 1(CRC_SCOUNT_REG1) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 CRC_SEC_COUNT1 Channel 1Sector Counter Preload Register. This register contains thenumber ofsectors in
oneblock ofmemory.
18.4.11 CRC Current Sector Register 1(CRC_CURSEC_REG1)
Figure 18-19. CRC Current Sector Preload Register 1(CRC_CURSEC_REG1) [offset =48h]
31 16
Reserved
R-0
15 0
CRC_CURSEC1
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-15. CRC Current Sector Register 1(CRC_CURSEC_REG1) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 CRC_CURSEC1 Channel 1Current Sector IDRegister. InAUTO mode, thisregister contains thecurrent sector
number ofwhich thesignature verification fails. The sector counter isafree running upcounter.
When asector fails, theerroneous sector number islogged intocurrent sector IDregister and
theCRC failinterrupt isgenerated The sector IDregister isfrozen until itisread andtheCRC
failstatus bitiscleared byCPU. While itisfrozen, itdoes notcapture another erroneous sector
number. When thiscondition happens, anoverrun interrupt isgenerated instead. Once the
register isread andtheCRC failinterrupt flagiscleared itcancapture new erroneous sector
number. InSemi-CPU mode, thisregister isused toindicate thesector number forwhich the
compression complete haslasthappened.

<!-- Page 653 -->

www.ti.com CRC Control Registers
653 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.12 CRC Channel 1Watchdog Timeout Preload Register A(CRC_WDTOPLD1)
Figure 18-20. CRC Channel 1Watchdog Timeout Preload Register A(CRC_WDTOPLD1)
[offset =4Ch]
31 24 23 16
Reserved CRC_WDTOPLD1
R-0 R/W-0
15 0
CRC_WDTOPLD1
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-16. CRC Channel 1Watchdog Timeout Preload Register A(CRC_WDTOPLD1)
Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 CRC_WDTOPLD1 Channel 1Watchdog Timeout Counter Preload Register. This register contains thenumber of
clock cycles within which theDMA must transfer thenext block ofdata patterns. InSemi-CPU
mode, thisregister isused toindicate thesector number forwhich thecompression complete
haslasthappened.
18.4.13 CRC Channel 1Block Complete Timeout Preload Register B(CRC_BCTOPLD1)
Figure 18-21. CRC Channel 1Block Complete Timeout Preload Register B(CRC_BCTOPLD1)
[offset =50h]
31 24 23 16
Reserved CRC_BCTOPLD1
R-0 R/W-0
15 0
CRC_BCTOPLD1
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-17. CRC Channel 1Block Complete Timeout Preload Register B(CRC_BCTOPLD1)
Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 CRC_BCTOPLD1 Channel 1Block Complete Timeout Counter Preload Register. This register contains the
number ofclock cycles within which theCRC foranentire block needs tocomplete before a
timeout interrupt isgenerated.

<!-- Page 654 -->

CRC Control Registers www.ti.com
654 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.14 Channel 1PSA Signature Low Register (PSA_SIGREGL1)
Figure 18-22. Channel 1PSA Signature Low Register (PSA_SIGREGL1) [offset =60h]
31 0
PSASIG1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-18. Channel 1PSA Signature Low Register (PSA_SIGREGL1) Field Descriptions
Bit Field Description
31-0 PSASIG1 Channel 1PSA Signature Low Register. This register contains thevalue stored atPSASIG1[31:0] register.
18.4.15 Channel 1PSA Signature High Register (PSA_SIGREGH1)
Figure 18-23. Channel 1PSA Signature High Register (PSA_SIGREGH1) [offset =64h]
31 0
PSASIG1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-19. Channel 1PSA Signature High Register (PSA_SIGREGH1) Field Descriptions
Bit Field Description
31-0 PSASIG1 Channel 1PSA Signature High Register. This register contains thevalue stored atPSASIG1[63:32] register.
18.4.16 Channel 1CRC Value Low Register (CRC_REGL1)
Figure 18-24. Channel 1CRC Value Low Register (CRC_REGL1) [offset =68h]
31 0
CRC1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-20. Channel 1CRC Value Low Register (CRC_REGL1) Field Descriptions
Bit Field Description
31-0 CRC1 Channel 1CRC Value Low Register. This register contains thecurrent known good signature value stored at
CRC1[31:0] register.

<!-- Page 655 -->

www.ti.com CRC Control Registers
655 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.17 Channel 1CRC Value High Register (CRC_REGH1)
Figure 18-25. Channel 1CRC Value High Register (CRC_REGH1) [offset =6Ch]
31 0
CRC1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-21. Channel 1CRC Value High Register (CRC_REGH1) Field Descriptions
Bit Field Description
31-0 CRC1 Channel 1CRC Value Low Register. This register contains thecurrent known good signature value stored at
CRC1[63:32] register.
18.4.18 Channel 1PSA Sector Signature Low Register (PSA_SECSIGREGL1)
Figure 18-26. Channel 1PSA Sector Signature Low Register (PSA_SECSIGREGL1) [offset =70h]
31 0
PSASECSIG1
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-22. Channel 1PSA Sector Signature Low Register (PSA_SECSIGREGL1)
Field Descriptions
Bit Field Description
31-0 PSASECSIG1 Channel 1PSA Sector Signature Low Register. This register contains thevalue stored at
PSASECSIG1[31:0] register.
18.4.19 Channel 1PSA Sector Signature High Register (PSA_SECSIGREGH1)
Figure 18-27. Channel 1PSA Sector Signature High Register (PSA_SECSIGREGH1) [offset =74h]
31 0
PSASECSIG1
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-23. Channel 1PSA Sector Signature High Register (PSA_SECSIGREGH1)
Field Descriptions
Bit Field Description
31-0 PSASECSIG1 Channel 1PSA Sector Signature High Register. This register contains thevalue stored at
PSASECSIG1[63:32] register.

<!-- Page 656 -->

CRC Control Registers www.ti.com
656 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.20 Channel 1Raw Data Low Register (RAW_DATAREGL1)
Figure 18-28. Channel 1Raw Data Low Register (RAW_DATAREGL1) [offset =78h]
31 0
RAW_DATA1
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-24. Channel 1Raw Data Low Register (RAW_DATAREGL1) Field Descriptions
Bit Field Description
31-0 RAW_DATA1 Channel 1Raw Data Low Register. This register contains bits31:0 oftheuncompressed rawdata.
18.4.21 Channel 1Raw Data High Register (RAW_DATAREGH1)
Figure 18-29. Channel 1Raw Data High Register (RAW_DATAREGH1) [offset =7Ch]
31 0
RAW_DATA1
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-25. Channel 1Raw Data High Register (RAW_DATAREGH1) Field Descriptions
Bit Field Description
31-0 RAW_DATA1 Channel 1Raw Data High Register. This register contains bits63:32 oftheuncompressed rawdata.
18.4.22 CRC Pattern Counter Preload Register 2(CRC_PCOUNT_REG2)
Figure 18-30. CRC Pattern Counter Preload Register 2(CRC_PCOUNT_REG2) [offset =80h]
31 18 19 16
Reserved CRC_PAT_COUNT2
R-0 R/W-0
15 0
CRC_PAT_COUNT2
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-26. CRC Pattern Counter Preload Register 2(CRC_PCOUNT_REG2) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-0 CRC_PAT_COUNT2 Channel 2Pattern Counter Preload Register. This register contains thenumber ofdata
patterns inonesector tobecompressed before aCRC isperformed.

<!-- Page 657 -->

www.ti.com CRC Control Registers
657 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.23 CRC Sector Counter Preload Register 2(CRC_SCOUNT_REG2)
Figure 18-31. CRC Sector Counter Preload Register 2(CRC_SCOUNT_REG2) [offset =84h]
31 16
Reserved
R-0
15 0
CRC_SEC_COUNT2
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-27. CRC Sector Counter Preload Register 2(CRC_SCOUNT_REG2) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 CRC_SEC_COUNT2 Channel 2Sector Counter Preload Register. This register contains thenumber ofsectors in
oneblock ofmemory.
18.4.24 CRC Current Sector Register 2(CRC_CURSEC_REG2)
Figure 18-32. CRC Current Sector Register 2(CRC_CURSEC_REG2) [offset =88h]
31 16
Reserved
R-0
15 0
CRC_CURSEC2
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-28. CRC Current Sector Register 2(CRC_CURSEC_REG2) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 CRC_CURSEC2 Channel 2Current Sector IDRegister. InAUTO mode, thisregister contains thecurrent
sector number ofwhich thesignature verification fails. The sector counter isafree running
upcounter. When asector fails, theerroneous sector number islogged intocurrent sector
IDregister andtheCRC failinterrupt isgenerated The sector IDregister isfrozen until itis
read andtheCRC failstatus bitiscleared byCPU. While itisfrozen, itdoes notcapture
another erroneous sector number. When thiscondition happens, anoverrun interrupt is
generated instead. Once theregister isread andtheCRC failinterrupt flagiscleared itcan
capture new erroneous sector number. InSemi-CPU mode, thisregister isused toindicate
thesector number forwhich thecompression complete haslasthappened.

<!-- Page 658 -->

CRC Control Registers www.ti.com
658 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.25 CRC Channel 2Watchdog Timeout Preload Register A(CRC_WDTOPLD2)
Figure 18-33. CRC Channel 2Watchdog Timeout Preload Register A(CRC_WDTOPLD2)
[offset =8Ch]
31 24 23 16
Reserved CRC_WDTOPLD2
R-0 R/W-0
15 0
CRC_WDTOPLD2
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-29. CRC Channel 2Watchdog Timeout Preload Register A(CRC_WDTOPLD2)
Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 CRC_WDTOPLD2 Channel 2Watchdog Timeout Counter Preload Register. This register contains thenumber of
clock cycles within which theDMA must transfer thenext block ofdata patterns. InSemi-CPU
mode, thisregister isused toindicate thesector number forwhich thecompression complete
haslasthappened.
18.4.26 CRC Channel 2Block Complete Timeout Preload Register B(CRC_BCTOPLD2)
Figure 18-34. CRC Channel 2Block Complete Timeout Preload Register B(CRC_BCTOPLD2)
[offset =90h]
31 24 23 16
Reserved CRC_BCTOPLD2
R-0 R/W-0
15 0
CRC_BCTOPLD2
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 18-30. CRC Channel 2Block Complete Timeout Preload Register B(CRC_BCTOPLD2)
Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 CRC_BCTOPLD2 Channel 2Block Complete Timeout Counter Preload Register. This register contains the
number ofclock cycles within which theCRC foranentire block needs tocomplete before a
timeout interrupt isgenerated.

<!-- Page 659 -->

www.ti.com CRC Control Registers
659 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.27 Channel 2PSA Signature Low Register (PSA_SIGREGL2)
Figure 18-35. Channel 2PSA Signature Low Register (PSA_SIGREGL2) [offset =A0h]
31 0
PSASIG2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-31. Channel 2PSA Signature Low Register (PSA_SIGREGL2) Field Descriptions
Bit Field Description
31-0 PSASIG2 Channel 2PSA Signature Low Register. This register contains thevalue stored atPSASIG2[31:0] register.
18.4.28 Channel 2PSA Signature High Register (PSA_SIGREGH2)
Figure 18-36. Channel 2PSA Signature High Register (PSA_SIGREGH2) [offset =A4h]
31 0
PSASIG2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-32. Channel 2PSA Signature High Register (PSA_SIGREGH2) Field Descriptions
Bit Field Description
31-0 PSASIG2 Channel 2PSA Signature High Register. This register contains thevalue stored atPSASIG2[63:32] register.
18.4.29 Channel 2CRC Value Low Register (CRC_REGL2)
Figure 18-37. Channel 2CRC Value Low Register (CRC_REGL2) [offset =A8h]
31 0
CRC2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-33. Channel 2CRC Value Low Register (CRC_REGL2) Field Descriptions
Bit Field Description
31-0 CRC2 Channel 2CRC Value Low Register. This register contains thecurrent known good signature value stored at
CRC2[31:0] register.

<!-- Page 660 -->

CRC Control Registers www.ti.com
660 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.30 Channel 2CRC Value High Register (CRC_REGH2)
Figure 18-38. Channel 2CRC Value High Register (CRC_REGH2) [offset =ACh]
31 0
CRC2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 18-34. Channel 2CRC Value High Register (CRC_REGH2) Field Descriptions
Bit Field Description
31-0 CRC2 Channel 2CRC Value High Register. This register contains thecurrent known good signature value stored at
CRC2[63:32] register.
18.4.31 Channel 2PSA Sector Signature Low Register (PSA_SECSIGREGL2)
Figure 18-39. Channel 2PSA Sector Signature Low Register (PSA_SECSIGREGL2)
[offset =B0h]
31 0
PSASECSIG2
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-35. Channel 2PSA Sector Signature Low Register (PSA_SECSIGREGL2)
Field Descriptions
Bit Field Description
31-0 PSASECSIG2 Channel 2PSA Sector Signature Low Register. This register contains thevalue stored at
PSASECSIG2[31:0] register.
18.4.32 Channel 2PSA Sector Signature High Register (PSA_SECSIGREGH2)
Figure 18-40. Channel 2PSA Sector Signature High Register (PSA_SECSIGREGH2)
[offset =B4h]
31 0
PSASECSIG2
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-36. Channel 2PSA Sector Signature High Register (PSA_SECSIGREGH2)
Field Descriptions
Bit Field Description
31-0 PSASECSIG2 Channel 2PSA Sector Signature High Register. This register contains thevalue stored at
PSASECSIG2[63:32] register.

<!-- Page 661 -->

www.ti.com CRC Control Registers
661 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCyclic Redundancy Check (CRC) Controller Module18.4.33 Channel 2Raw Data Low Register (RAW_DATAREGL2)
Figure 18-41. Channel 2Raw Data Low Register (RAW_DATAREGL2) [offset =B8h]
31 0
RAW_DATA2
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-37. Channel 2Raw Data Low Register (RAW_DATAREGL2) Field Descriptions
Bit Field Description
31-0 RAW_DATA2 Channel 2Raw Data Low Register. This register contains bits31:0 oftheuncompressed rawdata..
18.4.34 Channel 2Raw Data High Register (RAW_DATAREGH2)
Figure 18-42. Channel 2Raw Data High Register (RAW_DATAREGH2) [offset =BCh]
31 0
RAW_DATA2
R-0
LEGEND: R=Read only; -n=value after reset
Table 18-38. Channel 2Raw Data High Register (RAW_DATAREGH2) Field Descriptions
Bit Field Description
31-0 RAW_DATA2 Channel 2Raw Data High Register. This register contains bits63:32 oftheuncompressed rawdata..