/**
 * @file    bsw_stubs.c
 * @brief   Stubs for BSW/MCAL modules not yet on ThreadX CVC experiment
 */
#include "Std_Types.h"
#include "ComStack_Types.h"

/* Os stubs */
void Os_Init(void) {}
void StartOS(uint8 mode) { (void)mode; }

/* CanTp stubs */
void CanTp_Init(const void* cfg) { (void)cfg; }
void CanTp_RxIndication(PduIdType id, const PduInfoType* info) { (void)id; (void)info; }
void CanTp_TxConfirmation(PduIdType id) { (void)id; }
void CanTp_MainFunction(void) {}

/* Dcm stubs */
void Dcm_Init(const void* cfg) { (void)cfg; }
void Dcm_RxIndication(PduIdType id, Std_ReturnType result) { (void)id; (void)result; }
void Dcm_TxConfirmation(PduIdType id, Std_ReturnType result) { (void)id; (void)result; }
void Dcm_MainFunction(void) {}

/* BswM / WdgM / Dem / NvM stubs */
void BswM_Init(const void* cfg) { (void)cfg; }
void BswM_MainFunction(void) {}
void BswM_RequestMode(uint16 user, uint16 mode) { (void)user; (void)mode; }
void WdgM_Init(const void* cfg) { (void)cfg; }
void WdgM_MainFunction(void) {}
void WdgM_UpdateAliveCounter(uint16 id) { (void)id; }
void WdgM_CheckpointReached(uint16 seId, uint16 cpId) { (void)seId; (void)cpId; }
void Dem_Init(const void* cfg) { (void)cfg; }
void Dem_ReportErrorStatus(uint16 id, uint8 status) { (void)id; (void)status; }
void Dem_MainFunction(void) {}
void NvM_Init(const void* cfg) { (void)cfg; }
void NvM_MainFunction(void) {}
Std_ReturnType NvM_ReadBlock(uint16 id, void* dst) { (void)id; (void)dst; return E_OK; }
Std_ReturnType NvM_WriteBlock(uint16 id, const void* src) { (void)id; (void)src; return E_OK; }

/* IoHwAb stubs — CVC reads pedal, E-Stop, battery via IoHwAb */
void IoHwAb_Init(const void* cfg) { (void)cfg; }
uint16 IoHwAb_ReadPedalPosition(void) { return 0u; }
uint16 IoHwAb_ReadPedalAngle(void) { return 0u; }

/* SelfTest stub */
void Swc_SelfTest_Init(void) {}
void Swc_SelfTest_MainFunction(void) {}
uint8  IoHwAb_ReadEStop(void) { return 0u; }
uint16 IoHwAb_ReadBatteryVoltage(void) { return 12600u; }
sint16 IoHwAb_ReadMotorTemp(void) { return 250; }
uint16 IoHwAb_ReadMotorCurrent(void) { return 0u; }
uint32 IoHwAb_ReadEncoderCount(void) { return 0u; }
uint8  IoHwAb_ReadEncoderDirection(void) { return 0u; }
void   IoHwAb_SetMotorPWM(uint8 dir, uint16 duty) { (void)dir; (void)duty; }

/* MCAL stubs */
void Spi_Init(const void* cfg) { (void)cfg; }
void Adc_Init(const void* cfg) { (void)cfg; }
uint16 Adc_ReadChannel(uint8 ch) { (void)ch; return 0u; }
void Dio_WriteChannel(uint8 ch, uint8 val) { (void)ch; (void)val; }
uint8 Dio_ReadChannel(uint8 ch) { (void)ch; return 0u; }
void Pwm_SetDutyCycle(uint8 ch, uint16 duty) { (void)ch; (void)duty; }
void Spi_Hw_Transmit(uint8 ch, const uint8* tx, uint8* rx, uint16 len) { (void)ch; (void)tx; (void)rx; (void)len; }
void Spi_Hw_PollUdp(void) {}

/* SSD1306 HW stub — I2C not available on ThreadX experiment */
Std_ReturnType Ssd1306_Hw_I2cWrite(uint8 addr, const uint8* data, uint16 len) { (void)addr; (void)data; (void)len; return E_OK; }
