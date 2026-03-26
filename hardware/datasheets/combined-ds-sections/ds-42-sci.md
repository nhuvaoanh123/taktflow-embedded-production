# SCI Serial Communication Interface

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 189-189 (1 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 189 -->
189TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.10 Serial Communication Interface (SCI)
7.10.1 Features
*Standard universal asynchronous receiver-transmitter (UART) communication
*Supports full-orhalf-duplex operation
*Standard nonreturn tozero (NRZ) format
*Double-buffered receive andtransmit functions
*Configurable frame format of3to13bitspercharacter based onthefollowing:
-Data word length programmable from onetoeight bits
-Additional address bitinaddress-bit mode
-Parity programmable forzero oroneparity bit,oddoreven parity
-Stop programmable foroneortwostop bits
*Asynchronous orisosynchronous communication modes
*Two multiprocessor communication formats allow communication between more than twodevices.
*Sleep mode isavailable tofree CPU resources during multiprocessor communication.
*The 24-bit programmable baud rate supports 224different baud rates provide high accuracy baud rate selection.
*Four error flags andFive status flags provide detailed information regarding SCIevents.
*Capability touseDMA fortransmit andreceive data.