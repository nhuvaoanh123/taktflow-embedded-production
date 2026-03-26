# Architecture - Peripheral Central Resource (PCR) Registers

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 217-251

---


<!-- Page 217 -->

www.ti.com System andPeripheral Control Registers
217 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3 Peripheral Central Resource (PCR) Control Registers
This section describes thePeripheral Central Resource (PCR) control registers. The arethree PCRx in
thismicrocontroller. The start address ofthePCR1 register frame isFFFF 1000h. The start address ofthe
PCR2 register frame isFCFF 1000h. The start address ofthePCR3 register frame isFFF7 8000h.
Table 2-85 lists theregisters inthePCR, which areused toconfigure thefollowing main functionalities:
*Protection control totheperipherals inPCS (Peripheral Memory) andPS(Peripheral) regions.
*Powerdown control totheperipherals inPCS (Peripheral Memory) andPS(Peripheral) regions.
*Powerdown control totheCoreSight debug peripherals indebug frame region from FFA0 0000h to
FFAF FFFFh.
*Master-ID Filtering control totheperipherals inPS(Peripheral), PPS (Privileged Peripheral) ,PPSE
(Privileged Peripheral Extended) regions.
*Master-ID Filtering control totheperipheral memories inPCS (Peripheral Memory), andPPCS
(Privileged Peripheral Memory) regions.
The following sections provide detailed information about these registers. Notallchip selects exist onthis
device.
Table 2-85. Peripheral Central Resource Control Registers
Offset Acronym Register Description Section
00h PMPROTSET0 Peripheral Memory Protection SetRegister 0 Section 2.5.3.1
04h PMPROTSET1 Peripheral Memory Protection SetRegister 1 Section 2.5.3.2
10h PMPROTCLR0 Peripheral Memory Protection Clear Register 0 Section 2.5.3.3
14h PMPROTCLR1 Peripheral Memory Protection Clear Register 1 Section 2.5.3.4
20h PPROTSET0 Peripheral Protection SetRegister 0 Section 2.5.3.5
24h PPROTSET1 Peripheral Protection SetRegister 1 Section 2.5.3.6
28h PPROTSET2 Peripheral Protection SetRegister 2 Section 2.5.3.7
2Ch PPROTSET3 Peripheral Protection SetRegister 3 Section 2.5.3.8
40h PPROTCLR0 Peripheral Protection Clear Register 0 Section 2.5.3.9
44h PPROTCLR1 Peripheral Protection Clear Register 1 Section 2.5.3.10
48h PPROTCLR2 Peripheral Protection Clear Register 2 Section 2.5.3.11
4Ch PPROTCLR3 Peripheral Protection Clear Register 3 Section 2.5.3.12
60h PCSPWRDWNSET0 Peripheral Memory Power-Down SetRegister 0 Section 2.5.3.13
64h PCSPWRDWNSET1 Peripheral Memory Power-Down SetRegister 1 Section 2.5.3.14
70h PCSPWRDWNCLR0 Peripheral Memory Power-Down Clear Register 0 Section 2.5.3.15
74h PCSPWRDWNCLR1 Peripheral Memory Power-Down Clear Register 1 Section 2.5.3.16
80h PSPWRDWNSET0 Peripheral Power-Down SetRegister 0 Section 2.5.3.17
84h PSPWRDWNSET1 Peripheral Power-Down SetRegister 1 Section 2.5.3.18
88h PSPWRDWNSET2 Peripheral Power-Down SetRegister 2 Section 2.5.3.19
8Ch PSPWRDWNSET3 Peripheral Power-Down SetRegister 3 Section 2.5.3.20
A0h PSPWRDWNCLR0 Peripheral Power-Down Clear Register 0 Section 2.5.3.21
A4h PSPWRDWNCLR1 Peripheral Power-Down Clear Register 1 Section 2.5.3.22
A8h PSPWRDWNCLR2 Peripheral Power-Down Clear Register 2 Section 2.5.3.23
ACh PSPWRDWNCLR3 Peripheral Power-Down Clear Register 3 Section 2.5.3.24
C0h PDPWRDWNSET Debug Frame Power-Down SetRegister Section 2.5.3.25
C4h PDPWRDWNCLR Debug Frame Power-Down Clear Register Section 2.5.3.26
200h MSTIDWRENA MasterID Protection Write Enable Register Section 2.5.3.27
204h MSTIDENA MasterID Protection Enable Register Section 2.5.3.28
208h MSTIDDIAGCTRL MasterID Diagnostic Control Register Section 2.5.3.29
300h PS0MSTID_L Peripheral Frame 0Master-ID Protection Register_L Section 2.5.3.30
304h PS0MSTID_H Peripheral Frame 0Master-ID Protection Register_H Section 2.5.3.31
308h PS1MSTID_L Peripheral Frame 1Master-ID Protection Register_L Section 2.5.3.32

<!-- Page 218 -->

System andPeripheral Control Registers www.ti.com
218 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
30Ch PS1MSTID_H Peripheral Frame 1Master-ID Protection Register_H Section 2.5.3.32
310h PS2MSTID_L Peripheral Frame 2Master-ID Protection Register_L Section 2.5.3.32
314h PS2MSTID_H Peripheral Frame 2Master-ID Protection Register_H Section 2.5.3.32
318h PS3MSTID_L Peripheral Frame 3Master-ID Protection Register_L Section 2.5.3.32
31Ch PS3MSTID_H Peripheral Frame 3Master-ID Protection Register_H Section 2.5.3.32
320h PS4MSTID_L Peripheral Frame 4Master-ID Protection Register_L Section 2.5.3.32
324h PS4MSTID_H Peripheral Frame 4Master-ID Protection Register_H Section 2.5.3.32
328h PS5MSTID_L Peripheral Frame 5Master-ID Protection Register_L Section 2.5.3.32
32Ch PS5MSTID_H Peripheral Frame 5Master-ID Protection Register_H Section 2.5.3.32
330h PS6MSTID_L Peripheral Frame 6Master-ID Protection Register_L Section 2.5.3.32
334h PS6MSTID_H Peripheral Frame 6Master-ID Protection Register_H Section 2.5.3.32
338h PS7MSTID_L Peripheral Frame 7Master-ID Protection Register_L Section 2.5.3.32
33Ch PS7MSTID_H Peripheral Frame 7Master-ID Protection Register_H Section 2.5.3.32
340h PS8MSTID_L Peripheral Frame 8Master-ID Protection Register_L Section 2.5.3.32
344h PS8MSTID_H Peripheral Frame 8Master-ID Protection Register_H Section 2.5.3.32
348h PS9MSTID_L Peripheral Frame 9Master-ID Protection Register_L Section 2.5.3.32
34Ch PS9MSTID_H Peripheral Frame 9Master-ID Protection Register_H Section 2.5.3.32
350h PS10MSTID_L Peripheral Frame 10Master-ID Protection Register_L Section 2.5.3.32
354h PS10MSTID_H Peripheral Frame 10Master-ID Protection Register_H Section 2.5.3.32
358h PS11MSTID_L Peripheral Frame 11Master-ID Protection Register_L Section 2.5.3.32
35Ch PS11MSTID_H Peripheral Frame 11Master-ID Protection Register_H Section 2.5.3.32
360h PS12MSTID_L Peripheral Frame 12Master-ID Protection Register_L Section 2.5.3.32
364h PS12MSTID_H Peripheral Frame 12Master-ID Protection Register_H Section 2.5.3.32
368h PS13MSTID_L Peripheral Frame 13Master-ID Protection Register_L Section 2.5.3.32
36Ch PS13MSTID_H Peripheral Frame 13Master-ID Protection Register_H Section 2.5.3.32
370h PS14MSTID_L Peripheral Frame 14Master-ID Protection Register_L Section 2.5.3.32
374h PS14MSTID_H Peripheral Frame 14Master-ID Protection Register_H Section 2.5.3.32
378h PS15MSTID_L Peripheral Frame 15Master-ID Protection Register_L Section 2.5.3.32
37Ch PS15MSTID_H Peripheral Frame 15Master-ID Protection Register_H Section 2.5.3.32
380h PS16MSTID_L Peripheral Frame 16Master-ID Protection Register_L Section 2.5.3.32
384h PS16MSTID_H Peripheral Frame 16Master-ID Protection Register_H Section 2.5.3.32
388h PS17MSTID_L Peripheral Frame 17Master-ID Protection Register_L Section 2.5.3.32
38Ch PS17MSTID_H Peripheral Frame 17Master-ID Protection Register_H Section 2.5.3.32
390h PS18MSTID_L Peripheral Frame 18Master-ID Protection Register_L Section 2.5.3.32
394h PS18MSTID_H Peripheral Frame 18Master-ID Protection Register_H Section 2.5.3.32
398h PS19MSTID_L Peripheral Frame 19Master-ID Protection Register_L Section 2.5.3.32
39Ch PS19MSTID_H Peripheral Frame 19Master-ID Protection Register_H Section 2.5.3.32
3A0h PS20MSTID_L Peripheral Frame 20Master-ID Protection Register_L Section 2.5.3.32
3A4h PS20MSTID_H Peripheral Frame 20Master-ID Protection Register_H Section 2.5.3.32
3A8h PS21MSTID_L Peripheral Frame 21Master-ID Protection Register_L Section 2.5.3.32
3ACh PS21MSTID_H Peripheral Frame 21Master-ID Protection Register_H Section 2.5.3.32
3B0h PS22MSTID_L Peripheral Frame 22Master-ID Protection Register_L Section 2.5.3.32
3B4h PS22MSTID_H Peripheral Frame 22Master-ID Protection Register_H Section 2.5.3.32
3B8h PS23MSTID_L Peripheral Frame 23Master-ID Protection Register_L Section 2.5.3.32
3BCh PS23MSTID_H Peripheral Frame 23Master-ID Protection Register_H Section 2.5.3.32
3C0h PS24MSTID_L Peripheral Frame 24Master-ID Protection Register_L Section 2.5.3.32
3C4h PS24MSTID_H Peripheral Frame 24Master-ID Protection Register_H Section 2.5.3.32

<!-- Page 219 -->

www.ti.com System andPeripheral Control Registers
219 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
3C8h PS25MSTID_L Peripheral Frame 25Master-ID Protection Register_L Section 2.5.3.32
3CCh PS25MSTID_H Peripheral Frame 25Master-ID Protection Register_H Section 2.5.3.32
3D0h PS26MSTID_L Peripheral Frame 26Master-ID Protection Register_L Section 2.5.3.32
3D4h PS26MSTID_H Peripheral Frame 26Master-ID Protection Register_H Section 2.5.3.32
3D8h PS27MSTID_L Peripheral Frame 27Master-ID Protection Register_L Section 2.5.3.32
3DCh PS27MSTID_H Peripheral Frame 27Master-ID Protection Register_H Section 2.5.3.32
3E0h PS28MSTID_L Peripheral Frame 28Master-ID Protection Register_L Section 2.5.3.32
3E4h PS28MSTID_H Peripheral Frame 28Master-ID Protection Register_H Section 2.5.3.32
3E8h PS29MSTID_L Peripheral Frame 29Master-ID Protection Register_L Section 2.5.3.32
3ECh PS29MSTID_H Peripheral Frame 29Master-ID Protection Register_H Section 2.5.3.32
3E0h PS30MSTID_L Peripheral Frame 30Master-ID Protection Register_L Section 2.5.3.32
3F4h PS30MSTID_H Peripheral Frame 30Master-ID Protection Register_H Section 2.5.3.32
3F8h PS31MSTID_L Peripheral Frame 31Master-ID Protection Register_L Section 2.5.3.32
3FCh PS31MSTID_H Peripheral Frame 31Master-ID Protection Register_H Section 2.5.3.32
400h PPS0MSTID_L Privileged Peripheral Frame 0Master-ID Protection Register_L Section 2.5.3.33
404h PPS0MSTID_H Privileged Peripheral Frame 0Master-ID Protection Register_H Section 2.5.3.34
408h PPS1MSTID_L Privileged Peripheral Frame 1Master-ID Protection Register_L Section 2.5.3.35
40Ch PPS1MSTID_H Privileged Peripheral Frame 1Master-ID Protection Register_H Section 2.5.3.35
410h PPS2MSTID_L Privileged Peripheral Frame 2Master-ID Protection Register_L Section 2.5.3.35
414h PPS2MSTID_H Privileged Peripheral Frame 2Master-ID Protection Register_H Section 2.5.3.35
418h PPS3MSTID_L Privileged Peripheral Frame 3Master-ID Protection Register_L Section 2.5.3.35
41Ch PPS3MSTID_H Privileged Peripheral Frame 3Master-ID Protection Register_H Section 2.5.3.35
420h PPS4MSTID_L Privileged Peripheral Frame 4Master-ID Protection Register_L Section 2.5.3.35
424h PPS4MSTID_H Privileged Peripheral Frame 4Master-ID Protection Register_H Section 2.5.3.35
428h PPS5MSTID_L Privileged Peripheral Frame 5Master-ID Protection Register_L Section 2.5.3.35
42Ch PPS5MSTID_H Privileged Peripheral Frame 5Master-ID Protection Register_H Section 2.5.3.35
430h PPS6MSTID_L Privileged Peripheral Frame 6Master-ID Protection Register_L Section 2.5.3.35
434h PPS6MSTID_H Privileged Peripheral Frame 6Master-ID Protection Register_H Section 2.5.3.35
438h PPS7MSTID_L Privileged Peripheral Frame 7Master-ID Protection Register_L Section 2.5.3.35
43Ch PPS7MSTID_H Privileged Peripheral Frame 7Master-ID Protection Register_H Section 2.5.3.35
440h PPSE0MSTID_L Privilege Peripheral Extended Frame 0Master-ID Protection
Register_LSection 2.5.3.36
444h PPSE0MSTID_H Privilege Peripheral Extended Frame 0Master-ID Protection
Register_HSection 2.5.3.37
448h PPSE1MSTID_L Privilege Peripheral Extended Frame 1Master-ID Protection
Register_LSection 2.5.3.38
44Ch PPSE1MSTID_H Privilege Peripheral Extended Frame 1Master-ID Protection
Register_HSection 2.5.3.38
450h PPSE2MSTID_L Privilege Peripheral Extended Frame 2Master-ID Protection
Register_LSection 2.5.3.38
454h PPSE2MSTID_H Privilege Peripheral Extended Frame 2Master-ID Protection
Register_HSection 2.5.3.38
458h PPSE3MSTID_L Privilege Peripheral Extended Frame 3Master-ID Protection
Register_LSection 2.5.3.38
45Ch PPSE3MSTID_H Privilege Peripheral Extended Frame 3Master-ID Protection
Register_HSection 2.5.3.38
460h PPSE4MSTID_L Privilege Peripheral Extended Frame 4Master-ID Protection
Register_LSection 2.5.3.38
464h PPSE4MSTID_H Privilege Peripheral Extended Frame 4Master-ID Protection
Register_HSection 2.5.3.38

<!-- Page 220 -->

System andPeripheral Control Registers www.ti.com
220 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
468h PPSE5MSTID_L Privilege Peripheral Extended Frame 5Master-ID Protection
Register_LSection 2.5.3.38
46Ch PPSE5MSTID_H Privilege Peripheral Extended Frame 5Master-ID Protection
Register_HSection 2.5.3.38
470h PPSE6MSTID_L Privilege Peripheral Extended Frame 6Master-ID Protection
Register_LSection 2.5.3.38
474h PPSE6MSTID_H Privilege Peripheral Extended Frame 6Master-ID Protection
Register_HSection 2.5.3.38
478h PPSE7MSTID_L Privilege Peripheral Extended Frame 7Master-ID Protection
Register_LSection 2.5.3.38
47Ch PPSE7MSTID_H Privilege Peripheral Extended Frame 7Master-ID Protection
Register_HSection 2.5.3.38
480h PPSE8MSTID_L Privilege Peripheral Extended Frame 8Master-ID Protection
Register_LSection 2.5.3.38
484h PPSE8MSTID_H Privilege Peripheral Extended Frame 8Master-ID Protection
Register_HSection 2.5.3.38
488h PPSE9MSTID_L Privilege Peripheral Extended Frame 9Master-ID Protection
Register_LSection 2.5.3.38
48Ch PPSE9MSTID_H Privilege Peripheral Extended Frame 9Master-ID Protection
Register_HSection 2.5.3.38
490h PPSE10MSTID_L Privilege Peripheral Extended Frame 10Master-ID Protection
Register_LSection 2.5.3.38
494h PPSE10MSTID_H Privilege Peripheral Extended Frame 10Master-ID Protection
Register_HSection 2.5.3.38
498h PPSE11MSTID_L Privilege Peripheral Extended Frame 11Master-ID Protection
Register_LSection 2.5.3.38
49Ch PPSE11MSTID_H Privilege Peripheral Extended Frame 11Master-ID Protection
Register_HSection 2.5.3.38
4A0h PPSE12MSTID_L Privilege Peripheral Extended Frame 12Master-ID Protection
Register_LSection 2.5.3.38
4A4h PPSE12MSTID_H Privilege Peripheral Extended Frame 12Master-ID Protection
Register_HSection 2.5.3.38
4A8h PPSE13MSTID_L Privilege Peripheral Extended Frame 13Master-ID Protection
Register_LSection 2.5.3.38
4ACh PPSE13MSTID_H Privilege Peripheral Extended Frame 13Master-ID Protection
Register_HSection 2.5.3.38
4B0h PPSE14MSTID_L Privilege Peripheral Extended Frame 14Master-ID Protection
Register_LSection 2.5.3.38
4B4h PPSE14MSTID_H Privilege Peripheral Extended Frame 14Master-ID Protection
Register_HSection 2.5.3.38
4B8h PPSE15MSTID_L Privilege Peripheral Extended Frame 15Master-ID Protection
Register_LSection 2.5.3.38
4BCh PPSE15MSTID_H Privilege Peripheral Extended Frame 15Master-ID Protection
Register_HSection 2.5.3.38
4C0h PPSE16MSTID_L Privilege Peripheral Extended Frame 16Master-ID Protection
Register_LSection 2.5.3.38
4C4h PPSE16MSTID_H Privilege Peripheral Extended Frame 16Master-ID Protection
Register_HSection 2.5.3.38
4C8h PPSE17MSTID_L Privilege Peripheral Extended Frame 17Master-ID Protection
Register_LSection 2.5.3.38
4CCh PPSE17MSTID_H Privilege Peripheral Extended Frame 17Master-ID Protection
Register_HSection 2.5.3.38
4D0h PPSE18MSTID_L Privilege Peripheral Extended Frame 18Master-ID Protection
Register_LSection 2.5.3.38
4D4h PPSE18MSTID_H Privilege Peripheral Extended Frame 18Master-ID Protection
Register_HSection 2.5.3.38

<!-- Page 221 -->

www.ti.com System andPeripheral Control Registers
221 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
4D8h PPSE19MSTID_L Privilege Peripheral Extended Frame 19Master-ID Protection
Register_LSection 2.5.3.38
4DCh PPSE19MSTID_H Privilege Peripheral Extended Frame 19Master-ID Protection
Register_HSection 2.5.3.38
4E0h PPSE20MSTID_L Privilege Peripheral Extended Frame 20Master-ID Protection
Register_LSection 2.5.3.38
4E4h PPSE20MSTID_H Privilege Peripheral Extended Frame 20Master-ID Protection
Register_HSection 2.5.3.38
4E8h PPSE21MSTID_L Privilege Peripheral Extended Frame 21Master-ID Protection
Register_LSection 2.5.3.38
4ECh PPSE21MSTID_H Privilege Peripheral Extended Frame 21Master-ID Protection
Register_HSection 2.5.3.38
4E0h PPSE22MSTID_L Privilege Peripheral Extended Frame 22Master-ID Protection
Register_LSection 2.5.3.38
4F4h PPSE22MSTID_H Privilege Peripheral Extended Frame 22Master-ID Protection
Register_HSection 2.5.3.38
4F8h PPSE23MSTID_L Privilege Peripheral Extended Frame 23Master-ID Protection
Register_LSection 2.5.3.38
4FCh PPSE23MSTID_H Privilege Peripheral Extended Frame 23Master-ID Protection
Register_HSection 2.5.3.38
500h PPSE24MSTID_L Privilege Peripheral Extended Frame 24Master-ID Protection
Register_LSection 2.5.3.38
504h PPSE24MSTID_H Privilege Peripheral Extended Frame 24Master-ID Protection
Register_HSection 2.5.3.38
508h PPSE25MSTID_L Privilege Peripheral Extended Frame 25Master-ID Protection
Register_LSection 2.5.3.38
50Ch PPSE25MSTID_H Privilege Peripheral Extended Frame 25Master-ID Protection
Register_HSection 2.5.3.38
510h PPSE26MSTID_L Privilege Peripheral Extended Frame 26Master-ID Protection
Register_LSection 2.5.3.38
514h PPSE26MSTID_H Privilege Peripheral Extended Frame 26Master-ID Protection
Register_HSection 2.5.3.38
518h PPSE27MSTID_L Privilege Peripheral Extended Frame 27Master-ID Protection
Register_LSection 2.5.3.38
51Ch PPSE27MSTID_H Privilege Peripheral Extended Frame 27Master-ID Protection
Register_HSection 2.5.3.38
520h PPSE28MSTID_L Privilege Peripheral Extended Frame 28Master-ID Protection
Register_LSection 2.5.3.38
524h PPSE28MSTID_H Privilege Peripheral Extended Frame 28Master-ID Protection
Register_HSection 2.5.3.38
528h PPSE29MSTID_L Privilege Peripheral Extended Frame 29Master-ID Protection
Register_LSection 2.5.3.38
52Ch PPSE29MSTID_H Privilege Peripheral Extended Frame 29Master-ID Protection
Register_HSection 2.5.3.38
530h PPSE30MSTID_L Privilege Peripheral Extended Frame 30Master-ID Protection
Register_LSection 2.5.3.38
534h PPSE30MSTID_H Privilege Peripheral Extended Frame 30Master-ID Protection
Register_HSection 2.5.3.38
538h PPSE31MSTID_L Privilege Peripheral Extended Frame 31Master-ID Protection
Register_LSection 2.5.3.38
53Ch PPSE31MSTID_H Privilege Peripheral Extended Frame 31Master-ID Protection
Register_HSection 2.5.3.38
540h PCS0MSTID Peripheral Memory Frame Master-ID Protection Register0 Section 2.5.3.39
544h PCS1MSTID Peripheral Memory Frame Master-ID Protection Register1 Section 2.5.3.39
548h PCS2MSTID Peripheral Memory Frame Master-ID Protection Register2 Section 2.5.3.39
54Ch PCS3MSTID Peripheral Memory Frame Master-ID Protection Register3 Section 2.5.3.39

<!-- Page 222 -->

System andPeripheral Control Registers www.ti.com
222 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-85. Peripheral Central Resource Control Registers (continued)
Offset Acronym Register Description Section
550h PCS4MSTID Peripheral Memory Frame Master-ID Protection Register4 Section 2.5.3.39
554h PCS5MSTID Peripheral Memory Frame Master-ID Protection Register5 Section 2.5.3.39
558h PCS6MSTID Peripheral Memory Frame Master-ID Protection Register6 Section 2.5.3.39
55Ch PCS7MSTID Peripheral Memory Frame Master-ID Protection Register7 Section 2.5.3.39
560h PCS8MSTID Peripheral Memory Frame Master-ID Protection Register8 Section 2.5.3.39
564h PCS9MSTID Peripheral Memory Frame Master-ID Protection Register9 Section 2.5.3.39
568h PCS10MSTID Peripheral Memory Frame Master-ID Protection Register10 Section 2.5.3.39
56Ch PCS11MSTID Peripheral Memory Frame Master-ID Protection Register11 Section 2.5.3.39
570h PCS12MSTID Peripheral Memory Frame Master-ID Protection Register12 Section 2.5.3.39
574h PCS13MSTID Peripheral Memory Frame Master-ID Protection Register13 Section 2.5.3.39
578h PCS14MSTID Peripheral Memory Frame Master-ID Protection Register14 Section 2.5.3.39
57Ch PCS15MSTID Peripheral Memory Frame Master-ID Protection Register15 Section 2.5.3.39
580h PCS16MSTID Peripheral Memory Frame Master-ID Protection Register16 Section 2.5.3.39
584h PCS17MSTID Peripheral Memory Frame Master-ID Protection Register17 Section 2.5.3.39
588h PCS18MSTID Peripheral Memory Frame Master-ID Protection Register18 Section 2.5.3.39
58Ch PCS19MSTID Peripheral Memory Frame Master-ID Protection Register19 Section 2.5.3.39
590h PCS20MSTID Peripheral Memory Frame Master-ID Protection Register20 Section 2.5.3.39
594h PCS21MSTID Peripheral Memory Frame Master-ID Protection Register21 Section 2.5.3.39
598h PCS22MSTID Peripheral Memory Frame Master-ID Protection Register22 Section 2.5.3.39
59Ch PCS23MSTID Peripheral Memory Frame Master-ID Protection Register23 Section 2.5.3.39
5A0h PCS24MSTID Peripheral Memory Frame Master-ID Protection Register24 Section 2.5.3.39
5A4h PCS25MSTID Peripheral Memory Frame Master-ID Protection Register25 Section 2.5.3.39
5A8h PCS26MSTID Peripheral Memory Frame Master-ID Protection Register26 Section 2.5.3.39
5ACh PCS27MSTID Peripheral Memory Frame Master-ID Protection Register27 Section 2.5.3.39
5B0h PCS28MSTID Peripheral Memory Frame Master-ID Protection Register28 Section 2.5.3.39
5B4h PCS29MSTID Peripheral Memory Frame Master-ID Protection Register29 Section 2.5.3.39
5B8h PCS30MSTID Peripheral Memory Frame Master-ID Protection Register30 Section 2.5.3.39
5BCh PCS31MSTID Peripheral Memory Frame Master-ID Protection Register31 Section 2.5.3.39
5C0h PPCS0MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register0Section 2.5.3.40
5C4h PPCS1MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register1Section 2.5.3.40
5C8h PPCS2MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register2Section 2.5.3.40
5CCh PPCS3MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register3Section 2.5.3.40
5D0h PPCS4MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register4Section 2.5.3.40
5D4h PPCS5MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register5Section 2.5.3.40
5D8h PPCS6MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register6Section 2.5.3.40
5DCh PPCS7MSTID Privileged Peripheral Memory Frame Master-ID Protection
Register7Section 2.5.3.40

<!-- Page 223 -->

www.ti.com System andPeripheral Control Registers
223 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.1 Peripheral Memory Protection SetRegister 0(PMPROTSET0)
This register isshown inFigure 2-72 anddescribed inTable 2-86.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tonon-implemented bitshave noeffect andreads are0.
Figure 2-72. Peripheral Memory Protection SetRegister 0(PMPROTSET0) (offset =00h)
31 0
PCS[31-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-86. Peripheral Memory Protection SetRegister 0(PMPROTSET0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PROTSET Peripheral memory frame protection set.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET0 andPMPROTCLR0 registers issetto1.
2.5.3.2 Peripheral Memory Protection SetRegister 1(PMPROTSET1)
This register isshown inFigure 2-73 anddescribed inTable 2-87.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-73. Peripheral Memory Protection SetRegister 1(PMPROTSET1) (offset =04h)
31 0
PCS[63-32]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-87. Peripheral Memory Protection SetRegister 1(PMPROTSET1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PROTSET Peripheral memory frame protection set.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET1 andPMPROTCLR1 registers issetto1.

<!-- Page 224 -->

System andPeripheral Control Registers www.ti.com
224 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.3 Peripheral Memory Protection Clear Register 0(PMPROTCLR0)
This register isshown inFigure 2-74 anddescribed inTable 2-88.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-74. Peripheral Memory Protection Clear Register 0(PMPROTCLR0) (offset =10h)
31 0
PCS[31-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-88. Peripheral Memory Protection Clear Register 0(PMPROTCLR0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PROTCLR Peripheral memory frame protection clear.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET0 andPMPROTCLR0 registers iscleared to0.
2.5.3.4 Peripheral Memory Protection Clear Register 1(PMPROTCLR1)
This register isshown inFigure 2-75 anddescribed inTable 2-89.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-75. Peripheral Memory Protection Clear Register 1(PMPROTCLR1) (offset =14h)
31 0
PCS[63-32]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-89. Peripheral Memory Protection Clear Register 1(PMPROTCLR1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PROTCLR Peripheral memory frame protection clear.
0 Read: The peripheral memory frame ncanbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral memory frame ncanbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPMPROTSET1 andPMPROTCLR1 registers iscleared to0.

<!-- Page 225 -->

www.ti.com System andPeripheral Control Registers
225 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.5 Peripheral Protection SetRegister 0(PPROTSET0)
There isonebitforeach quadrant forPS0 toPS7.
The following aretheways thatquadrants areused within aPSframe:
a.The slave uses allthefour quadrants.
Only thebitcorresponding tothequadrant 0ofPSn isimplemented. Itprotects thewhole 1K-byte
frame. The remaining three bitsarenotimplemented.
b.The slave uses twoquadrants.
Each quadrant hastobeinoneofthese groups: (Quad 0andQuad 1)or(Quad 2andQuad 3).
Forthegroup Quad0/Quad1, thebitquadrant 0protects both quadrants 0and1.The bitquadrant 1is
notimplemented.
Forthegroup Quad2/Quad3, thebitquadrant 2protects both quadrants 2and3.The bitquadrant 3is
notimplemented
c.The slave uses only onequadrant.
Inthiscase, thebit,asspecified inTable 2-90 ,protects theslave.
The above arrangement istrue foralltheperipheral selects (PS0 toPS31), presented inSection 2.5.3.6 -
Section 2.5.3.12 .This register holds bitsforPS0 toPS7 andisshown inFigure 2-76 anddescribed in
Table 2-90.
NOTE: Writes tounimplemented bitshave noeffect andreads are0.
Figure 2-76. Peripheral Protection SetRegister 0(PPROTSET0) (offset =20h)
31 0
PS[7-0]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-90. Peripheral Protection SetRegister 0(PPROTSET0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant anbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET0 andPPROTCLR0 registers issetto1.

<!-- Page 226 -->

System andPeripheral Control Registers www.ti.com
226 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.6 Peripheral Protection SetRegister 1(PPROTSET1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-77 anddescribed inTable 2-91 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-77. Peripheral Protection SetRegister 1(PPROTSET1) (offset =24h)
31 0
PS[15-8]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-91. Peripheral Protection SetRegister 1(PPROTSET1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET1 andPPROTCLR1 registers issetto1.
2.5.3.7 Peripheral Protection SetRegister 2(PPROTSET2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-78 anddescribed inTable 2-92 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-78. Peripheral Protection SetRegister 2(PPROTSET2) (offset =28h)
31 0
PS[23-16]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-92. Peripheral Protection SetRegister 2(PPROTSET2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET2 andPPROTCLR2 registers issetto1.

<!-- Page 227 -->

www.ti.com System andPeripheral Control Registers
227 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.8 Peripheral Protection SetRegister 3(PPROTSET3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-79 anddescribed inTable 2-93 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-79. Peripheral Protection SetRegister 3(PPROTSET3) (offset =2Ch)
31 0
PS[31-24]QUAD[3-0]PROTSET
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-93. Peripheral Protection SetRegister 3(PPROTSET3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PROTSETPeripheral select quadrant protection set.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET3 andPPROTCLR3 registers issetto1.
2.5.3.9 Peripheral Protection Clear Register 0(PPROTCLR0)
There isonebitforeach quadrant forPS0 toPS7. The protection scheme isdescribed inSection 2.5.3.5 .
This register isshown inFigure 2-80 anddescribed inTable 2-94.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-80. Peripheral Protection Clear Register 0(PPROTCLR0) (offset =40h)
31 0
PS[7-0]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-94. Peripheral Protection Clear Register 0(PPROTCLR0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET0 andPPROTCLR0 registers iscleared to0.

<!-- Page 228 -->

System andPeripheral Control Registers www.ti.com
228 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.10 Peripheral Protection Clear Register 1(PPROTCLR1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-81 anddescribed inTable 2-95 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-81. Peripheral Protection Clear Register 1(PPROTCLR1) (offset =44h)
31 0
PS[15-8]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-95. Peripheral Protection Clear Register 1(PPROTCLR1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET1 andPPROTCLR1 registers iscleared to0.
2.5.3.11 Peripheral Protection Clear Register 2(PPROTCLR2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-82 anddescribed inTable 2-96 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-82. Peripheral Protection Clear Register 2(PPROTCLR2) (offset =48h)
31 0
PS[23-16]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-96. Peripheral Protection Clear Register 2(PPROTCLR2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET2 andPPROTCLR2 registers iscleared to0.

<!-- Page 229 -->

www.ti.com System andPeripheral Control Registers
229 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.12 Peripheral Protection Clear Register 3(PPROTCLR3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.5 .This register isshown inFigure 2-83 anddescribed inTable 2-97 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-83. Peripheral Protection Clear Register 3(PPROTCLR3) (offset =4Ch)
31 0
PS[31-24]QUAD[3-0]PROTCLR
R/WP-0
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-97. Peripheral Protection Clear Register 3(PPROTCLR3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PROTCLRPeripheral select quadrant protection clear.
0 Read: The peripheral select quadrant canbewritten toandread from inboth user and
privileged modes.
Write: The bitisunchanged.
1 Read: The peripheral select quadrant canbewritten toonly inprivileged mode, butitcanbe
read inboth user andprivileged modes.
Write: The corresponding bitinPPROTSET3 andPPROTCLR3 registers iscleared to0.

<!-- Page 230 -->

System andPeripheral Control Registers www.ti.com
230 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.13 Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0)
Each bitcorresponds toabitatthesame index inthePMPROT register inthatthey both relate tothe
same peripheral. This register isshown inFigure 2-84 anddescribed inTable 2-98.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-84. Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0) (offset =60h)
31 0
PCS[31-0]PWRDNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-98. Peripheral Memory Power-Down SetRegister 0(PCSPWRDWNSET0) Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PWRDNSET Peripheral memory clock power-down set.
0 Read: The peripheral memory clock[31-0] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[31-0] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET0 andPCSPWRDWNCLR0 registers
issetto1.
2.5.3.14 Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1)
This register isshown inFigure 2-85 anddescribed inTable 2-99.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-85. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1) (offset =64h)
31 0
PCS[63-32]PWRDNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-99. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNSET1) Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PWRDNSET Peripheral memory clock power-down set.
0 Read: The peripheral memory clock[63-32] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[63-32] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET1 andPCSPWRDWNCLR1 registers
issetto1.

<!-- Page 231 -->

www.ti.com System andPeripheral Control Registers
231 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.15 Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
This register isshown inFigure 2-86 anddescribed inTable 2-100 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-86. Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
(offset =70h)
31 0
PCS[31-0]PWRDNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-100. Peripheral Memory Power-Down Clear Register 0(PCSPWRDWNCLR0)
Field Descriptions
Bit Field Value Description
31-0 PCS[31-0]PWRDNCLR Peripheral memory clock power-down clear.
0 Read: The peripheral memory clock[31-0] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[31-0] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET0 andPCSPWRDWNCLR0 registers
iscleared to0.
2.5.3.16 Peripheral Memory Power-Down Clear Register 1(PCSPWRDWNCLR1)
This register isshown inFigure 2-87 anddescribed inTable 2-101 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-87. Peripheral Memory Power-Down Clear Register 1(PCSPWRDWNCLR1)
(offset =74h)
31 0
PCS[63-32]PWRDNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-101. Peripheral Memory Power-Down SetRegister 1(PCSPWRDWNCLR1)
Field Descriptions
Bit Field Value Description
31-0 PCS[63-32]PWRDNCLR Peripheral memory clock power-down clear.
0 Read: The peripheral memory clock[63-32] isactive.
Write: The bitisunchanged.
1 Read: The peripheral memory clock[63-32] isinactive.
Write: The corresponding bitinthePCSPWRDWNSET1 andPCSPWRDWNCLR1 registers
iscleared to0.

<!-- Page 232 -->

System andPeripheral Control Registers www.ti.com
232 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.17 Peripheral Power-Down SetRegister 0(PSPWRDWNSET0)
There isonebitforeach quadrant forPS0 toPS7. Each bitofthisregister corresponds tothebitatthe
same index inthecorresponding PPROT register inthatthey relate tothesame peripheral. These bitsare
used topower down/power uptheclock tothecorresponding peripheral.
Forevery bitimplemented inthePPROT register, there isonebitinthePSnPWRDWN register, except
when twoperipherals (both inPSarea) share buses. Inthatcase, only onePower-Down bitis
implemented, attheposition corresponding tothatperipheral whose quadrant comes first(the lower
numbered).
The ways inwhich quadrants canbeused within aframe areidentical towhat isdescribed under
PPROTSET0, Section 2.5.3.5 .
This arrangement isthesame forbitsofPS8 toPS31, presented inSection 2.5.3.18 -Section 2.5.3.24 .
This register holds bitsforPS0 toPS7. This register isshown inFigure 2-88 anddescribed inTable 2-
102.
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-88. Peripheral Power-Down SetRegister 0(PSPWRDWNSET0) (offset =80h)
31 0
PS[7-0]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-102. Peripheral Power-Down SetRegister 0(PSPWRDWNSET0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET0 andPSPWRDWNCLR0 registers isset
to1.

<!-- Page 233 -->

www.ti.com System andPeripheral Control Registers
233 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.18 Peripheral Power-Down SetRegister 1(PSPWRDWNSET1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-89 anddescribed inTable 2-103 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-89. Peripheral Power-Down SetRegister 1(PSPWRDWNSET1) (offset =84h)
31 0
PS[15-8]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-103. Peripheral Power-Down SetRegister 1(PSPWRDWNSET1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET1 andPSPWRDWNCLR1 registers isset
to1.
2.5.3.19 Peripheral Power-Down SetRegister 2(PSPWRDWNSET2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-90 anddescribed inTable 2-104 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-90. Peripheral Power-Down SetRegister 2(PSPWRDWNSET2) (offset =88h)
31 0
PS[23-16]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-104. Peripheral Power-Down SetRegister 2(PSPWRDWNSET2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET2 andPSPWRDWNCLR2 registers isset
to1.

<!-- Page 234 -->

System andPeripheral Control Registers www.ti.com
234 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.20 Peripheral Power-Down SetRegister 3(PSPWRDWNSET3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-91 anddescribed inTable 2-105 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-91. Peripheral Power-Down SetRegister 3(PSPWRDWNSET3) (offset =8Ch)
31 0
PS[31-24]QUAD[3-0]PWRDWNSET
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-105. Peripheral Power-Down SetRegister 3(PSPWRDWNSET3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PWRDWNSETPeripheral select quadrant clock power-down set.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET3 andPSPWRDWNCLR3 registers isset
to1.
2.5.3.21 Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0)
There isonebitforeach quadrant forPS0 toPS7. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-92 anddescribed inTable 2-106 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-92. Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0) (offset =A0h)
31 0
PS[7-0]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-106. Peripheral Power-Down Clear Register 0(PSPWRDWNCLR0) Field Descriptions
Bit Field Value Description
31-0 PS[7-0]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET0 andPSPWRDWNCLR0 registers is
cleared to0.

<!-- Page 235 -->

www.ti.com System andPeripheral Control Registers
235 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.22 Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1)
There isonebitforeach quadrant forPS8 toPS15. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-93 anddescribed inTable 2-107 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-93. Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1) (offset =A4h)
31 0
PS[15-8]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-107. Peripheral Power-Down Clear Register 1(PSPWRDWNCLR1) Field Descriptions
Bit Field Value Description
31-0 PS[15-8]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET1 andPSPWRDWNCLR1 registers is
cleared to0.
2.5.3.23 Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2)
There isonebitforeach quadrant forPS16 toPS23. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-94 anddescribed inTable 2-108 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-94. Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2) (offset =A8h)
31 0
PS[23-16]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-108. Peripheral Power-Down Clear Register 2(PSPWRDWNCLR2) Field Descriptions
Bit Field Value Description
31-0 PS[23-16]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET2 andPSPWRDWNCLR2 registers is
cleared to0.

<!-- Page 236 -->

System andPeripheral Control Registers www.ti.com
236 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.24 Peripheral Power-Down Clear Register 3(PSPWRDWNCLR3)
There isonebitforeach quadrant forPS24 toPS31. The protection scheme isdescribed in
Section 2.5.3.17 .This register isshown inFigure 2-95 anddescribed inTable 2-109 .
NOTE: Only those bitsthathave aslave atthecorresponding bitposition areimplemented. Writes
tounimplemented bitshave noeffect andreads are0.
Figure 2-95. Peripheral Power-Down Clear Register 3(PSPWRDWNCLR) (offset =ACh)
31 0
PS[31-24]QUAD[3-0]PWRDWNCLR
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-109. Peripheral Power-Down Clear Register 3(PSPWRDWNCLR3) Field Descriptions
Bit Field Value Description
31-0 PS[31-24]QUAD[3-0]
PWRDWNCLRPeripheral select quadrant clock power-down clear.
0 Read: The clock totheperipheral select quadrant isactive.
Write: The bitisunchanged.
1 Read: The clock totheperipheral select quadrant isinactive.
Write: The corresponding bitinPSPWRDWNSET3 andPSPWRDWNCLR3 registers is
cleared to0.
2.5.3.25 Debug Frame Powerdown SetRegister (PDPWRDWNSET)
Figure 2-96. Debug Frame Powerdown SetRegister (PDPWRDWNSET) (offset =C0h)
31 1 0
Reserved PDWRDWNSET
R-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-110. Debug Frame Powerdown SetRegister (PDPWRDWNSET) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PDWRDWNSET Debug Frame Powerdown SetRegister.
0 Read: The clock tothedebug frame isactive.
Write: The bitisunchanged.
1 Read: The clock tothedebug frame isinactive.
Write: Setthebitto1.

<!-- Page 237 -->

www.ti.com System andPeripheral Control Registers
237 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.26 Debug Frame Powerdown Clear Register (PDPWRDWNCLR)
Figure 2-97. Debug Frame Powerdown Clear Register (PDPWRDWNCLR) (offset =C4h)
31 1 0
Reserved PDWRDWNCLR
R-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-111. Debug Frame Powerdown Clear Register (PDPWRDWNCLR) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PDWRDWNCLR Debug Frame Powerdown SetRegister.
0 Read: The clock tothedebug frame isactive.
Write: The bitisunchanged.
1 Read: The clock tothedebug frame isinactive.
Write: Clear thebitto0.
2.5.3.27 MasterID Protection Write Enable Register (MSTIDWRENA)
Figure 2-98. MasterID Protection Write Enable Register (MSTIDWRENA) (offset =200h)
31 16
Reserved
R-0
15 4 3 0
Reserved MSTIDREG_WRENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-112. MasterID Protection Write Enable Register (MSTIDWRENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTIDREG_WRENA MasterID Register Write Enable. This isa4-bit keyforenabling writes toallMaster-ID
registers from address offset 0x300-0x5DC. This keymust beprogrammed with 1010 to
unlock writes toallMaster-ID registers.
Ah Read: Allmaster-ID registers areunlocked andavailable forwrites.
Write: Writes tomaster-ID registers areunlocked.
Others Read: Writes toallmaster-ID registers arelocked.
Write: Write tomaster-ID registers arelocked.

<!-- Page 238 -->

System andPeripheral Control Registers www.ti.com
238 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.28 MasterID Enable Register (MSTIDENA)
Figure 2-99. MasterID Enable Register (MSTIDENA) (offset =204h)
31 16
Reserved
R-0
15 4 3 0
Reserved MSTID_CHK_ENA
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-113. MasterID Enable Register (MSTIDENA) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 MSTID_CHK_ENA MasterID Check Enable. This isa4-bit keyforenabling Master-ID check. This keymust be
programmed with 1010 toenable Master-ID Check functionality.
Ah Read: The master-ID check isenabled.
Write: Enable master-ID check.
Others Read: The master-ID check isdisabled.
Write: Disable master-ID check.

<!-- Page 239 -->

www.ti.com System andPeripheral Control Registers
239 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.29 MasterID Diagnostic Control Register (MSTIDDIAGCTRL)
Figure 2-100. MasterID Diagnostic Control Register (MSTIDDIAGCTRL) (offset =208h)
31 16
Reserved
R-0
15 12 11 8 7 4 3 0
Reserved DIAG_CMP_VALUE Reserved DIAG_MODE_ENA
R-0 R/WP-0 R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inprivileged mode only; -n=value after reset
Table 2-114. MasterID Diagnostic Control Register (MSTIDDIAGCTRL) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 DIAG_CMP_VALUE Diagnostic Compare Value. The value stored inthisregister iscompared against the
programmed master-ID register bitsforallaccesses. Indiagnostic mode, themaster-ID
register selection depends ontheDIAG_CMP_VALUE instead oftheinput 4-bit master-ID
generated bytheinterconnect. Any mismatch willbesignaled tothebusmaster asabus
error. After thediagnostic mode isenabled inDIAG_MODE_ENA register andadiagnostic
compare value isprogrammed intotheDIAG_CMP_VALUE register, theapplication must
issue adummy diagnostic write access toanyoneoftheperipherals tocause adiagnostic
check. Forexample, ifallmaster-ID protection registers listed from address 0x300-0x5DC
areprogrammed toblock master-ID 5from write access totheperipherals, then the
application canprogram theDIAG_CMP_VALUE to5.The application canusetheCPU
(whose master-ID is0)toissue adummy write access toanyperipheral tocause master-ID
violation during diagnostic mode instead ofusing thebusmaster whose master-ID is5to
perform thiswrite access.
Ah Read: The master-ID check isenabled.
Write: Enable master-ID check.
Others Read: The master-ID check isdisabled.
Write: Disable master-ID check.
7-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 DIAG_MODE_ENA Diagnostic Mode Enable. This isa4-bit keyforenabling Diagnostic Mode. This keymust be
programmed with 1010 toenable Diagnostic Mode.
Ah Read: The diagnostic mode isenabled.
Write: Enable diagnostic mode.
Others Read: The diagnostic mode isdisabled.
Write: Disable diagnostic mode.

<!-- Page 240 -->

System andPeripheral Control Registers www.ti.com
240 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.30 Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
There isonebitforeach quadrant forPS0 toPS31.
NOTE: Ifamodule occupies twoquadrants, then only thelower quadrant register isused toenable
ordisable themasterID. The upper quadrant register remains zeros.
The following aretheways thatquadrants areused within aPSframe:
a.The slave uses allthefour quadrants.
Only thebitcorresponding tothequadrant 0ofPSn isimplemented. Itprotects thewhole 1K-byte
frame. The remaining three bitsarenotimplemented.
b.The slave uses twoquadrants.
Each quadrant hastobeinoneofthese groups: (Quad 0andQuad 1)or(Quad 2andQuad 3).
Forthegroup Quad0/Quad1, thebitquadrant 0protects both quadrants 0and1.The bitquadrant 1is
notimplemented.
Forthegroup Quad2/Quad3, thebitquadrant 2protects both quadrants 2and3.The bitquadrant 3is
notimplemented
c.The slave uses only onequadrant.
Inthiscase, thebit,asspecified inTable 2-115 ,protects theslave.
The above arrangement istrue foralltheperipheral selects (PS0 toPS31), presented inSection 2.5.3.31 -
Section 2.5.3.32 .This register holds bitsforPS0 andisshown inFigure 2-101 anddescribed inTable 2-
115.
Figure 2-101. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
(offset =300h)
31 16
PS0_QUAD1_MSTID
R/WP-FFFFh
15 0
PS0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-115. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
Field Descriptions
Bit Field Value Description
31-16 PS0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPS[0]. There are16bitsforeach quadrant inPSframe.
Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds tomaster-ID 0
andbit15corresponds tomaster-ID 15.These bitssetthepermission formaximum of16
masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPS[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPS[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 241 -->

www.ti.com System andPeripheral Control Registers
241 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitectureTable 2-115. Peripheral Frame 0MasterID Protection Register_L (PS0MSTID_L)
Field Descriptions (continued)
Bit Field Value Description
15-0 PS0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 242 -->

System andPeripheral Control Registers www.ti.com
242 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.31 Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
There isonebitforeach quadrant forPS0 toPS31. The protection scheme isdescribed in
Section 2.5.3.30 .This register isshown inFigure 2-102 anddescribed inTable 2-116 .
Figure 2-102. Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
(offset =304h)
31 16
PS0_QUAD3_MSTID
R/WP-FFFFh
15 0
PS0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-116. Peripheral Frame 0MasterID Protection Register_H (PS0MSTID_H)
Field Descriptions
Bit Field Value Description
31-16 PS0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PS0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 243 -->

www.ti.com System andPeripheral Control Registers
243 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.32 Peripheral Frame nMasterID Protection Register_L/H (PS[1-31]MSTID_L/H)
There isonebitforeach quadrant forPS0 toPS31. The protection scheme isdescribed in
Section 2.5.3.30 .This register isshown inFigure 2-103 anddescribed inTable 2-117 .
Figure 2-103. Peripheral Frame nMasterID Protection Register_L/H (PSnMSTID_L/H)
(offset =308h-3FCh)
31 16
PSn_QUAD3_MSTID orPSn_QUAD1_MSTID
R/WP-FFFFh
15 0
PSn_QUAD2_MSTID orPSn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-117. Peripheral Frame nMasterID Protection Register_L/H (PSnMSTID_L/H)
Field Descriptions
Bit Field Value Description
31-16 PSn_QUAD3_MSTID or
PSn_QUAD1_MSTIDn:1to31.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPS[n] orQuadrant 1ofPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PSn_QUAD2_MSTID or
PSn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPS[n] orQuadrant 0ofPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 244 -->

System andPeripheral Control Registers www.ti.com
244 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.33 Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
Figure 2-104. Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
(offset =400h)
31 16
PPS0_QUAD1_MSTID
R/WP-FFFFh
15 0
PPS0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-118. Privileged Peripheral Frame 0MasterID Protection Register_L (PPS0MSTID_L)
Field Descriptions
Bit Field Value Description
31-16 PPS0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPPS[0]. There are16bitsforeach quadrant inPPS
frame. Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds to
master-ID 0andbit15corresponds tomaster-ID 15.These bitssetthepermission for
maximum of16masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPPS[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPPS[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
15-0 PPS0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 245 -->

www.ti.com System andPeripheral Control Registers
245 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.34 Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
Figure 2-105. Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
(offset =404h)
31 16
PPS0_QUAD3_MSTID
R/WP-FFFFh
15 0
PPS0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-119. Privileged Peripheral Frame 0MasterID Protection Register_H (PPS0MSTID_H)
Field Description
Bit Field Value Description
31-16 PPS0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPS0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPPS[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 246 -->

System andPeripheral Control Registers www.ti.com
246 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.35 Privileged Peripheral Frame nMasterID Protection Register_L/H (PPS[1-7]MSTID_L/H)
Figure 2-106. Privileged Peripheral Frame nMasterID Protection Register_L/H (PPSnMSTID_L/H)
(offset =408h-43Ch)
31 16
PPSn_QUAD3_MSTID orPPSn_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSn_QUAD2_MSTID orPPSn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-120. Privileged Peripheral Frame nMasterID Protection Register_L/H (PPSnMSTID_L/H)
Field Descriptions
Bit Field Value Description
31-16 PPSn_QUAD3_MSTID or
PPSn_QUAD1_MSTIDn:1to7.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPPS[n] orQuadrant 1ofPPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSn_QUAD2_MSTID or
PPSn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPPS[n] orQuadrant 0ofPPS[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 247 -->

www.ti.com System andPeripheral Control Registers
247 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.36 Privileged Peripheral Extended Frame 0MasterID Protection Register_L (PPSE0MSTID_L)
Figure 2-107. Privileged Peripheral Extended Frame 0MasterID Protection Register_L
(PPSE0MSTID_L) (offset =440h)
31 16
PPSE0_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSE0_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-121. Privileged Peripheral Extended Frame 0MasterID Protection Register_L
(PPSE0MSTID_L) Field Descriptions
Bit Field Value Description
31-16 PPSE0_QUAD1_MSTID MasterID filtering forQuadrant 1ofPPSE[0]. There are16bitsforeach quadrant inPPSE
frame. Each bitcorresponds toamaster-ID value. Forexample, bit0corresponds to
master-ID 0andbit15corresponds tomaster-ID 15.These bitssetthepermission for
maximum of16masters toaddress theperipheral mapped ineach ofthequadrant.
The following examples shows theusage ofthese register bits.
(a)Ifbits15:0 are1010_1010_1010_1010, then theperipheral thatismapped toQuadrant
0ofPPSE[0] canbeaddressed byMasters with Master-ID equals to1,3,5,7,9,11,13,15.
(b)ifbits15:0 are0000_0000_0000_0001, then theperipheral thatismapped toQuadrant
0ofPPSE[0] canonly addressed bythemaster with theMaster-ID equal to0.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
15-0 PPSE0_QUAD0_MSTID MasterID filtering forQuadrant 0ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral mapped to
thisquadrant.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral mapped tothis
quadrant.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral mapped
tothisquadrant.

<!-- Page 248 -->

System andPeripheral Control Registers www.ti.com
248 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.37 Privileged Peripheral Extended Frame 0MasterID Protection Register_H (PPSE0MSTID_H)
Figure 2-108. Privileged Peripheral Extended Frame 0MasterID Protection Register_H
(PPSE0MSTID_H) (offset =444h)
31 16
PPSE0_QUAD3_MSTID
R/WP-FFFFh
15 0
PPSE0_QUAD2_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-122. Privileged Peripheral Extended Frame 0MasterID Protection Register_H
(PPSE0MSTID_H) Field Descriptions
Bit Field Value Description
31-16 PPSE0_QUAD3_MSTID MasterID filtering forQuadrant 3ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSE0_QUAD2_MSTID MasterID filtering forQuadrant 2ofPPSE[0].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 249 -->

www.ti.com System andPeripheral Control Registers
249 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.38 Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSE[1-31]MSTID_L/H)
Figure 2-109. Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSEnMSTID_L/H) (offset =448h-53Ch)
31 16
PPSEn_QUAD3_MSTID orPPSEn_QUAD1_MSTID
R/WP-FFFFh
15 0
PPSEn_QUAD2_MSTID orPPSEn_QUAD0_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-123. Privileged Peripheral Extended Frame nMasterID Protection Register_L/H
(PPSEnMSTID_L/H) Field Descriptions
Bit Field Value Description
31-16 PPSEn_QUAD3_MSTID
or
PPSEn_QUAD1_MSTIDn:1to31.L:quadrant0 andquadrant1. H:quadrant2 andquadrant3.
MasterID filtering forQuadrant 3ofPPSE[n] orQuadrant 1ofPPSE[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPSEn_QUAD2_MSTID
or
PPSEn_QUAD0_MSTIDMasterID filtering forQuadrant 2ofPPSE[n] orQuadrant 0ofPPSE[n].
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 250 -->

System andPeripheral Control Registers www.ti.com
250 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.39 Peripheral Memory Frame MasterID Protection Register (PCS[0-31]MSTID)
Figure 2-110. Peripheral Memory Frame MasterID Protection Register (PCSnMSTID)
(offset =540h-5BCh)
31 16
PCS(2n+1)_MSTID
R/WP-FFFFh
15 0
PCS(2n)_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-124. Peripheral Memory Frame MasterID Protection Register (PCSnMSTID)
Field Descriptions
Bit Field Value Description
31-16 PCS(2n+1)_MSTID MasterID filtering forPCS[2n+1], where n=0to31.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PCS(2n)_MSTID MasterID filtering forPCS[2n], where n=0to31.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.

<!-- Page 251 -->

www.ti.com System andPeripheral Control Registers
251 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedArchitecture2.5.3.40 Privileged Peripheral Memory Frame MasterID Protection Register (PPCS[0-7]MSTID)
Figure 2-111. Privileged Peripheral Memory Frame MasterID Protection Register (PPCSnMSTID)
(offset =5C0h-5DCh)
31 16
PPCS(2n+1)_MSTID
R/WP-FFFFh
15 0
PPCS(2n)_MSTID
R/WP-FFFFh
LEGEND: R/W =Read/Write; WP=Write inprivileged mode only; -n=value after reset
Table 2-125. Privileged Peripheral Memory Frame MasterID Protection Register (PPCSnMSTID)
Field Descriptions
Bit Field Value Description
31-16 PPCS(2n+1)_MSTID MasterID filtering forPPCS[2n+1], where n=0to7.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.
15-0 PPCS(2n)_MSTID MasterID filtering forPPCS[2n], where n=0to7.
0 Read: The corresponding master-ID isnotpermitted toaccess theperipheral.
Write: Disable thepermission ofthecorresponding master toaccess theperipheral.
1 Read: The corresponding master-ID ispermitted toaccess theperipheral.
Write: Enable thepermission ofthecorresponding master toaccess theperipheral.