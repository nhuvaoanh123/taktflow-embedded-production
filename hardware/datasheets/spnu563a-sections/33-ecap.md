# Enhanced Capture (eCAP) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 1927-1956

---


<!-- Page 1927 -->

1927 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleChapter 33
SPNU563A -March 2018
Enhanced Capture (eCAP) Module
The enhanced Capture (eCAP) module isessential insystems where accurate timing ofexternal events is
important. This microcontroller implements 6instances oftheeCAP module.
Topic ........................................................................................................................... Page
33.1 Introduction ................................................................................................... 1928
33.2 Basic Operation .............................................................................................. 1929
33.3 Application oftheECAP Module ...................................................................... 1936
33.4 Application oftheAPWM Mode ........................................................................ 1945
33.5 eCAP Registers .............................................................................................. 1946

<!-- Page 1928 -->

Introduction www.ti.com
1928 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.1 Introduction
Uses foreCAP include:
*Speed measurements ofrotating machinery (forexample, toothed sprockets sensed viaHall sensors)
*Elapsed time measurements between position sensor pulses
*Period andduty cycle measurements ofpulse train signals
*Decoding current orvoltage amplitude derived from duty cycle encoded current/voltage sensors
33.1.1 Features
The eCAP module includes thefollowing features:
*4-event time-stamp registers (each 32bits)
*Edge polarity selection foruptofour sequenced time-stamp capture events
*Interrupt oneither ofthefour events
*Single shot capture ofuptofour event time-stamps
*Continuous mode capture oftime-stamps inafour-deep circular buffer
*Absolute time-stamp capture
*Difference (Delta) mode time-stamp capture
*Allabove resources dedicated toasingle input pin
*When notused incapture mode, theECAP module canbeconfigured asasingle channel PWM output
33.1.2 Description
One eCAP channel hasthefollowing independent keyresources:
*Dedicated input capture pin
*32-bit time base (counter)
*4x32-bit time-stamp capture registers (CAP1-CAP4)
*4-stage sequencer (Modulo4 counter) thatissynchronized toexternal events, ECAP pinrising/falling
edges.
*Independent edge polarity (rising/falling edge) selection forall4events
*Input capture signal prescaling (from 2to62)
*One-shot compare register (2bits) tofreeze captures after 1to4time-stamp events
*Control forcontinuous time-stamp captures using a4-deep circular buffer (CAP1-CAP4) scheme
*Interrupt capabilities onanyofthe4capture events

<!-- Page 1929 -->

Counter (ºtimerº) SyncIn
32
CAP1 reg
CAP2 reg
CAP4 regCAP3 reg
Interrupt I/F ECAPxINTSequencing
Edge detection
Edge polarity
PrescaleECAPx
pinNote:
Same pin
depends on
operating
mode
Counter (ºtimerº) SyncIn
32Capture
mode
APWM
mode
Period reg
(active) (ºCAP1º)
Compare reg
(active) (ºCAP2º)
Period reg
(shadow) (ºCAP3º)
(shadow) (ºCAP4º)Compare reg
Interrupt I/F ECAPxINTPWM
Compare logicAPWMx
pinOr
Syncout
www.ti.com Basic Operation
1929 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.2 Basic Operation
33.2.1 Capture andAPWM Operating Mode
You canusetheeCAP module resources toimplement asingle-channel PWM generator (with 32bit
capabilities) when itisnotbeing used forinput captures. The counter operates incount-up mode,
providing atime-base forasymmetrical pulse width modulation (PWM) waveforms. The CAP1 andCAP2
registers become theactive period andcompare registers, respectively, while CAP3 andCAP4 registers
become theperiod andcapture shadow registers, respectively. Figure 33-1 isahigh-level view ofboth the
capture andauxiliary pulse-width modulator (APWM) modes ofoperation.
Figure 33-1. Capture andAPWM Modes ofOperation
A Asingle pinisshared between CAP andAPWM functions. Incapture mode, itisaninput; inAPWM mode, itisan
output.
B InAPWM mode, writing anyvalue toCAP1/CAP2 active registers also writes thesame value tothecorresponding
shadow registers CAP3/CAP4. This emulates immediate mode. Writing totheshadow registers CAP3/CAP4 invokes
theshadow mode.

<!-- Page 1930 -->

Polarity
select
Polarity
select
Polarity
selectSYNCTSCTR
(counter-32 bit)
RST
CAP1
(APRD active)LD
CAP2
(ACMP active)LD
CAP3
(APRD shadow)LD
CAP4
(ACMP shadow)LD
Continuous /
Oneshot
Capture ControlLD1
LD2
LD3
LD432
32PRD [0-31]
CMP [0-31]CTR [0-31]
ECAPx
Interrupt
Trigger
and
Flag
controlto VIM
CTR=CMP32
32
32
3232
ACMP
shadowEvent
PrescaleCTRPHS
(phase register-32 bit)
SYNCOutSYNCIn
Event
qualifierPolarity
select
CTR=PRDCTR_OVF4PWM
compare
logicCTR [0-31]
PRD [0-31]
CMP [0-31]
CTR=CMPCTR=PRDCTR_OVF OVFAPWM  mode
Delta-mode
4Capture□events
CEVT[1:4]APRD
shadow
3232
MODE SELECTECCTL2 [ SYNCI_EN, SYNCOSEL, SWSYNC]
ECCTL2[CAP/APWM]
Edge□Polarity□Select
ECCTL1[CAPxPOL]ECCTL1[EVTPS]ECCTL1 [ CAPLDEN, CTRRSTx]
ECCTL2 [ RE-ARM, CONT/ONESHT, STOP_WRAP]
Registers: ECEINT, ECFLG, ECCLR, ECFRC
Basic Operation www.ti.com
1930 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.2.2 Capture Mode Description
Figure 33-2 shows thevarious components thatimplement thecapture function.
Figure 33-2. Capture Function Diagram
33.2.2.1 Event Prescaler
*Aninput capture signal (pulse train) canbeprescaled byN=2-62 (inmultiples of2)orcanbypass the
prescaler.
This isuseful when very high frequency signals areused asinputs. Figure 33-3 shows afunctional
diagram andFigure 33-4 shows theoperation oftheprescale function.

<!-- Page 1931 -->

ECAPx
PSout
div 2
PSout
div 4
PSout
div 6
PSout
div 8
PSout
div 10
0
1 /n
5
ECCTL1[EVTPS]
prescaler [5 bits]
(counter)By−passEvent prescaler
ECAPx pin
(from GPIO)PSout
www.ti.com Basic Operation
1931 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleFigure 33-3. Event Prescale Control
A When aprescale value of1ischosen (ECCTL1[13:9] =0,0,0,0,0 ),theinput capture signal by-passes theprescale
logic completely.
Figure 33-4. Prescale Function Waveforms
33.2.2.2 Edge Polarity Select andQualifier
*Four independent edge polarity (rising edge/falling edge) selection MUXes areused, oneforeach
capture event.
*Each edge (upto4)isevent qualified bytheModulo4 sequencer.
*The edge event isgated toitsrespective CAPx register bytheMod4 counter. The CAPx register is
loaded onthefalling edge.

<!-- Page 1932 -->

CEVT1
CEVT2
CEVT3
CEVT4
One−shot
control logicStopCLK
RSTModulo 4
counter2
Mod_eq
Stop value (2b)
ECCTL2[STOP_WRAP] ECCTL2[RE−ARM]
ECCTL2[CONT/ONESHT]0 1 32
2:4 MUX
Basic Operation www.ti.com
1932 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.2.2.3 Continuous/One-Shot Control
*The Mod4 (2bit)counter isincremented viaedge qualified events (CEVT1-CEVT4).
*The Mod4 counter continues counting (0->1->2->3->0)andwraps around unless stopped.
*A2-bit stop register isused tocompare theMod4 counter output, andwhen equal stops theMod4
counter andinhibits further loads oftheCAP1-CAP4 registers. This occurs during one-shot operation.
The continuous/one-shot block controls thestart/stop andreset (zero) functions oftheMod4 counter viaa
mono-shot type ofaction thatcanbetriggered bythestop-value comparator andre-armed viasoftware
control.
Once armed, theeCAP module waits for1-4(defined bystop-value) capture events before freezing both
theMod4 counter andcontents ofCAP1-4 registers (time-stamps).
Re-arming prepares theeCAP module foranother capture sequence. Also re-arming clears (tozero) the
Mod4 counter andpermits loading ofCAP1-4 registers again, providing theCAPLDEN bitisset.
Incontinuous mode, theMod4 counter continues torun(0->1->2->3->0,theone-shot action isignored,
andcapture values continue tobewritten toCAP1-4 inacircular buffer sequence.
Figure 33-5. Continuous/One-shot Block
33.2.2.4 32-Bit Counter andPhase Control
This counter provides thetime-base forevent captures, andisclocked viathesystem clock.
Aphase register isprovided toachieve synchronization with other counters, viaahardware andsoftware
forced sync. This isuseful inAPWM mode when aphase offset between modules isneeded.
Onanyofthefour event loads, anoption toreset the32-bit counter isgiven. This isuseful fortime
difference capture. The 32-bit counter value iscaptured first, then itisreset to0byanyoftheLD1-LD4
signals.

<!-- Page 1933 -->

SYNCI
CTR=PRD
Disable
Disable
Sync out
selectECCTL2[SYNCOSEL]
SYNCOECCTL2[SWSYNC]SYNC
ECCTL2[SYNCI_EN]
CTRPHS
LD_CTRPHS
CLKTSCTR
(counter 32b)RST
OVF SYSCLKDelta−mode
CTR−OVF
CTR[31−0]
www.ti.com Basic Operation
1933 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleFigure 33-6. Counter andSynchronization Block
33.2.2.5 CAP1-CAP4 Registers
These 32-bit registers arefedbythe32-bit counter timer bus, CTR[0-31] andareloaded (that is,capture a
time-stamp) when their respective LDinputs arestrobed.
Loading ofthecapture registers canbeinhibited viacontrol bitCAPLDEN. During one-shot operation, this
bitiscleared (loading isinhibited) automatically when astop condition occurs, StopValue =Mod4.
CAP1 andCAP2 registers become theactive period andcompare registers, respectively, inAPWM mode.
CAP3 andCAP4 registers become therespective shadow registers (APRD andACMP) forCAP1 and
CAP2 during APWM operation.
33.2.2.6 Interrupt Control
AnInterrupt canbegenerated oncapture events (CEVT1-CEVT4, CTROVF) orAPWM events (CTR =
PRD, CTR =CMP).
Acounter overflow event (FFFFFFFF- >00000000) isalso provided asaninterrupt source (CTROVF).
The capture events areedge andsequencer qualified (ordered intime) bythepolarity select andMod4
gating, respectively.
One ofthese events canbeselected astheinterrupt source (from theeCAPx module) going tothePIE.
Seven interrupt events (CEVT1, CEVT2, CEVT3, CEVT4, CNTOVF, CTR =PRD, CTR =CMP) canbe
generated. The interrupt enable register (ECEINT) isused toenable/disable individual interrupt event
sources. The interrupt flagregister (ECFLG) indicates ifanyinterrupt event hasbeen latched andcontains
theglobal interrupt flagbit(INT). Aninterrupt pulse isgenerated tothePIEonly ifanyoftheinterrupt
events areenabled, theflagbitis1,andtheINTflagbitis0.The interrupt service routine must clear the
global interrupt flagbitandtheserviced event viatheinterrupt clear register (ECCLR) before anyother
interrupt pulses aregenerated. You canforce aninterrupt event viatheinterrupt force register (ECFRC).
This isuseful fortestpurposes.

<!-- Page 1934 -->

PRDEQClear
SetLatch
CEVT1ECFRCECCLRECFLG
ECEINT
ECCLR
ECEINT SetClear
Latch
CEVT2ECFRCECFLG
ECCLRECCLR
Clear
ECEINTLatch
SetECFLGECEINT SetClear
Latch
ECFRC
CEVT4CEVT3ECFRCECFLG
ECFRC
ECCLR
ECEINT SetECEINT
ECFLG
LatchClearLatch
Set
CMPEQECFRCECCLRECCLR Clear
ECFLGECEINT
ClearLatch
SetECFRC
CTROVFECFLG01
0Generate
interrupt
pulse when
input=1LatchClear
SetECCLR
ECAPxINTECFLG
Basic Operation www.ti.com
1934 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleNote: The CEVT1, CEVT2, CEVT3, CEVT4 flags areonly active incapture mode (ECCTL2[CAP_APWM
==0]).The CTR_PRD andCTR_CMP flags areonly valid inAPWM mode (ECCTL2[CAP_APWM ==1]).
CNTOVF flagisvalid inboth modes.
Figure 33-7. Interrupts ineCAP Module
33.2.2.7 Shadow Load andLockout Control
Incapture mode, thislogic inhibits (locks out) anyshadow loading ofCAP1 orCAP2 from APRD and
ACMP registers, respectively.
InAPWM mode, shadow loading isactive andtwochoices arepermitted:
*Immediate -APRD orACMP aretransferred toCAP1 orCAP2 immediately upon writing anew value.
*Onperiod equal, CTR[31:0] =PRD[31:0]

<!-- Page 1935 -->

APRDTSCTR
FFFFFFFF
ACMP
0000000C
APWMx
(o/p pin)
On
timeOff−time
Period1000h
500h
300h
www.ti.com Basic Operation
1935 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.2.2.8 APWM Mode Operation
Main operating highlights oftheAPWM section:
*The time-stamp counter busismade available forcomparison via2digital (32-bit) comparators.
*When CAP1/2 registers arenotused incapture mode, their contents canbeused asPeriod and
Compare values inAPWM mode.
*Double buffering isachieved viashadow registers APRD andACMP (CAP3/4). The shadow register
contents aretransferred over toCAP1/2 registers either immediately upon awrite, oronaCTR =PRD
trigger.
*InAPWM mode, writing toCAP1/CAP2 active registers willalso write thesame value tothe
corresponding shadow registers CAP3/CAP4. This emulates immediate mode. Writing totheshadow
registers CAP3/CAP4 willinvoke theshadow mode.
*During initialization, youmust write totheactive registers forboth period andcompare. This
automatically copies theinitial values intotheshadow values. Forsubsequent compare updates,
during run-time, youonly need tousetheshadow registers.
Figure 33-8. PWM Waveform Details ofAPWM Mode Operation
The behavior ofAPWM active high mode (APWMPOL ==0)isasfollows:
CMP =0x00000000, output low for duration ofperiod (0% duty)
CMP =0x00000001, output high 1cycle
CMP =0x00000002, output high 2cycles
CMP =PERIOD, output high except for 1cycle (<100% duty)
CMP =PERIOD+1, output high for complete period (100% duty)
CMP >PERIOD+1, output high for complete period
The behavior ofAPWM active lowmode (APWMPOL ==1)isasfollows:
CMP =0x00000000, output high for duration ofperiod (0% duty)
CMP =0x00000001, output low 1cycle
CMP =0x00000002, output low 2cycles
CMP =PERIOD, output low except for 1cycle (<100% duty)
CMP =PERIOD+1, output low for complete period (100% duty)
CMP >PERIOD+1, output low for complete period

<!-- Page 1936 -->

Application oftheECAP Module www.ti.com
1936 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3 Application oftheECAP Module
The following sections willprovide Applications examples andcode snippets toshow how toconfigure and
operate theeCAP module. Forclarity andease ofuse, theexamples usetheeCAP "C"header files.
Below areuseful #defines which willhelp intheunderstanding oftheexamples.
//ECCTL1 (ECAP Control Reg 1)
//==========================
//CAPxPOL bits
#define EC_RISING 0x0
#define EC_FALLING 0x1
//CTRRSTx bits
#define EC_ABS_MODE 0x0
#define EC_DELTA_MODE 0x1
//PRESCALE bits
#define EC_BYPASS 0x0
#define EC_DIV1 0x0
#define EC_DIV2 0x1
#define EC_DIV4 0x2
#define EC_DIV6 0x3
#define EC_DIV8 0x4
#define EC_DIV10 0x5
//ECCTL2 (ECAP Control Reg 2)
//==========================
//CONT/ONESHOT bit
#define EC_CONTINUOUS 0x0
#define EC_ONESHOT 0x1
//STOPVALUE bit
#define EC_EVENT1 0x0
#define EC_EVENT2 0x1
#define EC_EVENT3 0x2
#define EC_EVENT4 0x3
//RE-ARM bit
#define EC_ARM 0x1
//TSCTRSTOP bit
#define EC_FREEZE 0x0
#define EC_RUN 0x1
//SYNCO_SEL bit
#define EC_SYNCIN 0x0
#define EC_CTR_PRD 0x1
#define EC_SYNCO_DIS 0x2
//CAP_APWM mode bit
#define EC_CAP_MODE 0x0
#define EC_APWM_MODE 0x1
//APWMPOL bit
#define EC_ACTV_HI 0x0
#define EC_ACTV_LO 0x1
//Generic
#define EC_DISABLE 0x0
#define EC_ENABLE 0x1
#define EC_FORCE 0x1

<!-- Page 1937 -->

CEVT1 CEVT2 CEVT3 CEVT4 CEVT1
CAPx pin
0 1 2 3 0 1
XX t5
XX t2
XX t3
XX t4t1
Capture registers [1−4]All capture values valid
(can be read) at this time00000000FFFFFFFF
CTR[0−31] t1t2t3t4t5
MOD4
CTR
CAP1
CAP2
CAP3
CAP4
t
Polarity selection
www.ti.com Application oftheECAP Module
1937 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.1 Example 1-Absolute Time-Stamp Operation Rising Edge Trigger
Figure 33-9 shows anexample ofcontinuous capture operation (Mod4 counter wraps around). Inthis
figure, TSCTR counts-up without resetting andcapture events arequalified ontherising edge only, this
gives period (and frequency) information.
Onanevent, theTSCTR contents (time-stamp) isfirstcaptured, then Mod4 counter isincremented tothe
next state. When theTSCTR reaches FFFFFFFF (maximum value), itwraps around to00000000 (not
shown inFigure 33-9),ifthisoccurs, theCTROVF (counter overflow) flagisset,andaninterrupt (if
enabled) occurs, CTROVF (counter overflow) Flag isset,andanInterrupt (ifenabled) occurs. Captured
Time-stamps arevalid atthepoint indicated bythediagram, after the4thevent, hence event CEVT4 can
conveniently beused totrigger aninterrupt andtheCPU canread data from theCAPx registers.
Figure 33-9. Capture Sequence forAbsolute Time-stamp andRising Edge Detect

<!-- Page 1938 -->

Application oftheECAP Module www.ti.com
1938 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.1.1 Code Snippet forCAP Mode Absolute Time, Rising Edge Trigger
//Code snippet for CAP mode Absolute Time, Rising edge trigger
//Initialization Time
//=======================
//ECAP module 1config
ECap1Regs.ECCTL1.bit.CAP1POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP2POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP3POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP4POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CTRRST1 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST2 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST3 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST4 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CAPLDEN =EC_ENABLE;
ECap1Regs.ECCTL1.bit.PRESCALE =EC_DIV1;
ECap1Regs.ECCTL2.bit.CAP_APWM =EC_CAP_MODE;
ECap1Regs.ECCTL2.bit.CONT_ONESHT =EC_CONTINUOUS;
ECap1Regs.ECCTL2.bit.SYNCO_SEL =EC_SYNCO_DIS;
ECap1Regs.ECCTL2.bit.SYNCI_EN =EC_DISABLE;
ECap1Regs.ECCTL2.bit.TSCTRSTOP =EC_RUN; //Allow TSCTR torun
//Run Time (e.g. CEVT4 triggered ISR call)
//==========================================
TSt1 =ECap1Regs.CAP1; //Fetch Time-Stamp captured att1
TSt2 =ECap1Regs.CAP2; //Fetch Time-Stamp captured att2
TSt3 =ECap1Regs.CAP3; //Fetch Time-Stamp captured att3
TSt4 =ECap1Regs.CAP4; //Fetch Time-Stamp captured att4
Period1 =TSt2-TSt1; //Calculate 1st period
Period2 =TSt3-TSt2; //Calculate 2nd period
Period3 =TSt4-TSt3; //Calculate 3rd period

<!-- Page 1939 -->

CEVT1CEVT2
CEVT3CEVT4
CEVT1
FFFFFFFF
CTR[0−31]
00000000CAPx pin
tMOD4
CTR
CAP1
CAP2
CAP3
CAP4
Capture registers [1−4]CEVT2
CEVT1 CEVT3CEVT4
t1t2t3t4t5t6t7t8t9
0 1 2 3 0 1 2 3 0
XX t 1 t5
XX t 2 t6
XX t3 t7
XX t4 t8
Polarity selectiont
www.ti.com Application oftheECAP Module
1939 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.2 Example 2-Absolute Time-Stamp Operation Rising andFalling Edge Trigger
InFigure 33-10 ,theeCAP operating mode isalmost thesame asintheprevious section except capture
events arequalified aseither rising orfalling edge, thisnow gives both period andduty cycle information:
Period1 =t3-t1,Period2 =t5-t3,...etc.Duty Cycle1 (on-time %)=(t2-t1)/Period1 x100%, etc.Duty
Cycle1 (off-time %)=(t3-t2)/Period1 x100%, etc.
Figure 33-10. Capture Sequence forAbsolute Time-stamp With Rising andFalling Edge Detect

<!-- Page 1940 -->

Application oftheECAP Module www.ti.com
1940 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.2.1 Code Snippet forCAP Mode Absolute Time, Rising andFalling Edge Triggers
//Code snippet for CAP mode Absolute Time, Rising &Falling edge triggers
//Initialization Time
//=======================
//ECAP module 1config
ECap1Regs.ECCTL1.bit.CAP1POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP2POL =EC_FALLING;
ECap1Regs.ECCTL1.bit.CAP3POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP4POL =EC_FALLING;
ECap1Regs.ECCTL1.bit.CTRRST1 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST2 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST3 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CTRRST4 =EC_ABS_MODE;
ECap1Regs.ECCTL1.bit.CAPLDEN =EC_ENABLE;
ECap1Regs.ECCTL1.bit.PRESCALE =EC_DIV1;
ECap1Regs.ECCTL2.bit.CAP_APWM =EC_CAP_MODE;
ECap1Regs.ECCTL2.bit.CONT_ONESHT =EC_CONTINUOUS;
ECap1Regs.ECCTL2.bit.SYNCO_SEL =EC_SYNCO_DIS;
ECap1Regs.ECCTL2.bit.SYNCI_EN =EC_DISABLE;
ECap1Regs.ECCTL2.bit.TSCTRSTOP =EC_RUN; //Allow TSCTR torun
//Run Time (e.g. CEVT4 triggered ISR call)
//==========================================
TSt1 =ECap1Regs.CAP1; //Fetch Time-Stamp captured att1
TSt2 =ECap1Regs.CAP2; //Fetch Time-Stamp captured att2
TSt3 =ECap1Regs.CAP3; //Fetch Time-Stamp captured att3
TSt4 =ECap1Regs.CAP4; //Fetch Time-Stamp captured att4
Period1 =TSt3-TSt1; //Calculate 1st period
DutyOnTime1 =TSt2-TSt1; //Calculate Ontime
DutyOffTime1 =TSt3-TSt2; //Calculate Off time

<!-- Page 1941 -->

CEVT1 CEVT2 CEVT1
FFFFFFFF
CTR[0−31]
00000000CAPx pin
tMOD4
CTR
CAP1
CAP2
CAP3
CAP4
Capture registers [1−4]CEVT3 CEVT4
0 1 2 3 0 1
XX
XX
t2 XX
t3 XXt4T1T2T3T4
CTR value at CEVT1
t1
All capture values valid
(can be read) at this timePolarity selection
www.ti.com Application oftheECAP Module
1941 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.3 Example 3-Time Difference (Delta) Operation Rising Edge Trigger
Figure 33-11 shows anexample ofhow theeCAP module canbeused tocollect Delta timing data from
pulse train waveforms. Here Continuous Capture mode (TSCTR counts-up without resetting, andMod4
counter wraps around) isused. InDelta-time mode, TSCTR isReset back toZero onevery valid event.
Here Capture events arequalified asRising edge only. Onanevent, TSCTR contents (Time-Stamp) is
captured first, andthen TSCTR isreset toZero. The Mod4 counter then increments tothenext state. If
TSCTR reaches FFFFFFFF (maximum value), before thenext event, itwraps around to00000000 and
continues, aCNTOVF (counter overflow) Flag isset,andanInterrupt (ifenabled) occurs. The advantage
ofDelta-time Mode isthattheCAPx contents directly give timing data without theneed forCPU
calculations: Period1 =T1,Period2 =T2,...etc.Asshown inthediagram, theCEVT1 event isagood
trigger point toread thetiming data, T1,T2,T3,T4areallvalid here.
Figure 33-11. Capture Sequence forDelta Mode Time-stamp andRising Edge Detect

<!-- Page 1942 -->

Application oftheECAP Module www.ti.com
1942 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.3.1 Code Snippet forCAP Mode Delta Time, Rising Edge Trigger
//Code snippet for CAP mode Delta Time, Rising edge trigger
//Initialization Time
//=======================
//ECAP module 1config
ECap1Regs.ECCTL1.bit.CAP1POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP2POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP3POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP4POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CTRRST1 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST2 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST3 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST4 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CAPLDEN =EC_ENABLE;
ECap1Regs.ECCTL1.bit.PRESCALE =EC_DIV1;
ECap1Regs.ECCTL2.bit.CAP_APWM =EC_CAP_MODE;
ECap1Regs.ECCTL2.bit.CONT_ONESHT =EC_CONTINUOUS;
ECap1Regs.ECCTL2.bit.SYNCO_SEL =EC_SYNCO_DIS;
ECap1Regs.ECCTL2.bit.SYNCI_EN =EC_DISABLE;
ECap1Regs.ECCTL2.bit.TSCTRSTOP =EC_RUN; //Allow TSCTR torun
//Run Time (e.g. CEVT1 triggered ISR call)
//==========================================
//Note: here Time-stamp directly represents the Period value.
Period4 =ECap1Regs.CAP1; //Fetch Time-Stamp captured atT1
Period1 =ECap1Regs.CAP2; //Fetch Time-Stamp captured atT2
Period2 =ECap1Regs.CAP3; //Fetch Time-Stamp captured atT3
Period3 =ECap1Regs.CAP4; //Fetch Time-Stamp captured atT4

<!-- Page 1943 -->

CEVT1CEVT2
CEVT5
FFFFFFFF
CTR[0−31]
00000000CAPx□pin
tMOD4
CTR
CAP1
CAP2
CAP3
CAP4
Capture□registers□[1−4]CEVT3CEVT4
0 1 2 3 0 1
XX
XX
t2 XX
t3 XXt4 CTR□value□at□CEVT1
t1T1
T2
2 3 0
t5
t6
t7T3
T4T5
T6
T7T8CEVT1CEVT2
CEVT3CEVT4
Polarity□selection
www.ti.com Application oftheECAP Module
1943 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.4 Example 4-Time Difference (Delta) Operation Rising andFalling Edge Trigger
InFigure 33-12 ,theeCAP operating mode isalmost thesame asinprevious section except Capture
events arequalified aseither Rising orFalling edge, thisnow gives both Period andDuty cycle
information: Period1 =T1+T2,Period2 =T3+T4,...etcDuty Cycle1 (on-time %)=T1/Period1 x100%, etc
Duty Cycle1 (off-time %)=T2/Period1 x100%, etc
During initialization, youmust write totheactive registers forboth period andcompare. This willthen
automatically copy theinitvalues intotheshadow values. Forsubsequent compare updates, during run-
time, only theshadow registers must beused.
Figure 33-12. Capture Sequence forDelta Mode Time-stamp With Rising andFalling Edge Detect

<!-- Page 1944 -->

Application oftheECAP Module www.ti.com
1944 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.3.4.1 Code Snippet forCAP Mode Delta Time, Rising andFalling Edge Triggers
//Code snippet for CAP mode Delta Time, Rising and Falling edge triggers
//Initialization Time
//=======================
//ECAP module 1config
ECap1Regs.ECCTL1.bit.CAP1POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP2POL =EC_FALLING;
ECap1Regs.ECCTL1.bit.CAP3POL =EC_RISING;
ECap1Regs.ECCTL1.bit.CAP4POL =EC_FALLING;
ECap1Regs.ECCTL1.bit.CTRRST1 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST2 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST3 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CTRRST4 =EC_DELTA_MODE;
ECap1Regs.ECCTL1.bit.CAPLDEN =EC_ENABLE;
ECap1Regs.ECCTL1.bit.PRESCALE =EC_DIV1;
ECap1Regs.ECCTL2.bit.CAP_APWM =EC_CAP_MODE;
ECap1Regs.ECCTL2.bit.CONT_ONESHT =EC_CONTINUOUS;
ECap1Regs.ECCTL2.bit.SYNCO_SEL =EC_SYNCO_DIS;
ECap1Regs.ECCTL2.bit.SYNCI_EN =EC_DISABLE;
ECap1Regs.ECCTL2.bit.TSCTRSTOP =EC_RUN; //Allow TSCTR torun
//Run Time (e.g. CEVT1 triggered ISR call)
//==========================================
//Note: here Time-stamp directly represents the Duty cycle values.
DutyOnTime1 =ECap1Regs.CAP2; //Fetch Time-Stamp captured atT2
DutyOffTime1 =ECap1Regs.CAP3; //Fetch Time-Stamp captured atT3
DutyOnTime2 =ECap1Regs.CAP4; //Fetch Time-Stamp captured atT4
DutyOffTime2 =ECap1Regs.CAP1; //Fetch Time-Stamp captured atT1
Period1 =DutyOnTime1 +DutyOffTime1;
Period2 =DutyOnTime2 +DutyOffTime2;

<!-- Page 1945 -->

APRDTSCTR
FFFFFFFF
ACMP
0000000C
APWMx
(o/p pin)
On
timeOff−time
Period1000h
500h
300h
www.ti.com Application oftheAPWM Mode
1945 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.4 Application oftheAPWM Mode
Inthissection, theeCAP module isconfigured tooperate asaPWM generator. Here avery simple single
channel PWM waveform isgenerated from output pinAPWMx. The PWM polarity isactive high, which
means thatthecompare value (CAP2 regisnow acompare register) represents theon-time (high level) of
theperiod. Alternatively, iftheAPWMPOL bitisconfigured foractive low, then thecompare value
represents theoff-time. Note here values areinhexadecimal ("h")notation.
33.4.1 Simple PWM Generation (Independent Channel/s)
Figure 33-13. PWM Waveform Details ofAPWM Mode Operation
33.4.1.1 Code Snippet forAPWM Mode
//Code snippet for APWM mode Example 1
//Initialization Time
//=======================
//ECAP module 1config
ECap1Regs.CAP1 =0x1000; //Set period value
ECap1Regs.CTRPHS =0x0; //make phase zero
ECap1Regs.ECCTL2.bit.CAP_APWM =EC_APWM_MODE;
ECap1Regs.ECCTL2.bit.APWMPOL =EC_ACTV_HI; //Active high
ECap1Regs.ECCTL2.bit.SYNCI_EN =EC_DISABLE; //Synch not used
ECap1Regs.ECCTL2.bit.SYNCO_SEL =EC_SYNCO_DIS; //Synch not used
ECap1Regs.ECCTL2.bit.TSCTRSTOP =EC_RUN; //Allow TSCTR torun
//Run Time (Instant 1,e.g. ISR call)
//======================
ECap1Regs.CAP2 =0x300; //Set Duty cycle i.e. compare value
//Run Time (Instant 2,e.g. another ISR call)
//======================
ECap1Regs.CAP2 =0x500; //Set Duty cycle i.e. compare value

<!-- Page 1946 -->

eCAP Registers www.ti.com
1946 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5 eCAP Registers
Table 33-1 shows theeCAP module control andstatus registers. The base address forthecontrol
registers isFCF7 9300h foreCAP1, FCF7 9400h foreCAP2, FCF7 9500h foreCAP3, FCF7 9600h for
eCAP4, FCF7 9700h foreCAP5, andFCF7 9800h foreCAP6.
Table 33-1. ECAP Control andStatus Registers
Address Offset Acronym Description Section
00h TSCTR Time-Stamp Counter Register Section 33.5.1
04h CTRPHS Counter Phase Offset Value Register Section 33.5.2
08h CAP1 Capture 1Register Section 33.5.3
0Ch CAP2 Capture 2Register Section 33.5.4
10h CAP3 Capture 3Register Section 33.5.5
14h CAP4 Capture 4Register Section 33.5.6
28h ECCTL2 Capture Control Register 2 Section 33.5.7
2Ah ECCTL1 Capture Control Register 1 Section 33.5.8
2Ch ECFLG Capture Interrupt Flag Register Section 33.5.9
2Eh ECEINT Capture Interrupt Enable Register Section 33.5.10
30h ECFRC Capture Interrupt Forcing Register Section 33.5.11
32h ECCLR Capture Interrupt Clear Register Section 33.5.12
33.5.1 Time-Stamp Counter Register (TSCTR)
Figure 33-14. Time-Stamp Counter Register (TSCTR) [offset =00h]
31 0
TSCTR
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-2. Time-Stamp Counter Register (TSCTR) Field Descriptions
Bits Field Description
31-0 TSCTR Active 32-bit counter register thatisused asthecapture time-base.
33.5.2 Counter Phase Control Register (CTRPHS)
Figure 33-15. Counter Phase Control Register (CTRPHS) [offset =04h]
31 0
CTRPHS
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-3. Counter Phase Control Register (CTRPHS) Field Descriptions
Bits Field Description
31-0 CTRPHS Counter phase value register thatcanbeprogrammed forphase lag/lead. This register shadows TSCTR andis
loaded intoTSCTR upon either aSYNCI event orS/W force viaacontrol bit.Used toachieve phase control
synchronization with respect toother eCAP andEPWM time-bases.

<!-- Page 1947 -->

www.ti.com eCAP Registers
1947 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.3 Capture-1 Register (CAP1)
NOTE: InAPWM mode, writing toCAP1/CAP2 active registers also writes thesame value tothe
corresponding shadow registers CAP3/CAP4. This emulates immediate mode. Writing tothe
shadow registers CAP3/CAP4 invokes theshadow mode.
Figure 33-16. Capture-1 Register (CAP1) [offset =08h]
31 0
CAP1
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-4. Capture-1 Register (CAP1) Field Descriptions
Bits Field Description
31-0 CAP1 This register canbeloaded (written) by:
*Time-Stamp (counter value) during acapture event
*Software -may beuseful fortestpurposes /initialization
*APRD shadow register (CAP3) when used inAPWM mode
33.5.4 Capture-2 Register (CAP2)
NOTE: InAPWM mode, writing toCAP1/CAP2 active registers also writes thesame value tothe
corresponding shadow registers CAP3/CAP4. This emulates immediate mode. Writing tothe
shadow registers CAP3/CAP4 invokes theshadow mode.
Figure 33-17. Capture-2 Register (CAP2) [offset =0Ch]
31 0
CAP2
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-5. Capture-2 Register (CAP2) Field Descriptions
Bits Field Description
31-0 CAP2 This register canbeloaded (written) by:
*Time-Stamp (counter value) during acapture event
*Software -may beuseful fortestpurposes
*APRD shadow register (CAP4) when used inAPWM mode

<!-- Page 1948 -->

eCAP Registers www.ti.com
1948 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.5 Capture-3 Register (CAP3)
Figure 33-18. Capture-3 Register (CAP3) [offset =10h]
31 0
CAP3
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-6. Capture-3 Register (CAP3) Field Descriptions
Bits Field Description
31-0 CAP3 InCMP mode, thisisatime-stamp capture register. InAPWM mode, thisistheperiod shadow (APRD) register.
You update thePWM period value through thisregister. Inthismode, CAP3 (APRD) shadows CAP1.
33.5.6 Capture-4 Register (CAP4)
Figure 33-19. Capture-4 Register (CAP4) [offset =14h]
31 0
CAP4
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-7. Capture-4 Register (CAP4) Field Descriptions
Bits Field Description
31-0 CAP4 InCMP mode, thisisatime-stamp capture register. InAPWM mode, thisisthecompare shadow (ACMP) register.
You update thePWM compare value viathisregister. Inthismode, CAP4 (ACMP) shadows CAP2.

<!-- Page 1949 -->

www.ti.com eCAP Registers
1949 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.7 ECAP Control Register 2(ECCTL2)
Figure 33-20. ECAP Control Register 2(ECCTL2) [offset =28h]
15 11 10 9 8
Reserved APWMPOL CAP_APWM SWSYNC
R-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
SYNCO_SEL SYNCI_EN TSCTRSTOP REARM STOP_WRAP CONT_ONESHT
R/W-0 R/W-0 R/W-0 R/W-0 R/W-3h R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 33-8. ECAP Control Register 2(ECCTL2) Field Descriptions
Bits Field Value Description
15-11 Reserved 0 Reserved
10 APWMPOL APWM output polarity select. This isapplicable only inAPWM operating mode.
0 Output isactive-high (Compare value defines high time).
1 Output isactive-low (Compare value defines lowtime).
9 CAP_APWM CAP/APWM operating mode select.
0 ECAP module operates incapture mode. This mode forces thefollowing configuration:
*Inhibits TSCTR resets viaCTR =PRD event
*Inhibits shadow loads onCAP1 and2registers
*Permits user toenable CAP1-4 register load
*CAPx/APWMx pinoperates asacapture input
1 ECAP module operates inAPWM mode. This mode forces thefollowing configuration:
*Resets TSCTR onCTR =PRD event (period boundary
*Permits shadow loading onCAP1 and2registers
*Disables loading oftime-stamps intoCAP1-4 registers
*CAPx/APWMx pinoperates asaAPWM output
8 SWSYNC Software-forced Counter (TSCTR) Synchronizing. This provides aconvenient software method to
synchronize some orallECAP time bases. InAPWM mode, thesynchronizing canalso bedone via
theCTR =PRD event.
0 Writing a0hasnoeffect. Reading always returns a0.
1 Writing a1forces aTSCTR shadow load ofcurrent ECAP module andanyECAP modules down-
stream providing theSYNCO_SEL bitsare0,0.After writing a1,thisbitreturns toa0.
Note: Selection CTR =PRD ismeaningful only inAPWM mode; however, youcanchoose itinCAP
mode ifyoufinddoing souseful.
7-6 SYNCO_SEL Sync-Out select.
0 Select sync-in event tobethesync-out signal (pass through).
1h Select CTR =PRD event tobethesync-out signal.
2h Disable sync outsignal.
3h Disable sync outsignal.
5 SYNCI_EN Counter (TSCTR) Sync-In select mode.
0 Disable sync-in option.
1 Enable counter (TSCTR) tobeloaded from CTRPHS register upon either aSYNCI signal oraS/W
force event.
4 TSCTRSTOP Time Stamp (TSCTR) Counter Stop (freeze) Control.
0 TSCTR isstopped.
1 TSCTR isfree-running.

<!-- Page 1950 -->

eCAP Registers www.ti.com
1950 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleTable 33-8. ECAP Control Register 2(ECCTL2) Field Descriptions (continued)
Bits Field Value Description
3 REARM One-Shot Re-Arming Control, wait forstop trigger. Note: The re-arm function isvalid inoneshot or
continuous mode.
0 Has noeffect (reading always returns a0).
1 Arms theone-shot sequence asfollows:
1)Resets theMod4 counter to0
2)Unfreezes theMod4 counter
3)Enables capture register loads
2-1 STOP_WRAP Stop value forone-shot mode. This isthenumber (between 1-4) ofcaptures allowed tooccur
before theCAP(1-4) registers arefrozen, capture sequence isstopped.
Wrap value forcontinuous mode. This isthenumber (between 1-4) ofthecapture register inwhich
thecircular buffer wraps around andstarts again.
0 Stop after Capture Event 1inone-shot mode.
Wrap after Capture Event 1incontinuous mode.
1h Stop after Capture Event 2inone-shot mode.
Wrap after Capture Event 2incontinuous mode.
2h Stop after Capture Event 3inone-shot mode.
Wrap after Capture Event 3incontinuous mode.
3h Stop after Capture Event 4inone-shot mode.
Wrap after Capture Event 4incontinuous mode.
Notes: STOP_WRAP iscompared toMod4 counter and, when equal, 2actions occur:
*Mod4 counter isstopped (frozen)
*Capture register loads areinhibited
Inone-shot mode, further interrupt events areblocked until re-armed.
0 CONT_ONESHT Continuous orone-shot mode control (applicable only incapture mode).
0 Operate incontinuous mode.
1 Operate inone-shot mode.

<!-- Page 1951 -->

www.ti.com eCAP Registers
1951 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.8 ECAP Control Regiser 1(ECCTL1)
Figure 33-21. ECAP Control Register 1(ECCTL1) [offset =2Ah]
15 14 13 9 8
FREE SOFT PRESCALE CAPLDEN
R/W-0 R/W-0 R/W-0 R/W-0
7 6 5 4 3 2 1 0
CTRRST4 CAP4POL CTRRST3 CAP3POL CTRRST2 CAP2POL CTRRST1 CAP1POL
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 33-9. ECAP Control Register 1(ECCTL1) Field Descriptions
Bits Field Value Description
15-14 FREE/SOFT Emulation Control.
0 TSCTR counter stops immediately onemulation suspend.
1h TSCTR counter runs until =0.
2h-3h TSCTR counter isunaffected byemulation suspend (Run Free).
13-9 PRESCALE Event Filter prescale select.
0 Divide by1(noprescale, by-pass theprescaler).
1h Divide by2.
2h Divide by4.
3h Divide by6.
4h Divide by8.
5h Divide by10.
: :
1Eh Divide by60.
1Fh Divide by62.
8 CAPLDEN Enable Loading ofCAP1-4 registers onacapture event.
0 Disable CAP1-4 register loads atcapture event time.
1 Enable CAP1-4 register loads atcapture event time.
7 CTRRST4 Counter Reset onCapture Event 4.
0 Donotreset counter onCapture Event 4(absolute time stamp operation).
1 Reset counter after Capture Event 4time-stamp hasbeen captured
(used indifference mode operation).
6 CAP4POL Capture Event 4Polarity select.
0 Capture Event 4triggered onarising edge (RE).
1 Capture Event 4triggered onafalling edge (FE).
5 CTRRST3 Counter Reset onCapture Event 3.
0 Donotreset counter onCapture Event 3(absolute time stamp).
1 Reset counter after Event 3time-stamp hasbeen captured
(used indifference mode operation).
4 CAP3POL Capture Event 3Polarity select.
0 Capture Event 3triggered onarising edge (RE).
1 Capture Event 3triggered onafalling edge (FE).
3 CTRRST2 Counter Reset onCapture Event 2.
0 Donotreset counter onCapture Event 2(absolute time stamp).
1 Reset counter after Event 2time-stamp hasbeen captured
(used indifference mode operation).

<!-- Page 1952 -->

eCAP Registers www.ti.com
1952 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) ModuleTable 33-9. ECAP Control Register 1(ECCTL1) Field Descriptions (continued)
Bits Field Value Description
2 CAP2POL Capture Event 2Polarity select.
0 Capture Event 2triggered onarising edge (RE).
1 Capture Event 2triggered onafalling edge (FE).
1 CTRRST1 Counter Reset onCapture Event 1.
0 Donotreset counter onCapture Event 1(absolute time stamp).
1 Reset counter after Event 1time-stamp hasbeen captured (used indifference mode operation).
0 CAP1POL Capture Event 1Polarity select.
0 Capture Event 1triggered onarising edge (RE).
1 Capture Event 1triggered onafalling edge (FE).

<!-- Page 1953 -->

www.ti.com eCAP Registers
1953 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.9 ECAP Interrupt Flag Register (ECFLG)
Figure 33-22. ECAP Interrupt Flag Register (ECFLG) [offset =2Ch]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
CTR_CMP CTR_PRD CTROVF CEVT4 CETV3 CEVT2 CETV1 INT
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 33-10. ECAP Interrupt Flag Register (ECFLG) Field Descriptions
Bits Field Value Description
15-8 Reserved 0 Reserved
7 CTR_CMP Compare Equal Compare Status Flag. This flagisactive only inAPWM mode.
0 Indicates noevent occurred.
1 Indicates thecounter (TSCTR) reached thecompare register value (ACMP).
6 CTR_PRD Counter Equal Period Status Flag. This flagisonly active inAPWM mode.
0 Indicates noevent occurred.
1 Indicates thecounter (TSCTR) reached theperiod register value (APRD) andwas reset.
5 CTROVF Counter Overflow Status Flag. This flagisactive inCAP andAPWM mode.
0 Indicates noevent occurred.
1 Indicates thecounter (TSCTR) hasmade thetransition from FFFF FFFFh to0000 0000h.
4 CEVT4 Capture Event 4Status Flag This flagisonly active inCAP mode.
0 Indicates noevent occurred.
1 Indicates thefourth event occurred atECAPx pin.
3 CEVT3 Capture Event 3Status Flag. This flagisactive only inCAP mode.
0 Indicates noevent occurred.
1 Indicates thethird event occurred atECAPx pin.
2 CEVT2 Capture Event 2Status Flag. This flagisonly active inCAP mode.
0 Indicates noevent occurred.
1 Indicates thesecond event occurred atECAPx pin.
1 CEVT1 Capture Event 1Status Flag. This flagisonly active inCAP mode.
0 Indicates noevent occurred.
1 Indicates thefirstevent occurred atECAPx pin.
0 INT Global Interrupt Status Flag.
0 Indicates nointerrupt generated.
1 Indicates thataninterrupt was generated.

<!-- Page 1954 -->

eCAP Registers www.ti.com
1954 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.10 ECAP Interrupt Enable Register (ECEINT)
The interrupt enable bitsblock anyoftheselected events from generating aninterrupt. Events willstillbe
latched intotheflagbit(ECFLG register) andcanbeforced/cleared viatheECFRC/ECCLR registers.
The proper procedure forconfiguring peripheral modes andinterrupts isasfollows:
*Disable global interrupts
*Stop eCAP counter
*Disable eCAP interrupts
*Configure peripheral registers
*Clear spurious eCAP interrupt flags
*Enable eCAP interrupts
*Start eCAP counter
*Enable global interrupts
Figure 33-23. ECAP Interrupt Enable Register (ECEINT) [offset =2Eh]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
CTR_CMP CTR_PRD CTROVF CEVT4 CEVT3 CEVT2 CETV1 Reserved
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 33-11. ECAP Interrupt Enable Register (ECEINT) Field Descriptions
Bits Field Value Description
15-8 Reserved 0 Reserved
7 CTR_CMP Counter Equal Compare Interrupt Enable.
0 Disable Compare Equal asanInterrupt source.
1 Enable Compare Equal asanInterrupt source.
6 CTR_PRD Counter Equal Period Interrupt Enable.
0 Disable Period Equal asanInterrupt source.
1 Enable Period Equal asanInterrupt source.
5 CTROVF Counter Overflow Interrupt Enable.
0 Disabled counter Overflow asanInterrupt source.
1 Enable counter Overflow asanInterrupt source.
4 CEVT4 Capture Event 4Interrupt Enable.
0 Disable Capture Event 4asanInterrupt source.
1 Capture Event 4Interrupt Enable.
3 CEVT3 Capture Event 3Interrupt Enable.
0 Disable Capture Event 3asanInterrupt source.
1 Enable Capture Event 3asanInterrupt source.
2 CEVT2 Capture Event 2Interrupt Enable.
0 Disable Capture Event 2asanInterrupt source.
1 Enable Capture Event 2asanInterrupt source.
1 CEVT1 Capture Event 1Interrupt Enable.
0 Disable Capture Event 1asanInterrupt source.
1 Enable Capture Event 1asanInterrupt source.
0 Reserved 0 Reserved

<!-- Page 1955 -->

www.ti.com eCAP Registers
1955 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.11 ECAP Interrupt Forcing Register (ECFRC)
Figure 33-24. ECAP Interrupt Forcing Register (ECFRC) [offset =30h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
CTR_CMP CTR_PRD CTROVF CEVT4 CETV3 CETV2 CETV1 Reserved
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 33-12. ECAP Interrupt Forcing Register (ECFRC) Field Descriptions
Bits Field Value Description
15-8 Reserved 0 Any writes tothese bit(s) must always have avalue of0.
7 CTR_CMP Force Counter Equal Compare Interrupt.
0 Noeffect. Always reads back a0.
1 Writing a1sets theCTR_CMP flagbit.
6 CTR_PRD Force Counter Equal Period Interrupt.
0 Noeffect. Always reads back a0.
1 Writing a1sets theCTR_PRD flagbit.
5 CTROVF Force Counter Overflow.
0 Noeffect. Always reads back a0.
1 Writing a1tothisbitsets theCTROVF flagbit.
4 CEVT4 Force Capture Event 4.
0 Noeffect. Always reads back a0.
1 Writing a1sets theCEVT4 flagbit.
3 CEVT3 Force Capture Event 3.
0 Noeffect. Always reads back a0.
1 Writing a1sets theCEVT3 flagbit.
2 CEVT2 Force Capture Event 2.
0 Noeffect. Always reads back a0.
1 Writing a1sets theCEVT2 flagbit.
1 CEVT1 Force Capture Event 1.
1 Noeffect. Always reads back a0.
0 Sets theCEVT1 flagbit.
0 Reserved 0 Any writes tothese bit(s) must always have avalue of0.

<!-- Page 1956 -->

eCAP Registers www.ti.com
1956 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedEnhanced Capture (eCAP) Module33.5.12 ECAP Interrupt Clear Register (ECCLR)
Figure 33-25. ECAP Interrupt Clear Register (ECCLR) [offset =32h]
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
CTR_CMP CTR_PRD CTROVF CEVT4 CETV3 CETV2 CETV1 INT
R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 33-13. ECAP Interrupt Clear Register (ECCLR) Field Descriptions
Bits Field Value Description
15-8 Reserved 0 Any writes tothese bit(s) must always have avalue of0.
7 CTR_CMP Counter Equal Compare Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCTR_CMP flagcondition.
6 CTR_PRD Counter Equal Period Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCTR_PRD flagcondition.
5 CTROVF Counter Overflow Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCTROVF flagcondition.
4 CEVT4 Capture Event 4Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCEVT4 flagcondition.
3 CEVT3 Capture Event 3Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCEVT3 flagcondition.
2 CEVT2 Capture Event 2Status Flag.
1 Writing a0hasnoeffect. Always reads back a0.
0 Writing a1clears theCEVT2 flagcondition.
1 CEVT1 Capture Event 1Status Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theCEVT1 flagcondition.
0 INT Global Interrupt Clear Flag.
0 Writing a0hasnoeffect. Always reads back a0.
1 Writing a1clears theINTflagandenable further interrupts tobegenerated ifanyoftheevent flags
aresetto1.