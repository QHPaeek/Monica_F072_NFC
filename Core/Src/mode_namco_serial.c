/*
 * mode_namco_serial.c
 *
 *  Created on: Aug 25, 2025
 *      Author: Qinh
 */
#include <stdbool.h>
#include <stdint.h>
#include "Card_Reader.h"

uint8_t *namco_cmd_data = Card.operation_tmp;
uint8_t namco_led_mode = 0;

enum{
	NAMCO_CMD_READ_REGISTER = 0x06,
	NAMCO_CMD_WRITE_REGISTER = 0x08,
	NAMCO_CMD_READ_GPIO = 0x0C,
	NAMCO_CMD_BEEP_AND_LED = 0x0E,
	NAMCO_CMD_SET_PARAMETER = 0x12,
	NAMCO_CMD_UNKNOW = 0x18,
	NAMCO_CMD_RFCONFIG = 0x32,
	NAMCO_CMD_INDESELECT = 0x44,
	NAMCO_CMD_INRELEASE = 0x52,
	NAMCO_CMD_READ_FELICA = 0xA0,
};

enum{
	PN532_CMD_POLLING = 0x4A,
    PN532_CMD_INDATAEXCHANGE = 0x40,
};

enum{
	MIFARE_CMD_AUTHORIZE_A = 0x60,
	MIFARE_CMD_AUTHORIZE_B = 0x61,
	MIFARE_CMD_READ = 0x30,
	MIFARE_CMD_WRITE = 0xA0,

};

uint8_t namco_packet_check(uint8_t* data,uint8_t len){
	if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0xff){
		if(data[3] > len + 7){
			//packet incomplete
			return 0;
		}
		if(data[3] + data[4] != 0){
			//packet length error
			return 0;
		}
		memcpy(namco_cmd_data,data,len);
		return data[6];
	}else{
		//packet head error
		return 0;
	}
}

void namco_packet_process(uint8_t cmd){
	switch(cmd){
		case PN532_CMD_POLLING:{
			if(namco_cmd_data[8] == 0x00){
				//PN532 fine ISO14443 card
				if(Card.type == Card_Type_Mifare_Classic){
					const uint8_t tmp[] =  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xff, 0x0c, 0xf4, 0xd5, 0x4b, 0x01, 0x01, 0x00, 0x04, 0x08, 0x04,
											0xaa, 0xaa, 0xaa, 0xaa, 0xbb, 0x00};
					//aa:uid bb:checksum
					memcpy(namco_cmd_data,tmp,sizeof(tmp));
					memcpy(namco_cmd_data + 13 + 6,Card.iso14443_uid4,4);
					uint8_t checksum = 0;
					for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
						checksum += namco_cmd_data[i + 5 + 6];
					}
					checksum = 0xff - checksum;
					checksum ++;
					namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
					Interface_Send(namco_cmd_data,sizeof(namco_cmd_data));
				}
			}else if(namco_cmd_data[8] == 0x01){
				//PN532 fine felica card
				if(Card.type == Card_Type_Felica_AIC){
					const uint8_t tmp[] =  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xff, 0x0c, 0xf4, 0xd5, 0x4b, 0x01, 0x01, 0x12, 0x01,
											0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xbb, 0xcc, 0x00};
					//aa:idm bb:pmm cc:checksum
					memcpy(namco_cmd_data,tmp,sizeof(tmp));
					memcpy(namco_cmd_data + 17 + 6,Card.felica_IDm,8);
					memcpy(namco_cmd_data + 15 + 6,Card.felica_PMm,8);
					uint8_t checksum = 0;
					for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
						checksum += namco_cmd_data[i + 5 + 6];
					}
					checksum = 0xff - checksum;
					checksum ++;
					namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
					Interface_Send(namco_cmd_data,sizeof(namco_cmd_data));
				}
			}

			break;
		}
		case PN532_CMD_INDATAEXCHANGE:{
			switch(namco_cmd_data[8]){
				case MIFARE_CMD_AUTHORIZE_A:{
					const uint8_t tmp[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd5, 0x41, 0x00, 0xbb, 0x00};
					if((namco_cmd_data[9 + 6] != 0) && (memcmp(Card.iso14443_uid4,&namco_cmd_data[16 + 6],4)) && !(Card.mifare_auth_status & Auth_KeyA_Right) &&(memcmp(Card.mifare_right_key_a,&namco_cmd_data[10 + 6],6))){
						//can only auth block0		;different uid error									;auth failed										;key error
						memcpy(namco_cmd_data,tmp,sizeof(tmp));
						namco_cmd_data[7 +6] = 0x14;
						return;
					}else{
						//auth success,true key
						memcpy(namco_cmd_data,tmp,sizeof(tmp));
						namco_cmd_data[7 + 6] = 0x00;
					}
					uint8_t checksum = 0;
					for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
						checksum += namco_cmd_data[i + 5 + 6];
					}
					checksum = 0xff - checksum;
					checksum ++;
					namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
					Interface_Send(namco_cmd_data,sizeof(tmp));
				}
				case MIFARE_CMD_AUTHORIZE_B:{
					const uint8_t tmp[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd5, 0x41, 0x00, 0xbb, 0x00};
					if((namco_cmd_data[9 + 6] != 0) && (memcmp(Card.iso14443_uid4,&namco_cmd_data[16 + 6],4)) && !(Card.mifare_auth_status & Auth_KeyB_Right) &&(memcmp(Card.mifare_right_key_b,&namco_cmd_data[10 + 6],6))){
						//can only auth block0		;different uid error									;auth failed										;key error
						memcpy(namco_cmd_data,tmp,sizeof(tmp));
						namco_cmd_data[7 +6] = 0x14;
						return;
					}else{
						//auth success,true key
						memcpy(namco_cmd_data,tmp,sizeof(tmp));
						namco_cmd_data[7 + 6] = 0x00;
					}
					uint8_t checksum = 0;
					for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
						checksum += namco_cmd_data[i + 5 + 6];
					}
					checksum = 0xff - checksum;
					checksum ++;
					namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
					Interface_Send(namco_cmd_data,sizeof(tmp));
				}
				case MIFARE_CMD_READ:{
					uint8_t block = namco_cmd_data[8];
					const uint8_t tmp[] =  {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x13, 0xED, 0xD5, 0x41, 0x00,
											0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xbb, 0x00};
					memcpy(namco_cmd_data,tmp,sizeof(tmp));
					if(Card.mifare_auth_status == Auth_ALL_Failed){
						//failed
						namco_cmd_data[7 + 6] = 0x01;
					}else{
						//success
						memcpy(namco_cmd_data +8 + 6,Card.mifare_data[block],16);
					}
					uint8_t checksum = 0;
					for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
						checksum += namco_cmd_data[i + 5 + 6];
					}
					checksum = 0xff - checksum;
					checksum ++;
					namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
					Interface_Send(namco_cmd_data,sizeof(tmp
							));
				}
			}

		}
		case NAMCO_CMD_BEEP_AND_LED:{
			if(namco_cmd_data[7] == 0x01){
				namco_led_mode = namco_cmd_data[8];
			}
			const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x0F, 0x1C, 0x00};
			Interface_Send(tempBuf,sizeof(tempBuf));
			break;
		}
		case NAMCO_CMD_UNKNOW:{
			if(namco_cmd_data[7] == 0x01){
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x19, 0x12, 0x00};
				Interface_Send(tempBuf,sizeof(tempBuf));
			}
			break;
		}
		case NAMCO_CMD_RFCONFIG:{
			if(namco_cmd_data[7] == 0x81){
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x33, 0xF8, 0x00};
				Interface_Send(tempBuf,sizeof(tempBuf));
			}
			break;
		}
		case NAMCO_CMD_READ_REGISTER:{
			if(namco_cmd_data[3] == 0x12){
				//BngRwSpecialRegisterLen 0x12
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0A, 0xF6, 0xD5, 0x07, 0xFF, 0x3F, 0x0E, 0xF1, 0xFF, 0x3F, 0x0E, 0xF1, 0xAA, 0x00};
				Interface_Send(tempBuf,sizeof(tempBuf));
			}else if(namco_cmd_data[3] == 0x18){
				//BngRwSpecialRegisterLen0x18
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0D, 0xF3, 0xD5, 0x07, 0xDC, 0xF4, 0x3F, 0x11, 0x4D, 0x85, 0x61, 0xF1, 0x26, 0x6A, 0x87, 0xC9, 0x00};
				Interface_Send(tempBuf,sizeof(tempBuf));
			}
			break;
		}
		case NAMCO_CMD_READ_GPIO:{
			const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x05, 0xFB, 0xD5, 0x0D, 0x20, 0x06, 0x00, 0xF8, 0x00};
			Interface_Send(tempBuf,sizeof(tempBuf));
			break;
		}
		case NAMCO_CMD_SET_PARAMETER:{
			const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD5, 0x13, 0x18, 0x00};
			Interface_Send(tempBuf,sizeof(tempBuf));
			break;
		}
		case NAMCO_CMD_WRITE_REGISTER:{
			if(namco_cmd_data[7] == 0xFF){
				//Syntax err: Unknown SFR address
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD5, 0x09, 0x00, 0x22, 0x00 };
				Interface_Send(tempBuf,sizeof(tempBuf));
			}else if(namco_cmd_data[7] == 0x63){
				//clear cardreading service
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD5, 0x09, 0x00, 0x22, 0x00 };
				Interface_Send(tempBuf,sizeof(tempBuf));
			}
			break;
		}
		case NAMCO_CMD_INRELEASE:{
			//success
		    const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD5, 0x53, 0x01, 0x00, 0xD7, 0x00};
		    //failed
		    //uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD5, 0x53, 0x01, 0x27, 0xB0, 0x00};
		    Interface_Send(tempBuf,sizeof(tempBuf));
			break;
		}
		case NAMCO_CMD_INDESELECT:{
			//success
			const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD5, 0x45, 0x01, 0x00, 0xE5, 0x00};
			//failed
			//const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD5, 0x45, 0x01, 0x27, 0xBF, 0x00};
			Interface_Send(tempBuf,sizeof(tempBuf));
			break;
		}
		case NAMCO_CMD_READ_FELICA:{
			if(Card.type != Card_Type_Felica_AIC){
				//card lost
				const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFD, 0xD5, 0xA1, 0x01, 0x89, 0x00};
				Interface_Send(tempBuf,sizeof(tempBuf));
			}
			break;
		}
		default:{
			//Unknown Command,try general reply
			const uint8_t tempBuf[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD5, 0x45, 0x01, 0x00, 0xE5, 0x00};
			uint8_t cc = namco_cmd_data[6] +1;
			memcpy(namco_cmd_data,tempBuf,sizeof(tempBuf));
			namco_cmd_data[6 + 6] = cc;
			uint8_t checksum = 0;
			for(uint8_t i = 0;i< namco_cmd_data[3 + 6];i++){
				checksum += namco_cmd_data[i + 5 + 6];
			}
			checksum = 0xff - checksum;
			checksum ++;
			namco_cmd_data[sizeof(namco_cmd_data) - 2] = checksum;
			Interface_Send(namco_cmd_data,sizeof(tempBuf));
		}
	}
}
