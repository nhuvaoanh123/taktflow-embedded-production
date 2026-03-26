# Architecture - Secondary System Control Registers (SYS2)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 205-216

---


<!-- Page 205 -->

www.ti.com System andPeripheral Control Registers
205 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2 Secondary System Control Registers (SYS2)
This section describes thesecondary frame ofsystem registers. The start address ofthesecondary
system module frame isFFFF E100h. The registers support 8-,16-, and32-bit writes. The offset isrelative
tothesystem module frame start address.
Table 2-71 contains alistofthesecondary system control registers.
NOTE: Alladditional registers inthesecondary system frame arereserved.
Table 2-71. Secondary System Control Registers
Offset Acronym Register Description Section
00h PLLCTL3 PLL Control Register 3 Section 2.5.2.1
08h STCLKDIV CPU Logic BIST Clock Divider Section 2.5.2.2
24h ECPCNTL ECP Control Register. The ECPCNTL register hasthemirrored
location atthisaddress.Section 2.5.1.43
28h ECPCNTL1 ECP Control Register 1. Section 2.5.2.3
3Ch CLK2CNTRL Clock 2Control Register Section 2.5.2.4
40h VCLKACON1 Peripheral Asynchronous Clock Configuration 1Register Section 2.5.2.5
54h HCLKCNTL HCLK Control Register Section 2.5.2.6
70h CLKSLIP Clock SlipControl Register Section 2.5.2.7
78h IP1ECCERREN IPECC Error Enable Register Section 2.5.2.8
ECh EFC_CTLREG EFUSE Controller Control Register Section 2.5.2.9
F0h DIEIDL_REG0 DieIdentification Register Lower Word Section 2.5.2.10
F4h DIEIDH_REG1 DieIdentification Register Upper Word Section 2.5.2.11
F8h DIEIDL_REG2 DieIdentification Register Lower Word Section 2.5.2.12
FCh DIEIDH_REG3 DieIdentification Register Upper Word Section 2.5.2.13

<!-- Page 206 -->

System andPeripheral Control Registers www.ti.com
206 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.1 PLL Control Register 3(PLLCTL3)
The PLLCTL3 register isshown inFigure 2-59 anddescribed inTable 2-72;controls thesettings ofPLL2
(Clock Source 6-FPLL).
Figure 2-59. PLL Control Register 3(PLLCTL3) (offset =00h)
31 29 28 24 23 22 21 16
ODPLL2 PLLDIV2 Reserved REFCLKDIV2
R/WP-0 R/WP-4h R-0 R/WP-0
15 0
PLLMUL2
R/WP-1300h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-72. PLL Control Register 3(PLLCTL3) Field Descriptions
Bit Field Value Description
31-29 ODPLL2 Internal PLL Output Divider
OD2 =ODPLL2 +1,ranges from 1to8.
fpost_ODCLK2 =foutput_CLK2 /OD2
Note: PLL output clock isgated offifODPLL2 ischanged while thePLL#2 isactive.
0 fpost_ODCLK2 =foutput_CLK2 /1
1h fpost_ODCLK2 =foutput_CLK2 /2
: :
7h fpost_ODCLK2 =foutput_CLK2 /8
28-24 PLLDIV2 PLL2 Output Clock Divider
R2=PLLDIV2 +1,ranges from 1to32.
fPLL2 CLK=fpost_ODCLK2 /R2
0 fPLL2 CLK=fpost_ODCLK2 /1
1h fPLL2 CLK=fpost_ODCLK2 /2
: :
1Fh fPLL2 CLK=fpost_ODCLK2 /32
23-22 Reserved 0 Value hasnoeffect onPLL operation.
21-16 REFCLKDIV2 Reference Clock Divider
NR2 =REFCLKDIV2 +1,ranges from 1to64.
fINTCLK2 =fOSCIN /NR2
Note: This value should notbechanged while thePLL2 isactive.
0 fINTCLK2 =fOSCIN /1
1h fINTCLK2 =fOSCIN /2
: :
3Fh fINTCLK2 =fOSCIN /64
15-0 PLLMUL2 PLL2 Multiplication Factor
NF2 =(PLLMUL2 /256) +1,valid multiplication factors arefrom 1to256.
fVCOCLK2 =fINTCLK2 xNF2
User andprivileged mode (read):
Privileged mode (write):
100h fVCOCLK2 =fINTCLK2 x1
: :
5B00h fVCOCLK2 =fINTCLK2 x92
5C00h fVCOCLK2 =fINTCLK2 x93
: :
FF00h fVCOCLK2 =fINTCLK2 x256

<!-- Page 207 -->

www.ti.com System andPeripheral Control Registers
207 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.2 CPU Logic Bist Clock Divider (STCLKDIV)
This register isshown inFigure 2-60 anddescribed inTable 2-73.
Figure 2-60. CPU Logic BIST Clock Prescaler (STCLKDIV) (offset =08h)
31 27 26 24 23 16
Reserved CLKDIV Reserved
R-0 R/WP-0 R-0
15 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-73. CPU Logic BIST Clock Prescaler (STCLKDIV) Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 CLKDIV 0 Clock divider/prescaler forCPU clock during logic BIST
23-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 208 -->

/c40 /c41 1 E C P D IVN V C L K o rO S C IE C L K/c43/c61
System andPeripheral Control Registers www.ti.com
208 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.3 ECP Control Register 1(ECPCNTL1)
The ECP register, shown inFigure 2-61 anddescribed inTable 2-74,configures theECLK2 pinin
functional mode.
NOTE: ECLK2 Functional mode configuration.
The ECLK2 pinmust beplaced intoFunctional mode bysetting theECPCLKFUN bitto1in
theSYSPC1 register before aclock source willbevisible ontheECLK pin.
Figure 2-61. ECP Control Register 1(ECPCNTL1) (offset =28h)
31 28 27 25 24 23 22 16
ECP_KEY Reserved ECPSSEL ECPCOS Reserved
R/WP-5h R-0 R/W-0 R/W-0 R-0
15 0
ECPDIV
R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 2-74. ECP Control Register 1(ECPCNTL1) Field Descriptions
Bit Field Value Description
31-28 ECP_KEY Enable ECP clock logic forECLK2.
Ah Clock functionality ofECP clock isenabled.
Others Clock functionality ofECP clock isdisabled.
27-25 Reserved 0 Reads return 0.Writes have noeffect.
24 ECPSSEL This bitallows theselection between VCLK andOSCIN astheclock source forECLK2.
0 VCLK isselected astheECP clock source.
1 OSCIN isselected astheECP clock source.
23 ECPCOS ECP continue onsuspend.
Note: Suspend mode isentered while performing certain JTAG debugging operations.
0 ECLK output isdisabled insuspend mode. ECLK output willbeshut offandwillnotbeseen on
theI/Opinofthedevice.
1 ECLK output isnotdisabled insuspend mode. ECLK output willnotbeshut offandwillbeseen
ontheI/Opinofthedevice.
22-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 ECPDIV 0-FFFFh ECP divider value. The value ofECPDIV bitsdetermine theexternal clock (ECP clock) frequency
asaratio ofVBUS clock orOSCIN asshown intheformula:

<!-- Page 209 -->

www.ti.com System andPeripheral Control Registers
209 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.4 Clock 2Control Register (CLK2CNTRL)
This register isshown inFigure 2-62 anddescribed inTable 2-75.
Figure 2-62. Clock 2Control Register (CLK2CNTRL) (offset =3Ch)
31 16
Reserved
R-0
15 12 11 8 7 4 3 0
Reserved Reserved Reserved VCLK3R
R-0 R/WP-1h R-0 R/WP-1h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-75. Clock 2Control Register (CLK2CNTRL) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 Reserved Reads return value andwrites allowed inprivilege mode.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 VCLK3R VBUS clock3 ratio.
0 The ratio isHCLK divide by1.
: :
Fh The ratio isHCLK divided by16.

<!-- Page 210 -->

System andPeripheral Control Registers www.ti.com
210 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.5 Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1)
This register isshown inFigure 2-63 anddescribed inTable 2-76.
Figure 2-63. Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1) [offset =40h]
31 27 26 24
Reserved VCLKA4R
R-0 R/WP-1h
23 21 20 19 16
Reserved VCLKA4_DIV_
CDDISVCLKA4S
R-0 R/WP-0 R/WP-9h
15 11 10 8 7 5 4 0
Reserved Reserved Reserved Reserved
R-0 R/WP-1h R-0 R/WP-9h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-76. Peripheral Asynchronous Clock Configuration 1Register (VCLKACON1)
Field Descriptions
Bit Field Value Description
31-27 Reserved 0 Reads return 0.Writes have noeffect.
26-24 VCLKA4R Clock divider fortheVCLKA4 source. Output willbepresent onVCLKA4_DIVR.
VCLKA4 domain willbeenabled bywriting totheCDDIS register andVCLKA4_DIV_CDDIS bit.
Itcaninferred thatVCLKA4_DIV clock isdisabled when VCLKA4 clock isdisabled.
0 The ratio isVCLKA4 divided by1.
: :
7h The ratio isVCLKA4 divided by8.
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20 VCLKA4_DIV_CDDIS Disable theVCLKA4 divider output.
VCLKA4 domain willbeenabled bywriting totheCDDIS register.
0 Enable theprescaled VCLKA4 clock onVCLKA4_DIVR.
1 Disable theprescaled VCLKA4 clock onVCLKA4_DIVR.
19-16 VCLKA4S Peripheral asynchronous clock4 source.
0 Clock source0 isthesource forperipheral asynchronous clock4.
1h Clock source1 isthesource forperipheral asynchronous clock4.
2h Clock source2 isthesource forperipheral asynchronous clock4.
3h Clock source3 isthesource forperipheral asynchronous clock4.
4h Clock source4 isthesource forperipheral asynchronous clock4.
5h Clock source5 isthesource forperipheral asynchronous clock4.
6h Clock source6 isthesource forperipheral asynchronous clock4.
7h Clock source7 isthesource forperipheral asynchronous clock4.
8h-Fh VCLK oradivided VCLK isthesource forperipheral asynchronous clock4. See thedevice-
specific data manual fordetails.
15-0 Reserved 109h Reserved
NOTE: Non-implemented clock sources should notbeenabled orused. Alistoftheavailable clock
sources isshown intheTable 2-29.

<!-- Page 211 -->

www.ti.com System andPeripheral Control Registers
211 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.6 HCLK Control Register (HCLKCNTL)
This register isshown inFigure 2-64 anddescribed inTable 2-77.
Figure 2-64. HCLK Control Register (HCLKCNTL) (offset =54h)
31 16
Reserved
R-0
15 2 1 0
Reserved HCLKR
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-77. HCLK Control Register (HCLKCNTL) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 HCLKR HCLK divider value. The value ofHCLKR bitsdetermine theHCLK frequency asaratio ofGCLK1.
0 HCLK isequal toGCLK1 divide by1.
1h HCLK isequal toGCLK1 divide by2.
2h HCLK isequal toGCLK1 divide by3.
3h HCLK isequal toGCLK1 divide by4.

<!-- Page 212 -->

System andPeripheral Control Registers www.ti.com
212 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.7 Clock SlipControl Register (CLKSLIP)
This register isshown inFigure 2-65 anddescribed inTable 2-78.Forinformation onfiltering theFBSLIP,
seeSection 2.5.1.34 .
Figure 2-65. Clock SlipControl Register (CLKSLIP) (offset =70h)
31 16
Reserved
R-0
15 14 13 8 7 4 3 0
Reserved PLL1_RFSLIP_FILTER_COUNT Reserved PLL1_RFSLIP_FILTER_KEY
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-78. Clock SlipControl Register (CLKSLIP) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 PLL1_RFSLIP_FILTER_COUNT PLL RFSLIP down counter programmed value. Count ison10M clock.
Onreset, counter value is0.Counter must beprogrammed toanon-zero value
andenabled forthefiltering tobeenabled.
0 Filtering isdisabled.
1h Filtering isenabled. Every slipisrecognized.
2h Filtering isenabled. The slipmust beatleast 2HFLPO cycles wide inorder to
berecognized asaslip.
: :
3Fh Filtering isenabled. The RFSLIP must beatleast 63HFLPO cycles wide in
order toberecognized asaslip.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 PLL1_RFSLIP_FILTER_KEY Enable thePLL RFSLIP filtering.
5h Onreset, thePLL RFSLIP filter isdisabled andthePLL RFSLIP passes through.
Fh This isanunsupported value. You should avoid writing thisvalue tothisbitfield.
Others PLL RFSLIP filtering isenabled. Recommended toprogram Ahinthisbitfield.
Enabling ofthePLL RFSLIP occurs when theKEY isprogrammed andanon-
zero value ispresent intheCOUNT field.

<!-- Page 213 -->

www.ti.com System andPeripheral Control Registers
213 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.8 IPECC Error Enable Register (IP1ECCERREN)
This register isshown inFigure 2-66 anddescribed inTable 2-79.
Figure 2-66. IPECC Error Enable Register (IP1ECCERREN) (offset =78h)
31 28 27 24 23 20 19 16
Reserved Reserved Reserved Reserved
R-0 R/WP-5h R/WP-5h
15 12 11 8 7 4 3 0
Reserved IP2_ECC_KEY Reserved IP1_ECC_KEY
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-79. Clock SlipRegister (CLKSLIP) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reads return 0.Writes have noeffect.
27-24 Reserved 0-Fh Reads return 0or1anddepends onwhat iswritten inprivileged mode. The
functionality ofthisbitisunavailable inthisdevice.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 Reserved 0-Fh Reads return 0or1anddepends onwhat iswritten inprivileged mode. The
functionality ofthisbitisunavailable inthisdevice.
15-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 IP2_ECC_KEY ECC Error Enable Key forPS_SCR_M master. There isanECC Evaluation block
inside theCPU Interconnect Subsystem responsible forECC correction anddetection
onthedata path fortransactions initiated bythePS_SCR_M master. IfanECC error
(either single-bit ordouble-bit error) isdetected, then thecorresponding error signal is
asserted ifECC enable keywritten toIP2_ECC_KEY isAh.
Others Disable ECC error generation forECC errors detected onPS_SCR_M master bythe
CPU Interconnect Subsystem.
Ah Enable ECC error generation forECC errors detected onPS_SCR_M master bythe
CPU Interconnect Subsystem.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 IP1_ECC_KEY ECC Error Enable Key forDMA Port Amaster. There isanECC Evaluation block
inside theCPU Interconnect Subsystem responsible forECC correction anddetection
onthedata path fortransactions initiated bytheDMA Port Amaster. IfanECC error
(either single-bit ordouble-bit error) isdetected, then thecorresponding error signal is
asserted ifECC enable keywritten toIP1_ECC_KEY isAh.
Others Disable ECC error generation forECC errors detected onDMA Port Amaster bythe
CPU Interconnect Subsystem.
Ah Enable ECC error generation forECC errors detected onDMA Port Amaster bythe
CPU Interconnect Subsystem.

<!-- Page 214 -->

System andPeripheral Control Registers www.ti.com
214 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.9 EFUSE Controller Control Register (EFC_CTLREG)
This register isshown inFigure 2-67 anddescribed inTable 2-80.
Figure 2-67. EFUSE Controller Control Register (EFC_CTLREG) (offset =ECh)
31 16
Reserved
R-0
15 4 3 0
Reserved EFC_INSTR_WEN
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-80. EFUSE Controller Control Register (EFC_CTLREG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EFC_INSTR_WEN Enable user write of4EFUSE controller instructions.
SYS module generates theenable signal thatwillbetiedtoOCP_FROM_WRITE_DISABLE
onefuse controller port.
Ah Writing ofinstructions (Program, ProgramCRA, RunAutoload, andLoadFuseScanchain) to
EFC isallowed.
Others Writing ofinstructions (Program, ProgramCRA, RunAutoload, andLoadFuseScanchain) in
EFC registers isblocked.
2.5.2.10 DieIdentification Register Lower Word (DIEIDL_REG0)
The DIEIDL_REG0 register isaduplicate oftheDIEIDL register, seeSection 2.5.1.28 .The DIEIDL_REG0
register, shown inFigure 2-68 anddescribed inTable 2-81,contains information about thediewafer
number, andX,Ywafer coordinates.
Figure 2-68. DieIdentification Register, Lower Word (DIEIDL_REG0) [offset =F0h]
31 24 23 16
WAFER # YWAFER COORDINATE
R-D R-D
15 12 11 0
YWAFER COORDINATE XWAFER COORDINATE
R-D R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-81. DieIdentification Register, Lower Word (DIEIDL_REG0) Field Descriptions
Bit Field Description
31-24 WAFER # These read-only bitscontain thewafer number ofthedevice.
23-12 YWAFER COORDINATE These read-only bitscontain theYwafer coordinate ofthedevice.
11-0 XWAFER COORDINATE These read-only bitscontain theXwafer coordinate ofthedevice.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.

<!-- Page 215 -->

www.ti.com System andPeripheral Control Registers
215 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.11 DieIdentification Register Upper Word (DIEIDH_REG1)
The DIEIDH_REG1 register isaduplicate oftheDIEIDH register, seeSection 2.5.1.29 .The
DIEIDH_REG1 register, shown inFigure 2-69 anddescribed inTable 2-82,contains information about the
dielotnumber.
Figure 2-69. DieIdentification Register, Upper Word (DIEIDH_REG1) [offset =F4h]
31 24 23 16
Reserved LOT #
R-0 R-D
15 0
LOT #
R-D
LEGEND: R=Read only; D=value isdevice specific; -n=value after reset
Table 2-82. DieIdentification Register, Upper Word (DIEIDH_REG1) Field Descriptions
Bit Field Description
31-24 Reserved Reserved forTIuse. Writes have noeffect.
23-0 LOT # This read-only register contains thedevice lotnumber.
NOTE: DieIdentification Information
The dieidentification information willvary from unittounit. This information isprogrammed
byTIaspart oftheinitial device testprocedure.
2.5.2.12 DieIdentification Register Lower Word (DIEIDL_REG2)
This register isshown inFigure 2-70 anddescribed inTable 2-83.
Figure 2-70. DieIdentification Register, Lower Word (DIEIDL_REG2) [offset =F8h]
31 0
DIEIDL2
R-X
LEGEND: R=Read only; X=value isunchanged after reset; -n=value after reset
Table 2-83. DieIdentification Register, Lower Word (DIEIDL_REG2) Field Descriptions
Bit Field Value Description
31-0 DIEIDL2(95-64) 0-FFFF FFFFh This read-only register contains thelower word (95:64) ofthedieIDinformation. The
contents ofthisregister isreserved.

<!-- Page 216 -->

System andPeripheral Control Registers www.ti.com
216 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.2.13 DieIdentification Register Upper Word (DIEIDH_REG3)
This register isshown inFigure 2-71 anddescribed inTable 2-84.
Figure 2-71. DieIdentification Register, Upper Word (DIEIDH_REG3) [offset =FCh]
31 0
DIEIDH2
R-X
LEGEND: R=Read only; X=value isunchanged after reset ;-n=value after reset
Table 2-84. DieIdentification Register, Upper Word (DIEIDH_REG3) Field Descriptions
Bit Field Value Description
31-0 DIEIDH2(127-96) 0-FFFF FFFFh This read-only register contains theupper word (127:97) ofthedieIDinformation. The
contents ofthisregister isreserved.