# Revision History

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 7-8 (2 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 7 -->
7TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Revision History Copyright ©2014 -2016, Texas Instruments Incorporated2Revision History
This data manual revision history highlights thetechnical changes made totheSPNS195B device-specific
data manual tomake itanSPNS195C revision. These devices arenow intheProduction Data (PD) stage
ofdevelopment.
Changes from January 31,2016 toJune 25,2016 (from BRevision (January 2016) toCRevision) Page
*Global: Updated/Changed theproduct status from Product Preview toProduction Data................................... 1
*Table 6-13 (LPO Specifications): Updated/Changed LPO -HFoscillator, Untrimmed frequency TYP value from
"9.6"to"9"MHz..................................................................................................................... 78
*Section 6.9.3 (Special Consideration forCPU Access Errors Resulting inImprecise Aborts): Add missing
subsection ............................................................................................................................ 96
*Section 6.14.1 (External Memory Interface (EMIF), Features): Updated/Changed theEMIF asynchronous
memory maximum addressable size from "32KB "to"16MB "each......................................................... 109
*Section 6.14 (External Memory Interface (EMIF)): Added 32-bit access note using a16-bit EMIF interface. ........ 109
*Added "Commonly caused by..."statement forclarification ................................................................. 131
*Table 6-56 (ETMTRACECLK Timing): Restructured timing table formatting tostandards ............................... 149
*Table 7-7,(eCAPx Clock Enable Control): Updated/Changed "ePWM "to"eCAP "intheMODULE INSTANCE
column ............................................................................................................................... 162
*Table 7-11,(eQEPx Clock Enable Control): Updated/Changed "ePWM "to"eQEP "intheMODULE INSTANCE
column ............................................................................................................................... 164
*Table 7-16 (MibADC1 Event Trigger Selection): Added lead-in paragraph referencing theable....................... 166
*(MibADC1 Event Trigger Hookup): NOTE: Added new paragraph ......................................................... 168
*Table 7-17 (MibADC2 Event Trigger Selection): Added lead-in paragraph referencing theable....................... 168
*Section 7.4.2.3 (Controlling ADC1 andADC2 Event Trigger Options Using SOC Output from ePWM Modules):
Updated/Changed thenames ofthefour ePWM signals thatevent trigger theADC.................................... 170
*Table 7-22 (MibADC Operating Characteristics Over 3.0Vto3.6VOperating Conditions): Updated/Changed
the10-and12-bit mode formulas tobesuperscript power of2values .................................................... 175
*Table 7-23 (MibADC Operating Characteristics Over 3.6Vto5.25 VOperating Conditions): Updated/Changed
the10-and12-bit mode formulas tobesuperscript power of2values .................................................... 175

<!-- Page 8 -->
8TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Device Comparison Copyright ©2014 -2016, Texas Instruments Incorporated(1) Foradditional device variants, seewww.ti.com/tms570 .
(2) This table reflects themaximum configuration foreach peripheral. Some functions aremultiplexed andnotallpins areavailable atthe
same time.
(3) Superset device
(4) Total number ofpins thatcanbeused asgeneral-purpose input oroutput when notused aspart ofaperipheral.3Device Comparison
Table 3-1lists thefeatures oftheTMS570LC4357 devices.
Table 3-1.TMS570LC4357 Device Comparison(1)(2)
FEATURES DEVICES
Generic Part Number TMS570LC4357ZWT(3)TMS570LS3137ZWT(3)TMS570LS3135ZWT TMS570LS1227ZWT(3)
Package 337BGA 337BGA 337BGA 337BGA
CPU ARM Cortex-R5F ARM Cortex-R4F ARM Cortex-R4F ARM Cortex-R4F
Frequency (MHz) 300 180 180 180
Cache (KB)32I
32D- - -
Flash (KB) 4096 3072 3072 1280
RAM (KB) 512 256 256 192
Data Flash [EEPROM] (KB) 128 64 64 64
EMAC 10/100 10/100 - 10/100
FlexRay 2-ch 2-ch 2-ch 2-ch
CAN 4 3 3 3
MibADC
12-bit (Ch)2(41ch) 2(24ch) 2(24ch) 2(24ch)
N2HET (Ch) 2(64) 2(44) 2(44) 2(44)
ePWM Channels 14 - - 14
eCAP Channels 6 - - 6
eQEP Channels 2 - - 2
MibSPI (CS) 5(4x6+2) 3(6+6+4) 3(6+6+4) 3(6+6+4)
SPI(CS) - 2(2+1) 2(2+1) 2(2+1)
SCI(LIN) 4(2with LIN) 2(1with LIN) 2(1with LIN) 2(1with LIN)
I2C 2 1 1 1
GPIO (INT)(4)168(with 16interrupt capable) 144(with 16interrupt capable) 144(with 16interrupt capable) 101(with 16interrupt capable)
EMIF 16-bit data 16-bit data 16-bit data 16-bit data
ETM (Trace) 32-bit 32-bit 32-bit -
RTP/DMM 16/16 16/16 16/16 -
Operating
Temperature-40ºCto125ºC -40ºCto125ºC -40ºCto125ºC -40ºCto125ºC
Core Supply (V) 1.14 V-1.32 V 1.14 V-1.32 V 1.14 V-1.32 V 1.14 V-1.32 V
I/OSupply (V) 3.0V-3.6V 3.0V-3.6V 3.0V-3.6V 3.0V-3.6V