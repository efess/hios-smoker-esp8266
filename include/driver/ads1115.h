#ifndef ADS1115_H
#define ADS1115_H

#include "os_type.h"

#define ADS1115_CONFIG_MPLEX 12
#define ADS1115_ADDRESS 0x48

#define ADS1115_REGISTER_CONVERSION 0x00
#define ADS1115_REGISTER_CONFIG 0x01

#define ADS1115_WRITE_BIT 0x00
#define ADS1115_READ_BIT 0x01

#define ADS1115_INPUT_0 0x04
#define ADS1115_INPUT_1 0x05

#define ADS1115_READ_DELAY 3000 // 3 ms between reads

int8_t ICACHE_FLASH_ATTR ads1115_init(void);
int16_t ICACHE_FLASH_ATTR ads1115_read_analog(uint8_t ain);



#endif // ADS1115_H
