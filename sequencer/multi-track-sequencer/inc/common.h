#ifndef _COMMON_H
#define _COMMON_H

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t MIDI_mutex;
extern SemaphoreHandle_t sequence_mutex;

#endif // _COMMON_H