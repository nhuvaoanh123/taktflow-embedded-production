# General-Purpose I/O (GIO)

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 179-179 (1 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 179 -->
179TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.5 General-Purpose Input/Output
The GPIO module onthisdevice supports twoports, GIOA and GIOB. The I/Opins arebidirectional and
bit-programmable. Both GIOA andGIOB support external interrupt capability.
7.5.1 Features
The GPIO module hasthefollowing features:
*Each I/Opincanbeconfigured as:
-Input
-Output
-Open Drain
*The interrupts have thefollowing characteristics:
-Programmable interrupt detection either onboth edges oronasingle edge (setinGIOINTDET)
-Programmable edge-detection polarity, either rising orfalling edge (setinGIOPOL register)
-Individual interrupt flags (setinGIOFLG register)
-Individual interrupt enables, setand cleared through GIOENASET and GIOENACLR registers
respectively
-Programmable interrupt priority, setthrough GIOLVLSET andGIOLVLCLR registers
*Internal pullup/pulldown allows unused I/Opins tobeleftunconnected
Forinformation oninput andoutput timings seeSection 5.10.1 andSection 5.10.2 .