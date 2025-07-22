/**
  ******************************************************************************
  * @file           : nfc06a1.c
  * @author         : MMY Application Team
  * @brief          : This file provides nfc06a1 specific functions
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

/* Includes ------------------------------------------------------------------*/
#include "nfc06a1.h"

/** @addtogroup BSP
 *  @{
 */

/** @addtogroup NFC06A1
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/** @defgroup NFC06A1_Private_Variables
 * @{
 */

/** Array including all led pin and port information */
static NFC06A1_Led_TypeDef NFC06A1_Led[6] = { { NFC06A1_LED1_PIN, NFC06A1_LED1_PIN_PORT } , 
                                              { NFC06A1_LED2_PIN, NFC06A1_LED2_PIN_PORT } ,
                                              { NFC06A1_LED3_PIN, NFC06A1_LED3_PIN_PORT } ,
                                              { NFC06A1_LED4_PIN, NFC06A1_LED4_PIN_PORT } , 
                                              { NFC06A1_LED5_PIN, NFC06A1_LED5_PIN_PORT } ,
                                              { NFC06A1_LED6_PIN, NFC06A1_LED6_PIN_PORT } };

/**
 * @}
 */

/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/** @defgroup NFC06A1_Private_Functions
 * @{
 */

/**
  * @brief  This function initialize the GPIO to manage the Leds
  *         through GPIO
  * @param  None
  * @return None
  */
void NFC06A1_LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  uint8_t led;
  
  /* Enable all GPIO port clock for the Led pins */
  NFC06A1_ALLLED_GPIO_CLK_ENABLE();
  
  /* Configure GPIO for all Led pins */
  for (led = 0; led < (sizeof(NFC06A1_Led) / sizeof(NFC06A1_Led_TypeDef)); led++)
  {
    GPIO_InitStruct.Pin = NFC06A1_Led[led].NFC06A1_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, &GPIO_InitStruct);
    
    /* Configure Led pin Output Level as off */
    HAL_GPIO_WritePin(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, NFC06A1_Led[led].NFC06A1_LED_PIN, GPIO_PIN_RESET);
  }
}

/**
  * @brief     DeInit LEDs.
  * @note      Led DeInit does not disable the GPIO clock nor disable the Mfx
  * @param[in] Led : LED to be de-init. 
  * @return    None
  */
void NFC06A1_LED_DeInit(const NFC06A1_Led_E led)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Turn off LED */
  HAL_GPIO_WritePin(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, NFC06A1_Led[led].NFC06A1_LED_PIN, GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  gpio_init_structure.Pin = NFC06A1_Led[led].NFC06A1_LED_PIN;
  HAL_GPIO_DeInit(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, gpio_init_structure.Pin);
}

/**
  * @brief     This function light on selected Led
  * @param[in] led : Led to be lit on
  * @return    None
  */
void NFC06A1_LED_ON(const NFC06A1_Led_E led)
{
  HAL_GPIO_WritePin(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, NFC06A1_Led[led].NFC06A1_LED_PIN, GPIO_PIN_SET);
}

/**
  * @brief     This function light off selected Led
  * @param[in] led : Led to be lit off
  * @return    None
  */
void NFC06A1_LED_OFF(const NFC06A1_Led_E led)
{
  HAL_GPIO_WritePin(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, NFC06A1_Led[led].NFC06A1_LED_PIN, GPIO_PIN_RESET);
}

/**
  * @brief     Toggles the selected LED
  * @param[in] led : Specifies the Led to be toggled
  * @return    None
  */
void NFC06A1_LED_Toggle(const NFC06A1_Led_E led)
{
  HAL_GPIO_TogglePin(NFC06A1_Led[led].NFC06A1_LED_PIN_PORT, NFC06A1_Led[led].NFC06A1_LED_PIN);
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */


