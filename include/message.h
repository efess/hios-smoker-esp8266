#ifndef MESSAGE_H
#define MESSAGE_H
#include "state.h"

#define TOPIC_UPDATE "/home/outside/smoker/stoker/state"
void ICACHE_FLASH_ATTR message_send_state(State* state);

#endif // MESSAGE_H

