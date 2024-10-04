#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "cmsis_gcc.h"
#include "stm32f722xx.h"
#include "uart.h"
#include "midi.h"
#include "start_up.h"
#include "tasks.h"
#include "sequence.h"
#include "common.h"

SemaphoreHandle_t MIDI_mutex;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    char msg[] = "stack overflow\n\r";
    send(USART3, msg, sizeof(msg)-1);
    __disable_irq();
}

int main(void) {
    MIDI_mutex = xSemaphoreCreateMutex();

    start_up();

    xTaskCreate(MIDI_task, "MIDI Task", configMINIMAL_STACK_SIZE, &sequence1, 1, NULL);
    xTaskCreate(MIDI_task, "MIDI Task", configMINIMAL_STACK_SIZE, &sequence0, 1, NULL);
    vTaskStartScheduler();

    while(1) {
        
    }
}