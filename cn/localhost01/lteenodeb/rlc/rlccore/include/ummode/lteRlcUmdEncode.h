/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdEncode.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:58 gur20491 Exp $
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
 * $Log: lteRlcUmdEncode.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:58  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.6.2.2  2010/07/17 13:34:28  gur21148
 * RLC_UM_FIXES
 *
 * Revision 1.6.2.1  2010/07/17 11:40:50  gur23054
 * Review Comments fixed
 *
 * Revision 1.6  2010/06/14 12:00:57  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.5  2009/06/04 05:20:38  gur19836
 * Optimization Changes
 *
 * Revision 1.4  2009/04/07 09:27:32  gur19140
 * status updated
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
#ifndef UMDENCODE_H
#define UMDENCODE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcEntityDef.h"
#include "lteRlcTypes.h"
#include "lteRlcUmdPdu.h"
#include "lteRlcUmdSdu.h"
#include "lteRlcTxProcessor.h"
#include "lteRlcStat.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* SPR 5599_129119 Fix Start */
UInt32 rlcEncodeUmdRawPdu( TxUMEntity *pE,
/* SPR 5599_129119 Fix End */
                                  UInt32* pAvaliableSize, 
                                  UInt32* txQueueSize_p, 
                                  UmdRawPdu *pOutPdu,
                                  UInt32 *dfeCount,
                                  /* SPR 15909 fix start */
                                  tickType_t *pktArrTime
                                  /* SPR 15909 fix end */
                                 );

 UInt32 rlcSetFixedHdrUmdRawPdu( UInt8 *base_data_p,
                                              UInt8 fi,
                                              UInt8 e,
                                              UInt16 sn,
                                              UInt32 snLen
                                            );

 UInt32 rlcEncodePduHeader(UInt8 *encodedPdu_p,
                                        RlcPduHeaderInfo *pduHdrInfo_p,
                                        UInt32 *pSzRawPdu);

 UInt32 rlcSetFixedHdrUmdRawPdu( UInt8 *base_data_p,
                                              UInt8 fi,
                                              UInt8 e,
                                              UInt16 sn,
                                              UInt32 snLen
                                            );

 UInt32 rlcAddDfeToPdu(UInt8 *encodedPdu_p,
                                    UInt8 *pDfe,
                                    UInt16 size,
                                    RlcPduHeaderInfo *pduHdrInfo_p,
                                    UInt32 *pAvailableSize,
                                    UmdRawSduIn **incompleteSdu_p);

 UInt32 rlcAddSduSegmentToPdu(UInt8 *encodedPdu_p, 
                                           RlcPduHeaderInfo *pduHdrInfo_p,
                                           UInt32 *pAvailableSize,
                                           UmdRawSduIn **incompleteSdu_p);
#endif  /* UMDENCODE_H */
