#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tasks.h"
#include "timers.h"
#include "autoconf.h"

void tick_task(void *pvParameters) {
    TickType_t lastWakeTime;

    while(1) {
        lastWakeTime = xTaskGetTickCount();

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(4.46));
    }
}