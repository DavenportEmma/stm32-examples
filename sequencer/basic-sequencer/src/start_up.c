#include "stm32f722xx.h"
#include "start_up.h"
#include "midi.h"
#include "uart.h"

int all_notes_off() {
    // turn off all notes
    MIDICC_TypeDef *c;
    c->status = CONTROLLER;
    c->channel = CHANNEL_12;
    c->control = ALL_NOTES_OFF;
    c->value = 0;

    return send_midi_control(USART1, c);
}

void start_up() {
    // initialise stlink uart
    if(init_uart(USART3, 9600) == 0) {
        char msg[] = "USART3 initialised\n\r";
        send(USART3, msg, sizeof(msg)-1);
    } else {
        while(1) {}
    }

    // initialise midi uart
    if(init_uart(USART1, 31250) == 0) {
        char msg[] = "USART1 initialised\n\r";
        send(USART3, msg, sizeof(msg)-1);
    } else {
        char msg[] = "USART1 failed to initialise\n\r";
        send(USART3, msg, sizeof(msg)-1);
    }

    all_notes_off();
}