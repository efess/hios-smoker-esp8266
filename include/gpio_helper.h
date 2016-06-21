#ifndef _GPIO_HELPER_H_
#define _GPIO_HELPER_H_

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x1
#define OUTPUT 0x0

#define GPIO_PIN_ADDR(i)    (GPIO_PIN0_ADDRESS + i*4)

int ICACHE_FLASH_ATTR digitalRead(uint8_t pin);
void ICACHE_FLASH_ATTR digitalWrite(uint8_t pin, uint8_t state);
void ICACHE_FLASH_ATTR pinMode(uint8_t pin, uint8_t mode, uint8_t pullup);

#endif // _GPIO_HELPER_H_
