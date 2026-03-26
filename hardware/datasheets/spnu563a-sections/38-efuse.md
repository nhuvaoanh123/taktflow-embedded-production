# eFuse Controller

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 2187-2195

---


<!-- Page 2187 -->

2187 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse ControllerChapter 38
SPNU563A -March 2018
eFuse Controller
This chapter describes theeFuse controller.
Topic ........................................................................................................................... Page
38.1 Overview ........................................................................................................ 2188
38.2 Introduction ................................................................................................... 2188
38.3 eFuse Controller Testing ................................................................................. 2188
38.4 eFuse Controller Registers .............................................................................. 2191

<!-- Page 2188 -->

Overview www.ti.com
2188 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.1 Overview
Electrically programmable fuses (eFuses) areused toconfigure thedevice after deassertion ofPORRST.
The eFuse values areread andloaded intointernal registers aspart ofthepower-on-reset sequence. The
eFuse values areprotected with single biterror correction, double biterror detection (SECDED) codes.
These fuses areprogrammed during theinitial factory testofthedevice. The eFuse controller isdesigned
sothatthestate oftheeFuses cannot bechanged once thedevice ispackaged.
38.2 Introduction
The eFuse controller automatically reads thevalues oftheeFuses andshifts them intoregisters during the
power-on reset sequence. Noaction isrequired from theapplication code. However, inasafety critical
application, theuser code should check toseeifacorrectable oranuncorrectable error was detected
during thereset sequence andthen preform aself-test ontheeFuse controller ECC logic.
38.3 eFuse Controller Testing
38.3.1 eFuse Controller Connections toESM
There arethree connections from theeFuse controller totheError Signaling Module (ESM). Ifan
uncorrectable error occurs during theloading oftheeFuse values after reset, agroup three, channel one
error andagroup onechannel 40error aresent totheESM. The group three error willcause theERROR
pintogolow. Ifduring theeFuse loading acorrectable error occurs, only agroup onechannel 40error is
sent totheESM. Ifanerror occurs during theeFuse controller selftest, then agroup onechannel 41error
andagroup onechannel 40error aresent totheESM. After reset, bydefault, thegroup oneerrors donot
affect theERROR pin.Ifthesoftware enables theappropriate bitintheappropriate ESM Influence Error
PinSet/Status Register (ESMIEPSRn) while thegroup oneerror isset,theERROR pinwillgolow.
Table 38-1. ESM Signals SetbyeFuse Controller
ESM SignalUncorrected Load
FailureCorrectable Load
ErrorSelfTest
eFuse SelfTest eFuse stuck at0Test
Version a:
with Error pinVersion b:
without Error pin
Group 3Channel 1 X X
Group 1Channel 40 X X X
Group 1Channel 41 X X X
38.3.2 Checking foreFuse Errors After Power Up
Forsafety critical systems, itisrequired thatyoucheck thestatus oftheeFuse controller after adevice
reset. Asuggested flow chart forchecking theeFuse controller after device reset isshown inFigure 38-1 .
Failures during theeFuse selftestcanbegrouped intothree levels ofseverity. Depending onthesafety
critical application, theerror handling foreach error type may bedifferent.
38.3.2.1 Class 1Error
Aclass 1error oftheeFuse controller means thatthere was afailure during theautoload sequence. The
values read from theeFuses cannot berelied on.Alldevice operation issuspect. Aclass 1error is
indicated byasignal togroup 3channel 1oftheESM. This willcause theERROR pintogoactive low.
38.3.2.2 Class 2Errors
Aclass 2error isanindication thatthesafety checks oftheeFuse controller didnotwork. These arealso
serious errors because youcannolonger guarantee thatamore severe error didnotoccur.

<!-- Page 2189 -->

www.ti.com eFuse Controller Testing
2189 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.3.2.3 Class 3Error
Aclass 3error indicates thatthere was asingle bitfailure reading theeFuses thatwas corrected byECC
bits. Proper operation isstilllikely, butthesystem isnow atahigher riskforafuture non-correctable error.
When acorrectable error occurs, ESM group 1,channel 40willbeset.Inthesuggested flow chart shown
inFigure 38-1 below, thesingle biterror isdetermined bydirectly reading theeFuse error status register,
andnotdepending ontheintegrity oftheconnections between theeFuse controller andtheESM.
38.3.2.4 Stuck atZero Test
The purpose ofthestuck atzero testistoverify thattheeFuse controller could signal theESM ifan
autoload error didoccur. Itbasically verifies thepath through theeFuse controller andtotheESM. This is
done bywriting aspecial instruction totheeFuse controller boundary register, then verifying thatthe
proper bitsaresetintheeFuse controller pins register. Upon successful completion ofthistestESM
group 1channel 41andESM group 3channel 1willbeset.This willforce theERROR pinlow.
*Version A
-Write boundary register (address 0xFFF8C01C) with 0x003FC000 tosettheerror signals.
-Read pins register (address 0xFFF8C02C) andverify thatbits14,12,11and10areset.
-Write boundary register (address 0xFFF8C01C) with 0x003C0000, toclear theerror signals.
-Verify thatESM group 1channel 41andgroup 3channel 1areset,then clear them.
Ifthesystem cannot support atestwhich causes theERROR pintogolow, then thestuck atzero testcan
bemodified asfollows:
*Version B
-Write boundary register (address 0xFFF8C01C) with 0x003BC000.
-Read pins register (address 0xFFF8C02C) andverify thatbits14,12,and11areset.
-Write boundary register (address 0xFFF8C01C) with 0x003C0000, toclear theerror signals.
-Verify thatESM group 1channel 41isset,then clear it.
This alternate method provides less testcoverage because thepath from theuncorrectable error signal
from theeFuse controller totheESM isnotspecifically tested. However, even ifthispath isbroken,
reading thefiveeFuse error status bitswillindicate thatanerror occurred.
38.3.2.5 eFuse ECC Logic SelfTest
The eFuse controller selftestperforms extensive validation oftheECC logic intheeFuse controller. This
testshould only beperformed once forevery device PORRST cycle. Perform theselftestbyfollowing
these steps:
*Write 0x00000258 totheselftestcycles register (EFCSTCY) ataddress 0xFFF8C048.
*Write 0x5362F97F totheselftestsignature register (EFCSTSIG) ataddress 0xFFF8C04C.
*Write 0x0000200F totheboundary register ataddress 0xFFF8C01C. This triggers theselftest. The
testtakes 610VCLK cycles tocomplete. The application canpollbit15ofthepins register ataddress
0xFFF8C02C towait forthetesttocomplete.
*Check ESM group 1channels 40and41foranyerrors, neither should beset.
*Verify thatbits4to0oftheeFuse Error Status register ataddress 0xFFF8C03C arezero.

<!-- Page 2190 -->

eFuse□Controller
Test
Is□ESM□group□3
channel□1□set?
Stuck□at□zero
test□pass?
Did□self□test
pass?
PASSY
Y
Y
YN
N
N
NClass□1
error
routine
Y
NRun□stuck
at□zero□test
Run□eFuse
self□test
Class□2
error
routineY
NTest□bits□4-0□of
eFuse□Error
status□register
Are□all□5□bits
zero?
Are□the□5□bits□=
0x15?
Class□3
error
routineRun□eFuse
self□test
Did□self□test
pass?
eFuse Controller Testing www.ti.com
2190 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse ControllerFigure 38-1. eFuse SelfTest Flow Chart

<!-- Page 2191 -->

www.ti.com eFuse Controller Registers
2191 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.4 eFuse Controller Registers
Allregisters intheeFuse Controller module are32-bit, word-aligned; 8-bit, 16-bit and32-bit accesses are
allowed. Table 38-2 provides aquick reference toeach ofthese registers. Specific bitdescriptions are
discussed inthefollowing subsections. The base address forthecontrol registers isFFF8 C000h.
Table 38-2. eFuse Controller Registers
Offset Acronym Register Description Section
1Ch EFCBOUND EFC Boundary Control Register Section 38.4.1
2Ch EFCPINS EFC Pins Register Section 38.4.2
3Ch EFCERRSTAT EFC Error Status Register Section 38.4.3
48h EFCSTCY EFC Self Test Cycles Register Section 38.4.4
4Ch EFCSTSIG EFC Self Test Signature Register Section 38.4.5
38.4.1 EFC Boundary Control Register (EFCBOUND)
Figure 38-2 andTable 38-3 describe theEFCBOUND register. The eFuse Boundary Control Register is
used totesttheconnections between theeFuse controller andtheESM module. The eFuse Boundary
Control Register isalso used toinitiate aneFuse controller ECC self-test.
Figure 38-2. EFC Boundary Control Register (EFCBOUND) [offset =1Ch]
31 24
Reserved
R-0
23 22 21 20 19 18 17 16
Reserved EFC Self Test
ErrorEFC Single Bit
ErrorEFC Instruction
ErrorEFC Autoload
ErrorSelf Test
Error OESingle Bit
Error OE
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
15 14 13 12 8
Instruction
Error OEAutoload
Error OEEFC ECC Selftest
EnableReserved
R/W-0 R/W-0 R/W-0 R-0
7 4 3 0
Reserved Input Enable
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after power-on reset (nPORRST)
Table 38-3. EFC Boundary Register (EFCBOUND) Field Descriptions
Bit Field Value Description
31-22 Reserved 0 Read returns 0.Writes have noeffect.
21 EFC Self Test Error This bitdrives theselftesterror signal when bit17(Self Test Error OE) ishigh. This signal
isattached toESM error Group 1,Channel 41.
0 Drives theselftesterror signal low, ifSelf Test OEishigh.
1 Drives theselftesterror signal high, ifSelf Test OEishigh.
20 EFC Single BitError This bitdrives thesingle biterror signal when bit16(Single bitError OE) ishigh. This signal
isattached toESM error Group 1,Channel 40.
0 Drives theselftesterror signal low, ifSingle BitError OEishigh.
1 Drives theselftesterror signal high, ifSingle BitError OEishigh.

<!-- Page 2192 -->

eFuse Controller Registers www.ti.com
2192 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse ControllerTable 38-3. EFC Boundary Register (EFCBOUND) Field Descriptions (continued)
Bit Field Value Description
19 EFC Instruction Error This bitdrives theinstruction error signal when bit15(Instruction Error OE) ishigh. This
signal isused todenote anerror occurred during e-fuse programming. This signal isnot
attached totheESM.
0 Drives theInstruction Error signal low, ifInstruction Error OEishigh.
1 Drives theInstruction Error signal high, ifInstruction Error OEishigh.
18 EFC Autoload Error This bitdrives theAutoload Error signal when bit14(Autoload Error OE) ishigh. This signal
isattached toESM error Group 3,Channel 1.
0 Drives theAutoload Error signal low, ifAutoload Error OEishigh.
1 Drives theAutoload Error signal high, ifAutoload Error OEishigh.
17 Self Test Error OE The Self Test Error Output Enable bitdetermines iftheEFC Self Test signal comes from the
eFuse controller orfrom bit21oftheboundary register.
0 EFC Self Test Error comes from eFuse controller.
1 EFC Self Test Error comes from theboundary register.
16 Single BitError OE The single biterror output enable signal determines iftheEFC Single BitError signal comes
from theeFuse controller orfrom bit20oftheboundary register.
0 EFC Single BitError comes from eFuse controller.
1 EFC Single BitError comes from theboundary register.
15 Instruction Error OE The instruction error output enable signal determines iftheEFC Instruction Error signal
comes from theeFuse controller orfrom bit19oftheboundary register.
0 EFC Instruction Error comes from eFuse controller.
1 EFC Instruction Error comes from theboundary register.
14 Autoload Error OE The autoload error output enable signal determines iftheEFC Autoload Error signal comes
from theeFuse controller orfrom bit18oftheboundary register.
0 EFC Autoload Error comes from eFuse controller.
1 EFC Autoload Error comes from theboundary register.
13 EFC ECC Selftest
EnableThe eFuse Controller ECC Selftest Enable bitstarts theselftest oftheECC logic ifthefour
input enable bits(EFCBOUND[3:0) areall1s.
0 Noaction
1 Start ECC selftest ifEFCBOUND[3:0] areFh.
12-4 Reserved 0 Read returns 0.Writes have noeffect.
3-0 Input Enable The eFuse Controller ECC Selftest Enable bitstarts theselftest oftheECC logic ifthefour
input enable bits(EFCBOUND[3:0) areall1s.
Fh ECC selftest canbestarted ifEFC ECC Selftest Enable, bit13,isset
Allothers ECC selftest cannot bestarted.

<!-- Page 2193 -->

www.ti.com eFuse Controller Registers
2193 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.4.2 EFC Pins Register (EFCPINS)
Figure 38-3 andFigure 38-3 describe theEFCPINS register.
Figure 38-3. EFC Pins Register (EFCPINS) [offset =2Ch]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
EFC Selftest
DoneEFC Selftest
ErrorReserved EFC Single Bit
ErrorEFC Instruction
ErrorEFC Autoload
ErrorReserved
R-0 R-0 R-0 R-x R-0 R-x R-x
7 0
Reserved
R-x
LEGEND: R=Read only; -n=value after power-on reset (nPORRST); x=Indeterminate
Table 38-4. EFC Pins Register (EFCPINS) Field Descriptions
Bit Name Value Description
31-16 Reserved 0 Reads return zeros, writes have noeffect.
15 EFC Selftest Done This bitcanbepolled todetermine when theEFC ECC selftest iscomplete
0 EFC ECC selftest isnotcomplete.
1 EFC ECC selftest iscomplete.
14 EFC Selftest Error This bitindicates thepass/fail status oftheEFC ECC Selftest once theEFC Selftest Done
bit(bit15)isset.
0 EFC ECC Selftest passed.
1 EFC ECC Selftest failed.
13 Reserved 0 Reads return zeros. DoNOT write a1tothisbit.
12 EFC Single BitError This bitindicates ifasingle biterror was corrected bytheECC logic during theautoload
after reset.
0 Nosingle biterror was detected.
1 Asingle biterror was detected andcorrected.
11 EFC Instruction Error This bitindicates anerror occurred during afactory testorprogram operation. This bit
should notbesetfrom normal use.
0 Noinstruction error detected.
1 Anerror occurred during afactory testorprogram operation.
10 EFC Autoload Error This bitindicates thatsome non-correctable error occurred during theautoload sequence
after reset. This bitalso sets ESM group 3,channel 1.
0 The autoload function completed successfully.
1 There were non-correctable errors during theautoload sequence.
9-0 Reserved 0-1 After reset, these bitsareindeterminate andreads return either a1or0.

<!-- Page 2194 -->

eFuse Controller Registers www.ti.com
2194 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.4.3 EFC Error Status Register (EFCERRSTAT)
Figure 38-4 andTable 38-5 describe theEFCERRSTAT register.
Figure 38-4. EFC Error Status Register (EFCERRSTAT) [offset =3Ch]
31 8
Reserved
R-0
7 6 5 4 0
Reserved Instruc Done Error Code
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after power-on reset (nPORRST)
Table 38-5. EFC Error Status Register (EFCERRSTAT) Field Descriptions
Bit Name Value Description
31-6 Reserved 0 Reads return zeros, writes have noeffect.
5 Instruc Done Instruction done. Used toindicate thattheeFuse selftesthascompleted
0 The eFuse controller isstillexecuting.
1 The eFuse controller hascompleted executing.
4-0 Error Code The error status ofthelastinstruction executed bytheeFuse Controller
0 Noerror.
5h Anuncorrectable (multibit) error was detected during thepower-on autoload sequence.
15h Atleast onesingle biterror was detected andcorrected during thepower-on autoload
sequence.
18h The signature generated bytheECC self-test logic didnotmatch thegolden signature
written intheEFCSTSIG register. The EDAC circuitry might have afault.
Allother
valuesAllother values arereserved fore-fuse system tests andarenotexpected tooccur in
normal system use.
38.4.4 EFC Self Test Cycles Register (EFCSTCY)
Figure 38-5 andTable 38-6 describe theEFCSTCY register.
Figure 38-5. EFC SelfTest Cycles Register (EFCSTCY) [offset =48h]
31 16
Cycles
R/W-0
15 0
Cycles
R/W-0
LEGEND: R/W =Read/Write; -n=value after power-on reset (nPORRST)
Table 38-6. EFC SelfTest Cycles Register (EFCSTCY) Field Descriptions
Bit Name Description
31-0 Cycles This register isused todetermine thenumber ofcycles toruntheeFuse controller ECC logic selftest. Itis
recommended touseavalue of600(0x00000258).

<!-- Page 2195 -->

www.ti.com eFuse Controller Registers
2195 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedeFuse Controller38.4.5 EFC Self Test Signature Register (EFCSTSIG)
Figure 38-6 andTable 38-7 describe theEFCSTSIG register.
Figure 38-6. EFC SelfTest Cycles Register (EFCSTSIG) [offset =4Ch]
31 16
Signature
R/W-0
15 0
Signature
R/W-0
LEGEND: R/W =Read/Write; -n=value after power-on reset (nPORRST)
Table 38-7. EFC SelfTest Cycles Register (EFCSTSIG) Field Descriptions
Bit Name Description
31-0 Signature This register isused tohold theexpected signature fortheeFuse ECC logic selftest. Itisrecommended to
write avalue of0x5362F97F tothisregister andavalue of600(0x00000258) totheEFCSTCY register. If
after running theeFuse ECC logic selftest, thecalculated signature does notmatch theexpected
signature intheEFCSTSIG register, then avalue of18hisstored intheEFCERRSTAT register.