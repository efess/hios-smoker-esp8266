#ifndef TEMPERATURE
#define TEMPERATURE

#include "os_type.h"

#define TEMPERATURE_RANGE_SIZE (32)
#define TEMPERATURE_READ_TRIES (10)

typedef struct {
    uint8_t adc_input;
    float current_value;
    uint16_t upper_threshold;
    uint16_t lower_threshold;
} TempState;

float ICACHE_FLASH_ATTR temperature_read(TempState state);

#endif // TEMPERATURE
