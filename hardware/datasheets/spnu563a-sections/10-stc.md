# Self-Test Controller (STC) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 428-459

---


<!-- Page 428 -->

428 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleChapter 10
SPNU563A -March 2018
Self-Test Controller (STC) Module
This chapter describes thebasics andconfiguration oftheonchip self-test controller (STC) modules.
Topic ........................................................................................................................... Page
10.1 General Description .......................................................................................... 429
10.2 STC Module Assignments ................................................................................. 436
10.3 STC Programmers Flow .................................................................................... 437
10.4 Application Self-Test Flow ................................................................................. 438
10.5 STC1 Segment 0(CPU) Test Coverage andDuration ............................................ 441
10.6 STC1 Segment 1(µSCU) Test Coverage andDuration ........................................... 444
10.7 STC2 (nHET) Test Coverage andDuration ........................................................... 444
10.8 STC Control Registers ...................................................................................... 446
10.9 STC Configuration Example ............................................................................... 458
10.10 Self-Test Controller Diagnostics ........................................................................ 459

<!-- Page 429 -->

www.ti.com General Description
429 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.1 General Description
The self-test controller (STC) module isused totesttheARM CPU core andother complex digital IPs
using the'Deterministic Logic Built-in Self-Test' (LBIST) controller asthetestengine. Toachieve better
coverage fortheself-test ofcomplex cores likeCortex-R5F, on-chip logic BIST isthepreferred solution
over software based self-test.
There aretwoSTC modules implemented onthisdevice. STC1 forredundant CPUs andtheir µSCU
block. STC2 forthetwonHET modules. The STC module provides thecapability totestredundant IPsin
parallel orindividually.
10.1.1 Self-Test Controller Features
The self-test controller hasthefollowing features:
*Capable ofrunning thecomplete testaswell asrunning asingle ormultiple testsets (intervals) ata
time.
-Ability tocontinue from thelastexecuted interval aswell astheability torestart from thebeginning
(first interval).
*Support oftwological segments. Figure 10-1 shows theimplementation with multiple segments. Each
interval canbemapped toalogical segment. Asegment identifier corresponding toeach interval is
stored intheself-test ROM.
-Segment 0:Segment 0hastheadditional capability totestredundant logic orcores inoneofthe
following modes:
*Parallel Mode: Redundant logic cores aretested inparallel with thesame patterns buthave a
dedicated signature generator. This isused inthesafety critical redundant logic thatruns in
lock-step. Figure 10-2 andFigure 10-3 show thisconfiguration forSTC1 andSTC2.
*Split Mode: Each redundant logic istested individually. Figure 10-4 andFigure 10-5 show this
configuration forSTC1.
-Allredundant cores (orIPs) within asegment have their own dedicated DBIST controllers.
-Other segment (segment 1)cantestonly asingle logic segment during theself-test run.
-Ability toselect segment forwhich thefirstinterval isselected forrun.
*Complete isolation oftheself-tested core from therestofthesystem during theself-test run
-The self-tested CPU core master bustransaction signals areconfigured tobeinidlemode during
theself-test run
-Any master access totheCPU core under self-test (example: DMA access toCPU TCM) willbe
held until thecompletion oftheself-test
*Ability tocapture thefailure segment andinterval number
*Timeout counter fortheself-test runasafail-safe feature
*Able toread theMISR data (shifted from LBIST controller) ofthelastexecuted interval oftheself-test
runfordebugging purposes
*STCCLK determines theself-test execution speed, STC clock divider (STCCLKDIV) register isused to
divide oneofthesystem clocks togenerate STCCLK. The divider canbeconfigured persegment. For
STC1, GCLK1 isdivided down; forSTC2, VCLK2 isdivided down togenerate STCCLK.
*Low-frequency shift. Programmable clock divider register inside STC toreduce theshift frequency in
order toreduce theshift power.

<!-- Page 430 -->

General Description www.ti.com
430 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.1.2 Terminology
Interval: Aninterval corresponds toatestsetthatisthebasic testunitfortheSTC module
Segment: Aself-test segment corresponds toaportion oftheunique/discrete safety critical logic which
canbetested inisolation from therestofthesystem bytheself-test controller andDBIST logic. Aself-test
segment may correspond toalogic likeCPU core (forexample, Cortex-R5F) oranIP(forexample, µSCU
ornHET) orasub-system.
The assignment ofsegments todigital logic isdevice dependent.
NOTE: Allsegments need torunsequentially during theself-test run. Itisnotrecommended to
switch from onesegment toanother before theself-test forthecurrent segment is
completed. The segment intervals intheSTC ROM areorganized sequentially.
10.1.3 STC Block Diagram
STC module provides aninterface totheLBIST controller implemented ontheCPUs andthenHET
modules. There aretwoseparate STC modules implemented: oneforredundant Cortex-R5F CPUs and
µSCU andanother oneforthenHET modules. Each STC module comprises ofthesame basic blocks and
hassame features andfunctionality.
The STC module iscomposed offollowing blocks oflogic:
*ROM Interface
*FSM andSequence Control
*Register Block
*Peripheral Bus Interface (VBUSP Interface)
10.1.3.1 ROM Interface
This block handles theROM address andcontrol signal generation toread theself-test microcode from
theROM. The testmicrocode andgolden signature value foreach interval arestored inROM.
10.1.3.1.1 FSM andSequence Control
This block generates control signal anddata totheLBIST controller based ontheseed, test_type and
scan chain depth.
10.1.3.1.2 Clock Control
The clock controller sub-block handles theinternal clock selection andgeneration fortheROM, LBIST
controller andlogic under test.
The clock control ratio canbeprogrammed inSTC module byprogramming STCCLKDIV register.
10.1.3.2 Register Block
This block handles thecontrol oftheself-test controller. This block contains various configuration and
status registers thatprovide theresult ofaself-test run. These registers arememory mapped and
accessible through thePeripheral Bus (VBUSP) Interface. This block controls thereseeding (reloading the
existing seed ofthePRPG) intheLBIST controller.

<!-- Page 431 -->

PCRROM
InterfaceESM
ROMClock Controller
FSM
and
Sequence
Controller
COMP
BLKsegment_Reset
misr_outGlobal Clock
Controller
STC
STC REG
BLOCKSTC_BYPASS/
ATE Interface
VBUSP
Inteface
Test
ControllerSEG1
Bisted SCU
including
DBIST/c109SEG0
Bisted CPU1
and CPU2
DBISTincluding
www.ti.com General Description
431 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.1.3.3 Peripheral Bus (VBUSP) Interface
STC control registers areaccessed through Peripheral Bus (VBUSP) Interface. During application
programming, configuration registers areprogrammed through thePeripheral Bus Interface toenable and
runtheself-test controller.
Figure 10-1. Block Diagram forSTC With Multiple Segments

<!-- Page 432 -->

PCRROM
InterfaceESM
ROMClock Controller
FSM
and
Sequence
ControllerCOMP
BLK2
COMP
BLK1Core_Reset
misr_out
misr_outGlobal Clock
Controller
STC
STC REG
BLOCKSTC_BYPASS/
ATE Interface
VBUSP
Inteface
Test
ControllerDBIST
CNTRL2CompareDBIST
CNTRL1CPU1
Cortex-R5F
(Bisted CORE)
CPU2
Cortex-R5F
(Bisted CORE)
General Description www.ti.com
432 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleFigure 10-2. STC1 -Segment 0Redundant Core Architecture With CCM-R5F (Parallel Mode)

<!-- Page 433 -->

PCRROM
InterfaceESM
ROMClock Controller
FSM
and
Sequence
ControllerCOMP
BLK2
COMP
BLK1nHET_Reset
misr_outGlobal Clock
Controller
STC
STC REG
BLOCKSTC_BYPASS/
ATE Interface
VBUSP
Inteface
Test
ControllerDBIST
CNTRL2DBIST
CNTRL1nHET1
nHET2misr_out
www.ti.com General Description
433 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleFigure 10-3. STC2 -Segment 0Redundant Architecture (Parallel Mode)

<!-- Page 434 -->

PCRROM
InterfaceESM
ROMClock Controller
FSM
and
Sequence
ControllerCOMP
BLK2
COMP
BLK1Core_Reset
misr_out
misr_outGlobal Clock
Controller
STC
STC REG
BLOCKSTC_BYPASS/
ATE Interface
VBUSP
Inteface
Test
ControllerDBIST
CNTRL2CCM-R5FDBIST
CNTRL1CPU1
Cortex-R5F
(Bisted CORE)
CPU2
(Bisted CORE)Cortex-R5F
General Description www.ti.com
434 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleFigure 10-4. STC1 -Segment 0Redundant Core Architecture With Only CPU1 Selected
Modules highlighted inredwillnotbeenabled fortestwhile testing CORE1 only inaredundant system.

<!-- Page 435 -->

PCRROM
InterfaceESM
ROMClock Controller
FSM
and
Sequence
ControllerCOMP
BLK2
COMP
BLK1Core_Reset
misr_out
misr_outGlobal Clock
Controller
STC
STC REG
BLOCKSTC_BYPASS/
ATE Interface
VBUSP
Inteface
Test
ControllerDBIST
CNTRL2CCM-R5FDBIST
CNTRL1CPU1
Cortex-R5F
(Bisted CORE)
CPU2
Cortex-R5F
(Bisted CORE)
www.ti.com General Description
435 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleFigure 10-5. STC1 -Segment 0Redundant Core Architecture With Only CPU2 Selected
Modules highlighted inredwillnotbeenabled fortestwhile testing CORE2 only inaredundant system.

<!-- Page 436 -->

STC Module Assignments www.ti.com
436 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.2 STC Module Assignments
There aretwoinstances ofSTC modules available onthisdevice, seeTable 10-1 .STC1 isused for
running self-test ontheredundant CPUs andµSCU. STC2 isused forrunning self-test onthetwonHET
modules. The twoinstances areindependent ofeach other.
Table 10-1. STC Module Assignments
Module Segments Targeted IPNumber of
IntervalsSTCCLK Derived
From Note
STC1 Segment 0 CPU1 andCPU2 125 GCLK1 Segment 0allows CPU1 and
CPU2 tobetested inparallel
orindividually.
Segment 1 µSCU (ACP Block) 3 GCLK1 None
STC2 Segment 0 nHET1 andnHET2 57 VCLK2 Segment 0allows nHET1 and
nHET2 tobetested inparallel
orindividually.

<!-- Page 437 -->

N
YY
YN
YY
N
Y
Read Self test status registers.
Retrieve MUT state if required.Self test execution starts MUT
in safe modeMUT idle/safe
acknowledge1) For the logic under test, Save any Critical data/states
if required (context switch)
2) Configure the Segment/core/IP under test to be in
Idle/Safe mode (ex: the CPU in WFI mode)
3) Program STC enable(MUT)Program Core_SEL
for Segment0Read back CORE_SEL.
Is the value the same?Configure the STC run:
-STCCLKDIV registers for clock
division ratio of the source clock
for each segment
-Number of intervals for the run
and interval start type (restart,
continue from previous interval,
preload)
-Program the time out counterIs ST_ACTIVE Key
Active? Program SYS GHVSRC to select PLL and
program CLKCNTL registers
Self-test Done?Is SYS_NRST = 1SYSTEM RESET
Is PLL_LOCK = 1?
N
NN
www.ti.com STC Programmers Flow
437 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.3 STC Programmers Flow
Figure 10-6. STC Programmers Flow Chart
The steps shown inredcanbebypassed forself-test with single core only.

<!-- Page 438 -->

Application Self-Test Flow www.ti.com
438 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.4 Application Self-Test Flow
This section describes theSTC module configuration andtheapplication self-test flow thatyoushould
follow forsuccessful execution. The following twoconfigurations must bepart oftheSTC initialization
code:
*STC clock rate configuration, STC clock divider (STCCLKDIV) register isused todivide system clock
togenerate STCCLK foreach segment.
*Clear SYSESR register before triggering anSTC test.
10.4.1 STC Module Configuration
*Configure thetestinterval count using STCGCR0[31:16] register. STC1, segment 0supports a
maximum of125intervals, STC1 segment 1supports amaximum of3andSTC2 supports amaximum
of57intervals. The intervals within each group canberanindividually orsequentially atonetime. If
thetestintervals arerunindividually, theuser software canspecify totheself-test controller whether to
continue therunfrom thenext interval ortorestart from interval 0using bitSTCGCR0[0]. This bitgets
reset after thecompletion oftheself-test run.
*Configure self-test runtimeout counter preload register STCTPR. This register contains thetotal
number ofVBUS clock cycles itwilltake before aself-test timeout error (TO_ERR) willbetriggered
after theinitiation oftheself-test run.
*Configure Segment 0forparallel orserial execution foreach ofthe2elements tobetested (primary
andredundant logic).
*Enable self-test bywriting theenable keytoSTCGCR1 register.
10.4.2 Context Saving -CPU
STC generates aCPU reset after completion ofeach testregardless ofpass orfail.You canruntheSTC
testduring startup orcandivide STC intosubsets of1ormore intervals andexecuted during application
runtime.
The STC testisadestructive testsuch thatcontent within theelement being tested may need tobe
preserved.
IfSTC isrunonly onstartup, theuser software need notsave theCPU contents since thereset atthe
completion ofthetestwillbefollowed bynormal device initializations/startup configuration. During startup,
theuser code should check theSTCGSTAT register fortheself-test status before going totheapplication
software.
IfSTC isdivided intointervals andranduring application runtime, theuser software must save theCPU
contents andreload them after each CPU reset caused bythecompletion oftheSTC testinterval. The
check forSTC status should bypass theSTC runifthereset iscaused byacompleted testexecution. The
STCGSTAT register should bechecked fortheself-test status before returning totheapplication software.
Following aresome oftheregisters thatarerequired tobebacked upbefore andrestored after self-test:
1.CPU core registers: allmodes R0-R15, PC,CPSR
2.CP15 System Control Coprocessor registers: MPU control andconfiguration registers, Auxiliary Control
Register used toEnable ECC, Fault Status Register
3.CP13 Coprocessor Registers: FPU configuration registers, General Purpose Registers
4.Hardware Break Point andwatch point registers: BVR, BSR, WVR, WSR
Formore information ontheCPU reset, refer totheARM ®Cortex ®-R5F Technical Reference Manual .
NOTE: Check allreset source flags intheSYSESR register after aCPU BIST execution. Ifaflagin
addition toCPU reset isset,clear theCPU reset flagandservice theother reset sources
accordingly.

<!-- Page 439 -->

www.ti.com Application Self-Test Flow
439 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.4.3 Entering CPU IdleMode
After enabling theSTC testbywriting theSTC enable key, thetestistriggered only after theCPU istaken
toidlemode byexecuting theCPU IdleInstruction asm( "WFI").
10.4.4 Entering nHET IdleMode
After enabling theSTC testbywriting theSTC enable key, thetestistriggered only after thenHET
module isputinreset state bywriting tobit0theHETGCR Global Configuration Register inthenHET
module.
10.4.5 Self-Test Completion andError Generation
Attheendofeach interval, the128bitMISR value (reflected inregisters CPUx_CURMISR[3:0]) from the
DBIST controller isshifted intotheSTC. This iscompared with thegolden MISR value stored intheROM.
AttheendofaCPU self-test, theSTC controller updates thestatus flags intheGlobal Status Register
(STCGSTAT) andresets theCPU. Incase ofaMISR mismatch oratesttimeout, anerror isgenerated
through theESM module. TEST_ERR signal isasserted when anMISR miscompare occurs during the
self-test. ATO_ERR isasserted when atimeout occurs during theself-test, meaning thetestcould not
complete within thetime specified inthetimeout counter preload register STCTPR. However, atthe
device level, these twoerrors arecombined andmapped toasingle ESM channel. Toidentify which error
occurred, user software must check theglobal status register (STCGSTAT) andfailstatus register
STCFSTAT intheESM interrupt service routine.
Figure 10-7 illustrates theself-test hardware execution flow chart, based ontheassumption thatthe
device hasgone through startup, necessary clocks initialized andSYSESR register bitscleared.

<!-- Page 440 -->

NO
YES
YESmisr mismatch?Increment CICRNO
YESNOIntervals doneYESYESStartNO NO
Has the segment
or core/ip under test asserted
its idle state acknowledge
signal
The STC reads the MICRO code from ROM
and saves the seed_cntrl_data and Gloden
MISR for the next interval (CICR + 1)
(CICR is 0 for the first time selftest run);
The STC reads the seed_data into buffers
based on seed_cntrl_bits generate DBIST
control signals and shift the seed through
shadow_si ports of DBIT
Read the MISR value into STC from the dbist,
to compare with Golden MISRThe STC enable forces the CPU bus
to idle transaction modeIs Self Test
Enabled?
All patterns
completed?
End of Self Test
(Disable the STC_ENA Key)
The STC Updates the STC status registers
and generates CPU reset and stc_testerr_o
End of Self Test
(Disable the STC_ENA Key)Set the STC complete flag in the
STC status registers.
STC asserts the CPU reset
Application Self-Test Flow www.ti.com
440 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleFigure 10-7. Self-Test Hardware Execution Flow Chart

<!-- Page 441 -->

www.ti.com STC1 Segment 0(CPU) Test Coverage andDuration
441 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.5 STC1 Segment 0(CPU) Test Coverage andDuration
The testcoverage andnumber oftestexecution cycles (STCCLK) foreach testinterval areshown in
Table 10-2.
Table 10-2. STC1 Segment 0Test Coverage andDuration
Intervals Test Coverage (%) Test Time (Cycles)
0 0 0
1 56.85 1629
2 64.19 3258
3 68.76 4887
4 71.99 6516
5 75.00 8145
6 76.61 9774
7 78.08 11403
8 79.20 13032
9 80.18 14661
10 81.03 16290
11 81.90 17919
12 82.58 19548
13 83.24 21177
14 83.73 22806
15 84.15 24435
16 84.52 26064
17 84.90 27693
18 85.26 29322
19 85.68 30951
20 86.05 32580
21 86.40 34209
22 86.68 35838
23 86.94 37467
24 87.21 39096
25 87.48 40725
26 87.74 42354
27 87.98 43983
28 88.18 45612
29 88.38 47241
30 88.56 48870
31 88.75 50499
32 88.93 52128
33 89.10 53757
34 89.23 55386
35 89.41 57015
36 89.55 58644
37 89.70 60273
38 89.83 61902
39 89.96 63531
40 90.10 65160
41 90.23 66789
42 90.33 68418
43 90.43 70047

<!-- Page 442 -->

STC1 Segment 0(CPU) Test Coverage andDuration www.ti.com
442 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleTable 10-2. STC1 Segment 0Test Coverage andDuration (continued)
Intervals Test Coverage (%) Test Time (Cycles)
44 90.57 71676
45 90.67 73305
46 90.77 74934
47 90.89 76563
48 91.00 78192
49 91.08 79821
50 91.17 81450
51 91.26 83079
52 91.35 84708
53 91.42 86337
54 91.52 87966
55 91.63 89595
56 91.73 91224
57 91.81 92853
58 91.89 94482
59 91.97 96111
60 92.05 97740
61 92.11 99369
62 92.17 100998
63 92.24 102627
64 92.31 104256
65 92.38 105885
66 92.44 107514
67 92.51 109143
68 92.57 110772
69 92.63 112401
70 92.70 114030
71 92.76 115659
72 92.82 117288
73 92.92 118917
74 92.98 120546
75 93.06 122175
76 93.12 123804
77 93.20 125433
78 93.25 127062
79 93.31 128691
80 93.36 130320
81 93.42 131949
82 93.48 133578
83 93.55 135207
84 93.60 136836
85 93.66 138465
86 93.71 140094
87 93.76 141723
88 93.81 143352
89 93.86 144981
90 93.91 146610

<!-- Page 443 -->

www.ti.com STC1 Segment 0(CPU) Test Coverage andDuration
443 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleTable 10-2. STC1 Segment 0Test Coverage andDuration (continued)
Intervals Test Coverage (%) Test Time (Cycles)
91 93.96 148239
92 94.01 149868
93 94.07 151497
94 94.12 153126
95 94.17 154755
96 94.22 156384
97 94.27 158013
98 94.32 159642
99 94.37 161271
100 94.41 162900
101 94.46 164529
102 94.50 166158
103 94.54 167787
104 94.60 169416
105 94.64 171045
106 94.68 172674
107 94.72 174303
108 94.78 175932
109 94.82 177561
110 94.86 179190
111 94.91 180819
112 94.95 182448
113 94.99 184077
114 95.04 185706
115 95.08 187335
116 95.15 188964
117 95.19 190593
118 95.23 192222
119 95.27 193851
120 95.31 195480
121 95.35 197109
122 95.39 198738
123 95.43 200367
124 95.47 201996
125 95.51 203625
Table 10-3 gives thetypical STC execution times for40intervals and125intervals atdifferent clock rates.
You canchoose thenumber ofintervals toberunbased onthecoverage needed andallowed time for
STC execution.
Table 10-3. Typical Execution Times forSTC1 Segment 0
Number ofIntervals Coverage@GCLK1 =330MHz
STCCLK =110MHz@GCLK1 =300MHz
STCCLK =100MHz
40 >90% 592.4 µS 651.6 µS
125 >95% 1.8511 mS 2.036 mS

<!-- Page 444 -->

STC1 Segment 1(µSCU) Test Coverage andDuration www.ti.com
444 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.6 STC1 Segment 1(µSCU) Test Coverage andDuration
The testcoverage andnumber oftestexecution cycles (STCCLK) foreach testinterval areshown in
Table 10-4.
Table 10-4. STC1 Segment 1Test Coverage andDuration
Intervals Test Coverage (%) Test Time (Cycles)
0 0 0
1 84.79 1629
2 87.96 3258
3 88.33 4887
Table 10-5 gives thetypical STC execution times for3intervals atdifferent clock rates. You canchoose
thenumber ofintervals toberunbased onthecoverage needed andallowed time forSTC execution.
Table 10-5. Typical Execution Times forSTC1 Segment 1
Number ofIntervals Coverage@GCLK1 =330MHz
STCCLK =110MHz@GCLK1 =300MHz
STCCLK =100MHz
3 >88% 44.43 µS 48.87 µS
10.7 STC2 (nHET) Test Coverage andDuration
The testcoverage andnumber oftestexecution cycles (STCCLK) foreach testinterval areshown in
Table 10-6.
Table 10-6. STC2 Test Coverage andDuration
Intervals Test Coverage (%) Test Time (Cycles)
0 0 0
1 70.01 1365
2 77.89 2730
3 81.73 4095
4 84.11 5460
5 86.05 6825
6 87.78 8190
7 88.96 9555
8 89.95 10920
9 90.63 12285
10 91.20 13650
11 91.60 15015
12 92.02 16380
13 92.37 17745
14 92.66 19110
15 92.87 20475
16 93.04 21840
17 93.26 23205
18 93.47 24570
19 93.67 25935
20 93.82 27300
21 93.96 28665
22 94.12 30030

<!-- Page 445 -->

www.ti.com STC2 (nHET) Test Coverage andDuration
445 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) ModuleTable 10-6. STC2 Test Coverage andDuration (continued)
Intervals Test Coverage (%) Test Time (Cycles)
23 94.24 31395
24 94.38 32760
25 94.50 34125
26 94.72 35490
27 94.80 36855
28 94.90 38220
29 94.97 39585
30 95.03 40950
31 95.10 42315
32 95.16 43680
33 95.22 45045
34 95.27 46410
35 95.33 47775
36 95.42 49140
37 95.49 50505
38 95.54 51870
39 95.66 53235
40 95.69 54600
41 95.75 55965
42 95.79 57330
43 95.82 58695
44 95.85 60060
45 95.91 61425
46 95.95 62790
47 95.99 64155
48 96.01 65520
49 96.04 66885
50 96.07 68250
51 96.09 69615
52 96.12 70980
53 96.15 72345
54 96.19 73710
55 96.24 75075
56 96.29 76440
57 96.41 77805
Table 10-7 gives thetypical STC execution times for9intervals and57intervals atdifferent clock rates.
You canchoose thenumber ofintervals toberunbased onthecoverage needed andallowed time for
STC execution.
Table 10-7. Typical Execution Times forSTC2
Number ofIntervals Coverage@VCLK =110MHz
STCCLK =110MHz@VCLK =150MHz
STCCLK =75MHz
9 >90% 111.68 µS 163.8 µS
57 >96% 707.3 µS 1.038 mS

<!-- Page 446 -->

STC Control Registers www.ti.com
446 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8 STC Control Registers
STC control registers areaccessed through Peripheral Bus (VBUSP) interface. Read andwrite access in
8,16, and32bitaresupported.
The base address forthecontrol registers ofSTC1 isFFFF E600h. The base address forthecontrol
registers ofSTC2 isFFFF 0800h.
NOTE: Insuspend mode, allregisters canbewritten irrespective ofuser orprivilege mode and
reads willnotclear the'read-clear' (RC) bits.
Table 10-8. STC Control Registers
Offset Acronym Register Description Section
00h STCGCR0 STC Global Control Register 0 Section 10.8.1
04h STCGCR1 STC Global Control Register 1 Section 10.8.2
08h STCTPR Self-Test Run Timeout Counter Preload Register Section 10.8.3
0Ch STCCADDR1 STC Current ROM Address Register -CORE1 Section 10.8.4
10h STCCICR STC Current Interval Count Register Section 10.8.5
14h STCGSTAT Self-Test Global Status Register Section 10.8.6
18h STCFSTAT Self-Test FailStatus Register Section 10.8.7
1Ch CORE1_CURMISR3 CORE1 Current MISR Register Section 10.8.8
20h CORE1_CURMISR2 CORE1 Current MISR Register Section 10.8.8
24h CORE1_CURMISR1 CORE1 Current MISR Register Section 10.8.8
28h CORE1_CURMISR0 CORE1 Current MISR Register Section 10.8.8
2Ch CORE2_CURMISR3 CORE2 Current MISR Register Section 10.8.9
30h CORE2_CURMISR2 CORE2 Current MISR Register Section 10.8.9
34h CORE2_CURMISR1 CORE2 Current MISR Register Section 10.8.9
38h CORE2_CURMISR0 CORE2 Current MISR Register Section 10.8.9
3Ch STCSCSCR Signature Compare Self-Check Register Section 10.8.10
40h STCCADDR2 STC Current ROM Address Register -CORE2 Section 10.8.11
44h STCCLKDIV STC Clock Divider Register Section 10.8.12
48h STCSEGPLR STC Segment First Preload Register Section 10.8.13

<!-- Page 447 -->

www.ti.com STC Control Registers
447 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.1 STC Global Control Register 0(STCGCR0)
This register isdescribed inFigure 10-8 andTable 10-9.
NOTE: Onapower-on reset orsystem reset, thisregister gets reset toitsdefault values.
Figure 10-8. STC Global Control Register 0(STCGCR0) [offset =00h]
31 16
INTCOUNT
R/WP-1
15 11 10 8 7 2 1 0
Reserved CAP_IDLE_CYCLE Reserved RS_CNT
R-0 R/WP-1 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 10-9. STC Global Control Register 0(STCGCR0) Field Descriptions
Bit Field Value Description
31-16 INTCOUNT Number ofintervals ofself-test run.
0-FFFFh This register specifies thenumber ofintervals torunfortheself-test run. This corresponds
tothenumber ofintervals toberunfrom thevalue reflected inthecurrent interval counter.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 CAP_IDLE_CYCLE Idlecycle before andafter thecapture clock.
0 Disabled
1 Enabled
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 RS_CNT Restart orContinue
This bitspecifies whether tocontinue therunfrom next interval onwards ortorestart from
interval 0.This bitgets reset after thecompletion ofaself-test run.
0 Continue STC runfrom theprevious interval.
1h Restart STC runfrom interval 0.
2h-3h Reserved

<!-- Page 448 -->

STC Control Registers www.ti.com
448 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.2 STC Global Control Register 1(STCGCR1)
This register isdescribed inFigure 10-9 andTable 10-10 .
NOTE: Onapower-on reset orsystem reset, thisregister resets toitsdefault values. Also, this
register automatically resets toitsdefault values atthecompletion ofaself-test run.
The SEG0_CORE_SEL bitsmust bewritten firstbefore theSTC_ENA bitsarewritten, in
order fortheSTC toproperly initiate theselected core forself-test.
Figure 10-9. STC Global Control Register 1(STCGCR1) [offset =04h]
31 16
Reserved
R-0
15 12 11 8 7 4 3 0
Reserved SEG0_CORE_SEL Reserved STC_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after nPORST (power onreset) orSystem reset
Table 10-10. STC Global Control Register 1(STCGCR1) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 SEG0_CORE_SEL Selects cores inSegment 0forself-test. These bitscanbeprogrammed only when
SEG0_CORE_SEL is0000. Once thefield iswritten itignores allfurther writes until the
self-test sequence completes. This istomaintain coherency forself-test runs.
5h Select only Core1 forself-test.
Ah Select only Core2 forself-test.
Allother values Select both cores forself-test inparallel.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 STC_ENA Self-test runenable key.
Ah Self-test runisenabled.
Allother values Self-test runisdisabled.

<!-- Page 449 -->

www.ti.com STC Control Registers
449 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.3 Self-Test Run Timeout Counter Preload Register (STCTPR)
This register isdescribed inFigure 10-10 andTable 10-11 .
NOTE: Onapower-on reset orsystem reset, thisregister gets reset toitsdefault values.
Figure 10-10. Self-Test Run Timeout Counter Preload Register (STCTPR) [offset =08h]
31 0
RTOD
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after nPORST (power onreset) orSystem reset
Table 10-11. Self-Test Run Timeout Counter Preload Register (STCTPR)
Bit Field Description
31-0 RTOD Self-test timeout count preload.
This register contains thetotal number ofVBUS clock cycles itwilltake before anself-test timeout error
(TO_ERR) willbetriggered after theinitiation oftheself-test run. This isafailsafe feature toprevent thedevice
from hanging upduetoarunaway testduring theself-test.
The preload count value gets loaded intotheself-test time outdown counter whenever aself-test runis
initiated (STC_KEY isenabled) andgets disabled oncompletion ofaself-test run.

<!-- Page 450 -->

STC Control Registers www.ti.com
450 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.4 STC Current ROM Address Register -CORE1 (STCCADDR1)
This register isdescribed inFigure 10-11 andTable 10-12 .
NOTE: When theRS_CNT bitinSTCGCR0 issettoa1onthestart ofaself-test run, orona
power-on reset orsystem reset, thisregister resets toallzeroes.
Figure 10-11. STC Current ROM Address Register (STCCADDR1) [offset =0Ch]
31 0
ADDR
R-0
LEGEND: R=Read only; -n=value after nPORST (power onreset) orSystem reset
Table 10-12. STC Current ROM Address Register (STCCADDR1) Field Descriptions
Bit Field Description
31-0 ADDR Current ROM Address
This register reflects thecurrent ROM address (address ormicro code load) accessed during self-test
Segment0 -Core1 andother segments. This isthecurrent value oftheSTC program counter.
10.8.5 STC Current Interval Count Register (STCCICR)
This register isdescribed inFigure 10-12 andTable 10-13 .
NOTE: When theRS_CNT bitinSTCGCR0 issettoa1oronapower-on reset, thecurrent interval
counter resets tothedefault value.
Figure 10-12. STC Current Interval Count Register (STCCICR) [offset =10h]
31 16
CORE2_ICOUNT
R-0
15 0
CORE1_ICOUNT
R-0
LEGEND: R=Read only; -n=value after reset
Table 10-13. STC Current Interval Count Register (STCCICR) Field Descriptions
Bit Field Description
31-16 CORE2_ICOUNT Interval Number
This specifies thelastexecuted interval number forCore2 incase ofself-test being runonSegement0
redundant cores.
15-0 CORE1_ICOUNT Interval Number
This specifies thelastexecuted interval number forCore1 incase ofself-test being runonSegment0 or
anyother segments.

<!-- Page 451 -->

www.ti.com STC Control Registers
451 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.6 Self-Test Global Status Register (STCGSTAT)
This register isdescribed inFigure 10-13 andTable 10-14 .
NOTE: The twostatus bitscanbecleared totheir default values onawrite of1tothebits.
Additionally when theSTC_ENA keyinSTCGCR1 iswritten from adisabled state toan
enabled state, thetwostatus flags getcleared totheir default values. This register gets reset
toitsdefault value with power-on reset assertion.
Figure 10-13. Self-Test Global Status Register (STCGSTAT) [offset =14h]
31 16
Reserved
R-0
15 12 11 8 7 2 1 0
Reserved ST_ACTIVE Reserved TEST_FAIL TEST_DONE
R-0 R-5h R-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode; -n=value after reset
Table 10-14. Self-Test Global Status Register (STCGSTAT) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 ST_ACTIVE This field indicates iftheself-test isactive.
Ah Self-test isactive.
Allother values Self-test isnotactive.
This willbesetinthecycle after CORE_SEL isprogrammed. This willbereset once theSTC
generated theCPU reset after completion oftheself-test.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 TEST_FAIL Test Fail
0 Self-test runhasnotfailed.
1 Self-test runhasfailed.
0 TEST_DONE Test Done
0 Self-test runisnotcompleted.
1 Self-test runiscompleted.
The testdone flagissettoa1foranyofthefollowing conditions:
1.When theSTC runiscomplete without anyfailure
2.When afailure occurs onaSTC run
3.When atimeout failure occurs
Reset isgenerated totheCPU onwhich theSTC runisbeing performed when TEST_DONE
goes high (the testiscompleted).

<!-- Page 452 -->

STC Control Registers www.ti.com
452 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.7 Self-Test FailStatus Register (STCFSTAT)
This register isdescribed inFigure 10-14 andTable 10-15 .
NOTE: The three status bitscanbecleared totheir default values onawrite of1tothebits.
Additionally when theSTC_ENA keyinSTCGCR1 iswritten from adisabled state toan
enabled state, thethree status bitsgetcleared totheir default values. This register gets reset
toitsdefault value with power-on reset assertion.
When multiple segments areenabled inaself-test run, theSTC willindicate theself-test
complete onthefirstfailed interval corresponding toasegment. The subsequent segments
willnotberun. FSEG_ID bitsinthisregister indicate which segment failed.
Figure 10-14. Self-Test FailStatus Register (STCFSTAT) [offset =18h]
31 16
Reserved
R-0
15 5 4 3 2 1 0
Reserved FSEG_ID TO_ERR CORE2_FAIL CORE1_FAIL
R-0 RCP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode; -n=value after reset
Table 10-15. Self-Test FailStatus Register (STCFSTAT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-3 FSEG_ID Failed Segment Number
0 Segment 0Failed
1 Segment 1Failed
Allother values Reserved
2 TO_ERR Timeout Error
0 Notime outerror occurred.
1 Self-test runfailed duetoatimeout error.
1 CORE2_FAIL CORE2 failure infoforsegment 0only.
0 NoMISR mismatch forCORE2.
1 Self-test runfailed duetoMISR mismatch forCORE2.
0 CORE1_FAIL CORE1 failure infoforsegment 0only.
0 NoMISR mismatch forCORE1.
1 Self-test runfailed duetoMISR mismatch forCORE1.

<!-- Page 453 -->

www.ti.com STC Control Registers
453 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.8 CORE1 Current MISR Registers (CORE1_CURMISR[3:0])
This register isdescribed inFigure 10-15 through Figure 10-18 andTable 10-16 .
NOTE: This register gets reset toitsdefault value with power-on orsystem reset assertion.
Figure 10-15. CORE1 Current MISR Register (CORE1_CURMISR3) [offset =1Ch]
31 16
MISR[31:16]
R-0
15 0
MISR[15:0]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-16. CORE1 Current MISR Register (CORE1_CURMISR2) [offset =20h]
31 16
MISR[63:48]
R-0
15 0
MISR[47:32]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-17. CORE1 Current MISR Register (CORE1_CURMISR1) [offset =24h]
31 16
MISR[95:80]
R-0
15 0
MISR[79:64]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-18. CORE1 Current MISR Register (CORE1_CURMISR0) [offset =28h]
31 16
MISR[127:112]
R-0
15 0
MISR[111:96]
R-0
LEGEND: R=Read only; -n=value after reset
Table 10-16. CORE1 Current MISR Register (CORE1_CURMISRn) Field Descriptions
Bit Field Description
127-0 MISR MISR data from CORE1
This register contains theMISR data from theCORE1 forthemost recent interval incase ofsegment 0andall
other segments. This value iscompared with theGOLDEN MISR value copied from ROM.

<!-- Page 454 -->

STC Control Registers www.ti.com
454 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.9 CORE2 Current MISR Registers (CORE2_CURMISR[3:0])
This register isdescribed inFigure 10-19 through Figure 10-22 andTable 10-17 .
NOTE: This register gets reset toitsdefault value with power-on orsystem reset assertion.
Figure 10-19. CORE2 Current MISR Register (CORE2_CURMISR3) [offset =2Ch]
31 16
MISR[31:16]
R-0
15 0
MISR[15:0]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-20. CORE2 Current MISR Register (CORE2_CURMISR2) [offset =30h]
31 16
MISR[63:48]
R-0
15 0
MISR[47:32]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-21. CORE2 Current MISR Register (CORE2_CURMISR1) [offset =34h]
31 16
MISR[95:80]
R-0
15 0
MISR[79:64]
R-0
LEGEND: R=Read only; -n=value after reset
Figure 10-22. CORE2 Current MISR Register (CORE2_CURMISR0) [offset =38h]
31 16
MISR[127:112]
R-0
15 0
MISR[111:96]
R-0
LEGEND: R=Read only; -n=value after reset
Table 10-17. CORE2 Current MISR Register (CORE2_CURMISRn) Field Descriptions
Bit Field Description
127-0 MISR MISR data from CORE2
This register contains theMISR data from theCORE2 forthemost recent interval incase ofsegment 0l.This
value iscompared with theGOLDEN MISR value copied from ROM.

<!-- Page 455 -->

www.ti.com STC Control Registers
455 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.10 Signature Compare Self-Check Register (STCSCSCR)
This register isdescribed inFigure 10-23 .This register isused toenable theself-check feature ofthe
CPU Self-Test Controller's (STC) signature compare logic. Self-check canonly bedone fortheSTC
interval 0bysetting theRS_CNT bitinSTCGCR0 to1torestart theself-test. The STC runwillfailfor
signature miss-compare, provided thesignature compare logic isoperating correctly. Toproceed with
regular CPU self-test, STCSCSCR should beprogrammed todisable theself-check feature andclear the
RS_CNT bitinSTCGCR0 to0.This register gets reset toitsdefault value with anysystem reset assertion.
Figure 10-23. Signature Compare Self-Check Register (STCSCSCR) [offset =3Ch]
31 16
Reserved
R-0
15 5 4 3 0
Reserved FAULT_INS SELF_CHECK_KEY
R-0 R/WP-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after nPORST (power onreset) orSystem reset
Table 10-18. Signature Compare Self-Check Regsiter (STCSCSCR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 FAULT_INS Enable fault insertion.
0 Nofault isinserted.
1 Insert stuck-at-fault inside CPU sothatSTC signature compare willfail.
3-0 SELF_CHECK_KEY Signature compare logic self-check enable key.
Ah Signature compare logic self-check isenabled. This allows afault tobeinserted using
theFAULT_INS field.
Allother values Signature compare logic self-check isdisabled The FAULT_INS field hasnoeffect in
thiscase.
10.8.11 STC Current ROM Address Register -CORE2 (STCCADDR2)
This register isdescribed inFigure 10-24 andTable 10-19 .
NOTE: When theRS_CNT bitinSTCGCR0 issettoa1onthestart ofaself-test run, orona
power-on reset orsystem reset, thisregister resets toallzeroes.
Figure 10-24. STC Current ROM Address Register (STCCADDR2) [offset =40h]
31 0
ADDR
R-0
LEGEND: R=Read only; -n=value after nPORST (power onreset) orSystem reset
Table 10-19. STC Current ROM Address Register (STCCADDR2) Field Descriptions
Bit Field Description
31-0 ADDR Current ROM Address
This register reflects thecurrent ROM address (address ormicro code load) accessed during self-test
Segment0 -Core2. This isthecurrent value oftheSTC program counter.

<!-- Page 456 -->

STC Control Registers www.ti.com
456 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.12 STC Clock Prescalar Register (STCCLKDIV)
This register isdescribed inFigure 10-25 .This register isused toconfigure STC clock divider ratio for
each segment. STCCLK isderived from thesystem clock (GCLK1 forSTC1 andVCLK2 forSTC2) and
theconfigured ratio isapplied when thecorresponding segment isunder test. The division ratio
programmed inthisregister willhave effect only when thevalue intheCLKDIV field oftheSTCLKDIV
register (FFFF E108h) from SYS2 module iszero. Else thedivision ratio willbetaken from SYS2. This is
done forsoftware compatibility.
NOTE: The clock divider ratio isapplied when thecorresponding segment isunder test.
Figure 10-25. STC Clock Prescalar Register (STCCLKDIV) [offset =44h]
31 27 26 24 23 19 18 16
Reserved CLKDIV0 Reserved CLKDIV1
R-0 R/WP-0 R-0 R/WP-0
15 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after nPORST (power onreset) orSystem reset
Table 10-20. STC Clock Prescalar Register (STCCLKDIV) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CLKDIV0 STCCLK divider forsegment 0.
0-7h Division ratio ofsegment 0willben+1. STCCLK clock willbedivided by(n+1) forsegment 0.
23-19 Reserved 0 Reads return 0.Writes have noeffect.
18-16 CLKDIV1 STCCLK divider forsegment 1.
0-7h Division ratio ofsegment 1willben+1. STCCLK clock willbedivided by(n+1) forsegment 1.
15-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 457 -->

www.ti.com STC Control Registers
457 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.8.13 Segment Interval Preload Register (STCSEGPLR)
This register isdescribed inFigure 10-26 .This register isused tospecify thesegment forwhich thefirst
interval willberun. The address ofthefirstinterval oftheselected segment isloaded totheSTC ROM
address counter before thetestisstarted.
Figure 10-26. Segment Interval Preload Register (STCSEGPLR) [offset =48h]
31 16
Reserved
R-0
15 2 1 0
Reserved SEGID_PLOAD
R-0 RWP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after nPORST (power onreset) orSystem reset
Table 10-21. Segment Interval Preload Register (STCSEGPLR) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 SEGID_PLOAD Specifies thesegment forthefirstinterval toberun.
0 Preload theaddress ofthe1stinterval forSegment 0.
1 Preload theaddress ofthe1stinterval forSegment 1.
Allother values Reserved

<!-- Page 458 -->

STC Configuration Example www.ti.com
458 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.9 STC Configuration Example
The following example provides steps toconfigure STC1 torunself-test onCPUs andtheµSCU unit. It
thatthePLL islocked andselected asthesystem clock source with GCLK1 =330MHz and
HCLK =110MHz prior togoing through thefollowing configurations.
10.9.1 Example: STC1 Self-Test Run
This example explains theconfigurations forrunning STC Test foron40testintervals.
1.Maximum STC clock rate support at330MHz GCLK1 is110MHz. Divide GCLK1 by3toachieve this
clock rate. BitsSTCCLKDIV[26:24] andSTCCLKDIV[18:16] need tobeconfigured.
STCCLKDIV[26:24] =2,STCCLKDIV[18:16] =2
2.Clear CPU RST status bitintheSystem Exception Status Register inthesystem module.
SYSESR[5] =1
3.Configure thetestinterval count inSTC module. Note thatincase ofmultiple segments, segments run
sequentially, oneafter another depending onthenumber ofintervals selected.
STCGCR0[31:16] =40.
4.Configure self-test runtime outcounter preload register.
STCTPR[31:0] =0xFFFFFFFF
5.Optionally, configure SEG0_CORE_SEL bitsinregister STCGCR1 toselect oneoftheredundant
cores. Bydefault bitsSEG0_CORE_SEL areclear, which configures theSTC torunboth redundant
cores inparallel.
6.Enable CPU self-test.
STCGCR1[3:0]= 0xA;
7.Perform acontext save ofCPU state andconfiguration registers thatgetreset onCPU reset.
8.PuttheCPU inidlemode byexecuting theCPU idleinstruction.
asm( "WFI")
9.Upon CPU reset, verify theCPU RST status bitintheSystem Exception Status Register isset.This
also verifies thatnoother resets occurred during theself-test.
SYSESR[5] ==1
10.Check theSTCGSTAT register fortheself-test status.
Check TEST_DONE bitbefore evaluating TEST_FAIL bit.
If(TEST_DONE =1andTEST_FAIL =1),theself-test iscompleted andFailed.
*Read STC FailStatus Register STCFSTAT[2:0] toidentify thetype ofFailure (Timeout, CORE1 fail,
CORE2 fail,FSEG_ID).
Incase there isnofailure (TEST_DONE =1andTEST_FAIL =0),theCPU self-test iscompleted
successfully.
*Recover theCPU status, configuration registers andcontinue theapplication software.

<!-- Page 459 -->

www.ti.com Self-Test Controller Diagnostics
459 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSelf-Test Controller (STC) Module10.10 Self-Test Controller Diagnostics
This section provides therecommended flow fortheself-test controller diagnostics. This testis
recommended tobedone attheapplication startup only, notwith individual interval runs during the
application.
Step 1:Configure theinterval count to1inSTCGCR0 register.
Segment 0
Step 2:Enable theSELF_CHECK_KEY andFAULT_INS bitsintheSTCSCSCR register andkick offthe
self-test byenabling thefirstinterval ofsegment 0.Onthecompletion ofself-test, TEST_FAIL bitwillbe
setintheSTCGSTAT register. Check iftheFSEGID bitsintheSTCFSTAT register aresetto00.
Depending onthesegment 0configuration (parallel orindividual cores), theCORE1_FAIL or
CORE2_FAIL bitswould beset.
Step 3:Disable oneorboth oftheSELF_CHECK_KEY andFAULT_INS bitsintheSTCSCSCR register.
Then restart theself-test byprogramming bit0oftheSTCGCR0 register to1.Onthecompletion ofthe
test, theTEST_FAIL bitwillbecleared intheSTCGSTAT register.
Segment 1(forSTC1 only)
Step 4:Configure theSEGID_PLOAD bitsinSTCSEGPLR register toselect thefirstinterval ofsegment 1.
Configure RS_CNT bitinSTCGCR0 register to1.This willstart theself-test from thefirstinterval ofthe
selected segment. Onthecompletion ofself-test, TEST_FAIL bitwillbesetintheSTCGSTAT register.
Check iftheFSEGID bitsintheSTCFSTAT register aresetto01.
Step 5:Disable oneorboth oftheSELF_CHECK_KEY andFAULT_INS bitsintheSTCSCSCR register.
Then restart theself-test byprogramming bit0oftheSTCGCR register to1.Onthecompletion ofthetest,
theTEST_FAIL bitwillbecleared intheSTCGSTAT register.
After thediagnostics, theapplication cancontinue with theself-test asdescribed inSection 10.4.