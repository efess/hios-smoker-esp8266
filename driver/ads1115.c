#include "driver/ads1115.h"
#include "driver/i2c.h"
#include "debug.h"

uint16 _config;

uint8_t get_config_input()
{
    return (_config >> ADS1115_CONFIG_MPLEX) & 0x07;
}

void set_config_input(uint8_t ain)
{
    _config &= ~(0x07 << ADS1115_CONFIG_MPLEX);
    _config |= ((ain & 0x07) << ADS1115_CONFIG_MPLEX);
}

int8_t send_ptr_set(uint8_t register_ptr)
{
    uint8_t address = (ADS1115_ADDRESS << 1) | ADS1115_WRITE_BIT;
    
    i2c_start();
    i2c_writeByte(address);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -39;
    }
    
    i2c_writeByte(register_ptr);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -38;
    }
    
    i2c_stop();
    
    return 0;
}

int8_t send_config()
{
    uint8_t address = (ADS1115_ADDRESS << 1) | ADS1115_WRITE_BIT;
    
    i2c_start();

    // send address
    i2c_writeByte(address);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -37;
    }
    
    // config ptr
    i2c_writeByte(ADS1115_REGISTER_CONFIG); 
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -36;
    }
    
    // MSB config
    i2c_writeByte(_config >> 8);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -35;
    }
    
    // LSB config
    i2c_writeByte(_config & 0xFF);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -34;
    }
    
    i2c_stop();
    return 0;
}

int8_t set_analog_input(uint8_t ain)
{
    set_config_input(ain);
    INFO("config set: %d\r\n", _config);
    return send_config();
}

int8_t ads1115_init(void)
{
    _config = 0;
    
    // defaults
    _config = _config | (ADS1115_INPUT_0 << ADS1115_CONFIG_MPLEX);
    _config = _config | (0x02 << 9); // PGA: 4.096
    _config = _config | (0x00 << 8); // Continuous mode
    _config = _config | (0x07 << 5); // 0x04 is 128SPS, 0x07 is 860SPS
    _config = _config | (0x00 << 4); 
    
    _config = _config | 0x03; // disable comparator
                  
    return send_config();
}


int16_t ads1115_read_analog(uint8_t ain)
{
    if(get_config_input() != ain) 
    {
        if(set_analog_input(ain) < 0)
        {
            return -33;
        }
    }
    
    if(send_ptr_set(ADS1115_REGISTER_CONVERSION) < 0)
    {
        return -32;
    }
        
    os_delay_us(ADS1115_READ_DELAY); // wait min 1ms
    
    uint8_t address = (ADS1115_ADDRESS << 1) | ADS1115_READ_BIT;
    
    i2c_start();

    i2c_writeByte(address);
    if (!i2c_check_ack())
    {
        i2c_stop();
        return -31;
    }
    
    
    uint8_t msb = i2c_readByte();
    i2c_send_ack(1);
    uint8_t lsb = i2c_readByte();
    i2c_send_ack(1);
    i2c_stop();
    
    
    return (msb << 8) | lsb;
}
