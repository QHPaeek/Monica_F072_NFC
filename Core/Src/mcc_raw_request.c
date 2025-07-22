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
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include <string.h>
#include "mcc.h"
#include "rfal_rf.h"
#include "st_errno.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
******************************************************************************
* DEFINES
******************************************************************************
*/

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
* LOCAL VARIABLES
******************************************************************************
*/
static uint8_t mccRawBuffer[((MCC_BUFFER_SIZE*9)+7)/8];

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/
static uint16_t mccCopyToRawBuffer ( uint16_t *message, uint16_t length );
static uint16_t mccExtractMessage ( uint8_t* response, uint16_t responseLength );

/*
******************************************************************************
* GLOBAL FUNCTIONS
******************************************************************************
*/
int8_t mccSendRawRequest ( uint16_t *request,
                           uint16_t requestLength,
                           uint8_t *response,
                           uint16_t maxResponseLength,
                           uint16_t *responseLength,
                           uint16_t timeout,
                           uint8_t fourBitResponse )
{
    ReturnCode ret;
    int8_t err = ERR_NONE;
    rfalTransceiveContext ctx;
    uint16_t len_bits;

    len_bits = mccCopyToRawBuffer(request, requestLength);

    ctx.txBuf = mccRawBuffer;
    ctx.txBufLen = len_bits;
    ctx.rxBuf = mccRawBuffer;
    ctx.rxBufLen = sizeof(mccRawBuffer) * 8;
    ctx.rxRcvdLen = responseLength;
    ctx.flags = RFAL_TXRX_FLAGS_CRC_TX_MANUAL |RFAL_TXRX_FLAGS_CRC_RX_KEEP | RFAL_TXRX_FLAGS_NFCIP1_OFF | RFAL_TXRX_FLAGS_AGC_ON | RFAL_TXRX_FLAGS_PAR_RX_KEEP | RFAL_TXRX_FLAGS_PAR_TX_NONE;
    ctx.fwt = rfalConvMsTo1fc(timeout);

    err = rfalStartTransceive( &ctx );
    do{
        rfalWorker();
    }
    while((ret = rfalGetTransceiveStatus() ) == ERR_BUSY);

    err = ret;
    platformLog("error2:%X\r\n",err);
    platformLog("DUMP:");
    for(uint8_t i = 0;i<=(*ctx.rxRcvdLen/8);i++){
    	platformLog("%X ",mccRawBuffer[i]);
    }
    platformLog("\r\n");
    /* Treat pseudo errors for incomplete byte as no error */
    if (ret >= ERR_INCOMPLETE_BYTE && ret <= ERR_INCOMPLETE_BYTE_07)
    {
        err = ERR_NONE;
    }

    if ((*responseLength / 9) > maxResponseLength)
    {
        *responseLength = maxResponseLength * 9;
        err = ERR_NOMEM; /* This will drop an existing CRC */
    }
    *responseLength = mccExtractMessage(response, *responseLength);

    return err;
}

/*
******************************************************************************
* LOCAL FUNCTIONS
******************************************************************************
*/
static uint16_t mccCopyToRawBuffer ( uint16_t *message, uint16_t length )
{
    int i, bytepos = 0;
    int bitpos = 0;
    memset(mccRawBuffer,0,sizeof(mccRawBuffer));
    MCC_DBG("transmitting: ");
    for(i = 0; i<length; i++)
    {
        MCC_DBG("%hx,",message[i]);
    }
    MCC_DBG("\n");


    for (i = 0; i < length; i++)
    {
        uint16_t m = message[i];
        mccRawBuffer[bytepos] |= (m & ((1<<(8 - bitpos))-1)) << bitpos;
        bytepos++;
        mccRawBuffer[bytepos] |= (m >> (8-bitpos));

        bitpos += 1;
        if (bitpos >=8 )
        {
            bitpos -= 8;
            bytepos++;
        }
    }
    MCC_DBG("  raw: ");
    for ( i= 0; i< ((length*9)+7)/8;i++)
    {
        MCC_DBG("%hhx,",mccRawBuffer[i]);
    }
    MCC_DBG("\n");
    return length*9;
}

static uint16_t mccExtractMessage ( uint8_t* response, uint16_t responseLength_bits )
{
    int bytes = responseLength_bits / 9;
    int i, bytepos = 0;
    int bitpos = 0;
	  MCC_DBG("extracting (%d bytes)", bytes);
    MCC_DUMP(mccRawBuffer,bytes);
    if (responseLength_bits<=8)
    {
        response[0] = mccRawBuffer[0];
        return 1;
    }
    for (i = 0; i < bytes; i++)
    {
        uint8_t m;
        m = (mccRawBuffer[bytepos] >> bitpos);
        bytepos++;
        m |= (mccRawBuffer[bytepos] << (8-bitpos));

        bitpos += 1;
        if (bitpos >=8 )
        {
            bitpos -= 8;
            bytepos++;
        }

        response[i] = m;
    }
    MCC_DBG(" extracted: (%d bytes)", bytes);
    MCC_DUMP(response,bytes);
    return bytes;
}

#ifdef __cplusplus
}
#endif
