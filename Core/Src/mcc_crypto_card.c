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
 *  \brief MCC layer implementation for authentication as a card
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "mcc_crypto.h"
#include "mcc_crypto_card.h"
#include "mcc_macros.h"

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */

#if 0
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

void mccCryptoAuthCardStep1 ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce )
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

int mccCryptoAuthCardStep2 ( CryptoHandle_t *handle, uint16_t *reader_challenge, uint16_t* actual_response )
{
    uint32_t reader_challenge_wo_p = TO_uint32_t( reader_challenge );
    uint32_t actual_response_wo_p = TO_uint32_t( actual_response );
    int rv = 0;

    mccLfsrUpdate( handle, reader_challenge_wo_p, 1 );

    uint32_t expected_response = mccPrng( handle, 64 ) ^ mccLfsrUpdate( handle, 0, 0 );

    if ( expected_response == actual_response_wo_p ) 
    {
        rv = 1;
    }
    return rv;
}

int mccCryptoAuthCardStep3 ( CryptoHandle_t *handle, uint16_t *response )
{
    uint32_t tag_response = mccPrng( handle, 96 );
    uint32_t_TO_ARRAY_PARITY( tag_response, response );

    mccCryptoTranscode( handle, response, 4, 0 );

    return 1;
}
