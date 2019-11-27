/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmTxOperation.h,v 1.1.6.2.2.1 2010/09/21 15:49:22 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the Tx Operations of AM Entity
 ****************************************************************************
 *  Revision Details
 * ------------------
 *
 *  $Log: lteRlcAmTxOperation.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:22  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.1  2010/08/10 09:37:44  gur19836
 *  Files added for RLC AM Release
 *
 *  Revision 1.1.6.7  2010/07/28 04:18:22  gur19836
 *  UT Bug Fixes
 *
 *  Revision 1.1.6.6  2010/07/17 11:33:08  gur23054
 *  Review Comments fixed
 *
 *  Revision 1.1.6.5  2010/07/05 09:43:13  gur23054
 *  Corrected Indentation
 *
 *  Revision 1.1.6.4  2010/06/28 10:16:50  gur23054
 *  rlcActionOnNackReceiveStatusReport-argument added function
 *
 *  Revision 1.1.6.3  2010/06/25 09:49:52  gur23054
 *  Definitions Exported
 *
 *  Revision 1.1.6.2  2010/06/25 04:29:39  gur23054
 *  Exported function added
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:38  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.3  2009/11/10 09:03:45  gur19140
 *  Minor changes
 *
 *  Revision 1.1.2.2  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 *
 ****************************************************************************/

#ifndef RLCAMTXOPERATION_H
#define RLCAMTXOPERATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteLayer2MacRlcInterface.h"
#include "lteRlcTxProcessor.h"
#include "lteRlcTypes.h"
#include "lteRlcEntityDef.h"
#include "lteRlcAmArithmetic.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcAmdEncode.h"
#include "lteRlcAmdSdu.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define REPORTING_TRUE 1
#define REPORTING_FALSE 0

#define AM_PDU_SEG_FIX_HDR_SIZE 2
/**SPR 1203 Start **/
#define AM_PDU_RESEG_FIX_HDR_SIZE 4
/** SPR 1203 End**/


/****************************************************************************
 * Exported Functions
 ****************************************************************************/
UInt32 rlcReceiveAmSduFromUpperLayer
(
    TxAMEntity * pE, 
    RlcUeDataReq *pAmdSdu
);
UInt32 rlcReceiveAmSduDiscardFromUpperLayer
(
    TxAMEntity * pE,
    /* SPR 2812 changes start */
    void *transId
    /* SPR 2812 changes end */
);
/*Rlc Rel 2.0 Changes Start*/
UInt32 rlcHandleTxOppForAMEntity
(
    AMEntity* amEntity_p,
    RLCDataBuffer* rlcDataBuffer_p,
    LCQueueLoad * lcQLoadArr_p, 
    RLCHarqFailureInfo* rlcHarqFailureInfo_p
);

/** Rel 3.1 Chanegs Start **/
void rlcFetchQueueLoad
(
    TxAMEntity* txAMEntity_p,
    LCQueueLoad *lcQueueLoad
    /** Rel 3.1 Chanegs End **/
    
);
/*Rlc Rel 2.0 Changes End*/
RlcSendPduTypes rlcGetTransmisionTypePDU
(
    TxAMEntity* pAmTxE
);
#endif  /* RLCAMTXOPERATION_H */
