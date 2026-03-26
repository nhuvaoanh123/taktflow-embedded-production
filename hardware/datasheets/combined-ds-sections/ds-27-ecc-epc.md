# ECC Error Event Monitoring

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 121-122 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 121 -->
CAMFIFO
FIFO
FIFO
FIFOCPU0□Correctable□Error
CPU□SCR□Correctable□ECC□for□DMA I/F
CPU□□SCR□Correctable□ECC□for□PS_SCR_M□I/F
L2RAMW□RMW□Correctable□ErrorLookupFSM
Err□Gen
Err□Stat
ESM Correctable□Error□Capture□Block
CPU□SCR□Uncorrectable□ECC□for□DMA I/F
CPU□SCR□Uncorrectable□ECC□for□PS_SCR_M□I/FErr□Gen
Uncorrectable□Error□Capture□BlockUERR Addr□Reg
UERR Addr□RegErr□Stat
Err□Stat
EPC□ModuleCorrectable□Error□Event□Source
Unorrectable□Error□Event□Sourcech0
ch2
ch3
ch4
ch0
ch1
121TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedNOTE
The application can change themapping ofinterrupt sources totheinterrupt channels
through theinterrupt channel control registers (CHANCTRLx) inside theVIM module.
6.16 ECC Error Event Monitoring andProfiling
This device includes anError Profiling Controller (EPC) module. The main goal ofthismodule istoenable
thesystem totolerate acertain amount ofECC correctable errors onthesame address repeatedly inthe
memory system with minimal runtime overhead. Main features implemented inthisdevice aredescribed
below.
*Capture theaddress ofcorrectable ECC faults from different sources (forexample, CPU, L2RAM, Interconnect)
intoa16-entry Content Addressable Memory (CAM).
*Forcorrectable faults, theerror handling depends onthebelow conditions:
-iftheincoming address isalready inthe16-entry CAM, discard thefail.Noerror generated toESM
-iftheaddress isnotintheCAM list,and theCAM has empty entries, add theaddress into theCAM list.In
addition, raise theerror signal totheESM group 1ifenabled.
-iftheaddress isnotintheCAM list,and theCAM has noempty entries, always raise asignal totheESM
group 1.
*A4-entry FIFO tostore correctable error events andaddresses foreach IPinterface.
*Foruncorrectable faults ofnon-CPU access, capture theaddress andraise asignal totheESM group 2.
*The CAM isimplemented inmemory mapped registers. The CPU canread and write toanyentry fordiagnostic
testasifarealCAM memory macro.
Figure 6-18. EPC Block Diagram

<!-- Page 122 -->
122TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.16.1 EPC Module Operation
6.16.1.1 Correctable Error Handling
When acorrectable error isdetected inthesystem byanIP,itsends theerror signal along with theerror
address toEPC module. The EPC module willscan thiserror address inthe16-entry CAM. Ifthere isa
match then theaddress isdiscard and noerror isgenerated toESM bytheECP. Ittakes one cycle to
scan one address atatime through theCAM. The idea istoallow thesystem totolerate acorrectable
error occurring onthesame address because thiserror hasbeen handled before bytheCPU. This error
scenario isparticularly frequent when thesoftware isinaforloop fetching thesame address. Because
there aremultiple IPswhich cansimultaneously detect correctable errors inthesystem, theEPC employs
a4-entry FIFO perIPinterface sothaterror addresses arenotlost.
Ifanaddress isnotmatched intheCAM then itdepends ifthere isempty entry intheCAM. Ifthere isan
empty entry then thenew address isstored intotheempty entry. Foreach entry there isa4-bit valid key.
When anew address isstored the4-bit keyisupdated with "1010". Itisprogrammable togenerate a
correctable error totheESM iftheaddress isnotmatched andthere isanempty CAM entry. Once CPU is
interrupted, itcanchoose toevaluate theerror address and handle itaccordingly. The software canalso
invalidate theentry bywriting "0101".
Ifandress isnotmatched and there isnoempty entry intheCAM then thecorrectable error is
immediately sent totheESM. The new error address islostifthere isnoempty entry leftintheCAM.
6.16.1.2 Uncorrectable Error Handling
Uncorrectable errors reported bytheIP(non-CPU access) are immediately captured fortheir error
addresses and update totheuncorrectable error status register. Formore information see thedevice
specific technical reference guide SPNU563 .