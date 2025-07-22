/**
  ******************************************************************************
  * @file    rfal_defConfig.h
  * @author  MMY Application Team
  * @brief   RFAL Hardware Abstraction Layer
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RFAL_DEF_CONFIG_H__
#define __RFAL_DEF_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/


/*
******************************************************************************
* RFAL FEATURES DEFAULT CONFIGURATION
******************************************************************************
*/
#ifndef RFAL_FEATURE_LISTEN_MODE
    #if RFAL_SUPPORT_CE || RFAL_SUPPORT_MODE_LISTEN_ACTIVE_P2P
        #define RFAL_FEATURE_LISTEN_MODE            true       /*!< Enable RFAL support for Listen Mode                               */
    #endif /* SUPPORT LISTEN_MODE */
#endif /* RFAL_FEATURE_LISTEN_MODE */

        
#ifndef RFAL_FEATURE_WAKEUP_MODE
    #define RFAL_FEATURE_WAKEUP_MODE                true       /*!< Enable RFAL support for the Wake-Up mode                          */
#endif /* RFAL_FEATURE_WAKEUP_MODE */


#ifndef RFAL_FEATURE_LOWPOWER_MODE
    #define RFAL_FEATURE_LOWPOWER_MODE              true       /*!< Enable RFAL support for the Low Power mode                        */
#endif /* RFAL_FEATURE_LOWPOWER_MODE */


#ifndef RFAL_FEATURE_NFCA
    #if RFAL_SUPPORT_MODE_POLL_NFCA
        #define RFAL_FEATURE_NFCA                   true       /*!< Enable RFAL support for NFC-A (ISO14443A)                         */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCA */
#endif /* RFAL_FEATURE_NFCA */


#ifndef RFAL_FEATURE_T1T
    #if RFAL_SUPPORT_MODE_POLL_NFCA
        #define RFAL_FEATURE_T1T                    true       /*!< Enable RFAL support for T1T (Topaz)                               */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCA */
#endif /* RFAL_FEATURE_T1T */

#ifndef RFAL_FEATURE_T2T
    #if RFAL_SUPPORT_MODE_POLL_NFCA
        #define RFAL_FEATURE_T2T                    true       /*!< Enable RFAL support for T2T                                       */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCA */
#endif /* RFAL_FEATURE_T2T */

#ifndef RFAL_FEATURE_T4T
    #if RFAL_SUPPORT_MODE_POLL_NFCA
        #define RFAL_FEATURE_T4T                    true       /*!< Enable RFAL support for T4T                                       */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCA */
#endif /* RFAL_FEATURE_T2T */


#ifndef RFAL_FEATURE_NFCB
    #if RFAL_SUPPORT_MODE_POLL_NFCB
        #define RFAL_FEATURE_NFCB                   true       /*!< Enable RFAL support for NFC-B (ISO14443B)                         */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCB */
#endif /* RFAL_FEATURE_NFCB */


#ifndef RFAL_FEATURE_ST25TB
    #if RFAL_SUPPORT_MODE_POLL_NFCB
        #define RFAL_FEATURE_ST25TB                 true       /*!< Enable RFAL support for ST25TB                                    */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCB */
#endif /* RFAL_FEATURE_ST25TB */


#ifndef RFAL_FEATURE_NFCF
    #if RFAL_SUPPORT_MODE_POLL_NFCF
        #define RFAL_FEATURE_NFCF                   true       /*!< Enable RFAL support for NFC-F (FeliCa)                            */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCF */
#endif /* RFAL_FEATURE_NFCF */


#ifndef RFAL_FEATURE_NFCV
    #if RFAL_SUPPORT_MODE_POLL_NFCV
        #define RFAL_FEATURE_NFCV                   true       /*!< Enable RFAL support for NFC-V (ISO15693)                          */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCV */
#endif /* RFAL_FEATURE_NFCV */


#ifndef RFAL_FEATURE_ISO_DEP
    #if RFAL_SUPPORT_MODE_POLL_NFCA || RFAL_SUPPORT_MODE_POLL_NFCB || RFAL_SUPPORT_CE
        #define RFAL_FEATURE_ISO_DEP                true       /*!< Enable RFAL support for ISO-DEP (ISO14443-4)                      */
    #endif /* RFAL_SUPPORT_MODE_ */
#endif /* RFAL_FEATURE_ISO_DEP */


#ifndef RFAL_FEATURE_ISO_DEP_POLL
    #if RFAL_SUPPORT_MODE_POLL_NFCA || RFAL_SUPPORT_MODE_POLL_NFCB
        #define RFAL_FEATURE_ISO_DEP_POLL           true       /*!< Enable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
    #endif /* RFAL_SUPPORT_MODE_ */
#endif /* RFAL_FEATURE_ISO_DEP */


#ifndef RFAL_FEATURE_ISO_DEP_LISTEN
    #if RFAL_SUPPORT_CE
        #define RFAL_FEATURE_ISO_DEP_LISTEN         true       /*!< Enable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
    #endif /* RFAL_SUPPORT_MODE_ */
#endif /* RFAL_FEATURE_ISO_DEP */


#ifndef RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN
    #if RFAL_FEATURE_ISO_DEP
        #define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN 256U       /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
    #endif /* RFAL_FEATURE_ISO_DEP */
#endif /* RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN */


#ifndef RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN
    #if RFAL_FEATURE_ISO_DEP
        #define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN   512U       /*!< ISO-DEP APDU max length.                                                  */
    #endif /* RFAL_FEATURE_ISO_DEP */
#endif /* RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN */


#ifndef RFAL_FEATURE_NFC_DEP
    #if RFAL_SUPPORT_MODE_POLL_NFCA && RFAL_SUPPORT_MODE_POLL_NFCF
        #define RFAL_FEATURE_NFC_DEP                true       /*!< Enable RFAL support for NFC-DEP (NFCIP1/P2P)                              */
    #endif /* RFAL_SUPPORT_MODE_POLL_NFCA/F */
#endif /* RFAL_FEATURE_NFC_DEP */

#ifndef RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN
    #if RFAL_FEATURE_NFC_DEP
        #define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN  254U       /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
    #endif /* RFAL_FEATURE_NFC_DEP */
#endif /* RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN */


#ifndef RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN
    #if RFAL_FEATURE_NFC_DEP
        #define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN    512U       /*!< NFC-DEP PDU max length.                                                   */
    #endif /* RFAL_FEATURE_NFC_DEP */
#endif /* RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN */


#ifndef RFAL_FEATURE_NFC_RF_BUF_LEN
    #define RFAL_FEATURE_NFC_RF_BUF_LEN             258U       /*!< RF buffer length used by RFAL NFC layer                                   */
#endif /* RFAL_FEATURE_NFC_RF_BUF_LEN */


/**************************************
    Disabled by default/omission:
        - RFAL_FEATURE_ST25xV
        - RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG
        - RFAL_FEATURE_DPO
 */
#ifdef __cplusplus
}
#endif

#endif /* __RFAL_DEF_CONFIG_H__*/