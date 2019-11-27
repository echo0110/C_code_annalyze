/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmRxOperation.h,v 1.1.6.2.2.1 2010/09/21 15:49:21 gur20491 Exp $ 
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the Rx Operations of AM Entity
 *
 ****************************************************************************
 *  Revision Details
 * ------------------
 *
 *  $Log: lteRlcAmRxOperation.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:21  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.1  2010/08/10 09:37:44  gur19836
 *  Files added for RLC AM Release
 *
 *  Revision 1.1.6.4  2010/08/08 05:14:36  gur19479
 *  updated for ut fixes
 *
 *  Revision 1.1.6.3  2010/07/17 11:32:50  gur23054
 *  Review Comments fixed
 *
 *  Revision 1.1.6.2  2010/07/15 06:29:35  gur23054
 *  Review comments fixes
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:38  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.4  2009/11/10 09:03:19  gur19140
 *  RLC RX init draft
 *
 *  Revision 1.1.2.3  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 *
 ****************************************************************************/

#ifndef AMRXOPERATION_H
#define AMRXOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcAmArithmetic.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcAmdSdu.h"
#include "lteRlcAmdRing.h"
#include "lteRlcEntityDef.h"
#include "lteRlcAmdArqOperation.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/** SPR 1753 Changes Start **/
#define RLC_DRB_LC 3
UInt32 rlcReceiveAmdPduFromLowerLayer
(
    AMEntity *amEntity, 
    AmdRawPdu *pAmdRawPdu
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
); 


typedef struct AmdRawPduIndT {
    LTE_SNODE sNode ;
    UInt32  dataLength;
    UInt8 *rlcAmdRawPduBuf_p;
    UInt16 ueIndex ;
    RlcLcId lcId ;

}AmdRawPduInd;
/** SPR 1753 Changes End **/
UInt32 rlcActionAmdPduInReceptionBuffer
(
    AMEntity *amEntity,
    RlcSn x,
    UInt8 triggerStatFlag
);
/* SPR 5599_129119 Fix Start */
UInt32 rlcActionAmTReorderingExpire
/* SPR 5599_129119 Fix End */
(
    RxAMEntity * pRxAMEntity
);
UInt32 rlcReassembleAmdSdu
(
    RxAMEntity * pRxAMEntity,
    RlcSnList *snList
);
UInt32 rlcReassembleAmdSduForReestablish
(
    RxAMEntity * pRxAMEntity,
    RlcSnList *snList
);
UInt32 rlcDeliverAmSduToUpperLayer 
(
    RxAMEntity * pRxAMEntity,
    RlcAmBuffer * rlcAmBuffer
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    , tickType_t      packetTime
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
); 

UInt32 rlcActionOnExpireStatusProhibitTimer
(
    RxAMEntity * pAmRxE
);
UInt32 rlcTriggerStatusReport
(
    AMEntity *amEntity
);
UInt32 rlcGetRingStatusForAM
(
    RxAMEntity * rxAMEntity_p
);
#endif  
