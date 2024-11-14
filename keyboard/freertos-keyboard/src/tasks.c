#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "tasks.h"
#include "keyboard.h"

void key_scan_task(void *pvParameters) {
    TickType_t lastWakeTime;
    uint32_t m[COLS] = {0};

    while(1) {
        lastWakeTime = xTaskGetTickCount();
        scan(m);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(KEY_SCAN_MS));
    }
}