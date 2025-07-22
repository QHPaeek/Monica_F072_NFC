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
 *  \brief Implementation of authentication from card
 *
 */

#ifndef MCC_CRYPTO_CARD_H
#define MCC_CRYPTO_CARD_H

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "mcc_crypto.h"

/*
 ******************************************************************************
 * GLOBAL DATATYPES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 ******************************************************************************
 */

/*!
 ******************************************************************************
 * \brief Card authentication, 1st step
 *
 * Initialises internal variables with \a uid and \a nonce.
 *
 * \param handle Pointer to the handle data
 * \param uid Given UID
 * \param nonce Nonce set by the card
 ******************************************************************************
 */
void mccCryptoAuthCardStep1 ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce );

/*!
 ******************************************************************************
 * \brief Card authentication, 2nd step
 *
 * This step calculates the expected response, based on the reader challenge
 * and compares it with the actual response from the reader.
 *
 * \param handle Pointer to the handle data
 * \param response Pointer to the 4 byte challenge from the reader and the 4 bytes response
 *
 * \return 1 : success
 * \return 0 : expected and actual response are not equal
 ******************************************************************************
 */
int mccCryptoAuthCardStep2 ( CryptoHandle_t *handle, uint16_t reader_challenge[4], uint16_t actual_response[4] );

/*!
 ******************************************************************************
 * \brief Card authentication, 3rd step
 *
 * Calculates the tag response.
 *
 * \param handle Pointer to the handle data
 * \param response Pointer to the 4 byte response of the card
 *
 * \return 1 : success
 * \return 0 : expected and actual response are not equal
 ******************************************************************************
 */
int mccCryptoAuthCardStep3 ( CryptoHandle_t *handle, uint16_t response[4] );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MCC_CRYPTO_CARD_H
