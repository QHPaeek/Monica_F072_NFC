/*
 * mode_spice_api.c
 *
 *  Created on: Aug 23, 2025
 *      Author: Qinh
 */
#include <stdbool.h>
#include <stdint.h>
#include "Card_Reader.h"

extern uint8_t auth_flag;

char spice_api_send_buffer[78] = "{\"id\":1,\"module\":\"card\",\"function\":\"insert\",\"params\":[0,\"E00401AF87654321\"]}";//应为E00401开头
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
	mccInitialize();
	Card.mifare_auth_status = 0;
	if(auth_flag){
		if(mifareAuthenticate(MCC_AUTH_KEY_A, 0, Card.iso14443_uid4, 4, AimeKey) != RFAL_ERR_NONE){

		}else{
			memcpy(Card.mifare_right_key_a,AimeKey,6);
			Card.mifare_auth_status |= Auth_KeyA_Right;
		}
		if(mifareAuthenticate(MCC_AUTH_KEY_B, 0, Card.iso14443_uid4, 4, AimeKey) != RFAL_ERR_NONE){

		}else{
			memcpy(Card.mifare_right_key_b,AimeKey,6);
			Card.mifare_auth_status |= Auth_KeyB_Right;
		}
		auth_flag = 0;
	}else{
		if(mifareAuthenticate(MCC_AUTH_KEY_A, 0, Card.iso14443_uid4, 4, BanaKey_A) != RFAL_ERR_NONE){
			//platformLog("banakey a fail\r\n");
		}else{
			//platformLog("banakey a success\r\n");
			memcpy(Card.mifare_right_key_a,BanaKey_A,6);
			Card.mifare_auth_status |= Auth_KeyA_Right;
		}
		if(mifareAuthenticate(MCC_AUTH_KEY_B, 0, Card.iso14443_uid4, 4, BanaKey_B) != RFAL_ERR_NONE){
		}else{
			memcpy(Card.mifare_right_key_b,BanaKey_B,6);
			Card.mifare_auth_status |= Auth_KeyB_Right;
		}
		auth_flag = 1;
	}

	if(Card.mifare_auth_status == Auth_ALL_Right){
		for(uint8_t i = 0;i<4;i++){
		  spice_api_send_buffer[65+2*i] = hex2str(Card.iso14443_uid4[i] >> 4);
		  spice_api_send_buffer[66+2*i] = hex2str(Card.iso14443_uid4[i] & 0xF);
		}
		spice_api_send_buffer[54] = 48;
		//	  if(system_setting[0] & 0b1000000){//开启了2P刷卡
		//		buffer[54] = 49;
		//	  }
		Interface_Send(&spice_api_send_buffer,78);
	}
	mccDeinitialise(true);
}

void spice_request(){
	  static char spice_light_cmd_buffer[113] = "{\"id\":2,\"module\":\"lights\",\"function\":\"read\",\"params\":[\"IC Card Reader R\",\"IC Card Reader G\",\"IC Card Reader B\"]}";
	  spice_light_cmd_buffer[112] = 0x00;
	  Interface_Send(&spice_light_cmd_buffer,113);
}

uint8_t spice_request_check(uint8_t* data,uint8_t len){
	if((data[0] == '{') && (data[1] == '\"') && (data[2] == 'i') && (data[3] == 'd') && (data[4] == '\"') && (data[5] == ':')){
		if(len < 120){
			return 1;
		}else{
			uint8_t rgb[3] = {0,0,0};
			uint8_t sync_count = 0;
			for(uint8_t i = 0;i<len;i++){
				if(data[i] == ','){
					sync_count ++;
					if(sync_count == 3){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							rgb[0] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1 + (data[i+4] - 48) * 0.01);
						}else{
							rgb[0] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1);
						}

					}
					if(sync_count == 6){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							rgb[1] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1 + (data[i+4] - 48) * 0.01);
						}else{
							rgb[1] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1);
						}
					}
					if(sync_count == 9){
						if(data[i+4] >= 0x30 && data[i+4] <= 0x39){
							rgb[2] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1 + (data[i+4] - 48) * 0.01);
						}else{
							rgb[2] = 255 * ((data[i+1] - 48) + (data[i+3] - 48) * 0.1);
						}
					}
				}
			}
			LED_show(rgb[0],rgb[1],rgb[2]);
			spice_request();
			//spice_led_ready = 1;
			return 1;
		}
	}else{
		return 0;
	}
}
