/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcSnList.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:37 gur20491 Exp $
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
 * $Log: lteRlcSnList.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:37  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4.28.1  2010/07/17 11:35:21  gur23054
 * Review Comments fixed
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/02/11 13:25:19  gur19140
 * fix logging issue timer issue snList issue
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

#ifndef RLCSNLIST_H
#define RLCSNLIST_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMemPool.h"
#include "lteMisc.h"
#include "lteRlcTypes.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RLC_SN_LIST_SIZE_DEFAULT 512

#define RLC_SN_LIST_INIT {RLC_SN_LIST_SIZE_DEFAULT,0,{0,}}

#define GET_SN_LIST_SIZE(RlcSnList_p) \
    RlcSnList_p->size

#define GET_SN_LIST_SIZE_USED(RlcSnList_p) \
    RlcSnList_p->szUsed

#define GET_SN_LIST_PTR(RlcSnList_p) \
    (RlcSnList_p != RLC_NULL ? RlcSnList_p->snList : RLC_NULL )
    
#define FREE_SN_LIST(RlcSnList_p) \
    freeMemPool(RlcSnList_p)

#define GET_SN_LIST(RlcSnList_p, ui32_idx) \
    RlcSnList_p->snList[ui32_idx % RlcSnList_p->size]

#define RESET_SN_LIST(RlcSnList_p) \
    RlcSnList_p->szUsed = 0

#define SORT_SN_LIST(RlcSnList_p){ \
    RlcSn *snList = &RlcSnList_p->snList[0];\
    rlcHeapsort((UInt32*)snList,RlcSnList_p->szUsed);\
}


/****************************************************************************
 * Exported Types
 ****************************************************************************/
 /* if RlcSnList is allocated on stack instesd of Heap , 
 its default size RLC_SN_LIST_SIZE_DEFAULT is considered 
 no need to set size member of RlcSnList */
typedef struct RlcSnListT {
    UInt32 size ;
    UInt32 szUsed ;
    RlcSn snList[RLC_SN_LIST_SIZE_DEFAULT] ;
} RlcSnList ;


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

RlcSnList *rlcCreateSnList(UInt32 size) ;

void rlcSetSnList(RlcSnList* snL,RlcSn sn) ;

void rlcReverseSnList(RlcSnList* snL) ;
#endif  /* RlcSnList_H */
