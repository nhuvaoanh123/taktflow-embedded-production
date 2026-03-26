# Real-Time Interrupt Module

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 127-128 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 127 -->
31  031  0
RTICLK31  0
31  0
31  0
External
controlCAP event source 0
CAP event source 1
= Up counter
Capture
up counterCompare
up counter
Free-running counter
CaptureRTIFRCx
free-running counter
RTICAFRCxOVLINTx RTICPUCx
RTIUCx
RTICAUCxTo Compare
UnitNTU0
NTU1
NTU2
NTU3
127TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.18 Real-Time Interrupt Module
The real-time interrupt (RTI) module provides timer functionality foroperating systems and for
benchmarking code. The RTImodule canincorporate several counters thatdefine thetime bases needed
forscheduling anoperating system.
The timers also letyou benchmark certain areas ofcode byreading thevalues ofthecounters atthe
beginning andtheendofthedesired code range andcalculating thedifference between thevalues.
6.18.1 Features
The RTImodule hasthefollowing features:
*Two independent 64-bit counter blocks
*Four configurable compares forgenerating operating system ticks orDMA requests. Each event can
bedriven byeither counter block 0orcounter block 1.
*Fast enabling/disabling ofevents
*Two timestamp (capture) functions forsystem orperipheral interrupts, oneforeach counter block
6.18.2 Block Diagrams
Figure 6-20 shows ahigh-level block diagram forone ofthetwo 64-bit counter blocks inside theRTI
module. Both thecounter blocks areidentical except theNetwork Time Unit (NTUx) inputs areonly
available astime-base inputs forthecounter block 0.Figure 6-21 shows thecompare unitblock diagram
oftheRTImodule.
Figure 6-20. Counter Block Diagram

<!-- Page 128 -->
31  0
Compare
controlINTyDMAREQy CompareUpdate
compare
From counter
block 0
From counter
block 1RTIUDCPy
RTICOMPy31  0=+
128TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 6-21. Compare Block Diagram
6.18.3 Clock Source Options
The RTImodule uses theRTI1CLK clock domain forgenerating theRTItime bases.
The application canselect theclock source fortheRTI1CLK byconfiguring theRCLKSRC register inthe
system module ataddress 0xFFFFFF50. The default source forRTI1CLK isVCLK.
Formore information onclock sources, seeTable 6-11 andTable 6-16.
6.18.4 Network Time Synchronization Inputs
The RTI module supports four Network Time Unit (NTU) inputs that signal internal system events, and
which can beused tosynchronize thetime base used bytheRTI module. Onthisdevice, these NTU
inputs areconnected asshown inTable 6-43.
Table 6-43. Network Time Synchronization Inputs
NTU INPUT SOURCE
0 FlexRay Macrotick
1 FlexRay Start ofCycle
2 PLL2 Clock output
3 EXTCLKIN1 clock input