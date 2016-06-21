#ifndef i2c_hd44780_h
#define i2c_hd44780_h

#include "i2c.h"
#include "lcd_config.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT   0x08 
#define LCD_NOBACKLIGHT 0x00 

#define En 0x04  // Enable bit
#define Rw 0x02  // Read/Write bit
#define Rs 0x01  // Register select bit

void ICACHE_FLASH_ATTR LCD_clear();
void ICACHE_FLASH_ATTR LCD_home();
void ICACHE_FLASH_ATTR LCD_noDisplay();
void ICACHE_FLASH_ATTR LCD_display();
void ICACHE_FLASH_ATTR LCD_noBlink();
void ICACHE_FLASH_ATTR LCD_blink();
void ICACHE_FLASH_ATTR LCD_noCursor();
void ICACHE_FLASH_ATTR LCD_cursor();
void ICACHE_FLASH_ATTR LCD_scrollDisplayLeft();
void ICACHE_FLASH_ATTR LCD_scrollDisplayRight();
void ICACHE_FLASH_ATTR LCD_printLeft();
void ICACHE_FLASH_ATTR LCD_printRight();
void ICACHE_FLASH_ATTR LCD_leftToRight();
void ICACHE_FLASH_ATTR LCD_rightToLeft();
void ICACHE_FLASH_ATTR LCD_shiftIncrement();
void ICACHE_FLASH_ATTR LCD_shiftDecrement();
void ICACHE_FLASH_ATTR LCD_noBacklight();
void ICACHE_FLASH_ATTR LCD_backlight();
void ICACHE_FLASH_ATTR LCD_autoscroll();
void ICACHE_FLASH_ATTR LCD_noAutoscroll(); 
void ICACHE_FLASH_ATTR LCD_createChar(uint8, uint8[]);
void ICACHE_FLASH_ATTR LCD_setCursor(uint8, uint8); 
void ICACHE_FLASH_ATTR LCD_print(char[]);
uint8 ICACHE_FLASH_ATTR LCD_init();
#endif
