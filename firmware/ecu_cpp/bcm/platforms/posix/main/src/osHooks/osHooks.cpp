// Taktflow Systems - FreeRTOS OS hooks

#include "FreeRTOS.h"
#include "task.h"

extern "C"
{
void vApplicationStackOverflowHook(TaskHandle_t /* xTask */, char* /* pcTaskName */)
{
    while (true) {}
}

} // extern "C"
