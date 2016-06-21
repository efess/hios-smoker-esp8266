#include "user_interface.h"
#include "message.h"
#include "osapi.h"
#include "state.h"
#include "mqttclient.h"
#include "util.h"

void ICACHE_FLASH_ATTR message_send_state(State* state)
{
    char grill[16] = "";
    char meat[16] = "";
    char stateStr[200] = "";
    
    printFloat(state->grill.current_value, grill);
    printFloat(state->meat.current_value, meat);
    os_sprintf(stateStr, "{\"grill\": \"%s\", \"meat\": \"%s\", \"fanState\": %d}", 
               grill, meat, state->fanState);
    
    MQTT_Publish(mqttClient, TOPIC_UPDATE, stateStr, strlen(stateStr), 0, 0);
}
