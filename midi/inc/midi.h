#ifndef MIDI_H
#define MIDI_H

typedef enum {
    NOTE_OFF    = 0x80,
    NOTE_ON     = 0x90
} MIDIStatus_TypeDef;

typedef enum {
    CHANNEL_1   = 0x00,
    CHANNEL_2   = 0x01,
    CHANNEL_3   = 0x02,
    CHANNEL_4   = 0x03,
    CHANNEL_5   = 0x04,
    CHANNEL_6   = 0x05,
    CHANNEL_7   = 0x06,
    CHANNEL_8   = 0x07,
    CHANNEL_9   = 0x08,
    CHANNEL_10  = 0x09,
    CHANNEL_11  = 0x0A,
    CHANNEL_12  = 0x0B,
    CHANNEL_13  = 0x0C,
    CHANNEL_14  = 0x0D,
    CHANNEL_15  = 0x0E,
    CHANNEL_16  = 0x0F
} MIDIChannel_TypeDef;

typedef enum {
    C4          = 0x3C,
    C4_SHARP    = 0x3D,
    D4          = 0x3E,
    D4_SHARP    = 0x3F,
    E4          = 0x40,
    F4          = 0x41,
    F4_SHARP    = 0x42,
    G4          = 0x43,
    G4_SHARP    = 0x44,
    A4          = 0x45,
    A4_SHARP    = 0x46,
    B4          = 0x47,
    C5          = 0x48
} MIDINote_TypeDef;

typedef enum {
    NO_ERROR            = 0x00,
    SEND_ERR_TIMEOUT    = 0x01
} MIDIError_TypeDef;

#endif  // MIDI_H