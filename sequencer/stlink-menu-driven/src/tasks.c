#include "stm32f722xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "menu.h"
#include "config.h"

int wait_for_input() {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    return usart_rx_data;
}

void menu_task() {
    int menu_id = SEQUENCE_SELECT;
    int sequence = 0;
    int step = 0;

    while(1) {
        switch(menu_id) {
            case SEQUENCE_SELECT:
                print("*** select a sequence ***\n\r\0");
                /*
                    print the sequences and show which ones are active
                */
                sequence = wait_for_input();
                menu_id = FUNCTION_SELECT;
                break;
            case FUNCTION_SELECT:
                print("*** choose function ***\n\r\0");
                /*
                    after a sequence has been selected print:
                        - active steps
                        - prescaler
                        - midi channel  
                */
                print("1 - modify step\n\r\0");
                print("2 - change prescaler\n\r\0");
                print("3 - set midi channel\n\r\0");
                print("4 - set active steps\n\r\0");
                print("5 - clear sequence\n\r\0");
                int func = wait_for_input();
                switch(func) {
                    case 1:
                        menu_id = STEP_SELECT;
                        break;
                    case 2:
                        menu_id = CHANGE_PRESCALER;
                        break;
                    case 3:
                        menu_id = SET_MIDI_CHANNEL;
                        break;
                    case 4:
                        menu_id = SET_ACTIVE_STEPS;
                        break;
                    case 5:
                        // clear sequence
                        break;
                }
                break;
            case STEP_SELECT:
                print("*** choose step ***\n\r\0");
                step = wait_for_input();
                menu_id = MODIFY_STEP;
                break;
            case MODIFY_STEP:
                print("*** choose modification ***\n\r\0");
                print("1 - pitch\n\r\0");
                print("2 - velocity\n\r\0");
                print("3 - glide\n\r\0");
                int mod = wait_for_input();
                switch(mod) {
                    case 1:
                        menu_id = PITCH_MENU;
                        break;
                    case 2:
                        menu_id = VELOCITY_MENU;
                        break;
                    case 3:
                        menu_id = GLIDE_MENU;
                        break;
                }
                break;
            case PITCH_MENU:
                print("*** modify pitch ***\n\r\0");
                int pitch = wait_for_input();
                menu_id = SEQUENCE_SELECT;
                break;
            case VELOCITY_MENU:
                print("*** modify velocity ***\n\r\0");
                int velocity = wait_for_input();
                menu_id = SEQUENCE_SELECT;
                break;
            case GLIDE_MENU:
                print("*** modify glide ***\n\r\0");
                int glide = wait_for_input();
                menu_id = SEQUENCE_SELECT;
                break;
        }
    }
}