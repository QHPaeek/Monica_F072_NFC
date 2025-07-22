/*
 *****************************************************************************
 * Copyright by ams AG                                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************
 */
/*
 *      PROJECT:   AS3911 firmware
 *      $Revision: $
 *      LANGUAGE:  ANSI C
 */

/*! \file
 *
 *  \author G. Wagner
 *
 *  \brief Macros and utility functions
 *
 */

#ifndef MCC_MACROS_H
#define MCC_MACROS_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "stdint.h"

/*
 ******************************************************************************
 * DEFINES & MACROS
 ******************************************************************************
 */

/*!
 ******************************************************************************
 * \brief Extract a bit from a given variable
 *
 * \param x Variable containing the desired bit
 * \param b Number of the bit to be extracted
 ******************************************************************************
 */
#define BIT(x, b) ( ( (x) >> (b) ) & 0x01 )

/*!
 ******************************************************************************
 * \brief MCC filter function
 *
 * Calculates filter function from a 64 bit variable
 *
 * \param x 64 bit variable containing the data to be filtered
 ******************************************************************************
 */
#define FILTER( x ) \
        BIT( 0xEC57E80A, \
               (  ((0xf22c0 >> (x       & 0xf)) & 16) \
                | ((0x6c9c0 >> (x >>  4 & 0xf)) &  8) \
                | ((0x3c8b0 >> (x >>  8 & 0xf)) &  4) \
                | ((0x1e458 >> (x >> 12 & 0xf)) &  2) \
                | ((0x0d938 >> (x >> 16 & 0xf)) &  1) \
               ) \
           )

/*!
 ******************************************************************************
 * \brief Creates an uint32_t variable out of an array of 16 bit types (
 * tossing parity).
 *
 * The array must contain at least 4 fields. Offset 0
 * is the MSB.
 *
 * \param a Array containing the data for conversion
 ******************************************************************************
 */
#define TO_uint32_t(a) ( \
                         ((uint32_t)((a)[3] & 0xFF) <<  0) \
                       | ((uint32_t)((a)[2] & 0xFF) <<  8) \
                       | ((uint32_t)((a)[1] & 0xFF) << 16) \
                       | ((uint32_t)((a)[0] & 0xFF) << 24) \
                       )

/*!
 ******************************************************************************
 * \brief Creates an array (4 entries) out of an uint32_t with parity
 *
 * Bytes need to be shifted to the correct offset, then calculate
 * parity over the bytes
 *
 * \param u 32 bit variable
 * \param a 4 byte array
 ******************************************************************************
 */
#define uint32_t_TO_ARRAY_PARITY(u, a) \
    do { \
        uint16_t b; \
        b = ((u) >>  0) & 0xFF; (a)[3] = b | (mccParityOdd(b) << 8); \
        b = ((u) >>  8) & 0xFF; (a)[2] = b | (mccParityOdd(b) << 8); \
        b = ((u) >> 16) & 0xFF; (a)[1] = b | (mccParityOdd(b) << 8); \
        b = ((u) >> 24) & 0xFF; (a)[0] = b | (mccParityOdd(b) << 8); \
    } while ( 0 )


/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */

/*!
 ******************************************************************************
 * \brief Reverses all bits from input
 *
 * Converts MSB first into LSB first and vice versa.
 *
 * \param x Input to be reversed
 *
 * \return Bit reversed value of x
 ******************************************************************************
 */
uint32_t mccReverse ( register uint32_t x );

/*!
 ******************************************************************************
 * \brief Calculate even parity
 *
 * \param val Value from which parity is calculated from
 *
 * \return 1 : Even parity
 * \return 0 : Odd parity
 ******************************************************************************
 */
uint32_t mccParityEven ( uint32_t val );

/*!
 ******************************************************************************
 * \brief Calculate odd parity
 *
 * \param val Value from which parity is calculated from
 *
 * \return 1 : Odd parity
 * \return 0 : Even parity
 ******************************************************************************
 */
uint32_t mccParityOdd ( uint32_t val );


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MCC_MACROS_H
