# Digital Windowed Watchdog

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 138-138 (1 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 138 -->
RESETDown
Counter
Down CounterDown CounterDWWD PreloadDWWD Preload
=0
Digital
Windowed
Watch
DogDigital
Windowed
Watch
Dog100%
WindowWindow OpenWindow Open Window Open Window Open
50%
Window
25%
Window
12.5%
Window
6.25%
Window
3.125%
WindowINTERRUPT
ESMDigital
Windowed
Watch
DogWindow Open Window Open
Window Open Window Open
W Open W Open
Op Op
O O
O O
138TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.21 Digital Windowed Watchdog
This device includes aDigital Windowed Watchdog (DWWD) module that protects against runaway code
execution (see Figure 6-22).
The DWWD module allows theapplication toconfigure thetime window within which theDWWD module
expects theapplication toservice thewatchdog. Awatchdog violation occurs iftheapplication services the
watchdog outside ofthiswindow, orfails toservice thewatchdog atall.The application can choose to
generate asystem reset oranonmaskable interrupt totheCPU incase ofawatchdog violation.
The watchdog isdisabled bydefault andmust beenabled bytheapplication. Once enabled, thewatchdog
canonly bedisabled upon asystem reset.
Figure 6-22. Digital Windowed Watchdog Example