/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdDecode.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:57 gur20491 Exp $
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
 * $Log: lteRlcUmdDecode.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:57  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4.2.1  2010/07/17 11:40:44  gur23054
 * Review Comments fixed
 *
 * Revision 1.4  2010/06/14 12:00:57  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/
#ifndef UMDDECODE_H
#define UMDDECODE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcUmdPdu.h"
#include "lteRlcUmdSdu.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt32 rlcGetDFECount(UInt8* ,UInt32*) ;
UInt32 rlcDecodeUmdH(UmdRawPdu *rawPdu,UmdH *h,UInt8 snLen);

UInt32 rlcDecodeUmdRawPdu(UmdRawPdu *rawPdu_p,
                          UmdPdu *umdPdu_p,
                          UmdH * umdHdr_p,
                          UInt8 hIdx
#ifndef UE_SIM_TESTING
                          , UInt32 *rlcRxSegCounter
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
#endif
    /* 32.425 Events code changes end*/
);

UmdDFE* rlcExtractNextDFE(UmdDFEQ* q,UInt8* data, UInt32 *hIdx,UInt32 *dfIdx,UInt32 *szRemainingDF,UInt8* eToggle);
UInt32 rlcSpiltNextDFE(UmdDFEQ* q,UInt8* header,UInt32 *hIdx, UmdDFE **dfesSegment,UInt8* eToggle);

#endif  /* UMDDECODE_H */
