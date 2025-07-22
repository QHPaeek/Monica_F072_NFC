/**
  ******************************************************************************
  * @file           : analogConfigTbl_NFC06A1.c
  * @brief          : This file contains the analog interface
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

/*
 *      PROJECT:   ST25R391x firmware
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*! \file analogConfigTbl_NFC06A1.c
 *
 * Please set RFAL_ANALOG_CONFIG_CUSTOM in rfal_platform.h and include
 * this file to your project to become active and overwrite the default analog
 * configs coming with RFAL
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "rfal_analogConfig.h"

#ifndef RFAL_ANALOG_CONFIG_CUSTOM
#warning "Custom analog configs are compiled but not being used. Please define RFAL_ANALOG_CONFIG_CUSTOM."
#endif

/*
 ******************************************************************************
 * GLOBAL MACROS
 ******************************************************************************
 */

/*! Macro for Configuration Setting with only 1 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1] */
#define MODE_ENTRY_1_REG(MODE, R0, M0, V0)              \
    (MODE >> 8), (MODE & 0xFF), 1, (R0 >> 8), (R0 & 0xFF), (M0), (V0)

/*! Macro for Configuration Setting with only 2 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1] */
#define MODE_ENTRY_2_REG(MODE, R0, M0, V0, R1, M1, V1)  \
    (MODE >> 8), (MODE & 0xFF), 2, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1)

/*! Macro for Configuration Setting with only 3 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_3_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2) \
    (MODE >> 8), (MODE & 0xFF), 3, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \

/*! Macro for Configuration Setting with only 4 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_4_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3) \
    (MODE >> 8), (MODE & 0xFF), 4, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \

/*! Macro for Configuration Setting with only 5 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_5_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4) \
    (MODE >> 8), (MODE & 0xFF), 5, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \

/*! Macro for Configuration Setting with only 6 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_6_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5) \
    (MODE >> 8), (MODE & 0xFF), 6, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \

/*! Macro for Configuration Setting with only 7 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_7_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6) \
    (MODE >> 8), (MODE & 0xFF), 7, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \

/*! Macro for Configuration Setting with only 8 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_8_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7) \
    (MODE >> 8), (MODE & 0xFF), 8, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \

/*! Macro for Configuration Setting with only 9 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_9_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8) \
    (MODE >> 8), (MODE & 0xFF), 9, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \

/*! Macro for Configuration Setting with only 10 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_10_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9) \
    (MODE >> 8), (MODE & 0xFF), 10, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \

/*! Macro for Configuration Setting with only 11 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_11_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10) \
    (MODE >> 8), (MODE & 0xFF), 11, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \

/*! Macro for Configuration Setting with only 12 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_12_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11) \
    (MODE >> 8), (MODE & 0xFF), 12, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \

/*! Macro for Configuration Setting with only 13 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_13_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12) \
    (MODE >> 8), (MODE & 0xFF), 13, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \
                                 , (R12 >> 8), (R12 & 0xFF), (M12), (V12) \

/*! Macro for Configuration Setting with only 14 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_14_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13) \
    (MODE >> 8), (MODE & 0xFF), 14, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \
                                 , (R12 >> 8), (R12 & 0xFF), (M12), (V12) \
                                 , (R13 >> 8), (R13 & 0xFF), (M13), (V13) \

/*! Macro for Configuration Setting with only 15 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_15_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14) \
    (MODE >> 8), (MODE & 0xFF), 15, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \
                                 , (R12 >> 8), (R12 & 0xFF), (M12), (V12) \
                                 , (R13 >> 8), (R13 & 0xFF), (M13), (V13) \
                                 , (R14 >> 8), (R14 & 0xFF), (M14), (V14) \

/*! Macro for Configuration Setting with only 16 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_16_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14, R15, M15, V15) \
    (MODE >> 8), (MODE & 0xFF), 16, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \
                                 , (R12 >> 8), (R12 & 0xFF), (M12), (V12) \
                                 , (R13 >> 8), (R13 & 0xFF), (M13), (V13) \
                                 , (R14 >> 8), (R14 & 0xFF), (M14), (V14) \
                                 , (R15 >> 8), (R15 & 0xFF), (M15), (V15) \

/*! Macro for Configuration Setting with only 17 register-mask-value set:
*  - Configuration ID[2], Number of Register sets to follow[1], Register[2], Mask[1], Value[1], Register[2], Mask[1], Value[1], Register[2]... */
#define MODE_ENTRY_17_REG(MODE, R0, M0, V0, R1, M1, V1, R2, M2, V2, R3, M3, V3, R4, M4, V4, R5, M5, V5, R6, M6, V6, R7, M7, V7, R8, M8, V8, R9, M9, V9, R10, M10, V10, R11, M11, V11, R12, M12, V12, R13, M13, V13, R14, M14, V14, R15, M15, V15, R16, M16, V16) \
    (MODE >> 8), (MODE & 0xFF), 17, (R0 >> 8), (R0 & 0xFF), (M0), (V0) \
                                 , (R1 >> 8), (R1 & 0xFF), (M1), (V1) \
                                 , (R2 >> 8), (R2 & 0xFF), (M2), (V2) \
                                 , (R3 >> 8), (R3 & 0xFF), (M3), (V3) \
                                 , (R4 >> 8), (R4 & 0xFF), (M4), (V4) \
                                 , (R5 >> 8), (R5 & 0xFF), (M5), (V5) \
                                 , (R6 >> 8), (R6 & 0xFF), (M6), (V6) \
                                 , (R7 >> 8), (R7 & 0xFF), (M7), (V7) \
                                 , (R8 >> 8), (R8 & 0xFF), (M8), (V8) \
                                 , (R9 >> 8), (R9 & 0xFF), (M9), (V9) \
                                 , (R10 >> 8), (R10 & 0xFF), (M10), (V10) \
                                 , (R11 >> 8), (R11 & 0xFF), (M11), (V11) \
                                 , (R12 >> 8), (R12 & 0xFF), (M12), (V12) \
                                 , (R13 >> 8), (R13 & 0xFF), (M13), (V13) \
                                 , (R14 >> 8), (R14 & 0xFF), (M14), (V14) \
                                 , (R15 >> 8), (R15 & 0xFF), (M15), (V15) \
                                 , (R16 >> 8), (R16 & 0xFF), (M16), (V16) \

/*
******************************************************************************
* GLOBAL DATA TYPES
******************************************************************************
*/

const uint8_t rfalAnalogConfigCustomSettings[] = {

    /* Mode Name: CHIP_INIT, Mode ID: 0x0000 */
    MODE_ENTRY_17_REG(0x0000,
        0x0000,0x07,0x07, /* User Defined ; Disable MCU_CLK */
        0x0001,0x18,0x18, /* User Defined ; SPI Pull downs */
        0x0001,0x20,0x20, /* User Defined ; Enable AAT */
        0x0028,0x0f,0x00, /* RFO driver resistance Active transmission (d_res) ; Set RFO resistance Active Tx */
        0x006a,0xff,0x80, /* User Defined ; Use minimum non-overlap */
        0x002a,0x70,0x10, /* Activation Peer Detection Threshold (trg_lNa) ; Lower activation threshold (higher than deactivation) */
        0x002a,0x0f,0x01, /* Activation Collision Avoidance Threshold (rfe_tNa) : 105 mVpp ; Lower activation threshold (higher than deactivation) */
        0x002b,0x70,0x00, /* Deactivation Collision Avoidance Threshold (trg_lNd) :  75 mVpp ; Lower deactivation threshold */
        0x002b,0x0f,0x00, /* Deactivation Peer Detection Threshold (rfe_tNd) :  75 mVpp ; Lower deactivation threshold */
        0x0068,0x20,0x00, /* External Load Modulation Selection (lm_ext) : External Load modulation disabled ; Disable external Load Modulation */
        0x0068,0x10,0x10, /* Driver Load Modulation Selection (lm_dri) ; Use internal Load Modulation */
        0x0008,0xf0,0x50, /* PCD to PICC FDT compensation (fdel) ; Adjust the FDT to be aligned with the bitgrid */
        0x0029,0xff,0x5f, /* User Defined ; Change RFO resistance for Load Modulation */
        0x0045,0x40,0x40, /* User Defined ; Enable start on first 4 bits */
        0x0026,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0027,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0084,0x10,0x10  /* User Defined ; Avoid chip internal overheat protection */
    ),

    /* Mode Name: POLL_COMMON, Mode ID: 0x0008 */
    MODE_ENTRY_9_REG(0x0008,
        0x0003,0x04,0x04, /* Transmit AM (tr_am) ; Use AM modulation */
        0x0028,0xf0,0x70, /* AM Modulation Index (am_mod) ; Set Modulation index */
        0x0068,0x88,0x00, /* User Defined ; Use AM via regulator */
        0x0026,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0027,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0070,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0071,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0072,0xff,0x00, /* User Defined ; Disable Undershoot Protection */
        0x0073,0xff,0x00  /* User Defined ; Disable Undershoot Protection */
    ),

    /* Mode Name: POLL_A_COMMON_RX, Mode ID: 0x0102 */
    MODE_ENTRY_1_REG(0x0102,
        0x000a,0x04,0x00  /* Receiver Operation Selection (dis_corr) : Correlation operation ; Use Correlator Receiver */
    ),

    /* Mode Name: POLL_A_106_TX, Mode ID: 0x0111 */
    MODE_ENTRY_5_REG(0x0111,
        0x0003,0x04,0x00, /* Transmit AM (tr_am) : OOK ; Use OOK modulation */
        0x0070,0xff,0x40, /* User Defined ; Set default Overshoot Protection */
        0x0071,0xff,0x03, /* User Defined ; Set default Overshoot Protection */
        0x0072,0xff,0x40, /* User Defined ; Set default Undershoot Protection */
        0x0073,0xff,0x03  /* User Defined ; Set default Undershoot Protection */
    ),

    /* Mode Name: POLL_A_106_RX, Mode ID: 0x0112 */
    MODE_ENTRY_6_REG(0x0112,
        0x000b,0xff,0x08, /* User Defined */
        0x000c,0xff,0x2d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x51, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_A_212_TX, Mode ID: 0x0121 */
    MODE_ENTRY_7_REG(0x0121,
        0x0003,0x04,0x04, /* Transmit AM (tr_am) ; Use AM modulation */
        0x0068,0x88,0x88, /* User Defined ; Use Resistive AM */
        0x006a,0x7f,0x7f, /* User Defined ; Set Resistive modulation */
        0x0070,0xff,0x40, /* User Defined ; Set default Overshoot Protection */
        0x0071,0xff,0x03, /* User Defined ; Set default Overshoot Protection */
        0x0072,0xff,0x40, /* User Defined ; Set default Undershoot Protection */
        0x0073,0xff,0x03  /* User Defined ; Set default Undershoot Protection */
    ),

    /* Mode Name: POLL_A_212_RX, Mode ID: 0x0122 */
    MODE_ENTRY_6_REG(0x0122,
        0x000b,0xff,0x02, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x14, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_A_424_TX, Mode ID: 0x0131 */
    MODE_ENTRY_7_REG(0x0131,
        0x0003,0x04,0x04, /* Transmit AM (tr_am) ; Use AM modulation */
        0x0068,0x88,0x88, /* User Defined ; Use Resistive AM */
        0x006a,0x7f,0x7f, /* User Defined ; Set Resistive modulation */
        0x0070,0xff,0x40, /* User Defined ; Set default Overshoot Protection */
        0x0071,0xff,0x03, /* User Defined ; Set default Overshoot Protection */
        0x0072,0xff,0x40, /* User Defined ; Set default Undershoot Protection */
        0x0073,0xff,0x03  /* User Defined ; Set default Undershoot Protection */
    ),

    /* Mode Name: POLL_A_424_RX, Mode ID: 0x0132 */
    MODE_ENTRY_6_REG(0x0132,
        0x000b,0xff,0x42, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x54, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_A_848_TX, Mode ID: 0x0141 */
    MODE_ENTRY_7_REG(0x0141,
        0x0003,0x04,0x04, /* Transmit AM (tr_am) ; Use AM modulation */
        0x0028,0xf0,0xf0, /* AM Modulation Index (am_mod) ; Set Modulation index */
        0x0068,0x88,0x00, /* User Defined ; Use AM via regulator */
        0x0070,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0071,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0072,0xff,0x00, /* User Defined ; Disable Undershoot Protection */
        0x0073,0xff,0x00  /* User Defined ; Disable Undershoot Protection */
    ),

    /* Mode Name: POLL_A_848_RX, Mode ID: 0x0142 */
    MODE_ENTRY_6_REG(0x0142,
        0x000b,0xff,0x42, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x44, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_A_ANTICOL, Mode ID: 0x0103 */
    MODE_ENTRY_1_REG(0x0103,
        0x004c,0x40,0x00  /* User Defined ; Set collision detection level different from data */
    ),

    /* Mode Name: POLL_B_COMMON_RX, Mode ID: 0x0202 */
    MODE_ENTRY_1_REG(0x0202,
        0x000a,0x04,0x00  /* Receiver Operation Selection (dis_corr) : Correlation operation ; Use Correlator Receiver */
    ),

    /* Mode Name: POLL_B_106_RX, Mode ID: 0x0212 */
    MODE_ENTRY_6_REG(0x0212,
        0x000b,0xff,0x04, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x1b, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_B_212_RX, Mode ID: 0x0222 */
    MODE_ENTRY_6_REG(0x0222,
        0x000b,0xff,0x02, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x14, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_B_424_RX, Mode ID: 0x0232 */
    MODE_ENTRY_6_REG(0x0232,
        0x000b,0xff,0x42, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x54, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_B_848_RX, Mode ID: 0x0242 */
    MODE_ENTRY_6_REG(0x0242,
        0x000b,0xff,0x42, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x44, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_F_COMMON_RX, Mode ID: 0x0402 */
    MODE_ENTRY_7_REG(0x0402,
        0x000a,0x04,0x00, /* Receiver Operation Selection (dis_corr) : Correlation operation ; Use Correlator Receiver */
        0x000b,0xff,0x13, /* User Defined */
        0x000c,0xff,0x3d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x54, /* User Defined */
        0x004d,0xff,0x00  /* User Defined */
    ),

    /* Mode Name: POLL_V_1OF4_TX, Mode ID: 0x10c1 */
    MODE_ENTRY_1_REG(0x10c1,
        0x0003,0x04,0x00  /* Transmit AM (tr_am) : OOK ; Use OOK */
    ),

    /* Mode Name: POLL_V_COMMON_RX, Mode ID: 0x1002 */
    MODE_ENTRY_7_REG(0x1002,
        0x000a,0x04,0x00, /* Receiver Operation Selection (dis_corr) : Correlation operation ; Use Correlator Receiver */
        0x000b,0xff,0x13, /* User Defined */
        0x000c,0xff,0x2d, /* User Defined */
        0x000d,0xff,0x00, /* User Defined */
        0x000e,0xff,0x00, /* User Defined */
        0x004c,0xff,0x13, /* User Defined */
        0x004d,0xff,0x01  /* User Defined */
    ),

    /* Mode Name: POLL_AP2P_106_TX, Mode ID: 0x0811 */
    MODE_ENTRY_5_REG(0x0811,
        0x0003,0x04,0x00, /* Transmit AM (tr_am) : OOK ; Use OOK modulation */
        0x0070,0xff,0x40, /* User Defined ; Set default Overshoot Protection */
        0x0071,0xff,0x03, /* User Defined ; Set default Overshoot Protection */
        0x0072,0xff,0x40, /* User Defined ; Set default Undershoot Protection */
        0x0073,0xff,0x03  /* User Defined ; Set default Undershoot Protection */
    ),

    /* Mode Name: POLL_AP2P_212_TX, Mode ID: 0x0821 */
    MODE_ENTRY_1_REG(0x0821,
        0x0003,0x04,0x04  /* Transmit AM (tr_am) ; Use AM modulation */
    ),

    /* Mode Name: POLL_AP2P_424_TX, Mode ID: 0x0831 */
    MODE_ENTRY_1_REG(0x0831,
        0x0003,0x04,0x04  /* Transmit AM (tr_am) ; Use AM modulation */
    ),

    /* Mode Name: LISTEN_ON, Mode ID: 0x0006 */
    MODE_ENTRY_6_REG(0x0006,
        0x0026,0xff,0x00, /* User Defined ; Set Antenna Tuning (Listener): NFC06A1 */
        0x0027,0xff,0xff, /* User Defined ; Set Antenna Tuning (Listener): NFC06A1 */
        0x0070,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0071,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0072,0xff,0x00, /* User Defined ; Disable Undershoot Protection */
        0x0073,0xff,0x00  /* User Defined ; Disable Undershoot Protection */
    ),

    /* Mode Name: LISTEN_AP2P_COMMON_TX, Mode ID: 0x8801 */
    MODE_ENTRY_7_REG(0x8801,
        0x0026,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0027,0xff,0x82, /* User Defined ; Set Antenna Tuning (Poller): NFC06A1 */
        0x0028,0xf0,0x70, /* AM Modulation Index (am_mod) ; Set Modulation index */
        0x0070,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0071,0xff,0x00, /* User Defined ; Disable Overshoot Protection */
        0x0072,0xff,0x00, /* User Defined ; Disable Undershoot Protection */
        0x0073,0xff,0x00  /* User Defined ; Disable Undershoot Protection */
    ),

    /* Mode Name: LISTEN_AP2P_106_TX, Mode ID: 0x8811 */
    MODE_ENTRY_5_REG(0x8811,
        0x0003,0x04,0x00, /* Transmit AM (tr_am) : OOK ; Use OOK modulation */
        0x0070,0xff,0x40, /* User Defined ; Set default Overshoot Protection */
        0x0071,0xff,0x03, /* User Defined ; Set default Overshoot Protection */
        0x0072,0xff,0x40, /* User Defined ; Set default Undershoot Protection */
        0x0073,0xff,0x03  /* User Defined ; Set default Undershoot Protection */
    ),

    /* Mode Name: LISTEN_AP2P_212_TX, Mode ID: 0x8821 */
    MODE_ENTRY_1_REG(0x8821,
        0x0003,0x04,0x04  /* Transmit AM (tr_am) ; Use AM modulation */
    ),

    /* Mode Name: LISTEN_AP2P_424_TX, Mode ID: 0x8831 */
    MODE_ENTRY_1_REG(0x8831,
        0x0003,0x04,0x04  /* Transmit AM (tr_am) ; Use AM modulation */
    )

};

const uint16_t rfalAnalogConfigCustomSettingsLength = sizeof(rfalAnalogConfigCustomSettings);
