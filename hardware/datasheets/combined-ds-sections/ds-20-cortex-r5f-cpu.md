# Cortex-R5F CPU Information

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 69-75 (7 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 69 -->
69TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.5 ARM Cortex-R5F CPU Information
6.5.1 Summary ofARM Cortex-R5F CPU Features
The features oftheARM Cortex-R5F CPU include:
*Aninteger unitwith integral Embedded ICE-RT logic.
*High-speed Advanced Microprocessor Bus Architecture (AMBA) Advanced eXtensible Interfaces (AXI)
forLevel two(L2) master andslave interfaces.
*Floating-Point Coprocessor
*Dynamic branch prediction with aglobal history buffer, anda4-entry return stack
*Low interrupt latency.
*Nonmaskable interrupt.
*Harvard Level one(L1) memory system with:
-32KB ofinstruction cache and 32KB ofdata cache implemented. Both Instruction and data cache
have ECC support.
-ARMv7-R architecture Memory Protection Unit (MPU) with 16regions
*Dual core logic forfault detection insafety-critical applications.
*L2memory interface:
-Single 64-bit master AXIinterface
-64-bit slave AXIinterface tocache memories
-32-bit AXI_Peri ports tosupport lowlatency peripheral ports
*Debug interface toaCoreSight Debug Access Port (DAP).
*Performance Monitoring Unit (PMU).
*Vectored Interrupt Controller (VIC) port.
*AXIaccelerator coherency port (ACP) supporting IOcoherency with write-through cacheable regions
*Ability togenerate ECC onL2data buses andparity ofallcontrol channels
*Both CPU cores inlock-step
*Eight hardware breakpoints
*Eight watchpoints
6.5.2 Dual Core Implementation
The device hastwoCortex-R5F cores, where theoutput signals ofboth CPUs arecompared intheCCM-
R5F unit. Toavoid common mode impacts thesignals oftheCPUs tobecompared aredelayed bytwo
clock cycles asshown inFigure 6-2.

<!-- Page 70 -->
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
Inputs to CPU1cpu2clkcpu1clkOutputs from CPU1 to
the systemOutputs from CPU2 to
the system
Safe values (values
that will force the
ZlWh[}µµ
to inactive states) CCM-R5F
Compare errors
ESMPDx PDy
70TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-2.Dual Core Implementation
6.5.3 Duplicate Clock Tree After GCLK
The CPU clock domain issplit intotwoclock trees, one foreach CPU, with theclock ofthesecond CPU
running atthesame frequency andinphase totheclock ofCPU1. See Figure 6-2.
6.5.4 ARM Cortex-R5F CPU Compare Module (CCM) forSafety
CCM-R5F hastwomajor functions. One istocompare theoutputs oftwoCortex-R5F processor cores and
theVIM modules. The second function isinactivity monitoring, todetect anyfaulted transaction initiated by
thechecker core.
6.5.4.1 Signal Compare Operating Modes
The CCM-R5F module runinone offour operating modes -active compare lockstep, self-test, error
forcing, andself-test error forcing mode. Toselect anoperating mode, adedicated keymust bewritten to
thekeyregister. CPU compare block and VIM compare block have separate keyregisters toselect their
operating modes. Status registers arealso separate forthese blocks.
6.5.4.1.1 Active Compare Lockstep Mode
Inthismode theoutput signals ofboth CPUs andboth VIMs arecompared, andadifference intheoutputs
isindicated bythecompare_error terminal. Formore details about CPU and VIM lockstep comparison,
refer tothedevice technical reference manual.

<!-- Page 71 -->
71TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedCCM-R5F also produces asignal toESM GP1.92 toindicate itscurrent status whether itisoutoflockstep
orisinself-test mode. This ensures thatanylock step fault isreported totheCPU.
6.5.4.1.2 Self-Test Mode
Inself-test mode theCCM-R5F ischecked forfaults, byapplying internally generated, series oftest
patterns tolook forany hardware faults inside themodule. During self-test thecompare error signal is
deactivated. Ifafault ontheCCM-R5F module isdetected, anerror isshown ontheselftest_error pin.
6.5.4.1.3 Error Forcing Mode
Inerror forcing mode atestpattern isapplied totheCPU and VIM related inputs ofthecompare logic to
force anerror atthecompare error signal ofthecompare unit. Error forcing mode isdone separately for
VIM signal compare block andCPU signal compare block. Foreach block, thismode isenabled bywriting
thekeyincorresponding block 'skeyregister.
6.5.4.1.4 Self-Test Error Forcing Mode
Inself-test error forcing mode anerror isforced attheself-test error signal. The compare block isstill
running inlockstep mode andthekeyisswitched tolockstep after oneclock cycle.
Table 6-7.CPU Compare Self-Test Cycles
MODE NUMBER OFGCLK CYCLES
Self-Test Mode 4947
Self-Test Error Forcing Mode 1
Error Forcing Mode 1
Table 6-8.VIMCompare Self-Test Cycles
MODE NUMBER OFVCLK CYCLES
Self-Test Mode 151
Self-Test Error Forcing Mode 1
Error Forcing Mode 1
6.5.4.2 Bus Inactivity Monitor
CCM-R5F also monitors theinputs totheinterconnect coming from thechecker Cortex-R5F core. The
input signals totheinterconnect arecompared against their default clamped values. The checker core
must notgenerate anybustransaction totheinterconnect system asallbustransactions arecarried out
through themain CPU core. Ifany signal value isdifferent from itsclamped value, anerror signal is
generated. The error response incase ofadetected transaction issent toESM.
Inaddition tobusmonitoring thechecker CPU core, theCCM-R5F willalso monitor several other critical
signals from other masters residing inother power domains. This istoensure aninadvertent bus
transaction from anunused power domain canbedetected. Toenable detection ofunwanted transaction
from anunused master, thepower domain inwhich themaster tobemonitored willneed tobeconfigured
inOFF power state through thePMM module.
6.5.4.3 CPU Registers Initialization
Toavoid anerroneous CCM-R5F compare error, theapplication software must ensure that theCPU
registers ofboth CPUs areinitialized with thesame values before theregisters areused, including
function calls where theregister values arepushed onto thestack.
Example routine forCPU register initialization:

<!-- Page 72 -->
.text
.state32
.global __clearRegisters_
.asmfunc
__clearRegisters_:
mov  r0,  lr
mov  r1,  #0x0000
mov  r2,  #0x0000
mov  r3,  #0x0000
mov  r4,  #0x0000
mov  r5,  #0x0000
mov  r6,  #0x0000
mov  r7,  #0x0000
mov  r8,  #0x0000
mov  r9,  #0x0000
mov  r10, #0x0000
mov  r11, #0x0000
mov  r12, #0x0000
mov  r1,  #0x11 ; FIQ Mode = 10001
msr  cpsr, r1
msr  spsr, r1
mov  lr,  r0
mov  r8,  #0x0000 ; Registers R8 to R12 are also
banked in FIQ mode
mov  r9,  #0x0000
mov  r10, #0x0000
mov  r11, #0x0000
mov  r12, #0x0000
mov  r1, #0x13  ; SVC Mode = 10011
msr  cpsr, r1
msr  spsr, r1
mov  lr,  r0
mov  r1, #0x17  ; ABT Mode = 10111
msr  cpsr, r13
msr  spsr, r13
mov  lr,  r0
mov  r1, #0x12  ; IRQ Mode = 10010
msr  cpsr, r13
msr  spsr, r13
mov  lr,  r0
mov  r1, #0x1B  ; UDEF Mode = 11011
msr  cpsr, r13
msr  spsr, r13
mov  lr,  r0
mov  r1, #0xDF  ; System Mode = 11011111
msr  cpsr, r13
msr  spsr, r13
; Floating Point Co-Processor Initialization. FPU needs to be enabled f irst.
mrc  p15,  #0x00, r2,  c1, c0, #0x02
orr  r2,  r2, #0xF00000
mcr  p15,  #0x00, r2,  c1, c0, #0x02
mov  r2,  #0x40000000
fmxr  fpexc,  r2
fmdrr d0,  r1,  r1
fmdrr d1,  r1,  r1
fmdrr d2,  r1, r1
fmdrr d3,  r1,  r1
fmdrr d4,  r1,  r1
fmdrr d5,  r1,  r1
fmdrr d6,  r1,  r1
fmdrr d7,  r1,  r1
fmdrr d8,  r1,  r1
fmdrr d9,  r1,  r1
fmdrr d10, r1,  r1
fmdrr d11, r1,  r1
fmdrr d12, r1,  r1
fmdrr d13, r1,  r1
fmdrr d14, r1,  r1
fmdrr d15, r1,  r1
bl  $+4
bl  $+4
bl  $+4
bl  $+4
bx  r0
.endasmfunc
72TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated

<!-- Page 73 -->
73TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.5.5 CPU Self-Test
The CPU STC (Self-Test Controller) isused totest the two Cortex-R5F CPU Cores using the
Deterministic Logic BIST Controller asthetestengine.
The main features oftheself-test controller are:
*Ability todivide thecomplete testrunintoindependent testintervals
*Capable ofrunning thecomplete testaswell asrunning fewintervals atatime
*Ability tocontinue from thelast executed interval (test set) aswell asability torestart from the
beginning (First testset)
*Complete isolation oftheself-tested CPU core from restofthesystem during theself-test run
*Ability tocapture theFailure interval number
*Time-out counter fortheCPU self-test runasafail-safe feature
6.5.5.1 Application Sequence forCPU Self-Test
1.Configure clock domain frequencies.
2.Select number oftestintervals toberun.
3.Configure thetime-out period fortheself-test run.
4.Enable self-test.
5.Wait forCPU reset.
6.Inthereset handler, read CPU self-test status toidentify anyfailures.
7.Retrieve CPU state ifrequired.
Formore information seethedevice technical reference manual.
6.5.5.2 CPU Self-Test Clock Configuration
The maximum clock rate fortheself-test is110 MHz. The STCCLK isdivided down from theCPU clock.
This divider isconfigured bytheSTCCLKDIV register ataddress 0xFFFFE644.
Formore information seethedevice-specific Technical Reference Manual.
6.5.5.3 CPU Self-Test Coverage
The self-test, ifenabled, isautomatically applied totheentire processor group. Self-test willonly start
when nCLKSTOPPEDm isasserted which indicates theCPU cores and theACP interface are in
quiescent state. While theprocessor group isinself-test, other masters can still function normally
including accesses tothesystem memory such astheL2SRAM. Because uSCU ispart oftheprocessor
group under self-test, thecache coherency checking willbebypassed.
When theself-test iscompleted, reset isasserted toalllogic subjected toself-test. After self-test is
complete, software must invalidate thecache accordingly.
The default value oftheCPU LBIST clock prescaler is'divide-by-1 '.Aprescalar intheSTC module canbe
used toconfigure theCPU LBIST frequency with respect totheCPU GCLK frequency.
Table 6-9lists theCPU testcoverage achieved foreach self-test interval. Italso lists thecumulative test
cycles. The testtime canbecalculated bymultiplying thenumber oftestcycles with theSTC clock period.

<!-- Page 74 -->
74TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-9.CPU Self-Test Coverage
INTERVALS TEST COVERAGE, % TEST CYCLES
0 0 0
1 56.85 1629
2 64.19 3258
3 68.76 4887
4 71.99 6516
5 75 8145
6 76.61 9774
7 78.08 11403
8 79.2 13032
9 80.18 14661
10 81.03 16290
11 81.9 17919
12 82.58 19548
13 83.24 21177
14 83.73 22806
15 84.15 24435
16 84.52 26064
17 84.9 27693
18 85.26 29322
19 85.68 30951
20 86.05 32580
21 86.4 34209
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
33 89.1 53757
34 89.23 55386
35 89.41 57015
36 89.55 58644
37 89.7 60273
38 89.83 61902
39 89.96 63531
40 90.1 65160

<!-- Page 75 -->
75TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.5.6 N2HET STC /LBIST Self-Test Coverage
Logic BIST self-test capability forN2HETs isavailable inthis device. The STC2 can beconfigured to
perform self-test forboth N2HETs atthesame time orone atthetime. The default value oftheN2HET
LBIST clock prescaler isdivide-by-1. However, themaximum clock rate fortheN2HET STC /LBIST is
VCLK/2. N2HET STC testshould notbeexecuted concurrently with CPU STC test.
Table 6-10. N2HET Self-Test Coverage
INTERVALS TEST COVERAGE, % TEST CYCLES
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