#ifndef STATE_H
#define STATE_H

#include "c_types.h"

#define STATE_PROBE_COUNT 4
#define STATE_PROBE_MEAT_COUNT 3
#define STATE_PROBEID_GRILL 0
#define STATE_PROBEID_FIRST_MEAT 1

typedef struct {
    uint8_t probe_id_shown;
    uint32_t timestamp_last_state_change;
} LcdState;

typedef struct {
    uint8_t level;
    uint8_t is_on;
    uint32_t timestamp_last_state_change;
} FanState;

typedef struct {
    uint8_t adc_input;
    float current_value;
} ProbeState;

typedef struct {
    FanState fanState;
    
    ProbeState probes[STATE_PROBE_COUNT];
    
    LcdState lcdState;
} State;

#endif // STATE_H

