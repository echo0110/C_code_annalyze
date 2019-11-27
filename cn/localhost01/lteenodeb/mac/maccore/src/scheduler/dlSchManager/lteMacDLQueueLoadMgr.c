/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacDLQueueLoadMgr.c,v 1.1.1.1.6.1.4.2.2.1 2010/09/21 15:46:14 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functionality of updating the queue
                       load of UE.And also if Queue Load of the UE becomes non 
                       Zero from Zero and vice versa,Then put the trigger to 
                       enter/to remove from / into the 
                       UE Non Zero QueueLoadAndHarqList

 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacDLQueueLoadMgr.c,v $
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:46:14  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.12  2009/11/15 13:34:02  gur11912
 * LTE_MAC_UT_LOG(LOG_WARNING) used instead of lteWarning
 *
 * Revision 1.11  2009/07/30 05:14:20  gur11912
 * reset assignedRBQueueLoad array in case of queueload mismatch
 *
 * Revision 1.10  2009/07/23 15:35:09  gur18569
 * setting lc queue load to zero in case of qload mismatch
 *
 * Revision 1.9  2009/07/13 15:43:49  gur20052
 * updated for prefix string DL_Q_LOAD_UPDATE
 *
 * Revision 1.8  2009/07/13 15:42:44  gur20052
 * updated for prefix string DL_Q_LOAD_UPDATE
 *
 * Revision 1.7  2009/06/17 12:23:30  gur20052
 * Added more debug logs
 *
 * Revision 1.6  2009/06/08 08:23:00  gur20052
 * Updated for Review comments incorporations
 *
 * Revision 1.5  2009/06/02 07:10:22  gur20052
 * Update to incorporated UE context changes
 *
 * Revision 1.4  2009/06/01 06:45:15  gur18569
 * removed compilation errors
 *
 * Revision 1.3  2009/05/11 09:26:08  gur20052
 * Reveiw Comments are incorporated
 *
 * Revision 1.2  2009/05/09 10:28:20  gur20052
 * Added New Logs
 *
 * Revision 1.1  2009/05/04 10:43:25  gur20052
 * Initial version
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/



/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include "lteMacDLUEContext.h"
#include "lteMacDLQueueLoadMgr.h"
#include "ltePerfStats.h"

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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : updateUEQueueLoad
 * Input          : ueContext_p - Pointer to DLUEContext,
 *                  be updated.
 *                  qLoad - Queue Load that needs to be updated
 * Outputs        : Updated queue load of UE, And Add UE Triggers for Addition 
 *                  and deletion, if Queue Load of the UE becomes non Zero 
 *                  from Zero or vice versa.
 * Returns        : None
 * Description    : This function updates the queue load of UE with the value 
 *                  that has been sent in current TB for particular LCID.
 *****************************************************************************/
/* SPR 11273 Start */
 void updateUEQueueLoad(DLUEContext *ueContext_p , UInt32 qLoad ,
         BOType boFlag)
/* SPR 11273 End */
{
    UInt32 prevQload = 0;
    InternalCellIndex internalCellIndex = \
        ueContext_p->internalCellIndex;                            
#ifdef PERF_STATS
    LteUePdcpDLPerfStats *lteUePdcpDLPerfStats_p = (LteUePdcpDLPerfStats *)
        &gPdcpUePerfStats_p[ueContext_p->ueIndex].lteUePdcpDLPerfStats;
    /* +- SPR 18490 */
    UInt8 lcId = INVALID_LC_ID;
#endif
    do{
        prevQload = ueContext_p->ueQueueLoad;
    }while (!BOOL_COMPARE_AND_SWAP(&ueContext_p->ueQueueLoad, prevQload, qLoad));
/* SPR 11273 Start */
    LOG_MAC_DEBUG( MAC_DL_Strategy, "Update Q load for UE[%u] Qload[%d] "
            "boflag[%u]", ueContext_p->ueIndex, qLoad, boFlag );
    if ( RLC_BO_TYPE_NEW  == boFlag )
    {
        /*Adding the Queue Load in UE Context */
        if (prevQload == 0 && qLoad != 0)
        {
             /* SPR 3608 Changes start */
            if( ueContext_p->prioritySchedulingReqDL == TRUE )
            {
                putEntryInDLPriorityQueue(ueContext_p->ueIndex, internalCellIndex);
                 /* SPR 6897 changes start */
                ueContext_p->isPresentInPriorityQueue = LTE_TRUE;
            }
            putEntryInZeroNonZeroQueueLoadTriggerQueue( ueContext_p,
                    ADD_TRIGGER,internalCellIndex);
             /* SPR 6897 changes end */
             /* SPR 3608 Changes end */
        }
         /* SPR 4473 Changes start */
        else if (prevQload != 0 )
        {
             /* SPR 6897 changes start */
            if(ueContext_p->prioritySchedulingReqDL == TRUE &&
                    !(ueContext_p->isPresentInPriorityQueue) )
            {
                putEntryInDLPriorityQueue(ueContext_p->ueIndex, internalCellIndex);
                ueContext_p->isPresentInPriorityQueue = LTE_TRUE;
            }
             /* SPR 6897 changes start */
        }
         /* SPR 4473 Changes End */



        /*Update Last Activity Tick when received BO Type New received
          from RLC*/
        ueContext_p->lastActivityTick = getCurrentTick();
    }
    else
    {
        /*Making the Assigned Queue Load to Zero*/
        ueContext_p->assignedRBQueueLoad[
            ueContext_p->assignedRBQueueLoadRLCIndex] = 0;

        /*Moving to the next RLC index so that next time the RLC Queue
         * Load will be updated for the next RLC Index*/
        ueContext_p->assignedRBQueueLoadRLCIndex =
            ((ueContext_p->assignedRBQueueLoadRLCIndex + 1) %
             MAX_TEMP_QUEUELOAD_ARR_SIZE);

        if (qLoad == 0)
        {
            LTE_MAC_UT_LOG(LOG_INFO,DL_Q_LOAD_UPDATE,
                    "[%s]Insert Trigger to delete UE::%d from Non Zero Queue \
                    Load List With Queue Load %d\n",__func__,ueContext_p->ueIndex,ueContext_p->ueQueueLoad );
            /* SPR 3608 Changes start */
            putEntryInZeroNonZeroQueueLoadTriggerQueue( ueContext_p,
                    DELETE_TRIGGER, internalCellIndex);
#ifdef PERF_STATS
            for( lcId = 3; lcId <=  MAX_LCID; lcId++)
            {
                 pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].currentIndex =\
                    ! (pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].currentIndex);
                 pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].previousIndex =\
                    ! (pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].previousIndex);
                /* Store the value in Shared Memory from Previous Index */
                 lteUePdcpDLPerfStats_p->pdcpLastTtiTransVolDl[lcId -3] =\
                    pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].pdcpDataSize
                    [ pdcpLastTtiTransVolDl_g[ueContext_p->ueIndex][lcId -3 ].previousIndex];
            }

#endif
        /* +- SPR 17404 */
        }
         /*SPR 12339 changes start */
        else if(qLoad != 0)
        {
            LOG_MAC_MSG (DL_PRIORITY_QUEUE_SCHEDULED_LOG_ID, LOGDEBUG,
                    MAC_DL_Strategy, getCurrentTick (), ueContext_p->ueIndex,
                    prevQload,
                    qLoad,
                    ueContext_p->prioritySchedulingReqDL,
                    ueContext_p->isPresentInPriorityQueue,
                    DEFAULT_FLOAT_VALUE, DEFAULT_FLOAT_VALUE, __func__, "Bug900_2104");
            if(ueContext_p->prioritySchedulingReqDL == TRUE &&
                    !(ueContext_p->isPresentInPriorityQueue) )
            {
                putEntryInDLPriorityQueue(ueContext_p->ueIndex, internalCellIndex);
                ueContext_p->isPresentInPriorityQueue = LTE_TRUE;
            }
        }
         /*SPR 12339 changes end */
    }
/* SPR 11273 End */

    return;
}
