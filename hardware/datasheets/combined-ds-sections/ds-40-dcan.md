# DCAN Controller Area Network

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 187-187 (1 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 187 -->
187TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) These values donotinclude rise/fall times oftheoutput buffer.7.8 Controller Area Network (DCAN)
The DCAN supports theCAN 2.0B protocol standard and uses aserial, multimaster communication
protocol that efficiently supports distributed real-time control with robust communication rates ofupto1
megabit per second (Mbps). The DCAN isideal forapplications operating innoisy and harsh
environments (e.g., automotive and industrial fields) that require reliable serial communication or
multiplexed wiring.
7.8.1 Features
Features oftheDCAN module include:
*Supports CAN protocol version 2.0part A,B
*Bitrates upto1MBit/s
*The CAN kernel canbeclocked bytheoscillator forbaud-rate generation.
*64mailboxes oneach DCAN
*Individual identifier mask foreach message object
*Programmable FIFO mode formessage objects
*Programmable loop-back modes forself-test operation
*Automatic busonafter Bus-Off state byaprogrammable 32-bit timer
*Message RAM protected byECC
*Direct access toMessage RAM during testmode
*CAN Rx/Txpins configurable asgeneral purpose IOpins
*Message RAM Auto Initialization
*DMA support
Formore information ontheDCAN seethedevice specific technical reference manual.
7.8.2 Electrical andTiming Specifications
Table 7-29. Dynamic Characteristics fortheDCANx TXandRXpins
Parameter MIN MAX Unit
td(CANnTX) Delay time, transmit shift register toCANnTX pin(1)15 ns
td(CANnRX) Delay time, CANnRX pintoreceive shift register 5 ns