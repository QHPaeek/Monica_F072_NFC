/******************************************************************************
  * \attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        www.st.com/myliberty
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/

/*! \file
 *
 *  \author 
 *
 *  \brief Demo application
 *
 *  This demo shows how to poll for several types of NFC cards/devices and how 
 *  to exchange data with these devices, using the RFAL library.
 *
 *  This demo does not fully implement the activities according to the standards,
 *  it performs the required to communicate with a card/device and retrieve 
 *  its UID. Also blocking methods are used for data exchange which may lead to
 *  long periods of blocking CPU/MCU.
 *  For standard compliant example please refer to the Examples provided
 *  with the RFAL library.
 * 
 */
 
/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include <demo.h>
#include "utils.h"
#include "rfal_nfc.h"

#if defined(ST25R3916) && defined(RFAL_FEATURE_LISTEN_MODE)
#include <demo_ce.h>
#endif

/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/

/* Definition of possible states the demo state machine could have */
#define DEMO_ST_NOTINIT               0     /*!< Demo State:  Not initialized        */
#define DEMO_ST_START_DISCOVERY       1     /*!< Demo State:  Start Discovery        */
#define DEMO_ST_DISCOVERY             2     /*!< Demo State:  Discovery              */

#define DEMO_NFCV_BLOCK_LEN           4     /*!< NFCV Block len                      */

#define DEMO_NFCV_USE_SELECT_MODE     false /*!< NFCV demonstrate select mode        */
#define DEMO_NFCV_WRITE_TAG           false /*!< NFCV demonstrate Write Single Block */

/*
 ******************************************************************************
 * GLOBAL MACROS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */

/* P2P communication data */
static uint8_t NFCID3[] = {0x01, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
static uint8_t GB[] = {0x46, 0x66, 0x6d, 0x01, 0x01, 0x11, 0x02, 0x02, 0x07, 0x80, 0x03, 0x02, 0x00, 0x03, 0x04, 0x01, 0x32, 0x07, 0x01, 0x03};
    
/* APDUs communication data */    
static uint8_t ndefSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD2, 0x76, 0x00, 0x00, 0x85, 0x01, 0x01, 0x00 };
static uint8_t ccSelectFile[] = { 0x00, 0xA4, 0x00, 0x0C, 0x02, 0xE1, 0x03};
static uint8_t readBynary[] = { 0x00, 0xB0, 0x00, 0x00, 0x0F };
/*static uint8_t ppseSelectApp[] = { 0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 };*/

/* P2P communication data */    
static uint8_t ndefLLCPSYMM[] = {0x00, 0x00};
static uint8_t ndefInit[] = {0x05, 0x20, 0x06, 0x0F, 0x75, 0x72, 0x6E, 0x3A, 0x6E, 0x66, 0x63, 0x3A, 0x73, 0x6E, 0x3A, 0x73, 0x6E, 0x65, 0x70, 0x02, 0x02, 0x07, 0x80, 0x05, 0x01, 0x02};
static uint8_t ndefUriSTcom[] = {0x13, 0x20, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x19, 0xc1, 0x01, 0x00, 0x00, 0x00, 0x12, 0x55, 0x00, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x73, 0x74, 0x2e, 0x63, 0x6f, 0x6d};

#if defined(ST25R3916) && defined(RFAL_FEATURE_LISTEN_MODE)
/* NFC-A CE config */
static uint8_t ceNFCA_NFCID[]     = {0x02, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};                   /* NFCID / UID (7 bytes)                    */
static uint8_t ceNFCA_SENS_RES[]  = {0x44, 0x00};                                                 /* SENS_RES / ATQA                          */
static uint8_t ceNFCA_SEL_RES     = 0x20;                                                         /* SEL_RES / SAK                            */

/* NFC-F CE config */
static uint8_t ceNFCF_nfcid2[]     = {0x02, 0xFE, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
static uint8_t ceNFCF_SC[]         = {0x12, 0xFC};
static uint8_t ceNFCF_SENSF_RES[]  = {0x01,                                                       /* SENSF_RES                                */
                                      0x02, 0xFE, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,             /* NFCID2                                   */
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00,             /* PAD0, PAD01, MRTIcheck, MRTIupdate, PAD2 */
                                      0x00, 0x00 };                                               /* RD                                       */
#endif /* RFAL_FEATURE_LISTEN_MODE */

  
/*
 ******************************************************************************
 * LOCAL VARIABLES
 ******************************************************************************
 */
static rfalNfcDiscoverParam discParam;
static uint8_t              state = DEMO_ST_NOTINIT;

/*
******************************************************************************
* LOCAL FUNCTION PROTOTYPES
******************************************************************************
*/

static void demoP2P( void );
static void demoAPDU( void );
static void demoNfcv( rfalNfcvListenDevice *nfcvDev );
static void demoNfcf( rfalNfcfListenDevice *nfcfDev );
static void demoCE( rfalNfcDevice *nfcDev );
static void demoNotif( rfalNfcState st );
ReturnCode  demoTransceiveBlocking( uint8_t *txBuf, uint16_t txBufSize, uint8_t **rxBuf, uint16_t **rcvLen, uint32_t fwt );



/*!
 *****************************************************************************
 * \brief Demo Notification
 *
 *  This function receives the event notifications from RFAL
 *****************************************************************************
 */
static void demoNotif( rfalNfcState st )
{
    uint8_t       devCnt;
    rfalNfcDevice *dev;
    
    
    if( st == RFAL_NFC_STATE_WAKEUP_MODE )
    {
        platformLog("Wake Up mode started \r\n");
    }
    else if( st == RFAL_NFC_STATE_POLL_TECHDETECT )
    {
        platformLog("Wake Up mode terminated. Polling for devices \r\n");
    }
    else if( st == RFAL_NFC_STATE_POLL_SELECT )
    {
        /* Multiple devices were found, activate first of them */
        rfalNfcGetDevicesFound( &dev, &devCnt );
        rfalNfcSelect( 0 );
        
        platformLog("Multiple Tags detected: %d \r\n", devCnt);
    }
}

/*!
 *****************************************************************************
 * \brief Demo Ini
 *
 *  This function Initializes the required layers for the demo
 *
 * \return true  : Initialization ok
 * \return false : Initialization failed
 *****************************************************************************
 */

bool demoIni( void )
{
    ReturnCode err;
    err = rfalNfcInitialize();
    if( err == ERR_NONE )
    {
        discParam.compMode      = RFAL_COMPLIANCE_MODE_NFC;
        discParam.devLimit      = 1U;
        discParam.nfcfBR        = RFAL_BR_212;
        discParam.ap2pBR        = RFAL_BR_424;
        
        ST_MEMCPY( &discParam.nfcid3, NFCID3, sizeof(NFCID3) );
        ST_MEMCPY( &discParam.GB, GB, sizeof(GB) );
        discParam.GBLen         = sizeof(GB);

        discParam.notifyCb             = demoNotif;
        discParam.totalDuration        = 1000U;
        discParam.wakeupEnabled        = false;
        discParam.wakeupConfigDefault  = true;
        discParam.techs2Find           = ( RFAL_NFC_POLL_TECH_A | RFAL_NFC_POLL_TECH_B | RFAL_NFC_POLL_TECH_F | RFAL_NFC_POLL_TECH_V | RFAL_NFC_POLL_TECH_ST25TB );
        
#if defined(ST25R3911) || defined(ST25R3916)
        discParam.techs2Find   |= RFAL_NFC_POLL_TECH_AP2P;
#endif /* ST25R95 */
        
        
#if defined(ST25R3916)
      
      /* Set configuration for NFC-A CE */
      ST_MEMCPY( discParam.lmConfigPA.SENS_RES, ceNFCA_SENS_RES, RFAL_LM_SENS_RES_LEN );                        /* Set SENS_RES / ATQA */
      ST_MEMCPY( discParam.lmConfigPA.nfcid, ceNFCA_NFCID, RFAL_NFCID2_LEN );                                   /* Set NFCID / UID */
      discParam.lmConfigPA.nfcidLen = RFAL_LM_NFCID_LEN_07;                                                     /* Set NFCID length to 7 bytes */
      discParam.lmConfigPA.SEL_RES  = ceNFCA_SEL_RES;                                                           /* Set SEL_RES / SAK */

      /* Set configuration for NFC-F CE */
      ST_MEMCPY( discParam.lmConfigPF.SC, ceNFCF_SC, RFAL_LM_SENSF_SC_LEN );                                    /* Set System Code */
      ST_MEMCPY( &ceNFCF_SENSF_RES[RFAL_NFCF_LENGTH_LEN], ceNFCF_nfcid2, RFAL_LM_SENSF_RES_LEN );               /* Load NFCID2 on SENSF_RES */
      ST_MEMCPY( discParam.lmConfigPF.SENSF_RES, ceNFCF_SENSF_RES, RFAL_LM_SENSF_RES_LEN );                     /* Set SENSF_RES / Poll Response */
      
      discParam.techs2Find |= ( RFAL_NFC_LISTEN_TECH_A | RFAL_NFC_LISTEN_TECH_F );
      
#endif /* ST25R95 */

        state = DEMO_ST_START_DISCOVERY;
        return true;
    }
    return false;
}

/*!
 *****************************************************************************
 * \brief Demo Cycle
 *
 *  This function executes the demo state machine. 
 *  It must be called periodically
 *****************************************************************************
 */
void demoCycle( void )
{
    static rfalNfcDevice *nfcDevice;
    
    rfalNfcWorker();                                    /* Run RFAL worker periodically */

    /*******************************************************************************/
    /* Check if USER button is pressed */
    if( platformGpioIsLow(PLATFORM_USER_BUTTON_PORT, PLATFORM_USER_BUTTON_PIN))
    {
        discParam.wakeupEnabled = !discParam.wakeupEnabled;    /* enable/disable wakeup */
        state = DEMO_ST_START_DISCOVERY;                       /* restart loop          */
        platformLog("Toggling Wake Up mode %s\r\n", discParam.wakeupEnabled ? "ON": "OFF");

        /* Debounce button */
        while( platformGpioIsLow(PLATFORM_USER_BUTTON_PORT, PLATFORM_USER_BUTTON_PIN) );
    }
  
    
    switch( state )
    {
        /*******************************************************************************/
        case DEMO_ST_START_DISCOVERY:

          platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
          platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
          platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
          platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
          platformLedOff(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
          platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);
          
          rfalNfcDeactivate( false );
          rfalNfcDiscover( &discParam );
          
          state = DEMO_ST_DISCOVERY;
          break;

        /*******************************************************************************/
        case DEMO_ST_DISCOVERY:
        
            if( rfalNfcIsDevActivated( rfalNfcGetState() ) )
            {
                rfalNfcGetActiveDevice( &nfcDevice );
                
                switch( nfcDevice->type )
                {
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_NFCA:
                    
                        platformLedOn(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
                        switch( nfcDevice->dev.nfca.type )
                        {
                            case RFAL_NFCA_T1T:
                                platformLog("ISO14443A/Topaz (NFC-A T1T) TAG found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                                break;
                            
                            case RFAL_NFCA_T4T:
                                platformLog("NFCA Passive ISO-DEP device found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                            
                                demoAPDU();
                                break;
                            
                            case RFAL_NFCA_T4T_NFCDEP:
                            case RFAL_NFCA_NFCDEP:
                                platformLog("NFCA Passive P2P device found. NFCID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                                
                                demoP2P();
                                break;
                                
                            default:
                                platformLog("ISO14443A/NFC-A card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                                break;
                        }
                        break;
                    
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_NFCB:
                        
                        platformLog("ISO14443B/NFC-B card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                        platformLedOn(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
                    
                        if( rfalNfcbIsIsoDepSupported( &nfcDevice->dev.nfcb ) )
                        {
                            demoAPDU();
                        }
                        break;
                        
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_NFCF:
                        
                        if( rfalNfcfIsNfcDepSupported( &nfcDevice->dev.nfcf ) )
                        {
                            platformLog("NFCF Passive P2P device found. NFCID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ) );
                            demoP2P();
                        }
                        else
                        {
                            platformLog("Felica/NFC-F card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ));
                            
                            demoNfcf( &nfcDevice->dev.nfcf );
                        }
                        
                        platformLedOn(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
                        break;
                    
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_NFCV:
                        {
                            uint8_t devUID[RFAL_NFCV_UID_LEN];
                            
                            ST_MEMCPY( devUID, nfcDevice->nfcid, nfcDevice->nfcidLen );   /* Copy the UID into local var */
                            REVERSE_BYTES( devUID, RFAL_NFCV_UID_LEN );                 /* Reverse the UID for display purposes */
                            platformLog("ISO15693/NFC-V card found. UID: %s\r\n", hex2Str(devUID, RFAL_NFCV_UID_LEN));
                        
                            platformLedOn(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
                            
                            demoNfcv( &nfcDevice->dev.nfcv );
                        }
                        break;
                        
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_ST25TB:
                        
                        platformLog("ST25TB card found. UID: %s\r\n", hex2Str( nfcDevice->nfcid, nfcDevice->nfcidLen ));
                        platformLedOn(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
                        break;
                    
                    /*******************************************************************************/
                    case RFAL_NFC_LISTEN_TYPE_AP2P:
                        
                        platformLog("NFC Active P2P device found. NFCID3: %s\r\n", hex2Str(nfcDevice->nfcid, nfcDevice->nfcidLen));
                        platformLedOn(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
                    
                        demoP2P();
                        break;
                    
                    /*******************************************************************************/
                    case RFAL_NFC_POLL_TYPE_NFCA:
                    case RFAL_NFC_POLL_TYPE_NFCF:
                        
                        platformLog("Activated in CE %s mode.\r\n", (nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? "NFC-A" : "NFC-F");
                        platformLedOn( ((nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? PLATFORM_LED_A_PORT : PLATFORM_LED_F_PORT), 
                                       ((nfcDevice->type == RFAL_NFC_POLL_TYPE_NFCA) ? PLATFORM_LED_A_PIN  : PLATFORM_LED_F_PIN)  );
                    
                        demoCE( nfcDevice );
                        break;
                    
                    /*******************************************************************************/
                    default:
                        break;
                }
                
                rfalNfcDeactivate( false );
                platformDelay( 500 );
                state = DEMO_ST_START_DISCOVERY;
            }
            break;

        /*******************************************************************************/
        case DEMO_ST_NOTINIT:
        default:
            break;
    }
}

static void demoCE( rfalNfcDevice *nfcDev )
{
#if defined(ST25R3916) && defined(RFAL_FEATURE_LISTEN_MODE)
    
    ReturnCode err;
    uint8_t *rxData;
    uint16_t *rcvLen;
    uint8_t  txBuf[100];
    uint16_t txLen;
    
    demoCeInit( ceNFCF_nfcid2 );
    
    do
    {
        rfalNfcWorker();
        
        switch( rfalNfcGetState() )
        {
            case RFAL_NFC_STATE_ACTIVATED:
                err = demoTransceiveBlocking( NULL, 0, &rxData, &rcvLen, 0);
                break;
            
            case RFAL_NFC_STATE_DATAEXCHANGE:
            case RFAL_NFC_STATE_DATAEXCHANGE_DONE:
                
                txLen = ( (nfcDev->type == RFAL_NFC_POLL_TYPE_NFCA) ? demoCeT4T( rxData, *rcvLen, txBuf, sizeof(txBuf) ): demoCeT3T( rxData, *rcvLen, txBuf, sizeof(txBuf) ) );
                err   = demoTransceiveBlocking( txBuf, txLen, &rxData, &rcvLen, RFAL_FWT_NONE );
                break;
            
            case RFAL_NFC_STATE_LISTEN_SLEEP:
            default:
                break;
        }
    }
    while( (err == ERR_NONE) || (err == ERR_SLEEP_REQ) );
    
#endif /* RFAL_FEATURE_LISTEN_MODE */
}

/*!
 *****************************************************************************
 * \brief Demo NFC-F 
 *
 * Example how to exchange read and write blocks on a NFC-F tag
 * 
 *****************************************************************************
 */
static void demoNfcf( rfalNfcfListenDevice *nfcfDev )
{
    ReturnCode                 err;
    uint8_t                    buf[ (RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN + (3*RFAL_NFCF_BLOCK_LEN)) ];
    uint16_t                   rcvLen;
    rfalNfcfServ               srv = RFAL_NFCF_SERVICECODE_RDWR;
    rfalNfcfBlockListElem      bl[3];
    rfalNfcfServBlockListParam servBlock;
    //uint8_t                    wrData[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    
    servBlock.numServ   = 1;                            /* Only one Service to be used           */
    servBlock.servList  = &srv;                         /* Service Code: NDEF is Read/Writeable  */
    servBlock.numBlock  = 1;                            /* Only one block to be used             */
    servBlock.blockList = bl;
    bl[0].conf     = 0x80;       /* Two-byte Block List Element           */
    bl[0].blockNum = 0x0001;                            /* Block: NDEF Data                      */
    
    err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
    platformLog(" Check Block: %s Data:  %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
    
    #if 0  /* Writing example */
        err = rfalNfcfPollerUpdate( nfcfDev->sensfRes.NFCID2, &servBlock, buf , sizeof(buf), wrData, buf, sizeof(buf) );
        platformLog(" Update Block: %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( wrData, RFAL_NFCF_BLOCK_LEN) );
        err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
        platformLog(" Check Block:  %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
    #endif
}

/*!
 *****************************************************************************
 * \brief Demo NFC-V Exchange
 *
 * Example how to exchange read and write blocks on a NFC-V tag
 * 
 *****************************************************************************
 */
static void demoNfcv( rfalNfcvListenDevice *nfcvDev )
{
    ReturnCode            err;
    uint16_t              rcvLen;
    uint8_t               blockNum = 1;
    uint8_t               rxBuf[ 1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN ];                        /* Flags + Block Data + CRC */
    uint8_t *             uid; 
#if DEMO_NFCV_WRITE_TAG
    uint8_t               wrData[DEMO_NFCV_BLOCK_LEN] = { 0x11, 0x22, 0x33, 0x99 };             /* Write block example */
#endif
              

    uid = nfcvDev->InvRes.UID;
    
    #if DEMO_NFCV_USE_SELECT_MODE
        /*
        * Activate selected state
        */
        err = rfalNfcvPollerSelect(RFAL_NFCV_REQ_FLAG_DEFAULT, nfcvDev->InvRes.UID );
        platformLog(" Select %s \r\n", (err != ERR_NONE) ? "FAIL (revert to addressed mode)": "OK" );
        if( err == ERR_NONE )
        {
            uid = NULL;
        }
    #endif    

    /*
    * Read block using Read Single Block command
    * with addressed mode (uid != NULL) or selected mode (uid == NULL)
    */
    err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
    platformLog(" Read Block: %s %s\r\n", (err != ERR_NONE) ? "FAIL": "OK Data:", (err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
 
    #if DEMO_NFCV_WRITE_TAG /* Writing example */
        err = rfalNfcvPollerWriteSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, wrData, sizeof(wrData));
        platformLog(" Write Block: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( wrData, DEMO_NFCV_BLOCK_LEN) );
        err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
        platformLog(" Read Block: %s %s\r\n", (err != ERR_NONE) ? "FAIL": "OK Data:", (err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
    #endif
}


/*!
 *****************************************************************************
 * \brief Demo P2P Exchange
 *
 * Sends a NDEF URI record 'http://www.ST.com' via NFC-DEP (P2P) protocol.
 * 
 * This method sends a set of static predefined frames which tries to establish
 * a LLCP connection, followed by the NDEF record, and then keeps sending 
 * LLCP SYMM packets to maintain the connection.
 * 
 * 
 *****************************************************************************
 */
void demoP2P( void )
{
    uint16_t   *rxLen;
    uint8_t    *rxData;
    ReturnCode err;

    platformLog(" Initalize device .. ");
    err = demoTransceiveBlocking( ndefInit, sizeof(ndefInit), &rxData, &rxLen, RFAL_FWT_NONE);
    if( err != ERR_NONE )
    {
        platformLog("failed.");
        return;
    }
    platformLog("succeeded.\r\n");

    platformLog(" Push NDEF Uri: www.ST.com .. ");
    err = demoTransceiveBlocking( ndefUriSTcom, sizeof(ndefUriSTcom), &rxData, &rxLen, RFAL_FWT_NONE);
    if( err != ERR_NONE )
    {
        platformLog("failed.");
        return;
    }
    platformLog("succeeded.\r\n");


    platformLog(" Device present, maintaining connection ");
    while(err == ERR_NONE) 
    {
        err = demoTransceiveBlocking( ndefLLCPSYMM, sizeof(ndefLLCPSYMM), &rxData, &rxLen, RFAL_FWT_NONE);
        platformLog(".");
        platformDelay(50);
    }
    platformLog("\r\n Device removed.\r\n");
}


/*!
 *****************************************************************************
 * \brief Demo APDUs Exchange
 *
 * Example how to exchange a set of predefined APDUs with PICC. The NDEF
 * application will be selected and then CC will be selected and read.
 * 
 *****************************************************************************
 */
void demoAPDU( void )
{
    ReturnCode err;
    uint16_t   *rxLen;
    uint8_t    *rxData;


    /* Exchange APDU: NDEF Tag Application Select command */
    err = demoTransceiveBlocking( ndefSelectApp, sizeof(ndefSelectApp), &rxData, &rxLen, RFAL_FWT_NONE );
    platformLog(" Select NDEF Application: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( rxData, *rxLen) );

    if( (err == ERR_NONE) && rxData[0] == 0x90 && rxData[1] == 0x00)
    {
        /* Exchange APDU: Select Capability Container File */
        err = demoTransceiveBlocking( ccSelectFile, sizeof(ccSelectFile), &rxData, &rxLen, RFAL_FWT_NONE );
        platformLog(" Select CC: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( rxData, *rxLen) );

        /* Exchange APDU: Read Capability Container File  */
        err = demoTransceiveBlocking( readBynary, sizeof(readBynary), &rxData, &rxLen, RFAL_FWT_NONE );
        platformLog(" Read CC: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( rxData, *rxLen) );
    }
}


/*!
 *****************************************************************************
 * \brief Demo Blocking Transceive 
 *
 * Helper function to send data in a blocking manner via the rfalNfc module 
 *  
 * \warning A protocol transceive handles long timeouts (several seconds), 
 * transmission errors and retransmissions which may lead to a long period of 
 * time where the MCU/CPU is blocked in this method.
 * This is a demo implementation, for a non-blocking usage example please 
 * refer to the Examples available with RFAL
 *
 * \param[in]  txBuf      : data to be transmitted
 * \param[in]  txBufSize  : size of the data to be transmited
 * \param[out] rxData     : location where the received data has been placed
 * \param[out] rcvLen     : number of data bytes received
 * \param[in]  fwt        : FWT to be used (only for RF frame interface, 
 *                                          otherwise use RFAL_FWT_NONE)
 *
 * 
 *  \return ERR_PARAM     : Invalid parameters
 *  \return ERR_TIMEOUT   : Timeout error
 *  \return ERR_FRAMING   : Framing error detected
 *  \return ERR_PROTO     : Protocol error detected
 *  \return ERR_NONE      : No error, activation successful
 * 
 *****************************************************************************
 */
ReturnCode demoTransceiveBlocking( uint8_t *txBuf, uint16_t txBufSize, uint8_t **rxData, uint16_t **rcvLen, uint32_t fwt )
{
    ReturnCode err;
    
    err = rfalNfcDataExchangeStart( txBuf, txBufSize, rxData, rcvLen, fwt );
    if( err == ERR_NONE )
    {
        do{
            rfalNfcWorker();
            err = rfalNfcDataExchangeGetStatus();
        }
        while( err == ERR_BUSY );
    }
    return err;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
