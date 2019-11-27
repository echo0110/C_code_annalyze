/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTxProcessor.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:42 gur20491 Exp $
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
 * $Log: lteRlcTxProcessor.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:42  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.2.1  2010/07/17 11:34:59  gur23054
 * Review Comments fixed
 *
 * Revision 1.7  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.6  2009/06/04 05:20:31  gur19836
 * Optimization Changes
 *
 * Revision 1.5  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.4  2009/03/06 12:44:41  gur19140
 * Review comments fixes
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

#ifndef RLCTXPROCESSOR_H
#define RLCTXPROCESSOR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcThread.h"
#include "lteRlcEntityDef.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define MAX_PDCP_DATA_HANDLING 2
#define MAX_TX_PROCESSOR_LOOP 10

/* SPR 4497 Changes Start */
#define RLC_PDCP_SDU_ENQUEUE_FAILURE 2
/* SPR 4497 Changes End */
/* SPR 14622 fix start */
#if defined (RLC_PDCP_IPC_INTF) && !defined (UE_SIM_TESTING)
#define RLC_DISCARD_SUCCESS  3
#endif
/* SPR 14622 fix end */

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

extern RlcThread rlcTxThread_g ;


/****************************************************************************
 * Exported Functions
 ****************************************************************************/

void* rlcRunTxProcessor(void* args);
SInt32 rlcInitTxThread(void* args);
/* SPR 5599_129119 Fix Start */
UInt32 rlcHandleDataFromPdcp(RlcUeDataReq* ueDataReq_p);
/** CA Changes Start **/
#if !defined (DL_DATA_SEPARATION ) || defined (UE_SIM_TESTING)
UInt32 rlcHandleTxOppurtunity(InternalCellIndex cellIndex); 
#else
/*SPR 15909 Fix Start*/
UInt32 rlcHandleTxOppurtunity(tickType_t currentTick,InternalCellIndex cellIndex);
/*SPR 15909 Fix Start*/
#endif
/** CA Changes End **/

/* SPR 5599_129119 Fix End */
/* + SPR 17439 */
UInt32 rlcHandleComChTxOppurtunity(void);
/*RLC Rel 2.0 Changes Start*/
UInt32 rlcHandleTxExpiredTimer(void);
UInt32 rlcHandleControlPduForRlcAm(void);
/* - SPR 17439 */
/*coverity-530-32868 */
#ifdef OBSELETE_REL_8
/*coverity-530-32868 */
UInt32 rlcHandleHarqFailureInd(UInt32 count);
#endif
/*RLC Rel 2.0 Changes End*/
UInt32 rlcRunTxExecutionLeg(void);
/* SPR 5599_129119 Fix Start */
UInt32 rlcUMFetchQLoad( TxUMEntity* pE,
                               LCQueueLoad *lcQLoadArr_p
                             );
/* SPR 5599_129119 Fix End */
#ifdef RLC_STANDALONE_SETUP
LTE_SEM rlcTxSemlock_g ;
#endif


#if defined(ENDC_ENABLED) && defined(PDCP_GTPU_INTF)
/*OPTION3X Changes Start */
void handlePdcpDataFromX2u();
/*OPTION3X Changes End */
#endif


#endif  /* RLCTXPROCESSOR_H */
