/*
 * mode_sega_serial.c
 *
 *  Created on: Jun 27, 2025
 *      Author: Qinh
 */
#include "mode_sega_serial.h"
#include "Card_Reader.h"
#include "rfal_utils.h"
#include "mcc.h"

packet_request_t req;
packet_response_t res;

uint8_t len, r, checksum;

uint8_t sega_write_buffer[128];
uint8_t sega_current_mifare_key_a[6];
uint8_t sega_current_mifare_key_b[6];
uint8_t sega_systemcode[2] = {0x88,0xb4};

//uint8_t sega_access_aime[4] = {0x70,0xf8,0x78,0x11};
extern uint8_t auth_flag;

void ascii_to_accesscode(uint8_t *ascii_data ,uint8_t *accesscode){
	accesscode[0] = 0x00;
	accesscode[1] = 0x00;
	for(uint8_t i = 0;i<8;i++){
		accesscode[i + 2] = (ascii_data[2 * i] - 0x30) << 4 | (ascii_data[2 * i + 1] - 0x30);
	}
}

void hex_to_accesscode(uint8_t *hex ,uint8_t *accesscode){
	accesscode[0] = 0x00;
	accesscode[1] = 0x00;
	memcpy(accesscode + 2, hex, 8);
	for(uint8_t i = 0;i<8;i++){
		uint8_t tmp_high = (hex[i] >> 4);
		uint8_t tmp_low = (hex[i] & 0b1111);
		if(tmp_high >= 10){
			tmp_high -= 7;
		}
		if(tmp_low >= 10){
			tmp_low -= 7;
		}
		accesscode[i+2] = tmp_high << 4 | tmp_low;
	}
}

uint8_t sega_packet_check(uint8_t* data,uint8_t len) {
	bool escape = false;
	uint8_t raw_pos = 0;
	uint8_t req_pos = 1;
	uint8_t checksum = 0;
	while(raw_pos<len){
		if(data[raw_pos] == SERIAL_CMD_START){
			req.frame_len = data[++raw_pos];
			checksum += req.frame_len;
			raw_pos++;
			break;
		}
		raw_pos++;
	}
	if(raw_pos == len){
		return 0;
	}
	while(req_pos != req.frame_len){
		if (data[raw_pos] == 0xD0) {
			escape = true;
			raw_pos++;
		}else if (escape) {
			req.bytes[req_pos] = data[raw_pos] + 1;
			checksum += req.bytes[req_pos];
			escape = false;
			raw_pos++;
			req_pos++;
		}else{
			req.bytes[req_pos] = data[raw_pos];
			checksum += req.bytes[req_pos];
			raw_pos++;
			req_pos++;
		}
		if(raw_pos == len){
			return 0;
		}
	}
	req.bytes[req_pos] = data[raw_pos];
	if (req.cmd == CMD_SEND_BINDATA_EXEC){
		return CMD_SEND_BINDATA_EXEC;
	}else if(checksum == req.bytes[req.frame_len]){
		return req.cmd;
	}else{
		return STATUS_SUM_ERROR;
	}
}

void sega_packet_write() {
  uint8_t checksum = 0, len = 0;
  if (res.cmd == 0) {
    return;
  }
  memset(sega_write_buffer,0,128);
  sega_write_buffer[0] = 0xE0;
  uint8_t current_pos = 0;
  while (len <= res.frame_len) {
    uint8_t w;
    if (len == res.frame_len) {
      w = checksum;
    } else {
      w = res.bytes[len];
      checksum += w;
    }
    if (w == 0xE0 || w == 0xD0) {
    	sega_write_buffer[++current_pos] = 0xD0;
    	sega_write_buffer[++current_pos] = --w;
    } else {
    	sega_write_buffer[++current_pos] = w;
    }
    len++;
  }
  res.cmd = 0;
  Interface_Send(sega_write_buffer ,++current_pos);
}

void res_clear(uint8_t payload_len) {
  res.frame_len = 6 + payload_len;
  res.addr = req.addr;
  res.seq_no = req.seq_no;
  res.cmd = req.cmd;
  res.status = STATUS_OK;
  res.payload_len = payload_len;
}

void sys_to_normal_mode() {
	res_clear(0);
	res.seq_no = 0;
}

void sys_get_fw_version() {
	if(Flash.sega_setting & 1){
		const char fw_version[24] = "TN32MSEC003S F/W Ver1.2";
		res_clear(sizeof(fw_version) - 1);
		memcpy(res.version, fw_version, res.payload_len);
	}
	else{
		const char fw_version[2] = "\x94";
		//char fw_version[2] = "\x92";
		res_clear(sizeof(fw_version) - 1);
		memcpy(res.version, fw_version, res.payload_len);
	}
}

void sys_get_hw_version() {
	if(Flash.sega_setting & 1){
		const char hw_version[24] = "TN32MSEC003S H/W Ver3.0";
		res_clear(sizeof(hw_version) - 1);
		memcpy(res.version, hw_version, res.payload_len);
	}
	else{
		const char hw_version[10] = "837-15396";
		//char hw_version[10] = "837-15286";
		res_clear(sizeof(hw_version) - 1);
		memcpy(res.version, hw_version, res.payload_len);
	}
}

void sys_get_led_info() {
	if(Flash.sega_setting & 1){
		const char led_info[10] = "15084\xFF\x10\x00\x12";
		res_clear(sizeof(led_info) - 1);
		memcpy(res.version, led_info, res.payload_len);
	}
	else{
		const char led_info[13] = "000-00000\xFF\x11\x40";
		res_clear(sizeof(led_info) - 1);
		memcpy(res.version, led_info, res.payload_len);
	}
}


void nfc_start_polling() {
  res_clear(0);
}

void nfc_stop_polling() {
  res_clear(0);
}

void nfc_card_detect() {
	if(Card.operation != Operation_idle){
		switch(Card.type){
			case Card_None:
				break;
			case Card_Type_Mifare_Classic:
				if(Card.mifare_auth_status == 0){
					//pre-read still going...
					break;
				}
				memcpy(res.mifare_uid,Card.iso14443_uid4,4);
			    res_clear(0x07);
			    res.id_len = 4;
			    res.count = 1;
			    res.type = 0x10;
			    return;
			case Card_Type_Mifare_UltraLight:
				if(Flash.sega_setting && SYSTEM_NESICA_SUPPORT){
					memcpy(res.mifare_uid,Card.iso14443_uid7,4);
				    res_clear(0x07);
				    res.id_len = 4;
				    res.count = 1;
				    res.type = 0x10;
				}
				return;
			case Card_Type_Felica:
				memcpy(res.IDm, Card.felica_IDm,8);
				memcpy(res.PMm, Card.felica_PMm,8);
		        res_clear(0x13);
		        res.count = 1;
		        res.type = 0x20;
		        res.id_len = 0x10;
		        return;
			case Card_Type_ISO15693:
				if(Flash.sega_setting && SYSTEM_EPASS_SUPPORT){
					memcpy(res.mifare_uid,Card.iso15693_uid,4);
				    res_clear(0x07);
				    res.id_len = 4;
				    res.count = 1;
				    res.type = 0x10;
				}
				return;
			case Card_Type_ISO14443A_Unknow:
				break;
		}
	}
	res_clear(1);
	res.count = 0;
}

void nfc_mifare_authorize_a() {
	res_clear(0);
	if((Card.type == Card_Type_Mifare_UltraLight) && (Flash.sega_setting && SYSTEM_NESICA_SUPPORT) && !memcmp(sega_current_mifare_key_a,AimeKey,6)){
		return;
	}
	if((Card.type == Card_Type_ISO15693) && (Flash.sega_setting && SYSTEM_EPASS_SUPPORT) && !memcmp(sega_current_mifare_key_a,AimeKey,6)){
		return;
	}
	if((Card.type != Card_Type_Mifare_Classic) || !(Card.mifare_auth_status & Auth_KeyA_Right)){
		res.status = STATUS_CARD_ERROR;
		return;
	}
//	if(mifareAuthenticate(MCC_AUTH_KEY_A, 0, Card.iso14443_uid4, 4, sega_mifare_key_a) != RFAL_ERR_NONE){
	if(memcmp(sega_current_mifare_key_a,Card.mifare_right_key_a,6)){
		res.status = STATUS_CARD_ERROR;
	}
}

void nfc_mifare_authorize_b() {
	res_clear(0);
	if((Card.type == Card_Type_Mifare_UltraLight) && (Flash.sega_setting && SYSTEM_NESICA_SUPPORT) && !memcmp(sega_current_mifare_key_b,AimeKey,6)){
		return;
	}
	if((Card.type == Card_Type_ISO15693) && (Flash.sega_setting && SYSTEM_EPASS_SUPPORT) && !memcmp(sega_current_mifare_key_b,AimeKey,6)){
		return;
	}
	if((Card.type != Card_Type_Mifare_Classic) || !(Card.mifare_auth_status & Auth_KeyB_Right)){
		res.status = STATUS_CARD_ERROR;
		return;
	}
//	if(mifareAuthenticate(MCC_AUTH_KEY_B, 0, Card.iso14443_uid4, 4, sega_mifare_key_b) != RFAL_ERR_NONE){
	if(memcmp(sega_current_mifare_key_b,Card.mifare_right_key_b,6)){
		res.status = STATUS_CARD_ERROR;
	}
}

void nfc_mifare_read() {
	res_clear(0x10);
	if((Card.type == Card_Type_Mifare_UltraLight) && (Flash.sega_setting && SYSTEM_NESICA_SUPPORT)){
		if(req.block_no == 0){
			memcpy(res.block,Card.iso14443_uid7,4);
		}
		else if(req.block_no == 2){
			ascii_to_accesscode(Card.nesica_serial ,res.block + 6);
		}else{
			memset(res.block, 0, 16);
		}
	}else if((Card.type == Card_Type_ISO15693) && (Flash.sega_setting && SYSTEM_EPASS_SUPPORT)){
		if(req.block_no == 0){
			memcpy(res.block,Card.iso15693_uid,4);
		}
		if(req.block_no == 2){
			//memcpy(res.block + 13,Card.iso14443_uid7,7);
			hex_to_accesscode(Card.iso15693_uid ,res.block+6);
		}
	}else if((Card.type != Card_Type_Mifare_Classic)){
	  res.status = STATUS_CARD_ERROR;
	  return;
	}else{
		memcpy(res.block,Card.mifare_data[req.block_no],16);
	}
//	if(mifareReadBlock(0, req.block_no, res.block, 16) != RFAL_ERR_NONE){
//		res_clear(0);
//		res.status = STATUS_CARD_ERROR;
//	}
}

void nfc_felica_through() {
	if((Card.operation != Operation_busy) && (Card.type != Card_Type_Felica)){
		res_clear(0);
		res.status = STATUS_CARD_ERROR;
		return;
	}
	uint8_t code = req.encap_code;
	res.encap_code = code + 1;
	switch (code) {
		case FelicaPolling:
		{
			res_clear(0x14);
			memcpy(res.poll_systemCode,sega_systemcode,2);
		}
		break;
		case FelicaReqSysCode:
		{
			res_clear(0x0D);
			res.felica_payload[0] = 0x01;
			memcpy(&res.felica_payload[1],sega_systemcode,2);
		}
		break;
		case FelicaActive2:
		{
			res_clear(0x0B);
			res.felica_payload[0] = 0x00;
		}
		break;
		case FelicaReadWithoutEncryptData:
		{
//			uint16_t serviceCodeList = req.serviceCodeList[1] << 8 | req.serviceCodeList[0];
//			uint16_t blockList[4];
//			for (uint8_t i = 0; i < req.numBlock; i++) {
//			  blockList[i] = (uint16_t)(req.blockList[i][0] << 8 | req.blockList[i][1]);
//			}
//			if(nfcfReadBlock(Card.felica_IDm, 1, &serviceCodeList , req.numBlock, blockList, res.blockData) != RFAL_ERR_NONE){
//				LED_show(0,255,255);
//				res.status = STATUS_CARD_ERROR;
//			}else{
			if((req.blockList[0][0] == 0x80) && (req.blockList[0][1] == 0x82)){
				memcpy(res.blockData[0],Card.felica_IDm,8);
			}
				res.RW_status[0] = 0;
				res.RW_status[1] = 0;
				res.numBlock = req.numBlock;
				res_clear(0x0D + req.numBlock * 16);
//			}
		}
		break;
		case FelicaWriteWithoutEncryptData:
		{
//			int8_t result = 0;
//			uint16_t serviceCodeList = req.serviceCodeList[1] << 8 | req.serviceCodeList[0];
//			uint16_t blockList = (uint16_t)(req.blockList_write[0][0] << 8 | req.blockList_write[0][1]);
//			if(nfcfWriteSingleBlock(Card.felica_IDm, 1, &serviceCodeList , &blockList, req.blockData) != RFAL_ERR_NONE){
//				LED_show(255,255,0);
//				res.status = STATUS_CARD_ERROR;
//			}else{
				res_clear(0x0C);
				res.RW_status[0] = 0;
				res.RW_status[1] = 0;
//			}
		}
		break;
		default:
			res_clear(0);
			res.status = STATUS_INVALID_COMMAND;
  }
  res.encap_len = res.payload_len;
}

void Sega_Mode_Loop(uint8_t cmd){
  switch (cmd) {
      case 0:
        break;
      case CMD_TO_NORMAL_MODE:
        sys_to_normal_mode();
        break;
      case CMD_TO_UPDATER_MODE:
        res_clear(0);
        res.status = STATUS_OK;
        break;
      case CMD_SEND_BINDATA_EXEC:
        res_clear(0);
        res.status = STATUS_FIRM_UPDATE_SUCCESS;
      case CMD_GET_FW_VERSION:
        sys_get_fw_version();
        break;
      case CMD_GET_HW_VERSION:
        sys_get_hw_version();
        break;
      case CMD_SEND_HEX_DATA:
        res_clear(0);
        res.status = STATUS_COMP_DUMMY_3RD;
        break;

    // Card read
      case CMD_START_POLLING:
    	Card.operation = Operation_idle;
        nfc_start_polling();
        break;
      case CMD_STOP_POLLING:
        nfc_stop_polling();
        break;
      case CMD_CARD_DETECT:
        nfc_card_detect();
        break;

    // MIFARE
      case CMD_MIFARE_KEY_SET_A:
        memcpy(sega_current_mifare_key_a, req.key, 6);
        res_clear(0);
        break;

      case CMD_MIFARE_KEY_SET_B:
        memcpy(sega_current_mifare_key_b, req.key, 6);
        res_clear(0);
        break;

      case CMD_MIFARE_AUTHORIZE_A:
        nfc_mifare_authorize_a();
        break;

      case CMD_MIFARE_AUTHORIZE_B:
        nfc_mifare_authorize_b();
        break;

      case CMD_MIFARE_READ:
        nfc_mifare_read();
        break;

    // FeliCa
      case CMD_FELICA_THROUGH:
        nfc_felica_through();
        break;

    // LED
      case CMD_EXT_BOARD_LED_RGB:
        LED_show(req.color_payload[0] , req.color_payload[1] , req.color_payload[2]);
    	  res_clear(0);
        break;

      case CMD_EXT_BOARD_INFO:
        sys_get_led_info();
        break;

      case CMD_EXT_BOARD_LED_RGB_UNKNOWN:
    	  res_clear(0);
        break;

      case CMD_CARD_SELECT:
          res_clear(0);
          break;
      case CMD_CARD_HALT:
          res_clear(0);
          break;
      case CMD_EXT_TO_NORMAL_MODE:
        res_clear(0);
        break;

      case STATUS_SUM_ERROR:
        res_clear(0);
        res.status = STATUS_SUM_ERROR;
        break;

//      case CMD_READ_EEPROM:
//        res_init();
//        res.payload_len = 4;
//        res.frame_len = 10;
//       for(uint8_t i = 0;i<3;i++){
//          res.eeprom_data[i] = system_setting[i];
//        }
//        res.board_vision = BOARD_VISION;
//        break;
//
//     case CMD_WRITE_EEPROM:
//        system_setting[0] = req.eeprom_data[0];
//        system_setting[1] = req.eeprom_data[1];
//        for(uint8_t i = 0;i<8;i++)
//        {
//          mapped_card_IDm[i] = req.mapped_IDm[i];
//        }
//        for(uint8_t i = 0;i<10;i++)
//        {
//          card_reflect.block2[i+6] = req.target_accesscode[i];
//        }
//        EEPROM.write(0, system_setting[0]);
//        EEPROM.write(1, system_setting[1]);
//        for(uint8_t j = 0;j<8;j++)
//        {
//          EEPROM.write(j+4, req.mapped_IDm[j]);
//        }
//        for(uint8_t k = 0;k<10;k++)
//        {
//          EEPROM.write(k+12, req.target_accesscode[k]);
//        }
//        SerialDevice.begin((system_setting[0] & 0b10)? 115200 : 38400);
//        if (system_setting[0] & 0b10){
//         LED_show(0,0,255);
//         }
//        else{
//          LED_show(0,255,0);
//        }
//        delay(1);
//        res_init();
//        break;

      default:
        res_clear(0);
        res.status = STATUS_OK;
        break;
  }
  sega_packet_write();
}


