/*
 * Card_Read.c
 *
 *  Created on: Jul 19, 2025
 *      Author: Qinh
 */
#include "Card_Reader.h"
#include "mode_manager.h"

#define RFAL_NFCA_SEL_RES_CONF_MIFARE    	0x08 /* SEL_RES (SAK) Mifare configuration */
#define DEMO_NFCV_BLOCK_LEN           		4     /*!< NFCV Block len                      */
#define MIFARE_BLOCK_SIZE            		16
#define MIFARE_CRC_LEN                		2
#define MIFARE_READ_TIMEOUT           		50
//#define RFAL_CRC_LEN 						2

CardData Card;
uint8_t AimeKey[6] = {0x57, 0x43, 0x43, 0x46, 0x76, 0x32};
uint8_t BanaKey_A[6] = {0x60, 0x90, 0xD0, 0x06, 0x32, 0xF5};
uint8_t BanaKey_B[6] = {0x01, 0x97, 0x61, 0xAA, 0x80, 0x82};

uint16_t Get_Card_ATQA(rfalNfcaSensRes atqa){
	//return ((atqa.anticollisionInfo << 8) | atqa.platformInfo);
	return ((atqa.platformInfo << 8) | atqa.anticollisionInfo);
}

void Card_Poll()
{

    ReturnCode           	err;
    rfalNfcaListenDevice 	nfcaDev;
    rfalNfcfListenDevice  	nfcfDev;
    rfalNfcvListenDevice  	nfcvDev;
    uint8_t              	devCnt = 0;

    /*******************************************************************************/
    /* NFC-A Technology Detection                                                  */
    /*******************************************************************************/

    rfalNfcaPollerInitialize();                                                       /* Initialize RFAL for NFC-A */
    rfalFieldOnAndStartGT();                                                          /* Turns the Field On and starts GT timer */

    //err = rfalNfcaPollerTechnologyDetection( RFAL_COMPLIANCE_MODE_NFC, &sensRes ); /* Poll for NFC-A devices */
    err = rfalNfcaPollerFullCollisionResolution(RFAL_COMPLIANCE_MODE_NFC,1,&nfcaDev,&devCnt);
    if( (err == ERR_NONE) && (devCnt > 0) )
    {
    	platformLedOn(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
    	platformLog("ISO14443A/NFC-A card found. UID: %s\r\n", hex2Str(nfcaDev.nfcId1,nfcaDev.nfcId1Len));
    	if(nfcaDev.nfcId1Len == 4){
    		if(memcmp(Card.iso14443_uid4,nfcaDev.nfcId1,nfcaDev.nfcId1Len) == 0){
    			platformLog("same,skip\n");
    			if(Card.mifare_auth_status == Auth_ALL_Right){
    				Card.operation = Operation_detected;
    				rfalFieldOff();
    				return;
    			}
    		}else {
    			memcpy(Card.iso14443_uid4,nfcaDev.nfcId1,nfcaDev.nfcId1Len);
    		}
    	}else if (nfcaDev.nfcId1Len == 7){
    		memcpy(Card.iso14443_uid7,nfcaDev.nfcId1,nfcaDev.nfcId1Len);
    	}
    	Card.operation = Operation_detected;
        /*******************************************************************************/
        /* Check if desired device is in Sleep                                         */
        if( nfcaDev.isSleep )
        {
            err = rfalNfcaPollerCheckPresence( RFAL_14443A_SHORTFRAME_CMD_WUPA, &nfcaDev.sensRes ); /* Wake up all cards  */
            if( err == ERR_NONE )
            {
            	err = rfalNfcaPollerSelect( nfcaDev.nfcId1, nfcaDev.nfcId1Len, &nfcaDev.selRes ); /* Select specific device  */
            }
        }
		/* Check if device supports Mifare */
		if (nfcaDev.type == RFAL_NFCA_T2T)
		{
			switch(Get_Card_ATQA(nfcaDev.sensRes)){
				case 0x0044:
					if(nfcaDev.nfcId1Len == 7){
						//Detected: Mifare Ultralight or NFC Type 2 Tag
						if(Card.type != Card_Type_Mifare_UltraLight){
							memset(Card.data,0,128);
						}
						Card.type = Card_Type_Mifare_UltraLight;
					}
					break;
		        case 0x0004:{
		        	platformLog("Detected: Mifare Classic 1K\n");
		        	if(Card.type != Card_Type_Mifare_Classic){
		        		memset(Card.data,0,128);
		        	}
		        	Card.type = Card_Type_Mifare_Classic;
		        	if(Reader.Current_Mode == MODE_SEGA_SERIAL){
		        		sega_mifare_pre_read();
		        	}

//		        	mccInitialize();
////		            uint8_t key = MCC_AUTH_KEY_A;
//////		            uint8_t mifareKey[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
////		            uint8_t mifareKey[] = {0x57, 0x43, 0x43, 0x46, 0x76, 0x32};
//		            uint8_t sector = 0;
//		            uint8_t block = 2;
////
//		            uint8_t request[2];
//		            uint8_t buffer[MIFARE_BLOCK_SIZE + MIFARE_CRC_LEN];
//		            uint16_t numBytesReceived;
////
//		            const uint32_t nonce = 0x94857192;
////		            /* Initialize it */
////		            mccInitialize();
////
////		            /* Authenticate */
//		            err = mccAuthenticate(MCC_AUTH_KEY_A, sector, Card.iso14443_uid4, 4, BanaKey, nonce);
////		            err = mccAuthenticate(MCC_AUTH_KEY_B, sector, Card.iso14443_uid4, 4, mifareKey, nonce);
//		            if (err == ERR_NONE)
//		            {
//		                platformLog("Authentication succeeded...\r\n");
////		                /* Read the block */
//		                request[0] = MCC_READ_BLOCK;
//		                request[1] = (sector * 4) + block; /* address */
//		                 //request[1] = (sector * 4) + block;
//		                err = mccSendRequest(request, sizeof(request),
//		                                     buffer, sizeof(buffer),
//		                                     &numBytesReceived, MIFARE_READ_TIMEOUT, false);
//
//		                if (err == ERR_NONE)
//		                {
//		                    platformLog(" Read block %d: %s\r\n", block, hex2Str( buffer, sizeof(buffer) - MIFARE_CRC_LEN));
//		                }
//		            }
//		            else
//		            {
//		                platformLog("Authentication failed\r\n");
//		            }
////
////		            /* and return ressources... */
//		            mccDeinitialise(true);

		        	break;
		        }
		        case 0x0002:
		        	platformLog("Detected: Mifare Classic 4K\n");
		        	if(Card.type != Card_Type_Mifare_Classic){
		        		memset(Card.data,0,128);
		        	}
		        	Card.type = Card_Type_Mifare_Classic;
		        	if(Reader.Current_Mode == MODE_SEGA_SERIAL){
						sega_mifare_pre_read();
					}
		            break;
			}
//			if (nfcaDev.selRes.sak & RFAL_NFCA_SEL_RES_CONF_MIFARE)
//			{
//				platformLog("Mifare device detected.UID:");
//				for(uint8_t i = 0;i<nfcaDev.nfcId1Len;i++){
//					platformLog("%X",nfcaDev.nfcId1[i]);
//				}
//
//			}
		}
		/* Check if it is Topaz aka T1T */
		else if( nfcaDev.type == RFAL_NFCA_T1T )
		{
			/********************************************/
			/* NFC-A T1T card found                     */
			/* NFCID/UID is contained in: t1tRidRes.uid */
			//platformLog("ISO14443A/Topaz (NFC-A T1T) TAG found. UID: %s\r\n", hex2Str(nfcaDev.ridRes.uid, RFAL_T1T_UID_LEN));
	    	Card.type = Card_Type_ISO14443A_Unknow;
		}
		/* Check if device is NFCA_T4T */
		else if( (nfcaDev.type == RFAL_NFCA_T4T))
		{
	    	Card.type = Card_Type_ISO14443A_Unknow;
		}
		/* Check if device supports P2P/NFC-DEP */
		else if( (nfcaDev.type == RFAL_NFCA_NFCDEP) || (nfcaDev.type == RFAL_NFCA_T4T_NFCDEP))
		{
	    	Card.type = Card_Type_ISO14443A_Unknow;
		}
		rfalFieldOff();
		return;
    }
	/*******************************************************************************/
	/* Felica/NFC_F_PASSIVE_POLL_MODE                                              */
	/*******************************************************************************/

	rfalNfcfPollerInitialize( RFAL_BR_212 ); /* Initialize for NFC-F */
	rfalFieldOnAndStartGT();                 /* Turns the Field On if not already and start GT timer */

	err = rfalNfcfPollerCheckPresence();
	if( err == ERR_NONE )
	{
		err = rfalNfcfPollerCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 1, &nfcfDev, &devCnt );
		if( (err == ERR_NONE) && (devCnt > 0) )
		{
			/******************************************************/
			/* NFC-F card found                                   */
			/* NFCID/UID is contained in: nfcfDev.sensfRes.NFCID2 */
			platformLog("Felica/NFC-F card found. UID: %s\r\n", hex2Str(nfcfDev.sensfRes.NFCID2, RFAL_NFCF_NFCID2_LEN));
			platformLedOn(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
			if(Card.type != Card_Type_Felica){
				memset(Card.data,0,128);
			}
			Card.type = Card_Type_Felica;
			Card.operation = Operation_detected;
			memcpy(Card.felica_IDm,nfcfDev.sensfRes.NFCID2, RFAL_NFCF_NFCID2_LEN);
			memcpy(Card.felica_PMm,&nfcfDev.sensfRes.PAD0[0], 8);
			memcpy(Card.felica_systemcode,nfcfDev.sensfRes.RD,2);
			rfalFieldOff();
			return;
		}

	}
	/*******************************************************************************/
	/* ISO15693/NFC_V_PASSIVE_POLL_MODE                                            */
	/*******************************************************************************/

	rfalNfcvPollerInitialize();           /* Initialize for NFC-F */
	rfalFieldOnAndStartGT();              /* Turns the Field On if not already and start GT timer */

	err = rfalNfcvPollerCollisionResolution(1,1, &nfcvDev, &devCnt);
	if( (err == ERR_NONE) && (devCnt > 0) )
	{
		/******************************************************/
		/* NFC-V card found                                   */
		/* NFCID/UID is contained in: invRes.UID */
		REVERSE_BYTES(nfcvDev.InvRes.UID, RFAL_NFCV_UID_LEN);
		platformLog("ISO15693/NFC-V card found. UID: %s\r\n", hex2Str(nfcvDev.InvRes.UID, RFAL_NFCV_UID_LEN));
		platformLedOn(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
		if(Card.type != Card_Type_ISO15693){
			memset(Card.data,0,128);
		}
		Card.type = Card_Type_ISO15693;
		Card.operation = Operation_detected;
		memcpy(Card.iso15693_uid,nfcvDev.InvRes.UID, RFAL_NFCV_UID_LEN);
		rfalFieldOff();
		return;
	}

	//No Card Deteced
	platformLog("no card\r\n");
    platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
    platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
    platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
    platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
    platformLedOff(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
    platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);
    Card.type = Card_None;
    Card.operation = Operation_idle;
    memset(Card.data,0,128);
    rfalFieldOff();
}

void Normal_Poll(){
	ReturnCode           	err;
	rfalNfcaListenDevice 	nfcaDev;
	rfalNfcfListenDevice  	nfcfDev;
	rfalNfcvListenDevice  	nfcvDev;
	uint8_t              	devCnt = 0;

	/*******************************************************************************/
	/* NFC-A Technology Detection                                                  */
	/*******************************************************************************/

	rfalNfcaPollerInitialize();                                                       /* Initialize RFAL for NFC-A */
	rfalFieldOnAndStartGT();                                                          /* Turns the Field On and starts GT timer */

	//err = rfalNfcaPollerTechnologyDetection( RFAL_COMPLIANCE_MODE_NFC, &sensRes ); /* Poll for NFC-A devices */
	err = rfalNfcaPollerFullCollisionResolution(RFAL_COMPLIANCE_MODE_NFC,1,&nfcaDev,&devCnt);
	if( (err == ERR_NONE) && (devCnt > 0) )
	{
		return;
	}
	/*******************************************************************************/
	/* Felica/NFC_F_PASSIVE_POLL_MODE                                              */
	/*******************************************************************************/

	rfalNfcfPollerInitialize( RFAL_BR_212 ); /* Initialize for NFC-F */
	rfalFieldOnAndStartGT();                 /* Turns the Field On if not already and start GT timer */

	err = rfalNfcfPollerCheckPresence();
	if( err == ERR_NONE )
	{
		err = rfalNfcfPollerCollisionResolution( RFAL_COMPLIANCE_MODE_NFC, 1, &nfcfDev, &devCnt );
		if( (err == ERR_NONE) && (devCnt > 0) )
		{
			return;
		}

	}
	/*******************************************************************************/
	/* ISO15693/NFC_V_PASSIVE_POLL_MODE                                            */
	/*******************************************************************************/

	rfalNfcvPollerInitialize();           /* Initialize for NFC-F */
	rfalFieldOnAndStartGT();              /* Turns the Field On if not already and start GT timer */

	err = rfalNfcvPollerCollisionResolution(1,1, &nfcvDev, &devCnt);
	if( (err == ERR_NONE) && (devCnt > 0) )
	{
		return;
	}

	//No Card Deteced
	platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
	platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
	platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
	platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
	platformLedOff(PLATFORM_LED_AP2P_PORT, PLATFORM_LED_AP2P_PIN);
	platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);
	rfalFieldOff();
}
//void demoNfcf(void)
//{
//    ReturnCode                 err;
//    rfalNfcfListenDevice  nfcfDev;
//    uint8_t                    buf[ (RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN + (3*RFAL_NFCF_BLOCK_LEN)) ];
//    uint16_t                   rcvLen;
//    rfalNfcfServ               srv = RFAL_NFCF_SERVICECODE_RDWR;
//    rfalNfcfBlockListElem      bl[3];
//    rfalNfcfServBlockListParam servBlock;
//
//    servBlock.numServ   = 1;                            /* Only one Service to be used           */
//    servBlock.servList  = &srv;                         /* Service Code: NDEF is Read/Writeable  */
//    servBlock.numBlock  = 1;                            /* Only one block to be used             */
//    servBlock.blockList = bl;
//    bl[0].conf     = RFAL_NFCF_BLOCKLISTELEM_LEN_BIT;       /* Two-byte Block List Element           */
//    bl[0].blockNum = 0x0001;                            /* Block: NDEF Data                      */
//
//    err = rfalNfcfPollerCheck( nfcfDev.sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
//    platformLog(" Check Block: %s Data:  %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
//
//    uint8_t wrData[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
//
//    err = rfalNfcfPollerUpdate( nfcfDev->sensfRes.NFCID2, &servBlock, buf , sizeof(buf), wrData, buf, sizeof(buf) );
//    platformLog(" Update Block: %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( wrData, RFAL_NFCF_BLOCK_LEN) );
//    err = rfalNfcfPollerCheck( nfcfDev->sensfRes.NFCID2, &servBlock, buf, sizeof(buf), &rcvLen);
//    platformLog(" Check Block:  %s Data: %s \r\n", (err != ERR_NONE) ? "FAIL": "OK", (err != ERR_NONE) ? "" : hex2Str( &buf[1], RFAL_NFCF_BLOCK_LEN) );
//}

ReturnCode nfcfReadBlock(uint8_t *idm, uint8_t num_service,uint16_t *serviceList ,uint8_t num_block,uint16_t *blockList ,uint8_t blockdata[][16])
{
    rfalNfcfServBlockListParam servBlock;
    rfalNfcfBlockListElem _blockList[4];
    uint16_t rcvLen;
    uint8_t buf[ (RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN + (3*RFAL_NFCF_BLOCK_LEN)) ];

    servBlock.numServ   = num_service;
    servBlock.servList  = serviceList;
    servBlock.numBlock  = num_block;
    servBlock.blockList = _blockList;
    for(uint8_t i = 0;i<num_block;i++){
    	_blockList[i].conf = RFAL_NFCF_BLOCKLISTELEM_LEN_BIT;
    	_blockList[i].blockNum = blockList[i];
    }

    ReturnCode err = rfalNfcfPollerCheck(idm, &servBlock, buf, sizeof(buf), rcvLen);
    if(err != RFAL_ERR_NONE){
    	return err;
    }
//    platformLog(" Read Block: %s Data: %s \r\n",
//               (err != ERR_NONE) ? "FAIL" : "OK",
//               (err != ERR_NONE) ? "" : hex2Str(&buf[1], RFAL_NFCF_BLOCK_LEN));
    for(uint8_t i = 0;i<num_block;i++){
    	memcpy(blockdata[i][16],&buf[1 + (16*i)],16);
    }
    return err;
}

ReturnCode nfcfWriteSingleBlock(uint8_t *idm, uint8_t num_service,uint16_t *serviceList ,uint16_t *blockList,uint8_t *blockdata)
{
    rfalNfcfServBlockListParam servBlock;
    rfalNfcfBlockListElem _blockList[4];
    uint8_t buf[ (RFAL_NFCF_NFCID2_LEN + RFAL_NFCF_CMD_LEN + (3*RFAL_NFCF_BLOCK_LEN)) ];

    servBlock.numServ   = num_service;
    servBlock.servList  = serviceList;
    servBlock.numBlock  = 1;
    servBlock.blockList = _blockList;
	_blockList[0].conf = RFAL_NFCF_BLOCKLISTELEM_LEN_BIT;
	_blockList[0].blockNum = blockList[0];

    ReturnCode err = rfalNfcfPollerUpdate(idm, &servBlock, buf, sizeof(buf), blockdata, buf, sizeof(buf));

//    platformLog(" Write Block: %s Data: %s \r\n",
//               (err != ERR_NONE) ? "FAIL" : "OK",
//               (err != ERR_NONE) ? "" : hex2Str(wrData, RFAL_NFCF_BLOCK_LEN));

    return err;
}

//void demoNfcv(void)
//{
//    ReturnCode            err;
//    rfalNfcvListenDevice  nfcvDev  = {0};
//    uint16_t              rcvLen;
//    uint8_t               blockNum = 1;
//    uint8_t               rxBuf[ 1 + DEMO_NFCV_BLOCK_LEN + RFAL_CRC_LEN ];                        /* Flags + Block Data + CRC */
//    uint8_t *             uid;
//    uint8_t               wrData[DEMO_NFCV_BLOCK_LEN] = { 0x11, 0x22, 0x33, 0x99 };             /* Write block example */
//
//
//    uid = nfcvDev.InvRes.UID;
//
//    #if DEMO_NFCV_USE_SELECT_MODE
//        /*
//        * Activate selected state
//        */
//        err = rfalNfcvPollerSelect(RFAL_NFCV_REQ_FLAG_DEFAULT, nfcvDev->InvRes.UID );
//        platformLog(" Select %s \r\n", (err != ERR_NONE) ? "FAIL (revert to addressed mode)": "OK" );
//        if( err == ERR_NONE )
//        {
//            uid = NULL;
//        }
//    #endif
//
//    /*
//    * Read block using Read Single Block command
//    * with addressed mode (uid != NULL) or selected mode (uid == NULL)
//    */
//    err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
//    platformLog(" Read Block: %s %s\r\n", (err != ERR_NONE) ? "FAIL": "OK Data:", (err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
//
//        err = rfalNfcvPollerWriteSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, wrData, sizeof(wrData));
//        platformLog(" Write Block: %s Data: %s\r\n", (err != ERR_NONE) ? "FAIL": "OK", hex2Str( wrData, DEMO_NFCV_BLOCK_LEN) );
//        err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, sizeof(rxBuf), &rcvLen);
//        platformLog(" Read Block: %s %s\r\n", (err != ERR_NONE) ? "FAIL": "OK Data:", (err != ERR_NONE) ? "" : hex2Str( &rxBuf[1], DEMO_NFCV_BLOCK_LEN));
//}

ReturnCode nfcvReadBlock(rfalNfcvListenDevice *device, uint8_t blockNum, uint8_t *rxBuf, uint16_t bufSize, uint8_t *uid)
{
    uint16_t rcvLen;
    ReturnCode err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, rxBuf, bufSize, &rcvLen);
    platformLog(" Read Block: %s %s\r\n",
               (err != ERR_NONE) ? "FAIL" : "OK Data:",
               (err != ERR_NONE) ? "" : hex2Str(&rxBuf[1], DEMO_NFCV_BLOCK_LEN));
    return err;
}

ReturnCode nfcvWriteBlock(rfalNfcvListenDevice *device, uint8_t blockNum, uint8_t *wrData, uint16_t dataLen, uint8_t *uid)
{
    ReturnCode err = rfalNfcvPollerWriteSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, blockNum, wrData, dataLen);
    platformLog(" Write Block: %s Data: %s\r\n",
               (err != ERR_NONE) ? "FAIL" : "OK",
               hex2Str(wrData, DEMO_NFCV_BLOCK_LEN));
    return err;
}

ReturnCode mifareAuthenticate(uint8_t keyType, uint8_t sector, uint8_t* uid, uint32_t uidLen, uint8_t* key)
{
    const uint32_t nonce = 0x94857192;
    ReturnCode err = mccAuthenticate(keyType, sector, uid, uidLen, key, nonce);
    if(err != ERR_NONE) {
        //platformLog("Authentication failed:%04X\r\n",err);
//        mccDeinitialise(true);
    }
    return err;
}

ReturnCode mifareReadBlock(uint8_t sector, uint8_t block, uint8_t* buffer, uint16_t bufSize)
{
    uint8_t request[2] = {MCC_READ_BLOCK, (sector * 4) + block};
    uint16_t numBytesReceived;

    ReturnCode err = mccSendRequest(request, sizeof(request), buffer, bufSize,
                                  &numBytesReceived, MIFARE_READ_TIMEOUT, false);
    if(err == ERR_NONE) {
//        platformLog(" Read block %d: %s\r\n", block,
//                   hex2Str(buffer, bufSize - MIFARE_CRC_LEN));
    }
//    mccDeinitialise(true);
    return err;
}
