/*
 * mode_manager.c
 *
 *  Created on: Jun 26, 2025
 *      Author: Qinh
 */
#include "mode_manager.h"
#include "mode_sega_serial.h"

extern USBD_HandleTypeDef hUsbDevice;
extern TIM_HandleTypeDef htim17;


extern uint8_t spice_led_ready;
Machine Reader;
uint8_t mode_probe_flag[2] = {0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM17) {
    	if(Reader.Current_Mode == MODE_SPICE_API){
    		spice_request();
    		return;
    	}
    }
}

void Mode_Poll(){
	if(Reader.Current_Mode != MODE_IDLE){
		return;
	}
	if(Reader.Current_Interface == INTERFACE_NONE){
		switch(mode_probe_flag[0]){
			case 0 :{
				Reader.Current_Interface = INTERFACE_CDC;
				spice_request();
				Reader.Current_Interface = INTERFACE_NONE;
				mode_probe_flag[0] = 1;
			}
			case 1 :{
				Reader.Current_Interface = INTERFACE_UART;
				spice_request();
				Reader.Current_Interface = INTERFACE_NONE;
				mode_probe_flag[0] = 0;
			}
		}
	}

}

uint8_t Mode_Detect(uint8_t* data,uint8_t len){
	uint8_t test;
	test = sega_packet_check(data,len);
	if(test){
		Sega_Mode_Loop(test);
		return MODE_SEGA_SERIAL;
	}
	test = spice_request_check(data,len);
	if(test){
		HAL_TIM_Base_Start_IT(&htim17);
		return MODE_SPICE_API;
	}
	test = namco_packet_check(data,len);
	if(test){
		//HAL_TIM_Base_Start_IT(&htim17);
		namco_packet_process(test);
		return MODE_SPICE_API;
	}
	return MODE_IDLE;
}

void Reader_CDC_IRQHandler(uint8_t* data, uint8_t len){
	if(Reader.Current_Interface == MODE_IDLE){
		Reader.Current_Interface = INTERFACE_CDC;
	}
	if(Reader.Current_Interface == INTERFACE_CDC){
		switch(Reader.Current_Mode){
			case MODE_IDLE:
				Reader.Current_Mode = Mode_Detect(data,len);
				break;
			case MODE_SEGA_SERIAL:
				Sega_Mode_Loop(sega_packet_check(data,len));
				break;
			case MODE_SPICE_API:
				spice_request_check(data,len);
				break;
			case MODE_NAMCO_SERIAL:
				namco_packet_process(namco_packet_check(data,len));
				break;
			default:
				break;
		}
	}
	if(Reader.Current_Mode == MODE_IDLE){
		Reader.Current_Interface = INTERFACE_NONE;
	}
}

void Reader_CDC_SendCommand(uint8_t* data, uint8_t len){
	if(len == 64 && len == 128){
		len++;
	}
	CDC_Transmit(0, data, len);
}

void Reader_HID_SendReport(uint8_t* data){
	USBD_CUSTOM_HID_SendReport(&hUsbDevice, data, 9);
}

bool Interface_Send(const uint8_t* data ,uint8_t len){
	Reader_CDC_SendCommand(data,len);
}
