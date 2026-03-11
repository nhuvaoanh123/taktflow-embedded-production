/**
 * @file    Rzc_Cfg.h
 * @brief   RZC configuration — all RZC-specific ID definitions
 * @date    2026-02-23
 *
 * @details Unified configuration header for the Rear Zone Controller.
 *          Contains RTE signal IDs, Com PDU IDs, DTC event IDs, E2E data IDs,
 *          motor constants, current constants, temperature constants, encoder
 *          constants, battery constants, heartbeat constants, and self-test
 *          constants.
 *
 * @safety_req SWR-RZC-001 to SWR-RZC-030
 * @traces_to  SSR-RZC-001 to SSR-RZC-017, TSR-022, TSR-030, TSR-038, TSR-046
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */
#ifndef RZC_CFG_H
#define RZC_CFG_H

/* ====================================================================
 * RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ==================================================================== */

#define RZC_SIG_TORQUE_CMD         16u
#define RZC_SIG_TORQUE_ECHO        17u
#define RZC_SIG_MOTOR_SPEED        18u
#define RZC_SIG_MOTOR_DIR          19u
#define RZC_SIG_MOTOR_ENABLE       20u
#define RZC_SIG_MOTOR_FAULT        21u
#define RZC_SIG_CURRENT_MA         22u
#define RZC_SIG_OVERCURRENT        23u
#define RZC_SIG_TEMP1_DC           24u
#define RZC_SIG_TEMP2_DC           25u
#define RZC_SIG_DERATING_PCT       26u
#define RZC_SIG_TEMP_FAULT         27u
#define RZC_SIG_BATTERY_MV         28u
#define RZC_SIG_BATTERY_STATUS     29u
#define RZC_SIG_ENCODER_SPEED      30u
#define RZC_SIG_ENCODER_DIR        31u
#define RZC_SIG_ENCODER_STALL      32u
#define RZC_SIG_VEHICLE_STATE      33u
#define RZC_SIG_ESTOP_ACTIVE       34u
#define RZC_SIG_FAULT_MASK         35u
#define RZC_SIG_SELF_TEST_RESULT   36u
#define RZC_SIG_HEARTBEAT_ALIVE    37u
#define RZC_SIG_SAFETY_STATUS      38u
#define RZC_SIG_CMD_TIMEOUT        39u
#define RZC_SIG_BATTERY_SOC        40u
/* RZC_SIG_COUNT defined below with ARXML signal IDs */

/* ====================================================================
 * Com TX PDU IDs
 * ==================================================================== */

#define RZC_COM_TX_HEARTBEAT       0u   /* CAN 0x012 */
#define RZC_COM_TX_MOTOR_STATUS    1u   /* CAN 0x300 */
#define RZC_COM_TX_MOTOR_CURRENT   2u   /* CAN 0x301 */
#define RZC_COM_TX_MOTOR_TEMP      3u   /* CAN 0x302 */
#define RZC_COM_TX_BATTERY_STATUS  4u   /* CAN 0x303 */
#define RZC_COM_TX_DTC_BROADCAST   5u   /* CAN 0x500 -- DTC broadcast */

/* ============================================================
 * ARXML-generated COM TX PDU aliases and additions
 * ============================================================ */
#define RZC_COM_TX_RZC_HEARTBEAT      RZC_COM_TX_HEARTBEAT
#define RZC_COM_TX_MOTOR_TEMPERATURE  RZC_COM_TX_MOTOR_TEMP
#define RZC_COM_TX_UDS_RESP_RZC       6u   /* CAN 0x7EA */

/* ====================================================================
 * Com RX PDU IDs
 * ==================================================================== */

#define RZC_COM_RX_ESTOP           0u   /* CAN 0x001 */
#define RZC_COM_RX_VEHICLE_TORQUE  1u   /* CAN 0x100 -- legacy alias */
#define RZC_COM_RX_VIRT_SENSORS    28u  /* CAN 0x601 -- virtual sensors from plant-sim */

/* ============================================================
 * ARXML-generated COM RX PDU IDs
 * ============================================================ */
#define RZC_COM_RX_ESTOP_BROADCAST    RZC_COM_RX_ESTOP
#define RZC_COM_RX_CVC_HEARTBEAT      1u   /* CAN 0x010 */
#define RZC_COM_RX_FZC_HEARTBEAT      2u   /* CAN 0x011 */
#define RZC_COM_RX_SC_STATUS          3u   /* CAN 0x013 */
#define RZC_COM_RX_ICU_HEARTBEAT      4u   /* CAN 0x014 */
#define RZC_COM_RX_TCU_HEARTBEAT      5u   /* CAN 0x015 */
#define RZC_COM_RX_VEHICLE_STATE      6u   /* CAN 0x100 */
#define RZC_COM_RX_TORQUE_REQUEST     7u   /* CAN 0x101 */
#define RZC_COM_RX_STEER_COMMAND      8u   /* CAN 0x102 */
#define RZC_COM_RX_BRAKE_COMMAND      9u   /* CAN 0x103 */
#define RZC_COM_RX_STEERING_STATUS   10u   /* CAN 0x200 */
#define RZC_COM_RX_BRAKE_STATUS      11u   /* CAN 0x201 */
#define RZC_COM_RX_BRAKE_FAULT       12u   /* CAN 0x210 */
#define RZC_COM_RX_MOTOR_CUTOFF_REQ  13u   /* CAN 0x211 */
#define RZC_COM_RX_LIDAR_DISTANCE    14u   /* CAN 0x220 */
#define RZC_COM_RX_BODY_CONTROL_CMD  15u   /* CAN 0x350 */
#define RZC_COM_RX_LIGHT_STATUS      16u   /* CAN 0x400 */
#define RZC_COM_RX_INDICATOR_STATE   17u   /* CAN 0x401 */
#define RZC_COM_RX_DOOR_LOCK_STATUS  18u   /* CAN 0x402 */
#define RZC_COM_RX_DTC_BROADCAST     19u   /* CAN 0x500 */
#define RZC_COM_RX_UDS_RESP_TCU      20u   /* CAN 0x644 */
#define RZC_COM_RX_UDS_FUNC_REQUEST  21u   /* CAN 0x7DF */
#define RZC_COM_RX_UDS_PHYS_REQ_CVC  22u   /* CAN 0x7E0 */
#define RZC_COM_RX_UDS_PHYS_REQ_FZC  23u   /* CAN 0x7E1 */
#define RZC_COM_RX_UDS_PHYS_REQ_RZC  24u   /* CAN 0x7E2 */
#define RZC_COM_RX_UDS_PHYS_REQ_TCU  25u   /* CAN 0x7E3 */
#define RZC_COM_RX_UDS_RESP_CVC      26u   /* CAN 0x7E8 */
#define RZC_COM_RX_UDS_RESP_FZC      27u   /* CAN 0x7E9 */

/* ====================================================================
 * Com Signal IDs for Virtual Sensors (RX from plant-sim, SIL only)
 * ==================================================================== */

#define RZC_COM_SIG_RX_VIRT_MOTOR_CURRENT   18u  /* uint16 LE, mA */
#define RZC_COM_SIG_RX_VIRT_MOTOR_TEMP      19u  /* uint16 LE, 0.1°C */
#define RZC_COM_SIG_RX_VIRT_BATTERY_VOLTAGE 20u  /* uint16 LE, mV */
#define RZC_COM_SIG_RX_VIRT_MOTOR_RPM      21u  /* uint16 LE, RPM */

/* ADC group/channel mapping for virtual sensor injection (SIL) */
#define RZC_MOTOR_CURRENT_ADC_GROUP    0u
#define RZC_MOTOR_CURRENT_ADC_CH       0u
#define RZC_MOTOR_TEMP_ADC_GROUP       1u
#define RZC_MOTOR_TEMP_ADC_CH          0u
#define RZC_BATTERY_VOLTAGE_ADC_GROUP  2u
#define RZC_BATTERY_VOLTAGE_ADC_CH     0u

/* ====================================================================
 * DTC Event IDs (Dem_EventIdType)
 * ==================================================================== */

#define RZC_DTC_OVERCURRENT        0u   /* 0xE00100 */
#define RZC_DTC_OVERTEMP           1u   /* 0xE00200 */
#define RZC_DTC_STALL              2u   /* 0xE00300 */
#define RZC_DTC_DIRECTION          3u   /* 0xE00400 */
#define RZC_DTC_SHOOT_THROUGH      4u   /* 0xE00500 */
#define RZC_DTC_CAN_BUS_OFF        5u   /* 0xE00600 */
#define RZC_DTC_CMD_TIMEOUT        6u   /* 0xE00700 */
#define RZC_DTC_SELF_TEST_FAIL     7u   /* 0xE00800 */
#define RZC_DTC_WATCHDOG_FAIL      8u   /* 0xE00900 */
#define RZC_DTC_BATTERY            9u   /* 0xE00A00 */
#define RZC_DTC_ENCODER           10u   /* 0xE00B00 */
#define RZC_DTC_ZERO_CAL          11u   /* 0xE00C00 */

/* ====================================================================
 * E2E Data IDs (per CAN message)
 * ==================================================================== */

#define RZC_E2E_HEARTBEAT_DATA_ID    0x04u
#define RZC_E2E_MOTOR_STATUS_DATA_ID 0x0Eu
#define RZC_E2E_MOTOR_CURRENT_DATA_ID 0x0Fu
#define RZC_E2E_MOTOR_TEMP_DATA_ID   0x10u
#define RZC_E2E_BATTERY_DATA_ID      0x11u
#define RZC_E2E_ESTOP_DATA_ID        0x01u   /* Matches CVC TX Data ID for 0x001 */
#define RZC_E2E_VEHSTATE_DATA_ID     0x05u   /* Matches CVC TX Data ID for 0x100 */

/* ====================================================================
 * Motor Constants (ASIL D — BTS7960 H-Bridge)
 * ==================================================================== */

/** TIM1 base frequency for 20kHz switching */
#define RZC_MOTOR_PWM_FREQ_HZ     20000u

/** Maximum duty cycle (95% cap — BTS7960 bootstrap capacitor) */
#define RZC_MOTOR_MAX_DUTY_PCT       95u

/** Dead-time between direction change (10 microseconds) */
#define RZC_MOTOR_DEADTIME_US        10u

/** Mode-based torque limits (percent of max torque) */
#define RZC_MOTOR_LIMIT_RUN         100u
#define RZC_MOTOR_LIMIT_DEGRADED     75u
#define RZC_MOTOR_LIMIT_LIMP         30u
#define RZC_MOTOR_LIMIT_SAFE_STOP     0u

/** Command timeout: 100ms with no valid torque command */
#define RZC_MOTOR_CMD_TIMEOUT_MS    100u

/** Recovery: 5 valid messages to re-enable after timeout */
#define RZC_MOTOR_CMD_RECOVERY        5u

/** PWM duty range: 0..10000 (0.01% resolution, from IoHwAb) */
#define RZC_MOTOR_PWM_SCALE       10000u

/* ====================================================================
 * Current Monitoring Constants (ASIL A — ACS723)
 * ==================================================================== */

/** Overcurrent threshold in milliamps */
#define RZC_CURRENT_OC_THRESH_MA  25000u

/** Overcurrent debounce: 10ms at 1kHz = 10 consecutive samples */
#define RZC_CURRENT_OC_DEBOUNCE      10u

/** Recovery time: 500ms below threshold */
#define RZC_CURRENT_RECOVERY_MS     500u

/** Zero-cal: 64 samples averaged at startup */
#define RZC_CURRENT_ZEROCAL_SAMPLES  64u

/** Zero-cal expected center (12-bit ADC midpoint, assumes 3.3V supply: Vcc/2 = 1.65V) */
#define RZC_CURRENT_ZEROCAL_CENTER 2048u

/** Zero-cal acceptable offset range from center */
#define RZC_CURRENT_ZEROCAL_RANGE   200u

/** ACS723 sensitivity: 100mV/A for 20A variant (ACS723LLCTR-20AB-T) */
#define RZC_CURRENT_SENSITIVITY_UV  100u

/** Moving average window size */
#define RZC_CURRENT_AVG_WINDOW        4u

/* ====================================================================
 * Temperature Monitoring Constants (ASIL A — NTC Steinhart-Hart)
 * ==================================================================== */

/** Steinhart-Hart reference temperature (25°C in Kelvin) */
#define RZC_TEMP_T0_K             29815u   /* 298.15 K * 100 (scaled) */

/** NTC reference resistance at T0 in ohms */
#define RZC_TEMP_R0_OHM           10000u

/** NTC B-coefficient */
#define RZC_TEMP_B_COEFF           3950u

/** Temperature derating curve thresholds (degrees C) */
#define RZC_TEMP_DERATE_NONE_C       60u   /* Below: 100% power */
#define RZC_TEMP_DERATE_75_C         80u   /* 60-79°C: 75% power */
#define RZC_TEMP_DERATE_50_C        100u   /* 80-99°C: 50% power */
                                           /* >= 100°C: 0% power (shutdown) */

/** Derating power percentages */
#define RZC_TEMP_DERATE_100_PCT     100u
#define RZC_TEMP_DERATE_75_PCT       75u
#define RZC_TEMP_DERATE_50_PCT       50u
#define RZC_TEMP_DERATE_0_PCT         0u

/** Recovery hysteresis (10°C lower to recover to higher power level) */
#define RZC_TEMP_HYSTERESIS_C        10u

/** Plausible NTC temperature range (deci-degrees C) */
#define RZC_TEMP_MIN_DDC           (-300)  /* -30.0°C */
#define RZC_TEMP_MAX_DDC           1500    /* 150.0°C */

/* ====================================================================
 * Encoder Constants (ASIL C — Quadrature Encoder TIM4)
 * ==================================================================== */

/** Encoder pulses per revolution */
#define RZC_ENCODER_PPR             360u

/** Stall detection: PWM > 10% but zero speed for 500ms */
#define RZC_ENCODER_STALL_TIMEOUT_MS 500u

/** Stall check period: 10ms per check = 50 checks for 500ms */
#define RZC_ENCODER_STALL_CHECKS     50u

/** Direction mismatch: commanded vs encoder for 50ms */
#define RZC_ENCODER_DIR_MISMATCH_MS  50u

/** Direction mismatch check count at 10ms period */
#define RZC_ENCODER_DIR_CHECKS        5u

/** Grace period after direction change (stall) */
#define RZC_ENCODER_STALL_GRACE_MS  200u

/** Grace period after direction change (direction plausibility) */
#define RZC_ENCODER_DIR_GRACE_MS    100u

/** Minimum PWM % to trigger stall detection */
#define RZC_ENCODER_STALL_MIN_PWM    10u

/* ====================================================================
 * Battery Monitoring Constants (QM — Voltage Divider)
 * ==================================================================== */

/** Battery disable low threshold (mV) */
#define RZC_BATT_DISABLE_LOW_MV    8000u

/** Battery warning low threshold (mV) */
#define RZC_BATT_WARN_LOW_MV      10500u

/** Battery warning high threshold (mV) */
#define RZC_BATT_WARN_HIGH_MV     15000u

/** Battery disable high threshold (mV) */
#define RZC_BATT_DISABLE_HIGH_MV  17000u

/** Hysteresis band (mV) for recovery */
#define RZC_BATT_HYSTERESIS_MV      500u

/** Voltage divider ratio: (R_H + R_L) / R_L = (47k + 10k) / 10k = 5.7 */
#define RZC_BATT_DIVIDER_RH       47000u   /* High-side resistor ohms */
#define RZC_BATT_DIVIDER_RL       10000u   /* Low-side resistor ohms */

/** Nominal battery voltage (mV) — used to seed the average buffer so
 *  the first few samples don't produce a false undervoltage fault. */
#define RZC_BATT_NOMINAL_MV       12600u

/** Moving average window */
#define RZC_BATT_AVG_WINDOW           4u

/** Battery status codes */
#define RZC_BATT_STATUS_DISABLE_LOW   0u
#define RZC_BATT_STATUS_WARN_LOW      1u
#define RZC_BATT_STATUS_NORMAL        2u
#define RZC_BATT_STATUS_WARN_HIGH     3u
#define RZC_BATT_STATUS_DISABLE_HIGH  4u

/* ====================================================================
 * RTE Period
 * ==================================================================== */

#define RZC_RTE_PERIOD_MS          50u     /* 50ms cyclic task rate */

/* ====================================================================
 * Heartbeat Constants
 * ==================================================================== */

#define RZC_HB_TX_PERIOD_MS        50u     /* TX every 50ms */
#define RZC_HB_TIMEOUT_MS         150u     /* 3x TX period */
#define RZC_HB_MAX_MISS             3u     /* Consecutive misses before timeout */
#define RZC_HB_ALIVE_MAX           15u     /* 4-bit alive counter wraps at 15 */

#define RZC_ECU_ID               0x03u     /* RZC ECU identifier */

/* ====================================================================
 * Vehicle State (received from CVC)
 * ==================================================================== */

#define RZC_STATE_INIT              0u
#define RZC_STATE_RUN               1u
#define RZC_STATE_DEGRADED          2u
#define RZC_STATE_LIMP              3u
#define RZC_STATE_SAFE_STOP         4u
#define RZC_STATE_SHUTDOWN          5u
#define RZC_STATE_COUNT             6u

/* ====================================================================
 * Self-Test Constants
 * ==================================================================== */

#define RZC_SELF_TEST_PASS          1u
#define RZC_SELF_TEST_FAIL          0u

/** Number of self-test items (8 for RZC) */
#define RZC_SELF_TEST_ITEMS         8u

/* ====================================================================
 * Fault Bitmask Positions
 * ==================================================================== */

#define RZC_FAULT_NONE           0x00u
#define RZC_FAULT_OVERCURRENT    0x01u
#define RZC_FAULT_OVERTEMP       0x02u
#define RZC_FAULT_DIRECTION      0x04u
#define RZC_FAULT_CAN            0x08u
#define RZC_FAULT_WATCHDOG       0x10u
#define RZC_FAULT_SELF_TEST      0x20u
#define RZC_FAULT_BATTERY        0x40u
#define RZC_FAULT_STALL          0x80u

/* ====================================================================
 * Motor Fault Enum
 * ==================================================================== */

#define RZC_MOTOR_NO_FAULT          0u
#define RZC_MOTOR_SHOOT_THROUGH     1u
#define RZC_MOTOR_CMD_TIMEOUT       2u
#define RZC_MOTOR_OVERCURRENT       3u
#define RZC_MOTOR_OVERTEMP          4u
#define RZC_MOTOR_STALL             5u
#define RZC_MOTOR_DIRECTION         6u

/* ====================================================================
 * Motor Direction Enum (matches IoHwAb)
 * ==================================================================== */

#define RZC_DIR_FORWARD             0u
#define RZC_DIR_REVERSE             1u
#define RZC_DIR_STOP                2u

/* ====================================================================
 * Safety WDI Pin
 * ==================================================================== */

#define RZC_SAFETY_WDI_CHANNEL      4u     /* PB4 — TPS3823 WDI pin */

/* ====================================================================
 * BTS7960 Enable Pins (independent disable path)
 * ==================================================================== */

#define RZC_MOTOR_R_EN_CHANNEL      5u     /* R_EN GPIO */
#define RZC_MOTOR_L_EN_CHANNEL      6u     /* L_EN GPIO */

/* ====================================================================
 * CAN Bus Loss Constants
 * ==================================================================== */

#define RZC_CAN_SILENCE_TIMEOUT_MS 200u    /* 200ms CAN silence -> disable */
#define RZC_CAN_ERR_WARN_TIMEOUT_MS 500u   /* Error warning > 500ms -> disable */

/* ============================================================
 * ARXML-generated signal IDs (16+ are application signals)
 * ============================================================ */
#define RZC_SIG_COUNT  178u

#define RZC_SIG_BATTERY_STATUS_BATTERY_STATUS                   16u
#define RZC_SIG_BATTERY_STATUS_BATTERY_VOLTAGE_M_V              17u
#define RZC_SIG_BATTERY_STATUS_E_2_E_ALIVE_COUNTER              18u
#define RZC_SIG_BATTERY_STATUS_E_2_E_CRC_8                      19u
#define RZC_SIG_BATTERY_STATUS_E_2_E_DATA_ID                    20u

#define RZC_SIG_BODY_CONTROL_CMD_DOOR_LOCK_CMD                  21u
#define RZC_SIG_BODY_CONTROL_CMD_HAZARD_CMD                     22u
#define RZC_SIG_BODY_CONTROL_CMD_HEADLIGHT_CMD                  23u
#define RZC_SIG_BODY_CONTROL_CMD_TAIL_LIGHT_CMD                 24u
#define RZC_SIG_BODY_CONTROL_CMD_TURN_SIGNAL_CMD                25u

#define RZC_SIG_BRAKE_COMMAND_BRAKE_FORCE_CMD                   26u
#define RZC_SIG_BRAKE_COMMAND_BRAKE_MODE                        27u
#define RZC_SIG_BRAKE_COMMAND_E_2_E_ALIVE_COUNTER               28u
#define RZC_SIG_BRAKE_COMMAND_E_2_E_CRC_8                       29u
#define RZC_SIG_BRAKE_COMMAND_E_2_E_DATA_ID                     30u
#define RZC_SIG_BRAKE_COMMAND_VEHICLE_STATE                     31u

#define RZC_SIG_BRAKE_FAULT_COMMANDED_BRAKE                     32u
#define RZC_SIG_BRAKE_FAULT_E_2_E_ALIVE_COUNTER                 33u
#define RZC_SIG_BRAKE_FAULT_E_2_E_CRC_8                         34u
#define RZC_SIG_BRAKE_FAULT_E_2_E_DATA_ID                       35u
#define RZC_SIG_BRAKE_FAULT_FAULT_TYPE                          36u
#define RZC_SIG_BRAKE_FAULT_MEASURED_BRAKE                      37u

#define RZC_SIG_BRAKE_STATUS_BRAKE_COMMAND_ECHO                 38u
#define RZC_SIG_BRAKE_STATUS_BRAKE_FAULT_STATUS                 39u
#define RZC_SIG_BRAKE_STATUS_BRAKE_MODE                         40u
#define RZC_SIG_BRAKE_STATUS_BRAKE_POSITION                     41u
#define RZC_SIG_BRAKE_STATUS_E_2_E_ALIVE_COUNTER                42u
#define RZC_SIG_BRAKE_STATUS_E_2_E_CRC_8                        43u
#define RZC_SIG_BRAKE_STATUS_E_2_E_DATA_ID                      44u
#define RZC_SIG_BRAKE_STATUS_SERVO_CURRENT_M_A                  45u

#define RZC_SIG_CVC_HEARTBEAT_E_2_E_ALIVE_COUNTER               46u
#define RZC_SIG_CVC_HEARTBEAT_E_2_E_CRC_8                       47u
#define RZC_SIG_CVC_HEARTBEAT_E_2_E_DATA_ID                     48u
#define RZC_SIG_CVC_HEARTBEAT_ECU_ID                            49u
#define RZC_SIG_CVC_HEARTBEAT_FAULT_STATUS                      50u
#define RZC_SIG_CVC_HEARTBEAT_OPERATING_MODE                    51u

#define RZC_SIG_DTC_BROADCAST_DTC_NUMBER                        52u
#define RZC_SIG_DTC_BROADCAST_DTC_STATUS                        53u
#define RZC_SIG_DTC_BROADCAST_ECU_SOURCE                        54u
#define RZC_SIG_DTC_BROADCAST_FREEZE_FRAME_0                    55u
#define RZC_SIG_DTC_BROADCAST_FREEZE_FRAME_1                    56u
#define RZC_SIG_DTC_BROADCAST_OCCURRENCE_COUNT                  57u

#define RZC_SIG_DOOR_LOCK_STATUS_CENTRAL_LOCK                   58u
#define RZC_SIG_DOOR_LOCK_STATUS_FRONT_LEFT_LOCK                59u
#define RZC_SIG_DOOR_LOCK_STATUS_FRONT_RIGHT_LOCK               60u
#define RZC_SIG_DOOR_LOCK_STATUS_REAR_LEFT_LOCK                 61u
#define RZC_SIG_DOOR_LOCK_STATUS_REAR_RIGHT_LOCK                62u

#define RZC_SIG_ESTOP_BROADCAST_E_2_E_ALIVE_COUNTER             63u
#define RZC_SIG_ESTOP_BROADCAST_E_2_E_CRC_8                     64u
#define RZC_SIG_ESTOP_BROADCAST_E_2_E_DATA_ID                   65u
#define RZC_SIG_ESTOP_BROADCAST_ESTOP_ACTIVE                    66u
#define RZC_SIG_ESTOP_BROADCAST_ESTOP_SOURCE                    67u

#define RZC_SIG_FZC_HEARTBEAT_E_2_E_ALIVE_COUNTER               68u
#define RZC_SIG_FZC_HEARTBEAT_E_2_E_CRC_8                       69u
#define RZC_SIG_FZC_HEARTBEAT_E_2_E_DATA_ID                     70u
#define RZC_SIG_FZC_HEARTBEAT_ECU_ID                            71u
#define RZC_SIG_FZC_HEARTBEAT_FAULT_STATUS                      72u
#define RZC_SIG_FZC_HEARTBEAT_OPERATING_MODE                    73u

#define RZC_SIG_ICU_HEARTBEAT_ALIVE_COUNTER                     74u
#define RZC_SIG_ICU_HEARTBEAT_E_2_E_ALIVE_COUNTER               75u
#define RZC_SIG_ICU_HEARTBEAT_E_2_E_CRC_8                       76u
#define RZC_SIG_ICU_HEARTBEAT_E_2_E_DATA_ID                     77u
#define RZC_SIG_ICU_HEARTBEAT_ECU_ID                            78u

#define RZC_SIG_INDICATOR_STATE_BLINK_STATE                     79u
#define RZC_SIG_INDICATOR_STATE_HAZARD_ACTIVE                   80u
#define RZC_SIG_INDICATOR_STATE_LEFT_INDICATOR                  81u
#define RZC_SIG_INDICATOR_STATE_RIGHT_INDICATOR                 82u

#define RZC_SIG_LIDAR_DISTANCE_DISTANCE_CM                      83u
#define RZC_SIG_LIDAR_DISTANCE_E_2_E_ALIVE_COUNTER              84u
#define RZC_SIG_LIDAR_DISTANCE_E_2_E_CRC_8                      85u
#define RZC_SIG_LIDAR_DISTANCE_E_2_E_DATA_ID                    86u
#define RZC_SIG_LIDAR_DISTANCE_OBSTACLE_ZONE                    87u
#define RZC_SIG_LIDAR_DISTANCE_SENSOR_STATUS                    88u
#define RZC_SIG_LIDAR_DISTANCE_SIGNAL_STRENGTH                  89u

#define RZC_SIG_LIGHT_STATUS_BRAKE_LIGHT_ON                     90u
#define RZC_SIG_LIGHT_STATUS_FOG_LIGHT_ON                       91u
#define RZC_SIG_LIGHT_STATUS_HEADLIGHT_LEVEL                    92u
#define RZC_SIG_LIGHT_STATUS_HEADLIGHT_ON                       93u
#define RZC_SIG_LIGHT_STATUS_TAIL_LIGHT_ON                      94u

#define RZC_SIG_MOTOR_CURRENT_CURRENT_DIRECTION                 95u
#define RZC_SIG_MOTOR_CURRENT_CURRENT_M_A                       96u
#define RZC_SIG_MOTOR_CURRENT_E_2_E_ALIVE_COUNTER               97u
#define RZC_SIG_MOTOR_CURRENT_E_2_E_CRC_8                       98u
#define RZC_SIG_MOTOR_CURRENT_E_2_E_DATA_ID                     99u
#define RZC_SIG_MOTOR_CURRENT_MOTOR_ENABLE                      100u
#define RZC_SIG_MOTOR_CURRENT_OVERCURRENT_FLAG                  101u
#define RZC_SIG_MOTOR_CURRENT_TORQUE_ECHO                       102u

#define RZC_SIG_MOTOR_CUTOFF_REQ_E_2_E_ALIVE_COUNTER            103u
#define RZC_SIG_MOTOR_CUTOFF_REQ_E_2_E_CRC_8                    104u
#define RZC_SIG_MOTOR_CUTOFF_REQ_E_2_E_DATA_ID                  105u
#define RZC_SIG_MOTOR_CUTOFF_REQ_REASON                         106u
#define RZC_SIG_MOTOR_CUTOFF_REQ_REQUEST_TYPE                   107u

#define RZC_SIG_MOTOR_STATUS_E_2_E_ALIVE_COUNTER                108u
#define RZC_SIG_MOTOR_STATUS_E_2_E_CRC_8                        109u
#define RZC_SIG_MOTOR_STATUS_E_2_E_DATA_ID                      110u
#define RZC_SIG_MOTOR_STATUS_MOTOR_DIRECTION                    111u
#define RZC_SIG_MOTOR_STATUS_MOTOR_ENABLE                       112u
#define RZC_SIG_MOTOR_STATUS_MOTOR_FAULT_STATUS                 113u
#define RZC_SIG_MOTOR_STATUS_MOTOR_SPEED_RPM                    114u
#define RZC_SIG_MOTOR_STATUS_TORQUE_ECHO                        115u

#define RZC_SIG_MOTOR_TEMPERATURE_DERATING_PERCENT              116u
#define RZC_SIG_MOTOR_TEMPERATURE_E_2_E_ALIVE_COUNTER           117u
#define RZC_SIG_MOTOR_TEMPERATURE_E_2_E_CRC_8                   118u
#define RZC_SIG_MOTOR_TEMPERATURE_E_2_E_DATA_ID                 119u
#define RZC_SIG_MOTOR_TEMPERATURE_WINDING_TEMP_1_C              120u
#define RZC_SIG_MOTOR_TEMPERATURE_WINDING_TEMP_2_C              121u

#define RZC_SIG_RZC_HEARTBEAT_E_2_E_ALIVE_COUNTER               122u
#define RZC_SIG_RZC_HEARTBEAT_E_2_E_CRC_8                       123u
#define RZC_SIG_RZC_HEARTBEAT_E_2_E_DATA_ID                     124u
#define RZC_SIG_RZC_HEARTBEAT_ECU_ID                            125u
#define RZC_SIG_RZC_HEARTBEAT_FAULT_STATUS                      126u
#define RZC_SIG_RZC_HEARTBEAT_OPERATING_MODE                    127u

#define RZC_SIG_SC_STATUS_ECU_HEALTH                            128u
#define RZC_SIG_SC_STATUS_FAULT_REASON                          129u
#define RZC_SIG_SC_STATUS_RELAY_STATE                           130u
#define RZC_SIG_SC_STATUS_SC_ALIVE_COUNTER                      131u
#define RZC_SIG_SC_STATUS_SC_CRC_8                              132u
#define RZC_SIG_SC_STATUS_SC_FAULT_FLAGS                        133u
#define RZC_SIG_SC_STATUS_SC_MODE                               134u

#define RZC_SIG_STEER_COMMAND_E_2_E_ALIVE_COUNTER               135u
#define RZC_SIG_STEER_COMMAND_E_2_E_CRC_8                       136u
#define RZC_SIG_STEER_COMMAND_E_2_E_DATA_ID                     137u
#define RZC_SIG_STEER_COMMAND_STEER_ANGLE_CMD                   138u
#define RZC_SIG_STEER_COMMAND_STEER_RATE_LIMIT                  139u
#define RZC_SIG_STEER_COMMAND_VEHICLE_STATE                     140u

#define RZC_SIG_STEERING_STATUS_ACTUAL_ANGLE                    141u
#define RZC_SIG_STEERING_STATUS_COMMANDED_ANGLE                 142u
#define RZC_SIG_STEERING_STATUS_E_2_E_ALIVE_COUNTER             143u
#define RZC_SIG_STEERING_STATUS_E_2_E_CRC_8                     144u
#define RZC_SIG_STEERING_STATUS_E_2_E_DATA_ID                   145u
#define RZC_SIG_STEERING_STATUS_SERVO_CURRENT_M_A               146u
#define RZC_SIG_STEERING_STATUS_STEER_FAULT_STATUS              147u
#define RZC_SIG_STEERING_STATUS_STEER_MODE                      148u

#define RZC_SIG_TCU_HEARTBEAT_ALIVE_COUNTER                     149u
#define RZC_SIG_TCU_HEARTBEAT_E_2_E_ALIVE_COUNTER               150u
#define RZC_SIG_TCU_HEARTBEAT_E_2_E_CRC_8                       151u
#define RZC_SIG_TCU_HEARTBEAT_E_2_E_DATA_ID                     152u
#define RZC_SIG_TCU_HEARTBEAT_ECU_ID                            153u

#define RZC_SIG_TORQUE_REQUEST_DIRECTION                        154u
#define RZC_SIG_TORQUE_REQUEST_E_2_E_ALIVE_COUNTER              155u
#define RZC_SIG_TORQUE_REQUEST_E_2_E_CRC_8                      156u
#define RZC_SIG_TORQUE_REQUEST_E_2_E_DATA_ID                    157u
#define RZC_SIG_TORQUE_REQUEST_PEDAL_FAULT                      158u
#define RZC_SIG_TORQUE_REQUEST_PEDAL_POSITION_1                 159u
#define RZC_SIG_TORQUE_REQUEST_PEDAL_POSITION_2                 160u
#define RZC_SIG_TORQUE_REQUEST_TORQUE_REQUEST                   161u

#define RZC_SIG_UDS_FUNC_REQUEST_UDS_DATA                       162u

#define RZC_SIG_UDS_PHYS_REQ_CVC_UDS_DATA                       163u
#define RZC_SIG_UDS_PHYS_REQ_FZC_UDS_DATA                       164u
#define RZC_SIG_UDS_PHYS_REQ_RZC_UDS_DATA                       165u
#define RZC_SIG_UDS_PHYS_REQ_TCU_UDS_DATA                       166u

#define RZC_SIG_UDS_RESP_CVC_UDS_DATA                           167u
#define RZC_SIG_UDS_RESP_FZC_UDS_DATA                           168u
#define RZC_SIG_UDS_RESP_RZC_UDS_DATA                           169u
#define RZC_SIG_UDS_RESP_TCU_UDS_DATA                           170u

#define RZC_SIG_VEHICLE_STATE_E_2_E_ALIVE_COUNTER               171u
#define RZC_SIG_VEHICLE_STATE_E_2_E_CRC_8                       172u
#define RZC_SIG_VEHICLE_STATE_E_2_E_DATA_ID                     173u
#define RZC_SIG_VEHICLE_STATE_FAULT_MASK                        174u
#define RZC_SIG_VEHICLE_STATE_SPEED_LIMIT                       175u
#define RZC_SIG_VEHICLE_STATE_TORQUE_LIMIT                      176u
#define RZC_SIG_VEHICLE_STATE_VEHICLE_STATE                     177u

/* Det module and API IDs are in Det_ErrIds.h */

#endif /* RZC_CFG_H */
