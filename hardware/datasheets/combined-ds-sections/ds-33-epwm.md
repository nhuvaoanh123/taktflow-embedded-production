# Enhanced PWM Modules

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 156-160 (5 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 156 -->
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
156TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7Peripheral Information andElectrical Specifications
7.1 Enhanced Translator PWM Modules (ePWM)
Figure 7-1shows theconnections between theseven ePWM modules (ePWM1 -ePWM7) onthedevice.
A. Formore detail ontheePWMx input synchronization selection, seeFigure 7-2.
Figure 7-1.ePWMx Module Interconnections

<!-- Page 157 -->
TZxn
(x = 1, 2, or 3)
ePWMxdouble
sync
(x = 1 through 7) 6 VCLK3
Cycles Filter
157TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-2shows thedetailed input synchronization selection (asynchronous, double-synchronous, or
double synchronous +filter width) forePWMx.
Figure 7-2.ePWMx Input Synchronization Selection Detail
7.1.1 ePWM Clocking andReset
Each ePWM module hasaclock enable (ePWMxENCLK) which iscontrolled byitsrespective Peripheral
Power Down bitinthePSPWRDWNCLRx register ofthePCR2 module. Toproperly reset theperipherals,
theperipherals must bereleased from reset bysetting thePENA bitoftheCLKCNTL register inthe
system module. Inadditional, theperipherals must bereleased from their power down state byclearing
their respective bitinthePSPWRDWNCLRx register. Bydefault after reset, theperipherals are in
powerdown state.
Table 7-1.ePWMx Clock Enable Control
ePWM MODULE INSTANCECONTROL REGISTER TO
ENABLE CLOCKDEFAULT VALUE
ePWM1 PSPWRDWNCLR3[16] 1
ePWM2 PSPWRDWNCLR3[17] 1
ePWM3 PSPWRDWNCLR3[18] 1
ePWM4 PSPWRDWNCLR3[19] 1
ePWM5 PSPWRDWNCLR3[12] 1
ePWM6 PSPWRDWNCLR3[13] 1
ePWM7 PSPWRDWNCLR3[14] 1
7.1.2 Synchronization ofePWMx Time-Base Counters
Atime-base synchronization scheme connects alloftheePWM modules onadevice. Each ePWM
module hasasynchronization input (EPWMxSYNCI) andasynchronization output (EPWMxSYNCO). The
input synchronization forthefirst instance (ePWM1) comes from anexternal pin. Figure 7-1shows the
synchronization connections foralltheePWMx modules. Each ePWM module canbeconfigured touseor
ignore thesynchronization input. Formore information, see theePWM module chapter ofthedevice-
specific TRM.

<!-- Page 158 -->
N2HET1 N2HET2
ePWM12 VCLK3 cycles
Pulse Stretch
EPWM1SYNCISYNCIEXT_LOOP_SYNC N2HET1_LOOP_SYNC
double
syncPINMMR165[24]=0 and PINMMR165[25]=1
6 VCLK3
Cycles Filter
158TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.1.3 Synchronizing allePWM Modules totheN2HET1 Module Time Base
The connection between the NHET1_LOOP_SYNC and the SYNCI input ofePWM1 module is
implemented asshown inFigure 7-3.
Figure 7-3.Synchronizing Time Bases Between N2HET1, N2HET2 andePWMx Modules
7.1.4 Phase-Locking theTime-Base Clocks ofMultiple ePWM Modules
The TBCLKSYNC bitcan beused toglobally synchronize thetime-base clocks ofallenabled ePWM
modules onadevice. This bitisimplemented asPINMMR166[1] register bit1.
When TBCLKSYNC =0,thetime-base clock ofallePWM modules isstopped. This isthedefault
condition.
When TBCLKSYNC =1,allePWM time-base clocks arestarted with therising edge ofTBCLK aligned.
Forperfectly synchronized TBCLKs, theprescaler bitsintheTBCTL register ofeach ePWM module must
besetidentically. The proper procedure forenabling theePWM clocks isasfollows:
*Each ePWM isindividually associated with apower down bitinthePSPWRDWNCLRx register ofthePCR2
module. Enable theindividual ePWM module clocks (ifdisable) using thecontrol registers inthePCR2.
*Configure TBCLKSYNC =0.This willstop thetime-base clock within anyenabled ePWM module.
*Configure theprescaler values anddesired ePWM modes.
*Configure TBCLKSYNC =1.
7.1.5 ePWM Synchronization with External Devices
The output sync from theePWM1 module isalso exported totheI/OMux such that multiple devices can
besynchronized together. The signal pulse must bestretched by8VCLK3 cycles before being exported
ontheIOMux pinastheePWMSYNCO signal.

<!-- Page 159 -->
159TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The filter width is6VCLK3 cycles.7.1.6 ePWM Trip Zones
The ePWMx modules have 6tripzone inputs each. These areactive-low signals. The application can
control theePWMx module response toeach ofthetripzone input separately. The timing requirements
from theassertion ofthetripzone inputs totheactual response arespecified intheelectrical and timing
section ofthisdocument.
7.1.6.1 Trip Zones TZ1n, TZ2n, TZ3n
These 3tripzone inputs aredriven byexternal circuits and areconnected todevice-level inputs. These
signals areeither connected asynchronously totheePWMx tripzone inputs, ordouble-synchronized with
VCLK3, ordouble-synchronized and then filtered with a6-cycle VCLK3-based counter before connecting
totheePWMx (see Figure 7-2).Bydefault, thetripzone inputs areasynchronously connected tothe
ePWMx modules.
Table 7-2.Connection toePWMx Modules forDevice-Level Trip Zone Inputs
TRIP ZONE
INPUTCONTROL FOR
ASYNCHRONOUS
CONNECTION TOePWMxCONTROL FOR
DOUBLE-SYNCHRONIZED
CONNECTION TOePWMxCONTROL FOR
DOUBLE-SYNCHRONIZED AND
FILTERED CONNECTION TOePWMx(1)
TZ1n PINMMR172[18:16] =001 PINMMR172[18:16] =010 PINMMR172[18:16] =100
TZ2n PINMMR172[26:24] =001 PINMMR172[26:24] =010 PINMMR172[26:24] =100
TZ3n PINMMR173[2:0] =001 PINMMR173[2:0] =010 PINMMR173[2:0] =100
7.1.6.2 Trip Zone TZ4n
This tripzone input isdedicated toeQEPx error indications. There are2eQEP modules onthisdevice.
Each eQEP module indicates aphase error bydriving itsEQEPxERR output high. The following control
registers allow theapplication toconfigure thetripzone input (TZ4n) toeach ePWMx module based on
therequirements oftheapplication application's requirements.
Table 7-3.TZ4n Connections forePWMx Modules
ePWMxCONTROL FOR TZ4n =
NOT(EQEP1ERR OREQEP2ERR)CONTROL FOR TZ4n =
NOT(EQEP1ERR)CONTROL FOR TZ4n =
NOT(EQEP2ERR)
ePWM1 PINMMR167[2:0] =001 PINMMR167[2:0] =010 PINMMR167[2:0] =100
ePWM2 PINMMR167[10:8] =001 PINMMR167[10:8] =010 PINMMR167[10:8] =100
ePWM3 PINMMR167[18:16] =001 PINMMR167[18:16] =010 PINMMR167[18:16] =100
ePWM4 PINMMR167[26:24] =001 PINMMR167[26:24] =010 PINMMR167[26:24] =100
ePWM5 PINMMR168[2:0] =001 PINMMR168[2:0] =010 PINMMR168[2:0] =100
ePWM6 PINMMR168[10:8] =001 PINMMR168[10:8] =010 PINMMR168[10:8] =100
ePWM7 PINMMR168[18:16] =001 PINMMR168[18:16] =010 PINMMR168[18:16] =100
NOTE
The EQEPxERR signal isanactive high signal coming outofEQEPx module. Aslisted in
Table 7-3,theselected combination oftheEQEPxERR signals must beinverted before
connecting totheTZ4n input oftheePWMx modules.

<!-- Page 160 -->
160TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The filter width is6VCLK3 cycles.7.1.6.3 Trip Zone TZ5n
This tripzone input isdedicated toaclock failure onthedevice. That is,thistripzone input isasserted
whenever anoscillator failure oraPLL slipisdetected onthedevice. The applciation can use thistrip
zone input foreach ePWMx module toprevent theexternal system from going outofcontrol when the
device clocks arenotwithin expected range (system running atlimp clock).
The oscillator failure andPLL slipsignals used forthistripzone input aretaken from thestatus flags inthe
system module. These level signals aresetuntil cleared bytheapplication.
7.1.6.4 Trip Zone TZ6n
This tripzone input totheePWMx modules isdedicated toadebug mode entry oftheCPU. Ifenabled,
theuser canforce thePWM outputs toaknown state when theemulator stops theCPU. This prevents the
external system from going outofcontrol when theCPU isstopped.
NOTE
There isasignal called DBGACK that theCPU drives when itenters debug mode. This
signal must beinverted andused astheDebug Mode Entry signal forthetripzone input.
7.1.7 Triggering ofADC Start ofConversion Using ePWMx SOCA andSOCB Outputs
Aspecial scheme isimplemented toselect theactual signal used fortriggering thestart ofconversion on
thetwoADCs onthisdevice. This scheme isdefined inSection 7.4.2.3 .
7.1.8 Enhanced Translator-Pulse Width Modulator (ePWMx) Electrical Data/Timing
Table 7-4.ePWMx Timing Requirements
TEST CONDITIONS MIN MAX UNIT
tw(SYNCIN) Synchronization input pulse widthAsynchronous 2tc(VCLK3) cycles
Synchronous 2tc(VCLK3) cycles
Synchronous with input filter 2tc(VCLK3) +filter width(1)cycles
Table 7-5.ePWMx Switching Characteristics
PARAMETERTEST
CONDITIONSMIN MAX UNIT
tw(PWM) Pulse duration, ePWMx output high orlow 33.33 ns
tw(SYNCOUT) Synchronization Output Pulse Width 8tc(VCLK3) cycles
td(PWM)tzaDelay time, tripinput active toPWM forced high, OR
Delay time, tripinput active toPWM forced lowNopinload 25 ns
td(TZ-PWM)HZ Delay time, tripinput active toPWM Hi-Z 20 ns
(1) The filter width is6VCLK3 cycles.Table 7-6.ePWMx Trip-Zone Timing Requirements
TEST CONDITIONS MIN MAX UNIT
tw(TZ) Pulse duration, TZn input lowAsynchronous 2*TBePWMx
cycles Synchronous 2tc(VCLK3)
Synchronous with input filter 2tc(VCLK3) +filter width(1)