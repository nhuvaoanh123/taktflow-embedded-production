/**
 * @file    Os_Internal.h
 * @brief   Internal state for the OSEK bootstrap kernel
 * @date    2026-03-13
 */
#ifndef OS_INTERNAL_H
#define OS_INTERNAL_H

#include "Os.h"

#include "Det.h"

#define OS_DET_MODULE_ID               0x50u
#define OS_DET_API_ACTIVATE_TASK       0x03u
#define OS_DET_API_TERMINATE_TASK      0x04u
#define OS_DET_API_CHAIN_TASK          0x05u
#define OS_DET_API_SCHEDULE            0x06u
#define OS_DET_API_GET_TASK_ID         0x07u
#define OS_DET_API_GET_TASK_STATE      0x08u
#define OS_DET_API_GET_RESOURCE        0x09u
#define OS_DET_API_RELEASE_RESOURCE    0x0Au
#define OS_DET_API_SET_EVENT           0x0Bu
#define OS_DET_API_CLEAR_EVENT         0x0Cu
#define OS_DET_API_GET_EVENT           0x0Du
#define OS_DET_API_WAIT_EVENT          0x0Eu

typedef struct {
    TaskStateType State;
    uint8 PendingActivations;
    uint32 ReadyStamp;
    uint8 CurrentPriority;
    ResourceType ResourceStack[OS_MAX_RESOURCES];
    uint8 ResourceCount;
    EventMaskType SetEvents;
    EventMaskType WaitEvents;
} Os_TaskControlBlockType;

typedef struct {
    boolean InUse;
    TaskType OwnerTask;
} Os_ResourceControlBlockType;

extern Os_TaskConfigType os_task_cfg[OS_MAX_TASKS];
extern Os_TaskControlBlockType os_tcb[OS_MAX_TASKS];
extern uint8 os_task_count;
extern Os_ResourceConfigType os_resource_cfg[OS_MAX_RESOURCES];
extern Os_ResourceControlBlockType os_resource_cb[OS_MAX_RESOURCES];
extern uint8 os_resource_count;
extern TaskType os_current_task;
extern AppModeType os_active_app_mode;
extern StatusType os_shutdown_error;
extern boolean os_initialized;
extern boolean os_started;
extern boolean os_shutdown_requested;
extern uint32 os_ready_bitmap;
extern uint32 os_ready_stamp_counter;
extern uint32 os_dispatch_count;
extern Os_HookType os_startup_hook;
extern Os_ErrorHookType os_error_hook;
extern uint8 os_isr_cat2_nesting;
extern TaskType os_preempted_task_stack[OS_MAX_TASKS];
extern uint8 os_preempted_task_depth;

#if defined(UNIT_TEST)
void os_clear_task_cfg(void);
void os_clear_resource_cfg(void);
#endif
void os_reset_runtime_state(void);
boolean os_is_valid_task(TaskType TaskID);
boolean os_is_valid_resource(ResourceType ResID);
boolean os_is_extended_task(TaskType TaskID);
void os_recalculate_task_priority(TaskType TaskID);
void os_rebuild_ready_bitmap(void);
void os_report_service_error(uint8 ApiId, uint8 DetErrorId, StatusType Status);
StatusType os_activate_task_internal(TaskType TaskID, boolean AllowPreemption);
TaskType os_select_next_ready_task(void);
void os_complete_running_task(void);
StatusType os_dispatch_one(void);
StatusType os_run_ready_tasks(void);
StatusType os_maybe_dispatch_preemption(void);

#endif /* OS_INTERNAL_H */
