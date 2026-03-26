# Getting Started

> **Source**: `slau597-launchxl2-570lc43-user-guide.pdf` | Pages 3-4
>
> Auto-extracted via PyPDF2. Formatting is approximate.

---

<!-- Page 3 -->
www.ti.com Getting Started
3 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)6 Source FileandFolders ................................................................................................... 24
7 Source Files andFolders .................................................................................................. 27
8 How MSP Device Documentation isOrganized ........................................................................ 30
Trademarks
SimpleLink, LaunchPad, MSP432, BoosterPack, Code Composer Studio, EnergyTrace, E2E are
trademarks ofTexas Instruments.
ARM, Cortex, Keil, µVision areregistered trademarks ofARM Ltd.
Bluetooth isaregistered trademark ofBluetooth SIG.
IAREmbedded Workbench isatrademark ofIARSystems.
Wi-Fi isaregistered trademark ofWi-Fi Alliance.
Allother trademarks aretheproperty oftheir respective owners.
1 Getting Started
1.1 Introduction
The SimpleLink MSP-EXP432P401R LaunchPad development kitisaneasy-to-use evaluation module for
theMSP432P401R microcontroller .Itcontains everything needed tostart developing ontheMSP432 Low-
Power +Performance ARM 32-bit Cortex-M4F microcontroller (MCU), including onboard debug probe for
programming, debugging, andenergy measurements. The MSP432P401R microcontroller supports low-
power applications thatrequire increased CPU speed, memory, analog, and32-bit performance.
Rapid prototyping issimplified byaccess tothe40-pin headers andawide variety ofBoosterPack ™plug-
inmodules thatenable technologies such aswireless connectivity, graphical displays, environmental
sensing, andmany more. Free software development tools arealso available such asTI'sEclipse-based
Code Composer Studio ™IDE, IAREmbedded Workbench ™IDE, andKeil ®µVision ®IDE. Code
Composer Studio IDEsupports EnergyTrace ™technology when paired with theMSP432P401R
LaunchPad development kit.More information about theLaunchPad development kit,thesupported
BoosterPack plug-in modules, andtheavailable resources canbefound atTI'sLaunchPad development
kitportal .Togetstarted quickly andfindavailable resources intheSimpleLink MSP432 software
development kit(SDK), visit theTICloud Development Environment .

---

<!-- Page 4 -->
Getting Started www.ti.com
4 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)1.2 KeyFeatures
-Low-power ARM Cortex-M4F MSP432P401R
-40-pin LaunchPad development kitstandard thatleverages theBoosterPack plug-in module ecosystem
-XDS110-ET, anopen-source onboard debug probe featuring EnergyTrace+ technology andapplication
UART
-Two buttons andtwoLEDs foruser interaction
-Backchannel UART through USB toPC
1.3 What 'sIncluded
1.3.1 KitContents
-One MSP-EXP432P401R LaunchPad development kit
-One Micro USB cable
-One Quick Start Guide
1.3.2 Software Examples (Section 3)
-Out-of-Box Software Example
-BOOSTXL-K350QVG-S1 Graphics Library Example
-430BOOST-SHARP96 Graphics Library Example
-BOOSTXL-BATPAKMKII Fuel Gauge Example
-BOOSTXL-SENSORS Sensor GUI Example
-BOOSTXL-SENSORS Sensor GUI with TI-RTOS Example
1.4 First Steps: Out-of-Box Experience
Aneasy way togetfamiliar with theEVM isbyusing itspreprogrammed out-of-box code. Itdemonstrates
some keyfeatures oftheLaunchPad development kitfrom auser level, showing how tousethe
pushbutton switches together with onboard LEDs andbasic serial communication with acomputer.
Foramore detailed explanation oftheout-of-box demo, seeSection 3.
1.5 Next Steps: Looking IntotheProvided Code
Itisnow time tostart exploring more features oftheEVM!
www.ti.com/beginMSP432launchpad
Togetstarted, youwillneed anintegrated development environment (IDE) toexplore andstart editing the
code examples. See Section 4formore information onIDEs andwhere todownload them.
The out-of-box source code andmore code examples canbedownloaded from theMSP-EXP432P401R
toolfolder .Find what code examples areavailable andmore details about each example inSection 3.All
code islicensed under BSD, andTIencourages reuse andmodifications tofitspecific needs.
