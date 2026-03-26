# Error Profiling Controller (EPC)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 483-496

---


<!-- Page 483 -->

483 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)Chapter 12
SPNU563A -March 2018
Error Profiling Controller (EPC)
This chapter describes overall functionality andhow tousetheError Profiling Controller (EPC).
Topic ........................................................................................................................... Page
12.1 Overview ......................................................................................................... 484
12.2 Module Operation ............................................................................................. 484
12.3 How toUseEPC............................................................................................... 487
12.4 EPC Control Registers ...................................................................................... 488

<!-- Page 484 -->

Overview www.ti.com
484 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.1 Overview
The EPC isused asadiagnostic forfunctional safety purposes.
The primary goal ofthismodule istoprovide aunified correctable ECC error (single-bit ECC fault) profiling
capability anderror address cache onECC failures insystem busmemory slaves likeFlash, FEE, and
SRAM.
The secondary goal ofthismodule istoprovide anECC error reporting capability forbusmasters which
arenotnatively built tomanage ECC error liketheinterconnect.
The EPC cannotdistinguish between memory failure andinterconnect failure. Afault address may not
always mean there isatrue issue inthememory. EPC captures both thecorrectable anduncorrectable
information.
The EPC supports thefollowing features:
*Traps thecorrectable anduncorrectable faults from RAM, CPU, andInterconnect.
*Forcorrectable fault, EPC willkeep track ofunique addresses through theusage ofContent Address
Memory (CAM).
*Allow CPU accessing CAM tosetorclear anyCAM entry index and/or content during execution run
time aswell asdiagnosing theCAM.
*Trigger error event toError Signaling Module (ESM) andkeep track oferror instatus register foruser
query.
12.2 Module Operation
Figure 12-1 shows thetypical usage ofEPC indevice architecture. IntheEPC chapter, theCPU, RAM, or
Interconnect isreferred toasIP.The Error Profiling Module section intheArchitecture chapter indicates
which IPcorrectable anduncorrectable event arehooked uptoEPC. Each IPcanprovide either orboth
correctable anduncorrectable fault event toEPC. The EPC chapter willmention IPcorrectable or
uncorrectable fault event with generic description ofhow EPC process these fault events.
EPC captures theuncorrectable address from IPthatarenotnatively built tomanage ECC error like
interconnect andtriggers uerr_event toESM. See Section 12.2.1 formore details.
EPC performs error profiling onthecorrectable fault andtrigger serr_event toESM iftheaddress ofthe
correctable fault isnotpart oftheCAM andSERRENA control bitsaresettoenable values. Detail
description oferror profiling definition isdescribed inSection 12.2.2 .Each single fault correctable IPhasa
FIFO tobuffer correctable fault address input. Following isthebehavior ofFIFO andCAM operation:
1.IfFIFO overflow happens onaparticular ECC correctable IP,EPC willsetthecorresponding FIFO
Overflow BitintheOverflow Status Register (OVRFLWSTAT) andtrigger serr_event.
2.IfanyoftheFIFOs isfull(any FIFOFULLSTAT(x) isset), EPC willtrigger thecam_fifo_full_int port if
CAM/FIFO fullinterrupt enable (EPCCNTRL(24)) isset.
3.IfCAM indexes arealloccupied, EPC willsettheCAM FullBitinEPCERRSTAT register andtrigger
thecam_fifo_full_int port ifCAM/FIFO fullinterrupt enable isset.
4.IfCAM overflow happens, EPC willsetCAM overflow status bit(cam ovflw) inEPCERRSTAT register.
5.You canaccess CAM content andCAM index during functional anddiagnostic runtime.

<!-- Page 485 -->

CPU
IP0
IP1EPCESMserr_event
uerr_eventserr_valid
serr_addr
32
serr_valid
serr_addr
32
uerr_valid
uerr_addr
32cam_fifo_full_int
VIM
www.ti.com Module Operation
485 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)Figure 12-1. EPC System Block Diagram
12.2.1 Uncorrectable Fault Operation
EPC willcapture full32-bit addresses ofuncorrectable fault from interconnect andRAM IPmodules to
UERRADDR_(0,1) registers andsetthecorresponding uncorrectable status bitinUERRSTAT register.
The firstuncorrectable address from RAM andinterconnect IPwillbecaptured tothecorresponding
UERRADDR_(0,1) andfrozen until CPU inprivilege mode write clears tothecorresponding UERRSTAT
bit.
Whenever anystatus bitinERRSTAT justgotsetbythepresence ofanew uncorrectable fault, EPC will
trigger uncorrectable fault event toESM. The bitsinUERRSTAT canonly becleared bydevice power on
reset orCPU write clears inprivilege mode.
12.2.2 Correctable Fault Operation
12.2.2.1 Functional Mode
CPU, Interconnect, andRAM IPcantrigger correctable fault event toEPC. The EPC provides a4-entry
FIFO toeach ofthese IP(s) tocapture correctable event andits64-bit aligned addresses.
AFIFO fullcondition happens when all4entries ofaparticular FIFO areoccupied. Inthiscase, the
corresponding FIFO fullstatus bitoftheIPwillbesetinFIFOFULLSTAT register. Aninterrupt willbe
generated ifCAMFIFOFULLENA bitissetinEPCCNTRL register.
AFIFO overflow canhappen when allentries areoccupied andthere isanew correctable fault event just
arrives tothesame FIFO. Inthiscase, thenew correctable fault event andaddress willbediscarded, but
theoverflow bitremained tobeset.IftheSERRENA bitsinEPCCNTRL register areenabled, thesingle-
biterror correctable fault event willbetriggered toESM.
AFIFO fulloroverflow interrupt indicates toyouthatthere isanabnormal condition onthenumber of
correctable faults happening totheparticular IP.Itisuptoapplication software tohandle thissituation by
either putting thesystem insafe stage iftheIPcausing fulloroverflow interrupt happens tobeacritical IP
insafety application ordoing extra diagnostic ofthecorresponding IPmemory during diagnostic time. You
canwrite clears thecorresponding FIFO fulloroverflow status bitinprivilege mode.

<!-- Page 486 -->

Module Operation www.ti.com
486 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)The 64-bit aligned address ofthecorrectable fault from each IPFIFO issent totheCAM tocheck ifthe
correctable fault isunique orrepetitive. Ifitisarepetitive address forthecorrectable fault, then the
correctable fault anditsaddress arediscarded andnofurther indication totheCPU. Ifitisaunique
address, then theaddress willberemembered intheCAM content andCAM index willbesettooccupied.
Itissoftware configurable toraise anerror event toESM ifSERRENA bitsinEPCCNTRL areenable.
IfallCAM indexes areoccupied, EPC willsettheCAM fullstatus bitinEPCERRSTAT register andtrigger
aninterrupt toVIM ifCAMFIFOFULLENA bitinEPCCNTRL register isset.You caninspect CAM andset
itsindexes toavailable.
IfallCAM indexes areoccupied andthere isanew correctable fault event tobechecked, theEPC willset
theCAM overflow status bitinEPCERRSTAT register andtrigger anerror event toESM ifcorrectable
error event enable bitsaresetinEPCCNTRL register. The CAM content andindex arenotupdated when
CAM overflow happens.
Reading aCAM index value of5hindicates theCAM entry isavailable andreading avalue ofAhindicates
theCAM entry isoccupied. You canalso inspect thenumber ofCAM indexes thatarestillinavailable
state byreading theCAMAVAILSTAT register.
CAM content andindex canonly beupdated inprivilege mode.
Infunctional mode, CPU canonly setCAM index toavailable state butnotoccupied state. Occupied state
setting byCPU willbeignored.
CPU canalso update theCAM content. Inthiscase, once theCAM content isupdated, theCAM index will
auto settooccupied state, butthere isnocorrectable error event generation toESM. This ismainly used
asaway toavoid correctable error event generation forhard fault onsingle (correctable) biterror address
infunctional mode. Anexample usage would be:Assume address 0x0800_0000 hasahard fault single-bit
error intheRAM. You canwrite 0x0800_0000 totheEPC CAM content. This write willupdate the
corresponding CAM index to"occupied "byEPC hardware. You canavoid EPC generation ofsingle-bit
error event every time theCPU accessing address 0x0800_0000.
12.2.2.2 Diagnostic Mode
EPC allows youtodiagnose theCAM content, CAM index, andcorrectable event generation toensure the
CAM operates correctly andtoavoid latent fault.
You need tosetDIAG_ENA_KEY inEPCCNTRL register toAhtoenter diagnostic mode.
Once indiagnostic mode, youcanchange anyCAM index toavailable oroccupied state. Setting allCAM
indexes tooccupied willresult inCAM fullstatus bittobesetinEPCCNTRL register. Inthiscase, EPC will
generate theCAM fullinterrupt ifCAM/FIFO fullinterrupt enable isset.The NUMCAMAVAIL bitsof
CAMAVAILSTAT willalso reflect thenumber ofCAM index available when youchange theCAM index
values between available andoccupied.
Writes toCAM content willalso setthecorresponding CAM index tooccupied andtrigger correctable error
event toESM. This isdone totestthesignal chain inCAM content update forunique address and
triggering correctable event infunctional mode.

<!-- Page 487 -->

www.ti.com How toUse EPC
487 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.3 How toUseEPC
12.3.1 Functional Mode
Following steps aretherecommended sequences toinitialize EPC:
1.Setupcorrect values forSERRENA andCAMFIFO FULL ENA bitsinEPCCNTRL. Setting SERRENA
willenable correctable error event generation toESM. Setting CAMFIFO FULL ENA willenable CAM
fullorFIFO fullinterrupt totheCPU. You need tosetthese values inaccording totheir safety
application requirement.
2.Read CAMAVAILSTAT toensure thatallCAM indexes areavailable after system reset.
OnCAM orFIFO fullinterrupt, following sequences arerecommended toquery theEPC:
1.Read EPCERRSTAT andFIFOFULLSTAT todetermine ifthisisCAM orFIFO full.
a.IfitisFIFO full,theFIFOFULLSTAT indicates which IPFIFO isfullsoyoucanmake adecision on
whether toputthesystem insafe state iftheparticular IPhappens tobeacritical IPinsafety
application ordoing extra diagnostic ofthecorresponding IPRAM during diagnostic time. Clear the
FIFO fullbywrite clear totheFIFOFULLSTAT register.
b.IfitisCAM full,youneed toread CAM content tofindoutifmost ofthecorrectable fault happens
tobeinthesame IPorscatter among IPinorder totake decision onwhether toputthesystem in
safe state ordecides toruncertain RAM testduring diagnostic time. You canalso keep track of
thecorrectable fault insystem RAM inorder toclear theCAM index toavoid CAM overflow
condition.
Oncorrectable error event orCAM overflow orFIFO overflow from ESM interrupt CPU, following
sequences arerecommended toquery theEPC:
1.Read EPCERRSTAT andOVRFLWSTAT registers todetermine ifthisisCAM orFIFO overflow ora
registration ofnew correctable fault event inCAM.
a.IfitisFIFO overflow, theOVRFLWSTAT indicates which IPFIFO hasoverflow soyoucanmake
decision onwhether toputthesystem insafe state iftheparticular IPhappens tobeacritical IPin
safety application ordoing extra diagnostic ofthecorresponding IPRAM during diagnostic time.
Clear theFIFO overflow bywrite clear totheOVRFLWSTAT register.
b.IfitisCAM overflow, thatmeans youdonotservice theCAM fullinterrupt intime. You need to
read CAM content tofindoutifmost ofthecorrectable fault happens tobeinthesame IPor
scatter among IPinorder totake decision onwhether toputthesystem insafe state ordecides to
runcertain RAM testduring diagnostic time. You canalso keep track ofthecorrectable fault in
system RAM inorder toclear theCAM index toavoid CAM overflow condition.
c.Ifitisnone ofthetwoabove cases, then itisanew correctable fault event register onCAM. You
canread theCAM index registers andCAM content registers todetermine which IPRAM orRAM
location thathasthecorrectable fault anddoes aquick diagnose ofthatRAM location bybacking
uplocation content, write andread back new RAM value. Ifitisatransient fault, restores RAM
backup data andclear theCAM index. Otherwise, mark itaspermanent fault bynotclearing the
index toavailable sothatitdoes notgenerate correctable error event again.
Onuncorrectable error event, following sequences arerecommended toquery theEPC:
1.Read UERRSTAT register todetermine which IP_(n) causing uncorrectable fault.
2.Read thecorresponding UERR_ADDR_(n) todetermine thelocation ofthefault.
3.Diagnose thecorresponding location todetermine ifthisisapermanent ortransient fault. Depending
onthecriticality ofthisuncorrectable fault insafety application, itisuptoyoutobring thesystem to
safe state ornot.

<!-- Page 488 -->

How toUse EPC www.ti.com
488 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.3.2 CAM Diagnostic Mode
Inorder totesttheCAM logic anderror event generation functionality, youneed todiagnose theCAM at
diagnostic time intheir control loop.
Following sequences arerecommended todiagnose theCAM anderror event generation:
1.Configure EPC indiagnostic mode bysetting theDIAG_ENA_KEY toAhinEPCCNTRL register.
2.Backing uptheCAM content andCAM index tosystem RAM.
3.Change CAM index toavailable state from occupied state andvice versa andcheck thenumber of
CAM available status correctly reflected inCAMAVAILSTAT register aswell astheCAM index
registers correctly reflecting thenew state.
4.Write toCAM content ofanyavailable index andshould observe acorrectable error event setinESM
aswell asCAM index settooccupied.
5.Restore theCAM content andCAM index values.
6.Exitdiagnostic mode bywriting 5htoDIAG_ENA_KEY.
12.4 EPC Control Registers
The error profiling controller registers listed inTable 12-1 areaccessed through thesystem module
register space intheCortex-R5F CPUs memory-map. Allregisters are32-bit wide andarelocated ona
32-bit boundary. Reads andwrites toregisters aresupported in8-,16-, and32-bit accesses. The base
address forthecontrol registers isFFFF 0C00h.
Table 12-1. EPC Control Registers
Offset Acronym Register Description Section
00h EPCREVID EPC REVID Register Section 12.4.1
04h EPCCNTRL EPC Control Register Section 12.4.2
08h UERRSTAT Uncorrectable Error Status Register Section 12.4.3
0Ch EPCERRSTAT EPC Error Status Register Section 12.4.4
10h FIFOFULLSTAT FIFO FullStatus Register Section 12.4.5
14h OVRFLWSTAT IPInterface FIFO Overflow Status Register Section 12.4.6
18h CAMAVAILSTAT CAM Index Available Status Register Section 12.4.7
20h-24h UERRADDR Uncorrectable Error Address Registers Section 12.4.8
A0h-11Ch CAM_CONTENT CAM Content Update Registers Section 12.4.9
200h CAM_INDEX0 CAM Index Register 0 Section 12.4.10
204h CAM_INDEX1 CAM Index Register 1 Section 12.4.10
208h CAM_INDEX2 CAM Index Register 2 Section 12.4.10
20Ch CAM_INDEX3 CAM Index Register 3 Section 12.4.10
210h CAM_INDEX4 CAM Index Register 4 Section 12.4.10
214h CAM_INDEX5 CAM Index Register 5 Section 12.4.10
218h CAM_INDEX6 CAM Index Register 6 Section 12.4.10
21Ch CAM_INDEX7 CAM Index Register 7 Section 12.4.10

<!-- Page 489 -->

www.ti.com EPC Control Registers
489 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.1 EPC REVID Register (EPCREVID)
Figure 12-2. EPC REVID Register (EPCREVID) (offset =00h)
31 30 29 28 27 16
SCHEME Reserved FUNC
R-1 R-0 R-A0Ah
15 11 10 8 7 6 5 0
RTL MAJOR CUSTOM MINOR
R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after synchronous reset onsystem reset
Table 12-2. EPC REVID Register (EPCREVID) Field Descriptions
Bit Field Value Description
31-30 SCHEME 1 Identification scheme.
29-28 Reserved 0 Reserved. Reads return 0.
27-16 FUNC A0Ah Indicates functionally equivalent module family.
15-11 RTL 0 RTL version number.
10-8 MAJOR 0 Major revision number.
7-6 CUSTOM 0 Indicates device-specific implementation.
5-0 MINOR 0 Minor revision number.

<!-- Page 490 -->

EPC Control Registers www.ti.com
490 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.2 EPC Control Register (EPCCNTRL)
Figure 12-3. EPC Control Register (EPCCNTRL) (offset =04h)
31 25 24 23 16
Reserved CAM/FIFO_
FULL_ENAReserved
R-0 R/WP-0 R-0
15 12 11 8 7 4 3 0
Reserved DIA_ENA_KEY Reserved SERRENA
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after synchronous reset onsystem reset
Table 12-3. EPC Control Register (EPCCNTRL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reserved. Reads return 0.
24 CAM/FIFO_FULL_ENA CAM orFIFO fullinterrupt enable. IfthisbitissetandCAM isfull,CAM FullInterrupt
isgenerated.
Read:
0 CAM/FIFO fullinterrupt isdisabled.
1 CAM/FIFO fullinterrupt isenabled.
Write inPrivilege:
0 Disable CAM/FIFO fullinterrupt.
1 Enable CAM/FIFO fullinterrupt.
23-12 Reserved 0 Reserved. Reads return 0.
11-8 DIA_ENA_KEY CAM diagnostic enable key. These bits(when enabled) allow theCPU toaccess the
CAM content toclear orsetanyentry (CAM index) orwrite anypattern toCAM
content.
Internal RTL willimplement self-correction logic toavoid single bitflipping.
Read:
5h CAM diagnostic isdisabled.
Ah CAM diagnostic isenabled.
Allother values Reserved
Write inPrivilege:
5h CAM diagnostic isdisabled.
Ah CAM diagnostic isenabled.
Allother values Reserved
7-4 Reserved 0 Reserved. Reads return 0.
3-0 SERRENA Single (correctable) biterror event enable. These bits(when enable) cause EPC to
generate theserr_event ifthere isacorrectable ECC fault address arrives from oneof
theEPC-IP interface andtheCAM hasanempty entry. These bitsalso allow EPC to
generate theserr_event ifthere isacorrectable ECC fault address arrives from oneof
theEPC-IP interface andtheCAM isfull.Inthiscase, CAM FULL status bitissetin
EPCERRSTAT.
Internal RTL willimplement self-correction logic toavoid single bitflipping.
Read:
5h serr_event generation isdisabled.
Ah serr_event generation isenabled.
Allother values Reserved
Write inPrivilege:
5h serr_event generation isdisabled.
Ah serr_event generation isenabled.
Allother values Reserved

<!-- Page 491 -->

www.ti.com EPC Control Registers
491 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.3 Uncorrectable Error Status Register (UERRSTAT)
Figure 12-4. Uncorrectable Error Status Register (UERRSTAT) (offset =08h)
31 16
Reserved
R-0
15 2 1 0
Reserved UE1 UE0
R-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after asynchronous reset by
power-on reset
Table 12-4. Uncorrectable Error Status Register (UERRSTAT) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved. Reads return 0.
1-0 UEn Uncorrectable ECC Fault Status Bitforinterface n.Each bitcorresponds tooneuncorrectable EPC-IP
interface. IftheIPtriggers uncorrectable error, oneofthese bitsgets set.Once itisset,itcanonly be
cleared bypower-on reset orCPU write-clear inprivilege mode orbyreading thecorresponding
UERRADDR register. Any ofthese bitssetcauses anuncorrectable error event (uerr_event) tobe
triggered toESM.
The number ofimplemented bitsdepends onthenumber ofimplemented EPC IPuncorrectable address
ports. Unimplemented bitsarereserved andarenotwritable. Reserved bitsareread as0.
Read:
0 Uncorrectable ECC fault status bitisnotactive forinterface n.
1 Uncorrectable ECC fault status bitisactive forinterface n.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.

<!-- Page 492 -->

EPC Control Registers www.ti.com
492 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.4 EPC Error Status Register (EPCERRSTAT)
Figure 12-5. EPC Error Status Register (EPCERRSTAT) (offset =0Ch)
31 16
Reserved
R-0
15 3 2 1 0
Reserved CAM_FULL BUS_ERR CAM_OVFLW
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after asynchronous reset by
power-on reset
Table 12-5. EPC Error Status Register (EPCERRSTAT) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reserved. Reads return 0.
2 CAM_FULL CAM fullstatus bit.This bitissetwhen CAM hasnomore available index available toaccept new
correctable address.
Read:
0 CAM isnotfull.
1 CAM isfull.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.
1 BUS_ERR MMR interface buserror status bit.This bitissetifMMR interface receives unsupported bus
commands likeReadLink-WriteConditional.
Read:
0 NoMMR buserror.
1 MMR unsupported buscommand isdetected.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.
0 CAM_OVFLW CAM overflow status bit.CAM isfullandthere isanother correctable address arrives.
Read:
0 NoCAM overflow.
1 CAM overflow isdetected.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.

<!-- Page 493 -->

www.ti.com EPC Control Registers
493 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.5 FIFO FullStatus Register (FIFOFULLSTAT)
Figure 12-6. FIFO FullStatus Register (FIFOFULLSTAT) (offset =10h)
31 16
Reserved
R-0
15 5 4 3 2 1 0
Reserved FULL4 FULL3 FULL2 FULL1 FULL0
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after asynchronous reset by
power-on reset
Table 12-6. FIFO FullStatus Register (FIFOFULLSTAT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reserved. Reads return 0.
4-0 FULL n FIFO interface nisfull.Ifthere isaFIFO fulloccurs onaparticular interface, thecorresponding bitis
set.Ifanyofthese bitsissetandtheCAM/FIFO fullena(enabled) bitsareset,EPC triggers
cam_fifo_full_int.
The number ofimplemented bitsdepends onthenumber ofimplemented EPC IPcorrectable address
ports. Unimplemented bitsarereserved andarenotwritable. Reserved bitsareread as0.
Read:
0 FIFO interface nisnotfull.
1 FIFO interface nfulloccurred.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.

<!-- Page 494 -->

EPC Control Registers www.ti.com
494 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.6 IPInterface FIFO Overflow Status Register (OVRFLWSTAT)
Figure 12-7. IPInterface FIFO Overflow Status Register (OVRFLWSTAT) (offset =14h)
31 16
Reserved
R-0
15 5 4 3 2 1 0
Reserved OVFL4 OVFL3 OVFL2 OVFL1 OVFL0
R-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after asynchronous reset by
power-on reset
Table 12-7. IPInterface FIFO Overflow Status Register (OVRFLWSTAT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reserved. Reads return 0.
4-0 OVFL n Correctable EPC-IP interface nFIFO overflow. Each bitcorresponds toonecorrectable EPC-IP
interface FIFO status. Ifthere isaFIFO overflow occurs, thisbitisset.Ifanyofthese bitsissetandthe
FIFO overflow interrupt enable bitisset,EPC triggers FIFO overflow interrupt.
The number ofimplemented bitsdepends onthenumber ofimplemented EPC IPcorrectable address
ports. Unimplemented bitsarereserved andarenotwritable. Reserved bitsareread as0.
Read:
0 NoFIFO overflow.
1 FIFO overflow occurred.
Write inPrivilege:
0 Noeffect.
1 Clear thisflagbit.
12.4.7 CAM Index Available Status Register (CAMAVAILSTAT)
Figure 12-8. CAM Index Available Status Register (CAMAVAILSTAT) (offset =18h)
31 16
Reserved
R-0
15 6 5 0
Reserved NUMCAMAVAIL
R-0 R-20h
LEGEND: R=Read only; -n=value after synchronous reset onsystem reset
Table 12-8. CAM Index Available Status Register (CAMAVAILSTAT) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reserved. Reads return 0.
5-0 NUMCAMAVAIL Number ofcurrent available CAM index. These bitsindicate (binary encoded value) thenumber
ofcurrently available CAM index.
0 Reserved
1h 1CAM index isavailable.
2h 2CAM index isavailable.
: :
20h 32CAM index isavailable.

<!-- Page 495 -->

www.ti.com EPC Control Registers
495 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.8 Uncorrectable Error Address Register n(UERR_ADDR)
Figure 12-9. Uncorrectable Error Address Register n(UERR_ADDR) (offset =20h-24h)
31 0
UERR_ADDR
R-0
LEGEND: R=Read only; -n=value after asynchronous reset onpower-on reset
Table 12-9. Uncorrectable Error Address Register n(UERR_ADDR) Field Descriptions
Bit Field Description
31-0 UERR_ADDR Register ncorresponds touncorrectable portn.The number ofuncorrectable ports isconfigured through
thegeneric parameter: Number ofEPC uncorrectable ports.
This 32-bit register captures theuncorrectable address error from each EPC-IP uncorrectable interface.
Once EPC-IP interface receives theuerr_valid_x, thecorresponding address iscaptured andfrozen. CPU
read (privilege mode) ofthisaddress unfreezes thisregister. EMUDBG read access isnon-intrusive (not
unfreeze). Power-on reset, write-clear tothecorresponding UERRSTAT bitorreading oftheERRADDR
register also unfreezes thisregister foreach interface. Unfreeze means thattheregister content canbe
updated whenever there isthenext uncorrectable error address becomes active onthisinterface.
12.4.9 CAM Content Update Register n(CAM_CONTENT)
Figure 12-10. CAM Content Update Register n(CAM_CONTENT) (offset =A0h-11Ch)
31 16
CAM_CONTENT
R/WP-0
15 3 2 0
CAM_CONTENT Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after synchronous reset onsystem reset
Table 12-10. CAM Content Update Register n(CAM_CONTENT) Field Descriptions
Bit Field Value Description
31-3 CAM_CONTENT CAM content register n.CPU writes tothisfield infunctional ordiagnostic mode. The write data is
masked with byten andstored intoCAM oneach index. Address A0h corresponds toindex 0;
address 11Ch corresponds toindex 31.The number ofactive registers changes depending onthe
number ofCAM indexes available upon configuration during device integration.
2-0 Reserved 0 Reserved. Reads return 0.

<!-- Page 496 -->

EPC Control Registers www.ti.com
496 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedError Profiling Controller (EPC)12.4.10 CAM Index Registers (CAM_INDEX[0-7])
Figure 12-11. CAM Index Registers (CAM_INDEXn) (offset =200h-21Ch)
31 28 27 24 23 20 19 16
Reserved index n×4+3 Reserved index n×4+2
R-0 R/WP-5h R-0 R/WP-5h
15 12 11 8 7 4 3 0
Reserved index n×4+1 Reserved index n×4
R-0 R/WP-5h R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after synchronous reset onsystem reset
Table 12-11. CAM Index Registers (CAM_INDEXn) Field Descriptions
Field Value Description
Reserved 0 Reserved. Reads return 0.
index n Entry valid tagforindex n.See Table 12-12 .This isthe4-bit keythatdetermines whether thecurrent
entry inCAM isoccupied ornot.Awrite (privilege mode) toA0h to11Ch indiagnostic mode or
functional mode does notonly update theCAM content incorresponding index butalso updates the
corresponding bitfield intheentry valid tags registers CAM_INDEXn.
The index hasaselfcorrection mechanism asfollows:
*Key active ifvalid key=1010 or1011 or1000 or1110 or0010
*Key inactive ifvalid key=0101 or0100 or0111 or1101 or0001
Read:
5h Entry isclear andavailable forfuture CAM usage.
Ah Entry isoccupied.
Allother values Reserved
Write inDiagnostic Mode:
5h Entry iscleared andavailable forfuture CAM usage.
Ah Entry issetandoccupied.
Allother values Reserved
Write inFunctional Mode:
5h Entry iscleared andavailable forfuture CAM usage.
Allother values Reserved
Table 12-12. CAM Index Register n
Address Offset CAM Index RegisterCAM Index Register Bits
Bits 27-24 Bits 19-16 Bits 11-8 Bits 3-0
200h CAM Index Register 0 index 3 index 2 index 1 index 0
204h CAM Index Register 1 index 7 index 6 index 5 index 4
208h CAM Index Register 2 index 11 index 10 index 9 index 8
20Ch CAM Index Register 3 index 15 index 14 index 13 index 12
210h CAM Index Register 4 index 19 index 18 index 17 index 16
214h CAM Index Register 5 index 23 index 22 index 21 index 20
218h CAM Index Register 6 index 27 index 26 index 25 index 24
21Ch CAM Index Register 7 index 31 index 30 index 29 index 28