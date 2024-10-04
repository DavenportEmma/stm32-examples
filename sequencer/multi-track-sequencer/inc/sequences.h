#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "midi.h"

MIDIStep_TypeDef steps0[8] = {
    {NOTE_ON, A4, 127, 0, 0},
    {NOTE_ON, A3, 127, 0, 0},
    {NOTE_ON, C4, 127, 0, 0},
    {NOTE_ON, A4, 127, 0, 0},
    {NOTE_ON, A3, 127, 0, 0},
    {NOTE_ON, G3, 127, 0, 0},
    {NOTE_ON, C4, 127, 0, 0},
    {NOTE_ON, A3, 127, 0, 1}
};

static const MIDISequence_TypeDef sequence0 = {
    1,
    0,
    8,
    CHANNEL_1,
    steps0
};

MIDIStep_TypeDef steps1[8] = {
    {NOTE_ON, A3, 127, 0, 0},
    {NOTE_ON, A2, 127, 0, 1}
};

static const MIDISequence_TypeDef sequence1 = {
    16,
    0,
    2,
    CHANNEL_2,
    steps1
};

MIDISequence_TypeDef sequence_arr[2] = {
    sequence0,
    sequence1
};

#endif