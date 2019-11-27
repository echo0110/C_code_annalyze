/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpUserSap.c,v 1.1.1.1.14.2.2.1 2010/09/21 15:49:19 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Interface implementation for interaction beetwen the 
 *                     PDCP layer and PDCP user (PDCP SAP).
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpUserSap.c,v $
 * Revision 1.1.1.1.14.2.2.1  2010/09/21 15:49:19  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.6.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.5  2009/05/20 13:36:24  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/15 09:10:14  gur19140
 * bug fixes
 *
 * Revision 1.3  2009/05/15 07:16:51  gur19836
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

#include "ltePdcpLteDefs.h"
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpTxQueues.h"
#include "ltePdcpContext.h"
#include "logging.h"
#include "alarm.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpErrors.h"
#include "ltePdcpExecutionLegs.h"
/* ECN changes start*/
#include "ltePdcpRRMUtils.h"
#include "ltePdcpTlvUtils.h"
/* ECN changes end*/
#ifdef L2_FAST_PATH
#include "ltePdcpDataTransferTx.h"
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* SPR 10894 fix start */
STATIC void pdcpSourceUlDataRelocReq( LP_PdcpUeContext ueCtx_p, 
        LP_PdcpEnityContext pdcpCtx_p, UInt16 ueIndex, UInt8 lcId );
/* SPR 10894 fix end */
STATIC void pdcpSourceDlDataRelocReq(LP_PdcpUeContext ueCtx_p, 
        LP_PdcpEnityContext pdcpCtx_p, UInt16 ueIndex, UInt8 lcId ); 

STATIC inline void pdcpEcnDataReq( UInt16 ueIndex, 
        UInt16 lcId, LP_PdcpEnityContext ctx_p, void *data_p);

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */    
extern tickType_t pdcpCurrentTime_g ;
/* SPR 15909 fix end */
extern PdcpStats   pdcpStats ;
extern PdcpContext pdcpContext;
#ifdef L2_FAST_PATH
UInt8             pdcpIsDrbDataReceivedInTxQ_g = 0;
#endif

#ifdef PDCP_THROUGHPUT_STATS
extern UInt32 pdcpDataTx;
/* Add Num Pkt Stat Start */
extern UInt32 pdcpPktTx;
/* Add Num Pkt Stat End */
#endif

/*****************************************************************************
 * Exported Function Prototypes
 *****************************************************************************/
extern UInt16 pdcpSourceDlDataRelocInd( UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size);
extern UInt16 pdcpSourceUlDataRelocInd(UInt16 ueIndex, UInt8 lcId, UInt16 sn,
        void *data_p, UInt32 size
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING) /*SPR 18490 +-*/
        ,InternalCellIndex internalCellIndex
#endif        
        );/* +- SPR 17415 */
/* SPR 3085 Changes Start */
extern void pdcpEndOfDataForwardingInd( UInt16 ueIndex, UInt8 lcId, 
        UInt8 isEndMarkerIndRecvd );
/* SPR 3085 Changes End */

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpCreateDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  ctx_p   - 
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : LP_PdcpDataReq
 * Description    : This function allocates PDCP Data Req and sets values
 ****************************************************************************/
LP_PdcpDataReq pdcpCreateDataReq( UInt16 ueIndex, UInt16 lcId, 
    LP_PdcpEnityContext ctx_p, void* data_p, UInt16 size )
{
    LP_PdcpDataReq dataReq_p  = PNULL;

    /*SPR 8544 FIX START*/
#if  !defined(RLC_PDCP_IPC_INTF) && !defined(UE_SIM_TESTING)
    if (1 == rlcSduQFullInd( ueIndex, lcId)) 
    { 
        LOG_PDCP_MSG( PDCP_TX_SDU_Q_FULL, LOGWARNING, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,
                0, 0,0, __func__, "");
        pdcpStats.statsUEs[ueIndex].drbs[lcId - LTE_DRB_LC_ID_LOW_BOUND].
            numberOfDlDrbSduDiscarded += 1;
        return PNULL;
    }
#endif
    /*SPR 8544 FIX END*/
/* + KPI_CA */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    /* SPR 13888 fix +- */
    UInt8 qci = ctx_p->qci;
    UInt32 activeIndexForPdcpStats = (gMacStats.
        pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats);
    UInt32 activeIndexForPdcpThpStats = (gMacStats.
        pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
    LtePdcpKPIStats * pLtePdcpKPIStats = 
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);
    LtePdcpKPIThpStats * pLtePdcpKPIThpStats;

    /* SPR 13888 fix start */
    /* code removed */
    /* pdcpTick is now updated in mac thread while encoding rlc pdu */
    /* SPR 13888 fix end */
#endif
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    LteUePdcpDLPerfStats * lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats*)
        &(gPdcpUePerfStats_p[ueIndex].lteUePdcpDLPerfStats);
#endif
    /* create TX request */
    dataReq_p = pdcpAllocateTxDataReq( ueIndex, lcId, PDCP_INVALID_SN, size );

    if ( PNULL == dataReq_p )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataReq), ueIndex,
                lcId, sizeof(ctx_p),0,0,__func__,"");
        /* +- SPR 17777 */
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("MemAlloc fails for PdcpDataReq\n");
        return PNULL;
    }    

    /* Associate discard timer for freshly arrived SDU, if configured */
    /*SPR 2852 Fix Start - QoS requirement */
    dataReq_p->tickAssociated = pdcpCurrentTime_g;
    LOG_UT( PDCP_DISCARD_TIMER_ASSOCIATED, LOGDEBUG, PDCP_TX,
            pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
            dataReq_p->tickAssociated, ctx_p->discardTimer, 0,0,
            __func__,"");
    /*SPR 2852 Fix End */

    /* store pointer on ZCB in TX request */
    dataReq_p->data_p = data_p;

    /*Update UE stats when data received from Packet Relay*/
    pdcpStats.statsUEs[ueIndex].drbs[lcId - LTE_DRB_LC_ID_LOW_BOUND].
        numberOfDlDrbSduReceived += 1;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    if( 0 != qci )
        pLtePdcpKPIStats->totalDlDrbSduRcvd[--qci]++;

    /* PDCP_32.425 code changes start */
    pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
    pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] += size;

    /* PDCP_32.425 code changes end */
#endif
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
    /* Update KPI Counters for total DL SDU rcvd per LC and per UE*/
    lteUePdcpDLPerfStats_p->numberOfDlDrbSduReceived
        [lcId - LTE_DRB_LC_ID_LOW_BOUND]++;
#endif

    LOG_UT( PDCP_SDU_FROM_PACKET_RELAY_TO_PDCP, LOGDEBUG, PDCP_TX,
            pdcpCurrentTime_g, __LINE__, ueIndex, lcId, PDCP_INVALID_SN,
            0, 0,0, __func__, "pdcpTxDataReqQ");

    return dataReq_p;
}

/****************************************************************************
 * Function Name  : pdcpDataReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes SDU to the PDCP layer.
 *                  It's asynchronous function. It stores SDU into PDCP TX 
 *                  queue and returns.
 ****************************************************************************/
void pdcpDataReq( UInt16 ueIndex, UInt16 lcId, UInt32  transactionId, 
        void* data_p, UInt16 size ,UInt8 instanceId)
{
    LP_PdcpDataReq dataReq_p  = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt32 dataSize = 0;
    /* CA Changes start */
    LP_PdcpUeContext    ueCtx_p = PNULL;
    InternalCellIndex cellIndex  = 0;
    /* CA changes end */

#ifdef PDCP_THROUGHPUT_STATS 
    pdcpDataTx += size;
    /* Add Num Pkt Stat Start */
    pdcpPktTx++;
    /* Add Num Pkt Stat End */
#endif
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    UInt32 activeIndexForPdcpStats = 0;
    LtePdcpKPIStats * pLtePdcpKPIStats = PNULL; 
    UInt8 qci = 0;
#endif
    /* Check data pointer */
    if ( PNULL == data_p ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId,transactionId,instanceId, 0,0,
                __func__,"Packet Relay gives NULL data" );
        return;
    }

    do 
    {
        /* check message size */
        dataSize = msgSize( data_p, PNULL );
        /* SPR 3538 changes start */
        if ( dataSize != size || PDCP_TS_SDU_MAX_SIZE < dataSize ) 
            /* SPR 3538 changes end */
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, dataSize,0, ueIndex, lcId,
                    0,0, __func__,"");
            break;
        }

        /* Check ueIndex and lcId parameters */
        if ( (MAX_UE_SUPPORTED <= ueIndex) ||
                (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < lcId) ) 
        {
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, lcId, ueIndex, 0,0, 0,0, __func__,"");
            break;
        }

        /* CA Changes start */
        /*Check UE context*/
        ueCtx_p = PDCP_GET_UE_CONTEXT( ueIndex );
        if ( PNULL == ueCtx_p )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, ueIndex,
                    pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            /* MSG POOL LEAK FIX START */
            break;            
            /* MSG POOL LEAK FIX END */
        }
#ifndef UE_SIM_TESTING
        cellIndex = ueCtx_p->cellIndex ;
#endif
        /* CA changes end */

        ctx_p = pdcpGetEntityContext(ueIndex,lcId,PDCP_ENTITY_RB_TX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0, 0, 0, 
                    0,__func__,"");
            break;
        }
        /* + SPR_12794 */        
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        activeIndexForPdcpStats = 
            (gMacStats.pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);

        pLtePdcpKPIStats = &(gMacStats.pStats->lteCommonKPIStats[cellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);

        qci =  ctx_p->qci;
#endif 
        /* - SPR_12794 */        
        /* SPR 3692 changes start    */
        UInt32 pdcpSduCount = QCOUNT_PDCP_TX_DATA_REQ_Q( ctx_p );
        if (( MAX_PDCP_SDU_QUEUE_SIZE <= pdcpSduCount)
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
		|| (!ctx_p->isTrafficAllowed)
#endif
		)
        {
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
			if (!ctx_p->isTrafficAllowed)
			{
                LOG_PDCP_MSG( PDCP_TRAFFIC_NOT_ALLOWED, LOGWARNING, PDCP_TX,
                     pdcpCurrentTime_g, __LINE__, ueIndex, lcId, pdcpSduCount,
                     0, 0,0, __func__, "");
			}
			else
#endif
			{
                /* Discard packet as SDU queue is full */
                LOG_PDCP_MSG( PDCP_TX_SDU_Q_FULL, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, pdcpSduCount,
                    0, 0,0, __func__, "");
            }

            /* + SPR_12794 */        
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
            {
                pLtePdcpKPIStats->totalDlDrbSduRcvd[qci-1]++;
                pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
            }
#endif
            /* - SPR_12794 */        
            break;
        }
        /* SPR 3692 changes end */

        /* ECN changes start*/
        if (pdcpContext.pdcpEcnConfigParam[cellIndex].\
                dlWaterMarkEnabled)
        {
            if ( ( pdcpSduCount + 1) >= 
               pdcpContext.pdcpEcnConfigParam[cellIndex].highWaterMarkSduCount)
            {
                ctx_p->congestionStatus |= ECN_T1_BASED_ON_WATERMARKS;
            }
            else if ( ( pdcpSduCount + 1) <= 
              pdcpContext.pdcpEcnConfigParam[cellIndex].lowWaterMarkSduCount)
            {
                ctx_p->congestionStatus &= (~ECN_T1_BASED_ON_WATERMARKS);
            }  
        }
        /* ECN changes end*/

        dataReq_p = pdcpCreateDataReq(ueIndex, lcId, ctx_p, data_p, size);

        if ( PNULL == dataReq_p )
        {
            /* + SPR_12794 */        
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
            {
                pLtePdcpKPIStats->totalDlDrbSduRcvd[qci-1]++;
                pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
            }
#endif
             /* - SPR_12794 */        
            break;
        }    

        /* ECN changes start*/ 
        // this flag is enabled when RRM sends the PDCP to PDCP_UE_DRB_ECN_CE_REQ
        if( ( (pdcpContext.pdcpEcnConfigParam[cellIndex].\
                        ecnFeatureStatus == ECN_APPLIED_IN_DL ) ||
                    (pdcpContext.pdcpEcnConfigParam[cellIndex].\
                     ecnFeatureStatus == ECN_APPLIED_IN_BOTH_UL_DL ))  && 
              (ctx_p->congestionStatus))  
        {
            /* Cyclomatic Complexity changes - starts here */
            pdcpEcnDataReq (ueIndex, lcId, ctx_p, data_p);
            /* Cyclomatic Complexity changes - ends here */
        } // end of if congestionNotification enabled
        /* ECN changes end*/ 

        /* put data indication to PDCP TX queue */
        ENQUEUE_PDCP_TX_DATA_REQ_Q(ctx_p, dataReq_p);
#ifdef L2_FAST_PATH
        pdcpIsDrbDataReceivedInTxQ_g = LTE_TRUE;
#endif
        return;
    } while (0);

    /* abnormal termination */
    msgFree(data_p);
    return;
}

/* Cyclomatic Complexity changes - starts here */
/****************************************************************************
 * Function Name  : pdcpEcnDataReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  ctx_p   - 
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function sets the ECN IP header and sends ECN DRB 
 *                  UE Indication to RRM when ECN is enabled.
 ****************************************************************************/
STATIC inline void pdcpEcnDataReq( UInt16 ueIndex, 
        UInt16 lcId, LP_PdcpEnityContext ctx_p, void *data_p)
{
    UInt8 version               = IPV4_VER;
    UInt8 *buffP1_p             = NULL;
    UInt8 ipHeaderLength        = 0 ;
    EcnCeBit set_ecn_ip_hdr     = SET_ECN_CE_BIT_IPV4_HDR;
    RrmPdcpDrbEcnCeInd ecnCeInd = {0};

    /* msgLocate gives the location of pointer buffP1_p where PDCP
     * data payload containing IP header starts.
     * It is assumed that the data is kept in the single node segment*/
    if ( 0 == msgLocate(data_p, 0, &buffP1_p ))  
    {
        ltePanic("%s: msgLocate failed while extracting bufer",__func__);
        /* Coverity_53487 Fix Start */
        return ;
        /* Coverity_53487 Fix End */
    }

    if ( IPV6_VER == ( (*(buffP1_p) >> 4) & 0xF ) )
    { 
        set_ecn_ip_hdr = SET_ECN_CE_BIT_IPV6_HDR;
        version = IPV6_VER;
    }

    /* comparison with last 2 bits of DSCP value to know ECN 
     * Transport Capability */
    if ( set_ecn_ip_hdr & (*(buffP1_p + 1)))
    { 
        if(ctx_p->ecnIndtobeSent)
        {
            //send the indication
            ecnCeInd.ueIndex          = ueIndex;
            ecnCeInd.lcId             = lcId; 
            ecnCeInd.direction        = PDCP_ENTITY_RB_TX; 
            ecnCeInd.congestionStatus = CONGESTION_SET_TO_RRM ; 
            sendEcnDrbUeIndicationToRRM(ecnCeInd);
            ctx_p->ecnIndtobeSent = LTE_FALSE;
        }    

        /* If already set congestion enable ECN bit "11" then no need to
         * change the bit and checksum value in IP header*/
        if( ( set_ecn_ip_hdr & (*(buffP1_p + 1))) != set_ecn_ip_hdr)
        {    
            *(buffP1_p + 1) |= set_ecn_ip_hdr;
            UInt16 *ptr = NULL;
            if (IPV4_VER == version)
            {
                /* If optional bytes are included in IP header then its header
                 * length is more then 20 bytes*/
                /* Need to mask first 4 bit containing version */ 
                ipHeaderLength = *buffP1_p << 2; 

                ptr = (UInt16 *)(buffP1_p + CHECKSUM_BYTE);
                *ptr = 0; // set the checksum byte to ZERO

                // set the checksum byte to the value got from getCheckSum
                *ptr = pdcpGetIPHdrChecksum(ipHeaderLength, buffP1_p);
                CONVERT_INT16_HOST_TO_NETWORK(*ptr);
            }
        }
    }
    else
    {
        if(ctx_p->ecnIndtobeSent)
        {
            //send the indication
            ecnCeInd.ueIndex          = ueIndex;
            ecnCeInd.lcId             = lcId; 
            ecnCeInd.direction        = PDCP_ENTITY_RB_TX; 
            ecnCeInd.congestionStatus = CONGESTION_CANT_SET_TO_RRM ; 
            sendEcnDrbUeIndicationToRRM(ecnCeInd);
            ctx_p->ecnIndtobeSent = LTE_FALSE;
        }    
    }
}

#ifdef   L2_FAST_PATH
/****************************************************************************
 * Function Name  : pdcpDataFPReq 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  ctx_p -
 *                  transactionId - Transaction Id
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes SDU to the PDCP layer.
 *                  It directly submits the data (after PDCP processing) to RLC 
 *                  through the fast path and stores a copy of it in the 
 *                  PDCP Storage Q as well for delivery status indication
 ****************************************************************************/
void pdcpDataFpReq( UInt16 ueIndex, UInt16 lcId, 
    LP_PdcpEnityContext ctx_p, UInt32  transactionId, 
        void* data_p, UInt16 size )
{
    LP_PdcpDataReq dataReq_p  = PNULL;
    UInt32 dataSize = 0;
    UInt32 result = PDCP_SUCCESS;
#ifdef PDCP_THROUGHPUT_STATS
    pdcpDataTx += size;
    /* Add Num Pkt Stat Start */
    pdcpPktTx++;
    /* Add Num Pkt Stat End */
#endif
    /* SPR 16921 fix start */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
    UInt32 activeIndexForPdcpStats =
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats;
    LtePdcpKPIStats * pLtePdcpKPIStats =
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);
    UInt8 qci = ctx_p->qci;
#endif
    /* SPR 16921 fix end */

    /* Check data pointer */
    if ( PNULL == data_p ) 
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId,transactionId,0, 0,0,
                __func__,"Packet Relay gives NULL data" );
        /* +- SPR 17777 */
        return;
    }

    do 
    {
        /* check message size */
        dataSize = msgSize( data_p, PNULL );
        /* SPR 3538 changes start */
        if ( dataSize != size || PDCP_TS_SDU_MAX_SIZE < dataSize ) 
            /* SPR 3538 changes end */
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, dataSize,0, ueIndex, lcId,
                    0,0, __func__,"");
            break;
        }

        dataReq_p = pdcpCreateDataReq(ueIndex, lcId, ctx_p, data_p, size);

        if ( PNULL == dataReq_p )
        {
            /* SPR 16921 fix start  */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
            {
                pLtePdcpKPIStats->totalDlDrbSduRcvd[qci-1]++;
                pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
            }
#endif
            /* SPR 16921 fix end  */
            break;
        }    
        /* Its not a case of Handover so execute Fast Path */
        pdcpAssociateSnHfn(ctx_p, &(dataReq_p->commDataReq));
        /* SPR 15520 fix start */
        result = pdcpProcessSduTx(ctx_p, dataReq_p, data_p);
        if ((PDCP_SUCCESS != result) &&
                (RLC_PDCP_ENTITY_NOT_ACTIVE != result))
        {
            /* Decremnt SN as this PDU is not transmitted */
            PDCP_DECREMENT_SN( &ctx_p->next_SN, &ctx_p->hfn, ctx_p->snSize );
            /* SPR 16921 fix start  */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            if(qci != 0)
            {
                pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci-1]++;
            }
#endif
            /* SPR 16921 fix end  */
        }
        /* SPR 15520 fix end */

		// pdcpProcessSduTx(ctx_p, dataReq_p, data_p);

        return;

    } while (0);

    /* abnormal termination */
    msgFree(data_p);
    return;
}
#endif

/****************************************************************************
 * Function Name  : pdcpTargetDlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes Forwarded DL Data SDU to the PDCP 
 *                  layer. It's asynchronous function. It stores SDU into PDCP
 *                  Data Fwding queue and returns. Applicable only at target
 *                  PDCP.
 ****************************************************************************/
void pdcpTargetDlDataRelocReq( UInt16 ueIndex, UInt8 lcId, UInt16 sn, 
        void *data_p, UInt16 size,UInt8 instanceId )
{
    LP_PdcpDataReq dataReq_p = PNULL;
    LP_PdcpUeContext ueCtx_p = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt32  dataSize = 0;

#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /*SPR 20667 +*/
    UInt32 activeIndexForPdcpThpStats = 0;
    InternalCellIndex internalCellIndex = 0;
    LtePdcpKPIThpStats *pLtePdcpKPIThpStats = PNULL;
    LtePdcpKPIStats * pLtePdcpKPIStats = PNULL;
    UInt32 activeIndexForPdcpStats = 0;
    UInt8 qci = 0;
    /*SPR 20667 +*/
#endif

    /* Check data pointer */
    if ( PNULL == data_p )
    { 
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_TX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, instanceId,0, 0,0,
                __func__,"Packet Relay gives NULL data" );
        return;
    }

    do
    {
        /* check message size */
        dataSize = msgSize( data_p, PNULL );
        /* SPR 3538 changes start */
        if ( dataSize != size || PDCP_TS_SDU_MAX_SIZE < dataSize ) 
        /* SPR 3538 changes end */
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_TX, 
                     pdcpCurrentTime_g, __LINE__, dataSize,0, ueIndex, lcId,
                     0,0, __func__,"");
            break;
        }

        /* Check ueIndex and lcId parameters */
        /*CSR 40799 chg start*/
        if ( (MAX_UE_SUPPORTED <= ueIndex) ||
        /*CSR 40799 chg end*/
                (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < lcId) ) 
        {
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                     __LINE__, lcId, ueIndex, 0,0, 0,0, __func__,"");
            break;
        }

        ueCtx_p = PDCP_GET_UE_CONTEXT(ueIndex);
        if ( PNULL == ueCtx_p )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, ueIndex, 
                    pdcpContext.isInitialized,0,0, 0,0, __func__,"");
            break;
        }
        ctx_p = pdcpGetEntityContext(ueIndex,lcId,PDCP_ENTITY_RB_TX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                     pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0,
                     __func__,"");
            break;
        }

        /* create TX request */
        dataReq_p = pdcpAllocateTxDataReq( ueIndex, lcId, sn, size);
        if ( PNULL == dataReq_p )
        {
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX, 
                     pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataReq), ueIndex,
                     lcId, 0,0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("MemAlloc fails for PdcpDataReq\n");
            break;
        }    

        /* Associate discard timer for freshly arrived SDU, if configured */
        /*SPR 2852 Fix Start - QoS requirement */
        dataReq_p->tickAssociated = pdcpCurrentTime_g;
        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_ASSOCIATED, LOGDEBUG, PDCP_TX,
              pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
              dataReq_p->tickAssociated
              , ctx_p->discardTimer, 0,0,
              __func__,"");
        /*SPR 2852 Fix End */

        /* store pointer on ZCB in TX request */
        dataReq_p->data_p = data_p;
        /* SPR 16921 fix start */    
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        qci = ctx_p->qci;
        /*SPR 20667 +*/
        activeIndexForPdcpThpStats = (gMacStats.
            pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats);
        internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);
        pLtePdcpKPIThpStats = &(gMacStats.pStats->lteCommonUeKPIStats.
            ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
        pLtePdcpKPIThpStats->SharedMemoryDLPDCPSDU[ueIndex][lcId - 3] += size;
	    /* - KPI_CA */
        /* SPR 16921 fix start */
        activeIndexForPdcpStats =
        gMacStats.pStats->lteCommonKPIStats[internalCellIndex].activeIndexForPdcpStats;
        pLtePdcpKPIStats =
        &(gMacStats.pStats->lteCommonKPIStats[internalCellIndex].ltePdcpKPIStats[
                activeIndexForPdcpStats]);
        /*SPR 20667 +*/
        /* SPR 16921 fix end */
        if( 0 != qci )
        {
            pLtePdcpKPIStats->totalDlDrbSduRcvd[--qci]++;
        }
#endif
        /* SPR 16921 fix end */    
        LOG_PDCP_MSG( PDCP_SDU_FROM_PACKET_RELAY_TO_PDCP, LOGDEBUG, PDCP_TX,
                 pdcpCurrentTime_g, __LINE__, ueIndex, lcId, sn, 0, 0,0, 
                 __func__, "pdcpDataForwardingDlQ");
        /* Enqueing data in DL Data Forwardig queue */
        ENQUEUE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
        return;
    } while (0);

    /* abnormal termination */
    if (PNULL != data_p)
    {
        msgFree(data_p);
        data_p = PNULL;	
    }
    return;
}

/****************************************************************************
 * Function Name  : pdcpTargetUlDataRelocReq
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  sn      - PDCP PDU Sequence Number
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function passes Forwarded UL Data SDU to the PDCP 
 *                  layer. It's asynchronous function. It stores SDU into PDCP
 *                  Data Fwding queue and returns. Applicable only at target
 *                  PDCP.
 ****************************************************************************/
void pdcpTargetUlDataRelocReq( UInt16 ueIndex, UInt8 lcId, UInt16 sn, 
        void* data_p, UInt16 size,UInt8 instanceId )
{
    LP_PdcpDataInd dataInd_p = PNULL;
    LP_PdcpEnityContext ctx_p = PNULL;
    UInt32  dataSize = 0;
    /* + SPR 17415 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /*SPR 20667 +*/
    /* SPR 20831 Changes Start */
   /*code deleted*/ 
    /* SPR 20831 Changes End*/
    UInt32 actIdx = 0;
    LtePdcpKPIThpStats *kpiThpStat_p = PNULL;
    /*SPR 20667 +*/
#endif
    /* - SPR 17415 */

    /* Check data pointer */
    if ( PNULL == data_p ) 
    {
        LOG_PDCP_MSG (PDCP_DATA_POINTER_NULL, LOGFATAL, PDCP_RX, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, instanceId,0, 0,0,
                __func__,"Packet Relay gives NULL data" );
        return;
    }

    do
    {
        /* check message size */
        dataSize = msgSize( data_p, PNULL );
        /* SPR 3538 changes start */
        if ( dataSize != size || PDCP_TS_SDU_MAX_SIZE < dataSize ) 
        /* SPR 3538 changes end */
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_RX, 
                     pdcpCurrentTime_g, __LINE__, dataSize,0, ueIndex, lcId,
                     0,0, __func__,"");
            break;
        }

        /* Check ueIndex and lcId parameters */
        /*CSR 40799 chg start*/ 
        if ( (MAX_UE_SUPPORTED <= ueIndex) ||
        /*CSR 40799 chg end*/
                (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < lcId) ) 
        {
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                     __LINE__, lcId, ueIndex, 0,0, 0,0, __func__,"");
            break;
        }
  
        ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_RX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                     pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0,
                     __func__,"");
            break;
        }

        /* create RX request */
        dataInd_p = pdcpAllocateRxDataInd( ueIndex, lcId, sn, size, 
                                           RE_ESTABLISHED_PACKET, 0, 
                                           PDCP_SPS_IDLE
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                           ,0
#endif
                                           );
        if ( PNULL == dataInd_p )
        {            
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RX, 
                     pdcpCurrentTime_g,__LINE__, sizeof(PdcpDataInd), ueIndex,
                     lcId, 0,0,0,__func__,"");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                       CRITICAL_ALARM);
            ltePanic("MemAlloc fails for PdcpDataReq\n");
            break;
        }

        /* store pointer on ZCB in TX request */
        dataInd_p->data_p = data_p;

        LOG_PDCP_MSG( PDCP_SDU_FROM_PACKET_RELAY_TO_PDCP, LOGDEBUG, PDCP_RX,
                 pdcpCurrentTime_g, __LINE__, ueIndex, lcId, sn, 0, 0,0, 
                 __func__, "pdcpUlReorderingQ");
        /* + SPR 17415 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
        /*SPR 20667 +*/
        /* SPR 20831 Changes Start */
        actIdx = gMacStats.pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
        kpiThpStat_p = &(gMacStats.pStats->lteCommonUeKPIStats.
                ltePdcpKPIThpStats[actIdx]);
        /* SPR 20831 Changes End */
        /*SPR 20667 +*/
        /* Storing the cumulative count for number of UL forwarded packets 
         * received */
        kpiThpStat_p->uLFwdedPdcpSdu[ueIndex][lcId - LTE_DRB_LC_ID_LOW_BOUND] +=
            size ;
#endif
        /* - SPR 17415 */
        /* Enqueing in UL Data Re-ordering queue */
        ENQUEUE_PDCP_RX_STORAGE_Q( ctx_p, dataInd_p );
        return;
    } while (0);

    /* abnormal termination */
    if (PNULL != data_p)
    {
	    msgFree(data_p);
	 data_p = PNULL;
    }
    return;
}

/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReq
 * Inputs         : ueIndex - UE Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy 
 *                            buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives PDCP_DATA_FWD_REQ from Packet Relay.
 *                  It's asynchronous function. It will forward all PDCP SDU 
 *                  with there associated SN Number and returns.
 ****************************************************************************/
void pdcpSourceDataFwdReq( void* data_p, UInt16 size,UInt8 instanceId )
{
    LP_PdcpDataForwardReq dataFwdReq_p = PNULL;
    LP_PdcpEnityContext   ctx_p  = PNULL;
    LP_PdcpUeContext      ueCtx_p   = PNULL;
    UInt8                 lcCount = 0;
    UInt8                 lcId = 0;

    dataFwdReq_p = (LP_PdcpDataForwardReq)data_p;
    
    do {
        /* Check if PDCP is initialized */
        if ( !pdcpContextIsInitialized()) 
        {
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, (PDCP_TX|PDCP_RX), 
                 pdcpCurrentTime_g, __LINE__,instanceId,0,0,0, 0,0,__func__,"" );
            break;
        }

        /* check message size */
        if ( sizeof( PdcpDataForwardReq ) != size ) {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, (PDCP_TX|PDCP_RX), 
                     pdcpCurrentTime_g, __LINE__, sizeof( PdcpDataForwardReq ), 
                     size, 0,0, 0,0, __func__,"PDCP_DATA_FWD_REQ");
            break;
        }

        ueCtx_p = PDCP_GET_UE_CONTEXT( dataFwdReq_p->ueIndex );
        if ( PNULL == ueCtx_p )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, (PDCP_TX|PDCP_RX), 
                     pdcpCurrentTime_g, __LINE__, dataFwdReq_p->ueIndex, 
                     pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            break;
        }

        ueCtx_p->isFwdingApiRecvd = LTE_TRUE;
        /* SPR 10894 fix start*/
        ueCtx_p->newUeIndex = INVALID_UE_INDEX;
        /* Set the type of HO as NORMAL */
        ueCtx_p->hoType = NORMAL_HO;
        /* SPR 10894 fix end*/

        /* Perform data forwarding from source to target eNodeB.
         * Following order of data forwarding needs to be performed:-
         * 1. UL Forwarding (in case of AM mode if UL forwarding is 
         *                   enabled ).
         * 2. DL Forwarding of processed packets, i.e, packets which are
         *    present in storage queue. 
         * 3. DL Forwarding of unprocessed packets, i.e, packets which are
         *    present in fresh SDU queue. 
         * */

        /* Processing for UL forwarding */
        for (lcCount = 0; lcCount < dataFwdReq_p->count; lcCount++)
        {
            lcId = dataFwdReq_p->dataFwdInfo[lcCount].lcId;
            ctx_p = pdcpGetEntityContext( dataFwdReq_p->ueIndex, lcId, 
                                          PDCP_ENTITY_RB_RX);
            if ( PNULL != ctx_p)
            {
                if ( dataFwdReq_p->dataFwdInfo[lcCount].ulDataFwdFlag )
                {
                    /* SPR 10894 fix start*/
                    pdcpSourceUlDataRelocReq( ueCtx_p, ctx_p, 
                            dataFwdReq_p->ueIndex, lcId );
                    /* SPR 10894 fix end*/
                }
            }
            else
            {
                LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RX,
                        pdcpCurrentTime_g, __LINE__, dataFwdReq_p->ueIndex, 
                        lcId, 0,0, 0,0, __func__, "");
            }
        }
        
        /* Processing for DL forwarding */
        for (lcCount = 0; lcCount < dataFwdReq_p->count; lcCount++)
        {
            lcId = dataFwdReq_p->dataFwdInfo[lcCount].lcId;
            ctx_p = pdcpGetEntityContext( dataFwdReq_p->ueIndex, lcId, 
                                          PDCP_ENTITY_RB_TX );
            if ( PNULL != ctx_p)
            {
                /* SPR 16770 fix start */
                ctx_p->isFwdingEnabled = LTE_TRUE;
                /* SPR 16770 fix start */
                pdcpSourceDlDataRelocReq( ueCtx_p, ctx_p, 
                        dataFwdReq_p->ueIndex, lcId  );
            }
            else
            {
                LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                        pdcpCurrentTime_g, __LINE__, dataFwdReq_p->ueIndex,
                        lcId, 0,0, 0,0,__func__,"");
            }
        }
    	if ( (0 == ueCtx_p->numOfActiveDrbLc) && 
	   	     (0 != ueCtx_p->tickAss)
	       )    
        {
            /* Deallocate memory for UE Context */
            freeMemPool( 
                    pdcpContext.ueContexts[dataFwdReq_p->ueIndex] );
            pdcpContext.ueContexts[dataFwdReq_p->ueIndex] = PNULL;
            /* Updates the list of active UEs*/
            updateActiveUeInfo( dataFwdReq_p->ueIndex,
                    pdcpContext.activeUeInfo, 
                    pdcpContext.activeUeIndex 
                    );
    
            /*reset UE Context*/
            pdcpContext.numOfActiveUe -= 1;
            pdcpStats.statNumActiveUe -= 1;
        }
    } while (0);
}

/* SPR 10894 Fix start*/
/****************************************************************************
 * Function Name  : pdcpSourceDataFwdReqInIntraeNB
 * Inputs         : oldUeIndex - old UE Index
 *                  newUeIndex - new UE Index
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function receives PDCP_DATA_FWD_REQ from Packet Relay.
 *                  It's asynchronous function. It will copy all PDCP SDU 
 *                  with there associated SN Number to new context.
 ****************************************************************************/
void pdcpSourceDataFwdReqInIntraeNB( UInt16 oldUeIndex, UInt16 newUeIndex,UInt8 instanceId)
{
    LP_PdcpEnityContext ctx_p      = PNULL; 
    LP_PdcpUeContext    ueCtx_p    = PNULL;
    LP_PdcpUeContext    newUeCtx_p = PNULL;
    UInt8               lcId = 0;

    do 
    {
        /* Check if PDCP is initialized */
        if ( !pdcpContextIsInitialized()) 
        {
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, (PDCP_TX|PDCP_RX), 
                    pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            break;
        }

        ueCtx_p = PDCP_GET_UE_CONTEXT( oldUeIndex );
        if ( PNULL == ueCtx_p )
        {
            /* Unable to fetch the UE Context with old ueIndex provided */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, oldUeIndex,  
                    pdcpContext.isInitialized, instanceId,0, 0,0, 
                    __func__,"");
            break;
        }
        
        if(oldUeIndex == newUeIndex)
        {
            /* oldUeIndex and newUeIndex are same, so intra data forwarding
             * cannot be performed */
            LOG_PDCP_MSG (PDCP_INTRA_HO_FWD_FAIL, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, oldUeIndex, newUeIndex, 0,0, 
                    0,0,__func__,"" );

            /* Resetting the variables for Intra eNB HO */
            ueCtx_p->newUeIndex = INVALID_UE_INDEX;
            ueCtx_p->isFwdingApiRecvd = LTE_FALSE;
            ueCtx_p->hoType = NORMAL_HO;

            break;
        }

        newUeCtx_p = PDCP_GET_UE_CONTEXT( newUeIndex );
        if ( PNULL == newUeCtx_p )
        {
            /* Unable to fetch the UE Context with new ueIndex provided */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, newUeIndex, 
                    pdcpContext.isInitialized, 0,0, 0,0, 
                    __func__,"");
            break;
        }

        /* Save the new UE-Id into the context which needs to be forwarded */
        ueCtx_p->newUeIndex = newUeIndex;
        ueCtx_p->hoType = INTRA_ENB_HO;
        ueCtx_p->isFwdingApiRecvd = LTE_TRUE;

        for( lcId = 0; lcId < ueCtx_p->numOfActiveDrbLc; lcId++ )
        {
            /* Perform data forwarding from old context to new context.
             * Following order of data forwarding needs to be performed:-
             * 1. UL Forwarding (in case of AM mode if UL forwarding is 
             *                   enabled ).
             * 2. DL Forwarding of processed packets, i.e, packets which are
             *    present in storage queue. 
             * 3. DL Forwarding of unprocessed packets, i.e, packets which are
             *    present in fresh SDU queue. 
             * */
            
            /* Fetch RX context for UL forwarding */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[lcId], PDCP_ENTITY_RB_RX );

            pdcpSourceUlDataRelocReq( ueCtx_p, ctx_p, oldUeIndex, 
                    ueCtx_p->activeLcInfo[lcId] );

            /* Fetch TX context for DL forwarding */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[lcId], PDCP_ENTITY_RB_TX );
		    
            /* SPR 16770 fix start */
            ctx_p->isFwdingEnabled = LTE_TRUE;
            /* SPR 16770 fix end */
            pdcpSourceDlDataRelocReq( ueCtx_p, ctx_p, oldUeIndex, 
                    ueCtx_p->activeLcInfo[lcId] );
        }
        
        /* Delete the UE context if DELETE request is already received from L3
         * and no active LC is present in the UE Context */
        if ( (0 == ueCtx_p->numOfActiveDrbLc) &&
             (0 != ueCtx_p->tickAss) 
	       )
        {
            /* Deallocate memory for UE Context */
            freeMemPool( 
                    pdcpContext.ueContexts[oldUeIndex] );
            pdcpContext.ueContexts[oldUeIndex] = PNULL;
            /* Updates the list of active UEs*/
            updateActiveUeInfo( oldUeIndex,
                    pdcpContext.activeUeInfo, 
                    pdcpContext.activeUeIndex 
                    );
    
            /*reset UE Context*/
            pdcpContext.numOfActiveUe -= 1;
            pdcpStats.statNumActiveUe -= 1;
        }
    } while (0);
}
/* SPR 10894 fix end*/


/****************************************************************************
 * Function Name  : pdcpSourceUlDataRelocReq
 * Inputs         : ueCtx_p - 
 *                  pdcpCtx_p - pdcp Context to fwd each of UL packets,
 *                              if asked.
 *                  ueIndex -
 *                  lcId -
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will forward all the uplink packets.
 ****************************************************************************/
/* SPR 10894 fix start */
void pdcpSourceUlDataRelocReq(LP_PdcpUeContext ueCtx_p, 
        LP_PdcpEnityContext pdcpCtx_p, UInt16 ueIndex, UInt8 lcId )
/* SPR 10894 fix end */
{
	UInt32 qCount = QCOUNT_PDCP_RX_STORAGE_Q(pdcpCtx_p); 
	LP_PdcpDataInd ulFwdData_p =PNULL;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    InternalCellIndex internalCellIndex = PDCP_GET_CELL_INDEX(ueIndex);/* +- SPR 17415 */
#endif    
	if ( 0 < qCount )
	{
        /* 
        ** Forwards the PDCP SDUs that have been received in out of order
        ** sequence from RLC 
        */
		GET_FIRST_PDCP_RX_STORAGE_Q(pdcpCtx_p, ulFwdData_p);
        if ( PNULL == ulFwdData_p )
        {
            LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_RX, pdcpCurrentTime_g,
                     __LINE__, ueIndex, lcId, 0,0, 0,0, __func__,
                     "pdcpUlReorderingQ");            
        }
        else
        {
		    do
            {
                LOG_PDCP_MSG( PDCP_SDU_FWDED_TO_PACKET_RELAY, LOGDEBUG, PDCP_RX,
                        pdcpCurrentTime_g, __LINE__, ulFwdData_p->ueIndex,
                        ulFwdData_p->lcId, ulFwdData_p->sn, ulFwdData_p->size,
                        0,0, __func__, "");
                /* SPR 10894 fix start */
                if( NORMAL_HO == ueCtx_p->hoType )
                {
                    /* Forward the packet to packet relay to be forwarded to 
                     * SGW */
                    /* + SPR 17415 */
                    if ( PDCP_SUCCESS != pdcpSourceUlDataRelocInd(
                                ulFwdData_p->ueIndex, ulFwdData_p->lcId, 
                                ulFwdData_p->sn, ulFwdData_p->data_p, 
                                msgSize(ulFwdData_p->data_p, PNULL) 
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)/*SPR 18490+-*/ 
                                ,internalCellIndex
#endif                                
                                ))
                    /* - SPR 17415 */    
                    {
                        LOG_PDCP_MSG( PDCP_UL_DATA_FWD_FAILS, LOGWARNING, PDCP_RX, 
                                pdcpCurrentTime_g,__LINE__, 
                                ulFwdData_p->ueIndex,
                                ulFwdData_p->lcId, ulFwdData_p->sn, 
                                ulFwdData_p->size,
                                0,0, __func__,"");
                    }
                }
                else
                {
                    /* Clone the data to keep the copy at new context as this 
                     * will be removed once old context is deleted after 
                     * forwarding */
                    UInt8 *data_p = msgClone(ulFwdData_p->data_p);
                    if( PNULL == data_p )
                    {
                        LOG_PDCP_MSG( PDCP_MSG_CLONE_FAILED, LOGFATAL, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                msgSize(ulFwdData_p->data_p, PNULL),
                                ulFwdData_p->ueIndex, ulFwdData_p->lcId,
                                ulFwdData_p->sn, 0,0, __func__,"");
                    }
                    else
                    {
                        /* + SPR 17415 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)                   
                        /* SPR 20831 Changes Start */
                        UInt32 activeIndexForPdcpThpStats = gMacStats.
                            pStats->lteCommonUeKPIStats.activeIndexForPdcpThpStats;
                        LtePdcpKPIThpStats *pLtePdcpKPIThpStats = &(gMacStats.
                                pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[activeIndexForPdcpThpStats]);
                        /* SPR 20831 Changes End */
                        /* UlSduBitRate needs to be calculated at source eNB
                         * and will not be incremented at target, hence 
                         * incrementing the stats here for Intra eNB HO.
                         * (Reference: Section 4.4.1.2 of 32.425) */
                        pLtePdcpKPIThpStats->SharedMemoryULPDCPSDU
                            [ulFwdData_p->ueIndex]
                            [ulFwdData_p->lcId - LTE_DRB_LC_ID_LOW_BOUND] += 
                            ulFwdData_p->size;
#endif
                        /* - SPR 17415 */
                        /* Perform internal forwarding, i.e., copy the UL data
                         * to new context */
                        pdcpTargetUlDataRelocReq( ueCtx_p->newUeIndex, 
                                ulFwdData_p->lcId, ulFwdData_p->sn, 
                                data_p, ulFwdData_p->size,0 );
                    }
                }
                /* SPR 10894 fix end */
                GET_NEXT_PDCP_RX_STORAGE_Q(pdcpCtx_p, ulFwdData_p, 
                        ulFwdData_p);
            } while ( PNULL != ulFwdData_p );
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpSourceDlDataRelocReq
 * Inputs         : ueCtx_p - 
 *                  pdcpCtx_p - pdcp Context to fwd each of DL packets.
 *                  ueIndex -
 *                  lcId -
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will forward all the downlink packets.
 *                  (Storage nodes if any in case of AM mode).
 ****************************************************************************/
void pdcpSourceDlDataRelocReq(LP_PdcpUeContext ueCtx_p, 
        LP_PdcpEnityContext pdcpCtx_p, UInt16 ueIndex, UInt8 lcId ) 
{    
	UInt32 qCount = QCOUNT_PDCP_TX_STORAGE_Q(pdcpCtx_p); 
	LP_PdcpDataReq dlFwdData_p = PNULL;
#ifdef LOG_PRINT_ENABLED 
	UInt32 state  = 0;
#endif

    if ( 0 < qCount && LTE_FALSE == pdcpCtx_p->lossyHoRab )
	{
        /* 
        ** Forwards the PDCP SDUs that have already been processed but waiting
        ** for delivery indications from RLC 
        */
		GET_FIRST_PDCP_TX_STORAGE_Q(pdcpCtx_p, dlFwdData_p);
        if ( PNULL == dlFwdData_p )
        {
            LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                     __LINE__, ueIndex, lcId, 0,0, 0,0, __func__,
                     "pdcpStorageQ");            
        }
        else
        {
            do 
            {
                LOG_PDCP_MSG( PDCP_SDU_FWDED_TO_PACKET_RELAY, LOGDEBUG, PDCP_TX,
                        pdcpCurrentTime_g, __LINE__, dlFwdData_p->commDataReq.ueIndex,
                        dlFwdData_p->commDataReq.lcId,
                        dlFwdData_p->commDataReq.sn, dlFwdData_p->commDataReq.size,
                        0,0, __func__, "");
                /* SPR 10894 fix start */
                if( NORMAL_HO == ueCtx_p->hoType )
                {
                    /* Forward the packet to packet relay to be forwarded to 
                     * SGW */
                    if ( PDCP_SUCCESS != pdcpSourceDlDataRelocInd(
                                dlFwdData_p->commDataReq.ueIndex, 
                                dlFwdData_p->commDataReq.lcId, 
                                dlFwdData_p->commDataReq.sn, 
                                dlFwdData_p->data_p, 
                                dlFwdData_p->commDataReq.size) ) 
                    {
                        LOG_PDCP_MSG( PDCP_DL_DATA_FWD_FAILS, LOGWARNING, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                dlFwdData_p->commDataReq.ueIndex,
                                dlFwdData_p->commDataReq.lcId,
                                dlFwdData_p->commDataReq.sn, 
                                dlFwdData_p->commDataReq.size,
                                0,0, __func__,"");
                    }
                }
                else
                {
                    /* Clone the data to keep the copy at new context as this 
                     * will be removed once old context is deleted after 
                     * forwarding */
                    UInt8 *data_p = msgClone(dlFwdData_p->data_p);
                    if( PNULL == data_p )
                    {
                        LOG_PDCP_MSG( PDCP_MSG_CLONE_FAILED, LOGFATAL, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                msgSize(dlFwdData_p->data_p, PNULL),
                                dlFwdData_p->commDataReq.ueIndex, 
                                dlFwdData_p->commDataReq.lcId,
                                dlFwdData_p->commDataReq.sn, 
                                0,0, __func__,"");
                    }
                    else
                    {
                        /* + SPR 17415 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                        /* SPR 20831 Changes Start */
                        UInt32 actIdx = gMacStats.pStats->lteCommonUeKPIStats.
                            activeIndexForPdcpThpStats;
                        LtePdcpKPIThpStats *kpiThpStat_p = &(gMacStats.
                                pStats->lteCommonUeKPIStats.ltePdcpKPIThpStats[actIdx]);
                        /* SPR 20831 Changes End */
                        /* DlSduBitRate needs to be calculated at target eNB 
                         * and so the stats will be decremented from here for
                         * Intra eNB HO */
                        if( kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] >= 
                                dlFwdData_p->commDataReq.size )
                        {
                            kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] -= 
                                dlFwdData_p->commDataReq.size;
                        }
                        else
                        {
                            kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] = 0;
                        }
#endif
                        /* - SPR 17415 */
                        /* Perform internal forwarding, i.e., copy the DL data
                         * to new context */
                        pdcpTargetDlDataRelocReq( ueCtx_p->newUeIndex, 
                                dlFwdData_p->commDataReq.lcId, 
                                dlFwdData_p->commDataReq.sn, data_p,
                                dlFwdData_p->commDataReq.size,0);
                    }
                }
                /* SPR 10894 fix end */
                GET_NEXT_PDCP_TX_STORAGE_Q(pdcpCtx_p, dlFwdData_p);
            } while ( PNULL != dlFwdData_p);
        }
	}
   
    pdcpCtx_p->dataNode = PNULL;
    pdcpCtx_p->numberOfNodesFwded = 0;
#ifdef LOG_PRINT_ENABLED 
    state = ueCtx_p->pdcpTxSrb2DrbState;
#endif
    /* Forwards new PDCP SDUs that have not yet been processed by PDCP */
    (void)pdcpTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState][PDCP_SOURCE_DRB_FWD_TX](
                     ueIndex, lcId, ueCtx_p->pdcpTxSrb2DrbState, 
                     PDCP_SOURCE_DRB_FWD_TX, pdcpCtx_p );
    LOG_PDCP_MSG( PDCP_TX_EVENT_RCVD, LOGDEBUG, PDCP_TX, pdcpCurrentTime_g,
             __LINE__, ueIndex, state, PDCP_SOURCE_DRB_FWD_TX, ueCtx_p->pdcpTxSrb2DrbState, lcId,0, __func__,
             "PdcpTxStateMachine");
}

/****************************************************************************
 * Function Name  : pdcpFwdDlFreshSdu
 * Inputs         : ueIndex - UE Id, 
 *                  lcId - logical Channel Id, 
 *                  state, event, pointer to PdcpEntityContext Structure. 
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : This function will forward all the downlink packets (only
 *                  fresh SDUs).
 ****************************************************************************/
UInt32 pdcpFwdDlFreshSdu( UInt16 ueIndex, UInt8 lcId, UInt32 state, 
        UInt32 event, void *context_p )
{
    LP_PdcpUeContext ueCtx_p     = PDCP_GET_VALID_UE_CONTEXT( ueIndex );
    LP_PdcpDataReq   dlFwdData_p = PNULL;    
    LP_PdcpEnityContext pdcpCtx_p = (LP_PdcpEnityContext)context_p;
    UInt32           qCount  = QCOUNT_PDCP_TX_DATA_REQ_Q(pdcpCtx_p);
    UInt32           qCounter = 0;
    
    if (qCount < pdcpCtx_p->numberOfNodesFwded)
    {
        pdcpCtx_p->numberOfNodesFwded = qCount;
    }
    /* SPR 10111 Fix End */
    qCount = ( (qCount - pdcpCtx_p->numberOfNodesFwded) > 
            MAX_NODES_FWDED_AT_TIME ) ? MAX_NODES_FWDED_AT_TIME :
        (qCount - pdcpCtx_p->numberOfNodesFwded);
    qCounter = qCount;

    if ( 0 < qCount )
    {
        if ( 0 == pdcpCtx_p->numberOfNodesFwded )
        {
            GET_FIRST_PDCP_TX_DATA_REQ_Q(pdcpCtx_p, dlFwdData_p);
        }
        else
        {
            GET_NEXT_PDCP_TX_DATA_REQ_Q(pdcpCtx_p,  pdcpCtx_p->dataNode );
            dlFwdData_p = pdcpCtx_p->dataNode;
        }
        if ( PNULL == dlFwdData_p )
        {
            /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, ueIndex, lcId,event,state, 0,0, __func__,
                    "pdcpTxDataReqQ");            
        }
        else
        {
            while (qCount--)
            {
                LOG_PDCP_MSG( PDCP_SDU_FWDED_TO_PACKET_RELAY, LOGDEBUG, PDCP_TX,
                        pdcpCurrentTime_g, __LINE__, 
                        dlFwdData_p->commDataReq.ueIndex,
                        dlFwdData_p->commDataReq.lcId,
                        dlFwdData_p->commDataReq.sn, 
                        dlFwdData_p->commDataReq.size,
                        0,0, __func__, "");
                /* SPR 10894 fix start */
                if( NORMAL_HO == ueCtx_p->hoType )
                {
                    /* Forward the packet to packet relay to be forwarded to 
                     * SGW */
                    if ( PDCP_SUCCESS != pdcpSourceDlDataRelocInd(
                                dlFwdData_p->commDataReq.ueIndex, 
                                dlFwdData_p->commDataReq.lcId, 
                                dlFwdData_p->commDataReq.sn, 
                                dlFwdData_p->data_p,
                                dlFwdData_p->commDataReq.size) )
                    {
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
                        /* EVENT: EVENT_PARAM_PER_DRB_PACKET_LOST_HO_DL , 
                         * Number of DL PDCP SDUs discarded due to HO per DRB*/
                        gPdcpUePerfStats_p[dlFwdData_p->commDataReq.ueIndex].
                            lteUePdcpDLPerfStats.
                            totalDlDrbSduDiscardedHO[lcId - 3]++;
#endif
                        LOG_PDCP_MSG( PDCP_DL_DATA_FWD_FAILS, LOGWARNING, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                dlFwdData_p->commDataReq.ueIndex,
                                dlFwdData_p->commDataReq.lcId,
                                dlFwdData_p->commDataReq.sn, 
                                dlFwdData_p->commDataReq.size,
                                0,0, __func__,"");
                    }
                }
                else
                {
                    /* Clone the data to keep the copy at new context as this 
                     * will be removed once old context is deleted after 
                     * forwarding */
                    UInt8 *data_p = msgClone(dlFwdData_p->data_p);
                    if( PNULL == data_p )
                    {
                        LOG_PDCP_MSG( PDCP_MSG_CLONE_FAILED, LOGFATAL, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                msgSize(dlFwdData_p->data_p, PNULL),
                                dlFwdData_p->commDataReq.ueIndex, 
                                dlFwdData_p->commDataReq.lcId,
                                dlFwdData_p->commDataReq.sn, 
                                0,0, __func__,"");
                    }
                    else
                    {
                        /* + SPR 17415 */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                        /* SPR 20831 Changes Start */
                        UInt32 actIdx = gMacStats.pStats->lteCommonUeKPIStats.
                            activeIndexForPdcpThpStats;
                        LtePdcpKPIThpStats *kpiThpStat_p = &(gMacStats.
                                pStats->lteCommonUeKPIStats.
                                ltePdcpKPIThpStats[actIdx]);
                        /* SPR 20831 Changes End */
                        /* DlSduBitRate needs to be calculated at target eNB 
                         * and so the stats will be decremented from here for
                         * Intra eNB HO */
                        if( kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] >= 
                                dlFwdData_p->commDataReq.size )
                        {
                            kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] -= 
                                dlFwdData_p->commDataReq.size;
                        }
                        else
                        {
                            kpiThpStat_p->SharedMemoryDLPDCPSDU[ueIndex]
                                [lcId - LTE_DRB_LC_ID_LOW_BOUND] = 0;
                        }
#endif
                        /* - SPR 17415 */
                        /* Perform internal forwarding, i.e., copy the DL data
                         * to new context */
                        pdcpTargetDlDataRelocReq( ueCtx_p->newUeIndex, 
                                dlFwdData_p->commDataReq.lcId, 
                                dlFwdData_p->commDataReq.sn, data_p, 
                                dlFwdData_p->commDataReq.size,0);
                    }
                }
                /* SPR 10894 fix end */
                pdcpCtx_p->dataNode = dlFwdData_p;

                GET_NEXT_PDCP_TX_DATA_REQ_Q(pdcpCtx_p, dlFwdData_p);
                if ( PNULL == dlFwdData_p )
                {
                    break;
                }
            }
        }
    }
    pdcpCtx_p->numberOfNodesFwded += qCounter;
  
    /* SPR 10894 fix start */
    if ( LTE_TRUE == pdcpCtx_p->isEndMarkerIndRecvd && 
            ( pdcpCtx_p->numberOfNodesFwded == 
              QCOUNT_PDCP_TX_DATA_REQ_Q(pdcpCtx_p) ) )
        /* SPR 10894 fix end */
    {
        /* Send local EMI to Packet Relay */
        /* SPR 3085 Changes Start */
        pdcpEndOfDataForwardingInd( ueIndex, lcId, 
                pdcpCtx_p->isEndMarkerIndRecvd);
        /* SPR 3085 Changes End */
        /* SPR 3225 changes start */
        pdcpCtx_p->isEMIFwded = LTE_TRUE;
        /* SPR 3225 changes end */
        /* Check if Delete Request is already received from L3 and contexts
         * could not be deleted because of ongoing forwarding */
        if ( 0 != ueCtx_p->tickAss )
        {
            /* Delete the TX context */
            LOG_PDCP_MSG( PDCP_DELETE_RB_AFTER_EMI, LOGDEBUG, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0,
                    __func__, "");
            /* Delete LC at TX side */
            /* SPR 3852 changes start */
#ifdef PDCP_ASYNC_INTERFACE
            pdcpDeleteSecurityContext( pdcpCtx_p->pdcpSecurityContext_p );
            /* +- SPR 15236 */
            pdcpCtx_p->pdcpSecurityContext_p = PNULL;
#endif
            /* SPR 3852 changes end */
            /* SPR 3159 changes start */
            pdcpEntityContextFree( pdcpCtx_p, ueIndex, lcId, 
                                  PDCP_ENTITY_RB_TX );
            /* SPR 3159 changes end */

            /*SPR 21128 Start*/
            semDestroy((LTE_SEM *)&(pdcpCtx_p->pdcpTxDataReqQ).sem_lock);
            semDestroy((LTE_SEM *)&(pdcpCtx_p->pdcpStorageQ).sem_lock);
            /*SPR 21128 End*/

            freeMemPool( pdcpCtx_p );
            /* SPR 3159 changes start */
            ueCtx_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_TX] = PNULL;
            /* SPR 3159 changes end */
            pdcpCtx_p = PNULL;

            /* SPR 5485 changes start */
            /* UL Data is already forwarded, so delete RX context also */
            pdcpCtx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, lcId, 
                           PDCP_ENTITY_RB_RX ); 

            LOG_PDCP_MSG( PDCP_DELETE_RB_AFTER_EMI, LOGDEBUG, PDCP_RX, 
                    pdcpCurrentTime_g, __LINE__, ueIndex,
                    lcId, 0,0, 0,0, __func__,"");

            /* Delete LC at RX side */
            /* SPR 3852 changes start */
#ifdef PDCP_ASYNC_INTERFACE
            pdcpDeleteSecurityContext( pdcpCtx_p->pdcpSecurityContext_p );
            /* +- SPR 15236 */
            pdcpCtx_p->pdcpSecurityContext_p = PNULL;
#endif
            /* SPR 3852 changes end */
            /* SPR 3159 changes start */
            pdcpEntityContextFree( pdcpCtx_p, ueIndex, lcId , 
                                   PDCP_ENTITY_RB_RX );
            /* SPR 3159 changes end */

            /*SPR 21128 Start*/
            DEINIT_PDCP_RX_DATA_IND_Q(pdcpCtx_p);
            semDestroy((LTE_SEM *)&(pdcpCtx_p->pdcpUlReorderingQ).sem_lock);
            /*SPR 21128 End*/

            freeMemPool( pdcpCtx_p );
            /* SPR 3159 changes start */
            ueCtx_p->pdcpEntities[lcId - 1][PDCP_ENTITY_RB_RX] = PNULL;
            /* SPR 3159 changes end */
            pdcpCtx_p = PNULL;	
            /* SPR 5485 changes end */

            /* Updates the list of active DRBs as both the contexts are 
             * deleetd */
            updateActiveLcInfo( ueIndex, lcId, ueCtx_p->activeLcInfo,
                                ueCtx_p->activeLcIndex );
            pdcpStats.statsUEs[ueIndex].statNumActiveDrbLc -= 1;
            ueCtx_p->numOfActiveDrbLc -= 1;
        
            /* SPR 5485 changes start */
            /* Delete the UE-Context if no more active DRBs are available */
            if ( 0 == ueCtx_p->numOfActiveDrbLc )
            {
                /* Deallocate memory for UE Context */
                freeMemPool( pdcpContext.ueContexts[ueIndex] );
                pdcpContext.ueContexts[ueIndex] = PNULL;
                /* Updates the list of active UEs*/
                updateActiveUeInfo( ueIndex,
                        pdcpContext.activeUeInfo, 
                        pdcpContext.activeUeIndex 
                        );

                /*reset UE Context*/
                pdcpContext.numOfActiveUe -= 1;
                pdcpStats.statNumActiveUe -= 1;
            }
            /* SPR 5485 changes end */
        }
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : pdcpEndMarkerInd
 * Inputs         : ueIndex, lcId.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function will receive EMI from packet relay to 
 *                  indicate last packet received at source PDCP.
 ****************************************************************************/
void pdcpEndMarkerInd( UInt16 ueIndex, UInt8 lcId,UInt8 instanceId )
{
    LP_PdcpEnityContext   ctx_p  = PNULL;
   
    do
    {        
        /* Check ueIndex and lcId parameters */
        if ( (MAX_UE_SUPPORTED <= ueIndex) || 
                (LTE_DRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < lcId) )
        {
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__, lcId, ueIndex, 0,0, 0,0, __func__,"");
            break;
        }
        
        ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_TX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, instanceId,0, 0,0, 
                    __func__, "");
            break;
        }
        ctx_p->isEndMarkerIndRecvd = LTE_TRUE;

        ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_RX);
        if ( PNULL == ctx_p )
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RX,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0, 
                    __func__, "");
            break;
        }
        ctx_p->isEndMarkerIndRecvd = LTE_TRUE;
        
        LOG_PDCP_MSG( PDCP_EMI_RECVD, LOGDEBUG, ( PDCP_TX | PDCP_RX ), 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0, 
                __func__, "");

    }while (0);
}
