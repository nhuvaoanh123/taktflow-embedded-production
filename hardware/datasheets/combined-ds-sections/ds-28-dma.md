# DMA Controller

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 123-126 (4 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 123 -->
123TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.17 DMA Controller
The DMA controller isused totransfer data between twolocations inthememory map inthebackground
ofCPU operations. Typically, theDMA isused to:
*Transfer blocks ofdata between external andinternal data memories
*Restructure portions ofinternal data memory
*Continually service aperipheral
6.17.1 DMA Features
*64-bit OCP protocol toperform busmaster accesses
*INCR-4 64-bit burst accesses
*Multithreading architecture allowing data oftwo different channel transfers tobeinterleaved during nonburst
accesses
*2-port configuration forparallel busmaster
*Channels canbeassigned toeither high-priority queue orlow-priority queue. Within each queue, fixed orround-
robin priorities canbeserviced
*Built-in ECC generation andevaluation logic forinternal RAM storing channel transfer information
*Supports multiple interrupt outputs formapping tomultiple interrupt controllers inmulticore systems
*48requests canbemapped toany32channels
*Supports LEendianess
*External ECC Gen/Eval block ofDMA support ECC generation fordata transactions, and parity foraddress, and
control signals (following Cortex-R5F standard)
*8MPU regions
*Channel chaining capability
*Hardware andsoftware DMA requests
*8-,16-, 32-, or64-bit transactions supported
*Multiple addressing modes forsource/destination (fixed, increment, offset)
*Auto-initiation
6.17.2 DMA Transfer Port Assignment
There aretwoports, port Aand port Battached totheDMA controller. When configuring aDMA channel
foratransfer, the application must also specify the port associated with the transfer source and
destination. Table 6-40 lists themapping between each port andtheresources. Forexample, ifatransfer
istobemade from thetheflash totheSRAM, theapplication willneed configure thedesired DMA
channel inthePARx register toselect port Aasthetarget forboth thesource anddestination. Ifatransfer
istobemade from theSRAM toaperipheral oraperipheral memory, theapplication willneed to
configure thedesired DMA channel inthePARx register toselect port Aforread and port Bforwrite.
Likewise, ifatransfer isfrom aperipheral totheSRAM then thePARx willbeconfigured toselect port B
forread andport Aforwrite.
Table 6-40. DMA Port Assignment
TARGET NAME ACCESS PORT OFDMA
Flash Port A
SRAM Port A
EMIF Port A
Flash OTP/ECC/EEPROM Port A
Allother targets (peripherals, peripheral memories) Port B

<!-- Page 124 -->
124TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) SPI1, SPI2, SPI3, SPI4, SPI5 receive incompatibility mode
(2) SPI1, SPI2, SPI3, SPI4, SPI5 transmit incompatibility mode6.17.3 Default DMA Request Map
The DMA module onthismicrocontroller has 32channels and upto48hardware DMA requests. The
module contains DREQASIx registers which areused tomap theDMA requests totheDMA channels. By
default, channel 0ismapped torequest 0,channel 1torequest 1,andsoon.
Some DMA requests have multiple sources, seeTable 6-41.The application must ensure thatonly oneof
these DMA request sources isenabled atanytime.
Table 6-41. DMA Request Line Connection
MODULES DMA REQUEST SOURCES DMA REQUEST
MIBSPI1 MIBSPI1[1](1)DMAREQ[0]
MIBSPI1 MIBSPI1[0](2)DMAREQ[1]
MIBSPI2 MIBSPI2[1](1)DMAREQ[2]
MIBSPI2 MIBSPI2[0](2)DMAREQ[3]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[2] /MIBSPI3[2] /DCAN2 IF3 DMAREQ[4]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[3] /MIBSPI3[3] /DCAN2 IF2 DMAREQ[5]
DCAN1 /MIBSPI5 DCAN1 IF2/MIBSPI5[2] DMAREQ[6]
MIBADC1 /MIBSPI5 MIBADC1 event /MIBSPI5[3] DMAREQ[7]
MIBSPI1 /MIBSPI3 /DCAN1 MIBSPI1[4] /MIBSPI3[4] /DCAN1 IF1 DMAREQ[8]
MIBSPI1 /MIBSPI3 /DCAN2 MIBSPI1[5] /MIBSPI3[5] /DCAN2 IF1 DMAREQ[9]
MIBADC1 /I2C/MIBSPI5 MIBADC1 G1/I2Creceive /MIBSPI5[4] DMAREQ[10]
MIBADC1 /I2C/MIBSPI5 MIBADC1 G2/I2Ctransmit /MIBSPI5[5] DMAREQ[11]
RTI1 /MIBSPI1 /MIBSPI3 RTI1 DMAREQ0 /MIBSPI1[6] /MIBSPI3[6] DMAREQ[12]
RTI1 /MIBSPI1 /MIBSPI3 RTI1 DMAREQ1 /MIBSPI1[7] /MIBSPI3[7] DMAREQ[13]
MIBSPI3 /MibADC2 /MIBSPI5 MIBSPI3[1](1)/MibADC2 event /MIBSPI5[6] DMAREQ[14]
MIBSPI3 /MIBSPI5 MIBSPI3[0](2)/MIBSPI5[7] DMAREQ[15]
MIBSPI1 /MIBSPI3 /DCAN1 /MibADC2 MIBSPI1[8] /MIBSPI3[8] /DCAN1 IF3/MibADC2 G1 DMAREQ[16]
MIBSPI1 /MIBSPI3 /DCAN3 /MibADC2 MIBSPI1[9] /MIBSPI3[9] /DCAN3 IF1/MibADC2 G2 DMAREQ[17]
RTI1 /MIBSPI5 RTI1 DMAREQ2 /MIBSPI5[8] DMAREQ[18]
RTI1 /MIBSPI5 RTI1 DMAREQ3 /MIBSPI5[9] DMAREQ[19]
NHET1 /NHET2 /DCAN3 NHET1 DMAREQ[4] /NHET2 DMAREQ[4] /DCAN3 IF2 DMAREQ[20]
NHET1 /NHET2 /DCAN3 NHET1 DMAREQ[5] /NHET2 DMAREQ[5] /DCAN3 IF3 DMAREQ[21]
MIBSPI1 /MIBSPI3 /MIBSPI5 MIBSPI1[10] /MIBSPI3[10] /MIBSPI5[10] DMAREQ[22]
MIBSPI1 /MIBSPI3 /MIBSPI5 MIBSPI1[11] /MIBSPI3[11] /MIBSPI5[11] DMAREQ[23]
NHET1 /NHET2 /MIBSPI4 /MIBSPI5 NHET1 DMAREQ[6] /NHET2 DMAREQ[6] /MIBSPI4[1](1)/MIBSPI5[12] DMAREQ[24]
NHET1 /NHET2 /MIBSPI4 /MIBSPI5 NHET1 DMAREQ[7] /NHET2 DMAREQ[7] /MIBSPI4[0](2)/MIBSPI5[13] DMAREQ[25]
CRC 1/MIBSPI1 /MIBSPI3 CRC 1DMAREQ[0] /MIBSPI1[12] /MIBSPI3[12] DMAREQ[26]
CRC 1/MIBSPI1 /MIBSPI3 CRC 1DMAREQ[1] /MIBSPI1[13] /MIBSPI3[13] DMAREQ[27]
LIN1 /MIBSPI5 LIN1 receive /MIBSPI5[14] DMAREQ[28]
LIN1 /MIBSPI5 LIN1 transmit /MIBSPI5[15] DMAREQ[29]
MIBSPI1 /MIBSPI3 /SCI3 /MIBSPI5 MIBSPI1[14] /MIBSPI3[14] /SCI3 receive /MIBSPI5[1](1)DMAREQ[30]
MIBSPI1 /MIBSPI3 /SCI3 /MIBSPI5 MIBSPI1[15] /MIBSPI3[15] /SCI3 transmit /MIBSPI5[0](2)DMAREQ[31]
I2C2 /ePWM1 /MIBSPI2 /MIBSPI4 /GIOA I2C2 receive /ePWM1_SOCA /MIBSPI2[2] /MIBSPI4[2] /GIOA[0] DMAREQ[32]
I2C2 /ePWM 1/MIBSPI2 /MIBSPI4 /GIOA I2C2 transmit /ePWM1_SOCB /MIBSPI2[3] /MIBSPI4[3] /GIOA[1] DMAREQ[33]
ePWM2 /MIBSPI2 /MIBSPI4 /GIOA ePWM2_SOCA /MIBSPI2[4] /MIBSPI4[4] /GIOA[2] DMAREQ[34]
ePWM2 /MIBSPI2 /MIBSPI4 /GIOA ePWM2_SOCB /MIBSPI2[5] /MIBSPI4[5] /GIOA[3] DMAREQ[35]
ePWM3 /MIBSPI2 /MIBSPI4 /GIOA ePWM3_SOCA /MIBSPI2[6] /MIBSPI4[6] /GIOA[4] DMAREQ[36]
ePWM3 /MIBSPI2 /MIBSPI4 /GIOA ePWM3_SOCB /MIBSPI2[7] /MIBSPI4[7] /GIOA[5] DMAREQ[37]
CRC2 /ePWM4 /MIBSPI2 /MIBSPI4 /GIOA CRC2 DMAREQ[0] /ePWM4_SOCA /MIBSPI2[8] /MIBSPI4[8] /GIOA[6] DMAREQ[38]
CRC2 /ePWM4 /MIBSPI2 /MIBSPI4 /GIOA CRC2 DMAREQ[1] /ePWM4_SOCB /MIBSPI2[9] /MIBSPI4[9] /GIOA[7] DMAREQ[39]
LIN2 /ePWM5 /MIBSPI2 /MIBSPI4 /GIOB LIN2 receive /ePWM5_SOCA /MIBSPI2[10] /MIBSPI4[10] /GIOB[0] DMAREQ[40]
LIN2 /ePWM5 /MIBSPI2 /MIBSPI4 /GIOB LIN2 transmit /ePWM5_SOCB /MIBSPI2[11] /MIBSPI4[11] /GIOB[1] DMAREQ[41]

<!-- Page 125 -->
125TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedTable 6-41. DMA Request Line Connection (continued)
MODULES DMA REQUEST SOURCES DMA REQUEST
SCI4 /ePWM6 /MIBSPI2 /MIBSPI4 /GIOB SCI4 receive /ePWM6_SOCA /MIBSPI2[12] /MIBSPI4[12] /GIOB[2] DMAREQ[42]
SCI4 /ePWM6 /MIBSPI2 /MIBSPI4 /GIOB SCI4 transmit /ePWM6_SOCB /MIBSPI2[13] /MIBSPI4[13] /GIOB[3] DMAREQ[43]
ePWM7 /MIBSPI2 /MIBSPI4 /GIOB ePWM7_SOCA /MIBSPI2[14] /MIBSPI4[14] /GIOB[4] DMAREQ[44]
ePWM7 /MIBSPI2 /MIBSPI4 /GIOB /
DCAN4ePWM7_SOCB /MIBSPI2[15] /MIBSPI4[15] /GIOB[5] /DCAN4 IF1 DMAREQ[45]
GIOB /DCAN4 GIOB[6] /DCAN4_IF2 DMAREQ[46]
GIOB /DCAN4 GIOB[7] /DCAN4_IF3 DMAREQ[47]

<!-- Page 126 -->
GIOA[0]I2C2 receive
EPWM1_SOCA
MIBSPI2[2]
MIBSPI4[2]DMAREQ[32]
DMA
PINMMR175[0]01
DMAREQ[47]
126TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357System Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated6.17.4 Using aGIOterminal asaDMA Request Input
Each GIO terminal canalso directly beused asDMA request input aslisted inTable 6-41.The polarity of
theGIO terminal totrigger aDMA request can beselected inside theDMA module. Touse theGIO
terminal asaDMA request input, thecorresponding select bitmust besettolow. See Figure 6-19 foran
illustration. Formore information seethetechnical reference guide SPNU563 .
Figure 6-19. Using aGIOterminal asaDMA Request Input
Table 6-42. GIODMA Request Disable Mapping
GIOTERMINAL GIODMA REQUEST SELECT BIT
GIOA[0] PINMMR175[0]
GIOA[1] PINMMR175[8]
GIOA[2] PINMMR175[16]
GIOA[3] PINMMR175[24]
GIOA[4] PINMMR176[0]
GIOA[5] PINMMR176[8]
GIOA[6] PINMMR176[16]
GIOA[7] PINMMR176[24]
GIOB[0] PINMMR177[0]
GIOB[1] PINMMR177[8]
GIOB[2] PINMMR177[16]
GIOB[3] PINMMR177[24]
GIOB[4] PINMMR178[0]
GIOB[5] PINMMR178[8]
GIOB[6] PINMMR178[16]
GIOB[7] PINMMR178[24]