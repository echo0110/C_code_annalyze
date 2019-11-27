/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpDataTransferTx.c,v 1.2.12.2.2.1 2010/09/21 15:49:15 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Data transfer TX routines implementation.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpDataTransferTx.c,v $
 * Revision 1.2.12.2.2.1  2010/09/21 15:49:15  gur20491
 * FAPI changes
 *
 * Revision 1.2.10.1  2010/08/10 12:11:46  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2  2010/02/13 10:39:08  gur15697
 * added pdcp thread timing logs
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.11.2.7  2009/07/20 08:21:55  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.11.2.6  2009/07/15 06:11:23  gur19140
 * changes during intg cycle1 for downlink
 *
 * Revision 1.11.2.5  2009/07/13 11:10:46  gur20435
 * *** empty log message ***
 *
 * Revision 1.11.2.4  2009/07/13 11:01:31  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.11.2.3  2009/07/10 09:07:18  gur19140
 * optimization changes for zcb
 *
 * Revision 1.11.2.2  2009/07/08 08:42:41  gur19836
 * Tx flow Optimization changes
 *
 * Revision 1.11.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.11  2009/05/28 13:47:39  gur19140
 * :wq
 *
 * Revision 1.10  2009/05/26 15:00:56  gur19140
 * *** empty log message ***
 *
 * Revision 1.9  2009/05/22 09:20:52  gur19836
 * ROHC Changes merged
 *
 * Revision 1.8  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.7  2009/05/20 13:40:56  gur20548
 * header file name changed
 *
 * Revision 1.6  2009/05/15 12:37:37  gur19836
 * Compilation error removed
 *
 * Revision 1.5  2009/05/15 11:23:05  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteFramework.h"
#include "rohc_ex.h"
#include "rohc_typ.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpTSDefs.h"
#include "ltePdcpErrors.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpRlcSap.h"
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpRrcSap.h"
#include "ltePdcpDiscardTimer.h"
#include "integrity.h"
#include "ltePdcpStatistic.h"
#include "rohc_main_ex.h"
#include "confidentiality.h"
#include "logging.h"
#include "alarm.h"
/* SPR 3607 Changes Start */
#include "ltePdcpTlvUtils.h"
/* SPR 3607 Changes End */



/******************************************************************************
  Private Definitions
 *****************************************************************************/

/* SPR 3607 Changes Start */
#define MAX_THRESHOLD_VALUE_FOR_WRAPAROUND 0xFFFFFFA0
/* SPR 3607 Changes End */
/* SPR 14622 fix start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
#define RLC_DISCARD_SUCCESS 3
#endif

/* SPR 14622 fix end */
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifndef PDCP_ASYNC_INTERFACE
static UInt32 pdcpProcessCiphering(LP_PdcpCommDataReq dataReq_p, 
        LP_PdcpEnityContext ctx_p,
        void    *data_p,
        UInt8   *ciphering_p );

static inline UInt32 pdcpGenIntegrityTx( LP_PdcpEnityContext ctx_p,
    LP_PdcpSrbDataReq dataReq_p, void *data_p, UInt8 *pdcpProcessSrbTxMacI_p);
#endif

#ifdef L2_FAST_PATH
UInt32 pdcpProcessSduTx(LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataReq dataReq_p, void *data_p);
#else 
static UInt32 pdcpProcessSduTx(LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataReq dataReq_p, void *data_p);
#endif

static inline void pdcpHandleControlPduIndRx ( UInt8 type, 
    LP_PdcpControlPduInd controlPduInd_p, LP_PdcpEnityContext ctx_p, 
    LP_PdcpUeContext ueCtx_p, LP_PdcpEnityContext rx_ctx_p);
#ifndef UE_SIM_TESTING
STATIC inline UInt32 pdcpProcessSpsTxSdu( LP_PdcpEnityContext ctx_p,
    LP_PdcpDataReq dataReq_p, void *data_p);
#endif



/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SRP 3608 changes start */
extern void* pdcpMakePDUHeaderControlPdu(UInt8 ctrl_type, UInt16);
/* SPR 3608 changes end */

/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */

extern PdcpContext pdcpContext;

extern UInt8 *pdcpProcessSrbTxMacI_p;


/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/* SPR 3607 Changes Start */
UInt32 snhfnCountThreshold_g = MAX_THRESHOLD_VALUE_FOR_WRAPAROUND;
/* SPR 3607 Changes End */
/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpRetransmitTx
 * Inputs         : ueIndex, lcId, state, event, PDCP Context
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code 
 * Description    : performs Retransmission of SDUs for which ACK is not 
 *                  recvd from RLC.
 ****************************************************************************/
UInt32 pdcpRetransmitTx( UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p)
{
    LP_PdcpEnityContext ctx_p = (LP_PdcpEnityContext)context_p;
    LP_PdcpDataReq dataReq_p = PNULL;
    void      *data_p = PNULL;
    UInt32    result  = PDCP_SUCCESS;
    /* SPR 16921 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    UInt8  qci = ctx_p->qci;
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
#endif
    /* SPR 16921 fix end */

    DEQUEUE_PDCP_TX_STORAGE_Q ( ctx_p, dataReq_p );
    if (PNULL == dataReq_p)
    {
        /*Nothing to process*/
        /* +- SPR 17777*/
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId,state,event, 0,0, __func__,"pdcpStorageQ");
        return PDCP_FAIL;
    }

    data_p = dataReq_p->data_p;
    if (PNULL == data_p) 
    {
        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count,
                0, 0,0, __func__,"");
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[ueIndex].drbs[lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
        if(qci != 0)
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
        /* SPR 16921 fix end  */
        pdcpFreeTxDataReq(dataReq_p);
        return PDCP_FAIL;
    }    

    /* Process SDU for RoHC and ciphering */
    if( PDCP_ENTITY_RLC_UM == ctx_p->rlcMode )
    {
       pdcpAssociateSnHfn(ctx_p, &(dataReq_p->commDataReq));
    }
    result = pdcpProcessSduTx(ctx_p, dataReq_p, data_p);        
    /* SPR 16921 fix start */   
    if ((PDCP_SUCCESS != result) && 
            (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
    {
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if(qci != 0)
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
    }
    /* SPR 16921 fix end  */
    
    return result;
}

/****************************************************************************
 * Function Name  : processDeliveryStatusInd 
 * Inputs         : deliveryInd_p.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function handles delivery status information from RLC layer.
 ****************************************************************************/ 
void processDeliveryStatusInd(LP_PdcpDeliveryInd deliveryInd_p)
{
    LP_PdcpDataReq dataReq_p = PNULL;
    LP_PdcpSrbDataReq srbDataReq_p = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt16 startVar=0;
    /* + SPR 19066 */
    LP_PdcpUeContext    ueCtx_p = PNULL;
    /* - SPR 19066 */
    CHECK_PNULL(deliveryInd_p);

    ctx_p = pdcpGetEntityContext(deliveryInd_p->ueIndex, deliveryInd_p->lcId,
                                 PDCP_ENTITY_RB_TX);
    if(PNULL == ctx_p)
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                 pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                 deliveryInd_p->lcId,0,0,0,0,__func__,"");
        return;
    }
    /*Extract QCI from Entity Context and DRB state from UE context*/
/*spr 19296 fix start*/
	/*Code deleted*/
/*spr 19296 fix end*/

    if ( PDCP_ENTITY_DRB == ctx_p->type)
    {
        for (startVar=0;startVar<deliveryInd_p->count;startVar++)
        {
            /* SPR 14622 fix start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
            UInt8 isDlDrbSdusduDiscarded = LTE_FALSE;

            if(deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus == RLC_DISCARD_SUCCESS)
            {
                /* Update statistic data */
                pdcpStats.statsUEs[deliveryInd_p->ueIndex].drbs[ \
                   deliveryInd_p->lcId - 3].numberOfDlDrbSduDiscarded += 1;
#ifdef KPI_STATS
		/*SPR 16164 fix start*/
                InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(deliveryInd_p->ueIndex);
                UInt32 activeIndexForPdcpStats = (gMacStats.
                        pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
                LtePdcpKPIStats * pLtePdcpKPIStats = 
                    &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[activeIndexForPdcpStats]);
		/*SPR 16164 fix end*/
                UInt8 qci = ctx_p->qci;
                /* Update KPI counters in case of discard timer timeout
                */
                if (0 != qci)
                {
                    pLtePdcpKPIStats->\
                        totalDlDrbDiscardRatePerQci[qci - 1]++; 
                }
#endif
                deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus = PDCP_RLC_ACK;
                isDlDrbSdusduDiscarded = LTE_TRUE;
            }
#endif
            /* SPR 14622 fix end */
            switch( deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus )
            {
                case PDCP_RLC_ACK :
                {
                    /* SPACC_OPTIMIZATION */
                    dataReq_p = (LP_PdcpDataReq)( deliveryInd_p->
                            deliveryStatusArr[startVar].transactionId );
                    /* Count to pointer changes end*/
                    if( PNULL == dataReq_p )
                    {
                        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                                deliveryInd_p->lcId, deliveryInd_p->count, 0, 0,0, 
                                __func__, "dataReq_p is NULL");
                        continue;
                    }

                    /*spr 19296 fix start*/
                    /*Code deleted*/
                    /*spr 19296 fix end*/
                    /* Count to pointer changes start*/
                    DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);            
                    /* Count to pointer changes end*/
                    /* + SPR 19066 */
                    /* Get Valid UE Context */
                    ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(
                            deliveryInd_p->ueIndex );
                    /* Update the last scheduled tick in DL */
                    ueCtx_p->lastSchedTickDl = pdcpCurrentTime_g;
                    /* Reset lapsedInactiveTime as data is received */
                    ueCtx_p->lapsedInactiveTime = 0;
                    /* - SPR 19066 */

                    /*Send notification to upper layer*/
                    LOG_PDCP_MSG (PDCP_STATUS_IND_GTPU, LOGINFO, PDCP_TX, 
                            pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                            deliveryInd_p->lcId, dataReq_p->commDataReq.count, 
                            PDCP_DELIVERY_STATUS_SUCCESS, 0,0, __func__, "");

                     /*coverity 31998*/
                    if(deliveryInd_p->ueIndex < MAX_UE_SUPPORTED)
                    {
                        pdcpDataStatusInd(deliveryInd_p->ueIndex, deliveryInd_p->lcId,
                                PDCP_DELIVERY_STATUS_SUCCESS );

                        pdcpStats.statsUEs[deliveryInd_p->ueIndex].drbs[ \
                        deliveryInd_p->lcId - 3].numberOfDrbDeliveryIndSent += 1;
                    }
                     /*coverity 31998*/
                    /* SPR 14622 fix start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
                    if(isDlDrbSdusduDiscarded)
                    {
                       LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, 
                               PDCP_TX, pdcpCurrentTime_g, __LINE__, 
                               dataReq_p->commDataReq.ueIndex, dataReq_p->commDataReq.lcId, 
                               dataReq_p->commDataReq.count, dataReq_p->tickAssociated, 
                               /* SPR 16766 fix start */
                               0, 0, __func__, "TimerExpiry");
                       /* SPR 16766 fix end */
 
                        isDlDrbSdusduDiscarded = LTE_FALSE;
                    }
#endif
                    /* SPR 14622 fix end */
                    /* Fixed SPR 925 ----START */
                    pdcpFreeTxDataReq(dataReq_p);
                    /*SPR 2498 changes start*/
                    dataReq_p = PNULL;
                    /*SPR 2498 changes end*/                   
                    break;
                }
                case PDCP_RLC_NACK:
                {
                    /* RLC NACK is received */
                    /* No need to search and delete the node from queue as it has 
                     * to be forwarded to target PDCP or needs to be re-transmitted
                     * in case of re-establishment */                
                    /* SPR 20636 Changes Start */
                    LOG_PDCP_MSG( PDCP_STATUS_IND_GTPU, LOGINFO, PDCP_TX, 
                            pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                            deliveryInd_p->lcId, 
                            (ADDR)(deliveryInd_p->deliveryStatusArr[startVar].\
                                transactionId),
                            PDCP_DELIVERY_STATUS_FAILURE, 0,0, __func__, "");
                    /* SPR 20636 Changes End */

                    pdcpDataStatusInd(deliveryInd_p->ueIndex,deliveryInd_p->lcId,
                            PDCP_DELIVERY_STATUS_FAILURE );
                    /* Update statistic data */
                    pdcpStats.statsUEs[deliveryInd_p->ueIndex].drbs[ \
                        deliveryInd_p->lcId - 3].numberOfDlDrbPduDropForNack += 1;
                    break;
                }
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)   
                case PDCP_RLC_Q_FULL:
                {
                    /* Count to pointer changes start*/
                    dataReq_p = (LP_PdcpDataReq)( deliveryInd_p->
                            deliveryStatusArr[startVar].transactionId );
                    if( PNULL == dataReq_p )
                    {
                        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                                deliveryInd_p->lcId, deliveryInd_p->count, 0, 0,0, 
                                __func__, "dataReq_p is NULL");
                        continue;
                    }
                    /* SPR 14677 fix start */
                    /* Update statistic data */
                    pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                        drbs[dataReq_p->commDataReq.lcId - 3].
                        numberOfDlDrbSduDiscarded += 1;
                    /* SPR 14677 fix end */
                    /* Count to pointer changes start*/
                    DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                    /* Count to pointer changes end */
                    pdcpFreeTxDataReq(dataReq_p);
                    dataReq_p = PNULL;
                    break;
                }
#endif
                default:
                {
                    break;
                }
            }
        }
    }
    else
    {
        for (startVar=0;startVar<deliveryInd_p->count;startVar++)
        {
            /* Count to pointer changes start*/
            srbDataReq_p = (LP_PdcpSrbDataReq)(
                    deliveryInd_p->deliveryStatusArr[startVar].transactionId );
            if( PNULL == srbDataReq_p )
            {
                LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX,
                        pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                        deliveryInd_p->lcId, deliveryInd_p->count, 0, 0,0,
                        __func__, "srbDataReq_p is NULL");
                continue;
            }
            /* Count to pointer changes start*/
            DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, srbDataReq_p);
            /* Count to pointer changes end */
            switch( deliveryInd_p->deliveryStatusArr[startVar].deliveryStatus )
            {
                case PDCP_RLC_ACK:
                {
                    LOG_PDCP_MSG (PDCP_STATUS_IND_RRC, LOGINFO, PDCP_TX, 
                            pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                            deliveryInd_p->lcId, srbDataReq_p->srbServiceReqested, 
                            PDCP_SUCCESS, 0,0, __func__, "");
                    if ( LTE_TRUE == srbDataReq_p->srbServiceReqested )
                    {
                        /* SPR 1825 changes start */
                        pdcpRrcSrbDataStatusInd( 0, deliveryInd_p->ueIndex, 
                                deliveryInd_p->lcId,
                                srbDataReq_p->transactionId, PDCP_SUCCESS, 
                                PDCP_SUCCESS, srbDataReq_p->data_p,
                                LTE_TRUE,  msgSize(srbDataReq_p->data_p, PNULL));
                        /* SPR 1825 changes end */
                        pdcpStats.statsUEs[srbDataReq_p->commDataReq.ueIndex].srbs[ 
                            srbDataReq_p->commDataReq.lcId - 1].numberOfSrbDeliveryIndSent += 1;
                    }
                    break;
                }
                case PDCP_RLC_NACK:
                {

                    LOG_PDCP_MSG( PDCP_STATUS_IND_RRC, LOGINFO, PDCP_TX, 
                            pdcpCurrentTime_g, __LINE__, deliveryInd_p->ueIndex,
                            deliveryInd_p->lcId, srbDataReq_p->srbServiceReqested, 
                            PDCP_ERR_DT_RLC_NACK, 0,0, __func__, "");
                    if ( LTE_TRUE == srbDataReq_p->srbServiceReqested )
                    {
                        /* SPR 1825 changes start */
                        pdcpRrcSrbDataStatusInd( 0, deliveryInd_p->ueIndex, 
                                deliveryInd_p->lcId, 
                                srbDataReq_p->transactionId, PDCP_FAIL, 
                                PDCP_ERR_DT_RLC_NACK, srbDataReq_p->data_p,
                                LTE_TRUE,  msgSize(srbDataReq_p->data_p, PNULL));
                        /* SPR 1825 changes end */
                    }
                    /* Update statistic data */

                    pdcpStats.statsUEs[srbDataReq_p->commDataReq.ueIndex].srbs[ \
                        srbDataReq_p->commDataReq.lcId - 1].numberOfDlSrbPduDropNackRecvd += 1;
                    break;
                }
                default:
                {
                    break;
                }
            }
            pdcpFreeSrbTxDataReq(srbDataReq_p);
            srbDataReq_p = PNULL;
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpSrbFinishPduTx
 * Inputs         : LP_PdcpSrbDataReq dataReq_p,LP_PdcpEnityContext ctx_p,
 *                  void *data_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Stores and transmit PDU to RLC, depending on AM/UM mode.
 ****************************************************************************/
UInt32 pdcpSrbFinishPduTx(LP_PdcpSrbDataReq dataReq_p,
        LP_PdcpEnityContext ctx_p, void *data_p)
{
    UInt32 result     = PDCP_SUCCESS;
#ifndef PDCP_ASYNC_INTERFACE
    UInt32 pdcpSduCount = QCOUNT_PDCP_TX_STORAGE_Q( ctx_p );
#endif

    /* SPR 3607 Fix Start */
    if(PDCP_SECURITY_OFFSET_ENABLED ==  ctx_p->securityOffset)
    {
        UInt8 lcCtr = 0;
        /*SPR 10446 fix start */
        
        /* Apply New Integrity/Ciphering Parameters to SRB's */ 
        for ( lcCtr = LTE_SRB_LC_ID_LOW_BOUND;
                lcCtr <= LTE_SRB_LC_ID_HIGH_BOUND; lcCtr++)
        {
            /* configure Integrity */
            pdcpEntityConfigureIntegrity(
                    dataReq_p->commDataReq.ueIndex,
                    lcCtr,
                    ctx_p->new_integrity.algorithmId,
                    ctx_p->new_integrity.key,
                    PDCP_SECURITY_OFFSET_DISABLED );

            /* configure Ciphering */
            pdcpEntityConfigureSrbCiphering(
                    dataReq_p->commDataReq.ueIndex,
                    lcCtr,
                    ctx_p->new_ciphering.algorithmId,
                    ctx_p->new_ciphering.key,
                    PDCP_SECURITY_OFFSET_DISABLED );
            /*SPR 10446 fix end */
        }
    }
    /* SPR 3607 Fix End */

    do
    {
        if ( 0 != ctx_p->discardTimer )
        {
            if ( (dataReq_p->tickAssociated + ctx_p->discardTimer) < 
                    pdcpCurrentTime_g)

            {
                /* Update statistic data */
                pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                    srbs[dataReq_p->commDataReq.lcId - 1].
                    numberOfDlSrbSduDiscarded += 1;
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, 
                        dataReq_p->commDataReq.ueIndex, 
                        dataReq_p->commDataReq.lcId, 
                        dataReq_p->commDataReq.count, 
                        dataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        /* SPR 16766 fix start */
                        "TimerExpiry");
                /* SPR 16766 fix end */
                /* Discard SDU */            
                msgFree(data_p);
                data_p = PNULL;	
#ifdef PDCP_ASYNC_INTERFACE
                /* Delete the node from the storage queue in case of AM mode*/
                DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                /* SPR 15520 fix start */
#else
                result = PDCP_FAIL;
                /* SPR 15520 fix end */
#endif
                pdcpFreeSrbTxDataReq(dataReq_p);
                /*SPR 2498 changes start*/
                dataReq_p = PNULL;	
                /*SPR 2498 changes end*/ 
                break;
            }
        }
        /*Pass PDU to the RLC layer*/
#ifndef PDCP_ASYNC_INTERFACE
        if ( pdcpSduCount >= MAX_PDCP_SDU_QUEUE_SIZE )
        {
            /* Discard packet as storage queue is full */
            LOG_PDCP_MSG( PDCP_RECEIVED_MAX_SDU, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, 
                    dataReq_p->commDataReq.ueIndex,
                    dataReq_p->commDataReq.lcId, pdcpSduCount, 
                    dataReq_p->commDataReq.count, 0,0, __func__, "");
            /* SPR 14677 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                srbs[dataReq_p->commDataReq.lcId - 1].
                numberOfDlSrbSduDiscarded += 1;
            /* SPR 14677 fix end */
            /* SPR 15520 fix start */
            result = PDCP_FAIL;
            /* SPR 15520 fix end */

            msgFree( data_p );
            data_p = PNULL;	
            pdcpFreeSrbTxDataReq(dataReq_p);
            dataReq_p = PNULL;	
        }
        else
        {
            /*ADD to storage*/
            ENQUEUE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
#endif
            LOG_UT( PDCP_PDU_FROM_PDCP_TO_RLC , LOGDEBUG, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, 
                    dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count,
                    0, 0,0,__func__,"");
            
            /*!!! Please keep in mind that data_p should be released in 
              rlcUeDataReq in any case */              

#ifdef LINUX_PC_TEST
            extern UInt32 compose_and_send_pdcp_rlc_ue_data_req(UInt16  ueIndex, 
                                                       UInt16  lcId,
                                                       UInt8*  data_p,
                                                       UInt16  size,
                                                       UInt8   transactionIdPresent,
                                                       void*   transId,
                                                       UInt16  serviceRequested,
                                                       QTIME   pktArrTime,
                                                       UInt8   ipPktType,
                                                       UInt8   voiceSilenceType,
                                                       UInt32  avgPduSize);
            QTIME    currentTime;
   
            result = compose_and_send_pdcp_rlc_ue_data_req(
                            dataReq_p->commDataReq.ueIndex, 
                            dataReq_p->commDataReq.lcId,
                            data_p,
                            msgSize(data_p, PNULL),
                            PDCP_RLC_TRANSACTION_ID_VALID,
                            (void *)(dataReq_p),
                            PDCP_RLC_REQ_ACK_TRUE /*AM*/,
                            currentTime,
                            NOT_DETERMINED,
                            PDCP_NO_PACKET, 0 );

            return PDCP_SUCCESS;
#endif

            result = sendrlcUeDataReqToRlc(
                    dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId,
                    data_p,
                    msgSize(data_p, PNULL),
                    PDCP_RLC_TRANSACTION_ID_VALID,
                    /* Count to pointer changes start*/
                    /* SPR 2812 changes start */
                    (void *)(dataReq_p),
/* SPACC_OPTIMIZATION */
                    /* SPR 2812 changes end */
                    /* Count to pointer changes end*/
                    PDCP_RLC_REQ_ACK_TRUE /*AM*/,
                    dataReq_p->tickAssociated,
                    NOT_DETERMINED,
                    PDCP_NO_PACKET, 0 );
            /* SPR 4994 Fix Start */
            if ((PDCP_SUCCESS != result) && 
                    (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
                /* SPR 4994 Fix End */
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, 
                        dataReq_p->commDataReq.ueIndex, 
                        dataReq_p->commDataReq.lcId, 
                        dataReq_p->commDataReq.count, 
                        dataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        "Rlcfail");
                /* SPR 16766 fix end */
                /* SPR 14677 fix start */
                /* Update statistic data */
                pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                    srbs[dataReq_p->commDataReq.lcId - 1].
                    numberOfDlSrbSduDiscarded += 1;
                /* SPR 14677 fix end */
                /*In rlcUeDataReq failed - delete from storage*/
                DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                pdcpFreeSrbTxDataReq(dataReq_p);
                dataReq_p = PNULL;	
            }
#ifndef PDCP_ASYNC_INTERFACE
        }
#endif
    } while (0);
    return result;
}

/****************************************************************************
 * Function Name  : pdcpDrbFinishPduTx
 * Inputs         : LP_PdcpDataReq dataReq_p,LP_PdcpEnityContext ctx_p,
 *                  void *data_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Stores and transmit PDU to RLC, depending on AM/UM mode.
 ****************************************************************************/
UInt32 pdcpDrbFinishPduTx(LP_PdcpDataReq dataReq_p,
        LP_PdcpEnityContext ctx_p, void *data_p)
{
    UInt32 result     = PDCP_SUCCESS;
    UInt32 avgPduSize = 0;
    UInt32 pduSize    = 0;
#ifndef PDCP_ASYNC_INTERFACE
    UInt32 pdcpSduCount = QCOUNT_PDCP_TX_STORAGE_Q( ctx_p );
#endif

    do
    {
        if ( 0 != ctx_p->discardTimer )
        {
            if ( (dataReq_p->tickAssociated + ctx_p->discardTimer) < 
                    pdcpCurrentTime_g)
            {
                /* Update statistic data */
                pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].\
                    drbs[dataReq_p->commDataReq.lcId - 3].numberOfDlDrbSduDiscarded += 1;
                    /* +- SPR 16921 */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                        dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                        dataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        /* SPR 16766 fix start */
                        "TimerExpiry" );
                        /* SPR 16766 fix end */
                /* Discard SDU */            
                msgFree(data_p);
                data_p = PNULL;	
#ifdef PDCP_ASYNC_INTERFACE
                /* Delete the node from the storage queue in case of AM mode*/
                DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                /* SPR 15520 fix start */
                /* SPR 16921 fix start */
#endif
                result = PDCP_FAIL;
                /* SPR 16921 fix end */
                /* SPR 15520 fix start */
                pdcpFreeTxDataReq(dataReq_p);
                /*SPR 2498 changes start*/
                dataReq_p = PNULL;
                /*SPR 2498 changes end*/ 
                break;
            }
        }
        pduSize = msgSize(data_p, PNULL);
        /*Pass PDU to the RLC layer*/
#ifndef PDCP_ASYNC_INTERFACE
        if (( pdcpSduCount >= MAX_PDCP_SDU_QUEUE_SIZE )
#if defined (ENDC_ENABLED) && !defined (UE_SIM_TESTING)        	
			|| (!ctx_p->isTrafficAllowed)
#endif
			)
        {
#if defined (ENDC_ENABLED) && !defined (UE_SIM_TESTING)        	
            if (!ctx_p->isTrafficAllowed)
            {
                LOG_PDCP_MSG( PDCP_TRAFFIC_NOT_ALLOWED, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                    dataReq_p->commDataReq.lcId, pdcpSduCount,
                    0, 0,0, __func__, "");
			}
			else
#endif
            /* Discard packet as storage queue is full */
            LOG_PDCP_MSG( PDCP_RECEIVED_MAX_SDU, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                    dataReq_p->commDataReq.lcId, pdcpSduCount, dataReq_p->commDataReq.count, 
                    0,0, __func__, "");

            /* SPR 14677 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                drbs[dataReq_p->commDataReq.lcId - 3].
                numberOfDlDrbSduDiscarded += 1;
            /* SPR 14677 fix end */
            /* SPR 15520 fix start */
            result = PDCP_FAIL;
            /* SPR 15520 fix start */
            msgFree( data_p );
            data_p = PNULL;	
            pdcpFreeTxDataReq(dataReq_p);
            dataReq_p = PNULL;	
        }
        else
        {
            /*ADD to storage*/
            ENQUEUE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
#endif
            LOG_UT( PDCP_PDU_FROM_PDCP_TO_RLC , LOGDEBUG, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 0, 0,0,__func__,"");
            /*!!! Please keep in mind that data_p should be released in 
              rlcUeDataReq in any case */
            {
#ifdef LINUX_PC_TEST

            extern UInt32 compose_and_send_pdcp_rlc_ue_data_req(UInt16  ueIndex, 
                                                       UInt16  lcId,
                                                       UInt8*  data_p,
                                                       UInt16  size,
                                                       UInt8   transactionIdPresent,
                                                       void*   transId,
                                                       UInt16  serviceRequested,
                                                       QTIME   pktArrTime,
                                                       UInt8   ipPktType,
                                                       UInt8   voiceSilenceType,
                                                       UInt32  avgPduSize);

                QTIME    currentTime;
                result = compose_and_send_pdcp_rlc_ue_data_req(
                    dataReq_p->commDataReq.ueIndex,
                    dataReq_p->commDataReq.lcId,
                    data_p,
                    pduSize,
                    PDCP_RLC_TRANSACTION_ID_VALID, 
                    (void*)dataReq_p,
                    PDCP_RLC_REQ_ACK_TRUE, 
                    currentTime,
                    ctx_p->ipPktType,
                    dataReq_p->commDataReq.voiceSilenceType,
                    avgPduSize);

                return PDCP_SUCCESS;
#endif
                result = sendrlcUeDataReqToRlc(
                        dataReq_p->commDataReq.ueIndex, 
                        dataReq_p->commDataReq.lcId,
                        data_p,
                        pduSize,
                        PDCP_RLC_TRANSACTION_ID_VALID,
                    /* Count to pointer changes start*/
                    /* SPR 2812 changes start */
                        (void *)(dataReq_p),
                    /* SPR 2812 changes end */
                    /* Count to pointer changes end*/
                        PDCP_RLC_REQ_ACK_TRUE /*AM*/,
                        dataReq_p->tickAssociated,
                        ctx_p->ipPktType,
                    /* SPR 20570 Fix Start */
                        dataReq_p->commDataReq.voiceSilenceType,
                    /* SPR 20526 Fix End */
                        avgPduSize );
	    /* SPR 5178 Changes End */
            }
            /* SPR 4994 Fix Start */
            /*SPR 19094 Fix Start*/ 
		/*Code removed under flag INTEL_T3300_ICCOM_RLC_PDCP_SPLIT*/
	    /*SPR 19094 Fix End*/
            if ((PDCP_SUCCESS != result) && 
                    (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
                /* SPR 4994 Fix End */
            {
                /* SPR 16766 fix start */
                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                        pdcpCurrentTime_g, __LINE__, 
                        dataReq_p->commDataReq.ueIndex, 
                        dataReq_p->commDataReq.lcId, 
                        dataReq_p->commDataReq.count, 
                        dataReq_p->tickAssociated
                        , 0, 0, __func__, 
                        "Rlcfail");
                /* SPR 16766 fix end */
                /* SPR 14677 fix start */
                /* Update statistic data */
                pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                    drbs[dataReq_p->commDataReq.lcId - 3].
                    numberOfDlDrbSduDiscarded += 1;
                /* SPR 14677 fix end */
                /* +- SPR 16921 */
                /*In rlcUeDataReq failed - delete from storage*/
                DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                pdcpFreeTxDataReq(dataReq_p);
                dataReq_p = PNULL;	
            }
#ifndef PDCP_ASYNC_INTERFACE
        }
#endif
    } while (0);
    return result;
}

/****************************************************************************
 * Function Name  : pdcpAssociateSnHfn 
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Associates Count = HFN + SN to a packet
 ****************************************************************************/
void pdcpAssociateSnHfn(LP_PdcpEnityContext ctx_p,
        LP_PdcpCommDataReq dataReq_p)
{

/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    LP_PdcpUeContext    ueCtx_p = PNULL;
    InternalCellIndex   cellIndex = INVALID_CELL_INDEX ;
#endif

    /* Discard timer already associated with newly arrived 
     * packet. So removed code from here */

    dataReq_p->count = PDCP_MAKE_COUNT( ctx_p->next_SN, ctx_p->hfn, 
            ctx_p->snSize);
    if( PDCP_ENTITY_RLC_AM == ctx_p->rlcMode )
    {
        dataReq_p->sn = ctx_p->next_SN;
    }
    LOG_UT( PDCP_SDU_SN_HFN_TX, LOGINFO, PDCP_TX, 
             pdcpCurrentTime_g, dataReq_p->ueIndex, dataReq_p->lcId,
             dataReq_p->sn, ctx_p->hfn, dataReq_p->count, 0,0, __func__, "");
    PDCP_INCREMENT_SN(&ctx_p->next_SN,
                      &ctx_p->hfn,
                      ctx_p->snSize);

#ifndef UE_SIM_TESTING
    /* Get UE context*/
    ueCtx_p = PDCP_GET_VALID_UE_CONTEXT( dataReq_p->ueIndex );
    if ( PNULL == ueCtx_p )
    {
        LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex, 
                pdcpContext.isInitialized, 0, 0, 0,0, __func__,"");
        return;
    }

    cellIndex = ueCtx_p->cellIndex;

    if(dataReq_p->count == pdcpContext.sCellInfo[cellIndex].thresholdValue)
    {
        pdcpSendCountWraparoundInd(dataReq_p->ueIndex);
    }
#endif
}

/******************************************************************************
 * Function Name  : pdcpProcessSduTx
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 *                  data_p -
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Performs operations after count is associated to the packet
 *****************************************************************************/
UInt32 pdcpProcessSduTx(LP_PdcpEnityContext ctx_p, 
        LP_PdcpDataReq dataReq_p, void *data_p)
{
    UInt8     *out_buff_p = PNULL;
    UInt32    eCode = PDCP_SUCCESS;
    rohc_api_info_t  rohc_api_data = {0};
    UInt32    result = PDCP_SUCCESS;
#ifndef PDCP_ASYNC_INTERFACE
    void      *header_p = PNULL;
    UInt8     *ciphering_p = PNULL;
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING) 
    LteUePdcpDLPerfStats * lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats*)
        &(gPdcpUePerfStats_p[dataReq_p->commDataReq.ueIndex].
                lteUePdcpDLPerfStats);
#endif
#endif
    /*+- SPR 16921 */
#if defined (PERF_STATS) && !defined(UE_SIM_TESTING) 
    rohc_entity_t *p_rohc_entity = PNULL;
#endif

#ifndef UE_SIM_TESTING
    /* Checks if LC is an SPS LC */
    if( LTE_TRUE == ctx_p->isSpsLc )
    {
        /* Cyclomatic Complexity changes - starts here */
        if (PDCP_SUCCESS != pdcpProcessSpsTxSdu(ctx_p, dataReq_p, data_p))
        {
            /* SPR 16921 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                drbs[dataReq_p->commDataReq.lcId - 3].
                numberOfDlDrbSduDiscarded += 1;
            /* SPR 16921 fix end */
            return PDCP_FAIL;
        }
        /* Cyclomatic Complexity changes - ends here */
    }    
#endif

    /*Clone Data Before ROHC and Ciphering in */
    dataReq_p->data_p = msgClone(data_p);
    if ( PNULL == dataReq_p->data_p )
    {
        LOG_PDCP_MSG( PDCP_MSG_CLONE_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, msgSize(data_p, PNULL),
                dataReq_p->commDataReq.ueIndex, dataReq_p->commDataReq.lcId,
                dataReq_p->commDataReq.count,0,0, __func__,"");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
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
        ltePanic("msgClone failed in %s",__FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

    /*ROHC compress (for DRB only)*/
    if(PNULL != ctx_p->rohc.context) 
    {
        rohc_api_data.api_id = ROHC_DATA_REQ;
        rohc_api_data.entity_id.ueIndex = dataReq_p->commDataReq.ueIndex;
        rohc_api_data.entity_id.lcId = dataReq_p->commDataReq.lcId;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
        p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
        rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = LTE_FALSE;
        rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len = 
            msgSize(data_p,PNULL);

        LOG_PDCP_MSG( PDCP_DATA_TO_ROHC_ENGINE, LOGDEBUG, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count,0,0,0,__func__,"");
        if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                    data_p, &out_buff_p, &rohc_api_data, &eCode))
        {
            LOG_PDCP_MSG( PDCP_ROHC_COMPRESSION_FAILED, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 0,0,0,__func__,"");
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].drbs[ \
                dataReq_p->commDataReq.lcId - 3].numberOfDlDrbPduDropForRoHCFailure += 1;
            /* +- SPR 16921 */
            msgFree(data_p);
            data_p = PNULL;	
            pdcpFreeTxDataReq(dataReq_p);
            /*SPR 2498 changes start*/
            dataReq_p = PNULL;
            /*SPR 2498 changes end*/ 
            return PDCP_ERR_DT_ROHC_COMPRESS_FAIL;              
        }
    }

#ifdef PDCP_ASYNC_INTERFACE
    result = pdcpDrbPktForSec( ctx_p, dataReq_p, data_p );
#else
    if ( ALGO_EEA0 != ctx_p->ciphering.algorithmId )
    {
        ciphering_p = (UInt8 *)msgAlloc(0,0,0,0);
        if ( PNULL == ciphering_p )          
        {
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,0,0,0, 
                    dataReq_p->commDataReq.count, 0, 0,
                    __func__,"Allocating bytes for ciphered output");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
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
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        result = pdcpProcessCiphering( &(dataReq_p->commDataReq), 
                ctx_p, data_p, ciphering_p);
        msgFree(data_p);        
        data_p = ciphering_p;
    }
    else
    {
        pdcpStats.statsCiphering.numberOfPacketsCipheredUsingEIA0 += 1;
    }

    if ( PDCP_SUCCESS == result)
    {
        header_p = pdcpMakePDUHeaderDRB( dataReq_p->commDataReq.count, 
                ctx_p->snSize, data_p);
        if(PNULL == header_p) {
            LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, 
                    dataReq_p->commDataReq.count, 
                    ctx_p->snSize, 0,0,__func__,"");
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

        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbPduTransmitted += 1;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        /* Update KPI Counter for total DRB Bytes Transmitted per LC in DL */
        lteUePdcpDLPerfStats_p->totalDlDrbBytesSent
            [dataReq_p->commDataReq.lcId - LTE_DRB_LC_ID_LOW_BOUND] += 
            msgSize(data_p, PNULL) ;
#endif
        result = pdcpDrbFinishPduTx(dataReq_p,ctx_p, data_p);
    }
    else
    {
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            drbs[dataReq_p->commDataReq.lcId - 3].
            numberOfDlDrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        /*coverity 25195 fix*/
        msgFree(data_p);
        data_p = PNULL;	
        pdcpFreeTxDataReq(dataReq_p);
        dataReq_p = PNULL;	
        /*coverity 25195 fix*/
    }
#endif
    return result;
}

#ifndef UE_SIM_TESTING
/******************************************************************************
 * Function Name  : pdcpProcessSpsTxSdu
 * Inputs         : ctx_p - Pointer to LC context, 
 *                  dataReq_p - Data to be processed
 *                  data_p - Data pointer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / PDCP_FAIL
 * Description    : Processes SPS Tx SDU received
 *****************************************************************************/
STATIC inline UInt32 pdcpProcessSpsTxSdu( LP_PdcpEnityContext ctx_p,
    LP_PdcpDataReq dataReq_p, void *data_p )
{
    QSEGMENT segment = {0};
    LP_PdcpUeContext ueContext_p = PNULL;
    UInt8 dlSpsInterval = 0;
    UInt8 configuredSilencePktSize = 0;

    /* + Layer2 NON CA Changes */
    InternalCellIndex  cellIndex = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */

    /* Following piece of code determines IP packet type only if SPS 
     * is enabled and will compute only once */
    if( NOT_DETERMINED == ctx_p->ipPktType )
    {
        UInt8 *buf_p = PNULL;
        if (PNULL == msgSegNext(data_p, 0, &segment) )
        {
            LOG_PDCP_MSG( PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, 
                    dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, 0,0, 0,0, 
                    __func__, "");
            pdcpFreeTxDataReq(dataReq_p);
            dataReq_p = PNULL;	
            return PDCP_FAIL;
        }
        buf_p = segment.base;
        /* As per below byte shown for IP header first 4 bits contains
         * packet type where 0x4 indicates IPv4 and 0x6 reperesents IPv6
         *   0 1 2 3 	4 5 6 7 	 	
         *   Version 	IHL      */
        if( 0x40 == (buf_p[0] & 0xF0) )
        {
            ctx_p->ipPktType = IP_TYPE_IPV4;
        }
        else       
        {
            ctx_p->ipPktType = IP_TYPE_IPV6;
        }
    }
    /* Following piece of code determines packet type information, i.e,
     * if it is SILENCE or VOICE */
    /* SPR 5485 changes start */
    ueContext_p = PDCP_GET_VALID_UE_CONTEXT( 
            dataReq_p->commDataReq.ueIndex );
    /* SPR 5485 changes end */
    
    /* + Layer2 NON CA Changes */
    cellIndex = ueContext_p->cellIndex;

    dlSpsInterval = ueContext_p->semiPersistentSchedIntervalDL;
    
    if( IP_TYPE_IPV4 == ctx_p->ipPktType )
    {
        configuredSilencePktSize =
            pdcpContext.sCellInfo[cellIndex].spsProfileParams_p[dlSpsInterval]->
            silencePdcpSduSizeIpv4;
    }
    else
    {
        configuredSilencePktSize =
            pdcpContext.sCellInfo[cellIndex].spsProfileParams_p[dlSpsInterval]->
            silencePdcpSduSizeIpv6;
            /*pdcpContext.spsProfileParams_p[dlSpsInterval]->
            silencePdcpSduSizeIpv6;*/
    }
    /* - Layer2 NON CA Changes */
    
    if( configuredSilencePktSize >= dataReq_p->commDataReq.size )
    {
        /* SPR 20570 Fix Start */
        dataReq_p->commDataReq.voiceSilenceType = PDCP_SILENCE_PKT;
        /* SPR 20570 Fix End */
    }
    return PDCP_SUCCESS;
}
#endif

#ifndef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpProcessCiphering
 * Inputs         : ctx_p - Pointer to UE and LC context, 
 *                  dataReq_p - Data to be processed
 *                  data_p -
 *                  ciphering_p -
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Performs ciphering of data
 ****************************************************************************/
UInt32 pdcpProcessCiphering(
        LP_PdcpCommDataReq dataReq_p, 
        LP_PdcpEnityContext ctx_p,
        void    *data_p,
        UInt8   *ciphering_p )
{
    UInt32 result = 0;
    UInt8     *outBuff_p = PNULL;
    QSEGMENT segment ;
    UInt32 size = msgSize(data_p,PNULL);
    UInt32 sizeOfP1 = 0,sizeOfP2=0;
    UInt8 *inBuffP1_p = PNULL,*inBuffP2_p=PNULL,*prevSeg_p=PNULL;
    UInt32 msgSegCnt = msgSegCount(data_p);
    /* SPR 2556 changes start */
    UInt32 cipherSize = 0;

    /*
     ** Ciphering (EEA1) compliance requires data to be in multiple
     ** of 4, i.e size (bytes) should be in multiple of 4
     */
    cipherSize = size;

    /* Single segment in the message  */
    /* SES-525 Changes Start */
    /*if ( ( ( ALGO_EEA1 == ctx_p->ciphering.algorithmId ) && 
            ( SINGLE_SEGMENT == msgSegCnt) &&
            ( 0 == size % CIPHER_MULTIPLE ) ) || 
            ( ALGO_EEA1 != ctx_p->ciphering.algorithmId ) )*/
	if (( SINGLE_SEGMENT == msgSegCnt) && ( 0 == size % CIPHER_MULTIPLE ))
    /* SES-525 Changes End */
    {
        outBuff_p = msgReserve(ciphering_p,0,size);
        if (PNULL == outBuff_p)
        {         
            LOG_PDCP_MSG( PDCP_MSG_RESERVE_FAILED, LOGFATAL, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, size, 0,0, dataReq_p->count,
                    0,0, __func__, "Reserving bytes for ciphered output");
            ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("msgReserve failed in %s", __FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        prevSeg_p = msgSegNext(data_p,0,&segment);
        if ( PNULL == prevSeg_p )
        {
            LOG_PDCP_MSG( PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex, 
                    dataReq_p->lcId, 0,0, 0,0, __func__, "");
            return PDCP_FAIL;
        }

        inBuffP1_p = segment.base;
        sizeOfP1 = segment.size;

        LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
                dataReq_p->lcId, dataReq_p->count,0, 0,0,__func__,"");

        result = sendToCiphering(ctx_p->ciphering.algorithmId,
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataReq_p->count,
                PDCP_ENTITY_RB_RX,
                ctx_p->ciphering.key,
                 inBuffP1_p, inBuffP2_p, 
                 outBuff_p,
                sizeOfP1, sizeOfP2);
        /* SPR_1210_FIX_START*/
        if (PDCP_SUCCESS != result)
        {
            LOG_PDCP_MSG (PDCP_CIPHERING_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
                    dataReq_p->lcId, dataReq_p->count, size, 0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("ciphering function fails %s",__FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
    }
    else /* Multiple segments in the message */
    {
        if ( 0 != size % CIPHER_MULTIPLE )
        {
            cipherSize += ( CIPHER_MULTIPLE - (size % CIPHER_MULTIPLE) );
        }
        inBuffP1_p = (UInt8 *)getMemFromPool(cipherSize, PNULL);
        if (PNULL == inBuffP1_p )
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, cipherSize, dataReq_p->ueIndex,
                    dataReq_p->lcId, dataReq_p->count, 0,0, __func__, 
                    "Cannot allocate memory for ciphering");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic ("getMemPool error in %s", __func__);
            return PDCP_FAIL;
        }
        if ( 0 == msgExtract(data_p, 0, inBuffP1_p, size ) )
        {
            LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, size, dataReq_p->ueIndex,
                    dataReq_p->lcId, dataReq_p->count, 0,0, __func__,
                    "ZCB data cannot be extracted for deciphering");
            freeMemPool(inBuffP1_p);
            inBuffP1_p = PNULL;	
            ltePanic ("msgExtract error in %s", __func__);
            return PDCP_FAIL;
        }
        LOG_PDCP_MSG( PDCP_DATA_TO_CIPHERING_ENGINE, LOGDEBUG, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
                dataReq_p->lcId, dataReq_p->count,0, 0,0,__func__,"");
        result = sendToCiphering(ctx_p->ciphering.algorithmId,
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataReq_p->count,
                PDCP_ENTITY_RB_RX,
                ctx_p->ciphering.key,
                inBuffP1_p, PNULL, 
                inBuffP1_p,
                cipherSize, sizeOfP2);
        /* SPR_1210_FIX_START*/
        if (PDCP_SUCCESS != result)
        {
            LOG_PDCP_MSG (PDCP_CIPHERING_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, dataReq_p->ueIndex,
                    dataReq_p->lcId, dataReq_p->count, cipherSize, 0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("ciphering function fails %s",__FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        if (PNULL == msgInsert(ciphering_p, 0, inBuffP1_p, size) )
        {
            LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, size, dataReq_p->ueIndex,
                    dataReq_p->lcId, dataReq_p->count, 0,0, __func__,
                    "data cannot be converted bach to ZCB after ciphering ");
            freeMemPool(inBuffP1_p);
            inBuffP1_p = PNULL;	
            ltePanic("%s: MsgInsert failed while during ciphering",__func__);
            return PDCP_FAIL;
        }
        freeMemPool(inBuffP1_p);
        inBuffP1_p = PNULL;
    }
    /* SPR 2556 changes end */

    return result;
}

/****************************************************************************
 * Function Name  : pdcpGenIntegrityTx
 * Inputs         : ctx_p - Pointer to UE and LC context
 *                  dataReq_p - Pointer to Data Request
 *                  data_p - Pointer to data
 *                  pdcpProcessSrbTxMacI_p - Pointer to MAC-I
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : Processes SRB Tx PDU and applies integrity protection
 ****************************************************************************/
static inline UInt32 pdcpGenIntegrityTx( LP_PdcpEnityContext ctx_p,
    LP_PdcpSrbDataReq dataReq_p, void *data_p, UInt8 *pdcpProcessSrbTxMacI_p)
{
    void      *header_p      = PNULL;
    if ( ALGO_EIA0 != ctx_p->integrity.algorithmId ) 
    {
        /*have to add SRB header before calculating MACI*/
        header_p = pdcpMakePDUHeaderSRB( dataReq_p->commDataReq.count,
                ctx_p->snSize, data_p);
        if(PNULL == header_p)
        {
            LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, 
                    dataReq_p->commDataReq.count, ctx_p->snSize, 0,0, 
                    __func__,"");
            /* SPR 16921 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                srbs[dataReq_p->commDataReq.lcId - 1].
                numberOfDlSrbSduDiscarded += 1;
            /* SPR 16921 fix end */
            msgFree(data_p);
            data_p = PNULL;	
            pdcpFreeSrbTxDataReq(dataReq_p);
            /*SPR 2498 changes start*/
            dataReq_p = PNULL;
            /*SPR 2498 changes end*/ 
            ltePanic("%s:header_p is NULL",__FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        LOG_PDCP_MSG( PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count,
                0,0,0,__func__,"");
        /*calc MACI*/
        integrityGenerateMACI(
                ctx_p->integrity.algorithmId,
                /*SPR 1936 fix start*/
                /* SPR 3276 changes start */
                (ctx_p->radioBearer - 1),
                /* SPR 3276 changes end */
                /*SPR 1936 fix end*/
                dataReq_p->commDataReq.count,
                PDCP_ENTITY_RB_RX, /* SPR_1205_FIX */
                ctx_p->integrity.key,
                data_p,
                pdcpProcessSrbTxMacI_p ); 
        /*we don't need SRB header here. It will be recreated during pdcpFinishPduTx()*/
        if ( 0 == msgRemove( data_p, 0, 1 ) )
        {
            LOG_PDCP_MSG( PDCP_MSG_REMOVE_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, 
                    dataReq_p->commDataReq.ueIndex,
                    dataReq_p->commDataReq.lcId, 1, 
                    dataReq_p->commDataReq.count, 0,0, __func__, 
                    "Cannot remove PDCP Header");
            /* SPR 16921 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                srbs[dataReq_p->commDataReq.lcId - 1].
                numberOfDlSrbSduDiscarded += 1;
            /* SPR 16921 fix end */
            msgFree(data_p);
            data_p = PNULL;	
            pdcpFreeSrbTxDataReq(dataReq_p);
            /*SPR 2498 changes start*/
            dataReq_p = PNULL;
            /*SPR 2498 changes end*/ 
            ltePanic("msgRemove failed in %s",__FUNCTION__);
            return PDCP_FAIL;
        }
    }
    else 
    {
        /*6.3.4. For control plane data that are not integrity protected,
          the MAC-I field is still present and should be padded with 
          padding bits set to 0.*/
        /* pdcpProcessSrbTxMacI_p is already reset to zero at the 
         * beginning, hence removing memSet from here  */
        /*Update stats */        
        pdcpStats.statsIntegrity.\
            numberOfPacketsIntegrityProtectedUsingEIA0 += 1;
    }
    /*Store MACI value after DATA*/
    if(PNULL == msgInsert(data_p,-1,pdcpProcessSrbTxMacI_p,MACI_LEN))
    {
        LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, MACI_LEN, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId,
                dataReq_p->commDataReq.count, 0,0, 
                __func__,"Cannot insert MAC-I");
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
        /*SPR 2498 changes start*/
        dataReq_p = PNULL;
        /*SPR 2498 changes end*/ 
        ltePanic("msgInsert failed in %s",__FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    return PDCP_SUCCESS;
}
#endif

/****************************************************************************
 * Function Name  : pdcpProcessControlPduIndTx  
 * Inputs         : controlPduInd_p - pointer on PDCP Control PDU indication
 *                                    structure.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Processes signle PDCP Control PDU indication 
 ****************************************************************************/
void pdcpProcessControlPduIndTx(LP_PdcpControlPduInd controlPduInd_p )
{
    LP_PdcpUeContext ueCtx_p = PNULL; 
    LP_PdcpEnityContext ctx_p = PNULL;
    LP_PdcpEnityContext rx_ctx_p = PNULL;
    void *header_p = PNULL;
    /*SPR 3608 changes start*/
    UInt8 type = 0 ;
#ifdef LOG_PRINT_ENABLED 
    UInt32 state = 0;
#endif
    do
    {
        /*SPR 4865 Fix*/
        ueCtx_p = PDCP_GET_UE_CONTEXT(controlPduInd_p->ueIndex);
        if (PNULL == ueCtx_p)
        {
            LOG_PDCP_MSG( PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,controlPduInd_p->ueIndex,
                    pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            break;
        }
        /*SPR 4865 Fix*/
        ctx_p = pdcpGetEntityContext(controlPduInd_p->ueIndex,
                controlPduInd_p->lcId,PDCP_ENTITY_RB_TX);
        if(PNULL == ctx_p)
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, controlPduInd_p->ueIndex,
                    controlPduInd_p->lcId,0,0,0,0,__func__,"");
            break;
        }
        rx_ctx_p = pdcpGetEntityContext( controlPduInd_p->ueIndex,
                controlPduInd_p->lcId, 
                PDCP_ENTITY_RB_RX);
        if(PNULL == rx_ctx_p)
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__,  controlPduInd_p->ueIndex,
                    controlPduInd_p->lcId, 0,0,0,0, __func__,"");
            break;
        }
        /*SPR 3608 changes end*/

        if ( PDCP_TX_CONTROL_PDU_IND == controlPduInd_p->type )
        {
            /* Transfer PDCP Control PDU to the RLC layer */
            /*SPR 3608 changes start*/
            header_p = pdcpMakePDUHeaderControlPdu(controlPduInd_p->ctrl_type,   
                    rx_ctx_p->fms );
            /*SPR 3608 changes end*/
            if( PNULL == msgJoin(header_p, controlPduInd_p->data_p) )
            {
                LOG_PDCP_MSG( PDCP_MSG_JOIN_FAILED, LOGFATAL, PDCP_TX, 
                        pdcpCurrentTime_g, controlPduInd_p->ueIndex,
                        controlPduInd_p->lcId, msgSize(header_p, PNULL),
                        msgSize(controlPduInd_p->data_p, PNULL), 
                        controlPduInd_p->ctrl_type, 0, 0,
                        __func__, "header_p cannot be joined with data_p");
                ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
                /* Coverity_31454_Fix Start*/
                if(header_p)
                {
                    msgFree(header_p);
                    header_p = PNULL;	
                }
                /* Coverity_31454_Fix End */
                ltePanic("msgJoin failed in %s",__FUNCTION__);
                break;
            }
            msgFree(controlPduInd_p->data_p);
            controlPduInd_p->data_p = PNULL;

            LOG_PDCP_MSG( PDCP_PDU_FROM_PDCP_TO_RLC , LOGINFO, PDCP_TX, 
                    pdcpCurrentTime_g,  __LINE__, controlPduInd_p->ueIndex,
                    controlPduInd_p->lcId, 0,0, 0,0,__func__,"");
            /*!!! Please keep in mind that controlPduInd_p->data_p should be 
              released in rlcUeDataReq in any case */

            sendrlcUeDataReqToRlc(
                    controlPduInd_p->ueIndex,
                    controlPduInd_p->lcId,
                    header_p,
                    msgSize(header_p, PNULL),
                    PDCP_RLC_TRANSACTION_ID_INVALID,
                    /* SPR 2812 changes start */
                    PNULL,
                    /* SPR 2812 changes end */
                    PDCP_RLC_REQ_ACK_FALSE,pdcpCurrentTime_g,
                    NOT_DETERMINED, PDCP_CONTROL_PDU, 0);
            /*SPR 4865 Fix*/
            if(!(controlPduInd_p->ctrl_type))
            {
                (ueCtx_p->numOfPSRSent)++;
            }
            /*SPR 4865 Fix*/
        }
        else
        {
            /* PDCP_RX_CONTROL_PDU_IND - Handle PDCP Control PDU */
            /* Here PDCP_TS_PDU_TYPE_STATUS_REPORT, 
             * PDCP_TS_PDU_TYPE_ROHC_FEEDBACK PDCP Control PDUs shall be 
             * handled */
            QSEGMENT segment ;
            if (PNULL == msgSegNext(controlPduInd_p->data_p, 0, &segment))
            {
                LOG_PDCP_MSG (PDCP_MSG_SEG_NEXT_FAILED, LOGWARNING,
                        PDCP_TX, pdcpCurrentTime_g, __LINE__, 
                        controlPduInd_p->ueIndex, controlPduInd_p->lcId, 0,0,
                        0,0, __func__, "");
                break;
            }
            UInt8 * dataPtr_p = segment.base;
            /*SPR 3608 changes start*/
            type = (dataPtr_p[0] & 0xff)>>4;
            if(!((0x1 == type) || (0x0 == type)))
            {
                break;
            }
            /* Cyclomatic Complexity changes - starts here */
            pdcpHandleControlPduIndRx (type, controlPduInd_p, ctx_p, ueCtx_p, 
                    rx_ctx_p);
            /* Cyclomatic Complexity changes - starts here */
            /*SPR 3608 changes end*/
        }
        if( PDCP_TX_STATE_AWAITED != ueCtx_p->pdcpTxSrb2DrbState ) 
        {
            break;
        }
        if( ueCtx_p->totNumOfPSRtoSend == ueCtx_p->numOfPSRSent && 
                ueCtx_p->numOfPsrRecvd == ueCtx_p->totNumPsrRecvd)
        {
            if( PDCP_AWAITED == ueCtx_p->event )
            {
                ueCtx_p->event = PDCP_RESUME;
            }
#ifdef LOG_PRINT_ENABLED 
            state = ueCtx_p->pdcpTxSrb2DrbState;
#endif
            (void)pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
                [ueCtx_p->event] ( 
                        controlPduInd_p->ueIndex,
                        controlPduInd_p->lcId,
                        ueCtx_p->pdcpTxSrb2DrbState,
                        ueCtx_p->event, ueCtx_p );

            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, controlPduInd_p->ueIndex, state, ueCtx_p->event,
                    ueCtx_p->pdcpTxSrb2DrbState, controlPduInd_p->lcId, 0, __func__,
                    "PdcpRrcTxStateMachine");

        }
    } while(0);

    pdcpFreeControlPduInd( controlPduInd_p );
    controlPduInd_p = PNULL;	
}

/****************************************************************************
 * Function Name  : pdcpHandleControlPduIndRx  
 * Inputs         : type - PDU type
 *                  controlPduInd_p - pointer to PDCP Control PDU Ind struct
 *                  ctx_p - pointer to PDCP TX Entity Context struct
 *                  ueCtx_p - pointer to PDCP UE Context struct
 *                  rx_ctx_p - pointer to PDCP RX Entity Context struct
 * Outputs        : None.
 * Returns        : None.
 * Description    : Processes single PDCPControl PDU Ind based on type of PDU
 ****************************************************************************/
static inline void pdcpHandleControlPduIndRx( UInt8 type, 
    LP_PdcpControlPduInd controlPduInd_p, LP_PdcpEnityContext  ctx_p, 
    LP_PdcpUeContext ueCtx_p, LP_PdcpEnityContext rx_ctx_p)
{
    UInt8  *out_buff_p = PNULL;
    UInt32 eCode = PDCP_SUCCESS;
    rohc_api_info_t  rohc_api_data;
#ifdef PERF_STATS
#ifndef UE_SIM_TESTING
    rohc_entity_t     *p_rohc_entity = PNULL;
#endif
#endif
    if( type ) /*ROHC PDU*/
    {
        if ( 0 == msgRemove(controlPduInd_p->data_p, 0, 1) )
        {
            LOG_PDCP_MSG (PDCP_MSG_REMOVE_FAILED, LOGWARNING,
                    PDCP_TX, pdcpCurrentTime_g, __LINE__,
                    controlPduInd_p->ueIndex, controlPduInd_p->lcId, 0,0,
                    0,0, __func__, "");
        }
        if(PNULL != ctx_p->rohc.context)
        {
            rohc_api_data.api_id = ROHC_DATA_REQ;
            rohc_api_data.entity_id.ueIndex = controlPduInd_p->ueIndex;
            rohc_api_data.entity_id.lcId = controlPduInd_p->lcId;
            rohc_api_data.rohc_api_union.rohc_data_req.feedback_flag = LTE_TRUE;
            rohc_api_data.rohc_api_union.rohc_data_req.in_buff_len 
                = msgSize(controlPduInd_p->data_p,PNULL);
#ifdef PERF_STATS
#ifndef UE_SIM_TESTING 
            p_rohc_entity = (rohc_entity_t*)(ctx_p->rohc.context);
            p_rohc_entity->rohc_entity_id.qci = ctx_p->qci;
#endif
#endif
            if(RO_SUCCESS != rfc3095_process_msg(ctx_p->rohc.context,
                        controlPduInd_p->data_p, &out_buff_p, &rohc_api_data, &eCode))
            {
                LOG_PDCP_MSG (PDCP_CONTROL_PDU_NOT_HANDLED_PROPERLY, LOGWARNING,
                        PDCP_TX, pdcpCurrentTime_g, __LINE__, 
                        controlPduInd_p->ueIndex, controlPduInd_p->lcId, 0,0,
                        0,0, __func__, "");
            }
        }
    }
    else /*PSR PDU*/
    {
        /* Parse the PSR only if it is NOT in RESUME or CONNECTED state,
         * otherwise it may be the case that it will provide PDU to RLC
         * and on receipt of delivery indication system will crash. */
        /* SPR 5332 fix start */
        if( ( PDCP_TX_STATE_RESUME != ueCtx_p->pdcpTxSrb2DrbState ) && 
            ( PDCP_TX_STATE_CONNECTED !=  ueCtx_p->pdcpTxSrb2DrbState )
        /* SPR 5332 fix end */
          )
        {
            if( PDCP_SUCCESS != pdcpParsePSR( ueCtx_p, ctx_p, 
                        controlPduInd_p->lcId,
                        controlPduInd_p->data_p, 
                        msgSize(controlPduInd_p->data_p,PNULL) ) )
            {
                LOG_PDCP_MSG( PDCP_CONTROL_PDU_NOT_HANDLED_PROPERLY, LOGWARNING,
                        PDCP_TX, pdcpCurrentTime_g, __LINE__, 
                        controlPduInd_p->ueIndex, controlPduInd_p->lcId, 
                        0,0, 0,0, __func__, "");
            }
            (ueCtx_p->numOfPsrRecvd)++;
            rx_ctx_p->isPsrRecvd = LTE_TRUE;
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpProcessDrbTx  
 * Inputs         : ueIndex -
 *                  lcId -
 *                  state -
 *                  event -
 *                  context_p -  
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : single function for DRB TX Leg
 ****************************************************************************/
UInt32 pdcpProcessDrbTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p)
{
    LP_PdcpEnityContext ctx_p = (LP_PdcpEnityContext)context_p;
    LP_PdcpDataReq dataReq_p = PNULL;
    void      *data_p = PNULL;
    UInt32    result = PDCP_SUCCESS;
    /* SPR 16921 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8  qci = ctx_p->qci;
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
#endif
    /* SPR 16921 fix end */

    DEQUEUE_PDCP_TX_DATA_REQ_Q(ctx_p, dataReq_p);
    if (PNULL == dataReq_p)
    {
        /*Nothing to process*/
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId,event,state, 0,0, __func__,"pdcpTxDataReqQ");
        return PDCP_FAIL;
    }

    data_p = dataReq_p->data_p;
    if (PNULL == data_p) 
    {
         LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                  pdcpCurrentTime_g, __LINE__, 
                  dataReq_p->commDataReq.ueIndex,
                  dataReq_p->commDataReq.lcId, 
                  dataReq_p->commDataReq.count, 0, 0,0, __func__,"");
        /* SPR 16921 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
        if(qci != 0)
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
        /* SPR 16921 fix end  */
         pdcpFreeTxDataReq(dataReq_p);
         /*SPR 2498 changes start*/
         dataReq_p = PNULL;
         /*SPR 2498 changes end*/ 
         return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    pdcpAssociateSnHfn(ctx_p, &(dataReq_p->commDataReq));
    result = pdcpProcessSduTx(ctx_p, dataReq_p, data_p);
    /* SPR 15520 fix start */
    if ((PDCP_SUCCESS != result) &&
            (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
    {
        /* Decremnt SN as this PDU is not transmitted */
        PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
        /* SPR 16921 fix start */   
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if(qci != 0)
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
        /* SPR 16921 fix end  */
    }
    /* SPR 15520 fix end */
    return result;
}

/****************************************************************************
 * Function Name  : pdcpProcessDataFwdTx
 * Inputs         : ueIndex -
 *                  lcId -
 *                  state -
 *                  event -
 *                  context_p -  
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : single function for DRB TX Leg for fwded packets
 ****************************************************************************/
UInt32 pdcpProcessDataFwdTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p)
{
    UInt32           result   = 0;
    LP_PdcpUeContext ueCtx_p  = (LP_PdcpUeContext)context_p;
    LP_PdcpEnityContext ctx_p = PNULL;
    LP_PdcpDataReq dataReq_p  = PNULL;
    void           *data_p    = PNULL;
   UInt8          isFreshSdu = LTE_FALSE;	
    /* SPR 16921 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt8  qci = 0;
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].
            ltePdcpKPIStats[activeIndexForPdcpStats]);
#endif
    /* SPR 16921 fix end */


    DEQUEUE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
    if (PNULL == dataReq_p)
    {
        /*Nothing to process*/
        /* +- SPR 17777*/
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g, 
                __LINE__, ueIndex,state, event,lcId, 0,0, __func__,
                "pdcpTxDataForwardingDlQ");
        return PDCP_FAIL;
    }
    ctx_p = pdcpGetEntityContext( dataReq_p->commDataReq.ueIndex, 
            dataReq_p->commDataReq.lcId, PDCP_ENTITY_RB_TX );
    if ( PNULL == ctx_p )
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, 0,0, 0,0, __func__,"");
        pdcpFreeTxDataReq(dataReq_p);
        /*SPR 2498 changes start*/
        dataReq_p = PNULL;
        /*SPR 2498 changes end*/ 
        return PDCP_FAIL;
    }
    data_p = dataReq_p->data_p;
    /* SPR 16921 fix start */   
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    qci = ctx_p->qci;
#endif
    /* SPR 16921 fix end */   
    if (PNULL == data_p) 
    {
         LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                  pdcpCurrentTime_g, __LINE__, dataReq_p->commDataReq.ueIndex,
                  dataReq_p->commDataReq.lcId, dataReq_p->commDataReq.count, 
                  0, 0,0, __func__,"");
        /* SPR 16921 fix start */   
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
        /* SPR 16921 fix end  */
         pdcpFreeTxDataReq(dataReq_p);
         /*SPR 2498 changes start*/
         dataReq_p = PNULL;
         /*SPR 2498 changes end*/ 
         return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }
    
    if (PDCP_INVALID_SN == dataReq_p->commDataReq.sn)
    {
        /* Fresh SDU from source PDCP. No SN associated */
        pdcpAssociateSnHfn (ctx_p, &(dataReq_p->commDataReq));
        /* SPR 15520 fix start */
        isFreshSdu = LTE_TRUE;
        /* SPR 15520 fix end */
    }
    else
    {
        UInt32 hfn = ctx_p->hfn;
        /* Not a fresh SDU. SN already associated at source PDCP */
        if ( dataReq_p->commDataReq.sn > ctx_p->next_SN )
        {
            /* SN window roll-over case */
            hfn--;
        }

        dataReq_p->commDataReq.count = PDCP_MAKE_COUNT( 
                dataReq_p->commDataReq.sn, hfn, ctx_p->snSize);
        LOG_PDCP_MSG( PDCP_SDU_SN_HFN_TX, LOGINFO, PDCP_TX, pdcpCurrentTime_g, 
                dataReq_p->commDataReq.ueIndex, dataReq_p->commDataReq.lcId, 
                dataReq_p->commDataReq.sn, hfn, 
                dataReq_p->commDataReq.count, 0,0, __func__, "");
    }
    result = pdcpProcessSduTx(ctx_p, dataReq_p, data_p);
    /* SPR 15520 fix start */
    /* SPR 16921 fix start */   
    /* Decremnt SN only in case SUCCESS is not returned and packet is not a
     * fresh SDU*/
    if ( PDCP_SUCCESS != result &&
            RLC_PDCP_ENTITY_NOT_ACTIVE != result )
    {
        /* Packet is discarded in non-HO scenario, hence update discard rate
         * stats */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
        }
#endif
        if( LTE_TRUE == isFreshSdu )
    {
        PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
    }
    }
    /* SPR 16921 fix end  */
    /* SPR 15520 fix end */
    return result;
}

/****************************************************************************
 * Function Name  : pdcpProcessSrbTx
 * Inputs         : ueIndex -
 *                  lcId -
 *                  state -
 *                  event -
 *                  context_p -  
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS / Error code
 * Description    : signle function for SRB TX Leg
 ****************************************************************************/
UInt32 pdcpProcessSrbTx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p)
{
    LP_PdcpEnityContext ctx_p = (LP_PdcpEnityContext)context_p;
    LP_PdcpSrbDataReq dataReq_p = PNULL;
    void      *data_p = PNULL;
    UInt32    result = PDCP_SUCCESS;
#ifndef PDCP_ASYNC_INTERFACE
    void      *header_p      = PNULL;
    UInt8     *ciphering_p = PNULL;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpDLPerfStats * lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats*)
        &(gPdcpUePerfStats_p[ueIndex].lteUePdcpDLPerfStats);
#endif
#if defined (KPI_STATS) && !defined(UE_SIM_TESTING)
	InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    UInt32 activeIndexForPdcpStats = (gMacStats.
            pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = 
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);
#endif
#endif

    DEQUEUE_PDCP_SRB_TX_DATA_REQ_Q(ctx_p, dataReq_p);
    if (PNULL == dataReq_p)
    {
        /*Nothing to process*/
        LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId,state,event, 0,0, __func__,"pdcpTxDataReqQ");
        return PDCP_FAIL;
    }
    data_p = dataReq_p->data_p;
    if (PNULL == data_p) 
    {
        LOG_PDCP_MSG( PDCP_DATA_POINTER_NULL, LOGWARNING, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, 
                dataReq_p->commDataReq.ueIndex,
                dataReq_p->commDataReq.lcId, 
                dataReq_p->commDataReq.count, 0, 0,0, __func__,"");
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        pdcpFreeSrbTxDataReq(dataReq_p);
        /*SPR 2498 changes start*/
        dataReq_p = PNULL;
        /*SPR 2498 changes end*/ 
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }  

    pdcpAssociateSnHfn(ctx_p, &(dataReq_p->commDataReq));

    dataReq_p->data_p = msgClone(data_p);
    if ( PNULL == dataReq_p->data_p )
    {
        LOG_PDCP_MSG( PDCP_MSG_CLONE_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, msgSize(data_p, PNULL),
                dataReq_p->commDataReq.ueIndex, 
                dataReq_p->commDataReq.lcId, 
                dataReq_p->commDataReq.count,
                0,0, __func__,"");
        ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                CRITICAL_ALARM);
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        msgFree(data_p);

        data_p = PNULL;	
        pdcpFreeSrbTxDataReq(dataReq_p);
        /*SPR 2498 changes start*/
        dataReq_p = PNULL;
        /*SPR 2498 changes end*/ 
        /* SPR 15520 fix start */
        /* Decremnt SN as this PDU is not transmitted */
        PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
        /* SPR 15520 fix end */
        ltePanic("msgClone failed in %s",__FUNCTION__);
        return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
    }

#ifdef PDCP_ASYNC_INTERFACE
    result = pdcpSrbPktForSec( ctx_p, dataReq_p, data_p );
#else
    /* Reset pdcpProcessSrbTxMacI_p to zero */
    memSet( pdcpProcessSrbTxMacI_p, 0, MACI_LEN );

    /*
     ** Integrity protection (for SRB only) [36323-860 Section 5.7]. 
     ** The integrity protection function includes both integrity
     ** protection and integrity verification and is performed in PDCP
     ** for PDCP entities associated with SRBs. The data unit that is
     ** integrity protected is the PDU header and the data part of the
     ** PDU before ciphering.
     */
    /* Cyclomatic Complexity changes - starts here */
    if (PDCP_SUCCESS != pdcpGenIntegrityTx (ctx_p, dataReq_p, data_p,
                pdcpProcessSrbTxMacI_p))
    {
        /* SPR 15520 fix start */
        /* Decremnt SN as this PDU is not transmitted */
        PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
        /* SPR 15520 fix end */
        return PDCP_FAIL;
    }
    /* Cyclomatic Complexity changes - ends here */

    if ( ALGO_EEA0 != ctx_p->ciphering.algorithmId )
    {
        ciphering_p = (UInt8 *)msgAlloc(0,0,0,0);
        if ( PNULL == ciphering_p )          
        {
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,0,0,0, 
                    dataReq_p->commDataReq.count, 0, 0,
                    __func__,"Allocating bytes for ciphered output");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            /* SPR 16921 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                srbs[dataReq_p->commDataReq.lcId - 1].
                numberOfDlSrbSduDiscarded += 1;
            /* SPR 16921 fix end */
            msgFree(data_p);
            data_p = PNULL;	
            pdcpFreeSrbTxDataReq(dataReq_p);
            /*SPR 2498 changes start*/
            dataReq_p = PNULL;
            /*SPR 2498 changes end*/ 
            /* SPR 15520 fix start */
            /* Decremnt SN as this PDU is not transmitted */
            PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
            /* SPR 15520 fix end */
            ltePanic("msgAlloc failed in %s", __FUNCTION__);
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        result = pdcpProcessCiphering(&(dataReq_p->commDataReq), 
                ctx_p, data_p, ciphering_p);
        msgFree(data_p);
        data_p = ciphering_p;
    }
    else
    {
        pdcpStats.statsCiphering.numberOfPacketsCipheredUsingEIA0 += 1;
    }

    if ( PDCP_SUCCESS == result)
    {
        header_p = pdcpMakePDUHeaderSRB( dataReq_p->commDataReq.count, 
                ctx_p->snSize, data_p);
        if(PNULL == header_p) {
            LOG_PDCP_MSG( PDCP_NULL_HEADER, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, dataReq_p->commDataReq.ueIndex, 
                    dataReq_p->commDataReq.lcId, 
                    dataReq_p->commDataReq.count, ctx_p->snSize, 
                    0,0,__func__,"");
            /* SPR 16921 fix start */
            /* Update statistic data */
            pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
                srbs[dataReq_p->commDataReq.lcId - 1].
                numberOfDlSrbSduDiscarded += 1;
            /* SPR 16921 fix end */
            msgFree(data_p);
            data_p = PNULL;	
            pdcpFreeSrbTxDataReq(dataReq_p);
            /*SPR 2498 changes start*/
            dataReq_p = PNULL;
            /*SPR 2498 changes end*/ 
            /* SPR 15520 fix start */
            /* Decremnt SN as this PDU is not transmitted */
            PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
            /* SPR 15520 fix end */
            return PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
        }
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbPduTransmitted += 1;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        /* Update KPI Counter for total SRB Bytes Transmitted  in DL */
        /* MACI_LEN = MACI Header and 1 is equal to PDCP Header Size */
        lteUePdcpDLPerfStats_p->totalDlSrbBytesSent +=
            msgSize(data_p, PNULL)- (MACI_LEN + 1) ;
#endif

        /* KPI changes start */  
#if defined (KPI_STATS) && !defined(UE_SIM_TESTING)
        pLtePdcpKPIStats->dlCtrlPdcpSduBitRate += 
            msgSize(data_p, PNULL)- (MACI_LEN + 1);
#endif
        /* KPI changes end */  
        result = pdcpSrbFinishPduTx(dataReq_p,ctx_p, data_p);
    }
    else
    {
        /* SPR 16921 fix start */
        /* Update statistic data */
        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].
            srbs[dataReq_p->commDataReq.lcId - 1].
            numberOfDlSrbSduDiscarded += 1;
        /* SPR 16921 fix end */
        /*coverity 25196 fix*/
        msgFree(data_p);
        data_p = PNULL;	
        pdcpFreeSrbTxDataReq(dataReq_p);
        dataReq_p = PNULL;	
        /*coverity 25196 fix*/
    }
#endif
    /* SPR 15520 fix start */
    if ((PDCP_SUCCESS != result) &&
            (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
    {
        /* Decremnt SN as this PDU is not transmitted */
        PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
    }
    /* SPR 15520 fix end */
    return result;
}

/****************************************************************************
 * Function Name  : sendrlcUeDataReqToRlc 
 * Inputs         : ueIndex - UE Identifier of a UE
 *                  lcId - Logical Channel ID
 *                  data_p - Pointer to the data received from PDCP
 *                  size - Size of data received from PDCP
 *                  transId - Transaction ID
 *                  serviceRequested - Service Requested from PDCP
 * Outputs        : None
 * Returns        : Success or Failure
 * Description    : This function shall send RlcUeDataReq to RLC.
 * ******************************************************************************/
UInt32 sendrlcUeDataReqToRlc( UInt16 ueIndex,
                      UInt16  lcId,
                      UInt8 * data_p,
                      UInt16  size,
                      UInt8 transactionIdPresent,
                      void * transId,
                      UInt16 serviceRequested,
                      UInt32 pktArrTime,
                      UInt8   ipPktType,
                      UInt8   voiceSilenceType,
                      UInt32  avgPduSize)
{
    UInt32 result = PDCP_SUCCESS;
    result = rlcUeDataReq( ueIndex, lcId, data_p, size,
            transactionIdPresent, transId, serviceRequested, pktArrTime,
            ipPktType, voiceSilenceType, avgPduSize
            );
    return result;
}
/* SPR 5178 changes ends here */

/* SPACC_OPTIMIZATION */


