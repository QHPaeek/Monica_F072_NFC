/*
 * mode_manager.c
 *
 *  Created on: Jun 26, 2025
 *      Author: Qinh
 */
#include "mode_manager.h"
#include "mode_sega_serial.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern USBD_HandleTypeDef hUsbDevice;
extern TIM_HandleTypeDef htim17;


extern uint8_t spice_led_ready;
Machine Reader;
uint8_t mode_probe_flag[2] = {0,0};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM17) {
    	if(Reader.Current_Mode == MODE_SPICE_API){// && spice_led_ready == 0){
    		spice_request();
    		return;
    	}
    }
}

void Mode_Poll(){
	if(Reader.Current_Mode == MODE_SPICE_API){// && spice_led_ready == 1){
//		spice_request();
//		spice_led_ready = 0;
		return;
	}else if(Reader.Current_Mode != MODE_IDLE){
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
	}else{
		//spice_request();
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
	return MODE_IDLE;
	//CDC_Transmit(0,data, len); // echo back on same channel
}

void Reader_UART_Init(){
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, Reader.Uart_Buffer_Receive, 256);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	uint8_t data = 1;
	HAL_UART_Transmit_DMA(&huart1, &data, 1);
}

void Reader_UART_IRQHandler(){
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)){

		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		HAL_UART_DMAStop(&huart1);
		if(Reader.Current_Interface == INTERFACE_NONE){
			Reader.Current_Interface = INTERFACE_UART;
		}
		if(Reader.Current_Interface == INTERFACE_UART){
			//CDC_Transmit_FS((uint8_t*)uart_dma_buffer, 70);
			switch(Reader.Current_Mode){
				case MODE_IDLE:
					Reader.Current_Mode = Mode_Detect(Reader.Uart_Buffer_Receive,256 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx));
					break;
				case MODE_SEGA_SERIAL:
					Sega_Mode_Loop(sega_packet_check(Reader.Uart_Buffer_Receive,256 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx)));
					//memcpy(req.bytes,Reader.Uart_Buffer_Receive);
					break;
				case MODE_SPICE_API:
					spice_request_check(Reader.Uart_Buffer_Receive,256 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx));
					break;
				default:
					break;
			}
			if(Reader.Current_Mode == MODE_IDLE){
				Reader.Current_Interface = INTERFACE_NONE;
			}
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
			HAL_UART_Receive_DMA(&huart1,Reader.Uart_Buffer_Receive,256);
		}else{
			HAL_UART_Receive_DMA(&huart1,Reader.Uart_Buffer_Receive,256);
		}
	}
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
			default:
				break;
		}
	}
	if(Reader.Current_Mode == MODE_IDLE){
		Reader.Current_Interface = INTERFACE_NONE;
	}
}

void Reader_Uart_SendCommand(uint8_t* data, uint8_t len){
	HAL_UART_Transmit_DMA(&huart1, data, len);
}

void Reader_CDC_SendCommand(uint8_t* data, uint8_t len){
//	if(len > 128){
//		return;
//		//cann't send over 128 byte,too long
//	}
//	if(len> 62){
//		CDC_Transmit(0, data, 62);
//		//HAL_Delay(100);
//
//		CDC_Transmit(0, data+62, len - 62);
//	}else{
//		CDC_Transmit(0, data, len);
//	}
	if(len == 64){
		len++;
	}
	CDC_Transmit(0, data, len);
}

void Reader_HID_SendReport(uint8_t* data){
	USBD_CUSTOM_HID_SendReport(&hUsbDevice, data, 9);
}

bool Interface_Send(uint8_t* data ,uint8_t len){
//	uint8_t data2 = Reader.Current_Interface;
//	Reader_Uart_SendCommand( data, 5);
	switch(Reader.Current_Interface){
		case INTERFACE_CDC:
			Reader_CDC_SendCommand(data,len);
			break;
		case INTERFACE_UART:
			Reader_Uart_SendCommand(data,len);
			break;
		case INTERFACE_HID:
			USBD_CUSTOM_HID_SendReport(&hUsbDevice, data, 9);
			break;
		default:
			return false;
	}
	return true;
}
