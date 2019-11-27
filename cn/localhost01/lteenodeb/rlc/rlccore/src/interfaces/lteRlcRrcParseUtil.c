/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRrcParseUtil.c,v 1.1.1.1.6.2.4.2.2.5 2010/10/20 05:36:07 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file contains the parsing APIs for the message received from RRC 
 *  Layer.
 *
 *  1. parseRRCMsg
 *  2. handleCommonChData
 *  3. handleReconfigCommCh
 *  4. rlcParseUeIndex
 *  5. rlcCreateUEContext 
 *  6. validateComChType 
 *  7. validatePollPDU
 *  8. validateTReordering 
 *  9. validatePollByte
 * 10. validateMaxReTxThrshld 
 * 11. validateStatusProhibit
 * 12. validatePollRetransmit
 * 13. rlcParseComChData
 * 14. rlcParseCreateMsg
 * 15. rlcParseReconfigMsg 
 * 16. rlcParseReEstablishMsg
 * 17. handleRlcCreateUEEntity
 * 18. handleRlcDeleteEntity
 * 19. handleRlcReconfigMsg
 * 20. handleRlcReEstablishMsg
 * 21. handleUeEntityReq
 * 22. sendCnfMsgToRRC
 * 23. prepareCnfBufferHeader
 * 24. createTMEntity
 * 25. parseRLCComChReconfigInfo
 * 26. parseCreateTxUMRlcEntity
 * 27. parseCreateRxUMRlcEntity
 * 28. parseCreateTxRxUMRlcEntity
 * 29. parseCreateRxTxAMEntity
 * 30. parseDelTxUMEntity 
 * 31. parseDelRxUMEntity
 * 32. parseDelTxRxUMEntity 
 * 33. parseDelAMEntity 
 * 34. parseReconfigTxUMEntity
 * 35. parseReconfigRxUMEntity 
 * 36. parseReconfigRxTxUMEntity
 * 37. parseReconfigAMEntity
 * 38. parseReEstablish
 * 39. parseRLCUeEntityInfo
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcRrcParseUtil.c,v $
*
*
****************************************************************************/
/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcRrcParseUtil.h"
#include "logging.h"
#include "lteRlcOamIntf.h"
#include "lteRlcErrorCode.h"
/* RLC EMBMS changes Starts */
#ifdef LTE_EMBMS_SUPPORTED
#include "lteRlcRrcEmbmsParseUtil.h"
#endif
/* RLC EMBMS changes Ends */


/****************************************************************************
  Private Definitions
 ****************************************************************************/
/****************************************************************************
  Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifndef UE_SIM_TESTING
extern UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
extern UInt16 setUeIdxValid(UInt16 ueIdx, UInt16 layer);
extern UInt16 setUeIdxInvalid(UInt16 ueIdx, UInt16 layer);
extern void freeUeIdx(UInt16 ueIdx);
#endif

extern void cleanupLayer2(void); 
extern UInt16 numOfSupportedUsers_g;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
extern UInt32 activeUEs_g;
#ifndef UE_SIM_TESTING

extern RlcSecondaryCellInfo secondaryCellInfo_g[MAX_NUM_CELL];

#endif
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
extern SInt32 rlcRrcCnfOutputQId_g;
#endif

/* SPR 16766 ++ */
#ifdef LOG_PRINT_ENABLED
extern tickType_t getCurrentTick(void);
#endif
/* SPR 16766 -- */
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/


/* */

/* */
//U8 to U16
inline STATIC UInt32 handleUeEntityReq( UInt16 msgId,    
                                  UInt8 *msg_p,
                                  UInt16 msgLen
                                      );

/* */

/* */
inline static UInt32 rlcParseUeIndex( UInt8* msg_p,
                                      UInt32 msgLen,
                                      UInt16 *ueIndex_p,
                                      UInt16 *current_p
     /* + SPR 20472 */ 
                                      ,UInt16 *ueIndex
     /* - SPR 20472 */ 
                                    );

/* */
inline static UInt32 rlcCreateUEContext( RlcConfigReq *rlcConfigReq_p
                                        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
                                         ,UInt16 rrcUeIndex
#endif
                                         ,InternalCellIndex  cellIndex
                                        /* - Layer2 NON CA Changes */
					 );

/* */

/* */
inline static UInt32 validatePollPDU ( UInt16 pollPDU );

/* */
inline static UInt32 validateTReordering ( UInt16 tReordering );

/* */
inline static UInt32 validatePollByte ( UInt16 pollByte );

/* */
inline static UInt32 validateMaxReTxThrshld( UInt16 maxRetxThreshold );

/* */
inline static UInt32 validateStatusProhibit( UInt16 tStatusProhibit);

/* */
inline static UInt32 validatePollRetransmit( UInt16 pollRetransmit );

/* */

/* */
inline static UInt32 rlcParseCreateMsg( UInt8 *msg_p,
                                        UInt16 msgLen,
                                        RlcConfigReq *rlcConfigReq_p,
                                        UInt16 *current_p,
                                        /* SPR 12917 fix start */   
                                        InternalCellIndex cellIndex
                                        /* SPR 12917 fix end */   
#ifndef UE_SIM_TESTING
                                        ,UInt16 *rrcUeIndex_p
#endif                                      
                                         );

/* */
inline static UInt32 rlcParseReconfigMsg ( UInt8 *msg_p,
        UInt16 msgLen,
        RlcConfigReq *rlcConfigReq_p,
        UInt16 *current_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex
        /* SPR 12917 fix end */   
        );

inline static UInt32 rlcParseChangeCrntiMsg ( UInt8 *msg_p, 
                                              UInt16 msgLen, 
                                              RlcChangeCrntiReq *rlcChangeCrntiReq_p, 
                                              UInt16 *current_p
     /* + SPR 20472 */ 
                                              ,UInt16 *ueIndex
     /* - SPR 20472 */ 
                                            );
/* */
inline static UInt32 handleRlcCreateUEEntity (
                                                UInt8 *msg_p,
                                                UInt16 msgLen
                                             );

/* */
inline static int handleRlcDeleteEntity (
                                          UInt8 *msg_p,
                                          UInt16 msgLen
                                        );

/* */
inline static int handleRlcReconfigMsg (
                                         UInt8 *msg_p,
                                         UInt16 msgLen
                                       );

/* */
inline static int handleRlcReEstablishMsg (
                                            UInt8 *msg_p,
                                            UInt16 msgLen
                                          );

inline static UInt32 handleRlcChangeCrntiMsg ( 
                                            UInt8 *msg_p,
                                            UInt16 msgLen
                                             );
/* */
inline STATIC void sendCnfMsgToRRC( UInt16 msgId,
                                    UInt16 transactionId,
                                    RlcConfigReq *rlcConfigReq_p,
                                    UInt16 resp,
                                    /* + Layer2 NON CA Changes */
                                    RrcCellIndex cellId
/* SPR 4690 Start */
#ifndef UE_SIM_TESTING
                                    , UInt16 rrcUeIndex
#endif
/* SPR 4690 End */
                                );
                                    /* - Layer2 NON CA Changes */

/* */
inline STATIC UInt32 parseRLCUeEntityInfo( UInt8* msg_p,
        RlcEntityCfgInfo *rlcEntityCfgInfo_p,
        UInt16 *current_p,
        UInt16 *msgLen_p,
        UInt16 ueIndex,
        UInt32 *ueBitRateFlag_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex
        /* SPR 12917 fix end */   
        );

/* */
inline static int parseReconfigAMEntity( UInt8 *msg_p,
                                         UInt16 *current_p,
                                         RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                       );

/* */
inline static int parseReconfigRxTxUMEntity( UInt8 *msg_p,
                                             UInt16 *current_p,
                                             RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                           );

/* */
inline static int parseReconfigRxUMEntity( UInt8 *msg_p,
                                           UInt16 *current_p,
                                           RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                         );

/* */
inline static int parseReconfigTxUMEntity(  UInt8 *msg_p,
                                            UInt16 *current_p,
                                            RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                          );

/* */
inline static int parseDelAMEntity( UInt8 *msg_p,
                                    UInt16 *current_p,
                                    RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                  );

/* */
inline static int parseDelTxRxUMEntity(
                                        UInt8 *msg_p,
                                        UInt16 *current_p,
                                        RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                      );

/* */
inline static int parseDelRxUMEntity(
                                      UInt8 *msg_p,
                                      UInt16 *current_p,
                                      RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                    );

/* */
inline static int parseDelTxUMEntity(
                                      UInt8 *msg_p,
                                      UInt16 *current_p,
                                      RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                    );

/* */
inline static int parseCreateRxTxAMEntity(
                                           UInt8* msg_p,
                                           UInt16 *current_p,
                                           RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                         );

/* */
inline static UInt32 parseCreateTxRxUMRlcEntity(
                                                  UInt8  *msg_p,
                                                  UInt16 *current_p,
                                                  RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                               );

/* */
inline static int parseCreateRxUMRlcEntity(
                                             UInt8 *msg_p,
                                             UInt16 *current_p,
                                             RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                           );

/* */
inline static int parseCreateTxUMRlcEntity(
                                             UInt8 *msg_p,
                                             UInt16 *current_p,
                                             RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                          );
#ifndef UE_SIM_TESTING
inline static int parseBitRateInfo(
                                     UInt8* msg_p,
                                     UInt16 *current_p,
                                     UInt16 ueIndex,
                                     UInt32 *ueBitRateFlag
                                   );

inline static UInt16 parseSpsConfigInfo( UInt8* msg_p,
        UInt16 *current_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex );
        /* SPR 12917 fix end */   

#endif
/* SPR 4994 changes start */
inline static UInt32 handleRlcReEstablishCompInd ( UInt8   *msg_p,
                                                   UInt16  msgLen );

inline static UInt32 rlcParseReEstabMsg ( UInt8 *msg_p,
                                          UInt16 msgLen,
                                          RlcReEstabReq *rlcReEstabCompInd_p,
                                          UInt16 *current_p
     /* + SPR 20472 */ 
                                          ,UInt16 *ueIndex
     /* - SPR 20472 */ 
                                         );

inline static UInt32 parseRLCReEstabLcInfo( UInt8* msg_p,
                                           RlcReEstabLcInfo *rlcReEstabLcInfo_p,
                                           UInt16 *current_p,
                                           UInt16 *msgLen_p
                                         );
void  rlcSendMsgToRrc(UInt8 *ui_CnfMsgBuf_p,
        UInt16 ui16_msgLen);

/*FSM changes started*/

typedef UInt32 (*lteRlcLPrioThrApiHandlerFuncPtr) (void*, SInt32);
STATIC lteRlcLPrioThrApiHandlerFuncPtr lteRlcLPrioThrApiHandlerFuncTable[RLC_CTRL_MAX_ST][RLC_CTRL_MAX_EV]; //= {lteMacInvalidEvent,};


/** CA Changes Start **/
UInt8 rlcProcessOAMRRCState_g[MAX_NUM_CELL] = {RLC_CTRL_IDLE_ST};
/** CA Changes End **/
LTE_SQUEUE rlcPendingRrcOamMsgQ_g;


UInt32 ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM(void* recvBuffer_p, SInt32 byteRead);
UInt32 ElRlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead);
UInt32 rlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead);

extern UInt32 rlcHandleSocketMsgs( void *, SInt32);

/* SPR 4994 changes end */

#ifndef UE_SIM_TESTING
STATIC inline UInt16 parseRlcSpsDlInfo( UInt16 *current,
        UInt16 subTagLen,
        UInt8 *msg_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex );
        /* SPR 12917 fix end */   
STATIC inline UInt16 parseRlcSpsUlInfo( UInt16 *current,
        UInt16 subTagLen,
        UInt8 *msg_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex );
        /* SPR 12917 fix end */   
#endif


/****************************************************************************
 * Function Name  : parseRRCMsg 
 * Inputs         : msgBuf - points to the Msg read from RRC socket,
 *                  bytesRead - number of bytes read
 * Outputs        : None
 * Returns        : RLC_FAILURE/ErrorCode
 * Description    : This parse the RRC message and based on msgId it call
 *                  the corresponding API.
 *                  1. RLC_CREATE_UE_ENTITY_REQ
 *                  2. RLC_DELETE_UE_ENTITY_REQ
 *                  3. RLC_RECONFIG_UE_ENTITY_REQ 
 *                  4. RLC_RE_ESTABLISH_UE_ENTITY_REQ   
 *                  5. RLC_COMMON_CHANNEL_DATA_REQ
 *                  6. RLC_CONFIG_COMMON_CHANNEL_ENTITIES_REQ
 ****************************************************************************/


/* SPR 5599_129119 Fix Start */
UInt32 parseRRCMsg ( UInt8* msgBuf, 
                            SInt32 bytesRead
/* SPR 5599_129119 Fix End */
                          )
{
    UInt16 msgId          = 0;
    UInt16 msgLen         = 0;
    UInt16 current        = 0;
    UInt32 errorCode      = INVALID_ERR_CODE;

    /* - Layer2 NON CA Changes */
    /* Parse the Header of recieved buffer */
    /*
       Transaction ID   -  2 bytes
       Src Module ID    -  2 bytes
       Dest. Module ID  -  2 bytes
       API - Identifier -  2 bytes
       Length           -  2 bytes
    */
    /* + SPR#5898 */
    //transactionId = LTE_GET_U16BIT(msgBuf + current);
    /* - SPR#5898 */
    current += 6;
    msgId = LTE_GET_U16BIT(msgBuf + current);
    current += 2;
    msgLen = LTE_GET_U16BIT(msgBuf + current);
    current += 2;

/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    current += 1;
#endif

    if ( bytesRead != msgLen ) 
    {
    /* + SPR#5898 */
        /*LOG_RLC_MSG(LTE_RLC_PARSE_RRC_MSG_ERR, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__,
                transactionId, bytesRead, msgLen, 0, 0, 0, __func__,
                "");*/
        LOG_RLC_MSG(LTE_RLC_PARSE_RRC_MSG_ERR, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__,
                LTE_GET_U16BIT(msgBuf + 0), bytesRead, msgLen, 0, 0, 0, __func__,
                "");
    /* - SPR#5898 */

        return RLC_RRC_RX_INVALID_MSG_LEN;
    }   
 
    msgLen -= RRC_RLC_API_HEADER_LEN;

    switch ( msgId ) 
    {
        case RLC_CREATE_UE_ENTITY_REQ:
        case RLC_DELETE_UE_ENTITY_REQ:
        case RLC_RECONFIG_UE_ENTITY_REQ:
        case RLC_RE_ESTABLISH_UE_ENTITY_REQ:
        case RLC_CHANGE_CRNTI_REQ:       
        /* SPR 4994 Changes Start */
        case RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND: 
        /* SPR 4994 Changes End */
            errorCode = handleUeEntityReq ( msgId, msgBuf, msgLen);

            break;
        /* RLC EMBMS changes Start */
#ifdef LTE_EMBMS_SUPPORTED
        /* Block for handling RRC-RLC eMBMS APIs Start */

        case RLC_CREATE_MBMS_AREA_ENTITY_REQ:
        case RLC_DELETE_MBMS_AREA_ENTITY_REQ:
        case RRC_RLC_AREA_CONFIG_REQUEST:
        case RRC_RLC_MCCH_IND:
            
            errorCode = handleAreaEntityReq( msgId, msgBuf, msgLen);

        break;
        
        /* Block for handling RRC-RLC eMBMS APIs End */
#endif
        /* RLC EMBMS changes End */

        default:

    /* + SPR#5898 */
        /*LOG_RLC_MSG( LTE_RLC_PARSE_RRC_MSG_ERR_1, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__,
                transactionId,bytesRead, msgLen, msgId, 0, 0, __func__,
                "");*/
        LOG_RLC_MSG( LTE_RLC_PARSE_RRC_MSG_ERR_1, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__,
                LTE_GET_U16BIT(msgBuf + 0),bytesRead, msgLen, msgId, 0, 0, __func__,
                "");
    /* - SPR#5898 */

            errorCode = RLC_RRC_INVALID_RRC_MSG_ID;     

            break;
    }
    return errorCode;
}

/****************************************************************************
 * Function Name  : rlcParseUeIndex
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - Length of msg to be read,
 *                  ueIndex_p - pointer to RRC ueIndex,
 *                  current_p - tells the current position of msg
 *                  ueIndex   - pointer to Internal ueIndex,
 * Outputs        : RLC_SUCCESS if parsed successfully error msg otherwise
 * Returns        : None
 * Description    : This API is to parse and validate the ueIndex.
 ****************************************************************************/

inline static UInt32 rlcParseUeIndex( UInt8* msg_p, 
                                      UInt32 msgLen, 
                                      UInt16 *ueIndex_p,
                                      UInt16 *current_p
                                      /* + SPR 20472 */ 
                                      ,UInt16 *ueIndex
                                      /* - SPR 20472 */ 
                                    )
{
    UInt16 current = *current_p;
    UInt16 resp    = RLC_SUCCESS;
    
    if ( msgLen < (RECNFG_REESTAB_DEL_UE_MANDATORY_LENGTH ))
    {
        LOG_RLC_MSG( LTE_RLC_PARSE_UE_IDX_ERR, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__, msgLen,
        0, 0, 0, 0, 0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }
   /*SPR 21653 changes start*/ 
    *ueIndex = LTE_GET_U16BIT(msg_p + current);
    /*SPR 17139 fix start*/
     /* + SPR 20472 */
#ifndef  UE_SIM_TESTING    
     *ueIndex_p  = getUeIdxFromRrcUeIdx(*ueIndex);
#else
     *ueIndex_p  = *ueIndex;
   /*SPR 21653 changes end*/ 
#endif    
     /* - SPR 20472 */
    /*SPR 17139 fix end*/

    /*17392 + -*/

    current += 2;
   /*SPR 21653 changes start*/ 
    /* + SPR 20472 */ 
    resp = IS_UE_ID_INVALID(*ueIndex_p);
    /* - SPR 20472 */ 
    if ( resp != RLC_SUCCESS )
    {
        LOG_RLC_MSG( LTE_RLC_PARSE_UE_IDX_ERR_1, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__,*ueIndex , msgLen,
        0, 0, 0, 0, __func__, "");
        return RLC_INVALID_UE_ID;
    }
    /* + SPR 20472 */ 
    resp = IS_UE_ID_NOT_EXISTS(*ueIndex_p);                        
    /* - SPR 20472 */ 
    if ( RLC_SUCCESS == resp )
    {
        LOG_RLC_MSG( LTE_RLC_PARSE_UE_IDX_ERR_2, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__,*ueIndex , msgLen,
        0, 0, 0, 0, __func__, "UE does not exist");
   /*SPR 21653 changes end*/ 
        return RLC_UE_NOT_EXISTS;
    }
    *current_p = current;
    return RLC_SUCCESS; 
}

/****************************************************************************
 * Function Name  : rlcCreateUEContext
 * Inputs         : rlcConfigReq_p - Pointer to RlcConfigReq,
 *                  rrcUeIndex - RRC UE Index,
 *                  cellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS if the UE Context created successfully 
 *                  otherwise error msg
 * Description    : This API is to create the UE context and initialize it.
 ****************************************************************************/

inline static UInt32 rlcCreateUEContext( RlcConfigReq *rlcConfigReq_p
#ifndef UE_SIM_TESTING
                                         ,UInt16   rrcUeIndex
                                        /* + Layer2 NON CA Changes */
#endif
                                         ,InternalCellIndex  cellIndex
                                        /* - Layer2 NON CA Changes */
                                        )
{

    UInt8 idx          = 0;
    UInt8 resp         = RLC_FAILURE;
    UInt8 noOfFailures = 0;

    /* creating the ueContext at ueIndex */
    if ( RLC_NULL == rlcDbAllocateUEContext(rlcConfigReq_p->ueIndex))
    {
        LOG_RLC_MSG(LTE_RLC_CREATE_UE_CTX_ERR_4, LOGFATAL, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcConfigReq_p->ueIndex, 0, 0,
                0, 0,0, __func__, "");
        return RLC_FAILURE;
    }
    UEContext* outUEContext_p = DB_FETCH_UE_CONTEXT(rlcConfigReq_p->ueIndex);
    /* + Layer2 NON CA Changes */
    outUEContext_p->cellIndex = cellIndex;
    /* - Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    outUEContext_p->rrcUeIndex = rrcUeIndex;
#endif
    activeUEs_g++;
    
    /* creating UE Context */
    for ( idx = 0; idx < rlcConfigReq_p->lcCount; idx++)
    {
        resp = rlcCreateUEEntity ( rlcConfigReq_p->ueIndex,
                rlcConfigReq_p->ueId,
                &(rlcConfigReq_p->rlcEntityCfgList[idx]
                 ));

        rlcConfigReq_p->rlcEntityCfgList[idx].result = resp;

        if ( RLC_SUCCESS != resp )
        {
            noOfFailures++;
        }
    }/* end forloop */

#ifndef UE_SIM_TESTING
/* + Layer2 NON CA Changes */
    /* if SPS is enabled by OAM in system, configure SPS to UE and LCs */
    if( LTE_TRUE == secondaryCellInfo_g[cellIndex].enableSps )
    {
        resp = rlcConfigSpsInfo( rlcConfigReq_p->ueIndex, rlcConfigReq_p );
        if( RLC_SUCCESS != resp )   
        {
            noOfFailures++;
        }
    }
/* - Layer2 NON CA Changes */
#endif
    if ( noOfFailures > 0)
    {
        resp = RLC_PARTIAL_SUCCESS;
    }

    gRlcStats.pRlcStats->lteSysStat.numActiveUE ++;
    gRlcStats.pRlcStats->lteRlcUeStat[rlcConfigReq_p->ueIndex].
        numActiveLC = (rlcConfigReq_p->lcCount - noOfFailures);
	/* + PERF_CA GBL */
    lteRlcUEStatInfo *lteUEStat_p = &(lteRlcStatInfo_g[cellIndex].lteUEStat);
	/* - PERF_CA GBL */
    lteUEStat_p->numOfActiveUE++;
    lteUEStat_p->rlcUeStats[(lteUEStat_p->numOfActiveUE) - 1].crnti = 
        rlcConfigReq_p->ueId;

    return resp; 
}



/****************************************************************************
 * Function Name  : validatePollPDU 
 * Inputs         : pollPDU
 * Outputs        : None
 * Returns        : RLC_SUCCESS if pollPDU is valide otherwise error message 
 * Description    : This API is used to check the validation of pollPDU.
 ****************************************************************************/

inline static UInt32 validatePollPDU ( UInt16 pollPDU )
{
    switch(pollPDU){
        case p4:
        case p8:
        case p16:
        case p32:
        /*SPR 8407 Fix Start*/
        case p64:
        /*SPR 8407 Fix End*/
        case p128:
        case p256:
        case pInfinity:
            return RLC_SUCCESS;
            break;
    }
    return RLC_INVALID_POLL_PDU;
}

/****************************************************************************
 * Function Name  : validateTReordering 
 * Inputs         : tReordering
 * Outputs        : None
 * Returns        : RLC_SUCCESS if tReordering is valid else error message
 * Description    : This API is used to check the validation of tReordering.
 ****************************************************************************/

inline static UInt32 validateTReordering(UInt16 tReordering)
{
    switch(tReordering){
 /* SPR 3273 Fix Start */
        case rms0:
 /* SPR 3273 Fix End */
        case rms5: 
        case rms10:
        case rms15:
        case rms20:
        case rms25:
        case rms30:
        case rms35:
        case rms40:
        case rms45: 
        case rms50:
        case rms55:
        case rms60:
        case rms65:
        case rms70:
        case rms75:
        case rms80:
        case rms85: 
        case rms90:
        case rms95:
        case rms100:
        case rms110:
        case rms120:
        case rms130:
        case rms140:
        case rms150:
        case rms160:
        case rms170:
        case rms180:
        case rms190:
        case rms200:
        case 2000:
            return RLC_SUCCESS;
            break;
    }
    return RLC_SYNTAX_ERROR;
}

/****************************************************************************
 * Function Name  : validatePollByte 
 * Inputs         : pollByte
 * Outputs        : None
 * Returns        : RLC_SUCCESS if pollByte is valide error message otherwise
 * Description    : This API checks the validation for the PollByte. 
 ****************************************************************************/

inline static UInt32 validatePollByte(UInt16 pollByte)
{
    switch(pollByte){
        case kb25: 
        case kb50: 
        case kb75: 
        case kb100: 
        case kb125: 
        case kb250: 
        case kb375: 
        case kb500: 
        case kb750: 
        case kb1000: 
        case kb1250: 
        case kb1500: 
        case kb2000: 
        case kb3000:
        case kbinfinity:
            return RLC_SUCCESS;
            break;
    }
    return RLC_INVALID_POLL_BYTE;
}    

/****************************************************************************
 * Function Name  : validateMaxReTxThrshld 
 * Inputs         : maxRetxThreshold
 * Outputs        : None
 * Returns        : RLC_SUCCESS if maxRetxThreshold is valide error message
 *                  otherwise
 * Description    : This API is used to check the validation for 
 *                  maxRetxThreshold.
 ****************************************************************************/

inline static UInt32 validateMaxReTxThrshld( UInt16 maxRetxThreshold )
{
    switch(maxRetxThreshold){
        case t1:
        case t2:
        case t3:
        case t4:
        case t6:
        case t8:
        case t16:
        case t32:
            return RLC_SUCCESS;
            break;
    }
    return RLC_INVALID_MAX_RETRA_THRESHLD;
}    

/****************************************************************************
 * Function Name  : validateStatusProhibit 
 * Inputs         : tStatusProhibit
 * Outputs        : None
 * Returns        : RLC_SUCCESS if tStatusProhibit is valide error message 
 *                  otherwise
 * Description    : This API is used to validate the tStatusProhibit.
 ****************************************************************************/

inline static UInt32 validateStatusProhibit( UInt16 tStatusProhibit)
{
    switch(tStatusProhibit){
        case spms0:
        case spms5:
        case spms10:
        case spms15:
        case spms20:
        case spms25:
        case spms30:
        case spms35:
        case spms40:
        case spms45:
        case spms50:
        case spms55:
        case spms60:
        case spms65:
        case spms70:
        case spms75:
        case spms80:
        case spms85:
        case spms90:
        case spms95:
        case spms100:
        case spms105:
        case spms110:
        case spms115:
        case spms120:
        case spms125:
        case spms130:
        case spms135:
        case spms140:
        case spms145:
        case spms150:
        case spms155:
        case spms160:
        case spms165:
        case spms170:
        case spms175:
        case spms180:
        case spms185:
        case spms190:
        case spms195:
        case spms200:
        case spms205:
        case spms210:
        case spms215:
        case spms220:
        case spms225:
        case spms230:
        case spms235:
        case spms240:
        case spms245:
        case spms250:
        case spms300:
        case spms350:
        case spms400:
        case spms450:
        case spms500:
            return RLC_SUCCESS;
            break;
    }
    return RLC_INVALID_STATUS_PROHBT;
}

/****************************************************************************
 * Function Name  : validatePollRetransmit 
 * Inputs         : pollRetransmit
 * Outputs        : None
 * Returns        : RLC_SUCCESS if pollRetransmit is valid, error message 
 *                  otherwise
 * Description    : This API is used to validate the pollRetransmit.
 ****************************************************************************/

inline static UInt32 validatePollRetransmit( UInt16 pollRetransmit )
{
    switch ( pollRetransmit )
    {
        case prms5:
        case prms10:
        case prms15:
        case prms20:
        case prms25:
        case prms30:
        case prms35:
        case prms40:
        case prms45:
        case prms50:
        case prms55:
        case prms60:
        case prms65:
        case prms70:
        case prms75:
        case prms80:
        case prms85:
        case prms90:
        case prms95:
        case prms100:
        case prms105:
        case prms110:
        case prms115:
        case prms120:
        case prms125:
        case prms130:
        case prms135:
        case prms140:
        case prms145:
        case prms150:
        case prms155:
        case prms160:
        case prms165:
        case prms170:
        case prms175:
        case prms180:
        case prms185:
        case prms190:
        case prms195:
        case prms200:
        case prms205:
        case prms210:
        case prms215:
        case prms220:
        case prms225:
        case prms230:
        case prms235:
        case prms240:
        case prms245:
        case prms250:
        case prms300:
        case prms350:
        case prms400:
        case prms450:
        case prms500:
            return RLC_SUCCESS;
            break;
    }
    return RLC_SYNTAX_ERROR;
}


/****************************************************************************
 * Function Name  : rlcParseCreateMsg
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread,
 *                  rlcConfigReq_p - Pointer to RLC Config structure,
 *                  current_p - Pointer to current Idx,
 *                  rrcUeIndex_p - Pointer to RRC UE Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS on success and error code on failure
 * Description    : This API is to parse the create msg read and return error
 *                  code or RLC_SUCCESS.
 ****************************************************************************/

inline static UInt32 rlcParseCreateMsg( UInt8 *msg_p, 
        UInt16 msgLen, 
        RlcConfigReq *rlcConfigReq_p, 
        UInt16 *current_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex
        /* SPR 12917 fix end */   
#ifndef UE_SIM_TESTING
        ,UInt16 *rrcUeIndex_p
#endif
        )
{
    UInt16 current = *current_p;
    UInt16 resp    = RLC_SUCCESS;
    UInt16 lcCount = 0;
    UInt32 ueBitRateFlag = RLC_FALSE;

    if ( msgLen < (ADD_RLC_UE_MANDATORY_LENGTH ))
    {
        LOG_RLC_MSG(LTE_RLC_PARSE_CREATE_MSG_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                msgLen, 0, 0, 0, 0, 0,
                __func__, "");

        return RLC_SYNTAX_ERROR;
    }
    /* Mandatory Parameter of RLC_CREATE_UE_ENTITY_REQ API :-
          Parameter           Bytes    Range 
       1. UE Index     -    2 bytes  ( 0 - 299 )
       2. Rnti         -    2 bytes  ( 0x0001 - 0xFFF3 )     
    */
#ifndef UE_SIM_TESTING
    *rrcUeIndex_p = LTE_GET_U16BIT(msg_p + current);
    rlcConfigReq_p->ueIndex = getUeIdxFromRrcUeIdx(*rrcUeIndex_p);
#else
    rlcConfigReq_p->ueIndex = LTE_GET_U16BIT(msg_p + current);
#endif
    current += 2;

    /* Validate the UE Index */
    resp = IS_UE_ID_INVALID(rlcConfigReq_p->ueIndex);
    if ( resp != RLC_SUCCESS )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_CREATE_MSG_ERR_1, LOGWARNING, 
                  RLC_RRC, globalTTITickCount_g, __LINE__,
                  msgLen, rlcConfigReq_p->ueIndex, 0, 0,
                  0, 0, __func__, "");

        return RLC_INVALID_UE_ID;
    }
    resp = IS_UE_ID_NOT_EXISTS(rlcConfigReq_p->ueIndex);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_CREATE_MSG_ERR_2, LOGWARNING, 
                  RLC_RRC, globalTTITickCount_g, __LINE__,
                  msgLen, rlcConfigReq_p->ueIndex, 0,
                  0, 0, 0,  __func__, "");
        return RLC_UE_ID_EXISTS;
    }

    /* Parse the RNTI Value (  0x0001 - 0xFFF3)*/
    rlcConfigReq_p->ueId = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    msgLen -= ADD_RLC_UE_MANDATORY_LENGTH;

    while ( msgLen > 4)
    {
        if ( lcCount >= MAX_LC_ID )
        {
            /* + Layer2 NON CA Changes */
            LOG_RLC_MSG ( LTE_RLC_PARSE_CREATE_MSG_ERR_4, LOGWARNING, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    rlcConfigReq_p->ueId, 
                    0, 0,
                    lcCount, 0, 0, __func__, "");
            return RLC_SYNTAX_ERROR;
        }
        /* - Layer2 NON CA Changes */
        /* Parse the recieved buffer and check the following optional API :-
           1. CREATE_TX_UM_RLC_ENTITY
           2. CREATE_RX_UM_RLC_ENTITY
           3. CREATE_TX_RX_UM_RLC_ENTITY
           4. CREATE_TX_RX_AM_RLC_ENTITY
           5. RLC_UE_BIT_RATE_INFO
           6. RLC_SPS_CONFIG
       */

        resp = parseRLCUeEntityInfo( msg_p,
                   &(rlcConfigReq_p->rlcEntityCfgList[lcCount]), 
                    &current, &msgLen,rlcConfigReq_p->ueIndex, &ueBitRateFlag,
                    rlcConfigReq_p, 
                    /* SPR 12917 fix start */
                    cellIndex );
                    /* SPR 12917 fix end */
        if ( RLC_SUCCESS != resp)
        {
            return resp;
        }
        else if ( RLC_ADD_ENTITY != 
                rlcConfigReq_p->rlcEntityCfgList[lcCount].opCode 
                && (!ueBitRateFlag) 
#ifndef UE_SIM_TESTING
                && !( rlcConfigReq_p->spsBitmask == 0x01 || rlcConfigReq_p->spsBitmask == 0x02 ||
                     rlcConfigReq_p->spsBitmask == 0x03) 
#endif
                )
        {
            return RLC_SYNTAX_ERROR;
        }    
        else 
        {
            if((!ueBitRateFlag)
#ifndef UE_SIM_TESTING
                && !( rlcConfigReq_p->spsBitmask == 0x01 || rlcConfigReq_p->spsBitmask == 0x02 ||
                     rlcConfigReq_p->spsBitmask == 0x03) 
#endif
                    )
            {
                lcCount++;
            }
            ueBitRateFlag =RLC_FALSE;
        }   
    }
    if ( msgLen > 0 )
    {
        return RLC_SYNTAX_ERROR;
    }    
    rlcConfigReq_p->lcCount = lcCount;
    *current_p = current;
    return RLC_SUCCESS;            
}

/****************************************************************************
 * Function Name  : rlcParseReconfigMsg
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread,
 *                  rlcConfigReq_p - Pointer to RLC Config structure,
 *                  current_p - Pointer to current Idx
 * Outputs        : None
 * Returns        : RLC_SUCCESS on success and error code on failure
 * Description    : This API is to parse the reconfig msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/

inline static UInt32 rlcParseReconfigMsg( UInt8 *msg_p, 
        UInt16 msgLen, 
        RlcConfigReq *rlcConfigReq_p, 
        UInt16 *current_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex
        /* SPR 12917 fix end */   
        )
{
    UInt16 current = *current_p;
    UInt16 resp = RLC_SUCCESS;
    UInt16 lcCount = 0;
    UInt32 ueBitRateFlag = RLC_FALSE;
    /* + SPR 20472 */ 
    UInt16 ueIndex = INVALID_UE_INDEX;

    resp = rlcParseUeIndex(msg_p,msgLen,&(rlcConfigReq_p->ueIndex),&current,&ueIndex);
    /* - SPR 20472 */ 
    if(RLC_SUCCESS != resp)
    {
        return resp;
    }    
    msgLen -= RECNFG_REESTAB_DEL_UE_MANDATORY_LENGTH;

    /* Parse Optional IEs if present */
    while ( msgLen > 4)
    {
        if ( lcCount >= MAX_LC_ID )
        {
            return RLC_SYNTAX_ERROR;
        }   
        resp = parseRLCUeEntityInfo(msg_p,
                &(rlcConfigReq_p->rlcEntityCfgList[lcCount]), 
                /* + SPR 20472 */
/*SPR 21653 changes start*/ 
                &current, &msgLen, rlcConfigReq_p->ueIndex, &ueBitRateFlag, 
/*SPR 21653 changes end*/ 
                /* - SPR 20472 */ 
                rlcConfigReq_p,
                /* SPR 12917 fix start */
                cellIndex );
        /* SPR 12917 fix end */

        if ( RLC_SUCCESS != resp )
        {
            return resp;
        }
        else if ( !((RLC_ADD_ENTITY 
                        == rlcConfigReq_p->rlcEntityCfgList[lcCount].opCode)
                    || (RLC_DELETE_ENTITY 
                        == rlcConfigReq_p->rlcEntityCfgList[lcCount].opCode)
                    || (RLC_RECONFIG_ENTITY 
                        == rlcConfigReq_p->rlcEntityCfgList[lcCount].opCode)) 
                && (!ueBitRateFlag)
#ifndef UE_SIM_TESTING
                && !( rlcConfigReq_p->spsBitmask == 0x01 || rlcConfigReq_p->spsBitmask == 0x02 ||
                    rlcConfigReq_p->spsBitmask == 0x03) 
#endif
                )
        {
            return RLC_SYNTAX_ERROR;
        }
        else 
        {
            if((!ueBitRateFlag) 
#ifndef UE_SIM_TESTING
                    && !( rlcConfigReq_p->spsBitmask == 0x01 || rlcConfigReq_p->spsBitmask == 0x02 ||
                        rlcConfigReq_p->spsBitmask == 0x03)
#endif
              )
            {
                lcCount++;
            }
            ueBitRateFlag = RLC_FALSE;
        }   
    }
    if ( msgLen > 0)
    {
        return RLC_SYNTAX_ERROR;
    }    
    rlcConfigReq_p->lcCount = lcCount;
    *current_p = current;
    return RLC_SUCCESS;            
}

/****************************************************************************
 * Function Name  : handleRlcCreateUEEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : ErrorCode 
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/

inline static UInt32 handleRlcCreateUEEntity (   
                                                UInt8 *msg_p,
                                                UInt16 msgLen
                                             )
{ 
    /* Buffer Header is of 10 bytes */
    UInt16 current            = RRC_RLC_API_HEADER_LEN ;  
    RlcConfigReq rlcConfigReq = {0};
    UInt16 resp               = RLC_SUCCESS;
    UInt16 transactionId      = 0;
    UInt32 errorCode          = 0;
#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex          = 0;
    RrcCellIndex cellId = msg_p[API_HEADER_CELL_INDEX_OFFSET];

    /* SPR 12917 fix start */   
    InternalCellIndex cellIndex = RLC_GET_INTERNAL_CELL_INDEX( cellId );

    if(INVALID_CELL_INDEX == cellIndex)
    {
        errorCode = resp = RLC_FAILURE;
        LOG_RLC_MSG(LTE_RLC_RRC_HANDLE_CREATE_UE_ENTITY_REQ_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                RLC_CREATE_UE_ENTITY_REQ, activeUEs_g, numOfSupportedUsers_g,
                0, 0,0, __func__, "");
    }
    /* SPR 12917 fix end */   
#else
    RrcCellIndex cellId = 0;
    InternalCellIndex cellIndex = msg_p[API_HEADER_CELL_INDEX_OFFSET];
    cellId = cellIndex;
    current++;
    msgLen -=1;
#endif
    /* SPR 12917 fix start */   
    if( RLC_SUCCESS == resp )
    {
        /* SPR 12917 fix end */   
        transactionId = LTE_GET_U16BIT(msg_p);

        if ( activeUEs_g < numOfSupportedUsers_g)
        {
            /* Parsing the RLC Create UE Entity*/
            resp = rlcParseCreateMsg( msg_p, 
                    msgLen, 
                    &rlcConfigReq, 
                    &current,
                    /* SPR 12917 fix start */   
                    cellIndex
                    /* SPR 12917 fix end */   
#ifndef UE_SIM_TESTING
                    ,&rrcUeIndex
#endif
                    );
            errorCode = resp;
        }
        else
        {
            LOG_RLC_MSG(LTE_RLC_RRC_HANDLE_CREATE_UE_ENTITY_REQ_ERR, LOGWARNING, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    RLC_CREATE_UE_ENTITY_REQ, activeUEs_g, numOfSupportedUsers_g,
                    0, 0,0, __func__, "");

            errorCode = resp = RLC_MAX_ACTIVE_UE_REACHED_ERR;
        }
    }

    if ( RLC_SUCCESS == resp)
    {
        /* creating UE Context */
        resp = rlcCreateUEContext( &rlcConfigReq
#ifndef UE_SIM_TESTING
                ,rrcUeIndex
#endif        
                ,cellIndex
                );
#ifndef UE_SIM_TESTING
        /* SPR 4864 fix start */
        setUeIdxValid(getUeIdxFromRrcUeIdx(rrcUeIndex), UE_IDX_MAPPING_FOR_RLC);
        /* SPR 4864 fix end */
#endif        
        errorCode = resp;
    }
    else
    {
        /* SPR 16717 fix start */
        resp = RLC_FAILURE;
        /* SPR 16717 fix end */
    }

    /* SPR 4690 Start */
    /* Sending Confirmation to RRC */
#ifndef UE_SIM_TESTING
    sendCnfMsgToRRC ( RLC_CREATE_UE_ENTITY_CNF, transactionId, &rlcConfigReq, resp, cellId,rrcUeIndex);
#else
    sendCnfMsgToRRC ( RLC_CREATE_UE_ENTITY_CNF, transactionId, &rlcConfigReq, resp,cellId);
#endif
    /* SPR 4690 End */

    LOG_RLC_MSG( LTE_RLC_RRC_HANDLE_CREATE_UE_ENTITY_REQ, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            RLC_CREATE_UE_ENTITY_REQ, activeUEs_g, resp, 0, 0, 0, __func__, "");

    return errorCode;
}

/****************************************************************************
 * Function Name  : lteRlcInvalidEvent
 * Inputs         : recvBuffer_p - Pointer to Received Buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This API is used to check LTE RLC Invalid Event.
 ****************************************************************************/
UInt32 lteRlcInvalidEvent( void* recvBuffer_p, SInt32 byteRead)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(byteRead)
  LTE_GCC_UNUSED_PARAM(recvBuffer_p)
    /* +- SPR 17777 */
    lteWarning("Invalid Event RLC: Event is not handled\n");
    return RLC_SUCCESS;
}


/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING

/****************************************************************************
 * Function Name  : lteRlcEnqueueCtrlReqLPHandler
 * Inputs         : recvBuffer_p - Pointer to Received Buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC Enqueue Control
 *                  requests.
 ****************************************************************************/
UInt32 lteRlcEnqueueCtrlReqLPHandler(void* recvBuffer_p, SInt32 byteRead)
{
    RlcPendingRrcOamMsgNode * rlcPendingRrcOamMsg_p = getMemFromPool(sizeof(RlcPendingRrcOamMsgNode),PNULL);

    if(PNULL == rlcPendingRrcOamMsg_p)
    {
        lteWarning("No more memory left [%s]\n",__func__);
        /* SPR 20469 Fix + */
        freeMemPool(recvBuffer_p);
        /* SPR 20469 Fix - */
        return RLC_FAILURE;
    }
    rlcPendingRrcOamMsg_p->msg_p = recvBuffer_p;
    rlcPendingRrcOamMsg_p->msgLenth = byteRead;
   
    /* + Coverity 32535 */
    if( pushNode(&rlcPendingRrcOamMsgQ_g,&(rlcPendingRrcOamMsg_p->anchorNode)) )
    {
        /* SPR 20469 Fix + */
        freeMemPool(recvBuffer_p);
        /* SPR 20469 Fix - */
        freeMemPool(rlcPendingRrcOamMsg_p);
        rlcPendingRrcOamMsg_p = RLC_NULL;
        return RLC_FAILURE ;
    }
    /* + Coverity 32535 */
    
    return RLC_SUCCESS;
}



/*SPR 6631 Start*/

/****************************************************************************
 * Function Name  : lteRlcUeContextNotExistLPHandler
 * Inputs         : msgBuf - Message Buffer,
 *                  byteRead - Bytes read,
 *                  resp_conf - Response Conf
 * Outputs        : None
 * Returns        : RLC_SUCCESS on success and error code on failure
 * Description    : This API is used to handle LTE RLC Context not exist
 *                  requests.
 ****************************************************************************/
UInt32 lteRlcUeContextNotExistLPHandler(void * msgBuf, SInt32 byteRead,UInt16 resp_conf)
/*SPR 6631 End*/
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(byteRead)
    /* +- SPR 17777 */
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)msgBuf;
    UInt16 cnfMsgIdx = 0;

    /* + Layer2 NON CA Changes */
    UInt16 transactionId = node_p->transactionId;
    RrcCellIndex cellId = node_p->cellId;
    /* - Layer2 NON CA Changes */
    UInt16 ueIndex = node_p->ueIndex;
    UInt8 cnfMsgBuf[MAX_RES_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(node_p->cellId);
    /** CA Changes End **/
    freeMemPool(node_p);
    node_p = RLC_NULL;

    cnfMsgIdx =  RRC_RLC_API_HEADER_LEN;
    resp_p = cnfMsgBuf;

#ifndef UE_SIM_TESTING
/* SPR 22337_22360 Fix Start */
    if(PNULL==rlcUEContextList_g[ueIndex])
    {
        LTE_SET_U16BIT(( resp_p + cnfMsgIdx),getRrcUeIdxFromUeIdx(ueIndex));
    }
    else
    {
        LTE_SET_U16BIT(( resp_p + cnfMsgIdx), rlcUEContextList_g[ueIndex]->rrcUeIndex);
    }
/* SPR 22337_22360 Fix End */
#else
    LTE_SET_U16BIT(( resp_p + cnfMsgIdx),ueIndex);
#endif
    cnfMsgIdx += 2;

    LTE_SET_U16BIT((resp_p + cnfMsgIdx),RLC_UE_NOT_EXISTS);
    cnfMsgIdx += 2;

    /* + Layer2 NON CA Changes */
    /* preparing header for confirmation */
    prepareCnfBufferHeader( resp_p, resp_conf, transactionId, 
            cellId, cnfMsgIdx);
    /* + Layer2 NON CA Changes */

    /* writting to socket */
    rlcSendMsgToRrc(cnfMsgBuf,
            cnfMsgIdx);
    activeUEs_g--;
    /** CA Changes Start **/
    if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
    {
        rlcProcessOAMRRCState_g[internalCellIndex] = RLC_CTRL_IDLE_ST;
    }
    /** CA Changes End **/
    return RLC_SUCCESS;
}


/*SPR 6631 Start*/

/****************************************************************************
 * Function Name  : lteRlcUeCNEForDeleteUEContextLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This API is used to handle LTE RLC UE CNE for Delete UE
 *                  Context.
 ****************************************************************************/
UInt32 lteRlcUeCNEForDeleteUEContextLPHandler(void * msgBuf, SInt32 byteRead)
{
   return lteRlcUeContextNotExistLPHandler(msgBuf,byteRead,RLC_DELETE_UE_ENTITY_CNF);
}

/****************************************************************************
 * Function Name  : lteRlcUeCNEForDeleteUEEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC UE CNE for Delete UE
 *                  Entity.
 ****************************************************************************/
UInt32 lteRlcUeCNEForDeleteUEEntityLPHandler(void * msgBuf, SInt32 byteRead)
{
   return lteRlcUeContextNotExistLPHandler(msgBuf,byteRead,RLC_RECONFIG_UE_ENTITY_CNF);
}

/****************************************************************************
 * Function Name  : lteRlcUeCNEForReconfigUEEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC UE CNE for Reconfigure
 *                  UE Entity.
 ****************************************************************************/
UInt32 lteRlcUeCNEForReconfigUEEntityLPHandler(void * msgBuf, SInt32 byteRead)
{
   return lteRlcUeContextNotExistLPHandler(msgBuf,byteRead,RLC_RECONFIG_UE_ENTITY_CNF);
}

/****************************************************************************
 * Function Name  : lteRlcUeCNEForReEstabUEEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC UE CNE for Re-establish
 *                  UE Entity.
 ****************************************************************************/
UInt32 lteRlcUeCNEForReEstabUEEntityLPHandler(void * msgBuf, SInt32 byteRead)
{
   return lteRlcUeContextNotExistLPHandler(msgBuf,byteRead,RLC_RE_ESTABLISH_UE_ENTITY_CNF);
}

/*SPR 6631 End*/


/*SandeepG FSM changes Start*/
/****************************************************************************
 * Function Name  : lteRlcDeleteUeContextLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC Delete UE Context.
 ****************************************************************************/
UInt32 lteRlcDeleteUeContextLPHandler(void * msgBuf, SInt32 byteRead)
{

    RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p =  RLC_NULL;
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)msgBuf;

    UInt16 resp = RLC_FAILURE;
    /* +COVERITY 10481 */
    UInt16 cnfMsgIdx = 0;
    /* -COVERITY 10481 */

    /* + Layer2 NON CA Changes */
    UInt16 transactionId = node_p->transactionId;
    RrcCellIndex cellId = node_p->cellId;
    /* - Layer2 NON CA Changes */
    UInt16 ueIndex = node_p->ueIndex;
    /** CA Changes Start **/
    UInt8 cnfMsgBuf[MAX_RES_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;

    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(
                                                            node_p->cellId);
    /** CA Changes End **/

#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex = 0;
    /* SPR 4915 Fix Start */
    UInt16 mappedUeIdx = 0;
    /* SPR 4915 Fix End */
#endif

    rlcRrcOamPendingReq_p = rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p;

    if (node_p->rlcState != RLC_ENTITY_PENDING_DELETION){
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_DELETE_ENTITY_ERR, LOGERROR,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex,byteRead, 0, 0, 0, 0, __func__, "");
    }
    freeMemPool(node_p);
    node_p = RLC_NULL;

    rlcRrcOamPendingReq_p->nodesPushedInQueueCount--;

    if(!(rlcRrcOamPendingReq_p->nodesPushedInQueueCount))
    {
        freeMemPool(rlcRrcOamPendingReq_p);
        rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p = RLC_NULL;
        /* SPR 4723 Fix Start */
#ifndef UE_SIM_TESTING
        rrcUeIndex = rlcUEContextList_g[ueIndex]->rrcUeIndex; /* Fix */
#endif
        /* SPR 4723 Fix End */
        resp = rlcDbDeleteUEContext(ueIndex);

        {
            cnfMsgIdx =  RRC_RLC_API_HEADER_LEN;
            resp_p = cnfMsgBuf;

#ifndef UE_SIM_TESTING
            LTE_SET_U16BIT((resp_p + cnfMsgIdx),rrcUeIndex);
            /* SPR 4915 fix start */
            mappedUeIdx = getUeIdxFromRrcUeIdx(rrcUeIndex);
            if ((mappedUeIdx != INVALID_UE_ID) && (0 == setUeIdxInvalid(mappedUeIdx, UE_IDX_MAPPING_FOR_RLC)))
            {
                freeUeIdx(mappedUeIdx);
            }
            /* SPR 4915 fix End */
#else
            LTE_SET_U16BIT((resp_p + cnfMsgIdx),ueIndex);
#endif
            cnfMsgIdx += 2;
            LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
            cnfMsgIdx += 2;

            /* preparing header for confirmation */
            /* + Layer2 NON CA Changes */
            {
            	prepareCnfBufferHeader(cnfMsgBuf, RLC_DELETE_UE_ENTITY_CNF,transactionId, cellId, cnfMsgIdx);
            }
            /* - Layer2 NON CA Changes */

            /* writting to socket */
            rlcSendMsgToRrc(cnfMsgBuf,
                    cnfMsgIdx);
            activeUEs_g--;
            /** CA Changes Start **/
            if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
            {
                rlcProcessOAMRRCState_g[internalCellIndex] = RLC_CTRL_IDLE_ST;
            }
            /** CA Changes End **/
        }
    }
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : lteRlcDeleteEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC Delete Entity.
 ****************************************************************************/
UInt32 lteRlcDeleteEntityLPHandler( void * msgBuf, SInt32 byteRead)
{
    RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p =  RLC_NULL;
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)msgBuf;

    /* Rel 5.3: Coverity 32385 Fix Start */
    UInt32 resp = RLC_FAILURE;
    /* Rel 5.3: Coverity 32385 Fix End */
    /* +COVERITY 10481 */
    UInt16 cnfMsgIdx = 0;
    /* -COVERITY 10481 */

    /* + Layer2 NON CA Changes */
    UInt16 transactionId = node_p->transactionId;
    RrcCellIndex cellId = node_p->cellId;
    /* - Layer2 NON CA Changes */
    /* Rel 5.3: Coverity 32385 Fix Start */
    RlcEntityType entityType = RLC_ENTITY_TYPE_UNKNOWN;
    UInt32 ueIndex = node_p->ueIndex;
    RlcLcId lcId = 0;
    /* Rel 5.3: Coverity 32385 Fix End */
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = 
        RLC_GET_INTERNAL_CELL_INDEX(node_p->cellId);
    /** CA Changes End **/
    /* SPR 19698 Changes Start */
    UInt8 idx = 0;
    /* SPR 19698 Changes End */

    UInt8 cnfMsgBuf[MAX_RES_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;

#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex = 0;
#endif

    rlcRrcOamPendingReq_p = rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p;
    /* SPR 19698 Changes Start */
    lcId = rlcRrcOamPendingReq_p->lcId[idx];
    /* SPR 19698 Changes End */
        /*--coverity--530--54181*/
	/*    entityType = (RlcEntityType)rlcRrcOamPendingReq_p->entityType; coverity warning : 110645 fixed*/
        /*--coverity--530--54181*/


    if (node_p->rlcState != RLC_ENTITY_PENDING_DELETION){
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_DELETE_ENTITY_ERR, LOGERROR,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex,byteRead, 0, 0, 0, 0, __func__, "");
    }
    freeMemPool(node_p);
    node_p = RLC_NULL;

    rlcRrcOamPendingReq_p->nodesPushedInQueueCount--;

    if(!(rlcRrcOamPendingReq_p->nodesPushedInQueueCount))
    {
        /****Coverity 110648 start +
          LINE DELETED
         ***Coverity 110648 end - ***/
        /* SPR 4723 Fix Start */
#ifndef UE_SIM_TESTING
        rrcUeIndex = rlcUEContextList_g[ueIndex]->rrcUeIndex; /* Fix */
#endif
        /* SPR 4723 Fix End */

        /* SPR 19698 Changes Start */
        for ( idx = 0; idx < rlcRrcOamPendingReq_p->lcCount; idx++)
        {
            lcId = rlcRrcOamPendingReq_p->lcId[idx];
            /*--coverity--530--54181*/
            entityType = (RlcEntityType)rlcRrcOamPendingReq_p->entityType[idx];
            /*--coverity--530--54181*/
            resp = rlcDbDeleteRLCEntityInfo(ueIndex,lcId,entityType);
        }
        /* SPR 19698 Changes End */

        cnfMsgIdx =  RRC_RLC_API_HEADER_LEN;
        resp_p = cnfMsgBuf;

#ifndef UE_SIM_TESTING
        /* SPR 4772 change end */
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),rrcUeIndex);
#else
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),ueIndex);
#endif
        cnfMsgIdx += 2;
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
        cnfMsgIdx += 2;

        /* + Layer2 NON CA Changes */
        /* preparing header for confirmation */
        prepareCnfBufferHeader(resp_p, RLC_RECONFIG_UE_ENTITY_CNF, 
                transactionId, cellId,cnfMsgIdx);
        /* - Layer2 NON CA Changes */

        /* writting to socket */
        rlcSendMsgToRrc(cnfMsgBuf,
                cnfMsgIdx);
        /** CA Changes Start **/
        if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
        {
            rlcProcessOAMRRCState_g[internalCellIndex] = RLC_CTRL_IDLE_ST;
        }
        /** CA Changes End **/
        /* SPR 19698 Changes Start */
        freeMemPool(rlcRrcOamPendingReq_p);
        rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p = RLC_NULL;
        /* SPR 19698 Changes End */
    }

    return RLC_SUCCESS;
}


/****************************************************************************
 * Function Name  : lteRlcReconfigureEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC Re-configure Entity.
 ****************************************************************************/
UInt32 lteRlcReconfigureEntityLPHandler( void * msgBuf, SInt32 byteRead)
{
    RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p =  RLC_NULL;
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)msgBuf;

    UInt8 resp = RLC_SUCCESS;

    UInt16 transactionId = node_p->transactionId;
    RrcCellIndex cellId = node_p->cellId;
    UInt16 ueIndex = node_p->ueIndex;
    RlcConfigReq *rlcConfigReq_p;

    UInt8 noOfFailures ;
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(
                                                            node_p->cellId);
    /* coverity Fix 60263 starts */
    if( MAX_INTERNAL_CELL_INDEX < internalCellIndex )
    {
        resp = RLC_FAILURE;
    }
    /* coverity Fix 60263 ends */

    /** CA Changes End **/

#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex = 0;
#endif

    rlcRrcOamPendingReq_p = rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p;
    rlcConfigReq_p = &rlcRrcOamPendingReq_p->rlcConfigReq;
    noOfFailures = rlcRrcOamPendingReq_p->noOfFailures;

    if (node_p->rlcState != RLC_ENTITY_PENDING_DELETION)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RECONFIG_ERR_9, LOGERROR,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcConfigReq_p->ueIndex,byteRead, 0, 0, 0, 0, __func__, "");
    }
    freeMemPool(node_p);
    node_p = RLC_NULL;

    rlcRrcOamPendingReq_p->nodesPushedInQueueCount--;

    if(!(rlcRrcOamPendingReq_p->nodesPushedInQueueCount))
    {
#ifndef UE_SIM_TESTING
        rrcUeIndex = rlcUEContextList_g[ueIndex]->rrcUeIndex; /* Fix */
#endif

#ifndef UE_SIM_TESTING
    /* + Layer2 NON CA Changes */

    /* if SPS is enabled by OAM in system, configure SPS to UE and LCs */
       /* coverity Fix 60263 starts */ 
    if((RLC_FAILURE != resp) && 
       /* coverity Fix 60263 ends */ 
            (LTE_TRUE == secondaryCellInfo_g[internalCellIndex].enableSps ))
    {
        if( RLC_SUCCESS != rlcConfigSpsInfo( rlcConfigReq_p->ueIndex, 
                    rlcConfigReq_p) )
        {
            if( 0 == noOfFailures )
            {
                resp = RLC_FAILURE; 
            }
        }
    }
    /* - Layer2 NON CA Changes */
#endif
        if ( noOfFailures > 0 )
        {
            resp = RLC_PARTIAL_SUCCESS;
        }
        /* SPR 4690 Start */
        /* SPR 5366 Changes Start */
        /* SPR 5366 Changes End */
#ifndef UE_SIM_TESTING
        sendCnfMsgToRRC(RLC_RECONFIG_UE_ENTITY_CNF, transactionId,
                rlcConfigReq_p, resp, cellId,rrcUeIndex);
#else
        sendCnfMsgToRRC(RLC_RECONFIG_UE_ENTITY_CNF, transactionId,
                rlcConfigReq_p, resp,cellId);
#endif
        /** CA Changes Start **/
        if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
        {
            rlcProcessOAMRRCState_g[internalCellIndex] = RLC_CTRL_IDLE_ST;
        }
        /** CA Changes Start **/
        freeMemPool(rlcRrcOamPendingReq_p);
        rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p = RLC_NULL;
    }
    return RLC_SUCCESS;
}


/****************************************************************************
 * Function Name  : lteRlcRestabEntityLPHandler
 * Inputs         : msgBuf - Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is used to handle LTE RLC Re-establish Entity.
 ****************************************************************************/
UInt32 lteRlcRestabEntityLPHandler( void * msgBuf, SInt32 byteRead)
{
    /* SPR 4994 Change End */
    UInt16 resp    = RLC_SUCCESS;
    UInt16 idx = 0;
    /* SPR 4737 Fix Start */
    UInt16 cnfMsgIdx      = 0;
    RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p =  RLC_NULL;
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)msgBuf;

    RlcReEstabReq * rlcReEstabReq_p;

    /* + Layer2 NON CA Changes */
    UInt16 transactionId = node_p->transactionId;
    RrcCellIndex cellId = node_p->cellId;
    /* - Layer2 NON CA Changes */

    UInt8 cnfMsgBuf[MAX_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;

    UInt16 ueIndex = node_p->ueIndex;

    UInt8 noOfFailures ;
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(
            node_p->cellId);
    /** CA Changes End **/

#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex = 0;
#endif

    rlcRrcOamPendingReq_p = rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p;
/* SPR 22337_22360 Fix Start */
    if((PNULL==rlcRrcOamPendingReq_p))
    {
        freeMemPool(node_p);
        node_p = RLC_NULL;
        //fprintf(stderr,"\nHO-Crash Issue Triggered  BUFF: %p \n",rlcRrcOamPendingReq_p);
        return RLC_SUCCESS;
    }
/* SPR 22337_22360 Fix End */
    rlcReEstabReq_p = &rlcRrcOamPendingReq_p->rlcReEstabReq;
    noOfFailures = rlcRrcOamPendingReq_p->noOfFailures;

    if (node_p->rlcState != RLC_SUCCESS)
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_1, LOGERROR,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                ueIndex,byteRead, 0, 0, 0, 0, __func__, "");
    }
    freeMemPool(node_p);
    node_p = RLC_NULL;

    rlcRrcOamPendingReq_p->nodesPushedInQueueCount--;

    if(!(rlcRrcOamPendingReq_p->nodesPushedInQueueCount))
    {

        if ( noOfFailures > 0 )
        {
            resp = RLC_PARTIAL_SUCCESS;
        }
        /** SPR 3015 Changes Start **/
        cnfMsgIdx = RRC_RLC_API_HEADER_LEN;
        resp_p = cnfMsgBuf;

#ifndef UE_SIM_TESTING
        /* SPR 21808 Fix Start */
        rrcUeIndex = rlcUEContextList_g[ueIndex]->rrcUeIndex;
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),rrcUeIndex);
#else
        LTE_SET_U16BIT(( resp_p + cnfMsgIdx),ueIndex);
        /* SPR 21808 Fix End */
#endif
        cnfMsgIdx += 2;
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
        cnfMsgIdx += 2;
        if ( RLC_PARTIAL_SUCCESS == resp )
        {
            for ( idx = 0; idx < rlcReEstabReq_p->lcCount; idx++)
            {
                if ( RLC_SUCCESS !=
                        rlcReEstabReq_p->lcIdReEstList[idx].result)
                {
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            RE_ESTABLISH_ENTITY_ERROR);
                    cnfMsgIdx += 2;
                    /* tag length of CREATE_ENTITY_ERROR tag is 8 */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            ENTITY_ERROR_TAG_LENGTH);
                    cnfMsgIdx += 2;
                    /* lcId of entity */
                    resp_p[cnfMsgIdx++] =
                        rlcReEstabReq_p->lcIdReEstList[idx].lcId & 0x00FF;
                    /* response of create request */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            rlcReEstabReq_p->lcIdReEstList[idx].result);
                    cnfMsgIdx += 2;
                }
            }
        }

        /* + Layer2 NON CA Changes */
        prepareCnfBufferHeader(resp_p,RLC_RE_ESTABLISH_UE_ENTITY_CNF, transactionId, cellId, cnfMsgIdx);
        /* - Layer2 NON CA Changes */

        /* writing to socket */
        rlcSendMsgToRrc(cnfMsgBuf,
                cnfMsgIdx);
        /** SPR 3015 Changes End **/
        /** CA Changes Start **/
        if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
        {
            rlcProcessOAMRRCState_g[internalCellIndex] = RLC_CTRL_IDLE_ST;
        }
        /** CA Changes End **/
        freeMemPool(rlcRrcOamPendingReq_p);
        rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p = RLC_NULL;
    }

    return RLC_SUCCESS;
}


/*******************************************************************************
 * Function Name  : lteRlcHandlePendingRrcOamMsg
 * Inputs         : None
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This API is used to handle LTE RLC Pending RRC OAM Messages.
 *******************************************************************************/
/* + SPR 17439 */
UInt32 lteRlcHandlePendingRrcOamMsg(void)
/* - SPR 17439 */    
{
    RlcPendingRrcOamMsgNode * rlcPendingRrcOamMsg_p;
    UInt8 *buf = PNULL;
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0;
#endif

    /* SPR 7587 Changes Start */
    rlcPendingRrcOamMsg_p = (RlcPendingRrcOamMsgNode *)popNode(&rlcPendingRrcOamMsgQ_g) ;
    if(rlcPendingRrcOamMsg_p)
    {
       buf = (UInt8 *)rlcPendingRrcOamMsg_p->msg_p;
       RrcCellIndex cellId = buf[API_HEADER_CELL_INDEX_OFFSET];
       InternalCellIndex internalCellIndex  = RLC_GET_INTERNAL_CELL_INDEX(cellId);
       if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
       {
#ifdef LOG_PRINT_ENABLED
           state = rlcProcessOAMRRCState_g[internalCellIndex];
#endif
           /*+COVERITY 5.2.2 - 32688*/
           if( 0x2FFF  ==  lteRlcLPrioThrApiHandlerFuncTable
                  [rlcProcessOAMRRCState_g[internalCellIndex]]
                  [RLC_RECV_MSG_FROM_RRCOAM_EV](rlcPendingRrcOamMsg_p->msg_p,
                     rlcPendingRrcOamMsg_p->msgLenth))
           /*-COVERITY 5.2.2 - 32688*/
           {
               cleanupLayer2();
           }
           LOG_RLC_MSG(LTE_RLC_STATE_MACHINE_INFO, LOGINFO, RLC_RRC,
             globalTTITickCount_g, __LINE__,
             internalCellIndex, state, RLC_RECV_MSG_FROM_RRCOAM_EV,
             rlcProcessOAMRRCState_g[internalCellIndex], 0, 0, __func__, "ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM");
       }
        freeMemPool(rlcPendingRrcOamMsg_p);
    }
    rlcPendingRrcOamMsg_p = PNULL;
    /* SPR 7587 Changes End */
    return RLC_SUCCESS;
}

/*******************************************************************************
 * Function Name  : lteRlcRecvMsgWithIccQ
 * Inputs         : None
 * Outputs        : None
 * Returns        : node_p 
 * Description    : This API is used to handle LTE RLC Receive Message with ICCQ.
 *******************************************************************************/
/* + SPR 17439 */
void * lteRlcRecvMsgWithIccQ(void)
/* - SPR 17439 */    
{
    RlcRrcOamHiPrioQNode * node_p = RLC_NULL;
    recvMsgWithIccQ(rlcRrcCnfOutputQId_g, (const char *)&node_p);
    return node_p;
}



/****************************************************************************
 * Function Name  : ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM
 * Inputs         : recvBuffer_p - Pointer to Received Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This function process the OAM and rrc messages calls 
 *                  the RLC state machine 
 ****************************************************************************/
UInt32 ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM
            (void* recvBuffer_p, SInt32 byteRead)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(byteRead)
    /* +- SPR 17777 */
    RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)recvBuffer_p;
    UInt8 rlcEvent = RLC_CTRL_NULL_EV;
    rlcEvent = node_p->processRrcOamMsgEvent;
#ifdef LOG_PRINT_ENABLED
    UInt32 state = 0; 
#endif
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(
                                                            node_p->cellId);
    if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
    {
#ifdef LOG_PRINT_ENABLED 
        state = rlcProcessOAMRRCState_g[internalCellIndex];
#endif
        lteRlcLPrioThrApiHandlerFuncTable
            [rlcProcessOAMRRCState_g[internalCellIndex]][rlcEvent](node_p,0);
              LOG_RLC_MSG(LTE_RLC_STATE_MACHINE_INFO, LOGINFO, RLC_RRC,
             globalTTITickCount_g, __LINE__,
             internalCellIndex, state, rlcEvent,
             rlcProcessOAMRRCState_g[internalCellIndex], 0, 0, __func__,"ElRlcProcessHPrioThrResForOAMAndRRCMsgFSM");

        /* SPR 7587 Changes Start */
        if (rlcProcessOAMRRCState_g[internalCellIndex] == RLC_CTRL_IDLE_ST)
        {
            return lteRlcHandlePendingRrcOamMsg();
        }
        else
        {
            return RLC_SUCCESS;
        }
    }
    return RLC_FAILURE;
    /** CA Changes End **/
    /* SPR 7587 Changes End */
}
#endif

/****************************************************************************
 * Function Name  : ElRlcProcessOAMAndRRCMsgFSM
 * Inputs         : recvBuffer_p - Pointer to Received Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This API is used to Process OAM and RRC Message 
 ****************************************************************************/
UInt32 ElRlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead)
{
#ifdef LOG_PRINT_ENABLED
   UInt32 state  = 0; 
#endif

   /** CA Changes Start **/
   RlcRrcOamHiPrioQNode * node_p = (RlcRrcOamHiPrioQNode*)recvBuffer_p;
   InternalCellIndex internalCellIndex = layer2CommonGetInternalCellIndex(
                                                               node_p->cellId);
   if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
   {
#ifdef LOG_PRINT_ENABLED
       state  = rlcProcessOAMRRCState_g[internalCellIndex];
#endif

       LOG_RLC_MSG(LTE_RLC_STATE_MACHINE_INFO, LOGINFO, RLC_RRC,
             globalTTITickCount_g, __LINE__,
             internalCellIndex, state, RLC_RECV_MSG_FROM_RRCOAM_EV,
             rlcProcessOAMRRCState_g[internalCellIndex], 0, 0, __func__,"ElRlcProcessOAMAndRRCMsgFSM");
  
       return lteRlcLPrioThrApiHandlerFuncTable
           [rlcProcessOAMRRCState_g[internalCellIndex]]
              [RLC_RECV_MSG_FROM_RRCOAM_EV](recvBuffer_p,byteRead);
   }
   return RLC_FAILURE;
   /** CA Changes End **/
}
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rlcProcessOAMAndRRCMsgFSM
 * Inputs         : recvBuffer_p - Pointer to Received Message buffer,
 *                  byteRead - Bytes read
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE 
 * Description    : This API is used to Process OAM and RRC Message 
 ****************************************************************************/
UInt32 rlcProcessOAMAndRRCMsgFSM(UInt8* recvBuffer_p, SInt32 byteRead)
{
    RrcCellIndex cellId = recvBuffer_p[API_HEADER_CELL_INDEX_OFFSET];
    UInt16 apiId = LTE_GET_U16BIT(recvBuffer_p + API_HEADER_API_ID_OFFSET);
    InternalCellIndex internalCellIndex = INVALID_CELL_INDEX ;
#ifdef LOG_PRINT_ENABLED
    UInt32 state  = 0;
#endif

    if( RLC_INIT_LAYER_REQ == apiId )
    {
       internalCellIndex =  RLC_UPDATE_AND_GET_INTERNAL_CELL_INDEX(cellId);
    }
    else
    {
        internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX( cellId );
    }

    if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
    {
#ifdef LOG_PRINT_ENABLED
        state  = rlcProcessOAMRRCState_g[internalCellIndex];
#endif
        LOG_RLC_MSG(LTE_RLC_STATE_MACHINE_INFO, LOGINFO, RLC_RRC,
             globalTTITickCount_g, __LINE__,
             internalCellIndex, state, RLC_RECV_MSG_FROM_RRCOAM_EV,
             rlcProcessOAMRRCState_g[internalCellIndex], 0, 0, __func__,"RlcProcessOAMAndRRCMsgFSM");
        return lteRlcLPrioThrApiHandlerFuncTable
            [rlcProcessOAMRRCState_g[internalCellIndex]]
            [RLC_RECV_MSG_FROM_RRCOAM_EV](recvBuffer_p,byteRead);
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_PARSE_RRC_CELL_INDEX_ERR, LOGWARNING, RLC_RRC,
                globalTTITickCount_g, __LINE__,
                0, cellId, internalCellIndex,
                0, 0, 0, __func__, "");
    }

    return RLC_FAILURE;
    /** CA Changes End **/
}

#endif
/****************************************************************************
 * Function Name  : initRlcProcessRrcOamMsgFSM
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This API is used to initialize RLC and process RRC OAM
 *                  Message state machine 
 ****************************************************************************/
/* + SPR 17439 */
void initRlcProcessRrcOamMsgFSM(void)
/* - SPR 17439 */    
{
    UInt32 rlcState,rlcEvent;
    for(rlcState= 0;rlcState < RLC_CTRL_MAX_ST;rlcState++)
    {
        for(rlcEvent = 0; rlcEvent < RLC_CTRL_MAX_EV; rlcEvent++)
        {
            lteRlcLPrioThrApiHandlerFuncTable[rlcState][rlcEvent] = lteRlcInvalidEvent;
        }
    }

    /*SPR 6931 Fix start*/
    sQueueInit(&rlcPendingRrcOamMsgQ_g);
    /*SPR 6931 Fix end*/

    lteRlcLPrioThrApiHandlerFuncTable[RLC_CTRL_IDLE_ST][RLC_RECV_MSG_FROM_RRCOAM_EV] =  rlcHandleSocketMsgs ;

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST][RLC_DELETE_UE_CONTEXT_EV] =  lteRlcDeleteUeContextLPHandler ;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST][RLC_DELETE_TX_ENTITY_EV] =  lteRlcDeleteEntityLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST][RLC_DELETE_RX_ENTITY_EV] = lteRlcDeleteEntityLPHandler;
    /*SPR 6631 Start*/
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST][RLC_DELETE_TX_RX_ENTITY_EV] = lteRlcDeleteEntityLPHandler;
    /*SPR 6631 End*/
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST][RLC_RECONFIG_TX_ENTITY_EV] = lteRlcReconfigureEntityLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST][RLC_RECONFIG_RX_ENTITY_EV] = lteRlcReconfigureEntityLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST][RLC_REESTAB_ENTITY_EV] = lteRlcRestabEntityLPHandler;

    /*SPR 6631 Start*/

    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST][RLC_UE_CONTEXT_NOT_EXIST_EV] = lteRlcUeCNEForDeleteUEContextLPHandler ;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST][RLC_UE_CONTEXT_NOT_EXIST_EV] =  lteRlcUeCNEForDeleteUEEntityLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST][RLC_UE_CONTEXT_NOT_EXIST_EV] =  lteRlcUeCNEForReconfigUEEntityLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST][RLC_UE_CONTEXT_NOT_EXIST_EV] = lteRlcUeCNEForReEstabUEEntityLPHandler;
    /*SPR 6631 End*/

    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST][RLC_RECV_MSG_FROM_RRCOAM_EV] =  lteRlcEnqueueCtrlReqLPHandler ;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST][RLC_RECV_MSG_FROM_RRCOAM_EV] = lteRlcEnqueueCtrlReqLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST][RLC_RECV_MSG_FROM_RRCOAM_EV] = lteRlcEnqueueCtrlReqLPHandler;
    lteRlcLPrioThrApiHandlerFuncTable[WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST][RLC_RECV_MSG_FROM_RRCOAM_EV] = lteRlcEnqueueCtrlReqLPHandler;

#endif

}


/****************************************************************************
 * Function Name  : handleRlcDeleteEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC
 *                  msgLen - indicate the length of msg unread,
 * Outputs        : None
 * Returns        : ErrorCode 
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/

inline static int handleRlcDeleteEntity (  
                                          UInt8 *msg_p,
                                          UInt16 msgLen
                                        )
{
    UInt16 current = RRC_RLC_API_HEADER_LEN;  /* Buffer Header is of 10 bytes */
    UInt16 ueIndex = 0;
    RlcConfigReq rlcConfigReq = {0};
    UInt16 resp    = RLC_SUCCESS;
    UInt16 transactionId = 0;
    /* + Layer2 NON CA Changes */
    RrcCellIndex cellId = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */
    transactionId = LTE_GET_U16BIT(msg_p);
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellId = *(msg_p+API_HEADER_CELL_INDEX_OFFSET);
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    /** CA Changes End **/
#endif
    /* - Layer2 NON CA Changes */
    /* + SPR 20472 */ 
    resp =  rlcParseUeIndex( msg_p, msgLen, &(rlcConfigReq.ueIndex), &current, &ueIndex );
    /* - SPR 20472 */ 

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    UInt8 nodesPushedInQueueCount = 0;   
#endif
    if ( RLC_SUCCESS == resp)
    {
		/* + PERF_CA GBL */
#ifndef UE_SIM_TESTING
        /* + coverity 62623 */
        if(MAX_NUM_CELL > internalCellIndex)
        {
            lteRlcStatInfo_g[internalCellIndex].lteUEStat.numOfActiveUE--;
	    /* SPR 21808 Fix Start */
	    lteRlcStatInfo_g[internalCellIndex].lteUEStat.rlcUeStats[rlcConfigReq.ueIndex].crnti = 0xFFF;
	    /* SPR 21808 Fix End */
	}
		/* - coverity 62623 */
#endif
		/* - PERF_CA GBL */
        gRlcStats.pRlcStats->lteSysStat.numActiveUE --;
	/* SPR 21808 Fix Start */
        memSet(&(gRlcStats.pRlcStats->lteRlcUeStat[rlcConfigReq.ueIndex]),
                0,sizeof(LteRlcUEInfo));
	/* SPR 21808 Fix End */
        /*Delete the UE Context*/
/*SPR 21653 changes start*/ 
        resp = rlcDeleteUEContext ( rlcConfigReq.ueIndex, transactionId,cellId
/*SPR 21653 changes end*/ 
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
        , &nodesPushedInQueueCount
#endif
        );
    }
    
    if ( RLC_SUCCESS != resp)
    {
    /* +- SPR 20472 Code removed */
#ifndef UE_SIM_TESTING
       /* SPR 4864 fix start */
       /* SPR 4915 fix start */
       /* + SPR 20472 */ 
/*SPR 21653 changes start*/ 
       if ((rlcConfigReq.ueIndex != INVALID_UE_ID) && (0 == setUeIdxInvalid(rlcConfigReq.ueIndex, UE_IDX_MAPPING_FOR_RLC)))
       {
            freeUeIdx(rlcConfigReq.ueIndex);
       }
       /* SPR 4915 fix End */
       /* SPR 4864 fix end */
        /* SPR 4864 Change Start */
	{
        sendCnfMsgToRRC(RLC_DELETE_UE_ENTITY_CNF, transactionId,
                &rlcConfigReq, RLC_FAILURE, cellId, ueIndex);
/*SPR 21653 changes end*/ 
    }
    /* - SPR 20472 */
        /* SPR 4864 Change End */
#else
        /* SPR 4864 Change Start */
        sendCnfMsgToRRC(RLC_DELETE_UE_ENTITY_CNF, transactionId,
                &rlcConfigReq, RLC_FAILURE,cellId);
        /* SPR 4864 Change End */
#endif
/* SPR 4690 End */
    }
/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    /* Wait for Confirmation from DL/UL RLC threads & delete UE context*/
    else
    {
        RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p = 
            (RlcRrcOamPendingReqLP *)getMemFromPool(sizeof(RlcRrcOamPendingReqLP), PNULL);
        /*++klockwork warning */
        if(rlcRrcOamPendingReq_p)
        /*--klockwork warning */
        {
            rlcRrcOamPendingReq_p->nodesPushedInQueueCount = nodesPushedInQueueCount;
            /* SPR 21808 Fix Start */
            rlcUEContextList_g[rlcConfigReq.ueIndex]->rlcRrcOamPendingReq_p =  rlcRrcOamPendingReq_p;
            /* SPR 21808 Fix End */
            /** CA Changes Start **/
            if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
            {
                rlcProcessOAMRRCState_g[internalCellIndex] = WAITING_FOR_HP_RLC_DELETE_UE_CONTEXT_RES_ST;
            }
            /** CA Changes End **/
        }
        else
        {
            LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL, L2_SYS_FAIL, globalTTITickCount_g, __LINE__,
                    0, 0,0,0, 0,0, __func__, "");
        }
		
#ifdef LINUX_PC_TEST//jsm
        if(circQueueCount(&rlcTxPendingDelQ_g[0]))
        {
            rlcProcessDelMsg(&rlcTxPendingDelQ_g[0]);
        }

	    if(circQueueCount(&rlcRxPendingDelQ_g))
	    {
	        rlcProcessDelMsg(&rlcRxPendingDelQ_g);
	    }
#endif
    }
#endif
/*SPR 21653 changes start*/ 
    LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_DELETE_ENTITY, LOGINFO,
            RLC_RRC, globalTTITickCount_g, __LINE__,
            rlcConfigReq.ueIndex, resp, msgLen, 0, 0, 0, __func__, "");
/*SPR 21653 changes end*/ 

    return resp;
}

/****************************************************************************
 * Function Name  : handleRlcReconfigMsg 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : ErrorCode 
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static int handleRlcReconfigMsg (
                                         UInt8 *msg_p,
                                         UInt16 msgLen
                                       )
/* Coverity Fix 55379 Start */
{
    UInt16 current = RRC_RLC_API_HEADER_LEN;  /* Buffer Header is of 10 bytes */
    RlcConfigReq rlcConfigReq = {0};
    UInt16 resp           = RLC_ENTITY_IN_PENDING_DEL_Q;
    UInt16 transactionId  = 0;
    UInt8  idx            = 0;
    UInt16 noOfFailures   = 0;
    UInt8  opCode         = 0;
#if !defined(UE_SIM_TESTING)
    UInt8 entityReconfig = RLC_FALSE;
#endif    
    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    UInt8 nodesPushedInQueueCount = 0;   
    /* SPR 5366 Changes Start */
    UInt8 entityDeleted = RLC_FALSE;
    /* SPR 5366 Changes End */
#endif
    RrcCellIndex cellId = INVALID_CELL_INDEX;

    /* SPR 4690 Start */
#ifndef UE_SIM_TESTING
    UInt16 rrcUeIndex = 0;
#endif

    /* SPR 4690 End */
    transactionId = LTE_GET_U16BIT(msg_p);
#ifndef UE_SIM_TESTING
    cellId = *(msg_p+API_HEADER_CELL_INDEX_OFFSET);
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
#else
    InternalCellIndex internalCellIndex = 0;
#endif
    /* SPR 19698 Changes Start */
#ifndef UE_SIM_TESTING
    RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p = PNULL;
#endif
    /* SPR 19698 Changes End */

    resp = rlcParseReconfigMsg( msg_p,
            msgLen,
            &rlcConfigReq,
            &current,
            /* SPR 12917 fix start */
            internalCellIndex
            /* SPR 12917 fix end */
            );
    if ( RLC_SUCCESS == resp )
    {
        /* SPR 4690 Start */
#ifndef UE_SIM_TESTING
        rrcUeIndex = rlcUEContextList_g[rlcConfigReq.ueIndex]->rrcUeIndex;
#endif
        /* SPR 4690 End */
        for ( idx = 0; idx < rlcConfigReq.lcCount; idx++)
        {
            opCode = rlcConfigReq.rlcEntityCfgList[idx].opCode;

            switch ( opCode )
            {
                case RLC_ADD_ENTITY:

                resp = rlcCreateUEEntity(rlcConfigReq.ueIndex,
                        rlcConfigReq.ueId,
                        &(rlcConfigReq.rlcEntityCfgList[idx]));
                if ( RLC_SUCCESS != resp )
                    noOfFailures++;

                break;

                case RLC_DELETE_ENTITY:

                resp = rlcDeleteUEEntity(rlcConfigReq.ueIndex,
                        rlcConfigReq.rlcEntityCfgList[idx].lcId,
                        transactionId, cellId, rlcConfigReq.
                        rlcEntityCfgList[idx].entityType,
                        rlcConfigReq.rlcEntityCfgList[idx].
                        entityMode
                        /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                        , &nodesPushedInQueueCount
#endif                            
                        );
                /* SPR 2712 Fix Start */
                if ( RLC_SUCCESS != resp)
                    noOfFailures++;
                /* SPR 2712 Fix End */
                /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                /* Wait for Confirmation from DL/UL RLC threads & delete UE Entity */    
                else
                {
                    /* SPR 19698 Changes Start */
                    if (rlcRrcOamPendingReq_p == PNULL)
                    {
                        rlcRrcOamPendingReq_p = (RlcRrcOamPendingReqLP *)getMemFromPool(sizeof(RlcRrcOamPendingReqLP), PNULL);
                    }
                    /* SPR 19698 Changes End */
                    /* SPR 22515 SPR fix start */
                    if(rlcRrcOamPendingReq_p)
                        /*--klockwork warning */
                    {
                        rlcRrcOamPendingReq_p->nodesPushedInQueueCount = nodesPushedInQueueCount;
                        /* SPR 19698 Changes Start */
                        rlcRrcOamPendingReq_p->lcId[idx] = rlcConfigReq.rlcEntityCfgList[idx].lcId;
                        /* SPR 13668 fix start */
                        /* Adding enityType so that
                         * lteRlcDeleteEntityLPHandler() will be able to delete
                         * entity */
                        rlcRrcOamPendingReq_p->entityType[idx] =
                            rlcConfigReq.rlcEntityCfgList[idx].entityType;
                        /* SPR 13668 fix end */
                        rlcRrcOamPendingReq_p->lcCount = rlcConfigReq.lcCount;
                        /* SPR 19698 Changes End */

                        /* SPR 13668 fix start */
                        /* Adding enityType so that lteRlcDeleteEntityLPHandler() will be able to delete entity */
    			/* SPR 19698 Changes Start */
                        rlcRrcOamPendingReq_p->entityType[idx] = rlcConfigReq.rlcEntityCfgList[idx].entityType; 
    			/* SPR 19698 Changes End */
                        /* SPR 13668 fix end */
                        rlcUEContextList_g[rlcConfigReq.ueIndex]->rlcRrcOamPendingReq_p =  rlcRrcOamPendingReq_p;
                        entityDeleted = RLC_TRUE;

                        /** CA Changes Start **/
                        if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
                        {
                            rlcProcessOAMRRCState_g[internalCellIndex] = WAITING_FOR_HP_RLC_DELETE_ENTITY_RES_ST;
                        }
                        /** CA Changes End **/
                    }
                    else
                    {
                        LOG_RLC_FATAL(RLC_PDCP, "Unable to allocate memory, "
                                "size[%u]",sizeof(RlcRrcOamPendingReqLP));
                    }
                    /* SPR 22515 SPR fix end */
                }
#endif                        
                break;

                case RLC_RECONFIG_ENTITY:

                /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING

                /* Send separate request to DL/UL RLC Threads */
                resp = rlcReconfigTxRxEntity(rlcConfigReq.ueIndex,
                        transactionId, cellId,
                        &(rlcConfigReq.rlcEntityCfgList[idx]), 
                        &nodesPushedInQueueCount);                    
                if(resp == RLC_SUCCESS)
                {
                    entityReconfig = RLC_TRUE;

                }
#else
                resp = rlcReconfigUEEntity(rlcConfigReq.ueIndex,
                        &(rlcConfigReq.rlcEntityCfgList[idx]) );
#endif
                if ( RLC_SUCCESS != resp )
                    noOfFailures++;

                break;

                default:
                //print log
                break;
            }
            rlcConfigReq.rlcEntityCfgList[idx].result = resp;
        }

        /* + Layer2 NON CA changes */
#ifndef UE_SIM_TESTING
        /* if SPS is enabled by OAM in system, configure SPS to UE and LCs */
        /*Coverity 65726 Start*/
        if((internalCellIndex <= MAX_INTERNAL_CELL_INDEX) && ((RLC_TRUE != entityReconfig) 
                    && (LTE_TRUE == secondaryCellInfo_g[internalCellIndex].enableSps)))
            /*Coverity 65726 End*/
        {
            if( RLC_SUCCESS != rlcConfigSpsInfo( rlcConfigReq.ueIndex, 
                        &rlcConfigReq ) )
            {
                if( 0 == noOfFailures )
                {
                    resp = RLC_FAILURE; 
                }
            }   
        }
        /* - Layer2 NON CA changes */
#endif
    }
    else
    {
        /* SPR 4690 Start */
#ifndef UE_SIM_TESTING
        /* UeID passed was possibly invalid, get the rrcUeIndex
         * to fill in the error response */
        rrcUeIndex = LTE_GET_U16BIT(msg_p + RRC_RLC_API_HEADER_LEN);
#endif
        /* SPR 4690 End */
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_RECONFIG_ERR, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcConfigReq.ueIndex, msgLen, resp ,0, 0, 0, __func__, "");

    }
    if ( noOfFailures > 0 )
    {
        resp = RLC_PARTIAL_SUCCESS;
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_RECONFIG_ERR_1, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcConfigReq.ueIndex, msgLen, noOfFailures, resp, 0, 0, __func__, "");
    }
    /* SPR 4690 Start */
    /* SPR 5366 Changes Start */
    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    if((entityDeleted == RLC_FALSE) && (RLC_FALSE == entityReconfig))
    {
#endif
        /* SPR 5366 Changes End */
#ifndef UE_SIM_TESTING
        sendCnfMsgToRRC(RLC_RECONFIG_UE_ENTITY_CNF, transactionId,
                &rlcConfigReq, resp, cellId, rrcUeIndex);
#else
    /* CID 55379 fix - indentation corrected */
    sendCnfMsgToRRC(RLC_RECONFIG_UE_ENTITY_CNF, transactionId,
                &rlcConfigReq, resp, cellId);
#endif
        /* SPR 5366 Changes Start */
        /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    }
    if(RLC_TRUE == entityReconfig)
    {
        RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p = 
            (RlcRrcOamPendingReqLP *)getMemFromPool(sizeof(RlcRrcOamPendingReqLP), PNULL);
        /* 67299 klockwork fix */
        if(rlcRrcOamPendingReq_p)
        {
        rlcRrcOamPendingReq_p->rlcConfigReq = rlcConfigReq;
        rlcRrcOamPendingReq_p->noOfFailures = noOfFailures;
        rlcRrcOamPendingReq_p->nodesPushedInQueueCount = nodesPushedInQueueCount;

        rlcUEContextList_g[rlcConfigReq.ueIndex]->rlcRrcOamPendingReq_p =  rlcRrcOamPendingReq_p;
        /** CA Changes Start **/
        if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
        {
            rlcProcessOAMRRCState_g[internalCellIndex] = WAITING_FOR_HP_RLC_RECONFIG_ENTITY_RES_ST ;
        }
        /** CA Changes End **/
    }
        else
        {
            LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL, L2_SYS_FAIL, globalTTITickCount_g, __LINE__,
                    0, 0,0,0, 0,0, __func__, "");
        }
        /* 67299 klockwork fix */
    }
#endif
    /* SPR 5366 Changes End */
    /* SPR 4690 End */
    /** 2712 Changes End **/ 
    return resp;
}
/* Coverity Fix 55379 End */

/****************************************************************************
 * Function Name  : handleRlcReEstablishMsg 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : ErrorCode 
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static int handleRlcReEstablishMsg ( 
                                            UInt8 *msg_p,
                                            UInt16 msgLen
                                          )
{
    UInt16 current = RRC_RLC_API_HEADER_LEN;  /* Buffer Header is of 10 bytes */
    /* SPR 4994 Change Start */
    RlcReEstabReq     rlcReEstabReq = {0};
    /* SPR 4994 Change End */
    UInt16 resp    = RLC_SUCCESS;
    UInt8 restabEntity = RLC_FALSE;
    /* SPR 4737 Fix Start */
    UInt16 cnfMsgIdx      = 0;
    /* SPR 4737 Fix End */
    UInt16 transactionId = 0;
    UInt8 idx            = 0;
    UInt16 noOfFailures  = 0;

    /* + Layer2 NON CA Changes */
    RrcCellIndex cellId = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */
    /* + SPR 20472 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* - SPR 20472 */

    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    UInt8 nodesPushedInQueueCount = 0;   
#endif
    UInt8 cnfMsgBuf[MAX_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;

    transactionId = LTE_GET_U16BIT(msg_p);
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellId = *(msg_p+API_HEADER_CELL_INDEX_OFFSET);
    /** CA Changes Start **/
    InternalCellIndex internalCellIndex = RLC_GET_INTERNAL_CELL_INDEX(cellId);
    /** CA Changes End **/
#endif
    /* - Layer2 NON CA Changes */

    /* Parse the Re Establish Message coming from RRC.*/      
    /* SPR 4994 Change Start */
    /* + SPR 20472 */
    resp = rlcParseReEstabMsg ( msg_p, msgLen, &rlcReEstabReq, &current, &ueIndex);
    /* - SPR 20472 */
    /* SPR 4994 Change End */

    if ( RLC_SUCCESS == resp )
    {
        for ( idx = 0; idx < rlcReEstabReq.lcCount; idx++)
        {   
            /* Create the RLC Entity */
	    /* + Layer2 NON CA Changes */
            /* + SPR 20472 */
            resp = rlcReEstabUeEntity(ueIndex,
                    /* - SPR 20472 */
                    rlcReEstabReq.lcIdReEstList[idx].lcId,
                    rlcReEstabReq.lcIdReEstList[idx].state,
                    transactionId, cellId
	    /* - Layer2 NON CA Changes */
                    /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
                    , &nodesPushedInQueueCount
#endif
                    );
            rlcReEstabReq.lcIdReEstList[idx].result = resp;
            if ( RLC_SUCCESS != resp )
            {
                noOfFailures++;
            }
            /*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
            else
            {
                restabEntity = RLC_TRUE;
            }
#endif
        }
        if ( noOfFailures > 0 )
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_RE_ESTABLISH_ERR, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    rlcReEstabReq.ueIndex, resp, msgLen, noOfFailures, 0, 0, __func__, "Parsing Error");
            resp = RLC_PARTIAL_SUCCESS;
        }
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_HANDLE_RLC_RE_ESTABLISH_ERR_1, LOGWARNING,
                RLC_RRC, globalTTITickCount_g, __LINE__,
                rlcReEstabReq.ueIndex, resp, msgLen, 0, 0, 0, __func__, "Parsing Error");
        /* SPR 4994 Change Start */
        if (resp != RLC_PARTIAL_SUCCESS)
            resp = RLC_FAILURE;
        /* SPR 4994 Change End */
    }
    if(RLC_TRUE != restabEntity)
    {
        /** SPR 3015 Changes Start **/

        cnfMsgIdx = RRC_RLC_API_HEADER_LEN;
        resp_p = cnfMsgBuf;
        /* + SPR 20472 */
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                rlcReEstabReq.ueIndex);
        /* - SPR 20472 */

        cnfMsgIdx += 2;
        LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
        cnfMsgIdx += 2;
        if ( RLC_PARTIAL_SUCCESS == resp )
        {
            for ( idx = 0; idx < rlcReEstabReq.lcCount; idx++)
            {
                if ( RLC_SUCCESS !=
                        rlcReEstabReq.lcIdReEstList[idx].result)
                {
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            RE_ESTABLISH_ENTITY_ERROR);
                    cnfMsgIdx += 2;
                    /* tag length of CREATE_ENTITY_ERROR tag is 8 */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            ENTITY_ERROR_TAG_LENGTH);
                    cnfMsgIdx += 2;
                    /* lcId of entity */
                    resp_p[cnfMsgIdx++] =
                        rlcReEstabReq.lcIdReEstList[idx].lcId & 0x00FF;
                    /* response of create request */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            rlcReEstabReq.lcIdReEstList[idx].result);
                    cnfMsgIdx += 2;
                }
            }
        }

        /* + Layer2 NON CA Changes */
        prepareCnfBufferHeader(resp_p,RLC_RE_ESTABLISH_UE_ENTITY_CNF, transactionId, cellId, cnfMsgIdx);
        /* - Layer2 NON CA Changes */
        /* writing to socket */
        rlcSendMsgToRrc(cnfMsgBuf,
                cnfMsgIdx);
        /** SPR 3015 Changes End **/
    }

/*SPR 16855 +-*/
#ifndef UE_SIM_TESTING
    else 
    {
        RlcRrcOamPendingReqLP *rlcRrcOamPendingReq_p = 
            (RlcRrcOamPendingReqLP *)getMemFromPool(sizeof(RlcRrcOamPendingReqLP), PNULL);
        /*++klockwork warning */
        if(rlcRrcOamPendingReq_p)
        {
            /*--klockwork warning */
            rlcRrcOamPendingReq_p->rlcReEstabReq = rlcReEstabReq;
            rlcRrcOamPendingReq_p->noOfFailures = noOfFailures;
            rlcRrcOamPendingReq_p->nodesPushedInQueueCount = nodesPushedInQueueCount;

            /* + SPR 20472 */
            rlcUEContextList_g[ueIndex]->rlcRrcOamPendingReq_p =  rlcRrcOamPendingReq_p;
            /* - SPR 20472 */
            /* CA Changes Start */
            if(internalCellIndex <= MAX_INTERNAL_CELL_INDEX)
            {
                rlcProcessOAMRRCState_g[internalCellIndex] = WAITING_FOR_HP_RLC_REESTAB_ENTITY_RES_ST;
            }
            /* CA Changes End */
        }
        else
        {
            LOG_RLC_MSG(LTE_RLC_MEM_ALLOC_FAILURE, LOGFATAL, L2_SYS_FAIL, globalTTITickCount_g, __LINE__,
                    0, 0,0,0, 0,0, __func__, "");
        }
    }
#endif
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : rlcParseChangeCrntiMsg
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread,
 *                  rlcConfigReq_p - Pointer to RLC Config structure,
 *                  current_p - Pointer to current Idx
 * Outputs        : None
 * Returns        : RLC_SUCCESS on success and error code on failure
 * Description    : This API is to parse the reEstablish msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static UInt32 rlcParseChangeCrntiMsg ( UInt8 *msg_p, 
                                              UInt16 msgLen,
                                              RlcChangeCrntiReq *rlcChangeCrntiReq_p, 
                                              UInt16 *current_p
                                              /* + SPR 20472 */
                                              ,UInt16 *ueIndex
                                              /* - SPR 20472 */
                                            )
{
    UInt16 current = *current_p;
    UInt16 resp = RLC_SUCCESS;
    if(msgLen < (CHANGE_CRNTI_MANDATORY_LENGTH ))
    {
        return RLC_SYNTAX_ERROR;
    }
    /* + SPR 20472 */
    rlcChangeCrntiReq_p->ueIndex = LTE_GET_U16BIT(msg_p + current);
#ifndef UE_SIM_TESTING
/* SPR 4558 Fix Start */
    *ueIndex = getUeIdxFromRrcUeIdx(rlcChangeCrntiReq_p->ueIndex);
/* SPR 4558 Fix End */
#else
    *ueIndex = rlcChangeCrntiReq_p->ueIndex;
#endif
    /* - SPR 20472 */ 
    current += 2;
    /* + SPR 20472 */
    resp = IS_UE_ID_INVALID(*ueIndex);
    /* - SPR 20472 */
    if(resp != RLC_SUCCESS)
    {
        return RLC_INVALID_UE_ID;
    }
    /* + SPR 20472 */
    resp = IS_UE_ID_NOT_EXISTS(*ueIndex);
    /* - SPR 20472 */
    if(RLC_SUCCESS == resp)
    {
        /* + SPR 20472 */
        return RLC_UE_NOT_EXISTS;
        /* - SPR 20472 */
    }
    rlcChangeCrntiReq_p->oldCrnti = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    
    rlcChangeCrntiReq_p->newCrnti = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    *current_p = current;
    return RLC_SUCCESS; 
}

/* SPR 4994 Changes Start */
/****************************************************************************
 * Function Name  : handleRlcReEstablishCompInd
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : ErrorCode
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static UInt32 handleRlcReEstablishCompInd ( UInt8   *msg_p,
                                                   UInt16  msgLen )
{
    UInt16 current = RRC_RLC_API_HEADER_LEN;
    UInt16 resp = RLC_SUCCESS;
    UInt16 cnfMsgIdx = 0;
    RlcReEstabReq rlcReEstabCompInd = {0};
    UEContext* ueContext_p = RLC_NULL;
    RLCEntityInfo *rlcEntityInfo_p = RLC_NULL;
    RlcLcId    lcId = 0;
    UInt8 idx = 0;
    UInt16 transactionId = 0;
    /* + SPR 20472 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* - SPR 20472 */
    transactionId = LTE_GET_U16BIT(msg_p);
    /* + Layer2 NON CA Changes */
        RrcCellIndex cellId = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */
    UInt8 cnfMsgBuf[MAX_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;

    /* SPR_10881_Fix Start */
#ifndef UE_SIM_TESTING
    cellId = *(msg_p+API_HEADER_CELL_INDEX_OFFSET);
#endif
    /* SPR_10881_Fix End */

    /* + SPR 20472 */ 
    resp = rlcParseReEstabMsg(msg_p, msgLen,&rlcReEstabCompInd,&current,&ueIndex);
    /* - SPR 20472 */ 

    if (resp == RLC_SUCCESS)
    {
        /* Update the state of the LC */
    /* + SPR 20472 */ 
        ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    /* - SPR 20472 */ 
        if (ueContext_p)
        {
            for ( idx = 0; idx < rlcReEstabCompInd.lcCount; idx++)
            {
                lcId = rlcReEstabCompInd.lcIdReEstList[idx].lcId;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
                rlcEntityInfo_p = ueContext_p->rlcEntityInfoList[lcId];
                rlcEntityInfo_p->rlcEntityState = RLC_ENTITY_ACTIVE;
#else
                rlcEntityInfo_p = ueContext_p->rlcEntityInfoList[lcId];
                if ( RLC_NULL == rlcEntityInfo_p)
                {
                    LOG_RLC_MSG(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_4, LOGWARNING,
                            RLC_RRC, globalTTITickCount_g, __LINE__,
                            rlcReEstabCompInd.ueIndex, 0, 0, 0, 0, 0, __func__,
                            "RLC_LCID_NOT_EXISTS");
                    resp = RLC_LCID_NOT_EXISTS;
                }
                else
                {
                    if (rlcEntityInfo_p->entityType == RLC_RX_ENTITY )
                    {
                        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
                        {
                            rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState =
                                RLC_ENTITY_ACTIVE;
                        }
                        else if (rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
                        {
                            rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState =
                                RLC_ENTITY_ACTIVE;
                        } 
                    }
                    else if ( rlcEntityInfo_p->entityType == RLC_TX_ENTITY )
                    {
                        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
                        {
                            rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState =
                                RLC_ENTITY_ACTIVE;
                        }
                        else if (rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
                        {
                            rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState =
                                RLC_ENTITY_ACTIVE;
                        }
                    }
                    /* SPR 6185, 6728 changes start */
                    else if( RLC_TX_RX_ENTITY == rlcEntityInfo_p->entityType )
                    {
                        if(rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_UM)
                        {
                            rlcEntityInfo_p->rlcEntity.umEntity_p->rxEntityState =
                                RLC_ENTITY_ACTIVE;
                            rlcEntityInfo_p->rlcEntity.umEntity_p->txEntityState =
                                RLC_ENTITY_ACTIVE;
                        }
                        else if (rlcEntityInfo_p->rlcEntityMode == RLC_ENTITY_AM)
                        {
                            rlcEntityInfo_p->rlcEntity.amEntity_p->rxEntityState =
                                RLC_ENTITY_ACTIVE;
                            rlcEntityInfo_p->rlcEntity.amEntity_p->txEntityState =
                                RLC_ENTITY_ACTIVE;
                        }
                    }
                    /* SPR 6185, 6728 changes end */
                }
#endif
            }
        } 
        else
        {
            LOG_RLC_MSG(LTE_RLC_HANDLE_RE_ESTABLISH_ERR_3, LOGWARNING,
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    rlcReEstabCompInd.ueIndex, 0, 0, 0, 0, 0, __func__, "RLC_UE_NOT_EXISTS");
            resp = RLC_UE_NOT_EXISTS;
        }
    }
    /* SPR 6185 changes start */
    if( RLC_SUCCESS != resp )
    {
        resp = RLC_FAILURE;
    }

    /* SPR 7274 Fix Start */
    cnfMsgIdx = RRC_RLC_API_HEADER_LEN; 
    /* SPR 7274 Fix End */
    resp_p = cnfMsgBuf;

#ifndef UE_SIM_TESTING
    /* SPR 21976 Fix Start */
    UInt16 rrcUeIndex=LTE_GET_U16BIT(msg_p+RRC_RLC_API_HEADER_LEN);
    /* SPR 21976 Fix End */
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),rrcUeIndex);
#else
    LTE_SET_U16BIT((resp_p + cnfMsgIdx), rlcReEstabCompInd.ueIndex );
#endif
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
    cnfMsgIdx += 2;

    /* Prepare response buffer */
    /* + Layer2 NON CA Changes */
    prepareCnfBufferHeader(resp_p,RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_CNF, transactionId, cellId, cnfMsgIdx);
    /* - Layer2 NON CA Changes */

    rlcSendMsgToRrc(cnfMsgBuf,
            cnfMsgIdx);
    /* SPR 6185 changes end */

    return resp;
}

/****************************************************************************
 * Function Name  : rlcParseReEstabMsg
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread,
 *                  rlcReEstab_p - Pointer to RlcReEstabCompInd structure,
 *                  current_p - Pointer to current Idx
 *                  ueIndex - Pointer to internal UE Index
 * Outputs        : None
 * Returns        : RLC_SUCCESS on success and error code on failure
 * Description    : This API is to parse the Re-Establish complete  msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static UInt32 rlcParseReEstabMsg ( UInt8 *msg_p,
                                          UInt16 msgLen,
                                          RlcReEstabReq *rlcReEstab_p,
                                          UInt16 *current_p
                                          /* + SPR 20472 */
                                          ,UInt16 *ueIndex
                                          /* - SPR 20472 */
                                          )
{
    UInt16 resp    = RLC_SUCCESS;
    UInt16 lcCount = 0;
    UInt16 current = *current_p;

    if ( msgLen < (RLC_RE_ESTABLISH_COMPLETE_MANDATORY_LEN))
    {
        LOG_RLC_MSG( LTE_RLC_PARSE_UE_IDX_ERR, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__, msgLen,
        0, 0, 0, 0, 0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }
    rlcReEstab_p->ueIndex = LTE_GET_U16BIT(msg_p + current);
#ifndef UE_SIM_TESTING
    /* + SPR 20472 */
    *ueIndex = getUeIdxFromRrcUeIdx(rlcReEstab_p->ueIndex);
#else
    *ueIndex = rlcReEstab_p->ueIndex;
    /* - SPR 20472 */
#endif
    current += 2;
    /* + SPR 20472 */
    resp = IS_UE_ID_INVALID(*ueIndex);
    /* - SPR 20472 */
    if ( resp != RLC_SUCCESS )
    {
        LOG_RLC_MSG (LTE_RLC_PARSE_UE_IDX_ERR_1, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__,rlcReEstab_p->ueIndex, msgLen,
        0, 0, 0, 0, __func__, "");
        return RLC_INVALID_UE_ID;
    }
    /* + SPR 20472 */
    resp = IS_UE_ID_NOT_EXISTS(*ueIndex);
    /* - SPR 20472 */
    if ( RLC_SUCCESS == resp )
    {
        LOG_RLC_MSG (LTE_RLC_PARSE_UE_IDX_ERR_2, LOGWARNING, RLC_RRC,
        globalTTITickCount_g, __LINE__,rlcReEstab_p->ueIndex, msgLen,
        0, 0, 0, 0, __func__, "UE does not exist");
        return RLC_UE_NOT_EXISTS;
    }

    msgLen = (msgLen + RRC_RLC_API_HEADER_LEN) - current;

    /* Parse Optional IEs if present */
    while ( msgLen > 4)
    {
        if ( lcCount >= MAX_LC_ID )
        {
            return RLC_SYNTAX_ERROR;
        }
        resp = parseRLCReEstabLcInfo( msg_p,
                 &(rlcReEstab_p->lcIdReEstList[lcCount]),
                 &current,
                 &msgLen);
        if ( RLC_SUCCESS != resp )
        {
            return resp;
        }
        else
        {
            lcCount++;
        }
    }
/*
    if ( msgLen > 0)
    {
        return RLC_SYNTAX_ERROR;
    }
*/
    rlcReEstab_p->lcCount = lcCount;
    *current_p = current;
    return resp; 
}
/******************************************************************************
 * Function Name  : parseRLCReEstabLcInfo
 * Inputs         : msg_p - points to the msg read from RRC,
 *                  rlcReEstabLcInfo_p - pointer of RlcReEstabCompLcInfo,
 *                  current_p - points to the current position from where the
 *                              msg to be read,
 *                  msgLen_p - points to msg length which is unread
 * Outputs        : None
 * Returns        : RLC_SUCCESS if parse successfully otherwise value which
 *                  indicates error message.
 * Description    : This API is to parse the msg and prepare the config params
 *                  which later is used to change the Entity state ENTITY_LCID.
 ******************************************************************************/
inline static UInt32 parseRLCReEstabLcInfo( UInt8* msg_p,
                                           RlcReEstabLcInfo *rlcReEstabLcInfo_p,
                                           UInt16 *current_p,
                                           UInt16 *msgLen_p )
{
    UInt16 tag       = 0;
    UInt16 tagLen    = 0;
    UInt16 errorCode = RLC_SUCCESS;
    UInt16 current = *current_p;
 
    tag = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if (tagLen > *msgLen_p)
    {
        LOG_RLC_MSG ( LTE_RLC_RRC_PARSE_UE_ENTITY_INFO_ERR, LOGWARNING, RLC_RRC,
                globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
                0, 0, 0, __func__, "");

        return RLC_SYNTAX_ERROR;
    }
    if (tag == ENTITY_LCID)
    {
        rlcReEstabLcInfo_p->lcId = *(msg_p + current);       
        current++;
        *msgLen_p -= tagLen;
        if (tagLen > LCID_TAG_LENGTH)
        {
            tag = LTE_GET_U16BIT(msg_p + current);
            current += 2;
            tagLen = LTE_GET_U16BIT(msg_p + current);
            current += 2;
            if (tag == RLC_ENTITY_STATE)
            {
                /* Rel 5.3: Coverity 32384 Fix Start */
                rlcReEstabLcInfo_p->state = (RlcState)(*(msg_p + current));
                if ((rlcReEstabLcInfo_p->state == RLC_FREEZE) || (rlcReEstabLcInfo_p->state == RLC_NO_ACTION))
                {
                    current++;
                }
                else
                {    
                    LOG_RLC_MSG ( LTE_RLC_RRC_PARSE_UE_ENTITY_INFO_ERR, LOGWARNING, RLC_RRC,
                    globalTTITickCount_g, __LINE__, tag, tagLen, rlcReEstabLcInfo_p->state,
                    0, 0, 0, __func__, "");
                     
                    return RLC_SYNTAX_ERROR;
                }
                /* Rel 5.3: Coverity 32384 Fix End */
            }
            else
            {
                /*+-Coverity 73144*/
                LOG_RLC_MSG ( LTE_RLC_RRC_PARSE_UE_ENTITY_INFO_ERR, LOGWARNING, RLC_RRC,
                globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
                0, 0, 0, __func__, "");

                return RLC_SYNTAX_ERROR;
            }
        } 
    }
    else
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_UE_ENTITY_INFO, LOGWARNING, RLC_RRC,
                  globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
                  0, 0, 0, __func__, "");
        errorCode = INVALID_RRC_MSG_ID;
    }

    LOG_RLC_MSG ( LTE_RLC_PARSE_UE_ENTITY_INFO_1, LOGWARNING, RLC_RRC,
            globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
            errorCode, 0, 0, __func__, "");
    *current_p = current;
    return errorCode;
}
/* SPR 4994 Changes End */

/****************************************************************************
 * Function Name  : handleRlcChangeCrntiMsg 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : ErrorCode 
 * Description    : This API is to parse the   msg
 *                  read and return error code or RLC_SUCCESS.
 ****************************************************************************/
inline static UInt32 handleRlcChangeCrntiMsg ( 
                                            UInt8 *msg_p,
                                            UInt16 msgLen 
                                            )
{
    UInt16 current = RRC_RLC_API_HEADER_LEN;  
    UInt16 resp = RLC_SUCCESS;
    UInt32 errorCode = RLC_SUCCESS;
    UInt8 cnfMsgBuf[MAX_DATASIZE] = {0};
    UInt8 *resp_p = PNULL;
    /* SPR 6621 Fix Start */
    /* SPR 4737 Fix Start */
    UInt16 cnfMsgIdx = 0;
    /* SPR 4737 Fix End */
    UInt16 transactionId = 0;
    RlcChangeCrntiReq rlcChangeCrntiReq = {0};
    /* + Layer2 NON CA Changes */
    RrcCellIndex cellId = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */
    /* + SPR 20472 */
    UInt16 ueIndex = INVALID_UE_INDEX;
    /* - SPR 20472 */

    transactionId = LTE_GET_U16BIT(msg_p);
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellId = *(msg_p + API_HEADER_CELL_INDEX_OFFSET);
#endif
    /* - Layer2 NON CA Changes */

     /* + SPR 20472 */ 
    resp = rlcParseChangeCrntiMsg(msg_p, msgLen,&rlcChangeCrntiReq,&current, &ueIndex);
     /* - SPR 20472 */ 
    if( resp != RLC_SUCCESS)
    {
        errorCode = resp;
        resp = RLC_FAILURE;
    }
    /* SPR 11083 fix - start */
    else
    {
        /* Update New CRNTI in UE context */
     /* + SPR 20472 */
        rlcUEContextList_g[ueIndex]->ueId = rlcChangeCrntiReq.newCrnti;
     /* - SPR 20472 */ 
    }
    /* SPR 11083 fix - end */
    cnfMsgIdx = RRC_RLC_API_HEADER_LEN;
    resp_p = cnfMsgBuf;
     /* + SPR 20472 */ 
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),rlcChangeCrntiReq.ueIndex);
     /* - SPR 20472 */ 
    
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
    cnfMsgIdx += 2;
    prepareCnfBufferHeader(resp_p,RLC_CHANGE_CRNTI_CNF,transactionId,cellId,cnfMsgIdx);
    /* SPR 6621 Fix End */
    rlcSendMsgToRrc(cnfMsgBuf,
            cnfMsgIdx);
    return errorCode;
}


/****************************************************************************
 * Function Name  : handleUeEntityReq
 * Inputs         : msgId - Message Type of the message,
 *                : msg_p - Pointer to the data received from RRC,
 *                  msgLen - indicate the length of msg unread
 * Outputs        : None
 * Returns        : errorCode
 * Description    : This API is to parse the msg read and called the relevant
 *                  APIs.
 *                  1. RLC_CREATE_UE_ENTITY_REQ 
 *                  2. RLC_DELETE_UE_ENTITY_REQ    
 *                  3. RLC_RECONFIG_UE_ENTITY_REQ
 *                  4. RLC_RE_ESTABLISH_UE_ENTITY_REQ 
 *
 ****************************************************************************/
//U8 to U16
inline STATIC UInt32  handleUeEntityReq( UInt16 msgId,
                                   UInt8 *msg_p,
                                   UInt16 msgLen
                                    )
{
    UInt32 errorCode = INVALID_ERR_CODE;
    switch ( msgId ) 
    {
        case RLC_CREATE_UE_ENTITY_REQ:

            errorCode = handleRlcCreateUEEntity( msg_p, msgLen );

            break;

        case RLC_DELETE_UE_ENTITY_REQ:

            errorCode = handleRlcDeleteEntity ( msg_p, msgLen );

            break;

        case RLC_RECONFIG_UE_ENTITY_REQ:

            errorCode = handleRlcReconfigMsg ( msg_p, msgLen );

            break;

        case RLC_RE_ESTABLISH_UE_ENTITY_REQ:

            errorCode = handleRlcReEstablishMsg ( msg_p, msgLen );

            break;
        case RLC_CHANGE_CRNTI_REQ:

            errorCode = handleRlcChangeCrntiMsg(msg_p, msgLen );

            break;
/* SPR 4994 Changes Start */
        case RLC_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND: 

            errorCode = handleRlcReEstablishCompInd ( msg_p, msgLen );        

            break;
/* SPR 4994 Changes End */

        default:

            LOG_RLC_MSG( LTE_RLC_RRC_HANDLE_UE_ENTITY_REQ_ERR, LOGWARNING, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    msgId, 0, 0, 0, 0, 0, __func__,
                    "");

            errorCode = RLC_RRC_INVALID_RRC_MSG_ID;

            break;
    }/* end of switch */

    LOG_RLC_MSG( LTE_RLC_RRC_HANDLE_UE_ENTITY_REQ, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            msgId, errorCode, 0, 0, 0, 0, __func__, "");

    return errorCode;
}


/****************************************************************************
 * Function Name  : sendCnfMsgToRRC
 * Inputs         : msgId - Message Type of the message,
 *                : transactionId - Transaction ID to be filled in header,
 *                  rlcConfigReq_p - Pointer to Config structure,
 *                  resp - Response for the confirmation message,
 *                  cellId - RRC Cell Index,
 *                  rrcUeIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API is to send the confirmation message to RRC.
 ****************************************************************************/

inline STATIC void sendCnfMsgToRRC( UInt16 msgId, 
                                    UInt16 transactionId, 
                                    RlcConfigReq *rlcConfigReq_p, 
                                    UInt16 resp,
                                    /* + Layer2 NON CA Changes */
                                    RrcCellIndex cellId
                                    /* - Layer2 NON CA Changes */
/* SPR 4690 Start */                                    
#ifndef UE_SIM_TESTING 
                                    ,UInt16 rrcUeIndex
#endif
/* SPR 4690 End */                                    
                                )
{
    /*As per SPR 698 - 4737 change start*/
    UInt16 cnfMsgIdx = RRC_RLC_API_HEADER_LEN;
    /*As per SPR 698 - 4737 change end*/
    /* arry for confirmation msg */

    /* + Coverity_Stack Fix */
    UInt8 *cnfMsgBuf = PNULL;
    UInt8 *resp_p = PNULL;
    cnfMsgBuf = (UInt8 *)getMemFromPool( MAX_DATASIZE, PNULL);
    /* - Coverity_Stack Fix */

    if(PNULL == cnfMsgBuf)
    {
        lteWarning("In %s, MEMORY ALLOC FAIL\n",__func__);
        return;
    } 

    /* SPR 11109 Fix start */
    memSet (cnfMsgBuf, 0, MAX_DATASIZE);
    /* SPR 11109 Fix end */
    UInt8 idx = 0;
    UInt16 msgCnf = RLC_DELETE_UE_ENTITY_CNF;

    resp_p = cnfMsgBuf;
#ifndef UE_SIM_TESTING
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),rrcUeIndex);
#else
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),rlcConfigReq_p->ueIndex);
#endif
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((resp_p + cnfMsgIdx),resp);
    cnfMsgIdx += 2;

    if (( RLC_PARTIAL_SUCCESS == resp) && (msgCnf != msgId))
    {
        for ( idx = 0; idx < rlcConfigReq_p->lcCount; idx++)
        {
            if ( RLC_SUCCESS != rlcConfigReq_p->rlcEntityCfgList[idx].result )
            {
                if ( RLC_ADD_ENTITY == 
                        rlcConfigReq_p->rlcEntityCfgList[idx].opCode)
                {
                    /* putting tag for create entity error */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),CREATE_ENTITY_ERROR);
                    cnfMsgIdx += 2;
                    /* tag length of CREATE_ENTITY_ERROR tag is 8 */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            ENTITY_ERROR_TAG_LENGTH);
                    cnfMsgIdx += 2;
                }
                else if ( RLC_DELETE_ENTITY == 
                        rlcConfigReq_p->rlcEntityCfgList[idx].opCode)
                {
                    /* putting tag for create entity error */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),DELETE_ENTITY_ERROR);
                    cnfMsgIdx += 2;
                    /* tag length of CREATE_ENTITY_ERROR tag is 8 */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            ENTITY_ERROR_TAG_LENGTH);
                    cnfMsgIdx += 2;
                }
                else
                {
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            RECONFIG_ENTITY_ERROR);
                    cnfMsgIdx += 2;
                    /* tag length of CREATE_ENTITY_ERROR tag is 8 */
                    LTE_SET_U16BIT((resp_p + cnfMsgIdx),
                            ENTITY_ERROR_TAG_LENGTH);
                    cnfMsgIdx += 2;
                }
                /* lcId of entity */
                resp_p[cnfMsgIdx++] = 
                    rlcConfigReq_p->rlcEntityCfgList[idx].lcId & 0x00FF;
                /* response of create request */
                /* SPR 4008 Fix Start */
                LTE_SET_U16BIT((resp_p + cnfMsgIdx),RLC_FAILURE);
                /* SPR 4008 Fix End */
                cnfMsgIdx += 2;
            }
            LOG_RLC_MSG( LTE_RLC_SEND_CNF_MSG_TO_RRC, LOGINFO, 
                    RLC_RRC, globalTTITickCount_g, __LINE__,
                    msgId, rlcConfigReq_p->rlcEntityCfgList[idx].opCode,
                    rlcConfigReq_p->rlcEntityCfgList[idx].result,
                    0, 0, 0, __func__, "");
        }    
    }

    /* + Layer2 NON CA Changes */
    /* Prepare response to be send to RRC */
    /* creating buffer header for reconfig cnf message */
    prepareCnfBufferHeader(resp_p, msgId, transactionId, cellId, cnfMsgIdx);
    /* - Layer2 NON CA Changes */

    /* writting to socket */
    rlcSendMsgToRrc(
            cnfMsgBuf,
            cnfMsgIdx);
    /* + Coverity_Stack Fix */
    freeMemPool(cnfMsgBuf);
    /* - Coverity_Stack Fix */
}                

/****************************************************************************
 * Function Name  : prepareCnfBufferHeader
 * Inputs         : cnfMsgBuf - Buffer to be filled,
 *                  cnfMsgId - Cnf Msg Type to be send to RRC,
 *                : transactionId - transaction Id to be filled
 *                : cellId - Cell Index to be filled
 *                : length - Size of the whole message including header
 * Outputs        : cnfMsgBuf - Buffer to be filled
 * Returns        : None
 * Variables      :
 * Description    : This API takes the buffer to be filled by message
 *                  header as input.
 ****************************************************************************/
/* CA Changes start */
void prepareCnfBufferHeader ( UInt8 *cnfMsgBuf, 
                                     UInt16 cnfMsgId, 
                                     UInt16 transactionId, 
                                     RrcCellIndex cellId,
                                     UInt16 length
                            )
{
    UInt16 cnfMsgIdx=0;

    LTE_SET_U16BIT((cnfMsgBuf + cnfMsgIdx),transactionId);
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((cnfMsgBuf + cnfMsgIdx),RLC_MODULE_ID);
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((cnfMsgBuf + cnfMsgIdx),RRC_MODULE_ID);
    cnfMsgIdx += 2;
    LTE_SET_U16BIT((cnfMsgBuf + cnfMsgIdx),cnfMsgId);
    cnfMsgIdx += 2;
    /* Set the length */
    LTE_SET_U16BIT((cnfMsgBuf + cnfMsgIdx),length);
    cnfMsgIdx += 2;
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
/* - Layer2 NON CA Changes */
    /* set the cell Id sent by RRC */
    *(cnfMsgBuf+cnfMsgIdx)= cellId;
    cnfMsgIdx++;
#endif
}
/* CA Changes end */



/****************************************************************************
 * Function Name  : parseCreateTxUMRlcEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - Pointer to current message,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/Error Code
 * Description    : This API is to parse the msg for CREATE TX UM Entity
 *                  1. Parse CREATE TX UM Entity
 *                  2. Validate the Parameters 
 *                  3. if validation fails then return the errorCode 
 *                  4. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseCreateTxUMRlcEntity(
                                      UInt8 *msg_p,
                                      UInt16 *current_p,
                                      RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                    )
{
    UInt8 lcId           = RLC_INVALID_LC_ID;
    UInt8 resp           = RLC_FAILURE;
    UInt16 snFieldLen    = 0;
    UInt16 current       = *current_p;
    UInt16 tagLen        = 0;
#ifndef UE_SIM_TESTING
    UInt16 createTxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
#ifdef ENDC_ENABLED
    DCBearerType dcBearerType = INVALID_DC_BEARER;
#endif
#endif

    /* CREATE_TX_UM_RLC_ENTITY API
       API Element       Bytes        Range
       1. TagLen         2 bytes      7 
       2. LCID           1 bytes      0 - 10
       3. snFieldLength  2 bytes      5 or 10
    */
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if ( tagLen < TX_UM_ENTITY_TAG_LENGTH )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_UM_RLC_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0,
                __func__, "");
        return RLC_SYNTAX_ERROR;
    } 

    lcId = *(msg_p + current);
    current++;

    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_UM_RLC_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0,
                __func__, "");
        return resp;
    }     

    snFieldLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if ( (size5 != snFieldLen) && (size10 != snFieldLen ) )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_UM_RLC_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, snFieldLen, 0, 0, 0,
                __func__, "");
        return RLC_SYNTAX_ERROR;
    }
    
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    createTxUmLength = tagLen - CREATE_TX_UM_RLC_ENTITY_MIN_LENGTH;
    while(createTxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        createTxUmLength = createTxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen !=QCI_INFO_TAG_LENGTH )
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
#ifdef ENDC_ENABLED
			case RLC_DC_BEARER_TYPE_INFO:
                {
                    if(subTagLen != DC_BEARER_TYPE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    dcBearerType = (DCBearerType)(*(msg_p + current));
					
					/*Only SCG Bearer can be configured in UM mode */
					if (dcBearerType != SN_TERMINATED_SCG_SPLIT_BEARER)
					{
					    LOG_RLC_ERROR( RLC_RRC, "Wrong DC Bearer [%u] configuration in UM mode [%s] ",dcBearerType,__func__ );
					    return RLC_WRONG_DC_BEARER_CONFIGURATION;
					}
                    current += 1;
                }
			    break;
#endif				
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 changes End **/
    
    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_ADD_ENTITY;
    /* + Coverity 32389 */
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.snFieldLength = 
							(SnFieldLength) snFieldLen;
    /* - Coverity 32389 */
    
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.
        rlcQueueSize = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.
        bitRateTx = txBitRate;
#ifdef ENDC_ENABLED
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.dcBearerType = dcBearerType;
#endif
#endif
    /** Rel 4.0 Changes End **/
    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_CREATE_TX_UM_RLC_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            lcId, snFieldLen, 0, 0, 0, 0,
            __func__, "");

    return RLC_SUCCESS;
}
     
/****************************************************************************
 * Function Name  : parseCreateRxUMRlcEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - Pointer to current message,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API is to parse the msg for CREATE RX UM Entity
 *                  1. Parse CREATE RX UM Entity
 *                  2. Validate the Parameters 
 *                  3. if validation fails then return the errorCode 
 *                  4. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseCreateRxUMRlcEntity(
                                             UInt8 *msg_p,
                                             UInt16 *current_p,
                                             RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                           )
{
    UInt8  lcId     = RLC_INVALID_LC_ID;
    UInt8  resp     = RLC_FAILURE;
    UInt16 current  = *current_p;
    UInt16 tagLen   = 0;
    UInt16 snFieldLen  = 0; 
    UInt16 tReordering = 0;
#ifndef UE_SIM_TESTING
    UInt16 createRxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rxBitRate        = 0;
#ifdef ENDC_ENABLED
	DCBearerType dcBearerType = INVALID_DC_BEARER;
#endif
#endif

    /* CREATE_RX_UM_RLC_ENTITY API
       API Element       Bytes        Range
       1. TagLen         2 bytes      9
       2. LCID           1 bytes      0 - 10
       3. snFieldLength  2 bytes      5 or 10
       4. tReordering    2 bytes      0 - 200 ms
     */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen < RX_UM_ENTITY_TAG_LENGTH )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_UM_RLC_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }   
    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_UM_RLC_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    snFieldLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating snField Length */
    if ( (size5 != snFieldLen) && (size10 != snFieldLen) )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_UM_RLC_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, snFieldLen, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }
    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if ( RLC_SUCCESS != validateTReordering(tReordering))
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_UM_RLC_ERR_3, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, snFieldLen, tReordering,
                0, 0, __func__,"");
        return RLC_SYNTAX_ERROR;
    }
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    createRxUmLength = tagLen - CREATE_RX_UM_RLC_ENTITY_MIN_LENGTH;
    while(createRxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        createRxUmLength = createRxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {        if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
#ifdef ENDC_ENABLED
			case RLC_DC_BEARER_TYPE_INFO:
                {
                    if(subTagLen != DC_BEARER_TYPE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    dcBearerType = (DCBearerType)(*(msg_p + current));
					
					if ((dcBearerType != SN_TERMINATED_SCG_SPLIT_BEARER))
					{
					    LOG_RLC_ERROR( RLC_RRC, "Wrong DC Bearer [%u] configuration in UM mode [%s] ",dcBearerType,__func__ );
					    return RLC_WRONG_DC_BEARER_CONFIGURATION;
					}
                    current += 1;
                }
			    break;
#endif					
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_ADD_ENTITY;
    RxUmRlc *rxUmRlc_p = &(rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc);
    /* + Coverity 32392, 32393 */
    rxUmRlc_p->snFieldLength = (SnFieldLength ) snFieldLen;
    rxUmRlc_p->tReordering   = (TReordering ) tReordering;
    /* - Coverity 32392, 32393 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc.
        bitRateRx = rxBitRate;
#ifdef ENDC_ENABLED
	rxUmRlc_p->dcBearerType = dcBearerType;
#endif
#endif
    /** Rel 4.0 Changes End **/

    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_CREATE_RX_UM_RLC_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            lcId, snFieldLen, tReordering, 0, 0, 0, __func__,
            "");
    return RLC_SUCCESS;
}
                                  
/****************************************************************************
 * Function Name  : parseCreateTxRxUMRlcEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/Error code
 * Description    : This API is to parse the CREATE_TX_RX_UM_RLC_ENTITY msg
 *                  1. Parse the TX-RX UM Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static UInt32 parseCreateTxRxUMRlcEntity(
                                                  UInt8  *msg_p,
                                                  UInt16 *current_p,
                                                  RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                               )
{

    UInt16 snFieldLen_tx = 0;
    UInt16 snFieldLen_rx = 0;
    UInt16 tReordering   = 0;
    UInt8 lcId           = RLC_INVALID_LC_ID;
    UInt8 resp           = RLC_FAILURE;
    UInt16 current       = *current_p;
    UInt16 tagLen        = 0;
#ifndef UE_SIM_TESTING
    UInt16 createTxRxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
    UInt32 rxBitRate        = 0;
#ifdef ENDC_ENABLED
	DCBearerType dcBearerType = INVALID_DC_BEARER;
#endif
#endif

    /* CREATE_RX_UM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      11
       2. LCID               1 bytes      0 - 10
       3. snFieldLength Tx   2 bytes      5 or 10
       4. snFieldLength Rx   2 bytes      5 or 10
       5. tReordering        2 bytes      0 - 200 ms

    */
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen < TX_RX_UM_ENTITY_TAG_LENGTH )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_RX_UM_ERR, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen,0,0,0, 0,0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }  
 
    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_RX_UM_ERR_1, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId,0,0, 0,0, __func__, 
            "");
        return resp;
    }
    
    snFieldLen_tx = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    snFieldLen_rx = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating snField Length */
    if (  ((   size5 != snFieldLen_tx) 
                && (size10 != snFieldLen_tx)) 
            || ((size5 != snFieldLen_rx) 
                && (size10 != snFieldLen_rx)))
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_RX_UM_ERR_2, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, snFieldLen_tx, snFieldLen_rx, 0,0, __func__, 
            "");
        return RLC_SYNTAX_ERROR;
    }
    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if ( RLC_SUCCESS != validateTReordering(tReordering) )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_TX_RX_UM_ERR_3, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, tReordering,0, 0,0, __func__, 
            "");
        return RLC_SYNTAX_ERROR;
    }
    
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    createTxRxUmLength = tagLen - CREATE_TX_RX_UM_RLC_ENTITY_MIN_LENGTH;
    while (createTxRxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        createTxRxUmLength = createTxRxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
#ifdef ENDC_ENABLED
			case RLC_DC_BEARER_TYPE_INFO:
                {
                    if(subTagLen != DC_BEARER_TYPE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    dcBearerType = (DCBearerType)(*(msg_p + current));
					
					if ((dcBearerType != SN_TERMINATED_SCG_SPLIT_BEARER))
					{
					    LOG_RLC_ERROR( RLC_RRC, "Wrong DC Bearer [%u] configuration in UM mode [%s] ",dcBearerType,__func__ );
					    return RLC_WRONG_DC_BEARER_CONFIGURATION;
					}
                    current += 1;
                }
			    break;
#endif					
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/
    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_ADD_ENTITY;
    /* Rel 5.3: Coverity 32387 Fix Start */
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        snFieldLength              = (SnFieldLength)snFieldLen_tx;
    /* Rel 5.3: Coverity 32387 Fix End */
    /* Rel 5.3: Coverity 32386 Fix Start */
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
        snFieldLength              = (SnFieldLength)snFieldLen_rx;
    /* Rel 5.3: Coverity 32386 Fix End */
    /* + Coverity 32388 */
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
        tReordering                = (TReordering )tReordering;
    /* - Coverity 32388 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        rlcQueueSize = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        bitRateTx = txBitRate;
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
        bitRateRx = rxBitRate;
#ifdef ENDC_ENABLED 
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        dcBearerType = dcBearerType;
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
		dcBearerType = dcBearerType;
#endif
#endif
    /** Rel 4.0 Changes End **/

    (*current_p) = current;
    
    LOG_RLC_MSG ( LTE_RLC_CREATE_TX_RX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            lcId, snFieldLen_tx, snFieldLen_rx, tReordering, 0, 0,
            __func__, "");

    return RLC_SUCCESS; 
}

/****************************************************************************
 * Function Name  : parseCreateRxTxAMEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the CREATE_TX_RX_AM_RLC_ENTTIY msg
 *                  1. Parse the TX-RX AM Entity Msg. 
 *                  2. Validate the parameters.
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseCreateRxTxAMEntity(
                                           UInt8* msg_p,
                                           UInt16 *current_p,
                                           RlcEntityCfgInfo *rlcEntityCfgInfo_p 
                                         )
{
    UInt8 lcId              = RLC_INVALID_LC_ID;
    UInt8 resp              = RLC_FAILURE;
    UInt16 tPollRetransmit  = 0;
    UInt16 pollPDU          = 0;
    UInt16 pollByte         = 0;
    UInt16 maxRetxThreshold = 0; 
    UInt16 tReordering      = 0;
    UInt16 tStatusProhibit  = 0;
    UInt64 current          = *current_p;
    UInt16 tagLen           = 0;
#ifndef UE_SIM_TESTING
    UInt16 createTxRxAmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
    UInt32 rxBitRate        = 0;
#endif
#ifdef ENDC_ENABLED
	    DCBearerType dcBearerType = INVALID_DC_BEARER;
#endif

    LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            0,0,0,0, 0,0, __func__, "Creating TxRx AM Rlc Entity");
  /* CREATE_TX_RX_AM_RLC_ENTTIY  API

       API Element              Bytes        Range
       1. TagLen                2 bytes      17
       2. LCID                  1 bytes      0 - 10
       3. tPollRetransmission   2 bytes      5 ms - 500 ms
       4. pollPDU               2 bytes      4, 8, 16, 32, 64, 128, 256, Infinity
       5. pollByte              2 bytes      25 kBytes -  3000 kBytes, infinity
       6. tReordering           2 bytes      0 - 200 ms
       7. maxReTxThrshld        2 bytes      1 - 32
       8. tReordering           2 bytes      0 ms - 200 ms 
       9. tStatusProhibit       2 bytes      0 ms - 500 ms
    */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if ( tagLen < TX_RX_AM_ENTITY_TAG_LENGTH )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen,0,0,0, 0,0, __func__, "");
        return RLC_SYNTAX_ERROR;
    }
    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_1, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, 0,0, 0,0, __func__, "");
        return resp;
    }

    tPollRetransmit = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    /* validating tPollRetransmit  */
    if( RLC_SUCCESS != validatePollRetransmit ( tPollRetransmit ) ) 
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_2, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, tPollRetransmit, 0, 0,0,
            __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    pollPDU = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating pollPDU  */
    resp = validatePollPDU(pollPDU);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_3, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, tPollRetransmit, pollPDU, 0,0,
            __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    pollByte = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating PollByte  */
    resp = validatePollByte(pollByte);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_4, LOGWARNING, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, pollByte, 0,0,0,
            __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    maxRetxThreshold = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating maxRetxThreshold */
    resp = validateMaxReTxThrshld(maxRetxThreshold);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_5, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, maxRetxThreshold,0 , 0,0,
                __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if(RLC_SUCCESS != validateTReordering(tReordering))
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_6, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                lcId, tPollRetransmit, tReordering, 0,0, 0,
                __func__, "");
        return RLC_SYNTAX_ERROR;
    }

    tStatusProhibit = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tStatusProhibit value */
    resp = validateStatusProhibit(tStatusProhibit);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ERR_7, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                lcId, tPollRetransmit, tReordering, tStatusProhibit,0, 0,
                __func__, "");
        return RLC_SYNTAX_ERROR;
    }
    
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    createTxRxAmLength = tagLen - CREATE_TX_RX_AM_RLC_ENTITY_MIN_LENGTH;
    while(createTxRxAmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        createTxRxAmLength = createTxRxAmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
#ifdef ENDC_ENABLED
			case RLC_DC_BEARER_TYPE_INFO:
                {
                    if(subTagLen != DC_BEARER_TYPE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    dcBearerType = (DCBearerType)(*(msg_p + current));
					
					if ((dcBearerType != SN_TERMINATED_SCG_SPLIT_BEARER))
					{
					    LOG_RLC_ERROR( RLC_RRC, "Wrong DC Bearer [%u] configuration in UM mode [%s] ",dcBearerType,__func__ );
					    return RLC_WRONG_DC_BEARER_CONFIGURATION;
					}
                    current += 1;

                }
			    break;
#endif					
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_AM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode = RLC_ADD_ENTITY;

    TxAmRlc *txAmRlc_p = &(rlcEntityCfgInfo_p->configParam.am.txAmRlc);
     RxAmRlc *rxAmRlc_p = 
         &(rlcEntityCfgInfo_p->configParam.am.rxAmRlc);
    /* + Coverity 32397, 32396, 32395, 32394, 32390, 32391 */
    txAmRlc_p->tPollRetransmit  = (TPollRetransmit) tPollRetransmit;
    txAmRlc_p->pollPDU          = (PollPDU) pollPDU;
    txAmRlc_p->pollByte         =  pollByte * 1024;
    txAmRlc_p->maxRetxThreshold = (MaxRetxThreshold) maxRetxThreshold;
    rxAmRlc_p->tReordering      = (TReordering) tReordering;
    rxAmRlc_p->tStatusProhibit  = (TStatusProhibit) tStatusProhibit;
    /* - Coverity 32397, 32396, 32395, 32394, 32390, 32391 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.am.txAmRlc.rlcQueueSize
        = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.am.txAmRlc.bitRateTx = txBitRate;
    rlcEntityCfgInfo_p->configParam.am.rxAmRlc.bitRateRx = rxBitRate;
#endif
    /** Rel 4.0 Changes end **/
#ifdef ENDC_ENABLED
    rlcEntityCfgInfo_p->configParam.am.txAmRlc.dcBearerType = dcBearerType;
    rlcEntityCfgInfo_p->configParam.am.rxAmRlc.dcBearerType = dcBearerType;
#endif
    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ENTITY_1, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            lcId, tPollRetransmit, pollPDU, pollByte, 
            0,0, __func__, "");

    LOG_RLC_MSG ( LTE_RLC_CREATE_RX_TX_ENTITY_2, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            lcId, tStatusProhibit, maxRetxThreshold,tReordering, 
            0, 0, __func__, "");

    return RLC_SUCCESS;
}
#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : parseBitRateInfo
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  ueIndex - Index of UE,
 *                  ueBitRateFlag - UE Bit rate Flag
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode
 * Description    : This API is to parse the CREATE_TX_RX_AM_RLC_ENTTIY msg
 *                  1. Parse the TX-RX AM Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseBitRateInfo(
                                     UInt8* msg_p,
                                     UInt16 *current_p,
                                     UInt16 ueIndex,
                                     UInt32 *ueBitRateFlag
                                   )
{
    UInt16 current = *current_p;
    UInt16 tagLen  = 0;
    UInt32  txBitRate = 0;
    UInt32  rxBitRate = 0;
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if(tagLen != UE_BIT_RATE_TAG_LENGTH)
    {
        return RLC_SYNTAX_ERROR;
    }
    txBitRate = LTE_GET_U32BIT(msg_p + current);
    current += 4;
    rxBitRate = LTE_GET_U32BIT(msg_p + current);
    current += 4;
    rlcUeBitRateInfo_g[ueIndex].bitRateTx = txBitRate;
    rlcUeBitRateInfo_g[ueIndex].bitRateRx = rxBitRate;
    *ueBitRateFlag = RLC_TRUE;
    *current_p = current;

    return RLC_SUCCESS;

}

/****************************************************************************
 * Function Name  : parseSpsConfigInfo
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcConfigReq_p - Pointer to RlcConfigReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the RLC_SPS_CONFIG tag
 *                  1. Validate the parameters and nested tags.
 ****************************************************************************/
inline static UInt16 parseSpsConfigInfo(UInt8* msg_p,
        UInt16 *current_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex )
    /* SPR 12917 fix end */   
{
    UInt16 current       = *current_p;
    UInt16 tagLen        = 0;
    UInt16 subTag        = 0;
    UInt16 subTagLen     = 0;
    UInt16 spsTagLen     = 0;
    UInt16 retVal = RLC_SUCCESS; 

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if( RLC_SPS_CONFIG_MIN_LENGTH > tagLen )
    {
        return RLC_SYNTAX_ERROR;
    }

    spsTagLen = tagLen - RLC_SPS_CONFIG_MIN_LENGTH;
    while( 0 < spsTagLen )
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        spsTagLen = spsTagLen - subTagLen;
        switch( subTag )
        {
            case RLC_SPS_DL_INFO:
            {
                retVal = parseRlcSpsDlInfo( &current, subTagLen, msg_p,
                        /* SPR 12917 fix start */
                        rlcConfigReq_p, cellIndex);
                /* SPR 12917 fix end */ 
                if (retVal != RLC_SUCCESS)
                {
                    return retVal;
                }
                break;
            }
            case RLC_SPS_UL_INFO:
            {
                retVal = parseRlcSpsUlInfo( &current, subTagLen, msg_p,
                        /* SPR 12917 fix start */
                        rlcConfigReq_p, cellIndex);
                /* SPR 12917 fix end */ 
                if (retVal != RLC_SUCCESS)
                {
                    return retVal;
                }
                break;
            }
            default:
            {
                break;
            }
        }

    }
    *current_p = current;
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseRlcSpsDlInfo 
 * Inputs         : current - indicate the pointer to the msg unread,
 *                  subTagLen - Length of subtag,
 *                  msg_p - Pointer to the data received from RRC,
 *                  rlcConfigReq_p - Pointer to RlcConfigReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the RLC_SPS_DL_INFO msg.
 ****************************************************************************/
STATIC inline UInt16 parseRlcSpsDlInfo( UInt16 *current,
        UInt16 subTagLen,
        UInt8 *msg_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex )
    /* SPR 12917 fix end */   
{
    UInt8  dlRequestType = 0;
    UInt16 subTag2       = 0;
    UInt16 subTag2Len    = 0;
    UInt8  semiPersistentSchedIntervalDl = 0;

    if( RLC_SPS_DL_INFO_MIN_LENGTH > subTagLen )
    {
        return RLC_SYNTAX_ERROR;
    }
    dlRequestType = *(msg_p + *current);
    *current += 1;
    if( LTE_TRUE < dlRequestType )
    {
        return RLC_SYNTAX_ERROR;
    }
    rlcConfigReq_p->rlcUeSpsInfo.dlRequestType = dlRequestType;

    subTagLen -= RLC_SPS_DL_INFO_MIN_LENGTH;
    while(0< subTagLen)
    {
        subTag2 = LTE_GET_U16BIT(msg_p + *current);
        *current = *current + 2;
        subTag2Len = LTE_GET_U16BIT(msg_p + *current);
        *current = *current + 2;

        subTagLen = subTagLen - subTag2Len;
        switch( subTag2 )
        {
            /*SPR 6861 Fix start*/
            case RLC_SPS_DL_SETUP_INFO:
            /*SPR 6861 Fix end*/
            {
                if( RLC_SPS_DL_SETUP_INFO_LENGTH != subTag2Len )
                {
                    return RLC_SYNTAX_ERROR;
                }
                semiPersistentSchedIntervalDl = *(msg_p + *current);
                /*Coverity 72872_97205 +*/
                if(SPS_INTERVAL_LIST <= semiPersistentSchedIntervalDl)
                {
                    return RLC_SYNTAX_ERROR;
                }
                /*Coverity 72872_97205 -*/
                *current += 1;

                /* if SPS is enabled by OAM in system, configure SPS
                 * to UE and LCs */
                if( LTE_FALSE == secondaryCellInfo_g[cellIndex].
                        supportedDlIntervalList[semiPersistentSchedIntervalDl])
                {
                    return RLC_SYNTAX_ERROR;
                }
                rlcConfigReq_p->rlcUeSpsInfo.semiPersistentSchedIntervalDl 
                    = semiPersistentSchedIntervalDl;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    /* Set BitMask that SPS DL config has recieved */
    rlcConfigReq_p->spsBitmask |= 0x01;
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseRlcSpsUlInfo 
 * Inputs         : current - indicate the pointer to the msg unread,
 *                  subTagLen - Length of subtag,
 *                  msg_p - Pointer to the data received from RRC,
 *                  rlcConfigReq_p - Pointer to RlcConfigReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the RLC_SPS_UL_INFO msg.
 ****************************************************************************/
STATIC inline UInt16 parseRlcSpsUlInfo(UInt16 *current,
        UInt16 subTagLen,
        UInt8 *msg_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex )
    /* SPR 12917 fix end */   
{    
    UInt8  ulRequestType = 0;
    UInt16 subTag2       = 0;
    UInt16 subTag2Len    = 0;
    UInt8  semiPersistentSchedIntervalUl = 0;

    if( RLC_SPS_UL_INFO_MIN_LENGTH > subTagLen )
    {
        return RLC_SYNTAX_ERROR;
    }
    ulRequestType = *(msg_p + *current);
    *current += 1;
    if( LTE_TRUE < ulRequestType )
    {
        return RLC_SYNTAX_ERROR;
    }
    rlcConfigReq_p->rlcUeSpsInfo.ulRequestType = ulRequestType;

    subTagLen -= RLC_SPS_UL_INFO_MIN_LENGTH;
    while(0< subTagLen)
    {
        subTag2 = LTE_GET_U16BIT(msg_p + *current);
        *current = *current + 2;
        subTag2Len = LTE_GET_U16BIT(msg_p + *current);
        *current = *current + 2;

        subTagLen = subTagLen - subTag2Len;
        switch( subTag2 )
        {
            /*SPR 6861 Fix start*/
            case RLC_SPS_UL_SETUP_INFO:
            /*SPR 6861 Fix end*/
            {
                if( RLC_SPS_UL_SETUP_INFO_LENGTH != subTag2Len )
                {
                    return RLC_SYNTAX_ERROR;
                }
                semiPersistentSchedIntervalUl = *(msg_p + *current);
                /*Coverity 72785_97206 +*/
                if(SPS_INTERVAL_LIST <= semiPersistentSchedIntervalUl)
                {
                    return RLC_SYNTAX_ERROR;
                }
                /*Coverity 72785_97206 -*/

                *current += 1;

                /* if SPS is enabled by OAM in system, configure SPS to
                 * UE and LCs */
                if( LTE_FALSE == secondaryCellInfo_g[cellIndex].\
                        supportedUlIntervalList[semiPersistentSchedIntervalUl])
                {
                    return RLC_SYNTAX_ERROR;
                }
                rlcConfigReq_p->rlcUeSpsInfo.semiPersistentSchedIntervalUl = 
                    semiPersistentSchedIntervalUl;
                break;
            }
            default:
            {
                break;
            }
        }
    }
    /* Set BitMask that SPS UL config has recieved */
    rlcConfigReq_p->spsBitmask |= 0x02;
    return RLC_SUCCESS;
}
#endif

/****************************************************************************
 * Function Name  : parseDelTxUMEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 *                  msgLen - indicate the length of msg unread.
 * Description    : This API is to parse the DELETE_TX_UM_RLC_ENTITY msg
 *                  1. Parse the Delete TX UM RLC Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseDelTxUMEntity(
                                      UInt8 *msg_p,
                                      UInt16 *current_p,
                                      RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                    )
{
    UInt8 lcId              = RLC_INVALID_LC_ID;
    UInt8 resp              = RLC_FAILURE;
    UInt16 current          = *current_p;
    UInt16 tagLen           = 0;

  
/*  DELETE_TX_UM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      5
       2. LCID               1 bytes      0 - 10
*/
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen != LCID_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    rlcEntityCfgInfo_p->lcId = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_ENTITY;
    rlcEntityCfgInfo_p->opCode = RLC_DELETE_ENTITY;

    (*current_p) = current;
    
    LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__, lcId, 0, 0, 0,
            0, 0, __func__, "");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseDelRxUMEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the DELETE_RX_UM_RLC_ENTITY msg
 *                  1. Parse the Delete RX UM RLC Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseDelRxUMEntity(
                                      UInt8 *msg_p,
                                      UInt16 *current_p,
                                      RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                    )
{
    UInt8 lcId     = RLC_INVALID_LC_ID;
    UInt8 resp     = RLC_FAILURE;
    UInt16 current = *current_p;
    UInt16 tagLen  = 0;

 /*  DELETE_RX_UM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      5
       2. LCID               1 bytes      0 - 10
  */
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen != LCID_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_RX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_RX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_DELETE_ENTITY;

    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_RX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__, lcId, 0, 0, 0,
            0, 0, __func__, "Deleting Rx UM Rlc Entity");
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseDelTxRxUMEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the DELETE_TX_RX_UM_RLC_ENTITY msg
 *                  1. Parse the Delete TX RX UM RLC Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseDelTxRxUMEntity(
                                        UInt8 *msg_p,
                                        UInt16 *current_p,
                                        RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                      )
{
    UInt8 lcId      = RLC_INVALID_LC_ID;
    UInt8 resp      = RLC_FAILURE;
    UInt16 current  = *current_p;
    UInt16 tagLen   = 0;    

 /*  DELETE_TX_RX_UM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      5
       2. LCID               1 bytes      0 - 10
  */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen != LCID_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_DELETE_ENTITY;

    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__, lcId, 0, 0, 0,
            0, 0, __func__, "");
            
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseDelAMEntity
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the DELETE_TX_RX_AM_RLC_ENTTIY msg
 *                  1. Parse the Delete TX RX AM RLC Entity Msg
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseDelAMEntity( UInt8 *msg_p,
                                    UInt16 *current_p,
                                    RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                  )
{
    UInt8 lcId      = RLC_INVALID_LC_ID;
    UInt8 resp      = RLC_FAILURE;
    UInt16 current  = (*current_p);
    UInt16 tagLen   = 0;

 /*  DELETE_TX_RX_AM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      5
       2. LCID               1 bytes      0 - 10
*/

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if ( tagLen != LCID_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_AM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }
    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if (RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_AM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    rlcEntityCfgInfo_p->lcId = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_AM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode = RLC_DELETE_ENTITY;

    (*current_p) = current;
    
    LOG_RLC_MSG ( LTE_RLC_PARSE_DEL_TX_RX_AM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__, lcId, 0, 0, 0,
            0, 0, __func__, "");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseReconfigTxUMEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the RECONFIG_TX_UM_RLC_ENTITY
 *                  1. Parse the Reconfigure TX UM RLC Entity Msg 
 *                  2. Validate the parameters
 *                  3. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseReconfigTxUMEntity(  UInt8 *msg_p,
                                            UInt16 *current_p,
                                            RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                          )
{

    UInt8 lcId      = RLC_INVALID_LC_ID;
    UInt8 resp      = RLC_FAILURE;
    UInt16 current  = *current_p;
    UInt16 tagLen   = 0;
#ifndef UE_SIM_TESTING
    UInt16 reconfigTxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
#endif

    /*  RECONFIG_TX_UM_RLC_ENTITY API

        API Element           Bytes        Range
        1. TagLen             2 bytes      5
        2. LCID               1 bytes      0 - 10
     */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen < LCID_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_TX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_TX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }
    
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    reconfigTxUmLength = tagLen - RECONFIG_TX_UM_RLC_ENTITY_MIN_LENGTH;
    while(reconfigTxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        reconfigTxUmLength = reconfigTxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_RECONFIG_ENTITY;
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.
        rlcQueueSize  = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umUniDirectionalTX.txUmRlc.
        bitRateTx = txBitRate;
#endif
    /** Rel 4.0 changes End **/

    (*current_p) = current;
    
    LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_TX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, 0, 0, 0, 0, __func__,
            "Reconfigure UM TX Entity");
    return RLC_SUCCESS;

}

/****************************************************************************
 * Function Name  : parseReconfigRxUMEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the msg for RECONFIG_RX_UM_RLC_ENTITY
 *                  1. Parse RECONFIG_RX_UM_RLC_ENTITY
 *                  2. Validate the Parameters 
 *                  3. if validation fails then return the errorCode 
 *                  4. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseReconfigRxUMEntity( UInt8 *msg_p,
                                           UInt16 *current_p,
                                           RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                         )
{

    UInt8 lcId     = RLC_INVALID_LC_ID;
    UInt8 resp     = RLC_FAILURE; 
    UInt16 current = *current_p;
    UInt16 tagLen  = 0; 
    UInt16 tReordering = 0;
#ifndef UE_SIM_TESTING
    UInt16 reconfigRxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rxBitRate        = 0;
#endif

 /*  RECONFIG_RX_UM_RLC_ENTITY API

       API Element           Bytes        Range
       1. TagLen             2 bytes      5
       2. LCID               1 bytes      0 - 10
       3. tReordering        2 bytes      0 ms - 200 ms
  */
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen < UM_ENTITY_RECNFIG_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
        return resp;
    }

    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if(RLC_SUCCESS != validateTReordering(tReordering))
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_UM_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, tReordering, 0, 0, 0, __func__,
                "");
        return RLC_SYNTAX_ERROR;
    }
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    reconfigRxUmLength = tagLen - RECONFIG_RX_UM_RLC_ENTITY_MIN_LENGTH;
    while(reconfigRxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        reconfigRxUmLength = reconfigRxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_RECONFIG_ENTITY;
    /* + Coverity 32388 */
    rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc.
        tReordering                = (TReordering ) tReordering;
    /* - Coverity 32388 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umUniDirectionalRx.rxUmRlc.
        bitRateRx = rxBitRate;
#endif
    /** Rel 4.0 Changes End **/

    (*current_p) = current;
    
    LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, 0, 0, 0, 0, __func__,
            "Reconfigure UM RX Entity");
            
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseReconfigRxTxUMEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the msg for RECONFIG_TX_RX_UM_RLC_ENTITY
 *                  1. Parse RECONFIG_TX_RX_UM_RLC_ENTITY
 *                  2. Validate the Parameters 
 *                  3. if validation fails then return the errorCode 
 *                  4. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseReconfigRxTxUMEntity( UInt8 *msg_p,
                                             UInt16 *current_p,
                                             RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                           )
{
    UInt8 lcId     = RLC_INVALID_LC_ID;
    UInt8 resp     = RLC_FAILURE;
    UInt16 current = *current_p;
    UInt16 tagLen  = 0;
    UInt16 tReordering = 0;
#ifndef UE_SIM_TESTING
    UInt16 reconfigTxRxUmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
    UInt32 rxBitRate        = 0;
#endif

    /*  RECONFIG_TX_RX_UM_RLC_ENTITY API

        API Element           Bytes        Range
        1. TagLen             2 bytes      5
        2. LCID               1 bytes      0 - 10
        3. tReordering        2 bytes      0 ms - 200 ms
     */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if ( tagLen < UM_ENTITY_RECNFIG_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }
    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");
                
        return resp;
    }

    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if(RLC_SUCCESS != validateTReordering(tReordering))
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, tReordering, 0, 0, 0, __func__,
                "");
                
        return RLC_SYNTAX_ERROR;
    }
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    reconfigTxRxUmLength = tagLen - RECONFIG_TX_RX_UM_RLC_ENTITY_MIN_LENGTH;
    while(reconfigTxRxUmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        reconfigTxRxUmLength = reconfigTxRxUmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_UM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_RECONFIG_ENTITY;
    /* + Coverity 32388 */
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
        tReordering                = (TReordering ) tReordering;
    /* - Coverity 32388 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        rlcQueueSize = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.umBiDirectional.txUmRlc.
        bitRateTx = txBitRate;
    rlcEntityCfgInfo_p->configParam.umBiDirectional.rxUmRlc.
        bitRateRx = rxBitRate;
#endif
    /** Rel 4.0 Changes Start **/

    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_RX_TX_UM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, 0, 0, 0, 0, __func__,
            "");
            
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseReconfigAMEntity 
 * Inputs         : msg_p - Pointer to the data received from RRC,
 *                  current_p - indicate the pointer to the msg unread,
 *                  rlcEntityCfgInfo_p - Pointer to RlcEntityCfgInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/ErrorCode 
 * Description    : This API is to parse the msg for RECONFIG_TX_RX_AM_RLC_ENTTIY
 *                  1. Parse RECONFIG_TX_RX_AM_RLC_ENTTIY
 *                  2. Validate the Parameters 
 *                  3. if validation fails then return the errorCode 
 *                  4. Fill the RlcEntityCfgInfo structure.
 ****************************************************************************/
inline static int parseReconfigAMEntity( UInt8 *msg_p,
                                         UInt16 *current_p,
                                         RlcEntityCfgInfo *rlcEntityCfgInfo_p
                                       )
{
    UInt8 lcId              = RLC_INVALID_LC_ID;
    UInt8 resp              = RLC_FAILURE;
    UInt16 current          = *current_p;
    UInt16 tagLen           = 0;
    UInt16 tPollRetransmit  = 0;
    UInt16 pollPDU          = 0;
    UInt16 pollByte         = 0;
    UInt16 maxRetxThreshold = 0;
    UInt16 tReordering      = 0;
    UInt16 tStatusProhibit  = 0;
#ifndef UE_SIM_TESTING
    UInt16 reconfigTxRxAmLength = 0;
    UInt16 subTag           = 0; 
    UInt16 subTagLen        = 0;
    UInt32 qci              = 0;
    UInt32 rlcQueueSize     = 0;
    UInt32 txBitRate        = 0;
    UInt32 rxBitRate        = 0;
#endif

    /*  RECONFIG_TX_RX_AM_RLC_ENTITY API

        API Element              Bytes        Range
        1. TagLen                2 bytes      17
        2. LCID                  1 bytes      0 - 10
        3. tPollRetransmission   2 bytes      5 ms - 500 ms
        4. pollPDU               2 bytes      4, 8, 16, 32, 64, 128, 256, Infinity
        5. pollByte              2 bytes      25 kBytes - 3000 kBytes
        6. maxReTxThrshld        2 bytes      1 - 32
        7. tReordering           2 bytes      0 ms - 200 ms
        8. tStatusProhibit       2 bytes      0 ms - 500 ms
     */

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if ( tagLen < AM_ENTITY_RECNFIG_TAG_LENGTH)
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, 0, 0, 0, 0, 0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }

    lcId = *(msg_p + current);
    current++;
    /* validating LCID */
    resp = IS_LCID_INVALID(lcId);
    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_1, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, 0, 0, 0, 0, __func__,
                "");

        return resp;
    }

    tPollRetransmit = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tPollRetransmit  */
    if ( RLC_SUCCESS != validatePollRetransmit(tPollRetransmit)) 
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_2, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, tPollRetransmit, 0, 0, 0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }

    pollPDU = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating pollPDU  */
    resp = validatePollPDU(pollPDU);

    if ( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_3, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, tPollRetransmit, pollPDU, 0, 0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }
    pollByte = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating PollByte  */
    resp = validatePollByte(pollByte);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_4, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, pollPDU, pollByte, 0,0,
                __func__,"");

        return RLC_SYNTAX_ERROR;
    }

    maxRetxThreshold = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating maxRetxThreshold */
    resp = validateMaxReTxThrshld(maxRetxThreshold);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_5, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId, maxRetxThreshold,0,0,0,
                __func__,"");

        return RLC_SYNTAX_ERROR;
    }

    tReordering = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tReordering */
    if(RLC_SUCCESS != validateTReordering(tReordering))
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_6, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId,tReordering, 0,0,0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }

    tStatusProhibit = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    /* validating tStatusProhibit value */
    resp = validateStatusProhibit(tStatusProhibit);
    if( RLC_SUCCESS != resp )
    {
        LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ERR_7, LOGWARNING, 
                RLC_RRC, globalTTITickCount_g, __LINE__,
                tagLen, lcId,tReordering, tStatusProhibit, 0, 0, __func__,
                "");

        return RLC_SYNTAX_ERROR;
    }
    /** Rel 4.0 changes Start **/
#ifndef UE_SIM_TESTING
    reconfigTxRxAmLength = tagLen - RECONFIG_TX_RX_AM_RLC_ENTITY_MIN_LENGTH;
    while(reconfigTxRxAmLength > 0)
    {
        subTag = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        subTagLen = LTE_GET_U16BIT(msg_p + current);
        current = current + 2;
        reconfigTxRxAmLength = reconfigTxRxAmLength - subTagLen;
        switch(subTag)
        {
            case RLC_QCI_INFO:
                {
                    if(subTagLen != QCI_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    qci = *(msg_p + current);
                    current += 1;
                }
                break;
            case RLC_RX_BIT_RATE_INFO:
                {
                    if(subTagLen != RX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rxBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_TX_BIT_RATE_INFO:
                {
                    if(subTagLen != TX_BIT_RATE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    txBitRate = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            case RLC_QUEUE_SIZE_INFO:
                {
                    if(subTagLen != QUEUE_SIZE_INFO_TAG_LENGTH)
                    {
                        return RLC_SYNTAX_ERROR;
                    }
                    rlcQueueSize = LTE_GET_U32BIT(msg_p + current);
                    current += 4;
                }
                break;
            default:
                break;
        }
    }
#endif
    /** Rel 4.0 Changes End **/

    rlcEntityCfgInfo_p->lcId       = lcId;
    rlcEntityCfgInfo_p->entityMode = RLC_ENTITY_AM;
    rlcEntityCfgInfo_p->entityType = RLC_TX_RX_ENTITY;
    rlcEntityCfgInfo_p->opCode     = RLC_RECONFIG_ENTITY;

    TxAmRlc *txAmRlc_p = 
        &(rlcEntityCfgInfo_p->configParam.am.txAmRlc);

    RxAmRlc *rxAmRlc_p = 
        &(rlcEntityCfgInfo_p->configParam.am.rxAmRlc);
    /* + Coverity 32397, 32396, 32395, 32394, 32390, 32391 */
    txAmRlc_p->tPollRetransmit  = (TPollRetransmit ) tPollRetransmit;
    txAmRlc_p->pollPDU          = (PollPDU ) pollPDU;
    txAmRlc_p->pollByte         = (PollByte ) pollByte * 1000; //fix bug 114863 by dbc
    txAmRlc_p->maxRetxThreshold = (MaxRetxThreshold ) maxRetxThreshold;
    rxAmRlc_p->tReordering      = (TReordering ) tReordering;
    rxAmRlc_p->tStatusProhibit  = (TStatusProhibit ) tStatusProhibit;
    /* - Coverity 32397, 32396, 32395, 32394, 32390, 32391 */
    /** Rel 4.0 Changes Start **/
#ifndef UE_SIM_TESTING
    rlcEntityCfgInfo_p->configParam.am.txAmRlc.rlcQueueSize
        = rlcQueueSize;
    rlcEntityCfgInfo_p->rlcQciInfo.qci = qci;
    rlcEntityCfgInfo_p->configParam.am.txAmRlc.bitRateTx = txBitRate;
    rlcEntityCfgInfo_p->configParam.am.rxAmRlc.bitRateRx = rxBitRate;
#endif
    /** Rel 4.0 Changes Start **/

    (*current_p) = current;

    LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ENTITY, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__,
            tagLen, lcId, tPollRetransmit, pollPDU, 0,0,
            __func__,"");

    LOG_RLC_MSG ( LTE_RLC_PARSE_RECONFIG_AM_ENTITY_1, LOGINFO, 
            RLC_RRC, globalTTITickCount_g, __LINE__, 
            lcId, pollByte, maxRetxThreshold,tReordering, tStatusProhibit, 0, 
            __func__,"");

    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseRLCUeEntityInfo 
 * Inputs         : msg_p - points to the msg read from RRC,
 *                  rlcEntityCfgInfo_p - pointer of RlcEntityCfgInfo,
 *                  current_p - points to the current position from where the
 *                              msg to be read,
 *                  msgLen_p - points to msg length which is unread,
 *                  ueIndex - Index of UE,
 *                  ueBitRateFlag_p - Pointer to UE Bit Rate Flag,
 *                  rlcConfigReq_p - Pointer to RlcConfigReq
 * Outputs        : None
 * Returns        : RLC_SUCCESS if parse successfully otherwise value which
 *                  indicates error message.
 * Description    : This API is to parse the msg and prepare the config params
 *                  which later is used to create or configer the entity.
 *                  1. CREATE_TX_UM_RLC_ENTITY  
 *                  2. CREATE_RX_UM_RLC_ENTITY
 *                  3. CREATE_TX_RX_UM_RLC_ENTITY
 *                  4. CREATE_TX_RX_AM_RLC_ENTITY
 *                  5. DELETE_TX_UM_RLC_ENTITY
 *                  6. DELETE_RX_UM_RLC_ENTITY
 *                  7. DELETE_TX_RX_UM_RLC_ENTITY
 *                  8. DELETE_TX_RX_AM_RLC_ENTITY
 *                  9. RECONFIG_TX_UM_RLC_ENTITY
 *                 10. RECONFIG_RX_UM_RLC_ENTITY
 *                 11. RECONFIG_TX_RX_UM_RLC_ENTITY
 *                 12. RECONFIG_TX_RX_AM_RLC_ENTITY
 *                 13. ENTITY_LCID.
 ****************************************************************************/

inline STATIC UInt32 parseRLCUeEntityInfo( UInt8* msg_p,
        RlcEntityCfgInfo *rlcEntityCfgInfo_p,
        UInt16 *current_p, 
        UInt16 *msgLen_p,
        UInt16 ueIndex,
        UInt32 *ueBitRateFlag_p,
        RlcConfigReq *rlcConfigReq_p,
        /* SPR 12917 fix start */   
        InternalCellIndex cellIndex
        /* SPR 12917 fix end */   
        )
{
    UInt16 current   = *current_p;
    UInt16 tag       = 0;
    UInt16 tagLen    = 0;
    UInt16 errorCode = INVALID_ERR_CODE;

    tag = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    tagLen = LTE_GET_U16BIT(msg_p + current);

    if ( tagLen >  *msgLen_p )
    {
        LOG_RLC_MSG ( LTE_RLC_RRC_PARSE_UE_ENTITY_INFO_ERR, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
                0, 0, 0, __func__, "");

        return RLC_SYNTAX_ERROR;
    }   

    switch ( tag )
    {
        case CREATE_TX_UM_RLC_ENTITY:
        {
            errorCode =  
                parseCreateTxUMRlcEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case CREATE_RX_UM_RLC_ENTITY:
        {
            errorCode = 
                parseCreateRxUMRlcEntity ( msg_p, &current, rlcEntityCfgInfo_p); 
        }
        break;
        case CREATE_TX_RX_UM_RLC_ENTITY:
        {
            errorCode = 
                parseCreateTxRxUMRlcEntity ( msg_p, &current, rlcEntityCfgInfo_p); 
        }
        break;
        case CREATE_TX_RX_AM_RLC_ENTITY:
        {
            errorCode = 
                parseCreateRxTxAMEntity(msg_p, &current, rlcEntityCfgInfo_p);   
        }
        break;
        case DELETE_TX_UM_RLC_ENTITY:
        {
            errorCode = 
                parseDelTxUMEntity(msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case DELETE_RX_UM_RLC_ENTITY:
        {
            errorCode = 
                parseDelRxUMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case DELETE_TX_RX_UM_RLC_ENTITY:
        {
            errorCode = 
                parseDelTxRxUMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case DELETE_TX_RX_AM_RLC_ENTITY:
        {
            errorCode = 
                parseDelAMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case RECONFIG_TX_UM_RLC_ENTITY: 
        {
            errorCode = 
                parseReconfigTxUMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case RECONFIG_RX_UM_RLC_ENTITY: 
        {
            errorCode = 
                parseReconfigRxUMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case RECONFIG_TX_RX_UM_RLC_ENTITY: 
        {
            errorCode = 
                parseReconfigRxTxUMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
        case RECONFIG_TX_RX_AM_RLC_ENTITY: 
        {
            errorCode = 
                parseReconfigAMEntity( msg_p, &current, rlcEntityCfgInfo_p);
        }
        break;
#ifndef UE_SIM_TESTING
        case RLC_UE_BIT_RATE_INFO:
        {
            errorCode = parseBitRateInfo(msg_p, &current, ueIndex,
                    ueBitRateFlag_p);
        }
        break;
        case RLC_SPS_CONFIG:
        {
            errorCode = parseSpsConfigInfo( msg_p, &current, rlcConfigReq_p,
                    /* SPR 12917 fix start */   
                    cellIndex );
            /* SPR 12917 fix end */   
            break;
        }
#endif

        default:    
        LOG_RLC_MSG ( LTE_RLC_PARSE_UE_ENTITY_INFO, LOGWARNING, RLC_RRC, 
                globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
                0, 0, 0, __func__, "");
        errorCode = INVALID_RRC_MSG_ID;     
        break;
    }
    *msgLen_p -= tagLen;
    *current_p = current;

    LOG_RLC_MSG ( LTE_RLC_PARSE_UE_ENTITY_INFO_1, LOGWARNING, RLC_RRC, 
            globalTTITickCount_g, __LINE__, tag, tagLen, *msgLen_p,
            errorCode, 0, 0, __func__, "");
    return errorCode;
} /* end of parseRLCUeEntityInfo */    

/****************************************************************************
 * Function Name  : rlcSendMsgToRrc
 * Inputs         : ui_CnfMsgBuf_p - Pointer to UI cng message buffer,
 *                  ui16_msgLen - UI16 Message length
 * Outputs        : None
 * Returns        : None
 * Description    : This function sends th confirmation message to RRC 
 ****************************************************************************/
void  rlcSendMsgToRrc(UInt8 *ui_CnfMsgBuf_p,
        UInt16 ui16_msgLen )
{
  if (ui16_msgLen != sendto_wrapper(writeSockFD_g, (UInt8 *)ui_CnfMsgBuf_p, ui16_msgLen, 0,
                   &(writeAddr_g), sizeof(writeAddr_g)))
  {
      ALARM_MSG (RLC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
  }
}




