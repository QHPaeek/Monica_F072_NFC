/**
  ******************************************************************************
  * @file           : rfal_defConfig.h
  * @brief          : This file contains definitions for RFAL Default Configuration
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
#ifndef __RFAL_DEFCONFIG_H__
#define __RFAL_DEFCONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

/* Exported constants --------------------------------------------------------*/
/** @defgroup Platform_Exported_Constants
 *  @{
 */

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

/*
******************************************************************************
* RFAL FEATURES CONFIGURATION
******************************************************************************
*/
#define RFAL_FEATURE_LISTEN_MODE               true       /*!< Enable/Disable RFAL support for Listen Mode                               */
#define RFAL_FEATURE_WAKEUP_MODE               true       /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
#define RFAL_FEATURE_LOWPOWER_MODE             false      /*!< Enable/Disable RFAL support for the Low Power mode                        */
#define RFAL_FEATURE_NFCA                      true       /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
#define RFAL_FEATURE_NFCB                      true       /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
#define RFAL_FEATURE_NFCF                      true       /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
#define RFAL_FEATURE_NFCV                      true       /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
#define RFAL_FEATURE_T1T                       true       /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
#define RFAL_FEATURE_T2T                       true       /*!< Enable/Disable RFAL support for T2T                                       */
#define RFAL_FEATURE_T4T                       true       /*!< Enable/Disable RFAL support for T4T                                       */
#define RFAL_FEATURE_ST25TB                    true       /*!< Enable/Disable RFAL support for ST25TB                                    */
#define RFAL_FEATURE_ST25xV                    true       /*!< Enable/Disable RFAL support for  ST25TV/ST25DV                            */
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG     false      /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
#define RFAL_FEATURE_DPO                       false      /*!< Enable/Disable RFAL Dynamic Power Output support                          */
#define RFAL_FEATURE_ISO_DEP                   true       /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
#define RFAL_FEATURE_ISO_DEP_POLL              true       /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
#define RFAL_FEATURE_ISO_DEP_LISTEN            true       /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
#define RFAL_FEATURE_NFC_DEP                   true       /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */

#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN    256U       /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN     254U       /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
#define RFAL_FEATURE_NFC_RF_BUF_LEN            258U       /*!< RF buffer length used by RFAL NFC layer                                   */

#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN      256U       /*!< ISO-DEP APDU max length. Please use multiples of I-Block max length       */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN       512U       /*!< NFC-DEP PDU max length.                                                   */

/*
******************************************************************************
* RFAL CUSTOM SETTINGS
******************************************************************************
  Custom analog configs are used to cope with Automatic Antenna Tuning (AAT)
  that are optimized differently for each board.
*/
#define RFAL_ANALOG_CONFIG_CUSTOM                         /*!< Use Custom Analog Configs when defined                                    */

/* Exported variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* __RFAL_DEFCONFIG_H__*/

