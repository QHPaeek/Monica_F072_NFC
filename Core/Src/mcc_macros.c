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
 *  \brief Utility functions and macros
 *
 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
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

uint32_t mccReverse ( register uint32_t x )
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1)); // Swap odd, even
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2)); // 2 bits change, nibble is reversed
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4)); // swap 4 bits and get desired result
    return x;
}

uint32_t mccParityEven ( uint32_t val )
{
    val ^= val >> 16;
    val ^= val >>  8;
    val ^= val >>  4;
    val ^= val >>  2;
    return (val ^ (val>>1)) & 1;
}

uint32_t mccParityOdd ( uint32_t val )
{
    val ^= val >> 16;
    val ^= val >>  8;
    val ^= val >>  4;
    val ^= val >>  2;
    return (~(val ^ (val>>1))&1);
}

#ifdef __cplusplus
}
#endif // __cplusplus
