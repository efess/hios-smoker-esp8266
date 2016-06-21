#ifndef FAN
#define FAN

#include "state.h"

#define PWM_PERIOD 1000

#define FAN_GPIO_PIN 12

#define FAN_IS_ON 1
#define FAN_IS_OFF 0

#define FAN_INTERVAL_ON_SECONDS 10

// fan levels are from 0 to 4, 0 is completely off, four the fan is on the most
#define FAN_LEVEL_ONE_INTERVAL_SECONDS 60
#define FAN_LEVEL_TWO_INTERVAL_SECONDS 30
#define FAN_LEVEL_THREE_INTERVAL_SECONDS 15
#define FAN_LEVEL_FOUR_INTERVAL_SECONDS 10

// temperature needs to be 
// negative means grill temp is above target
#define FAN_LEVEL_ZERO_TEMP_TARGET_DIFFERENCE -5
#define FAN_LEVEL_ONE_TEMP_TARGET_DIFFERENCE 2
#define FAN_LEVEL_TWO_TEMP_TARGET_DIFFERENCE 5
#define FAN_LEVEL_THREE_TEMP_TARGET_DIFFERENCE 10
#define FAN_LEVEL_FOUR_TEMP_TARGET_DIFFERENCE 20

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12

void ICACHE_FLASH_ATTR fan_modify_fan_state(State* state);

#endif // FAN

