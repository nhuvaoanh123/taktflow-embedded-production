# FAQ

> **Source**: `slau597-launchxl2-570lc43-user-guide.pdf` | Pages 31-35
>
> Auto-extracted via PyPDF2. Formatting is approximate.

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

---

<!-- Page 32 -->
FAQ www.ti.com
32 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 23.Target Configurations
Ifusing theonboard debug probe, XDS110-ET isshown.
-Right click Launch Selected Configuration .
Figure 24.Launch Selected Configuration
-The debug probe now connects tothedevice (which isstillpossible) butdoes nottrytohaltthe
CPU, write toregisters, oreven download code (which would notbepossible). The Debug view that
isspawned shows theCPU core butmarks itasdisconnected.
-Right click Show allcores .

---

<!-- Page 33 -->
www.ti.com FAQ
33 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 25.Show AllCores
The MSP432 Debug Access Port, orDAP, isshown under Non Debuggable Devices .
-Right click Connect Target

---

<!-- Page 34 -->
FAQ www.ti.com
34 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Figure 26.Connect Target
Now runascript toreturn thedevice back tofactory settings:
Scripts >default >MSP432_Factory_Reset
Figure 27.MSP432_Factory_Reset Script
-These instructions aregenerally thesame forallIDEs, buttheexact steps may vary slightly byIDE.
See thefollowing IDEuser'sguides foradditional details:
-Code Composer Studio ™IDE7.1+ forSimpleLink ™MSP432 ™Microcontrollers User 'sGuide
-ARM ®Keil®MDK Version 5forSimpleLink ™MSP432 ™Microcontrollers User 'sGuide
-IAREmbedded Workbench forARM 7.xforSimpleLink ™MSP432 ™Microcontrollers User 'sGuide
Q:How doIusetheLaunchPad development kitandmySegger J-Link todebug thetarget externally? It
won'tconnect totheonboard connector.
A:The Segger J-Link does notcome with anadapter forthe10-pin small-pitch ARM connector. The
adapter cable isavailable from SEGGER orfrom Digi-Key .

---

<!-- Page 35 -->
www.ti.com FAQ
35 SLAU597C -March 2015 -Revised March 2017
Submit Documentation Feedback
Copyright ©2015 -2017, Texas Instruments IncorporatedMSP432P401R SimpleLink ™Microcontroller LaunchPad ™Development Kit
(MSP-EXP432P401R)Q:Problems plugging theMSP432 LaunchPad development kitintoaUSB3.0 port.
A:Ithasbeen observed thatwhen theMSP432 LaunchPad development kitisconnected toUSB3.0 ports
provided byacertain combination ofUSB3.0 host controller hardware andassociated device drivers that
theIDEisunable toestablish adebug session with theLaunchPad development kit,resulting inanerror
message like"CS_DAP_0: Error connecting tothetarget: (Error -260 @0x0) Anattempt toconnect tothe
XDS110 failed. "inthecase ofCode Composer Studio IDE. Inthiscase theCCS-provided low-level
command lineutility 'xdsdfu 'willalso notbeable toestablish aconnection with theLaunchPad
development kit.
Specifically, thisissue was observed onPCs running Windows 7thatshow the"Renesas Electronics USB
3.0Host Controller "andtheassociated "Renesas Electronics USB 3.0Root Hub"inthedevice manager.
After updating theassociated Windows USB drivers tomore recent versions obtained from thehardware
vendor theissue went away. There might beother USB3.0 hardware anddevice driver combinations that
willlead tothesame issue. Ifyouthink youmight beaffected, trytocontact your PCvendor ortryto
locate andinstall more recent versions oftheUSB3.0 device drivers. Alternatively, connect theLaunchPad
development kittoanUSB2.0 port onyour PC,ifoneisavailable.
Q:Ican'tgetthebackchannel UART toconnect. What 'swrong?
A:Check thefollowing:
-Dothebaud rate inthehost'sterminal application andtheeUSCI settings match?
-Aretheappropriate jumpers inplace ontheisolation jumper block?
-Probe onRXD andsend data from thehost. Ifyoudonotseedata, itmight beaproblem onthehost
side.
-Probe onTXD while sending data from theMSP432. Ifyoudonotseedata, itmight beaconfiguration
problem with theeUSCI module.
-Consider theuseofthehardware flow control lines (especially forhigher baud rates).
