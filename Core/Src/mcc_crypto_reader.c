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
 *  \brief MCC layer implementation for authentication as a reader
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "mcc_crypto.h"
#include "mcc_crypto_reader.h"
#include "mcc_macros.h"
#include "logger.h"
/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */

#if 1
#define MCC_DBG logUsart
#define MCC_DUMP dbgHexDump
#else
#define MCC_DBG(...)
#define MCC_DUMP(...)
#endif

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */

/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */

void mccCryptoAuthReaderStep1 ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce )
{
    if ( handle->is_active != 0 )
    {
        mccCryptoAuthReinit( handle, uid, nonce );
    }
    else
    {
        mccCryptoAuthInit( handle, uid, nonce );
    }
}

int mccCryptoAuthReaderStep2 ( CryptoHandle_t *handle, uint16_t challenge[4], uint16_t response[4] )
{
    uint32_t nonce = TO_uint32_t( challenge );
    uint32_t readerResponse;
    int i;

    MCC_DBG( "I: PRNG = 0x%x\n", handle->prng );
    MCC_DBG( "I: nonce: 0x%x%x\n", (uint16_t)(nonce >> 16), (uint16_t)(nonce) );
    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

#ifdef DEBUG_LFSR
    MCC_DBG("D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
                                );
#endif // DEBUG_LFSR

    for ( i = 3; i >= 0; i-- )
    {
        challenge[3-i] ^= mccLfsrUpdateByte( handle, (nonce >> (i*8)) & 0xFF, 0 );
        challenge[3-i] ^= FILTER( handle->lfsr_odd) << 0x08;
    }

    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

#ifdef DEBUG_LFSR
    MCC_DBG("D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
                                );
#endif // DEBUG_LFSR

    readerResponse = mccPrng( handle, 64 );

    // Reuse response to send data, add it after the response from the tag
    uint32_t_TO_ARRAY_PARITY( readerResponse, response );

    mccCryptoTranscode( handle, response, 4, 0 );

    return 1;
}

int mccCryptoAuthReaderStep3 ( CryptoHandle_t *handle, uint16_t response[4] )
{
    uint32_t tag_response = TO_uint32_t( response );
    uint32_t expected_response;
    int rv = 0;

    MCC_DBG("D: PRNG: 0x%x\n", handle->prng);

    expected_response = mccPrng( handle, 96 ) ^ mccLfsrUpdate( handle, 0, 0 );

    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

#ifdef DEBUG_LFSR
    MCC_DBG("D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
                                );
#endif // DEBUG_LFSR

    MCC_DBG("D: response: tag = 0x%x%x\n", (uint16_t)(tag_response >> 16), (uint16_t)tag_response);
    MCC_DBG("D: response: exp = 0x%x%x\n", (uint16_t)(expected_response >> 16), (uint16_t)expected_response);

    if ( tag_response == expected_response )
    {
        rv = 1;
    }
    
    return rv;
}
