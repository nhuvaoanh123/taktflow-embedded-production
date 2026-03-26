# Terminal Functions - Debug, System, Clocks

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 39-46 (8 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 39 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435739TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.17 Embedded Trace Macrocell Interface forCortex-R5F (ETM-R5)
Table 4-19. ZWT Embedded Trace Macrocell Interface forCortex-R5F (ETM-R5)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
ETMDATA[0] R12 Output Pulldown 20µA 8mA ETM data
ETMDATA[1] R13 Output Pulldown 20µA 8mA ETM data
ETMDATA[2] J15 Output Pulldown 20µA 8mA ETM data
ETMDATA[3] H15 Output Pulldown 20µA 8mA ETM data
ETMDATA[4] G15 Output Pulldown 20µA 8mA ETM data
ETMDATA[5] F15 Output Pulldown 20µA 8mA ETM data
ETMDATA[6] E15 Output Pulldown 20µA 8mA ETM data
ETMDATA[7] E14 Output Pulldown 20µA 8mA ETM data
ETMDATA[8] /EMIF_ADDR[5] E9 Output Pulldown 20µA 8mA ETM data
ETMDATA[9] /EMIF_ADDR[4] E8 Output Pulldown 20µA 8mA ETM data
ETMDATA[10] /EMIF_ADDR[3] E7 Output Pulldown 20µA 8mA ETM data
ETMDATA[11] /EMIF_ADDR[2] E6 Output Pulldown 20µA 8mA ETM data
ETMDATA[12] /EMIF_BA[0] E13 Output Pulldown 20µA 8mA ETM data
ETMDATA[13] /EMIF_nOE E12 Output Pulldown 20µA 8mA ETM data
ETMDATA[14] /EMIF_nDQM[1] E11 Output Pulldown 20µA 8mA ETM data
ETMDATA[15] /EMIF_nDQM[0] E10 Output Pulldown 20µA 8mA ETM data
ETMDATA[16] /EMIF_DATA[0] K15 Output Pulldown 20µA 8mA ETM data
ETMDATA[17] /EMIF_DATA[1] L15 Output Pulldown 20µA 8mA ETM data
ETMDATA[18] /EMIF_DATA[2] M15 Output Pulldown 20µA 8mA ETM data
ETMDATA[19] /EMIF_DATA[3] N15 Output Pulldown 20µA 8mA ETM data
ETMDATA[20] /EMIF_DATA[4] E5 Output Pulldown 20µA 8mA ETM data
ETMDATA[21] /EMIF_DATA[5] F5 Output Pulldown 20µA 8mA ETM data
ETMDATA[22] /EMIF_DATA[6] G5 Output Pulldown 20µA 8mA ETM data
ETMDATA[23] /EMIF_DATA[7] K5 Output Pulldown 20µA 8mA ETM data
ETMDATA[24] /EMIF_DATA[8]/N2HET2[24]/MIBSPI5NCS[4] L5 Output Pulldown 20µA 8mA ETM data
ETMDATA[25] /EMIF_DATA[9]/N2HET2[25]/MIBSPI5NCS[5] M5 Output Pulldown 20µA 8mA ETM data
ETMDATA[26] /EMIF_DATA[10]/N2HET2[26] N5 Output Pulldown 20µA 8mA ETM data
ETMDATA[27] /EMIF_DATA[11]/N2HET2[27] P5 Output Pulldown 20µA 8mA ETM data
ETMDATA[28] /EMIF_DATA[12]/N2HET2[28]/GIOA[0] R5 Output Pulldown 20µA 8mA ETM data

<!-- Page 40 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435740TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-19. ZWT Embedded Trace Macrocell Interface forCortex-R5F (ETM-R5) (continued)
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
ETMDATA[29] /EMIF_DATA[13]/N2HET2[29]/GIOA[1] R6 Output Pulldown 20µA 8mA ETM data
ETMDATA[30] /EMIF_DATA[14]/N2HET2[30]/GIOA[3] R7 Output Pulldown 20µA 8mA ETM data
ETMDATA[31] /EMIF_DATA[15]/N2HET2[31]/GIOA[4] R8 Output Pulldown 20µA 8mA ETM data
ETMTRACECLKIN /EXTCLKIN2/GIOA[5] R9 Input Pullup Fixed, 20µA - ETM trace clock input
ETMTRACECLKOUT /GIOA[6] R10 Output Pulldown 20µA 8mA ETM trace clock output
ETMTRACECTL /GIOA[7] R11 Output Pulldown 20µA 8mA ETM trace control

<!-- Page 41 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435741TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.18 System Module Interface
Table 4-20. ZWT System Module Interface
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
nERROR B14 Output Pulldown 20µA 8mA ESM error (And ofError 1and
Error 2)
GIOB[6]/ nERROR J2 Output Pulldown 20µA 8mA ESM error 1
nPORRST W7 Input Pulldown 100µA - Power-on reset, cold reset
nRST B17 I/O Pullup 100µA 4mA System reset, warm reset
4.2.1.19 Clock Inputs andOutputs
Table 4-21. ZWT Clock Inputs andOutputs
Terminal
Signal TypeDefault Pull
StatePullTypeOutput Buffer
Drive StrengthDescription
Signal Name337
ZWT
ECLK1 A12 I/O Pulldown Programmable, 20µA 2mA ZD/8mA External clock output, orGIO
EMIF_CLK/ ECLK2 K3 I/O Pulldown Programmable, 20µA 2mA ZD/8mA External clock output, orGIO
GIOA[5]/ EXTCLKIN1 /ePWM1A B5 Input Pulldown Fixed, 20µA - External clock input
ETMTRACECLKIN/ EXTCLKIN2 /GIOA[5] R9 Input Pullup Fixed, 20µA - External clock input #2
KELVIN_GND L2 Input - - - Kelvin ground foroscillator
OSCIN K1 Input - - - From external crystal/resonator, or
external clock input
OSCOUT L1 Output - - - Toexternal crystal/resonator

<!-- Page 42 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435742TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.20 Test andDebug Modules Interface
Table 4-22. ZWT Test andDebug Modules Interface
TERMINAL
SIGNAL
TYPEDEFAULT
PULL STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
nTRST D18 Input Pulldown 100µA - JTAG testhardware reset
TCK B18 Input Pulldown Fixed, 100µA - JTAG testclock
TDI A17 Input Pullup Fixed, 100µA - JTAG testdata in
TDO C18 Output Pulldown Fixed, 100µA 8mA JTAG testdata out
TEST U2 Input Pulldown Fixed, 100µA -Test mode enable. This terminal
must beconnected toground directly
orthrough apulldown resistor.
TMS C19 Input Pullup Fixed, 100µA - JTAG testmode select
RTCK A16 Output - - 8mA JTAG return testclock
4.2.1.21 Flash Supply andTest Pads
Table 4-23. ZWT Flash Supply andTest Pads
TERMINAL
SIGNAL
TYPEDEFAULT
PULL STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
VCCP F8 3.3-V Power - - - Flash pump supply
FLTP1 J5 Input - - - Flash testpads. These terminals are
reserved forTIuseonly. Forproper
operation these terminals must
connect only toatestpadornotbe
connected atall[noconnect (NC)].FLTP2 H5 Input - - -

<!-- Page 43 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435743TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.22 Supply forCore Logic: 1.2-V Nominal
Table 4-24. ZWT Supply forCore Logic: 1.2-V Nominal
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
VCC P10 1.2-V
Power- - - Core supply
VCC L6 - - - Core supply
VCC K6 - - - Core supply
VCC F9 - - - Core supply
VCC F10 - - - Core supply
VCC J14 - - - Core supply
VCC K14 - - - Core supply
VCC M10 - - - Core supply
VCC K8 - - - Core supply
VCC H10 - - - Core supply
VCC K12 - - - Core supply

<!-- Page 44 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435744TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.23 Supply forI/OCells: 3.3-V Nominal
Table 4-25. ZWT Supply forI/OCells: 3.3-V Nominal
Terminal
Signal
TypeDefault Pull
StatePullTypeOutput
Buffer
Drive
StrengthDescription
Signal Name337
ZWT
VCCIO F11 3.3-V
Power- - - Operating supply forI/Os
VCCIO F12 - - - Operating supply forI/Os
VCCIO F13 - - - Operating supply forI/Os
VCCIO F14 - - - Operating supply forI/Os
VCCIO G14 - - - Operating supply forI/Os
VCCIO H14 - - - Operating supply forI/Os
VCCIO L14 - - - Operating supply forI/Os
VCCIO M14 - - - Operating supply forI/Os
VCCIO N14 - - - Operating supply forI/Os
VCCIO P14 - - - Operating supply forI/Os
VCCIO P13 - - - Operating supply forI/Os
VCCIO P12 - - - Operating supply forI/Os
VCCIO P9 - - - Operating supply forI/Os
VCCIO P8 - - - Operating supply forI/Os
VCCIO P7 - - - Operating supply forI/Os
VCCIO P6 - - - Operating supply forI/Os
VCCIO N6 - - - Operating supply forI/Os
VCCIO M6 - - - Operating supply forI/Os
VCCIO J6 - - - Operating supply forI/Os
VCCIO H6 - - - Operating supply forI/Os
VCCIO G6 - - - Operating supply forI/Os
VCCIO F6 - - - Operating supply forI/Os
VCCIO F7 - - - Operating supply forI/Os

<!-- Page 45 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435745TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1.24 Ground Reference forAllSupplies Except VCCAD
Table 4-26. ZWT Ground Reference forAllSupplies Except VCCAD
TERMINALSIGNAL
TYPEDEFAULT
PULL
STATEPULL TYPEOUTPUT BUFFER
DRIVE STRENGTHDESCRIPTION
SIGNAL NAME 337ZWT
VSS W1
Ground- - - Ground reference
VSS V1 - - - Ground reference
VSS W2 - - - Ground reference
VSS B1 - - - Ground reference
VSS A1 - - - Ground reference
VSS A2 - - - Ground reference
VSS A18 - - - Ground reference
VSS A19 - - - Ground reference
VSS B19 - - - Ground reference
VSS M8 - - - Ground reference
VSS M9 - - - Ground reference
VSS M11 - - - Ground reference
VSS M12 - - - Ground reference
VSS L8 - - - Ground reference
VSS L9 - - - Ground reference
VSS L10 - - - Ground reference
VSS L11 - - - Ground reference
VSS L12 - - - Ground reference
VSS K9 - - - Ground reference
VSS K10 - - - Ground reference
VSS K11 - - - Ground reference
VSS J8 - - - Ground reference
VSS J9 - - - Ground reference
VSS J10 - - - Ground reference
VSS J11 - - - Ground reference
VSS J12 - - - Ground reference
VSS H8 - - - Ground reference
VSS H9 - - - Ground reference
VSS H11 - - - Ground reference
VSS H12 - - - Ground reference

<!-- Page 46 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435746TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
4.2.1.25 Other Supplies
Table 4-27. Other Supplies
TERMINAL
SIGNAL
TYPEDEFAULT
PULL
STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
Supply forPLL: 1.2-V nominal
VCCPLL P111.2-V
Power- - - Core supply forPLL's