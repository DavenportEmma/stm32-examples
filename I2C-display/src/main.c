#include "stm32f722xx.h"
#include "i2c.h"
#include "ssd1306.h"
#include <string.h>
#include "ascii.h"

static int utf_to_index(char s) {
    return s * 5;
}

int main(void) {
    init_i2c();

    init_ssd1306();

    uint8_t d[1024];
    memset(d, 0x00, 1024);

    char s[] = "hello world\n\rhello world\n\r";
    int s_len = 26;

    int buf_index = 0;

    for(int i = 0; i < s_len; i++) {
        int utf = utf_to_index(s[i]);

        switch(s[i]) {
            case 0xA:   // new line
                buf_index+=128;
                break;
            case 0xD:   // carriage return
                buf_index-=(buf_index%128);
                break;
            default:
                memcpy(&d[buf_index], &font[utf], 5);
                buf_index+=6;
                break;
        }

        if(buf_index > 1024) {
            buf_index-=1024;
        }
    }

    int f = display_ssd1306(d);

    while(1) {
        int j = 128;
        int k = 64;
        int l = 210;
        for(int i = 0; i < 256; i++) {
            memset(&d[buf_index], 0x00, 5);
            memcpy(&d[buf_index], &font[i*5], 5);

            memset(&d[buf_index+6], 0x00, 5);
            memcpy(&d[buf_index+6], &font[j*5], 5);
            j++;
            if(j >= 256) {
                j = 0;
            }

            memset(&d[buf_index+12], 0x00, 5);
            memcpy(&d[buf_index+12], &font[k*5], 5);
            k++;
            if(k >= 256) {
                k = 0;
            }

            memset(&d[buf_index+18], 0x00, 5);
            memcpy(&d[buf_index+18], &font[l*5], 5);
            l++;
            if(l >= 256) {
                l = 0;
            }

            display_ssd1306(d);
        }
    }
}
