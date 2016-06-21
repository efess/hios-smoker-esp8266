/* config.h
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

#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_
#include "os_type.h"
#include "user_config.h"

#define DEVICE_TYPE	"undercabinet"

//(1) isoccupied
//(4) occupied brightness
//(3) occupied program
//(2) occupied transition
//(4) unoccupied brightness
//(3) unoccupied program
//(2) unoccupied transition
//(1) isroombright

#define OPTIONS_SHIFT_ROOM_ISOCCUPIED 0
#define OPTIONS_SHIFT_ROOM_ISBRIGHT 19

#define OPTIONS_BITMASK_OCCUPIED_PROGRAM 0x0007
#define OPTIONS_BITMASK_OCCUPIED_TRANSITION 0x0024

#define OPTIONS_BITMASK_UNOCCUPIED_PROGRAM 0x0700
#define OPTIONS_BITMASK_LIGHT_PROGRAM 0x2400
#define OPTIONS_BITMASK_UNCOND_ON 0x08
#define OPTIONS_BITMASK_MOTION_ON 0x16
#define OPTIONS_BITMASK_LIGHT_SENSE_ON 0x32

#define DEVICE_CONFIG_MAX 318

typedef struct{
	uint32_t cfg_holder;
	uint8_t device_id[16];

	uint8_t sta_ssid[64];
	uint8_t sta_pwd[64];
	uint32_t sta_type;

	uint8_t mqtt_host[64];
    uint32_t mqtt_port;

    // LIGHT setup
    uint8_t device_config[DEVICE_CONFIG_MAX];
    uint16_t device_config_length;

//    uint32_t light_color;
//    uint32_t light_options;
//    uint8_t light_pallete_size;
//    uint32_t light_pallete[16];

} SYSCFG;

typedef struct {
    uint8 flag;
    uint8 pad[3];
} SAVE_FLAG;

void ICACHE_FLASH_ATTR CFG_Save();
void ICACHE_FLASH_ATTR CFG_Load();

extern SYSCFG sysCfg;

#endif /* USER_CONFIG_H_ */
