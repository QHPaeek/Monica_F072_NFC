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
 *  \brief Crypto implementation
 *
 */

#ifndef MCC_CRYPTO_H
#define MCC_CRYPTO_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "stdint.h"

/*
 ******************************************************************************
 * GLOBAL DATATYPES
 ******************************************************************************
 */
struct _CryptoHandle_t;
typedef struct _CryptoHandle_t CryptoHandle_t;

struct _CryptoHandle_t
{
    uint16_t prng;           /*!< pseudo random number generator */
    uint8_t is_active;       /*!< Indicates wether this handle is already initialised and in use */
    uint32_t lfsr_odd;       /*!< Odd bits of LFSR*/
    uint32_t lfsr_even;      /*!< Even bits of LFSR*/
#ifdef DEBUG_LFSR
    uint64_t lfsr_lfsr;      /*!< LFSR for debugging reasons*/
#endif // DEBUG_LFSR
};

/*
 ******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 ******************************************************************************
 */

/*! 
 *****************************************************************************
 * \brief Updates LFSR and calculates the new keystream for a bit
 *
 * Updates LFSR and calculates a new keystream. The old
 * keystream can be feed back into the LFSR if useKeyStream is set to a value
 * other than 0. The function returns the new keystream for this bit.
 *
 * \param handle Pointer to the handle data
 * \param dataBit byte containing the bit to be shifted into the LFSR
 * \param useKeyStream Deceides if keystream is also shifted back into LFSR
 *
 * \return New keystream
 *****************************************************************************
 */
uint8_t mccLfsrUpdateBit ( CryptoHandle_t * const handle, uint8_t dataBit, uint8_t useKeyStream );

/*! 
 *****************************************************************************
 * \brief Update LFSR and calculate new keystream values for a byte
 *
 * Splits up \a data into bits and calls \a mccLfsrUpdateBit for each bit.
 * The new keystream is then returned.
 *
 * \param handle Pointer to the handle data
 * \param data byte containing the data to be shifted into the LFSR
 * \param useKeyStream Deceides if keystream is also shifted back into LFSR
 *
 * \return New keystream
 *****************************************************************************
 */
uint8_t mccLfsrUpdateByte ( CryptoHandle_t *handle, uint8_t data, uint8_t useKeyStream );

/*! 
 *****************************************************************************
 * \brief Update LFSR and calculate new keystream values
 *
 * Splits up \a data into bytes and calls \a mccLfsrUpdateByte
 * for each byte. The new keystream is returned.
 *
 * \param handle Pointer to the handle data
 * \param data 32 bits of data to be shifted into the LFSR
 * \param useKeyStream Deceides if keystream is also shifted back into LFSR
 *
 * \return New keystream
 *****************************************************************************
 */
uint32_t mccLfsrUpdate ( CryptoHandle_t *handle, uint32_t data, uint8_t useKeyStream );

/*! 
 *****************************************************************************
 * \brief Calculate next PRNG value 
 *
 * \param handle Pointer to the handle data
 * \param steps Number of steps to proceed
 *
 * \return New PRNG value.
 *****************************************************************************
 */
uint32_t mccPrng ( CryptoHandle_t *handle, uint32_t steps );

/*! 
 *****************************************************************************
 * \brief Initialise crypto
 *
 * Sets the internal variables to valid values.
 * PRNG is reset to 0 and LFSR is set to
 * the key, which is shifted in, in reverse order.
 *
 * \param handle Pointer to the handle data
 * \param key Secret key
 *****************************************************************************
 */
void mccCryptoInit ( CryptoHandle_t *handle, uint64_t key );

/*! 
 *****************************************************************************
 * \brief Reset all internal variables of crypto .
 *
 * Resets the crypto implementation to its default values.
 * If \a keepActive is set, then only the prng and lfsr are reset,
 * but the reader is kept active.
 *
 * \param handle Pointer to the handle data
 * \param keepActive set to value other than 0 if only variables should be reset, but not the state.
 *****************************************************************************
 */
void mccCryptoReset ( CryptoHandle_t *handle, uint8_t keepActive );

/*! 
 *****************************************************************************
 * \brief Initialise authentication between card and reader.
 *
 * LFSR is updated with uid XOR challenge, while the PRNG is set to challenge
 *
 * \param handle Pointer to the handle data
 * \param uid Given UID from card or reader
 * \param nonce Nonce received from the card
 *****************************************************************************
 */
void mccCryptoAuthInit ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce );

/*! 
 *****************************************************************************
 * \brief Reinitialisation of authentication between card and reader.
 *
 * Calculates the new keystream and xors it with nonce to get the
 * next valid PRNG value.
 *
 * \param handle Pointer to the handle data
 * \param uid Given UID from card or reader
 * \param nonce Nonce received from the card
 *****************************************************************************
 */
void mccCryptoAuthReinit ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce );

/*! 
 *****************************************************************************
 * \brief  De-/Encrypt data
 *
 * \param handle Pointer to the handle data
 * \param data_w_parity Pointer to data with parity bits
 * \param bytes Number of bytes in \a data_w_parity
 * \param bits Number of bits left in \a data_w_parity
 *****************************************************************************
 */
void mccCryptoTranscode ( CryptoHandle_t *handle, uint16_t * data_w_parity, uint16_t bytes, uint8_t bits );

/*! 
 *****************************************************************************
 * \brief Check if crypto is initialised and active. 
 *
 * \param handle Pointer to the handle data
 *
 * \return 0 : inactive
 * \return 1 : active
 *****************************************************************************
 */
uint8_t mccCryptoIsActive ( CryptoHandle_t *handle );

/*! 
 *****************************************************************************
 * \brief Sets the state to \a value.
 *
 * Depending on the value of \a value the internal state will be set.
 *
 * \param handle Pointer to the handle data
 * \param value Describes the state. 0 is inactive, while 1 is active
 *****************************************************************************
 */
void mccCryptoSetActive ( CryptoHandle_t *handle, uint8_t value );


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // MCC_CRYPTO_H
