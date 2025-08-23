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
 *  \brief MCC crypto implementation
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "mcc_crypto.h"
#include "mcc_crypto_card.h"
#include "mcc_crypto_reader.h"
#include "mcc_macros.h"

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */
//#if 0
//#define MCC_DBG logUsart
//#define MCC_DUMP dbgHexDump
//#else
#define MCC_DBG(...)
#define MCC_DUMP(...)
//#endif

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

/* Split feedback polynomial */
#define FEEDBACK_ODD    (0x29CE5C)
#define FEEDBACK_EVEN   (0x870804)

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

#ifdef DEBUG_LFSR
/*!
 *****************************************************************************
 * Reconstruct LFSR by shifting lfsr_odd and lfsr_even into the correct positions
 *
 * \return LFSR value
 *****************************************************************************
 */
static uint64_t reconstructLfsr ( CryptoHandle_t *handle );
#endif // DEBUG_LFSR

/*!
 *****************************************************************************
 * Shift even parameters by one and add parity bit to the last position
 * adapt odd and even bytes accordingly to this
 * 
 * Bits are sparately stored in odd and even.
 * 1. Shift even one to the left and add parity
 * 2. XOR with odd values, this gives us the "negative" of how even should look like
 * 3. XOR with even values, which results in the true even values
 * 4. XOR with odd values, even values are correct -> xor-ing provides true odd values
 *
 * This swaps the values as it can be seen at http://aggregate.org/MAGIC/#Bit%20Reversal
 * (Swap Values Without a Temporary)
 *
 * \param handle Pointer to the internal used variables
 * \param parityBit Bit to be introduced into LFSR
 *****************************************************************************
 */
static void correctLfsrValues ( CryptoHandle_t * handle, uint32_t parityBit );


/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */

uint8_t mccLfsrUpdateBit ( CryptoHandle_t * const handle, uint8_t dataBit, uint8_t useKeyStream )
{
    uint32_t u = 0;
    uint32_t k = FILTER( handle->lfsr_odd );
    uint32_t p;

#ifdef DEBUG_LFSR
    handle->lfsr_lfsr = reconstruct_lfsr( handle );
    MCC_DBG( "D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
                                );
#endif // DEBUG_LFSR
    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

    u  = handle->lfsr_odd & FEEDBACK_ODD;
    u ^=  handle->lfsr_even & FEEDBACK_EVEN;
    u ^= (k & useKeyStream) ^ dataBit;

    p = mccParityEven(u);

    MCC_DBG("D: parity = 0x%x\n", p);
    MCC_DBG("D: u = 0x%x%x\n", (uint16_t)(u>>16), (uint16_t)u);

    correctLfsrValues( handle, p );

    return k;
}

uint8_t mccLfsrUpdateByte ( CryptoHandle_t *handle, uint8_t data, uint8_t useKeyStream )
{
    int i;
    uint8_t rv = 0;

    // Update the lfsr with each data bit
    for ( i = 0; i < 8; i++ )
    {
        rv |= mccLfsrUpdateBit( handle, BIT(data, i), useKeyStream ) << i;
    }

    return rv;
}

uint32_t mccLfsrUpdate ( CryptoHandle_t *handle, uint32_t data, uint8_t useKeyStream )
{
    int i;
    uint32_t rv = 0;

    for ( i = 3; i >= 0; i-- )
    {
        uint16_t offset = i*0x08;
        rv |= (uint32_t)mccLfsrUpdateByte(handle, ((data >> offset) & 0xFF), useKeyStream) << offset;
    }


    return rv;
}

uint32_t mccPrng ( CryptoHandle_t *handle, uint32_t steps )
{
    uint32_t i;
    uint32_t prngNew = handle->prng;
    prngNew = mccReverse( prngNew );

    for ( i = 0; i < steps; i++ )
    {
        prngNew = (prngNew << 0x01)
                     | (
                         (
                             (prngNew >> 15) 
                           ^ (prngNew >> 13)
                           ^ (prngNew >> 12)
                           ^ (prngNew >> 10)
                         )
                         & 0x01 // make sure we take only one bit
                      );
    }

    return mccReverse(prngNew);
}

void mccCryptoInit ( CryptoHandle_t *handle, uint64_t key )
{
    // PRNG cannot be loaded at this time. This will be done
    // when the first part of the authentication is starting
    // (aka challenge from tag), where the PRNG receives its
    // true initial value (uid ^ card_challenge)
    handle->prng = 0;

    int i = 0;
    handle->lfsr_odd = 0;
    handle->lfsr_even = 0;
#ifdef DEBUG_LFSR
    handle->lfsr_lfsr = 0;
#endif // DEBUG_LFSR


    for( i = 47; i > 0; i -= 2 )
    {
        handle->lfsr_odd  = (handle->lfsr_odd << 1)  | BIT( key, (i - 1) ^ 7 );
        handle->lfsr_even = (handle->lfsr_even << 1) | BIT( key, i ^ 7 );
    }

    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

#ifdef DEBUG_LFSR
    uint64_t lfsr = reconstruct_lfsr( handle );
    MCC_DBG( "D: lfsr = 0x%x%x%x%x\n", (uint16_t)(lfsr >> 48), (uint16_t)(lfsr >> 32), (uint16_t)(lfsr >> 16), (uint16_t)(lfsr));
    handle->lfsr_lfsr = lfsr;
#endif // DEBUG_LFSR

    MCC_DBG( "--- end crypto init\n" );
}

void mccCryptoReset ( CryptoHandle_t *handle, uint8_t keepActive )
{
    handle->prng = 0;
    handle->lfsr_odd = 0;
    handle->lfsr_even = 0;

#ifdef DEBUG_LFSR
    handle->lfsr_lfsr = 0;
#endif // DEBUG_LFSR
    
    if ( keepActive != 1 )
    {
        handle->is_active = 0;
    }
}

void mccCryptoAuthInit ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce )
{
    handle->prng = nonce;
    MCC_DBG( "D: nonce: %x%x\n", (uint16_t)(nonce >> 16), (uint16_t)(nonce) );
    MCC_DBG( "D: uid: %x%x\n", (uint16_t)(uid >> 16), (uint16_t)(uid) );

    mccLfsrUpdate( handle, uid ^ nonce, 0 );

    MCC_DBG( "D: PRNG = 0x%x\n", handle->prng );
    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );
#ifdef DEBUG_LFSR
    MCC_DBG( "D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
                                );
#endif // DEBUG_LFSR

    MCC_DBG( "--- end auth init\n" );
}

void mccCryptoAuthReinit ( CryptoHandle_t *handle, uint32_t uid, uint32_t nonce )
{
    uint32_t ks = mccLfsrUpdate( handle, nonce ^ uid, 1 );
    uint32_t nonce_decrypted = nonce ^ ks;

    handle->prng = nonce_decrypted;

    MCC_DBG( "D: PRNG = 0x%x\n", handle->prng );
    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );

#ifdef DEBUG_LFSR
    MCC_DBG( "D: LFSR:  %x %x %x %x\n", 
                                  (uint16_t)(handle->lfsr_lfsr >> 48),
                                  (uint16_t)(handle->lfsr_lfsr >> 32),
                                  (uint16_t)(handle->lfsr_lfsr >> 16),
                                  (uint16_t)(handle->lfsr_lfsr >> 0)
           );
#endif // DEBUG_LFSR

    MCC_DBG( "--- reinit end\n" );
}

void mccCryptoTranscode ( CryptoHandle_t *handle, uint16_t * data_w_parity, uint16_t bytes, uint8_t bits )
{
    int i=0;
    
    for ( i=0; i < bytes; i++ )
    {
        data_w_parity[i] ^= mccLfsrUpdateByte( handle, 0, 0 );
        data_w_parity[i] ^= (FILTER(handle->lfsr_odd) << 0x08);
    }

    for ( i = 0; i < bits; i++ )
    {
        data_w_parity[bytes] ^= mccLfsrUpdateBit( handle, 0, 0 ) << i;
    }
}

uint8_t mccCryptoIsActive ( CryptoHandle_t *handle )
{
    return handle->is_active;
}

void mccCryptoSetActive ( CryptoHandle_t *handle, uint8_t value )
{
    handle->is_active = value;
}

/*
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 */

#ifdef DEBUG_LFSR
static uint64_t reconstructLfsr ( CryptoHandle_t *handle )
{
    uint64_t lfsr = 0;
    int i = 0;

    for ( i = 23; i >= 0; i-- )
    {
        lfsr = lfsr << 1 | BIT( handle->lfsr_odd, i^3 );
        lfsr = lfsr << 1 | BIT( handle->lfsr_even, i^3 );
    }

    lfsr = (((lfsr & 0x00FF00FF00FF00FF) <<  8) | ((lfsr & 0xFF00FF00FF00FF00) >>  8));
    lfsr = (((lfsr & 0x0000FFFF0000FFFF) << 16) | ((lfsr & 0xFFFF0000FFFF0000) >> 16));
    lfsr = ((lfsr >> 32) | (lfsr << 32));
    lfsr >>= 16;

    return lfsr;
}
#endif // DEBUG_LFSR

static void correctLfsrValues ( CryptoHandle_t * handle, uint32_t parityBit )
{
    handle->lfsr_even = handle->lfsr_even << 1 | parityBit;
    handle->lfsr_odd ^= handle->lfsr_even;
    handle->lfsr_even ^= handle->lfsr_odd;
    handle->lfsr_odd ^= handle->lfsr_even;
}
