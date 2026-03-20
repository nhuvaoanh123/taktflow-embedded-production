/**
 * @file    can_diag_test.c
 * @brief   Standalone bxCAN diagnostic test for F413ZH
 * @date    2026-03-20
 *
 * Isolated test — call from main() BEFORE any BSW init.
 * Tests CAN in stages, printing register dumps between each.
 * Safe for bus: stays in loopback unless explicitly told to go normal.
 *
 * Usage: call Can_DiagTest(0) for loopback-only (safe)
 *        call Can_DiagTest(1) for loopback + normal mode (may disrupt bus)
 */

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* Forward: UART print from rzc_f4_hw_stm32f4.c */
extern void Dbg_Uart_Print(const char* str);

static void hex32(uint32_t val, char* buf)
{
    static const char h[] = "0123456789ABCDEF";
    int i;
    for (i = 7; i >= 0; i--)
    {
        buf[i] = h[val & 0xFu];
        val >>= 4;
    }
    buf[8] = '\0';
}

static void print_reg(const char* name, uint32_t val)
{
    char buf[9];
    Dbg_Uart_Print("  ");
    Dbg_Uart_Print(name);
    Dbg_Uart_Print("=0x");
    hex32(val, buf);
    Dbg_Uart_Print(buf);
    Dbg_Uart_Print("\r\n");
}

static void dump_can_regs(const char* phase)
{
    Dbg_Uart_Print("--- ");
    Dbg_Uart_Print(phase);
    Dbg_Uart_Print(" ---\r\n");
    print_reg("MCR ", CAN1->MCR);
    print_reg("MSR ", CAN1->MSR);
    print_reg("TSR ", CAN1->TSR);
    print_reg("ESR ", CAN1->ESR);
    print_reg("BTR ", CAN1->BTR);
    print_reg("RF0R", CAN1->RF0R);

    /* GPIO PD0/PD1 state */
    print_reg("GPIOD_MODER", GPIOD->MODER);
    print_reg("GPIOD_AFRL ", GPIOD->AFR[0]);
    print_reg("GPIOD_IDR  ", GPIOD->IDR);
    print_reg("GPIOD_OTYPER", GPIOD->OTYPER);
    print_reg("GPIOD_PUPDR", GPIOD->PUPDR);

    /* Check RCC CAN1 clock */
    print_reg("RCC_APB1ENR", RCC->APB1ENR);
}

static CAN_HandleTypeDef hcan_diag;

/**
 * @brief  Full CAN diagnostic test
 * @param  try_normal  0 = loopback only (safe), 1 = also try normal mode
 */
void Can_DiagTest(int try_normal)
{
    CAN_FilterTypeDef filter;
    CAN_TxHeaderTypeDef txH;
    CAN_RxHeaderTypeDef rxH;
    uint8_t txD[8] = {0xCA, 0xFE, 0xBA, 0xBE, 0xDE, 0xAD, 0x01, 0x02};
    uint8_t rxD[8];
    uint32_t txMb;
    uint32_t tick;

    Dbg_Uart_Print("\r\n========== CAN DIAG TEST ==========\r\n");

    /* Step 0: Dump registers BEFORE any CAN init */
    dump_can_regs("BEFORE CAN INIT");

    /* Step 1: Enable clocks manually */
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    Dbg_Uart_Print("Step 1: Clocks enabled\r\n");

    /* Step 2: Configure GPIO PD0/PD1 manually (not via MspInit) */
    {
        GPIO_InitTypeDef gpio;
        gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_NOPULL;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOD, &gpio);
    }
    Dbg_Uart_Print("Step 2: GPIO PD0/PD1 AF9 configured\r\n");
    dump_can_regs("AFTER GPIO INIT");

    /* Step 3: HAL_CAN_Init in LOOPBACK mode */
    hcan_diag.Instance                = CAN1;
    hcan_diag.Init.Prescaler          = 6u;
    hcan_diag.Init.Mode               = CAN_MODE_LOOPBACK;
    hcan_diag.Init.SyncJumpWidth      = CAN_SJW_1TQ;
    hcan_diag.Init.TimeSeg1           = CAN_BS1_13TQ;
    hcan_diag.Init.TimeSeg2           = CAN_BS2_2TQ;
    hcan_diag.Init.TimeTriggeredMode  = DISABLE;
    hcan_diag.Init.AutoBusOff         = ENABLE;
    hcan_diag.Init.AutoWakeUp         = ENABLE;
    hcan_diag.Init.AutoRetransmission = ENABLE;
    hcan_diag.Init.ReceiveFifoLocked  = DISABLE;
    hcan_diag.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan_diag) != HAL_OK)
    {
        Dbg_Uart_Print("Step 3: HAL_CAN_Init LOOPBACK FAILED\r\n");
        dump_can_regs("INIT FAILED");
        return;
    }
    Dbg_Uart_Print("Step 3: HAL_CAN_Init LOOPBACK OK\r\n");

    /* Clear sleep */
    CAN1->MCR &= ~CAN_MCR_SLEEP;
    while ((CAN1->MSR & CAN_MSR_SLAK) != 0u) {}
    Dbg_Uart_Print("Step 3b: Sleep cleared\r\n");
    dump_can_regs("AFTER LOOPBACK INIT");

    /* Step 4: Configure filter */
    filter.FilterBank           = 0u;
    filter.FilterMode           = CAN_FILTERMODE_IDMASK;
    filter.FilterScale          = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh         = 0x0000u;
    filter.FilterIdLow          = 0x0000u;
    filter.FilterMaskIdHigh     = 0x0000u;
    filter.FilterMaskIdLow      = 0x0000u;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation     = ENABLE;
    filter.SlaveStartFilterBank = 14u;

    if (HAL_CAN_ConfigFilter(&hcan_diag, &filter) != HAL_OK)
    {
        Dbg_Uart_Print("Step 4: Filter config FAILED\r\n");
        return;
    }
    Dbg_Uart_Print("Step 4: Filter OK\r\n");

    /* Step 5: Start CAN */
    if (HAL_CAN_Start(&hcan_diag) != HAL_OK)
    {
        Dbg_Uart_Print("Step 5: HAL_CAN_Start FAILED\r\n");
        dump_can_regs("START FAILED");
        return;
    }
    Dbg_Uart_Print("Step 5: CAN Started in LOOPBACK\r\n");
    dump_can_regs("AFTER START LOOPBACK");

    /* Step 6: TX test frame in loopback */
    txH.StdId = 0x7FFu;
    txH.ExtId = 0u;
    txH.IDE   = CAN_ID_STD;
    txH.RTR   = CAN_RTR_DATA;
    txH.DLC   = 8u;
    txH.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan_diag, &txH, txD, &txMb) != HAL_OK)
    {
        Dbg_Uart_Print("Step 6: TX FAILED\r\n");
        dump_can_regs("TX FAILED");
        return;
    }
    Dbg_Uart_Print("Step 6: TX queued\r\n");

    /* Step 7: Wait for RX in loopback */
    tick = HAL_GetTick();
    while ((HAL_GetTick() - tick) < 100u)
    {
        if (HAL_CAN_GetRxFifoFillLevel(&hcan_diag, CAN_RX_FIFO0) > 0u)
        {
            if (HAL_CAN_GetRxMessage(&hcan_diag, CAN_RX_FIFO0, &rxH, rxD) == HAL_OK)
            {
                if (rxH.StdId == 0x7FFu && rxD[0] == 0xCAu && rxD[1] == 0xFEu)
                {
                    Dbg_Uart_Print("Step 7: LOOPBACK RX OK — CAN peripheral WORKS\r\n");
                    break;
                }
                else
                {
                    Dbg_Uart_Print("Step 7: RX data MISMATCH\r\n");
                }
            }
        }
    }
    if ((HAL_GetTick() - tick) >= 100u)
    {
        Dbg_Uart_Print("Step 7: LOOPBACK RX TIMEOUT\r\n");
        dump_can_regs("RX TIMEOUT");
        return;
    }

    dump_can_regs("LOOPBACK PASS");

    if (!try_normal)
    {
        Dbg_Uart_Print("=== LOOPBACK ONLY — not touching normal mode (safe) ===\r\n");
        HAL_CAN_Stop(&hcan_diag);
        return;
    }

    /* Step 8: Switch to NORMAL mode */
    Dbg_Uart_Print("Step 8: Switching to NORMAL mode...\r\n");
    HAL_CAN_Stop(&hcan_diag);
    HAL_CAN_DeInit(&hcan_diag);

    hcan_diag.Init.Mode = CAN_MODE_NORMAL;
    if (HAL_CAN_Init(&hcan_diag) != HAL_OK)
    {
        Dbg_Uart_Print("Step 8: NORMAL init FAILED\r\n");
        dump_can_regs("NORMAL INIT FAILED");
        return;
    }

    CAN1->MCR &= ~CAN_MCR_SLEEP;
    {
        volatile uint32_t to = 100000u;
        while (((CAN1->MSR & CAN_MSR_SLAK) != 0u) && (to > 0u)) { to--; }
        if (to == 0u)
        {
            Dbg_Uart_Print("Step 8: SLEEP clear TIMEOUT in normal mode\r\n");
            Dbg_Uart_Print("  This means RX pin sees dominant — no recessive from transceiver\r\n");
            dump_can_regs("SLEEP STUCK");
            return;
        }
    }

    HAL_CAN_ConfigFilter(&hcan_diag, &filter);

    if (HAL_CAN_Start(&hcan_diag) != HAL_OK)
    {
        Dbg_Uart_Print("Step 8: NORMAL start FAILED\r\n");
        dump_can_regs("NORMAL START FAILED");
        return;
    }

    Dbg_Uart_Print("Step 8: CAN NORMAL mode started\r\n");
    dump_can_regs("AFTER NORMAL START");

    /* Step 9: TX in normal mode */
    txH.StdId = 0x558u;
    txD[0] = 0xF4; txD[1] = 0x13; /* F413 identifier */

    if (HAL_CAN_AddTxMessage(&hcan_diag, &txH, txD, &txMb) != HAL_OK)
    {
        Dbg_Uart_Print("Step 9: NORMAL TX FAILED\r\n");
        return;
    }
    Dbg_Uart_Print("Step 9: TX 0x558 queued in normal mode\r\n");

    /* Wait for TX complete or error */
    tick = HAL_GetTick();
    while ((HAL_GetTick() - tick) < 200u)
    {
        uint32_t tsr = CAN1->TSR;
        uint32_t esr = CAN1->ESR;

        if ((tsr & CAN_TSR_RQCP0) != 0u)
        {
            if ((tsr & CAN_TSR_TXOK0) != 0u)
            {
                Dbg_Uart_Print("Step 9: TX SUCCESS — frame sent on bus!\r\n");
            }
            else
            {
                Dbg_Uart_Print("Step 9: TX COMPLETE but TXOK=0 — arbitration lost or error\r\n");
            }
            dump_can_regs("AFTER NORMAL TX");
            break;
        }

        /* Check for bus-off */
        if ((esr & CAN_ESR_BOFF) != 0u)
        {
            Dbg_Uart_Print("Step 9: BUS-OFF detected!\r\n");
            dump_can_regs("BUS-OFF");
            break;
        }
    }

    if ((HAL_GetTick() - tick) >= 200u)
    {
        Dbg_Uart_Print("Step 9: TX TIMEOUT — no ACK from bus (no other node?)\r\n");
        dump_can_regs("TX TIMEOUT");
    }

    Dbg_Uart_Print("========== CAN DIAG DONE ==========\r\n");
}
