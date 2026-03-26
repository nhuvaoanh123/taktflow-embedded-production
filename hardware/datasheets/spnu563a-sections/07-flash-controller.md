# F021 Level 2 Flash Module Controller (L2FMC)

> **Source:** spnu563a.pdf (TMS570LC43x Technical Reference Manual)
> **Pages:** 338-386

---


<!-- Page 338 -->

338 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)Chapter 7
SPNU563A -March 2018
F021 Level 2Flash Module Controller (L2FMC)
The Flash electrically-erasable programmable read-only memory module isatype ofnonvolatile memory
that hasfastread access times and isable tobereprogrammed inthefield orintheapplication. Italso
allows remapping oftheFlash toRAM spaces inorder tosave onrepeated program/erase cycles. This
chapter describes theLevel 2F021 Flash module controller (L2FMC).
Topic ........................................................................................................................... Page
7.1 Overview ......................................................................................................... 339
7.2 Default Flash Configuration ............................................................................... 340
7.3 EEPROM Emulation Support .............................................................................. 340
7.4 SECDED .......................................................................................................... 341
7.5 Memory Map.................................................................................................... 345
7.6 Power On,Power OffConsiderations .................................................................. 350
7.7 Emulation andSIL3 Diagnostic Modes ................................................................ 350
7.8 Parameter Overlay Module (POM) ....................................................................... 353
7.9 Summary ofL2FMC Errors ................................................................................ 354
7.10 Flash Control Registers ..................................................................................... 355
7.11 POM Control Registers ...................................................................................... 383

<!-- Page 339 -->

www.ti.com Overview
339 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.1 Overview
The F021 Flash isused toprovide non-volatile memory forinstruction execution ordata storage. The
Flash canbeelectrically programmed anderased many times toease code development.
Refer tothefollowing documents forsupport onhow toinitialize andusetheon-chip Flash anditsAPI:
*F021 (Texas Instruments 65nm Flash) Flash APIReference Guide (SPNU501 )
7.1.1 Features
*Symmetric dual port (Port AandPort B)forhigher performance andconcurrent access todifferent
banks from oneormore busmasters.
*Read, program anderase with asingle 3.3Vsupply voltage
*Supports error detection andcorrection
-Single Error Correction andDouble Error Detection (SECDED)
-Error Correction Code (ECC) isevaluated intheCPU.
-Address bitsincluded inECC calculation
*Provides different read modes tooptimize performance andverify theintegrity ofFlash contents
*Provides software controllable power mode control logic
*Integrated program/erase state machine
-Simplifies software algorithms
-Supports simultaneous read access onuptotwobanks while performing awrite orerase operation
onanyoneoftheremaining banks
-Suspend command allows read access toasector being programmed/erased
-Fast erase andprogram times (fordetails, seethedevice-specific data sheet
*Allows remapping ofFlash toRAM spaces through "Parameter Overlay Module" (POM)
Fortheactual size oftheFlash memory forthedevice, seethedevice-specific data sheet.
7.1.2 Definition ofTerms
Terms used inthisdocument have thefollowing meaning:
*bw-Normal data space bank data width ofaFlash bank. The bwis256bits(288 bitsincluding the
error correction bits).
*bwe -EEPROM emulation bank is64-bit wide (72bitsincluding theerror correction bits).
*Bus Master -Any ofCPU, DMA orother modules which canrequest data access.
*Charge pump: Voltage generators andassociated control (logic, oscillator, andbandgap, forexample).
*CSM: Program/erase command state machine
*Flash bank: Agroup ofFlash sectors thatshare input/output buffers, data paths, sense amplifiers, and
control logic.
*FEE -Flash EEPROM Emulation. Features ontheL2FMC tosupport using aFlash type memory in
place ofanEEPROM Flash memory. EEPROM iserasable bytheword while thisFlash memory is
only erasable bythesector. The FEE bank isaccessible through thesame busasthemain bank (ina
special address range) andalways resides inbank 7.
*Flash module: Flash banks, charge pump, andFlash wrapper.
*Flash wrapper: Power andmode control logic, data path, wait logic, andwrite/erase state machines.
*L2FMC -Level 2Flash Module Controller.
*Command -Asequence ofcoded instructions toFlash module toexecute acertain task.
*FSM (Flash State Machine) -State machine thatparses anddecodes FSM commands. Itexecutes
embedded algorithms andgenerates control signals toboth Flash bank andcharge pump during the
actual program/erase operation.

<!-- Page 340 -->

Overview www.ti.com
340 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)*OTP (one-time programmable): Aprogram-only-once Flash sector (cannot beerased)
*Sector: Acontiguous region ofFlash memory thatmust beerased simultaneously.
*Wide_Word -thewidth ofthedata output from theFlash bank. This is288-bits wide formain Flash
banks and72-bits wide fortheFEE bank.
*Prefetch Mode -Provides higher performance byfetching thesubsequent cache lineahead ofthe
actual request.
*Read Margin 1mode: More stringent read mode designed forearly detection ofmarginally erased bits.
*Read Margin 0mode: More stringent read mode designed forearly detection ofmarginally
programmed bits.
*Implicit read -Atstartup theL2FMC performs multiple automatic reads from OTP toread device
settings.
*Bus Error -L2FMC willgenerate abuserror tothebusmaster oncertain accesses forexample, writes
toFlash onPort A/Port Boraccess toaddresses beyond theavailable Flash space.
*POM -Parameter Overlay Module provides amethod toremap theFlash when there isaneed tohave
different values intheFlash contents without actually erasing andreprogramming theFlash.
7.1.3 F021 Flash Tools
Texas Instruments provides thefollowing tools forF021 Flash:
*nowECC Generation Tool -togenerate theFlash ECC from theFlash data.
*UNIFLASH Programming Tool -toerase/program/verify thedevice Flash content through JTAG.
*Code Composer Studio -thedevelopment environment with integrated Flash programming capabilities.
*F021 Flash APILibrary -asetofsoftware peripheral functions toprogram/erase theFlash module.
Refer toF021 Flash APIReference Guide (SPNU501 )formore information.
7.2 Default Flash Configuration
Atpower up,theFlash module state exhibits thefollowing properties:
*Wait states aresetto1data wait state. Animplicit address wait states aresetto1andcannot be
changed.
*Prefetch mode isenabled
*The Flash content isprotected from modification
*Power modes aresettoActive (nopower savings)
*The boot code must initialize thewait states andthedesired prefetch mode byinitializing the
FRDCNTL register toachieve theoptimum system performance. This needs tobedone before
switching tothefinal device operating frequency.
7.3 EEPROM Emulation Support
Several features oftheL2FMC support EEPROM emulation. They arelisted here.
*Inorder toallow zeroing outused portions ofFlash when thetable hastobemoved toanew block,
L2FMC allows replacing theall-zero ECC with thecorrect ECC value ofanall-zero 64bdata. This is
enabled bysetting theEE_FEDACCTRL1.EZCV bitataddress offset 8h.
*Similarly, inorder tobeable toread theFlash after successfully erasing it,L2FMC willcompute the
correct ECC forall-ones 64bdata. This isenabled bysetting theEE_FEDACCTRL1.EOCV bitat
address offset 8h.
*Normally, forECC tocorrectly work allthe64bofdata must beprogrammed intotheFlash. However, it
isnotuncommon toprogram partial words intheEEPROM Emulation bank. Inorder toallow this,
L2FMC provides theFEDACSDIS andFEDACSDIS2 registers thatidentifies upto4chosen sectors
where partial words may beprogrammed. Insuch acase, L2FMC computes ECC ontheflyforthese
sectors thus avoiding anyerrors.

<!-- Page 341 -->

www.ti.com SECDED
341 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.4 SECDED
The Flash memory canbeprotected bySingle Error Correction Double Error Detection (SECDED). This
protection isenabled bytheSECDED circuit inside ofthebusmaster.
7.4.1 SECDED Initialization
Flash error detection andcorrection isenabled atreset.
The ECC values foralloftheFlash memory space (Flash banks 0through 6)must beprogrammed into
theFlash before theprogram/data canberead. This canbedone bygenerating thecorrect values ofthe
ECC with anexternal toolsuch asnowECC ormay begenerated bytheprogramming tool. The Cortex
R5F CPU may generate speculative fetches toanylocation within theFlash memory space. Aspeculative
fetch toalocation with invalid ECC, which issubsequently notused, willnotcreate anabort, butwillset
theESM flags foracorrectable oruncorrectable error. Anuncorrectable error willunconditionally cause
thenERROR pintotoggle low. Therefore care must betaken togenerate thecorrect ECC fortheentire
Flash space including theholes between sections andanyunused orblank Flash areas.
The Cortex R5F CPU does notgenerate speculative fetches intotheaddress space ofbank 7,the
EEPROM Emulation Flash. Therefore, itisonly necessary toinitialize theECC values ofthelocations that
willbeintentionally read bytheCPU orother busmasters.
7.4.2 ECC Encoding
Twenty-nine address lines arealso included intheECC calculation. Afailure ofasingle address line
inside ofthebank willresult inanuncorrectable error atthebusmaster. The ECC encoding isshown in
Table 7-1.

<!-- Page 342 -->

SECDED www.ti.com
342 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)Table 7-1.ECC Encoding forBE32 Devices
9
29
19
08
98
88
78
68
58
48
38
28
18
07
97
87
77
67
57
47
37
27
17
06
96
86
76
66
56
4
Participating Address Bits
ADDR_MSW_LSW E
C
C3
13
02
92
82
72
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
00
90
80
70
60
50
40
3
1FC0007F_00FFFF00
_FF0000FF7xxxxxxx xxxxxxx
3FFF80_FF0000FF
_FF0000FF6 xxxxxxxxxxxxxxx
1FC07F80_00FF00FF
_00FF00FF5xxxxxxx xxxxxxxx
FC19F83_FCC0FCC0
_FCC0FCC04 xxxxxx xx xxxxxx xx
13C6A78D_E338E338
_E338E3383x xxxx xx x x xxxx xx x
14DAA9B5_99A699A6
_99A699A62x x xx xx x x x x xx xx x x
1D68BAD1_57155715
_571557151xxx x xx x x xxx x xx x x
A7554EA_D1B4D1B4
_2E4B2E4B0 x x xxx x x x x x xxx x x
Participating Data Bits
6
36
26
16
05
95
85
75
65
55
45
35
25
15
04
94
84
74
64
54
44
34
24
14
03
93
83
73
63
53
43
33
23
13
02
92
82
7
xxxxxxxxxxxxxxxx xxxxx
xxxxxxxx xxxxxxxxxxxxx
xxxxxxxx xxxxxxxx
xxxxxx xx xxxxxx xx xxxxx
xxx xx xxx xxx xx xxx xxx
x xx xx x xx x xx xx x xx x xx
x x xxx x x x x x xxx x x x x x
xx x xx xx x xx x xx xx x x x
(1)ForOdd parity, XOR a1totherow'sXOR result. Foreven Parity, usetherow'sXOR result directly.
(2)Each ECC[x] bitrepresents theXOR ofalltheaddress anddata bitsmarked with xinthesame row.Participating Data Bits Parity(1)Check Bits(2)
2
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
00
90
80
70
60
50
40
30
20
10
0
xxx xxxxxxxx Even ECC[7]
xxx xxxxxxxx Even ECC[6]
xxxxxxxx xxxxxxxx Even ECC[5]
x xx xxxxxx xx Even ECC[4]
xx xxx xxx xx xxx Odd ECC[3]
xx x xx x xx xx x xx Odd ECC[2]
xxx x x x x x xxx x x x Even ECC[1]
xx x x xx x xxx x x xx Even ECC[0]

<!-- Page 343 -->

www.ti.com SECDED
343 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.4.3 Syndrome Table: Decode toBitinError
The syndrome isan8-bit value thatdecodes tothebitinerror. The bitinerror canbeabitamong the64
data bitsorabitamong the8ECC check bits. Asyndrome value of0000 0000 indicates there isnoerror.
Any other syndrome combinations notshown inthetable areuncorrectable multi-bit error. Errors ofthree
ofmore bitsmay escape detection. The syndrome decoding isshown inTable 7-2.
Table 7-2.Syndrome Table
Data BitError Position
6
36
26
16
05
95
85
75
65
55
45
35
25
15
04
94
84
74
64
54
44
34
24
14
03
93
83
73
63
53
43
33
23
13
02
92
82
7
0000000011111111111111110000000011111
1111111100000000000000001111111111111
1111111100000000111111110000000011111
1100000011111100110000001111110011000
0011100011100011001110001110001100111
1010011010011001101001101001100110100
0001010101010111000101010101011100010
1011010011010001101101001101000101001
Data BitError Position ECC Error Bit
2
62
52
42
32
22
12
01
91
81
71
61
51
41
31
21
11
00
90
80
70
60
50
40
30
20
10
00
70
60
50
40
30
20
10
0
11100000000000000001111111110000000Bit[7]
11100000000000000001111111101000000Bit[6]
11100000000111111110000000000100000Bit[5]
00011111100110000001111110000010000Bit[4]
00011100011001110001110001100001000Bit[3]
11010011001101001101001100100000100Bit[2]
10101010111000101010101011100000010Bit1]
01100101110010010110010111000000001Bit[0]

<!-- Page 344 -->

SECDED www.ti.com
344 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.4.4 Syndrome Table: AnAlternate Method
Table 7-3.Alternate Syndrome Table
Syndrome
LSB: 3:0Syndrome MSB 7:4
0x 1x 2x 3x 4x 5x 6x 7x 8x 9x Ax Bx Cx Dx Ex Fx
x0 good E04 E05 D E06 D D D62 E07 D D D46 D M M D
x1 E00 D D D14 D M M D D M M D M D D D30
x2 E01 D D M D D34 D56 D D D50 D40 D M D D M
x3 D D18 D08 D M D D M M D D M D D02 D24 D
x4 E02 D D D15 D D35 D57 D D D51 D41 D M D D D31
x5 D D19 D09 D M D D D63 M D D D47 D D03 D25 D
x6 D D20 D10 D M D D M M D D M D D04 D26 D
x7 M D D M D D36 D58 D D D52 D42 D M D D M
x8 E03 D D M D D37 D59 D D D53 D43 D M D D M
x9 D D21 D11 D M D D M M D D M D D05 D27 D
xA D D22 D12 D D33 D D M D49 D D M D D06 D28 D
xB D17 D D M D D38 D60 D D D54 D44 D D01 D D M
xC D D23 D13 D M D D M M D D M D D07 D29 D
xD M D D M D D39 D61 D D D55 D45 D M D D M
xE D16 D D M D M M D D M M D D00 D D M
xF D M M D D32 D D M D48 D D M D M M D
*E0x -Single-bit ECC error, correctable
*Dxx -Single-bit data error, correctable
*D-Double-bit error, uncorrectable
*M-Multi-bit errors, uncorrectable

<!-- Page 345 -->

64 - bit data word 0 0xF02000000xF02000080xF02000100xF02000180xF0200020
64 - bit data word 164 - bit data word 264 - bit data word 364 - bit data word 464 - bit data word 5 0xF0200028
ECC0 0xF0100000ECC1 0xF0100001ECC2 0xF0100002ECC3ECC4ECC5
0xF01000030xF01000040xF0100005Big Endian
8-bit Read
64- bit data word 0 0x000000000x000000080x000000100x000000180x00000020
64- bit data word 164- bit data word 264- bit data word 364- bit data word 464- bit data word 5 0x00000028
ECC0 0xF0400000ECC1 0xF0400001ECC2 0xF0400002ECC3ECC4ECC5
0xF04000030xF04000040xF0400005Big Endian
8-bit Read
ECC0 0xF0400000 ECC1ECC2 0xF0400002 ECC3ECC4 0xF0400004 ECC516-bit Read
0xF04000000xF040000432-bit Read
ECC0 ECC1 ECC2 ECC3ECC4 ECC5 ECC6 ECC7
www.ti.com Memory Map
345 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.5 Memory Map
The Flash module contains theprogram memory, which ismapped starting atlocation 0,andone
Customer OTP sector andoneTIOTP sector perbank. The Customer OTP sectors may beprogrammed
bythecustomer, butcannot beerased. They aretypically blank innew parts. The TIOTP sectors are
used tocontain manufacturing information. They may beread bythecustomer butcannotbe
programmed orerased. The TIOTP sectors contain settings used bytheFlash APItosetup theFlash
state machine forerase andprogram operations.
Allofthese OTP regions arememory-mapped tofacilitate ease ofaccess bytheCPU. They arememory-
mapped toanoffset starting atF000 0000h intheCPUs memory map.
The RWAIT value isused todefine thenumber ofwait states fortheprogram memory Flash. The EWAIT
value isused todefine thenumber ofwait states forthedata Flash inbank 7.Bank 7starting atoffset
F020 0000h isdedicated fordata storage such asEEPROM Emulation.
7.5.1 Location ofFlash ECC Bits
The ECC bitsarepacked intheir memory space asshown inFigure 7-1andFigure 7-2.
NOTE: Unlike previous versions ofthismodule, alltheECC bytes corresponding totheaddress and
size ofaccess arereturned. Forexample, ifaLoad Multiple (LDM) was used tofetch
32bytes ofECC, alloftheactual bytes corresponding totherange oftheaccess are
returned. There isnoreplication ofthebytes returned.
Figure 7-1.ECC Organization forBank 0-1(288-Bits Wide)
Figure 7-2.ECC Organization forBank 7(72-Bits Wide)

<!-- Page 346 -->

Memory Map www.ti.com
346 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.5.2 OTP Memory
7.5.2.1 Flash Bank andSector Sizes
Flash Bank/Sectoring information canbedetermined from thedevice-specific datasheet orcanbe
computed byreading locations intheTIOTP andL2FMC registers.
The number ofbanks, which banks areavailable, andthenumber ofsectors forbank 0canberead from
TIOTP location F008 0158h asshown inFigure 7-3anddescribed inTable 7-4.
Figure 7-3.TIOTP Bank 0Sector Information
31 24 23 16
Reserved BX_NUM_Sectors
R R
15 8 7 0
B7 B6 B5 B4 B3 B2 B1 B0 NUM_Banks
R-1 R-0 R-0 R-0 R-0 R-0 R-1 R-1 R
LEGEND: R=Read only
Table 7-4.TIOTP Bank 0Sector Information Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reserved. Allbitswillberead as0.
23-16 BX_NUM_Sectors 1-32 Number ofsectors inthisbank.
15 B7 1 1=Bank 7ispresent
14 B6 0 0=Bank 6isnotpresent
13 B5 0 0=Bank 5isnotpresent
12 B4 0 0=Bank 4isnotpresent
11 B3 0 0=Bank 3isnotpresent
10 B2 0 0=Bank 2isnotpresent
9 B1 1 1=Bank 1ispresent
8 B0 1 1=Bank 0ispresent
7-0 NUM_Banks 3 Number ofbanks onthispart.
The bank sector information isrepeated once foreach bank inthedevice. The number ofsectors is
unique foreach bank. The number ofbanks andwhich banks areimplemented isrepeated ineach
location. Use theTIOTP information forbank 0todetermine which banks areinthedevice, andthen read
thenumber ofsectors foreach bank using theTIOTP locations shown inTable 7-5.
Table 7-5.TIOTP Sector Information Address
Bank TIOTP Address
0 F008 0158h
1 F008 2158h
2 F008 4158h
3 F008 6158h
4 F008 8158h
5 F008 A158h
6 F008 C158h
7 F008 E158h

<!-- Page 347 -->

www.ti.com Memory Map
347 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.5.2.2 Package andMemory Size
Package andmemory size information canbedetermined from thedevice-specific datasheet, orcanbe
computed byreading locations intheTIOTP Bank 0registers.
The package andmemory size canberead from TIOTP location F008 015Ch asshown inFigure 7-4and
described inTable 7-6.
Figure 7-4.TIOTP Bank 0Package andMemory Size Information
31 28 27 16
Reserved PACKAGE
R R
15 0
MEMORY_SIZE
R
LEGEND: R=Read only
Table 7-6.TIOTP Bank 0Package andMemory Size Information Field Descriptions
Bit Field Description
31-28 Reserved Reserved
27-16 PACKAGE Count ofpins inthepackage.
15-0 MEMORY_SIZE Flash memory size inKbytes.
7.5.2.3 LPO Trim andMax HCLK
The HFLPO trimsolution, LFLPO trimsolution andmaximum GCLK1 frequency canberead from TI
OTP location F008 01B4h asshown inFigure 7-5anddescribed inTable 7-7.
Figure 7-5.TIOTP Bank 0LPO Trim andMax HCLK Information
31 24 23 16
HFLPO_TRIM LFLPO_TRIM
R R
15 0
MAX_GCLK
R
LEGEND: R=Read only
Table 7-7.TIOTP Bank 0LPO Trim andMax HCLK Information Field Descriptions
Bit Field Description
31-24 HFLPO_TRIM HFLPO Trim Solution
23-16 LFLPO_TRIM LFLPO Trim Solution
15-0 MAX_GCLK Maximum GCLK1 Speed

<!-- Page 348 -->

Memory Map www.ti.com
348 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.5.2.4 Part Number Symbolization
Device part number symbolization information canbedetermined from thedevice-specific data manual or
canbecomputed byreading locations intheTIOTP bank 0registers.
Forexample, thedevice part number symbolization "TMS570LC4357AZWTQQ1" canberead from TI
OTP bank 0location F008 01E0h through F008 01FFh asshown inFigure 7-6.The part number isstored
asanullterminated ASCII string.
Figure 7-6.TIOTP Bank 0Symbolization Information (F008 01E0h-F008 01FFh)
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F
0x54 0x4D 0x53 0x35 0x37 0x30 0x4C 0x43 0x34 0x33 0x35 0x37 0x41 0x5A 0x57 0x54
R
0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F
0x51 0x51 0x31 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
R
LEGEND: R=Read only
7.5.2.5 Temperature Sensor
There arethree temperature sensors thatcanbeused toread theinternal junction temperature onthis
device. The temperature sensors areconnected totheADC converter. See Section 6.5.13 forinformation
onhow toselect thetemperature sensors. During device testing, thevalue read from theADC along with
thejunction temperature ofthesilicon was recorded intheOTP atthree different temperatures. These
values canberead from TIOTP starting atlocation F008 0310h asshown inFigure 7-7anddescribed in
Table 7-8.The values recorded were measured with ADREFHI equal to3.3V.
Figure 7-7.TIOTP Bank 0Temperature Sensor 1Calibration Information (F008 0310h-F008 031Fh)
0x00 0x02 0x04 0x06 0x08 0x0A 0x0C 0x0E
S1TEMP1VAL S1TEMP1 S1TEMP2VAL S1TEMP2 S1TEMP3VAL S1TEMP3 0xFFFF 0xFFFF
R R R R R R R R
LEGEND: R=Read only
Figure 7-8.TIOTP Bank 0Temperature Sensor 2Calibration Information (F008 0320h-F008 032Fh)
0x00 0x02 0x04 0x06 0x08 0x0A 0x0C 0x0E
S2TEMP1VAL S2TEMP1 S2TEMP2VAL S2TEMP2 S2TEMP3VAL S2TEMP3 0xFFFF 0xFFFF
R R R R R R R R
LEGEND: R=Read only
Figure 7-9.TIOTP Bank 0Temperature Sensor 3Calibration Information (F008 0330h-F008 033Fh)
0x00 0x02 0x04 0x06 0x08 0x0A 0x0C 0x0E
S3TEMP1VAL S3TEMP1 S3TEMP2VAL S3TEMP2 S3TEMP3VAL S3TEMP3 0xFFFF 0xFFFF
R R R R R R R R
LEGEND: R=Read only

<!-- Page 349 -->

www.ti.com Memory Map
349 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)Table 7-8.TIOTP Bank 0Temperature Sensor Calibration Information Field Descriptions
Address Width Field Description
F008 03x0h 16bits SxTEMP1VAL The value read from theADC forthissensor atthefirstcalibration temperature.
F008 03x2h 16bits SxTEMP1 The temperature indegrees Kelvin.
F008 03x4h 16bits SxTEMP2VAL The value read from theADC forthissensor atthesecond calibration temperature.
F008 03x6h 16bits SxTEMP2 The temperature indegrees Kelvin.
F008 03x8h 16bits SxTEMP3VAL The value read from theADC forthissensor atthethird calibration temperature.
F008 03xAh 16bits SxTEMP3 The temperature indegrees Kelvin.
F008 03xCh 16bits 0xFFFF Reserved
F008 03xEh 16bits 0xFFFF Reserved
7.5.2.6 Deliberate ECC Errors forFMC ECC Checking
Deliberate single-bit anddouble-bit errors have been placed intheOTP forchecking theL2FMC ECC
functionality. Any portion ofthe64bitsinTIOTP bank 0location F008 03F0h through F008 03F7h as
shown inFigure 7-10 willgenerate asingle-bit error. Any portion ofthe64bitsinTIOTP bank 0location
F008 03F8h through F008 03FFh asshown inFigure 7-10 willgenerate adouble-bit error.
Figure 7-10. TIOTP Bank 0Deliberate ECC Error Information
0x00 0x04 0x08 0x0C
0x12345678 0x9ABCDEF1 0x12345678 0x9ABCDEF3
R R R R
LEGEND: R=Read only, *ECC iscalculated forthevalue 0x123456789ABCDEF0

<!-- Page 350 -->

Power On,Power OffConsiderations www.ti.com
350 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.6 Power On,Power OffConsiderations
7.6.1 Error Checking atPower On
Asthedevice iscoming outofthedevice reset sequence, theFlash wrapper reads aconfiguration word
from theTIOTP section ofbank 0.These areknown asImplicit Reads. This isalso readable from abus
master ataddress F008 0140h. During these reads ECC isenabled. Single-bit errors arecorrected and
uncorrectable errors willgenerate anerror event. Accordingly, theIMPLICIT_COR_ERR orthe
IMPLICIT_UNC_ERR bitsintheFEDAC_GBLSTATUS register (offset =1Ch) willgetset.Refer tothe
data manual tofindtheESM group andchannel number onwhich itistriggered.
7.6.2 Flash Integrity atPower Off
Ifpower islostduring aprogramming orerase operation, apower-on reset must beasserted before the
core supply voltage drops below specification. The PORRST pinhasaglitch filter thatmeans thatthe
PORRST pinmust beasserted lowtf(nPORRST) (2µs)before thecore supply drops below VccMIN(1.14V). If
thisrequirement ismet, then thebitsbeing programmed when PORRST goes lowareindeterminate;
however, theother bitsintheFlash arenotdisturbed. Likewise, ifthisrequirement ismet, andPORRST is
asserted while erasing, thesector orsectors being erased willhave indeterminate bits; however, theother
sectors inthesame bank andtheother banks willnotbedisturbed.
7.7 Emulation andSIL3 Diagnostic Modes
7.7.1 System Emulation
During emulation when theSUSPEND signal ishigh, address tagandcommand parity error events are
notgenerated.
7.7.2 Diagnostic Mode
The Flash wrapper canbeputindiagnostic mode toverify various logic. There aremultiple diagnostic
modes supported bythewrapper. Aspecific diagnostic mode isselected viatheDIAGMODE control bits
inthediagnostic control register (FDIAGCTRL), aslisted inTable 7-9.
The diagnostic mode isonly enabled bya4-bit keystored intheDIAG_EN_KEY bitsinFDIAGCTRL
register. Only DIAG_EN_KEY =5henables anydiagnostic mode andalldiagnostic modes usethe
DIAG_TRIG bitinFDIAGCTRL register toinitiate theaction.
Forallmodes itisbest tofollow thissequence:
1.Write 5htotheDIAG_EN_KEY bitsandsetthedesired DIAGMODE control bits.
2.Setanydata registers needed forthismode.
3.Write a1totheDIAG_TRIG bittoinitiate theaction andallow anerror tohappen.
4.Write aAhtotheDIAG_EN_KEY bitstodisable thediagnostic modes.
Table 7-9.DIAGMODE Encoding
Mode DIAGMODE Bits Description
0 0 0 0 Diagnostic mode isdisabled. Same asDIAG_EN_KEY notequal to5h.
5 1 0 1 Address Tag Register testmode
7 1 1 1 ECC Data Correction Diagnostic testmode
Others Other Combinations Reserved

<!-- Page 351 -->

www.ti.com Emulation andSIL3 Diagnostic Modes
351 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.7.2.1 Address TagRegister Test Mode: DIAGMODE =5
There aresixsets ofaddress tagregisters, twoforPort Aandfour forPort B.Each setconsists ofa
primary andaduplicate address tagregisters. Normally, these registers store therecently issued CPU
addresses during prefetch mode. Todetect errors inthese registers, theprimary andduplicate address
tagregisters arecontinuously compared toeach other ifthebuffer isvalid. Ifthey aredifferent, then an
address tagregister error event isgenerated.
These registers arememory-mapped. Allprimary address tagregisters arememory-mapped toone
address and, likewise, allduplicate tagregisters aremapped toanother single address. During diagnostic
mode, each individual setcanbeselected bytheDIAG_BUF_SEL (Diagnostic Buffer Select) bitinthe
FDIAGCTRL register. User-supplied values canbewritten intotheselected setduring adiagnostic mode.
This diagnostic mode uses theFRAW_ADDR register tosupply thealternate address. When the
DIAG_TRIG bitisset,theFRAW_ADDR register value iscompared with theprimary andtheduplicate
address tagregisters. Iftheresults ofthecomparison aredifferent, then theADD_TAG_ERR (Address
Tag Error) flagintheFEDAC_PxSTATUS register willbeset.Also, refer tothedevice data manual forthe
specific error channel thatwillbeasserted inthissituation.
The sequence todothistestwould be:
1.Branch toanon-Flash region forexecuting thissequence. Ensure norequests from anybusmaster
arearriving attheport (AorB)thatisbeing diagnosed.
2.SetDIAGMODE to5handDIAG_EN_KEY to5hintheFDIAGCTRL register.
3.Select theappropriate buffer tobediagnosed using theDIAG_BUF_SEL bitsintheFDIAGCTRL
register using thetable inSection 7.10.23 .
4.SettheFRAW_ADDR register toacertain arbitrary value 'A'.The lowest 5bitsshould becleared to0.
5.SettheFPRIM_ADD_TAG register andtheFDUP_ADD_TAG register insuch away thatoneofthem
equals 'A'andtheother onedoes not.The lowest 5bitsinboth these writes should becleared to0.
6.SettheDIAG_TRIG bitintheFDIAGCTRL register.
7.Now check theappropriate ADD_TAG_ERR flagintheFEDAC_PxSTATUS register based ontheport
being diagnosed. Ensure thatitis1,implying successful operation ofthecompare logic.
8.Write 1totheADD_TAG_ERR bittoclear it.
9.Repeat forthedifferent buffers.
10.Attheendofthetest, clear DIAGMODE bitto0andsetDIAG_EN_KEY bitstoAhintheFDIAGCTRL
register tocompletely disable thetest.
Alladdress tags andbuffer valid bitswillbecleared to0when leaving diag_mode 5.
NOTE: You should pre-load theregisters with thetestvalues with DIAG_TRIG =0.After alltest
values arewritten, theDIAG_TRIG should then besethigh tovalidate thediagnostic result.

<!-- Page 352 -->

Emulation andSIL3 Diagnostic Modes www.ti.com
352 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.7.2.2 ECC Data Correction Diagnostic Test Mode: DIAGMODE =7
Testing theerror correction andECC logic intheCPU involves corrupting theECC value returned tothe
CPU. Byinverting oneormore bitsoftheECC, theCPU willdetect errors inaselected data orECC bit,or
inanypossible value returned bytheECC.
Tosetanerror foraparticular bitusethesyndrome, seeSection 7.4.3 .Forexample, ifyouwant to
corrupt data bit62then putthevalue 70hintothetestregister.
The method uses theFEMU_ECC andtheFEMU_DxSW registers toalter theECC anddata,
respectively, foroneFlash access port read. The values intheFEMU_ECC andFEMU_DxSW registers
willbeXORed with thecurrent ECC anddata, respectively, togive abadECC ordata value back tothe
busmaster. This willonly occur foroneread when theDIAGMODE is7h,theDIAG_EN_KEY is5h,and
theDIAG_TRIG iswritten with value of1intheFDIAGCTRL register.
The sequence todothistestis:
1.Branch toanon-Flash region toexecute thissequence.
2.SetDIAGMODE to7handDIAG_EN_KEY to5hintheFDIAGCTRL register.
3.Setdesired values toXOR intheFEMU_ECC andFEMU_DxSW registers.
4.SetDIAG_TRIG to1intheFDIAGCTRL register.
5.Select theappropriate port inwhich theflipisdesired using theDIAG_BUF_SEL bitsinthe
FDIAGCTRL register. Only legal values are0forport Aand4hforport B.
6.Doaport AorBread tothedesired address. The L2FMC willXOR thedata andECC with
FEMU_DxSW andFEMU_ECC, respectively, forthisread before delivering ittotheCPU. Nofurther
reads areaffected bythisdiagnostic.
7.The error routine ofthebusmaster (forexample, CPU) shall cause theaddress anderroneous bitto
beknown. This should match with thebitflipped instep 3.
8.Repeat asnecessary totestoutvarious bitsofdata andECC.
9.Clear DIAGMODE to0andsetDIAG_EN_KEY toAhintheFDIAGCTRL register tocompletely disable
thistest.
NOTE: Make sure theaddress tobeused fordiagnostic isnotalready cached; otherwise, theread
willread from thecache memory instead oftheFlash.
7.7.3 Diagnostic Mode Summary
Table 7-10 gives asummary oftheinput registers needed foreach mode, thepossible registers thatcan
change, andthepossible error bitsinFEDACSTATUS thatmay set.
Table 7-10. Diagnostic Mode Summary
DIAG
MODE Name Inputs Possible Outputs Possible Error Bits Set Notes
5 Address Tag
Register test
modeFPRIM_ADD_TAG ADD_TAG_ERR in
FEDAC_PxSTATUS
registerThis willcause ESM
error. Please refer tothe
data manual tofind
group andchannel
number.FDUP_ADD_TAG
FRAW_ADDR
7 ECC Data
Correction
Diagnostic test
modeFEMU_ECC Bus master willindicate
data ECC single-bit or
multi-bit error.None This willcause ESM
error. Please refer tothe
data manual tofind
group andchannel
number.FEMU_DxSW

<!-- Page 353 -->

www.ti.com Emulation andSIL3 Diagnostic Modes
353 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.7.4 SECDED Software Diagnostic
The SECDED block isused toperform error detection andcorrection ontheimplicit reads made after
reset byL2FMC. Tosimplify thediagnostic forthislogic, asoftware mechanism isused. Tocheck thatthe
SECDED module correctly performed itsoperation, thefollowing steps must beused:
1.CPU reads the64-bit memory location oftheimplicit read. Forexample, implicit read location isat
0xF008_0140.
2.Next, CPU reads thememory mapped registers RCR_VALUEx registers accessible ataddress offsets
D0h andD4h.
3.The two64-bit values read insteps 1and2arecompared forbeing equal.
a.Ifthetwovalues areequal, then thelocation inmemory after correction bytheCPU SECDED is
thesame value aslocation inmemory after correction byL2FMC SECDED. Assuming theCPU
SECDED canbeindependently verified, theL2FMC SECDED must befunctioning correctly.
b.Ifthetwovalues arenotequal, then L2FMC SECDED isnotfunctioning correctly.
7.7.5 Read Margin
When thebitsareprogrammed orerased, they arechecked against aprogram_verify orerase_verify
reference level thatisfaraway from thenormal read reference point. Over time, bitlevels may drifttoward
thenormal read point andifitistoomuch then abitwillread thewrong value. Tocounteract this, thebits
canberead using different read_margin reference points togive anearly detection oftheproblem. The
bitscanthen beeither re-programmed (most common) orthesector canbeerased andreprogrammed.
7.8 Parameter Overlay Module (POM)
Inmany applications itisimportant tobeable tochange certain parameters intheprogram without having
tore-flash thedevice andimmediately testthese changes either inahardware-in-the-loop simulation orin
arealenvironment. The Parameter Overlay Module (POM) helps toachieve thisgoal. The POM provides
amechanism toredirect accesses tonon-volatile memory intoavolatile memory thatcanbeinternal to
thedevice orexternal. The data requested bythemaster willbefetched from theoverlay memory instead
ofthemain non-volatile memory. The overlay memory canbeaccessed byother masters inthesystem to
provide aneasy update path ofthestored data. Other masters canbe,forexample, themain CPU, DMA,
DMM, orDAP AHB-AP.
7.8.1 Example Procedure toConfigure thePOM
Suppose theintent istoremap 128KB ofFlash ataddress 10_0000h to8000_0000h. Note thatboth
program region andoverlay region have tobealigned tothesize oftheregion. Sequence toperform this
configuration would beasfollows:
1.Ensure thatthere arenoactive accesses tothisspace while thefollowing configuration isongoing.
2.Write tothePOMGLBCTRL.OTADDR (offset 0h)avalue of200h. These aretheupper 10bits ofthe
overlay region base address.
3.Write tothePOMPROGSTART0.STARTADDRESS (offset 200h) avalue of0x10_0000h.
4.Write tothePOMOVLSTART0.STARTADDRESS (offset 204h) avalue of00_0000h. These arethe
bits21-17 oftheoverlay region address. Since theregion size is128KB thelower bitsdonotmatter.
5.Write tothePOMREGSIZE0.SIZE (offset 208h) avalue ofCh.
6.Finally write tothePOMGLBCTRL.ON_OFF toAh.
7.End ofsequence.

<!-- Page 354 -->

Summary ofL2FMC Errors www.ti.com
354 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.9 Summary ofL2FMC Errors
Table 7-11. Errors inL2FMC
ScenarioDoes this
error cause a
Bus Error?Does this
error goto
ESM?Isaflagin
L2FMC set?Name ofFlag
Access parity error/Internal parity errors Yes Yes YesFEDAC_PxSTATUS.
ADD_PAR_ERR
Port A/BIdleState parity error No Yes YesFEDAC_PxSTATUS.
MCMD_PAR_ERR
Address tagerror Yes Yes YesFEDAC_PxSTATUS.
ADD_TAG_ERR
Access toFlash space beyond available
sizeYes No No -
Access toFlash while pump/bank arenot
activeYes No No -
Flash Access time-out Yes Yes YesFEDAC_PxSTATUS.
ACC_TOUT
Invalid access toL2FMC (forexample,
writes)Yes No No -
Single-bit Error during Implicit Reads No Yes YesFEDAC_GBLSTATUS.
IMPLICIT_COR_ERR
Uncorrectable Error during Implicit Reads No Yes YesFEDAC_GBLSTATUS.
IMPLICIT_UNC_ERR
Access tobank while program/erase
operations areongoing onthesame bankYes No No -
Access toregister address offsets
between 2C8h and3FFh or4B8h and
7FFhYes No No -
Redirected access toPOM received abus
errorYes No No -
Response ofredirected access toPOM
hasaccess parity errorYes No Yes POMFLG.PERR_Px
POM IdleState parity error No Yes YesFEDAC_PxSTATUS.
MCMD_PAR_ERR
Soft Errors inhigh integrity bitscarrying
Implicit read dataNo Yes YesFEDAC_GBLSTATUS.
RCR_ERR

<!-- Page 355 -->

www.ti.com Flash Control Registers
355 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10 Flash Control Registers
This section details theFlash module registers, summarized inTable 7-12.The Flash module control
registers canonly beread and/or written bytheCPU while inprivileged mode. Each register begins ona
word boundary. Allregisters are32-bit, 16-bit and8-bit accessible. The start address oftheFlash module
isFFF8 7000h.
Table 7-12. Flash Control Registers
Offset Acronym Register Description Section
00h FRDCNTL Flash Read Control Register Section 7.10.1
04h FSPRD Read Margin Control Register Section 7.10.2
08h EE_FEDACCTRL1 EEPROM Error Correction Control Register 1 Section 7.10.3
14h FEDAC_PASTATUS Flash Port AError andStatus Register Section 7.10.4
18h FEDAC_PBSTATUS Flash Port BError andStatus Register Section 7.10.5
1Ch FEDAC_GBLSTATUS Flash Global Error andStatus Register Section 7.10.6
24h FEDACSDIS Flash Error Detection andCorrection Sector Disable
RegisterSection 7.10.7
28h FPRIM_ADD_TAG Flash Primary Address Tag Register Section 7.10.8
2Ch FDUP_ADD_TAG Flash Duplicate Address Tag Register Section 7.10.9
30h FBPROT Flash Bank Protection Register Section 7.10.10
34h FBSE Flash Bank Sector Enable Register Section 7.10.11
38h FBBUSY Flash Bank Busy Register Section 7.10.12
3Ch FBAC Flash Bank Access Control Register Section 7.10.13
40h FBPWRMODE Flash Bank Power Mode Register Section 7.10.14
44h FBPRDY Bank/Pump Ready Register Section 7.10.15
48h FPAC1 Flash Pump Access Control Register 1 Section 7.10.16
50h FMAC Flash Module Access Control Register Section 7.10.17
54h FMSTAT Flash Module Status Register Section 7.10.18
58h FEMU_DMSW EEPROM Emulation Data MSW Register Section 7.10.19
5Ch FEMU_DLSW EEPROM Emulation Data LSW Register Section 7.10.20
60h FEMU_ECC EEPROM Emulation Address Register Section 7.10.21
64h FLOCK Flash Lock Register Section 7.10.22
6Ch FDIAGCTRL Diagnostic Control Register Section 7.10.23
74h FRAW_ADDR Raw Address Section 7.10.24
7Ch FPAR_OVR Parity Override Register Section 7.10.25
B4h RCR_VALID Reset Configuration Valid Register Section 7.10.26
B8h ACC_THRESHOLD Crossbar Access Time Threshold Register Section 7.10.27
C0h FEDACSDIS2 Flash Error Detection andCorrection Sector Disable
Register 2Section 7.10.28
D0h RCR_VALUE0 Lower Word oftheReset Configuration Read Register Section 7.10.29
D4h RCR_VALUE1 Upper Word oftheReset Configuration Read Register Section 7.10.30
288h FSM_WR_ENA FSM Register Write Enable Register Section 7.10.31
2B8h EEPROM_CONFIG EEPROM Emulation Configuration Register Section 7.10.32
2C0h FSM_SECTOR1 FSM Sector Register 1 Section 7.10.33
2C4h FSM_SECTOR2 FSM Sector Register 2 Section 7.10.34
400h FCFG_BANK Flash Bank Configuration Register Section 7.10.35

<!-- Page 356 -->

Flash Control Registers www.ti.com
356 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.1 Flash Read Control Register (FRDCNTL)
FRDCNTL supports prefetch mode. This register controls Flash timings forthemain Flash banks. Forthe
equivalent register thatcontrols Flash timings fortheEEPROM Emulation Flash bank (bank 7),see
Section 7.10.32 .
Figure 7-11. Flash Read Control Register (FRDCNTL) (offset =00h)
31 16
Reserved
R-0
15 12 11 8 7 2 1 0
Reserved RWAIT Reserved PFUENB PFUENA
R-0 R/WP-1 R-0 R/WP-1 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-13. Flash Read Control Register (FRDCNTL) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
11-8 RWAIT 0-Fh Random/data Read Wait State
The random read wait state bitsindicate how many wait states areadded toaFlash read access.
Address wait state isfixed to1HCLK cycle.
Note: The required wait states foreach HCLK frequency canbefound inthedevice-specific data
sheet.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 PFUENB Prefetch Enable forPort B
0 Prefetch Mode isdisabled.
1 Prefetch Mode isenabled. (Recommended)
0 PFUENA Prefetch Enable forPort A
0 Prefetch Mode isdisabled.
1 Prefetch Mode isenabled. (Recommended)

<!-- Page 357 -->

www.ti.com Flash Control Registers
357 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.2 Read Margin Control Register (FSPRD)
This register controls theread margin mode.
NOTE: Ifboth RM0 andRM1 bitsaresetthen Read Margin 0isenabled.
Figure 7-12. Read Margin Control Register (FSPRD) (offset =04h)
31 16
Reserved
R-0
15 8 7 2 1 0
RMBSEL[7:0] Reserved RM1 RM0
R/WP-0 R-0 R/WP-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-14. Read Margin Control Register (FSPRD) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-8 RMBSEL[ n] Read Margin Bank Select. Each bitcorresponds toaFlash bank.
RMBSEL isonly decoded ifeither theRM1 orRM0 bitisset.When either RM1 orRM0 isset,the
RMBSEL bitcorresponding toabank forces theselected bank(s) toberead intheselected margin
mode. The unselected bank(s) arestillread innormal mode.
There must be2accesses tothebank before theread margin takes effect.
7-2 Reserved 0 Reads return 0.Writes have noeffect.
1 RM1 Read Margin 1
0 Read Margin 1Mode isdisabled.
1 Read Margin 1Mode isenabled.
0 RM0 Read Margin 0
0 Read Margin 0Mode isdisabled.
1 Read Margin 0Mode isenabled.

<!-- Page 358 -->

Flash Control Registers www.ti.com
358 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.3 EEPROM Error Correction Control Register (EE_FEDACCTRL1)
When aEEPROM bank iserased orzeroed out,thecontents willbeall1'sorall0's,respectively. Insuch
acase, theECC willbeincorrect. EE_FEDACCTRL1 letstheL2FMC ignore anall1'sandall0's
condition, onreads from theEEPROM bank.
Figure 7-13. EEPROM Error Correction Control Register (EE_FEDACCTRL1) (offset =08h)
31 16
Reserved
R-0
15 6 5 4 3 0
Reserved EOCV EZCV Reserved
R-0 R/WP-0 R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-15. EEPROM Error Correction Control Register (EE_FEDACCTRL1) Field Descriptions
Bit Field Value Description
31-6 Reserved 0 Reads return 0.Writes have noeffect.
5 EOCV One condition valid
0 DONOT allow thecondition ofalldata bitsandECC bitstobe1.
1 Allow thecondition ofalldata bitsandECC bitstobe1.
4 EZCV Zero condition valid
0 DONOT allow thecondition ofalldata bitsandECC bitstobe0.
1 Allow thecondition ofalldata bitsandECC bitstobe0.
3-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 359 -->

www.ti.com Flash Control Registers
359 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.4 Flash Port AError andStatus Register (FEDAC_PASTATUS)
This register applies toaccesses made tothemain orEEPROM Flash banks through Port A.
Allthese error status bitscanbecleared bywriting a1tothebit;writing a0hasnoeffect.
Figure 7-14. Flash Port AError andStatus Register (FEDAC_PASTATUS) (offset =14h)
31 24
Reserved
R-0
23 16
Reserved
R-0
15 14 13 12 11 10 9 8
ACCTOUT MCMD_PAR_
ERRReserved ADD_TAG_
ERRADD_PAR_
ERRReserved
RCP-0 RCP-u R-0 RCP-u RCP-u R-0
7 0
Reserved
R-0
LEGEND: R=Read only; RCP =Read andClear inPrivilege Mode; -u=unchanged value oninternal reset, cleared onpower up;-n=
value after reset
Table 7-16. Flash Port AError andStatus Register (FEDAC_PASTATUS)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 ACCTOUT Severe internal switch timeout/parity error onPort Aaccess.
0 L2FMC internal switch hasNOT encountered asevere error (access timeout orparity).
1 L2FMC internal switch hasencountered asevere error (access timeout orparity).
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
14 MCMD_PAR_ERR Parity Error inidlestate. This bitissetwhen aparity error occurs during idlestate ofPort
A.
0 Noidlestate parity error isdetected.
1 Parity error isdetected inidlestate.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
13-12 Reserved 0 Reads return 0.Writes have noeffect.
11 ADD_TAG_ERR Port AAddress Tag Register Error Flag. This bitissetiftheprimary address taghasahit
buttheduplicate address tagdoes notmatch theprimary address tag.This bitis
functional only when Port Aprefetch mode isenabled (PFUENA =1).
0 Address Tag Register Error notdetected onPort A.
1 Address Tag Register Error detected onPort A.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
10 ADD_PAR_ERR Address Parity Error Flag.
0 Noparity error was detected ontheincoming access totheL2FMC Port A.
1 Aparity error was detected ontheincoming access totheL2FMC Port A.The address of
theerroneous access isnotstored inL2FMC.
This error isrouted totheESM. Refer tothedevice data manual tofindthespecific group
andchannel onwhich itisrouted.
9-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 360 -->

Flash Control Registers www.ti.com
360 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.5 Flash Port BError andStatus Register (FEDAC_PBSTATUS)
This register applies toaccesses made tothemain orEEPROM Flash banks through Port B.
Allthese error status bitscanbecleared bywriting a1tothebit;writing a0hasnoeffect.
Figure 7-15. Flash Port BError andStatus Register (FEDAC_PBSTATUS) (offset =18h)
31 24
Reserved
R-0
23 16
Reserved
R-0
15 14 13 12 11 10 9 8
ACCTOUT MCMD_PAR_
ERRReserved ADD_TAG_
ERRADD_PAR_
ERRReserved
RCP-0 RCP-u R-0 RCP-u RCP-u R-0
7 0
Reserved
R-0
LEGEND: R=Read only; RCP =Read andClear inPrivilege Mode; -u=unchanged value oninternal reset, cleared onpower up;-n=
value after reset
Table 7-17. Flash Port BError andStatus Register (FEDAC_PBSTATUS)
Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15 ACCTOUT Severe error -internal switch timeout.
0 L2FMC internal switch hasNOT encountered asevere error (access timeout).
1 L2FMC internal switch hasencountered asevere error (access timeout).
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
14 MCMD_PAR_ERR Parity Error inidlestate. This bitissetwhen aparity error occurs during idlestate ofPort
B.
0 Noidlestate parity error isdetected.
1 Parity error isdetected inidlestate.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
13-12 Reserved 0 Reads return 0.Writes have noeffect.
11 ADD_TAG_ERR Port BAddress Tag Register Error Flag. This bitissetiftheprimary address taghasahit
buttheduplicate address tagdoes notmatch theprimary address tag.This bitis
functional only when Port Bprefetch mode isenabled (PFUENB =1).
0 Address Tag Register Error notdetected onPort B.
1 Address Tag Register Error detected onPort B.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
10 ADD_PAR_ERR Address Parity Error Flag.
0 Noparity error was detected ontheincoming access totheL2FMC Port B.
1 Aparity error was detected ontheincoming access totheL2FMC Port B.The address of
theerroneous access isnotstored inL2FMC.
This error isrouted totheESM. Refer tothedevice data manual tofindthespecific group
andchannel onwhich itisrouted.
9-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 361 -->

www.ti.com Flash Control Registers
361 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.6 Flash Global Error andStatus Register (FEDAC_GBLSTATUS)
This register applies toglobal error andstatus flags inL2FMC.
Allthese status bitscanbecleared bywriting a1tothebit;writing a0hasnoeffect.
Figure 7-16. Flash Global Error andStatus Register (FEDAC_GBLSTATUS) (offset =1Ch)
31 24
Reserved FSM_DONE
R-0 RCP-0
23 16
Reserved
R-0
15 14 13 12 8
RCR_ERR IMPLICIT_COR_
ERRIMPLICIT_UNC_
ERRReserved
RCP-0 RCP-0 RCP-0 R-0
7 0
Reserved
R-0
LEGEND: R=Read only; RCP =Read andClear inPrivilege Mode; -n=value after reset
Table 7-18. Flash Global Error andStatus Register (FEDAC_GBLSTATUS)
Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 FSM_DONE Flash State Machine Done
This bitissetto1when theFlash state machine completes aprogram orerase operation.
This bitwillgenerate aninterrupt onVIM channel 61iftheFSM_EVT_EN bitofthe
FSM_ST_MACHINE register isset.This bitmust becleared bywriting a1toitinthe
interrupt routine toclear theinterrupt request.
23-16 Reserved 0 Reads return 0.Writes have noeffect.
15 RCR_ERR Soft error inhigh integrity bitscarrying implicit read data.
0 Noerror detected inhigh-integrity bits.
1 Error detected inhigh-integrity bits.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
14 IMPLICIT_COR_ERR Correctable error occurred during implicit reads.
0 Nosingle-bit error isdetected during implicit read.
1 Single-bit error isdetected during implicit read.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
13 IMPLICIT_UNC_ERR Uncorrectable error occurred during implicit reads.
0 Nodouble-bit error isdetected during implicit read.
1 Double-bit error isdetected during implicit read.
This error isrouted totheESM. Refer tothedevice data manual tofindthegroup and
channel onwhich itisrouted.
12-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 362 -->

Flash Control Registers www.ti.com
362 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.7 Flash Error Detection andCorrection Sector Disable Register (FEDACSDIS)
This register isused todisable theSECDED function foroneortwosectors from theEEPROM Emulation
Flash (bank 7).Anadditional twosectors canhave SECDED disabled bytheuseoftheFEDACSDIS2
register (see Section 7.10.28 ).
Figure 7-17. Flash Error Detection andCorrection Sector Disable Register (FEDACSDIS)
(offset =24h)
31 29 24 23 22 21 16
Rsvd SectorID1_inverse Rsvd SectorID1
R-0 R/WP-0 R-0 R/WP-0
15 14 13 8 7 6 5 0
Rsvd SectorID0_inverse Rsvd SectorID0
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-19. Flash Error Detection andCorrection Sector Disable Register (FEDACSDIS)
Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 SectorID1_inverse 0-3Fh The sector IDinverse bitsareused with thesector IDbitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID1.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 SectorID1 0-3Fh The sector IDbitsareused with thesector IDinverse bitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID1.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 SectorID0_inverse 0-Fh The sector IDinverse bitsareused with thesector IDbitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID0.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 SectorID0 0-3Fh The sector IDbitsareused with thesector IDinverse bitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID0.

<!-- Page 363 -->

www.ti.com Flash Control Registers
363 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.8 Primary Address TagRegister (FPRIM_ADD_TAG)
This register isused totesttheprefetch address tagregisters. (see Section 7.7.2.1 )
Figure 7-18. Primary Address TagRegister (FPRIM_ADD_TAG) (offset =28h)
31 16
PRIM_ADD_TAG
R/WP-0
15 5 4 0
PRIM_ADD_TAG Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-20. Primary Address TagRegister (FPRIM_ADD)_TAG Field Descriptions
Bit Field Value Description
31-5 PRIM_ADD_TAG 0-7FF FFFFh Primary Address Tag Register
The primary address tagregister selected bytheDIAG_BUF_SEL bitsinthe
FDIAGCTRL register ismemory-mapped here. This register canonly bewritten in
privileged mode when diagnostic mode isenabled with DIAG_EN_KEY =5hand
DIAGMODE =5hintheFDIAGCTRL register. This register isnotupdated with new
Flash data ifDIAG_EN_KEY isnotequal to5horDIAGMODE is0or7h.Valid reads
canoccur inanymode. The register clears when anaddress tagerror isfound and
when leaving DIAG_MODE 5.
4-0 Reserved 0 Reads return 0.Writes have noeffect.
7.10.9 Duplicate Address TagRegister (FDUP_ADD_TAG)
This register isused totesttheprefetch address tagregisters. (see Section 7.7.2.1 )
Figure 7-19. Duplicate Address TagRegister (FDUP_ADD_TAG) (offset =2Ch)
31 16
DUP_ADD_TAG
R/WP-0
15 5 4 0
DUP_ADD_TAG Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-21. Duplicate Address TagRegister (FDUP_ADD)_TAG Field Descriptions
Bit Field Value Description
31-5 DUP_ADD_TAG 0-7FF FFFFh Duplicate Address Tag Register
The duplicate address tagregister selected bytheDIAG_BUF_SEL bitsinthe
FDIAGCTRL register ismemory-mapped here. This register canonly bewritten in
privileged mode when diagnostic mode isenabled with DIAG_EN_KEY =5hand
DIAGMODE =5hintheFDIAGCTRL register. This register isnotupdated with new
Flash data ifDIAG_EN_KEY isnotequal to5horDIAGMODE is0or7h.Valid reads
canoccur inanymode. The register clears when anaddress tagerror isfound and
when leaving DIAG_MODE 5.
3-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 364 -->

Flash Control Registers www.ti.com
364 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.10 Flash Bank Protection Register (FBPROT)
Figure 7-20. Flash Bank Protection Register (FBPROT) (offset =30h)
31 16
Reserved
R-0
15 1 0
Reserved PROTL1DIS
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-22. Flash Bank Protection Register (FBPROT) Field Descriptions
Bit Field Value Description
31-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PROTL1DIS Level 1Protection Disable bit
Setting thisbitdisables protection from writing totheOTPPROTDIS bitsintheFBAC register as
well astheBSE bitsforallbanks intheFBSE register. Clearing thisbitenables protection and
disables write access totheOTPPROTDIS bitsandFBSE register.
0 Level 1protection isenabled.
1 Level 1protection isdisabled.
7.10.11 Flash Bank Sector Enable Register (FBSE)
FBSE provides oneenable bitpersector forupto16sectors perbank. Each bank intheFlash module
hasoneFBSE register. The bank isselected viatheBANK bitsintheFMAC register. Asonly onebank at
atime canbeselected byFMAC, only theregister forthebank selected appears atthisaddress.
Figure 7-21. Flash Bank Sector Enable Register (FBSE) (offset =34h)
31 16
Reserved
R-0
15 0
BSE[15:0]
R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-23. Flash Bank Sector Enable Register (FBSE) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 BSE[ n] Bank Sector Enable. Each bitcorresponds toaFlash sector inthebank specified bytheFMAC
register. Bit0corresponds tosector 0,bit1corresponds tosector 1,andsoon.These bitscanbe
setonly when PROTL1DIS =1intheFBPROT register andinprivilege mode.
0 The corresponding numbered sector isdisabled forprogram orerase access.
1 The corresponding numbered sector isenabled forprogram orerase access.

<!-- Page 365 -->

www.ti.com Flash Control Registers
365 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.12 Flash Bank Busy Register (FBBUSY)
Figure 7-22. Flash Bank Busy Register (FBBUSY) (offset =38h)
31 16
Reserved
R-0
15 8 7 0
Reserved BUSY[7:0]
R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 7-24. Flash Bank Busy Register (FBBUSY) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 BUSY[ n] Bank Busy. Each bitcorresponds toaFlash bank.
0 The corresponding bank isnotbusy.
1 The corresponding bank isbusy with astate machine operation, orthebank isnotimplemented.
7.10.13 Flash Bank Access Control Register (FBAC)
Figure 7-23. Flash Bank Access Control Register (FBAC) (offset =3Ch)
31 24 23 16
Reserved OTPPROTDIS[7:0]
R-0 R/WP-0
15 8 7 0
BAGP VREADST
R/WP-0 R/WP-Fh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-25. Flash Bank Access Control Register (FBAC) Field Descriptions
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23-16 OTPPROTDIS[ n] OTP Sector Protection Disable. Each bitcorresponds toaFlash bank. This bitcanbesetonly
when PROTL1DIS =1intheFBPROT register andinprivilege mode.
0 Programming oftheOTP sector isdisabled.
1 Programming oftheOTP sector isenabled.
15-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 VREADST 0-FFh VREAD Setup.
VREAD isgenerated bytheFlash pump andused forFlash read operation. The bank power up
sequencing starts VREADST HCLK cycles after VREAD power supply becomes stable.
Note :There isnotaprogrammable Bank Sleep counter andStandby counter register. The number
ofclock cycles totransition from sleep tostandby andstandby toactive ishardcoded intheFlash
wrapper design.

<!-- Page 366 -->

Flash Control Registers www.ti.com
366 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.14 Flash Bank Power Mode Register (FBPWRMODE)
Figure 7-24. Flash Bank Power Mode Register (FBPWRMODE) (offset =40h)
31 16
Reserved
R-505h
15 14 13 4 3 2 1 0
BANKPWR7 Reserved BANKPWR1 BANKPWR0
R/WP-3h R-3FFh R/WP-3h R/WP-3h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-26. Flash Bank Power Mode Register (FBPWRMODE) Field Descriptions
Bit Field Value Description
31-16 Reserved 505h Donotwrite tothese register bits.
15-14 BANKPWR7 Bank 7Power Mode.
0 Bank sleep mode
1h Bank standby mode
2h Reserved
3h Bank active mode
13-4 Reserved 3FFh Donotwrite tothese register bits.
3-2 BANKPWR1 Bank 1Power Mode.
0 Bank sleep mode
1h Bank standby mode
2h Reserved
3h Bank active mode
1-0 BANKPWR0 Bank 0Power Mode.
0 Bank sleep mode
1h Bank standby mode
2h Reserved
3h Bank active mode

<!-- Page 367 -->

www.ti.com Flash Control Registers
367 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.15 Flash Bank/Pump Ready Register (FBPRDY)
FBPRDY register allows youtodetermine ifthepump andbanks areready forperforming aread access.
Figure 7-25. Flash Bank/Pump Ready Register (FBPRDY) (offset =44h)
31 24 23 22 18 17 16
Reserved BANKBUSY[7] Reserved BANKBUSY[1:0]
R-0 R-0 R-1 R-0
15 14 8 7 6 2 1 0
PUMPRDY Reserved BANKRDY[7] Reserved BANKRDY[1:0]
R-1 R-0 R-1 R-0 R-1
LEGEND: R=Read only; -n=value after reset
Table 7-27. Flash Bank/Pump Ready Register (FBPRDY) Register Description
Bit Field Value Description
31-24 Reserved 0 Reads return 0.Writes have noeffect.
23 BANKBUSY[7] Bank 7Busy Status
0 Bank isnotbusy with anyFSM orCPU operation.
1 Bank isbusy with anFSM orCPU operation.
22-18 Reserved 1 Reads return 1.Writes have noeffect.
17-16 BANKBUSY[1:0] Bank 0(bit16)andBank 1(bit17)Busy Status
0 Bank isnotbusy with anyFSM orCPU operation.
1 Bank isbusy with anFSM orCPU operation.
15 PUMPRDY Pump Ready isaread-only bitwhich allows software todetermine ifthepump isready forFlash
access before attempting theactual access. When set,itmeans thatthecharge pump isinactive
power state. Ifanaccess ismade toabank which isnotready then wait states areasserted until it
becomes ready
0 Pump isnotready.
1 Pump isready.
14-8 Reserved 0 Reads return 0.Writes have noeffect.
7 BANKRDY[7] Bank 7Ready Status
0 Bank isnotready forFlash access.
1 Bank isready forFlash access.
6-2 Reserved 0 Reads return 0.Writes have noeffect.
1-0 BANKRDY[1:0] Bank 0(bit0)andBank 1(bit1)Ready Status
0 Bank isnotready forFlash access.
1 Bank isready forFlash access.

<!-- Page 368 -->

Flash Control Registers www.ti.com
368 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.16 Flash Pump Access Control Register 1(FPAC1)
Figure 7-26. Flash Pump Access Control Register 1(FPAC1) (offset =48h)
31 27 26 16
Reserved PSLEEP
R-0 R/WP-C8h
15 1 0
Reserved PUMPPWR
R-0 R/WP-1
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-28. Flash Pump Access Control Register 1(FPAC1) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reads return 0.Writes have noeffect.
26-16 PSLEEP 0-7FFh Pump Sleep.
These bitscontain thestarting count value forthecharge pump sleep down counter. While the
charge pump isinsleep mode, thepower mode management logic holds thecharge pump sleep
counter atthisvalue. When thecharge pump exits sleep power mode, thedown counter delays
from 0toPSLEEP pump sleep down clock cycles before putting thecharge pump intoactive power
mode.
Note: Pump sleep down counter clock isadivide by2input ofHCLK. That is,there are2×HCLK
cycles forevery PSLEEP counter cycle.
15-1 Reserved 0 Reads return 0.Writes have noeffect.
0 PUMPPWR Flash Charge Pump Fallback Power Mode
0 Sleep (allpump circuits aredisabled)
1 Active (allpump circuits areactive)

<!-- Page 369 -->

www.ti.com Flash Control Registers
369 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.17 Flash Module Access Control Register (FMAC)
Figure 7-27. Flash Module Access Control Register (FMAC) (offset =50h)
31 16
Reserved
R-0
15 3 2 0
Reserved BANK
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-29. Flash Module Access Control Register (FMAC) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
2-0 BANK 0-7h Bank Enable.
These bitsselect which bank isenabled foroperations such aslocal register access, OTP sector
access, andprogram/erase commands. These bitsselect only onebank atatime from uptoeight
banks depending onthespecific device being used. Forexample, a000selects bank 0;011selects
bank 3.
Note: BANK canidentify upto8Flash banks. IfBANK isselected foranun-implemented bank,
then theBANK willsetitself tothenumber ofanimplemented bank. Todetermine ifabank is
implemented, write thebank number toBANK andread back thevalue toseeifwhat was written
canberead back.

<!-- Page 370 -->

Flash Control Registers www.ti.com
370 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.18 Flash Module Status Register (FMSTAT)
Figure 7-28. Flash Module Status Register (FMSTAT) (offset =54h)
31 24
Reserved
R-0
23 18 17 16
Reserved RVSUSP RDVER
R-0 R-0 R-0
15 14 13 12 11 10 9 8
RVF ILA DBT PGV PCV EV CV BUSY
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
7 6 5 4 3 2 1 0
ERS PGM INV-DAT CSTAT VOLTSTAT ESUSP PSUSP SLOCK
R-0 R-0 R-0 R-0 R-0 R-0 R-0 R-0
LEGEND: R=Read only; -n=value after reset
Table 7-30. Flash Module Status Register (FMSTAT) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reads return 0.Writes have noeffect.
17 RVSUSP Read Verify Suspend
1 When set,thisbitindicates thattheFlash module hasreceived andprocessed asuspend
command during aread-verify operation. This bitremains setuntil theread-verify-resume command
hasbeen issued ortheClear_More command isrun.
16 RVDER Read verify command currently underway
1 When set,thisbitindicates thattheFlash module isactively performing aread-verify operation.
This bitissetwhen read-verify starts andiscleared when itiscomplete. Itisalso cleared when the
read-verify issuspended andsetwhen theread-verify resumes.
15 RVF Read Verify Failure
1 When set,indicates thataread verify mismatch isdetected using theRead Verify command. This
bitremains setuntil clear_status orclear_more FSM commands arerun.
14 ILA Illegal Address
1 When set,indicates thatanillegal address isdetected. The following conditions cansettheillegal
address flag.
1.Writing toahole (un-implemented logical address space) within aFlash bank.
2.Writing toanaddress location toanun-implemented Flash space.
3.Input address forwrite isdecoded toselect adifferent bank from thebank IDregister.
4.The address range does notmatch thetype ofFSM command. Forexample, theerase_sector
command must match theaddress regions.
5.TI-OTP address selected butCMD_EN inFSM_ST_MACHINE isnotset.
13 DBT Disturbance Test Fail
1 This bitissetduring aProgram Sector command when theFSM firstreads anaddress anditisnot
all1s.
12 PGV Program Verify
1 When set,indicates thataword isnotsuccessfully programmed after themaximum allowed
number ofprogram pulses aregiven forprogram operation.
11 PCV Precondition Verify.
1 When set,indicates thatasector isnotsuccessfully preconditioned (pre-erased) after themaximum
allowed number ofprogram pulses aregiven forprecondition operation foranyapplied command
such asErase Sector command. During Precondition verify command, thisflagissetimmediately if
aFlash bitisfound tobe1.

<!-- Page 371 -->

www.ti.com Flash Control Registers
371 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)Table 7-30. Flash Module Status Register (FMSTAT) Field Descriptions (continued)
Bit Field Value Description
10 EV Erase Verify
1 When set,indicates thatasector isnotsuccessfully erased after themaximum allowed number of
erase pulses aregiven forerase operation. During Erase verify command, thisflagisset
immediately ifabitisfound tobe0.
9 CV Compact Verify
1 When set,indicates thatasector contains oneormore bitsindepletion after anerase operation
with CMPV_ALLOWED set.During compact verify command, thisflagissetimmediately ifabitis
found tobe1.
8 BUSY Busy
1 When set,thisbitindicates thataprogram, erase, orsuspend operation isbeing processed.
7 ERS Erase Active
1 When set,thisbitindicates thattheFlash module isactively performing anerase operation. This bit
issetwhen erasing starts andiscleared when erasing iscomplete. Itisalso cleared when the
erase issuspended andsetwhen theerase resumes.
6 PGM Program Active
1 When set,thisbitindicates thattheFlash module iscurrently performing aprogram operation. This
bitissetwhen programming starts andiscleared when programming iscomplete. Itisalso cleared
when programming issuspended andsetwhen programming isresumes.
5 INVDAT Invalid Data
1 When set,thisbitindicates thattheuser attempted toprogram a1where a0was already present.
This bitiscleared bytheClear Status command.
4 CSTAT Command Status
1 Once theFSM starts anyfailure willsetthisbit.When set,thisbitinforms thehost thatthe
program, erase, orvalidate sector command failed andthecommand was stopped. This bitis
cleared bytheClear Status command. Forsome errors, thiswillbetheonly indication ofanFSM
error because thecause does notfallwithin theother error bittypes.
3 VOLTSTAT Core Voltage Status
1 When set,thisbitindicates thatthecore voltage generator ofthepump power supply dipped below
thelower limit allowable during aprogram orerase operation. This bitiscleared bytheClear Status
command.
2 ESUSP Erase Suspended
1 When set,thisbitindicates thattheFlash module hasreceived andprocessed anerase suspend
operation. This bitremains setuntil theerase resume command hasbeen issued oruntil the
Clear_More command isrun.
1 PSUSP Program Suspended
1 When set,thisbitindicates thattheFlash module hasreceived andprocessed aprogram suspend
operation. This bitremains setuntil theprogram resume command hasbeen issued oruntil the
Clear_More command isrun.
0 SLOCK Sector Lock Status
1 When set,thisbitindicates thattheoperation was halted because thetarget sector was locked for
erasing andprogramming either bythesector protect bitorbyOTP write protection disable bits.
(BSE bitsintheFBSE register orOTPPROTDIS bitsintheFBAC register). This bitiscleared by
theClear Status command.
NoSLOCK FSM error willoccur ifallsectors inabank erase operation aresetto1.Allthesectors
willbechecked butnoSLOCK willbesetifnooperation occurs duetotheSECT_ERASED bits
being settoall1s.ASLOCK error willoccur ifattempting todoasector erase with either BSE is
cleared orSECT_ERASED isset.

<!-- Page 372 -->

Flash Control Registers www.ti.com
372 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.19 EEPROM Emulation Data MSW Register (FEMU_DMSW)
Figure 7-29. EEPROM Emulation Data MSW Register (FEMU_DMSW) (offset =58h)
31 16
EMU_DMSW[63:48]
R/WP-0h
15 0
EMU_DMSW[47:32]
R/WP-0h
LEGEND: R/W =Read/Write; WP=Write inPrivilege mode; -n=value after reset
Table 7-31. EEPROM Emulation Data MSW Register (FEMU_DMSW) Field Descriptions
Bit Field Description
31-0 EMU_DMSW This register canbewritten bytheCPU inanymode.
This register isused indiagnostic mode 7toXOR theupper 32bitsofthedata being delivered tothebus
master.
7.10.20 EEPROM Emulation Data LSW Register (FEMU_DLSW)
Figure 7-30. EEPROM Emulation Data LSW Register (FEMU_DLSW) (offset =5Ch)
31 16
EMU_DLSW[31:16]
R/WP-0h
15 0
EMU_DLSW[15:0]
R/WP-0h
LEGEND: R/W =Read/Write; WP=Write inPrivilege mode; -n=value after reset
Table 7-32. EEPROM Emulation Data LSW Register (FEMU_DLSW) Field Descriptions
Bit Field Description
31-0 EMU_DLSW This register canbewritten bytheCPU inanymode.
This register isused indiagnostic mode 7toXOR thelower 32bitsofthedata being delivered tothebus
master.

<!-- Page 373 -->

www.ti.com Flash Control Registers
373 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.21 EEPROM Emulation ECC Register (FEMU_ECC)
Figure 7-31. EEPROM Emulation ECC Register (FEMU_ECC) (offset =60h)
31 16
Reserved
R-0
15 8 7 0
Reserved EMU_ECC
R-0 R/WP-0h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege mode; -n=value after reset
Table 7-33. EEPROM Emulation ECC Register (FEMU_ECC) Field Descriptions
Bit Field Value Description
31-8 Reserved 0 Reads return 0.Writes have noeffect.
7-0 EMU_ECC 0-FFh This register canbewritten bytheCPU inanymode.
This register isused indiagnostic mode 7toXOR theECC being delivered tothebusmaster.
7.10.22 Flash Lock Register (FLOCK)
Figure 7-32. Flash Lock Register (FLOCK) (offset =64h)
31 16
Reserved
R-0
15 0
ENCOM
R/WP-55AAh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-34. Flash Lock Register (FLOCK) Field Descriptions
Bit Field Value Description
31-16 Reserved 0 Reads return 0.Writes have noeffect.
15-0 ENCOM AA55h Enable writes toEE_FEDACCTRL1 register (see Section 7.10.3 ).
Allother values Writes toEE_FEDACCTRL1 areignored.
Itisrecommended toleave thisregister as55AAh when notwriting totheFEDACCTRL1
register.

<!-- Page 374 -->

Flash Control Registers www.ti.com
374 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.23 Diagnostic Control Register (FDIAGCTRL)
First settheDIAGMODE andtheDIAG_EN_KEY bitsbefore setting uptheother registers toblock the
other registers from causing afalse error. The final write should settheDIAG_TRIG bittoactivate thetest.
Running outofRAM willprevent problems with thediagnostic testcorrupting theFlash access insome of
themodes.
Figure 7-33. Diagnostic Control Register (FDIAGCTRL) (offset =6Ch)
31 25 24 23 20 19 16
Reserved DIAG_TRIG Reserved DIAG_EN_KEY
R-0 R/WP-0 R-0 R/WP-Ah
15 11 10 8 7 3 2 0
Reserved DIAG_BUF_SEL Reserved DIAGMODE
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege mode; -n=value after reset
Table 7-35. Diagnostic Control Register (FDIAGCTRL) Field Descriptions
Bit Field Value Description
31-25 Reserved 0 Reads return 0.Writes have noeffect.
24 DIAG_TRIG Diagnostic Trigger
Diagnostic trigger isthefinal qualifier forthediagnostic result. After setting allthe
other diagnostic register values, theDIAG_TRIG issetto1.This activates the
diagnostic logic foroneaccess andthen automatically clears theDIAG_TRIG value.
The DIAG_EN_KEY andDIAGMODE bitsmust besetbefore setting DIAG_TRIG.
This bitalways reads as0.
23-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 DIAG_EN_KEY Diagnostic Enable Key
5h Diagnostic mode isenabled.
Allother values Diagnostic mode isdisabled.
15-11 Reserved 0 Reads return 0.Writes have noeffect.
10-8 DIAG_BUF_ SEL Diagnostic Buffer Select
0 During diagnostic mode 5theDIAG_BUF_SEL selects thebuffer toread orwrite when
accessing theFPRIM_ADD_TAG andFDUP_ADD_TAG registers. The address tags
consists ofmatching primary andduplicate address tagregisters. Alltheprimary
address tagregisters arememory mapped toacommon address (see Section 7.10.8 )
andareselected byDIAG_BUF_SEL. The same occurs fortheduplicate address (see
Section 7.10.9 ).Port Ahas2buffers andPort Bhas4buffers.
During diagnostic mode 7thevalue selects theport onwhich toperform the
diagnostic.
0 Port ABuffer 0(diag mode 5)/Port Aselected toflipdata/ECC (diag mode 7)
1h Port ABuffer 1(diag mode 5)/Reserved indiag mode 7
2h Reserved
3h Reserved
4h Port BBuffer 0(diag mode 5)/Port Bselected toflipdata/ECC (diag mode 7)
5h Port BBuffer 1(diag mode 5)/Reserved (diag mode 7)
6h Port BBuffer 2(diag mode 5)/Reserved (diag mode 7)
7h Port BBuffer 3(diag mode 5)/Reserved (diag mode 7)
7-4 Reserved 0 Reads return 0.Writes have noeffect.
2-0 DIAGMODE Diagnostic Mode (Only values 0,5,and7arevalid. Other values arereserved).
0 Diagnostic mode isdisabled. This isthesame asDIAG_EN_KEY isnotequal to5h.
5h Address Tag Register testmode (see Section 7.7.2.1 ).
7h ECC Data Correction Diagnostic testmode (see Section 7.7.2.2 ).

<!-- Page 375 -->

www.ti.com Flash Control Registers
375 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.24 Raw Address Register (FRAW_ADDR)
Figure 7-34. Raw Address Register (FRAW_ADDR) (offset =74h)
31 54 0
RAW_DATA[31:5] Reserved
R/WP-u R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege mode; -u=Unchanged value oninternal reset, cleared onpower up;-
n=value after reset
Table 7-36. Raw Address Register (FRAW_ADDR) Field Descriptions
Bit Field Description
31-5 RAW_DATA Raw Address.
This register isused during theaddress tagregister testmode, DIAGMODE =5,toreplace theaddress
busbits31:3. The lower 5bitsarenotcompared during thediagnostic.
4-0 Reserved Reads return 0.Writes have noeffect.

<!-- Page 376 -->

Flash Control Registers www.ti.com
376 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.25 Parity Override Register (FPAR_OVR)
This register allows overriding theparity thatisinternally computed bytheL2FMC forchecking theparity
circuit.
Figure 7-35. Parity Override Register (FPAR_OVR) (offset =7Ch)
31 18 17 16
Reserved PAR_OVR_SEL
R-0 R/WP-0
15 12 11 9 8 0
PAR_DIS_KEY PAR_OVR_KEY Reserved
R/WP-5h R/WP-2h R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-37. Parity Override Register (FPAR_OVR) Field Descriptions
Bit Field Value Description
31-18 Reserved 0 Reserved
17-16 PAR_OVR_SEL Select which parity checker toinvert thepolarity oftheparity.
0 Noeffect.
1h Idlestate parity checker received inverted parity polarity.
2h Command parity checker receives inverted parity polarity.
3h Internal address parity checker receives inverted parity polarity
15-12 PAR_DIS_KEY Disable access Parity. ECC onData isNOT affected bythissetting andbehaves thesame
way.
Ah The access parity error isdisabled andnochecking isdone andnoevents aregenerated.
Allother values Any other value enables theparity checking ontheaccess.
11-9 PAR_OVR_KEY Parity Override
5h The selected parity checker selected through PAR_OVR_SEL willreceive inverted
SYS_ODD_PARITY.
Allother values Any other value causes themodule tousetheglobal system parity bitinthesystem register
DEVCR1.
8-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 377 -->

www.ti.com Flash Control Registers
377 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.26 Reset Configuration Valid Register (RCR_VALID)
This register reflects thevalidity oftheimplicit read.
Figure 7-36. Reset Configuration Valid Register (RCR_VALID) (offset =B4h)
31 16
Reserved
R-0
15 2 1 0
Reserved JSM_VALID RCR_VALID
R-0 R-1 R-1
LEGEND: R=Read only; -n=value after reset
Table 7-38. Reset Configuration Valid Register (RCR_VALID) Field Descriptions
Bit Field Value Description
31-2 Reserved 0 Reserved
1 JSM_VALID When theL2FMC finishes theimplicit read, itsets thisbittoindicate thatthecontents of
RCR_VALUE0 andRCR_VALUE1 arevalid. This bitwillbecleared incase there was adouble-bit
error during implicit reads.
0 The implicit read hasfailed. The device level settings may notbecorrect.
1 Implicit read issuccessful. Device level settings arecorrect.
0 RCR_VALID When theL2FMC finishes theimplicit read, itsets thisbittoindicate thatthecontents of
RCR_VALUE0 andRCR_VALUE1 arevalid. This bitwillbecleared incase there was adouble-bit
error during implicit reads.
0 The implicit read hasfailed. The device level settings may notbecorrect.
1 Implicit read issuccessful. Device level settings arecorrect.
7.10.27 Crossbar Access Time Threshold Register (ACC_THRESHOLD)
Figure 7-37. Crossbar Access Time Threshold Register (ACC_THRESHOLD) (offset =B8h)
31 16
Reserved
R-0
15 12 11 0
Reserved ACC_THRESH_CNT
R-0 R/WP-5FFh
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-39. Crossbar Access Time Threshold Register (ACC_THRESHOLD) Field Descriptions
Bit Field Value Description
31-12 Reserved 0 Reserved
11-0 ACC_THRESH_CNT 5FFh Configures maximum number ofclocks beyond which theL2FMC internal switch willtimeout the
access. This canoccur duetosofterror ininternal logic. ItisNOT recommended tomodify this
register unless acrossbar diagnostic isbeing performed.

<!-- Page 378 -->

Flash Control Registers www.ti.com
378 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.28 Flash Error Detection andCorrection Sector Disable Register 2(FEDACSDIS2)
This register isused todisable theSECDED function onadditional twosectors ontheEEPROM
Emulation Flash (bank 7).
Figure 7-38. Flash Error Detection andCorrection Sector Disable Register 2(FEDACSDIS2)
(offset =C0h)
31 30 29 24 23 22 21 16
Rsvd SectorID3_inverse Rsvd SectorID3
R-0 R/WP-0 R-0 R/WP-0
15 14 13 8 7 6 5 0
Rsvd SectorID2_inverse Rsvd SectorID2
R-0 R/WP-0 R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-40. Flash Error Detection andCorrection Sector Disable Register 2(FEDACSDIS2)
Field Descriptions
Bit Field Value Description
31-30 Reserved 0 Reads return 0.Writes have noeffect.
29-24 SectorID3_inverse 0-3Fh The sector IDinverse bitsareused with thesector IDbitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID3.
23-22 Reserved 0 Reads return 0.Writes have noeffect.
21-16 SectorID3 0-3Fh The sector IDbitsareused with thesector IDinverse bitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID3.
15-14 Reserved 0 Reads return 0.Writes have noeffect.
13-8 SectorID2_inverse 0-3Fh The sector IDinverse bitsareused with thesector IDbitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID2.
7-6 Reserved 0 Reads return 0.Writes have noeffect.
5-0 SectorID2 0-3Fh The sector IDbitsareused with thesector IDinverse bitstodetermine which sector is
disabled. Ifthesector IDbitsarenotpointing toavalid sector (0-3) orthesector ID
inverse bitsarenotaninverse ofthesector IDbits, then nosector isdisabled bydisable
ID2.

<!-- Page 379 -->

www.ti.com Flash Control Registers
379 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.29 Lower Word ofReset Configuration Read Register (RCR_VALUE0)
When L2FMC completes theimplicit read, itpopulates thisregister with thelower 32bitsofthedata. This
isuseful toperform asoftware diagnostic oftheSECDED module
Figure 7-39. Lower Word ofReset Configuration Read Register (RCR_VALUE0) (offset =D0h)
31 16
RCR_VALUE[31:16]
R-u
15 0
RCR_VALUE[15:0]
R-u
LEGEND: R=Read only; -u=Unchanged value oninternal reset, cleared onpower up;-n=value after reset
Table 7-41. Lower Word ofReset Configuration Read Register (RCR_VALUE0) Field Descriptions
Bit Field Value Description
31-0 RCR_VALUE 0 Value ofthelower 32bitsoftheimplicit read. Valid only ifRCR_VALID isset.
7.10.30 Upper Word ofReset Configuration Read Register (RCR_VALUE1)
When L2FMC completes theimplicit read, itpopulates thisregister with theupper 32bitsofthedata. This
isuseful toperform asoftware diagnostic oftheSECDED module
Figure 7-40. Upper Word ofReset Configuration Read Register (RCR_VALUE1) (offset =D4h)
31 16
RCR_VALUE[63:48]
R-u
15 0
RCR_VALUE[47:32]
R-u
LEGEND: R=Read only; -u=Unchanged value oninternal reset, cleared onpower up;-n=value after reset
Table 7-42. Upper Word ofReset Configuration Read Register (RCR_VALUE1) Field Descriptions
Bit Field Value Description
31-0 RCR_VALUE Varies with device Value oftheupper 32bitsoftheimplicit read. Valid only ifRCR_VALID isset.

<!-- Page 380 -->

Flash Control Registers www.ti.com
380 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.31 FSM Register Write Enable Register (FSM_WR_ENA)
Figure 7-41. FSM Register Write Enable Register (FSM_WR_ENA) (offset =288h)
31 16
Reserved
R-0
15 3 2 0
Reserved WR_ENA
R-0 R/WP-2h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-43. FSM Register Write Enable Register (FSM_WR_ENA) Field Descriptions
Bit Field Value Description
31-3 Reserved 0 Reads return 0.Writes have noeffect.
2-0 WR_ENA FSM Write Enable
5h This register must contain 5hinorder towrite toanyother register intherange FFF8 7200h
toFFF8 72FFh. This isthefirstregister tobewritten when setting uptheFSM.
Allother values Forallother values, theFSM registers cannot bewritten.
7.10.32 EEPROM Emulation Configuration Register (EEPROM_CONFIG)
Figure 7-42. EEPROM Emulation Configuration Register (EEPROM_CONFIG) (offset =2B8h)
31 20 19 16
Reserved EWAIT
R-0 R/WP-1
15 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-44. EPROM Emulation Configuration Register (EEPROM_CONFIG) Field Descriptions
Bit Field Value Description
31-20 Reserved 0 Reads return 0.Writes have noeffect.
19-16 EWAIT 0-Fh EEPROM Wait state Counter
Replaces theRWAIT count intheEEPROM register. The same formulas thatapply toRWAIT apply
toEWAIT intheEEPROM bank.
15-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 381 -->

www.ti.com Flash Control Registers
381 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.33 FSM Sector Register 1(FSM_SECTOR1)
This isabanked register. Aseparate register isimplemented foreach bank, butthey alloccupy thesame
address. The correct bank must beselected intheFMAC register before reading orwriting thisregister.
See Section 7.10.17 .
Figure 7-43. FSM Sector Register 1(FSM_SECTOR1) (offset =2C0h)
31 16
SECT_ERASED[31:16]
R/WP-1
15 0
SECT_ERASED[15:0]
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inPrivilege Mode; -n=value after reset
Table 7-45. FSM Sector Register 1(FSM_SECTOR1) Field Descriptions
Bit Field Value Description
31-0 SECT_ERASED Sectors Erased. Each bitcorresponds toaFlash sector inthebank specified bythe
FMAC register. Bit0corresponds tosector 0,bit1corresponds tosector 1,andsoon.
0 During bank erase, each sector whose corresponding bitis0willbeerased. After bank
erase, thebitcorresponding toeach sector thatiserased willbechanged from 0to1.
1 During bank erase, each sector whose corresponding bitis1willnotbeerased.
NOTE: Ifthebank hasless than 32sectors, only those many LSB bitsofFSM_SECTOR1 arevalid.
ForEEPROM bank having more than 32sectors, usethisregister inconjunction with
FSM_SECTOR2.
7.10.34 FSM Sector Register 2(FSM_SECTOR2)
This register isapplicable toEEPROM bank having more than 32sectors only. Refer tothedevice
datasheet tofindthenumber ofEEPROM sectors inaparticular device.
Figure 7-44. FSM Sector Register 2(FSM_SECTOR2) (offset =2C4h)
31 16
SECT_ERASED[63:48]
R/WP-1
15 0
SECT_ERASED[47:32]
R/WP-1
LEGEND: R/W =Read/Write; WP=Write inPrivilege Mode; -n=value after reset
Table 7-46. FSM Sector Register 2(FSM_SECTOR2) Field Descriptions
Bit Field Value Description
31-0 SECT_ERASED Sectors Erased. Each bitcorresponds toaFlash sector inthebank specified bythe
FMAC register. Bit0corresponds tosector 32,bit1corresponds tosector 33,andso
on.
0 During bank erase, each sector whose corresponding bitis0willbeerased. After bank
erase, thebitcorresponding toeach sector thatiserased willbechanged from 0to1.
1 During bank erase, each sector whose corresponding bitis1willnotbeerased.

<!-- Page 382 -->

Flash Control Registers www.ti.com
382 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.10.35 Flash Bank Configuration Register (FCFG_BANK)
Figure 7-45. Flash Bank Configuration Register (FCFG_BANK) (offset =400h)
31 20 19 16
EE_BANK_WIDTH Reserved
R-48h R-1
15 4 3 0
MAIN_BANK_WIDTH Reserved
R-90h R-2h
LEGEND: R=Read only; -n=value after reset
Table 7-47. Flash Bank Configuration Register (FCFG_BANK) Field Descriptions
Bit Field Value Description
31-20 EE_BANK_WIDTH 48h Bank 7width (72-bits wide)
This read-only value indicates themaximum number ofbitsthatcanbeprogrammed inthe
bank inoneoperation. The 72bitsincludes 64data bitsand8ECC bits.
19-16 Reserved 1 Writes have noeffect.
15-4 MAIN_BANK_WIDTH 90h Width ofmain Flash banks (288-bits wide)
This read-only value indicates themaximum number ofbitsthatcanbeprogrammed inthe
bank inoneoperation. The 288bitsincludes 256data bitsand32ECC bits.
3-0 Reserved 2h Writes have noeffect.

<!-- Page 383 -->

www.ti.com POM Control Registers
383 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.11 POM Control Registers
This section details thePOM module registers listed inTable 7-48 .
The POM module control registers canonly beread and/or written while inprivileged ordebug mode.
Each register begins onaword boundary. Allregisters are32-bit, 16-bit and8-bit accessible. The start
address ofthePOM module isFFA0 4000h.
Table 7-48. POM Control Registers
Offset Acronym Register Description Section
00h POMGLBCTRL POM Global Control Register Section 7.11.1
04h POM_REVID POM Revision IDRegister Section 7.11.2
0Ch POMFLG POM Flag Register Section 7.11.3
200h, 210h, ... PROMPROGSTARTx POM Region Start Address Register Section 7.11.4
204h, 214h,... POMOVLSTARTx POM Overlay Start Address Register Section 7.11.5
208h, 218h,... POMREGSIZEx POM Region Size Register Section 7.11.6
7.11.1 POM Global Control Register (POMGLBCTRL)
Contains enable control forthePOM module.
Figure 7-46. POM Global Control Register (POMGLBCTRL) (offset =00h)
31 22 21 16
OTADDR Reserved
R/WP-01 1000 0000 R-0
15 4 3 0
Reserved ON_OFF
R-0 R/WP-5h
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-49. POM Global Control Register (POMGLBCTRL) Field Descriptions
Bit Field Value Description
31-22 OTADDR Overlay Target Address. These bitsdetermine theupper address bitsoftheremapped address.
Writing adifferent value tothisbitfield willsteer theaccess toadifferent location inthe4GB
address space. Care hastobetaken thatthevalue written represents actual memory.
21-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 ON_OFF POM enable
except Ah POM isdisabled.
Ah POM isenabled.

<!-- Page 384 -->

POM Control Registers www.ti.com
384 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.11.2 POM Revision IDRegister (POMREV)
Figure 7-47. POM Revision IDRegister (POMREV) (offset =04h)
31 16
REVID
R-0108h
15 0
REVID
R-CA03h
LEGEND: R=Read only; -n=value after reset
Table 7-50. POM Revision IDRegister (POMREV) Field Descriptions
Bit Field Value Description
31-0 REVID 0108CA03h Revision IDofPOM
7.11.3 POM Flag Register (POMFLG)
This register conveys status bitsthatgetsetduring POM accesses.
Allthese error status bitscanbecleared bywriting a1tothebit;writing a0hasnoeffect.
Figure 7-48. POM Flag Register (POMFLG) (offset =0Ch)
31 16
Reserved
R-0
15 11 10 9 8
Reserved PERR_SRESP_IDLE PERR_PB PERR_PA
R-0 R/W1CP-u R/W1CP-u R/W1CP-u
7 0
Reserved
R-0
LEGEND: R/W =Read/Write; R=Read only; W1CP =Write 1toclear inPrivilege Mode; -u=unchanged value oninternal reset, cleared
onpower up;-n=value after reset
Table 7-51. POM Flag Register (POMFLG) Field Descriptions
Bit Field Value Description
31-11 Reserved 0 Reads return 0.Writes have noeffect.
10 PERR_SRESP_IDLE Idleresponse parity error onPOM access.
0 Idleresponse parity error onPOM access hasNOT occurred.
1 Idleresponse parity error onPOM access hasoccurred.
9 PERR_PB Parity Error onPOM access duetoremapping request onPort B.
0 Parity error onPOM Port Bremap request hasNOT occurred.
1 Parity error onPOM Port Bremap request hasoccurred.
8 PERR_PA Parity Error onPOM access duetoremapping request onPort A.
0 Parity error onPOM Port Aremap request hasNOT occurred.
1 Parity error onPOM Port Aremap request hasoccurred.
7-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 385 -->

www.ti.com POM Control Registers
385 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.11.4 POM Region Start Address Register (POMPROGSTARTx)
This setofregisters contains thestart address ofeach region which istoberemapped. These registers
areatanoffset 200h +(10h xregion number). Region numbers arecounted from 0onwards.
Figure 7-49. POM Region Start Address Register (POMPROGSTARTx) (offset =200h, 210h,..)
31 23 22 16
Reserved STARTADDRESS
R-0 R/WP-0
15 6 5 0
STARTADDRESS Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-52. POM Region Start Address Register (POMPROGSTARTx)
Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22-6 STARTADDRESS Start address oftheprogram memory region.
5-0 Reserved 0 Reads return 0.Writes have noeffect.
7.11.5 POM Overlay Region Start Address Register (POMOVLSTARTx)
Contains thestart address oftheoverlay region involatile memory.
Figure 7-50. POM Overlay Region Start Address Register (POMOVLSTARTx) (offset =204h, 214h,...)
31 23 22 16
Reserved STARTADDRESS
R-0 R/WP-0
15 6 5 0
STARTADDRESS Reserved
R/WP-0 R-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-53. POM Overlay Region Start Address Register (POMOVLSTARTx)
Field Descriptions
Bit Field Value Description
31-23 Reserved 0 Reads return 0.Writes have noeffect.
22-6 STARTADDRESS Start address oftheprogram memory region.
5-0 Reserved 0 Reads return 0.Writes have noeffect.

<!-- Page 386 -->

POM Control Registers www.ti.com
386 SPNU563A -March 2018
Submit Documentation Feedback
Copyright ©2018, Texas Instruments IncorporatedF021 Level 2Flash Module Controller (L2FMC)7.11.6 POM Region Size Register (POMREGSIZEx)
Contains thesize oftheprogram memory andoverlay memory region.
Figure 7-51. POM Region Size Register (POMREGSIZEx) (offset =208h, 218h, ...)
31 16
Reserved
R-0
15 4 3 0
Reserved SIZE
R-0 R/WP-0
LEGEND: R/W =Read/Write; R=Read only; WP=Write inPrivilege Mode; -n=value after reset
Table 7-54. POM Region Size Register (POMREGSIZEx) Field Descriptions
Bit Field Value Description
31-4 Reserved 0 Reads return 0.Writes have noeffect.
3-0 SIZE 0 Region isdisabled.
1h 64bytes
2h 128bytes
: :
Ch 128K bytes
Dh 256K bytes
Eh-Fh Reserved