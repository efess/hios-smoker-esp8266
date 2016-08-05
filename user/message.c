#include "user_interface.h"
#include "message.h"
#include "osapi.h"
#include "state.h"
#include "mqttclient.h"
#include "util.h"

void ICACHE_FLASH_ATTR message_send_state(State* state)
{
    char probes[STATE_PROBE_COUNT][16];
    char stateStr[200] = "";
    uint8_t i = 0;
    for(i = 0; i < STATE_PROBE_COUNT; i++)
    {
        os_memset(probes[i], 0, 16);
        printFloat(state->probes[i].current_value, probes[i]);
    }
    
    os_sprintf(stateStr, "{\"probe0\": \"%s\", \"probe1\": \"%s\", \"probe2\": \"%s\", \"probe3\": \"%s\", \"fanState\": \"%d\"}", 
               probes[0], probes[1], probes[2], probes[3], state->fanState);
    
    MQTT_Publish(mqttClient, TOPIC_UPDATE, stateStr, strlen(stateStr), 0, 0);
}
