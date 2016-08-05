#ifndef DISPLAY
#define DISPLAY

#include "state.h"
#include "config.h"
#include "util.h"

#define DISPLAY_CLEAR_ROW  "                    "
void ICACHE_FLASH_ATTR display_show_loading();

void ICACHE_FLASH_ATTR display_show_normal_state(State* state, AppCfg* cfg);

#endif // DISPLAY

