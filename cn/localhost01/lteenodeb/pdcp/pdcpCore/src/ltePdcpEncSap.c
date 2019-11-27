/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2011 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpEncSap.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Interface between the PDCP layer and Security engine
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 ****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#ifdef PDCP_ASYNC_INTERFACE

#include "ltePdcpEncSap.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpDataTransferRx.h"
#include "ltePdcpContext.h"
#include "ltePdcpErrors.h"
#include "lteMsgPool.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpOamSap.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpRrcSap.h"

/* SPR 15816 fix start */
#include "lteCircQueue.h"
/* SPR 15816 fix end */



/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15236 fix start */
/* Global Sec UE Context */
PdcpSecUeContext   secUeCtx[MAX_UE_SUPPORTED];
/* SPR 15236 fix end */

/* Queue to store input packets */
/* + SPR 14179 */
LTE_CIRCQUEUE pdcpSecInputTxQ_g;
LTE_CIRCQUEUE pdcpSecInputRxQ_g;
/* - SPR 14179 */
/* + SPR 17073 */
LTE_CIRCQUEUE pdcpSecSrbInputTxQ_g;
LTE_CIRCQUEUE pdcpSecSrbInputRxQ_g;
/* - SPR 17073 */
#ifdef PDCP_ASYNC_INTR
LTE_SQUEUE secIntrInputTxQ_g;
LTE_SQUEUE secIntrInputRxQ_g;
#endif
/*SPR 15909 Fix Start*/
extern tickType_t pdcpCurrentTime_g;
/*SPR 15909 Fix End*/
extern MSGPOOL txOutMsgPool_g;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/
/****************************************************************************
 * Function Name  : pdcpCreateSecContext
 * Inputs         : entity context, radio bearer
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Creates shared descriptor with security engine
 ****************************************************************************/
UInt16 pdcpCreateSecContext (
        /* SPR 15236 fix start */
        UInt16 ueIndex, UInt8 lcId, 
        /* SPR 15236 fix start */
        LP_PdcpEnityContext entity_p,
    /* SPR 3276 changes start */
        UInt8       radioBearer) 
    /* SPR 3276 changes end */
{
    /* SPR 15236 fix start */
    /* Get the pointer of global context of SEC */
    LP_PdcpSecContextInfo localSecCtx_p = PDCP_SEC_CONTEXT( ueIndex, lcId, 
            entity_p->rbDirection ); 

    entity_p->pdcpSecurityContext_p = localSecCtx_p;
    /* SPR 15236 fix end */
    /*SPR 17001 fix start*/
    /*code removed*/
    /*SPR 17001 fix end*/
 
    /* update the context */
    entity_p->pdcpSecurityContext_p->snSize = (UInt8)(entity_p->snSize);
    /* SPR 3276 changes start */
    entity_p->pdcpSecurityContext_p->bearer = radioBearer - 1;
    /* SPR 3276 changes end */
    entity_p->pdcpSecurityContext_p->direction = 
                            (UInt8)(entity_p->rbDirection);
    entity_p->pdcpSecurityContext_p->integrityAlgoId = 
                            (UInt8)entity_p->integrity.algorithmId;
    entity_p->pdcpSecurityContext_p->oldCiphAlgoId = 
                            (UInt8)entity_p->ciphering.algorithmId;
    entity_p->pdcpSecurityContext_p->newCiphAlgoId = 
                            (UInt8)entity_p->ciphering.newAlgorithmId;
    entity_p->pdcpSecurityContext_p->hfn = entity_p->hfn;
    /* SPR 15236 fix start */
    memCpy( entity_p->pdcpSecurityContext_p->oldCiphKey_p, 
            entity_p->ciphering.key, LTE_SECURITY_KEY_LENGTH );
    memCpy( entity_p->pdcpSecurityContext_p->newCiphKey_p, 
            entity_p->ciphering.newKey, LTE_SECURITY_KEY_LENGTH );
    memCpy( entity_p->pdcpSecurityContext_p->integrityKey_p,
            entity_p->integrity.key, LTE_SECURITY_KEY_LENGTH );
    /* SPR 15236 fix end */
    entity_p->pdcpSecurityContext_p->ciphKeyLen = LTE_SECURITY_KEY_LENGTH;
    /*SPR_16529_review_commet +-*/
    entity_p->pdcpSecurityContext_p->integrityKeyLen = LTE_SECURITY_KEY_LENGTH;
 
    if( PDCP_ENTITY_RB_TX == entity_p->rbDirection )
    {
        /* Fill a pointer of a callback function */
        if( PDCP_ENTITY_SRB == entity_p->type)
        {
            entity_p->pdcpSecurityContext_p->notifyPacket = 
                pdcpProcessTxSrbPktsFrmSec;
        }
        else
        {
            entity_p->pdcpSecurityContext_p->notifyPacket = 
                pdcpProcessTxDrbPktsFrmSec;
        }
        return( pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputTxQ_g) ,
                    entity_p->pdcpSecurityContext_p ) );
    }
    else
    {
        /* Fill a pointer of a callback function */
        if( PDCP_ENTITY_SRB == entity_p->type)
        {
            entity_p->pdcpSecurityContext_p->notifyPacket =
                                            pdcpProcessRxSrbPktsFrmSec;
        }
        else
        {
            entity_p->pdcpSecurityContext_p->notifyPacket = 
                                            pdcpProcessRxDrbPktsFrmSec;
        }
        return( pdcpCreateSecurityContext( (UInt8 *)(&pdcpSecInputRxQ_g), 
                               entity_p->pdcpSecurityContext_p ) );
    }

}
/****************************************************************************
 * Function Name  : pdcpDrbPktForSec
 * Inputs         : ctx_p -
 *                  dataReq_p -
 *                  data_p -
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Appends PDCP header to DRB packets and send the packet to
 *                  security engine
 ****************************************************************************/
UInt32 pdcpDrbPktForSec( LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataReq dataReq_p, void *data_p )
{
    void    *header_p    = PNULL;
    UInt32  result       = PDCP_SUCCESS;
    UInt8   *outData_p   = PNULL;
    UInt32  pdcpSduCount = QCOUNT_PDCP_TX_STORAGE_Q( ctx_p );

    /* Append the header before passing to the security engine */
    header_p = pdcpMakePDUHeaderDRB( dataReq_p->commDataReq.count, 
            ctx_p->snSize, data_p);
    if(PNULL == header_p) {
        LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, 
                dataReq_p->commDataReq.count, ctx_p->snSize, 0,0,__func__,"");
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree(data_p);
        data_p = PNULL;
        pdcpFreeTxDataReq(dataReq_p);
        /*SPR 2498 changes start*/
        dataReq_p = PNULL;
        /*SPR 2498 changes end*/ 
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;           
    }
    /* Allocate memory for output buffer and pass the details to security 
       engine. */
    outData_p = (UInt8 *)msgAlloc(txOutMsgPool_g,0,0,0);
    if( PNULL == outData_p )          
    {
        LOG_PDCP_FATAL( PDCP_TX, "Unable to allocate memory for output "
                "packet, ue[%u] lcid[%u]", dataReq_p->commDataReq.ueIndex,
                 dataReq_p->commDataReq.lcId );
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree(data_p);
        data_p = PNULL;
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        ltePanic("msgAlloc failed in %s", __FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    /* SPR 3406 changes start */
    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_TRUE;
    /* SPR 3406 changes end */
    if ( pdcpSduCount >= MAX_PDCP_SDU_QUEUE_SIZE 
#if defined (ENDC_ENABLED) && !defined (UE_SIM_TESTING)        	
            || (!ctx_p->isTrafficAllowed)
#endif
       )
    {
#if defined (ENDC_ENABLED) && !defined (UE_SIM_TESTING)        	
		if (!ctx_p->isTrafficAllowed)
		{
            LOG_PDCP_MSG( PDCP_TRAFFIC_NOT_ALLOWED, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, pdcpSduCount,
                    0, 0,0, __func__, "");
		}
		else
#endif
        /* Discard packet as storage queue is full */
        LOG_PDCP_MSG( PDCP_RECEIVED_MAX_SDU, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, pdcpSduCount, 
                dataReq_p->commDataReq.count, 0,0, __func__, "");
        /* SPR 14677 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
        /* SPR 14677 fix end */

        msgFree( data_p );
        data_p = PNULL;        
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }
    else
    {
        /*ADD to storage*/
        ENQUEUE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
    }

    /* Pass the shared context, input/output data and meta-data to security
     * engine */
    result = pdcpSecurityProcessPacket( ctx_p->pdcpSecurityContext_p, 
                                      data_p, outData_p, 
                                      (uaContextHandler)dataReq_p );
    if ( PDCP_SUCCESS != result )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                dataReq_p->tickAssociated, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        msgFree(data_p);
        data_p = PNULL;
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpSrbPktForSec
 * Inputs         : ctx_p -
 *                  dataReq_p -
 *                  data_p -
 * Outputs        : 
 * Returns        : SUCCESS/FAILURE
 * Description    : Appends PDCP header to SRB packets and send the packet to
 *                  security engine
 ****************************************************************************/
UInt32 pdcpSrbPktForSec( LP_PdcpEnityContext ctx_p, 
        LP_PdcpSrbDataReq dataReq_p, void *data_p )
{
    void    *header_p = PNULL;
    UInt32  result = PDCP_SUCCESS;
    UInt8   *outData_p = PNULL;
    UInt32  pdcpSduCount = QCOUNT_PDCP_TX_STORAGE_Q( ctx_p );
    
    /* Append the header before passing to the security engine */
    header_p = pdcpMakePDUHeaderSRB( dataReq_p->commDataReq.count, 
            ctx_p->snSize, data_p);
    if(PNULL == header_p) {
        LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, 
                dataReq_p->commDataReq.count, ctx_p->snSize, 0,0,__func__,"");
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree(data_p);
        data_p = PNULL;
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;           
    }
    /* Allocate memory for output buffer and pass the details to security 
       engine. */
    outData_p = (UInt8 *)msgAlloc(txOutMsgPool_g,0,0,0);
    if( PNULL == outData_p )          
    {
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__,0,0,0, 
                dataReq_p->commDataReq.count, 0, 0,
                __func__,"Allocating bytes for ciphered output");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree(data_p);
        data_p = PNULL;
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        ltePanic("msgAlloc failed in %s", __FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    /* SPR 3406 changes start */
    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_TRUE;
    /* SPR 3406 changes end */
    if ( pdcpSduCount >= MAX_PDCP_SDU_QUEUE_SIZE )
    {
        /* Discard packet as storage queue is full */
        LOG_PDCP_MSG( PDCP_RECEIVED_MAX_SDU, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, pdcpSduCount,
                dataReq_p->commDataReq.count, 0,0, __func__, "");
        /* SPR 14677 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 14677 fix end */

        msgFree( data_p );
        data_p = PNULL;        
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }
    else
    {
        /*ADD to storage*/
        ENQUEUE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
    }

    /* Pass the shared context, input/output data and meta-data to security
     * engine */
    result = pdcpSecurityProcessPacket( ctx_p->pdcpSecurityContext_p, 
                                      data_p, outData_p, 
                                      (uaContextHandler)dataReq_p );
    if ( PDCP_SUCCESS != result )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                0, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        msgFree(data_p);
        data_p = PNULL;
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
    }

    return result;
}

/****************************************************************************
 * Function Name  : pdcpProcessTxSrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process TX packets that 
 *                  are received from security engine.
 ****************************************************************************/
Int pdcpProcessTxSrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo)
{
    LP_PdcpSrbDataReq dataReq_p = (uaContextHandler)uaCtxHandle_p;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt16 result = 0;
    /* Coverity 69641 Fix Start*/
#if (defined PERF_STATS) || (defined KPI_STATS)
    UInt32 size = 0;
#endif
    /* Coverity 69641 Fix End*/

    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */
    msgFree( inData_p );
    inData_p = PNULL;
    if( PNULL == dataReq_p )
    {
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__, "Data from SEC");
        msgFree( outData_p );
        outData_p = PNULL;
        return PDCP_FAIL;
    }

    /* SPR 19802 Start */
       /* Code deleted */
    /* SPR 19802 End */

    ctx_p = pdcpGetEntityContext( dataReq_p->commDataReq.ueIndex, 
            dataReq_p->commDataReq.lcId, PDCP_ENTITY_RB_TX );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, 0,0, 0,0,__func__,"");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;

        return PDCP_FAIL;
    }

    if ( SEC_STATUS_ERROR == status )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                status, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }

    if ( SEC_SUCCESS != errInfo )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                errInfo, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        /* Device failure, notify OAM */
        oamCmdNotifyDeviceFailure();
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }
    /* SPR 3406 changes start */
    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_FALSE;
    /* SPR 3406 changes end */

    /* Update statistic data */
    pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].\
        srbs[dataReq_p->commDataReq.lcId - 1].\
        numberOfDlSrbPduTransmitted += 1;


    /* SPR 19802 Start */
    /* + coverity 97049 */
#ifdef KPI_STATS
   InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(dataReq_p->commDataReq.ueIndex); 
   UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats =
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);
    /* SPR 19802 Start */
    size = msgSize(outData_p, PNULL);
    if( (MACI_LEN + 1) < size )
    {
        pLtePdcpKPIStats->dlCtrlPdcpSduBitRate +=
            size - (MACI_LEN + 1);
    }
    else
    {
        pLtePdcpKPIStats->dlCtrlPdcpSduBitRate +=
            size;
    }
    /* SPR 19802 End */
#endif
    /* - coverity 97049 */

#ifdef PERF_STATS 
    LteUePdcpDLPerfStats *lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats*)
        &(gPdcpUePerfStats_p[dataReq_p->commDataReq.ueIndex].
        lteUePdcpDLPerfStats);

/* Coverity 69641 Fix Start*/
    /* Update KPI Counter for total SRB Bytes Transmitted in DL
       MACI_LEN = MACI Header and 1 is equal to PDCP Header Size*/
    size = msgSize(outData_p, PNULL);
    if( (MACI_LEN + 1) < size )
    {
        lteUePdcpDLPerfStats_p->totalDlSrbBytesSent +=
            size - (MACI_LEN + 1);
    }
    else
    {
        lteUePdcpDLPerfStats_p->totalDlSrbBytesSent +=
            size;
    }
#endif
/* Coverity 69641 Fix End*/

    /* SPR 19802 Start */
       /* Code deleted */
    /* SPR 19802 End */


    result = pdcpSrbFinishPduTx( dataReq_p, ctx_p, outData_p);
    return result;
}

/****************************************************************************
 * Function Name  : pdcpProcessTxDrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process TX packets that 
 *                  are received from security engine.
 ****************************************************************************/
Int pdcpProcessTxDrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo)
{
    LP_PdcpDataReq dataReq_p = (uaContextHandler)uaCtxHandle_p;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt16 result = 0;

    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */
    msgFree( inData_p );
    inData_p = PNULL;
    if( PNULL == dataReq_p )
    {
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__, "Data from SEC");
        msgFree( outData_p );
        outData_p = PNULL;
        return PDCP_FAIL;
    }


    ctx_p = pdcpGetEntityContext( dataReq_p->commDataReq.ueIndex, 
            dataReq_p->commDataReq.lcId, PDCP_ENTITY_RB_TX );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, 0,0, 0,0,__func__,"");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;

        return PDCP_FAIL;
    }

    /* SPR 19802 Start */
    /* + coverity 97048 */
#ifdef KPI_STATS
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(dataReq_p->commDataReq.ueIndex);
    UInt8  qci = 0;
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
#endif
    /* - coverity 97048 */
    /* SPR 19802 End */


    /* SPR 16921 fix start */
#ifdef KPI_STATS
    qci = ctx_p->qci;
#endif
    /* SPR 16921 fix end */
    if ( SEC_STATUS_ERROR == status )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                dataReq_p->tickAssociated, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        msgFree( outData_p );
        outData_p = PNULL;
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);

        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
#ifdef KPI_STATS
        /* Update KPI Discard counter in case packet cannot be processed */ 
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++; 
        }
#endif
        /* SPR 16921 fix end */
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }

    if ( SEC_SUCCESS != errInfo )
    {
        /* SPR 16766 fix start */
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                dataReq_p->tickAssociated, 0, 0, __func__, "SecFail");
        /* SPR 16766 fix end */
        /* Device failure, notify OAM */
        oamCmdNotifyDeviceFailure();

        msgFree( outData_p );
        outData_p = PNULL;
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
#ifdef KPI_STATS
        /* Update KPI Discard counter in case packet cannot be processed */ 
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++; 
        }
#endif
        /* SPR 16921 fix end */
        DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;
        return PDCP_FAIL;
    }

    dataReq_p->commDataReq.isPacketWithSecEngine = LTE_FALSE;

    /* Update statistic data */
    pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].\
        drbs[dataReq_p->commDataReq.lcId - 3].\
        numberOfDlDrbPduTransmitted += 1;

#ifdef PERF_STATS 
    LteUePdcpDLPerfStats *lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats*)
        &(gPdcpUePerfStats_p[dataReq_p->commDataReq.ueIndex].
                lteUePdcpDLPerfStats);

    /* Update KPI Counter for total DRB Bytes Transmitted per LC in DL */
    lteUePdcpDLPerfStats_p->totalDlDrbBytesSent
        [dataReq_p->commDataReq.lcId - LTE_DRB_LC_ID_LOW_BOUND] += 
        msgSize(outData_p, PNULL) ;
#endif


    result = pdcpDrbFinishPduTx( dataReq_p, ctx_p, outData_p);
    /* SPR 16921 fix start */
    if ((PDCP_SUCCESS != result) && 
            (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
    {
        /* Update KPI counters if packet is discarded for non-HO scenario */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
    }

    /* SPR 16921 fix end */
    return result;
}

/****************************************************************************
 * Function Name  : pdcpProcessRxSrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process RX SRB packets 
 *                  that are received from security engine.
 ****************************************************************************/
Int pdcpProcessRxSrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo)
{
    LP_PdcpDataInd dataInd_p = (LP_PdcpDataInd)uaCtxHandle_p ;
    LP_PdcpEnityContext ctx_p = PNULL;

    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */
    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */
    msgFree( inData_p );
    inData_p = PNULL;

    if( PNULL == dataInd_p )
    {
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__, "Data from SEC");
        msgFree( outData_p );
        outData_p = PNULL;
        return PDCP_FAIL;
    }

    /* Assign data_p in dataInd_p to PNULL */
    dataInd_p->data_p = PNULL;    
    if ( SEC_STATUS_ERROR == status )
    {
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return PDCP_FAIL;
    }
    if ( SEC_SUCCESS != errInfo )
    {
        /* Device failure, notify OAM */
        oamCmdNotifyDeviceFailure();

        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return PDCP_FAIL;
    }
    ctx_p = pdcpGetEntityContext( dataInd_p->ueIndex, dataInd_p->lcId, 
                                PDCP_ENTITY_RB_RX );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RX,
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                dataInd_p->lcId, 0,0, 0,0,__func__,"");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;

        return PDCP_FAIL;
    }
    if ( 0 == msgRemove(outData_p, 0, 
                        GET_BYTESIZE_FROM_BITSIZE(ctx_p->snSize) ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 1, dataInd_p->count,0,0, __func__,
                "PDCP Header cannot be removed");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        ltePanic("PDCP Header cannot be removed %s",__FUNCTION__);
        
        return PDCP_FAIL;
    }

    dataInd_p->data_p = outData_p;
    if ( SEC_STATUS_SUCCESS == status )
    {
        pdcpSrbFinishPduRx( dataInd_p, ctx_p);
    }
    /*Coverity Fix 26345 */
    else
    {
        /* notify RRC for integrity verification failure */
        /* SPR 3175 Changes Start */
        pdcpSendNotifyIntegrityFailureInd(dataInd_p->ueIndex, dataInd_p->lcId);
        /* SPR 3175 Changes Start */
                    
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
    }
    
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpProcessRxDrbPktsFrmSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to process RX DRB packets 
 *                  that are received from security engine.
 ****************************************************************************/
Int pdcpProcessRxDrbPktsFrmSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo)
{
    LP_PdcpDataInd dataInd_p = (LP_PdcpDataInd)uaCtxHandle_p ;
    LP_PdcpEnityContext ctx_p = PNULL;
    /* Coverity # 62873*/

    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */

    /* Free the input buffer as cloned copy is already saved before sending the
     * data to security engine */
    msgFree( inData_p );
    /* +- coverity 69506  */

    if( PNULL == dataInd_p )
    {
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                __LINE__, 0,0,0,0, 0,0, __func__, "Data from SEC");
        msgFree( outData_p );
        outData_p = PNULL;
        return PDCP_FAIL;
    }
    /* Assign data_p in dataInd_p to PNULL */
    dataInd_p->data_p = PNULL;   
    if ( SEC_STATUS_ERROR == status )
    {
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return PDCP_FAIL;
    }
    if ( SEC_SUCCESS != errInfo )
    {
        /* Device failure, notify OAM */
        oamCmdNotifyDeviceFailure();

        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        return PDCP_FAIL;
    }
    
    ctx_p = pdcpGetEntityContext( dataInd_p->ueIndex, dataInd_p->lcId, 
                                PDCP_ENTITY_RB_RX );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RX,
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex,
                dataInd_p->lcId, 0,0, 0,0,__func__,"");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;

        return PDCP_FAIL;
    }

    if ( 0 == msgRemove(outData_p, 0, 
                        GET_BYTESIZE_FROM_BITSIZE(ctx_p->snSize) ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, dataInd_p->ueIndex, 
                dataInd_p->lcId, 1, dataInd_p->count,0,0, __func__,
                "PDCP Header cannot be removed");
        msgFree( outData_p );
        outData_p = PNULL;
        pdcpFreeRxDataInd(dataInd_p);
        dataInd_p = PNULL;
        ltePanic("PDCP Header cannot be removed %s",__FUNCTION__);
        
        return PDCP_FAIL;
    }

    dataInd_p->data_p = outData_p;    
    if ( PNULL != ctx_p->rohc.context) 
    {
        /* Decompression needs to be performed, hence enqueue in a ciphering
         * queue */
        /* + coverity 30573 */
        if(  1 == ENQUEUE_PDCP_DECIPHERED_RX_Q(dataInd_p) )
        {            
            pdcpFreeRxDataInd(dataInd_p);
            dataInd_p = PNULL;
        }
        /* - coverity 30573 */
    }
    else
    {
        /* No Decompression required, directly send the data to upper layer */
        pdcpDrbFinishPduRx( dataInd_p, ctx_p);
    }
    return PDCP_SUCCESS;
}

/* + Coverity 65114, 65115 */
/****************************************************************************
 * Function Name  : pdcpFreeTxDataFromSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to free the TX data from SEC
 *                  when context is not located by ASYNC thread.
 ****************************************************************************/
/* SPR 21800 Fix + */
Int pdcpFreeTxDataFromSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo
        )
/* SPR 21800 Fix - */
{
    msgFree( inData_p );
    inData_p = PNULL;
    msgFree( outData_p );
    outData_p = PNULL;
    /* Since context is already removed, hence meta-data for TX entity which 
     * would have been stored in the storage queue is already removed. */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpFreeRxDataFromSec
 * Inputs         : inData_p  - pointer to input data.
 *                  outData_p  - pointer to output data.
 *                  uaCtxHandle - Pointer to meta-data.
 *                  status - status of the packet after processing by the 
 *                           security engine.
 *                  errInfo - error indications if any.
 * Outputs        : None.
 * Returns        : SUCCESS/FAILURE.
 * Description    : This API will be used by PDCP to free the RX data from SEC
 *                  when context is not located by ASYNC thread.
 ****************************************************************************/
/* SPR 21800 Fix + */
Int pdcpFreeRxDataFromSec(
        void *inData_p,
        void *outData_p,
        uaContextHandler uaCtxHandle_p,
        secStatus status,
        UInt32 errInfo
        )
/* SPR 21800 Fix - */
{
    LP_PdcpDataInd dataInd_p = (LP_PdcpDataInd)uaCtxHandle_p ;

    msgFree( inData_p );
    inData_p = PNULL;
    msgFree( outData_p );
    outData_p = PNULL;

    pdcpFreeRxDataInd( dataInd_p );
    dataInd_p = PNULL;

    return PDCP_SUCCESS;
}
/* + Coverity 65114, 65115 */
#endif
