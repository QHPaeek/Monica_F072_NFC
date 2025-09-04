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
 *  \brief Implementation of authentication from reader
 *
 */

#ifndef MCC_CRYPTO_READER_H
#define MCC_CRYPTO_READER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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
 * \brief Reader authentication, step 1
 *
 * Initialises internal variables with \a uid and \a nonce.
 *
 * \param handle Pointer to the handle data
 * \param uid Given UID from card
 * \param nonce Nonce set by the card
 ******************************************************************************
 */
void mccCryptoAuthReaderStep1 ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce );

/*!
 ******************************************************************************
 * \brief Reader authentication, step 2
 *
 * Calculates the reader response from the challenge of the card.
 *
 * \param handle Pointer to the handle data
 * \param challenge Pointer to response
 * \param response Pointer to response
 *
 * \return 1 : success
 * \return 0 : failure
 */
int mccCryptoAuthReaderStep2 ( CryptoHandle_t *handle, uint16_t challenge[4], uint16_t response[4] );

/*!
 ******************************************************************************
 * \brief Reader authentication, step 3
 *
 * Compares the response from the tag with an calculated expected response.
 *
 * \note \a response must be at least 4 bytes wide.
 *
 * \param handle Pointer to the handle data
 * \param response Pointer to response
 *
 * \return 1 : success
 * \return 0 : expected and actual response are not equal
 ******************************************************************************
 */
int mccCryptoAuthReaderStep3 ( CryptoHandle_t *handle, uint16_t response[4] );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MCC_CRYPTO_READER_H
