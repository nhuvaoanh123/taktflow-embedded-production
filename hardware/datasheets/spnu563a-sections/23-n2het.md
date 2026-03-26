# High-End Timer (N2HET) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 953-1130

---


<!-- Page 953 -->

953 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleChapter 23
SPNU563A -March 2018
High-End Timer (N2HET) Module
This chapter provides ageneral description oftheHigh-End Timer (N2HET). The N2HET isasoftware-
controlled timer with adedicated specialized timer micromachine andasetof30instructions. The N2HET
micromachine isconnected toaport ofupto32input/output (I/O) pins.
NOTE: This chapter describes asuperset implementation oftheN2HET module thatincludes
features andfunctionality thatrequire DMA. Since notalldevices have DMA capability,
consult your device-specific datasheet todetermine theapplicability ofthese features and
functions toyour device being used.
Topic ........................................................................................................................... Page
23.1 Overview ......................................................................................................... 954
23.2 N2HET Functional Description ........................................................................... 958
23.3 Angle Functions ............................................................................................... 990
23.4 N2HET Control Registers ................................................................................. 1017
23.5 HWAG Registers ............................................................................................. 1044
23.6 Instruction Set................................................................................................ 1060

<!-- Page 954 -->

Overview www.ti.com
954 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.1 Overview
The N2HET isafifth-generation Texas Instruments (TI)advanced intelligent timer module. Itprovides an
enhanced feature setcompared toprevious generations.
This timer module provides sophisticated timing functions forreal-time applications such asengine
management ormotor control. The high resolution hardware channels allow greater accuracy forwidely
used timing functions such asperiod andpulse measurements, output compare, andPWMs.
The reduced instruction set,based mostly onvery simple, butcomprehensive instructions, improves the
definition anddevelopment cycle time ofanapplication anditsderivatives. The N2HET breakpoint feature,
combined with various stop capabilities, makes theN2HET software application easy todebug.
23.1.1 Features
*Programmable timer forinput andoutput timing functions
*Reduced instruction set(30instructions) fordedicated time andangle functions
*Uptomaximum of12896-bit words ofinstruction RAM protected byparity. Check your datasheet for
theactual number ofwords implemented.
*User defined configuration of25-bit virtual counters fortimer, event counters andangle counters
*7-bit hardware counters foreach pinallow upto32-bit resolution inconjunction with the25-bit virtual
counters
*Upto32pins usable forinput signal measurements oroutput signal generation
*Programmable suppression filter foreach input pinwith adjustable suppression window
*Low CPU overhead andinterrupt load
*Efficient data transfer toorfrom theCPU memory with dedicated High-End-Timer Transfer Unit (HTU)
orDMA
*Diagnostic capabilities with different loopback mechanisms andpinstatus readback functionality
*Hardware Angle Generator (HWAG)
23.1.2 Major Advantages
Inaddition toclassic time functions such asinput capture ormultiple PWMs, higher-level time functions
canbeeasily implemented inthetimer program main loop. Higher-level time functions include angle
driven wave forms, angle- andtime-driven pulses, andinput pulse width modulation (PWM) duty cycle
measurement.
Because ofthese high-level functions, data exchanges with theCPU arelimited tothefundamental
parameters oftheapplication (periods, pulse widths, angle values, etc.); andthereal-time constraints for
parameter communication aredramatically minimized; forexample, fewinterrupts arerequired and
asynchronous parameter updates areallowed.
The reduced instruction setandsimple execution flow control make itsimple andeasy todevelop and
modify programs. Simple algorithms canembed theentire flow control inside theN2HET program itself.
More complex algorithms cantake advantage oftheCPU access totheN2HET RAM. With this, theCPU
program canmake calculations andcanmodify thetimer program flow bychanging thedata andcontrol
fields oftheN2HET RAM. CPU access totheN2HET RAM also improves thedebug anddevelopment of
timer programs. The CPU program canstop theN2HET andview thecontents oftheprogram, control,
anddata fields thatreside intheN2HET RAM.
Finally, themodular structure provides maximum flexibility toaddress awide range ofapplications. The
timer resolution canbeselected from twocascaded prescalers toadjust theloop resolution andHR
clocks. The 32I/Opins canprovide anycombination ofinput, period orpulse capture, andoutput
compare, including high resolution foreach channel.

<!-- Page 955 -->

Address DecodePeripheral bus
Register AOFF
ONProgram RAMinternal multi-
N2HET syncSlave
Master
HETGCR.16
HETGCR.0
32 ALUCompare
T o VIM
HETOFF2.7:0HETOFF1.7:0
HETDOUT.31:0HETDSET.31:0Register RRegister B
HETDCLR.31:0HETFLG.31:0
HETDIN.31:0Priority 1
Priority 2T o VIM
32
HETDIR.31:0Rotate/
Shift by NHETADDR.8:0
HETPRY.31:0
HR blockHET[31:0]Control RAM Data RAM
Ignore Suspend
HETGCR.17StopShadow Registers Shadow Registers High Resolution
prescaler
HETPFR.5:0
Loop resolution
prescaler
HETPFR.10:8
Register S
Register THR clock
(to IO PIN
CONTROL)
HR clockCURRENT INSTRUCTIONHOST
INTERFACE
N2HET
RAM
SPECIALIZED
TIMER
MICROMACHINE
I/O
PIN
CONTROLPROGRAM FIELD CONTROL FIELD DATA FIELD
www.ti.com Overview
955 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.1.3 Block Diagram
The N2HET module (see Figure 23-1)comprises four separate components:
*Host interface
*N2HET RAM
*Specialized timer micromachine
*I/Ocontrol (the N2HET isattached toanI/Oport ofupto32pins)
Figure 23-1. N2HET Block Diagram

<!-- Page 956 -->

Overview www.ti.com
956 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.1.4 Timer Module Structure andExecution
The timer consists ofaspecialized micromachine thatoperates areduced instruction set.Two 25-bit
registers andthree 32-bit registers areavailable tomanipulate information such astime, event counts, and
angle values. System performance isimproved byawide instruction format (96bits) thatallows the
N2HET tofetch theinstructional operation code anddata inonesystem cycle, thus increasing thespeed
atwhich data canbeprocessed. The typical operations performed intheALU areadditions (count),
compares, andmagnitude compares (higher orsame).
Each instruction ismade upofa32-bit program field, a32-bit control field anda32-bit data field. The
N2HET execution unitfetches thecomplete 96-bit instruction inonecycle andexecutes it.Allinstructions
include a9-bit field forspecifying theaddress ofthenext instruction tobeexecuted. Some instructions
also include a9-bit conditional address, which isused asthenext address whenever aparticular condition
istrue. This makes controlling theflow ofanN2HET program inexpensive; inmany cases aseparate
branch instruction isnotrequired.
The interface tothehost CPU isbased onboth communication memory andcontrol registers. The
communication memory includes timer instructions (program anddata). This memory istypically initialized
bytheCPU orDMA after reset before thetimer starts execution. Once thetimer program isloaded into
thememory, theCPU starts thetimer execution, andtypically data parameters arethen read orwritten
intothetimer memory. The control registers include bitsforselecting timer clock, configuring I/Opins, and
controlling thetimer module.
The programmer implements timer functions bycombining instructions inspecific sequences. For
instance, asingle count (CNT) instruction implements atimer. Asimple PWM generator canbe
implemented with atwoinstruction sequence: CNT andcompare (ECMP orMCMP). Acomplex time
function may include many instructions inthesequence. The total timer program isasetofinstructions
executed sequentially, oneafter theother. Reaching theend, theprogram must rolltothefirstinstruction
sothatitbehaves asaloop. The time foraloop toexecute isreferred toasaloop resolution clock cycle
orloop resolution period (LRP). When theN2HET rolls over tothefirstinstruction, thetimer waits forthe
loop resolution clock torestart theexecution oftheloop toensure thatonly oneloop isexecuted foreach
loop resolution clock.
The longest path through anN2HET program must becompleted within theloop resolution clock (LRP).
Otherwise, theprogram willexecute unpredictably because some instructions willnotbeexecuted each
time through theloop. This effect creates astrong linkbetween theaccuracy ofthetimer functions andthe
number offunctions (the number ofinstructions) thetimer canperform. High resolution (HR) hardware
timer extensions areavailable foreach oftheN2HET pins tohelp overcome thislimitation.
The high resolution hardware timers operate from thehigh resolution clock ,which may beconfigured for
frequency multiples between 2and128times theloop resolution clock frequency. This extending the
resolution oftimer events andmeasurements well beyond what ispossible with only loop resolution
instructions.
Most ofthecommonly used N2HET instructions canoperate either atloop resolution orhigh resolution;
with therestriction thatforeach pinatmost onehigh resolution instruction canbeexecuted perloop
resolution period.
Certain instructions (MOV32, ADM32, ...)canmodify thedata fields ofother instructions. This feature
enables theN2HET program toimplement double buffering oncapture andcompare functions. For
example, anECMP compare instruction canbefollowed byaMOV32 instruction thatisconditionally
executed when theECMP instruction matches. The host CPU canupdate thenext compare value by
writing asynchronously tothedata field oftheMOV32 instruction instead ofwriting directly tothedata field
oftheECMP instruction. The copy from thebuffer (MOV32 data field) tothecompare register (ECMP data
field) willoccur when theMOV32 instruction isactually executed which occurs after theECMP instruction
matches itscurrent compare value. This isthesame behavior asonewould expect from adouble buffered
hardware compare register.

<!-- Page 957 -->

www.ti.com Overview
957 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleOther instructions (MOV64, RADM64) canmodify both thecontrol anddata fields ofother instructions.
This allows theN2HET toimplement toggle functionality. Forexample, anECMP instruction canbe
followed byapairofMOV64 instructions. The MOV64 instruction updates thedata field oftheECMP
instruction toimplement thedouble buffering behavior. Butitalso updates thecontrol field oftheECMP
instruction which allows ittochange things likepinaction andtheconditional address. IfoneMOV64
instruction configures theECMP pinaction toSET while thesecond changes ittoCLEAR, andthetwo
MOV64 instructions update theconditional address topoint toeach other, then asingle ECMP instruction
canbeused totoggle apineach time thecompare match occurs.
23.1.5 Performance
Most instructions execute inonecycle, butafewtake twoorthree cycles.
The N2HET cangenerate many complex output waveforms without CPU interrupts. Where special
algorithms areneeded following aspecific event (forexample, missing teeth orashort/long input signal), a
minimal number ofinterrupts totheCPU areneeded freeing theCPU forother tasks.
23.1.6 N2HET Compared toNHET
N2HET enhancements from NHET include:
*Eight new instructions: ADD, ADC, SUB, SBB, AND, OR, XOR, RCNT
*FullsetofALU flags Carry (C),Negative (N),Zero (Z),Overflow (V)
*Branch instruction (BR) extended tosupport signed andunsigned arithmetic comparison conditions
*Two additional 32-bit temporary working registers R,S.
*New HETAND register forAND-Sharing ofHigh Resolution structure between pairs ofpins
*Improved high resolution PCNT instruction
23.1.7 NHET andN2HET Compared toHET
Compared totheHET module, theN2HET contains alloftheenhancements described inSection 23.1.6
plus thefollowing additional enhancements:
*New Interrupt Enable SetandClear registers
*Capability togenerate requests totheDMA module ortheHET Transfer Unit (HTU) including new
Request Enable SetandClear registers
*N2HET RAM parity error detection
*Suppression filters foreach ofthe32I/Ochannel andcontrol register toconfigure thelimiting
frequency andcounter clock
*Enhanced edge detection hardware thatdoes notrelyontheprevious bitfield inthecontrol word of
theN2HET instruction.
*The next, conditional andremote addresses areextended from 8to9bits
*The loop resolution data fields areextended from 20to25bits
*The high resolution data fields areextended from 5to7bits
*Instructions with anadequate condition areable tospecify thenumber oftherequest line, which
triggers either theHET Transfer Unit (HTU) ortheDMA module
*The CNT instruction provides abit,which allows toconfigure either anequal comparison oragreater
orequal comparison when comparing theselected register value with theMax-value
*The MOV32 instruction provides anew bit.IfsettoonetheMOV32 willonly perform themove, when
theZ-flag isset.Ifsettozero theMOV32 willperform themove whenever itisexecuted (independent
onthestate oftheZ-flag)
*There isanew instruction WCAPE, which isacombination ofatime stamp andanedge counter
*New Open Drain, PullDisable, andPullSelect registers

<!-- Page 958 -->

Overview www.ti.com
958 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.1.8 Instructions Features
The N2HET hasthefollowing instructions features:
*N2HET uses aRISC-based specialized timer micromachine tocarry outasetof30instructions
*Instructions areimplemented inaVery Long Instruction Word (VLIW) format (96-bits wide)
*The N2HET program execution isself-driven byexternal orinternal events, branching tospecial
routines based oninput edges oroutput compares
*Instructions point tothenext instruction executed, eliminating theneed foraprogram counter
*Several instructions canchange theprogram flow based oninternal orexternal conditions
23.1.9 Program Usage
The N2HET instructions/program canbeassembled with theN2HET assembler. The assembler generates
aC-structure which canbeincluded intothemain application program. The application hastocopy the
content ofthestructure intotheN2HET RAM, setupnecessary registers andstart theN2HET program
execution. Inaddition totheC-structure, theassembler generates also aheader filewhich makes iteasy
forthemain application toaccess thedifferent instructions andchange forexample theduty cycle ofa
PWM orread outthecaptured value ofaspecific signal edge.
23.2 N2HET Functional Description
The N2HET contains RAM intowhich N2HET code isloaded. The N2HET code isrunbythespecialized
timer micromachine. The host interface andI/Ocontrol provide aninterface totheCPU andexternal pins
respectively.
23.2.1 Specialized Timer Micromachine
The N2HET hasitsown instruction set,detailed inSection 23.6.1 .The timer micromachine reads each
instruction from theN2HET RAM. The program andcontrol fields contain theinstructions forhow the
specialized timer micromachine executes thecommand. Formost instructions, thedata field stores the
information thatneeds tobemanipulated.
The specialized timer micromachine executes theinstructions stored intheN2HET RAM sequentially. The
N2HET program execution isself-driven byexternal orinternal events. This means thatinput edges or
output compares may force theprogram tobranch tospecial routines using aconditional address.
Figure 23-2 shows some ofthemajor operations thattheN2HET cancarry out,namely compares,
captures, angle functions, additions, andshifts. The N2HET contains fiveregisters (A,B,R,S,andT)
used tohold compare orcounter values andareused bytheN2HET instructions. Data may betaken from
theregisters orthedata field formanipulation; likewise, thedata may bereturned totheregisters orthe
data field.
23.2.1.1 Time Slots andResolution Loop
Each instruction requires aspecific number ofcycles ortime slots toexecute. The resolution specified in
theprescaler bitfields determines thetimer accuracy. Allinput captures, event counts, andoutput
compares areexecuted once ineach resolution loop. HRcaptures andcompares arepossible (upto
N2HET clock accuracy) ontheHRI/Opins. Formore information about theHRI/Ostructure, see
Section 23.2.5 .

<!-- Page 959 -->

Cont.
Register AOff
On
HETGCR.17Stop
HETGCR.0
32 Bit ALUCompare
T o VIMHETOFF2.7:0HETOFF1.7:0Register RRegister B
HETFLG.31:0
Specialized timer micromachinePriority 1
Priority 2T o VIM
Rotate/
Shift By NHETADDR.8:0
HETPRY .31:0From N2HET RAM
To I/O ControlRegister TRegister SCURRENT INSTRUCTION
PROGRAM FIELD CONTROL FIELD DATA FIELD
www.ti.com N2HET Functional Description
959 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-2. Specialized Timer Micromachine

<!-- Page 960 -->

Program loop
Next program address=00h1 2 3 4 N 1 2 3 4Time slotLoop Resolution Period = LRP
VCLK2
Loop Res.
clock
InstructionsHigh Res.
clock...
...
N2HET Functional Description www.ti.com
960 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.1.2 Program Loop Time
The program loop time isthesum ofallcycles used forinstruction execution. This time may vary from one
loop toanother iftheN2HET program includes conditionally executed instructions.
The timer program restarts onevery resolution loop. The start address isfixed atN2HET RAM address
00h. The longest path through aprogram must fitwithin oneloop resolution period toguarantee complete
accuracy.
The lastinstruction ofaprogram must branch back tothefixed start address (next program address =
00h). When anN2HET program branches back toaddress 00hbefore theendofaloop resolution period,
theN2HET detects thisandpauses instruction execution until thebeginning ofthenext loop resolution
period.
The timing diagram inFigure 23-3 illustrates theprogram flow execution.
Figure 23-3. Program Flow Timings
23.2.1.3 Instruction Execution Sequence
The execution ofaN2HET program begins with thefirstoccurrence oftheloop resolution clock, after the
N2HET isturned on.Atthefirstandsubsequent occurrences oftheloop resolution, theinstruction at
location address 00hisprefetched. The program execution begins attheoccurrence oftheloop resolution
clock andcontinues executing theinstructions until theprogram branches to00hlocation. The instruction
isprefetched atlocation 00handexecution flagisreset. The N2HET pauses instruction execution until the
occurrence oftheloop resolution clock andresumes normal execution.
N2HET programs must bewritten sothatthey complete execution andreturn toaddress 00hbefore the
occurrence ofthenext loop resolution clock. IftheN2HET program exceeds thisexecution time limit, then
aprogram overflow condition occurs asdescribed inSection 23.2.1.4 .
23.2.1.4 Program Overflow Condition
Ifthenumber oftime slots used inaprogram loop exceeds thenumber available time slots inoneloop
resolution, thetimer sets theprogram overflow interrupt flaglocated intheHETEXC2 register. Tomaintain
synchronization oftheI/Os, thiscondition should never beallowed tooccur inanormal operation. The
HETEXC2.PRGMOVRFLFLAG flagprovides amechanism forchecking thatthecondition does notoccur
during thedebug andvalidation phases.
AsFigure 23-4 illustrates, when aprogram overflow occurs, thecurrently executing N2HET program
sequence isinterrupted andrestarted atN2HETaddress 0forthebeginning ofthenext loop resolution
clock period. Also, HETEXC2.PRGMOVRFLFLAG isset.
Iftheinstruction thatcaused theoverflow (instruction ataddress 0xC inFigure 23-4 )hasanypinactions
selected, these pinactions willnotbeperformed. However other actions oftheinstruction including
register andRAM updates willstillbeperformed.

<!-- Page 961 -->

Loop Resolution clock
N2HET Program
Program Overflow
(HETEXC2Address012378AB 0 123789A 012 CB
No Overflow.
Program returns to address 0
before start of next loopOverflow.
Program did not return
to address 0 before start
of next loop..PRGMOVRFLFLAG)
www.ti.com N2HET Functional Description
961 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-4. UseoftheOverflow Interrupt Flag (HETEXC2)
23.2.1.5 Architectural Restrictions onN2HET Programs
Certain architectural restrictions apply toN2HET programs:
1.The size ofanN2HET program must begreater than oneinstruction.
2.Anextra wait state isincurred byanyinstruction thatmodifies afield inthenext instruction tobe
executed.
3.Only oneinstruction (using high resolution) isallowed perhigh resolution pin.
4.Consecutive break points arenotsupported. Instructions with break points must have atleast a
distance oftwoinstructions (forexample, ataddresses 1,3,5,7,andsoon,assuming theprogram
executes linearly)
NOTE: While itwould beunusual tocode anN2HET program thatisonly oneinstruction long, itis
trivial tomodify such aprogram tomeet therequirement ofrestriction 1.Simply adda
second instruction totheprogram, which may beasimple branch tozero.
Toenforce restriction 3,thehigh resolution pinstructures respond only tothefirstinstruction
thatisexecuted matching their pinnumber with hr_lr=HIGH, regardless ofwhether ornotthe
en_pin_action field isON. Subsequent instructions areignored bythehigh resolution pin
structure fortheremainder oftheloop resolution period.
23.2.1.6 Multi-Resolution Scheme
The N2HET hasthecapability tovirtually extend thecounter width byexecuting instructions only once
every Nloop resolution periods. This decreases thetimer resolution, butextends thecounter range which
may beuseful when generating ormeasuring slow signals. Figure 23-5 illustrates how amulti-resolution
scheme may beimplemented inanN2HET program. Anunconditional Branch instruction andanindex
sequence, using aMOV64 instruction ineach lowresolution loop, isrequired tocontrol thisparticular
program flow.
NOTE: HRinstructions must beplaced inthemain (fullresolution) loop toensure proper operation.

<!-- Page 962 -->

Instructions with
full resolution
(2 Ps)
Branch on
0 1 2
Instructions with
lower resolution
(6 Ps)Instructions with
lower resolution
(6 Ps)Instructions with
lower resolution
(6 Ps)
Change conditionalconditional address
addressChange conditional
addressChange conditional
address
N2HET Functional Description www.ti.com
962 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-5. Multi-Resolution Operation Flow Example
23.2.1.7 Debug Capability
The N2HET supports breakpoints toallow youtomore easily debug your N2HET program. Figure 23-6
provides anillustration ofthebreakpoint mechanism.
The steps toenable anN2HET breakpoint are:
1.Make sure thedevice nTRST pinishigh, since N2HET breakpoints aredisabled whenever thispinis
low. (Normally thisishandled automatically when aJTAG debugger isattached).
2.Attach aJTAG debugger andconnect tothedevice thathasbeen already programmed with the
N2HET code thatneeds todebugged. (downloading toon-chip flash isoutside thescope ofthis
section).
3.Execute theCPU program atleast until thepoint where theN2HET program RAM hasbeen initialized
bytheCPU.
4.Open amemory window intheN2HET registers.
5.Make sure HETEXC2.DEBUGSTATUSFLAG bitiscleared.
6.Open amemory window ontheN2HET RAM
7.Setbit22intheprogram field oftheinstruction(s) onwhich youwish tobreak. Note thatthisinstruction
willbeexecuted before theN2HET ishalted -slightly different from how CPU breakpoints behave.
8.Make sure theCPU andN2HET arerunning, ifthey arehalted then restart theCPU through theJTAG
emulator (N2HET willstart when theCPU starts).
9.Both theCPU andN2HET willhaltwhen breakpoint isreached.
When theN2HET ishalted, itsstate machines arefrozen butalloftheN2HET control registers canbe
accessed through theJTAG emulator interface.
The current N2HET instruction address canbeinspected byreading theHETADDR register; thisshould
bepointing totheinstruction thatcaused thebreakpoint.

<!-- Page 963 -->

Device test
mode enableDebug request to CPUDebug
mode
control
Debug
status
bitN2HET RAMBreakpoint bit (P22)
(nTRST)
nTRST signal = 0: Functional mode
nTRST signal = 1: Test/Debug modeDebug ack from CPU
www.ti.com N2HET Functional Description
963 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe N2HET internal working registers (A,B,R,S,T) arenotdirectly visible through theJTAG emulator
interface. Ifthecontent ofthese registers needs tobeinspected, itisbest toaddaninstruction like
MOV32 which copies theregister value totheN2HET RAM. This RAM location canbeinspected when the
N2HET halts.
Torestart execution ofboth theCPU andtheN2HET from thehalted state:
1.Clear HETEXC2.DEBUGSTATUSFLAG bit.
2.Clear bit22intheprogram field oftheinstruction onwhich thebreakpoint was reached.
3.Restart theCPU through thenormal JTAG emulator procedure ('Run'or'Go').The N2HET will
automatically start executing when itsees thattheCPU hasexited thedebug state.
Figure 23-6. Debug Control Configuration
NOTE: Consecutive break points arenotsupported. Instructions with break points must have at
least adistance oftwoinstructions (forexample, atN2HET addresses 1,3,5,7,andsoon)
23.2.2 N2HET RAM Organization
The N2HET RAM isorganized intotwosections. The firstcontains theN2HET program itself. The second
contains parity protection bitsfortheN2HET program.
Each N2HET instruction is96-bits wide butaligned toa128-bit boundary. Instructions consist ofthree 32-
bitfields: Program, Control, andData. Instructions areseparated byafourth unimplemented address to
force alignment to128-bit boundaries.
The integrity oftheN2HET program canbeprotected byParity. Parity protection isenabled through the
N2HET Parity Control Register (HETPCR).
Table 23-1 shows thebase addresses forN2HET RAM andN2HET Parity RAM.
Table 23-1. N2HET RAM Base Addresses
N2HET1 Base Address N2HET2 Base Address Memory
0xFF46_0000 0xFF44_0000 N2HET Instruction RAM (Program/Control/Data)
0xFF46_2000 0xFF44_2000 N2HET Parity RAM

<!-- Page 964 -->

N2HET Functional Description www.ti.com
964 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.2.1 N2HET RAM Banking
Because theCPU must make updates totheN2HET RAM while theN2HET isexecuting, forexample to
update theduty cycle value ofaPWM, itisimportant tounderstand how theN2HET RAM organization
facilitates simultaneous accesses byboth theHOST CPU andtheN2HET.
The N2HET RAM isimplemented as4banks of96-bit wide twoport RAM. This means thatthere atotal of
8ports available; four read andfour write. Normally theN2HET willuseuptotwoofthese ports atatime.
One read port isused toallow theN2HET toprefetch thenext N2HET instruction while awrite port may
beused toupdate thedata orcontrol fields thathave changed asaresult ofexecuting thecurrent
instruction.
N2HET accesses toitsown internal RAM aregiven priority over accesses from anexternal host (CPU or
DMA), thismakes N2HET program execution deterministic which isacritical requirement foratimer.
Most N2HET instructions execute inasingle cycle. Cases where await state impacts theN2HET program
execution time are:
*The current N2HET instruction writes data back tothenext N2HET intheexecution sequence.
*The external host reads from anN2HET instruction where theautomatic read-clear option isset,while
theN2HET isexecuting from/on thesame address (See Section 23.2.4.3 ).
Except forthecase ofautomatic read-clear, theexternal host isstalled when thehost andN2HET have a
bank conflict. However thiswilltypically only result inastall ofonecycle, duetotheN2HET bank ordering
which isorganized ontheN2HET Address least significant bitboundaries (See Table 23-2 ).
Assuming most oftheN2HET program executes linearly through theN2HET Address space; ifabank
conflict does exist itisusually resolved inthenext cycle astheN2HET program moves tothenext bank.
N2HET programmers should avoid writing aprogram thataccesses thesame bank ofN2HET RAM on
every cycle, asthiscould lock theexternal host outoftheN2HET memory completely.
Table 23-2 describes theN2HET memory map, asviewed bytheN2HET aswell asfrom thememory
space ofthehost CPU andDMA.
Table 23-2. N2HET RAM Bank Structure
N2HET Address Host CPU orDMA Address Space
InstructionProgram Field
AddressControl Field
AddressData Field
AddressReserved
AddressN2HET RAM
Bank
000h XX0000h XX0004h XX0008h XX000Ch A
001h XX0010h XX0014h XX0018h XX001Ch B
002h XX0020h XX0024h XX0028h XX002Ch C
003h XX0030h XX0034h XX0038h XX003Ch D
004h XX0040h XX0044h XX0048h XX004Ch A
: : : : : :
03Fh XX03F0h XX03F4h XX03F8h XX03FCh D
040h XX0400h XX0404h XX0408h XX040Ch A
: : : : : :
1FFh XX1FF0h XX1FF4h XX1FF8h XX1FFCh D
NOTE: The external host interface supports anyaccess size forreads, butonly 32-bit writes tothe
N2HET RAM aresupported. Reserved addresses should notbeaccessed, theresult of
doing soisindeterminate.

<!-- Page 965 -->

www.ti.com N2HET Functional Description
965 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.2.2 Parity Checking
The N2HET module candetect parity errors inN2HET RAM. Asdescribed inSection 23.2.2 theN2HET
allows 32-bit writes only. Therefore N2HET RAM parity checking isimplemented using oneparity bitper
32-bit field inN2HET RAM.
Even oroddparity selection forN2HET parity detection canbeconfigured inthesystem module. Parity
calculation andchecking canbeenabled/disabled bya4-bit keyinHETPCR.
During aread access totheN2HET RAM, theparity iscalculated based onthedata read from theRAM
andcompared with thegood parity value stored intheparity bits. The parity check isperformed when the
N2HET execution unitmakes aread access toN2HET RAM, butalso when adifferent master (for
example, CPU, HTU, DMA) performs theread access. Ifany32-bit-word fails theparity check then an
error issignaled totheESM module. The N2HET address, which generated theerror isdetected andis
captured inHETPAR forhost system debugging. The address isfrozen from being updated until itisread
bythebusmaster.
The N2HET execution unitreads theinstructions, which are96-bit wide. They contain theprogram-,
control- anddata-field whereby each is32-bit wide. Sowhen fetching N2HET instructions parity checking
isperformed onthree words inparallel.
Ifaparity error isdetected intwoormore words inthesame cycle then only oneaddress (word atthe
lower address) iscaptured. The captured N2HET address isalways aligned toa32-bit word boundary.
During debug, parity checking isstillperformed onaccesses originating from theon-chip host CPU and
DMA. However, parity errors thataredetected during anaccess initiated bythedebugger itself are
ignored.
23.2.2.3 Parity Error Detection Actions
Detection ofaN2HET parity error causes thefollowing actions:
1.Anerror issignaled totheESM module.
2.The Parity Address Register (HETPAR) isloaded with theaddress ofthefaulty N2HET field.
3.N2HET execution immediately stops. (The instruction thattriggered theparity error isnotexecuted.)
4.The Turn-On/Off-Bit intheN2HET Global Configuration Register (HETGCR) isautomatically cleared.
5.AllN2HET internal flags arecleared.
6.AllN2HET pins selected byN2HET Parity PinRegister (HETPPR) enter apredefined safe state.
7.Register HETDOUT isalso updated toreflect changes inpinstate duetoHETPPR.
The safe state forN2HET pins selected through theHETPPR register depends onhow thepinis
configured intheHETDIR, HETPDR, andHETPSL registers. Table 23-3 explains how thesafe state is
determined.
Table 23-3. PinSafe State Upon Parity Error Detection
Safe State HETDIR HETPDR HETPSL
Drive Low 1 0 0
Drive High 1 0 1
High Impedance 1 1 x

<!-- Page 966 -->

N2HET Functional Description www.ti.com
966 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.2.4 Testing Parity Detection Logic
Totesttheparity detection logic, theparity RAM hastobemade accessible totheCPU inorder toallow a
diagnostic program toinsert parity errors. The control register bitHETPCR.TEST must besetinorder to
make theparity RAM accessible. Once HETPCR.TEST isset,theparity bitsareaccessible asdescribed
inTable 23-4.
Each 32-bit N2HET field hasitsown parity bitintheN2HET Parity RAM asshown inTable 23-4 .There
arenoparity bitsforthereserved fields, since there isnophysical N2HET RAM forthese fields.
Table 23-4. N2HET Parity BitMapping
Address
N2HET1Address
N2HET2Bits
[31:1] [0]
0xFF46_2000 0xFF44_2000 Reads 0,Writes have noeffect Instruction 0Program Field Parity Bit
0xFF46_2004 0xFF44_2004 Reads 0,Writes have noeffect Instruction 0Control Field Parity Bit
0xFF46_2008 0xFF44_2008 Reads 0,Writes have noeffect Instruction 0Data Field Parity Bit
0xFF46_200C 0xFF44_200C Reads 0,Writes have noeffect Read 0
0xFF46_2010 0xFF44_2010 Reads 0,Writes have noeffect Instruction 1Program Field Parity Bit
.... .... ... ...
23.2.2.5 Initialization ofParity RAM
After device power up,theN2HET RAM contents including theparity bitscannot beguaranteed. Inorder
toavoid false parity failures duetotherandom state inwhich RAM powers up,theRAM hastobe
initialized.
Before initializing theN2HET RAM, enable theN2HET parity logic bywriting toHETPCR. Then the
N2HET Instruction RAM should beinitialized. With parity enabled, theN2HET parity RAM willbeinitialized
automatically byN2HET atthesame time thattheN2HET instruction RAM isinitialized bytheCPU. Note
thatloading theN2HET program with parity enabled isalso effective.
Another possibility toinitialize theN2HET memory anditsparity bitsis,tousethesystem module tostart
theautomatic initialization ofallRAMs onthemicrocontroller. The RAMs willbeinitialized to'0'.
Depending ontheeven/odd parity selection, theparity bitwillbecalculated accordingly.
23.2.3 Time Base
AllN2HET timings arederived from VCLK2 (see Figure 23-7).Internally N2HET instructions execute at
theVCLK2 rate; butthetimer loop clock andthehigh-resolution hardware timer clock canbescaled down
from VCLK2. Two prescalers areavailable toadjust thetimer loop resolution clock fortheprogram loop,
andthehigh resolution (HR) clock fortheHRI/Ocounters.
*Time Slots: The number ofcycles available forinstruction execution perloop. Time Slots isthe
number ofVCLK2 cycles inaLoop Resolution Clock.
*High Resolution Clock: The high resolution clock isthesmallest time increment with which apincan
change it'sstate orcanbemeasured inthecase ofinput signals. A6-bit prescaler dividing VCLK2 by
auser-defined HRprescale divide rate (hr)stored inthe6-bit HRprescale factor code (HETPFR). See
Table 23-5.
*Loop Resolution Clock: The loop resolution clock defines thetimebase forexecuting allinstructions
inaN2HET program. Since instructions canbeconditionally executed, thelongest path through the
N2HET program must fitintooneloop resolution clock period (LRP).A 3-bit prescaler dividing theHR
clock byauser-defined loop-resolution prescale divide rate (lr)stored inthe3-bit loop-resolution
prescale factor code (HETPFR). See Table 23-5 .

<!-- Page 967 -->

HR
prescalerLoop resolution
prescaler
(3 bits) (6 bits)VCLK2Loop resolution
clock
HR
clock
www.ti.com N2HET Functional Description
967 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-7. Prescaler Configuration
The following abbreviations andrelations areused inthisdocument:
1.hr:high resolution prescale factor (1,2,3,4,..., 63,64)
2.lr:loop resolution prescale factor (1,2,4,8,16,32,64,128)
3.ts:Time slots (cycles) available forinstruction execution perloop. ts=hrxlr
4.HRP =high resolution clock period HRP =hr×TVCLK2 (ns)
5.LRP =loop resolution clock period LRP =lr×HRP (ns)
The loop resolution period (LRP) must beselected tobelarger than thenumber ofTime slots (VCLK2
cycles) required tocomplete theworst-case execution path through theN2HET program. Otherwise a
program overflow condition may occur (see Section 23.2.1.4 ).Because oftherelationship oftime slots to
thehrandlrprescalers asdescribed initem 3above, increasing either hrorlrincreases thenumber of
time slots available forprogram execution. However, lrwould typically beincreased first, since increasing
hrresults inadecrease intimer resolution since itreduces theclock totheHigh Resolution IOstructures.
The divide rates hrandlrcanbedefined intheHETPFR register. Table 23-5 lists thebitfield encodings
fortheprescale options.
Table 23-5. Prescale Factor Register Encoding
LRPFC -Loop Resolution HRPFC -High Resolution
HETPFR[10:8] Prescale Factor lr HETPFR[5:0] Prescale Factor hr
000 /1 000000 /1
001 /2 000001 /2
010 /4 000010 /3
011 /8 000011 /4
100 /16 : :
101 /32 111101 /62
110 /64 111110 /63
111 /128 111111 /64

<!-- Page 968 -->

hr = 2 HRPhr
VCLK2- - - - - - - - - - - - - - - - - - - -2
32MHz- - - - - - - - - - - - - - - - - - = = =
lr =128 lr x HRP = 128 x 62.5ns = 8 s μ
ts = hr x lr = 2 x 128 = 256
hr = 2, lr = 128 HETPFR[31:0] = 0x0000070162.5ns
N2HET Functional Description www.ti.com
968 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.3.1 Determining Loop Resolution
Asanexample, consider anapplication thatrequires high resolution ofHRP =62.5 ns,andloop resolution
ofLRP =8μs,andneeds atleast 250time slots fortheN2HET application program.
Assuming VCLK2 =32MHz, thefollowing shows which divide-by rates andwhich value inthePrescale
Factor Register (HETPFR) isrequired fortheabove requirements:
(29)
Intheexample above, iftheloop resolution period needs todecrease from 8μsto4μs,then only 128
time slots willbeavailable forprogram execution. The program may need toberestructured assuggested
inSection 23.2.1.6 .
23.2.3.2 The7-Bit HRData Field
The instruction execution examples ofECMP (Section 23.2.5.9 ),MCMP (Section 23.2.5.10 ),PCNT
(Section 23.2.5.12 ),PWCNT (Section 23.2.5.11 ),andWCAP (Section 23.2.5.13 )show thatthe7-bit HR
data field cangenerate ormeasure high resolution delays (HR delay) relative tothestart ofanLRP within
oneN2HET loop LRP. The lastsection showed that:
LRP =lr×HRP
There arelrhigh resolution clock periods (HRP) within theN2HET loop resolution clock period (LRP). Iflr
=128then theHRdelay canrange from 0to127 HRP clocks within LRP andall7bitsoftheHRdata field
areneeded. Instead ofbeing limited tomeasuring andtriggering events based ontheloop resolution clock
period (LRP) theHRextension allows measurements andevents tobedescribed interms fractions ofan
LRP (down to1/128 ofanLRP). The only limitation isthatamaximum ofoneHRdelay canbespecified
perpinduring each loop resolution period.
Table 23-6 shows which bitsoftheHRdata field arenotused bythehigh resolution IOstructures iflris
less than 128. Inthiscase thenon-relevant bits(LSBs) oftheHRdata fields willbeoneofthefollowing:
*Written as0forHRcapture (forPCNT, WCAP)
*Orinterpreted as0forHRcompare (forECMP, MCMP. PWCNT)
(1)X=Non-relevant bit(treated as'0')Table 23-6. Interpretation ofthe7-Bit HRData Field
Loop Resolution
Prescale divide rate(lr)Bits oftheHRdata field(1)
HRP Cycles delay range D[6] D[5] D[4] D[3] D[2] D[1] D[0]
1 XXXXXXX 0
2 1/2 XXXXXX 0to1
4 1/2 1/4 XXXXX 0to3
8 1/2 1/4 1/8 XXXX 0to7
16 1/2 1/4 1/8 1/16 XXX 0to15
32 1/2 1/4 1/8 1/16 1/32 XX 0to31
64 1/2 1/4 1/8 1/16 1/32 1/64 X 0to63
128 1/2 1/4 1/8 1/16 1/32 1/64 1/128 0to127

<!-- Page 969 -->

www.ti.com N2HET Functional Description
969 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.3.2.1 Example:
Prescale Factor Register (HETPFR) =0x0300
-->lr=8-->LRP =8×HRP
Assumption: HRdata field =0x50 =1010000b
lr=8-->BitsD[3:0] areignored -->HRdelay =101b =5HRPs
orbyusing thecalculation with weight factors:
HRDelay
=lr·(D[6] ·1/2+D[5] ·1/4+D[4] ·1/8+D[3] ·1/16 +D[2] ·1/32 +D[1] ·1/64 +D[0] ·1/128)
=8·(1·1/2+0·1/4+1·1/8+0·1/16 +0·1/32 +0·1/64 +0·1/128)
=5HRPs
23.2.4 Host Interface
The host interface controls allcommunications between timer-RAM andmasters accessing theN2HET
RAM. Itincludes following components:
23.2.4.1 Host Accesses toN2HET RAM
The host interface supports thefollowing types ofaccesses toN2HET RAM:
*Read accesses of8,16,or32bits
*Read accesses of64-bits thatfollow theshadow register sequence described inSection 23.2.4.2 .
*Write accesses of32bits
Writes of8or16bitstoN2HET RAM byanexternal host arenotsupported.
23.2.4.2 64-bit Read Access
The consecutive read ofacontrol field CF(n) andadata field DF(n) ofthesame instruction (n)performed
bythesame master (forexample, CPU, DMA, oranyother master) isalways done asasimultaneous 64-
bitread access. This means thatatthesame time CF(n) isread, DF(n) isloaded inashadow register. So
thesecond access willread DF(n) from theshadow register instead oftheN2HET RAM.
Ingeneral a64-bit read access ofonemaster could beinterrupted bya64-bit read access ofanother
master. Atotal ofthree shadow registers areavailable. Therefore uptothree masters canperform 64-bit
reads inaninterleaved manner (Master1 CF,Master2 CF,Master3 CF,Master1 DF,Master2 DF,Master3
DF).
Ifallthree shadow registers areactivated anda4thmaster performs aCForDFread itwillresult inan
address error andtheRAM access willnothappen. Other access types byafourth master (reads from the
PFfield orwrites toanyofthefields) willoccur because these access types donotrequire anavailable
shadow register resource tocomplete.
23.2.4.3 Automatic Read Clear Feature
The N2HET provides afeature allowing toautomatically clear thedata field immediately after thedata field
isread bytheexternal host CPU (orDMA). This feature isimplemented viathecontrol bit,which is
located inthecontrol field (bitC26). This isastatic bitthatcanbeused byanyinstruction, andspecified in
theN2HET program byadding theoption (control =ON) totheN2HET instruction. The automatic read
clear feature works forboth 32and64bitreads thatfollow thesequence described inSection 23.2.4.2 .
When thehost CPU reads thedata field ofthatinstruction, thecurrent data value isreturned tothehost
CPU butthefield iscleared automatically asaside effect oftheread. Incase themaster reads data from
aninstruction currently executing, anynew capture result isstored andthistakes priority over the
automatic read clear feature, sothatthenew capture result isnotlost.

<!-- Page 970 -->

Timer data in
HET[x]Loop
Resolution
Clock
Timer data out
HETDIRHETDIN
HETDOUTHETDSET
HETDCLR
High Resolution
Structure
N2HET Functional Description www.ti.com
970 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleAsanexample ofwhere theautomatic read clear feature isuseful, consider thePCNT instruction. Ifthis
instruction isconfigured forautomatic read clear, then when thehost CPU reads thePCNT data field itwill
becleared automatically. The host CPU canthen pollthePCNT data field again, andaslong asthefield
returns avalue ofzero thehost CPU program knows anew capture event hasnotoccurred. Ifthedata
field were notcleared, itwould beimpossible forthehost CPU todetermine whether thedata field holds
data from theprevious capture event, orifithappens tobedata from anew capture event with thesame
value.
23.2.4.4 Emulation Mode
Emulation mode, used bythesoftware debugger, isspecified intheglobal configuration register. When
thehost CPU debugger hitsabreakpoint, theCPU sends asuspend signal tothemodules. Two modes of
operation areprovided: suspend andignore suspend.
*Suspend
When asuspend isissued, thetimer operation stops attheendofthecurrent timer instruction. However,
theCPU accesses tothetimer RAM orcontrol registers arefreely executed.
*Ignore suspend
The timer RAM ignores thesuspend signal andoperates realtime asnormal.
23.2.4.5 Power-Down
After setting theturn-off bitintheGlobal Configuration Register (HETGCR), itisrequired todelay until the
endofthetimer program loop before putting theN2HET inpower-down mode. This canbedone by
waiting until theN2HET Current Address (HETADDR) becomes zero, before disabling theN2HET clock
source inthedevice 'sGlobal Clock Module (GCM).
23.2.5 I/OControl
The N2HET hasupto32pins. Refer todevice specific data sheets forinformation concerning thenumber
ofN2HETIO available. AlloftheN2HET pins available areprogrammable aseither inputs oroutputs.
These 32I/Os have anidentical structure connected topins HET[31] toHET[0]. See Figure 23-8 foran
illustration oftheI/Ocontrol. Inaddition all32I/Os have aspecial HRstructure based ontheHRclock.
This structure allows anyN2HET instruction touseanyofthese I/Os with anaccuracy ofeither loop
resolution orhigh resolution accuracy.
Figure 23-8. I/OControl
Pins N2HET [31]toN2HET [0]canbeused bytheCPU asgeneral-purpose inputs oroutputs using the
N2HET Data Input Register (HETDIN) forreading andN2HET Data Output Register (HETDOUT), N2HET
Data SetRegister (HETDSET) orN2HET Data Clear Register (HETDCLR) forwriting, depending onthe
type ofaction toperform. The N2HET pins used asgeneral-purpose inputs aresampled oneach VCLK2
period.

<!-- Page 971 -->

Timer data in
HET[x]Loop
Resolution
Clock
Timer data out
HETDIRHETDIN
HETDOUTHETDSET
HETDCLR
www.ti.com N2HET Functional Description
971 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.1 Using General-Purpose I/OData SetandClear Registers
The N2HET Data Clear Register (HETDCLR) andN2HET Data SetRegister (HETDSET) canbeused to
minimize thenumber ofaccesses totheperipheral tomodify theoutput register andoutput pins. When the
application needs tosetortoreset some N2HET pins without changing thevalue oftheothers pins, the
firstpossibility istoread N2HET Data Output Register (HETDOUT), modify thecontent (AND, OR, andso
on), andwrite theresult intoN2HET Data Output Register (HETDOUT). However, thisread-modify-write
sequence could beinterrupted byadifferent function modifying thesame register which willresult ina
data coherency problem.
Using theN2HET Data SetRegister (HETDSET) orN2HET Data Clear Register (HETDCLR), the
application program must write themask value (same mask value forthefirstoption) totheregister toset
orreset thedesired pins. Any bitswritten as0toHETDSET andHETDCLR areleftunchanged, which
avoids thepossible coherency problem oftheread-modify-write approach.
Coding Example (Cprogram): Set pins using the 2methods.
unsigned int MASK; /*Variable that content the bit mask */
volatile unsigned int *HETDOUT,*HETDSET; /*Pointer toHET registers */
...
*HETDOUT =*HETDOUT |MASK; /*Read-modify-write ofHETDOUT */
*HETDSET =MASK; /*Set the pin without reading HETDOUT */
23.2.5.2 Loop Resolution Structure
The N2HET uses thepins N2HET [31:0] asinput and/or output bytheway oftheinstruction set.Actually,
each pincould monitor theN2HET program orcould bemonitored bytheN2HET program. Byusing the
I/Oregister oftheN2HET, theCPU isable tointeract with theN2HET program flow.
When anaction (setorreset) istaken onapinbytheN2HET program, theN2HET willmodify thepinat
therising edge ofthenext loop resolution clock.
When anevent occurs onaN2HET I/Opin,itistaken intoaccount atthenext rising edge oftheloop
resolution clock.
The structure ofeach pinisshown inFigure 23-9.
Figure 23-9. N2HET Loop Resolution Structure forEach Bit
The example inFigure 23-10 shows asimple PWM generation with loop resolution accuracy. The
corresponding program is:
HETPFR[31:0] register =0x201 -->lr=4 and hr=2 -->ts=8
N2HET Program:
L00 CNT {next= L01, reg=A, irq=OFF, max =4}
L01 ECMP {next= L00, cond_addr= L00, hr_lr=LOW, en_pin_action=ON, pin=0,
action=PULSEHI, reg=A, irq=OFF, data= 1,hr_data =0x0 }
;25bit compare value is1and the 7-bit HRcompare value is0

<!-- Page 972 -->

40 1 2 3 4 0 10 1 0VCLK2
HR Clock
LR Clock
Instruction
Counter
Pin HET[0]
Z-FlagLRP
HRP
25-bit ECMP
matchPin action in next
loop resolution cycleCNT resets
Sets Z-FlagOpposite Pin action in
next loop resolution
cycle1 1 1 1 1 1 0 0 0 0 0
N2HET Functional Description www.ti.com
972 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe CNT andECMP instructions areexecuted once each loop resolution cycle. When theCNT instruction
isexecuted, thespecified register (A)andtheCNT instruction data field areboth incremented byone.
Next theECMP isexecuted andthedata field oftheECMP iscompared with thespecified register (A).If
both values match, then thepinaction (PULSEHI inthiscase) willbeperformed inthenext loop resolution
cycle. The CNT continues incrementing each loop resolution cycle. When thedata field overflows (max +
1),then theZ-flag issetbytheCNT instruction. Inthenext loop resolution cycle, theZ-flag isevaluated
andtheopposite pinaction isperformed ifitisset.The Z-flag willonly beactive foroneloop resolution
cycle.
Figure 23-10. Loop Resolution Instruction Execution Example
23.2.5.3 High Resolution Structure
All32I/Os provide theHRstructure based ontheHRclock. The HRclock frequency isprogrammed
through thePrescale Factor Register (HETPFR). Inaddition tothestandard I/Ostructure, allpins have HR
hardware sothatthese pins canbeused asHRinput captures (using theHRinstructions PCNT or
WCAP) orHRoutput compares (using theHRinstructions ECMP, MCMP, orPWCNT).
AllfiveHRinstructions (PCNT, WCAP, ECMP, MCMP, andPWCNT) have adedicated hr_lr bit(high
resolution/low resolution; program field bit8)allowing operation either inHRmode orinstandard
resolution mode byignoring theHRfield. Bydefault, thehr_lr bitvalue is0which implies HRoperation
mode. However, setting thisbittooneallows theuseofseveral HRinstructions onasingle HRpin.Only
oneinstruction isallowed tooperate inHRmode (bitcleared to0),buttheother instructions canbeused
instandard resolution mode (bitsetto1).

<!-- Page 973 -->

HR control logic
HR flags
HR up/down counter (7 bits)
HR registerTimer data in
HR prescale driver
HR compare dataResolution clockTimer data in
HET[x]
Timer data out>
Loop
Resolution
ClockHETDIR
HETDIN
HETDSET
HETDOUT
HETDCLR
{HR
Structure
One Per
Pin
www.ti.com N2HET Functional Description
973 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.4 HRBlock Diagram
Each time anHRinstruction isexecuted onagiven pin,theHRstructure forthatpinisprogrammed and
synchronized tothenext loop-resolution cycle (which HRfunction toperform andonwhich edges itshould
take anaction) with theinformation given bytheinstruction. The HRstructure foreach pindecodes the
pinselect field oftheinstruction andprograms itsHRstructure ifitmatches.
NOTE: Foreach N2HET pin,only oneinstruction specifying ahigh resolution operation
(hr_lr =HIGH) isallowed toexecute perloop resolution period. This includes any
instructions where (hr_lr =HIGH) but(en_pin_action =OFF).
The firsthigh resolution instruction thatexecutes andspecifies aparticular pinlocks out
subsequent high resolution instructions from operating onthesame pinuntil theendfothe
current loop resolution period.
Figure 23-11. HRI/OArchitecture

<!-- Page 974 -->

HET[0]
HET[1]N2HET
HR 0
N2HET
HR 101
HR share 1/0
N2HET Functional Description www.ti.com
974 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.5 HRStructures Sharing (Input)
The HRShare Control Register (HETHRSH) allows twoHRstructures toshare thesame pinforinput
capture only .Ifthese bitsareset,theHRstructures NandN+1 areconnected topinN.Inthisstructure,
pinN+1 remains available forgeneral-purpose input/output. See Figure 23-12 .
Figure 23-12. Example ofHRStructure Sharing forN2HET Pins 0/1
The following program gives anexample how theHRshare feature (HET[0] HRstructure andHET[1] HR
structure shared) canbeused forthePCNT instruction:
L00 PCNT {next=L01, type=rise2fall, pin=0 }
L01 PCNT {next=L00, type=fall2rise, pin=1 }
The HET[1] HRstructure isalso connected totheHET[0] pin.The L00_PCNT data field isable tocapture
ahigh pulse andtheL01_PCNT captures alowpulse onthesame pin(N2HET [0]pin).

<!-- Page 975 -->

HET[0]
HET[1]00
1HETXOR0
HETXOR0N2HET HR 0
N2HET HR 1
www.ti.com N2HET Functional Description
975 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.6 AND /XOR-shared HRStructure (Output)
Usually theN2HET design allows only oneHRstructure togenerate HRedges onapinconfigured as
output pin.The HETXOR register allows alogical XOR oftheoutput signals oftwoconsecutive HR
structures N(even) andN+1 (odd). See Figure 23-13 .Inthisway, itispossible togenerate pulses smaller
than theloop resolution clock since both edges canbegenerated bytwoindependent HRstructures. This
isespecially required forsymmetrical PWM. See Figure 23-14 .
The hardware provides aXOR gate thatisconnected totheoutputs oftheHRstructure oftwo
consecutive pins. Inthisstructure, pinN+1 remains available forgeneral-purpose input/output.
Figure 23-13. XOR-shared HRI/O
The following N2HET program gives anexample forone channel ofthesymmetrical PWM. The generated
timing isgiven inFigure 23-14 .
MAXC .equ 22
A_ .equ 0;HRstructure HR0
B_ .equ 1;HRstructure HR1
CNCNT {next=EA, reg=A, max=MAXC }
EAECMP {next=EB, cond_addr=MA, hr_lr=HIGH, en_pin_action=ON, pin=A_,
action=PULSELO, reg=A, data=17, hr_data=115 }
MAMOV32 {next=EB, remote=EA, type=IMTOREG &REM, reg=NONE, data=17, hr_data=19 }
EBECMP {next=CN, cond_addr=MB, hr_lr=HIGH, en_pin_action=ON, pin=B_,
action=PULSELO, reg=A, data=5, hr_data=13 }
MBMOV32 {next=CN, remote=EB, type=IMTOREG &REM, reg=NONE, data=5, hr_data=13 }
N2HET Settings andoutput signal calculation forthisexample program:
*PinHET[0] andHET[1] areXOR-shared.
*HETPFR[31:0] register =0x700: lr=128, hr=1, time slots ts=128
*PWM period (determined byCNT_max field) =(22+1) ·LRP =2944 HRP
*Length ofhigh pulse of(HET[0] XOR HET[1]) =
LH=(17·LRP+115 ·HRP) -(5·LRP+13 ·HRP)
With lr=128 there isLRP =128·HRP, so
LH=(2291 -653) ·HRP =1638 HRP
*Duty cycle =DC=LH/PWM_period =1638 HRP /(2944 ·HRP) =55.6 %

<!-- Page 976 -->

N2HET HR 0HET[0]
00
1HETAND0
HETAND0N2HET HR 1 HET[1]
HET[0 ]
HR0
HR1Asym metric al
counter
(CNT )Symmetrical
counter
(not in HET)
N2HET Functional Description www.ti.com
976 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-14 graphically shows theimplementation oftheXOR-shared feature. The first2waveforms
(symmetrical counter andCNT) show asymmetric counter andasymmetric counter. The symmetric
counter isshown only tohighlight theaxis ofsymmetry andisnotimplemented intheN2HET. The
asymmetric counter, which isimplemented with aCNT instruction, needs tobesettotheperiod ofthe
symmetric counter. The next twowaveforms (HR [0]andHR[1])show theoutput oftheHRstructures,
which aretheinputs fortheXOR gate tocreate thePWM output onpinHET[0]. Notice thatthepulses of
signal HET[0] arecentered about theaxis ofsymmetry.
Figure 23-14. Symmetrical PWM with XOR-sharing Output
Asanalternative, HRstructures may beshared using alogical AND function tocombine theeffects ofthe
pinstructures. The HETAND allows sharing twoconsecutive HRstructures N(even) andN+1 (odd). See
Figure 23-15 .Inthisstructure, pinN+1 remains available forgeneral-purpose input/output.
NOTE: Setting both theHETAND bitandHETXOR bitsatthesame time foragiven pairofN2HET
pins isnotsupported, must beavoided bytheapplication program.
Figure 23-15. AND-shared HRI/O

<!-- Page 977 -->

HR 0
HR 1X
XOutput
BufferOutput
BufferPin 0
Pin 1Loopback values will NOT be
seen on the pins in Digital
Loopback Mode
LBPDIR [0] value
determines which HR
block is input and which
is output
LBSEL[0] value
determines whether or
not loopback is enabled
for these two blocks
www.ti.com N2HET Functional Description
977 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.7 Loop Back Mode
The loop back feature canbeused bytheapplication tomonitor anN2HET output signal. Forexample, if
aPWM isgenerated byHRstructure 0,then aPCNT instruction assigned toHRstructure 1canmeasure
back thepulse length orperiods ofthePWM output signal.
Loopback mode isactivated between twohigh resolution structures bysetting LBPSEL[x] to1inthe
HETLBPSEL register forthecorresponding structure pair. Thedirection oftheloopback between thetwo
structures inthestructure pairisdetermined bythevalue ofLBPDIR[x] intheHETLBPDIR Register.
Forexample, ifbitLBPSEL[0] issetto1,then HRstructures 0and1willbeinternally connected inloop
back mode. IfbitLBPDIR[0] issetto0,then structure 0willbetheinput andstructure 1willbetheoutput.
Digital Loopback
Digital loopback mode isenabled bysetting LBPTYPE[x] to0intheHETLBPSEL register forthe
corresponding structure pairs. Indigital loopback mode, thestructure pairs areconnected directly andthe
output buffers arebypassed. Therefore, theloopback values willNOT beseen onthecorresponding pins.
Figure 23-16 shows anexample ofdigital loopback between structures HR0 andHR1. LBSEL[0] hasbeen
setto1toenable loopback between thetwostructures. LBTYPE[0] hasbeen setto0toselect digital
mode fortheloopback pair. The LPBDIR[0] value willdetermine thedirection oftheloopback byselecting
which oftheHRblocks isoutput, andwhich isinput. The bold lines show thedigital loopback path.
Figure 23-16. HR0 toHR1 Digital Loopback Logic: LBTYPE[0] =0

<!-- Page 978 -->

HR 0
HR 1X
XOutput
BufferOutput
BufferLBPDIR [0] value
determines which HR
block is input and which
is output
LBSEL[0] value
determines whether or
not loopback is enabled
for these two blocksPin 0
Pin 1Loopback values WILL be seen
on the pins in Analog Loopback
Mode
N2HET Functional Description www.ti.com
978 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleAnalog Loopback
Analog loopback mode isenabled bysetting LBPTYPE[x] to1intheHETLBPSEL register forthe
corresponding structure pairs. Inanalog loopback mode, thestructure pairs areconnected outside ofthe
output buffers. Therefore, theloopback values WILL beseen onthecorresponding pins. Figure 23-17
shows anexample ofanalog loopback between structures HR0 andHR1. LBSEL[0] hasbeen setto1to
enable loopback between thetwostructures. LBTYPE[0] hasbeen setto1toselect analog mode forthe
loopback pair. The LPBDIR[0] value willdetermine thedirection oftheloopback byselecting which ofthe
HRblocks isoutput, andwhich isinput. The bold lines show theanalog loopback path.
Figure 23-17. HR0 toHR1 Analog Loop Back Logic: LBTYPE[0] =1
Note:
*The loop back direction canbeselected independent oftheHETDIR register setting.
*The pinthatisnotdriven bytheN2HET output pinactions canstillbeused asnormal GIO pin.

<!-- Page 979 -->

N2HETx
3
4
21
www.ti.com N2HET Functional Description
979 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.8 Edge Detection Input Timing
There areseveral timing requirements forinput signals inorder tobecaptured correctly byN2HET.
Figure 23-18 illustrates these requirements, with minandmax values described inTable 23-7 (Loop
Resolution) andTable 23-8 (High Resolution).
Figure 23-18. N2HET Input Edge Detection
Table 23-7. Edge Detection Input Timing forLoop Resolution Instructions
Parameter # Description min max
1 Input Signal Period, rising edge torising edge
>2(hr)(lr)tc(VCLK2)
<225(hr)(lr)tc(VCLK2)2 Input Signal Period, falling edge tofalling edge
3 Input Signal, high phase
>(hr)(lr)tc(VCLK2)4 Input Signal, high phase
Table 23-8. Edge Detection Input Timing forHigh Resolution Instructions
Parameter # Description min max
1 Input Signal Period, rising edge torising edge
>(hr)(lr)tc(VCLK2)
<225(hr)(lr)tc(VCLK2)2 Input Signal Period, falling edge tofalling edge
3 Input Signal, high phase
>2(hr)tc(VCLK2)4 Input Signal, high phase
These aretheN2HET architectural limitations. Actual limitations willbeslightly different duetoonchip
routing andIObuffer delays, usually byseveral nanoseconds. Besure toconsult thedevice datasheet for
actual timings thatapply tothatdevice. Also, certain devices place additional restrictions onwhich pins
support thehigh resolution timings ofTable 23-8 ,ifpresent these additional limitations willalso becalled
outinthedevice datasheet.
Note thatthemax limit inTable 23-7 andTable 23-8 isbased onthecounter range ofasingle N2HET
instruction. The max value could beextended byemploying anadditional N2HET instruction tokeep track
ofcounter overflows oftheinput counter /capture instruction.
23.2.5.9 PWM Generation Example 1(inHRMode)
The following example shows how anECMP instruction works inhigh resolution mode. The example
assumes aVCLK2 of32MHz andthefollowing values fortheprescale divide rates (hrandlr),number of
time slots (ts), high andloop resolution period (HRP andLRP):
hr=2,lr=4,ts=hr×lr=8
HRP =hr/VCLK2 =2/32MHz =62.5 ns
LRP =(hr×lr)/VCLK2 =8/32MHz =250ns
With ts=8,there areeight time slots available fortheprogram execution, which inthiscase willconsist of
oneCNT andoneECMP instruction asshown below. The data field oftheECMP instruction isthe32-bit
compare value, whereby thelower 7bitsrepresent thehigh resolution compare field.

<!-- Page 980 -->

0 2 1 00 1 01 01 01 01 01 01VCLK2
HR Clock
LR Clock
Instruction
LR Counter
Pin HET[0]
Z-FlagLRP
HRP
25-bit ECMP
matchPin action in next
loop resolution cycle
+
high resolution delayCNT resets
Sets Z-FlagOpposite Pin action in
next loop resolution
cycle40 1 2 3 4 0 1
HR Counter
HR delay
N2HET Functional Description www.ti.com
980 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleWhen the25-bit (loop resolution) compare matches, theHRcompare value willbeloaded from the7lower
bitsoftheinstruction data field totheHRcounter. Atthenext loop resolution clock, theHRcounter will
count down attheHRclock frequency andperform thepinaction when itreaches zero.
Intheexample illustrated byFigure 23-19 ,the25-bit compare value is1andthe7-bit HRcompare value
is2.According toSection 23.2.3.2 ,depending ontheloop resolution divide rate (lr),only certain bitsofthe
7-bit HRcompare value arevalid. Inthisexample only theupper 2bits(D[6:5]) aretaken intoaccount.
The example program below hasasetting ofhr_data =100000b. Shifting thisvalue right by5bits, results
in10bwhich equals thetwoHRclock cycles delay mentioned above.
Figure 23-19. ECMP Execution Timings
HETPFR[31:0] register =0x201 -->lr=4 and hr=2 -->ts=8
N2HET Program:
L00 CNT {next= L01, reg=A, irq=OFF, max =4}
L01 ECMP {next= L00, cond_addr= L00, hr_lr=HIGH, en_pin_action=ON, pin=0,
action=PULSEHI, reg=A, irq=OFF, data= 1,hr_data =0x40 }
;25bit compare value is1and the 7-bit HRcompare value is2
;(Because oflr=4 the D[4:0] ofthe 7-bit HRfield are ignored )
NOTE: ECMP Opposite Actions
ECMP opposite pinactions arealways synchronized totheloop resolution clock.
Changing theduty cycle ofaPWM generated byanECMP instruction, canlead toamissing pulse ifthe
data field oftheinstruction isupdated directly. This canhappen when itischanged from ahigh value toa
lower value while theCNT instruction hasalready passed thenew updated lower value. Toavoid thisa
synchronous duty cycle update canbeperformed with theuseofanadditional instruction (MOV32). This
instruction isonly executed when thecompare oftheECMP matches. Forthisthecond_addr oftheECMP
needs topoint totheMOV32. Onexecution oftheMOV32, itmoves itsdata field intothedata field ofthe
ECMP. The update oftheduty cycle hastobemade totheMOV32 data field instead oftheECMP data
field.

<!-- Page 981 -->

HR clock ECMP
Pin
actionOpposite
pin actionLR clock
LR clock HR clock PWCNT
Pin
actionOpposite
pin action
www.ti.com N2HET Functional Description
981 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.10 PWM Generation Example 2(inHRMode)
The MCMP instruction canalso beused inHRmode. Inthiscase operation isexactly thesame asforthe
ECMP instruction except thatthe25-bit lowresolution isnow theresult ofamagnitude compare (greater
orequal) rather than anequality compare. When the25-bit (loop resolution) magnitude compare matches,
theHRcompare value willbeloaded from the7lower bitsoftheinstruction data field totheHRcounter.
Atthenext loop resolution clock, theHRcounter willcount down attheHRclock frequency andperform
thepinaction when itreaches zero.
The MCMP instruction avoids themissing pulse problem oftheECMP instruction (see previous example),
however theduty cycle ofthesignal might notbeexact foronePWM period. The benefit oftheMCMP is
thatitavoids adding another instruction todotheduty cycle update synchronously.
23.2.5.11 Pulse Generation Example (inHRMode)
The PWCNT instruction may also beused inHRmode togenerate pulse outputs with HRwidth. It
generates asingle pulse when thedata field oftheinstruction isnon-zero. Itremains attheopposite pin
action when thedata field iszero.
The PWCNT instruction operates conversely totheECMP instruction. See Figure 23-20 .ForPWCNT, the
opposite pinaction issynchronous with theHRclock andforECMP thepinaction issynchronous with the
HRclock. The PWCNT pinaction issynchronous with theloop resolution clock.
Figure 23-20. High/Low Resolution Modes forECMP andPWCNT
23.2.5.12 Pulse Measurement Example (inHRMode)
The PCNT instruction captures HRmeasurement ofthehigh/low pulse time orperiods oftheinput. As
shown inFigure 23-21 ,atmarker (1)theinput goes HIGH andtheHRcounter immediately begins to
count. The counter increments androlls over until thefalling edge ontheinput pin,where itcaptures the
counter value intotheHRcapture register (marker (2)). The PCNT instruction begins counting when the
synchronized input signal goes HIGH andcaptures both the25-bit data field andtheHRcapture register
intoRAM when thesynchronized input falls (marker (3)).
NOTE: The HRcapture value written intoRAM isshifted appropriately depending ontheloop
resolution prescale divide rate (lr).(See also Section 23.2.3.2 ).

<!-- Page 982 -->

HR clock
Loop res
clock
PCNT CF
HR counter
HR capt.
reg
PCNT DF
Input pin
Input pin
sync'dX 0 1 2
012301230 0
X 3
X 1
1 2 3
HR clock
Loop res
clock
PCNT CF
HR counter
HR capt.
reg
PCNT DF
Input pin
Input pin
sync'dX 0 1 2
012301230 0
X 1
X 2
1 2 3
N2HET Functional Description www.ti.com
982 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-21 shows what happens when thecapture edge arrives after theHRcounter overflows. This
causes theincremented value tobecaptured bythePCNT instruction.
Figure 23-21. PCNT Instruction Timing (With Capture Edge After HRCounter Overflow)
Figure 23-22 shows what happens when thecapture edge arrives before theHRcounter overflows. This
causes thenon-incremented value tobecaptured bythePCNT instruction.
Figure 23-22. PCNT Instruction Timing (With Capture Edge Before HRCounter Overflow)

<!-- Page 983 -->

HR clock
Loop res
clock
HR counter
HR capt.
reg
WCAP DFInput pin HET[0]0 1 2301230
X 2
X 4InstructionCNT WCAP CNT WCAP CNT WCAP CNT WCAP CNT WCAP CNT WCAP
0 A register 1 2 3 4 5 6
12301230123012 3
WCAP
Previous bit
0x0240 captured to WCAP DF [31:0]LRP
HRP
sync'd to VCLK2
Input pin HET[0]
sampled by LRP
www.ti.com N2HET Functional Description
983 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.13 WCAP Execution Example (inHRMode)
The HRcapability isenabled forWCAP, ifitshr_lr bitiszero. Inthiscase theHRcounter isalways
enabled andissynchronized with theresolution loop. When thespecified edge isdetected, thecurrent
value oftheHRcounter iscaptured intheHRcapture register andwritten intotheRAM after thenext
WCAP execution. The WCAP instruction effectively time stamps thefree running timer saved inaregister
(forexample, register Ashown inFigure 23-23 ).
Figure 23-23. WCAP Instruction Timing
HETPFR_register =0x0200 -->lr=4,hr=1,ts=4
N2HET Program:
L00 CNT {reg=A, max=01ffffffh}
L01 WCAP {next=L00, cond_addr=L00, hr_lr=high, reg=A, event= FALL, pin=0,
data=0}
Intheexample, theWCAP isconfigured tocapture thecounter when afalling edge occurs. The WCAP
data field (WCAP_DF) isupdated intheloop succeeding theloop inwhich theedge occurred. The WCAP
instruction evaluates anedge bycomparing itsPrevious bitwith thesync 'dinput signal. InFigure 23-23 ,
thecurrent value ofthecounter (4)iscaptured toWCAP_DF[31:7] andthevalue oftheHRcapture
register (2)istransferred tothevalid bits(according thelrprescaler) ofWCAP_DF[6:0]. Therefore, inthe
example 0x0240 iscaptured inWCAP_DF[31:0].

<!-- Page 984 -->

Output enable
Data out
Data in
Pull control disable
Pull selectN2HET pin
Pull control
logicInput enable
N2HET Functional Description www.ti.com
984 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.5.14 I/OPullControl Feature
Figure 23-24. I/OBlock Diagram Including PullControl Logic
The following apply ifthedevice isunder reset:
*Pullcontrol: The reset pullcontrol onthepins isenabled andapulldown isconfigured.
*Input buffer: The input buffer isenabled.
*Output buffer: The output buffer isdisabled.
The following apply ifthedevice isoutofreset:
*Pullcontrol: The pullcontrol isenabled byclearing thecorresponding bitintheN2HET PullDisable
Register (HETPULDIS). Inthiscase, ifthecorresponding bitintheN2HET PullSelect Register
(HETPSL) isset,thepinwillhave apull-up; ifthebitintheN2HET PullSelect Register (HETPSL) is
cleared, thepinwillhave apull-down. IfthebitintheN2HET PullDisable Register (HETPULDIS) is
set,there isnopull-up orpull-down onthepin.
*Input buffer: The input buffer isdisabled only ifthepindirection issettoinput AND thepullcontrol is
disabled AND pulldown isselected asthepullbias. Inallother cases, theinput buffer isenabled.
NOTE: The pull-disable logic depends onthepindirection. Ifthepinisconfigured asoutput, then
thepulls aredisabled automatically. Ifthepinisconfigured asinput, thepulls areenabled or
disabled depending onthepulldisable register bit.
*Output buffer: Apincanbedriven asanoutput pinifthecorresponding bitintheN2HET Direction
Register (HETDIR) issetAND theopen-drain feature (N2HET Open Drain Register (HETPDR)) isnot
enabled. See Section 23.2.5.15 formore details.
The behavior oftheinput buffer, output buffer, andthepullcontrol issummarized inTable 23-9 .When an
input buffer isdisabled, itappears asalogic lowtoon-chip logic.

<!-- Page 985 -->

N2HET pin10
to other N2HET pin structures
N2HET pin enablenDIS pin*
*nDIS pin realized by GIOA[5] (N2HET1) and GIOB[2] (N2HET2)A BHETPINDIS
HETDIR
HETDINHETDOUT
www.ti.com N2HET Functional Description
985 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module(1)X=Don'tcareTable 23-9. Input Buffer, Output Buffer, andPullControl Behavior
Device
under
Reset?PinDirection
(DIR)(1)PullDisable
(PULDIS)(1)PullSelect
(PULSEL)(1)PullControl Output Buffer Input Buffer
Yes X X X Enabled Disabled Enabled
No 0 0 0 Pulldown Disabled Enabled
No 0 0 1 Pullup Disabled Enabled
No 0 1 0 Disabled Disabled Disabled
No 0 1 1 Disabled Disabled Enabled
No 1 X X Disabled Enabled Enabled
23.2.5.15 Open-Drain Feature
The following apply iftheopen-drain feature isenabled onapin,thatis,thecorresponding bitinthe
N2HET Open Drain Register (HETPDR) isset:
*Output buffer isenabled ifalowsignal isbeing driven internally tothepin.
*The output buffer isdisabled ifahigh signal isbeing driven internally tothepin.
23.2.5.16 N2HET PinDisable Feature
This feature isprovided forthesafe operation ofsystems such aspower converters andmotor drives. It
canbeused toinform themonitoring software ofmotor drive abnormalities such asover-voltage, over-
current, andexcessive temperature rise.
Table 23-10 shows theconditions fortheoutput buffer tobeenabled/disabled.
Figure 23-25. N2HET PinDisable Feature Diagram

<!-- Page 986 -->

N2HET Functional Description www.ti.com
986 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-10. N2HET PinDisable Feature
HETPINDIS.x nDIS Pin(Input) HET_PIN_ENA (HETGCR.24) HETDIR.x Output Buffer
0 X X 0 Disabled
0 X X 1 Enabled
1 0 X 0 Disabled
1 0 X 1 Disabled
1 1 X 0 Disabled
1 1 0 1 Disabled
1 1 1 1 Enabled
Aninterrupt capable device I/Opincanshare thesame pinastheN2HET nDIS signal. Normally GIOA[5]
serves asnDIS forN2HET1 andGIOB[2] asnDIS forN2HET2. Check thedevice datasheet fortheactual
implementation. Sharing apinwith aGIO pinthatisInterrupt capable allows theN2HET nDIS input to
also generate aninterrupt totheCPU. Anactive lowlevel onnDIS isintended tosignal anabnormal
situation asdescribed above. AllN2HET pins, which areselected with theN2HET PinDisable Register
(HETPINDIS), willbeputinthehigh-impedance state byhardware immediately after thenDIS signal is
pulled low. Atthistime aCPU interrupt isissued, ifitisenabled intheGIO pinlogic.
The bitHET_PIN_ENA isautomatically cleared inthefailure condition andthisstate remains aslong as
thesoftware explicitly sets thebitagain. The steps todothisare:
*Software detects, byreading theHETDIN register oftheGIO pin,thatthelevel onnDIS isinactive
(high).
*Software sets bitHET_PIN_ENA todeactivate thehigh impedance state ofthepins.
23.2.6 Suppression Filters
Each N2HET pinisequipped with asuppression filter. Ifthepinisconfigured asaninput itenables tofilter
outpulses shorter than aprogrammable duration. Each filter consists ofa10-bit down counter, which
starts counting ataprogrammable preloaded value andisdecremented using theVCLK2 clock.
*The counter starts counting when thefilter input signal hastheopposite state ofthefilter output signal.
The output signal ispreset tothesame input signal state after reset, inorder toensure proper
operation after device reset.
*Once thecounter reaches zero without detecting anopposite pinstate onthefilter input signal, the
output signal issettotheopposite state.
*When thecounter detects anopposite pinaction onthefilter input signal before reaching zero, the
counter isloaded with it'spreload value andtheopposite pinaction onthefilter output signal does not
take place. The counter resumes atthepreload value until itdetects anopposite pinaction onthe
input signal again.
*Therefore thefilter output signal isdelayed compared tothefilter input signal. The amount ofdelay
depends onthecounter clock frequency (VCLK2) andtheprogrammed preload value.
*The accuracy oftheoutput signal is+/-thecounter clock frequency.
Table 23-11 gives examples fora100MHz VCLK2 frequency.

<!-- Page 987 -->

Filter input
Filter outputCounter 0Filter input
Filter outputCounterpreload value
0
www.ti.com N2HET Functional Description
987 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-26. Suppression Filter Counter Operation
Table 23-11. Pulse Length Examples forSuppression Filter
Divider CCDIV VCLK2Possible values forthesuppressed pulse length /frequency resulting from the
programmable 10bitpreload value (0,1,..,1023)
1 100.0 MHz 10ns,20ns,...,10.22 µs,10.23 µs 50MHz, 25MHz, ...,48.924 kHz, 48.876 kHz
2 50.0 MHz 20ns,40ns,...,20.44 µs,20.48 µs 25MHz, 12.5 MHz, ...,24.462 kHz, 24.414 kHz
3 33.3 MHz 30ns,60ns,...,30.66 µs,30.69 µs 16.7 MHz, 8.3MHz, ...,16.308 kHz, 16.292 kHz
23.2.7 Interrupts andExceptions
N2HET interrupts canbegenerated byanyinstruction thathasaninterrupt enable bitinitsinstruction
format. When theinterrupt condition inaninstruction istrue andtheinterrupt enable bitofthatinstruction
isset,aninterrupt flagisthen setintheN2HET Interrupt Flag Register (HETFLG). The address code for
thisflagisdetermined bythefiveLSBs ofthecurrent timer program address. The flagintheN2HET
Interrupt Flag Register (HETFLG) isseteven ifthecorresponding bitintheN2HET Interrupt Enable Set
Register (HETINTENAS) is0.Togenerate aninterrupt, thecorresponding bitintheN2HET Interrupt
Enable SetRegister (HETINTENAS) must be1.IntheN2HET interrupt service routine, themain CPU
must firstdetermine which source inside theN2HET created theinterrupt request. This operation is
accelerated bytheN2HET Offset Index Priority Level 1Register (HETOFF1) orN2HET Offset Index
Priority Level 2Register (HETOFF2) thatautomatically provides thenumber ofthehighest priority source
within each priority level. Reading theoffset register willautomatically clear thecorresponding N2HET
interrupt flagthatcreated therequest. However, iftheoffset registers arenotused bytheN2HET interrupt
service routine, theflagshould becleared explicitly bytheCPU once theinterrupt hasbeen serviced.
Table 23-12. Interrupt Sources andCorresponding Offset Values inRegisters HETOFFx
Source No. Offset Value
nointerrupt 0
Instruction 0,32,64... 1
Instruction 1,33,65... 2
: :
Instruction 31,63,95... 32
Program Overflow 33
APCNT underflow: 34
APCNT overflow 35

<!-- Page 988 -->

5 LSB address
code 00000
5 LSB address
code 1 1 1 1 1Interrupt enableInterrupt condition
Interrupt condition
Interrupt enableInterrupt
Flag 0
Interrupt
Flag 31
N2HET Functional Description www.ti.com
988 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe instructions capable ofgenerating interrupts arelisted inTable 23-75 .
Figure 23-27. Interrupt Functionality onInstruction Level
Each interrupt source isassociated with apriority level (level 1orlevel 2).When multiple interrupts with
thesame priority level occur during thesame loop resolution thelowest flagbitisserviced first.
Inaddition totheinterrupts generated bytheinstructions theN2HET cangenerate three additional
exceptions:
*Program overflow
*APCNT underflow (see Section 23.3.1.2 )
*APCNT overflow (see Section 23.3.1.3 )
23.2.8 Hardware Priority Scheme
Iftwoormore software interrupts arepending onthesame priority level, theoffset value willshow theone
with thehighest priority. The interrupt with thehighest priority istheonewith thelower offset value. This
scheme ishard-wired intheoffset encoder. See Figure 23-28 .

<!-- Page 989 -->

Offset index
encoder
for level 2
priorityHET interrupt priority 2
offset vectorPriority 1 global
interrupt request
Priority 2 global
interrupt requestOffset index
encoder
for level 1
priorityHET interrupt priority 1
offset vector
T o V ectored
Interrupt ManagerPL
bit 0
SW Int
flag 0
PL
bit 1
SW Int
flag 1
PL
bit 23
SW Int
flag 23
PL
bit 24
SW Int
flag 24
PL
bit 31
SW Int
flag 31
PL
bit 34
Exc Int
En 2
Exc Int
flag 2
www.ti.com N2HET Functional Description
989 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-28. Interrupt Flag/Priority Level Architecture

<!-- Page 990 -->

HTUDCP[0]
DCP[1]
DCP[2]
DCP[3]
DCP[4]
DCP[5]
DCP[6]
DCP[7]
DMADMAREQ[20]
DMAREQ[21]
DMAREQ[24]
DMAREQ[25]N2HETDMAREQ[0]
HTUREQ[0]
DMAREQ[1]
HTUREQ[1]
DMAREQ[2]
HTUREQ[2]
DMAREQ[3]
HTUREQ[3]
DMAREQ[4]
HTUREQ[4]
DMAREQ[5]
HTUREQ[5]
DMAREQ[6]
HTUREQ[6]
DMAREQ[7]
HTUREQ[7]
N2HET Functional Description www.ti.com
990 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.2.9 N2HET Requests toDMA andHTU
Asdescribed inSection 23.6.3 ,themajority oftheN2HET instructions areable togenerate atransfer
request totheHigh-End Timer Transfer Unit (HTU) and/or totheDMA module when aninstruction-specific
condition istrue. One N2HET instruction canselect oneof8request lines byprogramming the"reqnum "
parameter. The "request "field inaninstruction isused toenable, disable, ortogenerate aquiet request
(see Section 23.6.2 )ontheselected request line. Quiet requests canbeused bytheHTU, butnotbythe
DMA. Forquiet request, refer totheHigh-End Timer Transfer Unit (HTU) Module chapter (see
Section 24.2.4.1 ).
The configuration oftheN2HET Request Destination Select Register (HETREQDS) bitsdetermines ifa
request linetriggers anHTU-DCP, aDMA channel orboth. This means theregister bitswilldetermine
whether anN2HET instruction triggers DMAREQ[x], HTUREQ[x] orboth signals (shown inFigure 23-29 ).
The request linenumber xcorresponds tothe"reqnum "parameter used intheinstruction.
Figure 23-29. Request Line Assignment Example
23.3 Angle Functions
Engine management systems require anangle-referenced time base tosynchronize signals totheengine
toothed wheel. The N2HET hasamethod toprovide such atime base forlow-end engine systems. The
reference iscreated bytheN2HET using three dedicated instructions with fractional angle steps equal to
/8,/16,/32,/64.
23.3.1 Software Angle Generator
The N2HET provides three specialized count instructions togenerate anangle referenced time base
synchronized toanexternal reference signal (the toothed wheel signal) thatdefines angular reference
points.
The time base isused togenerate fractional angle steps between thereference points. The step width K
(=8,16,32,or64)programmed bytheuser defines theangle accuracy ofthetime base. These fractional
steps arethen accumulated inanangle counter toform theabsolute angle value.
The firstcounter, APCNT, incremented oneach loop resolution clock measures theperiods P(n) ofthe
external signal. The second counter SCNT counts bystep Kuptotheprevious period value P(n-1),
measured byAPCNT, andthen recycles. The resulting period ofSCNT isthefraction P(n-1) /K.The third
counter ACNT accumulates thefractions generated bySCNT.
Figure 23-30 illustrates thebasic operation ofAPCNT, SCNT, andACNT.
AN2HET timer program canonly have oneangle generator.

<!-- Page 991 -->

N1=N0+nK-P(n-1)N1+KN1+2KFinal Count = N1+mK Final Count = N0+nK
E
EN2=N1+mK-P(n-1)Target=P(n-1)
SCNT
step
counter
N0N0+KN0+2KN0+3K
P(n) P(n-1)
P(n-1)
K
K countsHET[2]
ext. ref.
signal
APCNT
period
counter
SCNT
step
counter
ACNT
angle
generatorP(n-1)
K
www.ti.com Angle Functions
991 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-30. Operation ofN2HET Count Instructions
Due tostepping, thefinal count ofSCNT does notusually exactly match thetarget value P(n-1).
Figure 23-31 illustrates how SCNT compensates forthisfeature bystarting each cycle with theremainder
(final count -target) oftheprevious cycle.
Figure 23-31. SCNT Count Operation

<!-- Page 992 -->

P(n) P(n+1)HET[2]
ext. ref.
signal
APCNT
period
counter
SCNT
step
counter
ACNT
angle
generatorDeceleration Acceleration
ACF
AccelerationK-10 1 2 0 1 2 0 1
flagDCF
Deceleration
flagK-1
Angle Functions www.ti.com
992 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleACNT detects period variations oftheexternal signal measured byAPCNT andcompensates related
counting errors. Aperiod increase isflagged inthedeceleration flag. Aperiod decrease isflagged inthe
acceleration flag. Ifnovariation isflagged, ACNT increments thecounter value each time SCNT reaches
itstarget. Ifacceleration isdetected, ACNT increments thecounter value oneach timer resolution (fast
mode). Ifdeceleration isdetected, ACNT isstopped. Figure 23-32 illustrates how thecompensations for
acceleration anddeceleration operate.
Figure 23-32. ACNT Period Variation Compensations

<!-- Page 993 -->

HET[2]
ext. ref.
signal
APCNT
period
counter
DCF
Decel
flag
ACNT
angle
generatorSingularity
GPF
Gap flag
Gap End
Gap Start
www.ti.com Angle Functions
993 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.1.1 Singularities
Singularities (gaps, inthiscase, from missing teeth inatoothed wheel) intheexternal reference signal
canbemasked. The start andendofsingularities aredefined bygapstart andgapendvalues specified in
SCNT andACNT. When ACNT reaches gapstart orgapend, itsets/resets thegapflag.
While thegapflagisset,new periods oftheexternal reference signal areignored forangle computation.
SCNT uses thelastperiod measured byAPCNT justbefore gapstart.
Figure 23-33 andFigure 23-34 illustrate thebehavior oftheangle generator during agapafter a
deceleration oracceleration oftheN2HET.
Figure 23-33. N2HET Timings Associated with theGap Flag (ACNT Deceleration)

<!-- Page 994 -->

HET[2]
ext. ref.
signal
APCNT
period
counter
ACF
Accel.
flag
ACNT
angle
generatorSingularity
GPF
Gap flag
Gap EndGap Start
Angle Functions www.ti.com
994 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-34. N2HET Timings Associated with theGap Flag (ACNT Acceleration)
23.3.1.2 APCNT Underflow
The fastest valid external signal APCNT canaccept must satisfy thefollowing condition:
Step Width K<Period Min. Resolution (LRP)
This condition fixes themaximum possible step width once theminimum period andtheresolution ofan
application arespecified.
Ifaperiod value accidentally falls below theminimum allowed, APCNT stops thecapture ofthese periods
andsets theAPCNT underflow interrupt flaglocated intheexceptions interrupt control register. Insuch a
situation, SCNT andACNT continue tobeexecuted using thelastvalid period captured byAPCNT.
23.3.1.3 APCNT Overflow
The slowest valid external signal APCNT canmeasure must satisfy thefollowing condition:
Period Max Resolution <33554431
When thislimit isreached (APCNT Count equals all1's),APCNT stays atamaximum count (stops
counting). APCNT remains inthisposition until thenext specified capture edge isdetected ontheselected
pinandsets theAPCNT overflow interrupt flaglocated intheexceptions interrupt control register. Inthis
situation, SCNT andACNT continue tobeexecuted using themaximum APCNT period count.

<!-- Page 995 -->

20 bit angle value Toothed wheel Speed
PositionHardware angle generator
Toothed
wheel
input
Angular
value
Step width 1/4 - 1/512
www.ti.com Angle Functions
995 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2 Hardware Angle Generator (HWAG)
23.3.2.1 Overview
More engine control functions require powerful microcontrollers toprocess thetiming. These controllers
must generate signals such asdwell time, spark time, andfuelinjection, atprecise engine angles. These
signals must besynchronized with theengine cycle.
The hardware angle generator (HWAG) generates angle value from toothed wheels. Because thetoothed
wheels areinaccurate (the most widely wheel used has60teeth with 6°/tooth), theperiod between two
tooth edges (\)interpolates theangle value andthestep width gives thenumber ofinterpolated angles.
Foranexample oftheangle generator principle, seeFigure 23-35 .
The HWAG cancomplement thehigh-end timer (NHET) togenerate complex angle-angle orangle-time
wave forms.
Towork with themajority oftoothed wheels, theHWAG provides registers toallow theCPU toconfigure
step width, singularity, andfiltering when initializing.
Figure 23-35. Angle Generator Principle
23.3.2.1.1 HWAG Features
The HWAG provides thefollowing features:
*Programmable step width from 1/4to1/512
*Automatic synchronization check after firstsingularity synchronization
*Direct interface with thehigh-end timer
*15to10,000 RPM range
*Programmable toothed-wheel input filter
*Programmable active edge ontoothed-wheel
*Start bitsynchronized tothetooth edge
*Pinselection capability fortoothed-wheel input

<!-- Page 996 -->

RegistersGap Verification
Toothed
Wheel
Peripheral
busIntICLK
Angle increment HET Resolution
To HETTo CPUHWAG
Angle Tick
GenerationNoise Filtering
HET InterfaceHWAG core
42
Angle Functions www.ti.com
996 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.1.2 Block Diagram
Figure 23-36. Hardware Angle Generator Block Diagram

<!-- Page 997 -->

Toothed wheel
Angle Tick
K Ticks
P(n-1) P(n)P(n-1)
K
www.ti.com Angle Functions
997 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2 HWAG Operation
23.3.2.2.1 Angle Tick Generation Algorithm
23.3.2.2.1.1 Angle Tick Generation Principle
The angle tickgenerator isthecore kernel ofthismodule. Ituses thetime-interpolation algorithm to
generate angle ticks based onthelasttoothed wheel period. The angle counter isincremented ateach
new angle tick.
Because thetoothed wheel istooinaccurate tofitwith actual power-train applications, thealgorithm is
based ondividing theprevious tooth period byKangle steps. The tooth period istheperiod between two
active edges, which theHWAG global control register 2(HWAGCR2) defines asthefalling ortherising
edge oftheinput signal. Foranexample oftheangle tickgeneration principle, seeFigure 23-37 .
The speed ofthetoothed wheel varies. This variance inspeed creates some discontinuities intheangle
counter behavior.
When thetoothed wheel accelerates, thecurrent period becomes shorter than theprevious oneandthe
tooth edge arrives before thelasttickhasbeen generated. Tocompensate foranymissed ticks, the
HWAG adds them totheangle counter when theactive edge ofthetooth arrives. The angle value is
updated andresynchronized ateach new active tooth edge.
When thetoothed wheel decelerates, theperiod becomes longer than theprevious period andKticks are
already counted before theactive edge tooth arrives. After thelasttickhasbeen generated, theHWAG
generates atickonly after theactive tooth edge arrives.
Figure 23-37. Angle Tick Generation Principle

<!-- Page 998 -->

Teeth Register
TCNT=
+1Gap Flag
PCNT (n)
+1
PCNT (n-1)P(n) >
2 x P ( n-1 )Criteria
/c163SCNT
/c43/c47/c45
Step RegisterACNT
/c43Angle Tick
Tick CNT
-1"1"
Tickcount<>0
& teeth'eventToothed wheel
Input
Teeth'event
ACNT Inc.
Angle Functions www.ti.com
998 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.1.2 Angle Tick Generation Implementation
The time-interpolation algorithm, which generates ticks based onthetoothed wheel tooth period, consists
ofthefollowing fivemain counters linked together:
*Tooth counter (TCNT): Current tooth
*Period counter (PCNT): Period between twoteeth
*Step counter (SCNT): Angle step
*Tick counter (TCKC): Angle ticks
*Angle counter (ACNT): Angle value
The algorithm also includes differences comparison, adder, andworking registers asshown inFigure 23-
38.
Figure 23-38. New Angle Tick Generation Architecture
The TCNT isan8-bit counter. Itcounts teeth until itreaches theteeth register value then generates agap
flagsignal. The gapflagsignal which changes thebehavior oftheHWAG during thesingularity andresets
theTCNT onthenext active edge ofthetoothed wheel input.
The PCNT calculates theperiod P(n) between twoteeth (two active edges onthetoothed wheel input).
The active edge (falling orrising) isselected bysetting theTED bitintheHWAG global control register 2
(HWAGCR2). Onanactive edge from thetoothed wheel input, thePCNT issaved intheHWAG previous
tooth period value register (HWAPCNT1).
The SCNT counts byKsteps uptotheprevious period value, which iscontained intheHWAPCNT1
register. When theSCNT overflows PCNT(n-1), anangle tickisgenerated andSCNT isreset tothe
remainder between theSCNT andPCNT(n-1). The resulting period oftheSCNT isthefraction PCNT(n-
1)/K.
The TCKC counts every angle tickuntil itreaches Kandthen stops theSCNT. Ifanactive edge occurs
before theTCKC hasreached K,theremainder isadded directly totheACNT.
When encountering anearlier active edge, theACNT accumulates thefractions (angle ticks) generated by
theSCNT andtheremainder oftheTCKC. Foranexample ofangle generation using thetime-based
algorithm, seeFigure 23-39 .

<!-- Page 999 -->

P(n-1)
SCNT
N0 +KN0 +2KN0 +3KN0 +4K
N0N1 +KN1 +2KN1 +3KN1 +4K
N1=N0+nK-P(n-1)Final Count = N0+nK Final Count = N1+mK
N2=N1+mK-P(n-1)
ACNTSCNTPCNTInput pin
K countsP n 1 -/c40 /c41
K- - - - - - - - - - - - - - - - - - - - -P n 1 -/c40 /c41
K- - - - - - - - - - - - - - - - - - - - -P(n-1) P(n)
Angle counterStep counterPeriod counter
www.ti.com Angle Functions
999 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-39. Angle Generation Using Time Based Algorithm
Because ofstepping, thefinal count oftheSCNT willusually beunequal tothetarget value PCNT(n-1)
andthen willoverflow. Tocompensate forthiserror generated bythealgorithm, reset theSCNT tothe
remainder ofthedifference between (SCNT -PCNT(n-1)).
Toseehow theSCNT andPCNT(n-1) generate angle ticks andcompensate fortheerror duetothe
integer fractions, seeFigure 23-40 .
Figure 23-40. SCNT Stepping Compensation

<!-- Page 1000 -->

Toothed Wheel
ACNT ACNTToothed Wheel
Tick CNT Tick CNT 0nn
Acceleration DecelerationStep Width
Angle Functions www.ti.com
1000 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.1.3 Acceleration andDeceleration
Because thetoothed wheel speed isinconstant, itcreates discontinuities intheangle counter behavior.
IftheTCKC reaches zero before anew active tooth edge during adeceleration, theangle ticksignal isno
longer generated bytheSCNT andPCNT(n-1). This halts theACNT until thenew active tooth arrives.
IftheTCKC isunequal tozero when thenew active tooth edge arrives during anacceleration (that is,the
falling edge onthetoothed wheel input intheexample below), therestofthetickcounter increments the
ACNT. Foranexample oftheACNT during acceleration anddeceleration, seeFigure 23-41 .
Figure 23-41. ACNT During Acceleration andDeceleration
23.3.2.2.1.4 End ofCycle
The HWAG behaves differently during thesingularity tooth period ofthetoothed wheel. During the
singularity period, theHWAG counts three virtual teeth (that is,three times thestep width isadded tothe
ACNT) toensure thattheACNT reaches themaximum value (that is,every angle step hasbeen counted)
before resetting it.
During thesingularity period, theHWAG generates angle ticks likeforanormal tooth butwith three times
thevalue. Togenerate these angle ticks, theHWAG uses aconstant period based ontheprevious tooth
period. Because theperiod isbased ontheprevious tooth period, theHWAG must recover from a
deceleration oracceleration ofthree teeth when realizing theactive edge tooth attheendofthesingularity
tooth.
The HWAG must ensure thatthesingularity occurs where expected andmust verify it.When the
singularity tooth arrives, TCNT reaches theteeth register, sets thesignal gapflag, andthen keeps
PCNT(n-1) until thefirsttooth ofthenext round haspassed. Because ofthese conditions, angle ticks
before thesecond tooth willbebased ontheprevious singularity tooth period.
The tickcounter isfirstloaded with anormal value. When thecounter reaches zero, itisreloaded once
with twice thestep width value ifthecriteria flagisnotset.PCNT(n) continues tobeincremented andto
check thecriteria with PCNT(n-1). Formore information ongapverification, seeSection 23.3.2.2.4 .The
SCNT continues togenerate angle ticks until thetickcounter reaches zero thesecond time. The criteria
flagvalidates thetooth inorder toreset thecounters. Foranexample ofhow thecriteria flagvalidates the
tooth toreset thecounters, seeFigure 23-42 .
When thetooth active edge occurs, theACNT isincremented with theremainder value ifthetickcounter
isnotequal tozero. When theACNT contains avalue equals toKtimes theteeth register, thePCNT, the
TCNT andtheACNT arereset tobegin anew revolution.

<!-- Page 1001 -->

Toothed
wheel
Period
counter55 56 57 0
T1 T2 T3 T4T3 > 2 x T2
Gap flag
Criteria flag
Tick counter
P(n-1) T1 T2 T4
ACNT2
43
51
1
2
43
5When TCNT = teeth register, the Gap flag is raised
Tick CNT reloads automatically with 2x the step-width because the Gap flag = '1'
The tick counter is not reloaded because the Criteria flag is raisedIf PCNT ( n ) > 2 x PCNT ( n-1 ) and the Gap flag = '1' then the Criteria flag is raised
The Gap flag and tooth active edge reset, followed by ACNT
www.ti.com Angle Functions
1001 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-42. Singularity Check, ACNT Reset andTiming Associated

<!-- Page 1002 -->

Toothed
wheel
ACNT
counter#0 #0 #0 #1 TCNT #2
Start bit
Angle
Tick2 x Step Width
RUN time Synchronization time
Angle Functions www.ti.com
1002 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.2 Angle Zero Initialization
Before anyangle operation, initialize theHWAG andthen initialize theangle zero asthesingularity tooth.
Toinitialize theangle zero asthesingularity tooth, theHWAG cansend aninterrupt ateach new tooth to
help thesoftware detect thefirsttooth iftheinterrupt isset.This allows youtodecide which algorithm to
apply todetect thezero degree tooth (byenabling thecorresponding interrupt, youcanalso usethewired
criteria).
When researching which algorithm toapply, thecounters ACNT andTCNT arefrozen andmust be
initialized totheir start values. The ACNT value isequal toTtimes thestep value (Tisthetooth where the
start willtake effect andtheinitial value ofthetooth counter). The counters PCNT(n) andPCNT(n-1)
contain thecurrent period andtheprevious period respectively. These counters allow youtoseta
detection criteria. When theapplication software sets thestart bit,thesoftware unfreezes theACNT and
TCNT counters. The counters count from thepreloaded values atthenext tooth active edge. The ACNT is
preloaded with thevalue of2teeth andstarted synchronously with thenext active edge ofthetoothed
wheel. Foranexample oftheHWAG start sequence, seeFigure 23-43 .
Figure 23-43. Example ofHWAG Start Sequence

<!-- Page 1003 -->

www.ti.com Angle Functions
1003 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-44 isanexample ofasingularity research initializing theHWAG atthesecond tooth tostart
synchronously with thethird tooth. The HWAG angle value register (HWAACNT) contains 1024 (2×512)
andtheHWAG current teeth number register (HWATHVL) contains 2.
The code isexecuted inatooth interrupt subroutine incode using thePCNT(n-2) >PCNT (n-3) +PCNT
(n-1) algorithm.
Figure 23-44. Code
23.3.2.2.3 Stopping theHWAG
The HWAG starts synchronously with theactive edge ofthetoothed wheel, butstops when thestart
(STRT) bitintheHWAG global control register 2(HWAGCR2) isreset. Within atooth, theHWAG canbe
stopped andparameters canbechanged (that is,step width, angle counter, andsoon)Ifthishappens,
therestart willtake effect onthenext active tooth edge.
NOTE: When stopping theHWAG, stop theangle increment delivered totheNHET andsetitto
zero. Reload theNHET counter with thesame value oftheangle counter (±corrections), if
restarting theHWAG.

<!-- Page 1004 -->

Toothed
wheel
Period
counter55 56 57 0
T1 T2 T3 T4
T3 > 2 x T2
Angle Functions www.ti.com
1004 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.4 Gap Verification
After theCPU sets thesynchronization andputs theHWAG intoRUN time (that is,thestart bitisset), the
tooth counter counts until reaching theteeth register (the number ofrealteeth ofafullwheel revolution).
When thetooth counter reaches theteeth register, thegapflagsignal isset.Formore information onthe
endofthecycle, seeSection 23.3.2.2.1.4 .When thegapflagsignal isset,itallows theHWAG toverify if
thesingularity isinthecorrect position (last tooth). The module then applies thePCNT(n) >2xPCNT(n-1)
criteria bycomparing PCNT(n) andPCNT(n-1) with onebitleftshifted. Ifthecriteria does notmatch when
thetooth arrives, then theHWAG sends aninterrupt totheCPU anddoes notreset theACNT counter.
The application software must recover from such aninterrupt tokeep theHWAG operating optimally. For
anexample ofgapverification criteria fora60-2 toothed wheel, seeFigure 23-45 .
Figure 23-45. Gap Verification Criteria Fora60-2 Toothed Wheel
Ifthehardware criteria isnotenabled, youmust settheangle reset (ARST) bitintheHWAG global control
register 2(HWAGCR2) tovalidate thesingularity. The HWAGCR2 register must validate thesingularity
before theactive edge ofthesingularity tooth. IftheHWAGCR2 register fails tovalidate thesingularity, the
HWAG generates aninterrupt anddoes notclear theACNT counter when thetooth edge occurs.
NOTE: Fora60-2 toothed wheel, settheARST flagafter thereload ofthetickcounter( when
PCNT(n) =PCNT(n-1)). Byverifying thecriteria, theapplication software cansettheARST
bitafter thispoint.
The CPU canread thePCNT counter andmake acustom criteria settheARST bitontime fortheHWAG.
The application software canusethegapflaginterrupt tofindthesingularity tooth. Alternately, theCPU
canverify thevalidity ofthesingularity inthesecond tooth with amore accurate criteria byusing the
HWAG previous tooth period value register (HWAPCNT1).

<!-- Page 1005 -->

Toothed
wheel55 56 57 0
Gap flag
ARST
Tick counter
ACNT1
www.ti.com Angle Functions
1005 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.4.1 Use oftheARST BitInCase ofaToothed Wheel Without Singularity
Ifatoothed wheel hasnosingularity (that is,nomissing teeth), theACNT must bereset when itreaches
theangle zero point. Toreset theACNT when itreaches theangle zero point, settheARST bitto1.
Setting theARST bitbefore thereload ofthetickcounter willcause theHWAG tofailtoreload thetick
counter. The HWAG willactlikeanormal tooth butthenext active edge onthetoothed wheel input will
reset theACNT andTCNT andclear theARST bit.Foranexample ofusing theARST bitinatoothed
wheel without singularity, seeFigure 23-46 .
Figure 23-46. Using theARST BitinaToothed Wheel Without Singularity

<!-- Page 1006 -->

X% X%Toothed
Input
Filter
Output
glitch during the window glitch after the window
Angle Functions www.ti.com
1006 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.5 Input Noise Filtering
The toothed wheel input comes from ananalog part andissensitive toexternal noise. Due tothis
sensitivity, theinput needs tobefiltered because ofglitches inthesignal.
The HWAG digitally filters thetoothed wheel input signal before itisused inside thecore. The filter blocks
thesignal which negates theeffect inside theHWAG. The HWAG provides twofilter registers thatfilter the
same way.
The filters validate theinput signal after nangle ticks. The nangle ticks arelikeX%ofthetickcounter.
The value oftheremaining percentage ofthetickcounter (1-X%) need tobesetbecause thetickcounter
isadown counter. Calculate thevalue toputintothefilter registers from thestep width value (orangle
ticks value pertooth). The toothed wheel input islikealowpass filter with acut-off frequency that
functions likeatoothed-wheel speed, butwithout acceleration anddecelerations side effects. Foran
example ofawindowing filter foratoothed wheel input onafalling active edge, seeFigure 23-47 .
NOTE: Atanytime, theCPU canmodify thefilter values tofinetune with theapplication.
Figure 23-47. Windowing Filter forToothed Wheel Input onFalling Active Edge
Tocalculate thisnumber:
Step Width ×(1-X%) =Filter Register Value
Ifthestep width value isequal to512andyouwant tofilter 75% ofthetooth, calculate thefilter register as
follows:
512×(1-0.75) =128
When thetickcounter reaches thefilter register value, thetoothed wheel input isunblocked.

<!-- Page 1007 -->

Toothed
wheel55 56 57 0
Filter1
Filter 1
Filter 2X% X% X% X% X%
Y%
www.ti.com Angle Functions
1007 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.2.5.1 Filter During Singularity Tooth
During thesingularity tooth, thefilter acts differently than during anormal tooth. The filter releases the
input foranormal tooth. When thetickcounter isreloaded, asecond filter value isapplied tothetoothed
wheel input. Foranexample offiltering during asingularity tooth, seeFigure 23-48 .
Figure 23-48. Filtering During Singularity Tooth
The second filter value issetusing thesame equation asthefirstfilter with thestep width multiplied by3.
Tocalculate thisnumber:
(3×Step Width) ×(1-Y%) =Second Filter Register Value (30)
Ifthestep width value isequal to512andyouwant 70% ofsingularity tooth period tobefiltered, calculate
thefilter register value asfollows:
3×512×(1-0.70) =460
23.3.2.2.6 HWAG Interrupts
When conditions areset,theHWAG interrupts aregenerated.
When theinterrupt condition istrue, thecorresponding flagissetintheHWAG interrupt flagregister
(HWAFLG). Ifthecorresponding enable bitintheHWAG interrupt enable setregister (HWAENASET) is
also set,aninterrupt request issent totheCPU through oneoftheinterrupt lines, depending onthe
priority oftheinterrupt (HWAG interrupt level setregister (HWALVLSET)).
Because theHWAG cansetinterruptions, theCPU must determine which source created theinterrupt
request andthen execute theinterrupt service routine. The CPU reads theoffset register (HWAOFFx) that
gives thenumber ofthesource. IftheCPU reads theoffset register, itwillautomatically clear thesource
flagthatcreated therequest.
NOTE: Ifthecorresponding enable bitisnotset,aread intheoffset register willnotclear aflag. To
setthebit,write a1inthecorresponding bitwithin theHWAG interrupt flagregister
(HWAFLG).
The HWAG generates eight different interrupts:
*0=Overflow period
*1=Singularity notfound
*2=Tooth interrupt
*3=ACNT overflow
*4=PCNT(n) >2×PCNT (n-1) during normal tooth
*5=Bad active edge tooth
*6=Gap flag
*7=Angle increment overflow

<!-- Page 1008 -->

Interrupt
EnableInterrupt
PriorityHigh
Low
OFFSET BOFFSET AOVRF Period
Sign. Not Found
Tooth Interrupt
ACNT OVRF
Criteria Found
Angle Inc. OVRFPriority
Priority
Bad active edge toothFlag
Gap flag
Angle Functions www.ti.com
1008 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFormore information onthese interrupts, seeTable 23-14 .Each interrupt source isassociated with alow
orhigh priority. When oneormore interrupts with thesame priority occur, afixed priority determines the
offset vector ifthecorresponding enable bitsareset.
The HWAG generates twointerrupt request signals forthecentral interrupt module (CIM). Forinformation
onservicing interrupts, seeFigure 23-49 .Foralistoffset values, seeTable 23-13 .
Table 23-13. HWAG Interrupt Sources andOffset Values
Source Number Offset Value
0 1
1 2
: :
7 8
Figure 23-49. HWAG Interrupt Block Diagram

<!-- Page 1009 -->

www.ti.com Angle Functions
1009 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-14. HWAG Interrupt Descriptions
Interrupt Names Interrupt Descriptions
Overflow period Occurs when thePCNT (n)counter reaches themaximum value. Can occur ifthetoothed
wheel input remains stable. May indicate failure ofanengine stall oratoothed wheel sensor.
Singularity notfound When theTCNT counter sets thegapflag, theHWAG waits forthecriteria flagtoraise
before thetoothed wheel active edge. Ifthetoothed wheel active edge occurs before the
criteria flag, theHWAG raises thesingularity notfound interrupt flag.
New edge tooth This interrupt cansync orletyoucontrol thetickgeneration. This interrupt indicates thenew
active edge tooth. This interrupt could befiltered orunfiltered (BitFILincontrol register).
Angle counter (ACNT) overflow This interrupt occurs when thesingularity isunable tobefound. The angle counter (ACNT)
continues until overflow.
Singularity found during normal tooth This interrupt indicates thattheperiod ofthecurrent tooth isatleast twotimes longer than
theprevious onewhen theHWAG expects anormal tooth. This interrupt candetect the
singularity without bitmanipulation bytheCPU.
Bad active edge tooth This interrupt indicates thatanactive edge hasoccurred before theendofthefiltering
(toothed wheel input blocked) buttheHWAG remains inactive internally. This interrupt can
detect glitches onthetoothed wheel input.
Gap flag When TCNT reaches theteeth register andtheHWAG raises thegapflag,This interrupt is
setwhen thegapflagisraised bytheHWAG,
Angle increment overflow This interrupt indicates thatthenumber oftheangle increment ismore than 15since thelast
resolution tick. This interrupt canprevent anydiscrepancies between theNHET andthe
HWAG.
NOTE: Before enabling anyinterruption, clear theHWAG interrupt flagregister (HWAFLG) toensure
thatanyinterrupts have finished. Ifinterrupts arepending, theHWAG could generate an
interrupt based onanunrealistic event.
23.3.2.3 Emulation
Because theHWAG isdesigned tosynchronize with areal-time environment, theHWAG counters
continue during emulation.
When theCPU isfrozen, theHWAG continues torunandupdate registers. Only theoffset registers
remain uncleared when entering debug mode.
During debug mode, interrupts canoccur andwillwait until theCPU enters runmode again. Ifinterrupts
occur, they could affect synchronization with thetoothed wheel

<!-- Page 1010 -->

10 11 12
1 1 0 1Angle
count
HET
res.
Angle
increment
Resolution Angle increment
HETHWAG
HET InterfaceHWAG coreToothed wheel B
U
S
I
/
FTo CPU
Angle Functions www.ti.com
1010 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.4 Hardware Angle Generator andHigh-End Timer
Intheengine management application, theHWAG isusually connected tooneormore high-end timers.
This connection allows youtoperform angle compare andangle/time compare. Foranexample ofthe
hardware angle generator/high-end timer interface, seeFigure 23-50 .
Figure 23-50. Hardware Angle Generator/High End Timer Interface
23.3.2.4.1 Signal Description
Toperform aresynchronization, theHWAG interface provides totheNHET atevery resolution clock an
angle increment value thatrepresents how much theangle counter oftheHWAG hasbeen incremented
since thelastNHET resolution clock. Foranexample oftheangle count within theHWAG, seeFigure 23-
51.
Figure 23-51. Angle Count Within theHWAG atResolution Clock

<!-- Page 1011 -->

3
94
13 6HET
Res.
Angle
increment
HET
counter
10Compare
value
When the HET counter passes from 9 to 13, the equality compare can not
match the compare value 10. Consequently, the angle position is missed!
9 10 11 12 13 14 15 16
3
94
13Angle
counter
HET
Res.
Angle
increment
HET
counter6
CNT position in the loop
www.ti.com Angle Functions
1011 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleWhen theengine speed increases, theangle count canincrement bymore than oneinaNHET resolution
buttheHWAG willcontinue toprovide theangle increment value atevery resolution..
The NHET canthen implement itsown angle counter (using aCNT instruction inangle mode) which will
beincremented once perresolution bythevalue given bytheangle increment. Foranexample ofan
angle count within theNHET with increments, seeFigure 23-52 .
Figure 23-52. Angle Count Within theNHET With Increments
23.3.2.4.2 NHET Operation onAngle Functions (ACMP, CNT)
23.3.2.4.2.1 State oftheArt
Because theangle value canbeincreased bymore than one, thecompare value could bein-between the
oldangle value andthenew angle value oftheNHET angle counter (where new angle value =oldangle
value +angle increment). Toperform anangle compare thatensures nottomiss acompare value, the
NHET provides theACMP instruction. Foranexample ofacompare without ACMP instruction, see
Figure 23-53 .
Figure 23-53. Compare Without ACMP Instruction

<!-- Page 1012 -->

HET
Res.
Angle
increment
HET
counter
10Compare
value
Associ-
ated Pin
With the ACMP instruction, the compare that is performed will be: 9 10 13 /c163 /c604
133
16 9
CNT ACMP
Angle Functions www.ti.com
1012 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.4.2.2 ACMP Instruction Advantage
The ACMP instruction ismore than anequality compare. ACMP instruction performs anin-between
comparison (oldangle value <compare value≤new angle value) tomatch theposition ofthetoothed
wheel. This instruction, where anequality compare executes every resolution, may miss acompare
match. Foranexample ofACMP compare within theNHET, seeFigure 23-54 .
Figure 23-54. Example ofACMP Compare Within theNHET
With theACMP instruction, thecompare is:9<10≤13
NOTE: Toavoid multiple matches, theACMP only matches during asingle resolution.
Performing thefollowing equations atthesame time implements thiscompare:
CMP >NHET angle counter -Angle increment
CMP≤NHET angle counter

<!-- Page 1013 -->

Angle Tick
+ACNT Inc.
11 bitsHET Res.
Angle4 bitscounterAngle Increment
increment [3:0]register
www.ti.com Angle Functions
1013 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.4.3 NHET Interface
23.3.2.4.3.1 Input Signal Selection
The input pinofthetoothed-wheel signal issoftware selectable. Inprevious generations ofNHET/HWAG,
thiswas fixed toHET[2]. Onthisdevice, theinput pinisprogrammable toprovide more flexibility forthe
system implementation. However, theimplementation isdone inaway tobebackward compatible.
Aseparate register, HWAG pinselect register (HWAPINSEL), isimplemented toallow thisselection
functionality. The HWAPINSEL register should beprogrammed before theHWAG isturned on.The
default selection willbeHET[2] (PINSEL =2h)after reset. The signals willbederived from theinput buffer
ofeach pin.This willallow configuring thepinasanoutput andmeasure back theoutput signal with the
HWAG.
You canchange theHWAPINSEL register atanytime, buttheproper functionality oftheHWAG isnot
assured iftheselection ischanged while theHWAG isalready operational. Itisrecommended thatthe
input selection isdone before theSTRT bitintheHWAG global control register 2(HWAGCR2)) is
programmed to1.
23.3.2.4.3.2 HWAG toNHET Interface
The NHET interface isa11-bit counter sampled bytheNHET andreset bytheNHET resolution. The
counter contains thevalue ofACNT incremented during thelastresolution (see Section 23.3.2.4.1 ).For
theNHET interface block diagram, seeFigure 23-55 .
Figure 23-55. NHET Interface Block Diagram
When theACNT register isreset tozero, theangle increment register isnotreset. The NHET software
checks ifitsown angle register ishigher than 360°andeither clears itorcontinues to720°.IfACNT is
reset within theHWAG, theangle increment register gives theNHET thenumber ofangle ticks from the
lastresolution.
During astrong acceleration after atooth active edge, thenumber ofangle ticks canexceed 15.Ifthe
number ofticks exceeds 15,theHWAG delivers totheNHET several angle increments at15.This allow
theNHET tofollow without missing anyangle positions from theHWAG. When thecounter isbelow 15,
theangle increment reflects thecounter. When theangle increment overflows, sets to15,andifthe
enable bit(bit7inthecontrol register) isset,theHWAG cansend aninterrupt totheCPU.
During astrong deceleration, theangle increment canstay nullforoneormore NHET resolution clocks.
Tominimize theerror between thefly-wheel andNHET angle counter, thestep width andtheNHET
resolution must besettoavoid anyoverflow ofthe11-bit counter oftheNHET interface. This canhappen
ifthenumber ofangle ticks always exceeds 15during oneresolution.

<!-- Page 1014 -->

60RPMTeethNumber ToothPeriod u
Angle Functions www.ti.com
1014 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.5 Range ofOperations
23.3.2.5.1 Intrinsic HWAG Limitation
The following factors limit theHWAG:
*SYSCLK
*PCNT counter (overflow)
*Number ofteeth
*Angle step
These factors willinfluence theengine speed range (RPM limitation) andthemaximum accuracy ofthe
angle steps (wheel limitation).
*RPM limitation
The toothed wheel speed islimited bytheperiod counter (PCNT) andtheangle step foragiven
SYSCLK.
RPM minimum isrelated toPCNT overflow andSYSCLK.
Maximum PCNT value ×SYSCLK =Maximum tooth period
PCNT isa24-bit counter based onSYSCLK.
RPM maximum isrelated totheangle step andSYSCLK.
Minimum tooth period >Step Width ×SYSCLK
The angle ticks period could notbeinferior totheSYSCLK.
Example: The toothed wheel isa60-2, SYSCLK is50Mhz (20ns),andstep width is512:
RPM minimum ≥16777215×20ns=335.5443 ms≥~3RPM
RPM maximum ≥512×20ns=10.24 µs≥97656RPM
NOTE: With a60-2 toothed wheel, thetooth period isthereverse oftheRPM number.
*Wheel Limitation
The HWAG islimited bythenumber ofteeth andtheincrements inarevolution.
The maximum number ofteeth is256. This limits thenumber ofincrements perrevolution to512steps
×256teeth =131072angle increments.

<!-- Page 1015 -->

15 100MaxHETresolution 2.93 s512u  P
15 MinToothPeriodMaxHETresolutionStepWidthu 
MaxHETresolution StepWidth15MinimumToothPeriodu
StepWidthAngularSpeedMinimumToothPeriod 
www.ti.com Angle Functions
1015 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.5.2 HWAG-NHET Limitation
The maximum angle accuracy isafunction oftheangle step andtheNHET loop resolution.
The increment perresolution limits theinterface between theHWAG andtheNHET. The maximum angle
increment perNHET resolution is15increments/NHET_res, which isanangular speed. Iftheangle
increment overflows 15during aconstant speed, thesystem isdiverging.
IntheHWAG, theangular speed isgiven bytherelation:
Toensure thatthevalues arecorrect, they must satisfy thefollowing equation:
Then,
Example: Fora60-2 at10000 RPM, thetooth period is100µsandthestep width is512:

<!-- Page 1016 -->

Angle Functions www.ti.com
1016 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.3.2.6 Tricks
23.3.2.6.1 Using HWAG Previous Tooth Period Value Register (HWAPCNT1)
The HWAG previous tooth period value register (HWAPCNT1) cancompensate forerrors because of
acceleration ordeceleration.
Ifthere isavariation ofthetoothed wheel, theACNT register willhave adiscontinuity .Foranexplanation
ofacceleration anddeceleration, seeSection 23.3.2.2.1.3 .Avoid thisdiscontinuity bygiving the
HWAPCNT1 register asmaller orlarger value, depending ofthevariation. When HWAPCNT1 ismodified,
theangle tickperiod isalso bemodified which causes faster orslower tickgeneration anddecreases the
discontinuity onthenext falling edge.
Because ofthiscompensation, theNHET interface willnotoverflow andfewer errors willoccur onthe
NHET angle counter incase ofstrong acceleration.
NOTE: Reading theangle increment willgive theapplication theamount oftheacceleration.
However, adding thevalue directly totheNHET counter willresult inadiscontinuity inthe
compare sequence. Particularly angle based compare could bemissed.
23.3.2.6.2 Using theSingularity During Normal Tooth Interrupt
This interrupt detects iftheHWAG isdesynchronized with thetoothed wheel andresynchronizes the
HWAG.
Because thecriteria was setduring atooth other than thesingularity tooth, theinterrupt occurs. Because
thecriteria isbased onPCNT >2×PCNT (n-1), thisinterrupt islikely duetothesingularity.
The following steps explain how toresynchronize theHWAG with thisinterrupt:
1.Stop theHWAG
2.Reset ACNT
3.Reset tooth counter
4.Reset interrupt
5.Setstart bit.
The HWAG willrestart onthetooth zero.

<!-- Page 1017 -->

www.ti.com N2HET Control Registers
1017 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4 N2HET Control Registers
Table 23-15 summarizes alltheN2HET registers. The base address forthecontrol registers is
FFF7 B800h forN2HET1 andFFF7 B900h forN2HET2.
Table 23-15. N2HET Registers
Offset Acronym Register Description Section
00h HETGCR Global Configuration Register Section 23.4.1
04h HETPFR Prescale Factor Register Section 23.4.2
08h HETADDR NHET Current Address Register Section 23.4.3
0Ch HETOFF1 Offset Index Priority Level 1Register Section 23.4.4
10h HETOFF2 Offset Index Priority Level 2Register Section 23.4.5
14h HETINTENAS Interrupt Enable SetRegister Section 23.4.6
18h HETINTENAC Interrupt Enable Clear Register Section 23.4.7
1Ch HETEXC1 Exception Control Register 1 Section 23.4.8
20h HETEXC2 Exception Control Register 2 Section 23.4.9
24h HETPRY Interrupt Priority Register Section 23.4.10
28h HETFLG Interrupt Flag Register Section 23.4.11
2Ch HETAND AND Share Control Register Section 23.4.12
34h HETHRSH HRShare Control Register Section 23.4.13
38h HETXOR HRXOR-Share Control Register Section 23.4.14
3Ch HETREQENS Request Enable SetRegister Section 23.4.15
40h HETREQENC Request Enable Clear Register Section 23.4.16
44h HETREQDS Request Destination Select Register Section 23.4.17
4Ch HETDIR NHET Direction Register Section 23.4.18
50h HETDIN NHET Data Input Register Section 23.4.19
54h HETDOUT NHET Data Output Register Section 23.4.20
58h HETDSET NHET Data SetRegister Section 23.4.21
5Ch HETDCLR NHET Data Clear Register Section 23.4.22
60h HETPDR NHET Open Drain Register Section 23.4.23
64h HETPULDIS NHET PullDisable Register Section 23.4.24
68h HETPSL NHET PullSelect Register Section 23.4.25
74h HETPCR Parity Control Register Section 23.4.26
78h HETPAR Parity Address Register Section 23.4.27
7Ch HETPPR Parity PinRegister Section 23.4.28
80h HETSFPRLD Suppression Filter Preload Register Section 23.4.29
84h HETSFENA Suppression Filter Enable Register Section 23.4.30
8Ch HETLBPSEL Loop Back Pair Select Register Section 23.4.31
90h HETLBPDIR Loop Back Pair Direction Register Section 23.4.32
94h HETPINDIS NHET PinDisable Register Section 23.4.33

<!-- Page 1018 -->

N2HET Control Registers www.ti.com
1018 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.1 Global Configuration Register (HETGCR)
N2HET1: offset =FFF7 B800h; N2HET2: offset =FFF7 B900h
Figure 23-56. Global Configuration Register (HETGCR) [offset =00h]
31 25 24
Reserved HET_PIN_ENA
R-0 R/W-1
23 22 21 20 19 18 17 16
Reserved MP Reserved PPF IS CMS
R-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
15 1 0
Reserved TO
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-16. Global Configuration Register (HETGCR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 HET_PIN_ENA Enables theoutput buffers ofthepinstructures depending onthevalue ofnDIS andDIR.x when
PINDIS.x isset.
Note: This bitwillautomatically getcleared when nDIS pin(input port) value is0.
0 Noaffect onthepinoutput buffer structure.
1 Enables thepinoutput buffer structure when DIR =output, PINDIS.x issetandnDIS =1.
23 Reserved 0 Reads return 0.Writes have noeffect.
22-21 MP Master Priority
The NHET canprioritize master accesses toN2HET RAM between theHET Transfer Unit and
another arbiter, which outputs theaccess ofoneoftheremaining masters. The MPbitsallow the
following selections:
0 The HTU haslower priority toaccess theN2HET RAM than thearbiter output.
1h The HTU hashigher priority toaccess theN2HET RAM than thearbiter output.
2h The HTU andthearbiter output usearound robin scheme toaccess theN2HET RAM.
3h Reserved
20-19 Reserved 0 Reads return 0.Writes have noeffect.
18 PPF Protect Program Fields
The PPF bittogether with theTurn On/Off bit(TO) allows toprotect theprogram fields ofall
instructions inN2HET RAM.
When TO=0:
0 Allmasters canread andwrite theprogram fields.
1 Allmasters canread andwrite theprogram fields.
When TO=1:
0 Allmasters canread andwrite theprogram fields.
1 The program fields arereadable butnotwritable forallmasters, which could access theN2HET
RAM. Possible masters aretheCPU, HTU, DMA andasecondary CPU (ifavailable). Writes
initiated bythese masters arediscarded.
17 IS Ignore Suspend
When Ignore Suspend =0,thetimer operation isstopped onsuspend (the current timer instruction
iscompleted). Timer RAM canbefreely accessed during suspend. When setto1,thesuspend is
ignored andtheN2HET continues operating.
0 N2HET stops when insuspend mode.
1 N2HET ignores suspend mode andcontinues operation.

<!-- Page 1019 -->

www.ti.com N2HET Control Registers
1019 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-16. Global Configuration Register (HETGCR) Field Descriptions (continued)
Bit Field Value Description
16 CMS Clk_master/slave
This bitisused tosynchronize multi-N2HETs. Ifset(N2HET ismaster), theN2HET outputs asignal
tosynchronize theprescalers oftheslave N2HET. Bydefault, thisbitisreset, which means aslave
configuration.
Note: This bitmust besettoone(1)forsingle-N2HET configuration.
0 N2HET isconfigured asaslave.
1 N2HET isconfigured asamaster.
15-1 Reserved 0 Reads return 0.Writes have noeffect.
0 TO Turn On/Off
TOdoes notaffect thestate ofthepins. You must set/reset thetimer pins when they areturned off,
orre-initialize thetimer RAM andcontrol registers before areset. After adevice reset, thetimer is
turned offbydefault.
0 N2HET isOFF. The timer program stops executing. Turn-off isautomatically delayed until the
current timer program loop iscompleted. Turn-off does notaffect thecontent ofthetimer RAM, ALU
registers, orcontrol registers. Turn-off resets allflags.
1 N2HET isON. The timer program execution starts synchronously totheLoop clock. Incase of
multiple N2HETs configuration, theslave N2HETs arewaiting fortheloop clock tocome from the
master before starting execution. Then, thetimer address points automatically address 00h
(corresponding toprogram start).

<!-- Page 1020 -->

N2HET Control Registers www.ti.com
1020 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.2 Prescale Factor Register (HETPFR)
N2HET1: offset =FFF7 B804h; N2HET2: offset =FFF7 B904h
Figure 23-57. Prescale Factor Register (HETPFR)
31 17 16
Reserved
R-0
15 11 10 8 7 6 5 0
Reserved LRPFC Reserved HRPFC
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 23-17. Prescale Factor Register (HETPFR) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 LRPFC Loop-Resolution Pre-scale Factor Code. LRPFC determines theloop-resolution prescale divide
rate (lr).
0 /1
1h /2
2h /4
3h /8
4h /16
5h /32
6h /64
7h /128
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 HRPFC High-Resolution Pre-scale Factor Code. HRPFC determines thehigh-resolution prescale divide
rate (hr).
0 /1
1h /2
2h /3
3h /4
: :
3Dh /62
3Eh /63
3Fh /64

<!-- Page 1021 -->

www.ti.com N2HET Control Registers
1021 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.3 N2HET Current Address Register (HETADDR)
N2HET1: offset =FFF7 B808h; N2HET2: offset =FFF7 B908h
Figure 23-58. N2HET Current Address (HETADDR)
31 16
Reserved
R-0
15 9 8 0
Reserved HETADDR
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-18. N2HET Current Address (HETADDR) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 HETADDR N2HET Current Address
Read: Returns thecurrent N2HET program address.
Write: Writes have noeffect.
23.4.4 Offset Index Priority Level 1Register (HETOFF1)
N2HET1: offset =FFF7 B80Ch; N2HET2: offset =FFF7 B90Ch
Figure 23-59. Offset Index Priority Level 1Register (HETOFF1)
31 16
Reserved
R-0
15 6 5 0
Reserved OFFSET1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-19. Offset Index Priority Level 1Register (HETOFF1) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 OFFSET1 OFFSET1 indexes thecurrently pending high-priority interrupt. Offset values andsources arelisted in
Table 23-20 .
Read: Read ofthese bitsdetermines thepending N2HET interrupt.
Write: Writes have noeffect.
Note: Inanyread operation mode, thecorresponding flag(intheHETFLG) isalso cleared. InEmulation
mode thecorresponding flagisnotcleared.

<!-- Page 1022 -->

N2HET Control Registers www.ti.com
1022 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-20. Interrupt Offset Encoding Format
Offset Value Source No.
0 Nointerrupt
1 Instruction 0,32,64...
2 Instruction 1,33,65...
: :
32 Instruction 31,63,95...
33 Program Overflow
34 APCNT Underflow
35 APCNT Overflow
23.4.5 Offset Index Priority Level 2Register (HETOFF2)
N2HET1: offset =FFF7 B810h; N2HET2: offset =FFF7 B910h
Figure 23-60. Offset Index Priority Level 2Register (HETOFF2)
31 16
Reserved
R-0
15 6 5 0
Reserved OFFSET2
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-21. Offset Index Priority Level 2Register (HETOFF2) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 OFFSET2 OFFSET2 indexes thecurrently pending low-priority interrupt. Offset values andsources arelisted in
Table 23-20 .
Read: Read ofthese bitsdetermines thepending N2HET interrupt.
Write: Writes have noeffect.
Note: Inanyread operation mode, thecorresponding flag(intheHETFLG) isalso cleared. InEmulation
mode, thecorresponding flagisnotcleared.

<!-- Page 1023 -->

www.ti.com N2HET Control Registers
1023 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.6 Interrupt Enable SetRegister (HETINTENAS)
N2HET1: offset =FFF7 B814h; N2HET2: offset =FFF7 B914h
Figure 23-61. Interrupt Enable SetRegister (HETINTENAS)
31 16
HETINTENAS
R/W-0
15 0
HETINTENAS
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 23-22. Interrupt Enable SetRegister (HETINTENAS) Field Descriptions
Bit Field Value Description
31-0 HETINTENAS[n] Interrupt Enable Setbits. HETINTENAS isreadable andwritable inanyoperation mode.
Writing a1tobitxenables theinterrupts oftheN2HET instructions atN2HET addresses x+0,
x+32, x+64, andsoon.Generating aninterrupt requires tosetbitxinHETINTENAS andtoenable
theinterrupt bitinoneoftheinstructions ataddresses x+0, x+32, x+64, andsoon.Toavoid
ambiguity, only oneoftheinstructions x+0, x+32, x+64, andsoon,should have theinterrupt enable
bit(inside theinstruction) set.Writing a0toHETINTENAS hasnoeffect.
When reading from HETINTENAS bitxgives theinformation, ifN2HET instructions x+0, x+32,
x+64, andsoon,have theinterrupt enabled ordisabled.
0 Read: Interrupt isdisabled.
Write: Writes have noeffect.
1 Read: Interrupt isenabled.
Write: Interrupt isenabled.
23.4.7 Interrupt Enable Clear Register (HETINTENAC)
N2HET1: offset =FFF7 B818h; N2HET2: offset =FFF7 B918h
Figure 23-62. Interrupt Enable Clear (HETINTENAC)
31 16
HETINTENAC
R/W-0
15 0
HETINTENAC
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-23. NHET Interrupt Enable Clear (HETINTENAC) Field Descriptions
Bit Field Value Description
31-0 HETINTENAC[n] Interrupt Enable Clear bits. HETINTENAC isreadable andwritable inanyoperation mode.
Writing a1tobitxdisables theinterrupts oftheN2HET instructions atN2HET addresses x+0,
x+32, x+64, andsoon.(See also description inTable 23-22 ).Writing a0toHETINTENAC hasno
effect.
When reading from HETINTENAC bitxgives theinformation, ifN2HET instructions x+0, x+32,
x+64, andsoon,have theinterrupt enabled ordisabled.
0 Read: Interrupt isdisabled.
Write: Writes have noeffect.
1 Read: Interrupt isenabled.
Write: Interrupt isdisabled.

<!-- Page 1024 -->

N2HET Control Registers www.ti.com
1024 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.8 Exception Control Register 1(HETEXC1)
N2HET1: offset =FFF7 B81Ch; N2HET2: offset =FFF7 B91Ch
Figure 23-63. Exception Control Register (HETEXC1)
31 25 24
Reserved APCNT_OVRFL_
ENA
R-0 R/W-0
23 17 16
Reserved APCNT_UNRFL_
ENA
R-0 R/W-0
15 9 8
Reserved PRGM_OVRFL_
ENA
R-0 R/W-0
7 3 2 1 0
Reserved APCNT_OVRFL_
PRYAPCNT_UNRFL_
PRYPRGM_OVRFL_
PRY
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-24. Exception Control Register 1(HETEXC1) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
24 APCNT_OVRFL_ENA APCNT Overflow Enable
0 APCNT overflow exception isnotenabled.
1 Enables theAPCNT overflow exception.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 APCNT_UNRFL_ENA APCNT Underflow Enable
0 APCNT underflow exception isnotenabled.
1 Enables theAPCNT underflow exception.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PRGM_OVRFL_ENA Program Overflow Enable
0 The program overflow exception isnotenabled.
1 Enables theprogram overflow exception.
7-3 Reserved 0 Reads return 0.Writes have noeffect.
2 APCNT_OVRFL_PRY APCNT Overflow Exception Interrupt Priority
0 Exception priority level 2.
1 Exception priority level 1.
1 APCNT_UNRFL_PRY APCNT Underflow Exception Interrupt Priority
0 Exception priority level 2.
1 Exception priority level 1.
0 PRGM_OVRFL_PRY ProgramOverflow Exception Interrupt Priority
0 Exception priority level 2.
1 Exception priority level 1.

<!-- Page 1025 -->

www.ti.com N2HET Control Registers
1025 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.9 Exception Control Register 2(HETEXC2)
N2HET1: offset =FFF7 B820h; N2HET2: offset =FFF7 B920h
Figure 23-64. Exception Control Register 2(HETEXC2)
31 16
Reserved
R-0
15 9 8
Reserved DEBUG_STATUS_
FLAG
R-0 R/WC-0
7 3 2 1 0
Reserved APCNT_OVRFL_
FLAGAPCNT_UNRFL_
FLAGPRGM_OVRFL_
FLAG
R-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 23-25. Exception Control Register 2(HETEXC2) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DEBUG_STATUS_FLAG Debug Status Flag.
This flagissetwhen N2HET hasstopped atabreakpoint. Also generates adebug
request tohalttheARM CPU.
0 Read: N2HET iseither running, orstopped, flagcleared butnotyetrestarted.
Write: Noeffect.
1 Read: N2HET isstopped atabreakpoint.
Write: Clears thebit.Torestart N2HET clear thisbitandthen restart theARM CPU.
The N2HET andARM CPU willstart synchronously.
7-3 Reserved 0 Reads return 0.Writes have noeffect.
2 APCNT_OVRFL_FLAG APCNT Overflow Flag
0 Read: Exception hasnotoccurred since theflagwas cleared.
Write: Noeffect.
1 Read: Exception hasoccurred since theflagwas cleared.
Write: Clears thebit.
1 APCNT_UNDFL_FLAG APCNT Underflow Flag
0 Read: Exception hasnotoccurred since theflagwas cleared.
Write: Noeffect.
1 Read: Exception hasoccurred since theflagwas cleared.
Write: Clears thebit.
0 PRGM_OVERFL_FLAG Program Overflow Flag
0 Read: Exception hasnotoccurred since theflagwas cleared.
Write: Noeffect.
1 Read: Exception hasoccurred since theflagwas cleared
Write: Clears thebit.

<!-- Page 1026 -->

N2HET Control Registers www.ti.com
1026 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.10 Interrupt Priority Register (HETPRY)
N2HET1: offset =FFF7 B824h; N2HET2: offset =FFF7 B924h
Figure 23-65. Interrupt Priority Register (HETPRY)
31 16
HETPRY
R/WP-0
15 0
HETPRY
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 23-26. Interrupt Priority Register (HETPRY) Field Descriptions
Bit Field Value Description
31-0 HETPRY[n] HET Interrupt Priority Level Bits
Used toselect thepriority ofanyofthe32potential interrupt sources coming from N2HET instructions.
0 Interrupt priority level 2(low level).
1 Interrupt priority level 1(high level).
23.4.11 Interrupt Flag Register (HETFLG)
N2HET1: offset =FFF7 B828h; N2HET2: offset =FFF7 B928h
Figure 23-66. Interrupt Flag Register (HETFLG)
31 16
HETFLAG
R/W1C-0
15 0
HETFLAG
R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset; X=Unknown
Table 23-27. Interrupt Flag Register (HETFLG) Field Descriptions
Bit Field Value Description
31-0 HETFLAG[n] Interrupt Flag Register Bits
Bitxissetwhen aninterrupt condition hasoccurred ononeoftheinstructions x+0, x+32, x+64, andso
on.The flagposition x(intheregister) isdecoded from thefiveLSBs oftheinstruction address that
generated theinterrupt. The hardware willsettheflagonly iftheinterrupt enable bit(inthe
corresponding instruction) isset.The flagwillbeseteven ifbitxintheInterrupt Enable SetRegister
(HETINTENAS) isnotenabled. Enabling bitxinHETINTENAS isrequired ifaninterrupt should be
generated.
Clearing theflagcanbedone bywriting aonetotheflag. Alternatively reading thecorresponding Offset
Index Priority Level 1Register (HETOFF1) orOffset Index Priority Level 2Register (HETOFF2) will
automatically clear theflag.
0 Read: NoN2HET instruction with aninterrupt hasbeen reached since theflagwas cleared.
Write: Noeffect.
1 Read: AN2HET instruction with aninterrupt hasbeen reached since theflagwas cleared.
Write: Clears thebit.

<!-- Page 1027 -->

www.ti.com N2HET Control Registers
1027 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.12 AND Share Control Register (HETAND)
N2HET1: offset =FFF7 B82Ch; N2HET2: offset =FFF7 B92Ch
Figure 23-67. AND Share Control Register (HETAND)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
AND
SHARE31/30AND
SHARE29/28AND
SHARE27/26AND
SHARE25/24AND
SHARE23/22AND
SHARE21/20AND
SHARE19/18AND
SHARE17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
AND
SHARE15/14AND
SHARE13/12AND
SHARE11/10AND
SHARE9/8AND
SHARE7/6AND
SHARE5/4AND
SHARE3/2AND
SHARE1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-28. AND Share Control Register (HETAND) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 ANDSHARE
n+1/nAND Share Enable
Enable theAND sharing ofthesame pinfortwoHRstructures. Forexample, ifbitANDSHARE1/0
isset,thepinHET[0] willthen becommanded byalogical AND ofboth HRstructures 0and1.
Note: IfHRAND SHARE bitsareused, pins notconnected toHRstructures (the oddnumber pinin
each pair) canbeaccessed asgeneral inputs/outputs.
0 HROutput ofHET[n+1] andHET[n] arenotAND shared.
1 HROutput ofHET[n+1] andHET[n] areAND shared onto pinHET[n].

<!-- Page 1028 -->

N2HET Control Registers www.ti.com
1028 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.13 HRShare Control Register (HETHRSH)
N2HET1: offset =FFF7 B834h; N2HET2: offset =FFF7 B934h
Figure 23-68. HRShare Control Register (HETHRSH)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
HR
SHARE31/30HR
SHARE29/28HR
SHARE27/26HR
SHARE25/24HR
SHARE23/22HR
SHARE21/20HR
SHARE19/18HR
SHARE17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
HR
SHARE15/14HR
SHARE13/12HR
SHARE11/10HR
SHARE9/8HR
SHARE7/6HR
SHARE5/4HR
SHARE3/2HR
SHARE1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-29. HRShare Control Register (HETHRSH) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 HRSHARE
n+1/nHRShare Bits
Enables theshare ofthesame pinfortwoHRstructures. Forexample, ifbitHRSHARE1/0 isset,
thepinHET[0] willthen beconnected toboth HRinput structures 0and1.
Note: IfHRshare bitsareused, pins notconnected toHRstructures (the oddnumber pinineach
pair) canbeaccessed asgeneral inputs/outputs.
0 HRInput ofHET[n+1] andHET[n] arenotshared.
1 HRInput ofHET[n+1] andHET[n] areshared; both measure pinHET[n].

<!-- Page 1029 -->

www.ti.com N2HET Control Registers
1029 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.14 XOR Share Control Register (HETXOR)
N2HET1: offset =FFF7 B838h; N2HET2: offset =FFF7 B938h
Figure 23-69. XOR Share Control Register (HETXOR)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
XOR
SHARE31/30XOR
SHARE29/28XOR
SHARE27/26XOR
SHARE25/24XOR
SHARE23/22XOR
SHARE21/20XOR
SHARE19/18XOR
SHARE17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
XOR
SHARE15/14XOR
SHARE13/12XOR
SHARE11/10XOR
SHARE9/8XOR
SHARE7/6XOR
SHARE5/4XOR
SHARE3/2XOR
SHARE1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-30. XOR Share Control Register (HETXOR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 XORSHARE
n+1/nXOR Share Enable
Enable theXOR-share ofthesame pinfortwooutput HRstructures. Forexample, ifbit
XORSHARE1/0 isset,thepinHET[0] willthen becommanded byalogical XOR ofboth HR
structures 0and1.
Note: IfXOR share bitsareused, pins notconnected toHRstructures (the oddnumber pinineach
pair) canbeaccessed asgeneral inputs/outputs.
0 HROutput ofHET[n+1] andHET[n] arenotXOR shared.
1 HROutput ofHET[n+1] andHET[n] areXOR shared onto pinHET[n].

<!-- Page 1030 -->

N2HET Control Registers www.ti.com
1030 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.15 Request Enable SetRegister (HETREQENS)
N2HET1: offset =FFF7 B83Ch; N2HET2: offset =FFF7 B93Ch
Figure 23-70. Request Enable SetRegister (HETREQENS)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
REQENA7 REQENA6 REQENA5 REQENA4 REQENA3 REQENA2 REQENA1 REQENA0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-31. Request Enable SetRegister (HETREQENS) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 REQENAn Request Enable Bits
0 Read: Returns theinformation thatrequest linenisdisabled.
Write: Writing a0hasnoeffect.
1 Read: Returns theinformation thatrequest linenisenabled.
Write: Writing a1tobitnenables theN2HET request linen.
Note: The request linecantrigger aDMA control packet (DMA channel), anHTU double control
packet (DCP) orboth simultaneously. The HETREQDS register determines towhich module(s) the
N2HET request linenisassigned.
Note: Adisabled request linedoes notmemorize oldrequests. Sothere arenopending requests to
service after enabling request linen.
23.4.16 Request Enable Clear Register (HETREQENC)
N2HET1: offset =FFF7 B840h; N2HET2: offset =FFF7 B940h
Figure 23-71. Request Enable Clear Register (HETREQENC)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
REQDIS7 REQDIS6 REQDIS5 REQDIS4 REQDIS3 REQDIS2 REQDIS1 REQDIS0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-32. Request Enable Clear Register (HETREQENC) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 REQDISn Request Disable Bits
0 Read: Returns theinformation thatrequest linenisdisabled.
Write: Writing a0hasnoeffect.
1 Read: Returns theinformation thatrequest linenisenabled.
Write: Writing a1tobitndisables theN2HET request linen.

<!-- Page 1031 -->

www.ti.com N2HET Control Registers
1031 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.17 Request Destination Select Register (HETREQDS)
N2HET1: offset =FFF7 B844h; N2HET2: offset =FFF7 B944h
Figure 23-72. Request Destination Select Register (HETREQDS) [offset =FFF7 B844h]
31 24 23 22 21 20 19 18 17 16
Reserved TDBS7 TDBS6 TDBS5 TDBS4 TDBS3 TDBS2 TDBS1 TDBS0
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
15 8 7 6 5 4 3 2 1 0
Reserved TDS7 TDS6 TDS5 TDS4 TDS3 TDS2 TDS1 TDS0
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-33. Request Destination Select Register (HETREQDS) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 TDBSn HTU, DMA orBoth Select Bits
0 N2HET request linenisassigned tothemodule specified byTDS bitn.
1 N2HET request linenisassigned toboth DMA andHTU. TDS bitnisignored inthiscase.
15-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 TDSn HTU orDMA Select Bits
Note: Itmust beensured intheN2HET program, thatonerequest lineistriggered byonly oneN2HET
instruction.
0 N2HET request linenisassigned toHTU (TDBS bitniszero).
1 N2HET request linenisassigned toDMA (TDBS bitniszero).
NOTE: Please refer tothedevice data sheet how each ofthe8N2HET request lines areconnected
tothese modules. See also Section 23.2.9 .

<!-- Page 1032 -->

N2HET Control Registers www.ti.com
1032 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.18 NHET Direction Register (HETDIR)
N2HET1: offset =FFF7 B84Ch; N2HET2: offset =FFF7 B94Ch
Figure 23-73. N2HET Direction Register (HETDIR)
31 16
HETDIR
R/W-0
15 0
HETDIR
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-34. N2HET Direction Register (HETDIR) Field Descriptions
Bit Field Value Description
31-0 HETDIR[n] Data direction ofNHET pins
0 PinHET[n] isaninput (and itsoutput buffer istristated).
1 PinHET[n] isanoutput.
NOTE: Table 23-9 shows how theregister bitsofDIR, PULDIS andPULSEL areaffecting the
N2HET pins.

<!-- Page 1033 -->

www.ti.com N2HET Control Registers
1033 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.19 N2HET Data Input Register (HETDIN)
N2HET1: offset =FFF7 B850h; N2HET2: offset =FFF7 B950h
Figure 23-74. N2HET Data Input Register (HETDIN)
31 16
HETDIN
R-x
15 0
HETDIN
R-x
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset;
Table 23-35. N2HET Data Input Register (HETDIN) Field Descriptions
Bit Field Value Description
31-0 HETDIN[n] Data input. This bitdisplays thelogic state ofthepin.
0 PinHET[n] isatlogic low(0).
1 PinHET[n] isatlogic high (1).
23.4.20 N2HET Data Output Register (HETDOUT)
N2HET1: offset =FFF7 B854h; N2HET2: offset =FFF7 B954h
Figure 23-75. N2HET Data Output Register (HETDOUT)
31 16
HETDOUT
R/W-0
15 0
HETDOUT
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-36. N2HET Data Output Register (HETDOUT) Field Descriptions
Bit Field Value Description
31-0 HETDOUT[n] Data outwrite. Writes tothisbitwillonly take effect when thepinisconfigured asanoutput. The
current logic state ofthepinwillbedisplayed bythisbiteven when thepinstate ischanged by
writing toHETDSET orHETDCLR.
0 PinHET[n] isatlogic low(0).
1 PinHET[n] isatlogic high (1)iftheHETPDR[n] bit=0ortheoutput isinhigh-impedance state if
theHETPDR[n] bit=1.

<!-- Page 1034 -->

N2HET Control Registers www.ti.com
1034 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.21 NHET Data SetRegister (HETDSET)
N2HET1: offset =FFF7 B858h; N2HET2: offset =FFF7 B958h
Figure 23-76. N2HET Data SetRegister (HETDSET)
31 16
HETDSET
R/WS-0
15 0
HETDSET
R/WS-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset
Table 23-37. N2HET Data SetRegister (HETDSET) Field Descriptions
Bit Field Value Description
31-0 HETDSET[n] This register allows bitsofHETDOUT tobesetwhile avoiding thepitfalls ofaread-modify-write
sequence inamultitasking environment.
Bitswritten asalogic 1setthesame bitintheHETDOUT register; while bitswritten aslogic 0
leave thesame bitinHETDOUT unchanged. Reads from thisaddress return thevalue ofthe
HETDOUT register.
0 Write: HETDOUT[n] isunchanged.
1 Write: HETDOUT[n] isset.
23.4.22 N2HET Data Clear Register (HETDCLR)
N2HET1: offset =FFF7 B85Ch; N2HET2: offset =FFF7 B95Ch
Figure 23-77. N2HET Data Clear Register (HETDCLR)
31 16
HETDCLR
R/WC-0
15 0
HETDCLR
R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 23-38. N2HET Data Clear Register (HETDCLR) Field Descriptions
Bit Field Value Description
31-0 HETDCLR[n] This register allows bitsofHETDOUT tobecleared while avoiding thepitfalls ofaread-modify-write
sequence inamultitasking environment.
Bitswritten asalogic 1clear thesame bitintheHETDOUT register; while bitswritten aslogic 0
leave thesame bitinHETDOUT unchanged. Reads from thisaddress return thevalue ofthe
HETDOUT register.
0 Write: HETDOUT[n] isunchanged.
1 Write: HETDOUT[n] iscleared.

<!-- Page 1035 -->

www.ti.com N2HET Control Registers
1035 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.23 N2HET Open Drain Register (HETPDR)
Values inthisregister enable ordisable theopen drain capability ofthedata pins.
N2HET1: offset =FFF7 B860h; N2HET2: offset =FFF7 B960h
Figure 23-78. N2HET Open Drain Register (HETPDR)
31 16
HETPDR
R/W-0
15 0
HETPDR
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-39. N2HET Open Drain Register (HETPDR) Field Descriptions
Bit Field Value Description
31-0 HETPDR[n] Open drain control forHET[n] pins
0 The pinisconfigured inpush/pull mode.
1 The pinisconfigured inopen drain mode. The HETDOUT register controls thestate oftheoutput
buffer:
HETDOUT[n] =0The output buffer ofpinHET[n] isdriven low.
HETDOUT[n] =1The output buffer ofpinHET[n] istristated.
23.4.24 N2HET Pull Disable Register (HETPULDIS)
Values inthisregister enable ordisable thepull-up/-down functionality ofthepins.
N2HET1: offset =FFF7 B864h; N2HET2: offset =FFF7 B964h
Figure 23-79. N2HET PullDisable Register (HETPULDIS)
31 16
HETPULDIS
R/W-n
15 0
HETPULDIS
R/W-n
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; nisdevice dependent, seedevice specific data sheet
Table 23-40. N2HET PullDisable Register (HETPULDIS) Field Descriptions
Bit Field Value Description
31-0 HETPULDIS[n] Pulldisable forN2HET pins
0 The pullfunctionality isenabled onpinHET[n].
1 The pullfunctionality isdisabled onpinHET[n].
NOTE: See device data sheet forwhich pins provide programmable pullups/pulldowns.
Table 23-9 shows how theregister bitsofHETDIR, HETPULDIS, andHETPSL areaffecting
theN2HET pins.

<!-- Page 1036 -->

N2HET Control Registers www.ti.com
1036 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.25 N2HET Pull Select Register (HETPSL)
Values inthisregister select thepull-up orpull-down functionality ofthepins.
N2HET1: offset =FFF7 B868h; N2HET2: offset =FFF7 B968h
Figure 23-80. N2HET PullSelect Register (HETPSL)
31 16
HETPSL
R/W-0
15 0
HETPSL
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-41. N2HET PullSelect Register (HETPSL) Field Descriptions
Bit Field Value Description
31-0 HETPSL[n] Pullselect forNHET pins
0 The pulldown functionality isenabled ifcorresponding bitinHETPULDIS is0.
1 The pullupfunctionality isenabled ifcorresponding bitinHETPULDIS is0.
NOTE: See device data sheet forwhich pins provide programmable pullups/pulldowns.
Table 23-9 shows how theregister bitsofHETDIR, HETPULDIS andHETPSL areaffecting
theN2HET pins.
The information ofthisregister isalso used todefine thepinstates after aparity error:
After aparity error allN2HET pins, which are
1.Defined asoutput pins intheHETDIR register
2.Notdefined asopen drain pins (with theHETPDR register)
3.Selected with theHETPPR register, willremain outputs, butautomatically
change their levels inthefollowing way:
*IftheHETPSL register specifies 0forthepin,itwillswitch tolowlevel.
*IftheHETPSL register specifies 1forthepin,itwillswitch tohigh level.
This behavior isindependent ofthevalue, which register HETPULDIS specifies forthe
corresponding pin.

<!-- Page 1037 -->

www.ti.com N2HET Control Registers
1037 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.26 Parity Control Register (HETPCR)
N2HET1: offset =FFF7 B874h; N2HET2: offset =FFF7 B974h
Figure 23-81. Parity Control Register (HETPCR)
31 16
Reserved
R-0
15 9 8 7 4 3 0
Reserved TEST Reserved PARITY_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 23-42. Parity Control Register (HETPCR) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 TEST Test Bit.When thisbitisset,theparity bitsaremapped intotheperipheral RAM frame tomake
them accessible bytheCPU.
0 Read: Parity bitsarenotmemory mapped.
Write: Disable mapping.
1 Read: Parity bitsarememory mapped.
Write: Enable mapping.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 PARITY_ENA Enable/disable parity checking. This bitfield enables ordisables theparity check onread
operations andtheparity calculation onwrite operations. Ifparity checking isenabled andaparity
error isdetected theN2HET_UERR signal isactivated.
5h Read: Parity check isdisabled.
Write: Disable checking.
Others Read: Parity check isenabled.
Write: Enable checking.
NOTE: Itisrecommended towrite Ahtoenable error detection, toguard against softerrors flipping
PARITY_ENA toadisable state.

<!-- Page 1038 -->

N2HET Control Registers www.ti.com
1038 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.27 Parity Address Register (HETPAR)
N2HET1: offset =FFF7 B878h; N2HET2: offset =FFF7 B978h
Figure 23-82. Parity Address Register (HETPAR)
31 16
Reserved
R-0
15 13 12 2 1 0
Reserved PAOFF Reserved
R-0 R-X R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset; X=Value unchanged after reset
Table 23-43. Parity Address Register (HETPAR) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reads return 0.Writes have noeffect.
12-2 PAOFF Parity Error Address Offset. This register holds theoffset address ofthefirstparity error, which is
detected inN2HET RAM. This error address isfrozen from being updated until itisread bytheCPU.
During emulation mode, thisaddress isfrozen even when read.
Incase ofaN2HET RAM parity error, PAOFF willcontain theoffset address oftheerroneous 32-bit
N2HET RAM field counted from thebeginning oftheN2HET RAM.
Examples: The 32-bit program field ofinstruction 0willreturn 0,the32-bit control field ofinstruction 0
willreturn 1,...,the32-bit control field ofinstruction 1willreturn 5,andsoon.
Read: Returns theoffset address oftheerroneous 32-bit word inbytes from thebeginning ofthe
N2HET RAM.
Write: Writes have noeffect.
1-0 Reserved 0 Reads return 0.Writes have noeffect.
NOTE: The Parity Error Address Register willnotbereset, neither byPORRST norbyanyother
reset source.

<!-- Page 1039 -->

www.ti.com N2HET Control Registers
1039 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.28 Parity PinRegister (HETPPR)
N2HET1: offset =FFF7 B87Ch; N2HET2: offset =FFF7 B97Ch
Figure 23-83. Parity PinRegister (HETPPR)
31 16
HETPPR
R/W-0
15 0
HETPPR
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-44. Parity PinRegister (HETPPR) Field Descriptions
Bit Field Value Description
31-0 HETPPR[n] NHET Parity PinSelect Bits. Allows HET[n] pins tobeconfigured todrive toaknown state when an
N2HET parity error isdetected.
0 PinHET[n] isnotaffected bythedetection ofanN2HET parity error.
1 PinHET[n] isdriven toaknown state when anN2HET parity error isdetected. The known state isa
function ofbitsHETDIR[n], HETPSL[n], HETPDR[n] asdescribed inTable 23-45 (this state isalso
independent ofHETPULDIS[n]).
Table 23-45. Known State onParity Error
HETDIR[n] HETPDR[n] HETPSL[n] Known State onParity Error
0 x x High Impedance
1 0 0 Drive Logic 0
1 0 1 Drive Logic 1
1 1 x High Impedance

<!-- Page 1040 -->

N2HET Control Registers www.ti.com
1040 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.29 Suppression Filter Preload Register (HETSFPRLD)
N2HET1: offset =FFF7 B880h; N2HET2: offset =FFF7 B980h
Figure 23-84. Suppression Filter Preload Register (HETSFPRLD)
31 18 17 16
Reserved CCDIV
R-0 R/W-0
15 10 9 0
Reserved CPRLD
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-46. Suppression Filter Preload Register (HETSFPRLD) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17-16 CCDIV Counter Clock Divider
CCDIV determines theratio between thecounter clock andVCLK2.
0 CCLK =VCLK2
1h CCLK =VCLK2 /2
2h CCLK =VCLK2 /3
3h CCLK =VCLK2 /4
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 CPRLD Counter Preload Value
CPRLD contains thepreload value forthecounter clock.
23.4.30 Suppression Filter Enable Register (HETSFENA)
N2HET1: offset =FFF7 B884h; N2HET2: offset =FFF7 B984h
Figure 23-85. Suppression Filter Enable Register (HETSFENA)
31 16
HETSFENA
R/W-0
15 0
HETSFENA
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-47. Suppression Filter Enable Register (HETSFENA) Field Descriptions
Bit Field Value Description
31-0 HETSFENA[n] Suppression Filter Enable Bits
Note: Ifthepinisconfigured asanoutput bytheN2HET Direction Register (HETDIR), thefilter is
automatically disabled independent onthebitinHETSFENA.
0 The input noise suppression filter forpinHET[n] isdisabled.
1 The input noise suppression filter forpinHET[n] isenabled.

<!-- Page 1041 -->

www.ti.com N2HET Control Registers
1041 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.31 Loop Back Pair Select Register (HETLBPSEL)
Refer toSection 23.2.5.7 foradescription ofloopback testfunctions.
N2HET1: offset =FFF7 B88Ch; N2HET2: offset =FFF7 B98Ch
Figure 23-86. Loop Back Pair Select Register (HETLBPSEL)
31 30 29 28 27 26 25 24
LBPTYPE31/30 LBPTYPE29/28 LBPTYPE27/26 LBPTYPE25/24 LBPTYPE23/22 LBPTYPE21/20 LBPTYPE19/18 LBPTYPE17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
23 22 21 20 19 18 17 16
LBPTYPE15/14 LBPTYPE13/12 LBPTYPE11/10 LBPTYPE9/8 LBPTYPE7/6 LBPTYPE5/4 LBPTYPE3/2 LBPTYPE1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
15 14 13 12 11 10 9 8
LBPSEL31/30 LBPSEL29/28 LBPSEL27/26 LBPSEL25/24 LBPSEL23/22 LBPSEL21/20 LBPSEL19/18 LBPSEL17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
LBPSEL15/14 LBPSEL13/12 LBPSEL11/10 LBPSEL9/8 LBPSEL7/6 LBPSEL5/4 LBPSEL3/2 LBPSEL1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-48. Loop Back Pair Select Register (HETLBPSEL) Field Descriptions
Bit Field Value Description
31-16 LBPTYPE
n+1/nLoop Back Pair Type Select Bits
These bitsarevalid only when Loopback mode isenabled (HETLBPDIR[19:16] =1010).
0 Digital loopback isselected forHRstructures onpins HET[n+1] andHET[n].
1 Analog loopback isselected forHRstructures onpins HET[n+1] andHET[n].
15-0 LBPSEL
n+1/nLoop Back Pair Select Bits
These bitsarevalid only when Loopback mode isenabled (HETLBPDIR[19:16] =1010).
Ifbitxisset,theHRstructures onpins HET[n+1] andHET[n] areconnected inaloop back mode. The
direction isgiven byLBPDIR n+1/n andtype isselected byLBPTYPE n+1/n.
The pinwhich isnotdriven bytheN2HET pinactions canstillbeused asnormal GIO pin.

<!-- Page 1042 -->

N2HET Control Registers www.ti.com
1042 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.32 Loop Back Pair Direction Register (HETLBPDIR)
Refer toSection 23.2.5.7 foradescription ofloopback testfunctions.
N2HET1: offset =FFF7 B890h; N2HET2: offset =FFF7 B990h
Figure 23-87. Loop Back Pair Direction Register (HETLBPDIR)
31 20 19 16
Reserved LBPTSTENA
R-0 R/WP-5h
15 14 13 12 11 10 9 8
LBPDIR31/30 LBPDIR29/28 LBPDIR27/26 LBPDIR25/24 LBPDIR23/22 LBPDIR21/20 LBPDIR19/18 LBPDIR17/16
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
LBPDIR15/14 LBPDIR13/12 LBPDIR11/10 LBPDIR9/8 LBPDIR7/6 LBPDIR5/4 LBPDIR3/2 LBPDIR1/0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 23-49. Loop Back Pair Direction Register (HETLBPDIR) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 LBPTSTENA Loopback Test Enable Key
5h Loopback Test isdisabled.
Ah Loopback Test isenabled.
Others Loopback Test isdisabled.
15-0 LBPDIR
n+1/nLoop Back Pair Direction Bits
0 The HRstructures onpins HET[n+1] andHET[n] areinternally connected with HET[n] asinput and
HET[n+1] asoutput.
1 The HRstructures onpins HET[n+1] andHET[n] connected with HET[n] asoutput andHET[n+1]
asinput.
NOTE: The loop back direction canbeselected independent ontheHETDIR register setting.

<!-- Page 1043 -->

www.ti.com N2HET Control Registers
1043 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.4.33 N2HET PinDisable Register (HETPINDIS)
N2HET1: offset =FFF7 B894h; N2HET2: offset =FFF7 B994h
Figure 23-88. N2HET PinDisable Register (HETPINDIS)
31 16
HETPINDIS
R/W-0
15 0
HETPINDIS
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-50. NHET PinDisable Register (HETPINDIS) Field Descriptions
Bit Field Value Description
31-0 HETPINDIS[n] N2HET PinDisable Bits
0 Logic low: Noaffect ontheoutput buffer enable ofthepin(iscontrolled bythevalue ofthe
HETDIR[n] bit).
1 Logic high: Output buffer ofthepinisenabled ifpinnDIS =1,HET_PIN_ENA =1,andHETDIR =
1;ordisabled ifnDIS =0,HETDIR =0,orHET_PIN_ENA =0.

<!-- Page 1044 -->

HWAG Registers www.ti.com
1044 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5 HWAG Registers
Table 23-51 lists theHWAG registers.
Table 23-51. HWAG Registers
Offset Acronym Register Description Section
9Ch HWAPINSEL HWAG PinSelect Register Section 23.5.1
A0h HWAGCR0 HWAG Global Control Register 0 Section 23.5.2
A4h HWAGCR1 HWAG Global Control Register 1 Section 23.5.3
A8h HWAGCR2 HWAG Global Control Register 2 Section 23.5.4
ACh HWAENASET HWAG Interrupt Enable SetRegister Section 23.5.5
B0h HWAENACLR HWAG Interrupt Enable Clear Register Section 23.5.6
B4h HWALVLSET HWAG Interrupt Level SetRegister Section 23.5.7
B8h HWALVLCLR HWAG Interrupt Level Clear Register Section 23.5.8
BCh HWAFLG HWAG Interrupt Flag Register Section 23.5.9
C0h HWAOFF0 HWAG Interrupt Offset Register 1 Section 23.5.10
C4h HWAOFF1 HWAG Interrupt Offset Register 2 Section 23.5.11
C8h HWAACNT HWAG Angle Value Register Section 23.5.12
CCh HWAPCNT1 HWAG Previous Tooth Period Value Register Section 23.5.13
D0h HWAPCNT HWAG Current Tooth Period Value Register Section 23.5.14
D4h HWASTWD HWAG Step Width Register Section 23.5.15
D8h HWATHNB HWAG Teeth Number Register Section 23.5.16
DCh HWATHVL HWAG Current Teeth Number Register Section 23.5.17
E0h HWAFIL HWAG Filter Register Section 23.5.18
E8h HWAFIL2 HWAG Filter Register 2 Section 23.5.19
F0h HWAANGI HWAG Angle Increment Register Section 23.5.20

<!-- Page 1045 -->

www.ti.com HWAG Registers
1045 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.1 HWAG PinSelect Register (HWAPINSEL)
Figure 23-89. HWAG PinSelect Register (HWAPINSEL)
31 16
Reserved
R-0
15 5 4 0
Reserved PINSEL
R-0 R/W-2h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-52. HWAG PinSelect Register (HWAPINSEL) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 PINSEL HWAG PinSelect. Selects from which NHET pininput buffer theHWAG toothed-wheel signal is
derived.
0 Read: PinHET[0] isselected.
Write: Selects pinHET[0].
1h Read: PinHET[1] isselected
Write: Selects pinHET[1].
2h Read: PinHET[2] isselected
Write: Selects pinHET[2]. Default after reset forbackwards compatibility
: :
1Fh Read: PinHET[31] selected
Write: Selects pinHET[31].

<!-- Page 1046 -->

HWAG Registers www.ti.com
1046 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.2 HWAG Global Control Register 0(HWAGCR0)
Figure 23-90. HWAG Global Control Register 0(HWAGCR0)
31 16
Reserved
R-0
15 1 0
Reserved RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-53. HWAG Global Control Register 0(HWAGCR0) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RESET HWAG Module Reset.
0 HWAG module isreset.
1 HWAG module isnotinreset.
23.5.3 HWAG Global Control Register 1(HWAGCR1)
Figure 23-91. HWAG Global Control Register 1(HWAGCR1)
31 16
Reserved
R-0
15 1 0
Reserved PPWN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-54. HWAG Global Control Register 1(HWAGCR1) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PPWN 0 HWAG Module Power Down. This bitisimplemented forlegacy purposes, buthasnofunctionality,
however theHWAG module power down iscontrolled bytheNHET power down. The HWAG
cannot bepowered down separately.

<!-- Page 1047 -->

www.ti.com HWAG Registers
1047 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.4 HWAG Global Control Register 2(HWAGCR2)
Figure 23-92. HWAG Global Control Register 2(HWAGCR2)
31 25 24 23 18 17 16
Reserved ARST Reserved TED CRI
R-0 R/W-0 R-0 R/W-0 R/W-0
15 9 8 7 1 0
Reserved FIL Reserved STRT
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-55. HWAG Global Control Register 2(HWAGCR2) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ARST Angle Reset. This bitisused bytheHWAG tovalidate thesingularity when thehardware criteria is
notused. The bitiscleared when theHWAG angle value register (HWAACNT) iscleared bythe
HWAG, when thelasttooth edge occurs.
Ifthisbitisnotsetbefore thetooth edge during ansingularity tooth, theHWAG generates an
interruption "singularity notfound ",iftheinterrupt isenabled.
0 Donotreset ACNT once itreaches theangle zero point.
1 Reset ACNT once itreaches theangle zero point.
23-18 Reserved 0 Reads return 0.Writes have noeffect.
17 TED Tooth Edge. This bitisused toselect which edge ofthetooth wheel must beconsidered asactive.
0 Falling edge
1 Rising edge
16 CRI Criteria enable. This bitsisused tocontrol whether thecriteria areapplied. You could setyour own
criteria filter bydisabling thehardwired criteria.
0 Criteria isdisabled.
1 Criteria isenabled.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 FIL Input Filter Enable. This bitisused toenable thetoothed wheel input filter.
0 Filter isdisabled.
1 Filter isenabled.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 STRT Start bit.PuttheHWAG intoruntime. Allows theHWAG tostart counting ACNT, TCNT andcriteria
mechanism (ifset). The HWAG starts atthenext active edge from thetoothed wheel, once set.If
thestart bitiscleared to0,theHWAG isstopped immediately.
0 Donotstart counting.
1 Start counting.

<!-- Page 1048 -->

HWAG Registers www.ti.com
1048 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.5 HWAG Interrupt Enable SetRegister (HWAENASET)
Figure 23-93. HWAG Interrupt Enable SetRegister (HWAENASET)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
SETINTENA7 SETINTENA6 SETINTENA5 SETINTENA4 SETINTENA3 SETINTENA2 SETINTENA1 SETINTENA0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-56. HWAG Interrupt Enable SetRegister (HWAENASET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 SETINTENA[n] Enable interrupt. See Table 23-57 .
0 Read: Corresponding interrupt isnotenabled.
Write: Noeffect.
1 Read: Corresponding interrupt isenabled.
Write: Enable corresponding interrupt.
Table 23-57. HWAG Interrupts
Bit Interrupt
0 Overflow period
1 Singularity notfound
2 Tooth interrupt
3 ACNT overflow
4 PCNT(n) >2xPCNT (n-1) during normal tooth
5 Bad active edge tooth
6 Gap flag
7 Angle increment overflow

<!-- Page 1049 -->

www.ti.com HWAG Registers
1049 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.6 HWAG Interrupt Enable Clear Register (HWAENACLR)
Figure 23-94. HWAG Interrupt Enable Clear Register (HWAENACLR)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLRINTENA7 CLRINTENA6 CLRINTENA5 CLRINTENA4 CLRINTENA3 CLRINTENA2 CLRINTENA1 CLRINTENA0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-58. HWAG Interrupt Enable Clear Register (HWAENACLR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 CLRINTENA[n] Disable interrupt. See Table 23-57 .
0 Read: Corresponding interrupt isnotenabled.
Write: Noeffect.
1 Read: Corresponding interrupt isenabled.
Write: Disable corresponding interrupt.

<!-- Page 1050 -->

HWAG Registers www.ti.com
1050 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.7 HWAG Interrupt Level SetRegister (HWALVLSET)
Figure 23-95. HWAG Interrupt Level SetRegister (HWALVLSET)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
SETINTLVL7 SETINTLVL6 SETINTLVL5 SETINTLVL4 SETINTLVL3 SETINTLVL2 SETINTLVL1 SETINTLVL0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-59. HWAG Interrupt Level SetRegister (HWALVLSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 SETINTLVL[n] SetInterrupt Level. See Table 23-57 .
0 Read: Low-priority interrupt.
Write: Noeffect.
1 Read: High-priority interrupt.
Write: Setinterrupt priority tohigh.
23.5.8 HWAG Interrupt Level Clear Register (HWALVLCLR)
Figure 23-96. HWAG Interrupt Level Clear Register (HWALVLCLR)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLRINTLVL7 CLRINTLVL6 CLRINTLVL5 CLRINTLVL4 CLRINTLVL3 CLRINTLVL2 CLRINTLVL1 CLRINTLVL0
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-60. HWAG Interrupt Level Clear Register (HWALVLCLR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 CLRINTLVL[n] Clear Interrupt Level. See Table 23-57 .
0 Read: Low-priority interrupt.
Write: Noeffect.
1 Read: High-priority interrupt.
Write: Setinterrupt priority tolow.

<!-- Page 1051 -->

www.ti.com HWAG Registers
1051 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.9 HWAG Interrupt Flag Register (HWAFLG)
Figure 23-97. HWAG Interrupt Flag Register (HWAFLG)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
INTFLG7 INTFLG6 INTFLG5 INTFLG4 INTFLG3 INTFLG2 INTFLG1 INTFLG0
R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 23-61. HWAG Interrupt Flag Register (HWAFLG) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 INTFLG[n] Interrupt Flag. These bitaresetwhen aninterrupt condition hasoccurred inside theHWAG. The
interrupt issent totheCPU if,andonly if,thecorresponding enable bitisset.HWAFLG iscleared
byeither reading theHWAOFF0 orHWAOFF1 register (ifthecorresponding bitisset)orbywriting
1tothebit.IfHWAFLG is1butthecorresponding interrupt isnotenabled then itwillnotgenerate
aninterrupt, also theOFFSET index willnotbegenerated forthatparticular HWAFLG bit.So,a
read ofHWAOFF registers willnotclear aHWAFLG bitthatisnotenabled. See Table 23-57 .
0 Read: Nointerrupt ispending.
Write: Noeffect.
1 Read: Interrupt ispending.
Write: Clear thecorresponding interrupt flag.

<!-- Page 1052 -->

HWAG Registers www.ti.com
1052 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.10 HWAG Interrupt Offset Register 0(HWAOFF0)
This register isaread-only register andprovides anumerical value thatrepresents thepending interrupt
with ahigh priority. The index canbeused tolocate theinterrupt routine position inthevector table. A
read tothisregister clears thecorresponding interrupt pending bitintheHWAG interrupt flagregister
(HWAFLG). Aninterrupt pending bitintheHWAFLG register isthebitforwhich thecorresponding
interrupt enable bitisset.
During suspend mode, aread tothisregister does notclear thecorresponding interrupt bit.
Figure 23-98. HWAG Interrupt Offset Register 0(HWAOFF0)
31 16
Reserved
R-0
15 8 7 0
Reserved OFFSET1
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-62. HWAG Interrupt Offset Register 0(HWAOFF0) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 OFFSET1 High-Priority Interrupt Offset. These bitsgive theoffset forthecorresponding interrupts.
0 Phantom interrupt
1 Overflow period
2 Singularity notfound
3 Tooth interrupt
4 ACNT overflow
5 PCNT(n) >2×PCNT (n-1) during normal tooth
6 Bad active edge tooth
7 Gap flag
8 Angle increment overflow

<!-- Page 1053 -->

www.ti.com HWAG Registers
1053 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.11 HWAG Interrupt Offset Register 1(HWAOFF1)
This register isaread-only register andprovides anumerical value thatrepresents thepending interrupt
with alowpriority. The index canbeused tolocate theinterrupt routine position inthevector table. Aread
tothisregister clears thecorresponding interrupt pending bitintheHWAG interrupt flagregister
(HWAFLG). Aninterrupt pending bitintheHWAFLG register isthebitforwhich thecorresponding
interrupt enable bitisset.
During suspend mode, aread tothisregister does notclear thecorresponding interrupt bit.
Figure 23-99. HWAG Interrupt Offset Register 1(HWAOFF1)
31 16
Reserved
R-0
15 8 7 0
Reserved OFFSET2
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-63. HWAG Interrupt Offset Register 1(HWAOFF1) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 OFFSET2 Low-Priority Interrupt Offset.. These bitsgive theoffset forthecorresponding interrupts.
0 Phantom interrupt
1 Overflow period
2 Singularity notfound
3 Tooth interrupt
4 ACNT overflow
5 PCNT(n) >2×PCNT (n-1) during normal tooth
6 Bad active edge tooth
7 Gap flag
8 Angle increment overflow

<!-- Page 1054 -->

HWAG Registers www.ti.com
1054 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.12 HWAG Angle Value Register (HWAACNT)
Figure 23-100. HWAG Angle Value Register (HWAACNT)
31 24 23 16
Reserved ACNT
R-0 R/W-0
15 0
ACNT
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-64. HWAG Angle Value Register (HWAACNT) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 ACNT 0-FF FFFFh Angle Value. Provides thecurrent angle value from thetoothed wheel. This isequal tostep
width ×teeth value.

<!-- Page 1055 -->

www.ti.com HWAG Registers
1055 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.13 HWAG Previous Tooth Period Value Register (HWAPCNT1)
Figure 23-101. HWAG Previous Tooth Period Value Register (HWAPCNT1)
31 24 23 16
Reserved PCNT(n-1)
R-0 R/W-0
15 0
PCNT(n-1)
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-65. HWAG Previous Tooth Period Value Register (HWAPCNT1) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 PCNT(n-1) 0-FF FFFFh Period (n-1) Value. Gives theperiod value oftheprevious tooth.
23.5.14 HWAG Current Tooth Period Value Register (HWAPCNT)
Figure 23-102. HWAG Current Tooth Period Value Register (HWAPCNT)
31 24 23 16
Reserved PCNT(n)
R-0 R/W-0
15 0
PCNT(n)
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-66. HWAG Current Tooth Period Value Register (HWAPCNT) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 PCNT(n) 0-FF FFFFh Period (n)Value. Provides thecurrent period since thebeginning ofthelasttooth active
edge seen bytheHWAG (PCNT (n)).
This period would notbeaccurate duetothefactthatthePCNT counter isrunning atVCLK2
andthattheperipheral busisrunning atVCLK. Then, thevalue willhave changed when
used.

<!-- Page 1056 -->

HWAG Registers www.ti.com
1056 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.15 HWAG Step Width Register (HWASTWD)
Figure 23-103. HWAG Step Width Register (HWASTWD)
31 16
Reserved
R-0
15 4 3 0
Reserved STWD
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-67. HWAG Step Width Register (HWASTWD) Field Descriptions
Bit Field Value Description
31-4 Reserved Reads return 0.Writes have noeffect.
3-0 STWD Step Width. Sets thestep width forthetickgeneration, dividing theperiod intoKsteps. (131072,
65536, ...,8,4).The step count isdecoded from thethree LSBs using thefollowing encoding:
0h 4ticks perperiod
1h 8ticks perperiod
2h 16ticks perperiod
: :
Eh 65536 ticks perperiod
Fh 131072 ticks perperiod

<!-- Page 1057 -->

www.ti.com HWAG Registers
1057 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.16 HWAG Teeth Number Register (HWATHNB)
Figure 23-104. HWAG Teeth Number Register (HWATHNB)
31 16
Reserved
R-0
15 8 7 0
Reserved THNB
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-68. HWAG Teeth Number Register (HWATHNB) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 THNB 0-FFh Teeth Number. Sets theteeth number with themaximum value ofthetoothed wheel. This
must beequal toN-1realteeth (that is,57fora60-2 toothed wheel).
23.5.17 HWAG Current Teeth Number Register (HWATHVL)
Figure 23-105. HWAG Current Teeth Number Register (HWATHVL)
31 16
Reserved
R-0
15 8 7 0
Reserved THVL
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-69. HWAG Current Teeth Number Register (HWATHVL) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 THVL 0-FFh Teeth Value. Provides thecurrent teeth number.

<!-- Page 1058 -->

HWAG Registers www.ti.com
1058 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.18 HWAG Filter Register (HWAFIL)
Figure 23-106. HWAG Filter Register (HWAFIL)
31 16
Reserved
R-0
15 10 9 0
Reserved FIL1
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-70. HWAG Filter Register (HWAFIL) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 FIL1 0-3FFh Filter Value. Contains thevalue tobecompared tothetickcounter. Itallows thetooth signal
tobetaken intoaccount bytheHWAG. This function works only ifthemode filtering isset.
The value iscalculated asshown inSection 23.3.2.2.5 .
23.5.19 HWAG Filter Register 2(HWAFIL2)
Figure 23-107. HWAG Filter Register 2(HWAFIL2)
31 16
Reserved
R-0
15 12 11 0
Reserved FIL2
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 23-71. HWAG Filter Register 2(HWAFIL2) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 FIL2 0-FFFh Filter Value 2.Contains thevalue tobecompared tothetickcounter during thesingularity
tooth. Itallows thetooth signal tobetaken intoaccount bytheHWAG. This function works
only ifthemode filtering isset.The value iscalculated asshown inSection 23.3.2.2.5.1 .

<!-- Page 1059 -->

www.ti.com HWAG Registers
1059 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.5.20 HWAG Angle Increment Register (HWAANGI)
Figure 23-108. HWAG Angle Increment Register (HWAANGI)
31 16
Reserved
R-0
15 10 9 0
Reserved ANGI
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 23-72. HWAG Angle Increment Register (HWAANGI) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 ANGI 0-3FFh Angle Increment Value. Provides thecurrent angle increment value. The value is
incremented bythetickcounter andisdecremented bytheNHET resolution clock.

<!-- Page 1060 -->

Instruction Set www.ti.com
1060 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6 Instruction Set
23.6.1 Instruction Summary
Table 23-73 presents alistoftheinstructions intheN2HET instruction set.The pages following describe
each instruction indetail.
(1)Cycles refers totheclock cycle oftheN2HET module; which onmost devices isVCLK2. (Check thedevice datasheet
description ofclock domains toconfirm). Ifthehigh-resolution prescale value issetto/1,then thisisalso thesame asthe
number ofHRclock cycles.Table 23-73. Instruction Summary
Abbreviation Instruction Name Opcode Sub-Opcode Cycles(1)
ACMP Angle Compare Ch - 1
ACNT Angle Count 9h - 2
ADCNST Add Constant 5h - 2
ADC Add with Carry andShift 4h C[25:23] =011, C5=1 1-3
ADD Add andShift 4h C[25:23] =001, C5=1 1-3
ADM32 Add Move 32 4h C[25:23] =000, C5=1 1-2
AND Bitwise AND andShift 4h C[25:23] =010, C5=1 1-3
APCNT Angle Period Count Eh - 1-2
BR Branch Dh - 1
CNT Count 6h - 1-2
DADM64 Data Add Move 64 2h - 2
DJZ Decrement andJump if-zero Ah P[7:6] =10 1
ECMP Equality Compare 0h C[6:5] =00 1
ECNT Event Count Ah P[7:6] =01 1
MCMP Magnitude Compare 0h C[6] =1 1
MOV32 Move 32 4h C[5] =0 1-2
MOV64 Move 64 1h - 1
OR Bitwise OR 4h C[25:23] =100, C5=1 1-3
PCNT Period/Pulse Count 7h - 1
PWCNT Pulse Width Count Ah P[7:6] =11 1
RADM64 Register Add Move 64 3h - 1
RCNT Ratio Count Ah P[7:6] =00,P[0] =1 3
SBB Subtract with Borrow andShift 4h C[25:23] =110, C[5] =1 1-3
SCMP Sequence Compare 0h C[6:5] =01 1
SCNT Step Count Ah P[7:6] =00,P[0] =0 3
SHFT Shift Fh C[3] =0 1
SUB Subtract andShift 4h C[25:23] =101, C[5] =1 1-3
WCAP Software Capture Word Bh - 1
WCAPE Software Capture Word andEvent Count 8h - 1
XOR Bitwise Exclusive-Or andShift 4h C[25:23] =111, C[5] =1 1-3

<!-- Page 1061 -->

www.ti.com Instruction Set
1061 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-74. FLAGS Generated byInstruction
Abbreviation Flag Name Set/Reset by Used by
C Carry Flag ADC, ADD, AND, OR, RCNT, SBB, SUB, XOR ADC, BR,SBB
N Negative Flag ADC, ADD, AND, OR, SBB, SUB, XOR BR
V Overflow Flag ADC, ADD, AND, OR, SBB, SUB, XOR BR
Z Zero flagACNT, ADC, ADD, AND, APCNT, CNT, OR, PCNT,
SBB, SCNT, SHFT, SUB, XORACMP, ACNT, BR,ECMP,
MCMP, MOV32, RCNT,
SCMP, SHFT
XAngle Compare Match
FlagACMP SCMP
SWF 0-1 Step Width flags SCNT ACNT
NAF New Angle Flag ACNT NAF_global
NAF_global New Angle Flag (global) HWAG orNAFACMP, BR,CNT, ECMP,
ECNT
ACF Acceleration Flag ACNT ,ACNT, SCNT
DCF Deceleration Flag ACNT ,ACNT, SCNT
GPF Gap Flag ACNT ACNT, APCNT
The instructions capable ofgenerating software interrupts arelisted inTable 23-75 .
Table 23-75. Interrupt Capable Instructions
Interrupt Capable Instructions Non Interrupt Capable Instructions
ACMP ADC
ACNT ADCNST
APCNT ADD
BR ADM32
CNT AND
DJZ DADM32
ECMP MOV32
ECNT MOV64
MCMP OR
PCNT RADM64
PWCNT RCNT
SCMP SBB
SHFT SCNT
WCAP SUB
WCAPE XOR

<!-- Page 1062 -->

Instruction Set www.ti.com
1062 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.2 Abbreviations, Encoding Formats andBits
Abbreviations marked with astar(*)areavailable only onspecific instructions.
U Reading abitmarked with Uwillreturn anindeterminate value.
BRK Defines thesoftware breakpoint forthedevice software debugger.
Default: OFF
Location: Program field [22]
next Defines theprogram address ofthenext instruction intheprogram flow. This value
may bealabel oran9-bit unsigned integer.
Default: Current instruction +1
Location: Program field [21:13]
reqnum* Defines thenumber oftherequest line(0,1,..,7) totrigger either theHTU ortheDMA.
Default: 0
Location: Program field [25:23]
request* Allows toselect between norequest (NOREQ), request (GENREQ) andquiet request
(QUIET). See Section 23.2.9 .
Default: Norequest
Location: Control Field [28:27]
Request C[28] C[27] ToHTU ToDMA
NOREQ0 0
norequest norequest
1 0
GENREQ 0 1 request request
QUIET 1 1 quiet request norequest
remote* Determines the9-bit address oftheremote address fortheinstruction.
Default: Current instruction +1
Location: Program field [8:0]
control Determines whether theimmediate data field [31:0] iscleared when itisread. When
thebitisnotset,reads donotclear theimmediate data field.
Default: OFF
Location: Control field [26]
en_pin_action* Determines whether theselected pinisONsothattheaction occurs onthechosen pin
Default: OFF
Location: Control field [22]
Cond_addr* Conditional address (optional): Defines theaddress ofthenext instruction when the
condition occurs.
Default: Current address +1
Location: Control field [21:13]
Pin* PinSelect: Selects thepinonwhich theaction occurs. Enter thepinnumber.
Default: pin0
Location: Control field [12:8] except PCNT

<!-- Page 1063 -->

www.ti.com Instruction Set
1063 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe format CC{pin number} isalso supported.
MSB LSB Description
0 0 0 0 0 Select HET[0]
0 0 0 0 1 Select HET[1]
(Each pinmay beselected bywriting itsnumber inbinary)
1 1 1 1 0 Select HET[30]
1 1 1 1 1 Select HET[31]
Reg* Register select: Selects theregister fordata comparison andstorage
Default: Noregister (None)
Location: Control field [2:1] except forCNT instruction.
Extended Register Select C[7] isavailable forACMP, ADC, ADD, ADM32, AND,
DADM64, ECMP, ECNT, MCMP, MOV32, MOV64, OR, RADM64, SBB, SHFT, SUB,
WCAP, WCAPE instructions.
Register ExtReg. C[7] C[2] C[1]
A 0 0 0
B 0 0 1
T 0 1 0
None 0 1 1
R 1 0 0
S 1 0 1
Reserved 1 1 0
Reserved 1 1 1
Action* (2Action Option) Either sets orclears thepin
Default: Clear
Location: Control Field [4]
Action C[4]
Clear 0
Set 1
Action* (4Action Option) Either sets, clears, pulse high orpulse lowonthepin.Set/clear are
single pinactions, pulse high/low include theopposite pinaction.
Default: Clear
Location: Control Field [4:3]
Action Action Type C[4] C[3]
Clear Setlowonmatch 0 0
Set Sethigh onmatch 1 0
Pulse Low Setlowonmatch +reset tohigh onZ=1 (opposite action) 0 1
Pulse High Sethigh onmatch +reset tolowonZ=1 (opposite action) 1 1

<!-- Page 1064 -->

Instruction Set www.ti.com
1064 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Modulehr_lr* Specifies HIGH/LOW data resolution. Ifthehr_lr field isHIGH, theinstruction uses the
hr_data field. Ifthehr_lr field isLOW, thehr_data field isignored.
Default: HIGH
Location: Program Field [8]
hr_lr Prog. field [8]
LOW 1
HIGH 0
prv* Specifies theinitial value defining theprevious bit(see Section 23.2.5.8 ).Avalue of
ONsets theprevious pin-level bitto1.Avalue ofOFF sets theinitial value ofthe
previous (prv) bitto0.The prvbitisoverwritten (setorreset) bytheN2HET thefirst
time theinstruction isexecuted.
Default: OFF
Location: Control Field [25]
cntl_val* Available forDADM64, MOV64, andRADM64, thisbitfield allows theuser tospecify
thereplacement value fortheremote control field.
comp_mode* Specifies thecompare mode. This field isused with the64-bit move instructions. This
field ensures thatthesub-opcodes aremoved correctly.
Default: ECMP
Location: Control Field [6:5]
Action C[6] C[5] Order
ECMP 0 0
SCMP 0 1
MCMP1 1 0 REG_GE_DATA
MCMP2 1 1 DATA_GE_REG

<!-- Page 1065 -->

www.ti.com Instruction Set
1065 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3 Instruction Description
The following sections provide information forindividual instructions.
Parameters in[]areoptional. Refer totheN2HET assembler user guide forthedefault values when
parameters areomitted.
23.6.3.1 ACMP (Angle Compare)
Syntax ACMP {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}]
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
pin={pin number}
[action={CLEAR |SET}]
reg={A |B|R|S|T|NONE}
[irq={OFF |ON}]
data={25-bit unsigned integer}
}
Figure 23-109. ACMP Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Request
NumberBRK Next program address 1100 Reserved
6 3 1 9 4 9
Figure 23-110. ACMP Control Field (C31:C0)
31 29 28 27 26 25 24 23 22 21 16
Reserved Request type Control Cout
prvReserved En.pin
actionConditional address
3 2 1 1 2 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext.
RegReserved Pin
actionRes. Register select Int.
ena
9 5 1 2 1 1 2 1
Figure 23-111. ACMP Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One
Register modified Selected register (A,B,R,S,orT)

<!-- Page 1066 -->

Instruction Set www.ti.com
1066 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe purpose ofthecomparison istoassert pinaction when theangle compare value liesbetween theold
counter value andthenew counter value (held intheselected register). Since theangle increment varies
from oneloop resolution clock toanother, anexact equality testcannot beapplied. Instead, thefollowing
inequality isused todetermine theoccurrence ofamatch:
Oldcounter value <Angle compare value≤New counter value
This isdone byperforming following comparisons:
Selected register value minus angle increment <angle compare value
Angle compare value≤Selected register value
register Register Bisrecommended fortypical applications with ACMP.
irq Specifies whether ornotaninterrupt isgenerated. Specifying ON
generates aninterrupt when theedge state issatisfied andthegap
flagisset.Specifying OFF prevents aninterrupt from being
generated.
Default: OFF.
data Specifies the25-bit angle compare value.
Execution
X=0;
If(Data <=Selected Register)
Cout =0;
else
Cout =1;
If(Z==0AND (Selected Register -Angle Inc. <Data )AND Cout ==0)OR
(Z==1AND (Cout_prv ==1ORCout ==0)))
{
X=1;
If(Enable Pin Action ==1)
Selected Pin =Pin Action ATnext loop resolution clock;
If(Interrupt Enable ==1)
HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01)
Generate request onrequest line [P25:P23];
If([C28:C27] ==11)
Generate quiet request onrequest line [P25:P23];
Jump toConditional Address;
}
else
Jump toNext Program Address;
Cout_prv =Cout (always executed )
NOTE: Carry-Out Signal (Cout)
Cout isthecarry-out signal oftheadder. Even ifitisnotaflag, itisvalid allalong ACMP
instruction execution.
Angle inc.=NAF_global orhardware angle generator 11-bit input.
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1067 -->

www.ti.com Instruction Set
1067 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.2 ACNT (Angle Count)
Syntax ACNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}]
[request={NOREQ |GENREQ |QUIET}]
edge={RISING |FALLING}
[irq={OFF |ON}]
[control={OFF |ON}]
[prv={OFF |ON}]
gapend ={25-bit unsigned integer}
data={25-bit unsigned integer}
}
Figure 23-112. ACNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 1 0
0 Request
NumberBRK Next program address 1001 Edge
selectReserved Int.
ena
6 3 1 9 4 1 7 1
Figure 23-113. ACNT Control Field (C31:C0)
31 2928 27 26 25 24 0
Res. Request
typeControl Prv. Gap End
3 2 1 1 25
Figure 23-114. ACNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles Two, asfollows:
*First cycle: Angle increment condition andgapendcomparison.
*Second cycle: Gap start comparison.
Register modified Register B(angle value)
Description This instruction defines aspecialized virtual timer used after SCNT and
APCNT togenerate anangle-referenced time base thatissynchronized toan
external signal (that is,atoothed wheel signal). ACNT uses pinHET[2]
exclusively. The edge select must bethesame astheHET[2] edge which was
selected intheprevious APCNT.
ACNT refers tothesame step width selection thattheprevious SCNT saved
inflags SWF0 andSWF1 (see information onSCNT).
ACNT detects period variations oftheexternal signal measured byAPCNT
andcompensates related count errors.

<!-- Page 1068 -->

Instruction Set www.ti.com
1068 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleAperiod increase isflagged inthedeceleration flag(DCF). Aperiod decrease
isflagged intheacceleration flag(ACF). Ifnovariation isdetected, ACNT
increments thecounter value each time SCNT reaches itstarget.
Ifacceleration isdetected, ACNT increments thecounter value oneach timer
resolution. Ifdeceleration isdetected ACNT does notincrement andisthus
saturated.
ACNT also specifies thegapendangle value defining theendvalue ofagap
range inACNT where period measurements inAPCNT aretemporarily
stopped tomask singularities intheexternal signal. ACNT uses register A
containing gapstart andregister Btostore thecounter value.
Edge Specifies theedge fortheinput capture pin(HET[2]).
Action P8 Edge Select
Rising 1 Detects arising edge ofHET[2]
Falling 0 Detects afalling edge ofHET[2]
irq ONgenerates aninterrupt when theedge state issatisfied andthe
gapflagisset.OFF prevents aninterrupt from being generated.
Default: OFF.
gapend Defines the25-bit endvalue ofagaprange. The start value is
defined intheSCNT instruction.
GAPEND =(Step Value *(#ofteeth onthetoothed wheel +#of
missing teeth)) -1
data Specifies the25-bit initial count value forthedata field.
Default: 0.
NOTE: Target Edge Field
The target edge field represents thethree LSBs ofdata field register incase ofstep width =
8,four LSBs forstep width =16,fiveLSBs forstep width =32andsixLSBs forstep width =
64.
Execution
Increment Condition :((Z =1AND DCF =0)ORACF =1)
Pin Edge Condition :Specified edge detected onHET[2]
Target Edge Condition :(Target Edge field indata field =0)AND (Angle
Increment condition istrue) AND (GPF =0)
If(Angle Increment Condition) isfalse
{
NAF =0;
Register B=Data field register;
}
else
{
NAF =1;
If(Counter value !=GapEnd)
{
Register B=Data field register +1;
Data Field Register =Counter value +1;
}

<!-- Page 1069 -->

www.ti.com Instruction Set
1069 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleelse
{
Register B=0;
Data Field Register =0;
If(ACF ==0)DCF =1;
}
}
Z=0;
If(Data field register ==GapStart)
{
GPF =1;
If(Target Edge condition istrue)
{
ACF =0;
If((specified edge isnot detected onpin HET[2]) AND (data
field register !=0)AND (ACF ==0)AND (angle increment condition
istrue))
DCF =1;
}
If(specified edge isdetected onpin HET[2])
{
DCF =0;
If((target_edge_field !=0)AND (DCF ==0)) ACF =1;
If(GPF ==1)
{
GPF =0;
Z=1;
If(Interrupt Enable ==1)
HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01)
Generate request onrequest line [P25:P23];;
If([C28:C27] ==11)
Generate quiet request onrequest line
[P25:P23];
}
}
}
If((target_edge_field !=0)and (pin_edge_cond ==1))
{
pin_update =0;
}
else if(target_edge_field ==0)
{
pin_update =1;
}
If(pin_update istrue innext loop clock cycle)
{
Prv bit =Current Lxvalue ofHET[2] pin;
}
Jump tonext program address;
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1070 -->

Instruction Set www.ti.com
1070 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.3 ADCNST (Add Constant)
Syntax ADCNST {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[control={OFF |ON}]
remote={label |9-bit unsigned integer}
min_off={25-bit unsigned integer}
data={25-bit unsigned integer}
[hr_data={7-bit unsigned integer}]
}
Figure 23-115. ADCNST Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0101 Remote address
6 3 1 9 4 9
Figure 23-116. ADCNST Control Field (C31:C0)
31 27 26 25 24 0
Reserved Control Res. Minimum offset
5 1 1 25
Figure 23-117. ADCNST Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles Two
Register modified Register T(implicity)
Description ADCNST isanextension ofADM32. ADCNST firstchecks whether thedata
field value attheremote address iszero; itthen performs different adds and
moves ontheresult. ADCNST istypically used toextend thecounter value of
PWCNT.
min_off A25-bit constant value thatisadded tothedata field value ifthe
remote data field isnull.
data A25-bit value thatisalways added totheremote data field.
Default: 0.
hr_data Seven least significant bitsofthedata addition totheremote data
field.
Default: 0.

<!-- Page 1071 -->

+
=Minimum offset
Immediate DF
Remote DF25 bits25-bit addition
32 bits
HR
HR
+
=Remote DF
Immediate DF
Remote DF32 bitsLSBs (HR data field) 25-bit addition
HR
HR
www.ti.com Instruction Set
1071 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-118 andFigure 23-119 illustrate thebehavior ofADCNST iftheremote data field iszero oris
notzero.
Figure 23-118. ADCNST Operation IfRemote Data Field[31:7] IsNotZero
Figure 23-119. ADCNST Operation ifRemote Data Field [31:7] IsZero
Execution
If(Remote Data Field Value [31:7] !=0)
Remote Data Field =Immediate Data Field +Remote Data Field;
else
Remote Data Field =Immediate Data Field +min. offset(bits C24:C0);
Jump toNext Program Address;

<!-- Page 1072 -->

Instruction Set www.ti.com
1072 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.4 ADC, ADD, AND, OR,SBB, SUB, XOR
Syntax ADC |ADD |AND |OR|SBB |SUB |XOR {
src1 ={ZERO |IMM |A|B|R|S|T|ONES |REM |REMP }
src2 ={ZERO |IMM |A|B|R|S|T|ONES }
dest ={NONE |IMM |A|B|R|S|T}
[rdest ={NONE |REM |REMP }]
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[remote={label |9-bit unsigned integer}]
[control={OFF |ON}]
[init={OFF |ON}]
[smode ={LSL |CSL |LSR |CSR |RR|CRR |ASR }]
[scount ={5bitunsigned integer}]
[data={25-bit unsigned integer}]
[hr_data={7-bit unsigned integer}]
}
Figure 23-120. ADC, ADD, AND, OR,SBB, SUB, XOR Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0100 Remote address
6 3 1 9 4 9
Figure 23-121. ADC, ADD, AND, OR,SBB, SUB, XOR Control Field (C31:C0)
31 27 26 25 23 22 19 18 16
Reserved Control Sub Opcode Src1 Src2
5 1 3 4 3
15 13 12 8 7 6 5 4 3 2 1 0
Smode Scount Ext. Reg Initflag 1 Rdest Register select Res.
3 5 1 1 1 2 2 1
Figure 23-122. ADC, ADD, AND, OR,SBB, SUB, XOR Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One tothree cycles, depending onoperands selected. (See Table 23-80 )
Register modified Selected register (A,B,R,S,T,orNONE)
Description This instruction performs thespecified 32-bit arithmetic orlogical operation on
operands src1 andsrc2, followed byanoptional shift/rotate step. The result of
thisoperation isthen stored toeither anN2HET register ortheimmediate
data field oftheinstruction. Inaddition, thesame result may bestored ina
remote data field ortheleast signficant bitsofaremote instruction program
field (P[8:0]). BitsP[8:0] oftheprogram field areused bymost instructions
formats tohold theremote address thattheinstruction operates on,sothe
ability toupdate thisfield programatically makes iteasier towrite subroutines
thatoperate ondifferent data sets.

<!-- Page 1073 -->

www.ti.com Instruction Set
1073 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleThe Sub-Opcode field C[25:3] determines which type ofoperation (ADD, ADC, AND, OR, SBB, SUB,
XOR) isexecuted bytheinstruction. Alistofthese operations andthecorresponding Sub-Opcode
encoding canbefound inTable 23-76 .
Allarithmetic isperformed using 32-bit integer math. However, source anddestination operands vary in
width andcanbe9bits(REMP), 25bits(A,B)or32bits(R,S,T, IMM, REM). Source operands REMP,
A,Bareextended to32-bits before being operated on.Also theresult ofthecomputation needs tobe
truncated before being written back toREMP, A,orBwhen these areselected asdestination operands.
Table 23-77 provides alistofsource operand options, how they areexpanded to32-bit integers (if
applicable) andthecontrol field encoding toselect theoption forsrc1 andsrc2 operands.
Table 23-78 provides asimilar listofdestination operands andtheir encodings. Uptotwodestination
operands may beselected foreach instruction, aregister/immediate destination andaremote destination
may beselected simultaneously. Truncation isperformed independently foreach destination operand as
appropriate toitssize.
Anoptional shift step following thearithmetic orlogical operation may beselected through thesmode and
scount operands. The shift orrotate type isselected bythesmode field; Table 23-79 illustrates theoptions
thatareavailable forsmode. The number ofbitsshifted isdetermined bythescount operand.
Table 23-76. Arithmetic /Bitwise Logic Sub-Opcodes
Instruction Description Operation Sub-Opcode
ADC Add with Carry result =src1 +src2 +C C[25:23] =011
ADD Add result =src1 +src2 C[25:23] =001
AND Bitwise Logic And result =src1 &src2 C[25:23] =010
OR Bitwise Logic Or result =src1 |src2 C[25:23] =100
SBB Subtract with Borrow result =src1 -src2 -C C[25:23] =110
SUB Subtract result =src1 -src2 C[25:23] =101
XOR Bitwise Logic Exclusive Or result =src1 ^src2 C[25:23] =111
Table 23-77. Source Operand Choices
Source Operand 32-bit value Address src1 src2
A {A[24:0], 0x00} n/a C[22:19] =0010 C[18:16] =010
B {B[24:0], 0x00} n/a C[22:19] =0011 C[18:16] =011
R R[31:0] n/a C[22:19] =0100 C[18:16] =100
S S[31:0] n/a C[22:19] =0101 C[18:16] =101
T T[31:0] n/a C[22:19] =0110 C[18:16] =110
IMM D[31:0] current instruction address C[22:19] =0001 C[18:16] =001
ZERO 0x00000000 n/a C[22:19] =0000 C[18:16] =000
ONES 0xFFFFFFFF n/a C[22:19] =0111 C[18:16] =111
REM D[31:0] specified byremote[8:0] C[22:19] =1000 n/a
REMP {0x000000, P[8:0]} specified byremote[8:0] C[22:19] =1001 n/a
Table 23-78. Destination Operand Choices
Destination
OperandStored Value Address dest rdest
A A[24:0] =result [31:8] n/a C[7] =0,C[2:1] =00 n/a
B B[24:0] =result [31:8] n/a C[7] =0,C[2:1] =01 n/a
R R[24:0] =result [31:0] n/a C[7] =1,C[2:1] =00 n/a
S S[24:0] =result [31:0] n/a C[7] =1,C[2:1] =01 n/a
T T[24:0] =result [31:0] n/a C[7] =0,C[2:1] =10 n/a

<!-- Page 1074 -->

IC2bit 31 0
IC2bit 31 0
IC1IC2bit 31 0
00 bit 31
IC2 IC1bit 31 0
0bit 31 0
IC2 Sbit 31 0
Instruction Set www.ti.com
1074 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-78. Destination Operand Choices (continued)
Destination
OperandStored Value Address dest rdest
IMM D[31:0] =result [31:0] current instruction address C[7] =1,C[2:1] =10 n/a
NONE n/a n/a C[7] =0,C[2:1] =11 C[4:3] =00
REM D[31:0] =result [31:0] specified byremote[8:0] n/a C[4:3] =01
REMP P[8:0] =result [8:0] specified byremote[8:0] n/a C[4:3] =10
(1)IC1isthecarry flagafter thearithmetic /logical operation isperformed. Ic2istheupdated carry flagafter theshift operation is
performed. sisthesign bit.Table 23-79. Shift Encoding
Shift Type C[15:13] smode Operation Illustrated(1)
NoShift Applied 000 n/a-noshift
ASR-Arithmetic Shift Right 001
LSL-Logical Shift Left 010
CSL-Carry Shift Left 011
LSR-Logical Shift Right 100
CSR-Carry Shift Right 101
RR-Rotate Right 110
CRR -Carry Rotate Right 111
Table 23-80. Execution Time forADC, ADD, AND, OR,SBB, SUB, XOR Instructions
src1 dest rdest remote[8:0]Cycle
s
ZERO, IMM, A,B,R,S,T,orONES A,B,R,S,T, orNONE NONE !=next[8:0] 1
REM orREMP A,B,R,S,T, orNONE NONE !=next[8:0] 2
ZERO, IMM, A,B,R,S,T,orONES IMM REM !=next[8:0] 2
ZERO, IMM, A,B,R,S,T,orONES A,B,R,S,T, orNONE REMP !=next[8:0] 2
ZERO, IMM, A,B,R,S,T,orONES A,B,R,S,T, orNONE NONE ==next[8:0] 2
REM orREMP IMM REM x 3
x IMM REMP x 3
REM orREMP x REM ==next[8:0] 3
x x REMP ==next[8:0] 3

<!-- Page 1075 -->

www.ti.com Instruction Set
1075 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
/Notes: IR1, IR2 are 32-bit intermediate results
//SRC1, SRC2 are 32-bit sources selected
// byfields src1, src2
//IC1, IC2 are intermediate values ofthe carry flag
//IZ1, IZ2 are intermediate values ofthe zero flag
//IN1, IN2 are intermediate values ofthe negative flag
//IV1, IV2 are intermediate values ofthe overflow flag
//scount isthe shift count (0to31) specified byC12:C8
/********** SOURCE OPERAND DECODING STAGE **********/
switch (C22:C19)
{
case 0000:SRC1[31:0] =0x00000000
case 0001:SRC1[31:0] =Immediate Data Field D[31:0]
case 0010:SRC1[31:8] =A[24:0]; SRC1[6:0] =0
case 0011:SRC1[31:8] =B[24:0]; SRC1[6:0] =0
case 0100:SRC1[31:0] =R[31:0]
case 0101:SRC1[31:0] =S[31:0]
case 0110:SRC1[31:0] =T[31:0]
case 0111:SRC1[31:0] =0xFFFFFFFF
case 1000:SRC1[31:0] =Remote Data Field D[31:0]
case 1001:SRC1[31:9] =0;SRC1[8:0] =Remote Program Field P[8:0]
}
switch (C18:C16)
{
case 000:SRC2[31:0] =0x00000000
case 001:SRC2[31:0] =Immediate Data Field[31:0]
case 010:SRC2[31:8] =A[24:0]; SRC2[6:0] =0
case 011:SRC2[31:8] =B[24:0]; SRC2[6:0] =0
case 100:SRC2[31:0] =R[31:0]
case 101:SRC2[31:0] =S[31:0]
case 110:SRC2[31:0] =T[31:0]
case 111:SRC2[31:0] =0xFFFFFFFF
}
/******** ARITHMETIC /LOGICAL OPERATION STAGE *******/
switch (C[25:23])
{
case 011:IR1 =src1 +src2 +C//ADC
case 001:IR1 =src1 +src2 //ADD
case 010:IR1 =src1 &src2 //AND
case 100:IR1 =src1 |src2 //OR
case 110:IR1 =src1 -src2 -C//SBB
case 101:IR1 =src1 -src2 //SUB
case 111:IR1 =src1 ^src2 //XOR
}
IC1 =Carry Out ifOperation isADD, ADC, SUB, SBB
=0ifOperation isAND, OR, XOR
IZ1 =Set ifIR1 iszero, Clear ifIR1 isnon-zero
IN1 =IR[31]
IV1 =(IC1 XOR IR1[31]) AND NOT(SRC1[31] XOR SRC2[31])
/******************** SHIFT STAGE ********************/
switch (C15:C13)
{
case 000: //smode =NoShift
IR2 =IR1
IC2 =IC1; IZ2 =IZ1; IN2 =IN1; IV2 =IV1;
case 001: //smode =Arithmetic Shift Right
IR2[31 -scount :0]=IR1[31:scount]
if(scount >0){
IR2[31 :31-scount +1]=IR1[31]

<!-- Page 1076 -->

Instruction Set www.ti.com
1076 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleIC2 =IR1[scount-1]
}
else {
IC2 =IC1
}
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 010: //smode =Logical Shift Left
IR2[31 :scount] =IR1[31 -scount: 0]
if(scount >0){
IR2[scount -1:0]=0
}
IC2 =IC1
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 011: //smode =Carry Shift Left
IR2[31 :scount] =IR1[31 -scount: 0]
if(scount >0){
IR2[scount -1:0]=[IC1,...IC1]
IC2 =IR1[31 -scount +1]
}
else
{
IC2 =IC1
}
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 100: //smode =Logical Shift Right
IR2[31 -scount :0]=IR1[31:scount]
if(scount >0){
IR2[31 :31-scount +1]=0
}
IC2 =IC1
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 101: //smode =Carry Shift Right
IR2[31 -scount :0]=IR1[31:scount]
if(scount >0){
IR2[31:31-scount +1]=[IC1,...IC1]
IC2 =IR1[scount-1]
}
else {
IC2 =IC1
}
IN2 =IR2[31];
IZ2 =Set ifIR2 ==0;
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 110: //smode =Rotate Right

<!-- Page 1077 -->

www.ti.com Instruction Set
1077 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleIR2[31 -scount :0]=IR1[31:scount]
if(scount >0){
IR2[31:31-scount+1] =IR1[scount-1:0]
IC2 =IR1[scount-1]
}
else {
IC2 =IC1
}
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
case 111: //smode =Carry Rotate Right
IR2[31 -scount :0]=IR1[31:scount]
if(scount ==0){
IC2 =IC1
}
else if(scount ==1){
IR2[31] =IC1
IC2 =IR1[0]
}
else {
IR2[31:31-scount+1] ={IR1[scount-2:0],IC1}
IC2 =IR1[scount -1]
}
IN2 =IR2[31];
if(IR2 ==0){IZ2 =1}else {IZ2 =0};
IV2 =(IR2[31] XOR IR1[31]) ORIV1
}
/********** WRITE REGISTER DESTINATION STAGE ***********/
switch (C7, C2:C1)
{
case 000:A[24:0] =IR2[31:8]
case 001:B[24:0] =IR2[31:8]
case 010:T[31:0] =IR2[31:0]
case 011:IR2 isnot stored inregister, immediate
case 100:R[31:0] =IR2[31:0]
case 101:S[31:0] =IR2[31:0]
case 110:Immediate Data Field[31:0] =IR2
case 111:IR2 isnot stored inregister, immediate
}
/*********** WRITE REMOTE DESTINATION STAGE ***********/
switch (C4:3)
{
case 00:IR2 isnot stored inremote field
case 01:Remote Data Field D[31:0] =IR2
case 10:Remote Program Field P[8:0] =IR2[8:0]
case 11:IR2 isnot stored inremote field
}
/***************** UPDATE FLAGS STAGE *****************/
CFLAG =IC2
NFLAG =IN2
ZFLAG =IZ2
VFLAG =IV2
If(Init Flag ==1)
{
ACF =0;
DCF =1;
GPF =0;
NAF =0;
}
else ACF, DCF, GPF, NAF remain unchanged;

<!-- Page 1078 -->

Instruction Set www.ti.com
1078 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.5 ADM32 (Add Move 32)
Syntax ADM32 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
[control={OFF |ON}]
[init={OFF |ON}]
type={IM &REGTOREG |REM &REGTOREG |IM&REMTOREG |
IM&REGTOREM}
reg={A |B|R|S|T}
data={25-bit unsigned integer}
[hr_data={7-bit unsigned integer}]
}
Figure 23-123. ADM32 Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0100 Remote address
6 3 1 9 4 9
Figure 23-124. ADM32 Control Field (C31:C0)
31 27 26 25 23 22 16
Reserved Control 000 Reserved
5 1 3 15
15 8 7 6 5 4 3 2 1 0
Reserved ExtReg Initflag 1 Move type Register select Res.
15 1 1 1 2 2 1
Figure 23-125. ADM32 Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One ortwocycles (see Table 23-81 )
Register modified Selected register (A,B,R,S,orT)
Description This instruction modifies theselected ALU register ordata field values atthe
remote address depending onthemove type. The modified value results from
adding theimmediate orremote data field totheALU register ortheremote
data field, depending onthemove type. Table description shows theC2and
C1bitencoding fordetermining which register isselected.
init (Optional) Determines whether ornotsystem flags areinitialized. A
value ofONreinitializes thefollowing system flags tothese states:
Acceleration flag(ACF) =0
Deceleration flag(DCF) =1
Gap flag(GPF) =0

<!-- Page 1079 -->

www.ti.com Instruction Set
1079 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleNew angle flag(NAF) =0
Avalue ofOFF results innochange tothesystem flags.
Default: OFF
type Specifies themove type tobeexecuted.
Table 23-81. Move Types forADM32
Type C4 C3 Add Destination(s) Cycles
IM&REGTOREG 0 0Imm. data field +Reg. A,B,R,S,
orTRegister A,B,R,S,orT 1
REM &REGTOREG 0 1Remote data field +Reg. A,B,R,
S,orTRegister A,B,R,S,orT 2
IM&REMTOREG 1 0Imm. data field +Remote data
fieldRegister A,B,R,S,orT 2
IM&REGTOREM 1 1Imm. data field +Reg. A,B,R,S,
orTRemote data field 1
Ifselected register isR,S,orT,theoperation isa32-bit Addition/move. IfAorBregister isselected, itis
limited to25-bit operation since AandBonly support 25-bit.
data Specifies the25-bit integer value fortheimmediate data field.
hr_data Specifies the7least significant bitsoftheimmediate data field.
Default: 0.
Execution
switch (C4:C3)
{
case 00:
Selected register =Selected register +Immediate Data Field;
case 01:
Selected register =Selected register +Remote Data Field;
case 10:
Selected register =Immediate Data Field +Remote Data Field;
case 11:
Remote Data Field =Selected register +Immediate Data Field;
}
If(Init Flag ==1)
{
ACF =0;
DCF =1;
GPF =0;
NAF =0;
}
else
All flags remain unchanged;
Jump toNext Program Address;
Figure 23-126 andFigure 23-127 illustrate theADM32 operation forvarious cases.

<!-- Page 1080 -->

+
=Remote DF
Register A, B, R, S, or T
Register A, B, R, S, or T32 bitsLSBs (HR data field) 25/32-bit addition/move
HR
HR
HR(dashed for R, S, T)
(dashed for R, S, T)
+
=Immediate DF
Register A, B, R, S or T
Register A, B, R, S or T32 bitsLSBs (HR data field) 25/32-bit addition/move
HR
HR
HR(dashed for R, S, T)
(dashed for R, S, T)
Instruction Set www.ti.com
1080 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-126. ADM32 Add andMove Operation forIM&REGTOREG (Case 00)
Figure 23-127. ADM32 Add andMove Operation forREM &REGTOREG (Case 01)

<!-- Page 1081 -->

www.ti.com Instruction Set
1081 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.6 APCNT (Angle Period Count)
Syntax APCNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}]
[request={NOREQ |GENREQ |QUIET}]
[irq={OFF |ON}]
type={FALL2FALL |RISE2RISE}
[control={OFF |ON}]
prv={OFF |ON}}]
period={25-bit unsigned integer}
data={25-bit unsigned integer}
}
Figure 23-128. APCNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 65 0
0 Request
NumberBRK Next program address 1110 Int.
enaEdge
selectReserved
6 3 1 9 4 1 2 6
Figure 23-129. APCNT Control Field (C31:C0)
31 2928 27 26 25 24 0
Res. Request
typeControl Prv. Period Count
3 2 1 1 25
Figure 23-130. APCNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One ortwocycles
*Cycle 1:edge detected (normal operation)
*Cycle 2:edge detected andGPF =1andunderflow condition istrue
One cycle (normal operation) twocycles (edge detected)
Register modified Register AandT(implicitly)
Description This instruction isused before SCNT andACNT togenerate anangle-
referenced time base synchronized toanexternal signal (that is,atoothed
wheel signal). Itisassumed thatthepinandedge selections arethesame for
APCNT andACNT.
APCNT isrestricted topinHET[2]. The toothed wheel must then beconnected
topinHET[2].
APCNT uses thegapflag(GPF) defined byACNT tostart orstop captures in
theperiod count field [C24:C0]. When GPF =1,theprevious period value is
held inthecontrol field andinregister T.When GPF =0,thecurrent period
value iscaptured inthecontrol field andinregister T.

<!-- Page 1082 -->

Instruction Set www.ti.com
1082 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleAPCNT uses thestep width flags (SWF0 andSWF1) defined bySCNT to
detect period durations shorter than onestep, andthen disables capture.
The edge select encoding isshown inTable 23-82 .
irq ONgenerates aninterrupt when theedge state issatisfied. OFF
prevents aninterrupt from being generated.
Default: OFF.
type Specifies theedge type thattriggers theinstruction.
Default: Fall2Fall.
Table 23-82. Edge Select Encoding forAPCNT
type P7 P6 Selected Condition
Fall2Fall 1 0 Falling edge
Rise2Rise 1 1 Rising edge
period Contains the25-bit count value from theprevious APCNT period.
data 25-bit value serving asacounter.
Default: 0.

<!-- Page 1083 -->

www.ti.com Instruction Set
1083 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
Z=0;
If(Data field register !=1FFFFFFh)
{
Register A=Data field register +1;
Data field register =Data field register +1;
}
elseIf (specified edge not detected onHET[2])
{
Register A=1FFFFFFh;
APCNT Ovflw flag =1;
}
If(specified edge detected onHET[2])
{
Z=1;
If(Data field register ==1FFFFFFh)
{
Register A=1FFFFFFh;
Register T=1FFFFFFh;Period count =1FFFFFFh;
Period count =1FFFFFFh;
}
elseIf (GPF ==0AND Data Field register >=Step width)
{
Register A=Data field register +1;
Register T=Register A;
Period count =Register T;
If(Interrupt Enable ==1)
HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01)
Generate request onrequest line [P25:P23];
If([C28:C27] ==11)
Generate quiet request onrequest line [P25:P23];
}
If(GPF ==1)
Register T=Period count;
If(Data Field register <Step width)
{
Register T=Period count;
APCNT Undflw flag =1;
Period Count =000000h;
}
Data field register =000000h;
}
else
{
Register T=Period count;
}
Prv bit =Current Lxvalue ofHET[2] pin;
Jump toNext Program Address;
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1084 -->

Instruction Set www.ti.com
1084 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.7 BR(Branch)
Syntax BR{
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}]
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[prv={OFF |ON}]
cond_addr={label |9-bit unsigned integer}
[pin= {pinnumber}]
event={NOCOND |FALL |RISE |BOTH |ZERO |NAF |LOW |HIGH |C|NC
|EQ|Z|NE|NZ|N|PZ|V|NV|ZN|P|GE|LT|GT|LE|LO|HS}
[irq={OFF |ON}]
}
Figure 23-131. BRProgram Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Request
NumberBRK Next program address 1101 Reserved
6 3 1 9 4 9
Figure 23-132. BRControl Field (C31:C0)
31 29 28 27 26 25 24 22 21 16
Reserved Request type Control Prv Reserved Conditional address
3 2 1 1 3 9
15 13 12 8 7 3 2 1 0
Conditional address Pinselect Branch cond. Reserved Int.ena
9 5 5 2 1
Figure 23-133. BRData Field (D31:D0)
31 0
Reserved
32
Cycles One
Register modified None
Description This instruction executes ajump totheconditional address [C21:C13] onapin
oraflagcondition, andcanbeused with allpins.
Table 23-83 provides thebranch condition encoding.
event Specifies theevent thattriggers ajump totheindexed program
address.
Default: FALL

<!-- Page 1085 -->

www.ti.com Instruction Set
1085 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleirq ONgenerates aninterrupt when theevent occurs thattriggers the
jump. IfirqissettoOFF, nointerrupt isgenerated.
Default: OFF.
Table 23-83. Branch Condition Encoding forBR
Event C7 C6 C5 C4 C3 Branch Condition
NOCOND 0 0 0 0 0 Always
FALL 0 0 1 0 0 Onfalling edge ontheselected pin
RISE 0 1 0 0 0 Onrising edge onselected pin
BOTH 0 1 1 0 0 Onrising orfalling edge onselected pin
ZERO 1 0 0 0 0 IfZero flagisset
NAF 1 0 1 0 0 IfNAF_global flagisset
LOW 1 1 0 0 0 OnLOW level onselected pin
HIGH 1 1 1 0 0 OnHIGH level onselected pin
C 0 0 0 0 1 Carry Set: C==1
NC 0 0 0 1 1 Carry NotSet: C==0
EQ, Z 0 0 1 0 1 Equal orZero: Z==1
NE,NZ 0 0 1 1 1 NotEqual orNotZero: Z==0
N 0 1 0 0 1 Negative: N==1
PZ 0 0 1 1 1 Positive orZero: N==0
V 0 1 1 0 1 Overflow: V==1
NV 0 1 1 1 1 NoOverflow: V==0
ZN 1 0 0 0 1 Zero orNegative: (ZORN)==1
P 1 0 0 1 1 Positive: (ZORN)==0
GE 1 0 1 1 1 Signed Greater Than orEqual: (NXOR V)==0
L 1 0 1 0 1 Signed Less Than (NXOR V)==1
G 1 1 0 1 1 Signed Greater Than (ZOR(NXOR V))==0
LE 1 1 0 0 1 Signed Less Than (ZOR(NXOR V))==1
LO 1 1 1 1 1 Unsigned Less Than: (CORZ)==0
HS 1 1 1 0 1 Unsigned Higher orSame (CORZ)==1
Execution
If(Condition istrue)
{
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Jump toConditional Address;
}
else
{
Jump toNext Program Address;
}
Prv bit =Current Lxvalue ofselected pin; (Always Executed)
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1086 -->

Instruction Set www.ti.com
1086 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.8 CNT (Count)
Syntax CNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}]
[request={NOREQ |GENREQ |QUIET}]
[angle_count={OFF |ON}]
[reg={A |B|T|NONE}]
[comp ={EQ |GE}]
[irq={OFF |ON}]
[control={OFF |ON}]
max={25-bit unsigned integer}
[data={25-bit unsigned integer]
}
Figure 23-134. CNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 76 5 41 0
0 Request
NumberBRK Next program address 0110 Angle
countRegister Comp.
selectRes. Int.ena
6 3 1 9 4 1 2 1 4 1
Figure 23-135. CNT Control Field (C31:C0)
31 2928 27 26 25 24 0
Res. Request
typeControl Res. Max Count
3 2 1 1 25
Figure 23-136. CNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One ortwo
One cycle (time mode), twocycles (angle mode)
Register modified Selected register (A,BorT)
Description This instruction defines avirtual timer. The counter value stored inthedata
field [D31:7] isincremented unconditionally oneach execution ofthe
instruction when intime mode (angle count bit[P8] =0).When thecount
reaches themaximum count specified inthecontrol field, thecounter isreset.
Ittakes onecycle inthismode.
Inangle mode (angle count bit[P8] =1),CNT needs data from thesoftware
angle generator (SWAG). When inangle count mode theangle increment
value willbe0or1.Ittakes twocycles inthismode.

<!-- Page 1087 -->

www.ti.com Instruction Set
1087 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleangle_count Specifies when thecounter isincremented. Avalue ofONcauses the
counter value tobeincremented only ifthenew angle flagisset
(NAF_global =1).Avalue ofOFF increments thecounter each time
theCNT instruction isexecuted.
Default value forthisfield isOFF.
comp When settoEQthecounter isreset, when itisequal tothemaximum
count.
When settoGEthecounter isreset, when itisgreater orequal tothe
maximum count.
Default: GE.
irq ONgenerates aninterrupt when thecounter overflows tozero. The
interrupt isnotgenerated until thedata field isreset tozero. Ifirqis
settoOFF, nointerrupt isgenerated.
Default: OFF.
max Specifies the25-bit integer value thatdefines themaximum count
value allowed inthedata field. When thecount inthedata field is
equal tomax, thedata field isreset to0andtheZsystem flagisset
to1.
data Specifies the25-bit integer value serving asacounter.
Default: 0.

<!-- Page 1088 -->

Instruction Set www.ti.com
1088 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
Z=0;
If(Angle Count (bit P8==1))
{
If(NAF_global ==0)
{
Selected register =immediate data field;
Jump toNext Program Address;
}
else
{
If((Immediate Data Field +Angle Increment) >=Max count)
{
Z=1;
Selected register =((Immediate Data Field +Angle Inc.) -Max count);
Immediate Data Field =((Immediate Data Field +Angle Inc.) -Max count);
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
}
else
{
Selected register =Immediate Data Field +Angle Increment;
Immediate Data Field =Immediate Data Field +Angle Increment;
}
}
}
else if(Time mode (bit P8==0))
{
If[(P5==0) AND (Immediate Data Field ==Max count)]
OR[(P5==1) AND (Immediate Data Field >=Max count)]
{
Z=1;
Selected register =00000;
Immediate Data Field =00000;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
}
else
{
Selected register =Immediate Data Field +1;
Immediate Data Field =Immediate Data Field +1;
}
}
Jump toNext Program Address;
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1089 -->

www.ti.com Instruction Set
1089 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.9 DADM64 (Data Add Move 64)
Syntax DADM64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
[pin={pin number}]
comp_mode={ECMP |SCMP |MCMP1 |MCMP2}
[action={CLEAR |SET |PULSELO |PULSEHI}]
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}]
}
-or-
Syntax DADM64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
cntl_val={29-bit unsigned integer}
data={25-bit unsigned integer}
[hr_data= {7-bit unsigned integer}]
}

<!-- Page 1090 -->

+
=Immediate DF
Remote DF
Remote DF32 bitsLSBs (HR Data Field)
Immediate CF
Remote CFHR
HR
HR
Instruction Set www.ti.com
1090 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-137. DADM64 Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0010 Remote Address
6 3 1 9 4 9
Figure 23-138. DADM64 Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegComp. mode Action Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-139. DADM64 Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles Two
Register modified Register T(implicitly)
Description This instruction modifies thedata field andthecontrol field attheremote
address. The remote data field value isnotjustreplaced, butisadded with the
DADM64 data field.
DADM64 hastwodistinct syntaxes. Inthefirstsyntax, bitvalues may beset
byassigning avalue toeach ofthecontrol fields. This syntax isconvenient for
modifying control fields thatarearranged similarly totheformat ofthe
DADM64 control field. Asecond syntax, inwhich theentire 29-bit control field
isspecified bythecntl_val field, isconvenient when theremote control field is
dissimilar totheDADM64 control field. Either syntax may beused, butyou
must useoneortheother butnotacombination ofsyntaxes.
Figure 23-140 shows theDADM64 addandmove operation.
Figure 23-140. DADM64 Add andMove Operation
Table 23-84. DADM64 Control Field Description
request maintains thecontrol field fortheremote instruction
control maintains thecontrol field fortheremote instruction
en_pin_action maintains thecontrol field fortheremote instruction
cond_addr maintains thecontrol field fortheremote instruction

<!-- Page 1091 -->

www.ti.com Instruction Set
1091 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-84. DADM64 Control Field Description (continued)
pin maintains thecontrol field fortheremote instruction
register maintains thecontrol field fortheremote instruction
action maintains thecontrol field fortheremote instruction
irq maintains thecontrol field fortheremote instruction
data Specifies the25-bit initial value forthedata field.
hr_dataSeven least significant bitsofthe32bitdata field.
Default: 0
cntl_val Specifies the29least significant bitsoftheControl field.
Execution
Remote Data Field =Remote Data Field +Immediate Data Field;
Register T=Immediate Data Field;
Remote Control Field =Immediate Control Field;
Jump toNext Program Address;

<!-- Page 1092 -->

Instruction Set www.ti.com
1092 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.10 DJZ (Decrement andJump ifZero)
DJNZ isalso asupported syntax. The functionality ofthetwoinstruction names isidentical.
Syntax DJZ {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
[reg={A |B|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
}
Figure 23-141. DJZ Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 65 0
0 Request
NumberBRK Next program address 1010 Res. 10 Reserved
6 3 1 9 4 1 2 6
Figure 23-142. DJZ Control Field (C31:C0)
31 29 28 27 26 25 22 21 16
Reserved Request type Control Reserved Conditional address
3 2 1 4 9
15 13 12 8 7 3 2 1 0
Conditional address Reserved Register select Int.ena
9 10 2 1
Figure 23-143. DJZ Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One
Register modified Selected register (A,B,orT)
Description This instruction defines avirtual down counter used fordelayed execution of
certain instructions (togenerate minimum on/off times). When DJZ is
executed with counter value notzero, thecounter value isdecremented. Ifthe
counter value iszero, thecounter remains zero until itisreloaded with anon-
zero value. The program flow canbemodified when down counter value is
zero byusing theconditional address.

<!-- Page 1093 -->

www.ti.com Instruction Set
1093 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Modulecond_addr This field isnotoptional fortheDJZ instruction.
irq ONgenerates aninterrupt when thedata field reaches zero. No
interrupt isgenerated when thebitisOFF.
Default: OFF.
data Specifies the25-bit integer value used asacounter. This counter is
decremented each time theDJZ instruction isexecuted until the
counter reaches 0.
Default: 0.
Execution
If(Data !=0)
{
Data =Selected register =Data -1;
Jump toNext Program Address;
}
else
{
Selected register =000000h;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Jump toconditional Address;
}
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1094 -->

Instruction Set www.ti.com
1094 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.11 ECMP (Equality Compare)
Syntax ECMP {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[hr_lr={HIGH |LOW}]
[angle_comp={OFF |ON}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
pin={pin number}
[action={CLEAR |SET |PULSELO |PULSEHI}]
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data={7-bit unsigned integer}]
}
Figure 23-144. ECMP Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 6 0
0 Request
NumberBRK Next program address 0000 hr_lr Angle
comp.Reserved
6 3 1 9 4 1 1 7
Figure 23-145. ECMP Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
Reg00 Action Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-146. ECMP Data Field (D31:D0)
31 76 0
Data HRData
25 7

<!-- Page 1095 -->

www.ti.com Instruction Set
1095 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleCycles One
Register modified Register A,B,R,SorTifselected
Description ECMP canuseallpins. This instruction compares a25-bit data value stored in
thedata field (D31 -D7)tothevalue stored intheselected ALU register (A,B,
R,S,orT).Register select encoding canbefound inSection 23.6.2 .
IfR,S,orTregisters areselected, andifthe25-bit data field matches, ECMP
updates theregister with the32-bit value (D31-D0).
Ifthehr_lr bitiscleared, thepinaction willoccur after ahigh resolution delay
from thenext loop resolution clock. Ifthehr_lr bitisset,thedelay isignored.
This delay isprogrammed inthedata field (D6-D0).
The behavior ofthepins isgoverned bythefour action options inbitsC4:C3.
ECMP uses thezero flagtogenerate opposite pinaction (synchronized tothe
loop resolution clock).
angle_comp Determines ifanangle compare isperformed. Avalue ofONcauses
thecomparison tobeperformed only ifthenew angle flagisset(NAF
=1).IfOFF isspecified, thecompare isthen performed regardless of
thestate ofthenew angle flag.
Default: OFF.
irq Specifies whether ornotaninterrupt isgenerated. Avalue ofON
sends aninterrupt ifregister anddata field values areequivalent. If
OFF isselected, nointerrupt isgenerated.
Default: OFF.
data Specifies thevalue forthedata field. This value iscompared with the
selected register.
hr_data Specifies theHRdelay.
Default: 0.

<!-- Page 1096 -->

Instruction Set www.ti.com
1096 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
If(Angle Comp. bit ==0OR(Angle Comp. bit ==1AND NAF_global ==1))
{
If(Selected register value ==Immediate data field value)
{
If(hr_lr bit ==0)
{
If(Enable Pin action ==1)
{
Selected Pin =Pin Action ATnext loop resolution clock +HRdelay;
}
}
else
{
If(Enable Pin action ==1)
{
Selected Pin =Pin Action ATnext loop resolution clock;
}
}
If(Z==1AND Opposite action ==1)
{
If(Enable Pin action ==1)
{
Selected Pin =opposite Pin Action ATnext loop resolution clock;
}
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11)Generate quiet request onrequest line [P25:P23];
If(register Risselected) Rregister =Compare value (32 bit);
If(register Sisselected) Sregister =Compare value (32 bit);
If(register Tisselected) Tregister =Compare value (32 bit);
Jump toConditional Address;
}
}
elseIf (Z==1AND Opposite action ==1)
{
If(Enable Pin action ==1)
{
Selected Pin =opposite Pin Action ATnext loop resolution clock;
}
Jump toNext Program Address;
}
else //Angle Comp. bit ==1AND NAF_global ==0
{
Jump toNext Program Address;
}
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1097 -->

www.ti.com Instruction Set
1097 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.12 ECNT (Event Count)
Syntax ECNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[prv={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
pin={pin number}
event={NAF |FALL |RISE |BOTH |ACCUHIGH |ACCULOW}
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
}
Figure 23-147. ECNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 65 0
0 Request
NumberBRK Next program address 1010 Res. 01 Reserved
6 3 1 9 4 1 2 6
Figure 23-148. ECNT Control Field (C31:C0)
31 29 28 27 26 25 24 22 21 16
Reserved Request type Control Prv. Reserved Conditional address
3 2 1 1 3 9
15 13 12 8 7 6 4 3 2 1 0
Conditional address Pinselect Ext
RegEvent Res. Register select Int.
ena
9 5 1 3 1 2 1
Figure 23-149. ECNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One cycle
Register modified Selected Register (A,B,R,S,Tornone)
Description This instruction defines aspecialized 25-bit virtual counter used asanevent
counter orpulse accumulator (see Table 23-85 ).The counter value isstored
inthedata field [D31:D7] andtheselected register. Ifoneofthe32-bit
registers (R,S,T) isselected, the25bitcount value isstored leftjustified inthe
register with zeros intheseven least significant bits.
When anevent count condition isspecified, thecounter value isincremented
onapinedge condition orontheNAF condition (NAF isdefined inACNT).
This instruction canbeused with allpins.

<!-- Page 1098 -->

Instruction Set www.ti.com
1098 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleevent The event thattriggers thecounter.
Table 23-85. Event Encoding Format forECNT
Event C6 C5 C4 Count Conditions Mode Int.Available
NAF 0 0 0 NAF flagisSet Angle counter Y
FALL 0 0 1 Falling edge onselected pin Event counter Y
RISE 0 1 0 Rising edge onselected pin Event counter Y
BOTH 0 1 1Rising andFalling edge on
selected pinEvent counter Y
ACCUHIGH 1 0 - while pinishigh level Pulse accumulation N
ACCULOW 1 1 - while pinislowlevel Pulse accumulation N
irq ONgenerates aninterrupt when event incounter mode occurs. No
interrupt isgenerated with OFF.
Default: OFF.
data 25-bit integer value serving asacounter.
Default: 0.
Execution
If(event occurs)
{
If(Register AorBSelected) {
Selected register =Immediate Data Field +1;
}
If(Register R,SorTSelected)
{
Selected register[31:7] =Immediate Data Field +1;
Selected register[6:0] =0;
}
Immediate Data Field =Immediate Data Field +1;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request online [P25:P23];
If([C28:C27] ==11) Generate quiet request online [P25:P23];
Jump toConditional Address;
}
else
{
Jump toNext Program Address;
}
Prv bit =Current Logic (Lx) value ofselected pin; (Always executed)
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1099 -->

www.ti.com Instruction Set
1099 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.13 MCMP (Magnitude Compare)
Syntax MCMP {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[hr_lr={LOW |HIGH}]
[angle_comp={OFF |ON}]
[savesub={OFF |ON}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
pin={pin number}
order={REG_GE_DATA |DATA_GE_REG}
[action={CLEAR |SET |PULSELO |PULSEHI}]
reg={A |B|R|S|T|NONE}
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data={7-bit unsigned integer}]
}
Figure 23-150. MCMP Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 6 5 40
0 Request
NumberBRK Next program address 0000 hr_lr Angle
comp.Res. Save
sub.Res.
6 3 1 9 4 1 1 1 1 5
Figure 23-151. MCMP Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
Reg1 Order Action Register select Int.
ena
9 5 1 1 1 2 2 1
Figure 23-152. MCMP Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One
Register modified T(ifsave subbitP[5] isset)

<!-- Page 1100 -->

Instruction Set www.ti.com
1100 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleDescription This instruction compares themagnitude ofthe25-bit data value stored inthe
data field (D31-D7) andthe25-bit value stored intheselected ALU register
(A,B,R,S,orT).
Ifthehr_lr bitisreset, pinaction willoccur after adelay from thenext loop
resolution clock. Ifthehr_lr bitisset,thedelay isignored. This delay is
programmed inthedata field (D6-D0).
When thedata value matches, anoutput pincanbesetorreset according to
thepinaction bit(C[4]). The pinwillnotchange states iftheenable pinaction
bit(C[22]) isreset.
MCMP uses thezero flagsettogenerate opposite pinaction (synchronized to
theloop resolution clock). The save subbit(P[5]) provides theoption tosave
theresult ofasubtraction intoregister T.
NOTE: TheDifference Between Compare Values
The difference between thetwodata values must notexceed (224)-1.
angle_comp Determines whether ornotanangle compare isperformed. Avalue of
ONcauses thecomparison tobeperformed only ifthenew angle flag
isset(NAF =1).IfOFF isspecified, thecompare isthen performed
regardless ofthestate ofthenew angle flag.
Default: OFF.
savesub When set,thecomparison result issaved intotheTregister (upper
25bits).
Default: OFF.
order Specifies theorder oftheoperands forthecomparison.
Table 23-86. Magnitude Compare Order forMCMP
Order C5 Description
REG_GE_DATA 0 Evaluates totrue iftheregister value isgreater than orequal tothedata field value.
DATA_GE_REG 1 Evaluates totrue ifthedata field value isgreater than orequal totheregister value.
irq Specifies whether ornotaninterrupt isgenerated. Avalue ofON
sends aninterrupt ifthecompare match occurs according totheorder
selected. IfOFF isselected, nointerrupt isgenerated.
data Specifies thevalue forthedata field. This value iscompared with the
selected register.
hr_data HRdelay. The default value foranunspecified bitis0.

<!-- Page 1101 -->

www.ti.com Instruction Set
1101 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
If(Angle Compare P[7] ==0OR(P[7] ==1AND NAF_global ==1))
{
If( (Order C[5] ==1)AND (Data[31:7]- Selected register[31:7]) >=0))
OR((Order C[5] ==0)AND Selected register[31:7] -Data[31:7]) >=0))
{
If(Order C[5] ==1AND Save subtract P[5] ==1)
{
Register T[31:7] =Data[31:7] -Selected register[31:7];
Register T[6:0] =0;
}
If(Order C[5] ==0AND Save subtract P[5] ==1)
{
Register T[31:7] =Selected register[31:7] -Data[31:7];
Register T[6:0] =0;
}
If(Enable Pin Action C[22] ==1)
{
If(hr_lr P[8] =0){
Schedule Action onSelected Pin C[12:8] atstart ofnext loop
+HRDelay D[6:0];
}
else
{
Schedule Pin Action onSelected Pin C[12:8] atstart ofnext loop;
}
}
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Jump toConditional Address;
}
else if(Z==1AND Opposite Action C[3] ==1)
{
If(Enable Pin Action C[22] ==1)
{
Schedule Opposite Pin Action onSelected Pin C[12:8] atstart ofnext loop;
}
Jump toNext Program Address;
}
else
Jump toNext Program Address;
}
else //Angle Comp. bit ==1AND NAF_global ==0
Jump toNext Program Address;
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1102 -->

Instruction Set www.ti.com
1102 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.14 MOV32 (Data Move 32)
Syntax MOV32 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
[control={OFF |ON}]
[z_cond={OFF |ON}]
[init={OFF |ON}]| ON}]
type={IMTOREG |IMTOREG &REM |REGTOREM |REMTOREG}
[reg={A |B|R|S|T|NONE}]
[data={25-bit unsigned integer]
[hr_data={7-bit unsigned integer}]
}
Figure 23-153. MOV32 Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0100 Remote Address
6 3 1 9 4 9
Figure 23-154. MOV32 Control Field (C31:C0)
31 27 26 25 23 22 21 16
Reserved Control Reserved ZFl.Cond. Reserved
5 1 3 1 14
15 8 7 6 5 4 3 2 1 0
Reserved ExtReg Initflag 0 Move type Register select Res.
14 1 1 1 2 2 1
Figure 23-155. MOV32 Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One ortwocycles
Register modified Selected register (A,B,R,S,orT)
Description MOV32 replaces theselected ALU register and/or thedata field values atthe
remote address location depending onthemove type.
Figure 23-156 through Figure 23-159 illustrate these operations. Ifnoregister
isselected, themove isnotexecuted, except forconfiguration C4:C3 =01,
where theremote data field iswritten with theimmediate data field value.

<!-- Page 1103 -->

Immediate DF
Register A, B, or R, S or T32 bitsLSBs (HR data field) 25/32-bit move
HR
HR
(dashed for R, S, T)
www.ti.com Instruction Set
1103 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleremote Determines thelocation oftheremote address.
Default: Current instruction +1.
z_cond When settoOFF theMOV32 performs themove operation specified
bythemove type whenever itisexecuted (independent onthestate
oftheZ-Flag).
When settoONtheMOV32 performs themove operation specified
bythemove type only when theZ-Flag isset.
init (Optional) Determines whether ornotsystem flags areinitialized. A
value ofONreinitializes thefollowing system flags tothese states:
Acceleration flag(ACF) =0
Deceleration flag(DCF) =1
Gap flag(GPF) =0
New angle flag(NAF) =0
Avalue ofOFF results innochange tothesystem flags.
type Specifies themove type tobeexecuted.
Table 23-87. Move Type Encoding Selection
Move Type C4 C3 Source Destination(s) Cycles
IMTOREG 0 0 Immediate data field Register A,B,R,S,orT 1
IMTOREG &REM 0 1 Immediate data fieldRemote data field andregister
A,B,R,S,orT1
REGTOREM 1 0 Register A,B,R,S,orT Remote data field 1
REMTOREG 1 1 Remote data field Register A,B,R,S,orT 2
Figure 23-156. MOV32 Move Operation forIMTOREG (Case 00)
reg Specifies which register (A,B,T,orNONE) isinvolved inthemove. A
register (A,B,orT)must bespecified forevery move type except
IMTOREG &REM. IfNONE isused with move type IMTOREG &REM,
theMOV32 executes amove from theimmediate data field tothe
remote data field. IfNONE isused with anyother move type, no
move isexecuted.
data Specifies a25-bit integer value tobewritten totheremote data field
orselected register.
hr_data (Optional) HRdelay. The default value foranunspecified bitis0.

<!-- Page 1104 -->

Register A, B, R, S, or TRemote DF32 bitsLSBs (HR data field) 25/32-bit move
HR
HR
(dashed for R, S, T)
Remote DFRegister A, B, R, S, or T
LSBs (HR data field = 0 if A or B)25/32-bit move
HR
HR(dashed for R, S, T)
32 bitsLSBs (HR data field) 25/32-bit move
Immediate DF
Register A, B, R, S or T Remote DFHR
HR HR
(dashed for R, S, T)
Instruction Set www.ti.com
1104 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-157. MOV32 Move Operation forIMTOREG &REM (Case 01)
Figure 23-158. MOV32 Move Operation forREGTOREM (Case 10)
Figure 23-159. MOV32 Move Operation forREMTOREG (Case 11)

<!-- Page 1105 -->

www.ti.com Instruction Set
1105 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
If[(z_cond C[22] ==0) OR((z_cond C[22] ==1)AND (ZFlag ==1))]
{
switch (type C[4:3])
{
case 00: //IMTOREG
Selected register =Immediate Data Field;
case 01: //IMTOREG &REM
Selected register =Immediate Data Field;
Remote Data Field =Immediate Data Field;
case 10: //REGTOREM
Remote Data Field =Selected register;
case 11: //REMTOREG
Selected register =Remote Data Field;
}
}
If(Init Flag ==1)
{
ACF =0;
DCF =1;
GPF =0;
NAF =0;
}
else
All flags remain unchanged;
Jump toNext Program Address;

<!-- Page 1106 -->

Instruction Set www.ti.com
1106 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.15 MOV64 (Data Move 64)
Syntax MOV64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
[pin={pin number}]
comp_mode={ECMP |SCMP |MCMP1 |MCMP2}
[action={CLEAR |SET |PULSELO |PULSEHI}]
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}
}
-or-
Syntax MOV64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
cntl_val={29-bit unsigned integer}
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}
}

<!-- Page 1107 -->

Immediate CF + DF
Remote CF + DFHR
HR
www.ti.com Instruction Set
1107 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-160. MOV64 Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0001 Remote Address
6 3 1 9 4 9
Figure 23-161. MOV64 Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegComp. mode Action Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-162. MOV64 Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One
Register modified None
Description This instruction modifies thedata field andthecontrol field attheremote
address.
MOV64 hastwodistinct syntaxes. Inthefirstsyntax, bitvalues may besetby
assigning avalue toeach ofthecontrol fields. This syntax isconvenient for
modifying control fields thatarearranged similarly totheformat oftheMOV64
control field. Asecond syntax, inwhich theentire 29-bit control field is
specified bythecntl_val field, isconvenient when theremote control field is
dissimilar totheMOV64 control field. Either syntax may beused, butyoumust
useoneortheeither butnotacombination ofsyntaxes. See Figure 23-163 .
Figure 23-163. MOV64 Move Operation
Table 23-88. MOV64 Control Field Descriptions
request Maintains thecontrol field fortheremote instruction.
control Maintains thecontrol field fortheremote instruction.
en_pin_action Maintains thecontrol field fortheremote instruction.
cond_addr Maintains thecontrol field fortheremote instruction.
pin Maintains thecontrol field fortheremote instruction.
register, extreg Maintains thecontrol field fortheremote instruction.
comp_mode Selects thecomparison mode type tobeused bytheremote instruction.

<!-- Page 1108 -->

Instruction Set www.ti.com
1108 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-88. MOV64 Control Field Descriptions (continued)
action Maintains thecontrol field fortheremote instruction.
irq Maintains thecontrol field fortheremote instruction.
data Specifies the25-bit initial count value forthedata field. Ifomitted, thefield defaults to0.
hr_data (Optional) HRdelay. The default value foranunspecified bitis0.
Table 23-89. Comparison Type Encoding Format
comp_mode C[6] C[5] MCMP Order
ECMP 0 0
SCMP 0 1
MCMP1 1 0 REG_GE_DATA
MCMP2 1 1 DATA_GE_REG
Execution
Remote Data Field =Immediate Data Field;
Remote Control Field =Immediate control Field;
Jump toNext Program Address;

<!-- Page 1109 -->

www.ti.com Instruction Set
1109 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.16 PCNT (Period/Pulse Count)
Syntax PCNT {
[hr_lr={HIGH |LOW}]
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[irq={OFF |ON}]
type={FALL2RISE |RISE2FALL |FALL2FALL |RISE2RISE}
pin={pin number}
[control={OFF |ON}]
[prv={OFF |ON}]
[period={25-bit unsigned integer}]
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}
}
Figure 23-164. PCNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 6 5 40
0 Request
NumberBRK Next program address 0111 Int.
enaType select hr_lr Pin
select
6 3 1 9 4 1 2 1 5
Figure 23-165. PCNT Control Field (C31:C0)
31 2928 27 26 25 24 0
Res. Request
typeControl Prv. Period Count
3 2 1 1 25
Figure 23-166. PCNT Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One
Register modified Register A
Description This instruction detects theedges oftheexternal signal atloop start and
measures itsperiod orpulse duration. The counter value stored inthecontrol
field C[24:0] andintheregister Aisincremented each N2HET loop. PCNT
uses theHRstructure onthepintomeasure anHRperiod/pulse count value.
hr_lr (Optional) Specifies whether thePCNT instruction captures theHR
delay intotheHRdata field ontheselected edge condition. Ifhr_lr is
0(HIGH) then PCNT captures theHRdelay. ifhr_lr is1(LOW) then
PCNT only captures atloop resolution.

<!-- Page 1110 -->

Instruction Set www.ti.com
1110 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleirq (Optional) Specifies whether ornotaninterrupt isgenerated. Avalue
ofONsends aninterrupt when anew value iscaptured. IfOFF is
selected, nointerrupt isgenerated.
type (Optional) Determines thetype ofcounter thatisimplemented.
Table 23-90. Counter Type Encoding Format
P7 P6 Period/Pulse Select Reset On Capture On
FALL2RISE 0 0Count low-pulse duration on
selected pinFalling edge Rising edge
RISE2FALL 0 1Count high-pulse duration on
selected pinRising edge Falling edge
FALL2FALL 1 0Count period between falling edges
onselected pinFalling edge Falling edge
RISE2RISE 1 1Count period between rising edges
onselected pinRising edge Rising edge
period Specifies the25-bit integer value thatholds thecounter value. The
counter value isalso stored inregister A.
Default: 0.
data 25-bit integer representing thelastcaptured counter value.
Default: 0.
hr_data HRdelay.
Default: 0.
Ifperiod-measure isselected, PCNT captures thecounter value intotheperiod/pulse data field [D31:D7]
ontheselected edge. The HRstructure provides HRcapture field [D6:D0]. The counter value [C24:C0] is
reset onthesame edge. The captured period value isa32-bit value.
Ifpulse-measure isselected, PCNT captures thecounter value intotheperiod/pulse count field [D31:D7]
ontheselected edge. The HRstructure provides HRcapture field [D6:D0]. The counter value [C24:C0] is
reset onthenext opposite edge. The captured pulse value isa32-bit value.
When theoverflow count (all1'sinthecounter value) isreached, PCNT stops counting until thenext reset
edge isdetected.
Note: ForFALL2FALL/RISE2RISE, theuser should always discard thefirstinterrupt/HTU request if
interrupt/request areenabled before HET_ON. Forboth thetypes, reset edge andcapture edge arethe
same andtheinterrupt orHTU request istriggered oncapture edge (which isnothing butthereset edge).
Once theexecution unitisenabled, thefirstedge generates aninterrupt butthevalue ofthecounter isof
nouseasthisisnottheperiod between 2edges. Sofirstedge after turning onN2HET isused mainly for
resetting thecounter andstart theperiod count.

<!-- Page 1111 -->

www.ti.com Instruction Set
1111 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
Z=0;
If(Period C[24:0] !=1FF_FFFFh) {
Period C[24:0] =Period C[24:0] +1;
}
Register A=Period C[24:0];
If(specified capture edge detected onselected pin)
{
Z=1;
If(Period value !=1FF_FFFFh)
{
HRCapture Value =selected HRcounter;
}
else
{
HRCapture Value =7Fh;
}
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
}
If(specified reset edge detected onselected pin)
{
Period value =0000000h;
}
Prv bit =Current Logic (Lx) value ofselected pin;
Jump toNext Program Address;
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1112 -->

Instruction Set www.ti.com
1112 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.17 PWCNT (Pulse Width Count)
Syntax PWCNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[hr_lr={HIGH |LOW}]
[control={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}
[en_pin_action={OFF |ON}]
pin={pin number}
[action={CLEAR |SET |PULSELO |PULSEHI}]
[reg={A |B|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data={7-bit unsigned integer}]
}
Figure 23-167. PWCNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 65 0
0 Request
NumberBRK Next program address 1010 hr_lr 11 Reserved
6 3 1 9 4 1 2 6
Figure 23-168. PWCNT Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 5 4 3 2 1 0
Conditional address Pinselect Reserved Action Register select Int.
ena
9 5 3 2 2 1
Figure 23-169. PWCNT Data Field (D31:D0)
31 76 0
Data HRData
25 7

<!-- Page 1113 -->

www.ti.com Instruction Set
1113 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleCycles One
Register modified Selected register (A,BorT)
Description This instruction defines avirtual timer used togenerate variable length pulses.
The counter value stored inthedata field isdecremented unconditionally on
each timer resolution until itreaches zero, anditthen stays atzero until itis
reloaded with anon-zero value.
The specified pinaction isperformed aslong asthecount after count value is
decremented isgreater than 0.The opposite pinaction isperformed when the
count after decrement justreaches 0.
Ifthehr_lr bitisreset, theopposite pinaction willbetaken after aHRdelay
from thenext loop resolution clock. Ifthehr_lr bitisset,thedelay isignored.
This delay isprogrammed inbits[D6:D0].
irq ONgenerates aninterrupt when thedata field value reaches 0.No
interrupt isgenerated forOFF.
Default: OFF.
data 25-bit integer value serving asacounter.
hr_data HRdelay.
Default: 0.

<!-- Page 1114 -->

Instruction Set www.ti.com
1114 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
If(Data field value ==0)
{
Selected register =0;
Jump toNext Program Address;
}
If(Data field value >1)
{
Selected register =Data field value -1;
Data field value =Counter value -1;
If(Enable Pin action ==1)
{
Selected Pin =Pin Action ATnext loop resolution clock;
}
Jump toNext Program Address;
}
If(Data field value ==1)
{
Selected register =0000000h;
Data field value =0000000h;
If(Opposite action ==1)
{
If(hr_lr bit ==0)
{
If(Enable Pin action ==1)
{
Selected Pin =Opposite level ofPin Action ATnext loop resolution clock
+HRdelay;
}
}
else
{
If(Enable Pin action ==1)
{
Selected Pin =Opposite level ofPin Action ATnext loop
resolution clock;
}
}
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
}
Jump toConditional Address
}
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1115 -->

www.ti.com Instruction Set
1115 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.18 RADM64 (Register Add Move 64)
Syntax RADM64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
[pin={pin number}]
comp_mode={ECMP |SCMP |MCMP1 |MCMP2}
[action={CLEAR |SET |PULSELO |PULSEHI}]
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}
}
-or-
Syntax RADM64 {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
remote={label |9-bit unsigned integer}
cntl_val={29-bit unsigned integer}
[data={25-bit unsigned integer]
[hr_data= {7-bit unsigned integer}
}

<!-- Page 1116 -->

+
=Immediate DF
Register A, B, R, S, or T
Remote DF32 bitsLSBs (HR data field)
Immediate CF
Remote CFHR
HR(dashed for R, S, T)HR
Instruction Set www.ti.com
1116 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleFigure 23-170. RADM64 Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Reserved BRK Next program address 0011 Remote Address
6 3 1 9 4 9
Figure 23-171. RADM64 Control Field (C31:C0)
31 29 28 27 26 25 23 22 21 16
Reserved Request type Control Reserved En.pin
actionConditional address
3 2 1 3 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegComp. mode Action Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-172. RADM64 Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles Normally One Cycle. Two cycles ifwriting toremote address thatisalso the
next address.
Register modified None
Description This instruction modifies thedata field, theHRdata field andthecontrol field
attheremote address. The advantage over DADM64 isthatItexecutes one
cycle faster. Incase theR,S,orTregister isselected, theaddition isa32-bit
addition. The table description shows thebitencoding fordetermining which
ALU register isselected.
RADM64 hastwodistinct syntaxes. Inthefirstsyntax, bitvalues may beset
byassigning avalue toeach ofthecontrol fields. This syntax isconvenient for
modifying control fields thatarearranged similar totheformat oftheRADM64
control field. Asecond syntax, inwhich theentire 29-bit control field is
specified bythecntl_val field, isconvenient when theremote control field is
dissimilar from theRADM64 control field. Either syntax may beused, butyou
must useoneortheeither butnotacombination ofsyntaxes. See Figure 23-
173.
Figure 23-173. RADM64 Add andMove Operation
comp_mode Selects thecomparison mode type tobeused.

<!-- Page 1117 -->

www.ti.com Instruction Set
1117 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-91. Comparison Type Encoding Format
comp_mode C[6] C[5] MCMP Order
ECMP 0 0
SCMP 0 1
MCMP1 1 0 REG_GE_DATA
MCMP2 1 1 DATA_GE_REG
Table 23-92. RADM64 Control Field Descriptions
request Maintains thecontrol field fortheremote instruction.
Control Maintains thecontrol field fortheremote instruction.
en_pin_action Maintains thecontrol field fortheremote instruction.
cond_addr Maintains thecontrol field fortheremote instruction.
pin Maintains thecontrol field fortheremote instruction.
register Maintains thecontrol field fortheremote instruction.
action Maintains thecontrol field fortheremote instruction.
irq Maintains thecontrol field fortheremote instruction.
data Specifies the25-bit initial value forthedata field. Ifomitted, thefield defaults to0.
hr_dataSeven least significant bitsofthe32-bit data field.
Default: 0.
cntl_val Specifies the29least significant bitsoftheControl field.
Execution
Remote Data Field =Selected register +Immediate Data Field (including HRfield);
Remote Control Field =Immediate Control Field;
Jump toNext Program Address;

<!-- Page 1118 -->

/c247
/c248/c246/c231
/c232/c230/c183 /c61MNT TferenceR e Input
Instruction Set www.ti.com
1118 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.19 RCNT (Ratio Count)
Syntax RCNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[control={OFF |ON}]
divisor={25-bit unsigned integer}
[data={25-bit unsigned integer]
}
Figure 23-174. RCNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 76 5 4 3 10
0 Reserved BRK Next program address 1010 Res. 00 Step
widthRes. 1
6 3 1 9 4 1 2 2 3 1
Figure 23-175. RCNT Control Field (C31:C0)
31 27 26 25 24 0
Reserved Control Res. Divisor
5 1 1 25
Figure 23-176. RCNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles Two Cycles (One Cycle ifT=0)
Register modified None
Description RCNT isused with other instructions toconvert aninput period measurement
TInputtotheform of(Equation 31)where theinput period isexpressed asa
fraction ofareference period TReference .
(31)
RCNT computes thenumerator Nof(Equation 31).The denominator Mof
(Equation 31)isaconstant thatisofinterest. Forexample, choosing M=100
allows theinput period tobeexpressed asapercentage (%)ofthereference
period. Note thatifTInput>TReference ,then RCNT willreturn N>M;which would
becorrect if,forexample, theinput pulse period is110% ofthereference
pulse period.
RCNT expects thatregister Tisloaded with thevalue ofTReference .The input
period TInputisdetermined bycounting thenumber ofloop resolution periods
between edges ontheinput pin.This information isconveyed through theZ
flagfrom aPCNT instruction thatprecedes theRCNT instruction.
The divisor field oftheRCNT instruction should bechosen as:
Divisor =M· lr,where Misthedesired denominator from
(Equation 31)andlristheloop resolution prescale value.

<!-- Page 1119 -->

www.ti.com Instruction Set
1119 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleAnexample N2HET program thatmakes useoftheRCNT instruction is:
L0: MOV32 {remote=dummy,type=IMTOREG,reg=T,data=0x8,hr_data=0};
L1: PCNT {hr_lr=HIGH,brk=OFF,type=FALL2FALL,pin=0};
L2: RCNT {divisor=320,data=0x4};
L3: BR{cond_addr=L5, event =Z}
L4: ADC {src1=ZERO,src2=IMM,dest=IMM,next=L0,data=0,hr_data=0};
L5: ADD {src1=REM,src2=ZERO,dest=IMM,remote=L4,data=0,hr_data=0};
L6: ADD {src1=ZERO,src2=ZERO,dest=NONE,rdest=REM,
next=L0,remote=L4,data=0,hr_data=0};
dummy
Inthissmall program aninput signal onpin0ismeasured both interms ofabsolute cycles bythePCNT
instruction atL1andasin1/10ths ofthereference period bytheRCNT instruction atL2.Inthisexample
thereference period isaconstant 0x400 cycles; thisvalue isloaded intoregister TbytheMOV32
instruction atL0.(0x400 isdata=8, hr_data=0)
RCNT follows PCNT andisinitialized toaworking count ofT/2(0x200) whenever thePCNT instruction
detects afalling edge onpin0.Between falling edges onpin0, RCNT accumulates counts 10xfaster than
PCNT; sothattheworking data field ofRCNT willreach thereference value of0x400 in1/10th thetime
thataPCNT instruction would. Each time theRCNT instruction passes thereference value, itsets the
carry outflagandsubtracts thereference value from theworking count. Byaccumulating carry-outs from
RCNT, theaddwith carry instruction atL4effectively counts inincrements of1/10th ofthereference
period. Note thatthedivisor value 320is10times 32;thisassumes lr=32.
When thenext falling edge isdetected onpin0,PCNT sets theZflagandtheRCNT instruction resets
again totheinitial data field ofT/2. RCNT does notmodify theZflag, sothatthebranch instruction atL3
canexecute instructions atL5,L6instead ofL4.The instructions atL5andL6capture thefinal result from
L4andreset theADC instruction atL4tozero forthestart ofthenext period measurement.
Execution
If(register T[31:0] !=00000000h)
{
C=0;
If(Z==0)
{
Data Field[31:0] =Data Field[31:0] +Divisor[24:0];
If(Data Field[31:0] >=Reg T[31:0])
{
Data Field[31:0]=Data Field[31:0] -Reg T[31:0];
C=1;
}
}
else
{
Data Field[31:0] =T[31:0] >>1;/*T/2 */
}
}
Jump toNext Program Address;

<!-- Page 1120 -->

Instruction Set www.ti.com
1120 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.20 SCMP (Sequence Compare)
Syntax SCMP {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[en_pin_action={OFF |ON}]
cond_addr={label |9-bit unsigned integer}
pin={pin number}
[action={CLEAR |SET}]
[restart={OFF |ON}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
}
Figure 23-177. SCMP Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Request
NumberBRK Next program address 0000 Reserved
6 3 1 9 4 9
Figure 23-178. SCMP Control Field (C31:C0)
31 29 28 27 26 25 24 23 22 21 16
Reserved Request type Control Cout
prvReserved En.pin
actionConditional address
3 2 1 1 2 1 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Res. 01 Action Reserved Restart
enableInt.
ena
9 5 1 2 1 2 1 1
Figure 23-179. SCMP Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One
Register modified Register T(implicitly)
Description This instruction alternately performs angle- andtime-based operations to
generate pulse sequences, using theangle referenced time base. These
pulse sequences lastforarelative duration using afree running time base.
Generally, register Bholds theangle values andregister Aholds thetime
values. Bit0oftheconditional address field (C13) specifies whether the
instruction isoperating inangle ortime operation mode.

<!-- Page 1121 -->

www.ti.com Instruction Set
1121 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleWhen thecompared values match inangle mode, apincanbesetorreset
according tothepinaction bit(C4). The pindoes notchange states ifthe
enable pinaction bit(C22) isreset.
The restart enable bit(C1) provides theoption tounconditionally restart a
sequence using theX-flag bitofACMP.
restart Ifrestart issettoONandtheXflag=1,theassembler writes avalue
of1intotheimmediate index field, writes thevalue inregister Ainto
theimmediate data field, andjumps tothenext program address. The
Xflagissetorcleared bytheACMP instruction. Ifrestart issetto
OFF, theXflagisignored; nospecial action isperformed.
Default: OFF.
irq ONgenerates aninterrupt ifthecompare match occurs inangle
mode. Nointerrupt isgenerated when thefield isOFF.
Default: OFF.
data Specifies the25-bit compare value.
cond_addr Since theLSB oftheconditional address isused toselect between
time mode andangle mode, andsince theconditional address is
taken only intime mode, thedestination fortheconditional address
must beodd.
Execution
If(Data field value <=Selected register value) Cout =0;else Cout =1;
If(Restart Enable ==1AND X==1)
{
C13 =1;
Immediate Data Field =Register A;
Cout =0;
Jump toNext Program Address;
}
If(Angle Mode (C13 ==0)AND ((Restart En. ==1AND X==0)ORRestart En. ==0))
{
If(Z==0AND (Register Bvalue -Angle Inc. <Data field value) AND Cout ==0)OR
(Z==1AND (Cout_prv ==1ORCout ==0)))
{
If(Enable Pin Action ==1)Selected Pin =Pin Action;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Immediate Data Field =Register A;
C13 =1;/*** switch toTime Mode ***/
}
Jump toNext Program Address;
}
Else If(Time Mode (C13 ==1)) AND ((Restart En. ==1AND X==0)ORRestart En. ==0)
{
/*Result ofsubtract must not exceed 2^24 -1*/
Register T=Register A-Immediate Data Field;
Jump toConditional Program Address;
}
Cout_prv =Cout; (always executed)
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1122 -->

Instruction Set www.ti.com
1122 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.21 SCNT (Step Count)
Syntax SCNT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
step={8 |16|32|64}
[control={OFF |ON}]
gapstart={25-bit unsigned integer}
[data={25-bit unsigned integer]
}
Figure 23-180. SCNT Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 76 5 4 3 1 0
0 Reserved BRK Next program address 1010 Res. 00 Step
widthRes. 1
6 3 1 9 4 1 2 2 3 1
Figure 23-181. SCNT Control Field (C31:C0)
31 27 26 25 24 0
Reserved Control Res. Gap start
5 1 1 25
Figure 23-182. SCNT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One ortwocycles (two cycles when DFisinvolved inthecalculations)
Register modified Register A
Description This instruction canbeused only once inaprogram anddefines aspecialized
virtual timer used after APCNT andbefore ACNT togenerate anangle-
referenced time base synchronized toanexternal signal (that is,atoothed
wheel signal) asdefined inAPCNT andACNT. Step width selection bitsare
saved intwoflags, SWF0, andSWF1, tobere-used inACNT.
SCNT multiplies thefrequency oftheexternal signal byaconstant Kdefined
inthestep width field, [P5:P4]. The bitencoding forthisfield isdefined in
Table 23-93 .
step Specifies thestep increment tobeadded tothecounter value each
program resolution. These twobitsprovide thevalues fortheSWF0
andSWF1 flags. The valid values arelisted inTable 23-93 .

<!-- Page 1123 -->

www.ti.com Instruction Set
1123 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleTable 23-93. Step Width Encoding forSCNT
P5 P4 Step Width (K)
0 0 8
0 1 16
1 0 32
1 1 64
gapstart Defines thegapstart angle, which SCNT writes toregister A.The gap
start value hasnoeffect ontheSCNT instruction, butiftheACNT
instruction isbeing used, register Amust contain thecorrect gapstart
value. Foratypical toothed wheel gear:
GAPSTART =(stepwidth ×(actual teeth ongear -1))+1.
data Specifies the25-bit integer value serving asacounter.
Default: 0.
This instruction isincremented bythestep value Koneach timer resolution uptotheprevious period
value P(n-1) measured byAPCNT (stored inregister T).The resulting period ofSCNT is:P(n-1)/K
Due tostepping, thefinal count ofSCNT willnotusually exactly match thetarget p(n-1). SCNT
compensates forthiserror bystarting each cycle with theremainder oftheprevious cycle.
When SCNT reaches thetarget p(n-1), thezero flagissetasanincrement condition forACNT.SCNT also
specifies agapstart angle, defining thestart ofarange inACNT where period measurements inAPCNT
aretemporarily stopped tomask singularities intheexternal signal.
SCNT uses register Atostore thegapstart value. Gap start hasnoeffect forSCNT.
Execution
SWF1 =P5;
SWF0 =P4;
Z=0;
If(register T!=0000000h)
{
If(DCF ==1ORACF ==1)
{
Data Field register =0000000h;
Counter value =0000000h;
}
If(DCF ==0AND ACF ==0)
{
Data Field register =Data field register +Step Width;
}
If((Data Field register -register T)>=0)
{
Data field register =Data Field register -register T;
Z=1;
}
Register A=Gap start value;
}
Jump toNext Program Address;

<!-- Page 1124 -->

Instruction Set www.ti.com
1124 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.22 SHFT (Shift)
Syntax SHFT {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
smode={OR0 |OL0 |OR1 |OL1 |ORZ |OLZ |IRM |ILL|IRZ|ILZ}
[control={OFF |ON}]
[prv={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}
cond={UNC |FALL |RISE}
pin={pin number}
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[data={25-bit unsigned integer]
}
Figure 23-183. SHFT Program Field (P31:P0)
31 2625 23 22 21 1312 98 43 0
0 Request
NumberBRK Next program address 1111 Reserved Smode
6 3 1 9 4 5 4
Figure 23-184. SHFT Control Field (C31:C0)
31 29 28 27 26 25 24 22 21 16
Reserved Request type Control Prv. Reserved Conditional address
3 2 1 1 3 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegShift condition Res. 0 Register select Int.
ena
9 5 1 2 1 1 2 1
Figure 23-185. SHFT Data Field (D31:D0)
31 76 0
Data Reserved
25 7
Cycles One
Register modified Selected register (A,B,R,SorT)
Description This instruction shifts thedata field oftheInstruction. N2HET pins canbe
used fordata inordata out.SHFT includes parameters toselect theshift
direction (in,out,left,right), shift condition (shift onadefined clock edge on
HET[0] orshift always), register fordata storage (A,B,R,SorT),andthe
data pin.

<!-- Page 1125 -->

www.ti.com Instruction Set
1125 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Modulesmode Shift mode
Table 23-94. SHIFT MODE Encoding Format
smode P3 P2 P1 P0 Operation
OR0 0 0 0 0 Shift Out/Right LSB 1stonHETx /0intoMSB
OL0 0 0 0 1 Shift Out/Left MSB 1stonHETx /0intoLSB
OR1 0 0 1 0 Shift Out/Right LSB 1stonHETx /1intoMSB
OL1 0 0 1 1 Shift Out/Left MSB 1stonHETx /1intoLSB
ORZ 0 1 0 0 Shift Out/Right LSB 1stonHETx /ZintoMSB
OLZ 0 1 0 1 Shift Out/Left MSB 1stonHETx /ZintoLSB
IRM 1 0 0 0 Shift In/Right HETx intoMSB
ILL 1 0 0 1 Shift In/Left HETx intoLSB
IRZ 1 0 1 0 Shift In/Right HETx inMSB /LSB intoZ
ILZ 1 0 1 1 Shift In/Left HETx inLSB /MSB intoZ
cond Specifies theshift condition.
Table 23-95. SHIFT Condition Encoding
C6 C5 Shift Condition
0 X Always
1 0 Rising edge ofHET[0]
1 1 Falling edge ofHET[0]
irq ONgenerates aninterrupt iftheZflagisset.Avalue ofOFF does not
generate aninterrupt.
Default: OFF.
data Specifies the25-bit value forthedata field.

<!-- Page 1126 -->

Instruction Set www.ti.com
1126 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) ModuleExecution
If(SHIFT condition ==0X)
OR(SHIFT condition ==10AND HET[0] rising edge)
OR(SHIFT condition ==11AND HET[0] falling edge)
{
If([P3:P2] ==00)
{
If((Immediate Data Field ==all 0'sAND [P3:P0] ==000X)
OR(Immediate Data Field ==all 1'sAND [P3:P0] ==001X))
{
Z=1;
}
else
{
Z=0;
}
}
else If([P3:P0] ==1010)
{
Z=LSB ofthe Immediate Data Field;
}
else if([P3:P0] ==1011)
{
Z=MSB ofthe Immediate Data Field;
}
}
If( (Immediate Data Field ==all 0's)OR
(Immediate Data Field ==all 1's))
{
if(Interrupt Enable ==1)
{
HETFLG[n] =1; /*ndepends onaddress */
}
Jump toConditional Address;
}
else
{
Jump toNext Program Address;
}
Prv. bit =HET[0] Pin level; (Always executed)
Shift Immediate Data Field once according toP[3:0];
Immediate Data Field =Result ofthe shift;
Selected register =Result ofthe shift;
Jump toNext Program Address;
NOTE: The immediate data field evaluates all0sorall1sandisperformed before theshift
operation.
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1127 -->

www.ti.com Instruction Set
1127 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.23 WCAP (Software Capture Word)
Syntax WCAP {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[hr_lr={HIGH |LOW}]
[control={OFF |ON}]
[prv={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}]
pin={pin number}
event={NOCOND |FALL |RISE |BOTH}
reg={A |B|R|S|T|NONE}
[irq={OFF |ON}]
[data={25-bit unsigned integer]
[hr_data={7-bit unsigned integer}]
}
Figure 23-186. WCAP Program Field (P31:P0)
31 2625 23 22 21 1312 9 8 7 0
0 Request
NumberBRK Next program address 1011 hr_lr Reserved
6 3 1 9 4 1 8
Figure 23-187. WCAP Control Field (C31:C0)
31 29 28 27 26 25 24 22 21 16
Reserved Request type Control Prv. Reserved Conditional address
3 2 1 1 3 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegCapture
conditionReserved Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-188. WCAP Data Field (D31:D0)
31 76 0
Data HRData
25 7
Cycles One
Register modified None
Description This instruction captures theselected register intothedata field ifthespecified
capture condition istrue ontheselected pin.This instruction canbeused with
allpins.
Ifthehr_lr bitisreset, theWCAP instruction willcapture anHRtime stamp
intothedata field ontheselected edge condition. Ifthehr_lr bitisset,theHR
capture isignored.

<!-- Page 1128 -->

Instruction Set www.ti.com
1128 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleevent Specifies theevent thattriggers thecapture.
Table 23-96. Event Encoding Format forWCAP
C6 C5 Capture Condition
0 0 Always
0 1 Capture onfalling edge
1 0 Capture onrising edge
1 1 Capture onrising andfalling edge
irq ONgenerates aninterrupt when thecapture condition ismet. No
interrupt isgenerated forOFF.
Default: OFF.
data Specifies the25-bit integer value tobewritten tothedata field or
selected register.
hr_data HRcapture value.
Default: 0.
NOTE: WCAP inHRMode: The HRCounter starts onaWCAP instruction execution (inthefirst
loop clock) andwillsynchronize tothenext loop clock. When N2HET isturned onanda
capture edge occurs inthefirstloop clock (where theHRcounter hasn 'tbeen synchronized
totheloop clock), then thecaptured HRcounter value iswrong andisofnouse. Sothe
captured HRdata inthefirstloop clock should beignored.
Execution
If(Specified Capture Condition istrue onSelected Pin
ORUnconditional capture isselected)
{
Immediate Data Field =Selected register value;
If(hr_lr bit ==0)Capture the HRvalue inImmediate HRData Field;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Jump toConditional Address;
}
Jump toNext Program Address;
Prv bit =Current Logic (Lx) value ofselected pin; (always executed)
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .

<!-- Page 1129 -->

www.ti.com Instruction Set
1129 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Module23.6.3.24 WCAPE (Software Capture Word andEvent Count)
Syntax WCAPE {
[brk={OFF |ON}]
[next={label |9-bit unsigned integer}]
[reqnum={3-bit unsigned integer}
[request={NOREQ |GENREQ |QUIET}]
[control={OFF |ON}]
[prv={OFF |ON}]
[cond_addr={label |9-bit unsigned integer}
pin={pin number}
event={NOCOND |FALL |RISE |BOTH}
[reg={A |B|R|S|T|NONE}]
[irq={OFF |ON}]
[ts_data={25-bit unsigned integer]
[ec_data={7-bit unsigned integer}]
}
Figure 23-189. WCAPE Program Field (P31:P0)
31 2625 23 22 21 1312 98 0
0 Request
NumberBRK Next program address 1000 Reserved
6 3 1 9 4 9
Figure 23-190. WCAPE Control Field (C31:C0)
31 29 28 27 26 25 24 23 22 21 16
Reserved Request type Control Prv. Reserved Conditional address
3 2 1 1 3 9
15 13 12 8 7 6 5 4 3 2 1 0
Conditional address Pinselect Ext
RegCapture
conditionReserved Register select Int.
ena
9 5 1 2 2 2 1
Figure 23-191. WCAPE Data Field (D31:D0)
31 76 0
Time Stamp Edge Counter
25 7
Cycles One
Register modified None
Description This instruction captures theselected register intothedata field [D31:D7] and
increments anevent counter [D6:D0] ifthespecified capture condition istrue
ontheselected pin.This instruction canbeused with allpins, butthetime
stamp [D31:D7] hasloop resolution only.

<!-- Page 1130 -->

Instruction Set www.ti.com
1130 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedHigh-End Timer (N2HET) Moduleevent Specifies theevent thattriggers thecapture.
Table 23-97. Event Encoding Format forWCAPE
C6 C5 Capture Condition
0 0 Always
0 1 Capture onfalling edge
1 0 Capture onrising edge
1 1 Capture onrising andfalling edge
irq ONgenerates aninterrupt when thecapture condition ismet. No
interrupt isgenerated forOFF.
Default: OFF.
ts_data Specifies the25-bit integer value for[D31:D7]
Default: 0.
ec_data Specifies theinitial 7-bit integer value for[D6:D0].
Default: 0.
Execution
If(Specified Capture Condition istrue onSelected Pin
ORUnconditional capture isselected)
{
Immediate Data Field[31:7] =Selected register value;
Immediate Data Field [6:0] =Immediate Data Field [6:0] +1;
If(Interrupt Enable ==1)HETFLG[n] =1; /*ndepends onaddress */
If([C28:C27] ==01) Generate request onrequest line [P25:P23];
If([C28:C27] ==11) Generate quiet request onrequest line [P25:P23];
Jump toConditional Address;
}
Jump toNext Program Address;
Prv bit =Current Logic (Lx) value ofselected pin; (always executed)
The specific interrupt flagthatistriggered depends ontheaddress from which theinstruction isexecuted,
seeSection 23.2.7 .