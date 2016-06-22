#include "temperature.h"
#include "driver/ads1115.h"
#include "debug.h"
#include "osapi.h"
#include "util.h"

float ranges[TEMPERATURE_RANGE_SIZE][2] = {
    {2.356965701168474, -60},
     {5.227045705561331, -50},
     {10.99737318722662, -40},
     {22.082136625627353, -30},
     {42.545035190874295, -20},
     {78.92135825392377, -10},
     {141.1721622242647, 0},
     {243.310546875, 10},
     {409.2898346656977, 20},
     {522.7563234839108, 25},
     {663.2963400989321, 30},
     {1049.6697549080518, 40},
     {1614.6296229931195, 50},
     {2410.8853274828766, 60},
     {3519.892578125, 70},
     {4985.683538420681, 80},
     {6874.790191650391, 90},
     {9198.325552591465, 100},
     {11972.42373511905, 110},
     {15085.253906249998, 120},
     {16814.77346238057, 125},
     {18525.750411184214, 130},
     {22091.37601333682, 140},
     {25630.285763046115, 150},
     {29154.273148467695, 160},
     {35316.64794105351, 180},
     {40181.422124714605, 200},
     {43779.75843439055, 220},
     {46314.37602796053, 240},
     {48125.411240429305, 260},
     {49381.20900848765, 281},
     {50260.246236911, 300}
};


float ICACHE_FLASH_ATTR interpolate(float minAdc, float highAdc, int16_t minC, int16_t highC, uint16_t value)
{
    float ratio = (value - minAdc) / (highAdc - minAdc);
    
    return ((highC - minC) * ratio) + minC;
}

float ICACHE_FLASH_ATTR convert_to_farenheit(uint16_t adcValue)
{
    if(adcValue < ranges[0][0])
    {
        INFO("ADC READING NOT IN RANGE: TOO LOW\r\n");
        return 0;
    }
    if(adcValue > ranges[TEMPERATURE_RANGE_SIZE - 1][0])
    {
        INFO("ADC READING NOT IN RANGE: TOO HIGH\r\n");
        return 0;
    }
    uint8_t i = 0;
    
    for(i = 0; i < TEMPERATURE_RANGE_SIZE - 1; i++)
    {
        if(adcValue > ranges[i][0] && adcValue < ranges[i + 1][0]){
            return 32.00 + 1.8000 *interpolate(
                    ranges[i][0], 
                    ranges[i + 1][0],
                    ranges[i][1], 
                    ranges[i + 1][1],
                    adcValue);
        }
    }
    INFO("Ughh, shouldn't get here\r\n");
    return 0;
}


float ICACHE_FLASH_ATTR temperature_read(TempState state)
{
    int16_t result = 0;
    uint8_t i = 0;
    uint8_t count = 0;
    uint32_t total = 0;
    
    for(i = 0; i < TEMPERATURE_READ_TRIES; i++)
    {
        
        result = ads1115_read_analog(state.adc_input);
        if(result < 0) 
        {
            INFO("Bad ADS1115 read %d\r\n", result);
        }
        else
        {
            INFO("ADS1115 read %d\r\n", result);
            total += result;
            count++;
        }
    }
        
    uint16_t avg = total / count;
    
    return convert_to_farenheit(avg);
}
