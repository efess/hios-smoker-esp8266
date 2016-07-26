#ifndef TEMPERATURE
#define TEMPERATURE

#include "os_type.h"

#define TEMPERATURE_RANGE_SIZE (32)
#define TEMPERATURE_READ_TRIES (10)
#define TEMPERATURE_MEAT_COUNT 3

typedef struct {
    uint8_t enabled;
    uint8_t adc_input;
    float current_value;
    char name[16];
    uint16_t target;
} TempState;

float ICACHE_FLASH_ATTR temperature_read(TempState state);

#endif // TEMPERATURE
