#include "stm32f722xx.h"
#include "i2c.h"
#include "ssd1306.h"

int main(void) {
    init_i2c();

    init_ssd1306();

    uint8_t buffer[1024] = {0};
    buffer[512] = 0xFF;

    display_ssd1306(buffer);

    while(1) {

    }
}
