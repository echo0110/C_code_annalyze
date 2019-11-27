
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacULSpsScheduler.h,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the definitions for SPS scheduler.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _LIB_MAC_UL_SPS_SCH_DEF_
#define _LIB_MAC_UL_SPS_SCH_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteCircQueue.h"
#include "lteMacTypes.h"
#include "lteMacSpsCommonTypes.h"
#include "lteMacULResourceAllocator.h"
#include "lteMacPDCCHMgr.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Wrapper macros definitions for SPS queues for initialization,
 * de-initialization, enqueing, dequeing process and also for determining 
 * number of nodes in queue. Following queues are handled using these macros:
 * 1. Staus queue, ulSpsStatusQ_g 
 * 2. Deactivation queue, ulSpsDeactQ_g
 * 3. Activation queue, ulSpsActQ_g
 * 4. Reeactivation queue, ulSpsReactQ_g
 * 6. Resource reserve request queue, ulSpsResRsvQ_g
 * 7. Resource release request queue, ulSpsResRelQ_g
*/
#define QINIT_MAC_UL_SPS_Q(ulSpsStrQueue_p)\
    circQInit( ulSpsStrQueue_p, MAX_NODES )

#define QCOUNT_MAC_UL_SPS_Q(ulSpsStrQueue_p)\
    circQueueCount( ulSpsStrQueue_p )

#define ENQUEUE_MAC_UL_SPS_Q( ulSpsStrQueue_p, node_p )\
    pushNodeCircQ( ulSpsStrQueue_p, node_p )

#define DEQUEUE_MAC_UL_SPS_Q( ulSpsStrQueue_p, node_p ){\
    popNodeCircQ( ulSpsStrQueue_p, (void *)&node_p );\
}

#define QDEINIT_MAC_UL_SPS_Q( ulSpsStrQueue_p )\
   circQDeInit( ulSpsStrQueue_p )

/* Wrapper macros definitions for UE LIST maintained in TTI Map */
#define QINIT_MAC_SPS_UE_LIST_PER_TTI( ttiMap_p )\
    queueInit( &ttiMap_p )

#define QCOUNT_MAC_SPS_UE_LIST_PER_TTI( ttiMap_p )\
    queueCount( &ttiMap_p )

#define ENQUEUE_MAC_SPS_UE_LIST_PER_TTI( ttiMap_p, ueNode_p ){\
    enQueue( &ttiMap_p, &(ueNode_p->node) ); \
}

#define DELETE_MAC_SPS_UE_LIST_PER_TTI(ttiMap_p, ueNode_p){\
    delQueueNode( &ttiMap_p, &(ueNode_p->node) );\
}

#define GET_FIRST_MAC_SPS_UE_LIST_PER_TTI( ttiMap_p, ueNode_p ){\
    ueNode_p = (void *)getFirstQueueNode( &ttiMap_p );\
}

#define GET_NEXT_MAC_SPS_UE_LIST_PER_TTI( ttiMap_p, ueNode_p ){\
    ueNode_p = (void *)getNextQueueNode( &ttiMap_p, \
                                &(ueNode_p->node ));\
}

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* Following structure defines the SPS Interval TTI Map which stores SPS DCI 
 * list information */
typedef struct SpsULStrIntervalTTIContextT
{
    /* Number of SPS activation, reactivation or deactivation which 
     * can be scheduled in a TTI for uplink.*/
    UInt8 numULSpsDciPerTti;
    /* Index number of SPS TTIContext */
    UInt8 index;
    /* Number of total RBs to be reserved for SPS per TTI for uplink */
    UInt8 numULSpsRbsPerTti;
    /* Number of SPS occasions that are configured in a TTI for uplink. */
    UInt8 numULSpsOccasionPerTti;
    /* Index in Sorted Array for each periodicity */
    UInt8 indexInSortedArray;
    /* + SPS_TDD_Changes  */
#ifdef TDD_CONFIG
    UInt8 sfType;
#endif
    /* - SPS_TDD_Changes  */
#ifdef FDD_CONFIG
    /*To keep track of number of SPS + TTIb Ues in current TTI ,
    It is required to subtract these Ues while processing occasion node
    as occasion node is added once for 4 ticks but numberofUe at a TTI
    need to be updated,incremented for only n+1,n+2,n+3 where n is where occasion node
    is to be added*/
    UInt8 numULSpsTTIBUePerTti;
#endif
    /* Number of RBs per TTI per interval: Index 0 corresponds to 10ms
     * periodicity, 1 corresponds to 20ms and so on */
    UInt16 numULSpsRbsPerTtiPerInterval[SPS_INTERVAL_MAX];
    /* SPS DCI List (SDL) containing info for UE */
    RbMapNode rbMapNode;
    LTE_QUEUE spsULDciList;
    /* List containing SPS occasions for UE */
    LTE_QUEUE spsULOccasionList;
} *LP_SpsULStrIntervalTTIContext, SpsULStrIntervalTTIContext;


/* Following structure contains the SPS DCI List for UE */
typedef struct MacULSpsDciListTTIT
{
    LTE_QUEUE_NODE node;
    UInt16 ueIndex;
} *LP_MacULSpsDciListTTI, MacULSpsDciListTTI;

/* Following structure contains the SPS occasion information for an UE in a 
 * TTI */
typedef struct MacULSpsOccListT
{
    LTE_QUEUE_NODE node;
    UInt16  ueIndex;
#ifdef FDD_CONFIG
    /* check whether occasion added was TTI Bundled or non TTIB ,required 
        to decide about TTIB scheduling of SPS occasion,and deletion of occaion etc*/
    UInt8  isTTIBundle;
#endif
                
} *LP_MacULSpsOccList, MacULSpsOccList;

/* Following structure contains the information for activation */
typedef struct MacULSpsActReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Activation */
    /*coverity-530 CID-32343*/
    SpsCause  cause;
    /*coverity-530 CID-32343*/
} *LP_MacULSpsActReq, MacULSpsActReq;


/* Following structure contains the information for reactivation */
typedef struct MacULSpsReactReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Reactivation */
    /*coverity-530 CID-32343*/
    SpsCause  cause;
    /*coverity-530 CID-32343*/
    /* TTI for SPS Reactivation */
    UInt16 ttiIndex;
} *LP_MacULSpsReactReq, MacULSpsReactReq;

/* Following structure contains the information for deactivation */
typedef struct MacULSpsDeactReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Activation */
    /*coverity-530 CID-32343*/
    SpsCause  cause;
    /*coverity-530 CID-32343*/
} *LP_MacULSpsDeactReq, MacULSpsDeactReq;

/* Following structure contains the status received */
typedef struct MacULSpsStatusT
{
    UInt16  ueIndex;
    /* Strategy receives following type of responses from PUSCH or HARQ module:
       1. Pusch res reserv Success
       2. Pusch res reserv Failure (Mcs Poor)
       3. Harq First Ack / NACK for Activation
       4. HARQ DTX or timer expiry Information 
     */

    /* Coverity 32345 Fix Start */
    SpsCause response;
    /* Coverity 32345 Fix End*/

    /* Type of request for Reserve or release resources */
    UInt8    requestType ;
    /* Index in TTI interval map */
    UInt16   index;
    /* Latest Opportunity provided to SPS LC */
    UInt32  spsOpp;
    /* RBs occupied*/
    UInt16   numRbs;
} *LP_MacULSpsStatus, MacULSpsStatus;

/* Following structure contains the information to exclude UE for SPS */
typedef struct MacULSpsExcludeT
{
    UInt16  ueIndex;
    /* TTI from Occasion List for UE to be excluded by PUSCH */
    /*SPr 15909 fix start*/
    tickType_t ttiIndex;
    /*SPr 15909 fix end*/
} *LP_MacULSpsExclude, MacULSpsExclude;

/* Following structure is for providing Resources to be Reserved or Released 
  * by PUSCH */
typedef struct MacULSpsResReqT
{    
    UInt16  ueIndex ;
    /* Type of request for Reserve or release resources */
    UInt8   requestType ;
    /* Average RLC SDU size for PDSCH */
    UInt32  avgRlcSduSize;
} *LP_MacULSpsResReq, MacULSpsResReq;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/*CA Changes start  */
extern LTE_CIRCQUEUE* ulSpsResRsvQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsResRelQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsStatusQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsActQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsDeactQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE* ulSpsReactQ_gp[MAX_NUM_CELL];


extern SpsULStrIntervalTTIContext* spsULStrTTIMap_gp[MAX_NUM_CELL];
extern UInt8 numUlSpsRbsInTti_g[MAX_NUM_CELL];
/*CA Changes end  */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
typedef MacRetType (*macULStrategyHandlerFuncT) (UInt16 ueIndex,
        SpsStates state, SpsCause cause, ULUEContext *ulUeCtx_p, 
        /*CA Changes start  */
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex cellIndex);
        /*CA Changes end  */

/* State machine to handle startegy actions for various states */
extern macULStrategyHandlerFuncT ulStateMachine [MAX_SPS_STATE]
                                                [MAX_SPS_CAUSE + 1];

/****************************************************************************
 * Function Name  : macULSpsProcessActQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the activation queue nodes.
 ****************************************************************************/
/*CA Changes start  */
UInt16 macUlSpsProcessActQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellId );
/*CA Changes end  */

/****************************************************************************
 * Function Name  : macUlSpsProcessStatusQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the status queue nodes.
 ****************************************************************************/
/*CA Changes start  */
UInt16 macUlSpsProcessStatusQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellId );
/*CA Changes end  */

/****************************************************************************
 * Function Name  : macULSpsProcessDeactQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the deactivation queue nodes.
 ****************************************************************************/
/*CA Changes start  */
UInt16 macUlSpsProcessDeactQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellId );
/*CA Changes end  */

/****************************************************************************
 * Function Name  : processULSpsOccasion
 * Inputs         : currentGlobalTTITickCount - current Global Tick
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function checks for SPS occasion and calls to process
 *                  SDL and occasion list.
 ****************************************************************************/
/* SPR 15909 fix start */
UInt32 processULSpsOccasion( tickType_t currentGlobalTTITickCount, 
        /* SPR 15909 fix end */
        UInt32 delayToPhy,
        /*CA Changes start  */
        InternalCellIndex internalCellId );
        /*CA Changes end  */

/****************************************************************************
 * Function Name  : ulSpsPendingDeleteHandling
 * Inputs         : ulUeCtx_p - UL UE Context
 * Outputs        : None.
 * Returns        : None
 * Description    : Handles pending delete UE request and clears the TTI context
 *                  stored by SPS scheduler. This API clears resources reserved
 *                  by SPS PUSCH, SPS occasion list, SDL. Release SPS-RNTI and
 *                  also releases UL Setup Info pointer.
 ****************************************************************************/
/*CA Changes start  */
void ulSpsPendingDeleteHandling( ULUEContext *ulUeCtx_p, InternalCellIndex internalCellId );
/*CA Changes end  */

/****************************************************************************
 * Function Name  : fillUlSpsDciInfo
 * Inputs         : ueULContext_p - UL UE Context
 *                  ulHarqInfo_p - Pointer to HARQ node
 *                  msgType - MsgType used
 * Outputs        : None.
 * Returns        : Pointer to DCI node for PDCCH.
 * Description    : Allocates and fills information for DCI.
 ****************************************************************************/
ULStrategyTxNode * fillUlSpsDciInfo( 
        ULUEContext   *ueULContext_p, 
        /* +- SPR 17777 */
        ULGrantRequestType       msgType,
        /*CA Changes start  */
        InternalCellIndex internalCellId);
        /*CA Changes end  */

/****************************************************************************
 * Function Name  : checkIsSpsDataReceivedOnOccasion
 * Inputs         : ueULContext_p - UL UE Context
 *                  harqIndex- harq index.
 * Outputs        : None.
 * Returns        : void
 * Description    : check whether data is received on sps occasion.
 ****************************************************************************/
void checkIsSpsDataReceivedOnOccasion( ULUEContext *ulCntxt_p,
                                       UInt8  harqIndex,
                                       /* + SPS_TDD_Changes */
                                       UInt16 recvdRNTI,
                                       /* - SPS_TDD_Changes */
                                       /*CA Changes start  */
                                       InternalCellIndex internalCellId);
                                       /*CA Changes end  */

/****************************************************************************
 * Function Name  : handleMaxSpsHarqRetxOnOccasion 
 * Inputs         : ueULContext_p - UL UE Context
 *                  harqIndex- harq index.
 * Outputs        : None.
 * Returns        : void
 * Description    : handles max retx case of sps ocassion.
 ****************************************************************************/
void handleMaxSpsHarqRetxOnOccasion( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex,
                             /*CA Changes start  */
                             InternalCellIndex internalCellId);
                             /*CA Changes end  */
/****************************************************************************
 * Function Name  : checkSpsActTti 
 * Inputs         : ueULContext_p - UL UE Context
 *                  harqIndex- harq index.
 * Outputs        : None.
 * Returns        : TRUE/FALSE
 * Description    : checks sps Activation tti.
 ****************************************************************************/
UInt8 checkSpsActTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);

/****************************************************************************
 * Function Name  : checkSpsActTti 
 * Inputs         : ueULContext_p - UL UE Context
 *                  harqIndex- harq index.
 * Outputs        : None.
 * Returns        : TRUE/FALSE
 * Description    : checks sps occassion tti.
 ****************************************************************************/
UInt8 checkSpsOccasionTti( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);

/** SPR 12364 fix Starts**/
/******************************************************************************
 * Function Name  : checkSpsOccasionTtiFrmDemux
 * Inputs         : ulCntxt_p - Pointer to ULUEContext,
 *                  harqIndex - Index of HARQ
 * Outputs        : None
 * Returns        : TRUE/FALSE
 * Description    : This function checks SPS occasion TTI from local backup 
 *                  created when CRC Ack is received.
 *****************************************************************************/
UInt8 checkSpsOccasionTtiFrmDemux( ULUEContext *ulCntxt_p,
                             UInt8  harqIndex);
/** SPR 12364 fix Ends **/
#endif

