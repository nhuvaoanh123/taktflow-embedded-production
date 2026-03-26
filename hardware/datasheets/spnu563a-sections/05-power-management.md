# Power Management Module (PMM)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 279-300

---


<!-- Page 279 -->

279 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)Chapter 5
SPNU563A -March 2018
Power Management Module (PMM)
This chapter describes thepower management module (PMM).
Topic ........................................................................................................................... Page
5.1 Overview ......................................................................................................... 280
5.2 Power Domains ................................................................................................ 282
5.3 PMM Operation ................................................................................................. 283
5.4 PMM Registers ................................................................................................. 285

<!-- Page 280 -->

Overview www.ti.com
280 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.1 Overview
The microcontroller ispart ofthefamily ofmicrocontrollers from Texas Instruments forsafety-critical
applications. Several functions areimplemented onthismicrocontroller targeted towards varied
applications. The core logic isdivided intoseveral domains thatcanbeindependently turned onoroff
based ontheapplication 'srequirements. Turning offadomain hastheeffect toonly turn offtheclocks into
thedomain. Dynamic current isvirtually reduced tozero. Leakage willremain thesame asinthisdevice
nophysical power switches areimplemented toisolate adomain from itscore supply.
This chapter describes thePower Management Module (PMM). The PMM provides memory-mapped
registers thatcontrol thestates ofthesupported power domains. The PMM includes interfaces tothe
Power Mode Controller (PMC) andthePower State Controller (PSCON). The PMC andPSCON control
thepower up/down sequence ofeach power domain.
5.1.1 Features
The main features ofthePMM implemented onthemicrocontroller are:
*Supports 6logic power domains: PD1, PD2, PD3, PD4, PD5 andPD6
*Allows configurable default states foreach power domain
*Allows each power domain tobepermanently disabled
*Manages theclocks foreach power domain
*Manages theresets toeach power domain
*Includes failsafe compare logic tocontinuously monitor thestates ofeach power domain
*Supports diagnostic andself-test logic tovalidate failsafe compare logic
5.1.2 Block Diagram
PMM consists ofseveral keycomponents:
*Register interface -thePMM control registers aremapped tothedevice memory space andstart at
address 0xFFFF0000.
*System Interface -thePMM receives theclocks, resets, errors andallother control signals through
thisinterface.
*PSCON Diagnostic Compare -thisblock compares theoutputs ofeach primary PSCON andthe
respective diagnostic PSCON implemented forfailsafe safety.
*Self-Test Diagnostic -thisblock contains thelogic toplace thePSCON diagnostic compare block ina
self-test mode inorder totestthefailsafe feature.
*Clock management -thePMM provides independent clock gating andhandshaking controls foreach
power domain andalso generates theclock domains foreach power domain.
*Reset Management -thePMM provides independent reset signals foreach power domain.
*Power Mode Controller (PMC) -The PMC isafinite state machine thatcontrols thepower sequence
from onepower mode toanother. Apower mode isthestates ofallpower domains atagiven time.
*Power State Controller (PSCON) -The PSCON isafinite state machine thatcontrols thepower
sequence ofapower domain from onestate toanother. Each power domain iscontrolled byone
dedicated PSCON.
*Power Domain -Inthisdevice, apower domain isagroup oflogic and/or memories which shares the
common control inputs.

<!-- Page 281 -->

www.ti.com Overview
281 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)Figure 5-1.PMM Block Diagram

<!-- Page 282 -->

PD1 (always ON)
All modules for essential operation of
microcontroller (Cortex-R5F CPUs, Level 1
cache memory, Level 2 Flash memory, Level 2
SRAM, Interconnect, Clock control, Basic
peripheral set)PD2
ETM-R5, TPIU,
CTI, CTM, ATB,
RTP, DMMPD3
MIBADC2, MIBSPI4,
MIBSPI5, DCAN3,
DCAN4, NHET2,
HTU2, SCI3, SCI4,
I2C1, I2C2
PD4
FlexRay, FTUPD5
Ethernet, EMIFPD6
ePWM[1..7],
eCAP[1..6],
eQEP[1..2]
Switchable domains
Power Domains www.ti.com
282 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.2 Power Domains
Figure 5-2shows thecore domains implemented onthemicrocontroller.
This device has6separate core power domains:
*PD1 isanalways-ON domain andisnotcontrolled byPMM. Itcontains theCPU aswell asother
principal modules andtheinterconnect required foroperation ofthemicrocontroller. This domain also
includes thelevel 1cache memory andthelevel 2flash memory andSRAM. The PD1 canoperate on
itsown even when alltheother core power domains areturned offbythePMM. Note thatallI/Os are
inthisalways-ON domain aswell.
Core power domains PD2 through PD6 arecontrolled bythePMM.
*PD2 contains logic related todebug, instrumentation andtrace such astheEmbedded Trace Macrocell
(ETM-R5), RAM Trace Port (RTP), andData Modification Module (DMM) components.
*PD3 contains some additional peripheral modules asanenhanced configuration over andabove the
peripheral setavailable inPD1. These include asecond High-End Timer (N2HET2) with itsdedicated
transfer unit(HTU2), asecond Analog-to-Digital Converter (ADC2), twoSerial Communication
Interfaces (SCI3 andSCI4), twoInter-Integrated Circuit controllers (I2C1 andI2C2), twoController
Area Network controller (DCAN3 andDCAN4), andtwoMulti-buffer Serial Peripheral Interface module
(MibSPI4 andMibSPI5).
*PD4 contains theFlexRay controller anditsdedicated transfer unit(FTU).
*PD5 contains theEthernet controller (EMAC), theExternal Memory Interface (EMIF), aswell assome
components oftheinterconnect fabric required bythese modules.
*PD6 contains seven Enhanced Pulse Width Modulation modules (ePWM), twoQuadrature Encoder
Pulse modules (nQEP), andsixEnhanced Capture modules (eCAP).
Figure 5-2.Core Power Domains

<!-- Page 283 -->

www.ti.com PMM Operation
283 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.3 PMM Operation
Itisimportant tounderstand some fundamental concepts beforehand.
5.3.1 Power Domain State
Each core power domain canbeinoneofthree states: Active, Idle, orOff.
IntheActive state, apower domain isfully powered with normal supply voltage.
IntheIdle state, allclocks toapower domain areturned off(driven low). The supply voltage isstill
maintained atthenormal level.
Inthisdevice, theOffstate isequivalent totheIdle state interms ofpower saving. Users canstillfrom a
programmer's model perspective putapower domain intotheOffstate asifthepower domain canbe
physically turned off.
NOTE: This device does notimplement power switches tophysically isolate thepower domain from
itspower supply. Putting apower domain intotheOffstate hasnoeffect toremove leakage
power. Power domains inthisdevice aregroup ofmodules surrounded bytheisolation cells.
Isolation cells areplaced attheoutputs ofthepower domains. When apower domain isput
intoOffstate, theisolation cells areenabled andforce inactive states ontheoutput signals.
PMM andthePSCONs donotknow thephysical implementation ofthepower domains. The
logic tocontrol thetransition from onepower state toanother willbehave thesame asifthe
power domains canbephysically turned off.
5.3.2 Default Power Domain State
The default state ofeach power domain, except forPD1, iscontrolled byTIduring production testing via
programmation ofindividual bitswithin thereset configuration word intheTI-OTP sector offlash bank 0.
This allows each power domain todefault toeither theactive state ortheoffstate.
5.3.3 Disabling aPower Domain Permanently
TIcanalso permanently disable anypower domain, except forPD1. This isalso controlled by
programmation ofindividual bitswithin thereset configuration word intheTI-OTP sector offlash bank 0.
5.3.4 Changing Power Domain State
Adomain canonly change state when commanded bytheapplication. Each domain hasanassociated 4-
bitkeytodefine theintended power state. When thecorrect keyisprogrammed, thePMM initiates the
sequence totransition thatdomain tothecommanded state.
5.3.4.1 Turning aPower Domain Off
Itisnecessary toturn offallclocks going toapower domain before thatdomain canbepowered down.
PMM contains thehardware interlocks tohandle this. Each power domain hasanassociated memory-
mapped register which allows theapplication toturn offclocks tothatpower domain.
Steps topower down adomain with logic -PD2, PD3, PD4, PD5, PD6:
1.Write tothePDCLK_DISx register todisable allclocks tothepower domain.
2.Write 0xA totheLOGICPDPWRCTRL0 register topower down thedomain.
3.PollforLOGICPDPWRSTATx tobecome "00".The power domain isnow powered down.

<!-- Page 284 -->

PMM Operation www.ti.com
284 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.3.4.2 Turning aPower Domain On
Apower domain canbeturned onbywriting thecorrect keytotheLOGICPDON register. PMM will
automatically restart theclocks tothepower domain once theActive power state isrestored ifthe
"automatic clock enable upon wake up"option isselected. Ifthisoption isnotselected, theapplication can
turn onclocks tothepower domain byclearing thePDCLK_DIS register manually. The application must
polltheDOMAINISON register toensure thatthepower hasbeen fully restored before enabling the
clocks.
5.3.5 Reset Management
PMM handles thereset sequence foreach power domain. When apower domain isturned onfrom anoff
state, thePMM willreset thepower domain toensure thatalllogic begins initsdefault reset state.
PMM generates nPORRST (power-on reset), nRST (system reset), nPRST (peripheral reset), andnTRST
(test /debug logic reset) foreach domain.
5.3.6 Diagnostic Power State Controller (PSCON)
Each power domain state iscontrolled byaprimary PSCON. There isasecond PSCON aswell foreach
power domain. This isthediagnostic PSCON. Allpower management inputs toapower domain are
controlled only bytheprimary PSCON. Allpower management outputs from thepower domain arefed
back toboth theprimary andthediagnostic PSCON.
The PMM commands both thePSCON identically sothatthey arealways inalock-step operating mode. A
dedicated compare unitchecks theoutputs ofthetwoPSCON modules onevery cycle.
5.3.7 PSCON Compare Block
The diagnostic compare block canoperate inoneoffour modes.
5.3.7.1 Lock-Step Mode
This isthedefault mode ofoperation ofthePSCON compare block. The PSCON diagnostic compare
block compares theoutputs from thetwoPSCONs onevery cycle. Any mismatch inthePSCON outputs is
indicated asaPSCON compare error. This error signal ismapped totheError Signaling Module 's(ESM)
Group1 channel 38.The application candefine theresponse tothiserror.
5.3.7.2 Self-Test Mode
Aself-test mechanism isprovided tocheck thePSCON compare logic forfaults. The compare error signal
output isdisabled inself-test mode. The PSCON diagnostic compare block generates twotypes of
patterns during self-test mode: compare match testfollowed bycompare mismatch test. During theself-
test, each testpattern isapplied onboth PSCON signal ports ofthePSCON diagnostic compare block
andthen isclocked foronecycle. The duration oftheself-test is24cycles. Any detected fault isindicated
asaself-test error, mapped toESM group1 channel 39.Ifnofault isdetected, theself-test complete flag
isset.
The application canpollforthisflagtobesetandthen switch themode ofthePSCON compare block
back tolock-step mode bywriting tothemode keyregister.
NOTE: PSCON operation when compare block isinself-test mode
When thePSCON compare block isinitsself-test mode, both PSCONs continue tofunction
normally. However, there isnocomparison done onthePSCON outputs.
Compare match test:
Anidentical vector isapplied toboth input ports atthesame time, thereby expecting acompare match. If
thecompare unitproduces amismatch then theself-test error flagissetandtheself-test error signal is
generated. The compare match testisterminated ifacompare mismatch isdetected. The compare match
testtakes 4cycles tocomplete when thetestpasses.

<!-- Page 285 -->

www.ti.com PMM Registers
285 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)Compare mismatch test:
Avector with all1'sisapplied tothePSCON diagnostic compare block 'sprimary input port andthesame
input isalso applied tothesecondary input port butwith onebitflipped starting from bitposition 0.The
unequal vectors should cause thePSCON diagnostic compare block togenerate acompare mismatch at
bitposition 0.Incase amismatch isnotdetected, aself-test error isindicated. This compare mismatch
testalgorithm isrepeated until every single bitposition isverified onboth PSCON signal ports.
5.3.7.3 Error-Forcing Mode
This mode isdesigned specifically toensure thattheerror signal output from thePSCON compare block
isnotstuck inactive. Inthismode, atestpattern isapplied tothePSCON related inputs ofthecompare
logic toforce anerror. The application canclear theflagforESM group1 channel 38once theerror is
flagged. IftheESM group1 channel 38flagdoes notgetset,thisindicates thatthePSCON compare error
signal isstuck inactive andcannot berelied upon todetect aPSCON mismatch.
5.3.7.4 Self-Test Error-Forcing Mode
Inthismode, anerror isforced sothattheself-test error output from thePSCON compare block is
activated. The application canclear theflagforESM group1 channel 39once theerror isflagged. Ifthe
ESM group1 channel 39flagdoes notgetset,thisindicates thatthePSCON compare block self-test error
signal isstuck inactive andthere isnoself-test mechanism available forthePSCON compare block.
5.3.7.5 PMM Operation During CPU Halt Debug Mode
Nocompare errors aregenerated when theCPU ishalted indebug mode, regardless ofthemode ofthe
diagnostic compare block. Nostatus flags areupdated inthismode. Normal operation ofthecompare
block isresumed once theCPU exits thedebug mode.
5.4 PMM Registers
Table 5-1lists thecontrol registers inthePMM module. The registers support 8-,16-, and32-bit
accesses. The address offset isspecified from thebase address ofFFFF 0000h. Any access toan
unimplemented location within thePMM register frame willgenerate abuserror thatresults inanAbort
exception.
Table 5-1.PMM Registers
Offset Acronym Register Description Section
00h LOGICPDPWRCTRL0 Logic Power Domain Control Register 0 Section 5.4.1
04h LOGICPDPWRCTRL1 Logic Power Domain Control Register 1 Section 5.4.2
20h PDCLKDIS Power Domain Clock Disable Register Section 5.4.3
24h PDCLKDISSET Power Domain Clock Disable SetRegister Section 5.4.4
28h PDCLKDISCLR Power Domain Clock Disable Clear Register Section 5.4.5
40h LOGICPDPWRSTAT0 Logic Power Domain PD2 Power Status Register Section 5.4.6
44h LOGICPDPWRSTAT1 Logic Power Domain PD3 Power Status Register Section 5.4.7
48h LOGICPDPWRSTAT2 Logic Power Domain PD4 Power Status Register Section 5.4.8
4Ch LOGICPDPWRSTAT3 Logic Power Domain PD5 Power Status Register Section 5.4.9
50h LOGICPDPWRSTAT4 Logic Power Domain PD6 Power Status Register Section 5.4.10
A0h GLOBALCTRL1 Global Control Register 1 Section 5.4.11
A8h GLOBALSTAT Global Status Register Section 5.4.12
ACh PRCKEYREG PSCON Diagnostic Compare Key Register Section 5.4.13
B0h LPDDCSTAT1 LogicPD PSCON Diagnostic Compare Status Register 1 Section 5.4.14
B4h LPDDCSTAT2 LogicPD PSCON Diagnostic Compare Status Register 2 Section 5.4.15
C0h ISODIAGSTAT Isolation Diagnostic Status Register Section 5.4.16

<!-- Page 286 -->

PMM Registers www.ti.com
286 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.1 Logic Power Domain Control Register (LOGICPDPWRCTRL0)
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-3.Logic Power Domain Control Register (LOGICPDPWRCTRL0) (offset =00h)
31 28 27 24 23 20 19 16
Reserved LOGICPDON0 Reserved LOGICPDON1
R-0 R/WP-n R-0 R/WP-n
15 12 11 8 7 4 3 0
Reserved LOGICPDON2 Reserved LOGICPDON3
R-0 R/WP-n R-0 R/WP-n
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-2.Logic Power Domain Control Register (LOGICPDPWRCTRL0) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 LOGICPDON0 Read inUser andPrivileged Mode. Write inPrivileged Mode only.
Ah Read: Power domain PD2 isinOFF state.
Write: Power domain PD2 iscommanded toswitch toOFF state.
9h Reserved
Any other value Read: Power domain PD2 isinActive state.
Write: Power domain PD2 iscommanded toswitch toActive state.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 LOGICPDON1 Read inUser andPrivileged Mode. Write inPrivileged Mode only.
Ah Read: Power domain PD3 isinOFF state.
Write: Power domain PD3 iscommanded toswitch toOFF state.
9h Reserved
Any other value Read: Power domain PD3 isinActive state.
Write: Power domain PD3 iscommanded toswitch toActive state.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 LOGICPDON2 Read inUser andPrivileged Mode. Write inPrivileged Mode only.
Ah Read: Power domain PD4 isinOFF state.
Write: Power domain PD4 iscommanded toswitch toOFF state.
9h Reserved
Any other value Read: Power domain PD4 isinActive state.
Write: Power domain PD4 iscommanded toswitch toActive state.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 LOGICPDON3 Read inUser andPrivileged Mode. Write inPrivileged Mode only.
Ah Read: Power domain PD5 isinOFF state.
Write: Power domain PD5 iscommanded toswitch toOFF state.
9h Reserved
Any other value Read: Power domain PD5 isinActive state.
Write: Power domain PD5 iscommanded toswitch toActive state.

<!-- Page 287 -->

www.ti.com PMM Registers
287 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.2 Logic Power Domain Control Register (LOGICPDPWRCTRL1)
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-4.Logic Power Domain Control Register (LOGICPDPWRCTRL1) (offset =04h)
31 28 27 24 23 16
Reserved LOGICPDON4 Reserved
R-0 R/WP-n R-0
15 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-3.Logic Power Domain Control Register (LOGICPDPWRCTRL1) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 LOGICPDON4 Read inUser andPrivileged Mode. Write inPrivileged Mode only.
Ah Read: Power domain PD6 isinOFF state.
Write: Power domain PD6 iscommanded toswitch toOFF state.
9h Reserved
Any other value Read: Power domain PD6 isinActive state.
Write: Power domain PD6 iscommanded toswitch toActive state.
23-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 288 -->

PMM Registers www.ti.com
288 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.3 Power Domain Clock Disable Register (PDCLKDISREG)
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-5.Power Domain Clock Disable Register (PDCLKDISREG) (offset =20h)
31 8
Reserved
R-0
7 5 4 3 2 1 0
Reserved PDCLK_DIS[4] PDCLK_DIS[3] PDCLK_DIS[2] PDCLK_DIS[1] PDCLK_DIS[0]
R-0 R/WP-n R/WP-n R/WP-n R/WP-n R/WP-n
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-4.Power Domain Clock Disable Register (PDCLKDISREG) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 PDCLK_DIS[4] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[4]. Write inPrivileged
Mode only.
0 Enable clocks tologic power domain PD6.
1 Disable clocks tologic power domain PD6.
3 PDCLK_DIS[3] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[3]. Write inPrivileged
Mode only.
0 Enable clocks tologic power domain PD5.
1 Disable clocks tologic power domain PD5.
2 PDCLK_DIS[2] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[2]. Write inPrivileged
Mode only
0 Enable clocks tologic power domain PD4.
1 Disable clocks tologic power domain PD4.
1 PDCLK_DIS[1] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[1]. Write inPrivileged
Mode only.
0 Enable clocks tologic power domain PD3.
1 Disable clocks tologic power domain PD3.
0 PDCLK_DIS[0] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[0]. Write inPrivileged
Mode only.
0 Enable clocks tologic power domain PD2.
1 Disable clocks tologic power domain PD2.

<!-- Page 289 -->

www.ti.com PMM Registers
289 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.4 Power Domain Clock Disable SetRegister (PDCLKDISSETREG)
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-6. Power Domain Clock Disable SetRegister (PDCLKDISSETREG) (offset =24h)
31 8
Reserved
R-0
7 5 4 3 2 1 0
Reserved PDCLK_DISSET[4] PDCLK_DISSET[3] PDCLK_DISSET[2] PDCLK_DISSET[1] PDCLK_DISSET[0]
R-0 R/WP-n R/WP-n R/WP-n R/WP-n R/WP-n
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-5.Power Domain Clock Disable SetRegister (PDCLKDISSETREG)
Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 PDCLK_DISSET[4] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DISSET[4]. Write in
Privileged Mode only.
0 Noeffect tostate ofclocks topower domain PD6.
1 Disable clocks tologic power domain PD6.
3 PDCLK_DISSET[3] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DISSET[3]. Write in
Privileged Mode only.
0 Noeffect tostate ofclocks topower domain PD5.
1 Disable clocks tologic power domain PD5.
2 PDCLK_DISSET[2] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DISSET[2]. Write in
Privileged Mode only.
0 Noeffect tostate ofclocks topower domain PD4.
1 Disable clocks tologic power domain PD4.
1 PDCLK_DISSET[1] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DISSET[1]. Write in
Privileged Mode only.
0 Noeffect tostate ofclocks topower domain PD3.
1 Disable clocks tologic power domain PD3.
0 PDCLK_DISSET[0] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DISSET[0]. Write in
Privileged Mode only.
0 Noeffect tostate ofclocks topower domain PD2.
1 Disable clocks tologic power domain PD2.

<!-- Page 290 -->

PMM Registers www.ti.com
290 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.5 Power Domain Clock Disable Clear Register (PDCLKDISCLRREG)
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-7.Power Domain Clock Disable Clear Register (PDCLKDISCLRREG) (offset =28h)
31 8
Reserved
R-0
7 5 4 3 2 1 0
Reserved PDCLK_DISCLR[4] PDCLK_DISCLR[3] PDCLK_DISCLR[2] PDCLK_DISCLR[1] PDCLK_DISCLR[0]
R-0 R/WP-n R/WP-n R/WP-n R/WP-n R/WP-n
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-6.Power Domain Clock Disable Clear Register (PDCLKDISCLRREG)
Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 PDCLK_DISCLR[4] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[4]. Write inPrivileged
Mode only.
0 Noeffect tostate ofclocks topower domain PD6.
1 Enable clocks tologic power domain PD6.
3 PDCLK_DISCLR[3] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[3]. Write inPrivileged
Mode only.
0 Noeffect tostate ofclocks topower domain PD5.
1 Enable clocks tologic power domain PD5.
2 PDCLK_DISCLR[2] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[2]. Write inPrivileged
Mode only.
0 Noeffect tostate ofclocks topower domain PD4.
1 Enable clocks tologic power domain PD4.
1 PDCLK_DISCLR[1] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[1]. Write inPrivileged
Mode only.
0 Noeffect tostate ofclocks topower domain PD3.
1 Enable clocks tologic power domain PD3.
0 PDCLK_DISCLR[0] Read inUser andPrivileged Mode returns thecurrent value ofPDCLK_DIS[0]. Write inPrivileged
Mode only.
0 Noeffect tostate ofclocks topower domain PD2.
1 Enable clocks tologic power domain PD2.

<!-- Page 291 -->

www.ti.com PMM Registers
291 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.6 Logic Power Domain PD2 Power Status Register (LOGICPDPWRSTAT0)
This isaread-only register. Allwrites areignored.
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-8.Logic Power Domain PD2 Power Status Register (LOGICPDPWRSTAT0) (offset =40h)
31 25 24 23 17 16
ReservedLOGIC IN
TRANS0ReservedMEM IN
TRANS0
R-0 R-n R-0 R-n
15 9 8 7 2 1 0
ReservedDOMAIN
ON0ReservedLOGICPDPWR
STAT0
R-0 R-n R-0 R-n
LEGEND: R=Read only; -n=value after reset
Table 5-7.Logic Power Domain PD2 Power Status Register (LOGICPDPWRSTAT0)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 LOGIC INTRANS0 Logic intransition status forpower domain PD2.
Read inUser andPrivileged Mode.
0 Logic inpower domain PD2 isinthesteady Active orOFF state.
1 Logic inpower domain PD2 isintheprocess ofpower-down/up.
13-17 Reserved 0 Reads return 0.Writes have noeffect.
16 MEM INTRANS0 Memory intransition status forpower domain PD2.
Read inUser andPrivileged Mode.
0 Memory inpower domain PD2 isinthesteady Active orOFF state.
1 Memory inpower domain PD2 isintheprocess ofpower-down/up.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DOMAIN ON0 Current state ofpower domain PD2. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Power domain PD2 isintheOFF state.
1 Power domain PD2 isintheActive state.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 LOGICPDPWR STAT0 Logic power domain PD2 power state. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Logic power domain PD2 isswitched OFF.
1h Logic power domain PD2 isinIdlestate.
2h Reserved
3h Logic power domain PD2 isinActive state.

<!-- Page 292 -->

PMM Registers www.ti.com
292 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.7 Logic Power Domain PD3 Power Status Register (LOGICPDPWRSTAT1)
This isaread-only register. Allwrites areignored.
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-9.Logic Power Domain PD3 Power Status Register (LOGICPDPWRSTAT1) (offset =44h)
31 25 24 23 17 16
ReservedLOGIC IN
TRANS1ReservedMEM IN
TRANS1
R-0 R-n R-0 R-n
15 9 8 7 2 1 0
ReservedDOMAIN
ON1ReservedLOGICPDPWR
STAT1
R-0 R-n R-0 R-n
LEGEND: R=Read only; -n=value after reset
Table 5-8.Logic Power Domain PD3 Power Status Register (LOGICPDPWRSTAT1)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 LOGIC INTRANS1 Logic intransition status forpower domain PD3.
Read inUser andPrivileged Mode.
0 Logic inpower domain PD3 isinthesteady Active orOFF state.
1 Logic inpower domain PD3 isintheprocess ofpower-down/up.
13-17 Reserved 0 Reads return 0.Writes have noeffect.
16 MEM INTRANS1 Memory intransition status forpower domain PD3.
Read inUser andPrivileged Mode.
0 Memory inpower domain PD3 isinthesteady Active orOFF state.
1 Memory inpower domain PD3 isintheprocess ofpower-down/up.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DOMAIN ON1 Current state ofpower domain PD3. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Power domain PD3 isintheOFF state.
1 Power domain PD3 isintheActive state.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 LOGICPDPWR STAT1 Logic power domain PD3 power state. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Logic power domain PD3 isswitched OFF.
1h Logic power domain PD3 isinIdlestate.
2h Reserved
3h Logic power domain PD3 isinActive state.

<!-- Page 293 -->

www.ti.com PMM Registers
293 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.8 Logic Power Domain PD4 Power Status Register (LOGICPDPWRSTAT2)
This isaread-only register. Allwrites areignored.
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-10. Logic Power Domain PD4 Power Status Register (LOGICPDPWRSTAT2) (offset =48h)
31 25 24 23 17 16
ReservedLOGIC IN
TRANS2ReservedMEM IN
TRANS2
R-0 R-n R-0 R-n
15 9 8 7 2 1 0
ReservedDOMAIN
ON2ReservedLOGICPDPWR
STAT2
R-0 R-n R-0 R-n
LEGEND: R=Read only; -n=value after reset
Table 5-9.Logic Power Domain PD4 Power Status Register (LOGICPDPWRSTAT2)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 LOGIC INTRANS2 Logic intransition status forpower domain PD4.
Read inUser andPrivileged Mode.
0 Logic inpower domain PD4 isinthesteady Active orOFF state.
1 Logic inpower domain PD4 isintheprocess ofpower-down/up.
13-17 Reserved 0 Reads return 0.Writes have noeffect.
16 MEM INTRANS2 Memory intransition status forpower domain PD4.
Read inUser andPrivileged Mode.
0 Memory inpower domain PD4 isinthesteady Active orOFF state.
1 Memory inpower domain PD4 isintheprocess ofpower-down/up.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DOMAIN ON2 Current state ofpower domain PD4. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Power domain PD4 isintheOFF state.
1 Power domain PD4 isintheActive state.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 LOGICPDPWR STAT2 Logic power domain PD4 power state. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Logic power domain PD4 isswitched OFF.
1h Logic power domain PD4 isinIdlestate.
2h Reserved
3h Logic power domain PD4 isinActive state.

<!-- Page 294 -->

PMM Registers www.ti.com
294 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.9 Logic Power Domain PD5 Power Status Register (LOGICPDPWRSTAT3)
This isaread-only register. Allwrites areignored.
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-11. Logic Power Domain PD5 Power Status Register (LOGICPDPWRSTAT3) (offset =4Ch)
31 25 24 23 17 16
ReservedLOGIC IN
TRANS3ReservedMEM IN
TRANS3
R-0 R-n R-0 R-n
15 9 8 7 2 1 0
ReservedDOMAIN
ON3ReservedLOGICPDPWR
STAT3
R-0 R-n R-0 R-n
LEGEND: R=Read only; -n=value after reset
Table 5-10. Logic Power Domain PD5 Power Status Register (LOGICPDPWRSTAT3)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 LOGIC INTRANS3 Logic intransition status forpower domain PD5.
Read inUser andPrivileged Mode.
0 Logic inpower domain PD5 isinthesteady Active orOFF state.
1 Logic inpower domain PD5 isintheprocess ofpower-down/up.
13-17 Reserved 0 Reads return 0.Writes have noeffect.
16 MEM INTRANS3 Memory intransition status forpower domain PD5.
Read inUser andPrivileged Mode.
0 Memory inpower domain PD5 isinthesteady Active orOFF state.
1 Memory inpower domain PD5 isintheprocess ofpower-down/up.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DOMAIN ON3 Current state ofpower domain PD5. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Power domain PD5 isintheOFF state.
1 Power domain PD5 isintheActive state.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 LOGICPDPWR STAT3 Logic power domain PD5 power state. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Logic power domain PD5 isswitched OFF.
1h Logic power domain PD5 isinIdlestate.
2h Reserved
3h Logic power domain PD5 isinActive state.

<!-- Page 295 -->

www.ti.com PMM Registers
295 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.10 Logic Power Domain PD6 Power Status Register (LOGICPDPWRSTAT4)
This isaread-only register. Allwrites areignored.
The default values ofthecontrol fields aredetermined bythedevice reset configuration word stored inthe
TI-OTP region offlash bank 0.
Figure 5-12. Logic Power Domain PD6 Power Status Register (LOGICPDPWRSTAT4) (offset =50h)
31 25 24 23 17 16
ReservedLOGIC IN
TRANS4ReservedMEM IN
TRANS4
R-0 R-n R-0 R-n
15 9 8 7 2 1 0
ReservedDOMAIN
ON4ReservedLOGICPDPWR
STAT4
R-0 R-n R-0 R-n
LEGEND: R=Read only; -n=value after reset
Table 5-11. Logic Power Domain PD6 Power Status Register (LOGICPDPWRSTAT4)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 LOGIC INTRANS4 Logic intransition status forpower domain PD6.
Read inUser andPrivileged Mode.
0 Logic inpower domain PD6 isinthesteady Active orOFF state.
1 Logic inpower domain PD6 isintheprocess ofpower-down/up.
13-17 Reserved 0 Reads return 0.Writes have noeffect.
16 MEM INTRANS4 Memory intransition status forpower domain PD6.
Read inUser andPrivileged Mode.
0 Memory inpower domain PD6 isinthesteady Active orOFF state.
1 Memory inpower domain PD6 isintheprocess ofpower-down/up.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 DOMAIN ON4 Current state ofpower domain PD6. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Power domain PD6 isintheOFF state.
1 Power domain PD6 isintheActive state.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 LOGICPDPWR STAT4 Logic power domain PD6 power state. The default value ofthisfield iscontrolled bythe
device reset configuration word intheTI-OTP region offlash bank 0.
Read inUser andPrivileged Mode.
0 Logic power domain PD6 isswitched OFF.
1h Logic power domain PD6 isinIdlestate.
2h Reserved
3h Logic power domain PD6 isinActive state.

<!-- Page 296 -->

PMM Registers www.ti.com
296 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.11 Global Control Register 1(GLOBALCTRL1)
Figure 5-13. Global Control Register 1(GLOBALCTRL1) (offset =A0h)
31 16
Reserved
R-0
15 9 8
Reserved PMCTRL PWRDN
R-0 R/WP-0
7 1 0
Reserved AUTO CLK WAKE ENA
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-12. Global Control Register 1(GLOBALCTRL1) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PMCTRL PWRDN PMC/PSCON Power Down
Read inUser andPrivileged Mode returns current value ofPMCTRL PWRDN. Write in
Privileged mode only.
0 Enable theclock topmctrl_wakeup block.
1 Disable theclock topmctrl_wakeup block, which contains PMC andallPSCONs.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 AUTO CLK WAKE ENA Automatic Clock Enable onWake Up
Read inUser andPrivileged Mode returns current value ofAUTO CLK WAKE ENA. Write in
Privileged mode only.
0 Disable automatic clock wake up.The application must enable clocks byclearing thecorrect
bitinthePDCLK_DIS register.
1 Enable automatic clock wake upwhen apower domain transitions toActive state.

<!-- Page 297 -->

www.ti.com PMM Registers
297 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.12 Global Status Register (GLOBALSTAT)
Figure 5-14. Global Status Register (GLOBALSTAT) (offset =A8h)
31 16
Reserved
R-0
15 1 0
Reserved PMCTRL IDLE
R-0 R-1
LEGEND: R=Read only; -n=value after reset
Table 5-13. Global Status Register (GLOBALSTAT) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PMCTRL IDLE State ofPMC andallPSCONs. The PMM captures thestatus ofPMC andPSCONs asthey donot
have aregister interface tothehost CPU.
0 PMC andPSCONs forallpower domains areintheprocess ofgenerating power state transition
control sequence forlogic and/or SRAM.
1 PMC andPSCONs forallpower domains have completed generating power state transition control
sequence triggered byPMC input control signals.
5.4.13 PSCON Diagnostic Compare Key Register (PRCKEYREG)
Figure 5-15. PSCON Diagnostic Compare KeyRegister (PRCKEYREG) (offset =ACh)
31 16
Reserved
R-0
15 4 3 0
Reserved MKEY
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 5-14. PSCON Diagnostic Compare KeyRegister (PRCKEYREG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MKEY Diagnostic PSCON Mode Key. The mode keyisapplied toallindividual PSCON compare units.
Read inUser andPrivileged mode returns thecurrent value ofMKEY. Write inPrivileged mode only.
0 Lock Step mode
6h Self-test mode
9h Error Forcing mode
Fh Self-test Error Forcing Mode
Allothers Lock Step mode

<!-- Page 298 -->

PMM Registers www.ti.com
298 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.14 LogicPD PSCON Diagnostic Compare Status Register 1(LPDDCSTAT1)
Figure 5-16. LogicPD PSCON Diagnostic Compare Status Register 1(LPDDCSTAT1) (offset =B0h)
31 24
Reserved
R-0
23 21 20 19 18 17 16
Reserved LCMPE[4] LCMPE[3] LCMPE[2] LCMPE[1] LCMPE[0]
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
15 8
Reserved
R-0
7 5 4 3 2 1 0
Reserved LSTC[4] LSTC[3] LSTC[2] LSTC[1] LSTC[0]
R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toClear; WP=Write inprivileged mode only; -n=value after reset
Table 5-15. LogicPD PSCON Diagnostic Compare Status Register 1(LPDDCSTAT1)
Field Descriptions
Bit Field Value Description
31-21 Reserved 0 Reads return 0.Writes have noeffect.
20-16 LCMPE[4-0] Logic Power Domain Compare Error
Each ofthese bitscorresponds toalogic power domain:
Bit4forPD6,
Bit3forPD5,
Bit2forPD4,
Bit1forPD3,
Bit0forPD2.
Read inUser andPrivileged Mode. Write inPrivileged mode only.
0 Read: PSCON signals areidentical.
Write: Writing 0hasnoeffect.
1 Read: PSCON signal compare mismatch identified.
Write: Clears thecorresponding LCMPE bit,ifset.
15-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 LSTC[4-0] Logic Power Domain Self-test Complete
Each ofthese bitscorresponds toalogic power domain:
Bit4forPD6,
Bit3forPD5,
Bit2forPD4,
Bit1forPD3,
Bit0forPD2.
Read inUser andPrivileged Mode. Writes have noeffect.
0 Self-test isongoing ifself-test mode isentered.
1 Self-test iscomplete.

<!-- Page 299 -->

www.ti.com PMM Registers
299 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.15 LogicPD PSCON Diagnostic Compare Status Register 2(LPDDCSTAT2)
Figure 5-17. LogicPD PSCON Diagnostic Compare Status Register 2(LPDDCSTAT2) (offset =B4h)
31 24
Reserved
R-0
23 21 20 19 18 17 16
Reserved LSTET[4] LSTET[3] LSTET[2] LSTET[1] LSTET[0]
R-0 R-0 R-0 R-0 R-0 R-0
15 8
Reserved
R-0
7 5 4 3 2 1 0
Reserved LSTE[4] LSTE[3] LSTE[2] LSTE[1] LSTE[0]
R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 5-16. LogicPD PSCON Diagnostic Compare Status Register 2(LPDDCSTAT2)
Field Descriptions
Bit Field Value Description
31-21 Reserved 0 Reads return 0.Writes have noeffect.
20-16 LSTET[4-0] Logic Power Domain Self-test Error Type
Each ofthese bitscorresponds toalogic power domain:
Bit4forPD6,
Bit3forPD5,
Bit2forPD4,
Bit1forPD3,
Bit0forPD2.
Read inUser andPrivileged Mode. Writes have noeffect.
0 Self-test failed during compare match test.
1 Self-test failed during compare mismatch test.
15-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 LSTE[4-0] Logic Power Domain Self-test Error
Each ofthese bitscorresponds toalogic power domain:
Bit4forPD6,
Bit3forPD5,
Bit2forPD4,
Bit1forPD3,
Bit0forPD2.
Read inUser andPrivileged Mode. Writes have noeffect.
0 Self-test passed.
1 Self-test failed.

<!-- Page 300 -->

PMM Registers www.ti.com
300 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedPower Management Module (PMM)5.4.16 Isolation Diagnostic Status Register (ISODIAGSTAT)
Figure 5-18. Isolation Diagnostic Status Register (ISODIAGSTAT) (offset =C0h)
31 8
Reserved
R-0
4 3 2 1 0
Reserved ISO DIAG[4] ISO DIAG[3] ISO DIAG[2] ISO DIAG[1] ISO DIAG[0]
R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 5-17. Isolation Diagnostic Status Register (ISODIAGSTAT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 ISO DIAG[4-0] Isolation Diagnostic
Each ofthese bitscorresponds toalogic power domain:
Bit4forPD6,
Bit3forPD5,
Bit2forPD4,
Bit1forPD3,
Bit0forPD2.
Read inUser andPrivileged Mode. Writes have noeffect.
0 Isolation isenabled forcorresponding power domain
1 Isolation isdisabled forcorresponding power domain