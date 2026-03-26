# N2HET High-End Timer

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 180-184 (5 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 180 -->
N2HETx
3
4
21
180TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.6 Enhanced High-End Timer (N2HET)
The N2HET isanadvanced intelligent timer that provides sophisticated timing functions forreal-time
applications. The timer issoftware-controlled, using areduced instruction set, with aspecialized timer
micromachine and anattached I/Oport. The N2HET can beused forpulse width modulated outputs,
capture orcompare inputs, orgeneral-purpose I/O.. Itisespecially well suited forapplications requiring
multiple sensor information anddrive actuators with complex andaccurate time pulses.
7.6.1 Features
The N2HET module hasthefollowing features:
*Programmable timer forinput andoutput timing functions
*Reduced instruction set(30instructions) fordedicated time andangle functions
*256words ofinstruction RAM protected byparity
*User defined number of25-bit virtual counters fortimer, event counters andangle counters
*7-bit hardware counters foreach pinallow upto32-bit resolution inconjunction with the25-bit virtual
counters
*Upto32pins usable forinput signal measurements oroutput signal generation
*Programmable suppression filter foreach input pinwith adjustable limiting frequency
*Low CPU overhead andinterrupt load
*Efficient data transfer toorfrom theCPU memory with dedicated High-End-Timer Transfer Unit (HTU)
orDMA
*Diagnostic capabilities with different loopback mechanisms andpinstatus readback functionality
7.6.2 N2HET RAM Organization
The timer RAM uses 4RAM banks, where each bank hastwoport access capability. This means thatone
RAM address may bewritten while another address isread. The RAM words are96-bits wide, which are
split intothree 32-bit fields (program, control, anddata).
7.6.3 Input Timing Specifications
The N2HET instructions PCNT andWCAP impose some timing constraints ontheinput signals.
Figure 7-14. N2HET Input Capture Timings

<!-- Page 181 -->
NHET_LOOP_SYNC EXT_LOOP_SYNCEXT_LOOP_SYNC NHET_LOOP_SYNCN2HET1 N2HET2
181TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 7-24. Dynamic Characteristics fortheN2HET Input Capture Functionality
PARAMETER MIN MAX UNIT
1Input signal period, PCNT orWCAP forrising edge
torising edge(HRP) (LRP) tc(VCLK2) +2 225(HRP) (LRP) tc(VCLK2) -2 ns
2Input signal period, PCNT orWCAP forfalling edge
tofalling edge(HRP) (LRP) tc(VCLK2) +2 225(HRP) (LRP) tc(VCLK2) -2 ns
3Input signal high phase, PCNT orWCAP forrising
edge tofalling edge2(HRP) tc(VCLK2) +2 225(HRP) (LRP) tc(VCLK2) -2 ns
4Input signal lowphase, PCNT orWCAP forfalling
edge torising edge2(HRP) tc(VCLK2) +2 225(HRP) (LRP) tc(VCLK2) -2 ns
7.6.4 N2HET1-N2HET2 Interconnections
Insome applications theN2HET resolutions must besynchronized. Some other applications require a
single time base tobeused forallPWM outputs andinput timing captures.
The N2HET provides such asynchronization mechanism. The Clk_master/slave (HETGCR.16) configures
theN2HET inmaster orslave mode (default isslave mode). AN2HET inmaster mode provides asignal
tosynchronize theprescalers oftheslave N2HET. The slave N2HET synchronizes itsloop resolution to
theloop resolution signal sent bythemaster. The slave does notrequire thissignal after itreceives the
first synchronization signal. However, anytime theslave receives theresynchronization signal from the
master, theslave must synchronize itself again..
Figure 7-15. N2HET1 -N2HET2 Synchronization Hookup

<!-- Page 182 -->
N2HET1
N2HET2IOMM□mux□control□signal□x
N2HET1[1]□/□N2HET2[8]N2HET1[1]
N2HET2[8]
N2HET1[3]□/□N2HET2[10]
N2HET1[5]□/□N2HET2[12]
N2HET1[7]□/□N2HET2[14]
N2HET1[9]□/□N2HET2[16]
N2HET1
N2HET2IOMM□mux□control□signal□x
N2HET1[11]□/□N2HET2[18]N2HET1[11]
N2HET2[18]
182TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.6.5 N2HET Checking
7.6.5.1 Internal Monitoring
Toassure correctness ofthehigh-end timer operation andoutput signals, thetwoN2HET modules canbe
used tomonitor each other 'ssignals asshown inFigure 7-16.The direction ofthemonitoring iscontrolled
bytheI/Omultiplexing control module.
Figure 7-16. N2HET Monitoring
7.6.5.2 Output Monitoring using Dual Clock Comparator (DCC)
N2HET1[31] isconnected asaclock source forcounter 1inDCC1. This allows theapplication tomeasure
thefrequency ofthepulse-width modulated (PWM) signal onN2HET1[31].
Similarly, N2HET2[0] isconnected asaclock source forcounter 1inDCC2. This allows theapplication to
measure thefrequency ofthepulse-width modulated (PWM) signal onN2HET2[0].
Both N2HET1[31] and N2HET2[0] canbeconfigured tobeinternal-only channels. That is,theconnection
totheDCC module ismade directly from theoutput oftheN2HETx module (from theinput oftheoutput
buffer).

<!-- Page 183 -->
183TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFormore information onDCC seeSection 6.7.3 .
7.6.6 Disabling N2HET Outputs
Some applications require theN2HET outputs tobedisabled under some fault condition. The N2HET
module provides this capability through the"Pin Disable" input signal. This signal, when driven low,
causes theN2HET outputs identified byaprogrammable register (HETPINDIS) tobetri-stated. Refer to
theIOMM chapter inthedevice specific technical reference manual formore details onthe"N2HET Pin
Disable" feature.
GIOA[5] isconnected tothe"Pin Disable" input forN2HET1, and GIOB[2] isconnected tothe"Pin
Disable" input forN2HET2.

<!-- Page 184 -->
184TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.6.7 High-End Timer Transfer Unit (HET-TU)
AHigh End Timer Transfer Unit (HET-TU) canperform DMA type transactions totransfer N2HET data to
orfrom main memory. AMemory Protection Unit (MPU) isbuilt intotheHET-TU.
7.6.7.1 Features
*CPU andDMA independent
*Master Port toaccess system memory
*8control packets supporting dual buffer configuration
*Control packet information isstored inRAM protected byparity
*Event synchronization (HET transfer requests)
*Supports 32or64bittransactions
*Addressing modes forHET address (8byte or16byte) andsystem memory address (fixed, 32bitor64bit)
*One shot, circular andauto switch buffer transfer modes
*Request lostdetection
7.6.7.2 Trigger Connections
Table 7-25. HET TU1 Request Line Connection
Modules Request Source HET TU1 Request
N2HET1 HTUREQ[0] HET TU1 DCP[0]
N2HET1 HTUREQ[1] HET TU1 DCP[1]
N2HET1 HTUREQ[2] HET TU1 DCP[2]
N2HET1 HTUREQ[3] HET TU1 DCP[3]
N2HET1 HTUREQ[4] HET TU1 DCP[4]
N2HET1 HTUREQ[5] HET TU1 DCP[5]
N2HET1 HTUREQ[6] HET TU1 DCP[6]
N2HET1 HTUREQ[7] HET TU1 DCP[7]
Table 7-26. HET TU2 Request Line Connection
Modules Request Source HET TU2 Request
N2HET2 HTUREQ[0] HET TU2 DCP[0]
N2HET2 HTUREQ[1] HET TU2 DCP[1]
N2HET2 HTUREQ[2] HET TU2 DCP[2]
N2HET2 HTUREQ[3] HET TU2 DCP[3]
N2HET2 HTUREQ[4] HET TU2 DCP[4]
N2HET2 HTUREQ[5] HET TU2 DCP[5]
N2HET2 HTUREQ[6] HET TU2 DCP[6]
N2HET2 HTUREQ[7] HET TU2 DCP[7]