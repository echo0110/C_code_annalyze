/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcMacInterface.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:45 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcMacInterface.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:45  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:07  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.13.2.2  2010/07/15 06:30:54  gur23054
 * Review comments fixes
 *
 * Revision 1.13.2.1  2010/06/22 09:13:05  gur19836
 * Updated for ARQ-HARQ Feature Enhancement
 *
 * Revision 1.13  2010/06/14 11:59:35  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.12  2009/06/04 05:20:34  gur19836
 * Optimization Changes
 *
 * Revision 1.11  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.10  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.9  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.8  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.7  2009/03/05 14:12:16  gur19140
 * Peer to Peer changes
 *
 * Revision 1.6  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.5  2009/03/02 05:34:08  gur19836
 * Updated for OAM Enhancement
 *
 * Revision 1.4  2009/02/09 14:23:35  gur19140
 * Fixed not able to de-queue problem
 *
 * Revision 1.3  2009/02/06 10:59:03  gur19836
 * CR comments incorporated
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef MACINTERFACE_H
#define MACINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcUmdPdu.h"
#include "lteLayer2MacRlcInterface.h"
#include "lteCircQueue.h"
#include "lteCommonQueueDefs.h"
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* This queue will be data interface from MAC layer to RLC layer. */
/** CA 2 Core Changes Start **/
extern MAC_UE_DATA_IND_TYPE macUeDataIndQ_g[MAX_NUM_CELL] ;
/** CA 2 Core Changes End **/
#if (defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED))
extern MAC_UE_DATA_IND_TYPE macEmbmsDataIndQ_g[MAX_NUM_CELL] ;
UInt32 maceMBMSDataInd(UInt8 areaIndex, RLCUplinkData* dataToRlc_p, UInt8 rxCellIndex); 
#endif


/*RLC Rel 2.0 Changes Start*/
/* This queue will be the harq failure indication interface from MAC layer
   to RLC layer*/
extern LTE_SQUEUE macUeHarqFailureIndQ_g;
/*Rlc Rel 2.0 Changes End*/
/* This queue will be common channel data interface from MAC layer to RLC 
   layer. */
extern LTE_SQUEUE  macComChDataIndQ_g ;
/* This queue will be common channel data request interface from MAC layer 
   to RLC layer. */
extern LTE_SQUEUE  macComChOpporIndQ_g ;

extern UInt16 rlcNumOfSupportedUsers_g;
extern UInt16 rlcNumOfSupportedAmLC_g; 
extern UInt16 rlcNumOfSupportedUmLC_g;

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_NODES_SPLIT (MAX_LOGICAL_CHANNEL * MAX_UE_SUPPORTED) 
 /*Macros for  Ind Queue */


/** CA 2 Core Changes Start **/
#define QDEINIT_MAC_UEDATA_IND_Q(cellIndex){\
    MacUeDataInd *cnode = RLC_NULL;\
    DEQUEUE_MAC_UE_DATA_IND_Q(MacUeDataInd,(void **)&cnode,cellIndex);\
    while(cnode  != RLC_NULL){\
        freeMemPool(cnode);\
        cnode = RLC_NULL;\
	DEQUEUE_MAC_UE_DATA_IND_Q(MacUeDataInd,(void **)&cnode,cellIndex);}\
    circQDeInit(&macUeDataIndQ_g[cellIndex]);\
}

/** CA 2 Core Changes End **/
/* CA Changes start */
#define QDEINIT_MAC_UEOPPOR_IND_Q(cellIndex){\
    MacUeOpporInd *cnode = RLC_NULL;\
    DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd ,(void **)&cnode, cellIndex);\
    while(cnode  != RLC_NULL){\
        freeMemPool(cnode);\
        cnode = RLC_NULL;\
	DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(MacUeOpporInd ,(void **)&cnode, cellIndex);}\
    circQDeInit(macUeOpporIndCircQ_gp[cellIndex]);\
}
/* CA Changes end */





/*RLC Rel 2.0 Changes Start*/
#define QINIT_MAC_UE_HARQ_FAILURE_IND_Q(){\
    sQueueInit(&macUeHarqFailureIndQ_g);\
}

#define QDEINIT_MAC_UE_HARQ_FAILURE_IND_Q(){\
    MacUeHarqFailureInd *snode = RLC_NULL;\
    while((snode = (MacUeHarqFailureInd *)popNode(&macUeHarqFailureIndQ_g))!=RLC_NULL)\
        freeMemPool(snode);\
}
/*RLC Rel 2.0 Changes End*/


#define QINIT_MAC_COMCH_DATA_IND_Q()\
        sQueueInit(&macComChDataIndQ_g)

#define QDEINIT_MAC_COMCH_DATA_IND_Q(){\
    MacComChDataInd *snode = RLC_NULL;\
    while((snode = (MacComChDataInd *)popNode(&macComChDataIndQ_g))!=RLC_NULL)\
        freeMemPool(snode);\
}

#define QINIT_MAC_COMCH_OPPOR_IND_Q()\
        sQueueInit(&macComChOpporIndQ_g)

#define QDEINIT_MAC_COMCH_OPPOR_IND_Q(){\
    MacComChOpporInd *snode = RLC_NULL;\
    while((snode = (MacComChOpporInd *)popNode(&macComChOpporIndQ_g))!=RLC_NULL)\
        freeMemPool(snode);\
}
#define QCOUNT_MAC_COMCH_DATA_IND_Q()\
        sQueueCount(&macComChDataIndQ_g)
#define QCOUNT_MAC_COMCH_OPPOR_IND_Q()\
        sQueueCount(&macComChOpporIndQ_g)
/*RLC Rel 2.0 Changes Start*/
#define QCOUNT_MAC_UE_HARQ_FAILURE_IND_Q()\
        sQueueCount(&macUeHarqFailureIndQ_g)
/*RLC Rel 2.0 Changes End*/




/*RLC Rel 2.0 Changes Start*/
#define DEQUEUE_MAC_UE_HARQ_FAILURE_IND_Q(harqFailureInd_p){ \
    harqFailureInd_p = (MacUeHarqFailureInd *)popNode(&macUeHarqFailureIndQ_g) ;\
}
/*RLC Rel 2.0 Changes End*/
#define DEQUEUE_MAC_COMCH_DATA_IND_Q(ind){ \
    ind = (MacComChDataInd *)popNode(&macComChDataIndQ_g) ;\
}

#define DEQUEUE_MAC_COMCH_OPPOR_IND_Q(ind){ \
    ind = (MacComChOpporInd *)popNode(&macComChOpporIndQ_g) ;\
}
/*RLC Rel 2.0 Changes Start*/    
#define ENQUEUE_MAC_UEHARQ_FAILURE_IND_Q(inMacUeHarqFailureInd_p) \
    pushNode(&macUeHarqFailureIndQ_g,&(inMacUeHarqFailureInd_p)->sNode)
/*RLC Rel 2.0 Changes End*/    

#define ENQUEUE_MAC_COMCH_DATA_IND_Q(inMacComChOpporInd_p) \
       pushNode(&macComChDataIndQ_g,&(inMacComChOpporInd_p)->sNode) 
#define ENQUEUE_MAC_COMCH_OPPOR_IND_Q(inMacComChDataInd_p) \
       pushNode(&macComChOpporIndQ_g,&(inMacComChDataInd_p)->sNode) 

#ifndef UE_SIM_TESTING

#define QINIT_RLC_MAC_PDU_IND_Q(cellIndex)\
    circQInit(&rlcMacPduIndQ_g[cellIndex],MAX_NODES_SEG_PDU_IND)

#define ENQUEUE_RLC_MAC_PDU_IND_Q(inRlcPduInd_p,cellIndex) \
            pushNodeCircQ(&rlcMacPduIndQ_g[cellIndex],inRlcPduInd_p)

#endif            

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* typedef for Indications to be queued */

typedef struct MacComChOpporIndT {
    LTE_SNODE sNode ;
    UInt16 comChType ;
    UInt16 rnti ;
    UInt32 size ;
} MacComChOpporInd ;

typedef struct MacComChDataIndT {
    LTE_SNODE sNode ;
    UInt16 comChType ;
    UInt16 rnti ;
    UInt8* data_p ;
    UInt32 size ;
} MacComChDataInd ;

typedef struct MacUeOpporIndT {
    LTE_SNODE sNode ;
    UInt16 ueIndex ;
    UInt8 lcCount ;
    RLCDataBuffer* rlcBuffer_p ;
    UInt32 requestedSubFrame;
} MacUeOpporInd ;


/** SPR 3657 Changes Start **/
typedef struct RLCHarqFailReTxInfoT {
    UInt16  retxPduSeqNum;
    UInt16  SOStart;   /* RLC PDU buffer Segment Offset Start */
    UInt16  SOEnd;
}rlcHarqFailReTxInfo;


typedef struct MacUeHarqFailureIndT {
    LTE_SNODE sNode;
    UInt16 ueIndex;
    UInt8 lcId;
    UInt16  newPduSeqNum;
    /** SPR 3657 Changes Start **/
    UInt32 reTxPduCount;
    rlcHarqFailReTxInfo harqFailReTxInfo[MAX_RETX_PDU];
    /** SPR 3657 Changes End **/
} MacUeHarqFailureInd;


typedef struct MacUeDataIndT {
    UInt16 ueIndex ;
    RLCUplinkData* dataToRlc_p ;
} MacUeDataInd ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* Initializes the MAC interface. */
/* + SPR 17439 */
void rlcInitMacInterface(void) ;
void rlcDeInitMacInterface(void) ;
/* - SPR 17439 */

/* FAPI L2 AS MASTER START */
#ifdef FAPI_L2_MASTER
/* SPR 15909 fix start */
extern void  macFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
#ifndef UE_SIM_TESTING
extern void rlcFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
/* SPR 15909 fix end */
#endif
#endif
/* FAPI L2 AS MASTER END */

#endif  /* INCLUDED_MACINTERFACE_H */
