/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacDLSpsScheduler.h,v  $
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

#ifndef _LIB_MAC_DL_SPS_SCH_DEF_
#define _LIB_MAC_DL_SPS_SCH_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteCircQueue.h"
#include "lteMacTypes.h"
#include "lteMacSpsCommonTypes.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacPDCCHMgr.h"
/* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
#include "lteMacTDDConfig.h"
#endif
/* - SPS_TDD_Changes */

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Wrapper macros definitions for SPS queues for initialization,
 * de-initialization, enqueing, dequeing process and also for determining 
 * number of nodes in queue. Following queues are handled using these macros:
 * 1. Staus queue, dlSpsStatusQ_g 
 * 2. Deactivation queue, dlSpsDeactQ_g
 * 3. Activation queue, dlSpsActQ_g
 * 4. Reeactivation queue, dlSpsReactQ_g
 * 5. UE exclude list, dlSpsUeExclude_g
 * 6. Resource reserve request queue, dlSpsResRsvQ_g
 * 7. Resource reserve request queue, dlSpsResRelQ_g
*/
#define QINIT_MAC_DL_SPS_Q(dlSpsStrQueue_p)\
    circQInit( dlSpsStrQueue_p, MAX_NODES )

#define QCOUNT_MAC_DL_SPS_Q(dlSpsStrQueue_p)\
    circQueueCount( dlSpsStrQueue_p )

#define ENQUEUE_MAC_DL_SPS_Q( dlSpsStrQueue_p, node_p )\
    pushNodeCircQ( dlSpsStrQueue_p, node_p )

#define DEQUEUE_MAC_DL_SPS_Q( dlSpsStrQueue_p, node_p )\
    popNodeCircQ( dlSpsStrQueue_p, (void *)&node_p )

#define QDEINIT_MAC_DL_SPS_Q( dlSpsStrQueue_p )\
   circQDeInit( dlSpsStrQueue_p )

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

/* Maximum number of packets for which SPS is not activated if due to SPS-PDSCH
 * failure */
#define MAX_PKTS_RECEIVED_IN_DEACT_DL 10

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* Following structure defines the SPS Interval TTI Map which stores SPS DCI 
 * list information */
typedef struct SpsStrIntervalTTIContextT
{
    /* Number of SPS activation, reactivation or deactivation which 
     * can be scheduled in a TTI for downlink.*/
    UInt8 numDlSpsDciPerTti;
    /* Index number of SPS TTIContext */
    UInt8 index;
    /* Number of total RBs to be reserved for SPS per TTI for downlink */
    UInt8 numDlSpsRbsPerTti;
    /* Number of SPS occasions that are configured in a TTI for downlink. */
    UInt8 numDlSpsOccasionPerTti;
    /* Index in Sorted Array for each periodicity */
    UInt8 indexInSortedArray;
    /* Number of RBs per TTI per interval: Index 0 corresponds to 10ms
     * periodicity, 1 corresponds to 20ms and so on */
    UInt16 numSpsRbsPerTtiPerInterval[SPS_INTERVAL_MAX];
    /* The structure contains the resource map to be saved for each SPS
     * interval */
    RAT2LocalizedInfo spsRAT2LocalizedInfo;
    /* Bitmap used for RAT2 Localized in which 1 bit is for 1 RB for DL FSS */
    UInt8             spsTTIBitMap[13];
    /* SPS DCI List (SDL) containing info for UE */
    LTE_QUEUE spsDciList;
    /* List containing SPS occasions for UE */
    LTE_QUEUE spsOccasionList;
    /* + SPS_TDD_Changes */
#ifdef TDD_CONFIG
    ULDLSPFrame sfType;
#endif
    /* - SPS_TDD_Changes */

} *LP_SpsStrIntervalTTIContext, SpsStrIntervalTTIContext;

/* Following structure contains the SPS DCI List for UE */
typedef struct MacSpsDciListTTIT
{
    LTE_QUEUE_NODE node;
    UInt16 ueIndex;
} *LP_MacSpsDciListTTI, MacSpsDciListTTI;

/* Following structure contains the SPS occasion information for an UE in a 
 * TTI */
typedef struct MacSpsOccListT
{
    LTE_QUEUE_NODE node;
    UInt16  ueIndex;
} *LP_MacSpsOccList, MacSpsOccList;

/* Following structure contains the information for activation */
typedef struct MacDlSpsActReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Activation */
   /* coverity-530 CID-32324*/ 
    SpsCause  cause;
   /* coverity-530 CID-32324*/ 
} *LP_MacDlSpsActReq, MacDlSpsActReq;

/* Following structure contains the information for reactivation */
typedef struct MacDlSpsReactReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Reactivation */
   /* coverity-530 CID-32324*/ 
    SpsCause  cause;
   /* coverity-530 CID-32324*/ 
    /* TTI for SPS Reactivation */
    UInt16 ttiIndex;
} *LP_MacDlSpsReactReq, MacDlSpsReactReq;

/* Following structure contains the information for deactivation */
typedef struct MacDlSpsDeactReqT
{
    UInt16 ueIndex;
    /* Reason for SPS Activation */
   /* coverity-530 CID-32324*/ 
    SpsCause  cause;
   /* coverity-530 CID-32324*/ 
} *LP_MacDlSpsDeactReq, MacDlSpsDeactReq;

/* Following structure contains the status received */
typedef struct MacDlSpsStatusT
{
    UInt16  ueIndex;
    /* Strategy receives following type of responses from PDSCH or HARQ module:
       1. Pdsch res reserv Success
       2. Pdsch res reserv Failure (Mcs Poor)
       3. Harq First Ack / NACK for Activation
       4. HARQ DTX or timer expiry Information 
     */
     /*coverity-530 CID-32331 */
    SpsCause   response;
     /*coverity-530 CID-32331 */
    /* Type of request for Reserve or release resources */
    UInt8    requestType ;
    /* Index in TTI interval map */
    UInt16   index;
    /* Latest Opportunity provided to SPS LC */
    UInt32  spsOpp;
    /* RBs occupied*/
    UInt16   numRbs;
} *LP_MacDlSpsStatus, MacDlSpsStatus;
   
/* Following structure contains the information to exclude UE for SPS */
typedef struct MacDlSpsExcludeT
{
    UInt16  ueIndex;
    /* TTI from Occasion List for UE to be excluded by PDSCH */
    /*SPR 15909 fix start*/
    tickType_t ttiIndex;
    /*SPR 15909 fix end*/
} *LP_MacDlSpsExclude, MacDlSpsExclude;

/* Following structure is for providing Resources to be Reserved or Released 
  * by PDSCH */
typedef struct MacDlSpsResReqT
{    
    UInt16  ueIndex ;
    /* Type of request for Reserve or release resources */
    UInt8   requestType ;
    /* Average RLC SDU size for PDSCH */
    UInt32  avgRlcSduSize;
} *LP_MacDlSpsResReq, MacDlSpsResReq;

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern LTE_CIRCQUEUE *dlSpsResRsvQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsResRelQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsStatusQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsActQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsDeactQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsReactQ_gp[MAX_NUM_CELL];
extern LTE_CIRCQUEUE *dlSpsUeExclude_gp[MAX_NUM_CELL];

extern SpsStrIntervalTTIContext spsStrTTIMap_g[MAX_NUM_CELL][MAX_SPS_INTERVAL];

extern UInt8 numDlSpsRbsInTti_g[MAX_NUM_CELL];

extern UInt8 numDlSpsUeInTti_g[MAX_NUM_CELL];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
typedef MacRetType (*macStrategyHandlerFuncT) (UInt16 ueIndex,
        SpsStates state, SpsCause cause, DLUEContext *dlUeCtx_p, 
        void * spsCtx_p, UInt32 delayToPhy, InternalCellIndex internalCellIndex);

/* State machine to handle startegy actions for various states */
extern macStrategyHandlerFuncT dlStateMachine [MAX_SPS_STATE][MAX_SPS_CAUSE];

/****************************************************************************
 * Function Name  : macDlSpsProcessActQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the activation queue nodes.
 ****************************************************************************/
UInt16 macDlSpsProcessActQ( UInt32 count, UInt32 delayToPhy, InternalCellIndex internalCellIndex );

/****************************************************************************
 * Function Name  : macDlSpsProcessStatusQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the status queue nodes.
 ****************************************************************************/
UInt16 macDlSpsProcessStatusQ( UInt32 count, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : macDlSpsProcessDeactQ
 * Inputs         : count - number of nodes in queue
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function processes the deactivation queue nodes.
 ****************************************************************************/
UInt16 macDlSpsProcessDeactQ( UInt32 count, UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : processDLSpsOccasion
 * Inputs         : currentGlobalTTITickCount - current Global Tick
 *                  delayToPhy - Delay received from PHY
 * Outputs        : None.
 * Returns        : MAC_SUCCESS or MAC_FAILURE.
 * Description    : This function checks for SPS occasion and calls to process
 *                  SDL and occasion list.
 ****************************************************************************/
/* SPR 15909 fix start */
UInt32 processDLSpsOccasion(tickType_t currentGlobalTTITickCount, 
         /* SPR 15909 fix end */
        UInt32 delayToPhy,
        InternalCellIndex internalCellIndex);

/****************************************************************************
 * Function Name  : dlSpsPendingDeleteHandling
 * Inputs         : dlUeCtx_p - DL UE Context
 * Outputs        : None.
 * Returns        : None
 * Description    : Handles pending delete UE request and clears the TTI context
 *                  stored by SPS scheduler. This API clears resources reserved
 *                  by SPS PDSCH, SPS occasion list, SDL. Release SPS-RNTI and
 *                  also releases DL Setup Info pointer.
 ****************************************************************************/
void dlSpsPendingDeleteHandling( DLUEContext *dlUeCtx_p, InternalCellIndex internalCellIndex );

/****************************************************************************
 * Function Name  : fillDlSpsDciInfo
 * Inputs         : ueDLContext_p - DL UE Context
 *                  harqProcess_p - Pointer to HARQ node
 *                  msgType - MsgType used
 * Outputs        : None.
 * Returns        : Pointer to DCI node for PDCCH.
 * Description    : Allocates and fills information for DCI.
 ****************************************************************************/
DLStrategyTxNode * fillDlSpsDciInfo( 
        DLUEContext   *ueDLContext_p, 
        DLHARQProcess *harqProcess_p,
        MsgType       msgType,
        InternalCellIndex internalCellIndex);

/* + SPS_TDD_Changes */
void processSPSDLScheduling(UInt32 currentGlobalTTITickCount,
                        UInt32 delayToPhy,
			InternalCellIndex internalCellIndex);	 
void processDummyDlSPSSchedulerStrategy(UInt32 currentGlobalTTITickCount,
                        UInt32 delayToPhy,
                        InternalCellIndex internalCellIndex);
/* - SPS_TDD_Changes */
#endif
