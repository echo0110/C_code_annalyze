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
#include "lteMacCellConfMgr.h"
#include "lteMacParseUtil.h"
#include "lteLog.h"
#include "lteMacParseOAMMsg.h"
#include "lteOamInterface.h"


#include "lteMacRRMInterface.h"
#include "lteLayer2CommanTypes.h"
#include "lteMacRRMPeriodicMgr.h"
#include "lteMacRRMIpcPort.h"
#ifdef PERF_STATS
#include "ltePerfStats.h"
#endif

/**************************************************************************
 * Private Definitions
 *************************************************************************/
#define RRM_MAC_CELL_CONFIG_REQ_LENGTH        sizeof(RrmMacCellConfigReq)
#define RRM_MAC_SCHEDULER_RECONFIG_REQ_LENGTH sizeof(RrmMacReconfigSchedulerReq)
#define RRM_MAC_UE_RECONFIG_REQ_LENGTH        sizeof(RrmMacUeReconfigReq)

/* Perf stats RRM-MAC start */
#ifdef PERF_STATS
#define RRM_MAC_CONFIGURE_PERF_STATS_REQ_LENGTH sizeof(RrmMacConfigPerfStatsReq)
#endif
/* Perf stats RRM-MAC end */


#ifdef KPI_STATS
/*RRM KPI STATS*/
/* SPR 5464 */         
#define RRM_MAC_CONFIGURE_KPI_STATS_REQ_LENGTH sizeof(MacConfigureKpiStatsReqParams)
/* SPR 5464 */         
#endif /*KPI_STATS*/
#define RRM_MAC_UE_SINR_TA_REQ_LENGTH sizeof(MacUeSinrTaReq)

/* + E_CID_5.3 */
#define RRM_MAC_E_CID_MEAS_CONFIG_REQ_LENGTH sizeof(RrmMacEcidMeasConfigReq)
#define INVALID_ E_CID_MEAS_REQ_API_LEN 3
/* - E_CID_5.3 */
/**************************************************************************
 * Global types
 *************************************************************************/
#ifdef KPI_STATS
/* + KPI_CA */
extern UInt16 handleMACConfigureKpiStatReq(UInt8 *msgBuf_p,  UInt16 transactionId_p, UInt8 moduleId);
extern void parseMACGetKpiStatReq(UInt8 *msgBuf_p,UInt8 *cnfBuff_p, 
                                  UInt16 *currentBuf_p, UInt8 modCounter, 
								  InternalCellIndex internalCellIndex);
/* 100MS_SUPPORT : start */
/* - KPI_CA */
/* 100MS_SUPPORT : end */
#endif /*KPI_STATS*/

#ifdef PERF_STATS
extern void resetMacPerfContainer(UInt8 resetId);
#endif

/*+ Dyn UEs To Be SchPerTTI +*/
#ifdef PERF_STATS
/* + PERF_CA GBL */
extern UInt16 totalActiveUEsDL_g[MAX_NUM_CELL];
extern UInt16 totalActiveUEsUL_g[MAX_NUM_CELL];
/* - PERF_CA GBL */
#endif
/*+ Dyn UEs To Be SchPerTTI +*/

/* Cyclomatic_complexity_changes_start */
UInt32 parseRrmMacCellConfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);

UInt32 parseRrmMacSchedulerReconfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);

UInt32 parseRrmMacUeReconfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);

#ifdef KPI_STATS
UInt32 parseRrmMacConfigKpiStatsReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);

/* +- SPR 17777 */
UInt32 parseRrmMacGetKpiStatsReq(
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);
#endif

UInt32 parseRrmMacUeSinrTAReq(/* +- SPR 17777 */
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);

#ifdef PERF_STATS
UInt32 parseRrmMacConfigPerfStatsReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);
#endif

UInt32 parseRrmMacEcidMeasConfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex);
/* Cyclomatic_complexity_changes_end */
/****************************************************************************
 * Function Name  : prepareMACRRMApiHeader
 * Inputs         : l2RrmMsg_p - pointer to L2RrmMsg,
 *                  msgId - Msg Type to be sent to RRM,
 *                  transactionId - specifies the id of a particular transaction
 *                  length - length of the msg
 *                  internalCellIndex - Cell-Index at MAC
 * Returns        : None
 * Description    : This API takes the msgId & buffer to be filled by msg
 *                  header as input.
 ****************************************************************************/
/*SPR 20863 End*/
 void prepareMACRRMApiHeader( L2RrmMsg *l2RrmMsg_p,
        UInt16 msgId, 
        UInt16 transactionId,
        UInt16 length,
        InternalCellIndex internalCellIndex)
{
    UInt8 *cnfMsg_p = (UInt8 *)l2RrmMsg_p;
    UInt16 current = 0;
    InternalCellIndex rrmCellIndex = 0;
    
    if(MAX_NUM_CELL > internalCellIndex)
    {
        rrmCellIndex = macCommonGetRRCCellIndex(internalCellIndex);
        if(INVALID_CELL_INDEX == rrmCellIndex)
        {
            LOG_MAC_MSG(MAC_UE_SCELL_ERR_RESPONSE,LOGWARNING,MAC_RRM_INF,
                    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                    rrmCellIndex,LINE_NUMBER,DEFAULT_INT_VALUE,
                    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                    FUNCTION_NAME,"");
        }
        LTE_SET_U16BIT(cnfMsg_p, transactionId);
        current += U16BIT_LEN;
        LTE_SET_U16BIT(cnfMsg_p + current, MAC_MODULE_ID);
        current += U16BIT_LEN;
        LTE_SET_U16BIT(cnfMsg_p + current, RRM_MODULE_ID);
        current += U16BIT_LEN;
        LTE_SET_U16BIT(cnfMsg_p + current, msgId);
        current += U16BIT_LEN;
        LTE_SET_U16BIT(cnfMsg_p + current, length);
        current += U16BIT_LEN;
        cnfMsg_p[current] = rrmCellIndex;
    }
    else
    {
        cnfMsg_p[current] = API_HEADER_DO_NOT_CARE_CELL_INDEX;
    }
	/* - KPI_CA Review */
}
/*SPR 20863 End*/
/**************************************************************************
 * Function Name  : parseRRMMsg
 * Inputs         : l2RrmMsgReq_p - pointer to structure L2RrmMsg
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required.
 ****************************************************************************/
UInt8 parseRRMMsg(L2RrmMsg *l2RrmMsgReq_p, InternalCellIndex internalCellIndex)
{
    /*+COVERITY 5.3.0 - 32286*/
    UInt32 retVal              = MAC_SUCCESS;
    /*-COVERITY 5.3.0 - 32286*/
    
    /* + Coverity_Stack Fix */
    UInt8 *cnfMsg = PNULL;
    GET_MEM_FROM_POOL(UInt8,cnfMsg,MAX_MSG_SIZE, PNULL);
    /*Coverity 530 CID 40847 */
    /*+COVERITY 5.3.0 - 32114*/
    if(PNULL == cnfMsg)
    {
        return MAC_FAILURE;
    }
    /*-COVERITY 5.3.0 - 32114*/
    /*Coverity 530 CID 40847 */
/*BUG:10657:start*/ 
    memSet(cnfMsg, 0, MAX_MSG_SIZE);
/*BUG:10657:end*/ 
    /* - Coverity_Stack Fix */
    /* SPR 10970 fix + */

    
    L2RrmMsg *l2RrmMsgCnf_p = PNULL;
    UInt16 bytesRead = l2RrmMsgReq_p->header.length - L2_RRM_API_HEADER_LEN;
    UInt16 transactionId = l2RrmMsgReq_p->header.transactionId;
    l2RrmMsgCnf_p = (L2RrmMsg *)cnfMsg;
    /* SPR 13402 fix start */
    memSet( cnfMsg, 0, sizeof(L2RrmMsg));
    /* SPR 13402 fix end */

    switch(l2RrmMsgReq_p->header.apiId){

	    case RRM_MAC_CELL_CONFIG_REQ:
		    {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacCellConfigReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
			    break;
		    }

	    case RRM_MAC_SCHEDULER_RECONFIG_REQ:
		    {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacSchedulerReconfigReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
			    break;
		    }


	    case RRM_MAC_UE_RECONFIG_REQ:
		    {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacUeReconfigReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
			    break;
		    }
#ifdef KPI_STATS
        case RRM_MAC_CONFIGURE_KPI_STATS_REQ:
            {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacConfigKpiStatsReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                break;
            }

        case RRM_MAC_GET_KPI_STATS_REQ:
            {
                /* Cyclomatic_complexity_changes_start */
                /* +- SPR 17777 */
                retVal = parseRrmMacGetKpiStatsReq(
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                break;
            }
#endif /*KPI_STATS*/
        case RRM_MAC_UE_SINR_TA_REQ:
            {
                /* Cyclomatic_complexity_changes_start */
                /* +- SPR 17777 */
                retVal = parseRrmMacUeSinrTAReq(
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                break;
            }
 
            /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
        case RRM_MAC_CONFIGURE_PERF_STATS_REQ:
            {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacConfigPerfStatsReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                break;
            }
#endif
          /* Perf stats RRM-MAC end */

 
            /* + E_CID_5.3 */
        case RRM_MAC_E_CID_MEAS_CONFIG_REQ:
            {
                /* Cyclomatic_complexity_changes_start */
                retVal = parseRrmMacEcidMeasConfigReq(bytesRead,
                        transactionId,
                        l2RrmMsgReq_p,
                        l2RrmMsgCnf_p,
                        cnfMsg,
                        internalCellIndex);
                /* Cyclomatic_complexity_changes_end */
                break; 
            }
        /* - E_CID_5.3 */
        default:

		    LOG_MAC_MSG(MAC_RRM_INVALID_API_ID,LOGWARNING,MAC_RRM_INF,
				    GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
				    l2RrmMsgReq_p->header.apiId,
                    LINE_NUMBER,DEFAULT_INT_VALUE,
				    DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
				    DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
				    FUNCTION_NAME,"");

    }
    /* + Coverity_Stack Fix */
    freeMemPool(cnfMsg);
    /* - Coverity_Stack Fix */
    return retVal;
}
/* CA changes start*/
/**************************************************************************
 * Function Name  : macSendRRMMsgCnf
 * Inputs         : l2RrmMsgReq_p - buffer which came from message queue.
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This API sends the response of the messages received from 
                    RRM.
 ****************************************************************************/
UInt8 macSendRRMMsgCnf(L2RrmMsg *l2RrmMsgReq_p, InternalCellIndex internalCellIndex)
{
    UInt32 retVal              = MAC_SUCCESS;
    UInt16 current             = 0;
    RrmMacCellConfigCnf *rrmMacCellConfigCnf_p               = PNULL;
    RrmMacReconfigSchedulerCnf *rrmMacReconfigSchedulerCnf_p = PNULL;   
    RrmMacUEReConfigCnf *rrmMacUEReConfigCnf_p               = PNULL;
#ifdef KPI_STATS    
    MacConfigureKpiStatsResp *macConfigureKpiStatsResp_p     =PNULL;
#endif /*KPI_STATS*/    
    MacUeSinrTaResp *macUeSinrTaResp                         =PNULL;

    UInt8 *cnfMsg = PNULL;
    GET_MEM_FROM_POOL(UInt8,cnfMsg,MAX_MSG_SIZE, PNULL);
    if(PNULL == cnfMsg)
    {
        LOG_MAC_FATAL( MAC_RRM_INF, "Unable to allocate memory for RRM cnf "
                "message, apiId[%u]", l2RrmMsgReq_p->header.apiId );
        return MAC_FAILURE;
    }

    MacRrmEcidMeasurementResp *macRrmEcidMeasurementResp_p = PNULL;
#ifdef PERF_STATS
    RrmMacConfigPerfStatsCnf *rrmMacConfigPerfStatsCnf_p = PNULL;
#endif

    L2RrmMsg *l2RrmMsgCnf_p = PNULL;
    UInt16 transactionId = l2RrmMsgReq_p->header.transactionId;

    l2RrmMsgCnf_p = (L2RrmMsg *)&cnfMsg[0];
    /*since the cell id which RRM sent to MAC in API header
      is not configured on MAC,setting cell id in response which
      came from RRM*/
    l2RrmMsgCnf_p->header.cellIndex = l2RrmMsgReq_p->header.cellIndex;


    switch(l2RrmMsgReq_p->header.apiId){

        case RRM_MAC_CELL_CONFIG_REQ:
        {  
            retVal = INVALID_CELL_ID_ERROR;

            /* Prepare a response and send it to RRM */
            current += L2_RRM_API_HEADER_LEN;

            rrmMacCellConfigCnf_p = (RrmMacCellConfigCnf *)l2RrmMsgCnf_p->data;
            rrmMacCellConfigCnf_p->responseCode = retVal;
            /* Fill Msg Len in the API header */
            current += sizeof(RrmMacCellConfigCnf);
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CELL_CONFIG_CNF,transactionId,
                    current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }

        case RRM_MAC_SCHEDULER_RECONFIG_REQ:
        {   
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */
            current += L2_RRM_API_HEADER_LEN;
            rrmMacReconfigSchedulerCnf_p = (RrmMacReconfigSchedulerCnf *)l2RrmMsgCnf_p->data;
            rrmMacReconfigSchedulerCnf_p->responseCode = retVal;
            /* Fill Msg Len in the API header */
            current += sizeof(RrmMacReconfigSchedulerCnf);

            /* Prepare a response and send it to RRM */
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_SCHEDULER_RECONFIG_CNF,
                    transactionId,current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }


        case RRM_MAC_UE_RECONFIG_REQ:
        {
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */
            current += L2_RRM_API_HEADER_LEN;
            rrmMacUEReConfigCnf_p = (RrmMacUEReConfigCnf *)l2RrmMsgCnf_p->data;
            rrmMacUEReConfigCnf_p->responseCode = retVal;
            current += sizeof(RrmMacUEReConfigCnf);

            /* Prepare a response and send it to RRM */
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_UE_RECONFIG_CNF,transactionId,
                    current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }
#ifdef KPI_STATS
        case RRM_MAC_CONFIGURE_KPI_STATS_REQ:
        {
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */
            current += L2_RRM_API_HEADER_LEN;
            macConfigureKpiStatsResp_p = (MacConfigureKpiStatsResp *)l2RrmMsgCnf_p->data;
            macConfigureKpiStatsResp_p->responseCode = retVal;
            current += sizeof(MacConfigureKpiStatsResp);
            /* Prepare a response and send it to RRM */
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CONFIGURE_KPI_STATS_CNF,transactionId,
                    current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }

        case RRM_MAC_GET_KPI_STATS_REQ:
        {
            /*RRM does not use this KPI,this API shouldn't come to layer2,Discarding */
            break;
        }
#endif /*KPI_STATS*/
        case RRM_MAC_UE_SINR_TA_REQ:
        {
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */

            current += L2_RRM_API_HEADER_LEN;
            macUeSinrTaResp = (MacUeSinrTaResp *)l2RrmMsgCnf_p->data;
            macUeSinrTaResp->responseCode = retVal;
            current += sizeof(MacUeSinrTaResp);
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_UE_SINR_TA_RESP,transactionId,
                    current, internalCellIndex);

            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }

        /* Perf stats RRM-MAC start */
#ifdef PERF_STATS
        case RRM_MAC_CONFIGURE_PERF_STATS_REQ:
        {
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */

            current += L2_RRM_API_HEADER_LEN;
            rrmMacConfigPerfStatsCnf_p = (RrmMacConfigPerfStatsCnf *)l2RrmMsgCnf_p->data;
            rrmMacConfigPerfStatsCnf_p->responseCode = retVal;
            current += sizeof(RrmMacConfigPerfStatsCnf);
            /* Prepare a response and send it to RRM */
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CONFIGURE_PERF_STATS_CNF,
                    transactionId,current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }
            break;
        }
#endif
        /* Perf stats RRM-MAC end */


        /* + E_CID_5.3 */
        case RRM_MAC_E_CID_MEAS_CONFIG_REQ:
        {
            retVal = INVALID_CELL_ID_ERROR;
            /* Prepare a response and send it to RRM */
            current += L2_RRM_API_HEADER_LEN;
            macRrmEcidMeasurementResp_p = (MacRrmEcidMeasurementResp *)l2RrmMsgCnf_p->data;
            macRrmEcidMeasurementResp_p->bitmask = NO_REPORT_TYPE_REQUIRED;
            macRrmEcidMeasurementResp_p->responseCode = retVal;
            /* Fill Msg Len in the API header */
            current += sizeof(MacRrmEcidMeasurementResp);

            /* Prepare a response and send it to RRM */
            prepareMACRRMApiHeader(l2RrmMsgCnf_p,MAC_RRM_E_CID_MEAS_REPORT_RESP,
                    transactionId,current, internalCellIndex);
            /* Send the confimration back to RRM */
            if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                        ))
            {
                //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
            }

            break; 
        }
        /* - E_CID_5.3 */
        default:

        LOG_MAC_MSG(MAC_RRM_INVALID_API_ID,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                l2RrmMsgReq_p->header.apiId,
                LINE_NUMBER,DEFAULT_INT_VALUE,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");

    }
    /* + Coverity_Stack Fix */
    freeMemPool(cnfMsg);
    /* - Coverity_Stack Fix */
    return retVal;
}
/* CA changes end*/
/****************************************************************************
 * function name  : processRRMMsg
 * inputs         : buffer - pointer to buffer received
 *                  bytesRead - Number of bytes received .
 * returns        : None
 * description    : this api receives message, parse the msgid and call
 *                  the corresponding api to deal with the message.
 ****************************************************************************/
void processRRMMsg (UInt8 *buffer, SInt16 bytesRead)
{
    L2RrmMsg *l2RrmMsgReq_p = PNULL;
    InternalCellIndex internalCellIndex  = INVALID_CELL_INDEX;

    /* RRM message received, perform validations */
    if(MAC_RECV_ERROR == bytesRead)
    {
        LOG_MAC_ERROR( MAC_RRM_INF, "Invalid msgLength[%d]", bytesRead );
        //ALARM_MSG (MAC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        return;
    }

    if(bytesRead < (SInt32)L2_RRM_API_HEADER_LEN)
    {
        LOG_MAC_ERROR( MAC_RRM_INF, "Invalid msgLength[%d]", bytesRead );
        return;
    }
     /*SPR 20863 FIX*/
    l2RrmMsgReq_p = (L2RrmMsg *)getMemFromPool(bytesRead,NULL);
    if(l2RrmMsgReq_p == PNULL)
    {
        ltePanic("Memory alloc failed\n");
        return;
    }

    l2RrmMsgReq_p->header.transactionId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(l2RrmMsgReq_p->header.transactionId);

    l2RrmMsgReq_p->header.srcModuleId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(l2RrmMsgReq_p->header.srcModuleId);

    l2RrmMsgReq_p->header.destModuleId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(l2RrmMsgReq_p->header.destModuleId);

    l2RrmMsgReq_p->header.apiId = LTE_GET_U16BIT(buffer);
    buffer += sizeof(l2RrmMsgReq_p->header.apiId);

    l2RrmMsgReq_p->header.length = LTE_GET_U16BIT(buffer);
    buffer += sizeof(l2RrmMsgReq_p->header.length);

    l2RrmMsgReq_p->header.cellIndex = *buffer;
    buffer += sizeof(l2RrmMsgReq_p->header.cellIndex);
    buffer += sizeof(l2RrmMsgReq_p->header.reserved1);
    /*SPR 21653 changes start*/
    buffer += sizeof(l2RrmMsgReq_p->header.reserved2);
    /*SPR 21653 changes end*/

	LOG_MAC_MSG(RRM_MAC_MSG_ID,LOGWARNING,MAC_RRM_INF,
			getCurrentTick(), l2RrmMsgReq_p->header.destModuleId, l2RrmMsgReq_p->header.transactionId,
			l2RrmMsgReq_p->header.cellIndex, l2RrmMsgReq_p->header.apiId,l2RrmMsgReq_p->header.length,
			DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
			FUNCTION_NAME,"");

    if(l2RrmMsgReq_p->header.length <= L2_RRM_API_HEADER_LEN)
    {
        lteWarning ("Message received with invalid length = [%d].\
                API ID - [%d].\n", l2RrmMsgReq_p->header.length,
                l2RrmMsgReq_p->header.apiId);
        macSendRRMMsgCnf(l2RrmMsgReq_p,internalCellIndex);
        if(l2RrmMsgReq_p != NULL)
        {	
            freeMemPool(l2RrmMsgReq_p);
        }
        return;
    }
    (void)memcpy(l2RrmMsgReq_p->data,buffer,(l2RrmMsgReq_p->header.length - L2_RRM_API_HEADER_LEN));
    /*SPR 20863 FIX*/

    /*CA changes start*/
    /*Validate the cell index(i.e. whether the cell is configured or not)*/
    internalCellIndex = macCommonGetInternalCellIndex(
            l2RrmMsgReq_p->header.cellIndex ); 
	/* + KPI_CA Review Comments */
    if(INVALID_CELL_INDEX == internalCellIndex  )
    {
        LOG_MAC_WARNING ( MAC_RRM_INF, "Message received for Non-configured "
                "Cell = [%d]. API ID - [%d].\n", 
                l2RrmMsgReq_p->header.cellIndex,
                l2RrmMsgReq_p->header.apiId);
        macSendRRMMsgCnf(l2RrmMsgReq_p,internalCellIndex);
        /*SPR 20863 FIX*/
        if(l2RrmMsgReq_p != NULL)
        {	
            freeMemPool(l2RrmMsgReq_p);
        }
        /*SPR 20863 FIX*/

        return;
    }
	/* - KPI_CA Review Comments */
    /*CA changes end*/
    if((RRM_MODULE_ID == l2RrmMsgReq_p->header.srcModuleId)
            && (MAC_MODULE_ID == l2RrmMsgReq_p->header.destModuleId))
    {
        parseRRMMsg(l2RrmMsgReq_p, internalCellIndex);
    }
    else
    {
        LOG_MAC_MSG(MAC_RRM_WRONG_MODULE_ID,LOGWARNING,MAC_RRM_INF,
                GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
                l2RrmMsgReq_p->header.srcModuleId,
                l2RrmMsgReq_p->header.destModuleId,
                LINE_NUMBER,
                DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
                DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
                FUNCTION_NAME,"");
    }
    /*SPR 20863 FIX*/
    if(l2RrmMsgReq_p != NULL)
    {	
        freeMemPool(l2RrmMsgReq_p);
    }
    /*SPR 20863 FIX*/
}
/* Perf stats RRM-MAC start */

#ifdef PERF_STATS
/****************************************************************************
 *  Function Name  : cellMacFillPerfStatsForRRM
 *  Inputs         : cnfBuff_p - Pointer to confirmation buffer
 *                   perfBitmap - Perf Stats to be filled in confirmation buffer
 *                   periodicFlag
 *                   internalCellIndex - Cell-Index at MAC
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This API will fill the stats in the confirmation buffer for RRM.
 *******************************************************************************/
/* + PERF_CA */
void cellMacFillPerfStatsForRRM(UInt8 *cnfBuff_p, UInt32 perfBitmap, UInt32 periodicFlag,
						InternalCellIndex internalCellIndex)
{
    UInt32 qci = 0;
    UInt32 range = 0;
    UInt32 srb = 0;
    UInt32 utilIndex = 0;
    UInt32 cfi = 0;
    UInt32 localKpiBitmap = 0;
    UInt8 firstBitSet = 0;

    CellMacPerfStatsResp  *cellMacPerfStatsResp_p = PNULL;
    CellMacPerfStatsResp  *pLteMacCellPerfStats   = PNULL;
    CellHarqPerfStats     *pCellHarqPerfStats     = PNULL;
    RachSuccFailPerfStats *pRachSuccFailPerfStats = PNULL;
    PdcchUtilizationStats *pPdcchUtilizationStats = PNULL;
    CellPRBUsagePerfStats *pCellPRBUsagePerfStats = PNULL;

    pLteMacCellPerfStats   = &(g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].\
                               perfStats.lteMacPerfStats.lteMacCellPerfStats[internalCellIndex]);
    pCellHarqPerfStats     = &(pLteMacCellPerfStats->cellHarqPerfStats);
    pRachSuccFailPerfStats = &(pLteMacCellPerfStats->rachSuccFailPerfStats);
    pPdcchUtilizationStats = &(pLteMacCellPerfStats->pdcchUtilizationStats);
    pCellPRBUsagePerfStats = &(pLteMacCellPerfStats->cellPRBUsagePerfStats);

    MacCellPerfStatsIndParams *macCellPerfStatsIndParams_p = PNULL;
    MacGetCellPerfStatsCnfParams *macGetCellPerfStatsCnfParams_p = PNULL;
    localKpiBitmap = perfBitmap;
    /*Set the pointer required cnt/Ind acc. to periodic flag*/
    /* Set the pointer required cnt/Ind acc. to periodic flag*/
    if(periodicFlag)
    {
        macCellPerfStatsIndParams_p = (MacCellPerfStatsIndParams *)&cnfBuff_p[L2_RRM_API_HEADER_LEN];
        macCellPerfStatsIndParams_p->cellMacPerfStatsResp.durationTTI = 
             gPerfStats.perfStats->macLastCalculatedTTI;
        cellMacPerfStatsResp_p = (CellMacPerfStatsResp*)(&macCellPerfStatsIndParams_p->cellMacPerfStatsResp);
    }
    else
    {
        macGetCellPerfStatsCnfParams_p = (MacGetCellPerfStatsCnfParams *)&cnfBuff_p[L2_RRM_API_HEADER_LEN];
        macGetCellPerfStatsCnfParams_p->cellMacPerfStatsResp.durationTTI =  \
                                                                            gPerfStats.perfStats->macLastCalculatedTTI;
        cellMacPerfStatsResp_p = (CellMacPerfStatsResp*)(&macGetCellPerfStatsCnfParams_p->cellMacPerfStatsResp);
    }

    firstBitSet = ffs_wrapper( localKpiBitmap );
    /* Fills the response buffer according to the requested bitmap*/
    while (firstBitSet)
    {
	switch (firstBitSet - 1)
	{
	    case PERF_STATS_PRB_USAGE_HARQ:
		for (qci = 0; qci < MAX_QCI; qci++)
		{
		    cellMacPerfStatsResp_p->cellHarqPerfStats.totalDLPRBUsageforHarq[qci] = 
			pCellHarqPerfStats->totalDLPRBUsageforHarq[qci];
		}
		cellMacPerfStatsResp_p->cellHarqPerfStats.totalDLPRBUsageforHarqRetx =
		    pCellHarqPerfStats->totalDLPRBUsageforHarqRetx;
	    break;

	    case PERF_STATS_RACH_SUCC_FAIL:
		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCBRA = 
		    pRachSuccFailPerfStats->totalRcvdMsg3RandomAccessPreamblesCBRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalRcvdMsg3RandomAccessPreamblesCFRA = 
		    pRachSuccFailPerfStats->totalRcvdMsg3RandomAccessPreamblesCFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CBRA = 
		    pRachSuccFailPerfStats->totalFailedRandomAccessPreamblesMsg2CBRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalFailedRandomAccessPreamblesMsg2CFRA = 
		    pRachSuccFailPerfStats->totalFailedRandomAccessPreamblesMsg2CFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalAllocatedRandomAccessPreambles = 
		    pRachSuccFailPerfStats->totalAllocatedRandomAccessPreambles;

		for (range = 0; range < MAX_TA_RANGE; range++)
		{
		    cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalSuccRandomAccessTA[range] = 
			pRachSuccFailPerfStats->totalSuccRandomAccessTA[range];
		}
		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalUnassignedFalseCFRA = 
		    pRachSuccFailPerfStats->totalUnassignedFalseCFRA;

		cellMacPerfStatsResp_p->rachSuccFailPerfStats.totalUnassignedSumCFRA = 
		    pRachSuccFailPerfStats->totalUnassignedSumCFRA;
	    break;

	    case PERF_STATS_PDCCH_UTILIZATION:
		for (utilIndex = 0; utilIndex < MAX_CCE_UTIL_INDEX; utilIndex++)
		{
		    cellMacPerfStatsResp_p->pdcchUtilizationStats.totalPdcchCceUtil[utilIndex] = 
			pPdcchUtilizationStats->totalPdcchCceUtil[utilIndex];
		}
		for (cfi = 0; cfi < MAX_CFI; cfi++)
		{
		    cellMacPerfStatsResp_p->pdcchUtilizationStats.noOfSubFrameWithPdcchCfi[cfi] = 
			pPdcchUtilizationStats->noOfSubFrameWithPdcchCfi[cfi];
		}
		cellMacPerfStatsResp_p->pdcchUtilizationStats.noOfSubFrameWithCceUtilMoreThanZero = 
		    pPdcchUtilizationStats->noOfSubFrameWithCceUtilMoreThanZero;

		cellMacPerfStatsResp_p->pdcchUtilizationStats.totalCceAvail = 
		    pPdcchUtilizationStats->totalCceAvail;
	    break;

	    case PERF_STATS_PUCCH_UTILIZATION:
		cellMacPerfStatsResp_p->totalPUCCHResAllocationAttempts = 
		    pLteMacCellPerfStats->totalPUCCHResAllocationAttempts;
	    break;

	    case PERF_STATS_CELL_PRB_USAGE:

	    /* + SPR_5711 */
               cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBUsageSRB[0] = 
		pCellPRBUsagePerfStats->totalULPRBUsageSRB[0];

	       cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageSRB[0] = \
				  pCellPRBUsagePerfStats->totalDLPRBUsageSRB[0];
             for(srb=1; srb<MAX_SRB; srb++)
	     {
	          cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBUsageSRB[srb] = \
		   pCellPRBUsagePerfStats->totalULPRBUsageSRB[srb] >> SCALE_UP_MULTIPLYING_FACTOR;
		  cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageSRB[srb] = \
		     pCellPRBUsagePerfStats->totalDLPRBUsageSRB[srb] >> SCALE_UP_MULTIPLYING_FACTOR;
       	    /* - SPR_5711 */
	     }
	     cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsagePCCH =
		pCellPRBUsagePerfStats->totalDLPRBUsagePCCH;

	     cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBUsageBCCH = 
		    pCellPRBUsagePerfStats->totalDLPRBUsageBCCH;
                  
             cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalDLPRBAvailable  = \
                                            pCellPRBUsagePerfStats->totalDLPRBAvailable;
             cellMacPerfStatsResp_p->cellPRBUsagePerfStats.totalULPRBAvailable  = \
                                              pCellPRBUsagePerfStats->totalULPRBAvailable;
	    break;
            
            /*+ Dyn UEs To Be SchPerTTI +*/
            case PERF_STATS_TOTAL_ACT_UE_IN_UL_DL:
			/* + PERF_CA GBL */
            cellMacPerfStatsResp_p->activeUEsInDlUlStats.totalActiveUEsInDl = totalActiveUEsDL_g[internalCellIndex];
            cellMacPerfStatsResp_p->activeUEsInDlUlStats.totalActiveUEsInUl = totalActiveUEsUL_g[internalCellIndex];
			/* - PERF_CA GBL */
            break;
            /*- Dyn UEs To Be SchPerTTI -*/

	    default :
		break;
	}
	localKpiBitmap = localKpiBitmap & (~(1<<(firstBitSet - 1)));
	firstBitSet = ffs_wrapper(localKpiBitmap);
    }
}
/* - PERF_CA */


/****************************************************************************
 *  Function Name  : macProcessPerfCounterforRRM
 *  Inputs         : None
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function fills the perf stats and send to RRM.
 *******************************************************************************/
/* + SPR 17439 */
void macProcessPerfCounterforRRM(void)
/* - SPR 17439 */    
{
    LtePerfStatsConfig *lteMacPerfStatsConfig_p =  &(g_perfStatsScannerObj.\
            perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].perfConfig.lteMacPerfStatsConfig);
    UInt32 msgSize = 0;
    L2RrmMsg *l2RrmMsg_p = PNULL;
	/* + PERF_CA */
	InternalCellIndex cellIndex = 0;
	/* - PERF_CA */
    /* + coverity 42167, 42166 */
    UInt8* cnfBuff_p = PNULL;
    /* + coverity 42167, 42166 */

    if(lteMacPerfStatsConfig_p->periodicReporting)
    {
        if (lteMacPerfStatsConfig_p->cellPerfStatsToReportBitMap)
        {
			/* + PERF_CA */
			for(cellIndex = 0; cellIndex < cellSpecificParams_g.numOfCells; cellIndex++)
            {
    			cnfBuff_p = (UInt8 *)getMemFromPool(MAX_OAM_PERF_STATS_SIZE,NULL);
    			if(PNULL == cnfBuff_p)
    			{
       				lteWarning("In %s, MEMORY ALLOC FAILURE\n",__func__);
       				return;
    			}
    			l2RrmMsg_p = (L2RrmMsg *)&cnfBuff_p[0];

				cellMacFillPerfStatsForRRM( (UInt8 *)l2RrmMsg_p, 
                        lteMacPerfStatsConfig_p->cellPerfStatsToReportBitMap,
                        SET_PERIODIC_FLAG, cellIndex );
            	msgSize = sizeof(CellMacPerfStatsResp) + L2_RRM_API_HEADER_LEN;
                /* + SPR 11139 Fix */
            	prepareMACRRMApiHeader(l2RrmMsg_p, RRM_MAC_CELL_PERF_STATS_IND,0,msgSize,cellIndex);
                /* - SPR 11139 Fix */

            	/* Sending message to RRM */
            	if(MAC_SEND_ERROR == sendMsgToRRM(cnfBuff_p, msgSize
                        ))
            	{
                    LOG_MAC_ERROR( MAC_RRM_INF, "Unable to send the message to RRM!" );
            	}
    			/* + coverity 42167, 42166 */
    			freeMemPool(cnfBuff_p);
    			/* + coverity 42167, 42166 */
			}
			/* - PERF_CA */
        }
    }
    resetMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC_RESET_CELL_CONTAINER);
    resetMacPerfContainer(PERF_STATS_CONTAINER_RRM_MAC_RESET_UE_CONTAINER);

    g_perfStatsScannerObj.perfStatsContainer[PERF_STATS_CONTAINER_RRM_MAC].
        perfStats.lteMacPerfStats.lastMACCalculatedTTI = 0;

}
#endif
/* Perf stats RRM-MAC end */

/* Cyclomatic_complexity_changes_start */
/**************************************************************************
 * Function Name  : parseRrmMacCellConfigReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for cell config request.
 ****************************************************************************/
UInt32 parseRrmMacCellConfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal = MAC_SUCCESS;
    UInt16 current = 0;
    RrmMacCellConfigReq *rrmMacCellConfigReq_p = PNULL;
    RrmMacCellConfigCnf *rrmMacCellConfigCnf_p = PNULL;

    /* Check validation */
    if(RRM_MAC_CELL_CONFIG_REQ_LENGTH != bytesRead)
    {
        retVal = INVALID_CELL_CONFIG_API_LENGTH;
    }
    else
    {
        rrmMacCellConfigReq_p = (RrmMacCellConfigReq *)(l2RrmMsgReq_p->data);
        retVal = initResourceInfo(rrmMacCellConfigReq_p, internalCellIndex);
    }

    /* Prepare a response and send it to RRM */
    current += L2_RRM_API_HEADER_LEN;

    rrmMacCellConfigCnf_p = (RrmMacCellConfigCnf *)l2RrmMsgCnf_p->data;
    rrmMacCellConfigCnf_p->responseCode = retVal;
    /* Fill Msg Len in the API header */
    current += sizeof(RrmMacCellConfigCnf);

    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CELL_CONFIG_CNF,transactionId,
            current, internalCellIndex);
    LOG_MAC_MSG(MAC_RRM_MESSAGE_SENT,LOGDEBUG,MAC_RRM_INF,
            GET_CURRENT_TICK_FOR_CELL(internalCellIndex),
            retVal,LINE_NUMBER,DEFAULT_INT_VALUE,
            DEFAULT_INT_VALUE,DEFAULT_INT_VALUE,
            DEFAULT_FLOAT_VALUE,DEFAULT_FLOAT_VALUE,
            FUNCTION_NAME,"");

    /* Send the confimration back to RRM */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return retVal;
}

/**************************************************************************
 * Function Name  : parseRrmMacSchedulerReconfigReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for scheduler reconfig request.
 ****************************************************************************/
UInt32 parseRrmMacSchedulerReconfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal = MAC_SUCCESS;
    UInt16 current = 0;
    RrmMacReconfigSchedulerReq *rrmMacReconfigSchedulerReq_p = PNULL;
    RrmMacReconfigSchedulerCnf *rrmMacReconfigSchedulerCnf_p = PNULL;   

    /* Check Validation */
    if(RRM_MAC_SCHEDULER_RECONFIG_REQ_LENGTH != bytesRead)
    {
        retVal = INVALID_SCHEDULER_RECONFIG_API_LENGTH;
    } 
    else
    {
        rrmMacReconfigSchedulerReq_p = (RrmMacReconfigSchedulerReq *)(l2RrmMsgReq_p->data);
        retVal = macReconfigSchedulerParams(rrmMacReconfigSchedulerReq_p, internalCellIndex);
    }

    current += L2_RRM_API_HEADER_LEN;
    rrmMacReconfigSchedulerCnf_p = (RrmMacReconfigSchedulerCnf *)l2RrmMsgCnf_p->data;
    rrmMacReconfigSchedulerCnf_p->responseCode = retVal;
    /* Fill Msg Len in the API header */
    current += sizeof(RrmMacReconfigSchedulerCnf);

    /* Prepare a response and send it to RRM */
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_SCHEDULER_RECONFIG_CNF,
            transactionId,current, internalCellIndex);
    /* Send the confimration back to RRM */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return retVal;
}

/**************************************************************************
 * Function Name  : parseRrmMacUeReconfigReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for ue reconfig request.
 ****************************************************************************/
UInt32 parseRrmMacUeReconfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal    = MAC_SUCCESS;
    UInt16 current = 0;

    /*+COVERITY 5.3.0 - 32278*/
    UInt32 returnVal = MAC_SUCCESS;
    /*-COVERITY 5.3.0 - 32278*/
    /*+COVERITY 5.3.0 - 32286*/
    UInt32 retValue    = MAC_SUCCESS;
    UInt32 returnValue = MAC_SUCCESS;
    /*-COVERITY 5.3.0 - 32286*/
    /* ca changes start*/
    UInt32 retVal1 = MAC_SUCCESS;
    UInt32 retVal2 = MAC_SUCCESS;
    /*ca changes end*/
    RrmMacUeReconfigReq *rrmMacUeReconfigReq_p               = PNULL;
    RrmMacUEReConfigCnf *rrmMacUEReConfigCnf_p               = PNULL;

    /* Check Validation */
    /*Fix_RRM_KPI  SPR 4862 START*/
#ifdef KPI_STATS
    if (RRM_MAC_CONFIGURE_KPI_STATS_REQ_LENGTH > bytesRead)
#else    
    if (RRM_MAC_UE_RECONFIG_REQ_LENGTH > bytesRead)
#endif    
        /*Fix_RRM_KPI SPR 4862 END*/
    {
        retVal = INVALID_UE_RECONFIG_API_LENGTH;
    }
    else
    {
        /*Coverity 65728 Start*/
        rrmMacUeReconfigReq_p = (RrmMacUeReconfigReq *)(l2RrmMsgReq_p->data);
        /*UL_MU_MIMO_CHG Start*/
        /*bit 0 indicates the reconfig of ueLocationInfo*/
        if (rrmMacUeReconfigReq_p->bitmask & UE_LOCATION_RECONFIG_BIT)
        {
            /* SPR5889 Fix Start */
            returnVal = reconfigUELocationType(rrmMacUeReconfigReq_p, 
                    l2RrmMsgCnf_p->data, internalCellIndex);
            /* SPR5889 Fix End */
        }
        /*bit 1 indicates the reconfig of UL MUMIMO info*/
        if (rrmMacUeReconfigReq_p->bitmask & UE_ULMUMIMO_RECONFIG_BIT)
        {
            retVal = reconfigUEUlMUMIMO(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }
        /*UL_MU_MIMO_CHG End*/

        /* + TM7_8 Changes Start*/

        /* bit 2 indicates the reconfig of NumOfLayerInfo */
        if (rrmMacUeReconfigReq_p->bitmask & UE_NUM_OF_LAYER_RECONFIG_BIT)
        {
            retValue = reconfigNumOfLayer(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }

        /* bit 3 indicates the reconfig of Beam Forming Algo */
        if (rrmMacUeReconfigReq_p->bitmask & UE_BEAM_FORMING_ALGO_RECONFIG_BIT)
        {
            returnValue = reconfigBeamFormingAlgo(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }

        /* - TM7_8 Changes End*/
        /*CA changes start*/
        /* bit 4 indicates the reconfig of CA config info list */
        if (rrmMacUeReconfigReq_p->bitmask & UE_CA_CONFIG_INFO_LIST_RECONFIG_BIT)
        {
            retVal1 = macReconfigUECAConfigInfoList(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }
        /* bit 5 indicates the reconfig of CQI validity Timer info list */
        if (rrmMacUeReconfigReq_p->bitmask & UE_CQI_VALIDITY_TIMER_LIST_RECONFIG_BIT)
        {
            retVal2 = macReconfigUECqiValidityTimerList(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }
        if (rrmMacUeReconfigReq_p->bitmask & UE_DL_CONTROL_CHANNEL_INTERFERENCE_LIST_RECONFIG_BIT)
        {
            retVal2 = macReconfigUEControlChannelInterferenceList(rrmMacUeReconfigReq_p,
                    l2RrmMsgCnf_p->data, internalCellIndex);
        }
        /*CA changes end*/
    }
    /*Coverity 65728 End*/


    current += L2_RRM_API_HEADER_LEN;

    rrmMacUEReConfigCnf_p = (RrmMacUEReConfigCnf *)l2RrmMsgCnf_p->data;
    if(retVal == INVALID_UE_RECONFIG_API_LENGTH)
    {
        rrmMacUEReConfigCnf_p->responseCode =  INVALID_UE_RECONFIG_API_LENGTH;
    }
    /* SPR5889 Fix Start */
    else if ( (MAC_FAILURE == returnVal) || (MAC_FAILURE == retVal)
            /* + TM7_8 Changes Start*/
            || (MAC_FAILURE == retValue) || (MAC_FAILURE == returnValue)
            /* - TM7_8 Changes End*/
            /*CA changes start*/
            || (MAC_FAILURE == retVal1)||(MAC_FAILURE == retVal2))
        /*CA changes end*/
    {
        rrmMacUEReConfigCnf_p->responseCode =  MAC_FAILURE;
    }
    /* SPR 9814 start */
    else if ( (INVALID_NUM_OF_UE == returnVal) || (INVALID_NUM_OF_UE == retVal) ||
            (INVALID_NUM_OF_UE == retValue) || (INVALID_NUM_OF_UE == returnValue) ||
            (INVALID_NUM_OF_UE == retVal1) || (INVALID_NUM_OF_UE == retVal2))
    {
        rrmMacUEReConfigCnf_p->responseCode =  INVALID_NUM_OF_UE;
    }
    /* SPR 9814 end */
    else if ( (MAC_PARTIAL_SUCCESS == returnVal) || (MAC_PARTIAL_SUCCESS == retVal) ||
            /* + TM7_8 Changes Start*/
            (MAC_PARTIAL_SUCCESS == retValue) || (MAC_PARTIAL_SUCCESS == returnValue) ||
            /* - TM7_8 Changes End*/
            (MAC_PARTIAL_SUCCESS == retVal1) || (MAC_PARTIAL_SUCCESS == retVal2))
    {
        rrmMacUEReConfigCnf_p->responseCode =  MAC_PARTIAL_SUCCESS;
    }
    else
    {
        rrmMacUEReConfigCnf_p->responseCode = MAC_SUCCESS;
    }
    /* SPR5889 Fix End */

    /* + TM7_8 Changes Start*/
    retVal = rrmMacUEReConfigCnf_p->responseCode;
    /* - TM7_8 Changes End*/
    /* Review comment fix start RJ39 */
    current += sizeof(RrmMacUEReConfigCnf);
    /* Review comment fix start  RJ39 */

    /* Prepare a response and send it to RRM */
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_UE_RECONFIG_CNF,
            transactionId,current, internalCellIndex);
    /* Send the confimration back to RRM */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return retVal;
}

#ifdef KPI_STATS
/**************************************************************************
 * Function Name  : parseRrmMacConfigKpiStatsReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for Config kpi stats request.
 ****************************************************************************/
UInt32 parseRrmMacConfigKpiStatsReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal  = MAC_SUCCESS;
    UInt16 current = 0;
    MacRrmConfigureKpiStatsResp *macConfigureKpiStatsResp_p     =PNULL;

    /*Fix_RRM_KPI  SPR 4862 START*/
    if (RRM_MAC_CONFIGURE_KPI_STATS_REQ_LENGTH > bytesRead)
        /*Fix_RRM_KPI SPR 4862 END*/
    {
        /* Coverity_31229 Fix */
        retVal = INVALID_API_LENGTH;
        return retVal;
        /* Coverity_31229 Fix */
    }
    /* + KPI_CA Review */
    /* Has MAC inited */
    if( macCommonGetNumberOfInitReceived() == macParams_g.numCellsConfigured )	
    {
        retVal = handleMACConfigureKpiStatReq(l2RrmMsgReq_p->data,transactionId,KPI_CONTAINER_RRM_MAC);
    } 
    else {
        retVal = MAC_LAYER_NOT_INITIALIZED;
    }
    current += L2_RRM_API_HEADER_LEN;
    /* SPR 9068 Fix Start */
    macConfigureKpiStatsResp_p = (MacRrmConfigureKpiStatsResp *)l2RrmMsgCnf_p->data;
    macConfigureKpiStatsResp_p->responseCode = retVal;
    current += sizeof(MacRrmConfigureKpiStatsResp);
    /* SPR 9068 Fix End */
    /* + KPI_CA Review Comments */
    /* Prepare a response and send it to RRM */
    /* + KPI Review Comments */
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CONFIGURE_KPI_STATS_CNF,
            transactionId,current,internalCellIndex);/* KPI_CA */
    /* Send the confimration back to RRM */
    /* - KPI_CA Review Comments */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    /* - KPI_CA Review */
    return retVal;
}

/**************************************************************************
 * Function Name  : parseRrmMacGetKpiStatsReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for Get kpi stats request.
 ****************************************************************************/
/* +- SPR 17777 */
UInt32 parseRrmMacGetKpiStatsReq(
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal  = MAC_SUCCESS;
    UInt16 current = 0;

    /* + stats_review_comments_ipc */
    LTE_SET_U16BIT( &(l2RrmMsgCnf_p->header.transactionId), transactionId);
    parseMACGetKpiStatReq(l2RrmMsgReq_p->data, (UInt8 *)l2RrmMsgCnf_p,&current, KPI_CONTAINER_RRM_MAC,internalCellIndex);
    current += L2_RRM_API_HEADER_LEN;
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_GET_KPI_STATS_CNF,
            transactionId,current, internalCellIndex);
    /* Send the confimration back to RRM */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }
    /* - stats_review_comments_ipc */

    return retVal;
}
#endif

/**************************************************************************
 * Function Name  : parseRrmMacUeSinrTAReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for ue sinr ta request.
 ****************************************************************************/
/* +- SPR 17777 */
UInt32 parseRrmMacUeSinrTAReq(
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal  = MAC_SUCCESS;
    UInt16 current = 0;
    UInt16 ueIndex = 0;
    UInt16 rrmUeIndex = 0;
    UInt16 *buffer_p = (UInt16 *)(l2RrmMsgReq_p->data);
    MacUeSinrTaResp *macUeSinrTaResp = PNULL;

    rrmUeIndex = *buffer_p;

    current += L2_RRM_API_HEADER_LEN;

    macUeSinrTaResp = (MacUeSinrTaResp *)l2RrmMsgCnf_p->data;

    /* fill the ueIndex recvd from RRM in response */
    macUeSinrTaResp->ueIndex = rrmUeIndex;

    /* Now get the internal cell index */
    ueIndex = getUeIdxFromRrcUeIdx(rrmUeIndex);
    /* The validity of internal UE index is checked in the below function
     * call to fill the data */

    /* Fill the data for requested UE */
    fillMACUeSinrTaReq (ueIndex, internalCellIndex, macUeSinrTaResp);

    current += sizeof(MacUeSinrTaResp);
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_UE_SINR_TA_RESP,
            transactionId,current, internalCellIndex);
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return retVal;
}

#ifdef PERF_STATS
/**************************************************************************
 * Function Name  : parseRrmMacConfigPerfStatsReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for config performance stats request.
 ****************************************************************************/
UInt32 parseRrmMacConfigPerfStatsReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal  = MAC_SUCCESS;
    UInt16 current = 0;
    RrmMacConfigPerfStatsReq *rrmMacConfigPerfStatsReq_p = PNULL;
    RrmMacConfigPerfStatsCnf *rrmMacConfigPerfStatsCnf_p = PNULL;

    if ( RRM_MAC_CONFIGURE_PERF_STATS_REQ_LENGTH != bytesRead )
    {
        retVal = MAC_FAILURE;
    }
    else
    {
        rrmMacConfigPerfStatsReq_p = (RrmMacConfigPerfStatsReq *)(l2RrmMsgReq_p->data);
        retVal = processRrmMacConfigPerfStatsReq(rrmMacConfigPerfStatsReq_p,transactionId,internalCellIndex);
    }

    /* Sending confirm message */
    current += L2_RRM_API_HEADER_LEN;
    rrmMacConfigPerfStatsCnf_p = (RrmMacConfigPerfStatsCnf *)l2RrmMsgCnf_p->data;
    rrmMacConfigPerfStatsCnf_p->responseCode = retVal;
    current += sizeof(RrmMacConfigPerfStatsCnf);
    /* Prepare a response and send it to RRM */
    prepareMACRRMApiHeader(l2RrmMsgCnf_p,RRM_MAC_CONFIGURE_PERF_STATS_CNF,
            transactionId,current, internalCellIndex);
    /* Send the confimration back to RRM */
    if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                ))
    {
        //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
    }

    return retVal;
}
#endif

/**************************************************************************
 * Function Name  : parseRrmMacEcidMeasConfigReq
 * Inputs         : bytesRead - Message size
 *                  transactionId
 *                  l2RrmMsgReq_p - pointer to req rrm msg
 *                  l2RrmMsgCnf_p - pointer to reponse msg
 *                  cnfMsg - pointer to msg to send rrm
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS/MAC_FAILURE
 * Description    : This API parse the RRM message received and send 
 *                  confirmation if required for ECID MEas Config request.
 ****************************************************************************/
UInt32 parseRrmMacEcidMeasConfigReq(UInt16 bytesRead,
        UInt16 transactionId,
        L2RrmMsg *l2RrmMsgReq_p,
        L2RrmMsg *l2RrmMsgCnf_p,
        UInt8 *cnfMsg,
        InternalCellIndex internalCellIndex)
{
    UInt32 retVal  = MAC_SUCCESS;
    UInt16 current = 0;
    RrmMacEcidMeasConfigReq *rrmMacEcidMeasConfigReq_p = PNULL;
    MacRrmEcidMeasurementResp *macRrmEcidMeasurementResp_p = PNULL;

    if(RRM_MAC_E_CID_MEAS_CONFIG_REQ_LENGTH != bytesRead)
    {
        current += L2_RRM_API_HEADER_LEN;
        macRrmEcidMeasurementResp_p = (MacRrmEcidMeasurementResp *)l2RrmMsgCnf_p->data;
        macRrmEcidMeasurementResp_p->bitmask = NO_REPORT_TYPE_REQUIRED;
        macRrmEcidMeasurementResp_p->responseCode = INVALID_E_CID_MEAS_CONFIG_API_LENGTH;
        /* Fill Msg Len in the API header */
        current += sizeof(MacRrmEcidMeasurementResp);

        /* Prepare a response and send it to RRM */
        prepareMACRRMApiHeader(l2RrmMsgCnf_p,MAC_RRM_E_CID_MEAS_REPORT_RESP,
                transactionId,current, internalCellIndex);
        /* Send the confimration back to RRM */
        if(MAC_SEND_ERROR ==  sendMsgToRRM(cnfMsg, current
                    ))
        {
            //ALARM_MSG (MAC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID, MAJOR_ALARM);
        }

    }
    else
    {
        rrmMacEcidMeasConfigReq_p = (RrmMacEcidMeasConfigReq *)(l2RrmMsgReq_p->data);
        /*CA Changes start  */
        processECidMeasReportReq(rrmMacEcidMeasConfigReq_p,transactionId,internalCellIndex);
        /*CA Changes end  */
    }

    return retVal;
}
/* Cyclomatic_complexity_changes_end */

