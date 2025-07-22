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


typedef struct{
	uint8_t Current_Mode;
	uint8_t Current_Interface;
	uint8_t Uart_Buffer_Receive[128];
}Machine;

Machine Reader;

uint8_t Mode_Detect(uint8_t* data,uint8_t len){
	uint8_t test;
	test = sega_packet_check(data,len);
	if(test != 0){
		return MODE_SEGA_SERIAL;
	}
	return MODE_IDLE;
	//CDC_Transmit(0,data, len); // echo back on same channel
}

void Reader_UART_Init(){
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, Reader.Uart_Buffer_Receive, 128);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

void Reader_UART_IRQHandler(){
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)){
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		HAL_UART_DMAStop(&huart1);
		if(Reader.Current_Interface == MODE_IDLE){
			Reader.Current_Interface = INTERFACE_UART;
		}
		if(Reader.Current_Interface == INTERFACE_UART){
			//CDC_Transmit_FS((uint8_t*)uart_dma_buffer, 70);
			switch(Reader.Current_Mode){
				case MODE_IDLE:
					Reader.Current_Mode = Mode_Detect(Reader.Uart_Buffer_Receive,128 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx));
					break;
				case MODE_SEGA_SERIAL:
					Sega_Mode_Loop(sega_packet_check(Reader.Uart_Buffer_Receive,128 - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx)));
					//memcpy(req.bytes,Reader.Uart_Buffer_Receive);
			}
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
			HAL_UART_Receive_DMA(&huart1,Reader.Uart_Buffer_Receive,128);
		}else{
			HAL_UART_Receive_DMA(&huart1,Reader.Uart_Buffer_Receive,128);
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
		}
	}
}

void Reader_Uart_SendCommand(uint8_t* data, uint8_t len){
	HAL_UART_Transmit_DMA(&huart1, data, len);
}

void Reader_CDC_SendCommand(uint8_t* data, uint8_t len){
	CDC_Transmit(0, data, len);
}

void Reader_HID_SendReport(uint8_t* data){
	USBD_CUSTOM_HID_SendReport(&hUsbDevice, data, 9);
}

bool Interface_Send(uint8_t* data ,uint8_t len){
	switch(Reader.Current_Interface){
		case INTERFACE_CDC:
			Reader_CDC_SendCommand(data,len);
			return true;
		case INTERFACE_UART:
			Reader_Uart_SendCommand(data,len);
			return true;
		case INTERFACE_HID:
			USBD_CUSTOM_HID_SendReport(&hUsbDevice, data, 9);
			return true;
		default:
			return false;
	}
}
