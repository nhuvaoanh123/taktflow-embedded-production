# Enhanced Quadrature Encoder

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 164-165 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 164 -->
EQEPxA or EQEPxB
(x = 1 or 2)
eQEPxdouble
sync
(x = 1 or 2) 6 VCLK3
Cycles Filter
EQEP1A
VIMEQEP1INTnEQEP1
Module
IO
MuxEQEP1ENCLK
EQEP1IOEQEP1IVBus32
VCLK3
SYS_nRSTEQEP1B
EQEP1IOE
EQEP1SOEQEP1S
EQEP1SOEEQEP1ERREPWM1/../7
TZ4n
EQEP2A
VIMEQEP2INTnEQEP2
ModuleEQEP2ENCLK
EQEP2IOEQEP2IVBus32
VCLK3
SYS_nRSTEQEP2B
EQEP2IOE
EQEP2SOEQEP2S
EQEP2SOEEQEP2ERRConnection
Selection
Muxsee Note A
see Note A
164TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.3 Enhanced Quadrature Encoder (eQEP)
Figure 7-6shows theeQEP module interconnections onthedevice.
A. Formore detail ontheeQEPx input synchronization selection, seeFigure 7-7.
Figure 7-6.eQEP Module Interconnections
Figure 7-7shows thedetailed input synchronization selection (asynchronous, double-synchronous, or
double synchronous +filter width) foreQEPx.
Figure 7-7.eQEPx Input Synchronization Selection Detail
7.3.1 Clock Enable Control foreQEPx Modules
Each oftheEQEPx modules has aclock enable (EQEPxENCLK) which iscontrolled byitsrespective
Peripheral Power Down bitinthePSPWRDWNCLRx register ofthePCR2 module. Toproperly reset the
peripherals, theperipherals must bereleased from reset bysetting thePENA bitoftheCLKCNTL register
inthesystem module. Inaddition, theperipherals must bereleased from their power down state by
clearing therespective bitinthePSPWRDWNCLRx register. Bydefault after reset, theperipherals arein
power down state.
Table 7-11. eQEPx Clock Enable Control
eQEP MODULE INSTANCECONTROL REGISTER TO
ENABLE CLOCKDEFAULT VALUE
eQEP1 PSPWRDWNCLR3[5] 1
eQEP2 PSPWRDWNCLR3[6] 1

<!-- Page 165 -->
165TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The filter width is6VCLK3 cycles.7.3.2 Using eQEPx Phase Error toTrip ePWMx Outputs
The eQEP module sets theEQEPERR signal output whenever aphase error isdetected initsinputs
EQEPxA andEQEPxB. This error signal from both theeQEP modules isinput totheconnection selection
multiplexer. This multiplexer isdefined inTable 7-3.Asshown inFigure 7-6,theoutput ofthisselection
multiplexer isinverted and connected totheTZ4n trip-zone input ofallePWMx modules. This connection
allows theapplication todefine theresponse ofeach ePWMx module onaphase error indicated bythe
eQEP modules.
7.3.3 Input Connection toeQEPx Modules
The input connection toeach ofthe eQEP modules can beselected between adouble-VCLK3-
synchronized input oradouble-VCLK3-synchronized andfiltered input, aslisted inTable 7-12.
Table 7-12. Device-Level Input Connection toeQEPx Modules
INPUT SIGNALCONTROL FOR DOUBLE-SYNCHRONIZED
CONNECTION TOeQEPxCONTROL FOR
DOUBLE-SYNCHRONIZED AND
FILTERED CONNECTION(1)TOeQEPx
eQEP1A PINMMR170[18:16] =001 PINMMR170[18:16] =010
eQEP1B PINMMR170[26:24] =001 PINMMR170[26:24] =010
eQEP1I PINMMR171[2:0] =001 PINMMR171[2:0] =010
eQEP1S PINMMR171[10:8] =001 PINMMR171[10:8] =010
eQEP2A PINMMR171[18:16] =001 PINMMR171[18:16] =010
eQEP2B PINMMR171[26:24] =001 PINMMR171[26:24] =010
eQEP2I PINMMR172[2:0] =001 PINMMR172[2:0] =010
eQEP2S PINMMR172[10:8] =001 PINMMR172[10:8] =010
(1) The filter width is6VCLK3 cycles.7.3.4 Enhanced Quadrature Encoder Pulse (eQEPx) Timing
Table 7-13. eQEPx Timing Requirements(1)
TEST CONDITIONS MIN MAX UNIT
tw(QEPP) QEP input periodSynchronous 2tc(VCLK3)cycles
Synchronous with input filter 2tc(VCLK3) +filter width
tw(INDEXH) QEP Index Input High TimeSynchronous 2tc(VCLK3)cycles
Synchronous with input filter 2tc(VCLK3) +filter width
tw(INDEXL) QEP Index Input Low TimeSynchronous 2tc(VCLK3)cycles
Synchronous with input filter 2tc(VCLK3) +filter width
tw(STROBH) QEP Strobe Input High TimeSynchronous 2tc(VCLK3)cycles
Synchronous with input filter 2tc(VCLK3) +filter width
tw(STROBL) QEP Strobe Input Low TimeSynchronous 2tc(VCLK3)cycles
Synchronous with input filter 2tc(VCLK3) +filter width
Table 7-14. eQEPx Switching Characteristics
PARAMETER MIN MAX UNIT
td(CNTR)xin Delay time, external clock tocounter increment 4tc(VCLK3) cycles
td(PCS-OUT)QEP Delay time, QEP input edge toposition compare sync output 6tc(VCLK3) cycles