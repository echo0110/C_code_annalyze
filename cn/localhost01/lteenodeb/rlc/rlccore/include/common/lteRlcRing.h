/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRing.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:35 gur20491 Exp $
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
 * $Log: lteRlcRing.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:35  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4.28.2  2010/07/17 11:34:48  gur23054
 * Review Comments fixed
 *
 * Revision 1.4.28.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/02/10 17:16:36  gur20294
 * assign data pointer of ring not to RLC_NULL after freeMemPool
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

#ifndef RLCRING_H
#define RLCRING_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
typedef UInt16 RingSz ;
typedef UInt16 RingNodeId ;

/*RLC Rel 2.0 Changes Start*/
#define RING_DE_INIT(Ring_p){ \
    (Ring_p)->size = 0;\
    if((Ring_p)->node_p){ \
	/* + SPR 9646 Fix */\
        /*semDestroy((LTE_SEM *)&(((AmdPdu *)(Ring_p)->node_p->nD)->LQ).sem_lock);*/\
	/* - SPR 9646 Fix */\
        freeMemPool((Ring_p)->node_p);\
    }\
    (Ring_p)->node_p = RLC_NULL;\
}
/*RLC Rel 2.0 Changes End*/

#define GET_RING_SIZE(Ring_p) (Ring_p)->size

#define GET_RING_SS(Ring_p,RingNodeId) \
    (Ring_p)->node_p[(RingNodeId)].nodeSlotStatus
   
#define SET_RING_SS(Ring_p,RingNodeId,RingSS) \
    (Ring_p)->node_p[(RingNodeId)].nodeSlotStatus = (RingSS)

#define GET_RING_DATA(Ring_p,RingNodeId) \
    (Ring_p)->node_p[(RingNodeId)].nD
    
    //    (Ring_p)->node_p[(RingNodeId) % (Ring_p)->size].nD

#define SET_RING_DATA(Ring_p,RingNodeId,voiddata_p){ \
    (Ring_p)->node_p[(RingNodeId)].nD = (voiddata_p);\
    (Ring_p)->node_p[(RingNodeId)].nodeSlotStatus = RING_SLOT_FREE;\
}    

/****************************************************************************
 * Exported Types
 ****************************************************************************/

typedef enum RingSlotStatusT {
    RING_SLOT_FREE = 0 ,
    RING_SLOT_IN_USE = 1 ,
    RING_SLOT_READY = 2
} RingSS ;

typedef struct RingNodeT {
    RingSS nodeSlotStatus ;
    void* nD ;
} RingN ;

typedef struct RingT {
    RingSz size ;
    RingN *node_p ;
} Ring ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* Below is list of ring  operations */

UInt32 rlcRingInit(Ring *pR,RingSz sz) ;

#endif  /* RLCRING_H */
