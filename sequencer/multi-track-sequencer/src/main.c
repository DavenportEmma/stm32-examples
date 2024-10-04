#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "config.h"
#include "task.h"
#include "semphr.h"
#include "cmsis_gcc.h"
#include "stm32f722xx.h"
#include "uart.h"
#include "midi.h"
#include "start_up.h"
#include "tasks.h"
#include "common.h"

SemaphoreHandle_t MIDI_mutex;
SemaphoreHandle_t sequence_mutex;


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    __disable_irq();
}

int main(void) {
    MIDI_mutex = xSemaphoreCreateMutex();
    sequence_mutex = xSemaphoreCreateMutex();

    start_up();

    all_channels_off(MIDI_UART);
    
    xTaskCreate(step_task, "step Task", 2048, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1) {
        
    }
}
