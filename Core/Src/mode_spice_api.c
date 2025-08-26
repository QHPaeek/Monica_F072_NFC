/*
 * mode_spice_api.c
 *
 *  Created on: Aug 23, 2025
 *      Author: Qinh
 */
#include <stdbool.h>
#include <stdint.h>
#include "Card_Reader.h"
#include "Flash.h"
#include "LED.h"
#include "mode_manager.h"

extern uint8_t auth_flag;

uint8_t spice_api_send_buffer[79];
const char spice_insert_cmd[78] = "{\"id\":1,\"module\":\"card\",\"function\":\"insert\",\"params\":[0,\"E00401AF87654321\"]}";//应为E00401开头
const char spice_light_cmd[113] = "{\"id\":2,\"module\":\"lights\",\"function\":\"read\",\"params\":[\"IC Card Reader R\",\"IC Card Reader G\",\"IC Card Reader B\"]}";
uint8_t spice_led_ready = 0;


char hex2str(uint8_t hex){
  //注意这里传输的必须是半字节
  if(hex < 0xA){
    return (hex + 48);//ascii 0 = 48
  }
  else{
    return (hex - 0xA + 65);//ascii A = 65
  }
}

void spice_mifare_process(){
	memcpy(spice_api_send_buffer,spice_insert_cmd,78);
	if((memcmp(Card.mifare_right_key_a,AimeKey,6) == 0) && (Flash.spice_setting & SYSTEM_AIME_SUPPORT)){
		//aime
	}else if((memcmp(Card.mifare_right_key_a,BanaKey_A,6) == 0) && (Flash.spice_setting & SYSTEM_BANA_SUPPORT)){
		//bana
	}else{
		return;
	}
	if(Card.mifare_auth_status == Auth_ALL_Right){
		for(uint8_t i = 0;i<4;i++){
		  spice_api_send_buffer[65+2*i] = hex2str(Card.iso14443_uid4[i] >> 4);
		  spice_api_send_buffer[66+2*i] = hex2str(Card.iso14443_uid4[i] & 0xF);
		}
		spice_api_send_buffer[54] = 48;
		if(Flash.spice_setting & SYSTEM_MODE_SEETING){//开启了2P刷卡
			spice_api_send_buffer[54] = 49;//"params\":[1,......
		}
		Interface_Send(spice_api_send_buffer,79);
	}
}

void spice_felice_process(){
	memcpy(spice_api_send_buffer,spice_insert_cmd,78);
    for(uint8_t i = 0;i<8;i++){
    	spice_api_send_buffer[57+2*i] = hex2str(Card.felica_IDm[i] >> 4);//高4位转换为字符
    	spice_api_send_buffer[58+2*i] = hex2str(Card.felica_IDm[i] & 0xF);//低4位转换为字符
    }
    if(Flash.spice_setting & SYSTEM_MODE_SEETING){//开启了2P刷卡
		spice_api_send_buffer[54] = 49;//"params\":[1,......
	}
    spice_api_send_buffer[54] = 48;
    Interface_Send(spice_api_send_buffer,79);
}

void spice_iso15693_process(){
	memcpy(spice_api_send_buffer,spice_insert_cmd,78);
    for(uint8_t i = 0;i<8;i++){
    	spice_api_send_buffer[57+2*i] = hex2str(Card.iso15693_uid[i] >> 4);//高4位转换为字符
    	spice_api_send_buffer[58+2*i] = hex2str(Card.iso15693_uid[i] & 0xF);//低4位转换为字符
    }
    if(Flash.spice_setting & SYSTEM_MODE_SEETING){//开启了2P刷卡
		spice_api_send_buffer[54] = 49;//"params\":[1,......
	}
    spice_api_send_buffer[54] = 48;
    Interface_Send(spice_api_send_buffer,79);
}

void spice_mifare_ul_process(){
	memcpy(spice_api_send_buffer,spice_insert_cmd,78);
	uint8_t ret = 0;
	for(uint8_t i = 0;i<7;i++){
		if(Card.iso14443_uid7[1] == 0){
			ret++;
		}
	}
	if(ret > 5){
		return;
	}
    for(uint8_t i = 0;i<7;i++){
    	spice_api_send_buffer[59+2*i] = hex2str(Card.iso14443_uid7[i] >> 4);//高4位转换为字符
    	spice_api_send_buffer[60+2*i] = hex2str(Card.iso14443_uid7[i] & 0xF);//低4位转换为字符
    }
    if(Flash.spice_setting & SYSTEM_MODE_SEETING){//开启了2P刷卡
    	spice_api_send_buffer[54] = 49;//"params\":[1,......
    }
    spice_api_send_buffer[54] = 48;
    Interface_Send(spice_api_send_buffer,79);
}

void spice_request(){
	  Interface_Send(spice_light_cmd,113);
}

uint8_t spice_request_check(uint8_t* data,uint8_t len){
	if((data[0] == '{') && (data[1] == '\"') && (data[2] == 'i') && (data[3] == 'd') && (data[4] == '\"') && (data[5] == ':')){
		if(len < 120){
			return 1;
		}else{
			uint8_t spice_api_target_rgb[3] = {0,0,0};
			uint8_t sync_count = 0;
			for(uint8_t i = 0;i<len;i++){
				if(data[i] == ','){
					sync_count ++;
					if(sync_count == 3){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							spice_api_target_rgb[0] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10 + (data[i+4] - 48) * 1);
						}else{
							spice_api_target_rgb[0] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10);
						}
					}
					if(sync_count == 6){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							spice_api_target_rgb[1] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10 + (data[i+4] - 48) * 1);
						}else{
							spice_api_target_rgb[1] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10);
						}
					}
					if(sync_count == 9){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							spice_api_target_rgb[2] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10 + (data[i+4] - 48) * 1);
						}else{
							spice_api_target_rgb[2] = 2.5 * ((data[i+1] - 48) * 100 + (data[i+3] - 48) * 10);
						}
					}
				}
			}
			LED_show(spice_api_target_rgb[0],spice_api_target_rgb[1],spice_api_target_rgb[2]);
			return 1;
		}
	}else{
		return 0;
	}
}
