/**
  ******************************************************************************
  * @file           : nfc06a1.h
  * @author         : MMY Application Team
  * @brief          : This file contains definitions for the nfc06a1.c
  *                   board specific functions. 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef NFC06A1_H
#define NFC06A1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rfal_platform.h"

/** @addtogroup BSP
  * @{
  */

/** @defgroup NFC06A1
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup NFC06A1_Exported_Types
  * @{
  */
/**
 * @brief  NFC06A1 Led enumerator definition
 */
typedef enum 
{
  TF_LED = 0,         /*!< Led Type F   */
  TB_LED,             /*!< Led Type B   */
  TA_LED,             /*!< Led Type A   */
  TV_LED,             /*!< Led Type V   */
  AP2P_LED,           /*!< Led Type P2P */
  TX_LED              /*!< Led Field    */
}NFC06A1_Led_E;

/**
 * @brief  NFC06A1 Led structure definition
 */
typedef struct
{
  uint16_t          NFC06A1_LED_PIN;
  GPIO_TypeDef *    NFC06A1_LED_PIN_PORT;
}NFC06A1_Led_TypeDef;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/** @defgroup NFC06A1_Exported_Functions
  * @{
  */

void NFC06A1_LED_Init(void);
void NFC06A1_LED_DeInit(const NFC06A1_Led_E led);
void NFC06A1_LED_ON(const NFC06A1_Led_E led);
void NFC06A1_LED_OFF(const NFC06A1_Led_E led);
void NFC06A1_LED_Toggle(const NFC06A1_Led_E led);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

#ifdef __cplusplus
  }
#endif

#endif /* NFC06A1_H */


