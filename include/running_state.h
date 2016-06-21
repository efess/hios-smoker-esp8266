#include "osapi.h"
#include "state.h"
#include "os_type.h"
#include "user_config.h"

void ICACHE_FLASH_ATTR state_init();
void ICACHE_FLASH_ATTR state_update();
void ICACHE_FLASH_ATTR handle_mqtt_config_update(char *reqBuffer);
void ICACHE_FLASH_ATTR handle_mqtt_request(char **respBuffer);
