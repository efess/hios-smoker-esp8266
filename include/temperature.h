#ifndef TEMPERATURE
#define TEMPERATURE

#include "os_type.h"
#include "state.h"

#define TEMPERATURE_RANGE_SIZE (32)
#define TEMPERATURE_READ_TRIES (10)
#define TEMPERATURE_PROBE_COUNT 4
#define TEMPERATURE_MEAT_COUNT 3

float ICACHE_FLASH_ATTR temperature_read(ProbeState state);

#endif // TEMPERATURE
