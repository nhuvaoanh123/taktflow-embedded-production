# Ethernet MAC

> **Source:** TMS570LC4357 Combined Datasheet
> **PDF:** datasheet-1518516-evaluation-board-launchxl2-570lc43-hercules-hercules.pdf
> **Pages:** 207-210 (4 pages)
> **Extracted:** 2026-03-05

---


<!-- Page 207 -->
1 2
MII_RX_CLK
MII_RXD[3:0]
MII_RX_DV
MII_RX_ERVALID
207TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.13 Ethernet Media Access Controller
The Ethernet Media Access Controller (EMAC) provides anefficient interface between thedevice andthe
network. The EMAC supports both 10Base-T and100Base-TX, or10Mbits/second (Mbps) and100Mbps
ineither half- orfull-duplex mode, with hardware flow control andquality ofservice (QoS) support.
The EMAC controls theflow ofpacket data from thedevice tothePHY. The MDIO module controls PHY
configuration andstatus monitoring.
Both theEMAC and theMDIO modules interface tothedevice through acustom interface that allows
efficient data transmission and reception. This custom interface isreferred toastheEMAC control
module, and isconsidered integral totheEMAC/MDIO peripheral. The control module isalso used to
multiplex andcontrol interrupts.
7.13.1 Ethernet MIIElectrical andTiming Specifications
Figure 7-27. MIIReceive Timing
Table 7-41. MIIReceive Timing
Parameter Description MIN MAX
tsu(GMIIMRXD) Setup time, GMIIMRXD toGMIIMRCLK rising edge 8ns
tsu(GMIIMRXDV) Setup time, GMIIMRXDV toGMIIMRCLK rising edge 8ns
tsu(GMIIMRXER) Setup time, GMIIMRXER toGMIIMRCLK rising edge 8ns
th(GMIIMRXD) Hold time, GMIIMRXD valid after GMIIRCLK rising
edge8ns
th(GMIIMRXDV) Hold time, GMIIMRXDV valid after GMIIRCLK rising
edge8ns
th(GMIIMRXER) Hold time, GMIIMRXDV valid after GMIIRCLK rising
edge8ns

<!-- Page 208 -->
1
MII_TX_CLK
MII_TXD[3:0]
MII_TXENVALID
208TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments IncorporatedFigure 7-28. MIITransmit Timing
Table 7-42. MIITransmit Timing
Parameter Description MIN MAX
td(GMIIMTXD) Delay time, GMIIMTCLK rising edge toGMIIMTXD 5ns 25ns
td(GMIIMTXEN) Delay time, GMIIMTCLK rising edge toGMIIMTXEN 5ns 25ns

<!-- Page 209 -->
1
2
3
RMII_REFCLK
RMII_TXEN
RMII_TXD[1:0]
RMII_RXD[1:0]
RMII_CRS_DV
RMII_RX_ER6
7
119 85
4
105
209TMS570LC4357
www.ti.com SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated7.13.2 Ethernet RMII Timing
Figure 7-29. RMII Timing Diagram
Table 7-43. RMII Timing Requirements
NO. Parameter Value Unit
MIN NOM MAX
1 tc(REFCLK) Cycle time, RMII_REF_CLK - 20 - ns
2 tw(REFCLKH) Pulse width, RMII_REF_CLK High 7 - 13 ns
3 tw(REFCLKL) Pulse width, RMII_REF_CLK Low 7 - 13 ns
6 tsu(RXD-REFCLK) Input setup time, RMII_RXD valid before
RMII_REF_CLK High4 - - ns
7 th(REFCLK-RXD) Input hold time, RMII_RXD valid after
RMII_REF_CLK High2 - - ns
8 tsu(CRSDV-REFCLK) Input setup time, RMII_CRSDV valid before
RMII_REF_CLK High4 - - ns
9 th(REFCLK-CRSDV) Input hold time, RMII_CRSDV valid after
RMII_REF_CLK High2 - - ns
10 tsu(RXER-REFCLK) Input setup time, RMII_RXER valid before
RMII_REF_CLK High4 - - ns
11 th(REFCLK-RXER) Input hold time, RMII_RXER valid after
RMII_REF_CLK High2 - - ns
4 td(REFCLK-TXD) Output delay time, RMII_REF_CLK High to
RMII_TXD valid2 - 16 ns
5 td(REFCLK-TXEN) Output delay time, RMII_REF_CLK High to
RMII_TX_EN valid2 - 16 ns

<!-- Page 210 -->
MDCLK
MDIO
(output)1
7
3
MDCLK
MDIO
(input)1
3
4
5
210TMS570LC4357
SPNS195C -FEBRUARY 2014 -REVISED JUNE 2016 www.ti.com
Submit Documentation Feedback
Product Folder Links: TMS570LC4357Peripheral Information andElectrical Specifications Copyright ©2014 -2016, Texas Instruments Incorporated(1) This isadiscrepancy toIEEE 802.3, butiscompatible with many PHY devices.7.13.3 Management Data Input/Output (MDIO)
Figure 7-30. MDIO Input Timing
Table 7-44. MDIO Input Timing Requirements
NO. Parameter Value Unit
MIN MAX
1 tc(MDCLK) Cycle time, MDCLK 400 - ns
2 tw(MDCLK) Pulse duration, MDCLK high/low 180 - ns
3 tt(MDCLK) Transition time, MDCLK - 5 ns
4 tsu(MDIO-MDCLKH) Setup time, MDIO data input valid before MDCLK
High12(1)- ns
5 th(MDCLKH-MDIO) Hold time, MDIO data input valid after MDCLK
High1 - ns
Figure 7-31. MDIO Output Timing
Table 7-45. MDIO Output Timing Requirements
NO. Parameter Value Unit
MIN MAX
1 tc(MDCLK) Cycle time, MDCLK 400 - ns
7 td(MDCLKL-MDIO) Delay time, MDCLK lowtoMDIO data output
valid0 100 ns