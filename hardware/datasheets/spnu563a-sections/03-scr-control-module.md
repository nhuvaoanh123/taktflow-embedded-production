# SCR Control Module (SCM)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 252-264

---


<!-- Page 252 -->

252 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)Chapter 3
SPNU563A -March 2018
SCR Control Module (SCM)
This chapter describes theSCR control module (SCM). SCR istheCPU Interconnect Subsystem.
Topic ........................................................................................................................... Page
3.1 Overview ......................................................................................................... 253
3.2 Module Operation ............................................................................................. 255
3.3 How toUseSCM............................................................................................... 257
3.4 SCM Registers ................................................................................................. 260

<!-- Page 253 -->

www.ti.com Overview
253 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.1 Overview
The SCR (Switch Central Resource) Control Module (SCM) provides ameans tocontrol andmonitor the
main interconnect.
Interconnect hardware checker performs four major functional checks oninterconnect:
*Arbitration
*Timeout
*Protocol conversion
*Parity oncontrol /address signals
Any ofthese errors willforce theinterconnect totrigger anerror event toESM group 1(see ESM group1
mapping). Itisrecommended thatyoushould settheinterconnect error totoggle ESM pinaction. The
reason isthatifanerror occurs andCPU cannotaccess toFlash orRAM torundiagnostic orretry,
external monitoring ASIC canbenotified byESM error pin.
3.1.1 Features
The following main features aresupported:
*Compares therealtime running counter oftransaction command request totransaction command
accept from each initiator agent (IA-isthebusmaster thatinitiates transactions totheinterconnect.
Refer totheInterconnect chapter formore definition andconnection) with theREQ2ACCEPT threshold
value. iftherealtime counter isequal orlarger than thethreshold, theSCM willtrigger anerror event
toESM. Acorresponding status bitofthecorresponding IAwillalso beset.
*Compares therealtime running counter oftransaction command request accepted totransaction
command response accepted from each IAwith theREQ2RESP threshold value. Iftherealtime
counter isequal orlarger than thethreshold, theSCM willtrigger anerror event toESM. A
corresponding status bitofthecorresponding IAwillalso beset.
*Provides acontrol keytoclear thetime outcounters overrun inside hardware checker ofthe
interconnect. This control bitwillclear allregisters andmake thetimeout module available torestart
properly.
*Provides acontrol keytostart aself-test sequence oftheinterconnect hardware checker.
*Provides acontrol keytoclear global error flaginside interconnect hardware checker.
*Captures theactive status ofeach IAandtarget agent (TA-isthebusslave receives transaction
request from interconnect andresponses toit.Refer totheInterconnect chapter oftheTRM formore
definition andconnection) oftheinterconnect. The active status bitindicates thatthere isstillpending
transactions inside interconnect.
*Provides theability tooverride parity polarity oftheinterconnect hardware checker sothattheparity
detection logic canbeself-tested.

<!-- Page 254 -->

SCRIA0 IA1 IA_n
TA0 TA_mSCM
req2accept req2resp req2accept req2resp req2accept req2respTo_clearDtc_soft_reset (3:0)
Global_error_clractive_ia_o(n-1:0)err_eventSdc_test_finished
Hwchkr_sdc_soft_resetclkstopppedm_0/1
acpidleParity_diagnostic_enable
active_ta_o(m-1:0)
Overview www.ti.com
254 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.1.2 System Block Diagram
Figure 3-1shows thesystem level block diagram ofSCM andinterconnect (SCR).
SCM compares thetransaction command request totransaction command accept (req2accept) counters
andtransaction command request totransaction command response (req2resp) counters ofeach initiator
agent (IA)tothecorresponding threshold values (programmable). Ifthereq2accept orreq2resp counters
arelarger than orequal tothethreshold, SCM willgenerate error event toESM module.
SCM canclear thereq2accept andreq2resp counters inside interconnect SCR. Itcanalso initiate self-test
sequence tomake sure thehardware checker diagnostic logic isfunctioning properly.
Figure 3-1.System Level Block Diagram
nisthemaximum number ofIA.misthemaximum number ofTA.

<!-- Page 255 -->

MMR RegistersOCP MMR interface
Threshold
Compare Block
SCM Control
Blockactive_ia_o(n-1:0)
active_ta_o(m-1:0)
Req2accept from each IA
Req2resp from each IA
err_event
Keys/Controlsignals
To_clear
Dtc_soft_reset(3:0)
Global_error_clr
Hwchkr_sdc_soft_resetCLKSTOPPEDm_0/1
ACPIDLE
Parity_diagnostic_enable
www.ti.com Module Operation
255 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.2 Module Operation
3.2.1 Block Diagram
Figure 3-2shows theblock diagram ofSCM.
Figure 3-2.SCM Block Diagram
nisthemaximum number ofIA.misthemaximum number ofTA.
3.2.2 Timeout Threshold Compare Block
The threshold compare block (Figure 3-3)takes thereal-time counters (command request tocommand
accepted andcommand request tocommand response) from each IAoftheinterconnect hardware
checker module andcompare against thecorresponding threshold value inSCM every cycle. IfanyIA
comparison fails, theSCM module willupdate thecorresponding status bitinSCMIAERR0STAT and
SCMIAERR1STAT registers. SCMIAERR0STAT logs thetime outerror forcommand request tocommand
accepted. SCMIAERR1STAT logs thetime outerror forcommand request tocommand response. Any
status bitsetinthese twostatus registers willtrigger anerror event toESM (Error Signaling Module) and
willnottrigger again until cleared byCPU.
Any ofthese status bitscanbecleared byaprivilege write totheindividual bit.The write clear from CPU
tothese status bitswillalways take higher priority than setting ofthestatus bitsfrom theinterconnect.

<!-- Page 256 -->

To_clear
MMR Key
decodeTo_clear
Dtc_soft_reset
MMR Key
decodeDtc_soft_reset (3:0)
Global_error_clr
MMR Key
decodeGlobal_error_clrlogicHwchkr_sdc_soft_reset
PAR DIAG EN
MMR Key
decodeParity_diagnostic_enable
REQ2ACCPT_MAX REQ2RESP_MAX
>=
IA(n) REQ2ACCEPT IA(n) REQ2RESP>=
SCMIAERR0STAT(n) SCMIAERR1STAT(n)error error
Error_event
Other
compare
error
Module Operation www.ti.com
256 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)Figure 3-3.Timeout Threshold Compare
3.2.2.1 Interconnect Timeout Clearing Control Key
When thethreshold compare block triggers atime outerror, theESM willbenotified andcaninterrupt the
main CPU. The interconnect hardware checker real-time counter needs tobereset to0inorder torestart
properly. Section 3.3hasrecommendations onhow youshould react inthiscase. You canclear allthe
realtime counter values inside interconnect hardware checker. This isnecessary torestart therealtime
counter.
3.2.3 SCM Control Block
Figure 3-4shows ablock diagram oftheSCM.
Figure 3-4.SCM Control Block

<!-- Page 257 -->

www.ti.com How toUse SCM
257 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.2.3.1 Control Keytoinvert Parity Polarity forInterconnect Hardware Checker Parity Detection
Diagnostic
The interconnect receives parity bitsassociated with input control andaddress signals anddoes theparity
checking. The interconnect also generates parity bitsforcorresponding control andaddress signals. To
testtheparity checking logic, theSCM caninvert theparity polarity bittotheinterconnect topurposely
creates afailorpass parity checking condition.
Section 3.3hasrecommendations onhow youshould testtheparity detection logic inside interconnect
hardware checker.
3.2.3.2 Global Error Clearing Control Key
Interconnect subsystem triggers global error incase ofanyofthefollowing errors happen:
*Parity checking error onanybusmaster.
*Arbitration error.
*Protocol conversion error.
*Self-test failinself-test diagnostic mode.
Aglobal error from interconnect subsystem canresult innon-recoverable condition forthedevice. Itis
recommended thatuser issues aglobal error clear bywriting 0xA totheGLOBAL_ERR_CLR ofthe
SCMCNTRL register inconjunction tosystem reset.
3.2.3.3 Interconnect Hardware Checker Self-test
Interconnect hardware checker performs four major diagnostic checks oninterconnect:
*Arbitration
*Timeout
*Protocol conversion
*Parity oncontrol /address signals
Thus, itisnecessary tobeable todoself-test ofinterconnect hardware checker logic whenever you
decide atappropriate time intheapplication control loop. The self-test logic willcreate normal and
erroneous transaction from each master toeach slave according tothebusconnection matrix toverify that
thehardware checker properly functioning. See Section 3.3.2 fordetail onhow tostart self-test.
3.3 How toUseSCM
3.3.1 How toCheck theParity Compare Logic
Interconnect hasassociated parity bitsforcontrol andaddress bitofthecommunication bus. Parity check
isdone forallcontrol andaddress input. Parity generation isdone forallcontrol andaddress output.
Forfailsafety reason, parity checking logic needs tobetested atyour choice oftime intheir control loop.
Toenable theparity detection test, youshould switch toprivilege mode andwrite 0xA to
SCMCNTRL[27:24] control register. This willinvert theparity polarity andtesting foronly onecycle. The
SCM module willreset thecontrol keyback to0x5once ittriggers aninversion parity polarity to
interconnect hardware checker. Since parity polarity isinverted only inside interconnect, theinterconnect
willflagparity error forinput control andaddress signals. The interconnect also output aninverted polarity
foroutput control andaddress signals. Thus, master andslave IPconnected tointerconnect could
potentially generate parity error aswell. This way, thecorresponding parity detection logic inmaster and
slave IPcanbetested atthesame time. You should clear allparity error status bitsresiding inmaster IP,
slave IP,orinterconnect status registers.
Note thatthehardware only does parity inversion check inonecycle sothatitdoes notblock outCPU
access toFlash andRAM onsubsequence cycle.

<!-- Page 258 -->

How toUse SCM www.ti.com
258 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.3.2 How toInitiate Self-test Sequence
Itisnecessary tobeable todoself-test oftheinterconnect hardware checker logic todetect residual faults
when youdecide atappropriate time intheapplication control loop. The self-test logic willcreate normal
anderroneous transaction from each master toeach slave according tothebusconnection matrix toverify
thatthehardware checker andtheinterconnect functioning properly.
Toinitiate theself-test sequence, youshould switch toprivilege mode.
1.Software needs toensure thatMASK_SOFT_RESET control bitoftheinterconnect self-test control
register (Interconnect SDC MMR offset at0xFA00_0000[0]) is0.
2.Software needs toensure thatGCLK1 isstillrunning.
3.Software needs toensure thatallbusmaster connecting tointerconnect should stop sending new
transaction tointerconnect. The hardware willmake sure thatalloutstanding transaction willcomplete.
4.Software writes toSCM control register bitfield DTC_SOFT_RESET akeyvalue: 0xA toinitiate self-
test.
5.CPU0 andCPU1 must execute WFI instruction.
a.Atthispoint, thehardware willensure thatthere isnooutstanding transaction inside interconnect
andwilltrigger self-test.
b.While hardware checker self-test isongoing, theCPU willbeheld inreset andreleased until self-
testcompletes
6.Once self-test completes, CPU willboot upfrom 0x0again andyouneed toread interconnect
diagnostic register toinspect foranyerror detected during self-test. Refer todevice technical reference
manual forbase address.

<!-- Page 259 -->

www.ti.com How toUse SCM
259 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.3.3 How toConfigure Timeout Check
The threshold compare block takes therealtime counters (command request tocommand accepted and
command request tocommand response) from each IAoftheinterconnect hardware checker module and
compare against thecorresponding threshold value inSCM every cycle. IfanyIAcomparison fails, the
SCM module willupdate thecorresponding status bitinSCMIAERR0STAT andSCMIAERR1STAT
registers. SCMIAERR0STAT logs thetime outerror forcommand request tocommand accepted.
SCMIAERR1STAT logs thetime outerror forcommand request tocommand response. Any status bitset
inthese twostatus registers willtrigger anerror event toESM (Error Signaling Module) andwillnottrigger
again until cleared byCPU.
You should configure theSCMTHRESHOLD control register tosetup thecommand transaction request to
command transaction acceptance threshold aswell ascommand transaction request tocommand
transaction response threshold. Itisrecommended thatyouusethedefault reset value ofdecimal 1024
(400h) fortheSCMTHRESHOLD control registers. However, youcanchange thisvalues depending on
application depending onthenumber ofIAandTArequired bytheinterconnect.
When threshold compare block triggers atime outerror, theerror willbesent totheESM module resulting
inaninterrupt exception totheCPU.
When interrupted, itisrecommended thatyouread theSCMIAERR0STAT andSCMIAERR1STAT tofind
outwhich master orslave having thetime outissue andclear therealtime counter inside interconnect.
Then, issue aretry onthetransaction
1.Iftheretry issuccessful, youcanresume operation.
2.Iftheretry fails because time outstilloccurs, youshould trigger aself-test tocheck foranyissue of
interconnect. Ifself-test fails ortime outerror stilloccurs after passing self-test, youshould trytoshut
down thesystem inasafe way. Inthecase thatinterconnect hasissue andblocking access toFlash
orRAM, ESM pinaction cannotbereset thus external monitoring ASIC willbenotified.
Toclear realtime counters inside SCR, youshould switch toprivilege mode andwrite Ahtothe
SCMCNTRL[3:0] control register. The SCM module willreset thecontrol keyback to5honce ittriggers a
clear command tointerconnect hardware checker. The interconnect hardware checker realtime counter
needs tobereset to0inorder torestart properly.

<!-- Page 260 -->

SCM Registers www.ti.com
260 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.4 SCM Registers
The SCM registers arelisted inTable 3-1.Each register begins onaword boundary. The registers
support 8-,16-, and32-bit accesses. The address offset isspecified from thebase address of
FFFF 0A00h.
Registers areaccessed through adedicated MMR interface. Support only read, write, andwrite non-
posted. Read andwrite arealways returning response status. Awrite toreserved bitshasnoeffect.
Ifthere issofterror oranyother event thatresults inanunsupported command such asreadlink-write
conditional orbroadcast bustransactions, theMMR interface willreturn with response buserror for
unsupported command. Software should check forvalid address andwhether thetarget isinlowpower
mode ornotprior toissue aretry access.
Table 3-1.SCM Registers
Offset Acronym Register Description Section
00h SCMREVID SCM REVID Register Section 3.4.1
04h SCMCNTRL SCM Control Register Section 3.4.2
08h SCMTHRESHOLD SCM Compare Threshold Counter Register Section 3.4.3
10h SCMIAERR0STAT SCM Initiator Error0 Status Register Section 3.4.4
14h SCMIAERR1STAT SCM Initiator Error1 Status Register Section 3.4.5
18h SCMIASTAT SCM Initiator Active Status Register Section 3.4.6
20h SCMTASTAT SCM Target Active Status Register Section 3.4.7
3.4.1 SCM REVID Register (SCMREVID)
Figure 3-5.SCM REVID Register (SCMREVID) [offset =00h]
31 30 29 28 27 16
SCHEME Reserved FUNC
R-1 R-0 R-A0Bh
15 11 10 8 7 6 5 0
RTL MAJOR CUSTOM MINOR
R-0 R-0 R-0 R-2h
LEGEND: R=Read only; -n=value after synchronous reset onsystem reset
Table 3-2.SCM REVID Register (SCMREVID) Field Descriptions
Bit Field Value Description
31-30 SCHEME 1 Identification scheme.
29-28 Reserved 0 Reserved. Reads return 0.
27-16 FUNC A0Bh Indicates functionally equivalent module family.
15-11 RTL 0 RTL version number.
10-8 MAJOR 0 Major revision number.
7-6 CUSTOM 0 Indicates device-specific implementation.
5-0 MINOR 2h Minor revision number.

<!-- Page 261 -->

www.ti.com SCM Registers
261 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.4.2 SCM Control Register (SCMCNTRL)
Figure 3-6.SCM Control Register (SCMCNTRL) [offset =04h]
31 28 27 24 23 20 19 16
Reserved PAR DIAG EN Reserved GLOBAL_ERROR_CLR
R-0 R/WP-5h R-0 R/WP-5h
15 12 11 8 7 4 3 0
Reserved DTC_SOFT_RESET Reserved TO_CLEAR
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after synchronous reset onsystem reset
Table 3-3.SCM Control Register (SCMCNTRL) Field Descriptions
Bit Field Value Description
31-28 Reserved 0 Reserved. Reads return 0.
27-24 PAR DIAG EN Sticky keywrite values ofAh.Writing Ahsends outanactive-high "pulse "totheSCR
andresets thekeyback to5h.
Read:
5h Sticky key.
Allother values Reserved
Write inPrivilege:
Ah Parity diagnostic enable.
Allother values Reserved
23-20 Reserved 0 Reserved. Reads return 0.
19-16 GLOBAL_ERROR_CLR Clear global error ininterconnect. Writing Ahsends outaclear pulse totheSCR and
resets thekeyback to5h.
Read:
5h Sticky key.
Allother values Reserved
Write inPrivilege:
Ah Enable global error clear.
Allother values Reserved
15-12 Reserved 0 Reserved. Reads return 0.
11-8 DTC_SOFT_RESET Diagnostic self-test error enable. Writing Ahforces theSCM toinitiate self-test
sequence. The hardware willreset thekeyback to0x5whenever self-test is
completed.
Read:
5h Sticky key.
Allother values Reserved
Write inPrivilege:
Ah Enable sequence tostart interconnect self-test.
Allother values Reserved
7-4 Reserved 0 Reserved. Reads return 0.
3-0 TO_CLEAR Clear realtime counters inside SCR. Writing Ahsends outaclear pulse totheSCR
andresets thekeyback to5h.
Read:
5h Sticky key.
Allother values Reserved
Write inPrivilege:
Ah Enable global error clear.
Allother values Reserved

<!-- Page 262 -->

SCM Registers www.ti.com
262 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.4.3 SCM Compare Threshold Counter Register (SCMTHRESHOLD)
Figure 3-7.SCM Compare Threshold Counter Register (SCMTHRESHOLD) [offset =08h]
31 16
REQ2RESPONSE_MAX
R/WP-400h
15 0
REQ2ACCEPT_MAX
R/WP-400h
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after synchronous reset onsystem reset
Table 3-4.SCM Compare Threshold Counter Register (SCMTHRESHOLD) Field Descriptions
Bit Field Value Description
31-16 REQ2RESPONSE_MAX 0-FFFFh Request toResponse Threshold values. You need toconfigure themaximum
threshold values forrequest toresponse timeout. Reset values equals tothevalues of
REQ2RESP_RST generic parameter.
Read: Values ofcounter.
Write inPrivilege: Values ofcounter.
15-0 REQ2ACCEPT_MAX 0-FFFFh Request toAccept Threshold values. You need toconfigure themaximum threshold
values forrequest toaccept timeout. Reset values equals tothevalues of
REQ2ACCEPT_RST generic parameter.
Read: Values ofcounter.
Write inPrivilege: Values ofcounter.

<!-- Page 263 -->

www.ti.com SCM Registers
263 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.4.4 SCM Initiator Error0 Status Register (SCMIAERR0STAT)
Figure 3-8.SCM Initiator Error0 Status Register (SCMIAERR0STAT) [offset =10h]
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
R2A7 R2A6 R2A5 R2A4 R2A3 R2A2 R2A1 R2A0
R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after synchronous reset bypower-on reset
Table 3-5.SCM Initiator Error0 Status Register (SCMIAERR0STAT) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved. Read returns 0.
7-0 R2An Request toAcceptance Timeout Error happens onIAn.Each bitncorresponds torequest toaccept
time outerror occurred foreach IA.Refer toInterconnect chapter oftheTRM forspecific mapping of
each R2Antoaparticular IP.
Read:
0 Norequest toaccept time outerror happens onIAn.
1 Request toaccept time outerror happens onIAn.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.
3.4.5 SCM Initiator Error1 Status Register (SCMIAERR1STAT)
Figure 3-9.SCM Initiator Error1 Status Register (SCMIAERR1STAT) [offset =14h]
31 8
Reserved
R-0
7 6 5 4 3 2 1 0
R2R7 R2R6 R2R5 R2R4 R2R3 R2R2 R2R1 R2R0
R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after synchronous reset bypower-on reset
Table 3-6.SCM Initiator Error1 Status Register (SCMIAERR1STAT) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reserved. Read returns 0.
7-0 R2Rn Request toResponse Timeout Error happens onIAn.Each bitncorresponds torequest toresponse
time outerror occurred foreach IA..Refer toInterconnect chapter oftheTRM
Read:
0 Norequest toresponse time outerror happens onIAn.
1 Request toresponse time outerror happens onIAn.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.

<!-- Page 264 -->

SCM Registers www.ti.com
264 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSCR Control Module (SCM)3.4.6 SCM Initiator Active Status Register (SCMIASTAT)
Figure 3-10. SCM Initiator Active Status Register (SCMIASTAT) [offset =18h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
Reserved IAST13 IAST12 IAST11 IAST10 IAST9 IAST8
R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
IAST7 IAST6 IAST5 IAST4 IAST3 IAST2 IAST1 IAST0
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after synchronous reset bysystem reset
Table 3-7.SCM Initiator Active Status Register (SCMIASTAT) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reserved. Read returns 0.
13-0 IAST n IA(Initiator Agent) Status. Each bitnindicates thatthere isapending transaction onthecorresponding
IAn.Refer toInterconnect chapter oftheTRM formapping ofmaster port totheSCMIASTAT register
bit.
0 Nopending transaction inIAn.
1 Pending transaction inIAn.
3.4.7 SCM Target Active Status Register (SCMTASTAT)
Figure 3-11. SCM Target Active Status Register (SCMTASTAT) [offset =20h]
31 16
Reserved
R-0
15 14 13 12 11 10 9 8
Reserved TAST13 TAST12 TAST11 TAST10 TAST9 TAST8
R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
TAST7 TAST6 TAST5 TAST4 TAST3 TAST2 TAST1 TAST0
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after synchronous reset bysystem reset
Table 3-8.SCM Target Active Status Register (SCMTASTAT) Field Descriptions
Bit Field Value Description
31-14 Reserved 0 Reserved. Read returns 0.
13-0 TAST n TA(Target Agent) Status. Each bitnindicates thatthere isapending transaction onthecorresponding
TAn.Refer toInterconnect chapter oftheTRM formapping ofslave port totheSCMTASTAT register bit.
0 Nopending transaction inTAn.
1 Pending transaction inTAn.