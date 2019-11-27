/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUeContext.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:44 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  This file declares the schema for database.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcUeContext.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:44  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.9.2.1  2010/07/17 11:36:02  gur23054
 * Review Comments fixed
 *
 * Revision 1.9  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.8  2009/06/04 05:20:31  gur19836
 * Optimization Changes
 *
 * Revision 1.7  2009/04/15 16:04:22  gur19140
 * rlc latency + logging related changes
 *
 * Revision 1.6  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.5  2009/02/11 10:29:21  gur19836
 * UT Defects Fixed
 *
 * Revision 1.4  2009/02/06 15:39:33  gur20294
 * updated
 *
 * Revision 1.3  2009/02/06 15:03:49  gur20294
 * updated
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

#ifndef LTERLCUECONTEXT_H
#define LTERLCUECONTEXT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcDefs.h"
#include "lteRlcEntityDef.h"
#include "lteRlcRrcInterface.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
 /* fetch the ue context based on ueIndex */

#define ONE_BYTE_FIX_HEADER 1
#define TWO_BYTE_FIX_HEADER 2



#define DB_FETCH_UE_CONTEXT(ui32_ueIndex) \
    rlcUEContextList_g[ui32_ueIndex]
/* fetch the rlc entity info based on the lcId and ueIndex */
#define DB_FETCH_RLC_ENTITY_INFO(UeContext_p,RlcLcId_lcId) \
        UeContext_p->rlcEntityInfoList[RlcLcId_lcId]
/* fetch the comon channel entity */
#define DB_FETCH_COMCH_TM_ENTITY(ui32_ComChType) \
        rlcComChContext_g.rlcTmComChList[ui32_ComChType]
/* + PERF_CA GBL */
/* fetch cellIndex from ueIndex */
#define RLC_FETCH_INTERNAL_CELL_INDEX(ui32_ueIndex) \
    rlcUEContextList_g[ui32_ueIndex]->cellIndex
/* - PERF_CA GBL */
/****************************************************************************
 * Exported Types
 ****************************************************************************/


/*SPR 16855 +-*/

/* This structure is used to send the information from control thread to
 * RLC high priority thread */
typedef struct RlcRrcOamPendingReqLPT
{
    RlcConfigReq rlcConfigReq;
    RlcReEstabReq rlcReEstabReq ;
    /* SPR 19698 Changes Start */
    RlcLcId lcId[MAX_LC_ID];
    RlcEntityType entityType[MAX_LC_ID];
    UInt16 lcCount;
    /* SPR 19698 Changes End */
    UInt8 nodesPushedInQueueCount;
    UInt8 noOfFailures;
}RlcRrcOamPendingReqLP;


/* This structure is used to send the information from RLC high priority 
 * thread to control thread */
typedef struct RlcRrcOamHiPrioQNodeT
{
    UInt8 rlcState;
    UInt8 processRrcOamMsgEvent;
    /* CA Changes start */
    RrcCellIndex cellId;
    UInt16 transactionId;
    /* CA Changes end */
    UInt16 ueIndex;
}RlcRrcOamHiPrioQNode;

/*SPR 16855 +-*/


typedef struct UEContextT 
{
    RlcUEState rlcUEState;
    RlcUeId ueId ;
/* + Layer2 NON CA Changes */
/*SPR 21653 changes start*/
/*commented lines removed*/
    UInt16 rrcUeIndex;
/*SPR 21653 changes end*/
/* - Layer2 NON CA Changes */
    /*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    /* flag which is used in decision making while processing the 
       delete request for UE context*/
    UInt8 delProcessCnt;
#endif
    /* Array that contains pointer to the RLCEntityInfo structure.
       Its each index is directly mapped to the logical channel 
       identifier for direct memory access.  */
    RLCEntityInfo* rlcEntityInfoList[MAX_LC_ID] ;

#ifndef UE_SIM_TESTING
    /* DL Request Type received from L3: SETUP (1) or RELEASE (0) */
    UInt8    dlSpsReqType;
    /* Semi Persistant Interval in DL */
    UInt8    semiPersistentSchedIntervalDl;
    /* UL Request Type received from L3: SETUP (1) or RELEASE (0) */
    UInt8    ulSpsReqType;
    /* Semi Persistant Interval in UL */
    UInt8    semiPersistentSchedIntervalUl;
    /* + Layer2 NON CA Changes */
#endif    
    InternalCellIndex   cellIndex;
    /* - Layer2 NON CA Changes */

    /*SPR 16855 +-*/
    RlcRrcOamPendingReqLP * rlcRrcOamPendingReq_p;
    /*SPR 16855 +-*/
} UEContext ;

typedef struct ComChContextT {
    TMEntity* rlcTmComChList[MAX_COMMON_CHANNEL] ;
} ComChContext ;


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/* List of UE context addresses. This will store the list of
  pointers to active UEs context i.e. UEContext structure.  */
  
extern UEContext* rlcUEContextList_g[MAX_UE_ID] ;
extern ComChContext rlcComChContext_g ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* + SPR 17439 */
void rlcDbInitDB(void) ;
void rlcDbDeInitDB(void) ;
/* - SPR 17439 */
void rlcDbInitUEContextDB(UEContext* pUEContext) ;
//void rlcDbDeInitUEContext(UEContext* pUEContext) ;

UEContext* rlcDbAllocateUEContext(UInt32 ueIndex) ;
UInt32 rlcDbDeleteUEContext(UInt32 ueIndex) ;

RLCEntityInfo* rlcDbAllocateRLCEntityInfo(UInt32 ueIndex,RlcLcId lcId,
                                       RlcEntityMode eM,RlcEntityType eT);
UInt32 rlcDbDeleteRLCEntityInfo(UInt32 ueIndex,RlcLcId lcId, 
                             RlcEntityType entityType);
void rlcGetRlcEntityID(RLCEntityInfo* eInfo_p,UInt32 *ueIndex, RlcLcId *lcId);

void rlcCalculateLatencyForUl(UInt32 ueIndex,TimeSpec *startTime,
                           UInt32 requestFailure, UInt16 flag);
void rlcCalculateLatencyForDl(UInt32 ueIndex,TimeSpec *startTime,
                           UInt32 requestFailure, UInt16 flag);


#endif  /**/
