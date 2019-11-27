/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmRxOperation.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:55 gur20491 Exp $
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
 * $Log: lteRlcUmRxOperation.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:55  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.3.28.1  2010/07/17 11:38:05  gur23054
 * Review Comments fixed
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
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

#ifndef RLCUMRXOPERATION_H
#define RLCUMRXOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcUmArithmetic.h"
#include "lteRlcUmdPdu.h"
#include "lteRlcUmdSdu.h"
#include "lteRlcUmdRing.h"
#include "lteRlcEntityDef.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcReceiveUmPduFromLowerLayer(RxUMEntity * pRxUMEntity, 
                                     UmdRawPdu *pUmdRawPdu
#ifndef UE_SIM_TESTING
                                     /* SPR 15909 fix start */
                                     , tickType_t pktTimeStamp,
                                     /* SPR 15909 fix end */
                                     UInt8 spsState
#endif
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                     /* SPR 15909 fix start */
                                     ,tickType_t packetTime
                                     /* SPR 15909 fix end */
    /* 32.425 Events code changes end*/
#endif
                                     );
UInt32 rlcReassembleUmSdu( RxUMEntity * rxUMEntity_p,
                           RlcSnList *snList
                         );
                                                    
UInt32 rlcActionUmTReorderingExpire(RxUMEntity * pRxUMEntity) ;

#ifndef UE_SIM_TESTING
void macUeSegPduInd(UInt32 ueIndex, UInt8 lcId, UInt8 rlcPduInd,InternalCellIndex cellIndex);
#endif  
inline UInt32 isValidRxUMPduForReceptionBuffer(RxUMEntity *rxUMEntity_p ,UInt16 seqNum);
#endif  
