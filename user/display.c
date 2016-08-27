#include "display.h"
#include "driver/i2c_hd44780.h"
#include "user_interface.h"
#include "user_config.h"
#include"debug.h"

void ICACHE_FLASH_ATTR display_show_loading()
{
    LCD_setCursor(0, 1);
    LCD_print("   Smoker Stoker!");
    LCD_setCursor(0, 2);
    LCD_print("   Starting...  ");
}

void ICACHE_FLASH_ATTR set_next_probe_id(State* state, AppCfg* cfg)
{
    
}

void ICACHE_FLASH_ATTR update_state(State* state, AppCfg* cfg)
{
    uint32_t seconds_since_boot = system_get_time() / 1000000;
    uint32_t seconds_since_change = seconds_since_boot - state->lcdState.timestamp_probe_state_change;
    uint8_t c = 3;
    uint8_t i = state->lcdState.probe_id_shown;
    
    if(seconds_since_change > DISPLAY_SWITCH_PROBE_AFTER_SECONDS) 
    {
        INFO("changing probe display..\r\n");
        seconds_since_change = seconds_since_boot;
        for(c = 0;c < 3; c++) 
        {
            i = (i + 1) % 3;
            if(cfg->probes[i].enabled) 
            {
                INFO("probe selected %d\r\n", i);
                state->lcdState.probe_id_shown = i;
                break;
            }
        }
    }
}

// [<grill:7><4><temp:4><1><temp:4>]
// [<meat name:16>         <temp:4>]
// [<fan state:5>          <temp:4>]
// [
void ICACHE_FLASH_ATTR display_show_normal_state(State* state, AppCfg* cfg)
{
    
    char degrees[2] = {(char)223, 0};
    
    // Tried using %11.11s but chip crashed, maybe doesn't support precision format for char arrays
    char grill_temp_str[12] = "";
    char meat_temp_str[12] = "";
    char line[21] = "";
    
    update_state(state, cfg);
    
    ProbeState* meatState = &state->probes[STATE_PROBEID_FIRST_MEAT + state->lcdState.probe_id_shown];
    ProbeCfg* meatCfg = &cfg->probes[state->lcdState.probe_id_shown];
    
    os_strncpy(meat_temp_str, meatCfg->name, 11);
    printFloat(state->probes[STATE_PROBEID_GRILL].current_value, grill_temp_str);
    
    LCD_setCursor(0, 0);
    os_sprintf(line, "%-11s%3d%s %3d%s", "Grill", 
               (int)state->probes[STATE_PROBEID_GRILL].current_value, degrees, 
               cfg->grillTarget, degrees);;
    
    LCD_print(line);
    
    LCD_setCursor(0, 1);
    os_sprintf(line, "%-11s%3d%s %3d%s", meat_temp_str, (int)meatState->current_value, degrees, meatCfg->target, degrees);;
    LCD_print(line);

    LCD_setCursor(0, 2); 
    LCD_print("Fan speed: ");
    os_sprintf(line, "%1d", state->fanState.level);
    LCD_print(line);
    
    LCD_setCursor(0, 3); 
    os_sprintf(line, "%s%11u", STA_SSID, system_get_time());
    LCD_print(line);
}

