/* main.c -- MQTT client example
*
* Copyright (c) 2014-2015, Tuan PM <tuanpm at live dot com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
#include "ets_sys.h"
#include "driver/uart.h"
#include "osapi.h"
#include "mqttClient.h"
#include "wifi.h"
#include "config.h"
#include "debug.h"
#include "gpio.h"
#include "user_interface.h" 
#include "running_state.h"
#include "driver/i2c_hd44780.h"
#include "driver/ads1115.h"
#include "fan.h"

//#include "../gdbstub/gdbstub.h"

#include "mem.h"
#define TOPIC_SUB_OPTIONS_CONFIG_UPDATE "/home/outside/smoker/stoker/config/update"

#define TOPIC_SUB_OPTIONS_REQUEST "/home/outside/smoker/stoker/request"

#define TOPIC_PUB_OPTIONS_RESPONSE "/home/outside/smoker/stoker/response"

MQTT_Client *mqttClient;

void wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
        MQTT_Connect(mqttClient);
	} else {
        MQTT_Disconnect(mqttClient);
    }
}

void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
    MQTT_Subscribe(client, TOPIC_SUB_OPTIONS_REQUEST, 0);
}

void mqttDisconnectedCb(uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
    MQTT_Client* client = (MQTT_Client*)args;
    INFO("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
    //  /home/kitchen/cabinet/lights
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

    //INFO("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);

    char status[50];
    os_sprintf(status, "type: %s, device id: %s", DEVICE_TYPE, sysCfg.device_id);
    MQTT_Publish(client, "/home/query_resp/sensors", status, strlen(status), 0, 0);

    INFO("Received %s\r\n", dataBuf);
    INFO("topic %s\r\n", topicBuf);
//    if(strcmp(TOPIC_SUB_OPTIONS_UPDATE, topicBuf) == 0)
//    {
//        handle_mqtt_config_update(dataBuf);
//    }
//    else 
    if(strcmp(TOPIC_SUB_OPTIONS_REQUEST, topicBuf) == 0)
    {
        char* response;

        handle_mqtt_request(&response);

        INFO(response);

        MQTT_Publish(client, TOPIC_PUB_OPTIONS_RESPONSE, response, strlen(response), 0, 0);
        os_free(response);
    }

	os_free(topicBuf);
    os_free(dataBuf);
}

void user_init(void)
{
    //system_update_cpu_freq(160) ;
    mqttClient = (MQTT_Client*)os_zalloc(sizeof(MQTT_Client));

    //gdbstub_init();
    uart_init(BIT_RATE_115200, BIT_RATE_115200);

    os_delay_us(1000000);

    // this is to setup UART1 TX on GPIO2
    //PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
    //os_install_putc1((void *)uart0_write_char);


	CFG_Load();

    INFO("Init MQTT ... \r\n");
    MQTT_InitConnection(mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, 0);

    MQTT_InitClient(mqttClient, sysCfg.device_id, "", "", 120, 1);

    MQTT_InitLWT(mqttClient, "/lwt", "offline", 0, 0);
    MQTT_OnConnected(mqttClient, mqttConnectedCb);
    MQTT_OnDisconnected(mqttClient, mqttDisconnectedCb);
    MQTT_OnPublished(mqttClient, mqttPublishedCb);
    MQTT_OnData(mqttClient, mqttDataCb);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
    
    LCD_init();
    LCD_setCursor(0, 1);
    LCD_print("   Smoker Stoker!");
    LCD_setCursor(0, 2);
    LCD_print("   Starting...  ");
    
    if(ads1115_init() < 0)
    {
        INFO("ADC failed to initialize!\r\n");
    } 
    else 
    {
        INFO("\r\nADC init ...\r\n");
    }
    
    state_init();
    sensors_init();
    fan_init();

	INFO("\r\nSystem started ...\r\n");
}
