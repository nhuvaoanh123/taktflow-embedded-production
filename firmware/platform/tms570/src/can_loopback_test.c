/**
 * @file    can_loopback_test.c
 * @brief   Minimal DCAN1 loopback + external TX test for TMS570 LaunchPad
 *
 * Replaces main(). Tests:
 *   1. DCAN1 internal loopback (TX→RX without bus)
 *   2. DCAN1 external TX (sends 0x013 on bus every 500ms)
 *
 * LED2 (GIOB6): ON = loopback passed
 * LED3 (GIOB7): toggles every TX = external CAN working
 */

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_gio.h"
#include "HL_can.h"

/* DCAN1 base */
#define DCAN1_BASE  0xFFF7DC00u

static volatile uint32 *dcan1 = (volatile uint32 *)DCAN1_BASE;

/* Register offsets (word index) */
#define CTL     (0x00u/4u)
#define ES      (0x04u/4u)
#define BTR     (0x0Cu/4u)
#define TEST    (0x14u/4u)
#define IF1CMD  (0x40u/4u)
#define IF1MSK  (0x44u/4u)
#define IF1ARB  (0x48u/4u)
#define IF1MCTL (0x4Cu/4u)
#define IF1DATA (0x50u/4u)
#define IF1DATB (0x54u/4u)
#define IF2CMD  (0x80u/4u)
#define IF2MSK  (0x84u/4u)
#define IF2ARB  (0x88u/4u)
#define IF2MCTL (0x8Cu/4u)
#define IF2DATA (0x90u/4u)
#define IF2DATB (0x94u/4u)

static void delay(volatile uint32 count)
{
    while (count > 0u) { count--; }
}

static void wait_if1(void)
{
    volatile uint32 t = 100000u;
    while ((dcan1[IF1CMD] & 0x8000u) && (t > 0u)) { t--; }
}

static void wait_if2(void)
{
    volatile uint32 t = 100000u;
    while ((dcan1[IF2CMD] & 0x8000u) && (t > 0u)) { t--; }
}

/**
 * @brief  Init DCAN1 in specified mode with 500 kbps
 * @param  mode  0=normal, 1=loopback (TEST.LBACK)
 */
static void can_init(uint32 mode)
{
    /* Enter init + CCE */
    dcan1[CTL] = 0x41u;
    delay(1000u);

    /* BTR: 500 kbps at VCLK=75 MHz
     * BRP=9, TSEG1=9, TSEG2=3, SJW=3 */
    dcan1[BTR] = 9u | (3u << 6u) | (9u << 8u) | (3u << 12u);

    if (mode == 1u) {
        /* Enable test mode + loopback */
        dcan1[CTL] = 0x41u | (1u << 7u);  /* Init + CCE + Test */
        dcan1[TEST] = (1u << 4u);          /* LBACK */
    }

    /* Exit init */
    dcan1[CTL] = (mode == 1u) ? (1u << 7u) : 0u;
    delay(1000u);
}

/**
 * @brief  Configure message object as RX (accept all)
 */
static void can_setup_rx(uint8 obj)
{
    wait_if1();
    dcan1[IF1MSK]  = 0u;                          /* Accept all */
    dcan1[IF1ARB]  = (1u << 31u);                  /* MsgVal, Dir=0 (RX) */
    dcan1[IF1MCTL] = 8u | (1u << 7u) | (1u << 10u); /* DLC=8, EOB, UMask */
    dcan1[IF1CMD]  = 0x00F80000u | (uint32)obj;    /* WR, Mask, Arb, Ctrl, DataA, DataB */
    wait_if1();
}

/**
 * @brief  Configure message object as TX with given CAN ID
 */
static void can_setup_tx(uint8 obj, uint16 canId)
{
    wait_if1();
    dcan1[IF1MSK]  = 0xFFFFFFFFu;
    dcan1[IF1ARB]  = ((uint32)canId << 18u) | (1u << 31u) | (1u << 29u); /* MsgVal, Dir=TX */
    dcan1[IF1MCTL] = 4u | (1u << 7u);  /* DLC=4, EOB */
    dcan1[IF1DATA] = 0u;
    dcan1[IF1DATB] = 0u;
    dcan1[IF1CMD]  = 0x00F80000u | (uint32)obj;
    wait_if1();
}

/**
 * @brief  Transmit data on a TX message object
 */
static void can_tx(uint8 obj, uint32 data)
{
    wait_if1();
    dcan1[IF1DATA] = data;
    dcan1[IF1DATB] = 0u;
    dcan1[IF1MCTL] = 4u | (1u << 7u) | (1u << 8u); /* DLC=4, EOB, TxRqst */
    dcan1[IF1CMD]  = 0x00B00000u | (uint32)obj;     /* WR, Ctrl, DataA, DataB */
    wait_if1();
}

/**
 * @brief  Read RX message object, return TRUE if new data
 */
static boolean can_rx(uint8 obj, uint32 *data)
{
    uint32 mctl;

    wait_if2();
    dcan1[IF2CMD] = 0x007F0000u | (uint32)obj;  /* RD, all fields */
    wait_if2();

    mctl = dcan1[IF2MCTL];
    if ((mctl & (1u << 15u)) == 0u) {  /* NewDat bit */
        return FALSE;
    }
    *data = dcan1[IF2DATA];
    return TRUE;
}

/* Entry point — called from sc_startup.S as _c_int00 */
void _c_int00(void)
{
    uint32 rx_data;
    uint32 tx_counter = 0u;
    volatile uint32 timeout;
    boolean loopback_ok = FALSE;

    systemInit();
    gioInit();

    /* GIOB6, GIOB7 as outputs (LEDs) */
    gioPORTB->DIR |= (1u << 6u) | (1u << 7u);
    gioPORTB->DCLR = (1u << 6u) | (1u << 7u);  /* Both OFF */

    /* === Test 1: DCAN1 Loopback === */
    canInit();  /* HALCoGen init (parity, message RAM) */
    can_init(1u);  /* Loopback mode, 500 kbps */
    can_setup_rx(2u);
    can_setup_tx(1u, 0x013u);
    can_tx(1u, 0xDEADBEEFu);

    timeout = 500000u;
    while (timeout > 0u) {
        if (can_rx(2u, &rx_data) == TRUE) {
            if (rx_data == 0xDEADBEEFu) {
                loopback_ok = TRUE;
            }
            break;
        }
        timeout--;
    }

    if (loopback_ok) {
        gioPORTB->DSET = (1u << 6u);  /* LED2 ON = loopback passed */
    }

    /* === Test 2: External TX === */
    can_init(0u);  /* Normal mode */
    can_setup_tx(1u, 0x013u);

    for (;;) {
        tx_counter++;
        can_tx(1u, tx_counter);

        /* Toggle LED3 */
        if (tx_counter & 1u) {
            gioPORTB->DSET = (1u << 7u);
        } else {
            gioPORTB->DCLR = (1u << 7u);
        }

        delay(5000000u);  /* ~500ms at 300 MHz */
    }

}
