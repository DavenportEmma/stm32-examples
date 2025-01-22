#include "stm32f722xx.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "midi.h"
#include "setup.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    __disable_irq();
}

int main(void) {
    setup();

    all_channels_off(USART1);

    xTaskCreate(tick_task, "tick task", 2048, NULL, 1, NULL);

    vTaskStartScheduler();
    while(1){}
}