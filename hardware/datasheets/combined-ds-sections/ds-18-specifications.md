# Specifications (Electrical, Timing)

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 55-63 (9 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 55 -->
55TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Stresses beyond those listed under Absolute Maximum Ratings may cause permanent damage tothedevice. These arestress ratings
only, andfunctional operation ofthedevice atthese oranyother conditions beyond those indicated under Recommended Operating
Conditions isnotimplied. Exposure toabsolute-maximum-rated conditions forextended periods may affect device reliability.
(2) Maximum-rated conditions forextended periods may affect device reliability. Allvoltage values arewith respect totheir associated
grounds.5Specifications
5.1 Absolute Maximum Ratings(1)
Over Operating Free-Air Temperature Range
MIN MAX UNIT
Supply voltageVCC(2)-0.3 1.43
V VCCIO,VCCP(2)-0.3 4.6
VCCAD -0.3 6.25
Input voltageAllinput pins, with exception ofADC pins -0.3 4.6
V
ADC input pins -0.3 6.25
Input clamp current:IIK(VI<0orVI>VCCIO)
Allpins, except AD1IN[31:0] andAD2IN[24:0]-20 20
mA IIK(VI<0orVI>VCCAD )
AD1IN[31:0] andAD2IN[24:0]-10 10
Total -40 40
Operating free-air temperature (TA) -40 125 °C
Operating junction temperature (TJ) -40 150 °C
Storage temperature (Tstg) -65 150 °C
(1) AEC Q100-002D indicates HBM stressing isdone inaccordance with theANSI/ESDA/JEDEC JS-001-2011 specification.5.2 ESD Ratings
MIN MAX UNIT
VESDElectrostatic discharge (ESD)
performance:Human Body Model (HBM) ,perAEC Q100-002D(1)-2 2 kV
Charged Device Model (CDM) ,perAEC
Q100-011Allpins except corner
balls-500 500 V
Corner balls -750 750 V
(1) POH represent device operation under thespecified nominal conditions continuously fortheduration ofthecalculated lifetime.5.3 Power-On Hours (POH)
POH isafunction ofvoltage and temperature. Usage athigher voltages and temperatures willresult ina
reduction inPOH toachieve thesame reliability performance. The POH information inTable 5-1is
provided solely forconvenience anddoes notextend ormodify thewarranty provided under TI'sstandard
terms and conditions forTISemiconductor Products. Toavoid significant device degradation, thedevice
POH must belimited tothose listed inTable 5-1.Toconvert toequivalent POH foraspecific temperature
profile, see theCalculating Equivalent Power-on-Hours forHercules Safety MCUs Application Report
(SPNA207 ).
Table 5-1.Power-On Hours Limits
NOMINAL VCCVOLTAGE (V)JUNCTION
TEMPERATURE (TJ)LIFETIME POH(1)
1.2V 105ºC 100K

<!-- Page 56 -->
56TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Allvoltages arewith respect toVSS,except VCCAD ,which iswith respect toVSSAD .5.4 Device Recommended Operating Conditions(1)
MIN NOM MAX UNIT
VCC Digital logic supply voltage (Core) 1.14 1.2 1.32 V
VCCPLL PLL supply voltage 1.14 1.2 1.32 V
VCCIO Digital logic supply voltage (I/O) 3 3.3 3.6 V
VCCAD MibADC supply voltage 3 5.25 V
VCCP Flash pump supply voltage 3 3.3 3.6 V
VSS Digital logic supply ground 0 V
VSSAD MibADC supply ground -0.1 0.1 V
VADREFHI Analog-to-Digital (A-to-D) high-voltage reference source VSSAD VCCAD V
VADREFLO A-to-D low-voltage reference source VSSAD VCCAD V
TA Operating free-air temperature -40 125 °C
TJ Operating junction temperature -40 150 °C

<!-- Page 57 -->
57TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated5.5 Switching Characteristics over Recommended Operating Conditions forClock Domains
Table 5-2.Clock Domain Timing Specifications
PARAMETERTEST
CONDITIONSMIN MAX UNIT
fOSC OSC -oscillator clock frequency using anexternal crystal 5 20 MHz
fGCLK1 GCLK -R5F CPU clock frequency 300 MHz
fGCLK2 GCLK -R5F CPU clock frequency 300 MHz
fHCLK HCLK -System clock frequency 150 MHz
fVCLK VCLK -Primary peripheral clock frequency 110 MHz
fVCLK2 VCLK2 -Secondary peripheral clock frequency 110 MHz
fVCLK3 VCLK3 -Secondary peripheral clock frequency 150 MHz
fVCLKA1 VCLKA1 -Primary asynchronous peripheral clock frequency 110 MHz
fVCLKA2 VCLKA2 -Secondary asynchronous peripheral clock frequency 110 MHz
fVCLKA4 VCLKA4 -Secondary asynchronous peripheral clock frequency 110 MHz
fRTICLK1 RTICLK1 -clock frequency fVCLK MHz
fPROG/ERASE System clock frequency -flash programming/erase fHCLK MHz
fECLK 1 External Clock 1 110 MHz
fECLK2 External Clock 2 110 MHz
fETMCLKOUT ETM trace clock output 55 MHz
fETMCLKIN ETM trace clock input 110 MHz
fEXTCLKIN1 External input clock 1 110 MHz
fEXTCLKIN2 External input clock 2 110 MHz
Table 5-2lists themaximum frequency oftheCPU (GLKx), thelevel-2 memory (HCLK) andtheperipheral
clocks (VCLKx). Itisnotalways possible toruneach clock atitsmaximum frequency asGCLK must bean
integral multiple ofHCLK andHCLK must beanintegral multiple ofVCLKx. Depending onthesystem, the
optimum performance may beobtained bymaximizing either theCPU frequency, thelevel-two RAM
interface, thelevel-two flash interface, ortheperipherals.
5.6 Wait States Required -L2Memories
Wait states arecycles theCPU must wait inorder toretrieve data from thememories which have access
times longer than aCPU clock. Memory wrapper, SCR interconnect and theCPU itself may introduce
additional cycles oflatency duetologic pipelining andsynchronization. Therefore, thetotal latency cycles
asseen bytheCPU canbemore than thenumber ofwait states tocover thememory access time.
Figure 5-1shows only thenumber ofprogrammable wait states needed forL2flash memory atdifferent
frequencies. The number ofwait states iscorrelated toHCLK frequency. The clock ratio between CPU
clock (GCLKx) and HCLK canvary. Therefore, thetotal number ofwait states interms ofGCLKx canbe
obtained bytaking theprogrammed wait states multiplied bytheclock ratio.
There isnouser programmable wait state forL2SRAM access. L2SRAM isclocked byHCLK and is
limited tomaximum 150MHz.

<!-- Page 58 -->
Data WaitstatesRAM
RWAIT SettingFlash (Main Memory)
HCLK = 0MHzHCLK = 0MHz
90MHz 45MHz0 1 30
135MHz2
EWAIT Setting
HCLK = 0MHz1 6 4150MHz
150MHz
150MHzEEPROM Flash (BUS2)
7
60MHz 90MHz 120MHz 45MHz2
75MHz3 8
135MHz 105MHz5
58TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 5-1.Wait States Scheme
L2flash isclocked byHCLK andislimited tomaximum 150MHz. The L2flash cansupport zero data wait
state upto45MHz.

<!-- Page 59 -->
59TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The typical value istheaverage current forthenominal process corner andjunction temperature of25ºC.
(2) Themaximum ICC,value canbederated
*linearly with voltage
*by1.8mA/MHz forlower GCLK frequency when fGCLK =2*fHCLK=4*fVCLK
*forlower junction temperature bytheequation below where TJKisthejunction temperature inKelvin andtheresult isinmilliamperes.
405-0.2e0.018 T
JK
(3) Themaximum ICC,value canbederated
*linearly with voltage
*by3.2mA/MHz forlower GCLK frequency
*forlower junction temperature bytheequation below where TJKisthejunction temperature inKelvin andtheresult isinmilliamperes.
405-0.2e0.018 T
JK
(4) LBIST andPBIST currents areforashort duration, typically less than 10ms.They areusually ignored forthermal calculations forthe
device andthevoltage regulator.5.7 Power Consumption Summary
Over Recommended Operating Conditions
PARAMETER TEST CONDITIONS MIN TYP(1)MAX UNIT
ICCVCCdigital supply andPLL current
(operating mode)fGCLK =300MHz,
fHCLK =150MHz,
fVCLK =75MHz,
fVCLK2 =75MHz,
fVCLK3 =150MHz510 990(2)mA
VCCdigital supply andPLL current
(LBIST mode, orPBIST mode)LBIST clock rate =75MHz
880 1375(3)(4)
mA PBIST ROM clock frequency =75MHz
ICCIO VCCIO digital supply current (operating mode) NoDCload, VCCmax 15 mA
ICCAD VCCAD supply current (operating mode)Single ADC operational, VCCADmax 15 mA
Single ADC power down, VCCADmax 5µA
Both ADCs operational, VCCADmax 30 mA
ICCREF
HIADREFHI supply current (operating mode)Single ADC operational, ADREFHImax 5mA
Both ADCs operational, ADREFHImax 10 mA
ICCP VCCPpump supply currentRead operation oftwobanks inparallel,
VCCPmax70 mA
Read from twobanks andprogram or
erase another bank, VCCPmax93 mA

<!-- Page 60 -->
60TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Source currents (out ofthedevice) arenegative while sink currents (into thedevice) arepositive.
(2) This does notapply tothenPORRST pin.5.8 Input/Output Electrical Characteristics Over Recommended Operating Conditions(1)
PARAMETER TEST CONDITIONS MIN TYP MAX UNIT
Vhys Input hysteresisAllinputs (except
FRAYRX1,
FRAYRX2)180 mV
FRAYRX1, FRAYRX2 100 mV
VIL Low-level input voltageAllinputs(2)(except
FRAYRX1,
FRAYRX2)-0.3 0.8 V
FRAYRX1, FRAYRX2 0.4*VCCIO V
VIH High-level input voltageAllinputs(2)(except
FRAYRX1,
FRAYRX2)2 VCCIO +0.3 V
FRAYRX1, FRAYRX2 0.6*VCCIO V
VOL Low-level output voltageIOL=IOLmax 0.2*VCCIO
V IOL=50µA,standard
output mode0.2
VOH High-level output voltageIOH=IOHmax 0.8*VCCIO
V IOH=50µA,standard
output modeVCCIO -0.3
IIC Input clamp current (I/Opins)VI<VSSIO -0.3orVI
>VCCIO +0.3-3.5 3.5 mA
II Input current (I/Opins)IIHPulldown 20µA VI=VCCIO 5 40
µAIIHPulldown 100µA VI=VCCIO 40 195
IILPullup 20µA VI=VSS -40 -5
IILPullup 100µA VI=VSS -195 -40
Allother pins Nopullup orpulldown -1 1
IOL Low-level output currentPins with output
buffers of8mAdrive
strengthVOLmax
8
mAPins with output
buffers of4mAdrive
strength4
Pins with output
buffers of2mAdrive
strength2
IOH High-level output currentPins with output
buffers of8mAdrive
strengthVOLmin
-8
mAPins with output
buffers of4mAdrive
strength-4
Pins with output
buffers of2mAdrive
strength-2
CI Input capacitance 2 pF
CO Output capacitance 3 pF

<!-- Page 61 -->
VCCIO
0.6*VCCIO
0.4*VCCIO
0Inputtpw
0.6*VCCIO
0.4*VCCIO
VCCIO
VIHVIH
VIL0Inputtpw
VIL
61TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) Formore information about traditional andnew thermal metrics, seetheSemiconductor andICPackage Thermal Metrics application
report SPRA9535.9 Thermal Resistance Characteristics fortheBGA Package (ZWT)
Over operating free-air temperature range (unless otherwise noted)(1)
°C/W
RΘJA Junction-to-free airthermal resistance, stillair(includes 5×5thermal viacluster in2s2p PCB connected to1stground plane) 14.3
RΘJB Junction-to-board thermal resistance (includes 5×5thermal viacluster in2s2p PCB connected to1stground plane) 5.49
RΘJC Junction-to-case thermal resistance (2s0p PCB) 5.02
ΨJT Junction-to-package top,stillair(includes 5×5thermal viacluster in2s2p PCB connected to1stground plane) 0.29
ΨJB Junction-to-board, stillair(includes 5×5thermal viacluster in2s2p PCB connected to1stground plane) 6.41
(1) tc(VCLK) =peripheral VBUS clock cycle time =1/f(VCLK)
(2) The timing shown above isonly valid forpinused ingeneral-purpose input mode.5.10 Timing andSwitching Characteristics
5.10.1 Input Timings
Figure 5-2.TTL-Level Inputs
Table 5-3.Timing Requirements forInputs(1)
MIN MAX UNIT
tpw Input minimum pulse width tc(VCLK) +10(2)ns
tin_slew Time forinput signal togofrom VILtoVIHorfrom VIHtoVIL 1 ns
(1) tc(VCLKA2) =sample clock cycle time forFlexRay =1/f(VCLKA2)Figure 5-3.FlexRay Inputs
Table 5-4.Timing Requirements forFlexRay Inputs(1)
MIN MAX UNIT
tpw Input minimum pulse width tomeet theFlexRay sampling requirementtc(VCLKA2) +
2.5ns

<!-- Page 62 -->
62TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated5.10.2 Output Timings
Table 5-5.Switching Characteristics forOutput Timings versus Load Capacitance (CL)
PARAMETER MIN MAX UNIT
Rise time, tr
8mAlowEMI pinsCL=15pF 2.5
nsCL=50pF 4
CL=100pF 7.2
CL=150pF 12.5
Falltime, tfCL=15pF 2.5
nsCL=50pF 4
CL=100pF 7.2
CL=150pF 12.5
Rise time, tr
4mAlowEMI pinsCL=15pF 5.6
nsCL=50pF 10.4
CL=100pF 16.8
CL=150pF 23.2
Falltime, tfCL=15pF 5.6
nsCL= 50pF 10.4
CL=100pF 16.8
CL=150pF 23.2
Rise time, tr
2mA-z lowEMI pinsCL=15pF 8
nsCL=50pF 15
CL=100pF 23
CL=150pF 33
Falltime, tfCL=15pF 8
nsCL=50pF 15
CL=100pF 23
CL=150pF 33
Rise time, tr
Selectable 8mA /2mA-z pins8mAmodeCL=15pF 2.5
nsCL=50pF 4
CL=100pF 7.2
CL=150pF 12.5
Falltime, tfCL=15pF 2.5
nsCL=50pF 4
CL=100pF 7.2
CL=150pF 12.5
Rise time, tr
2mA-z modeCL=15pF 8
nsCL=50pF 15
CL=100pF 23
CL=150pF 33
Falltime, tfCL=15pF 8
nsCL=50pF 15
CL=100pF 23
CL=150pF 33

<!-- Page 63 -->
tftr
VCCIOVOH VOH
VOL VOL0Output
63TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) This specification does notaccount foranyoutput buffer drive strength differences oranyexternal capacitive loading differences. Check
foroutput buffer drive strength information oneach signal.Figure 5-4.CMOS-Level Outputs
Table 5-6.Timing Requirements forOutputs(1)
MIN MAX UNIT
td(parallel_out)Delay between lowtohigh, orhigh tolowtransition ofgeneral-purpose output signals
thatcanbeconfigured byanapplication inparallel, forexample, allsignals ina
GIOA port, orallN2HET1 signals, andsoforth.6 ns