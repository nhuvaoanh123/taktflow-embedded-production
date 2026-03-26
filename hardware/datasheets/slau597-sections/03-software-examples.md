# Software Examples

> **Source**: `slau597-launchxl2-570lc43-user-guide.pdf` | Pages 20-27
>
> Auto-extracted via PyPDF2. Formatting is approximate.

---

<!-- Page 20 -->
Software Examples www.ti.com
20 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)3 Software Examples
Table 4lists thesoftware examples thatareincluded with theMSP-EXP432P401R LaunchPad
development kit.These examples canbedownloaded with theSimpleLink MSP432 SDK .
Table 4.Software Examples
Demo Name BoosterPack Required Description More Details
Out-of-Box Software Example N/AThe out-of-box demo programmed onthe
LaunchPad development kitfrom thefactory.Section 3.1
BOOSTXL-K350QVG-S1
Graphics Library ExampleBOOSTXL-K350SVG-S1Asimple example showing how touseMSP
Graphics Library (grlib) todisplay graphics
primitives andimages andimplement
touchscreen functionalitySection 3.2
430BOOST-SHARP96
Graphics Library Example430BOOST-SHARP96Asimple example showing how touseMSP
Graphics Library (grlib) todisplay graphics
primitives andimagesSection 3.3
BOOSTXL-
BATPAKMKII_FuelGauge_
MSP432P401RBOOSTXL-BATPAKMKIIDemonstrates how toinitialize bq27441-G1 fuel
gauge configurations andhow tocontrol and
read data registersSection 3.4
BOOSTXL-
SENSORS_SensorGUI_
MSP432P401RBOOSTXL-SENSORSDemonstrates how tosample data from thefive
onboard digital sensors andcommunicate that
over UART inaJSON payloadSection 3.5
BOOSTXL-SENSORS_TI-
RTOS_SensorGUI_
MSP432P401RBOOSTXL-SENSORSDemonstrates how tosample data from thefive
onboard digital sensors andcommunicate that
over UART inaJSON payload using TI-RTOSSection 3.6
Touseanyofthesoftware examples with theLaunchPad development kit,youmust have anintegrated
development environment (IDE) thatsupports theMSP432P401R device (see Table 5).
Table 5.IDEMinimum Requirements forMSP--EXP432P401R
Code Composer Studio ™IDE IAREmbedded Workbench ®IDE Keil®µVision ®MDK-ARM
v6.1 orlater v7.10 orlater v5orlater
Formore details onhow togetstarted quickly andwhere todownload thelatest TI,IAR, andKeilIDEs,
seeSection 4.
3.1 Out-of-Box Software Example
This section describes thefunctionality andstructure oftheout-of-box software thatispreloaded onthe
EVM. The source code canbefound intheSimpleLink MSP432 SDK .
The out-of-box software extends abasic blink LED example toallow users tocontrol theblink rate and
color ofanRGB LED ontheMSP432 LaunchPad development kit.
This software example iscreated towork with TI-RTOS orFreeRTOS, with aPOSIX layer tomake it
compatible with both kernels. There aremany advantages tousing aReal Time Operating System over
bare-metal code. AnRTOS manages many aspects ofthesystem, which allows adeveloper tofocus on
theapplication. Typically, anRTOS isused when theapplication needs todomore than afewsimple
actions.
This Out-of-Box example isintentionally designed tobemuch simpler than what would typically be
expected ofanRTOS project. The purpose ofthisdesign istoprovide theuser with avery simple and
straight-forward example ofhow touseanRTOS kernel. Using anRTOS kernel inthisexample requires
more overhead inmemory than bare-metal code, butitallows theuser more modularity andflexibility
when adding peripherals ormodules. This becomes particularly useful with more complex systems such
aswhen using connectivity devices. Formore information, gototheMSP432 SimpleLink Academy
training onTIResource Explorer.

---

<!-- Page 21 -->
www.ti.com Software Examples
21 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)3.1.1 Operation
Upon powering uptheout-of-box demo, theRGB LED2 blinks redat1Hz.Switch S1canbetapped
repeatedly ataconstant rate tosettheblink frequency ofLED2. Switch S2cycles LED2 through four
different color settings: Red, Green, Blue, andrandom RGB color. Each color setting retains itsown blink
frequency.
APCGUI accompanies theout-of-box demo toallow user tosetthecolor andblink rate oftheRGB LED.
Connect theLaunchPad development kittoacomputer using theincluded USB cable. The out-of-box GUI
canbeopened from within CCS using theTIResource Explorer: Software >SimpleLink MSP432 SDK >
Development Tools >MSP-EXP432P401R >Demos >outOfBox_msp432p401r, andlaunch theOutof
Box Experience GUI.
Figure 14.Out-of-Box GUIRunning Locally
The GUI canalso rundirectly from theTICloud Tools (see Section 4.1.2 ).

---

<!-- Page 22 -->
Software Examples www.ti.com
22 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 15.Out-of-Box GUIRunning From TICloud Tools
Click ontheConnect button toconnect totheLaunchPad development kitthen open theserial COM port.
Once theconnection hasbeen established andtheGUI indicates, "Target Status: Running ...,"youcan
usethecolor wheel ortheRed, Green, andBlue color sliders tosetthecolor oftheLaunchPad
development kitRGB LED. Changing theLED Beats PerMinute input boxsets theRGB LED blink rate.
3.2 BOOSTXL-K350QVG-S1 Graphics Library Example
This software isavailable intheSimpleLink MSP432 SDK (see Section 4.3).
The demo shows how tousetheMSP Graphics Library ,or"grlib, "inaproject with theKentec display.
This demo shows theuser how toenable thetouch screen, create buttons, andusegraphics primitives
including colors andimages.
The program begins bycalibrating thetouch screen. There isaroutine thatdetects thefour corner
coordinates todetermine ifaneligible rectangle boundary isformed. Ifthecalibration was incorrect, a
message displays onthescreen toindicate thatthecalibration failed. When successful, thecalibration
provides areference forallbutton presses throughout therestoftheprogram.
The next step istoselect themode oftheprogram: display primitives orimages. Each mode simply cycles
through without user interaction todemonstrate features ofthedisplay. Inthegraphics primitives mode,
thefollowing primitives areshown:
-Pixels
-Lines
-Circles
-Rectangles
-Text
The application isheavily commented toensure itisvery clear how tousethegrlib APIs. The above
primitives areshown aswell astheunderlying concepts ofgrlib including background andforeground
colors, context, fonts, opacity, andmore.

---

<!-- Page 23 -->
www.ti.com Software Examples
23 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)The images mode shows thedrawing ofafewdifferent images both compressed anduncompressed.
Image compression canhave abigimpact todrawing speeds forsimple images. Todraw images with the
MSP Graphics Library, they must firstbeconverted intotheright fileformat. These files canbegenerated
bytheImage Reformer toolthatcomes packaged with grlib. Launch thistoolfrom thegrlib folder or
directly from TIResource Explorer.
-FilePath: <grlib root>\utils\image-reformer\imagereformer.exe
The Image Reformer toolallows youtoimport images andoutput intogrlib specific files toaddtoyour
grlib project. Image Reformer does notmanipulate anyimages (such ascolor modifications, rotation, or
cropping), anyimage manipulation must bedone before importing intotheImage Reformer tool. More
information about MSP grlib andtheImage Reformer toolcanbefound inDesign Considerations When
Using theMSP430 Graphics Library .
Figure 16.Importing andConverting anImage With MSP Image Reformer

---

<!-- Page 24 -->
Software Examples www.ti.com
24 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)3.3 430BOOST-SHARP96 Graphics Library Example
This software example issimilar totheBOOSTXL-K350QVG-S1 Graphics library example. Itshows how
tousetheMSP Graphics Library ,or"grlib, "inaproject with theSharp 96×96display. The Sharp 96×96
display BoosterPack plug-in module does notsupport touch orcolor, itisasimple monochrome LCD. Itis
agreat LCD forultra-low power display applications andhasaunique mirrored pixel display.
This demo cycles screens without user interaction toshow simple graphics primitives.
-Pixels
-Lines
-Circles
-Rectangles
-Text
-Images
This demo introduces thefunctions toconfigure grlib such asinitialization, color inversion, andusing
foreground andbackground colors properly.
3.4 BOOSTXL-BATPAKMKII_FuelGauge_MSP432P401R
This section describes thefunctionality andstructure oftheBOOSTXL-
BATPAKMKII_FuelGauge_MSP432P401R demo thatisincluded intheSimpleLink MSP432 SDK (see
Section 4.3).
3.4.1 Source FileStructure
The project issplit intomultiple files (see Table 6).This makes iteasier tonavigate andreuse parts ofit
forother projects.
Table 6.Source FileandFolders
Name Description
Library: driverlib Device driver library (MSP432DRIVERLIB )
startup_msp432p401r.c MSP432 ™MCU family interrupt vector table forCGT
HAL_BQ27441.c Driver forcommunicating with thebq27441-G1 fuelgauge
HAL_I2C.c Board specific support driver forI2Ccommunication
HAL_UART.c Board specific driver forUART communication through Application/User UART
main.c The main function ofthedemo, global variables, andmore
3.4.2 Running theFuel Gauge Example
After thecompiling andloading theBOOSTXL-BATPAKMKII_FuelGauge_MSP432P401R project or
downloading theprebuilt firmware binary onto theMSP-EXP432P401R LaunchPad development kit,follow
thesteps below torunthedemo firmware.
Figure 17.Hardware Setup andConnections

---

<!-- Page 25 -->
www.ti.com Software Examples
25 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)1.Attach theBOOSTXL-BATPAKMKII Battery BoosterPack plug-in module totheLaunchPad
development kit.
2.Fliptheswitch tothe"ON"position ontheside oftheBOOSTXL-BATPAKMKII Battery BoosterPack
plug-in module.
3.Connect theMSP-EXP432P401R LaunchPad development kittoacomputer viamicro-USB cable.
4.Launch aserial terminal application andconnect totheCOM port for"XDS110 Class Application/User
UART "at115200 baud rate (see Figure 18andFigure 19).
Figure 18.Determine COM Port Number Using Device Manager onWindows
Figure 19.Example Serial Terminal Configuration
5.Press thereset button ontheMSP-EXP432P401R LaunchPad development kit.
6.Observe serial data displaying Fuel Gauge configuration andBattery Information (see Figure 20).

---

<!-- Page 26 -->
Software Examples www.ti.com
26 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 20.Snapshot ofSerial Terminal Connected toRunning Fuel Gauge Demo
3.4.3 Firmware Overview
See theQuick Start Guide forbq27441-G1 andthebq27441-G1 Technical Reference Manual fordetailed
description oftheBattery Fuel Gauge usage.
The demo program begins byinitializing anumber ofconfiguration parameters inthebq27441-G1 to
match thetarget battery. Four important parameters areDesign Capacity, Design Energy, Terminate
Voltage, andTaper Rate. Values aredetermined based onthetarget battery properties andbq27441-G1
documentation.
Next, thehost MSP432P401R MCU clears theBIE(Battery Insert Enable) bitinthefuelgauge operation
configuration register. When BIEiscleared, thebattery detection relies onthehost toissue a
BAT_INSERT subcommand toindicate battery presence, bypassing theJ6BINjumper ontheBOOSTXL-
BATPAKMKII BoosterPack plug-in module thatthefuelgauge relies onforbattery detection bydefault
when BIEisset(J6shorted =battery inserted; J6open =battery removed). This isdone toensure that
thedemo application works whether ornotJ6isconnected.
Inenduser applications, aswitch orthehost MCU ismore likely tocontrol theBINstate ofthefuelgauge
depending onbattery connection. However, thisisnotimplemented ontheBoosterPack plug-in module
andajumper isused tomanually toggle between battery insertion andremoval.
When thebq27441-G1 hasbeen configured properly, themain loop repeatedly reads back
DESIGN_CAPACITY, REMAINING_CAPACITY, STATE_OF_CHARGE, TEMPERATURE, VOLTAGE,
andAVERAGE_CURRENT from thefuelgauge. Results areformatted andtransmitted through
Application/User UART.

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
