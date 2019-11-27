/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcRxProcessor.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:36 gur20491 Exp $
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
 * $Log: lteRlcRxProcessor.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:36  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.4.14.1  2010/07/17 11:34:54  gur23054
 * Review Comments fixed
 *
 * Revision 1.4  2009/06/04 05:20:31  gur19836
 * Optimization Changes
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/02/09 11:30:46  gur19140
 * Changes to make Tx Rx thread wait at Sema
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCRXPROCESSOR_H
#define RLCRXPROCESSOR_H
/* intel specific */

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcThread.h"
#include "lteRlcMacInterface.h"
#include "lteRlcPdcpInterface.h"
#include "lteRlcUmRxOperation.h"
#include "lteRlcUeContext.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define RLC_MAX_PDU_LENGTH 18844
/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern RlcThread rlcRxThread_g ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void* rlcRunRxProcessor(void* args) ;
SInt32 rlcInitRxThread(void* args) ;
/* + SPR 17439 */
SInt32 rlcDeInitRxThread(void) ;
/* - SPR 17439 */
/* SPR 5599_129119 Fix Start */
/** CA 2 Core Changes Start **/
UInt32 rlcHandleDataFromMac(UInt32 count,InternalCellIndex internalCellIndex) ;
/** CA 2 Core Changes End **/
/* SPR 5599_129119 Fix End */
/* + SPR 17439 */
UInt32 rlcHandleComChDataFromMac(void) ;
/*RLC Rel 2.0 Changes Start*/
UInt32 rlcHandleRxExpiredTimer(void) ;
/* - SPR 17439 */
/*RLC Rel 2.0 Changes End*/
UInt32 rlcRunRxExecutionLeg(void);

/* SPR 5599_129119 Fix Start */
UInt32 rlcGetHeaderInfo(  UInt8* data_p,
                                  UInt32 fixedHdrIdx,
                                  RlcPduHeaderInfo *pduHdrInfo_p
                                );
/* SPR 5599_129119 Fix End */
/* +EMBMS CHANGES*/
UInt32 handleRlcUMRxPacket( RLCUplinkDataInfo* uplinkDataArr_p ,RxUMEntity* rxUMEntity_p);
/* -EMBMS CHANGES*/


#endif  /* RLCRXPROCESSOR_H */
