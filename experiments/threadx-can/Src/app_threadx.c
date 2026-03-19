/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @brief   ThreadX + BSW integration experiment (Step 7)
  ******************************************************************************
  * @details Steps 1-6: ThreadX + HAL FDCAN TX/RX working.
  *          Step 7: BSW modules integrated incrementally:
  *            7a: Can.c + Can_Hw_STM32.c + SchM_ThreadX.c + Det.c
  *            7b: CanIf.c
  *            7c: Com.c + PduR.c + Com_Cfg_Fzc.c
  *            7d: Rte.c + Rte_Cfg_Fzc.c + Swc_Heartbeat.c
  *
  *          BSW modules use the production source files via relative paths.
  *          SchM uses ThreadX TX_DISABLE/TX_RESTORE instead of __disable_irq.
  *          Unused modules (Dcm, BswM, WdgM, etc.) are stubbed in bsw_stubs.c.
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include <stdio.h>

/* BSW module headers */
#include "Std_Types.h"
#include "Can.h"
#include "CanIf.h"
#include "PduR.h"
#include "Com.h"
#include "Rte.h"
#include "Det.h"
#include "Fzc_Cfg.h"
#include "Swc_Heartbeat.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_EVENT_FLAGS_GROUP EventFlag;
TX_TIMER can_periodic_timer;
TX_TIMER bsw_1ms_timer;
TX_THREAD can_rx_thread;
TX_EVENT_FLAGS_GROUP can_rx_event;
static uint32_t can_tx_count = 0;
static FDCAN_RxHeaderTypeDef can_rx_header;
static uint8_t can_rx_data[8];

/* BSW configuration objects */
static const Can_ConfigType exp_can_config = {
    .baudrate     = 500000u,
    .controllerId = 0u,
};

/* Minimal CanIf config: 1 TX PDU (heartbeat 0x011), no RX routing
 * (full FZC config is too large for this experiment, but we use
 *  the FZC Com/Rte configs which reference FZC PDU IDs) */
static const CanIf_TxPduConfigType exp_canif_tx[] = {
    { 0x011u, FZC_COM_TX_FZC_HEARTBEAT, 8u, 0u },   /* Heartbeat */
    { 0x200u, FZC_COM_TX_STEERING_STATUS, 8u, 0u },  /* Steering (stub) */
    { 0x201u, FZC_COM_TX_BRAKE_STATUS, 8u, 0u },     /* Brake (stub) */
    { 0x210u, FZC_COM_TX_BRAKE_FAULT, 8u, 0u },      /* Brake fault (stub) */
    { 0x211u, FZC_COM_TX_MOTOR_CUTOFF_REQ, 8u, 0u }, /* Motor cutoff (stub) */
    { 0x220u, FZC_COM_TX_LIDAR_DISTANCE, 8u, 0u },   /* Lidar (stub) */
    { 0x500u, FZC_COM_TX_DTC_BROADCAST, 8u, 0u },    /* DTC (stub) */
    { 0x7E9u, FZC_COM_TX_UDS_RESP_FZC, 8u, 0u },    /* UDS (stub) */
};

static const CanIf_ConfigType exp_canif_config = {
    .txPduConfig = exp_canif_tx,
    .txPduCount  = (uint8)(sizeof(exp_canif_tx) / sizeof(exp_canif_tx[0])),
    .rxPduConfig = NULL_PTR,
    .rxPduCount  = 0u,
    .e2eRxCheck  = NULL_PTR,
};

/* PduR: no RX routing needed for experiment */
static const PduR_ConfigType exp_pdur_config = {
    .routingTable = NULL_PTR,
    .routingCount = 0u,
};

/* Com and Rte configs are the full FZC configs from generated code */
extern const Com_ConfigType fzc_com_config;
extern const Rte_ConfigType fzc_rte_config;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);
static void CAN_Periodic_Callback(ULONG arg);
static void BSW_1ms_Callback(ULONG arg);
static void CAN_RX_Thread_Entry(ULONG arg);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for Main Thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create Main Thread.  */
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  /* Allocate the stack for ThreadOne.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create ThreadOne.  */
  if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadTwo.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create ThreadTwo.  */
  if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0, pointer,
                       TX_APP_STACK_SIZE, THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create the event flags group.  */
  if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
  {
    return TX_GROUP_ERROR;
  }

  /* ====================================================================
   * Step 7: BSW Module Initialization (order matters)
   * ==================================================================== */

  /* 7a: Det + Can MCAL + SchM */
  Det_Init();
  Can_Init(&exp_can_config);

  /* 7b: CanIf */
  CanIf_Init(&exp_canif_config);

  /* 7c: PduR + Com */
  PduR_Init(&exp_pdur_config);
  Com_Init(&fzc_com_config);

  /* 7d: Rte + Swc_Heartbeat */
  Rte_Init(&fzc_rte_config);
  Swc_Heartbeat_Init();

  /* Start CAN controller via BSW API */
  (void)Can_SetControllerMode(0u, CAN_CS_STARTED);

  /* Create periodic CAN TX timer — 1000 ticks = 1 second (at 1000Hz) */
  tx_timer_create(&can_periodic_timer, "CAN_TX", CAN_Periodic_Callback,
                  0, 1000, 1000, TX_AUTO_ACTIVATE);

  /* Step 7d: 1ms periodic timer for RTE scheduler */
  tx_timer_create(&bsw_1ms_timer, "BSW_1MS", BSW_1ms_Callback,
                  0, 1, 1, TX_AUTO_ACTIVATE);

  /* Create CAN RX event flag + thread (ISR-based, kept from Steps 1-6) */
  tx_event_flags_create(&can_rx_event, "CAN_RX_EVT");

  if (tx_byte_allocate(byte_pool, (VOID**)&pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  tx_thread_create(&can_rx_thread, "CAN_RX", CAN_RX_Thread_Entry, 0,
                   pointer, TX_APP_STACK_SIZE, 3, 3,
                   TX_NO_TIME_SLICE, TX_AUTO_START);

  /* Enable FDCAN RX FIFO0 new message notification (ISR path) */
  extern FDCAN_HandleTypeDef hfdcan1;
  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
  HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the MainThread_Entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  /* USER CODE BEGIN MainThread_Entry */
  UINT old_prio = 0;
  UINT old_pre_threshold = 0;
  ULONG   actual_flags = 0;
  uint8_t count = 0;
  (void) thread_input;

  while (count < 3)
  {
    count++;
    if (tx_event_flags_get(&EventFlag, THREAD_ONE_EVT, TX_OR_CLEAR,
                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
    {
      Error_Handler();
    }
    else
    {
      /* Update the priority and preemption threshold of ThreadTwo
      to allow the preemption of ThreadOne */
      tx_thread_priority_change(&ThreadTwo, NEW_THREAD_TWO_PRIO, &old_prio);
      tx_thread_preemption_change(&ThreadTwo, NEW_THREAD_TWO_PREEMPTION_THRESHOLD, &old_pre_threshold);

      if (tx_event_flags_get(&EventFlag, THREAD_TWO_EVT, TX_OR_CLEAR,
                             &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
      {
        Error_Handler();
      }
      else
      {
        /* Reset the priority and preemption threshold of ThreadTwo */
        tx_thread_priority_change(&ThreadTwo, THREAD_TWO_PRIO, &old_prio);
        tx_thread_preemption_change(&ThreadTwo, THREAD_TWO_PREEMPTION_THRESHOLD, &old_pre_threshold);
      }
    }
  }

  /* Destroy ThreadOne and ThreadTwo */
  tx_thread_terminate(&ThreadOne);
  tx_thread_terminate(&ThreadTwo);

  /* Infinite loop — periodic status print */
  while(1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    printf("ThreadX tick=%lu BSW CAN_RX=%lu last_ID=0x%03lX\r\n",
           (unsigned long)tx_time_get(),
           (unsigned long)g_can_rx_count,
           (unsigned long)g_can_rx_last_id);

    /* Thread sleep for 5s (5000 ticks at 1000Hz) */
    tx_thread_sleep(5000);
  }
  /* USER CODE END MainThread_Entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing the ThreadOne thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
  (void) thread_input;
  uint8_t count = 0;
  /* Infinite loop */
  while(1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    /* Delay for 500ms (App_Delay is used to avoid context change). */
    App_Delay(500);
    count ++;
    if (count == 10)
    {
      count = 0;
      if (tx_event_flags_set(&EventFlag, THREAD_ONE_EVT, TX_OR) != TX_SUCCESS)
      {
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  Function implementing the ThreadTwo thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadTwo_Entry(ULONG thread_input)
{
  (void) thread_input;
  uint8_t count = 0;
  /* Infinite loop */
  while (1)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    /* Delay for 200ms (App_Delay is used to avoid context change). */
    App_Delay(200);
    count ++;
    if (count == 25)
    {
      count = 0;
      if (tx_event_flags_set(&EventFlag, THREAD_TWO_EVT, TX_OR) != TX_SUCCESS)
      {
        Error_Handler();
      }
    }
  }
}

/**
  * @brief  CAN RX thread — waits for ISR event, prints received frame
  */
static void CAN_RX_Thread_Entry(ULONG arg)
{
  (void)arg;
  ULONG actual;
  while (1)
  {
    if (tx_event_flags_get(&can_rx_event, 0x1, TX_AND_CLEAR,
                           &actual, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      printf("CAN RX: ID=0x%03lX DLC=%lu Data=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
             (unsigned long)can_rx_header.Identifier,
             (unsigned long)(can_rx_header.DataLength >> 16),
             can_rx_data[0], can_rx_data[1], can_rx_data[2], can_rx_data[3],
             can_rx_data[4], can_rx_data[5], can_rx_data[6], can_rx_data[7]);
    }
  }
}

/**
  * @brief  HAL FDCAN RX FIFO0 callback — called from ISR
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE)
  {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &can_rx_header, can_rx_data);
    tx_event_flags_set(&can_rx_event, 0x1, TX_OR);
  }
}

/**
  * @brief  FDCAN1 IT0 IRQ handler
  */
void FDCAN1_IT0_IRQHandler(void)
{
  extern FDCAN_HandleTypeDef hfdcan1;
  HAL_FDCAN_IRQHandler(&hfdcan1);
}

/**
  * @brief  Periodic CAN TX from ThreadX timer — 1s heartbeat via BSW stack
  * @note   Step 7a+: Uses Can_MainFunction_Read + Com_MainFunction_Tx
  *         instead of direct HAL calls. Also sends a raw BSW heartbeat
  *         as a diagnostic marker.
  */
static void CAN_Periodic_Callback(ULONG arg)
{
  (void)arg;

  can_tx_count++;

  /* Step 7a: Poll CAN RX via BSW (processes pending frames → CanIf → PduR → Com) */
  Can_MainFunction_Read();

  /* Step 7c: Flush pending Com TX PDUs (Swc_Heartbeat sets tx_pending) */
  Com_MainFunction_Tx();
  Com_MainFunction_Rx();
}

/**
  * @brief  1ms BSW timer callback — drives RTE scheduler
  * @note   Step 7d: Calls Rte_MainFunction which dispatches all configured
  *         runnables (including Swc_Heartbeat_MainFunction every 50ms).
  */
static void BSW_1ms_Callback(ULONG arg)
{
  (void)arg;
  Rte_MainFunction();
}

/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/* USER CODE END 1 */
