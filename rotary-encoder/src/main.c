#include "stm32f722xx.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    __disable_irq();
}

static void setup() {
    int err;

    USART_Handler u;
    u.uart = USART3;
    u.baud = 9600;
    u.gpio = GPIOD;
    u.tx_pin = 8;
    u.rx_pin = 9;
    u.afr_reg = 1;
    u.rx_interrupts = 0;

    err = init_uart(&u);
    if(err == 0) {
        send_uart(USART3, "ST-Link UART initialised\n\r", 26);
    }

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~((3 << GPIO_MODER_MODER12_Pos) | (3 << GPIO_MODER_MODER11_Pos));
    GPIOA->PUPDR |= ((2 << 24) | (2 << 22));

    EXTI->IMR |= (EXTI_EMR_EM11 | EXTI_EMR_EM12);
    EXTI->RTSR |= (EXTI_RTSR_TR11 | EXTI_RTSR_TR12);
    // EXTI->FTSR |= (EXTI_FTSR_TR11 | EXTI_FTSR_TR12);

    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

volatile int8_t direction = 0;

static void uart_task() {
    // static int8_t prev_direction = 0;

    // while(1) {
    //     if(prev_direction > (direction+1)) {
    //         send_uart(USART3, "-\n\r", 3);
    //         prev_direction = direction;
    //     } else if(prev_direction < (direction-1)) {
    //         send_uart(USART3, "+\n\r", 3);
    //         prev_direction = direction;
    //     } else {}

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    // int8_t step_accumulator = 0;

    // while (1) {
    //     taskENTER_CRITICAL();
    //     int8_t dir = direction;
    //     direction = 0;
    //     taskEXIT_CRITICAL();

    //     step_accumulator += dir;

    //     if (step_accumulator >= 4) {
    //         send_uart(USART3, "+\n\r", 3);
    //         step_accumulator = 0;
    //     } else if (step_accumulator <= -4) {
    //         send_uart(USART3, "-\n\r", 3);
    //         step_accumulator = 0;
    //     }

    //     vTaskDelay(pdMS_TO_TICKS(10));
    // }

    while(1) {
        send_hex(USART3, direction);
        send_uart(USART3, "\n\r", 2);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

int main(void) {
    setup();

    xTaskCreate(uart_task, "uart task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1) {

    }
}

void EXTI15_10_IRQHandler(void) {
    // volatile static uint8_t prev = 0;

    if((EXTI->PR & (1 << 11)) || (EXTI->PR & (1 << 12))) {
        uint16_t pin11 = (GPIOA->IDR & (1 << 11)) ? 1 : 0;
        uint16_t pin12 = (GPIOA->IDR & (1 << 12)) ? 1 : 0;
        // uint8_t state = (pin12 << 1) | pin11;
        
        // if(state == 1) {
        //     direction++;
        // } else if(state == 2) {
        //     direction--;
        // }

        // uint8_t transition = (prev << 2) | state;

        // const int8_t direction_lookup[16] = {
        //     0,  -1,   1,   0,
        //     1,   0,   0,  -1,
        //    -1,   0,   0,   1,
        //     0,   1,  -1,   0
        // };
        
        // int8_t dir = direction_lookup[transition];
        // direction += dir;
        
        // prev = state;

        if(EXTI->PR & (1 << 11)) {
            if(pin12) {
                direction++;
            }
            EXTI->PR |= (1 << 11);      // clear interrupt flag
        }

        if(EXTI->PR & (1 << 12)) {
            if(pin11) {
                direction--;
            }
            EXTI->PR |= (1 << 12);      // clear interrupt flag
        }
        

    }
}
