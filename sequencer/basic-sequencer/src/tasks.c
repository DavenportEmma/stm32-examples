#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "tasks.h"
#include "uart.h"
#include "start_up.h"
#include "common.h"
#include "sequence.h"

void MIDI_task(void* pvParameters) {
    MIDISequence_TypeDef* sequence = (MIDISequence_TypeDef*) pvParameters;
    
    while(1) {
        TickType_t lastWakeTime = xTaskGetTickCount();

        MIDIStep_TypeDef previous_step;
        MIDIStep_TypeDef this_step = sequence->steps[sequence->counter];

        if(sequence->counter == 0) {
            previous_step = sequence->steps[sequence->num_steps - 1];
        } else {
            previous_step = sequence->steps[sequence->counter - 1];
        }

        MIDIPacket_TypeDef previous_packet = {
            previous_step.status,
            sequence->channel,
            previous_step.note,
            previous_step.velocity,
            previous_step.glide
        };

        MIDIPacket_TypeDef this_packet = {
            this_step.status,
            sequence->channel,
            this_step.note,
            this_step.velocity,
            this_step.glide
        };

        if(xSemaphoreTake(MIDI_mutex, portMAX_DELAY) == pdTRUE) {
            if(previous_step.glide == 0) {
                previous_packet.status = NOTE_OFF;
                send_midi_note(USART1, &previous_packet);
            }

            if(send_midi_note(USART1, &this_packet) > 0) {
                char msg[] = "timeout\n\r";
                send(USART3, msg, sizeof(msg)-1);
            }

            if(previous_step.glide == 1) {
                previous_packet.status = NOTE_OFF;
                send_midi_note(USART1, &previous_packet);
            }

            xSemaphoreGive(MIDI_mutex);
        }

        if(this_step.end_of_sequence) {
            sequence->counter = 0;
        } else {
            sequence->counter++;
        }

        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(200) * sequence->divisor);
    }
}