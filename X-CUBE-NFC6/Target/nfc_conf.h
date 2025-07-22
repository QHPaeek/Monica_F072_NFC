/**
  ******************************************************************************
  * @file           : nfc_conf.h
  * @brief          : This file contains definitions for the NFC6/8 components bus interfaces
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NFC0XA1_CONF_H__
#define __NFC0XA1_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"
#include "custom_bus.h"
#include "custom_errno.h"
#include "stm32f0xx_hal_exti.h"
#include "RTE_Components.h"
#include "st25r3916_irq.h"
#include "rfal_defConfig.h"

#define TEST               1
#define TEST_LINE          EXTI0_1_IRQn

#define TEST_RFAL_DEFAULT

#define TESTED_VAR_OK_BMP

#define LED_FIELD_Pin 1         /*!< Enable usage of led field pin on the platform      */
#define LED_FIELD_GPIO_Port 1   /*!< Enable usage of led field port on the platform     */
#define USE_LOGGER 1

#define NFC06A1_LED1_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED2_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED3_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED4_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED5_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED6_PIN_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define NFC06A1_LED1_PIN                  GPIO_PIN_0
#define NFC06A1_LED1_PIN_PORT             GPIOB
#define NFC06A1_LED2_PIN                  GPIO_PIN_1
#define NFC06A1_LED2_PIN_PORT             GPIOB
#define NFC06A1_LED3_PIN                  GPIO_PIN_2
#define NFC06A1_LED3_PIN_PORT             GPIOB
#define NFC06A1_LED4_PIN                  GPIO_PIN_10
#define NFC06A1_LED4_PIN_PORT             GPIOB
#define NFC06A1_LED5_PIN                  GPIO_PIN_11
#define NFC06A1_LED5_PIN_PORT             GPIOB
#define NFC06A1_LED6_PIN                  GPIO_PIN_12
#define NFC06A1_LED6_PIN_PORT             GPIOB

#define NFC06A1_ALLLED_GPIO_CLK_ENABLE() {NFC06A1_LED1_PIN_CLK_ENABLE();NFC06A1_LED2_PIN_CLK_ENABLE();NFC06A1_LED3_PIN_CLK_ENABLE();NFC06A1_LED4_PIN_CLK_ENABLE();NFC06A1_LED5_PIN_CLK_ENABLE();NFC06A1_LED6_PIN_CLK_ENABLE();}
#define NFC06A1_NFCTAG_INSTANCE         (0)
#define NFC06A1_NFCTAG_GPO_PRIORITY     (0)

#ifdef LED_FIELD_Pin
/* Case of ST25R3916 */
#define PLATFORM_LED_FIELD_PIN       GPIO_PIN_12         /*!< GPIO pin used as field LED                        */
#endif

#ifdef LED_FIELD_GPIO_Port
/* Case of ST25R3916 */
#define PLATFORM_LED_FIELD_PORT       GPIOB    /*!< GPIO port used as field LED                       */
#endif

/* Case of ST25R3916 */
#define PLATFORM_LED_A_PIN           GPIO_PIN_2         /*!< GPIO pin used for LED A    */
#define PLATFORM_LED_A_PORT          GPIOB    /*!< GPIO port used for LED A   */
#define PLATFORM_LED_B_PIN           GPIO_PIN_1         /*!< GPIO pin used for LED B    */
#define PLATFORM_LED_B_PORT          GPIOB    /*!< GPIO port used for LED B   */
#define PLATFORM_LED_F_PIN           GPIO_PIN_0         /*!< GPIO pin used for LED F    */
#define PLATFORM_LED_F_PORT          GPIOB    /*!< GPIO port used for LED F   */
#define PLATFORM_LED_V_PIN           GPIO_PIN_10         /*!< GPIO pin used for LED V    */
#define PLATFORM_LED_V_PORT          GPIOB    /*!< GPIO port used for LED V   */
#define PLATFORM_LED_AP2P_PIN        GPIO_PIN_11         /*!< GPIO pin used for LED AP2P */
#define PLATFORM_LED_AP2P_PORT       GPIOB    /*!< GPIO port used for LED AP2P*/

#define BUS_SPI1_NSS_GPIO_PIN           GPIO_PIN_4
#define BUS_SPI1_NSS_GPIO_PORT          GPIOA
#define BUS_SPI1_IRQ_GPIO_PIN           GPIO_PIN_1
#define BUS_SPI1_IRQ_GPIO_PORT          GPIOA

/* Exported constants --------------------------------------------------------*/
/** @defgroup PTD_Platform_Exported_Constants
 *  @{
 */
#define ST25R_SS_PIN             BUS_SPI1_NSS_GPIO_PIN    /*!< GPIO pin used for ST25R SPI SS                */
#define ST25R_SS_PORT            BUS_SPI1_NSS_GPIO_PORT   /*!< GPIO port used for ST25R SPI SS port          */

#define ST25R_INT_PIN            BUS_SPI1_IRQ_GPIO_PIN    /*!< GPIO pin used for ST25R IRQ                   */
#define ST25R_INT_PORT           BUS_SPI1_IRQ_GPIO_PORT   /*!< GPIO port used for ST25R IRQ port             */

#define IRQ_ST25R_EXTI_IRQn      EXTI0_1_IRQn

#define PLATFORM_USER_BUTTON_PIN     USER_BUTTON_PIN          /*!< GPIO pin user button       */
#define PLATFORM_USER_BUTTON_PORT    USER_BUTTON_GPIO_PORT    /*!< GPIO port user button      */

#define USR_INT_LINE             H_EXTI_1
#define USR_INT_LINE_NUM         EXTI_LINE_1
#define BSP_NFC0XCOMM_Init             BSP_SPI1_Init
#define BSP_NFC0XCOMM_IRQ_Callback     BSP_SPI1_IRQ_Callback

#define BSP_NFC0XCOMM_SequencialSend  BSP_SPI1_SequencialSend
#define BSP_NFC0XCOMM_SequencialRecv  BSP_SPI1_SequencialRecv

void BSP_SPI1_IRQ_Callback(void);
/**
  * @}
  */

/**
  * @}
  */

/*
******************************************************************************
* GLOBAL VARIABLES
******************************************************************************
*/

/* Exported variables --------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

extern uint8_t globalCommProtectCnt;
extern  EXTI_HandleTypeDef H_EXTI_1;
/* Exported functions ------------------------------------------------------- */
int32_t BSP_NFC0XCOMM_SendRecv(const uint8_t * const pTxData, uint8_t * const pRxData, uint16_t Length);

int32_t BSP_NFC0XCOMM_Init(void);

#define  COMM_HANDLE                                     hspi1

void MX_NFC6_PollingTagDetect_Process(void);
void MX_NFC6_PollingDemo_Init(void);

void _Error_Handler(char * file, int line);

#ifdef __cplusplus
}
#endif

#endif /* __NFC0XA1_CONF_H__*/

