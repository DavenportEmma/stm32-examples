/*
    midi pass through example project
*/

#include "stm32f722xx.h"
#include "uart.h"
#include "midi.h"

uint8_t buffer[3] = {0};
volatile i = 0;

MIDIChannel_t routing_channel = CHANNEL_11;

int main(void) {
    USART_Handler u;
    u.uart = USART1;
    u.baud = 31250;
    u.gpio = GPIOB;
    u.tx_pin = 6;
    u.rx_pin = 7;
    u.afr_reg = 0;
    u.rx_interrupts = 1;

    if(init_uart(&u)) {
        while(1) {}
    }

    NVIC_EnableIRQ(USART1_IRQn);

    while(1) {
        if(i >= 3) {
            MIDIStatus_t status = buffer[0] & 0xF0;
            MIDIChannel_t channel = routing_channel;
            MIDINote_t note = buffer[1];
            uint8_t velocity = buffer[2];

            MIDIPacket_t p = {
                status, channel, note, velocity
            };

            send_midi_note(USART1, &p);
            
            i = 0;
        }
    }
}

void USART1_IRQHandler(void) {
    if(USART1->ISR & USART_ISR_RXNE) {
        while(USART1->ISR & USART_ISR_RXNE) {
            uint8_t d = USART1->RDR;
            // USART1->TDR = d;
            buffer[i] = d;
            i++;
            if(i >= 3) {
                i = 3;
            }
        }
    }
}