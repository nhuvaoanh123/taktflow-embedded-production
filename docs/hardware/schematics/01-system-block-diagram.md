# 01 — System Block Diagram

**Block**: Top-level system overview
**Source**: HWDES Section 4

## System Block Diagram

```
  +========================================================================+
  |                         12V BENCH SUPPLY (10A)                         |
  +====+==============+============+============+============+=============+
       |              |            |            |            |
   [SB560 Schottky - Reverse Polarity Protection]
       |
       +-- [10A Fuse] -- 12V MAIN RAIL
       |
       +------+------+------+------+------+------+
       |      |      |      |      |      |      |
    LM2596  LM2596  Nucleo Nucleo Nucleo  LP     |
    12->5V  12->3.3V LDO   LDO    LDO   LDO    |
     |       |      (CVC)  (FZC)  (RZC)  (SC)   |
     |       |       |      |      |      |      |
    5V      3.3V    3.3V   3.3V   3.3V   3.3V   |
    Rail    Rail     |      |      |      |      |
     |       |       |      |      |      |      |
   TFMini  AS5048A  CVC    FZC    RZC    SC     |
   RPi4    ACS723   MCU    MCU    MCU    MCU    |
           TJA1051                SN65HVD         |
           TPS3823                                |
           NTC divs                               |
                                                  |
       +--[Kill Relay (30A SPST-NO)]--[30A Fuse]--+
       |
       +-- 12V ACTUATOR RAIL (gated)
       |
       +------+------+------+
       |      |      |      |
    BTS7960  Steer  Brake  (Relay coil
    Motor    Servo  Servo   is on 12V
    Driver   6V reg 6V reg  main rail)
     |
    DC Motor (12V)

  ==========+===========+===========+===========+=====  CAN BUS (500 kbps)
            |           |           |           |
       +----+----+ +----+----+ +----+----+ +----+----+
       |  CVC    | |  FZC    | |  RZC    | |   SC    |
       | TJA1051 | | TJA1051 | | TJA1051 | | SN65HVD |
       | [120R]  | |         | |         | | [120R]  |
       +---------+ +---------+ +---------+ +---------+
                                                |
                                           CANable 2.0
                                           (RPi USB)
                                                |
                                         Raspberry Pi 4
                                                |
                                           MQTT/TLS
                                                |
                                            AWS Cloud
                                         (IoT Core ->
                                          Timestream ->
                                           Grafana)
```

## Key Points

- **Two power domains**: 12V main rail (always on) and 12V actuator rail (gated by kill relay)
- **SC powered independently** of kill relay — maintains fault monitoring even when actuators are off
- **CAN bus**: 500 kbps, 120 ohm termination at CVC (start) and SC (end)
- **Reverse polarity protection**: SB560 Schottky at PSU entry

## Design Decisions Explained

### Why two separate power domains (main rail vs actuator rail)?

**Safety reason.** If the motor or servo malfunctions (short circuit, runaway), the kill relay can cut power to ALL actuators instantly — while keeping the MCUs, sensors, and safety controller alive. This is the "energize-to-run" pattern: the relay must be actively held closed by the Safety Controller. If anything goes wrong (SC crash, power loss, firmware hang), the relay opens automatically and actuators lose power. This is the most basic automotive safety pattern — the safe state is "everything off."

### Why is the SC powered independently of the kill relay?

The Safety Controller's job is to monitor the other ECUs and cut power if something goes wrong. If the SC lost power when the relay opened, it couldn't log the fault, maintain LED indication, or decide when to re-enable power. The SC must survive any fault condition to remain the system's last line of defense.

### Why SB560 Schottky diode for reverse polarity protection?

If you accidentally connect the power supply backwards (+ to - and - to +), every chip on the board would be destroyed instantly. The SB560 blocks reverse current — it only conducts in the correct direction. We use a Schottky type (not a regular diode) because Schottky diodes have a lower voltage drop (~0.5V vs ~0.7V), wasting less power as heat. At 8.5A peak, even 0.2V difference means ~1.7W less heat.

### Why 120 ohm termination at CVC and SC only (not FZC/RZC)?

CAN bus is a differential pair (CAN_H and CAN_L). Without proper termination at both physical ends of the bus, signals reflect back like echoes, corrupting data. The standard requires exactly two 120-ohm resistors — one at each end of the bus. CVC is at one end, SC is at the other end. FZC and RZC are in the middle, so they don't get termination resistors. Adding extra terminators would lower the total impedance and distort the signal.

### Why CAN bus at 500 kbps (not faster like 1 Mbps)?

500 kbps is the standard automotive CAN speed for powertrain/chassis networks. It's reliable at the bus lengths we use (~700mm total). Going faster (1 Mbps) would require shorter bus lengths and tighter timing. Our bus is short enough for 1 Mbps, but 500 kbps is the industry standard that matches real vehicles, and our data rates don't need more bandwidth.

### Why separate Nucleo LDOs for each MCU instead of one shared 3.3V regulator?

Each Nucleo board has its own onboard voltage regulator (LDO). Using each board's built-in LDO means: (1) if one board shorts out, it doesn't take down the others, (2) each board is self-contained and can be tested independently, (3) we avoid the complexity of routing high-current 3.3V rails to multiple boards. The external 3.3V rail from the LM2596 only powers shared peripherals (sensors, transceivers).

### Why Raspberry Pi 4 as the edge gateway (not an ESP32 or another MCU)?

The Pi runs a full Linux OS, which makes it easy to run Python, MQTT, Docker containers, and ML inference — things that are impractical on a bare-metal MCU. It also has USB ports for the CANable adapters. For a demo/portfolio platform, the Pi's ecosystem (libraries, tools, community support) saves enormous development time compared to building everything from scratch on an MCU.

## Pin References

| ECU | CAN TX | CAN RX | Transceiver |
|-----|--------|--------|-------------|
| CVC | PA12 (AF9) | PA11 (AF9) | TJA1051T/3 |
| FZC | PA12 (AF9) | PA11 (AF9) | TJA1051T/3 |
| RZC | PA12 (AF9) | PA11 (AF9) | TJA1051T/3 |
| SC | DCAN1TX (J5) | DCAN1RX (J5) | SN65HVD230 |

## BOM References

| Component | BOM # |
|-----------|-------|
| STM32G474RE Nucleo-64 (x3) | #1 |
| TMS570LC43x LaunchPad | #2 |
| Raspberry Pi 4 | #3 |
| TJA1051T/3 (x3) | #6 |
| SN65HVD230 | #7 |
| CANable 2.0 (x2) | #8 |
| 120 ohm terminators | #9 |
| SB560 Schottky | #36 |
| 10A ATC fuse | #38 |
| 30A ATC fuse | #39 |
| Kill relay (30A SPST-NO) | #24 |
| 12V bench PSU | #34 |
| LM2596 buck converters (x2) | #35 |
