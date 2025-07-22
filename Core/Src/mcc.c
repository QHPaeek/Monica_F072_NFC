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

/*!
 *
 *  \author G. Wagner
 *
 *  \brief MCC layer implementation
 *
 */

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */

#include "rfal_crc.h"
#include "mcc_macros.h"
#include "mcc_crypto.h"
#include "mcc_crypto_reader.h"
#include "mcc_raw_request.h"
#include "mcc.h"
#include "utils.h"
#include "st_errno.h"
#include "rfal_utils.h"

/*
 ******************************************************************************
 * MACROS
 ******************************************************************************
 */

//#if 0
//#define MCC_DBG logUsart
//#define MCC_DUMP dbgHexDump
//else
#define MCC_DBG(...)
#define MCC_DUMP(...)
//#endif
/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define MCC_AUTHENTICATION_STEP1_TIMEOUT    (10)
#define MCC_AUTHENTICATION_STEP2_TIMEOUT    (20)

#define CRC_PRELOAD_VALUE                   (0x6363)
#define CRC_LEN                             (2)

#define AUTH_CMD_LEN    (4)
#define AUTH2_CMD_LEN   (8)
#define AUTH_RSP_LEN    (5)

#define UID_LEN         (4)

#define HIGH_BYTE(x) (((x) >> 8) & 0xFF)
#define LOW_BYTE(x)  ((x)        & 0xFF)

#define MIFARE_BLOCK_SIZE            16
#define MIFARE_CRC_LEN                2
#define MIFARE_READ_TIMEOUT           1

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

uint16_t MifareBuffer[MCC_BUFFER_SIZE];
static CryptoHandle_t mccHandle;
CryptoHandle_t *handle = &mccHandle;

/*
 ******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 ******************************************************************************
 */
static int8_t buildCommand ( uint8_t *cmd, uint8_t keySelect, uint8_t block );
static int8_t attachParityInformation( uint16_t *cmd, size_t length);

/*
 ******************************************************************************
 * GLOBAL FUNCTIONS
 ******************************************************************************
 */
int8_t mccInitialize ( )
{
    int8_t rv = ERR_NONE;

    MCC_DBG( "mcc init\n" );

    mccResetCipher();

    return rv;
}

int8_t mccDeinitialise ( uint8_t keep_on )
{
    int8_t rv = ERR_NONE;

    MCC_DBG( "mcc deinit\n" );

    return rv;
}

void mccSetKey ( const uint8_t key[6] )
{
    uint64_t bigKey = 0;
    unsigned int i;

    MCC_DBG( "mcc set key\n" );

    if ( key == 0 )
    {
        MCC_DBG( "no key passed!\n" );
        return;
    }

    // Copy key into an variable that is big enouth to hold the 
    // 48bits of the key at once
    for ( i = 0; i < 6; i++ )
    {
        bigKey |= ((uint64_t)key[i] << ((5-i)*8));
    }

    MCC_DBG( "  orig key:" );
    MCC_DUMP( key, 6 );
    MCC_DBG( "  trans key: " );
    MCC_DBG( " %x %x %x %x\n", (uint16_t)(bigKey >> 48),
                              (uint16_t)(bigKey >> 32),
                              (uint16_t)(bigKey >> 16),
                              (uint16_t)(bigKey >> 0)
                            );

    mccCryptoInit( handle, bigKey );

}

void mccResetCipher ( )
{
    MCC_DBG( "mcc reset\n" );
    mccCryptoReset( handle, 0 );
}

int8_t mccAuthenticate ( uint8_t keySelect,
                         uint8_t block,
                         uint8_t *uid,
                         uint8_t uidLength,
                         uint8_t key[],
                         uint32_t nonce )
{
    int8_t rv = 0;

    MCC_DBG( "mcc auth\n" );
    
    if ( uid == 0  || uidLength != UID_LEN )
    {
        MCC_DBG( "E: uid incorrect! Expected %d bytes, got %d -> abort!\n", UID_LEN, uidLength );
        MCC_DBG( "I; Failed authentication step 1\n" );
        rv = ERR_PARAM;
        goto out;
    }

    if ( key == 0 )
    {
        MCC_DBG( "E: no key given!\n" );
        MCC_DBG( "I; Failed authentication step 1\n" );
        rv = ERR_PARAM;
        goto out;
    }

    MCC_DBG( "  uid  : %x\n", *uid );
    MCC_DBG( "  block: %x\n", block );

    rv = mccAuthenticateStep1( keySelect, block, uid, uidLength, key );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    rv = mccAuthenticateStep2( nonce );

out:
    return rv;
}

int8_t mccAuthenticateStep1 ( const uint8_t keySelect,
                              uint8_t block,
                              const uint8_t *uid,
                              uint8_t uidLength,
                              const uint8_t key[] )
{
    uint8_t rv = ERR_NONE;
    uint8_t cmd[AUTH_CMD_LEN];
    uint16_t bytesReceived;
    uint8_t rsp[AUTH_RSP_LEN];
    int i;
    
    MCC_DBG( "A: mcc auth step 1\n" );
    if ( uidLength != UID_LEN || uid == 0 )
    {
        MCC_DBG( "E: uid incorrect! Expected %d bytes, got %d -> abort!\n", UID_LEN, uidLength );
        MCC_DBG( "I; Failed authentication step 1\n" );
        rv = ERR_PARAM;
        goto out;
    }

    if ( key == 0 )
    {
        MCC_DBG( "E: no key given!\n" );
        MCC_DBG( "I; Failed authentication step 1\n" );
        rv = ERR_PARAM;
        goto out;
    }

    rv = buildCommand( cmd, keySelect, block );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    for ( i = 0; i < AUTH_CMD_LEN; i++ )
    {
        MifareBuffer[i] = cmd[i];
    }

    rv = attachParityInformation( MifareBuffer, AUTH_CMD_LEN );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    if( mccCryptoIsActive( handle ) == 1 ) 
    {
        mccCryptoTranscode( handle, MifareBuffer, AUTH_CMD_LEN, 0 );
    }
    rv = mccSendRawRequest( MifareBuffer, AUTH_CMD_LEN,
                    rsp, AUTH_RSP_LEN,
                    &bytesReceived, MCC_AUTHENTICATION_STEP1_TIMEOUT, 0 );
    //EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    if ( bytesReceived != AUTH_RSP_LEN - 1 )
    {
        MCC_DBG( "E: received 0x%x bytes, expected %d bytes -> abort!\n", bytesReceived, AUTH_RSP_LEN - 1 );
        MCC_DBG( "I: Failed: Auth Step 1\n" );
        MCC_DUMP( rsp, bytesReceived );
        mccResetCipher( );
        return ERR_NOTFOUND;
    }
    mccCryptoReset( handle, 1 );
    mccSetKey( key );
    mccCryptoAuthReaderStep1( handle, TO_uint32_t( uid ), TO_uint32_t( rsp ) );


    MCC_DBG( "I: uid: %X%X\n", (uint16_t)( TO_uint32_t( uid )>> 16 ), (uint16_t)TO_uint32_t( uid ));
    MCC_DBG( "I: N_c: 0x%X%X\n", (uint16_t)( TO_uint32_t(rsp) >> 16 ), (uint16_t) TO_uint32_t(rsp) );
    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );
#ifdef DEBUG_LFSR
    MCC_DBG( "I: LFSR: 0x%X%X%X%X\n", (uint16_t)( handle->lfsr_lfsr >> 48 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 32 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 16 )
                                    , (uint16_t)( handle->lfsr_lfsr ) );
#endif // DEBUG_LFSR
    MCC_DBG( "I: Success: Auth Step 1\n" );


out:
    return rv;
}

int8_t mccAuthenticateStep2 ( uint32_t nonce )
{
    // Current nonce + generated nonce are in the cmd
    // for step two. Therefore the size is twice as big
    // as before.
    uint16_t cmd[AUTH2_CMD_LEN];
    uint8_t rsp[AUTH_RSP_LEN];
    uint16_t bytesReceived;
    int8_t rv = 0;
    unsigned int i;

    MCC_DBG( "A: mcc auth step 2\n" );
    MCC_DBG( "I: N_r: 0x%x%x\n", (uint16_t)(nonce >> 16), (uint16_t)(nonce) );

    // 1. Set nonce for reader
    uint32_t_TO_ARRAY_PARITY( nonce, cmd );

    // 2. Step 2 of authentication in the crypto algorithm
    mccCryptoAuthReaderStep2( handle, cmd, cmd+4 );

    MCC_DBG( "D: odd = 0x%x%x\n", (uint16_t)(handle->lfsr_odd >> 16), (uint16_t)handle->lfsr_odd );
    MCC_DBG( "D: evn = 0x%x%x\n", (uint16_t)(handle->lfsr_even >> 16), (uint16_t)handle->lfsr_even );
#ifdef DEBUG_LFSR
    MCC_DBG( "I: LFSR: 0x%X%X%X%X\n", (uint16_t)( handle->lfsr_lfsr >> 48 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 32 )
                                    , (uint16_t)( handle->lfsr_lfsr >> 16 )
                                    , (uint16_t)( handle->lfsr_lfsr ) );
#endif // DEBUG_LFSR

    // 2. send it back to the card
    rv = mccSendRawRequest( cmd, AUTH2_CMD_LEN,
                     rsp, AUTH_RSP_LEN,
                     &bytesReceived, MCC_AUTHENTICATION_STEP2_TIMEOUT, 0 );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    // 2.1 check if response contains 4 bytes only
    if ( bytesReceived != 4 )
    {
        MCC_DBG( "E: received 0x%x bytes, expected 4 bytes -> abort!\n", bytesReceived );
        MCC_DBG( "I: Failed: Auth Step 2\n" );
        MCC_DUMP( rsp, bytesReceived );
        mccResetCipher( );
        return ERR_NOTFOUND;
    }

    // 3. convert back
    for ( i = 0; i < AUTH_RSP_LEN; i ++ )
    {
        MifareBuffer[i] = rsp[i];
    }

    // 4. feed it to the crypto algorithm
    rv = mccCryptoAuthReaderStep3( handle, MifareBuffer );
    if ( rv != 1 )
    {
        MCC_DBG( "E: failed authentication step 3!\n" );
        mccResetCipher();
        return ERR_NOTFOUND;
    }

    rv = ERR_NONE;

    MCC_DBG( "I: prng = 0x%x\n", (uint16_t)(handle->prng) );

    mccCryptoSetActive( handle, 1 );

    MCC_DBG( "I: state: %d\n", mccCryptoIsActive( handle ) );

    MCC_DBG( "I: Success: Auth Step 2\n" );

out:
    return rv;
}

int8_t mccSendRequest ( const uint8_t *request,
                        uint16_t requestLength,
                        uint8_t *response,
                        uint16_t maxResponseLength,
                        uint16_t *responseLength,
                        uint16_t timeout,
                        uint8_t fourBitResponse )
{
    uint16_t crc;
    uint16_t bits = 0;
    uint16_t bytes = 0;
    int8_t rv = 0;
    unsigned int i;

    MCC_DBG( "mcc send request\n" );

    // 1. write to MifareBuffer
    MCC_DBG( "  request (%d): ", requestLength );
    for ( i = 0; i < requestLength; i++ )
    {
        MCC_DBG("%hx, ", request[i]);
        MifareBuffer[i] = request[i];
    }
    MCC_DBG( "\n" );

    // 2. calculate CRC
    crc = rfalCrcCalculateCcitt( CRC_PRELOAD_VALUE, request, requestLength );
    MifareBuffer[requestLength] = LOW_BYTE( crc );
    MifareBuffer[requestLength+1] = HIGH_BYTE( crc );

    // 3. calculate parity
    rv = attachParityInformation( MifareBuffer, requestLength + CRC_LEN );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );

    // 4. cypher is used -> encrypt
    if ( mccCryptoIsActive( handle ) != 0 )
    {
        mccCryptoTranscode( handle, MifareBuffer, requestLength + CRC_LEN, 0 );
    }

    // 5. transceive data
    rv = mccSendRawRequest( MifareBuffer, requestLength + CRC_LEN,
                     response, maxResponseLength, responseLength,
                     timeout, fourBitResponse );
    EVAL_ERR_NE_GOTO( ERR_NONE, rv, out );
    HAL_Delay(300);
    // 6. Copy response back
    for ( i = 0; i < *responseLength; i++ )
    {
        MifareBuffer[i] = response[i];
    }

    // 7. Encryption enabled ?
    if ( mccCryptoIsActive( handle ) != 0 )
    {
        bytes = *responseLength;
        bits = 0;
        // 7.1 response received? (0 or 1 byte)
        if ( *responseLength == 0 || *responseLength == 1 )
        {
            bytes = 0;
            bits = 4;
            MCC_DBG("D: %d bytes %d bits\n", bytes, bits);
            MCC_DBG("D: No Response\n");
        }

        // 7.2 transcode received data
        MCC_DBG("D: %d bytes %d bits\n", bytes, bits);
        mccCryptoTranscode( handle, MifareBuffer, bytes, bits );

    }

    // 7. write output
    for ( i = 0; i < *responseLength; i++ )
    {
        response[i] = MifareBuffer[i];
    }

out:
    return rv;
}
void demoMifareClassic( uint8_t * uid, uint32_t uidLen )
{
    ReturnCode err;

    uint8_t key = MCC_AUTH_KEY_A;
    uint8_t mifareKey[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t sector = 0;
    uint8_t block = 0;

    uint8_t request[2];
    uint8_t buffer[MIFARE_BLOCK_SIZE + MIFARE_CRC_LEN];
    uint16_t numBytesReceived;

    const uint32_t nonce = 0x11223344;
    /* Initialize it */
    mccInitialize();

    /* Authenticate */
    err = mccAuthenticate(key, sector, uid, uidLen, mifareKey, nonce);

    if (err == ERR_NONE)
    {
        platformLog("Authentication succeeded...\r\n");

        /* Read the block */
        request[0] = MCC_READ_BLOCK;
        request[1] = (sector * 4) + block; /* address */
         //request[1] = (sector * 4) + block;
        err = mccSendRequest(request, sizeof(request),
                             buffer, sizeof(buffer),
                             &numBytesReceived, MIFARE_READ_TIMEOUT, false);

        if (err == ERR_NONE)
        {
            platformLog(" Read block %d: %s\r\n", block, hex2Str( buffer, sizeof(buffer) - MIFARE_CRC_LEN));
        }
    }
    else
    {
        platformLog("Authentication failed\r\n");
    }

    /* and return ressources... */
    mccDeinitialise(true);
}

/*
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 */

static int8_t buildCommand ( uint8_t *cmd, uint8_t keySelect, uint8_t block )
{
    uint16_t crc;

    cmd[0] = keySelect;
    cmd[1] = block;

    crc = rfalCrcCalculateCcitt( CRC_PRELOAD_VALUE, cmd, CRC_LEN );
    cmd[2] = LOW_BYTE( crc );
    cmd[3] = HIGH_BYTE( crc );

    return ERR_NONE;
}

static int8_t attachParityInformation ( uint16_t *cmd, size_t length )
{
    unsigned int i = 0;
    for ( i = 0; i < length; i++ )
    {
        if ( mccParityOdd( cmd[i] ) == 1 )
        {
            cmd[i] |= 0x0100;
        }
        else
        {
            cmd[i] &= ~0x0100;
        }
    }

    return ERR_NONE;
}
