/**
 * @file    Os.h
 * @brief   OSEK-first OS starter kernel
 * @date    2026-03-13
 *
 * @details First milestone of the OSEK bootstrap kernel:
 *          - static task configuration
 *          - application modes
 *          - priority-based ready selection
 *          - host-testable dispatch semantics
 *
 *          The current implementation is a host-testable bootstrap with
 *          static priorities, bootstrap-form preemption, and PCP resource
 *          handling before real STM32/TMS570 context switch ports arrive.
 *
 * @safety_req SWR-BSW-050: Static priority task scheduling
 * @traces_to  TSR-OS-001, TSR-OS-002
 *
 * @standard OSEK/VDX, AUTOSAR_CP_OS, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */
#ifndef OS_H
#define OS_H

#include "Std_Types.h"

typedef uint8 StatusType;
typedef uint8 TaskType;
typedef TaskType* TaskRefType;
typedef uint8 AppModeType;
typedef uint8 TaskStateType;
typedef TaskStateType* TaskStateRefType;
typedef uint8 ResourceType;
typedef uint32 EventMaskType;
typedef EventMaskType* EventMaskRefType;
typedef void (*Os_TaskEntryType)(void);
typedef void (*Os_HookType)(void);
typedef void (*Os_ErrorHookType)(StatusType Error);
#if defined(UNIT_TEST)
typedef void (*Os_TestIsrHandlerType)(void);
#endif

typedef struct {
    const char* Name;
    Os_TaskEntryType Entry;
    uint8 Priority;
    uint8 ActivationLimit;
    uint32 AutostartMask;
    boolean Extended;
} Os_TaskConfigType;

typedef struct {
    const char* Name;
    uint8 CeilingPriority;
} Os_ResourceConfigType;

#define OS_MAX_TASKS            8u
#define OS_MAX_RESOURCES        8u
#define OS_MAX_PRIORITIES       8u
#define INVALID_TASK            ((TaskType)0xFFu)
#define INVALID_RESOURCE        ((ResourceType)0xFFu)
#define OSDEFAULTAPPMODE        ((AppModeType)0u)

#define SUSPENDED               ((TaskStateType)0u)
#define READY                   ((TaskStateType)1u)
#define RUNNING                 ((TaskStateType)2u)
#define WAITING                 ((TaskStateType)3u)

#define E_OS_ACCESS             ((StatusType)0x01u)
#define E_OS_CALLEVEL           ((StatusType)0x02u)
#define E_OS_ID                 ((StatusType)0x03u)
#define E_OS_LIMIT              ((StatusType)0x04u)
#define E_OS_NOFUNC             ((StatusType)0x05u)
#define E_OS_RESOURCE           ((StatusType)0x06u)
#define E_OS_STATE              ((StatusType)0x07u)
#define E_OS_VALUE              ((StatusType)0x08u)

void Os_Init(void);
void StartOS(AppModeType Mode);
void ShutdownOS(StatusType Error);

StatusType ActivateTask(TaskType TaskID);
StatusType TerminateTask(void);
StatusType ChainTask(TaskType TaskID);
StatusType Schedule(void);
StatusType GetTaskID(TaskRefType TaskID);
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State);
StatusType SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType ClearEvent(EventMaskType Mask);
StatusType GetEvent(TaskType TaskID, EventMaskRefType Event);
StatusType WaitEvent(EventMaskType Mask);
StatusType GetResource(ResourceType ResID);
StatusType ReleaseResource(ResourceType ResID);
AppModeType GetActiveApplicationMode(void);

#if defined(UNIT_TEST)
void Os_TestReset(void);
StatusType Os_TestConfigureTasks(const Os_TaskConfigType* Config, uint8 TaskCount);
StatusType Os_TestConfigureResources(const Os_ResourceConfigType* Config, uint8 ResourceCount);
void Os_TestSetStartupHook(Os_HookType Hook);
void Os_TestSetErrorHook(Os_ErrorHookType Hook);
StatusType Os_TestRunReadyTasks(void);
TaskType Os_TestGetCurrentTask(void);
uint32 Os_TestGetReadyBitmap(void);
uint8 Os_TestGetPendingActivations(TaskType TaskID);
uint32 Os_TestGetDispatchCount(void);
StatusType Os_TestInvokeIsrCat2(Os_TestIsrHandlerType Handler);
uint8 Os_TestGetIsrCat2Nesting(void);
#endif

#endif /* OS_H */
