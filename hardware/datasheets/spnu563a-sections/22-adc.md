# Analog To Digital Converter (ADC) Module

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 848-952

---


<!-- Page 848 -->

848 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleChapter 22
SPNU563A -March 2018
Analog ToDigital Converter (ADC) Module
This chapter describes theanalog todigital converter (ADC) interface module.
Topic ........................................................................................................................... Page
22.1 Overview ........................................................................................................ 849
22.2 Basic Operation ................................................................................................ 853
22.3 ADC Registers .................................................................................................. 881

<!-- Page 849 -->

www.ti.com Overview
849 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.1 Overview
This microcontrollers implements uptotwoinstances oftheADC module. The main features oftheADC
module are:
*Selectable 10-bit or12-bit resolution
*Successive-approximation-register architecture
*Three conversion groups -Group1, Group2, andEvent Group
*Allthree conversion groups canbeconfigured tobehardware-triggered; group1 andgroup2 canalso
betriggered bysoftware
*Conversion results arestored ina64-word memory (SRAM)
-These 64words aredivided between thethree conversion groups andareconfigurable bysoftware
-Accesses totheconversion result RAM areprotected byparity
*Flexible options forgenerating DMA requests fortransferring conversion results
*Selectable channel conversion order
-Sequential conversions inascending order ofchannel number, OR
-User-defined channel conversion order with theEnhanced Channel Selection Mode
*The Enhanced Channel Selection Mode isonly available toADC1.
*Single orcontinuous conversion modes
*Embedded self-test logic forinput channel failure detection (open /short topower /short toground)
*Embedded calibration logic foroffset error correction
*Enhanced Power-down mode
*External event pin(ADEVT) totrigger conversions
-ADEVT isalso programmable asgeneral-purpose I/O
*Eight hardware events totrigger conversions

<!-- Page 850 -->

ADC1
12 BitAD1EXT_SEL[4:0]
AD1EXT_ENA
AD1EVT
AD1IN[7:0]
AD1IN[15:8]/AD2IN[15:8]
AD1IN[23:16]/AD2IN[7:0]
AD1IN[31:24]
ADC2
12 BitVCCAD
VSSAD
AD REFHI
AD REFLO
AD2EVTAD2IN[24:16]
Overview www.ti.com
850 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleThe twoinstances ofthe12-bit ADC modules onthemicrocontroller share 16analog input channels. The
connections areshown inFigure 22-1.
*ADC1 supports 32channels.
*ADC2 supports 25channels, ofwhich 16channels areshared with ADC1.
*When using both ADC1 andADC2 onashared channel, thesample windows must beidentical such
thatthesample windows completely match each other ornon-overlapping with aminimum of2ADC
cycles buffer between theendofoneADC 'ssample window andthestart oftheother ADC 'ssample
window.
*The reference voltages, aswell asoperating supply andground, areshared between thetwoADC
cores.
Figure 22-1. Channel Assignments ofTwo ADC Cores

<!-- Page 851 -->

AD1IN31
AD1IN010/12-bit
Analog-to-DigitalSuccessive Approximation
Converter
Sequencer and
ADC Results' Memory Interface
Controller10/12-bit
VBUS Interface for Access to ADC Registers and Results' RAMAIN
VCCA D
VSS AD
End Of
Conversion ResultR1 R2S1 S2 S3 S4 S6ADREFHI
ADREFLO
ADCLK
START
PDZSelf Test
&
Calibration
Results' RAMAD1EXT_SEL[4:0]
AD1EXT_ENASample Cap
Discharge Switch
Input "Multiplexer"
GP1_DMA_REQEV_DMA_REQGP2_INTGP1_INTEV_INT
MAG_THR_INT[5:0]Analog Core Interface Input
Channel
Selection32
SWCNTRL[3:0]
Samp_Cap_Discharge
Interrupt
Generation
GP1_DMA_REQDMA
GenerationRequest
ADEVSRC.EV_SRC[2:0],
ADG1SRC.G1_SRC[2:0],
and ADG2SRC.G2_SRC[2:0] Event Trigger
Generation
ADC_resADC_res
www.ti.com Overview
851 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.1.1 Introduction
This section presents abrief functional description oftheanalog-to-digital converter (ADC) module.
Figure 22-2 shows thecomponents oftheADC module.
Figure 22-2. ADC Block Diagram

<!-- Page 852 -->

DigitalResult4096 x(InputVoltage - AD )REFLO
(AD - AD )REFHI REFLO- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 0.5- =
DigitalResult1024 x (InputVoltage - AD )REFLO
AD - ADREFHI REFLO- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 0.5- =
Overview www.ti.com
852 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.1.1.1 Input Multiplexor
The input multiplexor (MUX) connects theselected input channel totheAINinput oftheADC core. The
ADC1 module supports upto32inputs asshown inFigure 22-2.The ADC2 module supports upto25
inputs. The sequencer selects thechannel tobeconverted. Enabling theenhanced channel selection
mode also allows oneormore oftheanalog input channels tobeconnected totheoutput ofanexternal
analog switch ormultiplexor.
22.1.1.2 Self-Test andCalibration Cell
The ADC includes specific hardware thatallows asoftware algorithm todetect open/short onanADC
analog input. Italso allows theapplication program tocalibrate theADC. Also seeSection 22.2.6.1 and
Section 22.2.6.2 .
22.1.1.3 Analog-to-Digital Converter Core
The ADC core isacombination voltage scaling, charge redistribution Successive Approximation Register
(SAR) based analog-to-digital converter. The core canbeconfigured foroperation in10-bit resolution
(default) or12-bit resolution. This iscontrolled bythesequencer logic. This selection applies toall
conversions performed bytheADC module. Itisnotpossible toconvert some channels with a12-bit
resolution andsome with a10-bit resolution.
Asingle conversion from ananalog input toadigital conversion result occurs intwodistinct periods:
*Sampling Period:
-The sequencer generates aSTART signal totheADC core tosignal thestart ofthesampling
period.
-The analog input signal issampled directly ontotheswitched capacitor array during thisperiod,
providing aninherent sample-and-hold function.
-The sampling period ends onefullADCLK after thefalling edge oftheSTART signal.
-The sequencer cancontrol thesampling period duration byconfiguring theconversion group 's
sample time control register (ADEVSAMP, ADG1SAMP, ADG2SAMP). This register controls the
time forwhich theSTART signal stays high.
*Conversion Period:
-The conversion period starts onefullADCLK after thefalling edge ofSTART.
-One bitoftheconversion result isoutput oneach rising edge ofADCLK intheconversion period,
starting with themost-significant bitfirst.
-The conversion period is12ADCLK cycles incase ofa12-bit ADC, andis10ADCLK cycles in
case ofa10-bit ADC.
-The ADC core generates anEnd-Of-Conversion (EOC) signal tothesequencer attheendofthe
conversion period. Atthistime thecomplete 12-, or10-bit conversion result isavailable.
-The sequencer captures theADC core conversion result output assoon asEOC isdriven High.
The analog conversion range isdetermined bythereference voltages: ADREFHI andADREFLO.ADREFHI isthe
topreference voltage andisthemaximum analog voltage thatcanbeconverted. Ananalog input voltage
equal toADREFHI orhigher results inanoutput code of0x3FF for10-bit resolution and0xFFF for12-bit
resolution. ADREFLO isthebottom reference voltage andistheminimum analog voltage thatcanbe
converted. Applying aninput voltage equal toADREFLO orlower results inanoutput code of0x000. Both
ADREFHI andADREFLO must bechosen nottoexceed theanalog power supplies: VCCADandVSSAD,
respectively. Input voltages between ADREFHI andADREFLO produce aconversion result given by
Equation 27for10-bit resolution andbyEquation 28for12-bit resolution.
(27)
(28)

<!-- Page 853 -->

www.ti.com Basic Operation
853 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.1.1.4 Sequencer
The sequencer coordinates theoperations oftheADC, including theinput multiplexor, theADC core, and
theresult memory. Inaddition, thelogic ofthesequencer sets thestatus register flags when the
conversion isongoing, stopped, orfinished.
Allthefeatures ofthesequencer arediscussed indetail inthefollowing sections ofthisdocument.
22.1.1.5 Conversion Groups
Several applications require groups ofchannels tobeconverted using asingle trigger source forexample.
There could also besome groups ofchannels identified which require aspecific setting oftheacquisition
time. The ADC module supports three conversion groups forthispurpose -Group1, Group2 andthe
Event Group.
Any oftheavailable analog input channels canbeassigned toanyoftheconversion groups. This also
allows aparticular channel toberepeatedly sampled byselecting itinmultiple groups. There isan
inherent priority scheme used when multiple conversion groups aretriggered atonce. The Event Group is
thehighest-priority, followed bytheGroup1 andthen theGroup2.
The Event Group isalways hardware event-triggered. Group1 andGroup2 aresoftware-triggered by
default andcanbeconfigured tobehardware-, orevent-triggered aswell. The triggering ofconversions in
each group isdiscussed inSection 22.2.1.6 .
Each conversion group hasaseparate setofcontrol registers to:
*Select theinput channels tobeconverted
*Configure themode ofconversion: single conversion sequence orcontinuous conversions
*Configure theinput channel sampling time
*Configure theinterrupt and/or DMA request generation conditions
22.2 Basic Operation
22.2.1 Basic Features andUsage oftheADC
This section describes theusage ofthebasic features oftheADC module.
22.2.1.1 How toSelect Between 12-bit and10-bit Resolutions
The 10_12_BIT field oftheADC Operating Mode Control Register (ADOPMODECR) configures theADC
tobein10-bit or12-bit resolution mode:
*If10_12_BIT =0,themodule isin10-bit resolution mode. This isthedefault mode ofoperation.
*If10_12_BIT =1,themodule isin12-bit resolution mode.
22.2.1.2 How toSetUptheADCLK Speed
The ADC sequencer generates theclock fortheADC core, ADCLK. The ADC core uses theADCLK
signal foritstiming. The ADCLK isgenerated bydividing down theinput clock totheADC module, which
istheVBUSP interface clock, VCLK. A5-bit field (PS) intheADC Clock Control Register (ADCLOCKCR)
isused todivide down theVCLK by1upto32.The ADCLK valid frequency range isspecified inthe
device datasheet.
fADCLK =fVCLK/(PS+1)
The maximum frequency forADCLK isspecified inthedevice datasheet.

<!-- Page 854 -->

Basic Operation www.ti.com
854 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.1.3 How toSetUptheInput Channel Acquisition Time
The signal acquisition time foreach group isseparately configurable using theADG1SAMP[11:0],
ADG2SAMP[11:0], andADEVSAMP[11:0] registers.
The acquisition time isspecified interms ofADCLK cycles andranges from aminimum of2ADCLK
cycles toamaximum of4098 ADCLK cycles.
Forexample, Group1 acquisition time, tACQG1 =G1SAMP[11:0] +2,inADCLK cycles.
The minimum acquisition time isspecified inthedevice datasheet. This time also depends onthe
impedance ofthecircuit connected totheanalog input channel being converted. See theADC Source
Impedance forHercules ™ARM ®Safety MCUs Application Report (SPNA118 ).
22.2.1.4 How toSelect anInput Channel forConversion
The ADC module needs tobeenabled firstbefore selecting aninput channel forconversion. The ADC
module canbeenabled bysetting theADC_EN bitintheADC Operating Mode Control Register
(ADOPMODECR). Multiple input channels canbeselected forconversion ineach group. Only oneinput
channel isconverted atatime. The channels tobeconverted areconfigured inoneormore ofthethree
conversion groups 'channel selection registers. Channels tobeconverted inGroup1 areconfigured inthe
Group1 Channel-Select Register (ADG1SEL), those tobeconverted inGroup2 areconfigured inthe
Group2 Channel-Select Register (ADG2SEL), andthose tobeconverted intheEvent Group are
configured intheEvent Group Channel-Select Register (ADEVSEL).
The description inthissection only refers tothecase when theenhanced channel selection mode isnot
enabled. Input channel selection intheenhanced channel selection mode isdefined inSection 22.2.2 .
22.2.1.5 How toSelect Between Single Conversion Sequence orContinuous Conversions
Each group hasitsown mode control register. The MODE field ofthese control registers allow the
application toselect between asingle conversion sequence orcontinuous conversion mode.
NOTE: Selecting continuous conversion mode forallthree groups
Allthree conversion groups cannot beconfigured tobeinacontinuous conversion mode. If
theapplication configures thegroup mode control registers toenable continuous conversion
mode forallthree groups, then theGroup2 willbeautomatically beconfigured tobeina
single conversion sequence mode.
With conversions ongoing incontinuous conversion mode, iftheMODE field ofagroup iscleared, then
thatgroup switches tothesingle conversion sequence mode. Conversions forthisgroup willstop once all
channels selected forthatgroup have been converted.
22.2.1.6 How toStart aConversion
The conversion groups Group1 andGroup2 aresoftware-triggered bydefault. Aconversion inthese
groups canbestarted justbywriting thedesired channels totherespective Channel-Select Registers. For
example, inorder toconvert channels 0,1,2,and3inGroup1 andchannels 8,9,10,and11inGroup2,
theapplication justhastowrite 0x0000000F toADG1SEL and0x00000F00 toADG2SEL. The ADC
module willstart byservicing thegroup thatwas triggered first, Group1 inthisexample.
The conversions forallgroups areperformed inascending order ofthechannel number. FortheGroup1
theconversions willbeperformed intheorder: channel 0first, followed bychannel 1,then channel 2,and
then channel 3.The Group2 conversions willbeperformed intheorder: channels 8,9,10,and11.
The Event Group isonly hardware-triggered. There areuptoeight hardware event trigger sources defined
fortheADC module. Check thedevice datasheet foracomplete listing ofthese eight hardware trigger
options.
The trigger source tobeused needs tobeconfigured intheADEVSRC register. Similar registers also
exist fortheGroup1 andGroup2 asthese canalso beconfigured tobeevent-triggered.
The polarity oftheevent trigger isalso configurable, with afalling edge being thedefault.

<!-- Page 855 -->

BNDENDBNDA
BNDB0x00
Event Memory Depth
Group 1 Memory Depth
Group 2 Memory DepthTotal Memory Depth
www.ti.com Basic Operation
855 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleAnEvent Group conversion starts when atleast onechannel isselected forconversion inthisgroup, and
when thedefined event trigger occurs.
Ifanyconversion group isconfigured tobeinacontinuous conversion mode, then itneeds toonly be
triggered once. Allthechannels selected forconversion inthatgroup willbeconverted repeatedly.
22.2.1.7 How toKnow When theGroup Conversion isCompleted
Each conversion group hasastatus flagtoindicate when itsconversion hasended. See ADEVSR,
ADG1SR, andADG2SR. This bitissetwhen aconversion sequence foragroup ends. This bitdoes is
always setifagroup isconfigured forcontinuous conversions.
22.2.1.8 How Results areStored intheResults 'Memory
The ADC stores theconversion results inthree separate memory regions intheADC Results 'RAM, one
region foreach group. Each memory region isastack ofbuffers, with each buffer capable ofholding one
conversion result. The number ofbuffers allocated foreach group isprogrammed byconfiguring theADC
module registers ADBNDCR andADBNDEND.
ADBNDCR contains two9-bit pointers BNDA andBNDB. BNDA, BNDB, andBNDEND areused to
partition thetotal memory available intothree memory regions asshown inFigure 22-3.Both BNDA and
BNDB arepointers referenced from thestart oftheresults 'memory. BNDA specifies thenumber ofbuffers
allocated fortheEvent Group conversion results inunits oftwobuffers; BNDB specifies thenumber of
buffers allocated fortheEvent Group plus Group1 inunits oftwobuffers. Refer toSection 22.3.23 for
more details onconfiguring theADC results 'memory.
ADBNDEND contains a3-bit field called BNDEND thatconfigures thetotal memory available. The ADC
module cansupport upto1024 buffers. The device supports amaximum of64buffers forboth theADC
modules.
Figure 22-3. FIFO Implementation
*Number ofbuffers forEvent Group =2×BNDA
*Number ofbuffers forGroup1 =2×(BNDB -BNDA)
*Number ofbuffers forGroup2 =Total number ofbuffers -2×BNDB
22.2.1.9 How toRead theResults from theResults 'Memory
The CPU canread theconversion results inoneoftwoways:
1.Byusing theconversion results memory asaFIFO queue
2.Byaccessing theconversion results memory directly

<!-- Page 856 -->

Basic Operation www.ti.com
856 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.1.9.1 Reading Conversion Results from aFIFO
The conversion results foreach group canbeaccessed viaarange ofaddresses provided tofacilitate the
useoftheARM Cortex-R4 CPU 'sLoad-Multiple (LDM) instruction. Asingle read performed using theLDR
instruction canalso beused toread outasingle conversion result. The results areread outfrom the
group 'smemory region asaFIFO queue byreading from anylocation inside thisaddress range. The
conversion result thatgotstored firstgets read first. Aresult thatisread from thememory inthismethod
isremoved from thememory. Forexample, aread from anyaddress intherange ADEVBUFFER (offset
90htoAFh) pulls outoneconversion result from theEvent Group memory.
Figure 22-4. Format ofConversion Result Read from FIFO, 12-bit ADC
Offset Address
Register31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
0x90 to0xAF
ADEVBUFFEREV_
EMPTYReserved EV_CHID
Reserved EV_DR
0xB0 to0xCF
ADG1BUFFERG1_
EMPTYReserved G1_CHID
Reserved G1_DR
0xD0 to0xEF
ADG2BUFFERG2_
EMPTYReserved G2_CHID
Reserved G2_DR
Figure 22-5. Format ofConversion Result Read from FIFO, 10-bit ADC
Offset Address
Register31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
0x90 to0xAF
ADEVBUFFERReserved
EV_
EMPTYEV_CHID EV_DR
0xB0 to0xCF
ADG1BUFFERReserved
G1_
EMPTYG1_CHID G1_DR
0xD0 to0xEF
ADG2BUFFERReserved
G2_
EMPTYG2_CHID G2_DR
Option toread channel idalong with conversion result:
The application hasanoption toread thechannel idalong with theconversion result. This iscontrolled by
theCHID field ofthegroup 'smode control register. Iftheoption toread thechannel idisnotselected, the
channel idfield oftheconversion result reads aszeros.
Protection against reading from empty FIFO:
There isalso ahardware mechanism toprotect theapplication from reading past thenumber ofnew
conversion results held intheFIFO. Once allavailable conversion results have been read outoftheFIFO
bytheapplication, asubsequent read from theFIFO causes themechanism toindicate thattheFIFO is
empty bysetting theEMPTY field.
Debug /Emulation Support:
Fordebug purposes, each conversion group also provides anaddress thattheapplication canread from
forextracting thegroup 'sconversion results. However, nostatus flags foraconversion group areaffected
byreading from these emulation buffer addresses. Forexample, reading from ADEVEMUBUFFER (offset
F0h) returns thenext result intheEvent Group buffer butdoes notactually remove thatresult from the
buffer orchange theamount ofdata held inthebuffer.

<!-- Page 857 -->

Conversion word 0
Conversion word 1
Conversion word 2
Conversion word 62
Conversion word 63 0xFF3E00FC0xFF3E01F80xFF3E00080xFF3E00040xFF3E0000ADC1
0xFF3A00FC0xFF3A01F80xFF3A00080xFF3A00040xFF3A0000ADC2
www.ti.com Basic Operation
857 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.1.9.2 Reading Conversion Results Directly from theConversion Results 'Memory
The conversion result memory ispart ofthedevice 'smemory map. The base address fortheADC1 result
memory isFF3E 0000h andfortheADC2 result memory isFF3A 0000h.
Figure 22-6. ADC Memory Mapping
The application canidentify theaddress ranges foreach ofthethree memory regions forthethree
conversion groups after performing thesegmentation asdescribed inSection 22.2.1.8 .Itisuptothe
application toread thedesired results from thethree conversion groups. The formats oftheconversion
results when reading from RAM directly areshown inFigure 22-7 andFigure 22-8.
Figure 22-7. Format ofConversion Result Directly Read from ADC RAM, 12-bit ADC
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
ADC RAM
addressReserved channel id[4]
channel id[3-0] 12-bit conversion result
Figure 22-8. Format ofConversion Result Directly Read from ADC RAM, 10-bit ADC
31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
ADC RAM
addressReserved
Rsvd channel id[4-0] 10-bit conversion result
Note thatthere isnoEMPTY field toprotect theapplication from reading data thathasbeen previously
read.
Each group does have aseparate register which holds theaddress inthegroup 'sresult memory where
theADC willwrite thenext conversion result. These aretheADEVRAMWRADDR, ADG1RAMWRADDR,
andADG2RAMWRADDR registers. The application canusethisinformation tocalculate how many valid
conversion results areavailable toberead.
Benefit ofreading conversion results directly from ADC RAM:
The application does nothave toread outconversion results sequentially asinthecase ofreading from a
FIFO. Asaresult, theapplication canselectively read theconversion results foranyparticular input
channel ofinterest without having toread other channels 'conversion results.

<!-- Page 858 -->

BNDA
BNDBChannel 0
Channel 1
Channel 2
Channel 0
Channel 1
Channel 2
Channel 4
Channel 7
Channel 8
Channel 4
Channel 7
Channel 8
Channel 3
Channel 5
Channel 6
Channel 3
Channel 5
Channel 6...
...
...EV RAM ADDR
G1 RAM ADDR
G2 RAM ADDR
BNDEND0x00
Event Group Memory
Group 1 Memory
Group 2 Memory
Basic Operation www.ti.com
858 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.1.9.3 Example
Suppose thatchannels 0,1,and2areselected forconversion intheEvent Group, channels 4,7,and8
areselected forconversion ingroup 1,andchannels 3,5,and6areselected forconversion ingroup 2.
The conversion results willgetstored inthethree memory regions asshown inFigure 22-9.
Suppose thattheCPU wants toread outtheresults fortheEvent Group from aFIFO queue. The CPU
needs toread from anyaddress intherange ADEVBUFFER (offset 90htoAFh) multiple times, ordoa
"load multiple "from thisrange ofaddresses. This willcause theADC toreturn theresults forchannel 0,
then channel 1,then channel 2,then channel 0,andsoonforeach read access tothisaddress range.
Now suppose thattheapplication wants toread outtheresults forthegroup 1from theRAM directly. The
conversion results forthegroup 1areaccessible starting from address ADC RAM Base Address +BNDA.
Also, itisknown thatthefirstresult atthisaddress isfortheinput channel 4,thenext oneisforinput
channel 7,andsoon.Sotheapplication canselectively read theconversion results foronly onechannel if
sodesired.
Figure 22-9. Conversion Results Storage

<!-- Page 859 -->

www.ti.com Basic Operation
859 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.1.10 How toStop aConversion
Agroup 'sconversion canbestopped byclearing thegroup 'schannel select register.
22.2.1.11 Example Sequence forBasic Configuration ofADC Module
The following sequence isnecessary toconfigure theADC toconvert channels 0,2,4,and8insingle-
conversion mode using Group1:
1.Write 0totheReset Control Register (ADRSTCR) torelease themodule from thereset state
2.Write 1totheADC_EN bitoftheOperating Mode Control Register (ADOPMODECR) toenable the
ADC state machine
3.Configure theADCLK frequency byprogramming thedesired divider intotheClock Control Register
(ADCLOCKCR)
4.Configure theacquisition time forthegroup thatistobeused. Forexample, configure theGroup1
Sampling Time Control Register (ADG1SAMP) tosettheacquisition time forGroup1.
5.Select thechannels thatneed tobeconverted inGroup1 bywriting totheGroup1 Channel Select
Register (ADG1SEL). Inthisexample, avalue of0x115 needs tobewritten toADG1SEL inorder to
select channels 0,2,4,and8forconversion inGroup1.
*The ADC sequencer willstart theGroup1 conversions assoon asthewrite totheADG1SEL
register iscompleted.
6.Wait fortheGP1_END bittobesetintheGroup1 Conversion Status Register (ADG1SR). This bitgets
setwhen allthechannels selected forconversion inGroup1 areconverted andtheresults arestored in
theGroup1 memory.
7.Read theconversion results byreading from theGroup1 FIFO access location (ADG1BUFFER) orby
reading directly from theGroup1 results 'memory.

<!-- Page 860 -->

Basic Operation www.ti.com
860 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.2 Advanced Conversion Group Configuration Options
Figure 22-10 shows theoperating mode control registers andthestatus registers foreach ofthethree
conversion groups. The register addresses shown areoffsets from thebase address. The ADC1 register
frame base address isFFF7 C000h andtheADC2 register frame base address isFFF7 C200h.
Figure 22-10. ADC Groups 'Operating Mode Control andStatus Registers
Offset Address
Register31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
0x010
ADEVMODECRReservedNo
Reset
On
ChnSel
Reserved EV_DATA_FMT ReservedEV_
CHIDOVR_
EV_
RAM_
IGNRsvdEV_
8BITEV_
MODEFRZ_
EV
0x014
ADG1MODECRReservedNo
Reset
On
ChnSel
Reserved G1_DATA_FMT ReservedG1_
CHIDOVR_
G1_
RAM_
IGNRsvdG1_
8BITG1_
MODEFRZ_
G1
0x018
ADG2MODECRReservedNo
Reset
On
ChnSel
Reserved G2_DATA_FMT ReservedG2_
CHIDOVR_
G2_
RAM_
IGNRsvdG2_
8BITG2_
MODEFRZ_
G2
0x06C
ADEVSRReserved
ReservedEV_
MEM_
EMPTYEV_
BUSYEV_
STOPEV_
END
0x070
ADG1SRReserved
ReservedG1_
MEM_
EMPTYG1_
BUSYG1_
STOPG1_
END
0x074
ADG2SRReserved
ReservedG2_
MEM_
EMPTYG2_
BUSYG2_
STOPG2_
END
0x19C
ADEVCURRCOUNTReserved
Reserved EV_CURRENT_COUNT
0x1A0
ADEVMAXCOUNTReserved
Reserved EV_MAX_COUNT
0x1A4
ADG1CURRCOUNTReserved
Reserved G1_CURRENT_COUNT
0x1A8
ADG1MAXCOUNTReserved
Reserved G1_MAX_COUNT
0x1AC
ADG2CURRCOUNTReserved
Reserved G2_CURRENT_COUNT
0x1B0
ADG2MAXCOUNTReserved
Reserved G2_MAX_COUNT

<!-- Page 861 -->

www.ti.com Basic Operation
861 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.2.1 Group Trigger Options
The Group1 andGroup2 operating mode control registers have anextra control bit:HW_TRIG. This bit
configures thegroup tobehardware event-triggered instead ofsoftware-triggered, which isthedefault.
When agroup isconfigured tobeevent-triggered, thegroup conversion starts when atleast onechannel
isselected forconversion inthisgroup, andwhen thedefined event trigger occurs. The event trigger
source isdefined foreach group intheADEVSRC, ADG1SRC, andtheADG2SRC registers. The actual
connections used astheevent trigger sources aredefined inthedevice datasheet forboth theADC
modules.
22.2.2.2 Analog Input Channel Selection Mode Options
The ADC1 module onthisdevice supports twodifferent modes forselecting theanalog input channel to
beconverted:
*Sequential channel selection mode (default)
*Enhanced channel selection mode
NOTE: ADC2 module only supports thesequential channel selection mode (the default).
22.2.2.2.1 Sequential Channel Selection Mode
This isthedefault mode andallows theADC module tobeused inabackwards compatible mode tothe
ADC module onother Hercules ™ARM ®Safety MCUs from Texas Instruments. Asdiscussed in
Section 22.2.1.4 ,ananalog input channel canbeselected forconversion inoneormore conversion
groups bysetting thebitcorresponding tothatchannel number inthegroup's channel select register.
22.2.2.2.2 Enhanced Channel Selection Mode
There aresome important concepts related totheenhanced channel selection mode. These aredefined
first:
*Look-Up Table
This isa32-word deep memory-mapped region used todefine theanalog input channel number tobe
converted. The LUTs forthethree groups arestacked together sothattheentire LUT occupies 96
words. Each word isaligned ona32-bit boundary. The LUTs forADC1 start atFF3E 2000h andthe
LUTs forADC2 start atFF3A 2000h.
*Conversion Group Sub-Sequence
Agroup sub-sequence isdefined astheconversion forasetofchannels thatisconverted oneach
conversion trigger. The number ofchannels selected forconversion inagroup sub-sequence is
defined bythenumber ofbitsthataresetinthegroup's channel select register. Forexample, setting
bits0,1,29and31inADG1SEL means thateach Group1 conversion sub-sequence consists of4
conversions.
*LUT Index
A"CURRENT_COUNT" register foreach group ismaintained asanindex intothatgroup's LUT. This
register increments each time achannel conversion iscompleted. Therefore, asitsname suggests, a
read from thisregister returns thenumber ofconversions completed since thelastwrite tothegroup's
channel select register. The CURRENT_COUNT register resets toallzeros under anyofthefollowing
conditions:
1.The ADC peripheral isreset viaaglobal peripheral reset
2.The ADC peripheral isreset viatheADC Reset Control Register
3.The CURRENT_COUNT becomes equal totheMAX_COUNT defined forthatconversion group
4.The application writes zeros totheCURRENT_COUNT register
5.The conversion group's result RAM isreset

<!-- Page 862 -->

Basic Operation www.ti.com
862 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module*Maximum Number ofConversions
AMAX_COUNT register foreach conversion group stores themaximum number ofconversions tobe
performed before theindex intoagroup's LUT isreset to0.This register canbeprogrammed toa
value between 0and31.Itisrecommended toprogram theMAX_COUNT register with avalue thatis
oneless than amultiple ofthenumber ofchannels inthatgroup's conversion sub-sequence (number
ofbitsthataresetinthegroup's channel select register).
22.2.2.2.2.1 Look-Up Table Details
Asdescribed earlier, each conversion group hasalook-up table (LUT) which isused when theenhanced
channel selection mode isenabled. This look-up table starts atanoffset of8kB from thebase oftheADC
results RAM. The LUT holds 32entries foreach ofthethree conversion groups. The first32entries arefor
theevent group, thenext 32entries areforGroup1 andthelast32entries areforGroup2. Figure 22-11
shows anexample LUT entry fortheEvent group.
Figure 22-11. Example Look-Up Table Entry
31 16
Reserved
R-0
15 13 12 8 7 5 4 0
Reserved EV_EXT_CHN_MUX_SEL Reserved EV_INT_CHN_MUX_SEL
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-1. ADC Look-Up Table Field Descriptions
Bit Field Value Description
31-13Reserved 0 Reads return 0.Writes have noeffect.
12-8EV_EXT_CHN_MUX_SEL This field defines theexternal analog mux select thatisoutput from theADC module
when theEvent group CURRENT_COUNT register points tothisLUT entry, andwhen the
Event group conversion istriggered with theenhanced channel selection mode enabled.
7-5Reserved 0 Reads return 0.Writes have noeffect.
4-0EV_INT_CHN_MUX_SEL This field defines theinternal analog mux select thatisoutput from theADC module when
theEvent group CURRENT_COUNT register points tothisLUT entry, andwhen theEvent
group conversion istriggered with theenhanced channel selection mode enabled.
This canbeavalue between 0and31,which corresponds totheinternal analog input
channel number between 0and31.Note thatthisdevice only supports 24input channels
forADC1 and16input channels forADC2. Iftheapplication configures anunavailable
channel number intheEV_INT_CHN_MUX_SEL field, theADC willstillperform the
conversion andtheresult willbeindeterminate.

<!-- Page 863 -->

ADIN31
ADIN0On-chip□Input "Multiplexer"External 8:1 Analog Multiplexersto ADC□Sample/Hold□Circuit
012 293031
ADGxSEL 1 0 0 0 0 101293031
IndexExternal,□5-bit Internal,□5-bitChannel□Identifiers
7 30
5 10 29
4 2
1 18:1
8:1
8:1
8:1
LUT index, 0□to□315-bit□Select for□ext.□channel□muxInternal□Channel
Select,□32□bits
Current Max□Count
Count4 2
1 13
2Enable□Strobe
Generator1-bit Enable□or□nEnable
for□ext.□channel□muxStart□Of
Conversion
Increment□on
End□of□ConversionReset□when
Current□Count□=□Max□Count
www.ti.com Basic Operation
863 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.2.2.2.2 Example ADC Conversion Sequence Using Enhanced Channel Selection Mode
Consider theexample conversion Group1 configuration shown inFigure 22-12 .Only bits0and31of
ADG1SEL areset.Assume thatallother bitsinthisregister arezeros.
Incase ofthedefault sequential channel selection mode, thewrite totheADG1SEL register would cause
theGroup1 conversions tostart with channel 0followed bychannel 31.The conversions would then stop
orrepeat inthisorder depending onwhether Group1 isinsingle orcontinuous conversion mode.
Figure 22-12. Group1 Enhanced Channel Selection Mode Example

<!-- Page 864 -->

Basic Operation www.ti.com
864 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleNow suppose thattheapplication hasenabled theenhanced channel selection mode forGroup1 with the
G1_MAX_COUNT register configured tobe3.Also suppose thattheapplication hasprogrammed the
Group1 LUT asshown inFigure 22-12 .Now suppose thattheapplication triggers Group1 conversions by
writing 0x80000001 toADG1SEL, thatis,bits0and31aresetandallothers arezeros. The ADC
conversions willproceed inthefollowing sequence:
*Input Channel Selection
The initial value ofG1_CURRENT_COUNT is0,which isused astheindex intotheGroup1 LUT. The
row0ofGroup1's LUT hasvalues of1fortheG1_EXT_CHN_MUX_SEL andavalue of1forthe
G1_INT_CHN_MUX_SEL. The 5-bit external channel idof0b00001 isdriven outontheAD1EXT_SEL
terminals. This selects channel 1foralltheconnected external analog multiplexors, asshown in
Figure 22-12 .The ADC module also outputs anenable signal totheexternal analog multiplexors via
theAD1EXT_ENA terminal.
Now consider thefactthattheinternal channel idisalso configured tobe1inrow0oftheGroup1
LUT. This causes theswitch forADC's internal channel 1(ADIN1) tobeclosed. Allother internal ADC
input switches (ADIN0, ADIN2, ADIN3, ...,ADIN31) willbeopen. Note thattheADIN1 input channel is
actually connected totheoutput ofan8:1analog multiplexor.
Ineffect, theADC willconvert channel 1ofthe8:1analog multiplexor connected totheADIN1 terminal
ofthemicrocontroller.
*After Completion ofConversion
Once thefirstconversion iscompleted, theCURRENT_COUNT value of0isstored inthe"channel id"
field oftheconversion result RAM ofGroup1 along with theactual conversion result from theADC
core. Then theG1_CURRENT_COUNT value of0iscompared against theG1_MAX_COUNT value of
3.The values donotmatch, sothatG1_CURRENT_COUNT isincremented from 0to1.
*Next Channel Selection
There aretwobitssetintheADG1SEL register, sothattheADC module now uses the
G1_CURRENT_COUNT value of1toindex theGroup1 LUT. Asshown inFigure 22-12 ,thisrowin
Group1 LUT contains 4astheG1_EXT_CHN_MUX_SEL and2astheG1_INT_CHN_MUX_SEL.
ADC input channel ADIN2 isnotconnected toanyexternal analog multiplexor andisconnected
directly totheanalog signal tobeconverted. Note thattheADC module stilldrives theAD1EXT_ENA
andtheAD1EXT_SEL (value of4,thatis,0b00100) toalltheexternal analog multiplexors connected
tothemicrocontroller.
*End ofConversion Sub-Sequence
Once theconversion oftheinternal channel ADIN2 iscompleted, theG1_CURRENT_COUNT of1is
stored inthe"channel id"field oftheGroup1 result RAM along with theactual conversion result. This
value of1iscompared against theG1_MAX_COUNT value of3.The values donotmatch, sothat
G1_CURRENT_COUNT isincremented from 1to2.
There arenomore conversions required inthissub-sequence asonly twobitsaresetinADG1SEL.
*Continuation onNext Group1 Trigger
When theADC Group1 istriggered again orifGroup1 isincontinuous conversion mode, the
G1_CURRENT_COUNT of2isagain used toindex theGroup1 LUT. Following thesame reasoning as
before, thiswillcause thechannel 1ofthe8:1analog multiplexor connected toADIN1 tobeconverted.
Once thisconversion isdone, theG1_CURRENT_COUNT value of2isstored inthe"channel id"field
oftheresult RAM along with theconversion result. This stilldoes notmatch theG1_MAX_COUNT of
3,sothatG1_CURRENT_COUNT isnow incremented from 2to3.
This index value of3isused toagain convert channel ADIN2, following thesame reasoning asbefore.
When thisconversion iscompleted, theG1_CURRENT_COUNT of3isstored asthe"channel id"field
oftheresult RAM along with theconversion result.
Also, now thisG1_CURRENT_COUNT value of3matches theG1_MAX_COUNT. This resets the
G1_CURRENT_COUNT to0.
The sequence proceeds asdescribed whenever Group1 isnext triggered, orifGroup1 isconfigured tobe
inacontinuous conversion mode.

<!-- Page 865 -->

www.ti.com Basic Operation
865 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.2.3 Single orContinuous Conversion Modes
The EV_MODE, G1_MODE, andG2_MODE bitsareused toselect between either single orcontinuous
conversion mode foreach ofthethree groups.
22.2.2.3.1 Single Conversion Mode
Aconversion group configured tobeinsingle-conversion mode gets serviced only once bytheADC for
each group trigger. The trigger canbeasoftware trigger asinthecase ofGroup1 andGroup2 bydefault,
oritcould beahardware event trigger asinthecase oftheEvent Group orGroup1 orGroup2.
The entire conversion sequence, from theacceptance ofthegroup conversion request totheendofthe
lastchannel 'sconversion, isflagged foreach group bythecorresponding BUSY bitinthatgroup 'sstatus
register. After single-conversion mode isstarted, theBUSY bitisread as1until theconversion ofthelast
channel iscomplete. The END bitforthegroup issetonce allthechannels inthatgroup areconverted.
Forexample, saychannels 0,2,4,and6areselected forconversion inGroup1 insingle-conversion
mode. When theGroup1 gets serviced, theADC willstart conversion forchannel 0,then channel 2,then
channel 4,andthen channel 6.Itwillthen stop servicing theGroup1, settheGP1_END status bit,and
look toservice theEvent Group ortheGroup2, ifrequired.
22.2.2.3.2 Continuous Conversion Mode
Aconversion group configured tobeincontinuous-conversion mode gets serviced bytheADC
continuously. The group stillneeds tobetriggered appropriately forthefirstconversion tostart. The
conversions areperformed continuously thereafter.
The entire conversion sequence, from theacceptance ofthegroup conversion request totheendofthe
lastchannel 'sconversion, isflagged foreach group bythecorresponding BUSY bitinthatgroup 'sstatus
register. After continuous-conversion mode isstarted, theBUSY bitisread as1aslong asthe
continuous-conversion mode forthisgroup isselected.
Asanexample, saythechannels 0,2,4,and6areselected forconversion inGroup1, now incontinuous-
conversion mode. When theGroup1 gets serviced, theADC willcomplete conversions forchannels 0,2,4
and6,andthen look toservice theEvent Group ortheGroup2. Once itisdone servicing theEvent Group
ortheGroup2, itwillreturn toservice theGroup1 again. The Group1 does notneed tobetriggered again
fortherepeated conversion.
NOTE: Configuring allconversion groups incontinuous conversion mode
Allthethree groups cannot operate incontinuous-conversion mode atthesame time. Ifthe
application program configures allthree groups tobeincontinuous-conversion mode, the
Group2 isautomatically reset tosingle-conversion mode, andtheG2MODE bitinthe
ADG2MODECR register iscleared toreflect thesingle-conversion mode ofGroup2.
22.2.2.4 Conversion Group Freeze Capability
The ADC module hasaninherent priority order between thethree conversion groups. This group priority
determines theorder ofconversion incase multiple groups aretriggered. The priority ofconversions
between thethree groups indescending order is:
1.Event Group
2.Group1
3.Group2

<!-- Page 866 -->

Basic Operation www.ti.com
866 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleExamples ofconversion group priority:
*IfanEvent Group conversion isongoing insingle conversion sequence mode andGroup2 andGroup1
conversions arerequested, then theADC willfinish conversion ofchannels selected inEvent Group,
then switch over toconverting channels selected inGroup1, andthen convert channels selected in
Group2.
*IfGroup1 conversions areongoing incontinuous conversion mode andGroup2 conversion is
requested, then theADC willcomplete converting thecurrent channel forGroup1 andswitch over to
converting channels selected inGroup2. The new conversion request forGroup2 hasahigher priority
than thepending continuous conversion request forGroup1.
The conversion group freeze capability allows theapplication tooverride thisdefault priority between the
conversion groups. Enabling thefreeze capability allows theADC tofreeze ahigher-priority conversion
group 'sconversions whenever there isarequest forconversion inanother (lower-priority) group.
Forexample, setting theFRZ_EV bitintheADEVMODECR register willallow theADC tofreeze ongoing
Event Group conversions whenever there isapending request, oranew request foraGroup1 orGroup2
conversion. The conversions fortheEvent Group willbefrozen aslong astheGroup1 orGroup2
conversions areactive. Once theGroup1 orGroup2 conversions arecompleted, theEvent Group
conversions start from where they were frozen.
While agroup 'sconversions arefrozen, thegroup 'sSTOP status bitisset.This bitiscleared once the
group 'sconversions arerestarted.
22.2.2.5 Conversion Group Memory Overrun Option
Anoverrun condition occurs when theADC module tries tostore more conversion results toagroup 's
results 'memory which isalready full.Inthiscase, theADC allows twooptions.
IftheOVR_RAM_IGN bitinthegroup 'soperating mode control register (ADEVMODECR,
ADG1MODECR, ADG2MODECR) isset,then theADC module ignores thecontents ofthegroup 'sresults '
memory andwraps around tooverwrite thememory with theresults ofnew conversions.
IftheOVR_RAM_IGN bitisnotset,then theapplication program hastoread outthegroup 'sresults '
memory upon anoverrun condition; only then cantheADC continue towrite new results tothememory.
22.2.2.6 Response onWriting Non-Zero Value toConversion Group 'sChannel Select Register
Iftheapplication writes anon-zero value toagroup 'schannel select register while thatgroup 's
conversions arealready being serviced, then thatgroup 'sconversions willberestarted with thenew
configuration programmed inthechannel select registers.
The following rules apply interms oftheeffect ontheADC conversion sequence:
*Ifthenew conversion request comes from thesame group astheongoing conversion, then the
ongoing conversion willbestopped inwhichever stage itisin,andthenew sequence ofconversions
willbestarted.
*Ifthenew conversion request comes from aseparate group, then theongoing channel 'sconversion
willbecompleted before starting thenew sequence ofconversions.
The following rules apply interms oftheeffect onthegroup 'sresults memory:
*Ifagroup conversion isongoing orisfrozen, writing anon-zero value tothegroup 'schannel select
register willalso reset itsresults FIFO. This does notclear thecontents oftheresults FIFO; only the
ADC module isallowed tooverwrite theFIFO 'scontents with new conversion results starting from the
firstlocation.
*Ifthegroup conversion iscompleted (<GRP >_END flagisset), orthegroup isnotbeing used, then
writing anon-zero value tothegroup 'schannel select register willeither bereset ornotdepending on
thevalue oftheNoResetOnChnSel bitforthatgroup (ADEVMODECR, ADG1MODECR,
ADG2MODECR).
-IftheNoResetOnChnSel bitis0,then thegroup 'sFIFO willbereset.
-IftheNoResetOnChnSel bitis1,then thegroup 'sFIFO willnotbereset.

<!-- Page 867 -->

www.ti.com Basic Operation
867 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.2.7 Conversion Result Size onReading: 8-bit, 10-bit, or12-bit
Some applications donotneed thefull12-bit resolution oftheADC modules onthedevice andcanwork
with 8-bit or10-bit conversion results.
22.2.2.7.1 ADC Configured in12-bit Resolution
The mode control register foreach conversion group contains afield called DATA_FMT, which defines the
format oftheconversion result read outoftheresult RAM, when accessed asaFIFO.
The DATA_FMT field isencoded asfollows:
*IfDATA_FMT =0,thecomplete 12-bit conversion result isread outoftheFIFO.
*IfDATA_FMT =1h,the12-bit conversion result isright-shifted by2andtheresulting 10-bit result is
read outoftheFIFO.
*IfDATA_FMT =2h,the12-bit conversion result isright-shifted by4andtheresulting 8-bit result isread
outoftheFIFO.
This control field isnoteffective when theapplication chooses toaccess theconversion result memory
directly. Inthatcase, theapplication canchoose tomask offthenumber ofbitsasrequired.
22.2.2.7.2 ADC Configured in10-bit Resolution
The DATA_FMT field isnoteffective inthismode andtheapplication hasthechoice toread either thefull
10-bit conversion result oran8-bit conversion result. This iscontrolled bythe8BIT field ofthegroup 's
operating mode control register.
*If8BIT =0,thecomplete 10-bit conversion result isread outoftheFIFO.
*If8BIT =1,the10-bit conversion result isright-shifted by2andtheresulting 8-bit result isread outof
theFIFO.
22.2.2.8 Option toRead Group Channel IDAlong With Conversion Result
The ADC module allows theapplication program toalso read outtheanalog input channel number along
with itsconversion result. This capability isenabled bysetting theCHID bitinthegroup 'soperating mode
control register.
*IfCHID =0,bits[14-10] areforced to00000 when theconversion results areread outfrom thegroup 's
results 'FIFO.
*IfCHID =1,bits[14-10] inthegroup 'sresults 'memory contain theinput channel number towhich the
conversion result belongs.
NOTE: Actual Storage ofChannel ID
Regardless ofwhether theCHID bitissetornot,thechannel number isalways stored in
thememory along with theconversion result. The CHID bitonly affects whether thechannel
number isavailable with theconversion result when thegroup 'smemory isread .
Therefore, theCHID bitforagroup canbechanged dynamically without affecting that
group 'songoing conversions.

<!-- Page 868 -->

Basic Operation www.ti.com
868 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.3 ADC Module Basic Interrupts
This section describes thebasic interrupts generated bytheADC module.
22.2.3.1 Group Conversion End Interrupt
The ADC module sets thegroup 'sconversion endflag(EV_END, G1_END, orG2_END) inthatgroup 's
interrupt flagregister (ADEVINTFLG, ADG1INTFLG, ADG2INTFLG) when allthechannels selected for
conversion inthatgroup areconverted. This causes agroup conversion endinterrupt tobegenerated if
thisinterrupt isenabled bysetting thegroup 'sEND_INT_EN control bit(EV_END_INT_EN,
G1_END_INT_EN, orG2_END_INT_EN).
This interrupt canbeeasily used forconversion groups configured tobeinthesingle-conversion mode.
The application program canread outtheconversion results, change thegroup 'sconfiguration if
necessary, andrestart theconversions bytriggering thegroup from within theinterrupt service routine.
Forgroups configured tobeincontinuous conversion mode, thisinterrupt condition isnotpractical asthe
conversions arealways inprogress. Inthiscase, theGroup Memory Threshold Interrupt ismore practical
astheapplication canallow aprogrammable number ofconversion results toaccumulate before
interrupting theCPU.
22.2.3.2 Group Memory Threshold Interrupt
The ADC module hastheability togenerate aninterrupt forafixed number ofconversions foreach group.
Agroup memory threshold register determines how many conversion results must beinagroup 'smemory
region before theCPU isinterrupted. This feature canbeused tosignificantly reduce theCPU load when
using interrupts forreading theconversion results.
The group 'sthreshold register needs tobeconfigured before thegroup conversions aretriggered. This
threshold register value behaves likeadown-counter, which decrements each time theADC writes a
conversion result tothisgroup 'smemory. This counter isincremented each time theapplication program
reads aconversion result from theresults 'memory byaccessing theFIFO queue. Simultaneous read (by
application program) andwrite (byADC module) operations from thegroup 'sresults 'memory leave the
threshold counter unchanged.
The threshold counter candecrement past 0andbecome negative. Italways increments back toits
original value when thememory region isemptied. Todetermine how many samples areinthememory
region atagiven moment, thethreshold counter canbesubtracted from theoriginally configured threshold
count.
Whenever thethreshold counter transitions from +1to0,itsets thegroup 'sthreshold interrupt flag, and
theCPU isinterrupted ifthegroup 'sthreshold interrupt isenabled. The CPU isexpected toclear the
interrupt flagafter reading theconversion results from thememory.
The interrupt flagisnotsetwhen thethreshold counter stays at0ortransitions from -1to0.
22.2.3.3 Group Memory Overrun Interrupt
Aninterrupt canbegenerated foreach group ifthenumber ofADC conversions forthatgroup exceed the
number ofbuffers allocated forthatconversion group. The application program canchoose toread outall
theconversion results using theCPU ortheDMA. Alternatively, theapplication program cansetthe
group 'sOVR_RAM_IGN bitandallow theADC module tooverwrite thegroup 'sresults 'memory contents
with new conversion results.

<!-- Page 869 -->

www.ti.com Basic Operation
869 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.4 ADC Module DMA Requests
This section describes thecapabilities oftheADC module totake advantage ofthePlatform DMA
controller module. The ADC module cangenerate aDMA request under twoconditions:
22.2.4.1 DMA Request forEach Conversion Result Written totheResults 'Memory
Inthismode, theADC module willgenerate thefirstDMA request assoon asaconversion result gets
written tothegroup 'sresults 'memory. Subsequent writes totheresults 'memory willcause DMA requests
tobegenerated. This mode allows asmaller amount ofADC results 'memory tosuffice foranapplication.
This DMA request generation isenabled bysetting thegroup 'sDMA_EN bitinthegroup 'sDMA control
register. The BLK_XFER bitinthisregister must beleftcleared (default), ifaDMA request isdesired tobe
generated fornew results getting written totheresults 'memory.
22.2.4.2 DMA Request foraFixed Number ofConversion Results
This mode isenabled bysetting both thegroup 'sDMA_EN andthegroup 'sBLK_XFER bitsinthegroup 's
DMA control registers.
Inthismode, aDMA request willbegenerated foraspecified number ofconversion results being
available inthegroup 'sresults 'memory. The number ofconversion results desired areconfigured using
thegroup 'sBLOCKS field inthecontrol registers.
Forexample, iftheBLOCK count isconfigured for10,then ADC module willgenerate aDMA request at
theendof10th conversion. DMA controller should complete reading out10data before next setof10
conversions complete.
NOTE: Usage ofBlock DMA transfers with Threshold Interrupts
Itisnotrecommended toenable theblock DMA transfers foragroup atthesame time asthe
group threshold interrupt. The group 'sBLOCKS field isessentially thesame asthegroup 's
THRESHOLD field inthegroup 'sinterrupt control register described inSection 22.2.3.2 .

<!-- Page 870 -->

Basic Operation www.ti.com
870 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.5 ADC Magnitude Threshold Interrupts
The ADC allows uptothree magnitude threshold interrupts tobegenerated. The comparison parameters
areprogrammed viatheMagnitude Threshold Control Register (ADMAGINTxCR).
22.2.5.1 Magnitude Threshold Interrupt Configuration
The following fields areconfigurable foreach ofthethree available magnitude threshold interrupts:
1.CHN_THR_COMP: Specifies whether tocompare twochannels 'conversion results, ortocompare a
channel 'sconversion result toaprogrammable threshold value. Avalue of0willselect the
programmable threshold tobecompared, andavalue of1willselect theconversion result ofthe
channel identified bytheCOMP_CHID field tobecompared.
2.MAG_CHID: Specifies thechannel number from 0to31whose conversion result needs tobe
monitored.
3.COMP_CHID: Specifies thechannel number from 0to31whose lastconversion result isused forthe
comparison with theconversion result ofthechannel being monitored.
4.MAG_THRESHOLD: Specifies thevalue forcomparison with theconversion result ofthechannel
identified bytheMAG_CHID field.
5.CMP_GE_LT: Specifies whether theconversion result ofthechannel identified byMAG_CHID is
compared tobe"greater than orequal to",or"less than"thereference value. The reference value can
betheconversion result ofanother channel identified bytheCOMP CHID field, oritcould bea
threshold value specified intheMAG_THRESHOLD field. Avalue of0intheCMP_GE_LT field
indicates a"less than"comparison andavalue of1indicates a"greater than orequal to"comparison.
22.2.5.2 Magnitude Threshold Interrupt Comparison Mask Configuration
There isalso aseparate comparison mask register (ADMAGINTxMASK) foreach ofthethree magnitude
threshold interrupts. This register isused tospecify thebitsthataremasked offforthesake ofthe
comparison. Forexample, thelower 4bitsoftheconversion result canbemasked offbywriting 0xftothe
interrupt comparison mask register, allowing agross comparison tobemade. Bydefault, thefull10/12-bit
conversion results arecompared.
22.2.5.3 Magnitude Threshold Interrupt Enable /Disable Control
Each ofthethree magnitude interrupts also have separate interrupt enable set(ADMAGINTENASET) and
clear (ADMAGINTENACLR) registers. These areused torespectively enable anddisable thatparticular
magnitude threshold interrupt from being generated. Toenable amagnitude threshold interrupt, write a1
tothecorresponding bitoftheinterrupt enable setregister. Conversely, todisable amagnitude threshold
interrupt, write a1tothecorresponding bitoftheinterrupt enable clear register.
22.2.5.4 Magnitude Threshold Interrupt Flags
There isaseparate Magnitude Interrupt Flag register (ADMAGINTFLG) thatholds theflags forthese three
interrupts. This flaggets setwhenever thecomparison condition forthecorresponding interrupt ismet. A
magnitude threshold interrupt isgenerated ifthecorresponding flagissetinside theflagregister, andthe
interrupt generation isenabled. This flagcanbecleared bywriting a1totheflagorbyreading from the
interrupt offset register incase ofthisinterrupt being thecurrent highest-priority pending interrupt.
22.2.5.5 Magnitude Threshold Interrupt Offset Register
Itispossible tohave multiple magnitude threshold interrupts pending atthesame time. The magnitude
threshold interrupt offset register (ADMAGINTOFF) holds theindex ofthecurrently pending highest
priority magnitude threshold interrupt. The magnitude threshold interrupt 1hasthehighest priority while
themagnitude threshold interrupt 3hasthelowest priority. This isaread-only register andreturns zeros if
none ofthemagnitude threshold interrupts arepending. Writes tothisregister have noeffect.
Aread from thisregister updates theregister tothenext highest-priority pending magnitude threshold
interrupt. This read also clears thecorresponding flagfrom themagnitude threshold interrupt flagregister.
However, aread from themagnitude threshold interrupt offset register inemulation mode does notaffect
theinterrupt flagregister ortheinterrupt offset register.

<!-- Page 871 -->

ADIN0
ADIN31Self-test and
calibrationADREFLOADREFHI
VinR1 R2S4 S1S2 S3
S5ADC Core
ADDRx.16,9:0ADCALR.9:0CALRMUXR1 ~ 5K
R2 ~ 7K
www.ti.com Basic Operation
871 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.6 ADC Special Modes
The ADC module supports some special modes fordiagnostics andpower saving purposes.
22.2.6.1 ADC Error Calibration Mode
The application program canactivate acalibration sequence anytime self-test mode isdisabled
(SELF_TEST =0).This calibration sequence includes theconversion ofanembedded calibration
reference voltage followed bythecalculation ofanoffset error correction value.
NOTE: Disable Self-Test Mode Before Calibration
Toavoid errors during thecalibration operation, self-test mode must notbeenabled during a
calibration sequence. Inaddition, toensure accurate results, calibrate theADC inan
environment with minimum noise.
Calibration mode isenabled bysetting theCAL_EN bit(ADCALCR.0). The application needs toensure
thatnoconversion group isbeing serviced when thecalibration mode isenabled.
The input multiplexor gets disabled andonly thereference voltage isconnected totheADC core input.
Switch S5ofFigure 22-13 isopened. Inaddition, thedigital result issued from aconversion isoutput from
theADC core tothecalibration andoffset error correction register, ADCALR. The ADC results 'memory is
notaffected bythecalibration conversion.
When calibration mode isdisabled, theADC canbeconfigured fornormal conversions.
Figure 22-13. Self-Test andCalibration Logic
22.2.6.1.1 Calibration Conversion
The calibration conversion also needs tomeet theminimum sampling time specification fortheADC. This
value istypically 1us.The Event Group sample time register (ADEVSAMP) isused tospecify thenumber
ofADCLK cycles forthecalibration conversion.
The BRIDGE_EN andHILO bits(ADCALCR.9:8) control thevoltage tothecalibration reference device
shown inFigure 22-15 .The positions oftheswitches incalibration mode arelisted inTable 22-2.

<!-- Page 872 -->

Basic Operation www.ti.com
872 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module(1)The state oftheswitches inthistable assumes thatself-test mode isnotenabled.Table 22-2. Calibration Reference Voltages(1)
CAL_EN BRIDGE_EN HILO S1 S2 S3 S4 S5 Reference Voltage
1 0 0 1 0 1 0 0 (ADREFHI ×R1+ADREFLO ×R2)/(R1+R2)
1 0 1 0 1 0 1 0 (ADREFLO ×R1+ADREFHI ×R2)/(R1+R2)
1 1 0 0 1 1 0 0 ADREFLO
1 1 1 1 0 0 1 0 ADREFHI
0 X X 0 0 0 0 1 Vin
When CAL_ST (ADCALCR.16) isset,acalibration conversion isstarted. The voltage source selected via
thebitsBRIDGE_EN andHILO isconverted once (single conversion mode) andthedigital result is
returned tothecalibration andcorrection register, ADCALR, where itcanberead bytheCPU. The
CAL_ST bitacts asaflagandmust bepolled bytheCPU. Itisheld setduring theconversion process and
automatically clears toindicate theendofthereference voltage conversion.
NOTE: NoInterrupt forendofcalibration
The ADC does notgenerate aninterrupt tosignal theendofthecalibration conversion. The
application must polltheCAL_ST bittodetermine theendofthecalibration conversion.
After theCAL_ST bitissetbytheapplication program, itcanonly bereset bytheendoftheongoing
conversion generated bytheADC core. Ifthecalibration conversion isinterrupted (CAL_EN bitiscleared),
theCAL_ST bitisheld at1until anew calibration conversion hasbeen setandcompleted. Setting the
CAL_ST bitwhile calibration isdisabled (CAL_EN =0)hasnoeffect; however, inthissituation, setting
CAL_EN immediately starts acalibration conversion. When thecalibration conversion isinterrupted byan
ADC_Enable (ADC_EN =0,CAL_EN =1,andCAL_ST =1),anew conversion isautomatically restarted
assoon astheADC_Enable bitisreleased (ADC_EN =1).
22.2.6.1.2 Calibration andOffset Error Correction Sequences
The number ofmeasurements andthesource tomeasure foranADC calibration areapplication
dependent. The CAL_ST bitmust besetforeach calibration source tobemeasured. While calibration
mode isenabled, anyavailable calibration sources canbeconverted according totheBRIDGE_EN and
HILO bits(see Table 22-2).The digital results ofthecalibration measurements should beread from
ADCALR bytheapplication after each reference conversion sothatacorrection value canbecomputed
andwritten back intoADCALR.
When theapplication hasthenecessary calibration data, itshould compute theoffset error correction
value andload itintothecalibration andcorrection register, ADCALR. After theCAL_EN bitiscleared,
normal conversion mode restarts, continuing from where itwas frozen, butwith theaddition ofself-
correction data.
Innormal mode, theself-correction system adds thecorrection value stored inADCALR toeach digital
result before itiswritten totherespective group 'sFIFO.
The basic calibration routine isasfollows:
1.Enable calibration viaCAL_EN (ADCALCR.0).
2.Select thevoltage source viaBRIDGE_EN andHILO (ADCALCR.9:8).
3.Start theconversion with CAL_ST (ADCALCR.16).
4.Wait forCAL_ST togoto0.
5.Gettheresults from ADCALR andsave tomemory.
6.Loop tostep 2until thecalibration conversion data iscollected forthedesired reference voltages.
7.Compute theerror correction value using calibration data saved inmemory.
8.Load theADCALR register with the2scomplement ofthecomputed error correction value.
9.Disable calibration mode.

<!-- Page 873 -->

www.ti.com Basic Operation
873 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleAtthispoint, theADC canbeconfigured fornormal operation, anditcorrects each digital result with the
error correction value loaded inADCALR.
NOTE: Prevent ADC Calibration Data From Being Overwritten
Incalibration mode, theconversion result iswritten toADCALR thatoverwrites anyprevious
calibration data; therefore, theADCALR register must beread before anew conversion is
started.
Fornocorrection, avalue of0x0000 must bewritten toADCALR. Innoncalibration mode, theADCALR
register canberead andwritten. Any value written toADCALR innormal mode (CAL_EN =0)isadded to
each digital result from theADC core.
22.2.6.1.3 Mid-Point Calibration
Because ofitsconnections totheADC 'sreference voltage (VrefHi, VrefLo), theprecision ofthecalibration
reference isvoltage independent. Ontheother hand, theaccuracy oftheswitched bridge resistor (R1&
R2)relies onthemanufacturing process deviation. Consequently, themid-point voltage 'saccuracy canbe
affected duetotheimperfections inthetworesistors (expected mismatch error isaround 1.5%).
The switched reference voltage device hasbeen specially designed tosupport adifferential measurement
ofitsmid-point voltage. This ensures theaccuracy ofthemid-point reference, andhence theefficiency of
thecalibration.
The differential mid-point calibration issoftware controlled; thealgorithm (voltage source measurements
andassociated calculation) isinserted within thecalibration software module included intheapplication
program.
The basic differential mid-point calibration flow isillustrated here after:
1.The application program connects thevoltage VrefHi toR1andVrefLo toR2,(BRIDGE_EN =0,
HILO =0),launches aconversion oftheinput voltage V(cal1), andstores thedigital result D(cal1) into
thememory.
2.Then theapplication program switches thevoltage VrefHi toR2andVrefLo toR1(BRIDGE_EN =0,
HILO =1),converts thisnew input voltage V(cal2) andagain stores theissued digital result D(cal2)
intothememory.
3.The actual value oftherealmiddle point isobtained bycomputing theaverage ofthese tworesults.
[D(cal1)+D(cal2)] /2;Figure 22-14 summarizes themid-point calibration flow.

<!-- Page 874 -->

3FF
R
Vin
V(cal1)
V(cal1) = [VREFHI*R1+VREFLO*R2] / (R1 + R2)
V(cal2) = [VREFLO*R1+VREFHI*R2] / (R1 + R2)
[V(cal1) + V(cal2)] / 2 = (VrefHi-VrefLo) / 2Digital Code (hex)
D(cal1)D(cal)D(cal2)
[D(cal1) + D(cal2)] / 2 = D(cal)*Real10-bit ADC's Theoretical
Transfer Function
VrefHi VrefLo V(cal2)* The Real function shown is a straight
line between the ends points of the real
staircase characteristic.
The Theoretical transfer function is
for reference only.
straight line
Transfer Function
CPUMEMORY
D(cal1)
D(cal2)(VrefHi - VrefLo)/2FS
Basic Operation www.ti.com
874 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleFigure 22-14. Mid-point Value Calculation
22.2.6.2 ADC Self-Test Mode
The ADC module supports aself-test mode which canbeused todetect anopen orashort ontheADC
input channels. Self-test mode isenabled bysetting theSELF_TEST bit(ADCALCR.24). Any conversion
type (continuous orsingle conversion, freeze enabled ornon-freeze enabled, interrupts enabled or
disabled) canbeperformed inthismode.
Innormal mode, setting theself-test mode while aconversion sequence isinprocess cancorrupt the
current channel conversion results. However, thenext channel inthesequence isconverted correctly
during theadditional self-test cycle. The logic associated with both self-test andcalibration isshown in
Figure 22-15 .

<!-- Page 875 -->

ADIN0
ADIN31Self-test and
calibrationADREFLOADREFHI
VinR1 R2S4 S1S2 S3
S5ADC Core
ADDRx.16,9:0ADCALR.9:0CALRMUXR1 ~ 5K
R2 ~ 7K
www.ti.com Basic Operation
875 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleFigure 22-15. Self-Test andCalibration Logic
Inself-test mode, atestvoltage defined bytheHILO bit(ADCALCR.8) isprovided totheADC core input
through aresistor (see Table 22-3).Tochange thetestsource, thisbitcanbetoggled before anysingle
conversion mode request. Changing thisbitwhile aconversion isinprogress cancorrupt theresults ifthe
source switches during theacquisition period.
Note thattheswitch S5shown inFigure 22-15 isonly forthepurpose ofexplaining theself-test sequence.
There isnophysical switch.
(1)Switches refer toFigure 22-15 .Table 22-3. Self-Test Reference Voltages(1)
SELF_TEST HILO S1 S2 S3 S4 S5 Reference Voltage
1 0 0 1 1 0 1 ADREFLO viaR1||R2connected toVin
1 1 1 0 0 1 1 ADREFHI viaR1||R2connected toVin
0 X 0 0 0 0 1 Vin
Conversions inself-test mode arestarted justasthey areinthenormal operating mode (see
Section 22.2.1.6 ).The conversion starts according totheconfiguration setinthethree mode control
registers (ADEVMODECR, ADG1MODECR, ADG2MODECR) andthesampling time control registers
(ADEVSAMP, ADG1SAMP, ADG2SAMP). The acquisition time foreach conversion inself-test mode is
extended totwice thenormal configured acquisition time. The selected reference voltage andtheinput
voltage from theADINx input channel areboth connected totheADC internal sampling capacitor
throughout thisextended acquisition period. Figure 22-16 shows theself-test mode timing when the
ADREFLO ischosen asthereference voltage fortheself-test mode conversion. Italso assumes an
external capacitor connected totheADC input channel.

<!-- Page 876 -->

StartTsamp1
ADREFLO + ADINxSample time in normal operation modeSample time doubled in self-test mode
Conversion of last value sampled
AD_Core _InADREFHI
ADREFLOADREFLO + ADINx
timeExt. Input
discharge of ext. cap charging of ext. capTsamp2
Basic Operation www.ti.com
876 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleFigure 22-16. Timing forSelf-Test Mode
22.2.6.2.1 Use ofSelf-Test Mode toDetermine Open/Short onADC Input Channels
The following sequence needs tobeused todeduce theADC pinstatus:
*Convert thechannel with selftestenabled andwith thereference voltage asVreflo. Store the
conversion result, sayVd.
*Convert thechannel with selftestenabled andwith thereference voltage asVrefhi. Store the
conversion result, sayVu.
*Convert thechannel with selftestdisabled. Store theconversion result, sayVn.
The results canbeinterpreted using thefollowing table.
Table 22-4. Determination ofADC Input Channel Condition
Normal Conversion
Result, VnSelf-test Conversion
Result, VuSelf-test Conversion
Result, Vd PinCondition
Vn Vn<Vu<ADREFHI ADREFLO <Vd<Vn Good
ADREFHI ADREFHI approx. ADREFHI Shorted toADREFHI
ADREFLO approx. ADREFLO ADREFLO Shorted toADREFLO
Unknown ADREFHI ADREFLO Open
22.2.6.3 ADC Power-Down Mode
This isaninactive mode inwhich theclocks totheADC module arestopped leaving themodule inastatic
state. The clock totheADC core (ADCLK) isstopped whenever there arenoongoing conversions. This is
theclock-gating implementation requirement. Also, theADC module places theADC core intothepower
down mode such thatthere isminimal current drawn from theADC operating andreference supplies.
22.2.6.3.1 Powering Down Just TheADC Core
The ADC core canbeindividually powered down without stopping theclocks totheADC module. This can
bedone bysetting thePOWERDOWN bitoftheADC Operating Mode Control Register
(ADOPMODECR.3). Whenever aconversion isrequired thePOWERDOWN bitmust becleared, anda
minimum time td(PU-ADV) ,(see thespecific device data sheet foractual value) hastobeallowed before
starting anew conversion. This wait must beimplemented intheapplication software.

<!-- Page 877 -->

StartTdischarge
VrefloSampling time
T samp
ADINxSample cap discharge time
Conversion of last value sampled
www.ti.com Basic Operation
877 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.6.3.2 Enhanced Power-Down Mode
AbitintheADC operating mode control register, IDLE_PWRDN (ADOPMODECR.4) enables the
enhanced power-down mode oftheADC.
Once thisbitisset,theADC module willpower down theADC core whenever there arenomore ongoing
orpending ADC conversions. The ADC core willbepowered down regardless ofthestate ofthe
POWERDOWN bit(ADOPMODECR.3).
The ADC module releases theADC core from power down mode assoon asanew conversion is
requested. The ADC logic state machine then hastowait foratleast td(PU-ADV) (see thedevice data sheet
foractual value) before starting anew conversion. The IDLE_PWRDN bitwillremain setatalltimes. The
logic state machine canusethisbittodetermine thatitneeds towait foraprogrammable number ofVCLK
cycles before itallows theinput channel tobesampled. This time isconfigured bytheADC Power Up
Delay Control register (ADPWRUPDLYCTRL).
IfIDLE_PWRDN isnotset,theADC module does notwait foranyadditional delay before sampling the
input channel andtheapplication software hastotake account ofthisrequired delay.
22.2.6.3.3 Managing Clocks totheADC Module
The clock totheADC module canbeturned offviatheappropriate Peripheral Central Resource (PCR)
controller PSPWRDNSET register (check thespecific device datasheet toidentify theregister andthebit
tobeset). Ifaconversion isongoing when thisbitisset,theADC module willwait until thecurrent
conversion completes before allowing theADC module clock tobestopped.
22.2.6.4 ADC Sample Capacitor Discharge Mode
This mode allows thecharge ontheADC core'sinternal sampling capacitor tobedischarged before
starting thesampling phase ofthenext channel.
The ADC Sample Cap Discharge Mode isenabled bysetting theSAMP_DIS_EN bitofthegroup 's
ADSAMPDISEN register. Adischarge period forthesampling capacitor isadded before thesampling
period foreach channel asshown inFigure 22-17 .The duration ofthisdischarge period isconfigurable via
thecorresponding group 's_SAMP_DIS_CYC field intheADSAMPDISEN register. The discharge time is
specified interms ofnumber ofADCLK cycles.
During thesample capacitor discharge period, theVREFLO reference voltage isconnected totheinput
voltage terminal oftheADC core. This allows anycharge collected onthesampling capacitor from the
previous conversion tobedischarged toground. The VREFLO reference voltage isusually connected to
ground.
Figure 22-17. Timing forSample Capacitor Discharge Mode

<!-- Page 878 -->

Basic Operation www.ti.com
878 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.2.7 ADC Results 'RAM Special Features
The following section describes some ofthespecial features supported bytheADC module toenhance
theresults 'RAM testability andintegrity.
22.2.7.1 ADC Results 'RAM Auto-Initialization
The ADC module allows theapplication toauto-initialize theADC results 'RAM toallzeros. The
application must ensure thattheADC module isnotinanyoftheconversion modes before triggering off
theauto-initialization process.
The auto-initialization sequence isasfollows:
1.Enable theglobal hardware memory initialization keybyprogramming avalue of1010 tothebits[3-0]
oftheMINITGCR register oftheSystem module.
2.Setthecontrol bitfortheADC results 'RAM intheMSINENA System module register. The bit8ofthe
MSINENA register isused tocontrol theinitialization oftheADC1 results 'RAM, while bit14controls
theinitialization oftheADC2 results 'RAM. This starts theinitialization process. The
BUF_INIT_ACTIVE flagintheADBNDEND register willgetsettoreflect thattheinitialization is
ongoing.
3.When thememory initialization iscompleted, thecorresponding status bitintheMINISTAT register will
beset.Also, theBUF_INIT_ACTIVE flagwillgetcleared.
22.2.7.2 ADC Results 'RAM Test Mode
Inthedefined conversion modes oftheADC, theapplication canonly read from theADC results 'RAM.
Only theADC module isallowed towrite totheresults 'RAM. Aspecial testmode isdefined toallow the
application toalso write intotheADC results 'RAM -thismode istheADC Results 'RAM Test Mode. Only
32-bit reads andwrites areallowed totheADC results 'RAM inthistestmode.
NOTE: Contention onaccess toADC Results 'RAM
The ADC module cannot handle acontention between theapplication write totheresults '
RAM andtheADC writing aconversion result totheresults 'RAM. The application must
ensure thattheADC isnotlikely towrite anew conversion result totheresults 'RAM when
theADC Results 'RAM Test Mode isenabled.
The ADC Results 'RAM Test Mode isenabled bysetting theRAM_TEST_EN bitintheADOPMODECR.
22.2.7.3 ADC Results 'RAM Parity
The following shows theADC Results 'RAM parity control registers.
Parity checking isimplemented using parity onaper-half word basis fortheADC RAM. That is,there is
oneparity bitfor16bitsoftheADC RAM. The polarity oftheADC RAM parity iscontrolled bythe
DEVCR1 register inthesystem module (address =0xFFFFFFDC). The parity checking isenabled bythe
ADPARCR register. After reset, theparity checking isdisabled andmust beenabled ifparity protection is
required.
During aread access, theparity iscalculated based onthedata read from theADC RAM andcompared
with thegood parity value stored intheparity bits. Ifanyword fails theparity check then theADC
generates anerror signal hooked uptotheError Signaling Module (ESM). The ADC RAM address which
generated theparity error iscaptured forhost system debugging, andisfrozen from being updated until it
isread bytheapplication.
Testing theParity Checking Mechanism:
Totesttheparity checking mechanism itself, theparity RAM ismade writable bytheCPU inaspecial test
mode. This isdone byacontrol bitcalled TEST intheADPAR CRregister. Once thisbitisset,theparity
bitsaremapped toanaddress starting atanaddress offset of4KB from thebase address oftheADC
RAM. See Figure 22-18 .The CPU cannow manually insert parity errors. Note thattheADC RAM only
supports 32-bit accesses.

<!-- Page 879 -->

Output enable
Data out
Data in
Pull control disable
Pull selectADxEVT
Pull control
logicpin
Conversion word 0
Conversion word 1
Conversion word 2
Conversion word 62
Conversion word 63 0xFF3E00FC0xFF3E01F80xFF3E00080xFF3E00040xFF3E0000ADC1
0xFF3A00FC0xFF3A01F80xFF3A00080xFF3A00040xFF3A0000ADC2
BASE ADDRESS
Reserved
0xFF3A1000 0xFF3E1000
Parity Bits
www.ti.com Basic Operation
879 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleFigure 22-18. ADC Memory Map inParity Test Mode
22.2.8 ADEVT PinGeneral Purpose I/OFunctionality
The AD1EVT pinforADC1 andAD2EVT pinforADC2 canbeconfigured asgeneral-purpose I/Osignals.
The following sections describe thedifferent ways inwhich theapplication canconfigure theADxEVT
pins.
22.2.8.1 GPIO Functionality
Figure 22-19 illustrates theGPIO functionality oftheADxEVT pin.
Figure 22-19. GPIO Functionality ofADxEVT

<!-- Page 880 -->

Basic Operation www.ti.com
880 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleOnce thedevice power-on reset isreleased, theADC module controls thestate oftheADxEVT pin.
*Pullcontrol: The pullcontrol caneither beenabled ordisabled bydefault (while system reset isactive
andafter itisreleased). The actual default state ofthepullcontrol isspecified inthedevice datasheet.
The application canenable pullcontrol byclearing thePDIS (pull control disable) bitinthe
ADEVTPDIS register. Inthiscase, ifthePSEL (pull select) bitintheADEVTPSEL register isset,the
pinwillhave apull-up. IfthePSEL bitiscleared, thepinwillhave apull-down. IfthePDIS bitissetin
thecontrol register, there isnopull-up orpull-down onthepin.
NOTE: PullBehavior when ADxEVT isconfigured asoutput
IftheADxEVT pinisconfigured asoutput, then thepulls aredisabled automatically. Ifthepin
isconfigured asinput, thepulls areenabled ordisabled depending onbitPDIS inthepull
disable register ADEVTPDIS.
*Output buffer: The ADxEVT pincanbedriven asanoutput piniftheADEVTDIR bitissetinthepin
direction control register.
*Open-Drain Feature: The open drain output capability isenabled viatheADEVTPDR control register.
The ADxEVT pinmust bealso configured tobeanoutput pinforthismode.
-The output buffer isenabled ifalowsignal isbeing driven ontothepin.
-The output buffer isdisabled ifahigh signal isbeing driven ontothepin.
22.2.8.2 Summary
The behavior oftheoutput buffer, andthepullcontrol issummarized inTable 22-5 .The input buffer for
theADxEVT pins areenabled once thedevice power-on reset isreleased.
(1)X=Don'tcare
(2)DIR =0forinput, 1foroutput
(3)PULDIS =0forenabling pullcontrol, 1fordisabling pullcontrol
(4)PULSEL =0forpull-down functionality, 1forpull-up functionalityTable 22-5. Output Buffer andPullControl Behavior forADxEVT asGPIO Pins
System Reset
Active?PinDirection
(DIR)(1)(2)PullDisable
(PDIS)(1)(3)PullSelect
(PSEL)(1)(4)PullControl Output Buffer
Yes X X X Enabled Disabled
No 0 0 0 Pulldown Disabled
No 0 0 1 Pullup Disabled
No 0 1 0 Disabled Disabled
No 0 1 1 Disabled Disabled
No 1 X X Disabled Enabled

<!-- Page 881 -->

www.ti.com ADC Registers
881 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3 ADC Registers
Allregisters intheADC module are32-bit, word-aligned; 8-bit, 16-bit and32-bit accesses areallowed.
The application must ensure thatthereserved bitsarealways written as0toensure software compatibility
tofuture revisions ofthemodule. Table 22-6 shows register address offsets from thebase address ofthe
ADC modules. The base address ofADC1 registers isFFF7 C000h andthebase address ofADC2
registers isFFF7 C200h.
Table 22-6. ADC Registers
Offset Acronym Register Description Section
00h ADRSTCR ADC Reset Control Register Section 22.3.1
04h ADOPMODECR ADC Operating Mode Control Register Section 22.3.2
08h ADCLOCKCR ADC Clock Control Register Section 22.3.3
0Ch ADCALCR ADC Calibration Mode Control Register Section 22.3.4
10h ADEVMODECR ADC Event Group Operating Mode Control Register Section 22.3.5
14h ADG1MODECR ADC Group1 Operating Mode Control Register Section 22.3.6
18h ADG2MODECR ADC Group2 Operating Mode Control Register Section 22.3.7
1Ch ADEVSRC ADC Trigger Source Select Register Section 22.3.8
20h ADG1SRC ADC Group1 Trigger Source Select Register Section 22.3.9
24h ADG2SRC ADC Group2 Trigger Source Select Register Section 22.3.10
28h ADEVINTENA ADC Event Interrupt Enable Control Register Section 22.3.11
2Ch ADG1INTENA ADC Group1 Interrupt Enable Control Register Section 22.3.12
30h ADG2INTENA ADC Group2 Interrupt Enable Control Register Section 22.3.13
34h ADEVINTFLG ADC Event Group Interrupt Flag Register Section 22.3.14
38h ADG1INTFLG ADC Group1 Interrupt Flag Register Section 22.3.15
3Ch ADG2INTFLG ADC Group2 Interrupt Flag Register Section 22.3.16
40h ADEVTHRINTCR ADC Event Group Threshold Interrupt Control Register Section 22.3.17
44h ADG1THRINTCR ADC Group1 Threshold Interrupt Control Register Section 22.3.18
48h ADG2THRINTCR ADC Group2 Threshold Interrupt Control Register Section 22.3.19
4Ch ADEVDMACR ADC Event Group DMA Control Register Section 22.3.20
50h ADG1DMACR ADC Group1 DMA Control Register Section 22.3.21
54h ADG2DMACR ADC Group2 DMA Control Register Section 22.3.22
58h ADBNDCR ADC Results Memory Configuration Register Section 22.3.23
5Ch ADBNDEND ADC Results Memory Size Configuration Register Section 22.3.24
60h ADEVSAMP ADC Event Group Sampling Time Configuration Register Section 22.3.25
64h ADG1SAMP ADC Group1 Sampling Time Configuration Register() Section 22.3.26
68h ADG2SAMP ADC Group2 Sampling Time Configuration Register Section 22.3.27
6Ch ADEVSR ADC Event Group Status Register Section 22.3.28
70h ADG1SR ADC Group1 Status Register Section 22.3.29
74h ADG2SR ADC Group2 Status Register Section 22.3.30
78h ADEVSEL ADC Event Group Channel Select Register Section 22.3.31
7Ch ADG1SEL ADC Group1 Channel Select Register Section 22.3.32
80h ADG2SEL ADC Group2 Channel Select Register Section 22.3.33
84h ADCALR ADC Calibration andError Offset Correction Register Section 22.3.34
88h ADSMSTATE ADC State Machine Status Register Section 22.3.35
8Ch ADLASTCONV ADC Channel Last Conversion Value Register Section 22.3.36
90h-AFh ADEVBUFFER ADC Event Group Results FIFO Register Section 22.3.37
B0h-CFh ADG1BUFFER ADC Group1 Results FIFO Register Section 22.3.38
D0h-EFh ADG2BUFFER ADC Group2 Results FIFO Register Section 22.3.39
F0h ADEVEMUBUFFER ADC Event Group Results Emulation FIFO Register Section 22.3.40
F4h ADG1EMUBUFFER ADC Group1 Results Emulation FIFO Register Section 22.3.41

<!-- Page 882 -->

ADC Registers www.ti.com
882 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-6. ADC Registers (continued)
Offset Acronym Register Description Section
F8h ADG2EMUBUFFER ADC Group2 Results Emulation FIFO Register Section 22.3.42
FCh ADEVTDIR ADC ADEVT PinDirection Control Register Section 22.3.43
100h ADEVTOUT ADC ADEVT PinOutput Value Control Register Section 22.3.44
104h ADEVTIN ADC ADEVT PinInput Value Register Section 22.3.45
108h ADEVTSET ADC ADEVT PinSetRegister Section 22.3.46
10Ch ADEVTCLR ADC ADEVT PinClear Register Section 22.3.47
110h ADEVTPDR ADC ADEVT PinOpen Drain Enable Register Section 22.3.48
114h ADEVTPDIS ADC ADEVT PinPullControl Disable Register Section 22.3.49
118h ADEVTPSEL ADC ADEVT PinPullControl Select Register Section 22.3.50
11Ch ADEVSAMPDISEN ADC Event Group Sample Cap Discharge Control Register Section 22.3.51
120h ADG1SAMPDISEN ADC Group1 Sample Cap Discharge Control Register Section 22.3.52
124h ADG2SAMPDISEN ADC Group2 Sample Cap Discharge Control Register Section 22.3.53
128h-138h ADMAGINTxCR ADC Magnitude Compare Interrupt Control Register Section 22.3.54
12Ch-13Ch ADMAGxMASK ADC Magnitude Compare Mask Register Section 22.3.55
158h ADMAGINTENASET ADC Magnitude Compare Interrupt Enable SetRegister Section 22.3.56
15Ch ADMAGINTENACLR ADC Magnitude Compare Interrupt Enable Clear Register Section 22.3.57
160h ADMAGINTFLG ADC Magnitude Compare Interrupt Flag Register Section 22.3.58
164h ADMAGINTOFF ADC Magnitude Compare Interrupt Offset Register Section 22.3.59
168h ADEVFIFORESETCR ADC Event Group FIFO Reset Control Register Section 22.3.60
16Ch ADG1FIFORESETCR ADC Group1 FIFO Reset Control Register Section 22.3.61
170h ADG2FIFORESETCR ADC Group2 FIFO Reset Control Register Section 22.3.62
174h ADEVRAMWRADDR ADC Event Group RAM Write Address Register Section 22.3.63
178h ADG1RAMWRADDR ADC Group1 RAM Write Address Register Section 22.3.64
17Ch ADG2RAMWRADDR ADC Group2 RAM Write Address Register Section 22.3.65
180h ADPARCR ADC Parity Control Register Section 22.3.66
184h ADPARADDR ADC Parity Error Address Register Section 22.3.67
188h ADPWRUPDLYCTRL ADC Power-Up Delay Control Register Section 22.3.68
190h ADEVCHNSELMODECTRL ADC Event Group Channel Selection Mode Control Register Section 22.3.69
194h ADG1CHNSELMODECTRL ADC Group1 Channel Selection Mode Control Register Section 22.3.70
198h ADG2CHNSELMODECTRL ADC Group2 Channel Selection Mode Control Register Section 22.3.71
19Ch ADEVCURRCOUNT ADC Event Group Current Count Register Section 22.3.72
1A0h ADEVMAXCOUNT ADC Event Group Max Count Register Section 22.3.73
1A4h ADG1CURRCOUNT ADC Group1 Current Count Register Section 22.3.74
1A8h ADG1MAXCOUNT ADC Group1 Max Count Register Section 22.3.75
1ACh ADG2CURRCOUNT ADC Group2 Current Count Register Section 22.3.76
1B0h ADG2MAXCOUNT ADC Group2 Max Count Register Section 22.3.77

<!-- Page 883 -->

www.ti.com ADC Registers
883 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.1 ADC Reset Control Register (ADRSTCR)
Figure 22-20 andTable 22-7 describe theADRSTCR register.
Figure 22-20. ADC Reset Control Register (ADRSTCR) [offset =00]
31 1 0
Reserved RESET
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 22-7. ADC Reset Control Register (ADRSTCR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 RESET This bitisused toreset theADC internal state machines andcontrol/status registers. This reset
state isheld until thisbitiscleared. Read inallmodes, write inprivileged mode.
0 Module isreleased from thereset state.
1 Allthemodule 'sinternal state machines andthecontrol/status registers arereset.
22.3.2 ADC Operating Mode Control Register (ADOPMODECR)
Figure 22-21 andTable 22-8 describe theADOPMODECR register.
Figure 22-21. ADC Operating Mode Control Register (ADOPMODECR) [offset =04]
31 30 25 24
10_12_BIT Reserved COS
R/W-0 R-0 R/W-0
23 21 20 17 16
Reserved CHN_TEST_EN RAM_TEST_
EN
R-0 R/W-Ah R/W-0
15 9 8
Reserved POWER
DOWN
R-0 R/W-0
7 5 4 3 1 0
Reserved IDLE_PWRDN Reserved ADC_EN
R-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-8. ADC Operating Mode Control Register (ADOPMODECR) Field Descriptions
Bit Field Value Description
31 10_12_BIT This bitcontrols theresolution oftheADC core. Italso affects thesize oftheconversion
results stored intheresults 'RAM.
Any operation mode read/write:
0 The ADC core anddigital logic areconfigured tobein10-bit resolution. This isthedefault
mode ofoperation.
1 The ADC core anddigital logic areconfigured tobein12-bit resolution.
30-25 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 884 -->

ADC Registers www.ti.com
884 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-8. ADC Operating Mode Control Register (ADOPMODECR) Field Descriptions (continued)
Bit Field Value Description
24 COS This bitaffects emulation operation only.Itdefines whether theADC core clock (ADCLK) is
immediately halted when theemulation system enters suspend mode orifitshould continue
operating normally.
Note :IfCOS =0when theADC module enters theemulation mode, then theaccuracy of
theconversion results canbeaffected depending onhow long themodule stays inthe
emulation mode.
Any operation mode read/write:
0 ADC module halts allongoing conversions immediately after emulation mode isentered.
1 ADC module continues allongoing conversions aspertheconfigurations ofthethree
conversion groups.
23-21 Reserved 0 Reads return 0.Writes have noeffect.
20-17 CHN_TEST_EN Enable theinput channels 'impedance measurement mode.
This mode isreserved forusebyTI.
Any operation mode read/write:
Ah Input impedance measurement mode isdisabled.
5h Input impedance measurement mode isenabled.
other values Input impedance measurement mode isdisabled.
16 RAM_TEST_EN Enable theADC Results 'RAM Test Mode.
Refer toSection 22.2.7.2 formore details.
Any operation mode read/write:
0 ADC RAM Test Mode isdisabled. The application cannot write totheADC RAM bytheCPU
ortheDMA.
1 ADC RAM Test Mode isenabled. The application candirectly write totheADC RAM bythe
CPU ortheDMA.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8 POWERDOWN ADC Power Down. This bitpowers down only theADC core; thedigital logic inthe
sequencer stays active. Torelease thecore from power down mode, thisbitmust be
cleared. Ifaconversion isongoing, theADC module willwait until thecurrent conversion is
completed before powering down theADC core.
Also refer toSection 22.3.68 ,ADC Power-Up Delay Control Register
(ADPWRUPDLYCTRL).
Any operation mode read/write:
0 The state oftheADC core iscontrolled bytheIDLE_PWRDN bit,orbyaglobal power down
mode entry.
1 ADC core isinthepower-down state.
7-5 Reserved 0 Reads return 0.Writes have noeffect.
4 IDLE_PWRDN ADC Power Down When Idle. When thisbitisset,theADC module willautomatically power
down theADC core whenever there arenoconversions ongoing orpending. This isthe
enhanced power down mode.
Also refer toSection 22.3.68 ,ADC Power-Up Delay Control Register
(ADPWRUPDLYCTRL).
Any operation mode read/write:
0 The ADC stays inthenormal operating mode even ifnoconversions areongoing or
pending. The power down state isentered only byconfiguring thePOWER DOWN bitorvia
aglobal power down mode entry.
1 Enhanced power down mode isenabled.
3-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADC_EN ADC Enable. This bitmust besettoallow theADC module tobeconfigured toperform any
conversions.
Any operation mode read/write:
0 NoADC conversions canoccur. The input channel select registers: ADEVSEL, ADG1SEL,
andADG2SEL areheld attheir reset values.
1 ADC conversions cannow proceed asconfigured.

<!-- Page 885 -->

www.ti.com ADC Registers
885 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.3 ADC Clock Control Register (ADCLOCKCR)
Figure 22-22 andTable 22-9 describe theADCLOCKCR register.
Figure 22-22. ADC Clock Control Register (ADCLOCKCR) [offset =08h]
31 5 4 0
Reserved PS
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-9. ADC Clock Control Register (ADCLOCKCR) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 PS 0-1Fh ADC Clock Prescaler. These bitsdefine theprescaler value fortheADC core clock (ADCLK). The
ADCLK isgenerated bydividing down theinput busclock (VCLK) totheADC module.
Note: The supported range fortheADC clock frequency isspecified inthedevice datasheet. The
ADC clock prescaler must beconfigured tomeet thisdatasheet specification.
Any operation mode read/write:
tC(ADCLK) =tC(VCLK) ×(PS[4:0] +1),
where tC(ADCLK) istheperiod oftheADCLK andtC(VCLK) istheperiod oftheVCLK.
22.3.4 ADC Calibration Mode Control Register (ADCALCR)
Figure 22-23 andTable 22-10 describe theADCALCR register.
Figure 22-23. ADC Calibration Mode Control Register (ADCALCR) [offset =0Ch]
31 25 24
Reserved SELF_TEST
R-0 R/W-0
23 17 16
Reserved CAL_ST
R-0 R/S-0
15 10 9 8
Reserved BRIDGE_EN HILO
R-0 R/W-0 R/W-0
7 1 0
Reserved CAL_EN
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; S=Set; -n=value after reset

<!-- Page 886 -->

ADC Registers www.ti.com
886 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-10. ADC Calibration Mode Control Register (ADCALCR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 SELF_TEST ADC Self Test Enable. When thisbitisSet, either ADREFHI orADREFLO isconnected through a
resistor totheselected input channel. The desired conversion mode isconfigured inthegroup
mode control registers. Formore details ontheADC Self Test Mode, refer toSection 22.2.6.2 .
Any operation mode read/write:
0 ADC Self Test mode isdisabled.
1 ADC Self Test mode isenabled.
23-17 Reserved 0 Reads return 0.Writes have noeffect.
16 CAL_ST ADC Calibration Conversion Start. Setting theCAL_ST bitwhile theCAL_EN bitissetstarts
conversion oftheselected reference voltage. The ADC module uses thesample time configured in
theEvent Group sample time configuration register (ADEVSAMP) forthecalibration conversion.
Any operation mode:
0 Read: Calibration conversion hascompleted, orhasnotyetbeen started.
Write: Noeffect.
1 Read: Calibration conversion isinprogress.
Write: ADC module starts calibration conversion.
15-10 Reserved 0 Reads return 0.Writes have noeffect.
9 BRIDGE_EN Bridge Enable. When setwith theHILO bit,BRIDGE_EN allows areference voltage tobe
converted incalibration mode. Table 22-2 defines thefour different reference voltages thatcanbe
selected.
8 HILO ADC Self Test mode andCalibration Mode Reference Source Selection.
IntheADC Self Test mode, thisbitdefines thetestvoltage tobecombined through aresistor with
theselected input pinvoltage. Refer toSection 22.2.6.2 fordetails ontheADC Self Test Mode.
IntheADC Calibration Mode, thisbitdefines thereference source polarity. Refer to
Section 22.2.6.1 fordetails ontheADC Calibration Mode.
IntheADC module 'snormal operating mode, thisbithasnoeffect.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 CAL_EN ADC Calibration Enable. When thisbitisset,theinput channel multiplexor isdisconnected andthe
calibration reference voltage isconnected totheADC core input. The calibration reference voltage
isselected bythecombination oftheBRIDGE_EN andHILO. The actual conversion ofthis
reference voltage starts when theCAL_ST bitisset.IftheCAL_ST bitisalready setwhen the
CAL_EN bitisset,then thecalibration conversion isimmediately started.
Refer toSection 22.2.6.1 formore details ontheADC calibration mode.
Any operation mode read/write:
0 Calibration mode isdisabled.
1 Calibration mode isenabled.

<!-- Page 887 -->

www.ti.com ADC Registers
887 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.5 ADC Event Group Operating Mode Control Register (ADEVMODECR)
ADC Event Group Operating Mode Control Register (ADEVMODECR) isshown inFigure 22-24 and
Figure 22-25 ,anddescribed inTable 22-11 .Asshown, theformat oftheADEVMODECR isdifferent
based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-24. 12-bit ADC Event Group Operating Mode Control Register (ADEVMODECR)
[offset =10h]
31 24
Reserved
R-0
23 17 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 10 9 8
Reserved EV_DATA_FMT
R-0 R/W-0
7 6 5 4 3 2 1 0
Reserved EV_CHID OVR_EV_
RAM_IGNReserved EV_MODE FRZ_EV
R-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-25. 10-bit ADC Event Group Operating Mode Control Register (ADEVMODECR)
[offset =10h]
31 24
Reserved
R-0
23 17 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved EV_CHID OVR_EV_
RAM_IGNReserved EV_8BIT EV_MODE FRZ_EV
R-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset

<!-- Page 888 -->

ADC Registers www.ti.com
888 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-11. ADC Event Group Operating Mode Control Register (ADEVMODECR)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
NoReset onChnSel NoEvent Group Results Memory Reset onNew Channel Select.
This bitdetermines whether theevent group results 'RAM isreset whenever anon-zero value iswritten
totheevent group channel select register.
Any operation mode read/write:
0 Event group results RAM isreset when anon-zero value iswritten toevent group channel select
register, even ifevent group conversions arecompleted.
1 Event group results RAM isnotreset when anon-zero value iswritten toevent group channel select
register, andevent group conversions arecompleted.
Iftheevent group conversions areongoing (active orfrozen), then writing anon-zero value tothe
event group channel select register willalways reset theevent group results RAM.
EV_DATA_FMT Event Group Read Data Format.
This field isonly applicable when theADC module isconfigured tobeinthe12-bit ADC module. This
field isreserved when themodule isconfigured asa10-bit ADC module.
This field determines theformat inwhich theconversion results areread outoftheEvent group results
RAM when using theFIFO interface, thatis,when reading from theADEVBUFFER or
ADEVEMUBUFFER locations.
Any operation mode read/write:
0 Conversion results areread outinfull12-bit format. This isthedefault mode.
1h Conversion results areread outin10-bit format. Bits11-2 ofthe12-bit conversion result arereturned
asthe10-bit conversion result.
2h Conversion results areread outin8-bit format. Bits11-4 ofthe12-bit conversion result arereturned as
the8-bit conversion result.
3h Reserved. The full12-bit conversion result isreturned ifprogrammed.
EV_CHID Enable Channel IdfortheEvent Group conversion results toberead. This bitonly affects the"read
from FIFO "mode. The ADC always stores thechannel idintheresults RAM. Any 16-bit read
performed inthe"read from RAM "mode willreturn the5-bit channel idalong with the10-bit conversion
result.
Any operation mode read/write:
0 Bits14-10, thechannel idfield, ofthedata read from theEvent Group results 'FIFO isread as00000b.
1 Bits14-10, thechannel idfield, ofthedata read from theEvent Group results 'FIFO contains the
number oftheADC analog input towhich theconversion result belongs.
OVR_EV_RAM_IGN This bitallows theADC module tooverwrite thecontents oftheEvent Group results memory under an
overrun condition.
Any operation mode read/write:
0 The ADC cannot overwrite thecontents oftheEvent Group results memory. When anoverrun ofthis
memory occurs, thesoftware needs toread outallthecontents ofthismemory before theADC isable
towrite anew conversion result fortheEvent Group.
1 When anoverrun oftheEvent Group results memory occurs, theADC proceeds tooverwrite the
contents with anynew conversion results fortheEvent Group, starting with thefirstlocation inthis
memory.
EV_8BIT Event Group 8-bit result mode.
This bitisonly applicable when theADC module isconfigured tobea10-bit ADC module. This field is
reserved when themodule isconfigured asa12-bit ADC module.
This bitallows theEvent Group conversion results toberead outinan8-bit format. This bitonly
applies tothe"read from FIFO "mode. The lower 2bitsofthe10-bit conversion result arediscarded
andtheupper 8bitsareshifted right twoplaces toform the8-bit conversion result.
Any operation mode read/write:
0 The Event Group conversion result isread outasa10-bit value inthe"read from Event Group FIFO "
mode.
1 The Event Group conversion result isread outasan8-bit value inthe"read from Event Group FIFO "
mode.

<!-- Page 889 -->

www.ti.com ADC Registers
889 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-11. ADC Event Group Operating Mode Control Register (ADEVMODECR)
Field Descriptions (continued)
Field Value Description
EV_MODE Event Group Conversion Mode. This bitdefines whether theinput channels selected forconversion in
theEvent Group areconverted only once pertrigger, orarecontinuously converted.
Any operation mode read/write:
0 The channels selected forconversion intheEvent Group areconverted only once when theselected
event trigger condition occurs.
1 The channels selected forconversion intheEvent Group areconverted continuously when the
selected event trigger condition occurs.
FRZ_EV Event Group Freeze Enable. This bitallows anEvent Group conversion sequence tobefrozen ifa
Group1 oraGroup2 conversion isrequested. The Event Group conversion iskept frozen while the
Group1 orGroup2 conversion isactive, andcontinues from where itwas frozen once theGroup1 or
Group2 conversions arecompleted.
While theEvent Group conversion isfrozen, theEV_STOP status flagintheADEVSR register
indicates thattheEvent Group conversions have stopped. This bitgets cleared when theEvent Group
conversions resume.
Any operation mode read/write:
0 Event Group conversions cannot befrozen. Allthechannels selected forconversion intheEvent
Group areconverted before theADC canswitch over toservicing anyother conversion group.
1 Event Group conversions arefrozen whenever there isarequest forconversion from Group1 or
Group2.

<!-- Page 890 -->

ADC Registers www.ti.com
890 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.6 ADC Group1 Operating Mode Control Register (ADG1MODECR)
ADC Group1 Operating Mode Control Register (ADG1MODECR) isshown inFigure 22-26 andFigure 22-
27,anddescribed inTable 22-12 .Asshown, theformat oftheADG1MODECR isdifferent based on
whether theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-26. 12-bit ADC Group1 Operating Mode Control Register (ADG1MODECR)
[offset =14h]
31 24
Reserved
R-0
23 17 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 10 9 8
Reserved G1_DATA_FMT
R-0 R/W-0
7 6 5 4 3 2 1 0
Reserved G1_CHID OVR_G1_
RAM_IGNG1_HW_TRIG Reserved G1_MODE FRZ_G1
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-27. 10-bit ADC Group1 Operating Mode Control Register (ADG1MODECR)
[offset =14h]
31 24
Reserved
R-0
23 17 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved G1_CHID OVR_G1_
RAM_IGNG1_HW_TRIG G1_8BIT G1_MODE FRZ_G1
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset

<!-- Page 891 -->

www.ti.com ADC Registers
891 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-12. ADC Group1 Operating Mode Control Register (ADG1MODECR)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
NoReset onChnSel NoGroup1 Results Memory Reset onNew Channel Select.
This bitdetermines whether thegroup1 results 'RAM isreset whenever anon-zero value iswritten to
thegroup1 channel select register.
Any operation mode read/write:
0 Group1 results RAM isreset when anon-zero value iswritten togroup1 channel select register, even if
group1 conversions arecompleted.
1 Group1 results RAM isnotreset when anon-zero value iswritten togroup1 channel select register,
andgroup1 conversions arecompleted.
Ifthegroup1 conversions areongoing (active orfrozen), then writing anon-zero value tothegroup1
channel select register willalways reset thegroup1 results RAM.
G1_DATA_FMT Group1 Read Data Format.
This field isonly applicable when theADC module isconfigured tobeinthe12-bit ADC module. This
field isreserved when themodule isconfigured asa10-bit ADC module.
This field determines theformat inwhich theconversion results areread outofthegroup1 results RAM
when using theFIFO interface, thatis,when reading from theADG1BUFFER orADG1EMUBUFFER
locations.
Any operation mode read/write:
0 Conversion results areread outinfull12-bit format. This isthedefault mode.
1h Conversion results areread outin10-bit format. Bits11-2 ofthe12-bit conversion result arereturned
asthe10-bit conversion result.
2h Conversion results areread outin8-bit format. Bits11-4 ofthe12-bit conversion result arereturned as
the8-bit conversion result.
3h Reserved. The full12-bit conversion result isreturned ifprogrammed.
G1_CHID Enable Channel IdfortheGroup1 conversion results toberead. This bitonly affects the"read from
FIFO "mode. The ADC always stores thechannel idintheresults RAM. Any 16-bit read performed in
the"read from RAM "mode willreturn the5-bit channel idalong with the10-bit conversion result.
Any operation mode read/write:
0 Bits14-10, thechannel idfield, ofthedata read from theGroup1 results 'FIFO isread as00000b.
1 Bits14-10, thechannel idfield, ofthedata read from theGroup1 results 'FIFO contains thenumber of
theADC analog input towhich theconversion result belongs.
OVR_G1_RAM_IGN This bitallows theADC module tooverwrite thecontents oftheGroup1 results memory under an
overrun condition.
Any operation mode read/write:
0 The ADC cannot overwrite thecontents oftheGroup1 results memory. When anoverrun ofthis
memory occurs, thesoftware needs toread outallthecontents ofthismemory before theADC isable
towrite anew conversion result fortheGroup1.
1 When anoverrun oftheGroup1 results memory occurs, theADC proceeds tooverwrite thecontents
with anynew conversion results fortheGroup1, starting with thefirstlocation inthismemory.
G1_HW_TRIG Group1 Hardware Triggered. This bitallows theGroup1 tobehardware triggered. The Group1 is
software triggered bydefault. Formore details onhow totrigger aconversion group, refer to
Section 22.2.1.6 .
Any operation mode read/write:
0 The Group1 issoftware-triggered. AGroup1 conversion starts whenever theGroup1 channel select
register (ADG1SEL) iswritten with anon-zero value.
1 The Group1 ishardware-triggered. AGroup1 conversion starts whenever theGroup1 channel select
register hasanon-zero value, andthespecified hardware trigger occurs. The hardware trigger forthe
Group1 isspecified intheGroup1 Trigger Source register (ADG1SRC).

<!-- Page 892 -->

ADC Registers www.ti.com
892 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-12. ADC Group1 Operating Mode Control Register (ADG1MODECR)
Field Descriptions (continued)
Field Value Description
G1_8BIT Group1 8-bit result mode.
This field isonly applicable when theADC module isconfigured tobeinthe10-bit ADC module. This
field isreserved when themodule isconfigured asa12-bit ADC module.
This bitallows theGroup1 conversion results toberead outinan8-bit format. This bitonly applies to
the"read from FIFO "mode. The lower 2bitsofthe10-bit conversion result arediscarded andthe
upper 8bitsareshifted right twoplaces toform the8-bit conversion result.
Any operation mode read/write:
0 The Group1 conversion result isread outasa10-bit value inthe"read from Group1 FIFO "mode.
1 The Group1 conversion result isread outasan8-bit value inthe"read from Group1 FIFO "mode.
G1_MODE Group1 Conversion Mode. This bitdefines whether theinput channels selected forconversion inthe
Group1 areconverted only once, orarecontinuously converted.
Any operation mode read/write:
0 The channels selected forconversion intheGroup1 areconverted only once.
1 The channels selected forconversion intheGroup1 areconverted continuously.
FRZ_G1 Group1 Freeze Enable. This bitallows aGroup1 conversion sequence tobefrozen ifanEvent Group
oraGroup2 conversion isrequested. The Group1 conversion iskept frozen while theEvent Group or
Group2 conversion isactive, andcontinues from where itwas frozen once theEvent Group orGroup2
conversions arecompleted.
While theGroup1 conversion isfrozen, theG1_STOP status flagintheADG1SR register indicates that
theGroup1 conversions have stopped. This bitgets cleared when theGroup1 conversions resume.
Any operation mode read/write:
0 Group1 conversions cannot befrozen. Allthechannels selected forconversion intheGroup1 are
converted before theADC canswitch over toservicing anyother conversion group.
1 Group1 conversions arefrozen whenever there isarequest forconversion from Event Group or
Group2.

<!-- Page 893 -->

www.ti.com ADC Registers
893 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.7 ADC Group2 Operating Mode Control Register (ADG2MODECR)
ADC Group2 Operating Mode Control Register (ADG2MODECR) isshown inFigure 22-28 andFigure 22-
29,described inTable 22-13 .Asshown, theformat oftheADG2MODECR isdifferent based onwhether
theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-28. 12-bit ADC Group2 Operating Mode Control Register (ADG2MODECR)
[offset =18h]
31 24
Reserved
R-0
23 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 10 9 8
Reserved G2_DATA_FMT
R-0 R/W-0
7 6 5 4 3 2 1 0
Reserved G2_CHID OVR_G2_
RAM_IGNG2_HW_TRIG Reserved G2_MODE FRZ_G2
R-0 R/W-0 R/W-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-29. 10-bit ADC Group2 Operating Mode Control Register (ADG2MODECR)
[offset =18h]
31 24
Reserved
R-0
23 16
ReservedNoReset on
ChnSel
R-0 R/W-0
15 8
Reserved
R-0
7 6 5 4 3 2 1 0
Reserved G2_CHID OVR_G2_
RAM_IGNG2_HW_TRIG G2_8BIT G2_MODE FRZ_G2
R-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset

<!-- Page 894 -->

ADC Registers www.ti.com
894 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-13. ADC Group 2Operating Mode Control Register (ADG2MODECR)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
NoReset onChnSel NoGroup2 Results Memory Reset onNew Channel Select.
This bitdetermines whether thegroup2 results 'RAM isreset whenever anon-zero value iswritten to
thegroup2 channel select register.
Any operation mode read/write:
0 Group2 results RAM isreset when anon-zero value iswritten togroup2 channel select register, even if
group2 conversions arecompleted.
1 Group2 results RAM isnotreset when anon-zero value iswritten togroup2 channel select register,
andgroup2 conversions arecompleted.
Ifthegroup2 conversions areongoing (active orfrozen), then writing anon-zero value tothegroup2
channel select register willalways reset thegroup2 results RAM.
G2_DATA_FMT Group2 Read Data Format.
This field isonly applicable when theADC module isconfigured tobeinthe12-bit ADC module. This
field isreserved when themodule isconfigured asa10-bit ADC module.
This field determines theformat inwhich theconversion results areread outofthegroup1 results RAM
when using theFIFO interface, thatis,when reading from theADG2BUFFER orADG2EMUBUFFER
locations.
Any operation mode read/write:
0 Conversion results areread outinfull12-bit format. This isthedefault mode.
1h Conversion results areread outin10-bit format. Bits11-2 ofthe12-bit conversion result arereturned
asthe10-bit conversion result.
2h Conversion results areread outin8-bit format. Bits11-4 ofthe12-bit conversion result arereturned as
the8-bit conversion result.
3h Reserved. The full12-bit conversion result isreturned ifprogrammed.
G2_CHID Enable Channel IdfortheGroup2 conversion results toberead. This bitonly affects the"read from
FIFO "mode. The ADC always stores thechannel idintheresults RAM. Any 16-bit read performed in
the"read from RAM "mode willreturn the5-bit channel idalong with the10-bit conversion result.
Any operation mode read/write:
0 Bits14-10, thechannel idfield, ofthedata read from theGroup2 results 'FIFO isread as00000b.
1 Bits14-10, thechannel idfield, ofthedata read from theGroup2 results 'FIFO contains thenumber of
theADC analog input towhich theconversion result belongs.
OVR_G2_RAM_IGN This bitallows theADC module tooverwrite thecontents oftheGroup2 results memory under an
overrun condition.
Any operation mode read/write:
0 The ADC cannot overwrite thecontents oftheGroup2 results memory. When anoverrun ofthis
memory occurs, thesoftware needs toread outallthecontents ofthismemory before theADC isable
towrite anew conversion result fortheGroup2.
1 When anoverrun oftheGroup2 results memory occurs, theADC proceeds tooverwrite thecontents
with anynew conversion results fortheGroup2, starting with thefirstlocation inthismemory.
G2_HW_TRIG Group2 Hardware Triggered. This bitallows theGroup2 tobehardware triggered. The Group2 is
software triggered bydefault. Formore details onhow totrigger aconversion group, refer to
Section 22.2.1.6 .
Any operation mode read/write:
0 The Group2 issoftware-triggered. AGroup2 conversion starts whenever theGroup2 channel select
register (ADG2SEL) iswritten with anon-zero value.
1 The Group2 ishardware-triggered. AGroup2 conversion starts whenever theGroup2 channel select
register hasanon-zero value, andthespecified hardware trigger occurs. The hardware trigger forthe
Group2 isspecified intheGroup2 Trigger Source register (ADG2SRC).

<!-- Page 895 -->

www.ti.com ADC Registers
895 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-13. ADC Group 2Operating Mode Control Register (ADG2MODECR)
Field Descriptions (continued)
Field Value Description
G2_8BIT Group2 8-bit result mode.
This field isonly applicable when theADC module isconfigured tobeinthe10-bit ADC module. This
field isreserved when themodule isconfigured asa12-bit ADC module.
This bitallows theGroup2 conversion results toberead outinan8-bit format. This bitonly applies to
the"read from FIFO "mode. The lower 2bitsofthe10-bit conversion result arediscarded andthe
upper 8bitsareshifted right twoplaces toform the8-bit conversion result.
Any operation mode read/write:
0 The Group2 conversion result isread outasa10-bit value inthe"read from Group2 FIFO "mode.
1 The Group2 conversion result isread outasan8-bit value inthe"read from Group2 FIFO "mode.
G2_MODE Group2 Conversion Mode. This bitdefines whether theinput channels selected forconversion inthe
Group2 areconverted only once, orarecontinuously converted.
Any operation mode read/write:
0 The channels selected forconversion intheGroup2 areconverted only once.
1 The channels selected forconversion intheGroup2 areconverted continuously.
FRZ_G2 Group2 Freeze Enable. This bitallows aGroup2 conversion sequence tobefrozen ifanEvent Group
oraGroup1 conversion isrequested. The Group2 conversion iskept frozen while theEvent Group or
Group1 conversion isactive, andcontinues from where itwas frozen once theEvent Group orGroup1
conversions arecompleted.
While theGroup2 conversion isfrozen, theG2_STOP status flagintheADG2SR register indicates that
theGroup2 conversions have stopped. This bitgets cleared when theGroup2 conversions resume.
Any operation mode read/write:
0 Group2 conversions cannot befrozen. Allthechannels selected forconversion intheGroup2 are
converted before theADC canswitch over toservicing anyother conversion group.
1 Group2 conversions arefrozen whenever there isarequest forconversion from Event Group or
Group1.

<!-- Page 896 -->

ADC Registers www.ti.com
896 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.8 ADC Event Group Trigger Source Select Register (ADEVSRC)
ADC Event Group Trigger Source Select Register (ADEVSRC) isshown inFigure 22-30 anddescribed in
Table 22-14 .
Figure 22-30. ADC Event Group Trigger Source Select Register (ADEVSRC) [offset =1Ch]
31 8
Reserved
R-0
7 5 4 3 2 0
Reserved EV_EDG_BOTH EV_EDG_SEL EV_SRC
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-14. ADC Event Group Trigger Source Select Register (ADEVSRC) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 EV_EDG_BOTH EVGroup Trigger Edge Polarity Select. This bitconfigures theevent group tobetriggered onboth
rising andfalling edge detected ontheselected trigger source.
Any operation mode read/write:
0 The conversion istriggered only upon detecting anedge defined bytheEV_EDG_SEL bit.
1 The conversion istriggered upon detecting either arising orfalling edge.
3 EV_EDG_SEL Event Group Trigger Edge Polarity Select. This bitdetermines thepolarity ofthetransition onthe
selected source thattriggers theEvent Group conversion.
Any operation mode read/write:
0 Ahigh-to-low transition ontheselected source willtrigger theEvent Group conversion.
1 Alow-to-high transition ontheselected source willtrigger theEvent Group conversion.
2-0 EV_SRC Event Group Trigger Source.
Any operation mode read/write:
0-7h The ADC module allows atrigger source tobeselected fortheEvent Group from uptoeight
options. These options aredevice-specific andthedevice specification must bereferred toidentify
theactual trigger sources.

<!-- Page 897 -->

www.ti.com ADC Registers
897 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.9 ADC Group1 Trigger Source Select Register (ADG1SRC)
ADC Group1 Trigger Source Select Register (ADG1SRC) isshown inFigure 22-31 anddescribed in
Table 22-15 .
Figure 22-31. ADC Group1 Trigger Source Select Register (ADG1SRC) [offset =20h]
31 8
Reserved
R-0
7 5 4 3 2 0
Reserved G1_EDG_BOTH G1_EDG_SEL G1_SRC
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-15. ADC Group1 Trigger Source Select Register (ADG1SRC) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 GI_EDG_BOTH Group1 Trigger Edge Polarity Select. This bitconfigures thegroup1 tobetriggered onboth rising
andfalling edge detected ontheselected trigger source.
Any operation mode read/write:
0 The conversion istriggered only upon detecting anedge defined bytheG1_EDG_SEL bit.
1 The conversion istriggered upon detecting either arising orfalling edge.
3 G1_EDG_SEL Group1 Trigger Edge Polarity Select. This bitdetermines thepolarity ofthetransition onthe
selected source thattriggers theGroup1 conversion.
Any operation mode read/write:
0 Ahigh-to-low transition ontheselected source willtrigger theGroup1 conversion.
1 Alow-to-high transition ontheselected source willtrigger theGroup1 conversion.
2-0 G1_SRC Group1 Trigger Source.
Any operation mode read/write:
0-7h The ADC module allows atrigger source tobeselected fortheGroup1 from uptoeight options.
These options aredevice-specific andthedevice specification must bereferred toidentify the
actual trigger sources.

<!-- Page 898 -->

ADC Registers www.ti.com
898 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.10 ADC Group2 Trigger Source Select Register (ADG2SRC)
ADC Group2 Trigger Source Select Register (ADG2SRC) isshown inFigure 22-32 anddescribed in
Table 22-16 .
Figure 22-32. ADC Group2 Trigger Source Select Register (ADG2SRC) [offset =24h]
31 8
Reserved
R-0
7 5 4 3 2 0
Reserved G2_EDG_BOTH G2_EDG_SEL G2_SRC
R-0 R/W-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-16. ADC Group2 Trigger Source Select Register (ADG2SRC) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4 G2_EDG_BOTH Group2 Trigger Edge Polarity Select. This bitconfigures thegroup2 tobetriggered onboth rising
andfalling edge detected ontheselected trigger source.
Any operation mode read/write:
0 The conversion istriggered only upon detecting anedge defined bytheG2_EDG_SEL bit.
1 The conversion istriggered upon detecting either arising orfalling edge.
3 G2_EDG_SEL Group2 Trigger Edge Polarity Select. This bitdetermines thepolarity ofthetransition onthe
selected source thattriggers theGroup2 conversion.
Any operation mode read/write:
0 Ahigh-to-low transition ontheselected source willtrigger theGroup2 conversion.
1 Alow-to-high transition ontheselected source willtrigger theGroup2 conversion.
2-0 G2_SRC Group2 Trigger Source.
Any operation mode read/write:
0-7h The ADC module allows atrigger source tobeselected fortheGroup2 from uptoeight options.
These options aredevice-specific andthedevice specification must bereferred toidentify the
actual trigger sources.

<!-- Page 899 -->

www.ti.com ADC Registers
899 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.11 ADC Event Interrupt Enable Control Register (ADEVINTENA)
ADC Event Group Interrupt Enable Control Register (ADEVINTENA) isshown inFigure 22-33 and
described inTable 22-17 .
Figure 22-33. ADC Event Group Interrupt Enable Control Register (ADEVINTENA) [offset =28h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved EV_END_
INT_ENReserved EV_OVR_
INT_ENEV_THR_
INT_EN
R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-17. ADC Event Group Interrupt Enable Control Register (ADEVINTENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 EV_END_INT_EN Event Group Conversion End Interrupt Enable. Refer toSection 22.2.3.1 formore details onthe
conversion endinterrupts.
Any operation mode read/write:
0 Nointerrupt isgenerated when conversion ofallthechannels selected forconversion inthe
Event Group isdone.
1 AnEvent Group conversion endinterrupt isgenerated when conversion ofallthechannels
selected forconversion intheEvent Group isdone.
2 Reserved 0 Reads return 0.Writes have noeffect.
1 EV_OVR_INT_EN Event Group Memory Overrun Interrupt Enable. Amemory overrun occurs when theADC tries
towrite anew conversion result totheEvent Group results memory which isalready full.For
more details ontheoverrun interrupts, refer toSection 22.2.3.3 .
Any operation mode read/write:
0 Nointerrupt isgenerated ifanEvent Group memory overrun occurs.
1 AnEvent Group memory overrun interrupt isgenerated ifanEvent Group memory overrun
condition occurs.
0 EV_THR_INT_EN Event Group Threshold Interrupt Enable. AnEvent Group threshold interrupt occurs when the
programmed Event Group threshold counter counts down to0.Refer toSection 22.2.3.2 for
more details.
Any operation mode read/write:
0 Nointerrupt isgenerated iftheEvent Group threshold counter reaches 0.
1 AnEvent Group threshold interrupt isgenerated iftheEvent Group threshold counter reaches
0.

<!-- Page 900 -->

ADC Registers www.ti.com
900 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.12 ADC Group1 Interrupt Enable Control Register (ADG1INTENA)
ADC Group1 Interrupt Enable Control Register (ADG1INTENA) isshown inFigure 22-34 anddescribed in
Table 22-18 .
Figure 22-34. ADC Group1 Interrupt Enable Control Register (ADG1INTENA) [offset =2Ch]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G1_END_
INT_ENReserved G1_OVR_
INT_ENG1_THR_
INT_EN
R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-18. ADC Group1 Interrupt Enable Control Register (ADG1INTENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G1_END_INT_EN Group1 Conversion End Interrupt Enable. Refer toSection 22.2.3.1 formore details onthe
conversion endinterrupts.
Any operation mode read/write:
0 Nointerrupt isgenerated when conversion ofallthechannels selected forconversion inthe
Group1 isdone.
1 AGroup1 conversion endinterrupt isgenerated when conversion ofallthechannels selected
forconversion intheGroup1 isdone.
2 Reserved 0 Reads return 0.Writes have noeffect.
1 G1_OVR_INT_EN Group1 Memory Overrun Interrupt Enable. Amemory overrun occurs when theADC tries to
write anew conversion result totheGroup1 results memory which isalready full.Formore
details ontheoverrun interrupts Refer toSection 22.2.3.3 .
Any operation mode read/write:
0 Nointerrupt isgenerated ifaGroup1 memory overrun occurs.
1 AGroup1 memory overrun interrupt isgenerated ifaGroup1 memory overrun condition occurs.
0 G1_THR_INT_EN Group1 Threshold Interrupt Enable. AGroup1 threshold interrupt occurs when theprogrammed
Group1 threshold counter counts down to0.Refer toSection 22.2.3.2 formore details.
Any operation mode read/write:
0 Nointerrupt isgenerated iftheGroup1 threshold counter reaches 0.
1 AGroup1 threshold interrupt isgenerated iftheGroup1 threshold counter reaches 0.

<!-- Page 901 -->

www.ti.com ADC Registers
901 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.13 ADC Group2 Interrupt Enable Control Register (ADG2INTENA)
ADC Group2 Interrupt Enable Control Register (ADG2INTENA) isshown inFigure 22-35 anddescribed in
Table 22-19 .
Figure 22-35. ADC Group2 Interrupt Enable Control Register (ADG2INTENA) [offset =30h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G2_END_
INT_ENReserved G2_OVR_
INT_ENG2_THR_
INT_EN
R-0 R/W-0 R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-19. ADC Group2 Interrupt Enable Control Register (ADG2INTENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G2_END_INT_EN Group2 Conversion End Interrupt Enable. Refer toSection 22.2.3.1 formore details onthe
conversion endinterrupts.
Any operation mode read/write:
0 Nointerrupt isgenerated when conversion ofallthechannels selected forconversion inthe
Group2 isdone.
1 AGroup2 conversion endinterrupt isgenerated when conversion ofallthechannels selected
forconversion intheGroup2 isdone.
2 Reserved 0 Reads return 0.Writes have noeffect.
1 G2_OVR_INT_EN Group2 Memory Overrun Interrupt Enable. Amemory overrun occurs when theADC tries to
write anew conversion result totheGroup2 results memory which isalready full.Formore
details ontheoverrun interrupts, refer toSection 22.2.3.3 .
Any operation mode read/write:
0 Nointerrupt isgenerated ifaGroup2 memory overrun occurs.
1 AGroup2 memory overrun interrupt isgenerated ifaGroup2 memory overrun condition occurs.
0 G2_THR_INT_EN Group2 Threshold Interrupt Enable. AGroup2 threshold interrupt occurs when theprogrammed
Group2 threshold counter counts down to0.Refer toSection 22.2.3.2 formore details.
Any operation mode read/write:
0 Nointerrupt isgenerated iftheGroup2 threshold counter reaches 0.
1 AGroup2 threshold interrupt isgenerated iftheGroup2 threshold counter reaches 0.

<!-- Page 902 -->

ADC Registers www.ti.com
902 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.14 ADC Event Group Interrupt Flag Register (ADEVINTFLG)
ADC Event Group Interrupt Enable Control Register (ADEVINTENA) isshown inFigure 22-36 and
described inTable 22-20 .
Figure 22-36. ADC Event Group Interrupt Flag Register (ADEVINTFLG) [offset =34h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved EV_END EV_MEM_
EMPTYEV_MEM_
OVERRUNEV_THR_
INT_FLG
R-0 R/W1C-0 R-1 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-20. ADC Event Group Interrupt Flag Register (ADEVINTFLG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 EV_END Event Group Conversion End. This bitwillbesetonly iftheEvent Group conversions are
configured tobeinthesingle-conversion mode.
Any operation mode read:
0 Allthechannels selected forconversion intheEvent Group have notyetbeen converted.
1 Allthechannels selected forconversion intheEvent Group have been converted. AnEvent
Group conversion endinterrupt isgenerated, ifenabled, when thisbitgets set.
This bitcanbecleared byanyoneofthefollowing ways:
*Bywriting a1tothisbit
*Bywriting a1totheEvent Group status register (ADEVSR) bit0(EV_END)
*Byreading oneconversion result from theEvent Group results 'memory inthe"read from
FIFO "mode
*Bywriting anew setofchannels totheEvent Group channel select register
2 EV_MEM_EMPTY Event Group Results Memory Empty. This isaread-only bit;writes have noeffect. Itisnota
source ofaninterrupt from theADC module.
Any operation mode read:
0 The Event Group results memory isnotempty.
1 The Event Group results memory isempty.
1 EV_MEM_OVERRUN Event Group Memory Overrun. This isaread-only bit;writes have noeffect.
Any operation mode read:
0 Event Group results memory hasnotoverrun.
1 Event Group results memory hasoverrun.
0 EV_THR_INT_FLG Event Group Threshold Interrupt Flag.
Any operation mode read:
0 The number ofconversions completed fortheEvent Group issmaller than thethreshold
programmed intheEvent Group interrupt threshold register.
1 The number ofconversions completed fortheEvent Group isequal toorgreater than the
threshold programmed intheEvent Group interrupt threshold register.
This bitcanbecleared bywriting a1;writing a0hasnoeffect.

<!-- Page 903 -->

www.ti.com ADC Registers
903 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.15 ADC Group1 Interrupt Flag Register (ADG1INTFLG)
ADC Group1 Interrupt Flag Register (ADG1INTFLG) isshown inFigure 22-37 anddescribed inTable 22-
21.
Figure 22-37. ADC Group1 Interrupt Flag Register (ADG1INTFLG) [offset =38h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G1_END G1_MEM_
EMPTYG1_MEM_
OVERRUNG1_THR_
INT_FLG
R-0 R/W1C-0 R-1 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-21. ADC Group1 Interrupt Flag Register (ADG1INTFLG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G1_END Group1 Conversion End. This bitwillbesetonly iftheGroup1 conversions areconfigured tobe
inthesingle-conversion mode.
Any operation mode read:
0 Allthechannels selected forconversion intheGroup1 have notyetbeen converted.
1 Allthechannels selected forconversion intheGroup1 have been converted. AGroup1
conversion endinterrupt isgenerated, ifenabled, when thisbitgets set.
This bitcanbecleared byanyoneofthefollowing ways:
*Bywriting a1tothisbit
*Bywriting a1totheGroup1 status register (ADG1SR) bit0(G1_END)
*Byreading oneconversion result from theGroup1 results 'memory inthe"read from FIFO "
mode
*Bywriting anew setofchannels totheGroup1 channel select register
2 G1_MEM_EMPTY Group1 Results Memory Empty. This isaread-only bit;writes have noeffect. Itisnotasource
ofaninterrupt from theADC module.
Any operation mode read:
0 The Group1 results memory isnotempty.
1 The Group1 results memory isempty.
1 G1_MEM_OVERRUN Group1 Memory Overrun. This isaread-only bit;writes have noeffect.
Any operation mode read:
0 Group1 results memory hasnotoverrun.
1 Group1 results memory hasoverrun.
0 G1_THR_INT_FLG Group1 Threshold Interrupt Flag.
Any operation mode read:
0 The number ofconversions completed fortheGroup1 issmaller than thethreshold
programmed intheGroup1 interrupt threshold register.
1 The number ofconversions completed fortheGroup1 isequal toorgreater than thethreshold
programmed intheGroup1 interrupt threshold register.
This bitcanbecleared bywriting a1;writing a0hasnoeffect.

<!-- Page 904 -->

ADC Registers www.ti.com
904 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.16 ADC Group2 Interrupt Flag Register (ADG2INTFLG)
ADC Group2 Interrupt Flag Register (ADG2INTFLG) isshown inFigure 22-38 anddescribed inTable 22-
22.
Figure 22-38. ADC Group2 Interrupt Flag Register (ADG2INTFLG) [offset =3Ch]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G2_END G2_MEM_
EMPTYG2_MEM_
OVERRUNG2_THR_
INT_FLG
R-0 R/W1C-0 R-1 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-22. ADC Group2 Interrupt Flag Register (ADG2INTFLG) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G2_END Group2 Conversion End. This bitwillbesetonly iftheGroup2 conversions areconfigured tobe
inthesingle-conversion mode.
Any operation mode read:
0 Allthechannels selected forconversion intheGroup2 have notyetbeen converted.
1 Allthechannels selected forconversion intheGroup2 have been converted. AGroup2
conversion endinterrupt isgenerated, ifenabled, when thisbitgets set.
This bitcanbecleared byanyoneofthefollowing ways:
*Bywriting a1tothisbit
*Bywriting a1totheGroup2 status register (ADG2SR) bit0(G2_END)
*Byreading oneconversion result from theGroup2 results 'memory inthe"read from FIFO "
mode
*Bywriting anew setofchannels totheGroup2 channel select register
2 G2_MEM_EMPTY Group2 Results Memory Empty. This isaread-only bit;writes have noeffect. Itisnotasource
ofaninterrupt from theADC module.
Any operation mode read:
0 The Group2 results memory isnotempty.
1 The Group2 results memory isempty.
1 G2_MEM_OVERRUN Group2 Memory Overrun. This isaread-only bit;writes have noeffect.
Any operation mode read:
0 Group2 results memory hasnotoverrun.
1 Group2 results memory hasoverrun.
0 G2_THR_INT_FLG Group2 Threshold Interrupt Flag.
Any operation mode read:
0 The number ofconversions completed fortheGroup2 issmaller than thethreshold
programmed intheGroup2 interrupt threshold register.
1 The number ofconversions completed fortheGroup2 isequal toorgreater than thethreshold
programmed intheGroup2 interrupt threshold register.
This bitcanbecleared bywriting a1;writing a0hasnoeffect.

<!-- Page 905 -->

www.ti.com ADC Registers
905 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.17 ADC Event Group Threshold Interrupt Control Register (ADEVTHRINTCR)
ADC Event Group Threshold Interrupt Control Register (ADEVTHRINTCR) isshown inFigure 22-39 and
described inTable 22-23 .
Figure 22-39. ADC Event Group Threshold Interrupt Control Register (ADEVTHRINTCR)
[offset =40h]
31 16 15 9 8 0
Reserved Sign Extension EV_THR
R-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-23. ADC Event Group Threshold Interrupt Control Register (ADEVTHRINTCR)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-9 Sign Extension These bitsalways read thesame asEV_THR bit8ofthisregister.
8-0 EV_THR Event Group Threshold Counter.
Before ADC conversions begin ontheEvent Group, thisfield isinitialized tothenumber of
conversion results thattheEvent Group memory should contain before interrupting theCPU. This
counter decrements when theADC module writes anew conversion result totheEvent Group
results 'memory. The counter increments foreach read ofaconversion result from theEvent Group
results 'memory inthe"read from FIFO "mode. The threshold counter isnotaffected foradirect
read from theEvent Group results 'memory. Also, asimultaneous ADC write andaCPU/DMA read
from theEvent Group FIFO willleave thethreshold counter unchanged. Incase ofanEvent Group
Results 'memory overrun condition, ifnew conversion results arenotallowed tooverwrite the
existing memory contents, then theEvent Group threshold counter isnotdecremented.
Refer toSection 22.2.3.2 formore details onthethreshold interrupts.
22.3.18 ADC Group1 Threshold Interrupt Control Register (ADG1THRINTCR)
ADC Group1 Threshold Interrupt Control Register (ADG1THRINTCR) isshown inFigure 22-40 and
described inTable 22-24 .
Figure 22-40. ADC Group1 Threshold Interrupt Control Register (ADG1THRINTCR) [offset =44h]
31 16 15 9 8 0
Reserved Sign Extension G1_THR
R-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-24. ADC Group1 Threshold Interrupt Control Register (ADG1THRINTCR)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-9 Sign Extension These bitsalways read thesame asG1_THR bit8ofthisregister.
8-0 G1_THR Group1 Threshold Counter.
Before ADC conversions begin ontheGroup1, thisfield isinitialized tothenumber ofconversion
results thattheGroup1 memory should contain before interrupting theCPU. This counter
decrements when theADC module writes anew conversion result totheGroup1 results 'memory.
The counter increments foreach read ofaconversion result from theGroup1 results 'memory inthe
"read from FIFO "mode. The threshold counter isnotaffected foradirect read from thegroup1
results 'memory. Also, asimultaneous ADC write andaCPU/DMA read from theGroup1 FIFO will
leave thethreshold counter unchanged. Incase ofanGroup1 Results 'memory overrun condition, if
new conversion results arenotallowed tooverwrite theexisting memory contents, then theGroup1
threshold counter isnotdecremented.
Refer toSection 22.2.3.2 formore details onthethreshold interrupts.

<!-- Page 906 -->

ADC Registers www.ti.com
906 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.19 ADC Group2 Threshold Interrupt Control Register (ADG2THRINTCR)
The ADC Group2 Threshold Interrupt Control Register (ADG2THRINTCR) isshown inFigure 22-41 and
described inTable 22-25 .
Figure 22-41. ADC Group2 Threshold Interrupt Control Register (ADG2THRINTCR) [offset =48h]
31 16 15 9 8 0
Reserved Sign Extension G2_THR
R-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-25. ADC Group2 Threshold Interrupt Control Register (ADG2THRINTCR)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-9 Sign Extension These bitsalways read thesame asG2_THR bit8ofthisregister.
8-0 G2_THR Group2 Threshold Counter.
Before ADC conversions begin ontheGroup2, thisfield isinitialized tothenumber ofconversion
results thattheGroup2 memory should contain before interrupting theCPU. This counter
decrements when theADC module writes anew conversion result totheGroup2 results 'memory.
The counter increments foreach read ofaconversion result from theGroup2 results 'memory inthe
"read from FIFO "mode. The threshold counter isnotaffected foradirect read from thegroup2
results 'memory. Also, asimultaneous ADC write andaCPU/DMA read from theGroup2 FIFO will
leave thethreshold counter unchanged. Incase ofanGroup2 Results 'memory overrun condition, if
new conversion results arenotallowed tooverwrite theexisting memory contents, then theGrou21
threshold counter isnotdecremented.
Refer toSection 22.2.3.2 formore details onthethreshold interrupts.

<!-- Page 907 -->

www.ti.com ADC Registers
907 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.20 ADC Event Group DMA Control Register (ADEVDMACR)
ADC Event Group DMA Control Register (ADEVDMACR) isshown inFigure 22-42 anddescribed in
Table 22-26 .
Figure 22-42. ADC Event Group DMA Control Register (ADEVDMACR) [offset =4Ch]
31 25 24 16
Reserved EV_BLOCKS
R-0 R/W-0
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved DMA_EV_END EV_BLK_XFER Reserved EV_DMA_EN
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-26. ADC Event Group DMA Control Register (ADEVDMACR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24-16 EV_BLOCKS Number ofEvent Group Result buffers tobetransferred using DMA iftheADC module is
configured togenerate aDMA request. IftheEvent Group isconfigured tousetheblock
transfer mode oftheDMA module, then theADC module generates aDMA request after the
Event Group results 'memory accumulates EV_BLOCKS number ofconversion results.
This feature isdesigned tobeused inplace ofthethreshold interrupt fortheEvent Group. Asa
result, theEV_THR field oftheEvent Group Interrupt Threshold Control Register andthe
EV_BLOCKS field oftheEvent Group DMA Control Register arethesame.
Any operation mode read/write:
0 NoDMA transfer occurs even ifEV_BLK_XFER issetto1.
1h-1FFh One DMA request isgenerated iftheEV_BLK_XFER issetto1andthespecified number of
Event Group conversion results have been accumulated.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3 DMA_EV_END Event Group Conversion End DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write totheEvent group results RAM if
EV_DMA_EN isset.
1 ADC module generates aDMA request when theADC hascompleted theconversions forall
channels selected forconversion intheevent group.
IfDMA_EV_END bitissetto1,EV_DMA_EN bitisignored andDMA requests willbe
generated every time theDMA_EV_END flagintheevent group status register isset.The
DMA_EV_END bitmust besetbefore enabling conversions fortheevent group.
2 EV_BLK_XFER Event Group Block DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write totheEvent Group memory if
EV_DMA_EN isset.
1 ADC module generates aDMA request when theADC haswritten EV_BLOCKS number of
buffers intotheEvent Group memory.
IfEV_BLK_XFER bitissetto1,EV_DMA_EN bitisignored andDMA requests willbe
generated every time theThreshold Counter reaches 0from acount value of1.
1 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 908 -->

ADC Registers www.ti.com
908 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-26. ADC Event Group DMA Control Register (ADEVDMACR) Field Descriptions (continued)
Bit Field Value Description
0 EV_DMA_EN Event Group DMA Transfer Enable.
Any operation mode read:
0 ADC module does notgenerate aDMA request when itwrites theconversion result tothe
Event Group memory.
1 ADC module generates aDMA transfer when theADC haswritten totheEvent Group memory.
The EV_BLK_XFER bitmust becleared to0forthisDMA request tobegenerated.

<!-- Page 909 -->

www.ti.com ADC Registers
909 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.21 ADC Group1 DMA Control Register (ADG1DMACR)
ADC Group1 DMA Control Register (ADG1DMACR) isshown inFigure 22-43 anddescribed inTable 22-
27.
Figure 22-43. ADC Group1 DMA Control Register (ADG1DMACR) [offset =50h]
31 25 24 16
Reserved G1_BLOCKS
R-0 R/W-0
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved DMA_G1_END G1_BLK_XFER Reserved G1_DMA_EN
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-27. ADC Group1 DMA Control Register (ADG1DMACR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24-16 G1_BLOCKS Number ofGroup1 Result buffers tobetransferred using DMA iftheADC module isconfigured
togenerate aDMA request. IftheGroup1 isconfigured tousetheblock transfer mode ofthe
DMA module, then theADC module generates aDMA request after theGroup1 results '
memory accumulates G1_BLOCKS number ofconversion results.
This feature isdesigned tobeused inplace ofthethreshold interrupt fortheGroup1. Asa
result, theG1_THR field oftheGroup1 Interrupt Threshold Control Register andthe
G1_BLOCKS field oftheGroup1 DMA Control Register arethesame.
Any operation mode read/write:
0 NoDMA transfer occurs even ifG1_BLK_XFER issetto1.
1h-1FFh One DMA request isgenerated iftheG1_BLK_XFER issetto1andthespecified number of
Group1 conversion results have been accumulated.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3 DMA_G1_END Group1 Conversion End DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write tothegroup1 results RAM if
G1_DMA_EN isset.
1 ADC module generates aDMA request when theADC hascompleted theconversions forall
channels selected forconversion inthegroup1.
IfDMA_G1_END bitissetto1,G1_DMA_EN bitisignored andDMA requests willbe
generated every time theDMA_G1_END flaginthegroup 1status register isset.The
DMA_G1_END bitmust besetbefore enabling conversions forthegroup 1.
2 G1_BLK_XFER Group1 Block DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write totheGroup1 memory ifG1_DMA_EN is
set.
1 ADC module generates aDMA request when theADC haswritten G1_BLOCKS number of
buffers intotheGroup1 memory.
IfG1_BLK_XFER bitissetto1,G1_DMA_EN bitisignored andDMA requests willbe
generated every time theThreshold Counter reaches 0from acount value of1.
1 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 910 -->

ADC Registers www.ti.com
910 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-27. ADC Group1 DMA Control Register (ADG1DMACR) Field Descriptions (continued)
Bit Field Value Description
0 G1_DMA_EN Group1 DMA Transfer Enable.
Any operation mode read:
0 ADC module does notgenerate aDMA request when itwrites theconversion result tothe
Group1 memory.
1 ADC module generates aDMA transfer when theADC haswritten totheGroup1 memory. The
G1_BLK_XFER bitmust becleared to0forthisDMA request tobegenerated.

<!-- Page 911 -->

www.ti.com ADC Registers
911 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.22 ADC Group2 DMA Control Register (ADG2DMACR)
ADC Group2 DMA Control Register (ADG2DMACR) isshown inFigure 22-44 anddescribed inTable 22-
28.
Figure 22-44. ADC Group2 DMA Control Register (ADG2DMACR) [offset =54h]
31 25 24 16
Reserved G2_BLOCKS
R-0 R/W-0
15 8
Reserved
R-0
7 4 3 2 1 0
Reserved DMA_G2_END G2_BLK_XFER Reserved G2_DMA_EN
R-0 R/W-0 R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-28. ADC Group2 DMA Control Register (ADG2DMACR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24-16 G2_BLOCKS Number ofGroup2 Result buffers tobetransferred using DMA iftheADC module isconfigured
togenerate aDMA request. IftheGroup2 isconfigured tousetheblock transfer mode ofthe
DMA module, then theADC module generates aDMA request after theGroup2 results '
memory accumulates G2_BLOCKS number ofconversion results.
This feature isdesigned tobeused inplace ofthethreshold interrupt fortheGroup2. Asa
result, theG2_THR field oftheGroup2 Interrupt Threshold Control Register andtheG2
BLOCKS field oftheGroup2 DMA Control Register arethesame.
Any operation mode read/write:
0 NoDMA transfer occurs even ifG2_BLK_XFER issetto1.
1h-1FFh One DMA request isgenerated iftheG2_BLK_XFER issetto1andthespecified number of
Group2 conversion results have been accumulated.
15-4 Reserved 0 Reads return 0.Writes have noeffect.
3 DMA_G2_END Group2 Conversion End DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write tothegroup2 results RAM if
G2_DMA_EN isset.
1 ADC module generates aDMA request when theADC hascompleted theconversions forall
channels selected forconversion inthegroup2.
IfDMA_G2_END bitissetto1,G2_DMA_EN bitisignored andDMA requests willbe
generated every time theDMA_G2_END flaginthegroup 2status register isset.The
DMA_G2_END bitmust besetbefore enabling conversions forthegroup 2.
2 G2_BLK_XFER Group2 Block DMA Transfer Enable.
Any operation mode read:
0 ADC module generates aDMA request foreach write totheGroup2 memory ifG2_DMA_EN is
set.
1 ADC module generates aDMA request when theADC haswritten G2_BLOCKS number of
buffers intotheGroup2 memory.
IfG2_BLK_XFER bitissetto1,G2_DMA_EN bitisignored andDMA requests willbe
generated every time theThreshold Counter reaches 0from acount value of1.
1 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 912 -->

ADC Registers www.ti.com
912 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-28. ADC Group2 DMA Control Register (ADG2DMACR) Field Descriptions (continued)
Bit Field Value Description
0 G2_DMA_EN Group2 DMA Transfer Enable.
Any operation mode read:
0 ADC module does notgenerate aDMA request when itwrites theconversion result tothe
Group2 memory.
1 ADC module generates aDMA transfer when theADC haswritten totheGroup2 memory. The
G2_BLK_XFER bitmust becleared to0forthisDMA request tobegenerated.

<!-- Page 913 -->

www.ti.com ADC Registers
913 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.23 ADC Results Memory Configuration Register (ADBNDCR)
ADC Results Memory Configuration Register (ADBNDCR) [offset =0x58] isshown inFigure 22-45 and
described inTable 22-29 .
Refer toSection 22.2.7 forfurther details onhow theconversion results arestored intheADC results '
RAM.
Figure 22-45. ADC Results Memory Configuration Register (ADBNDCR) [offset =58h]
31 25 24 16
Reserved BNDA
R-0 R/W-0
15 9 8 0
Reserved BNDB
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-29. ADC Results Memory Configuration Register (ADBNDCR) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24-16 BNDA Buffer Boundary A.These bitsdetermine thememory available fortheEvent Group conversion
results. The memory available isspecified interms ofpairs ofresult buffers.
Any operation mode read/write:
0 Event Group conversions arenotrequired. IfEvent Group conversions areperformed with the
BNDA value of0,then theEvent Group memory size willdefault to1024 words. Forproper
usage oftheADC results memory, configure theBNDA value tobenon-zero andlower than the
BNDB value.
0-1FFh Atotal of(2×BNDA) buffers areavailable intheADC results memory forstoring Event Group
conversion results.
15-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 BNDB Buffer Boundary B.These bitsspecify thenumber ofbuffers allocated fortheEvent Group plus
thenumber ofbuffers allocated fortheGroup1. The number ofbuffer pairs allocated forstoring
Group1 conversion results canbedetermined bysubtracting BNDA from BNDB. Asaresult,
BNDB must always bespecified asgreater than orequal toBNDA.
Any operation mode read/write:
0 Event Group aswell asGroup1 conversions arenotrequired.
0-1FFh Atotal of2×(BNDB -BNDA) buffers areavailable intheADC results memory forstoring
Group1 conversion results.

<!-- Page 914 -->

ADC Registers www.ti.com
914 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.24 ADC Results Memory Size Configuration Register (ADBNDEND)
ADC Results Memory Size Configuration Register (ADBNDEND) isshown inFigure 22-46 anddescribed
inTable 22-30 .
Figure 22-46. ADC Results Memory Size Configuration Register (ADBNDEND) [offset =5Ch]
31 17 16
Reserved BUF_INIT_ACTIVE
R-0 R-0
15 3 2 0
Reserved BNDEND
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-30. ADC Results Memory Size Configuration Register (ADBNDEND) Field Descriptions
Bit Field Value Description
31-17 Reserved 0 Reads return 0.Writes have noeffect.
16 BUF_INIT_ACTIVE ADC Results Memory Auto-initialization Status.
Any operation mode read/write:
0 ADC Results Memory iscurrently notbeing initialized, andtheADC isavailable. Ifthisbitis
read as'0'after triggering anauto-initialization oftheADC results memory, then theADC
results memory hasbeen completely initialized tozeros. Fordevices requiring parity
checking ontheADC results memory, theparity bitintheresults memory willalso be
initialized according totheparity polarity. The parity polarity aswell astheauto-initialization
process iscontrolled bytheSystem module. Please refer toChapter 2formore details.
1 ADC results memory isbeing initialized, andtheADC isnotavailable forconversion.
15-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 BNDEND Buffer Boundary End. These bitsspecify thetotal number ofmemory buffers available for
storing theADC conversion results. These bitsshould beprogrammed tomatch thenumber
ofADC conversion result buffers required tobeused fortheapplication.
Any operation mode read/write:
0 16words available forstoring ADC conversion results.
1h 32words available forstoring ADC conversion results.
2h 64words available forstoring ADC conversion results. This isthemaximum configuration
allowed since thedevice supports 64buffers each forADC1 aswell asADC2.
4h-7h Reserved. These combinations must notbeused.

<!-- Page 915 -->

www.ti.com ADC Registers
915 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.25 ADC Event Group Sampling Time Configuration Register (ADEVSAMP)
ADC Event Group Sampling Time Configuration Register (ADEVSAMP) isshown inFigure 22-47 and
described inTable 22-31 .
Figure 22-47. ADC Event Group Sampling Time Configuration Register (ADEVSAMP) [offset =60h]
31 12 11 0
Reserved EV_ACQ
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-31. ADC Event Group Sampling Time Configuration Register (ADEVSAMP)
Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 EV_ACQ Event Group Acquisition Time. These bitsdefine thesampling window (SW) fortheEvent Group
conversions.
SW=EV_ACQ +2interms ofADCLK cycles.
There aretwofactors thatdetermine theminimum sampling window value required:
First, theADC module design requires thatSW>=3ADCLK cycles.
Second, theADC input impedance necessitates acertain minimum sampling time. This needs tobe
assured byconfiguring theEV_ACQ value properly considering thefrequency oftheADCLK signal.
Refer tothedevice datasheet todetermine theminimum sampling time forthisdevice.
22.3.26 ADC Group1 Sampling Time Configuration Register (ADG1SAMP)
ADC Group1 Sampling Time Configuration Register (ADG1SAMP) isshown inFigure 22-48 and
described inTable 22-32 .
Figure 22-48. ADC Group1 Sampling Time Configuration Register (ADG1SAMP) [offset =64h]
31 12 11 0
Reserved G1_ACQ
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-32. ADC Group1 Sampling Time Configuration Register (ADG1SAMP)
Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 G1_ACQ Group1 Acquisition Time. These bitsdefine thesampling window (SW) fortheGroup1 conversions.
SW=G1_ACQ +2interms ofADCLK cycles.
There aretwofactors thatdetermine theminimum sampling window value required:
First, theADC module design requires thatSW>=3ADCLK cycles.
Second, theADC input impedance necessitates acertain minimum sampling time. This needs tobe
assured byconfiguring theG1_ACQ value properly considering thefrequency oftheADCLK signal.
Refer tothedevice datasheet todetermine theminimum sampling time forthisdevice.

<!-- Page 916 -->

ADC Registers www.ti.com
916 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.27 ADC Group2 Sampling Time Configuration Register (ADG2SAMP)
ADC Group2 Sampling Time Configuration Register (ADG2SAMP) isshown inFigure 22-49 and
described inTable 22-33 .
Figure 22-49. ADC Group2 Sampling Time Configuration Register (ADG2SAMP) [offset =68h]
31 12 11 0
Reserved G2_ACQ
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-33. ADC Group2 Sampling Time Configuration Register (ADG2SAMP)
Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-0 G2_ACQ Group2 Acquisition Time. These bitsdefine thesampling window (SW) fortheGroup2 conversions.
SW=G2_ACQ +2interms ofADCLK cycles.
There aretwofactors thatdetermine theminimum sampling window value required:
First, theADC module design requires thatSW>=3ADCLK cycles.
Second, theADC input impedance necessitates acertain minimum sampling time. This needs tobe
assured byconfiguring theG2_ACQ value properly considering thefrequency oftheADCLK signal.
Refer tothedevice datasheet todetermine theminimum sampling time forthisdevice.

<!-- Page 917 -->

www.ti.com ADC Registers
917 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.28 ADC Event Group Status Register (ADEVSR)
ADC Event Group Status Register (ADEVSR) isshown inFigure 22-50 anddescribed inTable 22-34 .
Figure 22-50. ADC Event Group Status Register (ADEVSR) [offset =6Ch]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved EV_MEM_
EMPTYEV_BUSY EV_STOP EV_END
R-0 R-1 R-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-34. ADC Event Group Status Register (ADEVSR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 EV_MEM_EMPTY Event Group Results Memory Empty. This bitcanbeeffectively used only when theconversion
results areread outoftheEvent Group results memory inthe"read from FIFO "mode.
Any operation mode read:
0 The Event Group results memory hasvalid conversion results.
1 The Event Group results memory isempty, ordoes notcontain anyunread conversion results.
2 EV_BUSY Event Group Conversion Busy.
Any operation mode read:
0 Event Group conversions areneither inprogress norfrozen.
1 Event Group conversions areeither inprogress, orarefrozen forservicing some other group.
This bitwillalways besetwhen theEvent Group isconfigured tobeinthecontinuous
conversion mode.
1 EV_STOP Event Group Conversion Stopped.
Any operation mode read:
0 Event Group conversions arenotcurrently frozen.
1 Event Group conversions arecurrently frozen.
0 EV_END Event Group Conversions Ended.
Any operation mode read:
0 Event Group conversions have either notbeen started orhave notyetcompleted since thelast
time thisstatus bitwas cleared.
1 The conversion forallthechannels selected intheEvent Group hascompleted. This bitcanbe
cleared under thefollowing conditions:
*Byreading aconversion result from theEvent Group results memory inthe"read from FIFO "
mode.
*Bywriting anew value totheEvent Group channel select register (ADEVSEL).
*Bywriting a1tothisbit.
*Bydisabling theADC module byclearing theADC_EN bitintheADC operating mode control
register (ADOPMODECR).

<!-- Page 918 -->

ADC Registers www.ti.com
918 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.29 ADC Group1 Status Register (ADG1SR)
ADC Group1 Status Register (ADG1SR) isshown inFigure 22-51 anddescribed inTable 22-35 .
Figure 22-51. ADC Group1 Status Register (ADG1SR) [offset =70h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G1_MEM_
EMPTYG1_BUSY G1_STOP G1_END
R-0 R-1 R-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-35. ADC Group1 Status Register (ADG1SR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G1_MEM_EMPTY Group1 Results Memory Empty. This bitcanbeeffectively used only when theconversion
results areread outoftheGroup1 results memory inthe"read from FIFO "mode.
Any operation mode read:
0 The Group1 results memory hasvalid conversion results.
1 The Group1 results memory isempty, ordoes notcontain anyunread conversion results.
2 G1_BUSY Group1 Conversion Busy.
Any operation mode read:
0 Group1 conversions areneither inprogress norfrozen.
1 Group1 conversions areeither inprogress, orarefrozen forservicing some other group. This
bitwillalways besetwhen theGroup1 isconfigured tobeinthecontinuous conversion mode.
1 G1_STOP Group1 Conversion Stopped.
Any operation mode read:
0 Group1 conversions arenotcurrently frozen.
1 Group1 conversions arecurrently frozen.
0 G1_END Group1 Conversions Ended.
Any operation mode read:
0 Group1 conversions have either notbeen started orhave notyetcompleted since thelasttime
thisstatus bitwas cleared.
1 The conversion forallthechannels selected intheGroup1 hascompleted. This bitcanbe
cleared under thefollowing conditions:
*Byreading aconversion result from theGroup1 results memory inthe"read from FIFO "
mode.
*Bywriting anew value totheGroup1 channel select register (ADG1SEL).
*Bywriting a1tothisbit.
*Bydisabling theADC module byclearing theADC_EN bitintheADC operating mode control
register (ADOPMODECR).

<!-- Page 919 -->

www.ti.com ADC Registers
919 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.30 ADC Group2 Status Register (ADG2SR)
ADC Group2 Status Register (ADG2SR) isshown inFigure 22-52 anddescribed inTable 22-36 .
Figure 22-52. ADC Group2 Status Register (ADG2SR) [offset =74h]
31 8
Reserved
R-0
7 4 3 2 1 0
Reserved G2_MEM_
EMPTYG2_BUSY G2_STOP G2_END
R-0 R-1 R-0 R-0 R/W1C-0
LEGEND: R/W =Read/Write; R=Read only; W1C =Write 1toclear; -n=value after reset
Table 22-36. ADC Group2 Status Register (ADG2SR) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3 G2_MEM_EMPTY Group2 Results Memory Empty. This bitcanbeeffectively used only when theconversion
results areread outoftheGroup2 results memory inthe"read from FIFO "mode.
Any operation mode read:
0 The Group2 results memory hasvalid conversion results.
1 The Group2 results memory isempty, ordoes notcontain anyunread conversion results.
2 G2_BUSY Group2 Conversion Busy.
Any operation mode read:
0 Group2 conversions areneither inprogress norfrozen.
1 Group2 conversions areeither inprogress, orarefrozen forservicing some other group. This
bitwillalways besetwhen theGroup2 isconfigured tobeinthecontinuous conversion mode.
1 G2_STOP Group2 Conversion Stopped.
Any operation mode read:
0 Group2 conversions arenotcurrently frozen.
1 Group2 conversions arecurrently frozen.
0 G2_END Group2 Conversions Ended.
Any operation mode read:
0 Group2 conversions have either notbeen started orhave notyetcompleted since thelasttime
thisstatus bitwas cleared.
1 The conversion forallthechannels selected intheGroup2 hascompleted. This bitcanbe
cleared under thefollowing conditions:
*Byreading aconversion result from theGroup2 results memory inthe"read from FIFO "
mode.
*Bywriting anew value totheGroup2 channel select register (ADG2SEL).
*Bywriting a1tothisbit.
*Bydisabling theADC module byclearing theADC_EN bitintheADC operating mode control
register (ADOPMODECR).

<!-- Page 920 -->

ADC Registers www.ti.com
920 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.31 ADC Event Group Channel Select Register (ADEVSEL)
ADC Event Group Channel Select Register (ADEVSEL) isshown inFigure 22-53 anddescribed in
Table 22-37 .
NOTE: Clearing ADEVSEL During aConversion
Writing 0x0000 toADEVSEL stops theEvent Group conversions. This does notcause the
ADC Event Group Results Memory pointer ortheEvent Group Threshold Register tobe
reset.
NOTE: Writing ANon-Zero Value ToADEVSEL During aConversion
Writing anew value toADEVSEL while aChannel inEvent Group isbeing converted results
inanew conversion sequence starting immediately with thehighest priority channel inthe
new ADEVSEL selection. This also causes theADC Event Group Results Memory pointer to
bereset sothatthememory allocated forstoring theEvent Group conversion results gets
overwritten. Care should betaken tore-program thecorresponding Interrupt Threshold
Counter orDMA Threshold Counter again sothatcorrect number ofconversions happen
before aThreshold interrupt orBlock DMA request isgenerated.
ADC1 supports upto32channels andADC2 supports upto25channels onthemicrocontroller.
Figure 22-53. ADC Event Group Channel Select Register (ADEVSEL) [offset =78h]
31 0
EV_SEL
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 22-37. ADC Event Group Channel Select Register (ADEVSEL) Field Descriptions
Bit Field Value Description
31-0 EV_SEL Event Group channels selected.
Any operation mode read/write:
0 NoADC input channel isselected forconversion intheEvent Group.
Non-zero The channels marked bythebitpositions thataresetto1willbeconverted inascending
order when theEvent Group istriggered.

<!-- Page 921 -->

www.ti.com ADC Registers
921 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.32 ADC Group1 Channel Select Register (ADG1SEL)
ADC Group1 Channel Select Register (ADG1SEL) isshown inFigure 22-54 anddescribed inTable 22-38 .
NOTE: Clearing ADG1SEL During aConversion
Writing 0x0000 toADG1SEL stops theGroup1 conversions. This does notcause theADC
Group1 Results Memory pointer ortheGroup1 Threshold Register tobereset.
NOTE: Writing ANon-Zero Value ToADG1SEL During aConversion
Writing anew value toADG1SEL while aChannel inGroup1 isbeing converted results ina
new conversion sequence starting immediately with thehighest priority channel inthenew
ADG1SEL selection. This also causes theADC Group1 Results Memory pointer tobereset
sothatthememory allocated forstoring theGroup1 conversion results gets overwritten.
Care should betaken tore-program thecorresponding Interrupt Threshold Counter orDMA
Threshold Counter again sothatcorrect number ofconversions happen before aThreshold
interrupt orBlock DMA request isgenerated.
ADC1 supports upto32channels andADC2 supports upto25channels onthemicrocontroller.
Figure 22-54. ADC Group1 Channel Select Register (ADG1SEL) [offset =7Ch]
31 0
G1_SEL
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 22-38. ADC Group1 Channel Select Register (ADG1SEL) Field Descriptions
Bit Field Value Description
31-0 G1_SEL Group1 channels selected.
Any operation mode read/write:
0 NoADC input channel isselected forconversion intheGroup1.
Non-zero The channels marked bythebitpositions thataresetto1willbeconverted inascending
order when theGroup1 istriggered.

<!-- Page 922 -->

ADC Registers www.ti.com
922 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.33 ADC Group2 Channel Select Register (ADG2SEL)
ADC Group2 Channel Select Register (ADG2SEL) isshown inFigure 22-55 anddescribed inTable 22-39 .
NOTE: Clearing ADG2SEL During aConversion
Writing 0x0000 toADG2SEL stops theGroup2 conversions. This does notcause theADC
Group2 Results Memory pointer ortheGroup2 Threshold Register tobereset.
NOTE: Writing ANon-Zero Value ToADG2SEL During aConversion
Writing anew value toADG2SEL while aChannel inGroup2 isbeing converted results ina
new conversion sequence starting immediately with thehighest priority channel inthenew
ADG2SEL selection. This also causes theADC Group2 Results Memory pointer tobereset
sothatthememory allocated forstoring theGroup2 conversion results gets overwritten.
Care should betaken tore-program thecorresponding Interrupt Threshold Counter orDMA
Threshold Counter again sothatcorrect number ofconversions happen before aThreshold
interrupt orBlock DMA request isgenerated.
ADC1 supports upto32channels andADC2 supports upto25channels onthemicrocontroller.
Figure 22-55. ADC Group2 Channel Select Register (ADG2SEL) [offset =80h]
31 0
G2_SEL
R/W-0
LEGEND: R/W =Read/Write; -n=value after reset
Table 22-39. ADC Group2 Channel Select Register (ADG2SEL) Field Descriptions
Bit Field Value Description
31-0 G2_SEL Group2 channels selected.
Any operation mode read/write:
0 NoADC input channel isselected forconversion intheGroup2.
Non-zero The channels marked bythebitpositions thataresetto1willbeconverted inascending
order when theGroup2 istriggered.

<!-- Page 923 -->

www.ti.com ADC Registers
923 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.34 ADC Calibration andError Offset Correction Register (ADCALR)
ADC Calibration andError Offset Correction Register (ADCALR) isshown inFigure 22-56 andFigure 22-
57,anddescribed inTable 22-40 .Asshown, theformat oftheADCALR isdifferent based onwhether the
ADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-56. 12-bit ADC Calibration andError Offset Correction Register (ADCALR) [offset =84h]
31 12 11 0
Reserved ADCALR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-57. 10-bit ADC Calibration andError Offset Correction Register (ADCALR) [offset =84h]
31 10 9 0
Reserved ADCALR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-40. ADC Calibration andError Offset Correction Register (ADCALR) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
ADCALR ADC Calibration Result andOffset Error Correction Value.
The ADC module writes theresults ofthecalibration conversions tothisregister. The application is
required tousethese conversion results anddetermine theADC offset error. The application canthen
compute thecorrection fortheoffset error andthiscorrection value needs tobewritten back tothe
ADCALR register inthe2'scomplement form.
During normal conversion (when calibration isdisabled), theADCALR register contents areautomatically
added toeach digital output from theADC core before itisstored intheADC results memory. Formore
details onerror calibration, refer toSection 22.2.6.1 .
22.3.35 ADC State Machine Status Register (ADSMSTATE)
Figure 22-58 andTable 22-41 describe theADSMSTATE register.
Figure 22-58. ADC State Machine Status Register (ADSMSTATE) [offset =88h]
31 4 3 0
Reserved SMSTATE
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 22-41. ADC State Machine Status Register (ADSMSTATE) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 SMSTATE ADC State Machine Current State.
These bitsreflect thecurrent state ofthestate machine andarereserved forusebyTIfordebug
purposes.

<!-- Page 924 -->

ADC Registers www.ti.com
924 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.36 ADC Channel Last Conversion Value Register (ADLASTCONV)
ADC Channel Last Conversion Value Register (ADLASTCONV) isshown inFigure 22-59 anddescribed in
Table 22-42 .
Figure 22-59. ADC Channel Last Conversion Value Register (ADLASTCONV) [offset =8Ch]
31 24 23 0
Reserved LAST_CONV
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-42. ADC Channel Last Conversion Value Register (ADLASTCONV) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-0 LAST_CONV ADC Input Channel 'sLast Converted Value.
This register indicates whether thelastconverted value foraparticular input channel was lower or
higher than themid-point ofthereference voltage. Inother words, thisregister acts asadigital
input register andcanberead bytheapplication todetermine thedigital level attheinput pins.
This data isonly valid foraninput channel ifithasbeen converted atleast once.
Any operation mode read foreach bitofthisregister:
0 Alevel lower than themidpoint reference voltage was measured atthelastconversion forthis
channel.
1 Alevel higher than orequal tothemidpoint reference voltage was measured atthelastconversion
forthischannel.

<!-- Page 925 -->

www.ti.com ADC Registers
925 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.37 ADC Event Group Results 'FIFO Register (ADEVBUFFER)
ADC Event Group Results' FIFO Register (ADEVBUFFER) isshown inFigure 22-60 andFigure 22-61 ,
anddescribed inTable 22-43 .Asshown, theformat ofthedata read from theADEVBUFFER locations is
different based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-60. 12-bit ADC Event Group Results 'FIFO Register (ADEVBUFFER)
[offset =90h-AFh]
31 30 21 20 16
EV_EMPTY Reserved EV_CHID
R-1 R-0 R-0
15 12 11 0
Reserved EV_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-61. 10-bit ADC Event Group Results 'FIFO Register (ADEVBUFFER)
[offset =90h-AFh]
31 16
Reserved
R-0
15 14 10 9 0
EV_EMPTY EV_CHID EV_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-43. ADC Event Group Results 'FIFO Register (ADEVBUFFER) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
EV_EMPTY Event Group FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading
theEvent Group conversion results.
Any operation mode read:
0 The data intheEV_DR field ofthisbuffer isvalid.
1 The data intheEV_DR field ofthisbuffer isnotvalid andthere arenovalid data intheEvent Group results
memory.
EV_CHID Event Group Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theEvent Group conversion results.
Any operation mode read:
0 The conversion result intheEV_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheEvent Group mode control register (ADEVMODECR).
1h-1Fh The conversion result intheEV_DR field ofthisbuffer isfrom theADC input channel number denoted by
theEV_CHID field.
EV_DR Event Group Digital Conversion Result.
The Event Group results 'FIFO location isaliased eight times, sothatanyword-aligned read from the
address range 90htoAFh results inoneconversion result toberead from theEvent Group results '
memory. This allows theARM LDMIA instruction toread outupto8conversion results from theEvent
Group results 'memory with justoneinstruction.

<!-- Page 926 -->

ADC Registers www.ti.com
926 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.38 ADC Group1 Results FIFO Register (ADG1BUFFER)
ADC Group1 Results FIFO Register (ADG1BUFFER) isshown inFigure 22-62 andFigure 22-63 ,
described inTable 22-44 .Asshown, theformat ofthedata read from theADG1BUFFER locations is
different based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-62. 12-bit ADC Group1 Results FIFO Register (ADG1BUFFER)
[offset =B0h-CFh]
31 30 21 20 16
G1_EMPTY Reserved G1_CHID
R-1 R-0 R-0
15 12 11 0
Reserved G1_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-63. 10-bit ADC Group1 Results 'FIFO Register (ADG1BUFFER)
[offset =B0h-CFh]
31 16
Reserved
R-0
15 14 10 9 0
G1_EMPTY G1_CHID G1_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-44. ADC Group1 Results FIFO Register (ADG1BUFFER) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
G1_EMPTY Group1 FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading the
Group1 conversion results.
Any operation mode read:
0 The data intheG1_DR field ofthisbuffer isvalid.
1 The data intheG1_DR field ofthisbuffer isnotvalid andthere arenovalid data intheGroup1 results
memory.
G1_CHID Group1 Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theGroup1 conversion results.
Any operation mode read:
0 The conversion result intheG1_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheGroup1 mode control register (ADG1MODECR).
1h-1Fh The conversion result intheG1_DR field ofthisbuffer isfrom theADC input channel number denoted by
theG1_CHID field.
G1_DR Group1 Digital Conversion Result.
The Group1 results 'FIFO location isaliased eight times, sothatanyword-aligned read from theaddress
range B0h toCFh results inoneconversion result toberead from theGroup1 results 'memory. This allows
theARM LDMIA instruction toread outupto8conversion results from theGroup1 results 'memory with
justoneinstruction.

<!-- Page 927 -->

www.ti.com ADC Registers
927 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.39 ADC Group2 Results FIFO Register (ADG2BUFFER)
ADC Group2 Results FIFO Register (ADG2BUFFER) isshown inFigure 22-64 andFigure 22-65 ,
described inTable 22-45 .Asshown, theformat ofthedata read from theADG2BUFFER locations is
different based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module.
Figure 22-64. 12-bit ADC Group2 Results FIFO Register (ADG2BUFFER)
[offset =D0h-EFh]
31 30 21 20 16
G2_EMPTY Reserved G2_CHID
R-1 R-0 R-0
15 12 11 0
Reserved G2_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-65. 10-bit ADC Group2 Results 'FIFO Register (ADG2BUFFER)
[offset =D0h-EFh]
31 16
Reserved
R-0
15 14 10 9 0
G2_EMPTY G2_CHID G2_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-45. ADC Group2 Results FIFO Register (ADG2BUFFER) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
G2_EMPTY Group2 FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading the
Group2 conversion results.
Any operation mode read:
0 The data intheG2_DR field ofthisbuffer isvalid.
1 The data intheG2_DR field ofthisbuffer isnotvalid andthere arenovalid data intheGroup2 results
memory.
G2_CHID Group2 Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theGroup2 conversion results.
Any operation mode read:
0 The conversion result intheG2_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheGroup2 mode control register (ADG2MODECR).
1h-1Fh The conversion result intheG2_DR field ofthisbuffer isfrom theADC input channel number denoted by
theG2_CHID field.
G2_DR Group2 Digital Conversion Result.
The Group2 results 'FIFO location isaliased eight times, sothatanyword-aligned read from theaddress
range D0h toEFh results inoneconversion result toberead from theGroup2 results 'memory. This allows
theARM LDMIA instruction toread outupto8conversion results from theGroup2 results 'memory with
justoneinstruction.

<!-- Page 928 -->

ADC Registers www.ti.com
928 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.40 ADC Event Group Results Emulation FIFO Register (ADEVEMUBUFFER)
ADC Event Group Results Emulation FIFO Register (ADEVEMUBUFFER) isshown inFigure 22-66 and
Figure 22-67 ,anddescribed inTable 22-46 .Asshown, theformat ofthedata read from the
ADEVEMUBUFFER locations isdifferent based onwhether theADC module isconfigured tobea12-bit
ora10-bit ADC module.
Aread from thislocation also gives outoneconversion result from theEvent Group results 'memory along
with theEV_EMPTY status bitandtheoptional channel id.However, thisread willnotaffect anyofthe
status flags intheEvent Group interrupt flagregister ortheEvent Group status register. This register is
useful fordebuggers.
Figure 22-66. 12-bit ADC Event Group Results Emulation FIFO Register (ADEVEMUBUFFER)
[offset =F0h]
31 30 21 20 16
EV_EMPTY Reserved EV_CHID
R-1 R-0 R-0
15 12 11 0
Reserved EV_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-67. 10-bit ADC Event Group Results Emulation FIFO Register (ADEVEMUBUFFER)
[offset =F0h]
31 16
Reserved
R-0
15 14 10 9 0
EV_EMPTY EV_CHID EV_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-46. ADC Event Group Results Emulation FIFO Register (ADEVEMUBUFFER)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
EV_EMPTY Event Group FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading
theEvent Group conversion results.
Any operation mode read:
0 The data intheEV_DR field ofthisbuffer isvalid.
1 The data intheEV_DR field ofthisbuffer isnotvalid andthere arenovalid data intheEvent Group results
memory.
EV_CHID Event Group Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theEvent Group conversion results.
Any operation mode read:
0 The conversion result intheEV_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheEvent Group operating mode control register (ADEVMODECR).
1h-1Fh The conversion result intheEV_DR field ofthisbuffer isfrom theADC input channel number denoted by
theEV_CHID field.
EV_DR Event Group Digital Conversion Result.
These bitscontain thedigital result output from theEvent Group FIFO buffer. The result canbepresented
inan8-bit, 10-bit, or12-bit format fora12-bit ADC module, orinan8-bit or10-bit format fora10-bit ADC
module. The conversion result data isautomatically shifted right bytheappropriate number ofbitswhen
using areduced-size data format with theupper bitsreading aszeros.

<!-- Page 929 -->

www.ti.com ADC Registers
929 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.41 ADC Group1 Results Emulation FIFO Register (ADG1EMUBUFFER)
ADC Group1 Results Emulation FIFO Register (ADG1EMUBUFFER) isshown inFigure 22-68 and
Figure 22-69 ,described inTable 22-47 .Asshown, theformat ofthedata read from the
ADG1EMUBUFFER locations isdifferent based onwhether theADC module isconfigured tobea12-bit
ora10-bit ADC module.
Aread from thislocation also gives outoneconversion result from theGroup1 results 'memory along with
theG1_EMPTY status bitandtheoptional channel id.However, thisread willnotaffect anyofthestatus
flags intheGroup1 interrupt flagregister ortheGroup1 status register. This register isuseful for
debuggers.
Figure 22-68. 12-bit ADC Group1 Results Emulation FIFO Register (ADG1EMUBUFFER)
[offset =F4h]
31 30 21 20 16
G1_EMPTY Reserved G1_CHID
R-1 R-0 R-0
15 12 11 0
Reserved G1_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-69. 10-bit ADC Group1 Results Emulation FIFO Register (ADG1EMUBUFFER)
[offset =F4h]
31 16
Reserved
R-0
15 14 10 9 0
G1_EMPTY G1_CHID G1_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-47. ADC Group1 Results Emulation FIFO Register (ADG1EMUBUFFER) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
G1_EMPTY Group1 FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading the
Group1 conversion results.
Any operation mode read:
0 The data intheG1_DR field ofthisbuffer isvalid.
1 The data intheG1_DR field ofthisbuffer isnotvalid andthere arenovalid data intheGroup1 results
memory.
G1_CHID Group1 Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theGroup1 conversion results.
Any operation mode read:
0 The conversion result intheG1_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheGroup1 operating mode control register (ADG1MODECR).
1h-1Fh The conversion result intheG1_DR field ofthisbuffer isfrom theADC input channel number denoted by
theG1_CHID field.
G1_DR Group1 Digital Conversion Result.
These bitscontain thedigital result output from theGroup 1FIFO buffer. The result canbepresented inan
8-bit, 10-bit, or12-bit format fora12-bit ADC module, orinan8-bit or10-bit format fora10-bit ADC
module. The conversion result data isautomatically shifted right bytheappropriate number ofbitswhen
using areduced-size data format with theupper bitsreading aszeros.

<!-- Page 930 -->

ADC Registers www.ti.com
930 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.42 ADC Group2 Results Emulation FIFO Register (ADG2EMUBUFFER)
ADC Group2 Results Emulation FIFO Register (ADG2EMUBUFFER) isshown inFigure 22-70 and
Figure 22-71 ,described inTable 22-48 .Asshown, theformat ofthedata read from the
ADG2EMUBUFFER locations isdifferent based onwhether theADC module isconfigured tobea12-bit
ora10-bit ADC module.
Aread from thislocation also gives outoneconversion result from theGroup2 results 'memory along with
theG2_EMPTY status bitandtheoptional channel id.However, thisread willnotaffect anyofthestatus
flags intheGroup2 interrupt flagregister ortheGroup2 status register. This register isuseful for
debuggers.
Figure 22-70. 12-bit ADC Group2 Results Emulation FIFO Register (ADG2EMUBUFFER)
[offset =F8h]
31 30 21 20 16
G2_EMPTY Reserved G2_CHID
R-1 R-0 R-0
15 12 11 0
Reserved G2_DR
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Figure 22-71. 10-bit ADC Group2 Results Emulation FIFO Register (ADG2EMUBUFFER)
[offset =F8h]
31 16
Reserved
R-0
15 14 10 9 0
G2_EMPTY G2_CHID G2_DR
R-1 R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-48. ADC Group2 Results Emulation FIFO Register (ADG2EMUBUFFER) Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
G2_EMPTY Group2 FIFO Empty. This bitisapplicable only when the"read from FIFO "mode isused forreading the
Group2 conversion results.
Any operation mode read:
0 The data intheG2_DR field ofthisbuffer isvalid.
1 The data intheG2_DR field ofthisbuffer isnotvalid andthere arenovalid data intheGroup2 results
memory.
G2_CHID Group2 Channel Id.These bitsarealso applicable only when the"read from FIFO "mode isused for
reading theGroup2 conversion results.
Any operation mode read:
0 The conversion result intheG2_DR field ofthisbuffer isfrom theADC input channel 0,orthechannel id
mode isdisabled intheGroup2 operating mode control register (ADG2MODECR).
1h-1Fh The conversion result intheG2_DR field ofthisbuffer isfrom theADC input channel number denoted by
theG2_CHID field.
G2_DR Group2 Digital Conversion Result.
These bitscontain thedigital result output from theGroup 2FIFO buffer. The result canbepresented inan
8-bit, 10-bit, or12-bit format fora12-bit ADC module, orinan8-bit or10-bit format fora10-bit ADC
module. The conversion result data isautomatically shifted right bytheappropriate number ofbitswhen
using areduced-size data format with theupper bitsreading aszeros.

<!-- Page 931 -->

www.ti.com ADC Registers
931 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.43 ADC ADEVT PinDirection Control Register (ADEVTDIR)
ADC ADEVT PinDirection Control Register (ADEVTDIR) isshown inFigure 22-72 anddescribed in
Table 22-49 .
Figure 22-72. ADC ADEVT PinDirection Control Register (ADEVTDIR) [offset =FCh]
31 1 0
Reserved ADEVT_DIR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-49. ADC ADEVT PinDirection Control Register (ADEVTDIR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_DIR ADEVT PinDirection.
Any operating mode read/write:
0 ADEVT isaninput pin;theoutput buffer isdisabled.
1 ADEVT isanoutput pin;theoutput buffer isenabled.

<!-- Page 932 -->

ADC Registers www.ti.com
932 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.44 ADC ADEVT PinOutput Value Control Register (ADEVTOUT)
ADC ADEVT PinOutput Value Control Register (ADEVTOUT) isshown inFigure 22-73 anddescribed in
Table 22-50 .
Figure 22-73. ADC ADEVT PinOutput Value Control Register (ADEVTOUT) [offset =100h]
31 1 0
Reserved ADEVT_OUT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-50. ADC ADEVT PinOutput Value Control Register (ADEVTOUT) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_OUT ADEVT PinOutput Value. This bitdetermines thelogic level tobeoutput totheADEVT pinwhen
thepinisconfigured tobeanoutput pin.
Any operating mode read/write:
0 Output logic LOW ontheADEVT pin.
1 Output logic HIGH ontheADEVT pin.
22.3.45 ADC ADEVT PinInput Value Register (ADEVTIN)
ADC ADEVT PinInput Value Register (ADEVTIN) isshown inFigure 22-74 anddescribed inTable 22-51 .
Figure 22-74. ADC ADEVT PinInput Value Register (ADEVTIN) [offset =104h]
31 1 0
Reserved ADEVT_IN
R-0 R-U
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-51. ADC ADEVT PinInput Value Register (ADEVTIN) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_IN ADEVT PinInput Value. This isaread-only bitthatreflects thelogic level ontheADEVT pin.
Any operating mode read:
0 Logic LOW present ontheADEVT pin.
1 Logic HIGH present ontheADEVT pin.

<!-- Page 933 -->

www.ti.com ADC Registers
933 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.46 ADC ADEVT PinSetRegister (ADEVTSET)
ADC ADEVT PinSetRegister (ADEVTSET) isshown inFigure 22-75 anddescribed inTable 22-52 .
Figure 22-75. ADC ADEVT PinSetRegister (ADEVTSET) [offset =108h]
31 1 0
Reserved ADEVT_SET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-52. ADC ADEVT PinSetRegister (ADEVTSET) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_SET ADEVT PinSet. This bitdrives theoutput oftheADEVT pinhigh. Aread from thisbitalways
returns thecurrent state oftheADEVT pin.
Any operating mode read/write:
0 Output value ontheADEVT pinisunchanged.
1 Output logic HIGH ontheADEVT pin,ifthepinisconfigured tobeanoutput pin.
22.3.47 ADC ADEVT PinClear Register (ADEVTCLR)
ADC ADEVT PinClear Register (ADEVTCLR) isshown inFigure 22-76 anddescribed inTable 22-53 .
Figure 22-76. ADC ADEVT PinClear Register (ADEVTCLR) [offset =10Ch]
31 1 0
Reserved ADEVT_CLR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-53. ADC ADEVT PinClear Register (ADEVTCLR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_CLR ADEVT PinClear. Aread from thisbitalways returns thecurrent state oftheADEVT pin.
Any operating mode read/write:
0 Output value ontheADEVT pinisunchanged.
1 Output logic LOW ontheADEVT pin,ifthepinisconfigured tobeanoutput pin.

<!-- Page 934 -->

ADC Registers www.ti.com
934 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.48 ADC ADEVT PinOpen Drain Enable Register (ADEVTPDR)
ADC ADEVT PinOpen Drain Enable Register (ADEVTPDR) isshown inFigure 22-77 anddescribed in
Table 22-54 .
Figure 22-77. ADC ADEVT PinOpen Drain Enable Register (ADEVTPDR) [offset =110h]
31 1 0
Reserved ADEVT_PDR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-54. ADC ADEVT PinOpen Drain Enable Register (ADEVTPDR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_PDR ADEVT PinOpen Drain Enable. This bitenables theopen-drain capability fortheADEVT pinifitis
configured tobeanoutput andalogic HIGH isbeing driven ontothepin.
Any operating mode read/write:
0 Output value ontheADEVT pinislogic HIGH.
1 The ADEVT pinistristated.
22.3.49 ADC ADEVT PinPull Control Disable Register (ADEVTPDIS)
ADC ADEVT PinPullControl Disable Register (ADEVTPDIS) isshown inFigure 22-78 anddescribed in
Table 22-55 .
Figure 22-78. ADC ADEVT PinPullControl Disable Register (ADEVTPDIS) [offset =114h]
31 1 0
Reserved ADEVT_PDIS
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-55. ADC ADEVT PinPullControl Disable Register (ADEVTPDIS) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_PDIS ADEVT PinPullControl Disable. This bitenables ordisables thepullcontrol ontheADEVT pinifit
isconfigured tobeaninput pin.
Any operating mode read/write:
0 PullonADEVT pinisenabled.
1 PullonADEVT pinisdisabled.

<!-- Page 935 -->

www.ti.com ADC Registers
935 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.50 ADC ADEVT PinPull Control Select Register (ADEVTPSEL)
ADC ADEVT PinPullControl Select Register (ADEVTPSEL) isshown inFigure 22-79 anddescribed in
Table 22-56 .
Figure 22-79. ADC ADEVT PinPullControl Select Register (ADEVTPSEL) [offset =118h]
31 1 0
Reserved ADEVT_PSEL
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-56. ADC ADEVT PinPullControl Select Register (ADEVTPSEL) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 ADEVT_PSEL ADEVT PinPullControl Select. This bitselects apull-down orpull-up ontheADEVT pinifitis
configured tobeaninput pin.
Any operating mode read/write:
0 Pull-down isselected onADEVT pin.
1 Pull-up isselected onADEVT pin.
22.3.51 ADC Event Group Sample Cap Discharge Control Register (ADEVSAMPDISEN)
ADC Event Group Sample Cap Discharge Control Register (ADEVSAMPDISEN) isshown inFigure 22-80
anddescribed inTable 22-57 .
Figure 22-80. ADC Event Group Sample Cap Discharge Control Register (ADEVSAMPDISEN)
[offset =11Ch]
31 16
Reserved
R-0
15 8 7 1 0
EV_SAMP_DIS_CYC Reserved EV_SAMP_
DIS_EN
R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-57. ADC Event Group Sample Cap Discharge Control Register (ADEVSAMPDISEN)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 EV_SAMP_DIS_CYC Event Group sample capdischarge cycles. These bitsspecify theduration interms ofADCLK
cycles forwhich theADC internal sampling capacitor isallowed todischarge before sampling
theinput channel voltage.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 EV_SAMP_DIS_EN Event Group sample capdischarge enable.
Any operation mode read/write:
0 Event Group sample capdischarge mode isdisabled.
1 Event Group sample capdischarge mode isenabled. The ADC internal sampling capacitor is
connected totheVREFLO reference voltage foraduration specified bytheEV_SAMP_DIS_CYC
field. After thisdischarge time hasexpired theselected ADC input channel issampled and
converted normally based ontheEvent Group settings.

<!-- Page 936 -->

ADC Registers www.ti.com
936 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.52 ADC Group1 Sample Cap Discharge Control Register (ADG1SAMPDISEN)
ADC Group1 Sample Cap Discharge Control Register (ADG1SAMPDISEN) isshown inFigure 22-81 and
described inTable 22-58 .
Figure 22-81. ADC Group1 Sample Cap Discharge Control Register (ADG1SAMPDISEN)
[offset =120h]
31 16
Reserved
R-0
15 8 7 1 0
G1_SAMP_DIS_CYC Reserved G1_SAMP_
DIS_EN
R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-58. ADC Group1 Sample Cap Discharge Control Register (ADG1SAMPDISEN)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 G1_SAMP_DIS_CYC Group1 sample capdischarge cycles. These bitsspecify theduration interms ofADCLK cycles
forwhich theADC internal sampling capacitor isallowed todischarge before sampling theinput
channel voltage.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 G1_SAMP_DIS_EN Group1 sample capdischarge enable.
Any operation mode read/write:
0 Group1 sample capdischarge mode isdisabled.
1 Group1 sample capdischarge mode isenabled. The ADC internal sampling capacitor is
connected totheVREFLO reference voltage foraduration specified bytheG1_SAMP_DIS_CYC
field. After thisdischarge time hasexpired theselected ADC input channel issampled and
converted normally based ontheGroup1 settings.

<!-- Page 937 -->

www.ti.com ADC Registers
937 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.53 ADC Group2 Sample Cap Discharge Control Register (ADG2SAMPDISEN)
ADC Group2 Sample Cap Discharge Control Register (ADG2SAMPDISEN) isshown inFigure 22-82 and
described inTable 22-59 .
Figure 22-82. ADC Group2 Sample Cap Discharge Control Register (ADG2SAMPDISEN)
[offset =124h]
31 16
Reserved
R-0
15 8 7 1 0
G2_SAMP_DIS_CYC Reserved G2_SAMP_
DIS_EN
R/W-0 R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-59. ADC Group2 Sample Cap Discharge Control Register (ADG2SAMPDISEN)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 G2_SAMP_DIS_CYC Group2 sample capdischarge cycles. These bitsspecify theduration interms ofADCLK cycles
forwhich theADC internal sampling capacitor isallowed todischarge before sampling theinput
channel voltage.
7-1 Reserved 0 Reads return 0.Writes have noeffect.
0 G2_SAMP_DIS_EN Group2 sample capdischarge enable.
Any operation mode read/write:
0 Group2 sample capdischarge mode isdisabled.
1 Group2 sample capdischarge mode isenabled. The ADC internal sampling capacitor is
connected totheVREFLO reference voltage foraduration specified bytheG2_SAMP_DIS_CYC
field. After thisdischarge time hasexpired theselected ADC input channel issampled and
converted normally based ontheGroup2 settings.

<!-- Page 938 -->

ADC Registers www.ti.com
938 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.54 ADC Magnitude Compare Interrupt Control Registers (ADMAGINTxCR)
ADC Magnitude Compare Interrupt Control Registers (ADMAGINTxCR) areshown inFigure 22-83 and
Figure 22-84 ,anddescribed inTable 22-60 .Asshown, theformat oftheADMAGINTxCR isdifferent
based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module. The ADC module
supports uptothree magnitude compare interrupts. These registers areatoffset addresses 128h, 130h,
and138h.
Figure 22-83. 12-bit ADC Magnitude Compare Interrupt Control Registers (ADMAGINTxCR)
[offset =128h-138h]
31 28 27 16
Reserved MAG_THRx
R-0 R/W-0
15 14 13 12 8
CHN_THR_
COMPxCMP_GE_LTx Reserved COMP_CHIDx
R/W-0 R/W-0 R-0 R/W-0
7 5 4 0
Reserved MAG_CHIDx
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-84. 10-bit ADC Magnitude Compare Interrupt Control Registers (ADMAGINTxCR)
[offset =128h-138h]
31 30 26 25 16
Rsvd MAG_CHIDx MAG_THRx
R-0 R/W-0 R/W-0
15 13 12 8
Reserved COMP_CHIDx
R-0 R/W-0
7 2 1 0
Reserved CHN_THR_
COMPxCMP_GE_LTx
R-0 R/W-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset

<!-- Page 939 -->

www.ti.com ADC Registers
939 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) ModuleTable 22-60. ADC Magnitude Compare Interrupt Control Registers (ADMAGINTxCR)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
MAG_CHIDx These bitsspecify thechannel number from 0to31forwhich theconversion result needs tobe
monitored bytheADC.
MAG_THRx These bitsspecify the12-bit or10-bit compare value thattheADC willuseforthecomparison with the
MAG_CHIDx channel 'sconversion result.
COMP_CHIDx These bitsspecify thechannel number from 0to31whose lastconversion result iscompared with the
MAG_CHIDx channel 'sconversion result.
CHN_THR_COMPx Channel ORThreshold comparison.
Any operation mode read/write:
0 The ADC module willcompare theMAG_CHIDx channel 'sconversion result with thefixed threshold
value specified bytheMAG_THRx field
1 The ADC module willcompare theMAG_CHIDx channel 'sconversion result with thelastconversion
result fortheCOMP_CHIDx channel.
Both theMAG_CHIDx andtheCOMP_CHIDx channel must have been converted atleast once forthe
ADC toperform thecomparison.
CMP_GE_LTx "Greater than orequal to"OR"Less than"comparison operator.
Any operation mode read/write:
0 The ADC module willcheck iftheconversion result islower than thereference value (fixed threshold
orCOMP_CHIDx conversion result).
1 The ADC module willcheck iftheconversion result isgreater than orequal tothereference value
(fixed threshold orCOMP_CHIDx conversion result).

<!-- Page 940 -->

ADC Registers www.ti.com
940 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.55 ADC Magnitude Compare Interruptx Mask Register (ADMAGINTxMASK)
ADC Magnitude Compare Interruptx Mask Register (ADMAGINTxMASK) isshown inFigure 22-85 and
Figure 22-86 ,anddescribed inTable 22-61 .Asshown, theformat oftheADMAGINTxMASK isdifferent
based onwhether theADC module isconfigured tobea12-bit ora10-bit ADC module. There arethree
mask registers forthethree magnitude compare interrupts. These registers areatoffset addresses 12Ch,
134h, and13Ch.
Figure 22-85. 12-bit ADC Magnitude Compare Mask Register (ADMAGINTxMASK)
[offset =12Ch-13Ch]
31 12 11 0
Reserved MAG_INTx_MASK
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Figure 22-86. 10-bit ADC Magnitude Compare Mask Register (ADMAGINTxMASK)
[offset =12Ch-13Ch]
31 10 9 0
Reserved MAG_INTx_MASK
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-61. ADC Magnitude Compare Interruptx Mask Register (ADMAGINTxMASK)
Field Descriptions
Field Value Description
Reserved 0 Reads return 0.Writes have noeffect.
MAG_INTx_MASK These bitsspecify themask forthecomparison inorder togenerate themagnitude compare interrupt #x.
Any operation mode read/write:
0 The ADC module willnotmask thecorresponding bitforthecomparison.
1 The ADC module willmask thecorresponding bitforthecomparison.

<!-- Page 941 -->

www.ti.com ADC Registers
941 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.56 ADC Magnitude Compare Interrupt Enable SetRegister (ADMAGINTENASET)
ADC Magnitude Compare Interrupt Enable SetRegister (ADMAGINTENASET) isshown inFigure 22-87
anddescribed inTable 22-62 .
Figure 22-87. ADC Magnitude Compare Interrupt Enable SetRegister (ADMAGINTENASET)
[offset =158h]
31 3 2 0
Reserved MAG_INT_ENA_SET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-62. ADC Magnitude Compare Interrupt Enable SetRegister (ADMAGINTENASET)
Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 MAG_INT_ENA_SET Each ofthese three bits, when set,enable thecorresponding magnitude compare interrupt.
Any operation mode read/write foreach bit:
0 The enable status ofthecorresponding magnitude compare interrupt isleftunchanged.
1 The corresponding magnitude compare interrupt isenabled.
22.3.57 ADC Magnitude Compare Interrupt Enable Clear Register (ADMAGINTENACLR)
ADC Magnitude Compare Interrupt Enable Clear Register (ADMAGINTENACLR) isshown inFigure 22-88
anddescribed inTable 22-63 .
Figure 22-88. ADC Magnitude Compare Interrupt Enable Clear Register (ADMAGINTENACLR)
[offset =15Ch]
31 3 2 0
Reserved MAG_INT_ENA_CLR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-63. ADC Magnitude Compare Interrupt Enable Clear Register (ADMAGINTENACLR)
Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 MAG_INT_ENA_CLR Each ofthese three bits, when set,enable thecorresponding magnitude compare interrupt.
Any operation mode read/write foreach bit:
0 The enable status ofthecorresponding magnitude compare interrupt isleftunchanged.
1 The corresponding magnitude compare interrupt isdisabled.

<!-- Page 942 -->

ADC Registers www.ti.com
942 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.58 ADC Magnitude Compare Interrupt Flag Register (ADMAGINTFLG)
ADC Magnitude Compare Interrupt Flag Register (ADMAGINTFLG) isshown inFigure 22-89 and
described inTable 22-64 .
Figure 22-89. ADC Magnitude Compare Interrupt Flag Register (ADMAGINTFLG) [offset =160h]
31 3 2 0
Reserved MAG_INT_FLG
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-64. ADC Magnitude Compare Interrupt Flag Register (ADMAGINTFLG) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 MAG_INT_FLG Magnitude Compare Interrupt Flags. These bitscanbepolled bytheapplication todetermine ifthe
magnitude compares have been evaluated astrue. When amagnitude compare interrupt flagisset,
thecorresponding magnitude compare interrupt willbegenerated ifenabled.
Any operation mode, foreach bit:
0 Read: The condition forthecorresponding magnitude threshold interrupt was false.
Write: The corresponding flagisleftunchanged.
1 Read: The condition forthecorresponding magnitude threshold interrupt was true.
Write: The corresponding flagiscleared. The flagcanalso becleared byreading from the
magnitude compare interrupt offset register.
22.3.59 ADC Magnitude Compare Interrupt Offset Register (ADMAGINTOFF)
ADC Magnitude Compare Interrupt Offset Register (ADMAGINTOFF) isshown inFigure 22-90 and
described inTable 22-65 .
Figure 22-90. ADC Magnitude Compare Interrupt Offset Register (ADMAGINTOFF) [offset =164h]
31 4 3 0
Reserved MAG_INT_OFF
R-0 RC-0
LEGEND: R=Read only; RC=Clear field after read; -n=value after reset
Table 22-65. ADC Magnitude Compare Interrupt Offset Register (ADMAGINTOFF) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MAG_INT_OFF Magnitude Compare Interrupt Offset. This field indexes thecurrently highest-priority magnitude
compare interrupt. Interrupt 1hasthehighest priority andinterrupt 3hasthelowest priority among
themagnitude compare interrupts.
Writes tothese bitshave noeffect. Aread from thisregister clears thisregister aswell asthe
corresponding magnitude compare interrupt flagintheADMAGINTFLG register. However, aread
from thisregister inemulation mode does notaffect thisregister ortheinterrupt status flags.
Any operation mode read:
0 Nomagnitude compare interrupt ispending.
1h Magnitude compare interrupt #1ispending.
2h Magnitude compare interrupt #2ispending.
3h Magnitude compare interrupt #3ispending.
4h-Fh Reserved.

<!-- Page 943 -->

www.ti.com ADC Registers
943 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.60 ADC Event Group FIFO Reset Control Register (ADEVFIFORESETCR)
ADC Event Group FIFO Reset Control Register (ADEVFIFORESETCR) isshown inFigure 22-91 and
described inTable 22-66 .
Figure 22-91. ADC Event Group FIFO Reset Control Register (ADEVFIFORESETCR) [offset =168h]
31 1 0
Reserved EV_FIFO_RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-66. ADC Event Group FIFO Reset Control Register (ADEVFIFORESETCR)
Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 EV_FIFO_RESET ADC Event Group FIFO Reset. The application cansetthisbitincase ofanoverrun condition. This
allows theADC module tooverwrite thecontents oftheEvent Group results memory starting from
thefirstlocation.
When thisbitissetto1,theADC module resets itsinternal Event Group results memory pointers.
Then thisbitautomatically gets cleared, sothattheADC module allows theEvent Group results
memory tobeoverwritten only once each time thisbitissetto1.Asaresult, theEV_FIFO_RESET
bitwillalways beread asa0.
The EV_FIFO_RESET bitwillonly have thedesired effect when theEvent Group results memory is
inanoverrun condition. Itmust beused when thedata already available intheresults memory can
bediscarded.
Iftheapplication needs theEvent Group memory toalways beoverwritten with thelatest available
conversion results, then theOVR_EV_RAM_IGN bitintheEvent Group operating mode control
register (ADEVMODECR) needs tobesetto1.
22.3.61 ADC Group1 FIFO Reset Control Register (ADG1FIFORESETCR)
ADC Group1 FIFO Reset Control Register (ADG1FIFORESETCR) isshown inFigure 22-92 and
described inTable 22-67 .
Figure 22-92. ADC Group1 FIFO Reset Control Register (ADG1FIFORESETCR) [offset =16Ch]
31 1 0
Reserved G1_FIFO_RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-67. ADC Group1 FIFO Reset Control Register (ADG1FIFORESETCR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 G1_FIFO_RESET ADC Group1 FIFO Reset. The application cansetthisbitincase ofanoverrun condition. This
allows theADC module tooverwrite thecontents oftheGroup1 results memory starting from the
firstlocation.
When thisbitissetto1,theADC module resets itsinternal Group1 results memory pointers. Then
thisbitautomatically gets cleared, sothattheADC module allows theGroup1 results memory tobe
overwritten only once each time thisbitissetto1.Asaresult, theG1_FIFO_RESET bitwillalways
beread asa0.
The G1_FIFO_RESETbit willonly have thedesired effect when theGroup1 results memory isinan
overrun condition. Itmust beused when thedata already available intheresults memory canbe
discarded.
Iftheapplication needs theGroup1 memory toalways beoverwritten with thelatest available
conversion results, then theOVR_G1_RAM_IGN bitintheGroup1 operating mode control register
(ADG1MODECR) needs tobesetto1.

<!-- Page 944 -->

ADC Registers www.ti.com
944 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.62 ADC Group2 FIFO Reset Control Register (ADG2FIFORESETCR)
ADC Group2 FIFO Reset Control Register (ADG2FIFORESETCR) isshown inFigure 22-93 and
described inTable 22-68 .
Figure 22-93. ADC Group2 FIFO Reset Control Register (ADG2FIFORESETCR) [offset =170h]
31 1 0
Reserved G2_FIFO_RESET
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-68. ADC Group2 FIFO Reset Control Register (ADG2FIFORESETCR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 G2_FIFO_RESET ADC Group2 FIFO Reset. The application cansetthisbitincase ofanoverrun condition. This
allows theADC module tooverwrite thecontents oftheGroup2 results memory starting from the
firstlocation.
When thisbitissetto1,theADC module resets itsinternal Group2 results memory pointers. Then
thisbitautomatically gets cleared, sothattheADC module allows theGroup2 results memory tobe
overwritten only once each time thisbitissetto1.Asaresult, theG2_FIFO_RESET bitwillalways
beread asa0.
The G2_FIFO_RESET bitwillonly have thedesired effect when theGroup2 results memory isinan
overrun condition. Itmust beused when thedata already available intheresults memory canbe
discarded.
Iftheapplication needs theGroup2 memory toalways beoverwritten with thelatest available
conversion results, then theOVR_G2_RAM_IGN bitintheGroup2 operating mode control register
(ADG2MODECR) needs tobesetto1.
22.3.63 ADC Event Group RAM Write Address Register (ADEVRAMWRADDR)
ADC Event Group RAM Write Address Register (ADEVRAMWRADDR) isshown inFigure 22-94 and
described inTable 22-69 .
Figure 22-94. ADC Event Group RAM Write Address Register (ADEVRAMWRADDR) [offset =174h]
31 9 8 0
Reserved EV_RAM_ADDR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-69. ADC Event Group RAM Write Address Register (ADEVRAMWRADDR)
Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 EV_RAM_ADDR Event Group results memory write pointer. This field shows theaddress ofthelocation where the
next Event Group conversion result willbestored. This isspecified interms ofthebuffer number.
The application canread thisregister todetermine thenumber ofvalid Event Group conversion
results available until thattime.

<!-- Page 945 -->

www.ti.com ADC Registers
945 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.64 ADC Group1 RAM Write Address Register (ADG1RAMWRADDR)
ADC Group1 RAM Write Address Register (ADG1RAMWRADDR) isshown inFigure 22-95 anddescribed
inTable 22-70 .
Figure 22-95. ADC Group1 RAM Write Address Register (ADG1RAMWRADDR) [offset =178h]
31 9 8 0
Reserved G1_RAM_ADDR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-70. ADC Group1 RAM Write Address Register (ADG1RAMWRADDR)
Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 G1_RAM_ADDR Group1 results memory write pointer. This field shows theaddress ofthelocation where thenext
Group1 conversion result willbestored. This isspecified interms ofthebuffer number.
The application canread thisregister todetermine thenumber ofvalid Group1 conversion results
available until thattime.
22.3.65 ADC Group2 RAM Write Address Register (ADG2RAMWRADDR)
ADC Group2 RAM Write Address Register (ADG2RAMWRADDR) isshown inFigure 22-96 anddescribed
inTable 22-71 .
Figure 22-96. ADC Group2 RAM Write Address Register (ADG2RAMWRADDR) [offset =17Ch]
31 9 8 0
Reserved G2_RAM_ADDR
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-71. ADC Group2 RAM Write Address Register (ADG2RAMWRADDR)
Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8-0 G2_RAM_ADDR Group2 results memory write pointer. This field shows theaddress ofthelocation where thenext
Group2 conversion result willbestored. This isspecified interms ofthebuffer number.
The application canread thisregister todetermine thenumber ofvalid Group2 conversion results
available until thattime.

<!-- Page 946 -->

ADC Registers www.ti.com
946 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.66 ADC Parity Control Register (ADPARCR)
ADC Parity Control Register (ADPARCR) isshown inFigure 22-97 anddescribed inTable 22-72 .
Figure 22-97. ADC Parity Control Register (ADPARCR) [offset =180h]
31 16
Reserved
R-0
15 9 8 7 4 3 0
Reserved TEST Reserved PARITY_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 22-72. ADC Parity Control Register (ADPARCR) Field Descriptions
Bit Field Value Description
31-9 Reserved 0 Reads return 0.Writes have noeffect.
8 TEST This bitmaps theparity bitsintotheADC results 'RAM frame sothattheapplication can
access them.
Any operation mode read, privileged mode write:
0 The parity bitsarenotmemory-mapped.
1 The parity bitsarememory-mapped.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 PARITY_ENA Enable parity checking. These bitsenable theparity check onread operations andtheparity
calculation onwrite operations totheADC results memory.
Ifparity checking isenabled andaparity error isdetected theADC module sends aparity
error signal totheSystem module.
Any operation mode read, privileged mode write:
5h Parity check isdisabled.
Allother values Parity check isenabled.

<!-- Page 947 -->

www.ti.com ADC Registers
947 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.67 ADC Parity Error Address Register (ADPARADDR)
ADC Parity Error Address Register (ADPARADDR) isshown inFigure 22-98 anddescribed inTable 22-73 .
Figure 22-98. ADC Parity Error Address Register (ADPARADDR) [offset =184h]
31 16
Reserved
R-0
15 12 11 2 1 0
Reserved ERROR_ADDRESS Reserved
R-0 R-U R-0
LEGEND: R=Read only; -n=value after reset; U=value after reset isunknown
Table 22-73. ADC Parity Error Address Register (ADPARADDR) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-2 ERROR_ADDRESS These bitshold theaddress ofthefirstparity error generated intheADC results 'RAM. This
error address isfrozen from being updated until itisread bytheapplication. Inemulation mode,
thisaddress ismaintained frozen even when read.
1-0 Reserved 0 Reads return 0.Writes have noeffect. Reading [11:0] provides the32-bit aligned address.
22.3.68 ADC Power-Up Delay Control Register (ADPWRUPDLYCTRL)
Figure 22-99 andTable 22-74 describe theADPWRDLYCTRL register.
Figure 22-99. ADC Power-Up Delay Control Register (ADPWRUPDLYCTRL) [offset =188h]
31 10 9 0
Reserved PWRUP_DLY
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-74. ADC Power-Up Delay Control Register (ADPWRUPDLYCTRL) Field Descriptions
Bit Field Value Description
31-10 Reserved 0 Reads return 0.Writes have noeffect.
9-0 PWRUP_DLY This register defines thenumber ofVCLK cycles thattheADC state machine hastowait after
releasing theADC core from power down before starting anew conversion. Refer to
Section 22.2.6.3 formore details.

<!-- Page 948 -->

ADC Registers www.ti.com
948 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.69 ADC Event Group Channel Selection Mode Control Register
(ADEVCHNSELMODECTRL)
Figure 22-100 andTable 22-75 describe theADEVCHNSELMODECTRL register.
Figure 22-100. ADC Event Group Channel Selection Mode Control Register
(ADEVCHNSELMODECTRL) (offset =190h)
31 4 3 0
Reserved EV_ENH_CHNSEL_MODE_ENABLE
R-0 R/W-5h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-75. ADC Event Group Channel Selection Mode Control Register
(ADEVCHNSELMODECTRL) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 EV_ENH_CHNSEL_
MODE_ENABLEEnable enhanced channel selection mode forEvent group. Refer toSection 22.2.2.2.2 for
adescription oftheenhanced channel selection mode.
5h Read: Indicates thattheenhanced channel selection mode forEvent group isnotenabled.
The default sequential channel selection mode isused forEvent group conversions.
Write: Disables theenhanced channel selection mode forEvent group andenables the
sequential channel selection mode.
Ah Read: Indicates thattheenhanced channel selection mode forEvent group isenabled.
Write: Enables theenhanced channel selection mode forEvent group.
Allother values Writing anyvalue other than 5horAhtothisfield hasnoeffect ontheselected channel
selection mode fortheEvent group, andtheADC module continues tousethechannel
selection mode thatwas previously programmed channel selection mode.
22.3.70 ADC Group1 Channel Selection Mode Control Register (ADG1CHNSELMODECTRL)
Figure 22-101 andTable 22-76 describe theADG1CHNSELMODECTRL register.
Figure 22-101. ADC Group1 Channel Selection Mode Control Register
(ADG1CHNSELMODECTRL) (offset =194h)
31 4 3 0
Reserved G1_ENH_CHNSEL_MODE_ENABLE
R-0 R/W-5h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-76. ADC Group1 Channel Selection Mode Control Register
(ADG1CHNSELMODECTRL) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 G1_ENH_CHNSEL_
MODE_ENABLEEnable enhanced channel selection mode forGroup1. Refer toSection 22.2.2.2.2 fora
description oftheenhanced channel selection mode.
5h Read: Indicates thattheenhanced channel selection mode forGroup1 isnotenabled. The
default sequential channel selection mode isused forGroup1 conversions.
Write: Disables theenhanced channel selection mode forGroup1 andenables the
sequential channel selection mode.
Ah Read: Indicates thattheenhanced channel selection mode forGroup1 isenabled.
Write: Enables theenhanced channel selection mode forGroup1.
Allother values Writing anyvalue other than 5horAhtothisfield hasnoeffect ontheselected channel
selection mode fortheGroup1, andtheADC module continues tousethechannel
selection mode thatwas previously programmed channel selection mode.

<!-- Page 949 -->

www.ti.com ADC Registers
949 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.71 ADC Group2 Channel Selection Mode Control Register (ADG2CHNSELMODECTRL)
Figure 22-102 andTable 22-77 describe theADG2CHNSELMODECTRL register.
Figure 22-102. ADC Group2 Channel Selection Mode Control Register
(ADG1CHNSELMODECTRL) (offset =198h)
31 4 3 0
Reserved G2_ENH_CHNSEL_MODE_ENABLE
R-0 R//W-5h
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-77. ADC Group2 Channel Selection Mode Control Register
(ADG2CHNSELMODECTRL) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 G2_ENH_CHNSEL_
MODE_ENABLEEnable enhanced channel selection mode forGroup2. Refer toSection 22.2.2.2.2 fora
description oftheenhanced channel selection mode.
5h Read: Indicates thattheenhanced channel selection mode forGroup2 isnotenabled. The
default sequential channel selection mode isused forGroup2 conversions.
Write: Disables theenhanced channel selection mode forGroup2 andenables the
sequential channel selection mode.
Ah Read: Indicates thattheenhanced channel selection mode forGroup2 isenabled.
Write: Enables theenhanced channel selection mode forGroup2.
Allother values Writing anyvalue other than 5horAhtothisfield hasnoeffect ontheselected channel
selection mode fortheGroup2, andtheADC module continues tousethechannel
selection mode thatwas previously programmed channel selection mode.

<!-- Page 950 -->

ADC Registers www.ti.com
950 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.72 ADC Event Group Current Count Register (ADEVCURRCOUNT)
Figure 22-103 andTable 22-78 describe theADEVCURRCOUNT register.
Figure 22-103. ADC Event Group Current Count Register (ADEVCURRCOUNT) (offset =19Ch)
31 5 4 0
Reserved EV_CURRENT_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-78. ADC Event Group Current Count Register (ADEVCURRCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 EV_CURRENT_
COUNTCURRENT_COUNT value fortheEvent group conversions when enhanced channel selection mode
isenabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
This register resets to0onanyofthefollowing conditions:
*Aperipheral reset occurs
*AnADC software reset occurs viatheADC Reset Control Register (ADRSTCR)
*EV_CURRENT_COUNT becomes equal toEV_MAX_COUNT
*Application writes zeros toADEVCURRCOUNT register
*Event group 'sresult RAM isreset
Aread from theADEVCURRCOUNT register returns thevalue ofthecurrent index intotheEvent
group 'slook-up table.
22.3.73 ADC Event Group Maximum Count Register (ADEVMAXCOUNT)
Figure 22-104 andTable 22-79 describe theADEVMAXCOUNT register.
Figure 22-104. ADC Event Group Maximum Count Register (ADEVMAXCOUNT) (offset =1A0h)
31 5 4 0
Reserved EV_MAX_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-79. ADC Event Group Maximum Count Register (ADEVMAXCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 EV_MAX_
COUNTMAX_COUNT value fortheEvent group conversions when enhanced channel selection mode is
enabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
Itisrecommended toclear theEvent group 'sCURRENT_COUNT register (ADEVCURRCOUNT)
whenever theEV_MAX_COUNT ischanged.

<!-- Page 951 -->

www.ti.com ADC Registers
951 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.74 ADC Group1 Current Count Register (ADG1CURRCOUNT)
Figure 22-105 andTable 22-80 describe theADG1CURRCOUNT register.
Figure 22-105. ADC Group1 Current Count Register (ADG1CURRCOUNT) (offset =1A4h)
31 5 4 0
Reserved G1_CURRENT_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-80. ADC Group1 Current Count Register (ADG1CURRCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 G1_CURRENT_
COUNTCURRENT_COUNT value fortheGroup1 conversions when enhanced channel selection mode is
enabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
This register resets to0onanyofthefollowing conditions:
*Aperipheral reset occurs
*AnADC software reset occurs viatheADC Reset Control Register (ADRSTCR)
*G1_CURRENT_COUNT becomes equal toG1_MAX_COUNT
*Application writes zeros toADG1CURRCOUNT register
*Group1 'sresult RAM isreset
Aread from theADG1CURRCOUNT register returns thevalue ofthecurrent index intothe
Group1 'slook-up table.
22.3.75 ADC Group1 Maximum Count Register (ADG1MAXCOUNT)
Figure 22-106 andTable 22-81 describe theADG1MAXCOUNT register.
Figure 22-106. ADC Group1 Maximum Count Register (ADG1MAXCOUNT) (offset =1A8h)
31 5 4 0
Reserved G1_MAX_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-81. ADC Group1 Maximum Count Register (ADG1MAXCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 G1_MAX_
COUNTMAX_COUNT value fortheGroup1 conversions when enhanced channel selection mode is
enabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
Itisrecommended toclear theGroup1 'sCURRENT_COUNT register (ADG1CURRCOUNT)
whenever theG1_MAX_COUNT ischanged.

<!-- Page 952 -->

ADC Registers www.ti.com
952 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedAnalog ToDigital Converter (ADC) Module22.3.76 ADC Group2 Current Count Register (ADG2CURRCOUNT)
Figure 22-107 andTable 22-82 describe theADG2CURRCOUNT register.
Figure 22-107. ADC Group2 Current Count Register (ADG2CURRCOUNT) (offset =1ACh)
31 5 4 0
Reserved G2_CURRENT_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-82. ADC Group2 Current Count Register (ADG2CURRCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 G2_CURRENT_
COUNTCURRENT_COUNT value fortheGroup2 conversions when enhanced channel selection mode is
enabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
This register resets to0onanyofthefollowing conditions:
*Aperipheral reset occurs
*AnADC software reset occurs viatheADC Reset Control Register (ADRSTCR)
*G2_CURRENT_COUNT becomes equal toG2_MAX_COUNT
*Application writes zeros toADG2CURRCOUNT register
*Group2 'sresult RAM isreset
Aread from theADG2CURRCOUNT register returns thevalue ofthecurrent index intothe
Group2 'slook-up table.
22.3.77 ADC Group2 Maximum Count Register (ADG2MAXCOUNT)
Figure 22-108 andTable 22-83 describe theADG2MAXCOUNT register.
Figure 22-108. ADC Group2 Maximum Count Register (ADG2MAXCOUNT) (offset =1B0h)
31 5 4 0
Reserved G2_MAX_COUNT
R-0 R/W-0
LEGEND: R/W =Read/Write; R=Read only; -n=value after reset
Table 22-83. ADC Group2 Maximum Count Register (ADG2MAXCOUNT) Field Descriptions
Bit Field Value Description
31-5 Reserved 0 Reads return 0.Writes have noeffect.
4-0 G2_MAX_
COUNTMAX_COUNT value fortheGroup2 conversions when enhanced channel selection mode is
enabled. Refer toSection 22.2.2.2.2 foradescription oftheenhanced channel selection mode.
Itisrecommended toclear theGroup2 'sCURRENT_COUNT register (ADG2CURRCOUNT)
whenever theG2_MAX_COUNT ischanged.