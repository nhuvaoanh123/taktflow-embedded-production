# System Memory Protection Unit (NMPU)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 460-482

---


<!-- Page 460 -->

460 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)Chapter 11
SPNU563A -March 2018
System Memory Protection Unit (NMPU)
This chapter describes theSystem Memory Protection Unit (NMPU).
Topic ........................................................................................................................... Page
11.1 Overview ......................................................................................................... 461
11.2 Module Operation ............................................................................................. 463
11.3 How toUseNMPU ............................................................................................ 466
11.4 NMPU Registers ............................................................................................... 471

<!-- Page 461 -->

www.ti.com Overview
461 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.1 Overview
The System Memory Protection Unit module(s) provide anmechanism tocontrol thememory access
rights ofbusmasters inthesystem other than thehost CPU. The programmer's model fortheSystem
Memory Protection unitissimilar tobutasubset ofthehost CPU's own memory protection unit. Itallows
memory partition intomultiple regions andallows individual access protection foreach region from abus
master point ofview. Anaccess from busmaster ischecked against each memory region access
permission tomake sure thattheaccess from busmaster does notalter theunintended memory region
thatcould cause asystem failure.
11.1.1 Features
NMPU offers thefollowing main features:
*Software programmer model issimilar tobutasubset ofthehost CPUs own memory protection unit.
*Provide protection tomemory regions ranging from 32-bytes to4GB insize
*Upto8memory protection regions. Note thatthenumber ofmemory region isdifferent foreach bus
master IPthattheNMPU isdedicated for.Each region isdefined bythebase address andregion size
thatareprogrammable inNMPU control registers. Table 11-1 defines thenumber ofregion available
forthecorresponding busmaster IP.
*Programmable access permissions foreach region such asfullaccess, read-only, write-only, andno
access.
*Different access permissions foruser andprivilege mode.
*Onaccess violation, NMPU cannotify ESM ifERRENA keyinMPUCTRL1 register (Section 11.4.7 )is
enabled.
Table 11-1. NMPU Region
NMPU Module Number ofAvailable Regions
DMA-NMPU 8
Peripheral Interconnect Subsystem-NMPU 8
EMAC-NMPU 2
11.1.2 Safety Diagnostic
NMPU offers thefollowing safety diagnostic capabilities:
*Provide alock mechanism toavoid unintentional changes toNMPU control registers.
*Provide diagnostic capability tocheck theMPU region access permission logic.

<!-- Page 462 -->

 0Diagnostic 
LogicMPU Register 
Block
Address and Access 
Permission Comparator 7Error Pulse 
and 
Response 
Generation...
...Input Bus Master Interface
Output Bus Interconnect 
InterfacePriority 
Mux
Priority 
MuxfailcontrolDiag modeInt addr
Error
Overview www.ti.com
462 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.1.3 Block Diagram
Figure 11-1 shows theblock diagram ofNMPU.
Figure 11-1. NMPU Block Diagram

<!-- Page 463 -->

www.ti.com Module Operation
463 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.2 Module Operation
11.2.1 Functional Mode
Onreset, NMPU isdisabled andnofiltering willbedone onthebus. User must ensure thatnobus
transaction from themaster isongoing while NMPU isgetting disabled orenabled. This issimilar tothe
need toflush transactions using memory barrier instructions ontheCPU before changing CPU MPU
setting.
The MPU canbeenabled bywriting 0xAh totheMPUENA keybitsofMPUCTRL1 register andcanbe
disabled bywriting 0x5h tothesame bits.
Access permission (AP) foreach MPU region isdefined inAPfield intheMPU region access control
register (MPUREGACR), seeTable 11-2 .
Table 11-2. Access Permission
APField Privilege Mode Permissions User Mode Permissions
000 NoAccess NoAccess
001 Read/Write NoAccess
010 Read/Write Read Only
011 Read/Write Read/Write
100 NoAccess NoAccess
101 Read only NoAccess
110 Read only Read only
111 NoAccess NoAccess
Each MPU region hasthree control registers:
*MPUREGBASE: MPU based address register. Itdefines thebase address foraparticular MPU region
*MPUREGSENA: MPU region size andenable register. Itdefines thesize ofaparticular MPU region
andallows youtoenable theregion
*MPUREGACR: MPU region access control register. Itdefines theMPU region accessing permission
foruser orprivilege mode
NMPU hasoneregion register thatyouhave toconfigure todetermine which MPU region user is
programming thecorresponding MPUREGBASE, MPUREGSENA, andMPUREGACR registers. Inthis
scheme, theMPUREGBASE, MPUREGSENA, andMPUREGACR canshare thesame memory map
offset from user programming point ofview.
Size ofeach MPU region canvary from 32bytes to4GB. Region based address must start atanoffset
thatisamultiple ofregion size. Incase thebase address does notstart atanoffset thatisamultiple of
region size, theregion size takes priority andMPU ignores theLSB bitsofbase address.
Overlapping regions enable efficient programming ofmemory map. When theincoming address hits
multiple MPU regions, access permission isdecided bythehighest numbered region forwhich there was
anaddress compare match. InMPU configuration with 8regions, region 0hasthelowest priority and
region 7hasthehighest priority.
Figure 11-2 shows how theregion priority isresolved inahigh-level abstraction.
MPU does notsupport default background memory map. Ifmemory protection isenabled without region
configuration, alltransactions willresult inbuserror response. Before theprotection unitisenabled, care
needs tobetaken toensure thatatleast onevalid protection region isspecified anditsaccess permission
fields aredefined.

<!-- Page 464 -->

Region N-1
Enable?Region N-1
Address
MatchRegion N-1
Permission
Match
Region N-2
Enable?Region N-2
Address
MatchRegion N-2
Permission
Match
Region 0
Enable?Region 0
Address
MatchRegion 0
Permission
MatchYes Yes
Yes Yes
Yes YesNo
No
No No
Update ERRSTAT and 
ERRADDR.
Set BGERR/Redirect Access 
to NULL SlaveUpdate ERRSTAT and 
ERRADDR.
Set APERR/Redirect Access 
to NULL SlaveNo
No
No
Bus Output to 
InterconnectYes
Yes
YesHighest 
Priority
Lowest 
Priority
Module Operation www.ti.com
464 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)Figure 11-2. MPU Region Priority

<!-- Page 465 -->

www.ti.com Module Operation
465 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.2.2 Diagnostic Mode
Diagnostic mode canbeused toverify theMPU address andaccess permission comparator logic.
Entering orexiting thediagnostic mode willautomatically clear theMPUERRSTAT andMPUERRADDR
registers. Memory protection must bedisabled while entering orexiting diagnostic mode. There aretwo
different diagnostic modes: internal diagnostic mode andexternal diagnostic mode.
11.2.2.1 Internal Diagnostic Mode
Ininternal diagnostic mode, diagnostic logic inside theNMPU module drives theinput oftheMPU address
andaccess permission comparator logic. You canprogram theaddress forwhich comparison needs tobe
performed andthetype oftransaction (read/write anduser/privilege). Forevery write tothe
MPUDIAGADDR register, anaddress andaccess permission comparison isperformed andtheresults are
stored inMPUERRSTAT andMPUERRADDR registers. ERROR output toESM willbegenerated if
ERRENA keyinMPUCTRL2 register isAh.You must ensure thatnobustransactions from themaster are
going onwhile NMPU isininternal diagnostic mode. NMPU does notaccept anyaccess originated from
thebusmaster andensures thattheinternal diagnostic logic willnotresult inanybustransactions onto
thebusinterconnect.
How tousetheinternal diagnostic mode isdiscussed inSection 11.3.
11.2.2.2 External Diagnostic Mode
Inexternal diagnostic mode, theactual busmaster initiates theaccess totheNMPU. Address ofthe
access from thebusmaster isreplaced bytheaddress inMPUDIAGADDR register before theaddress
reaches theaddress comparator logic. Inthismode, both buserror response andERROR pulse toESM
(ifERRENA =Ah)aregenerated foraccesses thatviolate theaccess permissions. This diagnostic mode
isuseful totestthefullsignal chain from busmaster access generation logic toNMPU comparator logic.
How tousetheexternal diagnostic mode isdiscussed inSection 11.3.
11.2.3 Functional FailSafe
Since NMPU module check andmanipulate address ormode ofbusmaster transaction, itisimportant to
have functional failsafe features toguarantee thatfaults inMPU region checking, address translation, or
user mode translation canbedetected.
11.2.3.1 Run-time Diagnostics forFunctional Features
Since features likeinput address masking, address translation andmode translation areintegrated along
with acritical function likememory protection, NMPU needs tohave thefollowing hardware logic forrun-
time diagnostics. This logic isimplemented using 1oo1D safety architecture.
*There aretwoindependent blocks (primary andchecker) running inlock-step andcompare address
masking output every cycle. Outputs from NMPU aredriven bytheprimary block.
*There aretwoindependent blocks (primary andchecker) running inlock-step andcompare address
translation output every cycle. Outputs from NMPU aredriven bytheprimary block.
*There aretwoindependent blocks (primary andchecker) running inlock-step andcompare mode
translation output every cycle. Outputs from NMPU aredriven bytheprimary block.
*Ifthere isalockstep comparison error, DIAGERR bitinMPUERRSTAT register issetto1.ERRFLAG
bitinthesame register isalso set.ERROR pulse output toESM isgenerated irrespective ofERRENA
keyvalue inMPUCTRL2 register.
*Afault insertion allows user verifying thattheindividual lockstep comparator logic isfunctional and
avoid latent fault. User canprogram thefault insertion bitsinMPUDIAGCTRL register tointroduce a
fault inoneofthelockstep comparator inputs forinput address masking, address translation ormode
translation during start uporshut down ofthedevice.

<!-- Page 466 -->

How toUse NMPU www.ti.com
466 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.2.3.2 Lock Feature forNMPU
Lock feature prevents unintentional updates toNMPU registers. Writes toregisters other than
MPUERRSTAT ispossible only when NMPU isunlocked bywriting 0xAh toLOCK keybitsofMPULOCK
register. Onreset, these bitsaresetto0x5h andhence theNMPU registers areinthelocked state. All
NMPU registers arewritable only inprivilege mode. There isnobuilt inprotection based onmaster ID.Itis
user responsibility toensure thatonly asingle valid privilege master updates theMPU registers.
11.2.3.3 Multibit Keys forFeature Enable/Disable
4-bit keyisused toprotect critical function enters enable ordisable state from softerror. These keyare
updated only ifthewrite data is0x5h or0xAh. Register write isignored forallother write values. Abuilt in
correction logic detects single bitsofterror onthisfield andcorrects thevalue inthenext cycle.
Functionality andregister read data remain thesame during thecorrection cycle.
11.3 How toUseNMPU
11.3.1 How toUse NMPU inFunctional Mode
The NMPU isused toconfigure thebusmaster MPU region insuch away thatthebusmaster does not
interfere with thememory region reserved forother tasks andnotbelonging tothesystem partitioning for
theIP.
Once user determines thearchitectural memory partitioning oftheIPbusmasters onmemory system
frame according totheir application, user should configure thecorresponding MPU region foreach bus
master accordingly.
Figure 11-3 shows theexample recommended memory setting forabusmaster inthedevice, for
example, DMA.
Assume theDMA busmaster has3MPU regions.
The lowest priority MPU region1 isprogrammed toenable fullread andwrite toperipheral memory frame.
MPU region 2isprogrammed toallow read andwrite toalower 10KB portion ofthesystem RAM starting
from 0x0800_0000.
MPU region 3isprogrammed toallow read andwrite totheupper 10KB away from 0x0843_FFFF portion
ofthesystem RAM.
Any access inbetween 0x0800_2800 and0x0843_D7FF isaread only mode forDMA.
With thisconfiguration, DMA canhave read orwrite access totheentire peripheral frame andonly able to
write toupper orlower 10KB ofthesystem RAM.
The restofthesystem RAM isreserved forother tasks inwhich theDMA should notinterfere with.

<!-- Page 467 -->

R R
RWRWRW
Peripheral
FramePeripheral
Frame
RAM
Flash (3MB)
0x0000_00000x002F_FFFF0x0800_00000x0843_FFFFRegion 0
Full Read
Write of
Peripheral 
FrameRegion 1
Write only
First 10 KBRegion 2
Write only
Last 10 KB
0xF07F_FFFF0xFFFF_FFFF
Region 1
Write:
0x0800_0000
0x0800_2800Region 2
Write:
0x0843_D7FF
0x0843_FFFF
www.ti.com How toUse NMPU
467 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)Figure 11-3. Example ofDMA 3MPU Region SetUp
This willallow DMA tobeable tocreate transfer from anylocation within peripheral frame toaspecific
allocation insystem RAM toavoid corrupting thesystem memory RAM reserved forother tasks.
Following istherecommended generic software sequence tosetup theMPU regions:
1.Make sure thebusmaster isidleandnotsending anytransaction. Please follow thebusmaster TRM
onhow toidlethebusinterface. itwillbedifferent from onebusmaster toanother.
2.Write 0xA tounlocked field LOCK ofMPULOCK register (Section 11.4.2 )toallow update toNMPU
control registers.
3.Enable MPU error pulse event toESM bywriting 0xA tofield ERRENA field ofMPUCTRL2 register
(Section 11.4.8 ).Program thisstep ifandonly ifthebusmaster hasnocapability tocapture theMPU
transaction error from NMPU. Ifbusmaster hastheability toreport transaction error, disable the
ERRENA. Software willrelyonbusmaster totrigger error event causing interrupt totheCPU.
4.Read MPUTYPE register (Section 11.4.9 )toidentify how many regions areimplemented forthisbus
master inaparticular device.
5.Program theMPUREGNUM register (Section 11.4.13 )toindicate thatMPU region number towrite
starting address, size, permission, andsoon.
6.Program theMPUREGBASE register (Section 11.4.10 )tosetthebase address fortheparticular MPU
region number thatwas setinstep 5.

<!-- Page 468 -->

How toUse NMPU www.ti.com
468 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)7.Program theMPUREGSENA register (Section 11.4.11 )tosetthesize, andenable MPU region number
indicated bystep 5above. Notice thatthisisnotyetenabling theNMPU module.
8.Program theMPUREGACR register (Section 11.4.12 )tosetaccess permission foruser orprivilege
mode.
9.Repeat steps 5to8fortheremaining MPU regions available fortheparticular NMPU instance.
10.Write 0xA toMPUENA field ofMPUCTRL1 register (Section 11.4.7 )toenable NMPU tostart access
permission check.
11.Write 0x5tounlocked keyfield LOCK ofMPULOCK register (Section 11.4.2 )toavoid unintentional
write toNMPU configuration registers.
12.Enable busmaster tostart transaction. Please follow thebusmaster TRM onhow tostart thebus
interface. itwillbedifferent from onebusmaster toanother.
During application runtime, iftheNMPU detects amemory access violation orthefunctional failsafe logic
detects alock step compare error, youwillbeinterrupted. Refer toTRM tofindoutwhich ESM group and
channel theNMPU orbusmaster error event ismapped (based onstep 3above) andwhich VIM channel
theESM interrupt output ismapped.
Following istherecommended generic software sequence tofindoutwhat causes MPU error:
1.Read theMPUERRSTAT register:
*IfRERR (read error), WERR (write error), BGERR (background error), orAPERR (access
permission error) issetandERRFLAG isalso set,thisindicates thatthebusmaster tries toaccess
theaddress location thatviolates thememory protection setting. This canhappen duetosoftware
bug, transient fault, orpermanent fault.
*IfDIAGERR (safety diagnostic error) bitissetandERRFLAG isalso set,thisindicates thatthe
1oo1D diagnostic architect forinput address masking, address translation, ormode translation has
detected anerror. This canhappen duetotransient fault orpermanent fault.
2.You canfurther read additional information from REGION, MASTERID, orERRFLAG bitfields of
MPUERRSTAT register andMPUERRADDR register tonarrow down thecauses.
3.Infault case, itisuptoendapplication todecide onwhether tobring thesystem tosafe state orignore
NMPU error ortrytorecover byretrying transaction ifmaster isable tosupport it.Ifbusmaster does
notsupport retry ofaparticular transaction, usecanhaltbusmaster, starts NMPU internal diagnostic
(see Section 11.2.2.1 ).Assume diagnostic result passes, youcanrestart busmaster operation. Ifthe
recover attempt stillfails, youcandecide tobring thesystem tosafe state.
11.3.2 How toUse Diagnostics
Diagnostic mode canbeused toverify theMPU address andaccess permission comparator logic working
properly ateither start uptime orduring application runtime with/without error encountered. This is
achieved byinternal orexternal diagnostic mode. Entering orexiting thediagnostic mode willautomatically
clear theMPUERRSTAT (Section 11.4.5 )andMPUERRADDR (Section 11.4.6 )registers. Itis
recommended thatyouback upthevalues ofMPUERRSTAT (Section 11.4.5 )andMPUERRADDR
(Section 11.4.6 )registers tosystem RAM prior tostart diagnostic during runtime.
Another diagnostic mode ofNMPU isforfunctional failsafe diagnostic. Features likeinput address
masking, address translation andmode translation areintegrated along with acritical function likememory
protection, thus NMPU needs tohave thehardware logic forrun-time diagnostics. This logic is
implemented using 1oo1D safety architecture.

<!-- Page 469 -->

www.ti.com How toUse NMPU
469 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.3.2.1 How toRun Internal Diagnostic Mode
Ininternal diagnostic mode, diagnostic logic inside theNMPU module drives theinput oftheMPU address
andaccess permission comparator logic. You canprogram theaddress forwhich comparison needs tobe
performed andthetype oftransaction (read/write anduser/privilege). Forevery write tothe
MPUDIAGADDR register, anaddress andaccess permission comparison isperformed andtheresults are
stored inMPUERRSTAT andMPUERRADDR registers. ERROR output toESM willbegenerated if
ERRENA field inMPUCTRL2 register issetto0xAh. The following istherecommended sequence for
internal diagnostic mode:
1.User must ensure thatnobustransactions from themaster aregoing onwhile NMPU isininternal
diagnostic mode bydisabling busmaster access. Please follow thebusmaster TRM onhow toidlethe
businterface. itwillbedifferent from onebusmaster toanother.
2.Unlock theMPU registers bywriting 0xA toLOCK field ofMPULOCK register.
3.Disable memory protection bywriting 0x5toMPUENA keyofMPUCTRL1 register.
4.Program thedifferent MPU regions inMPUREGBASE0-7, MPUREGSENA0-7 andMPUREGACR0-7
registers.
5.InMPUDIAGCTRL register, program theINT/EXT bitas0.
6.Enable theDIAGKEY inMPUDIAGCTRL register bywriting 0xA.
7.Program thediagnostic transaction type asread/write inR/W bitinMPUDIAGCTRL register.
User/Privilege mode issetinU/Pbitinthesame register.
8.Program thediagnostic address inMPUDIAGADDR register.
9.Ifthere should beanaccess permission violation according tothediagnostic test, error flagisset.
10.Read theMPUERRSTAT andMPUERRADDR registers andverify theexpected results.
11.Clear theERRFLAG bitinMPUERRSTAT register.
12.Repeat steps 6to10fordifferent values ofdiagnostic address andR/W bit.
13.Exitthediagnostic mode bywriting 0x5toDIAGKEY field inMPUDIAGCTRL register.
14.Lock theMPU registers bywriting 0xA toLOCK field ofMPULOCK register.
15.User enables busmaster. Please follow thebusmaster TRM onhow toenable thebusinterface. itwill
bedifferent from onebusmaster toanother.

<!-- Page 470 -->

How toUse NMPU www.ti.com
470 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.3.2.2 How toRun External Diagnostic Mode
Inexternal diagnostic mode, theactual busmaster initiates thememory transaction. Address from thebus
master isreplaced bytheaddress inMPUDIAGADDR register before theaddress reaches theaddress
comparator logic. Inthismode, both buserror response andERROR pulse toESM (ifERRENA field of
MPUCTRL2 register issetto0xAh) aregenerated foraccesses thatviolate theaccess permissions. This
mode isuseful foracomplete signal chain testing from busmaster internal busgeneration logic toNMPU
comparator logics. The following istherecommended sequence forexternal diagnostic mode:
1.User must ensure thatnobustransactions from themaster aregoing onwhile NMPU isconfiguring in
external diagnostic mode bydisabling busmaster access. Please follow thebusmaster TRM onhow
toidlethebusinterface. itwillbedifferent from onebusmaster toanother.
2.Unlock theMPU registers bywriting 0xA toLOCK field ofMPULOCK register.
3.Disable memory protection bywriting 0x5toMPUENA field ofMPUCTRL1 register.
4.Program thedifferent MPU regions inMPUREGBASE0-7, MPUREGSENA0-7 andMPUREGACR0-7
registers.
5.InMPUDIAGCTRL register, program theINT/EXT bitas1.
6.Enable theDIAGKEY field ofMPUDIAGCTRL register bywriting 0xA.
7.Program thediagnostic address inMPUDIAGADDR register.
8.Enable memory protection bywriting 0xA toMPUENA field ofMPUCTRL1 register.
9.User enables busmaster. Please follow thebusmaster TRM onhow toenable thebusinterface. itwill
bedifferent from onebusmaster toanother.
10.Initiate ONE bustransactions using theactual busmaster.
11.Read theMPUERRSTAT andMPUERRADDR registers andverify theexpected results.
12.Clear theERRFLAG bitinMPUERRSTAT register.
13.Repeat steps 7,10, 11,12fordifferent values ofdiagnostic address.
14.Disable memory protection bywriting 0x5toMPUENA field ofMPUCTRL1 register.
15.Exitthediagnostic mode bywriting 0x5toDIAGKEY field inMPUDIAGCTRL register.
16.Lock theMPU registers bywriting 0xA toLOCK field ofMPULOCK register.
17.Restart thebusmaster functional operation. Please follow thebusmaster TRM onhow toenable the
businterface. itwillbedifferent from onebusmaster toanother.

<!-- Page 471 -->

www.ti.com NMPU Registers
471 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4 NMPU Registers
The new memory protection unit(NMPU) registers listed inTable 11-3 areaccessed through thesystem
module register space intheCortex-R5F CPUs memory-map. Allregisters are32-bit wide andarelocated
ona32-bit boundary. Reads andwrites toregisters aresupported in8-,16-, and32-bit accesses. Refer to
thedevice specific datasheet forthebase address ofeach instance ofNMPU inthedevice.
NOTE: Ifaregister isnotimplemented, corresponding address location behaves likeareserved
location, thatis,reads return 0andwrites have noeffect. User mode writes toNMPU
registers areignored. Noerror response isgiven forsuch anaccess. Writes toregisters
other than MPUERRSTAT register areignored, when NMPU registers arelocked (LOCK =
5hinMPULOCK register). Noerror response isgiven forsuch anaccess.
Table 11-3. NMPU Registers
Offset Acronym Register Description Section
00h MPUREV MPU Revision IDRegister Section 11.4.1
04h MPULOCK MPU Lock Register Section 11.4.2
08h MPUDIAGCTRL MPU Diagnostics Control Register Section 11.4.3
0Ch MPUDIAGADDR MPU Diagnostic Address Register Section 11.4.4
10h MPUERRSTAT MPU Error Status Register Section 11.4.5
14h MPUERRADDR MPU Error Address Register Section 11.4.6
20h MPUCTRL1 MPU Control Register 1 Section 11.4.7
24h MPUCTRL2 MPU Control Register 2 Section 11.4.8
2Ch MPUTYPE MPU Type Register Section 11.4.9
30h MPUREGBASE MPU Region Base Address Register Section 11.4.10
34h MPUREGSENA MPU Region Size andEnable Register Section 11.4.11
38h MPUREGACR MPU Region Access Control Register Section 11.4.12
3Ch MPUREGNUM MPU Region Number Register Section 11.4.13

<!-- Page 472 -->

NMPU Registers www.ti.com
472 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.1 MPU Revision IDRegister (MPUREV)
Figure 11-4. MPU Revision IDRegister (MPUREV) [offset =00h]
31 30 29 28 27 16
SCHEME Reserved FUNC
R-1 R-0 R-A0Ch
15 11 10 8 7 6 5 0
RTL MAJOR CUSTOM MINOR
R-0 R-1 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 11-4. MPU Revision IDRegister (MPUREV) Field Descriptions
Bit Field Value Description
31-30 SCHEME 1 Identification scheme.
29-28 Reserved 0 Reserved. Reads return 0.
27-16 FUNC A0Ch Indicates functionally equivalent module family. This value isdedicated toHercules family from other
general Texas Instruments MCU orMPU family.
15-11 RTL 0 RTL version number.
10-8 MAJOR 1 Major revision number.
7-6 CUSTOM 0 Indicates device-specific implementation.
5-0 MINOR 0 Minor revision number.
11.4.2 MPU Lock Register (MPULOCK)
Figure 11-5. MPU Lock Register (MPULOCK) [offset =04h]
31 16
Reserved
R-0
15 4 3 0
Reserved LOCK
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-5. MPU Lock Register (MPULOCK) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reserved. Reads return 0.
3-0 LOCK MPU Register Lock Key. Lock feature prevents unintentional updates toMPU registers. Writes
toregisters other than MPUERRSTAT ispossible only when MPU isunlocked. This field is
updated only ifthewrite data is5horAh.Register writes areignored forallother values ofwrite
data.
Abuilt-in correction logic detects single bitsofterror onthisfield andcorrects thevalue inthe
next cycle. Functionality andregister read data remain thesame during thecorrection cycle.
Read: Returns current value ofLOCK bits.
Write inPrivilege:
5h Writes toother MPU registers areblocked.
Ah Writes toother MPU registers areallowed.
Allother values Reserved. The bitsremain unchanged.

<!-- Page 473 -->

www.ti.com NMPU Registers
473 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.3 MPU Diagnostics Control Register (MPUDIAGCTRL)
Figure 11-6. MPU Diagnostics Control Register (MPUDIAGCTRL) [offset =08h]
31 24
Reserved
R-0
23 19 18 17 16
Reserved U_P R_W INT_EXT
R-0 R/WP-0 R/WP-0 R/WP-0
15 8 7 4 3 0
Reserved DIAGKEY Reserved
R-0 R/WP-5h R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-6. MPU Diagnostics Control Register (MPUDIAGCTRL) Field Descriptions
Bit Field Value Description
31-19 Reserved 0 Reserved. Reads return 0.
18 U_P User/Privilege transaction ininternal diagnostic mode. This field isused only ininternal
diagnostic mode.
Read: Returns thecurrent value ofU_P.
Write inPrivilege:
0 Foraccess permission checks, treat thetransaction asuser mode access.
1 Foraccess permission checks, treat thetransaction asprivilege mode access.
17 R_W Read/Write transaction ininternal diagnostic mode. This field isused only ininternal diagnostic
mode.
Read: Returns thecurrent value ofR_W.
Write inPrivilege:
0 Foraccess permission checks, treat thetransaction asread.
1 Foraccess permission checks, treat thetransaction aswrite.
16 INT_EXT Internal/External diagnostic mode.
Read: Returns thecurrent value ofINT_EXT.
Write inPrivilege:
0 Enable internal diagnostic mode.
1 Enable external diagnostic mode.
15-8 Reserved 0 Reserved. Reads return 0.
7-4 DIAGKEY Diagnostics mode key. This isthekeyforenabling diagnostics mode. Allother diagnostic
configuration fields must beprogrammed before enabling thiskey. Diagnostic mode isentered
bywriting Ahtothiskey. Entering orexiting thediagnostic mode automatically clears the
MPUERRSTAT andMPUERRADDR registers. This field isupdated only ifthewrite data is5h
orAh.Register writes areignored forallother values ofwrite data.
Abuilt-in correction logic detects single bitsofterror onthisfield andcorrects thevalue inthe
next cycle. Functionality andregister read data remain thesame during thecorrection cycle.
Read: Returns thecurrent value ofDIAGKEY.
Write inPrivilege:
5h Diagnostics mode isdisabled.
Ah Diagnostics mode isenabled.
Allother values Reserved. The bitsremain unchanged.
3-0 Reserved 0 Reserved. Reads return 0.

<!-- Page 474 -->

NMPU Registers www.ti.com
474 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.4 MPU Diagnostic Address Register (MPUDIAGADDR)
Figure 11-7. MPU Diagnostic Address Register (MPUDIAGADDR) [offset =0Ch]
31 0
DIAG ADDRESS
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 11-7. MPU Diagnostic Address Register (MPUDIAGADDR) Field Descriptions
Bit Field Description
31-0 DIAG ADDRESS Diagnostic address. This register isused indiagnostic mode.
Read: Returns thecurrent value ofdiagnostic address.
Write inPrivilege: Address tobeused fordiagnostic mode tocheck theaddress comparator logic.
11.4.5 MPU Error Status Register (MPUERRSTAT)
Figure 11-8. MPU Error Status Register (MPUERRSTAT) [offset =10h]
31 29 28 27 26 25 24
Reserved RERR WERR BGERR APERR Reserved
R-0 R-0 R-0 R-0 R-0 R-0
23 19 18 16
Reserved REGION
R-0 R-0
15 14 13 8 7 1 0
Reserved MASTERID Reserved ERRFLAG
R-0 R-0 R-0 R/W1CP-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inprivilege mode only; -n=value after reset
Table 11-8. MPU Error Status Register (MPUERRSTAT) Field Descriptions
Bit Field Value Description
31-29 Reserved 0 Reserved. Reads return 0.
28 RERR Read Error. This field isvalid only when theAPERR orBGERR bitis1.This field isread only and
isautomatically reset byclearing theERRFLAG bit.This field isnotupdated when theERRFLAG
bitisset.Writes have noeffect.
0 MPU compare faildidnotoccur onaread access.
1 MPU compare failoccurred onaread access.
27 WERR Write Error. This field isvalid only when theAPERR orBGERR bitis1.This field isread only andis
automatically reset byclearing theERRFLAG bit.This field isnotupdated when theERRFLAG bit
isset.Writes have noeffect.
0 MPU compare faildidnotoccur onawrite access.
1 MPU compare failoccurred onawrite access.
26 BGERR Background Error. This field isread only andisautomatically reset byclearing theERRFLAG bit.
This field isnotupdated when theERRFLAG bitisset.Writes have noeffect.
0 There was nomemory access toaddresses thatareoutside alltheenabled MPU regions.
1 MPU compare failgenerated because ofaccess toanaddress thatisoutside alltheenabled MPU
regions.

<!-- Page 475 -->

www.ti.com NMPU Registers
475 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)Table 11-8. MPU Error Status Register (MPUERRSTAT) Field Descriptions (continued)
Bit Field Value Description
25 APERR Access Permission Error. This field isread only andisautomatically reset byclearing the
ERRFLAG bit.This field isnotupdated when theERRFLAG bitisset.Writes have noeffect.
0 Access permission violation didnotoccur inanyoftheenabled MPU regions.
1 MPU compare failgenerated because ofaccess permission violation inoneoftheenabled MPU
regions.
24-19 Reserved 0 Reserved. Reads return 0.
18-16 REGION Region. This field isvalid only when theAPERR bitis1.This field indicates thehighest priority
MPU region forwhich anaccess permission error was detected. This field isread only andis
automatically reset byclearing theERRFLAG bit.This field isnotupdated when theERRFLAG bit
isset.Writes have noeffect.
0 MPU compare failgenerated because ofaccess permission violation inregion-0.
1h MPU compare failgenerated because ofaccess permission violation inregion-1.
2h MPU compare failgenerated because ofaccess permission violation inregion-2.
3h MPU compare failgenerated because ofaccess permission violation inregion-3.
4h MPU compare failgenerated because ofaccess permission violation inregion-4.
5h MPU compare failgenerated because ofaccess permission violation inregion-5.
6h MPU compare failgenerated because ofaccess permission violation inregion-6.
7h MPU compare failgenerated because ofaccess permission violation inregion-7.
15-14 Reserved 0 Reserved. Reads return 0.
13-8 MASTERID Master IDforMPU compare fail.This field isvalid only when APERR orBGERR bitis1.This field
isread only andisautomatically reset byclearing theERRFLAG bit.This field isnotupdated when
theERRFLAG bitisset.Writes have noeffect.
Shows themaster IDforthefirsttransaction thatresulted inacompare fail.Master IDistaken from
MReqInfo[8:3] bits.
7-1 Reserved 0 Reserved. Reads return 0.
0 ERRFLAG MPU compare error flag.
Read:
0 NoMPU compare failwas detected.
1 Atleast oneMPU compare failwas detected.
Write inPrivilege:
0 Reserved. The bitremains unchanged.
1 Clears thebit.

<!-- Page 476 -->

NMPU Registers www.ti.com
476 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.6 MPU Error Address Register (MPUERRADDR)
Figure 11-9. MPU Error Address Register (MPUERRADDR) [offset =14h]
31 0
COMPARE FAIL ADDRESS
R-0
LEGEND: R=Read only; -n=value after reset
Table 11-9. MPU Error Address Register (MPUERRADDR) Field Descriptions
Bit Field Description
31-0 COMPARE FAIL ADDRESS Address forMPU compare fail.This field isvalid only when theERRFLAG bitintheMPU error
status register (MPUERRSTAT) issetandtheAPERR orBGERR bitinMPUERRSTAT register
is1.This field isread only andisautomatically reset byclearing theERRFLAG bitin
MPUERRSTAT register. This field isnotupdated when theERRFLAG bitisset.Once the
ERRFLAG bitiscleared, thisfield gets updated forthenext MPU compare failafter clearing the
flag. Writes have noeffect.
Shows theaddress forthefirsttransaction thatresulted inacompare fail.
11.4.7 MPU Control Register 1(MPUCTRL1)
Figure 11-10. MPU Control Register 1(MPUCTRL1) [offset =20h]
31 16
Reserved
R-0
15 4 3 0
Reserved MPUENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-10. MPU Control Register 1(MPUCTRL1) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reserved. Reads return 0.
3-0 MPUENA MPU Enable Key. This isthekeyforenabling memory protection. This field isupdated only if
thewrite data is5horAh.Register writes areignored forallother values ofwrite data. Allother
configuration registers must beprogrammed before enabling theMPU.
Abuilt-in correction logic detects single bitsofterror onthisfield andcorrects thevalue inthe
next cycle. Functionality andregister read data remain thesame during thecorrection cycle.
Read: Returns current value ofMPUENA.
Write inPrivilege:
5h Memory protection isdisabled.
Ah Memory protection isenabled.
Allother values Reserved. The bitsremain unchanged.

<!-- Page 477 -->

www.ti.com NMPU Registers
477 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.8 MPU Control Register 2(MPUCTRL2)
Figure 11-11. MPU Control Register 2(MPUCTRL2) [offset =24h]
31 16
Reserved
R-0
15 4 3 0
Reserved ERRENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-11. MPU Control Register 2(MPUCTRL2) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reserved. Reads return 0.
3-0 ERRENA MPU Error Pulse Enable. This isthekeyforenabling ERROR pulse output generation forthe
Error Signaling Module. This field isupdated only ifthewrite data is5horAh.Register writes
areignored forallother values ofwrite data.
Abuilt-in correction logic detects single bitsofterror onthisfield andcorrects thevalue inthe
next cycle. Functionality andregister read data remain thesame during thecorrection cycle.
Read: Returns current value ofERRENA.
Write inPrivilege:
5h Error pulse output toESM isdisabled.
Ah Error pulse output toESM isenabled.
Allother values Reserved. The bitsremain unchanged.

<!-- Page 478 -->

NMPU Registers www.ti.com
478 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.9 MPU Type Register (MPUTYPE)
Figure 11-12. MPU Type Register (MPUTYPE) [offset =2Ch]
31 16
Reserved
R-0
15 8 7 0
NUMREG Reserved
R-x R-0
LEGEND: R=Read only; -n=value after reset; -x=value isimplementation defined
Table 11-12. MPU Type Register (MPUTYPE) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reserved. Reads return 0.
15-8 NUMREG Number ofMPU Regions. Indicates thenumber ofimplemented MPU regions.
0 Reserved
1h 1MPU region isimplemented.
2h 2MPU regions areimplemented.
3h 3MPU regions areimplemented.
4h 4MPU regions areimplemented.
5h 5MPU regions areimplemented.
6h 6MPU regions areimplemented.
7h 7MPU regions areimplemented.
8h 8MPU regions areimplemented.
Allother values Reserved
7-0 Reserved 0 Reserved. Reads return 0.

<!-- Page 479 -->

www.ti.com NMPU Registers
479 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.10 MPU Region Base Address Register (MPUREGBASE)
NOTE: MPUREGBASE0-7 registers arememory-mapped tothesame address. Which region
register isselected forread/write access isdecided bytheREGION field intheMPU region
number register (MPUREGNUM).
Figure 11-13. MPU Region Base Address Register (MPUREGBASE) [offset =30h]
31 16
BASE_ADDRESS
R/WP-0
15 5 4 0
BASE_ADDRESS Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-13. MPU Region Base Address Register (MPUREGBASE) Field Descriptions
Bit Field Value Description
3-0 BASE_ADDRESS Base address. Defines thebase address foranMPU region.
Read: Returns current value ofbase address.
Write inPrivilege: Defines thebase address foranMPU region.
4-0 Reserved 0 Reserved. Reads return 0.
11.4.11 MPU Region Size andEnable Register (MPUREGSENA)
NOTE: MPUREGSENA0-7 registers arememory-mapped tothesame address. Which region
register isselected forread/write access isdecided bytheREGION field intheMPU region
number register (MPUREGNUM).
Figure 11-14. MPU Region Size andEnable Register (MPUREGSENA) [offset =34h]
31 16
Reserved
R-0
15 6 5 1 0
Reserved REG_SIZE REGENA
R-0 R/WP-1Fh R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-14. MPU Region Size andEnable Register (MPUREGSENA) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reserved. Reads return 0.

<!-- Page 480 -->

NMPU Registers www.ti.com
480 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)Table 11-14. MPU Region Size andEnable Register (MPUREGSENA) Field Descriptions (continued)
Bit Field Value Description
5-1 REG_SIZE MPU Region size. This field determines thesize ofanMPU region.
Read: Returns current value ofREG_SIZE.
Write inPrivilege: Defines thesize ofanMPU region.
0-3h Reserved
4h 32bytes
5h 64bytes
6h 128bytes
7h 256bytes
8h 512bytes
9h 1KB
Ah 2KB
Bh 4KB
Ch 8KB
Dh 16KB
Eh 32KB
Fh 64KB
10h 128KB
11h 256KB
12h 512KB
13h 1MB
14h 2MB
15h 4MB
16h 8MB
17h 16MB
18h 32MB
19h 64MB
1Ah 128MB
1Bh 256MB
1Ch 512MB
1Dh 1GB
1Eh 2GB
1Fh 4GB
0 REGENA MPU Region Enable. This istheregister bitforenabling anMPU region.
Read:
0 MPU region isdisabled.
1 MPU region isenabled.
Write inPrivilege:
0 Disable MPU region.
1 Enable MPU region.

<!-- Page 481 -->

www.ti.com NMPU Registers
481 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.12 MPU Region Access Control Register (MPUREGACR)
NOTE: MPUREGACR0-7 registers arememory-mapped tothesame address. Which region register
isselected forread/write access isdecided bytheREGION field intheMPU region number
register (MPUREGNUM).
Figure 11-15. MPU Region Access Control Register (MPUREGACR) [offset =38h]
31 16
Reserved
R-0
15 11 10 8 7 0
Reserved AP Reserved
R-0 R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-15. MPU Region Access Control Register (MPUREGACR) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reserved. Reads return 0.
10-8 AP MPU Region Access Permission. This field determines theaccess permission formemory accesses to
addresses thatareinanMPU region.
Read: Returns current value ofAP.
Write inPrivilege: Defines access permissions.
0 Noaccess.
1h Read/write inprivileged mode; Noaccess inuser mode.
2h Read/write inprivileged mode; Read only inuser mode.
3h Read/write.
4h Noaccess.
5h Read only inprivileged mode; Noaccess inuser mode.
6h Read only.
7h Noaccess.
7-0 Reserved 0 Reserved. Reads return 0.

<!-- Page 482 -->

NMPU Registers www.ti.com
482 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedSystem Memory Protection Unit (NMPU)11.4.13 MPU Region Number Register (MPUREGNUM)
NOTE: MPUREGBASE0-7, MPUREGSENA0-7, MPUREGACR0-7, MPUREGAM0-7, MPUREGTA0-
7,andMPUREGMT0-7 registers arememory-mapped tojustsixdifferent addresses. Which
region register isselected forread/write access isdecided bytheREGION field intheMPU
region number register (MPUREGNUM).
Figure 11-16. MPU Region Number Register (MPUREGNUM) [offset =3Ch]
31 16
Reserved
R-0
15 4 3 0
Reserved REGION
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 11-16. MPU Region Number Register (MPUREGNUM) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reserved. Reads return 0.
3-0 REGION MPU Region Number. This field determines which MPU region registers areaccessed. Writing this
register with avalue greater than orequal tothenumber ofimplemented MPU regions (indicated by
MPUTYPE register) does notaffect theNMPU functionality. Behavior willbesame asthatofreserved
space.
Read: Returns current value ofREGION.
Write inPrivilege:
0 Access MPU region 0registers.
1h Access MPU region 1registers.
2h Access MPU region 2registers.
3h Access MPU region 3registers.
4h Access MPU region 4registers.
5h Access MPU region 5registers.
6h Access MPU region 6registers.
7h Access MPU region 7registers.