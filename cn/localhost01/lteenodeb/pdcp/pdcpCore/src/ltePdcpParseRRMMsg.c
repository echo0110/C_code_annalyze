/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ***************************************************************************
 *
 *  $Id: lteMacParseRrmMsg.c
 *
 ****************************************************************************
 *
 *  File Description : This file contains function for parsing RRM messages. 
 *
 ************************************************************************/

/**************************************************************************
 * Standard Library Includes
 *************************************************************************/

/**************************************************************************
 * Project Includes
 *************************************************************************/
#include "ltePdcpRRMInterface.h"
#include "lteLayer2CommanTypes.h"
#include "ltePdcpContext.h"
#include "ltePdcpErrors.h"
#include "logging.h"
#include "ltePdcpRRMUtils.h"
#include "lteCommonStatsManager.h"
#include "ltePdcpOamSap.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpStatistic.h"
/**************************************************************************
 * Global types
 *************************************************************************/
/*SPR 15909 Fix start*/
extern tickType_t pdcpCurrentTime_g;
/*SPR 15909 Fix end*/
#ifndef UE_SIM_TESTING
#ifdef KPI_STATS
/* + stats_review_comments_ipc */
extern void modCmdGetKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId );
/* - stats_review_comments_ipc */
#endif
#endif
/**************************************************************************
 * Private types
 *************************************************************************/
STATIC UInt8 validateEcnCeParameter(RrmPdcpDrbEcnCeReq *rrmPdcpDrbEcnCeReq_p);
STATIC UInt8 validateEcnConfigParameter(RrmPdcpEcnConfigParamReq *rrmPdcpEcnConfigParam_p);


/****************************************************************************
 * Function Name  : sendEcnDrbUeIndicationToRRM
 * Inputs         : ecnCeInd -
 * Returns        : None
 * Description    : This API sends the Drb ue indication to RRM 
 ****************************************************************************/
void sendEcnDrbUeIndicationToRRM( RrmPdcpDrbEcnCeInd ecnCeInd)
{
    UInt8                *respBuf_p      = PNULL;
    UInt16               transactionId  = L2_RRM_IND_TRANS_ID;
    LP_TlvMessage        cnfMsg_p       = PNULL;
    RrmPdcpDrbEcnCeInd   *ecnInd_p      = PNULL;
    UInt16               msgLength      = 0;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    InternalCellIndex    cellIndex = INVALID_CELL_INDEX;
#endif
    /* - Layer2 NON CA Changes */

    respBuf_p = (UInt8 *)getMemFromPool (L2_RRM_API_HEADER_LEN +
            RRM_PDCP_UE_DRB_ECN_CE_IND_LENGTH, PNULL);
    if( PNULL == respBuf_p)
    {
        ltePanic ("getMemPool error in %s", __func__);
        return;
    }    
    cnfMsg_p = (LP_TlvMessage)&respBuf_p[0];
    ecnInd_p = ( RrmPdcpDrbEcnCeInd *)cnfMsg_p->data;
    *ecnInd_p = ecnCeInd;

    msgLength = L2_RRM_API_HEADER_LEN + RRM_PDCP_UE_DRB_ECN_CE_IND_LENGTH;

    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellIndex = pdcpContext.ueContexts[ecnInd_p->ueIndex]->cellIndex;
    /* SPR 21039 fix start */
#endif
    ecnInd_p->ueIndex = getRrcUeIdxFromUeIdx(ecnInd_p->ueIndex);
    /* SPR 21039 fix end */


    /* SPR 21583 fix start */
    tlvPrepareMessage( cnfMsg_p, transactionId, RRM_MODULE_ID,
        RRM_PDCP_UE_DRB_ECN_CE_IND, RRM_PDCP_UE_DRB_ECN_CE_IND_LENGTH
    /* SPR 21583 fix end */
#ifndef UE_SIM_TESTING
            ,cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    LOG_PDCP_MSG( PDCP_DRB_ECN_CN_MSG, LOGDEBUG, PDCP_RRM_INT,
            pdcpCurrentTime_g, __LINE__, ecnCeInd.ueIndex,
            ecnCeInd.lcId, 0,0,0,0,__func__,"");

    /* Send the confimration back to RRM */
    if(PDCP_SEND_ERROR ==  
            sendPdcpMsgToRRM( respBuf_p, msgLength
                )
      )
    {
        LOG_PDCP_MSG( PDCP_RRM_MSG_SENT_FAIL, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
                "PDCP_RRM_MSG_SENT_FAIL" );
    }
    freeMemPool( respBuf_p );
}

/****************************************************************************
 * Function Name  : validateEcnCeParameter
 * Inputs         : RrmPdcpDrbEcnCeReq
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : This API validate the parameter send in RrmPdcpDrbEcnCeReq
 ****************************************************************************/
STATIC UInt8 validateEcnCeParameter(RrmPdcpDrbEcnCeReq *rrmPdcpDrbEcnCeReq_p)
{
    UInt8 retVal = PDCP_SUCCESS;
    UInt8 idx = 0;
    /*CSR 40799 chg start*/
     /* SPR 15553 fix start */
    if( (rrmPdcpDrbEcnCeReq_p->ueIndex >= MAX_UE_SUPPORTED) || 
        /* SPR 15553 fix end */
            (rrmPdcpDrbEcnCeReq_p->numOfLc > MAX_CONFIGURED_DRB))
    {/*CSR 40799 chg end*/
        LOG_PDCP_MSG( PDCP_EXCEED_MAX_UE, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, rrmPdcpDrbEcnCeReq_p->ueIndex,
                rrmPdcpDrbEcnCeReq_p->numOfLc , 0,0,0,0, __func__,
                "PDCP_RRM_DRB_ECN_CE_REQ" );
        return PDCP_FAIL;
    }

    for (; idx < rrmPdcpDrbEcnCeReq_p->numOfLc; idx ++)
    {
        if( (rrmPdcpDrbEcnCeReq_p->ceLc[idx].lcId < LTE_DRB_LC_ID_LOW_BOUND) ||
                (rrmPdcpDrbEcnCeReq_p->ceLc[idx].lcId > LTE_DRB_LC_ID_HIGH_BOUND ))
        {
            LOG_PDCP_MSG( PDCP_EXCEED_MAX_LC, LOGWARNING, PDCP_RRM_INT,
                    pdcpCurrentTime_g, __LINE__, rrmPdcpDrbEcnCeReq_p->ceLc[idx].lcId,
                    0,0,0,0,0, __func__, "PDCP_RRM_DRB_ECN_CE_REQ" );
            retVal = PDCP_FAIL;
            break;
        }
        else if( ( rrmPdcpDrbEcnCeReq_p->ceLc[idx].direction > PDCP_ENTITY_RB_BOTH) ||
                ( rrmPdcpDrbEcnCeReq_p->ceLc[idx].congestionStatus > ECN_APPLY_CONGESTION))

        {
            LOG_PDCP_MSG( PDCP_INVALID_CONGESTION_DIRECTION, LOGWARNING, PDCP_RRM_INT,
                    pdcpCurrentTime_g, __LINE__, rrmPdcpDrbEcnCeReq_p->ceLc[idx].direction,
                    rrmPdcpDrbEcnCeReq_p->ceLc[idx].congestionStatus, 0,0,0,0, __func__,
                    "PDCP_RRM_DRB_ECN_CE_REQ" );
            retVal = PDCP_FAIL;
            break;
        }
    }

    return retVal; 
}

/****************************************************************************
 * Function Name  : validateEcnConfigParameter
 * Inputs         : RrmPdcpDrbEcnCeReq
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : This API validate the parameter send in RrmPdcpEcnConfigParam
 ****************************************************************************/
STATIC UInt8 validateEcnConfigParameter(RrmPdcpEcnConfigParamReq *rrmPdcpEcnConfigParam_p)
{
    UInt8 retVal = PDCP_SUCCESS;

    if( rrmPdcpEcnConfigParam_p->ecnFeatureStatus > ECN_APPLIED_IN_BOTH_UL_DL ||
            rrmPdcpEcnConfigParam_p->ecnTrigger > ECN_BOTH_TRIGGERS_APPLIED ) 
    {
        LOG_PDCP_MSG( PDCP_INVALID_ECN_FEATURE, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, rrmPdcpEcnConfigParam_p->ecnFeatureStatus,
                rrmPdcpEcnConfigParam_p->ecnTrigger,0,0,0,0, __func__,
                "PDCP_RRM_ECN_CONFIG_REQ" );
        retVal = PDCP_FAIL; 
    }
    else if( (!( rrmPdcpEcnConfigParam_p->lowWaterMark >= WATERMARK_START_RANGE  &&
                    rrmPdcpEcnConfigParam_p->lowWaterMark <= WATERMARK_END_RANGE ))  ||
            (!( rrmPdcpEcnConfigParam_p->highWaterMark >= WATERMARK_START_RANGE &&
                rrmPdcpEcnConfigParam_p->highWaterMark <= WATERMARK_END_RANGE )))
    {
        LOG_PDCP_MSG( PDCP_INVALID_WATERMARK_VALUE, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, rrmPdcpEcnConfigParam_p->lowWaterMark,
                rrmPdcpEcnConfigParam_p->highWaterMark,0,0,0,0, __func__,
                "PDCP_RRM_ECN_CONFIG_REQ" );
        retVal = PDCP_FAIL; 
    }
    else if (rrmPdcpEcnConfigParam_p->lowWaterMark > rrmPdcpEcnConfigParam_p->highWaterMark)
    {
        LOG_PDCP_MSG( PDCP_INVALID_WATERMARK_VALUE, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, rrmPdcpEcnConfigParam_p->lowWaterMark,
                rrmPdcpEcnConfigParam_p->highWaterMark,0,0,0,0, __func__,
                "PDCP_RRM_ECN_CONFIG_REQ" );
        retVal = PDCP_FAIL; 
    }
    return retVal;
}

/**************************************************************************
 * Function Name  : parsePdcpRrmMsg
 * Inputs         : msgId - Message Id received in the buffer header.
 *                  bytesRead - lenght of the messasge.
 * Outputs        : None
 * Returns        : PDCP_SUCCESS or PDCP_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required.
 ****************************************************************************/
/* + Layer2 NON CA Changes */
UInt8 parsePdcpRrmMsg(L2RrmMsg *msg_p, SInt16 bytesRead)
    /* + Layer2 NON CA Changes */
{
    UInt8 *respBuf_p = PNULL;
    LP_TlvMessage cnfMsg_p = PNULL;
    UInt16 retVal              = PDCP_SUCCESS;
    UInt16 msgLength           = 0;
    RrmPdcpDrbEcnCeReq        *rrmPdcpDrbEcnCeReq_p                 = PNULL;
    RrmPdcpDrbEcnCeCnf        *rrmPdcpDrbEcnCeCnf_p                 = PNULL;
    RrmPdcpEcnConfigParamReq  *rrmPdcpEcnConfigParam_p              = PNULL;
    RrmPdcpEcnConfigParamCnf  *rrmPdcpEcnConfigParamCnf_p           = PNULL;
    /* + Layer2 NON CA Changes */
    UInt16      msgId = msg_p->header.apiId;
    UInt16      transactionId = msg_p->header.transactionId;
    UInt8       *buffer = msg_p->data;
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( 
            msg_p->header.cellIndex );
#else
    InternalCellIndex cellIndex = 0; 
#endif
    /* - Layer2 NON CA Changes */

    /* coverity_60783 Fix Start */
    if(INVALID_CELL_INDEX == cellIndex)
    {
        return PDCP_FAIL;
    }
    /* coverity_60783 Fix End */

    switch(msgId)
    {
        case RRM_PDCP_UE_DRB_ECN_CE_REQ:
        {  
            /* Check validation */
            if(RRM_PDCP_UE_DRB_ECN_CE_REQ_LENGTH != bytesRead)
            {
                LOG_PDCP_MSG( PDCP_RRM_INVALID_MSG_LENGTH, LOGWARNING, PDCP_RRM_INT,
                        pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
                        "PDCP_RRM_INVALID_BYTES_READ " );
                return PDCP_RRM_INVALID_API_LENGTH;
            }
            rrmPdcpDrbEcnCeReq_p = (RrmPdcpDrbEcnCeReq *)buffer;

            respBuf_p = (UInt8 *)getMemFromPool (L2_RRM_API_HEADER_LEN + 
                    RRM_PDCP_UE_DRB_ECN_CE_CNF_LENGTH, PNULL);
            if( PNULL == respBuf_p)
            {
                LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRM_INT, 
                        pdcpCurrentTime_g, __LINE__, L2_RRM_API_HEADER_LEN + RRM_PDCP_UE_DRB_ECN_CE_CNF_LENGTH,
                        0,0,0,0,0, __func__,"");
                ltePanic ("getMemPool error in %s", __func__);
                return PDCP_FAIL;
            }    
            cnfMsg_p = (LP_TlvMessage )&respBuf_p[0];

            rrmPdcpDrbEcnCeCnf_p = (RrmPdcpDrbEcnCeCnf *)cnfMsg_p->data;
            rrmPdcpDrbEcnCeCnf_p->ueIndex = rrmPdcpDrbEcnCeReq_p->ueIndex;
            rrmPdcpDrbEcnCeCnf_p->numOfLc = rrmPdcpDrbEcnCeReq_p->numOfLc;
            memCpy(rrmPdcpDrbEcnCeCnf_p->ceLc, rrmPdcpDrbEcnCeReq_p->ceLc, 
                    (sizeof(RrmPdcpCeLcListReq)) * PDCP_MAX_DRB);

            if(PDCP_FAIL == validateEcnCeParameter(rrmPdcpDrbEcnCeReq_p))
            {
                retVal = PDCP_FAIL;
            }
            else
            {    
                retVal = pdcpHandleCongestionNotificationStatus(rrmPdcpDrbEcnCeReq_p, rrmPdcpDrbEcnCeCnf_p);
            }

            /* Updating the response code in CNF msg*/
            rrmPdcpDrbEcnCeCnf_p->respCode = retVal;

            /* Fill Msg Len in the API header */
            msgLength = L2_RRM_API_HEADER_LEN + RRM_PDCP_UE_DRB_ECN_CE_CNF_LENGTH;

            /* + Layer2 NON CA Changes */
            /* SPR 21583 fix start */
            tlvPrepareMessage(cnfMsg_p,transactionId,RRM_MODULE_ID,RRM_PDCP_UE_DRB_ECN_CE_CNF ,RRM_PDCP_UE_DRB_ECN_CE_CNF_LENGTH
            /* SPR 21583 fix end */
#ifndef UE_SIM_TESTING
                    ,cellIndex
#endif
                    );

            /* - Layer2 NON CA Changes */

            LOG_PDCP_MSG( PDCP_DRB_ECN_CN_MSG, LOGDEBUG, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__, rrmPdcpDrbEcnCeReq_p->ueIndex,
                    rrmPdcpDrbEcnCeReq_p->numOfLc, 0,0,0,0,__func__,"");

            /* Send the confimration back to RRM */
            if(PDCP_SEND_ERROR == 
                    sendPdcpMsgToRRM( respBuf_p, msgLength
                        )
              )
            {
                LOG_PDCP_MSG( PDCP_RRM_MSG_SENT_FAIL, LOGWARNING, PDCP_RRM_INT,
                        pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
                        "PDCP_RRM_ECN_CN_MSG_SENT_FAIL" );
            }
            freeMemPool( respBuf_p );
            break;
        }
        case RRM_PDCP_CONFIG_PARAMS_REQ:
        {  
            /* Check validation */
            if(RRM_PDCP_CONFIG_PARAMS_REQ_LENGTH != bytesRead)
            {
                LOG_PDCP_MSG( PDCP_RRM_INVALID_MSG_LENGTH, LOGWARNING, PDCP_RRM_INT,
                        pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
                        "PDCP_RRM_INVALID_BYTES_READ" );
                return PDCP_RRM_INVALID_API_LENGTH;
            }
            rrmPdcpEcnConfigParam_p = (RrmPdcpEcnConfigParamReq *)buffer;

            /* Prepare a response and send it to RRM */
            respBuf_p = (UInt8 *)getMemFromPool (L2_RRM_API_HEADER_LEN + 
                    RRM_PDCP_CONFIG_PARAMS_CNF_LENGTH, PNULL);
            if( PNULL == respBuf_p)
            {
                LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRM_INT, 
                        pdcpCurrentTime_g, __LINE__, L2_RRM_API_HEADER_LEN + RRM_PDCP_CONFIG_PARAMS_CNF_LENGTH,
                        0,0,0,0,0, __func__,"");
                ltePanic ("getMemPool error in %s", __func__);
                return PDCP_FAIL;
            }    
            cnfMsg_p = (LP_TlvMessage )&respBuf_p[0];
            rrmPdcpEcnConfigParamCnf_p = (RrmPdcpEcnConfigParamCnf *)cnfMsg_p->data;

            if(PDCP_FAIL == validateEcnConfigParameter(rrmPdcpEcnConfigParam_p))
            {
                retVal = PDCP_FAIL;
            }
            else
            {
                /* CA changes start */
                pdcpSetEcnStatus(rrmPdcpEcnConfigParam_p, cellIndex);
                /* CA changes end */
            }

            rrmPdcpEcnConfigParamCnf_p->respCode = retVal;

            /* Fill Msg Len in the API header */
            msgLength = L2_RRM_API_HEADER_LEN + RRM_PDCP_CONFIG_PARAMS_CNF_LENGTH;

            /* + Layer2 NON CA Changes */
            /* SPR 21583 fix start */
            tlvPrepareMessage(cnfMsg_p,transactionId,RRM_MODULE_ID,RRM_PDCP_CONFIG_PARAMS_CNF,RRM_PDCP_CONFIG_PARAMS_CNF_LENGTH
            /* SPR 21583 fix end */
#ifndef UE_SIM_TESTING
                    ,cellIndex
#endif
                    );
            /* - Layer2 NON CA Changes */

            /* Send the confimration back to RRM */
            if(PDCP_SEND_ERROR ==  
                    sendPdcpMsgToRRM( respBuf_p, msgLength
                        )
              )
            {
                LOG_PDCP_MSG( PDCP_RRM_MSG_SENT_FAIL, LOGWARNING, PDCP_RRM_INT,
                        pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
                        "PDCP_RRM_CONFIG_PARAM_MSG_SENT_FAIL " );
            }
            freeMemPool(cnfMsg_p);
            break;
        }

#ifndef UE_SIM_TESTING
#ifdef KPI_STATS            
        case RRM_PDCP_CONFIGURE_KPI_STATS_REQ:
        {
            modCmdConfigureKpiStatsReq(  buffer , bytesRead, KPI_CONTAINER_RRM_PDCP);
            break;
        }

        case RRM_PDCP_GET_KPI_STATS_REQ:
        {
            /* + stats_review_comments_ipc */
            modCmdGetKpiStatsReq(  buffer , bytesRead, KPI_CONTAINER_RRM_PDCP );
            /* - stats_review_comments_ipc */
            break;
        }   
        /*+ SPR 17439*/        
#endif
#endif            

        default:

        LOG_PDCP_MSG( PDCP_RRM_INVALID_MSG_ID, LOGWARNING, PDCP_RRM_INT,
                pdcpCurrentTime_g, __LINE__, msgId,0,0,0,0,0, __func__,
                "PDCP_RRM_API_ID " );

    }
    return retVal;
}

/****************************************************************************
 * Function Name  : processPdcpRRMMsg
 * Inputs         : buffer pointer -
 *                  bytesRead -
 * Returns        : None
 * Description    : This API receives message, parse the msgId and call
 *                  the corresponding API to deal with the message.
 ****************************************************************************/
void processPdcpRRMMsg (UInt8 * buffer, SInt16 bytesRead)
{

    L2RrmMsg *  msg_p = PNULL;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    InternalCellIndex cellIndex = INVALID_CELL_INDEX ;
#endif
    /* - Layer2 NON CA Changes */

    /* RRM message received, perform validations */
    if(PDCP_RECV_ERROR == bytesRead)
    {    
	    LOG_PDCP_MSG( PDCP_RRM_MSG_SENT_FAIL, LOGWARNING, PDCP_RRM_INT,
			    pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
			    "PDCP_RRM_MSG_RECV_FAIL " );
	    return;
    }

    if(bytesRead < (SInt32)L2_RRM_API_HEADER_LEN )
    {
	    LOG_PDCP_MSG( PDCP_RRM_INVALID_MSG_LENGTH, LOGWARNING, PDCP_RRM_INT,
			    pdcpCurrentTime_g, __LINE__, 0,0,0,0,0,0, __func__,
			    "PDCP_RRM_INVALID_BYTES_READ " );
	    return;
    }

    /* Parse the buffer header */
    /*SPR 20863 + */        
    msg_p = (L2RrmMsg *)getMemFromPool(bytesRead,NULL);
    if(msg_p == PNULL)
    {
        ltePanic("MemAlloc fail");
            return;
    }
    msg_p->header.transactionId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(msg_p->header.transactionId);

    msg_p->header.srcModuleId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(msg_p->header.srcModuleId);

    msg_p->header.destModuleId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(msg_p->header.destModuleId);

    msg_p->header.apiId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(msg_p->header.apiId);

    msg_p->header.length = LTE_GET_U16BIT(buffer);
    buffer += sizeof(msg_p->header.length);

    msg_p->header.cellIndex = *buffer;
    buffer += sizeof(msg_p->header.cellIndex);
    buffer += sizeof(msg_p->header.reserved1);
/*SPR 21653 changes start*/
    buffer += sizeof(msg_p->header.reserved2);
/*SPR 21653 changes end*/
    if(msg_p->header.length <= L2_RRM_API_HEADER_LEN)
    {
        lteWarning ("Message received with invalid length = [%d].\
                API ID - [%d].\n", msg_p->header.length,
                msg_p->header.apiId);
        if(msg_p != NULL)
        {
            freeMemPool(msg_p);
        }
        return;
    }
    /* SPR 21950 fix start */
    (void)memCpy(msg_p->data,buffer,(msg_p->header.length - L2_RRM_API_HEADER_LEN));
    /* SPR 21950 fix end */
    /*SPR 20863 -*/
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( msg_p->header.cellIndex );
#endif
    if ( (RRM_MODULE_ID == msg_p->header.srcModuleId) 
            && (PDCP_MODULE_ID == msg_p->header.destModuleId) 
            && (bytesRead == msg_p->header.length)
#ifndef UE_SIM_TESTING
            && ( INVALID_CELL_INDEX != cellIndex)
#endif
       )
    {   
        parsePdcpRrmMsg(msg_p,bytesRead - L2_RRM_API_HEADER_LEN);
/* - Layer2 NON CA Changes */
    }
    else
    {
	    LOG_PDCP_MSG( PDCP_RRM_INVALID_MODULE_ID, LOGWARNING, PDCP_RRM_INT,
			    pdcpCurrentTime_g, __LINE__, msg_p->header.srcModuleId, 
                            msg_p->header.destModuleId, 0, 0, 0, 0, __func__,
			    "PDCP_RRM_MSG_PROCESS" );
    }
    /*SPR 20863 +*/
    if(msg_p != NULL)
    {
        freeMemPool(msg_p);
    }
    /*SPR 20863 -*/
}
