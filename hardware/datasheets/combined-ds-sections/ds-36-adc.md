# ADC Module

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 166-178 (13 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 166 -->
166TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.4 12-bit Multibuffered Analog-to-Digital Converter (MibADC)
The MibADC hasaseparate power busforitsanalog circuitry thatenhances theAnalog-to-Digital (A-to-D)
performance bypreventing digital switching noise onthelogic circuitry which could bepresent onVSSand
VCCfrom coupling into theA-to-D analog stage. AllA-to-D specifications are given with respect to
ADREFLO ,unless otherwise noted.
Table 7-15. MibADC Overview
DESCRIPTION VALUE
Resolution 12bits
Monotonic Assured
Output conversion code 00htoFFFh [00forVAI≤ADREFLO ;FFF forVAI≥ADREFHI ]
7.4.1 MibADC Features
*10-/12-bit resolution
*AD REFHI andAD REFLO pins (high andlowreference voltages)
*Total Sample/Hold/Convert time: 600nsTypical Minimum at30MHz ADCLK
*One memory region perconversion group isavailable (Event Group, Group 1,andGroup 2)
*Allocation ofchannels toconversion groups iscompletely programmable
*Memory regions areserviced either byinterrupt orbyDMA
*Programmable interrupt threshold counter isavailable foreach group
*Programmable magnitude threshold interrupt foreach group foranyonechannel
*Option toread either 8-,10-, or12-bit values from memory regions
*Single orcontinuous conversion modes
*Embedded self-test
*Embedded calibration logic
*Enhanced power-down mode
-Optional feature toautomatically power down ADC core when noconversion isinprogress
*External event pin(ADEVT) programmable asgeneral-purpose I/O
7.4.2 Event Trigger Options
The ADC module supports three conversion groups: Event Group, Group1, and Group2. Each ofthese
three groups can beconfigured tobetriggered byahardware event. Inthat case, theapplication can
select from among eight event sources tobethetrigger foragroup's conversions.
7.4.2.1 MibADC1 Event Trigger Hookup
Table 7-16 lists theevent sources thatcantrigger theconversions fortheMibADC1 groups.

<!-- Page 167 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Peripheral Information andElectrical Specifications
Submit Documentation Feedback
Product Folder Links: TMS570LC4357167TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 7-16. MibADC1 Event Trigger Selection
GROUP SOURCE SELECT BITS
(G1SRC, G2SRC OREVSRC)EVENT NO. PINMMR161[0] PINMMR161[1]CONTROL FOR
OPTION ACONTROL FOR
OPTION BTRIGGER SOURCE
000 1 x x -- -- AD1EVT
001 21 0 PINMMR161[8] =x PINMMR161[9] =x N2HET1[8]
0 1 PINMMR161[8] =1 PINMMR161[9] =0 N2HET2[5]
0 1 PINMMR161[8] =0 PINMMR161[9] =1 e_TPWM_B
010 31 0 -- -- N2HET1[10]
0 1 -- -- N2HET1[27]
011 41 0 PINMMR161[16] =x PINMMR161[17] =x RTI1 Comp0
0 1 PINMMR161[16] =1 PINMMR161[17] =0 RTI1 Comp0
0 1 PINMMR161[16] =0 PINMMR161[17] =1 e_TPWM_A1
100 51 0 -- -- N2HET1[12]
0 1 -- -- N2HET1[17]
101 61 0 PINMMR161[24] =x PINMMR161[25] =x N2HET1[14]
0 1 PINMMR161[24] =1 PINMMR161[25] =0 N2HET1[19]
0 1 PINMMR161[24] =0 PINMMR161[25] =1 N2HET2[1]
110 71 0 PINMMR162[0] =x PINMMR162[1] =x GIOB[0]
0 1 PINMMR162[0] =1 PINMMR162[1] =0 N2HET1[11]
0 1 PINMMR162[0] =0 PINMMR162[1] =1 ePWM_A2
111 81 0 PINMMR162[8] =x PINMMR162[9] =x GIOB[1]
0 1 PINMMR162[8] =1 PINMMR162[9] =0 N2HET2[13]
0 1 PINMMR162[8] =0 PINMMR162[9] =1 ePWM_AB

<!-- Page 168 -->
168TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
ForADEVT trigger source, theconnection totheMibADC1 module trigger input ismade from
theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either by
configuring ADEVT asanoutput function ontothepad (through themux control), orby
driving theADEVT signal from anexternal trigger source asinput. Ifthemux control module
isused toselect different functionality instead oftheADEVT signal, then care must betaken
todisable ADEVT from triggering conversions; there isnomultiplexing onthe input
connection.
IfePWM_B, ePWM_A2, ePWM_AB, N2HET2[1], N2HET2[5], N2HET2[13], N2HET1[11],
N2HET1[17], orN2HET1[19] isused totrigger theADC, theconnection totheADC ismade
directly from theN2HET orePWM module outputs. Asaresult, theADC can betriggered
without having toenable thesignal from being output onadevice terminal.
NOTE
ForN2HETx trigger sources, theconnection totheMibADC1 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HETx module boundary). This way, a
trigger condition canbegenerated even iftheN2HETx signal isnotselected tobeoutput on
thepad.
NOTE
FortheRTIcompare 0interrupt source, theconnection ismade directly from theoutput of
theRTImodule. That is,theinterrupt condition canbeused asatrigger source even ifthe
actual interrupt isnotsignaled totheCPU.
7.4.2.2 MibADC2 Event Trigger Hookup
Table 7-17 lists theevent sources thatcantrigger theconversions fortheMibADC2 groups.

<!-- Page 169 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Peripheral Information andElectrical Specifications
Submit Documentation Feedback
Product Folder Links: TMS570LC4357169TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 7-17. MibADC2 Event Trigger Selection
GROUP SOURCE SELECT BITS
(G1SRC, G2SRC, orEVSRC)EVENT NO. PINMMR161[0] PINMMR161[1]CONTROL FOR
OPTION ACONTROL FOR
OPTION BTRIGGER SOURCE
000 1 x x NA NA AD2EVT
001 21 0 PINMMR162[16] =x PINMMR162[17] =x N2HET1[8]
0 1 PINMMR162[16] =1 PINMMR162[17] =0 N2HET2[5]
0 1 PINMMR162[16] =0 PINMMR162[17] =1 e_TPWM_B
010 31 0 NA NA N2HET1[10]
0 1 NA NA N2HET1[27]
011 41 0 PINMMR162[24] =x PINMMR162[25] =x RTI1 Comp0
0 1 PINMMR162[24] =1 PINMMR162[25] =0 RTI1 Comp0
0 1 PINMMR162[24] =0 PINMMR162[25] =1 e_TPWM_A1
100 51 0 NA NA N2HET1[12]
0 1 NA NA N2HET1[17]
101 61 0 PINMMR163[0] =x PINMMR163[0] =x N2HET1[14]
0 1 PINMMR163[0] =1 PINMMR163[0] =0 N2HET1[19]
0 1 PINMMR163[0] =0 PINMMR163[0] =1 N2HET2[1]
110 71 0 PINMMR163[8] =x PINMMR163[8] =x GIOB[0]
0 1 PINMMR163[8] =1 PINMMR163[8] =0 N2HET1[11]
0 1 PINMMR163[8] =0 PINMMR163[8] =1 ePWM_A2
111 81 0 PINMMR163[16] =x PINMMR163[16] =x GIOB[1]
0 1 PINMMR163[16] =1 PINMMR163[16] =0 N2HET2[13]
0 1 PINMMR163[16] =0 PINMMR163[16] =1 ePWM_AB

<!-- Page 170 -->
170TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
ForAD2EVT trigger source, theconnection totheMibADC2 module trigger input ismade
from theoutput side oftheinput buffer. This way, atrigger condition canbegenerated either
byconfiguring AD2EVT asanoutput function ontothepad (through themux control), orby
driving theAD2EVT signal from anexternal trigger source asinput. Ifthemux control module
isused toselect different functionality instead oftheAD2EVT signal, then care must be
taken todisable AD2EVT from triggering conversions; there isnomultiplexing ontheinput
connections.
IfePWM_B, ePWM_A2, ePWM_AB, N2HET2[1], N2HET2[5], N2HET2[13], N2HET1[11],
N2HET1[17], orN2HET1[19] isused totrigger theADC, theconnection totheADC ismade
directly from theN2HET orePWM module outputs. Asaresult, theADC can betriggered
without having toenable thesignal from being output onadevice terminal.
NOTE
ForN2HETx trigger sources, theconnection totheMibADC2 module trigger input ismade
from theinput side oftheoutput buffer (attheN2HETx module boundary). This way, a
trigger condition canbegenerated even iftheN2HETx signal isnotselected tobeoutput on
thepad.
NOTE
FortheRTIcompare 0interrupt source, theconnection ismade directly from theoutput of
theRTImodule. That is,theinterrupt condition canbeused asatrigger source even ifthe
actual interrupt isnotsignaled totheCPU.
7.4.2.3 Controlling ADC1 andADC2 Event Trigger Options Using SOC Output from ePWM Modules
Asshown inFigure 7-8,theePWMxSOCA andePWMxSOCB outputs from each ePWM module areused
togenerate four signals -ePWM_B, ePWM_A1, ePWM_A2, and ePWM_AB, that areavailable totrigger
theADC based ontheapplication requirement.

<!-- Page 171 -->
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
171TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-8.ADC Trigger Source Generation from ePWMx

<!-- Page 172 -->
ePWM10 0
1
1
From switch on
SOC2A
when PINMMR164[8] = 0SOC1A
PINMMR164[0]
EPWM1SOCAFrom switch on
SOC2A
when PINMMR164[8] = 1
0 0
172TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 7-18. Control BittoSOC Output
CONTROL BIT SOC OUTPUT
PINMMR164[0] SOC1A_SEL
PINMMR164[8] SOC2A_SEL
PINMMR164[16] SOC3A_SEL
PINMMR164[24] SOC4A_SEL
PINMMR165[0] SOC5A_SEL
PINMMR165[8] SOC6A_SEL
PINMMR165[16] SOC7A_SEL
The SOCA output from each ePWM module isconnected toa"switch" shown inFigure 7-8.This switch is
implemented byusing thecontrol registers inthePINMMR module. Figure 7-9isanexample ofthe
implementation isshown fortheswitch onSOC1A. The switches ontheother SOCA signals are
implemented inthesame way.
Figure 7-9.ePWM1SOC1A Switch Implementation
The logic equations forthefour outputs from thecombinational logic shown inFigure 7-8are:
ePWM_B =SOC1B orSOC2B orSOC3B orSOC4B orSOC5B orSOC6B orSOC7B (1)
ePWM_A1 =[SOC1A andnot(SOC1A_SEL) ]or[SOC2A andnot(SOC2A_SEL) ]or[SOC3A andnot(SOC3A_SEL) ]or (2)
[SOC4A andnot(SOC4A_SEL) ]or[SOC5A andnot(SOC5A_SEL) ]or[SOC6A andnot(SOC6A_SEL) ]or
[SOC7A andnot(SOC7A_SEL) ]
ePWM_A2 =[SOC1A andSOC1A_SEL ]or[SOC2A andSOC2A_SEL ]or[SOC3A andSOC3A_SEL ]or (3)
[SOC4A andSOC4A_SEL ]or[SOC5A andSOC5A_SEL ]or[SOC6A andSOC6A_SEL ]or
[SOC7A andSOC7A_SEL ]
ePWM_AB =ePWM_B orePWM_A2 (4)

<!-- Page 173 -->
173TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) ForVCCAD andVSSAD recommended operating conditions, seeSection 5.4.
(2) Input currents intoanyADC input channel outside thespecified limits could affect conversion results ofother channels.7.4.3 ADC Electrical andTiming Specifications
Table 7-19. MibADC Recommended Operating Conditions
PARAMETER MIN MAX UNIT
ADREFHI A-to-D high-voltage reference source ADREFLO VCCAD(1)V
ADREFLO A-to-D low-voltage reference source VSSAD(1)ADREFHI V
VAI Analog input voltage ADREFLO ADREFHI V
IAIC Analog input clamp current(2)(VAI <VSSAD -0.3orVAI>VCCAD +0.3) -2 2 mA
(1) ForICCAD andICCREFHI seeSection 5.7.
(2) Ifashared channel isbeing converted byboth ADC converters atthesame time, theon-state leakage isdoubled.Table 7-20. MibADC Electrical Characteristics Over FullRanges ofRecommended Operating
Conditions(1)
PARAMETER DESCRIPTION/CONDITIONS MIN MAX UNIT
Rmux Analog input mux on-resistance See Figure 7-10 250Ω
Rsamp ADC sample switch on-resistance See Figure 7-10 250Ω
Cmux Input mux capacitance See Figure 7-10 16 pF
Csamp ADC sample capacitance See Figure 7-10 13 pF
IAIL Analog off-state input leakage current VCCAD =3.6VVSSAD≤VIN<VSSAD +100mV -300 200
nA VSSAD +100mV≤VIN≤VCCAD -200mV -200 200
VCCAD -200mV<VIN≤VCCAD -200 500
IAIL Analog off-state input leakage current VCCAD =5.25 VVSSAD≤VIN<VSSAD +300mV -1000 250
nA VSSAD +300mV≤VIN≤VCCAD -300mV -250 250
VCCAD -300mV<VIN≤VCCAD -250 1000
IAOSB(2)Analog on-state input bias current VCCAD =3.6VVSSAD≤VIN<VSSAD +100mV -10 2
µA VSSAD +100mV<VIN<VCCAD -200mV -4 2
VCCAD -200mV<VIN<VCCAD -4 16
IAOSB(2)Analog on-state input bias current VCCAD =5.25 VVSSAD≤VIN<VSSAD +300mV -12 3
µA VSSAD +300mV≤VIN≤VCCAD -300mV -5 3
VCCAD -300mV<VIN≤VCCAD -5 18

<!-- Page 174 -->
VS1
On-State
Bias Current
Off-State
LeakagesVS2
VS24IAOSB
IAIL
IAILRext
Rext
RextPin SmuxRmux
PinSmux Rmux
PinSmux Rmux SsampRsamp
Csamp
Cext IAILIAILIAIL
IAILCmuxCextCext
174TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The MibADC clock istheADCLK, generated bydividing down theVCLK1 byaprescale factor defined bytheADCLOCKCR register
bits4:0.
(2) The sample andhold time fortheADC conversions isdefined bytheADCLK frequency andtheAD<GP>SAMP register foreach
conversion group. The sample time must bedetermined byaccounting fortheexternal impedance connected totheinput channel as
well astheinternal impedance oftheADC.
(3) This istheminimum sample/hold andconversion time thatcanbeachieved. These parameters aredependent onmany factors (for
example, theprescale settings).Figure 7-10. MibADC Input Equivalent Circuit
Table 7-21. MibADC Timing Specifications
PARAMETER MIN NOM MAX UNIT
tc(ADCLK)(1)Cycle time, MibADC clock 0.033 µs
td(SH)(2)Delay time, sample andhold time 0.2 µs
12-BIT MODE
td(C) Delay time, conversion time 0.4 µs
td(SHC)(3)Delay time, total sample/hold andconversion time 0.6 µs
10-BIT MODE
td(C) Delay time, conversion time 0.33 µs
td(SHC)(3)Delay time, total sample/hold andconversion time 0.53 µs

<!-- Page 175 -->
175TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) 1LSB =(AD REFHI -ADREFLO )/212for12-bit mode
(2) 1LSB =(AD REFHI -ADREFLO )/210for10-bit modeTable 7-22. MibADC Operating Characteristics Over 3.0Vto3.6VOperating Conditions(1)(2)
PARAMETER DESCRIPTION/CONDITIONS MIN MAX UNIT
CRConversion range over which specified
accuracy ismaintainedADREFHI -ADREFLO 3 3.6 V
ZSE
TZero Scale OffsetDifference between thefirstideal transition (from
code 000h to001h) andtheactual transition10-bit mode 1LSB
12-bit mode 2LSB
FSE
TFullScale OffsetDifference between therange ofthemeasured
code transitions (from firsttolast) andtherange
oftheideal code transitions10-bit mode 2LSB
12-bit mode 3LSB
EDN
LDifferential nonlinearity errorDifference between theactual step width andthe
ideal value. (See Figure 7-11)10-bit mode -1 1.5 LSB
12-bit mode -1 2LSB
EIN
LIntegral nonlinearity errorMaximum deviation from thebest straight line
through theMibADC. MibADC transfer
characteristics, excluding thequantization error.10-bit mode -2 2LSB
12-bit mode -2 2LSB
ETO
TTotal unadjusted error (after calibration)Maximum value ofthedifference between an
analog value andtheideal midstep value.10-bit mode -2 2LSB
12-bit mode -4 4LSB
(1) 1LSB =(AD REFHI -ADREFLO )/212for12-bit mode
(2) 1LSB =(AD REFHI -ADREFLO )/210for10-bit modeTable 7-23. MibADC Operating Characteristics Over 3.6Vto5.25 VOperating Conditions(1)(2)
PARAMETER DESCRIPTION/CONDITIONS MIN MAX UNIT
CRConversion range over which specified
accuracy ismaintainedADREFHI -ADREFLO 3.6 5.25 V
ZSE
TZero Scale OffsetDifference between thefirstideal transition (from
code 000h to001h) andtheactual transition10-bit mode 1LSB
12-bit mode 2LSB
FSE
TFullScale OffsetDifference between therange ofthemeasured
code transitions (from firsttolast) andtherange
oftheideal code transitions10-bit mode 2LSB
12-bit mode 3LSB
EDN
LDifferential nonlinearity errorDifference between theactual step width andthe
ideal value. (See Figure 7-11)10-bit mode -1 1.5 LSB
12-bit mode -1 3LSB
EIN
LIntegral nonlinearity errorMaximum deviation from thebest straight line
through theMibADC. MibADC transfer
characteristics, excluding thequantization error.10-bit mode -2 2LSB
12-bit mode -4.5 2LSB
ETO
TTotal unadjusted error (after calibration)Maximum value ofthedifference between an
analog value andtheideal midstep value.10-bit mode -2 2LSB
12-bit mode -6 5LSB

<!-- Page 176 -->
Differential Linearity
Error (-½ LSB)1 LSB
1 LSBDifferential Linearity
Error (-½ LSB)0 ... 110
0 ... 101
0 ... 100
0 ... 011
0 ... 010
0 ... 001
0 ... 000
0 1 2 3 4 5Digital Output Code
Analog Input Value (LSB)
176TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.4.4 Performance (Accuracy) Specifications
7.4.4.1 MibADC Nonlinearity Errors
The differential nonlinearity error shown inFigure 7-11 (sometimes referred toasdifferential linearity) is
thedifference between anactual step width andtheideal value of1LSB.
A. 1LSB =(AD REFHI -ADREFLO )/212
Figure 7-11. Differential Nonlinearity (DNL) Error

<!-- Page 177 -->
0 ... 111
0 ... 101
0 ... 100
0 ... 011
0 ... 010
0 ... 001
0 ... 000
0 1 2 3 4 5Digital Output Code
Analog Input Value (LSB)0 ... 110
6 7At Transition
011/100
(-½ LSB)
At Transition
001/010 (-1/4 LSB)Actual
TransitionIdeal
Transition
End-Point Lin. Error
177TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedThe integral nonlinearity error shown inFigure 7-12 (sometimes referred toaslinearity error) isthe
deviation ofthevalues ontheactual transfer function from astraight line.
A. 1LSB =(AD REFHI -ADREFLO )/212
Figure 7-12. Integral Nonlinearity (INL) Error(A)

<!-- Page 178 -->
0 ... 111
0 ... 101
0 ... 100
0 ... 011
0 ... 010
0 ... 001
0 ... 000
0 1 2 3 4 5Digital Output Code
Analog Input Value (LSB)0 ... 110
6 7Total Error
At Step
0 ... 001 (1/2 LSB)Total Error
At Step 0 ... 101
(-1 1/4 LSB)
178TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.4.4.2 MibADC Total Error
The absolute accuracy ortotal error ofanMibADC asshown inFigure 7-13 isthemaximum value ofthe
difference between ananalog value andtheideal midstep value.
A. 1LSB =(AD REFHI -ADREFLO )/212
Figure 7-13. Absolute Accuracy (Total) Error(A)