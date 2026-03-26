# Enhanced Pulse Width Modulator (ePWM) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1995-2107

---


<!-- Page 1995 -->

1995 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleChapter 35
SPNU563A -March 2018
Enhanced Pulse Width Modulator (ePWM) Module
The enhanced pulse width modulator (ePWM) peripheral isakeyelement incontrolling many ofthepower
electronic systems found inboth commercial and industrial equipments. The features supported bythe
ePWM make itespecially suitable fordigital motor control.
Topic ........................................................................................................................... Page
35.1 Introduction ................................................................................................... 1996
35.2 ePWM Submodules ......................................................................................... 2000
35.3 Application Examples ...................................................................................... 2055
35.4 ePWM Registers ............................................................................................. 2070

<!-- Page 1996 -->

Introduction www.ti.com
1996 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.1 Introduction
Aneffective PWM peripheral must beable togenerate complex pulse width waveforms with minimal CPU
overhead orintervention. Itneeds tobehighly programmable andvery flexible while being easy to
understand anduse. The ePWM unitdescribed here addresses these requirements byallocating all
needed timing andcontrol resources onaperPWM channel basis. Cross coupling orsharing ofresources
hasbeen avoided; instead, theePWM isbuilt upfrom smaller single channel modules with separate
resources thatcanoperate together asrequired toform asystem. This modular approach results inan
orthogonal architecture andprovides amore transparent view oftheperipheral structure, helping users to
understand itsoperation quickly.
Inthisdocument theletter xwithin asignal ormodule name isused toindicate ageneric ePWM instance
onadevice. Forexample output signals EPWMxA andEPWMxB refer totheoutput signals from the
ePWMx instance. Thus, EPWM1A andEPWM1B belong toePWM1 andlikewise EPWM4A andEPWM4B
belong toePWM4.
35.1.1 Submodule Overview
The ePWM module represents onecomplete PWM channel composed oftwoPWM outputs: EPWMxA
andEPWMxB. Multiple ePWM modules areinstanced within adevice asshown inFigure 35-1.Each
ePWM instance isidentical andisindicated byanumerical value starting with 1.Forexample, ePWM1 is
thefirstinstance andePWM3 isthethird instance inthesystem, andePWMx indicates anyinstance.
The ePWM modules arechained together viaaclock synchronization scheme thatallows them tooperate
asasingle system when required. Additionally, thissynchronization scheme canbeextended tothe
capture peripheral modules (eCAP). Modules canalso operate stand-alone.
Each ePWM module supports thefollowing features:
*Dedicated 16-bit time-base counter with period andfrequency control
*Two PWM outputs (EPWMxA andEPWMxB) thatcanbeused inthefollowing configurations:
-Two independent PWM outputs with single-edge operation
-Two independent PWM outputs with dual-edge symmetric operation
-One independent PWM output with dual-edge asymmetric operation
*Asynchronous override control ofPWM signals through software.
*Programmable phase-control support forlagorlead operation relative toother ePWM modules.
*Hardware-locked (synchronized) phase relationship onacycle-by-cycle basis.
*Dead-band generation with independent rising andfalling edge delay control.
*Programmable tripzone allocation ofboth cycle-by-cycle tripandone-shot triponfault conditions.
*Atripcondition canforce either high, low, orhigh-impedance state logic levels atPWM outputs.
*Allevents cantrigger both CPU interrupts andADC start ofconversion (SOC)
*Programmable event prescaling minimizes CPU overhead oninterrupts.
*PWM chopping byhigh-frequency carrier signal, useful forpulse transformer gate drives.
Each ePWM module isconnected totheinput/output signals shown inFigure 35-1 .The signals are
described indetail insubsequent sections.
Each ePWM module consists ofeight submodules andisconnected within asystem viathesignals shown
inFigure 35-2.

<!-- Page 1997 -->

TZ1/2/3n
SOCA1, SOCB1EPWM1INTnEPWM1TZINTn
EQEP1ERR / EQEP2ERR /
EQEP1ERR or EQEP2ERR
Debug Mode EntryOSC FAIL or PLL Slip
IOMUXEPWMSYNCI
ePWM1
eCAP1EPWM1A
EPWM1B
EPWM2/3/4/5/6A
EPWM2/3/4/5/6B
ePWM7EPWM7A
ECAP1Pulse
Stretch,
8 VCLK3
cyclesEPWM1SYNCO
(after stretch)ADC Wrapper
VBus32 / VBus32DPEPWM1ENCLK
TBCLKSYNCVIM
VCLK3, SYS_nRST
EPWM2/3/4/5/6ENCLK
TBCLKSYNC
EPWM7ENCLK
TBCLKSYNC
ECAP1INTnePWM
2/3/4/5/6
VIMEQEP1 + EQEP2
EPWM7BCPUSystem Module
TZ6nTZ5nTZ4n
VCLK3, SYS_nRSTTZ1/2/3n
TZ1/2/3nDebug Mode EntryOSC FAIL or PLL Slip
TZ6nTZ5nTZ4n
Debug Mode EntryOSC FAIL or PLL SLip
TZ6nTZ5nTZ4nSOCA2/3/4/5/6
SOCB2/3/4/5/6EPWM2/3/4/5/6INTnEPWM2/3/4/5/6TZINTn
EPWM7INTnEPWM7TZINTnVBus32
VBus32
VCLK3, SYS_nRSTVBus32VIM
ADC WrapperVIM
EQEP1 + EQEP2
CPUSystem ModuleVIM
ADC WrapperVIM
EQEP1 + EQEP2
CPUSystem ModuleVIMMux
Selector
Mux
Selector
EQEP1ERR / EQEP2ERR /
EQEP1ERR or EQEP2ERR
EQEP1ERR / EQEP2ERR /
EQEP1ERR or EQEP2ERRSOCA7, SOCB7Mux
SelectorNHET1_LOOP_SYNC
EPWM1SYNCO (before stretch)SYNCI
SYNCOsee Note A
see Note A
see Note A
www.ti.com Introduction
1997 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-1. Multiple ePWM Modules

<!-- Page 1998 -->

EPWMxINTEPWMxTZINT
EPWMxSOCA
EPWMxSOCBEPWMxSYNCI
EPWMxSYNCOTime-base (TB) module
Counter-compare (CC) module
Action-qualifier (AQ) module
Dead-band (DB) module
PWM-chopper (PC) module
Event-trigger (ET) module
Trip-zone (TZ) module Peripheral busePWM module 
nTZ1 to TZ3
EPWMxA
EPWMxBVIM
ADC
GPIO
MUX
Digital Compare (DC) moduleEQEP1ERR / EQEP2 ERROSCFAIL / PLL SlipCPU Debug Mode
Introduction www.ti.com
1998 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-2. Submodules andSignal Connections foranePWM Module
The main signals used bytheePWM module are:
*PWM output signals (EPWMxA andEPWMxB).
The PWM output signals aremade available external tothedevice through theI/OMultiplexing Module
(IOMM) asdescribed intheIOMM chapter.
*Trip-zone signals (TZ1toTZ6).
These input signals alert theePWM module offault conditions external totheePWM module. Each
ePWM module canbeconfigured toeither useorignore anyofthetrip-zone signals. The TZ1 toTZ3
trip-zone signals canbeconfigured asasynchronous inputs, ordouble-synchronized using VCLK3, or
double-synchronized andfiltered through a6-VCLK3-cycle counter before connecting totheePWM
modules. This selection isdone byconfiguring registers intheIOMM. TZ4 isconnected toaninverted
eQEP1 error signal (EQEP1ERR), ortoaninverted eQEP2 error signal (EQEP2ERR), oranOR-
combination ofEQEP1ERR andEQEP2ERR. This selection isalso done viatheIOMM registers. TZ5
isconnected tothesystem clock failstatus. This isasserted whenever anoscillator failure isdetected,
oraPLL slipisdetected. TZ6 isconnected tothedebug mode entry indicator output from theCPU.
This allows youtoconfigure atripaction when theCPU halts.
*Time-base synchronization input (EPWMxSYNCI) andoutput (EPWMxSYNCO) signals .
The synchronization signals daisy chain theePWM modules together. Each module canbeconfigured
toeither useorignore itssynchronization input. The clock synchronization input andoutput signal are
brought outtopins only forePWM1 (ePWM module #1). The synchronization output forePWM1
(EPWM1SYNCO) isalso connected totheSYNCI ofthefirstenhanced capture module (eCAP1).
*ADC start-of-conversion signals (EPWMxSOCA andEPWMxSOCB).
Each ePWM module hastwoADC start ofconversion signals. Any ePWM module cantrigger astart of
conversion. Which event triggers thestart ofconversion isconfigured intheEvent-Trigger submodule
oftheePWM.
*Peripheral Bus
The peripheral busis32-bits wide andallows both 16-bit and32-bit writes totheePWM register file.

<!-- Page 1999 -->

www.ti.com Introduction
1999 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.1.2 Register Mapping
The complete ePWM module control andstatus register setisgrouped bysubmodule asshown in
Table 35-1.Each register setisduplicated foreach instance oftheePWM module.
(1)Locations notshown arereserved.Table 35-1. ePWM Module Control andStatus Register SetGrouped bySubmodule
NameAddress
Offset(1)Size
(x16) ShadowPrivileged
Mode Write
Only? Description
Time-Base Submodule Registers
TBSTS 00h 1 No No Time-Base Status Register
TBCTL 02h 1 No No Time-Base Control Register
TBPHS 04h 1 No No Time-Base Phase Register
TBPRD 08h 1 Yes No Time-Base Period Register
TBCTR 0Ah 1 No No Time-Base Counter Register
Counter-Compare Submodule Registers
CMPCTL 0Ch 1 No No Counter-Compare Control Register
CMPA 10h 1 Yes No Counter-Compare ARegister
CMPB 16h 1 Yes No Counter-Compare BRegister
Action-Qualifier Submodule Registers
AQCTLA 14h 1 No No Action-Qualifier Control Register forOutput A(EPWMxA)
AQSFRC 18h 1 No No Action-Qualifier Software Force Register
AQCTLB 1Ah 1 No No Action-Qualifier Control Register forOutput B(EPWMxB)
AQCSFRC 1Eh 1 Yes No Action-Qualifier Continuous S/W Force Register Set
Dead-Band Generator Submodule Registers
DBCTL 1Ch 1 No No Dead-Band Generator Control Register
DBFED 20h 1 No No Dead-Band Generator Falling Edge Delay Count Register
DBRED 22h 1 No No Dead-Band Generator Rising Edge Delay Count Register
Trip-Zone Submodule Registers
TZDCSEL 24h 1 No Yes Trip Zone Digital Compare Select Register
TZSEL 26h 1 No Yes Trip-Zone Select Register
TZEINT 28h 1 No Yes Trip-Zone Enable Interrupt Register
TZCTL 2Ah 1 No Yes Trip-Zone Control Register
TZCLR 2Ch 1 No Yes Trip-Zone Clear Register
TZFLG 2Eh 1 No No Trip-Zone Flag Register
TZFRC 32h 1 No Yes Trip-Zone Force Register
Event-Trigger Submodule Registers
ETSEL 30h 1 No No Event-Trigger Selection Register
ETFLG 34h 1 No No Event-Trigger Flag Register
ETPS 36h 1 No No Event-Trigger Pre-Scale Register
ETFRC 38h 1 No No Event-Trigger Force Register
ETCLR 3Ah 1 No No Event-Trigger Clear Register
PWM-Chopper Submodule Registers
PCCTL 3Eh 1 No No PWM-Chopper Control Register

<!-- Page 2000 -->

ePWM Submodules www.ti.com
2000 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-1. ePWM Module Control andStatus Register SetGrouped bySubmodule (continued)
NameAddress
Offset(1)Size
(x16) ShadowPrivileged
Mode Write
Only? Description
Digital Compare Event Registers
DCACTL 60h 1 No Yes Digital Compare AControl Register
DCTRIPSEL 62h 1 No Yes Digital Compare Trip Select Register
DCFCTL 64h 1 No Yes Digital Compare Filter Control Register
DCBCTL 66h 1 No Yes Digital Compare BControl Register
DCFOFFSET 68h 1 Writes No Digital Compare Filter Offset Register
DCCAPCTL 6Ah 1 No Yes Digital Compare Capture Control Register
DCFWINDOW 6Ch 1 No No Digital Compare Filter Window Register
DCFOFFSETCNT 6Eh 1 No No Digital Compare Filter Offset Counter Register
DCCAP 70h 1 Yes No Digital Compare Counter Capture Register
DCFWINDOWCNT 72h 1 No No Digital Compare Filter Window Counter Register
35.2 ePWM Submodules
Eight submodules areincluded inevery ePWM peripheral. Each ofthese submodules performs specific
tasks thatcanbeconfigured bysoftware.
35.2.1 Overview
Table 35-2 lists theeight keysubmodules together with alistoftheir main configuration parameters. For
example, ifyouneed toadjust orcontrol theduty cycle ofaPWM waveform, then youshould seethe
counter-compare submodule inSection 35.2.3 forrelevant details.
Table 35-2. Submodule Configuration Parameters
Submodule Configuration Parameter orOption
Time-base (TB) *Scale thetime-base clock (TBCLK) relative tothesystem clock (VCLK3).
*Configure thePWM time-base counter (TBCTR) frequency orperiod.
*Setthemode forthetime-base counter:
-count-up mode: used forasymmetric PWM
-count-down mode: used forasymmetric PWM
-count-up-and-down mode: used forsymmetric PWM
*Configure thetime-base phase relative toanother ePWM module.
*Synchronize thetime-base counter between modules through hardware orsoftware.
*Configure thedirection (upordown) ofthetime-base counter after asynchronization event.
*Configure how thetime-base counter willbehave when thedevice ishalted byanemulator.
*Specify thesource forthesynchronization output oftheePWM module:
-Synchronization input signal
-Time-base counter equal tozero
-Time-base counter equal tocounter-compare B(CMPB)
-Nooutput synchronization signal generated.
Counter-compare (CC)*Specify thePWM duty cycle foroutput EPWMxA and/or output EPWMxB
*Specify thetime atwhich switching events occur ontheEPWMxA orEPWMxB output
Action-qualifier (AQ) *Specify thetype ofaction taken when atime-base orcounter-compare submodule event occurs:
-Noaction taken
-Output EPWMxA and/or EPWMxB switched high
-Output EPWMxA and/or EPWMxB switched low
-Output EPWMxA and/or EPWMxB toggled
*Force thePWM output state through software control
*Configure andcontrol thePWM dead-band through software

<!-- Page 2001 -->

www.ti.com ePWM Submodules
2001 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-2. Submodule Configuration Parameters (continued)
Submodule Configuration Parameter orOption
Dead-band (DB) *Control oftraditional complementary dead-band relationship between upper andlower switches
*Specify theoutput rising-edge-delay value
*Specify theoutput falling-edge delay value
*Bypass thedead-band module entirely. Inthiscase thePWM waveform ispassed through
without modification.
*Option toenable half-cycle clocking fordouble resolution.
PWM-chopper (PC) *Create achopping (carrier) frequency.
*Pulse width ofthefirstpulse inthechopped pulse train.
*Duty cycle ofthesecond andsubsequent pulses.
*Bypass thePWM-chopper module entirely. Inthiscase thePWM waveform ispassed through
without modification.
Trip-zone (TZ) *Configure theePWM module toreact toone, all,ornone ofthetrip-zone signals ordigital
compare events.
*Specify thetripping action taken when afault occurs:
-Force EPWMxA and/or EPWMxB high
-Force EPWMxA and/or EPWMxB low
-Force EPWMxA and/or EPWMxB toahigh-impedance state
-Configure EPWMxA and/or EPWMxB toignore anytripcondition.
*Configure how often theePWM willreact toeach trip-zone signal:
-One-shot
-Cycle-by-cycle
*Enable thetrip-zone toinitiate aninterrupt.
*Bypass thetrip-zone module entirely.
Event-trigger (ET) *Enable theePWM events thatwilltrigger aninterrupt.
*Enable ePWM events thatwilltrigger anADC start-of-conversion event.
*Specify therate atwhich events cause triggers (every occurrence orevery second orthird
occurrence)
*Poll, set,orclear event flags
Digital-compare (DC)*Enables tripzone signals tocreate events andfiltered events
*Specify event-filtering options tocapture TBCTR counter orgenerate blanking window
Code examples areprovided inthischapter thatshow how toimplement various ePWM module
configurations. These examples usetheconstant definitions inthedevice EPwm_defines.h fileinthe
device-specific header fileandperipheral examples software package.

<!-- Page 2002 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
ePWM Submodules www.ti.com
2002 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.2 Time-Base (TB) Submodule
Each ePWM module hasitsown time-base submodule thatdetermines alloftheevent timing forthe
ePWM module. Built-in synchronization logic allows thetime-base ofmultiple ePWM modules towork
together asasingle system. Figure 35-3 illustrates thetime-base module's place within theePWM.
Figure 35-3. Time-Base Submodule Block Diagram
35.2.2.1 Purpose oftheTime-Base Submodule
You canconfigure thetime-base submodule forthefollowing:
*Specify theePWM time-base counter (TBCTR) frequency orperiod tocontrol how often events occur.
*Manage time-base synchronization with other ePWM modules.
*Maintain aphase relationship with other ePWM modules.
*Setthetime-base counter tocount-up, count-down, orcount-up-and-down mode.
*Generate thefollowing events:
-CTR =PRD: Time-base counter equal tothespecified period (TBCTR =TBPRD).
-CTR =Zero: Time-base counter equal tozero (TBCTR =0x0000).
*Configure therate ofthetime-base clock; aprescaled version ofthedevice peripheral clock domain
(VCLK3). This allows thetime-base counter toincrement/decrement ataslower rate.
35.2.2.2 Controlling andMonitoring theTime-Base Submodule
Table 35-3 shows theregisters used tocontrol andmonitor thetime-base submodule.
Table 35-3. Time-Base Submodule Registers
Register Address Offset Shadowed Description
TBSTS 00h No Time-Base Status Register
TBCTL 02h No Time-Base Control Register
TBPHS 04h No Time-Base Phase Register
TBPRD 08h Yes Time-Base Period Register
TBCTR 0Ah No Time-Base Counter Register

<!-- Page 2003 -->

TBCTL[SYNCOSEL]TBPRD 
Period ActiveTBPRD 
Period Shadow
16TBCTL[SWFSYNC]
CTR = PRD
TBPHS
Phase Active RegCounter
UP/DOWN16
Sync
Out
SelectEPWMxSYNCOReset
Load16
TBCTL[PHSEN]CTR = Zero
CTR = CMPB
Disable
XEPWMxSYNCITBCTL[PRDLD]
TBCTR[15:0]
Mode TBCTL[CTRMODE]CTR = Zero
CTR_max
TBCLK
 Clock
PrescaleVCLK4 TBCLK
TBCTL[HSPCLKDIV]
TBCTL[CLKDIV]CTR_dir
TBCTR
Counter Active RegclkMaxDirZeroDCBEVT1.sync(A)DCAEVT1.sync(A)
A.  These signals are generated by the digital compare (DC) sub module.
www.ti.com ePWM Submodules
2003 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-4 shows thecritical signals andregisters ofthetime-base submodule. Table 35-4 provides
descriptions ofthekeysignals associated with thetime-base submodule.
Figure 35-4. Time-Base Submodule Signals andRegisters
Table 35-4. KeyTime-Base Signals
Signal Description
EPWMxSYNCI Time-base synchronization input.
Input pulse used tosynchronize thetime-base counter with thecounter ofePWM module earlier inthe
synchronization chain. AnePWM peripheral canbeconfigured touseorignore thissignal. ForthefirstePWM
module (EPWM1) thissignal comes from adevice pinorfrom theN2HET1 module. Forsubsequent ePWM
modules thissignal ispassed from another ePWM peripheral. Forexample, EPWM2SYNCI isgenerated bythe
ePWM1 peripheral, EPWM3SYNCI isgenerated byePWM2 andsoforth. See Section 35.2.2.3.3 for
information onthesynchronization order ofaparticular device.
EPWMxSYNCO Time-base synchronization output.
This output pulse isused tosynchronize thecounter ofanePWM module later inthesynchronization chain.
The ePWM module generates thissignal from oneofthree event sources:
1.EPWMxSYNCI (Synchronization input pulse)
2.CTR =Zero: The time-base counter equal tozero (TBCTR =0x0000).
3.CTR =CMPB: The time-base counter equal tothecounter-compare B(TBCTR =CMPB) register.
CTR =PRD Time-base counter equal tothespecified period.
This signal isgenerated whenever thecounter value isequal totheactive period register value. That iswhen
TBCTR =TBPRD.
CTR =Zero Time-base counter equal tozero
This signal isgenerated whenever thecounter value iszero. That iswhen TBCTR equals 0x0000.

<!-- Page 2004 -->

ePWM Submodules www.ti.com
2004 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-4. KeyTime-Base Signals (continued)
Signal Description
CTR =CMPB Time-base counter equal toactive counter-compare Bregister (TBCTR =CMPB).
This event isgenerated bythecounter-compare submodule andused bythesynchronization outlogic
CTR_dir Time-base counter direction.
Indicates thecurrent direction oftheePWM 'stime-base counter. This signal ishigh when thecounter is
increasing andlowwhen itisdecreasing.
CTR_max Time-base counter equal max value. (TBCTR =0xFFFF)
Generated event when theTBCTR value reaches itsmaximum value. This signal isonly used only asastatus
bit
TBCLK Time-base clock.
This isaprescaled version ofthesystem clock (VCLK3) andisused byallsubmodules within theePWM. This
clock determines therate atwhich time-base counter increments ordecrements.
35.2.2.3 Calculating PWM Period andFrequency
The frequency ofPWM events iscontrolled bythetime-base period (TBPRD) register andthemode ofthe
time-base counter. Figure 35-5 shows theperiod (Tpwm)andfrequency (Fpwm)relationships fortheup-
count, down-count, andup-down-count time-base counter modes when theperiod issetto4(TBPRD =
4).The time increment foreach step isdefined bythetime-base clock (TBCLK) which isaprescaled
version ofthesystem clock (VCLK3).
The time-base counter hasthree modes ofoperation selected bythetime-base control register (TBCTL):
*Up-Down-Count Mode:
Inup-down-count mode, thetime-base counter starts from zero andincrements until theperiod
(TBPRD) value isreached. When theperiod value isreached, thetime-base counter then decrements
until itreaches zero. Atthispoint thecounter repeats thepattern andbegins toincrement.
*Up-Count Mode:
Inthismode, thetime-base counter starts from zero andincrements until itreaches thevalue inthe
period register (TBPRD). When theperiod value isreached, thetime-base counter resets tozero and
begins toincrement once again.
*Down-Count Mode:
Indown-count mode, thetime-base counter starts from theperiod (TBPRD) value anddecrements until
itreaches zero. When itreaches zero, thetime-base counter isreset totheperiod value anditbegins
todecrement once again.
35.2.2.3.1 Time-Base Period Shadow Register
The time-base period register (TBPRD) hasashadow register. Shadowing allows theregister update to
besynchronized with thehardware. The following definitions areused todescribe allshadow registers in
theePWM module:
*Active Register
The active register controls thehardware andisresponsible foractions thatthehardware causes or
invokes.
*Shadow Register
The shadow register buffers orprovides atemporary holding location fortheactive register. Ithasno
direct effect onanycontrol hardware. Atastrategic point intime theshadow register's content is
transferred totheactive register. This prevents corruption orspurious operation duetotheregister
being asynchronously modified bysoftware.

<!-- Page 2005 -->

PRD
4
4 4PRD
4
123
0123
0123
0Z
1234
0123
CTR_dir1234
0123
0
Up Down Down UpTPWM = (TBPRD + 1) x T TBCLKFor Up Count and Down Count
For Up and Down CountFPWM = 1/ (TPWM)
TPWM = 2 x TBPRD x T TBCLK
FPWM = 1 / (TPWM)1234
01234
0123
0TPWM
Z
TPWM
TPWM TPWM
www.ti.com ePWM Submodules
2005 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-5. Time-Base Frequency andPeriod
The memory address oftheshadow period register isthesame astheactive register. Which register is
written toorread from isdetermined bytheTBCTL[PRDLD] bit.This bitenables anddisables theTBPRD
shadow register asfollows:
*Time-Base Period Shadow Mode:
The TBPRD shadow register isenabled when TBCTL[PRDLD] =0.Reads from andwrites tothe
TBPRD memory address gototheshadow register. The shadow register contents aretransferred to
theactive register (TBPRD (Active)←TBPRD (shadow)) when thetime-base counter equals zero
(TBCTR =0x0000). Bydefault theTBPRD shadow register isenabled.
*Time-Base Period Immediate Load Mode:
Ifimmediate load mode isselected (TBCTL[PRDLD] =1),then aread from orawrite totheTBPRD
memory address goes directly totheactive register.
35.2.2.3.2 Time-Base Clock Synchronization
Bit1ofthedevice-level multiplexing control module (IOMM) register PINMMR166 isdefined asthe
TBCLKSYNC bit.The TBCLKSYNC bitallows users toglobally synchronize allenabled ePWM modules to
thetime-base clock (TBCLK). When set,allenabled ePWM module clocks arestarted with thefirstrising
edge ofTBCLK aligned. Forperfectly synchronized TBCLKs, theprescalers foreach ePWM module must
besetidentically.

<!-- Page 2006 -->

EPWM2SYNCI
ePWM2
EPWM2SYNCOEPWM1SYNCOePWM1EPWM1SYNCI
GPIO
MUX
EPWM3SYNCOePWM3EPWM3SYNCI
ePWMxEPWMxSYNCISYNCI
eCAP1
EPWMxSYNCO
ePWM Submodules www.ti.com
2006 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleThe proper procedure forenabling ePWM clocks isasfollows:
1.Enable ePWM module clocks using theIOMM control registers foreach ePWM module instance
2.SetTBCLKSYNC =0.This willstop thetime-base clock within anyenabled ePWM module.
3.Configure ePWM modules: prescaler values andePWM modes.
4.SetTBCLKSYNC =1.
35.2.2.3.3 Time-Base Counter Synchronization
Atime-base synchronization scheme connects alloftheePWM modules onadevice. Each ePWM
module hasasynchronization input (EPWMxSYNCI) andasynchronization output (EPWMxSYNCO). The
input synchronization forthefirstinstance (ePWM1) comes from anexternal pin.The synchronization
connections fortheremaining ePWM modules areshown inFigure 35-6.
Figure 35-6. Time-Base Counter Synchronization Scheme

<!-- Page 2007 -->

www.ti.com ePWM Submodules
2007 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleEach ePWM module canbeconfigured touseorignore thesynchronization input. IftheTBCTL[PHSEN]
bitisset,then thetime-base counter (TBCTR) oftheePWM module willbeautomatically loaded with the
phase register (TBPHS) contents when oneofthefollowing conditions occur:
*EPWMxSYNCI: Synchronization Input Pulse:
The value ofthephase register isloaded intothecounter register when aninput synchronization pulse
isdetected (TBPHS →TBCTR). This operation occurs onthenext valid time-base clock (TBCLK)
edge.
The delay from internal master module toslave modules isgiven by:
-if(TBCLK =VCLK3): 2xVCLK3
-if(TBCLK !=VCLK3): 1TBCLK
*Software Forced Synchronization Pulse:
Writing a1totheTBCTL[SWFSYNC] control bitinvokes asoftware forced synchronization. This pulse
isORed with thesynchronization input signal, andtherefore hasthesame effect asapulse on
EPWMxSYNCI.
*Digital Compare Event Synchronization Pulse:
DCAEVT1 andDCBEVT1 digital compare events canbeconfigured togenerate synchronization
pulses which have thesame affect asEPWMxSYNCI.
This feature enables theePWM module tobeautomatically synchronized tothetime base ofanother
ePWM module. Lead orlagphase control canbeadded tothewaveforms generated bydifferent ePWM
modules tosynchronize them. Inup-down-count mode, theTBCTL[PSHDIR] bitconfigures thedirection of
thetime-base counter immediately after asynchronization event. The new direction isindependent ofthe
direction prior tothesynchronization event. The PHSDIR bitisignored incount-up orcount-down modes.
See Figure 35-7 through Figure 35-10 forexamples.
Clearing theTBCTL[PHSEN] bitconfigures theePWM toignore thesynchronization input pulse. The
synchronization pulse canstillbeallowed toflow-through totheEPWMxSYNCO andbeused to
synchronize other ePWM modules. Inthisway, youcansetupamaster time-base (forexample, ePWM1)
anddownstream modules (ePWM2 -ePWMx) may elect toruninsynchronization with themaster.
35.2.2.4 Phase Locking theTime-Base Clocks ofMultiple ePWM Modules
The TBCLKSYNC bitcanbeused toglobally synchronize thetime-base clocks ofallenabled ePWM
modules onadevice. When TBCLKSYNC =0,thetime-base clock ofallePWM modules isstopped
(default). When TBCLKSYNC =1,allePWM time-base clocks arestarted with therising edge ofTBCLK
aligned. Forperfectly synchronized TBCLKs, theprescaler bitsintheTBCTL register ofeach ePWM
module must besetidentically. The proper procedure forenabling theePWM clocks isasfollows:
1.Enable ePWM module clocks using theIOMM control registers foreach ePWM module instance
2.SetTBCLKSYNC= 0.This willstop thetime-base clock within anyenabled ePWM module.
3.Configure ePWM modules: prescaler values andePWM modes.
4.SetTBCLKSYNC=1.
35.2.2.5 Time-base Counter Modes andTiming Waveforms
The time-base counter operates inoneoffour modes:
*Up-count mode which isasymmetrical.
*Down-count mode which isasymmetrical.
*Up-down-count which issymmetrical
*Frozen where thetime-base counter isheld constant atthecurrent value
Toillustrate theoperation ofthefirstthree modes, thefollowing timing diagrams show when events are
generated andhow thetime-base responds toanEPWMxSYNCI signal.

<!-- Page 2008 -->

0000
EPWMxSYNCITBCTR[15:0]
CTR_dir
CTR = zero
CNT_maxCTR = PRD0xFFFF
TBPHS
(value)TBPRD
(value)
ePWM Submodules www.ti.com
2008 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-7. Time-Base Up-Count Mode Waveforms

<!-- Page 2009 -->

0x00000xFFFFTBCTR[15:0]
UP
DOWNUP
DOWNUP
DOWNUPTBPHS
(value)TBPRD
(value)
EPWMxSYNCI
CTR_dir
CTR□=□zero
CNT_maxCTR□=□PRD
0x0000xFFFFTBCTR[15:0]
TBPHS(value)TBPRD(value)
EPWMxSYNCI
CTR_dir
CTR = zero
CNT_maxCTR = PRD
www.ti.com ePWM Submodules
2009 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-8. Time-Base Down-Count Mode Waveforms
Figure 35-9. Time-Base Up-Down-Count Waveforms, TBCTL[PHSDIR =0]Count Down On
Synchronization Event

<!-- Page 2010 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
0x00000xFFFFTBCTR[15:0]
UP
DOWNUP
DOWNUP
DOWNTBPHS□(value)TBPRD□(value)
EPWMxSYNCI
CTR_dir
CTR□=□zero
CNT_maxCTR□=□PRD
ePWM Submodules www.ti.com
2010 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-10. Time-Base Up-Down Count Waveforms, TBCTL[PHSDIR =1]Count UpOn
Synchronization Event
35.2.3 Counter-Compare (CC) Submodule
Figure 35-11 illustrates thecounter-compare submodule within theePWM.
Figure 35-12 shows thebasic structure ofthecounter-compare submodule.
Figure 35-11. Counter-Compare Submodule

<!-- Page 2011 -->

TBCTR[15:0] Time
Base
(TB)
Module
16CMPA[15:0]
161616
CMPA
Compare□A□Active□Reg.CTR□=□CMPA
CTR□=□CMPBAction
Qualifier
Module
Digital
comparator□BCMPB[15:0]TBCTR[15:0]CTR□=□PRD
CTR□=0
CMPCTL[LOADAMODE]
Shadow
loadCTR□=□PRD
CTR□=□0
CMPCTL[LOADBMODE]CMPCTL[SHDWBFULL]
CMPCTL[SHDWBMODE]CMPCTL
[SHDWAFULL]
CMPCTL
[SHDWAMODE] CMPA
Compare□A□Shadow□Reg.Digital
comparator□A
CMPB
Compare□B□Active□Reg.
CMPB
Compare□B□Shadow□Reg.Shadow
load
(AQ)
www.ti.com ePWM Submodules
2011 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-12. Detailed View oftheCounter-Compare Submodule
35.2.3.1 Purpose oftheCounter-Compare Submodule
The counter-compare submodule takes asinput thetime-base counter value. This value iscontinuously
compared tothecounter-compare A(CMPA) andcounter-compare B(CMPB) registers. When thetime-
base counter isequal tooneofthecompare registers, thecounter-compare unitgenerates anappropriate
event.
The counter-compare:
*Generates events based onprogrammable time stamps using theCMPA andCMPB registers
-CTR =CMPA: Time-base counter equals counter-compare Aregister (TBCTR =CMPA).
-CTR =CMPB: Time-base counter equals counter-compare Bregister (TBCTR =CMPB)
*Controls thePWM duty cycle iftheaction-qualifier submodule isconfigured appropriately
*Shadows new compare values toprevent corruption orglitches during theactive PWM cycle
35.2.3.2 Controlling andMonitoring theCounter-Compare Submodule
The counter-compare submodule operation iscontrolled andmonitored bytheregisters listed inTable 35-
5.
The keysignals associated with thecounter-compare submodule aredescribed inTable 35-6.
Table 35-5. Counter-Compare Submodule Registers
Register Name Address Offset Shadowed Description
CMPCTL 0Ch No Counter-Compare Control Register.
CMPA 10h Yes Counter-Compare ARegister
CMPB 16h Yes Counter-Compare BRegister

<!-- Page 2012 -->

ePWM Submodules www.ti.com
2012 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-6. Counter-Compare Submodule KeySignals
Signal Description ofEvent Registers Compared
CTR =CMPA Time-base counter equal totheactive counter-compare Avalue TBCTR =CMPA
CTR =CMPB Time-base counter equal totheactive counter-compare Bvalue TBCTR =CMPB
CTR =PRD Time-base counter equal totheactive period.
Used toload active counter-compare AandBregisters from the
shadow registerTBCTR =TBPRD
CTR =ZERO Time-base counter equal tozero.
Used toload active counter-compare AandBregisters from the
shadow registerTBCTR =0x0000
35.2.3.3 Operational Highlights fortheCounter-Compare Submodule
The counter-compare submodule isresponsible forgenerating twoindependent compare events based on
twocompare registers:
1.CTR =CMPA: Time-base counter equal tocounter-compare Aregister (TBCTR =CMPA).
2.CTR =CMPB: Time-base counter equal tocounter-compare Bregister (TBCTR =CMPB).
Forup-count ordown-count mode, each event occurs only once percycle. Forup-down-count mode each
event occurs twice percycle ifthecompare value isbetween 0x0000-TBPRD andonce percycle ifthe
compare value isequal to0x0000 orequal toTBPRD. These events arefedintotheaction-qualifier
submodule where they arequalified bythecounter direction andconverted intoactions ifenabled. Refer
toSection 35.2.4.1 formore details.
The counter-compare registers CMPA andCMPB each have anassociated shadow register. Shadowing
provides away tokeep updates totheregisters synchronized with thehardware. When shadowing is
used, updates totheactive registers only occur atstrategic points. This prevents corruption orspurious
operation duetotheregister being asynchronously modified bysoftware. The memory address ofthe
active register andtheshadow register isidentical. Which register iswritten toorread from isdetermined
bytheCMPCTL[SHDWAMODE] andCMPCTL[SHDWBMODE] bits. These bitsenable anddisable the
CMPA shadow register andCMPB shadow register respectively. The behavior ofthetwoload modes is
described below:
Shadow Mode:
The shadow mode fortheCMPA isenabled byclearing theCMPCTL[SHDWAMODE] bitandtheshadow
register forCMPB isenabled byclearing theCMPCTL[SHDWBMODE] bit.Shadow mode isenabled by
default forboth CMPA andCMPB.
Iftheshadow register isenabled then thecontent oftheshadow register istransferred totheactive
register ononeofthefollowing events asspecified bytheCMPCTL[LOADAMODE] and
CMPCTL[LOADBMODE] register bits:
*CTR =PRD: Time-base counter equal totheperiod (TBCTR =TBPRD).
*CTR =Zero: Time-base counter equal tozero (TBCTR =0x0000)
*Both CTR =PRD andCTR =Zero
Only theactive register contents areused bythecounter-compare submodule togenerate events tobe
sent totheaction-qualifier.
Immediate Load Mode:
Ifimmediate load mode isselected (TBCTL[SHADWAMODE] =1orTBCTL[SHADWBMODE] =1),then a
read from orawrite totheregister willgodirectly totheactive register.
35.2.3.4 Count Mode Timing Waveforms
The counter-compare module cangenerate compare events inallthree count modes:
*Up-count mode: used togenerate anasymmetrical PWM waveform.
*Down-count mode: used togenerate anasymmetrical PWM waveform.

<!-- Page 2013 -->

TBCTR[15:0]
0x00000xFFFF
CTR□=□CMPACMPA
(value)
CMPB
(value)
TBPHS
(value)TBPRD
(value)
CTR□=□CMPBEPWMxSYNCI
0x00000xFFFF
CTR□=□CMPATBCTR[15:0]
CMPA
(value)
CMPB
(value)
TBPHS
(value)TBPRD
(value)
CTR□=□CMPBEPWMxSYNCI
www.ti.com ePWM Submodules
2013 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module*Up-down-count mode: used togenerate asymmetrical PWM waveform.
Toillustrate theoperation ofthefirstthree modes, thetiming diagrams inFigure 35-13 through Figure 35-
16show when events aregenerated andhow theEPWMxSYNCI signal interacts.
Figure 35-13. Counter-Compare Event Waveforms inUp-Count Mode
NOTE: AnEPWMxSYNCI external synchronization event cancause adiscontinuity intheTBCTR count
sequence. This canlead toacompare event being skipped. This skipping isconsidered normal operation and
must betaken intoaccount.
Figure 35-14. Counter-Compare Events inDown-Count Mode

<!-- Page 2014 -->

0x00000xFFFFTBCTR[15:0]
CMPA
(value)
CMPB
(value)
TBPHS
(value)TBPRD
(value)
CTR = CMP ACTR = CMPBEPWMxSYNCI
0x00000xFFFFTBCTR[15:0]
CTR = CMP ACMPA (value)
CMPB (value)
TBPHS (value)TBPRD (value)
CTR = CMPBEPWMxSYNCI
ePWM Submodules www.ti.com
2014 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-15. Counter-Compare Events InUp-Down-Count Mode, TBCTL[PHSDIR =0]Count Down On
Synchronization Event
Figure 35-16. Counter-Compare Events InUp-Down-Count Mode, TBCTL[PHSDIR =1]Count UpOn
Synchronization Event

<!-- Page 2015 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
www.ti.com ePWM Submodules
2015 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.4 Action-Qualifier (AQ) Submodule
Figure 35-17 shows theaction-qualifier (AQ) submodule (see shaded block) intheePWM system.
The action-qualifier submodule hasthemost important roleinwaveform construction andPWM
generation. Itdecides which events areconverted intovarious action types, thereby producing the
required switched waveforms attheEPWMxA andEPWMxB outputs.
Figure 35-17. Action-Qualifier Submodule
35.2.4.1 Purpose oftheAction-Qualifier Submodule
The action-qualifier submodule isresponsible forthefollowing:
*Qualifying andgenerating actions (set, clear, toggle) based onthefollowing events:
-CTR =PRD: Time-base counter equal totheperiod (TBCTR =TBPRD).
-CTR =Zero: Time-base counter equal tozero (TBCTR =0x0000)
-CTR =CMPA: Time-base counter equal tothecounter-compare Aregister (TBCTR =CMPA)
-CTR =CMPB: Time-base counter equal tothecounter-compare Bregister (TBCTR =CMPB)
*Managing priority when these events occur concurrently
*Providing independent control ofevents when thetime-base counter isincreasing andwhen itis
decreasing.
35.2.4.2 Action-Qualifier Submodule Control andStatus Register Definitions
The action-qualifier submodule operation iscontrolled andmonitored viatheregisters inTable 35-7.
Table 35-7. Action-Qualifier Submodule Registers
Register Name Address Offset Shadowed Description
AQCTLA 14h No Action-Qualifier Control Register ForOutput A(EPWMxA)
AQSFRC 18h No Action-Qualifier Software Force Register
AQCTLB 1Ah No Action-Qualifier Control Register ForOutput B(EPWMxB)
AQCSFRC 1Eh Yes Action-Qualifier Continuous Software Force

<!-- Page 2016 -->

Action-qualifier□(AQ)□Module
AQCTLA[15:0]
Action-qualifier□control□AEPWMA
EPWMBTBCLK
CTR□=□PRD
CTR□=□Zero
CTR□=□CMPA
CTR□=□CMPB
CTR_dirAQCTLB[15:0]
Action-qualifier□control□B
AQSFRC[15:0]
Action-qualifier□S/W□force
AQCSFRC[3:0]□(shadow)
continuous□S/W□force
AQCSFRC[3:0]□(active)
continuous□S/W□force
ePWM Submodules www.ti.com
2016 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleThe action-qualifier submodule isbased onevent-driven logic. Itcanbethought ofasaprogrammable
cross switch with events attheinput andactions attheoutput, allofwhich aresoftware controlled viathe
setofregisters shown inTable 35-7 .
Figure 35-18. Action-Qualifier Submodule Inputs andOutputs
The possible input events aresummarized again inTable 35-8.
Table 35-8. Action-Qualifier Submodule Possible Input Events
Signal Description Registers Compared
CTR =PRD Time-base counter equal totheperiod value TBCTR =TBPRD
CTR =Zero Time-base counter equal tozero TBCTR =0x0000
CTR =CMPA Time-base counter equal tothecounter-compare A TBCTR =CMPA
CTR =CMPB Time-base counter equal tothecounter-compare B TBCTR =CMPB
Software forced event Asynchronous event initiated bysoftware
The software forced action isauseful asynchronous event. This control ishandled byregisters AQSFRC
andAQCSFRC.
The action-qualifier submodule controls how thetwooutputs EPWMxA andEPWMxB behave when a
particular event occurs. The event inputs totheaction-qualifier submodule arefurther qualified bythe
counter direction (upordown). This allows forindependent action onoutputs onboth thecount-up and
count-down phases.
The possible actions imposed onoutputs EPWMxA andEPWMxB are:
*SetHigh:
Setoutput EPWMxA orEPWMxB toahigh level.
*Clear Low:
Setoutput EPWMxA orEPWMxB toalowlevel.
*Toggle:
IfEPWMxA orEPWMxB iscurrently pulled high, then pulltheoutput low. IfEPWMxA orEPWMxB is
currently pulled low, then pulltheoutput high.

<!-- Page 2017 -->

Z
Z
ZCA
CA
CA
Z
TCB
TP
TCA
TCB PCBCB PDo Nothing
Clear Low
Set High
TogglePZeroComp
AComp
BPeriodTB Counter equals:ActionsS/W
force
SW
SW
SW
SW
T
www.ti.com ePWM Submodules
2017 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module*DoNothing:
Keep outputs EPWMxA andEPWMxB atsame level ascurrently set.Although the"DoNothing" option
prevents anevent from causing anaction ontheEPWMxA andEPWMxB outputs, thisevent canstill
trigger interrupts andADC start ofconversion. See theEvent-trigger Submodule description in
Section 35.2.8 fordetails.
Actions arespecified independently foreither output (EPWMxA orEPWMxB). Any orallevents canbe
configured togenerate actions onagiven output. Forexample, both CTR =CMPA andCTR =CMPB can
operate onoutput EPWMxA. Allqualifier actions areconfigured viathecontrol registers found attheend
ofthissection.
Forclarity, thedrawings inthisdocument useasetofsymbolic actions. These symbols aresummarized in
Figure 35-19 .Each symbol represents anaction asamarker intime. Some actions arefixed intime (zero
andperiod) while theCMPA andCMPB actions aremoveable andtheir time positions areprogrammed
viathecounter-compare AandBregisters, respectively. Toturn offordisable anaction, usethe"Do
Nothing option"; itisthedefault atreset.
Figure 35-19. Possible Action-Qualifier Actions forEPWMxA andEPWMxB Outputs

<!-- Page 2018 -->

ePWM Submodules www.ti.com
2018 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.4.3 Action-Qualifier Event Priority
Itispossible fortheePWM action qualifier toreceive more than oneevent atthesame time. Inthiscase
events areassigned apriority bythehardware. The general ruleisevents occurring later intime have a
higher priority andsoftware forced events always have thehighest priority. The event priority levels forup-
down-count mode areshown inTable 35-9.Apriority level of1isthehighest priority andlevel 7isthe
lowest. The priority changes slightly depending onthedirection ofTBCTR.
Table 35-9. Action-Qualifier Event Priority forUp-Down-Count Mode
Priority Level Event IfTBCTR isIncrementing
TBCTR =Zero uptoTBCTR =TBPRDEvent IfTBCTR isDecrementing
TBCTR =TBPRD down toTBCTR =1
1(Highest) Software forced event Software forced event
2 Counter equals CMPB onup-count (CBU) Counter equals CMPB ondown-count (CBD)
3 Counter equals CMPA onup-count (CAU) Counter equals CMPA ondown-count (CAD)
4 Counter equals zero Counter equals period (TBPRD)
5 Counter equals CMPB ondown-count (CBD) Counter equals CMPB onup-count (CBU)
6(Lowest) Counter equals CMPA ondown-count (CAD) Counter equals CMPA onup-count (CBU)
Table 35-10 shows theaction-qualifier priority forup-count mode. Inthiscase, thecounter direction is
always defined asupandthus down-count events willnever betaken.
Table 35-10. Action-Qualifier Event Priority forUp-Count Mode
Priority Level Event
1(Highest) Software forced event
2 Counter equal toperiod (TBPRD)
3 Counter equal toCMPB onup-count (CBU)
4 Counter equal toCMPA onup-count (CAU)
5(Lowest) Counter equal toZero
Table 35-11 shows theaction-qualifier priority fordown-count mode. Inthiscase, thecounter direction is
always defined asdown andthus up-count events willnever betaken.
Table 35-11. Action-Qualifier Event Priority forDown-Count Mode
Priority Level Event
1(Highest) Software forced event
2 Counter equal toZero
3 Counter equal toCMPB ondown-count (CBD)
4 Counter equal toCMPA ondown-count (CAD)
5(Lowest) Counter equal toperiod (TBPRD)
Itispossible tosetthecompare value greater than theperiod. Inthiscase theaction willtake place as
shown inTable 35-12 .
Table 35-12. Behavior ifCMPA/CMPB isGreater than thePeriod
Counter Mode Compare onUp-Count Event
CAD/CBDCompare onDown-Count Event
CAD/CBD
Up-Count Mode IfCMPA/CMPB ≤TBPRD period, then theevent
occurs onacompare match (TBCTR=CMPA or
CMPB).Never occurs.
IfCMPA/CMPB >TBPRD, then theevent willnot
occur.

<!-- Page 2019 -->

www.ti.com ePWM Submodules
2019 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-12. Behavior ifCMPA/CMPB isGreater than thePeriod (continued)
Counter Mode Compare onUp-Count Event
CAD/CBDCompare onDown-Count Event
CAD/CBD
Down-Count Mode Never occurs. IfCMPA/CMPB <TBPRD, theevent willoccur ona
compare match (TBCTR=CMPA orCMPB).
IfCMPA/CMPB ≥TBPRD, theevent willoccur ona
period match (TBCTR=TBPRD).
Up-Down-Count
ModeIfCMPA/CMPB <TBPRD andthecounter is
incrementing, theevent occurs onacompare match
(TBCTR=CMPA orCMPB).IfCMPA/CMPB <TBPRD andthecounter is
decrementing, theevent occurs onacompare match
(TBCTR=CMPA orCMPB).
IfCMPA/CMPB is≥TBPRD, theevent willoccur ona
period match (TBCTR =TBPRD).IfCMPA/CMPB ≥TBPRD, theevent occurs ona
period match (TBCTR=TBPRD).
35.2.4.4 Waveforms forCommon Configurations
NOTE: The waveforms inthisdocument show theePWMs behavior forastatic compare register
value. Inarunning system, theactive compare registers (CMPA andCMPB) aretypically
updated from their respective shadow registers once every period. The user specifies when
theupdate willtake place; either when thetime-base counter reaches zero orwhen thetime-
base counter reaches period. There aresome cases when theaction based onthenew
value canbedelayed byoneperiod ortheaction based ontheoldvalue cantake effect for
anextra period. Some PWM configurations avoid thissituation. These include, butarenot
limited to,thefollowing:
Useup-down-count mode togenerate asymmetric PWM:
*Ifyouload CMPA/CMPB onzero, then useCMPA/CMPB values greater
than orequal to1.
*Ifyouload CMPA/CMPB onperiod, then useCMPA/CMPB values less than
orequal toTBPRD-1.
This means there willalways beapulse ofatleast oneTBCLK cycle ina
PWM period which, when very short, tend tobeignored bythesystem.
Useup-down-count mode togenerate anasymmetric PWM:
*Toachieve 50%-0% asymmetric PWM usethefollowing configuration: Load
CMPA/CMPB onperiod andusetheperiod action toclear thePWM anda
compare-up action tosetthePWM. Modulate thecompare value from 0to
TBPRD toachieve 50%-0% PWM duty.
When using up-count mode togenerate anasymmetric PWM:
*Toachieve 0-100% asymmetric PWM usethefollowing configuration: Load
CMPA/CMPB onTBPRD. Use theZero action tosetthePWM anda
compare-up action toclear thePWM. Modulate thecompare value from 0to
TBPRD+1 toachieve 0-100% PWM duty.
See theUsing Enhanced Pulse Width Modulator (ePWM) Module for0-100%
Duty Cycle Control Application Report (literature number SPRAAI1 )
Figure 35-20 shows how asymmetric PWM waveform canbegenerated using theup-down-count mode
oftheTBCTR. Inthismode 0%-100% DCmodulation isachieved byusing equal compare matches onthe
upcount anddown count portions ofthewaveform. Intheexample shown, CMPA isused tomake the
comparison. When thecounter isincrementing theCMPA match willpullthePWM output high. Likewise,
when thecounter isdecrementing thecompare match willpullthePWM signal low. When CMPA =0,the
PWM signal islowfortheentire period giving the0%duty waveform. When CMPA =TBPRD, thePWM
signal ishigh achieving 100% duty.
When using thisconfiguration inpractice, ifyouload CMPA/CMPB onzero, then useCMPA/CMPB values
greater than orequal to1.Ifyouload CMPA/CMPB onperiod, then useCMPA/CMPB values less than or
equal toTBPRD-1. This means there willalways beapulse ofatleast oneTBCLK cycle inaPWM period
which, when very short, tend tobeignored bythesystem.

<!-- Page 2020 -->

UP DOWN UP DOWN2
034
123
12
034
12
03
1
TBCTR
TBCTR□Direction
EPWMxA/EPWMxB
Case□2:
CMPA =□3,□25%□Duty
Case□3:
CMPA =□2,□50%□Duty
Case□3:
CMPA =□1,□75%□Duty
Case□4:
CMPA =□0,□100%□DutyCase□1:
CMPA =□4,□□0%□Duty
EPWMxA/EPWMxB
EPWMxA/EPWMxB
EPWMxA/EPWMxB
EPWMxA/EPWMxBMode:□Up-Down□Count
TBPRD□=□4
CAU□=□SET,□CAD□=□CLEAR
0%□-□100%□Duty
ePWM Submodules www.ti.com
2020 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-20. Up-Down-Count Mode Symmetrical Waveform
The PWM waveforms inFigure 35-21 through Figure 35-26 show some common action-qualifier
configurations. The C-code samples inExample 35-1 through Example 35-6 shows how toconfigure an
ePWM module foreach case. Some conventions used inthefigures andexamples areasfollows:
*TBPRD, CMPA, andCMPB refer tothevalue written intheir respective registers. The active register,
nottheshadow register, isused bythehardware.
*CMPx, refers toeither CMPA orCMPB.
*EPWMxA andEPWMxB refer totheoutput signals from ePWMx
*Up-Down means Count-up-and-down mode, Upmeans up-count mode andDwn means down-count
mode
*Sym =Symmetric, Asym =Asymmetric
Example 35-1 contains acode sample showing initialization andruntime forthewaveforms inFigure 35-
21.

<!-- Page 2021 -->

TBCTR
EPWMxA
EPWMxBTBPRD
value
CA Z P CB Z P CB CA Z P
Z P CA Z P CA Z P CB CB
www.ti.com ePWM Submodules
2021 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-21. Up,Single Edge Asymmetric Waveform, With Independent Modulation onEPWMxA and
EPWMxB --Active High
A PWM period =(TBPRD +1)×TTBCLK
B Duty modulation forEPWMxA issetbyCMPA, andisactive high (that is,high time duty proportional toCMPA).
C Duty modulation forEPWMxB issetbyCMPB andisactive high (that is,high time duty proportional toCMPB).
D The "DoNothing "actions (X)areshown forcompleteness, butwillnotbeshown onsubsequent diagrams.
E Actions atzero andperiod, although appearing tooccur concurrently, areactually separated byoneTBCLK period.
TBCTR wraps from period to0000.
Example 35-1. Code Sample forFigure 35-21
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; //Period =601 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =350; //Compare A=350 TBCLK counts
EPwm1Regs.CMPB =200; //Compare B=200 TBCLK counts
EPwm1Regs.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTR =0; //clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP;
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; //TBCLK =SYSCLK
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.AQCTLA.bit.ZRO =AQ_SET;
EPwm1Regs.AQCTLA.bit.CAU =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.ZRO =AQ_SET;
EPwm1Regs.AQCTLB.bit.CBU =AQ_CLEAR;
//
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =Duty1A; //adjust duty for output EPWM1A
EPwm1Regs.CMPB =Duty1B; //adjust duty for output EPWM1B

<!-- Page 2022 -->

TBCTR
EPWMxA
EPWMxBTBPRD
value
CBCA P
P PP
CBCA
PP
ePWM Submodules www.ti.com
2022 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-22. Up,Single Edge Asymmetric Waveform With Independent Modulation onEPWMxA and
EPWMxB --Active Low
A PWM period =(TBPRD +1)×TTBCLK
B Duty modulation forEPWMxA issetbyCMPA, andisactive low(that is,thelowtime duty isproportional toCMPA).
C Duty modulation forEPWMxB issetbyCMPB andisactive low(that is,thelowtime duty isproportional toCMPB).
D Actions atzero andperiod, although appearing tooccur concurrently, areactually separated byoneTBCLK period.
TBCTR wraps from period to0000.
Example 35-2 contains acode sample showing initialization andruntime forthewaveforms inFigure 35-
22.

<!-- Page 2023 -->

TBCTR
EPWMxA
EPWMxBTBPRD
value
CA
TZ
TZ
TZCA CB CB
www.ti.com ePWM Submodules
2023 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-2. Code Sample forFigure 35-22
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; //Period =601 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =350; //Compare A=350 TBCLK counts
EPwm1Regs.CMPB =200; //Compare B=200 TBCLK counts
EPwm1Regs.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTR =0; //clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP;
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; //TBCLK =VCLK3
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onTBCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onTBCTR =Zero
EPwm1Regs.AQCTLA.bit.PRD =AQ_CLEAR;
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
EPwm1Regs.AQCTLB.bit.PRD =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBU =AQ_SET;
//
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =Duty1A; //adjust duty for output EPWM1A
EPwm1Regs.CMPB =Duty1B; //adjust duty for output EPWM1B
Figure 35-23. Up-Count, Pulse Placement Asymmetric Waveform With Independent Modulation on
EPWMxA
A PWM frequency =1/((TBPRD +1)×TTBCLK )
B Pulse canbeplaced anywhere within thePWM cycle (0000 -TBPRD)
C High time duty proportional to(CMPB -CMPA)
D EPWMxB canbeused togenerate a50% duty square wave with frequency =1/2×((TBPRD +1)×TBCLK )
Example 35-3 contains acode sample showing initialization andruntime forthewaveforms Figure 35-23 .

<!-- Page 2024 -->

ePWM Submodules www.ti.com
2024 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-3. Code Sample forFigure 35-23
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; //Period =601 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =200; //Compare A=200 TBCLK counts
EPwm1Regs.CMPB =400; //Compare B=400 TBCLK counts
EPwm1Regs.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTR =0; //clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP;
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; //TBCLK =VCLK3
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onTBCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onTBCTR =Zero
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
EPwm1Regs.AQCTLA.bit.CBU =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.ZRO =AQ_TOGGLE;
//
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =EdgePosA; //adjust duty for output EPWM1A only
EPwm1Regs.CMPB =EdgePosB;

<!-- Page 2025 -->

TBCTR
EPWMxA
EPWMxBTBPRD
value
CA CA CA CA
CBCB CB CB CB
www.ti.com ePWM Submodules
2025 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-24. Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
EPWMxA andEPWMxB --Active Low
A PWM period =2xTBPRD ×TTBCLK
B Duty modulation forEPWMxA issetbyCMPA, andisactive low(that is,thelowtime duty isproportional toCMPA).
C Duty modulation forEPWMxB issetbyCMPB andisactive low(that is,thelowtime duty isproportional toCMPB).
D Outputs EPWMxA andEPWMxB candrive independent power switches
Example 35-4 contains acode sample showing initialization andruntime forthewaveforms inFigure 35-
24.
Example 35-4. Code Sample forFigure 35-24
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; //Period =2´600 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =400; //Compare A=400 TBCLK counts
EPwm1Regs.CMPB =500; //Compare B=500 TBCLK counts
EPwm1Regs.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTR =0; //clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetric
xEPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
xEPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; //TBCLK =VCLK3
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
EPwm1Regs.AQCTLA.bit.CAD =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBU =AQ_SET;
EPwm1Regs.AQCTLB.bit.CBD =AQ_CLEAR;
//
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =Duty1A; //adjust duty for output EPWM1A
EPwm1Regs.CMPB =Duty1B; //adjust duty for output EPWM1B

<!-- Page 2026 -->

CA CA CA CA
CB CB CB CBTBCTR
EPWMxA
EPWMxBTBPRD
value
ePWM Submodules www.ti.com
2026 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-25. Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
EPWMxA andEPWMxB --Complementary
A PWM period =2×TBPRD ×TTBCLK
B Duty modulation forEPWMxA issetbyCMPA, andisactive low, i.e.,lowtime duty proportional toCMPA
C Duty modulation forEPWMxB issetbyCMPB andisactive high, i.e.,high time duty proportional toCMPB
D Outputs EPWMx candrive upper/lower (complementary) power switches
E Dead-band =CMPB -CMPA (fully programmable edge placement bysoftware). Note thedead-band module isalso
available ifthemore classical edge delay method isrequired.
Example 35-5 contains acode sample showing initialization andruntime forthewaveforms inFigure 35-
25.
Example 35-5. Code Sample forFigure 35-25
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; //Period =2´600 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =350; //Compare A=350 TBCLK counts
EPwm1Regs.CMPB =400; //Compare B=400 TBCLK counts
EPwm1Regs.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTR =0; //clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetric
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; //TBCLK =VCLK3
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR =Zero
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
EPwm1Regs.AQCTLA.bit.CAD =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBU =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBD =AQ_SET;
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =Duty1A; //adjust duty for output EPWM1A
EPwm1Regs.CMPB =Duty1B; //adjust duty for output EPWM1B

<!-- Page 2027 -->

Z P Z PTBCTR
EPWMxA
EPWMxBCA CA CB CB
www.ti.com ePWM Submodules
2027 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-26. Up-Down-Count, Dual Edge Asymmetric Waveform, With Independent Modulation on
EPWMxA --Active Low
A PWM period =2×TBPRD ×TBCLK
B Rising edge andfalling edge canbeasymmetrically positioned within aPWM cycle. This allows forpulse placement
techniques.
C Duty modulation forEPWMxA issetbyCMPA andCMPB.
D Low time duty forEPWMxA isproportional to(CMPA +CMPB).
E Tochange thisexample toactive high, CMPA andCMPB actions need tobeinverted (i.e., Set!Clear andClear Set).
F Duty modulation forEPWMxB isfixed at50% (utilizes spare action resources forEPWMxB)
Example 35-6 contains acode sample showing initialization andruntime forthewaveforms inFigure 35-
26.
Example 35-6. Code Sample forFigure 35-26
//Initialization Time
//========================
EPwm1Regs.TBPRD =600; // Period =2´600 TBCLK counts
EPwm1Regs.CMPA.half.CMPA =250; // Compare A=250 TBCLK counts
EPwm1Regs.CMPB =450; // Compare B=450 TBCLK counts
EPwm1Regs.TBPHS =0; // Set Phase register tozero
EPwm1Regs.TBCTR =0; // clear TBcounter
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; // Symmetric
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; // Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.TBCTL.bit.HSPCLKDIV =TB_DIV1; // TBCLK =VCLK3
EPwm1Regs.TBCTL.bit.CLKDIV =TB_DIV1;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; // load onCTR =Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; // load onCTR =Zero
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
EPwm1Regs.AQCTLA.bit.CBD =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.ZRO =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.PRD =AQ_SET;
//Run Time
//========================
EPwm1Regs.CMPA.half.CMPA =EdgePosA; //adjust duty for output EPWM1A only
EPwm1Regs.CMPB =EdgePosB;

<!-- Page 2028 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
ePWM Submodules www.ti.com
2028 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.5 Dead-Band Generator (DB) Submodule
Figure 35-27 illustrates thedead-band submodule within theePWM module.
Figure 35-27. Dead_Band Submodule
35.2.5.1 Purpose oftheDead-Band Submodule
The "Action-qualifier (AQ) Module" section discussed how itispossible togenerate therequired dead-
band byhaving fullcontrol over edge placement using both theCMPA andCMPB resources oftheePWM
module. However, ifthemore classical edge delay-based dead-band with polarity control isrequired, then
thedead-band submodule described here should beused.
The keyfunctions ofthedead-band module are:
*Generating appropriate signal pairs (EPWMxA andEPWMxB) with dead-band relationship from a
single EPWMxA input
*Programming signal pairs for:
-Active high (AH)
-Active low(AL)
-Active high complementary (AHC)
-Active lowcomplementary (ALC)
*Adding programmable delay torising edges (RED)
*Adding programmable delay tofalling edges (FED)
*Can betotally bypassed from thesignal path (note dotted lines indiagram)
35.2.5.2 Controlling andMonitoring theDead-Band Submodule
The dead-band submodule operation iscontrolled andmonitored viathefollowing registers:
Table 35-13. Dead-Band Generator Submodule Registers
Register Name Address Offset Shadowed Description
DBCTL 1Ch No Dead-Band Control Register
DBFED 20h No Dead-Band Falling Edge Delay Count Register
DBRED 22h No Dead-Band Rising Edge Delay Count Register

<!-- Page 2029 -->

0
1S2
10S1
RED
Out InRising□edge
delay
(10-bit
counter)
(10-bit
counter)delayFalling□edge
In OutFED
10S3
0S01EPWMxA
EPWMxB
DBCTL[POLSEL] DBCTL[OUT_MODE]S5
DBCTL[IN_MODE]10S40
1EPWMxA in
EPWMxB□inDBCTL[HALFCYCLE]
www.ti.com ePWM Submodules
2029 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.5.3 Operational Highlights fortheDead-Band Submodule
The following sections provide theoperational highlights.
The dead-band submodule hastwogroups ofindependent selection options asshown inFigure 35-28 .
*Input Source Selection:
The input signals tothedead-band module aretheEPWMxA andEPWMxB output signals from the
action-qualifier. Inthissection they willbereferred toasEPWMxA InandEPWMxB In.Using the
DBCTL[IN_MODE) control bits, thesignal source foreach delay, falling-edge orrising-edge, canbe
selected:
-EPWMxA Inisthesource forboth falling-edge andrising-edge delay. This isthedefault mode.
-EPWMxA Inisthesource forfalling-edge delay, EPWMxB Inisthesource forrising-edge delay.
-EPWMxA Inisthesource forrising edge delay, EPWMxB Inisthesource forfalling-edge delay.
-EPWMxB Inisthesource forboth falling-edge andrising-edge delay.
*Half Cycle Clocking:
The dead-band submodule canbeclocked using halfcycle clocking todouble theresolution (i.e.
counter clocked at2×TBCLK)
*Output Mode Control:
The output mode isconfigured byway oftheDBCTL[OUT_MODE] bits. These bitsdetermine ifthe
falling-edge delay, rising-edge delay, neither, orboth areapplied totheinput signals.
*Polarity Control:
The polarity control (DBCTL[POLSEL]) allows youtospecify whether therising-edge delayed signal
and/or thefalling-edge delayed signal istobeinverted before being sent outofthedead-band
submodule.
Figure 35-28. Configuration Options fortheDead-Band Submodule

<!-- Page 2030 -->

ePWM Submodules www.ti.com
2030 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleAlthough allcombinations aresupported, notallaretypical usage modes. Table 35-14 documents some
classical dead-band configurations. These modes assume thattheDBCTL[IN_MODE] isconfigured such
thatEPWMxA Inisthesource forboth falling-edge andrising-edge delay. Enhanced, ornon-traditional
modes canbeachieved bychanging theinput signal source. The modes shown inTable 35-14 fallinto
thefollowing categories:
*Mode 1:Bypass both falling-edge delay (FED) andrising-edge delay (RED)
Allows youtofully disable thedead-band submodule from thePWM signal path.
*Mode 2-5:Classical Dead-Band Polarity Settings:
These represent typical polarity configurations thatshould address alltheactive high/low modes
required byavailable industry power switch gate drivers. The waveforms forthese typical cases are
shown inFigure 35-29 .Note thattogenerate equivalent waveforms toFigure 35-29 ,configure the
action-qualifier submodule togenerate thesignal asshown forEPWMxA.
*Mode 6:Bypass rising-edge-delay andMode 7:Bypass falling-edge-delay
Finally thelasttwoentries inTable 35-14 show combinations where either thefalling-edge-delay (FED)
orrising-edge-delay (RED) blocks arebypassed.
Table 35-14. Classical Dead-Band Operating Modes
Mode Mode DescriptionDBCTL[POLSEL] DBCTL[OUT_MODE]
S3 S2 S1 S0
1 EPWMxA andEPWMxB Passed Through (NoDelay) X X 0 0
2 Active High Complementary (AHC) 1 0 1 1
3 Active Low Complementary (ALC) 0 1 1 1
4 Active High (AH) 0 0 1 1
5 Active Low (AL) 1 1 1 1
6EPWMxA Out=EPWMxA In(NoDelay) 0or1 0or1 0 1
EPWMxB Out=EPWMxA Inwith Falling Edge Delay
7EPWMxA Out=EPWMxA Inwith Rising Edge Delay 0or1 0or1 1 0
EPWMxB Out=EPWMxB Inwith NoDelay

<!-- Page 2031 -->

Original
(outA)
Rising Edge
Delayed (RED)
Falling Edge
Delayed (FED)
Active High
Complementary
(AHC)
Active Low
Complementary
(ALC)
Active High
(AH)
Active Low
(AL)RED
FEDPeriod
www.ti.com ePWM Submodules
2031 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-29 shows waveforms fortypical cases where 0%<duty <100%.
Figure 35-29. Dead-Band Waveforms forTypical Cases (0%<Duty <100%)

<!-- Page 2032 -->

ePWM Submodules www.ti.com
2032 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleThe dead-band submodule supports independent values forrising-edge (RED) andfalling-edge (FED)
delays. The amount ofdelay isprogrammed using theDBRED andDBFED registers. These are10-bit
registers andtheir value represents thenumber oftime-base clock, TBCLK, periods asignal edge is
delayed by.Forexample, theformula tocalculate falling-edge-delay andrising-edge-delay are:
FED =DBFED ×TTBCLK
RED =DBRED ×TTBCLK
Where TTBCLK istheperiod ofTBCLK, theprescaled version ofVCLK3.
Forconvenience, delay values forvarious TBCLK options areshown inTable 35-15 .
Table 35-15. Dead-Band Delay Values inμSasaFunction ofDBFED andDBRED
Dead-Band Value Dead-Band Delay inμS
DBFED, DBRED TBCLK =VCLK3/1 TBCLK =VCLK3 /2 TBCLK =VCLK3/4
1 0.02μS 0.03μS 0.07μS
5 0.08μS 0.17μS 0.33μS
10 0.17μS 0.33μS 0.67μS
100 1.67μS 3.33μS 6.67μS
200 3.33μS 6.67μS 13.33μS
400 6.67μS 13.33μS 26.67μS
500 8.33μS 16.67μS 33.33μS
600 10.00μS 20.00μS 40.00μS
700 11.67μS 23.33μS 46.67μS
800 13.33μS 26.67μS 53.33μS
900 15.00μS 30.00μS 60.00μS
1000 16.67μS 33.33μS 66.67μS
When half-cycle clocking isenabled, theformula tocalculate thefalling-edge-delay andrising-edge-delay
becomes:
FED =DBFED ×TTBCLK/2
RED =DBRED ×TTBCLK/2

<!-- Page 2033 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
www.ti.com ePWM Submodules
2033 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.6 PWM-Chopper (PC) Submodule
Figure 35-30 illustrates thePWM-chopper (PC) submodule within theePWM module.
The PWM-chopper submodule allows ahigh-frequency carrier signal tomodulate thePWM waveform
generated bytheaction-qualifier anddead-band submodules. This capability isimportant ifyouneed
pulse transformer-based gate drivers tocontrol thepower switching elements.
Figure 35-30. PWM-Chopper Submodule
35.2.6.1 Purpose ofthePWM-Chopper Submodule
The keyfunctions ofthePWM-chopper submodule are:
*Programmable chopping (carrier) frequency
*Programmable pulse width offirstpulse
*Programmable duty cycle ofsecond andsubsequent pulses
*Can befully bypassed ifnotrequired
35.2.6.2 Controlling thePWM-Chopper Submodule
The PWM-chopper submodule operation iscontrolled viatheregisters inTable 35-16 .
Table 35-16. PWM-Chopper Submodule Registers
Register Name Address Offset Shadowed Description
PCCTL 3Eh No PWM-chopper Control Register
35.2.6.3 Operational Highlights forthePWM-Chopper Submodule
Figure 35-31 shows theoperational details ofthePWM-chopper submodule. The carrier clock isderived
from VCLK3. Itsfrequency andduty cycle arecontrolled viatheCHPFREQ andCHPDUTY bitsinthe
PCCTL register. The one-shot block isafeature thatprovides ahigh energy firstpulse toensure hard and
fastpower switch turn on,while thesubsequent pulses sustain pulses, ensuring thepower switch remains
on.The one-shot width isprogrammed viatheOSHTWTH bits. The PWM-chopper submodule canbefully
disabled (bypassed) viatheCHPEN bit.

<!-- Page 2034 -->

PSCLKEPWMxA
EPWMxB
EPWMxA
EPWMxB
Start
ClkOne
shot
Pulse-width
PCCTL
[OSHTWTH]PWMA_chBypass
Divider and
duty controlPSCLKOSHTEPWMxA
PCCTL
[CHPEN]EPWMxA
/8 VCLK4
Pulse-width
StartshotClk
OnePCCTL
[OSHTWTH]
1OSHTPCCTL[CHPFREQ]
PCCTL[CHPDUTY]
PWMB_ch
BypassEPWMxB EPWMxB10
0
ePWM Submodules www.ti.com
2034 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-31. PWM-Chopper Submodule Operational Details
35.2.6.4 Waveforms
Figure 35-32 shows simplified waveforms ofthechopping action only; one-shot andduty-cycle control are
notshown. Details oftheone-shot andduty-cycle control arediscussed inthefollowing sections.
Figure 35-32. Simple PWM-Chopper Submodule Waveforms Showing Chopping Action Only

<!-- Page 2035 -->

PSCLK
OSHTEPWMxA in
EPWMxA outProg. pulse width
(OSHTWTH)Start OSHT pulse
Sustaining pulses
www.ti.com ePWM Submodules
2035 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.6.4.1 One-Shot Pulse
The width ofthefirstpulse canbeprogrammed toanyof16possible pulse width values. The width or
period ofthefirstpulse isgiven by:
T1stpulse =TVCLK3 ×8×OSHTWTH
Where TVCLK3 istheperiod ofthesystem clock (VCLK3) andOSHTWTH isthefour control bits(value from
1to16)
Figure 35-33 shows thefirstandsubsequent sustaining pulses andTable 35-17 gives thepossible pulse
width values foraVCLK3 =100MHz.
Figure 35-33. PWM-Chopper Submodule Waveforms Showing theFirst Pulse andSubsequent Sustaining
Pulses
Table 35-17. Possible Pulse Width Values forVCLK3 =100MHz
OSHTWTHz
(hex)Pulse Width
(nS)
0 100
1 200
2 300
3 400
4 500
5 600
6 700
7 800
8 900
9 1000
A 1100
B 1200
C 1300
D 1400
E 1500
F 1600

<!-- Page 2036 -->

Duty
1/8
Duty
2/8
Duty
3/8
Duty
4/8
Duty
5/8
Duty
6/8
Duty
7/8PSCLK
12.5%25%
37.5%50%
62.5%75%
87.5%PSCLK PeriodPSCLK
period
ePWM Submodules www.ti.com
2036 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.6.4.2 Duty Cycle Control
Pulse transformer-based gate drive designs need tocomprehend themagnetic properties or
characteristics ofthetransformer andassociated circuitry. Saturation isonesuch consideration. Toassist
thegate drive designer, theduty cycles ofthesecond andsubsequent pulses have been made
programmable. These sustaining pulses ensure thecorrect drive strength andpolarity ismaintained onthe
power switch gate during theonperiod, andhence aprogrammable duty cycle allows adesign tobe
tuned oroptimized viasoftware control.
Figure 35-34 shows theduty cycle control thatispossible byprogramming theCHPDUTY bits. One of
seven possible duty ratios canbeselected ranging from 12.5% to87.5%.
Figure 35-34. PWM-Chopper Submodule Waveforms Showing thePulse Width (Duty Cycle) Control of
Sustaining Pulses

<!-- Page 2037 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
www.ti.com ePWM Submodules
2037 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.7 Trip-Zone (TZ) Submodule
Figure 35-35 shows how thetrip-zone (TZ) submodule fitswithin theePWM module.
Each ePWM module isconnected tosixTZn signals (TZ1 toTZ6). TZ1 toTZ3 aresourced from theGPIO
mux. TZ4 issourced from acombination ofEQEP1ERR andEQEP2ERR signals. TZ5 isconnected tothe
system oscillator orPLL clock faillogic, andTZ6 issourced from thedebug mode haltindication output
from theCPU. These signals indicate fault ortripconditions, andtheePWM outputs canbeprogrammed
torespond accordingly when faults occur.
Figure 35-35. Trip-Zone Submodule
35.2.7.1 Purpose oftheTrip-Zone Submodule
The keyfunctions oftheTrip-Zone submodule are:
*Trip inputs TZ1 toTZ6 aremapped toallePWM modules.
*Upon afault indication, either noaction istaken ortheePWM outputs EPWMxA andEPWMxB canbe
forced tooneofthefollowing:
-High
-Low
-High-impedance
*Support forone-shot trip(OSHT) formajor short circuits orover-current conditions.
*Support forcycle-by-cycle tripping (CBC) forcurrent limiting operation.
*Support fordigital compare tripping (DC) based onstate ofon-chip analog comparator module outputs
and/or TZ1 toTZ3 signals.
*Each trip-zone input anddigital compare (DC) submodule DCAEVT1/2 orDCBEVT1/2 force event can
beallocated toeither one-shot orcycle-by-cycle operation.
*Interrupt generation ispossible onanytrip-zone input.
*Software-forced tripping isalso supported.
*The trip-zone submodule canbefully bypassed ifitisnotrequired.

<!-- Page 2038 -->

ePWM Submodules www.ti.com
2038 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.7.2 Controlling andMonitoring theTrip-Zone Submodule
The trip-zone submodule operation iscontrolled andmonitored through thefollowing registers:
(1)Alltrip-zone registers arewritable only inprivileged mode.
(2)This register isdiscussed inmore detail inSection 35.2.9 .Table 35-18. Trip-Zone Submodule Registers
Register Name Address Offset Shadowed Description(1)
TZDCSEL 24h No Trip-zone Digital Compare Select Register(2)
TZSEL 26h No Trip-Zone Select Register
TZEINT 28h No Trip-Zone Enable Interrupt Register
TZCTL 2Ah No Trip-Zone Control Register
TZCLR 2Ch No Trip-Zone Clear Register
TZFLG 2Eh No Trip-Zone Flag Register
TZFRC 32h No Trip-Zone Force Register
35.2.7.3 Operational Highlights fortheTrip-Zone Submodule
The following sections describe theoperational highlights andconfiguration options forthetrip-zone
submodule.
The trip-zone signals TZ1 toTZ6 (also collectively referred toasTZn) areactive lowinput signals. When
oneofthese signals goes low, orwhen aDCAEVT1/2 orDCBEVT1/2 force happens based onthe
TZDCSEL register event selection, itindicates thatatripevent hasoccurred. Each ePWM module canbe
individually configured toignore oruseeach ofthetrip-zone signals orDCevents. Which trip-zone signals
orDCevents areused byaparticular ePWM module isdetermined bytheTZSEL register forthatspecific
ePWM module. The trip-zone signals may ormay notbesynchronized tothesystem clock (VCLK3) and
digitally filtered within theGPIO MUX block. Aminimum of3×TBCLK lowpulse width onTZn inputs is
sufficient totrigger afault condition ontheePWM module. Ifthepulse width isless than this, thetrip
condition may notbelatched. The asynchronous tripmakes sure thatifclocks aremissing foranyreason,
theoutputs canstillbetripped byavalid event present onTZn inputs. The GPIOs orperipherals must be
appropriately configured. Formore information, seetheIOMM chapter ofthedevice technical reference
manual.
Each TZn input canbeindividually configured toprovide either acycle-by-cycle orone-shot tripevent for
anePWM module. DCAEVT1 andDCBEVT1 events canbeconfigured todirectly tripanePWM module or
provide aone-shot tripevent tothemodule. Likewise, DCAVET2 andDCBEVT2 events canalso be
configured todirectly tripanePWM module orprovide acycle-by-cycle tripevent tothemodule. This
configuration isdetermined bytheTZSEL[DCAEVT1/2], TZSEL[DCBEVT1/2], TZSEL[CBCn], and
TZSEL[OSHTn] control bits(where ncorresponds tothetripinput) respectively.
*Cycle-by-Cycle (CBC):
When acycle-by-cycle tripevent occurs, theaction specified intheTZCTL[TZA] andTZCTL[TZB] bits
iscarried outimmediately ontheEPWMxA and/or EPWMxB output. Table 35-19 lists thepossible
actions. Inaddition, thecycle-by-cycle tripevent flag(TZFLG[CBC]) issetandaEPWMx_TZINT
interrupt isgenerated ifitisenabled intheTZEINT register andVIM peripheral.
IftheCBC interrupt isenabled viatheTZEINT register, andDCAEVT2 orDCBEVT2 areselected as
CBC tripsources viatheTZSEL register, itisnotnecessary toalso enable theDCAEVT2 orDCBEVT2
interrupts intheTZEINT register, astheDCevents trigger interrupts through theCBC mechanism.
The specified condition ontheinputs isautomatically cleared when theePWM time-base counter
reaches zero (TBCTR =0x0000) ifthetripevent isnolonger present. Therefore, inthismode, thetrip
event iscleared orreset every PWM cycle. The TZFLG[CBC] flagbitwillremain setuntil itismanually
cleared bywriting totheTZCLR[CBC] bit.Ifthecycle-by-cycle tripevent isstillpresent when the
TZFLG[CBC] bitiscleared, then itwillagain beimmediately set.

<!-- Page 2039 -->

www.ti.com ePWM Submodules
2039 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module*One-Shot (OSHT):
When aone-shot tripevent occurs, theaction specified intheTZCTL[TZA] andTZCTL[TZB] bitsis
carried outimmediately ontheEPWMxA and/or EPWMxB output. Table 35-19 lists thepossible
actions. Inaddition, theone-shot tripevent flag(TZFLG[OST]) issetandaEPWMx_TZINT interrupt is
generated ifitisenabled intheTZEINT register andVIM peripheral. The one-shot tripcondition must
becleared manually bywriting totheTZCLR[OST] bit.
Iftheone-shot interrupt isenabled viatheTZEINT register, andDCAEVT1 orDCBEVT1 areselected
asOSHT tripsources viatheTZSEL register, itisnotnecessary toalso enable theDCAEVT1 or
DCBEVT1 interrupts intheTZEINT register, astheDCevents trigger interrupts through theOSHT
mechanism.
*Digital Compare Events (DCAEVT1/2 andDCBEVT1/2):
Adigital compare DCAEVT1/2 orDCBEVT1/2 event isgenerated based onacombination ofthe
DCAH/DCAL andDCBH/DCBL signals asselected bytheTZDCSEL register. The signals which
source theDCAH/DCAL andDCBH/DCBL signals areselected viatheDCTRIPSEL register andcan
beeither tripzone input pins. Formore information onthedigital compare submodule signals, see
Section 35.2.9 .
When adigital compare event occurs, theaction specified intheTZCTL[DCAEVT1/2] and
TZCTL[DCBEVT1/2] bitsiscarried outimmediately ontheEPWMxA and/or EPWMxB output.
Table 35-19 lists thepossible actions. Inaddition, therelevant DCtripevent flag(TZFLG[DCAEVT1/2]
/TZFLG[DCBEVT1/2]) issetandaEPWMx_TZINT interrupt isgenerated ifitisenabled intheTZEINT
register andVIM peripheral.
The specified condition onthepins isautomatically cleared when theDCtripevent isnolonger
present. The TZFLG[DCAEVT1/2] orTZFLG[DCBEVT1/2] flagbitwillremain setuntil itismanually
cleared bywriting totheTZCLR[DCAEVT1/2] orTZCLR[DCBEVT1/2] bit.IftheDCtripevent isstill
present when theTZFLG[DCAEVT1/2] orTZFLG[DCBEVT1/2] flagiscleared, then itwillagain be
immediately set.
The action taken when atripevent occurs canbeconfigured individually foreach oftheePWM output
pins byway oftheTZCTL register bitfields. One offour possible actions, shown inTable 35-19 ,canbe
taken onatripevent.
Table 35-19. Possible Actions OnaTrip Event
TZCTL Register bit-
field SettingsEPWMxA
and/or
EPWMxBComment
0,0 High-Impedance Tripped
0,1 Force toHigh State Tripped
1,0 Force toLow State Tripped
1,1 NoChange DoNothing.
Nochange ismade totheoutput.

<!-- Page 2040 -->

ePWM Submodules www.ti.com
2040 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-7. Trip-Zone Configurations
Scenario A:
Aone-shot tripevent onTZ1 pulls both EPWM1A, EPWM1B lowandalso forces EPWM2A andEPWM2B
high.
*Configure theePWM1 registers asfollows:
-TZSEL[OSHT1] =1:enables TZ1 asaone-shot event source forePWM1
-TZCTL[TZA] =2:EPWM1A willbeforced lowonatripevent.
-TZCTL[TZB] =2:EPWM1B willbeforced lowonatripevent.
*Configure theePWM2 registers asfollows:
-TZSEL[OSHT1] =1:enables TZ1 asaone-shot event source forePWM2
-TZCTL[TZA] =1:EPWM2A willbeforced high onatripevent.
-TZCTL[TZB] =1:EPWM2B willbeforced high onatripevent.
Scenario B:
Acycle-by-cycle event onTZ5 pulls both EPWM1A, EPWM1B low.
Aone-shot event onTZ1 orTZ6 puts EPWM2A intoahigh impedance state.
*Configure theePWM1 registers asfollows:
-TZSEL[CBC5] =1:enables TZ5 asaone-shot event source forePWM1
-TZCTL[TZA] =2:EPWM1A willbeforced lowonatripevent.
-TZCTL[TZB] =2:EPWM1B willbeforced lowonatripevent.
*Configure theePWM2 registers asfollows:
-TZSEL[OSHT1] =1:enables TZ1 asaone-shot event source forePWM2
-TZSEL[OSHT6] =1:enables TZ6 asaone-shot event source forePWM2
-TZCTL[TZA] =0:EPWM2A willbeputintoahigh-impedance state onatripevent.
-TZCTL[TZB] =3:EPWM2B willignore thetripevent.

<!-- Page 2041 -->

Latch
cyc- by-cyc
mode
(CBC)CTR=zero
TZFRC[CBC]
TZ1
TZ2
TZ3
TZ4
TZ5
TZ6SyncClear
Set
Setone-shotLatch
(OSHT)modeClearTZSEL[CBC1 to CBC6, DCAEVT2, DCBEVT2]
TZCLR[OST]
TZSEL[OSHT1 to OSHT6, DCAEVT1, DCBEVT1]TZFRC[OSHT]Trip
Logic
Trip
TripCBC
trip event
OSHT
trip eventEPWMxA
EPWMxBEPWMxA
EPWMxBTZCTL[TZA, TZB, DCAEVT1, DCAEVT2, DCBEVT1, DCBEVT2]
AsyncTripSet
ClearTZFLG[CBC]
TZCLR[CBC]
Set
ClearTZFLG[OST]Digital
Compare
SubmoduleDCAEVT1.force
DCAEVT2.force
DCBEVT1.force
DCBEVT2.forceTZ1
TZ2
TZ3
DCAEVT2.force
DCBEVT2.force
TZ1
TZ2
TZ3
TZ4
TZ5
TZ6Sync
DCAEVT1.force
DCBEVT1.force
www.ti.com ePWM Submodules
2041 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.7.4 Generating Trip Event Interrupts
Figure 35-36 andFigure 35-37 illustrate thetrip-zone submodule control andinterrupt logic, respectively.
DCAEVT1/2 andDCBEVT1/2 signals aredescribed infurther detail inSection 35.2.9 .
Figure 35-36. Trip-Zone Submodule Mode Control Logic

<!-- Page 2042 -->

Clear
Latch
SetTZCLR[CBC]
CBC□Force
Output□EventTZEINT[CBC]TZFLG[CBC]
Clear
Latch
SetTZCLR[OST]
OST□Force
Output□EventTZEINT[OST]TZFLG[OST]
Clear
Latch
SetTZCLR[DCAEVT1]
DCAEVT1.interTZEINT[DCAEVT1]TZFLG[DCAEVT1]
Clear
Latch
SetTZCLR[DCAEVT2]
DCAEVT2.interTZEINT[DCAEVT2]TZFLG[DCAEVT2]
Clear
Latch
SetTZCLR[DCBEVT1]
DCBEVT1.interTZEINT[DCBEVT1]TZFLG[DCBEVT1]
Clear
Latch
SetTZCLR[DCBEVT2]
DCBEVT2.interTZEINT[DCBEVT2]TZFLG[DCBEVT2]Generate
Interrupt
Pulse
When
Input□=□1Clear
Latch
SetTZFLG[INT]
TZCLR[INT]
EPWMxTZINT□(PIE)
ePWM Submodules www.ti.com
2042 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-37. Trip-Zone Submodule Interrupt Logic

<!-- Page 2043 -->

Digital Compare
SignalsCounter Compare
SignalsTime Base
Signals
Dead
Band
(DB)
Counter
Compare
(CC)Action
Qualifier
(AQ)
EPWMxA
EPWMxB
CTR = CMPB
CTR = 0EPWMxINT
EPWMxSOCA
EPWMxSOCB
EPWMxA
EPWMxB
nTZ1 to nTZ3
CTR = CMP ATime-Base
(TB)CTR = PRD
CTR = 0
CTR_DirEPWMxSYNCI
EPWMxSYNCO
EPWMxTZINTPWM-
chopper
(PC)Event
Trigger
and
Interrupt
(ET)
Trip
Zone
(TZ)GPIO
MUXADC
VIM
Digital
Compare
(DC)CPU Debug Mode
OSCFAIL or PLL SLip
Combination of EQEP1ERR 
and EQEP2ERR
Digital Compare 
SignalsDigital Compare 
SignalsVIM
www.ti.com ePWM Submodules
2043 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.8 Event-Trigger (ET) Submodule
The keyfunctions oftheevent-trigger submodule are:
*Receives event inputs generated bythetime-base, counter-compare anddigital-compare submodules
*Uses thetime-base direction information forup/down event qualification
*Uses prescaling logic toissue interrupt requests andADC start ofconversion at:
-Every event
-Every second event
-Every third event
*Provides fullvisibility ofevent generation viaevent counters andflags
*Allows software forcing ofInterrupts andADC start ofconversion
The event-trigger submodule manages theevents generated bythetime-base submodule, thecounter-
compare submodule, andthedigital-compare submodule togenerate aninterrupt totheCPU and/or a
start ofconversion pulse totheADC when aselected event occurs. Figure 35-38 illustrates where the
event-trigger submodule fitswithin theePWM system.
Figure 35-38. Event-Trigger Submodule
35.2.8.1 Operational Overview oftheEvent-Trigger Submodule
The following sections describe theevent-trigger submodule's operational highlights.
Each ePWM module hasoneinterrupt request lineconnected totheVIM andtwostart ofconversion
signals connected totheADC module. Asshown inFigure 35-39 ,theePWMxSOCA andePWMxSOCB
signals arecombined togenerate four special signals thatcanbeused totrigger anADC start of
conversion, andhence multiple modules caninitiate anADC start ofconversion viatheADC trigger
inputs.

<!-- Page 2044 -->

EPWM2
moduleEPWM2SOCA
EPWM2SOCBEPWM1
moduleEPWM1SOCA
EPWM1SOCB
EPWM3
moduleEPWM3SOCA
EPWM3SOCB
EPWM4
moduleEPWM4SOCA
EPWM4SOCB
EPWM5
moduleEPWM5SOCA
EPWM5SOCB
EPWM6
moduleEPWM6SOCA
EPWM6SOCB
EPWM7SOCA
EPWM7SOCBEPWM7
module
ePWM_B ePWM_A1 ePWM_A2 ePWM_ABSOCAEN,□SOCBEN□bits
inside□ePWMx□modulesControlled□by□PINMMR
ePWM Submodules www.ti.com
2044 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-39. Event-Trigger Submodule Inter-Connectivity ofADC Start ofConversion

<!-- Page 2045 -->

VIM Event Trigger 
Module LogicCTR=Zero
CTR=PRD
CTR=CMPAEPWMxINTn
CTR=CMPB
CTR_dirDirection
qualifierCTRU=CMPA
ETSEL reg
EPWMxSOCA/n
/n
/nEPWMxSOCBADCclear
count
countclear
countclearCTRD=CMPA
CTRU=CMPB
CTRD=CMPBETPS reg
ETFLG reg
ETCLR reg
ETFRC regCTR=Zero or PRD
DCAEVT1.soc
DCBEVT1.socFrom Digital Compare
(DC) Submodule
www.ti.com ePWM Submodules
2045 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleThe event-trigger submodule monitors various event conditions (the leftside inputs toevent-trigger
submodule shown inFigure 35-40 )andcanbeconfigured toprescale these events before issuing an
Interrupt request oranADC start ofconversion. The event-trigger prescaling logic canissue Interrupt
requests andADC start ofconversion at:
*Every event
*Every second event
*Every third event
Figure 35-40. Event-Trigger Submodule Showing Event Inputs andPrescaled Outputs
The keyregisters used toconfigure theevent-trigger submodule arelisted inTable 35-20 .
Table 35-20. Event-Trigger Submodule Registers
Register Name Address Offset Shadowed Description
ETSEL 30h No Event-trigger Selection Register
ETFLG 34h No Event-trigger Flag Register
ETPS 36h No Event-trigger Prescale Register
ETFRC 38h No Event-trigger Force Register
ETCLR 3Ah No Event-trigger Clear Register
*ETSEL --This selects which ofthepossible events willtrigger aninterrupt orstart anADC conversion
*ETPS --This programs theevent prescaling options mentioned above.
*ETFLG --These areflagbitsindicating status oftheselected andprescaled events.
*ETCLR --These bitsallow youtoclear theflagbitsintheETFLG register viasoftware.
*ETFRC --These bitsallow software forcing ofanevent. Useful fordebugging ors/wintervention.
Amore detailed look athow thevarious register bitsinteract with theInterrupt andADC start of
conversion logic areshown inFigure 35-41 ,Figure 35-42 ,andFigure 35-43 .

<!-- Page 2046 -->

ePWM Submodules www.ti.com
2046 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-41 shows theevent-trigger's interrupt generation logic. The interrupt-period (ETPS[INTPRD])
bitsspecify thenumber ofevents required tocause aninterrupt pulse tobegenerated. The choices
available are:
*Donotgenerate aninterrupt.
*Generate aninterrupt onevery event
*Generate aninterrupt onevery second event
*Generate aninterrupt onevery third event
Which event cancause aninterrupt isconfigured bytheinterrupt selection (ETSEL[INTSEL]) bits. The
event canbeoneofthefollowing:
*Time-base counter equal tozero (TBCTR =0x0000).
*Time-base counter equal toperiod (TBCTR =TBPRD).
*Time-base counter equal tozero orperiod (TBCTR =0x0000 ||TBCTR =TBPRD)
*Time-base counter equal tothecompare Aregister (CMPA) when thetimer isincrementing.
*Time-base counter equal tothecompare Aregister (CMPA) when thetimer isdecrementing.
*Time-base counter equal tothecompare Bregister (CMPB) when thetimer isincrementing.
*Time-base counter equal tothecompare Bregister (CMPB) when thetimer isdecrementing.
The number ofevents thathave occurred canberead from theinterrupt event counter (ETPS[INTCNT])
register bits. That is,when thespecified event occurs theETPS[INTCNT] bitsareincremented until they
reach thevalue specified byETPS[INTPRD]. When ETPS[INTCNT] =ETPS[INTPRD] thecounter stops
counting anditsoutput isset.The counter isonly cleared when aninterrupt issent totheVIM.
When ETPS[INTCNT] reaches ETPS[INTPRD] thefollowing behaviors willoccur:
*Ifinterrupts areenabled, ETSEL[INTEN] =1andtheinterrupt flagisclear, ETFLG[INT] =0,then an
interrupt pulse isgenerated andtheinterrupt flagisset,ETFLG[INT] =1,andtheevent counter is
cleared ETPS[INTCNT] =0.The counter willbegin counting events again.
*Ifinterrupts aredisabled, ETSEL[INTEN] =0,ortheinterrupt flagisset,ETFLG[INT] =1,thecounter
stops counting events when itreaches theperiod value ETPS[INTCNT] =ETPS[INTPRD].
*Ifinterrupts areenabled, buttheinterrupt flagisalready set,then thecounter willhold itsoutput high
until theENTFLG[INT] flagiscleared. This allows foroneinterrupt tobepending while oneisserviced.
Writing totheINTPRD bitswillautomatically clear thecounter INTCNT =0andthecounter output willbe
reset (sonointerrupts aregenerated). Writing a1totheETFRC[INT] bitwillincrement theevent counter
INTCNT. The counter willbehave asdescribed above when INTCNT =INTPRD. When INTPRD =0,the
counter isdisabled andhence noevents willbedetected andtheETFRC[INT] bitisalso ignored.
The above definition means thatyoucangenerate aninterrupt onevery event, onevery second event, or
onevery third event. Aninterrupt cannot begenerated onevery fourth ormore events.

<!-- Page 2047 -->

Latch
Generate
SOC
Pulse
When
Input = 12-bit
CounterSetClear
Clear CNT
Inc CNTETPS[SOCACNT]
ETPS[SOCAPRD]ETCLR[SOCA]
SOCAETFRC[SOCA]
ETSEL[SOCA]000
001
010
011
100
101
111110DCAEVT1.soc[A]ETFLG[SOCA]
CTRU=CMPA
CTRD=CMPA
CTRU=CMPB
CTRD=CMPBETSEL[SOCASEL]
CTR=Zero
CTR=PRD
Latch
Generate
Interrupt
Pulse
When
Input = 12-bit
CounterSetClear
1
00
Clear CNT
Inc CNTETPS[INTCNT]
ETPS[INTPRD]ETCLR[INT]
EPWMxINT
ETFRC[INT]
ETSEL[INT]000
001
010
011
100
101
1111100ETFLG[INT]
CTR=Zero
CTR=PRD
CTRU=CMPA
CTRD=CMPA
CTRU=CMPB
CTRD=CMPBETSEL[INTSEL]
www.ti.com ePWM Submodules
2047 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-41. Event-Trigger Interrupt Generator
Figure 35-42 shows theoperation oftheevent-trigger's start-of-conversion-A (SOCA) pulse generator. The
ETPS[SOCACNT] counter andETPS[SOCAPRD] period values behave similarly totheinterrupt generator
except thatthepulses arecontinuously generated. That is,thepulse flagETFLG[SOCA] islatched when a
pulse isgenerated, butitdoes notstop further pulse generation. The enable/disable bitETSEL[SOCAEN]
stops pulse generation, butinput events canstillbecounted until theperiod value isreached aswith the
interrupt generation logic. The event thatwilltrigger anSOCA andSOCB pulse canbeconfigured
separately intheETSEL[SOCASEL] andETSEL[SOCBSEL] bits. The possible events arethesame
events thatcanbespecified fortheinterrupt generation logic with theaddition oftheDCAEVT1.soc and
DCBEVT1.soc event signals from thedigital compare (DC) submodule.
Figure 35-42. Event-Trigger SOCA Pulse Generator
A The DCAEVT1.soc signals aresignals generated bytheDigital compare (DC) submodule, described inSection 35.2.9

<!-- Page 2048 -->

 
Event A
Qual DCALDCAEVT 1
DCAEVT 2
Event 
TriggeringDCAEVT1 .inter
DCAEVT2.inter
DCBEVT 1.socDCBEVT1.inter
DCBEVT 2.interDCAEVT1.sy nc
DCAEVT1.socDCBEVT1.sync
DCBH Event B 
Qual DCBLDCBEVT 1
DCBEVT 2Event 
Filtering
Blanking 
Window
Counter 
CaptureDCEVTFILTD
C
T
R
I
P
S
E
LTZ1
TZ2
TZ3DCAHDigital Compare Submodule
Time- Base 
submodule
Trip- Zone 
submodule
Event- Trigger 
submoduleDCAEVT1.force
DCAEVT2.force
DCBEVT1.f orce
DCBEVT2.forceCOMPGPIO
MUX
Latch
Generate
SOC
Pulse
When
Input = 12-bit
CounterSetClear
Clear CNT
Inc CNTETPS[SOCBCNT]
ETPS[SOCBPRD]ETCLR[SOCB]
SOCBETFRC[SOCB]
ETSEL[SOCB]000
001
010
011
100
101
111110DCBEVT1.soc[A]ETFLG[SOCB]
CTR=Zero
CTR=PRD
CTRU=CMPA
CTRD=CMPA
CTRU=CMPB
CTRD=CMPBETSEL[SOCBSEL]
ePWM Submodules www.ti.com
2048 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-43 shows theoperation oftheevent-trigger's start-of-conversion-B (SOCB) pulse generator. The
event-trigger's SOCB pulse generator operates thesame way astheSOCA.
Figure 35-43. Event-Trigger SOCB Pulse Generator
A The DCBEVT1.soc signals aresignals generated bytheDigital compare (DC) submodule, described inSection 35.2.9
35.2.9 Digital Compare (DC) Submodule
Figure 35-44 illustrates where thedigital compare (DC) submodule signals interface toother submodules
intheePWM system.
The digital compare (DC) submodule compares signals external totheePWM module todirectly generate
PWM events/actions which then feed totheevent-trigger, trip-zone, andtime-base submodules.
Additionally, blanking window functionality issupported tofilter noise orunwanted pulses from theDC
event signals.
Figure 35-44. Digital-Compare Submodule High-Level Block Diagram

<!-- Page 2049 -->

www.ti.com ePWM Submodules
2049 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.9.1 Purpose oftheDigital Compare Submodule
The keyfunctions ofthedigital compare submodule are:
*TZ1, TZ2, andTZ3 inputs generate Digital Compare AHigh/Low (DCAH, DCAL) andDigital Compare
BHigh/Low (DCBH, DCBL) signals.
*DCAH/L andDCBH/L signals trigger events which canthen either befiltered orfeddirectly tothetrip-
zone, event-trigger, andtime-base submodules to:
-generate atripzone interrupt
-generate anADC start ofconversion
-force anevent
-generate asynchronization event forsynchronizing theePWM module TBCTR.
*Event filtering (blanking window logic) canoptionally blank theinput signal toremove noise.
35.2.9.2 Controlling andMonitoring theDigital Compare Submodule
The digital compare submodule operation iscontrolled andmonitored through thefollowing registers:
(1)These registers arewritable only inprivileged mode.
(2)The TZDCSEL register ispart ofthetrip-zone submodule butismentioned again here because ofitsfunctional significance to
thedigital compare submodule.Table 35-21. Digital Compare Submodule Registers
Register Name Address Offset Shadowed Description
TZDCSEL(1)(2)24h No Trip Zone Digital Compare Select Register
DCACTL(1)60h No Digital Compare AControl Register
DCTRIPSEL(1)62h No Digital Compare Trip Select Register
DCFCTL(1)64h No Digital Compare Filter Control Register
DCBCTL(1)66h No Digital Compare BControl Register
DCFOFFSET 68h Writes Digital Compare Filter Offset Register
DCCAPCTL(1)6Ah No Digital Compare Capture Control Register
DCFWINDOW 6Ch No Digital Compare Filter Window Register
DCFOFFSETCNT 6Eh No Digital Compare Filter Offset Counter Register
DCCAP 70h Yes Digital Compare Counter Capture Register
DCFWINDOWCNT 72h No Digital Compare Filter Window Counter Register

<!-- Page 2050 -->

ePWM Submodules www.ti.com
2050 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.9.3 Operation Highlights oftheDigital Compare Submodule
The following sections describe theoperational highlights andconfiguration options forthedigital compare
submodule.
35.2.9.3.1 Digital Compare Events
Asillustrated inFigure 35-44 earlier inthissection, tripzone inputs (TZ1, TZ2, andTZ3) canbeselected
viatheDCTRIPSEL bitstogenerate theDigital Compare AHigh andLow (DCAH/L) andDigital Compare
BHigh andLow (DCBH/L) signals. Then, theconfiguration oftheTZDCSEL register qualifies theactions
ontheselected DCAH/L andDCBH/L signals, which generate theDCAEVT1/2 andDCBEVT1/2 events
(Event Qualification AandB).
NOTE: The TZn signals, when used asaDCEVT tripping functions, aretreated asanormal input
signal andcanbedefined tobeactive high oractive lowinputs. EPWM outputs are
asynchronously tripped when either theTZn, DCAEVTx.force, orDCBEVTx.force signals are
active. Forthecondition toremain latched, aminimum of3*TBCLK sync pulse width is
required. Ifpulse width is<3*TBCLK sync pulse width, thetripcondition may ormay notget
latched byCBC orOST latches.
The DCAEVT1/2 andDCBEVT1/2 events canthen befiltered toprovide afiltered version oftheevent
signals (DCEVTFILT) orthefiltering canbebypassed. Filtering isdiscussed further insection 2.9.3.2.
Either theDCAEVT1/2 andDCBEVT1/2 event signals orthefiltered DCEVTFILT event signals can
generate aforce tothetripzone module, aTZinterrupt, anADC SOC, oraPWM sync signal.
*force signal:
DCAEVT1/2.force signals force tripzone conditions which either directly influence theoutput onthe
EPWMxA pin(viaTZCTL[DCAEVT1 orDCAEVT2] configurations) or,iftheDCAEVT1/2 signals are
selected asone-shot orcycle-by-cycle tripsources (viatheTZSEL register), theDCAEVT1/2.force
signals caneffect thetripaction viatheTZCTL[TZA] configuration. The DCBEVT1/2.force signals
behaves similarly, butaffect theEPWMxB output pininstead oftheEPWMxA output pin.
The priority ofconflicting actions ontheTZCTL register isasfollows (highest priority overrides lower
priority):
Output EPWMxA: TZA (highest) ->DCAEVT1 ->DCAEVT2 (lowest)
Output EPWMxB: TZB (highest) ->DCBEVT1 ->DCBEVT2 (lowest)
*interrupt signal:
DCAEVT1/2.interrupt signals generate tripzone interrupts totheVIM. Toenable theinterrupt, theuser
must settheDCAEVT1, DCAEVT2, DCBEVT1, orDCBEVT2 bitsintheTZEINT register. Once oneof
these events occurs, anEPWMxTZINT interrupt istriggered, andthecorresponding bitintheTZCLR
register must besetinorder toclear theinterrupt.
*socsignal:
The DCAEVT1.soc signal interfaces with theevent-trigger submodule andcanbeselected asanevent
which generates anADC start-of-conversion-A (SOCA) pulse viatheETSEL[SOCASEL] bit.Likewise,
theDCBEVT1.soc signal canbeselected asanevent which generates anADC start-of-conversion-B
(SOCB) pulse viatheETSEL[SOCBSEL] bit.
*sync signal:
The DCAEVT1.sync andDCBEVT1.sync events areORed with theEPWMxSYNCI input signal andthe
TBCTL[SWFSYNC] signal togenerate asynchronization pulse tothetime-base counter.

<!-- Page 2051 -->

Set
Latch
Clear
TZCLR[DCAEVT2]DCAEVT2.interTZEINT[DCAEVT2]
TZFLG[DCAEVT2]DCAEVT2.force1
0 Sync
TBCLKAsync1
0DCACTL[EVT2SRCSEL]
DCEVTFILT
DCAEVT2DCACTL[EVT2FRCSYNCSEL]
TZFRC[DCAEVT2]
DCACTL[EVT1SYNCE]DCAEVT1.syncDCACTL[EVT1SOCE]DCAEVT1.socSet
Latch
Clear
TZCLR[DCAEVT1]DCAEVT1.interTZEINT[DCAEVT1]
TZFLG[DCAEVT1]DCAEVT1.force1
0 Sync
TBCLKAsync1
0DCACTL[EVT1SRCSEL]
DCEVTFILT
DCAEVT1DCACTL[EVT1FRCSYNCSEL]
TZFRC[DCAEVT1]
www.ti.com ePWM Submodules
2051 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-45 andFigure 35-46 show how theDCAEVT1, DCAEVT2, orDCEVTFILT signals are
processed togenerate thedigital compare Aevent force, interrupt, socandsync signals.
Figure 35-45. DCAEVT1 Event Triggering
Figure 35-46. DCAEVT2 Event Triggering

<!-- Page 2052 -->

Sync DCBEVT2.force1
0
TBCLKDCBEVT21
0
TZFRC[DCBEVT2]Latchset
clear
TZCLR[DCBEVT2] TZFLG[DCBEVT2]TZEINT[DCBEVT2]
DCBEVT2.interasyncDCEVTFILTDCBCTL[EVT2FRCSYNCSEL] DCBCTL[EVT2SRCSEL]
Sync DCBEVT1.force1
0
TBCLKDCBEVT11
0
TZFRC[DCBEVT1]DCBEVT1.soc
DCBCTL[EVT1SOCE]Latchset
clear
TZCLR[DCBEVT1] TZFLG[DCBEVT1]TZEINT[DCBEVT1]
DCBEVT1.inter
DCBEVT1.sync
DCBCTL[EVT1SYNCE]asyncDCEVTFILTDCBCTL[EVT1FRCSYNCSEL] DCBCTL[EVT1SRCSEL]
ePWM Submodules www.ti.com
2052 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-47 andFigure 35-48 show how theDCBEVT1, DCBEVT2, orDCEVTFILT signals are
processed togenerate thedigital compare Bevent force, interrupt, socandsync signals.
Figure 35-47. DCBEVT1 Event Triggering
Figure 35-48. DCBEVT2 Event Triggering

<!-- Page 2053 -->

DCAEVT1DCFOFFSET[OFFSET]
SyncBlank
Control
LogicDCFWINDOW[WINDOW]DCFCTL[BLANKE, PULSESEL]DCCAP[15:0] Reg
DCAEVT2
DCBEVT1
DCBEVT200
01
10
11DCFCTL[PULSESEL]TBCTR (16)
Capture
Control
LogicCTR = PRD
CTR = 0
TBCLKCTR=PRD
CTR=Zero
TBCLK
TBCLK
DCEVTFILT async
DCFCTL[SRCSEL]1      0BLANKWDW
DCFCTL[INVERT]DCCAPCTL[CAPE, SHDWMODE]
www.ti.com ePWM Submodules
2053 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.2.9.3.2 Event Filtering
The DCAEVT1/2 andDCBEVT1/2 events canbefiltered viaevent filtering logic toremove noise by
optionally blanking events foracertain period oftime. This isuseful forcases where theanalog
comparator outputs may beselected totrigger DCAEVT1/2 andDCBEVT1/2 events, andtheblanking
logic isused tofilter outpotential noise onthesignal prior totripping thePWM outputs orgenerating an
interrupt orADC start-of-conversion. The event filtering canalso capture theTBCTR value ofthetrip
event. Figure 35-49 shows thedetails oftheevent filtering logic.
Figure 35-49. Event Filtering
Iftheblanking logic isenabled, oneofthedigital compare events -DCAEVT1, DCAEVT2, DCBEVT1,
DCBEVT2 -isselected forfiltering. The blanking window, which filters outallevent occurrences onthe
signal while itisactive, willbealigned toeither aCTR =PRD pulse oraCTR =0pulse (configured bythe
DCFCTL[PULSESEL] bits). Anoffset value inTBCLK counts isprogrammed intotheDCFOFFSET
register, which determines atwhat point after theCTR =PRD orCTR =0pulse theblanking window
starts. The duration oftheblanking window, innumber ofTBCLK counts after theoffset counter expires, is
written totheDCFWINDOW register bytheapplication. During theblanking window, allevents are
ignored. Before andafter theblanking window ends, events cangenerate soc, sync, interrupt, andforce
signals asbefore.
Figure 35-50 shows several timing conditions fortheoffset andblanking window within anePWM period.
Notice thatiftheblanking window crosses theCTR =0orCTR =PRD boundary, thenext window still
starts atthesame offset value after theCTR =0orCTR =PRD pulse.

<!-- Page 2054 -->

Offset(n)
Window(n)
Offset(n+1)Window(n+1)Period
Offset(n)
Window(n)Offset(n+1)
Window(n+1)
Offset(n)
Window(n)Offset(n+1)
Window(n+1)TBCLK
CTR = PRD
or□CTR = 0
BLANKWDW
BLANKWDW
BLANKWDW
ePWM Submodules www.ti.com
2054 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-50. Blanking Window Timing Diagram
35.2.10 Proper Interrupt Initialization Procedure
When theePWM peripheral clock isenabled itmay bepossible thatinterrupt flags may besetdueto
spurious events duetotheePWM registers notbeing properly initialized. The proper procedure for
initializing theePWM peripheral isasfollows:
1.Disable global interrupts (CPU INTM flag)
2.Disable ePWM interrupts
3.SetTBCLKSYNC =0
4.Initialize peripheral registers
5.SetTBCLKSYNC =1
6.Clear anyspurious ePWM flags (including interrupt flags)
7.Enable ePWM interrupts
8.Enable global interrupts

<!-- Page 2055 -->

CTR = 0
CTR=CMPB
XEN
SyncOutPhase reg
EPWMxA
EPWMxBSyncIn
F=0°
www.ti.com Application Examples
2055 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.3 Application Examples
AnePWM module hasallthelocal resources necessary tooperate completely asastandalone module or
tooperate insynchronization with other identical ePWM modules.
35.3.1 Overview ofMultiple Modules
Previously inthisuser's guide, alldiscussions have described theoperation ofasingle module. To
facilitate theunderstanding ofmultiple modules working together inasystem, theePWM module
described inreference isrepresented bythemore simplified block diagram shown inFigure 35-51 .This
simplified ePWM block shows only thekeyresources needed toexplain how amultiswitch power topology
iscontrolled with multiple ePWM modules working together.
Figure 35-51. Simplified ePWM Module
35.3.2 Key Configuration Capabilities
The keyconfiguration choices available toeach module areasfollows:
*Options forSyncIn
-Load own counter with phase register onanincoming sync strobe --enable (EN) switch closed
-Donothing orignore incoming sync strobe --enable switch open
-Sync flow-through -SyncOut connected toSyncIn
-Master mode, provides async atPWM boundaries --SyncOut connected toCTR =PRD
-Master mode, provides async atanyprogrammable point intime--SyncOut connected toCTR =
CMPB
-Module isinstandalone mode andprovides Nosync toother modules --SyncOut connected toX
(disabled)
*Options forSyncOut
-Sync flow-through -SyncOut connected toSyncIn
-Master mode, provides async atPWM boundaries --SyncOut connected toCTR =PRD
-Master mode, provides async atanyprogrammable point intime--SyncOut connected toCTR =
CMPB
-Module isinstandalone mode andprovides Nosync toother modules --SyncOut connected toX
(disabled)
Foreach choice ofSyncOut, amodule may also choose toload itsown counter with anew phase value
onaSyncIn strobe input orchoose toignore it,i.e.,viatheenable switch. Although various combinations
arepossible, thetwomost common --master module andslave module modes --areshown inFigure 35-
52.

<!-- Page 2056 -->

CTR=0
CTR=CMPB
XEN
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=0
XEN
EPWM2BEPWM2ASlave Master
SyncIn SyncIn
1 2F=0° F=0°
Application Examples www.ti.com
2056 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-52. EPWM1 Configured asaTypical Master, EPWM2 Configured asaSlave

<!-- Page 2057 -->

CTR=zero
CTR=CMPB
XEn
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=zero
XEn
EPWM2BEPWM2AMaster2Master1
SyncIn
CTR=zero
CTR=CMPB
SyncOutXEPWM3BPhase regMaster3
En
EPWM3A1
2
3Φ=X
Φ=X
Φ=X
CTR=zero
CTR=CMPB
SyncOutXEPWM4BPhase regMaster4
En
EPWM4A
3Φ=XBuck #1Vout1 Vin1
EPWM1A
Buck #2Vin2
EPWM2AVout2
Buck #4Buck #3Vin3
EPWM4AVin4EPWM3AVout3
Vout4SyncIn
SyncInSyncIn
www.ti.com Application Examples
2057 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.3.3 Controlling Multiple Buck Converters With Independent Frequencies
One ofthesimplest power converter topologies isthebuck. Asingle ePWM module configured asa
master cancontrol twobuck stages with thesame PWM frequency. Ifindependent frequency control is
required foreach buck converter, then oneePWM module must beallocated foreach converter stage.
Figure 35-53 shows four buck stages, each running atindependent frequencies. Inthiscase, allfour
ePWM modules areconfigured asMasters andnosynchronization isused. Figure 35-54 shows the
waveforms generated bythesetup shown inFigure 35-53 ;note thatonly three waveforms areshown,
although there arefour stages.
Figure 35-53. Control ofFour Buck Stages. Here FPWM1≠FPWM2≠FPWM3≠FPWM4
NOTE:Θ=Xindicates value inphase register isa"don'tcare"

<!-- Page 2058 -->

P CA CB
AP CA P
Pulse centerEPWM1A700 950
1200
P CA CB
AP CA700 1150
1400
EPWM2A
CA P CA
CB
AP CA P500650
800
EPWM3A
PIndicates this event triggers an interrupt CB
AIP
IP
IP
I
Indicates this event triggers an ADC start
of conversion
Application Examples www.ti.com
2058 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-54. Buck Waveforms forFigure 35-53 (Note: Only three bucks shown here)

<!-- Page 2059 -->

www.ti.com Application Examples
2059 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-8. Configuration forExample inFigure 35-54
//=====================================================================
//(Note: code for only 3modules shown)
//Initialization Time
//========================
//EPWM Module 1config
EPwm1Regs.TBPRD =1200; //Period =1201 TBCLK counts
EPwm1Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP; //Asymmetrical mode
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.AQCTLA.bit.PRD =AQ_CLEAR;
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET;
//EPWM Module 2config
EPwm2Regs.TBPRD =1400; //Period =1401 TBCLK counts
EPwm2Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm2Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP; //Asymmetrical mode
EPwm2Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm2Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm2Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm2Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.AQCTLA.bit.PRD =AQ_CLEAR;
EPwm2Regs.AQCTLA.bit.CAU =AQ_SET;
//EPWM Module 3config
EPwm3Regs.TBPRD =800; //Period =801 TBCLK counts
EPwm3Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm3Regs.TBCTL.bit.CTRMODE =TB_COUNT_UP;
EPwm3Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Phase loading disabled
EPwm3Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm3Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_DISABLE;
EPwm3Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm3Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm3Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm3Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm3Regs.AQCTLA.bit.PRD =AQ_CLEAR;
EPwm3Regs.AQCTLA.bit.CAU =AQ_SET;
//
//Run Time (Note: Example execution ofone run-time instant)
//=========================================================
EPwm1Regs.CMPA.half.CMPA =700; //adjust duty for output EPWM1A
EPwm2Regs.CMPA.half.CMPA =700; //adjust duty for output EPWM2A
EPwm3Regs.CMPA.half.CMPA =500; //adjust duty for output EPWM3A

<!-- Page 2060 -->

CTR=zero
CTR=CMPB
XEn
F=0°
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=zero
XF=XEn
EPWM2BEPWM2ASlaveMasterBuck #1Vout1 Vin1
EPWM1A
Buck #2Vin2
EPWM1BVout2
Buck #4Buck #3Vin3
EPWM2BVin4EPWM2AVout3
Vout4SyncIn
SyncIn
Application Examples www.ti.com
2060 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.3.4 Controlling Multiple Buck Converters With Same Frequencies
Ifsynchronization isarequirement, ePWM module 2canbeconfigured asaslave andcanoperate at
integer multiple (N)frequencies ofmodule 1.The sync signal from master toslave ensures these modules
remain locked. Figure 35-55 shows such aconfiguration; Figure 35-56 shows thewaveforms generated by
theconfiguration.
Figure 35-55. Control ofFour Buck Stages. (Note: FPWM2=NxFPWM1)

<!-- Page 2061 -->

200400600
400
200
300500
300500EPWM1A
EPWM1B
EPWM2BEPWM2AZ
I
APCA CAZ
IZ
I
APCA CA
CB CB CB CB
CA CA CA CA
CB CB CB CB
www.ti.com Application Examples
2061 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-56. Buck Waveforms forFigure 35-55 (Note: FPWM2=FPWM1))

<!-- Page 2062 -->

Application Examples www.ti.com
2062 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-9. Code Snippet forConfiguration inFigure 35-55
//========================
//EPWM Module 1config
EPwm1Regs.TBPRD =600; //Period =1200 TBCLK counts
EPwm1Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetrical mode
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Master module
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_CTR_ZERO; //Sync down-stream module
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.AQCTLA.bit.CAU =AQ_SET; //set actions for EPWM1A
EPwm1Regs.AQCTLA.bit.CAD =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBU =AQ_SET; //set actions for EPWM1B
EPwm1Regs.AQCTLB.bit.CBD =AQ_CLEAR;
//EPWM Module 2config
EPwm2Regs.TBPRD =600; //Period =1200 TBCLK counts
EPwm2Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm2Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetrical mode
EPwm2Regs.TBCTL.bit.PHSEN =TB_ENABLE; //Slave module
EPwm2Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm2Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_IN; //sync flow-through
EPwm2Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.AQCTLA.bit.CAU =AQ_SET; //set actions for EPWM2A
EPwm2Regs.AQCTLA.bit.CAD =AQ_CLEAR;
EPwm2Regs.AQCTLB.bit.CBU =AQ_SET; //set actions for EPWM2B
EPwm2Regs.AQCTLB.bit.CBD =AQ_CLEAR;
//
//Run Time (Note: Example execution ofone run-time instance)
//===========================================================
EPwm1Regs.CMPA.half.CMPA =400; //adjust duty for output EPWM1A
EPwm1Regs.CMPB =200; //adjust duty for output EPWM1B
EPwm2Regs.CMPA.half.CMPA =500; //adjust duty for output EPWM2A
EPwm2Regs.CMPB =300; //adjust duty for output EPWM2B

<!-- Page 2063 -->

CTR=zero
CTR=CMPB
XEn
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=zero
XEn
EPWM2BEPWM2ASlaveMasterVout1
EPWM1ASyncIn
SyncInVDC_bus
EPWM1B
EPWM2BEPWM2AVDC_busVout2F=0°
F=0°
www.ti.com Application Examples
2063 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.3.5 Controlling Multiple Half H-Bridge (HHB) Converters
Topologies thatrequire control ofmultiple switching elements canalso beaddressed with these same
ePWM modules. Itispossible tocontrol aHalf-H bridge stage with asingle ePWM module. This control
canbeextended tomultiple stages. Figure 35-57 shows control oftwosynchronized Half-H bridge stages
where stage 2canoperate atinteger multiple (N)frequencies ofstage 1.Figure 35-58 shows the
waveforms generated bytheconfiguration shown inFigure 35-57 .
Module 2(slave) isconfigured forSync flow-through; ifrequired, thisconfiguration allows forathird Half-H
bridge tobecontrolled byPWM module 3andalso, most importantly, toremain insynchronization with
master module 1.
Figure 35-57. Control ofTwo Half-H Bridge Stages (FPWM2=NxFPWM1)

<!-- Page 2064 -->

EPWM1A
EPWM1B
EPWM2A
EPWM2B600
200400400
200
250500500
250
Pulse CenterACBCA ZZ
IZ
IZ
IZ
I
ACBCA Z
Z
ACBCA
Pulse CenterZ
ACBCA
ACB CA Z
ACBCA Z
ACB Z CA
ACBZ CA
Application Examples www.ti.com
2064 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-58. Half-H Bridge Waveforms forFigure 35-57 (Note: Here FPWM2=FPWM1)

<!-- Page 2065 -->

www.ti.com Application Examples
2065 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleExample 35-10. Code Snippet forConfiguration inFigure 35-57
//=====================================================================
//Config
//=====================================================================
//Initialization Time
//========================
//EPWM Module 1config
EPwm1Regs.TBPRD =600; //Period =1200 TBCLK counts
EPwm1Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm1Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetrical mode
EPwm1Regs.TBCTL.bit.PHSEN =TB_DISABLE; //Master module
EPwm1Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm1Regs.TBCTL.bit.SYNCOSEL =TB_CTR_ZERO; //Sync down-stream module
EPwm1Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm1Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm1Regs.AQCTLA.bit.ZRO =AQ_SET; //set actions for EPWM1A
EPwm1Regs.AQCTLA.bit.CAU =AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.ZRO =AQ_CLEAR; //set actions for EPWM1B
EPwm1Regs.AQCTLB.bit.CAD =AQ_SET;
//EPWM Module 2config
EPwm2Regs.TBPRD =600; //Period =1200 TBCLK counts
EPwm2Regs.TBPHS.half.TBPHS =0; //Set Phase register tozero
EPwm2Regs.TBCTL.bit.CTRMODE =TB_COUNT_UPDOWN; //Symmetrical mode
EPwm2Regs.TBCTL.bit.PHSEN =TB_ENABLE; //Slave module
EPwm2Regs.TBCTL.bit.PRDLD =TB_SHADOW;
EPwm2Regs.TBCTL.bit.SYNCOSEL =TB_SYNC_IN; //sync flow-through
EPwm2Regs.CMPCTL.bit.SHDWAMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.SHDWBMODE =CC_SHADOW;
EPwm2Regs.CMPCTL.bit.LOADAMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.CMPCTL.bit.LOADBMODE =CC_CTR_ZERO; //load onCTR=Zero
EPwm2Regs.AQCTLA.bit.ZRO =AQ_SET; //set actions for EPWM1A
EPwm2Regs.AQCTLA.bit.CAU =AQ_CLEAR;
EPwm2Regs.AQCTLB.bit.ZRO =AQ_CLEAR; //set actions for EPWM1B
EPwm2Regs.AQCTLB.bit.CAD =AQ_SET;
//============================================================
EPwm1Regs.CMPA.half.CMPA =400; //adjust duty for output EPWM1A &EPWM1B
EPwm1Regs.CMPB =200; //adjust point-in-time for ADCSOC trigger
EPwm2Regs.CMPA.half.CMPA =500; //adjust duty for output EPWM2A &EPWM2B
EPwm2Regs.CMPB =250; //adjust point-in-time for ADCSOC trigger
35.3.6 Controlling Dual 3-Phase Inverters forMotors (ACI andPMSM)
The idea ofmultiple modules controlling asingle power stage canbeextended tothe3-phase Inverter
case. Insuch acase, sixswitching elements canbecontrolled using three PWM modules, oneforeach
legoftheinverter. Each legmust switch atthesame frequency andalllegs must besynchronized. A
master +twoslaves configuration caneasily address thisrequirement. Figure 35-59 shows how sixPWM
modules cancontrol twoindependent 3-phase Inverters; each running amotor.
Asinthecases shown intheprevious sections, wehave achoice ofrunning each inverter atadifferent
frequency (module 1andmodule 4aremasters asinFigure 35-59 ),orboth inverters canbesynchronized
byusing onemaster (module 1)andfiveslaves. Inthiscase, thefrequency ofmodules 4,5,and6(all
equal) canbeinteger multiples ofthefrequency formodules 1,2,3(also allequal).

<!-- Page 2066 -->

F=0°CTR=zero
CTR=CMPB
XEn
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=zero
XEn
EPWM2BEPWM2ASlaveMaster
EPWM1A
EPWM1BEPWM2A
EPWM2B EPWM3BEPWM3A
VAB
VCD
VEF
3 phase motor
EPWM4B EPWM5BVABEPWM4A
VCDEPWM5A
EPWM6B3 phase motorVEFEPWM6A3 phase inverter #1
3 phase inverter #21
2
CTR=zero
CTR=CMPBPhase reg
3Slave
SyncOutXEn
EPWM3BEPWM3A
Phase reg
CTR=CMPBCTR=zero
4Slave
SyncOutXEPWM4A
EPWM4BEn
SyncOutCTR=zero
CTR=CMPBPhase regPhase reg
CTR=CMPBCTR=zero
Slave
65Slave
XEnSyncIn
EPWM6BEPWM6ASyncOutXEPWM5A
EPWM5BEnF=0°
F=0°
F=0°
F=0°
F=0°SyncInSyncIn
SyncInSyncInSyncIn
Application Examples www.ti.com
2066 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-59. Control ofDual 3-Phase Inverter Stages asIsCommonly Used inMotor Control

<!-- Page 2067 -->

RED
FED
FED
FEDREDREDRED
FEDEPWM1A
EPWM1B
EPWM2A
EPWM2B
EPWM3A
EPWM3BΦ2=0
Φ3=0800
500 500
600 600
700700Z
I
APCA CAZ
I
APCA CA
CA CA CA CA
CACA CACA
www.ti.com Application Examples
2067 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-60. 3-Phase Inverter Waveforms forFigure 35-59 (Only One Inverter Shown)

<!-- Page 2068 -->

CTR=zero
CTR=CMPB
XEn
SyncOutPhase regExt SyncIn
(optional)
EPWM1A
EPWM1B
SyncOutPhase reg
CTR=CMPBCTR=zero
XEn
EPWM2BEPWM2ASlaveMaster
SyncIn
SyncIn1
2F=0°
F=120°
Application Examples www.ti.com
2068 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.3.7 Practical Applications Using Phase Control Between PWM Modules
Sofar,none oftheexamples have made useofthephase register (TBPHS). Ithaseither been settozero
oritsvalue hasbeen adon't care. However, byprogramming appropriate values intoTBPHS, multiple
PWM modules canaddress another class ofapplications thatrelyonphase relationship between stages
forcorrect operation. Asdescribed intheTBmodule section, aPWM module canbeconfigured toallow a
SyncIn pulse tocause theTBPHS register tobeloaded intotheTBCTR register. Toillustrate thisconcept,
Figure 35-61 shows amaster andslave module with aphase relationship of120°,i.e.,theslave leads the
master.
Figure 35-61. Configuring Two PWM Modules forPhase Control

<!-- Page 2069 -->

0000FFFFh
TBPRDTBCTR[0-15]
timeCTR = PRD 
(SycnOut)Master Module
F2Phase = 120 °
0000FFFFh
TBPRDTBCTR[0-15]
timeSyncInSlave Module
TBPHS600 600
600 600
200 200
www.ti.com Application Examples
2069 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleFigure 35-62 shows theassociated timing waveforms forthisconfiguration. Here, TBPRD =600forboth
master andslave. Fortheslave, TBPHS =200(200/600 ×360°=120°).Whenever themaster generates
aSyncIn pulse (CTR =PRD), thevalue ofTBPHS =200isloaded intotheslave TBCTR register sothe
slave time-base isalways leading themaster's time-base by120°.
Figure 35-62. Timing Waveforms Associated With Phase Control Between 2Modules

<!-- Page 2070 -->

ePWM Registers www.ti.com
2070 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4 ePWM Registers
Table 35-22 lists thecomplete ePWM module control andstatus register setgrouped bysubmodule. Each
register setisduplicated foreach instance oftheePWM module. The base address forthecontrol
registers isFCF7 8C00h forePWM1, FCF7 8D00h forePWM2, FCF7 8E00h forePWM3, FCF7 8F00h for
ePWM4, FCF7 9000h forePWM5, FCF7 9100h forePWM6, andFCF7 9200h forePWM7.
Table 35-22. ePWM Module Control andStatus Register SetGrouped bySubmodule
Address Offset Name Description Section
Time-Base Submodule Registers
00h TBSTS Time-Base Status Register Section 35.4.1.1
02h TBCTL Time-Base Control Register Section 35.4.1.2
04h TBPHS Time-Base Phase Register Section 35.4.1.3
08h TBPRD Time-Base Period Register Section 35.4.1.4
0Ah TBCTR Time-Base Counter Register Section 35.4.1.5
Counter-Compare Submodule Registers
0Ch CMPCTL Counter-Compare Control Register Section 35.4.2.1
10h CMPA Counter-Compare ARegister Section 35.4.2.2
16h CMPB Counter-Compare BRegister Section 35.4.2.3
Action-Qualifier Submodule Registers
14h AQCTLA Action-Qualifier Control Register forOutput A(EPWMxA) Section 35.4.3.1
18h AQSFRC Action-Qualifier Software Force Register Section 35.4.3.2
1Ah AQCTLB Action-Qualifier Control Register forOutput B(EPWMxB) Section 35.4.3.3
1Eh AQCSFRC Action-Qualifier Continuous S/W Force Register Set Section 35.4.3.4
Dead-Band Generator Submodule Registers
1Ch DBCTL Dead-Band Generator Control Register Section 35.4.4.1
20h DBFED Dead-Band Generator Falling Edge Delay Count Register Section 35.4.4.2
22h DBRED Dead-Band Generator Rising Edge Delay Count Register Section 35.4.4.3
Trip-Zone Submodule Registers
24h TZDCSEL Trip Zone Digital Compare Event Select Register Section 35.4.5.1
26h TZSEL Trip-Zone Select Register Section 35.4.5.2
28h TZEINT Trip-Zone Enable Interrupt Register Section 35.4.5.3
2Ah TZCTL Trip-Zone Control Register Section 35.4.5.4
2Ch TZCLR Trip-Zone Clear Register Section 35.4.5.5
2Eh TZFLG Trip-Zone Flag Register Section 35.4.5.6
32h TZFRC Trip-Zone Force Register Section 35.4.5.7
Event-Trigger Submodule Registers
30h ETSEL Event-Trigger Selection Register Section 35.4.6.1
34h ETFLG Event-Trigger Flag Register Section 35.4.6.2
36h ETPS Event-Trigger Pre-Scale Register Section 35.4.6.3
38h ETFRC Event-Trigger Force Register Section 35.4.6.4
3Ah ETCLR Event-Trigger Clear Register Section 35.4.6.5
PWM-Chopper Submodule Registers
3Eh PCCTL PWM-Chopper Control Register Section 35.4.7.1
Digital Compare Event Registers
60h DCACTL Digital Compare AControl Register Section 35.4.8.1
62h DCTRIPSEL Digital Compare Trip Select Register Section 35.4.8.2
64h DCFCTL Digital Compare Filter Control Register Section 35.4.8.3
66h DCBCTL Digital Compare BControl Register Section 35.4.8.4
68h DCFOFFSET Digital Compare Filter Offset Register Section 35.4.8.5
6Ah DCCAPCTL Digital Compare Capture Control Register Section 35.4.8.6

<!-- Page 2071 -->

www.ti.com ePWM Registers
2071 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-22. ePWM Module Control andStatus Register SetGrouped bySubmodule (continued)
Address Offset Name Description Section
6Ch DCFWINDOW Digital Compare Filter Window Register Section 35.4.8.7
6Eh DCFOFFSETCNT Digital Compare Filter Offset Counter Register Section 35.4.8.8
70h DCCAP Digital Compare Counter Capture Register Section 35.4.8.9
72h DCFWINDOWCNT Digital Compare Filter Window Counter Register Section 35.4.8.10
35.4.1 Time-Base Submodule Registers
35.4.1.1 Time-Base Status Register (TBSTS)
Figure 35-63. Time-Base Status Register (TBSTS) [offset =00h]
15 8
Reserved
R-0
7 3 2 1 0
Reserved CTRMAX SYNCI CTRDIR
R-0 R/W1C-0 R/W1C-0 R-1
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 35-23. Time-Base Status Register (TBSTS) Field Descriptions
Bit Field Value Description
15-3 Reserved 0 Reserved
2 CTRMAX Time-Base Counter Max Latched Status Bit.
0 Read: Indicates thetime-base counter never reached itsmaximum value.
Write: Noeffect.
1 Read: Indicates thatthetime-base counter reached themaximum value 0xFFFF.
Write: Clears thelatched event.
1 SYNCI Input Synchronization Latched Status Bit.
0 Read: Indicates noexternal synchronization event hasoccurred.
Write: Noeffect.
1 Read: Indicates thatanexternal synchronization event hasoccurred (EPWMxSYNCI).
Write: Clears thelatched event.
0 CTRDIR Time-Base Counter Direction Status Bit.Atreset, thecounter isfrozen; therefore, thisbithasno
meaning. Tomake thisbitmeaningful, youmust firstsettheappropriate mode via
TBCTL[CTRMODE].
0 Time-Base Counter iscurrently counting down.
1 Time-Base Counter iscurrently counting up.

<!-- Page 2072 -->

ePWM Registers www.ti.com
2072 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.1.2 Time-Base Control Register (TBCTL)
Figure 35-64. Time-Base Control Register (TBCTL) [offset =02h]
15 14 13 12 10 9 8
FREE SOFT PHSDIR CLKDIV HSPCLKDIV
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
HSPCLKDIV SWFSYNC SYNCOSEL PRDLD PHSEN CTRMODE
R/W-1 R/W-0 R/W-0 R/W-0 R/W-0 R/W-3h
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-24. Time-Base Control Register (TBCTL) Field Descriptions
Bit Field Value Description
15-14 FREE, SOFT Emulation Mode Bits. These bitsselect thebehavior oftheePWM time-base counter during
emulation events:
0 Stop after thenext time-base counter increment ordecrement.
1h Stop when counter completes awhole cycle:
*Up-count mode: stop when thetime-base counter =period (TBCTR =TBPRD)
*Down-count mode: stop when thetime-base counter =0x0000 (TBCTR =0x0000)
*Up-down-count mode: stop when thetime-base counter =0x0000 (TBCTR =0x0000)
2h-3h Free run
13 PHSDIR Phase Direction Bit.
This bitisonly used when thetime-base counter isconfigured intheup-down-count mode. The
PHSDIR bitindicates thedirection thetime-base counter (TBCTR) willcount after asynchronization
event occurs andanew phase value isloaded from thephase (TBPHS) register. This is
irrespective ofthedirection ofthecounter before thesynchronization event..
Intheup-count anddown-count modes thisbitisignored.
0 Count down after thesynchronization event.
1 Count upafter thesynchronization event.
12-10 CLKDIV Time-base Clock Prescale Bits.
These bitsdetermine part ofthetime-base clock prescale value:
TBCLK =VCLK3 /(HSPCLKDIV ×CLKDIV)
0 /1(default onreset)
1h /2
2h /4
3h /8
4h /16
5h /32
6h /64
7h /128
9-7 HSPCLKDIV High Speed Time-base Clock Prescale Bits.
These bitsdetermine part ofthetime-base clock prescale value:
TBCLK =VCLK3 /(HSPCLKDIV ×CLKDIV)
0 /1
1h /2(default onreset)
2h /4
3h /6
4h /8
5h /10
6h /12
7h /14

<!-- Page 2073 -->

www.ti.com ePWM Registers
2073 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-24. Time-Base Control Register (TBCTL) Field Descriptions (continued)
Bit Field Value Description
6 SWFSYNC Software Forced Synchronization Pulse.
0 Writing a0hasnoeffect andreads always return a0.
1 Writing a1forces aone-time synchronization pulse tobegenerated.
This event isORed with theEPWMxSYNCI input oftheePWM module.
SWFSYNC isvalid (operates) only when EPWMxSYNCI isselected bySYNCOSEL =00.
5-4 SYNCOSEL Synchronization Output Select. These bitsselect thesource oftheEPWMxSYNCO signal.
0 EPWMxSYNC
1h CTR =zero: Time-base counter equal tozero (TBCTR =0x0000)
2h CTR =CMPB :Time-base counter equal tocounter-compare B(TBCTR =CMPB)
3h Disable EPWMxSYNCO signal
3 PRDLD Active Period Register Load From Shadow Register Select.
0 The period register (TBPRD) isloaded from itsshadow register when thetime-base counter,
TBCTR, isequal tozero.
Awrite orread totheTBPRD register accesses theshadow register.
1 Load theTBPRD register immediately without using ashadow register.
Awrite orread totheTBPRD register directly accesses theactive register.
2 PHSEN Counter Register Load From Phase Register Enable.
0 Donotload thetime-base counter (TBCTR) from thetime-base phase register (TBPHS).
1 Load thetime-base counter with thephase register when anEPWMxSYNCI input signal occurs or
when asoftware synchronization isforced bytheSWFSYNC bit,orwhen adigital compare sync
event occurs.
1-0 CTRMODE Counter Mode.
The time-base counter mode isnormally configured once andnotchanged during normal operation.
Ifyouchange themode ofthecounter, thechange willtake effect atthenext TBCLK edge andthe
current counter value shall increment ordecrement from thevalue before themode change.
These bitssetthetime-base counter mode ofoperation asfollows:
0 Up-count mode
1h Down-count mode
2h Up-down-count mode
3h Stop-freeze counter operation (default onreset)

<!-- Page 2074 -->

ePWM Registers www.ti.com
2074 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.1.3 Time-Base Phase Register (TBPHS)
Figure 35-65. Time-Base Phase Register (TBPHS) [offset =04h]
15 0
TBPHS
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-25. Time-Base Phase Register (TBPHS) Field Descriptions
Bits Name Description
15-0 TBPHS These bitssettime-base counter phase oftheselected ePWM relative tothetime-base thatissupplying
thesynchronization input signal.
*IfTBCTL[PHSEN] =0,then thesynchronization event isignored andthetime-base counter isnotloaded
with thephase.
*IfTBCTL[PHSEN] =1,then thetime-base counter (TBCTR) willbeloaded with thephase (TBPHS)
when asynchronization event occurs. The synchronization event canbeinitiated bytheinput
synchronization signal (EPWMxSYNCI) orbyasoftware forced synchronization.
35.4.1.4 Time-Base Period Register (TBPRD)
Figure 35-66. Time-Base Period Register (TBPRD) [offset =08h]
15 0
TBPRD
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-26. Time-Base Period Register (TBPRD) Field Descriptions
Bits Name Description
15-0 TBPRD These bitsdetermine theperiod ofthetime-base counter. This sets thePWM frequency.
Shadowing ofthisregister isenabled anddisabled bytheTBCTL[PRDLD] bit.Bydefault thisregister is
shadowed.
*IfTBCTL[PRDLD] =0,then theshadow isenabled andanywrite orread willautomatically gotothe
shadow register. Inthiscase, theactive register willbeloaded from theshadow register when thetime-
base counter equals 0.
*IfTBCTL[PRDLD] =1,then theshadow isdisabled andanywrite orread willgodirectly totheactive
register, thatistheregister actively controlling thehardware.
*The active andshadow registers share thesame memory map address.
35.4.1.5 Time-Base Counter Register (TBCTR)
Figure 35-67. Time-Base Counter Register (TBCTR) [offset =0Ah]
15 0
TBCTR
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-27. Time-Base Counter Register (TBCTR) Field Descriptions
Bits Name Description
15-0 TBCTR Reading these bitsgives thecurrent time-base counter value.
Writing tothese bitssets thecurrent time-base counter value. The update happens assoon asthewrite
occurs; thewrite isNOT synchronized tothetime-base clock (TBCLK) andtheregister isnotshadowed.

<!-- Page 2075 -->

www.ti.com ePWM Registers
2075 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.2 Counter-Compare Submodule Registers
35.4.2.1 Counter-Compare Control Register (CMPCTL)
Figure 35-68. Counter-Compare Control Register (CMPCTL) [offset =0Ch]
15 10 9 8
Reserved SHDWBFULL SHDWAFULL
R-0 R-0 R-0
7 6 5 4 3 2 1 0
Reserved SHDWBMODE Reserved SHDWAMODE LOADBMODE LOADAMODE
R-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-28. Counter-Compare Control Register (CMPCTL) Field Descriptions
Bits Name Value Description
15-10 Reserved 0 Reserved
9 SHDWBFULL Counter-compare B(CMPB) Shadow Register FullStatus Flag.
This bitselfclears once aload-strobe occurs.
0 CMPB shadow FIFO notfullyet.
1 Indicates theCMPB shadow FIFO isfull;aCPU write willoverwrite current shadow value.
8 SHDWAFULL Counter-compare A(CMPA) Shadow Register FullStatus Flag.
The flagbitissetwhen a32-bit write toCMPA:CMPAHR register ora16-bit write toCMPA
register ismade. A16-bit write toCMPAHR register willnotaffect theflag.
This bitselfclears once aload-strobe occurs.
0 CMPA shadow FIFO notfullyet.
1 Indicates theCMPA shadow FIFO isfull,aCPU write willoverwrite thecurrent shadow value.
7 Reserved 0 Reserved
6 SHDWBMODE Counter-compare B(CMPB) Register Operating Mode.
0 Shadow mode. Operates asadouble buffer. Allwrites viatheCPU access theshadow register.
1 Immediate mode. Only theactive compare Bregister isused. Allwrites andreads directly
access theactive register forimmediate compare action.
5 Reserved 0 Reserved
4 SHDWAMODE Counter-compare A(CMPA) Register Operating Mode.
0 Shadow mode. Operates asadouble buffer. Allwrites viatheCPU access theshadow register.
1 Immediate mode. Only theactive compare register isused. Allwrites andreads directly access
theactive register forimmediate compare action.
3-2 LOADBMODE Active Counter-Compare B(CMPB) Load From Shadow Select Mode.
This bithasnoeffect inimmediate mode (CMPCTL[SHDWBMODE] =1).
0 Load onCTR =Zero: Time-base counter equal tozero (TBCTR =0x0000)
1h Load onCTR =PRD: Time-base counter equal toperiod (TBCTR =TBPRD)
2h Load oneither CTR =Zero orCTR =PRD
3h Freeze (noloads possible)
1-0 LOADAMODE Active Counter-Compare A(CMPA) Load From Shadow Select Mode.
This bithasnoeffect inimmediate mode (CMPCTL[SHDWAMODE] =1).
0 Load onCTR =Zero: Time-base counter equal tozero (TBCTR =0x0000)
1h Load onCTR =PRD: Time-base counter equal toperiod (TBCTR =TBPRD)
2h Load oneither CTR =Zero orCTR =PRD
3h Freeze (noloads possible)

<!-- Page 2076 -->

ePWM Registers www.ti.com
2076 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.2.2 Counter-Compare ARegister (CMPA)
Figure 35-69. Counter-Compare ARegister (CMPA) [offset =10h]
15 0
CMPA
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-29. Counter-Compare ARegister (CMPA) Field Descriptions
Bits Name Description
15-0 CMPA The value intheactive CMPA register iscontinuously compared tothetime-base counter (TBCTR). When
thevalues areequal, thecounter-compare module generates a"time-base counter equal tocounter
compare A"event. This event issent totheaction-qualifier where itisqualified andconverted itintooneor
more actions. These actions canbeapplied toeither theEPWMxA ortheEPWMxB output depending on
theconfiguration oftheAQCTLA andAQCTLB registers. The actions thatcanbedefined intheAQCTLA
andAQCTLB registers include:
*Donothing; theevent isignored.
*Clear: PulltheEPWMxA and/or EPWMxB signal low.
*Set: PulltheEPWMxA and/or EPWMxB signal high.
*Toggle theEPWMxA and/or EPWMxB signal.
Shadowing ofthisregister isenabled anddisabled bytheCMPCTL[SHDWAMODE] bit.Bydefault this
register isshadowed.
*IfCMPCTL[SHDWAMODE] =0,then theshadow isenabled andanywrite orread willautomatically go
totheshadow register. Inthiscase, theCMPCTL[LOADAMODE] bitfield determines which event will
load theactive register from theshadow register.
*Before awrite, theCMPCTL[SHDWAFULL] bitcanberead todetermine iftheshadow register is
currently full.
*IfCMPCTL[SHDWAMODE] =1,then theshadow register isdisabled andanywrite orread willgo
directly totheactive register, thatistheregister actively controlling thehardware.
*Ineither mode, theactive andshadow registers share thesame memory map address.

<!-- Page 2077 -->

www.ti.com ePWM Registers
2077 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.2.3 Counter-Compare BRegister (CMPB)
Figure 35-70. Counter-Compare BRegister (CMPB) [offset =16h]
15 0
CMPB
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-30. Counter-Compare BRegister (CMPB) Field Descriptions
Bits Name Description
15-0 CMPB The value intheactive CMPB register iscontinuously compared tothetime-base counter (TBCTR). When
thevalues areequal, thecounter-compare module generates a"time-base counter equal tocounter
compare B"event. This event issent totheaction-qualifier where itisqualified andconverted itintooneor
more actions. These actions canbeapplied toeither theEPWMxA ortheEPWMxB output depending on
theconfiguration oftheAQCTLA andAQCTLB registers. The actions thatcanbedefined intheAQCTLA
andAQCTLB registers include:
*Donothing. event isignored.
*Clear: PulltheEPWMxA and/or EPWMxB signal low.
*Set: PulltheEPWMxA and/or EPWMxB signal high.
*Toggle theEPWMxA and/or EPWMxB signal.
Shadowing ofthisregister isenabled anddisabled bytheCMPCTL[SHDWBMODE] bit.Bydefault this
register isshadowed.
*IfCMPCTL[SHDWBMODE] =0,then theshadow isenabled andanywrite orread willautomatically go
totheshadow register. Inthiscase, theCMPCTL[LOADBMODE] bitfield determines which event will
load theactive register from theshadow register:
*Before awrite, theCMPCTL[SHDWBFULL] bitcanberead todetermine iftheshadow register is
currently full.
*IfCMPCTL[SHDWBMODE] =1,then theshadow register isdisabled andanywrite orread willgo
directly totheactive register, thatistheregister actively controlling thehardware.
*Ineither mode, theactive andshadow registers share thesame memory map address.

<!-- Page 2078 -->

ePWM Registers www.ti.com
2078 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.3 Action-Qualifier Submodule Registers
35.4.3.1 Action-Qualifier Output AControl Register (AQCTLA)
Figure 35-71. Action-Qualifier Output AControl Register (AQCTLA) [offset =14h]
15 12 11 10 9 8
Reserved CBD CBU
R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
CAD CAU PRD ZRO
R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-31. Action-Qualifier Output AControl Register (AQCTLA) Field Descriptions
Bits Name Value Description
15-12 Reserved 0 Reserved
11-10 CBD Action when thetime-base counter equals theactive CMPB register andthecounter is
decrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
9-8 CBU Action when thecounter equals theactive CMPB register andthecounter isincrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
7-6 CAD Action when thecounter equals theactive CMPA register andthecounter isdecrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
5-4 CAU Action when thecounter equals theactive CMPA register andthecounter isincrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
3-2 PRD Action when thecounter equals theperiod.
Note: Bydefinition, incount up-down mode when thecounter equals period thedirection isdefined
as0orcounting down.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
1-0 ZRO Action when counter equals zero.
Note: Bydefinition, incount up-down mode when thecounter equals 0thedirection isdefined as1
orcounting up.
0 Donothing (action isdisabled).
1h Clear: force EPWMxA output low.
2h Set: force EPWMxA output high.
3h Toggle EPWMxA output: lowoutput signal willbeforced high, andahigh signal willbeforced low.

<!-- Page 2079 -->

www.ti.com ePWM Registers
2079 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.3.2 Action-Qualifier Software Force Register (AQSFRC)
Figure 35-72. Action-Qualifier Software Force Register (AQSFRC) [offset =18h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
RLDCSF OTSFB ACTSFB OTSFA ACTSFA
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-32. Action-Qualifier Software Force Register (AQSFRC) Field Descriptions
Bit Field Value Description
15-8 Reserved 0 Reserved
7-6 RLDCSF AQCSFRC Active Register Reload From Shadow Options.
0 Load onevent counter equals zero.
1h Load onevent counter equals period.
2h Load onevent counter equals zero orcounter equals period.
3h Load immediately (the active register isdirectly accessed bytheCPU andisnotloaded from the
shadow register).
5 OTSFB One-Time Software Forced Event onOutput B.
0 Writing a0hasnoeffect. Always reads back a0.
This bitisauto cleared once awrite tothisregister iscomplete (that is,aforced event isinitiated.)
This isaone-shot forced event. Itcanbeoverridden byanother subsequent event onoutput B.
1 Initiates asingle s/wforced event.
4-3 ACTSFB Action when One-Time Software Force BIsinvoked.
0 Does nothing (action isdisabled).
1h Clear (low)
2h Set(high)
3h Toggle (Low ->High, High ->Low)
Note :This action isnotqualified bycounter direction (CNT_dir).
2 OTSFA One-Time Software Forced Event onOutput A.
0 Writing a0hasnoeffect. Always reads back a0.
This bitisauto cleared once awrite tothisregister iscomplete (that is,aforced event isinitiated).
1 Initiates asingle software forced event.
1-0 ACTSFA Action When One-Time Software Force AIsInvoked.
0 Does nothing (action isdisabled).
1h Clear (low)
2h Set(high)
3h Toggle (Low→High, High→Low)
Note :This action isnotqualified bycounter direction (CNT_dir).

<!-- Page 2080 -->

ePWM Registers www.ti.com
2080 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.3.3 Action-Qualifier Output BControl Register (AQCTLB)
Figure 35-73. Action-Qualifier Output BControl Register (AQCTLB) [offset =1Ah]
15 12 11 10 9 8
Reserved CBD CBU
R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
CAD CAU PRD ZRO
R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-33. Action-Qualifier Output BControl Register (AQCTLB) Field Descriptions
Bits Name Value Description
15-12 Reserved 0 Reserved
11-10 CBD Action when thecounter equals theactive CMPB register andthecounter isdecrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
9-8 CBU Action when thecounter equals theactive CMPB register andthecounter isincrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
7-6 CAD Action when thecounter equals theactive CMPA register andthecounter isdecrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
5-4 CAU Action when thecounter equals theactive CMPA register andthecounter isincrementing.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
3-2 PRD Action when thecounter equals theperiod.
Note: Bydefinition, incount up-down mode when thecounter equals period thedirection isdefined
as0orcounting down.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.
1-0 ZRO Action when counter equals zero.
Note: Bydefinition, incount up-down mode when thecounter equals 0thedirection isdefined as1
orcounting up.
0 Donothing (action isdisabled).
1h Clear: force EPWMxB output low.
2h Set: force EPWMxB output high.
3h Toggle EPWMxB output: lowoutput signal willbeforced high, andahigh signal willbeforced low.

<!-- Page 2081 -->

www.ti.com ePWM Registers
2081 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.3.4 Action-Qualifier Continuous Force Register (AQCSFRC)
Figure 35-74. Action-Qualifier Continuous Software Force Register (AQCSFRC) [offset =1Eh]
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved CSFB CSFA
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-34. Action-qualifier Continuous Software Force Register (AQCSFRC) Field Descriptions
Bits Name Value Description
15-4 Reserved 0 Reserved
3-2 CSFB Continuous Software Force onOutput B.
Inimmediate mode, acontinuous force takes effect onthenext TBCLK edge.
Inshadow mode, acontinuous force takes effect onthenext TBCLK edge after ashadow load into
theactive register. Toconfigure shadow mode, useAQSFRC[RLDCSF].
0 Forcing disabled, thatis,hasnoeffect.
1h Forces acontinuous lowonoutput B.
2h Forces acontinuous high onoutput.
3h Software forcing isdisabled andhasnoeffect.
1-0 CSFA Continuous Software Force onOutput A.
Inimmediate mode, acontinuous force takes effect onthenext TBCLK edge.
Inshadow mode, acontinuous force takes effect onthenext TBCLK edge after ashadow load into
theactive register.
0 Forcing disabled, thatis,hasnoeffect.
1h Forces acontinuous lowonoutput A.
2h Forces acontinuous high onoutput A.
3h Software forcing isdisabled andhasnoeffect.

<!-- Page 2082 -->

ePWM Registers www.ti.com
2082 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.4 Dead-Band Submodule Registers
35.4.4.1 Dead-Band Generator Control Register (DBCTL)
Figure 35-75. Dead-Band Generator Control Register (DBCTL) [offset =1Ch]
15 14 8
HALFCYCLE Reserved
R/W-0 R-0
7 6 5 4 3 2 1 0
Reserved IN_MODE POLSEL OUT_MODE
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-35. Dead-Band Generator Control Register (DBCTL) Field Descriptions
Bits Name Value Description
15 HALFCYCLE Half Cycle Clocking Enable Bit.
0 Fullcycle clocking enabled. The dead-band counters areclocked attheTBCLK rate.
1 Half cycle clocking enabled. The dead-band counters areclocked atTBCLK ×2.
14-6 Reserved 0 Reserved
5-4 IN_MODE Dead Band Input Mode Control.
Bit5controls theS5switch andbit4controls theS4switch shown inFigure 35-28 .
This allows youtoselect theinput source tothefalling-edge andrising-edge delay.
Toproduce classical dead-band waveforms thedefault isEPWMxA Inisthesource forboth falling
andrising-edge delays.
0 EPWMxA In(from theaction-qualifier) isthesource forboth falling-edge andrising-edge delay.
1h EPWMxB In(from theaction-qualifier) isthesource forrising-edge delayed signal.
EPWMxA In(from theaction-qualifier) isthesource forfalling-edge delayed signal.
2h EPWMxA In(from theaction-qualifier) isthesource forrising-edge delayed signal.
EPWMxB In(from theaction-qualifier) isthesource forfalling-edge delayed signal.
3h EPWMxB In(from theaction-qualifier) isthesource forboth rising-edge delay andfalling-edge
delayed signal.
3-2 POLSEL Polarity Select Control.
Bit3controls theS3switch andbit2controls theS2switch shown inFigure 35-28 .
This allows youtoselectively invert oneofthedelayed signals before itissent outofthedead-band
submodule.
The following descriptions correspond toclassical upper/lower switch control asfound inonelegof
adigital motor control inverter.
These assume thatDBCTL[OUT_MODE] =1,1andDBCTL[IN_MODE] =0,0.Other enhanced
modes arealso possible, butnotregarded astypical usage modes.
0 Active high (AH) mode. Neither EPWMxA norEPWMxB isinverted (default).
1h Active lowcomplementary (ALC) mode. EPWMxA isinverted.
2h Active high complementary (AHC). EPWMxB isinverted.
3h Active low(AL) mode. Both EPWMxA andEPWMxB areinverted.

<!-- Page 2083 -->

www.ti.com ePWM Registers
2083 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-35. Dead-Band Generator Control Register (DBCTL) Field Descriptions (continued)
Bits Name Value Description
1-0 OUT_MODE Dead-band Output Mode Control.
Bit1controls theS1switch andbit0controls theS0switch shown inFigure 35-28 .
This allows youtoselectively enable orbypass thedead-band generation forthefalling-edge and
rising-edge delay.
0 Dead-band generation isbypassed forboth output signals. Inthismode, both theEPWMxA and
EPWMxB output signals from theaction-qualifier arepassed directly tothePWM-chopper
submodule.
Inthismode, thePOLSEL andIN_MODE bitshave noeffect.
1h Disable rising-edge delay. The EPWMxA signal from theaction-qualifier ispassed straight through
totheEPWMxA input ofthePWM-chopper submodule.
The falling-edge delayed signal isseen onoutput EPWMxB. The input signal forthedelay is
determined byDBCTL[IN_MODE].
2h The rising-edge delayed signal isseen onoutput EPWMxA. The input signal forthedelay is
determined byDBCTL[IN_MODE].
Disable falling-edge delay. The EPWMxB signal from theaction-qualifier ispassed straight through
totheEPWMxB input ofthePWM-chopper submodule.
3h Dead-band isfully enabled forboth rising-edge delay onoutput EPWMxA andfalling-edge delay on
output EPWMxB. The input signal forthedelay isdetermined byDBCTL[IN_MODE].

<!-- Page 2084 -->

ePWM Registers www.ti.com
2084 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.4.2 Dead-Band Generator Falling Edge Delay Register (DBFED)
Figure 35-76. Dead-Band Generator Falling Edge Delay Register (DBFED) [offset =20h]
15 10 9 8
Reserved DEL
R-0 R/W-0
7 0
DEL
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-36. Dead-Band Generator Falling Edge Delay Register (DBFED) Field Descriptions
Bits Name Description
15-10 Reserved Reserved
9-0 DEL Falling Edge Delay Count. 10-bit counter.
35.4.4.3 Dead-Band Generator Rising Edge Delay Register (DBRED)
Figure 35-77. Dead-Band Generator Rising Edge Delay Register (DBRED) [offset =22h]
15 10 9 8
Reserved DEL
R-0 R/W-0
7 0
DEL
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-37. Dead-Band Generator Rising Edge Delay Register (DBRED) Field Descriptions
Bits Name Description
15-10 Reserved Reserved
9-0 DEL Rising Edge Delay Count. 10-bit counter.

<!-- Page 2085 -->

www.ti.com ePWM Registers
2085 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5 Trip-Zone Submodule Registers
35.4.5.1 Trip-Zone Digital Compare Event Select Register (TZDCSEL)
Figure 35-78. Trip Zone Digital Compare Event Select Register (TZDCSEL) [offset =24h]
15 12 11 9 8 6 5 3 2 0
Reserved DCBEVT2 DCBEVT1 DCAEVT2 DCAEVT1
R-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-38. Trip Zone Digital Compare Event Select Register (TZDCSEL) Field Descriptions
Bit Field Value Description
15-12 Reserved 0 Reserved
11-9 DCBEVT2 Digital Compare Output BEvent 2Selection.
0 Event isdisabled.
1h DCBH =low, DCBL =don'tcare
2h DCBH =high, DCBL =don'tcare
3h DCBL =low, DCBH =don'tcare
4h DCBL =high, DCBH =don'tcare
5h DCBL =high, DCBH =low
6h-7h Reserved
8-6 DCBEVT1 Digital Compare Output BEvent 1Selection.
0 Event isdisabled.
1h DCBH =low, DCBL =don'tcare
2h DCBH =high, DCBL =don'tcare
3h DCBL =low, DCBH =don'tcare
4h DCBL =high, DCBH =don'tcare
5h DCBL =high, DCBH =low
6h-7h Reserved
5-3 DCAEVT2 Digital Compare Output AEvent 2Selection.
0 Event isdisabled.
1h DCAH =low, DCAL =don'tcare
2h DCAH =high, DCAL =don'tcare
3h DCAL =low, DCAH =don'tcare
4h DCAL =high, DCAH =don'tcare
5h DCAL =high, DCAH =low
6h-7h Reserved
2-0 DCAEVT1 Digital Compare Output AEvent 1Selection.
0 Event isdisabled.
1h DCAH =low, DCAL =don'tcare
2h DCAH =high, DCAL =don'tcare
3h DCAL =low, DCAH =don'tcare
4h DCAL =high, DCAH =don'tcare
5h DCAL =high, DCAH =low
6h-7h Reserved

<!-- Page 2086 -->

ePWM Registers www.ti.com
2086 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.2 Trip-Zone Select Register (TZSEL)
Figure 35-79. Trip-Zone Select Register (TZSEL) [offset =26h]
15 14 13 12 11 10 9 8
DCBEVT1 DCAEVT1 OSHT6 OSHT5 OSHT4 OSHT3 OSHT2 OSHT1
R-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
DCBEVT2 DCAEVT2 CBC6 CBC5 CBC4 CBC3 CBC2 CBC1
R-0 R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-39. Trip-Zone Submodule Select Register (TZSEL) Field Descriptions
Bits Name Value Description
One-Shot (OSHT) Trip-zone enable/disable. When anyoftheenabled pins golow, aone-shot tripevent occurs forthisePWM
module. When theevent occurs, theaction defined intheTZCTL register istaken ontheEPWMxA andEPWMxB outputs. The
one-shot tripcondition remains latched until theuser clears thecondition viatheTZCLR register.
15 DCBEVT1 Digital Compare Output BEvent 1Select.
0 Disable DCBEVT1 asone-shot-trip source forthisePWM module.
1 Enable DCBEVT1 asone-shot-trip source forthisePWM module.
14 DCAEVT1 Digital Compare Output AEvent 1Select.
0 Disable DCAEVT1 asone-shot-trip source forthisePWM module.
1 Enable DCAEVT1 asone-shot-trip source forthisePWM module.
13 OSHT6 Trip-zone 6(TZ6) Select.
0 Disable TZ6 asaone-shot tripsource forthisePWM module.
1 Enable TZ6 asaone-shot tripsource forthisePWM module.
12 OSHT5 Trip-zone 5(TZ5) Select.
0 Disable TZ5 asaone-shot tripsource forthisePWM module.
1 Enable TZ5 asaone-shot tripsource forthisePWM module.
11 OSHT4 Trip-zone 4(TZ4) Select.
0 Disable TZ4 asaone-shot tripsource forthisePWM module.
1 Enable TZ4 asaone-shot tripsource forthisePWM module.
10 OSHT3 Trip-zone 3(TZ3) Select.
0 Disable TZ3 asaone-shot tripsource forthisePWM module.
1 Enable TZ3 asaone-shot tripsource forthisePWM module.
9 OSHT2 Trip-zone 2(TZ2) Select.
0 Disable TZ2 asaone-shot tripsource forthisePWM module.
1 Enable TZ2 asaone-shot tripsource forthisePWM module.
8 OSHT1 Trip-zone 1(TZ1) Select.
0 Disable TZ1 asaone-shot tripsource forthisePWM module.
1 Enable TZ1 asaone-shot tripsource forthisePWM module.
Cycle-by-Cycle (CBC) Trip-zone enable/disable. When anyoftheenabled pins golow, acycle-by-cycle tripevent occurs forthis
ePWM module. When theevent occurs, theaction defined intheTZCTL register istaken ontheEPWMxA andEPWMxB outputs. A
cycle-by-cycle tripcondition isautomatically cleared when thetime-base counter reaches zero.
7 DCBEVT2 Digital Compare Output BEvent 2Select.
0 Disable DCBEVT2 asaCBC tripsource forthisePWM module.
1 Enable DCBEVT2 asaCBC tripsource forthisePWM module.
6 DCAEVT2 Digital Compare Output AEvent 2Select.
0 Disable DCAEVT2 asaCBC tripsource forthisePWM module.
1 Enable DCAEVT2 asaCBC tripsource forthisePWM module.

<!-- Page 2087 -->

www.ti.com ePWM Registers
2087 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-39. Trip-Zone Submodule Select Register (TZSEL) Field Descriptions (continued)
Bits Name Value Description
5 CBC6 Trip-zone 6(TZ6) Select.
0 Disable TZ6 asaCBC tripsource forthisePWM module.
1 Enable TZ6 asaCBC tripsource forthisePWM module.
4 CBC5 Trip-zone 5(TZ5) Select.
0 Disable TZ5 asaCBC tripsource forthisePWM module.
1 Enable TZ5 asaCBC tripsource forthisePWM module.
3 CBC4 Trip-zone 4(TZ4) Select.
0 Disable TZ4 asaCBC tripsource forthisePWM module.
1 Enable TZ4 asaCBC tripsource forthisePWM module.
2 CBC3 Trip-zone 3(TZ3) Select.
0 Disable TZ3 asaCBC tripsource forthisePWM module.
1 Enable TZ3 asaCBC tripsource forthisePWM module.
1 CBC2 Trip-zone 2(TZ2) Select.
0 Disable TZ2 asaCBC tripsource forthisePWM module.
1 Enable TZ2 asaCBC tripsource forthisePWM module.
0 CBC1 Trip-zone 1(TZ1) Select.
0 Disable TZ1 asaCBC tripsource forthisePWM module.
1 Enable TZ1 asaCBC tripsource forthisePWM module.

<!-- Page 2088 -->

ePWM Registers www.ti.com
2088 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.3 Trip-Zone Enable Interrupt Register (TZEINT)
Figure 35-80. Trip-Zone Enable Interrupt Register (TZEINT) [offset =28h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved DCBEVT2 DCBEVT1 DCAEVT2 DCAEVT1 OST CBC Reserved
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-40. Trip-Zone Enable Interrupt Register (TZEINT) Field Descriptions
Bits Name Value Description
15-3 Reserved 0 Reserved
6 DCBEVT2 Digital Comparator Output BEvent 2Interrupt Enable.
0 Disabled
1 Enabled
5 DCBEVT1 Digital Comparator Output BEvent 1Interrupt Enable.
0 Disabled
1 Enabled
4 DCAEVT2 Digital Comparator Output AEvent 2Interrupt Enable.
0 Disabled
1 Enabled
3 DCAEVT1 Digital Comparator Output AEvent 1Interrupt Enable.
0 Disabled
1 Enabled
2 OST Trip-zone One-Shot Interrupt Enable.
0 Disable one-shot interrupt generation.
1 Enable Interrupt generation; aone-shot tripevent willcause aEPWMx_TZINT VIM interrupt.
1 CBC Trip-zone Cycle-by-Cycle Interrupt Enable.
0 Disable cycle-by-cycle interrupt generation.
1 Enable interrupt generation; acycle-by-cycle tripevent willcause anEPWMx_TZINT VIM interrupt.
0 Reserved 0 Reserved

<!-- Page 2089 -->

www.ti.com ePWM Registers
2089 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.4 Trip-Zone Control Register (TZCTL)
Figure 35-81. Trip-Zone Control Register (TZCTL) [offset =2Ah]
15 12 11 10 9 8
Reserved DCBEVT2 DCBEVT1
R-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
DCAEVT2 DCAEVT1 TZB TZA
R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-41. Trip-Zone Control Register (TZCTL) Field Descriptions
Bit Field Value Description
15-12 Reserved 0 Reserved
11-10 DCBEVT2 Digital Compare Output BEvent 2Action OnEPWMxB.
0 High-impedance (EPWMxB =High-impedance state).
1h Force EPWMxB toahigh state.
2h Force EPWMxB toalowstate.
3h DoNothing, tripaction isdisabled.
9-8 DCBEVT1 Digital Compare Output BEvent 1Action OnEPWMxB.
0 High-impedance (EPWMxB =High-impedance state).
1h Force EPWMxB toahigh state.
2h Force EPWMxB toalowstate.
3h DoNothing, tripaction isdisabled.
7-6 DCAEVT2 Digital Compare Output AEvent 2Action OnEPWMxA.
0 High-impedance (EPWMxA =High-impedance state).
1h Force EPWMxA toahigh state.
2h Force EPWMxA toalowstate.
3h DoNothing, tripaction isdisabled.
5-4 DCAEVT1 Digital Compare Output AEvent 1Action OnEPWMxA.
0 High-impedance (EPWMxA =High-impedance state).
1h Force EPWMxA toahigh state.
2h Force EPWMxA toalowstate.
3h DoNothing, tripaction isdisabled.
3-2 TZB When atripevent occurs thefollowing action istaken onoutput EPWMxB. Which trip-zone pins can
cause anevent isdefined intheTZSEL register.
0 High-impedance (EPWMxB =High-impedance state).
1h Force EPWMxB toahigh state.
2h Force EPWMxB toalowstate.
3h Donothing, noaction istaken onEPWMxB.
1-0 TZA When atripevent occurs thefollowing action istaken onoutput EPWMxA. Which trip-zone pins can
cause anevent isdefined intheTZSEL register.
0 High-impedance (EPWMxA =High-impedance state).
1h Force EPWMxA toahigh state.
2h Force EPWMxA toalowstate.
3h Donothing, noaction istaken onEPWMxA.

<!-- Page 2090 -->

ePWM Registers www.ti.com
2090 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.5 Trip-Zone Clear Register (TZCLR)
Figure 35-82. Trip-Zone Clear Register (TZCLR) [offset =2Ch]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved DCBEVT2 DCBEVT1 DCAEVT2 DCAEVT1 OST CBC INT
R-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W1C-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 35-42. Trip-Zone Clear Register (TZCLR) Field Descriptions
Bit Field Value Description
15-7 Reserved 0 Reserved
6 DCBEVT2 Clear Flag forDigital Compare Output BEvent 2.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1clears theDCBEVT2 event tripcondition.
5 DCBEVT1 Clear Flag forDigital Compare Output BEvent 1.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1clears theDCBEVT1 event tripcondition.
4 DCAEVT2 Clear Flag forDigital Compare Output AEvent 2.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1clears theDCAEVT2 event tripcondition.
3 DCAEVT1 Clear Flag forDigital Compare Output AEvent 1.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1clears theDCAEVT1 event tripcondition.
2 OST Clear Flag forOne-Shot Trip (OST) Latch.
0 Has noeffect. Always reads back a0.
1 Clears thisTrip (set) condition.
1 CBC Clear Flag forCycle-By-Cycle (CBC) Trip Latch.
0 Has noeffect. Always reads back a0.
1 Clears thisTrip (set) condition.
0 INT Global Interrupt Clear Flag.
0 Has noeffect. Always reads back a0.
1 Clears thetrip-interrupt flagforthisePWM module (TZFLG[INT]).
NOTE: Nofurther EPWMx_TZINT VIM interrupts willbegenerated until theflagiscleared. Ifthe
TZFLG[INT] bitiscleared andanyoftheother flagbitsareset,then another interrupt pulse willbe
generated. Clearing allflagbitswillprevent further interrupts.

<!-- Page 2091 -->

www.ti.com ePWM Registers
2091 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.6 Trip-Zone Flag Register (TZFLG)
Figure 35-83. Trip-Zone Flag Register (TZFLG) [offset =2Eh]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved DCBEVT2 DCBEVT1 DCAEVT2 DCAEVT1 OST CBC INT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 35-43. Trip-Zone Flag Register (TZFLG) Field Descriptions
Bit Field Value Description
15-7 Reserved 0 Reserved
6 DCBEVT2 Latched Status Flag forDigital Compare Output BEvent 2.
0 Notripevent hasoccurred onDCBEVT2.
1 Atripevent hasoccurred fortheevent defined forDCBEVT2.
5 DCBEVT1 Latched Status Flag forDigital Compare Output BEvent 1.
0 Notripevent hasoccurred onDCBEVT1.
1 Atripevent hasoccurred fortheevent defined forDCBEVT1.
4 DCAEVT2 Latched Status Flag forDigital Compare Output AEvent 2.
0 Notripevent hasoccurred onDCAEVT2.
1 Atripevent hasoccurred fortheevent defined forDCAEVT2.
3 DCAEVT1 Latched Status Flag forDigital Compare Output AEvent 1.
0 Notripevent hasoccurred onDCAEVT1.
1 Atripevent hasoccurred fortheevent defined forDCAEVT1.
2 OST Latched Status Flag forAOne-Shot Trip Event.
0 Noone-shot tripevent hasoccurred.
1 Atripevent hasoccurred onapinselected asaone-shot tripsource.
This bitiscleared bywriting theappropriate value totheTZCLR register.
1 CBC Latched Status Flag forCycle-By-Cycle Trip Event.
0 Nocycle-by-cycle tripevent hasoccurred.
1 Atripevent hasoccurred onasignal selected asacycle-by-cycle tripsource. The TZFLG[CBC] bit
willremain setuntil itismanually cleared bytheuser. Ifthecycle-by-cycle tripevent isstillpresent
when theCBC bitiscleared, then CBC willbeimmediately setagain. The specified condition on
thesignal isautomatically cleared when theePWM time-base counter reaches zero (TBCTR =
0x0000) ifthetripcondition isnolonger present. The condition onthesignal isonly cleared when
theTBCTR =0x0000 nomatter where inthecycle theCBC flagiscleared.
This bitiscleared bywriting theappropriate value totheTZCLR register.
0 INT Latched Trip Interrupt Status Flag.
0 Nointerrupt hasbeen generated.
1 AnEPWMx_TZINT VIM interrupt was generated because ofatripcondition.
Nofurther EPWMx_TZINT VIM interrupts willbegenerated until thisflagiscleared. Iftheinterrupt
flagiscleared when either CBC orOST isset,then another interrupt pulse willbegenerated.
Clearing allflagbitswillprevent further interrupts.
This bitiscleared bywriting theappropriate value totheTZCLR register.

<!-- Page 2092 -->

ePWM Registers www.ti.com
2092 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.5.7 Trip-Zone Force Register (TZFRC)
Figure 35-84. Trip-Zone Force Register (TZFRC) [offset =32h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved DCBEVT2 DCBEVT1 DCAEVT2 DCAEVT1 OST CBC Reserved
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-44. Trip-Zone Force Register (TZFRC) Field Descriptions
Bits Name Value Description
15-7 Reserved 0 Reserved
6 DCBEVT2 Force Flag forDigital Compare Output BEvent 2.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1forces theDCBEVT2 event tripcondition andsets theTZFLG[DCBEVT2] bit.
5 DCBEVT1 Force Flag forDigital Compare Output BEvent 1.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1forces theDCBEVT1 event tripcondition andsets theTZFLG[DCBEVT1] bit.
4 DCAEVT2 Force Flag forDigital Compare Output AEvent 2.
0 Writing 0hasnoeffect. This bitalways reads back 0.
1 Writing 1forces theDCAEVT2 event tripcondition andsets theTZFLG[DCAEVT2] bit.
3 DCAEVT1 Force Flag forDigital Compare Output AEvent 1.
0 Writing 0hasnoeffect. This bitalways reads back 0
1 Writing 1forces theDCAEVT1 event tripcondition andsets theTZFLG[DCAEVT1] bit.
2 OST Force aOne-Shot Trip Event viaSoftware.
0 Writing of0isignored. Always reads back a0.
1 Forces aone-shot tripevent andsets theTZFLG[OST] bit.
1 CBC Force aCycle-by-Cycle Trip Event viaSoftware.
0 Writing of0isignored. Always reads back a0.
1 Forces acycle-by-cycle tripevent andsets theTZFLG[CBC] bit.
0 Reserved 0 Reserved

<!-- Page 2093 -->

www.ti.com ePWM Registers
2093 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.6 Event-Trigger Submodule Registers
35.4.6.1 Event-Trigger Selection Register (ETSEL)
Figure 35-85. Event-Trigger Selection Register (ETSEL) [offset =30h]
15 14 12 11 10 8
SOCBEN SOCBSEL SOCAEN SOCASEL
R/W-0 R/W-0 R/W-0 R/W-0
7 4 3 2 0
Reserved INTEN INTSEL
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-45. Event-Trigger Selection Register (ETSEL) Field Descriptions
Bits Name Value Description
15 SOCBEN Enable theADC Start ofConversion B(EPWMxSOCB) Pulse.
0 Disable EPWMxSOCB.
1 Enable EPWMxSOCB pulse.
14-12 SOCBSEL EPWMxSOCB Selection Options.
These bitsdetermine when aEPWMxSOCB pulse willbegenerated.
0 Enable DCBEVT1.soc event.
1h Enable event time-base counter equal tozero. (TBCTR =0x0000).
2h Enable event time-base counter equal toperiod (TBCTR =TBPRD).
3h Enable event time-base counter equal tozero orperiod (TBCTR =0x0000 orTBCTR =TBPRD).
This mode isuseful inup-down count mode.
4h Enable event time-base counter equal toCMPA when thetimer isincrementing.
5h Enable event time-base counter equal toCMPA when thetimer isdecrementing.
6h Enable event: time-base counter equal toCMPB when thetimer isincrementing.
7h Enable event: time-base counter equal toCMPB when thetimer isdecrementing.
11 SOCAEN Enable theADC Start ofConversion A(EPWMxSOCA) Pulse.
0 Disable EPWMxSOCA.
1 Enable EPWMxSOCA pulse.
10-8 SOCASEL EPWMxSOCA Selection Options.
These bitsdetermine when aEPWMxSOCA pulse willbegenerated.
0 Enable DCAEVT1.soc event.
1h Enable event time-base counter equal tozero. (TBCTR =0x0000).
2h Enable event time-base counter equal toperiod (TBCTR =TBPRD).
3h Enable event time-base counter equal tozero orperiod (TBCTR =0x0000 orTBCTR =TBPRD).
This mode isuseful inup-down count mode.
4h Enable event time-base counter equal toCMPA when thetimer isincrementing.
5h Enable event time-base counter equal toCMPA when thetimer isdecrementing.
6h Enable event: time-base counter equal toCMPB when thetimer isincrementing.
7h Enable event: time-base counter equal toCMPB when thetimer isdecrementing.
7-4 Reserved 0 Reserved
3 INTEN Enable ePWM Interrupt (EPWMx_INT) Generation.
0 Disable EPWMx_INT generation.
1 Enable EPWMx_INT generation.

<!-- Page 2094 -->

ePWM Registers www.ti.com
2094 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-45. Event-Trigger Selection Register (ETSEL) Field Descriptions (continued)
Bits Name Value Description
2-0 INTSEL ePWM Interrupt (EPWMx_INT) Selection Options.
0 Reserved
1h Enable event time-base counter equal tozero. (TBCTR =0x0000).
2h Enable event time-base counter equal toperiod (TBCTR =TBPRD).
3h Enable event time-base counter equal tozero orperiod (TBCTR =0x0000 orTBCTR =TBPRD).
This mode isuseful inup-down count mode.
4h Enable event time-base counter equal toCMPA when thetimer isincrementing.
5h Enable event time-base counter equal toCMPA when thetimer isdecrementing.
6h Enable event: time-base counter equal toCMPB when thetimer isincrementing.
7h Enable event: time-base counter equal toCMPB when thetimer isdecrementing.
35.4.6.2 Event-Trigger Flag Register (ETFLG)
Figure 35-86. Event-Trigger Flag Register (ETFLG) [offset =34h]
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved SOCB SOCA Reserved INT
R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 35-46. Event-Trigger Flag Register (ETFLG) Field Descriptions
Bits Name Value Description
15-4 Reserved 0 Reserved
3 SOCB Latched ePWM ADC Start-of-Conversion B(EPWMxSOCB) Status Flag.
0 NoEPWMxSOCB event occurred.
1 Astart ofconversion pulse was generated onEPWMxSOCB. The EPWMxSOCB output will
continue tobegenerated even iftheflagbitisset.
2 SOCA Latched ePWM ADC Start-of-Conversion A(EPWMxSOCA) Status Flag.
Unlike theETFLG[INT] flag, theEPWMxSOCA output willcontinue topulse even iftheflagbitis
set.
0 Noevent occurred.
1 Astart ofconversion pulse was generated onEPWMxSOCA. The EPWMxSOCA output will
continue tobegenerated even iftheflagbitisset.
1 Reserved 0 Reserved
0 INT Latched ePWM Interrupt (EPWMx_INT) Status Flag.
0 Noevent occurred.
1 AnePWMx interrupt (EWPMx_INT) was generated. Nofurther interrupts willbegenerated until the
flagbitiscleared. Uptooneinterrupt canbepending while theETFLG[INT] bitisstillset.Ifan
interrupt ispending, itwillnotbegenerated until after theETFLG[INT] bitiscleared. Refer to
Figure 35-41 .

<!-- Page 2095 -->

www.ti.com ePWM Registers
2095 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.6.3 Event-Trigger Prescale Register (ETPS)
Figure 35-87. Event-Trigger Prescale Register (ETPS) [offset =36h]
15 14 13 12 11 10 9 8
SOCBCNT SOCBPRD SOCACNT SOCAPRD
R-0 R/W-0 R-0 R/W-0
7 4 3 2 1 0
Reserved INTCNT INTPRD
R-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-47. Event-Trigger Prescale Register (ETPS) Field Descriptions
Bits Name Value Description
15-14 SOCBCNT ePWM ADC Start-of-Conversion BEvent (EPWMxSOCB) Counter Register.
These bitsindicate how many selected ETSEL[SOCBSEL] events have occurred.
0 Noevents have occurred.
1h 1event hasoccurred.
2h 2events have occurred.
3h 3events have occurred.
13-12 SOCBPRD ePWM ADC Start-of-Conversion BEvent (EPWMxSOCB) Period Select.
These bitsdetermine how many selected ETSEL[SOCBSEL] events need tooccur before an
EPWMxSOCB pulse isgenerated. Tobegenerated, thepulse must beenabled (ETSEL[SOCBEN]
=1).The SOCB pulse willbegenerated even ifthestatus flagissetfrom aprevious start of
conversion (ETFLG[SOCB] =1).Once theSOCB pulse isgenerated, theETPS[SOCBCNT] bitswill
automatically becleared.
0 Disable theSOCB event counter. NoEPWMxSOCB pulse willbegenerated.
1h Generate theEPWMxSOCB pulse onthefirstevent: ETPS[SOCBCNT] =0,1.
2h Generate theEPWMxSOCB pulse onthesecond event: ETPS[SOCBCNT] =1,0.
3h Generate theEPWMxSOCB pulse onthethird event: ETPS[SOCBCNT] =1,1.
11-10 SOCACNT ePWM ADC Start-of-Conversion AEvent (EPWMxSOCA) Counter Register.
These bitsindicate how many selected ETSEL[SOCASEL] events have occurred.
0 Noevents have occurred.
1h 1event hasoccurred.
2h 2events have occurred.
3h 3events have occurred.
9-8 SOCAPRD ePWM ADC Start-of-Conversion AEvent (EPWMxSOCA) Period Select.
These bitsdetermine how many selected ETSEL[SOCASEL] events need tooccur before an
EPWMxSOCA pulse isgenerated. Tobegenerated, thepulse must beenabled (ETSEL[SOCAEN]
=1).The SOCA pulse willbegenerated even ifthestatus flagissetfrom aprevious start of
conversion (ETFLG[SOCA] =1).Once theSOCA pulse isgenerated, theETPS[SOCACNT] bitswill
automatically becleared.
0 Disable theSOCA event counter. NoEPWMxSOCA pulse willbegenerated.
1h Generate theEPWMxSOCA pulse onthefirstevent: ETPS[SOCACNT] =0,1.
2h Generate theEPWMxSOCA pulse onthesecond event: ETPS[SOCACNT] =1,0.
3h Generate theEPWMxSOCA pulse onthethird event: ETPS[SOCACNT] =1,1.
7-4 Reserved 0 Reserved

<!-- Page 2096 -->

ePWM Registers www.ti.com
2096 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-47. Event-Trigger Prescale Register (ETPS) Field Descriptions (continued)
Bits Name Value Description
3-2 INTCNT ePWM Interrupt Event (EPWMx_INT) Counter Register.
These bitsindicate how many selected ETSEL[INTSEL] events have occurred. These bitsare
automatically cleared when aninterrupt pulse isgenerated. Ifinterrupts aredisabled, ETSEL[INT] =
0ortheinterrupt flagisset,ETFLG[INT] =1,thecounter willstop counting events when itreaches
theperiod value ETPS[INTCNT] =ETPS[INTPRD].
0 Noevents have occurred.
1h 1event hasoccurred.
2h 2events have occurred.
3h 3events have occurred.
1-0 INTPRD ePWM Interrupt (EPWMx_INT) Period Select.
These bitsdetermine how many selected ETSEL[INTSEL] events need tooccur before aninterrupt
isgenerated. Tobegenerated, theinterrupt must beenabled (ETSEL[INT] =1).Iftheinterrupt
status flagissetfrom aprevious interrupt (ETFLG[INT] =1)then nointerrupt willbegenerated until
theflagiscleared viatheETCLR[INT] bit.This allows foroneinterrupt tobepending while another
isstillbeing serviced. Once theinterrupt isgenerated, theETPS[INTCNT] bitswillautomatically be
cleared.
Writing aINTPRD value thatisthesame asthecurrent counter value willtrigger aninterrupt ifitis
enabled andthestatus flagisclear.
Writing aINTPRD value thatisless than thecurrent counter value willresult inanundefined state.
Ifacounter event occurs atthesame instant asanew zero ornon-zero INTPRD value iswritten,
thecounter isincremented.
0 Disable theinterrupt event counter. Nointerrupt willbegenerated andETFRC[INT] isignored.
1h Generate aninterrupt onthefirstevent INTCNT =01(first event).
2h Generate interrupt onETPS[INTCNT] =1,0(second event).
3h Generate interrupt onETPS[INTCNT] =1,1(third event).

<!-- Page 2097 -->

www.ti.com ePWM Registers
2097 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.6.4 Event-Trigger Force Register (ETFRC)
Figure 35-88. Event-Trigger Force Register (ETFRC) [offset =38h]
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved SOCB SOCA Reserved INT
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-48. Event-Trigger Force Register (ETFRC) Field Descriptions
Bits Name Value Description
15-4 Reserved 0 Reserved
3 SOCB SOCB Force Bit.The SOCB pulse willonly begenerated iftheevent isenabled intheETSEL
register. The ETFLG[SOCB] flagbitwillbesetregardless.
0 Has noeffect. Always reads back a0.
1 Generates apulse onEPWMxSOCB andsets theSOCBFLG bit.This bitisused fortestpurposes.
2 SOCA SOCA Force Bit.The SOCA pulse willonly begenerated iftheevent isenabled intheETSEL
register. The ETFLG[SOCA] flagbitwillbesetregardless.
0 Writing 0tothisbitwillbeignored. Always reads back a0.
1 Generates apulse onEPWMxSOCA andsettheSOCAFLG bit.This bitisused fortestpurposes.
1 Reserved 0 Reserved
0 INT INTForce Bit.The interrupt willonly begenerated iftheevent isenabled intheETSEL register.
The INTflagbitwillbesetregardless.
0 Writing 0tothisbitwillbeignored. Always reads back a0.
1 Generates aninterrupt onEPWMxINT andsettheINTflagbit.This bitisused fortestpurposes.

<!-- Page 2098 -->

ePWM Registers www.ti.com
2098 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.6.5 Event-Trigger Clear Register (ETCLR)
Figure 35-89. Event-Trigger Clear Register (ETCLR) [offset =3Ah]
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved SOCB SOCA Reserved INT
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-49. Event-Trigger Clear Register (ETCLR) Field Descriptions
Bits Name Value Description
15-4 Reserved 0 Reserved
3 SOCB ePWM ADC Start-of-Conversion B(EPWMxSOCB) Flag Clear Bit.
0 Writing a0hasnoeffect. Always reads back a0.
1 Clears theETFLG[SOCB] flagbit.
2 SOCA ePWM ADC Start-of-Conversion A(EPWMxSOCA) Flag Clear Bit.
0 Writing a0hasnoeffect. Always reads back a0.
1 Clears theETFLG[SOCA] flagbit.
1 Reserved 0 Reserved
0 INT ePWM Interrupt (EPWMx_INT) Flag Clear Bit.
0 Writing a0hasnoeffect. Always reads back a0.
1 Clears theETFLG[INT] flagbitandenable further interrupts pulses tobegenerated.

<!-- Page 2099 -->

www.ti.com ePWM Registers
2099 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.7 PWM-Chopper Submodule Register
35.4.7.1 PWM-Chopper Control Register (PCCTL)
Figure 35-90. PWM-Chopper Control Register (PCCTL) [offset =3Eh]
15 11 10 8
Reserved CHPDUTY
R-0 R/W-0
7 5 4 1 0
CHPFREQ OSHTWTH CHPEN
R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-50. PWM-Chopper Control Register (PCCTL) BitDescriptions
Bits Name Value Description
15-11 Reserved 0 Reserved
10-8 CHPDUTY Chopping Clock Duty Cycle.
0 Duty =1/8(12.5%)
1h Duty =2/8(25.0%)
2h Duty =3/8(37.5%)
3h Duty =4/8(50.0%)
4h Duty =5/8(62.5%)
5h Duty =6/8(75.0%)
6h Duty =7/8(87.5%)
7h Reserved
7-5 CHPFREQ Chopping Clock Frequency.
0 Divide by1(noprescale, =12.5 MHz at100MHz VCLK3)
1h Divide by2(6.25 MHz at100MHz VCLK3)
2h Divide by3(4.16 MHz at100MHz VCLK3)
3h Divide by4(3.12 MHz at100MHz VCLK3)
4h Divide by5(2.50 MHz at100MHz VCLK3)
5h Divide by6(2.08 MHz at100MHz VCLK3)
6h Divide by7(1.78 MHz at100MHz VCLK3)
7h Divide by8(1.56 MHz at100MHz VCLK3)

<!-- Page 2100 -->

ePWM Registers www.ti.com
2100 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) ModuleTable 35-50. PWM-Chopper Control Register (PCCTL) BitDescriptions (continued)
Bits Name Value Description
4-1 OSHTWTH One-Shot Pulse Width.
0 1xVCLK3 /8wide (=80nSat100MHz VCLK3)
1h 2xVCLK3 /8wide (=160nSat100MHz VCLK3)
2h 3xVCLK3 /8wide (=240nSat100MHz VCLK3)
3h 4xVCLK3 /8wide (=320nSat100MHz VCLK3)
4h 5xVCLK3 /8wide (=400nSat100MHz VCLK3)
5h 6xVCLK3 /8wide (=480nSat100MHz VCLK3)
6h 7xVCLK3 /8wide (=560nSat100MHz VCLK3)
7h 8xVCLK3 /8wide (=640nSat100MHz VCLK3)
8h 9xVCLK3 /8wide (=720nSat100MHz VCLK3)
9h 10xVCLK3 /8wide (=800nSat100MHz VCLK3)
Ah 11xVCLK3 /8wide (=880nSat100MHz VCLK3)
Bh 12xVCLK3 /8wide (=960nSat100MHz VCLK3)
Ch 13xVCLK3 /8wide (=1040 nSat100MHz VCLK3)
Dh 14xVCLK3 /8wide (=1120 nSat100MHz VCLK3)
Eh 15xVCLK3 /8wide (=1200 nSat100MHz VCLK3)
Fh 16xVCLK3 /8wide (=1280 nSat100MHz VCLK3)
0 CHPEN PWM-chopping Enable.
0 Disable (bypass) PWM chopping function.
1 Enable chopping function.

<!-- Page 2101 -->

www.ti.com ePWM Registers
2101 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8 Digital Compare Submodule Registers
35.4.8.1 Digital Compare AControl Register (DCACTL)
Figure 35-91. Digital Compare AControl Register (DCACTL) [offset =60h]
15 10 9 8
Reserved EVT2FRC
SYNCSELEVT2SRCSEL
R-0 R/W-0 R/W-0
7 4 3 2 1 0
Reserved EVT1SYNCE EVT1SOCE EVT1FRC
SYNCSELEVT1SRCSEL
R-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-51. Digital Compare AControl Register (DCACTL) Field Descriptions
Bit Field Value Description
15-10 Reserved 0 Reserved
9 EVT2FRC SYNCSEL DCAEVT2 Force Synchronization Signal Select.
0 Source IsSynchronous Signal.
1 Source IsAsynchronous Signal.
8 EVT2SRCSEL DCAEVT2 Source Signal Select.
0 Source IsDCAEVT2 Signal.
1 Source IsDCEVTFILT Signal.
7-4 Reserved 0 Reserved
3 EVT1SYNCE DCAEVT1 SYNC Enable.
0 SYNC Generation isdisabled.
1 SYNC Generation isenabled.
2 EVT1SOCE DCAEVT1 SOC Enable.
0 SOC Generation isdisabled.
1 SOC Generation isenabled .
1 EVT1FRC SYNCSEL DCAEVT1 Force Synchronization Signal Select.
0 Source IsSynchronous Signal.
1 Source IsAsynchronous Signal.
0 EVT1SRCSEL DCAEVT1 Source Signal Select.
0 Source IsDCAEVT1 Signal.
1 Source IsDCEVTFILT Signal.

<!-- Page 2102 -->

ePWM Registers www.ti.com
2102 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.2 Digital Compare Trip Select (DCTRIPSEL)
Figure 35-92. Digital Compare Trip Select (DCTRIPSEL) [offset =62h]
15 12 11 8
DCBLCOMPSEL DCBHCOMPSEL
R/W-0 R/W-0
7 4 3 0
DCALCOMPSEL DCAHCOMPSEL
R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 35-52. Digital Compare Trip Select (DCTRIPSEL) Field Descriptions
Bit Field Value Description
15-12 DCBLCOMPSEL Digital Compare BLow Input Select.
Defines thesource fortheDCBL input. The TZsignals, when used astripsignals, are
treated asnormal inputs andcanbedefined asactive high oractive low.
0 TZ1 input
1h TZ2 input
2h TZ3 input
Allother values Values notshown arereserved. Ifadevice does nothave aparticular comparitor, then
thatoption isreserved.
11-8 DCBHCOMPSEL Digital Compare BHigh Input Select.
Defines thesource fortheDCBH input. The TZsignals, when used astripsignals, are
treated asnormal inputs andcanbedefined asactive high oractive low.
0 TZ1 input
1h TZ2 input
2h TZ3 input
Allother values Values notshown arereserved. Ifadevice does nothave aparticular comparitor, then
thatoption isreserved.
7-4 DCALCOMPSEL Digital Compare ALow Input Select.
Defines thesource fortheDCAL input. The TZsignals, when used astripsignals, are
treated asnormal inputs andcanbedefined asactive high oractive low.
0 TZ1 input
1h TZ2 input
2h TZ3 input
Allother values Values notshown arereserved. Ifadevice does nothave aparticular comparitor, then
thatoption isreserved.
3-0 DCAHCOMPSEL Digital Compare AHigh Input Select.
Defines thesource fortheDCAH input. The TZsignals, when used astripsignals, are
treated asnormal inputs andcanbedefined asactive high oractive low.
0 TZ1 input
1h TZ2 input
2h TZ3 input
Allother values Values notshown arereserved. Ifadevice does nothave aparticular comparitor, then
thatoption isreserved.

<!-- Page 2103 -->

www.ti.com ePWM Registers
2103 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.3 Digital Compare Filter Control Register (DCFCTL)
Figure 35-93. Digital Compare Filter Control Register (DCFCTL) [offset =64h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved PULSESEL BLANKINV BLANKE SRCSEL
R-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-53. Digital Compare Filter Control Register (DCFCTL) Field Descriptions
Bit Field Value Description
15-6 Reserved 0 Reserved
5-4 PULSESEL Pulse Select ForBlanking &Capture Alignment.
0 Time-base counter equal toperiod (TBCTR =TBPRD)
1h Time-base counter equal tozero (TBCTR =0x0000)
2h-3h Reserved
3 BLANKINV Blanking Window Inversion.
0 Blanking window isnotinverted.
1 Blanking window isinverted.
2 BLANKE Blanking Window Enable.
0 Blanking window isdisabled.
1 Blanking window isenabled.
1-0 SRCSEL Filter Block Signal Source Select.
0 Source IsDCAEVT1 Signal.
1h Source IsDCAEVT2 Signal.
2h Source IsDCBEVT1 Signal.
3h Source IsDCBEVT2 Signal.

<!-- Page 2104 -->

ePWM Registers www.ti.com
2104 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.4 Digital Compare BControl Register (DCBCTL)
Figure 35-94. Digital Compare BControl Register (DCBCTL) [offset =66h]
15 10 9 8
Reserved EVT2FRC
SYNCSELEVT2SRCSEL
R-0 R/W-0 R/W-0
7 4 3 2 1 0
Reserved EVT1SYNCE EVT1SOCE EVT1FRC
SYNCSELEVT1SRCSEL
R-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-54. Digital Compare BControl Register (DCBCTL) Field Descriptions
Bit Field Value Description
15-10 Reserved 0 Reserved
9 EVT2FRC SYNCSEL DCBEVT2 Force Synchronization Signal Select.
0 Source IsSynchronous Signal.
1 Source IsAsynchronous Signal.
8 EVT2SRCSEL DCBEVT2 Source Signal Select.
0 Source IsDCBEVT2 Signal.
1 Source IsDCEVTFILT Signal.
7-4 Reserved 0 Reserved
3 EVT1SYNCE DCBEVT1 SYNC, Enable.
0 SYNC Generation isdisabled.
1 SYNC Generation isenabled.
2 EVT1SOCE DCBEVT1 SOC, Enable.
0 SOC Generation isdisabled.
1 SOC Generation isenabled.
1 EVT1FRC SYNCSEL DCBEVT1 Force Synchronization Signal Select.
0 Source IsSynchronous Signal.
1 Source IsAsynchronous Signal.
0 EVT1SRCSEL DCBEVT1 Source Signal Select.
0 Source IsDCBEVT1 Signal.
1 Source IsDCEVTFILT Signal.

<!-- Page 2105 -->

www.ti.com ePWM Registers
2105 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.5 Digital Compare Filter Offset Register (DCFOFFSET)
Figure 35-95. Digital Compare Filter Offset Register (DCFOFFSET) [offset =68h]
15 0
DCOFFSET
R-0
LEGEND: R=Read only; -n=value after reset
Table 35-55. Digital Compare Filter Offset Register (DCFOFFSET) Field Descriptions
Bit Field Description
15-0 OFFSET Blanking Window Offset
These 16-bits specify thenumber ofTBCLK cycles from theblanking window reference tothepoint when
theblanking window isapplied. The blanking window reference iseither period orzero asdefined bythe
DCFCTL[PULSESEL] bit.
This offset register isshadowed andtheactive register isloaded atthereference point defined by
DCFCTL[PULSESEL]. The offset counter isalso initialized andbegins tocount down when theactive
register isloaded. When thecounter expires, theblanking window isapplied. Iftheblanking window is
currently active, then theblanking window counter isrestarted.
35.4.8.6 Digital Compare Capture Control Register (DCCAPCTL)
Figure 35-96. Digital Compare Capture Control Register (DCCAPCTL) [offset =6Ah]
15 8
Reserved
R-0
7 2 1 0
Reserved SHDWMODE CAPE
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-56. Digital Compare Capture Control Register (DCCAPCTL) Field Descriptions
Bit Field Value Description
15-2 Reserved 0 Reserved
1 SHDWMODE TBCTR Counter Capture Shadow Select Mode.
0 Enable shadow mode. The DCCAP active register iscopied toshadow register onaTBCTR =
TBPRD orTBCTR =zero event asdefined bytheDCFCTL[PULSESEL] bit.CPU reads ofthe
DCCAP register willreturn theshadow register contents.
1 Active Mode. Inthismode theshadow register isdisabled. CPU reads from theDCCAP register will
always return theactive register contents.
0 CAPE TBCTR Counter Capture Enable.
0 Time-base counter capture isdisabled.
1 Time-base counter capture isenabled.

<!-- Page 2106 -->

ePWM Registers www.ti.com
2106 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.7 Digital Compare Filter Window Register (DCFWINDOW)
Figure 35-97. Digital Compare Filter Window Register (DCFWINDOW) [offset =6Ch]
15 8
Reserved
R-0
7 0
WINDOW
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 35-57. Digital Compare Filter Window Register (DCFWINDOW) Field Descriptions
Bit Field Value Description
15-8 Reserved 0 Reserved
7-0 WINDOW Blanking Window Width.
0 Noblanking window isgenerated.
1h-FFh Specifies thewidth oftheblanking window inTBCLK cycles. The blanking window begins
when theoffset counter expires. When thisoccurs, thewindow counter isloaded andbegins
tocount down. Iftheblanking window iscurrently active andtheoffset counter expires, the
blanking window counter isrestarted.
The blanking window cancross aPWM period boundary.
35.4.8.8 Digital Compare Filter Offset Counter Register (DCFOFFSETCNT)
Figure 35-98. Digital Compare Filter Offset Counter Register (DCFOFFSETCNT) [offset =6Eh]
15 0
OFFSETCNT
R-0
LEGEND: R=Read only; -n=value after reset
Table 35-58. Digital Compare Filter Offset Counter Register (DCFOFFSETCNT) Field Descriptions
Bit Field Description
15-0 OFFSETCNT Blanking Offset Counter.
These 16-bits areread only andindicate thecurrent value oftheoffset counter. The counter counts down
tozero andthen stops until itisre-loaded onthenext period orzero event asdefined bythe
DCFCTL[PULSESEL] bit.
The offset counter isnotaffected bythefree/soft emulation bits. That is,itwillalways continue tocount
down ifthedevice ishalted byaemulation stop.

<!-- Page 2107 -->

www.ti.com ePWM Registers
2107 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Pulse Width Modulator (ePWM) Module35.4.8.9 Digital Compare Counter Capture Register (DCCAP)
Figure 35-99. Digital Compare Counter Capture Register (DCCAP) [offset =70h]
15 0
DCCAP
R-0
LEGEND: R=Read only; -n=value after reset
Table 35-59. Digital Compare Counter Capture Register (DCCAP) Field Descriptions
Bit Field Description
15-0 DCCAP Digital Compare Time-Base Counter Capture.
Toenable time-base counter capture, settheDCCAPCLT[CAPE] bitto1.
Ifenabled, reflects thevalue ofthetime-base counter (TBCTR) onthelow-to-high edge transition ofa
filtered (DCEVTFLT) event. Further capture events areignored until thenext period orzero asselected by
theDCFCTL[PULSESEL] bit.
Shadowing ofDCCAP isenabled anddisabled bytheDCCAPCTL[SHDWMODE] bit.Bydefault this
register isshadowed.
*IfDCCAPCTL[SHDWMODE] =0,then theshadow isenabled. Inthismode, theactive register iscopied
totheshadow register ontheTBCTR =TBPRD orTBCTR =zero asdefined bythe
DCFCTL[PULSESEL] bit.CPU reads ofthisregister willreturn theshadow register value.
*IfDCCAPCTL[SHDWMODE] =1,then theshadow register isdisabled. Inthismode, CPU reads will
return theactive register value.
The active andshadow registers share thesame memory-map address.
35.4.8.10 Digital Compare Filter Window Counter Register (DCFWINDOWCNT)
Figure 35-100. Digital Compare Filter Window Counter Register (DCFWINDOWCNT) [offset =72h]
15 8
Reserved
R-0
7 0
WINDOWCNT
R-0
LEGEND: R=Read only; -n=value after reset
Table 35-60. Digital Compare Filter Window Counter Register (DCFWINDOWCNT) Field
Descriptions
Bit Field Value Description
15-8 Reserved 0 Any writes tothese bit(s) must always have avalue of0.
7-0 WINDOWCNT 0-FFh Blanking Window Counter.
These 8bitsareread-only andindicate thecurrent value ofthewindow counter. The counter
counts down tozero andthen stops until itisre-loaded when theoffset counter reaches zero again.