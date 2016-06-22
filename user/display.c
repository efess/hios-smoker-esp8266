#include "display.h"
#include "driver/i2c_hd44780.h"
#include "user_interface.h"
#include "user_config.h"

void ICACHE_FLASH_ATTR display_show_loading()
{
    LCD_setCursor(0, 1);
    LCD_print("   Smoker Stoker!");
    LCD_setCursor(0, 2);
    LCD_print("   Starting...  ");
}

void ICACHE_FLASH_ATTR display_show_normal_state(State* state)
{
    char degrees[2] = {(char)223, 0};
    char meat_temp_str[16] = "";
    char grill_temp_str[16] = "";
    char line[20] = "";
    
    printFloat(state->grill.current_value, grill_temp_str);
    printFloat(state->meat.current_value, meat_temp_str);

//    LCD_setCursor(0, 0); 
//    LCD_print(DISPLAY_CLEAR_ROW);
    LCD_setCursor(0, 0);
    os_sprintf(line, "Grill:%6s%sF T:%3d", grill_temp_str, degrees, state->grill.upper_threshold);
    LCD_print(line);
    
//    LCD_setCursor(0, 1); 
//    LCD_print(DISPLAY_CLEAR_ROW);
    LCD_setCursor(0, 1);
    os_sprintf(line, "Meat:%7s%sF T:%3d", meat_temp_str, degrees, state->meat.upper_threshold);
    LCD_print(line);
    
//    LCD_print(grill_temp_str);
//    LCD_print(degrees);
//    LCD_print("F T:");
//    os_sprintf(temp_buffer, "%3d", state->grill.upper_threshold);
//    LCD_print(temp_buffer);

//    LCD_setCursor(0, 1); 
//    LCD_print(DISPLAY_CLEAR_ROW);
//    LCD_setCursor(0, 1);
//    LCD_print("Meat: ");
//    LCD_print(meat_temp_str);
//    LCD_print(degrees);
//    LCD_print("F T:");
//    os_sprintf(temp_buffer, "%3d", state->meat.upper_threshold);
//    LCD_print(temp_buffer);

    LCD_setCursor(0, 2); 
    LCD_print("Fan speed: ");
    os_sprintf(line, "%1d", state->fanState.level);
    LCD_print(line);
    
    LCD_setCursor(0, 3); 
    os_sprintf(line, "%s%11d", STA_SSID, system_get_time());
    LCD_print(line);
}

