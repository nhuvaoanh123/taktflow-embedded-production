/**
 * @file    test_Os_asild.c
 * @brief   Unit tests for OSEK-first OS bootstrap kernel
 * @date    2026-03-13
 *
 * @details This file is the first TDD slice under the repo's OSEK roadmap.
 *          The tests exercise bootstrap BCC1 behavior on host, with spec
 *          citations carried in each test block so the suite can evolve
 *          toward a clause-to-test traceability matrix.
 */
#include "unity.h"

#include "Det.h"
#include "Os.h"

#define TASK_LOW            0u
#define TASK_HIGH           1u
#define TASK_CHAIN          2u
#define TASK_PEER           3u
#define TASK_PROBE          4u
#define TASK_SCHEDULE       5u
#define RES_SHARED          0u
#define RES_OUTER           0u
#define RES_INNER           1u
#define EVENT_ALPHA         ((EventMaskType)0x01u)

#define APPMODE_DIAG        1u

static char execution_log[32];
static uint8 low_runs;
static uint8 high_runs;
static uint8 chain_runs;
static uint8 peer_runs;
static uint8 probe_runs;
static uint8 schedule_runs;
static StatusType low_status;
static StatusType high_status;
static StatusType chain_status;
static StatusType peer_status;
static StatusType probe_status;
static StatusType schedule_status;
static StatusType error_hook_status;
static uint8 error_hook_count;
static TaskType observed_task_id;
static TaskStateType observed_task_state;
static uint8 low_preempt_runs;
static uint8 high_preempt_runs;
static uint8 isr_low_runs;
static uint8 isr_high_runs;
static uint8 isr_handler_runs;
static StatusType low_preempt_activate_status;
static StatusType low_preempt_term_status;
static StatusType high_preempt_term_status;
static StatusType isr_invoke_status;
static StatusType isr_activate_status;
static StatusType isr_low_term_status;
static StatusType isr_high_term_status;
static uint8 observed_isr_nesting;
static uint8 resource_low_runs;
static uint8 resource_high_runs;
static uint8 resource_order_runs;
static StatusType resource_get_status;
static StatusType resource_activate_status;
static StatusType resource_release_status;
static StatusType resource_low_term_status;
static StatusType resource_high_term_status;
static StatusType resource_outer_get_status;
static StatusType resource_inner_get_status;
static StatusType resource_wrong_release_status;
static StatusType resource_inner_release_status;
static StatusType resource_outer_release_status;
static StatusType resource_order_term_status;
static uint8 resource_terminate_runs;
static uint8 resource_chain_runs;
static StatusType resource_terminate_get_status;
static StatusType resource_terminate_status;
static StatusType resource_terminate_release_status;
static StatusType resource_terminate_cleanup_status;
static StatusType resource_chain_get_status;
static StatusType resource_chain_status;
static StatusType resource_chain_release_status;
static StatusType resource_chain_cleanup_status;
static uint8 event_wait_runs;
static uint8 event_setter_runs;
static uint8 event_basic_runs;
static uint8 event_wait_phase;
static StatusType event_wait_status;
static StatusType event_set_status;
static StatusType event_get_status;
static StatusType event_clear_status;
static StatusType event_get_after_clear_status;
static StatusType event_wait_term_status;
static StatusType event_setter_term_status;
static StatusType event_basic_wait_status;
static StatusType event_basic_term_status;
static EventMaskType event_mask_before_clear;
static EventMaskType event_mask_after_clear;
static TaskStateType event_wait_state_after_block;

static void reset_observations(void)
{
    execution_log[0] = '\0';
    low_runs = 0u;
    high_runs = 0u;
    chain_runs = 0u;
    peer_runs = 0u;
    probe_runs = 0u;
    schedule_runs = 0u;
    low_status = E_OK;
    high_status = E_OK;
    chain_status = E_OK;
    peer_status = E_OK;
    probe_status = E_OK;
    schedule_status = E_OK;
    error_hook_status = E_OK;
    error_hook_count = 0u;
    observed_task_id = INVALID_TASK;
    observed_task_state = SUSPENDED;
    low_preempt_runs = 0u;
    high_preempt_runs = 0u;
    isr_low_runs = 0u;
    isr_high_runs = 0u;
    isr_handler_runs = 0u;
    low_preempt_activate_status = E_OK;
    low_preempt_term_status = E_OK;
    high_preempt_term_status = E_OK;
    isr_invoke_status = E_OK;
    isr_activate_status = E_OK;
    isr_low_term_status = E_OK;
    isr_high_term_status = E_OK;
    observed_isr_nesting = 0u;
    resource_low_runs = 0u;
    resource_high_runs = 0u;
    resource_order_runs = 0u;
    resource_get_status = E_OK;
    resource_activate_status = E_OK;
    resource_release_status = E_OK;
    resource_low_term_status = E_OK;
    resource_high_term_status = E_OK;
    resource_outer_get_status = E_OK;
    resource_inner_get_status = E_OK;
    resource_wrong_release_status = E_OK;
    resource_inner_release_status = E_OK;
    resource_outer_release_status = E_OK;
    resource_order_term_status = E_OK;
    resource_terminate_runs = 0u;
    resource_chain_runs = 0u;
    resource_terminate_get_status = E_OK;
    resource_terminate_status = E_OK;
    resource_terminate_release_status = E_OK;
    resource_terminate_cleanup_status = E_OK;
    resource_chain_get_status = E_OK;
    resource_chain_status = E_OK;
    resource_chain_release_status = E_OK;
    resource_chain_cleanup_status = E_OK;
    event_wait_runs = 0u;
    event_setter_runs = 0u;
    event_basic_runs = 0u;
    event_wait_phase = 0u;
    event_wait_status = E_OK;
    event_set_status = E_OK;
    event_get_status = E_OK;
    event_clear_status = E_OK;
    event_get_after_clear_status = E_OK;
    event_wait_term_status = E_OK;
    event_setter_term_status = E_OK;
    event_basic_wait_status = E_OK;
    event_basic_term_status = E_OK;
    event_mask_before_clear = 0u;
    event_mask_after_clear = 0u;
    event_wait_state_after_block = SUSPENDED;
}

static void append_log(char Marker)
{
    uint8 idx = 0u;

    while ((execution_log[idx] != '\0') && (idx < (sizeof(execution_log) - 1u))) {
        idx++;
    }

    if (idx < (sizeof(execution_log) - 1u)) {
        execution_log[idx] = Marker;
        execution_log[idx + 1u] = '\0';
    }
}

static void error_hook(StatusType Error)
{
    error_hook_status = Error;
    error_hook_count++;
}

static void startup_hook(void)
{
    append_log('S');
}

static void task_low(void)
{
    TaskType current_task = INVALID_TASK;

    low_runs++;
    append_log('L');
    (void)GetTaskID(&current_task);
    observed_task_id = current_task;
    low_status = TerminateTask();
}

static void task_high(void)
{
    high_runs++;
    append_log('H');
    high_status = TerminateTask();
}

static void task_chain(void)
{
    chain_runs++;
    append_log('C');
    chain_status = ChainTask(TASK_HIGH);
}

static void task_peer(void)
{
    peer_runs++;
    append_log('Q');
    peer_status = TerminateTask();
}

static void task_probe(void)
{
    TaskType current_task = INVALID_TASK;

    probe_runs++;
    append_log('P');
    (void)GetTaskID(&current_task);
    (void)GetTaskState(current_task, &observed_task_state);
    probe_status = TerminateTask();
}

static void task_schedule(void)
{
    schedule_runs++;
    append_log('Y');
    schedule_status = Schedule();
    (void)TerminateTask();
}

static void task_low_preempt(void)
{
    low_preempt_runs++;
    append_log('A');
    low_preempt_activate_status = ActivateTask(TASK_HIGH);
    append_log('a');
    low_preempt_term_status = TerminateTask();
}

static void task_high_preempt(void)
{
    high_preempt_runs++;
    append_log('B');
    high_preempt_term_status = TerminateTask();
}

static void isr_activate_high(void)
{
    isr_handler_runs++;
    append_log('I');
    observed_isr_nesting = Os_TestGetIsrCat2Nesting();
    isr_activate_status = ActivateTask(TASK_HIGH);
}

static void task_low_isr(void)
{
    isr_low_runs++;
    append_log('L');
    isr_invoke_status = Os_TestInvokeIsrCat2(isr_activate_high);
    append_log('l');
    isr_low_term_status = TerminateTask();
}

static void task_high_isr(void)
{
    isr_high_runs++;
    append_log('H');
    isr_high_term_status = TerminateTask();
}

static void task_low_resource(void)
{
    resource_low_runs++;
    append_log('R');
    resource_get_status = GetResource(RES_SHARED);
    resource_activate_status = ActivateTask(TASK_HIGH);
    append_log('r');
    resource_release_status = ReleaseResource(RES_SHARED);
    append_log('x');
    resource_low_term_status = TerminateTask();
}

static void task_high_resource(void)
{
    resource_high_runs++;
    append_log('H');
    resource_high_term_status = TerminateTask();
}

static void task_resource_release_order(void)
{
    resource_order_runs++;
    append_log('N');
    resource_outer_get_status = GetResource(RES_OUTER);
    resource_inner_get_status = GetResource(RES_INNER);
    resource_wrong_release_status = ReleaseResource(RES_OUTER);
    append_log('n');
    resource_inner_release_status = ReleaseResource(RES_INNER);
    resource_outer_release_status = ReleaseResource(RES_OUTER);
    resource_order_term_status = TerminateTask();
}

static void task_terminate_with_resource(void)
{
    resource_terminate_runs++;
    append_log('T');
    resource_terminate_get_status = GetResource(RES_SHARED);
    resource_terminate_status = TerminateTask();
    append_log('t');
    resource_terminate_release_status = ReleaseResource(RES_SHARED);
    resource_terminate_cleanup_status = TerminateTask();
}

static void task_chain_with_resource(void)
{
    resource_chain_runs++;
    append_log('C');
    resource_chain_get_status = GetResource(RES_SHARED);
    resource_chain_status = ChainTask(TASK_HIGH);
    append_log('c');
    resource_chain_release_status = ReleaseResource(RES_SHARED);
    resource_chain_cleanup_status = TerminateTask();
}

static void task_wait_event_extended(void)
{
    event_wait_runs++;

    if (event_wait_phase == 0u) {
        append_log('W');
        event_wait_status = WaitEvent(EVENT_ALPHA);
        (void)GetTaskState(TASK_LOW, &event_wait_state_after_block);

        if (event_wait_state_after_block == WAITING) {
            event_wait_phase = 1u;
            return;
        }
    }

    append_log('E');
    event_get_status = GetEvent(TASK_LOW, &event_mask_before_clear);
    event_clear_status = ClearEvent(EVENT_ALPHA);
    event_get_after_clear_status = GetEvent(TASK_LOW, &event_mask_after_clear);
    event_wait_phase = 2u;
    event_wait_term_status = TerminateTask();
}

static void task_set_event_notifier(void)
{
    event_setter_runs++;
    append_log('N');
    event_set_status = SetEvent(TASK_LOW, EVENT_ALPHA);
    append_log('n');
    event_setter_term_status = TerminateTask();
}

static void task_wait_event_basic(void)
{
    event_basic_runs++;
    append_log('B');
    event_basic_wait_status = WaitEvent(EVENT_ALPHA);
    event_basic_term_status = TerminateTask();
}

void setUp(void)
{
    Det_Init();
    Os_TestReset();
    reset_observations();
}

void tearDown(void)
{
}

/**
 * @spec OSEK OS 2.2.3 §13.7
 * @requirement OS services shall not be used before the OS is started.
 * @verify ActivateTask returns E_OS_STATE and the error hook observes it.
 */
void test_Os_activate_task_before_start_reports_state_error(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 3u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);

    TEST_ASSERT_EQUAL(E_OS_STATE, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());
    TEST_ASSERT_EQUAL(E_OS_STATE, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.1, §13.7
 * @requirement Autostarted tasks shall become ready at StartOS and the
 *              highest-priority ready task shall run first.
 * @verify Startup hook runs once, then autostart tasks execute in priority
 *         order and return to SUSPENDED after completion.
 */
void test_Os_startos_runs_autostart_tasks_by_priority(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 3u, 1u, (1u << OSDEFAULTAPPMODE), FALSE },
        { "High", task_high, 1u, 1u, (1u << OSDEFAULTAPPMODE), FALSE }
    };
    TaskStateType low_state = RUNNING;
    TaskStateType high_state = RUNNING;

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    Os_TestSetStartupHook(startup_hook);
    Os_TestSetErrorHook(error_hook);

    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL_STRING("SHL", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, high_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, low_runs);
    TEST_ASSERT_EQUAL(E_OK, high_status);
    TEST_ASSERT_EQUAL(E_OK, low_status);
    TEST_ASSERT_EQUAL(OSDEFAULTAPPMODE, GetActiveApplicationMode());
    TEST_ASSERT_EQUAL_UINT32(0u, Os_TestGetReadyBitmap());
    TEST_ASSERT_EQUAL_UINT32(2u, Os_TestGetDispatchCount());
    TEST_ASSERT_EQUAL(E_OK, GetTaskState(TASK_LOW, &low_state));
    TEST_ASSERT_EQUAL(E_OK, GetTaskState(TASK_HIGH, &high_state));
    TEST_ASSERT_EQUAL(SUSPENDED, low_state);
    TEST_ASSERT_EQUAL(SUSPENDED, high_state);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement ActivateTask shall put a suspended task into the ready set.
 * @verify Schedule dispatches the activated task from idle context.
 */
void test_Os_schedule_dispatches_ready_tasks_from_idle_context(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 3u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL_UINT8(1u, Os_TestGetPendingActivations(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("L", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, low_runs);
    TEST_ASSERT_EQUAL(E_OK, low_status);
    TEST_ASSERT_EQUAL(TASK_LOW, observed_task_id);
}

/**
 * @spec OSEK OS 2.2.3 §13.1
 * @requirement Ready tasks at the same priority shall follow a deterministic
 *              ready-queue order.
 * @verify Equal-priority tasks execute FIFO in the bootstrap scheduler.
 */
void test_Os_same_priority_tasks_run_fifo_order(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 2u, 1u, 0u, FALSE },
        { "Peer", task_peer, 2u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_HIGH));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("LQ", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, low_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, peer_runs);
}

/**
 * @spec OSEK OS 2.2.3 §13.2.3.2
 * @requirement If ActivateTask is called for a BCC1 task that is not in
 *              SUSPENDED state, the service shall return E_OS_LIMIT.
 * @verify Pending activation count does not exceed one and the error hook
 *         receives E_OS_LIMIT.
 */
void test_Os_activation_limit_is_enforced(void)
{
    const Os_TaskConfigType cfg[] = {
        { "High", task_high, 1u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OS_LIMIT, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL_UINT8(1u, Os_TestGetPendingActivations(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OS_LIMIT, error_hook_status);
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("H", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, high_runs);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement ChainTask shall terminate the current task and activate the
 *              successor task atomically.
 * @verify The chained task runs first, then its successor runs next.
 */
void test_Os_chain_task_queues_successor_after_current_completion(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Chain", task_chain, 2u, 1u, 0u, FALSE },
        { "High", task_high, 1u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("CH", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, chain_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, high_runs);
    TEST_ASSERT_EQUAL(E_OK, chain_status);
    TEST_ASSERT_EQUAL(E_OK, high_status);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement GetTaskState shall reflect the current state of a valid task.
 * @verify A running task observes itself in RUNNING state.
 */
void test_Os_get_task_state_reports_running_while_task_executes(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Probe", task_probe, 2u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("P", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, probe_runs);
    TEST_ASSERT_EQUAL(RUNNING, observed_task_state);
    TEST_ASSERT_EQUAL(E_OK, probe_status);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement Schedule is meaningful only at the allowed call level.
 * @verify Calling Schedule from inside a running task is rejected in the
 *         current bootstrap with E_OS_CALLEVEL.
 */
void test_Os_schedule_from_running_task_is_rejected_in_current_milestone(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Schedule", task_schedule, 2u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("Y", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, schedule_runs);
    TEST_ASSERT_EQUAL(E_OS_CALLEVEL, schedule_status);
    TEST_ASSERT_EQUAL(E_OS_CALLEVEL, error_hook_status);
}

/**
 * @spec OSEK OS 2.2.3 §13.7
 * @requirement StartOS shall activate only the tasks configured for the
 *              selected application mode.
 * @verify A task autostarts only in the matching application mode.
 */
void test_Os_app_mode_mask_controls_autostart(void)
{
    const Os_TaskConfigType cfg[] = {
        { "High", task_high, 1u, 1u, (1u << APPMODE_DIAG), FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));

    StartOS(OSDEFAULTAPPMODE);
    TEST_ASSERT_EQUAL_STRING("", execution_log);

    Os_Init();
    reset_observations();
    StartOS(APPMODE_DIAG);
    TEST_ASSERT_EQUAL_STRING("H", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, high_runs);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement Services taking a TaskID shall reject invalid task IDs.
 * @verify ActivateTask returns E_OS_ID for an out-of-range task.
 */
void test_Os_activate_task_with_invalid_id_returns_e_os_id(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 3u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OS_ID, ActivateTask((TaskType)7u));
    TEST_ASSERT_EQUAL(E_OS_ID, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.2
 * @requirement Services with output pointers shall reject NULL pointers.
 * @verify GetTaskID returns E_OS_VALUE when called with NULL.
 */
void test_Os_get_task_id_with_null_pointer_returns_e_os_value(void)
{
    const Os_TaskConfigType cfg[] = {
        { "Low", task_low, 3u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);

    TEST_ASSERT_EQUAL(E_OS_VALUE, GetTaskID(NULL_PTR));
    TEST_ASSERT_EQUAL(E_OS_VALUE, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.1, §13.2
 * @requirement If a higher-priority task becomes ready while a lower-
 *              priority task is running, the higher-priority task shall run
 *              before the lower-priority task continues.
 * @verify ActivateTask from a running low-priority task causes immediate
 *         nested dispatch of the higher-priority task in the bootstrap.
 */
void test_Os_higher_priority_activation_preempts_running_task_immediately(void)
{
    const Os_TaskConfigType cfg[] = {
        { "LowPreempt", task_low_preempt, 3u, 1u, 0u, FALSE },
        { "HighPreempt", task_high_preempt, 1u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("ABa", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, low_preempt_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, high_preempt_runs);
    TEST_ASSERT_EQUAL(E_OK, low_preempt_activate_status);
    TEST_ASSERT_EQUAL(E_OK, low_preempt_term_status);
    TEST_ASSERT_EQUAL(E_OK, high_preempt_term_status);
}

/**
 * @spec OSEK OS 2.2.3 §13.1, §13.3
 * @requirement A higher-priority task readied from Category 2 ISR context
 *              shall be dispatched when the ISR completes.
 * @verify ISR activation does not preempt while nesting is non-zero, then
 *         dispatches the higher-priority task on ISR exit before low task
 *         execution resumes.
 */
void test_Os_cat2_isr_exit_dispatches_higher_priority_task_before_return(void)
{
    const Os_TaskConfigType cfg[] = {
        { "LowIsr", task_low_isr, 3u, 1u, 0u, FALSE },
        { "HighIsr", task_high_isr, 1u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("LIHl", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, isr_low_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, isr_high_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, isr_handler_runs);
    TEST_ASSERT_EQUAL(E_OK, isr_invoke_status);
    TEST_ASSERT_EQUAL(E_OK, isr_activate_status);
    TEST_ASSERT_EQUAL(E_OK, isr_low_term_status);
    TEST_ASSERT_EQUAL(E_OK, isr_high_term_status);
    TEST_ASSERT_EQUAL_UINT8(1u, observed_isr_nesting);
    TEST_ASSERT_EQUAL_UINT8(0u, Os_TestGetIsrCat2Nesting());
}

/**
 * @spec OSEK OS 2.2.3 §13.4
 * @requirement A task holding a resource shall execute at the resource
 *              ceiling priority so lower-ceiling preemption is deferred
 *              until ReleaseResource.
 * @verify A higher-priority ready task does not run while the resource is
 *         held, then preempts immediately when the resource is released.
 */
void test_Os_resource_ceiling_defers_preemption_until_release(void)
{
    const Os_TaskConfigType cfg[] = {
        { "ResLow", task_low_resource, 3u, 1u, 0u, FALSE },
        { "ResHigh", task_high_resource, 1u, 1u, 0u, FALSE }
    };
    const Os_ResourceConfigType res_cfg[] = {
        { "ResShared", 1u }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureResources(res_cfg, 1u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("RrHx", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, resource_low_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, resource_high_runs);
    TEST_ASSERT_EQUAL(E_OK, resource_get_status);
    TEST_ASSERT_EQUAL(E_OK, resource_activate_status);
    TEST_ASSERT_EQUAL(E_OK, resource_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_low_term_status);
    TEST_ASSERT_EQUAL(E_OK, resource_high_term_status);
}

/**
 * @spec OSEK OS 2.2.3 §13.4
 * @requirement Nested resources shall be released in reverse acquisition
 *              order.
 * @verify Releasing the outer resource first returns E_OS_NOFUNC, then the
 *         inner and outer resources can be released in proper LIFO order.
 */
void test_Os_nested_resources_must_release_in_lifo_order(void)
{
    const Os_TaskConfigType cfg[] = {
        { "ResOrder", task_resource_release_order, 2u, 1u, 0u, FALSE }
    };
    const Os_ResourceConfigType res_cfg[] = {
        { "ResOuter", 2u },
        { "ResInner", 1u }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureResources(res_cfg, 2u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("Nn", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, resource_order_runs);
    TEST_ASSERT_EQUAL(E_OK, resource_outer_get_status);
    TEST_ASSERT_EQUAL(E_OK, resource_inner_get_status);
    TEST_ASSERT_EQUAL(E_OS_NOFUNC, resource_wrong_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_inner_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_outer_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_order_term_status);
    TEST_ASSERT_EQUAL(E_OS_NOFUNC, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.2, §13.4
 * @requirement A task shall not terminate while it still occupies a
 *              resource.
 * @verify TerminateTask returns E_OS_RESOURCE until the held resource is
 *         released, then succeeds on cleanup termination.
 */
void test_Os_terminate_task_rejects_held_resource_with_e_os_resource(void)
{
    const Os_TaskConfigType cfg[] = {
        { "ResTerminate", task_terminate_with_resource, 2u, 1u, 0u, FALSE }
    };
    const Os_ResourceConfigType res_cfg[] = {
        { "ResShared", 1u }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureResources(res_cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("Tt", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, resource_terminate_runs);
    TEST_ASSERT_EQUAL(E_OK, resource_terminate_get_status);
    TEST_ASSERT_EQUAL(E_OS_RESOURCE, resource_terminate_status);
    TEST_ASSERT_EQUAL(E_OK, resource_terminate_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_terminate_cleanup_status);
    TEST_ASSERT_EQUAL(E_OS_RESOURCE, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.2, §13.4
 * @requirement ChainTask shall not terminate a task that still occupies a
 *              resource.
 * @verify ChainTask returns E_OS_RESOURCE while the resource is held and
 *         does not activate the successor task.
 */
void test_Os_chain_task_rejects_held_resource_with_e_os_resource(void)
{
    const Os_TaskConfigType cfg[] = {
        { "ResChain", task_chain_with_resource, 2u, 1u, 0u, FALSE },
        { "High", task_high, 1u, 1u, 0u, FALSE }
    };
    const Os_ResourceConfigType res_cfg[] = {
        { "ResShared", 1u }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureResources(res_cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("Cc", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, resource_chain_runs);
    TEST_ASSERT_EQUAL_UINT8(0u, high_runs);
    TEST_ASSERT_EQUAL(E_OK, resource_chain_get_status);
    TEST_ASSERT_EQUAL(E_OS_RESOURCE, resource_chain_status);
    TEST_ASSERT_EQUAL(E_OK, resource_chain_release_status);
    TEST_ASSERT_EQUAL(E_OK, resource_chain_cleanup_status);
    TEST_ASSERT_EQUAL(E_OS_RESOURCE, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

/**
 * @spec OSEK OS 2.2.3 §13.5
 * @requirement An extended task calling WaitEvent shall enter WAITING until
 *              a matching SetEvent readies it again.
 * @verify The waiter blocks in WAITING, resumes after SetEvent, observes the
 *         event mask, clears it, and then terminates cleanly.
 */
void test_Os_wait_event_blocks_extended_task_until_set_event(void)
{
    const Os_TaskConfigType cfg[] = {
        { "WaitExt", task_wait_event_extended, 2u, 1u, 0u, TRUE },
        { "Notifier", task_set_event_notifier, 3u, 1u, 0u, FALSE }
    };
    TaskStateType wait_state = RUNNING;

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 2u));
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("W", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, event_wait_runs);
    TEST_ASSERT_EQUAL(E_OK, event_wait_status);
    TEST_ASSERT_EQUAL(WAITING, event_wait_state_after_block);
    TEST_ASSERT_EQUAL(E_OK, GetTaskState(TASK_LOW, &wait_state));
    TEST_ASSERT_EQUAL(WAITING, wait_state);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_HIGH));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("WNEn", execution_log);
    TEST_ASSERT_EQUAL_UINT8(2u, event_wait_runs);
    TEST_ASSERT_EQUAL_UINT8(1u, event_setter_runs);
    TEST_ASSERT_EQUAL(E_OK, event_set_status);
    TEST_ASSERT_EQUAL(E_OK, event_get_status);
    TEST_ASSERT_EQUAL(EVENT_ALPHA, event_mask_before_clear);
    TEST_ASSERT_EQUAL(E_OK, event_clear_status);
    TEST_ASSERT_EQUAL(E_OK, event_get_after_clear_status);
    TEST_ASSERT_EQUAL(0u, event_mask_after_clear);
    TEST_ASSERT_EQUAL(E_OK, event_wait_term_status);
    TEST_ASSERT_EQUAL(E_OK, event_setter_term_status);
    TEST_ASSERT_EQUAL_UINT8(2u, event_wait_phase);
    TEST_ASSERT_EQUAL(E_OK, GetTaskState(TASK_LOW, &wait_state));
    TEST_ASSERT_EQUAL(SUSPENDED, wait_state);
}

/**
 * @spec OSEK OS 2.2.3 §13.5
 * @requirement Event services shall be available only to extended tasks.
 * @verify WaitEvent from a basic task returns E_OS_ACCESS and the task can
 *         still terminate normally afterward.
 */
void test_Os_wait_event_rejects_basic_task_with_e_os_access(void)
{
    const Os_TaskConfigType cfg[] = {
        { "BasicWait", task_wait_event_basic, 2u, 1u, 0u, FALSE }
    };

    TEST_ASSERT_EQUAL(E_OK, Os_TestConfigureTasks(cfg, 1u));
    Os_TestSetErrorHook(error_hook);
    StartOS(OSDEFAULTAPPMODE);

    TEST_ASSERT_EQUAL(E_OK, ActivateTask(TASK_LOW));
    TEST_ASSERT_EQUAL(E_OK, Schedule());
    TEST_ASSERT_EQUAL_STRING("B", execution_log);
    TEST_ASSERT_EQUAL_UINT8(1u, event_basic_runs);
    TEST_ASSERT_EQUAL(E_OS_ACCESS, event_basic_wait_status);
    TEST_ASSERT_EQUAL(E_OK, event_basic_term_status);
    TEST_ASSERT_EQUAL(E_OS_ACCESS, error_hook_status);
    TEST_ASSERT_EQUAL_UINT8(1u, error_hook_count);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_Os_activate_task_before_start_reports_state_error);
    RUN_TEST(test_Os_startos_runs_autostart_tasks_by_priority);
    RUN_TEST(test_Os_schedule_dispatches_ready_tasks_from_idle_context);
    RUN_TEST(test_Os_same_priority_tasks_run_fifo_order);
    RUN_TEST(test_Os_activation_limit_is_enforced);
    RUN_TEST(test_Os_chain_task_queues_successor_after_current_completion);
    RUN_TEST(test_Os_get_task_state_reports_running_while_task_executes);
    RUN_TEST(test_Os_schedule_from_running_task_is_rejected_in_current_milestone);
    RUN_TEST(test_Os_app_mode_mask_controls_autostart);
    RUN_TEST(test_Os_activate_task_with_invalid_id_returns_e_os_id);
    RUN_TEST(test_Os_get_task_id_with_null_pointer_returns_e_os_value);
    RUN_TEST(test_Os_higher_priority_activation_preempts_running_task_immediately);
    RUN_TEST(test_Os_cat2_isr_exit_dispatches_higher_priority_task_before_return);
    RUN_TEST(test_Os_resource_ceiling_defers_preemption_until_release);
    RUN_TEST(test_Os_nested_resources_must_release_in_lifo_order);
    RUN_TEST(test_Os_terminate_task_rejects_held_resource_with_e_os_resource);
    RUN_TEST(test_Os_chain_task_rejects_held_resource_with_e_os_resource);
    RUN_TEST(test_Os_wait_event_blocks_extended_task_until_set_event);
    RUN_TEST(test_Os_wait_event_rejects_basic_task_with_e_os_access);

    return UNITY_END();
}
