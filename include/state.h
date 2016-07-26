#ifndef STATE_H
#define STATE_H

#include "c_types.h"
#include "temperature.h"

typedef struct {
    uint8_t meat_index;
    uint32_t timestamp_last_state_change;
} LcdState;

typedef struct {
    uint8_t level;
    uint8_t is_on;
    uint32_t timestamp_last_state_change;
} FanState;

typedef struct {
    uint32_t update_after_this_time;
    bool needsConfigSend;
  
    FanState fanState;
    
    TempState grill;
    TempState meats[TEMPERATURE_MEAT_COUNT];
    
    LcdState lcdState;
} State;

#endif // STATE_H

