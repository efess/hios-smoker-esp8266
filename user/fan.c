#include "fan.h"
#include "user_interface.h"
#include "gpio_helper.h"
#include "pwm.h"
#include "debug.h"

uint32 calc_duty(uint8_t percentage)
{
    if(percentage <= 0) return 0;
    if(percentage >= 100) return ((PWM_PERIOD * 1000) / 45);
    
    float ratio = ((float)percentage / 100.0);
    return (uint32)((PWM_PERIOD * 1000) / 45) * ratio;
}

void ICACHE_FLASH_ATTR fan_init()
{
    //pinMode(FAN_GPIO_PIN, OUTPUT, 1);
    
    uint32 pinCfg [][3] = {
       {PWM_0_OUT_IO_MUX,PWM_0_OUT_IO_FUNC,PWM_0_OUT_IO_NUM}  
    };
    
    // start OFF
    uint32 duty_init[] = {calc_duty(0)};
    
    pwm_init(
        PWM_PERIOD,
        duty_init,
        1, // channel num
        pinCfg);
    pwm_start();
}

void ICACHE_FLASH_ATTR fan_modify_fan_state(State* state, AppCfg* cfg)
{
    
    uint32_t seconds_since_boot = system_get_time() / 1000000;
    uint32_t seconds_since_change = seconds_since_boot - state->fanState.timestamp_last_state_change;
    int16_t temp_difference = cfg->grillTarget - state->probes[STATE_PROBEID_GRILL].current_value;
    
    
    uint8_t seconds_off_time = 0;
    uint8_t state_changed = 0;
    
    if(temp_difference < FAN_LEVEL_ZERO_TEMP_TARGET_DIFFERENCE)
    {
        state->fanState.level = 0;
    }
    else if(temp_difference <= FAN_LEVEL_ONE_TEMP_TARGET_DIFFERENCE)
    {
        state->fanState.level = 1;
        seconds_off_time = FAN_LEVEL_ONE_INTERVAL_SECONDS;
    }
    else if(temp_difference <= FAN_LEVEL_TWO_TEMP_TARGET_DIFFERENCE)
    {
        state->fanState.level = 2;
        seconds_off_time = FAN_LEVEL_TWO_INTERVAL_SECONDS;
    }
    else if(temp_difference <= FAN_LEVEL_THREE_TEMP_TARGET_DIFFERENCE)
    {
        state->fanState.level = 3;
        seconds_off_time = FAN_LEVEL_THREE_INTERVAL_SECONDS;
    }
    else
    {
        state->fanState.level = 4;
        seconds_off_time = FAN_LEVEL_FOUR_INTERVAL_SECONDS;
    }
    
    INFO("TIMESTAMP: %d\r\n ", seconds_since_boot);
    INFO("STATECHANGE: %d\r\n ", seconds_since_change);
    if(state->fanState.is_on) 
    {
        if(seconds_since_change > FAN_INTERVAL_ON_SECONDS)
        {
            state->fanState.is_on = FAN_IS_OFF;
            state->fanState.timestamp_last_state_change = seconds_since_boot;
            state_changed = 1;
        }
    } 
    else 
    {
        // fan is off
        if(state->fanState.level != 0 && seconds_since_change > seconds_off_time) 
        {
            state->fanState.is_on = FAN_IS_ON;
            state->fanState.timestamp_last_state_change = seconds_since_boot;
            state_changed = 1;
        }
    }
    
    if(state_changed) 
    {
        if(state->fanState.is_on) 
        {
            pwm_set_duty(calc_duty(50), 0);
            pwm_start();
            INFO("Turning fan ON\r\n");
        } 
        else 
        {
            pwm_set_duty(calc_duty(0), 0);
            pwm_start();
            INFO("Turning fan OFF\r\n");
        }
    }
}
