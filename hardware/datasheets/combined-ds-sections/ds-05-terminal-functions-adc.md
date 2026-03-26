# Terminal Functions - ADC

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 11-13 (3 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 11 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435711TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
4.2.1 ZWT Package
4.2.1.1 Multibuffered Analog-to-Digital Converters (MibADC)
Table 4-1.ZWT Multibuffered Analog-to-Digital Converters (MibADC1, MibADC2)
TERMINAL
SIGNAL
TYPEDEFAULT
PULL STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
AD1EVT /MII_RX_ER/RMII_RX_ER/nTZ1_1 N19 I/O Pulldown Programmable, 20µA 2mA ZD ADC1 event trigger input, orGIO
AD1IN[0] W14 Input - - - ADC1 Input
AD1IN[1] V17 Input - - - ADC1 Input
AD1IN[2] V18 Input - - - ADC1 Input
AD1IN[3] T17 Input - - - ADC1 Input
AD1IN[4] U18 Input - - - ADC1 Input
AD1IN[5] R17 Input - - - ADC1 Input
AD1IN[6] T19 Input - - - ADC1 Input
AD1IN[7] V14 Input - - - ADC1 Input
AD1IN[8]/AD2IN[8] P18 Input - - - ADC1/ADC2 shared Input
AD1IN[9]/AD2IN[9] W17 Input - - - ADC1/ADC2 shared Input
AD1IN[10]/AD2IN[10] U17 Input - - - ADC1/ADC2 shared Input
AD1IN[11]/AD2IN[11] U19 Input - - - ADC1/ADC2 shared Input
AD1IN[12]/AD2IN[12] T16 Input - - - ADC1/ADC2 shared Input
AD1IN[13]/AD2IN[13] T18 Input - - - ADC1/ADC2 shared Input
AD1IN[14]/AD2IN[14] R18 Input - - - ADC1/ADC2 shared Input
AD1IN[15]/AD2IN[15] P19 Input - - - ADC1/ADC2 shared Input
AD1IN[16]/AD2IN[0] V13 Input - - - ADC1/ADC2 shared Input
AD1IN[17]/AD2IN[1] U13 Input - - - ADC1/ADC2 shared Input
AD1IN[18]/AD2IN[2] U14 Input - - - ADC1/ADC2 shared Input
AD1IN[19]/AD2IN[3] U16 Input - - - ADC1/ADC2 shared Input
AD1IN[20]/AD2IN[4] U15 Input - - - ADC1/ADC2 shared Input
AD1IN[21]/AD2IN[5] T15 Input - - - ADC1/ADC2 shared Input
AD1IN[22]/AD2IN[6] R19 Input - - - ADC1/ADC2 shared Input
AD1IN[23]/AD2IN[7] R16 Input - - - ADC1/ADC2 shared Input
AD1IN[24] N18 Input - - - ADC1 Input

<!-- Page 12 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435712TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Table 4-1.ZWT Multibuffered Analog-to-Digital Converters (MibADC1, MibADC2) (continued)
TERMINAL
SIGNAL
TYPEDEFAULT
PULL STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
AD1IN[25] P17 Input - - - ADC1 Input
AD1IN[26] P16 Input - - - ADC1 Input
AD1IN[27] P15 Input - - - ADC1 Input
AD1IN[28] R15 Input - - - ADC1 Input
AD1IN[29] R14 Input - - - ADC1 Input
AD1IN[30] T14 Input - - - ADC1 Input
AD1IN[31] T13 Input - - - ADC1 Input (1)
AD2EVT T10
I/O Pulldown Programmable, 20µA 2mA ZD ADC2 event trigger input, orGIO
MIBSPI3NCS[0]/ AD2EVT /eQEP1I V10(2)
AD2IN[16] W13 Input - - - ADC2 Input
AD2IN[17] W12 Input - - - ADC2 Input
AD2IN[18] V12 Input - - - ADC2 Input
AD2IN[19] U12 Input - - - ADC2 Input
AD2IN[20] T11 Input - - - ADC2 Input
AD2IN[21] U11 Input - - - ADC2 Input
AD2IN[22] V11 Input - - - ADC2 Input
AD2IN[23] W11 Input - - - ADC2 Input
AD2IN[24] V19
Input - - - ADC2 Input
AD2IN[24] W18
ADREFHI V15(3) Input - - - ADC high reference supply
ADREFLO V16(3) Input - - - ADC lowreference supply
MIBSPI3SOMI/ AD1EXT_ENA /ECAP2 V8
Output Pullup 20µA 2mA ZD External Mux ENA
MIBSPI5SOMI[3] /DMM_DATA[15]/I2C2_SCL/ AD1EXT_ENA G16
MIBSPI3SIMO/ AD1EXT_SEL[0] /ECAP3 W8
Output Pullup 20µA 2mA ZD External Mux Select 0
MIBSPI5SIMO[1]/DMM_DATA[9]/ AD1EXT_SEL[0] E16
MIBSPI3CLK/ AD1EXT_SEL[1] /eQEP1A V9
Output Pullup 20µA 2mA ZD External Mux Select 1
MIBSPI5SIMO[2]/DMM_DATA[10]/ AD1EXT_SEL[1] H17
MIBSPI5SIMO[3]/DMM_DATA[11]/I2C2_SDA/ AD1EXT_SEL[2] G17 Output Pullup 20µA 2mA ZD External Mux Select 2
MIBSPI5SOMI[1]/DMM_DATA[13]/ AD1EXT_SEL[3] E17 Output Pullup 20µA 2mA ZD External Mux Select 3
MIBSPI5SOMI[2]/DMM_DATA[14]/ AD1EXT_SEL[4] H16 Output Pullup 20µA 2mA ZD External Mux Select 4

<!-- Page 13 -->
Copyright ©2014 -2016, Texas Instruments Incorporated Terminal Configuration andFunctions
Submit Documentation Feedback
Product Folder Links: TMS570LC435713TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Table 4-1.ZWT Multibuffered Analog-to-Digital Converters (MibADC1, MibADC2) (continued)
TERMINAL
SIGNAL
TYPEDEFAULT
PULL STATEPULL TYPEOUTPUT
BUFFER
DRIVE
STRENGTHDESCRIPTION
SIGNAL NAME337
ZWT
VCCAD W15 (3) Input - - - Operating supply forADC
VSSAD W16 (3) Input - - - ADC supply ground
VSSAD W19 (3) Input - - - ADC supply ground
(1)This ADC channel isalso multiplexed with aninternal temperature sensor.
(2)This isthesecondary terminal atwhich thesignal isalso available. See Section 4.2.2.2 formore detail onhow toselect between theavailable terminals forinput functionality.
(3)The ADREFHI, ADREFLO, VCCAD, andVSSAD connections arecommon forboth ADC cores.