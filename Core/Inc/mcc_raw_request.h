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

#ifndef MCC_RAW_REQUEST_H
#define MCC_RAW_REQUEST_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/

/*
******************************************************************************
* GLOBAL FUNCTION PROTOTYPES
******************************************************************************
*/
/*!
 *****************************************************************************
 * \brief Send a raw MCC request.
 *
 * Send a raw MCC request.
 *
 * \note No more than \a maxResponseLength bytes are written into
 * the response buffer. If more than \a maxResponseLength bytes are
 * received a buffer overflow error will be returned.
 *
 * \note Receiving a 4 bit response while 4 bit response receiving
 * is not enabled will lead to the returned number of received bytes being
 * zero and no data being placed into the response buffer.
 *
 * \note Receiving 4 bit response is disabled right now and the parameter
 * \a fourBitResponse will not have any effect!
 *
 * \param request Pointer to the array storing the request.
 * \param requestLength Length of the request.
 * \param response Pointer to the array where the received response is stored.
 * \param maxResponseLength Size of the response buffer in bytes.
 * \param responseLength Set to the length of the received response in bytes.
 * \param timeout Timeout in milliseconds.
 * \param fourBitRespnose If this parameter is \a TRUE, then
 *        receiving of four bit responses is enabled.
 *
 * \return Returns the number of received bytes or a negative error code.
 *****************************************************************************
 */
int8_t mccSendRawRequest ( uint16_t *request,
                           uint16_t requestLength,
                           uint8_t *response,
                           uint16_t maxResponseLength,
                           uint16_t *responseLength,
                           uint16_t timeout,
                           uint8_t fourBitResponse );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* MCC_RAW_REQUEST_H */
