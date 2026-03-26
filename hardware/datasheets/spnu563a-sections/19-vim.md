# Vectored Interrupt Manager (VIM) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 662-695

---


<!-- Page 662 -->

662 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleChapter 19
SPNU563A -March 2018
Vectored Interrupt Manager (VIM) Module
This chapter describes thebehavior ofthevectored interrupt manager (VIM) module ofthedevice family.
Topic ........................................................................................................................... Page
19.1 Overview ......................................................................................................... 663
19.2 Dual VIMforSafety ........................................................................................... 664
19.3 Device Level Interrupt Management .................................................................... 665
19.4 Interrupt Handling Inside VIM............................................................................. 668
19.5 Interrupt Vector Table (VIM RAM) ....................................................................... 672
19.6 VIMWakeup Interrupt ........................................................................................ 676
19.7 Capture Event Sources ..................................................................................... 677
19.8 Examples ......................................................................................................... 677
19.9 VIMControl Registers ....................................................................................... 680

<!-- Page 663 -->

www.ti.com Overview
663 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.1 Overview
The vectored interrupt manager (VIM) provides hardware assistance forprioritizing andcontrolling the
many interrupt sources present onadevice. Interrupts arecaused byevents outside ofthenormal flow of
program execution. Normally, these events require atimely response from thecentral processing unit
(CPU); therefore, when aninterrupt occurs, theCPU switches execution from thenormal program flow to
aninterrupt service routine (ISR).
The VIM module hasthefollowing features:
*Dual VIM forsafety
*Supports 127interrupt channels, inboth register vectored interrupt andhardware vectored interrupt
mode.
-Provides IRQ vector directly totheCPU VICport
-Provides FIQ/IRQ vector through registers
-Provides programmable priority andenable forinterrupt request lines
*Provides adirect hardware dispatch mechanism forfastest IRQ dispatch.
*Provides twosoftware dispatch mechanisms forbackward compatibility with earlier generation ofTI
processors.
-Index interrupt
-Register vectored interrupt
*ECC (Error Code Correction) protected vector interrupt table against softerrors.

<!-- Page 664 -->

VIM Interrupt
Vector Table 1
VIM
Core1
VIM
Core2To CPU1
To CPU2
VIM Interrupt
Vector Table 2MMR I/F1
INT_REQ
CCM
2 cyc
delay2 cyc
delay2 cyc
delay
Dual VIM forSafety www.ti.com
664 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.2 Dual VIMforSafety
Ablock diagram ofDual VIM forsafety support isshown inFigure 19-1 .Toreduce probability ofcommon
cause failure, theVIM module mimics thedual CPU scheme oftwocycle delayed operation ofthetwo
cores. Inthiscase, theMMR (Memory Mapped Register) interface tothesecond instance isdelayed by
twocycles. Similarly, theinterrupt inputs arealso delayed bytwocycles tothesecond instance.
Aseparate setof"2cycle "delayed versions ofoutput ports fortheCPU interrupt interface oftheVIM1 are
provided. These willbeused asoneofthecompare inputs ofCPU Compare Module (CCM). The CPU
interface signals ofVIM2 areused assecond setofinputs ofCCM.
VIM2 uses thesame address space asthatofVIM1. During LockStep mode, anywrite toVIM1 (including
theInterrupt Vector Table) willberouted toVIM2 aswell sothatthesecondary instance isprogrammed
exactly asthefirstoneandprovide compare diagnostic support. Auto initialization oftheVIM1 Interrupt
Vector Table willresult inVIM2 Interrupt Vector Table getting initialized aswell inthismode. Inthismode,
reads from VIM willreturn only VIM1 data.VIM2 registers andInterrupt Vector Table cannot beread outin
Locked mode.
Figure 19-1. Block Diagram ofDual VIMforSafety Support

<!-- Page 665 -->

ESM ADC LIN SPI DCAN NHETPeripherals - Generate Interrupt Requests
INT_REQ0   INT_REQ1 INT_REQ126
VIM
VBUSP
CPU RTI GCMCAPEVT[1:0] Wakeup_INTINT
TableIRQ
IndexFIQ
IndexIRQ
VectorFIQ
VectorIRQ FIQ IRQ
Vector
VIC Port- Interrupt Priority
- Interrupt Mapping- Interrupt Enable
- Interrupt Generation
Configuration Register   RegisterRegister   RegisterRequest Request t
(Direct
Hardware
Vector)Special Interrup ts CPU Interrupts
www.ti.com Device Level Interrupt Management
665 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.3 Device Level Interrupt Management
Ablock diagram ofdevice level interrupt handling isshown inFigure 19-2.When anevent occurs within a
peripheral, theperipheral makes aninterrupt request totheVIM. Then, VIM prioritizes therequests from
peripherals andprovides theaddress ofthehighest interrupt service routine (ISR) totheCPU. Finally,
CPU starts executing theISRinstructions from thataddress intheISR. Section 19.3.1 through
Section 19.3.3 provide additional details about these three steps.
Figure 19-2. Device Level Interrupt Block Diagram
19.3.1 Interrupt Generation atthePeripheral
Interrupt generation begins when anevent occurs within aperipheral module. Some examples ofinterrupt-
capable events areexpiration ofacounter within atimer module, receipt ofacharacter ina
communications module, andcompletion ofaconversion inananalog-to-digital converter (ADC) module.
Some device peripherals arecapable ofrequesting interrupts onmore than oneinterrupt request line.
Interrupts arenotalways generated when anevent occurs; theperipheral must make aninterrupt request
totheVIM based ontheevent occurrence. Typically, theperipheral contains:
*Aninterrupt flagbitforeach event tosignify theevent occurrence.
*Aninterrupt enable bittocontrol whether theevent occurrence causes aninterrupt request totheVIM.

<!-- Page 666 -->

Device Level Interrupt Management www.ti.com
666 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.3.2 Interrupt Handling attheCPU
The ARM CPU provides twovectors forinterrupt requests --fastinterrupt requests (FIQs) andnormal
interrupt requests (IRQs). FIQs arehigher priority than IRQs, andFIQinterrupts may interrupt IRQ
interrupts.
NOTE: The FIQimplemented inCortex-R4F/R5F isNon-Maskable Fast Interrupts (NMFI). Once FIQ
isenabled (byclearing FbitinCPSR), itcanNOT bedisabled bysetting FbitinCPSR. Only
areset oranFIQwillbeable tosettheFbitinCPSR. Byhardware, Non Maskable FIQare
notreentrant.
After reset (power reset orwarm reset), both FIQandIRQ aredisabled. The CPU may enable these
interrupt request channels individually within theCPSR (Current Program Status Register); CPSR bits6
and7must becleared toenable theFIQ(bit6)andIRQ (bit7)interrupt requests attheCPU. CPSR is
writable inprivilege mode only. Example 19-2 shows how toenable theIRQ andFIQthrough CPSR.
When theCPU receives aninterrupt request, theCPSR mode field changes toeither FIQorIRQ mode.
When anIRQ interrupt isreceived, theCPU disables other IRQ interrupts bysetting CPSR bit7.When an
FIQinterrupt isreceived, theCPU disables both IRQ andFIQinterrupts bysetting CPSR bits6and7.
Awrite of1toCPSR bit7disables theIRQ from CPU. However, awrite of1toCPSR bit6leaves it
unchanged. Example 19-2 also shows how todisable theIRQ through CPSR.

<!-- Page 667 -->

www.ti.com Device Level Interrupt Management
667 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.3.3 Software Interrupt Handling Options
The device supports three different possibilities forsoftware tohandle interrupts
1.Index interrupts mode (compatible with TMS470R1x legacy code)
After theinterrupt isreceived bytheCPU, theCPU branches to0x18 (IRQ) or0x1C (FIQ) toexecute
themain ISR. The main ISRroutine reads theoffset register (IRQINDEX, FIQINDEX) todetermine the
source oftheinterrupt.
This mode iscompatible with theTMS470R1x (CIM) module andprovides thesame interrupt registers.
This mode could beused iflegacy code needs tobereused, porting itfrom theTMS470R1x family.
However, imported software willnotbenefit from theVIM improvements.
Toport legacy software, theinterrupt vector at0x18 (IRQ) or0x1C (FIQ) only needs tobeabranch
statement toasoftware interrupt table. The software interrupt table reads thepending interrupt from a
vector offset register (FIQINDEX[7:0] forFIQinterrupts andIRQINDEX[7:0] forIRQ interrupts). All
pending interrupts canbeviewed intheINTREQ register. Example 19-4 shows how torespond toFIQ
with short latency inthismode.
2.Register vectored interrupts (automatically provide vector address toapplication)
Before enabling interrupts, theapplication software also hastoinitiate theinterrupt vector table (VIM
RAM).
Once theVIM receives aninterrupt, itloads theaddress ofISRfrom interrupt vector table, andstore it
intotheinterrupt vector register (IRQVECREG forIRQ interrupt, FIQVECREG forFIQinterrupt).
After theinterrupt isreceived bytheCPU, theCPU executes theinstruction placed at0x18 or0x1C
(IRQ orFIQvector) toload theaddress ofISR(interrupt vector) from theinterrupt vector register.
Example 19-3 illustrates theconfiguration fortheexception vectors using thismode.
3.Hardware vectored interrupts (automatically dispatch toISR, IRQ only)
Before enabling interrupts, theapplication software must initiate theinterrupt vector table (VIM RAM)
pointing totheISRforeach interrupt channel.
After theinterrupt (IRQ) isreceived bytheCPU, CPU reads theaddress ofISRdirectly from the
interface with VIM (VIC port) instead ofbranching to0x18. The CPU willbranch directly totheISR.
The hardware vectored interrupt behavior must beexplicitly enabled bysetting thevector enable (VE)
bitintheCP15 R1register. This bitresets to0,sothatthedefault state after reset isbackward
compatible toearlier ARM CPU. Example 19-1 shows how toenable thehardware vectored interrupt.
NOTE: This mode isNOT available forFIQ.
4.Software-Based Priority Decoding Scheme
Iftheapplication uses asoftware-based interrupt priority decoding scheme instead ofthehardware
vector capabilities, then there isanadditional step which was notrequired onearlier devices. This
version oftheVIM willhold aninterrupt request generated byaperipheral. When thesoftware clears
theinterrupt condition inthesource module (forexample, RTI, GIO, andsoon), then itmust also
perform anadditional clear oftheinterrupt request intheVIM. This canbedone byreading the
IRQVECREG register (Section 19.9.15 )orFIQVECREG register (Section 19.9.16 ),orbywriting a1to
theINTREQ(i) bit(Section 19.9.10 )intheVIM. This isnotnecessary ifanyofthethree previous
methods areused astheinterrupt request bitintheVIM willbeautomatically cleared when thevector
isread.

<!-- Page 668 -->

FIQ INDEXINT_
REQ0
IRQ FIQFIQ LEVEL
PRIORITY ENCODERIRQ LEVEL
PRIORITY ENCODERCHANNEL
MAPPINGINT_
REQ1INT_
REQ2INT_
REQ126INT_
REQ125
CHAN0
INTERRUPT
ENABLECHAN1 CHAN2 CHAN126 CHAN125
INT_
CHAN0
IRQ / FIQ
LEVELINT_
CHAN1INT_
CHAN2 CHAN126INT_
CHAN125
FIQ_
CHAN0FIQ_
CHAN1FIQ_
CHAN2FIQ_
CHAN126FIQ_
CHAN125IRQ_
CHAN2IRQ_
CHAN126IRQ_
CHAN125
IRQ INDEX
FIQ VECTOR IRQ VECTOR
FIQVECREG IRQVECREGPROGRAMMABLE INTERRUPT VECTOR TABLE
Phantom Vector
Channel 0 Vector
Channel 1 Vector
Channel 126 VectorFIQINDEX IRQINDEX
TO CPU
VIC PortRegister RegisterT o CPU T o CPU
Register Register
Interrupt Handling Inside VIM www.ti.com
668 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.4 Interrupt Handling Inside VIM
Ablock diagram oftheinterrupt handling inside VIM isshown inFigure 19-3
Figure 19-3. VIMInterrupt Handling Block Diagram

<!-- Page 669 -->

INT_
REQ0INT_
REQ1INT_
REQ2INT_
REQ126
CHAN27CHANMAP2[6:0]
INT_
REQ0INT_
REQ1INT_
REQ2INT_
REQ126
CHAN1267CHANMAP126[6:0]CHANNEL
MAPPING
127 Interrupt
ChannelsNOTE :
CHAN0 and CHAN1 are hard wired to
INT_REQ0 and INT_REQ1, can NOT
be remapped.
www.ti.com Interrupt Handling Inside VIM
669 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.4.1 VIMInterrupt Channel Mapping
The VIM support 128interrupt channels (including phantom interrupt). Ablock diagram oftheVIM
interrupt requests arrangement from peripheral modules totheinterrupt channels isprovided inFigure 19-
4.Each interrupt channel (CHANx) hasacorresponding mapping register bitfield (CHANMAPx[6:0]). This
mapping register determines which interrupt channel itmaps each VIM interrupt request. With this
scheme, thesame request canbemapped tomultiple channels. Alower numbered channel ineach FIQ
andIRQ hashigher priority. The programmability oftheVIM allows software tocontrol theinterrupt
priority.
Figure 19-4. VIMChannel Mapping
NOTE: CHAN127
CHAN127 hasnodedicated interrupt vector table entry. Therefore, CHAN127 shall NOT be
remapped toother INT_REQ (INT_REQ127 isreserved atdevice level).
Inthereset state, theVIM maps alloftheinterrupt requests inthesystem totheir respective interrupt
channels. Figure 19-5 shows thedefault state following thereset.
Figure 19-6 shows theVIM INT2 isremapped toboth Channel 2and4,andINT3 ismapped tochannel 3.
NOTE: Bymapping INT2 tochannel 2andchannel 4,andmapping INT3 tochannel 3,itispossible
forthesoftware tochange thepriority dynamically bychanging theENABLE register
(REQENASET andREQENACLR). When channel 2isenabled, thepriority is:
1.INT0
2.INT1
3.INT2
4.INT3
Disabling channel 2,thepriority becomes:
1.INT0
2.INT1
3.INT3
4.INT2

<!-- Page 670 -->

CHANMAP126CHANMAP125CHANMAP4CHANMAP3CHANMAP2CHANMAP1CHANMAP0 INT_REQ0 CHAN0
Interrupt
requests
(from peripheral)Peripheral 0 Channel 0 Vector 0xFFF82004
INT_REQ1 CHAN1 Peripheral 1 Channel 1 Vector 0xFFF82008
INT_REQ2 CHAN2 Peripheral 2 Channel 2 Vector 0xFFF8200C
INT_REQ3 CHAN3 Peripheral 3 Channel 3 Vector 0xFFF82010
INT_REQ4 CHAN4 Peripheral 4 Channel 4 Vector 0xFFF82014
INT_REQ125 CHAN125 Peripheral 125 Channel 125 Vector 0xFFF821F8
INT_REQ126 CHAN126 Peripheral 126 Channel 126 Vector 0xFFF821FCInterrupt
channels0xFFF82000 Phantom Vector
CHANMAP126CHANMAP125CHANMAP4CHANMAP3CHANMAP2CHANMAP1CHANMAP0 INT_REQ0 CHAN0
Interrupt
requests
(from peripheral)Peripheral 0 Channel 0 Vector 0xFFF82004
INT_REQ1 CHAN1 Peripheral 1 Channel 1 Vector 0xFFF82008
INT_REQ2 CHAN2 Peripheral 2 Channel 2 Vector 0xFFF8200C
INT_REQ3 CHAN3 Peripheral 3 Channel 3 Vector 0xFFF82010
INT_REQ4 CHAN4 Peripheral 4 Channel 4 Vector 0xFFF82014
INT_REQ125 CHAN125 Peripheral 125 Channel 125 Vector 0xFFF821F8
INT_REQ126 CHAN126 Peripheral 126 Channel 126 Vector 0xFFF821FCInterrupt
channels0xFFF82000 Phantom Vector
Interrupt Handling Inside VIM www.ti.com
670 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleFigure 19-5. VIMinDefault State
NOTE: CHAN0 andCHAN1 arehardwired toINT_REQ0 andINT_REQ1, sothey cannot be
remapped.
Figure 19-6. VIMinaProgrammed State
NOTE: CHAN0 andCHAN1 arehard wired toINT_REQ0 andINT_REQ1, sothey cannot be
remapped.

<!-- Page 671 -->

INT_CHAN0
INT_CHAN2INT_CHAN1
INT_CHAN127CHAN0INTREQ.0INT FLAG
FIQ_CHAN[0]
CHAN2INTREQ.2INT FLAG
1
0FIQ_CHAN[2]
IRQ_CHAN[2]
FIRQPR.2REQENA.2
Controlled by:
REQENASET.2
REQENACLR.2CHAN1INTREQ.1INT FLAG
FIQ_CHAN[1]
CHAN127INTREQ.127INT FLAG
1
0FIQ_CHAN[127]
IRQ_CHAN[127]
FIRQPR.127REQENA.127
Controlled by:
REQENASET.127
REQENACLR.127
www.ti.com Interrupt Handling Inside VIM
671 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.4.2 VIMInput Channel Management
Asshown inFigure 19-7,theVIM enables channels onachannel-by-channel basis (intheREQENASET
andREQENACLR registers); unused channels may bemasked toprevent spurious interrupts.
NOTE: The interrupt ENABLE register does notaffect thevalue ofINTREQ.
Figure 19-7. Interrupt Channel Management
Bydefault, interrupt CHAN0 ismapped toESM (Error Signal Module) high level interrupt andCHAN1 is
reserved forother NMI. Forsafety reasons, these twochannels aremapped toFIQonly andcanNOT be
disabled through ENABLE registers.
NOTE: NMIChannel
Channel 0andchannel 1arenotmaskable bytheREQENASET /REQENACLR bitandboth
channel arerouted exclusively toFIQ/NMI request line(FIRQPR0 andFIRQPR1 have no
effect).
The VIM prioritizes thereceived interrupts based upon aprogrammed prioritization scheme. The VIM can
send twointerrupt requests totheCPU simultaneously --oneIRQ andoneFIQ. Ifboth interrupt types are
enabled attheCPU level, then theFIQhasgreater priority andishandled first. Each interrupt channel,
except channel 0and1,canbeassigned tosend either anFIQorIRQ request totheCPU (inthe
FIRQPR register).
The VIM provides adefault prioritization scheme, which sends thelowest numbered active channel (in
each FIQandIRQ classes) totheCPU. Within theFIQandIRQ classes ofinterrupts, thelowest channel
hasthehighest priority interrupt. The channel number isprogrammable through register CHANMAPx.

<!-- Page 672 -->

Phantom Vector
Channel 0 Vector
Channel 1 Vector
Channel 125 Vector
Channel 126 Vector 0xFFF821FC0xFFF821F80xFFF820080xFFF820040xFFF82000Interrupt vector table address space
Interrupt Vector Table (VIM RAM) www.ti.com
672 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleAfter theVIM hasgenerated thevector corresponding tothehighest active IRQ, itupdates theFIQINDEX
ortheIRQINDEX register, depending ontheclass ofinterrupt. Then, itaccesses theinterrupt vector table
using thevector value tofetch theaddress ofthecorresponding ISR. Iftherequest isanFIQclass
interrupt, theaddress read from theinterrupt vector table, iswritten totheFIQVECREG register. Ifthe
request isanIRQ class interrupt, theaddress iswritten totheIRQVECREG register andputontheVIC
port oftheCPU (incase ofhardware vectored interrupt isenabled).
Alloftheinterrupt registers areupdated when anew high priority interrupt linebecomes active.
19.5 Interrupt Vector Table (VIM RAM)
Interrupt vector table stores theaddress ofISRs. During register vectored interrupt andhardware vectored
interrupt, VIM accesses theinterrupt vector table using thevector value tofetch theaddress ofthe
corresponding ISR.
Forsafety reasons, theinterrupt vector table hasprotection byECC toindicate corruption duetosoft
errors. The ECC scheme isimplemented asacontinuous background check based onmemory access.
The ECC logic inside VIM supports Single-bit Error Correction andDouble-bit Error Detection (SECDED).
Section 19.5.1 through Section 19.5.4 describe how ECC works intheinterrupt vector table.
NOTE: Writes totheinterrupt vector table ECC status register (ECCSTAT) andtheinterrupt vector
table ECC control register (ECCCTL) areinprivilege mode only.
19.5.1 Interrupt Vector Table Operation
The interrupt vector table isorganized in128words of32bits. Figure 19-8 shows theinterrupt memory
mapping. The table base address is0xFFF82000.
Figure 19-8. VIMInterrupt Address Memory Map
NOTE: The interrupt vector table only has128entries, onephantom vector and127interrupt
channels. Channel 127does nothave adedicated vector andshall notbeused.

<!-- Page 673 -->

www.ti.com Interrupt Vector Table (VIM RAM)
673 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleThere areseven bitsofECC per32-bit ISRaddress. When awrite isperformed intotheinterrupt vector
table, theECC bitsarecalculated forthe32-bit word andwritten intothecorresponding ECC region of
interrupt vector table ifECC isenabled inVIM.
NOTE: Only 32-bit write/read access areallowed oninterrupt vector table ifECC isrequired. Non
32-bit access might result inECC errors.
When aread occurs from theCPU orVIM, theVIM calculates theECC bitsfrom thedata coming from the
interrupt vector table andcompares ittotheknown good ECC value stored inthetable. Ifasingle-bit error
isdetected inthedata, theSECDED block willautomatically correct it.The read data willbeacorrected
oneinthiscase. Ifdouble-bit errors aredetected, theread data willbetheuncorrected one. The access of
thedata andtheECC bitsareperformed inthesame clock cycle.
The Double-Bit Error (DBE) andSingle-Bit Error (SBE) events willbegenerated only iftheECC feature is
enabled byECCENA field. Correction ofthedata upon aSBE event willbedone only ifenabled
EDAC_MODE field. Any double-bit error willbeflagged outtoESM module andasUERR flagin
ECCSTAT register. The address ofthedata forwhich UERR isdetected willalso bestored as
UERRADDR register.
Any single-bit error willberegistered intoSBERR flaginECCSTAT register andthecorresponding
address willbecaptured asSBERRADDR register. IfSBE_INT_EN field onECCCTL register issetto
enable value, then itwillbeflagged outtoESM module.
Since theinterrupt vector table may have anuncorrectable error (forexample, DBE), theFBVECADDR
register willprovide totheVICport, IRQVECREG andFIQVECREG, afall-back address toanISRthat
canrestore theinterrupt vector table content. The FB_VECADDR register should besetbefore initializing
theinterrupt intheinterrupt vector table, toavoid branching toanunpredictable location.
The normal operation isrestored when theECCSTAT iscleared bytheCPU. Itisrecommended torestore
thecontent oftheVIM before clearing theECCSTAT.
19.5.2 VIMECC Syndrome
The VIM ECC iscontrolled bytheECCENA bitsofECCCTL register. After reset, theSECDED feature is
disabled. The SECDED feature canbeenabled bywriting 0xA (1010b) intheECCENA[3:0] bitfield ofthe
ECCCTL register.
The ECC generation isdone according totheECC syndrome table asshown inTable 19-1 andTable 19-
2.Each ECC bitisbuilt bygenerating theparity oftheXORed bitsofthedata word, whereas ECC bit2
and3areeven parity andtheother bitsoddparity.
Table 19-1. ECC Syndrome Table
3
13
02
92
82
72
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
09876543210ECC
xxxxxxxx xxxxxxxx 6
xxxxxxxx xxxxxxxx 5
xx xxxxxx xx xxxxxx 4
xxx xxx xx xxx xxx xx 3
x x xx x xx xx x xx x xx x 2
x x x x x xxx x x x x x xxx 1
x xx x x xxx x x xxxx x x 0

<!-- Page 674 -->

Interrupt Vector Table (VIM RAM) www.ti.com
674 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleTable 19-2. ECC Error Bits forSyndrome Decode
6 5 4 3 2 1 0 ECC
x 6
x 5
x 4
x 3
x 2
x 1
x 0
19.5.3 Interrupt Vector Table Initialization
After reset, theinterrupt vector table content, including theECC bitsisnotinitialized. Therefore, the
interrupt vector table hastobeinitialized firstbefore enabling thecorresponding interrupt channel. This
canbedone either using thehardware initialization mechanism (inChapter Architecture Overview) orit
canbedone bywriting known values intotheinterrupt vector table bysoftware. IfECC isrequired, this
initialization should bedone after theECC functionality isenabled. Inthisway, thecorresponding ECC
bitswillbeautomatically updated. This initialization isonly required when vectored interrupts areused,
index interrupt management does notneed thetable tobeinitialized.
19.5.4 Interrupt Vector Table ECC Testing
TotesttheECC checking mechanism, theECC bitsallows manual insertion offaults. This option is
implemented using theTEST_DIAG_EN bitintheECCCTL register control bit.Once TEST_DIAG_EN is
enabled, theECC bitsaremapped to0xFFF82400. Inthismode, theuser canmodify theECC bits
without changing thedata bits. IfECCENA isdisabled, writing todata bitsdoes notautomatically update
ECC bits. The CPU reads andwrites under different conditions aresummarized inTable 19-3 and
Table 19-4.After that, user canforce faults intoeither thedata ortheECC bits. Finally, theECC error can
betriggered byreading interrupt vector table (not ECC bits) from VIM orCPU. Please note thatnoECC
checking willbedone forreads ofECC bitsintestmode.
Table 19-3. CPU Reads -Address Bit10Selects Between Normal Data andECC Bits
VBUSP_ ADDR(10) TEST_DIAG_ EN ECCENA Action
0 x(don 'tcare) x(don 'tcare) Normal RAM location read
1 x x ECC bitsread
Table 19-4. CPU Writes -Address Bit10Selects Between Normal Data andECC Bits
VBUSP_ ADDR(10) TEST_DIAG_ EN ECCENA Action
0 x 1 Normal RAM locations write with ECC bits
1 0 1 This write willbeblocked
1 1 1 ECC bitswrite
0 x 0 Normal RAM locations write without ECC bits
1 0 0 This write willbeblocked
1 1 0 This write isnotallowed

<!-- Page 675 -->

Word 0
Word 1
Word 2
Word 3
ECC031 0
0 6 31
Read 0
Read 0
ECC2 Read 0
ECC3 Read 032-bit only accessible
0xFFF82000
0xFFF82400
ECC1
www.ti.com Interrupt Vector Table (VIM RAM)
675 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleThe following sequence should beused forinjecting faults toECC bitsandtesting theECC check feature.
1.Write thedata locations ofVIM RAM with therequired patterns while keeping ECCENA active. The
ECC bitswillbeautomatically initialized along with data bits.
2.Enable ECC testmode using TEST_DIAG_EN field ofECCCTRL register.
3.Inthismode, itispossible tocorrupt ECC bitsusing anyofthefollowing methods.
*Read theECC bits, fliponebitandwrite back
*Read theECC bits, flip2bitsandwrite back
4.Depending onthekind corruption created, read back thedata bitsandcheck forthecorrection error
(single-bit error ordouble-bit error ornoerror).
5.Read theUERRADDR andSBERRADDR registers andcheck forthecorrect address capture aswell.
The following sequence should beused forinjecting faults todata bitsandtesting theECC check feature.
1.Write thedata locations ofVIM RAM with therequired patterns while keeping ECCENA active. The
ECC bitswillbeautomatically initialized along with data bits.
2.Disable ECC bysetting ECCENA=0 inECCCTRL register. Inthismode, writing todata bitsdoes not
automatically update ECC bits.
3.Inthismode, itispossible tocorrupt data bitsusing anyofthefollowing methods.
*Read thedata bits, fliponebitandwrite back
*Read thedata bits, flip2bitsandwrite back
4.Depending onthekind corruption created, read back thedata bitsandcheck forthecorrection error
(single-bit error ordouble-bit error ornoerror).
5.Read theUERRADDR andSBERRADDR registers andcheck forthecorrect address capture aswell.
NOTE: After completing thetests forECC check features, itshould beensured thatVIM Interrupt
Vector Table isinitialized with valid data andcorresponding check bits. Care should also be
taken toclear theUERR andSBERR flagregisters andtheerror address registers.
Figure 19-9. ECC Bits Mapping

<!-- Page 676 -->

INT_REQ0
WAKEENA.0
Controlled by:
WAKEENASET.0
WAKEENACLR.0WAKEUP0
INT_REQ1
WAKEENA.1
Controlled by:
WAKEENASET.1
WAKEENACLR.1WAKEUP1
INT_REQ2
WAKEENA.2
Controlled by:
WAKEENASET.2
WAKEENACLR.2WAKEUP2
INT_REQ127
WAKEENA.127
Controlled by:
WAKEENASET.127
WAKEENACLR.127WAKEUP127ORWAKE_INT
VIM Wakeup Interrupt www.ti.com
676 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.6 VIMWakeup Interrupt
The wakeup interrupts areused tocome outoflowpower mode (LPM). Any interrupt requests canbe
used towake upthedevice. After reset, allinterrupt requests aresettowake upfrom LPM. However, the
VIM canmask unwanted interrupt lines forwake-up byusing theWAKEENASET andWAKEENACLR
register. The value inREQENASET /REQENACLR does NOT impact thewakeup interrupt.
Asshown inFigure 19-10 ,theWAKEENASET andWAKEENACLR registers willenable/disable an
interrupt forwake-up from low-power mode. Allwake-up interrupts are"ORed "intoasingle signal
WAKE_INT connected totheGlobal Clock Module.
Figure 19-10. Detail oftheIRQInput

<!-- Page 677 -->

7
CAPEVT0CAPEVTSRC0[6:0]7
CAPEVT1CAPEVTSRC1[6:0]
INT_REQ0
INT_REQ1
INT_REQ126
INT_REQ127INT_REQ0
INT_REQ1
INT_REQ126
INT_REQ127To RTI To RTI
www.ti.com Capture Event Sources
677 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.7 Capture Event Sources
The VIM canselect anyofthe128interrupt request togenerate uptotwocapture events forthereal-time
interrupt (RTI) module (see Figure 19-11 ).The value inREQENASET /REQENACLR does NOT impact
thecapture event. Two registers (Section 19.9.17 )areavailable, oneforeach capture event source.
Figure 19-11. Capture Event Sources
19.8 Examples
The following sections provide examples about theoperation oftheVIM.
19.8.1 Examples -Configure CPU ToReceive Interrupts
Example 19-1 shows how tosetthevector enable (VE) bitintheCP15 R1register toenable thehardware
vector interrupt. Example 19-2 shows how toenable/disable theIRQ andFIQthrough CPSR. Asa
convention, theprogram who calls these subroutines shall preserve register R1ifneeded. Example 19-2
canONLY runinprivileged mode. However, inUSER mode, theapplication software canforce the
program intosoftware interrupt byinstruction 'SWI'.Then, inthesoftware interrupt service routine, user
canwrite register SPSR, which isthecopy ofCPSR inthisexception mode.
Example 19-1. Enable Hardware Vector Interrupt (IRQ Only)
_HW_Vec_Init
MRC p15 ,#0 ,R1 ,c1 ,c0 ,#0
ORR R1,R1 ,#0x01000000 ;Mask 0-31 bits except bit 24inSys
;Ctrl Reg ofCORTEX-R4
MCR p15 ,#0 ,R1 ,c1 ,c0 ,#0 ;Enable bit 24
MOV PC, LR

<!-- Page 678 -->

Examples www.ti.com
678 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleExample 19-2. Enable/Disable IRQ/FIQ through CPSR
FIQENABLE .equ 0x40
IRQENABLE .equ 0x80
......
_Enable_Fiq
MRS R1, CPSR
BIC R1, R1, #FIQENABLE
MSR CPSR, R1
MOV PC, LR
......
_Disable_Irq
MRS R1, CPSR
ORR R1, R1, #IRQENABLE
MSR CPSR, R1
MOV PC, LR
......
_Enable_Irq
MRS R1, CPSR
BIC R1, R1, #IRQENABLE
MSR CPSR, R1
MOV PC, LR
19.8.2 Examples -Register Vector Interrupt andIndex Interrupt Handling
Example 19-3 illustrates theconfiguration fortheexception vectors inRegister Vector Interrupt handling.
After theinterrupt isreceived bytheCPU, theCPU branches to0x18 (IRQ) or0x1C (FIQ). The instruction
placed here should beLDR PC,[PC,#-0x1B0] .The pending ISRaddress iswritten intothecorresponding
vector register (IRQVECREG forIRQ, FIQVECREG forFIQ). The CPU reads thecontent oftheregister
andbranches totheISR.
Example 19-3. Exception Vector Configuration forVIMVector
.sect ".intvecs"
00000000h b_RESET ;RESET interrupt
00000004h b_UNDEF_INST_INT ;UNDEFINED INSTRUCTION interrupt
00000008h b_SW_INT ;SOFTWARE interrupt
0000000Ch b_ABORT_PREF_INT ;ABORT (PREFETCH) interrupt
00000010h b_ABORT_DATA_INT ;ABORT (DATA) interrupt
00000014h b#-8 ;Reserved
00000018h ldr pc,[pc,#-0x1B0] ;IRQ interrupt
0000001Ch ldr pc,[pc,#-0x1B0] ;FIQ interrupt
NOTE: Program Counter (PC) always pointers twoinstructions beyond thecurrent executed
instruction. Inthiscase, PCequals to'0x18 or0x1C +0x08 '.TheLDR instruction load the
memory at'PC-0x1B0 ',which is'0x18 or0x1C +0x08 -0x1B0 =0xFFFFFE70 or
0xFFFFFE74 '.These aretheaddress ofIRQVECREG andFIQVECREG, which store the
pending ISRaddress.

<!-- Page 679 -->

www.ti.com Examples
679 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleExample 19-4 shows afastresponse totheFIQinterrupt inIndex Interrupt andcanbeapplied toasystem
thathasmore than onechannel assigned asaFIQ. Itisbuilt inIndex Interrupt compatible with
TMS470R1x legacy code.
Example 19-4. How toRespond toFIQWith Short Latency
.sect ".intvecs" ;Interrupt and exception vector sector
00000000h b_RESET ;RESET interrupt
00000004h b_UNDEF_INST_INT ;UNDEFINED INSTRUCTION interrupt
00000008h b_SW_INT ;SOFTWARE interrupt
0000000Ch b_ABORT_PREF_INT ;ABORT (PREFETCH) interrupt
00000010h b_ABORT_DATA_INT ;ABORT (DATA) interrupt
00000014h b#-8 ;Reserved
00000018h b_IRQ_ENTRY_0 ;IRQ interrupt
;*********************************
;INTERRUPT PROCESSING AREA
;*********************************
0000001Ch ldrb R8, [PC,#-0x21d] ;FIQ INTERRUPT ENTRY
;R8used toget the FIQ index
;with address pointer tothe
;first FIQ banked register
00000020h ldr PC, [PC, R8, LSL#2] ;Branch tothe indexed interrupt
;routine. The prefetch
;operation causes the PCtobe2
;words (8bytes) ahead ofthe
;current instruction, so
;pointing to_INT_TABLE.
00000024h nop ;Required due topipeline.
;=================================
00000028h _INT_TABLE ;FIQ INTERRUPT DISPATCH
;=================================
0000002Ch .word _FIQ_TABLE ;beginning ofFIQ Dispatch
00000030h .word _ISR1 ;dispatch tointerrupt routine 1
00000034h .word _ISR2 ;dispatch tointerrupt routine 2
.
.
Another way toimprove theFIQlatency istoassign only onechannel totheFIQinterrupt andtomap the
ISRcode corresponding tothischannel directly starting at0x1C.
NOTE: When theCPU isinvector-enabled mode, Example 19-3 andExample 19-4 arestillvalid.
The difference isthattheCPU willnotread from the0x18 location during IRQ interrupt, but
willjump directly tothecorresponding ISRroutine.

<!-- Page 680 -->

VIM Control Registers www.ti.com
680 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9 VIMControl Registers
Table 19-5 lists theVIM module registers. Each register begins onaword boundary. Allregisters are32-
bit,16-bit, and8-bit accessible forread andwrite. Write isonly possible inprivilege mode. The base
address ofthecontrol registers isFFFF FE00h. The base address oftheECC-related VIM registers is
FFFF FD00h. The address locations notlisted arereserved.
Table 19-5. VIMControl Registers
Offset Acronym Register Description Section
ECC-related Registers
ECh ECCSTAT Interrupt Vector Table ECC Status Register Section 19.9.1
F0h ECCCTL Interrupt Vector Table ECC Control Register Section 19.9.2
F4h UERRADDR Uncorrectable Error Address Register Section 19.9.3
F8h FBVECADDR Fallback Vector Address Register Section 19.9.4
FCh SBERRADDR Single-Bit Error Address Register Section 19.9.5
Control Registers
00h IRQINDEX IRQ Index Offset Vector Register Section 19.9.7
04h FIQINDEX FIQIndex Offset Vector Register Section 19.9.8
10h FIRQPR0 FIQ/IRQ Program Control Register 0 Section 19.9.9
14h FIRQPR1 FIQ/IRQ Program Control Register 1 Section 19.9.9
18h FIRQPR2 FIQ/IRQ Program Control Register 2 Section 19.9.9
1Ch FIRQPR3 FIQ/IRQ Program Control Register 3 Section 19.9.9
20h INTREQ0 Pending Interrupt Read Location Register 0 Section 19.9.10
24h INTREQ1 Pending Interrupt Read Location Register 1 Section 19.9.10
28h INTREQ2 Pending Interrupt Read Location Register 2 Section 19.9.10
2Ch INTREQ3 Pending Interrupt Read Location Register 3 Section 19.9.10
30h REQENASET0 Interrupt Enable SetRegister 0 Section 19.9.11
34h REQENASET1 Interrupt Enable SetRegister 1 Section 19.9.11
38h REQENASET2 Interrupt Enable SetRegister 2 Section 19.9.11
3Ch REQENASET3 Interrupt Enable SetRegister 3 Section 19.9.11
40h REQENACLR0 Interrupt Enable Clear Register 0 Section 19.9.12
44h REQENACLR1 Interrupt Enable Clear Register 1 Section 19.9.12
48h REQENACLR2 Interrupt Enable Clear Register 2 Section 19.9.12
4Ch REQENACLR3 Interrupt Enable Clear Register 3 Section 19.9.12
50h WAKEENASET0 Wake-up Enable SetRegister 0 Section 19.9.13
54h WAKEENASET1 Wake-up Enable SetRegister 1 Section 19.9.13
58h WAKEENASET2 Wake-up Enable SetRegister 2 Section 19.9.13
5Ch WAKEENASET3 Wake-up Enable SetRegister 3 Section 19.9.13
60h WAKEENACLR0 Wake-up Enable Clear Register 0 Section 19.9.14
64h WAKEENACLR1 Wake-up Enable Clear Register 1 Section 19.9.14
68h WAKEENACLR2 Wake-up Enable Clear Register 2 Section 19.9.14
6Ch WAKEENACLR3 Wake-up Enable Clear Register 3 Section 19.9.14
70h IRQVECREG IRQ Interrupt Vector Register Section 19.9.15
74h FIQVECREG FIQInterrupt Vector Register Section 19.9.16
78h CAPEVT Capture Event Register Section 19.9.17
80h-FCh CHANCTRL VIM Interrupt Control Register Section 19.9.18

<!-- Page 681 -->

www.ti.com VIM Control Registers
681 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.1 Interrupt Vector Table ECC Status Register (ECCSTAT)
Figure 19-12 andTable 19-6 describe thisregister.
Figure 19-12. Interrupt Vector Table ECC Status Register (ECCSTAT) [offset =ECh]
31 16
Reserved
R-0
15 9 8 7 1 0
Reserved SBERR Reserved UERR
R-0 R/W1CP-0 R-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 19-6. Interrupt Vector Table ECC Status Register (ECCSTAT) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 SBERR The SBERR indicates thatasingle-bit error hasbeen detected andhasbeen corrected bythe
SECDED logic andtheInterrupt Vector Table isbeing used fornormal operation (not bypassed).
0 Read: Nosingle-bit error hasoccurred.
Write: Noeffect.
1 Read: Asingle-bit error hasoccurred andwas corrected bytheSECDED logic.
Write: The SBERR bitiscleared.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 UERR The UERR indicates thatadouble-bit error hasbeen found andthattheInterrupt Vector Table is
bypassed. The resulting vector ofanyIRQ/FRQ interrupt isthen thevalue contained inthe
FBVECADDR register until thisbithasbeen cleared.
0 Read: Nodouble-bit error hasoccurred.
Write: Noeffect.
1 Read: Adouble-bit error hasoccurred andtheInterrupt Vector Table isbypassed.
Write: The UERR bitiscleared andtheinterrupt vector canberead from theInterrupt Vector Table.

<!-- Page 682 -->

VIM Control Registers www.ti.com
682 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.2 Interrupt Vector Table ECC Control Register (ECCCTL)
Figure 19-13. Interrupt Vector Table ECC Control Register (ECCCTL) [offset =F0h]
31 28 27 24 23 20 19 16
Reserved SBE_EVT_EN Reserved EDAC_MODE
R-0 R/WP-5h R-0 R/WP-Ah
15 12 11 8 7 4 3 0
Reserved TEST_DIAG_EN Reserved ECCENA
R-0 R/WP-Ah R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Table 19-7. Interrupt Vector Table ECC Control Register (ECCCTL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 SBE_EVT_EN These bitscontrol thegeneration ofError signal outbased onSingle-Bit Error (SBE)
indications from SECDED logic fortheInterrupt Vector Table.
5h Disable Error Event indication upon detection ofSBE ontheInterrupt Vector Table.
Ah Enable Error Event upon detection ofSBE theInterrupt Vector Table.
Allother values Writes areignored andthevalues arenotupdated intothisfield. The state ofthefeature
remains unchanged.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 EDAC_MODE These bitsdetermine whether Single-Bit Errors (SBE) detected bytheSECDED block will
becorrected ornot.
5h Disable correction ofSBE detected bytheSECDED block.
Ah Enable correction ofSBE detected bytheSECDED block.
Allother values Writes areignored andthevalues arenotupdated intothisfield. The state ofthefeature
remains unchanged.
Note: IfanSBE isselected tobenotcorrected (using EDAC_MODE), then anSBE
event willalso cause VIMRAM tobebypassed justlikeUERR andthemodule to
usetheFBVECADDR register asthevector address.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 TEST_DIAG_EN This bitmaps theECC bitsintotheInterrupt Vector Table frame tomake them accessible
bytheCPU. When enabled, theECC bitsarewritable aswell asreadable independent of
data bits.
5h Enable memory-mapping ofECC bitsforread/write operation.
Allother values Disable memory-mapping ofECC bitsforread/write operation.
Note: Toavoid softerror todisable VIMECC mapping, itisrecommended towrite
Ahtodisable ECC bitsmapping.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 ECCENA VIM ECC enable.
5h VIM ECC isdisabled.
Allother values VIM ECC isenabled.
Note: Toavoid softerror todisable VIMECC checking, itisrecommended towrite
Ahtoenable ECC checking.

<!-- Page 683 -->

www.ti.com VIM Control Registers
683 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.3 Uncorrectable Error Address Register (UERRADDR)
The uncorrectable error address register gives theaddress ofthefirstuncorrectable error location.
NOTE: Nocomputation isneeded when reading thecomplete register toretrieve theaddress inthe
Interrupt Vector Table.
This register willnever bereset byapower-on reset noranyother reset source.
Figure 19-14. Uncorrectable Error Address Register (UERRADDR) [offset =F4h]
31 16
Interrupt Vector Table offset
R-FFF8h
15 10 9 2 1 0
Interrupt Vector Table offset ADDERR Word offset
R-0010 000b R-x R-0
LEGEND: R=Read only; x=value isindeterminate; -n=value after reset
Table 19-8. Uncorrectable Error Address Register (UERRADDR) Field Descriptions
Bit Field Description
31-10 Interrupt Vector Table
offsetInterrupt Vector Table offset. Reads arealways FFF8 2xxxh; writes have noeffect.
9-2 ADDERR Uncorrectable error address register. This register gives theaddress ofthefirstencountered double-bit
error since theflaghasbeen clear. Subsequent ECC errors willnotupdate thisregister until theUERR
flaghasbeen cleared.
Note: This register isvalid only when PARFLG isset(see Section 19.9.1 ).
1-0 Word offset Word offset. Reads arealways 0;writes have noeffect.
19.9.4 Fallback Vector Address Register (FBVECADDR)
This register provides afall-back address totheVIM ifauncorrectable error hasoccurred intheInterrupt
Vector Table. Figure 19-15 andTable 19-9 describe thisregister.
NOTE: This register willnever bereset byapower-on reset noranyother reset source.
Figure 19-15. Fallback Vector Address Register (FBVECADDR) [offset =F8h]
31 0
FBVECADDR
R/WP-x
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; x=value isindeterminate; -n=value after reset
Table 19-9. Fallback Vector Address Register (FBVECADDR) Field Descriptions
Bit Field Description
31-0 FBVECADDR Fallback Vector Address Register. This register isused bytheVIM iftheInterrupt Vector Table hasbeen
corrupted. The contents oftheIRQVECREG andFIQVECREG registers willreflect thevalue programmed
inFBVECADDR. The value provided totheVICport willalso reflect FBVECADDR until theUERR register
hasbeen cleared.
This register provides theaddress oftheISRthatwillrestore theintegrity oftheInterrupt Vector Table.

<!-- Page 684 -->

VIM Control Registers www.ti.com
684 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.5 Single-Bit Error Address Register (SBERRADDR)
This register gives theaddress ofthefirstsingle-bit ECC error detected bytheECC logic. Figure 19-16
andTable 19-10 describe thisregister.
NOTE: This register willnever bereset byapower-on reset noranyother reset source.
Figure 19-16. Single-Bit Error Address Register (SBERRADDR) [offset =FCh]
31 0
SBERRADDR
R/WP-x
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; x=value isindeterminate; -n=value after reset
Table 19-10. Single-Bit Error Address Register (SBERRADDR) Field Descriptions
Bit Field Description
31-0 SBERRADDR Single-Bit Error Address Register. This register gives theaddress ofthefirstsingle-bit error detected bythe
SECDED logic since theSBERR flaghasbeen clear. Subsequent single-bit ECC errors willnotupdate this
register until theSBERR flaghasbeen cleared.
This register provides theInterrupt Vector Table address (offset from base address word aligned) ofthe
ECC error location. This register isvalid only when theSBERR flagisset.
19.9.6 VIMOffset Vector Registers
The VIM offset register provides theuser with thenumerical index value thatrepresents thepending
interrupt with thehighest precedence. The register IRQINDEX holds theindex tothehighest priority IRQ
interrupt; theregister FIQINDEX holds theindex tothehighest priority FIQinterrupt. The index canbe
used tolocate theinterrupt routine inadispatch table, asshown inTable 19-11 .
Table 19-11. Interrupt Dispatch
IRQINDEX /FIQINDEX Register BitField Highest Priority Pending Interrupt Enabled
0x00 Nointerrupt
0x01 Channel 0
: :
0x7F Channel 126
0x80 Channel 127
NOTE: Channel 127hasnodedicated interrupt vector table entry. Therefore, Channel 127shall
NOT beused inapplication.
The VIM offset registers areread only. They areupdated continuously bytheVIM. When aninterrupt is
serviced, theoffset vectors show theindex forthenext highest pending interrupt or0x0ifnointerrupt is
pending.

<!-- Page 685 -->

www.ti.com VIM Control Registers
685 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.7 IRQIndex Offset Vector Register (IRQINDEX)
The IRQ offset register provides theuser with thenumerical index value thatrepresents thepending IRQ
interrupt with thehighest priority. Figure 19-17 andTable 19-12 describe thisregister.
Figure 19-17. IRQIndex Offset Vector Register (IRQINDEX) [offset =00h]
31 16
Reserved
R-0
15 8 7 0
Reserved IRQINDEX
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 19-12. IRQIndex Offset Vector Register (IRQINDEX) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 IRQINDEX 0-FFh IRQ index vector. The least-significant bitsrepresent theindex oftheIRQ pending interrupt with
thehighest precedence, asshown inTable 19-11 .When nointerrupts arepending, theleast-
significant byte ofIRQINDEX is0.
19.9.8 FIQIndex Offset Vector Registers (FIQINDEX)
The FIQINDEX register provides theuser with anumerical index value thatrepresents thepending FIQ
interrupt with thehighest priority. Figure 19-18 andTable 19-13 describe thisregister.
Figure 19-18. FIQIndex Offset Vector Register (FIQINDEX) [offset =F04h]
31 16
Reserved
R-0
15 8 7 0
Reserved FIQINDEX
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 19-13. FIQIndex Offset Vector Register (FIQINDEX) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 FIQINDEX 0-FFh FIQindex offset vector. The least-significant bitsrepresent theindex oftheFIQpending
interrupt with thehighest precedence, asshown inTable 19-11 .When nointerrupts are
pending, theleast-significant byte ofFIQINDEX is0x00.

<!-- Page 686 -->

VIM Control Registers www.ti.com
686 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.9 FIQ/IRQ Program Control Registers (FIRQPR[0:3])
The FIQ/IRQ program control registers determine whether agiven interrupt request willbeeither FIQor
IRQ. Figure 19-19 ,Figure 19-20 ,Figure 19-21 ,Figure 19-22 andTable 19-14 describe these registers.
NOTE: Channel 0and1areFIQonly, notimpacted bythisregister.
Figure 19-19. FIQ/IRQ Program Control Register 0(FIRQPR0) [offset =10h]
31 16
FIRQPR0[31:16]
R/WP-0
15 2 1 0
FIRQPR0[15:2] Reserved
R/WP-0 R-3h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Figure 19-20. FIQ/IRQ Program Control Register 1(FIRQPR1) [offset =F14h]
31 0
FIRQPR1[63:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-21. FIQ/IRQ Program Control Register 2(FIRQPR2) [offset =18h]
31 0
FIRQPR2[95:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-22. FIQ/IRQ Program Control Register 3(FIRQPR3) [offset =1Ch]
31 0
FIRQPR3[127:96]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 19-14. FIQ/IRQ Program Control Registers (FIRQPR) Field Descriptions
Bit Field Value Description
127-2 FIRQPRx[ n] FIQ/IRQ program control bits. These bitsdetermine whether aninterrupt request from aperipheral
isoftype FIQorIRQ. BitFIRQPRx[127:2] corresponds torequest channel[127:2].
0 Interrupt request isofIRQ type.
1 Interrupt request isofFIQtype.
1-0 Reserved 3h Read only. Writes have noeffect.

<!-- Page 687 -->

www.ti.com VIM Control Registers
687 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.10 Pending Interrupt Read Location Registers (INTREQ[0:3])
The pending interrupt register gives thepending interrupt requests. The register isupdated every vbus
clock cycle. Figure 19-23 ,Figure 19-24 ,Figure 19-25 ,Figure 19-26 andTable 19-15 describe this
register.
Figure 19-23. Pending Interrupt Read Location Register 0(INTREQ0) [offset =20h]
31 0
INTREQ0[31:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Figure 19-24. Pending Interrupt Read Location Register 1(INTREQ1) [offset =24h]
31 0
INTREQ1[63:32]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Figure 19-25. Pending Interrupt Read Location Register 2(INTREQ2) [offset =28h]
31 0
INTREQ2[95:64]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Figure 19-26. Pending Interrupt Read Location Register 3(INTREQ3) [offset =2Ch]
31 0
INTREQ3[127:96]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 19-15. Pending Interrupt Read Location Registers (INTREQ) Field Descriptions
Bit Field Value Description
127-0 INTREQx[ n] Pending interrupt bits. These bitsdetermine whether aninterrupt request ispending fortherequest
channel between 0and127. The interrupt ENABLE register does notaffect thevalue ofthe
interrupt pending bit.BitINTREQx[127:0] corresponds torequest channel[127:0].
User andPrivilege Mode read:
0 Nointerrupt event hasoccurred.
1 Aninterrupt ispending.
Privilege Mode write only:
0 Writing 0hasnoeffect.
1 Clears theinterrupt pending status flag. This write-clear functionality isintended toallow clearing
those interrupts which have been signaled toVIM before enabling theinterrupt channel, ifthey are
undesired.

<!-- Page 688 -->

VIM Control Registers www.ti.com
688 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.11 Interrupt Enable SetRegisters (REQENASET[0:3])
The interrupt register enable selectively enables individual request channels. Figure 19-27 ,Figure 19-28 ,
Figure 19-29 ,Figure 19-30 andTable 19-16 describe these registers.
NOTE: Channel 0and1arealways enabled, notimpacted bythisregister.
Figure 19-27. Interrupt Enable SetRegister 0(REQENASET0) [offset =30h]
31 16
REQENASET0[31:16]
R/WP-0
15 2 1 0
REQENASET0[15:2] Reserved
R/WP-0 R-3h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Figure 19-28. Interrupt Enable SetRegister 1(REQENASET1) [offset =34h]
31 0
REQENASET1[63:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-29. Interrupt Enable SetRegister 2(REQENASET2) [offset =38h]
31 0
REQENASET2[95:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-30. Interrupt Enable SetRegister 3(REQENASET3) [offset =3Ch]
31 0
REQENASET3[127:96]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 19-16. Interrupt Enable SetRegisters (REQENASET) Field Descriptions
Bit Field Value Description
127-2 REQENASETx[ n] Request enable setbits. This vector determines whether theinterrupt request channel is
enabled. BitREQENASETx[127:2] corresponds torequest channel[127:2].
0 Read: Interrupt request channel isdisabled.
Write: Noeffect.
1 Read orWrite: The interrupt request channel isenabled.
1-0 Reserved 3h Read only. Writes have noeffect.

<!-- Page 689 -->

www.ti.com VIM Control Registers
689 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.12 Interrupt Enable Clear Registers (REQENACLR[0:3])
The interrupt register enable selectively disables individual request channels. Figure 19-31 ,Figure 19-32 ,
Figure 19-33 ,Figure 19-34 andTable 19-17 describe these registers.
NOTE: Channel 0and1arealways enabled, notimpacted bythisregister.
Figure 19-31. Interrupt Enable Clear Register 0(REQENACLR0) [offset =40h]
31 16
REQENACLR0[31:16]
R/WP-0
15 2 1 0
REQENACLR0[15:2] Reserved
R/WP-0 R-3h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivilege mode only; -n=value after reset
Figure 19-32. Interrupt Enable Clear Register 1(REQENACLR1) [offset =44h]
31 0
REQENACLR1[63:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-33. Interrupt Enable Clear Register 2(REQENACLR2) [offset =48h]
31 0
REQENACLR2[95:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-34. Interrupt Enable Clear Register 3(REQENACLR3) [offset =4Ch]
31 0
REQENACLR3[127:96]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 19-17. Interrupt Enable Clear Registers (REQENACLR) Field Descriptions
Bit Field Value Description
127-2 REQENACLRx[ n] Request enable clear bits. This vector determines whether theinterrupt request channel is
enabled. BitREQENACLRx[127:2] corresponds torequest channel[127:2].
0 Read: Interrupt request channel isdisabled.
Write: Noeffect.
1 Read: The interrupt request channel isenabled.
Write: The interrupt request channel isdisabled.
1-0 Reserved 3h Read only. Writes have noeffect.

<!-- Page 690 -->

VIM Control Registers www.ti.com
690 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.13 Wake-Up Enable SetRegisters (WAKEENASET[0:3])
The wake-up enable registers selectively enables individual wake-up interrupt request lines. Figure 19-35 ,
Figure 19-36 ,Figure 19-37 ,Figure 19-38 andTable 19-18 describe these registers.
Figure 19-35. Wake-Up Enable SetRegister 0(WAKEENASET0) [offset =50h]
31 0
WAKEENASET0[31:0]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-36. Wake-Up Enable SetRegister 1(WAKEENASET1) [offset =54h]
31 0
WAKEENASET1[63:32]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-37. Wake-Up Enable SetRegister 2(WAKEENASET2) [offset =58h]
31 0
WAKEENASET2[95:64]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-38. Wake-Up Enable SetRegister 3(WAKEENASET3) [offset =5Ch]
31 0
WAKEENASET3[127:96]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 19-18. Wake-Up Enable SetRegisters (WAKEENASET) Field Descriptions
Bit Field Value Description
127-0 WAKEENASETx[ n] Wake-up enable setbits. This vector determines whether thewake-up interrupt lineisenabled.
BitWAKEENASETx[127:0] corresponds tointerrupt request channel[127:0].
0 Read: Interrupt request channel isdisabled.
Write: Noeffect.
1 Read orWrite: The interrupt request channel isenabled.

<!-- Page 691 -->

www.ti.com VIM Control Registers
691 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.14 Wake-Up Enable Clear Registers (WAKEENACLR[0:3])
The wake-up enable register selectively disables individual wake-up interrupt request lines. Figure 19-39 ,
Figure 19-40 ,Figure 19-41 ,Figure 19-42 andTable 19-19 describe these registers.
Figure 19-39. Wake-Up Enable Clear Register 0(WAKEENACLR0) [offset =60h]
31 0
WAKEENACLR0[31:0]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-40. Wake-Up Enable Clear Register 1(WAKEENACLR1) [offset =64h]
31 0
WAKEENACLR1[63:32]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-41. Wake-Up Enable Clear Register 2(WAKEENACLR2) [offset =68h]
31 0
WAKEENACLR2[95:64]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Figure 19-42. Wake-Up Enable Clear Register 3(WAKEENACLR3) [offset =6Ch]
31 0
WAKEENACLR3[127:96]
R/WP-FFFF FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivilege mode only; -n=value after reset
Table 19-19. Wake-Up Enable Clear Registers (WAKEENACLR) Field Descriptions
Bit Field Value Description
127-0 WAKEENACLRx[ n] Wake-up enable clear bits. This vector determines whether thewake-up interrupt lineis
enabled. BitWAKEENACLRx[127:0] corresponds tointerrupt request channel[127:0].
0 Read: Wake-up interrupt channel isdisabled.
Write: Noeffect.
1 Read: The wake-up interrupt channel isenabled.
Write: The wake-up interrupt channel isdisabled.

<!-- Page 692 -->

VIM Control Registers www.ti.com
692 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.15 IRQInterrupt Vector Register (IRQVECREG)
The interrupt vector register gives theaddress oftheenabled andactive IRQ interrupt. Figure 19-43 and
Table 19-20 describe these registers.
Figure 19-43. IRQInterrupt Vector Register (IRQVECREG) [offset =70h]
31 0
IRQVECREG
R-0
LEGEND: R=Read only; -n=value after reset
Table 19-20. IRQInterrupt Vector Register (IRQVECREG) Field Descriptions
Bit Field Value Description
31-0 IRQVECREG From
Section 19.5IRQ interrupt vector register. This vector gives theaddress oftheISRwith thehighest
pending IRQ request. The CPU reads theaddress andbranches tothislocation.
19.9.16 FIQInterrupt Vector Register (FIQVECREG)
The interrupt vector register gives theaddress oftheenabled andactive FIQinterrupt. Figure 19-44 and
Table 19-21 describe these registers.
Figure 19-44. IRQInterrupt Vector Register (FIQVECREG )[offset =74h]
31 0
FIQVECREG
R-0
LEGEND: R=Read only; -n=value after reset
Table 19-21. FIQInterrupt Vector Register (FIQVECREG) Field Descriptions
Bit Field Value Description
31-0 FIQVECREG From
Section 19.5FIQinterrupt vector register. This vector gives theaddress oftheISRwith thehighest
pending FIQrequest. The CPU reads theaddress andbranches tothislocation.

<!-- Page 693 -->

www.ti.com VIM Control Registers
693 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.17 Capture Event Register (CAPEVT)
Figure 19-45 andTable 19-22 describe thisregister.
Figure 19-45. Capture Event Register (CAPEVT) [offset =78h]
31 23 22 16
Reserved CAPEVTSRC1
R-U R/WP-0
15 7 6 0
Reserved CAPEVTSRC0
R-U R/WP-0
LEGEND: R=Read only; WP=Write inprivilege mode only; U=value isundefined; -n=value after reset
Table 19-22. Capture Event Register (CAPEVT) Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads areindeterminate andwrites have noeffect.
22-16 CAPEVTSRC1 Capture event source 1mapping control. These bitsdetermine which interrupt request maps tothe
capture event source 1oftheRTI:
0 Interrupt request 0.
1h Interrupt request 1.
: :
7Fh Interrupt request 127.
15-7 Reserved 0 Reads areindeterminate andwrites have noeffect.
6-0 CAPEVTSRC0 Capture event source 0mapping control. These bitsdetermine which interrupt request maps tothe
capture event source 0oftheRTI:
0 Interrupt request 0.
1h Interrupt request 1.
: :
7Fh Interrupt request 127.

<!-- Page 694 -->

VIM Control Registers www.ti.com
694 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) Module19.9.18 VIMInterrupt Control Registers (CHANCTRL[0:31])
Thirty-two interrupt control registers control the128interrupt channels oftheVIM. Each register controls
four interrupt channels: each ofthem isindexed from 0to127. Table 19-23 shows theorganization ofall
theregisters andthereset value ofeach. Each four fields oftheregister hasbeen named with ageneric
index thatrefers tothedetailed register organization. Figure 19-46 andTable 19-24 describe these
registers.
Table 19-23. Interrupt Control Registers Organization
AddressRegister
AcronymRegister Field
31:24
CHANMAPx0Register Field
23:16
CHANMAPx1Register Field
15:8
CHANMAPx2Register Field
7:0
CHANMAPx3 Reset Value
FFFF FE80h CHANCTRL0 CHANMAP0 CHANMAP1 CHANMAP2 CHANMAP3 0001 0203h
FFFF FE84h CHANCTRL1 CHANMAP4 CHANMAP5 CHANMAP6 CHANMAP7 0405 0607h
: : : : : : :
FFFF FEF8h CHANCTRL30 CHANMAP120 CHANMAP121 CHANMAP122 CHANMAP123 7879 7A7Bh
FFFF FEFCh CHANCTRL31 CHANMAP124 CHANMAP125 CHANMAP126 CHANMAP127 7C7D 7E7Fh
NOTE: CHANMAP0 andCHANMAP1 arenotprogramable. CHAN0 andCHAN1 arehard wired to
INT_REQ0 andINT_REQ1.
DoNOT write anyvalue other than 0x7F toCHANMAP127. Channel 127isreserved
because nointerrupt vector table entry supports thischannel.
Figure 19-46. Interrupt Control Registers (CHANCTRL[0:31])
[offset =80h-FCh]
31 30 24 23 22 16
Rsvd CHANMAPx0 Rsvd CHANMAPx1
R-U R/WP-n R-U R/WP-n
15 14 8 7 6 0
Rsvd CHANMAPx2 Rsvd CHANMAPx3
R-U R/WP-n R-U R/WP-n
LEGEND: R=Read only; WP=Write inprivilege mode only; U=value isundefined; -n=value after reset (see Table 19-23 )
Table 19-24. Interrupt Control Registers (CHANCTRL[0:31]) Field Descriptions
Bit Field Value Description
31 Reserved 0 Reads areindeterminate andwrites have noeffect.
30-24 CHANMAPx0 CHANMAPx0(6-0). Interrupt CHANx0mapping control. These bitsdetermine which interrupt request
thepriority channel CHANx0maps to:
0 Read: Interrupt request 0maps tochannel priority CHANx0.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx0
issetwith theinterrupt request.
1h Read: Interrupt request 1maps tochannel priority CHANx0.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx0
issetwith theinterrupt request.
: :
7Fh Read: Interrupt request 127maps tochannel priority CHANx0.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx0
issetwith theinterrupt request.
23 Reserved 0 Reads areindeterminate andwrites have noeffect.

<!-- Page 695 -->

www.ti.com VIM Control Registers
695 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedVectored Interrupt Manager (VIM) ModuleTable 19-24. Interrupt Control Registers (CHANCTRL[0:31]) Field Descriptions (continued)
Bit Field Value Description
22-16 CHANMAPx1 CHANMAPx1(6-0). Interrupt CHANx1mapping control. These bitsdetermine which interrupt request
thepriority channel CHANx1maps to:
0 Read: Interrupt request 0maps tochannel priority CHANx1.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx1
issetwith theinterrupt request.
1h Read: Interrupt request 1maps tochannel priority CHANx1.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx1
issetwith theinterrupt request.
: :
7Fh Read: Interrupt request 127maps tochannel priority CHANx1.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx1
issetwith theinterrupt request.
15 Reserved 0 Reads areindeterminate andwrites have noeffect.
14-8 CHANMAPx2 CHANMAPx2(6-0). Interrupt CHANx2mapping control. These bitsdetermine which interrupt request
thepriority channel CHANx2maps to:
0 Read: Interrupt request 0maps tochannel priority CHANx2.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx2
issetwith theinterrupt request.
1h Read: Interrupt request 1maps tochannel priority CHANx2.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx2
issetwith theinterrupt request.
: :
7Fh Read: Interrupt request 127maps tochannel priority CHANx2.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx2
issetwith theinterrupt request.
7 Reserved 0 Reads areindeterminate andwrites have noeffect.
6-0 CHANMAPx3 CHANMAPx3(6-0). Interrupt CHANx3mapping control. These bitsdetermine which interrupt request
thepriority channel CHANx3maps to:
0 Read: Interrupt request 0maps tochannel priority CHANx3.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx3
issetwith theinterrupt request.
1h Read: Interrupt request 1maps tochannel priority CHANx3.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx3
issetwith theinterrupt request.
: :
7Fh Read: Interrupt request 127maps tochannel priority CHANx3.
Write: The default value ofthisbitafter reset isgiven inTable 19-23 .The channel priority CHANx3
issetwith theinterrupt request.