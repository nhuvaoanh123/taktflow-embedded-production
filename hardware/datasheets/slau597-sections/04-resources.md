# Resources

> **Source**: `slau597-launchxl2-570lc43-user-guide.pdf` | Pages 27-31
>
> Auto-extracted via PyPDF2. Formatting is approximate.

---

<!-- Page 27 -->
www.ti.com Software Examples
27 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)3.5 BOOSTXL-SENSORS_SensorGUI_MSP432P401R
The Sensors BoosterPack kit(BOOSTXL-SENSORS) isaneasy-to-use plug-in module foradding digital
sensors toyour LaunchPad development kitdesign. The SimpleLink MCU LaunchPad development kit
allows developers tousethisBoosterPack plug-in module tostart developing sensor applications using
theonboard gyroscope, accelerometer, magnetometer, pressure, ambient temperature, humidity, ambient
light, andinfrared temperature sensors. Forinformation ontheOut-of-Box experience andhow tousethe
BOOSTXL-SENSORS BoosterPack plug-in module, seetheBOOSTXL-SENSORS BoosterPack ™Plug-in
Module User 'sGuide .
3.6 BOOSTXL-SENSORS_TI-RTOS_SensorGUI_MSP432P401R
This section describes thefunctionality structure oftheBOOSTXL-
SENSORS_TI_RTOS_SensorGUI_MSP432P401R demo thatisincluded intheSimpleLink MSP432 SDK
(see Section 4.3).
This example requires TI-RTOS MSP43x version 2_16_01_14 tobeinstalled inCCS.
More information ontheuseofTI-RTOS canbefound intheTI-RTOS user'sguides, available intheTI-
RTOS toolfolder .
3.6.1 Source FileStructure
Table 7lists thesource files andfolders.
Table 7.Source Files andFolders
Name Description
OS: TI-RTOS Real-time operating system using TI-RTOS kernel
Library: driverlib Device driver library (MSP432DRIVERLIB )
bme280.c Driver forcommunicating with theenvironmental sensor
bme280_support.c Support driver forcommunicating with theenvironmental sensor
bmi160.c Driver forcommunicating with theIMU andmagnetometer sensors
bmi160_support.c Support driver forcommunicating with theIMU andmagnetometer sensors
MSP_EXP432P401R.c Driver forsetting upboard specific items (forexample, I2CandUART)
main.c The demo 'smain function, tasks, semaphores, global variables, andmore
opt3001.c Driver forcommunicating with theambient light sensor
tmp007.c Driver forcommunicating with theinfrared temperature sensor
3.6.2 Working With theGUI
Collaboration with theSensor GUI isidentical to,except forprogramming thedevice directly from the
GUI. The .outfilelocated within theGUI isspecific totheBOOSTXL-
SENSORS_SensorGUI_MSP432P401R example project. Todownload theprogram, youmust usea
separate IDE, such asCCS orIAR, andtheBOOSTXL-SENSORS_TI_RTOS_SensorGUI_MSP432P401R
source code intheSimpleLink MSP432 SDK .
4 Resources
4.1 Integrated Development Environments
Although thesource files canbeviewed with anytexteditor, more canbedone with theprojects ifthey're
opened with adevelopment environment likeCode Composer Studio (CCS) IDE, KeilµVision, IAR
Embedded Workbench, orEnergia.

---

<!-- Page 28 -->
Resources www.ti.com
28 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)4.1.1 SimpleLink MSP432 SDK
The MSP432P401R device ispart oftheSimpleLink microcontroller (MCU) platform, which consists ofWi-
Fi®,Bluetooth ®lowenergy, Sub-1 GHz, andhost MCUs. Allshare acommon, easy-to-use development
environment with asingle core software development kit(SDK) andrichtoolset.Aone-time integration of
theSimpleLink platform letsyouaddanycombination ofdevices from theportfolio intoyour design. The
ultimate goal oftheSimpleLink platform istoachieve 100percent code reuse when your design
requirements change. Formore information, visit www.ti.com/simplelink .
4.1.2 TICloud Development Tools
TI'sCloud-based software development tools provide instant access toSimpleLink SDK content anda
web-based IDE.
4.1.2.1 TIResource Explorer Cloud
TIResource Explorer Cloud provides aweb interface forbrowsing examples, libraries anddocumentation
found inSimpleLink SDK without having todownload files toyour local drive.
TrytheTIResource Explorer Cloud now atdev.ti.com .
4.1.2.2 Code Composer Studio ™Cloud IDE
Code Composer Studio Cloud isaweb-based IDEthatallows code edit, compile anddownload todevices
right from your web browser. Italso integrates seamlessly with TIResource Explorer Cloud with theability
toimport projects directly onthecloud.
Afullcomparison between Code Composer Studio Cloud andCode Composer Studio Desktop isavailable
here.
See Code Composer Studio Cloud now atdev.ti.com .
4.1.3 Code Composer Studio ™Desktop IDE
Code Composer Studio Desktop isaprofessional integrated development environment thatsupports TI's
Microcontroller andEmbedded Processors portfolio. Code Composer Studio comprises asuite oftools
used todevelop anddebug embedded applications. Itincludes anoptimizing C/C++ compiler, source code
editor, project build environment, debugger, profiler, andmany other features.
NOTE: The MSP432 LaunchPad development kitrequires CCS Version 6.1.0 orlater. See the
Code Composer Studio ™IDE7.1+ forSimpleLink ™MSP432 ™Microcontrollers User 's
Guide fordetailed instructions ofusing theIDEwith MSP432. TousetheSimpleLink
MSP432 SDK, CCS Version 7.1.0 orlater isrequired.
Learn more about CCS anddownload itatwww.ti.com/tool/ccstudio .
4.1.4 Keil®µVision®IDE
The µVision IDEisanembedded project development environment included inKeil'sMicrocontroller
Development KitVersion 5,thatprovides ansource code editor, project manager, andmake utility tool.
µVision supports alltheKeiltools including C/C++ Compiler, Macro Assembler, Linker, Library Manager,
andObject-HEX Converter.
NOTE: The MSP432 LaunchPad development kitrequires µVision IDE/MDK Version 5orlater. See
theARM®Keil®MDK Version 5forSimpleLink ™MSP432 ™Microcontrollers User 'sGuide for
detailed instructions ofusing theIDEwith MSP432.
Learn more about KeilµVision anddownload itatwww.keil.com/arm/mdk.asp .

---

<!-- Page 29 -->
www.ti.com Resources
29 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)4.1.5 IAREmbedded Workbench®forARM IDE
IAREmbedded Workbench forARM IDEisanother very powerful integrated development environment
thatallows youtodevelop andmanage complete embedded application projects. Itintegrates theIAR
C/C++ Compiler, IARAssembler, IARILINK Linker, editor, project manager, command linebuild utility,
andIARC-SPY Debugger.
NOTE: The MSP432 LaunchPad development kitrequires theIAREmbedded Workbench forARM
IDEVersion 7.10 orlater. See theIAREmbedded Workbench forARM 7.xforSimpleLink ™
MSP432 ™Microcontrollers User 'sGuide fordetailed instructions ofusing theIDEwith
MSP432. TousetheSimpleLink MSP432 SDK, IARVersion 7.80.3 orlater isrequired.
Learn more about IAREmbedded Workbench anddownload itathttps://www.iar.com/iar-embedded-
workbench/arm .
4.1.6 Energia
Energia isasimple open-source community-driven code editor thatisbased ontheWiring andArduino
framework. Energia provides unmatched ease ofusethrough very-high-level APIs thatcanbeused
across hardware platforms. Energia isalight-weight IDEthatdoes nothave thefullfeature setofCCS,
Keil, orIAR. However, Energia isgreat foranyone who wants togetstarted very quickly orwho does not
have significant coding experience.
Learn more about Energia anddownload itatenergia.nu .
4.2 LaunchPad Development KitWebsites
More information about theLaunchPad development kit,supported BoosterPack plug-in modules, and
available resources canbefound at:
-MSP-EXP432P401R toolfolder :Resources specific tothisparticular LaunchPad development kit
-TI'sLaunchPad development kitportal :Information about allLaunchPad development kitsfrom TI
4.3 SimpleLink SDK andTIResource Explorer
TIResource explorer isatoolintegrated intoCCS thatallows youtobrowse through available design
resources. TIResource Explorer willhelp youquickly findwhat youneed inside packages including
SimpleLink SDK fortheSimpleLink MCUs such asMSP432, CC3200, CC2640, andmore. TIResource
Explorer iswell organized tofindeverything thatyouneed quickly, andyoucanimport software projects
intoyour workspace, finddocumentation, andbrowse libraries inyour workspace injustafewclicks.
TIResource Explorer Cloud isoneoftheTICloud Development tools andistightly integrated with CCS
Cloud. See Section 4.1.2 formore information.
The SimpleLink SDK isacollection ofcode examples, software libraries, data sheets, andother design
resources forallSimpleLink MCU devices delivered inaconvenient package -essentially everything
developers need tobecome SimpleLink experts.
The SimpleLink MCU portfolio offers asingle development environment thatdelivers flexible hardware,
software, andtooloptions forcustomers developing wired andwireless applications. With anultimate goal
of100percent code reuse across host MCUs, Wi-Fi, Bluetooth lowenergy, Sub-1 GHz devices, and
more, choose theMCU orconnectivity standard thatfitsyour design. Aone-time investment with the
SimpleLink software development kit(SDK) letsyoureuse often, opening thedoor tocreate unlimited
applications. Formore information, visit ww.ti.com/simplelink .

---

<!-- Page 30 -->
Resources www.ti.com
30 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 21.Using TIResource Explorer toBrowse MSP-EXP432P401R inSimpleLink SDK
Inside TIResource Explorer, these examples andmany more canbefound andeasily imported intoCCS
with oneclick.
4.4 MSP432P401R
4.4.1 Device Documentation
Atsome point, youwillprobably want more information about theMSP432P401R device. Forevery MSP
device, thedocumentation isorganized asshown inTable 8.
Table 8.How MSP Device Documentation isOrganized
Document ForMSP432P401R Description
Device family user'sguideMSP432P4xx SimpleLink ™
Microcontrollers Technical
Reference ManualArchitectural information about thedevice, including allmodules
andperipherals such asclocks, timers, ADC, andsoon
Device-specific data sheetMSP432P401xx SimpleLink ™
Mixed-Signal MicrocontrollersDevice-specific information andallparametric information forthis
device
4.4.2 MSP432P401R Code Examples
Inside oftheSimpleLink MSP432 SDK, asetofvery simple MSP432P4xx code examples canbefound
thatdemonstrate how tousetheentire setofMSP432 peripheral: serial communication, ADC14, Timer_A,
Timer_B, andsoon.These examples show both thedirect register access anddriver library methods.
Every MSP derivative hasasetofthese code examples. When starting anew project oradding anew
peripheral, these examples serve asagreat starting point (see Section 4.3).
4.4.3 MSP432 Application Notes andTIDesigns
There aremany application notes thatcanbefound atwww.ti.com/msp432 ,aswell asTIDesigns with
practical design examples andtopics.

---

<!-- Page 31 -->
www.ti.com Resources
31 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)4.5 Community Resources
4.5.1 TIE2E™Community
Search theE2E forums ate2e.ti.com .Ifyoucannot findyour answer, post your question tothe
community.
4.5.2 Community atLarge
Many online communities focus ontheLaunchPad development kit;forexample, www.43oh.com .You can
findadditional tools, resources, andsupport from these communities.
5 FAQ
Q:Ican'tprogram myLaunchPad development kit;theIDEcan'tconnect totarget. What 'swrong?
A:Check thefollowing:
-AretheJTAG jumpers onJ101 populated (GND, RST, TMS, TCK, TDO, TDI)?
-Check forpower tothetarget
-Arethe3V3 andGND jumpers onJ101 populated andUSB cable plugged in?
-Ifusing anexternal debug probe, isUSB power provided asshown above? Otherwise isexternal
power provided tothetarget?
-Check thedebug probe settings: change toSerial Wire Debug (SWD) without SWO.
-Under targetconfigs, double-click the*.ccxml file.
-Click theAdvanced tabatthebottom.
-Click onTexas Instruments XDS110 USB Debug Probe.
-Under Connection Properties, change SWD Mode Settings toUse SWD Mode with SWO Trace
Disabled.
Figure 22.SWD Mode Settings
-When thesettings ofPort J(PJSEL0 andPJSEL1 bits) arechanged, fullJTAG access isprevented
onthese pins. Changing touseSWD allows access through thededicated debug pins only.
-Ifeven thiscannot connect, reset thedevice tofactory settings:
-Click View→Target Configurations .CCS shows thetarget configuration.
