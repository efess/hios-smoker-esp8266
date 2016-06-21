#include "gpio_helper.h"
                                                   /*   GPIO | SDK board    |       */
                                                   /*   --------------------|       */
int gpio_pin_register[16] = {PERIPHS_IO_MUX_GPIO0_U,    // 0  gpio 0        |
                             PERIPHS_IO_MUX_U0TXD_U,    // 1  gpio 1        |
                             PERIPHS_IO_MUX_GPIO2_U,    // 2  gpio 2        |
                             PERIPHS_IO_MUX_U0RXD_U,    // 3                |
                             PERIPHS_IO_MUX_GPIO4_U,    // 4  gpio 4        |
                             PERIPHS_IO_MUX_GPIO5_U,    // 5  gpio 5        |
                             PERIPHS_IO_MUX_SD_CLK_U,   // 6                |
                             PERIPHS_IO_MUX_SD_DATA0_U, // 7                |
                             PERIPHS_IO_MUX_SD_DATA1_U, // 8                |
                             PERIPHS_IO_MUX_SD_DATA2_U, // 9  gpio 9        |
                             PERIPHS_IO_MUX_SD_DATA3_U, // 10 gpio 10       |
                             PERIPHS_IO_MUX_SD_CMD_U,   // 11               |
                             PERIPHS_IO_MUX_MTDI_U,     // 12 gpio12        |
                             PERIPHS_IO_MUX_MTCK_U,     // 13 gpio13        |
                             PERIPHS_IO_MUX_MTMS_U,     // 14 gpio14        |
                             PERIPHS_IO_MUX_MTDO_U};    // 15 gpio15        |

/********************************************************************
* FunctionName: pinMode
* Description : set up a pin mode.
* Parameters  : pin: pin mumber
*             : mode 0 for OUTPUT or 1 for INPUT
*             : pullup 0 for pullup DISABLED and 1 for pullup ENABLED
*             : pulldown not yet supported
* Returns     : None
*********************************************************************/

void ICACHE_FLASH_ATTR pinMode(uint8_t pin, uint8_t mode, uint8_t pullup) {
    if ((0x1 << pin) & 0b110101) {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 0); //0,2,4,5
    } else {
        PIN_FUNC_SELECT(gpio_pin_register[pin], 3);
    }
    if(pullup)
      PIN_PULLUP_EN(gpio_pin_register[pin]);
    else
      PIN_PULLUP_DIS(gpio_pin_register[pin]);
    if (mode) {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TC_ADDRESS, 1<<pin); // GPIO input
    } else {
        GPIO_REG_WRITE(GPIO_ENABLE_W1TS_ADDRESS, 1<<pin); // GPIO output
    }
}

/********************************************************************
* FunctionName: digitalWrite
* Description : use to set up HIGH or LOW on a GPIO pin
* Parameters  : pin: pin mumber
*             : state HIGH or LOW
* Returns     : None
*********************************************************************/

void ICACHE_FLASH_ATTR digitalWrite(uint8_t pin, uint8_t state) {
    if (state) {
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1<<pin); // set GPIO pin high
    } else {
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1<<pin); // set GPIO pin low
    }
}
/********************************************************************
* FunctionName: digitalRead
* Description : use to set up HIGH or LOW on a GPIO pin
* Parameters  : pin: pin mumber
* Returns     : 1 if pin is HIGH or 0 if pin is LOW
*********************************************************************/

int ICACHE_FLASH_ATTR digitalRead(uint8_t pin) {
    return (GPIO_REG_READ(GPIO_OUT_ADDRESS)>>pin) & 1;
}
