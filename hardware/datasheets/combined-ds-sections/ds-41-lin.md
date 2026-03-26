# LIN Interface

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 188-188 (1 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 188 -->
188TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.9 Local Interconnect Network Interface (LIN)
The SCI/LIN module canbeprogrammed towork either asanSCIorasaLIN. The core ofthemodule is
anSCI. The SCI'shardware features areaugmented toachieve LINcompatibility.
The SCI module isauniversal asynchronous receiver-transmitter that implements thestandard nonreturn
tozero format. The SCI canbeused tocommunicate, forexample, through anRS-232 port orover aK-
line.
The LIN standard isbased ontheSCI (UART) serial data linkformat. The communication concept is
single-master/multiple-slave with amessage identification formulticast transmission between anynetwork
nodes.
7.9.1 LINFeatures
The following arefeatures oftheLINmodule:
*Compatible toLIN1.3,2.0and2.1protocols
*Multibuffered receive andtransmit units DMA capability forminimal CPU intervention
*Identification masks formessage filtering
*Automatic Master Header Generation
-Programmable Synch Break Field
-Synch Field
-Identifier Field
*Slave Automatic Synchronization
-Synch break detection
-Optional baudrate update
-Synchronization Validation
*231programmable transmission rates with 7fractional bits
*Error detection
*2Interrupt lines with priority encoding