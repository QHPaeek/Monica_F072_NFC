/*
 * mode_manager.h
 *
 *  Created on: Jun 27, 2025
 *      Author: Qinh
 */

#ifndef INC_MODE_MANAGER_H_
#define INC_MODE_MANAGER_H_

#include "usart.h"
#include "stdint.h"
#include "stdbool.h"
#include "usb_device.h"

enum{
	MODE_IDLE = 0x00,
	MODE_SEGA_SERIAL = 0x01,
	MODE_SPICE_API = 0X02,
	MODE_NAMCO_SERIAL = 0X03,
	MODE_CARD_IO = 0X04,
	MODE_KONAMI_SERIAL = 0X05,
};

enum{
	INTERFACE_NONE = 0X00,
	INTERFACE_CDC = 0X01,
	INTERFACE_UART = 0X02,
	INTERFACE_HID = 0X03,
};

void Reader_UART_IRQHandler();
void Reader_CDC_IRQHandler(uint8_t* data, uint8_t len);
bool Interface_Send(uint8_t* data ,uint8_t len);


#endif /* INC_MODE_MANAGER_H_ */
