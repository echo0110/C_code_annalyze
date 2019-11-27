/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcMacInterface.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:18 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *  This file contains the implementation of interface APIs for MAC.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcMacInterface.c,v $
*
*
****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcMacInterface.h"
#include "lteRlcEntityDef.h"
#include "lteRlcUeContext.h"
/*RLC Rel 2.0 Changes Start*/
#include "lteRlcAmdArqOperation.h"
/*RLC Rel 2.0 Changes End*/

#include "lteRlcPdcpInterface.h"

#include "logging.h"
#include "alarm.h"
#include "lteCommonStatsManager.h"
#include "lteRlcStat.h"
#include "lteRlcErrorCode.h"

#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
#include "lteRlcEmbmsDef.h"
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#ifdef PDCP_THROUGHPUT_STATS
UInt32 rlcDataTx = 0;
UInt32 rlcPktTx = 0;
#endif
/* This queue will be data interface from MAC layer to RLC layer. */
/** CA 2 Core Changes Start **/
MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELL] ;

#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
MAC_UE_DATA_IND_TYPE macEmbmsDataIndQ_g[MAX_NUM_CELL] ;
#endif
/** CA 2 Core Changes End **/
/* This queue will be harq failure reporting  interface from MAC layer to RLC 
   layer. */
/* Ti Warning Fix start*/
/* CA Changes start */
MAC_UE_OPPOR_IND_CIRC_TYPE* macUeOpporIndCircQ_gp[MAX_NUM_CELL] = {PNULL};
extern SEM_T waitForMacUeOpporInd_g[MAX_NUM_CELL];
/* CA Changes end */
/*RLC Rel 2.0 Changes Start*/
/* This queue will be harq failure reporting interface from MAC layer to RLC 
   layer. */
LTE_SQUEUE macUeHarqFailureIndQ_g;
/*RLC Rel 2.0 Changes End*/

/* This queue will be common channel data interface from MAC layer to RLC 
   layer. */


#ifdef DL_UL_SPLIT
LTE_SEM rlcTxOppertunityIndSem_g;
#endif

/* warning removed*/
#if (defined(DL_UL_SPLIT) && !defined(UE_SIM_TESTING))
void putEntryInMACUeOppIndQ(const char* ueOppInd_p, UInt32 size,
                   InternalCellIndex cellIndex);
#endif
extern UInt16 rlcLayerInitialized_g;
/* SPR 15909 fix Start */
extern tickType_t globalTTITickCount_g;
/*SPR 18697 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
tickType_t embmsPlatformTick_g;
#endif
/*SPR 18697 fix end*/ 
/* SPR 15909 fix end */

/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
/* SPR 15909 fix Start */
extern tickType_t rlcGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
#endif
extern UInt8 numOfRlcCellConfigured_g;
/*  - Layer2 NON CA Changes*/
SInt32 semPostDefault(SEM_T *pSem);
#ifndef UE_SIM_TESTING
/* This queue will be Seg Pdu reporting i/f from RLC to MAC */
LTE_CIRCQUEUE rlcMacPduIndQ_g[MAX_NUM_CELL];
#endif

#ifdef DL_DATA_SEPARATION
#ifdef SINGLE_NOTIF
static RlcBool sendSignalFlag_g = RLC_FALSE;
/* CA Changes start */
extern SEM_T waitForPdschInd_g[MAX_NUM_CELL];
#endif
extern SInt32 ev_fd_g[MAX_NUM_CELL];
/* CA Changes end */
#endif


/****************************************************************************
 * Function Name  : macUeDataInd
 * Inputs         : ueIndex - UE Identifier ,
 *                  dataToRlc_p - Pointer to data received from MAC
 * Outputs        : None 
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API will be used by MAC to send data to RLC.
 ****************************************************************************/

UInt32 macUeDataInd(UInt16 ueIndex, RLCUplinkData* dataToRlc_p) 
{
    CLOCK_START_RLC_API(MAC_UE_DATA_IND);
    UInt16 lIdx = 0 ;
    UInt16 numOfLC = 0 ;
#ifdef RLC_QUEUE_STATS
    UInt16 qCount =0;
#endif 
    if((RLC_NULL == dataToRlc_p))
    {
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }
    /** CA 2 Core Changes Start **/
    UEContext *ueContext_p             = RLC_NULL ;
    /* + SPR 13605 Fix */
    if ((( ueContext_p =
                DB_FETCH_UE_CONTEXT(ueIndex)) == RLC_NULL) ||
          (ueContext_p->rlcUEState != RLC_UECONTEXT_ACTIVE ))
    /* - SPR 13605 Fix */
    {
        LOG_RLC_MSG(LTE_RLC_INVALID_UE_INDEX_OR_LC, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                        ueIndex, numOfLC, rlcLayerInitialized_g, 0, 0,0, __func__, "");
        for(lIdx = 0;lIdx < dataToRlc_p->numLCInfo; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
            /* SPR 21754 Fix + */
            dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer = PNULL;
            /* SPR 21754 Fix - */
        }
        freeMemPool(dataToRlc_p);
        return  RLC_FAILURE;
    } 
    
    /** CA 2 Core Changes End **/
    numOfLC = dataToRlc_p->numLCInfo ;
    /** SPR 3657 Changes Start **/
    if((RLC_FAILURE == rlcLayerInitialized_g) || (ueIndex >= MAX_UE_ID) ||
            (0 == numOfLC) /*|| (numOfLC >= MAX_LC_ID) */) 
    /** SPR 3657 Chanegs End **/
    {

	 LOG_RLC_MSG(LTE_RLC_INVALID_UE_INDEX_OR_LC, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                        ueIndex, numOfLC, rlcLayerInitialized_g, 0, 0,0, __func__, "");
    /** SPR 3657 Changes Start **/
    /** SPR 3657 Changes End **/
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
            /* SPR 21754 Fix + */
            dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer = PNULL;
            /* SPR 21754 Fix - */
        }
        freeMemPool(dataToRlc_p);
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }
    MacUeDataInd* ueDataInd_p = PNULL;
    GET_MEM_NODE_MAC_UE_DATA_IND_Q(ueDataInd_p, MacUeDataInd);
    if(RLC_NULL == ueDataInd_p) 
    {
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        /** CA 2 Core Changes Start **/
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
            /* SPR 21754 Fix + */
            dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer = PNULL;
            /* SPR 21754 Fix - */
        }
        freeMemPool(dataToRlc_p);
        /** CA 2 Core Changes End **/
        return RLC_FAILURE;
    }    

    ueDataInd_p->ueIndex = ueIndex ;
    ueDataInd_p->dataToRlc_p = dataToRlc_p ;
/*SPR 99999 Queue full check start*/
    /** CA 2 Core Changes Start **/
    /* SPR 23483 Changes Start */
    if(CIRC_FAILURE == ENQUEUE_MAC_UE_DATA_IND_Q(MacUeDataInd, ueDataInd_p,ueContext_p->cellIndex))
        /* SPR 23483 Changes End */
    /** CA 2 Core Changes End **/
    {
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
            dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer = PNULL;
        }
        freeMemPool(dataToRlc_p);
        freeMemPool(ueDataInd_p);
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }

/*SPR 99999 Queue full check end*/

#ifdef RLC_QUEUE_STATS
      qCount =QCOUNT_MAC_UEDATA_IND_Q();
      lteMacUpdateStatsQueue(MAC_UEDATA_IND_Q,qCount);
#endif
/* SPR 3160 Fix Start */
#ifdef RLC_STANDALONE_SETUP
    semPost(&rlcRxSemlock_g);
#endif
/* SPR 3160 Fix End */
    CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
    return RLC_SUCCESS;
}

/****************************************************************************
 * Function Name  : macUeOpportunityInd
 * Inputs         : ueIndex - UE Identifier ,
 *                  lcCount - Number of LCs for which MAC provides the
 *                            transmission opportunity to RLC to send data,
 *                  rlcBuffer_p - Data buffer to be filled by RLC for the
 *                                number of lcCount if available,
 *                  requestedSubFrame - Sub Frame after which response will
 *                                      be rejected by MAC Layer,
 *                  cellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API will be used by MAC to send transmission 
 *                  opportunity to RLC.
 ****************************************************************************/

UInt32 macUeOpportunityInd(UInt16 ueIndex, UInt8 lcCount,
        RLCDataBuffer* rlcBuffer_p, UInt32 requestedSubFrame
        ,InternalCellIndex cellIndex)
{
    /** We have get the CellIndex From UeContext based on ueIndex **/ 

    CLOCK_START_RLC_API(MAC_UE_OPPOR_IND);
    if((RLC_NULL == rlcBuffer_p))
    { 
        /* SPR 16766 fix start */
        LOG_RLC_MSG(LTE_RLC_INVALID_UE_INDEX_OR_LC, LOGWARNING, RLC_RX, 
                globalTTITickCount_g, __LINE__, 0, 0, 0, 0, 0,0, __func__, "");
        /* SPR 16766 fix end */
        CLOCK_END_RLC_MAC_API(MAC_UE_OPPOR_IND);
        return RLC_FAILURE ;
    }
    if (( RLC_FAILURE == rlcLayerInitialized_g || ueIndex > MAX_UE_ID || 
	lcCount >= MAX_LC_ID ) || ((lcCount==0) && (ueIndex != MAX_UE_ID)) || 
	((lcCount != 0) && (MAX_UE_ID == ueIndex)))
    {
	LOG_RLC_MSG(LTE_RLC_INVALID_UE_INDEX_OR_LC, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                        ueIndex, lcCount, rlcLayerInitialized_g, 0, 0,0, __func__, "");
        CLOCK_END_RLC_MAC_API(MAC_UE_OPPOR_IND);
        return RLC_FAILURE ;
    }
    MacUeOpporInd* ueOppInd_p = PNULL;
    GET_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p, MacUeOpporInd);
    if(RLC_NULL == ueOppInd_p)
    {
        /* SPR 16766 fix start */
        LOG_RLC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID, LOGWARNING, L2_SYS_FAIL, 
                globalTTITickCount_g, __LINE__, sizeof(MacUeOpporInd), 0, 0, 0, 
                0,0, __FILE__, __func__ );
        /* SPR 16766 fix end */
        CLOCK_END_RLC_MAC_API(MAC_UE_OPPOR_IND);
        return RLC_FAILURE ;
    }    

    ueOppInd_p->ueIndex = ueIndex ;
    ueOppInd_p->lcCount = lcCount ;
    ueOppInd_p->rlcBuffer_p = rlcBuffer_p;
    ueOppInd_p->requestedSubFrame = requestedSubFrame;
#if (defined(DL_DATA_SEPARATION) && !defined(UE_SIM_TESTING))
/* CA Changes start */
/* + SPR 9253 */ 
    if(!ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd,(void *)&(ueOppInd_p->sNode), cellIndex))
    {
		LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                   ueIndex, 0, 0, 0, 0,0, __func__, "ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q Failed ");
        	FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);
		freeMemPool(rlcBuffer_p); 
		rlcBuffer_p = PNULL;
    }
/* - SPR 9253 */
/* CA Changes end */

/* SPR 8712 changes */    
#ifdef SINGLE_NOTIF
    if (sendSignalFlag_g==0)
    { 
        /* CA Changes start */
        eventfd_write_wrapper(ev_fd_g[cellIndex],1);
        /* CA Changes end */
        sendSignalFlag_g=RLC_TRUE;
   } 
    /* If dummy node sent to RLC Encoder, the reset flag */
    if ((MAX_UE_ID == ueIndex) || ( 0 == lcCount))
    {
        sendSignalFlag_g = RLC_FALSE;
    }
#else
    eventfd_write_wrapper(ev_fd_g[cellIndex],1);
#endif
/* SPR 8712 changes */    
#elif (defined(DL_UL_SPLIT) && !defined(UE_SIM_TESTING))
/* Ti Warning Fix start*/ 
#ifdef CIRC_QUEUE_IMPL
/* CA Changes start */
/* + SPR 9253 */
    if(!ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd,(void *)&(ueOppInd_p->sNode), cellIndex))
    {
		LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                   ueIndex, 0,0, 0, 0,0, __func__, "ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q Failed ");
     	   	FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);
		freeMemPool(rlcBuffer_p); 
		rlcBuffer_p = PNULL;

    }
/* - SPR 9253 */
    semPostDefault(&waitForMacUeOpporInd_g[cellIndex]);
#else
    putEntryInMACUeOppIndQ((const char*)ueOppInd_p, sizeof(MacUeOpporInd), cellIndex);
/* CA Changes end */
#endif
/* Ti Warning Fix end*/ 
#elif !defined(UE_SIM_TESTING)
    /* CA Changes start */
    /* + Coverity Id - 31243 */
   /* + SPR 9253 */
   if(!ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd,(void *)&(ueOppInd_p->sNode),
           cellIndex))
   {
		LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                   ueIndex, 0,0, 0, 0,0, __func__, "ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q Failed ");
        	FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);

        /*SPR 21723 Fix Start */
        if((INVALID_UE_ID==ueIndex)&&(0==lcCount))
        {
            //Do not free the memory as i is taken from global
        }
        else
        {
            freeMemPool(rlcBuffer_p);
        }
        /*SPR 21723 Fix End */

    	/* + Coverity 72764 */
		//rlcBuffer_p = PNULL;
    	/* - Coverity 72764 */
    }
    
    /* - Coverity Id - 31243 */
#else
    /* + Coverity Id - 31243 */
    if(!ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd,(void *)&(ueOppInd_p->sNode), cellIndex))
    {
       	        LOG_RLC_MSG(LTE_RLC_GET_MEM_FROM_POOL_FAIL, LOGWARNING, RLC_MAC, globalTTITickCount_g, __LINE__,
                   ueIndex, 0,0, 0, 0,0, __func__, "ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q Failed ");
        	FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(ueOppInd_p);
		freeMemPool(rlcBuffer_p); 
		rlcBuffer_p = PNULL;
    }
    /* - SPR 9253 */
    /* - Coverity Id - 31243 */
/* CA Changes end */
#endif    
    CLOCK_END_RLC_MAC_API(MAC_UE_OPPOR_IND);
    return RLC_SUCCESS;
}

/* SPR 1535 Changes Start*/
/****************************************************************************
 * Function Name  : macFirstSubFrameInd
 * Inputs         : sysFrameNum,
 *                  subFrameNum,
 *                  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This API will be used by MAC to update current tick.
 ****************************************************************************/
/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
void  macFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum,InternalCellIndex internalCellIndex)
#else
void  macFirstSubFrameInd(UInt32 sysFrameNum, UInt32 subFrameNum)
#endif
/*  - Layer2 NON CA Changes*/
{
/* SPR 3160 Fix Start */
#ifdef RLC_STANDALONE_SETUP
    semPost(&rlcCurrentTickSemlock_g);
#endif
/* SPR 3160 Fix End */   
/* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    rlcGlobalTTITickCount_g[internalCellIndex] = sysFrameNum * 10 + subFrameNum;
    /* this global TTI is updated for Logging 
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        globalTTITickCount_g = sysFrameNum * 10 + subFrameNum;
    }
    sendRlcFirstSubframeIndToPdcp (sysFrameNum, subFrameNum, internalCellIndex, layer2CommonGetActiveCellTick());
#else 
    globalTTITickCount_g = sysFrameNum * 10 + subFrameNum;
    rlcFirstSubFrameInd(sysFrameNum, subFrameNum );
#endif
/* - Layer2 NON CA Changes */
    return ;
}
/* SPR 1535 Changes End*/

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/* SPR 1535 Changes Start*/
/****************************************************************************
 * Function Name  : macFirstSubFrameIndFrmL2Tick
 * Inputs         : currentTickFrmL2Tick                  
 * Outputs        : None
 * Returns        : None
 * Description    : This API will be used by MAC to update current tick of RLC.
 ****************************************************************************/
/* SPR 15909 fix start */
void  macFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick)
/* SPR 15909 fix end */
{
/* SPR 3160 Fix Start */
#ifdef RLC_STANDALONE_SETUP
    semPost(&rlcCurrentTickSemlock_g);
#endif
/* SPR 3160 Fix End */   
    globalTTITickCount_g = currentTickFrmL2Tick;
#ifndef UE_SIM_TESTING
    rlcFirstSubFrameIndFrmL2Tick(globalTTITickCount_g);
#endif
    return ;
}
/* SPR 1535 Changes End*/
#endif
/* FAPI L2 AS MASTER END */

/****************************************************************************
 * Function Name  : macSubFrameInd
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : RLC_SUCCESS
 * Description    : This API will be used by MAC to update current tick.

 ****************************************************************************/
#ifndef UE_SIM_TESTING
UInt32 macSubFrameInd(InternalCellIndex internalCellIndex)
#else
UInt32 macSubFrameInd()
#endif
{
#ifdef RLC_STANDALONE_SETUP
    semPost(&rlcCurrentTickSemlock_g);
#endif
#ifndef UE_SIM_TESTING
    rlcGlobalTTITickCount_g[internalCellIndex]++;
    /* this global TTI is updated for Logging 
     * This will be updated only for first cell which is active for
     * tick updation */
    if (internalCellIndex == layer2CommonGetActiveCellTick())
    {
        globalTTITickCount_g++;
/*SPR 18697 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
	embmsPlatformTick_g++;
#endif
/*SPR 18697 fix end*/ 
        updateAndResetThrputStats(globalTTITickCount_g,THRPUT_DURATION);
#ifdef PDCP_THROUGHPUT_STATS
        if( 0 == globalTTITickCount_g % 5000 )
        {
#ifdef PRINT
            fprintf(stderr, "RLC THP: TX: %03f bps, Num Pkt Tx: %u\n", ((((float)rlcDataTx)/5)*8), rlcPktTx);
#endif
            rlcDataTx = 0;
            rlcPktTx = 0;
        }
#endif
    }
    sendRlcSubframeIndToPdcp (internalCellIndex);
#else
    globalTTITickCount_g++;
/*SPR 18697 fix start*/ 
#ifdef LTE_EMBMS_SUPPORTED
    embmsPlatformTick_g++;
#endif
/*SPR 18697 fix end*/ 
    rlcSubFrameInd();
#endif
    return RLC_SUCCESS;
}

#ifdef OBSELETE_REL_8
/****************************************************************************
 * Function Name  : macUeHarqFailureInd
 * Inputs         : ueIndex - UE Identifier,
 *                  rlcHarqFailureInfo_p - Pointer to RLCHarqFailureInfo
 * Outputs        : None
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API will be used by MAC to send data to RLC.
 ****************************************************************************/

UInt32 macUeHarqFailureInd(UInt16 ueIndex, 
                             RLCHarqFailureInfo* rlcHarqFailureInfo_p) 
{
    UInt32 loopCount = 0;
    CLOCK_START_RLC_API(MAC_UE_HARQ_FAILURE_IND);
    if((RLC_SUCCESS == rlcLayerInitialized_g) && 
          (RLC_NULL != rlcHarqFailureInfo_p) &&
          (ueIndex < MAX_UE_ID) && 
          ((rlcHarqFailureInfo_p[0].lcId) < MAX_LOGICAL_CHANNEL))
    {
        if(INVALID_SEQ_NUM != rlcHarqFailureInfo_p[0].newPduSeqNum)
        {
            MacUeHarqFailureInd* ueFailureInd_p =
            (MacUeHarqFailureInd*)getMemFromPool(sizeof(MacUeHarqFailureInd),
             RLC_NULL);
            if(RLC_NULL == ueFailureInd_p)
            {
                CLOCK_END_RLC_MAC_API(MAC_UE_HARQ_FAILURE_IND);
                return RLC_FAILURE ;
            }
            ueFailureInd_p->ueIndex = ueIndex;
            ueFailureInd_p->lcId = rlcHarqFailureInfo_p[0].lcId;
            ueFailureInd_p->newPduSeqNum = rlcHarqFailureInfo_p[0].newPduSeqNum;
            /** SPR 3657 Changes Start **/
            ueFailureInd_p->reTxPduCount = rlcHarqFailureInfo_p[0].reTxPduCount;
            for( ;loopCount < rlcHarqFailureInfo_p[0].reTxPduCount;loopCount++)
            {
                ueFailureInd_p->harqFailReTxInfo[loopCount].retxPduSeqNum = 
                    rlcHarqFailureInfo_p[0].harqFailReTxInfo[loopCount].retxPduSeqNum;
                ueFailureInd_p->harqFailReTxInfo[loopCount].SOStart = 
                    rlcHarqFailureInfo_p[0].harqFailReTxInfo[loopCount].SOStart;
                ueFailureInd_p->harqFailReTxInfo[loopCount].SOEnd   = 
                    rlcHarqFailureInfo_p[0].harqFailReTxInfo[loopCount].SOEnd;
            }
            /** SPR 3657 Changes Start **/

            /* + Coverity 32533 */
            if (ENQUEUE_MAC_UEHARQ_FAILURE_IND_Q(ueFailureInd_p))
            {
                freeMemPool(ueFailureInd_p);
                return RLC_FAILURE ;
            }
            /* - Coverity 32533 */            
        }
/* SPR 3160 Fix Start */
#ifdef RLC_STANDALONE_SETUP
        semPost(&rlcRxSemlock_g);
#endif 
/* SPR 3160 Fix End */       
        CLOCK_END_RLC_MAC_API(MAC_UE_HARQ_FAILURE_IND);
        return RLC_SUCCESS;
    }   
    CLOCK_END_RLC_MAC_API(MAC_UE_HARQ_FAILURE_IND);
    return RLC_FAILURE;
}

/*RLC Rel 2.0 Changes End*/
#endif

/****************************************************************************
 * Function Name  : rlcInitMacInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This initializes the MAC interface.
 ****************************************************************************/
/* + SPR 17439 */
void rlcInitMacInterface(void)
/* - SPR 17439 */    
{
/* CA Changes start */
    InternalCellIndex cellCount = 0;

    for (cellCount = 0; cellCount < numOfRlcCellConfigured_g; cellCount++)
    {
#ifndef UE_SIM_TESTING
        QINIT_RLC_MAC_PDU_IND_Q(cellCount);
#endif
        INIT_MAC_UE_DATA_IND_Q(cellCount);
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))        
        INIT_MAC_EMBMS_DATA_IND_Q(cellCount); 
#endif
        CONSTRUCT_AND_INIT_MAC_UE_OPPOR_IND_CIRC_Q(cellCount);
    }
    /* CA Changes end */

    QINIT_RLC_CONTROL_PDU_Q();
    QINIT_MAC_UE_HARQ_FAILURE_IND_Q();
    
}

/****************************************************************************
 * Function Name  : rlcDeInitMacInterface
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This deinitializes the MAC interface queue.
 ****************************************************************************/
/* + SPR 17439 */
void rlcDeInitMacInterface(void)
/* - SPR 17439 */    
{
/* CA Changes start */
    InternalCellIndex count;
/* CA Changes end */

    /* CA Changes start */
    for (count=0; count < numOfRlcCellConfigured_g; count++)
    {
        QDEINIT_MAC_UEDATA_IND_Q(count) ;
        QDEINIT_MAC_UEOPPOR_IND_Q(count);
    }
    /* CA Changes end */

    QDEINIT_RLC_CONTROL_PDU_Q();
    QDEINIT_MAC_UE_HARQ_FAILURE_IND_Q();
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : macUeSegPduInd
 * Inputs         : ueIndex - UE Identifier,
 *                  lcId - logical channel identifier,
 *                  rlcPduInd - seg Pdu Indication,
 *                  cellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : Called by RLC when SegCounter reaches to MAX_LIMIT.
 *                  It creates a node and fills the info/data given by RLC
 *                  and puts the node into pdu ind queue.
 ****************************************************************************/
void macUeSegPduInd(UInt32 ueIndex, UInt8 lcId, UInt8 rlcPduInd,InternalCellIndex cellIndex)
{
    RlcMacSegPduInd *rlcMacSegPduInd_p =
        (RlcMacSegPduInd*)getMemFromPool(sizeof(RlcMacSegPduInd),RLC_NULL);

    if( rlcMacSegPduInd_p == NULL )
    {
        ltePanic("getMemFromPool fails %s", __func__);
        /* + Coverity_11440 */
        return;
        /* - Coverity_11440 */
    }

    rlcMacSegPduInd_p->ueIndex = ueIndex;
    rlcMacSegPduInd_p->lcId = lcId;
    rlcMacSegPduInd_p->rlcPduInd = rlcPduInd;

    /*** Coverity 63547 Fix : Start ****/
    if (!ENQUEUE_RLC_MAC_PDU_IND_Q(rlcMacSegPduInd_p,cellIndex))
    {
        freeMemPool(rlcMacSegPduInd_p);
		/* + Coverity 72817 */
        //rlcMacSegPduInd_p = RLC_NULL;
		/* - Coverity 72817 */
        return;
    }
    /*** Coverity 63547 Fix : End ****/
}
#endif
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
/****************************************************************************
 * Function Name  : macUeDataInd
 * Inputs         : ueIndex - UE Identifier ,
 *                  dataToRlc_p - Pointer to data received from MAC
 * Outputs        : None 
 * Returns        : RLC_SUCCESS/RLC_FAILURE
 * Description    : This API will be used by MAC to send data to RLC.
 ****************************************************************************/


UInt32 maceMBMSDataInd(UInt8 areaIndex, RLCUplinkData* dataToRlc_p, UInt8 rxCellIndex) 
{
    CLOCK_START_RLC_API(MAC_UE_DATA_IND);
    UInt16 lIdx = 0 ;
    UInt16 numOfLC = 0 ;
#ifdef RLC_QUEUE_STATS
    UInt16 qCount =0;
#endif 
    if((RLC_NULL == dataToRlc_p))
    {
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }
    RlcEMbmsAreaContext* areaContext_p     = RLC_NULL ;

    
    numOfLC = dataToRlc_p->numLCInfo ;
    
    if((RLC_FAILURE == rlcLayerInitialized_g) || (areaIndex >= MAX_AREA_ID) ||
            (0 == numOfLC) /*|| (numOfLC >= MAX_LC_ID) */) 
    {
#ifdef PRINT
        fprintf(stderr,"Invalid Area Index\n");
#endif
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
        }
        freeMemPool(dataToRlc_p);
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }

    if ((( areaContext_p =
             DB_FETCH_AREA_CONTEXT(areaIndex)) == RLC_NULL) ||
          (areaContext_p->rlcAreaState != RLC_EMBMS_AREA_CONTEXT_ACTIVE ))
    {
#ifdef PRINT
        fprintf(stderr,"Area not present\n");
#endif
        for(lIdx = 0;lIdx < dataToRlc_p->numLCInfo; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
        }
        freeMemPool(dataToRlc_p);
        ltePanic("Area not present in RLC context\n");
        return  RLC_FAILURE;
    }

    MacUeDataInd* areaDataInd_p = PNULL;
    GET_MEM_NODE_MAC_UE_DATA_IND_Q(areaDataInd_p, MacUeDataInd);
    if(RLC_NULL == areaDataInd_p) 
    {
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        /** CA 2 Core Changes Start **/
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
        }
        freeMemPool(dataToRlc_p);
        /** CA 2 Core Changes End **/
        return RLC_FAILURE;
    }    

    areaDataInd_p->ueIndex = areaIndex ;
    areaDataInd_p->dataToRlc_p = dataToRlc_p ;
/*SPR 99999 Queue full check start*/
    /** CA 2 Core Changes Start **/
    if(FAILURE == ENQUEUE_MAC_EMBMS_DATA_IND_Q(MacUeDataInd, areaDataInd_p, rxCellIndex))
    /** CA 2 Core Changes End **/
    {
        for(lIdx = 0;lIdx < numOfLC; lIdx++ )
        {
            msgFree(dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer);
            dataToRlc_p->rlcDataArr[lIdx].rlcdataBuffer = PNULL;
        }
        freeMemPool(dataToRlc_p);
        freeMemPool(areaDataInd_p);
        CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
        return RLC_FAILURE;
    }

/*SPR 99999 Queue full check end*/

#ifdef RLC_QUEUE_STATS
      qCount =QCOUNT_MAC_UEDATA_IND_Q();
      lteMacUpdateStatsQueue(MAC_UEDATA_IND_Q,qCount);
#endif
/* SPR 3160 Fix Start */
#ifdef RLC_STANDALONE_SETUP
    semPost(&rlcRxSemlock_g);
#endif
/* SPR 3160 Fix End */
    CLOCK_END_RLC_MAC_API(MAC_UE_DATA_IND);
    return RLC_SUCCESS;
}
#endif
