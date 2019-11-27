/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpDiscardTimer.c,v 1.1.1.1.16.1 2010/10/07 13:22:34 gur23054 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Implementation for Discard Timer Interface 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpDiscardTimer.c,v $
 * Revision 1.1.1.1.16.1  2010/10/07 13:22:34  gur23054
 * Fix for SPR 713
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.8.2.2  2009/06/19 14:28:20  gur19836
 * Changes Done for Execution Scheduler
 *
 * Revision 1.8.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.8  2009/05/28 13:47:55  gur19140
 * *** empty log message ***
 *
 * Revision 1.7  2009/05/26 15:00:56  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.5  2009/05/20 13:40:56  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/15 05:54:17  gur19836
 * IncorporatedCR Comments
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

#include "ltePdcpTSDefs.h"
#include "ltePdcpDiscardTimer.h"
#include "ltePdcpErrors.h"
#include "logging.h"
#include "ltePdcpContext.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpRlcSap.h"
#include "ltePdcpPdcpUserSap.h"
#include "ltePdcpRrcSap.h"
#include "ltePdcpRrcSap.h"
#ifdef PDCP_ASYNC_INTERFACE
#include "ltePdcpEncSap.h"
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

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING
/* SPR 15909 fix start */
tickType_t pdcpGlobalTTITickCount_g[MAX_NUM_CELL];
/* SPR 15909 fix end */
#endif
/*  - Layer2 NON CA Changes*/
extern PdcpContext pdcpContext;
extern void pdcpEndOfDataForwardingInd(UInt16 ueIndex, UInt8 lcId,
                                           UInt8 isEndMarkerIndRecvd);
/* SPR 15909 fix start */
tickType_t pdcpCurrentTime_g = 0;
/* SPR 15909 fix end */
/****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpProcessDiscardTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Processing queues with timers
 ****************************************************************************/
void pdcpProcessDiscardTimers(void)
{
    UInt16               ueIndex       = 0;
    UInt8                lcId          = 0;
    LP_PdcpEnityContext  ctx_p         = PNULL;
    LP_PdcpDataReq       dataReq_p     = PNULL;
    LP_PdcpDataReq       tempDataReq_p = PNULL;
    LP_PdcpUeContext     ueContext_p   = PNULL;
    UInt16               retVal        = 0;
#ifndef UE_SIM_TESTING 
    /*  + Layer2 NON CA Changes*/
    InternalCellIndex  cellIndex   =  0;
    /*  - Layer2 NON CA Changes*/
#endif        


        while ( ueIndex < pdcpContext.numOfActiveUe)
        {
            /*
             ** activeUeInfo consists only of active UEs, hence valid UE context
             ** will always be available. Therefore, no need to check for NULL check
             */
            /* SPR 5485 changes start */
            ueContext_p = PDCP_GET_VALID_UE_CONTEXT(
                    pdcpContext.activeUeInfo[ueIndex]
                    );

#ifndef UE_SIM_TESTING 
            /*  + Layer2 NON CA Changes*/
            cellIndex   =  ueContext_p->cellIndex;
            /*  - Layer2 NON CA Changes*/
#endif        
            /* SPR 5485 changes end */
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
            /* + KPI_CA Review */
            UInt32 activeIndexForPdcpStats = (gMacStats.
                    pStats->lteCommonKPIStats[cellIndex].activeIndexForPdcpStats);
            LtePdcpKPIStats * pLtePdcpKPIStats = 
                &(gMacStats.pStats->lteCommonKPIStats[cellIndex].ltePdcpKPIStats[activeIndexForPdcpStats]);
            /* - KPI_CA Review */
#endif

            lcId = 0;
            /* Check discard timer for storage queue */
            while ( lcId < ueContext_p->numOfActiveDrbLc)
            {
                /*
                 ** activeLcInfo consists only of active DRBs in an UE, hence valid
                 ** PDCP context will always be available. Therefore, no need to 
                 ** check for NULL check
                 */
                /* SPR 5485 changes start */
                ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                        ueContext_p->activeLcInfo[lcId], 
                        PDCP_ENTITY_RB_TX);
                /* SPR 5485 changes end */
                if( 0 != ctx_p->discardTimer ) 
                {
                    GET_FIRST_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
                    if ( PNULL == dataReq_p)
                    {
                        /* +- SPR 14622 */
                        /* SPR 3406 changes start */
                        break;;
                        /* SPR 3406 changes end */
                    }                        
                    do
                    {
                        /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING 
                            if ( pdcpGlobalTTITickCount_g[cellIndex] < 
                                    (dataReq_p->tickAssociated + ctx_p->discardTimer) )
#else
                                if ( pdcpCurrentTime_g < 
                                        (dataReq_p->tickAssociated + ctx_p->discardTimer) )
#endif                        
                                    /*  - Layer2 NON CA Changes*/
                                {
                                    break;
                                }
                        /* SPR 14622 fix start */
                        /* statistic data as well as totalDlDrbDiscardRatePerQci Stats will be updated 
                         * only if rlcDataDiscardReq() returns success */
                        /* SPR 14622 fix end */

#ifdef PDCP_ASYNC_INTERFACE
                        if ( LTE_TRUE == dataReq_p->commDataReq.isPacketWithSecEngine)
                        {
#if  !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING) /* +- coverity 83259 */
                            retVal = PDCP_FAIL;
#endif
                        }
                        else
                        {
#endif
                            /* Notify RLC about PDCP SDU to be discarded */
#ifdef LINUX_PC_TEST
                                retVal = compose_and_send_pdcp_rlc_data_discard_req

#elif    !defined (RLC_PDCP_IPC_INTF) || defined (UE_SIM_TESTING) /* +- coverity 83259 */
                                (void)sendDataDiscardReqToRlc
#else
                                /* SPR 5178 changes starts here */
                                retVal = sendDataDiscardReqToRlc

#endif
                                /* Coverity 97563 -*/
                                ( dataReq_p->commDataReq.ueIndex,
                                  dataReq_p->commDataReq.lcId,
                                  /* SPR 3406 changes start */
                                  /* SPR 2812 changes start */
                                  (void *)dataReq_p);
                            /* SPR 2812 changes end */
                            /* SPR 5178 changes ends here */
#ifdef PDCP_ASYNC_INTERFACE
                        }
#endif
                        /* SPR 21773  Start:CID 108855 */
#if ( !defined (RLC_PDCP_IPC_INTF)) || defined (UE_SIM_TESTING) /* +- coverity 83259 */
                        tempDataReq_p = dataReq_p;
                        /* SPR21821 Code Delete */
#endif
                        GET_NEXT_PDCP_TX_STORAGE_Q( ctx_p, dataReq_p  );
                        /* SPR 21773  End:CID 108855 */
                        if ( PDCP_SUCCESS == retVal )
                        {
                            /* SPR 14622 fix start */
                            /* Update statistic data */
                            pdcpStats.statsUEs[tempDataReq_p->commDataReq.ueIndex].
                                drbs[tempDataReq_p->commDataReq.lcId - 3].numberOfDlDrbSduDiscarded 
                                += 1;
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                            UInt8 qci = ctx_p->qci;
                            /* Update KPI counters in case of discard timer timeout
                            */
                            if (0 != qci)
                            {
                                pLtePdcpKPIStats->
                                    totalDlDrbDiscardRatePerQci[qci - 1]++;
                            }
#endif
                            LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, 
                                    PDCP_TX, pdcpCurrentTime_g, __LINE__, 
                                    tempDataReq_p->commDataReq.ueIndex, tempDataReq_p->commDataReq.lcId, 
                                    tempDataReq_p->commDataReq.count,
                                    tempDataReq_p->tickAssociated, 
                                    /* SPR 16766 fix start */
                                    0, 0, __func__, "TimerExpiry");
                            /* SPR 16766 fix end */
                            /* SPR 14622 fix end */
                            /* Discard the SDU, RLC has also deleted from SDU
                             * queue */
                            DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, tempDataReq_p);
                            pdcpFreeTxDataReq(tempDataReq_p);
                            tempDataReq_p = PNULL;
                        }
                        else
                        {
                            /* SPR 14622 fix start */
#ifdef PDCP_ASYNC_INTERFACE
                            if ( LTE_FALSE == dataReq_p->commDataReq.isPacketWithSecEngine)
#endif
                                /* SPR 14622 fix end */
                            {
                                /* SPR 21773  Start:CID 108855 */
#if (!defined (RLC_PDCP_IPC_INTF)) || defined (UE_SIM_TESTING)
                                /* Do not discard the SDU from queue as RLC is not
                                 * able to discard it */
                                LOG_PDCP_MSG( PDCP_DISCARD_TIMER_SDU_NOT_DELETED , 
                                        LOGWARNING, PDCP_TX, pdcpCurrentTime_g, 
                                        __LINE__, tempDataReq_p->commDataReq.ueIndex, 
                                        tempDataReq_p->commDataReq.lcId, 
                                        tempDataReq_p->commDataReq.count, 
                                        tempDataReq_p->tickAssociated, 
                                        0, 0, __func__, "");
#endif
                            }
                        }
                        /* SPR 21773 End:CID 108855 */
                        /* SPR 3406 changes end */
                    } while ( PNULL != dataReq_p );
                        /* SPR 3406 changes start */
                }
                lcId++;
            }   
            lcId = 0;
            /* Check discard timer for fresh SDU queue */
            while ( lcId < ueContext_p->numOfActiveDrbLc)
            { 
                /*
                 ** activeLcInfo consists only of active DRBs in an UE, hence valid
                 ** PDCP context will always be available. Therefore, no need to 
                 ** check for NULL check
                 */
                /* SPR 5485 changes start */
                ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                        ueContext_p->activeLcInfo[lcId], 
                        PDCP_ENTITY_RB_TX);
                /* SPR 5485 changes end */
                if( 0 != ctx_p->discardTimer )
                {
                    /* SPR 3406 changes end */
                    do
                    {
                        GET_FIRST_PDCP_TX_DATA_REQ_Q(ctx_p, dataReq_p);
                        if ( PNULL == dataReq_p)
                        {
                            /* +- SPR 14622 */
                            /* SPR 3406 changes start */
                            break;
                            /* SPR 3406 changes end */
                        }

                        /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING 
                            if ( pdcpGlobalTTITickCount_g[cellIndex] < 
                                    (dataReq_p->tickAssociated + ctx_p->discardTimer) )
#else
                                if ( pdcpCurrentTime_g < 
                                        (dataReq_p->tickAssociated + ctx_p->discardTimer) )
#endif                        
                                    /*  - Layer2 NON CA Changes*/
                                {
                                    break;
                                }
                        /* Update statistic data */
                        pdcpStats.statsUEs[dataReq_p->commDataReq.ueIndex].\
                            drbs[dataReq_p->commDataReq.lcId - 3].numberOfDlDrbSduDiscarded += 1;
                        /* Update KPI counters due to discard timer timeout*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                        UInt8 qci = ctx_p->qci;
                        if (0 != qci)
                        {
                            pLtePdcpKPIStats->totalDlDrbDiscardRatePerQci[qci - 1]++;
                        }
#endif 

                        LOG_PDCP_MSG( PDCP_DISCARD_TIMER_EXPIRE , LOGWARNING, PDCP_TX, 
                                pdcpCurrentTime_g, __LINE__, 
                                dataReq_p->commDataReq.ueIndex, 
                                dataReq_p->commDataReq.lcId, 
                                dataReq_p->commDataReq.count, 
                                dataReq_p->tickAssociated,
                                0, 0, __func__, 
                                /* SPR 16766 fix start */
                                "TimerExpiry");
                        /* SPR 16766 fix end */

                        /* Discard SDU and and do not notify RLC as this SDU is not
                         * being submitted to RLC yet as it is yet to be processed
                         */            
                        DEQUEUE_PDCP_TX_DATA_REQ_Q(ctx_p, dataReq_p);
                        /*SPR 10111 Fix Start */ 
                        if( 0 < ctx_p->numberOfNodesFwded )
                        {
                            /* Decrement the numberOfNodesFwded as the node that is
                             * forwarded is deleted */
                            ctx_p->numberOfNodesFwded--;
                        }
                        if ( ctx_p->dataNode == dataReq_p)
                        {
                            /* Since node that is being forwarded upto node in 
                             * the context, so set the datanode pointer in the 
                             * context to NULL. */
                            ctx_p->dataNode = PNULL;
                        }
                        /*SPR 10111 Fix End */
                        if ( PNULL == dataReq_p )
                        {
                            LOG_PDCP_MSG (PDCP_NULL_NODE_Q, LOGWARNING, PDCP_TX,
                                    pdcpCurrentTime_g, __LINE__, 
                                    pdcpContext.activeUeInfo[ueIndex], 
                                    ueContext_p->activeLcInfo[lcId], 0,0, 0,0,
                                    __func__,"pdcpTxDataReqQ");
                            /* SPR 3406 changes start */
                            break;
                            /* SPR 3406 changes end */
                        }                    
                        pdcpFreeTxDataReq(dataReq_p);
                    } while ( PNULL != dataReq_p ); 
                }
                lcId++;
            }
            ueIndex++;
        }
}

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : pdcpProcessDeleteUeTimers  
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Checks if we need to delete the UE after EMI recvd from
 *                  packet relay
 ****************************************************************************/
void pdcpProcessDeleteUeTimers(void)
{
    UInt16               ueIndex       = 0;
    UInt8                lcId          = 0;
    LP_PdcpEnityContext  ctx_p         = PNULL;
    LP_PdcpUeContext     ueContext_p   = PNULL;

    /*  + Layer2 NON CA Changes*/
    InternalCellIndex  cellIndex   =  0;
    /*  - Layer2 NON CA Changes*/


    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
        ** activeUeInfo consists only of active UEs, hence valid UE context
        ** will always be available. Therefore, no need to check for NULL check
        */
        /* SPR 5485 changes start */
        ueContext_p = PDCP_GET_VALID_UE_CONTEXT(
                           pdcpContext.activeUeInfo[ueIndex]
                           );
        /*  + Layer2 NON CA Changes*/
        cellIndex   =  ueContext_p->cellIndex ;
        /*  - Layer2 NON CA Changes*/

        /* SPR 5485 changes end */
        if ( 
            ( 0 != ueContext_p->tickAss ) && 
            ( PDCP_TX_STATE_HO == ueContext_p->pdcpTxSrb2DrbState ) )
        {
            /*  + Layer2 NON CA Changes*/
            /*SPR 15909 Fix Start*/
            if ( pdcpGlobalTTITickCount_g[cellIndex] >= 
                    ( ueContext_p->tickAss + (UInt64)DELETE_UE_TIMER ) )
                /*SPR 15909 Fix End*/
                /*  - Layer2 NON CA Changes*/
            {
                /* Discard the forwarded data if in between and delete the
                 * entity even if EMI not recvd */
                lcId = 0;
                while ( lcId < ueContext_p->numOfActiveDrbLc)
                {
                    /*
                    ** activeLcInfo consists only of active DRBs in an UE, 
                    ** hence valid PDCP context will always be available. 
                    ** Therefore, no need to check for NULL check
                    */
                    /* SPR 5485 changes start */
                    ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                                   ueContext_p->activeLcInfo[lcId], 
                                   PDCP_ENTITY_RB_TX);
                    /* SPR 5485 changes end */
                    /* SPR 3085 Fix Start */
                    /* SPR 16770 fix start */
                    if( (ctx_p->isEMIFwded == LTE_FALSE) && 
                            (LTE_TRUE == ctx_p->isFwdingEnabled) )
                        /* SPR 16770 fix end */
                    {
                        /* Send local EMI to Packet Relay */
                        pdcpEndOfDataForwardingInd(
                             pdcpContext.activeUeInfo[ueIndex], 
                             ueContext_p->activeLcInfo[lcId], 
                             ctx_p->isEndMarkerIndRecvd );
                        ctx_p->isEMIFwded = LTE_TRUE;
                    }
                    /* SPR 3085 Fix End */
#ifdef PDCP_ASYNC_INTERFACE
                    pdcpDeleteSecurityContext( ctx_p->pdcpSecurityContext_p );
                    /* +- SPR 15236 */
                    ctx_p->pdcpSecurityContext_p = PNULL;
#endif
                    pdcpEntityContextFree( ctx_p, 
                            pdcpContext.activeUeInfo[ueIndex], 
                            ueContext_p->activeLcInfo[lcId],
                            /* SPR 3159 changes start */
                            PDCP_ENTITY_RB_TX );
                            /* SPR 3159 changes end */

                    /*SPR 21128 Start*/
                    semDestroy((LTE_SEM *)&(ctx_p->pdcpTxDataReqQ).sem_lock);
                    semDestroy((LTE_SEM *)&(ctx_p->pdcpStorageQ).sem_lock);
                    /*SPR 21128 End*/

                    freeMemPool( ctx_p );
                    ctx_p = PNULL;
                    /* SPR 3159 changes start */
                    ueContext_p->pdcpEntities[ueContext_p->activeLcInfo[lcId] - 1][PDCP_ENTITY_RB_TX] = PNULL;
                    /* SPR 3159 changes end */
                
                    /* SPR 5485 changes start */
                    ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueContext_p,
                                   ueContext_p->activeLcInfo[lcId], 
                                   PDCP_ENTITY_RB_RX);
                    /* SPR 5485 changes end */
#ifdef PDCP_ASYNC_INTERFACE
                    pdcpDeleteSecurityContext( ctx_p->pdcpSecurityContext_p );
                    /* +- SPR 15236 */
                    ctx_p->pdcpSecurityContext_p = PNULL;
#endif
                    pdcpEntityContextFree( ctx_p, 
                            pdcpContext.activeUeInfo[ueIndex], 
                            ueContext_p->activeLcInfo[lcId],
                            /* SPR 3159 changes start */
                            PDCP_ENTITY_RB_RX );
                            /* SPR 3159 changes end */

                    /*SPR 21128 Start*/
                    DEINIT_PDCP_RX_DATA_IND_Q(ctx_p);
                    semDestroy((LTE_SEM *)&(ctx_p->pdcpUlReorderingQ).sem_lock);
                    /*SPR 21128 End*/

                    freeMemPool( ctx_p );
                    ctx_p = PNULL;
                    /* SPR 3159 changes start */
                    ueContext_p->pdcpEntities[ueContext_p->activeLcInfo[lcId] - 1][PDCP_ENTITY_RB_RX] = PNULL;
                    /* SPR 3159 changes end */

                    /* Updates the list of active DRBs*/
                    updateActiveLcInfo( 
                            pdcpContext.activeUeInfo[ueIndex], 
                            ueContext_p->activeLcInfo[lcId], 
                            ueContext_p->activeLcInfo, 
                            ueContext_p->activeLcIndex );
                    pdcpStats.statsUEs[ueIndex].statNumActiveDrbLc -= 1;
                    ueContext_p->numOfActiveDrbLc -= 1;

                    lcId = 0;
                }
                /*Deallocate memory for UE Context*/
                freeMemPool( pdcpContext.ueContexts[pdcpContext.activeUeInfo[ueIndex]] );
                pdcpContext.ueContexts[pdcpContext.activeUeInfo[ueIndex]] = PNULL;

                /* Updates the list of active UEs*/
                updateActiveUeInfo( 
                            pdcpContext.activeUeInfo[ueIndex], 
                            pdcpContext.activeUeInfo, 
                            pdcpContext.activeUeIndex 
                            );
    
                /*reset UE Context*/
                pdcpContext.numOfActiveUe -= 1;
                pdcpStats.statNumActiveUe -= 1;
            }
        }
        ueIndex++;
    }
}
#endif

/* SPR 3608 changes start*/
/****************************************************************************
* Function Name  : pdcpProcessPSRTimers
* Inputs         : None
* Outputs        : None
* Returns        : None
* Description    : Processing Control queue with timers
****************************************************************************/
void pdcpProcessPSRTimers(void)
{
    UInt8                lcId    = 0;
    UInt16               ueIndex = 0;
    LP_PdcpUeContext     ueCtx_p = PNULL;
    LP_PdcpEnityContext  ctx_p   = PNULL;
#ifdef LOG_PRINT_ENABLED
    UInt32               state   = 0;
#endif
#if !defined(UE_SIM_TESTING)
    /*  + Layer2 NON CA Changes*/
    InternalCellIndex  cellIndex   =  0;
    /*  - Layer2 NON CA Changes*/
#endif                

    while ( ueIndex < pdcpContext.numOfActiveUe)
    {
        /*
         * activeUeInfo consists only of active UEs, hence valid UE context
         * will always be available. Therefore, no need to check for NULL check
         */
        /* SPR 5485 changes start */
        ueCtx_p = PDCP_GET_VALID_UE_CONTEXT(
                      pdcpContext.activeUeInfo[ueIndex]);
        /* SPR 5485 changes end */


        if(ueCtx_p->pdcpTxSrb2DrbState == PDCP_TX_STATE_AWAITED)
        {
        /*  + Layer2 NON CA Changes*/
#ifndef UE_SIM_TESTING        
            cellIndex   =  ueCtx_p->cellIndex ;
            if(pdcpGlobalTTITickCount_g[cellIndex] >= (ueCtx_p->pdcpPSRTimer + PDCP_STATUS_REPORT_TIMER_VALUE))
#else
            if(pdcpCurrentTime_g >= (ueCtx_p->pdcpPSRTimer + PDCP_STATUS_REPORT_TIMER_VALUE))
#endif                
        /*  - Layer2 NON CA Changes*/
            {        
                for( lcId = 0; lcId < ueCtx_p->numOfActiveDrbLc; lcId++)
                {
                    /*
                    ** activeLcInfo consists only of active DRBs in an UE, 
                    ** hence valid PDCP context will always be available. 
                    ** Therefore, no need to check for NULL check
                    */
                    /* SPR 5485 changes start */
                    ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                                ueCtx_p->activeLcInfo[lcId], 
                                PDCP_ENTITY_RB_RX );
                    /* SPR 5485 changes end */
                    /* It is assumed that the PdcpEntityContext will be present for
                     * both TX and RX */
#ifndef UE_SIM_TESTING 
                    if( LTE_TRUE == ctx_p->ueStatusReportReq &&
                        LTE_FALSE == ctx_p->isPsrRecvd ) 
#else
                    if( LTE_TRUE == ctx_p->statusReportRequired && 
                        LTE_FALSE == ctx_p->isPsrRecvd ) 
#endif
                    {
                        (ueCtx_p->numOfPsrRecvd)++;
                    }

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
                                      pdcpContext.activeUeInfo[ueIndex],
                                      LTE_DRB_LC_ID_LOW_BOUND,
                                      ueCtx_p->pdcpTxSrb2DrbState,
                                      ueCtx_p->event, ueCtx_p );
                    LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, 
                          pdcpContext.activeUeInfo[ueIndex], 
                          state, ueCtx_p->event,
                          ueCtx_p->pdcpTxSrb2DrbState, LTE_DRB_LC_ID_LOW_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");
                }
            }
        }
        ueIndex++;
    }
}
/* SPR 3608 changes end*/

/****************************************************************************
 * Function Name  : sendDataDiscardReqToRlc
 * Inputs         : ueIndex - UE Identifier of a UE
 *                  lcId - Logical Channel ID
 *                  transactionId - Transaction ID
 * Outputs        : None
 * Returns        : RLC_SUCCESS or FAILURE
 * Description    : This API is used by PDCP to discard any PDU already sent
 *                  to RLC Layer. RLC Layer will delete the referenced PDU only
 *                  if any portion of it is not transmitted to MAC yet.
 ******************************************************************************/
UInt16 sendDataDiscardReqToRlc( UInt16 ueIndex, UInt8 lcId, 
        void *transactionId )
{
    UInt16 result = PDCP_SUCCESS;
/* SPR 5178 changes starts here */
    result = rlcDataDiscardReq( ueIndex, (UInt16)lcId, transactionId );
/* SPR 5178 changes ends here */
    return result;
}

