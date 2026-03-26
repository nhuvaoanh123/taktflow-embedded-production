# External Memory Interface (EMIF)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 793-847

---


<!-- Page 793 -->

793 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Chapter 21
SPNU563A -March 2018
External Memory Interface (EMIF)
This chapter describes theexternal memory Interface (EMIF).
Topic ........................................................................................................................... Page
21.1 Introduction ..................................................................................................... 794
21.2 EMIF Module Architecture ................................................................................. 796
21.3 EMIF Registers ................................................................................................. 828
21.4 Example Configuration ...................................................................................... 840

<!-- Page 794 -->

Introduction www.ti.com
794 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.1 Introduction
21.1.1 Purpose ofthePeripheral
This EMIF memory controller iscompliant with theJESD21-C SDR SDRAM memories utilizing a16-bit
data bus. The purpose ofthisEMIF istoprovide ameans fortheCPU toconnect toavariety ofexternal
devices including:
*Single data rate (SDR) SDRAM
*Asynchronous devices including NOR Flash andSRAM
The most common usefortheEMIF istointerface with both aflash device andanSDRAM device
simultaneously. Section 21.4 contains anexample ofoperating theEMIF inthisconfiguration.
21.1.2 Features
The EMIF includes many features toenhance theease andflexibility ofconnecting toexternal SDR
SDRAM andasynchronous devices.
21.1.2.1 Asynchronous Memory Support
EMIF supports asynchronous:
*SRAM memories
*NOR Flash memories
The EMIF data buswidth isupto16bitsandthere areupto22address lines. There isanexternal wait
input thatallows slower asynchronous memories toextend thememory access. The EMIF module
supports upto3chip selects (EMIF_nCS[4:2]). Each chip select hasthefollowing individually
programmable attributes:
*Data Bus Width
*Read cycle timings: setup, hold, strobe
*Write cycle timings: setup, hold, strobe
*Bus turn-around time
*Extended Wait Option with Programmable Timeout
*Select Strobe option
21.1.2.2 Synchronous DRAM Memory Support
The EMIF module supports 16-bit SDRAM inaddition totheasynchronous memories listed in
Section 21.1.2.1 .Ithasasingle SDRAM chip select (EMIF_nCS[0]). SDRAM configurations thatare
supported are:
*One, Two andFour Bank SDRAM devices
*Devices with Eight, Nine, Ten, andEleven Column Address
*CAS Latency oftwoorthree clock cycles
*16-bit Data Bus Width
*3.3V LVCMOS Interface
Additionally, theEMIF supports placing theSDRAM inSelf-Refresh andPowerdown modes. Self-refresh
mode allows theSDRAM tobeputinalow-power state while stillretaining memory contents; since the
SDRAM willcontinue torefresh itself even without clocks from themicrocontroller. Powerdown mode
achieves even lower power, except themicrocontroller must periodically wake upandissue refreshes if
data retention isrequired.
Note thattheEMIF module does notsupport Mobile SDRAM devices.

<!-- Page 795 -->

EMIF_nCS[0]
EMIF_nCAS
EMIF_nRAS
EMIF_CLK
EMIF_CKE
EMIF_nCS[4:2]
EMIF_nOE
EMIF_nWAIT
EMIF_nWE
EMIF_BA[1:0]
EMIF_nDQM[1:0]
EMIF_DATA[15:0]
EMIF_ADDR[21:0]EMIF
CPU
EDMA
Master
PeripheralsSDRAM
interface
Asynchronous
interface
Shared SDRAM
and asynchronous
interface
www.ti.com Introduction
795 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.1.3 Functional Block Diagram
Figure 21-1 illustrates theconnections between theEMIF anditsinternal requesters, along with the
external EMIF pins. Section 21.2.2 contains adescription oftheentities internal totheSoC thatcansend
requests totheEMIF, along with their prioritization. Section 21.2.3 describes theEMIF external pins and
summarizes their purpose when interfacing with SDRAM andasynchronous devices.
Figure 21-1. EMIF Functional Block Diagram

<!-- Page 796 -->

EMIF Module Architecture www.ti.com
796 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2 EMIF Module Architecture
This section provides details about thearchitecture andoperation oftheEMIF. Both, SDRAM and
asynchronous Interface arecovered, along with other system-related issues such asclock control.
21.2.1 EMIF Clock Control
The EMIF clock isoutput ontheEMIF_CLK pinandshould beused when interfacing toexternal SDRAM
devices. The EMIF module gets theVCLK3 clock domain astheinput. This clock domain isrunning athalf
thefrequency ofthemain oscillator bydefault, thatis,between 2.5MHz to10MHz. The VCLK3 frequency
isdivided down from theHCLK domain frequency byaprogrammable divider (/1to/16). Refer the
Architecture chapter ofthedevice technical reference manual formore information onconfiguring the
VCLK3 domain frequency.
21.2.2 EMIF Requests
Different sources within theSoC canmake requests totheEMIF. These requests consist ofaccesses to
SDRAM memory, asynchronous memory, andEMIF registers. The EMIF canprocess only onerequest at
atime. Therefore ahigh performance crossbar switch exists within theSoC toprovide prioritized requests
from thedifferent sources totheEMIF. The sources are:
1.CPU
2.DMA
3.Other master peripherals
Ifarequest issubmitted from twoormore sources simultaneously, thecrossbar switch willforward the
highest priority request totheEMIF first. Upon completion ofarequest, thecrossbar switch again
evaluates thepending requests andforwards thehighest priority pending request totheEMIF.
When theEMIF receives arequest, itmay ormay notbeimmediately processed. Insome cases, the
EMIF willperform oneormore auto refresh cycles before processing therequest. Fordetails onthe
EMIF's internal arbitration between performing requests andperforming auto refresh cycles, see
Section 21.2.13 .
21.2.3 EMIF Signal Descriptions
This section describes thefunction ofeach oftheEMIF signals.
Table 21-1. EMIF Pins Used toAccess Both SDRAM andAsynchronous Memories
Pins(s) I/O Description
EMIF_DATA[15:0] I/O EMIF data bus.
EMIF_ADDR[21:0] O EMIF address bus.
When interfacing toanSDRAM device, these pins areprimarily used toprovide therowand
column address totheSDRAM. The mapping from theinternal program address totheexternal
values placed onthese pins canbefound inTable 21-13 .EMIF_A[10] isalso used during the
PRE command toselect which banks todeactivate.
When interfacing toanasynchronous device, these pins areused inconjunction with the
EMIF_BA pins toform theaddress thatissent tothedevice. The mapping from theinternal
program address totheexternal values placed onthese pins canbefound inSection 21.2.6.1 .
EMIF_BA[1:0] O EMIF bank address.
When interfacing toanSDRAM device, these pins areused toprovide thebank address inputs to
theSDRAM. The mapping from theinternal program address totheexternal values placed on
these pins canbefound inTable 21-13 .
When interfacing toanasynchronous device, these pins areused inconjunction with theEMIF_A
pins toform theaddress thatissent tothedevice. The mapping from theinternal program
address totheexternal values placed onthese pins canbefound inSection 21.2.6.1 .
EMIF_nDQM[1:0] O Active-low byte enables.
When interfacing toSDRAM, these pins areconnected totheDQM pins oftheSDRAM to
individually enable/disable each ofthebytes inadata access.
When interfacing toanasynchronous device, these pins areconnected tobyte enables. See
Section 21.2.6 fordetails.

<!-- Page 797 -->

www.ti.com EMIF Module Architecture
797 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-1. EMIF Pins Used toAccess Both SDRAM andAsynchronous Memories (continued)
Pins(s) I/O Description
EMIF_nWE O Active-low write enable.
When interfacing toSDRAM, thispinisconnected tothenWE pinoftheSDRAM andisused to
send commands tothedevice.
When interfacing toanasynchronous device, thispinprovides asignal which isactive-low during
thestrobe period ofanasynchronous write access cycle.
Table 21-2. EMIF Pins Specific toSDRAM
Pin(s) I/O Description
EMIF_nCS[0] O Active-low chip enable pinforSDRAM devices.
This pinisconnected tothechip-select pinoftheattached SDRAM device andisused for
enabling/disabling commands. Bydefault, theEMIF keeps thisSDRAM chip select active, even if
theEMIF isnotinterfaced with anSDRAM device. This pinisdeactivated when accessing the
asynchronous memory bank andisreactivated oncompletion oftheasynchronous access.
EMIF_nRAS O Active-low rowaddress strobe pin.
This pinisconnected tothenRAS pinoftheattached SDRAM device andisused forsending
commands tothedevice.
EMIF_nCAS O Active-low column address strobe pin.
This pinisconnected tothenCAS pinoftheattached SDRAM device andisused forsending
commands tothedevice.
EMIF_CKE O Clock enable pin.
This pinisconnected totheCKE pinoftheattached SDRAM device andisused forissuing the
SELF REFRESH command which places thedevice inselfrefresh mode. See Section 21.2.5.7
fordetails.
EMIF_CLK O SDRAM clock pin.
This pinisconnected totheCLK pinoftheattached SDRAM device. See Section 21.2.1 for
details ontheclock signal.
Table 21-3. EMIF Pins Specific toAsynchronous Memory
Pin(s) I/O Description
EMIF_nCS[4:2] O Active-low chip enable pins forasynchronous devices.
These pins aremeant tobeconnected tothechip-select pins oftheattached asynchronous
device. These pins areactive only during accesses totheasynchronous memory.
EMIF_nWAIT I Wait input with programmable polarity.
Aconnected asynchronous device canextend thestrobe period ofanaccess cycle byasserting
theEMIF_nWAIT input totheEMIF asdescribed inSection 21.2.6.6 .Toenable thisfunctionality,
theEWbitintheasynchronous 1configuration register (CE2CFG) must besetto1.Inaddition,
theWP0 bitinCE2CFG must beconfigured todefine thepolarity oftheEMIF_nWAIT pin.
EMIF_nOE O Active-low pinenable forasynchronous devices.
This pinprovides asignal which isactive-low during thestrobe period ofanasynchronous read
access cycle.
21.2.4 EMIF Signal Multiplexing Control
Several EMIF signals aremultiplexed with other functions onthismicrocontroller. Please refer totheI/O
Multiplexing Module chapter ofthetechnical reference manual formore information onhow toenable the
output ofthese EMIF signals.

<!-- Page 798 -->

EMIF Module Architecture www.ti.com
798 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5 SDRAM Controller andInterface
The EMIF cangluelessly interface tomost standard SDR SDRAM devices andsupports such features as
selfrefresh mode andprioritized refresh. Inaddition, itprovides flexibility through programmable
parameters such astherefresh rate, CAS latency, andmany SDRAM timing parameters. The following
sections include details onhow toInterface andproperly configure theEMIF toperform read andwrite
operations toexternally connected SDR SDRAM devices. Also, Section 21.4 provides adetailed example
ofinterfacing theEMIF toacommon SDRAM device.
21.2.5.1 SDRAM Commands
The EMIF supports theSDRAM commands described inTable 21-4 .Table 21-5 shows thetruth table for
theSDRAM commands, andanexample timing waveform ofthePRE command isshown inFigure 21-2.
EMIF_A[10] ispulled lowinthisexample todeactivate only thebank specified bytheEMIF_BA pins.
Table 21-4. EMIF SDRAM Commands
Command Function
PRE Precharge. Depending onthevalue ofEMIF_A[10], thePRE command either deactivates theopen rowinall
banks (EMIF_A[10] =1)oronly thebank specified bytheEMIF_BA[1:0] pins (EMIF_A[10] =0).
ACTV Activate .The ACTV command activates theselected rowinaparticular bank forthecurrent access.
READ Read. The READ command outputs thestarting column address andsignals theSDRAM tobegin theburst read
operation. Address EMIF_A[10] isalways pulled lowtoavoid auto precharge. This allows forbetter bank
interleaving performance.
WRT Write. The WRT command outputs thestarting column address andsignals theSDRAM tobegin theburst write
operation. Address EMIF_A[10] isalways pulled lowtoavoid auto precharge. This allows forbetter bank
interleaving performance.
BT Burst terminate .The BTcommand isused totruncate thecurrent read orwrite burst request.
LMR Load mode register .The LMR command sets themode register oftheattached SDRAM devices andisonly
issued during theSDRAM initialization sequence described inSection 21.2.5.4 .
REFR Auto refresh .The REFR command signals theSDRAM toperform anauto refresh according toitsinternal
address.
SLFR Selfrefresh .The selfrefresh command places theSDRAM intoselfrefresh mode, during which itprovides itsown
clock signal andauto refresh cycles.
NOP Nooperation .The NOP command isissued during allcycles inwhich oneoftheabove commands isnotissued.
Table 21-5. Truth Table forSDRAM Commands
SDRAM Pins: CKE nCS nRAS nCAS nWE BA[1:0] A[12:11] A[10] A[9:0]
EMIF Pins: EMIF_CKE EMIF_nCS[0] EMIF_nRAS EMIF_nCAS EMIF_nWE EMIF_BA[1:0] EMIF_A[12:11] EMIF_A[10] EMIF_A[9:0]
PRE H L L H L Bank/X X L/H X
ACTV H L L H H Bank Row Row Row
READ H L H L H Bank Column L Column
WRT H L H L L Bank Column L Column
BT H L H H L X X X X
LMR H L L L L X Mode Mode Mode
REFR H L L L H X X X X
SLFR L L L L H X X X X
NOP H L H H H X X X X

<!-- Page 799 -->

EMIF_nCS[0]
EMIF_nCAS
EMIF_nRAS
EMIF_nWE
EMIF_CLK
EMIF_CKE
EMIF_BA[1:0]
EMIF_A[11:0]
EMIF_nDQM[0]
EMIF_nDQM[1]
EMIF_D[15:0]EMIF
nCE
nCAS
nRAS
nWE
CLK
CKE
BA[1:0]
A[11:0]
LDQM
UDQM
DQ[15:0]SDRAM
2M x 16
x 4 bank
PRE
EMIF_CLK
EMIF_nCS[0]
EMIF_BA
EMIF_A
EMIF_nRAS
EMIF_nCAS
EMIF_nWEBank
EMIF_A[10]=0EMIF_nDQM
www.ti.com EMIF Module Architecture
799 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-2. Timing Waveform ofSDRAM PRE Command
21.2.5.2 Interfacing toSDRAM
The EMIF supports aglueless interface toSDRAM devices with thefollowing characteristics:
*Pre-charge bitisA[10]
*The number ofcolumn address bitsis8,9,10,or11.
*The number ofrowaddress bitsis13,14,15,or16.
*The number ofinternal banks is1,2,or4.
Figure 21-3 shows aninterface between theEMIF anda2M×16×4bank SDRAM device, and
Figure 21-4 shows aninterface between theEMIF anda512K ×16×2bank SDRAM device. Fordevices
supporting 16-bit interface, refer toTable 21-6 forlistofcommonly-supported SDRAM devices andthe
required connections fortheaddress pins.
Figure 21-3. EMIF to2M×16×4bank SDRAM Interface

<!-- Page 800 -->

EMIF_nCS[0]
EMIF_nCAS
EMIF_nRAS
EMIF_nWE
EMIF_CLK
EMIF_CKE
EMIF_BA[0]
EMIF_A[10:0]
EMIF_nDQM[0]
EMIF_nDQM[1]
EMIF_D[15:0]EMIF
nCE
nCAS
nRAS
nWE
CLK
CKE
BA[0]
A[10:0]
LDQM
UDQM
DQ[15:0]SDRAM
512 x 16
x 2 bank
EMIF Module Architecture www.ti.com
800 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-4. EMIF to512K ×16×2bank SDRAM Interface
Table 21-6. 16-bit EMIF Address PinConnections
SDRAM Size Width Banks Device Address Pins
16M bits ×16 2 SDRAM A[10:0]
EMIF EMIF_A[10:0]
64M bits ×16 4 SDRAM A[11:0]
EMIF EMIF_A[11:0]
128M bits ×16 4 SDRAM A[11:0]
EMIF EMIF_A[11:0]
256M bits x16 4 SDRAM A[12:0]
EMIF EMIF_A[12:0]
512M bits x16 4 SDRAM A[12:0]
EMIF EMIF_A[12:0]

<!-- Page 801 -->

www.ti.com EMIF Module Architecture
801 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.3 SDRAM Configuration Registers
The operation oftheEMIF's SDRAM interface iscontrolled byprogramming theappropriate configuration
registers. This section describes thepurpose andfunction ofeach configuration register, butSection 21.3
should bereferred foramore detailed description ofeach register, including thedefault registers values
andbit-field positions. The following tables listthefour such configuration registers, along with a
description ofeach oftheir programmable fields.
NOTE: Writing toanyofthefields: NM, CL,IBANK, andPAGESIZE intheSDRAM configuration
register (SDCR) causes theEMIF toabandon whatever itiscurrently doing andtrigger the
SDRAM initialization procedure described inSection 21.2.5.4 .
Table 21-7. Description oftheSDRAM Configuration Register (SDCR)
Parameter Description
SR This bitcontrols entering andexiting oftheSelf-Refresh mode. The field should bewritten using abyte-
write totheupper byte ofSDCR toavoid triggering theSDRAM initialization sequence.
PD This bitcontrols entering andexiting ofthePower down mode. The field should bewritten using abyte-
write totheupper byte ofSDCR toavoid triggering theSDRAM initialization sequence. Ifboth SRand
PDbitsareset,theEMIF willgointoSelf Refresh.
PDWR Perform refreshes during Power Down. Writing a1tothisbitwillcause theEMIF toexitthepower down
state andissue anAUTO REFRESH command every time Refresh May level isset.The field should be
written using abyte-write totheupper byte ofSDCR toavoid triggering theSDRAM initialization
sequence. This bitshould besetalong with PDwhen entering power-down mode.
NM Narrow Mode. This bitdefines thewidth ofthedata busbetween theEMIF andtheattached SDRAM
device. When setto1,thedata busissetto16-bits. When setto0,thedata busissetto32-bits. This
bitmust always besetto1.
CL CAS latency. This field defines thenumber ofclock cycles between when anSDRAM issues aREAD
command andwhen thefirstpiece ofdata appears onthebus. The value inthisfield issent tothe
attached SDRAM device viatheLOAD MODE REGISTER command during theSDRAM initialization
procedure asdescribed inSection 21.2.5.4 .Only, values of2h(CAS latency =2)and3h(CAS latency =
3)aresupported andshould bewritten tothisfield. A1must besimultaneously written tothe
BIT11_9LOCK bitfield ofSDCR inorder towrite totheCLbitfield.
IBANK Number ofInternal SDRAM Banks. This field defines thenumber ofbanks inside theattached SDRAM
devices inthefollowing way:
*When IBANK =0,1internal bank isused
*When IBANK =1h,2internal banks areused
*When IBANK =2h,4internal banks areused
This field value affects themapping oflogical addresses toSDRAM row, column, andbank addresses.
See Section 21.2.5.11 fordetails.
PAGESIZE Page Size. This field defines theinternal page size oftheattached SDRAM devices inthefollowing way:
*When PAGESIZE =0,256-word pages areused
*When PAGESIZE =1h,512-word pages areused
*When PAGESIZE =2h,1024-word pages areused
*When PAGESIZE =3h,2048-word pages areused
This field value affects themapping oflogical addresses toSDRAM row, column, andbank addresses.
See Section 21.2.5.11 fordetails.
Table 21-8. Description oftheSDRAM Refresh Control Register (SDRCR)
Parameter Description
RR Refresh Rate .This field controls therate atwhich attached SDRAM devices willberefreshed. The
following equation canbeused todetermine therequired value ofRRforanSDRAM device:
*RR=fEMIF_CLK /(Required SDRAM Refresh Rate)
More information about theoperation oftheSDRAM refresh controller canbefound inSection 21.2.5.6 .

<!-- Page 802 -->

EMIF Module Architecture www.ti.com
802 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-9. Description oftheSDRAM Timing Register (SDTIMR)
Parameter Description
T_RFC SDRAM Timing Parameters. These fields configure theEMIF tocomply with theACtiming
requirements oftheattached SDRAM devices. This allows theEMIF toavoid violating SDRAM timing
constraints andtomore efficiently schedule itsoperations. More details about each ofthese parameters
canbefound intheregister description inSection 21.3.6 .These parameters should besettosatisfy the
corresponding timing requirements found intheSDRAM 'sdatasheet.T_RP
T_RCD
T_WR
T_RAS
T_RC
T_RRD
Table 21-10. Description oftheSDRAM SelfRefresh ExitTiming Register (SDSRETR)
Parameter Description
T_XS SelfRefresh ExitParameter. The T_XS field ofthisregister informs theEMIF about theminimum
number ofEMIF_CLK cycles required between exiting Self Refresh andissuing anycommand. This
parameter should besettosatisfy thetXSRvalue fortheattached SDRAM device.
21.2.5.4 SDRAM Auto-Initialization Sequence
The EMIF automatically performs anSDRAM initialization sequence, regardless ofwhether itisinterfaced
toanSDRAM device, when either ofthefollowing twoevents occur:
*The EMIF comes outofreset. Nomemory accesses totheSDRAM andAsynchronous interfaces are
performed until thisauto-initialization iscomplete.
*Awrite isperformed toanyofthethree least significant bytes oftheSDRAM configuration register
(SDCR)
AnSDRAM initialization sequence consists ofthefollowing steps:
1.Iftheinitialization sequence isactivated byawrite toSDCR, andifanyoftheSDRAM banks areopen,
theEMIF issues aPRE command with EMIF_A[10] held high toindicate allbanks. This isdone sothat
themaximum ACTV toPRE timing foranSDRAM isnotviolated.
2.The EMIF drives EMIF_CKE high andbegins continuously issuing NOP commands until eight SDRAM
refresh intervals have elapsed. AnSDRAM refresh interval isequal tothevalue oftheRRfield of
SDRAM refresh control register (SDRCR), divided bythefrequency ofEMIF_CLK (RR/fEMIF_CLK ).This
step isused toavoid violating thePower-up constraint ofmost SDRAM devices thatrequires 200μs
(sometimes 100μs)between receiving stable Vdd andCLK andtheissuing ofaPRE command.
Depending onthefrequency ofEMIF_CLK, thisstep may ormay notbesufficient toavoid violating the
SDRAM constraint. See Section 21.2.5.5 formore information.
3.After therefresh intervals have elapsed, theEMIF issues aPRE command with EMIF_A[10] held high
toindicate allbanks.
4.The EMIF issues eight AUTO REFRESH commands.
5.The EMIF issues theLMR command with theEMIF_A[9:0] pins setasdescribed inTable 21-11 .
6.Finally, theEMIF performs arefresh cycle, which consists ofthefollowing steps:
a.Issuing aPRE command with EMIF_A[10] held high ifanybanks areopen
b.Issuing anREF command

<!-- Page 803 -->

www.ti.com EMIF Module Architecture
803 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-11. SDRAM LOAD MODE REGISTER Command
EMIF_A[9:7] EMIF_A[6:4] EMIF_A[3] EMIF_A[2:0]
0(Write bursts areof
theprogrammed burst
length in
EMIF_A[2:0])These bitscontrol theCAS latency ofthe
SDRAM andaresetaccording toCLfield in
theSDRAM configuration register (SDCR)
asfollows:
*IfCL=2,EMIF_A[6:4] =2h
(CAS latency =2)
*IfCL=3,EMIF_A[6:4] =3h
(CAS latency =3)0(Sequential Burst
Type. Interleaved
Burst Type not
supported)These bitscontrol theburst length ofthe
SDRAM andaresetaccording totheNM
field intheSDRAM configuration register
(SDCR) asfollows:
*IfNM=0,EMIF_A[2:0] =2h
(Burst Length =4)
*IfNM=1,EMIF_A[2:0] =3h
(Burst Length =8)
21.2.5.5 SDRAM Configuration Procedure
There aretwodifferent SDRAM configuration procedures. Although EMIF automatically performs the
SDRAM initialization sequence described inSection 21.2.5.4 when coming outofreset, itisrecommended
tofollow oneoftheprocedures listed below before performing anyEMIF memory requests. Procedure A
should befollowed ifitisdetermined thattheSDRAM Power-up constraint was notviolated during the
SDRAM Auto-Initialization Sequence detailed inSection 21.2.5.4 oncoming outofReset. The SDRAM
Power-up constraint specifies that200μs(sometimes 100μs)should exist between receiving stable Vdd
andCLK andtheissuing ofaPRE command. Procedure Bshould befollowed iftheSDRAM Power-up
constraint was violated. The 200μs(100μs)SDRAM Power-up constraint willbeviolated ifthefrequency
ofEMIF_CLK isgreater than 50MHz (100 MHz for100μsSDRAM power-up constraint) during SDRAM
Auto-Initialization Sequence. Procedure Bshould befollowed ifthere isanydoubt thatthePower-up
constraint was notmet.
Procedure A--Following istheprocedure tobefollowed iftheSDRAM Power-up constraint was NOT
violated:
1.Place theSDRAM intoSelf-Refresh Mode bysetting theSRbitofSDCR to1.Abyte-write totheupper
byte ofSDCR should beused toavoid restarting theSDRAM Auto-Initialization Sequence described in
Section 21.2.5.4 .The SDRAM should beplaced intoSelf-Refresh mode when changing thefrequency
ofEMIF_CLK toavoid incurring the200μsPower-up constraint again.
2.Configure thedesired EMIF_CLK clock frequency. The frequency ofthememory clock must meet the
timing requirements intheSDRAM manufacturer's documentation andthetiming limitations shown in
theelectrical specifications ofthedevice datasheet.
3.Remove theSDRAM from Self-Refresh Mode byclearing theSRbitofSDCR to0.Abyte-write tothe
upper byte ofSDCR should beused toavoid restarting theSDRAM Auto-Initialization Sequence
described inSection 21.2.5.4 .
4.Program SDTIMR andSDSRETR tosatisfy thetiming requirements fortheattached SDRAM device.
The timing parameters should betaken from theSDRAM datasheet.
5.Program theRRfield ofSDRCR tomatch thatoftheattached device's refresh interval. See
Section 21.2.5.6.1 details ondetermining theappropriate value.
6.Program SDCR tomatch thecharacteristics oftheattached SDRAM device. This willcause theauto-
initialization sequence inSection 21.2.5.4 tobere-run. This second initialization generally takes much
less time duetotheincreased frequency ofEMIF_CLK.
Procedure B--Following istheprocedure tobefollowed iftheSDRAM Power-up constraint was
violated:
1.Configure thedesired EMIF_CLK clock frequency. The frequency ofthememory clock must meet the
timing requirements intheSDRAM manufacturer's documentation andthetiming limitations shown in
theelectrical specifications ofthedevice datasheet.
2.Program SDTIMR andSDSRETR tosatisfy thetiming requirements fortheattached SDRAM device.
The timing parameters should betaken from theSDRAM datasheet.
3.Program theRRfield ofSDRCR such thatthefollowing equation issatisfied: (RR ×8)/(fEMIF_CLK )>
200μs(sometimes 100μs).Forexample, anEMIF_CLK frequency of100MHz would require setting
RRto2501 (9C5h) orhigher tomeet a200μsconstraint.

<!-- Page 804 -->

EMIF Module Architecture www.ti.com
804 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)4.Program SDCR tomatch thecharacteristics oftheattached SDRAM device. This willcause theauto-
initialization sequence inSection 21.2.5.4 tobere-run with thenew value ofRR.
5.Perform aread from theSDRAM toassure thatstep 5ofthisprocedure willoccur after theinitialization
process hascompleted. Alternatively, wait for200μsinstead ofperforming aread.
6.Finally, program theRRfield tomatch thatoftheattached device's refresh interval. See
Section 21.2.5.6.1 details ondetermining theappropriate value.
After following theabove procedure, theEMIF isready toperform accesses totheattached SDRAM
device. See Section 21.4 foranexample ofconfiguring theSDRAM interface.
21.2.5.6 EMIF Refresh Controller
AnSDRAM device requires thateach ofitsrows berefreshed ataminimum required rate. The EMIF can
meet thisconstraint byperforming auto refresh cycles atorabove thisrequired rate. Anauto refresh cycle
consists ofissuing aPRE command toallbanks oftheSDRAM device followed byissuing aREFR
command. Toinform theEMIF oftherequired rate forperforming auto refresh cycles, theRRfield ofthe
SDRAM refresh control register (SDRCR) must beprogrammed. The EMIF willusethisvalue along with
twointernal counters toautomatically perform auto refresh cycles attherequired rate. The auto refresh
cycles cannot bedisabled, even iftheEMIF isnotinterfaced with anSDRAM. The remainder ofthis
section details theEMIF's refresh scheme andprovides anexample fordetermining theappropriate value
toplace intheRRfield ofSDRCR.
The twocounters used toperform auto-refresh cycles area13-bit refresh interval counter anda4-bit
refresh backlog counter. Atreset andupon writing totheRRfield, therefresh interval counter isloaded
with thevalue from RRfield andbegins decrementing, byone, each EMIF clock cycle. When therefresh
interval counter reaches zero, thefollowing actions occur:
*The refresh interval counter isreloaded with thevalue from theRRfield andrestarts decrementing.
*The 4-bit refresh backlog counter increments unless ithasalready reached itsmaximum value.
The refresh backlog counter records thenumber ofauto refresh cycles thattheEMIF currently has
outstanding. This counter isdecremented byoneeach time anauto refresh cycle isperformed and
incremented byoneeach time therefresh interval counter expires. The refresh backlog counter saturates
atthevalues of0000b and1111b. The EMIF uses therefresh backlog counter todetermine theurgency
with which anauto refresh cycle should beperformed. The four levels ofurgency aredescribed in
Table 21-12 .This refresh scheme allows therequired refreshes tobeperformed with minimal impact on
access requests.
Table 21-12. Refresh Urgency Levels
Urgency LevelRefresh Backlog
Counter Range Action Taken
Refresh May 1-3 Anauto-refresh cycle isperformed only iftheEMIF hasnorequests pending andnone
oftheSDRAM banks areopen.
Refresh Release 4-7 Anauto-refresh cycle isperformed iftheEMIF hasnorequests pending, regardless of
whether anySDRAM banks areopen.
Refresh Need 8-11 Anauto-refresh cycle isperformed atthecompletion ofthecurrent access unless
there areread requests pending.
Refresh Must 12-15 Multiple auto-refresh cycles areperformed atthecompletion ofthecurrent access
until theRefresh Release urgency level isreached. Atthatpoint, theEMIF canbegin
servicing anynew read orwrite requests.

<!-- Page 805 -->

www.ti.com EMIF Module Architecture
805 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.6.1 Determining theAppropriate Value fortheRRField
The value thatshould beprogrammed intotheRRfield ofSDRCR canbecalculated byusing the
frequency oftheEMIF_CLK signal (fEMIF_CLK )andtherequired refresh rate oftheSDRAM (fRefresh ).The
following formula canbeused:
RR=fEMIF_CLK /fRefresh
The SDRAM datasheet often communicates therequired SDRAM Refresh Rate interms ofthenumber of
REFR commands required inagiven time interval. The required SDRAM Refresh Rate intheformula
above cantherefore becalculated bydividing thenumber ofrequired cycles pertime interval (ncycles)by
thetime interval given inthedatasheet (tRefresh Period):
fRefresh =ncycles/tRefresh Period
Combining these formulas, thevalue thatshould beprogrammed intotheRRfield canbecomputed as:
RR=fEMIF_CLK ×tRefresh Period /ncycles
The following example illustrates calculating thevalue ofRR. Given that:
*fEMIF_CLK =100MHz (frequency oftheEMIF clock)
*tRefresh Period =64ms(required refresh interval oftheSDRAM)
*ncycles=8192 (number ofcycles inarefresh interval fortheSDRAM)
RRcanbecalculated as:
RR=100MHz ×64ms/8192
RR=781.25
RR=782cycles =30Eh cycles
21.2.5.7 Self-Refresh Mode
The EMIF canbeprogrammed toenter theself-refresh state bysetting theSRbitofSDCR to1.This will
cause theEMIF toissue theSLFR command after completing anyoutstanding SDRAM access requests
andclearing therefresh backlog counter byperforming oneormore auto refresh cycles. This places the
attached SDRAM device intoself-refresh mode inwhich itconsumes aminimal amount ofpower while
performing itsown refresh cycles. The SRbitshould besetandcleared using abyte-write totheupper
byte oftheSDRAM configuration register (SDCR) toavoid triggering theSDRAM initialization sequence.
While intheself-refresh state, theEMIF continues toservice asynchronous bank requests andregister
accesses asnormal, with onecaveat. The EMIF willnotpark thedata busfollowing aread to
asynchronous memory while intheself-refresh state. Instead, theEMIF tri-states thedata bus. Therefore,
itisnotrecommended toperform asynchronous read operations while theEMIF isintheself-refresh state,
inorder toprevent floating inputs onthedata bus. More information about data busparking canbefound
inSection 21.2.7 .
The EMIF willexitfrom theself-refresh state ifeither ofthefollowing events occur:
*The SRbitofSDCR iscleared to0.
*AnSDRAM accesses isrequested.
The EMIF exits from theself-refresh state bydriving EMIF_CKE high andperforming anauto refresh
cycle.
The attached SDRAM device should also beplaced intoSelf-Refresh Mode when changing thefrequency
ofEMIF_CLK. Ifthefrequency ofEMIF_CLK changes while theSDRAM isnotinSelf-Refresh Mode,
Procedure BinSection 21.2.5.5 should befollowed toreinitialize thedevice.

<!-- Page 806 -->

EMIF Module Architecture www.ti.com
806 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.8 Power Down Mode
Tosupport low-power modes, theEMIF canberequested toissue aPOWER DOWN command tothe
SDRAM bysetting thePDbitintheSDRAM configuration register (SDCR). When thisbitisset,theEMIF
willcontinue normal operation until alloutstanding memory access requests have been serviced andthe
SDRAM refresh backlog (ifthere isone) hasbeen cleared. Atthispoint theEMIF willenter thepower-
down state. Upon entering thisstate, theEMIF willissue aPOWER DOWN command (same asaNOP
command butdriving EMIF_CKE lowonthesame cycle). The EMIF then maintains EMIF_CKE lowuntil it
exits thepower-down state.
Since theEMIF services therefresh backlog before itenters thepower-down state, allinternal banks of
theSDRAM areclosed (precharged) prior toissuing thePOWER DOWN command. Therefore, theEMIF
only supports Precharge Power Down. The EMIF does notsupport Active Power Down, where internal
banks oftheSDRAM areopen (active) before thePOWER DOWN command isissued.
During thepower-down state, theEMIF services theSDRAM, asynchronous memory, andregister
accesses asnormal, returning tothepower-down state upon completion.
The PDWR bitinSDCR indicates whether theEMIF should perform refreshes inpower-down state. Ifthe
PDWR bitisset,theEMIF exits thepower-down state every time theRefresh Must level isset,performs
AUTO REFRESH commands totheSDRAM, andreturns back tothepower-down state. This evenly
distributes therefreshes totheSDRAM inpower-down state. IfthePDWR bitisnotset,theEMIF does
notperform anyrefreshes totheSDRAM. Therefore, thedata integrity oftheSDRAM isnotassured upon
power down exitifthePDWR bitisnotset.
IfthePDbitiscleared while inthepower-down state, theEMIF willcome outofthepower-down state.
The EMIF:
*Drives EMIF_CKE high.
*Enters itsidlestate.

<!-- Page 807 -->

EMIF_CLK
EMIF_nCS[0]
EMIF_nDQM
EMIF_BA
EMIF_A
EMIF_D
EMIF_nRAS
EMIF_nCAS
EMIF_nWEBank
Row Col
D1 D2 D3 D4ACTV
READCL=3
D5 D6 D7 D8
www.ti.com EMIF Module Architecture
807 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.9 SDRAM Read Operation
When theEMIF receives aread request toSDRAM from oneoftherequesters listed inSection 21.2.2 ,it
performs oneormore read access cycles. Aread access cycle begins with theissuing oftheACTV
command toselect thedesired bank androwoftheSDRAM device. After therowhasbeen opened, the
EMIF proceeds toissue aREAD command while specifying thedesired bank andcolumn address.
EMIF_A[10] isheld lowduring theREAD command toavoid auto-precharging. The READ command
signals theSDRAM device tostart bursting data from thespecified address while theEMIF issues NOP
commands. Following aREAD command, theCLfield oftheSDRAM configuration register (SDCR)
defines how many delay cycles willbepresent before theread data appears onthedata bus. This is
referred toastheCAS latency.
Figure 21-5 shows thesignal waveforms forabasic SDRAM read operation inwhich aburst ofdata is
read from asingle page. When theEMIF SDRAM interface isconfigured to16bitbysetting theNMbitof
theSDRAM configuration register (SDCR) to1,aburst size ofeight isused. Figure 21-5 shows aburst
size ofeight.
The EMIF willtruncate aseries ofbursting data iftheremaining addresses oftheburst arenotrequired to
complete therequest. The EMIF cantruncate theburst inthree ways:
*Byissuing another READ tothesame page inthesame bank.
*Byissuing aPRE command inorder toprepare foraccessing adifferent page ofthesame bank.
*Byissuing aBTcommand inorder toprepare foraccessing apage inadifferent bank.
Figure 21-5. Timing Waveform forBasic SDRAM Read Operation
Several other pins arealso active during aread access. The EMIF_nDQM[1:0] pins aredriven lowduring
theREAD commands andarekept lowduring theNOP commands thatcorrespond totheburst request.
The state oftheother EMIF pins during each command canbefound inTable 21-5.
The EMIF schedules itscommands based onthetiming information thatisprovided toitintheSDRAM
timing register (SDTIMR). The values forthetiming parameters inthisregister should bechosen tosatisfy
thetiming requirements listed intheSDRAM datasheet. The EMIF uses thistiming information toavoid
violating anytiming constraints related toissuing commands. This iscommonly accomplished byinserting
NOP commands between various commands during anaccess. Refer totheregister description of
SDTIMR inSection 21.3.6 formore details onthevarious timing parameters.

<!-- Page 808 -->

EMIF_CLK
EMIF_nCS[0]
EMIF_BA
EMIF_A
EMIF_D
EMIF_nRAS
EMIF_nCAS
EMIF_nWEBank
Row Column
D1 D2 D3 D4ACTV
WRT
EMIF_nDQM
D5 D6 D7 D8
EMIF Module Architecture www.ti.com
808 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.10 SDRAM Write Operations
When theEMIF receives awrite request toSDRAM from oneoftherequesters listed inSection 21.2.2 ,it
performs oneormore write-access cycles. Awrite-access cycle begins with theissuing oftheACTV
command toselect thedesired bank androwoftheSDRAM device. After therowhasbeen opened, the
EMIF proceeds toissue aWRT command while specifying thedesired bank andcolumn address.
EMIF_A[10] isheld lowduring theWRT command toavoid auto-precharging. The WRT command signals
theSDRAM device tostart writing aburst ofdata tothespecified address while theEMIF issues NOP
commands. The associated write data willbeplaced onthedata businthecycle concurrent with theWRT
command andwith subsequent burst continuation NOP commands.
Figure 21-6 shows thesignal waveforms forabasic SDRAM write operation inwhich aburst ofdata is
read from asingle page. When theEMIF SDRAM interface isconfigured to16-bit bysetting theNMbitof
theSDRAM configuration register (SDCR) to1,aburst size ofeight isused. Figure 21-6 shows aburst
size ofeight.
Figure 21-6. Timing Waveform forBasic SDRAM Write Operation
The EMIF willtruncate aseries ofbursting data iftheremaining addresses oftheburst arenotpart ofthe
write request. The EMIF cantruncate theburst inthree ways:
*Byissuing another WRT tothesame page
*Byissuing aPRE command inorder toprepare foraccessing adifferent page ofthesame bank
*Byissuing aBTcommand inorder toprepare foraccessing apage inadifferent bank
Several other pins arealso active during awrite access. The EMIF_nDQM[1:0] pins aredriven toselect
which bytes ofthedata word willbewritten totheSDRAM device. They arealso used tomask outentire
undesired data words during aburst access. The state oftheother EMIF pins during each command can
befound inTable 21-5.
The EMIF schedules itscommands based onthetiming information thatisprovided toitintheSDRAM
timing register (SDTIMR). The values forthetiming parameters inthisregister should bechosen tosatisfy
thetiming requirements listed intheSDRAM datasheet. The EMIF uses thistiming information toavoid
violating anytiming constraints related toissuing commands. This iscommonly accomplished byinserting
NOP commands during various cycles ofanaccess. Refer totheregister description ofSDTIMR in
Section 21.3.6 formore details onthevarious timing parameters.

<!-- Page 809 -->

www.ti.com EMIF Module Architecture
809 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.5.11 Mapping from Logical Address toEMIF Pins
When theEMIF receives anSDRAM access request, itmust convert theaddress oftheaccess intothe
appropriate signals tosend totheSDRAM device. The details ofthisaddress mapping areshown in
Table 21-13 for16-bit operation. Using thesettings oftheIBANK andPAGESIZE fields oftheSDRAM
configuration register (SDCR), theEMIF determines which bitsofthelogical address willbemapped to
theSDRAM row, column, andbank addresses.
Asthelogical address isincremented byonehalfword (16-bit operation), thecolumn address islikewise
incremented byoneuntil apage boundary isreached. When thelogical address increments across a
page boundary, theEMIF moves intothesame page inthenext bank oftheattached device by
incrementing thebank address EMIF_BA andresetting thecolumn address. The page intheprevious
bank isleftopen until itisnecessary toclose it.This method oftraversal through theSDRAM banks helps
maximize thenumber ofopen banks inside oftheSDRAM andresults inanefficient useofthedevice.
There isnolimitation onthenumber ofbanks thatcanbeopen atonetime, butonly onepage within a
bank canbeopen atatime.
The EMIF uses theEMIF_nDQM[1:0] pins during aWRT command tomask outselected bytes orentire
words. The EMIF_nDQM[1:0] pins arealways lowduring aREAD command.
Table 21-13. Mapping from Logical Address toEMIF Pins for16-bit SDRAM
IBANK PAGESIZELogical Address
31:27 26 25 24 23 22 21:14 13 12 11 10 9 8:1 0
0 0 - Row Address ColAddress EMIF_nDQM[0]
1 0 - Row AddressEMIF_BA[0
]ColAddress EMIF_nDQM[0]
2 0 - Row Address EMIF_BA[1:0] ColAddress EMIF_nDQM[0]
0 1 - Row Address Column Address EMIF_nDQM[0]
1 1 - Row AddressEMIF_BA[0
]Column Address EMIF_nDQM[0]
2 1 - Row Address EMIF_BA[1:0] Column Address EMIF_nDQM[0]
0 2 - Row Address Column Address EMIF_nDQM[0]
1 2 - Row AddressEMIF_BA[0
]Column Address EMIF_nDQM[0]
2 2 - Row Address EMIF_BA[1:0] Column Address EMIF_nDQM[0]
0 3 - Row Address Column Address EMIF_nDQM[0]
1 3 - Row AddressEMIF_BA[0
]Column Address EMIF_nDQM[0]
2 3 - Row Address EMIF_BA[1:0] Column Address EMIF_nDQM[0]
NOTE: The upper bitoftheRow Address isused only when addressing 256-Mbit and512-Mbit
SDRAM memories.

<!-- Page 810 -->

EMIF_nCS[n]
EMIF_nWE
EMIF_nOE
EMIF_WAIT
EMIF_BA[1:0]EMIF_D[x:0]
EMIF_nDQM[x:0]
EMIF_A[x:0]EMIF
EMIF Module Architecture www.ti.com
810 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6 Asynchronous Controller andInterface
The EMIF easily interfaces toavariety ofasynchronous devices including NOR Flash andSRAM. Itcan
beoperated intwomajor modes (see Table 21-14 ):
*Normal Mode
*Select Strobe Mode
Table 21-14. Normal Mode vs.Select Strobe Mode
Mode Function ofEMIF_nDQM pins Operation ofEMIF_nCS[4:2]
Normal Mode Byte enables Active during theentire asynchronous access cycle
Select Strobe Mode Byte enables Active only during thestrobe period ofanaccess cycle
The firstmode ofoperation isNormal Mode, inwhich theEMIF_nDQM pins oftheEMIF function asbyte
enables. Inthismode, theEMIF_nCS[4:2] pins behaves astypical chip select signals, remaining active for
theduration oftheasynchronous access. See Section 21.2.6.1 foranexample interface with multiple 8-bit
devices.
The second mode ofoperation isSelect Strobe Mode, inwhich theEMIF_nCS[4:2] pins actasastrobe,
active only during thestrobe period ofanaccess. Inthismode, theEMIF_nDQM pins oftheEMIF function
asstandard byte enables forreads andwrites. Asummary ofthedifferences between thetwomodes of
operation areshown inTable 21-14 .Refer toSection 21.2.6.4 forthedetails ofasynchronous operations
inNormal Mode, andtoSection 21.2.6.5 forthedetails ofasynchronous operations inSelect Strobe
Mode. The EMIF hardware defaults toNormal Mode, butcanbemanually switched toSelect Strobe Mode
bysetting theSSbitintheasynchronous m(m=1,2,3,or4)configuration register (CEnCFG) (n=2,3,
or4).Throughout thechapter, mcanhold thevalues 1,2,3or4;andncanhold thevalues 2,3,or4.
The EMIF also provides configurable cycle timing parameters andanExtended Wait Mode thatallows the
connected device toextend thestrobe period ofanaccess cycle. The following sections describe the
features related tointerfacing with external asynchronous devices.
21.2.6.1 Interfacing toAsynchronous Memory
Figure 21-7 shows theEMIF's external pins used ininterfacing with anasynchronous device. In
EMIF_nCS[n], n=2,3,or4.
Figure 21-7. EMIF Asynchronous Interface

<!-- Page 811 -->

EMIF_nDQM[1:0]
DQ[15:0]asynchronous
BE[1:0]EMIF_nWE
EMIF_D[15:0]EMIF_nCS[n]EMIF
nWEnCEdevice16−bit
EMIF_D[7:0]
EMIF_A[x:0]
EMIF_BA[1:0]DQ[7:0]
A[(x+2):2]
A[1:0]EMIF 8−bit
asynchronous
memory
a) EMIF to 8-bit memory interface
EMIF_D[15:0]
EMIF_A[x:0]
EMIF_BA[1]DQ[15:0]
A[(x+1):1]
A[0]EMIF 16−bit asynchronous
memory
b) EMIF to 16-bit memory interface
www.ti.com EMIF Module Architecture
811 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Ofspecial note istheconnection between theEMIF andtheexternal device's address bus. The EMIF
address pinEMIF_A[0] always provides theleast significant bitofa32-bit word address. Therefore, when
interfacing toa16-bit or8-bit asynchronous device, theEMIF_BA[1] andEMIF_BA[0] pins provide the
least-significant bitsofthehalfword orbyte address, respectively. Additionally, when theEMIF interfaces
toa16-bit asynchronous device, theEMIF_BA[0] pincanserve astheupper address lineEMIF_A[22].
Figure 21-8 andFigure 21-9 show themapping between theEMIF andtheconnected device's data and
address pins forvarious programmed data buswidths. The data buswidth may beconfigured inthe
asynchronous nconfiguration register (CEnCFG).
Figure 21-9 shows acommon interface between theEMIF andexternal asynchronous memory. Figure 21-
9shows aninterface between theEMIF andanexternal memory with byte enables. The EMIF should be
operated ineither Normal Mode orSelect Strobe Mode when using thisinterface, sothattheEMIF_nDQM
signals operate asbyte enables.
Figure 21-8. EMIF to8-bit/16-bit Memory Interface
Figure 21-9. Common Asynchronous Interface

<!-- Page 812 -->

EMIF Module Architecture www.ti.com
812 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.2 Accessing Larger Asynchronous Memories
The device has22dedicated EMIF address lines. Ifadevice such asalarge asynchronous flash needs to
beattached totheEMIF, then GPIO pins may beused tocontrol theflash device 'supper address lines.
21.2.6.3 Configuring theEMIF forAsynchronous Accesses
The operation oftheEMIF's asynchronous interface canbeconfigured byprogramming theappropriate
register fields. The reset value andbitposition foreach register field canbefound inSection 21.3.The
following tables listtheregister fields thatcanbeprogrammed anddescribe thepurpose ofeach field.
These registers canbeprogrammed prior toaccessing theexternal memory, andthetransfer following a
write tothese registers willusethenew configuration.
Table 21-15. Description oftheAsynchronous mConfiguration Register (CEnCFG)
Parameter Description
SS Select Strobe mode. This bitselects theEMIF 'smode ofoperation inthefollowing way:
*SS=0selects Normal Mode
-EMIF_nDQM pins function asbyte enables
-EMIF_nCS[4:2] active forduration ofaccess
*SS=1selects Select Strobe Mode
-EMIF_nDQM pins function asbyte enables
-EMIF_nCS[4:2] acts asastrobe.
EW Extended Wait Mode enable.
*EW=0disables Extended Wait Mode
*EW=1enables Extended Wait Mode
When setto1,theEMIF enables itsExtended Wait Mode inwhich thestrobe width ofanaccess
cycle canbeextended inresponse totheassertion oftheEMIF_nWAIT pin.The WPnbitinthe
asynchronous wait cycle configuration register (AWCC) controls topolarity ofEMIF_nWAIT pin.
See Section 21.2.6.6 formore details onthismode ofoperation.
W_SETUP/R_SETUP Read/Write setup widths.
These fields define thenumber ofEMIF clock cycles ofsetup time fortheaddress pins (EMIF_A
andEMIF_BA), byte enables (EMIF_nDQM), andasynchronous chip enable (EMIF_nCS[4:2])
before theread strobe pin(EMIF_nOE) orwrite strobe pin(EMIF_nWE) falls, minus onecycle.
Forwrites, theW_SETUP field also defines thesetup time forthedata pins (EMIF_D). Refer to
thedatasheet oftheexternal asynchronous device todetermine theappropriate setting forthis
field.
W_STROBE/R_STROBE Read/Write strobe widths.
These fields define thenumber ofEMIF clock cycles between thefalling andrising oftheread
strobe pin(EMIF_nOE) orwrite strobe pin(EMIF_nWE), minus onecycle. IfExtended Wait Mode
isenabled bysetting theEWfield intheasynchronous nconfiguration register (CEnCFG), these
fields must besettoavalue greater than zero. Refer tothedatasheet oftheexternal
asynchronous device todetermine theappropriate setting forthisfield.
W_HOLD/R_HOLD Read/Write hold widths.
These fields define thenumber ofEMIF clock cycles ofhold time fortheaddress pins (EMIF_A
andEMIF_BA), byte enables (EMIF_nDQM), andasynchronous chip enable (EMIF_nCS[4:2])
after theread strobe pin(EMIF_nOE) orwrite strobe pin(EMIF_nWE) rises, minus onecycle. For
writes, theW_HOLD field also defines thehold time forthedata pins (EMIF_D). Refer tothe
datasheet oftheexternal asynchronous device todetermine theappropriate setting forthisfield.
TA Minimum turnaround time.
This field defines theminimum number ofEMIF clock cycles between asynchronous reads and
writes, minus onecycle. The purpose ofthisfeature istoavoid contention onthebus. The value
written tothisfield also determines thenumber ofcycles thatwillbeinserted between
asynchronous accesses andSDRAM accesses. Refer tothedatasheet oftheexternal
asynchronous device todetermine theappropriate setting forthisfield.

<!-- Page 813 -->

www.ti.com EMIF Module Architecture
813 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-15. Description oftheAsynchronous mConfiguration Register (CEnCFG) (continued)
Parameter Description
ASIZE Asynchronous Device Bus Width.
This field determines thedata buswidth oftheasynchronous interface inthefollowing way:
*ASIZE =0selects an8-bit bus
*ASIZE =1selects a16-bit bus
The configuration ofASIZE determines thefunction oftheEMIF_A andEMIF_BA pins as
described inSection 21.2.6.1 .This field also determines thenumber ofexternal accesses
required tofulfill arequest generated byoneofthesources mentioned inSection 21.2.2 .For
example, arequest fora32-bit word would require four external access when ASIZE =0.Refer to
thedatasheet oftheexternal asynchronous device todetermine theappropriate setting forthis
field.
Table 21-16. Description oftheAsynchronous Wait Cycle Configuration Register (AWCC)
Parameter Description
WPn EM_WAIT Polarity.
*WPn=0selects active-low polarity
*WPn=1selects active-high polarity
When setto1,theEMIF willwait iftheEMIF_nWAIT pinishigh. When cleared to0,theEMIF will
wait iftheEMIF_nWAIT pinislow. The EMIF must have theExtended Wait Mode enabled forthe
EMIF_nWAIT pintoaffect thewidth ofthestrobe period.
MAX_EXT_WAIT Maximum Extended Wait Cycles.
This field configures thenumber ofEMIF clock cycles theEMIF willwait fortheEMIF_nWAIT pin
tobedeactivated during thestrobe period ofanaccess cycle. The maximum number ofEMIF
clock cycles itwillwait isdetermined bythefollowing formula:
Maximum Extended Wait Cycles =(MAX_EXT_WAIT +1)×16
IftheEMIF_nWAIT pinisnotdeactivated within thetime specified bythisfield, theEMIF resumes
theaccess cycle, registering whatever data isonthebusandproceeding tothehold period ofthe
access cycle. This situation isreferred toasanAsynchronous Timeout. AnAsynchronous
Timeout generates aninterrupt, ifithasbeen enabled intheEMIF interrupt mask setregister
(INTMSKSET). Refer toSection 21.2.9.1 formore information about theEMIF interrupts.
Table 21-17. Description oftheEMIF Interrupt Mask SetRegister (INTMSKSET)
Parameter Description
WR_MASK_SET Wait Rise Mask Set.
Writing a1enables aninterrupt tobegenerated when arising edge onEMIF_nWAIT occurs
AT_MASK_SET Asynchronous Timeout Mask Set.
Writing a1tothisbitenables aninterrupt tobegenerated when anAsynchronous Timeout
occurs.
Table 21-18. Description oftheEMIF Interrupt Mast Clear Register (INTMSKCLR)
Parameter Description
WR_MASK_CLR Wait Rise Mask Clear.
Writing a1tothisbitdisables theinterrupt, clearing theWR_MASK_SET bitintheEMIF interrupt
mask setregister (INTMSKSET).
AT_MASK_CLR Asynchronous Timeout Mask Clear.
Writing a1tothisbitprevents aninterrupt from being generated when anAsynchronous Timeout
occurs.

<!-- Page 814 -->

EMIF Module Architecture www.ti.com
814 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.4 Read andWrite Operations inNormal Mode
Normal Mode istheasynchronous interface's default mode ofoperation. Itisselected when theSSbitin
theasynchronous nconfiguration register (CEnCFG) iscleared to0.Inthismode, theEMIF_nDQM pins
operate asbyte enables. Section 21.2.6.4.1 andSection 21.2.6.4.2 explain thedetails ofread andwrite
operations while inNormal Mode.
21.2.6.4.1 Asynchronous Read Operations (Normal Mode)
NOTE: During anentire asynchronous read operation, theEMIF_nWE pinisdriven high.
Anasynchronous read isperformed when anyoftherequesters mentioned inSection 21.2.2 request a
read from theattached asynchronous memory. After therequest isreceived, aread operation isinitiated
once itbecomes theEMIF's highest priority task, according tothepriority scheme detailed in
Section 21.2.13 .Intheevent thattheread request cannot beserviced byasingle access cycle tothe
external device, multiple access cycles willbeperformed bytheEMIF until theentire request isfulfilled.
The details ofanasynchronous read operation inNormal Mode aredescribed inTable 21-19 .Also,
Figure 21-10 shows anexample timing diagram ofabasic read operation.
Table 21-19. Asynchronous Read Operation inNormal Mode
Time Interval PinActivity inNormal Mode
Turn-around
periodOnce theread operation becomes thehighest priority task fortheEMIF, theEMIF waits fortheprogrammed
number ofturn-around cycles before proceeding tothesetup period oftheoperation. The number ofwait cycles is
taken directly from theTAfield oftheasynchronous nconfiguration register (CEnCFG). There aretwoexceptions
tothisrule:
*Ifthecurrent read operation was directly proceeded byanother read operation, noturnaround cycles are
inserted.
*Ifthecurrent read operation was directly proceeded byawrite operation andtheTAfield hasbeen cleared
to0,oneturn-around cycle willbeinserted.
After theEMIF haswaited fortheturnaround cycles tocomplete, itagain checks tomake sure thattheread
operation isstillitshighest priority task. Ifso,theEMIF proceeds tothesetup period oftheoperation. Ifitisno
longer thehighest priority task, theEMIF terminates theoperation.
Start ofthe
setup periodThe following actions occur atthestart ofthesetup period:
*The setup, strobe, andhold values aresetaccording totheR_SETUP, R_STROBE, andR_HOLD values in
CEnCFG.
*The address pins EMIF_A andEMIF_BA become valid andcarry thevalues described inSection 21.2.6.1 .
*EMIF_nCS[4:2] falls toenable theexternal device (ifnotalready lowfrom aprevious operation)
Strobe period The following actions occur during thestrobe period ofaread operation:
1.EMIF_nOE falls atthestart ofthestrobe period
2.Ontherising edge oftheclock which isconcurrent with theendofthestrobe period:
* EMIF_nOE rises
* The data ontheEMIF_D busissampled bytheEMIF.
InFigure 21-10 ,EMIF_nWAIT isinactive. IfEMIF_nWAIT isinstead activated, thestrobe period canbeextended
bytheexternal device togive itmore time toprovide thedata. Section 21.2.6.6 contains more details onusing the
EMIF_nWAIT pin.
End ofthehold
periodAttheendofthehold period:
*The address pins EMIF_A andEMIF_BA become invalid
*EMIF_nCS[4:2] rises (ifnomore operations arerequired tocomplete thecurrent request)
EMIF may berequired toissue additional read operations toadevice with asmall data buswidth inorder to
complete anentire word access. Inthiscase, theEMIF immediately re-enters thesetup period tobegin another
operation without incurring theturn-round cycle delay. The setup, strobe, andhold values arenotupdated inthis
case. Iftheentire word access hasbeen completed, theEMIF returns toitsprevious state unless another
asynchronous request hasbeen submitted andiscurrently thehighest priority task. Ifthisisthecase, theEMIF
instead enters directly intotheturnaround period forthepending read orwrite operation.

<!-- Page 815 -->

EMIF_CLK
EMIF_nCS[n]
EMIF_nDQM
EMIF_A/EMIF_BA
EMIF_D
EMIF_nOE
EMIF_nWESetupStrobeHold
232
Address
DataByte enable
www.ti.com EMIF Module Architecture
815 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-10. Timing Waveform ofanAsynchronous Read Cycle inNormal Mode

<!-- Page 816 -->

EMIF Module Architecture www.ti.com
816 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.4.2 Asynchronous Write Operations (Normal Mode)
NOTE: During anentire asynchronous write operation, theEMIF_nOE pinisdriven high.
Anasynchronous write isperformed when anyoftherequesters mentioned inSection 21.2.2 request a
write tomemory intheasynchronous bank oftheEMIF. After therequest isreceived, awrite operation is
initiated once itbecomes theEMIF's highest priority task, according tothepriority scheme detailed in
Section 21.2.13 .Intheevent thatthewrite request cannot beserviced byasingle access cycle tothe
external device, multiple access cycles willbeperformed bytheEMIF until theentire request isfulfilled.
The details ofanasynchronous write operation inNormal Mode aredescribed inTable 21-20 .Also,
Figure 21-11 shows anexample timing diagram ofabasic write operation.
Table 21-20. Asynchronous Write Operation inNormal Mode
Time Interval PinActivity inNormal Mode
Turnaround
periodOnce thewrite operation becomes thehighest priority task fortheEMIF, theEMIF waits fortheprogrammed
number ofturn-around cycles before proceeding tothesetup period oftheoperation. The number ofwait cycles is
taken directly from theTAfield oftheasynchronous nconfiguration register (CEnCFG). There aretwoexceptions
tothisrule:
*Ifthecurrent write operation was directly proceeded byanother write operation, noturn-around cycles are
inserted.
*Ifthecurrent write operation was directly proceeded byaread operation andtheTAfield hasbeen cleared
to0,oneturnaround cycle willbeinserted.
After theEMIF haswaited fortheturn-around cycles tocomplete, itagain checks tomake sure thatthewrite
operation isstillitshighest priority task. Ifso,theEMIF proceeds tothesetup period oftheoperation. Ifitisno
longer thehighest priority task, theEMIF terminates theoperation.
Start ofthe
setup periodThe following actions occur atthestart ofthesetup period:
*The setup, strobe, andhold values aresetaccording totheW_SETUP, W_STROBE, andW_HOLD values
inCEnCFG.
*The address pins EMIF_A andEMIF_BA andthedata pins EMIF_D become valid. The EMIF_A and
EMIF_BA pins carry thevalues described inSection 21.2.6.1 .
*EMIF_nCS[4:2] falls toenable theexternal device (ifnotalready lowfrom aprevious operation).
Strobe period The following actions occur atthestart ofthestrobe period ofawrite operation:
1.EMIF_nWE falls
2.The EMIF_nDQM pins become valid asbyte enables.
The following actions occur ontherising edge oftheclock which isconcurrent with theendofthestrobe period:
1.EMIF_nWE rises
2.The EMIF_nDQM pins deactivate
InFigure 21-11 ,EMIF_nWAIT isinactive. IfEMIF_nWAIT isinstead activated, thestrobe period canbeextended
bytheexternal device togive itmore time toaccept thedata. Section 21.2.6.6 contains more details onusing the
EMIF_nWAIT pin.
End ofthehold
periodAttheendofthehold period:
*The address pins EMIF_A andEMIF_BA become invalid
*The data pins become invalid
*EMIF_nCS[n] (n=2,3,or4)rises (ifnomore operations arerequired tocomplete thecurrent request)
The EMIF may berequired toissue additional write operations toadevice with asmall data buswidth inorder to
complete anentire word access. Inthiscase, theEMIF immediately re-enters thesetup period tobegin another
operation without incurring theturnaround cycle delay. The setup, strobe, andhold values arenotupdated inthis
case. Iftheentire word access hasbeen completed, theEMIF returns toitsprevious state unless another
asynchronous request hasbeen submitted andiscurrently thehighest priority task. Ifthisisthecase, theEMIF
instead enters directly intotheturnaround period forthepending read orwrite operation.

<!-- Page 817 -->

EMIF_CLK
EMIF_nCS[n]
EMIF_nDQM
EMIF_A/EMIF_BA
EMIF_D
EMIF_nOE
EMIF_nWESetupStrobeHold
232
Address
DataAddressByte enable
www.ti.com EMIF Module Architecture
817 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-11. Timing Waveform ofanAsynchronous Write Cycle inNormal Mode

<!-- Page 818 -->

EMIF Module Architecture www.ti.com
818 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.5 Read andWrite Operation inSelect Strobe Mode
Select Strobe Mode istheEMIF's second mode ofoperation. Itisselected when theSSbitofthe
asynchronous nconfiguration register (CEnCFG) issetto1.Inthismode, theEMIF_nDQM pins operate
asbyte enables andtheEMIF_nCS[n] (n=2,3,or4)pinisonly active during thestrobe period ofan
access cycle. Section 21.2.6.4.1 andSection 21.2.6.4.2 explain thedetails ofread andwrite operations
while inSelect Strobe Mode.
21.2.6.5.1 Asynchronous Read Operations (Select Strobe Mode)
NOTE: During theentirety ofanasynchronous read operation, theEMIF_nWE pinisdriven high.
Anasynchronous read isperformed when anyoftherequesters mentioned inSection 21.2.2 request a
read from theattached asynchronous memory. After therequest isreceived, aread operation isinitiated
once itbecomes theEMIF's highest priority task, according tothepriority scheme detailed in
Section 21.2.13 .Intheevent thattheread request cannot beserviced byasingle access cycle tothe
external device, multiple access cycles willbeperformed bytheEMIF until theentire request isfulfilled.
The details ofanasynchronous read operation inSelect Strobe Mode aredescribed inTable 21-21 .Also,
Figure 21-12 shows anexample timing diagram ofabasic read operation.
Table 21-21. Asynchronous Read Operation inSelect Strobe Mode
Time Interval PinActivity inSelect Strobe Mode
Turnaround
periodOnce theread operation becomes thehighest priority task fortheEMIF, theEMIF waits fortheprogrammed
number ofturn-around cycles before proceeding tothesetup period oftheoperation. The number ofwait cycles is
taken directly from theTAfield oftheasynchronous nconfiguration register (CEnCFG). There aretwoexceptions
tothisrule:
*Ifthecurrent read operation was directly proceeded byanother read operation, noturn-around cycles are
inserted.
*Ifthecurrent read operation was directly proceeded byawrite operation andtheTAfield hasbeen cleared
to0,oneturn-around cycle willbeinserted.
After theEMIF haswaited fortheturn-around cycles tocomplete, itagain checks tomake sure thattheread
operation isstillitshighest priority task. Ifso,theEMIF proceeds tothesetup period oftheoperation. Ifitisno
longer thehighest priority task, theEMIF terminates theoperation.
Start ofthe
setup periodThe following actions occur atthestart ofthesetup period:
*The setup, strobe, andhold values aresetaccording totheR_SETUP, R_STROBE, andR_HOLD values in
CEnCFG.
*The address pins EMIF_A andEMIF_BA become valid andcarry thevalues described inSection 21.2.6.1 .
*The EMIF_nDQM pins become valid asbyte enables.
Strobe period The following actions occur during thestrobe period ofaread operation:
1.EMIF_nCS[n] (n=2,3,or4)andEMIF_nOE fallatthestart ofthestrobe period
2.Ontherising edge oftheclock which isconcurrent with theendofthestrobe period:
* EMIF_nCS[n] (n=2,3,or4)andEMIF_nOE rise
* The data ontheEMIF_D busissampled bytheEMIF.
InFigure 21-12 ,EMIF_nWAIT isinactive. IfEMIF_nWAIT isinstead activated, thestrobe period canbeextended
bytheexternal device togive itmore time toprovide thedata. Section 21.2.6.6 contains more details onusing the
EMIF_nWAIT pin.
End ofthehold
periodAttheendofthehold period:
*The address pins EMIF_A andEMIF_BA become invalid
*The EMIF_nDQM pins become invalid
The EMIF may berequired toissue additional read operations toadevice with asmall data buswidth inorder to
complete anentire word access. Inthiscase, theEMIF immediately re-enters thesetup period tobegin another
operation without incurring theturnaround cycle delay. The setup, strobe, andhold values arenotupdated inthis
case. Iftheentire word access hasbeen completed, theEMIF returns toitsprevious state unless another
asynchronous request hasbeen submitted andiscurrently thehighest priority task. Ifthisisthecase, theEMIF
instead enters directly intotheturnaround period forthepending read orwrite operation.

<!-- Page 819 -->

EMIF_CLK
EMIF_nCS[n]
EMIF_nDQM
EMIF_A/EMIF_BA
EMIF_D
EMIF_nOE
EMIF_nWESetupStrobeHold
232
Byte enables
Address
Data
www.ti.com EMIF Module Architecture
819 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-12. Timing Waveform ofanAsynchronous Read Cycle inSelect Strobe Mode

<!-- Page 820 -->

EMIF Module Architecture www.ti.com
820 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.5.2 Asynchronous Write Operations (Select Strobe Mode)
NOTE: During theentirety ofanasynchronous write operation, theEMIF_nOE pinisdriven high.
Anasynchronous write isperformed when anyoftherequesters mentioned inSection 21.2.2 request a
write tomemory intheasynchronous bank oftheEMIF. After therequest isreceived, awrite operation is
initiated once itbecomes theEMIF's highest priority task, according tothepriority scheme detailed in
Section 21.2.13 .Intheevent thatthewrite request cannot beserviced byasingle access cycle tothe
external device, multiple access cycles willbeperformed bytheEMIF until theentire request isfulfilled.
The details ofanasynchronous write operation inSelect Strobe Mode aredescribed inTable 21-22 .Also,
Figure 21-13 shows anexample timing diagram ofabasic write operation.
Table 21-22. Asynchronous Write Operation inSelect Strobe Mode
Time Interval PinActivity inSelect Strobe Mode
Turnaround
periodOnce thewrite operation becomes thehighest priority task fortheEMIF, theEMIF waits fortheprogrammed
number ofturnaround cycles before proceeding tothesetup period oftheoperation. The number ofwait cycles is
taken directly from theTAfield oftheasynchronous nconfiguration register (CEnCFG). There aretwoexceptions
tothisrule:
*Ifthecurrent write operation was directly proceeded byanother write operation, noturn-around cycles are
inserted.
*Ifthecurrent write operation was directly proceeded byaread operation andtheTAfield hasbeen cleared
to0,oneturnaround cycle willbeinserted.
After theEMIF haswaited fortheturnaround cycles tocomplete, itagain checks tomake sure thatthewrite
operation isstillitshighest priority task. Ifso,theEMIF proceeds tothesetup period oftheoperation. Ifitisno
longer thehighest priority task, theEMIF terminates theoperation.
Start ofthe
setup periodThe following actions occur atthestart ofthesetup period:
*The setup, strobe, andhold values aresetaccording totheW_SETUP, W_STROBE, andW_HOLD values
inCEnCFG.
*The address pins EMIF_A andEMIF_BA andthedata pins EMIF_D become valid. The EMIF_A and
EMIF_BA pins carry thevalues described inSection 21.2.6.1 .
*The EMIF_nDQM pins become active asbyte enables.
Strobe period The following actions occur atthestart ofthestrobe period ofawrite operation:
*EMIF_nCS[n] (n=2,3,or4)andEMIF_nWE fall
The following actions occur ontherising edge oftheclock which isconcurrent with theendofthestrobe period:
*EMIF_nCS[n] (n=2,3,or4)andEMIF_nWE rise
InFigure 21-13 ,EMIF_nWAIT isinactive. IfEMIF_nWAIT isinstead activated, thestrobe period canbeextended
bytheexternal device togive itmore time toaccept thedata. Section 21.2.6.6 contains more details onusing the
EMIF_nWAIT pin.
End ofthehold
periodAttheendofthehold period:
*The address pins EMIF_A andEMIF_BA become invalid
*The data pins become invalid
*The EMIF_nDQM pins become invalid
The EMIF may berequired toissue additional write operations toadevice with asmall data buswidth inorder to
complete anentire word access. Inthiscase, theEMIF immediately re-enters thesetup period tobegin another
operation without incurring theturnaround cycle delay. The setup, strobe, andhold values arenotupdated inthis
case. Iftheentire word access hasbeen completed, theEMIF returns toitsprevious state unless another
asynchronous request hasbeen submitted andiscurrently thehighest priority task. Ifthisisthecase, theEMIF
instead enters directly intotheturn-around period forthepending read orwrite operation.

<!-- Page 821 -->

EMIF_CLK
EMIF_nCS[n]
EMIF_nDQM
EMIF_A/EMIF_BA
EMIF_D
EMIF_nOE
EMIF_nWESetupStrobeHold
232
Byte enables
Address
Data
www.ti.com EMIF Module Architecture
821 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-13. Timing Waveform ofanAsynchronous Write Cycle inSelect Strobe Mode
21.2.6.6 Extended Wait Mode andtheEMIF_nWAIT Pin
The EMIF supports theExtend Wait Mode. This isamode inwhich theexternal asynchronous device may
assert control over thelength ofthestrobe period. The Extended Wait Mode canbeentered bysetting the
EWbitintheasynchronous nconfiguration register (CEnCFG) (n=2,3,or4).When thisbitisset,the
EMIF monitors theEMIF_nWAIT pintodetermine iftheattached device wishes toextend thestrobe
period ofthecurrent access cycle beyond theprogrammed number ofclock cycles.
When theEMIF detects thattheEMIF_nWAIT pinhasbeen asserted, itwillbegin inserting extra strobe
cycles intotheoperation until theEMIF_nWAIT pinisdeactivated bytheexternal device. The EMIF will
then return tothelastcycle oftheprogrammed strobe period andtheoperation willproceed asusual from
thispoint. Please refer tothedevice data manual fordetails onthetiming requirements ofthe
EMIF_nWAIT signal.
The EMIF_nWAIT pincannot beused toextend thestrobe period indefinitely. The programmable
MAX_EXT_WAIT field intheasynchronous wait cycle configuration register (AWCC) determines the
maximum number ofEMIF_CLK cycles thestrobe period may beextended beyond theprogrammed
length. When thecounter expires, theEMIF proceeds tothehold period oftheoperation regardless ofthe
state oftheEMIF_nWAIT pin.The EMIF canalso generate aninterrupt upon expiration ofthiscounter.
See Section 21.2.9.1 fordetails onenabling thisinterrupt.
FortheEMIF tofunction properly intheExtended Wait mode, theWPnbitofAWCC must beprogrammed
tomatch thepolarity oftheEMIF_nWAIT pin.Initsreset state of1,theEMIF willinsert wait cycles when
theEMIF_nWAIT pinissampled high. When setto0,theEMIF willinsert wait cycles only when
EMIF_nWAIT issampled low. This programmability allows foraglueless connection tolarger variety of
asynchronous devices.
Finally, arestriction isplaced onthestrobe period timing parameters when operating inExtended Wait
mode. Specifically, thesum oftheW_SETUP andW_STROBE fields must begreater than 4,andthesum
oftheR_SETUP andR_STROBE fields must begreater than 4fortheEMIF torecognize the
EMIF_nWAIT pinhasbeen asserted. The W_SETUP, W_STROBE, R_SETUP, andR_STROBE fields are
inCEnCFG.

<!-- Page 822 -->

EMIF_CLKSetupStrobe pg_delay pg_delay pg_delayHold
EMIF_nCS[n]
EMIF_nDQM
EMIF_A/EMIF_BA A0 A1 A2 A3
D0 D1 D2 D3 EMIF_D
EMIF_nOE
EMIF_nWE
EMIF Module Architecture www.ti.com
822 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.6.7 NOR Flash Page Mode
EMIF supports Page mode reads forNOR Flash onitsasynchronous memory chip selects. This mode can
beenabled bywriting a1totheCSn_PG_MD_EN (n=2,3,or4)field inthePage Mode Control register
forthechip select inconsideration. Whenever Page Mode forreads isenabled foraparticular chip select,
thepage size forthedevice connected must also beprogrammed intheCSn_PG_SIZE field ofthePage
Mode Control register. The address change tovalid read data available timing must beprogrammed inthe
CSn_PG_DEL field ofthePage Control register. Allother asynchronous memory timings must be
programmed intheasynchronous configuration register (CEnCFG). See Figure 21-14 forread in
asynchronous page mode.
NOTE: The Extended Wait mode andtheSelect Strobe mode must bedisabled when using the
asynchronous interface inPage mode.
Figure 21-14. Asynchronous Read inPage Mode
21.2.7 Data Bus Parking
The EMIF always drives thedata bustotheprevious write data value when itisidle. This feature iscalled
data busparking. Only when theEMIF issues aread command totheexternal memory does itstop
driving thedata bus. After theEMIF latches thelastread data, itimmediately parks thedata busagain.
The oneexception tothisbehavior occurs after performing anasynchronous read operation while the
EMIF isintheself-refresh state. Inthissituation, theread operation isnotfollowed bytheEMIF parking
thedata bus. Instead, theEMIF tri-states thedata bus. Therefore, itisnotrecommended toperform
asynchronous read operations while theEMIF isintheself-refresh state, inorder toprevent floating inputs
onthedata bus. External pull-ups, such as10kΩresistors, should beplaced onthe16EMIF data bus
pins (which donothave internal pull-ups) ifitisrequired toperform reads inthissituation. The precise
resistor value should bechosen sothattheworst case combined off-state leakage currents donotcause
thevoltage levels ontheassociated pins todrop below thehigh-level input voltage requirement.
Forinformation about theself-refresh state, seeSection 21.2.5.7 .

<!-- Page 823 -->

www.ti.com EMIF Module Architecture
823 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.8 Reset andInitialization Considerations
The EMIF memory controller hastwoactive-low reset signals, CHIP_RST_n andMOD_G_RST_n. Both
these reset signals aredriven bythedevice system reset signal. This device does notoffer theflexibility to
reset justtheEMIF state machine without also resetting theEMIF controller's memory-mapped registers.
Assoon asthedevice system reset isreleased (driven High), theEMIF memory controller immediately
begins itsinitialization sequence. Command anddata stored intheEMIF memory controller FIFOs are
lost. Refer theArchitecture chapter ofthetecnical reference manual (TRM) formore information on
conditions thatcancause adevice system reset tobeasserted.
When system reset isreleased, theEMIF automatically begins running theSDRAM initialization sequence
described inSection 21.2.5.4 .Even though theinitialization procedure isautomatic, aspecial procedure,
found inSection 21.2.5.5 must stillbefollowed.
21.2.9 Interrupt Support
The EMIF supports asingle interrupt totheCPU. Section 21.2.9.1 details thegeneration andinternal
masking ofEMIF interrupts.
21.2.9.1 Interrupt Events
There arethree conditions thatmay cause theEMIF togenerate aninterrupt totheCPU. These conditions
are:
*Arising edge ontheEMIF_nWAIT signal (wait riseinterrupt)
*Anasynchronous time out
*Usage ofunsupported addressing mode (line trap interrupt)
The wait riseinterrupt occurs when arising edge isdetected onEMIF_nWAIT signal. This interrupt
generation isnotaffected bytheWPnbitintheasynchronous wait cycle configuration register (AWCC).
The asynchronous time outinterrupt condition occurs when theattached asynchronous device fails to
deassert theEMIF_nWAIT pinwithin thenumber ofcycles defined bytheMAX_EXT_WAIT bitinAWCC
(this happens only inextended wait mode). EMIF supports only linear incrementing andcache linewrap
addressing modes .Ifanaccess request foranunsupported addressing mode isreceived, theEMIF will
settheLTbitintheEMIF interrupt rawregister (INTRAW) andtreat therequest asalinear incrementing
request.
Only when theinterrupt isenabled bysetting theappropriate bit
(WR_MASK_SET/AT_MASK_SET/LT_MASK_SET) intheEMIF interrupt mask setregister (INTMSKSET)
to1,willtheinterrupt besent totheCPU. Once enabled, theinterrupt may bedisabled bywriting a1to
thecorresponding bitintheEMIF interrupt mask clear register (INTMSKCLR). The bitfields inboth the
INTMSKSET andINTMSKCLR may beused toindicate whether theinterrupt isenabled. When the
interrupt isenabled, thecorresponding bitfield inboth theINTMSKSET andINTMSKCLR willhave a
value of1;when theinterrupt isdisabled, thecorresponding bitfield willhave avalue of0.
The EMIF interrupt rawregister (INTRAW) andtheEMIF interrupt mask register (INTMSK) indicate the
status ofeach interrupt. The appropriate bit(WR/AT/LT) inINTRAW issetwhen theinterrupt condition
occurs, whether ornottheinterrupt hasbeen enabled. However, theappropriate bit
(WR_MASKED/AT_MASKED/LT_MASKED) inINTMSK issetonly when theinterrupt condition occurs
andtheinterrupt isenabled. Writing a1tothebitinINTRAW clears theINTRAW bitaswell asthe
corresponding bitinINTMSK. Table 21-23 contains abrief summary oftheinterrupt status andcontrol bit
fields. See Section 21.3 forcomplete details ontheregister fields.

<!-- Page 824 -->

EMIF Module Architecture www.ti.com
824 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-23. Interrupt Monitor andControl BitFields
Register Name BitName Description
EMIF interrupt rawregister (INTRAW) WR This bitissetwhen anrising edge ontheEMIF_nWAIT signal occurs. Writing
a1clears theWRbitaswell astheWR_MASKED bitinINTMSK.
AT This bitissetwhen anasynchronous timeout occurs. Writing a1clears the
ATbitaswell astheAT_MASKED bitinINTMSK.
LT This bitissetwhen anunsupported addressing mode isused. Writing a1
clears LTbitaswell astheLT_MASKED bitinINTMSK.
EMIF interrupt mask register
(INTMSK)WR_MASKED This bitissetonly when arising edge ontheEMIF_nWAIT signal occurs and
theinterrupt hasbeen enabled bywriting a1totheWR_MASK_SET bitin
INTMSKSET.
AT_MASKED This bitissetonly when anasynchronous timeout occurs andtheinterrupt
hasbeen enabled bywriting a1totheAT_MASK_SET bitinINTMSKSET.
LT_MASKED This bitissetonly when linetrap interrupt occurs andtheinterrupt hasbeen
enabled bywriting a1totheLT_MASK_SET bitinINTMSKSET.
EMIF interrupt mask setregister
(INTMSKSET)WR_MASK_SET Writing a1tothisbitenables thewait riseinterrupt.
AT_MASK_SET Writing a1tothisbitenables theasynchronous timeout interrupt.
LT_MASK_SET Writing a1tothisbitenables thelinetrap interrupt.
EMIF interrupt mask clear register
(INTMSKCLR)WR_MASK_CLR Writing a1tothisbitdisables thewait riseinterrupt.
AT_MASK_CLR Writing a1tothisbitdisables theasynchronous timeout interrupt.
LT_MASK_CLR Writing a1tothisbitdisables thelinetrap interrupt.
21.2.10 DMA Event Support
EMIF memory controller isaDMA slave peripheral andtherefore does notgenerate DMA events. Data
read andwrite requests may bemade directly, bymasters andtheDMA.
21.2.11 EMIF Signal Multiplexing
Fordetails onEMIF signal multiplexing, seetheI/OMultiplexing Module chapter ofthetechnical reference
manual.
21.2.12 Memory Map
Forinformation describing thedevice memory-map, seeyour device-specific datasheet.

<!-- Page 825 -->

www.ti.com EMIF Module Architecture
825 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.13 Priority andArbitration
Section 21.2.2 describes theexternal prioritization andarbitration among requests from different sources
within themicrocontroller. The result ofthisexternal arbitration isthatonly onerequest ispresented tothe
EMIF atatime. Once theEMIF completes arequest, theexternal arbiter then provides theEMIF with the
next pending request.
Internally, theEMIF undertakes memory device transactions according toastrict priority scheme. The
highest priority events are:
*Adevice reset.
*Awrite toanyofthethree least significant bytes oftheSDRAM configuration register (SDCR).
Either ofthese events willcause theEMIF toimmediately commence itsinitialization sequence as
described inSection 21.2.5.4 .
Once theEMIF hascompleted itsinitialization sequence, itperforms memory transactions according tothe
following priority scheme (highest priority listed first):
1.IftheEMIF's backlog refresh counter isattheRefresh Must urgency level, theEMIF performs multiple
SDRAM auto refresh cycles until theRefresh Release urgency level isreached.
2.IfanSDRAM orasynchronous read hasbeen requested, theEMIF performs aread operation.
3.IftheEMIF's backlog refresh counter isattheRefresh Need urgency level, theEMIF performs an
SDRAM auto refresh cycle.
4.IfanSDRAM orasynchronous write hasbeen requested, theEMIF performs awrite operation.
5.IftheEMIF's backlog refresh counter isattheRefresh May orRefresh Release urgency level, the
EMIF performs anSDRAM auto refresh cycle.
6.Ifthevalue oftheSRbitinSDCR hasbeen setto1,theEMIF willenter theself-refresh state as
described inSection 21.2.5.7 .
After taking oneoftheactions listed above, theEMIF then returns tothetopofthepriority listtodetermine
itsnext action.
Because theEMIF does notissue auto-refresh cycles when intheself-refresh state, theabove priority
scheme does notapply when inthisstate. See Section 21.2.5.7 fordetails ontheoperation oftheEMIF
when intheself-refresh state.

<!-- Page 826 -->

EMIF Module Architecture www.ti.com
826 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.14 System Considerations
This section describes various system considerations tokeep inmind when operating theEMIF.
21.2.14.1 Asynchronous Request Times
Inasystem thatinterfaces toboth SDRAM andasynchronous memory, theasynchronous requests must
nottake longer than thesmaller ofthefollowing twovalues:
*tRAS(typically 120μs)-toavoid violating themaximum time allowed between issuing anACTV and
PRE command totheSDRAM.
*tRefresh Rate×11(typically 15.7μs×11=172.7μs)-toavoid refresh violations ontheSDRAM.
The length ofanasynchronous request iscontrolled bymultiple factors, theprimary factor being the
number ofaccess cycles required tocomplete therequest. Forexample, anasynchronous request for
4bytes willrequire four access cycles using an8-bit data busandonly twoaccess cycle using a16-bit
data bus. The maximum request size thattheEMIF canbesent is16words, therefore themaximum
number ofaccess cycles permemory request is64when theEMIF isconfigured with an8-bit data
bus. The length oftheindividual access cycles thatmake uptheasynchronous request isdetermined
bytheprogrammed setup, strobe, hold, andturnaround values, butcanalso beextended with the
assertion oftheEMIF_nWAIT input signal uptoaprogrammed maximum limit. Itisuptotheuser to
make sure thatanentire asynchronous request does notexceed thetiming values listed above when
also interfacing toanSDRAM device. This canbedone bylimiting theasynchronous timing
parameters.
21.2.14.2 Interface toExternal Peripheral orFIFO Memory
IfEMIF isused tointerface toanexternal peripheral orFIFO logic (forexample, UHPI), itisrecommended
tousethehost CPU's Memory Protection Unit (MPU) todefine thisexternal memory range asaregion
thatiseither strongly-ordered orofdevice type.
21.2.14.3 Interface toExternal SDRAM
IfEMIF isused tointerface toanexternal SDRAM, itisrecommended toburst asmuch aspossible to
normal memory toimprove theinterface bandwidth.

<!-- Page 827 -->

www.ti.com EMIF Module Architecture
827 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.2.15 Power Management
Power dissipation from theEMIF memory controller may bemanaged byfollowing methods:
*Self-refresh mode
*Power-down mode
*Gating input clocks tothemodule off
Gating input clocks offtotheEMIF memory controller achieves higher power savings when compared to
thepower savings ofself-refresh orpower down mode. The input clock VCLK3 canbeturned offthrough
theuseoftheGlobal Clock Module (GCM). Before gating clocks off,theEMIF memory controller must
place theSDR SDRAM memory inself-refresh mode. Iftheexternal memory requires acontinuous clock,
theVCLK3 clock domain must notbeturned offbecause thismay result indata corruption. See the
following subsections fortheproper procedures tofollow when stopping theEMIF memory controller
clocks.
21.2.15.1 Power Management Using Self-Refresh Mode
The EMIF canbeplaced intoaself-refresh state inorder toplace theattached SDRAM devices intoself-
refresh mode, which consumes less power formost SDRAM devices. Inthisstate, theattached SDRAM
device uses aninternal clock toperform itsown auto refresh cycles. This maintains thevalidity ofthedata
intheSDRAM without theneed foranyexternal commands. Refer toSection 21.2.5.7 formore details on
placing theEMIF intotheself-refresh state.
21.2.15.2 Power Management Using Power Down Mode
Inthepower down mode, EMIF drives EMIF_CKE lowtolower thepower consumption. EMIF_CKE goes
high when there isaneed tosend refresh (REFR) commands, after which EMIF_CKE isagain driven low.
EMIF_CKE remains lowuntil anyrequest arrives. Refer toSection 21.2.5.8 formore details onplacing
EMIF inpower down mode.
21.2.16 Emulation Considerations
EMIF memory controller remains fully functional during emulation halts inorder toallow emulation access
toexternal memory.

<!-- Page 828 -->

EMIF Registers www.ti.com
828 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3 EMIF Registers
The external memory interface (EMIF) iscontrolled byprogramming itsinternal memory-mapped registers
(MMRs). Table 21-24 lists thememory-mapped registers fortheEMIF.
NOTE: AllEMIF MMRs, except SDCR, support only word (32-bit) accesses. Performing abyte (8-
bit)orhalfword (16-bit) write tothese registers results inundefined behavior. The SDCR is
byte writable toallow thesetting oftheSR,PD,andPDWR bitswithout triggering the
SDRAM initialization sequence.
The EMIF registers must always beaccessed using 32-bit accesses (unless otherwise specified inthis
chapter). The base address oftheEMIF memory-mapped registers isFCFF E800h.
Table 21-24. External Memory Interface (EMIF) Registers
Offset Acronym Register Description Section
00h MIDR Module IDRegister Section 21.3.1
04h AWCC Asynchronous Wait Cycle Configuration Register Section 21.3.2
08h SDCR SDRAM Configuration Register Section 21.3.3
0Ch SDRCR SDRAM Refresh Control Register Section 21.3.4
10h CE2CFG Asynchronous 1Configuration Register Section 21.3.5
14h CE3CFG Asynchronous 2Configuration Register Section 21.3.5
18h CE4CFG Asynchronous 3Configuration Register Section 21.3.5
1Ch CE5CFG Asynchronous 4Configuration Register Section 21.3.5
20h SDTIMR SDRAM Timing Register Section 21.3.6
3Ch SDSRETR SDRAM Self Refresh ExitTiming Register Section 21.3.7
40h INTRAW EMIF Interrupt Raw Register Section 21.3.8
44h INTMSK EMIF Interrupt Mask Register Section 21.3.9
48h INTMSKSET EMIF Interrupt Mask SetRegister Section 21.3.10
4Ch INTMSKCLR EMIF Interrupt Mask Clear Register Section 21.3.11
68h PMCR Page Mode Control Register Section 21.3.12
21.3.1 Module IDRegister (MIDR)
This isaread-only register indicating themodule IDoftheEMIF. The MIDR isshown inFigure 21-15 and
described inTable 21-25 .
Figure 21-15. Module IDRegister (MIDR) [offset =00]
31 0
REV
R-x
LEGEND: R=Read only; -n=value after reset
Table 21-25. Module IDRegister (MIDR) Field Descriptions
Bit Field Value Description
31-0 REV x Module IDofEMIF. See thedevice-specific data manual.

<!-- Page 829 -->

www.ti.com EMIF Registers
829 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.2 Asynchronous Wait Cycle Configuration Register (AWCC)
The asynchronous wait cycle configuration register (AWCC) isused toconfigure theparameters for
extended wait cycles. Both thepolarity oftheEMIF_nWAIT pin(s) andthemaximum allowable number of
extended wait cycles canbeconfigured. The AWCC isshown inFigure 21-16 anddescribed inTable 21-
26.Notalldevices support both EMIF_nWAIT[1] andEMIF_nWAIT[0], seethedevice-specific data
manual todetermine support oneach device.
NOTE: The EWbitintheasynchronous nconfiguration register (CEnCFG) must besettoallow for
theinsertion ofextended wait cycles.
Figure 21-16. Asynchronous Wait Cycle Configuration Register (AWCCR) [offset =04h]
31 30 29 28 27 24 23 22 21 20 19 18 17 16
Reserved WP1 WP0 Reserved CS5_WAIT CS4_WAIT CS3_WAIT CS2_WAIT
R-3h R/W-1 R/W-1 R-0 R/W-0 R/W-0 R/W-0 R/W-0
15 8 7 0
Reserved MAX_EXT_WAIT
R-0 R/W-80h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-26. Asynchronous Wait Cycle Configuration Register (AWCCR) Field Descriptions
Bit Field Value Description
31-30 Reserved 3h Reserved
29 WP1 EMIF_nWAIT[1] polarity bit.This bitdefines thepolarity oftheEMIF_nWAIT[1] pin.
0 Insert wait cycles ifEMIF_nWAIT[1] pinislow.
1 Insert wait cycles ifEMIF_nWAIT[1] pinishigh.
28 WP0 EMIF_nWAIT[0] polarity bit.This bitdefines thepolarity oftheEMIF_nWAIT[0] pin.
0 Insert wait cycles ifEMIF_nWAIT[0] pinislow.
1 Insert wait cycles ifEMIF_nWAIT[0] pinishigh.
27-24 Reserved 0 Reserved
23-22 CS5_WAIT 0-3h Chip Select 5WAIT signal selection. This signal determines which EMIF_nWAIT[ n]signal will
beused formemory accesses tochip select 5memory space. This device does notsupport
chip select 5,soanyvalue written tothisfield hasnoeffect.
21-20 CS4_WAIT Chip Select 4WAIT signal selection. This signal determines which EMIF_nWAIT[ n]signal will
beused formemory accesses tochip select 4memory space.
0 EMIF_nWAIT[0] pinisused tocontrol external wait states.
1h EMIF_nWAIT[1] pinisused tocontrol external wait states.
2h-3h Reserved
19-18 CS3_WAIT Chip Select 3WAIT signal selection. This signal determines which EMIF_nWAIT[ n]signal will
beused formemory accesses tochip select 3memory space.
0 EMIF_nWAIT[0] pinisused tocontrol external wait states.
1h EMIF_nWAIT[1] pinisused tocontrol external wait states.
2h-3h Reserved
17-16 CS2_WAIT Chip Select 2WAIT signal selection. This signal determines which EMIF_nWAIT[ n]signal will
beused formemory accesses tochip select 2memory space.
0 EMIF_nWAIT[0] pinisused tocontrol external wait states..
1h EMIF_nWAIT[1] pinisused tocontrol external wait states.
2h-3h Reserved
15-8 Reserved 0 Reserved
7-0 MAX_EXT_WAIT 0-FFh Maximum extended wait cycles. The EMIF willwait foramaximum of(MAX_EXT_WAIT +1)×
16clock cycles before itstops inserting asynchronous wait cycles andproceeds tothehold
period oftheaccess.

<!-- Page 830 -->

EMIF Registers www.ti.com
830 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.3 SDRAM Configuration Register (SDCR)
The SDRAM configuration register (SDCR) isused toconfigure various parameters oftheSDRAM
controller such asthenumber ofinternal banks, theinternal page size, andtheCAS latency tomatch
those oftheattached SDRAM device. Inaddition, thisregister isused toputtheattached SDRAM device
intoSelf-Refresh mode. The SDCR isshown inFigure 21-17 anddescribed inTable 21-27 .
NOTE: Writing tothelower three bytes ofthisregister willcause theEMIF tostart theSDRAM
initialization sequence described inSection 21.2.5.4 .
Figure 21-17. SDRAM Configuration Register (SDCR) [offset =08h]
31 30 29 28 24
SR PD PDWR Reserved
R/W-0 R/W-0 R/W-0 R-0
23 16
Reserved
R-0
15 14 13 12 11 9 8
Reserved NM(A)Reserved CL BIT11_9LOCK
R-0 R/W-0 R-0 R/W-3h R/W-0
7 6 4 3 2 0
Reserved IBANK Reserved PAGESIZE
R-0 R/W-2h R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
A.The NMbitmust besetto1iftheEMIF onyour device only has16data buspins.
Table 21-27. SDRAM Configuration Register (SDCR) Field Descriptions
Bit Field Value Description
31 SR Self-Refresh mode bit.This bitcontrols entering andexiting oftheSelf-Refresh mode described in
Section 21.2.5.7 .The field should bewritten using abyte-write totheupper byte ofSDCR toavoid
triggering theSDRAM initialization sequence.
0 Writing a0tothisbitwillcause connected SDRAM devices andtheEMIF toexittheSelf-Refresh
mode.
1 Writing a1tothisbitwillcause connected SDRAM devices andtheEMIF toenter theSelf-Refresh
mode.
30 PD Power Down bit.This bitcontrols entering andexiting ofthepower-down mode. The field should be
written using abyte-write totheupper byte ofSDCR toavoid triggering theSDRAM initialization
sequence. Ifboth SRandPDbitsareset,theEMIF willgointoSelf Refresh.
0 Writing a0tothisbitwillcause connected SDRAM devices andtheEMIF toexitthepower-down
mode.
1 Writing a1tothisbitwillcause connected SDRAM devices andtheEMIF toenter thepower-down
mode.
29 PDWR Perform refreshes during power down. Writing a1tothisbitwillcause EMIF toexitpower-down
state andissue andAUTO REFRESH command every time Refresh May level isset.
28-15 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
14 NM Narrow mode bit.This bitdefines whether a16-or32-bit-wide SDRAM isconnected totheEMIF.
This bitfield must always besetto1.Writing tothisfield triggers theSDRAM initialization
sequence.
0 32-bit SDRAM data busisused.
1 16-bit SDRAM data busisused.
13-12 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.

<!-- Page 831 -->

www.ti.com EMIF Registers
831 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Table 21-27. SDRAM Configuration Register (SDCR) Field Descriptions (continued)
Bit Field Value Description
11-9 CL CAS Latency. This field defines theCAS latency tobeused when accessing connected SDRAM
devices. A1must besimultaneously written totheBIT11_9LOCK bitfield ofthisregister inorder to
write totheCLbitfield. Writing tothisfield triggers theSDRAM initialization sequence.
0-1h Reserved
2h CAS latency =2EMIF_CLK cycles
3h CAS latency =3EMIF_CLK cycles
4h-7h Reserved
8 BIT11_9LOCK Bits11to9lock. CLcanonly bewritten ifBIT11_9LOCK issimultaneously written with a1.
BIT11_9LOCK isalways read as0.Writing tothisfield triggers theSDRAM initialization sequence.
0 CLcannot bewritten.
1 CLcanbewritten.
7 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
6-4 IBANK Internal SDRAM Bank size. This field defines number ofbanks inside theconnected SDRAM
devices. Writing tothisfield triggers theSDRAM initialization sequence.
0 1bank SDRAM devices.
1 2bank SDRAM devices.
2 4bank SDRAM devices.
3h-7h Reserved.
3 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
2-0 PAGESIZE Page Size. This field defines theinternal page size ofconnected SDRAM devices. Writing tothis
field triggers theSDRAM initialization sequence.
0 8column address bits(256 elements perrow)
1h 9column address bits(512 elements perrow)
2h 10column address bits(1024 elements perrow)
3h 11column address bits(2048 elements perrow)
4h-7h Reserved
21.3.4 SDRAM Refresh Control Register (SDRCR)
The SDRAM refresh control register (SDRCR) isused toconfigure therate atwhich connected SDRAM
devices willbeautomatically refreshed bytheEMIF. Refer toSection 21.2.5.6 ontherefresh controller for
more details. The SDRCR isshown inFigure 21-18 anddescribed inTable 21-28 .
Figure 21-18. SDRAM Refresh Control Register (SDRCR) [offset =0Ch]
31 16
Reserved
R-0
15 13 12 0
Reserved RR
R-0 R/W-80h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-28. SDRAM Refresh Control Register (SDRCR) Field Descriptions
Bit Field Value Description
31-13 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
12-0 RR 0-1FFFh Refresh Rate. This field isused todefine theSDRAM refresh period interms ofEMIF_CLK cycles.
Writing avalue <0x0020 tothisfield willcause ittobeloaded with (2×T_RFC) +1value from the
SDRAM timing register (SDTIMR).

<!-- Page 832 -->

EMIF Registers www.ti.com
832 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.5 Asynchronous nConfiguration Registers (CE2CFG-CE5CFG)
The asynchronous nconfiguration registers (CE2CFG, CE3CFG, CE4CFG, andCE5CFG) areused to
configure theshaping oftheaddress andcontrol signals during anaccess toasynchronous memory
connected toCS2, CS3, CS4, andCS5, respectively. CS5 isnotavailable onthisdevice. Itisalso used to
program thewidth ofasynchronous interface andtoselect from various modes ofoperation. This register
canbewritten prior toanytransfer, andanyasynchronous transfer following thewrite willusethenew
configuration. The CEnCFG isshown inFigure 21-19 anddescribed inTable 21-29 .
Figure 21-19. Asynchronous nConfiguration Register (CEnCFG) [offset =10h-1Ch]
31 30 29 26 25 24
SS EW(A)W_SETUP W_STROBE(B)
R/W-0 R/W-0 R/W-Fh R/W-3Fh
23 20 19 17 16
W_STROBE(B)W_HOLD R_SETUP
R/W-3Fh R/W-7h R/W-Fh
15 13 12 7 6 4 3 2 1 0
R_SETUP R_STROBE(B)R_HOLD TA ASIZE
R/W-Fh R/W-3Fh R/W-7h R/W-3h R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
A.The EWbitmust becleared to0.
B.This bitfield must becleared to0iftheEMIF onyour device does nothave anEMIF_nWAIT pin.
Table 21-29. Asynchronous nConfiguration Register (CEnCFG) Field Descriptions
Bit Field Value Description
31 SS Select Strobe bit.This bitdefines whether theasynchronous interface operates inNormal Mode or
Select Strobe Mode. See Section 21.2.6 fordetails onthetwomodes ofoperation.
0 Normal Mode enabled.
1 Select Strobe Mode enabled.
30 EW Extend Wait bit.This bitdefines whether extended wait cycles willbeenabled. See Section 21.2.6.6 on
extended wait cycles fordetails. This bitfield must besetto0,iftheEMIF onyour device does nothave
anEMIF_nWAIT pin.
0 Extended wait cycles disabled.
1 Extended wait cycles enabled.
29-26 W_SETUP 0-Fh Write setup width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
25-20 W_STROBE 0-3Fh Write strobe width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
19-17 W_HOLD 0-7h Write hold width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
16-13 R_SETUP 0-Fh Read setup width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
12-7 R_STROBE 0-3Fh Read strobe width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
6-4 R_HOLD 0-7h Read hold width inEMIF_CLK cycles, minus onecycle. See Section 21.2.6.3 fordetails.
3-2 TA 0-3h Minimum Turn-Around time. This field defines theminimum number ofEMIF_CLK cycles between reads
andwrites, minus onecycle. See Section 21.2.6.3 fordetails.
1-0 ASIZE Asynchronous Data Bus Width. This field defines thewidth oftheasynchronous device 'sdata bus.
0 8-bit data bus
1h 16-bit data bus
2h-3h Reserved

<!-- Page 833 -->

www.ti.com EMIF Registers
833 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.6 SDRAM Timing Register (SDTIMR)
The SDRAM timing register (SDTIMR) isused toprogram many oftheSDRAM timing parameters.
Consult theSDRAM datasheet forinformation ontheappropriate values toprogram intoeach field. The
SDTIMR isshown inFigure 21-20 anddescribed inTable 21-30 .
Figure 21-20. SDRAM Timing Register (SDTIMR) [offset =20h]
31 27 26 24 23 22 20 19 18 16
T_RFC T_RP Rsvd T_RCD Rsvd T_WR
R/W-8h R/W-2h R-0 R/W-2h R-0 R/W-1h
15 12 11 8 7 6 4 3 0
T_RAS T_RC Rsvd T_RRD Reserved
R/W-5h R/W-8h R-0 R/W-1h R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-30. SDRAM Timing Register (SDTIMR) Field Descriptions
Bit Field Value Description
31-27 T_RFC 0-1Fh Specifies theTrfc value oftheSDRAM. This defines theminimum number ofEMIF_CLK cycles from
Refresh (REFR) toRefresh (REFR), minus 1:
T_RFC =(Trfc/tEMIF_CLK )-1
26-24 T_RP 0-7h Specifies theTrpvalue oftheSDRAM. This defines theminimum number ofEMIF_CLK cycles from
Precharge (PRE) toActivate (ACTV) orRefresh (REFR) command, minus 1:
T_RP =(Trp/tEMIF_CLK )-1
23 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write thedefault
value of0.
22-20 T_RCD 0-7h Specifies theTrcd value oftheSDRAM. This defines theminimum number ofEMIF_CLK cycles from
Active (ACTV) toRead (READ) orWrite (WRT), minus 1:
T_RCD =(Trcd/tEMIF_CLK )-1
19 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write thedefault
value of0.
18-16 T_WR 0-7h Specifies theTwrvalue oftheSDRAM. This defines theminimum number ofEMIF_CLK cycles from
lastWrite (WRT) toPrecharge (PRE), minus 1:
T_WR =(Twr/tEMIF_CLK )-1
15-12 T_RAS 0-Fh Specifies theTras value oftheSDRAM. This defines theminimum number ofEMIF_CLK clock cycles
from Activate (ACTV) toPrecharge (PRE), minus 1:
T_RAS =(Tras/tEMIF_CLK )-1
11-8 T_RC 0-Fh Specifies theTrcvalue oftheSDRAM. This defines theminimum number ofEMIF_CLK clock cycles
from Activate (ACTV) toActivate (ACTV), minus 1:
T_RC =(Trc/tEMIF_CLK )-1
7 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write thedefault
value of0.
6-4 T_RRD 0-7h Specifies theTrrd value oftheSDRAM. This defines theminimum number ofEMIF_CLK clock cycles
from Activate (ACTV) toActivate (ACTV) foradifferent bank, minus 1:
T_RRD =(Trrd/tEMIF_CLK )-1
3-0 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write thedefault
value of0.

<!-- Page 834 -->

EMIF Registers www.ti.com
834 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.7 SDRAM Self Refresh Exit Timing Register (SDSRETR)
The SDRAM selfrefresh exittiming register (SDSRETR) isused toprogram theamount oftime between
when theSDRAM exits Self-Refresh mode andwhen theEMIF issues another command. The SDSRETR
isshown inFigure 21-21 anddescribed inTable 21-31 .
Figure 21-21. SDRAM SelfRefresh ExitTiming Register (SDSRETR) [offset =3Ch]
31 16
Reserved
R-0
15 5 4 0
Reserved T_XS
R-0 R/W-9h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-31. SDRAM SelfRefresh ExitTiming Register (SDSRETR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reserved. The reserved bitlocation isalways read as0.
4-0 T_XS 0-1Fh This field specifies theminimum number ofECLKOUT cycles from Self-Refresh exittoanycommand,
minus one.
T_XS =Txsr /tEMIF_CLK -1

<!-- Page 835 -->

www.ti.com EMIF Registers
835 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.8 EMIF Interrupt Raw Register (INTRAW)
The EMIF interrupt rawregister (INTRAW) isused tomonitor andclear theEMIF 'shardware-generated
Asynchronous Timeout Interrupt. The ATbitinthisregister willbesetwhen anAsynchronous Timeout
occurs regardless ofthestatus oftheEMIF interrupt mask setregister (INTMSKSET) andEMIF interrupt
mask clear register (INTMSKCLR). Writing a1tothisbitwillclear it.The EMIF onsome devices does not
have theEMIF_nWAIT pin;therefore, these registers andfields arereserved onthose devices. The
INTRAW isshown inFigure 21-22 anddescribed inTable 21-32 .
Figure 21-22. EMIF Interrupt Raw Register (INTRAW) [offset =40h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved WR LT AT
R-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing 0hasnoeffect); -n=value after reset
Table 21-32. EMIF Interrupt Raw Register (INTRAW) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write thedefault
value of0.
2 WR Wait Rise. This bitissetto1byhardware toindicate thatarising edge ontheEMIF_nWAIT pinhas
occurred.
0 Indicates thatarising edge hasnotoccurred ontheEMIF_nWAIT pin.Writing a0hasnoeffect.
1 Indicates thatarising edge hasoccurred ontheEMIF_nWAIT pin.Writing a1willclear thisbitandthe
WR_MASKED bitintheEMIF interrupt masked register (INTMSK).
1 LT Line Trap. Setto1byhardware toindicate illegal memory access type orinvalid cache linesize.
0 Writing a0hasnoeffect.
1 Indicates thatalinetrap hasoccurred. Writing a1willclear thisbitaswell astheLT_MASKED bitin
theEMIF interrupt masked register(INTMSK).
0 AT Asynchronous Timeout. This bitissetto1byhardware toindicate thatduring anextended
asynchronous memory access cycle, theEMIF_nWAIT pindidnotgoinactive within thenumber of
cycles defined bytheMAX_EXT_WAIT field intheasynchronous wait cycle configuration register
(AWCC).
0 Indicates thatanAsynchronous Timeout hasnotoccurred. Writing a0hasnoeffect.
1 Indicates thatanAsynchronous Timeout hasoccurred. Writing a1willclear thisbitaswell asthe
AT_MASKED bitintheEMIF interrupt masked register (INTMSK).

<!-- Page 836 -->

EMIF Registers www.ti.com
836 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.9 EMIF Interrupt Masked Register (INTMSK)
Like theEMIF interrupt rawregister (INTRAW), theEMIF interrupt masked register (INTMSK) isused to
monitor andclear thestatus oftheEMIF 'shardware-generated Asynchronous Timeout Interrupt. The main
difference between thetworegisters isthatwhen theAT_MASKED bitinthisregister isset,anactive-high
pulse willbesent totheCPU interrupt controller. Also, theAT_MASKED bitfield inINTMSK isonly setto
1iftheassociated interrupt hasbeen enabled intheEMIF interrupt mask setregister (INTMSKSET). The
EMIF onsome devices does nothave theEMIF_nWAIT pin,therefore, these registers andfields are
reserved onthose devices. The INTMSK isshown inFigure 21-23 anddescribed inTable 21-33 .
Figure 21-23. EMIF Interrupt Mask Register (INTMSK) [offset =44h]
31 8
Reserved
R-0
7 3 2 1 0
Reserved WR_MASKED LT_MASKED AT_MASKED
R-0 R/W1C-0 R/W1C-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear (writing 0hasnoeffect); -n=value after reset
Table 21-33. EMIF Interrupt Mask Register (INTMSK) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
2 WR_MASKED Wait Rise Masked. This bitissetto1byhardware toindicate arising edge hasoccurred onthe
EMIF_nWAIT pin,provided thattheWR_MASK_SET bitissetto1intheEMIF interrupt mask set
register (INTMSKSET).
0 Indicates thatawait riseinterrupt hasnotbeen generated. Writing a0hasnoeffect.
1 Indicates thatawait riseinterrupt hasbeen generated. Writing a1willclear thisbitandtheWRbit
intheEMIF interrupt rawregister (INTRAW).
1 LT_MASKED Masked Line Trap. Setto1byhardware toindicate illegal memory access type orinvalid cache line
size, only iftheLT_MASK_SET bitintheEMIF interrupt mask setregister (INTMSKSET) issetto1.
0 Writing a0hasnoeffect.
1 Writing a1willclear thisbitaswell astheLTbitintheEMIF interrupt rawregister(INTRAW).
0 AT_MASKED Asynchronous Timeout Masked. This bitissetto1byhardware toindicate thatduring anextended
asynchronous memory access cycle, theEMIF_nWAIT pindidnotgoinactive within thenumber of
cycles defined bytheMAX_EXT_WAIT field intheasynchronous wait cycle configuration register
(AWCC), provided thattheAT_MASK_SET bitissetto1intheEMIF interrupt mask setregister
(INTMSKSET).
0 Indicates thatanAsynchronous Timeout Interrupt hasnotbeen generated. Writing a0hasno
effect.
1 Indicates thatanAsynchronous Timeout Interrupt hasbeen generated. Writing a1willclear thisbit
aswell astheATbitintheEMIF interrupt rawregister (INTRAW).

<!-- Page 837 -->

www.ti.com EMIF Registers
837 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.10 EMIF Interrupt Mask SetRegister (INTMSKSET)
The EMIF interrupt mask setregister (INTMSKSET) isused toenable theAsynchronous Timeout
Interrupt. Ifread as1,theAT_MASKED bitintheEMIF interrupt masked register (INTMSK) willbesetand
aninterrupt willbegenerated when anAsynchronous Timeout occurs. Ifread as0,theAT_MASKED bit
willalways read 0andnointerrupt willbegenerated when anAsynchronous Timeout occurs. Writing a1
totheAT_MASK_SET bitenables theAsynchronous Timeout Interrupt. The EMIF onsome devices does
nothave theEMIF_nWAIT pin;therefore, these registers andfields arereserved onthose devices. The
INTMSKSET isshown inFigure 21-24 anddescribed inTable 21-34 .
Figure 21-24. EMIF Interrupt Mask SetRegister (INTMSKSET) [offset =48h]
31 16
Reserved
R-0
15 3 2 1 0
Reserved WR_MASK_SET LT_MASK_SET AT_MASK_SET
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-34. EMIF Interrupt Mask SetRegister (INTMSKSET) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
2 WR_MASK_SET Wait Rise Mask Set. This bitdetermines whether ornotthewait riseInterrupt isenabled. Writing a
1tothisbitsets thisbit,sets theWR_MASK_CLR bitintheEMIF interrupt mask clear register
(INTMSKCLR), andenables thewait riseinterrupt. Toclear thisbit,a1must bewritten tothe
WR_MASK_CLR bitinINTMSKCLR.
0 Indicates thatthewait riseinterrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thatthewait riseinterrupt isenabled. Writing a1sets thisbitandtheWR_MASK_CLR bit
intheEMIF interrupt mask clear register (INTMSKCLR).
1 LT_MASK_SET Mask setforLT_MASKED bitintheEMIF interrupt mask register (INTMSK).
0 Indicates thatthelinetrap interrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thatthelinetrap interrupt isenabled. Writing a1sets thisbitandtheLT_MASK_CLR bit
intheEMIF interrupt mask clear register (INTMSKCLR).
0 AT_MASK_SET Asynchronous Timeout Mask Set. This bitdetermines whether ornottheAsynchronous Timeout
Interrupt isenabled. Writing a1tothisbitsets thisbit,sets theAT_MASK_CLR bitintheEMIF
interrupt mask clear register (INTMSKCLR), andenables theAsynchronous Timeout Interrupt. To
clear thisbit,a1must bewritten totheAT_MASK_CLR bitoftheEMIF interrupt mask clear
register (INTMSKCLR).
0 Indicates thattheAsynchronous Timeout Interrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thattheAsynchronous Timeout Interrupt isenabled. Writing a1sets thisbitandthe
AT_MASK_CLR bitintheEMIF interrupt mask clear register (INTMSKCLR).

<!-- Page 838 -->

EMIF Registers www.ti.com
838 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.11 EMIF Interrupt Mask Clear Register (INTMSKCLR)
The EMIF interrupt mask clear register (INTMSKCLR) isused todisable theAsynchronous Timeout
Interrupt. Ifread as1,theAT_MASKED bitintheEMIF interrupt masked register (INTMSK) willbesetand
aninterrupt willbegenerated when anAsynchronous Timeout occurs. Ifread as0,theAT_MASKED bit
willalways read 0andnointerrupt willbegenerated when anAsynchronous Timeout occurs. Writing a1
totheAT_MASK_CLR bitdisables theAsynchronous Timeout Interrupt. The EMIF onsome devices does
nothave theEMIF_nWAIT pin,therefore, these registers andfields arereserved onthose devices. The
INTMSKCLR isshown inFigure 21-25 anddescribed inTable 21-35 .
Figure 21-25. EMIF Interrupt Mask Clear Register (INTMSKCLR) [offset =4Ch]
31 16
Reserved
R-0
15 3 2 1 0
Reserved WR_MASK_CLR LT_MASK_CLR AT_MASK_CLR
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 21-35. EMIF Interrupt Mask Clear Register (INTMSKCLR) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved. The reserved bitlocation isalways read as0.Ifwriting tothisfield, always write the
default value of0.
2 WR_MASK_CLR Wait Rise Mask Clear. This bitdetermines whether ornotthewait riseinterrupt isenabled. Writing
a1tothisbitclears thisbit,clears theWR_MASK_SET bitintheEMIF interrupt mask setregister
(INTMSKSET), anddisables thewait riseinterrupt. Tosetthisbit,a1must bewritten tothe
WR_MASK_SET bitinINTMSKSET.
0 Indicates thatthewait riseinterrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thatthewait riseinterrupt isenabled. Writing a1clears thisbitandtheWR_MASK_SET
bitintheEMIF interrupt mask setregister (INTMSKSET).
1 LT_MASK_CLR Line trap Mask Clear. This bitdetermines whether ornotthelinetrap interrupt isenabled. Writing a
1tothisbitclears thisbit,clears theLT_MASK_SET bitintheEMIF interrupt mask setregister
(INTMSKSET), anddisables thelinetrap interrupt. Tosetthisbit,a1must bewritten tothe
LT_MASK_SET bitinINTMSKSET.
0 Indicates thatthelinetrap interrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thatthelinetrap interrupt isenabled. Writing a1clears thisbitandtheLT_MASK_SET bit
intheEMIF interrupt mask setregister (INTMSKSET).
0 AT_MASK_CLR Asynchronous Timeout Mask Clear. This bitdetermines whether ornottheAsynchronous Timeout
Interrupt isenabled. Writing a1tothisbitclears thisbit,clears theAT_MASK_SET bitintheEMIF
interrupt mask setregister (INTMSKSET), anddisables theAsynchronous Timeout Interrupt. Toset
thisbit,a1must bewritten totheAT_MASK_SET bitoftheEMIF interrupt mask setregister
(INTMSKSET).
0 Indicates thattheAsynchronous Timeout Interrupt isdisabled. Writing a0hasnoeffect.
1 Indicates thattheAsynchronous Timeout Interrupt isenabled. Writing a1clears thisbitandthe
AT_MASK_SET bitintheEMIF interrupt mask setregister (INTMSKSET).

<!-- Page 839 -->

www.ti.com EMIF Registers
839 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.3.12 Page Mode Control Register (PMCR)
The page mode control register (PMCR) isshown inFigure 21-26 anddescribed inTable 21-36 .This
register isconfigured when using NOR Flash page mode.
Figure 21-26. Page Mode Control Register (PMCR) [offset =68h]
31 26 25 24
CS5_PG_DEL CS5_PG_SIZE CS5_PG_MD_EN
R/W-3Fh R/W-0 R/W-0
23 18 17 16
CS4_PG_DEL CS4_PG_SIZE CS4_PG_MD_EN
R/W-3Fh R/W-0 R/W-0
15 10 9 8
CS3_PG_DEL CS3_PG_SIZE CS3_PG_MD_EN
R/W-3Fh R/W-0 R/W-0
7 2 1 0
CS2_PG_DEL CS2_PG_SIZE CS2_PG_MD_EN
R/W-3Fh R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 21-36. Page Mode Control Register (PMCR) Field Descriptions
Bit Field Value Description
31-26 CS5_PG_DEL 1-3Fh Page access delay forNOR Flash connected onCS5. CS5 isnotavailable onthisdevice.
25 CS5_PG_SIZE Page Size forNOR Flash connected onCS5. CS5 isnotavailable onthisdevice.
24 CS5_PG_MD_EN Page Mode enable forNOR Flash connected onCS5. CS5 isnotavailable onthisdevice.
23-18 CS4_PG_DEL 1-3Fh Page access delay forNOR Flash connected onCS4. Number ofEMIF_CLK cycles required for
thepage read data tobevalid, minus onecycle. This value must notbecleared to0.
17 CS4_PG_SIZE Page Size forNOR Flash connected onCS4.
0 Page size is4words
1 Page size is8words
16 CS4_PG_MD_EN Page Mode enable forNOR Flash connected onCS4.
0 Page mode disabled forthischip select
1 Page mode enabled forthischip select
15-10 CS3_PG_DEL 1-3Fh Page access delay forNOR Flash connected onCS3. Number ofEMIF_CLK cycles required for
thepage read data tobevalid, minus onecycle. This value must notbecleared to0.
9 CS3_PG_SIZE Page Size forNOR Flash connected onCS3.
0 Page size is4words
1 Page size is8words
8 CS3_PG_MD_EN Page Mode enable forNOR Flash connected onCS3.
0 Page mode disabled forthischip select
1 Page mode enabled forthischip select
7-2 CS2_PG_DEL 1-3Fh Page access delay forNOR Flash connected onCS2. Number ofEMIF_CLK cycles required for
thepage read data tobevalid, minus onecycle. This value must notbecleared to0.
1 CS2_PG_SIZE Page Size forNOR Flash connected onCS2.
0 Page size is4words
1 Page size is8words
0 CS2_PG_MD_EN Page Mode enable forNOR Flash connected onCS2.
0 Page mode disabled forthischip select
1 Page mode enabled forthischip select

<!-- Page 840 -->

Example Configuration www.ti.com
840 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.4 Example Configuration
This section presents anexample ofinterfacing theEMIF toboth anSDR SDRAM device andan
asynchronous flash device.
21.4.1 Hardware Interface
Figure 21-27 shows thehardware interface between theEMIF, aSamsung K4S641632H-TC(L)70 64Mb
SDRAM device, andtwoSHARP LH28F800BJE-PTTL90 8Mb Flash memory. The connection between
theEMIF andtheSDRAM isstraightforward, buttheconnection between theEMIF andtheflash deserves
adetailed look.
The address inputs fortheflash areprovided bythree sources. The A[18:0] address inputs areprovided
byacombination oftheEMIF_A andEMIF_BA pins according toSection 21.2.6.1 .RD/nBY signal from
oneflash isconnected toEMIF_nWAIT pinofEMIF.
Finally, thisexample configuration connects theEMIF_nWE pintothenWE input oftheflash andoperates
theEMIF inSelect Strobe Mode.
21.4.2 Software Configuration
The following sections describe how toconfigure theEMIF registers andbitfields tointerface theEMIF
with theSamsung K4S641632H-TC(L)70 SDRAM andtheSHARP LH28F800BJE-PTTL90 8Mb Flash
memory.
21.4.2.1 Configuring theSDRAM Interface
This section describes how toconfigure theEMIF tointerface with theSamsung K4S641632H-TC(L)70
SDRAM with aclock frequency offEMIF_CLK =100MHz. Procedure Adescribed inSection 21.2.5.5 is
followed which assumes thattheSDRAM power-up timing constraint were metduring theSDRAM Auto-
Initialization sequence after Reset.
21.4.2.1.1 PLL Programming fortheEMIF toK4S641632H-TC(L)70 Interface
The device global clock module (GCM) should firstbeprogrammed toselect thedesired EMIF_CLK
frequency. Before doing this, theSDRAM should beplaced inSelf-Refresh Mode bysetting theSRbitin
theSDRAM configuration register (SDCR). The SRbitshould besetusing abyte-write totheupper byte
oftheSDCR toavoid triggering theSDRAM Initialization Sequence. The EMIF_CLK frequency cannow
beconfigured tothedesired value byselecting theappropriate clock source fortheVCLK3 domain. Once
theVCLK3 domain frequency hasbeen configured, remove theSDRAM from Self-Refresh byclearing the
SRbitinSDCR, again with abyte-write.
Table 21-37. SRField Value FortheEMIF toK4S641632H-TC(L)70 Interface
Field Value Purpose
SR 1then 0 Toplace theEMIF intotheselfrefresh state

<!-- Page 841 -->

EMIF
EMIF_nCS[0]
EMIF_nCAS
EMIF_nRAS
EMIF_nWE
EMIF_CLK
EMIF_CKE
EMIF_BA[1]
EMIF_BA[0]
EMIF_ADDR[11:0]
EMIF_nDQM[0]
EMIF_nDQM[1]
EMIF_DATA[15:0]
EMIF_nCS[2]
EMIF_nCS[3]
EMIF_nOE
EMIF_nWAIT
EMIF_ADDR
[18:13] nRESET
ResetSDRAM
1M x 16
x 4 banknCE
nCAS
nRAS
nWE
CLK
CKE
BA[1]
BA[0]
A[11:0]
LDQM
UDQM
DQ[15:0]
FLASH
512k x 16A[0]
A[12:1]
DQ[15:0]
nCE
nWE
nOE
nRESET
A[18:13]
RY/BY
nBYTE0
nBYTE1
FLASH
512k x 16A[0]
A[12:1]
DQ[15:0]
nCE
nWE
nOE
nRESET
A[18:13]
RY/BY
nBYTE0
nBYTE1
www.ti.com Example Configuration
841 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-27. Example Configuration Interface

<!-- Page 842 -->

Example Configuration www.ti.com
842 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.4.2.1.2 SDRAM Timing Register (SDTIMR) Settings fortheEMIF toK4S641632H-TC(L)70 Interface
The fields oftheSDRAM timing register (SDTIMR) should beprogrammed firstasdescribed inTable 21-
38tosatisfy therequired timing parameters fortheK4S641632H-TC(L)70. Based onthese calculations, a
value of6111 4610h should bewritten toSDTIMR. Figure 21-28 shows agraphical description ofhow
SDTIMR should beprogrammed.
(1)The Samsung datasheet does notspecify atRFCvalue. Instead, Samsung specifies tRCastheminimum auto refresh period.
(2)The Samsung datasheet does notspecify atWRvalue. Instead, Samsung specifies tRDLaslastdata intorowprecharge minimum
delay.Table 21-38. SDTIMR Field Calculations fortheEMIF toK4S641632H-TC(L)70 Interface
Field Name FormulaValue from K4S641632H-TC(L)70
DatasheetValue Calculated for
Field
T_RFC T_RFC >=(tRFC×fEMIF_CLK )-1 tRC=68ns(min)(1)6
T_RP T_RP >=(tRP×fEMIF_CLK )-1 tRP=20ns(min) 1
T_RCD T_RCD >=(tRCD×fEMIF_CLK )-1 tRCD=20ns(min) 1
T_WR T_WR >=(tWR×fEMIF_CLK )-1 tRDL=2CLK =20ns(min)(2)1
T_RAS T_RAS >=(tRAS×fEMIF_CLK )-1 tRAS=49ns(min) 4
T_RC T_RC >=(tRC×fEMIF_CLK )-1 tRC=68ns(min) 6
T_RRD T_RRD >=(tRRD×fEMIF_CLK )-1 tRRD=14ns(min) 1
Figure 21-28. SDRAM Timing Register (SDTIMR)
31 27 26 24 23 22 20 19 18 16
00110 001 0 001 0 001
T_RFC T_RP Rsvd T_RCD Rsvd T_WR
15 12 11 8 7 6 4 3 0
0100 0110 0 001 0000
T_RAS T_RC Rsvd T_RRD Reserved

<!-- Page 843 -->

www.ti.com Example Configuration
843 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.4.2.1.3 SDRAM Self Refresh Exit Timing Register (SDSRETR) Settings fortheEMIF toK4S641632H-
TC(L)70 Interface
The SDRAM selfrefresh exittiming register (SDSRETR) should beprogrammed second tosatisfy thetXSR
timing requirement from theK4S641632H-TC(L)70 datasheet. Table 21-39 shows thecalculation ofthe
proper value toprogram intotheT_XS field ofthisregister. Based onthiscalculation, avalue of6hshould
bewritten toSDSRETR. Figure 21-29 shows how SDSRETR should beprogrammed.
(1)The Samsung datasheet does notspecify atXSRvalue. Instead, Samsung specifies tRCastheminimum required time after CKE
going high tocomplete selfrefresh exit.Table 21-39. RRCalculation fortheEMIF toK4S641632H-TC(L)70 Interface
Field Name FormulaValue from K4S641632H-TC(L)70
DatasheetValue Calculated for
Field
T_XS T_XS >=(tXSR×fEMIF_CLK )-1 tRC=68ns(min)(1)6
Figure 21-29. SDRAM SelfRefresh ExitTiming Register (SDSRETR)
31 16
0000 0000 0000 0000
Reserved
15 5 4 0
0000000 0000 00110
Reserved T_XS
21.4.2.1.4 SDRAM Refresh Control Register (SDRCR) Settings fortheEMIF toK4S641632H-TC(L)70
Interface
The SDRAM refresh control register (SDRCR) should next beprogrammed tosatisfy therequired refresh
rate oftheK4S641632H-TC(L)70. Table 21-40 shows thecalculation oftheproper value toprogram into
theRRfield ofthisregister. Based onthiscalculation, avalue of61Ah should bewritten toSDRCR.
Figure 21-30 shows how SDRCR should beprogrammed.
Table 21-40. RRCalculation fortheEMIF toK4S641632H-TC(L)70 Interface
Field Name Formula Values Value Calculated forField
RR RR≤fEMIF_CLK ×tRefresh Period
/ncyclesFrom SDRAM datasheet: tRefresh Period
=64ms;ncycles =4096 EMIF clock
rate: fEMIF_CLK =100MHzRR=1562 cycles =61Ah cycles

<!-- Page 844 -->

Example Configuration www.ti.com
844 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-30. SDRAM Refresh Control Register (SDRCR)
31 19 18 16
00000 0000 0000 000
Reserved Reserved
15 13 12 0
000 00110 0001 1010 (61Ah)
Reserved RR
21.4.2.1.5 SDRAM Configuration Register (SDCR) Settings fortheEMIF toK4S641632H-TC(L)70
Interface
Finally, thefields oftheSDRAM configuration register (SDCR) should beprogrammed asdescribed in
Table 21-37 toproperly interface with theK4S641632H-TC(L)70 device. Based onthese settings, avalue
of4720h should bewritten toSDCR. Figure 21-31 shows how SDCR should beprogrammed. The EMIF is
now ready toperform read andwrite accesses totheSDRAM.
Table 21-41. SDCR Field Values FortheEMIF toK4S641632H-TC(L)70 Interface
Field Value Purpose
SR 0 Toavoid placing theEMIF intotheselfrefresh state
NM 1 Toconfigure theEMIF fora16-bit data bus
CL 011b Toselect aCAS latency of3
BIT11_9LOCK 1 Toallow theCLfield tobewritten
IBANK 010b Toselect 4internal SDRAM banks
PAGESIZE 0 Toselect apage size of256words
Figure 21-31. SDRAM Configuration Register (SDCR)
31 30 29 28 24
0 0 0 00000
SR Reserved Reserved Reserved
23 18 17 16
000000 0 0
Reserved Reserved Reserved
15 14 13 12 11 9 8
0 1 0 0 011 1
Reserved NM Reserved Reserved CL BIT11_9LOCK
7 6 4 3 2 0
0 010 0 000
Reserved IBANK Reserved PAGESIZE

<!-- Page 845 -->

Setup
StrobeHoldTA
tDtD
tELQVtHtSUtEHQZ
DataEMIF_CLK
EMIF_nCS[n]
EMIF_A/
EMIF_BA
EMIF_D
EMIF_nOE
www.ti.com Example Configuration
845 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)21.4.2.2 Configuring theFlash Interface
This section describes how toconfigure theEMIF tointerface with thetwoofSHARP LH28F800BJE-
PTTL90 8Mb Flash memory with aclock frequency offEMIF_CLK =100MHz. The example assumes thatone
flash isconnected toEMIF_nCS2 andtheother toEMIF_nCS3.
21.4.2.2.1 Asynchronous 1Configuration Register (CE2CFG) Settings fortheEMIF toLH28F800BJE-
PTTL90 Interface
The asynchronous 1configuration register (CE2CFG) andasynchronous 2configuration register
(CE3CFG) aretheonly registers thatisnecessary toprogram forthisasynchronous interface (assuming
thatoneFlash isconnected toEMIF_nCS[2] andtheother toEMIF_nCS[3]. The SSbit(inboth registers)
should besetto1toenable Select Strobe Mode andtheASIZE field (inboth registers) should besetto1
toselect a16-bit interface. The other fields inthisregister control theshaping oftheEMIF signals, andthe
proper values canbedetermined byreferring totheACCharacteristics intheFlash datasheet andthe
device datasheet. Based onthefollowing calculations, avalue of8862 25BDh should bewritten to
CE2CFG. Table 21-42 andTable 21-43 show thepertinent ACCharacteristics forreads andwrites tothe
Flash device, andFigure 21-32 andFigure 21-33 show theassociated timing waveforms. Finally,
Figure 21-34 shows programming theCEnCFG (n=2,3)with thecalculated values.
Table 21-42. ACCharacteristics foraRead Access
ACCharacteristic Device Definition Min Max Unit
tSU EMIF Setup time, read EMIF_D before EMIF_CLK
high6.5 ns
tH EMIF Data hold time, read EMIF_D after EMIF_CLK
high1 ns
tD EMIF Output delay time, EMIF_CLK high tooutput
signal valid7 ns
tELQV Flash nCE toOutput Delay 90 ns
tEHQZ Flash nCE High toOutput inHigh Impedance 55 ns
Table 21-43. ACCharacteristics foraWrite Access
ACCharacteristic Device Definition Min Max Unit
tAVAV Flash Write Cycle Time 90 ns
tELEH Flash nCE Pulse Width Low 50 ns
tEHEL Flash nCE Pulse Width High (not shown in
Figure 21-33 )30 ns
Figure 21-32. LH28F800BJE-PTTL90 toEMIF Read Timing Waveforms

<!-- Page 846 -->

SetupStrobeHold
tAVAV
tELEH
DataAddressEMIF_CLK
EMIF_nCS[n]
EMIF_A/
EMIF_BA
EMIF_D
EMIF_nWE
Example Configuration www.ti.com
846 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)Figure 21-33. LH28F800BJE-PTTL90 toEMIF Write Timing Waveforms
The R_STROBE field should besettomeet thefollowing equation:
R_STROBE >=(tD+tELQV+tSU)×fEMIF_CLK -1
R_STROBE >=(7ns+90ns+6.5ns)×100MHz -1
R_STROBE >=9.35
R_STROBE =10
The R_HOLD field must belarge enough tosatisfy theEMIF Data hold time, tH:
R_HOLD >=tH×fEMIF_CLK -1
R_HOLD >=1ns×100MHz -1
R_HOLD >=-0.9
The R_HOLD field must also combine with theTAfield tosatisfy theFlash's nCE High toOutput inHigh
Impedance time, tEHQZ:
R_HOLD +TA>=(tD+tEHQZ)×fEMIF_CLK -2
R_HOLD +TA>=(7ns+55ns)×100MHz -2
R_HOLD +TA>=4.2
The largest value thatcanbeprogrammed intotheTAfield is3h,therefore thefollowing values canbe
used:
R_HOLD =2
TA=3
ForWrites, theW_STROBE field should besettosatisfy theFlash's nCE Pulse Width constraint, tELEH:
W_STROBE >=tELEH×fEMIF_CLK -1
W_STROBE >=50ns×100MHz -1
W_STROBE >=4

<!-- Page 847 -->

www.ti.com Example Configuration
847 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedExternal Memory Interface (EMIF)The W_SETUP andW_HOLD fields should combine tosatisfy theFlash's nCE Pulse Width High
constraint, tEHEL,when performing back-to-back writes:
W_SETUP +W_HOLD >=tEHEL×fEMIF_CLK -2
W_SETUP +W_HOLD >=30ns×100MHz -2
W_SETUP +W_HOLD >=1
Inaddition, theentire Write access length must satisfy theFlash's minimum Write Cycle Time, tAVAV:
W_SETUP +W_STROBE +W_HOLD >=tAVAV×fEMIF_CLK -3
W_SETUP +W_STROBE +W_HOLD >=90ns×100MHz -3
W_SETUP +W_STROBE +W_HOLD >=6
Solving theabove equations fortheWrite fields results inthefollowing possible solution:
W_SETUP =1
W_STROBE =5
W_HOLD =0
Adding a10ns(1cycle) margin toeach oftheperiods (excluding TAwhich isalready atitsmaximum) in
thisexample produces thefollowing recommended values:
W_SETUP =2h
W_STROBE =6h
W_HOLD =1h
R_SETUP =1h
R_STROBE =Bh
R_HOLD =3h
TA=3h
Figure 21-34. Asynchronous mConfiguration Register (m=1,2)(CEnCFG (n=2,3))
31 30 29 26 25 24
1 0 0010 00
SS EW W_SETUP W_STROBE
23 20 19 17 16
0110 001 0
W_STROBE W_HOLD R_SETUP
15 13 12 7 6 4 3 2 1 0
001 001011 011 11 01
R_SETUP R_STROBE R_HOLD TA ASIZE