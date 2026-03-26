# Error Signaling Module (ESM)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 558-582

---


<!-- Page 558 -->

558 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)Chapter 16
SPNU563A -March 2018
Error Signaling Module (ESM)
This chapter provides thedetails oftheerror signaling module (ESM) that aggregates device errors and
provides internal andexternal error response based onerror severity.
Topic ........................................................................................................................... Page
16.1 Overview ......................................................................................................... 559
16.2 Module Operation ............................................................................................. 561
16.3 Recommended Programming Procedure ............................................................. 564
16.4 ESM Control Registers ...................................................................................... 565

<!-- Page 559 -->

Interrupt PriorityLow-Priority
Interrupt Handling
High-Priority
Interrupt Handling
Error Signal
Handlingerror_group1
error_group2
error_group3ERRORDeviceLow-Priority
Interrupt
High-Priority
Interruptfrom Hardware Diagnosticsto VIM Interrupt Controller
Interrupt Enable
ERROR Pin Enable
Output
PIN
www.ti.com Overview
559 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.1 Overview
The Error Signaling Module (ESM) collects andreports thevarious error conditions onthemicrocontroller.
The error condition iscategorized based onaseverity level. Error response isthen generated based on
thecategory oftheerror. Possible error responses include alowpriority interrupt, high priority interrupt,
andanexternal pinaction.
16.1.1 Feature List
*Upto160error channels aresupported, divided into3different groups:
-96Group1 (low severity) channels with configurable interrupt generation andconfigurable ERROR
pinbehavior
-32Group2 (high severity) channels with predefined interrupt generation andpredefined ERROR pin
behavior
-32Group3 (high severity) channels with nointerrupt generation andpredefined ERROR pin
behavior. These channels have nointerrupt response asthey arereserved forCPU based
diagnostics thatgenerate aborts directly totheCPU.
*Dedicated device ERROR pintosignal anexternal observer
*Configurable timebase forERROR pinoutput
*Error forcing capability forlatent fault testing
16.1.2 Block Diagram
Asshown inFigure 16-1,theESM channels aredivided intothree groups. Group1 channels are
considered tobelowseverity. Group1 errors have aconfigurable interrupt response andconfigurable
ERROR pinbehavior. Note thattheESM Status Register 1(ESMSR1) forerror group 1gets updated,
regardless iftheinterrupt enable isactive ornot.Group2 channels areERROR high severity. Group2
errors always generate ahigh priority interrupt andanoutput ontheERROR pin.Group3 channels
indicate errors ofthehighest severity. Check thespecific part's datasheet foridentifying group3 errors and
their expected responses. Group3 errors always generate anERROR pinoutput.
The ESM interrupt andERROR pinbehavior arealso summarized inTable 16-1.
Figure 16-1. Block Diagram
Note thattheESM Status Register 1(ESMSR1) forerror_group1 gets updated, regardless iftheinterrupt enable is
active ornot.

<!-- Page 560 -->

Error Signal
Low-Time
Countererror_group1
error_group2
error_group3ERRORControlLow-Time
Counter PreloadMemory mapped register interface
Peripheral clock (VCLK)
CPU clock (GCLK)
(L TCP )
(L TC)ERROR Pin Enable
Controlled by:
ESMIEPSR1
ESMIEPCR1
ESMIEPSR4
ESMIEPCR4
Device
Output
PIN
ESMEPSRESMIEPSR7
ESMIEPCR7
Interrupt Priority
Controlled by:
ESMILSR1
ESMILCR1
ESMILSR4
ESMILCR4Low-Priority
Interrupt Handling
High-Priority
Interrupt Handlingerror_group1
error_group2Low-Priority Interrupt
High-Priority Interruptfrom Hardware Diagnosticsto VIM Interrupt Controller
Interrupt Enable
Controlled by:
ESMIESR1
ESMIECR1
ESMIESR4
ESMIECR4
ESMIESR7
ESMIECR7ESMILSR7
ESMILCR7
Overview www.ti.com
560 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)Table 16-1. ESM Interrupt andERROR PinBehavior
Error Group Interrupt Generated Interrupt Priority ERROR PinResponse Generated
1 configurable interrupt configurable priority configurable output generation
2 interrupt generated high priority output generated
3 nointerrupt NA output generated
Figure 16-2 andFigure 16-3 show theinterrupt response handling andERROR pinresponse handling
with register configuration. The total active time oftheERROR piniscontrolled bytheLow-Time Counter
Preload register (LTCP) andthekeyregister (ESMEPSR) asshown inFigure 16-3 .See Section 16.2.2 for
details.
Figure 16-2. Interrupt Response Handling
Figure 16-3. ERROR PinResponse Handling

<!-- Page 561 -->

www.ti.com Module Operation
561 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.2 Module Operation
This device has160error channels, divided into3different error groups. Please refer tothedevice
datasheet forESM channel assignment details.
The ESM module haserror flags foreach error channel. The error status registers ESMSR1, ESMSR4,
ESMSR7, ESMSR2, ESMSR3 provide status information onapending error ofGroup1 (Channel 0-31),
Group1 (Channel 32-63), Group1 (Channel 64-95), Group2, andGroup3, respectively. The ESMEPSR
register provides thecurrent ERROR status. The module also provides astatus shadow register,
ESMSSR2, which maintains theerror flags ofGroup2 until power-on reset (PORRST) isasserted. See
Section 16.2.1 fordetails oftheir behavior during power onreset andwarm reset.
Once anerror occurs, theESM module willsetthecorresponding error flags. Inaddition, itcantrigger an
interrupt, ERROR pinoutputs lowdepending ontheESM settings. Once theERROR pinoutputs low, a
power onreset orawrite of0x5toESMEKR isrequired torelease theESM error pinback tonormal state.
See Section 16.2.2 fordetails. The application canread theerror status registers (ESMSR1, ESMSR4,
ESMSR7, ESMSR2, andESMSR3) todebug theerror. IfanRST istriggered ortheerror interrupt has
been served, theerror flagofGroup2 should beread from ESMSSR2 because theerror flaginESMSR2
willbecleared byRST.
You canalso testthefunctionality oftheERROR pinbyforcing anerror. See Section 16.2.3 fordetails.
16.2.1 Reset Behavior
Power onreset:
*ERROR pinbehavior
When nPORRST isactive, theERROR pinisinahigh impedance state (output drivers disabled).
*Register behavior
After PORRST, allregisters inESM module willbere-initialized tothedefault value. Alltheerror status
registers arecleared tozero.
Warm reset (RST):
*ERROR pinbehavior
During RST, theERROR pinisin"output active "state with pull-down disabled. The ERROR pin
remains unchanged after RST.
*Register behavior
After RST, ESMSR1, ESMSR4, ESMSR7, ESMSSR2, ESMSR3 andESMEPSR register values
remains un-changed. Since RST does notclear thecritical failure registers, theuser canread those
registers todebug thefailures after RST pingoes back tohigh.
After RST, ifoneoftheflags inESMSR1, ESMSR4 andESMSR7 isset,theinterrupt service routine
willbecalled once thecorresponding interrupt isenabled.
NOTE: ESMSR2 iscleared after RST. The flaginESMSR2 gets cleared when reading the
appropriate vector intheESMIOFFHR offset register. Reading ESMIOFFHR willnotclear the
ESMSR1, ESMSR4, ESMSR7 andtheshadow register ESMSSR2. Reading ESMIOFFLR
willalso notclear theESMSR1, ESMSR4 andESMSR7.

<!-- Page 562 -->

failure
ERROR tERROR _lowERROR pin reset request
failure
ERROR tERROR _lowERROR pin reset request
failure
ERROR tERROR _low
) 1 (_/c43 /c180 /c61 LTCP t tVCLK low ERROR
Module Operation www.ti.com
562 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.2.2 ERROR PinTiming
The ERROR pinisanactive lowfunction. The state ofthepinisalso readable from ERROR PinStatus
Register (ESMEPSR). Awarm reset (RST) does notaffect thestate ofthepin.The pinisinahigh-
impedance state during power-on reset. Once theESM module drives theERROR pinlow, itremains in
thisstate forthetime specified bytheLow-Time Counter Preload register (LTCPR). Based onthetime
period oftheperipheral clock (VCLK), thetotal active time oftheERROR pincanbecalculated as:
(22)
Once thisperiod expires, theERROR pinissettohigh incase thereset oftheERROR pinwas
requested. This request isdone bywriting anappropriate key(0x5) tothekeyregister (ESMEKR) during
theERROR pinlowtime. Here areafewexamples:
Example 1:ESM detects afailure anddrives theERROR pinlow. NoERROR pinreset isrequested. The
ERROR pincontinues outputting lowuntil power onreset occurs.
Figure 16-4. ERROR PinTiming -Example 1
Example 2:ESM detects afailure anddrives theERROR pinlow. AnERROR pinreset request is
received before tERROR_low expires. Inthiscase, theERROR pinissettohigh immediately after tERROR_low
expires.
Figure 16-5. ERROR PinTiming -Example 2
Example 3:ESM detects afailure anddrives theERROR pinlow. AnERROR pinreset request is
received after tERROR_low expires. Inthiscase, theERROR pinissettohigh immediately after ERROR pin
reset request isreceived.
Figure 16-6. ERROR PinTiming -Example 3

<!-- Page 563 -->

failure
ERRORWrite "1010" to ESMEKR Write "0" to ESMEKRWrite "0101" to ESMEKR
tERROR _low
failure
ERROR tERROR _lowERROR pin reset request
failure failure
ERRORtERROR _lowtERROR _lowERROR pin reset request
www.ti.com Module Operation
563 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)Example 4:ESM detects afailure anddrives theERROR pinlow. Another failure occurs within thetime
thepinstays low. Inthiscase, thelowtime counter willbereset when theother failure occurs. Inother
words, tERROR_low should becounted from whenever themost recent failure occurs.
Figure 16-7. ERROR PinTiming -Example 4
Example 5:The reset oftheERROR pinwas requested bythesoftware even before thefailure occurs. In
thiscase, theERROR pinissettohigh immediately after tERROR_low expires. This case isnotrecommended
andshould beavoided bytheapplication.
Figure 16-8. ERROR PinTiming -Example 5
16.2.3 Forcing anError Condition
The error response generation mechanism istestable bysoftware byforcing anerror condition. This
allows testing theERROR pinfunctionality. Bywriting adedicated keytotheerror forcing keyregister
(ESMEKR), theERROR pinissettolowforthespecified time. The following steps describe how toforce
anerror condition:
1.Check ERROR PinStatus Register (ESMEPSR). This register must be1toswitch intotheerror forcing
mode.
The ESM module cannot beswitched intotheerror forcing mode ifafailure hasalready been detected
infunctional mode. The application command toswitch toerror forcing mode isignored.
2.Write "1010b "totheerror forcing keyregister (ESMEKR). After that, theERROR pinshould output low
(error force mode).
Once theapplication puts theESM module intheerror forcing mode, theERROR pincannot indicate
thenormal error functionality. Ifafailure occurs during thistime, itgets stilllatched andtheLTC is
reset andstopped. The error output pinisalready driven lowonaccount oftheerror forcing mode.
When theESM isforced back tonormal functional mode, theLTC becomes active andforces the
ERROR pinlowuntil theexpiration oftheLTC (see Figure 16-9).
3.Write "0000 "totheerror forcing keyregister (ESMEKR) back totheactive normal mode.
Ifthere arenoerrors detected while theESM module isintheerror forcing mode, theERROR pin
goes high immediately after exiting theerror forcing mode.
Figure 16-9. ERROR PinTiming -Example 6

<!-- Page 564 -->

Initialize VIM RAM. Map the ESM low priority interrupt service routine and high priority interrupt
service routine to pre-defined device specific interrupt channel. (Refer to device specific datasheet.)Force error on pin to check the functionality of pin and external monitoring
device connected to pin (ESMEKR).ERROR ERROR
ERRORPower up or PORRST
Enable the interrupt in VIM and CPU.
Map ESM interrupt to high/low (ESM Group1 only, see register
ESMILSR1 and ESMILCR1, ESMILSR4 and ESMILCR4, ESMILSR7 and ESMILCR7).
Define ESM Low-Time Counter Preload Register ESMLTCPR to determine the pin
low time in case an error occurs.ERROREnable ESM interrupt and influence on pin (ESM Group1 only, see register ESMIEPSR1,
ESMIEPCR1, ESMIESR1, and ESMIECR1; ESMIEPSR4, ESMIEPCR4, ESMIESR4, and ESMIECR4;
ESMIEPSR7, ESMIEPCR7, ESMIESR7, and ESMIECR7).ERROR
Recommended Programming Procedure www.ti.com
564 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.3 Recommended Programming Procedure
During theinitialization stage, theapplication code should follow therecommendations inFigure 16-10 to
initialize theESM.
Once anerror occurs, itcantrigger aninterrupt, ERROR pinoutputs lowdepending ontheESM settings.
Once theERROR pinoutputs low, apower onreset orawrite of0x5toESMEKR isrequired torelease
theESM back tonormal state. The application canread theerror status registers (ESMSR1, ESMSR4,
ESMSR7, ESMSR2, andESMSR3) todebug theerror. IfanRST istriggered ortheerror interrupt has
been served, theerror flagofGroup2 should beread from ESMSSR2 because theerror flaginESMSR2
willbecleared byRST.
Figure 16-10. ESM Initialization

<!-- Page 565 -->

www.ti.com ESM Control Registers
565 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4 ESM Control Registers
Table 16-2 lists theESM registers. Each register begins ona32-bit word boundary. The registers support
8-,16-, and32-bit accesses. The base address forthecontrol registers isFFFF F500h.
Table 16-2. ESM Control Registers
Offset Acronym Register Description Section
00h ESMEEPAPR1 ESM Enable ERROR PinAction/Response Register 1 Section 16.4.1
04h ESMDEPAPR1 ESM Disable ERROR PinAction/Response Register 1 Section 16.4.2
08h ESMIESR1 ESM Interrupt Enable Set/Status Register 1 Section 16.4.3
0Ch ESMIECR1 ESM Interrupt Enable Clear/Status Register 1 Section 16.4.4
10h ESMILSR1 Interrupt Level Set/Status Register 1 Section 16.4.5
14h ESMILCR1 Interrupt Level Clear/Status Register 1 Section 16.4.6
18h ESMSR1 ESM Status Register 1 Section 16.4.7
1Ch ESMSR2 ESM Status Register 2 Section 16.4.8
20h ESMSR3 ESM Status Register 3 Section 16.4.9
24h ESMEPSR ESM ERROR PinStatus Register Section 16.4.10
28h ESMIOFFHR ESM Interrupt Offset High Register Section 16.4.11
2Ch ESMIOFFLR ESM Interrupt Offset Low Register Section 16.4.12
30h ESMLTCR ESM Low-Time Counter Register Section 16.4.13
34h ESMLTCPR ESM Low-Time Counter Preload Register Section 16.4.14
38h ESMEKR ESM Error Key Register Section 16.4.15
3Ch ESMSSR2 ESM Status Shadow Register 2 Section 16.4.16
40h ESMIEPSR4 ESM Influence ERROR PinSet/Status Register 4 Section 16.4.17
44h ESMIEPCR4 ESM Influence ERROR PinClear/Status Register 4 Section 16.4.18
48h ESMIESR4 ESM Interrupt Enable Set/Status Register 4 Section 16.4.19
4Ch ESMIECR4 ESM Interrupt Enable Clear/Status Register 4 Section 16.4.20
50h ESMILSR4 Interrupt Level Set/Status Register 4 Section 16.4.21
54h ESMILCR4 Interrupt Level Clear/Status Register 4 Section 16.4.22
58h ESMSR4 ESM Status Register 4 Section 16.4.23
80h ESMIEPSR7 ESM Influence ERROR PinSet/Status Register 7 Section 16.4.24
84h ESMIEPCR7 ESM Influence ERROR PinClear/Status Register 7 Section 16.4.25
88h ESMIESR7 ESM Interrupt Enable Set/Status Register 7 Section 16.4.26
8Ch ESMIECR7 ESM Interrupt Enable Clear/Status Register 7 Section 16.4.27
90h ESMILSR7 Interrupt Level Set/Status Register 7 Section 16.4.28
94h ESMILCR7 Interrupt Level Clear/Status Register 7 Section 16.4.29
98h ESMSR7 ESM Status Register 7 Section 16.4.30

<!-- Page 566 -->

ESM Control Registers www.ti.com
566 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.1 ESM Enable ERROR PinAction/Response Register 1(ESMEEPAPR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-11. ESM Enable ERROR PinAction/Response Register 1(ESMEEPAPR1)
[offset =00h]
31 16
IEPSET[31:16]
R/WP-0
15 0
IEPSET[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-3. ESM Enable ERROR PinAction/Response Register 1(ESMEEPAPR1)
Field Descriptions
Bit Field Value Description
31-0 IEPSET Enable ERROR PinAction/Response onGroup 1.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding clear bitintheESMIEPCR1 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Enables failure influence onERROR pinandsets thecorresponding clear bitinthe
ESMIEPCR1 register.
16.4.2 ESM Disable ERROR PinAction/Response Register 1(ESMDEPAPR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-12. ESM Disable ERROR PinAction/Response Register 1(ESMDEPAPR1)
[offset =04h]
31 16
IEPCLR[31:16]
R/WP-0
15 0
IEPCLR[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-4. ESM Disable ERROR PinAction/Response Register 1(ESMDEPAPR1)
Field Descriptions
Bit Field Value Description
31-0 IEPCLR Disable ERROR PinAction/Response onGroup 1.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding setbitintheESMIEPSR1 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Disables failure influence onERROR pinandclears thecorresponding setbitinthe
ESMIEPSR1 register.

<!-- Page 567 -->

www.ti.com ESM Control Registers
567 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.3 ESM Interrupt Enable Set/Status Register 1(ESMIESR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-13. ESM Interrupt Enable Set/Status Register 1(ESMIESR1) [offset =08h]
31 16
INTENSET[31:16]
R/WP-0
15 0
INTENSET[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-5. ESM Interrupt Enable Set/Status Register 1(ESMIESR1) Field Descriptions
Bit Field Value Description
31-0 INTENSET Setinterrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding clear bitintheESMIECR1 register unchanged.
1 Read: Interrupt isenabled.
Write: Enables interrupt andsets thecorresponding clear bitintheESMIECR1 register.
16.4.4 ESM Interrupt Enable Clear/Status Register 1(ESMIECR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-14. ESM Interrupt Enable Clear/Status Register 1(ESMIECR1) [offset =0Ch]
31 16
INTENCLR[31:16]
R/WP-0
15 0
INTENCLR[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-6. ESM Interrupt Enable Clear/Status Register 1(ESMIECR1) Field Descriptions
Bit Field Value Description
31-0 INTENCLR Clear interrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding setbitintheESMIESR1 register unchanged.
1 Read: Interrupt isenabled.
Write: Disables interrupt andclears thecorresponding setbitintheESMIESR1 register.

<!-- Page 568 -->

ESM Control Registers www.ti.com
568 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.5 ESM Interrupt Level Set/Status Register 1(ESMILSR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-15. ESM Interrupt Level Set/Status Register 1(ESMILSR1) [offset =10h]
31 16
INTLVLSET[31:16]
R/WP-0
15 0
INTLVLSET[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-7. ESM Interrupt Level Set/Status Register 1(ESMILSR1) Field Descriptions
Bit Field Value Description
31-0 INTLVLSET Setinterrupt priority.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt ofchannel xismapped tolowlevel interrupt line.
Write: Leaves thebitandthecorresponding clear bitintheESMILCR1 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh level interrupt line.
Write: Maps interrupt ofchannel xtohigh level interrupt lineandsets thecorresponding clear bitin
theESMILCR1 register.
16.4.6 ESM Interrupt Level Clear/Status Register 1(ESMILCR1)
This register isdedicated forGroup1 Channel[31:0].
Figure 16-16. ESM Interrupt Level Clear/Status Register 1(ESMILCR1) [offset =14h]
31 16
INTLVLCLR[31:16]
R/WP-0
15 0
INTLVLCLR[15:0]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-8. ESM Interrupt Level Clear/Status Register 1(ESMILCR1) Field Descriptions
Bit Field Value Description
31-0 INTLVLCLR Clear interrupt priority.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt ofchannel xismapped tolow-level interrupt line.
Write: Leaves thebitandthecorresponding setbitintheESMILSR1 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh-level interrupt line.
Write: Maps interrupt ofchannel xtolow-level interrupt lineandclears thecorresponding setbitin
theESMILSR1 register.

<!-- Page 569 -->

www.ti.com ESM Control Registers
569 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.7 ESM Status Register 1(ESMSR1)
This register isdedicated forGroup1 Channel[31:0]. Note thattheESMSR1 status register willget
updated ifanerror condition occurs, regardless ifthecorresponding interrupt enable flagissetornot.
Figure 16-17. ESM Status Register 1(ESMSR1) [offset =18h]
31 16
ESF[31:16]
R/W1CP-X/0
15 0
ESF[15:0]
R/W1CP-X/0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-9. ESM Status Register 1(ESMSR1) Field Descriptions
Bit Field Value Description
31-0 ESF Error Status Flag. Provides status information onapending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred; nointerrupt ispending.
Write: Leaves thebitunchanged.
1 Read: Error occurred; interrupt ispending.
Write: Clears thebit.
Note: After RST, ifoneofthese flags aresetandthecorresponding interrupt areenabled, the
interrupt service routine willbecalled.
16.4.8 ESM Status Register 2(ESMSR2)
This register isdedicated forGroup2.
Figure 16-18. ESM Status Register 2(ESMSR2) [offset =1Ch]
31 16
ESF2[31:16]
R/W1CP-0
15 0
ESF2[15:0]
R/W1CP-0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 16-10. ESM Status Register 2(ESMSR2) Field Descriptions
Bit Field Value Description
31-0 ESF2 Error Status Flag. Provides status information onapending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred; nointerrupt ispending.
Write: Leaves thebitunchanged.
1 Read: Error occurred; interrupt ispending.
Write: Clears thebit.ESMSSR2 isnotimpacted bythisaction.
Note: Innormal operation theflaggets cleared when reading theappropriate vector inthe
ESMIOFFHR offset register. Reading ESMIOFFHR willnotclear theESMSR1 andtheshadow
register ESMSSR2.

<!-- Page 570 -->

ESM Control Registers www.ti.com
570 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.9 ESM Status Register 3(ESMSR3)
This register isdedicated forGroup3.
Figure 16-19. ESM Status Register 3(ESMSR3) [offset =20h]
31 16
ESF3[31:0]
R/W1CP-X/0
15 0
ESF3[15:0]
R/W1CP-X/0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-11. ESM Status Register 3(ESMSR3) Field Descriptions
Bit Field Value Description
31-0 ESF3 Error Status Flag. Provides status information onapending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred.
Write: Leaves thebitunchanged.
1 Read: Error occurred.
Write: Clears thebit.
16.4.10 ESM ERROR PinStatus Register (ESMEPSR)
Figure 16-20. ESM ERROR PinStatus Register (ESMEPSR) [offset =24h]
31 16
Reserved
R-0
15 1 0
Reserved EPSF
R-0 R-X/1
LEGEND: R=Read only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-12. ESM ERROR PinStatus Register (ESMEPSR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 EPSF ERROR PinStatus Flag. Provides status information fortheERROR Pin.
Read/Write inUser andPrivileged mode.
0 Read: ERROR pinislow(active) ifanyerror hasoccurred.
Write: Writes have noeffect.
1 Read: ERROR pinishigh ifnoerror hasoccurred.
Write: Writes have noeffect.
Note: This flagwillbesetto1after PORRST. The value willbeunchanged after RST. The ERROR
pinstatus remains unchanged during after RST.

<!-- Page 571 -->

www.ti.com ESM Control Registers
571 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.11 ESM Interrupt Offset High Register (ESMIOFFHR)
Figure 16-21. ESM Interrupt Offset High Register (ESMIOFFHR) [offset =28h]
31 16
Reserved
R-0
15 8 7 0
Reserved INTOFFH
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 16-13. ESM Interrupt Offset High Register (ESMIOFFHR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 INTOFFH Offset High-Level Interrupt. This vector gives thechannel number ofthehighest-pending interrupt
request forthehigh-level interrupt line. Interrupts oferror Group2 have higher priority than
interrupts oferror Group1. Inside agroup, channel 0hashighest priority andchannel 31haslowest
priority.
User andprivileged mode (read):
Returns number ofpending interrupt with thehighest priority forthehigh-level interrupt line.
0 Nopending interrupt.
1h Interrupt pending forchannel 0,error Group1.
: :
20h Interrupt pending forchannel 31,error Group1.
21h Interrupt pending forchannel 0,error Group2.
: :
40h Interrupt pending forchannel 31,error Group2.
41h Interrupt pending forchannel 32,error Group1.
: :
60h Interrupt pending forchannel 63,error Group1.
61h Reserved
: :
80h Reserved
81h Interrupt pending forchannel 64,error Group1.
: :
A0h Interrupt pending forchannel 95,error Group1.
Note: Reading theinterrupt vector willclear thecorresponding flagintheESMSR2 register; willnot
clear ESMSR1 andESMSSR2 andtheoffset register gets updated.
User andprivileged mode (write):
Writes have noeffect.

<!-- Page 572 -->

ESM Control Registers www.ti.com
572 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.12 ESM Interrupt Offset Low Register (ESMIOFFLR)
Figure 16-22. ESM Interrupt Offset Low Register (ESMIOFFLR) [offset =2Ch]
31 16
Reserved
R-0
15 8 7 0
Reserved INTOFFL
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 16-14. ESM Interrupt Offset Low Register (ESMIOFFLR) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 INTOFFL Offset Low-Level Interrupt. This vector gives thechannel number ofthehighest-pending interrupt
request forthelow-level interrupt line. Inside agroup, channel 0hashighest priority andchannel 31
haslowest priority.
User andprivileged mode (read):
Returns number ofpending interrupt with thehighest priority forthelow-level interrupt line.
0 Nopending interrupt.
1h Interrupt pending forchannel 0,error Group1.
: :
20h Interrupt pending forchannel 31,error Group1.
21h Reserved
: :
40h Reserved
41h Interrupt pending forchannel 32,error Group1.
: :
60h Interrupt pending forchannel 63,error Group1.
61h Reserved
: :
80h Reserved
81h Interrupt pending forchannel 64,error Group1.
: :
A0h Interrupt pending forchannel 95,error Group1.
Note: Reading theinterrupt vector willnotclear thecorresponding flagintheESMSR1 register.
Group2 interrupts arefixed tothehigh level interrupt lineonly.
User andprivileged mode (write):
Writes have noeffect.

<!-- Page 573 -->

www.ti.com ESM Control Registers
573 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.13 ESM Low-Time Counter Register (ESMLTCR)
Figure 16-23. ESM Low-Time Counter Register (ESMLTCR) [offset =30h]
31 16
Reserved
R-0
15 0
LTC
R-3FFFh
LEGEND: R=Read only; -n=value after reset
Table 16-15. ESM Low-Time Counter Register (ESMLTCR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 LTC ERROR PinLow-Time Counter
16-bit pre-loadable down-counter tocontrol low-time ofERROR pin.The low-time counter is
triggered bytheperipheral clock (VCLK).
Note: Low time counter issettothedefault pre-load value oftheESMLTCPR inthefollowing
cases:
1.Reset (power onreset orwarm reset)
2.Anerror occurs
3.User forces anerror
16.4.14 ESM Low-Time Counter Preload Register (ESMLTCPR)
Figure 16-24. ESM Low-Time Counter Preload Register (ESMLTCPR) [offset =34h]
31 16
Reserved
R-0
15 14 13 0
LTCP LTCP
R/WP-0 R-3FFFh
LEGEND: R/W =Read/Write; R=Read; WP=Write inprivileged mode only; -n=value after reset
Table 16-16. ESM Low-Time Counter Preload Register (ESMLTCPR) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 LTCP ERROR PinLow-Time Counter Pre-load Value
16-bit pre-load value fortheERROR pinlow-time counter.
Note: Only LTCP.15 andLTCP.14 areconfigurable (privileged mode write).

<!-- Page 574 -->

ESM Control Registers www.ti.com
574 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.15 ESM Error Key Register (ESMEKR)
Figure 16-25. ESM Error KeyRegister (ESMEKR) [offset =38h]
31 16
Reserved
R-0
15 4 3 0
Reserved EKEY
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read; WP=Write inprivileged mode only; -n=value after reset
Table 16-17. ESM Error KeyRegister (ESMEKR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EKEY Error Key. The keytoreset theERROR pinortoforce anerror ontheERROR pin.
User andprivileged mode (read):
Returns current value oftheEKEY.
Privileged mode (write):
0 Activates normal mode (recommended default mode).
5h The ERROR pinsettohigh when thelowtime counter (LTC) hascompleted; then theEKEY
bitwillswitch back tonormal mode (EKEY =0000)
Ah Forces error onERROR pin.
Allother values Activates normal mode.
16.4.16 ESM Status Shadow Register 2(ESMSSR2)
This register isdedicated forGroup2.
Figure 16-26. ESM Status Shadow Register 2(ESMSSR2) [offset =3Ch]
31 16
ESF
R/W1CP-X/0
15 0
ESF
R/W1CP-X/0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-18. ESM Status Shadow Register 2(ESMSSR2) Field Descriptions
Bit Field Value Description
31-0 ESF Error Status Flag. Shadow register forstatus information onpending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred.
Write: Leaves thebitunchanged.
1 Read: Error occurred.
Write: Clears thebit.ESMSR2 isnotimpacted bythisaction.
Note: Errors arestored until they arecleared bythesoftware oratpower-on reset (PORRST).

<!-- Page 575 -->

www.ti.com ESM Control Registers
575 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.17 ESM Influence ERROR PinSet/Status Register 4(ESMIEPSR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-27. ESM Influence ERROR PinSet/Status Register 4(ESMIEPSR4) [offset =40h]
31 16
IEPSET[63:48]
R/WP-0
15 0
IEPSET[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-19. ESM Influence ERROR PinSet/Status Register 4(ESMIEPSR4) Field Descriptions
Bit Field Value Description
63-32 IEPSET Setinfluence onERROR pin.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding clear bitintheESMIEPCR4 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Enables failure influence onERROR pinandsets thecorresponding clear bitinthe
ESMIEPCR4 register.
16.4.18 ESM Influence ERROR PinClear/Status Register 4(ESMIEPCR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-28. ESM Influence ERROR PinClear/Status Register 4(ESMIEPCR4) [offset =44h]
31 16
IEPCLR[63:48]
R/WP-0
15 0
IEPCLR[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-20. ESM Influence ERROR PinClear/Status Register 4(ESMIEPCR4) Field Descriptions
Bit Field Value Description
63-32 IEPCLR Clear influence onERROR pin.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding clear bitintheESMIEPSR4 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Disables failure influence onERROR pinandclears thecorresponding clear bitinthe
ESMIEPSR4 register.

<!-- Page 576 -->

ESM Control Registers www.ti.com
576 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.19 ESM Interrupt Enable Set/Status Register 4(ESMIESR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-29. ESM Interrupt Enable Set/Status Register 4(ESMIESR4) [offset =48h]
31 16
INTENSET[63:48]
R/WP-0
15 0
INTENSET[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-21. ESM Interrupt Enable Set/Status Register 4(ESMIESR4) Field Descriptions
Bit Field Value Description
63-32 INTENSET Setinterrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding clear bitintheESMIECR4 register unchanged.
1 Read: Interrupt isenabled.
Write: Enables interrupt andsets thecorresponding clear bitintheESMIECR4 register.
16.4.20 ESM Interrupt Enable Clear/Status Register 4(ESMIECR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-30. ESM Interrupt Enable Clear/Status Register 4(ESMIECR4) [offset =4Ch]
31 16
INTENCLR[63:48]
R/WP-0
15 0
INTENCLR[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-22. ESM Interrupt Enable Clear/Status Register 4(ESMIECR4) Field Descriptions
Bit Field Value Description
63-32 INTENCLR Clear interrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding clear bitintheESMIESR4 register unchanged.
1 Read: Interrupt isenabled.
Write: Disables interrupt andclears thecorresponding clear bitintheESMIESR4 register.

<!-- Page 577 -->

www.ti.com ESM Control Registers
577 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.21 ESM Interrupt Level Set/Status Register 4(ESMILSR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-31. ESM Interrupt Level Set/Status Register 4(ESMILSR4) [offset =50h]
31 16
INTLVLSET[63:48]
R/WP-0
15 0
INTLVLSET[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-23. ESM Interrupt Level Set/Status Register 4(ESMILSR4) Field Descriptions
Bit Field Value Description
63-32 INTLVLSET Setinterrupt level.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Read: Interrupt ofchannel xismapped tolow-level interrupt line.
Write: Leaves thebitandthecorresponding clear bitintheESMILCR4 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh-level interrupt line.
Write: Maps interrupt ofchannel xtohigh-level interrupt lineandsets thecorresponding clear bitin
theESMILCR4 register.
16.4.22 ESM Interrupt Level Clear/Status Register 4(ESMILCR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-32. ESM Interrupt Level Clear/Status Register 4(ESMILCR4) [offset =54h]
31 16
INTLVLCLR[63:48]
R/WP-0
15 0
INTLVLCLR[47:32]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-24. ESM Interrupt Level Clear/Status Register 4(ESMILCR4) Field Descriptions
Bit Field Value Description
63-32 INTLVLCLR Clear interrupt level.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt ofchannel xismapped tolow-level interrupt line.
Write: Leaves thebitandthecorresponding setbitintheESMILSR4 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh-level interrupt line.
Write: Maps interrupt ofchannel xtolow-level interrupt lineandclears thecorresponding setbitin
theESMILSR4 register.

<!-- Page 578 -->

ESM Control Registers www.ti.com
578 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.23 ESM Status Register 4(ESMSR4)
This register isdedicated forGroup1 Channel[63:32].
Figure 16-33. ESM Status Register 4(ESMSR4) [offset =58h]
31 16
ESF[63:48]
R/W1CP-X/0
15 0
ESF[47:32]
R/W1CP-X/0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-25. ESM Status Register 4(ESMSR4) Field Descriptions
Bit Field Value Description
63-32 ESF Error Status Flag. Provides status information onapending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred; nointerrupt ispending.
Write: Leaves thebitunchanged.
1 Read: Error occurred; interrupt ispending.
Write: Clears thebit.
Note: After RST, ifoneofthese flags aresetandthecorresponding interrupt areenabled, the
interrupt service routine willbecalled.

<!-- Page 579 -->

www.ti.com ESM Control Registers
579 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.24 ESM Influence ERROR PinSet/Status Register 7(ESMIEPSR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-34. ESM Influence ERROR PinSet/Status Register 7(ESMIEPSR7) [offset =80h]
31 16
IEPSET[95:80]
R/WP-0
15 0
IEPSET[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-26. ESM Influence ERROR PinSet/Status Register 7(ESMIEPSR7) Field Descriptions
Bit Field Value Description
95-64 IEPSET Setinfluence onERROR pin.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding clear bitintheESMIEPCR7 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Enables failure influence onERROR pinandsets thecorresponding clear bitinthe
ESMIEPCR7 register.
16.4.25 ESM Influence ERROR PinClear/Status Register 7(ESMIEPCR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-35. ESM Influence ERROR PinClear/Status Register 7(ESMIEPCR7) [offset =84h]
31 16
IEPCLR[95:80]
R/WP-0
15 0
IEPCLR[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-27. ESM Influence ERROR PinClear/Status Register 7(ESMIEPCR7) Field Descriptions
Bit Field Value Description
95-64 IEPCLR Clear influence onERROR pin.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Failure onchannel xhasnoinfluence onERROR pin.
Write: Leaves thebitandthecorresponding clear bitintheESMIEPSR7 register unchanged.
1 Read: Failure onchannel xhasinfluence onERROR pin.
Write: Disables failure influence onERROR pinandclears thecorresponding clear bitinthe
ESMIEPSR7 register.

<!-- Page 580 -->

ESM Control Registers www.ti.com
580 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.26 ESM Interrupt Enable Set/Status Register 7(ESMIESR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-36. ESM Interrupt Enable Set/Status Register 7(ESMIESR7) [offset =88h]
31 16
INTENSET[95:80]
R/WP-0
15 0
INTENSET[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-28. ESM Interrupt Enable Set/Status Register 7(ESMIESR7) Field Descriptions
Bit Field Value Description
95-64 INTENSET Setinterrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding clear bitintheESMIECR7 register unchanged.
1 Read: Interrupt isenabled.
Write: Enables interrupt andsets thecorresponding clear bitintheESMIECR7 register.
16.4.27 ESM Interrupt Enable Clear/Status Register 7(ESMIECR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-37. ESM Interrupt Enable Clear/Status Register 7(ESMIECR7) [offset =8Ch]
31 16
INTENCLR[95:80]
R/WP-0
15 0
INTENCLR[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-29. ESM Interrupt Enable Clear/Status Register 7(ESMIECR7) Field Descriptions
Bit Field Value Description
95-64 INTENCLR Clear interrupt enable.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt isdisabled.
Write: Leaves thebitandthecorresponding clear bitintheESMIESR7 register unchanged.
1 Read: Interrupt isenabled.
Write: Disables interrupt andclears thecorresponding clear bitintheESMIESR7 register.

<!-- Page 581 -->

www.ti.com ESM Control Registers
581 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.28 ESM Interrupt Level Set/Status Register 7(ESMILSR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-38. ESM Interrupt Level Set/Status Register 7(ESMILSR7) [offset =90h]
31 16
INTLVLSET[95:80]
R/WP-0
15 0
INTLVLSET[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-30. ESM Interrupt Level Set/Status Register 7(ESMILSR7) Field Descriptions
Bit Field Value Description
95-64 INTLVLSET Setinterrupt level.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Read: Interrupt ofchannel xismapped tolow-level interrupt line.
Write: Leaves thebitandthecorresponding clear bitintheESMILCR7 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh-level interrupt line.
Write: Maps interrupt ofchannel xtohigh-level interrupt lineandsets thecorresponding clear bitin
theESMILCR7 register.
16.4.29 ESM Interrupt Level Clear/Status Register 7(ESMILCR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-39. ESM Interrupt Level Clear/Status Register 7(ESMILCR7) [offset =94h]
31 16
INTLVLCLR[95:80]
R/WP-0
15 0
INTLVLCLR[79:64]
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 16-31. ESM Interrupt Level Clear/Status Register 7(ESMILCR7) Field Descriptions
Bit Field Value Description
95-64 INTLVLCLR Clear interrupt level.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Interrupt ofchannel xismapped tolow-level interrupt line.
Write: Leaves thebitandthecorresponding setbitintheESMILSR7 register unchanged.
1 Read: Interrupt ofchannel xismapped tohigh-level interrupt line.
Write: Maps interrupt ofchannel xtolow-level interrupt lineandclears thecorresponding setbitin
theESMILSR7 register.

<!-- Page 582 -->

ESM Control Registers www.ti.com
582 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Signaling Module (ESM)16.4.30 ESM Status Register 7(ESMSR7)
This register isdedicated forGroup1 Channel[95:64].
Figure 16-40. ESM Status Register 7(ESMSR7) [offset =98h]
31 16
ESF[95:80]
R/W1CP-X/0
15 0
ESF[79:64]
R/W1CP-X/0
LEGEND: R/W =Read/Write; W1CP =Write 1toclear inprivilege mode only; -n=value after reset/ PORRST; X=value isunchanged
Table 16-32. ESM Status Register 7(ESMSR7) Field Descriptions
Bit Field Value Description
95-64 ESF Error Status Flag. Provides status information onapending error.
Read inUser andPrivileged mode. Write inPrivileged mode only.
0 Read: Noerror occurred; nointerrupt ispending.
Write: Leaves thebitunchanged.
1 Read: Error occurred; interrupt ispending.
Write: Clears thebit.
Note: After RST, ifoneofthese flags aresetandthecorresponding interrupt areenabled, the
interrupt service routine willbecalled.