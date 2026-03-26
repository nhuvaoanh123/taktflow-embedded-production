# Terminal Functions - FlexRay, DCAN

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 26-27 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 26 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435726TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.9 FlexRay Interface Controller (FlexRay)
Table 4-9.FlexRay Interface Controller (FlexRay)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
FRAYRX1 A15 Input Pullup Fixed, 100µA - FlexRay data receive (channel 1)
FRAYRX2 A8 Input Pullup Fixed, 100µA - FlexRay data receive (channel 2)
FRAYTX1 /GIOA[2] B15 Output Pulldown 20µA 8mA FlexRay data transmit (channel 1)
FRAYTX2 /GIOB[0] B8 Output Pulldown 20µA 8mA FlexRay data transmit (channel 2)
FRAYTXEN1 /GIOB[1] B16 Output Pulldown 20µA 8mA FlexRay transmit enable (channel 1)
FRAYTXEN2 /GIOB[2] B9 Output Pulldown 20µA 8mA FlexRay transmit enable (channel 2)

<!-- Page 27 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435727TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.10 Controller Area Network Controllers (DCAN)
Table 4-10. ZWT Controller Area Network Controllers (DCAN)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
DCAN1RX B10 I/O Pullup Programmable,
20µA2mA ZD CAN1 receive, orGIO
DCAN1TX A10 I/O Pullup Programmable,
20µA2mA ZD CAN1 transmit, orGIO
DCAN2RX H1 I/O Pullup Programmable,
20µA2mA ZD CAN2 receive, orGIO
DCAN2TX H2 I/O Pullup Programmable,
20µA2mA ZD CAN2 transmit, orGIO
DCAN3RX M19 I/O Pullup Programmable,
20µA2mA ZD CAN3 receive, orGIO
DCAN3TX M18 I/O Pullup Programmable,
20µA2mA ZD CAN3 transmit, orGIO
GIOB[3]/ DCAN4RX W10 I/O Pulldown Programmable,
20µA2mA ZD CAN4 receive, orGIO
GIOB[2]/ DCAN4TX F2 I/O Pulldown Programmable,
20µA2mA ZD CAN4 transmit, orGIO