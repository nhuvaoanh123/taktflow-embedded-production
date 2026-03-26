# System Info - Power Domains

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 64-68 (5 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 64 -->
64TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6System Information andElectrical Specifications
6.1 Device Power Domains
The device core logic issplit upinto multiple virtual power domains tooptimize thepower foragiven
application usecase.
This device hassixlogic power domains: PD1, PD2, PD3, PD4, PD5, and PD6. PD1 isadomain which
cannot turn offofitsclocks atonce through thePower-Management Module (PMM). However, individual
clock domain operating inPD1 canbeindividually enabled ordisabled through theSYS.CDDIS register.
Each oftheother power domains can beturned ON, IDLE orOFF aspertheapplication requirement
through thePMM module.
Inthisdevice, apower domain canoperate inoneofthethree possible power states: ON, IDLE andOFF.
ONstate isthenormal operating state where clocks areactively running inthepower domain. When
clocks areturned off,thedynamic current isremoved from thepower domain. Inthisdevice, both the
IDLE andOFF states have thesame power characteristic. When putintoeither theIDLE ortheOFF state,
only clocks areturned offfrom thepower domain. Leakage current from thepower domain stillremains.
Note that putting apower domain intheOFF state willnotremove anyleakage current inthisdevice. In
changing thepower domain states, theuser must pollthesystem status register tocheck thecompletion
ofthetransition. From aprogrammer model perspective, allthree power states areavailable from the
PMM module.
The actual management ofthepower domains and thehand-shaking mechanism ismanaged bythe
PMM. Refer tothePower Management Module (PMM) chapter ofthedevice technical reference manual
formore details.

<!-- Page 65 -->
65TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.2 Voltage Monitor Characteristics
Avoltage monitor isimplemented onthisdevice. The purpose ofthisvoltage monitor istoeliminate the
requirement foraspecific sequence when powering upthecore andI/Ovoltage supplies.
6.2.1 Important Considerations
*The voltage monitor does noteliminate theneed ofavoltage supervisor circuit toensure thatthedevice isheld in
reset when thevoltage supplies areoutofrange.
*The voltage monitor only monitors thecore supply (VCC) andtheI/Osupply (VCCIO). The other supplies arenot
monitored bytheVMON. Forexample, iftheVCCAD orVCCP aresupplied from asource different from that for
VCCIO, then there isnointernal voltage monitor fortheVCCAD andVCCP supplies.
6.2.2 Voltage Monitor Operation
The voltage monitor generates thePower Good MCU signal (PGMCU) aswell astheI/Os Power Good IO
signal (PGIO) onthedevice. During power-up orpower-down, thePGMCU andPGIO aredriven lowwhen
thecore orI/Osupplies arelower than thespecified minimum monitoring thresholds. The PGIO and
PGMCU being lowisolates thecore logic aswell astheI/Ocontrols during power uporpower down ofthe
supplies. This allows thecore andI/Osupplies tobepowered upordown inanyorder.
When thevoltage monitor detects alowvoltage ontheI/Osupply, itwillassert apower-on reset. When
thevoltage monitor detects anout-of-range voltage onthecore supply, itasynchronously makes alloutput
pins high impedance, and asserts apower-on reset. The I/Osupply must beabove thethreshold for
monitoring thecore supply. The voltage monitor isdisabled when thedevice enters alowpower mode.
The VMON also incorporates aglitch filter forthenPORRST input. Refer toSection 6.3.3.1 forthetiming
information onthisglitch filter.
Table 6-1.Voltage Monitoring Specifications
PARAMETER MIN TYP MAX UNIT
VMONVoltage
monitoring
thresholdsVCC low-VCC level below thisthreshold isdetected astoolow. 0.75 0.9 1.13
V VCC high -VCC level above thisthreshold isdetected astoohigh. 1.40 1.7 2.1
VCCIO low-VCCIO level below thisthreshold isdetected astoolow. 1.85 2.4 2.99
6.2.3 Supply Filtering
The VMON hasthecapability tofilter glitches ontheVCC andVCCIO supplies.
Table 6-2lists thecharacteristics ofthesupply filtering. Glitches inthesupply larger than themaximum
specification cannot befiltered.
Table 6-2.VMON Supply Glitch Filtering Capability
PARAMETER MIN MAX UNIT
Width ofglitch onVCC thatcanbefiltered 250 1000 ns
Width ofglitch onVCCIO thatcanbefiltered 250 1000 ns

<!-- Page 66 -->
66TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.3 Power Sequencing andPower-On Reset
6.3.1 Power-Up Sequence
There isnotiming dependency between theramp oftheVCCIO andtheVCC supply voltage. The power-
upsequence starts with theI/Ovoltage rising above theminimum I/Osupply threshold, (formore details,
seeTable 6-3),core voltage rising above theminimum core supply threshold andtherelease ofpower-on
reset. The high-frequency oscillator willstart upfirstanditsamplitude willgrow toanacceptable level. The
oscillator start-up time isdependent onthetype ofoscillator andisprovided bytheoscillator vendor. The
different supplies tothedevice canbepowered upinanyorder.
The device goes through thefollowing sequential phases during power up.
Table 6-3.Power-Up Phases
Oscillator start-up andvalidity check 1024 oscillator cycles
eFuse autoload 3650 oscillator cycles
Flash pump power-up 250oscillator cycles
Flash bank power-up 1460 oscillator cycles
Total 6384 oscillator cycles
The CPU reset isreleased attheendoftheabove sequence andfetches thefirstinstruction from address
0x00000000.

<!-- Page 67 -->
3.3 V
1.2 V
VCC(1.2 V)
VCCIO / VCCP(3.3 V)8
6
37
6
9VCCPORH
VCCIOPORL
VIL(PORRST)VCCIOPORH
7VCCIOPORH
VCCIOPORL
VCCPORLVCC
nPORRSTVCCIO / VCCP
VCCPORH
VCCPORL
VIL VILVIL VIL VIL(PORRST)
67TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.3.2 Power-Down Sequence
The different supplies tothedevice canbepowered down inanyorder.
6.3.3 Power-On Reset: nPORRST
This isthepower-on reset. This reset must beasserted byanexternal circuitry whenever theI/Oorcore
supplies areoutside thespecified recommended range. This signal hasaglitch filter onit.Italso hasan
internal pulldown.
6.3.3.1 nPORRST Electrical andTiming Requirements
Table 6-4.Electrical Requirements fornPORRST
NO. MIN MAX UNIT
VCCPORL VCClowsupply level when nPORRST must beactive during power up 0.5 V
VCCPORHVCChigh supply level when nPORRST must remain active during
power upandbecome active during power down1.14 V
VCCIOPORLVCCIO /VCCPlowsupply level when nPORRST must beactive during
power up1.1 V
VCCIOPORHVCCIO /VCCPhigh supply level when nPORRST must remain active
during power upandbecome active during power down3.0 V
VIL(PORRST)Low-level input voltage ofnPORRST VCCIO >2.5V 0.2*VCCIOV
Low-level input voltage ofnPORRST VCCIO <2.5V 0.5
3 tsu(PORRST)Setup time, nPORRST active before VCCIO andVCCP>VCCIOPORL
during power up0 ms
6 th(PORRST) Hold time, nPORRST active after VCC>VCCPORH 1 ms
7 tsu(PORRST)Setup time, nPORRST active before VCC<VCCPORH during power
down2 µs
8 th(PORRST) Hold time, nPORRST active after VCCIO andVCCP>VCCIOPORH 1 ms
9 th(PORRST) Hold time, nPORRST active after VCC<VCCPORL 0 ms
tf(nPORRST)Filter time nPORRST terminal; pulses less than MIN willbefiltered out,
pulses greater than MAX willgenerate areset. Pulses greater than
MIN butless than MAX may ormay notgenerate areset.475 2000 ns
A. Figure 6-1shows thatthere isnotiming dependency between theramp oftheVCCIO andtheVCCsupply voltages.
Figure 6-1.nPORRST Timing Diagram(A)

<!-- Page 68 -->
68TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.4 Warm Reset (nRST)
This isabidirectional reset signal. The internal circuitry drives thesignal lowondetecting anydevice reset
condition. Anexternal circuit can assert adevice reset byforcing thesignal low. Onthisterminal, the
output buffer isimplemented asanopen drain (drives low only). Toensure anexternal reset isnot
arbitrarily generated, TIrecommends thatanexternal pullup resistor isconnected tothisterminal.
This terminal hasaglitch filter. Italso hasaninternal pullup
6.4.1 Causes ofWarm Reset
Table 6-5.Causes ofWarm Reset
DEVICE EVENT SYSTEM STATUS FLAG
Power-Up Reset Exception Status Register, bit15
Oscillator fail Global Status Register, bit0
PLL slip Global Status Register, bits8and9
Watchdog exception Exception Status Register, bit13
Debugger reset Exception Status Register, bit11
CPU Reset (driven bytheCPU STC) Exception Status Register, bit5
Software Reset Exception Status Register, bit4
External Reset Exception Status Register, bit3
(1) Specified values donotinclude rise/fall times. Forriseandfalltimings, seeTable 5-5.6.4.2 nRST Timing Requirements
Table 6-6.nRST Timing Requirements(1)
MIN MAX UNIT
tv(RST)Valid time, nRST active after nPORRST inactive 5032t c(OSC)ns
Valid time, nRST active (allother System reset conditions) 32tc(VCLK)
tf(nRST)Filter time nRST terminal; pulses less than MIN willbefiltered
out,pulses greater than MAX willgenerate areset475 2000 ns