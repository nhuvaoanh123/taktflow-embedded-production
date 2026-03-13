/**
 * @file    Os_Core.c
 * @brief   Core control and bootstrap helpers for the OSEK bootstrap kernel
 * @date    2026-03-13
 */
#include "Os_Internal.h"

Os_TaskConfigType os_task_cfg[OS_MAX_TASKS];
Os_TaskControlBlockType os_tcb[OS_MAX_TASKS];
uint8 os_task_count = 0u;
Os_ResourceConfigType os_resource_cfg[OS_MAX_RESOURCES];
Os_ResourceControlBlockType os_resource_cb[OS_MAX_RESOURCES];
uint8 os_resource_count = 0u;
TaskType os_current_task = INVALID_TASK;
AppModeType os_active_app_mode = OSDEFAULTAPPMODE;
StatusType os_shutdown_error = E_OK;
boolean os_initialized = FALSE;
boolean os_started = FALSE;
boolean os_shutdown_requested = FALSE;
uint32 os_ready_bitmap = 0u;
uint32 os_ready_stamp_counter = 1u;
uint32 os_dispatch_count = 0u;
Os_HookType os_startup_hook = 0;
Os_ErrorHookType os_error_hook = 0;

#if defined(UNIT_TEST)
void os_clear_task_cfg(void)
{
    uint8 idx;

    for (idx = 0u; idx < OS_MAX_TASKS; idx++) {
        os_task_cfg[idx].Name = (const char*)0;
        os_task_cfg[idx].Entry = (Os_TaskEntryType)0;
        os_task_cfg[idx].Priority = 0u;
        os_task_cfg[idx].ActivationLimit = 0u;
        os_task_cfg[idx].AutostartMask = 0u;
    }
}

void os_clear_resource_cfg(void)
{
    uint8 idx;

    for (idx = 0u; idx < OS_MAX_RESOURCES; idx++) {
        os_resource_cfg[idx].Name = (const char*)0;
        os_resource_cfg[idx].CeilingPriority = 0u;
    }
}
#endif

void os_reset_runtime_state(void)
{
    uint8 idx;

    for (idx = 0u; idx < OS_MAX_TASKS; idx++) {
        os_tcb[idx].State = SUSPENDED;
        os_tcb[idx].PendingActivations = 0u;
        os_tcb[idx].ReadyStamp = 0u;
        os_tcb[idx].CurrentPriority = 0u;
        os_tcb[idx].ResourceCount = 0u;
        os_tcb[idx].SetEvents = 0u;
        os_tcb[idx].WaitEvents = 0u;
    }

    for (idx = 0u; idx < OS_MAX_TASKS; idx++) {
        uint8 resource_idx;

        for (resource_idx = 0u; resource_idx < OS_MAX_RESOURCES; resource_idx++) {
            os_tcb[idx].ResourceStack[resource_idx] = INVALID_RESOURCE;
        }
    }

    for (idx = 0u; idx < OS_MAX_RESOURCES; idx++) {
        os_resource_cb[idx].InUse = FALSE;
        os_resource_cb[idx].OwnerTask = INVALID_TASK;
    }

    os_current_task = INVALID_TASK;
    os_active_app_mode = OSDEFAULTAPPMODE;
    os_shutdown_error = E_OK;
    os_initialized = TRUE;
    os_started = FALSE;
    os_shutdown_requested = FALSE;
    os_ready_bitmap = 0u;
    os_ready_stamp_counter = 1u;
    os_dispatch_count = 0u;
    os_isr_cat2_nesting = 0u;
    os_preempted_task_depth = 0u;
}

boolean os_is_valid_task(TaskType TaskID)
{
    if (TaskID >= os_task_count) {
        return FALSE;
    }

    if (os_task_cfg[TaskID].Entry == (Os_TaskEntryType)0) {
        return FALSE;
    }

    return TRUE;
}

boolean os_is_valid_resource(ResourceType ResID)
{
    if (ResID >= os_resource_count) {
        return FALSE;
    }

    if (os_resource_cfg[ResID].Name == (const char*)0) {
        return FALSE;
    }

    return TRUE;
}

boolean os_is_extended_task(TaskType TaskID)
{
    if (os_is_valid_task(TaskID) == FALSE) {
        return FALSE;
    }

    return os_task_cfg[TaskID].Extended;
}

void os_recalculate_task_priority(TaskType TaskID)
{
    uint8 priority = os_task_cfg[TaskID].Priority;
    uint8 idx;

    for (idx = 0u; idx < os_tcb[TaskID].ResourceCount; idx++) {
        ResourceType resource_id = os_tcb[TaskID].ResourceStack[idx];

        if ((resource_id != INVALID_RESOURCE) &&
            (os_resource_cfg[resource_id].CeilingPriority < priority)) {
            priority = os_resource_cfg[resource_id].CeilingPriority;
        }
    }

    os_tcb[TaskID].CurrentPriority = priority;
}

void os_rebuild_ready_bitmap(void)
{
    uint8 idx;
    uint32 bitmap = 0u;

    for (idx = 0u; idx < os_task_count; idx++) {
        if (os_tcb[idx].State == READY) {
            bitmap |= ((uint32)1u << os_tcb[idx].CurrentPriority);
        }
    }

    os_ready_bitmap = bitmap;
}

void os_report_service_error(uint8 ApiId, uint8 DetErrorId, StatusType Status)
{
    Det_ReportError(OS_DET_MODULE_ID, 0u, ApiId, DetErrorId);

    if (os_error_hook != (Os_ErrorHookType)0) {
        os_error_hook(Status);
    }
}

void Os_Init(void)
{
    os_reset_runtime_state();
}

void StartOS(AppModeType Mode)
{
    uint8 idx;

    if (os_initialized == FALSE) {
        Os_Init();
    }

    os_started = TRUE;
    os_shutdown_requested = FALSE;
    os_active_app_mode = Mode;

    for (idx = 0u; idx < os_task_count; idx++) {
        uint32 mode_mask = ((uint32)1u << Mode);

        if ((os_task_cfg[idx].AutostartMask & mode_mask) != 0u) {
            (void)os_activate_task_internal(idx, FALSE);
        }
    }

    if (os_startup_hook != (Os_HookType)0) {
        os_startup_hook();
    }

    (void)os_run_ready_tasks();

#if !defined(UNIT_TEST)
    for (;;) {
        if (os_shutdown_requested == TRUE) {
            break;
        }

        if (os_dispatch_one() == E_OS_NOFUNC) {
            /* Idle until a future port layer wakes a task. */
        }
    }

    for (;;) {
    }
#endif
}

void ShutdownOS(StatusType Error)
{
    os_shutdown_requested = TRUE;
    os_shutdown_error = Error;
}

AppModeType GetActiveApplicationMode(void)
{
    return os_active_app_mode;
}

#if defined(UNIT_TEST)
void Os_TestReset(void)
{
    os_clear_task_cfg();
    os_clear_resource_cfg();
    os_task_count = 0u;
    os_resource_count = 0u;
    os_startup_hook = 0;
    os_error_hook = 0;
    os_reset_runtime_state();
}

StatusType Os_TestConfigureTasks(const Os_TaskConfigType* Config, uint8 TaskCount)
{
    uint8 idx;

    if ((Config == NULL_PTR) && (TaskCount > 0u)) {
        return E_OS_VALUE;
    }

    if (TaskCount > OS_MAX_TASKS) {
        return E_OS_LIMIT;
    }

    os_clear_task_cfg();

    for (idx = 0u; idx < TaskCount; idx++) {
        if ((Config[idx].Entry == (Os_TaskEntryType)0) ||
            (Config[idx].Priority >= OS_MAX_PRIORITIES) ||
            (Config[idx].ActivationLimit == 0u)) {
            return E_OS_VALUE;
        }

        os_task_cfg[idx].Name = Config[idx].Name;
        os_task_cfg[idx].Entry = Config[idx].Entry;
        os_task_cfg[idx].Priority = Config[idx].Priority;
        os_task_cfg[idx].ActivationLimit = Config[idx].ActivationLimit;
        os_task_cfg[idx].AutostartMask = Config[idx].AutostartMask;
        os_task_cfg[idx].Extended = Config[idx].Extended;
    }

    os_task_count = TaskCount;
    Os_Init();
    return E_OK;
}

StatusType Os_TestConfigureResources(const Os_ResourceConfigType* Config, uint8 ResourceCount)
{
    uint8 idx;

    if ((Config == NULL_PTR) && (ResourceCount > 0u)) {
        return E_OS_VALUE;
    }

    if (ResourceCount > OS_MAX_RESOURCES) {
        return E_OS_LIMIT;
    }

    os_clear_resource_cfg();

    for (idx = 0u; idx < ResourceCount; idx++) {
        if ((Config[idx].Name == (const char*)0) ||
            (Config[idx].CeilingPriority >= OS_MAX_PRIORITIES)) {
            return E_OS_VALUE;
        }

        os_resource_cfg[idx].Name = Config[idx].Name;
        os_resource_cfg[idx].CeilingPriority = Config[idx].CeilingPriority;
    }

    os_resource_count = ResourceCount;
    Os_Init();
    return E_OK;
}

void Os_TestSetStartupHook(Os_HookType Hook)
{
    os_startup_hook = Hook;
}

void Os_TestSetErrorHook(Os_ErrorHookType Hook)
{
    os_error_hook = Hook;
}

StatusType Os_TestRunReadyTasks(void)
{
    return os_run_ready_tasks();
}

TaskType Os_TestGetCurrentTask(void)
{
    return os_current_task;
}

uint32 Os_TestGetReadyBitmap(void)
{
    return os_ready_bitmap;
}

uint32 Os_TestGetDispatchCount(void)
{
    return os_dispatch_count;
}

StatusType Os_TestInvokeIsrCat2(Os_TestIsrHandlerType Handler)
{
    if (Handler == (Os_TestIsrHandlerType)0) {
        return E_OS_VALUE;
    }

    os_isr_cat2_nesting++;
    Handler();

    if (os_isr_cat2_nesting > 0u) {
        os_isr_cat2_nesting--;
    }

    if (os_isr_cat2_nesting == 0u) {
        if (os_current_task != INVALID_TASK) {
            (void)os_maybe_dispatch_preemption();
        } else {
            (void)os_run_ready_tasks();
        }
    }

    return E_OK;
}

uint8 Os_TestGetIsrCat2Nesting(void)
{
    return os_isr_cat2_nesting;
}
#endif
