# CPU Compare Module for Cortex-R5F (CCM-R5F)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 497-516

---


<!-- Page 497 -->

497 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)Chapter 13
SPNU563A -March 2018
CPU Compare Module forCortex-R5F (CCM-R5F)
This chapter describes theCPU compare module fortheARM ®Cortex ®-R5F (CCM-R5F). This device
implements twoinstances oftheCortex-R5F CPU that arerunning inlockstep todetect faults that may
result inunsafe operating conditions. The CCM-R5F detects faults and signals them toanerror signaling
module (ESM).
NOTE: Ingeneral, theR5F term isused when referencing theCortex-R5F CPU used inthe
Hercules family ofdevices; however, thefloating-point functionality isadevice-specific option
andmay notbeincluded insome devices. Consult your device-specific datasheet to
determine which core isincluded onyour specific device being used.
Topic ........................................................................................................................... Page
13.1 Overview ......................................................................................................... 498
13.2 Module Operation ............................................................................................. 499
13.3 Control Registers ............................................................................................. 507

<!-- Page 498 -->

Overview www.ti.com
498 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.1 Overview
Safety-critical applications require run-time detection offaults incritical components inthedevice such as
theCentral Processing Unit (CPU) andtheVectored Interrupt Controller Module (VIM). Forthispurpose,
theCPU Compare Module forCortex-R5F (CCM-R5F) compares thecore busoutputs oftwoCortex-R5F
CPUs running ina1oo1D (one-out-of-one, with diagnostics) lockstep configuration. This microcontroller
also implements twoVIM modules in1oo1D (one-out-of-one, with diagnostic) lockstep configuration. Any
difference inthecore compare busoutputs oftheCPUs ortheVIMs isflagged asanerror. Fordiagnostic
purposes, theCCM-R5F also incorporates aself-test capability toallow forboot time checking of
hardware faults within theCCM-R5F itself.
Inaddition tocomparing theCPU's andVIM's outputs forfault detection during run-time, theCCM-R5F
also incorporates twoadditional run-time diagnostic features.
The firstadditional measure istheChecker CPU Inactivity Monitor which willmonitor thechecker CPU's
keybussignals totheinterconnect. When thetwoCPUs areinlockstep configuration, several keybus
signals from thechecker CPU which would have indicated avalid bustransaction totheinterconnect on
themicrocontroller willbemonitored. Alistofthesignals tobemonitored isprovided inTable 13-5 .These
signals from thechecker CPU areexpected tobeinactive. Alltransactions between thelockstep CPUs
andtherestofthesystem should only gothrough themain CPU. Any signals which indicate activity will
beflagged asanerror.
The second feature isthePower Domain (PD) Inactivity Monitor. Similar totheChecker CPU Inactivity
Monitor inconcept, thePower Domain Inactivity Monitor willmonitor keybussignals forbusmasters
residing inpower domains which areturned off.When apower domain isturned off,theboundary ofthe
power domain isisolated from therestofthesystem. Bus signals which would have indicated avalid bus
transaction onto theinterconnect aremonitored. Any signals which indicate active state willbeflagged as
anerror.
13.1.1 Main Features
The main features oftheCCM-R5F are:
*Run-time detection offaults
-Run-time compare ofCPU's outputs
-Run-time compare ofVIM's outputs
-Run-time inactivity monitor onthechecker CPU's bussignals totheinterconnect
-Run-time inactivity monitor onthepower domains' bussignals totheinterconnect
*self-test capability
*error forcing capability
13.1.2 Block Diagram
Figure 13-1 shows theinterconnect diagram oftheCCM-R5F with thetwoCortex-R5F CPUs andthetwo
VIMs. The core busoutputs oftheCPUs arecompared intheCCM-R5F. Toavoid common mode
impacts, thesignals oftheCPUs tobecompared aretemporally diverse. The output signals ofthemaster
CPU aredelayed 2cycles while theinput signals ofchecker CPU aredelayed 2cycles. The twocycle
delay strategy isalso deployed between thetwoVIM modules. While inlockstep mode, thechecker CPU's
output signals tothesystem areclamped toinactive safe values. Key signals which would have indicated
avalid bustransaction totheinterconnect aremonitored bytheCCM-R5F. The same approach isused for
thekeypower domains ifinactive signals indicate thatbusmasters inside these power domains are
asserting valid bustransactions.

<!-- Page 499 -->

CPU Bus Compare
PD Inactivity 
Monitor
VIM Bus CompareChecker CPU 
Inactivity Monitor
CPU1 
(Main CPU) CPU2 
(Checker 
CPU)2 cycle delay
VIM1 VIM2
2 cycle delay
Inputs to CPU1 Inputs to CPU2Lockstep mode
cpu2clkcpu1clkOutputs from CPU1 to 
the systemOutputs CPU2 to the 
system
Safe values (values 
that will force the 
ZlWh[}µµ
to inactive states) Lockstep 
modeCCM-R5F
Compare errors
ESMPDx PDy
www.ti.com Module Operation
499 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)Figure 13-1. Block Diagram
13.2 Module Operation
Asdescribed inSection 13.1,there arefour different run-time diagnostics supported bytheCCM-R5F.
The CCM-R5F compares thecore busoutputs ofthemaster andchecker Cortex-R5F CPUs onthe
microcontroller andsignals anerror onanymismatch. This comparison isstarted 6CPU clock cycles after
theCPU comes outofreset toensure thatCPU output signals have propagated toaknown value after
reset. Once comparison isstarted, theCCM module continues tomonitor theoutputs ofthetwoCPUs
without anysoftware intervention. Ifanerror isdetected bytheCCM-R5F, asoftware handler isnecessary
toimplement theappropriate response totheerror dependent onapplication needs. The module principles
ofoperation areapplicable toboth theCPU output compare asdescribed above aswell astotheVIM
output compare.

<!-- Page 500 -->

Module Operation www.ti.com
500 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.1 CPU/VIM Output Compare Diagnostic
CPU /VIM Output Compare Diagnostic canruninoneofthefollowing four operating modes:
1.Active compare lockstep mode
2.Self-test
3.Error forcing
4.Self-test error forcing
The operating mode canbeselected bywriting adedicated keytothekeyregister (MKEYx) ofthe
corresponding diagnostic.
NOTE: MKEY1 andMKEY2 areused toselect theoperating mode fortheCPU Output Compare
Diagnostic andVIM Output Compare Diagnostic, respectively.
13.2.1.1 Active Compare lockstep Mode
This isthedefault mode onstart-up. Inlockstep mode, thebusoutput signals ofboth CPUs andVIMs are
compared. Adifference intheCPU compare busoutputs isindicated bysignaling anerror totheESM,
which sets theerror flag"CCM-R5F -CPU compare" and"CCM-R5F -VIM compare", respectively.
*CPU types ofoutput signals tobecompared:
-Global signals
-Interrupt signals
-AllL1cache interface signals
-Allcache coherency signals
-AllL1TCM interface signals
-AllL2AXIinterface signals
-ETM interface signals
-FPU signals
-AllACP interface signals
-AllAXIPeripheral port interface signals
-AllAHB Peripheral port interface signals
-Allstatus andcontrol signals
*VIM output signals tobecompared:
-nFIQ
-nIRQ
-IRQADDRV
-IRQVECTADDR
NOTE: The CPU compare error asserts "CCM-R5F self-test error "flagaswell. Bydoing this, the
CPU compare error hastwopaths ("CCM-R5F -CPU compare "and"CCM-R5F self-test
error "flag) totheESM, sothateven ifoneofthepaths fails, theerror isstillpropagated to
theESM. This isalso true for"CCM-R5F -VIM compare" error flag.
Notallinternal registers oftheCortex-R5F CPU have fixed values upon reset. Toavoid anerroneous
CCM-R5F compare error, theapplication software needs toensure thattheCPU registers ofboth CPUs
areinitialized with thesame values before theregisters areused, including function calls where the
register values arepushed onto thestack.

<!-- Page 501 -->

www.ti.com Module Operation
501 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.1.2 Self-Test Mode
Inself-test mode, theCCM-R5F checks itself forfaults. During self-test, thecompare error module output
signal isdeactivated. Any fault detected inside theCCM-R5F willbeflagged byESM error "CCM-R5F -
self-test ".
Inself-test mode, theCCM-R5F automatically generates testpatterns tolook foranyhardware faults. Ifa
fault isdetected, then aself-test error flagisset,aself-test error signal isasserted andsent totheESM,
andtheself-test isterminated immediately. Ifnofault isfound during self-test, theself-test complete flagis
set.Inboth cases, theCCM-R5F CPU /VIM Output Compare Diagnostic remains inself-test mode after
thetesthasbeen terminated orcompleted, andtheapplication needs toswitch theCCM-R5F mode by
writing another keytothemode keyregister (MKEY1 orMKEY2 depending which diagnostic isselected
forself-test). During theself-test operation, thecompare error signal output totheESM isinactive
irrespective ofthecompare result.
There aretwotypes ofpatterns generated byCCM-R5F during self-test mode:
1.Compare Match Test
2.Compare Mismatch Test
CCM-R5F firstgenerates Compare Match Test patterns, followed byCompare Mismatch Test patterns.
Each testpattern isapplied onboth CPU signal inputs oftheCCM-R5F 'scompare block andclocked for
onecycle. The duration ofself-test forCPU Output Compare Diagnostic is4947 CPU clock cycles
(GCLK1) and151system peripheral clock cycles (VCLK) forVIM Output Compare Diagnostic.
NOTE: During self-test, both CPUs canexecute normally, butthecompare logic willnotbechecking
anyCPU signals. Also during self-test, only thecompare unitlogic istested andnotthe
memory-mapped register controls fortheCCM-R5F. The self-test isnotinterruptible.
Self-test ofalldifferent diagnostics canberunatthesame time.
13.2.1.2.1 Compare Match Test
During theCompare Match Test, there arefour different testpatterns generated tostimulate theCCM-
R5F. Anidentical vector isapplied toboth input ports atthesame time expecting acompare match. These
patterns cause theself-test logic toexercise every CPU compare busoutput signal inparallel. Ifthe
compare unitproduces acompare mismatch then theself-test error flagisset,theself-test error signal is
generated, andtheCompare Match Test isterminated.
The four testpatterns used fortheCompare Match Test are:
*All1sonboth CPU /VIM signal ports
*All0sonboth CPU /VIM signal ports
*0xAs onboth CPU /VIM signal ports
*0x5s onboth CPU /VIM signal ports
These four testpatterns willtake four clock cycles tocomplete. Table 13-1 illustrates thesequence of
Compare Match Test.
Table 13-1. Compare Match Test Sequence
CPU 1(Main CPU) Signal Position CPU 2(Checker CPU) Signal Position
Cycle
n:8 7 6 5 4 3 2 1 0 n:8 7 6 5 4 3 2 1 0
1s 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 1 1 1 0
0s 0 0 0 0 0 0 0 0 0s 0 0 0 0 0 0 0 0 1
0xA 1 0 1 0 1 0 1 0 0xA 1 0 1 0 1 0 1 0 2
0x5 0 1 0 1 0 1 0 1 0x5 0 1 0 1 0 1 0 1 3

<!-- Page 502 -->

Module Operation www.ti.com
502 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.1.2.2 Compare Mismatch Test
During theCompare Mismatch Test, thenumber oftestpatterns isequal totwice thenumber ofCPU
output signals tocompare inlockstep mode. Anall1svector isapplied totheCCM-R5F 'sCPU1 /VIM1
input port andthesame pattern isalso applied totheCCM-R5F 'sCPU2 /VIM2 input port butwith onebit
flipped starting from signal position 0.The un-equal vector willcause theCCM-R5F toexpect acompare
mismatch atsignal position 0,iftheCCM-R5F logic isworking correctly. If,however, theCCM-R5F logic
reports acompare match, theself-test error flagisset,theself-test error signal isasserted, andthe
Compare Mismatch Test isterminated.
This Compare Mismatch Test algorithm repeats inadomino fashion with thenext signal position flipped
while forcing allother signals tologic level 1.This sequence isrepeated until every single signal position
isverified onboth CPU signal ports.
The Compare Mismatch Test isterminated iftheCCM-R5F reports acompare match versus theexpected
compare mismatch. This testensures thatthecompare unitisable todetect amismatch onevery CPU
signal being compared. Table 13-2 illustrates thesequence ofCompare Mismatch Test. There isnoerror
signal sent toESM iftheexpected errors areseen with each pattern.
Table 13-2. CPU /VIMCompare Mismatch Test Sequence
CPU 1(Main CPU) Signal Position CPU 2(Checker CPU) Signal Position
Cycle
n n-1:8 7 6 5 4 3 2 1 0 n n-1:8 7 6 5 4 3 2 1 0
1 1 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 1 1 0 0
1 1 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 1 0 1 1
1 1 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 0 1 1 2
1 1 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 0 1 1 1 3
::
1 1 1s 1 1 1 1 1 1 1 1 1 0 1s 1 1 1 1 1 1 1 1 -1
1 1 1s 1 1 1 1 1 1 1 1 0 1 1s 1 1 1 1 1 1 1 1 n
1 1 1s 1 1 1 1 1 1 1 0 1 1 1s 1 1 1 1 1 1 1 1 n+1
1 1 1s 1 1 1 1 1 1 0 1 1 1 1s 1 1 1 1 1 1 1 1 n+2
1 1 1s 1 1 1 1 1 0 1 1 1 1 1s 1 1 1 1 1 1 1 1 n+3
1 1 1s 1 1 1 1 0 1 1 1 1 1 1s 1 1 1 1 1 1 1 1 n+4
::
1 0 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 1 1 1 2n-1
0 1 1s 1 1 1 1 1 1 1 1 1 1 1s 1 1 1 1 1 1 1 1 2n

<!-- Page 503 -->

www.ti.com Module Operation
503 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.1.3 Error Forcing Mode
Inerror forcing mode, atestpattern isapplied totheCPU /VIM related inputs oftheCCM-R5F compare
logic toforce anerror inthecompare error output signal ofthecompare unit. Depending iferror forcing
mode isapplied totheCPU Output Compare Diagnostic orVIM Output Compare Diagnostic, theESM
error flag"CCM-R5F -CPU compare "or"CCM-R5F -VIM compare "isexpected after theerror forcing
mode completes. Asaside effect, the"CCM-R5F self-test error "flagisalso asserted whenever theCPU
compare error isasserted.
Error forcing mode issimilar totheCompare Mismatch Test operation ofself-test mode inwhich anun-
equal vector isapplied totheCCM-R5F CPU signal ports. The error forcing mode forces thecompare
mismatch toactually assert thecompare error output signal. This ensures thatafault inthepath between
CCM-R5F andESM isdetected.
Only onehardcoded testpattern isapplied intoCCM-R5F during error forcing mode. Arepeated 0x5
pattern isapplied toCPU1 /VIM1 signal port ofCCM-R5F input while arepeated 0xA pattern isapplied to
theCPU2 /VIM2 signal port ofCCM-R5F input. The error forcing mode takes onecycle tocomplete.
Hence, thefailing signature ispresented foroneclock cycle. After that, themode isautomatically switched
tolockstep mode. The keyregister (MKEY1 forCPU output compare andMKEY2 forVIM output compare)
willindicate thelockstep keymode once itisswitched tolockstep mode. During theonecycle required by
theerror forcing test, theCPU /VIM output signals arenotcompared. The user should expect theESM to
trigger aresponse (report theCCM-R5F fail). Ifnoerror isdetected bytheESM, then ahardware fault is
present.
13.2.1.4 Self-Test Error Forcing Mode
Inself-test error forcing mode, anerror isforced attheself-test error signal. The compare unitisstill
running inlockstep mode andthekeyisswitched tolockstep after oneclock cycle. The ESM error flag
"CCM-R5F -self-test "isexpected after theself-test error forcing mode completes. Once theexpected
errors areseen, theapplication canclean theerror through theESM module.
Table 13-3 shows what error signals andflags areasserted indifferent operating mode. The behavior of
different modes inthistable forCPU compare isalso valid forother diagnostics such asVIM compare,
Checker CPU Inactivity Monitor andPower Domain monitor.
Table 13-3. Error Flags andError Signals Generation inEach Mode
Mode KeySelfTest
Error SignalCompare
Error SignalCMPE STC STET STE
Active
Compare
Lockstep0000 Enabled Enabled Enabled Disabled Disabled Disabled
Self-Test 0110 Enabled Disabled Disabled Enabled Enabled Enabled
Error Forcing 1001 Error Error Disabled Disabled Disabled Disabled
Self-Test
Error Forcing1111 Error Enabled Enabled Disabled Disabled Disabled

<!-- Page 504 -->

CPU1
CPU2POLARITYINVERTXOR ESMN N-8 N
8
Module Operation www.ti.com
504 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.2 CPU Input Inversion Diagnostic
There isanother way tointentionally create amismatch between thetwoCPUs' outputs asadiagnostic
testtoself-test theCCM-R5F's CPU Output Compare Diagnostic block. Before theCPU1's outputs are
taken totheCCM-R5F, eight oftheoutput signals arefirstexclusive-ORed bitwise with the8-bit
POLARITYINVERT register. After reset, thedefault value ofthePOLARITYINVERT register isallzeros.
The resultant values ofthe8signals after theXOR logic with thePOLARITYINVERT register willstillbe
thesame astheoriginal 8signal values. However, byprogramming thePOLARITYINVERT toanon-zero
values itwillhave theeffect toinvert thesignal values. This intentional inversion ontheinputs totheCCM-
R5F willcause theCPU Output Compare Diagnostic todetect acompare error. See Figure 13-2 for
illustration.
Figure 13-2. CPU Input Inversion Scheme
Table 13-4. CPU1 (Main CPU) Signals Being Inverted Before Being Compared
Signals Remark
AWVALIDM Indicates write address andcontrol arevalid
ARVALIDM Indicates write address andcontrol arevalid
AWVALIDP Indicates write address andcontrol arevalid
ARVALIDP Indicates write address andcontrol arevalid
HTRANSP[1:0] Indicates write address andcontrol arevalid

<!-- Page 505 -->

www.ti.com Module Operation
505 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.3 Checker CPU Inactivity Monitor
Similar totheCPU /VIM Output Compare Diagnostic, theChecker CPU Inactivity Monitor canalso runin
oneofthefollowing four operating modes:
1.Active compare
2.Self-test
3.Error forcing
4.Self-test error forcing
The operating mode canbeselected bywriting adedicated keytothekeyregister (MKEY3).
13.2.3.1 Active Compare Mode
This isthedefault mode onstart-up. Inthismode, several keybussignals such asthebusvalid control
signals from thechecker CPU thatwould have indicated avalid bustransaction onto theinterconnect are
compared against their clamped safe values. While thetwoCPUs areinlockstep configuration, the
outputs ofthechecker CPU aresupposed toclamp totheinactive state thatisallzeros. Adifference
between thechecker CPU compare busoutputs andtheir respective inactive states isindicated by
signaling anerror totheESM which sets theerror flag"CCM-R5F -CPU1 AXIM Bus Monitor Failure".
Table 13-5. Checker CPU Signals toMonitor
Signals Remark
AWVALIDM When asserted, indicates address andcontrol arevalid ontheChecker CPU 'sAXImaster port for
write transaction.
ARVALIDM When asserted, indicates address andcontrol arevalid ontheChecker CPU 'sAXImaster port for
read transaction.
AWVALIDP When asserted, indicates address andcontrol arevalid ontheChecker CPU 'sAXIperipheral port
forwrite transaction.
ARVALIDP When asserted, indicates address andcontrol arevalid ontheChecker CPU 'sAXIperipheral port
forread transaction.
BVALIDS When asserted, indicates thatavalid write response isavailable ontheChecker CPU 'sAXIslave
port forwrite transaction
RVALIDS When asserted, indicates address andcontrol arevalid ontheChecker CPU 'sAXIslave port for
read transaction
13.2.3.2 Self-Test Mode
Similar totheother self-test described forCPU /VIM Output Compare Diagnostic, theChecker CPU
Inactivity Monitor canbeplaced inself-test mode. Inself-test mode, theCCM-R5F checks theChecker
CPU Inactivity Monitor itself forfaults. During self-test, thecompare error module output signal is
deactivated. Any fault detected inside theCCM-R5F willbeflagged byESM error "CCM-R5F -self-test ".
Inself-test mode, theCCM-R5F automatically generates testpatterns tolook foranyhardware faults. Ifa
fault isdetected, then aself-test error flagisset,aself-test error signal isasserted andsent totheESM,
andtheself-test isterminated immediately. Ifnofault isfound during self-test, theself-test complete flagis
set.Inboth cases, theCCM-R5F Checker CPU Inactivity Monitor Diagnostic remains inself-test mode
after thetesthasbeen terminated orcompleted, andtheapplication needs toswitch theCCM-R5F mode
bywriting another keytothemode keyregister (MKEY3). During theself-test operation, thecompare error
signal output totheESM isinactive irrespective ofthecompare result.
There arealso twotypes ofpatterns generated byCCM-R5F during self-test mode forCheck CPU
Inactivity Monitor. The difference here isthenumber oftestpatterns applied during self-test.
i.Compare Match Test
ii.Compare Mismatch Test
CCM-R5F firstgenerates Compare Match Test patterns, followed byCompare Mismatch Test patterns.

<!-- Page 506 -->

Module Operation www.ti.com
506 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.3.2.1 Compare Match Test
Since thecomparison isdone against theclamped values, andallcompared signals areclamped tozero,
only onetestpattern isapplied forthecompare match test. Apattern ofall-zeros areapplied forthe
compare match test. The testwilltake onecycle. Ifthecompare unitproduces acompare mismatch then
theself-test error flagisset,theself-test error signal isgenerated, andtheCompare Match Test is
terminated.
13.2.3.2.2 Compare Mismatch Test
During theCompare Mismatch Test, thenumber oftestpatterns isequal tothenumber ofbussignals on
thechecker CPU tobemonitored. There areatotal of6signals being monitored onthechecker CPU's
level 2interface andhence ittakes 6testpatterns forthemismatch test. The mismatch testwilltake a
total of6cycles tocomplete. Anall0'stestvector isapplied totheCCM-R5F 'sbutwith onebitflipped
starting from signal position 0.The un-equal vector willcause theCCM-R5F toexpect acompare
mismatch atsignal position 0,iftheCCM-R5F logic isworking correctly. If,however, theCCM-R5F logic
reports acompare match, theself-test error flagisset,theself-test error signal isasserted, andthe
Compare Mismatch Test isterminated.
This Compare Mismatch Test algorithm repeats inadomino fashion with thenext signal position flipped
while forcing allother signals tologic level 0.This sequence isrepeated until every inactivity monitor
signal position isverified onthechecker CPU .
Table 13-6 shows thesequence ofCompare Mismatch Test. There isnoerror signal sent toESM ifthe
expected errors areseen with each pattern.
Table 13-6. Checker CPU Inactivity Monitor Compare Mismatch Test
Signal Position
5 4 3 2 1 0 Cycle
0 0 0 0 0 1 0
0 0 0 0 1 0 1
0 0 0 1 0 0 2
0 0 1 0 0 0 3
0 1 0 0 0 0 4
1 0 0 0 0 0 5
13.2.3.3 Error Forcing Mode
Inerror forcing mode, atestpattern ofall1'sisapplied tothecheck CPU's compare logic toforce anerror
inthecompare error output signal ofthecompare unit. The ESM error flag"CCM-R5F -CPU1 AXIM Bus
Inactivity failure "isexpected after theerror forcing mode completes. Asaside effect, the"CCM-R5F self-
testerror "flagisalso asserted whenever theCPU compare error isasserted.
The error forcing mode takes onecycle tocomplete. Hence, thefailing signature ispresented forone
clock cycle. After that, themode isautomatically switched toactive compare mode. The keyregister
(MKEY3) willindicate theactive compare mode once itisswitched toactive compare mode. During the
onecycle required bytheerror forcing test, thechecker CPU Inactivity Monitor isdeactivated. User should
expect theESM totrigger aresponse (report theCCM-R5F fail). Ifnoerror isdetected byESM, then a
hardware fault ispresent.
13.2.3.4 Self-Test Error Forcing Mode
Inself-test error forcing mode, anerror isforced attheself-test error signal. The compare unitisstill
running inactive compare mode andthekeyisswitched toactive compare after oneclock cycle. The
ESM error flag"CCM-R5F -self-test "isexpected after theself-test error forcing mode completes. Once
theexpected errors areseen, theapplication canclean theerror through theESM module.

<!-- Page 507 -->

www.ti.com Module Operation
507 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.2.4 Power Domain Inactivity Monitor
The Power Domain Inactivity Monitor isvery similar totheChecker CPU Inactivity Monitor inconcept.
When apower domain isturned off,itsoutputs areisolated from therestofthesystem. The outputs are
clamped toinactive safe values. Depending onthesignals, theclamp value ofasignal may be0or1.
Some busmasters may beresiding intheturned offpower domains. Key bussignals from thepower
domains which would have indicated thatthebusmaster isgenerating avalid bustransaction are
compared against their clamped values.
The Power Domain Inactivity Monitor Diagnostic canalso runinoneofthefollowing four operating modes:
1.Active compare
2.Self-test
3.Error forcing
4.Self-test error forcing
The operating mode canbeselected bywriting adedicated keytothekeyregister (MKEY4).
13.2.4.1 Active Compare Mode
This isthedefault mode onstart-up.
Inthismode, several critical bussignals such asthebusrequest control signals from thepower domains
which would have indicated avalid bustransaction onto theinterconnect arecompared against their
clamped safe values. Ifapower domain isturned off,theoutputs ofthepower domain areexpected to
clamp totheinactive states. Adifference between thepower domain compare busoutputs andtheir
respective inactive states isindicated bysignaling anerror totheESM which sets theerror flag"CCM-
R5F -Power Domain Monitor Failure". Inaddition, thecorresponding busmasters forwhich thecompare
block detected themonitor failure arealso captured intheCCMPDSTAT0 register.
Self-test mode, Error forcing mode andSelf-test error forcing mode forPower Domain Inactivity Monitor
Diagnostic arethesame asChecker CPU Inactivity Monitor Diagnostic. See Section 13.2.3.2 ,
Section 13.2.3.3 ,andSection 13.2.3.4 fordetails.
13.2.5 Operation During CPU Debug Mode
Certain debug operations place theCPU inahalting debug state where thecode execution ishalted.
Because halting debug events areasynchronous, there isapossibility forthedebug requests tocause
loss oflockstep. CCM-R5F willdisable allfunctional diagnostics upon detection ofhalting debug requests.
Core compare error willnotbegenerated andflags willnotupdate. ACPU reset isneeded toensure the
CPUs areagain inlockstep andwillalso re-enable theCCM-R5F.
13.3 Control Registers
Table 13-7 lists theCCM-R5F registers. Each register begins ona32-bit word boundary. The registers
support 32-bit, 16-bit, and8-bit accesses. The base address forthecontrol registers isFFFF F600h.
Table 13-7. Control Registers
Offset Acronym Register Description Section
00h CCMSR1 CCM-R5F Status Register 1 Section 13.3.1
04h CCMKEYR1 CCM-R5F Key Register 1 Section 13.3.2
08h CCMSR2 CCM-R5F Status Register 2 Section 13.3.3
0Ch CCMKEYR2 CCM-R5F Key Register 2 Section 13.3.4
10h CCMSR3 CCM-R5F Status Register 3 Section 13.3.5
14h CCMKEYR3 CCM-R5F Key Register 3 Section 13.3.6
18h CCMPOLCNTRL Polarity Control Register Section 13.3.7
1Ch CCMSR4 CCM-R5F Status Register 4 Section 13.3.8
20h CCMKEYR4 CCM-R5F Key Register 4 Section 13.3.9
24h CCMPDSTAT0 CCM-R5F Power Domain Status Register 0 Section 13.3.10

<!-- Page 508 -->

Control Registers www.ti.com
508 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.1 CCM-R5F Status Register 1(CCMSR1)
The contents ofthisregister should beinterpreted incontext ofwhat testwas selected. That is,what
mode isCCM operating.
Figure 13-3. CCM-R5F Status Register 1(CCMSR1) (Offset =00h)
31 17 16
Reserved CPME1
R-0 R/W1CP-0
15 9 8 7 2 1 0
Reserved STC1 Reserved STET1 STE1
R-0 R-0 R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 13-8. CCM-R5F Status Register 1(CCMSR1) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 CMPE1 Compare Error forCPU Output Compare Diagnostic.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: CPU signals areidentical.
Write: Leaves thebitunchanged.
1 Read: CPU signal compare mismatch.
Write: Clears thebit.
15-9 Reserved Reads return 0.Writes have noeffect.
8 STC1 Self-test Complete forCPU Output Compare Diagnostic.
Note: This bitisalways 0when notinself-test mode. Once set,switching from self-test mode to
other modes willclear thisbit.
Read/Write inUser andPrivileged mode.
0 Read: Self-test on-going ifself-test mode isentered.
Write: Writes have noeffect.
1 Read: Self-test iscomplete.
Write: Writes have noeffect.
7-2 Reserved Reads return 0.Writes have noeffect.
1 STET1 Self-test Error Type forCPU Output Compare Diagnostic.
Read/Write inUser andPrivileged mode.
0 Read: Self-test failed during Compare Match Test ifSTE1 =1.
Write: Writes have noeffect.
1 Read: Self-test failed during Compare Mismatch Test ifSTE1 =1.
Write: Writes have noeffect.
0 STE1 Self-test Error forCPU Output Compare Diagnostic.
Note: This bitgets updated when theself-test iscomplete oranerror isdetected.
Read/Write inUser andPrivileged mode.
0 Read: Self-test passed.
Write: Writes have noeffect.
1 Read: Self-test failed.
Write: Writes have noeffect.

<!-- Page 509 -->

www.ti.com Control Registers
509 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.2 CCM-R5F Key Register 1(CCMKEYR1)
Figure 13-4. CCM-R5F KeyRegister 1(CCMKEYR1) (Offset =04h)
31 16
Reserved
R-0
15 4 3 0
Reserved MKEY1
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 13-9. CCM-R5F KeyRegister 1(CCMKEYR1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MKEY1 Mode Key toselect operation forCPU Output Compare Diagnostic .
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Returns current value oftheMKEY1.
Write: Active Compare Lockstep mode.
6h Read: Returns current value oftheMKEY1.
Write: Self-test mode.
9h Read: Returns current value oftheMKEY1.
Write: Error Forcing mode.
Fh Read: Returns current value oftheMKEY1.
Write: Self-test Error Forcing mode.
Other values Note: Itisrecommended tonotwrite anyother keycombinations. Invalid keys willresult in
switching operation tolockstep mode.

<!-- Page 510 -->

Control Registers www.ti.com
510 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.3 CCM-R5F Status Register 2(CCMSR2)
Figure 13-5. CCM-R5F Status Register 2(CCMSR2) (Offset =08h)
31 17 16
Reserved CPME2
R-0 R/W1CP-0
15 9 8 7 2 1 0
Reserved STC2 Reserved STET2 STE2
R-0 R-0 R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 13-10. CCM-R5F Status Register 2(CCMSR2) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 CMPE2 Compare Error forVIM Output Compare Diagnostic.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: CPU signals areidentical.
Write: Leaves thebitunchanged.
1 Read: CPU signal compare mismatch.
Write: Clears thebit.
15-9 Reserved Reads return 0.Writes have noeffect.
8 STC2 Self-test Complete forVIM Output Compare Diagnostic.
Note: This bitisalways 0when notinself-test mode. Once set,switching from self-test mode to
other modes willclear thisbit.
Read/Write inUser andPrivileged mode.
0 Read: Self-test on-going ifself-test mode isentered.
Write: Writes have noeffect.
1 Read: Self-test iscomplete.
Write: Writes have noeffect.
7-2 Reserved Reads return 0.Writes have noeffect.
1 STET2 Self-test Error Type forVIM Output Compare Diagnostic.
Read/Write inUser andPrivileged mode.
0 Read: Self-test failed during Compare Match Test ifSTE2 =1.
Write: Writes have noeffect.
1 Read: Self-test failed during Compare Mismatch Test ifSTE2 =1.
Write: Writes have noeffect.
0 STE2 Self-test Error forVIM Output Compare Diagnostic.
Note: This bitgets updated when theself-test iscomplete oranerror isdetected.
Read/Write inUser andPrivileged mode.
0 Read: Self-test passed.
Write: Writes have noeffect.
1 Read: Self-test failed.
Write: Writes have noeffect.

<!-- Page 511 -->

www.ti.com Control Registers
511 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.4 CCM-R5F Key Register 2(CCMKEYR2)
Figure 13-6. CCM-R5F KeyRegister 2(CCMKEYR2) (Offset =0Ch)
31 16
Reserved
R-0
15 4 3 0
Reserved MKEY2
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 13-11. CCM-R5F KeyRegister 2(CCMKEYR2) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MKEY2 Mode Key toselect operation forVIM Output Compare Diagnostic.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Returns current value oftheMKEY2.
Write: Active Compare Lockstep mode.
6h Read: Returns current value oftheMKEY2.
Write: Self-test mode.
9h Read: Returns current value oftheMKEY2.
Write: Error Forcing mode.
Fh Read: Returns current value oftheMKEY2.
Write: Self-test Error Forcing mode.
Other values Note: Itisrecommended tonotwrite anyother keycombinations. Invalid keys willresult in
switching operation tolockstep mode.

<!-- Page 512 -->

Control Registers www.ti.com
512 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.5 CCM-R5F Status Register 3(CCMSR3)
Figure 13-7. CCM-R5F Status Register 3(CCMSR3) (Offset =10h)
31 17 16
Reserved CPME3
R-0 R/W1CP-0
15 9 8 7 2 1 0
Reserved STC3 Reserved STET3 STE3
R-0 R-0 R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 13-12. CCM-R5F Status Register 3(CCMSR3) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 CMPE3 Compare Error forChecker CPU Inactivity Monitor.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: CPU signals areidentical.
Write: Leaves thebitunchanged.
1 Read: CPU signal compare mismatch.
Write: Clears thebit.
15-9 Reserved Reads return 0.Writes have noeffect.
8 STC3 Self-test Complete forChecker CPU Inactivity Monitor.
Note: This bitisalways 0when notinself-test mode. Once set,switching from self-test mode to
other modes willclear thisbit.
Read/Write inUser andPrivileged mode.
0 Read: Self-test on-going ifself-test mode isentered.
Write: Writes have noeffect.
1 Read: Self-test iscomplete.
Write: Writes have noeffect.
7-2 Reserved Reads return 0.Writes have noeffect.
1 STET3 Self-test Error Type forChecker CPU Inactivity Monitor.
Read/Write inUser andPrivileged mode.
0 Read: Self-test failed during Compare Match Test ifSTE3 =1.
Write: Writes have noeffect.
1 Read: Self-test failed during Compare Mismatch Test ifSTE3 =1.
Write: Writes have noeffect.
0 STE3 Self-test Error forChecker CPU Inactivity Monitor.
Note: This bitgets updated when theself-test iscomplete oranerror isdetected.
Read/Write inUser andPrivileged mode.
0 Read: Self-test passed.
Write: Writes have noeffect.
1 Read: Self-test failed.
Write: Writes have noeffect.

<!-- Page 513 -->

www.ti.com Control Registers
513 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.6 CCM-R5F Key Register 3(CCMKEYR3)
Figure 13-8. CCM-R5F KeyRegister 3(CCMKEYR3) (Offset =14h)
31 16
Reserved
R-0
15 4 3 0
Reserved MKEY3
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 13-13. CCM-R5F KeyRegister 2(CCMKEYR2) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MKEY3 Mode Key toselect operation forChecker CPU Inactivity Monitor.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Returns current value oftheMKEY3.
Write: Active Compare Lockstep mode.
6h Read: Returns current value oftheMKEY3.
Write: Self-test mode.
9h Read: Returns current value oftheMKEY3.
Write: Error Forcing mode.
Fh Read: Returns current value oftheMKEY3.
Write: Self-test Error Forcing mode.
Other values Note: Itisrecommended tonotwrite anyother keycombinations. Invalid keys willresult in
switching operation tolockstep mode.
13.3.7 CCM-R5F Polarity Control Register (CCMPOLCNTRL)
Figure 13-9. CCM-R5F Polarity Control Register (CCMPOLCNTRL) (Offset =18h)
31 16
Reserved
R-0
15 8 7 0
Reserved POLARITYINVERT
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 13-14. CCM-R5F Polarity Control Register (CCMPOLCNTRL) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
3-0 POLARITYINVERT Polarity Inversion. This value isused toinvert oneofthe8output compare signals from the
CPU1 totheCCM-R5F. Inverting anyonesignal willlead tocompare error bytheCPU Output
Compare Diagnostic.
Read inUser andPrivileged mode. Write inPrivileged mode only.

<!-- Page 514 -->

Control Registers www.ti.com
514 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.8 CCM-R5F Status Register 4(CCMSR4)
Figure 13-10. CCM-R5F Status Register 4(CCMSR4) (Offset =1Ch)
31 17 16
Reserved CPME4
R-0 R/W1CP-0
15 9 8 7 2 1 0
Reserved STC4 Reserved STET4 STE4
R-0 R-0 R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 13-15. CCM-R5F Status Register 4(CCMSR4) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 CMPE4 Compare Error forPower Domain Inactivity Monitor.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: CPU signals areidentical.
Write: Leaves thebitunchanged.
1 Read: CPU signal compare mismatch.
Write: Clears thebit.
15-9 Reserved Reads return 0.Writes have noeffect.
8 STC4 Self-test Complete forPower Domain Inactivity Monitor.
Note: This bitisalways 0when notinself-test mode. Once set,switching from self-test mode to
other modes willclear thisbit.
Read/Write inUser andPrivileged mode.
0 Read: Self-test on-going ifself-test mode isentered.
Write: Writes have noeffect.
1 Read: Self-test iscomplete.
Write: Writes have noeffect.
7-2 Reserved Reads return 0.Writes have noeffect.
1 STET4 Self-test Error Type forPower Domain Inactivity Monitor.
Read/Write inUser andPrivileged mode.
0 Read: Self-test failed during Compare Match Test ifSTE4 =1.
Write: Writes have noeffect.
1 Read: Self-test failed during Compare Mismatch Test ifSTE4 =1.
Write: Writes have noeffect.
0 STE4 Self-test Error forPower Domain Inactivity Monitor.
Note: This bitgets updated when theself-test iscomplete oranerror isdetected.
Read/Write inUser andPrivileged mode.
0 Read: Self-test passed.
Write: Writes have noeffect.
1 Read: Self-test failed.
Write: Writes have noeffect.

<!-- Page 515 -->

www.ti.com Control Registers
515 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.9 CCM-R5F Key Register 4(CCMKEYR4)
Figure 13-11. CCM-R5F KeyRegister 4(CCMKEYR4) (Offset =20h)
31 16
Reserved
R-0
15 4 3 0
Reserved MKEY4
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 13-16. CCM-R5F KeyRegister 4(CCMKEYR4) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MKEY4 Mode Key toselect operation forPower Domain Inactivity Monitor.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Returns current value oftheMKEY4.
Write: Active Compare Lockstep mode.
6h Read: Returns current value oftheMKEY4.
Write: Self-test mode.
9h Read: Returns current value oftheMKEY4.
Write: Error Forcing mode.
Fh Read: Returns current value oftheMKEY4.
Write: Self-test Error Forcing mode.
Other values Note: Itisrecommended tonotwrite anyother keycombinations. Invalid keys willresult in
switching operation tolockstep mode.

<!-- Page 516 -->

Control Registers www.ti.com
516 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedCPU Compare Module forCortex-R5F (CCM-R5F)13.3.10 CCM-R5F Power Domain Status Register 0(CCMPDSTAT0)
Figure 13-12. CCM-R5F Power Domain Status Register 0(CCMPDSTAT0) (Offset =24h)
31 16
Reserved
R-0
15 6 5 4 3 2 1 0
Reserved DMM_TRANS HTU2_TRANS FTU_TRANS
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 13-17. CCM-R5FPower Domain Status Register 0(CCMPDSTAT0) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-4 DMM_TRNS DMM Transaction. When thepower domain inwhich theDMM resides isturned off,an
unexpected bustransaction isdetected onDMM master.
Read inUser andPrivileged mode. Write hasnoeffect.
0 Read: Nobustransaction onthemaster when thepower domain isturned off.
Write: Writes have noeffect.
Any non-zero
valueRead: Anunexpected bustransaction isdetected onthemaster.
Write: Writes have noeffect.
3-2 HTU2_TRNS HTU2 Transaction. When thepower domain inwhich theHTU2 resides isturned off,an
unexpected bustransaction isdetected onHTU2 master.
Read inUser andPrivileged mode. Write hasnoeffect.
0 Read: Nobustransaction onthemaster when thepower domain isturned off.
Write: Writes have noeffect.
Any non-zero
valueRead: Anunexpected bustransaction isdetected onthemaster.
Write: Writes have noeffect.
1-0 FTU_TRNS FTU Transaction. When thepower domain inwhich theFTU resides isturned off,an
unexpected bustransaction isdetected onFTU master.
Read inUser andPrivileged mode. Write hasnoeffect.
0 Read: Nobustransaction onthemaster when thepower domain isturned off.
Write: Writes have noeffect.
Any non-zero
valueRead: Anunexpected bustransaction isdetected onthemaster.
Write: Writes have noeffect.