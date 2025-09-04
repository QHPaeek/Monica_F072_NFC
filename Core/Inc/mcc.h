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
 *  \brief MCC layer implementation
 *
 */

#ifndef MCC_H
#define MCC_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include <stdint.h>

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define MCC_BUFFER_SIZE         (64)    /* !< Buffer size for transmissions*/

#define MCC_AUTH_KEY_A                   (0x60) /* !< Authenticate Key A*/
#define MCC_AUTH_KEY_B                   (0x61) /* !< Authenticate Key B*/
#define MCC_READ_BLOCK                   (0x30) /* !< Command Read Block*/
#define MCC_WRITE_BLOCK                  (0xA0) /* !< Command Write Block*/
#define MCC_ACK                          (0x0A) /* !< Response ACK*/
#define MCC_NACK_NOT_ALLOWED             (0x04) /* !< Response NACK, reason not allowed*/
#define MCC_NACK_TRANSMISSION_ERROR      (0x04) /* !< Response NACK, transmission error*/

/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */

/*!
 *****************************************************************************
 * \brief Initialisation of MCC
 *
 * Setup of ISO 14443A for NFC usage.
 *
 * \return ERR_NONE : success.
 * \return A negative number in case of an error.
 *****************************************************************************
 */
int8_t mccInitialize ( void );

/*!
 *****************************************************************************
 * \brief Deinitialisation of MCC
 *
 * Tear down of the initialised ISO 14443A.
 *
 * \param keep_on Passed to iso14443ADeinitialize, define whether RF field 
 *                should be kept ON.
 *
 * \return ERR_NONE : success.
 * \return A negative number in case of an error.
 *****************************************************************************
 */
int8_t mccDeinitialise ( uint8_t keep_on );

/*!
 *****************************************************************************
 * Set the secret key
 *
 * \note The array pointed to by \a key must be at least 6 bytes long.
 *
 * \param key Pointer to an array holding the secret key.
 *****************************************************************************
 */
void mccSetKey ( const uint8_t key[6] );

/*!
 *****************************************************************************
 * \brief Reset the cipher
 *
 * Reset the crypto engine and deactivate de-/encryption for communication.
 * This must be called prior to communication with a new card.
 *****************************************************************************
 */
void mccResetCipher ( void );

/*!
 *****************************************************************************
 * \brief Perform a complete authentication
 *
 * This function is provided as a convenient interface to mccAuthenticateStep1
 * and mccAuthenticateStep2.
 * The reader nonce used in step2 of the authentication is defined by
 * the given nonce
 *
 * \param keySelect Select key for authentication
 * \param block Block to authenticate
 * \param uid Pointer to the UID of the card for authentication
 * \param uidLength Length of the UID pointed to by \q uid in bytes
 * \param key Pointer to the key.
 * \param nonce 32-bit nonce (random challenge)
 *
 * \return ERR_NONE : authentication steps successfull
 * \return A negative number in case of an error.
 *****************************************************************************
 */
int8_t mccAuthenticate ( uint8_t keySelect,
                         uint8_t block,
                         uint8_t *uid,
                         uint8_t uidLength,
                         uint8_t key[6],
                         uint32_t nonce );

/*!
 *****************************************************************************
 * \brief First step of authentication from the reader side.
 *
 * \note The array pointed to by \a uid must be at least 4 bytes long.
 *
 * \param keySelect Select key for authentication
 * \param block Block to authenticate
 * \param uid Pointer to the UID of the card for authentication
 * \param uidLength Length of the UID pointed to by \q uid in bytes
 * \param key Pointer to the key.
 *
 * \return ERR_NONE : authentication step was successfull.
 * \return ERR_PARAM : UID to short, or no key given
 * \return ERR_NOTFOUND : No data from card received
 *****************************************************************************
 */
int8_t mccAuthenticateStep1 ( const uint8_t keySelect,
                              uint8_t block,
                              const uint8_t *uid,
                              uint8_t uidLength,
                              const uint8_t key[6] );

/*!
 *****************************************************************************
 * \brief Second step of the authentication from the reader side.
 *
 * \param nonce Nonce generated by the reader.
 *
 * \return ERR_NONE : authentication step was successfull.
 * \return ERR_NOTFOUND : No data from card received.
 *****************************************************************************
 */
int8_t mccAuthenticateStep2 ( uint32_t nonce );

/*!
 *****************************************************************************
 * \brief Send encrypted data to the card
 *
 * \param request Pointer to the request data
 * \param requestLength Length of the content in \a request pointer
 * \param response Pointer to the memory location where the received data is written to
 * \param maxResponseLength Maximum memory the received data can occupy
 * \param responseLength Actual data received
 * \param timeout Time in [ms] the operation may take
 * \param fourBitResponse Indicating that the response only has four bits
 *
 * \return ERR_NONE : success
 * \return A negative number in case of an error.
 *****************************************************************************
 */
int8_t mccSendRequest ( const uint8_t *request,
                        uint16_t requestLength,
                        uint8_t *response,
                        uint16_t maxResponseLength,
                        uint16_t *responseLength,
                        uint16_t timeout,
                        uint8_t fourBitResponse );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MCC_H
