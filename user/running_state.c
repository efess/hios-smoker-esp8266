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
#include "mqttclient.h"

AppCfg* appCfg;
State* current_state;
ETSTimer outgoing_updates_timer;

void ICACHE_FLASH_ATTR set_app_cfg(){
    if(!sysCfg.device_config_length || 
            sysCfg.device_config_length != sizeof(AppCfg)) {
        INFO("set app cfg - defaults - config length is %d\r\n", sysCfg.device_config_length);
        sysCfg.device_config_length = sizeof(AppCfg);
        
        appCfg = (AppCfg*)sysCfg.device_config;
        
        appCfg->grillTarget = 230;
        appCfg->fanPulseSeconds = 8;
    } else {
        INFO("set app cfg\r\n");
        appCfg = (AppCfg*)sysCfg.device_config;
    }
}

void ICACHE_FLASH_ATTR state_handle_outgoing_updates()
{
    uint8_t i;
    for(i = 0; i < STATE_PROBE_COUNT; i++)
    {
        current_state->probes[i].current_value = temperature_read(current_state->probes[i]);
    }
    
    fan_modify_fan_state(current_state, appCfg);
    message_send_state(current_state);
    display_show_normal_state(current_state, appCfg);
}

void ICACHE_FLASH_ATTR state_init()
{
    set_app_cfg();
    
    current_state = (State*)os_zalloc(sizeof(State));
    current_state->probes[0].adc_input = ADS1115_INPUT_0;
    current_state->probes[1].adc_input = ADS1115_INPUT_1;
    current_state->probes[2].adc_input = ADS1115_INPUT_2;
    current_state->probes[3].adc_input = ADS1115_INPUT_3;

    os_timer_disarm(&outgoing_updates_timer);
    os_timer_setfn(&outgoing_updates_timer, (os_timer_func_t *)state_handle_outgoing_updates);
    os_timer_arm(&outgoing_updates_timer, 5000, 1);
}

void ICACHE_FLASH_ATTR test_app_cfg(){
    
    INFO("- %s\r\n", appCfg->probes[0].name);
    INFO("- %d\r\n", appCfg->probes[0].target);
    INFO("- %d\r\n", appCfg->probes[0].probId);
    INFO("- %d\r\n", appCfg->probes[0].enabled);
    INFO("----------------\r\n");
    INFO("- %s\r\n", appCfg->probes[1].name);
    INFO("- %d\r\n", appCfg->probes[1].target);
    INFO("- %d\r\n", appCfg->probes[1].probId);
    INFO("- %d\r\n", appCfg->probes[1].enabled);
    INFO("----------------\r\n");
    INFO("- %s\r\n", appCfg->probes[2].name);
    INFO("- %d\r\n", appCfg->probes[2].target);
    INFO("- %d\r\n", appCfg->probes[2].probId);
    INFO("- %d\r\n", appCfg->probes[2].enabled);
    INFO("----------------\r\n");
    INFO("- %d\r\n", appCfg->grillTarget);
    INFO("- %d\r\n", appCfg->fanPulseSeconds);
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
    set_app_cfg();
    test_app_cfg();

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

