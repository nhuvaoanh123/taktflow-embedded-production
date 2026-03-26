# Enhanced Capture Modules

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 161-163 (3 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 161 -->
IOMUXEPWM1SYNCO
eCAP1
eCAP
6ECAP1INTn
ECAP1ENCLKVCLK3, SYS_nRST
ECAP2ENCLKeCAP
2/3/4/5
VCLK3, SYS_nRSTECAP2INTn
ECAP6INTnVBus32VBus32VIM
VIM
VIMECAP1ECAP1SYNCI
ECAP1SYNCO
ECAP2SYNCI
ECAP2SYNCOECAP2
ECAP6ENCLKVCLK3, SYS_nRSTVBus32ECAP6ECAP6SYNCI
ECAP6SYNCOsee Note A
see Note Asee Note A
161TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.2 Enhanced Capture Modules (eCAP)
Figure 7-4shows how theeCAP modules areinterconnected onthismicrocontroller.
A. Formore detail ontheeCAPx input synchronization selection, seeFigure 7-5.
Figure 7-4.eCAP Module Connections

<!-- Page 162 -->
ECAPx
(x = 1, 2, 3, 4, 5, or 6)
eCAPxdouble
sync
(x = 1 through 6) 6 VCLK3
Cycles Filter
162TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-5shows thedetailed input synchronization selection (asynchronous, double-synchronous, or
double synchronous +filter width) foreCAPx.
Figure 7-5.eCAPx Input Synchronization Selection Detail
7.2.1 Clock Enable Control foreCAPx Modules
Each oftheeCAPx modules has aclock enable (ECAPxENCLK) which iscontrolled byitsrespective
Peripheral Power Down bitinthePSPWRDWNCLRx register ofthePCR2 module. Toproperly reset the
peripherals, theperipherals must bereleased from reset bysetting thePENA bitoftheCLKCNTL register
inthesystem module. Inaddition, theperipherals must bereleased from their power down state by
clearing therespective bitinthePSPWRDWNCLRx register. Bydefault, after reset, theperipherals arein
thepower down state.
Table 7-7.eCAPx Clock Enable Control
eCAP MODULE INSTANCECONTROL REGISTER TO
ENABLE CLOCKDEFAULT VALUE
eCAP1 PSPWRDWNCLR3[15] 1
eCAP2 PSPWRDWNCLR3[8] 1
eCAP3 PSPWRDWNCLR3[9] 1
eCAP4 PSPWRDWNCLR3[10] 1
eCAP5 PSPWRDWNCLR3[11] 1
eCAP6 PSPWRDWNCLR3[4] 1
(1) The filter width is6VCLK3 cycles.7.2.2 PWM Output Capability ofeCAPx
When notused incapture mode, each oftheeCAPx modules can beused asasingle-channel PWM
output. This iscalled theAuxiliary PWM (APWM) mode ofoperation oftheeCAPx modules. Formore
information, seetheeCAP module chapter ofthedevice-specific TRM.
7.2.3 Input Connection toeCAPx Modules
The input connection toeach ofthe eCAPx modules can beselected between adouble-VCLK3-
synchronized input oradouble-VCLK3-synchronized andfiltered input, aslisted inTable 7-8.
Table 7-8.Device-Level Input Connection toeCAPx Modules
INPUT SIGNALCONTROL FOR
DOUBLE-SYNCHRONIZED
CONNECTION TOeCAPxCONTROL FOR
DOUBLE-SYNCHRONIZED AND
FILTERED CONNECTION TOeCAPx(1)
eCAP1 PINMMR169[2:0] =001 PINMMR169[2:0] =010
eCAP2 PINMMR169[10:8] =001 PINMMR169[10:8] =010
eCAP3 PINMMR169[18:16] =001 PINMMR169[18:16] =010
eCAP4 PINMMR169[26:24] =001 PINMMR169[26:24] =010
eCAP5 PINMMR170[2:0] =001 PINMMR170[2:0] =010
eCAP6 PINMMR170[10:8] =001 PINMMR170[10:8] =010

<!-- Page 163 -->
163TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The filter width is6VCLK3 cycles.7.2.4 Enhanced Capture Module (eCAP) Electrical Data/Timing
Table 7-9.eCAPx Timing Requirements
TEST CONDITIONS MIN MAX UNIT
tw(CAP) Pulse width, capture inputSynchronous 2tc(VCLK3) cycles
Synchronous with input filter 2tc(VCLK3) +filter width(1)cycles
Table 7-10. eCAPx Switching Characteristics
PARAMETER TEST CONDITIONS MIN MAX UNIT
tw(APWM) Pulse duration, APWMx output high orlow 20 ns