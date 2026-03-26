# Inter-Integrated Circuit (I2C)

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 190-192 (3 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 190 -->
190TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.11 Inter-Integrated Circuit (I2C)
The inter-integrated circuit (I2C) module isamultimaster communication module providing aninterface
between the TMS570 microcontroller and devices compliant with Philips Semiconductor I2C-bus
specification version 2.1and connected byanI2C-bus. This module willsupport anyslave ormaster I2C
compatible device.
7.11.1 Features
The I2Chasthefollowing features:
*Compliance tothePhilips I2Cbusspecification, v2.1 (The I2CSpecification, Philips document number
9398 39340011)
-Bit/Byte format transfer
-7-bit and10-bit device addressing modes
-General call
-START byte
-Multimaster transmitter/ slave receiver mode
-Multimaster receiver/ slave transmitter mode
-Combined master transmit/receive andreceive/transmit mode
-Transfer rates of10kbps upto400kbps (Phillips fast-mode rate)
*Free data format
*Two DMA events (transmit andreceive)
*DMA event enable/disable capability
*Seven interrupts thatcanbeused bytheCPU
*Module enable/disable capability
*The SDA andSCL areoptionally configurable asgeneral purpose I/O
*Slew rate control oftheoutputs
*Open drain control oftheoutputs
*Programmable pullup/pulldown capability ontheinputs
*Supports Ignore NACK mode
NOTE
This I2Cmodule does notsupport:
*High-speed (HS) mode
*C-bus compatibility mode
*The combined format in10-bit address mode (the I2Csends theslave address second
byte every time itsends theslave address firstbyte)

<!-- Page 191 -->
SDA
SCLtw(SDAH)
tw(SCLL)
tw(SCLH)tw(SP)
th(SCLL-SDAL)th(SDA-SCLL)th(SCLL-SDAL)
tsu(SCLH-SDAL)tf(SCL)tc(SCL)tr(SCL)tsu(SCLH-SDAH)
Stop Start Repeated Start Stoptsu(SDA-SCLH)
191TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) The I2Cpins SDA andSCL donotfeature fail-safe I/Obuffers. These pins could potentially draw current when thedevice ispowered
down.
(2) Themaximum th(SDA-SCLL) forI2Cbusdevices hasonly tobemetifthedevice does notstretch thelowperiod (tw(SCLL) )oftheSCL
signal.
(3) Cb=The total capacitance ofonebuslineinpF.7.11.2 I2CI/OTiming Specifications
Table 7-30. I2CSignals (SDA andSCL) Switching Characteristics(1)
PARAMETERSTANDARD MODE FAST MODE
UNIT
MIN MAX MIN MAX
tc(I2CCLK)Cycle time, Internal Module clock forI2C,
prescaled from VCLK75.2 149 75.2 149 ns
f(SCL) SCL Clock frequency 0 100 0 400 kHz
tc(SCL) Cycle time, SCL 10 2.5 µs
tsu(SCLH-SDAL)Setup time, SCL high before SDA low(fora
repeated START condition)4.7 0.6 µs
th(SCLL-SDAL)Hold time, SCL lowafter SDA low(forarepeated
START condition)4 0.6 µs
tw(SCLL) Pulse duration, SCL low 4.7 1.3 µs
tw(SCLH) Pulse duration, SCL high 4 0.6 µs
tsu(SDA-SCLH) Setup time, SDA valid before SCL high 250 100 ns
th(SDA-SCLL)Hold time, SDA valid after SCL low(forI2Cbus
devices)0 3.45(2)0 0.9 µs
tw(SDAH)Pulse duration, SDA high between STOP and
START conditions4.7 1.3 µs
tsu(SCLH-SDAH)Setup time, SCL high before SDA high (forSTOP
condition)4.0 0.6 µs
tw(SP) Pulse duration, spike (must besuppressed) 0 50 ns
Cb(3)Capacitive load foreach busline 400 400 pF
Figure 7-18. I2CTimings

<!-- Page 192 -->
192TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
*Adevice must internally provide ahold time ofatleast 300 nsfortheSDA signal
(referred totheVIHmin oftheSCL signal) tobridge theundefined region ofthefalling
edge ofSCL.
*The maximum th(SDA-SCLL) has only tobemet ifthedevice does notstretch theLOW
period (tw(SCLL) )oftheSCL signal.
*AFast-mode I2C-bus device canbeused inaStandard-mode I2C-bus system, butthe
requirement tsu(SDA-SCLH) ≥250nsmust then bemet. This willautomatically bethecase if
thedevice does notstretch theLOW period oftheSCL signal. Ifsuch adevice does
stretch theLOW period oftheSCL signal, itmust output thenext data bittotheSDA line
trmax +tsu(SDA-SCLH) .
*Cb=total capacitance ofone buslineinpF.Ifmixed with fast-mode devices, faster fall-
times areallowed.