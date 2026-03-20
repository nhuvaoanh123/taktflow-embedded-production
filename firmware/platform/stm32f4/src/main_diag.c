/**
 * @file    main_diag.c
 * @brief   Minimal diagnostic main for F413ZH CAN bringup
 * @date    2026-03-20
 *
 * Build: make -f firmware/platform/stm32f4/Makefile.stm32f4 TARGET=diag
 * Flash: make -f firmware/platform/stm32f4/Makefile.stm32f4 TARGET=diag flash
 */

#include "stm32f4xx_hal.h"

/* From rzc_f4_hw_stm32f4.c */
extern void Dbg_Uart_Print(const char* str);

/* From can_diag_test.c */
extern void Can_DiagTest(int try_normal);

/* From rzc_f4_hw_stm32f4.c clock config */
static void Ecu_SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM       = 16u;
    RCC_OscInitStruct.PLL.PLLN       = 384u;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ       = 8u;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) { for(;;); }

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) { for(;;); }
}

static void Dbg_Uart_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    (void)RCC->APB1ENR;

    GPIOD->MODER = (GPIOD->MODER & ~(3u << (8u * 2u))) | (2u << (8u * 2u));
    GPIOD->AFR[1] = (GPIOD->AFR[1] & ~(0xFu << (0u * 4u))) | (7u << (0u * 4u));

    USART3->BRR = 417u;
    USART3->CR1 = USART_CR1_TE | USART_CR1_UE;
    while ((USART3->SR & USART_SR_TC) == 0u) {}
}

void Dbg_Uart_Print(const char* str)
{
    while (*str)
    {
        while ((USART3->SR & USART_SR_TXE) == 0u) {}
        USART3->DR = (uint16_t)*str++;
    }
}

/* LED blink to show we're alive */
static void Blink_Init(void)
{
    /* PB0 = LD1 (green), PB7 = LD2 (blue), PB14 = LD3 (red) on NUCLEO-F413ZH */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER = (GPIOB->MODER & ~(3u << (0u * 2u))) | (1u << (0u * 2u));   /* PB0 output */
    GPIOB->MODER = (GPIOB->MODER & ~(3u << (14u * 2u))) | (1u << (14u * 2u));  /* PB14 output */
}

static void Led_Green(int on) { if (on) GPIOB->BSRR = (1u << 0); else GPIOB->BSRR = (1u << 16); }
static void Led_Red(int on)   { if (on) GPIOB->BSRR = (1u << 14); else GPIOB->BSRR = (1u << 30); }

/* Stubs required by HAL */
void Error_Handler(void) { for(;;); }
void SysTick_Handler(void) { HAL_IncTick(); }

int main(void)
{
    HAL_Init();
    Ecu_SystemClock_Config();
    Dbg_Uart_Init();
    Blink_Init();

    Led_Green(1);
    Dbg_Uart_Print("\r\n=== F413ZH CAN DIAG TEST ===\r\n");
    Dbg_Uart_Print("Clock: HSI 16MHz -> PLL -> 96MHz\r\n");
    Dbg_Uart_Print("UART: USART3 PD8 115200\r\n\r\n");

    /* Run CAN diag — 0 = loopback only (safe), 1 = also try normal */
    Can_DiagTest(0);

    Dbg_Uart_Print("\r\nWaiting 3s before normal mode test...\r\n");
    HAL_Delay(3000);

    Can_DiagTest(1);

    /* Blink LED forever to show test complete */
    Dbg_Uart_Print("\r\nTest complete. LED blinking.\r\n");
    while (1)
    {
        Led_Green(1); Led_Red(0); HAL_Delay(200);
        Led_Green(0); Led_Red(1); HAL_Delay(200);
    }
}
