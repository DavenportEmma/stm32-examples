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
SemaphoreHandle_t rx_buffer_mutex;

MIDIStep_TypeDef mysteps[8] = {
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 0},
    {CONTROLLER, 0X7B, 0, 0, 1}
};

MIDISequence_TypeDef mysequence = {
    1,
    0,
    8,
    CHANNEL_1,
    mysteps
};

volatile uint8_t rx_buffer[RX_BUFFER_MAX] = {0}; 
volatile int rx_buffer_loc = 0;
volatile int rx_buffer_ready = 0;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    char msg[] = "stack overflow\n\r";
    send_uart(STLINK_UART, msg, sizeof(msg)-1);
    __disable_irq();
}

int main(void) {
    MIDI_mutex = xSemaphoreCreateMutex();
    sequence_mutex = xSemaphoreCreateMutex();
    rx_buffer_mutex = xSemaphoreCreateMutex();

    start_up();

    xTaskCreate(step_task, "step Task", 2048, &mysequence, 1, NULL);
    // xTaskCreate(MIDI_task, "MIDI Task", configMINIMAL_STACK_SIZE, &sequence0, 1, NULL);
    vTaskStartScheduler();

    while(1) {
        
    }
}

void USART3_IRQHandler(void) {
    if (STLINK_UART->ISR & USART_ISR_RXNE) {  // Check if RXNE flag is set
        uint8_t data = STLINK_UART->RDR;  // Read received data (clears RXNE flag)

        /* if the received byte is a start byte (F4) then begin recording

            start | note on, channel 1 | pitch | velocity | end
            f4      90                   3c      7f         f5
                  | note off, channel 1
            f4      80                   3c      7f         f5


        */
        if(data == USART_START_BYTE) {
            rx_buffer_ready = 0;
            rx_buffer_loc = 0;
        } 
        // finish recording when a stop byte (F5) is received and flag the
        // buffer as ready, unless not enough bytes have been received, then
        // discard the buffer
        else if(data == USART_END_BYTE) {
            if(rx_buffer_loc == 4) {
                rx_buffer_ready = 1;
            } else {
                rx_buffer_ready = 0;
                rx_buffer_loc = 0;
            }
        }

        rx_buffer[rx_buffer_loc] = data;
        rx_buffer_loc++;

        // discard the buffer if too many bytes have been received
        if(rx_buffer_loc > RX_BUFFER_MAX) {
            char msg[] = "rx buffer overflow\n\r";
            rx_buffer_ready = 0;
            rx_buffer_loc = 0;
            send_uart(STLINK_UART, msg, sizeof(msg));
        }
    }

    // Clear any flags you’ve handled or don't need further.
    STLINK_UART->RQR |= USART_RQR_RXFRQ;  // Example: Request a flush of the RX buffer (clears RXNE flag)
}