# Clock Sources, PLL, Clock Domains

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 76-88 (13 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 76 -->
OSCIN OSCOUT
C1
(see Note A)C2
Crystal
(a)OSCIN OSCOUT
(b)External
(toggling 0 V to 3.3 V)Clock Signal
Note A: The values of C1 and C2 should be provided by the resonator/crystal vendor.Kelvin_GND
Note B: Kelvin_GND should not be connected to any other GND.(see Note B)
76TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6 Clocks
6.6.1 Clock Sources
Table 6-11 lists theavailable clock sources onthedevice. Each clock source canbeenabled ordisabled
using theCSDISx registers inthesystem module. The clock source number inthetable corresponds to
thecontrol bitintheCSDISx register forthatclock source.
Table 6-11 also lists thedefault state ofeach clock source.
Table 6-11. Available Clock Sources
CLOCK
SOURCE NO.NAME DESCRIPTION DEFAULT STATE
0 OSCIN Main Oscillator Enabled
1 PLL1 Output From PLL1 Disabled
2 Reserved Reserved Disabled
3 EXTCLKIN1 External Clock Input 1 Disabled
4 CLK80K Low-Frequency Output ofInternal Reference Oscillator Enabled
5 CLK10M High-Frequency Output ofInternal Reference Oscillator Enabled
6 PLL2 Output From PLL2 Disabled
7 EXTCLKIN2 External Clock Input 2 Disabled
6.6.1.1 Main Oscillator
The oscillator isenabled byconnecting theappropriate fundamental resonator/crystal andload capacitors
across theexternal OSCIN and OSCOUT pins asshown inFigure 6-3.The oscillator isasingle-stage
inverter held inbias byanintegrated bias resistor. This resistor isdisabled during leakage test
measurement andlowpower modes.
NOTE
TIstrongly encourages each customer tosubmit samples ofthe device tothe
resonator/crystal vendors forvalidation. The vendors areequipped todetermine which load
capacitors willbest tune their resonator/crystal tothemicrocontroller device foroptimum
start-up andoperation over temperature andvoltage extremes.
Anexternal oscillator source canbeused byconnecting a3.3-V clock signal totheOSCIN terminal and
leaving theOSCOUT terminal unconnected (open) asshown inFigure 6-3.
Figure 6-3.Recommended Crystal/Clock Connection

<!-- Page 77 -->
77TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.1.1.1 Timing Requirements forMain Oscillator
Table 6-12. Timing Requirements forMain Oscillator
MIN NOM MAX UNIT
tc(OSC) Cycle time, OSCIN (when using asine-wave input) 50 200 ns
tc(OSC_SQR) Cycle time, OSCIN, (when input totheOSCIN isasquare wave) 50 200 ns
tw(OSCIL) Pulse duration, OSCIN low(when input totheOSCIN isasquare wave) 15 ns
tw(OSCIH) Pulse duration, OSCIN high (when input totheOSCIN isasquare wave) 15 ns

<!-- Page 78 -->
BIAS_EN
Low-Power
OscillatorLFEN
LF_TRIM
HFEN
HF_TRIMLFLPO
HFLPO
HFLPO_VALID
nPORRST
78TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.1.2 Low-Power Oscillator
The Low-Power Oscillator (LPO) iscomprised oftwo oscillators --HFLPO and LFLPO, inasingle
macro.
6.6.1.2.1 Features
The main features oftheLPO are:
*Supplies aclock atextremely lowpower toreduce power consumption. This isconnected asclock
source 4oftheGlobal Clock Module (GCM).
*Supplies ahigh-frequency clock fornontiming-critical systems. This isconnected asclock source 5of
theGCM.
*Provides acomparison clock forthecrystal oscillator failure detection circuit.
Figure 6-4.LPO Block Diagram
Figure 6-4shows ablock diagram oftheinternal reference oscillator. This isalow-power oscillator (LPO)
andprovides twoclock sources: onenominally 80kHz andonenominally 10MHz.
6.6.1.2.2 LPO Electrical andTiming Specifications
Table 6-13. LPO Specifications
PARAMETER MIN TYP MAX UNIT
Clock detectionOscillator failfrequency -lower threshold, using untrimmed
LPO output1.375 2.4 4.875 MHz
Oscillator failfrequency -higher threshold, using untrimmed
LPO output22 38.4 78 MHz
LPO -HFoscillatorUntrimmed frequency 5.5 9 19.5 MHz
Trimmed frequency 8.0 9.6 11.0 MHz
Start-up time from STANDBY (LPO BIAS_EN high foratleast
900µs)10 µs
Cold start-up time 900 µs
LPO -LFoscillatorUntrimmed frequency 36 85 180 kHz
Start-up time from STANDBY (LPO BIAS_EN high foratleast
900µs)100 µs
Cold start-up time 2000 µs

<!-- Page 79 -->
/NR
/1 to /64OSCIN
PLLINTCLK /OD
/1 to /8VCOCLK/R
/1 to /32post_ODCLK
/NF
/1 to /256PLLCLK
/NR2
/1 to /64OSCIN
PLL#2INTCLK2/OD2
/1 to /8VCOCLK2 /R2
/1 to /32post_ODCLK2
/NF2
/1 to /256PLL2CLKfPLLCLK = (fOSCIN / NR) * NF / (OD * R)
fPLL2CLK= (fOSCIN/ NR2) * NF2 / (OD2 * R2)
79TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.1.3 Phase-Locked Loop (PLL) Clock Modules
The PLL isused tomultiply theinput frequency tosome higher frequency.
The main features ofthePLL are:
*Frequency modulation can beoptionally superimposed onthesynthesized frequency ofPLL1. The
frequency modulation capability ofPLL2 ispermanently disabled.
*Configurable frequency multipliers anddividers
*Built-in PLL Slipmonitoring circuit
*Option toreset thedevice onaPLL slipdetection
6.6.1.3.1 Block Diagram
Figure 6-5shows ahigh-level block diagram ofthetwoPLL macros onthismicrocontroller. PLLCTL1 and
PLLCTL2 areused toconfigure themultiplier anddividers forthePLL1. PLLCTL3 isused toconfigure the
multiplier anddividers forPLL2.
Figure 6-5.ZWT PLLx Block Diagram
6.6.1.3.2 PLL Timing Specifications
Table 6-14. PLL Timing Specifications
PARAMETER MIN MAX UNIT
fINTCLK PLL1 Reference Clock frequency 1 20 MHz
fpost_ODCLK Post-ODCLK -PLL1 Post-divider input clock frequency 400 MHz
fVCOCLK VCOCLK -PLL1 Output Divider (OD) input clock frequency 550 MHz
fINTCLK2 PLL2 Reference Clock frequency 1 20 MHz
fpost_ODCLK2 Post-ODCLK -PLL2 Post-divider input clock frequency 400 MHz
fVCOCLK2 VCOCLK -PLL2 Output Divider (OD) input clock frequency 550 MHz

<!-- Page 80 -->
80TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.1.4 External Clock Inputs
The device supports uptotwo external clock inputs. This clock input must beasquare-wave input.
Table 6-15 specifies theelectrical andtiming requirements forthese clock inputs.
Table 6-15. External Clock Timing andElectrical Specifications
PARAMETER MIN MAX UNIT
fEXTCLKx External clock input frequency 80 MHz
tw(EXTCLKIN)H EXTCLK high-pulse duration 6 ns
tw(EXTCLKIN)L EXTCLK low-pulse duration 6 ns
viL(EXTCLKIN) Low-level input voltage -0.3 0.8 V
viH(EXTCLKIN) High-level input voltage 2 VCCIO +0.3 V

<!-- Page 81 -->
81TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.2 Clock Domains
6.6.2.1 Clock Domain Descriptions
Table 6-16 lists thedevice clock domains andtheir default clock sources. Table 6-16 also lists thesystem
module control register thatisused toselect anavailable clock source foreach clock domain.
Table 6-16. Clock Domain Descriptions
CLOCK DOMAIN CLOCK DISABLE BITDEFAULT
SOURCESOURCE SELECTION
REGISTERSPECIAL CONSIDERATIONS
GCLK1 SYS.CDDIS.0 OSCIN SYS.GHVSRC[3:0]* This themain clock from which HCLK is
divided down
* Inphase with HCLK
* Isdisabled separately from HCLK through
theCDDISx registers bit0
* Can bedivided-by-1 upto8when running
CPU self-test (LBIST) using theCLKDIV
field oftheSTCCLKDIV register at
address 0xFFFFE108
GCLK2 SYS.CDDIS.0 OSCIN SYS.GHVSRC[3:0]* Always thesame frequency asGCLK1
* 2cycles delayed from GCLK1
* Isdisabled along with GCLK1
* Gets divided bythesame divider setting
asthatforGCLK1 when running CPU self-
test(LBIST)
HCLK SYS.CDDIS.1 OSCIN SYS.GHVSRC[3:0]* Divided from GCLK1 through
HCLKCNTLregister
* Allowable clock ratio from 1:1to4:1
* Isdisabled through theCDDISx registers
bit1
VCLK SYS.CDDIS.2 OSCIN SYS.GHVSRC[3:0]* Divided down from HCLK through
CLKCNTL register
* Can beHCLK/1, HCLK/2,... orHCLK/16
* Isdisabled separately from HCLK through
theCDDISx registers bit2
* HCLK:VCLK2:VCLK must beinteger ratios
ofeach other
VCLK2 SYS.CDDIS.3 OSCIN SYS.GHVSRC[3:0]* Divided down from HCLK
* Can beHCLK/1, HCLK/2,... orHCLK/16
* Frequency must beaninteger multiple of
VCLK frequency
* Isdisabled separately from HCLK through
theCDDISx registers bit3
VCLK3 SYS.CDDIS.8 OSCIN SYS.GHVSRC[3:0]* Divided down from HCLK
* Can beHCLK/1, HCLK/2,... orHCLK/16
* Isdisabled separately from HCLK through
theCDDISx registers bit8
VCLKA1 SYS.CDDIS.4 VCLK SYS.VCLKASRC[3:0]* Defaults toVCLK asthesource
* Isdisabled through theCDDISx registers
bit4
VCLKA2 SYS.CDDIS.5 VCLK SYS.VCLKASRC[3:0]* Defaults toVCLK asthesource
* Isdisabled through theCDDISx registers
bit5
VCLKA4 SYS.CDDIS.11 VCLK SYS.VCLKACON1[19:16]* Defaults toVCLK asthesource
* Isdisabled through theCDDISx registers
bit11

<!-- Page 82 -->
82TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-16. Clock Domain Descriptions (continued)
CLOCK DOMAIN CLOCK DISABLE BITDEFAULT
SOURCESOURCE SELECTION
REGISTERSPECIAL CONSIDERATIONS
VCLKA4_DIVR SYS.VCLKACON1.20 VCLK SYS.VCLKACON1[19:16]* Divided down from VCLKA4 using the
VCLKA4R field oftheVCLKACON1
register
* Frequency canbeVCLKA4/1,
VCLKA4/2, ...,orVCLKA4/8
* Default frequency isVCLKA4/2
* Isdisabled separately through the
VCLKA4_DIV_CDDIS bitinthe
VCLKACON1 register, iftheVCLKA4 is
notalready disabled
RTICLK 1 SYS.CDDIS.6 VCLK SYS.RCLKSRC[3:0]* Defaults toVCLK asthesource
* Ifaclock source other than VCLK is
selected forRTICLK 1,then theRTICLK 1
frequency must beless than orequal to
VCLK/3
* Application canensure thisby
programming theRTI1DIV field ofthe
RCLKSRC register, ifnecessary
* Isdisabled through theCDDISx registers
bit6

<!-- Page 83 -->
HCLK1 (to SYSTEM)GCLK, GCLK2 (to CPU, CCM)GCM
VCLK (VCLK to peripherals on PCR3)
RTICLK1 (to RTI1, DWWD)/1, 2, 4, or 8
VCLKOSCIN
Low Power
Oscillator 10MHz80kHzFMzPLL
10
4
5/1..64 X1..256 /1..8 /1..32
6PLL # 2*
VCLK
/1,2,..1024
Phase_seg2
CAN Baud RatePhase_seg1VCLKA1
/1,2,..256
SPIx,MibSPIx/2,3..224
LIN, SCISPI LIN / SCI/1,2..32
MibADCxADCLK/1,2..65536
External ClockECLKVCLK2
N2HETxHRP
/1..64
LRP
/20..25
Loop
Resolution ClockHighBaud Rate Baud RateN2HETx
TUVCLK2EthernetVCLKA4_DIVR(SSPLL)(SSPLL)
/1..64 X1..256 /1..8 /1..32 *
EXTCLKIN1
EXTCLKIN23
7 VCLKA1 (to DCANx)CLKSRC(7:0)
VCLK
VCLK3
EMIFVCLK3 (to EMIF, , ePWMx,
and eQEPx)eCAPx
Ethernet/1..16VCLK_s (VCLK to system modules)
* the frequency at this node must not
exceed the maximum HCLK specifiation.
/1,2..256
I2CI2C baud
rateVCLKA2 (to FlexRa y and FTU)
/1,2,..4VCLKA2
GTUC1,2
NTU[1]
NTU[0]NTU[2]NTU[3]
RTI
Macro TickStart of cyclePLL#2 outputEXTCLKIN1VCLKA2
Prop_seg
DCANxFlexRay
Baud
RateFlexRay
TU
FlexRayVCLK2 (to N2HETx and HTUx)
VCLKA4_DIVR_EMAC (to Ethernet,
as alternate for MIIXCLK and/or
MIIRXCLK) VCLKA4 is left open.CLKSRC(7:0)
VCLK
CLKSRC(7:0)
VCLK
CLKSRC(7:0)/1..16/1..16/1..16/1..4
83TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.2.2 Mapping ofClock Domains toDevice Modules
Each clock domain hasadedicated functionality asshown inFigure 6-6.
Figure 6-6.Device Clock Domains

<!-- Page 84 -->
VCLKA4_SRC0
1
3
4
5
6
7
VCLKVCLKA4 (left open)
/DIVR
PLL2 post_ODCLK/8
PLL2 post_ODCLK/16VCLKA4_DIVR_EMAC
(to EMAC)
84TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.3 Special Clock Source Selection Scheme forVCLKA4_DIVR_EMAC
Some applications may need touseboth theFlexRay andtheEthernet interfaces. The FlexRay controller
requires theVCLKA2 frequency tobe80MHz, while theMIIinterface requires VCLKA4_DIVR_EMAC to
be25MHz andtheRMII requires VCLKA4_DIVR_EAMC tobe50MHz.
These different frequencies aresupported byadding special dedicated clock source selection options for
theVCLKA4_DIVR_EMAC clock domain. This logic isshown inFigure 6-7.
Figure 6-7.VCLKA4_DIVR Source Selection Options
The PLL2 post_ODCLK isbrought outasaseparate output from thePLL wrapper module. There aretwo
additional dividers implemented atthedevice-level todivide thisPLL2 post_ODCLK by8andby16.
Asshown inFigure 6-7,theVCLKA4_SRC configured through thesystem module VCLKACON1 control
register isused todetermine theclock source fortheVCLKA4 and VCLKA4_DIVR. Anadditional
multiplexor isimplemented toselect between theVCLKA4_DIVR and thetwoadditional clock sources -
PLL2 post_ODCLK/8 andpost_ODCLK/16.
Table 6-17 lists theVCLKA4_DIVR_EMAC clock source selections.
Table 6-17. VCLKA4_DIVR_EMAC Clock Source Selection
VCLKA4_SRC FROM VCLKACON1[19 -16] CLOCK SOURCE FOR VCLKA4_DIVR_EMAC
0x0 OSCIN /VCLKA4R
0x1 PLL1CLK /VCLKA4R
0x2 Reserved
0x3 EXTCLKIN1 /VCLKA4R
0x4 LFLPO /VCLKA4R
0x5 HFLPO /VCLKA4R
0x6 PLL2CLK /VCLKA4R
0x7 EXTCLKIN2 /VCLKA4R
0x8-0xD VCLK
0xE PLL2 post_ODCLK/8
0xF PLL2 post_ODCLK/16

<!-- Page 85 -->
85TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.6.4 Clock Test Mode
The TMS570 platform architecture defines aspecial mode thatallows various clock signals tobeselected
andoutput ontheECLK 1terminal andN2HET1[12] device outputs. This special mode, Clock Test Mode,
isvery useful fordebugging purposes andcanbeconfigured through theCLKTEST register inthesystem
module. See Table 6-18 andTable 6-19 fortheCLKTEST bitsvalue andsignal selection.
Table 6-18. Clock Test Mode Options forSignals onECLK 1
SEL_ECP_PIN =CLKTEST[4-0] SIGNAL ONECLK 1
00000 Oscillator Clock
00001 PLL1 Clock Output
00010 Reserved
00011 EXTCLKIN1
00100 Low-Frequency Low-Power Oscillator (LFLPO) Clock [CLK80K]
00101 High-Frequency Low-Power Oscillator (HFLPO) Clock [CLK10M]
00110 PLL2 Clock Output
00111 EXTCLKIN2
01000 GCLK1
01001 RTI1 Base
01010 Reserved
01011 VCLKA1
01100 VCLKA2
01101 Reserved
01110 VCLKA4_DIVR
01111 Flash HDPump Oscillator
10000 Reserved
10001 HCLK
10010 VCLK
10011 VCLK2
10100 VCLK3
10101 Reserved
10110 Reserved
10111 EMAC Clock Output
11000 Reserved
11001 Reserved
11010 Reserved
11011 Reserved
11100 Reserved
11101 Reserved
11110 Reserved
11111 Reserved

<!-- Page 86 -->
86TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-19. Clock Test Mode Options forSignals onN2HET1[12]
SEL_GIO_PIN =CLKTEST[11-8] SIGNAL ONN2HET1[12]
0000 Oscillator Valid Status
0001 PLL1 Valid Status
0010 Reserved
0011 Reserved
0100 Reserved
0101 HFLPO Clock Output Valid Status [CLK10M]
0110 PLL2 Valid Status
0111 Reserved
1000 LFLPO Clock Output Valid Status [CLK80K]
1001 Oscillator Valid status
1010 Oscillator Valid status
1011 Oscillator Valid status
1100 Oscillator Valid status
1101 Reserved
1110 VCLKA4
1111 Oscillator Valid status

<!-- Page 87 -->
f[MHz]1.375 4.875 22 78faillower
thresholdpassupper
thresholdfail
87TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.7 Clock Monitoring
The LPO Clock Detect (LPOCLKDET) module consists ofaclock monitor (CLKDET) andaninternal LPO.
The LPO provides two different clock sources -alow frequency (CLK80K) and ahigh frequency
(CLK10M).
The CLKDET isasupervisor circuit foranexternally supplied clock signal (OSCIN). Incase theOSCIN
frequency falls outofafrequency window, theCLKDET flags thiscondition intheglobal status register
(GLBSTAT bit0:OSC FAIL) andswitches allclock domains sourced byOSCIN totheCLK10M clock (limp
mode clock).
The valid OSCIN frequency range isdefined as:fCLK10M /4<fOSCIN <fCLK10M *4.
6.7.1 Clock Monitor Timings
Figure 6-8.LPO andClock Detection, Untrimmed CLK10M
6.7.2 External Clock (ECLK) Output Functionality
The ECLK 1/ECLK2 terminal canbeconfigured tooutput aprescaled clock signal indicative ofaninternal
device clock. This output canbeexternally monitored asasafety diagnostic.
6.7.3 Dual Clock Comparators
The Dual Clock Comparator (DCC) module determines theaccuracy ofselectable clock sources by
counting thepulses oftwoindependent clock sources (counter 0and counter 1).Ifone clock isoutof
spec, anerror signal isgenerated. Forexample, theDCC1 can beconfigured touse CLK10M asthe
reference clock (forcounter 0)and VCLK asthe"clock under test" (forcounter 1).This configuration
allows theDCC1 tomonitor thePLL output clock when VCLK isusing thePLL output asitssource.
Anadditional useofthismodule istomeasure thefrequency ofaselectable clock source. Forexample,
thereference clock isconnected toCounter 0and thesignal tobemeasured isconnected toCounter 1.
Counter 0isprogrammed with astart value ofknown time duration (measurement time) from the
reference clock. Counter 1isprogrammed with amaximum start value. Start both counter simultaneously.
When Counter 0decrements tozero, both counter willstop and anerror signal isgenerated ifCounter 1
does notreach zero. The frequency oftheinput signals canbecalculated from thecount value ofCounter
1andthemeasurement time.
6.7.3.1 Features
*Takes twodifferent clock sources asinput totwoindependent counter blocks.
*One oftheclock sources istheknown-good, orreference clock; thesecond clock source isthe"clock under test."
*Each counter block isprogrammable with initial, orseed values.
*The counter blocks start counting down from their seed values atthesame time; amismatch from theexpected
frequency fortheclock under testgenerates anerror signal which isused tointerrupt theCPU.

<!-- Page 88 -->
88TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.7.3.2 Mapping ofDCC Clock Source Inputs
Table 6-20. DCC1 Counter 0Clock Sources
CLOCK SOURCE[3:0] CLOCK NAME
Others Oscillator (OSCIN)
0x5 High-frequency LPO
0xA Test clock (TCK)
Table 6-21. DCC1 Counter 1Clock Sources
KEY[3:0] CLOCK SOURCE[3:0] CLOCK NAME
Others - N2HET1[31]
0x0Main PLL free-running clock
output
0x1 PLL #2free-running clock output
0x2 Low-frequency LPO
0xA 0x3 High-frequency LPO
0x4 Reserved
0x5 EXTCLKIN1
0x6 EXTCLKIN2
0x7 Reserved
0x8-0xF VCLK
Table 6-22. DCC2 Counter 0Clock Sources
CLOCK SOURCE[3:0] CLOCK NAME
Others Oscillator (OSCIN)
0xA Test clock (TCK)
Table 6-23. DCC2 Counter 1Clock Sources
KEY[3:0] CLOCK SOURCE[3:0] CLOCK NAME
Others - N2HET2[0]
0xA0x1 PLL2_post_ODCLK/8
0x2 PLL2_post_ODCLK/16
0x3-0x7 Reserved
0x8-0xF VCLK