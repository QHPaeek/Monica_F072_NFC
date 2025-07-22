/*
 * flash.h
 *
 *  Created on: Jul 18, 2025
 *      Author: Qinh
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

typedef union{
	uint64_t raw_flash[16];
	struct{
		uint8_t version;
		uint8_t gobal_LED_setting;
		//LED Brightness
		uint8_t sega_setting;
		//BIT0:tn32 emulator enable. Default:15693
		uint8_t spice_setting;
		//BIT0:2P mode enable. Default:1P
	};
}FlashData;
extern FlashData Flash;

void flash_write(uint64_t* data);
void flash_read(uint64_t* data);




#endif /* INC_FLASH_H_ */
