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
#include "sequences.h"

static void flash_callback(TimerHandle_t xTimer) {
    USER_LED_GPIO->ODR &= ~(1 << USER_LED_GPIO_PIN);
}

static void flash_tempo_led(xTimerHandle t) {
    USER_LED_GPIO->ODR |= (1 << USER_LED_GPIO_PIN);
    if(t) {
        xTimerStart(t, 0);
    }
}

static void play_step(
    MIDIChannel_TypeDef channel,
    MIDIStep_TypeDef this_step,
    MIDIStep_TypeDef prev_step
) {
    MIDIPacket_TypeDef prev_packet = {
        prev_step.status,
        channel,
        prev_step.note,
        prev_step.velocity,
    };

    MIDIPacket_TypeDef this_packet = {
        this_step.status,
        channel,
        this_step.note,
        this_step.velocity,
    };

    if(xSemaphoreTake(MIDI_mutex, portMAX_DELAY) == pdTRUE) {
        if(prev_step.glide == 0) {
            prev_packet.status = NOTE_OFF;
            send_midi_note(MIDI_UART, &prev_packet);
        }

        send_midi_note(MIDI_UART, &this_packet);
        
        if(prev_step.glide == 1) {
            prev_packet.status = NOTE_OFF;
            send_midi_note(MIDI_UART, &prev_packet);
        }

        xSemaphoreGive(MIDI_mutex);
    }
}

static void play_sequence(MIDISequence_TypeDef* s) {
    int counter = s->counter;

    MIDIStep_TypeDef prev_step;
    MIDIStep_TypeDef this_step = s->steps[counter];

    if(counter == 0) {
        prev_step = s->steps[s->num_steps - 1];
    } else {
        prev_step = s->steps[counter - 1];
    }

    play_step(s->channel, this_step, prev_step);

    if(xSemaphoreTake(sequence_mutex, portMAX_DELAY) == pdTRUE) {
        s->counter++;
        if(s->counter >= s->num_steps) {
            s->counter = 0;
        }
        xSemaphoreGive(sequence_mutex);
    }
}

void step_task(void *pvParameters) {
    int pulse_count = 0;

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
        
        if(pulse_count % PPQN == 0) {
            flash_tempo_led(t);
        }

        for(int i = 0; i < 2; i++)
        {
            MIDISequence_TypeDef* seq = &sequence_arr[i];
            // only play a step if the pulse count is a multiple of the
            // prescaler. 
            if(pulse_count % seq->prescaler == 0) {
                play_sequence(seq);
            }
        }

        pulse_count++;
        if(pulse_count >= MAX_PRESCALER_VALUE) {
            pulse_count = 0;
        }
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(STEP_MS));
    }
}
