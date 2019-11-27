/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacSFNMgr.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
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
 * $Log: lteMacSFNMgr.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.6  2009/07/16 08:11:40  gur19413
 * changed to  fun
 *
 * Revision 1.5  2009/07/13 04:35:48  gur19413
 * added new macro for log
 *
 * Revision 1.4  2009/06/10 13:25:56  gur19413
 * comments incorporated
 *
 * Revision 1.3  2009/06/05 14:06:11  gur19413
 * remove macro
 *
 * Revision 1.2  2009/06/05 12:30:49  gur19413
 * remove compilation error
 *
 * Revision 1.1  2009/06/05 09:46:54  gur19413
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_SFN_MGR_H 
#define LTE_MAC_SFN_MGR_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/



/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
extern __thread InternalCellIndex cellIndexMac_g;

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
#define SFN_MGR     "SFN Mgr"
/* FAPI AS L2 MASTER START */
#ifdef FAPI_L2_MASTER
/* SPR 15909 fix start */
#define INVALID_TICK 0xFFFFFFFFFFFFFFFF
/* SPR 15909 fix end */
#endif
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* This function indicates the MAC-PHY Regsitration Confirmation */
void macRegistrationCnf( UInt32 sysFrameNum,
                         UInt32 subFrameNum,
                         /*CA Changes start  */
                         InternalCellIndex internalCellIndex
                         /*CA Changes end  */
                       );

/* SPR 1535 Changes Start*/
/* This function process the First Sub Frame Ind  */
/* SPR 5599 changes start (ZIP ID 129059) */
/*CA changes start*/
/* SPR 23393 Changes Start */
 void processFirstSubFrameInd( UInt32 sysFrameNum,
/* SPR 23393 Changes End */
/* SPR 5599 changes end (ZIP ID 129059) */
                                UInt32 subFrameNum,
                                InternalCellIndex internalCellIndex
                              );
/*CA changes end*/
/* SPR 1535 Changes End*/

/* This function indicate the new tick is occured */
/* SPR 5599 changes start (ZIP ID 129059) */
/*CA changes start*/
/* SPR 23393 Changes Start */
 void processSubFrameInd( UInt16 sysFrameNum,
/* SPR 23393 Changes End */
/* SPR 5599 changes end (ZIP ID 129059) */
                                UInt8 subFrameNum,
                                InternalCellIndex internalCellIndex
                              );
/*CA changes end*/
#ifdef DL_UL_SPLIT_TDD
void processSubFrameIndUl( UInt16 sysFrameNum,
                                UInt8 subFrameNum
                              );
void processFirstSubFrameIndUl(UInt16 sysFrameNum, UInt8 subFrameNum);
#endif

/* SPR 5599 changes start (ZIP ID 129059) */
void processSubFrameIndicationForTickMiss( UInt16 sysFrameNum,
/* SPR 5599 changes end (ZIP ID 129059) */
                                           UInt8 subFrameNum,
                                           InternalCellIndex  internalCellIndex
                                                 );

/* This function return the current global tick*/
/* SPR 5599 changes start (ZIP ID 129059) */

/* SPR 15909 fix start */
/* + SPR 17439 */
/* SPR 23393 Changes Start */
 tickType_t getCurrentTick(void);
/*Start Changes DL_UL_SPLIT_TDD*/
 tickType_t getCurrentTickUl(void);
/* SPR 23393 Changes End */
/* - SPR 17439 */
 void updateWithCurrentTick(tickType_t tick,InternalCellIndex internalCellIndex );
/* SPR 15909 fix end */
/*End Changes DL_UL_SPLIT_TDD*/
/* SPR 5599 changes end (ZIP ID 129059) */
/*CA changes start*/
/* SPR 15909 fix start */
/* SPR 23393 Changes Start */
tickType_t  getCurrentTickForCell(InternalCellIndex internalCellIndex);
/* SPR 23393 Changes End */
/* SPR 15909 fix end */
/*Macro for accessing the global tick of a cell*/
#define GET_CURRENT_TICK_FOR_CELL(internalCellIndex)\
    getCurrentTickForCell(internalCellIndex)
/* SPR 15909 fix start */
#define CURRENT_TICK_INIT_VALUE 0xFFFFFFFFFFFFFFFF    
/* SPR22974 Start */
#define SF_TICK_INIT_VALUE 0xFF    
/* SPR22974 End */
/* SPR 15909 fix end */
/*CA changes end*/

/*CA Changes start  */

/* This function return the Current System Frame Number*/
/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 23393 Changes Start */
UInt32 getSysFrameNumer(InternalCellIndex internalCellIndex);
/* SPR 23393 Changes End */
/* SPR 5599 changes end (ZIP ID 129059) */

/* This function return the Current Sub Frame Number */
/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 23393 Changes Start */
UInt32 getSubFrameNumber(InternalCellIndex internalCellIndex);
/* SPR 23393 Changes End */
/* SPR 5599 changes end (ZIP ID 129059) */

/* This function fills the current system and sub frame number*/
/* SPR 5599 changes start (ZIP ID 129059) */
/* SPR 23393 Changes Start */
void getSFAndSFN( UInt32* sysFrameNum_p ,
/* SPR 23393 Changes End */
/* SPR 5599 changes end (ZIP ID 129059) */
                  UInt32* subFrameNum_p,
                  /*CA Changes start  */
                  InternalCellIndex internalCellIndex
                  /*CA Changes end  */
                );
#ifdef DL_UL_SPLIT_TDD
void getUlSFAndSFN( UInt32* sysFrameNum_p ,
                  UInt32* subFrameNum_p
                  );
#endif
/* Function to fetch the global Paging cycle value */
/* SPR 5599 changes start (ZIP ID 129059) */
UInt8 getPagingCycle(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */

/* SPR 609 changes start */ 
/* CA Stats Changes Start */
#ifdef DL_UL_SPLIT
void lteMacUpdateStatsPerf(UInt32 maxStats, InternalCellIndex internalCellIndex);
#else
/* SPR 23393 Changes Start */
 void lteMacUpdateStatsPerf(InternalCellIndex internalCellIndex);
/* SPR 23393 Changes End */
#endif
/* CA Stats Changes End */
/* SPR 609 changes end */    
/* SPR 5599 changes start (ZIP ID 129059) */
void resetCurrentTick(InternalCellIndex internalCellIndex);
/* SPR 5599 changes end (ZIP ID 129059) */
/* SPR 19288 change start */
/*SPR 21001 Changes start */
/*SPR 21001 Changes end */
/* SPR 19288 change end */
/* This function initializes current tick for a cell */
/* + SPR 17439 */
void initCurrentTick(void);
/* - SPR 17439 */
/*CA Changes end  */

/* FAPI AS L2 MASTER START */
#ifdef FAPI_L2_MASTER
extern void processTickReceivedOnL2(InternalCellIndex  internalCellIndex);
/* SPR 15909 fix start */
extern tickType_t getCurrentTickFrmL2Tick(InternalCellIndex internalCellIndex);
/* SPR 15909 fix end */
extern void validateSFIFrmL1(UInt16 sysFrameNum, UInt8 subFrameNum, InternalCellIndex internalCellIndex);
extern void initL2TickBeforeELExecution(InternalCellIndex internalCellIndex);
extern void initL2TickBeforeFirstELExecution(InternalCellIndex internalCellIndex);
extern void getSFAndSFNFrmL2Tick(UInt32* sysFrameNum_p , UInt32* subFrameNum_p, InternalCellIndex  internalCellIndex);
extern void processAndValidateL2TickBeforeELExecution(InternalCellIndex  internalCellIndex);
/* SPR 15909 fix start */
extern void macFirstSubFrameIndFrmL2Tick(tickType_t currentTickFrmL2Tick);
/* SPR 15909 fix end */
#endif
/* FAPI AS L2 MASTER END */


#endif  /* LTE_MAC_SFN_MGR_H */

