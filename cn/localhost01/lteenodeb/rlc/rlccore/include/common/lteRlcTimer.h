/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTimer.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:41 gur20491 Exp $
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
 * $Log: lteRlcTimer.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:41  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.7.14.2  2010/07/17 11:35:42  gur23054
 * Review Comments fixed
 *
 * Revision 1.7.14.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.7  2009/06/04 05:20:31  gur19836
 * Optimization Changes
 *
 * Revision 1.6  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.5  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.4  2009/03/02 05:32:51  gur19836
 * *** empty log message ***
 *
 * Revision 1.3  2009/02/09 09:19:43  gur19140
 * Update to make rlc init successufully
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

#ifndef RLCTIMER_H
#define RLCTIMER_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
/* intel specific */

//#include "lteRlcEntityDef.h"
#include "lteRlcTypes.h"
#include "lteRlcThread.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define INVALID_EXP_IDX 600
#define RLC_TIMER_NODE_VALID 112
#define INVALID_RLC_TIMER_NODE 113

/****************************************************************************
 * Exported Types
 ****************************************************************************/




/****************************************************************************
 * Varibale Definition
 ****************************************************************************/
/* there is separate thread for timer */
extern RlcThread rlcTmThread_g ;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void* rlcRunTmProcessor(void* arg) ;
SInt32 rlcInitTmThread(void* arg) ;
/* + SPR 17439 */
SInt32 rlcDeInitTmThread(void);
/* Timer queue methods */
void rlcInitTimerInterface(void) ;
void rlcDeInitTimerInterface(void);
/* - SPR 17439 */
/*****Status Prohibit Timer ************/
/* SPR 5599_129119 Fix Start */
UInt32 startSPTimer (  RxAMEntity* rxAMEntity_p);
UInt32  stopSPTimer ( RxAMEntity* rxAMEntity_p);
void restartSPTimer ( RxAMEntity* rxAMEntity_p);
/* SPR 5599_129119 Fix End */

/***************** UM Reordering Timer *******/
/* SPR 5599_129119 Fix Start */
UInt32 startUMROTimer( RxUMEntity* rxUMEntity_p);
UInt32 stopUMROTimer(  RxUMEntity* rxUMEntity_p);
void restartUMROTimer( RxUMEntity* rxUMEntity_p);
/* SPR 5599_129119 Fix End */
/*************** AM Reordering Timer *********/
/* SPR 5599_129119 Fix Start */
UInt32 startAMROTimer(  RxAMEntity* rxAMEntity_p);
UInt32 stopAMROTimer(   RxAMEntity* rxAMEntity_p);
void restartAMROTimer( RxAMEntity* rxAMEntity_p);
/* SPR 5599_129119 Fix End */

/****************** Poll Retransmit Timer *************/
/* SPR 5599_129119 Fix Start */
UInt32 startPRTimer(  TxAMEntity* txAMEntity_p);
UInt32 stopPRTimer( TxAMEntity* txAMEntity_p);
void restartPRTimer(  TxAMEntity* txAMEntity_p);
/* SPR 5599_129119 Fix End */
/* SPR 11571 changes start */
UInt32 getPRRunningTimerPollSN(TxAMEntity* txAMEntity_p, UInt16* runningPollSN_p);
/* SPR 11571 changes end */

/* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
void putEntryInRlcTrigStatusReportQueue(UInt16 ueIndex, UInt8 lcId,InternalCellIndex cellIndex);
void processRlcTriggerStatusReport(UInt32 statusReportsQueueCount,InternalCellIndex  cellIndex);
#endif
/* - SPR 9911 Fix */
#endif  /* RLCTIMER_H */
