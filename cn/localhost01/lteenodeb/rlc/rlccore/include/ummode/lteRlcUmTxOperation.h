/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmTxOperation.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:56 gur20491 Exp $
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
 * $Log: lteRlcUmTxOperation.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:56  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.5.2.1  2010/07/17 11:38:11  gur23054
 * Review Comments fixed
 *
 * Revision 1.5  2010/06/14 12:00:57  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.4  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.3  2009/03/02 09:10:54  gur19140
 * Changes reqarding Zero copy,pdcp discard and execution legs.
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

#ifndef RLCUMTXOPERATION_H
#define RLCUMTXOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteRlcTxProcessor.h"
#include "lteRlcTypes.h"
#include "lteRlcEntityDef.h"
#include "lteRlcUmArithmetic.h"
#include "lteRlcUmdPdu.h"
#include "lteRlcUmdEncode.h"
#include "lteRlcUmdSdu.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcReceiveUmSduFromUpperLayer(TxUMEntity * pE, RlcUeDataReq *pUmdSdu) ;
UInt32 rlcReceiveUmSduDiscardFromUpperLayer(TxUMEntity * pE, void *transId);

#endif  /* RLCUMTXOPERATION_H */
