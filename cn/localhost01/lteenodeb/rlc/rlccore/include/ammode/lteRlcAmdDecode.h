/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdDecode.h,v 1.1.6.2.2.1 2010/09/21 15:49:24 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcAmdDecode.h,v $
 * Revision 1.1.6.2.2.1  2010/09/21 15:49:24  gur20491
 * FAPI changes
 *
 * Revision 1.1.2.1  2010/08/10 09:37:44  gur19836
 * Files added for RLC AM Release
 *
 * Revision 1.1.6.10  2010/08/05 15:51:08  gur19479
 * updated for resegmentation
 *
 * Revision 1.1.6.9  2010/08/05 14:24:52  gur19836
 * UT Bug Fixes
 *
 * Revision 1.1.6.8  2010/07/17 11:33:16  gur23054
 * Review Comments fixed
 *
 * Revision 1.1.6.7  2010/07/04 12:58:04  gur19479
 * updated for prototype
 *
 * Revision 1.1.6.6  2010/07/04 09:42:34  gur19479
 * updated for macros
 *
 * Revision 1.1.6.5  2010/07/04 09:03:29  gur19479
 * updated for prototype
 *
 * Revision 1.1.6.4  2010/07/03 06:27:15  gur19479
 * updated prototype
 *
 * Revision 1.1.6.3  2010/07/03 06:12:42  gur19479
 * updated to remove warnings
 *
 * Revision 1.1.6.2  2010/07/03 06:04:10  gur19479
 * updated for resegmentation
 *
 * Revision 1.1.6.1  2010/06/17 10:59:39  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.1.2.4  2009/11/10 09:05:07  gur19140
 * RLC AM Rx Init Draft
 *
 * Revision 1.1.2.3  2009/10/28 09:01:42  gur19140
 * Minor changes for file headers
 *
 *
 *
 ****************************************************************************/
#ifndef AMDDECODE_H
#define AMDDECODE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcTypes.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcAmdSdu.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcGetAmdDFECount
(
    UInt8 * ,UInt32*
);
UInt32 rlcDecodeAmdHeader
(
    AmdRawPdu *rawPdu,
    AmdH *h
);
UInt32 rlcDecodeAmdRawPdu
(
    AmdRawPdu *rawPdu_p,
    AmdPdu *amdPdu_p,
    AmdH * amdHdr_p
    /* 32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
    /* SPR 15909 fix start */
    ,tickType_t packetTime
    /* SPR 15909 fix end */
#endif
    /* 32.425 Events code changes end*/
    );
UInt32 rlcSpiltNextAmdDFE
(
    AmdDFEQ* q,
    UInt8* header,
    UInt32 *hIdx, 
    AmdDFE **dfesSegment,
    UInt8* eToggle
);
void rlcInsertAmdSegmentInReptBuffer
(
    AmdPdu *amdPdu_p,
    AmdPduSegment * amdPduSegment_p,
    AmdH * amdHdr_p,
    AmdPduSegmentInfo* segmentInfo_p
);
AmdPduSegment* rlcDecodeAmdRawPduSegment
(
    AmdPdu *amdPdu_p,
    AmdH * amdHdr_p,
    AmdPduSegmentInfo* segmentInfo_p
);
UInt32 rlcAppendDfeInToSegmentQueue
(
    AmdDFEQ *amdDfeQ_p,
    AmdH * amdHdr_p,
    UInt8 **data_p,
    UInt16 soStart,
    UInt16 soEnd,
    UInt16 addOff,
    UInt16 lastDfe 
);

UInt32 rlcCheckForDuplicateSegment
(
    AmdRawPdu *rawPdu_p,
    AmdPdu *amdPdu_p,
    AmdH * amdHdr_p
);
UInt32 rlcProcessAmdSegment
(
    AmdPduSegmentInfo *segmentInfo,
    AmdH * amdHdr_p,
    AmdRawPdu *rawPdu_p,
    AmdPdu *amdPdu_p,
    UInt16 nodeSoStart, 
    UInt16 nodeSoEnd
);
UInt32 handleRlcAmdPduSegment (
                                 AmdRawPdu *rawPdu_p,
                                 AmdPdu *amdPdu_p,
                                 AmdH * amdHdr_p
                                 /*32.425 Events code changes start*/
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
                                 /* SPR 15909 fix start */
                                 ,tickType_t packetTime
                                 /* SPR 15909 fix end */
#endif
                                 /*32.425 Events code changes end*/
        );
#endif  /* AMDDECODE_H */
