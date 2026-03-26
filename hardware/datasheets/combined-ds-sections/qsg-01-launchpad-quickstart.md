# LAUNCHXL2-570LC43 Quick Start Guide

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 228-231 (4 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 228 -->
Meetithei
'erculesiiiTMSg&>LjIGx
LaunchPad
(evelopmentiKit
PartiNumberRiLOUNj'XLw@g&>LjIGTMFoosterPacki)cosystem
Seeithemialli*i ti.com/boosterpacksi 66
SoftwareiTools
'OLjoWeni
OipowerfuliWUIibasedicodei
generationitoolithatiallowsiusersi
toiconfigureiperipheralsiandiotheri
MjUiparametes.
)asilyiimporticodeiintoijjSiandi
otheriI()s.ProfessionaliSoftwareitools
LaunchPadiisialsoisupportedibyiprofessionali
I()sithatiprovideiindustrial@gradeifeaturesi
andifullidebug@capability.iSetibreakpointsyi
watchivariablesiXimoreiwithiLaunchPad.
w>fgiTexasiInstrumentsiIncorporated.iTheiplatformibaryi'erculesyiSafeTIiandijodeijomposeriStudioiareitrademarksiofiTexasiInstruments.i
Olliotheritrademarksiareitheipropertyiofitheirirespectiveiowners.
(isclaimerRiiwww.ti.com/lit/sszz>w&cwww.ti.com/ccs
jodeijomposeriStudioiiiiiI()TM
(RV1G>fiMotori(river
FoosterPack
@iSpiniOnyiThreeiPhaseiMotorN
@i,@wIViSupplyiInput
@if>Oijontinuous/fIOiPeak
www.ti.com/tool/halcogenSensori'ubiFoosterPack
InvenSenseiMPU@zfg>iz@axis
M)MSimotionisensor
@iG@axisigyroscope
@iG@axisiaccelerometer
@iG@axisicompass
*iFoschiSensorteciFMPf1>
pressureisensor
*iSensirioniS'TGwihumidtyiX
ambientitemperatureisensor
*iIntersiliISLwz>wGilightiXiIRi
SPI2CS
SPI2CS
SPI2CSGND
GPIO m5B
GPIO m5B
GPIO tt
RST
SPIMOSI
MISO
GPIO m5B
GPIO m5B
GPIO m5BPWMc1-1V
Analog2In
UARTRX2 m222222MCUB
TX2 m222222MCUB
GPIO m5B
Analog2In
SPI2CLK
GPIO m5B
IpCSCL
SDAc6V
Analog2In
Analog2In
Analog2In
Analog2In
Analog2In
Analog2InGND
Reserved
ReservedTimerTimer GPIO
GPIO
GPIO
GPIO
GPIO
GPIOGPIO
GPIOGPIO
GPIOPWM
PWMPWM
PWM
m5B
m5B
m5B
m5B
m5B
m5Bm5B
m5Bm5B
m5B
SPI2CS
SPI2CS
SPI2CSGND
GPIO m5B
GPIO m5B
GPIO tt
RST
SPIMOSI
MISO
GPIO m5B
GPIO m5B
GPIO m5BPWMc1-1V
Analog2In
UARTRX2 m222222MCUB
TX2 m222222MCUB
GPIO m5B
Analog2In
SPI2CLK
GPIO m5B
IpCSCL
SDAc6V
Analog2In
Analog2In
Analog2In
Analog2In
Analog2In
Analog2InGND
Reserved
ReservedTimerTimer GPIO
GPIO
GPIO
GPIO
GPIO
GPIOGPIO
GPIOGPIO
GPIOPWM
PWMPWM
PWM
m5B
m5B
m5B
m5B
m5B
m5Bm5B
m5Bm5B
m5BBoosterPack2standard LAUNCHXLpH6/0LC[12Pin2map LAUNCHXLpH6/0LC[12Pin2mapResources
ti.com/ launchpad {{jodeiexamples
OpeniSourcei(esigniJiles
(ocumentation
)xampleiprojects
Videos
Tutorials
OtheriTIiproductsFelowiareitheipinsiexposedi*itheiwxiFoosterPackiconnectorisites.i
Mappingitoitheiboosteripackistandardiisishown.iiInisomeicasesiaifunctionicanibeieitheriaiWIOiorianotherifunctiony
andimayibeilisteditwicei-coloricodediasibothiWIOiandiagainiasitheifunctionB.iiiiiOdditionalifunctionsibeyondithe
boosteripackistandardimayibeiavailable.iiJoridetailsiconsultitheilaunchpadischematic.iii
-NBii(enotesiI/Oipinsithatiareiinterrupt@capable.
BoosterPack2standard 2LAUNCHXLpH6/0LC[12Pin2map 2LAUNCHXLpH6/0LC[12Pin2map LAUNCHXLpH6/0LC[1
c1-1V
AD4[4]]7ADp[0]
SCI1TXSCI1RX
MIBSPI1CLK
NpHET4[[]
NpHET4[9]GIOA[/]m5B
AD4[4/]7ADp[4]
GIOA[]]m5BGND
AD4[0]
AD4[/]
AD4[4E]7ADp[p]
AD4[49]7ADp[1]
AD4[p0]7ADp[[]
AD4[p4]7ADp[6]
c1-1V
AD4[9]7ADp[9]
LIN4TXLIN4RX
MIBSPI4CLK
NpHET4[p[]
NpHET4[p]]ADpEVT
AD4[4]
NpHET4[14]m5Bc6V
GND
AD4[1]AD4[p]AD4[40]7ADp[40]
AD4[44]7ADp[44]
AD4[4p]7ADp[4p]AD4[[]
P60
P64
iGND
nRESET
MIBSPI4SIMO[0]
MIBSPI4SOMI[0]NpHETp[E] NpHET4[4]m5B
MIBSPI4NCS[0] MIBSPI4SOMI[4]
MIBSPI6SIMO[0]
MIBSPI6SOMI[0]MIBSPI4NCS[4] NpHET4[4/] m5B
MIBSPI4NCS[p] NpHET4[49] m5BNpHET4[40]m5B NpHET4[40]
GIOB[0]
AD4EVT
MIBSPI6CLKMIBSPI6NENA ECAP6NpHET4[pE] NpHET4[pE]m5B
NpHET4[E] NpHET4[E] m5B
NpHET4[p1] NpHET4[p1]m5B
NpHET4[44] NpHETp[4E]m5B
GIOB[4] m5B
m5BNpHET4[p]m5B EPWM1A
EPWM]A NpHET4[4E]m5B
NpHET4[10] NpHET4[10]m5B
NpHET4[4p] NpHET4[4p]m5B
GIOA[6] m5B
GIOA[p] m5B
GIOA[0] m5BGIOA[4] m5BNpHET4[4[]m5B NpHET4[4[]NpHET4[4]] NpHET4[4]]m5BGND
MIBSPI1SIMO
MIBSPI1SOMI
MIBSPI1NCS[p] NpHET4[p/] m5B
MIBSPI1NCS[1] NpHET4[p9] m5BNpHET4[pp] NpHET4[pp] m5B
MIBSPI1NCS[4] NpHET4[p6] m5B
GIOB[1] m5B
nRESET
GIOB[p] m5Bc6VJPw JPG
JPI JPgJfyJG JIyJw
JgyJ& J1yJ,
WheniusingisomeiFoosterPacksyiJPwyGyIyiorigimayineeditoibeiremoved.iiiJoriexampleyiusingithisiLaunchPadiwithiFoosterPackiFOOSTXL@(RV1G>fithatiincludesiai
G.GViregulator.iiToiavoidiconflictibetweenitheiLaunchPad2siG.GViregulatorianditheiregulatorionitheiFoosterPackyiiJPwi-oriJPIBishouldibeiremoved.

<!-- Page 229 -->
R5closer5look 5at5your5new5
$aunchPad5_evelopment5zit
™eatured5microcontrollerN5+ercules5T"Sb/1$)'xb/
This5$aunchPad5is5great5for666
25Starting5evaluation5with5+ercules5T"Sb/15")Us5designed5for5VSOG@G@G5and5V>)@8b1W5functional5safety5automotive5
and5transportation5applications6
25qetting5hands2on5experience5with5the5")Ufs5hardware5integrated5safety5and5diagnostic5features6
25qetting5familiar5with55SafeTV5software5and5development5tools5that5ease5development5of5functional5safety5applications65Out2of2box5_emo™ind5more5information5&5
ti6comElaunchxlG2b/1lc'x
What5comes5in5the5box,
$RU9)+X$G2b/1$)'x
$aunchPadQSG
This5Quick5
Start5quide
"icro2US(5
)able_ownload5Software5
ti6comElaunchxlG2b/1lc'x
Hercules
TM5570LC4357X_S8815)lass5_ebug5Probe
>nables5jTRq5programmingO5debugging5
g5application5URRT5via5US(6{{
25)omplementary5embedded5processing5and5analog5products5that55work5together5to5help5designer5meet5safety5standards
25Safety5development5process5certified5suitable5for5use5in5development5of5V>)@8b1W5and5VSOG@G@G5compliant5semiconductors
25Safety5related5documentationN5Safety Analysis Report, Safety Manual and Safety Report
25Safety5Tools5and5Software5.See5below-5
Software5library5of5functions5and5response5handlers5for5various5safety5features5of5the5+ercules5Safety5")Us6
_ownloadN5 ti6comEtoolEsafeti_diag_lib$RU9)+X$G2b/1$)'x5OverviewTMSafeTV5555_esign5Packages5
for5™unctional5Safety
™ind5more5information5&5
5555ti6comEsafeti
SafeTV5_iagnostic5$ibrary
SafeTV5)ompliance5Support5Packages
Hercules7MCU7ERE7Support7Forum:7
JJ5ti6comEhercules2support
Hercules7Training7Videos:
JJ5ti6comEherculestrainingSafeTV™5design5packages5help5designers5meet5industry5standard5functional5safety5requirements5while5managing5both5
systematic5and5random5failures65Using5SafeTV5components5helps5make5it5easier5for5designers5to5achieve5applicable5end2
product55certification5and5get5to5market5quickly5with5their5safety5critical5systems5which5are5pervasive5in5our5world5today6555555555
5555555555
SafeTV5)ompiler5Qualification5zit
Rssists5developers5in5qualifying5their5use5of5the5TV5RR"5)ompiler5to5functional5safety5standards5such5as5V>)5@8b1W5and5VSO5
G@G@G6
$earn5moreN5 ti6comEtoolEsafeti_cqkit
SafeTV5)ompliance5Support5Packages5for5+R$)oqen5and5SafeTV5_iagnostic5$ibrary5provide5the5necessary5documentationO5
reports5and5unit5test5capability5to5assist5developers5who5need5to5comply5with5functional5safety5standards5such5as5VSO5G@G@G5
and5V>)5@8b1W65R R
SP9U@8/
X_S8815)lass5_ebug5'12pin5BoosterPack7
plug2in5module5Connector7
.j82j'-
Optional5IbV5power5supply5jack
Rdditional5
")U5VEOs5for5
Prototyping55
.on5both5edges-5Potentiometer
55555555555.to5R_)5input-Optional5'12pin5(oosterPack5
plug2in5module5connector
.jb2jW-5Hercules7TMSb7ILCf-x7MCU
)urrent5$imit5$>_
On5indicates5>xternal5
power5is5necessaryUSB7Connector{{
User5$>_5G
User5$>_5xOS)5™ault5Vnjection5(uttonOptional5TV5)TV2G15jTRq5+eader5
for5use5with5external5debug5probes5
DP8-6-I7Ethernet7PHYTMSb7ILCf-b77Microcontroller
25Rutomotive5grade5")U
25x115"+z5$ockStep5RR"555)ortex555Rb™5)ached5)PU
25xGz5V4O5xGz5_45with5>))
25'"(5of5>mbedded5™lash5with5>))
25b8Gz(5of5>mbeeded5SRR"5with5>))
258GWz(5of5>mbedded5™lash5for5>>PRO"5with5>))
25(uilt2in5Self2Test5for5)PUs5and5on2chip5RR"s
25>rror5SignalingO5)lock5and5Voltage5"onitor25G5x5Programmable5+igh2>nd5Timer5.9G+>T-5"odules
25>nhanced5Timing5Peripherals5for5"otor5)ontrol
55555525/5x5ePW"O5@5x5e)RPO5G5x5eQ>P
25G5x58G2bit5Rnalog2to2_igital5)onverter5.R_)-
2581E8115"bps5>thernet5"R)5.>"R)-
25"ultiple5)ommunication5"odules55
55555525URRTES)VO$V9O5"ulti2buffered5SPVO5VG)
55555525'5x5)R95)ontroller5"odules5._)R9-
55555525™lexRay5)ontroller5wO76Optionalh7Installing7Code7Composer7Studio76CCSh
The5virtual5)O"5port5drivers5that5are5required5to5see5the5console5output5of5the5out5of5box5demo5are5bundled5with5))S65Vf5you5
wish5to5see5this5part5of5the5demoO5install5))S5v@61685or5later5before5connecting5the5$aunchPad5to5the5P)65
5
RO7Connecting7to7a7Computer7and7Powering7the7LaunchPad7
The5$aunchPad5is5configured5by5default5to5be5US(5powered5.jP@5must5be5shorted-O5which5can5be5done5by5connecting5the5
$aunchPad5to5a5computer5using5the5included5US(5cable65Vf5you5skipped5the5optional5step58O5ignore5any5errorEwarning5
messages5about5missing5drivers5during5this5step6
-O76Optionalh7Opening7a7Terminal7Program
Vf5you5completed5step585and5wish5to5see5the5console5output5of5the5demoO5this5is5the5time5to5configure5the5terminal5program5of5
your5choiceN
5555555J5Select5)O"5port5identified5as50X_S5)lass5RpplicationEUser5URRT05from5your5computerfs5device5manager6
5555555J5)onfigure5(aud5RateN58?G11O5_ata5(itsN5WO5Stop5(itsN5G55and5ParityN59one65
fO7Running7the7Out(of(box7Demo
This5$aunchPad5comes5pre2programmed5with5a5demo5set5that5highlights5several5of5")Ufs5safety5features65When5powered5the5
$aunchPad5will5start5blinking5US>R5$>_s6
Demo7wN5R5potentiometer5is5connected5to5the5")Ufs5Rnalog5Vnput5865$>_x5blinks5according5to5the5input5voltage5and5a5change5
in5the5potentiometer5setting5will5cause5it5to5blink5faster5or5slower65
Demo7RN5Pushing5button5zV$$5OS)5will5short5the5OS)5to5q9_5and5cause5an5Oscillator5™ault5in5the5")U65The5on2chip5monitor5
will5detect5and5trigger5an5error5signal5causing5the5>RR5$>_5to5light5up65Use5the5")U5Reset5push2button5to5bring5the5
$aunchPad5back5to5normal5operation6
Demo7-N5The5push2button5US>R5SWVT)+5(5will5inject5a5core5compare5error5.)PU5mismatch-65Rn5on2chip5monitor5will5detect5
the5fault5and5trigger5an5error5signal5causing5the5>RR5$>_5to5light5up6
Demo7fN5The5push2button5US>R5SWVT)+5R5will5inject5a5single5bit5error5in5the5")Ufs5flash5on5every5push65>))5logic5corrects5
single5bit5errors5in5flash5and5counts5them65$>_G5toggles5with5every5injection5of5the5single5bit5error65
When5you5are5ready5to5take5the5next5stepO5complete5Project5165qo5to5 www6ti6comElaunchpad 5and5click5on5Project515link5for5
+ercules5T"Sb/1$)'xx5$aunchPad65™ind5other5projects5on5wikiN5 processors6wiki6ti6comEindex6phpE$RU9)+X$G2b/1$)'x

<!-- Page 230 -->
IMPORTANT NOTICE
Texas Instruments Incorporated anditssubsidiaries (TI)reserve theright tomake corrections, enhancements, improvements andother
changes toitssemiconductor products andservices perJESD46, latest issue, andtodiscontinue anyproduct orservice perJESD48, latest
issue. Buyers should obtain thelatest relevant information before placing orders andshould verify thatsuch information iscurrent and
complete. Allsemiconductor products (also referred toherein as"components ")aresold subject toTI'sterms andconditions ofsale
supplied atthetime oforder acknowledgment.
TIwarrants performance ofitscomponents tothespecifications applicable atthetime ofsale, inaccordance with thewarranty inTI'sterms
andconditions ofsale ofsemiconductor products. Testing andother quality control techniques areused totheextent TIdeems necessary
tosupport thiswarranty. Except where mandated byapplicable law, testing ofallparameters ofeach component isnotnecessarily
performed.
TIassumes noliability forapplications assistance orthedesign ofBuyers 'products. Buyers areresponsible fortheir products and
applications using TIcomponents. Tominimize therisks associated with Buyers 'products andapplications, Buyers should provide
adequate design andoperating safeguards.
TIdoes notwarrant orrepresent thatanylicense, either express orimplied, isgranted under anypatent right, copyright, mask work right, or
other intellectual property right relating toanycombination, machine, orprocess inwhich TIcomponents orservices areused. Information
published byTIregarding third-party products orservices does notconstitute alicense tousesuch products orservices orawarranty or
endorsement thereof. Use ofsuch information may require alicense from athird party under thepatents orother intellectual property ofthe
third party, oralicense from TIunder thepatents orother intellectual property ofTI.
Reproduction ofsignificant portions ofTIinformation inTIdata books ordata sheets ispermissible only ifreproduction iswithout alteration
andisaccompanied byallassociated warranties, conditions, limitations, andnotices. TIisnotresponsible orliable forsuch altered
documentation. Information ofthird parties may besubject toadditional restrictions.
Resale ofTIcomponents orservices with statements different from orbeyond theparameters stated byTIforthatcomponent orservice
voids allexpress andanyimplied warranties fortheassociated TIcomponent orservice andisanunfair anddeceptive business practice.
TIisnotresponsible orliable foranysuch statements.
Buyer acknowledges andagrees thatitissolely responsible forcompliance with alllegal, regulatory andsafety-related requirements
concerning itsproducts, andanyuseofTIcomponents initsapplications, notwithstanding anyapplications-related information orsupport
thatmay beprovided byTI.Buyer represents andagrees thatithasallthenecessary expertise tocreate andimplement safeguards which
anticipate dangerous consequences offailures, monitor failures andtheir consequences, lessen thelikelihood offailures thatmight cause
harm andtake appropriate remedial actions. Buyer willfully indemnify TIanditsrepresentatives against anydamages arising outoftheuse
ofanyTIcomponents insafety-critical applications.
Insome cases, TIcomponents may bepromoted specifically tofacilitate safety-related applications. With such components, TI'sgoal isto
help enable customers todesign andcreate their own end-product solutions thatmeet applicable functional safety standards and
requirements. Nonetheless, such components aresubject tothese terms.
NoTIcomponents areauthorized foruseinFDA Class III(orsimilar life-critical medical equipment) unless authorized officers oftheparties
have executed aspecial agreement specifically governing such use.
Only those TIcomponents which TIhasspecifically designated asmilitary grade or"enhanced plastic "aredesigned andintended forusein
military/aerospace applications orenvironments. Buyer acknowledges andagrees thatanymilitary oraerospace useofTIcomponents
which have notbeen sodesignated issolely attheBuyer 'srisk, andthatBuyer issolely responsible forcompliance with alllegal and
regulatory requirements inconnection with such use.
TIhasspecifically designated certain components asmeeting ISO/TS16949 requirements, mainly forautomotive use. Inanycase ofuseof
non-designated products, TIwillnotberesponsible foranyfailure tomeet ISO/TS16949.
Products Applications
Audio www.ti.com/audio Automotive andTransportation www.ti.com/automotive
Amplifiers amplifier.ti.com Communications andTelecom www.ti.com/communications
Data Converters dataconverter.ti.com Computers andPeripherals www.ti.com/computers
DLP®Products www.dlp.com Consumer Electronics www.ti.com/consumer-apps
DSP dsp.ti.com Energy andLighting www.ti.com/energy
Clocks andTimers www.ti.com/clocks Industrial www.ti.com/industrial
Interface interface.ti.com Medical www.ti.com/medical
Logic logic.ti.com Security www.ti.com/security
Power Mgmt power.ti.com Space, Avionics andDefense www.ti.com/space-avionics-defense
Microcontrollers microcontroller.ti.com Video andImaging www.ti.com/video
RFID www.ti-rfid.com
OMAP Applications Processors www.ti.com/omap TIE2ECommunity e2e.ti.com
Wireless Connectivity www.ti.com/wirelessconnectivity
Mailing Address: Texas Instruments, Post Office Box 655303, Dallas, Texas 75265
Copyright ©2015, Texas Instruments Incorporated

<!-- Page 231 -->
TMS570LC43x 16/32-Bit RISC Flash
Microcontroller
Technical Reference Manual
Literature Number: SPNU563
May 2014