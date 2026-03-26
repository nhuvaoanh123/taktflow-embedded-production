# Architecture - System Control Registers (SYS)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 151-204

---


<!-- Page 151 -->

www.ti.com System andPeripheral Control Registers
151 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5 System andPeripheral Control Registers
The following sections describe thesystem andperipheral control registers oftheTMS570LC43x
microcontroller.
2.5.1 Primary System Control Registers (SYS)
This section describes theSYSTEM registers. These registers aredivided intotwoseparate frames. The
start address oftheprimary system module frame isFFFF FF00h. The start address ofthesecondary
system module frame isFFFF E100h. The registers support 8-,16-, and32-bit writes. The offset isrelative
tothesystem module frame start address.
Table 2-18 contains alistoftheprimary system control registers.
Table 2-18. Primary System Control Registers
Offset Acronym Register Description Section
00h SYSPC1 SYS PinControl Register 1 Section 2.5.1.1
04h SYSPC2 SYS PinControl Register 2 Section 2.5.1.2
08h SYSPC3 SYS PinControl Register 3 Section 2.5.1.3
0Ch SYSPC4 SYS PinControl Register 4 Section 2.5.1.4
10h SYSPC5 SYS PinControl Register 5 Section 2.5.1.5
14h SYSPC6 SYS PinControl Register 6 Section 2.5.1.6
18h SYSPC7 SYS PinControl Register 7 Section 2.5.1.7
1Ch SYSPC8 SYS PinControl Register 8 Section 2.5.1.8
20h SYSPC9 SYS PinControl Register 9 Section 2.5.1.9
30h CSDIS Clock Source Disable Register Section 2.5.1.10
34h CSDISSET Clock Source Disable SetRegister Section 2.5.1.11
38h CSDISCLR Clock Source Disable Clear Register Section 2.5.1.12
3Ch CDDIS Clock Domain Disable Register Section 2.5.1.13
40h CDDISSET Clock Domain Disable SetRegister Section 2.5.1.14
44h CDDISCLR Clock Domain Disable Clear Register Section 2.5.1.15
48h GHVSRC GCLK1, HCLK, VCLK, andVCLK2 Source Register Section 2.5.1.16
4Ch VCLKASRC Peripheral Asynchronous Clock Source Register Section 2.5.1.17
50h RCLKSRC RTIClock Source Register Section 2.5.1.18
54h CSVSTAT Clock Source Valid Status Register Section 2.5.1.19
58h MSTGCR Memory Self-Test Global Control Register Section 2.5.1.20
5Ch MINITGCR Memory Hardware Initialization Global Control Register Section 2.5.1.21
60h MSINENA Memory Self-Test/Initialization Enable Register Section 2.5.1.22
68h MSTCGSTAT MSTC Global Status Register Section 2.5.1.23
6Ch MINISTAT Memory Hardware Initialization Status Register Section 2.5.1.24
70h PLLCTL1 PLL Control Register 1 Section 2.5.1.25
74h PLLCTL2 PLL Control Register 2 Section 2.5.1.26
78h SYSPC10 SYS PinControl Register 10 Section 2.5.1.27
7Ch DIEIDL DieIdentification Register, Lower Word Section 2.5.1.28
80h DIEIDH DieIdentification Register, Upper Word Section 2.5.1.29
88h LPOMONCTL LPO/CLock Monitor Control Register Section 2.5.1.31
8Ch CLKTEST Clock Test Register Section 2.5.1.31
90h DFTCTRLREG DFT Control Register Section 2.5.1.32
94h DFTCTRLREG2 DFT Control Register 2 Section 2.5.1.33
A0h GPREG1 General Purpose Register Section 2.5.1.34
B0h SSIR1 System Software Interrupt Request 1Register Section 2.5.1.35
B4h SSIR2 System Software Interrupt Request 2Register Section 2.5.1.36
B8h SSIR3 System Software Interrupt Request 3Register Section 2.5.1.37

<!-- Page 152 -->

System andPeripheral Control Registers www.ti.com
152 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-18. Primary System Control Registers (continued)
Offset Acronym Register Description Section
BCh SSIR4 System Software Interrupt Request 4Register Section 2.5.1.38
C0h RAMGCR RAM Control Register Section 2.5.1.39
C4h BMMCR1 Bus Matrix Module Control Register 1 Section 2.5.1.40
CCh CPURSTCR CPU Reset Control Register Section 2.5.1.41
D0h CLKCNTL Clock Control Register Section 2.5.1.42
D4h ECPCNTL ECP Control Register Section 2.5.1.43
DCh DEVCR1 DEV Parity Control Register 1 Section 2.5.1.44
E0h SYSECR System Exception Control Register Section 2.5.1.45
E4h SYSESR System Exception Status Register Section 2.5.1.46
E8h SYSTASR System Test Abort Status Register Section 2.5.1.47
ECh GLBSTAT Global Status Register Section 2.5.1.48
F0h DEVID Device Identification Register Section 2.5.1.49
F4h SSIVEC Software Interrupt Vector Register Section 2.5.1.50
F8h SSIF System Software Interrupt Flag Register Section 2.5.1.51

<!-- Page 153 -->

www.ti.com System andPeripheral Control Registers
153 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.1 SYS PinControl Register 1(SYSPC1)
The SYSPC1 register, shown inFigure 2-8anddescribed inTable 2-19 ,controls thefunction oftheECLK
pin.
Figure 2-8.SYS PinControl Register 1(SYSPC1) (offset =00h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKFUN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-19. SYS PinControl Register 1(SYSPC1) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKFUN ECLK function. This bitchanges thefunction oftheECLK pin.
0 ECLK isinGIO mode.
1 ECLK isinfunctional mode asaclock output.
Note: Proper ECLK duty cycle isnotguaranteed until 1ECLK cycle haselapsed after
switching intofunctional mode.
2.5.1.2 SYS PinControl Register 2(SYSPC2)
The SYSPC2 register, shown inFigure 2-9anddescribed inTable 2-20 ,controls whether thepinisan
input oranoutput when inGIO mode.
Figure 2-9.SYS PinControl Register 2(SYSPC2) (offset =04h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDIR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-20. SYS PinControl Register 2(SYSPC2) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDIR ECLK data direction. This bitcontrols thedirection oftheECLK pinwhen itisconfigured tobe
inGIO mode only.
0 The ECLK pinisaninput.
Note: Ifthepindirection issetasaninput, theoutput buffer istristated.
1 The ECLK pinisanoutput.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register.

<!-- Page 154 -->

System andPeripheral Control Registers www.ti.com
154 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.3 SYS PinControl Register 3(SYSPC3)
The SYSPC3 register, shown inFigure 2-10 anddescribed inTable 2-21,displays thelogic state ofthe
ECLK pinwhen itisinGIO mode.
Figure 2-10. SYS PinControl Register 3(SYSPC3) (offset =08h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDIN
R-0 R-U
LEGEND: R=Read only; U=value isundefined; -n=value after reset
Table 2-21. SYS PinControl Register 3(SYSPC3) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDIN ECLK data in.This bitdisplays thelogic state oftheECLK pinwhen itisconfigured tobein
GIO mode.
0 The ECLK pinisatlogic low(0).
1 The ECLK pinisatlogic high (1).
2.5.1.4 SYS PinControl Register 4(SYSPC4)
The SYSPC4 register, shown inFigure 2-11 anddescribed inTable 2-22,controls thelogic level output
function oftheECLK pinwhen itisconfigured asanoutput inGIO mode.
Figure 2-11. SYS PinControl Register 4(SYSPC4) (offset =0Ch)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKDOUT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-22. SYS PinControl Register 4(SYSPC4) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKDOUT ECLK data outwrite. This bitisonly active when theECLK pinisconfigured tobeinGIO mode.
Writes tothisbitwillonly take effect when theECLK pinisconfigured asanoutput inGIO
mode. The current logic state oftheECLK pinwillbedisplayed bythisbitinboth input and
output GIO mode.
0 The ECLK pinisdriven tologic low(0).
1 The ECLK pinisdriven tologic high (1).
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.

<!-- Page 155 -->

www.ti.com System andPeripheral Control Registers
155 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.5 SYS PinControl Register 5(SYSPC5)
The SYSPC5 register, shown inFigure 2-12 anddescribed inTable 2-23,controls thesetfunction ofthe
ECLK pinwhen itisconfigured asanoutput inGIO mode.
Figure 2-12. SYS PinControl Register 5(SYSPC5) (offset =10h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKSET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-23. SYS PinControl Register 5(SYSPC5) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKSET ECLK data outset.This bitdrives theoutput oftheECLK pinhigh when setinGIO output
mode.
0 Write: Writing a0hasnoeffect.
1 Write: The ECLK pinisdriven tologic high (1).
Note: Thecurrent logic state oftheECPCLKDOUT bitwillalso bedisplayed bythisbit
when thepinisconfigured inGIOoutput mode.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.
2.5.1.6 SYS PinControl Register 6(SYSPC6)
The SYSPC6 register, shown inFigure 2-13 anddescribed inTable 2-24,controls theclear function ofthe
ECLK pinwhen itisconfigured asanoutput inGIO mode..
Figure 2-13. SYS PinControl Register 6(SYSPC6) (offset =14h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKCLR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-24. SYS PinControl Register 6(SYSPC6) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKCLR ECLK data outclear. This bitdrives theoutput oftheECLK pinlowwhen setinGIO output
mode.
0 Write: The ECLK pinvalue isunchanged.
1 Write: The ECLK pinisdriven tologic low(0).
Note: Thecurrent logic state oftheECPCLKDOUT bitwillalso bedisplayed bythisbit
when thepinisconfigured inGIOoutput mode.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced inoutput mode bysetting theECPCLKDIR bit
to1intheSYSPC2 register.

<!-- Page 156 -->

System andPeripheral Control Registers www.ti.com
156 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.7 SYS PinControl Register 7(SYSPC7)
The SYSPC7 register, shown inFigure 2-14 anddescribed inTable 2-25,controls theopen drain function
oftheECLK pin.
Figure 2-14. SYS PinControl Register 7(SYSPC7) (offset =18h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKODE
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-25. SYS PinControl Register 7(SYSPC7) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKODE ECLK open drain enable. This bitisonly active when ECLK isconfigured tobeinGIO mode.
0 The ECLK pinisconfigured inpush/pull (normal GIO) mode.
1 The ECLK pinisconfigured inopen drain mode. The ECPCLKDOUT bitintheSYSPC4 register
controls thestate oftheECLK output buffer:
ECPCLKDOUT =0:The ECLK output buffer isdriven low.
ECPCLKDOUT =1:The ECLK output buffer istristated.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register.
2.5.1.8 SYS PinControl Register 8(SYSPC8)
The SYSPC8 register, shown inFigure 2-15 anddescribed inTable 2-26,controls thepullenable function
oftheECLK pinwhen itisconfigured asaninput inGIO mode.
Figure 2-15. SYS PinControl Register 8(SYSPC8) (offset =1Ch)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKPUE
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; D=Device Specific; -n=value after reset
Table 2-26. SYS PinControl Register 8(SYSPC8) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKPUE ECLK pullenable. Writes tothisbitwillonly take effect when theECLK pinisconfigured asan
input inGIO mode.
0 ECLK pullenable isactive.
1 ECLK pullenable isinactive.
Note: Thepulldirection (up/down) isselected bytheECPCLKPS bitintheSYSPC9
register.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced ininput mode byclearing theECPCLKDIR bit
to0intheSYSPC2 register.

<!-- Page 157 -->

www.ti.com System andPeripheral Control Registers
157 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.9 SYS PinControl Register 9(SYSPC9)
The SYSPC9 register, shown inFigure 2-16 anddescribed inTable 2-27,controls thepullup/pull down
configuration oftheECLK pinwhen itisconfigured asaninput inGIO mode.
Figure 2-16. SYS PinControl Register 9(SYSPC9) (offset =20h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLKPS
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-27. SYS PinControl Register 9(SYSPC9) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLKPS ECLK pullup/pull down select. This bitisonly active when ECLK isconfigured asaninput in
GIO mode andthepullup/pull down logic isenabled.
0 ECLK pulldown isselected, when pullup/pull down logic isenabled.
1 ECLK pullupisselected, when pullup/pull down logic isenabled.
Note: TheECLK pinpullup/pull down logic isenabled byclearing theECPCLKPUE bitto
0intheSYSPC8 register.
Note: TheECLK pinisplaced intoGIOmode byclearing theECPCLKFUN bitto0inthe
SYSPC1 register. TheECLK pinisplaced ininput mode byclearing theECPCLKDIR bit
to0intheSYSPC2 register.

<!-- Page 158 -->

System andPeripheral Control Registers www.ti.com
158 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.10 Clock Source Disable Register (CSDIS)
The CSDIS register, shown inFigure 2-17 anddescribed inTable 2-28,controls anddisplays thestate of
thedevice clock sources.
Figure 2-17. Clock Source Disable Register (CSDIS) (offset =30h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLKSR7OFF CLKSR6OFF CLKSR5OFF CLKSR4OFF CLKSR3OFF Reserved CLKSR1OFF CLKSR0OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-28. Clock Source Disable Register (CSDIS) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 CLKSR[7-3]OFF Clock source[7-3] off.
0 Clock source[7-3] isenabled.
1 Clock source[7-3] isdisabled.
Note: Onwakeup, only clock sources 0,4,and5areenabled.
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 CLKSR[1-0]OFF Clock source[1-0] off.
0 Clock source[1-0] isenabled.
1 Clock source[1-0] isdisabled.
Note: Onwakeup, only clock sources 0,4,and5areenabled.
Table 2-29. Clock Sources Table
Clock Source # Clock Source Name
Clock Source 0 Oscillator
Clock Source1 PLL1
Clock Source 2 NotImplemented
Clock Source 3 EXTCLKIN
Clock Source 4 Low Frequency LPO (Low Power Oscillator) clock
Clock Source 5 High frequency LPO (Low Power Oscillator) clock
Clock Source 6 PLL2
Clock Source 7 EXTCLKIN2
NOTE: Non-implemented clock sources should notbeenabled orused.

<!-- Page 159 -->

www.ti.com System andPeripheral Control Registers
159 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.11 Clock Source Disable SetRegister (CSDISSET)
The CSDISSET register, shown inFigure 2-18 anddescribed inTable 2-30,sets clock sources tothe
disabled state.
Figure 2-18. Clock Source Disable SetRegister (CSDISSET) (offset =34h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
SETCLKSR7
OFFSETCLKSR6
OFFSETCLKSR5
OFFSETCLKSR4
OFFSETCLKSR3
OFFReserved SETCLKSR1
OFFSETCLKSR0
OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-30. Clock Source Disable SetRegister (CSDISSET) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 SETCLKSR[7-3]OFF Setclock source[7-3] tothedisabled state.
0 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] isunchanged.
1 Read: Clock source[7-3] isdisabled.
Write: Clock source[7-3] issettothedisabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 SETCLKSR[1-0]OFF Setclock source[1-0] tothedisabled state.
0 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] isunchanged.
1 Read: Clock source[1-0] isdisabled.
Write: Clock source[1-0] issettothedisabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 160 -->

System andPeripheral Control Registers www.ti.com
160 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.12 Clock Source Disable Clear Register (CSDISCLR)
The CSDISCLR register, shown inFigure 2-19 anddescribed inTable 2-31,clears clock sources tothe
enabled state.
Figure 2-19. Clock Source Disable Clear Register (CSDISCLR) (offset =38h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLRCLKSR7
OFFCLRCLKSR6
OFFCLRCLKSR5
OFFCLRCLKSR4
OFFCLRCLKSR3
OFFReserved CLRCLKSR1
OFFCLRCLKSR0
OFF
R/WP-1 R/WP-1 R/WP-0 R/WP-0 R/WP-1 R-1 R/WP-1 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-31. Clock Source Disable Clear Register (CSDISCLR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-3 CLRCLKSR[7-3]OFF Enables clock source[7-3].
0 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] isunchanged.
1 Read: Clock source[7-3] isenabled.
Write: Clock source[7-3] issettotheenabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h), andtheCSDISCLR register (offset 38h).
2 Reserved 1 Reads return 1.Writes have noeffect.
1-0 CLRCLKSR[1-0]OFF Enables clock source[1-0].
0 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] isunchanged.
1 Read: Clock source[1-0] isenabled.
Write: Clock source[1-0] issettotheenabled state.
Note: After anew clock source disable bitissetviatheCSDISSET register, thenew
status ofthebitwillbereflected intheCSDIS register (offset 30h), theCSDISSET
register (offset 34h) andtheCSDISCLR register (offset 38h).
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 161 -->

www.ti.com System andPeripheral Control Registers
161 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.13 Clock Domain Disable Register (CDDIS)
The CDDIS register, shown inFigure 2-20 anddescribed inTable 2-32,controls thestate oftheclock
domains.
NOTE: Alltheclock domains areenabled onwakeup.
The application should assure thatwhen HCLK andVCLK_sys areturned offthrough the
HCLKOFF bit,theGCLK1 domain isalso turned off.
The register bitsinCDDIS aredesignated ashigh-integrity bitsandhave been implemented
with error-correcting logic such thateach bit,although read andwritten asasingle bit,is
actually amulti-bit keywith error correction capability. Assuch, single-bit flips within the"key"
canbecorrected allowing protection ofthesystem asawhole. Anerror detected issignaled
totheESM module.
Figure 2-20. Clock Domain Disable Register (CDDIS) (offset =3Ch)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved VCLKA4OFF Reserved Reserved VCLK3OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved RTICLK1OFF VCLKA2OFF VCLKA1OFF VCLK2OFF VCLKPOFF HCLKOFF GCLK1OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-32. Clock Domain Disable Register (CDDIS) Field Descriptions
Bit Field Value Description
31-12 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
11 VCLKA4OFF VCLKA4 domain off.
0 The VCLKA4 domain isenabled.
1 The VCLKA4 domain isdisabled.
10-9 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
8 VCLK3OFF VCLK3 domain off.
0 The VCLK3 domain isenabled.
1 The VCLK3 domain isdisabled.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 RTICLK1OFF RTICLK1 domain off.
0 The RTICLK1 domain isenabled.
1 The RTICLK1 domain isdisabled.
5-4 VCLKA[2-1]OFF VCLKA[2-1] domain off.
0 The VCLKA[2-1] domain isenabled.
1 The VCLKA[2-1] domain isdisabled.
3 VCLK2OFF VCLK2 domain off.
0 The VCLK2 domain isenabled.
1 The VCLK2 domain isdisabled.
2 VCLKPOFF VCLK_periph domain off.
0 The VCLK_periph domain isenabled.
1 The VCLK_periph domain isdisabled.

<!-- Page 162 -->

System andPeripheral Control Registers www.ti.com
162 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-32. Clock Domain Disable Register (CDDIS) Field Descriptions (continued)
Bit Field Value Description
1 HCLKOFF HCLK andVCLK_sys domains off.
0 The HCLK andVCLK_sys domains areenabled.
1 The HCLK andVCLK_sys domains aredisabled.
0 GCLK1OFF GCLK1 domain off.
0 The GCLK1 domain isenabled.
1 The GCLK1 domain isdisabled.

<!-- Page 163 -->

www.ti.com System andPeripheral Control Registers
163 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.14 Clock Domain Disable SetRegister (CDDISSET)
This CDDISSET register, shown inFigure 2-21 anddescribed inTable 2-33,sets clock domains tothe
disabled state.
Figure 2-21. Clock Domain Disable SetRegister (CDDISSET) (offset =40h)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved SETVCLKA4
OFFReserved Reserved SETVCLK3
OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved SETRTICLK1
OFFSETVCLKA2
OFFSETVCLKA1
OFFSETVCLK2
OFFSETVCLKP
OFFSETHCLK
OFFSETGCLK1
OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-33. Clock Domain Disable SetRegister (CDDISSET) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11 SETVCLKA4OFF SetVCLKA4 domain.
0 Read: The VCLKA4 domain isenabled.
Write: The VCLKA4 domain isunchanged.
1 Read: The VCLKA4 domain isdisabled.
Write: The VCLKA4 domain issettotheenabled state.
10-9 Reserved 0 Reads return zero oroneandprivilege mode writes allowed.
8 SETVCLK3OFF SetVCLK3 domain.
0 Read: The VCLK3 domain isenabled.
Write: The VCLK3 domain isunchanged.
1 Read: The VCLK3 domain isdisabled.
Write: The VCLK3 domain issettotheenabled state.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 SETRTICLK1OFF SetRTICLK1 domain.
0 Read: The RTICLK1 domain isenabled.
Write: The RTICLK1 domain isunchanged.
1 Read: The RTICLK1 domain isdisabled.
Write: The RTICLK1 domain issettotheenabled state.
5-4 SETVCLKA[2-1]OFF SetVCLKA[2-1] domain.
0 Read: The VCLKA[2-1] domain isenabled.
Write: The VCLKA[2-1] domain isunchanged.
1 Read: The VCLKA[2-1] domain isdisabled.
Write: The VCLKA[2-1] domain issettotheenabled state.
3 SETVCLK2OFF SetVCLK2 domain.
0 Read: The VCLK2 domain isenabled.
Write: The VCLK2 domain isunchanged.
1 Read: The VCLK2 domain isdisabled.
Write: The VCLK2 domain issettotheenabled state.

<!-- Page 164 -->

System andPeripheral Control Registers www.ti.com
164 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-33. Clock Domain Disable SetRegister (CDDISSET) Field Descriptions (continued)
Bit Field Value Description
2 SETVCLKPOFF SetVCLK_periph domain.
0 Read: The VCLK_periph domain isenabled.
Write: The VCLK_periph domain isunchanged.
1 Read: The VCLK_periph domain isdisabled.
Write: The VCLK_periph domain issettotheenabled state.
1 SETHCLKOFF SetHCLK andVCLK_sys domains.
0 Read: The HCLK andVCLK_sys domain isenabled.
Write: The HCLK andVCLK_sys domain isunchanged.
1 Read: The HCLK andVCLK_sys domain isdisabled.
Write: The HCLK andVCLK_sys domain issettotheenabled state.
0 SETGCLK1OFF SetGCLK1 domain.
0 Read: The GCLK1 domain isenabled.
Write: The GCLK1 domain isunchanged.
1 Read: The GCLK1 domain isdisabled.
Write: The GCLK1 domain issettotheenabled state.

<!-- Page 165 -->

www.ti.com System andPeripheral Control Registers
165 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.15 Clock Domain Disable Clear Register (CDDISCLR)
The CDDISCLR register, shown inFigure 2-22 anddescribed inTable 2-34,clears clock domains tothe
enabled state.
Figure 2-22. Clock Domain Disable Clear Register (CDDISCLR) (offset =44h)
31 16
Reserved
R-0
15 12 11 10 9 8
Reserved CLRVCLKA4
OFFReserved Reserved CLRVCLK3
OFF
R-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved CLRRTICLK1
OFFCLRVCLKA2
OFFCLRVCLKA1
OFFCLRVCLK2
OFFCLRVCLKP
OFFCLRHCLK
OFFCLRGCLK1
OFF
R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-34. Clock Domain Disable Clear Register (CDDISCLR) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11 CLRVCLKA4OFF Clear VCLKA4 domain.
0 Read: The VCLKA4 domain isenabled.
Write: The VCLKA4 domain isunchanged.
1 Read: The VCLKA4 domain isdisabled.
Write: The VCLKA4 domain iscleared totheenabled state.
10-9 Reserved 0 Reads return zero oroneandprivilege mode writes allowed.
8 CLRVCLK3OFF Clear VCLK3 domain.
0 Read: The VCLK3 domain isenabled.
Write: The VCLK3 domain isunchanged.
1 Read: The VCLK3 domain isdisabled.
Write: The VCLK3 domain iscleared totheenabled state.
7 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
6 CLRRTICLK1OFF Clear RTICLK1 domain.
0 Read: The RTICLK1 domain isenabled.
Write: The RTICLK1 domain isunchanged.
1 Read: The RTICLK1 domain isdisabled.
Write: The RTICLK1 domain iscleared totheenabled state.
5-4 CLRVCLKA[2-1]OFF Clear VCLKA[2-1] domain.
0 Read: The VCLKA[2-1] domain isenabled.
Write: The VCLKA[2-1] domain isunchanged.
1 Read: The VCLKA[2-1] domain isdisabled.
Write: The VCLKA[2-1] domain iscleared totheenabled state.
3 CLRVCLK2OFF Clear VCLK2 domain.
0 Read: The VCLK2 domain isenabled.
Write: The VCLK2 domain isunchanged.
1 Read: The VCLK2 domain isdisabled.
Write: The VCLK2 domain iscleared totheenabled state.

<!-- Page 166 -->

System andPeripheral Control Registers www.ti.com
166 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-34. Clock Domain Disable Clear Register (CDDISCLR) Field Descriptions (continued)
Bit Field Value Description
2 CLRVCLKPOFF Clear VCLK_periph domain.
0 Read: The VCLK_periph domain isenabled.
Write: The VCLK_periph domain isunchanged.
1 Read: The VCLK_periph domain isdisabled.
Write: The VCLK_periph domain iscleared totheenabled state.
1 CLRHCLKOFF Clear HCLK andVCLK_sys domains.
0 Read: The HCLK andVCLK_sys domain isenabled.
Write: The HCLK andVCLK_sys domain isunchanged.
1 Read: The HCLK andVCLK_sys domain isdisabled.
Write: The HCLK andVCLK_sys domain iscleared totheenabled state.
0 CLRGCLK1OFF Clear GCLK1 domain.
0 Read: The GCLK1 domain isenabled.
Write: The GCLK1 domain isunchanged.
1 Read: The GCLK1 domain isdisabled.
Write: The GCLK1 domain iscleared totheenabled state.

<!-- Page 167 -->

www.ti.com System andPeripheral Control Registers
167 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.16 GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC)
The GHVSRC register, shown inFigure 2-23 anddescribed inTable 2-35,controls theclock source
configuration fortheGCLK1, HCLK, VCLK andVCLK2 clock domains.
Figure 2-23. GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC) (offset =48h)
31 28 27 24 23 20 19 16
Reserved GHVWAKE Reserved HVLPM
R-0 R/WP-0 R-0 R/WP-0
15 4 3 0
Reserved GHVSRC
R-0 R/WP-0
LEGEND: R=Read only; R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-35. GCLK1, HCLK, VCLK, andVCLK2 Source Register (GHVSRC) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 GHVWAKE GCLK1, HCLK, VCLK source onwakeup.
0 Clock source0 isthesource forGCLK1, HCLK, VCLK onwakeup.
1h Clock source1 isthesource forGCLK1, HCLK, VCLK onwakeup.
2h Clock source2 isthesource forGCLK1, HCLK, VCLK onwakeup.
3h Clock source3 isthesource forGCLK1, HCLK, VCLK onwakeup.
4h Clock source4 isthesource forGCLK1, HCLK, VCLK onwakeup.
5h Clock source5 isthesource forGCLK1, HCLK, VCLK onwakeup.
6h Clock source6 isthesource forGCLK1, HCLK, VCLK onwakeup.
7h Clock source7 isthesource forGCLK1, HCLK, VCLK onwakeup.
8h-Fh Reserved
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 HVLPM HCLK, VCLK, VCLK2 source onwakeup when GCLK1 isturned off.
0 Clock source0 isthesource forHCLK, VCLK, VCLK2 onwakeup.
1h Clock source1 isthesource forHCLK, VCLK, VCLK2 onwakeup.
2h Clock source2 isthesource forHCLK, VCLK, VCLK2 onwakeup.
3h Clock source3 isthesource forHCLK, VCLK, VCLK2 onwakeup.
4h Clock source4 isthesource forHCLK, VCLK, VCLK2 onwakeup.
5h Clock source5 isthesource forHCLK, VCLK, VCLK2 onwakeup.
6h Clock source6 isthesource forHCLK, VCLK, VCLK2 onwakeup.
7h Clock source7 isthesource forHCLK, VCLK, VCLK2 onwakeup.
8h-Fh Reserved
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 GHVSRC GCLK1, HCLK, VCLK, VCLK2 current source.
Note: TheGHVSRC[3-0] bitsareupdated with theHVLPM[3-0] setting when GCLK1 is
turned off,andareupdated with theGHVWAKE[3-0] setting onsystem wakeup.
0 Clock source0 isthesource forGCLK1, HCLK, VCLK, VCLK2.
1h Clock source1 isthesource forGCLK1, HCLK, VCLK, VCLK2.
2h Clock source2 isthesource forGCLK1, HCLK, VCLK, VCLK2.
3h Clock source3 isthesource forGCLK1, HCLK, VCLK, VCLK2.
4h Clock source4 isthesource forGCLK1, HCLK, VCLK, VCLK2.
5h Clock source5 isthesource forGCLK1, HCLK, VCLK, VCLK2.
6h Clock source6 isthesource forGCLK1, HCLK, VCLK, VCLK2.
7h Clock source7 isthesource forGCLK1, HCLK, VCLK, VCLK2.
8h-Fh Reserved

<!-- Page 168 -->

System andPeripheral Control Registers www.ti.com
168 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureNOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown intheTable 2-29.
2.5.1.17 Peripheral Asynchronous Clock Source Register (VCLKASRC)
The VCLKASRC register, shown inFigure 2-24 anddescribed inTable 2-36,sets theclock source forthe
asynchronous peripheral clock domains tobeconfigured torunfrom aspecific clock source.
Figure 2-24. Peripheral Asynchronous Clock Source Register (VCLKASRC) (offset =4Ch)
31 28 27 24 23 20 19 16
Reserved Reserved Reserved Reserved
R-0 R/WP-1h R-0 R/WP-1h
15 12 11 8 7 4 3 0
Reserved VCLKA2S Reserved VCLKA1S
R-0 R/WP-9h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-36. Peripheral Asynchronous Clock Source Register (VCLKASRC) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 Reserved 0-1 Reads return 0or1andprivilege mode writes allowed.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 VCLKA2S Peripheral asynchronous clock2 source.
0 Clock source0 isthesource forperipheral asynchronous clock2.
1h Clock source1 isthesource forperipheral asynchronous clock2.
2h Clock source2 isthesource forperipheral asynchronous clock2.
3h Clock source3 isthesource forperipheral asynchronous clock2.
4h Clock source4 isthesource forperipheral asynchronous clock2.
5h Clock source5 isthesource forperipheral asynchronous clock2.
6h Clock source6 isthesource forperipheral asynchronous clock2.
7h Clock source7 isthesource forperipheral asynchronous clock2.
8h-Fh VCLK isthesource forperipheral asynchronous clock2.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 VCLKA1S Peripheral asynchronous clock1 source.
0 Clock source0 isthesource forperipheral asynchronous clock1.
1h Clock source1 isthesource forperipheral asynchronous clock1.
2h Clock source2 isthesource forperipheral asynchronous clock1.
3h Clock source3 isthesource forperipheral asynchronous clock1.
4h Clock source4 isthesource forperipheral asynchronous clock1.
5h Clock source5 isthesource forperipheral asynchronous clock1.
6h Clock source6 isthesource forperipheral asynchronous clock1.
7h Clock source7 isthesource forperipheral asynchronous clock1.
8h-Fh VCLK isthesource forperipheral asynchronous clock1.
NOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown inTable 2-29.

<!-- Page 169 -->

www.ti.com System andPeripheral Control Registers
169 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.18 RTIClock Source Register (RCLKSRC)
The RCLKSRC register, shown inFigure 2-25 anddescribed inTable 2-37,controls theRTI(Real Time
Interrupt) clock source selection.
NOTE: Important constraint when theRTIclock source isnotVCLK
IftheRTIx clock source ischosen tobeanything other than thedefault VCLK, then theRTI
clock needs tobeatleast three times slower than theVCLK. This canbeachieved by
configuring theRTIxCLK divider inthisregister. This divider isinternally bypassed when the
RTIx clock source isVCLK.
Figure 2-25. RTIClock Source Register (RCLKSRC) (offset =50h)
31 16
Reserved
R-0
15 10 9 8 7 4 3 0
Reserved RTI1DIV Reserved RTI1SRC
R-0 R/WP-1h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-37. RTIClock Source Register (RCLKSRC) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 RTI1DIV RTIclock1 Divider.
0 RTICLK1 divider value is1.
1h RTICLK1 divider value is2.
2h RTICLK1 divider value is4.
3h RTICLK1 divider value is8.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 RTI1SRC RTIclock1 source.
0 Clock source0 isthesource forRTICLK1.
1h Clock source1 isthesource forRTICLK1.
2h Clock source2 isthesource forRTICLK1.
3h Clock source3 isthesource forRTICLK1.
4h Clock source4 isthesource forRTICLK1.
5h Clock source5 isthesource forRTICLK1.
6h Clock source6 isthesource forRTICLK1.
7h Clock source7 isthesource forRTICLK1.
8h-Fh VCLK isthesource forRTICLK1.
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 170 -->

System andPeripheral Control Registers www.ti.com
170 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.19 Clock Source Valid Status Register (CSVSTAT)
The CSVSTAT register, shown inFigure 2-26 anddescribed inTable 2-38,indicates thestatus ofusable
clock sources.
Figure 2-26. Clock Source Valid Status Register (CSVSTAT) (offset =54h)
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
CLKSR7V CLKSR6V CLKSR5V CLKSR4V CLKSR3V Reserved CLKSR1V CLKSR0V
R-1 R-0 R-0 R-1 R-1 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 2-38. Clock Source Valid Register (CSVSTAT) Field Descriptions
Bit Field Value Description
31-8 Reserved. 0 Reads return 0.Writes have noeffect.
7-3 CLKSR[7-3]V Clock source[7-0] valid.
0 Clock source[7-0] isnotvalid.
1 Clock source[7-0] isvalid.
Note: Ifthevalid bitofthesource ofaclock domain isnotset(that is,theclock source is
notfully stable), therespective clock domain isdisabled bytheGlobal Clock Module
(GCM).
2 Reserved. 0 Reads return 0.Writes have noeffect.
1-0 CLKSR[1-0]V Clock source[1 -0]valid.
0 Clock source[1 -0]isnotvalid.
1 Clock source[1 -0]isvalid.
Note: Ifthevalid bitofthesource ofaclock domain isnotset(that is,theclock source is
notfully stable), therespective clock domain isdisabled.
NOTE: Alistoftheavailable clock sources isshown intheTable 2-29.

<!-- Page 171 -->

www.ti.com System andPeripheral Control Registers
171 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.20 Memory Self-Test Global Control Register (MSTGCR)
The MSTGCR register, shown inFigure 2-27 anddescribed inTable 2-39,controls several aspects ofthe
PBIST (Programmable Built-In Self Test) memory controller.
Figure 2-27. Memory Self-Test Global Control Register (MSTGCR) (offset =58h)
31 24 23 16
Reserved Reserved
R-0 R/WP-0
15 10 9 8 7 4 3 0
Reserved ROM_DIV Reserved MSTGENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R=Read only; R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-39. Memory Self-Test Global Control Register (MSTGCR) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 Reserved 0-1 Reads return 0or1anddepends onwhat iswritten inprivileged mode. The functionality of
these bitsareunavailable inthisdevice.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 ROM_DIV Prescaler divider bitsforROM clock source.
0 ROM clock source isGCLK1 divided by1.PBIST willreset for16VBUS cycles.
1h ROM clock source isGCLK1 divided by2.PBIST willreset for32VBUS cycles.
2h ROM clock source isGCLK1 divided by4.PBIST willreset for64VBUS cycles.
3h ROM clock source isGCLK1 divided by8.PBIST willreset for96VBUS cycles.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTGENA Memory self-test controller global enable key
Note: Enabling theMSTGENA keywillgenerate areset tothestate machine ofthe
selected PBIST controller.
Ah Memory self-test controller isenabled.
Others Memory self-test controller isdisabled.
Note: Itisrecommended thatavalue ofAhbeused todisable thememory self-test
controller. This value willgive maximum protection from abitflipinducing event that
would inadvertently enable thecontroller.

<!-- Page 172 -->

System andPeripheral Control Registers www.ti.com
172 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.21 Memory Hardware Initialization Global Control Register (MINITGCR)
The MINITGCR register, shown inFigure 2-28 anddescribed inTable 2-40,enables automatic hardware
memory initialization.
Figure 2-28. Memory Hardware Initialization Global Control Register (MINITGCR) (offset =5Ch)
31 16
Reserved
R-0
15 4 3 0
Reserved MINITGENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-40. Memory Hardware Initialization Global Control Register (MINITGCR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MINITGENA Memory hardware initialization global enable key.
Ah Global memory hardware initialization isenabled.
Others Global memory hardware initialization isdisabled.
Note: Itisrecommended thatavalue of5hbeused todisable memory hardware
initialization. This value willgive maximum protection from anevent thatwould
inadvertently enable thecontroller.

<!-- Page 173 -->

www.ti.com System andPeripheral Control Registers
173 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.22 MBIST Controller/ Memory Initialization Enable Register (MSINENA)
The MSINENA register, shown inFigure 2-29 anddescribed inTable 2-41,enables PBIST controllers for
memory selftestandthememory modules initialized during automatic hardware memory initialization.
Figure 2-29. MBIST Controller/Memory Initialization Enable Register (MSINENA) (offset =60h)
31 16
MSIENA
R/WP-0
15 0
MSIENA
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-41. MBIST Controller/Memory Initialization Enable Register (MSINENA) Field Descriptions
Bit Field Value Description
31-0 MSIENA PBIST controller andmemory initialization enable register. Inmemory self-test mode, allthe
corresponding bitsofthememories tobetested should besetbefore enabling theglobal memory self-
testcontroller key(MSTGENA) intheMSTGCR register (offset 58h). The reason forthisisthat
MSTGENA, inaddition tobeing theglobal enable forallindividual PBIST controllers, isthesource for
thereset generation toallthePBIST controller state machines. Disabling theMSTGENA or
MINITGENA key(bywriting from anAhtoanyother value) willreset alltheMSIENA[31-0] bitstotheir
default values.
0 Inmemory self-test mode (MSTGENA =Ah):
PBIST controller [31-0] isdisabled.
Inmemory Initialization mode (MINITGENA =Ah):
Memory module [31-0] auto hardware initialization isdisabled.
1 Inmemory self-test mode (MSTGENA =Ah):
PBIST controller [31-0] isenabled.
Inmemory Initialization mode (MINITGENA =Ah):
Memory module [31-0] auto hardware initialization isenabled.
Note: Software should ensure thatboth thememory self-test global enable key(MSTGENA) and
thememory hardware initialization global key(MINITGENA) arenotenabled atthesame time.

<!-- Page 174 -->

System andPeripheral Control Registers www.ti.com
174 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.23 MSTC Global Status Register (MSTCGSTAT)
The MSTCGSTAT register, shown inFigure 2-30 anddescribed inTable 2-42,shows thestatus ofthe
memory hardware initialization andthememory self-test.
Figure 2-30. MSTC Global Status Register (MSTCGSTAT) (offset =68h)
31 16
Reserved
R-0
15 9 8 7 1 0
Reserved MINIDONE Reserved MSTDONE
R-0 R/WPC-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; WP=Write inprivileged mode only; -n=value after reset
Table 2-42. MSTC Global Status Register (MSTCGSTAT) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 MINIDONE Memory hardware initialization complete status.
Note: Disabling theMINITGENA key(Bywriting from aAhtoanyother value) willclear the
MINIDONE status bitto0.
Note: Individual memory initialization status isshown intheMINISTAT register.
0 Read: Memory hardware initialization isnotcomplete forallmemory.
Write: Awrite of0hasnoeffect.
1 Read: Hardware initialization ofallmemory iscompleted.
Write: The bitiscleared to0.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 MSTDONE Memory self-test runcomplete status.
Note: Disabling theMSTGENA key(bywriting from aAhtoanyother value) willclear the
MSTDONE status bitto0.
0 Read: Memory self-test isnotcompleted.
Write: Awrite of0hasnoeffect.
1 Read: Memory self-test iscompleted.
Write: The bitiscleared to0.

<!-- Page 175 -->

www.ti.com System andPeripheral Control Registers
175 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.24 Memory Hardware Initialization Status Register (MINISTAT)
The MINISTAT register, shown inFigure 2-31 anddescribed inTable 2-43,indicates thestatus of
hardware memory initialization.
Figure 2-31. Memory Hardware Initialization Status Register (MINISTAT) (offset =6Ch)
31 16
MIDONE
R/WP-0
15 0
MIDONE
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-43. Memory Hardware Initialization Status Register (MINISTAT) Field Descriptions
Bit Field Value Description
31-0 MIDONE Memory hardware initialization status bit.
0 Read: Memory module[31-0] hardware initialization isnotcompleted.
Write: Awrite of0hasnoeffect.
1 Read: Memory module[31-0] hardware initialization iscompleted.
Write: The bitiscleared to0.
Note: Disabling theMINITGENA key(bywriting from aAhtoanyother value) willreset allthe
individual status bitsto0.
2.5.1.25 PLL Control Register 1(PLLCTL1)
The PLLCTL1 register, shown inFigure 2-32 anddescribed inTable 2-44,controls theoutput frequency of
PLL1 (Clock Source 1-FMzPLL). Italso controls thebehavior ofthedevice ifaPLL sliporoscillator
failure isdetected.
Figure 2-32. PLL Control Register 1(PLLCTL1) (offset =70h)
31 30 29 28 24
ROS BPOS PLLDIV
R/WP-0 R/WP-1h R/WP-Fh
23 22 21 16
ROF Reserved REFCLKDIV
R/WP-0 R-0 R/WP-3h
15 0
PLLMUL
R/WP-4100h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset

<!-- Page 176 -->

System andPeripheral Control Registers www.ti.com
176 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-44. PLL Control Register 1(PLLCTL1) Field Descriptions
Bit Field Value Description
31 ROS Reset onPLL Slip.
0 Donotreset system when PLL slipisdetected.
1 Reset when PLL slipisdetected.
Note: BPOS (Bits 30-29) must also beenabled forROS tobeenabled.
30-29 BPOS Bypass ofPLL Slip.
2h Bypass onPLL Slipisdisabled. IfaPLL Slipisdetected noaction istaken.
Others Bypass onPLL Slipisenabled. IfaPLL Slipisdetected thedevice willautomatically bypass the
PLL andusetheoscillator toprovide thedevice clock.
Note: IfROS (Bit31)issetto1,thedevice willbereset ifaPLL SlipandthePLL willbe
bypassed after thereset occurs.
28-24 PLLDIV PLL Output Clock Divider
R=PLLDIV +1
fPLL CLK=fpost_ODCLK /R
0 fPLL CLK=fpost-ODCLK /1
1h fPLL CLK=fpost-ODCLK /2
: :
1Fh fPLL CLK=fpost-ODCLK /32
23 ROF Reset onOscillator Fail.
0 Donotreset system when oscillator isoutofrange.
1 The ROF bitenables theOSC_FAIL condition togenerate asystem reset. IftheROF bitinthe
PLLCTL1 register issetwhen theoscillator fails, then asystem reset occurs.
22 Reserved 0 Value hasnoeffect onPLL operation.
21-16 REFCLKDIV Reference Clock Divider
NR=REFCLKDIV +1
fINT CLK=fOSCIN /NR
0 fINT CLK=fOSCIN /1
1h fINT CLK=fOSCIN /2
: :
3Fh fINT CLK=fOSCIN /64
15-0 PLLMUL PLL Multiplication Factor
NF=(PLLMUL /256) +1,valid multiplication factors arefrom 1to256.
fVCO CLK=fINT CLKxNF
0h fVCO CLK=fINT CLKx1
100h fVCO CLK=fINT CLKx2
: :
5B00h fVCO CLK=fINT CLKx92
5C00h fVCO CLK=fINT CLKx93
: :
FF00h fVCO CLK=fINT CLKx256

<!-- Page 177 -->

www.ti.com System andPeripheral Control Registers
177 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.26 PLL Control Register 2(PLLCTL2)
The PLLCTL2 register, shown inFigure 2-33 anddescribed inTable 2-45,controls themodulation
characteristics andtheoutput divider ofthePLL.
Figure 2-33. PLL Control Register 2(PLLCTL2) (offset =74h)
31 30 22 21 20 16
FMENA SPREADINGRATE Rsvd MULMOD
R/WP-0 R/WP-1FFh R-0 R/WP-0
15 12 11 9 8 0
MULMOD ODPLL SPR_AMOUNT
R/WP-7h R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-45. PLL Control Register 2(PLLCTL2) Field Descriptions
Bit Field Value Description
31 FMENA Frequency Modulation Enable.
0 Disable frequency modulation.
1 Enable frequency modulation.
30-22 SPREADINGRATE NS=SPREADINGRATE +1
fmod=fs=fINT CLK/(2×NS)
0 fmod=fs=fINT CLK/(2×1)
1h fmod=fs=fINT CLK/(2×2)
: :
1FFh fmod=fs=fINT CLK/(2×512)
21 Reserved 0 Value hasnoeffect onPLL operation.
20-12 MULMOD Multiplier Correction when Frequency Modulation isenabled.
When FMENA =0,MUL_when_MOD =0;when FMENA =1,MUL_when_MOD =
(MULMOD /256)
0 Noadder toNF.
8h MUL_when_MOD =8/256
9h MUL_when_MOD =9/256
: :
1FFh MUL_when_MOD =511/256
11-9 ODPLL Internal PLL Output Divider
OD=ODPLL +1
fpost-ODCLK =fVCO CLK/OD
Note: PLL output clock isgated off,ifODPLL ischanged while thePLL isactive.
0 fpost-ODCLK =fVCO CLK/1
1h fpost-ODCLK =fVCO CLK/2
: :
7h fpost-ODCLK =fVCO CLK/8
8-0 SPR_AMOUNT Spreading Amount
NV=(SPR_AMOUNT +1)/2048
NVranges from 1/2048 to512/2048
Note thatthePLL output clock isdisabled for1modulation period, iftheSPR_AMOUNT
field ischanged while thefrequency modulation isenabled. Iffrequency modulation is
disabled andSPR_AMOUNT ischanged, there isnoeffect onthePLL output clock.
0 NV=1/2048
1h NV=2/2048
: :
1FFh NV=512/2048

<!-- Page 178 -->

System andPeripheral Control Registers www.ti.com
178 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.27 SYS PinControl Register 10(SYSPC10)
The SYSPC10 register, shown inFigure 2-34 anddescribed inTable 2-46,controls thefunction ofthe
ECPCLK slew mode.
Figure 2-34. SYS PinControl Register 10(SYSPC10) (offset =78h)
31 16
Reserved
R-0
15 1 0
Reserved ECPCLK_SLEW
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-46. SYS PinControl Register 10(SYSPC10) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ECPCLK_SLEW ECPCLK slew control. This bitcontrols between thefastorslow slew mode.
0 Fast mode isenabled; thenormal output buffer isused forthispin.
1 Slow mode isenabled; slew rate control isused forthispin.

<!-- Page 179 -->

www.ti.com System andPeripheral Control Registers
179 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.28 DieIdentification Register Lower Word (DIEIDL)
The DIEIDL register, shown inFigure 2-35 anddescribed inTable 2-47,contains information about thedie
wafer number, andX,Ywafer coordinates.
Figure 2-35. DieIdentification Register, Lower Word (DIEIDL) [offset =7Ch]
31 24 23 16
WAFER # YWAFER COORDINATE
R-D R-D
15 12 11 0
YWAFER COORDINATE XWAFER COORDINATE
R-D R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-47. DieIdentification Register, Lower Word (DIEIDL) Field Descriptions
Bit Field Description
31-24 WAFER # These read-only bitscontain thewafer number ofthedevice.
23-12 YWAFER COORDINATE These read-only bitscontain theYwafer coordinate ofthedevice.
11-0 XWAFER COORDINATE These read-only bitscontain theXwafer coordinate ofthedevice.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.
2.5.1.29 DieIdentification Register Upper Word (DIEIDH)
The DIEIDH register, shown inFigure 2-36 anddescribed inTable 2-48,contains information about the
dielotnumber.
Figure 2-36. DieIdentification Register, Upper Word (DIEIDH) [offset =80h]
31 24 23 16
Reserved LOT #
R-0 R-D
15 0
LOT #
R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-48. DieIdentification Register, Upper Word (DIEIDH) Field Descriptions
Bit Field Description
31-24 Reserved Reserved forTIuse. Writes have noeffect.
23-0 LOT # This read-only register contains thedevice lotnumber.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.

<!-- Page 180 -->

System andPeripheral Control Registers www.ti.com
180 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.30 LPO/Clock Monitor Control Register (LPOMONCTL)
The LPOMONCTL register, shown inFigure 2-37 anddescribed inTable 2-49,controls theLow
Frequency (Clock Source 4)andHigh Frequency (Clock Source 5)Low Power Oscillator's trimvalues.
Figure 2-37. LPO/Clock Monitor Control Register (LPOMONCTL) (offset =088h)
31 25 24 23 17 16
Reserved BIAS ENABLE Reserved OSCFRQCONFIGCNT
R-0 R/WP-1 R-0 R/WP-0
15 13 12 8 7 5 4 0
Reserved HFTRIM Reserved LFTRIM
R-0 R/WP-10h R-0 R/WP-10h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 BIAS ENABLE Bias enable.
0 The bias circuit inside thelow-power oscillator (LPO) isdisabled.
1 The bias circuit inside thelow-power oscillator (LPO) isenabled.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 OSCFRQCONFIGCNT Configures thecounter based onOSC frequency.
0 Read: OSC freq is≤20MHz.
Write: Awrite of0hasnoeffect.
1 Read: OSC freq is>20MHz and≤80MHz.
Write: Awrite of1hasnoeffect.
15-13 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 181 -->

www.ti.com System andPeripheral Control Registers
181 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions (continued)
Bit Field Value Description
12-8 HFTRIM High-frequency oscillator trimvalue. This four-bit value isused tocenter theHF
oscillator 'sfrequency.
Caution: This value should only bechanged when theHFoscillator isnotthe
source foraclock domain, otherwise asystem failure could result.
The following values aretheratio: f/fointheF021 process.
0 29.52
1h 34.24%
2h 38.85%
3h 43.45%
4h 47.99%
5h 52.55%
6h 57.02%
7h 61.46%
8h 65.92%
9h 70.17
Ah 74.55%
Bh 78.92%
Ch 83.17%
Dh 87.43%
Eh 91.75%
Fh 95.89%
10h 100.00% Default atReset.
11h 104.09
12h 108.17
13h 112.32
14h 116.41
15h 120.67
16h 124.42
17h 128.38
18h 132.24
19h 136.15
1Ah 140.15
1Bh 143.94
1Ch 148.02
1Dh 151.80x
1Eh 155.50x
1Fh 159.35%
7-5 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 182 -->

System andPeripheral Control Registers www.ti.com
182 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-49. LPO/Clock Monitor Control Register (LPOMONCTL) Field Descriptions (continued)
Bit Field Value Description
4-0 LFTRIM Low-frequency oscillator trimvalue. This four-bit value isused tocenter theLFoscillator 's
frequency.
Caution: This value should only bechanged when theLFoscillator isnotthe
source foraclock domain, otherwise asystem failure could result.
The following values aretheratio: f/fointheF021 process.
0 20.67
1h 25.76
2h 30.84
3h 35.90
4h 40.93
5h 45.95
6h 50.97
7h 55.91
8h 60.86
9h 65.78
Ah 70.75
Bh 75.63
Ch 80.61
Dh 85.39
Eh 90.23
Fh 95.11
10h 100.00% Default atReset
11h 104.84
12h 109.51
13h 114.31
14h 119.01
15h 123.75
16h 128.62
17h 133.31
18h 138.03
19h 142.75
1Ah 147.32
1Bh 152.02
1Ch 156.63
1Dh 161.38
1Eh 165.90
1Fh 170.42

<!-- Page 183 -->

www.ti.com System andPeripheral Control Registers
183 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.31 Clock Test Register (CLKTEST)
The CLKTEST register, shown inFigure 2-38 anddescribed inTable 2-50,controls theclock signal thatis
supplied totheECLK pinfortestanddebug purposes.
NOTE: Clock Test Register Usage
This register should only beused fortestanddebug purposes.
Figure 2-38. Clock Test Register (CLKTEST) (offset =8Ch)
31 27 26 25 24
Reserved TEST RANGEDET
CTRLRANGEDET
ENASSEL
R-0 R/WP-0 R/WP-0 R/WP-0
23 20 19 16
Reserved CLK_TEST_EN
R-0 R/WP-Ah
15 12 11 8 7 5 4 0
Reserved SEL_GIO_PIN Reserved SEL_ECP_PIN
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-50. Clock Test Register (CLKTEST) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26 TEST 0 This bitisused fortestpurposes. Itmust bewritten to0.
25 RANGEDETCTRL Range detection control. This bit'sfunctionality isdependant onthestate ofthe
RANGEDETENSSEL bit(Bit24)oftheCLKTEST register.
0 The clock monitor range detection circuitry (RANGEDETECTENABLE) isdisabled.
1 The clock monitor range detection circuitry (RANGEDETECTENABLE) isenabled.
24 RANGEDETENASSEL Selects range detection enable. This bitresets asynchronously onpower onreset.
0 The range detect enable isgenerated bythehardware intheclock monitor wrapper.
1 The range detect enable iscontrolled bytheRANGEDETCTRL bit(Bit25)ofthe
CLKTEST register.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 CLK_TEST_EN Clock testenable. This bitenables theclock going totheECLK pin.This bitfield enables
ordisables clock going todevice pins. Two pins inadevice cangetclock sources by
enabling CLK_TEST_EN bits. One pinistheECP andsecond pinisadevice specific GIO
pin.These bitsneed toasynchronously reset.
Note: TheECLK pinmust also beplaced intoFunctional mode bysetting the
ECPCLKFUN bitto1intheSYSPC1 register.
5h Clock going toECLK pinisenabled.
Others Clock going toECLK pinisdisabled.
15-12 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 184 -->

System andPeripheral Control Registers www.ti.com
184 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-50. Clock Test Register (CLKTEST) Field Descriptions (continued)
Bit Field Value Description
11-8 SEL_GIO_PIN GIOB[0] pinclock source valid, clock source select
0 Oscillator valid status
1h PLL1 valid status
2h-4h Reserved
5h High-frequency LPO (Low-Power Oscillator) clock output valid status [CLK10M]
6h PLL2 valid status
7h Reserved
8h Low-frequency LPO (Low-Power Oscillator) clock output valid status [CLK80K]
9h-Ch Oscillator valid status
Dh Reserved
Eh VCLKA4
Fh Oscillator valid status
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 SEL_ECP_PIN ECLK pinclock source select
Note: Only valid clock sources canbeselected fortheECLK pin.Valid clock
sources aredisplayed bytheCSVSTAT register.
0 Oscillator clock
1h PLL1 clock output
2h Reserved
3h EXTCLKIN1
4h Low-frequency LPO (Low-Power Oscillator) clock [CLK80K]
5h High-frequency LPO (Low-Power Oscillator) clock [CLK10M]
6h PLL2 clock output
7h EXTCLKIN2
8h GCLK1
9h RTI1 Base
Ah Reserved
Bh VCLKA1
Ch VCLKA2
Dh Reserved
Eh VCLKA4_DIVR
Fh Flash HDPump Oscillator
10h Reserved
11h HCLK
12h VCLK
13h VCLK2
14h VCLK3
15h-16h Reserved
17h EMAC clock output
18h-1Fh Reserved
NOTE: Non-implemented clock sources should notbeenabled orused.

<!-- Page 185 -->

www.ti.com System andPeripheral Control Registers
185 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.32 DFT Control Register (DFTCTRLREG)
This register isshown inFigure 2-39 anddescribed inTable 2-51.
Figure 2-39. DFT Control Register (DFTCTRLREG) (offset =90h)
31 16
Reserved
R-0
15 14 13 12 11 10 9 8 7 4 3 0
Reserved DFTWRITE Reserved DFTREAD Reserved TEST_MODE_KEY
R-0 R/WP-1h R-0 R/WP-1h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-51. DFT Control Register (DFTCTRLREG) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reads return 0.Writes have noeffect.
13-12 DFTWRITE DFT logic access.
ForF021:
DFTWRITE[0] =0andDFTREAD[0] =0configured instress mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured instress mode.
DFTWRITE[0] =0andDFTREAD[0] =0configured infastmode.
DFTWRITE[1] =1andDFTREAD[1] =1configured infastmode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inslow mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured inslow mode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inscreen mode.
DFTWRITE[1] =1andDFTREAD[1] =1configured inscreen mode.
11-10 Reserved 0 Reads return 0.Writes have noeffect.
9-8 DFTREAD DFT logic access.
ForF021:
DFTWRITE[0] =0andDFTREAD[0] =0configured instress mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured instress mode.
DFTWRITE[0] =0andDFTREAD[0] =0configured infastmode.
DFTWRITE[1] =1andDFTREAD[1] =1configured infastmode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inslow mode.
DFTWRITE[1] =0andDFTREAD[1] =0configured inslow mode.
DFTWRITE[0] =1andDFTREAD[0] =1configured inscreen mode.
DFTWRITE[1] =1andDFTREAD[1] =1configured inscreen mode.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 TEST_MODE_KEY Test mode key. This register isforinternal TIuseonly.
0-Fh
(except Ah)Register keydisable. Allbitsinthisregister willmaintain their default value andcannot be
written.
Ah Register keyenable. ALL thebitscanbewritten toonly when thekeyisenabled. Onreset,
these bitswillbesetto5h.

<!-- Page 186 -->

System andPeripheral Control Registers www.ti.com
186 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.33 DFT Control Register 2(DFTCTRLREG2)
This register isshown inFigure 2-40 anddescribed inTable 2-52.Forinformation onfiltering theRFSLIP
seeSection 2.5.2.7 .
Figure 2-40. DFT Control Register 2(DFTCTRLREG2) (offset =94h)
31 16
IMPDF(27:12)
R/WP-0
15 4 3 0
IMPDF(11:0) TEST_MODE_KEY
R/WP-0 R/WP-5h
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-52. DFT Control Register 2(DFTCTRLREG2) Field Descriptions
Bit Field Value Description
31-4 IMPDF[27:0] DFT Implementation defined bits.
0 IMPDF[27:0] isdisabled.
1 IMPDF[27:0] isenabled.
3-0 TEST_MODE_KEY Test mode key. This register isforinternal TIuseonly.
0-Fh
(except Ah)Register keydisable. Allbitsinthisregister willmaintain their default value andcannot be
written.
Ah Register keyenable. ALL thebitscanbewritten toonly when thekeyisenabled.

<!-- Page 187 -->

www.ti.com System andPeripheral Control Registers
187 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.34 General Purpose Register (GPREG1)
This register isshown inFigure 2-41 anddescribed inTable 2-53.Forinformation onfiltering theRFSLIP,
seeSection 2.5.2.7 .
Figure 2-41. General Purpose Register (GPREG1) (offset =A0h)
31 26 25 20 19 16
Reserved PLL1_FBSLIP_FILTER_COUNT PLL1_FBSLIP_FILTER_KEY
R-0 R/WP-0 R/WP-0
15 0
Reserved
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-53. General Purpose Register (GPREG1) Field Descriptions
Bit Field Value Description
31-26 Reserved 0 Reads return 0.Writes have noeffect.
25-20 PLL1_FBSLIP_FILTER_
COUNTFBSLIP down counter programmed value.
Configures thesystem response when aFBSLIP isindicated bythePLL macro.
When PLL1_FBSLIP_FILTER_KEY isnotAh,thedown counter counts from the
programmed value onevery LPO high-frequency clock once PLL macro indicates
FBSLIP. When thecount reaches 0,ifthesynchronized FBSLIP signal isstillhigh, an
FBSLIP condition isindicated tothesystem module andiscaptured intheglobal
status register. When theFBSLIP signal from thePLL macro isde-asserted before
thecount reaches 0,thecounter isreloaded with theprogrammed value.
Onreset, counter value is0.Counter must beprogrammed toanon-zero value and
enabled forthefiltering tobeenabled.
0 Filtering isdisabled.
1h Filtering isenabled. Every slipisrecognized.
2h Filtering isenabled. The slipmust beatleast 2HFLPO cycles wide inorder tobe
recognized asaslip.
: :
3Fh Filtering isenabled. The slipmust beatleast 63HFLPO cycles wide inorder tobe
recognized asaslip.
19-16 PLL1_FBSLIP_FILTER_
KEYEnable theFBSLIP filtering.
5h Onreset, theFBSLIP filter isdisabled andtheFBSLIP passes through.
Fh This isanunsupported value. You should avoid writing thisvalue tothisbitfield.
Allother
valuesFBSLIP filtering isenabled. Recommended toprogram Ahinthisbitfield. Enabling of
theFBSLIP occurs when theKEY isprogrammed andanon-zero value ispresent in
theCOUNT field.
15-0 Reserved 0-1 Reads return 0or1andwrite inprivilege mode. The functionality ofthisbitis
unavailable inthisdevice.

<!-- Page 188 -->

System andPeripheral Control Registers www.ti.com
188 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.35 System Software Interrupt Request 1Register (SSIR1)
The SSIR1 register, shown inFigure 2-42 anddescribed inTable 2-54,isused forsoftware interrupt
generation.
Figure 2-42. System Software Interrupt Request 1Register (SSIR1) (offset =B0h)
31 16
Reserved
R-0
15 8 7 0
SSKEY1 SSDATA1
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-54. System Software Interrupt Request 1Register (SSIR1) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY1 0-FFh System software interrupt request key. A075h written tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY1 field canbewritten intoonly ifthewrite data
matches thekey(75h). The SSDATA1 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY1 field, matches thekey(75h).
7-0 SSDATA1 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA1 field
cannot bewritten intounless thewrite data intotheSSKEY1 field matches thekey(75h);
therefore, byte writes cannot beperformed ontheSSDATA1 field.
NOTE: This register ismirrored atoffset FCh forcompatibility reasons.

<!-- Page 189 -->

www.ti.com System andPeripheral Control Registers
189 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.36 System Software Interrupt Request 2Register (SSIR2)
The SSIR2 register, shown inFigure 2-43 anddescribed inTable 2-55,isused forsoftware interrupt
generation.
Figure 2-43. System Software Interrupt Request 2Register (SSIR2) (offset =B4h)
31 16
Reserved
R-0
15 8 7 0
SSKEY2 SSDATA2
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-55. System Software Interrupt Request 2Register (SSIR2) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY2 0-FFh System software interrupt2 request key. A84hwritten tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY2 field canbewritten intoonly ifthewrite data
matches thekey(84h). The SSDATA2 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY2 field, matches thekey(84h).
7-0 SSDATA2 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA2 field
cannot bewritten intounless thewrite data intotheSSKEY2 field matches thekey(84h);
therefore, byte writes cannot beperformed ontheSSDATA2 field.

<!-- Page 190 -->

System andPeripheral Control Registers www.ti.com
190 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.37 System Software Interrupt Request 3Register (SSIR3)
The SSIR3 register, shown inFigure 2-44 anddescribed inTable 2-56,isused forsoftware interrupt
generation.
Figure 2-44. System Software Interrupt Request 3Register (SSIR3) (offset =B8h)
31 16
Reserved
R-0
15 8 7 0
SSKEY3 SSDATA3
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-56. System Software Interrupt Request 3Register (SSIR3) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY3 0-FFh System software interrupt request key. A93hwritten tothese bitsinitiates IRQ/FIQ interrupts. Data
inthisfield isalways read as0.The SSKEY3 field canbewritten intoonly ifthewrite data
matches thekey(93h). The SSDATA3 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY3 field, matches thekey(93h).
7-0 SSDATA3 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA3 field
cannot bewritten intounless thewrite data intotheSSKEY3 field matches thekey(93h);
therefore, byte writes cannot beperformed ontheSSDATA3 field.

<!-- Page 191 -->

www.ti.com System andPeripheral Control Registers
191 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.38 System Software Interrupt Request 4Register (SSIR4)
The SSIR4 register, shown inFigure 2-45 anddescribed inTable 2-57,isused forsoftware interrupt
generation.
Figure 2-45. System Software Interrupt Request 4Register (SSIR4) (offset =BCh)
31 16
Reserved
R-0
15 8 7 0
SSKEY4 SSDATA4
R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-57. System Software Interrupt Request 4Register (SSIR4) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSKEY4 0-FFh System software interrupt2 request key. AA2h written tothese bitsinitiates IRQ/FIQ interrupts.
Data inthisfield isalways read as0.The SSKEY4 field canbewritten intoonly ifthewrite data
matches thekey(A2h). The SSDATA4 field canonly bewritten intoifthewrite data intothisfield,
theSSKEY4 field, matches thekey(A2h).
7-0 SSDATA4 0-FFh System software interrupt data. These bitscontain user read/write register bits. They may beused
bytheapplication software asdifferent entry points fortheinterrupt routine. The SSDATA4 field
cannot bewritten intounless thewrite data intotheSSKEY4 field matches thekey(A2h);
therefore, byte writes cannot beperformed ontheSSDATA4 field.

<!-- Page 192 -->

System andPeripheral Control Registers www.ti.com
192 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.39 RAM Control Register (RAMGCR)
NOTE: TheRAM_DFT_EN bitsareforTIinternal useonly.
The contents oftheRAM_DFT_EN field should notbechanged.
Figure 2-46. RAM Control Register (RAMGCR) (offset =C0h)
31 20 19 16
Reserved RAM_DFT_EN
R-0 R/WP-5h
15 14 13 12 11 10 9 8
Reserved Reserved Reserved Reserved Reserved Reserved Reserved Reserved
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
7 6 5 4 3 2 1 0
Reserved Reserved Reserved Reserved Reserved Reserved Reserved Reserved
R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-58. RAM Control Register (RAMGCR) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 RAM_DFT_EN Functional mode RAM DFT (Design ForTest) port enable key.
Note: ForTIinternal useonly.
Ah RAM DFT port isenabled.
Others RAM DFT port isdisabled.
Note: Itisrecommended thatavalue of5hbeused todisable theRAM DFT port. This value
willgive maximum protection from abit-flip inducing event thatwould inadvertently enable
thecontroller.
15 Reserved 0 Reads return 0.Writes have noeffect.
14 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
13 Reserved 0 Reads return 0.Writes have noeffect.
12 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
11 Reserved 0 Reads return 0.Writes have noeffect.
10 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
9 Reserved 0 Reads return 0.Writes have noeffect.
8 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
7 Reserved 0 Reads return 0.Writes have noeffect.
6 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
5 Reserved 0 Reads return 0.Writes have noeffect.
4 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
3 Reserved 0 Reads return 0.Writes have noeffect.
2 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.
1 Reserved 0 Reads return 0.Writes have noeffect.
0 Reserved 0-1 Reads return 0or1depends onwhat iswritten inprivileged mode. The functionality ofthisbitis
unavailable inthisdevice.

<!-- Page 193 -->

www.ti.com System andPeripheral Control Registers
193 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.40 Bus Matrix Module Control Register 1(BMMCR1)
The BMMCR1 register, shown inFigure 2-47 anddescribed inTable 2-59,allows RAM andProgram
(Flash) memory addresses tobeswapped.
Figure 2-47. Bus Matrix Module Control Register 1(BMMCR) (offset =C4h)
31 16
Reserved
R-0
15 4 3 0
Reserved MEMSW
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-59. Bus Matrix Module Control Register 1(BMMCR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MEMSW Memory swap key.
Note: ACPU reset must beissued after thememory swap keyhasbeen changed forthe
memory swap tooccur. ACPU reset canbeinitiated bychanging thestate oftheCPU
RESET bitintheCPURSTCR register.
Ah Default memory-map:
Program memory (Flash) starts ataddress 0.eSRAM starts ataddress 8000000h.
5h Swapped memory-map:
eSRAM starts ataddress 0.Program memory (Flash) starts ataddress 8000000h.
Others The device memory-map isunchanged.

<!-- Page 194 -->

System andPeripheral Control Registers www.ti.com
194 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.41 CPU Reset Control Register (CPURSTCR)
The CPURSTCR register shown inFigure 2-48 anddescribed inTable 2-60 allows areset totheCortex-
R5F CPU tobegenerated.
NOTE: The register bitsinCPURSTCR aredesignated ashigh-integrity bitsandhave been
implemented with error-correcting logic such thateach bit,although read andwritten asa
single bit,isactually amulti-bit keywith error correction capability. Assuch, single-bit flips
within the"key"canbecorrected allowing protection ofthesystem asawhole. Anerror
detected issignaled totheESM module.
Figure 2-48. CPU Reset Control Register (CPURSTCR) (offset =CCh)
31 17 16
Reserved Reserved
R-0 R/WP-0
15 1 0
Reserved CPU RESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-60. CPU Reset Control Register (CPURSTGCR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 CPU RESET CPU RESET.
Only theCPU isreset whenever thisbitistoggled. There isnosystem reset.

<!-- Page 195 -->

www.ti.com System andPeripheral Control Registers
195 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.42 Clock Control Register (CLKCNTL)
The CLKCNTL register, shown inFigure 2-49 anddescribed inTable 2-61,controls peripheral reset and
theperipheral clock divide ratios.
NOTE: VCLK andVCLK2 clock ratio restrictions.
The VCLK2 frequency must always begreater than orequal totheVCLK frequency. The
VCLK2 frequency must beaninteger multiple oftheVCLK frequency.
Inaddition, theVCLK andVCLK2 clock ratios must notbechanged simultaneously. When
increasing thefrequency (decreasing thedivider), firstchange theVCLK2R field andthen
change theVCLKR field. When reducing thefrequency (increasing thedivider), firstchange
theVCLKR field andthen change theVCLK2R field.
You should doaread-back between thetwowrites. This assures thatthere areenough clock
cycles between thetwowrites.
Figure 2-49. Clock Control Register (CLKCNTL) (offset =D0h)
31 28 27 24 23 20 19 16
Reserved VCLK2R Reserved VCLKR
R-0 R/WP-1h R-0 R/WP-1h
15 9 8 7 0
Reserved PENA Reserved
R-0 R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-61. Clock Control Register (CLKCNTL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 VCLK2R VBUS clock2 ratio.
Note: TheVCLK2 frequency must always begreater than orequal totheVCLK frequency.
TheVCLK2 frequency must beaninteger multiple oftheVCLK frequency. Inaddition, the
VCLK andVCLK2 clock ratios must notbechanged simultaneously.
0 The VCLK2 speed isHCLK divided by1.
: :
Fh The VCLK2 speed isHCLK divided by16.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 VCLKR VBUS clock ratio.
Note: TheVCLK2 frequency must always begreater than orequal totheVCLK frequency.
TheVCLK2 frequency must beaninteger multiple oftheVCLK frequency. Inaddition, the
VCLK andVCLK2 clock ratios must notbechanged simultaneously.
0 The VCLK speed isHCLK divided by1.
: :
Fh The VCLK speed isHCLK divided by16.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 PENA Peripheral enable bit.The application must setthisbitbefore accessing anyperipheral.
0 The global peripheral/peripheral memory frames areinreset.
1 Allperipheral/peripheral memory frames areoutofreset.
7-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 196 -->

/c40 /c41 1 E C P D IVN V C L K o rO S C IE C L K/c43/c61
System andPeripheral Control Registers www.ti.com
196 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.43 ECP Control Register (ECPCNTL)
The ECP register, shown inFigure 2-50 anddescribed inTable 2-62,configures theECLK pinin
functional mode.
NOTE: ECLK Functional mode configuration.
The ECLK pinmust beplaced intoFunctional mode bysetting theECPCLKFUN bitto1in
theSYSPC1 register before aclock source willbevisible ontheECLK pin.
Figure 2-50. ECP Control Register (ECPCNTL) (offset =D4h)
31 25 24 23 22 18 17 16
Reserved ECPSSEL ECPCOS Reserved Reserved
R-0 R/W-0 R/W-0 R-0 R/W-0
15 0
ECPDIV
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-62. ECP Control Register (ECPCNTL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ECPSSEL This bitallows theselection between VCLK andOSCIN astheclock source forECLK.
Note: Other ECLK clock sources areavailable fordebug purposes byconfiguring the
CLKTEST register.
0 VCLK isselected astheECP clock source.
1 OSCIN isselected astheECP clock source.
23 ECPCOS ECP continue onsuspend.
Note: Suspend mode isentered while performing certain JTAG debugging operations.
0 ECLK output isdisabled insuspend mode. ECLK output willbeshut offandwillnotbeseen on
theI/Opinofthedevice.
1 ECLK output isnotdisabled insuspend mode. ECLK output willnotbeshut offandwillbeseen
ontheI/Opinofthedevice.
22-18 Reserved 0 Reads return 0.Writes have noeffect.
17-16 Reserved 0 Reads return 0or1depends onwhat iswritten. The functionality ofthisbitisunavailable inthis
device.
15-0 ECPDIV 0-FFFFh ECP divider value. The value ofECPDIV bitsdetermine theexternal clock (ECP clock) frequency
asaratio ofVBUS clock orOSCIN asshown intheformula:

<!-- Page 197 -->

www.ti.com System andPeripheral Control Registers
197 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.44 DEV Parity Control Register 1(DEVCR1)
This register isshown inFigure 2-51 anddescribed inTable 2-63.
Figure 2-51. DEV Parity Control Register 1(DEVCR1) (offset =DCh)
31 16
Reserved
R-0
15 4 3 0
Reserved DEVPARSEL
R-0 R/WP-Ah
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-63. DEV Parity Control Register 1(DEVCR1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 DEVPARSEL Device parity select bitkey.
Note: After anodd(DEVPARSEL =Ah)oreven (DEVPARSEL =5h)scheme isprogrammed
intotheDEVPARSEL register, anyonebitchange canbedetected andwillretain its
programmed scheme. More than onebitchanges inDEVPARSEL willcause adefault toodd
parity scheme.
5h The device parity iseven.
Ah The device parity isodd.
2.5.1.45 System Exception Control Register (SYSECR)
The SYSECR register, shown inFigure 2-52 anddescribed inTable 2-64,isused togenerate asoftware
reset.
NOTE: The register bitsinSYSECR aredesignated ashigh-integrity bitsandhave been
implemented with error-correcting logic such thateach bit,although read andwritten asa
single bit,isactually amulti-bit keywith error correction capability. Assuch, single-bit flips
within the"key"canbecorrected allowing protection ofthesystem asawhole. Anerror
detected issignaled totheESM module.
Figure 2-52. System Exception Control Register (SYSECR) (offset =E0h)
31 16
Reserved
R-0
15 14 13 0
RESET1 RESET0 Reserved
R/WP-0 R/WP-1 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-64. System Exception Control Register (SYSECR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-14 RESET[1-0] Software reset bits. Setting RESET1 orclearing RESET0 causes asystem software reset.
1h Noreset willoccur.
0,2h-3h Aglobal system reset willoccur.
13-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 198 -->

System andPeripheral Control Registers www.ti.com
198 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.46 System Exception Status Register (SYSESR)
The SYSESR register, shown inFigure 2-53 anddescribed inTable 2-65,shows thesource fordifferent
resets encountered. Previous reset source status bitsarenotautomatically cleared ifnew resets occur.
After reading thisregister, thesoftware should clear anyflags thataresetsothatthesource offuture
resets canbedetermined. Any bitinthisregister canbecleared bywriting a1tothebit.
Figure 2-53. System Exception Status Register (SYSESR) (offset =E4h)
31 16
Reserved
R-0
15 14 13 12 11 10 8
PORST OSCRST WDRST Reserved DBGRST Reserved
R/WC-X R/WC-X* R/WC-X* R-0 R/WC-X* R-0
7 6 5 4 3 2 0
ICSTRST Reserved CPURST SWRST EXTRST Reserved
R/WC-X* R/WC-X* R/WC-X* R/WC-X* R/WC-X* R-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; X=value isunchanged after reset; X*=0after PORST butunchanged after other
resets; -n=value after reset
Table 2-65. System Exception Status Register (SYSESR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 PORST Power-on reset. This bitissetwhen apower-on reset occurs, either internally asserted bytheVMON or
externally asserted bythenPORRST pin.
0 Nopower-on reset hasoccurred since thisbitwas lastcleared.
1 Areset was caused byapower-on reset. (This bitshould becleared after being read sothat
subsequent resets canbeproperly identified asnotbeing power-on resets.)
14 OSCRST Reset caused byanoscillator failure orPLL cycle slip. This bitissetwhen areset iscaused byan
oscillator failure orPLL slip. Write 1willclear thisbit.Write 0hasnoeffect.
Note: Theaction taken when anoscillator failure orPLL slipisdetected must configured inthe
PLLCTL1 register.
0 Noreset hasoccurred duetoanoscillator failure oraPLL cycle slip.
1 Areset was caused byanoscillator failure oraPLL cycle slip.
13 WDRST Watchdog reset flag. This bitissetwhen thelastreset was caused bythedigital watchdog (DWD).
Write 1willclear thisbit.Write 0hasnoeffect.
0 Noreset hasoccurred because oftheDWD.
1 Areset was caused bytheDWD.
12 Reserved 0 Reads return 0.Writes have noeffect.
11 DBGRST Debug reset flag. This bitissetwhen thelastreset was caused bythedebugger reset request. Write 1
willclear thisbit.Write 0hasnoeffect.
0 Noreset hasoccurred because ofthedebugger.
1 Areset was caused bythedebugger.
10-8 Reserved 0 Reads return 0.Writes have noeffect.
7 ICSTRST Interconnect reset flag. This bitissetwhen thelastCPU reset was caused bytheentering andexiting
ofinterconnect self-test check. While theinterconnect isunder self-test check, theCPU isalso held in
reset until theinterconnect self-test iscomplete.
0 NoCPUx reset hasoccurred because ofaninterconnect self-test check.
1 Areset hasoccurred totheCPUx because oftheinterconnect self-test check.
6 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 199 -->

www.ti.com System andPeripheral Control Registers
199 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-65. System Exception Status Register (SYSESR) Field Descriptions (continued)
Bit Field Value Description
5 CPURST CPU reset flag. This bitissetwhen theCPU isreset. Write 1willclear thisbit.Write 0hasnoeffect.
Note: ACPU reset canbeinitiated bytheCPU self-test controller (LBIST) orbytoggling theCPU
RESET bitfield inCPURSTCR register.
0 NoCPU reset hasoccurred.
1 ACPU reset occurred.
4 SWRST Software reset flag. This bitissetwhen asoftware system reset hasoccurred. Write 1willclear thisbit.
Write 0hasnoeffect.
Note: Asoftware system reset canbeinitiated bywriting totheRESET bitsintheSYSECR
register.
0 Nosoftware reset hasoccurred.
1 Asoftware reset occurred.
3 EXTRST External reset flag. This bitissetwhen areset iscaused bytheexternal reset pinnRST orbyanyreset
thatalso asserts thenRST pin(PORST, OSCRST, WDRST, WD2RST, andSWRST).
0 The external reset pinhasnotasserted areset.
1 Areset hasbeen caused bytheexternal reset pin.
2-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 200 -->

System andPeripheral Control Registers www.ti.com
200 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.47 System Test Abort Status Register (SYSTASR)
This register isshown inFigure 2-54 anddescribed inTable 2-66.
Figure 2-54. System Test Abort Status Register (SYSTASR) (offset =E8h)
31 16
Reserved
R-0
15 5 4 0
Reserved EFUSE_Abort
R-0 R/WPC-X/0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; C=Clear; -X=value isunchanged after reset; -n=
value after reset
Table 2-66. System Test Abort Status Register (SYSTASR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 EFUSE_Abort Test Abort status flag. These bitsaresetwhen testabort occurred:
0 Read: The lastoperation (ifany) completed successfully. This isalso thevalue thatthe
error/status register issettoafter reset.
1h Read: Controller times outbecause there isnolastrowsent from theFuseROM.
2h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine didnotfindenough FuseROM data
tofillthescan chain.
3h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine starts thescan chain with asignature
itexpects toseeafter thescan chain isfull.The autoload machine was able tofillthescan chain,
butthewrong signature was returned.
4h Read: The autoload machine was started, either through theSYS_INITZ signal from thesystem or
theJTAG data register. Ineither case, theautoload machine was notable ornotallowed to
complete itsoperation.
Others Read: Reserved.
1Fh Write: These bitsarecleared to0.

<!-- Page 201 -->

www.ti.com System andPeripheral Control Registers
201 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.48 Global Status Register (GLBSTAT)
The GLBSTAT register, shown inFigure 2-55 anddescribed inTable 2-67,indicates theFMzPLL (PLL1)
slipstatus andtheoscillator failstatus.
NOTE: PLL andOSC failbehavior
The device behavior after aPLL sliporanoscillator failure isconfigured inthePLLCTL1
register.
Figure 2-55. Global Status Register (GLBSTAT) (offset =ECh)
31 16
Reserved
R-0
15 10 9 8 7 1 0
Reserved FBSLIP RFSLIP Reserved OSCFAIL
R-0 R/W1C-n R/W1C-n R-0 R/W1C-n
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toClear; -n=value after reset
Table 2-67. Global Status Register (GLBSTAT) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9 FBSLIP PLL over cycle slipdetection. (cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: NoPLL over cycle sliphasbeen detected.
Write: The bitisunchanged.
1 Read: APLL over cycle sliphasbeen detected.
Write: The bitiscleared to0.
8 RFSLIP PLL under cycle slipdetection. (cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: NoPLL under cycle sliphasbeen detected.
Write: The bitisunchanged.
1 Read: APLL under cycle sliphasbeen detected.
Write: The bitiscleared to0.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 OSCFAIL Oscillator failflagbit.(cleared bynPORRST, maintains itsprevious value forallother resets).
0 Read: Nooscillator failure hasbeen detected.
Write: The bitisunchanged.
1 Read: Anoscillator failure hasbeen detected.
Write: The bitiscleared to0.

<!-- Page 202 -->

System andPeripheral Control Registers www.ti.com
202 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.49 Device Identification Register (DEVID)
The DEVID isaread-only register. Itcontains device-specific information thatishard-coded during device
manufacture. Fortheinitial silicon version, thedevice identification code value is8044 AD05h. This
register isshown inFigure 2-56 anddescribed inTable 2-68.
Figure 2-56. Device Identification Register (DEVID) (offset =F0h)
31 30 17 16
CP15 UNIQUE ID TECH
R-K R-K R-K
15 13 12 11 10 9 8
TECH I/OVOLTAGE PERIPHERAL
PARITYFLASH ECC RAM ECC
R-K R-K R-K R-K R-K
7 3 2 0
VERSION PLATFORM ID
R-K R-K
LEGEND: R=Read only; K=Constant value; -n=value after reset
Table 2-68. Device Identification Register (DEVID) Field Descriptions
Bit Field Value Description
31 CP15 CP15 CPU. This bitindicates whether theCPU hasacoprocessor 15(CP15).
0 The CPU hasnoCP15 present.
1 The CPU hasaCP15 present. The CPU IDcanberead using theCP15 C0,C0,0 register.
30-17 UNIQUE ID 0-3FFFh Device ID.The device IDisunique bydevice configuration.
16-13 TECH These bitsdefine theprocess technology bywhich thedevice was manufactured.
0 Device manufactured intheC05 process technology.
1h Device manufactured intheF05 process technology.
2h Device manufactured intheC035 process technology.
3h Device manufactured intheF035 process technology.
4h Device manufactured intheC021 process technology.
5h Device manufactured intheF021 process technology.
6h-7h Reserved
12 I/OVOLTAGE Input/output voltage. This bitdefines theI/Ovoltage ofthedevice.
0 The I/Ovoltage is3.3V.
1 The I/Ovoltage is5V.
11 PERIPHERAL
PARITYPeripheral parity. This bitindicates whether ornotperipheral memory parity ispresent.
0 The peripheral memories have noparity.
1 The peripheral memories have parity.
10-9 FLASH ECC These bitsindicate which parity ispresent fortheprogram memory.
0 Nomemory protection ispresent.
1h The program memory (Flash) hassingle-bit parity.
2h The program memory (Flash) hasECC.
3h This combination isreserved.
8 RAM ECC RAM ECC. This bitindicates whether ornotRAM memory ECC ispresent.
0 The RAM memories donothave ECC.
1 The RAM memories have ECC.
7-3 VERSION 0-1Fh Version. These bitsprovide therevision ofthedevice.
2-0 PLATFORM ID 5h The device ispart oftheTMS570Px family. The TMS570Px IDisalways 5h.

<!-- Page 203 -->

www.ti.com System andPeripheral Control Registers
203 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.50 Software Interrupt Vector Register (SSIVEC)
The SSIVEC register, shown inFigure 2-57 anddescribed inTable 2-69,contains information about
software interrupts.
Figure 2-57. Software Interrupt Vector Register (SSIVEC) (offset =F4h)
31 16
Reserved
R-0
15 8 7 0
SSIDATA SSIVECT
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 2-69. Software Interrupt Vector Register (SSIVEC) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 SSIDATA 0-FFh System software interrupt data key. These bitscontain thedata keyvalue ofthesource forthe
system software interrupt, which isindicated bythevector intheSSIVEC[7-0] field.
7-0 SSIVECT These bitscontain thesource forthesystem software interrupt.
Note: Aread from theSSIVECT bitsclears thecorresponding SSI_FLAG[4-1] bitinthe
SSIF register, corresponding tothesource vector ofthesystem software interrupt.
Note: TheSSIR[4-1] interrupt hasthefollowing priority order:
SSIR1 hasthehighest priority.
SSIR4 hasthelowest priority.
0 Nosoftware interrupt ispending.
1h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR1 register.
2h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR2 register.
3h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR3 register.
4h Asoftware interrupt hasbeen generated bywriting thecorrect keyvalue toThe SSIR4 register.
5h-FFh Reserved

<!-- Page 204 -->

System andPeripheral Control Registers www.ti.com
204 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.1.51 System Software Interrupt Flag Register (SSIF)
The SSIF register, shown inFigure 2-58 anddescribed inTable 2-70,contains software interrupt flag
status information.
Figure 2-58. System Software Interrupt Flag Register (SSIF) (offset =F8h)
31 16
Reserved
R-0
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved SSI_FLAG4 SSI_FLAG3 SSI_FLAG2 SSI_FLAG1
R-0 R/WC-0 R/WC-0 R/WC-0 R/WC-0
LEGEND: R/W =Read/Write; R=Read only; C=Clear; -n=value after reset
Table 2-70. System Software Interrupt Flag Register (SSIF) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 SSI_FLAG[4-1] System software interrupt flag[4-1]. This flagissetwhen thecorrect SSKEY iswritten tothe
SSIR register[4-1].
Note: Aread from theSSIVEC register clears thecorresponding SSI_FLAG[4-1] bitinthe
SSIF, corresponding tothesource vector ofthesystem software interrupt.
0 Read: NoIRQ/FIQ interrupt was generated since thebitwas lastcleared.
Write: The bitisunchanged.
1 Read: AnIRQ/FIQ interrupt was generated.
Write: The bitiscleared to0.