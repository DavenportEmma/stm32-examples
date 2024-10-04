#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tasks.h"
#include "uart.h"
#include "midi.h"
#include "common.h"
#include "timers.h"
#include "config.h"

static void flash_callback(TimerHandle_t xTimer) {
    USER_LED_GPIO->ODR &= ~(1 << USER_LED_GPIO_PIN);
}

static void flash_tempo_led(xTimerHandle t) {
    USER_LED_GPIO->ODR |= (1 << USER_LED_GPIO_PIN);
    if(t) {
        send_uart(STLINK_UART, "before\n\r", 8);
        xTimerStart(t, 0);
        send_uart(STLINK_UART, "after\n\r", 7);

    } else {
        char msg[] = "flash timer not initialised\n\r";
        send_uart(STLINK_UART, msg, sizeof(msg)-1);
    }
}

void step_task(void *pvParameters) {
    MIDISequence_TypeDef* mysequence = (MIDISequence_TypeDef*) pvParameters;

    xTimerHandle t = xTimerCreate(
        "flash timer",
        pdMS_TO_TICKS(TEMPO_LED_FLASH_PERIOD),
        pdFALSE,
        NULL,
        flash_callback
    );

    TickType_t lastWakeTime;
    
    while(1) {
        lastWakeTime = xTaskGetTickCount();
        
        flash_tempo_led(t);

        int counter = mysequence->counter;
        if(xSemaphoreTake(sequence_mutex, portMAX_DELAY) == pdTRUE) {
            if(rx_buffer_ready) {
                rx_buffer_ready = 0;
                rx_buffer_loc = 0;

                mysequence->steps[counter].status = rx_buffer[1];
                mysequence->steps[counter].note = rx_buffer[2];
                mysequence->steps[counter].velocity = rx_buffer[3];
            }
            xSemaphoreGive(sequence_mutex);
        }

        MIDIStep_TypeDef previous_step;
        MIDIStep_TypeDef this_step = mysequence->steps[counter];

        if(counter == 0) {
            previous_step = mysequence->steps[mysequence->num_steps - 1];
        } else {
            previous_step = mysequence->steps[counter - 1];
        }

        MIDIPacket_TypeDef previous_packet = {
            previous_step.status,
            mysequence->channel,
            previous_step.note,
            previous_step.velocity,
        };

        MIDIPacket_TypeDef this_packet = {
            this_step.status,
            mysequence->channel,
            this_step.note,
            this_step.velocity,
        };
        
        if(xSemaphoreTake(MIDI_mutex, portMAX_DELAY) == pdTRUE) {
            if(previous_step.glide == 0) {
                previous_packet.status = NOTE_OFF;
                send_midi_note(MIDI_UART, &previous_packet);
            }

            send_midi_note(MIDI_UART, &this_packet);
            
            
            if(previous_step.glide == 1) {
                previous_packet.status = NOTE_OFF;
                send_midi_note(MIDI_UART, &previous_packet);
            }

            xSemaphoreGive(MIDI_mutex);
        }

        if(xSemaphoreTake(sequence_mutex, portMAX_DELAY) == pdTRUE) {
            mysequence->counter++;
            if(mysequence->counter >= mysequence->num_steps) {
                mysequence->counter = 0;
            }
            xSemaphoreGive(sequence_mutex);
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(STEP_MS));
    }
}
