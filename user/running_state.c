#include "running_state.h"
#include "mem.h"
#include "debug.h"
#include "user_interface.h"
#include "message.h"
#include "config.h"
#include "../modules/include/cdecode.h"
#include "../modules/include/cencode.h"
#include "temperature.h"
#include "driver/ads1115.h"
#include "driver/i2c_hd44780.h"
#include "util.h"
#include "c_types.h"

State* current_state;
ETSTimer outgoing_updates_timer;

void ICACHE_FLASH_ATTR state_handle_outgoing_updates()
{
    current_state->grill.current_value = temperature_read(current_state->grill);
    current_state->meat.current_value = temperature_read(current_state->meat);
    
    
    char value[16] = "";
    printFloat(current_state->grill.current_value, value);
    
    INFO("ADC: %s\r\n", value);
    
    fan_modify_fan_state(current_state);
    message_send_state(current_state);
    display_show_normal_state(current_state);
    
    if(system_get_time() < current_state->update_after_this_time)
    {
        return;
    }

    if(current_state->needsConfigSend)
    {
        INFO("SENDIG CONFIG UPDATE\r\n");
        current_state->needsConfigSend = false;
    }
}

void ICACHE_FLASH_ATTR state_init()
{
    
    current_state = (State*)os_zalloc(sizeof(State));
    current_state->update_after_this_time = system_get_time() + (2 * 1000 * 1000);
    current_state->needsConfigSend = true; // send data to controller
    
    current_state->grill.adc_input = ADS1115_INPUT_0;
    current_state->grill.upper_threshold = 78;
    
    current_state->meat.adc_input = ADS1115_INPUT_1;
    current_state->meat.upper_threshold = 230;

    os_timer_disarm(&outgoing_updates_timer);
    os_timer_setfn(&outgoing_updates_timer, (os_timer_func_t *)state_handle_outgoing_updates);
    os_timer_arm(&outgoing_updates_timer, 5000, 1);
}

void ICACHE_FLASH_ATTR handle_mqtt_config_update(char* base64Update)
{
    uint16_t expectedLength = base64_decode_expected_len(os_strlen(base64Update));
    INFO("Update length %d\r\n", os_strlen(base64Update));

    if(expectedLength > DEVICE_CONFIG_MAX) {
        INFO("Data update is too large\r\n");
        return;
    }
    if(expectedLength == 0) {
        INFO("No data to update\r\n");
        return;
    }

    // compare to make sure we don't waste write cycles.
    char* newUpdate = (char*)os_zalloc(expectedLength);

    uint16_t actualLength = base64_decode_chars(base64Update, os_strlen(base64Update), newUpdate);
    INFO("%d\r\n", actualLength);

    if(sysCfg.device_config_length != actualLength &&
        os_memcmp(newUpdate, sysCfg.device_config, actualLength) == 0)
    {
        INFO("No change in update\r\n");
        os_free(newUpdate);
        return;
    }


    sysCfg.device_config_length = actualLength;
    os_memcpy(sysCfg.device_config, newUpdate, actualLength);
    os_free(newUpdate);

    CFG_Save();

    current_state->needsConfigSend = true;
    INFO("Received new update length %d\r\n", actualLength);
}

void ICACHE_FLASH_ATTR handle_mqtt_request(char **respBuffer)
{
    uint16_t expectedLength = base64_encode_expected_len(sysCfg.device_config_length);
    char* allocBuffer = (char*)os_zalloc(expectedLength + 1); // caller MUST free!!!\

    base64_encode_chars(sysCfg.device_config, sysCfg.device_config_length, allocBuffer);

    allocBuffer[expectedLength] = '\0';

    INFO("3 - length: %d\r\n", os_strlen(allocBuffer));

    *respBuffer = allocBuffer;
}

