/**
 * @file    Uart_Hw_STM32.c
 * @brief   STM32G4 USART3 hardware backend for UART MCAL driver
 * @date    2026-03-23
 *
 * @details Bare-metal USART3 driver for TFMini-S lidar on Nucleo-G474RE.
 *          PB10 = TX (AF7), PB11 = RX (AF7), 115200 baud @ 170 MHz APB1.
 *          Interrupt-driven circular RX buffer (64 bytes).
 *
 *          Uses USART3 instead of USART3 to avoid conflict with Nucleo
 *          VCP solder bridges (SB63/SB65 route PA2/PA3 to ST-LINK).
 *          PB10/PB11 are free on Morpho connector CN10 pins 32/34.
 *
 * @safety_req SWR-BSW-010: UART Driver for TFMini-S Lidar
 * @copyright Taktflow Systems 2026
 */

#include "Platform_Types.h"
#include "Std_Types.h"
#include "Uart.h"
#include "stm32g4xx.h"

/* ==================================================================
 * Circular RX Buffer
 * ================================================================== */

static volatile uint8  rx_buf[UART_RX_BUF_SIZE];
static volatile uint8  rx_head;  /* ISR writes here */
static volatile uint8  rx_tail;  /* API reads from here */
static volatile boolean uart_hw_initialized = FALSE;

/* ==================================================================
 * USART3 IRQ Handler
 * ================================================================== */

void USART3_IRQHandler(void)
{
    /* RXNE: data received */
    if ((USART3->ISR & USART_ISR_RXNE_RXFNE) != 0u)
    {
        uint8 byte = (uint8)(USART3->RDR & 0xFFu);
        uint8 next_head = (rx_head + 1u) % UART_RX_BUF_SIZE;

        if (next_head != rx_tail)  /* Buffer not full */
        {
            rx_buf[rx_head] = byte;
            rx_head = next_head;
        }
    }

    /* ORE: overrun error — clear flag */
    if ((USART3->ISR & USART_ISR_ORE) != 0u)
    {
        USART3->ICR = USART_ICR_ORECF;
    }

    /* FE/NE: framing/noise error — clear flags */
    if ((USART3->ISR & USART_ISR_FE) != 0u)
    {
        USART3->ICR = USART_ICR_FECF;
    }
    if ((USART3->ISR & USART_ISR_NE) != 0u)
    {
        USART3->ICR = USART_ICR_NECF;
    }
}

/* ==================================================================
 * API Implementation
 * ================================================================== */

Std_ReturnType Uart_Hw_Init(uint32 baudRate)
{
    /* Reset buffer */
    rx_head = 0u;
    rx_tail = 0u;

    /* Enable clocks: GPIOB + USART3 */
    RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

    /* Configure PB10 (TX) and PB11 (RX) as alternate function AF7
     * MODER: bits [21:20]=10 (PB10), bits [23:22]=10 (PB11) → AF mode */
    GPIOB->MODER = (GPIOB->MODER & ~(0xFu << 20)) | (0xAu << 20);

    /* AFR[1]: PB10=AF7 (bits [11:8]), PB11=AF7 (bits [15:12]) */
    GPIOB->AFR[1] = (GPIOB->AFR[1] & ~(0xFFu << 8)) | (0x77u << 8);

    /* Pull-up on PB11 (RX) for idle-high UART line */
    GPIOB->PUPDR = (GPIOB->PUPDR & ~(3u << 22)) | (1u << 22);

    /* Configure USART3: 115200 8N1, RX + TX, RXNEIE interrupt */
    USART3->CR1 = 0u;  /* Disable USART first */

    /* BRR = fclk / baudrate. APB1 = 170 MHz on G474. */
    USART3->BRR = (170000000u + baudRate / 2u) / baudRate;

    /* CR2: 1 stop bit (default) */
    USART3->CR2 = 0u;

    /* CR3: no flow control, no DMA (interrupt-driven) */
    USART3->CR3 = 0u;

    /* CR1: UE=1, RE=1, TE=1, RXNEIE=1 (interrupt on RX) */
    USART3->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE
                | USART_CR1_RXNEIE_RXFNEIE;

    /* Enable USART3 interrupt in NVIC */
    NVIC_SetPriority(USART3_IRQn, 5u);
    NVIC_EnableIRQ(USART3_IRQn);

    uart_hw_initialized = TRUE;
    return E_OK;
}

void Uart_Hw_DeInit(void)
{
    NVIC_DisableIRQ(USART3_IRQn);
    USART3->CR1 = 0u;  /* Disable USART */
    uart_hw_initialized = FALSE;
}

uint8 Uart_Hw_GetRxCount(void)
{
    if (uart_hw_initialized == FALSE)
    {
        return 0u;
    }

    /* Compute bytes in circular buffer */
    uint8 h = rx_head;
    uint8 t = rx_tail;
    return (uint8)((h - t + UART_RX_BUF_SIZE) % UART_RX_BUF_SIZE);
}

Std_ReturnType Uart_Hw_ReadRx(uint8* Buffer, uint8 Length, uint8* BytesRead)
{
    uint8 count = 0u;

    if ((Buffer == NULL_PTR) || (BytesRead == NULL_PTR))
    {
        return E_NOT_OK;
    }

    if (uart_hw_initialized == FALSE)
    {
        *BytesRead = 0u;
        return E_NOT_OK;
    }

    while ((count < Length) && (rx_tail != rx_head))
    {
        Buffer[count] = rx_buf[rx_tail];
        rx_tail = (rx_tail + 1u) % UART_RX_BUF_SIZE;
        count++;
    }

    *BytesRead = count;
    return E_OK;
}

uint8 Uart_Hw_GetStatus(void)
{
    if (uart_hw_initialized == FALSE)
    {
        return 0u;  /* UART_UNINIT */
    }
    return 1u;  /* UART_IDLE */
}
