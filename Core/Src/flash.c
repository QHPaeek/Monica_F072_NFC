/*
 * flash.c
 *
 *  Created on: Jul 18, 2025
 *      Author: Qinh
 */

#ifndef SRC_FLASH_C_
#define SRC_FLASH_C_

#include <stdint.h>
#include <string.h>
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"
#include "stm32f0xx_hal_flash_ex.h"
#include "flash.h"

#define F072_FLASH_BASE        	((uint32_t)0x08000000)
#define F072_FLASH_SIZE        	0x10000  // Flash大小为64KB
#define F072_FLASH_PAGE_SIZE   	0x400    // 页大小为1KB
#define F072_TARGET_PAGE       	62       // 使用倒数第二页
#define F072_TARGET_DOUBLEWORD 	16       // 16个双字存储
#define VERSION 				1

FlashData Flash;

void flash_init(){
	flash_read(Flash.raw_flash);
	if(Flash.version != VERSION){

	}
}
void flash_write(uint64_t* data){
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef Erase;
    Erase.TypeErase = FLASH_TYPEERASE_PAGES;
    Erase.PageAddress = F072_FLASH_BASE + F072_FLASH_PAGE_SIZE * F072_TARGET_PAGE;
    Erase.NbPages = 1;
    uint32_t PageError = 0;
    HAL_FLASHEx_Erase(&Erase, &PageError);

    for(uint8_t i = 0; i < F072_TARGET_DOUBLEWORD; i++){
        while(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,
              F072_FLASH_BASE + F072_FLASH_PAGE_SIZE * F072_TARGET_PAGE + i * 8,
              data[i]) != HAL_OK);
    }

    HAL_FLASH_Lock();
}

void flash_read(uint64_t* data){
    memcpy(data, F072_FLASH_BASE + F072_FLASH_PAGE_SIZE * F072_TARGET_PAGE, 128);
}

#endif /* SRC_FLASH_C_ */
